int16_t FBuildObject(PLANET *lppl, GrobjClass grobj, int16_t iItem, int16_t cBuilt, int32_t *rgMinerals) {
    int16_t       iWarp;
    int16_t       i;
    FLEET        *lpfl;
    MessageId     idm;
    int16_t       fTwoMAs;
    SHDEF        *lpshdef;
    int16_t       cAllowed;
    int32_t       dpOrig;
    int16_t       cshDamaged;
    int16_t       cshOrig;
    uint16_t      dpShdef;
    THING        *lpthMac;
    PacketDecay   iDecayRate;
    THING        *lpth;
    RaceAttribute raMajor;
    int16_t       iWarpAsked;
    int16_t       cSize;
    int16_t       rgwt[3];
    int32_t       l;
    EnvType       iEnv;
    PART          part;
    uint16_t      t_scratch_m16_3;
    uint16_t      t_scratch_m16_4;
    uint16_t      t_scratch_m16_5;
    int16_t       t_scratch_m16_6;
    int16_t       t_call_2d26;
    int16_t       t_scratch_m16_7;

L_19b2:
    if (grobj != grobjFleet)
        goto L_23c7;
    else
        goto L_19c4;

L_19c4:
    if (iItem < 16)
        goto L_1c78;
    else
        goto L_19cd;

L_19cd:
    iItem -= 16;
    lpshdef = rglpshdefSB[lppl->iPlayer] + iItem;
    if (lpshdef->fFree != 0)
        goto L_1a26;
    else
        goto L_1a09;

L_1a09:
    if (FCanBuildShdef(lpshdef, lppl->iPlayer) != 0)
        goto L_1a2c;
    else
        goto L_1a26;

L_1a26:
    return 0;

L_1a2c:
    idm = idmHasBuiltNew;
    if (lpshdef->hul.wtCargoMax == 0)
        goto L_1a60;
    else
        goto L_1a3e;

L_1a3e:
    idm++;
    if ((uint32_t)lpshdef->hul.wtCargoMax != 0xffff)
        goto L_1a60;
    else
        goto L_1a5c;

L_1a5c:
    idm++;

L_1a60:
    FSendPlrMsg(lppl->iPlayer, idm, lppl->id, lppl->id, lppl->iPlayer << 5 | iItem + 0x10, LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax, 0, 0, 0, 0);
    if (lppl->fStarbase == 0)
        goto L_1b3a;
    else
        goto L_1ad6;

L_1ad6:
    if ((int16_t)rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef <= (int16_t)rglpshdefSB[lppl->iPlayer][iItem].hul.ihuldef)
        goto L_1b3a;
    else
        goto L_1b2c;

L_1b2c:
    KillQueuedShips(lppl);

L_1b3a:
    iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
    if (lppl->fStarbase == 0)
        goto L_1b9b;
    else
        goto L_1b66;

L_1b66:
    rglpshdefSB[lppl->iPlayer][lppl->isb].cExist = rglpshdefSB[lppl->iPlayer][lppl->isb].cExist - 1;
    goto L_1baf;

L_1b9b:
    lppl->fStarbase = 1;

L_1baf:
    lppl->isb = iItem;
    if (iWarp > 0)
        goto L_1c55;
    else
        goto L_1bd1;

L_1bd1:
    iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
    if (iWarp <= 0)
        goto L_1c1f;
    else
        goto L_1bef;

L_1bef:
    lppl->iWarpFling = iWarp + fTwoMAs - 4;
    goto L_1c55;

L_1c1f:
    lppl->iWarpFling = 0;
    lppl->idFling = 0;
    KillQueuedMassPackets(lppl);

L_1c55:
    lpshdef->cBuilt++;
    lpshdef->cExist++;
    return 1;

L_1c78:
    if (lppl->fStarbase == 0)
        goto L_1c98;
    else
        goto L_1c8f;

L_1c8f:
    if (iItem < 16)
        goto L_1c9e;
    else
        goto L_1c98;

L_1c98:
    return 0;

L_1c9e:
    lpshdef = rglpshdef[lppl->iPlayer] + iItem;
    if (lpshdef->fFree != 0)
        goto L_1cf3;
    else
        goto L_1cd6;

L_1cd6:
    if (FCanBuildShdef(lpshdef, lppl->iPlayer) != 0)
        goto L_1d1d;
    else
        goto L_1cf3;

L_1cf3:
    FSendPlrMsg2(lppl->iPlayer, idmStarbaseFailedBuildNewShipTypeBecause, lppl->id, iItem + 1, 0);
    return 0;

L_1d1d:
    if (rgplr[lppl->iPlayer].cFleet != 0x200)
        goto L_219c;
    else
        goto L_1d3a;

L_1d3a:
    i = 0;
    goto L_1d46;

L_1d42:
    i++;

L_1d46:
    if (i >= cFleet)
        goto L_214f;
    else
        goto L_1d51;

L_1d51:
    lpfl = rglpfl[i];
    if (rglpfl[i] != 0)
        goto L_1d81;
    else
        goto L_214f;

L_1d81:
    if (lpfl->iPlayer > lppl->iPlayer)
        goto L_214f;
    else
        goto L_1d97;

L_1d97:
    if (lpfl->iPlayer < lppl->iPlayer)
        goto L_1d42;
    else
        goto L_1dad;

L_1dad:
    if (lpfl->lpplord->rgord[0].pt.x != rgptPlan[lppl->id].x)
        goto L_1d42;
    else
        goto L_1dcb;

L_1dcb:
    if (lpfl->lpplord->rgord[0].pt.y != rgptPlan[lppl->id].y)
        goto L_1d42;
    else
        goto L_1ded;

L_1ded:
    if (32766 - cBuilt <= lpfl->rgcsh[iItem])
        goto L_1d42;
    else
        goto L_1e0f;

L_1e0f:
    if (lpfl->rgcsh[iItem] == 0)
        goto L_20c5;
    else
        goto L_1e2c;

L_1e2c:
    if (lpfl->rgdv[iItem].pctDp == 0)
        goto L_20c5;
    else
        goto L_1e53;

L_1e53:
    dpShdef = rglpshdef[lpfl->iPlayer][iItem].hul.dp;
    cshOrig = lpfl->rgcsh[iItem];
    cshDamaged = LOWORD((int32_t)(lpfl->rgdv[iItem].pctSh * cshOrig) / 100);
    if (cshDamaged != 0)
        goto L_1edb;
    else
        goto L_1ed6;

L_1ed6:
    cshDamaged = 1;

L_1edb:
    dpOrig = (int32_t)((int32_t)((uint32_t)dpShdef * lpfl->rgdv[iItem].pctDp) / 10 * cshDamaged) / 50;
    lpfl->rgdv[iItem].pctSh = LOWORD((int32_t)(cshDamaged * 100) / (int16_t)(cshOrig + cBuilt));
    if (lpfl->rgdv[iItem].pctSh != 0)
        goto L_1ff4;
    else
        goto L_1fc0;

L_1fc0:
    lpfl->rgdv[iItem].pctSh = 1;

L_1ff4:
    cshDamaged = LOWORD((int32_t)(lpfl->rgdv[iItem].pctSh * (int16_t)(cshOrig + cBuilt)) / 100);
    if (cshDamaged != 0)
        goto L_2041;
    else
        goto L_203c;

L_203c:
    cshDamaged = 1;

L_2041:
    lpfl->rgdv[iItem].pctDp = LOWORD((int32_t)((int32_t)(dpOrig * 5) / cshDamaged) * 100 / (int32_t)dpShdef);
    goto L_20de;

L_20c5:
    lpfl->rgdv[iItem].dp = 0;

L_20de:
    CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
    FSendPlrMsg(lppl->iPlayer, idmStarbaseBuiltNewSDueLack27b, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 5 | iItem, lpfl->id, 0, 0, 0);
    return 1;

L_214f:
    FSendPlrMsg(lppl->iPlayer, idmStarbaseBuiltNewShipSTypeLost, lppl->id, lppl->id, cBuilt, lppl->iPlayer << 5 | iItem, 0, 0, 0, 0);
    return 0;

L_219c:
    lpfl = LpflNew(lppl->iPlayer, lppl->id);
    CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
    lpfl->rgwtMin[4] = LGetFleetStat(lpfl, 1);
    if (lppl->idRoute == 0)
        goto L_2321;
    else
        goto L_2201;

L_2201:
    AutoRouteFleet(lpfl, lppl);
    if (cBuilt != 1)
        goto L_22a0;
    else
        goto L_221e;

L_221e:
    idm = lpfl->lpplord->rgord[1].iWarp == 0 ? idmStarbaseHasBuiltNewWhichWillRouted : idmStarbaseHasBuiltNewWhichRouted;
    FSendPlrMsg(lppl->iPlayer, idm, lpfl->id | 0x8000, lppl->id, lppl->iPlayer << 5 | iItem, lppl->idRoute - 1, 0, 0, 0, 0);
    goto L_2fc9;

L_22a0:
    idm = lpfl->lpplord->rgord[1].iWarp == 0 ? idmStarbaseHasBuiltNewShipsWhichWill : idmStarbaseHasBuiltNewShipsWhichRouted;
    FSendPlrMsg(lppl->iPlayer, idm, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 5 | iItem, lppl->idRoute - 1, 0, 0, 0);

L_231e:
    goto L_2fc9;

L_2321:
    AutoFleetOrder(lpfl, lppl);
    if (cBuilt != 1)
        goto L_2379;
    else
        goto L_233e;

L_233e:
    FSendPlrMsg2(lppl->iPlayer, idmStarbaseHasBuiltNew, lpfl->id | 0x8000, lppl->id, lppl->iPlayer << 5 | iItem);
    goto L_2fc9;

L_2379:
    FSendPlrMsg(lppl->iPlayer, idmStarbaseHasBuiltNewShips, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 5 | iItem, 0, 0, 0, 0);

L_23c4:
    goto L_2fc9;

L_23c7:
    if (grobj != grobjPlanet)
        goto L_2fc3;
    else
        goto L_23d0;

L_23d0:
    goto L_2f77;

L_23d6:
    return 0;

L_23dc:
    t_scratch_m16_3 = lppl->cFactories;
    cAllowed = CMaxFactories(lppl, lppl->iPlayer) - t_scratch_m16_3;
    cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
    if (cBuilt <= 0)
        goto L_24fc;
    else
        goto L_2435;

L_2435:
    lppl->cFactories += cBuilt;
    idm = idmHaveBuiltFactory;

SendMsgFactMine:
    cBuilt += FRemovePlayerMessage(lppl->iPlayer, idm, lppl->id);
    if (cBuilt <= 1)
        goto L_24d7;
    else
        goto L_24af;

L_24af:
    FSendPlrMsg2(lppl->iPlayer, idm + 1, lppl->id, cBuilt, lppl->id);
    goto L_2fc9;

L_24d7:
    FSendPlrMsg2(lppl->iPlayer, idm, lppl->id, lppl->id, 0);

L_24f9:
    goto L_2fc9;

L_24fc:
    return 0;

L_2505:
    t_scratch_m16_4 = lppl->cMines;
    cAllowed = CMaxMines(lppl, lppl->iPlayer) - t_scratch_m16_4;
    cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
    if (cBuilt <= 0)
        goto L_25bb;
    else
        goto L_255e;

L_255e:
    lppl->cMines += cBuilt;
    idm = idmHaveBuiltMine;
    goto SendMsgFactMine;

L_25bb:
    return 0;

L_25c4:
    t_scratch_m16_5 = lppl->cDefenses;
    cAllowed = CMaxDefenses(lppl, lppl->iPlayer) - t_scratch_m16_5;
    cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
    if (cBuilt <= 0)
        goto L_2672;
    else
        goto L_2615;

L_2615:
    lppl->cDefenses += cBuilt;
    idm = idmHaveBuiltDefenseOutpost;
    goto SendMsgFactMine;

L_2672:
    return 0;

L_2681:
    raMajor = GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv);
    iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
    if (iWarp != 0)
        goto L_26e7;
    else
        goto L_26be;

