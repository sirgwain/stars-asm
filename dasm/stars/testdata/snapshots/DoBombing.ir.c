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

L_aefa:
    ifl = 0;
    goto L_af0f;

L_af0b:
    ifl++;

L_af0f:
    if (ifl >= cFleet)
        goto L_be8d;
    else
        goto L_af1a;

L_af1a:
    lpfl = rglpfl[ifl];
    if (rglpfl[ifl] != 0)
        goto L_af4a;
    else
        goto L_be8d;

L_af4a:
    if (lpfl->fDead != 0)
        goto L_af0b;
    else
        goto L_af61;

L_af61:
    if (lpfl->idPlanet == -1)
        goto L_af0b;
    else
        goto L_af6e;

L_af6e:
    if (lpfl->fBombed != 0)
        goto L_af0b;
    else
        goto L_af88;

L_af88:
    lppl = lpPlanets + lpfl->idPlanet;
    if (lppl->iPlayer == lpfl->iPlayer)
        goto L_af0b;
    else
        goto L_afb5;

L_afb5:
    if (lppl->iPlayer == -1)
        goto L_af0b;
    else
        goto L_afc2;

L_afc2:
    if (FAttackPlayer(lpfl, lppl->iPlayer) == 0)
        goto L_af0b;
    else
        goto L_afdf;

L_afdf:
    if (lppl->fStarbase != 0)
        goto L_af0b;
    else
        goto L_aff9;

L_aff9:
    if (FCalcFleetBombDamage(lpfl, &dmgBombPeople, &dmgBombFloor, &dmgPeopleSmart, &dmgBombBldg, &pctTerra, &fMulti) == 0)
        goto L_af0b;
    else
        goto L_b02a;

L_b02a:
    CalcPctSurvive(lppl, &pctSuccess, &pctSmart);
    if ((long double)pctSuccess >= (long double)1.0)
        goto L_b14e;
    else
        goto L_b055;

L_b055:
    if (dmgBombPeople <= 0)
        goto L_b08a;
    else
        goto L_b06c;

L_b06c:
    dmgBombPeople = (int32_t)((long double)dmgBombPeople * pctSuccess + 0.5);

L_b08a:
    if (dmgBombFloor <= 0)
        goto L_b0bf;
    else
        goto L_b0a1;

L_b0a1:
    dmgBombFloor = (int32_t)((long double)dmgBombFloor * pctSuccess + 0.5);

L_b0bf:
    if (dmgPeopleSmart <= 0)
        goto L_b0f4;
    else
        goto L_b0d6;

L_b0d6:
    dmgPeopleSmart = (int32_t)((long double)dmgPeopleSmart * pctSmart + 0.5);

L_b0f4:
    if (dmgBombBldg <= 0)
        goto L_b14e;
    else
        goto L_b10b;

L_b10b:
    pctSuccessHalf = (double)(1.0 - ((long double)1.0 - pctSuccess) / 2.0);
    dmgBombBldg = (int32_t)((long double)dmgBombBldg * pctSuccessHalf + 0.5);

L_b14e:
    cPPE = lppl->cMines + lppl->cFactories + (uint32_t)lppl->cDefenses;
    cKillDefenses = 0;
    cKillPeople = 0;
    cKillMine = 0;
    cKillFact = 0;
    if (dmgBombBldg <= 0)
        goto L_b45c;
    else
        goto L_b208;

L_b208:
    if (cPPE <= 0)
        goto L_b45c;
    else
        goto L_b21f;

L_b21f:
    cKillFact = (uint32_t)(lppl->cFactories * dmgBombBldg);
    modKill = (int32_t)(cKillFact % cPPE);
    cKillFact = (int32_t)(cKillFact / cPPE);
    if (modKill <= 0)
        goto L_b2c8;
    else
        goto L_b292;

L_b292:
    goto L_b2c2;

L_b2c2:
    cKillFact += (uint32_t)(Random(LOWORD(cPPE)) < modKill ? 1 : 0);

L_b2c8:
    if (cKillFact <= (int32_t)lppl->cFactories)
        goto L_b315;
    else
        goto L_b2f6;

L_b2f6:
    cKillFact = lppl->cFactories;

L_b315:
    cKillDefenses = (uint32_t)(lppl->cDefenses * dmgBombBldg);
    modKill = (int32_t)(cKillDefenses % cPPE);
    cKillDefenses = (int32_t)(cKillDefenses / cPPE);
    if (modKill <= 0)
        goto L_b3b6;
    else
        goto L_b380;

L_b380:
    goto L_b3b0;

