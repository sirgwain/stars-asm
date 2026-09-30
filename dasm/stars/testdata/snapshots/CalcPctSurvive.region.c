void CalcPctSurvive(PLANET *lppl, float *ppct, float *ppctSmart) {
    int16_t iPlrSav;
    int32_t cDefenses;
    float   pct;
    PART    part;
    int16_t cMax;

    if (ppctSmart != 0) {
        *ppctSmart = 1.0;
    }
    if (lppl->iPlayer != -1 && lppl->cDefenses != 0) {
        iPlrSav = idPlayer;
        idPlayer = lppl->iPlayer;
        if (FGetBestDefensePart(&part) != 0) {
            cDefenses = lppl->cDefenses;
            cMax = CMaxOperableDefenses(lppl, lppl->iPlayer, 0);
            if (cMax < cDefenses) {
                cDefenses = cMax;
            }
            pct = pow(1.0 - (double)part.pplanetary->grAbility / 1000.0, (double)cDefenses);
            if (ppctSmart != 0) {
                *ppctSmart = pow(1.0 - (double)part.pplanetary->grAbility / 2000.0, (double)cDefenses);
            }
        } else {
            pct = 1.0;
        }
        idPlayer = iPlrSav;
    } else {
        pct = 1.0;
    }
    *ppct = pct;
    return;
}
