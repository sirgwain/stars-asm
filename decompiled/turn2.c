#include "common.h"

void Produce() {
    int32_t   lResCur;
    int16_t   cMax;
    int32_t   rgResAvail[4];
    int16_t   iprodCur;
    int16_t   mdStatus;
    int16_t   cBuilt;
    int16_t   fNoResearch;
    PLANET   *lppl;
    int16_t   i;
    MessageId idm;
    PROD      prodPartial;
    int16_t   fPrevProdIsAlch;
    int16_t   fAutoBuildDone;
    int32_t   lResearchTake;
    PROD     *lpprod;
    PLANET   *lpplMac;
    int16_t   cMax2;

L_0000:
    MineMinerals();
    i = 0;
    goto L_001a;

L_0016:
    i = (i + 1);

L_001a:
    if ((i >= game.cPlayer))
        goto L_003d;
    else
        goto L_0025;

L_0025:
    rgplr[i].lResLastYear = 0;
    goto L_0016;

L_003d:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_0c55;

L_0068:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_016e;
    else
        goto L_0075;

L_0075:
    if ((HIWORD(lppl->lpplprod) != 0x0))
        goto L_016e;
    else
        goto L_007f;

L_007f:
    if ((lppl->iPlayer == -1))
        goto L_0c51;
    else
        goto L_008c;

L_008c:
    FSendPlrMsg2(lppl->iPlayer, 0x3f, lppl->id, lppl->id, 0x0);
    lResCur = (uint32_t)(CResourcesAtPlanet(lppl, lppl->iPlayer));
    if ((LOWORD(lResCur) != 0x0))
        goto L_00dd;
    else
        goto L_00d4;

L_00d4:
    if ((HIWORD(lResCur) == 0x0))
        goto L_0150;
    else
        goto L_00dd;

L_00dd:
    if ((vrgPlanResExtra[lppl->id] == 0x0))
        goto L_0150;
    else
        goto L_00fa;

L_00fa:
    /* untranslated: lResCur = (lResCur + (int32_t)((uint32_t)(lResCur * words(0x0, vrgPlanResExtra[lppl->id])) / words((0x0 + HIWORD(lResCur)),
     * (vrgPlanResExtra[lppl->id] + LOWORD(lResCur))))) */

L_0150:
    rgplr[lppl->iPlayer].lResLastYear = (rgplr[lppl->iPlayer].lResLastYear + lResCur);

L_016e:
    if ((lppl->iPlayer == -1))
        goto L_0c51;
    else
        goto L_017b;

L_017b:
    if ((lppl->lpplprod->iprodMac == 0x0))
        goto L_0c51;
    else
        goto L_018e;

L_018e:

L_0194:
    /* untranslated: fNoResearch = (loword((uint32_t)(words(*(lppl+0x18), *(lppl+0x1a)) >> 0x17)) & 0x1) */
    i = 0;
    goto L_01ea;

L_01b8:
    rgResAvail[i] = lppl->rgwtMin[i];
    i = (i + 1);

L_01ea:
    if ((i < 3))
        goto L_01b8;
    else
        goto L_01f3;

L_01f3:
    lResCur = (uint32_t)(CResourcesAtPlanet(lppl, lppl->iPlayer));
    if ((LOWORD(lResCur) != 0x0))
        goto L_0221;
    else
        goto L_0218;

L_0218:
    if ((HIWORD(lResCur) == 0x0))
        goto L_0294;
    else
        goto L_0221;

L_0221:
    if ((vrgPlanResExtra[lppl->id] == 0x0))
        goto L_0294;
    else
        goto L_023e;

L_023e:
    /* untranslated: lResCur = (lResCur + (int32_t)((uint32_t)(lResCur * words(0x0, vrgPlanResExtra[lppl->id])) / words((0x0 + HIWORD(lResCur)),
     * (vrgPlanResExtra[lppl->id] + LOWORD(lResCur))))) */

L_0294:
    rgResAvail[3] = lResCur;
    if ((((rgplr[lppl->iPlayer].wFlags >> 0x2) & 0x1) == 0x0))
        goto L_02e4;
    else
        goto L_02c1;

L_02c1:
    rgResAvail[3] = (int32_t)(((int32_t)((rgResAvail[3] * 4)) / 0x5));

L_02e4:
    if ((LOWORD(rgResAvail[0x3]) != 0x0))
        goto L_02f9;
    else
        goto L_02ed;

L_02ed:
    if ((HIWORD(rgResAvail[0x3]) == 0x0))
        goto L_0c51;
    else
        goto L_02f3;

L_02f3:

L_02f9:
    if ((fNoResearch == 0))
        goto L_030f;
    else
        goto L_0302;

L_0302:
    lResearchTake = 0;
    goto L_036c;

L_030f:
    lResearchTake = (int32_t)(((uint32_t)((rgResAvail[3] * (uint32_t)((uint16_t)(rgplr[lppl->iPlayer].pctResearch)))) / 0x64));
    rgResAvail[3] = (rgResAvail[3] - lResearchTake);
    rgplr[lppl->iPlayer].lResLastYear = (rgplr[lppl->iPlayer].lResLastYear + lResearchTake);

L_036c:
    fAutoBuildDone = 1;

TopOfQueue:
    fPrevProdIsAlch = 0;
    iprodCur = 0;

L_037b:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_0392;
    else
        goto L_0388;

L_0388:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_0b9a;
    else
        goto L_0392;

L_0392:
    if ((iprodCur >= lppl->lpplprod->iprodMac))
        goto L_0b9a;
    else
        goto L_03a8;

L_03a8:
    lpprod = &(lppl->lpplprod[(iprodCur + 0x1)]);
    if ((0x0 > 0x0))
        goto L_03ef;
    else
        goto L_03df;

L_03df:
    if ((0x0 < 0x0))
        goto RemoveFromQueue;
    else
        goto L_03e4;

L_03e4:
    if ((lpprod->cItem <= 0x0))
        goto RemoveFromQueue;
    else
        goto L_03e9;

L_03e9:

L_03ef:
    if ((lpprod->grobj != 0x1))
        goto L_07fc;
    else
        goto L_040f;

L_040f:
    if ((0x0 != 0x0))
        goto L_07fc;
    else
        goto L_0417;

L_0417:
    if ((0x0 < 0x0))
        goto L_0471;
    else
        goto L_0437;

L_0437:
    if ((0x0 > 0x0))
        goto L_0444;
    else
        goto L_043c;

L_043c:
    if ((lpprod->iItem < 0x12))
        goto L_0471;
    else
        goto L_0444;

L_0444:
    if ((0x0 < 0x0))
        goto L_0499;
    else
        goto L_0464;

L_0464:
    if ((0x0 > 0x0))
        goto L_0471;
    else
        goto L_0469;

L_0469:
    if ((lpprod->iItem <= 0x1a))
        goto L_0499;
    else
        goto L_0471;

L_0471:
    if ((lpprod->iItem != 0x1b))
        goto L_04eb;
    else
        goto L_0491;

L_0491:
    if ((0x0 != 0x0))
        goto L_04eb;
    else
        goto L_0499;

L_0499:
    /* untranslated: branch (loword((uint32_t)(words(*(lppl+0x18), *(lppl+0x1a)) >> 0xc)) & 0x1f) != 0x1f ? L_04c2 : L_04ba */

L_04ba:
    if ((0x0 == 0x0))
        goto L_07fc;
    else
        goto L_04c2;

L_04c2:
    FSendPlrMsg2(lppl->iPlayer, 0xb9, lppl->id, lppl->id, 0x0);
    goto RemoveFromQueue;

L_04eb:
    if ((0x0 < 0x0))
        goto L_059a;
    else
        goto L_050b;

L_050b:
    if ((0x0 > 0x0))
        goto L_0518;
    else
        goto L_0510;

L_0510:
    if ((lpprod->iItem < 0xe))
        goto L_059a;
    else
        goto L_0518;

L_0518:
    if ((0x0 > 0x0))
        goto L_059a;
    else
        goto L_0538;

L_0538:
    if ((0x0 < 0x0))
        goto L_0545;
    else
        goto L_053d;

L_053d:
    if ((lpprod->iItem > 0x11))
        goto L_059a;
    else
        goto L_0545;

L_0545:
    if ((IWarpMAFromLppl(lppl, 0x0) == 0))
        goto L_0571;
    else
        goto L_055f;

L_055f:
    if ((lppl->idFling != 0x0))
        goto L_07fc;
    else
        goto L_0571;

L_0571:
    FSendPlrMsg2(lppl->iPlayer, 0x129, lppl->id, lppl->id, 0x0);
    goto RemoveFromQueue;

L_059a:
    if ((lpprod->iItem != 0x7))
        goto L_06a5;
    else
        goto L_05ba;

L_05ba:
    if ((0x0 != 0x0))
        goto L_06a5;
    else
        goto L_05c2;

L_05c2:
    cMax = CMaxFactories(lppl, lppl->iPlayer);
    cMax2 = CMaxOperableFactories(lppl, lppl->iPlayer, 0x1);
    if ((cMax2 <= cMax))
        goto L_0607;
    else
        goto L_0601;

L_0601:
    cMax = cMax2;

L_0607:
    /* untranslated: cMax = (cMax - (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff)) */

LCantBuildP:
    idm = idmHasOrdersBuildPlanetaryInstallationsBeyondMaximu;

LCantBuildP2:
    if ((cMax >= lpprod->cItem))
        goto L_07fc;
    else
        goto L_0640;

L_0640:
    FSendPlrMsg2(lppl->iPlayer, idm, lppl->id, lppl->id, 0x0);
    if ((cMax <= 0))
        goto RemoveFromQueue;
    else
        goto L_0668;

L_0668:

L_0671:
    /* untranslated: lpprod->cItem = loword((int32_t)(words((cMax & 0x3ff), 0x0) << 0x0)) */
    /* untranslated: *(lpprod+0x2) = ((*(lpprod+0x2) & 0xffff) | hiword((int32_t)(words((cMax & 0x3ff), 0x0) << 0x0))) */

L_06a5:
    if ((lpprod->iItem != 0x8))
        goto L_0734;
    else
        goto L_06c5;

L_06c5:
    if ((0x0 != 0x0))
        goto L_0734;
    else
        goto L_06cd;

L_06cd:
    cMax = CMaxMines(lppl, lppl->iPlayer);
    cMax2 = CMaxOperableMines(lppl, lppl->iPlayer, 0x1);
    if ((cMax2 <= cMax))
        goto L_0712;
    else
        goto L_070c;

L_070c:
    cMax = cMax2;

L_0712:
    /* untranslated: cMax = (cMax - (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff)) */
    goto LCantBuildP;

L_0734:
    if ((lpprod->iItem != 0x9))
        goto L_07bb;
    else
        goto L_0754;

L_0754:
    if ((0x0 != 0x0))
        goto L_07bb;
    else
        goto L_075c;

L_075c:
    cMax = CMaxDefenses(lppl, lppl->iPlayer);
    cMax2 = CMaxOperableDefenses(lppl, lppl->iPlayer, 0x1);
    if ((cMax2 <= cMax))
        goto L_07a1;
    else
        goto L_079b;

L_079b:
    cMax = cMax2;

L_07a1:
    cMax = (cMax - lppl->cDefenses);
    goto LCantBuildP;

L_07bb:
    if ((lpprod->iItem != 0xc))
        goto L_07fc;
    else
        goto L_07db;

L_07db:
    if ((0x0 != 0x0))
        goto L_07fc;
    else
        goto L_07e3;

L_07e3:
    cMax = IpctCanTerraformLppl(lppl);
    idm = idmHasOrdersTerraformBeyondMaximumAllowedOrders;
    goto LCantBuildP2;

L_07fc:
    if ((lpprod->iItem != 0x3))
        goto L_0871;
    else
        goto L_081c;

L_081c:
    if ((0x0 != 0x0))
        goto L_0871;
    else
        goto L_0824;

L_0824:
    if ((lpprod->grobj != 0x1))
        goto L_0871;
    else
        goto L_0844;

L_0844:
    if ((0x0 != 0x0))
        goto L_0871;
    else
        goto L_084c;

L_084c:
    if ((iprodCur >= (lppl->lpplprod->iprodMac + 0xffff)))
        goto L_0871;
    else
        goto L_0865;

L_0865:
    fPrevProdIsAlch = 1;
    iprodCur = (iprodCur + 1);
    goto L_037b;

L_0871:
    prodPartial.cItem = 0x0;
    HIWORD(prodPartial) = ((HIWORD(prodPartial) & 0xffff) | 0x0);
    cBuilt = CBuildProdItem(lppl, lpprod, &(prodPartial), rgResAvail, fPrevProdIsAlch, &(mdStatus), 0x0);
    if ((fAutoBuildDone == 0))
        goto L_08d3;
    else
        goto L_08bc;

L_08bc:
    if ((mdStatus == 3))
        goto L_08ce;
    else
        goto L_08c5;

L_08c5:
    if ((mdStatus != 4))
        goto L_08d3;
    else
        goto L_08ce;

L_08ce:
    fAutoBuildDone = 0;

L_08d3:
    if ((cBuilt <= 0))
        goto L_09a0;
    else
        goto L_08dc;

L_08dc:
    if ((FBuildObject(lppl, lpprod->grobj, lpprod->iItem, cBuilt, rgResAvail) != 0))
        goto L_09a0;
    else
        goto L_092b;

L_092b:
    if ((lpprod->grobj != 0x1))
        goto L_0980;
    else
        goto L_094b;

L_094b:
    if ((0x0 != 0x0))
        goto L_0980;
    else
        goto L_0953;

L_0953:
    if ((0x0 < 0x0))
        goto L_09a0;
    else
        goto L_0973;

L_0973:
    if ((0x0 > 0x0))
        goto L_0980;
    else
        goto L_0978;

L_0978:
    if ((lpprod->iItem < 0x7))
        goto L_09a0;
    else
        goto L_0980;

L_0980:
    lpprod->cItem = 0x0;
    *(lpprod + 0x2) = ((*(lpprod + 0x2) & 0xffff) | 0x0);

L_09a0:
    if ((lppl->iPlayer != -1))
        goto L_09c7;
    else
        goto L_09ad;

L_09ad:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_09c7;
    else
        goto L_09ba;

L_09ba:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto TopOfQueue;
    else
        goto L_09c1;

L_09c1:

L_09c7:
    if ((mdStatus != 0))
        goto L_0aa6;
    else
        goto RemoveFromQueue;

RemoveFromQueue:
    if ((lppl->lpplprod->iprodMac != (fPrevProdIsAlch + 1)))
        goto L_0a13;
    else
        goto L_09eb;

L_09eb:
    FreePl(lppl->lpplprod);
    lppl->lpplprod = 0x0;
    goto L_0b9a;

L_0a13:
    if ((iprodCur >= (lppl->lpplprod->iprodMac + 0xffff)))
        goto L_0a89;
    else
        goto L_0a2c;

L_0a2c:
    fmemmove(&(lppl->lpplprod[((iprodCur - fPrevProdIsAlch) + 0x1)]), &(lppl->lpplprod[((iprodCur + 1) + 0x1)]),
             (((lppl->lpplprod->iprodMac - iprodCur) + 0xffff) * 0x4));

L_0a89:
    lppl->lpplprod->iprodMac = (lppl->lpplprod->iprodMac - LOBYTE((fPrevProdIsAlch + 0x1)));
    iprodCur = (iprodCur - (fPrevProdIsAlch + 1));
    goto L_0b8e;

L_0aa6:
    if ((mdStatus < 5))
        goto L_0b8e;
    else
        goto L_0aaf;

L_0aaf:
    if ((0x0 < 0x0))
        goto L_0b9a;
    else
        goto L_0ac3;

L_0ac3:
    if ((0x0 > 0x0))
        goto L_0ad0;
    else
        goto L_0ac8;

L_0ac8:
    if ((prodPartial.cItem <= 0x0))
        goto L_0b9a;
    else
        goto L_0ad0;

L_0ad0:
    if ((lppl->lpplprod->iprodMac != lppl->lpplprod->iprodMax))
        goto L_0b2b;
    else
        goto L_0afb;

L_0afb:
    lppl->lpplprod = LpplReAlloc(lppl->lpplprod, (lppl->lpplprod->iprodMac + 0x1));

L_0b2b:
    fmemmove(&(lppl->lpplprod->rgprod[0x1]), &(lppl->lpplprod->rgprod), (lppl->lpplprod->iprodMac * 0x4));
    lppl->lpplprod->rgprod[0] = prodPartial;
    lppl->lpplprod->iprodMac = (lppl->lpplprod->iprodMac + 0x1);

L_0b8e:
    iprodCur = (iprodCur + 1);
    fPrevProdIsAlch = 0;
    goto L_037b;

L_0b9a:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_0bb1;
    else
        goto L_0ba7;

L_0ba7:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_0bd0;
    else
        goto L_0bb1;

L_0bb1:
    if ((iprodCur < lppl->lpplprod->iprodMac))
        goto L_0bf3;
    else
        goto L_0bc7;

L_0bc7:
    if ((fAutoBuildDone == 0))
        goto L_0bf3;
    else
        goto L_0bd0;

L_0bd0:
    FSendPlrMsg2(lppl->iPlayer, 0x3e, lppl->id, lppl->id, 0x0);

L_0bf3:
    i = 0;
    goto L_0c2d;

L_0bfb:
    lppl->rgwtMin[i] = rgResAvail[i];
    i = (i + 1);

L_0c2d:
    if ((i < 3))
        goto L_0bfb;
    else
        goto L_0c36;

L_0c36:
    rgplr[lppl->iPlayer].lResLastYear = (rgplr[lppl->iPlayer].lResLastYear + rgResAvail[3]);

L_0c51:
    lppl = (lppl + 0x1);

L_0c55:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_0068;
    else
        goto L_0c63;

L_0c63:
    UpdatePopulations();
    UpdateResearchStatus(0x1);
    if ((game.fNoRandom != 0x0))
        goto L_0c8c;
    else
        goto L_0c87;

L_0c87:
    RandomEvents();

L_0c8c:
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
    uint16_t t_merge_0d4c_0001;
    int32_t  t_call_0ea9;
    int32_t  t_merge_126a_0001_wide;
    uint16_t t_merge_1410_0001;
    int32_t  t_merge_15f8_0001_wide;
    uint16_t t_merge_181f_0001;
    uint16_t t_merge_185e_0001;
    uint16_t t_merge_189c_0001;

L_0c92:
    cAlchemy = 0;
    LOWORD(pctInitial) = lpprod->pct;
    HIWORD(pctInitial) = 0x0;
    prod = lpprod;
    GetProductionCosts(lppl, lpprod, rgCost, lppl->iPlayer, 0x1);
    cBuilt = 0;
    if ((prod.grobj != 0x1))
        goto L_0d49;
    else
        goto L_0d12;

L_0d12:
    if ((0x0 != 0x0))
        goto L_0d49;
    else
        goto L_0d1a;

L_0d1a:
    if ((0x0 > 0x0))
        goto L_0d49;
    else
        goto L_0d36;

L_0d36:
    if ((0x0 < 0x0))
        goto L_0d43;
    else
        goto L_0d3b;

L_0d3b:
    if ((prod.iItem >= 0x7))
        goto L_0d49;
    else
        goto L_0d43;

L_0d43:
    t_merge_0d4c_0001 = 0x1;
    goto L_0d4c;

L_0d49:
    t_merge_0d4c_0001 = 0x0;

L_0d4c:
    fAutoBuild = t_merge_0d4c_0001;
    if ((fAutoBuild == 0))
        goto L_102c;
    else
        goto L_0d58;

L_0d58:
    cMax = 1000;
    goto L_0f2c;

L_0d74:
    iobjOther = 0x8;
    /* untranslated: ss:[bp-0x56] = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff) */
    /* untranslated: cMax = (CMaxOperableMines(lppl, lppl->iPlayer, 0x1) - ss:[bp-0x56]) */
    goto L_0f9f;

L_0dbe:
    iobjOther = 0x7;
    /* untranslated: ss:[bp-0x56] = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff) */
    /* untranslated: cMax = (CMaxOperableFactories(lppl, lppl->iPlayer, 0x1) - ss:[bp-0x56]) */
    goto L_0f9f;

L_0e08:
    iobjOther = 0x9;
    /* untranslated: ss:[bp-0x56] = lppl->cDefenses */
    /* untranslated: cMax = (CMaxOperableDefenses(lppl, lppl->iPlayer, 0x1) - ss:[bp-0x56]) */
    goto L_0f9f;

L_0e4a:
    iobjOther = 0xb;
    goto L_0f9f;

L_0e57:
    iobjOther = 0xc;
    cMax = IpctCanTerraformLppl(lppl);
    if ((cMax <= 0))
        goto L_0f9f;
    else
        goto L_0e7b;

L_0e7b:
    if ((prod.iItem != 0x4))
        goto L_0f9f;
    else
        goto L_0e97;

L_0e97:
    if ((0x0 != 0x0))
        goto L_0f9f;
    else
        goto L_0e9f;

L_0e9f:
    t_call_0ea9 = ChgPopFromPlanet(lppl, 0x0);
    if ((HIWORD(t_call_0ea9) < 0x0))
        goto L_0f9f;
    else
        goto L_0eb9;

L_0eb9:
    if ((HIWORD(t_call_0ea9) > 0x0))
        goto L_0ec6;
    else
        goto L_0ebe;

L_0ebe:
    if ((LOWORD(t_call_0ea9) < 0x0))
        goto L_0f9f;
    else
        goto L_0ec6;

L_0ec6:
    if ((PctPlanetDesirability(lppl, lppl->iPlayer) <= 0))
        goto L_0f9f;
    else
        goto L_0ee3;

L_0ee3:
    cMax = 0;

L_0eeb:
    iobjOther = 0x11;
    if ((IWarpMAFromLppl(lppl, 0x0) == 0))
        goto L_0f21;
    else
        goto L_0f0f;

L_0f0f:
    if ((lppl->idFling != 0x0))
        goto L_0f9f;
    else
        goto L_0f21;

L_0f21:
    cMax = 0;

L_0f2c:
    if ((prod.iItem != 0x0))
        goto L_0f3c;
    else
        goto L_0f34;

L_0f34:
    if ((0x0 == 0x0))
        goto L_0d74;
    else
        goto L_0f3c;

L_0f3c:
    if ((prod.iItem != 0x1))
        goto L_0f4c;
    else
        goto L_0f44;

L_0f44:
    if ((0x0 == 0x0))
        goto L_0dbe;
    else
        goto L_0f4c;

L_0f4c:
    if ((prod.iItem != 0x2))
        goto L_0f5c;
    else
        goto L_0f54;

L_0f54:
    if ((0x0 == 0x0))
        goto L_0e08;
    else
        goto L_0f5c;

L_0f5c:
    if ((prod.iItem != 0x3))
        goto L_0f6c;
    else
        goto L_0f64;

L_0f64:
    if ((0x0 == 0x0))
        goto L_0e4a;
    else
        goto L_0f6c;

L_0f6c:
    if ((prod.iItem != 0x4))
        goto L_0f7c;
    else
        goto L_0f74;

L_0f74:
    if ((0x0 == 0x0))
        goto L_0e57;
    else
        goto L_0f7c;

L_0f7c:
    if ((prod.iItem != 0x5))
        goto L_0f8c;
    else
        goto L_0f84;

L_0f84:
    if ((0x0 == 0x0))
        goto L_0e57;
    else
        goto L_0f8c;

L_0f8c:
    if ((prod.iItem != 0x6))
        goto L_0f9f;
    else
        goto L_0f94;

L_0f94:
    if ((0x0 == 0x0))
        goto L_0eeb;
    else
        goto L_0f9f;

L_0f9f:
    if ((cMax >= 0))
        goto L_0fad;
    else
        goto L_0fa8;

L_0fa8:
    cMax = 0;

L_0fad:
    if ((0x0 > SIGNHIWORD(cMax)))
        goto L_1000;
    else
        goto L_0fd0;

L_0fd0:
    /* untranslated: branch ss:[bp-0x56] < signhiword(cMax) ? L_0fdc : L_0fd5 */

L_0fd5:
    /* untranslated: branch ss:[bp-0x58] > cMax ? L_1000 : L_0fdc */

L_0fdc:
    if ((prod.iItem != 0x3))
        goto L_102c;
    else
        goto L_0ff8;

L_0ff8:
    if ((0x0 != 0x0))
        goto L_102c;
    else
        goto L_1000;

L_1000:
    /* untranslated: prod.cItem = loword((int32_t)(words((cMax & 0x3ff), 0x0) << 0x0)) */
    /* untranslated: HIWORD(prod) = ((HIWORD(prod) & 0xffff) | hiword((int32_t)(words((cMax & 0x3ff), 0x0) << 0x0))) */

L_102c:
    i = 0;
    goto L_1086;

L_1034:
    /* untranslated: rgCostPaid[i] = (uint32_t)((uint32_t)(rgCost[i] * words(0x0, prod.pct)) / 0x64) */
    i = (i + 1);

L_1086:
    if ((i < 4))
        goto L_1034;
    else
        goto L_108f;

L_108f:
    if ((0x0 < 0x0))
        goto L_1712;
    else
        goto L_10a3;

L_10a3:
    if ((0x0 > 0x0))
        goto L_10b0;
    else
        goto L_10a8;

L_10a8:
    if ((prod.cItem <= 0x0))
        goto L_1712;
    else
        goto L_10b0;

L_10b0:
    i = 0;
    goto L_1101;

L_10b8:
    if (((HIWORD(rgCost[i]) - HIWORD(rgCostPaid[i])) < HIWORD(rgRes[i])))
        goto L_10fd;
    else
        goto L_10ee;

L_10ee:
    if (((HIWORD(rgCost[i]) - HIWORD(rgCostPaid[i])) > HIWORD(rgRes[i])))
        goto L_110a;
    else
        goto L_10f3;

L_10f3:
    if (((LOWORD(rgCost[i]) - LOWORD(rgCostPaid[i])) > LOWORD(rgRes[i])))
        goto L_110a;
    else
        goto L_10f7;

L_10f7:

L_10fd:
    i = (i + 1);

L_1101:
    if ((i < 4))
        goto L_10b8;
    else
        goto L_110a;

L_110a:
    if ((i >= 4))
        goto L_165c;
    else
        goto L_1113;

L_1113:
    fMineralBlocked = 0;
    fResourceBlocked = 0;
    pct = 100;
    i = 0;
    goto L_12ea;

L_1130:
    if ((HIWORD(rgCost[i]) < 0x0))
        goto L_12e6;
    else
        goto L_1145;

L_1145:
    if ((HIWORD(rgCost[i]) > 0x0))
        goto L_1152;
    else
        goto L_114a;

L_114a:
    if ((LOWORD(rgCost[i]) <= 0x0))
        goto L_12e6;
    else
        goto L_1152;

L_1152:
    if ((HIWORD(rgRes[i]) < HIWORD(rgCost[i])))
        goto L_1190;
    else
        goto L_1177;

L_1177:
    if ((HIWORD(rgRes[i]) > HIWORD(rgCost[i])))
        goto L_1183;
    else
        goto L_117c;

L_117c:
    if ((LOWORD(rgRes[i]) < LOWORD(rgCost[i])))
        goto L_1190;
    else
        goto L_1183;

L_1183:
    pctT = 100;
    goto L_1270;

L_1190:
    /* untranslated: pctT = (int32_t)((uint32_t)(words((HIWORD(rgRes[i]) + HIWORD(rgCostPaid[i])), (LOWORD(rgRes[i]) + LOWORD(rgCostPaid[i]))) * 0x64) /
     * rgCost[i]) */
    /* untranslated: pctTooBig = (int32_t)((uint32_t)(words(((HIWORD(rgRes[i]) + HIWORD(rgCostPaid[i])) + 0x0), ((LOWORD(rgRes[i]) + LOWORD(rgCostPaid[i])) +
     * 0x1)) * 0x64) / rgCost[i]) */
    if ((HIWORD(pctT) < (HIWORD(pctTooBig) + 0xffff)))
        goto L_125e;
    else
        goto L_1248;

L_1248:
    if ((HIWORD(pctT) > (HIWORD(pctTooBig) + 0xffff)))
        goto L_1255;
    else
        goto L_124d;

L_124d:
    if ((LOWORD(pctT) <= (LOWORD(pctTooBig) + 0xffff)))
        goto L_125e;
    else
        goto L_1255;

L_1255:
    t_merge_126a_0001_wide = pctT;
    goto L_126a;

L_125e:
    t_merge_126a_0001_wide = (pctTooBig - 1);

L_126a:
    pctT = t_merge_126a_0001_wide;

L_1270:
    if ((HIWORD(pctT) > HIWORD(pct)))
        goto L_12e6;
    else
        goto L_127e;

L_127e:
    if ((HIWORD(pctT) < HIWORD(pct)))
        goto L_128b;
    else
        goto L_1283;

L_1283:
    if ((LOWORD(pctT) >= LOWORD(pct)))
        goto L_12e6;
    else
        goto L_128b;

L_128b:
    lMinNeeded = ((rgCost[i] - rgCostPaid[i]) - rgRes[i]);
    pct = pctT;
    if ((i != 3))
        goto L_12e1;
    else
        goto L_12d9;

L_12d9:
    fResourceBlocked = 1;
    goto L_12e6;

L_12e1:
    fMineralBlocked = 1;

L_12e6:
    i = (i + 1);

L_12ea:
    if ((i < 4))
        goto L_1130;
    else
        goto L_12f3;

L_12f3:
    if ((fMineralBlocked == 0))
        goto L_131c;
    else
        goto L_12fc;

L_12fc:
    if ((fAutoBuild == 0))
        goto L_131c;
    else
        goto L_1305;

L_1305:
    if ((fAlchemy != 0))
        goto LAlchemize;
    else
        goto L_130b;

L_130b:

L_1314:
    fAutoBuild = 2;
    goto L_1712;

L_131c:
    i = 0;
    goto L_1398;

L_1324:
    AddCost = ((int32_t)(((uint32_t)((rgCost[i] * pct)) / 0x64)) - rgCostPaid[i]);
    rgRes[i] = (rgRes[i] - AddCost);
    rgCostPaid[i] = (rgCostPaid[i] + AddCost);
    i = (i + 1);

L_1398:
    if ((i < 4))
        goto L_1324;
    else
        goto L_13a1;

L_13a1:
    /* untranslated: LOWORD(prod) = ((LOWORD(prod) & 0xffff) | loword((int32_t)(words((LOWORD(pct) & 0x7f), 0x0) << 0x14))) */
    /* untranslated: HIWORD(prod) = ((HIWORD(prod) & 0xf80f) | hiword((int32_t)(words((LOWORD(pct) & 0x7f), 0x0) << 0x14))) */
    if ((fAlchemy == 0))
        goto L_1712;
    else
        goto L_13d5;

L_13d5:
    if ((fResourceBlocked != 0))
        goto L_1712;
    else
        goto L_13db;

L_13db:

LAlchemize:
    if ((GetRaceGrbit(rgplr[lppl->iPlayer], ibitRaceMineralAlchemy) == 0))
        goto L_140c;
    else
        goto L_1405;

L_1405:
    t_merge_1410_0001 = 0x19;
    goto L_1410;

L_140c:
    t_merge_1410_0001 = 0x64;

L_1410:
    LOWORD(lAlchCost) = t_merge_1410_0001;
    HIWORD(lAlchCost) = 0x0;
    /* untranslated: cCanBuild = (int32_t)(words(*(rgRes+0xe), LOWORD(rgRes[0x3])) / lAlchCost) */
    if ((HIWORD(cCanBuild) < HIWORD(lMinNeeded)))
        goto L_1457;
    else
        goto L_143e;

L_143e:
    if ((HIWORD(cCanBuild) > HIWORD(lMinNeeded)))
        goto L_144b;
    else
        goto L_1443;

L_1443:
    if ((LOWORD(cCanBuild) <= LOWORD(lMinNeeded)))
        goto L_1457;
    else
        goto L_144b;

L_144b:
    cCanBuild = lMinNeeded;

L_1457:
    if ((HIWORD(cCanBuild) < 0x0))
        goto L_14c5;
    else
        goto L_1460;

L_1460:
    if ((HIWORD(cCanBuild) > 0x0))
        goto L_146e;
    else
        goto L_1465;

L_1465:
    if ((LOWORD(cCanBuild) <= 0x0))
        goto L_14c5;
    else
        goto L_146e;

L_146e:
    i = 0;
    goto L_1491;

L_1476:
    rgRes[i] = (rgRes[i] + cCanBuild);
    i = (i + 1);

L_1491:
    if ((i < 3))
        goto L_1476;
    else
        goto L_149a;

L_149a:
    rgRes[i] = (rgRes[i] - (uint32_t)((lAlchCost * cCanBuild)));
    cAlchemy = (cAlchemy + LOWORD(cCanBuild));

L_14c5:
    if ((LOWORD(cCanBuild) != LOWORD(lMinNeeded)))
        goto L_14de;
    else
        goto L_14d3;

L_14d3:
    if ((HIWORD(cCanBuild) == HIWORD(lMinNeeded)))
        goto L_108f;
    else
        goto L_14d8;

L_14d8:

L_14de:
    if ((*(rgRes + 0xe) < 0x0))
        goto L_1712;
    else
        goto L_14ea;

L_14ea:
    if ((*(rgRes + 0xe) > 0x0))
        goto L_14f8;
    else
        goto L_14ef;

L_14ef:
    if ((LOWORD(rgRes[0x3]) <= 0x0))
        goto L_1712;
    else
        goto L_14f8;

L_14f8:
    if ((pprodPartial == 0x0))
        goto L_1712;
    else
        goto L_1501;

L_1501:
    memset(pprodPartial, 0x0, 0x4);
    pprodPartial = ((pprodPartial & 0xffff) | 0x0);
    pprodPartial->grobj = grobjPlanet;
    pprodPartial = ((pprodPartial & 0x3ff) | 0x2c00);
    *(pprodPartial + 0x2) = ((*(pprodPartial + 0x2) & 0xfffe) | 0x0);
    pprodPartial->cItem = 0x1;
    *(pprodPartial + 0x2) = ((*(pprodPartial + 0x2) & 0xffff) | 0x0);
    /* untranslated: pctT = (int32_t)((uint32_t)(words(*(rgRes+0xe), LOWORD(rgRes[0x3])) * 0x64) / lAlchCost) */
    /* untranslated: pctTooBig = (int32_t)((uint32_t)(words((*(rgRes+0xe) + 0x0), (LOWORD(rgRes[0x3]) + 0x1)) * 0x64) / lAlchCost) */
    if ((HIWORD(pctT) < (HIWORD(pctTooBig) + 0xffff)))
        goto L_15ec;
    else
        goto L_15d6;

L_15d6:
    if ((HIWORD(pctT) > (HIWORD(pctTooBig) + 0xffff)))
        goto L_15e3;
    else
        goto L_15db;

L_15db:
    if ((LOWORD(pctT) <= (LOWORD(pctTooBig) + 0xffff)))
        goto L_15ec;
    else
        goto L_15e3;

L_15e3:
    t_merge_15f8_0001_wide = pctT;
    goto L_15f8;

L_15ec:
    t_merge_15f8_0001_wide = (pctTooBig - 1);

L_15f8:
    pctT = t_merge_15f8_0001_wide;
    /* untranslated: pprodPartial = ((pprodPartial & 0xffff) | loword((int32_t)(words((LOWORD(pctT) & 0x7f), 0x0) << 0x14))) */
    /* untranslated: *(pprodPartial+0x2) = ((*(pprodPartial+0x2) & 0xf80f) | hiword((int32_t)(words((LOWORD(pctT) & 0x7f), 0x0) << 0x14))) */
    LOWORD(rgRes[0x3]) = (LOWORD(rgRes[0x3]) - LOWORD((int32_t)(((uint32_t)((pctT * lAlchCost)) / 0x64))));
    *(rgRes + 0xe) = (*(rgRes + 0xe) - HIWORD((int32_t)(((uint32_t)((pctT * lAlchCost)) / 0x64))));

L_165c:
    cBuilt = (cBuilt + 1);
    /* untranslated: ss:[bp-0x58] = ((LOWORD(prod) + 0xffff) & 0x3ff) */
    /* untranslated: ss:[bp-0x56] = 0x0 */
    prod.cItem = 0x0;
    HIWORD(prod) = (HIWORD(prod) & 0xffff);
    /* untranslated: LOWORD(prod) = (LOWORD(prod) | ss:[bp-0x58]) */
    /* untranslated: HIWORD(prod) = (HIWORD(prod) | ss:[bp-0x56]) */
    LOWORD(prod) = ((LOWORD(prod) & 0xffff) | 0x0);
    prod.pct = 0x0;
    i = 0;
    goto L_1706;

L_16ba:
    rgRes[i] = (rgRes[i] - (rgCost[i] - rgCostPaid[i]));
    rgCostPaid[i] = 0;
    i = (i + 1);

L_1706:
    if ((i >= 4))
        goto L_108f;
    else
        goto L_170c;

L_170c:

L_1712:
    if ((cBuilt <= 0))
        goto L_17b7;
    else
        goto L_171b;

L_171b:
    if ((prod.grobj != 0x1))
        goto L_17b7;
    else
        goto L_1737;

L_1737:
    if ((0x0 != 0x0))
        goto L_17b7;
    else
        goto L_173f;

L_173f:
    if ((prod.iItem != 0xb))
        goto L_1763;
    else
        goto L_175b;

L_175b:
    if ((0x0 == 0x0))
        goto L_1787;
    else
        goto L_1763;

L_1763:
    if ((prod.iItem != 0x3))
        goto L_17b7;
    else
        goto L_177f;

L_177f:
    if ((0x0 != 0x0))
        goto L_17b7;
    else
        goto L_1787;

L_1787:
    cAlchemy = (cAlchemy + cBuilt);
    i = 0;
    goto L_17ae;

L_1795:
    rgRes[i] = (rgRes[i] + (uint32_t)(cBuilt));
    i = (i + 1);

L_17ae:
    if ((i < 3))
        goto L_1795;
    else
        goto L_17b7;

L_17b7:
    if ((cAlchemy == 0))
        goto L_17fb;
    else
        goto L_17c0;

L_17c0:
    if ((fCalcOnly != 0))
        goto L_17fb;
    else
        goto L_17c9;

L_17c9:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_17fb;
    else
        goto L_17d9;

L_17d9:
    FSendPlrMsg2(lppl->iPlayer, 0x8c, lppl->id, lppl->id, cAlchemy);

L_17fb:
    if ((pmdStatus == 0x0))
        goto L_18d1;
    else
        goto L_1804;

L_1804:
    if ((fAutoBuild != 2))
        goto L_1827;
    else
        goto L_180d;

L_180d:
    if ((cBuilt <= 0))
        goto L_181c;
    else
        goto L_1816;

L_1816:
    t_merge_181f_0001 = 0x3;
    goto L_181f;

L_181c:
    t_merge_181f_0001 = 0x4;

L_181f:
    *(pmdStatus) = t_merge_181f_0001;
    goto L_18d1;

L_1827:
    if ((fAutoBuild == 0))
        goto L_1866;
    else
        goto L_1830;

L_1830:
    if ((prod.cItem != 0x0))
        goto L_1866;
    else
        goto L_1844;

L_1844:
    if ((0x0 != 0x0))
        goto L_1866;
    else
        goto L_184c;

L_184c:
    if ((cBuilt <= 0))
        goto L_185b;
    else
        goto L_1855;

L_1855:
    t_merge_185e_0001 = 0x1;
    goto L_185e;

L_185b:
    t_merge_185e_0001 = 0x2;

L_185e:
    *(pmdStatus) = t_merge_185e_0001;
    goto L_18d1;

L_1866:
    if ((cBuilt != 0))
        goto L_18a4;
    else
        goto L_186f;

L_186f:
    if ((LOWORD(pctInitial) != prod.pct))
        goto L_1899;
    else
        goto L_188b;

L_188b:
    if ((HIWORD(pctInitial) != 0x0))
        goto L_1899;
    else
        goto L_1893;

L_1893:
    t_merge_189c_0001 = 0x7;
    goto L_189c;

L_1899:
    t_merge_189c_0001 = 0x6;

L_189c:
    *(pmdStatus) = t_merge_189c_0001;
    goto L_18d1;

L_18a4:
    if ((prod.cItem != 0x0))
        goto L_18ca;
    else
        goto L_18b8;

L_18b8:
    if ((0x0 != 0x0))
        goto L_18ca;
    else
        goto L_18c0;

L_18c0:
    *(pmdStatus) = 0;
    goto L_18d1;

L_18ca:
    *(pmdStatus) = 5;

L_18d1:
    if ((fCalcOnly != 0))
        goto L_18f3;
    else
        goto L_18da;

L_18da:
    if ((fAutoBuild != 0))
        goto L_18f3;
    else
        goto L_18e3;

L_18e3:
    LOWORD(lpprod) = LOWORD(prod);
    *(lpprod + 0x2) = HIWORD(prod);

L_18f3:
    if ((fAutoBuild == 0))
        goto L_19a5;
    else
        goto L_18fc;

L_18fc:
    if ((pprodPartial == 0x0))
        goto L_19a5;
    else
        goto L_1905;

L_1905:
    if ((pprodPartial->cItem != 0x0))
        goto L_19a5;
    else
        goto L_191b;

L_191b:
    if ((0x0 != 0x0))
        goto L_19a5;
    else
        goto L_1923;

L_1923:
    if ((0x0 < 0x0))
        goto L_19a5;
    else
        goto L_193f;

L_193f:
    if ((0x0 > 0x0))
        goto L_194c;
    else
        goto L_1944;

L_1944:
    if ((prod.pct <= 0x0))
        goto L_19a5;
    else
        goto L_194c;

L_194c:
    pprodPartial = LOWORD(prod);
    *(pprodPartial + 0x2) = HIWORD(prod);
    pprodPartial->cItem = 0x1;
    *(pprodPartial + 0x2) = ((*(pprodPartial + 0x2) & 0xffff) | 0x0);
    /* untranslated: pprodPartial = (pprodPartial->cItem | loword((int32_t)(words((LOWORD(iobjOther) & 0x7f), 0x0) << 0xa))) */
    /* untranslated: *(pprodPartial+0x2) = ((*(pprodPartial+0x2) & 0xfffe) | hiword((int32_t)(words((LOWORD(iobjOther) & 0x7f), 0x0) << 0xa))) */

L_19a5:

L_19ab:
    return cBuilt;
}

