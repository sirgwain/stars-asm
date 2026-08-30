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
    uint16_t       t_merge_01f2_0001;
    uint16_t       t_merge_0240_0001;
    uint16_t       t_merge_0d8d_0001;
    int16_t        t_merge_18b8_0001;
    int16_t        t_merge_1977_0001;
    int16_t        t_merge_199b_0001;
    uint16_t       t_merge_19ed_0001;

L_002a:
    /* untranslated: dOffsetPlanTemp = part[2:0](mpicolgrbitBU[game.cPlanMax]) */
    cSBDefenderFleets = 0x0;
    cFlMineLayers = 0x0;
    cMineLayers = 0x0;
    cFlArmadas = 0x0;
    cFlDestroyers = 0x0;
    lpiHistSize = vlpbAiData;
    if ((*(lpiHistSize) != 0x2))
        goto L_0097;
    else
        goto L_0071;

L_0071:
    fmemset(&(vlpbAiData), 0x0, 0x2000);
    /* untranslated: *lpiHistSize = part[2:0](mpicolgrbitBU[game.cPlanMax]) */

L_0097:
    fMarkedPlanets = 0x0;
    iroCur = IroEnsureAi(vrgAiCybertronResOrder, 0x2a, &(ishdefLatestSB), 0x11);
    EnsureCyberAiShdefs(iroCur);
    MergeAllShdefs(0x1);
    MergeAllShdefs(0x30);
    MergeAllShdefs(0xc000);
    MergeAllShdefs(0x3c0);
    MergeAllShdefs(0x3c00);
    iAttackStr = 0x1;
    if ((game.turn <= 0x32))
        goto L_0123;
    else
        goto L_0112;

L_0112:
    /* untranslated: iAttackStr = (iAttackStr + (words((game.turn + 0xffce), 0x0) / 0xa)) */

L_0123:
    if ((game.turn <= 0x64))
        goto L_0154;
    else
        goto L_012d;

L_012d:
    /* untranslated: iAttackStr = (iAttackStr + loword(((words((game.turn + 0xff9c), 0x0) / 0xa) * (words(game.turn, 0x0) / 0x64)))) */

L_0154:
    j = 0x3;
    if ((game.turn <= 0x82))
        goto L_0174;
    else
        goto L_0164;

L_0164:
    /* untranslated: j = (j + (words((game.turn + 0xff88), 0x0) / 0x14)) */

L_0174:
    if ((j <= 0x32))
        goto L_0182;
    else
        goto L_017d;

L_017d:
    j = 0x32;

L_0182:
    LOWORD(vrgAiCyberArmadaPotency) = LOBYTE(j);
    /* untranslated: part[1:2](vrgAiCyberArmadaPotency) = lobyte((sext16to32((j & 0xff)) / 0x2)) */
    j = 0x6;
    if ((game.turn <= 0x73))
        goto L_01b6;
    else
        goto L_01a6;

L_01a6:
    /* untranslated: j = (j + (words((game.turn + 0xff9c), 0x0) / 0x16)) */

L_01b6:
    if ((j <= 0xc))
        goto L_01c4;
    else
        goto L_01bf;

L_01bf:
    j = 0xc;

L_01c4:
    HIWORD(vrgAiCyberArmadaPotency) = LOBYTE(j);
    if ((0x3 >= (((uint32_t)(j) / 0x2) + 0xffff)))
        goto L_01e6;
    else
        goto L_01e0;

L_01e0:
    t_merge_01f2_0001 = 0x3;
    goto L_01f2;

L_01e6:
    t_merge_01f2_0001 = (((uint32_t)(j) / 0x2) + 0xffff);

L_01f2:
    /* untranslated: part[3:2](vrgAiCyberArmadaPotency) = lobyte(t_merge_01f2_0001) */
    memset(rgRecycleShdef, 0x0, 0x10);
    if ((game.turn >= 0x78))
        goto L_021b;
    else
        goto L_0213;

L_0213:
    cRecyclePeriod = 0x32;
    goto L_0243;

L_021b:
    if ((game.turn >= 0xc8))
        goto L_022c;
    else
        goto L_0226;

L_0226:
    t_merge_0240_0001 = 0x46;
    goto L_0240;

L_022c:
    if ((game.turn >= 0x190))
        goto L_023d;
    else
        goto L_0237;

L_0237:
    t_merge_0240_0001 = 0x64;
    goto L_0240;

L_023d:
    t_merge_0240_0001 = 0x12c;

L_0240:
    cRecyclePeriod = t_merge_0240_0001;

L_0243:
    /* untranslated: call CheckAiShdefStatus(0x4, 0x5, cRecyclePeriod, &iLatestDestroyer, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    rgRecycleShdef[iLatestDestroyer] = 0x0;
    /* untranslated: call CheckAiShdefStatus(0xe, 0xf, cRecyclePeriod, &iLatestSBDefender, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    rgRecycleShdef[iLatestDestroyer] = 0x0;
    /* untranslated: cExistCargo = sext16to32(CheckAiShdefStatus(0x2, 0x3, cRecyclePeriod, &iLatestCargo, &part[0:1](rgRecycleShdef))) */
    rgRecycleShdef[iLatestCargo] = 0x0;
    iLatestBattle = 0xffff;
    i = 0x0;
    goto L_045e;

L_02c9:
    /* untranslated: branch ((part[123:2](rgshdef[((i * 0x4) + 0x6)]) >> 0x9) & 0x1) != 0x0 ? L_03e9 : L_02f0 */

L_02f0:
    if (((game.turn - rgshdef[((i * 0x4) + 0x6)].turn) <= cRecyclePeriod))
        goto L_03e9;
    else
        goto L_0312;

L_0312:
    fScrap = 0x1;
    j = ((i * 0x4) + 0x9);
    goto L_032c;

L_0328:
    j = (j - 0x1);

L_032c:
    if ((j < ((i * 0x4) + 0x6)))
        goto L_03e9;
    else
        goto L_033e;

L_033e:
    /* untranslated: branch ((part[123:2](rgshdef[j]) >> 0x9) & 0x1) != 0x0 ? L_0328 : L_035c */

L_035c:
    /* untranslated: branch part[131:2](rgshdef[j]) != 0x0 ? L_03d5 : L_0371 */

L_0371:
    /* untranslated: branch part[133:2](rgshdef[j]) != 0x0 ? L_03d5 : L_037b */

L_037b:
    if ((fScrap != 0x0))
        goto L_0397;
    else
        goto L_0385;

L_0385:
    if ((j == ((i * 0x4) + 0x6)))
        goto L_03d5;
    else
        goto L_0397;

L_0397:
    shdef = rgshdef[j];
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), j);
    goto L_0328;

L_03d5:
    rgRecycleShdef[j] = 0x1;
    fScrap = 0x0;

L_03e9:
    /* untranslated: branch ((part[123:2](rgshdef[((i * 0x4) + 0x6)]) >> 0x9) & 0x1) != 0x0 ? L_045a : L_0410 */

L_0410:
    if ((iLatestBattle == 0xffff))
        goto L_0454;
    else
        goto L_0419;

L_0419:
    if ((rgshdef[((iLatestBattle * 0x4) + 0x6)].turn >= rgshdef[((i * 0x4) + 0x6)].turn))
        goto L_045a;
    else
        goto L_0454;

L_0454:
    iLatestBattle = i;

L_045a:
    i = (i + 0x1);

L_045e:
    if ((i <= 0x1))
        goto L_02c9;
    else
        goto L_0467;

L_0467:
    /* untranslated: LOWORD(cExistColony) = part[278:2](rgshdef) */
    /* untranslated: HIWORD(cExistColony) = part[280:2](rgshdef) */
    if ((game.turn <= 0x50))
        goto L_0505;
    else
        goto L_047e;

L_047e:
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleShdef)) -> callresult(void) */
    memset(rgRecycleSBShdef, 0x0, 0x10);
    /* untranslated: part[0:1](rgRecycleSBShdef) = 0x2 */
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleSBShdef)) -> callresult(void) */
    memset(rgRecycleSBShdef, 0x0, 0x10);
    /* untranslated: part[1:1](rgRecycleSBShdef) = 0x2 */
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleSBShdef)) -> callresult(void) */
    memset(rgRecycleSBShdef, 0x0, 0x10);
    /* untranslated: part[3:1](rgRecycleSBShdef) = 0x2 */
    /* untranslated: part[2:1](rgRecycleSBShdef) = 0x2 */
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleSBShdef)) -> callresult(void) */

L_0505:
    lpciPlanTemp = &(vlpbAiData[dOffsetPlanTemp]);
    fmemset(&(lpciPlanTemp), 0x0, (game.cPlanMax * 0x2));
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_0821;

L_0564:
    lpciPlan = ((uint8_t *)(vlpbAiData) + (0x2 + (lppl->id * 0x2)));
    lpciPlanTemp = ((uint8_t *)(vlpbAiData) + (dOffsetPlanTemp + (lppl->id * 0x2)));
    if ((lpciPlan->iPktTarget <= 0x0))
        goto L_05e3;
    else
        goto L_05bc;

L_05bc:
    /* untranslated: ss:[bp-0x8e] = ((lpciPlan->wInfo + 0xffe0) & 0x60) */
    lpciPlan->iPktTarget = 0x0;
    /* untranslated: lpciPlan->wInfo = (lpciPlan->wInfo | ss:[bp-0x8e]) */

L_05e3:
    if ((lppl->iPlayer == idPlayer))
        goto L_0686;
    else
        goto L_05f2;

L_05f2:
    if ((lppl->iPlayer == 0xffff))
        goto L_0686;
    else
        goto L_05ff;

L_05ff:
    /* untranslated: i = ((words(lppl->uPopGuess, 0x0) / 0xfa) + 0x1) */
    if ((i <= 0x6))
        goto L_0624;
    else
        goto L_061f;

L_061f:
    i = 0x6;

L_0624:
    if ((lppl->fStarbase == 0x0))
        goto L_063f;
    else
        goto L_063b;

L_063b:
    i = (i + 0x1);

L_063f:
    vlpbAiPlanet[((lppl->id * 0x10) + 0xa)] = LOBYTE(i);
    vlpbAiPlanet[((lppl->id * 0x10) + 0x9)] = 0x1;
    goto L_081d;

L_0686:
    if ((lppl->iPlayer != idPlayer))
        goto L_081d;
    else
        goto L_0695;

L_0695:
    if ((lppl->fStarbase == 0x0))
        goto L_081d;
    else
        goto L_06a9;

L_06a9:

L_06af:
    if ((lppl->isb == 0x1))
        goto L_06f7;
    else
        goto L_06c1;

L_06c1:
    if ((lppl->isb == 0x3))
        goto L_06f7;
    else
        goto L_06d3;

L_06d3:
    if ((lppl->isb == 0x6))
        goto L_06f7;
    else
        goto L_06e5;

L_06e5:
    if ((lppl->isb != 0x8))
        goto L_078a;
    else
        goto L_06f7;

L_06f7:
    if ((HIWORD(lppl->rgwtMin[0x0]) > 0x0))
        goto L_0727;
    else
        goto L_0704;

L_0704:
    if ((HIWORD(lppl->rgwtMin[0x0]) < 0x0))
        goto L_0713;
    else
        goto L_0709;

L_0709:
    if ((LOWORD(lppl->rgwtMin[0x0]) >= 0xa))
        goto L_0727;
    else
        goto L_0713;

L_0713:
    lpciPlanTemp->fNeedsMin1 = 0x1;

L_0727:
    if ((HIWORD(lppl->rgwtMin[0x1]) > 0x0))
        goto L_0757;
    else
        goto L_0734;

L_0734:
    if ((HIWORD(lppl->rgwtMin[0x1]) < 0x0))
        goto L_0743;
    else
        goto L_0739;

L_0739:
    if ((LOWORD(lppl->rgwtMin[0x1]) >= 0xa))
        goto L_0757;
    else
        goto L_0743;

L_0743:
    lpciPlanTemp->fNeedsMin2 = 0x1;

L_0757:
    if ((HIWORD(lppl->rgwtMin[0x2]) > 0x0))
        goto L_081d;
    else
        goto L_0764;

L_0764:
    if ((HIWORD(lppl->rgwtMin[0x2]) < 0x0))
        goto L_0773;
    else
        goto L_0769;

L_0769:
    if ((LOWORD(lppl->rgwtMin[0x2]) >= 0xa))
        goto L_081d;
    else
        goto L_0773;

L_0773:
    lpciPlanTemp->fNeedsMin3 = 0x1;

L_078a:
    if ((HIWORD(lppl->rgwtMin[0x0]) > 0x0))
        goto L_07bb;
    else
        goto L_0797;

L_0797:
    if ((HIWORD(lppl->rgwtMin[0x0]) < 0x0))
        goto L_07a7;
    else
        goto L_079c;

L_079c:
    if ((LOWORD(lppl->rgwtMin[0x0]) >= 0x3e8))
        goto L_07bb;
    else
        goto L_07a7;

L_07a7:
    lpciPlanTemp->fNeedsMin1 = 0x1;

L_07bb:
    if ((HIWORD(lppl->rgwtMin[0x1]) > 0x0))
        goto L_07ec;
    else
        goto L_07c8;

L_07c8:
    if ((HIWORD(lppl->rgwtMin[0x1]) < 0x0))
        goto L_07d8;
    else
        goto L_07cd;

L_07cd:
    if ((LOWORD(lppl->rgwtMin[0x1]) >= 0x3e8))
        goto L_07ec;
    else
        goto L_07d8;

L_07d8:
    lpciPlanTemp->fNeedsMin2 = 0x1;

L_07ec:
    if ((HIWORD(lppl->rgwtMin[0x2]) > 0x0))
        goto L_081d;
    else
        goto L_07f9;

L_07f9:
    if ((HIWORD(lppl->rgwtMin[0x2]) < 0x0))
        goto L_0809;
    else
        goto L_07fe;

L_07fe:
    if ((LOWORD(lppl->rgwtMin[0x2]) >= 0x3e8))
        goto L_081d;
    else
        goto L_0809;

L_0809:
    lpciPlanTemp->fNeedsMin3 = 0x1;

L_081d:
    lppl = (lppl + 0x1);

L_0821:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_0564;
    else
        goto L_082f;

L_082f:
    lpflAttack = 0x0;
    lpflEnemy = 0x0;
    ifl = 0x0;
    goto L_084f;

L_084b:
    ifl = (ifl + 0x1);

L_084f:
    if ((ifl >= cFleet))
        goto L_0b31;
    else
        goto L_085a;

L_085a:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_088a;
    else
        goto L_0882;

L_0882:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_0b31;
    else
        goto L_088a;

L_088a:
    if ((fMarkedPlanets != 0x0))
        goto L_08a6;
    else
        goto L_0894;

L_0894:
    IdNearestColonizablePlanet(lpfl, 0x0);

L_08a6:
    if ((lpfl->iPlayer == idPlayer))
        goto L_08d5;
    else
        goto L_08b5;

L_08b5:
    lpfl->lpflNext = lpflEnemy;
    lpflEnemy = lpfl;
    goto L_084b;

L_08d5:
    if ((lpfl->rgcsh[0xe] > 0x0))
        goto L_08ef;
    else
        goto L_08e2;

L_08e2:
    if ((lpfl->rgcsh[0xf] <= 0x0))
        goto L_08f6;
    else
        goto L_08ef;

L_08ef:
    cSBDefenderFleets = (cSBDefenderFleets + 0x1);
    goto L_084b;

L_08f6:
    if ((lpfl->rgcsh[0x0] <= 0x0))
        goto L_0911;
    else
        goto L_0903;

L_0903:
    cFlMineLayers = (cFlMineLayers + 0x1);
    cMineLayers = (cMineLayers + lpfl->rgcsh[0x0]);

L_0911:
    i = 0x4;
    goto L_093d;

L_0919:
    if ((lpfl->rgcsh[i] > 0x0))
        goto L_0946;
    else
        goto L_0933;

L_0933:

L_0939:
    i = (i + 0x1);

L_093d:
    if ((i <= 0xd))
        goto L_0919;
    else
        goto L_0946;

L_0946:
    if ((i > 0xd))
        goto L_0a3a;
    else
        goto L_094f;

L_094f:
    lpfl->lpflNext = lpflAttack;
    lpflAttack = lpfl;
    if ((j <= 0x5))
        goto L_0a33;
    else
        goto L_0975;

L_0975:
    if ((lpfl->cord <= 0x1))
        goto L_099d;
    else
        goto L_0982;

L_0982:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) == 0x1 ? L_09aa : L_099d */

L_099d:
    if ((lpfl->idPlanet == 0xffff))
        goto L_0a33;
    else
        goto L_09aa;

L_09aa:
    if ((lpfl->cord <= 0x1))
        goto L_09e4;
    else
        goto L_09b7;

L_09b7:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_09e4 : L_09d2 */

L_09d2:
    id = lpfl->lpplord->rgord[0x1].id;
    goto L_09ef;

L_09e4:
    id = lpfl->idPlanet;

L_09ef:
    lpb = ((uint8_t *)(vlpbAiPlanet) + ((id * 0x10) + 0xa));
    if ((*(lpb) == 0x0))
        goto L_0a2c;
    else
        goto L_0a24;

L_0a24:
    *(lpb) = (*(lpb) | 0x80);

L_0a2c:
    cFlArmadas = (cFlArmadas + 0x1);
    goto L_084b;

L_0a33:
    cFlDestroyers = (cFlDestroyers + 0x1);

L_0a3a:
    if ((lpfl->rgcsh[0x2] > 0x0))
        goto L_0a54;
    else
        goto L_0a47;

L_0a47:
    if ((lpfl->rgcsh[0x3] <= 0x0))
        goto L_084b;
    else
        goto L_0a54;

L_0a54:
    if ((lpfl->cord <= 0x1))
        goto L_084b;
    else
        goto L_0a61;

L_0a61:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < 0x0))
        goto L_084b;
    else
        goto L_0a6e;

L_0a6e:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > 0x0))
        goto L_0a7d;
    else
        goto L_0a73;

L_0a73:
    if ((LOWORD(lpfl->rgwtMin[0x3]) <= 0x0))
        goto L_084b;
    else
        goto L_0a7d;

L_0a7d:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_084b : L_0a98 */

L_0a98:
    if ((((lpciPlanTemp[lpfl->lpplord->rgord[0x1].id] >> 0x3) & 0x3) >= 0x3))
        goto L_084b;
    else
        goto L_0ac5;

