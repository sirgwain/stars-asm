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
    uint16_t  t_scratch_m16_3;
    int16_t   t_merge_2429_0001;
    uint16_t  t_scratch_m16_4;
    int16_t   t_merge_2552_0001;
    uint16_t  t_scratch_m16_5;
    int16_t   t_merge_2609_0001;
    uint16_t  t_merge_274b_0001;
    uint16_t  t_merge_2763_0001;
    int16_t   t_scratch_m16_6;
    int16_t   t_call_2d26;
    int16_t   t_scratch_m16_7;
    int16_t   t_2da8;
    uint16_t  t_merge_2df8_0001;
    uint16_t  t_scratch_m18;
    uint16_t  t_merge_2e2f_0001;
    uint16_t  t_merge_2e53_0001;
    int16_t   t_merge_2e9b_0001;

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
    lpshdef = (rglpshdefSB[lppl->iPlayer] + iItem);
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
    return 0;

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
    FSendPlrMsg(lppl->iPlayer, idm, lppl->id, lppl->id, ((lppl->iPlayer << 0x5) | (iItem + 16)), LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax, 0, 0, 0,
                0);
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
    lppl->isb = iItem;
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
    lppl->iWarpFling = ((iWarp + fTwoMAs) - 4);
    goto L_1c55;

L_1c1f:
    lppl->iWarpFling = 0x0;
    lppl->idFling = 0x0;
    KillQueuedMassPackets(lppl);

L_1c55:
    lpshdef->cBuilt = (lpshdef->cBuilt + 0x1);
    lpshdef->cExist = (lpshdef->cExist + 0x1);
    return 1;

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
    return 0;

L_1c9e:
    lpshdef = (rglpshdef[lppl->iPlayer] + iItem);
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
    FSendPlrMsg2(lppl->iPlayer, 79, lppl->id, (iItem + 1), 0);
    return 0;

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
    if ((HIWORD(rglpfl[i]) == 0x0))
        goto L_214f;
    else
        goto L_1d81;

L_1d81:
    if ((lpfl->iPlayer > lppl->iPlayer))
        goto L_214f;
    else
        goto L_1d97;

L_1d97:
    if ((lpfl->iPlayer < lppl->iPlayer))
        goto L_1d42;
    else
        goto L_1dad;

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
    if ((lpfl->rgdv[iItem].pctDp == 0x0))
        goto L_20c5;
    else
        goto L_1e53;

L_1e53:
    dpShdef = rglpshdef[lpfl->iPlayer][iItem].hul.dp;
    cshOrig = lpfl->rgcsh[iItem];
    cshDamaged = LOWORD((int32_t)(((uint32_t)((lpfl->rgdv[iItem].pctSh * (uint32_t)(cshOrig))) / 0x64)));
    if ((cshDamaged != 0))
        goto L_1edb;
    else
        goto L_1ed6;

L_1ed6:
    cshDamaged = 1;

L_1edb:
    dpOrig = (int32_t)(((uint32_t)(((int32_t)(((uint32_t)(((uint32_t)(dpShdef)*lpfl->rgdv[iItem].pctDp)) / 0xa)) * (uint32_t)(cshDamaged))) / 0x32));
    lpfl->rgdv[iItem].pctSh = LOWORD((int32_t)(((uint32_t)(((uint32_t)(cshDamaged) * 100)) / (uint32_t)((cshOrig + cBuilt)))));
    if ((lpfl->rgdv[iItem].pctSh != 0x0))
        goto L_1ff4;
    else
        goto L_1fc0;

L_1fc0:
    lpfl->rgdv[iItem].pctSh = 0x1;

L_1ff4:
    cshDamaged = LOWORD((int32_t)(((uint32_t)((lpfl->rgdv[iItem].pctSh * (uint32_t)((cshOrig + cBuilt)))) / 0x64)));
    if ((cshDamaged != 0))
        goto L_2041;
    else
        goto L_203c;

L_203c:
    cshDamaged = 1;