L_b3b0:
    cKillDefenses += (uint32_t)(Random(LOWORD(cPPE)) < modKill ? 1 : 0);

L_b3b6:
    if (cKillDefenses <= (int32_t)lppl->cDefenses)
        goto L_b3f3;
    else
        goto L_b3dc;

L_b3dc:
    cKillDefenses = lppl->cDefenses;

L_b3f3:
    cKillMine = dmgBombBldg - (cKillFact + cKillDefenses);
    if (cKillMine <= (int32_t)lppl->cMines)
        goto L_b45c;
    else
        goto L_b43d;

L_b43d:
    cKillMine = lppl->cMines;

L_b45c:
    if (dmgBombPeople <= 0)
        goto L_b473;
    else
        goto L_b4a1;

L_b473:
    if (dmgBombFloor <= 0)
        goto L_b48a;
    else
        goto L_b4a1;

L_b48a:
    if (dmgPeopleSmart <= 0)
        goto L_b65b;
    else
        goto L_b4a1;

L_b4a1:
    if (lppl->rgwtMin[3] <= 0)
        goto L_b65b;
    else
        goto L_b4bd;

L_b4bd:
    cKillPeopleS = (int32_t)(lppl->rgwtMin[3] * dmgPeopleSmart) / 1000;
    if (cKillPeopleS < lppl->rgwtMin[3])
        goto L_b51f;
    else
        goto L_b508;

L_b508:
    cKillPeopleS = lppl->rgwtMin[3] - 1;

L_b51f:
    cKillPeople = (uint32_t)((lppl->rgwtMin[3] - cKillPeopleS) * dmgBombPeople);
    modKill = (int32_t)(cKillPeople % 1000);
    cKillPeople = (int32_t)(cKillPeople / 1000);
    if (modKill <= 0)
        goto L_b5bf;
    else
        goto L_b58c;

L_b58c:
    goto L_b5b9;

L_b5b9:
    cKillPeople += (uint32_t)(Random(1000) <= modKill ? 1 : 0);

L_b5bf:
    cKillPeople += cKillPeopleS;
    if (dmgBombPeople <= 0)
        goto L_b603;
    else
        goto L_b5e2;

L_b5e2:
    if (cKillPeople <= 0)
        goto L_b5f9;
    else
        goto L_b603;

L_b5f9:
    cKillPeople = 1;

L_b603:
    if (cKillPeople < dmgBombFloor)
        goto L_b61e;
    else
        goto L_b62a;

L_b61e:
    cKillPeople = dmgBombFloor;

L_b62a:
    if (cKillPeople <= lppl->rgwtMin[3])
        goto L_b65b;
    else
        goto L_b64a;

L_b64a:
    cKillPeople = lppl->rgwtMin[3];

L_b65b:
    if (cKillPeople <= 0)
        goto L_b683;
    else
        goto L_b672;

L_b672:
    lppl->rgwtMin[3] -= cKillPeople;

L_b683:
    if (cKillFact <= 0)
        goto L_b6f1;
    else
        goto L_b69a;

L_b69a:
    lppl->cFactories -= cKillFact;

L_b6f1:
    if (cKillMine <= 0)
        goto L_b761;
    else
        goto L_b708;

L_b708:
    lppl->cMines -= cKillMine;

L_b761:
    if (cKillDefenses <= 0)
        goto L_b7d1;
    else
        goto L_b778;

L_b778:
    lppl->cDefenses -= cKillDefenses;

L_b7d1:
    if (pctTerra <= 0)
        goto L_b9d4;
    else
        goto L_b7e8;

L_b7e8:
    pctTot = 0;
    pctTerra -= (int32_t)(((long double)1.0 - pctSuccess) * pctTerra / 2);
    if (pctTerra <= 500)
        goto L_b84b;
    else
        goto L_b841;

L_b841:
    pctTerra = 500;

L_b84b:
    i = 0;
    goto L_b930;

L_b853:
    dChg = lppl->rgEnvVar[i] - lppl->rgEnvVarOrig[i];
    if (dChg <= 0)
        goto L_b8d8;
    else
        goto L_b895;

L_b895:
    if (dChg < pctTerra)
        goto L_b8b7;
    else
        goto L_b8ae;

L_b8ae:
    dChg = LOWORD(pctTerra);

L_b8b7:
    lppl->rgEnvVar[i] -= LOBYTE(dChg);
    pctTot += dChg;
    goto L_b92c;

L_b8d8:
    if (dChg >= 0)
        goto L_b92c;
    else
        goto L_b8e1;

L_b8e1:
    if ((int16_t)-dChg < pctTerra)
        goto L_b90c;
    else
        goto L_b8fc;