L_0ac5:
    /* untranslated: ss:[bp-0x8e] = ((lpciPlanTemp[lpfl->lpplord->rgord[0x1].id] + 0x8) & 0x18) */
    lpciPlanTemp[lpfl->lpplord->rgord[0x1].id] = (lpciPlanTemp[lpfl->lpplord->rgord[0x1].id] & 0xffe7);
    /* untranslated: lpciPlanTemp[lpfl->lpplord->rgord[0x1].id] = (lpciPlanTemp[lpfl->lpplord->rgord[0x1].id] | ss:[bp-0x8e]) */

L_0b31:
    UpdateProgressGauge(0xfc62);
    ifl = 0x0;
    goto L_0b49;

L_0b45:
    ifl = (ifl + 0x1);

L_0b49:
    if ((ifl >= cFleet))
        goto L_137f;
    else
        goto L_0b54;

L_0b54:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_0b84;
    else
        goto L_0b7c;

L_0b7c:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_137f;
    else
        goto L_0b84;

L_0b84:
    if ((lpfl->iPlayer != idPlayer))
        goto L_0b45;
    else
        goto L_0b90;

L_0b90:

L_0b96:
    i = 0x0;
    goto L_0bd7;

L_0b9e:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_0bd3;
    else
        goto L_0bbb;

L_0bbb:
    if ((rgRecycleShdef[i] == 0x0))
        goto L_0be0;
    else
        goto L_0bcd;

L_0bcd:

L_0bd3:
    i = (i + 0x1);

L_0bd7:
    if ((i < 0x10))
        goto L_0b9e;
    else
        goto L_0be0;

L_0be0:
    if ((i != 0x10))
        goto L_0cca;
    else
        goto L_0be9;

L_0be9:
    if ((lpfl->idPlanet == 0xffff))
        goto L_0c93;
    else
        goto L_0bf6;

L_0bf6:
    LpplFromId(lpfl->idPlanet);
    /* untranslated: lppl = callresult(PLANET *) */
    /* untranslated: branch faroff(callresult(PLANET *)) != 0x0 ? L_0c1b : L_0c13 */

L_0c13:
    /* untranslated: branch farseg(callresult(PLANET *)) == 0x0 ? L_0c93 : L_0c1b */

L_0c1b:
    if ((lppl->iPlayer != idPlayer))
        goto L_0c93;
    else
        goto L_0c2a;

L_0c2a:
    if ((lppl->fStarbase != 0x0))
        goto L_0c55;
    else
        goto L_0c41;

L_0c41:
    if ((Random(0x5) != 0x0))
        goto L_0c93;
    else
        goto L_0c55;

L_0c55:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x5) */
    FLookupFleet(0xffff, 0x4972);
    goto L_0b45;

L_0c93:
    if ((lpfl->cord <= 0x1))
        goto L_0cad;
    else
        goto L_0ca0;

L_0ca0:
    if ((lpfl->idPlanet == 0xffff))
        goto L_0b45;
    else
        goto L_0cad;

L_0cad:
    if ((FMoveToNearestStarbase(lpfl, 0x0) != 0x0))
        goto L_0b45;
    else
        goto L_0cc4;

L_0cc4:

L_0cca:
    if ((lpfl->rgcsh[0xe] > 0x0))
        goto L_0ce4;
    else
        goto L_0cd7;

L_0cd7:
    if ((lpfl->rgcsh[0xf] <= 0x0))
        goto L_0ddc;
    else
        goto L_0ce4;

L_0ce4:
    if ((lpfl->idPlanet == 0xffff))
        goto L_0ddc;
    else
        goto L_0cf1;

L_0cf1:
    iStrDef = 0x0;
    /* untranslated: branch (game.turn - part[2183:2](rgshdef)) >= (cRecyclePeriod + 0xfff6) ? L_0d16 : L_0d0b */

L_0d0b:
    iStrDef = (iStrDef + lpfl->rgcsh[0xe]);

L_0d16:
    /* untranslated: branch (game.turn - part[2330:2](rgshdef)) >= (cRecyclePeriod + 0xfff6) ? L_0d35 : L_0d2a */

L_0d2a:
    iStrDef = (iStrDef + lpfl->rgcsh[0xf]);

L_0d35:
    if ((iStrDef <= 0x0))
        goto L_0b45;
    else
        goto L_0d3f;

L_0d3f:
    lpciPlanTemp[lpfl->idPlanet] = ((lpciPlanTemp[lpfl->idPlanet] & 0xffbf) | 0x40);
    if ((iStrDef >= (iAttackStr * 0x2)))
        goto L_0d8a;
    else
        goto L_0d84;

L_0d84:
    t_merge_0d8d_0001 = 0x1;
    goto L_0d8d;

L_0d8a:
    t_merge_0d8d_0001 = 0x0;

L_0d8d:
    lpciPlanTemp[lpfl->idPlanet] = ((lpciPlanTemp[lpfl->idPlanet] & 0xffdf) | ((t_merge_0d8d_0001 & 0x1) << 0x5));

L_0ddc:
    i = 0x4;
    goto L_0e08;

L_0de4:
    if ((lpfl->rgcsh[i] > 0x0))
        goto L_0e11;
    else
        goto L_0dfe;

L_0dfe:

L_0e04:
    i = (i + 0x1);

L_0e08:
    if ((i <= 0xd))
        goto L_0de4;
    else
        goto L_0e11;

L_0e11:
    if ((i > 0xd))
        goto L_0f55;
    else
        goto L_0e1a;

L_0e1a:
    if ((lpfl->rgcsh[0x4] > 0x0))
        goto L_0e34;
    else
        goto L_0e27;

L_0e27:
    if ((lpfl->rgcsh[0x4] <= 0x0))
        goto L_0ecb;
    else
        goto L_0e34;

L_0e34:
    if (((lpfl->rgcsh[0x4] + lpfl->rgcsh[0x4]) >= iAttackStr))
        goto L_0e58;
    else
        goto L_0e4b;

L_0e4b:
    if ((lpfl->cord <= 0x1))
        goto L_0e81;
    else
        goto L_0e58;

L_0e58:
    IdTargetAttack(lpfl, lpflAttack, lpflEnemy, game.fAisBand);

L_0e81:
    if ((lpfl->cord != 0x1))
        goto L_0b45;
    else
        goto L_0e8e;

L_0e8e:
    if ((Random(0x64) >= 0x4b))
        goto L_0b45;
    else
        goto L_0ea2;

L_0ea2:
    FFindBuddyAndJoinUp(lpfl, 0x4, 0x5, 0x64, 0xc8);

L_0ecb:
    TargetCyberArmada(lpfl);
    if ((lpfl->cord != 0x1))
        goto L_0b45;
    else
        goto L_0ee6;

L_0ee6:
    if ((Random(0x64) >= 0x4b))
        goto L_0b45;
    else
        goto L_0efa;

L_0efa:
    if ((i > 0x9))
        goto L_0f2c;
    else
        goto L_0f03;

L_0f03:
    FFindBuddyAndJoinUp(lpfl, 0x6, 0x9, 0x64, 0xc8);
    goto L_0b45;

L_0f2c:
    FFindBuddyAndJoinUp(lpfl, 0xa, 0xd, 0x64, 0xc8);

L_0f55:
    if ((lpfl->cord > 0x1))
        goto L_0b45;
    else
        goto L_0f5f;

L_0f5f:

L_0f65:
    if ((game.turn > 0x5))
        goto L_0fc4;
    else
        goto L_0f6f;

L_0f6f:
    if ((lpfl->rgcsh[0x0] <= 0x0))
        goto L_0fc4;
    else
        goto L_0f7c;

L_0f7c:
    if ((lpfl->cord != 0x1))
        goto L_0fc4;
    else
        goto L_0f89;

L_0f89:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x5) */
    FLookupFleet(0xffff, 0x4972);
    goto L_0b45;

L_0fc4:
    if ((lpfl->rgcsh[0x1] <= 0x0))
        goto L_111c;
    else
        goto L_0fd1;

L_0fd1:
    idPlanDst = IdNearestColonizablePlanet(lpfl, 0x0);
    if ((idPlanDst != 0xffff))
        goto L_104d;
    else
        goto L_0ff1;

L_0ff1:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x0]) >> 0x8) & 0xf) != 0x1 ? L_0b45 : L_100c */

L_100c:
    lpciPlanTemp[lpfl->lpplord->rgord[0x0].id] = ((lpciPlanTemp[lpfl->lpplord->rgord[0x0].id] & 0xfffe) | 0x1);

L_104d:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    if ((lpfl->idPlanet == 0xffff))
        goto L_10c2;
    else
        goto L_106c;

L_106c:
    FLookupPlanet(lpfl->idPlanet, 0x49ee);
    if ((sel.pl.iPlayer != idPlayer))
        goto L_10c2;
    else
        goto L_108b;

L_108b:
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, LOWORD(lpfl), 0x3, 0xfa);
    FLookupFleet(LOWORD(lpfl), 0x4972);

L_10c2:
    if ((idPlanDst == 0xffff))
        goto L_111c;
    else
        goto L_10cc;

L_10cc:
    if ((FColonizeAiFleet(lpfl, idPlanDst) == 0x0))
        goto L_0b45;
    else
        goto L_10e6;

L_10e6:
    if ((lpfl->cord <= 0x1))
        goto L_0b45;
    else
        goto L_10f3;

L_10f3:
    vlpbAiPlanet[((lpfl->lpplord->rgord[0x1].id * 0x10) + 0xf)] = 0x4;

L_111c:
    if ((lpfl->rgcsh[0x2] > 0x0))
        goto L_1136;
    else
        goto L_1129;

L_1129:
    if ((lpfl->rgcsh[0x3] <= 0x0))
        goto L_1188;
    else
        goto L_1136;

L_1136:
    if ((lpfl->iplan == 0x4))
        goto L_116f;
    else
        goto L_1148;

L_1148:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    sel.fl.iplan = 0x4;
    FLookupFleet(0xffff, 0x4972);

L_116f:
    DoCyberFreighter(lpfl, lpciPlanTemp);
    goto L_0b45;

L_1188:
    if ((lpfl->rgcsh[0x0] <= 0x0))
        goto L_0b45;
    else
        goto L_1195;

L_1195:
    if ((game.turn <= 0x28))
        goto L_0b45;
    else
        goto L_119f;

L_119f:
    if ((lpfl->cord != 0x1))
        goto L_0b45;
    else
        goto L_11ac;

L_11ac:
    if ((lpfl->cord <= 0x1))
        goto L_11e4;
    else
        goto L_11b9;

L_11b9:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) == 0x0 ? L_0b45 : L_11cf */

L_11cf:
    ClearAiCurrentTask(lpfl, 0x1);

L_11e4:
    if ((cFlMineLayers > 0x37))
        goto L_120a;
    else
        goto L_11ed;

L_11ed:
    if ((cFlMineLayers <= 0x28))
        goto L_123b;
    else
        goto L_11f6;

L_11f6:
    if ((Random(0x3) == 0x0))
        goto L_123b;
    else
        goto L_120a;

L_120a:
    if ((FFindBuddyAndJoinUp(lpfl, 0x0, 0x0, 0x48, 0x6c) != 0x0))
        goto L_0b45;
    else
        goto L_1235;

L_1235:

L_123b:
    if ((lpfl->rgcsh[0x0] < 0x7))
        goto L_1317;
    else
        goto L_1248;

L_1248:
    if ((Random(0x5) != 0x0))
        goto L_1317;
    else
        goto L_125c;

L_125c:
    /* untranslated: call IdRandomPlanetNearby(words(lpfl->pt.y, lpfl->pt.x), 0x69, 0x1) -> callresult(int16_t) */
    /* untranslated: idPlanDst = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) == 0xffff ? L_1317 : L_1283 */

L_1283:
    if ((idPlanDst == lpfl->idPlanet))
        goto L_1317;
    else
        goto L_1293;

L_1293:
    ClearAiCurrentTask(lpfl, 0x1);
    ord.id = idPlanDst;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[idPlanDst].x;
    ord.pt.y = rgptPlan[idPlanDst].y;
    ord.grTask = grTaskLayMines;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    FMoveAiFleet(lpfl, &(ord), 0x0);
    goto L_0b45;

L_1317:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) == 0x6 ? L_0b45 : L_132d */

L_132d:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x6) */
    /* untranslated: part[8:2](sel.fl.lpplord->rgord[0x0]) = 0x5 */
    /* untranslated: part[10:2](sel.fl.lpplord->rgord[0x0]) = 0x5 */
    FLookupFleet(0xffff, 0x4972);
    goto L_0b45;

L_137f:
    UpdateProgressGauge(0xfc62);
    ipl = 0x0;
    goto L_1397;

L_1393:
    ipl = (ipl + 0x1);

L_1397:
    if ((ipl >= vclpplAi))
        goto L_1a27;
    else
        goto L_13a2;

L_13a2:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_13d2;
    else
        goto L_13ca;

L_13ca:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_1a27;
    else
        goto L_13d2;

L_13d2:
    lpciPlan = ((uint8_t *)(vlpbAiData) + (0x2 + (lppl->id * 0x2)));
    lpciPlanTemp = ((uint8_t *)(vlpbAiData) + (dOffsetPlanTemp + (lppl->id * 0x2)));
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    fWrite = 0x0;
    lNewPop = (uint32_t)((lppl->rgwtMin[0x3] * (uint32_t)(PctTrueMaxGrowth(idPlayer))));
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0x0;
    goto L_14bb;

L_1483:
    if ((HIWORD(rgResCost[i]) < HIWORD(rgResAvail[i])))
        goto L_14b7;
    else
        goto L_14a8;

L_14a8:
    if ((HIWORD(rgResCost[i]) > HIWORD(rgResAvail[i])))
        goto LFinishProduction;
    else
        goto L_14ad;

L_14ad:
    if ((LOWORD(rgResCost[i]) > LOWORD(rgResAvail[i])))
        goto LFinishProduction;
    else
        goto L_14b1;

L_14b1:

L_14b7:
    i = (i + 0x1);

L_14bb:
    if ((i < 0x4))
        goto L_1483;
    else
        goto L_14c4;

L_14c4:
    pctValue = PctPlanetDesirability(lppl, idPlayer);
    pctValueIdeal = PctPlanetOptValue(lppl, idPlayer);
    if ((pctValue >= 0xa))
        goto L_153c;
    else
        goto L_14f7;

L_14f7:
    /* untranslated: i = (loword((int32_t)(words((part[14:2](rgResAvail) - part[14:2](rgResCost)), (part[12:2](rgResAvail) - part[12:2](rgResCost))) / 0x46)) +
     * 0x1) */
    AddItemToQueue(0xc, i, grobjPlanet, 0x1);
    fWrite = 0x1;
    goto LFinishProduction;

L_153c:
    if ((pctValue >= pctValueIdeal))
        goto L_1586;
    else
        goto L_1547;

L_1547:
    /* untranslated: branch (part[14:2](rgResAvail) - part[14:2](rgResCost)) < 0x0 ? L_1586 : L_155b */

L_155b:
    /* untranslated: branch (part[14:2](rgResAvail) - part[14:2](rgResCost)) > 0x0 ? L_1568 : L_1560 */

L_1560:
    /* untranslated: branch (part[12:2](rgResAvail) - part[12:2](rgResCost)) <= 0x46 ? L_1586 : L_1568 */

L_1568:
    AddItemToQueue(0xc, 0x1, grobjPlanet, 0x1);
    fWrite = 0x1;

L_1586:
    if ((lppl->fStarbase == 0x0))
        goto LFinishProduction;
    else
        goto L_159a;

L_159a:

L_15a0:
    if ((lppl->isb == 0x1))
        goto LFinishProduction;
    else
        goto L_15b2;

L_15b2:
    if ((lppl->isb == 0x3))
        goto LFinishProduction;
    else
        goto L_15c4;

L_15c4:
    if ((lppl->isb == 0x6))
        goto LFinishProduction;
    else
        goto L_15d6;

L_15d6:
    if ((lppl->isb == 0x8))
        goto LFinishProduction;
    else
        goto L_15e5;

L_15e5:

L_15eb:
    if ((lpciPlan->fBltColony == 0x0))
        goto L_161a;
    else
        goto L_1602;

L_1602:
    if ((HIWORD(lNewPop) < 0x0))
        goto L_16c6;
    else
        goto L_160b;

L_160b:
    if ((HIWORD(lNewPop) > 0x0))
        goto L_161a;
    else
        goto L_1610;

L_1610:
    if ((LOWORD(lNewPop) <= 0x157c))
        goto L_16c6;
    else
        goto L_161a;

L_161a:
    if ((lpciPlanTemp->fIdleColonizers != 0x0))
        goto L_16c6;
    else
        goto L_162c;

L_162c:
    if ((HIWORD(cExistColony) > 0x0))
        goto L_16c6;
    else
        goto L_1635;

L_1635:
    if ((HIWORD(cExistColony) < 0x0))
        goto L_1643;
    else
        goto L_163a;

L_163a:
    if ((LOWORD(cExistColony) >= 0x28))
        goto L_16c6;
    else
        goto L_1643;

L_1643:
    if ((FShouldPlanetBuildColonizer(lppl) == 0x0))
        goto L_16c6;
    else
        goto L_1659;

L_1659:
    AddItemToQueue(0x1, 0x1, grobjFleet, 0x1);
    lpciPlan->fBltColony = 0x1;
    fWrite = 0x1;
    if ((HIWORD(lNewPop) < 0x0))
        goto L_16d8;
    else
        goto L_1692;

L_1692:
    if ((HIWORD(lNewPop) > 0x0))
        goto L_16a1;
    else
        goto L_1697;

L_1697:
    if ((LOWORD(lNewPop) <= 0x3a98))
        goto L_16d8;
    else
        goto L_16a1;

L_16a1:
    if ((game.turn >= 0x64))
        goto L_16d8;
    else
        goto L_16ab;

L_16ab:
    AddItemToQueue(0x1, 0x1, grobjFleet, 0x1);

L_16c6:
    lpciPlan->fBltColony = 0x0;

L_16d8:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_1777;
    else
        goto L_16e5;

L_16e5:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_16f5;
    else
        goto L_16ea;

L_16ea:
    if ((LOWORD(lppl->rgwtMin[0x3]) <= 0x7d0))
        goto L_1777;
    else
        goto L_16f5;

L_16f5:
    /* untranslated: branch ((part[417:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_1777 : L_1708 */

L_1708:
    if ((HIWORD(cExistCargo) > 0x0))
        goto L_1777;
    else
        goto L_1711;

L_1711:
    if ((HIWORD(cExistCargo) < 0x0))
        goto L_171f;
    else
        goto L_1716;

L_1716:
    if ((LOWORD(cExistCargo) >= 0x32))
        goto L_1777;
    else
        goto L_171f;

