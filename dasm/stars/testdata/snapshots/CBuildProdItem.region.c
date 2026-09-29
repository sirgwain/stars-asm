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
    int16_t  t_merge_0d4c_0001;
    uint16_t t_scratch_m56;
    uint16_t t_scratch_m56_2;
    uint16_t t_scratch_m56_3;
    int32_t  t_merge_126a_0001;
    int32_t  t_merge_15f8_0001;
    int16_t  t_merge_189c_0001;

    cAlchemy = 0;
    pctInitial = lpprod->pct;
    prod = *lpprod;
    GetProductionCosts(lppl, lpprod, rgCost, lppl->iPlayer, 1);
    cBuilt = 0;
    if (prod.grobj == grobjPlanet && prod.iItem < mdIdleFactory) {
        t_merge_0d4c_0001 = 1;
    } else {
        t_merge_0d4c_0001 = 0;
    }
    fAutoBuild = t_merge_0d4c_0001;
    if (fAutoBuild != 0) {
        cMax = 1000;
        switch (prod.iItem) {
        case iobjMine:
            iobjOther = mdIdleMine;
            t_scratch_m56 = lppl->cMines;
            cMax = CMaxOperableMines(lppl, lppl->iPlayer, 1) - t_scratch_m56;
            break;
        case iobjFactory:
            iobjOther = mdIdleFactory;
            t_scratch_m56_2 = lppl->cFactories;
            cMax = CMaxOperableFactories(lppl, lppl->iPlayer, 1) - t_scratch_m56_2;
            break;
        case iobjDefense:
            iobjOther = mdIdleDefense;
            t_scratch_m56_3 = lppl->cDefenses;
            cMax = CMaxOperableDefenses(lppl, lppl->iPlayer, 1) - t_scratch_m56_3;
            break;
        case iobjAlchemy:
            iobjOther = mdIdleAlchemy;
            break;
        case iobjMinTerraform:
        case iobjMaxTerraform:
            iobjOther = mdIdleTerraform;
            cMax = IpctCanTerraformLppl(lppl);
            if (cMax <= 0 || prod.iItem != iobjMinTerraform || ChgPopFromPlanet(lppl, 0) < 0 || PctPlanetDesirability(lppl, lppl->iPlayer) <= 0)
                break;
            cMax = 0;
            break;
        case iobjPacket:
            iobjOther = iobjPacketMixed;
            if (IWarpMAFromLppl(lppl, 0x0) == 0 || lppl->idFling == 0x0) {
                cMax = 0;
            }
        default:
        }
        if (cMax < 0) {
            cMax = 0;
        }
        if ((uint32_t)prod.cItem > (uint32_t)(int32_t)cMax || prod.iItem == iobjAlchemy) {
            prod.cItem = cMax;
        }
    }
    for (i = 0; i < 4; i++) {
        rgCostPaid[i] = (uint32_t)((uint32_t)(rgCost[i] * prod.pct) / 0x64);
    }
    while (1) {
        if (prod.cItem <= 0x0)
            goto L_1712;
        for (i = 0; i < 4 && rgCost[i] - rgCostPaid[i] <= rgRes[i]; i++) {
        }
        if (i >= 4) {
            cBuilt = cBuilt + 1;
            prod.cItem = prod.cItem - 0x1;
            prod.pct = 0x0;
            for (i = 0; i < 4; i++) {
                rgRes[i] = rgRes[i] - (rgCost[i] - rgCostPaid[i]);
                rgCostPaid[i] = 0;
            }
        } else {
            fMineralBlocked = 0;
            fResourceBlocked = 0;
            pct = 100;
            for (i = 0; i < 4; i++) {
                if (rgCost[i] > 0) {
                    if (rgRes[i] < rgCost[i]) {
                        pctT = (int32_t)((int32_t)((rgRes[i] + rgCostPaid[i]) * 0x64) / rgCost[i]);
                        pctTooBig = (int32_t)((int32_t)((rgRes[i] + rgCostPaid[i] + 0x1) * 0x64) / rgCost[i]);
                        t_merge_126a_0001 = pctT <= pctTooBig - 1 ? pctTooBig - 1 : pctT;
                        pctT = t_merge_126a_0001;
                    } else {
                        pctT = 100;
                    }
                    if (pctT < pct) {
                        lMinNeeded = rgCost[i] - rgCostPaid[i] - rgRes[i];
                        pct = pctT;
                        if (i != 3) {
                            fMineralBlocked = 1;
                        } else {
                            fResourceBlocked = 1;
                        }
                    }
                }
            }
            if (fMineralBlocked == 0 || fAutoBuild == 0) {
                for (i = 0; i < 4; i++) {
                    AddCost = (int32_t)((int32_t)(rgCost[i] * pct) / 100) - rgCostPaid[i];
                    rgRes[i] = rgRes[i] - AddCost;
                    rgCostPaid[i] = rgCostPaid[i] + AddCost;
                }
                prod.pct = LOWORD(pct);
                if (fAlchemy == 0 || fResourceBlocked != 0)
                    goto L_1712;
            } else if (fAlchemy == 0) {
                break;
            }
            lAlchCost = (uint32_t)(GetRaceGrbit(&rgplr[lppl->iPlayer], ibitRaceMineralAlchemy) == 0 ? 0x64 : 0x19);
            cCanBuild = (int32_t)(rgRes[3] / lAlchCost);
            if (cCanBuild > lMinNeeded) {
                cCanBuild = lMinNeeded;
            }
            if (cCanBuild > 0) {
                for (i = 0; i < 3; i++) {
                    rgRes[i] = rgRes[i] + cCanBuild;
                }
                rgRes[i] = rgRes[i] - (uint32_t)(lAlchCost * cCanBuild);
                cAlchemy = cAlchemy + LOWORD(cCanBuild);
            }
            if (cCanBuild != lMinNeeded)
                goto L_14de;
        }
    }
    fAutoBuild = 2;
    goto L_1712;
