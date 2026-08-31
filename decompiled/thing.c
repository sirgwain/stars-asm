#include "common.h"

THING *LpthNew(int16_t iplr, ThingType ith) {
    int16_t iItem;
    int16_t i;
    THING  *lpth;
    THING   thNew;

L_0000:
    if ((cThing < 0xfd2))
        goto L_001d;
    else
        goto L_0014;

L_0014:
    return 0x0;

L_001d:
    memset(thNew, 0x0, 0x12);
    thNew.idFull = ((thNew.idFull & 0xe1ff) | ((iplr & 0xf) << 0x9));
    thNew.idFull = ((thNew.idFull & 0x1fff) | ((ith & 0x7) << 0xd));
    lpth = lpThings;
    i = 0x0;
    goto L_0087;

L_0074:
    i = (i + 0x1);
    lpth = (lpth + 0x1);

L_0087:
    if ((i >= cThing))
        goto L_00a6;
    else
        goto L_0092;

L_0092:
    if ((thNew.idFull <= lpth->idFull))
        goto L_00a6;
    else
        goto L_009d;

L_009d:

L_00a6:
    if ((i >= cThing))
        goto L_0146;
    else
        goto L_00b1;

L_00b1:
    if ((thNew.idFull != lpth->idFull))
        goto L_0146;
    else
        goto L_00bf;

L_00bf:
    iItem = lpth->id;
    goto L_0114;

L_00ce:
    i = (i + 0x1);
    /* untranslated: ss:[bp-0x1e] = thNew.id */
    /* untranslated: ss:[bp-0x20] = ((thNew.idFull + 0x1) & 0x1ff) */
    thNew.id = 0x0;
    /* untranslated: thNew.idFull = (thNew.idFull | ss:[bp-0x20]) */
    lpth = (lpth + 0x1);
    iItem = (iItem + 0x1);

L_0114:
    if ((i >= cThing))
        goto L_0146;
    else
        goto L_011f;

L_011f:
    if ((iItem < 0x1ff))
        goto L_0132;
    else
        goto L_0129;

L_0129:
    return 0x0;

L_0132:
    if ((lpth->idFull != thNew.idFull))
        goto L_0146;
    else
        goto L_013d;

L_013d:

L_0146:
    if ((cThing < cThingAlloc))
        goto L_01c4;
    else
        goto L_0152;

L_0152:
    cThingAlloc = (cThingAlloc + 0xa);
    if ((LOWORD(lpThings) != 0x0))
        goto L_018a;
    else
        goto L_0161;

L_0161:
    if ((HIWORD(lpThings) != 0x0))
        goto L_018a;
    else
        goto L_016b;

L_016b:
    lpThings = LpAlloc(LOWORD((cThingAlloc * 0x12)), htThings);
    goto L_01ae;

L_018a:
    lpThings = LpReAlloc(&(lpThings), LOWORD((cThingAlloc * 0x12)), htThings);

L_01ae:
    lpth = &(lpThings[i]);

L_01c4:
    if ((i >= cThing))
        goto L_01f6;
    else
        goto L_01cf;

L_01cf:
    fmemmove(&(lpth[0x1]), &(lpth), LOWORD(((cThing - i) * 0x12)));

L_01f6:
    cThing = (cThing + 0x1);
    fmemcpy(&(lpth), &(thNew), 0x12);
    return lpth;
}

void FreeLpth(THING *lpth) {
L_0224:
    if ((LOWORD(lpth) >= ((LOWORD(lpThings) + LOWORD((0x12 * cThing))) + 0xffee)))
        goto L_0294;
    else
        goto L_024b;

L_024b:
    fmemmove(&(lpth), &(lpth[0x1]), LOWORD((((cThing - ((uint32_t)((LOWORD(lpth) - LOWORD(lpThings))) / 0x12)) + 0xffff) * 0x12)));

L_0294:
    cThing = (cThing - 0x1);
    return;
}

int16_t CPlanetsInCircle(POINT pt, int32_t r2) {
    int16_t xStart;
    POINT  *ppt;
    int16_t yEnd;
    int16_t dy;
    POINT  *pptEnd;
    int16_t yStart;
    int16_t i;
    int16_t r;
    int16_t cPl;
    int16_t dx;
    int16_t xEnd;

L_02a0:
    sqrt((double)(r2));
    r = LOWORD(__ftol());
    xStart = (pt.x - r);
    xEnd = (pt.x + r);
    yStart = (pt.y - r);
    yEnd = (pt.y + r);
    cPl = 0x0;
    pptEnd = rgptPlan[game.cPlanMax];
    dx = (rgptPlan[(game.cPlanMax - 0x1)].x - rgptPlan[0x0].x);
    i = LOWORD((int32_t)(((uint32_t)(((uint32_t)((pt.x - rgptPlan[0x0].x)) * (uint32_t)(game.cPlanMax))) / (uint32_t)(dx))));
    if ((i < game.cPlanMax))
        goto L_035d;
    else
        goto L_0354;

L_0354:
    i = (game.cPlanMax + 0xffff);

L_035d:
    if ((i >= 0x0))
        goto L_036b;
    else
        goto L_0366;

L_0366:
    i = 0x0;

L_036b:
    ppt = rgptPlan[i];

L_037a:
    if ((ppt->x < xStart))
        goto L_0398;
    else
        goto L_0387;

L_0387:
    if ((ppt <= 0x2f40))
        goto L_0398;
    else
        goto L_0391;

L_0391:
    ppt = (ppt - 0x4);
    goto L_037a;

L_0398:
    if ((ppt->x > xEnd))
        goto L_0442;
    else
        goto L_03a5;

L_03a5:
    if ((ppt >= pptEnd))
        goto L_0442;
    else
        goto L_03b0;

L_03b0:
    if ((ppt->x < xStart))
        goto L_043b;
    else
        goto L_03bd;

L_03bd:
    if ((ppt->y < yStart))
        goto L_043b;
    else
        goto L_03cb;

L_03cb:
    if ((ppt->y > yEnd))
        goto L_043b;
    else
        goto L_03d9;

L_03d9:
    dx = (ppt->x - pt.x);
    dy = (ppt->y - pt.y);
    if (((HIWORD((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)))) + HIWORD((uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))))) > HIWORD(r2)))
        goto L_043b;
    else
        goto L_042a;

