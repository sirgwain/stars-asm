int16_t GetShdefScannerRange(SHDEF *lpshdef, int16_t iplr, int16_t *pdPlanRange, int16_t *ppctDetect, int16_t *piSteal) {
    int16_t  chs;
    HS      *lphs;
    int16_t  dRangeT2;
    double   lBIR4;
    int16_t  dRangeT;
    int16_t  fHasScanner;
    int16_t  iScanner;
    int16_t  fBuiltIn;
    int16_t  cDetectors;
    double   lPlanRange4;
    int16_t  dRange;
    double   lT;
    int16_t  iSteal;
    int16_t  j;
    double   lBIPR4;
    double   lRange4;
    int16_t  t_merge_5142_0001;
    SCANNER *t_call_52ec;
    int16_t  t_merge_53ea_0001;

    lRange4 = 0.0;
    lPlanRange4 = 0.0;
    fHasScanner = 0;
    iSteal = 0;
    cDetectors = 0;
    if (iplr == -1 || GetRaceStat(&rgplr[iplr], rsMajorAdv) != raNone) {
        t_merge_5142_0001 = 0;
    } else {
        t_merge_5142_0001 = 1;
    }
    fBuiltIn = t_merge_5142_0001;
    lBIR4 = -1.0;
    lBIPR4 = -1.0;
    if (ppctDetect != 0x0) {
        *ppctDetect = 100;
    }
    if (fBuiltIn != 0) {
        switch (lpshdef->hul.ihuldef) {
        case ihuldefScout:
        case ihuldefDestroyer:
        case ihuldefFrigate:
            if (lBIR4 < 0.0) {
                if (game.fTutorial == 0x0) {
                    lBIPR4 = (double)(int32_t)((int16_t)rgplr[iplr].rgTech[4] * 10);
                    lBIR4 = lBIPR4 * 2.0;
                    lBIPR4 = lBIPR4 * lBIPR4;
                    lBIPR4 = lBIPR4 * lBIPR4;
                    lBIR4 = lBIR4 * lBIR4;
                    lBIR4 = lBIR4 * lBIR4;
                } else {
                    lBIR4 = 2.56e+06;
                    lBIPR4 = 160000.0;
                }
            }
            lRange4 = lBIR4;
            lPlanRange4 = lBIPR4;
        default:
        }
    }
    lphs = lpshdef->hul.rghs;
    chs = lpshdef->hul.chs;
    j = 0;
    while (j < chs) {
        if (lphs->cItem != 0x0) {
            if (lphs->grhst == hstScanner) {
                fHasScanner = 1;
                iScanner = lphs->iItem;
                t_call_52ec = LpscannerFromId(lphs->iItem);
                dRangeT = t_call_52ec->dRange;
                lT = (double)(int32_t)t_call_52ec->dRange;
                lT = lT * lT;
                lT = lT * lT;
                lT = lT * (double)(uint32_t)lphs->cItem;
                lRange4 = lRange4 + lT;
                dRangeT = LpscannerFromId(iScanner)->grfAbilities;
                switch (iScanner) {
                case 6:
                    dRangeT = 45;
                    goto LPlanScan;
                case 5:
                    dRangeT = 0;
                    iSteal = iSteal | 0x1;
                    goto LPlanScan;
                case 14:
                    dRangeT = 120;
                    iSteal = iSteal | 0x3;
                    goto LPlanScan;
                default:
                    if (dRangeT > 0) {
                        if (dRangeT != 1) {
                            if (dRangeT != 2) {
                                t_merge_53ea_0001 = 200;
                            } else {
                                t_merge_53ea_0001 = 100;
                            }
                        } else {
                            t_merge_53ea_0001 = 50;
                        }
                        dRangeT = t_merge_53ea_0001;
                        goto LPlanScan;
                    }
                }
                goto L_5298;
            }
            if (lphs->grhst != hstArmor || lphs->iItem != iarmorMegaPolyShell) {
                if (lphs->grhst != hstBeam || lphs->iItem != ibeamMultiContainedMunition) {
                    if (lphs->grhst != hstShield || lphs->iItem != ishieldLangstonShell) {
                        if (ppctDetect == 0x0 || lphs->grhst != hstSpecialE || lphs->iItem != ispecialETachyonDetector)
                            goto L_5298;
                        cDetectors = cDetectors + lphs->cItem;
                        goto L_5298;
                    }
                    dRangeT = 50;
                    dRangeT2 = 25;
                } else {
                    dRangeT = 150;
                    dRangeT2 = 75;
                }
            } else {
                dRangeT = 80;
                dRangeT2 = 40;
            }
            lT = (double)(int32_t)dRangeT;
            lT = lT * lT;
            lT = lT * lT;
            lT = lT * (double)(uint32_t)lphs->cItem;
            lRange4 = lRange4 + lT;
            dRangeT = dRangeT2;
        LPlanScan:
            lT = (double)(int32_t)dRangeT;
            lT = lT * lT;
            lT = lT * lT;
            lT = lT * (double)(uint32_t)lphs->cItem;
            lPlanRange4 = lPlanRange4 + lT;
        }
    L_5298:
        j = j + 1;
        lphs = lphs + 1;
    }
    if (lRange4 <= 0.0 && fHasScanner == 0) {
        dRange = -1;
    } else {
        dRange = LOWORD((int32_t)sqrt(sqrt(lRange4)));
        if (iplr != -1 && GetRaceGrbit(&rgplr[iplr], ibitRaceNoAdvScanner) != 0) {
            dRange = dRange * 2;
        }
    }
    if (pdPlanRange != 0x0) {
        *pdPlanRange = LOWORD((int32_t)sqrt(sqrt(lPlanRange4)));
    }
    if (piSteal != 0x0) {
        *piSteal = iSteal;
    }
    if (ppctDetect != 0x0) {
        if (cDetectors >= 18) {
            cDetectors = 17;
        }
        *ppctDetect = vrgbTachyon[cDetectors];
    }
    return dRange;
}