int16_t FBuildObject(PLANET *lppl, GrobjClass grobj, int16_t iItem, int16_t cBuilt, int32_t *rgMinerals) {
    int16_t   iWarp;
    int16_t   i;
    FLEET    *lpfl;
    MessageId idm;
    int16_t   fTwoMAs;
    SHDEF    *lpshdef;
    int16_t   cAllowed;
    int32_t   dpOrig;
    int16_t   cshDamaged;
    int16_t   cshOrig;
    uint16_t  dpShdef;
    THING    *lpthMac;
    int16_t   iDecayRate;
    THING    *lpth;
    int16_t   raMajor;
    int16_t   iWarpAsked;
    int16_t   cSize;
    int16_t   rgwt[3];
    int32_t   l;
    int16_t   iEnv;
    PART      part;
    uint16_t  t_merge_2245_0001;
    uint16_t  t_merge_22c7_0001;
    int16_t   t_merge_2429_0001;
    int16_t   t_merge_2552_0001;
    int16_t   t_merge_2609_0001;
    uint16_t  t_merge_274b_0001;
    uint16_t  t_merge_2763_0001;

L_19b2:
    if ((grobj != grobjFleet))
        goto L_23c7;
    else
        goto L_19c4;

L_19c4:
    if ((iItem < 16))
        goto L_1c78;
    else
        goto L_19cd;

L_19cd:
    iItem = (iItem - 16);
    lpshdef = &(rglpshdefSB[lppl->iPlayer][iItem]);
    if ((lpshdef->fFree != 0x0))
        goto L_1a26;
    else
        goto L_1a09;

L_1a09:
    if ((FCanBuildShdef(lpshdef, lppl->iPlayer) != 0))
        goto L_1a2c;
    else
        goto L_1a26;

L_1a26:
    return 0x0;

L_1a2c:
    idm = idmHasBuiltNew;
    if ((lpshdef->hul.wtCargoMax == 0x0))
        goto L_1a60;
    else
        goto L_1a3e;

L_1a3e:
    idm = (idm + 1);
    if ((lpshdef->hul.wtCargoMax != 0xffff))
        goto L_1a60;
    else
        goto L_1a54;

L_1a54:
    if ((0x0 != 0x0))
        goto L_1a60;
    else
        goto L_1a5c;

L_1a5c:
    idm = (idm + 1);

L_1a60:
    FSendPlrMsg(lppl->iPlayer, idm, lppl->id, lppl->id, ((lppl->iPlayer << 0x5) | (iItem + 16)), LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax, 0x0, 0x0,
                0x0, 0x0);
    if ((lppl->fStarbase == 0x0))
        goto L_1b3a;
    else
        goto L_1ad6;

L_1ad6:
    if ((rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef <= rglpshdefSB[lppl->iPlayer][iItem].hul.ihuldef))
        goto L_1b3a;
    else
        goto L_1b2c;

L_1b2c:
    KillQueuedShips(lppl);

L_1b3a:
    iWarp = IWarpMAFromLppl(lppl, &(fTwoMAs));
    if ((lppl->fStarbase == 0x0))
        goto L_1b9b;
    else
        goto L_1b66;

L_1b66:
    rglpshdefSB[lppl->iPlayer][lppl->isb].cExist = (rglpshdefSB[lppl->iPlayer][lppl->isb].cExist - 0x1);
    goto L_1baf;

L_1b9b:
    lppl->fStarbase = 0x1;

L_1baf:
    *(lppl + 0x2c) = ((*(lppl + 0x2c) & 0xfff0) | (iItem & 0xf));
    if ((iWarp > 0))
        goto L_1c55;
    else
        goto L_1bd1;

L_1bd1:
    iWarp = IWarpMAFromLppl(lppl, &(fTwoMAs));
    if ((iWarp <= 0))
        goto L_1c1f;
    else
        goto L_1bef;

L_1bef:
    *(lppl + 0x2e) = ((*(lppl + 0x2e) & 0xc3ff) | ((((iWarp + fTwoMAs) + 0xfffc) & 0xf) << 0xa));
    goto L_1c55;

L_1c1f:
    lppl->iWarpFling = 0x0;
    lppl->idFling = 0x0;
    KillQueuedMassPackets(lppl);

L_1c55:
    lpshdef->cBuilt = (lpshdef->cBuilt + 0x1);
    lpshdef->cExist = (lpshdef->cExist + 0x1);
    return 0x1;

L_1c78:
    if ((lppl->fStarbase == 0x0))
        goto L_1c98;
    else
        goto L_1c8f;

L_1c8f:
    if ((iItem < 16))
        goto L_1c9e;
    else
        goto L_1c98;

L_1c98:
    return 0x0;

L_1c9e:
    lpshdef = &(rglpshdef[lppl->iPlayer][iItem]);
    if ((lpshdef->fFree != 0x0))
        goto L_1cf3;
    else
        goto L_1cd6;

L_1cd6:
    if ((FCanBuildShdef(lpshdef, lppl->iPlayer) != 0))
        goto L_1d1d;
    else
        goto L_1cf3;

L_1cf3:
    FSendPlrMsg2(lppl->iPlayer, 0x4f, lppl->id, (iItem + 1), 0x0);
    return 0x0;

L_1d1d:
    if ((rgplr[lppl->iPlayer].cFleet != 0x200))
        goto L_219c;
    else
        goto L_1d3a;

L_1d3a:
    i = 0;
    goto L_1d46;

L_1d42:
    i = (i + 1);

L_1d46:
    if ((i >= cFleet))
        goto L_214f;
    else
        goto L_1d51;

L_1d51:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_1d81;
    else
        goto L_1d79;

L_1d79:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_214f;
    else
        goto L_1d81;

L_1d81:
    if ((lpfl->iPlayer > lppl->iPlayer))
        goto L_214f;
    else
        goto L_1d91;

L_1d91:

L_1d97:
    if ((lpfl->iPlayer < lppl->iPlayer))
        goto L_1d42;
    else
        goto L_1da7;

L_1da7:

L_1dad:
    if ((lpfl->lpplord->rgord[0].pt.x != rgptPlan[lppl->id].x))
        goto L_1d42;
    else
        goto L_1dcb;

L_1dcb:
    if ((lpfl->lpplord->rgord[0].pt.y != rgptPlan[lppl->id].y))
        goto L_1d42;
    else
        goto L_1ded;

L_1ded:
    if (((32766 - cBuilt) <= lpfl->rgcsh[iItem]))
        goto L_1d42;
    else
        goto L_1e0f;

L_1e0f:
    if ((lpfl->rgcsh[iItem] == 0))
        goto L_20c5;
    else
        goto L_1e2c;

L_1e2c:
    /* untranslated: branch ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 2))] >> 0x7) & 0x1ff) == 0x0 ? L_20c5 : L_1e53 */

L_1e53:
    dpShdef = rglpshdef[lpfl->iPlayer][iItem].hul.dp;
    cshOrig = lpfl->rgcsh[iItem];
    /* untranslated: cshDamaged = loword((int32_t)(words(hiword((uint32_t)(words(0x0, (HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] & 0x7f)) *
     * sext16to32(cshOrig))), loword((uint32_t)(words(0x0, (HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] & 0x7f)) * sext16to32(cshOrig)))) / 0x64)) */
    if ((cshDamaged != 0))
        goto L_1edb;
    else
        goto L_1ed6;

L_1ed6:
    cshDamaged = 1;

L_1edb:
    /* untranslated: LOWORD(dpOrig) = loword((int32_t)(words(hiword((uint32_t)(words(hiword((int32_t)(words(hiword((uint32_t)(words(0x0, dpShdef) * words(0x0,
     * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >> 0x7) & 0x1ff)))), loword((uint32_t)(words(0x0, dpShdef) * words(0x0,
     * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >> 0x7) & 0x1ff))))) / 0xa)), loword((int32_t)(words(hiword((uint32_t)(words(0x0, dpShdef) *
     * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >> 0x7) & 0x1ff)))), loword((uint32_t)(words(0x0, dpShdef) * words(0x0,
     * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >> 0x7) & 0x1ff))))) / 0xa))) * sext16to32(cshDamaged))),
     * loword((uint32_t)(words(hiword((int32_t)(words(hiword((uint32_t)(words(0x0, dpShdef) * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem *
     * 0x2))] >> 0x7) & 0x1ff)))), loword((uint32_t)(words(0x0, dpShdef) * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >> 0x7) &
     * 0x1ff))))) / 0xa)), loword((int32_t)(words(hiword((uint32_t)(words(0x0, dpShdef) * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >>
     * 0x7) & 0x1ff)))), loword((uint32_t)(words(0x0, dpShdef) * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >> 0x7) & 0x1ff))))) /
     * 0xa))) * sext16to32(cshDamaged)))) / 0x32)) */
    /* untranslated: HIWORD(dpOrig) = hiword((int32_t)(words(hiword((uint32_t)(words(hiword((int32_t)(words(hiword((uint32_t)(words(0x0, dpShdef) * words(0x0,
     * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >> 0x7) & 0x1ff)))), loword((uint32_t)(words(0x0, dpShdef) * words(0x0,
     * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >> 0x7) & 0x1ff))))) / 0xa)), loword((int32_t)(words(hiword((uint32_t)(words(0x0, dpShdef) *
     * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >> 0x7) & 0x1ff)))), loword((uint32_t)(words(0x0, dpShdef) * words(0x0,
     * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >> 0x7) & 0x1ff))))) / 0xa))) * sext16to32(cshDamaged))),
     * loword((uint32_t)(words(hiword((int32_t)(words(hiword((uint32_t)(words(0x0, dpShdef) * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem *
     * 0x2))] >> 0x7) & 0x1ff)))), loword((uint32_t)(words(0x0, dpShdef) * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >> 0x7) &
     * 0x1ff))))) / 0xa)), loword((int32_t)(words(hiword((uint32_t)(words(0x0, dpShdef) * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >>
     * 0x7) & 0x1ff)))), loword((uint32_t)(words(0x0, dpShdef) * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] >> 0x7) & 0x1ff))))) /
     * 0xa))) * sext16to32(cshDamaged)))) / 0x32)) */
    /* untranslated: HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 2))] = ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] & 0xff80) |
     * (loword((int32_t)((uint32_t)(sext16to32(cshDamaged) * 0x64) / sext16to32((cshOrig + cBuilt)))) & 0x7f)) */
    /* untranslated: branch (HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 2))] & 0x7f) != 0x0 ? L_1ff4 : L_1fc0 */

L_1fc0:
    /* untranslated: HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 2))] = ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] & 0xff80) | 0x1) */

L_1ff4:
    /* untranslated: cshDamaged = loword((int32_t)(words(hiword((uint32_t)(words(0x0, (HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] & 0x7f)) *
     * sext16to32((cshOrig + cBuilt)))), loword((uint32_t)(words(0x0, (HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] & 0x7f)) * sext16to32((cshOrig +
     * cBuilt))))) / 0x64)) */
    if ((cshDamaged != 0))
        goto L_2041;
    else
        goto L_203c;

L_203c:
    cshDamaged = 1;

L_2041:
    /* untranslated: HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 2))] = ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 0x2))] & 0x7f) |
     * ((loword((int32_t)((uint32_t)((int32_t)((uint32_t)(dpOrig * 0x5) / sext16to32(cshDamaged)) * 0x64) / words(0x0, dpShdef))) & 0x1ff) << 0x7)) */
    goto L_20de;

L_20c5:
    /* untranslated: HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (iItem * 2))] = 0x0 */

L_20de:
    CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
    FSendPlrMsg(lppl->iPlayer, 0x139, (lpfl->id | 0x8000), lppl->id, cBuilt, ((lppl->iPlayer << 0x5) | iItem), lpfl->id, 0x0, 0x0, 0x0);
    return 0x1;

L_214f:
    FSendPlrMsg(lppl->iPlayer, 0xba, lppl->id, lppl->id, cBuilt, ((lppl->iPlayer << 0x5) | iItem), 0x0, 0x0, 0x0, 0x0);
    return 0x0;

L_219c:
    lpfl = LpflNew(lppl->iPlayer, lppl->id);
    CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
    lpfl->rgwtMin[4] = LGetFleetStat(lpfl, 0x1);
    if ((lppl->idRoute == 0x0))
        goto L_2321;
    else
        goto L_2201;

L_2201:
    AutoRouteFleet(lpfl, lppl);
    if ((cBuilt != 1))
        goto L_22a0;
    else
        goto L_221e;

L_221e:
    if ((lpfl->lpplord->rgord[1].iWarp != 0x0))
        goto L_2242;
    else
        goto L_223c;

L_223c:
    t_merge_2245_0001 = 0x33;
    goto L_2245;

L_2242:
    t_merge_2245_0001 = 0x31;

L_2245:
    idm = t_merge_2245_0001;
    FSendPlrMsg(lppl->iPlayer, idm, (lpfl->id | 0x8000), lppl->id, ((lppl->iPlayer << 0x5) | iItem), (lppl->idRoute + 0xffff), 0x0, 0x0, 0x0, 0x0);
    goto L_2fc9;

L_22a0:
    if ((lpfl->lpplord->rgord[1].iWarp != 0x0))
        goto L_22c4;
    else
        goto L_22be;

L_22be:
    t_merge_22c7_0001 = 0x34;
    goto L_22c7;

L_22c4:
    t_merge_22c7_0001 = 0x32;

L_22c7:
    idm = t_merge_22c7_0001;
    FSendPlrMsg(lppl->iPlayer, idm, (lpfl->id | 0x8000), lppl->id, cBuilt, ((lppl->iPlayer << 0x5) | iItem), (lppl->idRoute + 0xffff), 0x0, 0x0, 0x0);

L_2321:
    AutoFleetOrder(lpfl, lppl);
    if ((cBuilt != 1))
        goto L_2379;
    else
        goto L_233e;

L_233e:
    FSendPlrMsg2(lppl->iPlayer, 0x2f, (lpfl->id | 0x8000), lppl->id, ((lppl->iPlayer << 0x5) | iItem));
    goto L_2fc9;

L_2379:
    FSendPlrMsg(lppl->iPlayer, 0x30, (lpfl->id | 0x8000), lppl->id, cBuilt, ((lppl->iPlayer << 0x5) | iItem), 0x0, 0x0, 0x0, 0x0);

L_23c7:
    if ((grobj != grobjPlanet))
        goto L_2fc3;
    else
        goto L_23d0;

L_23d0:
    goto L_2f77;

L_23d6:
    goto L_2fcf;
    /* untranslated: ss:[bp-0x16] = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff) */
    /* untranslated: cAllowed = (CMaxFactories(lppl, lppl->iPlayer) - ss:[bp-0x16]) */
    if ((cBuilt >= cAllowed))
        goto L_2426;
    else
        goto L_2420;

L_2420:
    t_merge_2429_0001 = cBuilt;
    goto L_2429;

L_2426:
    t_merge_2429_0001 = cAllowed;

L_2429:
    cBuilt = t_merge_2429_0001;
    if ((cBuilt <= 0))
        goto L_24fc;
    else
        goto L_2435;

L_2435:
    /* untranslated: ss:[bp-0x18] = 0x0 */
    /* untranslated: ss:[bp-0x16] = ((hiword((int32_t)(sext16to32(cBuilt) << 0x14)) + *(lppl+0x16)) & 0xfff0) */
    *(lppl + 0x14) = (*(lppl + 0x14) & 0xffff);
    lppl->cFactories = 0x0;
    /* untranslated: *(lppl+0x14) = (*(lppl+0x14) | ss:[bp-0x18]) */
    /* untranslated: *(lppl+0x16) = (*(lppl+0x16) | ss:[bp-0x16]) */
    idm = idmHaveBuiltFactory;

SendMsgFactMine:
    cBuilt = (cBuilt + FRemovePlayerMessage(lppl->iPlayer, idm, lppl->id));
    if ((cBuilt <= 1))
        goto L_24d7;
    else
        goto L_24af;

L_24af:
    FSendPlrMsg2(lppl->iPlayer, (idm + 1), lppl->id, cBuilt, lppl->id);
    goto L_2502;

L_24d7:
    FSendPlrMsg2(lppl->iPlayer, idm, lppl->id, lppl->id, 0x0);

L_24fc:
    return 0x0;

L_2502:
    goto L_2fc0;
    /* untranslated: ss:[bp-0x16] = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff) */
    /* untranslated: cAllowed = (CMaxMines(lppl, lppl->iPlayer) - ss:[bp-0x16]) */
    if ((cBuilt >= cAllowed))
        goto L_254f;
    else
        goto L_2549;

L_2549:
    t_merge_2552_0001 = cBuilt;
    goto L_2552;

L_254f:
    t_merge_2552_0001 = cAllowed;

L_2552:
    cBuilt = t_merge_2552_0001;
    if ((cBuilt <= 0))
        goto L_25bb;
    else
        goto L_255e;

L_255e:
    /* untranslated: ss:[bp-0x18] = ((loword((int32_t)(sext16to32(cBuilt) << 0x8)) + *(lppl+0x14)) & 0xff00) */
    /* untranslated: ss:[bp-0x16] = ((hiword((int32_t)(sext16to32(cBuilt) << 0x8)) + *(lppl+0x16)) & 0xf) */
    lppl->cMines = 0x0;
    /* untranslated: *(lppl+0x14) = (*(lppl+0x14) | ss:[bp-0x18]) */
    /* untranslated: *(lppl+0x16) = (*(lppl+0x16) | ss:[bp-0x16]) */
    idm = idmHaveBuiltMine;
    goto SendMsgFactMine;

L_25bb:
    goto L_2fcf;
    /* untranslated: ss:[bp-0x16] = lppl->cDefenses */
    /* untranslated: cAllowed = (CMaxDefenses(lppl, lppl->iPlayer) - ss:[bp-0x16]) */
    if ((cBuilt >= cAllowed))
        goto L_2606;
    else
        goto L_2600;

L_2600:
    t_merge_2609_0001 = cBuilt;
    goto L_2609;

L_2606:
    t_merge_2609_0001 = cAllowed;

L_2609:
    cBuilt = t_merge_2609_0001;
    if ((cBuilt <= 0))
        goto L_2672;
    else
        goto L_2615;

L_2615:
    /* untranslated: ss:[bp-0x18] = ((loword((int32_t)(sext16to32(cBuilt) << 0x0)) + *(lppl+0x18)) & 0xfff) */
    /* untranslated: ss:[bp-0x16] = 0x0 */
    lppl->cDefenses = 0x0;
    *(lppl + 0x1a) = (*(lppl + 0x1a) & 0xffff);
    /* untranslated: *(lppl+0x18) = (*(lppl+0x18) | ss:[bp-0x18]) */
    /* untranslated: *(lppl+0x1a) = (*(lppl+0x1a) | ss:[bp-0x16]) */
    idm = idmHaveBuiltDefenseOutpost;
    goto SendMsgFactMine;

L_2672:
    goto L_2fcf;
    goto L_2fc0;
    goto L_2fc0;
    raMajor = GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv);
    iWarp = IWarpMAFromLppl(lppl, &(fTwoMAs));
    if ((iWarp != 0))
        goto L_26e7;
    else
        goto L_26be;

