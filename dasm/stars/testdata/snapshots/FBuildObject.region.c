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
    EnvType   iEnv;
    PART      part;
    uint16_t  t_scratch_m16_3;
    uint16_t  t_scratch_m16_4;
    uint16_t  t_scratch_m16_5;
    int16_t   t_scratch_m16_6;
    int16_t   t_call_2d26;
    int16_t   t_scratch_m16_7;

    if (grobj == grobjFleet) {
        if (iItem >= 16) {
            iItem -= 16;
            lpshdef = rglpshdefSB[lppl->iPlayer] + iItem;
            if (lpshdef->fFree != 0 || FCanBuildShdef(lpshdef, lppl->iPlayer) == 0) {
                return 0;
            }
            idm = idmHasBuiltNew;
            if (lpshdef->hul.wtCargoMax != 0) {
                idm++;
                if ((uint32_t)lpshdef->hul.wtCargoMax == 0xffff) {
                    idm++;
                }
            }
            FSendPlrMsg(lppl->iPlayer, idm, lppl->id, lppl->id, lppl->iPlayer << 5 | iItem + 0x10, LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax, 0, 0,
                        0, 0);
            if (lppl->fStarbase != 0 && rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef > rglpshdefSB[lppl->iPlayer][iItem].hul.ihuldef) {
                KillQueuedShips(lppl);
            }
            iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
            if (lppl->fStarbase != 0) {
                rglpshdefSB[lppl->iPlayer][lppl->isb].cExist = rglpshdefSB[lppl->iPlayer][lppl->isb].cExist - 1;
            } else {
                lppl->fStarbase = 1;
            }
            lppl->isb = iItem;
            if (iWarp <= 0) {
                iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
                if (iWarp > 0) {
                    lppl->iWarpFling = iWarp + fTwoMAs - 4;
                } else {
                    lppl->iWarpFling = 0;
                    lppl->idFling = 0;
                    KillQueuedMassPackets(lppl);
                }
            }
            lpshdef->cBuilt++;
            lpshdef->cExist++;
            return 1;
        }
        if (lppl->fStarbase == 0 || iItem >= 16) {
            return 0;
        }
        lpshdef = rglpshdef[lppl->iPlayer] + iItem;
        if (lpshdef->fFree != 0 || FCanBuildShdef(lpshdef, lppl->iPlayer) == 0) {
            FSendPlrMsg2(lppl->iPlayer, 79, lppl->id, iItem + 1, 0);
            return 0;
        }
        if (rgplr[lppl->iPlayer].cFleet == 0x200) {
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0 || lpfl->iPlayer > lppl->iPlayer)
                    break;
                if (lpfl->iPlayer >= lppl->iPlayer && lpfl->lpplord->rgord[0].pt.x == rgptPlan[lppl->id].x &&
                    lpfl->lpplord->rgord[0].pt.y == rgptPlan[lppl->id].y && 32766 - cBuilt > lpfl->rgcsh[iItem]) {
                    if (lpfl->rgcsh[iItem] != 0 && lpfl->rgdv[iItem].pctDp != 0) {
                        dpShdef = rglpshdef[lpfl->iPlayer][iItem].hul.dp;
                        cshOrig = lpfl->rgcsh[iItem];
                        cshDamaged = LOWORD((int32_t)(lpfl->rgdv[iItem].pctSh * cshOrig) / 100);
                        if (cshDamaged == 0) {
                            cshDamaged = 1;
                        }
                        dpOrig = (int32_t)((int32_t)((uint32_t)dpShdef * lpfl->rgdv[iItem].pctDp) / 10 * cshDamaged) / 50;
                        lpfl->rgdv[iItem].pctSh = LOWORD((int32_t)(cshDamaged * 100) / (int16_t)(cshOrig + cBuilt));
                        if (lpfl->rgdv[iItem].pctSh == 0) {
                            lpfl->rgdv[iItem].pctSh = 1;
                        }
                        cshDamaged = LOWORD((int32_t)(lpfl->rgdv[iItem].pctSh * (int16_t)(cshOrig + cBuilt)) / 100);
                        if (cshDamaged == 0) {
                            cshDamaged = 1;
                        }
                        lpfl->rgdv[iItem].pctDp = LOWORD((int32_t)((int32_t)(dpOrig * 5) / cshDamaged) * 100 / (int32_t)dpShdef);
                    } else {
                        lpfl->rgdv[iItem].dp = 0;
                    }
                    CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
                    FSendPlrMsg(lppl->iPlayer, 313, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 5 | iItem, lpfl->id, 0, 0, 0);
                    return 1;
                }
            }
            FSendPlrMsg(lppl->iPlayer, 186, lppl->id, lppl->id, cBuilt, lppl->iPlayer << 5 | iItem, 0, 0, 0, 0);
            return 0;
        }
        lpfl = LpflNew(lppl->iPlayer, lppl->id);
        CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
        lpfl->rgwtMin[4] = LGetFleetStat(lpfl, 1);
        if (lppl->idRoute != 0) {
            AutoRouteFleet(lpfl, lppl);
            if (cBuilt == 1) {
                idm = lpfl->lpplord->rgord[1].iWarp == 0 ? idmStarbaseHasBuiltNewWhichWillRouted : idmStarbaseHasBuiltNewWhichRouted;
                FSendPlrMsg(lppl->iPlayer, idm, lpfl->id | 0x8000, lppl->id, lppl->iPlayer << 5 | iItem, lppl->idRoute - 1, 0, 0, 0, 0);
            } else {
                idm = lpfl->lpplord->rgord[1].iWarp == 0 ? idmStarbaseHasBuiltNewShipsWhichWill : idmStarbaseHasBuiltNewShipsWhichRouted;
                FSendPlrMsg(lppl->iPlayer, idm, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 5 | iItem, lppl->idRoute - 1, 0, 0, 0);
            }
        } else {
            AutoFleetOrder(lpfl, lppl);
            if (cBuilt == 1) {
                FSendPlrMsg2(lppl->iPlayer, 47, lpfl->id | 0x8000, lppl->id, lppl->iPlayer << 5 | iItem);
            } else {
                FSendPlrMsg(lppl->iPlayer, 48, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 5 | iItem, 0, 0, 0, 0);
            }
        }
    } else {
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
            if (cBuilt > 0) {
                lppl->cFactories += cBuilt;
                idm = idmHaveBuiltFactory;
                break;
            }
            return 0;
        case 0:
        case 8:
            t_scratch_m16_4 = lppl->cMines;
            cAllowed = CMaxMines(lppl, lppl->iPlayer) - t_scratch_m16_4;
            cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
            if (cBuilt > 0) {
                lppl->cMines += cBuilt;
                idm = idmHaveBuiltMine;
                break;
            }
            return 0;
        case 2:
        case 9:
            t_scratch_m16_5 = lppl->cDefenses;
            cAllowed = CMaxDefenses(lppl, lppl->iPlayer) - t_scratch_m16_5;
            cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
            if (cBuilt > 0) {
                lppl->cDefenses += cBuilt;
                idm = idmHaveBuiltDefenseOutpost;
                break;
            }
            return 0;
        case 6:
        case 14:
        case 15:
        case 16:
        case 17:
            raMajor = GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv);
            iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
            if (iWarp == 0) {
                FSendPlrMsg2(lppl->iPlayer, 209, lppl->id, lppl->id, 0);
                return 0;
            }
            if (lppl->idFling == 0) {
                FSendPlrMsg2(lppl->iPlayer, 210, lppl->id, lppl->id, 0);
                return 0;
            }
            if (iItem == 6) {
                iItem = 17;
            }
            if (iItem == 17) {
                cSize = raMajor == 6 ? 25 : 40;
            } else {
                cSize = raMajor == 6 ? 70 : 100;
            }
            for (i = 0; i < 3; i++) {
                if (i == iItem - 14 || iItem == 17) {
                    l = (uint32_t)(cSize * cBuilt);
                    if (l > 32760) {
                        l = 32760;
                    }
                    rgwt[i] = LOWORD(l);
                } else {
                    rgwt[i] = 0;
                }
            }
            iWarpAsked = lppl->iWarpFling + 4;
            if (iWarpAsked < 5 || iWarpAsked > iWarp + 3) {
                iWarpAsked = iWarp + fTwoMAs;
            }
            if (iWarpAsked <= iWarp + fTwoMAs) {
                iDecayRate = 0;
            } else {
                iDecayRate = iWarpAsked - iWarp - fTwoMAs;
            }
            if (raMajor == 7 && iDecayRate < 3) {
                iDecayRate++;
            }
            iWarp = iWarpAsked - 4;
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac &&
                   (lpth->iplr != lppl->iPlayer || lpth->ith != ithMineralPacket || lpth->pt.x != rgptPlan[lppl->id].x || lpth->pt.y != rgptPlan[lppl->id].y ||
                    lpth->thp.iWarp != iWarp || lpth->thp.idPlanet != lppl->idFling - 1 || lpth->thp.iDecayRate != iDecayRate || lpth->thp.wtMax >= 1630);
                 lpth++) {
            }
            if (lpth != lpthMac) {
                lpth->thp.wtMax = 0;
                for (i = 0; i < 3; i++) {
                    lpth->thp.rgwtMin[i] += rgwt[i];
                    if (lpth->thp.rgwtMin[i] < 0) {
                        lpth->thp.rgwtMin[i] = 32760;
                    }
                    lpth->thp.wtMax += (int16_t)(lpth->thp.rgwtMin[i] + 9) / 10;
                }
                FSendPlrMsg2(lppl->iPlayer, 212, lppl->id, lppl->id, lppl->idFling - 1);
                return 1;
            }
            lpth = LpthNew(lppl->iPlayer, ithMineralPacket);
            if (lpth == 0) {
                FSendPlrMsg2(lppl->iPlayer, 297, lppl->id, lppl->id, 0);
                return 1;
            }
            for (i = 0; i < 3; i++) {
                lpth->thp.rgwtMin[i] = rgwt[i];
                lpth->thp.wtMax += (int16_t)(rgwt[i] + 9) / 10;
            }
            lpth->thp.iWarp = iWarp;
            lpth->thp.iDecayRate = iDecayRate;
            lpth->thp.idPlanet = lppl->idFling - 1;
            lpth->pt = rgptPlan[lppl->id];
            FSendPlrMsg2(lppl->iPlayer, 211, lppl->id, lppl->id, lppl->idFling - 1);
            return 1;
        case 13:
            for (i = 0; i < game.cPlayer; i++) {
                FSendPlrMsg2(i, 283, lppl->id, lppl->id, 0);
            }
            if (GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                lppl->cFactories = 0;
                lppl->cMines = 0;
                lppl->cDefenses = 0;
                lppl->iScanner = 31;
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
            while (cBuilt-- != 0) {
                i = IBestTerraform(lppl, 1);
                if (i != 0) {
                    iEnv = abs(i) - 1;
                    cAllowed = lppl->rgEnvVar[iEnv] + (i <= 0 ? -1 : 1);
                    if (1 > (99 >= cAllowed ? cAllowed : 99)) {
                        cAllowed = 1;
                    } else if (99 < cAllowed) {
                        cAllowed = 99;
                    }
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
        cBuilt += FRemovePlayerMessage(lppl->iPlayer, idm, lppl->id);
        if (cBuilt > 1) {
            FSendPlrMsg2(lppl->iPlayer, idm + 1, lppl->id, cBuilt, lppl->id);
        } else {
            FSendPlrMsg2(lppl->iPlayer, idm, lppl->id, lppl->id, 0);
        }
    }
    return 1;
}