L_b8fc:
    dChg = -LOWORD(pctTerra);

L_b90c:
    lppl->rgEnvVar[i] -= LOBYTE(dChg);
    pctTot += -dChg;

L_b92c:
    i++;

L_b930:
    if (i < 3)
        goto L_b853;
    else
        goto L_b939;

L_b939:
    if (pctTot <= 0)
        goto L_b9d4;
    else
        goto L_b942;

L_b942:
    FSendPlrMsg(lpfl->iPlayer, fMulti == 0 ? 302 : 378, lpfl->id | 0x8000, lpfl->id, lppl->id, pctTot, 0, 0, 0, 0);
    FSendPlrMsg(lppl->iPlayer, fMulti == 0 ? 302 : 379, lppl->id, lpfl->id, lppl->id, pctTot, 0, 0, 0, 0);

L_b9d4:
    cPPE = cKillMine + cKillFact + cKillDefenses;
    if (cPPE <= 0)
        goto L_bd61;
    else
        goto L_ba03;

L_ba03:
    if (lppl->rgwtMin[3] <= 0)
        goto L_ba6b;
    else
        goto L_ba1f;

L_ba1f:
    idmSrc = fMulti == 0 ? idmHasBombedKillingColonistsDestroyingOneInstallati : idmFleetsHaveBombedKillingColonistsDestroyingOne;
    idmDst = fMulti == 0 ? idmHasBombedKillingColonistsDestroyingOneInstallati3 : idmFleetsHaveBombedKillingColonistsDestroyingOne3;
    if (cPPE <= 1)
        goto L_ba98;
    else
        goto L_ba60;

L_ba60:
    idmSrc++;
    idmDst++;

L_ba68:
    goto L_ba98;

L_ba6b:
    idmSrc = fMulti == 0 ? idmHasBombedKillingOffEnemyColonists : idmFleetsHaveBombedKillingOffEnemyColonists;
    idmDst = fMulti == 0 ? idmHasBombedKillingColonists3 : idmFleetsHaveBombedKillingColonists3;
    goto GenericBombMsg;

L_ba98:
    if (cKillPeople <= 0)
        goto L_bc0a;
    else
        goto L_baaf;

L_baaf:
    if ((long double)pctSuccess != (long double)1.0)
        goto L_bb47;
    else
        goto GenericBombMsg;

GenericBombMsg:
    FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), 0, 0, 0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), 0, 0, 0);
    goto L_be65;

L_bb47:
    idmSrc += 5;
    idmDst += 5;
    FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE),
                (int32_t)(((long double)1.0 - pctSuccess) * 10000), 0, 0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), (int32_t)(((long double)1.0 - pctSuccess) * 10000), 0,
                0);

L_bc07:
    goto L_be65;

L_bc0a:
    idmSrc -= 2;
    idmDst -= 2;
    if ((long double)pctSuccess != (long double)1.0)
        goto L_bca4;
    else
        goto L_bc27;

L_bc27:
    FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cPPE), 0, 0, 0, 0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cPPE), 0, 0, 0, 0);
    goto L_be65;

L_bca4:
    idmSrc += 5;
    idmDst += 5;
    FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cPPE), (int32_t)(((long double)1.0 - pctSuccess) * 10000), 0, 0, 0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cPPE), (int32_t)(((long double)1.0 - pctSuccess) * 10000), 0, 0, 0);

L_bd5e:
    goto L_be65;

L_bd61:
    if (cKillPeople <= 0)
        goto L_be65;
    else
        goto L_bd78;

L_bd78:
    if (lppl->rgwtMin[3] <= 0)
        goto L_bdc1;
    else
        goto L_bd94;

L_bd94:
    idmSrc = fMulti == 0 ? idmHasBombedKillingColonists : idmFleetsHaveBombedKillingColonists;
    idmDst = fMulti == 0 ? idmHasBombedKillingColonists2 : idmFleetsHaveBombedKillingColonists2;
    goto L_bdeb;

L_bdc1:
    idmSrc = fMulti == 0 ? idmHasBombedKillingOffEnemyColonists : idmFleetsHaveBombedKillingOffEnemyColonists;
    idmDst = fMulti == 0 ? idmHasBombedKillingColonists3 : idmFleetsHaveBombedKillingColonists3;

L_bdeb:
    FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cKillPeople), 0, 0, 0, 0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), 0, 0, 0, 0);

L_be65:
    if (lppl->rgwtMin[3] != 0)
        goto L_af0b;
    else
        goto L_be7c;

L_be7c:
    UninhabitPlanet(lppl);

L_be8a:
    goto L_af0b;

L_be8d:
    return;
}