L_171f:
    if ((lpciPlanTemp->cIdleFreighters >= 0x1))
        goto L_1777;
    else
        goto L_1733;

L_1733:
    if ((FAROFF(LpplFindClosestEnum(lppl, FEnumDropOffStage2)) != 0x0))
        goto L_1759;
    else
        goto L_1751;

L_1751:
    /* untranslated: branch farseg(callresult(PLANET *)) == 0x0 ? L_1777 : L_1759 */

L_1759:
    AddItemToQueue(iLatestCargo, 0x1, grobjFleet, 0x1);
    fWrite = 0x1;

L_1777:
    if ((LOWORD(rgshdef) != 0x5))
        goto L_1893;
    else
        goto L_1781;

L_1781:
    if ((Random(0x4) != 0x0))
        goto L_1893;
    else
        goto L_1795;

L_1795:
    if ((cMineLayers >= 0x2710))
        goto L_1893;
    else
        goto L_179f;

L_179f:
    id = lppl->id;
    cFr = 0x0;
    ifl = 0x0;
    goto L_17bb;

L_17b7:
    ifl = (ifl + 0x1);

L_17bb:
    if ((ifl >= cFleet))
        goto L_1833;
    else
        goto L_17c6;

L_17c6:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_17f6;
    else
        goto L_17ee;

L_17ee:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_1833;
    else
        goto L_17f6;

L_17f6:
    if ((lpfl->idPlanet != id))
        goto L_17b7;
    else
        goto L_1806;

L_1806:
    if ((lpfl->rgcsh[0x0] <= 0x0))
        goto L_17b7;
    else
        goto L_1813;

L_1813:
    if ((lpfl->iPlayer != idPlayer))
        goto L_17b7;
    else
        goto L_1822;

L_1822:
    cFr = lpfl->rgcsh[0x0];
    goto L_1833;

L_1833:
    if ((cFr < 0xa))
        goto L_185b;
    else
        goto L_183d;

L_183d:
    if ((cFr >= 0x11))
        goto L_1893;
    else
        goto L_1847;

L_1847:
    if ((Random(0xa) != 0x0))
        goto L_1893;
    else
        goto L_185b;

L_185b:
    /* untranslated: branch Random(part[1:0](mpicolgrbitBU[cFr])) != 0x0 ? L_1893 : L_1875 */

L_1875:
    AddItemToQueue(0x0, 0x4, grobjFleet, 0x1);
    fWrite = 0x1;

L_1893:
    if ((lpciPlanTemp->fNeedDefenders == 0x0))
        goto L_18b5;
    else
        goto L_18af;

L_18af:
    t_merge_18b8_0001 = iLatestSBDefender;
    goto L_18b8;

L_18b5:
    t_merge_18b8_0001 = 0xffff;

L_18b8:
    iSBDef = t_merge_18b8_0001;
    if ((iSBDef != 0xffff))
        goto L_199f;
    else
        goto L_18c6;

L_18c6:
    if ((lpciPlanTemp->fDefended != 0x0))
        goto L_199f;
    else
        goto L_18dd;

L_18dd:
    if ((cSBDefenderFleets >= 0x28))
        goto L_199f;
    else
        goto L_18e6;

L_18e6:
    lpplEnemy = LpplFindClosestEnum(lppl, FEnumCalcEnemyPlanets);
    if ((LOWORD(lpplEnemy) != 0x0))
        goto L_1918;
    else
        goto L_190e;

L_190e:
    if ((HIWORD(lpplEnemy) == 0x0))
        goto L_195a;
    else
        goto L_1918;

L_1918:
    /* untranslated: branch hiword(LDistance2(words(rgptPlan[lppl->id].y, rgptPlan[lppl->id].x), words(rgptPlan[lpplEnemy->id].y, rgptPlan[lpplEnemy->id].x))) <
     * 0x1 ? L_197e : L_194d */

L_194d:
    /* untranslated: branch hiword(callresult(int32_t)) > 0x1 ? L_195a : L_1952 */

L_1952:
    /* untranslated: branch loword(callresult(int32_t)) <= 0x5f90 ? L_197e : L_195a */

L_195a:
    if ((Random(0x64) >= 0xa))
        goto L_1974;
    else
        goto L_196e;

L_196e:
    t_merge_1977_0001 = iLatestSBDefender;
    goto L_1977;

L_1974:
    t_merge_1977_0001 = 0xffff;

L_1977:
    iSBDef = t_merge_1977_0001;
    goto L_199f;

L_197e:
    if ((Random(0x64) >= 0x32))
        goto L_1998;
    else
        goto L_1992;

L_1992:
    t_merge_199b_0001 = iLatestSBDefender;
    goto L_199b;

L_1998:
    t_merge_199b_0001 = 0xffff;

L_199b:
    iSBDef = t_merge_199b_0001;

L_199f:
    if ((cFlDestroyers <= 0x78))
        goto L_19ad;
    else
        goto L_19a8;

L_19a8:
    iLatestDestroyer = 0xffff;

L_19ad:
    if ((cFlArmadas <= 0xfa))
        goto L_19bc;
    else
        goto L_19b7;

L_19b7:
    iLatestBattle = 0xffff;

L_19bc:
    iBuilt = iAddAttackFleet(lppl, iAttackStr, iLatestDestroyer, iLatestBattle, iSBDef);
    if ((iBuilt == 0x0))
        goto L_19ea;
    else
        goto L_19e4;

L_19e4:
    t_merge_19ed_0001 = 0x1;
    goto L_19ed;

L_19ea:
    t_merge_19ed_0001 = 0x0;

L_19ed:
    fWrite = (fWrite | t_merge_19ed_0001);
    if ((iBuilt != 0x1))
        goto L_19fe;
    else
        goto L_19fa;

L_19fa:
    cFlArmadas = (cFlArmadas + 0x1);

L_19fe:
    if ((iBuilt != 0x2))
        goto L_1a0b;
    else
        goto L_1a07;

L_1a07:
    cSBDefenderFleets = (cSBDefenderFleets + 0x1);

L_1a0b:
    if ((iBuilt != 0x3))
        goto LFinishProduction;
    else
        goto L_1a14;

L_1a14:
    cFlDestroyers = (cFlDestroyers + 0x1);

LFinishProduction:
    FinishProduction(fWrite);
    goto L_1393;

L_1a27:
    UpdateProgressGauge(0xfc62);
    HandleBasicAiTasks(iroCur, rgprod, IshdefAiSBLatest(), rgResAvail, rgResCost);
    UpdateProgressGauge(0xfc62);
    DoCyberPackets();
    UpdateProgressGauge(0xfc62);
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
    int16_t        t_merge_1d7f_0001;
    int16_t        t_merge_1e0e_0001;
    int16_t        t_merge_1e9d_0001;
    int16_t        t_merge_1f4c_0001;
    int32_t        t_merge_20f9_0001_wide;
    int32_t        t_merge_2361_0001_wide;
    uint16_t       t_merge_273d_0001;

L_1a78:
    /* untranslated: dOffsetPlanTemp = part[2:0](mpicolgrbitBU[game.cPlanMax]) */
    ipl = 0x0;
    goto L_1a99;

L_1a95:
    ipl = (ipl + 0x1);

L_1a99:
    if ((ipl >= vclpplAi))
        goto L_2823;
    else
        goto L_1aa4;

L_1aa4:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_1ad4;
    else
        goto L_1acc;

L_1acc:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_2823;
    else
        goto L_1ad4;

L_1ad4:
    if ((lppl->fStarbase == 0x0))
        goto L_1a95;
    else
        goto L_1ae8;

L_1ae8:

L_1aee:
    lpciPlan = ((uint8_t *)(vlpbAiData) + (0x2 + (lppl->id * 0x2)));
    lpciPlanTemp = ((uint8_t *)(vlpbAiData) + (dOffsetPlanTemp + (lppl->id * 0x2)));
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    fWrite = 0x0;
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    if ((lpciPlan->fNeedScanPkt != 0x0))
        goto L_1fa3;
    else
        goto L_1b8a;

L_1b8a:
    if ((lppl->isb == 0x1))
        goto L_1bd2;
    else
        goto L_1b9c;

L_1b9c:
    if ((lppl->isb == 0x3))
        goto L_1bd2;
    else
        goto L_1bae;

L_1bae:
    if ((lppl->isb == 0x6))
        goto L_1bd2;
    else
        goto L_1bc0;

L_1bc0:
    if ((lppl->isb != 0x8))
        goto L_1fa3;
    else
        goto L_1bd2;

L_1bd2:
    if ((HIWORD(lppl->rgwtMin[0x0]) > 0x0))
        goto L_1c29;
    else
        goto L_1bdf;

L_1bdf:
    if ((HIWORD(lppl->rgwtMin[0x0]) < 0x0))
        goto L_1bef;
    else
        goto L_1be4;

L_1be4:
    if ((LOWORD(lppl->rgwtMin[0x0]) > 0x2bc))
        goto L_1c29;
    else
        goto L_1bef;

L_1bef:
    if ((HIWORD(lppl->rgwtMin[0x1]) > 0x0))
        goto L_1c29;
    else
        goto L_1bfc;

L_1bfc:
    if ((HIWORD(lppl->rgwtMin[0x1]) < 0x0))
        goto L_1c0c;
    else
        goto L_1c01;

L_1c01:
    if ((LOWORD(lppl->rgwtMin[0x1]) > 0x2bc))
        goto L_1c29;
    else
        goto L_1c0c;

L_1c0c:
    if ((HIWORD(lppl->rgwtMin[0x2]) < 0x0))
        goto L_1fa3;
    else
        goto L_1c19;

L_1c19:
    if ((HIWORD(lppl->rgwtMin[0x2]) > 0x0))
        goto L_1c29;
    else
        goto L_1c1e;

L_1c1e:
    if ((LOWORD(lppl->rgwtMin[0x2]) <= 0x2bc))
        goto L_1fa3;
    else
        goto L_1c29;

L_1c29:
    /* untranslated: iPacketMax = loword((int32_t)(words(part[14:2](rgResAvail), part[12:2](rgResAvail)) / 0x2)) */
    iPacketMax = ((uint32_t)(iPacketMax) / 0x5);
    if ((iPacketMax >= 0x7))
        goto L_1c65;
    else
        goto L_1c58;

L_1c58:
    lpplDst = 0x0;
    goto L_1c81;

L_1c65:
    lpplDst = LpplFindClosestEnum(lppl, FEnumNeedMinerals);

L_1c81:
    if ((LOWORD(lpplDst) != 0x0))
        goto L_1c93;
    else
        goto L_1c8a;

L_1c8a:
    if ((HIWORD(lpplDst) == 0x0))
        goto L_1fa3;
    else
        goto L_1c93;

L_1c93:
    lpciPlanDst = ((uint8_t *)(vlpbAiData) + (0x2 + (lpplDst->id * 0x2)));
    lpciPlanT = ((uint8_t *)(vlpbAiData) + (dOffsetPlanTemp + (lpplDst->id * 0x2)));
    iPacketAdd = 0x0;
    if ((lpplDst->isb == 0x1))
        goto L_1d20;
    else
        goto L_1cea;

L_1cea:
    if ((lpplDst->isb == 0x3))
        goto L_1d20;
    else
        goto L_1cfc;

L_1cfc:
    if ((lpplDst->isb == 0x6))
        goto L_1d20;
    else
        goto L_1d0e;

L_1d0e:
    if ((lpplDst->isb != 0x8))
        goto L_1d29;
    else
        goto L_1d20;

L_1d20:
    iMinLimit = 0xa;
    goto L_1d2f;

L_1d29:
    iMinLimit = 0x3e8;

L_1d2f:
    if ((HIWORD(lpplDst->rgwtMin[0x0]) > SIGNHIWORD(iMinLimit)))
        goto L_1dbe;
    else
        goto L_1d40;

L_1d40:
    if ((HIWORD(lpplDst->rgwtMin[0x0]) < SIGNHIWORD(iMinLimit)))
        goto L_1d4e;
    else
        goto L_1d45;

L_1d45:
    if ((LOWORD(lpplDst->rgwtMin[0x0]) >= iMinLimit))
        goto L_1dbe;
    else
        goto L_1d4e;

L_1d4e:
    if ((HIWORD(lppl->rgwtMin[0x0]) < 0x0))
        goto L_1dbe;
    else
        goto L_1d5b;

L_1d5b:
    if ((HIWORD(lppl->rgwtMin[0x0]) > 0x0))
        goto L_1d6b;
    else
        goto L_1d60;

L_1d60:
    if ((LOWORD(lppl->rgwtMin[0x0]) <= 0x2bc))
        goto L_1dbe;
    else
        goto L_1d6b;

L_1d6b:
    if ((iPacketMax >= 0x7))
        goto L_1d7c;
    else
        goto L_1d75;

L_1d75:
    t_merge_1d7f_0001 = iPacketMax;
    goto L_1d7f;

L_1d7c:
    t_merge_1d7f_0001 = 0x7;

L_1d7f:
    iPacketAdd = t_merge_1d7f_0001;
    AddItemToQueue(0xe, iPacketAdd, grobjPlanet, 0x0);
    iPacketMax = (iPacketMax - iPacketAdd);
    lpciPlanT->fNeedsMin1 = 0x0;
    fWrite = 0x1;

L_1dbe:
    if ((HIWORD(lpplDst->rgwtMin[0x1]) > SIGNHIWORD(iMinLimit)))
        goto L_1e4d;
    else
        goto L_1dcf;

L_1dcf:
    if ((HIWORD(lpplDst->rgwtMin[0x1]) < SIGNHIWORD(iMinLimit)))
        goto L_1ddd;
    else
        goto L_1dd4;

L_1dd4:
    if ((LOWORD(lpplDst->rgwtMin[0x1]) >= iMinLimit))
        goto L_1e4d;
    else
        goto L_1ddd;

L_1ddd:
    if ((HIWORD(lppl->rgwtMin[0x1]) < 0x0))
        goto L_1e4d;
    else
        goto L_1dea;

L_1dea:
    if ((HIWORD(lppl->rgwtMin[0x1]) > 0x0))
        goto L_1dfa;
    else
        goto L_1def;

L_1def:
    if ((LOWORD(lppl->rgwtMin[0x1]) <= 0x2bc))
        goto L_1e4d;
    else
        goto L_1dfa;

L_1dfa:
    if ((iPacketMax >= 0x7))
        goto L_1e0b;
    else
        goto L_1e04;

L_1e04:
    t_merge_1e0e_0001 = iPacketMax;
    goto L_1e0e;

L_1e0b:
    t_merge_1e0e_0001 = 0x7;

L_1e0e:
    iPacketAdd = t_merge_1e0e_0001;
    AddItemToQueue(0xf, iPacketAdd, grobjPlanet, 0x0);
    iPacketMax = (iPacketMax - iPacketAdd);
    lpciPlanT->fNeedsMin2 = 0x0;
    fWrite = 0x1;

L_1e4d:
    if ((HIWORD(lpplDst->rgwtMin[0x2]) > SIGNHIWORD(iMinLimit)))
        goto L_1edc;
    else
        goto L_1e5e;

L_1e5e:
    if ((HIWORD(lpplDst->rgwtMin[0x2]) < SIGNHIWORD(iMinLimit)))
        goto L_1e6c;
    else
        goto L_1e63;

L_1e63:
    if ((LOWORD(lpplDst->rgwtMin[0x2]) >= iMinLimit))
        goto L_1edc;
    else
        goto L_1e6c;

L_1e6c:
    if ((HIWORD(lppl->rgwtMin[0x2]) < 0x0))
        goto L_1edc;
    else
        goto L_1e79;

L_1e79:
    if ((HIWORD(lppl->rgwtMin[0x2]) > 0x0))
        goto L_1e89;
    else
        goto L_1e7e;

L_1e7e:
    if ((LOWORD(lppl->rgwtMin[0x2]) <= 0x2bc))
        goto L_1edc;
    else
        goto L_1e89;

L_1e89:
    if ((iPacketMax >= 0x7))
        goto L_1e9a;
    else
        goto L_1e93;

L_1e93:
    t_merge_1e9d_0001 = iPacketMax;
    goto L_1e9d;

L_1e9a:
    t_merge_1e9d_0001 = 0x7;

L_1e9d:
    iPacketAdd = t_merge_1e9d_0001;
    AddItemToQueue(0x10, iPacketAdd, grobjPlanet, 0x0);
    iPacketMax = (iPacketMax - iPacketAdd);
    lpciPlanT->fNeedsMin3 = 0x0;
    fWrite = 0x1;

L_1edc:
    if ((fWrite == 0x0))
        goto LFinish;
    else
        goto L_1ee6;

L_1ee6:
    iWarpSrc = (IWarpMAFromLppl(lppl, &(fTwoMA)) + 0xfffc);
    if ((fTwoMA == 0x0))
        goto L_1f0d;
    else
        goto L_1f08;

L_1f08:
    iWarpSrc = (iWarpSrc + 0x1);

L_1f0d:
    iWarpDst = (IWarpMAFromLppl(lpplDst, &(fTwoMA)) + 0xfffc);
    if ((fTwoMA == 0x0))
        goto L_1f34;
    else
        goto L_1f2f;

L_1f2f:
    iWarpDst = (iWarpDst + 0x1);

L_1f34:
    if ((iWarpSrc >= iWarpDst))
        goto L_1f48;
    else
        goto L_1f41;

L_1f41:
    t_merge_1f4c_0001 = iWarpSrc;
    goto L_1f4c;

L_1f48:
    t_merge_1f4c_0001 = iWarpDst;

L_1f4c:
    /* untranslated: part[198:2](sel) = ((part[198:2](sel) & 0xc3ff) | ((t_merge_1f4c_0001 & 0xf) << 0xa)) */
    /* untranslated: part[198:2](sel) = ((part[198:2](sel) & 0xfc00) | ((lpplDst->id + 0x1) & 0x3ff)) */
    FLookupPlanet(0xffff, 0x49ee);

L_1fa3:
    if ((lpciPlan->fNeedScanPkt != 0x0))
        goto L_25dc;
    else
        goto L_1fb9;

L_1fb9:
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) > 0x1 ? L_200b : L_1fd8 */

L_1fd8:
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) != 0x1 ? L_25dc : L_1ff7 */

L_1ff7:
    if ((Random(0x3) != 0x0))
        goto L_25dc;
    else
        goto L_200b;

