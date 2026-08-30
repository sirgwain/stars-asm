void AddMinesToBlockedQueues() {
    PROD    prod;
    int32_t cMaxBuild;
    int16_t etaBetterAlchemy;
    int32_t cBuild;
    int16_t etaFirst;
    PLANET *lppl;
    int32_t cResMine;
    int32_t cRes;
    int16_t ipl;
    int32_t rgCost[4];
    PROD    rgprod[64];
    int16_t etaBetterMines;

L_1792:
    ipl = 0x0;
    goto L_17a7;

L_17a3:
    ipl = (ipl + 0x1);

L_17a7:
    if ((ipl >= vclpplAi))
        goto L_1cef;
    else
        goto L_17b2;

L_17b2:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_17e2;
    else
        goto L_17da;

L_17da:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_1cef;
    else
        goto L_17e2;

L_17e2:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_17fc;
    else
        goto L_17ef;

L_17ef:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_17a3;
    else
        goto L_17f6;

L_17f6:

L_17fc:
    prod = lppl->lpplprod->rgprod[0x0];
    if ((prod.grobj != 0x1))
        goto L_18c8;
    else
        goto L_182d;

L_182d:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_18c8 : L_1835 */

L_1835:
    if ((prod.iItem != 0x8))
        goto L_1859;
    else
        goto L_1851;

L_1851:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) == 0x0 ? L_17a3 : L_1859 */

L_1859:
    if ((prod.iItem != 0x3))
        goto L_187d;
    else
        goto L_1875;

L_1875:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) == 0x0 ? L_17a3 : L_187d */

L_187d:
    if ((prod.iItem != 0xb))
        goto L_18a1;
    else
        goto L_1899;

L_1899:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) == 0x0 ? L_17a3 : L_18a1 */

L_18a1:
    if ((prod.iItem != 0xc))
        goto L_18c8;
    else
        goto L_18bd;

L_18bd:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) == 0x0 ? L_17a3 : L_18c2 */

L_18c2:

L_18c8:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    PszProductionETA(&(sel.pl), sel.pl.lpplprod, iobjMine, &(etaFirst), 0x0);
    if ((etaFirst == 0x1))
        goto L_17a3;
    else
        goto L_1903;

L_1903:

L_1909:
    if ((etaFirst != 0xffff))
        goto L_1917;
    else
        goto L_1912;

L_1912:
    etaFirst = 0x258;

L_1917:
    /* untranslated: call GetProductionCosts(lppl, &prod, &part[0:4](rgCost), idPlayer, 0x1) -> callresult(void) */
    cRes = (uint32_t)(CResourcesAtPlanet(&(sel.pl), idPlayer));
    /* untranslated: branch (loword((uint32_t)(words(part[176:2](sel), part[178:2](sel)) >> 0x17)) & 0x1) != 0x0 ? L_19aa : L_196f */

L_196f:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_19aa : L_1977 */

L_1977:
    cRes = (cRes - (int32_t)(((uint32_t)((cRes * (uint32_t)((uint16_t)(rgplr[idPlayer].pctResearch)))) / 0x64)));

L_19aa:
    /* untranslated: branch part[14:2](rgCost) < hiword((uint32_t)(cRes * sext16to32((etaFirst + 0xffff)))) ? L_19d6 : L_19c6 */

L_19c6:
    /* untranslated: branch part[14:2](rgCost) > hiword(callresult(uint32_t)) ? L_17a3 : L_19cb */

L_19cb:
    /* untranslated: branch part[12:2](rgCost) > loword(callresult(uint32_t)) ? L_17a3 : L_19d0 */

L_19d0:

L_19d6:
    /* untranslated: ss:[bp-0x136] = (loword((uint32_t)(words(part[172:2](sel), part[174:2](sel)) >> 0x8)) & 0xfff) */
    /* untranslated: ss:[bp-0x134] = (hiword((uint32_t)(words(part[172:2](sel), part[174:2](sel)) >> 0x8)) & 0x0) */
    CMaxOperableMines(&(sel.pl), idPlayer, 0x1);
    /* untranslated: LOWORD(cMaxBuild) = (callresult(int16_t) - ss:[bp-0x136]) */
    /* untranslated: HIWORD(cMaxBuild) = (signhiword(callresult(int16_t)) - ss:[bp-0x134]) */
    if ((HIWORD(cMaxBuild) > 0x0))
        goto L_1a3e;
    else
        goto L_1a26;

L_1a26:
    if ((HIWORD(cMaxBuild) < 0x0))
        goto L_1a34;
    else
        goto L_1a2b;

L_1a2b:
    if ((LOWORD(cMaxBuild) >= 0x0))
        goto L_1a3e;
    else
        goto L_1a34;

L_1a34:
    cMaxBuild = 0x0;

L_1a3e:
    cResMine = (uint32_t)(GetRaceStat(rgplr[idPlayer], rsMineBuild));
    if ((HIWORD((uint32_t)((cResMine * cMaxBuild))) > HIWORD(cRes)))
        goto L_1a93;
    else
        goto L_1a77;

