void DropSalvage(THING **plpth, int32_t *rgwtMinerals, int16_t iplr, POINT *ppt) {
    int32_t wtTotal;
    int32_t wt;
    int16_t i;
    THING  *lpth;

L_24dc:
    lpth = farptr(*(plpth + 0x2), plpth);
    wtTotal = 0x0;
    i = 0x0;
    goto L_2509;

L_2505:
    i = (i + 0x1);

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

L_253b:
    i = 0x0;
    goto L_2565;

L_2543:
    wtTotal = (wtTotal + rgwtMinerals[i]);
    i = (i + 0x1);

L_2565:
    if ((i >= 0x3))
        goto L_25cb;
    else
        goto L_256b;

L_256b:

L_2571:
    i = 0x0;
    goto L_25c2;

L_2579:
    rgwtMinerals[i] = (uint32_t)(Random(0xa));
    wtTotal = (wtTotal + rgwtMinerals[i]);
    i = (i + 0x1);

L_25c2:
    if ((i < 0x3))
        goto L_2579;
    else
        goto L_25cb;

L_25cb:
    if ((LOWORD(wtTotal) != 0x0))
        goto L_25dd;
    else
        goto L_25d4;

L_25d4:
    if ((HIWORD(wtTotal) == 0x0))
        goto L_2571;
    else
        goto L_25dd;

L_25dd:
    if ((LOWORD(lpth) != 0x0))
        goto L_2657;
    else
        goto L_25e6;

L_25e6:
    if ((HIWORD(lpth) != 0x0))
        goto L_2657;
    else
        goto L_25ef;

L_25ef:
    lpth = LpthNew(iplr, ithMineralPacket);
    if ((LOWORD(lpth) != 0x0))
        goto L_2619;
    else
        goto L_260d;

L_260d:
    if ((HIWORD(lpth) == 0x0))
        goto L_294c;
    else
        goto L_2613;

L_2613:

L_2619:
    LOWORD(lpth->thp) = ((LOWORD(lpth->thp) & 0xc3ff) | 0x0);
    lpth->pt.x = ppt->x;
    lpth->pt.y = ppt->y;
    LOWORD(lpth->thp) = ((LOWORD(lpth->thp) & 0xfc00) | 0x3ff);
    goto L_26e9;

L_2657:
    i = 0x0;
    goto L_26cc;

L_265f:
    /* untranslated: LOWORD(rgwtMinerals[i]) = (LOWORD(rgwtMinerals[i]) + HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) */
    /* untranslated: HIWORD(rgwtMinerals[i]) = (HIWORD(rgwtMinerals[i]) + signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))])) */
    /* untranslated: LOWORD(wtTotal) = (LOWORD(wtTotal) + HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) */
    /* untranslated: HIWORD(wtTotal) = (HIWORD(wtTotal) + signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))])) */
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] = 0x0 */
    i = (i + 0x1);

L_26cc:
    if ((i < 0x3))
        goto L_265f;
    else
        goto L_26d5;

L_26d5:
    lpth->thp.wtMax = 0x0;

L_26e9:
    LOWORD(lpth->thp) = ((LOWORD(lpth->thp) & 0xbfff) | 0x4000);
    goto L_2927;

L_2700:
    i = 0x0;
    goto L_291e;

L_2708:
    /* untranslated: branch (hiword((uint32_t)(words(0x0, lpth->thp.wtMax) * 0xa)) + HIWORD(rgwtMinerals[i])) < 0x0 ? L_2842 : L_2746 */

L_2746:
    /* untranslated: branch (hiword(callresult(uint32_t)) + HIWORD(rgwtMinerals[i])) > 0x0 ? L_2753 : L_274b */

L_274b:
    /* untranslated: branch (loword(callresult(uint32_t)) + LOWORD(rgwtMinerals[i])) <= 0x7530 ? L_2842 : L_2753 */

L_2753:
    /* untranslated: wt = (0x7530 - (uint32_t)(words(0x0, lpth->thp.wtMax) * 0xa)) */
    wtTotal = (wtTotal - wt);
    lpth->thp.wtMax = 0xbb8;
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] = (HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] + LOWORD(wt)) */
    rgwtMinerals[i] = (rgwtMinerals[i] - wt);
    lpth = LpthNew(iplr, ithMineralPacket);
    if ((LOWORD(lpth) != 0x0))
        goto L_2804;
    else
        goto L_27f8;

L_27f8:
    if ((HIWORD(lpth) == 0x0))
        goto L_294c;
    else
        goto L_27fe;

L_27fe:

L_2804:
    LOWORD(lpth->thp) = ((LOWORD(lpth->thp) & 0xc3ff) | 0x0);
    LOWORD(lpth->thp) = ((LOWORD(lpth->thp) & 0xfc00) | 0x3ff);
    lpth->pt.x = ppt->x;
    lpth->pt.y = ppt->y;
    goto L_2900;

L_2842:
    /* untranslated: ss:[bp-0x12] = ((loword((int32_t)(words((HIWORD(rgwtMinerals[i]) + 0x0), (LOWORD(rgwtMinerals[i]) + 0x9)) / 0xa)) + *(lpth+0xe)) & 0x3fff)
     */
    lpth->thp.wtMax = 0x0;
    /* untranslated: *(lpth+0xe) = (*(lpth+0xe) | ss:[bp-0x12]) */
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] = (HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] + LOWORD(rgwtMinerals[i])) */
    wtTotal = (wtTotal - rgwtMinerals[i]);
    rgwtMinerals[i] = 0x0;

L_2900:
    if ((HIWORD(wtTotal) > 0x0))
        goto L_291a;
    else
        goto L_2909;

L_2909:
    if ((HIWORD(wtTotal) < 0x0))
        goto L_2927;
    else
        goto L_290e;

L_290e:
    if ((LOWORD(wtTotal) <= 0x0))
        goto L_2927;
    else
        goto L_2914;

L_2914:

L_291a:
    i = (i + 0x1);

L_291e:
    if ((i < 0x3))
        goto L_2708;
    else
        goto L_2927;

L_2927:
    if ((HIWORD(wtTotal) > 0x0))
        goto L_2700;
    else
        goto L_2930;

L_2930:
    if ((HIWORD(wtTotal) < 0x0))
        goto L_293e;
    else
        goto L_2935;

L_2935:
    if ((LOWORD(wtTotal) > 0x0))
        goto L_2700;
    else
        goto L_293e;

L_293e:
    plpth = LOWORD(lpth);
    *(plpth + 0x2) = HIWORD(lpth);

L_294c:
    return;
}
