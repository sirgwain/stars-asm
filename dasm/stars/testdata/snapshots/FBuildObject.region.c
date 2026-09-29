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
    uint16_t  t_scratch_m16_3;
    uint16_t  t_scratch_m16_4;
    uint16_t  t_scratch_m16_5;
    int16_t   t_scratch_m16_6;
    int16_t   t_call_2d26;
    int16_t   t_scratch_m16_7;
    int16_t   t_2da8;
    int16_t   t_merge_2e53_0001;

    if (grobj != grobjFleet) {
        if (grobj != grobjPlanet) {
            return 0;
        }
        if ((uint16_t)iItem > 27) {
            return 0;
        }
        switch (iItem) {
        case 1:
        case 7:
            t_scratch_m16_3 = lppl->cFactories;
            cAllowed = CMaxFactories(lppl, lppl->iPlayer) - t_scratch_m16_3;
            cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
            if (cBuilt <= 0) {
                return 0;
            }
            lppl->cFactories = lppl->cFactories + cBuilt;
            idm = idmHaveBuiltFactory;
            break;
        case 0:
        case 8:
            t_scratch_m16_4 = lppl->cMines;
            cAllowed = CMaxMines(lppl, lppl->iPlayer) - t_scratch_m16_4;
            cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
            if (cBuilt <= 0) {
                return 0;
            }
            lppl->cMines = lppl->cMines + cBuilt;
            idm = idmHaveBuiltMine;
            break;
        case 2:
        case 9:
            t_scratch_m16_5 = lppl->cDefenses;
            cAllowed = CMaxDefenses(lppl, lppl->iPlayer) - t_scratch_m16_5;
            cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
            if (cBuilt <= 0) {
                return 0;
            }
            lppl->cDefenses = lppl->cDefenses + cBuilt;
            idm = idmHaveBuiltDefenseOutpost;
            break;
        case 6:
        case 14:
        case 15:
        case 16:
        case 17:
            raMajor = GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv);
            iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
            if (iWarp != 0) {
                if (lppl->idFling != 0x0) {
                    if (iItem == 6) {
                        iItem = 17;
                    }
                    if (iItem != 17) {
                        cSize = raMajor == 6 ? 70 : 100;
                    } else {
                        cSize = raMajor == 6 ? 25 : 40;
                    }
                    for (i = 0; i < 3; i++) {
                        if (i != iItem - 14 && iItem != 17) {
                            rgwt[i] = 0;
                        } else {
                            l = (uint32_t)((int32_t)cSize * (int32_t)cBuilt);
                            if (l > 32760) {
                                l = 32760;
                            }
                            rgwt[i] = LOWORD(l);
                        }
                    }
                    iWarpAsked = lppl->iWarpFling + 4;
                    if (iWarpAsked < 5 || iWarpAsked > iWarp + 3) {
                        iWarpAsked = iWarp + fTwoMAs;
                    }
                    if (iWarpAsked > iWarp + fTwoMAs) {
                        iDecayRate = iWarpAsked - iWarp - fTwoMAs;
                    } else {
                        iDecayRate = 0;
                    }
                    if (raMajor == 7 && iDecayRate < 3) {
                        iDecayRate = iDecayRate + 1;
                    }
                    iWarp = iWarpAsked - 4;
                    lpth = lpThings;
                    lpthMac = lpThings + cThing;
                    for (; lpth < lpthMac && (lpth->iplr != lppl->iPlayer || lpth->ith != ithMineralPacket || lpth->pt.x != rgptPlan[lppl->id].x ||
                                              lpth->pt.y != rgptPlan[lppl->id].y || lpth->thp.iWarp != iWarp || lpth->thp.idPlanet != lppl->idFling - 0x1 ||
                                              lpth->thp.iDecayRate != iDecayRate || lpth->thp.wtMax >= 0x65e);
                         lpth++) {
                    }
                    if (lpth != lpthMac) {
                        lpth->thp.wtMax = 0x0;
                        for (i = 0; i < 3; i++) {
                            lpth->thp.rgwtMin[i] = lpth->thp.rgwtMin[i] + rgwt[i];
                            if (lpth->thp.rgwtMin[i] < 0) {
                                lpth->thp.rgwtMin[i] = 32760;
                            }
                            lpth->thp.wtMax = lpth->thp.wtMax + (int32_t)(lpth->thp.rgwtMin[i] + 9) / 10;
                        }
                        FSendPlrMsg2(lppl->iPlayer, 212, lppl->id, lppl->id, lppl->idFling - 1);
                        return 1;
                    }
                    lpth = LpthNew(lppl->iPlayer, ithMineralPacket);
                    if (lpth != 0x0) {
                        for (i = 0; i < 3; i++) {
                            lpth->thp.rgwtMin[i] = rgwt[i];
                            lpth->thp.wtMax = lpth->thp.wtMax + (int32_t)(rgwt[i] + 9) / 10;
                        }
                        lpth->thp.iWarp = iWarp;
                        lpth->thp.iDecayRate = iDecayRate;
                        lpth->thp.idPlanet = lppl->idFling - 0x1;
                        lpth->pt = rgptPlan[lppl->id];
                        FSendPlrMsg2(lppl->iPlayer, 211, lppl->id, lppl->id, lppl->idFling - 1);
                        return 1;
                    }
                    FSendPlrMsg2(lppl->iPlayer, 297, lppl->id, lppl->id, 0);
                    return 1;
                }
                FSendPlrMsg2(lppl->iPlayer, 210, lppl->id, lppl->id, 0);
                return 0;
            }
            FSendPlrMsg2(lppl->iPlayer, 209, lppl->id, lppl->id, 0);
            return 0;
        case 13:
            for (i = 0; i < game.cPlayer; i++) {
                FSendPlrMsg2(i, 283, lppl->id, lppl->id, 0);
            }
            if (GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                lppl->cFactories = 0x0;
                lppl->cMines = 0x0;
                lppl->cDefenses = 0x0;
                lppl->iScanner = 0x1f;
            }
            for (i = 0; i < 3; i++) {
                lppl->rgwtMin[i] = 0;
                t_scratch_m16_6 = Random(50);
                t_call_2d26 = Random(50);
                lppl->rgEnvVarOrig[i] = LOBYTE(t_call_2d26 + 1 + t_scratch_m16_6);
                lppl->rgEnvVar[i] = LOBYTE(t_call_2d26 + 1 + t_scratch_m16_6);
                t_scratch_m16_7 = Random(40);
                lppl->rgMinConc[i] = LOBYTE(Random(40) + 25 + t_scratch_m16_7);
            }
            return 1;
        case 4:
        case 5:
        case 12:
            while (1) {
                t_2da8 = cBuilt;
                cBuilt = cBuilt - 1;
                if (t_2da8 == 0)
                    break;
                i = IBestTerraform(lppl, 1);
                if (i != 0) {
                    iEnv = abs(i) - 1;
                    cAllowed = (int16_t)lppl->rgEnvVar[iEnv] + (i <= 0 ? -1 : 1);
                    if (0x1 <= (99 >= cAllowed ? cAllowed : 0x63)) {
                        if (99 >= cAllowed) {
                            t_merge_2e53_0001 = cAllowed;
                        } else {
                            t_merge_2e53_0001 = 99;
                        }
                    } else {
                        t_merge_2e53_0001 = 1;
                    }
                    cAllowed = t_merge_2e53_0001;
                    lppl->rgEnvVar[iEnv] = LOBYTE(cAllowed);
                    FSendPlrMsg(lppl->iPlayer, 123, lppl->id, lppl->id, i <= 0 ? 0 : 1, iEnv, iEnv * 256 + cAllowed, 0, 0, 0);
                }
            }
            return 1;
        case 27:
            idPlayer = lppl->iPlayer;
            LookupBestPlanetaryScanner(&part);
            idPlayer = -1;
            iItem = part.hs.iItem + 18;
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
            FSendPlrMsg(lppl->iPlayer, 124, lppl->id, lppl->id, -32768, iItem - 18, 0, 0, 0, 0);
            lppl->iScanner = iItem - 18;
        case 3:
        case 10:
        case 11:
            return 1;
        }
        cBuilt = cBuilt + FRemovePlayerMessage(lppl->iPlayer, idm, lppl->id);
        if (cBuilt <= 1) {
            FSendPlrMsg2(lppl->iPlayer, idm, lppl->id, lppl->id, 0);
        } else {
            FSendPlrMsg2(lppl->iPlayer, idm + 1, lppl->id, cBuilt, lppl->id);
        }
    } else {
        if (iItem >= 16) {
            iItem = iItem - 16;
            lpshdef = rglpshdefSB[lppl->iPlayer] + iItem;
            if (lpshdef->fFree == 0x0 && FCanBuildShdef(lpshdef, lppl->iPlayer) != 0) {
                idm = idmHasBuiltNew;
                if (lpshdef->hul.wtCargoMax != 0x0) {
                    idm = idm + 1;
                    if ((uint32_t)lpshdef->hul.wtCargoMax == 0xffff) {
                        idm = idm + 1;
                    }
                }
                FSendPlrMsg(lppl->iPlayer, idm, lppl->id, lppl->id, lppl->iPlayer << 0x5 | iItem + 16, LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax, 0,
                            0, 0, 0);
                if (lppl->fStarbase != 0x0 && rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef > rglpshdefSB[lppl->iPlayer][iItem].hul.ihuldef) {
                    KillQueuedShips(lppl);
                }
                iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
                if (lppl->fStarbase == 0x0) {
                    lppl->fStarbase = 0x1;
                } else {
                    rglpshdefSB[lppl->iPlayer][lppl->isb].cExist = rglpshdefSB[lppl->iPlayer][lppl->isb].cExist - 0x1;
                }
                lppl->isb = iItem;
                if (iWarp <= 0) {
                    iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
                    if (iWarp <= 0) {
                        lppl->iWarpFling = 0x0;
                        lppl->idFling = 0x0;
                        KillQueuedMassPackets(lppl);
                    } else {
                        lppl->iWarpFling = iWarp + fTwoMAs - 4;
                    }
                }
                lpshdef->cBuilt = lpshdef->cBuilt + 0x1;
                lpshdef->cExist = lpshdef->cExist + 0x1;
                return 1;
            }
            return 0;
        }
        if (lppl->fStarbase == 0x0 || iItem >= 16) {
            return 0;
        }
        lpshdef = rglpshdef[lppl->iPlayer] + iItem;
        if (lpshdef->fFree != 0x0 || FCanBuildShdef(lpshdef, lppl->iPlayer) == 0) {
            FSendPlrMsg2(lppl->iPlayer, 79, lppl->id, iItem + 1, 0);
            return 0;
        }
        if (rgplr[lppl->iPlayer].cFleet == 0x200) {
            i = 0;
            while (1) {
                if (i >= cFleet)
                    goto L_214f;
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0x0 || lpfl->iPlayer > lppl->iPlayer)
                    goto L_214f;
                if (lpfl->iPlayer >= lppl->iPlayer && lpfl->lpplord->rgord[0].pt.x == rgptPlan[lppl->id].x &&
                    lpfl->lpplord->rgord[0].pt.y == rgptPlan[lppl->id].y && 32766 - cBuilt > lpfl->rgcsh[iItem])
                    break;
                i = i + 1;
            }
            if (lpfl->rgcsh[iItem] == 0 || lpfl->rgdv[iItem].pctDp == 0x0) {
                lpfl->rgdv[iItem].dp = 0x0;
            } else {
                dpShdef = rglpshdef[lpfl->iPlayer][iItem].hul.dp;
                cshOrig = lpfl->rgcsh[iItem];
                cshDamaged = LOWORD((int32_t)((int32_t)(lpfl->rgdv[iItem].pctSh * (int32_t)cshOrig) / 0x64));
                if (cshDamaged == 0) {
                    cshDamaged = 1;
                }
                dpOrig = (int32_t)((int32_t)((int32_t)((int32_t)((uint32_t)dpShdef * lpfl->rgdv[iItem].pctDp) / 0xa) * (int32_t)cshDamaged) / 0x32);
                lpfl->rgdv[iItem].pctSh = LOWORD((int32_t)((int32_t)((int32_t)cshDamaged * 100) / (int32_t)(cshOrig + cBuilt)));
                if (lpfl->rgdv[iItem].pctSh == 0x0) {
                    lpfl->rgdv[iItem].pctSh = 0x1;
                }
                cshDamaged = LOWORD((int32_t)((int32_t)(lpfl->rgdv[iItem].pctSh * (int32_t)(cshOrig + cBuilt)) / 0x64));
                if (cshDamaged == 0) {
                    cshDamaged = 1;
                }
                lpfl->rgdv[iItem].pctDp = LOWORD((int32_t)((int32_t)((int32_t)((int32_t)(dpOrig * 5) / (int32_t)cshDamaged) * 100) / (int32_t)dpShdef));
            }
            CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
            FSendPlrMsg(lppl->iPlayer, 313, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 0x5 | iItem, lpfl->id, 0, 0, 0);
            return 1;
        L_214f:
            FSendPlrMsg(lppl->iPlayer, 186, lppl->id, lppl->id, cBuilt, lppl->iPlayer << 0x5 | iItem, 0, 0, 0, 0);
            return 0;
        }
        lpfl = LpflNew(lppl->iPlayer, lppl->id);
        CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
        lpfl->rgwtMin[4] = LGetFleetStat(lpfl, 1);
        if (lppl->idRoute == 0x0) {
            AutoFleetOrder(lpfl, lppl);
            if (cBuilt != 1) {
                FSendPlrMsg(lppl->iPlayer, 48, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 0x5 | iItem, 0, 0, 0, 0);
            } else {
                FSendPlrMsg2(lppl->iPlayer, 47, lpfl->id | 0x8000, lppl->id, lppl->iPlayer << 0x5 | iItem);
            }
        } else {
            AutoRouteFleet(lpfl, lppl);
            if (cBuilt != 1) {
                idm = lpfl->lpplord->rgord[1].iWarp == 0x0 ? idmStarbaseHasBuiltNewShipsWhichWill : idmStarbaseHasBuiltNewShipsWhichRouted;
                FSendPlrMsg(lppl->iPlayer, idm, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 0x5 | iItem, lppl->idRoute - 1, 0, 0, 0);
            } else {
                idm = lpfl->lpplord->rgord[1].iWarp == 0x0 ? idmStarbaseHasBuiltNewWhichWillRouted : idmStarbaseHasBuiltNewWhichRouted;
                FSendPlrMsg(lppl->iPlayer, idm, lpfl->id | 0x8000, lppl->id, lppl->iPlayer << 0x5 | iItem, lppl->idRoute - 1, 0, 0, 0, 0);
            }
        }
    }
    return 1;
}
