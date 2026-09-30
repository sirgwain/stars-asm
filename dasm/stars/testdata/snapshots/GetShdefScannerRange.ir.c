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
    SCANNER *t_call_52ec;

L_50d0:
    lRange4 = 0.0;
    lPlanRange4 = 0.0;
    fHasScanner = 0;
    iSteal = 0;
    cDetectors = 0;
    if (iplr == -1)
        goto L_513f;
    else
        goto L_5119;

L_5119:
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) != raNone)
        goto L_513f;
    else
        goto L_5139;

L_5139:
    fBuiltIn = 1;
    goto L_5142;

L_513f:
    fBuiltIn = 0;

L_5142:
    lBIR4 = -1.0;
    lBIPR4 = -1.0;
    if (ppctDetect == 0)
        goto L_516b;
    else
        goto L_5164;

L_5164:
    *ppctDetect = 100;

L_516b:
    if (fBuiltIn == 0)
        goto L_5272;
    else
        goto L_5174;

L_5174:
    if (lpshdef->hul.ihuldef == ihuldefScout)
        goto L_5198;
    else
        goto L_5180;

L_5180:
    if (lpshdef->hul.ihuldef == ihuldefDestroyer)
        goto L_5198;
    else
        goto L_518c;

L_518c:
    if (lpshdef->hul.ihuldef != ihuldefFrigate)
        goto L_5272;
    else
        goto L_5198;

L_5198:
    if (lBIR4 >= 0.0)
        goto L_525e;
    else
        goto L_51b6;

L_51b6:
    if (game.fTutorial == 0)
        goto L_51e3;
    else
        goto L_51ca;

L_51ca:
    lBIR4 = 2.56e+06;
    lBIPR4 = 160000.0;
    goto L_525e;

L_51e3:
    lBIPR4 = (double)(int16_t)(rgplr[iplr].rgTech[4] * 10);
    lBIR4 = lBIPR4 * 2.0;
    lBIPR4 *= lBIPR4;
    lBIPR4 *= lBIPR4;
    lBIR4 *= lBIR4;
    lBIR4 *= lBIR4;

L_525e:
    lRange4 = lBIR4;
    lPlanRange4 = lBIPR4;

L_5272:
    lphs = lpshdef->hul.rghs;
    chs = lpshdef->hul.chs;
    j = 0;
    goto L_52ab;

L_5298:
    j++;
    lphs++;

L_52ab:
    if (j >= chs)
        goto L_5591;
    else
        goto L_52b6;

L_52b6:
    if (lphs->cItem == 0)
        goto L_5298;
    else
        goto L_52cd;

L_52cd:
    if (lphs->grhst != hstScanner)
        goto L_545a;
    else
        goto L_52d9;

L_52d9:
    fHasScanner = 1;
    iScanner = lphs->iItem;
    t_call_52ec = LpscannerFromId(lphs->iItem);
    dRangeT = t_call_52ec->dRange;
    lT = (double)t_call_52ec->dRange;
    lT *= lT;
    lT *= lT;
    lT *= (double)(uint32_t)lphs->cItem;
    lRange4 += lT;
    dRangeT = LpscannerFromId(iScanner)->grfAbilities;
    if (iScanner != 6)
        goto L_5390;
    else
        goto L_5385;

L_5385:
    dRangeT = 45;
    goto LPlanScan;

L_5390:
    if (iScanner != 5)
        goto L_53a8;
    else
        goto L_5399;

L_5399:
    dRangeT = 0;
    iSteal |= 1;
    goto LPlanScan;

L_53a8:
    if (iScanner != 14)
        goto L_53c0;
    else
        goto L_53b1;

L_53b1:
    dRangeT = 120;
    iSteal |= 3;
    goto LPlanScan;

L_53c0:
    if (dRangeT <= 0)
        goto L_5298;
    else
        goto L_53c9;

L_53c9:
    if (dRangeT != 1)
        goto L_53d8;
    else
        goto L_53d2;

L_53d2:
    dRangeT = 50;
    goto L_53ea;

L_53d8:
    if (dRangeT != 2)
        goto L_53e7;
    else
        goto L_53e1;

L_53e1:
    dRangeT = 100;
    goto L_53ea;

L_53e7:
    dRangeT = 200;

L_53ea:

LPlanScan:
    lT = (double)dRangeT;
    lT *= lT;
    lT *= lT;
    lT *= (double)(uint32_t)lphs->cItem;
    lPlanRange4 += lT;

L_5457:
    goto L_5298;

L_545a:
    if (lphs->grhst != hstArmor)
        goto L_54f8;
    else
        goto L_5466;

L_5466:
    if (lphs->iItem != iarmorMegaPolyShell)
        goto L_54f8;
    else
        goto L_5478;

L_5478:
    dRangeT = 80;
    dRangeT2 = 40;

LOddBallScanners:
    lT = (double)dRangeT;
    lT *= lT;
    lT *= lT;
    lT *= (double)(uint32_t)lphs->cItem;
    lRange4 += lT;
    dRangeT = dRangeT2;
    goto LPlanScan;

L_54f8:
    if (lphs->grhst != hstBeam)
        goto L_5526;
    else
        goto L_5504;

L_5504:
    if (lphs->iItem != ibeamMultiContainedMunition)
        goto L_5526;
    else
        goto L_5516;

L_5516:
    dRangeT = 150;
    dRangeT2 = 75;
    goto LOddBallScanners;

L_5526:
    if (lphs->grhst != hstShield)
        goto L_5554;
    else
        goto L_5532;

L_5532:
    if (lphs->iItem != ishieldLangstonShell)
        goto L_5554;
    else
        goto L_5544;

L_5544:
    dRangeT = 50;
    dRangeT2 = 25;
    goto LOddBallScanners;

L_5554:
    if (ppctDetect == 0)
        goto L_5298;
    else
        goto L_555d;

L_555d:
    if (lphs->grhst != hstSpecialE)
        goto L_5298;
    else
        goto L_556a;

L_556a:
    if (lphs->iItem != ispecialETachyonDetector)
        goto L_5298;
    else
        goto L_557c;

L_557c:
    cDetectors += lphs->cItem;

L_558e:
    goto L_5298;

L_5591:
    if (lRange4 > 0.0)
        goto L_55b8;
    else
        goto L_55af;

L_55af:
    if (fHasScanner == 0)
        goto L_5628;
    else
        goto L_55b8;

L_55b8:
    dRange = LOWORD((int32_t)sqrt(sqrt(lRange4)));
    if (iplr == -1)
        goto L_562d;
    else
        goto L_55fd;

L_55fd:
    if (GetRaceGrbit(&rgplr[iplr], ibitRaceNoAdvScanner) == 0)
        goto L_562d;
    else
        goto L_561d;

L_561d:
    dRange *= 2;

L_5625:
    goto L_562d;

L_5628:
    dRange = -1;

L_562d:
    if (pdPlanRange == 0)
        goto L_5674;
    else
        goto L_5636;

L_5636:
    *pdPlanRange = LOWORD((int32_t)sqrt(sqrt(lPlanRange4)));

L_5674:
    if (piSteal == 0)
        goto L_5685;
    else
        goto L_567d;

L_567d:
    *piSteal = iSteal;

L_5685:
    if (ppctDetect == 0)
        goto L_56ac;
    else
        goto L_568e;

L_568e:
    if (cDetectors < 18)
        goto L_569c;
    else
        goto L_5697;

L_5697:
    cDetectors = 17;

L_569c:
    *ppctDetect = vrgbTachyon[cDetectors];

L_56ac:

L_56b2:
    return dRange;
}
