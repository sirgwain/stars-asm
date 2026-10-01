void DoBombing() {
    MessageId idmDst;
    int32_t   modKill;
    int16_t   fMulti;
    int32_t   cKillPeople;
    int32_t   dmgBombBldg;
    int32_t   cKillPeopleS;
    int32_t   cKillMine;
    int32_t   dmgBombFloor;
    MessageId idmSrc;
    int32_t   cKillDefenses;
    int32_t   cKillFact;
    int32_t   pctTerra;
    PLANET   *lppl;
    int16_t   ifl;
    FLEET    *lpfl;
    int32_t   cPPE;
    int32_t   dmgBombPeople;
    float     pctSmart;
    float     pctSuccess;
    int32_t   dmgPeopleSmart;
    double    pctSuccessHalf;
    int16_t   pctTot;
    int16_t   dChg;
    int16_t   i;

    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->fDead == 0 && lpfl->idPlanet != -1 && lpfl->fBombed == 0) {
            lppl = lpPlanets + lpfl->idPlanet;
            if (lppl->iPlayer != lpfl->iPlayer && lppl->iPlayer != -1 && FAttackPlayer(lpfl, lppl->iPlayer) != 0 && lppl->fStarbase == 0 &&
                FCalcFleetBombDamage(lpfl, &dmgBombPeople, &dmgBombFloor, &dmgPeopleSmart, &dmgBombBldg, &pctTerra, &fMulti) != 0) {
                CalcPctSurvive(lppl, &pctSuccess, &pctSmart);
                if ((long double)pctSuccess < (long double)1.0) {
                    if (dmgBombPeople > 0) {
                        dmgBombPeople = (int32_t)((long double)dmgBombPeople * pctSuccess + 0.5);
                    }
                    if (dmgBombFloor > 0) {
                        dmgBombFloor = (int32_t)((long double)dmgBombFloor * pctSuccess + 0.5);
                    }
                    if (dmgPeopleSmart > 0) {
                        dmgPeopleSmart = (int32_t)((long double)dmgPeopleSmart * pctSmart + 0.5);
                    }
                    if (dmgBombBldg > 0) {
                        pctSuccessHalf = (double)(1.0 - ((long double)1.0 - pctSuccess) / 2.0);
                        dmgBombBldg = (int32_t)((long double)dmgBombBldg * pctSuccessHalf + 0.5);
                    }
                }
                cPPE = lppl->cMines + lppl->cFactories + (uint32_t)lppl->cDefenses;
                cKillDefenses = 0;
                cKillPeople = 0;
                cKillMine = 0;
                cKillFact = 0;
                if (dmgBombBldg > 0 && cPPE > 0) {
                    cKillFact = (uint32_t)(lppl->cFactories * dmgBombBldg);
                    modKill = (int32_t)(cKillFact % cPPE);
                    cKillFact = (int32_t)(cKillFact / cPPE);
                    if (modKill > 0) {
                        cKillFact += (uint32_t)(Random(LOWORD(cPPE)) < modKill ? 1 : 0);
                    }
                    if (cKillFact > lppl->cFactories) {
                        cKillFact = lppl->cFactories;
                    }
                    cKillDefenses = (uint32_t)(lppl->cDefenses * dmgBombBldg);
                    modKill = (int32_t)(cKillDefenses % cPPE);
                    cKillDefenses = (int32_t)(cKillDefenses / cPPE);
                    if (modKill > 0) {
                        cKillDefenses += (uint32_t)(Random(LOWORD(cPPE)) < modKill ? 1 : 0);
                    }
                    if (cKillDefenses > lppl->cDefenses) {
                        cKillDefenses = lppl->cDefenses;
                    }
                    cKillMine = dmgBombBldg - (cKillFact + cKillDefenses);
                    if (cKillMine > lppl->cMines) {
                        cKillMine = lppl->cMines;
                    }
                }
                if ((dmgBombPeople > 0 || dmgBombFloor > 0 || dmgPeopleSmart > 0) && lppl->rgwtMin[3] > 0) {
                    cKillPeopleS = (int32_t)(lppl->rgwtMin[3] * dmgPeopleSmart) / 1000;
                    if (cKillPeopleS >= lppl->rgwtMin[3]) {
                        cKillPeopleS = lppl->rgwtMin[3] - 1;
                    }
                    cKillPeople = (uint32_t)((lppl->rgwtMin[3] - cKillPeopleS) * dmgBombPeople);
                    modKill = (int32_t)(cKillPeople % 1000);
                    cKillPeople = (int32_t)(cKillPeople / 1000);
                    if (modKill > 0) {
                        cKillPeople += (uint32_t)(Random(1000) <= modKill ? 1 : 0);
                    }
                    cKillPeople += cKillPeopleS;
                    if (dmgBombPeople > 0 && cKillPeople <= 0) {
                        cKillPeople = 1;
                    }
                    if (cKillPeople < dmgBombFloor) {
                        cKillPeople = dmgBombFloor;
                    }
                    if (cKillPeople > lppl->rgwtMin[3]) {
                        cKillPeople = lppl->rgwtMin[3];
                    }
                }
                if (cKillPeople > 0) {
                    lppl->rgwtMin[3] -= cKillPeople;
                }
                if (cKillFact > 0) {
                    lppl->cFactories -= cKillFact;
                }
                if (cKillMine > 0) {
                    lppl->cMines -= cKillMine;
                }
                if (cKillDefenses > 0) {
                    lppl->cDefenses -= cKillDefenses;
                }
                if (pctTerra > 0) {
                    pctTot = 0;
                    pctTerra -= (int32_t)(((long double)1.0 - pctSuccess) * pctTerra / 2);
                    if (pctTerra > 500) {
                        pctTerra = 500;
                    }
                    for (i = 0; i < 3; i++) {
                        dChg = lppl->rgEnvVar[i] - lppl->rgEnvVarOrig[i];
                        if (dChg > 0) {
                            if (dChg >= pctTerra) {
                                dChg = LOWORD(pctTerra);
                            }
                            lppl->rgEnvVar[i] -= LOBYTE(dChg);
                            pctTot += dChg;
                        } else if (dChg < 0) {
                            if ((int16_t)-dChg >= pctTerra) {
                                dChg = -LOWORD(pctTerra);
                            }
                            lppl->rgEnvVar[i] -= LOBYTE(dChg);
                            pctTot += -dChg;
                        }
                    }
                    if (pctTot > 0) {
                        FSendPlrMsg(lpfl->iPlayer, fMulti == 0 ? 302 : 378, lpfl->id | 0x8000, lpfl->id, lppl->id, pctTot, 0, 0, 0, 0);
                        FSendPlrMsg(lppl->iPlayer, fMulti == 0 ? 302 : 379, lppl->id, lpfl->id, lppl->id, pctTot, 0, 0, 0, 0);
                    }
                }
                cPPE = cKillMine + cKillFact + cKillDefenses;
                if (cPPE > 0) {
                    if (lppl->rgwtMin[3] > 0) {
                        idmSrc = fMulti == 0 ? idmHasBombedKillingColonistsDestroyingOneInstallati : idmFleetsHaveBombedKillingColonistsDestroyingOne;
                        idmDst = fMulti == 0 ? idmHasBombedKillingColonistsDestroyingOneInstallati3 : idmFleetsHaveBombedKillingColonistsDestroyingOne3;
                        if (cPPE > 1) {
                            idmSrc++;
                            idmDst++;
                        }
                        if (cKillPeople > 0) {
                            if ((long double)pctSuccess != (long double)1.0) {
                                idmSrc += 5;
                                idmDst += 5;
                                FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE),
                                            (int32_t)(((long double)1.0 - pctSuccess) * 10000), 0, 0);
                                FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE),
                                            (int32_t)(((long double)1.0 - pctSuccess) * 10000), 0, 0);
                                goto L_be65;
                            }
                        } else {
                            idmSrc -= 2;
                            idmDst -= 2;
                            if ((long double)pctSuccess == (long double)1.0) {
                                FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cPPE), 0, 0, 0, 0);
                                FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cPPE), 0, 0, 0, 0);
                                goto L_be65;
                            }
                            idmSrc += 5;
                            idmDst += 5;
                            FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cPPE),
                                        (int32_t)(((long double)1.0 - pctSuccess) * 10000), 0, 0, 0);
                            FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cPPE), (int32_t)(((long double)1.0 - pctSuccess) * 10000),
                                        0, 0, 0);
                            goto L_be65;
                        }
                    } else {
                        idmSrc = fMulti == 0 ? idmHasBombedKillingOffEnemyColonists : idmFleetsHaveBombedKillingOffEnemyColonists;
                        idmDst = fMulti == 0 ? idmHasBombedKillingColonists3 : idmFleetsHaveBombedKillingColonists3;
                    }
                    FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), 0, 0, 0);
                    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), 0, 0, 0);
                } else if (cKillPeople > 0) {
                    if (lppl->rgwtMin[3] > 0) {
                        idmSrc = fMulti == 0 ? idmHasBombedKillingColonists : idmFleetsHaveBombedKillingColonists;
                        idmDst = fMulti == 0 ? idmHasBombedKillingColonists2 : idmFleetsHaveBombedKillingColonists2;
                    } else {
                        idmSrc = fMulti == 0 ? idmHasBombedKillingOffEnemyColonists : idmFleetsHaveBombedKillingOffEnemyColonists;
                        idmDst = fMulti == 0 ? idmHasBombedKillingColonists3 : idmFleetsHaveBombedKillingColonists3;
                    }
                    FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cKillPeople), 0, 0, 0, 0);
                    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), 0, 0, 0, 0);
                }
            L_be65:
                if (lppl->rgwtMin[3] == 0) {
                    UninhabitPlanet(lppl);
                }
            }
        }
    }
    return;
}