L_200b:
    plMinMax = ((uint8_t *)(vlpbAiData) + (dOffsetPlanTemp + (game.cPlanMax * 0x2)));
    LOWORD(rgResAvail) = (LOWORD(rgResAvail) - LOWORD(rgResCost));
    HIWORD(rgResAvail) = (HIWORD(rgResAvail) - HIWORD(rgResCost));
    /* untranslated: part[4:2](rgResAvail) = (part[4:2](rgResAvail) - part[4:2](rgResCost)) */
    /* untranslated: part[6:2](rgResAvail) = (part[6:2](rgResAvail) - part[6:2](rgResCost)) */
    /* untranslated: part[8:2](rgResAvail) = (part[8:2](rgResAvail) - part[8:2](rgResCost)) */
    /* untranslated: part[10:2](rgResAvail) = (part[10:2](rgResAvail) - part[10:2](rgResCost)) */
    /* untranslated: LOWORD(lMineral) = (((LOWORD(rgResAvail) + 0xffba) + (part[4:2](rgResAvail) + 0xffba)) + (part[8:2](rgResAvail) + 0xffba)) */
    /* untranslated: HIWORD(lMineral) = (((HIWORD(rgResAvail) + 0xffff) + (part[6:2](rgResAvail) + 0xffff)) + (part[10:2](rgResAvail) + 0xffff)) */
    /* untranslated: cResLeft = loword((int32_t)(words(part[14:2](rgResAvail), part[12:2](rgResAvail)) / 0x2)) */
    lPackets = (uint32_t)(((uint32_t)((cResLeft + 0xfffb)) / 0x5));
    if ((HIWORD(lMineral) > HIWORD((uint32_t)((lPackets * 0x46)))))
        goto L_20e4;
    else
        goto L_20cb;

L_20cb:
    /* untranslated: branch HIWORD(lMineral) < hiword(callresult(uint32_t)) ? L_20d9 : L_20d0 */

L_20d0:
    /* untranslated: branch LOWORD(lMineral) >= loword(callresult(uint32_t)) ? L_20e4 : L_20d9 */

L_20d9:
    t_merge_20f9_0001_wide = lMineral;
    goto L_20f9;

L_20e4:
    /* untranslated: t_merge_20f9_0001_wide = callresult(uint32_t) */

L_20f9:
    LOWORD(plMinMax) = LOWORD(t_merge_20f9_0001_wide);
    *(plMinMax + 0x2) = HIWORD(t_merge_20f9_0001_wide);
    if ((HIWORD(lMineral) < 0x0))
        goto L_213d;
    else
        goto L_210e;

L_210e:
    if ((HIWORD(lMineral) > 0x0))
        goto L_211e;
    else
        goto L_2113;

L_2113:
    if ((LOWORD(lMineral) <= 0x96))
        goto L_213d;
    else
        goto L_211e;

L_211e:
    lpplDst = LpplFindClosestEnum(lppl, FEnumPktAttack);
    goto L_2147;

L_213d:
    lpplDst = 0x0;

L_2147:
    if ((LOWORD(lpplDst) != 0x0))
        goto L_2159;
    else
        goto L_2150;

L_2150:
    if ((HIWORD(lpplDst) == 0x0))
        goto L_25dc;
    else
        goto L_2159;

L_2159:
    LOWORD(cPacket) = 0x0;
    HIWORD(cPacket) = 0x0;
    /* untranslated: part[4:2](cPacket) = 0x0 */
    lpciPlanDst = ((uint8_t *)(vlpbAiData) + (0x2 + (lpplDst->id * 0x2)));
    lpciPlanT = ((uint8_t *)(vlpbAiData) + (dOffsetPlanTemp + (lpplDst->id * 0x2)));
    iPacketAdd = 0x0;
    if ((lpplDst->fStarbase == 0x0))
        goto L_21ec;
    else
        goto L_21c7;

L_21c7:
    iWarpDst = IWarpMAFromLppl(lpplDst, &(fTwoMA));
    if ((fTwoMA == 0x0))
        goto L_21f1;
    else
        goto L_21e5;

L_21e5:
    iWarpDst = (iWarpDst + 0x1);

L_21ec:
    iWarpDst = 0x0;

L_21f1:
    iWarp = (IWarpMAFromLppl(lppl, &(fTwoMA)) + 0x3);
    /* untranslated: ss:[bp-0x184] = loword((iWarp * iWarp)) */
    /* untranslated: ss:[bp-0x182] = signhiword(loword((iWarp * iWarp))) */
    /* untranslated: dDistance = (double)dword ss:[bp-0x184] */
    dDistanceTgt = DGetDistance(rgptPlan[lppl->id].x, rgptPlan[lppl->id].y, rgptPlan[lpplDst->id].x, rgptPlan[lpplDst->id].y);
    /* untranslated: ss:[bp-0x186] = loword(((loword((iWarp * iWarp)) - loword((iWarpDst * iWarpDst))) * (0x64 - (lpplDst->uDefGuess + 0x5)))) */
    /* untranslated: ss:[bp-0x184] = 0x0 */
    /* untranslated: dMod = ((double)dword ss:[bp-0x186] / 16000) */
    if ((0x3e8 >= ((lpplDst->uPopGuess + 0x19) * 0x4)))
        goto L_22fa;
    else
        goto L_22e2;

L_22e2:
    goto L_231d;

L_22fa:
    /* untranslated: ss:[bp-0x18c] = ((lpplDst->uPopGuess + 0x19) * 0x4) */
    /* untranslated: ss:[bp-0x18a] = 0x0 */

L_231d:
    lMinNeeded = __ftol();
    if ((HIWORD(lMinNeeded) > HIWORD(lMineral)))
        goto L_2359;
    else
        goto L_2340;

L_2340:
    if ((HIWORD(lMinNeeded) < HIWORD(lMineral)))
        goto L_234e;
    else
        goto L_2345;

L_2345:
    if ((LOWORD(lMinNeeded) >= LOWORD(lMineral)))
        goto L_2359;
    else
        goto L_234e;

L_234e:
    t_merge_2361_0001_wide = lMinNeeded;
    goto L_2361;

L_2359:
    t_merge_2361_0001_wide = lMineral;

L_2361:
    lMineral = t_merge_2361_0001_wide;
    if ((fTwoMA == 0x0))
        goto L_23c7;
    else
        goto L_2372;

L_2372:
    /* untranslated: 10-byte ss:[bp-0x186] = (double)lMineral */
    pow(0.875, (dDistanceTgt / dDistance));
    lMineral = __ftol();
    goto L_24a9;

L_23c7:
    /* untranslated: 10-byte ss:[bp-0x186] = (double)lMineral */
    pow(0.75, (dDistanceTgt / dDistance));
    lMineral = __ftol();

L_241c:
    iMin = 0x0;
    /* untranslated: branch HIWORD(rgResAvail[iMin]) > part[6:2](rgResAvail) ? L_244f : L_243d */

L_243d:
    /* untranslated: branch HIWORD(rgResAvail[iMin]) < part[6:2](rgResAvail) ? L_2449 : L_2442 */

L_2442:
    /* untranslated: branch LOWORD(rgResAvail[iMin]) >= part[4:2](rgResAvail) ? L_244f : L_2449 */

L_2449:
    iMin = 0x1;

L_244f:
    /* untranslated: branch HIWORD(rgResAvail[iMin]) > part[10:2](rgResAvail) ? L_247c : L_246a */

L_246a:
    /* untranslated: branch HIWORD(rgResAvail[iMin]) < part[10:2](rgResAvail) ? L_2476 : L_246f */

L_246f:
    /* untranslated: branch LOWORD(rgResAvail[iMin]) >= part[8:2](rgResAvail) ? L_247c : L_2476 */

L_2476:
    iMin = 0x2;

L_247c:
    cPacket[iMin] = (cPacket[iMin] + 0x1);
    rgResAvail[iMin] = (rgResAvail[iMin] - 0x46);
    lMineral = (lMineral - 0x46);

L_24a9:
    if ((HIWORD(lMineral) > 0x0))
        goto L_241c;
    else
        goto L_24b3;

L_24b3:
    if ((HIWORD(lMineral) < 0x0))
        goto L_24c2;
    else
        goto L_24b8;

L_24b8:
    if ((LOWORD(lMineral) > 0x0))
        goto L_241c;
    else
        goto L_24c2;

L_24c2:
    if ((LOWORD(cPacket) <= 0x0))
        goto L_24e5;
    else
        goto L_24cc;

L_24cc:
    AddItemToQueue(0xe, LOWORD(cPacket), grobjPlanet, 0x0);

L_24e5:
    if ((HIWORD(cPacket) <= 0x0))
        goto L_2508;
    else
        goto L_24ef;

L_24ef:
    AddItemToQueue(0xf, HIWORD(cPacket), grobjPlanet, 0x0);

L_2508:
    /* untranslated: branch part[4:2](cPacket) <= 0x0 ? L_252b : L_2512 */

L_2512:
    /* untranslated: call AddItemToQueue(0x10, part[4:2](cPacket), grobjPlanet, 0x0) -> callresult(void) */

L_252b:
    /* untranslated: part[198:2](sel) = ((part[198:2](sel) & 0xc3ff) | (((iWarp + 0xfffc) & 0xf) << 0xa)) */
    /* untranslated: part[198:2](sel) = ((part[198:2](sel) & 0xfc00) | ((lpplDst->id + 0x1) & 0x3ff)) */
    FLookupPlanet(0xffff, 0x49ee);
    lpciPlanDst->iPktTarget = 0x3;
    __aFfcompp();
    /* untranslated: branch JNC ? L_25c1 : L_25af */

L_25af:
    lpciPlan->fNeedScanPkt = 0x1;

L_25c1:
    lpciPlan->fLaunchedPkt = 0x0;
    fWrite = 0x1;
    goto LFinish;

L_25dc:
    if ((lpciPlan->fLaunchedPkt == 0x0))
        goto L_260b;
    else
        goto L_25f5;

L_25f5:
    if ((lpciPlan->fNeedScanPkt == 0x0))
        goto L_2802;
    else
        goto L_260b;

L_260b:
    if ((lpciPlan->fNeedScanPkt != 0x0))
        goto L_269b;
    else
        goto L_2621;

L_2621:
    i = Random(0x7);
    if ((i != lpciPlan->iLstPktDir))
        goto L_2668;
    else
        goto L_2641;

L_2641:
    lpciPlan->wInfo = ((lpciPlan->wInfo & 0xfff8) | ((i + 0x1) & 0x7));
    goto L_267f;

L_2668:
    lpciPlan->wInfo = ((lpciPlan->wInfo & 0xfff8) | (i & 0x7));

L_267f:
    idPlanDst = IdGetBestScannerDest(lppl, lpciPlan->iLstPktDir);

L_269b:
    if ((lpciPlan->fNeedScanPkt != 0x0))
        goto L_26bb;
    else
        goto L_26b1;

L_26b1:
    if ((idPlanDst == 0xffff))
        goto LFinish;
    else
        goto L_26bb;

L_26bb:
    lpciPlanDst = ((uint8_t *)(vlpbAiData) + (0x2 + (idPlanDst * 0x2)));
    if ((lpciPlanDst->iPktTarget != 0x0))
        goto L_276c;
    else
        goto L_26f4;

L_26f4:
    if ((FAddPacketToQueue(lppl) == 0x0))
        goto L_276c;
    else
        goto L_270a;

L_270a:
    lpciPlanDst->iPktTarget = 0x3;
    if ((lpciPlan->fNeedScanPkt != 0x0))
        goto L_273a;
    else
        goto L_2734;

L_2734:
    t_merge_273d_0001 = 0x1;
    goto L_273d;

L_273a:
    t_merge_273d_0001 = 0x0;

L_273d:
    lpciPlan->wInfo = ((lpciPlan->wInfo & 0xffef) | ((t_merge_273d_0001 & 0x1) << 0x4));
    fWrite = 0x1;
    goto L_277e;

L_276c:
    lpciPlan->fLaunchedPkt = 0x0;

L_277e:
    iWarp = (IWarpMAFromLppl(lppl, &(fTwoMA)) + 0xffff);
    /* untranslated: part[198:2](sel) = ((part[198:2](sel) & 0xc3ff) | ((iWarp & 0xf) << 0xa)) */
    if ((lpciPlan->fNeedScanPkt != 0x0))
        goto L_27dd;
    else
        goto L_27c5;

L_27c5:
    /* untranslated: part[198:2](sel) = ((part[198:2](sel) & 0xfc00) | (idPlanDst & 0x3ff)) */
    goto L_27ef;

L_27dd:
    lpciPlan->fNeedScanPkt = 0x0;

L_27ef:
    FLookupPlanet(0xffff, 0x49ee);

L_2802:
    lpciPlan->fLaunchedPkt = 0x0;

LFinish:
    FinishProduction(fWrite);
    goto L_1a95;

L_2823:
    return;
}

int16_t IdGetBestScannerDest(PLANET *lppl, int16_t iDir) {
    int16_t iDistance;
    int16_t iWarp;
    PLANET *lpplDst;
    int16_t dAdjust;
    int16_t iSize;
    POINT   ptEdge;
    SCAN    scan;

L_282a:
    ptEdge.x = rgptPlan[lppl->id].x;
    ptEdge.y = rgptPlan[lppl->id].y;
    iSize = (LOWORD((0x190 * game.mdSize)) + 0x190);
    iWarp = (IWarpMAFromLppl(lppl, 0x0) + 0x3);
    iDistance = LOWORD((iWarp * iWarp));
    ptEdge.x = (ptEdge.x - 0x3e8);
    ptEdge.y = (ptEdge.y - 0x3e8);
    goto L_2969;
    ptEdge.x = iSize;
    goto L_298a;
    if (((iSize - ptEdge.x) <= ptEdge.y))
        goto L_28ae;
    else
        goto L_28a0;

L_28a0:
    ptEdge.x = (ptEdge.x + ptEdge.y);
    ptEdge.y = 0x0;
    goto L_28bd;

L_28ae:
    ptEdge.y = (ptEdge.y - (iSize - ptEdge.x));
    ptEdge.x = iSize;

L_28bd:
    goto L_298a;
    ptEdge.y = 0x0;
    goto L_298a;
    if ((ptEdge.x <= ptEdge.y))
        goto L_28e1;
    else
        goto L_28d3;

L_28d3:
    ptEdge.x = (ptEdge.x - ptEdge.y);
    ptEdge.y = 0x0;
    goto L_28ec;

L_28e1:
    ptEdge.y = (ptEdge.y - ptEdge.x);
    ptEdge.x = 0x0;

L_28ec:
    goto L_298a;
    ptEdge.x = 0x0;
    goto L_298a;
    if (((iSize - ptEdge.x) <= ptEdge.y))
        goto L_2913;
    else
        goto L_2905;

L_2905:
    ptEdge.y = (ptEdge.y + ptEdge.x);
    ptEdge.x = 0x0;
    goto L_2922;

L_2913:
    ptEdge.x = (ptEdge.x - (iSize - ptEdge.y));
    ptEdge.y = iSize;

L_2922:
    goto L_298a;
    ptEdge.y = iSize;
    goto L_298a;
    if ((ptEdge.x <= ptEdge.y))
        goto L_294b;
    else
        goto L_2939;

L_2939:
    ptEdge.y = (ptEdge.y + (iSize - ptEdge.x));
    ptEdge.x = iSize;
    goto L_298a;

L_294b:
    ptEdge.x = (ptEdge.x + (iSize - ptEdge.y));
    ptEdge.y = iSize;

L_2969:

L_298a:
    /* untranslated: ss:[bp-0x2a] = iSize */
    /* untranslated: ss:[bp-0x28] = signhiword(iSize) */
    dAdjust = Random(LOWORD(__ftol()));
    /* untranslated: ss:[bp-0x2a] = iSize */
    /* untranslated: ss:[bp-0x28] = signhiword(iSize) */
    dAdjust = (dAdjust - LOWORD(__ftol()));
    if ((ptEdge.x == 0x0))
        goto L_29e8;
    else
        goto L_29dd;

L_29dd:
    if ((ptEdge.x != iSize))
        goto L_2a45;
    else
        goto L_29e8;

L_29e8:
    ptEdge.y = (ptEdge.y + dAdjust);
    if ((ptEdge.y <= iSize))
        goto L_2a0b;
    else
        goto L_29f9;

L_29f9:
    dAdjust = (ptEdge.y - iSize);
    ptEdge.y = iSize;
    goto L_2a2a;

L_2a0b:
    if ((ptEdge.y >= 0x0))
        goto L_2a25;
    else
        goto L_2a14;

L_2a14:
    dAdjust = (0x0 - ptEdge.y);
    ptEdge.y = 0x0;
    goto L_2a2a;

L_2a25:
    dAdjust = 0x0;

L_2a2a:
    if ((ptEdge.x != 0x0))
        goto L_2a3c;
    else
        goto L_2a33;

L_2a33:
    ptEdge.x = (ptEdge.x + dAdjust);
    goto L_2a9f;

L_2a3c:
    ptEdge.x = (ptEdge.x - dAdjust);

L_2a45:
    ptEdge.x = (ptEdge.x + dAdjust);
    if ((ptEdge.x <= iSize))
        goto L_2a68;
    else
        goto L_2a56;

L_2a56:
    dAdjust = (ptEdge.x - iSize);
    ptEdge.x = iSize;
    goto L_2a87;

L_2a68:
    if ((ptEdge.x >= 0x0))
        goto L_2a82;
    else
        goto L_2a71;

L_2a71:
    dAdjust = (0x0 - ptEdge.x);
    ptEdge.x = 0x0;
    goto L_2a87;

L_2a82:
    dAdjust = 0x0;

L_2a87:
    if ((ptEdge.y != 0x0))
        goto L_2a99;
    else
        goto L_2a90;

L_2a90:
    ptEdge.y = (ptEdge.y + dAdjust);
    goto L_2a9f;

L_2a99:
    ptEdge.y = (ptEdge.y - dAdjust);

L_2a9f:
    dAdjust = Random(iDistance);
    if ((ptEdge.y != 0x0))
        goto L_2abf;
    else
        goto L_2ab6;

L_2ab6:
    ptEdge.y = (ptEdge.y + dAdjust);
    goto L_2af6;

L_2abf:
    if ((ptEdge.y != iSize))
        goto L_2ad3;
    else
        goto L_2aca;

L_2aca:
    ptEdge.y = (ptEdge.y - dAdjust);
    goto L_2af6;

L_2ad3:
    if ((ptEdge.x != 0x0))
        goto L_2ae5;
    else
        goto L_2adc;

L_2adc:
    ptEdge.x = (ptEdge.x + dAdjust);
    goto L_2af6;

L_2ae5:
    if ((ptEdge.x != iSize))
        goto L_2af6;
    else
        goto L_2af0;

L_2af0:
    ptEdge.x = (ptEdge.x - dAdjust);

L_2af6:
    ptEdge.y = (ptEdge.y + 0x3e8);
    ptEdge.x = (ptEdge.x + 0x3e8);
    /* untranslated: branch FFindNearestObject(words(ptEdge.y, ptEdge.x), 0x21, &scan) == 0x0 ? L_2bb4 : L_2b1e */

L_2b1e:
    lpplDst = LpplFromId(scan.idpl);
    if ((LOWORD(lpplDst) != 0x0))
        goto L_2b41;
    else
        goto L_2b38;

L_2b38:
    if ((HIWORD(lpplDst) == 0x0))
        goto L_2b50;
    else
        goto L_2b41;

L_2b41:
    if ((lpplDst->iPlayer == idPlayer))
        goto L_2b9f;
    else
        goto L_2b50;

L_2b50:
    /* untranslated: branch hiword(LDistance2(words(rgptPlan[scan.idpl].y, rgptPlan[scan.idpl].x), words(rgptPlan[lppl->id].y, rgptPlan[lppl->id].x))) >
     * signhiword(loword((iDistance * iDistance))) ? L_2ba8 : L_2b93 */

L_2b93:
    /* untranslated: branch ss:[bp-0x24] < signhiword(loword((iDistance * iDistance))) ? L_2b9f : L_2b98 */

L_2b98:
    /* untranslated: branch ss:[bp-0x26] >= loword((iDistance * iDistance)) ? L_2ba8 : L_2b9f */

L_2b9f:
    return 0xffff;

L_2ba8:
    return (scan.idpl + 0x1);

L_2bb4:
    return 0xffff;
}

