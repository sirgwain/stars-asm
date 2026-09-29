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
        if (vrglpplAi[ipl] == 0x0)
            break;
        if (lppl->lpplprod != 0x0) {
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
            PszProductionETA(&sel.pl, sel.pl.lpplprod, iobjMine, &etaFirst, 0x0);
            if (etaFirst != 1) {
                if (etaFirst == -1) {
                    etaFirst = 600;
                }
                GetProductionCosts(lppl, &prod, rgCost, idPlayer, 1);
                cRes = (int32_t)CResourcesAtPlanet(&sel.pl, idPlayer);
                if (sel.pl.fNoResearch == 0x0) {
                    cRes = cRes - (int32_t)((int32_t)(cRes * (int32_t)(int16_t)rgplr[idPlayer].pctResearch) / 0x64);
                }
                if (rgCost[3] <= (int32_t)(uint32_t)(cRes * (int32_t)(etaFirst - 1))) {
                    t_scratch_m136 = (uint32_t)sel.pl.cMines;
                    cMaxBuild = (int32_t)CMaxOperableMines(&sel.pl, idPlayer, 1) - t_scratch_m136;
                    if (cMaxBuild < 0) {
                        cMaxBuild = 0;
                    }
                    cResMine = (int32_t)GetRaceStat(&rgplr[idPlayer], rsMineBuild);
                    if ((int32_t)(uint32_t)(cResMine * cMaxBuild) <= cRes) {
                        cBuild = cMaxBuild;
                    } else {
                        cBuild = (int32_t)(cRes / cResMine);
                    }
                    InitProduction(rgprod);
                    if (cBuild <= 0) {
                        etaBetterMines = 700;
                        AddItemToQueue(0x3, 0x1, grobjPlanet, 0);
                        FinishProduction(1);
                    } else {
                        AddItemToQueue(0x8, LOWORD(cBuild), grobjPlanet, 0);
                        FinishProduction(1);
                        PszProductionETA(&sel.pl, sel.pl.lpplprod, iobjFactory, &etaBetterMines, 0x0);
                        if (etaBetterMines == -1) {
                            etaBetterMines = 700;
                        }
                        sel.pl.lpplprod->rgprod[0].cItem = 0x1;
                        sel.pl.lpplprod->rgprod[0].iItem = iobjAlchemy;
                    }
                    PszProductionETA(&sel.pl, sel.pl.lpplprod, iobjFactory, &etaBetterAlchemy, 0x0);
                    if (etaBetterAlchemy == -1) {
                        etaBetterAlchemy = 700;
                    }
                    if ((etaBetterAlchemy >= etaFirst || etaBetterAlchemy >= etaBetterMines) && cBuild >= 1) {
                        sel.pl.lpplprod->rgprod[0].iItem = mdIdleMine;
                        if (etaFirst < etaBetterMines || cBuild <= 0) {
                            sel.pl.lpplprod->iprodMac = sel.pl.lpplprod->iprodMac - 0x1;
                            fmemmove(sel.pl.lpplprod->rgprod, &sel.pl.lpplprod->rgprod[1], sel.pl.lpplprod->iprodMac * sizeof(PROD));
                        } else {
                            sel.pl.lpplprod->rgprod[0].cItem = LOWORD((uint32_t)LOWORD(cBuild));
                        }
                    }
                }
            }
        }
    }
    return;
}
