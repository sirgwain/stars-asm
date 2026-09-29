int16_t FFleetMightHaveTeeth(FLEET *lpfl) {
    HUL    *lphul;
    int16_t ishdef;

L_6152:
    ishdef = 0;
    goto L_61c8;

L_6163:
    if (lpfl->rgcsh[ishdef] == 0)
        goto L_61c4;
    else
        goto L_6180;

L_6180:
    lphul = &rglpshdef[lpfl->iplr][ishdef].hul;
    if (FHullHasTeeth(lphul) == 0)
        goto L_61c4;
    else
        goto L_61be;

L_61be:
    return 1;

L_61c4:
    ishdef = ishdef + 1;

L_61c8:
    if (ishdef < 16)
        goto L_6163;
    else
        goto L_61d1;

L_61d1:
    return 0;
}