int16_t FAddPacketToQueue(PLANET *lppl) {
    int32_t rgResCost[4];
    int16_t iMineral;
    int32_t rgResAvail[4];

L_2bc0:
    iMineral = 0x0;
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    /* untranslated: branch (HIWORD(rgResAvail[iMineral]) - HIWORD(rgResCost[iMineral])) > (part[6:2](rgResAvail) - part[6:2](rgResCost)) ? L_2c38 : L_2c27 */

L_2c27:
    /* untranslated: branch (HIWORD(rgResAvail[iMineral]) - HIWORD(rgResCost[iMineral])) < (part[6:2](rgResAvail) - part[6:2](rgResCost)) ? L_2c33 : L_2c2c */

L_2c2c:
    /* untranslated: branch (LOWORD(rgResAvail[iMineral]) - LOWORD(rgResCost[iMineral])) >= (part[4:2](rgResAvail) - part[4:2](rgResCost)) ? L_2c38 : L_2c33 */

L_2c33:
    iMineral = 0x1;

L_2c38:
    /* untranslated: branch (HIWORD(rgResAvail[iMineral]) - HIWORD(rgResCost[iMineral])) > (part[10:2](rgResAvail) - part[10:2](rgResCost)) ? L_2c7e : L_2c6d */

L_2c6d:
    /* untranslated: branch (HIWORD(rgResAvail[iMineral]) - HIWORD(rgResCost[iMineral])) < (part[10:2](rgResAvail) - part[10:2](rgResCost)) ? L_2c79 : L_2c72 */

L_2c72:
    /* untranslated: branch (LOWORD(rgResAvail[iMineral]) - LOWORD(rgResCost[iMineral])) >= (part[8:2](rgResAvail) - part[8:2](rgResCost)) ? L_2c7e : L_2c79 */

L_2c79:
    iMineral = 0x2;

L_2c7e:
    if (((HIWORD(rgResAvail[iMineral]) - HIWORD(rgResCost[iMineral])) < 0x0))
        goto L_2cd6;
    else
        goto L_2ca8;

L_2ca8:
    if (((HIWORD(rgResAvail[iMineral]) - HIWORD(rgResCost[iMineral])) > 0x0))
        goto L_2cb5;
    else
        goto L_2cad;

L_2cad:
    if (((LOWORD(rgResAvail[iMineral]) - LOWORD(rgResCost[iMineral])) < 0xaa))
        goto L_2cd6;
    else
        goto L_2cb5;

L_2cb5:
    AddItemToQueue((iMineral + 0xe), 0x1, grobjPlanet, 0x0);
    return 0x1;

L_2cd6:
    return 0x0;
}

void FillProductionQueue() {
    PLANET *lppl;
    int16_t ipl;
    PROD    rgprod[64];

L_2ce2:
    ipl = 0x0;
    goto L_2cf7;

L_2cf3:
    ipl = (ipl + 0x1);

L_2cf7:
    if ((ipl >= vclpplAi))
        goto L_2d6b;
    else
        goto L_2d02;

L_2d02:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_2d32;
    else
        goto L_2d2a;

L_2d2a:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_2d6b;
    else
        goto L_2d32;

L_2d32:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    FinishProduction(FFillProdMinesAndFactories(lppl));
    goto L_2cf3;

L_2d6b:
    return;
}

int16_t FFillProdMinesAndFactories(PLANET *lppl) {
    int32_t  rgResCost[4];
    int32_t  rgResAvail[4];
    int16_t  iAddFactories;
    int16_t  iAddMines;
    PROD     prod;
    int32_t  rgAlchCost[4];
    int32_t  rgMineCost[4];
    int32_t  rgFactCost[4];
    int32_t  rgResLeft[4];
    int16_t  iAddAlchemy;
    int16_t  iMaxTerra;
    int16_t  i;
    int16_t  iMaxFactories;
    int16_t  iMaxFactBuildable;
    int16_t  iMaxMines;
    int16_t  fInsert;
    PROD    *lpprod;
    int16_t  cAdd;
    uint16_t t_merge_302d_0001;
    uint16_t t_merge_30fb_0001;
    int16_t  t_merge_3375_0001;
    int16_t  t_merge_33fb_0001;
    int16_t  t_merge_3431_0001;
    int16_t  t_merge_3493_0001;
    int16_t  t_merge_34e1_0001;
    int16_t  t_merge_3559_0001;
    int16_t  t_merge_3595_0001;
    int16_t  t_merge_35f0_0001;
    uint16_t t_merge_36f1_0001;
    uint16_t t_merge_3778_0001;

L_2d72:
    fInsert = 0x0;
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0x0;
    goto L_2de3;

L_2dac:
    rgResLeft[i] = (rgResAvail[i] - rgResCost[i]);
    i = (i + 0x1);

L_2de3:
    if ((i < 0x4))
        goto L_2dac;
    else
        goto L_2dec;

L_2dec:
    /* untranslated: branch part[14:2](rgResLeft) > 0x0 ? L_2e09 : L_2df5 */

L_2df5:
    /* untranslated: branch part[14:2](rgResLeft) < 0x0 ? L_2e03 : L_2dfa */

L_2dfa:
    /* untranslated: branch part[12:2](rgResLeft) >= 0x0 ? L_2e09 : L_2e03 */

L_2e03:
    return 0x0;

L_2e09:
    iMaxTerra = 0x0;
    iMaxMines = 0x0;
    iAddMines = 0x0;
    iMaxFactories = 0x0;
    iAddFactories = 0x0;
    iAddAlchemy = 0x0;
    /* untranslated: part[12:2](rgMineCost) = 0x0 */
    /* untranslated: part[14:2](rgMineCost) = 0x0 */
    /* untranslated: part[12:2](rgFactCost) = 0x0 */
    /* untranslated: part[14:2](rgFactCost) = 0x0 */
    /* untranslated: part[12:2](rgAlchCost) = 0x0 */
    /* untranslated: part[14:2](rgAlchCost) = 0x0 */
    i = 0x0;
    lpprod = &(lpplProdGlob->rgprod);
    goto L_2e75;

L_2e62:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_2e75:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_2f51;
    else
        goto L_2e88;

L_2e88:
    if ((lpprod->grobj != 0x1))
        goto L_2eeb;
    else
        goto L_2ea8;

L_2ea8:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_2eeb : L_2eb0 */

L_2eb0:
    if ((lpprod->iItem != 0x8))
        goto L_2eeb;
    else
        goto L_2ed0;

L_2ed0:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_2eeb : L_2ed8 */

L_2ed8:
    iAddMines = (iAddMines + lpprod->cItem);

L_2eeb:
    if ((lpprod->grobj != 0x1))
        goto L_2e62;
    else
        goto L_2f0b;

L_2f0b:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_2e62 : L_2f13 */

L_2f13:
    if ((lpprod->iItem != 0x7))
        goto L_2e62;
    else
        goto L_2f33;

L_2f33:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_2e62 : L_2f3b */

L_2f3b:
    iAddFactories = (iAddFactories + lpprod->cItem);

L_2f51:
    i = (cProdGlob + 0xffff);
    goto L_31d3;

L_2f5d:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0x11))) & 0x7) != 0x1))
        goto L_31cf;
    else
        goto L_2f85;

L_2f85:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_31cf : L_2f8d */

L_2f8d:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x7f) != 0x8))
        goto L_305b;
    else
        goto L_2fb5;

L_2fb5:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_305b : L_2fbd */

L_2fbd:
    /* untranslated: ss:[bp-0x7e] = CMaxOperableMines(lppl, idPlayer, 0x0) */
    /* untranslated: branch 0x0 <= ((ss:[bp-0x7e] - CMinesOperating(lppl)) - iAddMines) ? L_2ffc : L_2ff6 */

L_2ff6:
    t_merge_302d_0001 = 0x0;
    goto L_302d;

L_2ffc:
    /* untranslated: ss:[bp-0x80] = CMaxOperableMines(lppl, idPlayer, 0x0) */
    /* untranslated: t_merge_302d_0001 = ((ss:[bp-0x80] - CMinesOperating(lppl)) - iAddMines) */

L_302d:
    iMaxMines = t_merge_302d_0001;
    /* untranslated: call GetProductionCosts(lppl, (pProdGlob + (i * 0x4)), &part[0:4](rgMineCost), idPlayer, 0x1) -> callresult(void) */

L_305b:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x7f) != 0x7))
        goto L_3129;
    else
        goto L_3083;

L_3083:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_3129 : L_308b */

L_308b:
    /* untranslated: ss:[bp-0x7e] = CMaxOperableFactories(lppl, idPlayer, 0x0) */
    /* untranslated: branch 0x0 <= ((ss:[bp-0x7e] - CFactoriesOperating(lppl)) - iAddFactories) ? L_30ca : L_30c4 */

L_30c4:
    t_merge_30fb_0001 = 0x0;
    goto L_30fb;

L_30ca:
    /* untranslated: ss:[bp-0x80] = CMaxOperableFactories(lppl, idPlayer, 0x0) */
    /* untranslated: t_merge_30fb_0001 = ((ss:[bp-0x80] - CFactoriesOperating(lppl)) - iAddFactories) */

L_30fb:
    iMaxFactories = t_merge_30fb_0001;
    /* untranslated: call GetProductionCosts(lppl, (pProdGlob + (i * 0x4)), &part[0:4](rgFactCost), idPlayer, 0x1) -> callresult(void) */

L_3129:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x7f) != 0xb))
        goto L_3184;
    else
        goto L_3151;

L_3151:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_3184 : L_3159 */

L_3159:
    /* untranslated: call GetProductionCosts(lppl, (pProdGlob + (i * 0x4)), &part[0:4](rgAlchCost), idPlayer, 0x1) -> callresult(void) */

L_3184:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x7f) != 0xc))
        goto L_31cf;
    else
        goto L_31ac;

L_31ac:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_31cf : L_31b4 */

L_31b4:
    iMaxTerra = (LOWORD(pProdGlob[i]) & 0x3ff);

L_31cf:
    i = (i - 0x1);

L_31d3:
    if ((i >= 0x0))
        goto L_2f5d;
    else
        goto L_31dc;

L_31dc:
    if ((iMaxTerra <= 0x0))
        goto L_3259;
    else
        goto L_31e5;

L_31e5:
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xd) & 0x7) != 0x5 ? L_3259 : L_3204 */

L_3204:
    /* untranslated: cAdd = loword((int32_t)(words((part[14:2](rgResLeft) + 0x0), (part[12:2](rgResLeft) + 0x45)) / 0x46)) */
    if ((cAdd <= iMaxTerra))
        goto L_3233;
    else
        goto L_322d;

L_322d:
    cAdd = iMaxTerra;

L_3233:
    if ((cAdd <= 0x0))
        goto L_3259;
    else
        goto L_323c;

L_323c:
    AddItemToQueue(0xc, cAdd, grobjPlanet, 0x0);
    return 0x1;

L_3259:
    if ((HIWORD(rgResLeft) < 0x0))
        goto L_329e;
    else
        goto L_3262;

L_3262:
    if ((HIWORD(rgResLeft) > 0x0))
        goto L_3270;
    else
        goto L_3267;

L_3267:
    if ((LOWORD(rgResLeft) <= 0x0))
        goto L_329e;
    else
        goto L_3270;

L_3270:
    /* untranslated: branch part[6:2](rgResLeft) < 0x0 ? L_329e : L_3279 */

L_3279:
    /* untranslated: branch part[6:2](rgResLeft) > 0x0 ? L_3287 : L_327e */

L_327e:
    /* untranslated: branch part[4:2](rgResLeft) <= 0x0 ? L_329e : L_3287 */

L_3287:
    /* untranslated: branch part[10:2](rgResLeft) > 0x0 ? L_33a9 : L_3290 */

L_3290:
    /* untranslated: branch part[10:2](rgResLeft) < 0x0 ? L_329e : L_3295 */

L_3295:
    /* untranslated: branch part[8:2](rgResLeft) > 0x0 ? L_33a9 : L_329e */

L_329e:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_32b5;
    else
        goto L_32ab;

L_32ab:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_333c;
    else
        goto L_32b5;

L_32b5:
    prod = lppl->lpplprod->rgprod[0x0];
    if ((prod.grobj != 0x1))
        goto L_333c;
    else
        goto L_32e6;

L_32e6:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_333c : L_32ee */

L_32ee:
    if ((prod.iItem != 0x3))
        goto L_3312;
    else
        goto L_330a;

L_330a:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) == 0x0 ? L_3336 : L_3312 */

L_3312:
    if ((prod.iItem != 0xb))
        goto L_333c;
    else
        goto L_332e;

L_332e:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_333c : L_3336 */

L_3336:
    return 0x0;

L_333c:
    if ((iMaxMines <= 0x0))
        goto L_337b;
    else
        goto L_3345;

L_3345:
    /* untranslated: branch iMaxMines >= loword((int32_t)(words(part[14:2](rgResLeft), part[12:2](rgResLeft)) / words(part[14:2](rgMineCost),
     * part[12:2](rgMineCost)))) ? L_3364 : L_335e */

L_335e:
    t_merge_3375_0001 = iMaxMines;
    goto L_3375;

L_3364:
    /* untranslated: t_merge_3375_0001 = loword(callresult(int32_t)) */

L_3375:
    iAddMines = t_merge_3375_0001;
    goto L_3380;

L_337b:
    iAddMines = 0x0;

L_3380:
    /* untranslated: part[12:2](rgResLeft) = (part[12:2](rgResLeft) - loword((uint32_t)(sext16to32(iAddMines) * words(part[14:2](rgMineCost),
     * part[12:2](rgMineCost))))) */
    /* untranslated: part[14:2](rgResLeft) = (part[14:2](rgResLeft) - hiword((uint32_t)(sext16to32(iAddMines) * words(part[14:2](rgMineCost),
     * part[12:2](rgMineCost))))) */
    iAddFactories = 0x0;
    iAddAlchemy = 0x0;
    fInsert = 0x1;
    goto LAdd;

L_33a9:
    iMaxFactBuildable = iMaxFactories;
    if ((iMaxFactories <= 0x0))
        goto L_355c;
    else
        goto L_33b8;

L_33b8:
    if ((gd.fTutorial != 0x0))
        goto L_3401;
    else
        goto L_33cb;

L_33cb:
    /* untranslated: branch iMaxFactories >= loword((int32_t)(words(part[10:2](rgResLeft), part[8:2](rgResLeft)) / words(part[10:2](rgFactCost),
     * part[8:2](rgFactCost)))) ? L_33ea : L_33e4 */

L_33e4:
    t_merge_33fb_0001 = iMaxFactories;
    goto L_33fb;

L_33ea:
    /* untranslated: t_merge_33fb_0001 = loword(callresult(int32_t)) */

L_33fb:
    iMaxFactories = t_merge_33fb_0001;
    goto L_355c;

L_3401:
    /* untranslated: branch iMaxFactories >= loword((int32_t)(words(HIWORD(rgResLeft), LOWORD(rgResLeft)) / words(HIWORD(rgFactCost), LOWORD(rgFactCost)))) ?
     * L_3420 : L_341a */

L_341a:
    t_merge_3431_0001 = iMaxFactories;
    goto L_3431;

L_3420:
    /* untranslated: t_merge_3431_0001 = loword(callresult(int32_t)) */

L_3431:
    /* untranslated: branch t_merge_3431_0001 >= loword((int32_t)(words(part[6:2](rgResLeft), part[4:2](rgResLeft)) / words(part[6:2](rgFactCost),
     * part[4:2](rgFactCost)))) ? L_3482 : L_344f */

L_344f:
    /* untranslated: branch iMaxFactories >= loword((int32_t)(words(HIWORD(rgResLeft), LOWORD(rgResLeft)) / words(HIWORD(rgFactCost), LOWORD(rgFactCost)))) ?
     * L_346e : L_3468 */

L_3468:
    t_merge_3493_0001 = iMaxFactories;
    goto L_3493;

L_346e:
    /* untranslated: t_merge_3493_0001 = loword(callresult(int32_t)) */

L_3482:
    /* untranslated: t_merge_3493_0001 = loword(callresult(int32_t)) */

L_3493:
    /* untranslated: branch t_merge_3493_0001 >= loword((int32_t)(words(part[10:2](rgResLeft), part[8:2](rgResLeft)) / words(part[10:2](rgFactCost),
     * part[8:2](rgFactCost)))) ? L_3548 : L_34b1 */

L_34b1:
    /* untranslated: branch iMaxFactories >= loword((int32_t)(words(HIWORD(rgResLeft), LOWORD(rgResLeft)) / words(HIWORD(rgFactCost), LOWORD(rgFactCost)))) ?
     * L_34d0 : L_34ca */

L_34ca:
    t_merge_34e1_0001 = iMaxFactories;
    goto L_34e1;

L_34d0:
    /* untranslated: t_merge_34e1_0001 = loword(callresult(int32_t)) */

L_34e1:
    /* untranslated: branch t_merge_34e1_0001 >= loword((int32_t)(words(part[6:2](rgResLeft), part[4:2](rgResLeft)) / words(part[6:2](rgFactCost),
     * part[4:2](rgFactCost)))) ? L_3534 : L_3501 */

