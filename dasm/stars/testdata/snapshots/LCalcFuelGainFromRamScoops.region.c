int32_t LCalcFuelGainFromRamScoops(FLEET *lpfl, int16_t iWarp, int32_t dTravel) {
    int16_t  i;
    int16_t *rgiFuel;
    SHDEF   *lpshdef;
    int32_t  pct10;
    int32_t  pctShip10;

    pct10 = 0;
    if (iWarp <= 10) {
        i = 0;
        lpshdef = rglpshdef[lpfl->iPlayer];
        while (i < 16) {
            if (lpfl->rgcsh[i] != 0) {
                rgiFuel = LpengineFromId(lpshdef->hul.rghs[0].iItem)->rgcFuelUsed;
                pctShip10 = 0;
                if (iWarp <= 9) {
                    if (rgiFuel[iWarp] == 0) {
                        pctShip10 = pctShip10 + lpshdef->hul.rghs[0].cItem;
                        if (rgiFuel[iWarp + 1] == 0) {
                            pctShip10 = pctShip10 + (uint32_t)(lpshdef->hul.rghs[0].cItem * 0x2);
                            if (iWarp < 9 && rgiFuel[iWarp + 2] == 0) {
                                pctShip10 = pctShip10 + (uint32_t)(lpshdef->hul.rghs[0].cItem * 0x3);
                                if (iWarp < 8 && rgiFuel[iWarp + 3] == 0) {
                                    pctShip10 = pctShip10 + (uint32_t)(lpshdef->hul.rghs[0].cItem * 0x4);
                                }
                            }
                        }
                    }
                    pct10 = pct10 + (uint32_t)(pctShip10 * (int32_t)lpfl->rgcsh[i]);
                }
            }
            i = i + 1;
            lpshdef = lpshdef + 1;
        }
        pct10 = (uint32_t)(pct10 * dTravel);
        return pct10;
    }
    return 0;
}