L_26be:
    FSendPlrMsg2(lppl->iPlayer, 0xd1, lppl->id, lppl->id, 0x0);
    return 0x0;

L_26e7:
    if ((lppl->idFling != 0x0))
        goto L_2722;
    else
        goto L_26f9;

L_26f9:
    FSendPlrMsg2(lppl->iPlayer, 0xd2, lppl->id, lppl->id, 0x0);
    return 0x0;

L_2722:
    if ((iItem != 6))
        goto L_2730;
    else
        goto L_272b;

L_272b:
    iItem = 17;

L_2730:
    if ((iItem != 17))
        goto L_2751;
    else
        goto L_2739;

L_2739:
    if ((raMajor != 6))
        goto L_2748;
    else
        goto L_2742;

L_2742:
    t_merge_274b_0001 = 0x19;
    goto L_274b;

L_2748:
    t_merge_274b_0001 = 0x28;

L_274b:
    cSize = t_merge_274b_0001;
    goto L_2766;

L_2751:
    if ((raMajor != 6))
        goto L_2760;
    else
        goto L_275a;

L_275a:
    t_merge_2763_0001 = 0x46;
    goto L_2763;

L_2760:
    t_merge_2763_0001 = 0x64;

L_2763:
    cSize = t_merge_2763_0001;

L_2766:
    i = 0;
    goto L_27e5;

L_276e:
    if ((i == (iItem - 14)))
        goto L_2785;
    else
        goto L_277c;

L_277c:
    if ((iItem != 17))
        goto L_27d3;
    else
        goto L_2785;

L_2785:
    l = (uint32_t)(((uint32_t)(cSize) * (uint32_t)(cBuilt)));
    if ((HIWORD(l) < 0x0))
        goto L_27be;
    else
        goto L_27a5;

L_27a5:
    if ((HIWORD(l) > 0x0))
        goto L_27b4;
    else
        goto L_27aa;

L_27aa:
    if ((LOWORD(l) <= 0x7ff8))
        goto L_27be;
    else
        goto L_27b4;

L_27b4:
    l = 32760;

L_27be:
    rgwt[i] = LOWORD(l);
    goto L_27e1;

L_27d3:
    rgwt[i] = 0;

L_27e1:
    i = (i + 1);

L_27e5:
    if ((i < 3))
        goto L_276e;
    else
        goto L_27ee;

L_27ee:
    iWarpAsked = (lppl->iWarpFling + 4);
    if ((iWarpAsked < 5))
        goto L_281a;
    else
        goto L_280c;

L_280c:
    if ((iWarpAsked <= (iWarp + 3)))
        goto L_2823;
    else
        goto L_281a;

L_281a:
    iWarpAsked = (iWarp + fTwoMAs);

L_2823:
    if ((iWarpAsked > (iWarp + fTwoMAs)))
        goto L_2839;
    else
        goto L_2831;

L_2831:
    iDecayRate = 0;
    goto L_2845;

L_2839:
    iDecayRate = ((iWarpAsked - iWarp) - fTwoMAs);

L_2845:
    if ((raMajor != 7))
        goto L_285b;
    else
        goto L_284e;

L_284e:
    if ((iDecayRate >= 3))
        goto L_285b;
    else
        goto L_2857;

L_2857:
    iDecayRate = (iDecayRate + 1);

L_285b:
    iWarp = (iWarpAsked - 4);
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_295a;

L_288f:
    if ((lpth->iplr != lppl->iPlayer))
        goto L_2956;
    else
        goto L_28ab;

L_28ab:
    if ((lpth->ith != ithMineralPacket))
        goto L_2956;
    else
        goto L_28c1;

L_28c1:
    if ((lpth->pt.x != rgptPlan[lppl->id].x))
        goto L_2956;
    else
        goto L_28e7;

L_28e7:
    if ((lpth->pt.y != rgptPlan[lppl->id].y))
        goto L_2956;
    else
        goto L_28f0;

L_28f0:
    if ((((*(lpth + 0x6) >> 0xa) & 0xf) != iWarp))
        goto L_2956;
    else
        goto L_2909;

L_2909:
    if (((*(lpth + 0x6) & 0x3ff) != (lppl->idFling + 0xffff)))
        goto L_2956;
    else
        goto L_2928;

L_2928:
    if ((lpth->thp.iDecayRate != iDecayRate))
        goto L_2956;
    else
        goto L_2941;

L_2941:
    if ((lpth->thp.wtMax < 0x65e))
        goto L_2968;
    else
        goto L_2950;

L_2950:

L_2956:
    lpth = (lpth + 0x1);

L_295a:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_288f;
    else
        goto L_2968;

L_2968:
    if ((LOWORD(lpth) != LOWORD(lpthMac)))
        goto L_297e;
    else
        goto L_2976;

L_2976:
    if ((HIWORD(lpth) == HIWORD(lpthMac)))
        goto L_2a78;
    else
        goto L_297e;

L_297e:
    lpth->thp.wtMax = 0x0;
    i = 0;
    goto L_2a3f;

L_299a:
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))] = (HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] + rgwt[i]) */
    /* untranslated: branch HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))] >= 0x0 ? L_29f3 : L_29da */

L_29da:
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))] = 0x7ff8 */

L_29f3:
    /* untranslated: ss:[bp-0x2e] = (((words((HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] + 0x9), signhiword((HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i *
     * 0x2))] + 0x9))) / 0xa) + *(lpth+0xe)) & 0x3fff) */
    lpth->thp.wtMax = 0x0;
    /* untranslated: *(lpth+0xe) = (*(lpth+0xe) | ss:[bp-0x2e]) */
    i = (i + 1);

L_2a3f:
    if ((i < 3))
        goto L_299a;
    else
        goto L_2a48;

L_2a48:
    FSendPlrMsg2(lppl->iPlayer, 0xd4, lppl->id, lppl->id, (lppl->idFling + 0xffff));
    goto L_2fc9;

L_2a78:
    lpth = LpthNew(lppl->iPlayer, ithMineralPacket);
    if ((LOWORD(lpth) != 0x0))
        goto L_2ac9;
    else
        goto L_2a9a;

L_2a9a:
    if ((HIWORD(lpth) != 0x0))
        goto L_2ac9;
    else
        goto L_2aa3;

L_2aa3:
    FSendPlrMsg2(lppl->iPlayer, 0x129, lppl->id, lppl->id, 0x0);
    goto L_2fc9;

L_2ac9:
    i = 0;
    goto L_2b35;

L_2ad1:
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))] = rgwt[i] */
    /* untranslated: ss:[bp-0x2e] = (((sext16to32((rgwt[i] + 0x9)) / 0xa) + *(lpth+0xe)) & 0x3fff) */
    lpth->thp.wtMax = 0x0;
    /* untranslated: *(lpth+0xe) = (*(lpth+0xe) | ss:[bp-0x2e]) */
    i = (i + 1);

L_2b35:
    if ((i < 3))
        goto L_2ad1;
    else
        goto L_2b3e;

L_2b3e:
    *(lpth + 0x6) = ((*(lpth + 0x6) & 0xc3ff) | ((iWarp & 0xf) << 0xa));
    *(lpth + 0xe) = (lpth->thp.wtMax | ((iDecayRate & 0x3) << 0xe));
    *(lpth + 0x6) = ((*(lpth + 0x6) & 0xfc00) | ((lppl->idFling + 0xffff) & 0x3ff));
    lpth->pt.x = rgptPlan[lppl->id].x;
    lpth->pt.y = rgptPlan[lppl->id].y;
    FSendPlrMsg2(lppl->iPlayer, 0xd3, lppl->id, lppl->id, (lppl->idFling + 0xffff));
    goto L_2fc0;
    i = 0;
    goto L_2c11;

L_2c0d:
    i = (i + 1);

L_2c11:
    if ((i >= game.cPlayer))
        goto L_2c3e;
    else
        goto L_2c1c;

L_2c1c:
    FSendPlrMsg2(i, 0x11b, lppl->id, lppl->id, 0x0);
    goto L_2c0d;

L_2c3e:
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) == raMacintosh))
        goto L_2cea;
    else
        goto L_2c62;

L_2c62:
    *(lppl + 0x14) = ((*(lppl + 0x14) & 0xffff) | 0x0);
    lppl->cFactories = 0x0;
    lppl->cMines = 0x0;
    lppl->cDefenses = 0x0;
    *(lppl + 0x1a) = ((*(lppl + 0x1a) & 0xffff) | 0x0);
    lppl->iScanner = 0x1f;

L_2cea:
    i = 0;
    goto L_2d9c;

L_2cf2:
    lppl->rgwtMin[i] = 0;
    /* untranslated: ss:[bp-0x16] = Random(0x32) */
    Random(0x32);
    /* untranslated: lppl->rgEnvVarOrig[i] = lobyte(((callresult(int16_t) + 0x1) + ss:[bp-0x16])) */
    /* untranslated: lppl->rgEnvVar[i] = lobyte(((callresult(int16_t) + 0x1) + ss:[bp-0x16])) */
    /* untranslated: ss:[bp-0x16] = Random(0x28) */
    /* untranslated: lppl->rgMinConc[i] = lobyte(((Random(0x28) + 0x19) + ss:[bp-0x16])) */
    i = (i + 1);

L_2d9c:
    if ((i >= 3))
        goto L_2fc9;
    else
        goto L_2da2;

L_2da2:

L_2f77:
    if ((iItem > 27))
        goto L_23d6;
    else
        goto L_2f7f;

L_2f7f:
    goto L_ffffffff;

L_2fc0:

L_2fc3:
    return 0x0;

L_2fc9:
    return 0x1;

L_2fcf:
}

void CreateShip(int16_t iPlr, FLEET *lpfl, int16_t ishdef, int16_t cShip) {
L_2fd6:
    lpfl->rgcsh[ishdef] = (lpfl->rgcsh[ishdef] + cShip);
    rglpshdef[iPlr][ishdef].cExist = (rglpshdef[iPlr][ishdef].cExist + (uint32_t)(cShip));
    rglpshdef[iPlr][ishdef].cBuilt = (rglpshdef[iPlr][ishdef].cBuilt + (uint32_t)(cShip));
    return;
}