L_3501:
    /* untranslated: branch iMaxFactories >= loword((int32_t)(words(HIWORD(rgResLeft), LOWORD(rgResLeft)) / words(HIWORD(rgFactCost), LOWORD(rgFactCost)))) ?
     * L_3520 : L_351a */

L_351a:
    t_merge_3559_0001 = iMaxFactories;
    goto L_3559;

L_3520:
    /* untranslated: t_merge_3559_0001 = loword(callresult(int32_t)) */

L_3534:
    /* untranslated: t_merge_3559_0001 = loword(callresult(int32_t)) */

L_3548:
    /* untranslated: t_merge_3559_0001 = loword(callresult(int32_t)) */

L_3559:
    iMaxFactories = t_merge_3559_0001;

L_355c:
    if ((iMaxFactories <= 0x0))
        goto L_359b;
    else
        goto L_3565;

L_3565:
    /* untranslated: branch iMaxFactories >= loword((int32_t)(words(part[14:2](rgResLeft), part[12:2](rgResLeft)) / words(part[14:2](rgFactCost),
     * part[12:2](rgFactCost)))) ? L_3584 : L_357e */

L_357e:
    t_merge_3595_0001 = iMaxFactories;
    goto L_3595;

L_3584:
    /* untranslated: t_merge_3595_0001 = loword(callresult(int32_t)) */

L_3595:
    iAddFactories = t_merge_3595_0001;
    goto L_35a0;

L_359b:
    iAddFactories = 0x0;

L_35a0:
    /* untranslated: part[12:2](rgResLeft) = (part[12:2](rgResLeft) - loword((uint32_t)(sext16to32(iAddFactories) * words(part[14:2](rgFactCost),
     * part[12:2](rgFactCost))))) */
    /* untranslated: part[14:2](rgResLeft) = (part[14:2](rgResLeft) - hiword((uint32_t)(sext16to32(iAddFactories) * words(part[14:2](rgFactCost),
     * part[12:2](rgFactCost))))) */
    if ((iMaxMines <= 0x0))
        goto L_35f6;
    else
        goto L_35c0;

L_35c0:
    /* untranslated: branch iMaxMines >= loword((int32_t)(words(part[14:2](rgResLeft), part[12:2](rgResLeft)) / words(part[14:2](rgMineCost),
     * part[12:2](rgMineCost)))) ? L_35df : L_35d9 */

L_35d9:
    t_merge_35f0_0001 = iMaxMines;
    goto L_35f0;

L_35df:
    /* untranslated: t_merge_35f0_0001 = loword(callresult(int32_t)) */

L_35f0:
    iAddMines = t_merge_35f0_0001;
    goto L_35fb;

L_35f6:
    iAddMines = 0x0;

L_35fb:
    /* untranslated: part[12:2](rgResLeft) = (part[12:2](rgResLeft) - loword((uint32_t)(sext16to32(iAddMines) * words(part[14:2](rgMineCost),
     * part[12:2](rgMineCost))))) */
    /* untranslated: part[14:2](rgResLeft) = (part[14:2](rgResLeft) - hiword((uint32_t)(sext16to32(iAddMines) * words(part[14:2](rgMineCost),
     * part[12:2](rgMineCost))))) */

LAdd:
    /* untranslated: branch part[14:2](rgAlchCost) < 0x0 ? L_36f7 : L_361b */

L_361b:
    /* untranslated: branch part[14:2](rgAlchCost) > 0x0 ? L_3629 : L_3620 */

L_3620:
    /* untranslated: branch part[12:2](rgAlchCost) <= 0x0 ? L_36f7 : L_3629 */

L_3629:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) != 0x1a))
        goto L_36f7;
    else
        goto L_3641;

L_3641:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x5]) != 0x1a))
        goto L_36f7;
    else
        goto L_3659;

L_3659:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x0]) != 0x1a))
        goto L_36f7;
    else
        goto L_3671;

L_3671:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) != 0x1a))
        goto L_36f7;
    else
        goto L_3689;

L_3689:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) != 0x1a))
        goto L_36f7;
    else
        goto L_36a1;

L_36a1:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) != 0x1a))
        goto L_36f7;
    else
        goto L_36b9;

L_36b9:
    /* untranslated: branch 0x0 <= (loword((int32_t)(words(part[14:2](rgResLeft), part[12:2](rgResLeft)) / words(part[14:2](rgAlchCost),
     * part[12:2](rgAlchCost)))) + 0x1) ? L_36dd : L_36d7 */

L_36d7:
    t_merge_36f1_0001 = 0x0;
    goto L_36f1;

L_36dd:
    /* untranslated: t_merge_36f1_0001 = (loword(callresult(int32_t)) + 0x1) */

L_36f1:
    iAddAlchemy = t_merge_36f1_0001;
    goto L_36fc;

L_36f7:
    iAddAlchemy = 0x0;

L_36fc:
    /* untranslated: part[12:2](rgResLeft) = (part[12:2](rgResLeft) - loword((uint32_t)(sext16to32(iAddAlchemy) * words(part[14:2](rgAlchCost),
     * part[12:2](rgAlchCost))))) */
    /* untranslated: part[14:2](rgResLeft) = (part[14:2](rgResLeft) - hiword((uint32_t)(sext16to32(iAddAlchemy) * words(part[14:2](rgAlchCost),
     * part[12:2](rgAlchCost))))) */
    if ((iAddFactories <= 0x0))
        goto L_3733;
    else
        goto L_371c;

L_371c:
    AddItemToQueue(0x7, iAddFactories, grobjPlanet, 0x1);

L_3733:
    if ((iAddMines <= 0x0))
        goto L_3753;
    else
        goto L_373c;

L_373c:
    AddItemToQueue(0x8, iAddMines, grobjPlanet, 0x0);

L_3753:
    if ((iAddAlchemy <= 0x0))
        goto L_378c;
    else
        goto L_375c;

L_375c:
    if ((game.turn <= 0x64))
        goto L_378c;
    else
        goto L_3766;

L_3766:
    if ((fInsert == 0x0))
        goto L_3775;
    else
        goto L_376f;

L_376f:
    t_merge_3778_0001 = 0x0;
    goto L_3778;

L_3775:
    t_merge_3778_0001 = 0x1;

L_3778:
    AddItemToQueue(0xb, iAddAlchemy, grobjPlanet, t_merge_3778_0001);

L_378c:
    if ((((iAddMines + iAddFactories) + iAddAlchemy) <= 0x0))
        goto L_37a3;
    else
        goto L_379d;

L_379d:
    return 0x1;

L_37a3:
    return 0x0;
}

void DoCyberFreighter(FLEET *lpfl, CYBERINFOTEMP *lpciPlanTemp) {
    ORDER    ord;
    PLANET  *lpplDst;
    PLANET  *lpplCur;
    int16_t  fDropOff;
    int16_t  idPlanDst;
    SCAN     scan;
    uint16_t t_396c;
    uint16_t t_merge_3a76_0001;
    uint16_t t_3c85;

L_37b0:
    fDropOff = 0x0;
    lpplCur = LpplFromId(lpfl->idPlanet);
    if ((LOWORD(lpplCur) != 0x0))
        goto L_3829;
    else
        goto L_37dc;

L_37dc:
    if ((HIWORD(lpplCur) != 0x0))
        goto L_3829;
    else
        goto L_37e5;

L_37e5:
    /* untranslated: branch FFindNearestObject(words(lpfl->pt.y, lpfl->pt.x), 0x21, &scan) == 0x0 ? L_381c : L_3808 */

L_3808:
    lpplDst = LpplFromId(scan.idpl);
    goto LTarget;

L_381c:
    lpplDst = 0x0;

L_3829:
    if ((lpplCur->iPlayer != idPlayer))
        goto L_38ce;
    else
        goto L_3838;

L_3838:
    if ((HIWORD(lpplCur->rgwtMin[0x3]) < 0x0))
        goto L_3894;
    else
        goto L_3845;

L_3845:
    if ((HIWORD(lpplCur->rgwtMin[0x3]) > 0x0))
        goto L_3855;
    else
        goto L_384a;

L_384a:
    if ((LOWORD(lpplCur->rgwtMin[0x3]) <= 0x7d0))
        goto L_3894;
    else
        goto L_3855;

L_3855:
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, LOWORD(lpfl), 0x3, 0x3e8);
    FLookupFleet(LOWORD(lpfl), 0x4972);
    fDropOff = 0x1;
    goto L_3a79;

L_3894:
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, LOWORD(lpfl), 0x3, 0xfc18);
    FLookupFleet(LOWORD(lpfl), 0x4972);

L_38ce:
    if ((lpplCur->iPlayer == 0xffff))
        goto L_3a51;
    else
        goto L_38db;

L_38db:
    if ((GetRaceStat(rgplr[lpplCur->iPlayer], rsMajorAdv) == raMacintosh))
        goto L_3a51;
    else
        goto L_38ff;

L_38ff:
    if ((lpplCur->fStarbase != 0x0))
        goto L_3a51;
    else
        goto L_3916;

L_3916:
    FLookupFleet(LOWORD(lpfl), 0x4972);
    idPlanDst = lpplCur->id;
    memset(ord, 0x0, 0x12);
    ord.pt.x = rgptPlan[idPlanDst].x;
    ord.pt.y = rgptPlan[idPlanDst].y;
    ord.grobj = grobjPlanet;
    ord.id = idPlanDst;
    /* untranslated: t_396c = part[6:2](ord) */
    ord.grTask = grTaskXfer;
    /* untranslated: part[6:2](ord) = ((((t_396c & 0xfff0) | 0x1) & 0xefff) | 0x1000) */
    /* untranslated: part[14:2](ord) = ((part[14:2](ord) & 0xfff) | 0x2000) */
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    if ((sel.fl.lpplord->rgord[0x0].id != idPlanDst))
        goto L_39f2;
    else
        goto L_39af;

L_39af:
    /* untranslated: branch ((part[6:2](sel.fl.lpplord->rgord[0x0]) >> 0x8) & 0xf) != 0x1 ? L_39f2 : L_39c7 */

L_39c7:
    sel.fl.lpplord->rgord[0x0] = ord;
    goto L_3a1a;

L_39f2:
    sel.fl.lpplord->rgord[0x1] = ord;

L_3a1a:
    FLookupFleet(0xffff, 0x4972);
    FMoveToNearestStarbase(lpfl, 0x0);
    FLookupFleet(LOWORD(lpfl), 0x4972);
    goto L_3a79;

L_3a51:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < 0x0))
        goto L_3a73;
    else
        goto L_3a5e;

L_3a5e:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > 0x0))
        goto L_3a6d;
    else
        goto L_3a63;

L_3a63:
    if ((LOWORD(lpfl->rgwtMin[0x3]) <= 0x0))
        goto L_3a73;
    else
        goto L_3a6d;

L_3a6d:
    t_merge_3a76_0001 = 0x1;
    goto L_3a76;

L_3a73:
    t_merge_3a76_0001 = 0x0;

L_3a76:
    fDropOff = t_merge_3a76_0001;

L_3a79:
    if ((fDropOff == 0x0))
        goto L_3c16;
    else
        goto L_3a82;

L_3a82:
    lpplDst = LpplFindClosestEnum(lpplCur, FEnumDropOffStage1);
    if ((LOWORD(lpplDst) != 0x0))
        goto L_3acc;
    else
        goto L_3aa7;

L_3aa7:
    if ((HIWORD(lpplDst) != 0x0))
        goto L_3acc;
    else
        goto L_3ab0;

L_3ab0:
    lpplDst = LpplFindClosestEnum(lpplCur, FEnumDropOffStage2);

L_3acc:
    if ((LOWORD(lpplDst) != 0x0))
        goto L_3b9b;
    else
        goto L_3ad5;

L_3ad5:
    if ((HIWORD(lpplDst) != 0x0))
        goto L_3b9b;
    else
        goto L_3ade;

L_3ade:
    if ((lpplCur->iPlayer != idPlayer))
        goto LTarget;
    else
        goto L_3aed;

L_3aed:
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, LOWORD(lpfl), 0x3, 0xfc18);
    FLookupFleet(LOWORD(lpfl), 0x4972);
    if ((((lpciPlanTemp[lpplCur->id] >> 0x1) & 0x3) >= 0x3))
        goto LTarget;
    else
        goto L_3b46;

L_3b46:
    /* untranslated: ss:[bp-0x32] = ((lpciPlanTemp[lpplCur->id] + 0x2) & 0x6) */
    lpciPlanTemp[lpplCur->id] = (lpciPlanTemp[lpplCur->id] & 0xfff9);
    /* untranslated: lpciPlanTemp[lpplCur->id] = (lpciPlanTemp[lpplCur->id] | ss:[bp-0x32]) */

L_3b9b:
    if ((((lpciPlanTemp[lpplDst->id] >> 0x3) & 0x3) >= 0x3))
        goto LTarget;
    else
        goto L_3bc1;

L_3bc1:
    /* untranslated: ss:[bp-0x32] = ((lpciPlanTemp[lpplDst->id] + 0x8) & 0x18) */
    lpciPlanTemp[lpplDst->id] = (lpciPlanTemp[lpplDst->id] & 0xffe7);
    /* untranslated: lpciPlanTemp[lpplDst->id] = (lpciPlanTemp[lpplDst->id] | ss:[bp-0x32]) */

L_3c16:
    lpplDst = LpplFindClosestEnum(lpplCur, FEnumPickUp);

LTarget:
    if ((LOWORD(lpplDst) != 0x0))
        goto L_3c44;
    else
        goto L_3c3b;

L_3c3b:
    if ((HIWORD(lpplDst) == 0x0))
        goto L_3cfa;
    else
        goto L_3c44;

L_3c44:
    memset(ord, 0x0, 0x12);
    ord.pt.x = rgptPlan[lpplDst->id].x;
    ord.pt.y = rgptPlan[lpplDst->id].y;
    ord.grobj = grobjPlanet;
    ord.id = lpplDst->id;
    /* untranslated: t_3c85 = part[6:2](ord) */
    ord.grTask = grTaskNone;
    /* untranslated: part[6:2](ord) = ((((t_3c85 & 0xfff0) | 0x0) & 0xefff) | 0x1000) */
    /* untranslated: part[6:2](ord) = ((part[6:2](ord) & 0xff0f) | ((IFindIdealWarp(lpfl, 0x1) & 0xf) << 0x4)) */
    if ((FMoveAiFleet(lpfl, &(ord), 0x0) == 0x0))
        goto L_3cfa;
    else
        goto L_3ce6;

L_3ce6:
    lpfl->fMark = 0x1;

L_3cfa:
    return;
}

int16_t FEnumDropOffStage1(PLANET *lpplSrc, PLANET *lpplTest) {
    CYBERINFOTEMP *lpciPlanTemp;
    int16_t        dOffsetPlanTemp;

L_3d00:
    /* untranslated: dOffsetPlanTemp = part[2:0](mpicolgrbitBU[game.cPlanMax]) */
    lpciPlanTemp = &(vlpbAiData[dOffsetPlanTemp]);
    if ((((lpciPlanTemp[lpplTest->id] >> 0x3) & 0x3) != 0x3))
        goto L_3d53;
    else
        goto L_3d4d;

L_3d4d:
    return 0x0;

L_3d53:
    if ((lpplTest->iPlayer != idPlayer))
        goto L_3df2;
    else
        goto L_3d62;

L_3d62:
    if ((HIWORD(lpplTest->rgwtMin[0x3]) > 0x0))
        goto L_3df2;
    else
        goto L_3d6f;

L_3d6f:
    if ((HIWORD(lpplTest->rgwtMin[0x3]) < 0x0))
        goto L_3d7f;
    else
        goto L_3d74;

L_3d74:
    if ((LOWORD(lpplTest->rgwtMin[0x3]) >= 0xc8))
        goto L_3df2;
    else
        goto L_3d7f;

L_3d7f:
    if ((((lpciPlanTemp[lpplTest->id] >> 0x3) & 0x3) != 0x0))
        goto L_3df2;
    else
        goto L_3da5;

L_3da5:
    /* untranslated: branch hiword(LDistance2(words(rgptPlan[lpplSrc->id].y, rgptPlan[lpplSrc->id].x), words(rgptPlan[lpplTest->id].y,
     * rgptPlan[lpplTest->id].x))) > 0x2 ? L_3dec : L_3dd9 */

L_3dd9:
    /* untranslated: branch hiword(callresult(int32_t)) < 0x2 ? L_3de6 : L_3dde */

L_3dde:
    /* untranslated: branch loword(callresult(int32_t)) > 0x7100 ? L_3dec : L_3de6 */

L_3de6:
    return 0x1;

L_3dec:
    return 0x0;

L_3df2:
    return 0x0;
}

int16_t FEnumDropOffStage2(PLANET *lpplSrc, PLANET *lpplTest) {
    CYBERINFOTEMP *lpciPlanTemp;
    int16_t        dOffsetPlanTemp;

L_3dfe:
    /* untranslated: dOffsetPlanTemp = part[2:0](mpicolgrbitBU[game.cPlanMax]) */
    lpciPlanTemp = &(vlpbAiData[dOffsetPlanTemp]);
    if ((((lpciPlanTemp[lpplTest->id] >> 0x3) & 0x3) != 0x3))
        goto L_3e51;
    else
        goto L_3e4b;

L_3e4b:
    return 0x0;

L_3e51:
    if ((lpplTest->iPlayer != idPlayer))
        goto L_3ef3;
    else
        goto L_3e60;

L_3e60:
    if (((0x0 + HIWORD(lpplTest->rgwtMin[0x3])) > 0x0))
        goto L_3ef3;
    else
        goto L_3e99;

L_3e99:
    if (((0x0 + HIWORD(lpplTest->rgwtMin[0x3])) < 0x0))
        goto L_3ea6;
    else
        goto L_3e9e;

L_3e9e:
    if (((LOWORD((((lpciPlanTemp[lpplTest->id] >> 0x3) & 0x3) * 0xd2)) + LOWORD(lpplTest->rgwtMin[0x3])) >= 0x3e8))
        goto L_3ef3;
    else
        goto L_3ea6;

L_3ea6:
    /* untranslated: branch hiword(LDistance2(words(rgptPlan[lpplSrc->id].y, rgptPlan[lpplSrc->id].x), words(rgptPlan[lpplTest->id].y,
     * rgptPlan[lpplTest->id].x))) > 0x2 ? L_3eed : L_3eda */

L_3eda:
    /* untranslated: branch hiword(callresult(int32_t)) < 0x2 ? L_3ee7 : L_3edf */

L_3edf:
    /* untranslated: branch loword(callresult(int32_t)) > 0x7100 ? L_3eed : L_3ee7 */

L_3ee7:
    return 0x1;

L_3eed:
    return 0x0;

L_3ef3:
    return 0x0;
}

