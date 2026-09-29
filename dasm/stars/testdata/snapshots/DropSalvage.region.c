void DropSalvage(THING **plpth, int32_t *rgwtMinerals, int16_t iplr, POINT16 *ppt) {
    int32_t wtTotal;
    int32_t wt;
    int16_t i;
    THING  *lpth;

    lpth = *plpth;
    wtTotal = 0;
    for (i = 0; i < game.cPlanMax; i++) {
        if (ppt->x == rgptPlan[i].x && ppt->y == rgptPlan[i].y) {
            return;
        }
    }
    for (i = 0; i < 3; i++) {
        wtTotal = wtTotal + rgwtMinerals[i];
    }
    while (wtTotal == 0) {
        for (i = 0; i < 3; i++) {
            rgwtMinerals[i] = (int32_t)Random(10);
            wtTotal = wtTotal + rgwtMinerals[i];
        }
    }
    if (lpth != 0x0) {
        for (i = 0; i < 3; i++) {
            rgwtMinerals[i] = rgwtMinerals[i] + (int32_t)lpth->thp.rgwtMin[i];
            wtTotal = wtTotal + (int32_t)lpth->thp.rgwtMin[i];
            lpth->thp.rgwtMin[i] = 0;
        }
        lpth->thp.wtMax = 0x0;
    } else {
        lpth = LpthNew(iplr, ithMineralPacket);
        if (lpth == 0x0) {
            return;
        }
        lpth->thp.iWarp = 0x0;
        lpth->pt.x = ppt->x;
        lpth->pt.y = ppt->y;
        lpth->thp.idPlanet = 0x3ff;
    }
    lpth->thp.fMoved = 0x1;
    while (wtTotal > 0) {
        for (i = 0; i < 3; i++) {
            if ((uint32_t)(lpth->thp.wtMax * 0xa) + rgwtMinerals[i] <= 0x7530) {
                lpth->thp.wtMax = lpth->thp.wtMax + (rgwtMinerals[i] + 9) / 0xa;
                lpth->thp.rgwtMin[i] = lpth->thp.rgwtMin[i] + LOWORD(rgwtMinerals[i]);
                wtTotal = wtTotal - rgwtMinerals[i];
                rgwtMinerals[i] = 0;
            } else {
                wt = 30000 - (uint32_t)(lpth->thp.wtMax * 0xa);
                wtTotal = wtTotal - wt;
                lpth->thp.wtMax = 0xbb8;
                lpth->thp.rgwtMin[i] = lpth->thp.rgwtMin[i] + LOWORD(wt);
                rgwtMinerals[i] = rgwtMinerals[i] - wt;
                lpth = LpthNew(iplr, ithMineralPacket);
                if (lpth == 0x0) {
                    return;
                }
                lpth->thp.iWarp = 0x0;
                lpth->thp.idPlanet = 0x3ff;
                lpth->pt.x = ppt->x;
                lpth->pt.y = ppt->y;
            }
            if (wtTotal <= 0)
                break;
        }
    }
    *plpth = lpth;
    return;
}
