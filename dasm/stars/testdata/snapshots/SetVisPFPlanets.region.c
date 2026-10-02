void SetVisPFPlanets(int16_t iPlr) {
    int32_t  lRadPlanet2;
    int16_t  iRadPlanet;
    PLANET  *lpplMac;
    POINT16  pt;
    int16_t  pctCloak;
    PLANET  *lppl2;
    int16_t  dy;
    FLEET   *lpfl2;
    int32_t  d2;
    PLANET  *lppl;
    int16_t  j;
    THING   *lpth;
    int32_t  lRadius2;
    int16_t  i;
    THING   *lpthMac;
    int16_t  iRadius;
    int16_t  fStargateView;
    int16_t  dx;
    int32_t  l;
    PLANET  *lpplMac2;
    uint16_t grbitPlr;
    int16_t  rgStargateRange[16];
    int32_t  lVis2;

    grbitPlr = iPlr == -1 ? 0 : 1 << iPlr;
    fStargateView = FALSE;
    if (GetRaceStat(&rgplr[iPlr], rsMajorAdv) == raStargate) {
        for (i = 0; i < 10; i++) {
            rgStargateRange[i] = 0;
            if (rglpshdefSB[iPlr][i].fFree == 0) {
                rgStargateRange[i] = StargateRangeFromLppl(NULL, iPlr, i);
                if (rgStargateRange[i] > 0) {
                    fStargateView = TRUE;
                }
            }
        }
    }
    if (iPlr != -1) {
        UpdateProgressGauge(progressStep1);
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            iRadius = GetPlanetScannerRange(lppl, &iRadPlanet);
            lRadius2 = (uint32_t)(iRadius * iRadius);
            lRadPlanet2 = (uint32_t)(iRadPlanet * iRadPlanet);
            pt = rgptPlan[lppl->id];
            for (j = 0; j < cFleet; j++) {
                lpfl2 = rglpfl[j];
                if (rglpfl[j] == 0)
                    break;
                if (lpfl2->fInclude == 0 && lpfl2->fDead == 0) {
                    dx = abs(pt.x - lpfl2->pt.x);
                    if (dx <= iRadius) {
                        dy = abs(pt.y - lpfl2->pt.y);
                        if (dy <= iRadius) {
                            l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                            if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lRadius2 && (lpfl2->idPlanet == -1 || l <= lRadPlanet2)) {
                                pctCloak = PctCloakFromLpfl(lpfl2);
                                if (pctCloak == 0) {
                                    MarkFleet(lpfl2, detSome);
                                } else if (l <= (int32_t)(lRadius2 * (int16_t)(100 - pctCloak)) / 100 * (int16_t)(100 - pctCloak) / 100 &&
                                           (lpfl2->idPlanet == -1 ||
                                            l <= (int32_t)(lRadPlanet2 * (int16_t)(100 - pctCloak)) / 100 * (int16_t)(100 - pctCloak) / 100)) {
                                    MarkFleet(lpfl2, detSome);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (iPlr != -1) {
        UpdateProgressGauge(progressStep1);
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            iRadius = GetPlanetScannerRange(lppl, &iRadPlanet);
            lRadius2 = (uint32_t)(iRadius * iRadius);
            lRadPlanet2 = (uint32_t)(iRadPlanet * iRadPlanet);
            pt = rgptPlan[lppl->id];
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac; lpth++) {
                if (iPlr != -1) {
                    switch (lpth->ith) {
                    case ithMinefield:
                    case ithMineralPacket:
                    case ithMysteryTrader:
                    case ithWormhole:
                        if ((lpth->ith != ithMinefield || (lpth->thm.grbitPlrNow & grbitPlr) == 0) &&
                            (lpth->ith != ithMysteryTrader || lpth->tht.fInclude == 0) && (lpth->ith != ithMineralPacket || lpth->thp.fInclude == 0) &&
                            (lpth->ith != ithWormhole || lpth->thw.fInclude == 0)) {
                            dx = abs(pt.x - lpth->pt.x);
                            if (dx <= iRadius) {
                                dy = abs(pt.y - lpth->pt.y);
                                if (dy <= iRadius) {
                                    l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                    if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lRadius2) {
                                        switch (lpth->ith) {
                                        case ithMineralPacket:
                                            lpth->thp.fInclude = TRUE;
                                            goto LThIncPlr2;
                                        case ithMysteryTrader:
                                            lpth->tht.fInclude = TRUE;
                                            break;
                                        case ithWormhole:
                                            if ((lpth->thw.grbitPlr & grbitPlr) == 0 && l > (int32_t)(lRadius2 >> 4) && l > lRadPlanet2)
                                                break;
                                            lpth->thw.grbitPlr |= grbitPlr;
                                            lpth->thw.fInclude = TRUE;
                                            break;
                                        default:
                                            if ((lpth->thm.grbitPlr & grbitPlr) != 0 || l <= lRadPlanet2 || l <= (int32_t)(lRadius2 >> 4)) {
                                                lpth->thm.grbitPlr |= grbitPlr;
                                                lpth->thm.grbitPlrNow |= grbitPlr;
                                                goto LThIncPlr2;
                                            }
                                        }
                                        break;
                                    LThIncPlr2:
                                        if (rgplr[lpth->iplr].fInclude == 0) {
                                            rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100;
                                            rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfff8) | 3;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (iPlr != -1) {
        UpdateProgressGauge(progressStep1);
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            iRadius = GetPlanetScannerRange(lppl, &iRadPlanet);
            lRadius2 = (uint32_t)(iRadius * iRadius);
            lRadPlanet2 = (uint32_t)(iRadPlanet * iRadPlanet);
            pt = rgptPlan[lppl->id];
            if (fStargateView != 0 && lppl->fStarbase != 0 && rgStargateRange[lppl->isb] > 0) {
                iRadius = rgStargateRange[lppl->isb];
                lRadius2 = (uint32_t)(iRadius * iRadius);
                lppl2 = lpPlanets;
                lpplMac2 = lpPlanets + cPlanet;
                for (; lppl2 < lpplMac2; lppl2++) {
                    if ((lppl2->fInclude == 0 || lppl2->det < detSome) && lppl2->fStarbase != 0 && StargateRangeFromLppl(lppl2, 0, 0) != 0) {
                        if (iRadius < 10000) {
                            dx = abs(rgptPlan[lppl2->id].x - pt.x);
                            if (dx > iRadius)
                                continue;
                            dy = abs(rgptPlan[lppl2->id].y - pt.y);
                            if (dy > iRadius)
                                continue;
                            d2 = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                            if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) > lRadius2)
                                continue;
                            lVis2 = rglpshdefSB[lppl2->iPlayer][lppl2->isb].lVisible;
                            if (lVis2 < 10000 && d2 > (int32_t)(lRadius2 * lVis2) / 10000)
                                continue;
                        }
                        MarkPlanet(lppl2, iPlr, detSome);
                    }
                }
            }
        }
    }
    if (iPlr != -1) {
        UpdateProgressGauge(progressStep1);
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            iRadius = GetPlanetScannerRange(lppl, &iRadPlanet);
            lRadius2 = (uint32_t)(iRadius * iRadius);
            lRadPlanet2 = (uint32_t)(iRadPlanet * iRadPlanet);
            pt = rgptPlan[lppl->id];
            if (iRadPlanet > 0) {
                iRadius = iRadPlanet;
                lRadius2 = lRadPlanet2;
                lppl2 = lpPlanets;
                lpplMac2 = lpPlanets + cPlanet;
                for (; lppl2 < lpplMac2; lppl2++) {
                    if (lppl2->fInclude == 0 || lppl2->det < detSome) {
                        dx = abs(rgptPlan[lppl2->id].x - pt.x);
                        if (dx <= iRadius) {
                            dy = abs(rgptPlan[lppl2->id].y - pt.y);
                            if (dy <= iRadius) {
                                d2 = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lRadius2) {
                                    if (lppl2->fStarbase != 0 && lppl2->iPlayer != -1) {
                                        lVis2 = rglpshdefSB[lppl2->iPlayer][lppl2->isb].lVisible;
                                        if (lVis2 < 10000 && d2 > (int32_t)(lRadius2 * lVis2) / 10000) {
                                            MarkPlanet(lppl2, iPlr, detObscure);
                                            continue;
                                        }
                                    }
                                    MarkPlanet(lppl2, iPlr, detSome);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}