L_2041:
    lpfl->rgdv[iItem].pctDp = LOWORD((int32_t)(((uint32_t)(((int32_t)(((uint32_t)((dpOrig * 5)) / (uint32_t)(cshDamaged))) * 0x64)) / (uint32_t)(dpShdef))));
    goto L_20de;

L_20c5:
    lpfl->rgdv[iItem].dp = 0x0;

L_20de:
    CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
    FSendPlrMsg(lppl->iPlayer, 313, (lpfl->id | 0x8000), lppl->id, cBuilt, ((lppl->iPlayer << 0x5) | iItem), lpfl->id, 0, 0, 0);
    return 1;

L_214f:
    FSendPlrMsg(lppl->iPlayer, 186, lppl->id, lppl->id, cBuilt, ((lppl->iPlayer << 0x5) | iItem), 0, 0, 0, 0);
    return 0;

L_219c:
    lpfl = LpflNew(lppl->iPlayer, lppl->id);
    CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
    lpfl->rgwtMin[4] = LGetFleetStat(lpfl, 1);
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
    FSendPlrMsg(lppl->iPlayer, idm, (lpfl->id | 0x8000), lppl->id, ((lppl->iPlayer << 0x5) | iItem), (lppl->idRoute - 1), 0, 0, 0, 0);
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
    FSendPlrMsg(lppl->iPlayer, idm, (lpfl->id | 0x8000), lppl->id, cBuilt, ((lppl->iPlayer << 0x5) | iItem), (lppl->idRoute - 1), 0, 0, 0);

L_231e:
    goto L_2fc9;

L_2321:
    AutoFleetOrder(lpfl, lppl);
    if ((cBuilt != 1))
        goto L_2379;
    else
        goto L_233e;

L_233e:
    FSendPlrMsg2(lppl->iPlayer, 47, (lpfl->id | 0x8000), lppl->id, ((lppl->iPlayer << 0x5) | iItem));
    goto L_2fc9;

L_2379:
    FSendPlrMsg(lppl->iPlayer, 48, (lpfl->id | 0x8000), lppl->id, cBuilt, ((lppl->iPlayer << 0x5) | iItem), 0, 0, 0, 0);

L_23c4:
    goto L_2fc9;

L_23c7:
    if ((grobj != grobjPlanet))
        goto L_2fc3;
    else
        goto L_23d0;

L_23d0:
    goto L_2f77;

L_23d6:
    return 0;

L_23dc:
    t_scratch_m16_3 = lppl->cFactories;
    cAllowed = (CMaxFactories(lppl, lppl->iPlayer) - t_scratch_m16_3);
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
    lppl->cFactories = (lppl->cFactories + cBuilt);
    idm = idmHaveBuiltFactory;

SendMsgFactMine:
    cBuilt = (cBuilt + FRemovePlayerMessage(lppl->iPlayer, idm, lppl->id));
    if ((cBuilt <= 1))
        goto L_24d7;
    else
        goto L_24af;

L_24af:
    FSendPlrMsg2(lppl->iPlayer, (idm + 1), lppl->id, cBuilt, lppl->id);
    goto L_2fc9;

L_24d7:
    FSendPlrMsg2(lppl->iPlayer, idm, lppl->id, lppl->id, 0);

L_24f9:
    goto L_2fc9;

L_24fc:
    return 0;

L_2505:
    t_scratch_m16_4 = lppl->cMines;
    cAllowed = (CMaxMines(lppl, lppl->iPlayer) - t_scratch_m16_4);
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
    lppl->cMines = (lppl->cMines + cBuilt);
    idm = idmHaveBuiltMine;
    goto SendMsgFactMine;

L_25bb:
    return 0;

L_25c4:
    t_scratch_m16_5 = lppl->cDefenses;
    cAllowed = (CMaxDefenses(lppl, lppl->iPlayer) - t_scratch_m16_5);
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
    lppl->cDefenses = (lppl->cDefenses + cBuilt);
    idm = idmHaveBuiltDefenseOutpost;
    goto SendMsgFactMine;

L_2672:
    return 0;

L_2681:
    raMajor = GetRaceStat(&(rgplr[lppl->iPlayer]), rsMajorAdv);
    iWarp = IWarpMAFromLppl(lppl, &(fTwoMAs));
    if ((iWarp != 0))
        goto L_26e7;
    else
        goto L_26be;