L_26be:
    FSendPlrMsg2(lppl->iPlayer, idmMineralPacketFormedHasDisintegratedBecausePlanet, lppl->id, lppl->id, 0);
    return 0;

L_26e7:
    if (lppl->idFling != 0)
        goto L_2722;
    else
        goto L_26f9;

L_26f9:
    FSendPlrMsg2(lppl->iPlayer, idmMineralPacketFormedHasDisintegratedBecauseDidnt, lppl->id, lppl->id, 0);
    return 0;

L_2722:
    if (iItem != iobjPacket)
        goto L_2730;
    else
        goto L_272b;

L_272b:
    iItem = iobjPacketMixed;

L_2730:
    if (iItem != iobjPacketMixed)
        goto L_2751;
    else
        goto L_2739;

L_2739:
    cSize = raMajor == raMassAccel ? 25 : 40;
    goto L_2766;

L_2751:
    cSize = raMajor == raMassAccel ? 70 : 100;

L_2766:
    i = 0;
    goto L_27e5;

L_276e:
    if (i == iItem - 14)
        goto L_2785;
    else
        goto L_277c;

L_277c:
    if (iItem != iobjPacketMixed)
        goto L_27d3;
    else
        goto L_2785;

L_2785:
    l = (uint32_t)(cSize * cBuilt);
    if (l <= 32760)
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
    i++;

