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
    if ((lppl->iPlayer == 0xffff))
        goto L_0487;
    else
        goto L_0324;

L_0324:
    if ((lppl->cDefenses != 0x0))
        goto L_0345;
    else
        goto L_033d;

L_033d:
    if ((0x0 == 0x0))
        goto L_0487;
    else
        goto L_0345;

L_0345:
    iPlrSav = idPlayer;
    idPlayer = lppl->iPlayer;
    if ((FGetBestDefensePart(&(part)) == 0x0))
        goto L_0473;
    else
        goto L_0369;

L_0369:
    LOWORD(cDefenses) = lppl->cDefenses;
    HIWORD(cDefenses) = 0x0;
    cMax = CMaxOperableDefenses(lppl, lppl->iPlayer, 0x0);
    if ((SIGNHIWORD(cMax) > HIWORD(cDefenses)))
        goto L_03bf;
    else
        goto L_03a8;

L_03a8:
    if ((SIGNHIWORD(cMax) < HIWORD(cDefenses)))
        goto L_03b5;
    else
        goto L_03ad;

L_03ad:
    if ((cMax >= LOWORD(cDefenses)))
        goto L_03bf;
    else
        goto L_03b5;

L_03b5:
    cDefenses = (uint32_t)(cMax);

L_03bf:
    pct = pow((1 - ((double)((int32_t)(part.pplanetary->grAbility)) / 1000)), (double)(cDefenses));
    if ((ppctSmart == 0x0))
        goto L_047e;
    else
        goto L_041a;

L_041a:
    *(ppctSmart) = pow((1 - ((double)((int32_t)(part.pplanetary->grAbility)) / 2000)), (double)(cDefenses));

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
