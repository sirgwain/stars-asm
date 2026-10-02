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
        wtTotal += rgwtMinerals[i];
    }
    while (wtTotal == 0) {
        for (i = 0; i < 3; i++) {
            rgwtMinerals[i] = Random(10);
            wtTotal += rgwtMinerals[i];
        }
    }
    if (lpth == 0) {
        lpth = LpthNew(iplr, ithMineralPacket);
        if (lpth == 0) {
            return;
        }
        lpth->thp.iWarp = 0;
        lpth->pt.x = ppt->x;
        lpth->pt.y = ppt->y;
        lpth->thp.idPlanet = 0x3ff;
    } else {
        for (i = 0; i < 3; i++) {
            rgwtMinerals[i] += lpth->thp.rgwtMin[i];
            wtTotal += lpth->thp.rgwtMin[i];
            lpth->thp.rgwtMin[i] = 0;
        }
        lpth->thp.wtMax = 0;
    }
    lpth->thp.fMoved = TRUE;
    while (wtTotal > 0) {
        for (i = 0; i < 3; i++) {
            if ((uint32_t)(lpth->thp.wtMax * 10) + rgwtMinerals[i] > 30000) {
                wt = 30000 - (uint32_t)(lpth->thp.wtMax * 10);
                wtTotal -= wt;
                lpth->thp.wtMax = 3000;
                lpth->thp.rgwtMin[i] += LOWORD(wt);
                rgwtMinerals[i] -= wt;
                lpth = LpthNew(iplr, ithMineralPacket);
                if (lpth == 0) {
                    return;
                }
                lpth->thp.iWarp = 0;
                lpth->thp.idPlanet = 0x3ff;
                lpth->pt.x = ppt->x;
                lpth->pt.y = ppt->y;
            } else {
                lpth->thp.wtMax += (rgwtMinerals[i] + 9) / 10;
                lpth->thp.rgwtMin[i] += LOWORD(rgwtMinerals[i]);
                wtTotal -= rgwtMinerals[i];
                rgwtMinerals[i] = 0;
            }
            if (wtTotal <= 0)
                break;
        }
    }
    *plpth = lpth;
    return;
}