L_1a77:
    /* untranslated: branch hiword(callresult(uint32_t)) < HIWORD(cRes) ? L_1a84 : L_1a7c */

L_1a7c:
    /* untranslated: branch loword(callresult(uint32_t)) > LOWORD(cRes) ? L_1a93 : L_1a84 */

L_1a84:
    cBuild = cMaxBuild;
    goto L_1aaa;

L_1a93:
    cBuild = (int32_t)((cRes / cResMine));

L_1aaa:
    InitProduction(rgprod);
    if ((HIWORD(cBuild) < 0x0))
        goto L_1b74;
    else
        goto L_1ac0;

L_1ac0:
    if ((HIWORD(cBuild) > 0x0))
        goto L_1ace;
    else
        goto L_1ac5;

L_1ac5:
    if ((LOWORD(cBuild) <= 0x0))
        goto L_1b74;
    else
        goto L_1ace;

L_1ace:
    AddItemToQueue(0x8, LOWORD(cBuild), grobjPlanet, 0x0);
    FinishProduction(0x1);
    PszProductionETA(&(sel.pl), sel.pl.lpplprod, iobjFactory, &(etaBetterMines), 0x0);
    if ((etaBetterMines != 0xffff))
        goto L_1b29;
    else
        goto L_1b23;

L_1b23:
    etaBetterMines = 0x2bc;

L_1b29:
    LOWORD(sel.pl.lpplprod->rgprod[0x0]) = ((LOWORD(sel.pl.lpplprod->rgprod[0x0]) & 0xfc00) | 0x1);
    HIWORD(sel.pl.lpplprod->rgprod[0x0]) = ((HIWORD(sel.pl.lpplprod->rgprod[0x0]) & 0xffff) | 0x0);
    LOWORD(sel.pl.lpplprod->rgprod[0x0]) = ((LOWORD(sel.pl.lpplprod->rgprod[0x0]) & 0x3ff) | 0xc00);
    HIWORD(sel.pl.lpplprod->rgprod[0x0]) = ((HIWORD(sel.pl.lpplprod->rgprod[0x0]) & 0xfffe) | 0x0);
    goto L_1b9e;

L_1b74:
    etaBetterMines = 0x2bc;
    AddItemToQueue(0x3, 0x1, grobjPlanet, 0x0);
    FinishProduction(0x1);

L_1b9e:
    PszProductionETA(&(sel.pl), sel.pl.lpplprod, iobjFactory, &(etaBetterAlchemy), 0x0);
    if ((etaBetterAlchemy != 0xffff))
        goto L_1bcf;
    else
        goto L_1bca;

L_1bca:
    etaBetterAlchemy = 0x2bc;

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
    if ((HIWORD(cBuild) > 0x0))
        goto L_1c03;
    else
        goto L_1bef;

L_1bef:
    if ((HIWORD(cBuild) < 0x0))
        goto L_17a3;
    else
        goto L_1bf4;

L_1bf4:
    if ((LOWORD(cBuild) < 0x1))
        goto L_17a3;
    else
        goto L_1bfa;

L_1bfa:

L_1c03:
    LOWORD(sel.pl.lpplprod->rgprod[0x0]) = ((LOWORD(sel.pl.lpplprod->rgprod[0x0]) & 0x3ff) | 0x2000);
    HIWORD(sel.pl.lpplprod->rgprod[0x0]) = ((HIWORD(sel.pl.lpplprod->rgprod[0x0]) & 0xfffe) | 0x0);
    if ((etaFirst < etaBetterMines))
        goto L_1c4a;
    else
        goto L_1c33;

L_1c33:
    if ((HIWORD(cBuild) > 0x0))
        goto L_1c8f;
    else
        goto L_1c3c;

L_1c3c:
    if ((HIWORD(cBuild) < 0x0))
        goto L_1c4a;
    else
        goto L_1c41;

L_1c41:
    if ((LOWORD(cBuild) > 0x0))
        goto L_1c8f;
    else
        goto L_1c4a;

L_1c4a:
    sel.pl.lpplprod->iprodMac = (sel.pl.lpplprod->iprodMac - 0x1);
    fmemmove(&(sel.pl.lpplprod->rgprod), &(sel.pl.lpplprod->rgprod[0x1]), (sel.pl.lpplprod->iprodMac * 0x4));
    goto L_17a3;

L_1c8f:
    /* untranslated: LOWORD(sel.pl.lpplprod->rgprod[0x0]) = ((LOWORD(sel.pl.lpplprod->rgprod[0x0]) & 0xfc00) | loword((int32_t)(words((LOWORD(cBuild) & 0x3ff),
     * (0x0 & 0x0)) << 0x0))) */
    /* untranslated: HIWORD(sel.pl.lpplprod->rgprod[0x0]) = ((HIWORD(sel.pl.lpplprod->rgprod[0x0]) & 0xffff) | hiword((int32_t)(words((LOWORD(cBuild) & 0x3ff),
     * (0x0 & 0x0)) << 0x0))) */
    goto L_17a3;

L_1cef:
    return;
}