void RandomEvents() {
L_3064:
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
    uint16_t  t_merge_31f3_0001;
    uint16_t  t_merge_3216_0001;
    uint16_t  t_merge_324a_0001;
    uint16_t  t_merge_32de_0001;
    uint16_t  t_merge_33af_0001;
    uint16_t  t_merge_33d2_0001;
    uint16_t  t_merge_3406_0001;
    uint16_t  t_merge_346d_0001;

L_3088:
    if ((cXferFull == 0))
        goto L_34dc;
    else
        goto L_3098;

L_3098:

L_309e:
    lpxfCur = lpxf;
    lpxfMax = &(lpxf[cXferFull]);
    goto L_34cb;

L_30c9:
    /* untranslated: branch FLookupObject(((*(lpxfCur+0x4) >> 0x4) & 0xf), lpxfCur->id2, &part[4:2](rgxf[0x1])) == 0 ? L_34c6 : L_30fb */

L_30fb:

L_3101:
    if ((lpxfCur->grobj1 != 0x1))
        goto L_3136;
    else
        goto L_3117;

L_3117:
    /* untranslated: part[6:2](rgxf[0x0]) = LpplFromId(lpxfCur->id1)->iPlayer */
    goto L_3149;

L_3136:
    /* untranslated: part[6:2](rgxf[0x0]) = ((lpxfCur->id1 >> 0x9) & 0xf) */

L_3149:
    i = 0;
    goto L_34bc;

L_3152:
    l2 = lpxfCur->rgcQuan[i];
    if ((LOWORD(l2) != 0x0))
        goto L_318a;
    else
        goto L_3181;

L_3181:
    if ((HIWORD(l2) == 0x0))
        goto L_34b7;
    else
        goto L_318a;

L_318a:
    l = ChgCargo(((*(lpxfCur + 0x4) >> 0x4) & 0xf), lpxfCur->id2, i, l2, 0x0);
    if ((LOWORD(l) != LOWORD(l2)))
        goto L_31e0;
    else
        goto L_31d7;

L_31d7:
    if ((HIWORD(l) == HIWORD(l2)))
        goto L_339c;
    else
        goto L_31e0;

L_31e0:
    if ((i != 4))
        goto L_31f0;
    else
        goto L_31ea;

L_31ea:
    t_merge_31f3_0001 = 0x47;
    goto L_31f3;

L_31f0:
    t_merge_31f3_0001 = 0x46;

L_31f3:
    idm = t_merge_31f3_0001;
    if ((lpxfCur->grobj1 != 0x2))
        goto L_3213;
    else
        goto L_320d;

L_320d:
    t_merge_3216_0001 = 0x8000;
    goto L_3216;

L_3213:
    t_merge_3216_0001 = 0x0;

L_3216:
    idSrc = (lpxfCur->id1 | t_merge_3216_0001);
    if ((((*(lpxfCur + 0x4) >> 0x4) & 0xf) != 0x2))
        goto L_3247;
    else
        goto L_3241;

L_3241:
    t_merge_324a_0001 = 0x8000;
    goto L_324a;

L_3247:
    t_merge_324a_0001 = 0x0;

L_324a:
    idDst = (lpxfCur->id2 | t_merge_324a_0001);
    if ((LOWORD(l) != 0x0))
        goto L_3270;
    else
        goto L_3261;

L_3261:
    if ((HIWORD(l) != 0x0))
        goto L_3270;
    else
        goto L_326b;

L_326b:
    idm = (idm + 4);

L_3270:
    /* untranslated: call FSendPlrMsg(part[6:2](rgxf[0x0]), idm, idSrc, idSrc, LOWORD(l2), (loword((uint32_t)(l2 >> 0x10)) & 0xffff), i, idDst, LOWORD(l),
     * (loword((uint32_t)(l >> 0x10)) & 0xffff)) -> callresult(int16_t) */
    if ((i != 4))
        goto L_32db;
    else
        goto L_32d5;

L_32d5:
    t_merge_32de_0001 = 0x49;
    goto L_32de;

L_32db:
    t_merge_32de_0001 = 0x48;

L_32de:
    idm = t_merge_32de_0001;
    if ((LOWORD(l) != 0x0))
        goto L_333f;
    else
        goto L_32ec;

L_32ec:
    if ((HIWORD(l) != 0x0))
        goto L_333f;
    else
        goto L_32f6;

L_32f6:
    /* untranslated: call FSendPlrMsg(part[6:2](rgxf[0x1]), (idm + 4), idDst, idDst, LOWORD(l2), (loword((uint32_t)(l2 >> 0x10)) & 0xffff), i, idSrc, 0x0, 0x0)
     * -> callresult(int16_t) */
    goto L_34b7;

L_333f:
    /* untranslated: call FSendPlrMsg(part[6:2](rgxf[0x1]), idm, idDst, idDst, LOWORD(l), (loword((uint32_t)(l >> 0x10)) & 0xffff), i, idSrc, LOWORD(l2),
     * (loword((uint32_t)(l2 >> 0x10)) & 0xffff)) -> callresult(int16_t) */

L_339c:
    if ((i != 4))
        goto L_33ac;
    else
        goto L_33a6;

L_33a6:
    t_merge_33af_0001 = 0x43;
    goto L_33af;

L_33ac:
    t_merge_33af_0001 = 0x42;

L_33af:
    idm = t_merge_33af_0001;
    if ((lpxfCur->grobj1 != 0x2))
        goto L_33cf;
    else
        goto L_33c9;

L_33c9:
    t_merge_33d2_0001 = 0x8000;
    goto L_33d2;

L_33cf:
    t_merge_33d2_0001 = 0x0;

L_33d2:
    idSrc = (lpxfCur->id1 | t_merge_33d2_0001);
    if ((((*(lpxfCur + 0x4) >> 0x4) & 0xf) != 0x2))
        goto L_3403;
    else
        goto L_33fd;

L_33fd:
    t_merge_3406_0001 = 0x8000;
    goto L_3406;

L_3403:
    t_merge_3406_0001 = 0x0;

L_3406:
    idDst = (lpxfCur->id2 | t_merge_3406_0001);
    /* untranslated: call FSendPlrMsg(part[6:2](rgxf[0x0]), idm, idSrc, idSrc, LOWORD(l), (loword((uint32_t)(l >> 0x10)) & 0xffff), i, idDst, 0x0, 0x0) ->
     * callresult(int16_t) */
    if ((i != 4))
        goto L_346a;
    else
        goto L_3464;

L_3464:
    t_merge_346d_0001 = 0x45;
    goto L_346d;

L_346a:
    t_merge_346d_0001 = 0x44;

L_346d:
    idm = t_merge_346d_0001;
    /* untranslated: call FSendPlrMsg(part[6:2](rgxf[0x1]), idm, idDst, idDst, LOWORD(l), (loword((uint32_t)(l >> 0x10)) & 0xffff), i, idSrc, 0x0, 0x0) ->
     * callresult(int16_t) */

L_34b7:
    i = (i + 1);

L_34bc:
    if ((i < 5))
        goto L_3152;
    else
        goto DoNext;

DoNext:
    lpxfCur = (lpxfCur + 0x1);

L_34cb:
    if ((LOWORD(lpxfCur) < LOWORD(lpxfMax)))
        goto L_30c9;
    else
        goto L_34dc;

L_34dc:
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
    uint16_t t_merge_3ea3_0001;

L_34e2:
    if ((cColDrop == 0))
        goto L_4445;
    else
        goto L_34f2;

L_34f2:

L_34f8:
    lpcdCur = lpcd;
    lpcdMax = &(lpcd[cColDrop]);
    goto L_442f;

L_3521:
    if ((lpcdCur->idPlanetDst == -1))
        goto IncCur;
    else
        goto L_352e;

L_352e:
    if ((LOWORD(lpcdCur->cColonist) != 0x0))
        goto L_3548;
    else
        goto L_353b;

L_353b:
    if ((HIWORD(lpcdCur->cColonist) == 0x0))
        goto IncCur;
    else
        goto L_3542;

L_3542:

L_3548:
    memset(rgcCol, 0x0, 0x40);
    memset(rgcPower, 0x0, 0x40);
    cPowerTot = 0;
    cColTot = 0;
    idPlanet = lpcdCur->idPlanetDst;
    FLookupPlanet(idPlanet, &(pl));
    iplrOldOwner = pl.iPlayer;
    CalcPctSurvive(&(pl), &(pctSurvive), 0x0);
    pctSurvive = (pctSurvive + ((1 - pctSurvive) / 4));
    lpcdLook = lpcdCur;
    goto L_382c;

L_35f2:
    if ((idPlanet != lpcdLook->idPlanetDst))
        goto L_3828;
    else
        goto L_3601;

L_3601:
    if ((GetRaceStat(rgplr[lpcdLook->idPlr], rsMajorAdv) != raMacintosh))
        goto L_3660;
    else
        goto L_3625;

L_3625:
    if ((lpcdLook->fCanColonize == 0x0))
        goto L_3640;
    else
        goto L_3637;

L_3637:
    if ((pl.iPlayer == -1))
        goto L_3660;
    else
        goto L_3640;

L_3640:
    FSendPlrMsg2(lpcdLook->idPlr, 0x57, pl.id, pl.id, 0x0);
    goto L_381f;

L_3660:
    if ((pl.iPlayer != -1))
        goto L_36cd;
    else
        goto L_3669;

L_3669:
    if ((lpcdLook->fCanColonize != 0x0))
        goto L_36cd;
    else
        goto L_367b;

L_367b:
    FSendPlrMsg(lpcdLook->idPlr, 0x2, pl.id, LOWORD(lpcdLook->cColonist), (LOWORD((uint32_t)((lpcdLook->cColonist >> 0x10))) & 0xffff), pl.id, 0x0, 0x0, 0x0,
                0x0);
    goto L_381f;

L_36cd:
    if ((pl.fStarbase == 0x0))
        goto L_3709;
    else
        goto L_36e0;

L_36e0:
    if ((pl.iPlayer == -1))
        goto L_3709;
    else
        goto L_36e9;

L_36e9:
    FSendPlrMsg2(lpcdLook->idPlr, 0x58, pl.id, pl.id, 0x0);
    goto L_381f;

L_3709:
    rgcCol[lpcdLook->idPlr] = (rgcCol[lpcdLook->idPlr] + lpcdLook->cColonist);
    cColTot = (cColTot + lpcdLook->cColonist);
    if ((GetRaceStat(rgplr[lpcdLook->idPlr], rsMajorAdv) != raAttack))
        goto L_376e;
    else
        goto L_375f;

L_375f:
    lPower = 165;
    goto L_37ad;

L_376e:
    if ((GetRaceStat(rgplr[lpcdLook->idPlr], rsMajorAdv) != raMacintosh))
        goto L_37a1;
    else
        goto L_3792;

L_3792:
    lPower = 0;
    goto L_37ad;

L_37a1:
    lPower = 110;

L_37ad:
    /* untranslated: ss:[bp-0xfa] = loword((int32_t)((uint32_t)(lpcdLook->cColonist * lPower) / 0x64)) */
    /* untranslated: ss:[bp-0xf8] = hiword((int32_t)((uint32_t)(lpcdLook->cColonist * lPower) / 0x64)) */
    lPower = __ftol();
    cPowerTot = (cPowerTot + lPower);
    rgcPower[lpcdLook->idPlr] = (rgcPower[lpcdLook->idPlr] + lPower);

L_381f:
    lpcdLook->idPlanetDst = -1;

L_3828:
    lpcdLook = (lpcdLook + 0x1);

L_382c:
    if ((LOWORD(lpcdLook) < LOWORD(lpcdMax)))
        goto L_35f2;
    else
        goto L_383c;

L_383c:
    if ((pl.iPlayer == -1))
        goto L_3aee;
    else
        goto L_3845;

L_3845:
    if ((GetRaceStat(rgplr[pl.iPlayer], rsMajorAdv) != raDefend))
        goto L_3874;
    else
        goto L_3865;

L_3865:
    lPower = 200;
    goto L_3880;

L_3874:
    lPower = 100;

L_3880:
    lDefensePower = (int32_t)(((uint32_t)((pl.rgwtMin[3] * lPower)) / 0x64));
    if ((HIWORD(lDefensePower) < HIWORD(cPowerTot)))
        goto L_3ad0;
    else
        goto L_38b6;

L_38b6:
    if ((HIWORD(lDefensePower) > HIWORD(cPowerTot)))
        goto L_38c3;
    else
        goto L_38bb;

L_38bb:
    if ((LOWORD(lDefensePower) <= LOWORD(cPowerTot)))
        goto L_3ad0;
    else
        goto L_38c3;

L_38c3:
    i = 0;
    goto L_3a9d;

L_38cb:
    if ((LOWORD(rgcCol[i]) != 0x0))
        goto L_38e9;
    else
        goto L_38e0;

L_38e0:
    if ((HIWORD(rgcCol[i]) == 0x0))
        goto L_3a99;
    else
        goto L_38e9;

L_38e9:
    if ((pctSurvive != 1))
        goto L_39bb;
    else
        goto L_3900;

L_3900:
    FSendPlrMsg(i, 0x0, pl.id, LOWORD(rgcCol[i]), (LOWORD((uint32_t)((rgcCol[i] >> 0x10))) & 0xffff), pl.id, (pl.iPlayer | 0x30), 0x0, 0x0, 0x0);
    FSendPlrMsg(pl.iPlayer, 0x3, pl.id, pl.id, LOWORD(rgcCol[i]), (LOWORD((uint32_t)((rgcCol[i] >> 0x10))) & 0xffff), (i | 0x30), 0x0, 0x0, 0x0);
    goto L_3a99;

L_39bb:
    /* untranslated: ss:[bp-0xfa] = 0x2710 */
    /* untranslated: ss:[bp-0xf8] = 0x0 */
    FSendPlrMsg(i, 0x1, pl.id, LOWORD(rgcCol[i]), (LOWORD((uint32_t)((rgcCol[i] >> 0x10))) & 0xffff), pl.id, LOWORD(__ftol()), (pl.iPlayer | 0x30), 0x0, 0x0);
    FSendPlrMsg(pl.iPlayer, 0x4, pl.id, pl.id, LOWORD(rgcCol[i]), (LOWORD((uint32_t)((rgcCol[i] >> 0x10))) & 0xffff), (i | 0x30), 0x0, 0x0, 0x0);

L_3a99:
    i = (i + 1);

L_3a9d:
    if ((i < 16))
        goto L_38cb;
    else
        goto L_3aa6;

L_3aa6:
    pl.rgwtMin[3] = (pl.rgwtMin[3] - (int32_t)(((uint32_t)((pl.rgwtMin[3] * cPowerTot)) / lDefensePower)));
    goto WritePlanet;

L_3ad0:
    lOldPop = pl.rgwtMin[3];
    UninhabitPlanet(&(pl));
    goto L_3b02;

L_3aee:
    lDefensePower = 0;
    lOldPop = 0;

L_3b02:
    cMax = -1;
    c2nd = 0;
    cSides = 0;
    fTie = 0;
    iMax = 0;
    i = 0;
    goto L_3b2f;

L_3b2b:
    i = (i + 1);

L_3b2f:
    if ((i >= game.cPlayer))
        goto L_3be0;
    else
        goto L_3b3a;

L_3b3a:
    if ((LOWORD(rgcCol[i]) != 0x0))
        goto L_3b58;
    else
        goto L_3b4f;

L_3b4f:
    if ((HIWORD(rgcCol[i]) == 0x0))
        goto L_3b2b;
    else
        goto L_3b58;

L_3b58:
    cSides = (cSides + 1);
    if ((HIWORD(rgcPower[i]) < HIWORD(cMax)))
        goto L_3b2b;
    else
        goto L_3b78;

L_3b78:
    if ((HIWORD(rgcPower[i]) > HIWORD(cMax)))
        goto L_3b84;
    else
        goto L_3b7d;

L_3b7d:
    if ((LOWORD(rgcPower[i]) < LOWORD(cMax)))
        goto L_3b2b;
    else
        goto L_3b84;

L_3b84:
    if ((LOWORD(rgcPower[i]) != LOWORD(cMax)))
        goto L_3bae;
    else
        goto L_3b9e;

L_3b9e:
    if ((HIWORD(rgcPower[i]) != HIWORD(cMax)))
        goto L_3bae;
    else
        goto L_3ba6;

L_3ba6:
    fTie = 1;
    goto L_3b2b;

L_3bae:
    fTie = 0;
    c2nd = cMax;
    cMax = rgcPower[i];
    iMax = i;

L_3be0:
    if ((HIWORD(cMax) > 0x0))
        goto L_3bfa;
    else
        goto L_3be9;

L_3be9:
    if ((HIWORD(cMax) < 0x0))
        goto IncCur;
    else
        goto L_3bee;

L_3bee:
    if ((LOWORD(cMax) < 0x0))
        goto IncCur;
    else
        goto L_3bf4;

L_3bf4:

L_3bfa:
    if ((fTie == 0))
        goto L_3c7e;
    else
        goto L_3c03;

L_3c03:
    i = 0;
    goto L_3c0f;

L_3c0b:
    i = (i + 1);

L_3c0f:
    if ((i >= game.cPlayer))
        goto L_3c54;
    else
        goto L_3c1a;

L_3c1a:
    if ((LOWORD(rgcCol[i]) != 0x0))
        goto L_3c38;
    else
        goto L_3c2f;

L_3c2f:
    if ((HIWORD(rgcCol[i]) == 0x0))
        goto L_3c0b;
    else
        goto L_3c38;

L_3c38:
    FSendPlrMsg2(i, 0x6, pl.id, cSides, pl.id);

L_3c54:
    if ((iplrOldOwner == -1))
        goto WritePlanet;
    else
        goto L_3c5d;

L_3c5d:
    FSendPlrMsg2(iplrOldOwner, 0x5, pl.id, cSides, pl.id);
    pl.iPlayer = -1;

L_3c7e:
    if ((iplrOldOwner == -1))
        goto L_3dd9;
    else
        goto L_3c87;

L_3c87:
    i = 0;
    goto L_3cf4;

L_3c8f:
    if ((LOWORD(rgcCol[i]) != 0x0))
        goto L_3cad;
    else
        goto L_3ca4;

L_3ca4:
    if ((HIWORD(rgcCol[i]) == 0x0))
        goto L_3cf0;
    else
        goto L_3cad;

L_3cad:
    if ((i != iMax))
        goto L_3cd7;
    else
        goto L_3cb8;

L_3cb8:
    FSendPlrMsg2(i, 0xc, pl.id, (iplrOldOwner | 0x20), pl.id);
    goto L_3cf0;

L_3cd7:
    FSendPlrMsg2(i, 0xd, pl.id, pl.id, 0x0);

L_3cf0:
    i = (i + 1);

L_3cf4:
    if ((i < 16))
        goto L_3c8f;
    else
        goto L_3cfd;

L_3cfd:
    FSendPlrMsg(iplrOldOwner, 0x7, pl.id, (iMax | 0x30), pl.id, LOWORD(rgcCol[iMax]), (LOWORD((uint32_t)((rgcCol[iMax] >> 0x10))) & 0xffff), 0x0, 0x0, 0x0);
    memset(&(rgTechBattle), 0x0, 0x6);
    memset(&(rgTechTrader), 0x0, 0xd);
    i = 0;
    goto L_3dab;

L_3d89:
    rgTechBattle[i] = rgplr[iplrOldOwner].rgTech[i];
    i = (i + 1);

L_3dab:
    if ((i < 6))
        goto L_3d89;
    else
        goto L_3db4;

L_3db4:
    i = ITechLearnATech(iMax, 0xffff, pl.id, idmWreckageDiscoveredBattleHasBoostedResearchResour, 0x0);
    pl.iPlayer = -1;
    goto L_3eb2;

L_3dd9:
    if ((cSides <= 1))
        goto L_3e70;
    else
        goto L_3de3;

L_3de3:
    i = 0;
    goto L_3e64;

L_3deb:
    if ((LOWORD(rgcCol[i]) != 0x0))
        goto L_3e09;
    else
        goto L_3e00;

L_3e00:
    if ((HIWORD(rgcCol[i]) == 0x0))
        goto L_3e60;
    else
        goto L_3e09;

L_3e09:
    if ((i != iMax))
        goto L_3e30;
    else
        goto L_3e14;

L_3e14:
    FSendPlrMsg2(i, 0x8, pl.id, cSides, pl.id);
    goto L_3e60;

L_3e30:
    FSendPlrMsg(i, 0x9, pl.id, cSides, pl.id, (iMax | 0xb0), 0x0, 0x0, 0x0, 0x0);

L_3e60:
    i = (i + 1);

L_3e64:
    if ((i >= 16))
        goto L_3eb2;
    else
        goto L_3e6a;

L_3e6a:

L_3e70:
    if ((GetRaceStat(rgplr[iMax], rsMajorAdv) != raMacintosh))
        goto L_3ea0;
    else
        goto L_3e9a;

L_3e9a:
    t_merge_3ea3_0001 = 0x1;
    goto L_3ea3;

L_3ea0:
    t_merge_3ea3_0001 = 0x0;

L_3ea3:
    FSendPlrMsg2(iMax, (t_merge_3ea3_0001 + 0xa), pl.id, pl.id, 0x0);

L_3eb2:
    if ((iMax == -1))
        goto L_418e;
    else
        goto L_3ebb;

L_3ebb:
    cpq = rgplr[iMax].zpq1.cpq;
    /* untranslated: ss:[bp-0xf6] = 0x0 */
    /* untranslated: part[24:2](pl) = ((part[24:2](pl) & 0xffff) | loword((int32_t)(words((rgplr[iMax].zpq1.fNoResearch & 0x1), 0x0) << 0x17))) */
    /* untranslated: part[26:2](pl) = ((part[26:2](pl) & 0xff7f) | hiword((int32_t)(words((ss:[bp-0xf8] & 0x1), 0x0) << 0x17))) */
    if ((cpq <= 0))
        goto L_418e;
    else
        goto L_3f27;

L_3f27:
    pl.lpplprod = LpplAlloc(0x4, rgplr[iMax].zpq1.cpq, htOrd);
    memset(prod, 0x0, 0x4);
    LOWORD(prod) = ((LOWORD(prod) & 0xffff) | 0x0);
    prod.grobj = grobjPlanet;
    iDst = 0;
    ipq = 0;
    goto L_4134;

L_3f90:
    if ((GetRaceStat(rgplr[iMax], rsMajorAdv) != raMacintosh))
        goto L_3fd8;
    else
        goto L_3fb0;

L_3fb0:
    if (((rgplr[iMax].zpq1.rgpq[ipq].w & 0x3f) <= 0x2))
        goto L_412f;
    else
        goto L_3fd2;

L_3fd2:

L_3fd8:
    if ((GetRaceStat(rgplr[iMax], rsMajorAdv) != raTerra))
        goto L_4045;
    else
        goto L_3ff8;

L_3ff8:
    if (((rgplr[iMax].zpq1.rgpq[ipq].w & 0x3f) == 0x4))
        goto L_412f;
    else
        goto L_401d;

L_401d:
    if (((rgplr[iMax].zpq1.rgpq[ipq].w & 0x3f) == 0x5))
        goto L_412f;
    else
        goto L_403f;

L_403f:

L_4045:
    /* untranslated: ss:[bp-0xfe] = 0x0 */
    /* untranslated: LOWORD(prod) = (prod.cItem | loword((int32_t)(words(((rgplr[iMax].zpq1.rgpq[ipq].w & 0x3f) & 0x7f), 0x0) << 0xa))) */
    /* untranslated: HIWORD(prod) = ((HIWORD(prod) & 0xfffe) | hiword((int32_t)(words(((rgplr[iMax].zpq1.rgpq[ipq].w & 0x3f) & 0x7f), 0x0) << 0xa))) */
    /* untranslated: ss:[bp-0xfe] = 0x0 */
    /* untranslated: prod.cItem = loword((int32_t)(words((((rgplr[iMax].zpq1.rgpq[ipq].w >> 0x6) & 0x3ff) & 0x3ff), 0x0) << 0x0)) */
    /* untranslated: HIWORD(prod) = ((HIWORD(prod) & 0xffff) | hiword((int32_t)(words((((rgplr[iMax].zpq1.rgpq[ipq].w >> 0x6) & 0x3ff) & 0x3ff), 0x0) << 0x0)))
     */
    LOWORD(pl.lpplprod[(iDst + 1)]) = LOWORD(prod);
    HIWORD(pl.lpplprod[iDst].rgprod[0x0]) = HIWORD(prod);
    iDst = (iDst + 1);

L_412f:
    ipq = (ipq + 1);

L_4134:
    if ((ipq < cpq))
        goto L_3f90;
    else
        goto L_4141;

L_4141:
    if ((iDst <= 0))
        goto L_4176;
    else
        goto L_414b;

L_414b:
    pl.lpplprod->iprodMac = LOBYTE(iDst);
    LpplFromId(pl.id)->lpplprod = pl.lpplprod;
    goto L_418e;

L_4176:
    FreePl(pl.lpplprod);
    pl.lpplprod = 0x0;

L_418e:
    pl.iPlayer = iMax;
    if ((GetRaceStat(rgplr[iMax], rsMajorAdv) != raMacintosh))
        goto L_41f9;
    else
        goto L_41b4;

L_41b4:
    pl.fStarbase = 0x1;
    pl.isb = 0x0;
    rglpshdefSB[iMax]->cExist = (rglpshdefSB[iMax]->cExist + 0x1);
    rglpshdefSB[iMax]->cBuilt = (rglpshdefSB[iMax]->cBuilt + 0x1);

L_41f9:
    if ((LOWORD(cPowerTot) != 0x0))
        goto L_420b;
    else
        goto L_4202;

L_4202:
    if ((HIWORD(cPowerTot) == 0x0))
        goto L_421d;
    else
        goto L_420b;

L_420b:
    if ((LOWORD(cMax) != 0x0))
        goto L_4238;
    else
        goto L_4214;

L_4214:
    if ((HIWORD(cMax) != 0x0))
        goto L_4238;
    else
        goto L_421d;

L_421d:
    pl.rgwtMin[3] = rgcCol[iMax];
    goto L_4298;

L_4238:
    /* untranslated: lPower = (int32_t)((uint32_t)(cMax * words((HIWORD(cPowerTot) - HIWORD(lDefensePower)), (LOWORD(cPowerTot) - LOWORD(lDefensePower)))) /
     * cPowerTot) */
    pl.rgwtMin[3] = (int32_t)(((uint32_t)((rgcCol[iMax] * lPower)) / cMax));

L_4298:
    if ((HIWORD(c2nd) < 0x0))
        goto L_42db;
    else
        goto L_42a1;

L_42a1:
    if ((HIWORD(c2nd) > 0x0))
        goto L_42af;
    else
        goto L_42a6;

L_42a6:
    if ((LOWORD(c2nd) <= 0x0))
        goto L_42db;
    else
        goto L_42af;

L_42af:
    /* untranslated: pl.rgwtMin[3] = (int32_t)((uint32_t)(pl.rgwtMin[3] * words((HIWORD(cMax) - HIWORD(c2nd)), (LOWORD(cMax) - LOWORD(c2nd)))) / cMax) */

L_42db:
    if ((HIWORD(pl.rgwtMin[0x3]) > 0x0))
        goto WritePlanet;
    else
        goto L_42e4;

L_42e4:
    if ((HIWORD(pl.rgwtMin[0x3]) < 0x0))
        goto L_42f2;
    else
        goto L_42e9;

L_42e9:
    if ((LOWORD(pl.rgwtMin[0x3]) >= 0x1))
        goto WritePlanet;
    else
        goto L_42f2;

L_42f2:
    pl.rgwtMin[3] = 1;

WritePlanet:
    if ((pl.iPlayer == -1))
        goto L_441b;
    else
        goto L_4305;

L_4305:
    if ((pl.fArtifact != 0x0))
        goto L_4329;
    else
        goto L_4321;

L_4321:
    if ((0x0 == 0x0))
        goto L_441b;
    else
        goto L_4329;

L_4329:
    /* untranslated: part[24:2](pl) = ((part[24:2](pl) & 0xffff) | 0x0) */
    pl.fArtifact = 0x0;
    if ((game.fNoRandom != 0x0))
        goto L_441b;
    else
        goto L_4354;

L_4354:
    iTech = Random(0x6);
    iBonus = (Random(0x12d) + 100);
    if ((HIWORD(pl.rgwtMin[0x3]) > 0x0))
        goto L_43a2;
    else
        goto L_4380;

L_4380:
    if ((HIWORD(pl.rgwtMin[0x3]) < 0x0))
        goto L_438e;
    else
        goto L_4385;

L_4385:
    if ((LOWORD(pl.rgwtMin[0x3]) >= 0xa))
        goto L_43a2;
    else
        goto L_438e;

L_438e:
    /* untranslated: iBonus = (words(loword((LOWORD(pl.rgwtMin[0x3]) * iBonus)), signhiword(loword((LOWORD(pl.rgwtMin[0x3]) * iBonus)))) / 10) */

L_43a2:
    FSendPlrMsg(pl.iPlayer, 0x5e, 0xfffe, pl.id, iTech, iBonus, 0x0, 0x0, 0x0, 0x0);
    rgplr[pl.iPlayer].rgResSpent[iTech] = (rgplr[pl.iPlayer].rgResSpent[iTech] + (uint32_t)(iBonus));
    if ((game.fSlowTech == 0x0))
        goto L_441b;
    else
        goto L_4414;

L_4414:
    iBonus = (iBonus >> 0x1);

L_441b:
    FLookupPlanet(0xffff, &(pl));

IncCur:
    lpcdCur = (lpcdCur + 0x1);

L_442f:
    if ((LOWORD(lpcdCur) < LOWORD(lpcdMax)))
        goto L_3521;
    else
        goto L_443f;

L_443f:
    cColDrop = 0;

L_4445:
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

L_444c:
    pctShipHeal = 0;
    i = 0;
    goto L_4466;

L_4462:
    i = (i + 1);

L_4466:
    if ((i >= cFleet))
        goto L_47a8;
    else
        goto L_4471;

L_4471:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_44a1;
    else
        goto L_4499;

L_4499:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_47a8;
    else
        goto L_44a1;

L_44a1:
    if ((lpfl->fDead != 0x0))
        goto L_4462;
    else
        goto L_44b8;

L_44b8:
    if ((lpfl->fNoHeal != 0x0))
        goto L_4462;
    else
        goto L_44cf;

L_44cf:
    dpHeal = 0;
    pctShipHeal = 0;
    ishdef = 0;
    goto L_4586;

L_44e1:
    /* untranslated: branch HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 2))] == 0x0 ? L_4503 : L_44fe */

L_44fe:
    dpHeal = 1;

L_4503:
    if ((lpfl->rgcsh[ishdef] == 0))
        goto L_4582;
    else
        goto L_4520;

L_4520:
    if ((rglpshdef[lpfl->iPlayer][ishdef].hul.ihuldef != ihuldefSuperFuelXport))
        goto L_454e;
    else
        goto L_4546;

L_4546:
    pctShipHeal = 50;
    goto L_4582;

L_454e:
    if ((pctShipHeal >= 5))
        goto L_4582;
    else
        goto L_4557;

L_4557:
    if ((rglpshdef[lpfl->iPlayer][ishdef].hul.ihuldef != ihuldefFuelTransport))
        goto L_4582;
    else
        goto L_457d;

L_457d:
    pctShipHeal = 25;

L_4582:
    ishdef = (ishdef + 1);

L_4586:
    if ((ishdef < 16))
        goto L_44e1;
    else
        goto L_458f;

L_458f:
    if ((dpHeal == 0))
        goto L_4462;
    else
        goto L_4595;

L_4595:

L_459b:
    if ((lpfl->fHereAllTurn != 0x0))
        goto L_45ba;
    else
        goto L_45b2;

L_45b2:
    pct = 5;
    goto L_469d;

L_45ba:
    if ((lpfl->idPlanet != -1))
        goto L_45cf;
    else
        goto L_45c7;

L_45c7:
    pct = 10;
    goto L_469d;

L_45cf:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((lppl->iPlayer != lpfl->iPlayer))
        goto L_4683;
    else
        goto L_45f7;

L_45f7:
    if ((lppl->fStarbase == 0x0))
        goto L_467b;
    else
        goto L_460e;

L_460e:
    if ((lppl->fNoHeal != 0x0))
        goto L_467b;
    else
        goto L_4625;

L_4625:
    lpshdef = &(rglpshdefSB[lppl->iPlayer][lppl->isb]);
    if ((LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax == 0x0))
        goto L_4673;
    else
        goto L_466b;

L_466b:
    pct = 100;
    goto L_469d;

L_4673:
    pct = 40;

L_467b:
    pct = 25;

L_4683:
    if ((lppl->iPlayer != -1))
        goto L_4698;
    else
        goto L_4690;

L_4690:
    pct = 15;
    goto L_469d;

L_4698:
    pct = 15;

L_469d:
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) != raDefend))
        goto L_46c9;
    else
        goto L_46c1;

L_46c1:
    pct = (pct * 2);

L_46c9:
    pct = (pct + pctShipHeal);
    ishdef = 0;
    goto L_479c;

L_46d7:
    /* untranslated: branch HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 2))] == 0x0 ? L_4798 : L_46f4 */

L_46f4:
    /* untranslated: branch ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 2))] >> 0x7) & 0x1ff) <= pct ? L_477f : L_471d */

L_471d:
    /* untranslated: ss:[bp-0x1e] = ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] - (pct << 0x7)) & 0xff80) */
    /* untranslated: HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 2))] = (HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) */
    /* untranslated: HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 2))] = (HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] | ss:[bp-0x1e]) */
    goto L_4798;

L_477f:
    /* untranslated: HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 2))] = 0x0 */

L_4798:
    ishdef = (ishdef + 1);

L_479c:
    if ((ishdef >= 16))
        goto L_4462;
    else
        goto L_47a2;

L_47a2:

L_47a8:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_48e2;

L_47d3:
    if ((lppl->fStarbase == 0x0))
        goto L_48de;
    else
        goto L_47ea;

L_47ea:
    if ((lppl->fNoHeal != 0x0))
        goto L_48de;
    else
        goto L_4801;

L_4801:
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) != raDefend))
        goto L_482d;
    else
        goto L_4825;

L_4825:
    pct = 75;
    goto L_4832;

L_482d:
    pct = 50;

L_4832:
    if ((lppl->pctDp == 0x0))
        goto L_48de;
    else
        goto L_484c;

L_484c:
    lpshdef = &(rglpshdefSB[lppl->iPlayer][lppl->isb]);
    if ((pct <= lppl->pctDp))
        goto L_48aa;
    else
        goto L_4893;

L_4893:
    lppl->pctDp = 0x0;
    goto L_48de;

L_48aa:
    /* untranslated: ss:[bp-0x1e] = ((*(lppl+0x2c) - (pct << 0x4)) & 0xfff0) */
    lppl->pctDp = 0x0;
    /* untranslated: *(lppl+0x2c) = (*(lppl+0x2c) | ss:[bp-0x1e]) */

L_48de:
    lppl = (lppl + 0x1);

L_48e2:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_47d3;
    else
        goto L_48f0;

L_48f0:
    return;
}

