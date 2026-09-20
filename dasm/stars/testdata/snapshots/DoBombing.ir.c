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
    uint16_t  t_merge_b2c2_0001;
    uint16_t  t_merge_b3b0_0001;
    uint16_t  t_merge_b5b9_0001;
    int16_t   t_merge_b97d_0001;
    int16_t   t_merge_b9c4_0001;
    uint16_t  t_merge_ba31_0001;
    uint16_t  t_merge_ba46_0001;
    uint16_t  t_merge_ba7d_0001;
    uint16_t  t_merge_ba92_0001;
    uint16_t  t_merge_bda6_0001;
    uint16_t  t_merge_bdbb_0001;
    uint16_t  t_merge_bdd3_0001;
    uint16_t  t_merge_bde8_0001;

L_aefa:
    ifl = 0;
    goto L_af0f;

L_af0b:
    ifl = (ifl + 1);

L_af0f:
    if ((ifl >= cFleet))
        goto L_be8d;
    else
        goto L_af1a;

L_af1a:
    lpfl = rglpfl[ifl];
    if ((rglpfl[ifl] != 0x0))
        goto L_af4a;
    else
        goto L_be8d;

L_af4a:
    if ((lpfl->fDead != 0x0))
        goto L_af0b;
    else
        goto L_af61;

L_af61:
    if ((lpfl->idPlanet == -1))
        goto L_af0b;
    else
        goto L_af6e;

L_af6e:
    if ((lpfl->fBombed != 0x0))
        goto L_af0b;
    else
        goto L_af88;

L_af88:
    lppl = (lpPlanets + lpfl->idPlanet);
    if ((lppl->iPlayer == lpfl->iPlayer))
        goto L_af0b;
    else
        goto L_afb5;

L_afb5:
    if ((lppl->iPlayer == -1))
        goto L_af0b;
    else
        goto L_afc2;

L_afc2:
    if ((FAttackPlayer(lpfl, lppl->iPlayer) == 0))
        goto L_af0b;
    else
        goto L_afdf;

L_afdf:
    if ((lppl->fStarbase != 0x0))
        goto L_af0b;
    else
        goto L_aff9;

L_aff9:
    if ((FCalcFleetBombDamage(lpfl, &(dmgBombPeople), &(dmgBombFloor), &(dmgPeopleSmart), &(dmgBombBldg), &(pctTerra), &(fMulti)) == 0))
        goto L_af0b;
    else
        goto L_b02a;

L_b02a:
    CalcPctSurvive(lppl, &(pctSuccess), &(pctSmart));
    if ((pctSuccess >= 1))
        goto L_b14e;
    else
        goto L_b055;

L_b055:
    if ((dmgBombPeople <= 0))
        goto L_b08a;
    else
        goto L_b06c;

L_b06c:
    dmgBombPeople = __ftol();

L_b08a:
    if ((dmgBombFloor <= 0))
        goto L_b0bf;
    else
        goto L_b0a1;

L_b0a1:
    dmgBombFloor = __ftol();

L_b0bf:
    if ((dmgPeopleSmart <= 0))
        goto L_b0f4;
    else
        goto L_b0d6;

L_b0d6:
    dmgPeopleSmart = __ftol();

L_b0f4:
    if ((dmgBombBldg <= 0))
        goto L_b14e;
    else
        goto L_b10b;

L_b10b:
    pctSuccessHalf = (1 - ((1 - pctSuccess) / 2));
    dmgBombBldg = __ftol();

L_b14e:
    cPPE = ((lppl->cMines + lppl->cFactories) + (uint32_t)(lppl->cDefenses));
    cKillDefenses = 0;
    cKillPeople = 0;
    cKillMine = 0;
    cKillFact = 0;
    if ((dmgBombBldg <= 0))
        goto L_b45c;
    else
        goto L_b208;

L_b208:
    if ((cPPE <= 0))
        goto L_b45c;
    else
        goto L_b21f;