L_042a:
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x1a]) < HIWORD(r2) ? L_0437 : L_042f */

L_042f:
    /* untranslated: branch (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x1c]) > LOWORD(r2) ? L_043b : L_0437 */

L_0437:
    cPl = (cPl + 0x1);

L_043b:
    ppt = (ppt + 0x4);
    goto L_0398;

L_0442:

L_0448:
    return cPl;
}

void DrawThingGauge(HDC hdc, RECT *prc, THING *lpth, int16_t md) {
    int16_t  iMode;
    int16_t  cSections;
    int16_t  fDisabled;
    uint16_t rghbr[5];
    int16_t  c;
    int16_t  i;
    int32_t  rgSize[5];
    int32_t  lMax;
    int32_t  l;

L_044e:
    fDisabled = 0x0;
    SelectObject(hdc, rghfontArial8[0x1]);
    cSections = 0x1;
    /* untranslated: lMax = (uint32_t)(words(0x0, lpth->thp.wtMax) * 0xa) */
    if ((md < 0x0))
        goto L_04fa;
    else
        goto L_0498;

L_0498:
    if ((md > 0x4))
        goto L_04fa;
    else
        goto L_04a1;

L_04a1:
    if ((md == 0x4))
        goto L_04b3;
    else
        goto L_04aa;

L_04aa:
    if ((md != 0x3))
        goto L_04cd;
    else
        goto L_04b3;

L_04b3:
    rghbr[0x0] = hbrButtonShadow;
    rgSize[0x0] = lMax;
    fDisabled = 0x1;
    goto L_055b;

L_04cd:
    rghbr[0x0] = rghbrMineral[md];
    /* untranslated: LOWORD(rgSize[0x0]) = HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (md * 0x2))] */
    /* untranslated: HIWORD(rgSize[0x0]) = signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (md * 0x2))]) */

L_04fa:
    if ((md != 0x5))
        goto L_055b;
    else
        goto L_0503;

L_0503:
    i = 0x0;
    goto L_054d;

L_050b:
    rghbr[i] = rghbrMineral[i];
    /* untranslated: LOWORD(rgSize[i]) = HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] */
    /* untranslated: HIWORD(rgSize[i]) = signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) */
    i = (i + 0x1);

L_054d:
    if ((i < 0x3))
        goto L_050b;
    else
        goto L_0556;

L_0556:
    cSections = 0x3;

L_055b:
    l = LDrawGauge(hdc, prc, cSections, rgSize, rghbr, lMax);
    iMode = SetBkMode(hdc, TRANSPARENT);
    if ((fDisabled == 0x0))
        goto L_05a2;
    else
        goto L_0598;

L_0598:
    l = 0x0;

L_05a2:
    if ((cSections != 0x1))
        goto L_05cd;
    else
        goto L_05ab;

L_05ab:
    c = _wsprintf(szWork, "%ldkT", LOWORD(l), HIWORD(l));
    goto L_05f2;

L_05cd:
    c = _wsprintf(szWork, "%ld of %ldkT", LOWORD(l), HIWORD(l), LOWORD(lMax), HIWORD(lMax));

L_05f2:
    l = GetTextExtent(hdc, szWork, c);
    if ((LOWORD(l) >= ((prc->right - prc->left) + 0xfffd)))
        goto L_063a;
    else
        goto L_0625;

L_0625:
    RcCtrTextOut(hdc, prc, szWork, c);

L_063a:
    SetBkMode(hdc, iMode);
    return;
}