L_26be:
    FSendPlrMsg2(lppl->iPlayer, 209, lppl->id, lppl->id, 0);
    return 0;

L_26e7:
    if ((lppl->idFling != 0x0))
        goto L_2722;
    else
        goto L_26f9;

L_26f9:
    FSendPlrMsg2(lppl->iPlayer, 210, lppl->id, lppl->id, 0);
    return 0;

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
    lpthMac = (lpThings + cThing);
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
    if ((lpth->thp.iWarp != iWarp))
        goto L_2956;
    else
        goto L_2909;

L_2909:
    if ((lpth->thp.idPlanet != (lppl->idFling + 0xffff)))
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
        goto L_2956;

L_2956:
    lpth = (lpth + 1);

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
    lpth->thp.rgwtMin[i] = (lpth->thp.rgwtMin[i] + rgwt[i]);
    if ((lpth->thp.rgwtMin[i] >= 0))
        goto L_29f3;
    else
        goto L_29da;

L_29da:
    lpth->thp.rgwtMin[i] = 32760;

L_29f3:
    lpth->thp.wtMax = (lpth->thp.wtMax + ((uint32_t)((lpth->thp.rgwtMin[i] + 9)) / 10));
    i = (i + 1);

L_2a3f:
    if ((i < 3))
        goto L_299a;
    else
        goto L_2a48;

L_2a48:
    FSendPlrMsg2(lppl->iPlayer, 212, lppl->id, lppl->id, (lppl->idFling - 1));
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
    FSendPlrMsg2(lppl->iPlayer, 297, lppl->id, lppl->id, 0);
    goto L_2fc9;

L_2ac9:
    i = 0;
    goto L_2b35;

L_2ad1:
    lpth->thp.rgwtMin[i] = rgwt[i];
    lpth->thp.wtMax = (lpth->thp.wtMax + ((uint32_t)((rgwt[i] + 9)) / 10));
    i = (i + 1);

L_2b35:
    if ((i < 3))
        goto L_2ad1;
    else
        goto L_2b3e;

L_2b3e:
    lpth->thp.iWarp = iWarp;
    lpth->thp.iDecayRate = iDecayRate;
    lpth->thp.idPlanet = (lppl->idFling + 0xffff);
    lpth->pt = rgptPlan[lppl->id];
    FSendPlrMsg2(lppl->iPlayer, 211, lppl->id, lppl->id, (lppl->idFling - 1));
    goto L_2fc9;

L_2c05:
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
    FSendPlrMsg2(i, 283, lppl->id, lppl->id, 0);
    goto L_2c0d;

L_2c3e:
    if ((GetRaceStat(&(rgplr[lppl->iPlayer]), rsMajorAdv) == raMacintosh))
        goto L_2cea;
    else
        goto L_2c62;

L_2c62:
    lppl->cFactories = 0x0;
    lppl->cMines = 0x0;
    lppl->cDefenses = 0x0;
    lppl->iScanner = 0x1f;

L_2cea:
    i = 0;
    goto L_2d9c;

L_2cf2:
    lppl->rgwtMin[i] = 0;
    t_scratch_m16_6 = Random(50);
    t_call_2d26 = Random(50);
    lppl->rgEnvVarOrig[i] = LOBYTE(((t_call_2d26 + 1) + t_scratch_m16_6));
    lppl->rgEnvVar[i] = LOBYTE(((t_call_2d26 + 1) + t_scratch_m16_6));
    t_scratch_m16_7 = Random(40);
    lppl->rgMinConc[i] = LOBYTE(((Random(40) + 25) + t_scratch_m16_7));
    i = (i + 1);

L_2d9c:
    if ((i >= 3))
        goto L_2fc9;
    else
        goto L_2da2;

L_2da2:
    goto L_2cf2;

L_2da8:
    t_2da8 = cBuilt;
    cBuilt = (cBuilt - 1);
    if ((t_2da8 == 0))
        goto L_2fc9;
    else
        goto L_2db7;

