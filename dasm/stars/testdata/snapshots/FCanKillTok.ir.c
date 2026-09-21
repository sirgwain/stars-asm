int16_t FCanKillTok(TOK *ptok1, TOK *ptok2) {
    int32_t lp1;
    int32_t lp2;

L_391e:
    lp1 = LpshdefFromTok(ptok1)->lPower;
    lp2 = LpshdefFromTok(ptok2)->lPower;
    if ((lp2 <= lp1))
        goto L_398f;
    else
        goto L_3986;

L_3986:
    return 0;

L_398f:
    if (((lp2 & 0x7ffff000) < (lp1 & 0x7ffff000)))
        goto L_39bd;
    else
        goto L_39c6;

L_39bd:
    return 1;

L_39c6:
    if (((lp2 & 0x7fffff00) != (lp1 & 0x7fffff00)))
        goto L_3a1a;
    else
        goto L_39ef;

L_39ef:
    if ((ptok1->spd < ptok2->spd))
        goto L_3a1a;
    else
        goto L_3a14;

L_3a14:
    return 1;

L_3a1a:
    return 0;
}
