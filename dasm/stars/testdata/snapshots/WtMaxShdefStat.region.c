int16_t WtMaxShdefStat(SHDEF *lpshdef, int16_t grStat) {
    int16_t wt;
    int16_t j;
    HUL    *lphul;

    lphul = &lpshdef->hul;
    if (grStat != 1) {
        if (grStat != 2) {
            return 0;
        }
        wt = LphuldefFromId(lphul->ihuldef)->hul.wtCargoMax;
        for (j = 0; j < lphul->chs; j++) {
            if (lphul->rghs[j].grhst == hstSpecialM) {
                switch (lphul->rghs[j].iItem) {
                default:
                    break;
                case ispecialMCargoPod:
                    wt += lphul->rghs[j].cItem * 50;
                    break;
                case ispecialMSuperCargoPod:
                    wt += lphul->rghs[j].cItem * 100;
                    break;
                case ispecialMMultiCargoPod:
                    wt += lphul->rghs[j].cItem * 250;
                }
            }
        }
    } else {
        wt = LphuldefFromId(lphul->ihuldef)->hul.wtFuelMax;
        for (j = 0; j < lphul->chs; j++) {
            if (lphul->rghs[j].grhst != hstSpecialM) {
                if (lphul->rghs[j].grhst == hstSpecialE && lphul->rghs[j].iItem == ispecialEAntiMatterGenerator) {
                    wt += lphul->rghs[j].cItem * 200;
                }
            } else if (lphul->rghs[j].iItem == ispecialMFuelTank) {
                wt += lphul->rghs[j].cItem * 250;
            } else if (lphul->rghs[j].iItem == ispecialMSuperFuelTank) {
                wt += lphul->rghs[j].cItem * 500;
            }
        }
    }
    return wt;
}