int16_t IValidateWormholePos(THING *lpthWorm) {
    int16_t iRet;
    POINT   pt;
    int32_t dy;
    THING  *lpthMac;
    FLEET  *lpfl;
    int16_t ifl;
    int16_t i;
    THING  *lpth;
    int16_t dUni;
    int32_t dx;
    int32_t l;

L_064c:
    iRet = 0x0;
    dUni = (LOWORD((0x190 * game.mdSize)) + 0x578);
    pt.x = lpthWorm->pt.x;
    pt.y = lpthWorm->pt.y;
    if ((pt.x < 0x3e8))
        goto L_068c;
    else
        goto L_0682;

L_0682:
    if ((pt.y >= 0x3e8))
        goto L_0692;
    else
        goto L_068c;

L_068c:
    return 0xf;

L_0692:
    if ((pt.x > dUni))
        goto L_06a8;
    else
        goto L_069d;

L_069d:
    if ((pt.y <= dUni))
        goto L_06ae;
    else
        goto L_06a8;

L_06a8:
    return 0xf;

L_06ae:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_071c;

L_06d9:
    /* untranslated: branch ss:[pt] != lpth->pt.x ? L_0718 : L_06f3 */

L_06f3:
    /* untranslated: branch ss:[pt+0x2] != lpth->pt.y ? L_0718 : L_06fc */

L_06fc:
    if ((LOWORD(lpth) != LOWORD(lpthWorm)))
        goto L_0712;
    else
        goto L_070a;

L_070a:
    if ((HIWORD(lpth) == HIWORD(lpthWorm)))
        goto L_0718;
    else
        goto L_0712;

L_0712:
    return 0xf;

L_0718:
    lpth = (lpth + 0x1);

L_071c:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_06d9;
    else
        goto L_072a;

L_072a:
    i = 0x0;
    goto L_0736;

L_0732:
    i = (i + 0x1);

L_0736:
    if ((i >= game.cPlanMax))
        goto L_0769;
    else
        goto L_0741;

L_0741:
    if ((pt.x != rgptPlan[i].x))
        goto L_0732;
    else
        goto L_0758;

L_0758:
    if ((pt.y != rgptPlan[i].y))
        goto L_0732;
    else
        goto L_0760;

L_0760:
    return 0xf;

L_0769:
    ifl = 0x0;
    goto L_0775;

L_0771:
    ifl = (ifl + 0x1);

L_0775:
    if ((ifl >= cFleet))
        goto L_07dc;
    else
        goto L_0780;

L_0780:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_07b0;
    else
        goto L_07a8;

L_07a8:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_07dc;
    else
        goto L_07b0;

L_07b0:
    /* untranslated: branch ss:[pt] != lpfl->pt.x ? L_0771 : L_07ca */

L_07ca:
    /* untranslated: branch ss:[pt+0x2] != lpfl->pt.y ? L_0771 : L_07d3 */

L_07d3:
    return 0xf;

L_07dc:
    if ((pt.x < 0x3f2))
        goto L_080c;
    else
        goto L_07e6;

L_07e6:
    if ((pt.y < 0x3f2))
        goto L_080c;
    else
        goto L_07f0;

L_07f0:
    if ((pt.x > (dUni + 0xfff6)))
        goto L_080c;
    else
        goto L_07fe;

L_07fe:
    if ((pt.y <= (dUni + 0xfff6)))
        goto L_0810;
    else
        goto L_080c;

L_080c:
    iRet = (iRet | 0x4);

L_0810:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_09cb;

L_083b:
    if ((lpth->ith != ithWormhole))
        goto L_09c7;
    else
        goto L_0851;

L_0851:
    if ((LOWORD(lpth) != LOWORD(lpthWorm)))
        goto L_086a;
    else
        goto L_085f;

L_085f:
    if ((HIWORD(lpth) == HIWORD(lpthWorm)))
        goto L_09c7;
    else
        goto L_0864;

L_0864:

L_086a:
    dx = (uint32_t)((pt.x - lpth->pt.x));
    dy = (uint32_t)((pt.y - lpth->pt.y));
    l = ((uint32_t)((dx * dx)) + (uint32_t)((dy * dy)));
    if ((lpth->idFull != *(lpthWorm + 0xc)))
        goto L_0950;
    else
        goto L_08d6;

L_08d6:
    if ((HIWORD(l) > 0x0))
        goto L_09c7;
    else
        goto L_08df;

L_08df:
    if ((HIWORD(l) < 0x0))
        goto L_08ee;
    else
        goto L_08e4;

L_08e4:
    if ((LOWORD(l) >= 0x1324))
        goto L_09c7;
    else
        goto L_08ee;

L_08ee:
    if ((HIWORD(l) > 0x0))
        goto L_090c;
    else
        goto L_08f7;

L_08f7:
    if ((HIWORD(l) < 0x0))
        goto L_0905;
    else
        goto L_08fc;

L_08fc:
    if ((LOWORD(l) >= 0x19))
        goto L_090c;
    else
        goto L_0905;

L_0905:
    iRet = (iRet | 0x8);
    goto L_09c7;

L_090c:
    if ((HIWORD(l) > 0x0))
        goto L_092a;
    else
        goto L_0915;

L_0915:
    if ((HIWORD(l) < 0x0))
        goto L_0923;
    else
        goto L_091a;

L_091a:
    if ((LOWORD(l) >= 0x64))
        goto L_092a;
    else
        goto L_0923;

L_0923:
    iRet = (iRet | 0x4);
    goto L_09c7;

L_092a:
    if ((HIWORD(l) > 0x0))
        goto L_0949;
    else
        goto L_0933;

L_0933:
    if ((HIWORD(l) < 0x0))
        goto L_0942;
    else
        goto L_0938;

L_0938:
    if ((LOWORD(l) >= 0x384))
        goto L_0949;
    else
        goto L_0942;

L_0942:
    iRet = (iRet | 0x2);
    goto L_09c7;

L_0949:
    iRet = (iRet | 0x1);

L_0950:
    if ((HIWORD(l) > 0x0))
        goto L_09c7;
    else
        goto L_0959;

L_0959:
    if ((HIWORD(l) < 0x0))
        goto L_0968;
    else
        goto L_095e;

L_095e:
    if ((LOWORD(l) >= 0x384))
        goto L_09c7;
    else
        goto L_0968;

L_0968:
    if ((HIWORD(l) > 0x0))
        goto L_0986;
    else
        goto L_0971;

L_0971:
    if ((HIWORD(l) < 0x0))
        goto L_097f;
    else
        goto L_0976;

L_0976:
    if ((LOWORD(l) >= 0x10))
        goto L_0986;
    else
        goto L_097f;

L_097f:
    iRet = (iRet | 0x8);
    goto L_09c7;

L_0986:
    if ((HIWORD(l) > 0x0))
        goto L_09a4;
    else
        goto L_098f;

L_098f:
    if ((HIWORD(l) < 0x0))
        goto L_099d;
    else
        goto L_0994;

L_0994:
    if ((LOWORD(l) >= 0x40))
        goto L_09a4;
    else
        goto L_099d;

L_099d:
    iRet = (iRet | 0x4);
    goto L_09c7;

L_09a4:
    if ((HIWORD(l) > 0x0))
        goto L_09c3;
    else
        goto L_09ad;

L_09ad:
    if ((HIWORD(l) < 0x0))
        goto L_09bc;
    else
        goto L_09b2;

L_09b2:
    if ((LOWORD(l) >= 0xe1))
        goto L_09c3;
    else
        goto L_09bc;

L_09bc:
    iRet = (iRet | 0x2);
    goto L_09c7;

L_09c3:
    iRet = (iRet | 0x1);

L_09c7:
    lpth = (lpth + 0x1);

L_09cb:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_083b;
    else
        goto L_09d9;

L_09d9:
    i = 0x0;
    goto L_09e5;

L_09e1:
    i = (i + 0x1);

L_09e5:
    if ((i >= game.cPlanMax))
        goto L_0ad0;
    else
        goto L_09f0;

L_09f0:
    dx = (uint32_t)((pt.x - rgptPlan[i].x));
    dy = (uint32_t)((pt.y - rgptPlan[i].y));
    l = ((uint32_t)((dx * dx)) + (uint32_t)((dy * dy)));
    if ((HIWORD(l) > 0x0))
        goto L_09e1;
    else
        goto L_0a5f;

L_0a5f:
    if ((HIWORD(l) < 0x0))
        goto L_0a6e;
    else
        goto L_0a64;

L_0a64:
    if ((LOWORD(l) >= 0x310))
        goto L_09e1;
    else
        goto L_0a6e;

L_0a6e:
    if ((HIWORD(l) > 0x0))
        goto L_0a8c;
    else
        goto L_0a77;

L_0a77:
    if ((HIWORD(l) < 0x0))
        goto L_0a85;
    else
        goto L_0a7c;

L_0a7c:
    if ((LOWORD(l) >= 0x19))
        goto L_0a8c;
    else
        goto L_0a85;

L_0a85:
    iRet = (iRet | 0x8);
    goto L_09e1;

L_0a8c:
    if ((HIWORD(l) > 0x0))
        goto L_0aaa;
    else
        goto L_0a95;

L_0a95:
    if ((HIWORD(l) < 0x0))
        goto L_0aa3;
    else
        goto L_0a9a;

L_0a9a:
    if ((LOWORD(l) >= 0x64))
        goto L_0aaa;
    else
        goto L_0aa3;

L_0aa3:
    iRet = (iRet | 0x4);
    goto L_09e1;

L_0aaa:
    if ((HIWORD(l) > 0x0))
        goto L_0ac9;
    else
        goto L_0ab3;

L_0ab3:
    if ((HIWORD(l) < 0x0))
        goto L_0ac2;
    else
        goto L_0ab8;

L_0ab8:
    if ((LOWORD(l) >= 0x190))
        goto L_0ac9;
    else
        goto L_0ac2;

L_0ac2:
    iRet = (iRet | 0x2);
    goto L_09e1;

L_0ac9:
    iRet = (iRet | 0x1);

L_0ad0:
    return iRet;
}