void AutoTerraform() {
    int16_t  rgMax[3];
    int16_t  rgp[16];
    PLANET  *lppl;
    int16_t  i;
    int16_t  rgMin[3];
    int16_t  rgCost[3];
    int16_t  fTerra;
    PLANET  *lpplMac;
    uint16_t t_merge_4944_0001;

L_48f6:
    fTerra = 0;
    i = 0;
    goto L_4910;

L_490c:
    i = (i + 1);

L_4910:
    if ((i >= game.cPlayer))
        goto L_496a;
    else
        goto L_491b;

L_491b:
    if ((GetRaceStat(rgplr[i], rsMajorAdv) != raTerra))
        goto L_4941;
    else
        goto L_493b;

L_493b:
    t_merge_4944_0001 = 0x1;
    goto L_4944;

L_4941:
    t_merge_4944_0001 = 0x0;

L_4944:
    rgp[i] = t_merge_4944_0001;
    if ((rgp[i] == 0))
        goto L_490c;
    else
        goto L_4962;

L_4962:
    fTerra = 1;

L_496a:
    if ((fTerra == 0))
        goto L_4c4f;
    else
        goto L_4970;

L_4970:

L_4976:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_4c41;

L_49a1:
    if ((lppl->iPlayer == -1))
        goto L_4c3d;
    else
        goto L_49ae;

L_49ae:
    if ((rgp[lppl->iPlayer] == 0))
        goto L_4c3d;
    else
        goto L_49c1;

L_49c1:

L_49c7:
    if ((lppl->fStarbase == 0x0))
        goto L_49ff;
    else
        goto L_49de;

L_49de:
    if ((lppl->iPlayer != -1))
        goto L_49ff;
    else
        goto L_49eb;

L_49eb:
    lppl->fStarbase = 0x0;

L_49ff:
    i = Random(0x3);
    if (((uint16_t)(rgplr[lppl->iPlayer].rgEnvVar[i]) == -1))
        goto L_4b5c;
    else
        goto L_4a32;

L_4a32:
    if (((uint16_t)(rgplr[lppl->iPlayer].rgEnvVar[i]) == (uint16_t)(lppl->rgEnvVarOrig[i])))
        goto L_4b5c;
    else
        goto L_4a73;

L_4a73:
    if ((Random(0xa) != 0))
        goto L_4b5c;
    else
        goto L_4a87;

L_4a87:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_4aca;
    else
        goto L_4a94;

L_4a94:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_4aa4;
    else
        goto L_4a99;

L_4a99:
    if ((LOWORD(lppl->rgwtMin[0x3]) >= 0x3e8))
        goto L_4aca;
    else
        goto L_4aa4;

L_4aa4:
    if ((Random(0x3e8) >= LOWORD(lppl->rgwtMin[0x3])))
        goto L_4b5c;
    else
        goto L_4aca;

L_4aca:
    if (((uint16_t)(rgplr[lppl->iPlayer].rgEnvVar[i]) >= (uint16_t)(lppl->rgEnvVarOrig[i])))
        goto L_4b24;
    else
        goto L_4b0b;

L_4b0b:
    lppl->rgEnvVarOrig[i] = (lppl->rgEnvVarOrig[i] - 1);
    goto L_4b3a;

L_4b24:
    lppl->rgEnvVarOrig[i] = (lppl->rgEnvVarOrig[i] + 1);

L_4b3a:
    FSendPlrMsg2(lppl->iPlayer, 0x15c, lppl->id, lppl->id, i);

L_4b5c:
    if ((FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, 0x1) == 0))
        goto L_4c3d;
    else
        goto L_4b7f;

L_4b7f:

L_4b85:
    i = 0;
    goto L_4bfa;

L_4b8d:
    if ((rgMin[i] == -1))
        goto L_4bc3;
    else
        goto L_4b9f;

L_4b9f:
    lppl->rgEnvVar[i] = LOBYTE(rgMin[i]);
    goto L_4bf6;

L_4bc3:
    if ((rgMax[i] == -1))
        goto L_4bf6;
    else
        goto L_4bd5;

L_4bd5:
    lppl->rgEnvVar[i] = LOBYTE(rgMax[i]);

L_4bf6:
    i = (i + 1);

L_4bfa:
    if ((i < 3))
        goto L_4b8d;
    else
        goto L_4c03;

L_4c03:
    i = PctPlanetDesirability(lppl, lppl->iPlayer);
    FSendPlrMsg2(lppl->iPlayer, 0x156, lppl->id, lppl->id, i);

L_4c3d:
    lppl = (lppl + 0x1);

L_4c41:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_49a1;
    else
        goto L_4c4f;

L_4c4f:
    return;
}

void RemoteTerraforming() {
    int16_t  fHelp;
    int16_t  iBest;
    int16_t  pctCur;
    PLANET  *lppl;
    int16_t  ifl;
    FLEET   *lpfl;
    int16_t  cDone;
    int16_t  iEnv;
    int16_t  cAllowed;
    int32_t  ipct;
    int16_t  pctNew;
    uint16_t t_merge_4d92_0001;
    uint16_t t_4dd8;
    uint16_t t_merge_4e42_0001;
    uint16_t t_merge_4e79_0001;
    uint16_t t_merge_4e9d_0001;
    uint16_t t_merge_4f28_0001;
    uint16_t t_merge_4f91_0001;

L_4c56:
    ifl = 0;
    goto L_4c6b;

L_4c67:
    ifl = (ifl + 1);

L_4c6b:
    if ((ifl >= cFleet))
        goto L_4fa4;
    else
        goto L_4c76;

L_4c76:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_4ca6;
    else
        goto L_4c9e;

L_4c9e:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_4fa4;
    else
        goto L_4ca6;

L_4ca6:
    if ((lpfl->fDead != 0x0))
        goto L_4c67;
    else
        goto L_4cbd;

L_4cbd:
    if ((lpfl->idPlanet == -1))
        goto L_4c67;
    else
        goto L_4cca;

L_4cca:
    if ((lpPlanets[lpfl->idPlanet].iPlayer == -1))
        goto L_4c67;
    else
        goto L_4ce7;

L_4ce7:

L_4ced:
    ipct = PctTerraFromLpfl(lpfl);
    if ((HIWORD(ipct) > 0x0))
        goto L_4d1b;
    else
        goto L_4d0a;

L_4d0a:
    if ((HIWORD(ipct) < 0x0))
        goto L_4c67;
    else
        goto L_4d0f;

L_4d0f:
    if ((LOWORD(ipct) <= 0x0))
        goto L_4c67;
    else
        goto L_4d15;

L_4d15:

L_4d1b:
    lppl = &(lpPlanets[lpfl->idPlanet]);
    if ((lpfl->iPlayer == lppl->iPlayer))
        goto L_4d89;
    else
        goto L_4d48;

L_4d48:
    /* untranslated: branch sext8to16(byte ds:[(rgplr[lpfl->iPlayer].rgmdRelation + lppl->iPlayer)]) == 0x1 ? L_4d89 : L_4d76 */

L_4d76:
    if ((lpfl->iPlayer != lppl->iPlayer))
        goto L_4d8f;
    else
        goto L_4d89;

L_4d89:
    t_merge_4d92_0001 = 0x1;
    goto L_4d92;

L_4d8f:
    t_merge_4d92_0001 = 0x0;

L_4d92:
    fHelp = t_merge_4d92_0001;
    if ((fHelp != 0))
        goto L_4db8;
    else
        goto L_4d9e;

L_4d9e:
    if ((lppl->fStarbase != 0x0))
        goto L_4c67;
    else
        goto L_4db2;

L_4db2:

L_4db8:
    pctCur = PctPlanetDesirability(lppl, lppl->iPlayer);
    cDone = 0;

L_4dd5:
    LOWORD(ipct) = (LOWORD(ipct) - 0x1);
    t_4dd8 = HIWORD(ipct);
    HIWORD(ipct) = (HIWORD(ipct) - 0x0);
    if ((t_4dd8 < 0x0))
        goto L_4ebf;
    else
        goto L_4deb;

L_4deb:
    if ((HIWORD(ipct) > 0x0))
        goto L_4df8;
    else
        goto L_4df0;

L_4df0:
    if ((LOWORD(ipct) <= 0x0))
        goto L_4ebf;
    else
        goto L_4df8;

L_4df8:
    iBest = IBestRemoteTerra(lppl, lpfl->iPlayer, fHelp);
    if ((iBest == 0))
        goto L_4ebf;
    else
        goto L_4e19;

L_4e19:

L_4e1f:
    iEnv = (abs(iBest) - 1);
    if ((iBest <= 0))
        goto L_4e3f;
    else
        goto L_4e39;

L_4e39:
    t_merge_4e42_0001 = 0x1;
    goto L_4e42;

L_4e3f:
    t_merge_4e42_0001 = 0xffff;

L_4e42:
    cAllowed = ((uint16_t)(lppl->rgEnvVar[iEnv]) + t_merge_4e42_0001);
    if ((99 >= cAllowed))
        goto L_4e76;
    else
        goto L_4e70;

L_4e70:
    t_merge_4e79_0001 = 0x63;
    goto L_4e79;

L_4e76:
    t_merge_4e79_0001 = cAllowed;

L_4e79:
    if ((0x1 <= t_merge_4e79_0001))
        goto L_4e89;
    else
        goto L_4e83;

L_4e83:
    t_merge_4e9d_0001 = 0x1;
    goto L_4e9d;

L_4e89:
    if ((99 >= cAllowed))
        goto L_4e9a;
    else
        goto L_4e94;

L_4e94:
    t_merge_4e9d_0001 = 0x63;
    goto L_4e9d;

L_4e9a:
    t_merge_4e9d_0001 = cAllowed;

L_4e9d:
    cAllowed = t_merge_4e9d_0001;
    lppl->rgEnvVar[iEnv] = LOBYTE(cAllowed);
    cDone = (cDone + 1);
    goto L_4dd5;

L_4ebf:
    pctNew = PctPlanetDesirability(lppl, lppl->iPlayer);
    if ((pctCur != pctNew))
        goto L_4f10;
    else
        goto L_4f0a;

L_4f0a:
    goto L_4f13;

L_4f10:

L_4f13:
    if ((fHelp == 0))
        goto L_4f25;
    else
        goto L_4f1f;

L_4f1f:
    t_merge_4f28_0001 = 0x12c;
    goto L_4f28;

L_4f25:
    t_merge_4f28_0001 = 0x15a;

L_4f28:
    /* untranslated: call FSendPlrMsg(lpfl->iPlayer, (t_merge_4f28_0001 + ss:[bp-0x20]), (lpfl->id | 0x8000), lpfl->id, lppl->id, pctCur, pctNew, 0x0, 0x0, 0x0)
     * -> callresult(int16_t) */
    if ((lpfl->iPlayer == lppl->iPlayer))
        goto L_4c67;
    else
        goto L_4f50;

L_4f50:
    if ((pctNew == pctCur))
        goto L_4c67;
    else
        goto L_4f5b;

L_4f5b:
    if ((fHelp == 0))
        goto L_4f8e;
    else
        goto L_4f88;

L_4f88:
    t_merge_4f91_0001 = 0x12c;
    goto L_4f91;

L_4f8e:
    t_merge_4f91_0001 = 0x15a;

L_4f91:
    FSendPlrMsg(lppl->iPlayer, t_merge_4f91_0001, lppl->id, lpfl->id, lppl->id, pctCur, pctNew, 0x0, 0x0, 0x0);

L_4fa4:
    return;
}

int16_t FQueueColonistDrop(FLEET *lpfl, PLANET *lppl, int32_t cColonists) {
    int16_t  iColDrop;
    COLDROP *lpcdT;

L_4faa:
    if ((HIWORD(cColonists) > 0x0))
        goto L_4fd0;
    else
        goto L_4fbc;

L_4fbc:
    if ((HIWORD(cColonists) < 0x0))
        goto L_4fca;
    else
        goto L_4fc1;

L_4fc1:
    if ((LOWORD(cColonists) > 0x0))
        goto L_4fd0;
    else
        goto L_4fca;

L_4fca:
    return 0x1;

L_4fd0:
    iColDrop = 0;
    lpcdT = lpcd;
    goto L_5013;

L_4fe5:
    if ((lpcdT->idFleetSrc != lpfl->id))
        goto L_500b;
    else
        goto L_4ff6;

L_4ff6:
    if ((lpcdT->idPlanetDst == lppl->id))
        goto L_501e;
    else
        goto L_5005;

L_5005:

L_500b:
    lpcdT = (lpcdT + 0x1);
    iColDrop = (iColDrop + 1);

L_5013:
    if ((iColDrop < cColDrop))
        goto L_4fe5;
    else
        goto L_501e;

L_501e:
    if ((iColDrop != cColDrop))
        goto L_5089;
    else
        goto L_5029;

L_5029:
    if ((cColDrop < 1000))
        goto L_503a;
    else
        goto L_5034;

L_5034:
    return 0x0;

L_503a:
    lpcdT->idFleetSrc = lpfl->id;
    lpcdT->idPlr = lpfl->iPlayer;
    lpcdT->idPlanetDst = lppl->id;
    lpcdT->cColonist = 0;
    lpcdT->fCanColonize = 0x1;
    cColDrop = (cColDrop + 1);

L_5089:
    lpcdT->cColonist = (lpcdT->cColonist + cColonists);
    return LOWORD(cColonists);
}

void UpdatePopulations() {
    int32_t  lPopChg;
    PLANET  *lppl;
    PLANET  *lpplMac;
    int32_t  lPopOld;
    int16_t  fMac;
    uint16_t t_merge_52a5_0001;
    uint16_t t_merge_52d8_0001;

L_50a0:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_5318;

L_50d4:
    if ((lppl->iPlayer == -1))
        goto NextPlanet;
    else
        goto L_50e1;

L_50e1:
    if ((LOWORD(lppl->rgwtMin[0x3]) != 0x0))
        goto L_50fb;
    else
        goto L_50ee;

L_50ee:
    if ((HIWORD(lppl->rgwtMin[0x3]) == 0x0))
        goto NextPlanet;
    else
        goto L_50f5;

L_50f5:

L_50fb:
    lPopChg = ChgPopFromPlanet(lppl, 0x1);
    if ((LOWORD(lPopChg) != 0x0))
        goto L_5128;
    else
        goto L_511c;

L_511c:
    if ((HIWORD(lPopChg) == 0x0))
        goto NextPlanet;
    else
        goto L_5122;

L_5122:

L_5128:
    if ((HIWORD(lPopChg) > 0x0))
        goto NextPlanet;
    else
        goto L_5131;

L_5131:
    if ((HIWORD(lPopChg) < 0x0))
        goto L_513f;
    else
        goto L_5136;

L_5136:
    if ((LOWORD(lPopChg) >= 0x0))
        goto NextPlanet;
    else
        goto L_513f;

L_513f:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto NextPlanet;
    else
        goto L_514c;

L_514c:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_515b;
    else
        goto L_5151;

L_5151:
    if ((LOWORD(lppl->rgwtMin[0x3]) <= 0x0))
        goto NextPlanet;
    else
        goto L_515b;

L_515b:
    lPopOld = (lppl->rgwtMin[3] - lPopChg);
    if ((PctPlanetDesirability(lppl, lppl->iPlayer) >= 0))
        goto L_51fb;
    else
        goto L_518f;

L_518f:
    FSendPlrMsg(lppl->iPlayer, 0x25, lppl->id, lppl->id, LOWORD(lPopOld), (LOWORD((uint32_t)((lPopOld >> 0x10))) & 0xffff), LOWORD(lppl->rgwtMin[0x3]),
                (LOWORD((uint32_t)((lppl->rgwtMin[3] >> 0x10))) & 0xffff), 0x0, 0x0);
    goto NextPlanet;

L_51fb:
    /* untranslated: call FSendPlrMsg(lppl->iPlayer, 0x26, lppl->id, lppl->id, (LOWORD(lPopChg) neg 0x0), (loword((uint32_t)(words((LOWORD(lPopChg) neg 0x0),
     * ((HIWORD(lPopChg) + 0x0) neg 0x0)) >> 0x10)) & 0xffff), 0x0, 0x0, 0x0, 0x0) -> callresult(int16_t) */

NextPlanet:
    if ((lppl->iPlayer == -1))
        goto L_52f9;
    else
        goto L_5261;

L_5261:
    if ((LOWORD(lppl->rgwtMin[0x3]) != 0x0))
        goto L_52f9;
    else
        goto L_526e;

L_526e:
    if ((HIWORD(lppl->rgwtMin[0x3]) != 0x0))
        goto L_52f9;
    else
        goto L_5278;

L_5278:
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh))
        goto L_52a2;
    else
        goto L_529c;

L_529c:
    t_merge_52a5_0001 = 0x1;
    goto L_52a5;

L_52a2:
    t_merge_52a5_0001 = 0x0;

L_52a5:
    fMac = t_merge_52a5_0001;
    if ((HIWORD(lPopChg) > 0x0))
        goto L_52d5;
    else
        goto L_52c1;

L_52c1:
    if ((HIWORD(lPopChg) < 0x0))
        goto L_52cf;
    else
        goto L_52c6;

L_52c6:
    if ((LOWORD(lPopChg) >= 0x0))
        goto L_52d5;
    else
        goto L_52cf;

L_52cf:
    t_merge_52d8_0001 = 0x23;
    goto L_52d8;

L_52d5:
    t_merge_52d8_0001 = 0x40;

L_52d8:
    FSendPlrMsg2(lppl->iPlayer, (t_merge_52d8_0001 + fMac), lppl->id, lppl->id, 0x0);
    UninhabitPlanet(lppl);

L_52f9:
    if ((lppl->iPlayer != -1))
        goto L_5314;
    else
        goto L_5306;

L_5306:
    UninhabitPlanet(lppl);

L_5314:
    lppl = (lppl + 0x1);

L_5318:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_50d4;
    else
        goto L_5326;

L_5326:
    return;
}

void UpdateGuesses() {
    PLANET  *lppl;
    float    pct;
    PLANET  *lpplMac;
    int32_t  l;
    uint16_t t_53da;

L_532c:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_5590;

L_5360:
    if ((LOWORD(lppl->rgwtMin[0x3]) != 0x0))
        goto L_5377;
    else
        goto L_536d;

L_536d:
    if ((HIWORD(lppl->rgwtMin[0x3]) == 0x0))
        goto L_5583;
    else
        goto L_5377;

L_5377:
    l = lppl->rgwtMin[3];
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh))
        goto L_53b9;
    else
        goto L_53ac;

L_53ac:
    l = 0;
    goto L_5456;

L_53b9:
    Random(LOWORD((int32_t)((l >> 0x2))));
    /* untranslated: ss:[bp-0x16] = callresult(int16_t) */
    /* untranslated: ss:[bp-0x14] = signhiword(callresult(int16_t)) */
    t_53da = LOWORD(l);
    /* untranslated: LOWORD(l) = (LOWORD(l) + ((loword((int32_t)(l >> 0x3)) neg 0x0) + ss:[bp-0x16])) */
    /* untranslated: HIWORD(l) = (HIWORD(l) + (((hiword((int32_t)(words(t_53da, HIWORD(l)) >> 0x3)) + 0x0) neg 0x0) + ss:[bp-0x14])) */
    l = (int32_t)((l >> 0x2));
    if ((HIWORD(l) < 0x0))
        goto L_5435;
    else
        goto L_5419;

L_5419:
    if ((HIWORD(l) > 0x0))
        goto L_5428;
    else
        goto L_541e;

L_541e:
    if ((LOWORD(l) <= 0xffa))
        goto L_5435;
    else
        goto L_5428;

L_5428:
    l = 4090;
    goto L_5456;

L_5435:
    if ((HIWORD(l) > 0x0))
        goto L_5456;
    else
        goto L_543e;

L_543e:
    if ((HIWORD(l) < 0x0))
        goto L_544c;
    else
        goto L_5443;

L_5443:
    if ((LOWORD(l) >= 0x1))
        goto L_5456;
    else
        goto L_544c;

L_544c:
    l = 1;

L_5456:
    lppl->uGuesses = ((lppl->uGuesses & 0xf000) | (LOWORD(l) & 0xfff));
    if ((lppl->cDefenses != 0x0))
        goto L_54b3;
    else
        goto L_5494;

L_5494:
    if ((0x0 != 0x0))
        goto L_54b3;
    else
        goto L_549c;

L_549c:
    lppl->uDefGuess = 0x0;
    goto L_558c;

L_54b3:
    CalcPctSurvive(lppl, &(pct), 0x0);
    l = ((100 - __ftol()) + 4);
    l = (int32_t)((l / 6));
    if ((HIWORD(l) > 0x0))
        goto L_5535;
    else
        goto L_551a;

L_551a:
    if ((HIWORD(l) < 0x0))
        goto L_5528;
    else
        goto L_551f;

L_551f:
    if ((LOWORD(l) >= 0x1))
        goto L_5535;
    else
        goto L_5528;

L_5528:
    l = 1;
    goto L_5556;

L_5535:
    if ((HIWORD(l) < 0x0))
        goto L_5556;
    else
        goto L_553e;

L_553e:
    if ((HIWORD(l) > 0x0))
        goto L_554c;
    else
        goto L_5543;

L_5543:
    if ((LOWORD(l) <= 0xf))
        goto L_5556;
    else
        goto L_554c;

L_554c:
    l = 15;

L_5556:
    lppl->uGuesses = (lppl->uPopGuess | ((LOWORD(l) & 0xf) << 0xc));

L_5583:
    lppl->uGuesses = 0x0;

L_558c:
    lppl = (lppl + 0x1);

L_5590:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_5360;
    else
        goto L_559e;

L_559e:
    return;
}

void MineMinerals() {
    int32_t rglQuan[3];
    PLANET *lppl;
    PLANET *lpplMac;

L_55a4:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_55fa;

L_55d8:
    EstMineralsMined(lppl, rglQuan, 0xffffffff, 0x1);
    lppl = (lppl + 0x1);

L_55fa:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_55d8;
    else
        goto L_5608;

L_5608:
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

L_560e:
    if ((Random(0x14) != 0))
        goto L_5ae6;
    else
        goto L_5628;

L_5628:

L_562e:
    lppl = &(lpPlanets[Random(cPlanet)]);
    if ((lppl->iPlayer == -1))
        goto L_5685;
    else
        goto L_565c;

L_565c:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_5685;
    else
        goto L_5669;

L_5669:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_5678;
    else
        goto L_566e;

L_566e:
    if ((LOWORD(lppl->rgwtMin[0x3]) <= 0x32))
        goto L_5685;
    else
        goto L_5678;

L_5678:
    if ((game.turn < 0x14))
        goto L_5ae6;
    else
        goto L_567f;

L_567f:

L_5685:
    if ((game.turn < 0xa))
        goto L_5ae6;
    else
        goto L_568c;

L_568c:

L_5692:
    iSize = Random(0x4);
    i = 0;
    goto L_56bc;

L_56a9:
    rgEnv[i] = i;
    i = (i + 1);

L_56bc:
    if ((i < 3))
        goto L_56a9;
    else
        goto L_56c5;

L_56c5:
    i = 0;
    goto L_5716;

L_56cd:
    j = Random(0x3);
    iT = rgEnv[i];
    rgEnv[i] = rgEnv[j];
    rgEnv[j] = iT;
    i = (i + 1);

L_5716:
    if ((i < 3))
        goto L_56cd;
    else
        goto L_571f;

L_571f:
    i = 0;
    goto L_575b;

L_5727:
    rgAffect[i] = i;
    rgQuan[i] = (uint32_t)((Random(0xfa) + 0x32));
    i = (i + 1);

L_575b:
    if ((i < 3))
        goto L_5727;
    else
        goto L_5764;

L_5764:
    i = 0;
    goto L_57bb;

L_576c:
    j = (Random((3 - i)) + i);
    iT = rgAffect[i];
    rgAffect[i] = rgAffect[j];
    rgAffect[j] = iT;
    i = (i + 1);

L_57bb:
    if ((i < 2))
        goto L_576c;
    else
        goto L_57c4;

L_57c4:
    i = 0;
    goto L_57d0;

L_57cc:
    i = (i + 1);

L_57d0:
    if ((i >= game.cPlayer))
        goto L_5846;
    else
        goto L_57db;

L_57db:
    if ((i != lppl->iPlayer))
        goto L_5831;
    else
        goto L_580b;

L_580b:
    if ((GetRaceStat(rgplr[i], rsMajorAdv) == raMacintosh))
        goto L_5831;
    else
        goto L_582b;

L_582b:
    t_merge_5834_0001 = 0x87;
    goto L_5834;

L_5831:
    t_merge_5834_0001 = 0x83;

L_5834:
    FSendPlrMsg(i, (t_merge_5834_0001 + iSize), lppl->id, lppl->id, rgEnv[0], rgEnv[1], rgEnv[2], 0x0, 0x0, 0x0);
    goto L_57cc;

L_5846:
    if ((lppl->iPlayer == -1))
        goto L_58ad;
    else
        goto L_5853;

L_5853:
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) == raMacintosh))
        goto L_58ad;
    else
        goto L_5877;

L_5877:
    lppl->rgwtMin[3] = (lppl->rgwtMin[3] - (int32_t)(((uint32_t)((lppl->rgwtMin[3] * (uint32_t)((LOWORD((0x14 * iSize)) + 0x19)))) / 0x64)));

L_58ad:
    i = 0;
    goto L_58b9;

L_58b5:
    i = (i + 1);

L_58b9:
    if ((i > iSize))
        goto L_597b;
    else
        goto L_58c4;

L_58c4:
    if ((i >= 3))
        goto L_597b;
    else
        goto L_58cd;

L_58cd:
    rgQuan[rgAffect[i]] = (rgQuan[rgAffect[i]] + (uint32_t)((Random(0x4268) + 0xbb8)));
    iConc = lppl->rgMinConc[rgAffect[i]];
    iConc = (iConc + (Random(0x32) + 50));
    if ((iSize != 3))
        goto L_5948;
    else
        goto L_5936;

L_5936:
    iConc = (iConc + (Random(0xf) + 15));

L_5948:
    if ((iConc <= 200))
        goto L_5957;
    else
        goto L_5952;

L_5952:
    iConc = 200;

L_5957:
    /* untranslated: lppl->rgMinConc[rgAffect[i]] = part[0:1](iConc) */
    goto L_58b5;

L_597b:
    i = 0;
    goto L_59bd;

L_5983:
    lppl->rgwtMin[i] = (lppl->rgwtMin[i] + (int32_t)((rgQuan[i] >> 0x4)));
    i = (i + 1);

L_59bd:
    if ((i < 3))
        goto L_5983;
    else
        goto L_59c6;

L_59c6:
    i = 0;
    goto L_59d2;

L_59ce:
    i = (i + 1);

L_59d2:
    if ((i >= 3))
        goto L_5ad8;
    else
        goto L_59db;

L_59db:
    if ((i > iSize))
        goto L_5ad8;
    else
        goto L_59e6;