L_b21f:
    cKillFact = (uint32_t)((lppl->cFactories * dmgBombBldg));
    modKill = (int32_t)((cKillFact % cPPE));
    cKillFact = (int32_t)((cKillFact / cPPE));
    if ((modKill <= 0))
        goto L_b2c8;
    else
        goto L_b292;

L_b292:
    if (((uint32_t)(Random(LOWORD(cPPE))) < modKill))
        goto L_b2b7;
    else
        goto L_b2be;

L_b2b7:
    t_merge_b2c2_0001 = 0x1;
    goto L_b2c2;

L_b2be:
    t_merge_b2c2_0001 = 0x0;

L_b2c2:
    cKillFact = (cKillFact + (uint32_t)(t_merge_b2c2_0001));

L_b2c8:
    if ((cKillFact <= lppl->cFactories))
        goto L_b315;
    else
        goto L_b2f6;

L_b2f6:
    cKillFact = lppl->cFactories;

L_b315:
    cKillDefenses = (uint32_t)((lppl->cDefenses * dmgBombBldg));
    modKill = (int32_t)((cKillDefenses % cPPE));
    cKillDefenses = (int32_t)((cKillDefenses / cPPE));
    if ((modKill <= 0))
        goto L_b3b6;
    else
        goto L_b380;

L_b380:
    if (((uint32_t)(Random(LOWORD(cPPE))) < modKill))
        goto L_b3a5;
    else
        goto L_b3ac;

L_b3a5:
    t_merge_b3b0_0001 = 0x1;
    goto L_b3b0;

L_b3ac:
    t_merge_b3b0_0001 = 0x0;

L_b3b0:
    cKillDefenses = (cKillDefenses + (uint32_t)(t_merge_b3b0_0001));

L_b3b6:
    if ((cKillDefenses <= lppl->cDefenses))
        goto L_b3f3;
    else
        goto L_b3dc;

L_b3dc:
    cKillDefenses = lppl->cDefenses;

L_b3f3:
    cKillMine = (dmgBombBldg - (cKillFact + cKillDefenses));
    if ((cKillMine <= lppl->cMines))
        goto L_b45c;
    else
        goto L_b43d;

L_b43d:
    cKillMine = lppl->cMines;

L_b45c:
    if ((dmgBombPeople <= 0))
        goto L_b473;
    else
        goto L_b4a1;

L_b473:
    if ((dmgBombFloor <= 0))
        goto L_b48a;
    else
        goto L_b4a1;

L_b48a:
    if ((dmgPeopleSmart <= 0))
        goto L_b65b;
    else
        goto L_b4a1;

L_b4a1:
    if ((HIWORD(lppl->rgwtMin[3]) < 0x0))
        goto L_b65b;
    else
        goto L_b4ae;

L_b4ae:
    if ((HIWORD(lppl->rgwtMin[3]) > 0x0))
        goto L_b4bd;
    else
        goto L_b4b3;

L_b4b3:
    if ((LOWORD(lppl->rgwtMin[3]) <= 0x0))
        goto L_b65b;
    else
        goto L_b4bd;

L_b4bd:
    cKillPeopleS = (int32_t)(((uint32_t)((lppl->rgwtMin[3] * dmgPeopleSmart)) / 0x3e8));
    if ((HIWORD(cKillPeopleS) < HIWORD(lppl->rgwtMin[3])))
        goto L_b51f;
    else
        goto L_b4fb;

L_b4fb:
    if ((HIWORD(cKillPeopleS) > HIWORD(lppl->rgwtMin[3])))
        goto L_b508;
    else
        goto L_b500;

L_b500:
    if ((LOWORD(cKillPeopleS) < LOWORD(lppl->rgwtMin[3])))
        goto L_b51f;
    else
        goto L_b508;

L_b508:
    cKillPeopleS = (lppl->rgwtMin[3] - 1);

