int16_t FCanKillTok(TOK *ptok1, TOK *ptok2) {
    int32_t lp1;
    int32_t lp2;

L_391e:
    lp1 = LpshdefFromTok(ptok1)->lPower;
    lp2 = LpshdefFromTok(ptok2)->lPower;
    if ((HIWORD(lp2) < HIWORD(lp1)))
        goto L_398f;
    else
        goto L_3979;

L_3979:
    if ((HIWORD(lp2) > HIWORD(lp1)))
        goto L_3986;
    else
        goto L_397e;

L_397e:
    if ((LOWORD(lp2) <= LOWORD(lp1)))
        goto L_398f;
    else
        goto L_3986;

L_3986:
    return 0;

L_398f:
    if (((HIWORD(lp2) & 0x7fff) > (HIWORD(lp1) & 0x7fff)))
        goto L_39c6;
    else
        goto L_39b1;

L_39b1:
    if (((HIWORD(lp2) & 0x7fff) < (HIWORD(lp1) & 0x7fff)))
        goto L_39bd;
    else
        goto L_39b6;

L_39b6:
    if (((LOWORD(lp2) & 0xf000) >= (LOWORD(lp1) & 0xf000)))
        goto L_39c6;
    else
        goto L_39bd;

L_39bd:
    return 1;

L_39c6:
    if (((LOWORD(lp2) & 0xff00) != (LOWORD(lp1) & 0xff00)))
        goto L_3a1a;
    else
        goto L_39e8;

L_39e8:
    if (((HIWORD(lp2) & 0x7fff) != (HIWORD(lp1) & 0x7fff)))
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
