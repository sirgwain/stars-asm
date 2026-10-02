void AddMinesToBlockedQueues() {
    PROD     prod;
    int32_t  cMaxBuild;
    int16_t  etaBetterAlchemy;
    int32_t  cBuild;
    int16_t  etaFirst;
    PLANET  *lppl;
    int32_t  cResMine;
    int32_t  cRes;
    int16_t  ipl;
    int32_t  rgCost[4];
    PROD     rgprod[64];
    int16_t  etaBetterMines;
    uint32_t t_scratch_m136;

    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0)
            break;
        if (lppl->lpplprod != 0) {
            prod = lppl->lpplprod->rgprod[0];
            if (prod.grobj == grobjPlanet) {
                switch (prod.iItem) {
                case mdIdleMine:
                case iobjAlchemy:
                case mdIdleAlchemy:
                case mdIdleTerraform:
                    break;
                default:
                    goto L_18c8;
                }
                continue;
            }
        L_18c8:
            ChangeMainObjSel(grobjPlanet, lppl->id);
            PszProductionETA(&sel.pl, sel.pl.lpplprod, iobjMine, &etaFirst, NULL);
            if (etaFirst != 1) {
                if (etaFirst == -1) {
                    etaFirst = 600;
                }
                GetProductionCosts(lppl, &prod, rgCost, idPlayer, 1);
                cRes = CResourcesAtPlanet(&sel.pl, idPlayer);
                if (sel.pl.fNoResearch == 0) {
                    cRes -= (int32_t)(cRes * (int16_t)rgplr[idPlayer].pctResearch) / 100;
                }
                if (rgCost[3] <= (int32_t)(uint32_t)(cRes * (int16_t)(etaFirst - 1))) {
                    t_scratch_m136 = (uint32_t)sel.pl.cMines;
                    cMaxBuild = CMaxOperableMines(&sel.pl, idPlayer, 1) - t_scratch_m136;
                    if (cMaxBuild < 0) {
                        cMaxBuild = 0;
                    }
                    cResMine = GetRaceStat(&rgplr[idPlayer], rsMineBuild);
                    if ((int32_t)(uint32_t)(cResMine * cMaxBuild) <= cRes) {
                        cBuild = cMaxBuild;
                    } else {
                        cBuild = (int32_t)(cRes / cResMine);
                    }
                    InitProduction(rgprod);
                    if (cBuild > 0) {
                        AddItemToQueue(mdIdleMine, LOWORD(cBuild), grobjPlanet, addItemFront);
                        FinishProduction(1);
                        PszProductionETA(&sel.pl, sel.pl.lpplprod, iobjFactory, &etaBetterMines, NULL);
                        if (etaBetterMines == -1) {
                            etaBetterMines = 700;
                        }
                        sel.pl.lpplprod->rgprod[0].cItem = 1;
                        sel.pl.lpplprod->rgprod[0].iItem = iobjAlchemy;
                    } else {
                        etaBetterMines = 700;
                        AddItemToQueue(iobjAlchemy, 1, grobjPlanet, addItemFront);
                        FinishProduction(1);
                    }
                    PszProductionETA(&sel.pl, sel.pl.lpplprod, iobjFactory, &etaBetterAlchemy, NULL);
                    if (etaBetterAlchemy == -1) {
                        etaBetterAlchemy = 700;
                    }
                    if ((etaBetterAlchemy >= etaFirst || etaBetterAlchemy >= etaBetterMines) && cBuild >= 1) {
                        sel.pl.lpplprod->rgprod[0].iItem = mdIdleMine;
                        if (etaFirst < etaBetterMines || cBuild <= 0) {
                            sel.pl.lpplprod->iprodMac--;
                            fmemmove(sel.pl.lpplprod->rgprod, &sel.pl.lpplprod->rgprod[1], sel.pl.lpplprod->iprodMac * sizeof(PROD));
                        } else {
                            sel.pl.lpplprod->rgprod[0].cItem = LOWORD(cBuild);
                        }
                    }
                }
            }
        }
    }
    return;
}