L_14de:
    if (rgRes[3] > 0 && pprodPartial != 0x0) {
        memset(pprodPartial, 0, sizeof(PROD));
        pprodPartial->grobj = grobjPlanet;
        pprodPartial->iItem = mdIdleAlchemy;
        pprodPartial->cItem = 0x1;
        pctT = (int32_t)((int32_t)(rgRes[3] * 100) / lAlchCost);
        pctTooBig = (int32_t)((int32_t)((rgRes[3] + 1) * 0x64) / lAlchCost);
        t_merge_15f8_0001 = pctT <= pctTooBig - 1 ? pctTooBig - 1 : pctT;
        pctT = t_merge_15f8_0001;
        pprodPartial->pct = LOWORD(pctT);
        rgRes[3] = rgRes[3] - (int32_t)((int32_t)(pctT * lAlchCost) / 100);
    }
L_1712:
    if (cBuilt > 0 && prod.grobj == grobjPlanet && (prod.iItem == mdIdleAlchemy || prod.iItem == iobjAlchemy)) {
        cAlchemy = cAlchemy + cBuilt;
        for (i = 0; i < 3; i++) {
            rgRes[i] = rgRes[i] + (int32_t)cBuilt;
        }
    }
    if (cAlchemy != 0 && fCalcOnly == 0 && gd.fGeneratingTurn != 0x0) {
        FSendPlrMsg2(lppl->iPlayer, 140, lppl->id, lppl->id, cAlchemy);
    }
    if (pmdStatus != 0x0) {
        if (fAutoBuild != 2) {
            if (fAutoBuild == 0 || prod.cItem != 0x0) {
                if (cBuilt != 0) {
                    if (prod.cItem != 0x0) {
                        *pmdStatus = 5;
                    } else {
                        *pmdStatus = 0;
                    }
                } else {
                    t_merge_189c_0001 = pctInitial == prod.pct ? 7 : 6;
                    *pmdStatus = t_merge_189c_0001;
                }
            } else {
                *pmdStatus = cBuilt <= 0 ? 2 : 1;
            }
        } else {
            *pmdStatus = cBuilt <= 0 ? 4 : 3;
        }
    }
    if (fCalcOnly == 0 && fAutoBuild == 0) {
        *lpprod = prod;
    }
    if (fAutoBuild != 0 && pprodPartial != 0x0 && pprodPartial->cItem == 0x0 && prod.pct > 0x0) {
        *pprodPartial = prod;
        pprodPartial->cItem = 0x1;
        pprodPartial->iItem = LOWORD(iobjOther);
    }
    return cBuilt;
}
