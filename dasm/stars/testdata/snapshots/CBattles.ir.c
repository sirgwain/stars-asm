int16_t CBattles() {
    BTLDATA *lpbd;
    HB      *lphb;
    int16_t  cBattles;

L_028c:
    cBattles = 0;
    lphb = rglphb[11];
    if (lphb != 0x0)
        goto L_02bf;
    else
        goto L_02b9;

L_02b9:
    return 0;

L_02bf:
    lpbd = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));

L_02d3:
    if (lpbd->id != 0xffff)
        goto L_0329;
    else
        goto L_02df;

L_02df:
    lphb = lphb->lphbNext;
    if (lphb != 0x0)
        goto L_0302;
    else
        goto L_030f;

L_0302:
    if (lphb->ibTop > sizeof(HB))
        goto L_0315;
    else
        goto L_030f;

L_030f:
    return cBattles;

L_0315:
    lpbd = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
    goto L_02d3;

L_0329:
    if (lpbd->cbData != 0x0)
        goto L_033f;
    else
        goto L_0336;

L_0336:
    return cBattles;

L_033f:
    lpbd = (BTLDATA *)((uint8_t *)lpbd + lpbd->cbData);
    cBattles = cBattles + 1;

L_0358:
    goto L_02d3;
}
