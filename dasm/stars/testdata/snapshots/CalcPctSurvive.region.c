void CalcPctSurvive(PLANET *lppl, float *ppct, float *ppctSmart) {
    int16_t iPlrSav;
    int32_t cDefenses;
    float   pct;
    PART    part;
    int16_t cMax;

    if (ppctSmart != 0x0) {
        *ppctSmart = 1.0;
    }
    if (lppl->iPlayer != -1 && lppl->cDefenses != 0x0) {
        iPlrSav = idPlayer;
        idPlayer = lppl->iPlayer;
        if (FGetBestDefensePart(&part) == 0) {
            pct = 1.0;
        } else {
            cDefenses = lppl->cDefenses;
            cMax = CMaxOperableDefenses(lppl, lppl->iPlayer, 0);
            if ((int32_t)cMax < cDefenses) {
                cDefenses = (int32_t)cMax;
            }
            pct = pow(1.0 - (double)(int32_t)part.pplanetary->grAbility / 1000.0, (double)cDefenses);
            if (ppctSmart != 0x0) {
                *ppctSmart = pow(1.0 - (double)(int32_t)part.pplanetary->grAbility / 2000.0, (double)cDefenses);
            }
        }
        idPlayer = iPlrSav;
    } else {
        pct = 1.0;
    }
    *ppct = pct;
    return;
}
