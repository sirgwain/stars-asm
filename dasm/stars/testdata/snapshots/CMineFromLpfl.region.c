int32_t CMineFromLpfl(FLEET *lpfl) {
    int32_t cMine;
    int16_t j;
    int16_t i;
    HUL    *lphuldef;
    PART    part;
    int32_t cMineTot;
    int16_t chs;
    HS     *lphs;

    cMineTot = 0;
    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            lphuldef = &rglpshdef[lpfl->iPlayer][i].hul;
            chs = lphuldef->chs;
            cMine = 0;
            j = 0;
            lphs = lphuldef->rghs;
            while (j < chs) {
                if (lphs->grhst == hstMining && lphs->iItem >= iminingRoboMidgetMiner && lphs->iItem <= iminingAlienMiner) {
                    part.hs = *lphs;
                    FLookupPart(&part);
                    cMine += (uint32_t)(lphs->cItem * part.pmining->grAbility);
                }
                j++;
                lphs++;
            }
            cMineTot += (uint32_t)(cMine * lpfl->rgcsh[i]);
        }
    }
    if (cMineTot < 4000) {
        return cMineTot;
    }
    return 4000;
}
