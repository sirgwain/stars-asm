int32_t LCalcFuelGainFromRamScoops(FLEET *lpfl, int16_t iWarp, int32_t dTravel) {
    int16_t  i;
    int16_t *rgiFuel;
    SHDEF   *lpshdef;
    int32_t  pct10;
    int32_t  pctShip10;

L_56b8:
    pct10 = 0;
    if ((iWarp <= 10))
        goto L_56dd;
    else
        goto L_56d4;

L_56d4:
    return 0x0;

L_56dd:
    i = 0;
    lpshdef = rglpshdef[lpfl->iPlayer];
    goto L_5712;

L_56fe:
    i = (i + 1);
    lpshdef = (lpshdef + 0x93);

L_5712:
    if ((i >= 16))
        goto L_5880;
    else
        goto L_571b;

L_571b:
    if ((lpfl->rgcsh[i] == 0))
        goto L_56fe;
    else
        goto L_5738;

L_5738:
    rgiFuel = (LpengineFromId(lpshdef->hul.rghs[0x0].iItem) + 0x36);
    pctShip10 = 0;
    if ((iWarp > 9))
        goto L_56fe;
    else
        goto L_5766;

L_5766:

L_576c:
    if ((rgiFuel[iWarp] != 0))
        goto L_5852;
    else
        goto L_5784;

L_5784:
    pctShip10 = (pctShip10 + lpshdef->hul.rghs[0x0].cItem);
    if ((rgiFuel[(iWarp + 1)] != 0))
        goto L_5852;
    else
        goto L_57b7;

L_57b7:
    pctShip10 = (pctShip10 + (uint32_t)((lpshdef->hul.rghs[0x0].cItem * 0x2)));
    if ((iWarp >= 9))
        goto L_5852;
    else
        goto L_57da;

L_57da:
    if ((rgiFuel[(iWarp + 2)] != 0))
        goto L_5852;
    else
        goto L_57f5;

L_57f5:
    pctShip10 = (pctShip10 + (uint32_t)(LOWORD((lpshdef->hul.rghs[0x0].cItem * 0x3))));
    if ((iWarp >= 8))
        goto L_5852;
    else
        goto L_581b;

L_581b:
    if ((rgiFuel[(iWarp + 3)] != 0))
        goto L_5852;
    else
        goto L_5836;

L_5836:
    pctShip10 = (pctShip10 + (uint32_t)((lpshdef->hul.rghs[0x0].cItem * 0x4)));

L_5852:
    pct10 = (pct10 + (uint32_t)((pctShip10 * (uint32_t)(lpfl->rgcsh[i]))));

L_5880:
    pct10 = (uint32_t)((pct10 * dTravel));
    return pct10;
}
