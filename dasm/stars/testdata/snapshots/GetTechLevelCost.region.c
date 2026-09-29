int32_t GetTechLevelCost(int16_t iTech, int16_t iLevel, int16_t iplr) {
    int32_t lCost;
    int16_t i;
    int16_t cTech;

    cTech = 0;
    for (i = 0; i < 6; i++) {
        cTech = cTech + (int16_t)rgplr[iplr].rgTech[i];
    }
    lCost = (int32_t)(10 * cTech) + rglTechCost[iLevel];
    i = GetRaceStat(&rgplr[iplr], iTech + 8) - 1;
    if (i != 0) {
        if (i >= 0) {
            lCost = (int32_t)(lCost / 2);
        } else {
            lCost = lCost + (lCost - (int32_t)(lCost >> 0x2));
        }
    }
    if (game.fSlowTech != 0x0) {
        lCost = (int32_t)(lCost * 2);
    }
    return lCost;
}
