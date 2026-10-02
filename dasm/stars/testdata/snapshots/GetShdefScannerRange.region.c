int16_t GetShdefScannerRange(SHDEF *lpshdef, int16_t iplr, int16_t *pdPlanRange, int16_t *ppctDetect, int16_t *piSteal) {
    int16_t  chs;
    HS      *lphs;
    int16_t  dRangeT2;
    double   lBIR4;
    int16_t  dRangeT;
    int16_t  fHasScanner;
    iscanner iScanner;
    int16_t  fBuiltIn;
    int16_t  cDetectors;
    double   lPlanRange4;
    int16_t  dRange;
    double   lT;
    int16_t  iSteal;
    int16_t  j;
    double   lBIPR4;
    double   lRange4;
    SCANNER *t_call_52ec;

    lRange4 = (double)0;
    lPlanRange4 = (double)0;
    fHasScanner = FALSE;
    iSteal = 0;
    cDetectors = 0;
    fBuiltIn = iplr != -1 && GetRaceStat(&rgplr[iplr], rsMajorAdv) == raNone;
    lBIR4 = -1.0;
    lBIPR4 = -1.0;
    if (ppctDetect != 0) {
        *ppctDetect = 100;
    }
    if (fBuiltIn != 0) {
        switch (lpshdef->hul.ihuldef) {
        case ihuldefScout:
        case ihuldefDestroyer:
        case ihuldefFrigate:
            if ((long double)lBIR4 < (long double)0) {
                if (game.fTutorial != 0) {
                    lBIR4 = 2.56e+06;
                    lBIPR4 = 160000.0;
                } else {
                    lBIPR4 = (double)(int16_t)(rgplr[iplr].rgTech[4] * 10);
                    lBIR4 = (double)((long double)lBIPR4 * 2);
                    lBIPR4 = (double)((long double)lBIPR4 * lBIPR4);
                    lBIPR4 = (double)((long double)lBIPR4 * lBIPR4);
                    lBIR4 = (double)((long double)lBIR4 * lBIR4);
                    lBIR4 = (double)((long double)lBIR4 * lBIR4);
                }
            }
            lRange4 = lBIR4;
            lPlanRange4 = lBIPR4;
        }
    }
    lphs = lpshdef->hul.rghs;
    chs = lpshdef->hul.chs;
    j = 0;
    while (j < chs) {
        if (lphs->cItem != 0) {
            if (lphs->grhst == hstScanner) {
                fHasScanner = TRUE;
                iScanner = lphs->iItem;
                t_call_52ec = LpscannerFromId(lphs->iItem);
                dRangeT = t_call_52ec->dRange;
                lT = (double)t_call_52ec->dRange;
                lT = (double)((long double)lT * lT);
                lT = (double)((long double)lT * lT);
                lT = (double)((long double)lT * (uint32_t)lphs->cItem);
                lRange4 = (double)((long double)lRange4 + lT);
                dRangeT = LpscannerFromId(iScanner)->grfAbilities;
                switch (iScanner) {
                case iscannerChameleonScanner:
                    dRangeT = 45;
                    goto LPlanScan;
                case iscannerPickPocketScanner:
                    dRangeT = 0;
                    iSteal |= 1;
                    goto LPlanScan;
                case iscannerRobberBaronScanner:
                    dRangeT = 120;
                    iSteal |= 3;
                    goto LPlanScan;
                default:
                    if (dRangeT > 0) {
                        if (dRangeT == 1) {
                            dRangeT = 50;
                            goto LPlanScan;
                        }
                        if (dRangeT == 2) {
                            dRangeT = 100;
                            goto LPlanScan;
                        }
                        dRangeT = 200;
                        goto LPlanScan;
                    }
                }
                goto L_5298;
            }
            if (lphs->grhst == hstArmor && lphs->iItem == iarmorMegaPolyShell) {
                dRangeT = 80;
                dRangeT2 = 40;
            } else if (lphs->grhst == hstBeam && lphs->iItem == ibeamMultiContainedMunition) {
                dRangeT = 150;
                dRangeT2 = 75;
            } else if (lphs->grhst == hstShield && lphs->iItem == ishieldLangstonShell) {
                dRangeT = 50;
                dRangeT2 = 25;
            } else {
                if (ppctDetect == 0 || lphs->grhst != hstSpecialE || lphs->iItem != ispecialETachyonDetector)
                    goto L_5298;
                cDetectors += lphs->cItem;
                goto L_5298;
            }
            lT = (double)dRangeT;
            lT = (double)((long double)lT * lT);
            lT = (double)((long double)lT * lT);
            lT = (double)((long double)lT * (uint32_t)lphs->cItem);
            lRange4 = (double)((long double)lRange4 + lT);
            dRangeT = dRangeT2;
        LPlanScan:
            lT = (double)dRangeT;
            lT = (double)((long double)lT * lT);
            lT = (double)((long double)lT * lT);
            lT = (double)((long double)lT * (uint32_t)lphs->cItem);
            lPlanRange4 = (double)((long double)lPlanRange4 + lT);
        }
    L_5298:
        j++;
        lphs++;
    }
    if ((long double)lRange4 > (long double)0 || fHasScanner != 0) {
        dRange = LOWORD((int32_t)sqrt((double)sqrt(lRange4)));
        if (iplr != -1 && GetRaceGrbit(&rgplr[iplr], ibitRaceNoAdvScanner) != 0) {
            dRange *= 2;
        }
    } else {
        dRange = -1;
    }
    if (pdPlanRange != 0) {
        *pdPlanRange = LOWORD((int32_t)sqrt((double)sqrt(lPlanRange4)));
    }
    if (piSteal != 0) {
        *piSteal = iSteal;
    }
    if (ppctDetect != 0) {
        if (cDetectors >= 18) {
            cDetectors = 17;
        }
        *ppctDetect = vrgbTachyon[cDetectors];
    }
    return dRange;
}
