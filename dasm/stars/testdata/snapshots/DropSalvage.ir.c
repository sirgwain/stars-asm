void DropSalvage(THING **plpth, int32_t *rgwtMinerals, int16_t iplr, POINT *ppt) {
    int32_t wtTotal;
    int32_t wt;
    int16_t i;
    THING  *lpth;

L_24dc:
    lpth = *(plpth);
    wtTotal = 0;
    i = 0;
    goto L_2509;

L_2505:
    i = (i + 1);

L_2509:
    if ((i >= game.cPlanMax))
        goto L_253b;
    else
        goto L_2514;

L_2514:
    if ((ppt->x != rgptPlan[i].x))
        goto L_2505;
    else
        goto L_252d;

L_252d:
    if ((ppt->y == rgptPlan[i].y))
        goto L_294c;
    else
        goto L_2532;

L_2532:
    goto L_2505;

L_253b:
    i = 0;
    goto L_2565;

L_2543:
    wtTotal = (wtTotal + rgwtMinerals[i]);
    i = (i + 1);

L_2565:
    if ((i >= 3))
        goto L_25cb;
    else
        goto L_256b;

L_256b:
    goto L_2543;

L_2571:
    i = 0;
    goto L_25c2;

L_2579:
    rgwtMinerals[i] = (int32_t)(Random(10));
    wtTotal = (wtTotal + rgwtMinerals[i]);
    i = (i + 1);

L_25c2:
    if ((i < 3))
        goto L_2579;
    else
        goto L_25cb;

L_25cb:
    if ((wtTotal != 0))
        goto L_25dd;
    else
        goto L_2571;

L_25dd:
    if ((lpth != 0x0))
        goto L_2657;
    else
        goto L_25ef;

L_25ef:
    lpth = LpthNew(iplr, ithMineralPacket);
    if ((lpth != 0x0))
        goto L_2619;
    else
        goto L_294c;

L_2619:
    lpth->thp.iWarp = 0x0;
    lpth->pt.x = ppt->x;
    lpth->pt.y = ppt->y;
    lpth->thp.idPlanet = 0x3ff;
    goto L_26e9;

L_2657:
    i = 0;
    goto L_26cc;

L_265f:
    rgwtMinerals[i] = (rgwtMinerals[i] + (int32_t)(lpth->thp.rgwtMin[i]));
    wtTotal = (wtTotal + (int32_t)(lpth->thp.rgwtMin[i]));
    lpth->thp.rgwtMin[i] = 0;
    i = (i + 1);

L_26cc:
    if ((i < 3))
        goto L_265f;
    else
        goto L_26d5;

L_26d5:
    lpth->thp.wtMax = 0x0;

L_26e9:
    lpth->thp.fMoved = 0x1;
    goto L_2927;

L_2700:
    i = 0;
    goto L_291e;

L_2708:
    if ((((uint32_t)((lpth->thp.wtMax * 0xa)) + rgwtMinerals[i]) <= 0x7530))
        goto L_2842;
    else
        goto L_2753;

L_2753:
    wt = (30000 - (uint32_t)((lpth->thp.wtMax * 0xa)));
    wtTotal = (wtTotal - wt);
    lpth->thp.wtMax = 0xbb8;
    lpth->thp.rgwtMin[i] = (lpth->thp.rgwtMin[i] + LOWORD(wt));
    rgwtMinerals[i] = (rgwtMinerals[i] - wt);
    lpth = LpthNew(iplr, ithMineralPacket);
    if ((lpth != 0x0))
        goto L_2804;
    else
        goto L_294c;

L_2804:
    lpth->thp.iWarp = 0x0;
    lpth->thp.idPlanet = 0x3ff;
    lpth->pt.x = ppt->x;
    lpth->pt.y = ppt->y;
    goto L_2900;

L_2842:
    lpth->thp.wtMax = (lpth->thp.wtMax + ((rgwtMinerals[i] + 9) / 0xa));
    lpth->thp.rgwtMin[i] = (lpth->thp.rgwtMin[i] + LOWORD(rgwtMinerals[i]));
    wtTotal = (wtTotal - rgwtMinerals[i]);
    rgwtMinerals[i] = 0;

L_2900:
    if ((wtTotal <= 0))
        goto L_2927;
    else
        goto L_291a;

L_291a:
    i = (i + 1);

L_291e:
    if ((i < 3))
        goto L_2708;
    else
        goto L_2927;

L_2927:
    if ((wtTotal <= 0))
        goto L_293e;
    else
        goto L_2700;

L_293e:
    *(plpth) = lpth;

L_294c:
    return;
}