L_59e6:
    iT = (Random(0x3) + 3);
    if ((iSize != 3))
        goto L_5a13;
    else
        goto L_5a01;

L_5a01:
    iT = (iT + (Random(0x3) + 3));

L_5a13:
    if ((Random(0x2) == 0))
        goto L_5a2f;
    else
        goto L_5a27;

L_5a27:
    /* untranslated: iT = (iT neg 0) */

L_5a2f:
    j = ((uint16_t)(lppl->rgEnvVar[i]) + iT);
    if ((j >= 1))
        goto L_5a5c;
    else
        goto L_5a54;

L_5a54:
    j = 1;
    goto L_5a6a;

L_5a5c:
    if ((j <= 99))
        goto L_5a6a;
    else
        goto L_5a65;

L_5a65:
    j = 99;

L_5a6a:
    /* untranslated: lppl->rgEnvVar[i] = part[0:1](j) */
    j = ((uint16_t)(lppl->rgEnvVarOrig[i]) + iT);
    if ((j >= 1))
        goto L_5aaf;
    else
        goto L_5aa7;

L_5aa7:
    j = 1;
    goto L_5abd;

L_5aaf:
    if ((j <= 99))
        goto L_5abd;
    else
        goto L_5ab8;

L_5ab8:
    j = 99;

L_5abd:
    /* untranslated: lppl->rgEnvVarOrig[i] = part[0:1](j) */
    goto L_59ce;

L_5ad8:
    TossNonAutoBuildItems(lppl);

L_5ae6:
    return;
}

void TossNonAutoBuildItems(PLANET *lppl) {
    int16_t iDst;
    int16_t iSrc;

L_5aec:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_5b0f;
    else
        goto L_5b02;

L_5b02:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_5c4e;
    else
        goto L_5b09;

L_5b09:

L_5b0f:
    iDst = 0;
    iSrc = 0;
    goto L_5b21;

L_5b1d:
    iSrc = (iSrc + 1);

L_5b21:
    if ((iSrc >= lppl->lpplprod->iprodMac))
        goto L_5c12;
    else
        goto L_5b37;

L_5b37:
    /* untranslated: branch (loword((uint32_t)(words(LOWORD(lppl->lpplprod[(iSrc + 1)]), HIWORD(lppl->lpplprod[iSrc].rgprod[0x0])) >> 0x11)) & 0x7) != 0x1 ?
     * L_5b1d : L_5b6f */

L_5b6f:
    if ((0x0 != 0x0))
        goto L_5b1d;
    else
        goto L_5b77;

L_5b77:
    if ((0x0 > 0x0))
        goto L_5b1d;
    else
        goto L_5baf;

L_5baf:
    if ((0x0 < 0x0))
        goto L_5bbc;
    else
        goto L_5bb4;

L_5bb4:
    /* untranslated: branch (loword((uint32_t)(words(LOWORD(lppl->lpplprod[(iSrc + 1)]), HIWORD(lppl->lpplprod[iSrc].rgprod[0x0])) >> 0xa)) & 0x7f) >= 0x7 ?
     * L_5b1d : L_5bbc */

L_5bbc:
    if ((iSrc <= iDst))
        goto L_5c0b;
    else
        goto L_5bc7;

L_5bc7:
    LOWORD(lppl->lpplprod[(iDst + 1)]) = LOWORD(lppl->lpplprod[(iSrc + 0x1)]);
    HIWORD(lppl->lpplprod[iDst].rgprod[0x0]) = HIWORD(lppl->lpplprod[iSrc].rgprod[0x0]);

L_5c0b:
    iDst = (iDst + 1);

L_5c12:
    if ((iDst <= 0))
        goto L_5c2c;
    else
        goto L_5c1b;

L_5c1b:
    /* untranslated: lppl->lpplprod->iprodMac = part[0:1](iDst) */
    goto L_5c4e;

L_5c2c:
    FreePl(lppl->lpplprod);
    lppl->lpplprod = 0x0;

L_5c4e:
    return;
}

void PlanetaryClimateChange() {
    int16_t iT;
    PLANET *lppl;
    int16_t i;
    int16_t j;

L_5c54:
    if ((Random(0x14) != 0))
        goto L_5e06;
    else
        goto L_5c6e;

L_5c6e:

L_5c74:
    lppl = &(lpPlanets[Random(cPlanet)]);
    if ((lppl->iPlayer == -1))
        goto L_5ccb;
    else
        goto L_5ca2;

L_5ca2:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_5ccb;
    else
        goto L_5caf;

L_5caf:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_5cbe;
    else
        goto L_5cb4;

L_5cb4:
    if ((LOWORD(lppl->rgwtMin[0x3]) <= 0x32))
        goto L_5ccb;
    else
        goto L_5cbe;

L_5cbe:
    if ((game.turn < 0x14))
        goto L_5e06;
    else
        goto L_5cc5;

L_5cc5:

L_5ccb:
    i = Random(0x3);
    if ((lppl->iPlayer == -1))
        goto L_5d09;
    else
        goto L_5ce7;

L_5ce7:
    FSendPlrMsg2(lppl->iPlayer, 0xfd, lppl->id, lppl->id, i);

L_5d09:
    iT = (Random(0x3) + 3);
    if ((iT != 3))
        goto L_5d36;
    else
        goto L_5d24;

L_5d24:
    iT = (iT + (Random(0x3) + 3));

L_5d36:
    if ((Random(0x2) == 0))
        goto L_5d52;
    else
        goto L_5d4a;

L_5d4a:
    /* untranslated: iT = (iT neg 0) */

L_5d52:
    j = ((uint16_t)(lppl->rgEnvVar[i]) + iT);
    if ((j >= 1))
        goto L_5d7f;
    else
        goto L_5d77;

L_5d77:
    j = 1;
    goto L_5d8d;

L_5d7f:
    if ((j <= 99))
        goto L_5d8d;
    else
        goto L_5d88;

L_5d88:
    j = 99;

L_5d8d:
    /* untranslated: lppl->rgEnvVar[i] = part[0:1](j) */
    j = ((uint16_t)(lppl->rgEnvVarOrig[i]) + iT);
    if ((j >= 1))
        goto L_5dd2;
    else
        goto L_5dca;

L_5dca:
    j = 1;
    goto L_5de0;

L_5dd2:
    if ((j <= 99))
        goto L_5de0;
    else
        goto L_5ddb;

L_5ddb:
    j = 99;

L_5de0:
    /* untranslated: lppl->rgEnvVarOrig[i] = part[0:1](j) */
    TossNonAutoBuildItems(lppl);

L_5e06:
    return;
}

void DiscoverNewMinerals() {
    PLANET *lppl;
    int16_t i;

L_5e0c:
    if ((Random((15 - game.mdSize)) != 0))
        goto L_5ef4;
    else
        goto L_5e2a;

L_5e2a:

L_5e30:
    lppl = &(lpPlanets[Random(cPlanet)]);
    if ((game.turn < 0xa))
        goto L_5ef4;
    else
        goto L_5e58;

L_5e58:

L_5e5e:
    i = Random(0x3);
    if ((lppl->iPlayer == -1))
        goto L_5eb0;
    else
        goto L_5e7a;

L_5e7a:
    FSendPlrMsg(lppl->iPlayer, 0xfe, lppl->id, lppl->id, i, 0x0, 0x0, 0x0, 0x0, 0x0);

L_5eb0:
    if ((lppl->rgMinConc[i] >= 0xb4))
        goto L_5ef4;
    else
        goto L_5ed0;

L_5ed0:
    lppl->rgMinConc[i] = (lppl->rgMinConc[i] + LOBYTE((Random(0xf) + 0x5)));

L_5ef4:
    return;
}

void MysteryTrader() {
    int16_t  iSrc;
    int16_t  cRand;
    int16_t  i;
    THING   *lpth;
    int16_t  grbitTrader;
    int16_t  rgC[4];
    uint16_t t_merge_6082_0001;
    uint16_t t_merge_60ba_0001;

L_5efa:
    if ((game.turn < 0x28))
        goto L_6251;
    else
        goto L_5f0a;

L_5f0a:

L_5f10:
    /* untranslated: branch (words(game.turn, 0x0) % 0x64) != 0x47 ? L_5f2a : L_5f22 */

L_5f22:
    cRand = 2;
    goto L_5f73;

L_5f2a:
    /* untranslated: branch (words(game.turn, 0x0) % 0x64) != 0x21 ? L_5f44 : L_5f3c */

L_5f3c:
    cRand = 3;
    goto L_5f73;

L_5f44:
    if (((game.turn & 0x7f) != 0x31))
        goto L_5f5a;
    else
        goto L_5f52;

L_5f52:
    cRand = 4;
    goto L_5f73;

L_5f5a:
    if (((game.turn & 0x1) != 0x0))
        goto L_6251;
    else
        goto L_5f65;

L_5f65:

L_5f6e:
    cRand = 7;

L_5f73:
    if ((Random(cRand) != 0))
        goto L_6251;
    else
        goto L_5f83;

L_5f83:

L_5f89:
    lpth = LpthNew(0x0, ithMysteryTrader);
    if ((LOWORD(lpth) != 0x0))
        goto L_5fb4;
    else
        goto L_5fa8;

L_5fa8:
    if ((HIWORD(lpth) == 0x0))
        goto L_6251;
    else
        goto L_5fae;

L_5fae:

L_5fb4:
    *(lpth + 0xa) = ((*(lpth + 0xa) & 0xfff0) | ((Random(0x5) + 0x8) & 0xf));
    i = 0;
    goto L_600d;

L_5fe7:
    rgC[i] = (Random((LOWORD((0x190 * game.mdSize)) + 0x169)) + 1020);
    i = (i + 2);

L_600d:
    if ((i < 4))
        goto L_5fe7;
    else
        goto L_6016;

L_6016:
    if ((Random(0x2) != 0))
        goto L_603f;
    else
        goto L_602a;

L_602a:
    rgC[1] = 1020;
    rgC[3] = (LOWORD((0x190 * game.mdSize)) + 1380);
    goto L_6051;

L_603f:
    rgC[1] = (LOWORD((0x190 * game.mdSize)) + 1380);
    rgC[3] = 1020;

L_6051:
    Random(0x2);
    /* untranslated: iSrc = callresult(int16_t) */
    /* untranslated: lpth->pt.x = rgC[callresult(int16_t)] */
    if ((iSrc != 0))
        goto L_607f;
    else
        goto L_6079;

L_6079:
    t_merge_6082_0001 = 0x1;
    goto L_6082;

L_607f:
    t_merge_6082_0001 = 0x0;

L_6082:
    lpth->pt.y = rgC[t_merge_6082_0001];
    lpth->tht.ptDest.x = rgC[(iSrc + 2)];
    if ((iSrc != 0))
        goto L_60b7;
    else
        goto L_60b1;

L_60b1:
    t_merge_60ba_0001 = 0x1;
    goto L_60ba;

L_60b7:
    t_merge_60ba_0001 = 0x0;

L_60ba:
    lpth->tht.ptDest.y = rgC[(t_merge_60ba_0001 + 2)];
    if ((game.turn >= 0x64))
        goto L_60df;
    else
        goto L_60d7;

L_60d7:
    cRand = 5;
    goto L_60f7;

L_60df:
    if ((game.turn >= 0xfa))
        goto L_60f2;
    else
        goto L_60ea;

L_60ea:
    cRand = 3;
    goto L_60f7;

L_60f2:
    cRand = 2;

L_60f7:
    if ((lpth->tht.iWarp > 0x9))
        goto L_6110;
    else
        goto L_6109;

L_6109:
    cRand = (cRand + 1);
    goto L_6126;

L_6110:
    if ((lpth->tht.iWarp < 0xb))
        goto L_6126;
    else
        goto L_6122;

L_6122:
    cRand = (cRand - 1);

L_6126:
    if ((Random(0xa) >= cRand))
        goto L_6166;
    else
        goto L_613a;

L_613a:
    if ((Random(0x6) != 0))
        goto L_615a;
    else
        goto L_614e;

L_614e:
    lpth->tht.grbitTrader = 0x1000;
    goto L_621a;

L_615a:
    lpth->tht.grbitTrader = 0x0;

L_6166:
    grbitTrader = (0x1 << Random(0xd));
    if ((grbitTrader == 64))
        goto L_61a3;
    else
        goto L_6185;

L_6185:
    if ((grbitTrader == 128))
        goto L_61a3;
    else
        goto L_618f;

L_618f:
    if ((grbitTrader == 1024))
        goto L_61a3;
    else
        goto L_6199;

L_6199:
    if ((grbitTrader != 2048))
        goto L_6210;
    else
        goto L_61a3;

L_61a3:
    grbitTrader = (0x1 << Random(0xd));
    if ((game.turn >= 0x78))
        goto L_61cd;
    else
        goto L_61c3;

L_61c3:
    if ((grbitTrader == 128))
        goto L_61f7;
    else
        goto L_61cd;

L_61cd:
    if ((game.turn >= 0x96))
        goto L_61e2;
    else
        goto L_61d8;

L_61d8:
    if ((grbitTrader == 1024))
        goto L_61f7;
    else
        goto L_61e2;

L_61e2:
    if ((game.turn >= 0xb4))
        goto L_6210;
    else
        goto L_61ed;

L_61ed:
    if ((grbitTrader != 2048))
        goto L_6210;
    else
        goto L_61f7;

L_61f7:
    if ((Random(0x2) == 0))
        goto L_6210;
    else
        goto L_620b;

L_620b:
    grbitTrader = 0;

L_6210:
    lpth->tht.grbitTrader = grbitTrader;

L_621a:
    i = 0;
    goto L_6226;

L_6222:
    i = (i + 1);

L_6226:
    if ((i >= game.cPlayer))
        goto L_6251;
    else
        goto L_6231;

L_6231:
    FSendPlrMsg2(i, 0x12b, 0xfffa, lpth->idFull, 0x0);
    goto L_6222;

L_6251:
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

L_6258:
    cDead = 0;
    cFirst = 0;
    iScoreMax = 0;
    lScore2nd = 0;
    lScoreTot = 0;
    gd.fGameOverMan = 0x0;
    memset(rgcCond, 0x0, 0x10);
    i = 0;
    goto L_62b0;

L_62ac:
    i = (i + 1);

L_62b0:
    if ((i >= game.cPlayer))
        goto L_688c;
    else
        goto L_62bb;

L_62bb:
    rglScore[i] = CalcPlayerScore(i, &(score));
    vlprgScoreX[i].score = score;
    vlprgScoreX[i].wWord = ((vlprgScoreX[i].wWord & 0xffe0) | (i & 0x1f));
    vlprgScoreX[i].wWord = ((vlprgScoreX[i].wWord & 0xffdf) | 0x20);
    vlprgScoreX[i].wWord = ((vlprgScoreX[i].wWord & 0xc03f) | 0x0);
    lScoreTot = (lScoreTot + rglScore[i]);
    if ((score.cPlanet != 0))
        goto L_6498;
    else
        goto L_63e0;

L_63e0:
    if ((score.rgcsh[0] != 0x0))
        goto L_6498;
    else
        goto L_63e9;

L_63e9:
    if ((score.rgcsh[1] != 0x0))
        goto L_6498;
    else
        goto L_63f2;

L_63f2:
    if ((score.rgcsh[2] != 0x0))
        goto L_6498;
    else
        goto L_63fb;

L_63fb:
    if (((rgplr[i].wFlags & 0x1) != 0x0))
        goto L_6498;
    else
        goto L_6414;

L_6414:
    rgplr[i].wFlags = ((rgplr[i].wFlags & 0xfffe) | 0x1);
    j = 0;
    goto L_644a;

L_6446:
    j = (j + 1);

L_644a:
    if ((j >= game.cPlayer))
        goto L_6498;
    else
        goto L_6455;

L_6455:
    if ((j == i))
        goto L_6446;
    else
        goto L_645d;

L_645d:

L_6463:
    FSendPrependedPlrMsg(j, 0xbb, 0xfffc, (i | 0x30), 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
    goto L_6446;

L_6498:
    if ((score.cPlanet < MulDiv(cPlanet, GetVCVal(&(game), 0x0, 0x0), 0x64)))
        goto L_6542;
    else
        goto L_64c2;

L_64c2:
    /* untranslated: ss:[bp-0x86] = ((vlprgScoreX[i].wWord | 0x40) & 0x3fc0) */
    vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xc03f);
    /* untranslated: vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord | ss:[bp-0x86]) */
    if ((GetVCCheck(&(game), 0x0) == 0))
        goto L_6542;
    else
        goto L_6537;

L_6537:
    rgcCond[i] = (rgcCond[i] + 0x1);

L_6542:
    /* untranslated: ss:[bp-0x88] = loword((int32_t)(words((score.rgcsh[0x2] & 0x1fff), 0x0) << ((score.rgcsh[0x2] >> 0xd) << 0x1))) */
    /* untranslated: ss:[bp-0x86] = hiword((int32_t)(words((score.rgcsh[0x2] & 0x1fff), 0x0) << ((score.rgcsh[0x2] >> 0xd) << 0x1))) */
    /* untranslated: branch ss:[bp-0x86] < signhiword(GetVCVal(&game, 0x6, 0x0)) ? L_6614 : L_6588 */

L_6588:
    /* untranslated: branch ss:[bp-0x86] > signhiword(callresult(int16_t)) ? L_6594 : L_658d */

L_658d:
    /* untranslated: branch ss:[bp-0x88] < callresult(int16_t) ? L_6614 : L_6594 */

L_6594:
    /* untranslated: ss:[bp-0x86] = ((vlprgScoreX[i].wWord | 0x800) & 0x3fc0) */
    vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xc03f);
    /* untranslated: vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord | ss:[bp-0x86]) */
    if ((GetVCCheck(&(game), 0x6) == 0))
        goto L_6614;
    else
        goto L_6609;

L_6609:
    rgcCond[i] = (rgcCond[i] + 0x1);

L_6614:
    if ((HIWORD(rglScore[i]) < SIGNHIWORD(GetVCVal(&(game), 0x3, 0x0))))
        goto L_66c9;
    else
        goto L_663d;

L_663d:
    /* untranslated: branch HIWORD(rglScore[i]) > signhiword(callresult(int16_t)) ? L_6649 : L_6642 */

L_6642:
    /* untranslated: branch LOWORD(rglScore[i]) < callresult(int16_t) ? L_66c9 : L_6649 */

L_6649:
    /* untranslated: ss:[bp-0x86] = ((vlprgScoreX[i].wWord | 0x100) & 0x3fc0) */
    vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xc03f);
    /* untranslated: vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord | ss:[bp-0x86]) */
    if ((GetVCCheck(&(game), 0x3) == 0))
        goto L_66c9;
    else
        goto L_66be;

L_66be:
    rgcCond[i] = (rgcCond[i] + 0x1);

L_66c9:
    c = 0;
    j = 0;
    goto L_6719;

L_66d6:
    /* untranslated: ss:[bp-0x86] = sext8to16(rgplr[i].rgTech[j]) */
    /* untranslated: branch ss:[bp-0x86] < GetVCVal(&game, 0x1, 0x0) ? L_6715 : L_6711 */

L_6711:
    c = (c + 1);

L_6715:
    j = (j + 1);

L_6719:
    if ((j < 6))
        goto L_66d6;
    else
        goto L_6722;

L_6722:
    if ((c < GetVCVal(&(game), 0x2, 0x0)))
        goto L_67be;
    else
        goto L_673e;

L_673e:
    /* untranslated: ss:[bp-0x86] = ((vlprgScoreX[i].wWord | 0x80) & 0x3fc0) */
    vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xc03f);
    /* untranslated: vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord | ss:[bp-0x86]) */
    if ((GetVCCheck(&(game), 0x1) == 0))
        goto L_67be;
    else
        goto L_67b3;

L_67b3:
    rgcCond[i] = (rgcCond[i] + 0x1);

L_67be:
    /* untranslated: ss:[bp-0x88] = loword((int32_t)(score.cResources / 0x3e8)) */
    /* untranslated: ss:[bp-0x86] = hiword((int32_t)(score.cResources / 0x3e8)) */
    /* untranslated: branch ss:[bp-0x86] < signhiword(GetVCVal(&game, 0x5, 0x0)) ? L_62ac : L_67fd */

L_67fd:
    /* untranslated: branch ss:[bp-0x86] > signhiword(callresult(int16_t)) ? L_6809 : L_6802 */

L_6802:
    /* untranslated: branch ss:[bp-0x88] < callresult(int16_t) ? L_62ac : L_6809 */

L_6809:
    /* untranslated: ss:[bp-0x86] = ((vlprgScoreX[i].wWord | 0x400) & 0x3fc0) */
    vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xc03f);
    /* untranslated: vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord | ss:[bp-0x86]) */
    if ((GetVCCheck(&(game), 0x5) == 0))
        goto L_62ac;
    else
        goto L_687e;

L_687e:
    rgcCond[i] = (rgcCond[i] + 0x1);

L_688c:
    if ((game.cPlayer != 1))
        goto L_68a3;
    else
        goto L_6896;

L_6896:
    *(vlprgScoreX + 0x2) = 0x1;
    goto L_6dc4;

L_68a3:
    i = 0;
    goto L_68af;

L_68ab:
    i = (i + 1);

L_68af:
    if ((i >= game.cPlayer))
        goto L_69a9;
    else
        goto L_68ba;

L_68ba:
    if (((rgplr[i].wFlags & 0x1) == 0x0))
        goto L_68d7;
    else
        goto L_68d3;

L_68d3:
    cDead = (cDead + 1);

L_68d7:
    rgplr[i].wScore = 0x1;
    j = 0;
    goto L_68f3;

L_68ef:
    j = (j + 1);

L_68f3:
    if ((j >= game.cPlayer))
        goto L_6941;
    else
        goto L_68fe;

L_68fe:
    if ((HIWORD(rglScore[j]) < HIWORD(rglScore[i])))
        goto L_68ef;
    else
        goto L_6923;

L_6923:
    if ((HIWORD(rglScore[j]) > HIWORD(rglScore[i])))
        goto L_692f;
    else
        goto L_6928;

L_6928:
    if ((LOWORD(rglScore[j]) <= LOWORD(rglScore[i])))
        goto L_68ef;
    else
        goto L_692f;

L_692f:
    rgplr[i].wScore = (rgplr[i].wScore + 0x1);

L_6941:
    if ((rgplr[i].wScore != 0x1))
        goto L_697b;
    else
        goto L_6955;

L_6955:
    iScoreMax = i;
    lScoreMax = rglScore[i];
    cFirst = (cFirst + 1);
    goto L_68ab;

L_697b:
    if ((rgplr[i].wScore != 0x2))
        goto L_68ab;
    else
        goto L_698f;

L_698f:
    lScore2nd = rglScore[i];

L_69a9:
    if ((cFirst <= 1))
        goto L_69c0;
    else
        goto L_69b2;

L_69b2:
    lScore2nd = lScoreMax;

L_69c0:
    i = 0;
    goto L_69cc;

L_69c8:
    i = (i + 1);

L_69cc:
    if ((i >= game.cPlayer))
        goto L_6a06;
    else
        goto L_69d7;

L_69d7:
    HIWORD(vlprgScoreX[i]) = rgplr[i].wScore;
    goto L_69c8;

L_6a06:
    if ((game.turn < GetVCVal(&(game), 0x7, 0x0)))
        goto L_6aac;
    else
        goto L_6a23;

L_6a23:
    if ((cFirst != 1))
        goto L_6aac;
    else
        goto L_6a2c;

L_6a2c:
    /* untranslated: ss:[bp-0x86] = ((vlprgScoreX[iScoreMax].wWord | 0x1000) & 0x3fc0) */
    vlprgScoreX[iScoreMax].wWord = (vlprgScoreX[iScoreMax].wWord & 0xc03f);
    /* untranslated: vlprgScoreX[iScoreMax].wWord = (vlprgScoreX[iScoreMax].wWord | ss:[bp-0x86]) */
    if ((GetVCCheck(&(game), 0x7) == 0))
        goto L_6aac;
    else
        goto L_6aa1;

L_6aa1:
    rgcCond[iScoreMax] = (rgcCond[iScoreMax] + 0x1);

L_6aac:
    if (((cDead + 1) < game.cPlayer))
        goto L_6b66;
    else
        goto L_6abb;

L_6abb:
    gd.fGameOverMan = 0x1;
    if (((rgplr[iScoreMax].wFlags & 0x1) != 0x0))
        goto L_6b0f;
    else
        goto L_6ae0;

L_6ae0:
    FSendPrependedPlrMsg(iScoreMax, 0xbc, 0xfffc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);

L_6b0f:
    i = 0;
    goto L_6b1b;

L_6b17:
    i = (i + 1);

L_6b1b:
    if ((i >= game.cPlayer))
        goto L_6dc4;
    else
        goto L_6b26;

L_6b26:
    if ((i == iScoreMax))
        goto L_6b17;
    else
        goto L_6b31;

L_6b31:
    FSendPrependedPlrMsg(i, 0xb8, 0xfffc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);

L_6b66:
    if ((HIWORD(lScoreMax) < HIWORD((int32_t)(((uint32_t)((lScore2nd * (uint32_t)((GetVCVal(&(game), 0x4, 0x0) + 100)))) / 0x64)))))
        goto L_6c31;
    else
        goto L_6ba3;

L_6ba3:
    /* untranslated: branch HIWORD(lScoreMax) > hiword((int32_t)((uint32_t)(lScore2nd * sext16to32((callresult(int16_t) + 100))) / 0x64)) ? L_6bb1 : L_6ba8 */

L_6ba8:
    /* untranslated: branch LOWORD(lScoreMax) < loword((int32_t)((uint32_t)(lScore2nd * sext16to32((callresult(int16_t) + 100))) / 0x64)) ? L_6c31 : L_6bb1 */

L_6bb1:
    /* untranslated: ss:[bp-0x86] = ((vlprgScoreX[iScoreMax].wWord | 0x200) & 0x3fc0) */
    vlprgScoreX[iScoreMax].wWord = (vlprgScoreX[iScoreMax].wWord & 0xc03f);
    /* untranslated: vlprgScoreX[iScoreMax].wWord = (vlprgScoreX[iScoreMax].wWord | ss:[bp-0x86]) */
    if ((GetVCCheck(&(game), 0x4) == 0))
        goto L_6c31;
    else
        goto L_6c26;

L_6c26:
    rgcCond[iScoreMax] = (rgcCond[iScoreMax] + 0x1);

L_6c31:
    if ((game.turn < GetVCVal(&(game), 0x9, 0x0)))
        goto L_6dc4;
    else
        goto L_6c4b;

L_6c4b:

L_6c51:
    wWinners = 0x0;
    j = GetVCVal(&(game), 0x8, 0x0);
    if ((j < 1))
        goto L_6cfb;
    else
        goto L_6c76;