L_b51f:
    cKillPeople = (uint32_t)(((lppl->rgwtMin[3] - cKillPeopleS) * dmgBombPeople));
    modKill = (int32_t)((cKillPeople % 1000));
    cKillPeople = (int32_t)((cKillPeople / 1000));
    if ((modKill <= 0))
        goto L_b5bf;
    else
        goto L_b58c;

L_b58c:
    if (((uint32_t)(Random(1000)) <= modKill))
        goto L_b5ae;
    else
        goto L_b5b5;

L_b5ae:
    t_merge_b5b9_0001 = 0x1;
    goto L_b5b9;

L_b5b5:
    t_merge_b5b9_0001 = 0x0;

L_b5b9:
    cKillPeople = (cKillPeople + (uint32_t)(t_merge_b5b9_0001));

L_b5bf:
    cKillPeople = (cKillPeople + cKillPeopleS);
    if ((dmgBombPeople <= 0))
        goto L_b603;
    else
        goto L_b5e2;

L_b5e2:
    if ((cKillPeople <= 0))
        goto L_b5f9;
    else
        goto L_b603;

L_b5f9:
    cKillPeople = 1;

L_b603:
    if ((cKillPeople < dmgBombFloor))
        goto L_b61e;
    else
        goto L_b62a;

L_b61e:
    cKillPeople = dmgBombFloor;

L_b62a:
    if ((HIWORD(cKillPeople) < HIWORD(lppl->rgwtMin[3])))
        goto L_b65b;
    else
        goto L_b63d;

L_b63d:
    if ((HIWORD(cKillPeople) > HIWORD(lppl->rgwtMin[3])))
        goto L_b64a;
    else
        goto L_b642;

L_b642:
    if ((LOWORD(cKillPeople) <= LOWORD(lppl->rgwtMin[3])))
        goto L_b65b;
    else
        goto L_b64a;

L_b64a:
    cKillPeople = lppl->rgwtMin[3];

L_b65b:
    if ((cKillPeople <= 0))
        goto L_b683;
    else
        goto L_b672;

L_b672:
    lppl->rgwtMin[3] = (lppl->rgwtMin[3] - cKillPeople);

L_b683:
    if ((cKillFact <= 0))
        goto L_b6f1;
    else
        goto L_b69a;

L_b69a:
    lppl->cFactories = (lppl->cFactories - cKillFact);

L_b6f1:
    if ((cKillMine <= 0))
        goto L_b761;
    else
        goto L_b708;

L_b708:
    lppl->cMines = (lppl->cMines - cKillMine);

L_b761:
    if ((cKillDefenses <= 0))
        goto L_b7d1;
    else
        goto L_b778;

L_b778:
    lppl->cDefenses = (lppl->cDefenses - cKillDefenses);

L_b7d1:
    if ((pctTerra <= 0))
        goto L_b9d4;
    else
        goto L_b7e8;

L_b7e8:
    pctTot = 0;
    pctTerra = (pctTerra - __ftol());
    if ((pctTerra <= 500))
        goto L_b84b;
    else
        goto L_b841;

L_b841:
    pctTerra = 500;

L_b84b:
    i = 0;
    goto L_b930;

L_b853:
    dChg = ((uint16_t)(lppl->rgEnvVar[i]) - (uint16_t)(lppl->rgEnvVarOrig[i]));
    if ((dChg <= 0))
        goto L_b8d8;
    else
        goto L_b895;

L_b895:
    if (((uint32_t)(dChg) < pctTerra))
        goto L_b8b7;
    else
        goto L_b8ae;

L_b8ae:
    dChg = LOWORD(pctTerra);

L_b8b7:
    lppl->rgEnvVar[i] = (lppl->rgEnvVar[i] - LOBYTE(dChg));
    pctTot = (pctTot + dChg);
    goto L_b92c;

L_b8d8:
    if ((dChg >= 0))
        goto L_b92c;
    else
        goto L_b8e1;

L_b8e1:
    if (((uint32_t)((-dChg)) < pctTerra))
        goto L_b90c;
    else
        goto L_b8fc;

