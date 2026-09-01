int16_t CBuildProdItem(PLANET *lppl, PROD *lpprod, PROD *pprodPartial, int32_t *rgRes, int16_t fAlchemy, int16_t *pmdStatus, int16_t fCalcOnly) {
    int32_t  pctT;
    int16_t  cMax;
    uint32_t iobjOther;
    int32_t  cCanBuild;
    int32_t  lMinNeeded;
    int32_t  lAlchCost;
    PROD     prod;
    int16_t  fAutoBuild;
    int16_t  cBuilt;
    int16_t  cAlchemy;
    int32_t  rgCostPaid[4];
    int16_t  i;
    int16_t  fResourceBlocked;
    int32_t  pctInitial;
    int32_t  pctTooBig;
    int32_t  pct;
    int32_t  rgCost[4];
    int16_t  fMineralBlocked;
    int32_t  AddCost;
    uint16_t t_merge_0d4c_0001;
    int32_t  t_call_0ea9;
    int32_t  t_merge_126a_0001_wide;
    uint16_t t_merge_1410_0001;
    int32_t  t_merge_15f8_0001_wide;
    uint16_t t_merge_181f_0001;
    uint16_t t_merge_185e_0001;
    uint16_t t_merge_189c_0001;

L_0c92:
    cAlchemy = 0;
    LOWORD(pctInitial) = lpprod->pct;
    HIWORD(pctInitial) = 0x0;
    prod = lpprod;
    GetProductionCosts(lppl, lpprod, rgCost, lppl->iPlayer, 0x1);
    cBuilt = 0;
    if ((prod.grobj != 0x1))
        goto L_0d49;
    else
        goto L_0d12;

L_0d12:
    if ((0x0 != 0x0))
        goto L_0d49;
    else
        goto L_0d1a;

L_0d1a:
    if ((0x0 > 0x0))
        goto L_0d49;
    else
        goto L_0d36;

L_0d36:
    if ((0x0 < 0x0))
        goto L_0d43;
    else
        goto L_0d3b;

L_0d3b:
    if ((prod.iItem >= 0x7))
        goto L_0d49;
    else
        goto L_0d43;

L_0d43:
    t_merge_0d4c_0001 = 0x1;
    goto L_0d4c;

L_0d49:
    t_merge_0d4c_0001 = 0x0;

L_0d4c:
    fAutoBuild = t_merge_0d4c_0001;
    if ((fAutoBuild == 0))
        goto L_102c;
    else
        goto L_0d58;

L_0d58:
    cMax = 1000;
    goto L_0f2c;

L_0d74:
    iobjOther = 0x8;
    /* untranslated: ss:[bp-0x56] = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff) */
    /* untranslated: cMax = (CMaxOperableMines(lppl, lppl->iPlayer, 0x1) - ss:[bp-0x56]) */
    goto L_0f9f;

L_0dbe:
    iobjOther = 0x7;
    /* untranslated: ss:[bp-0x56] = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff) */
    /* untranslated: cMax = (CMaxOperableFactories(lppl, lppl->iPlayer, 0x1) - ss:[bp-0x56]) */
    goto L_0f9f;

L_0e08:
    iobjOther = 0x9;
    /* untranslated: ss:[bp-0x56] = lppl->cDefenses */
    /* untranslated: cMax = (CMaxOperableDefenses(lppl, lppl->iPlayer, 0x1) - ss:[bp-0x56]) */
    goto L_0f9f;

L_0e4a:
    iobjOther = 0xb;
    goto L_0f9f;

L_0e57:
    iobjOther = 0xc;
    cMax = IpctCanTerraformLppl(lppl);
    if ((cMax <= 0))
        goto L_0f9f;
    else
        goto L_0e7b;

L_0e7b:
    if ((prod.iItem != 0x4))
        goto L_0f9f;
    else
        goto L_0e97;

L_0e97:
    if ((0x0 != 0x0))
        goto L_0f9f;
    else
        goto L_0e9f;

L_0e9f:
    t_call_0ea9 = ChgPopFromPlanet(lppl, 0x0);
    if ((HIWORD(t_call_0ea9) < 0x0))
        goto L_0f9f;
    else
        goto L_0eb9;

L_0eb9:
    if ((HIWORD(t_call_0ea9) > 0x0))
        goto L_0ec6;
    else
        goto L_0ebe;

L_0ebe:
    if ((LOWORD(t_call_0ea9) < 0x0))
        goto L_0f9f;
    else
        goto L_0ec6;

L_0ec6:
    if ((PctPlanetDesirability(lppl, lppl->iPlayer) <= 0))
        goto L_0f9f;
    else
        goto L_0ee3;

L_0ee3:
    cMax = 0;

L_0eeb:
    iobjOther = 0x11;
    if ((IWarpMAFromLppl(lppl, 0x0) == 0))
        goto L_0f21;
    else
        goto L_0f0f;

L_0f0f:
    if ((lppl->idFling != 0x0))
        goto L_0f9f;
    else
        goto L_0f21;

L_0f21:
    cMax = 0;

L_0f2c:
    if ((prod.iItem != 0x0))
        goto L_0f3c;
    else
        goto L_0f34;

L_0f34:
    if ((0x0 == 0x0))
        goto L_0d74;
    else
        goto L_0f3c;

L_0f3c:
    if ((prod.iItem != 0x1))
        goto L_0f4c;
    else
        goto L_0f44;

L_0f44:
    if ((0x0 == 0x0))
        goto L_0dbe;
    else
        goto L_0f4c;

L_0f4c:
    if ((prod.iItem != 0x2))
        goto L_0f5c;
    else
        goto L_0f54;

L_0f54:
    if ((0x0 == 0x0))
        goto L_0e08;
    else
        goto L_0f5c;

L_0f5c:
    if ((prod.iItem != 0x3))
        goto L_0f6c;
    else
        goto L_0f64;

L_0f64:
    if ((0x0 == 0x0))
        goto L_0e4a;
    else
        goto L_0f6c;

L_0f6c:
    if ((prod.iItem != 0x4))
        goto L_0f7c;
    else
        goto L_0f74;

L_0f74:
    if ((0x0 == 0x0))
        goto L_0e57;
    else
        goto L_0f7c;

L_0f7c:
    if ((prod.iItem != 0x5))
        goto L_0f8c;
    else
        goto L_0f84;

L_0f84:
    if ((0x0 == 0x0))
        goto L_0e57;
    else
        goto L_0f8c;

L_0f8c:
    if ((prod.iItem != 0x6))
        goto L_0f9f;
    else
        goto L_0f94;

L_0f94:
    if ((0x0 == 0x0))
        goto L_0eeb;
    else
        goto L_0f9f;

L_0f9f:
    if ((cMax >= 0))
        goto L_0fad;
    else
        goto L_0fa8;

L_0fa8:
    cMax = 0;

L_0fad:
    if ((0x0 > SIGNHIWORD(cMax)))
        goto L_1000;
    else
        goto L_0fd0;

L_0fd0:
    /* untranslated: branch ss:[bp-0x56] < signhiword(cMax) ? L_0fdc : L_0fd5 */

L_0fd5:
    /* untranslated: branch ss:[bp-0x58] > cMax ? L_1000 : L_0fdc */

L_0fdc:
    if ((prod.iItem != 0x3))
        goto L_102c;
    else
        goto L_0ff8;

L_0ff8:
    if ((0x0 != 0x0))
        goto L_102c;
    else
        goto L_1000;

L_1000:
    /* untranslated: prod.cItem = loword((int32_t)(words((cMax & 0x3ff), 0x0) << 0x0)) */
    /* untranslated: HIWORD(prod) = ((HIWORD(prod) & 0xffff) | hiword((int32_t)(words((cMax & 0x3ff), 0x0) << 0x0))) */

L_102c:
    i = 0;
    goto L_1086;

L_1034:
    /* untranslated: rgCostPaid[i] = (uint32_t)((uint32_t)(rgCost[i] * words(0x0, prod.pct)) / 0x64) */
    i = (i + 1);

L_1086:
    if ((i < 4))
        goto L_1034;
    else
        goto L_108f;

L_108f:
    if ((0x0 < 0x0))
        goto L_1712;
    else
        goto L_10a3;

L_10a3:
    if ((0x0 > 0x0))
        goto L_10b0;
    else
        goto L_10a8;

L_10a8:
    if ((prod.cItem <= 0x0))
        goto L_1712;
    else
        goto L_10b0;

L_10b0:
    i = 0;
    goto L_1101;

L_10b8:
    if (((HIWORD(rgCost[i]) - HIWORD(rgCostPaid[i])) < HIWORD(rgRes[i])))
        goto L_10fd;
    else
        goto L_10ee;

L_10ee:
    if (((HIWORD(rgCost[i]) - HIWORD(rgCostPaid[i])) > HIWORD(rgRes[i])))
        goto L_110a;
    else
        goto L_10f3;

L_10f3:
    if (((LOWORD(rgCost[i]) - LOWORD(rgCostPaid[i])) > LOWORD(rgRes[i])))
        goto L_110a;
    else
        goto L_10f7;

L_10f7:

L_10fd:
    i = (i + 1);

L_1101:
    if ((i < 4))
        goto L_10b8;
    else
        goto L_110a;

L_110a:
    if ((i >= 4))
        goto L_165c;
    else
        goto L_1113;

L_1113:
    fMineralBlocked = 0;
    fResourceBlocked = 0;
    pct = 100;
    i = 0;
    goto L_12ea;

L_1130:
    if ((HIWORD(rgCost[i]) < 0x0))
        goto L_12e6;
    else
        goto L_1145;

L_1145:
    if ((HIWORD(rgCost[i]) > 0x0))
        goto L_1152;
    else
        goto L_114a;

L_114a:
    if ((LOWORD(rgCost[i]) <= 0x0))
        goto L_12e6;
    else
        goto L_1152;

L_1152:
    if ((HIWORD(rgRes[i]) < HIWORD(rgCost[i])))
        goto L_1190;
    else
        goto L_1177;

L_1177:
    if ((HIWORD(rgRes[i]) > HIWORD(rgCost[i])))
        goto L_1183;
    else
        goto L_117c;

L_117c:
    if ((LOWORD(rgRes[i]) < LOWORD(rgCost[i])))
        goto L_1190;
    else
        goto L_1183;

L_1183:
    pctT = 100;
    goto L_1270;

L_1190:
    /* untranslated: pctT = (int32_t)((uint32_t)(words((HIWORD(rgRes[i]) + HIWORD(rgCostPaid[i])), (LOWORD(rgRes[i]) + LOWORD(rgCostPaid[i]))) * 0x64) /
     * rgCost[i]) */
    /* untranslated: pctTooBig = (int32_t)((uint32_t)(words(((HIWORD(rgRes[i]) + HIWORD(rgCostPaid[i])) + 0x0), ((LOWORD(rgRes[i]) + LOWORD(rgCostPaid[i])) +
     * 0x1)) * 0x64) / rgCost[i]) */
    if ((HIWORD(pctT) < (HIWORD(pctTooBig) + 0xffff)))
        goto L_125e;
    else
        goto L_1248;

L_1248:
    if ((HIWORD(pctT) > (HIWORD(pctTooBig) + 0xffff)))
        goto L_1255;
    else
        goto L_124d;

L_124d:
    if ((LOWORD(pctT) <= (LOWORD(pctTooBig) + 0xffff)))
        goto L_125e;
    else
        goto L_1255;

L_1255:
    t_merge_126a_0001_wide = pctT;
    goto L_126a;

L_125e:
    t_merge_126a_0001_wide = (pctTooBig - 1);

L_126a:
    pctT = t_merge_126a_0001_wide;

L_1270:
    if ((HIWORD(pctT) > HIWORD(pct)))
        goto L_12e6;
    else
        goto L_127e;

L_127e:
    if ((HIWORD(pctT) < HIWORD(pct)))
        goto L_128b;
    else
        goto L_1283;

L_1283:
    if ((LOWORD(pctT) >= LOWORD(pct)))
        goto L_12e6;
    else
        goto L_128b;

L_128b:
    lMinNeeded = ((rgCost[i] - rgCostPaid[i]) - rgRes[i]);
    pct = pctT;
    if ((i != 3))
        goto L_12e1;
    else
        goto L_12d9;

L_12d9:
    fResourceBlocked = 1;
    goto L_12e6;

L_12e1:
    fMineralBlocked = 1;

L_12e6:
    i = (i + 1);

L_12ea:
    if ((i < 4))
        goto L_1130;
    else
        goto L_12f3;

L_12f3:
    if ((fMineralBlocked == 0))
        goto L_131c;
    else
        goto L_12fc;

L_12fc:
    if ((fAutoBuild == 0))
        goto L_131c;
    else
        goto L_1305;

L_1305:
    if ((fAlchemy != 0))
        goto LAlchemize;
    else
        goto L_130b;

L_130b:

L_1314:
    fAutoBuild = 2;
    goto L_1712;

L_131c:
    i = 0;
    goto L_1398;

L_1324:
    AddCost = ((int32_t)(((uint32_t)((rgCost[i] * pct)) / 0x64)) - rgCostPaid[i]);
    rgRes[i] = (rgRes[i] - AddCost);
    rgCostPaid[i] = (rgCostPaid[i] + AddCost);
    i = (i + 1);

L_1398:
    if ((i < 4))
        goto L_1324;
    else
        goto L_13a1;

L_13a1:
    /* untranslated: LOWORD(prod) = ((LOWORD(prod) & 0xffff) | loword((int32_t)(words((LOWORD(pct) & 0x7f), 0x0) << 0x14))) */
    /* untranslated: HIWORD(prod) = ((HIWORD(prod) & 0xf80f) | hiword((int32_t)(words((LOWORD(pct) & 0x7f), 0x0) << 0x14))) */
    if ((fAlchemy == 0))
        goto L_1712;
    else
        goto L_13d5;

L_13d5:
    if ((fResourceBlocked != 0))
        goto L_1712;
    else
        goto L_13db;

L_13db:

LAlchemize:
    if ((GetRaceGrbit(rgplr[lppl->iPlayer], ibitRaceMineralAlchemy) == 0))
        goto L_140c;
    else
        goto L_1405;

L_1405:
    t_merge_1410_0001 = 0x19;
    goto L_1410;

L_140c:
    t_merge_1410_0001 = 0x64;

L_1410:
    LOWORD(lAlchCost) = t_merge_1410_0001;
    HIWORD(lAlchCost) = 0x0;
    /* untranslated: cCanBuild = (int32_t)(words(*(rgRes+0xe), LOWORD(rgRes[0x3])) / lAlchCost) */
    if ((HIWORD(cCanBuild) < HIWORD(lMinNeeded)))
        goto L_1457;
    else
        goto L_143e;

L_143e:
    if ((HIWORD(cCanBuild) > HIWORD(lMinNeeded)))
        goto L_144b;
    else
        goto L_1443;

L_1443:
    if ((LOWORD(cCanBuild) <= LOWORD(lMinNeeded)))
        goto L_1457;
    else
        goto L_144b;

L_144b:
    cCanBuild = lMinNeeded;

L_1457:
    if ((HIWORD(cCanBuild) < 0x0))
        goto L_14c5;
    else
        goto L_1460;

L_1460:
    if ((HIWORD(cCanBuild) > 0x0))
        goto L_146e;
    else
        goto L_1465;

L_1465:
    if ((LOWORD(cCanBuild) <= 0x0))
        goto L_14c5;
    else
        goto L_146e;

L_146e:
    i = 0;
    goto L_1491;

L_1476:
    rgRes[i] = (rgRes[i] + cCanBuild);
    i = (i + 1);

L_1491:
    if ((i < 3))
        goto L_1476;
    else
        goto L_149a;

L_149a:
    rgRes[i] = (rgRes[i] - (uint32_t)((lAlchCost * cCanBuild)));
    cAlchemy = (cAlchemy + LOWORD(cCanBuild));

L_14c5:
    if ((LOWORD(cCanBuild) != LOWORD(lMinNeeded)))
        goto L_14de;
    else
        goto L_14d3;

L_14d3:
    if ((HIWORD(cCanBuild) == HIWORD(lMinNeeded)))
        goto L_108f;
    else
        goto L_14d8;

L_14d8:

L_14de:
    if ((*(rgRes + 0xe) < 0x0))
        goto L_1712;
    else
        goto L_14ea;

L_14ea:
    if ((*(rgRes + 0xe) > 0x0))
        goto L_14f8;
    else
        goto L_14ef;

L_14ef:
    if ((LOWORD(rgRes[0x3]) <= 0x0))
        goto L_1712;
    else
        goto L_14f8;

L_14f8:
    if ((pprodPartial == 0x0))
        goto L_1712;
    else
        goto L_1501;

L_1501:
    memset(pprodPartial, 0x0, 0x4);
    pprodPartial = ((pprodPartial & 0xffff) | 0x0);
    pprodPartial->grobj = grobjPlanet;
    pprodPartial = ((pprodPartial & 0x3ff) | 0x2c00);
    *(pprodPartial + 0x2) = ((*(pprodPartial + 0x2) & 0xfffe) | 0x0);
    pprodPartial->cItem = 0x1;
    *(pprodPartial + 0x2) = ((*(pprodPartial + 0x2) & 0xffff) | 0x0);
    /* untranslated: pctT = (int32_t)((uint32_t)(words(*(rgRes+0xe), LOWORD(rgRes[0x3])) * 0x64) / lAlchCost) */
    /* untranslated: pctTooBig = (int32_t)((uint32_t)(words((*(rgRes+0xe) + 0x0), (LOWORD(rgRes[0x3]) + 0x1)) * 0x64) / lAlchCost) */
    if ((HIWORD(pctT) < (HIWORD(pctTooBig) + 0xffff)))
        goto L_15ec;
    else
        goto L_15d6;

L_15d6:
    if ((HIWORD(pctT) > (HIWORD(pctTooBig) + 0xffff)))
        goto L_15e3;
    else
        goto L_15db;

L_15db:
    if ((LOWORD(pctT) <= (LOWORD(pctTooBig) + 0xffff)))
        goto L_15ec;
    else
        goto L_15e3;

L_15e3:
    t_merge_15f8_0001_wide = pctT;
    goto L_15f8;

L_15ec:
    t_merge_15f8_0001_wide = (pctTooBig - 1);

L_15f8:
    pctT = t_merge_15f8_0001_wide;
    /* untranslated: pprodPartial = ((pprodPartial & 0xffff) | loword((int32_t)(words((LOWORD(pctT) & 0x7f), 0x0) << 0x14))) */
    /* untranslated: *(pprodPartial+0x2) = ((*(pprodPartial+0x2) & 0xf80f) | hiword((int32_t)(words((LOWORD(pctT) & 0x7f), 0x0) << 0x14))) */
    LOWORD(rgRes[0x3]) = (LOWORD(rgRes[0x3]) - LOWORD((int32_t)(((uint32_t)((pctT * lAlchCost)) / 0x64))));
    *(rgRes + 0xe) = (*(rgRes + 0xe) - HIWORD((int32_t)(((uint32_t)((pctT * lAlchCost)) / 0x64))));

L_165c:
    cBuilt = (cBuilt + 1);
    /* untranslated: ss:[bp-0x58] = ((LOWORD(prod) + 0xffff) & 0x3ff) */
    /* untranslated: ss:[bp-0x56] = 0x0 */
    prod.cItem = 0x0;
    HIWORD(prod) = (HIWORD(prod) & 0xffff);
    /* untranslated: LOWORD(prod) = (LOWORD(prod) | ss:[bp-0x58]) */
    /* untranslated: HIWORD(prod) = (HIWORD(prod) | ss:[bp-0x56]) */
    LOWORD(prod) = ((LOWORD(prod) & 0xffff) | 0x0);
    prod.pct = 0x0;
    i = 0;
    goto L_1706;

L_16ba:
    rgRes[i] = (rgRes[i] - (rgCost[i] - rgCostPaid[i]));
    rgCostPaid[i] = 0;
    i = (i + 1);

L_1706:
    if ((i >= 4))
        goto L_108f;
    else
        goto L_170c;

L_170c:

L_1712:
    if ((cBuilt <= 0))
        goto L_17b7;
    else
        goto L_171b;

L_171b:
    if ((prod.grobj != 0x1))
        goto L_17b7;
    else
        goto L_1737;

L_1737:
    if ((0x0 != 0x0))
        goto L_17b7;
    else
        goto L_173f;

L_173f:
    if ((prod.iItem != 0xb))
        goto L_1763;
    else
        goto L_175b;

L_175b:
    if ((0x0 == 0x0))
        goto L_1787;
    else
        goto L_1763;

L_1763:
    if ((prod.iItem != 0x3))
        goto L_17b7;
    else
        goto L_177f;

L_177f:
    if ((0x0 != 0x0))
        goto L_17b7;
    else
        goto L_1787;

L_1787:
    cAlchemy = (cAlchemy + cBuilt);
    i = 0;
    goto L_17ae;

L_1795:
    rgRes[i] = (rgRes[i] + (uint32_t)(cBuilt));
    i = (i + 1);

L_17ae:
    if ((i < 3))
        goto L_1795;
    else
        goto L_17b7;

L_17b7:
    if ((cAlchemy == 0))
        goto L_17fb;
    else
        goto L_17c0;

L_17c0:
    if ((fCalcOnly != 0))
        goto L_17fb;
    else
        goto L_17c9;

L_17c9:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_17fb;
    else
        goto L_17d9;

L_17d9:
    FSendPlrMsg2(lppl->iPlayer, 0x8c, lppl->id, lppl->id, cAlchemy);

L_17fb:
    if ((pmdStatus == 0x0))
        goto L_18d1;
    else
        goto L_1804;

L_1804:
    if ((fAutoBuild != 2))
        goto L_1827;
    else
        goto L_180d;

L_180d:
    if ((cBuilt <= 0))
        goto L_181c;
    else
        goto L_1816;

L_1816:
    t_merge_181f_0001 = 0x3;
    goto L_181f;

L_181c:
    t_merge_181f_0001 = 0x4;

L_181f:
    *(pmdStatus) = t_merge_181f_0001;
    goto L_18d1;

L_1827:
    if ((fAutoBuild == 0))
        goto L_1866;
    else
        goto L_1830;

L_1830:
    if ((prod.cItem != 0x0))
        goto L_1866;
    else
        goto L_1844;

L_1844:
    if ((0x0 != 0x0))
        goto L_1866;
    else
        goto L_184c;

L_184c:
    if ((cBuilt <= 0))
        goto L_185b;
    else
        goto L_1855;

L_1855:
    t_merge_185e_0001 = 0x1;
    goto L_185e;

L_185b:
    t_merge_185e_0001 = 0x2;

L_185e:
    *(pmdStatus) = t_merge_185e_0001;
    goto L_18d1;

L_1866:
    if ((cBuilt != 0))
        goto L_18a4;
    else
        goto L_186f;

L_186f:
    if ((LOWORD(pctInitial) != prod.pct))
        goto L_1899;
    else
        goto L_188b;

L_188b:
    if ((HIWORD(pctInitial) != 0x0))
        goto L_1899;
    else
        goto L_1893;

L_1893:
    t_merge_189c_0001 = 0x7;
    goto L_189c;

L_1899:
    t_merge_189c_0001 = 0x6;

L_189c:
    *(pmdStatus) = t_merge_189c_0001;
    goto L_18d1;

L_18a4:
    if ((prod.cItem != 0x0))
        goto L_18ca;
    else
        goto L_18b8;

L_18b8:
    if ((0x0 != 0x0))
        goto L_18ca;
    else
        goto L_18c0;

L_18c0:
    *(pmdStatus) = 0;
    goto L_18d1;

L_18ca:
    *(pmdStatus) = 5;

L_18d1:
    if ((fCalcOnly != 0))
        goto L_18f3;
    else
        goto L_18da;

L_18da:
    if ((fAutoBuild != 0))
        goto L_18f3;
    else
        goto L_18e3;

L_18e3:
    LOWORD(lpprod) = LOWORD(prod);
    *(lpprod + 0x2) = HIWORD(prod);

L_18f3:
    if ((fAutoBuild == 0))
        goto L_19a5;
    else
        goto L_18fc;

L_18fc:
    if ((pprodPartial == 0x0))
        goto L_19a5;
    else
        goto L_1905;

L_1905:
    if ((pprodPartial->cItem != 0x0))
        goto L_19a5;
    else
        goto L_191b;

L_191b:
    if ((0x0 != 0x0))
        goto L_19a5;
    else
        goto L_1923;

L_1923:
    if ((0x0 < 0x0))
        goto L_19a5;
    else
        goto L_193f;

L_193f:
    if ((0x0 > 0x0))
        goto L_194c;
    else
        goto L_1944;

L_1944:
    if ((prod.pct <= 0x0))
        goto L_19a5;
    else
        goto L_194c;

L_194c:
    pprodPartial = LOWORD(prod);
    *(pprodPartial + 0x2) = HIWORD(prod);
    pprodPartial->cItem = 0x1;
    *(pprodPartial + 0x2) = ((*(pprodPartial + 0x2) & 0xffff) | 0x0);
    /* untranslated: pprodPartial = (pprodPartial->cItem | loword((int32_t)(words((LOWORD(iobjOther) & 0x7f), 0x0) << 0xa))) */
    /* untranslated: *(pprodPartial+0x2) = ((*(pprodPartial+0x2) & 0xfffe) | hiword((int32_t)(words((LOWORD(iobjOther) & 0x7f), 0x0) << 0xa))) */

L_19a5:

L_19ab:
    return cBuilt;
}