L_6c76:
    i = (game.cPlayer - 1);
    goto L_6cdd;

L_6c82:
    wWinners = (wWinners * 0x2);
    if ((rgcCond[i] < j))
        goto L_6cd9;
    else
        goto L_6c9d;

L_6c9d:
    vlprgScoreX[i].wWord = ((vlprgScoreX[i].wWord & 0xbfff) | 0x4000);
    wWinners = (wWinners | 0x1);

L_6cd9:
    i = (i - 1);

L_6cdd:
    if ((i >= 0))
        goto L_6c82;
    else
        goto L_6ce6;

L_6ce6:
    if ((wWinners == 0x0))
        goto L_6cfb;
    else
        goto L_6cef;

L_6cef:
    gd.fGameOverMan = 0x1;

L_6cfb:
    if ((gd.fGameOverMan == 0x0))
        goto L_6dc4;
    else
        goto L_6d09;

L_6d09:
    i = 0;
    j = 1;
    goto L_6d29;

L_6d19:
    i = (i + 1);
    j = (j * 2);

L_6d29:
    if ((i >= game.cPlayer))
        goto L_6dc4;
    else
        goto L_6d34;

L_6d34:
    wWinners2 = wWinners;
    if (((rgplr[i].wFlags & 0x1) == 0x0))
        goto L_6d5b;
    else
        goto L_6d53;

L_6d53:
    imsg = 184;
    goto L_6d94;

L_6d5b:
    if (((j & wWinners) == 0x0))
        goto L_6d8f;
    else
        goto L_6d69;

L_6d69:
    if (((j ^ wWinners) == 0x0))
        goto L_6d87;
    else
        goto L_6d77;

L_6d77:
    imsg = 183;
    /* untranslated: wWinners2 = (wWinners2 & (j ~ 0x0)) */
    goto L_6d94;

L_6d87:
    imsg = 182;

L_6d8f:
    imsg = 181;

L_6d94:
    FSendPrependedPlrMsg(i, imsg, 0xfffc, wWinners2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
    goto L_6d19;

L_6dc4:
    return;
}

void CreateBackupDir() {
    char *pchT;

L_6dca:
    strcpy(szBackup, szBase);
    pchT = strrchr(szBackup, 0x5c);
    if ((pchT != 0x0))
        goto L_6e07;
    else
        goto L_6dff;

L_6dff:
    pchT = szBackup;
    goto L_6e0b;

L_6e07:
    pchT = (pchT + 0x1);

L_6e0b:
    *(pchT) = 0;
    if ((vcBackupDirs > 1))
        goto L_6e2d;
    else
        goto L_6e1b;

L_6e1b:
    strcpy(pchT, "backup");
    goto L_6e7a;

L_6e2d:
    if ((vcBackupDirs > 99))
        goto L_6e5a;
    else
        goto L_6e37;

L_6e37:
    /* untranslated: call _wsprintf(pchT, "backup%d", (words(game.turn, 0x0) % vcBackupDirs)) -> callresult(int16_t) */
    goto L_6e7a;

L_6e5a:
    /* untranslated: call _wsprintf(pchT, "backup.%03d", (words(game.turn, 0x0) % vcBackupDirs)) -> callresult(int16_t) */

L_6e7a:
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
    uint16_t t_merge_6fd7_0001;

L_6e9c:
    if ((lpth->thp.iDecayRate > 0x0))
        goto L_6ec2;
    else
        goto L_6ebc;

L_6ebc:
    return 0x0;

L_6ec2:
    goto L_6eef;

L_6ed4:
    iRate = 10;
    goto L_6f0a;

L_6edc:
    iRate = 25;
    goto L_6f0a;

L_6ee4:
    iRate = 50;
    goto L_6f0a;

L_6eef:
    if ((lpth->thp.iDecayRate == 0x1))
        goto L_6ed4;
    else
        goto L_6ef7;

L_6ef7:
    if ((lpth->thp.iDecayRate == 0x2))
        goto L_6edc;
    else
        goto L_6eff;

L_6eff:
    if ((lpth->thp.iDecayRate == 0x3))
        goto L_6ee4;
    else
        goto L_6f0a;

L_6f0a:
    if ((GetRaceStat(rgplr[lpth->iplr], rsMajorAdv) != raMassAccel))
        goto L_6f4b;
    else
        goto L_6f37;

L_6f37:
    iRate = ((uint32_t)(iRate) / 2);
    iRateMin = 0x5;
    goto L_6f50;

L_6f4b:
    iRateMin = 0xa;

L_6f50:
    lDecay = 0;
    i = 0;
    goto L_704f;

L_6f62:
    /* untranslated: branch HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))] == 0x0 ? L_704b : L_6f7f */

L_6f7f:
    /* untranslated: wDecay = loword((int32_t)(words(hiword((uint32_t)(words(hiword((uint32_t)(words(signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i *
     * 0x2))]), HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) * sext16to32(iRate))), loword((uint32_t)(words(signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8)
     * + (i * 0x2))]), HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) * sext16to32(iRate)))) * sext16to32(pctRate))),
     * loword((uint32_t)(words(hiword((uint32_t)(words(signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]), HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i *
     * 0x2))]) * sext16to32(iRate))), loword((uint32_t)(words(signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]), HIWORD(lpth):[((LOWORD(lpth) + 0x8)
     * + (i * 0x2))]) * sext16to32(iRate)))) * sext16to32(pctRate)))) / 0x2710)) */
    if ((iRateMin <= wDecay))
        goto L_6fd4;
    else
        goto L_6fce;

L_6fce:
    t_merge_6fd7_0001 = iRateMin;
    goto L_6fd7;

L_6fd4:
    t_merge_6fd7_0001 = wDecay;

L_6fd7:
    wDecay = t_merge_6fd7_0001;
    /* untranslated: branch HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))] > wDecay ? L_7013 : L_6ff9 */

L_6ff9:
    /* untranslated: wDecay = HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] */

L_7013:
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))] = (HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] - wDecay) */
    /* untranslated: LOWORD(lDecay) = (LOWORD(lDecay) + HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) */
    /* untranslated: HIWORD(lDecay) = (HIWORD(lDecay) + signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))])) */

L_704b:
    i = (i + 1);

L_704f:
    if ((i < 3))
        goto L_6f62;
    else
        goto L_7058;

L_7058:
    if ((LOWORD(lDecay) != 0x0))
        goto L_7081;
    else
        goto L_7061;

L_7061:
    if ((HIWORD(lDecay) != 0x0))
        goto L_7081;
    else
        goto L_706a;

L_706a:
    FreeLpth(lpth);
    return 0x1;

L_7081:
    /* untranslated: *(lpth+0xe) = ((*(lpth+0xe) & 0xc000) | (loword((int32_t)(words((HIWORD(lDecay) + 0x0), (LOWORD(lDecay) + 0x9)) / 0xa)) & 0x3fff)) */
    return 0x0;
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
    uint16_t t_merge_722c_0001;
    uint16_t t_merge_737c_0001;
    uint16_t t_merge_74cc_0001;
    uint32_t t_merge_75cf_0001_wide;

L_70c6:
    ifl = 0;
    goto L_70db;

L_70d7:
    ifl = (ifl + 1);

L_70db:
    if ((ifl >= cFleet))
        goto L_712d;
    else
        goto L_70e6;

L_70e6:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_7116;
    else
        goto L_710e;

L_710e:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_712d;
    else
        goto L_7116;

L_7116:
    lpfl->fBombed = 0x0;
    goto L_70d7;

L_712d:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_7623;

L_7158:
    if ((lpth->ith != ithMineralPacket))
        goto L_7330;
    else
        goto L_716e;

L_716e:
    if ((lpth->thp.iWarp != 0x0))
        goto L_7310;
    else
        goto L_7185;

L_7185:
    if ((lpth->thp.fMoved == 0x0))
        goto L_71b3;
    else
        goto L_719c;

L_719c:
    lpth->thp.fMoved = 0x0;
    goto L_761f;

L_71b3:
    lDecay = 0;
    i = 0;
    goto L_72a1;

L_71c5:
    /* untranslated: branch HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))] == 0x0 ? L_729d : L_71e2 */

L_71e2:
    /* untranslated: branch 0xa <= (words(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))], signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))])) / 0xa) ?
     * L_720f : L_7209 */

L_7209:
    t_merge_722c_0001 = 0xa;
    goto L_722c;

L_720f:
    /* untranslated: t_merge_722c_0001 = (words(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))], signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i *
     * 0x2))])) / 0xa) */

L_722c:
    wDecay = t_merge_722c_0001;
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))] = (HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] - wDecay) */
    /* untranslated: branch HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))] >= 0x0 ? L_727f : L_7266 */

L_7266:
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 2))] = 0x0 */

L_727f:
    /* untranslated: LOWORD(lDecay) = (LOWORD(lDecay) + HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) */
    /* untranslated: HIWORD(lDecay) = (HIWORD(lDecay) + signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))])) */

L_729d:
    i = (i + 1);

L_72a1:
    if ((i < 3))
        goto L_71c5;
    else
        goto L_72aa;

L_72aa:
    if ((LOWORD(lDecay) != 0x0))
        goto L_72d5;
    else
        goto L_72b3;

L_72b3:
    if ((HIWORD(lDecay) != 0x0))
        goto L_72d5;
    else
        goto L_72bc;

L_72bc:
    FreeLpth(lpth);

LFixUpLpth:
    lpth = (lpth - 0x1);
    lpthMac = (lpthMac - 0x1);
    goto L_761f;

L_72d5:
    /* untranslated: *(lpth+0xe) = ((*(lpth+0xe) & 0xc000) | (loword((int32_t)(words((HIWORD(lDecay) + 0x0), (LOWORD(lDecay) + 0x9)) / 0xa)) & 0x3fff)) */

L_7310:
    if ((FPacketDecay(lpth, 0x64) != 0))
        goto LFixUpLpth;
    else
        goto L_7327;

L_7327:

L_7330:
    if ((lpth->ith != ithMinefield))
        goto L_761f;
    else
        goto L_7346;

L_7346:
    if ((GetRaceStat(rgplr[lpth->iplr], rsMajorAdv) != raMines))
        goto L_7379;
    else
        goto L_7373;

L_7373:
    t_merge_737c_0001 = 0x1;
    goto L_737c;

L_7379:
    t_merge_737c_0001 = 0x0;

L_737c:
    fMineExpert = t_merge_737c_0001;
    if ((lpth->thm.fDetonate == 0x0))
        goto L_74ba;
    else
        goto L_7391;

L_7391:
    LOWORD(lDecay) = LOWORD(lpth->thm);
    HIWORD(lDecay) = HIWORD(lpth->thm.cMines);
    ifl = 0;
    goto L_73ae;

L_73aa:
    ifl = (ifl + 1);

L_73ae:
    if ((ifl >= cFleet))
        goto L_74ba;
    else
        goto L_73b9;

L_73b9:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_73e9;
    else
        goto L_73e1;

L_73e1:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_74ba;
    else
        goto L_73e9;

L_73e9:
    if ((lpfl->fDead != 0x0))
        goto L_73aa;
    else
        goto L_73fd;

L_73fd:

L_7403:
    dx = (uint32_t)((lpfl->pt.x - lpth->pt.x));
    dy = (uint32_t)((lpfl->pt.y - lpth->pt.y));
    if ((lpfl->fBombed != 0x0))
        goto L_73aa;
    else
        goto L_7444;

L_7444:
    if (((HIWORD((uint32_t)((dx * dx))) + HIWORD((uint32_t)((dy * dy)))) > HIWORD(lDecay)))
        goto L_73aa;
    else
        goto L_747e;

L_747e:
    /* untranslated: branch (hiword((uint32_t)(dx * dx)) + ss:[bp-0x28]) < HIWORD(lDecay) ? L_748b : L_7483 */

L_7483:
    /* untranslated: branch (loword((uint32_t)(dx * dx)) + ss:[bp-0x2a]) > LOWORD(lDecay) ? L_73aa : L_748b */

L_748b:
    FTravelThroughMineFields(lpfl, 0x0, lpth);
    lpfl->fBombed = 0x1;

L_74ba:
    if ((fMineExpert != 0))
        goto L_74c9;
    else
        goto L_74c3;

L_74c3:
    t_merge_74cc_0001 = 0x1;
    goto L_74cc;

L_74c9:
    t_merge_74cc_0001 = 0x0;

L_74cc:
    /* untranslated: ss:[bp-0x20] = (loword((t_merge_74cc_0001 * 0x3)) + 0x1) */
    /* untranslated: call CPlanetsInCircle(lpth->pt, words(HIWORD(lpth->thm.cMines), LOWORD(lpth->thm))) -> callresult(int16_t) */
    /* untranslated: LOWORD(pctDecay) = (loword((ss:[bp-0x20] * callresult(int16_t))) + 0x2) */
    /* untranslated: HIWORD(pctDecay) = signhiword((loword((ss:[bp-0x20] * callresult(int16_t))) + 0x2)) */
    if ((HIWORD(pctDecay) < 0x0))
        goto L_7527;
    else
        goto L_750f;

L_750f:
    if ((HIWORD(pctDecay) > 0x0))
        goto L_751d;
    else
        goto L_7514;

L_7514:
    if ((LOWORD(pctDecay) <= 0x32))
        goto L_7527;
    else
        goto L_751d;

L_751d:
    pctDecay = 50;

L_7527:
    if ((lpth->thm.fDetonate == 0x0))
        goto L_7541;
    else
        goto L_7539;

L_7539:
    pctDecay = (pctDecay + 25);

L_7541:
    /* untranslated: lDecay = (int32_t)((uint32_t)(words(HIWORD(lpth->thm.cMines), LOWORD(lpth->thm)) * pctDecay) / 0x64) */
    if ((HIWORD(lDecay) > HIWORD(pctDecay)))
        goto L_7593;
    else
        goto L_757a;

L_757a:
    if ((HIWORD(lDecay) < HIWORD(pctDecay)))
        goto L_7587;
    else
        goto L_757f;

L_757f:
    if ((LOWORD(lDecay) >= LOWORD(pctDecay)))
        goto L_7593;
    else
        goto L_7587;

L_7587:
    lDecay = pctDecay;

L_7593:
    if ((lpth->thm.iType == 0x2))
        goto L_75d5;
    else
        goto L_75a5;

L_75a5:
    if ((0x0 < HIWORD(lDecay)))
        goto L_75c9;
    else
        goto L_75b3;

L_75b3:
    if ((0x0 > HIWORD(lDecay)))
        goto L_75c0;
    else
        goto L_75b8;

L_75b8:
    if ((0xa <= LOWORD(lDecay)))
        goto L_75c9;
    else
        goto L_75c0;

L_75c0:
    t_merge_75cf_0001_wide = 0xa;
    goto L_75cf;

L_75c9:
    t_merge_75cf_0001_wide = lDecay;

L_75cf:
    lDecay = t_merge_75cf_0001_wide;

L_75d5:
    if ((HIWORD(lDecay) < HIWORD(lpth->thm.cMines)))
        goto L_760e;
    else
        goto L_75e8;

L_75e8:
    if ((HIWORD(lDecay) > HIWORD(lpth->thm.cMines)))
        goto L_75f5;
    else
        goto L_75ed;

L_75ed:
    if ((LOWORD(lDecay) < LOWORD(lpth->thm)))
        goto L_760e;
    else
        goto L_75f5;

L_75f5:
    FreeLpth(lpth);
    lpth = (lpth - 0x1);
    lpthMac = (lpthMac - 0x1);
    goto L_761f;

L_760e:
    LOWORD(lpth->thm) = (LOWORD(lpth->thm) - LOWORD(lDecay));
    HIWORD(lpth->thm.cMines) = (HIWORD(lpth->thm.cMines) - HIWORD(lDecay));

L_761f:
    lpth = (lpth + 0x1);

L_7623:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_7158;
    else
        goto L_7631;

L_7631:
    return;
}

void UnmarkMineFields() {
    THING *lpthMac;
    THING *lpth;

L_7638:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_768f;

L_766c:
    if ((lpth->ith != ithMinefield))
        goto L_768b;
    else
        goto L_7682;

L_7682:
    lpth->thm.grbitPlrNow = 0x0;

L_768b:
    lpth = (lpth + 0x1);

L_768f:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_766c;
    else
        goto L_769d;

L_769d:
    return;
}

void SweepForMines() {
    int16_t  iplr;
    THING   *lpthMac;
    POINT    pt;
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

L_76a4:
    ifl = 0;
    goto L_76b9;

L_76b5:
    ifl = (ifl + 1);

L_76b9:
    if ((ifl >= cFleet))
        goto L_7a5c;
    else
        goto L_76c4;

L_76c4:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_76f4;
    else
        goto L_76ec;

L_76ec:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_7a5c;
    else
        goto L_76f4;

L_76f4:
    cMine = CMineSweepFromLpfl(lpfl);
    if ((HIWORD(cMine) < 0x0))
        goto L_76b5;
    else
        goto L_7711;

L_7711:
    if ((HIWORD(cMine) > 0x0))
        goto L_771f;
    else
        goto L_7716;

L_7716:
    if ((LOWORD(cMine) <= 0x0))
        goto L_76b5;
    else
        goto L_771f;

L_771f:
    if ((lpfl->fDead != 0x0))
        goto L_76b5;
    else
        goto L_7733;

L_7733:

L_7739:
    iplr = lpfl->iplr;
    grbitPlr = (0x1 << lpfl->iplr);
    pt.x = lpfl->pt.x;
    pt.y = lpfl->pt.y;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_7a4b;

L_7790:
    if ((lpth->ith != ithMinefield))
        goto L_7a47;
    else
        goto L_77a6;

L_77a6:
    if ((lpth->iplr == iplr))
        goto L_7a47;
    else
        goto L_77bc;

L_77bc:
    if ((FAttackPlayer(lpfl, lpth->iplr) == 0))
        goto L_7a47;
    else
        goto L_77de;

L_77de:

L_77e4:
    dx = (uint32_t)((pt.x - lpth->pt.x));
    dy = (uint32_t)((pt.y - lpth->pt.y));
    lCur = ((uint32_t)((dx * dx)) + (uint32_t)((dy * dy)));
    /* untranslated: branch HIWORD(lpth->thm.cMines) < (hiword((uint32_t)(dx * dx)) + ss:[bp-0x36]) ? L_7a47 : L_784a */

L_784a:
    /* untranslated: branch HIWORD(lpth->thm.cMines) > (hiword((uint32_t)(dx * dx)) + ss:[bp-0x36]) ? L_7858 : L_784f */

L_784f:
    /* untranslated: branch LOWORD(lpth->thm) < (loword((uint32_t)(dx * dx)) + ss:[bp-0x38]) ? L_7a47 : L_7858 */

L_7858:
    if ((lpth->thm.iType != 0x2))
        goto L_7886;
    else
        goto L_786a;

L_786a:
    cMineCur = (int32_t)((cMine / 3));
    goto L_7892;

L_7886:
    cMineCur = cMine;

L_7892:
    if ((HIWORD(cMineCur) > 0x0))
        goto L_78b3;
    else
        goto L_789b;

L_789b:
    if ((HIWORD(cMineCur) < 0x0))
        goto L_78a9;
    else
        goto L_78a0;

L_78a0:
    if ((LOWORD(cMineCur) >= 0x2))
        goto L_78b3;
    else
        goto L_78a9;

L_78a9:
    cMineCur = 2;

L_78b3:
    if (((HIWORD(lpth->thm.cMines) - HIWORD(cMineCur)) > (HIWORD(lCur) + 0xffff)))
        goto L_7900;
    else
        goto L_78d7;

L_78d7:
    if (((HIWORD(lpth->thm.cMines) - HIWORD(cMineCur)) < (HIWORD(lCur) + 0xffff)))
        goto L_78e3;
    else
        goto L_78dc;

L_78dc:
    if (((LOWORD(lpth->thm) - LOWORD(cMineCur)) >= (LOWORD(lCur) + 0xffff)))
        goto L_7900;
    else
        goto L_78e3;

L_78e3:
    LOWORD(cMineCur) = ((LOWORD(lpth->thm) - LOWORD(lCur)) + 0x1);
    HIWORD(cMineCur) = ((HIWORD(lpth->thm.cMines) - HIWORD(lCur)) + 0x0);

L_7900:
    if ((HIWORD(cMineCur) < HIWORD(lpth->thm.cMines)))
        goto L_7931;
    else
        goto L_7913;

L_7913:
    if ((HIWORD(cMineCur) > HIWORD(lpth->thm.cMines)))
        goto L_7920;
    else
        goto L_7918;

L_7918:
    if ((LOWORD(cMineCur) <= LOWORD(lpth->thm)))
        goto L_7931;
    else
        goto L_7920;

L_7920:
    LOWORD(cMineCur) = LOWORD(lpth->thm);
    HIWORD(cMineCur) = HIWORD(lpth->thm.cMines);

L_7931:
    FSendPlrMsg(lpfl->iPlayer, 0xc2, (0x8000 | lpfl->id), lpfl->id, LOWORD(cMineCur), (LOWORD((uint32_t)((cMineCur >> 0x10))) & 0xffff), lpth->iplr,
                lpth->thm.iType, lpth->pt.x, lpth->pt.y);
    FSendPlrMsg(lpth->iplr, 0xbe, 0xfffa, lpth->idFull, LOWORD(cMineCur), (LOWORD((uint32_t)((cMineCur >> 0x10))) & 0xffff), lpth->thm.iType, lpth->pt.x,
                lpth->pt.y, 0x0);
    LOWORD(lpth->thm) = (LOWORD(lpth->thm) - LOWORD(cMineCur));
    HIWORD(lpth->thm.cMines) = (HIWORD(lpth->thm.cMines) - HIWORD(cMineCur));
    if ((HIWORD(lpth->thm.cMines) > 0x0))
        goto L_7a34;
    else
        goto L_7a0c;

L_7a0c:
    if ((HIWORD(lpth->thm.cMines) < 0x0))
        goto L_7a1b;
    else
        goto L_7a11;

L_7a11:
    if ((LOWORD(lpth->thm) > 0x0))
        goto L_7a34;
    else
        goto L_7a1b;

L_7a1b:
    FreeLpth(lpth);
    lpth = (lpth - 0x1);
    lpthMac = (lpthMac - 0x1);
    goto L_7a47;

L_7a34:
    lpth->thm.grbitPlr = (lpth->thm.grbitPlr | (0x1 << lpfl->iPlayer));

L_7a47:
    lpth = (lpth + 0x1);

L_7a4b:
    if ((LOWORD(lpth) >= LOWORD(lpthMac)))
        goto L_76b5;
    else
        goto L_7a56;

L_7a56:

L_7a5c:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_7e3a;

L_7a87:
    if ((lppl->fStarbase == 0x0))
        goto L_7e36;
    else
        goto L_7a9e;

L_7a9e:
    if ((lppl->iPlayer == -1))
        goto L_7e36;
    else
        goto L_7aa8;

L_7aa8:

L_7aae:
    cMine = CMineSweepFromLphul(&(rglpshdefSB[lppl->iPlayer][lppl->isb]));
    if ((HIWORD(cMine) > 0x0))
        goto L_7afc;
    else
        goto L_7aeb;

L_7aeb:
    if ((HIWORD(cMine) < 0x0))
        goto L_7e36;
    else
        goto L_7af0;

L_7af0:
    if ((LOWORD(cMine) <= 0x0))
        goto L_7e36;
    else
        goto L_7af6;

L_7af6:

L_7afc:
    iplr = lppl->iPlayer;
    grbitPlr = (0x1 << lppl->iPlayer);
    pt.x = rgptPlan[lppl->id].x;
    pt.y = rgptPlan[lppl->id].y;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_7e28;

L_7b53:
    if ((lpth->ith != ithMinefield))
        goto L_7e24;
    else
        goto L_7b69;

L_7b69:
    if ((lpth->iplr == iplr))
        goto L_7e24;
    else
        goto L_7b7f;

L_7b7f:
    if ((iplr == lpth->iplr))
        goto L_7e24;
    else
        goto L_7b95;

L_7b95:
    /* untranslated: branch sext8to16(byte ds:[(rgplr[iplr].rgmdRelation + lpth->iplr)]) == 0x1 ? L_7e24 : L_7bc3 */

L_7bc3:

L_7bc9:
    dx = (uint32_t)((pt.x - lpth->pt.x));
    dy = (uint32_t)((pt.y - lpth->pt.y));
    lCur = ((uint32_t)((dx * dx)) + (uint32_t)((dy * dy)));
    /* untranslated: branch HIWORD(lpth->thm.cMines) < (hiword((uint32_t)(dx * dx)) + ss:[bp-0x36]) ? L_7e24 : L_7c2f */

L_7c2f:
    /* untranslated: branch HIWORD(lpth->thm.cMines) > (hiword((uint32_t)(dx * dx)) + ss:[bp-0x36]) ? L_7c3d : L_7c34 */

L_7c34:
    /* untranslated: branch LOWORD(lpth->thm) < (loword((uint32_t)(dx * dx)) + ss:[bp-0x38]) ? L_7e24 : L_7c3d */

L_7c3d:
    if ((lpth->thm.iType != 0x2))
        goto L_7c6b;
    else
        goto L_7c4f;

L_7c4f:
    cMineCur = (int32_t)((cMine / 3));
    goto L_7c77;

L_7c6b:
    cMineCur = cMine;

L_7c77:
    if ((HIWORD(cMineCur) > 0x0))
        goto L_7c98;
    else
        goto L_7c80;

L_7c80:
    if ((HIWORD(cMineCur) < 0x0))
        goto L_7c8e;
    else
        goto L_7c85;

L_7c85:
    if ((LOWORD(cMineCur) >= 0x2))
        goto L_7c98;
    else
        goto L_7c8e;

L_7c8e:
    cMineCur = 2;

L_7c98:
    if (((HIWORD(lpth->thm.cMines) - HIWORD(cMineCur)) > (HIWORD(lCur) + 0xffff)))
        goto L_7ce5;
    else
        goto L_7cbc;

L_7cbc:
    if (((HIWORD(lpth->thm.cMines) - HIWORD(cMineCur)) < (HIWORD(lCur) + 0xffff)))
        goto L_7cc8;
    else
        goto L_7cc1;

L_7cc1:
    if (((LOWORD(lpth->thm) - LOWORD(cMineCur)) >= (LOWORD(lCur) + 0xffff)))
        goto L_7ce5;
    else
        goto L_7cc8;

L_7cc8:
    LOWORD(cMineCur) = ((LOWORD(lpth->thm) - LOWORD(lCur)) + 0x1);
    HIWORD(cMineCur) = ((HIWORD(lpth->thm.cMines) - HIWORD(lCur)) + 0x0);

L_7ce5:
    if ((HIWORD(cMineCur) < HIWORD(lpth->thm.cMines)))
        goto L_7d16;
    else
        goto L_7cf8;

L_7cf8:
    if ((HIWORD(cMineCur) > HIWORD(lpth->thm.cMines)))
        goto L_7d05;
    else
        goto L_7cfd;

L_7cfd:
    if ((LOWORD(cMineCur) <= LOWORD(lpth->thm)))
        goto L_7d16;
    else
        goto L_7d05;

L_7d05:
    LOWORD(cMineCur) = LOWORD(lpth->thm);
    HIWORD(cMineCur) = HIWORD(lpth->thm.cMines);

L_7d16:
    FSendPlrMsg(iplr, 0xf4, lppl->id, lppl->id, LOWORD(cMineCur), (LOWORD((uint32_t)((cMineCur >> 0x10))) & 0xffff), lpth->iplr, lpth->thm.iType, lpth->pt.x,
                lpth->pt.y);
    FSendPlrMsg(lpth->iplr, 0xbe, 0xfffa, lpth->idFull, LOWORD(cMineCur), (LOWORD((uint32_t)((cMineCur >> 0x10))) & 0xffff), lpth->thm.iType, lpth->pt.x,
                lpth->pt.y, 0x0);
    LOWORD(lpth->thm) = (LOWORD(lpth->thm) - LOWORD(cMineCur));
    HIWORD(lpth->thm.cMines) = (HIWORD(lpth->thm.cMines) - HIWORD(cMineCur));
    if ((HIWORD(lpth->thm.cMines) > 0x0))
        goto L_7e11;
    else
        goto L_7de9;

L_7de9:
    if ((HIWORD(lpth->thm.cMines) < 0x0))
        goto L_7df8;
    else
        goto L_7dee;

L_7dee:
    if ((LOWORD(lpth->thm) > 0x0))
        goto L_7e11;
    else
        goto L_7df8;

L_7df8:
    FreeLpth(lpth);
    lpth = (lpth - 0x1);
    lpthMac = (lpthMac - 0x1);
    goto L_7e24;

L_7e11:
    lpth->thm.grbitPlr = (lpth->thm.grbitPlr | (0x1 << lppl->iPlayer));

L_7e24:
    lpth = (lpth + 0x1);

L_7e28:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_7b53;
    else
        goto L_7e36;

L_7e36:
    lppl = (lppl + 0x1);

L_7e3a:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_7a87;
    else
        goto L_7e48;

L_7e48:
    return;
}

