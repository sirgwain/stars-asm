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
    ipl = (ipl + 1);

L_17a7:
    if ((ipl >= vclpplAi))
        goto L_1cef;
    else
        goto L_17b2;

L_17b2:
    lppl = vrglpplAi[ipl];
    if ((vrglpplAi[ipl] != 0x0))
        goto L_17e2;
    else
        goto L_1cef;

L_17e2:
    if ((lppl->lpplprod != 0x0))
        goto L_17fc;
    else
        goto L_17a3;

L_17fc:
    prod = lppl->lpplprod->rgprod[0];
    if ((prod.grobj != grobjPlanet))
        goto L_18c8;
    else
        goto L_1835;

L_1835:
    if ((prod.iItem != mdIdleMine))
        goto L_1859;
    else
        goto L_17a3;

L_1859:
    if ((prod.iItem != iobjAlchemy))
        goto L_187d;
    else
        goto L_17a3;

L_187d:
    if ((prod.iItem != mdIdleAlchemy))
        goto L_18a1;
    else
        goto L_17a3;

L_18a1:
    if ((prod.iItem != mdIdleTerraform))
        goto L_18c8;
    else
        goto L_17a3;

L_18c8:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    PszProductionETA(&(sel.pl), sel.pl.lpplprod, iobjMine, &(etaFirst), 0x0);
    if ((etaFirst == 1))
        goto L_17a3;
    else
        goto L_1909;

L_1909:
    if ((etaFirst != -1))
        goto L_1917;
    else
        goto L_1912;

L_1912:
    etaFirst = 600;

L_1917:
    GetProductionCosts(lppl, &(prod), rgCost, idPlayer, 1);
    cRes = (uint32_t)(CResourcesAtPlanet(&(sel.pl), idPlayer));
    if ((sel.pl.fNoResearch != 0x0))
        goto L_19aa;
    else
        goto L_1977;

L_1977:
    cRes = (cRes - (int32_t)(((uint32_t)((cRes * (uint32_t)((uint16_t)(rgplr[idPlayer].pctResearch)))) / 0x64)));

L_19aa:
    if ((HIWORD(rgCost[3]) < HIWORD((uint32_t)((cRes * (uint32_t)((etaFirst - 1)))))))
        goto L_19d6;
    else
        goto L_19c6;

L_19c6:
    if ((HIWORD(rgCost[3]) > HIWORD((uint32_t)((cRes * (uint32_t)((etaFirst - 1)))))))
        goto L_17a3;
    else
        goto L_19cb;

L_19cb:
    if ((LOWORD(rgCost[3]) > LOWORD((uint32_t)((cRes * (uint32_t)((etaFirst - 1)))))))
        goto L_17a3;
    else
        goto L_19d6;

L_19d6:
    t_scratch_m136 = (uint32_t)(sel.pl.cMines);
    cMaxBuild = ((uint32_t)(CMaxOperableMines(&(sel.pl), idPlayer, 1)) - t_scratch_m136);
    if ((cMaxBuild < 0))
        goto L_1a34;
    else
        goto L_1a3e;

L_1a34:
    cMaxBuild = 0;

L_1a3e:
    cResMine = (uint32_t)(GetRaceStat(&(rgplr[idPlayer]), rsMineBuild));
    if (((int32_t)((uint32_t)((cResMine * cMaxBuild))) <= cRes))
        goto L_1a84;
    else
        goto L_1a93;

L_1a84:
    cBuild = cMaxBuild;
    goto L_1aaa;

L_1a93:
    cBuild = (int32_t)((cRes / cResMine));

L_1aaa:
    InitProduction(rgprod);
    if ((cBuild <= 0))
        goto L_1b74;
    else
        goto L_1ace;

L_1ace:
    AddItemToQueue(0x8, LOWORD(cBuild), grobjPlanet, 0);
    FinishProduction(1);
    PszProductionETA(&(sel.pl), sel.pl.lpplprod, iobjFactory, &(etaBetterMines), 0x0);
    if ((etaBetterMines != -1))
        goto L_1b29;
    else
        goto L_1b23;

L_1b23:
    etaBetterMines = 700;

L_1b29:
    sel.pl.lpplprod->rgprod[0].cItem = 0x1;
    sel.pl.lpplprod->rgprod[0].iItem = iobjAlchemy;
    goto L_1b9e;

L_1b74:
    etaBetterMines = 700;
    AddItemToQueue(0x3, 0x1, grobjPlanet, 0);
    FinishProduction(1);

L_1b9e:
    PszProductionETA(&(sel.pl), sel.pl.lpplprod, iobjFactory, &(etaBetterAlchemy), 0x0);
    if ((etaBetterAlchemy != -1))
        goto L_1bcf;
    else
        goto L_1bca;

L_1bca:
    etaBetterAlchemy = 700;

L_1bcf:
    if ((etaBetterAlchemy >= etaFirst))
        goto L_1be6;
    else
        goto L_1bda;

L_1bda:
    if ((etaBetterAlchemy < etaBetterMines))
        goto L_17a3;
    else
        goto L_1be6;

L_1be6:
    if ((cBuild < 1))
        goto L_17a3;
    else
        goto L_1c03;

L_1c03:
    sel.pl.lpplprod->rgprod[0].iItem = mdIdleMine;
    if ((etaFirst < etaBetterMines))
        goto L_1c4a;
    else
        goto L_1c33;

L_1c33:
    if ((cBuild <= 0))
        goto L_1c4a;
    else
        goto L_1c8f;

L_1c4a:
    sel.pl.lpplprod->iprodMac = (sel.pl.lpplprod->iprodMac - 0x1);
    fmemmove(sel.pl.lpplprod->rgprod, &(sel.pl.lpplprod->rgprod[1]), (sel.pl.lpplprod->iprodMac * 0x4));
    goto L_17a3;

L_1c8f:
    sel.pl.lpplprod->rgprod[0].cItem = LOWORD((uint32_t)(LOWORD(cBuild)));
    goto L_17a3;

L_1cef:
    return;
}