L_27e5:
    if (i < 3)
        goto L_276e;
    else
        goto L_27ee;

L_27ee:
    iWarpAsked = lppl->iWarpFling + 4;
    if (iWarpAsked < 5)
        goto L_281a;
    else
        goto L_280c;

L_280c:
    if (iWarpAsked <= iWarp + 3)
        goto L_2823;
    else
        goto L_281a;

L_281a:
    iWarpAsked = iWarp + fTwoMAs;

L_2823:
    if (iWarpAsked > iWarp + fTwoMAs)
        goto L_2839;
    else
        goto L_2831;

L_2831:
    iDecayRate = decayNone;
    goto L_2845;

L_2839:
    iDecayRate = iWarpAsked - iWarp - fTwoMAs;

L_2845:
    if (raMajor != raStargate)
        goto L_285b;
    else
        goto L_284e;

L_284e:
    if ((int16_t)iDecayRate >= decay50Pct)
        goto L_285b;
    else
        goto L_2857;

L_2857:
    iDecayRate++;

L_285b:
    iWarp = iWarpAsked - 4;
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    goto L_295a;

L_288f:
    if (lpth->iplr != lppl->iPlayer)
        goto L_2956;
    else
        goto L_28ab;

L_28ab:
    if (lpth->ith != ithMineralPacket)
        goto L_2956;
    else
        goto L_28c1;