L_2db7:
    i = IBestTerraform(lppl, 1);
    if ((i == 0))
        goto L_2da8;
    else
        goto L_2dd5;

L_2dd5:
    iEnv = (abs(i) - 1);
    if ((i <= 0))
        goto L_2df5;
    else
        goto L_2def;

L_2def:
    t_merge_2df8_0001 = 0x1;
    goto L_2df8;

L_2df5:
    t_merge_2df8_0001 = 0xffff;

L_2df8:
    t_scratch_m18 = t_merge_2df8_0001;
    cAllowed = ((uint16_t)(lppl->rgEnvVar[iEnv]) + t_scratch_m18);
    if ((99 >= cAllowed))
        goto L_2e2c;
    else
        goto L_2e26;

L_2e26:
    t_merge_2e2f_0001 = 0x63;
    goto L_2e2f;

L_2e2c:
    t_merge_2e2f_0001 = cAllowed;

L_2e2f:
    if ((0x1 <= t_merge_2e2f_0001))
        goto L_2e3f;
    else
        goto L_2e39;

L_2e39:
    t_merge_2e53_0001 = 0x1;
    goto L_2e53;

L_2e3f:
    if ((99 >= cAllowed))
        goto L_2e50;
    else
        goto L_2e4a;

L_2e4a:
    t_merge_2e53_0001 = 0x63;
    goto L_2e53;

L_2e50:
    t_merge_2e53_0001 = cAllowed;

L_2e53:
    cAllowed = t_merge_2e53_0001;
    lppl->rgEnvVar[iEnv] = LOBYTE(cAllowed);
    if ((i <= 0))
        goto L_2e98;
    else
        goto L_2e92;

L_2e92:
    t_merge_2e9b_0001 = 1;
    goto L_2e9b;

L_2e98:
    t_merge_2e9b_0001 = 0;

L_2e9b:
    FSendPlrMsg(lppl->iPlayer, 123, lppl->id, lppl->id, t_merge_2e9b_0001, iEnv, ((iEnv * 256) + cAllowed), 0, 0, 0);

L_2ebb:
    goto L_2da8;

L_2ec1:
    idPlayer = lppl->iPlayer;
    LookupBestPlanetaryScanner(&(part));
    idPlayer = -1;
    iItem = (part.hs.iItem + 18);

L_2ee9:
    FSendPlrMsg(lppl->iPlayer, 124, lppl->id, lppl->id, -32768, (iItem - 18), 0, 0, 0, 0);
    lppl->iScanner = (iItem - 18);
    goto L_2fc9;

L_2f77:
    if ((iItem > 27))
        goto L_23d6;
    else
        goto L_2f7f;

L_2f7f:
    switch ((iItem * 2)) {
    case 0x0:
        goto L_2505;
    case 0x2:
        goto L_23dc;
    case 0x4:
        goto L_25c4;
    case 0x6:
        goto L_2fc9;
    case 0x8:
        goto L_2da8;
    case 0xa:
        goto L_2da8;
    case 0xc:
        goto L_2681;
    case 0xe:
        goto L_23dc;
    case 0x10:
        goto L_2505;
    case 0x12:
        goto L_25c4;
    case 0x14:
        goto L_2fc9;
    case 0x16:
        goto L_2fc9;
    case 0x18:
        goto L_2da8;
    case 0x1a:
        goto L_2c05;
    case 0x1c:
        goto L_2681;
    case 0x1e:
        goto L_2681;
    case 0x20:
        goto L_2681;
    case 0x22:
        goto L_2681;
    case 0x24:
        goto L_2ee9;
    case 0x26:
        goto L_2ee9;
    case 0x28:
        goto L_2ee9;
    case 0x2a:
        goto L_2ee9;
    case 0x2c:
        goto L_2ee9;
    case 0x2e:
        goto L_2ee9;
    case 0x30:
        goto L_2ee9;
    case 0x32:
        goto L_2ee9;
    case 0x34:
        goto L_2ee9;
    case 0x36:
        goto L_2ec1;
    }

L_2fc3:
    return 0;

L_2fc9:
    return 1;
}