int16_t FEnumPickUp(PLANET *lpplSrc, PLANET *lpplTest) {
L_3f00:
    if ((lpplTest->iPlayer != idPlayer))
        goto L_3f52;
    else
        goto L_3f18;

L_3f18:
    if ((HIWORD(lpplTest->rgwtMin[0x3]) < 0x0))
        goto L_3f52;
    else
        goto L_3f25;

L_3f25:
    if ((HIWORD(lpplTest->rgwtMin[0x3]) > 0x0))
        goto L_3f35;
    else
        goto L_3f2a;

L_3f2a:
    if ((LOWORD(lpplTest->rgwtMin[0x3]) <= 0x898))
        goto L_3f52;
    else
        goto L_3f35;

L_3f35:
    if ((lpplTest->fStarbase == 0x0))
        goto L_3f52;
    else
        goto L_3f4c;

L_3f4c:
    return 0x1;

L_3f52:
    return 0x0;
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
    double         t_merge_417b_0001;

L_3f5e:
    /* untranslated: dOffsetPlanTemp = part[2:0](mpicolgrbitBU[game.cPlanMax]) */
    lpciPlan = &(vlpbAiData[0x2]);
    lpciPlanTemp = &(vlpbAiData[dOffsetPlanTemp]);
    if ((lpplTest->isb == 0x1))
        goto L_3fe0;
    else
        goto L_3faa;

L_3faa:
    if ((lpplTest->isb == 0x3))
        goto L_3fe0;
    else
        goto L_3fbc;

L_3fbc:
    if ((lpplTest->isb == 0x6))
        goto L_3fe0;
    else
        goto L_3fce;

L_3fce:
    if ((lpplTest->isb != 0x8))
        goto L_3fe8;
    else
        goto L_3fe0;

L_3fe0:
    iMinLimit = 0xa;
    goto L_3fed;

L_3fe8:
    iMinLimit = 0x3e8;

L_3fed:
    if ((lpplTest->iPlayer != idPlayer))
        goto L_403d;
    else
        goto L_3ffc;

L_3ffc:
    if ((lpplTest->fStarbase == 0x0))
        goto L_403d;
    else
        goto L_4013;

L_4013:
    if ((((lpciPlan[lpplTest->id] >> 0x5) & 0x3) <= 0x0))
        goto L_4043;
    else
        goto L_403d;

L_403d:
    return 0x0;

L_4043:
    if ((HIWORD(lpplSrc->rgwtMin[0x0]) < 0x0))
        goto L_4085;
    else
        goto L_4050;

L_4050:
    if ((HIWORD(lpplSrc->rgwtMin[0x0]) > 0x0))
        goto L_4060;
    else
        goto L_4055;

L_4055:
    if ((LOWORD(lpplSrc->rgwtMin[0x0]) <= 0x2bc))
        goto L_4085;
    else
        goto L_4060;

L_4060:
    if ((((lpciPlanTemp[lpplTest->id] >> 0x8) & 0x1) != 0x0))
        goto L_4109;
    else
        goto L_4085;

L_4085:
    if ((HIWORD(lpplSrc->rgwtMin[0x1]) < 0x0))
        goto L_40c7;
    else
        goto L_4092;

L_4092:
    if ((HIWORD(lpplSrc->rgwtMin[0x1]) > 0x0))
        goto L_40a2;
    else
        goto L_4097;

L_4097:
    if ((LOWORD(lpplSrc->rgwtMin[0x1]) <= 0x2bc))
        goto L_40c7;
    else
        goto L_40a2;

L_40a2:
    if ((((lpciPlanTemp[lpplTest->id] >> 0x9) & 0x1) != 0x0))
        goto L_4109;
    else
        goto L_40c7;

L_40c7:
    if ((HIWORD(lpplSrc->rgwtMin[0x2]) < 0x0))
        goto L_41f8;
    else
        goto L_40d4;

L_40d4:
    if ((HIWORD(lpplSrc->rgwtMin[0x2]) > 0x0))
        goto L_40e4;
    else
        goto L_40d9;

L_40d9:
    if ((LOWORD(lpplSrc->rgwtMin[0x2]) <= 0x2bc))
        goto L_41f8;
    else
        goto L_40e4;

L_40e4:
    if ((((lpciPlanTemp[lpplTest->id] >> 0xa) & 0x1) == 0x0))
        goto L_41f8;
    else
        goto L_4109;

L_4109:
    iWarpSrc = IWarpMAFromLppl(lpplSrc, &(fTwoMA));
    if ((fTwoMA == 0x0))
        goto L_412b;
    else
        goto L_4127;

L_4127:
    iWarpSrc = (iWarpSrc + 0x1);

L_412b:
    iWarpDst = IWarpMAFromLppl(lpplTest, &(fTwoMA));
    if ((fTwoMA == 0x0))
        goto L_414d;
    else
        goto L_4149;

L_4149:
    iWarpDst = (iWarpDst + 0x1);

L_414d:
    if ((iWarpSrc >= iWarpDst))
        goto L_416b;
    else
        goto L_4158;

L_4158:
    /* untranslated: t_merge_417b_0001 = (double)dword ss:[bp-0x24] */
    goto L_417b;

L_416b:
    /* untranslated: ss:[bp-0x2c] = iWarpDst */
    /* untranslated: ss:[bp-0x2a] = signhiword(iWarpDst) */
    /* untranslated: t_merge_417b_0001 = (double)dword ss:[bp-0x2c] */

L_417b:
    dDistance = t_merge_417b_0001;
    dDistance = ((dDistance * dDistance) * 3.5);
    /* untranslated: 10-byte ss:[bp-0x26] = (dDistance * dDistance) */
    /* untranslated: call LDistance2(words(rgptPlan[lpplSrc->id].y, rgptPlan[lpplSrc->id].x), words(rgptPlan[lpplTest->id].y, rgptPlan[lpplTest->id].x)) ->
     * callresult(int32_t) */
    /* untranslated: ss:[bp-0x2e] = loword(callresult(int32_t)) */
    /* untranslated: ss:[bp-0x2c] = hiword(callresult(int32_t)) */
    __aFfcompp();
    /* untranslated: branch JA ? L_41f2 : L_41ec */

L_41ec:
    return 0x1;

L_41f2:
    return 0x0;

L_41f8:
    return 0x0;
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

L_4204:
    lMineral = 0x0;
    iWarpDst = 0x0;
    /* untranslated: dOffsetPlanTemp = part[2:0](mpicolgrbitBU[game.cPlanMax]) */
    lpciPlan = &(vlpbAiData[0x2]);
    plMinMax = ((uint8_t *)(vlpbAiData) + (dOffsetPlanTemp + (game.cPlanMax * 0x2)));
    if ((lpplTest->iPlayer == idPlayer))
        goto L_42be;
    else
        goto L_4263;

L_4263:
    if ((lpplTest->iPlayer == 0xffff))
        goto L_42be;
    else
        goto L_4270;

L_4270:
    if ((((lpciPlan[lpplTest->id] >> 0x5) & 0x3) > 0x0))
        goto L_42be;
    else
        goto L_429a;

L_429a:
    if ((GetRaceStat(rgplr[lpplTest->iPlayer], rsMajorAdv) != raMacintosh))
        goto L_42c4;
    else
        goto L_42be;

L_42be:
    return 0x0;

L_42c4:
    if ((lpplTest->fStarbase == 0x0))
        goto L_433b;
    else
        goto L_42db;

L_42db:
    /* untranslated: branch (part[123:2](rglpshdefSB[lpplTest->iPlayer][lpplTest->isb]) & 0xff) == 0x7 ? L_4319 : L_4310 */

L_4310:
    return 0x0;

L_4319:
    iWarpDst = IWarpMAFromLppl(lpplTest, &(fTwoMA));
    if ((fTwoMA == 0x0))
        goto L_433b;
    else
        goto L_4337;

L_4337:
    iWarpDst = (iWarpDst + 0x1);

L_433b:
    IWarpMAFromLppl(lpplSrc, &(fTwoMA));
    /* untranslated: iWarp = (callresult(int16_t) + 0x3) */
    /* untranslated: ss:[bp-0x3a] = (callresult(int16_t) + 0x3) */
    /* untranslated: ss:[bp-0x38] = signhiword((callresult(int16_t) + 0x3)) */
    /* untranslated: dDistance = (double)dword ss:[bp-0x3a] */
    if ((iWarp != iWarpDst))
        goto L_4375;
    else
        goto L_436f;

L_436f:
    return 0x0;

L_4375:
    dDistance = (dDistance * dDistance);
    dDistanceTgt = DGetDistance(rgptPlan[lpplSrc->id].x, rgptPlan[lpplSrc->id].y, rgptPlan[lpplTest->id].x, rgptPlan[lpplTest->id].y);
    __aFfcompp();
    /* untranslated: branch JA ? L_459a : L_43f2 */

L_43f2:
    if ((fTwoMA == 0x0))
        goto L_444c;
    else
        goto L_43fb;

L_43fb:
    /* untranslated: 10-byte ss:[bp-0x3c] = (double)*plMinMax */
    pow(0.875, (dDistanceTgt / dDistance));
    lMineral = __ftol();
    goto L_449a;

L_444c:
    /* untranslated: 10-byte ss:[bp-0x3c] = (double)*plMinMax */
    pow(0.75, (dDistanceTgt / dDistance));
    lMineral = __ftol();

L_449a:
    if ((lpplTest->uDefGuess > 0x63))
        goto L_44c3;
    else
        goto L_44b1;

L_44b1:
    if ((lpplTest->uPopGuess != 0x0))
        goto L_44c9;
    else
        goto L_44c3;

L_44c3:
    return 0x0;

L_44c9:
    /* untranslated: ss:[bp-0x3c] = loword(((loword((iWarp * iWarp)) - loword((iWarpDst * iWarpDst))) * (0x64 - (lpplTest->uDefGuess + 0x5)))) */
    /* untranslated: ss:[bp-0x3a] = 0x0 */
    /* untranslated: dMod = ((double)dword ss:[bp-0x3c] / 16000) */
    if ((0x3e8 >= ((lpplTest->uPopGuess + 0x19) * 0x4)))
        goto L_4544;
    else
        goto L_452f;

L_452f:
    goto L_4564;

L_4544:
    /* untranslated: ss:[bp-0x42] = ((lpplTest->uPopGuess + 0x19) * 0x4) */
    /* untranslated: ss:[bp-0x40] = 0x0 */

L_4564:
    lMinNeeded = __ftol();
    if ((HIWORD(lMinNeeded) > HIWORD(lMineral)))
        goto L_4594;
    else
        goto L_4581;

L_4581:
    if ((HIWORD(lMinNeeded) < HIWORD(lMineral)))
        goto L_458e;
    else
        goto L_4586;

L_4586:
    if ((LOWORD(lMinNeeded) > LOWORD(lMineral)))
        goto L_4594;
    else
        goto L_458e;

L_458e:
    return 0x1;

L_4594:
    return 0x0;

L_459a:
    return 0x0;
}

int16_t FEnumCalcEnemyPlanets(PLANET *lpplSrc, PLANET *lpplTest) {
L_45a6:
    if ((lpplTest->iPlayer == idPlayer))
        goto L_45d1;
    else
        goto L_45be;

L_45be:
    if ((lpplTest->iPlayer == 0xffff))
        goto L_45d1;
    else
        goto L_45cb;

L_45cb:
    return 0x1;

L_45d1:
    return 0x0;
}

int16_t iBuildCyberStarbase(PLANET *lppl) {
    int16_t ishdefSB;
    PROD    rgprod[64];

L_45de:
    if ((lppl->fStarbase != 0x0))
        goto L_4635;
    else
        goto L_45fe;

L_45fe:
    if ((PctPlanetDesirability(lppl, idPlayer) < 0xf))
        goto L_4635;
    else
        goto L_4618;

L_4618:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_463b;
    else
        goto L_4625;

L_4625:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_4635;
    else
        goto L_462a;

L_462a:
    if ((LOWORD(lppl->rgwtMin[0x3]) >= 0x1f4))
        goto L_463b;
    else
        goto L_4635;

L_4635:
    return 0xffff;

L_463b:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    if ((lppl->rgMinConc[0x0] <= 0xf))
        goto L_469b;
    else
        goto L_466c;

L_466c:
    if ((lppl->rgMinConc[0x1] <= 0xf))
        goto L_469b;
    else
        goto L_467e;

L_467e:
    if ((lppl->rgMinConc[0x2] <= 0xf))
        goto L_469b;
    else
        goto L_4690;

L_4690:
    ishdefSB = IshdefAiSBLatest();
    goto L_46a3;

L_469b:
    ishdefSB = IshdefAiSBLatestOF();

L_46a3:
    return ishdefSB;
}

void EnsureCyberAiShdefs(int16_t iroCur) {
    int16_t  low;
    int16_t  ish;
    int16_t  ishCur;
    int16_t  i;
    int16_t  high;
    SHDEF    shdef;
    uint16_t t_merge_4caf_0001;

L_4826:
    /* untranslated: branch ((part[123:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_48b9 : L_4842 */

L_4842:
    if ((LOWORD(rgshdef) == 0x5))
        goto L_48b9;
    else
        goto L_484c;

L_484c:
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) <= 0x1 ? L_48b9 : L_486b */

L_486b:
    /* untranslated: branch part[131:2](rgshdef) != 0x0 ? L_48b9 : L_4875 */

L_4875:
    /* untranslated: branch part[133:2](rgshdef) != 0x0 ? L_48b9 : L_487f */

L_487f:
    if ((game.turn <= 0x5))
        goto L_48b9;
    else
        goto L_4889;

L_4889:
    /* untranslated: shdef = part[0:147](rgshdef) */
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 0x0);

L_48b9:
    /* untranslated: branch ((part[123:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_48e8 : L_48cc */

L_48cc:
    /* untranslated: call FCreateAiShdef(0x0, 0x5, &vrgCyberAip[part[24:2](vrgCyberIshAip)]) -> callresult(int16_t) */

L_48e8:
    /* untranslated: branch ((part[711:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_497e : L_48fb */

L_48fb:
    if ((game.turn <= 0x1e))
        goto L_497e;
    else
        goto L_4905;

L_4905:
    if ((game.turn > 0x4b))
        goto L_4933;
    else
        goto L_490f;

L_490f:
    if ((FCreateAiShdef(0x4, 0x6, &(vrgCyberAip[vrgCyberIshAip])) != 0x0))
        goto L_497e;
    else
        goto L_4933;

L_4933:
    i = 0x5;
    goto L_4975;

L_493b:
    /* untranslated: branch FCreateAiShdef(0x4, 0x6, &vrgCyberAip[0x16:[(Random(i) * 0x2)+0x46b0]]) != 0x0 ? L_497e : L_496b */

L_496b:

L_4971:
    i = (i - 0x1);

L_4975:
    if ((i > 0x0))
        goto L_493b;
    else
        goto L_497e;

L_497e:
    /* untranslated: branch ((part[858:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_4a2f : L_4991 */

L_4991:
    /* untranslated: branch ((part[711:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_4a2f : L_49a4 */

L_49a4:
    /* untranslated: branch game.turn <= (part[713:2](rgshdef) + 0x14) ? L_4a2f : L_49b3 */

L_49b3:
    if ((game.turn > 0x4b))
        goto L_49e1;
    else
        goto L_49bd;

L_49bd:
    /* untranslated: branch FCreateAiShdef(0x5, 0x6, &vrgCyberAip[part[10:2](vrgCyberIshAip)]) != 0x0 ? L_4a2f : L_49e1 */

L_49e1:
    i = 0x5;
    goto L_4a26;

L_49e9:
    /* untranslated: branch FCreateAiShdef(0x5, 0x6, &vrgCyberAip[0x16:[((Random(i) + 0x5) * 0x2)+0x46b0]]) != 0x0 ? L_4a2f : L_4a1c */

L_4a1c:

L_4a22:
    i = (i - 0x1);

L_4a26:
    if ((i > 0x0))
        goto L_49e9;
    else
        goto L_4a2f;

L_4a2f:
    /* untranslated: branch ((part[417:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_4a68 : L_4a42 */

L_4a42:
    if ((game.turn <= 0x14))
        goto L_4a68;
    else
        goto L_4a4c;

L_4a4c:
    /* untranslated: call FCreateAiShdef(0x2, 0xb, &vrgCyberAip[part[20:2](vrgCyberIshAip)]) -> callresult(int16_t) */

L_4a68:
    /* untranslated: branch ((part[564:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_4ab9 : L_4a7b */

L_4a7b:
    /* untranslated: branch ((part[417:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_4ab9 : L_4a8e */

L_4a8e:
    /* untranslated: branch game.turn <= (part[419:2](rgshdef) + 0x14) ? L_4ab9 : L_4a9d */

L_4a9d:
    /* untranslated: call FCreateAiShdef(0x3, 0xb, &vrgCyberAip[part[22:2](vrgCyberIshAip)]) -> callresult(int16_t) */

L_4ab9:
    ish = 0x6;
    goto L_4d4d;

L_4ac1:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) == 0x0 ? L_4d49 : L_4adf */

L_4adf:
    if ((ish != 0x6))
        goto L_4af2;
    else
        goto L_4ae8;

L_4ae8:
    if ((game.turn > 0x28))
        goto L_4b14;
    else
        goto L_4af2;

L_4af2:
    /* untranslated: branch ((part[1005:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_4d49 : L_4b05 */

L_4b05:
    /* untranslated: branch game.turn <= (part[1007:2](rgshdef) + 0x1e) ? L_4d49 : L_4b14 */

L_4b14:
    ishCur = (ish + 0x2);
    i = 0x3;
    goto L_4b65;

L_4b25:
    /* untranslated: branch FCreateAiShdef(ishCur, 0x1d, &vrgCyberAip[0x16:[((Random(i) + 0x21) * 0x2)+0x46b0]]) == 0x0 ? L_4b61 : L_4b5a */

L_4b5a:
    ishCur = (ishCur - 0x1);
    goto L_4b6e;

L_4b61:
    i = (i - 0x1);

L_4b65:
    if ((i > 0x0))
        goto L_4b25;
    else
        goto L_4b6e;

L_4b6e:
    i = 0x4;
    goto L_4bb6;

L_4b76:
    /* untranslated: branch FCreateAiShdef(ishCur, 0x9, &vrgCyberAip[0x16:[((Random(i) + 0x1d) * 0x2)+0x46b0]]) == 0x0 ? L_4bb2 : L_4bab */

L_4bab:
    ishCur = (ishCur - 0x1);
    goto L_4bbf;

L_4bb2:
    i = (i - 0x1);