L_28c1:
    if (lpth->pt.x != rgptPlan[lppl->id].x)
        goto L_2956;
    else
        goto L_28e7;

L_28e7:
    if (lpth->pt.y != rgptPlan[lppl->id].y)
        goto L_2956;
    else
        goto L_28f0;

L_28f0:
    if (lpth->thp.iWarp != iWarp)
        goto L_2956;
    else
        goto L_2909;

L_2909:
    if (lpth->thp.idPlanet != lppl->idFling - 1)
        goto L_2956;
    else
        goto L_2928;

L_2928:
    if (lpth->thp.iDecayRate != iDecayRate)
        goto L_2956;
    else
        goto L_2941;

L_2941:
    if (lpth->thp.wtMax < 1630)
        goto L_2968;
    else
        goto L_2956;

L_2956:
    lpth++;

L_295a:
    if (lpth < lpthMac)
        goto L_288f;
    else
        goto L_2968;

L_2968:
    if (lpth != lpthMac)
        goto L_297e;
    else
        goto L_2a78;

L_297e:
    lpth->thp.wtMax = 0;
    i = 0;
    goto L_2a3f;

L_299a:
    lpth->thp.rgwtMin[i] += rgwt[i];
    if (lpth->thp.rgwtMin[i] >= 0)
        goto L_29f3;
    else
        goto L_29da;

L_29da:
    lpth->thp.rgwtMin[i] = 32760;

L_29f3:
    lpth->thp.wtMax += (int16_t)(lpth->thp.rgwtMin[i] + 9) / 10;
    i++;

L_2a3f:
    if (i < 3)
        goto L_299a;
    else
        goto L_2a48;

L_2a48:
    FSendPlrMsg2(lppl->iPlayer, idmHasProducedMineralPacketWhichHasCombined, lppl->id, lppl->id, lppl->idFling - 1);
    goto L_2fc9;

L_2a78:
    lpth = LpthNew(lppl->iPlayer, ithMineralPacket);
    if (lpth != 0)
        goto L_2ac9;
    else
        goto L_2aa3;

