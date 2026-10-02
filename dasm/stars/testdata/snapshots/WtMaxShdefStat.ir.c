int16_t WtMaxShdefStat(SHDEF *lpshdef, int16_t grStat) {
    int16_t wt;
    int16_t j;
    HUL    *lphul;

L_41b2:
    lphul = &lpshdef->hul;
    goto L_4496;

L_41cd:
    wt = LphuldefFromId(lphul->ihuldef)->hul.wtFuelMax;
    j = 0;
    goto L_41f2;

L_41ee:
    j++;

L_41f2:
    if (j >= lphul->chs)
        goto L_44a9;
    else
        goto L_4204;

L_4204:
    if (lphul->rghs[j].grhst != hstSpecialM)
        goto L_42c8;
    else
        goto L_4224;

L_4224:
    if (lphul->rghs[j].iItem != ispecialMFuelTank)
        goto L_4276;
    else
        goto L_4249;

L_4249:
    wt += lphul->rghs[j].cItem * 250;
    goto L_41ee;

L_4276:
    if (lphul->rghs[j].iItem != ispecialMSuperFuelTank)
        goto L_41ee;
    else
        goto L_429b;

L_429b:
    wt += lphul->rghs[j].cItem * 500;

L_42c5:
    goto L_41ee;

L_42c8:
    if (lphul->rghs[j].grhst != hstSpecialE)
        goto L_41ee;
    else
        goto L_42e8;

L_42e8:
    if (lphul->rghs[j].iItem != ispecialEAntiMatterGenerator)
        goto L_41ee;
    else
        goto L_430d;

L_430d:
    wt += lphul->rghs[j].cItem * 200;

L_4337:
    goto L_41ee;

L_433d:
    wt = LphuldefFromId(lphul->ihuldef)->hul.wtCargoMax;
    j = 0;
    goto L_4362;

L_435e:
    j++;

L_4362:
    if (j >= lphul->chs)
        goto L_44a9;
    else
        goto L_4374;

L_4374:
    if (lphul->rghs[j].grhst != hstSpecialM)
        goto L_435e;
    else
        goto L_4394;

L_4394:
    if (lphul->rghs[j].iItem != ispecialMCargoPod)
        goto L_43e6;
    else
        goto L_43b9;

L_43b9:
    wt += lphul->rghs[j].cItem * 50;
    goto L_435e;

L_43e6:
    if (lphul->rghs[j].iItem != ispecialMSuperCargoPod)
        goto L_4438;
    else
        goto L_440b;

L_440b:
    wt += lphul->rghs[j].cItem * 100;
    goto L_435e;

L_4438:
    if (lphul->rghs[j].iItem != ispecialMMultiCargoPod)
        goto L_435e;
    else
        goto L_445d;

L_445d:
    wt += lphul->rghs[j].cItem * 250;

L_4487:
    goto L_435e;

L_448d:
    return 0;

L_4496:
    if (grStat == 1)
        goto L_41cd;
    else
        goto L_449e;

L_449e:
    if (grStat != 2)
        goto L_448d;
    else
        goto L_44a3;

L_44a3:
    goto L_433d;

L_44a9:
    return wt;
}