L_4bb6:
    if ((i > 0x0))
        goto L_4b76;
    else
        goto L_4bbf;

L_4bbf:
    i = 0x3;
    goto L_4c07;

L_4bc7:
    /* untranslated: branch FCreateAiShdef(ishCur, 0x9, &vrgCyberAip[0x16:[((Random(i) + 0x1a) * 0x2)+0x46b0]]) == 0x0 ? L_4c03 : L_4bfc */

L_4bfc:
    ishCur = (ishCur - 0x1);
    goto L_4c10;

L_4c03:
    i = (i - 0x1);

L_4c07:
    if ((i > 0x0))
        goto L_4bc7;
    else
        goto L_4c10;

L_4c10:
    if ((ishCur < ish))
        goto LBomber;
    else
        goto L_4c18;

L_4c18:

L_4c1e:
    high = (0x9 - LOWORD(((ishCur - ish) * 0x3)));
    low = (0x1a - (0x9 - LOWORD(((ishCur - ish) * 0x3))));

LCruiser:
    i = high;
    goto L_4c7e;

L_4c42:
    /* untranslated: branch FCreateAiShdef(ishCur, 0x7, &vrgCyberAip[0x16:[((Random(i) + low) * 0x2)+0x46b0]]) != 0x0 ? L_4c87 : L_4c74 */

L_4c74:

L_4c7a:
    i = (i - 0x1);

L_4c7e:
    if ((i > 0x0))
        goto L_4c42;
    else
        goto L_4c87;

L_4c87:
    ishCur = (ishCur - 0x1);
    if ((ishCur < ish))
        goto LBomber;
    else
        goto L_4c96;

L_4c96:
    high = 0x3;
    if ((ishCur != ish))
        goto L_4cac;
    else
        goto L_4ca6;

L_4ca6:
    t_merge_4caf_0001 = 0x11;
    goto L_4caf;

L_4cac:
    t_merge_4caf_0001 = 0x14;

L_4caf:
    low = t_merge_4caf_0001;
    goto LCruiser;

LBomber:
    /* untranslated: branch FCreateAiShdef((ish + 0x3), 0x1d, &vrgCyberAip[part[32:2](vrgCyberIshAip)]) != 0x0 ? L_4d49 : L_4cdc */

L_4cdc:
    /* untranslated: branch FCreateAiShdef((ish + 0x3), 0x9, &vrgCyberAip[part[30:2](vrgCyberIshAip)]) != 0x0 ? L_4d49 : L_4d03 */

L_4d03:
    /* untranslated: branch FCreateAiShdef((ish + 0x3), 0x13, &vrgCyberAip[part[28:2](vrgCyberIshAip)]) != 0x0 ? L_4d49 : L_4d2a */

L_4d2a:
    /* untranslated: call FCreateAiShdef((ish + 0x3), 0x13, &vrgCyberAip[part[26:2](vrgCyberIshAip)]) -> callresult(int16_t) */

L_4d49:
    ish = (ish + 0x4);

L_4d4d:
    if ((ish <= 0xa))
        goto L_4ac1;
    else
        goto L_4d56;

L_4d56:
    ish = 0xe;
    goto L_4eab;

L_4d5e:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) == 0x0 ? L_4ea7 : L_4d7c */

L_4d7c:
    if ((ish != 0xe))
        goto L_4d8f;
    else
        goto L_4d85;

L_4d85:
    if ((game.turn > 0x1e))
        goto L_4db1;
    else
        goto L_4d8f;

L_4d8f:
    /* untranslated: branch ((part[2181:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_4ea7 : L_4da2 */

L_4da2:
    /* untranslated: branch game.turn <= (part[2183:2](rgshdef) + 0x14) ? L_4ea7 : L_4db1 */

L_4db1:
    i = 0x7;
    goto L_4df5;

L_4db9:
    /* untranslated: branch FCreateAiShdef(ish, 0x9, &vrgCyberAip[0x16:[((Random(i) + 0x1a) * 0x2)+0x46b0]]) != 0x0 ? L_4dfe : L_4deb */

L_4deb:

L_4df1:
    i = (i - 0x1);

L_4df5:
    if ((i > 0x0))
        goto L_4db9;
    else
        goto L_4dfe;

L_4dfe:
    if ((i != 0x0))
        goto L_4e54;
    else
        goto L_4e07;

L_4e07:
    i = 0x9;
    goto L_4e4b;

L_4e0f:
    /* untranslated: branch FCreateAiShdef(ish, 0x7, &vrgCyberAip[0x16:[((Random(i) + 0x11) * 0x2)+0x46b0]]) != 0x0 ? L_4e54 : L_4e41 */

L_4e41:

L_4e47:
    i = (i - 0x1);

L_4e4b:
    if ((i > 0x0))
        goto L_4e0f;
    else
        goto L_4e54;

L_4e54:
    if ((i != 0x0))
        goto L_4ea7;
    else
        goto L_4e5d;

L_4e5d:
    i = 0xa;
    goto L_4e9e;

L_4e65:
    /* untranslated: branch FCreateAiShdef(ish, 0x6, &vrgCyberAip[0x16:[(Random(i) * 0x2)+0x46b0]]) != 0x0 ? L_4ea7 : L_4e94 */

L_4e94:

L_4e9a:
    i = (i - 0x1);

L_4e9e:
    if ((i > 0x0))
        goto L_4e65;
    else
        goto L_4ea7;

L_4ea7:
    ish = (ish + 0x1);

L_4eab:
    if ((ish <= 0xf))
        goto L_4d5e;
    else
        goto L_4eb4;

L_4eb4:
    return;
}

int16_t iAddAttackFleet(PLANET *lppl, int16_t iAttackStr, int16_t iBestDestroyer, int16_t iBestBattle, int16_t iBestSBDefender) {
    int16_t  fRet;
    int16_t  iMaxFactories;
    int16_t  iMaxMines;
    int16_t  iRand;
    uint16_t t_merge_4f41_0001;
    uint16_t t_merge_4fae_0001;
    uint16_t t_merge_4fdb_0001;

L_4eba:
    fRet = 0x0;
    iRand = Random(0x64);
    /* untranslated: ss:[bp-0xc] = CMaxOperableMines(lppl, idPlayer, 0x0) */
    /* untranslated: branch 0x0 <= (ss:[bp-0xc] - CMinesOperating(lppl)) ? L_4f13 : L_4f0d */

L_4f0d:
    t_merge_4f41_0001 = 0x0;
    goto L_4f41;

L_4f13:
    /* untranslated: ss:[bp-0xe] = CMaxOperableMines(lppl, idPlayer, 0x0) */
    /* untranslated: t_merge_4f41_0001 = (ss:[bp-0xe] - CMinesOperating(lppl)) */

L_4f41:
    iMaxMines = t_merge_4f41_0001;
    /* untranslated: ss:[bp-0xc] = CMaxOperableFactories(lppl, idPlayer, 0x0) */
    /* untranslated: branch 0x0 <= (ss:[bp-0xc] - CFactoriesOperating(lppl)) ? L_4f80 : L_4f7a */

L_4f7a:
    t_merge_4fae_0001 = 0x0;
    goto L_4fae;

L_4f80:
    /* untranslated: ss:[bp-0xe] = CMaxOperableFactories(lppl, idPlayer, 0x0) */
    /* untranslated: t_merge_4fae_0001 = (ss:[bp-0xe] - CFactoriesOperating(lppl)) */

L_4fae:
    iMaxFactories = t_merge_4fae_0001;
    Random(0x64);
    if ((iMaxMines < 0x64))
        goto L_4fd2;
    else
        goto L_4fc9;

L_4fc9:
    if ((iMaxFactories >= 0x64))
        goto L_4fd8;
    else
        goto L_4fd2;

L_4fd2:
    t_merge_4fdb_0001 = 0x5a;
    goto L_4fdb;

L_4fd8:
    t_merge_4fdb_0001 = 0x3c;

L_4fdb:
    /* untranslated: branch ss:[bp-0xc] >= t_merge_4fdb_0001 ? L_4feb : L_4fe5 */

L_4fe5:
    return 0x0;

L_4feb:
    if ((iBestBattle == 0xffff))
        goto L_5143;
    else
        goto L_4ff4;

L_4ff4:
    if ((iRand <= 0x32))
        goto L_5143;
    else
        goto L_4ffd;

L_4ffd:
    /* untranslated: branch ((part[123:2](rgshdef[((iBestBattle * 0x4) + 0x6)]) >> 0x9) & 0x1) != 0x0 ? L_5043 : L_5024 */

L_5024:
    AddItemToQueue(((iBestBattle * 0x4) + 0x6), 0x2, grobjFleet, 0x1);

L_5043:
    /* untranslated: branch ((part[123:2](rgshdef[((iBestBattle * 0x4) + 0x7)]) >> 0x9) & 0x1) != 0x0 ? L_5089 : L_506a */

L_506a:
    AddItemToQueue(((iBestBattle * 0x4) + 0x7), 0x2, grobjFleet, 0x1);

L_5089:
    /* untranslated: branch ((part[123:2](rgshdef[((iBestBattle * 0x4) + 0x8)]) >> 0x9) & 0x1) != 0x0 ? L_50e3 : L_50b0 */

L_50b0:
    if ((Random(0x64) >= 0x4b))
        goto L_50e3;
    else
        goto L_50c4;

L_50c4:
    AddItemToQueue(((iBestBattle * 0x4) + 0x8), 0x1, grobjFleet, 0x1);

L_50e3:
    /* untranslated: branch ((part[123:2](rgshdef[((iBestBattle * 0x4) + 0x9)]) >> 0x9) & 0x1) != 0x0 ? L_513d : L_510a */

L_510a:
    if ((Random(0x64) >= 0x32))
        goto L_513d;
    else
        goto L_511e;

L_511e:
    AddItemToQueue(((iBestBattle * 0x4) + 0x9), 0x1, grobjFleet, 0x1);

L_513d:
    return 0x1;

L_5143:
    if ((iBestSBDefender == 0xffff))
        goto L_5172;
    else
        goto L_514c;

L_514c:
    if ((iRand <= 0x19))
        goto L_5172;
    else
        goto L_5155;

L_5155:
    AddItemToQueue(iBestSBDefender, 0x1, grobjFleet, 0x1);
    return 0x2;

L_5172:
    if ((iBestDestroyer == 0xffff))
        goto L_5198;
    else
        goto L_517b;

L_517b:
    AddItemToQueue(iBestDestroyer, 0x1, grobjFleet, 0x1);
    return 0x3;

L_5198:
    return 0x0;
}

void TargetCyberArmada(FLEET *lpfl) {
    FLEET   *lpflTarget;
    ORDER    ord;
    PLANET  *lppl;
    int16_t  cshBomb;
    int16_t  cshWar;
    PLANET  *lpplTarget;
    uint16_t t_545d;

L_51a4:
    if ((lpfl->cord <= 0x1))
        goto LTryNewTarget;
    else
        goto L_51ba;

L_51ba:
    ord = lpfl->lpplord->rgord[0x1];
    /* untranslated: branch hiword(LDistance2(words(lpfl->pt.y, lpfl->pt.x), words(ord.pt.y, ord.pt.x))) < 0x0 ? L_5221 : L_51fe */

L_51fe:
    /* untranslated: branch hiword(callresult(int32_t)) > 0x0 ? L_520b : L_5203 */

L_5203:
    /* untranslated: branch loword(callresult(int32_t)) <= 0xf424 ? L_5221 : L_520b */

L_520b:
    if ((ord.grobj == grobjFleet))
        goto LTryNewTarget;
    else
        goto L_521b;

L_521b:

L_5221:
    if ((ord.grobj == grobjFleet))
        goto L_567b;
    else
        goto L_5231;

L_5231:

L_5237:
    if ((ord.grobj != grobjPlanet))
        goto LTryNewTarget;
    else
        goto L_524a;

L_524a:
    lppl = LpplFromId(ord.id);
    if ((LOWORD(lppl) != 0x0))
        goto L_526d;
    else
        goto L_5264;

L_5264:
    if ((HIWORD(lppl) == 0x0))
        goto L_567b;
    else
        goto L_526d;

L_526d:
    if ((lppl->iPlayer == 0xffff))
        goto L_52a0;
    else
        goto L_527a;

L_527a:
    if ((lppl->iPlayer != idPlayer))
        goto L_567b;
    else
        goto L_5289;

L_5289:
    if ((lppl->fStarbase != 0x0))
        goto L_567b;
    else
        goto L_52a0;

L_52a0:
    if ((lppl->turn != game.turn))
        goto L_567b;
    else
        goto L_52ac;

L_52ac:

LTryNewTarget:
    cshWar = 0x0;
    cshWar = (cshWar + lpfl->rgcsh[0x6]);
    cshWar = (cshWar + lpfl->rgcsh[0x7]);
    cshWar = (cshWar + (lpfl->rgcsh[0x8] * 0x2));
    cshWar = (cshWar + lpfl->rgcsh[0xa]);
    cshWar = (cshWar + lpfl->rgcsh[0xb]);
    cshWar = (cshWar + (lpfl->rgcsh[0xc] * 0x2));
    cshBomb = (lpfl->rgcsh[0x9] + lpfl->rgcsh[0xd]);
    lpfl->fMark = 0x1;
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    if ((lpfl->idPlanet != 0xffff))
        goto L_5350;
    else
        goto L_533b;

L_533b:
    MoveToNearestPlanetOrEnemy(lpfl, 0x1c2);
    goto L_567b;

L_5350:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((lppl->iPlayer != idPlayer))
        goto L_558f;
    else
        goto L_5374;

L_5374:
    if ((cshWar < (LOWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_54f9;
    else
        goto L_5382;

L_5382:
    if ((cshBomb < (HIWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_54f9;
    else
        goto TargetPotentArmada;

TargetPotentArmada:
    if ((game.fAisBand == 0x0))
        goto L_53c5;
    else
        goto L_53a6;

L_53a6:
    lpplTarget = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
    goto L_53cf;

L_53c5:
    lpplTarget = 0x0;

L_53cf:
    if ((LOWORD(lpplTarget) != 0x0))
        goto TargetEveryArmada;
    else
        goto L_53d8;

L_53d8:
    if ((HIWORD(lpplTarget) != 0x0))
        goto TargetEveryArmada;
    else
        goto L_53e1;

L_53e1:
    lpplTarget = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);

TargetEveryArmada:
    if ((LOWORD(lpplTarget) != 0x0))
        goto L_540f;
    else
        goto L_5406;

L_5406:
    if ((HIWORD(lpplTarget) == 0x0))
        goto L_549f;
    else
        goto L_540f;

L_540f:
    vlpbAiPlanet[((lpplTarget->id * 0x10) + 0xa)] = (vlpbAiPlanet[((lpplTarget->id * 0x10) + 0xa)] | 0x80);
    ord.id = lpplTarget->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lpplTarget->id].x;
    ord.pt.y = rgptPlan[lpplTarget->id].y;

FinishTargeting:
    /* untranslated: t_545d = part[6:2](ord) */
    ord.grTask = grTaskNone;
    /* untranslated: part[6:2](ord) = ((((t_545d & 0xfff0) | 0x0) & 0xefff) | 0x1000) */
    /* untranslated: part[6:2](ord) = ((((((t_545d & 0xfff0) | 0x0) & 0xefff) | 0x1000) & 0xff0f) | 0x40) */
    if ((FMoveAiFleet(lpfl, &(ord), 0x0) == 0x0))
        goto L_567b;
    else
        goto L_5496;

L_5496:

L_549f:
    lpflTarget = LpflFindClosestEnum(lpfl, FEnumCalcEnemyFleets);
    if ((LOWORD(lpflTarget) != 0x0))
        goto L_54cd;
    else
        goto L_54c4;

L_54c4:
    if ((HIWORD(lpflTarget) == 0x0))
        goto L_567b;
    else
        goto L_54cd;

L_54cd:
    ord.id = LOWORD(lpflTarget);
    ord.grobj = grobjFleet;
    ord.pt.x = lpflTarget->pt.x;
    ord.pt.y = lpflTarget->pt.y;
    goto FinishTargeting;

L_54f9:
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) <= 0x1 ? L_567b : L_5518 */

L_5518:
    if ((cshWar > ((LOWORD(vrgAiArmadaPotency) & 0xff) * 0x2)))
        goto L_5531;
    else
        goto L_5528;

L_5528:
    if ((cshWar < 0x3c))
        goto L_567b;
    else
        goto L_5531;

L_5531:
    if ((Random(0xa) < 0x5))
        goto TargetPotentArmada;
    else
        goto L_5545;

L_5545:
    if ((cshWar <= LOWORD(((LOWORD(vrgAiArmadaPotency) & 0xff) * 0x3))))
        goto L_556c;
    else
        goto L_5558;

L_5558:
    if ((Random(0xa) < 0x7))
        goto TargetPotentArmada;
    else
        goto L_556c;

L_556c:
    if ((cshWar <= 0x78))
        goto L_567b;
    else
        goto L_5575;

L_5575:
    if ((Random(0xa) < 0x7))
        goto TargetPotentArmada;
    else
        goto L_5586;

L_5586:

L_558f:
    /* untranslated: branch cshWar < (part[1:2](vrgAiArmadaPotency) & 0xff) ? L_55ab : L_559d */

L_559d:
    /* untranslated: branch cshBomb >= (part[3:2](vrgAiArmadaPotency) & 0xff) ? L_5668 : L_55ab */

L_55ab:
    ClearAiCurrentTask(lpfl, 0x0);
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) <= 0x1 ? L_5646 : L_55dc */

L_55dc:
    if ((cshWar <= ((LOWORD(vrgAiArmadaPotency) & 0xff) * 0x2)))
        goto L_5600;
    else
        goto L_55ec;

L_55ec:
    if ((Random(0xa) < 0x5))
        goto TargetPotentArmada;
    else
        goto L_5600;

L_5600:
    if ((cshWar <= ((LOWORD(vrgAiArmadaPotency) & 0xff) * 0x4)))
        goto L_5626;
    else
        goto L_5612;

L_5612:
    if ((Random(0xa) < 0x7))
        goto TargetPotentArmada;
    else
        goto L_5626;

L_5626:
    if ((cshWar <= 0x78))
        goto L_5646;
    else
        goto L_562f;

L_562f:
    if ((Random(0xa) < 0x7))
        goto TargetPotentArmada;
    else
        goto L_5640;

L_5640:

L_5646:
    lpplTarget = LpplFindClosestEnum(lppl, FEnumOurStarbase);
    goto TargetEveryArmada;

L_5668:
    if ((lppl->iPlayer == 0xffff))
        goto TargetPotentArmada;
    else
        goto L_5672;

L_5672:

L_567b:
    return;
}