L_2aa3:
    FSendPlrMsg2(lppl->iPlayer, idmHasOrdersBuildMineralPacketEitherDoesnt, lppl->id, lppl->id, 0);
    goto L_2fc9;

L_2ac9:
    i = 0;
    goto L_2b35;

L_2ad1:
    lpth->thp.rgwtMin[i] = rgwt[i];
    lpth->thp.wtMax += (int16_t)(rgwt[i] + 9) / 10;
    i++;

L_2b35:
    if (i < 3)
        goto L_2ad1;
    else
        goto L_2b3e;

L_2b3e:
    lpth->thp.iWarp = iWarp;
    lpth->thp.iDecayRate = iDecayRate;
    lpth->thp.idPlanet = lppl->idFling - 1;
    lpth->pt = rgptPlan[lppl->id];
    FSendPlrMsg2(lppl->iPlayer, idmHasProducedMineralPacketWhichHasDestination, lppl->id, lppl->id, lppl->idFling - 1);
    goto L_2fc9;

L_2c05:
    i = 0;
    goto L_2c11;

L_2c0d:
    i++;

L_2c11:
    if (i >= game.cPlayer)
        goto L_2c3e;
    else
        goto L_2c1c;

L_2c1c:
    FSendPlrMsg2(i, idmStrongFundamentalForcesHaveRebirthed, lppl->id, lppl->id, 0);
    goto L_2c0d;

L_2c3e:
    if (GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) == raMacintosh)
        goto L_2cea;
    else
        goto L_2c62;

L_2c62:
    lppl->cFactories = 0;
    lppl->cMines = 0;
    lppl->cDefenses = 0;
    lppl->iScanner = 31;

L_2cea:
    i = 0;
    goto L_2d9c;

L_2cf2:
    lppl->rgwtMin[i] = 0;
    t_scratch_m16_6 = Random(50);
    t_call_2d26 = Random(50);
    lppl->rgEnvVarOrig[i] = t_call_2d26 + 1 + t_scratch_m16_6;
    lppl->rgEnvVar[i] = t_call_2d26 + 1 + t_scratch_m16_6;
    t_scratch_m16_7 = Random(40);
    lppl->rgMinConc[i] = Random(40) + 25 + t_scratch_m16_7;
    i++;

L_2d9c:
    if (i >= 3)
        goto L_2fc9;
    else
        goto L_2da2;

L_2da2:
    goto L_2cf2;

L_2da8:
    if (cBuilt-- == 0)
        goto L_2fc9;
    else
        goto L_2db7;

L_2db7:
    i = IBestTerraform(lppl, 1);
    if (i == 0)
        goto L_2da8;
    else
        goto L_2dd5;

L_2dd5:
    iEnv = abs(i) - 1;
    cAllowed = lppl->rgEnvVar[iEnv] + (i <= 0 ? -1 : 1);
    if (1 <= (99 >= cAllowed ? cAllowed : 99))
        goto L_2e3f;
    else
        goto L_2e39;

L_2e39:
    cAllowed = 1;
    goto L_2e53;

L_2e3f:
    if (99 >= cAllowed)
        goto L_2e50;
    else
        goto L_2e4a;

L_2e4a:
    cAllowed = 99;
    goto L_2e53;

L_2e50:

L_2e53:
    lppl->rgEnvVar[iEnv] = cAllowed;
    FSendPlrMsg(lppl->iPlayer, idmTerraformingEffortsHave, lppl->id, lppl->id, i > 0, iEnv, iEnv * 256 + cAllowed, 0, 0, 0);

L_2ebb:
    goto L_2da8;

L_2ec1:
    idPlayer = lppl->iPlayer;
    LookupBestPlanetaryScanner(&part);
    idPlayer = -1;
    iItem = part.hs.iItem + 18;

L_2ee9:
    FSendPlrMsg(lppl->iPlayer, idmHasBuiltNewPlanetaryScanner, lppl->id, lppl->id, -32768, iItem - 18, 0, 0, 0, 0);
    lppl->iScanner = iItem - 18;
    goto L_2fc9;

L_2f77:
    if ((uint16_t)iItem > iobjPlanetaryScanner)
        goto L_23d6;
    else
        goto L_2f7f;

L_2f7f:
    switch (iItem * 2) {
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