L_b8fc:
    dChg = (-LOWORD(pctTerra));

L_b90c:
    lppl->rgEnvVar[i] = (lppl->rgEnvVar[i] - LOBYTE(dChg));
    pctTot = (pctTot + (-dChg));

L_b92c:
    i = (i + 1);

L_b930:
    if ((i < 3))
        goto L_b853;
    else
        goto L_b939;

L_b939:
    if ((pctTot <= 0))
        goto L_b9d4;
    else
        goto L_b942;

L_b942:
    if ((fMulti == 0))
        goto L_b97a;
    else
        goto L_b974;

L_b974:
    t_merge_b97d_0001 = 378;
    goto L_b97d;

L_b97a:
    t_merge_b97d_0001 = 302;

L_b97d:
    FSendPlrMsg(lpfl->iPlayer, t_merge_b97d_0001, (lpfl->id | 0x8000), lpfl->id, lppl->id, pctTot, 0, 0, 0, 0);
    if ((fMulti == 0))
        goto L_b9c1;
    else
        goto L_b9bb;

L_b9bb:
    t_merge_b9c4_0001 = 379;
    goto L_b9c4;

L_b9c1:
    t_merge_b9c4_0001 = 302;

L_b9c4:
    FSendPlrMsg(lppl->iPlayer, t_merge_b9c4_0001, lppl->id, lpfl->id, lppl->id, pctTot, 0, 0, 0, 0);

L_b9d4:
    cPPE = ((cKillMine + cKillFact) + cKillDefenses);
    if ((cPPE <= 0))
        goto L_bd61;
    else
        goto L_ba03;

L_ba03:
    if ((HIWORD(lppl->rgwtMin[3]) < 0x0))
        goto L_ba6b;
    else
        goto L_ba10;

L_ba10:
    if ((HIWORD(lppl->rgwtMin[3]) > 0x0))
        goto L_ba1f;
    else
        goto L_ba15;

L_ba15:
    if ((LOWORD(lppl->rgwtMin[3]) <= 0x0))
        goto L_ba6b;
    else
        goto L_ba1f;

L_ba1f:
    if ((fMulti == 0))
        goto L_ba2e;
    else
        goto L_ba28;

L_ba28:
    t_merge_ba31_0001 = 0x169;
    goto L_ba31;

L_ba2e:
    t_merge_ba31_0001 = 0x63;

L_ba31:
    idmSrc = t_merge_ba31_0001;
    if ((fMulti == 0))
        goto L_ba43;
    else
        goto L_ba3d;

L_ba3d:
    t_merge_ba46_0001 = 0x173;
    goto L_ba46;

L_ba43:
    t_merge_ba46_0001 = 0x6d;

L_ba46:
    idmDst = t_merge_ba46_0001;
    if ((cPPE <= 1))
        goto L_ba98;
    else
        goto L_ba60;

L_ba60:
    idmSrc = (idmSrc + 1);
    idmDst = (idmDst + 1);

L_ba68:
    goto L_ba98;

L_ba6b:
    if ((fMulti == 0))
        goto L_ba7a;
    else
        goto L_ba74;

L_ba74:
    t_merge_ba7d_0001 = 0x17c;
    goto L_ba7d;

L_ba7a:
    t_merge_ba7d_0001 = 0x8f;

L_ba7d:
    idmSrc = t_merge_ba7d_0001;
    if ((fMulti == 0))
        goto L_ba8f;
    else
        goto L_ba89;

L_ba89:
    t_merge_ba92_0001 = 0x17d;
    goto L_ba92;

L_ba8f:
    t_merge_ba92_0001 = 0x90;

L_ba92:
    idmDst = t_merge_ba92_0001;
    goto GenericBombMsg;

L_ba98:
    if ((cKillPeople <= 0))
        goto L_bc0a;
    else
        goto L_baaf;

L_baaf:
    if ((pctSuccess != 1))
        goto L_bb47;
    else
        goto GenericBombMsg;

