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
