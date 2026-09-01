int32_t GetTechLevelCost(int16_t iTech, int16_t iLevel, int16_t iplr) {
    int32_t lCost;
    int16_t i;
    int16_t cTech;

L_1dba:
    cTech = 0;
    i = 0;
    goto L_1def;

L_1dd0:
    cTech = (cTech + (uint16_t)(rgplr[iplr].rgTech[i]));
    i = (i + 1);

L_1def:
    if ((i < 6))
        goto L_1dd0;
    else
        goto L_1df8;

L_1df8:
    LOWORD(lCost) = (LOWORD((0xa * cTech)) + LOWORD(rglTechCost[iLevel]));
    HIWORD(lCost) = (SIGNHIWORD(LOWORD((0xa * cTech))) + HIWORD(rglTechCost[iLevel]));
    i = (GetRaceStat(rgplr[iplr], (iTech + 8)) - 1);
    if ((i == 0))
        goto L_1e83;
    else
        goto L_1e40;

L_1e40:
    if ((i >= 0))
        goto L_1e6a;
    else
        goto L_1e49;

L_1e49:
    lCost = (lCost + (lCost - (int32_t)((lCost >> 0x2))));
    goto L_1e83;

L_1e6a:
    lCost = (int32_t)((lCost / 2));

L_1e83:
    if ((game.fSlowTech == 0x0))
        goto L_1ea7;
    else
        goto L_1e93;

L_1e93:
    lCost = (int32_t)((lCost * 2));

L_1ea7:

L_1eb0:
    return lCost;
}
