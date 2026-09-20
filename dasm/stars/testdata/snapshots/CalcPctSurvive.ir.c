void CalcPctSurvive(PLANET *lppl, float *ppct, float *ppctSmart) {
    int16_t iPlrSav;
    int32_t cDefenses;
    float   pct;
    PART    part;
    int16_t cMax;

L_02f6:
    if ((ppctSmart == 0x0))
        goto L_0317;
    else
        goto L_0308;

L_0308:
    *(ppctSmart) = 1;

L_0317:
    if ((lppl->iPlayer == -1))
        goto L_0487;
    else
        goto L_0324;

L_0324:
    if ((lppl->cDefenses != 0x0))
        goto L_0345;
    else
        goto L_0487;

L_0345:
    iPlrSav = idPlayer;
    idPlayer = lppl->iPlayer;
    if ((FGetBestDefensePart(&(part)) == 0))
        goto L_0473;
    else
        goto L_0369;

L_0369:
    cDefenses = lppl->cDefenses;
    cMax = CMaxOperableDefenses(lppl, lppl->iPlayer, 0);
    if (((int32_t)(cMax) < cDefenses))
        goto L_03b5;
    else
        goto L_03bf;

L_03b5:
    cDefenses = (int32_t)(cMax);

L_03bf:
    pct = pow((1 - ((double)((int32_t)(part.pplanetary->grAbility)) / 1000)), (double)(cDefenses));
    if ((ppctSmart == 0x0))
        goto L_047e;
    else
        goto L_041a;

L_041a:
    *(ppctSmart) = pow((1 - ((double)((int32_t)(part.pplanetary->grAbility)) / 2000)), (double)(cDefenses));

L_0470:
    goto L_047e;

L_0473:
    pct = 1;

L_047e:
    idPlayer = iPlrSav;
    goto L_0492;

L_0487:
    pct = 1;

L_0492:
    *(ppct) = pct;
    return;
}
