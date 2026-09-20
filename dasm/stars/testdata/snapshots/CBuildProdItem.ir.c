int16_t CBuildProdItem(PLANET *lppl, PROD *lpprod, PROD *pprodPartial, int32_t *rgRes, int16_t fAlchemy, int16_t *pmdStatus, int16_t fCalcOnly) {
    int32_t      pctT;
    int16_t      cMax;
    ProdItemType iobjOther;
    int32_t      cCanBuild;
    int32_t      lMinNeeded;
    int32_t      lAlchCost;
    PROD         prod;
    int16_t      fAutoBuild;
    int16_t      cBuilt;
    int16_t      cAlchemy;
    int32_t      rgCostPaid[4];
    int16_t      i;
    int16_t      fResourceBlocked;
    int32_t      pctInitial;
    int32_t      pctTooBig;
    int32_t      pct;
    int32_t      rgCost[4];
    int16_t      fMineralBlocked;
    int32_t      AddCost;
    uint16_t     t_merge_0d4c_0001;
    uint16_t     t_scratch_m56;
    uint16_t     t_scratch_m56_2;
    uint16_t     t_scratch_m56_3;
    int32_t      t_merge_126a_0001;
    uint16_t     t_merge_1410_0001;
    int32_t      t_merge_15f8_0001;
    uint16_t     t_merge_181f_0001;
    uint16_t     t_merge_185e_0001;
    uint16_t     t_merge_189c_0001;

L_0c92:
    cAlchemy = 0;
    pctInitial = lpprod->pct;
    prod = *(lpprod);
    GetProductionCosts(lppl, lpprod, rgCost, lppl->iPlayer, 1);
    cBuilt = 0;
    if ((prod.grobj != grobjPlanet))
        goto L_0d49;
    else
        goto L_0d1a;

L_0d1a:
    if ((prod.iItem < mdIdleFactory))
        goto L_0d43;
    else
        goto L_0d49;

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
    iobjOther = mdIdleMine;
    t_scratch_m56 = lppl->cMines;
    cMax = (CMaxOperableMines(lppl, lppl->iPlayer, 1) - t_scratch_m56);
    goto L_0f9f;

L_0dbe:
    iobjOther = mdIdleFactory;
    t_scratch_m56_2 = lppl->cFactories;
    cMax = (CMaxOperableFactories(lppl, lppl->iPlayer, 1) - t_scratch_m56_2);
    goto L_0f9f;

L_0e08:
    iobjOther = mdIdleDefense;
    t_scratch_m56_3 = lppl->cDefenses;
    cMax = (CMaxOperableDefenses(lppl, lppl->iPlayer, 1) - t_scratch_m56_3);
    goto L_0f9f;

L_0e4a:
    iobjOther = mdIdleAlchemy;
    goto L_0f9f;

L_0e57:
    iobjOther = mdIdleTerraform;
    cMax = IpctCanTerraformLppl(lppl);
    if ((cMax <= 0))
        goto L_0f9f;
    else
        goto L_0e7b;

L_0e7b:
    if ((prod.iItem != iobjMinTerraform))
        goto L_0f9f;
    else
        goto L_0e9f;

L_0e9f:
    if ((ChgPopFromPlanet(lppl, 0) < 0))
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

L_0ee8:
    goto L_0f9f;

L_0eeb:
    iobjOther = iobjPacketMixed;
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

L_0f26:
    goto L_0f9f;

L_0f2c:
    if ((prod.iItem != iobjMine))
        goto L_0f3c;
    else
        goto L_0d74;

L_0f3c:
    if ((prod.iItem != iobjFactory))
        goto L_0f4c;
    else
        goto L_0dbe;

L_0f4c:
    if ((prod.iItem != iobjDefense))
        goto L_0f5c;
    else
        goto L_0e08;

L_0f5c:
    if ((prod.iItem != iobjAlchemy))
        goto L_0f6c;
    else
        goto L_0e4a;

L_0f6c:
    if ((prod.iItem != iobjMinTerraform))
        goto L_0f7c;
    else
        goto L_0e57;

L_0f7c:
    if ((prod.iItem != iobjMaxTerraform))
        goto L_0f8c;
    else
        goto L_0e57;

L_0f8c:
    if ((prod.iItem != iobjPacket))
        goto L_0f9f;
    else
        goto L_0eeb;

L_0f9f:
    if ((cMax >= 0))
        goto L_0fad;
    else
        goto L_0fa8;

L_0fa8:
    cMax = 0;

L_0fad:
    if (((uint32_t)(prod.cItem) <= (uint32_t)((int32_t)(cMax))))
        goto L_0fdc;
    else
        goto L_1000;

L_0fdc:
    if ((prod.iItem != iobjAlchemy))
        goto L_102c;
    else
        goto L_1000;

L_1000:
    prod.cItem = cMax;

L_102c:
    i = 0;
    goto L_1086;

L_1034:
    rgCostPaid[i] = (uint32_t)(((uint32_t)((rgCost[i] * prod.pct)) / 0x64));
    i = (i + 1);

L_1086:
    if ((i < 4))
        goto L_1034;
    else
        goto L_108f;

L_108f:
    if ((prod.cItem <= 0x0))
        goto L_1712;
    else
        goto L_10b0;

L_10b0:
    i = 0;
    goto L_1101;

L_10b8:
    if (((rgCost[i] - rgCostPaid[i]) <= rgRes[i]))
        goto L_10fd;
    else
        goto L_110a;

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
    if ((rgCost[i] <= 0))
        goto L_12e6;
    else
        goto L_1152;

L_1152:
    if ((rgRes[i] < rgCost[i]))
        goto L_1190;
    else
        goto L_1183;

L_1183:
    pctT = 100;
    goto L_1270;

L_1190:
    pctT = (int32_t)(((uint32_t)(((rgRes[i] + rgCostPaid[i]) * 0x64)) / rgCost[i]));
    pctTooBig = (int32_t)(((uint32_t)((((rgRes[i] + rgCostPaid[i]) + 0x1) * 0x64)) / rgCost[i]));
    if ((pctT <= (pctTooBig - 1)))
        goto L_125e;
    else
        goto L_1255;

L_1255:
    t_merge_126a_0001 = pctT;
    goto L_126a;

L_125e:
    t_merge_126a_0001 = (pctTooBig - 1);

L_126a:
    pctT = t_merge_126a_0001;

L_1270:
    if ((pctT < pct))
        goto L_128b;
    else
        goto L_12e6;

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
        goto L_1314;

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
    prod.pct = LOWORD(pct);
    if ((fAlchemy == 0))
        goto L_1712;
    else
        goto L_13d5;

L_13d5:
    if ((fResourceBlocked != 0))
        goto L_1712;
    else
        goto LAlchemize;

LAlchemize:
    if ((GetRaceGrbit(&(rgplr[lppl->iPlayer]), ibitRaceMineralAlchemy) == 0))
        goto L_140c;
    else
        goto L_1405;

L_1405:
    t_merge_1410_0001 = 0x19;
    goto L_1410;

L_140c:
    t_merge_1410_0001 = 0x64;

L_1410:
    lAlchCost = (uint32_t)(t_merge_1410_0001);
    cCanBuild = (int32_t)((rgRes[3] / lAlchCost));
    if ((cCanBuild <= lMinNeeded))
        goto L_1457;
    else
        goto L_144b;

L_144b:
    cCanBuild = lMinNeeded;

L_1457:
    if ((cCanBuild <= 0))
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
    if ((cCanBuild != lMinNeeded))
        goto L_14de;
    else
        goto L_108f;

L_14de:
    if ((rgRes[3] <= 0))
        goto L_1712;
    else
        goto L_14f8;

L_14f8:
    if ((pprodPartial == 0x0))
        goto L_1712;
    else
        goto L_1501;

L_1501:
    memset(pprodPartial, 0, 0x4);
    pprodPartial->grobj = grobjPlanet;
    pprodPartial->iItem = mdIdleAlchemy;
    pprodPartial->cItem = 0x1;
    pctT = (int32_t)(((uint32_t)((rgRes[3] * 100)) / lAlchCost));
    pctTooBig = (int32_t)(((uint32_t)(((rgRes[3] + 1) * 0x64)) / lAlchCost));
    if ((pctT <= (pctTooBig - 1)))
        goto L_15ec;
    else
        goto L_15e3;

L_15e3:
    t_merge_15f8_0001 = pctT;
    goto L_15f8;

L_15ec:
    t_merge_15f8_0001 = (pctTooBig - 1);

L_15f8:
    pctT = t_merge_15f8_0001;
    pprodPartial->pct = LOWORD(pctT);
    rgRes[3] = (rgRes[3] - (int32_t)(((uint32_t)((pctT * lAlchCost)) / 0x64)));

L_1656:
    goto L_1712;

L_165c:
    cBuilt = (cBuilt + 1);
    prod.cItem = (prod.cItem + 0xffff);
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
    goto L_16ba;

L_1712:
    if ((cBuilt <= 0))
        goto L_17b7;
    else
        goto L_171b;

L_171b:
    if ((prod.grobj != grobjPlanet))
        goto L_17b7;
    else
        goto L_173f;

L_173f:
    if ((prod.iItem != mdIdleAlchemy))
        goto L_1763;
    else
        goto L_1787;

L_1763:
    if ((prod.iItem != iobjAlchemy))
        goto L_17b7;
    else
        goto L_1787;

L_1787:
    cAlchemy = (cAlchemy + cBuilt);
    i = 0;
    goto L_17ae;

L_1795:
    rgRes[i] = (rgRes[i] + (int32_t)(cBuilt));
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
    FSendPlrMsg2(lppl->iPlayer, 140, lppl->id, lppl->id, cAlchemy);

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
    if ((pctInitial != prod.pct))
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
    *(lpprod) = prod;

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
        goto L_1923;

L_1923:
    if ((prod.pct <= 0x0))
        goto L_19a5;
    else
        goto L_194c;

L_194c:
    *(pprodPartial) = prod;
    pprodPartial->cItem = 0x1;
    pprodPartial->iItem = LOWORD(iobjOther);

L_19a5:

L_19ab:
    return cBuilt;
}
