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

L_1792:
    ipl = 0;
    goto L_17a7;

L_17a3:
    ipl++;

L_17a7:
    if (ipl >= vclpplAi)
        goto L_1cef;
    else
        goto L_17b2;

L_17b2:
    lppl = vrglpplAi[ipl];
    if (vrglpplAi[ipl] != 0)
        goto L_17e2;
    else
        goto L_1cef;

L_17e2:
    if (lppl->lpplprod != 0)
        goto L_17fc;
    else
        goto L_17a3;

L_17fc:
    prod = lppl->lpplprod->rgprod[0];
    if (prod.grobj != grobjPlanet)
        goto L_18c8;
    else
        goto L_1835;

L_1835:
    if (prod.iItem != mdIdleMine)
        goto L_1859;
    else
        goto L_17a3;

L_1859:
    if (prod.iItem != iobjAlchemy)
        goto L_187d;
    else
        goto L_17a3;

L_187d:
    if (prod.iItem != mdIdleAlchemy)
        goto L_18a1;
    else
        goto L_17a3;

L_18a1:
    if (prod.iItem != mdIdleTerraform)
        goto L_18c8;
    else
        goto L_17a3;

L_18c8:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    PszProductionETA(&sel.pl, sel.pl.lpplprod, iobjMine, &etaFirst, NULL);
    if (etaFirst == 1)
        goto L_17a3;
    else
        goto L_1909;

L_1909:
    if (etaFirst != -1)
        goto L_1917;
    else
        goto L_1912;

L_1912:
    etaFirst = 600;

L_1917:
    GetProductionCosts(lppl, &prod, rgCost, idPlayer, TRUE);
    cRes = CResourcesAtPlanet(&sel.pl, idPlayer);
    if (sel.pl.fNoResearch != 0)
        goto L_19aa;
    else
        goto L_1977;

L_1977:
    cRes -= (int32_t)(cRes * (int16_t)rgplr[idPlayer].pctResearch) / 100;

L_19aa:
    if (rgCost[3] <= (int32_t)(uint32_t)(cRes * (int16_t)(etaFirst - 1)))
        goto L_19d6;
    else
        goto L_17a3;

L_19d6:
    t_scratch_m136 = (uint32_t)sel.pl.cMines;
    cMaxBuild = CMaxOperableMines(&sel.pl, idPlayer, TRUE) - t_scratch_m136;
    if (cMaxBuild < 0)
        goto L_1a34;
    else
        goto L_1a3e;

L_1a34:
    cMaxBuild = 0;

L_1a3e:
    cResMine = GetRaceStat(&rgplr[idPlayer], rsMineBuild);
    if ((int32_t)(uint32_t)(cResMine * cMaxBuild) <= cRes)
        goto L_1a84;
    else
        goto L_1a93;

L_1a84:
    cBuild = cMaxBuild;
    goto L_1aaa;

L_1a93:
    cBuild = (int32_t)(cRes / cResMine);

L_1aaa:
    InitProduction(rgprod);
    if (cBuild <= 0)
        goto L_1b74;
    else
        goto L_1ace;

L_1ace:
    AddItemToQueue(mdIdleMine, LOWORD(cBuild), grobjPlanet, addItemFront);
    FinishProduction(TRUE);
    PszProductionETA(&sel.pl, sel.pl.lpplprod, iobjFactory, &etaBetterMines, NULL);
    if (etaBetterMines != -1)
        goto L_1b29;
    else
        goto L_1b23;

L_1b23:
    etaBetterMines = 700;

L_1b29:
    sel.pl.lpplprod->rgprod[0].cItem = 1;
    sel.pl.lpplprod->rgprod[0].iItem = iobjAlchemy;
    goto L_1b9e;

L_1b74:
    etaBetterMines = 700;
    AddItemToQueue(iobjAlchemy, 1, grobjPlanet, addItemFront);
    FinishProduction(TRUE);

L_1b9e:
    PszProductionETA(&sel.pl, sel.pl.lpplprod, iobjFactory, &etaBetterAlchemy, NULL);
    if (etaBetterAlchemy != -1)
        goto L_1bcf;
    else
        goto L_1bca;

L_1bca:
    etaBetterAlchemy = 700;

L_1bcf:
    if (etaBetterAlchemy >= etaFirst)
        goto L_1be6;
    else
        goto L_1bda;

L_1bda:
    if (etaBetterAlchemy < etaBetterMines)
        goto L_17a3;
    else
        goto L_1be6;

L_1be6:
    if (cBuild < 1)
        goto L_17a3;
    else
        goto L_1c03;

L_1c03:
    sel.pl.lpplprod->rgprod[0].iItem = mdIdleMine;
    if (etaFirst < etaBetterMines)
        goto L_1c4a;
    else
        goto L_1c33;

L_1c33:
    if (cBuild <= 0)
        goto L_1c4a;
    else
        goto L_1c8f;

L_1c4a:
    sel.pl.lpplprod->iprodMac--;
    fmemmove(sel.pl.lpplprod->rgprod, &sel.pl.lpplprod->rgprod[1], sel.pl.lpplprod->iprodMac * sizeof(PROD));
    goto L_17a3;

L_1c8f:
    sel.pl.lpplprod->rgprod[0].cItem = LOWORD(cBuild);
    goto L_17a3;

L_1cef:
    return;
}