GenericBombMsg:
    FSendPlrMsg(lpfl->iPlayer, idmSrc, (lpfl->id | 0x8000), lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), 0, 0, 0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), 0, 0, 0);
    goto L_be65;

L_bb47:
    idmSrc = (idmSrc + 5);
    idmDst = (idmDst + 5);
    FSendPlrMsg(lpfl->iPlayer, idmSrc, (lpfl->id | 0x8000), lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), __ftol(), 0, 0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), __ftol(), 0, 0);

L_bc07:
    goto L_be65;

L_bc0a:
    idmSrc = (idmSrc - 2);
    idmDst = (idmDst - 2);
    if ((pctSuccess != 1))
        goto L_bca4;
    else
        goto L_bc27;

L_bc27:
    FSendPlrMsg(lpfl->iPlayer, idmSrc, (lpfl->id | 0x8000), lpfl->id, lppl->id, LOWORD(cPPE), 0, 0, 0, 0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cPPE), 0, 0, 0, 0);
    goto L_be65;

L_bca4:
    idmSrc = (idmSrc + 5);
    idmDst = (idmDst + 5);
    FSendPlrMsg(lpfl->iPlayer, idmSrc, (lpfl->id | 0x8000), lpfl->id, lppl->id, LOWORD(cPPE), __ftol(), 0, 0, 0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cPPE), __ftol(), 0, 0, 0);

L_bd5e:
    goto L_be65;

L_bd61:
    if ((cKillPeople <= 0))
        goto L_be65;
    else
        goto L_bd78;

L_bd78:
    if ((HIWORD(lppl->rgwtMin[3]) < 0x0))
        goto L_bdc1;
    else
        goto L_bd85;

L_bd85:
    if ((HIWORD(lppl->rgwtMin[3]) > 0x0))
        goto L_bd94;
    else
        goto L_bd8a;

L_bd8a:
    if ((LOWORD(lppl->rgwtMin[3]) <= 0x0))
        goto L_bdc1;
    else
        goto L_bd94;

L_bd94:
    if ((fMulti == 0))
        goto L_bda3;
    else
        goto L_bd9d;

L_bd9d:
    t_merge_bda6_0001 = 0x166;
    goto L_bda6;

L_bda3:
    t_merge_bda6_0001 = 0x60;

L_bda6:
    idmSrc = t_merge_bda6_0001;
    if ((fMulti == 0))
        goto L_bdb8;
    else
        goto L_bdb2;

L_bdb2:
    t_merge_bdbb_0001 = 0x170;
    goto L_bdbb;

L_bdb8:
    t_merge_bdbb_0001 = 0x6a;

L_bdbb:
    idmDst = t_merge_bdbb_0001;
    goto L_bdeb;

L_bdc1:
    if ((fMulti == 0))
        goto L_bdd0;
    else
        goto L_bdca;

L_bdca:
    t_merge_bdd3_0001 = 0x17c;
    goto L_bdd3;

L_bdd0:
    t_merge_bdd3_0001 = 0x8f;

L_bdd3:
    idmSrc = t_merge_bdd3_0001;
    if ((fMulti == 0))
        goto L_bde5;
    else
        goto L_bddf;

L_bddf:
    t_merge_bde8_0001 = 0x17d;
    goto L_bde8;

L_bde5:
    t_merge_bde8_0001 = 0x90;

L_bde8:
    idmDst = t_merge_bde8_0001;

L_bdeb:
    FSendPlrMsg(lpfl->iPlayer, idmSrc, (lpfl->id | 0x8000), lpfl->id, lppl->id, LOWORD(cKillPeople), 0, 0, 0, 0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), 0, 0, 0, 0);

L_be65:
    if ((LOWORD(lppl->rgwtMin[3]) != 0x0))
        goto L_af0b;
    else
        goto L_be72;

L_be72:
    if ((HIWORD(lppl->rgwtMin[3]) != 0x0))
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