void BreedColonistsInTransit() {
    int16_t  fNoBreeders;
    char     grfBreeder[16];
    int32_t  lColGain;
    PLANET  *lppl;
    int16_t  ifl;
    FLEET   *lpfl;
    int16_t  i;
    int32_t  lColGainAct;
    uint16_t t_merge_7e9a_0001;
    uint16_t t_merge_7e9a_0002;
    PLANET  *t_call_8058;

L_7e4e:
    fNoBreeders = 1;
    i = 0;
    goto L_7e68;

L_7e64:
    i = (i + 1);

L_7e68:
    if ((i >= game.cPlayer))
        goto L_7eb5;
    else
        goto L_7e73;

L_7e73:
    if ((GetRaceStat(rgplr[i], rsMajorAdv) != raDefend))
        goto L_7e98;
    else
        goto L_7e93;

L_7e93:
    /* untranslated: t_merge_7e9a_0001 = setlobyte(callresult(RaceAttribute), 0x1) */
    /* untranslated: t_merge_7e9a_0002 = setlobyte(callresult(RaceAttribute), 0x1) */
    goto L_7e9a;

L_7e98:
    /* untranslated: t_merge_7e9a_0001 = setlobyte(callresult(RaceAttribute), 0x0) */
    /* untranslated: t_merge_7e9a_0002 = setlobyte(callresult(RaceAttribute), 0x0) */

L_7e9a:
    grfBreeder[i] = LOBYTE(t_merge_7e9a_0001);
    if (((uint16_t)(LOBYTE(t_merge_7e9a_0002)) != 0x1))
        goto L_7e64;
    else
        goto L_7ead;

L_7ead:
    fNoBreeders = 0;

L_7eb5:
    if ((fNoBreeders != 0))
        goto L_80f7;
    else
        goto L_7ebb;

L_7ebb:

L_7ec1:
    ifl = 0;
    goto L_7ecd;

L_7ec9:
    ifl = (ifl + 1);

L_7ecd:
    if ((ifl >= cFleet))
        goto L_80f7;
    else
        goto L_7ed8;

L_7ed8:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_7f08;
    else
        goto L_7f00;

L_7f00:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_80f7;
    else
        goto L_7f08;

L_7f08:
    if ((lpfl->fDead != 0x0))
        goto L_7ec9;
    else
        goto L_7f1f;

L_7f1f:
    if (((uint16_t)(grfBreeder[lpfl->iPlayer]) == 0))
        goto L_7ec9;
    else
        goto L_7f36;

L_7f36:
    if ((LOWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_7f50;
    else
        goto L_7f43;

L_7f43:
    if ((HIWORD(lpfl->rgwtMin[0x3]) == 0x0))
        goto L_7ec9;
    else
        goto L_7f4a;

L_7f4a:

L_7f50:
    lColGain = (int32_t)(((uint32_t)((lpfl->rgwtMin[3] * (uint32_t)((uint16_t)(rgplr[lpfl->iPlayer].pctIdealGrowth)))) / 0xc8));
    if ((HIWORD(lColGain) > 0x0))
        goto L_7fc6;
    else
        goto L_7f94;

L_7f94:
    if ((HIWORD(lColGain) < 0x0))
        goto L_7fa2;
    else
        goto L_7f99;

L_7f99:
    if ((LOWORD(lColGain) > 0x0))
        goto L_7fc6;
    else
        goto L_7fa2;

L_7fa2:
    if ((Random(0x3) != 0))
        goto L_7ec9;
    else
        goto L_7fb6;

L_7fb6:
    lColGain = 1;
    goto L_7fc6;

L_7fc6:
    lColGainAct = ChgCargo(grobjFleet, lpfl->id, 0x3, lColGain, 0x0);
    if ((HIWORD(lColGainAct) < 0x0))
        goto L_8029;
    else
        goto L_7ff5;

L_7ff5:
    if ((HIWORD(lColGainAct) > 0x0))
        goto L_8003;
    else
        goto L_7ffa;

L_7ffa:
    if ((LOWORD(lColGainAct) <= 0x0))
        goto L_8029;
    else
        goto L_8003;

L_8003:
    FSendPlrMsg2(lpfl->iPlayer, 0xfb, (lpfl->id | 0x8000), lpfl->id, LOWORD(lColGainAct));

L_8029:
    if ((HIWORD(lColGainAct) > HIWORD(lColGain)))
        goto L_7ec9;
    else
        goto L_8037;

L_8037:
    if ((HIWORD(lColGainAct) < HIWORD(lColGain)))
        goto L_8044;
    else
        goto L_803c;

L_803c:
    if ((LOWORD(lColGainAct) >= LOWORD(lColGain)))
        goto L_7ec9;
    else
        goto L_8044;

L_8044:
    if ((lpfl->idPlanet == -1))
        goto L_7ec9;
    else
        goto L_8051;

L_8051:
    t_call_8058 = LpplFromId(lpfl->idPlanet);
    lppl = t_call_8058;
    if ((FAROFF(t_call_8058) != 0x0))
        goto L_8076;
    else
        goto L_806e;

L_806e:
    if ((FARSEG(t_call_8058) == 0x0))
        goto L_7ec9;
    else
        goto L_8076;

L_8076:
    if ((lppl->iPlayer != lpfl->iPlayer))
        goto L_7ec9;
    else
        goto L_8089;

L_8089:
    lColGain = (lColGain - lColGainAct);
    lppl->rgwtMin[3] = (lppl->rgwtMin[3] + lColGain);
    FSendPlrMsg(lpfl->iPlayer, 0x158, (lpfl->id | 0x8000), lpfl->id, LOWORD(lColGain), (LOWORD((uint32_t)((lColGain >> 0x10))) & 0xffff), lpfl->idPlanet, 0x0,
                0x0, 0x0);

L_80f7:
    return;
}

void UpdateResearchStatus(int16_t fUsePool) {
    int16_t   mdAvail;
    int16_t   fRedoItAll;
    int16_t   iTechCur;
    int16_t   fUsePoolOrig;
    int16_t   iTechNext;
    int16_t   iT;
    int16_t   iItem;
    int16_t   fGeneral;
    int16_t   fChgNow;
    int16_t   i;
    int16_t   ibitCur;
    int32_t   rglFieldSpent[6];
    int16_t   grbitCur;
    int16_t   cPlrAlive;
    int32_t   lSpent;
    PART      part;
    int32_t   l;
    int16_t   iTT;
    int32_t   l15pct;
    int16_t   iTechNext2;
    char      TechLevel;
    int16_t   jj;
    int16_t   iGoto;
    MessageId idm;
    uint16_t  t_merge_861e_0001;

L_80fe:
    cPlrAlive = 0;
    fUsePoolOrig = fUsePool;
    if ((fUsePool == 0))
        goto L_8145;
    else
        goto L_811b;

L_811b:
    i = 0;
    goto L_813c;

L_8123:
    rglFieldSpent[i] = 0;
    i = (i + 1);

L_813c:
    if ((i < 6))
        goto L_8123;
    else
        goto L_8145;

L_8145:
    i = 0;
    goto L_8151;

L_814d:
    i = (i + 1);

L_8151:
    if ((i >= game.cPlayer))
        goto L_8a0f;
    else
        goto L_815c;

L_815c:
    fUsePool = fUsePoolOrig;
    fGeneral = GetRaceGrbit(rgplr[i], ibitRaceGeneralizedResearch);
    iTechCur = ((uint16_t)(rgplr[i].iTechCur) & 0xf);
    iTechNext = (uint16_t)((rgplr[i].iTechCur >> 0x4));
    idPlayer = i;
    if (((rgplr[i].wFlags & 0x1) != 0x0))
        goto RedoItAll;
    else
        goto L_81cb;

L_81cb:
    cPlrAlive = (cPlrAlive + 1);

RedoItAll:
    fRedoItAll = 0;
    iT = 0;
    goto L_89f7;

L_81dc:
    lSpent = rgplr[i].rgResSpent[iT];
    fChgNow = 0;
    if ((game.fSlowTech == 0x0))
        goto L_8229;
    else
        goto L_8215;

L_8215:
    lSpent = (int32_t)((lSpent * 2));

L_8229:
    if ((iT != iTechCur))
        goto CheckForBreakthrough;
    else
        goto L_8234;

L_8234:
    if ((fUsePool == 0))
        goto CheckForBreakthrough;
    else
        goto L_823d;

L_823d:
    if ((fGeneral >= 2))
        goto CheckForBreakthrough;
    else
        goto L_8246;

L_8246:
    if ((fGeneral == 0))
        goto L_83a3;
    else
        goto L_824f;

L_824f:
    fRedoItAll = 1;
    fGeneral = 2;
    /* untranslated: lSpent = (lSpent + (int32_t)(words((HIWORD(rgplr[i].lResLastYear) + 0x0), (LOWORD(rgplr[i].lResLastYear) + 0x1)) / 0x2)) */
    /* untranslated: rglFieldSpent[iT] = (rglFieldSpent[iT] + (int32_t)(words((HIWORD(rgplr[i].lResLastYear) + 0x0), (LOWORD(rgplr[i].lResLastYear) + 0x1)) /
     * 0x2)) */
    iTT = 0;
    goto L_8397;

L_82c4:
    if ((iTT == iT))
        goto L_8393;
    else
        goto L_82cf;

L_82cf:
    /* untranslated: l15pct = (int32_t)(words((hiword((uint32_t)(rgplr[i].lResLastYear * 0x3)) + 0x0), (loword((uint32_t)(rgplr[i].lResLastYear * 0x3)) + 0x13))
     * / 0x14) */
    if ((game.fSlowTech == 0x0))
        goto L_8358;
    else
        goto L_8318;

L_8318:
    rgplr[i].rgResSpent[iTT] = (rgplr[i].rgResSpent[iTT] + (int32_t)((l15pct / 2)));
    goto L_837c;

L_8358:
    rgplr[i].rgResSpent[iTT] = (rgplr[i].rgResSpent[iTT] + l15pct);

L_837c:
    rglFieldSpent[iTT] = (rglFieldSpent[iTT] + l15pct);

L_8393:
    iTT = (iTT + 1);

L_8397:
    if ((iTT >= 6))
        goto CheckForBreakthrough;
    else
        goto L_839d;

L_839d:

L_83a3:
    lSpent = (lSpent + rgplr[i].lResLastYear);
    rglFieldSpent[iT] = (rglFieldSpent[iT] + rgplr[i].lResLastYear);

CheckForBreakthrough:
    if (((uint16_t)(rgplr[i].rgTech[iT]) >= 26))
        goto L_89f3;
    else
        goto L_83fc;

L_83fc:
    if ((((rgplr[i].wFlags >> 0x1) & 0x1) == 0x0))
        goto L_843a;
    else
        goto L_8417;

L_8417:
    if (((uint16_t)(rgplr[i].rgTech[iT]) >= 10))
        goto L_89f3;
    else
        goto L_8434;

L_8434:

L_843a:
    if ((((rgplr[i].wFlags >> 0x2) & 0x1) == 0x0))
        goto L_847a;
    else
        goto L_8457;

L_8457:
    if (((uint16_t)(rgplr[i].rgTech[iT]) >= 10))
        goto L_89f3;
    else
        goto L_8474;

L_8474:

L_847a:
    l = GetTechLevelCost(iT, ((uint16_t)(rgplr[i].rgTech[iT]) + 1), i);
    if ((HIWORD(l) > HIWORD(lSpent)))
        goto L_89a5;
    else
        goto L_84b8;

L_84b8:
    if ((HIWORD(l) < HIWORD(lSpent)))
        goto L_84c5;
    else
        goto L_84bd;

L_84bd:
    if ((LOWORD(l) > LOWORD(lSpent)))
        goto L_89a5;
    else
        goto L_84c5;

L_84c5:
    if (((uint16_t)(rgplr[i].rgTech[iT]) < 26))
        goto L_8520;
    else
        goto L_84e5;

L_84e5:
    if ((((rgplr[i].wFlags >> 0x1) & 0x1) != 0x0))
        goto L_89a5;
    else
        goto L_8500;

L_8500:
    if (((uint16_t)(rgplr[i].rgTech[iT]) >= 26))
        goto L_89a5;
    else
        goto L_8520;

L_8520:
    iTechNext2 = iTechCur;
    lSpent = (lSpent - l);
    rgplr[i].rgTech[iT] = (rgplr[i].rgTech[iT] + 1);
    TechLevel = rgplr[i].rgTech[iT];
    if (((uint16_t)(TechLevel) != 26))
        goto L_8569;
    else
        goto L_855b;

L_855b:
    if ((iTechNext != 6))
        goto L_8569;
    else
        goto L_8564;

L_8564:
    iTechNext = 7;

L_8569:
    if ((iTechCur != iT))
        goto L_85ed;
    else
        goto L_8574;

L_8574:
    if ((iTechNext == 6))
        goto L_85ed;
    else
        goto L_857d;

L_857d:
    if ((iTechNext == 7))
        goto L_858f;
    else
        goto L_8586;

L_8586:
    iTechNext2 = iTechNext;
    goto L_85e8;

L_858f:
    iTechNext2 = 0;
    jj = 1;
    goto L_85df;

L_859c:
    /* untranslated: branch rgplr[i].rgTech[jj] >= lobyte(setlobyte(iTechNext2, rgplr[i].rgTech[iTechNext2])) ? L_85db : L_85d5 */

L_85d5:
    iTechNext2 = jj;

L_85db:
    jj = (jj + 1);

L_85df:
    if ((jj < 6))
        goto L_859c;
    else
        goto L_85e8;

L_85e8:
    fChgNow = 1;

L_85ed:
    if ((fGeneral == 0))
        goto L_861b;
    else
        goto L_8615;

L_8615:
    t_merge_861e_0001 = 0x136;
    goto L_861e;

L_861b:
    t_merge_861e_0001 = 0x50;

L_861e:
    FSendPlrMsg(i, t_merge_861e_0001, 0xfffe, (uint16_t)(TechLevel), iT, iTechNext2, 0x0, 0x0, 0x0, 0x0);
    grbitCur = 1;
    ibitCur = 0;
    goto L_87d3;

L_8637:
    if (((grbitCur & 0xffff) == 0x0))
        goto L_87c9;
    else
        goto L_8645;

L_8645:
    iItem = 0;
    part.hs.grhst = grbitCur;

L_8653:
    part.hs.iItem = iItem;
    mdAvail = FLookupPart(&(part));
    if ((mdAvail == 0))
        goto L_87c9;
    else
        goto L_867a;

L_867a:

L_8680:
    if ((mdAvail != 1))
        goto L_87c2;
    else
        goto L_8689;

L_8689:
    /* untranslated: branch rgplr[i].rgTech[iT] != lobyte(setlobyte(iT, part.pcom->rgTech[iT])) ? L_87c2 : L_86c0 */

L_86c0:
    if ((grbitCur != 1024))
        goto L_86d7;
    else
        goto L_86ca;

L_86ca:
    idm = idmRecentBreakthroughHasAlsoGivenHullDesign;
    iGoto = -3;
    goto L_8798;

L_86d7:
    if ((grbitCur != 16384))
        goto L_86ee;
    else
        goto L_86e1;

L_86e1:
    idm = idmRecentBreakthroughHasAlsoGivenHullType;
    iGoto = -3;
    goto L_8798;

L_86ee:
    if ((grbitCur != 8192))
        goto L_873a;
    else
        goto L_86f8;

L_86f8:
    if ((GetRaceGrbit(rgplr[i], ibitRaceTT) == 0))
        goto L_873a;
    else
        goto L_8718;

L_8718:
    if ((iItem == 8))
        goto L_8733;
    else
        goto L_8721;

L_8721:
    if ((iItem == 12))
        goto L_8733;
    else
        goto L_872a;

L_872a:
    if ((iItem != 16))
        goto L_873a;
    else
        goto L_8733;

L_8733:
    iItem = (iItem + 1);
    goto L_8653;

L_873a:
    if ((grbitCur != -32768))
        goto L_875e;
    else
        goto L_8744;

L_8744:
    if ((iItem < 9))
        goto L_875e;
    else
        goto L_874d;

L_874d:
    if ((iItem > 13))
        goto L_875e;
    else
        goto L_8756;

L_8756:
    idm = idmRecentBreakthroughHasAlsoTaughtHowBuild;
    goto L_8787;

L_875e:
    if ((grbitCur != -32768))
        goto L_8782;
    else
        goto L_8768;

L_8768:
    if ((iItem < 0))
        goto L_8782;
    else
        goto L_8771;

L_8771:
    if ((iItem > 8))
        goto L_8782;
    else
        goto L_877a;

L_877a:
    idm = idmRecentBreakthroughHasAlsoTaughtHowBuild2;
    goto L_8787;

L_8782:
    idm = idmRecentBreakthroughHasAlsoGivenBenefit;

L_8787:
    iGoto = (((ibitCur << 0x8) | 0xc000) | iItem);

L_8798:
    FSendPlrMsg(i, idm, iGoto, iT, grbitCur, iItem, 0x0, 0x0, 0x0, 0x0);

L_87c2:
    iItem = (iItem + 1);

L_87c9:
    grbitCur = (grbitCur * 2);
    ibitCur = (ibitCur + 1);

L_87d3:
    if ((grbitCur != 0))
        goto L_8637;
    else
        goto L_87dc;

L_87dc:
    if ((fUsePool != 0))
        goto L_87f7;
    else
        goto L_87e5;

L_87e5:
    if ((fChgNow != 0))
        goto L_87f7;
    else
        goto L_87ee;

L_87ee:
    if ((iTechNext != 7))
        goto CheckForBreakthrough;
    else
        goto L_87f7;

L_87f7:
    if ((iTechNext == 6))
        goto CheckForBreakthrough;
    else
        goto L_8800;

L_8800:
    if ((iT != iTechCur))
        goto CheckForBreakthrough;
    else
        goto L_880b;

L_880b:
    if ((iTechNext != 7))
        goto L_88c6;
    else
        goto L_8814;

L_8814:
    iTechNext = 0;
    jj = 1;
    goto L_8864;

L_8821:
    /* untranslated: branch rgplr[i].rgTech[jj] >= lobyte(setlobyte(iTechNext, rgplr[i].rgTech[iTechNext])) ? L_8860 : L_885a */

L_885a:
    iTechNext = jj;

L_8860:
    jj = (jj + 1);

L_8864:
    if ((jj < 6))
        goto L_8821;
    else
        goto L_886d;

L_886d:
    /* untranslated: rgplr[idPlayer].iTechCur = lobyte(setlobyte(setlobyte(setlobyte(loword((0xc0 * idPlayer)), rgplr[idPlayer].iTechCur),
     * (rgplr[idPlayer].iTechCur & 0xf0)), ((rgplr[idPlayer].iTechCur & 0xf0) | part[0:1](iTechNext)))) */
    rgplr[i].rgResSpent[iT] = 0x0;
    iTechCur = iTechNext;
    iTechNext = 7;
    goto L_8944;

L_88c6:
    /* untranslated: rgplr[idPlayer].iTechCur = lobyte(setlobyte(setlobyte(setlobyte(loword((0xc0 * idPlayer)), rgplr[idPlayer].iTechCur),
     * (rgplr[idPlayer].iTechCur & 0xf)), ((rgplr[idPlayer].iTechCur & 0xf) | 0x60))) */
    /* untranslated: rgplr[idPlayer].iTechCur = lobyte(setlobyte(setlobyte(setlobyte(loword((0xc0 * idPlayer)), rgplr[idPlayer].iTechCur),
     * (rgplr[idPlayer].iTechCur & 0xf0)), ((rgplr[idPlayer].iTechCur & 0xf0) | part[0:1](iTechNext)))) */
    rgplr[i].rgResSpent[iT] = 0x0;
    iTechCur = iTechNext;
    iTechNext = 6;

L_8944:
    if ((game.fSlowTech == 0x0))
        goto L_896e;
    else
        goto L_8954;

L_8954:
    lSpent = (int32_t)(((lSpent + 0x1) >> 0x1));

L_896e:
    rgplr[i].rgResSpent[iTechCur] = (rgplr[i].rgResSpent[iTechCur] + lSpent);
    fUsePool = 0;
    fRedoItAll = 1;
    goto L_89f3;

L_89a5:
    if ((game.fSlowTech == 0x0))
        goto L_89cf;
    else
        goto L_89b5;

L_89b5:
    lSpent = (int32_t)(((lSpent + 0x1) >> 0x1));

L_89cf:
    rgplr[i].rgResSpent[iT] = lSpent;

L_89f3:
    iT = (iT + 1);

L_89f7:
    if ((iT < 6))
        goto L_81dc;
    else
        goto L_8a00;

L_8a00:
    if ((fRedoItAll != 0))
        goto RedoItAll;
    else
        goto L_8a06;

L_8a06:

L_8a0f:
    idPlayer = -1;
    fRedoItAll = 0;
    if ((fUsePoolOrig == 0))
        goto L_8b69;
    else
        goto L_8a23;

L_8a23:
    if ((cPlrAlive <= 1))
        goto L_8b69;
    else
        goto L_8a2c;

L_8a2c:
    i = 0;
    goto L_8a38;

L_8a34:
    i = (i + 1);

L_8a38:
    if ((i >= game.cPlayer))
        goto L_8b54;
    else
        goto L_8a43;

L_8a43:
    if ((GetRaceStat(rgplr[i], rsMajorAdv) != raStealth))
        goto L_8a34;
    else
        goto L_8a60;

L_8a60:

L_8a66:
    iT = 0;
    goto L_8b48;

L_8a6e:
    if ((HIWORD(rglFieldSpent[iT]) < 0x0))
        goto L_8b44;
    else
        goto L_8a83;

L_8a83:
    if ((HIWORD(rglFieldSpent[iT]) > 0x0))
        goto L_8a90;
    else
        goto L_8a88;

L_8a88:
    if ((LOWORD(rglFieldSpent[iT]) <= 0x0))
        goto L_8b44;
    else
        goto L_8a90;

L_8a90:
    lSpent = (int32_t)(((int32_t)((rglFieldSpent[iT] / (uint32_t)(cPlrAlive))) / 0x2));
    if ((HIWORD(lSpent) < 0x0))
        goto L_8b44;
    else
        goto L_8aca;

L_8aca:
    if ((HIWORD(lSpent) > 0x0))
        goto L_8ad8;
    else
        goto L_8acf;

L_8acf:
    if ((LOWORD(lSpent) <= 0x1))
        goto L_8b44;
    else
        goto L_8ad8;

L_8ad8:
    fRedoItAll = 1;
    FSendPlrMsg2(i, 0x159, 0xfffe, iT, LOWORD(lSpent));
    if ((game.fSlowTech == 0x0))
        goto L_8b20;
    else
        goto L_8b06;

L_8b06:
    lSpent = (int32_t)(((lSpent + 0x1) >> 0x1));

L_8b20:
    rgplr[i].rgResSpent[iT] = (rgplr[i].rgResSpent[iT] + lSpent);

L_8b44:
    iT = (iT + 1);

L_8b48:
    if ((iT >= 6))
        goto L_8a34;
    else
        goto L_8b4e;

L_8b4e:

L_8b54:
    if ((fRedoItAll == 0))
        goto L_8b69;
    else
        goto L_8b5d;

L_8b5d:
    UpdateResearchStatus(0x0);

L_8b69:
    return;
}

int16_t IBestRemoteTerra(PLANET *lppl, int16_t iplr, int16_t fHelp) {
    int16_t iBest;
    int16_t i;
    PLAYER  plrSav;

L_8b70:
    plrSav = rgplr[lppl->iPlayer];
    rgplr[lppl->iPlayer] = rgplr[iplr];
    i = 0;
    goto L_8c65;

L_8bd7:
    /* untranslated: rgplr[lppl->iPlayer].rgEnvVar[i] = lobyte(setlobyte(i, plrSav.rgEnvVar[i])) */
    /* untranslated: rgplr[lppl->iPlayer].rgEnvVarMin[i] = lobyte(setlobyte(i, plrSav.rgEnvVarMin[i])) */
    /* untranslated: rgplr[lppl->iPlayer].rgEnvVarMax[i] = lobyte(setlobyte(i, plrSav.rgEnvVarMax[i])) */
    i = (i + 1);

L_8c65:
    if ((i < 3))
        goto L_8bd7;
    else
        goto L_8c6e;

L_8c6e:
    iBest = IBestTerraform(lppl, fHelp);
    rgplr[lppl->iPlayer] = plrSav;

L_8cb5:
    return iBest;
}