int16_t PctWormholeMoves(THING *lpth) {
    int16_t pct;

L_0adc:
    pct = ((uint32_t)(((*(lpth + 0x6) >> 0x2) & 0x3ff)) / 0x5);
    pct = (pct - (0x2 - (*(lpth + 0x6) & 0x3)));
    if ((pct >= 0x0))
        goto L_0b1f;
    else
        goto L_0b17;

L_0b17:
    pct = 0x0;
    goto L_0b2d;

L_0b1f:
    if ((pct <= 0x6))
        goto L_0b2d;
    else
        goto L_0b28;

L_0b28:
    pct = 0x6;

L_0b2d:

L_0b33:
    return pct;
}

void DoThingInteractions(int16_t fPostMove) {
    int32_t   wtThreshhold;
    uint16_t  grbitPlrTrader;
    int16_t   iplr;
    int32_t   wtMin;
    POINT     pt;
    int16_t   iplrSav;
    uint8_t   rgTech[6];
    int32_t   wtNext;
    int32_t   dy;
    THING    *lpthMac;
    PLANET   *lpplMac;
    PLANET   *lppl;
    int16_t   i;
    int16_t   ifl;
    FLEET    *lpfl;
    THING    *lpth;
    MessageId idm;
    int16_t   cPlrTrueMaxTech;
    int32_t   dx;
    int16_t   fMaxTech;
    int32_t   l;
    int32_t   cTech;
    int16_t   iLowest;
    int16_t   cTechCur;
    int32_t   lSpent;
    int16_t   iGoto;
    uint16_t  grbitTrader;
    int16_t   cTry;
    int16_t   iOffset;
    int16_t   ish;
    SHDEF     shdef;
    SHDEF    *lpshdefDest;
    FLEET    *lpflNew;
    int16_t   cGive;
    int16_t   iLvl;
    int16_t   iPass;
    uint16_t  t_merge_0cf3_0001;
    uint16_t  t_merge_0d44_0001;
    uint16_t  t_0fac;
    int16_t   t_11bc;
    uint16_t  t_merge_123c_0001;
    uint16_t  t_merge_17f8_0001;
    int16_t   t_1851;
    int32_t   t_merge_1922_0001_wide;
    uint16_t  t_merge_19be_0001;

L_0b3a:
    if ((fPostMove == 0x0))
        goto L_1a8f;
    else
        goto L_0b49;

L_0b49:

L_0b4f:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_1a81;

L_0b7a:
    if ((lpth->ith != ithMysteryTrader))
        goto L_1a7d;
    else
        goto L_0b8d;

L_0b8d:

L_0b93:
    pt.x = lpth->pt.x;
    pt.y = lpth->pt.y;
    ifl = 0x0;
    goto L_0bb0;

L_0bac:
    ifl = (ifl + 0x1);

L_0bb0:
    if ((ifl >= cFleet))
        goto L_1631;
    else
        goto L_0bbb;

L_0bbb:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_0beb;
    else
        goto L_0be3;

L_0be3:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_1631;
    else
        goto L_0beb;

L_0beb:
    if ((lpfl->fDead != 0x0))
        goto L_0bac;
    else
        goto L_0c02;

L_0c02:
    /* untranslated: branch lpfl->pt.x != ss:[pt] ? L_0bac : L_0c1c */

L_0c1c:
    /* untranslated: branch lpfl->pt.y != ss:[pt+0x2] ? L_0bac : L_0c22 */

L_0c22:

L_0c28:
    wtMin = 0x0;
    i = 0x0;
    goto L_0c61;

L_0c3a:
    wtMin = (wtMin + lpfl->rgwtMin[i]);
    i = (i + 0x1);

L_0c61:
    if ((i <= 0x2))
        goto L_0c3a;
    else
        goto L_0c6a;

L_0c6a:
    if ((HIWORD(wtMin) > 0x0))
        goto L_0ccb;
    else
        goto L_0c73;

L_0c73:
    if ((HIWORD(wtMin) < 0x0))
        goto L_0c82;
    else
        goto L_0c78;

L_0c78:
    if ((LOWORD(wtMin) >= 0x1388))
        goto L_0ccb;
    else
        goto L_0c82;

L_0c82:
    if ((lpfl->fHereAllTurn != 0x0))
        goto L_0bac;
    else
        goto L_0c99;

L_0c99:
    FSendPlrMsg2(lpfl->iplr, 0x108, (lpfl->id | 0x8000), lpfl->id, 0x0);

L_0ccb:
    if ((((rgplr[lpfl->iPlayer].wFlags >> 0x1) & 0x1) == 0x0))
        goto L_0cf0;
    else
        goto L_0cea;

L_0cea:
    t_merge_0cf3_0001 = 0xa;
    goto L_0cf3;

L_0cf0:
    t_merge_0cf3_0001 = 0x1a;

L_0cf3:
    cPlrTrueMaxTech = t_merge_0cf3_0001;
    i = 0x0;
    goto L_0d29;

L_0cfe:
    if (((uint16_t)(rgplr[lpfl->iPlayer].rgTech[i]) < cPlrTrueMaxTech))
        goto L_0d32;
    else
        goto L_0d1f;

L_0d1f:

L_0d25:
    i = (i + 0x1);

L_0d29:
    if ((i < 0x6))
        goto L_0cfe;
    else
        goto L_0d32;

L_0d32:
    if ((i != 0x6))
        goto L_0d41;
    else
        goto L_0d3b;

L_0d3b:
    t_merge_0d44_0001 = 0x1;
    goto L_0d44;

L_0d41:
    t_merge_0d44_0001 = 0x0;

L_0d44:
    fMaxTech = t_merge_0d44_0001;
    grbitPlrTrader = rgplr[lpfl->iPlayer].grbitTrader;
    iplr = lpfl->iPlayer;
    if ((((0x1 << iplr) & lpth->tht.grbitPlr) == 0x0))
        goto L_0daf;
    else
        goto L_0d7d;

L_0d7d:
    FSendPlrMsg2(lpfl->iplr, 0x118, (lpfl->id | 0x8000), lpfl->id, 0x0);
    goto L_0bac;

L_0daf:
    lpth->tht.grbitPlr = (lpth->tht.grbitPlr | (0x1 << iplr));
    FRemovePlayerMessage(iplr, 0x4e, (lpfl->id | 0x8000));
    lpfl->fDead = 0x1;
    if ((lpth->tht.grbitTrader == 0x0))
        goto L_0e0a;
    else
        goto L_0df8;

L_0df8:
    if (((lpth->tht.grbitTrader & grbitPlrTrader) == 0x0))
        goto LGivePart;
    else
        goto L_0e0a;

L_0e0a:
    if ((fMaxTech == 0x0))
        goto L_0e63;
    else
        goto L_0e13;

L_0e13:
    if ((Random(0x5) != 0x0))
        goto LGivePart;
    else
        goto L_0e24;

L_0e24:

L_0e2a:
    idm = idmHasAbsorbedMysteryTraderHoweverTraderUnable2;
    FSendPlrMsg2(lpfl->iplr, idm, 0xffff, WFromLpfl(lpfl), 0x0);
    goto L_0bac;

L_0e63:
    cTechCur = 0x0;
    /* untranslated: cTech = ((int32_t)(words((HIWORD(wtMin) + 0xffff), (LOWORD(wtMin) + 0xec78)) / 0x4b0) + 0x6) */
    i = 0x0;
    goto L_0eba;

L_0e97:
    cTechCur = (cTechCur + (uint16_t)(rgplr[lpfl->iPlayer].rgTech[i]));
    i = (i + 0x1);

L_0eba:
    if ((i < 0x6))
        goto L_0e97;
    else
        goto L_0ec3;

L_0ec3:
    if ((HIWORD(cTech) < 0x0))
        goto L_0ee4;
    else
        goto L_0ecc;

L_0ecc:
    if ((HIWORD(cTech) > 0x0))
        goto L_0eda;
    else
        goto L_0ed1;

L_0ed1:
    if ((LOWORD(cTech) <= 0xa))
        goto L_0ee4;
    else
        goto L_0eda;

L_0eda:
    cTech = 0xa;

L_0ee4:
    if ((cTechCur < 0x6c))
        goto L_0efa;
    else
        goto L_0eed;

L_0eed:
    cTech = 0x1;
    goto L_0f49;

L_0efa:
    if ((cTechCur < 0x60))
        goto L_0f10;
    else
        goto L_0f03;

L_0f03:
    cTech = 0x2;
    goto L_0f49;

L_0f10:
    if ((cTechCur < 0x54))
        goto L_0f24;
    else
        goto L_0f19;

L_0f19:
    cTech = (cTech - 0x3);
    goto L_0f49;

L_0f24:
    if ((cTechCur < 0x48))
        goto L_0f38;
    else
        goto L_0f2d;

L_0f2d:
    cTech = (cTech - 0x2);
    goto L_0f49;

L_0f38:
    if ((cTechCur < 0x3c))
        goto L_0f49;
    else
        goto L_0f41;

L_0f41:
    cTech = (cTech - 0x1);

L_0f49:
    if (((grbitPlrTrader & 0x1fff) == 0x1fff))
        goto L_0f5f;
    else
        goto L_0f57;

L_0f57:
    idm = idmHasAbsorbedMysteryTraderTraderHasGiven;
    goto L_0f64;

L_0f5f:
    idm = idmHasAbsorbedMysteryTraderReturnTraderHas;

L_0f64:
    FSendPlrMsg(lpfl->iplr, idm, 0xffff, WFromLpfl(lpfl), LOWORD(cTech), 0x0, 0x0, 0x0, 0x0, 0x0);

L_0fac:
    t_0fac = LOWORD(cTech);
    cTech = (cTech - 0x1);
    if ((t_0fac != 0x0))
        goto L_0fca;
    else
        goto L_0fc2;

L_0fc2:
    if ((HIWORD(cTech) == 0x0))
        goto L_0bac;
    else
        goto L_0fca;

L_0fca:
    if ((Random(0x4) >= 0x3))
        goto L_1010;
    else
        goto L_0fde;

L_0fde:
    iLowest = Random(0x6);
    if (((uint16_t)(rgplr[iplr].rgTech[iLowest]) < cPlrTrueMaxTech))
        goto LGiveITech;
    else
        goto L_100a;

L_100a:

L_1010:
    iLowest = 0x0;
    i = 0x1;
    goto L_1064;

L_101d:
    if (((uint16_t)(rgplr[iplr].rgTech[i]) >= (uint16_t)(rgplr[iplr].rgTech[iLowest])))
        goto L_1060;
    else
        goto L_105a;

L_105a:
    iLowest = i;

L_1060:
    i = (i + 0x1);

L_1064:
    if ((i < 0x6))
        goto L_101d;
    else
        goto L_106d;

L_106d:
    if (((uint16_t)(rgplr[iplr].rgTech[iLowest]) >= cPlrTrueMaxTech))
        goto L_0bac;
    else
        goto L_108a;

L_108a:

LGiveITech:
    memcpy(rgTech, rgplr[iplr].rgTech, 0x6);
    rgTech[iLowest] = (rgTech[iLowest] + 0x1);
    iplrSav = idPlayer;
    idPlayer = iplr;
    wtNext = CostOfDevelopingItem(&(rgTech));
    idPlayer = iplrSav;
    lSpent = rgplr[iplr].rgResSpent[iLowest];
    lSpent = (int32_t)((lSpent * 0x2));
    if ((HIWORD(wtNext) < 0x0))
        goto L_113e;
    else
        goto L_1124;

L_1124:
    if ((HIWORD(wtNext) > 0x0))
        goto L_1132;
    else
        goto L_1129;

L_1129:
    if ((LOWORD(wtNext) <= 0x0))
        goto L_113e;
    else
        goto L_1132;

L_1132:
    lSpent = (lSpent + wtNext);

L_113e:
    rgplr[iplr].rgResSpent[iLowest] = lSpent;
    UpdateResearchStatus(0x0);
    goto L_0fac;

LGivePart:
    cTry = 0x19;
    grbitTrader = lpth->tht.grbitTrader;
    if ((grbitTrader != 0x0))
        goto L_11ae;
    else
        goto L_1198;

L_1198:
    grbitTrader = (0x1 << Random(0xd));

L_11ae:
    if (((grbitTrader & grbitPlrTrader) == 0x0))
        goto L_11e4;
    else
        goto L_11bc;

L_11bc:
    t_11bc = cTry;
    cTry = (cTry - 0x1);
    if ((t_11bc <= 0x0))
        goto L_11e4;
    else
        goto L_11cb;

L_11cb:
    grbitTrader = (0x1 << Random(0xd));
    goto L_11ae;

L_11e4:
    if ((cTry > 0x0))
        goto L_11f2;
    else
        goto L_11ed;

L_11ed:
    grbitTrader = 0x1000;

L_11f2:
    lpth->tht.grbitPlr = (lpth->tht.grbitPlr | (0x1 << iplr));
    if ((grbitTrader != 0x1000))
        goto L_15e9;
    else
        goto L_120b;

L_120b:
    if ((((rgplr[iplr].wMdPlr >> 0x9) & 0x1) != 0x0))
        goto L_0bac;
    else
        goto L_1229;

L_1229:
    if ((game.turn <= 0x64))
        goto L_1239;
    else
        goto L_1233;

L_1233:
    t_merge_123c_0001 = 0x1;
    goto L_123c;

L_1239:
    t_merge_123c_0001 = 0x0;

L_123c:
    iOffset = Random((0x4 - t_merge_123c_0001));
    if ((iOffset < 0x1))
        goto L_1268;
    else
        goto L_1256;

L_1256:
    iOffset = (Random(0x2) + 0x1);

L_1268:
    shdef = LpshdefT()[(iOffset + 0x13)];
    ish = IshFindSimilarDesign(&(shdef.hul), iplr);
    if ((ish >= 0x0))
        goto L_12f5;
    else
        goto L_12b3;

L_12b3:
    ish = (ish + 0x1);
    if ((ish >= 0x10))
        goto L_12f5;
    else
        goto L_12c2;

L_12c2:
    if ((((rglpshdef[iplr][ish].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_12f5;
    else
        goto L_12ec;

L_12ec:

L_12f5:
    if ((ish >= 0x10))
        goto LNoLifeboat;
    else
        goto L_12fe;

L_12fe:
    /* untranslated: branch (part[4:2](rgplr[lpfl->iplr]) & 0xfff) < 0x200 ? L_1359 : L_1324 */

LNoLifeboat:
    FSendPlrMsg2(lpfl->iplr, 0x150, 0xffff, WFromLpfl(lpfl), 0x0);
    goto L_0bac;

L_1359:
    if ((Random(0x3) != 0x0))
        goto L_1376;
    else
        goto L_136d;

L_136d:
    cGive = 0x2;
    goto L_137c;

L_1376:
    cGive = 0x1;

L_137c:
    if ((game.turn <= 0x64))
        goto L_13b2;
    else
        goto L_1386;

L_1386:
    if ((game.fSinglePlr != 0x0))
        goto L_13b2;
    else
        goto L_1398;

L_1398:
    /* untranslated: cGive = (cGive + Random(((words(game.turn, 0x0) / 0x64) + 0x1))) */

L_13b2:
    if ((cGive <= 0x5))
        goto L_13c2;
    else
        goto L_13bc;

L_13bc:
    cGive = 0x5;

L_13c2:
    if ((iOffset <= 0x0))
        goto L_13df;
    else
        goto L_13cb;

L_13cb:
    cGive = (cGive + Random((cGive + 0x1)));

L_13df:
    lpflNew = LpflNew(iplr, lpfl->idPlanet);
    if ((LOWORD(lpflNew) != 0x0))
        goto L_1410;
    else
        goto L_1403;

L_1403:
    if ((HIWORD(lpflNew) == 0x0))
        goto LNoLifeboat;
    else
        goto L_140a;

L_140a:

L_1410:
    FSendPlrMsg2(lpfl->iplr, 0x14f, (lpflNew->id | 0x8000), WFromLpfl(lpfl), cGive);
    if ((lpflNew->id >= lpfl->id))
        goto L_145f;
    else
        goto L_145b;

L_145b:
    ifl = (ifl + 0x1);

L_145f:
    lpflNew->pt.x = lpfl->pt.x;
    lpflNew->pt.y = lpfl->pt.y;
    lpflNew->lpplord->rgord[0x0].pt.x = lpfl->pt.x;
    lpflNew->lpplord->rgord[0x0].pt.y = lpfl->pt.y;
    lpflNew->fHereAllTurn = 0x1;
    lpshdefDest = &(rglpshdef[iplr][ish]);
    if ((lpshdefDest->fFree == 0x0))
        goto L_1584;
    else
        goto L_14de;

L_14de:
    *(lpshdefDest) = shdef;
    lpshdefDest->wFlags = ((lpshdefDest->wFlags & 0x83ff) | ((ish & 0x1f) << 0xa));
    lpshdefDest->fGift = 0x1;
    lpshdefDest->cBuilt = 0x0;
    lpshdefDest->cExist = 0x0;
    rgplr[iplr].cShDef = (rgplr[iplr].cShDef + 0x1);
    idPlayer = lpfl->iplr;
    UpdateShdefCost(lpshdefDest);
    idPlayer = 0xffff;

L_1584:
    lpshdefDest->cBuilt = (lpshdefDest->cBuilt + (uint32_t)(cGive));
    lpshdefDest->cExist = (lpshdefDest->cExist + (uint32_t)(cGive));
    lpflNew->rgcsh[ish] = cGive;
    lpflNew->rgwtMin[0x4] = LGetFleetStat(lpflNew, 0x1);

L_15e9:
    idm = IdmGiveTraderPart(grbitTrader, iplr, iGoto);
    FSendPlrMsg2(lpfl->iplr, idm, iGoto, WFromLpfl(lpfl), 0x0);

L_1631:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_1a6f;

L_165c:
    if ((lppl->iPlayer == 0xffff))
        goto L_1a6b;
    else
        goto L_1669;

L_1669:
    if ((lppl->fStarbase == 0x0))
        goto L_1a6b;
    else
        goto L_1680;

L_1680:
    if ((((rgplr[lppl->iPlayer].wMdPlr >> 0x9) & 0x1) == 0x0))
        goto L_1a6b;
    else
        goto L_16a2;

L_16a2:
    if ((((rgplr[lppl->iPlayer].wMdPlr >> 0xa) & 0x7) < 0x2))
        goto L_1a6b;
    else
        goto L_16c4;

L_16c4:
    if ((((0x1 << lppl->iPlayer) & lpth->tht.grbitPlr) != 0x0))
        goto L_1a6b;
    else
        goto L_16dc;

L_16dc:

L_16e2:
    dx = (uint32_t)((rgptPlan[lppl->id].x - pt.x));
    if ((HIWORD(dx) < 0x0))
        goto L_1714;
    else
        goto L_1703;

L_1703:
    if ((HIWORD(dx) > 0x0))
        goto L_1a7d;
    else
        goto L_1708;

L_1708:
    if ((LOWORD(dx) > 0x64))
        goto L_1a7d;
    else
        goto L_170e;

L_170e:

L_1714:
    dy = (uint32_t)((rgptPlan[lppl->id].y - pt.y));
    l = ((uint32_t)((dx * dx)) + (uint32_t)((dy * dy)));
    if ((HIWORD(l) < 0x0))
        goto L_1783;
    else
        goto L_1771;

L_1771:
    if ((HIWORD(l) > 0x0))
        goto L_1a6b;
    else
        goto L_1776;

L_1776:
    if ((LOWORD(l) > 0x2710))
        goto L_1a6b;
    else
        goto L_177d;

L_177d:

L_1783:
    wtNext = 0x0;
    i = 0x0;
    goto L_17bc;

L_1795:
    wtNext = (wtNext + lppl->rgwtMin[i]);
    i = (i + 0x1);

L_17bc:
    if ((i < 0x3))
        goto L_1795;
    else
        goto L_17c5;

L_17c5:
    iplr = lppl->iPlayer;
    if ((((rgplr[iplr].wMdPlr >> 0xa) & 0x7) != 0x2))
        goto L_17f4;
    else
        goto L_17ed;

L_17ed:
    t_merge_17f8_0001 = 0xdac;
    goto L_17f8;

L_17f4:
    t_merge_17f8_0001 = 0x1388;

L_17f8:
    LOWORD(wtThreshhold) = t_merge_17f8_0001;
    HIWORD(wtThreshhold) = 0x0;
    if ((HIWORD(wtNext) > HIWORD(wtThreshhold)))
        goto L_181c;
    else
        goto L_180c;

L_180c:
    if ((HIWORD(wtNext) < HIWORD(wtThreshhold)))
        goto L_1a6b;
    else
        goto L_1811;

L_1811:
    if ((LOWORD(wtNext) < LOWORD(wtThreshhold)))
        goto L_1a6b;
    else
        goto L_1816;

L_1816:

L_181c:
    if ((lpth->tht.grbitTrader == 0x0))
        goto LAutoTech;
    else
        goto L_1829;

L_1829:
    cTry = 0x32;
    grbitTrader = lpth->tht.grbitTrader;

L_1838:
    if (((grbitTrader & rgplr[iplr].grbitTrader) == 0x0))
        goto L_1879;
    else
        goto L_1851;

L_1851:
    t_1851 = cTry;
    cTry = (cTry - 0x1);
    if ((t_1851 <= 0x0))
        goto L_1879;
    else
        goto L_1860;

L_1860:
    grbitTrader = (0x1 << Random(0xd));
    goto L_1838;

L_1879:
    if ((cTry <= 0x0))
        goto LAutoTech;
    else
        goto L_187f;

L_187f:

L_1885:
    rgplr[iplr].grbitTrader = (rgplr[iplr].grbitTrader | grbitTrader);

LChgMin:
    lpth->tht.grbitPlr = (lpth->tht.grbitPlr | (0x1 << iplr));
    i = 0x2;

L_18aa:
    if ((HIWORD(wtNext) < 0x0))
        goto L_1a6b;
    else
        goto L_18b3;

L_18b3:
    if ((HIWORD(wtNext) > 0x0))
        goto L_18c1;
    else
        goto L_18b8;

L_18b8:
    if ((LOWORD(wtNext) <= 0x0))
        goto L_1a6b;
    else
        goto L_18c1;

L_18c1:
    if ((i < 0x0))
        goto L_1a6b;
    else
        goto L_18ca;

L_18ca:
    if ((HIWORD(wtNext) > HIWORD(lppl->rgwtMin[i])))
        goto L_1905;
    else
        goto L_18ef;

L_18ef:
    if ((HIWORD(wtNext) < HIWORD(lppl->rgwtMin[i])))
        goto L_18fc;
    else
        goto L_18f4;

L_18f4:
    if ((LOWORD(wtNext) >= LOWORD(lppl->rgwtMin[i])))
        goto L_1905;
    else
        goto L_18fc;

L_18fc:
    t_merge_1922_0001_wide = wtNext;
    goto L_1922;

L_1905:
    t_merge_1922_0001_wide = lppl->rgwtMin[i];

L_1922:
    wtMin = t_merge_1922_0001_wide;
    lppl->rgwtMin[i] = (lppl->rgwtMin[i] - wtMin);
    wtNext = (wtNext - wtMin);
    i = (i - 0x1);
    goto L_18aa;

LAutoTech:
    iLvl = 0x0;
    i = 0x0;
    goto L_198d;

L_196e:
    iLvl = (iLvl + (uint16_t)(rgplr[iplr].rgTech[i]));
    i = (i + 0x1);

L_198d:
    if ((i < 0x6))
        goto L_196e;
    else
        goto L_1996;

L_1996:
    if ((((rgplr[lpfl->iPlayer].wFlags >> 0x1) & 0x1) == 0x0))
        goto L_19bb;
    else
        goto L_19b5;

L_19b5:
    t_merge_19be_0001 = 0xa;
    goto L_19be;

L_19bb:
    t_merge_19be_0001 = 0x1a;

L_19be:
    cPlrTrueMaxTech = t_merge_19be_0001;
    if ((iLvl >= (LOWORD((0x6 * cPlrTrueMaxTech)) + 0xfffa)))
        goto L_1a6b;
    else
        goto L_19d2;

L_19d2:
    iPass = 0x0;
    goto L_1a53;

L_19da:
    iLvl = 0x0;
    i = 0x1;
    goto L_1a2e;

L_19e7:
    if (((uint16_t)(rgplr[iplr].rgTech[i]) >= (uint16_t)(rgplr[iplr].rgTech[iLvl])))
        goto L_1a2a;
    else
        goto L_1a24;

L_1a24:
    iLvl = i;

L_1a2a:
    i = (i + 0x1);

L_1a2e:
    if ((i < 0x6))
        goto L_19e7;
    else
        goto L_1a37;

L_1a37:
    rgplr[iplr].rgTech[iLvl] = (rgplr[iplr].rgTech[iLvl] + 0x1);
    iPass = (iPass + 0x1);

L_1a53:
    if ((iPass < 0x6))
        goto L_19da;
    else
        goto L_1a5c;

L_1a5c:
    wtNext = wtThreshhold;
    goto LChgMin;

L_1a6b:
    lppl = (lppl + 0x1);

L_1a6f:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_165c;
    else
        goto L_1a7d;

L_1a7d:
    lpth = (lpth + 0x1);

L_1a81:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_0b7a;
    else
        goto L_1a8f;

L_1a8f:
    return;
}

int16_t IdmGiveTraderPart(GrbitTrader grbitTrader, int16_t iplr, uint16_t *piGoto) {
    uint16_t  iGoto;
    MessageId idm;

L_1a96:
    rgplr[iplr].grbitTrader = (rgplr[iplr].grbitTrader | grbitTrader);
    idm = idmHasAbsorbedMysteryTraderHaveGivenPlans;
    goto L_1b28;

L_1abb:
    iGoto = 0xcc04;
    goto L_1b8b;

L_1ac3:
    iGoto = 0xcb04;
    goto L_1b8b;

L_1acb:
    iGoto = 0xc206;
    goto L_1b8b;

L_1ad3:
    iGoto = 0xc309;
    goto L_1b8b;

L_1adb:
    iGoto = 0xc706;
    goto L_1b8b;

L_1ae3:
    iGoto = 0xc608;
    goto L_1b8b;

L_1aeb:
    iGoto = 0xc412;
    goto L_1b8b;

L_1af3:
    iGoto = 0xc507;
    goto L_1b8b;

L_1afb:
    idm = idmHasAbsorbedMysteryTraderReturnHaveGiven;
    iGoto = 0xce1e;
    goto L_1b8b;

L_1b08:
    iGoto = 0xc008;
    goto L_1b8b;

L_1b10:
    idm = idmHasAbsorbedMysteryTraderReturnHaveGiven2;
    iGoto = 0xcf0e;
    goto L_1b8b;

L_1b1d:
    iGoto = 0xcc09;
    goto L_1b8b;

L_1b28:
    if ((grbitTrader == grbitTraderCargo))
        goto L_1abb;
    else
        goto L_1b30;

L_1b30:
    if ((grbitTrader == grbitTraderSpecial))
        goto L_1ac3;
    else
        goto L_1b38;

L_1b38:
    if ((grbitTrader == grbitTraderShield))
        goto L_1acb;
    else
        goto L_1b40;

L_1b40:
    if ((grbitTrader == grbitTraderArmor))
        goto L_1ad3;
    else
        goto L_1b48;

L_1b48:
    if ((grbitTrader == grbitTraderMiner))
        goto L_1adb;
    else
        goto L_1b50;

L_1b50:
    if ((grbitTrader == grbitTraderBomb))
        goto L_1ae3;
    else
        goto L_1b58;

L_1b58:
    if ((grbitTrader == grbitTraderTorp))
        goto L_1af3;
    else
        goto L_1b60;

L_1b60:
    if ((grbitTrader == grbitTraderBeam))
        goto L_1aeb;
    else
        goto L_1b68;

L_1b68:
    if ((grbitTrader == grbitTraderHull))
        goto L_1afb;
    else
        goto L_1b70;

L_1b70:
    if ((grbitTrader == grbitTraderEngine))
        goto L_1b08;
    else
        goto L_1b78;

L_1b78:
    if ((grbitTrader == grbitTraderGenesis))
        goto L_1b10;
    else
        goto L_1b80;

L_1b80:
    if ((grbitTrader != grbitTraderJumpgate))
        goto L_1abb;
    else
        goto L_1b85;

L_1b85:

L_1b8b:
    *(piGoto) = iGoto;

L_1b99:
    return idm;
}
