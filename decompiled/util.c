#include "common.h"

uint32_t rgcrDrawStars2b[5] = {8355711, 127, 32512, 8323072};
uint32_t rgcrDrawStars2a[5] = {12632256, 255, 65280, 16711680};
uint32_t rgcrDrawStars[5] = {8355711, 16777215, 255, 65280, 16711680};
int32_t  rgDSDivCnt2[5] = {80000, 210000, 310000, 260000};
int32_t  rgDSDivCnt[5] = {28000, 28000, 63000, 95000, 73000};
uint8_t  vrgbTachyon[18] = {100, 95, 93, 91, 90, 89, 88, 87, 86, 86, 85, 84, 84, 83, 83, 82, 82, 81};

int16_t FLookupSelPlanet(PLANET *ppl) {
L_0000:
    if ((sel.scan.grobj == grobjPlanet))
        goto L_0019;
    else
        goto L_0013;

L_0013:
    return 0x0;

L_0019:
    return FLookupPlanet(sel.scan.idpl, ppl);
}

int16_t FDupPlanet(PLANET *lppl, PLANET *ppl) {
    PLPROD *lpplprodT;

L_0032:
    lpplprodT = ppl->lpplprod;
    *(ppl) = *(lppl);
    ppl->lpplprod = lpplprodT;
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_00c0;
    else
        goto L_007d;

L_007d:
    if ((HIWORD(lppl->lpplprod) != 0x0))
        goto L_00c0;
    else
        goto L_0087;

L_0087:
    if ((LOWORD(ppl->lpplprod) != 0x0))
        goto L_009c;
    else
        goto L_0093;

L_0093:
    if ((HIWORD(ppl->lpplprod) == 0x0))
        goto L_00ba;
    else
        goto L_009c;

L_009c:
    FreePl(ppl->lpplprod);
    ppl->lpplprod = 0x0;

L_00ba:
    return 0x1;

L_00c0:
    if ((LOWORD(ppl->lpplprod) != 0x0))
        goto L_0100;
    else
        goto L_00cc;

L_00cc:
    if ((HIWORD(ppl->lpplprod) != 0x0))
        goto L_0100;
    else
        goto L_00d5;

L_00d5:
    ppl->lpplprod = LpplAlloc(0x4, lppl->lpplprod->iprodMax, htOrd);
    goto L_0153;

L_0100:
    if ((ppl->lpplprod->iprodMax >= lppl->lpplprod->iprodMac))
        goto L_0153;
    else
        goto L_012a;

L_012a:
    ppl->lpplprod = LpplReAlloc(ppl->lpplprod, lppl->lpplprod->iprodMax);

L_0153:
    fmemcpy(&(ppl->lpplprod->rgprod), &(lppl->lpplprod->rgprod), (lppl->lpplprod->iprodMac * 0x4));
    ppl->lpplprod->iprodMac = lppl->lpplprod->iprodMac;
    return 0x1;
}

THING *LpthFromId(int16_t idth) {
    THING *lpth;
    THING *lpthMac;
    THING *t_merge_0218_0001_wide;

L_01b2:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_0201;

L_01e6:
    if ((LOWORD(lpth) != idth))
        goto L_01fd;
    else
        goto L_01f4;

L_01f4:
    t_merge_0218_0001_wide = lpth;
    goto L_0218;

L_01fd:
    lpth = (lpth + 0x1);

L_0201:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_01e6;
    else
        goto L_020f;

L_020f:
    t_merge_0218_0001_wide = 0x0;

L_0218:
    return t_merge_0218_0001_wide;
}

PLANET *LpplFromId(int16_t idPlanet) {
    int16_t idGuess;
    int16_t iLo;
    PLANET *lppl;
    int16_t iGuess;
    int16_t iHi;
    PLANET *t_merge_02f0_0001_wide;

L_021e:
    if ((idPlanet < 0x0))
        goto L_023b;
    else
        goto L_0230;

L_0230:
    if ((idPlanet < game.cPlanMax))
        goto L_0244;
    else
        goto L_023b;

L_023b:
    t_merge_02f0_0001_wide = 0x0;
    goto L_02f0;

L_0244:
    if ((cPlanet != game.cPlanMax))
        goto L_026f;
    else
        goto L_0250;

L_0250:
    lppl = &(lpPlanets[idPlanet]);
    t_merge_02f0_0001_wide = lppl;
    goto L_02f0;

L_026f:
    iLo = 0xffff;
    iHi = cPlanet;

L_027a:
    if (((iLo + 0x1) >= iHi))
        goto L_02e7;
    else
        goto L_0288;

L_0288:
    iGuess = ((iLo + iHi) >> 0x1);
    lppl = &(lpPlanets[iGuess]);
    idGuess = lpPlanets[iGuess].id;
    if ((idGuess >= idPlanet))
        goto L_02c7;
    else
        goto L_02be;

L_02be:
    iLo = iGuess;
    goto L_027a;

L_02c7:
    if ((idGuess <= idPlanet))
        goto L_02db;
    else
        goto L_02d2;

L_02d2:
    iHi = iGuess;
    goto L_027a;

L_02db:
    t_merge_02f0_0001_wide = lppl;
    goto L_02f0;

L_02e7:
    t_merge_02f0_0001_wide = 0x0;

L_02f0:
    return t_merge_02f0_0001_wide;
}

void CalcPctSurvive(PLANET *lppl, float *ppct, float *ppctSmart) {
    int16_t iPlrSav;
    int32_t cDefenses;
    float   pct;
    PART    part;
    int16_t cMax;

L_02f6:
    if ((ppctSmart == 0x0))
        goto L_0317;
    else
        goto L_0308;

L_0308:
    *(ppctSmart) = 1;

L_0317:
    if ((lppl->iPlayer == 0xffff))
        goto L_0487;
    else
        goto L_0324;

L_0324:
    if ((lppl->cDefenses != 0x0))
        goto L_0345;
    else
        goto L_033d;

L_033d:
    if (((*(lppl + 0x1a) & 0x0) == 0x0))
        goto L_0487;
    else
        goto L_0345;

L_0345:
    iPlrSav = idPlayer;
    idPlayer = lppl->iPlayer;
    if ((FGetBestDefensePart(&(part)) == 0x0))
        goto L_0473;
    else
        goto L_0369;

L_0369:
    LOWORD(cDefenses) = lppl->cDefenses;
    HIWORD(cDefenses) = (*(lppl + 0x1a) & 0x0);
    cMax = CMaxOperableDefenses(lppl, lppl->iPlayer, 0x0);
    if ((SIGNHIWORD(cMax) > HIWORD(cDefenses)))
        goto L_03bf;
    else
        goto L_03a8;

L_03a8:
    if ((SIGNHIWORD(cMax) < HIWORD(cDefenses)))
        goto L_03b5;
    else
        goto L_03ad;

L_03ad:
    if ((cMax >= LOWORD(cDefenses)))
        goto L_03bf;
    else
        goto L_03b5;

L_03b5:
    cDefenses = (uint32_t)(cMax);

L_03bf:
    /* untranslated: ss:[bp-0x1e] = part[52:2](part[4:4](part)) */
    /* untranslated: ss:[bp-0x1c] = signhiword(part[52:2](part[4:4](part))) */
    /* untranslated: pct = pow((1 - ((double)dword ss:[bp-0x1e] / 1000)), (double)cDefenses) */
    if ((ppctSmart == 0x0))
        goto L_047e;
    else
        goto L_041a;

L_041a:
    /* untranslated: ss:[bp-0x1e] = part[52:2](part[4:4](part)) */
    /* untranslated: ss:[bp-0x1c] = signhiword(part[52:2](part[4:4](part))) */
    /* untranslated: *ppctSmart = pow((1 - ((double)dword ss:[bp-0x1e] / 2000)), (double)cDefenses) */

L_0473:
    pct = 1;

L_047e:
    idPlayer = iPlrSav;
    goto L_0492;

L_0487:
    pct = 1;

L_0492:
    *(ppct) = pct;
    return;
}

int16_t FLookupPlanet(int16_t iPlanet, PLANET *ppl) {
    PLANET *lpPl;
    int16_t fWrite;

L_04a6:
    fWrite = 0x0;
    if ((cPlanet > 0x0))
        goto L_04c4;
    else
        goto L_04be;

L_04be:
    return 0x0;

L_04c4:
    if ((iPlanet >= 0x0))
        goto L_04fc;
    else
        goto L_04cd;

L_04cd:
    iPlanet = ppl->id;
    if ((iPlanet != 0xffff))
        goto L_04f7;
    else
        goto L_04de;

L_04de:
    LogChangePlanet(0x0, ppl);
    return 0x1;

L_04f7:
    fWrite = 0x1;

L_04fc:
    lpPl = LpplFromId(iPlanet);
    if ((LOWORD(lpPl) != 0x0))
        goto L_051f;
    else
        goto L_0516;

L_0516:
    if ((HIWORD(lpPl) == 0x0))
        goto L_0722;
    else
        goto L_051f;

L_051f:
    if ((ppl == 0x0))
        goto L_0722;
    else
        goto L_0528;

L_0528:
    if ((fWrite == 0x0))
        goto L_06ed;
    else
        goto L_0531;

L_0531:
    InvalidateReport(0x0, 0x0);
    LogChangePlanet(lpPl, ppl);
    if ((LOWORD(lpPl->lpplprod) != LOWORD(ppl->lpplprod)))
        goto L_0570;
    else
        goto L_0567;

L_0567:
    if ((HIWORD(lpPl->lpplprod) == HIWORD(ppl->lpplprod)))
        goto FinishCopy;
    else
        goto L_0570;

L_0570:
    if ((LOWORD(lpPl->lpplprod) != 0x0))
        goto L_05c8;
    else
        goto L_057d;

L_057d:
    if ((HIWORD(lpPl->lpplprod) != 0x0))
        goto L_05c8;
    else
        goto L_0587;

L_0587:
    if ((LOWORD(ppl->lpplprod) != 0x0))
        goto L_059c;
    else
        goto L_0593;

L_0593:
    if ((HIWORD(ppl->lpplprod) == 0x0))
        goto L_0605;
    else
        goto L_059c;

L_059c:
    lpPl->lpplprod = LpplAlloc(0x4, ppl->lpplprod->iprodMac, htOrd);

L_05c8:
    if ((LOWORD(ppl->lpplprod) != 0x0))
        goto L_0605;
    else
        goto L_05d4;

L_05d4:
    if ((HIWORD(ppl->lpplprod) != 0x0))
        goto L_0605;
    else
        goto L_05dd;

L_05dd:
    FreePl(lpPl->lpplprod);
    lpPl->lpplprod = 0x0;
    goto FinishCopy;

L_0605:
    if ((lpPl->lpplprod->iprodMax >= ppl->lpplprod->iprodMac))
        goto L_065e;
    else
        goto L_062f;

L_062f:
    lpPl->lpplprod = LpplReAlloc(lpPl->lpplprod, (ppl->lpplprod->iprodMac + 0x2));

L_065e:
    fmemcpy(&(lpPl->lpplprod->rgprod), &(ppl->lpplprod->rgprod), (ppl->lpplprod->iprodMac * 0x4));
    lpPl->lpplprod->iprodMac = ppl->lpplprod->iprodMac;

FinishCopy:
    fmemcpy(&(lpPl), &(ppl), 0x34);
    if ((gd.fTutorial == 0x0))
        goto L_0722;
    else
        goto L_06db;

L_06db:
    if ((idPlayer != 0x0))
        goto L_0722;
    else
        goto L_06e5;

L_06e5:
    AdvanceTutor();

L_06ed:
    if ((ppl != 0x49ee))
        goto L_070b;
    else
        goto L_06f7;

L_06f7:
    FDupPlanet(lpPl, ppl);
    goto L_0722;

L_070b:
    *(ppl) = *(lpPl);

L_0722:
    if ((LOWORD(lpPl) != 0x0))
        goto L_0734;
    else
        goto L_072b;

L_072b:
    if ((HIWORD(lpPl) == 0x0))
        goto L_073a;
    else
        goto L_0734;

L_0734:
    return 0x1;

L_073a:
    return 0x0;
}

int32_t DpOfLpflIshdef(FLEET *lpfl, int16_t ishdef) {
    int16_t dpShdef;
    int32_t dp;

L_0746:
    dpShdef = rglpshdef[lpfl->iPlayer][ishdef].hul.dp;
    /* untranslated: LOWORD(dp) = loword((int32_t)(words(hiword((uint32_t)(sext16to32(lpfl->rgcsh[ishdef]) *
     * words(signhiword(loword(((words(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)),
     * signhiword(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)))) / 0xa) * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) +
     * (ishdef * 0x2))] >> 0x7) & 0x1ff)))), loword(((words(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)),
     * signhiword(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)))) / 0xa) * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) +
     * (ishdef * 0x2))] >> 0x7) & 0x1ff)))))), loword((uint32_t)(sext16to32(lpfl->rgcsh[ishdef]) *
     * words(signhiword(loword(((words(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)),
     * signhiword(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)))) / 0xa) * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) +
     * (ishdef * 0x2))] >> 0x7) & 0x1ff)))), loword(((words(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)),
     * signhiword(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)))) / 0xa) * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) +
     * (ishdef * 0x2))] >> 0x7) & 0x1ff))))))) / 0x1388)) */
    /* untranslated: HIWORD(dp) = hiword((int32_t)(words(hiword((uint32_t)(sext16to32(lpfl->rgcsh[ishdef]) *
     * words(signhiword(loword(((words(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)),
     * signhiword(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)))) / 0xa) * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) +
     * (ishdef * 0x2))] >> 0x7) & 0x1ff)))), loword(((words(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)),
     * signhiword(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)))) / 0xa) * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) +
     * (ishdef * 0x2))] >> 0x7) & 0x1ff)))))), loword((uint32_t)(sext16to32(lpfl->rgcsh[ishdef]) *
     * words(signhiword(loword(((words(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)),
     * signhiword(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)))) / 0xa) * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) +
     * (ishdef * 0x2))] >> 0x7) & 0x1ff)))), loword(((words(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)),
     * signhiword(loword(((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] & 0x7f) * dpShdef)))) / 0xa) * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) +
     * (ishdef * 0x2))] >> 0x7) & 0x1ff))))))) / 0x1388)) */

L_07f8:
    return dp;
}

int16_t FLookupThing(int16_t idth, THING *pth) {
    THING  *lpth;
    int16_t fWrite;

L_07fe:
    fWrite = 0x0;
    if ((cThing > 0x0))
        goto L_081c;
    else
        goto L_0816;

L_0816:
    return 0x0;

L_081c:
    if ((idth >= 0x0))
        goto L_0832;
    else
        goto L_0825;

L_0825:
    idth = pth;
    fWrite = 0x1;

L_0832:
    lpth = LpthFromId(idth);
    if ((LOWORD(lpth) != 0x0))
        goto L_0855;
    else
        goto L_084c;

L_084c:
    if ((HIWORD(lpth) == 0x0))
        goto L_08cd;
    else
        goto L_0855;

L_0855:
    if ((pth == 0x0))
        goto L_08cd;
    else
        goto L_085e;

L_085e:
    if ((fWrite == 0x0))
        goto L_08b6;
    else
        goto L_0867;

L_0867:
    LogChangeThing(lpth, pth);
    fmemcpy(&(lpth), &(pth), 0x12);
    if ((gd.fTutorial == 0x0))
        goto L_08cd;
    else
        goto L_08a4;

L_08a4:
    if ((idPlayer != 0x0))
        goto L_08cd;
    else
        goto L_08ae;

L_08ae:
    AdvanceTutor();

L_08b6:
    *(pth) = *(lpth);

L_08cd:
    if ((LOWORD(lpth) != 0x0))
        goto L_08df;
    else
        goto L_08d6;

L_08d6:
    if ((HIWORD(lpth) == 0x0))
        goto L_08e5;
    else
        goto L_08df;

L_08df:
    return 0x1;

L_08e5:
    return 0x0;
}

void SelectOursAtObject(POINT *ppt) {
    int16_t id;
    POINT   pt;
    int16_t ish;
    int16_t i;
    FLEET  *lpfl;
    SCAN    scan;

L_08f2:
    if ((ppt->x != 0xffff))
        goto L_094b;
    else
        goto L_0906;

L_0906:
    if (((ppt->y & 0x8000) == 0x0))
        goto L_0930;
    else
        goto L_0917;

L_0917:
    SelectAdjFleet(0x0, (ppt->y & 0x7fff));
    goto L_0b2b;

L_0930:
    pt.x = rgptPlan[ppt->y].x;
    pt.y = rgptPlan[ppt->y].y;
    goto L_0959;

L_094b:
    pt.x = ppt->x;
    pt.y = ppt->y;

L_0959:
    id = 0xffff;
    ish = 0x0;
    goto L_096a;

L_0966:
    ish = (ish + 0x1);

L_096a:
    if ((ish >= cFleet))
        goto L_0a01;
    else
        goto L_0975;

L_0975:
    lpfl = rglpfl[ish];
    if ((LOWORD(rglpfl[ish]) != 0x0))
        goto L_09a5;
    else
        goto L_099d;

L_099d:
    if ((*(rglpfl[ish] + 0x2) == 0x0))
        goto L_0a01;
    else
        goto L_09a5;

L_09a5:
    /* untranslated: branch ss:[pt] != lpfl->pt.x ? L_0966 : L_09bf */

L_09bf:
    /* untranslated: branch ss:[pt+0x2] != lpfl->pt.y ? L_0966 : L_09c8 */

L_09c8:
    if ((lpfl->iPlayer != idPlayer))
        goto L_09ec;
    else
        goto L_09d7;

L_09d7:
    SelectAdjFleet(0x0, LOWORD(lpfl));
    goto L_0b2b;

L_09ec:
    if ((id != 0xffff))
        goto L_0966;
    else
        goto L_09f5;

L_09f5:
    id = LOWORD(lpfl);

L_0a01:
    i = 0x0;
    goto L_0a0d;

L_0a09:
    i = (i + 0x1);

L_0a0d:
    if ((i >= game.cPlanMax))
        goto L_0a4c;
    else
        goto L_0a18;

L_0a18:
    if ((rgptPlan[i].x != pt.x))
        goto L_0a09;
    else
        goto L_0a2e;

L_0a2e:
    if ((rgptPlan[i].y != pt.y))
        goto L_0a09;
    else
        goto L_0a37;

L_0a37:
    SelectAdjPlanet(0x0, i);
    goto L_0b2b;

L_0a4c:
    scan.iwp = 0xffff;
    /* untranslated: branch FFindNearestObject(words(pt.y, pt.x), grobjThing, &scan) == 0x0 ? L_0b13 : L_0a6f */

L_0a6f:
    if ((scan.grobj != grobjThing))
        goto L_0b13;
    else
        goto L_0a78;

L_0a78:
    if ((scan.ith == 0xffff))
        goto L_0b13;
    else
        goto L_0a81;

L_0a81:
    if ((((LOWORD(lpThings[scan.ith]) >> 0xd) & 0x7) != 0x1))
        goto L_0b13;
    else
        goto L_0aa6;

L_0aa6:
    /* untranslated: branch ((part[6:2](lpThings[scan.ith]) >> 0xa) & 0xf) != 0x0 ? L_0b13 : L_0acc */

L_0acc:
    ChangeScanSel(&(scan), 0x1);
    /* untranslated: call FEnsurePointOnScreen(words(scan.pt.y, scan.pt.x), 0x1) -> callresult(int16_t) */
    UpdateWindow(hwndScanner);
    SendMessage(hwndScanner, WM_CHAR, 0x76, 0x0);
    goto L_0b2b;

L_0b13:
    if ((id == 0xffff))
        goto L_0b2b;
    else
        goto L_0b1c;

L_0b1c:
    SelectAdjFleet(0x0, id);

L_0b2b:
    return;
}

int32_t LComputePower(SHDEF *lpshdef) {
    int16_t dSpeed;
    int16_t dxRange;
    int16_t ihs;
    int32_t dpTorps;
    int16_t i;
    int32_t pctCap;
    int32_t dpBeams;
    int32_t dpBombs;
    int32_t dp;
    PART    part;

L_0b32:
    dpBombs = 0x0;
    dpBeams = 0x0;
    dpTorps = 0x0;
    dxRange = 0x3e7;
    pctCap = 0x3e8;
    ihs = 0x0;
    goto L_0b74;

L_0b70:
    ihs = (ihs + 0x1);

L_0b74:
    if ((ihs >= lpshdef->hul.chs))
        goto L_0d6c;
    else
        goto L_0b86;

L_0b86:
    part.hs.grhst = lpshdef->hul.rghs[ihs].grhst;
    /* untranslated: HIWORD(part) = lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[ihs])] */
    if ((part.hs.cItem == 0x0))
        goto L_0b70;
    else
        goto L_0bb9;

L_0bb9:

L_0bbf:
    if ((FLookupPart(&(part)) == 0x0))
        goto L_0b70;
    else
        goto L_0bd3;

L_0bd3:
    goto L_0d46;

L_0bd9:
    /* untranslated: dp = (int32_t)((uint32_t)((uint32_t)(sext16to32(part[54:2](part[4:4](part))) * words(0x0, part.hs.cItem)) *
     * sext16to32((part[52:2](part[4:4](part)) + 0x3))) / 0x4) */
    /* untranslated: branch (part[58:2](part[4:4](part)) & 0x1) == 0x0 ? L_0c4c : L_0c33 */

L_0c33:
    dp = (int32_t)((dp / 0x3));

L_0c4c:
    dpBeams = (dpBeams + dp);
    goto L_0b70;

L_0c5b:
    /* untranslated: dpTorps = (dpTorps + (int32_t)((uint32_t)((uint32_t)(sext16to32(part[54:2](part[4:4](part))) * words(0x0, part.hs.cItem)) *
     * sext16to32((part[52:2](part[4:4](part)) + 0xfffe))) / 0x2)) */
    goto L_0b70;

L_0ca6:
    /* untranslated: LOWORD(dpBombs) = (LOWORD(dpBombs) + (loword(((part[54:2](part[4:4](part)) + part[56:2](part[4:4](part))) * part.hs.cItem)) * 0x2)) */
    HIWORD(dpBombs) = (HIWORD(dpBombs) + 0x0);
    goto L_0b70;

L_0cd0:
    goto L_0d2d;

L_0cd9:
    i = part.hs.cItem;
    goto L_0d1e;

L_0cea:
    /* untranslated: pctCap = (int32_t)((uint32_t)(pctCap * words((signhiword(part[52:2](part[4:4](part))) + 0x0), (part[52:2](part[4:4](part)) + 0x64))) /
     * 0x64) */
    i = (i - 0x1);

L_0d1e:
    if ((i <= 0x0))
        goto L_0b70;
    else
        goto L_0d24;

L_0d24:

L_0d2d:
    if ((part.hs.iItem == ispecialEEnergyCapacitor))
        goto L_0cd9;
    else
        goto L_0d35;

L_0d35:
    if ((part.hs.iItem != ispecialEFluxCapacitor))
        goto L_0b70;
    else
        goto L_0d3a;

L_0d3a:

L_0d46:
    if ((part.hs.grhst == hstBeam))
        goto L_0bd9;
    else
        goto L_0d4e;

L_0d4e:
    if ((part.hs.grhst == hstTorp))
        goto L_0c5b;
    else
        goto L_0d56;

L_0d56:
    if ((part.hs.grhst == hstBomb))
        goto L_0ca6;
    else
        goto L_0d5e;

L_0d5e:
    if ((part.hs.grhst != hstSpecialE))
        goto L_0b70;
    else
        goto L_0d63;

L_0d63:

L_0d6c:
    if ((LOWORD(pctCap) != 0x3e8))
        goto L_0d7f;
    else
        goto L_0d76;

L_0d76:
    if ((HIWORD(pctCap) == 0x0))
        goto L_0de0;
    else
        goto L_0d7f;

L_0d7f:
    pctCap = (int32_t)((pctCap / 0xa));
    if ((HIWORD(pctCap) < 0x0))
        goto L_0dba;
    else
        goto L_0da1;

L_0da1:
    if ((HIWORD(pctCap) > 0x0))
        goto L_0db0;
    else
        goto L_0da6;

L_0da6:
    if ((LOWORD(pctCap) <= 0xff))
        goto L_0dba;
    else
        goto L_0db0;

L_0db0:
    pctCap = 0xff;

L_0dba:
    dpBeams = (int32_t)(((uint32_t)((dpBeams * pctCap)) / 0x64));

L_0de0:
    dSpeed = SpdOfShip(0x0, 0x0, 0x0, 0x0, lpshdef);
    dpBeams = (dpBeams + (int32_t)(((uint32_t)((dpBeams * (uint32_t)((dSpeed + 0xfffc)))) / 0xa)));

L_0e47:
    /* untranslated: return words(((LOWORD(dpBombs) + LOWORD(dpBeams)) + LOWORD(dpTorps)), ((HIWORD(dpBombs) + HIWORD(dpBeams)) + HIWORD(dpTorps))) */
}

void ComputeShdefPowers() {
    int16_t iplr;
    int16_t ishdef;

L_0e4e:
    iplr = 0x0;
    goto L_0e63;

L_0e5f:
    iplr = (iplr + 0x1);

L_0e63:
    if ((iplr >= game.cPlayer))
        goto L_0f1e;
    else
        goto L_0e6e;

L_0e6e:
    if ((LOWORD(rglpshdef[iplr]) != 0x0))
        goto L_0e89;
    else
        goto L_0e7f;

L_0e7f:
    if ((HIWORD(rglpshdef[iplr]) == 0x0))
        goto L_0e5f;
    else
        goto L_0e89;

L_0e89:
    ishdef = 0x0;
    goto L_0f12;

L_0e91:
    /* untranslated: branch ((part[123:2](rglpshdef[iplr][ishdef]) >> 0x9) & 0x1) != 0x0 ? L_0f0e : L_0ebe */

L_0ebe:
    LComputePower(&(rglpshdef[iplr][ishdef]));
    /* untranslated: ss:[bp-0x8] = hiword(callresult(int32_t)) */
    /* untranslated: part[135:2](rglpshdef[iplr][ishdef]) = loword(callresult(int32_t)) */
    /* untranslated: part[137:2](rglpshdef[iplr][ishdef]) = ss:[bp-0x8] */

L_0f0e:
    ishdef = (ishdef + 0x1);

L_0f12:
    if ((ishdef >= 0x10))
        goto L_0e5f;
    else
        goto L_0f18;

L_0f18:

L_0f1e:
    return;
}

int32_t DpShieldOfShdef(SHDEF *lpshdef, int16_t iplr) {
    int16_t chs;
    HS     *lphs;
    int16_t ihs;
    int32_t dpShdef;
    HUL    *lphul;
    PART    part;

L_0f24:
    dpShdef = 0x0;
    lphul = lpshdef;
    lphs = &(*(lphul + 0x3a));
    chs = lphul->chs;
    ihs = 0x0;
    goto L_0f7c;

L_0f69:
    ihs = (ihs + 0x1);
    lphs = (lphs + 0x1);

L_0f7c:
    if ((ihs >= chs))
        goto L_107e;
    else
        goto L_0f87;

L_0f87:
    if ((lphs->grhst != hstShield))
        goto L_0feb;
    else
        goto L_0f93;

L_0f93:
    if ((lphs->cItem <= 0x0))
        goto L_0feb;
    else
        goto L_0faa;

L_0faa:
    part.hs.grhst = lphs->grhst;
    HIWORD(part) = *(lphs + 0x2);
    FLookupPart(&(part));
    /* untranslated: LOWORD(dpShdef) = (LOWORD(dpShdef) + loword((part[52:2](part[4:4](part)) * lphs->cItem))) */
    HIWORD(dpShdef) = (HIWORD(dpShdef) + 0x0);
    goto L_0f69;

L_0feb:
    if ((lphs->grhst != hstArmor))
        goto L_1040;
    else
        goto L_0ff7;

L_0ff7:
    if ((lphs->cItem <= 0x0))
        goto L_1040;
    else
        goto L_100e;

L_100e:
    if ((lphs->iItem != iarmorFieldedKelarium))
        goto L_1040;
    else
        goto L_1020;

L_1020:
    LOWORD(dpShdef) = (LOWORD(dpShdef) + LOWORD((lphs->cItem * 0x32)));
    HIWORD(dpShdef) = (HIWORD(dpShdef) + 0x0);
    goto L_0f69;

L_1040:
    if ((lphs->grhst != hstArmor))
        goto L_0f69;
    else
        goto L_104c;

L_104c:
    if ((lphs->iItem != iarmorMegaPolyShell))
        goto L_0f69;
    else
        goto L_105e;

L_105e:
    LOWORD(dpShdef) = (LOWORD(dpShdef) + LOWORD((lphs->cItem * 0x64)));
    HIWORD(dpShdef) = (HIWORD(dpShdef) + 0x0);

L_107e:
    if ((GetRaceGrbit(rgplr[iplr], ibitRaceRegeneratingShields) == 0x0))
        goto L_10c1;
    else
        goto L_109e;

L_109e:
    dpShdef = (dpShdef + (int32_t)(((int32_t)((dpShdef * 0x2)) / 0x5)));

L_10c1:
    if (((LOWORD(dpShdef) & 0x0) != 0x0))
        goto L_10dd;
    else
        goto L_10d5;

L_10d5:
    if (((HIWORD(dpShdef) & 0xffff) == 0x0))
        goto L_10e7;
    else
        goto L_10dd;

L_10dd:
    dpShdef = 0xffff;

L_10e7:

L_10f3:
    /* untranslated: return words(LOWORD(dpShdef), 0x0) */
}

int16_t IStargateFromLppl(PLANET *lppl) {
    int16_t chs;
    HS     *lphs;
    int16_t ihs;
    HUL    *lphul;

L_10fa:
    if ((LOWORD(lppl) != 0x0))
        goto L_1115;
    else
        goto L_110c;

L_110c:
    if ((HIWORD(lppl) == 0x0))
        goto L_112c;
    else
        goto L_1115;

L_1115:
    if ((lppl->fStarbase != 0x0))
        goto L_1132;
    else
        goto L_112c;

L_112c:
    return 0xffff;

L_1132:
    lphul = &(rglpshdefSB[lppl->iPlayer][lppl->isb]);
    lphs = &(*(lphul + 0x3a));
    chs = lphul->chs;
    ihs = 0x0;
    goto L_1195;

L_1182:
    ihs = (ihs + 0x1);
    lphs = (lphs + 0x1);

L_1195:
    if ((ihs >= chs))
        goto L_11e6;
    else
        goto L_11a0;

L_11a0:
    if ((lphs->grhst != hstSpecialSB))
        goto L_1182;
    else
        goto L_11ad;

L_11ad:
    if ((lphs->cItem <= 0x0))
        goto L_1182;
    else
        goto L_11c4;

L_11c4:
    if ((lphs->iItem >= ispecialSBMassDriver5))
        goto L_1182;
    else
        goto L_11d6;

L_11d6:
    return lphs->iItem;

L_11e6:
    return 0xffff;
}

char *PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr) {
    char *pchEnd;
    char  szName[50];

L_11f2:
    if ((pplr != 0x0))
        goto L_1212;
    else
        goto L_1204;

L_1204:
    pplr = rgplr[iPlayer];

L_1212:
    if (((uint16_t)(pplr->szName[0x0]) == 0x0))
        goto L_13b0;
    else
        goto L_1222;

L_1222:
    if ((fThe == 0x0))
        goto L_124b;
    else
        goto L_122b;

L_122b:
    strcpy(szName, 0x50e);
    if ((fCapital == 0x0))
        goto L_124f;
    else
        goto L_1244;

L_1244:
    /* untranslated: part[0:1](szName) = 0x54 */

L_124b:
    /* untranslated: part[0:1](szName) = 0x0 */

L_124f:
    if ((fPlural == 0x0))
        goto L_1280;
    else
        goto L_1258;

L_1258:
    if (((uint16_t)(pplr->szNames[0x0]) == 0x0))
        goto L_1280;
    else
        goto L_1268;

L_1268:
    /* untranslated: call strcat(szName, part[16:0](game.szName[pplr])) -> callresult(char *) */
    goto L_1295;

L_1280:
    strcat(szName, (pplr + 0x80));

L_1295:
    pchEnd = (szName[strlen(szName)] + 0xffff);

L_12ae:
    if (((uint16_t)(*(pchEnd)) != 0x20))
        goto L_12d4;
    else
        goto L_12bc;

L_12bc:
    if ((pchEnd < szName))
        goto L_12d4;
    else
        goto L_12c7;

L_12c7:
    pchEnd = (pchEnd - 0x1);
    *(pchEnd) = 0x0;
    goto L_12ae;

L_12d4:
    if ((pchEnd >= szName))
        goto L_12ef;
    else
        goto L_12df;

L_12df:
    CchGetString(idsName, szName);

L_12ef:
    if ((fPlural == 0x0))
        goto L_135c;
    else
        goto L_12f8;

L_12f8:
    if (((uint16_t)(pplr->szNames[0x0]) != 0x0))
        goto L_135c;
    else
        goto L_1308;

L_1308:
    pchEnd = (szName[strlen(szName)] + 0xffff);
    if (((uint16_t)(*(pchEnd)) == 0x73))
        goto L_135c;
    else
        goto L_132f;

L_132f:
    if (((uint16_t)(*(pchEnd)) != 0x65))
        goto L_134c;
    else
        goto L_133d;

L_133d:
    if (((uint16_t)(*(pchEnd - 0x1)) == 0x73))
        goto L_135c;
    else
        goto L_134c;

L_134c:
    strcat(szName, 0x513);

L_135c:
    if ((grWord != 0x1))
        goto L_1386;
    else
        goto L_1365;

L_1365:
    CchGetString(idsHave2, szName[strlen(szName)]);
    goto L_1440;

L_1386:
    if ((grWord != 0x2))
        goto L_1440;
    else
        goto L_138f;

L_138f:
    CchGetString(idsAre, szName[strlen(szName)]);

L_13b0:
    _wsprintf(&(szName), PszGetCompressedString(idsPlayerD2), (iPlayer + 0x1));
    if ((fPlural != 0x0))
        goto L_13ef;
    else
        goto L_13df;

L_13df:
    strcat(szName, 0x515);

L_13ef:
    if ((grWord != 0x1))
        goto L_1419;
    else
        goto L_13f8;

L_13f8:
    CchGetString(idsHas, szName[strlen(szName)]);
    goto L_1440;

L_1419:
    if ((grWord != 0x2))
        goto L_1440;
    else
        goto L_1422;

L_1422:
    CchGetString(idsIs2, szName[strlen(szName)]);

L_1440:
    strcpy(0x57a4, szName);

L_1456:
    return 0x57a4;
}

int16_t FCalcFleetBombDamage(FLEET *lpfl, int32_t *pdmgPeople, int32_t *pdmgPeopleMin, int32_t *pdmgPeopleSmart, int32_t *pdmgBldg, int32_t *ppctTerra,
                             int16_t *pfMulti) {
    int16_t  iplr;
    int16_t  cfl;
    FLEET   *lpflNext;
    int16_t  dmgFloor;
    FLEET   *lpflHead;
    double   dmgSmart;
    int16_t  fBomber;
    int16_t  j;
    int16_t  ishdef;
    PART     part;
    int32_t  cIter;
    double   dmgT;
    uint16_t t_1678;
    uint16_t t_merge_1b39_0001;

L_145c:
    iplr = lpfl->iPlayer;
    cfl = 0x0;
    dmgSmart = 1;
    lpflHead = lpfl;
    ppctTerra = 0x0;
    *(ppctTerra + 0x2) = 0x0;
    pdmgPeopleMin = 0x0;
    *(pdmgPeopleMin + 0x2) = 0x0;
    pdmgBldg = 0x0;
    *(pdmgBldg + 0x2) = 0x0;
    pdmgPeopleSmart = 0x0;
    *(pdmgPeopleSmart + 0x2) = 0x0;
    pdmgPeople = 0x0;
    *(pdmgPeople + 0x2) = 0x0;
    goto L_1ac3;

L_14c0:
    fBomber = 0x0;
    lpfl->fBombed = 0x1;
    ishdef = 0x0;
    goto L_1a05;

L_14e1:
    if ((lpfl->rgcsh[ishdef] <= 0x0))
        goto L_1a01;
    else
        goto L_14fe;

L_14fe:
    j = 0x0;
    goto L_150a;

L_1506:
    j = (j + 0x1);

L_150a:
    if ((j >= rglpshdef[iplr][ishdef].hul.chs))
        goto L_1a01;
    else
        goto L_1532;

L_1532:
    if ((rglpshdef[iplr][ishdef].hul.rghs[j].grhst != hstBomb))
        goto L_17a4;
    else
        goto L_1562;

L_1562:
    part.hs.grhst = rglpshdef[iplr][ishdef].hul.rghs[j].grhst;
    HIWORD(part) = HIWORD(rglpshdef[iplr][ishdef].hul.rghs[j]);
    FLookupPart(&(part));
    fBomber = 0x1;
    if ((part.hs.iItem != 0x9))
        goto L_15f2;
    else
        goto L_15b5;

L_15b5:
    /* untranslated: ppctTerra = (ppctTerra + loword((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])))) */
    /* untranslated: *(ppctTerra+0x2) = (*(ppctTerra+0x2) + hiword((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])))) */
    goto L_1506;

L_15f2:
    if ((part.hs.cItem <= 0x0))
        goto L_1506;
    else
        goto L_1602;

L_1602:

L_1608:
    /* untranslated: branch part[56:2](part[4:4](part)) != 0x0 ? L_16ac : L_1615 */

L_1615:
    /* untranslated: cIter = (uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) */
    /* untranslated: ss:[bp-0x3a] = part[54:2](part[4:4](part)) */
    /* untranslated: ss:[bp-0x38] = signhiword(part[54:2](part[4:4](part))) */
    /* untranslated: dmgT = (1 - ((double)dword ss:[bp-0x3a] / 1000)) */

L_1675:
    LOWORD(cIter) = (LOWORD(cIter) - 0x1);
    t_1678 = HIWORD(cIter);
    HIWORD(cIter) = (HIWORD(cIter) - 0x0);
    if ((t_1678 < 0x0))
        goto L_1506;
    else
        goto L_168b;

L_168b:
    if ((HIWORD(cIter) > 0x0))
        goto L_1698;
    else
        goto L_1690;

L_1690:
    if ((LOWORD(cIter) <= 0x0))
        goto L_1506;
    else
        goto L_1698;

L_1698:
    dmgSmart = (dmgSmart * dmgT);
    goto L_1675;

L_16ac:
    /* untranslated: pdmgPeople = (pdmgPeople + loword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) *
     * sext16to32(part[54:2](part[4:4](part)))))) */
    /* untranslated: *(pdmgPeople+0x2) = (*(pdmgPeople+0x2) + hiword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) *
     * sext16to32(part[54:2](part[4:4](part)))))) */
    /* untranslated: pdmgBldg = (pdmgBldg + loword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) *
     * sext16to32(part[56:2](part[4:4](part)))))) */
    /* untranslated: *(pdmgBldg+0x2) = (*(pdmgBldg+0x2) + hiword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) *
     * sext16to32(part[56:2](part[4:4](part)))))) */
    if ((part.hs.iItem < 0x0))
        goto L_1506;
    else
        goto L_174a;

L_174a:
    if ((part.hs.iItem > 0x4))
        goto L_1506;
    else
        goto L_1758;

L_1758:
    dmgFloor = 0x3;
    /* untranslated: pdmgPeopleMin = (pdmgPeopleMin + loword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) *
     * sext16to32(dmgFloor)))) */
    /* untranslated: *(pdmgPeopleMin+0x2) = (*(pdmgPeopleMin+0x2) + hiword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) *
     * sext16to32(dmgFloor)))) */

L_17a4:
    if ((rglpshdef[iplr][ishdef].hul.rghs[j].grhst != hstBeam))
        goto L_1918;
    else
        goto L_17d4;

L_17d4:
    if (((HIWORD(rglpshdef[iplr][ishdef].hul.rghs[j]) & 0xff) != 0x12))
        goto L_1918;
    else
        goto L_180a;

L_180a:
    part.hs.grhst = rglpshdef[iplr][ishdef].hul.rghs[j].grhst;
    HIWORD(part) = HIWORD(rglpshdef[iplr][ishdef].hul.rghs[j]);
    fBomber = 0x1;
    /* untranslated: pdmgPeople = (pdmgPeople + loword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) * 0x14))) */
    /* untranslated: *(pdmgPeople+0x2) = (*(pdmgPeople+0x2) + hiword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) *
     * 0x14))) */
    /* untranslated: pdmgBldg = (pdmgBldg + loword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) * 0x5))) */
    /* untranslated: *(pdmgBldg+0x2) = (*(pdmgBldg+0x2) + hiword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) * 0x5))) */
    /* untranslated: pdmgPeopleMin = (pdmgPeopleMin + loword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) * 0x3))) */
    /* untranslated: *(pdmgPeopleMin+0x2) = (*(pdmgPeopleMin+0x2) + hiword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) *
     * 0x3))) */
    goto L_1506;

L_1918:
    if ((rglpshdef[iplr][ishdef].hul.rghs[j].grhst != hstSpecialM))
        goto L_1506;
    else
        goto L_1949;

L_1949:
    if (((HIWORD(rglpshdef[iplr][ishdef].hul.rghs[j]) & 0xff) != 0x1))
        goto L_1506;
    else
        goto L_197f;

L_197f:
    part.hs.grhst = rglpshdef[iplr][ishdef].hul.rghs[j].grhst;
    HIWORD(part) = HIWORD(rglpshdef[iplr][ishdef].hul.rghs[j]);
    fBomber = 0x1;
    /* untranslated: pdmgPeopleMin = (pdmgPeopleMin + loword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) * 0x14))) */
    /* untranslated: *(pdmgPeopleMin+0x2) = (*(pdmgPeopleMin+0x2) + hiword((uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * sext16to32(lpfl->rgcsh[ishdef])) *
     * 0x14))) */

L_1a01:
    ishdef = (ishdef + 0x1);

L_1a05:
    if ((ishdef < 0x10))
        goto L_14e1;
    else
        goto L_1a0e;

L_1a0e:
    cfl = (cfl + 0x1);
    lpflNext = lpfl->lpflNext;

L_1a23:
    if ((LOWORD(lpflNext) != 0x0))
        goto L_1a35;
    else
        goto L_1a2c;

L_1a2c:
    if ((HIWORD(lpflNext) == 0x0))
        goto L_1a6f;
    else
        goto L_1a35;

L_1a35:
    if ((lpflNext->fDead != 0x0))
        goto L_1a5b;
    else
        goto L_1a4c;

L_1a4c:
    if ((lpflNext->iPlayer == iplr))
        goto L_1a6f;
    else
        goto L_1a5b;

L_1a5b:
    lpflNext = lpflNext->lpflNext;
    goto L_1a23;

L_1a6f:
    if ((LOWORD(lpflNext) != 0x0))
        goto L_1a81;
    else
        goto L_1a78;

L_1a78:
    if ((HIWORD(lpflNext) == 0x0))
        goto L_1ab9;
    else
        goto L_1a81;

L_1a81:
    if ((LOWORD(lpflNext) != LOWORD(lpflHead)))
        goto L_1a97;
    else
        goto L_1a8f;

L_1a8f:
    if ((HIWORD(lpflNext) == HIWORD(lpflHead)))
        goto L_1ab9;
    else
        goto L_1a97;

L_1a97:
    if ((lpflNext->idPlanet != lpfl->idPlanet))
        goto L_1ab9;
    else
        goto L_1aaa;

L_1aaa:
    lpfl = lpflNext;
    goto L_1ac3;

L_1ab9:
    lpfl = 0x0;

L_1ac3:
    if ((LOWORD(lpfl) != 0x0))
        goto L_14c0;
    else
        goto L_1acc;

L_1acc:
    if ((HIWORD(lpfl) != 0x0))
        goto L_14c0;
    else
        goto L_1ad5;

L_1ad5:
    __ftol();
    /* untranslated: pdmgPeopleSmart = loword(callresult(int32_t)) */
    /* untranslated: *(pdmgPeopleSmart+0x2) = hiword(callresult(int32_t)) */
    if ((*(pdmgPeopleSmart + 0x2) < 0x0))
        goto L_1b27;
    else
        goto L_1b0d;

L_1b0d:
    if ((*(pdmgPeopleSmart + 0x2) > 0x0))
        goto L_1b1b;
    else
        goto L_1b12;

L_1b12:
    if ((pdmgPeopleSmart < 0x3e8))
        goto L_1b27;
    else
        goto L_1b1b;

L_1b1b:
    pdmgPeopleSmart = 0x3e8;
    *(pdmgPeopleSmart + 0x2) = 0x0;

L_1b27:
    if ((cfl <= 0x1))
        goto L_1b36;
    else
        goto L_1b30;

L_1b30:
    t_merge_1b39_0001 = 0x1;
    goto L_1b39;

L_1b36:
    t_merge_1b39_0001 = 0x0;

L_1b39:
    *(pfMulti) = t_merge_1b39_0001;
    if ((pdmgPeople != 0x0))
        goto L_1ba2;
    else
        goto L_1b49;

L_1b49:
    if ((*(pdmgPeople + 0x2) != 0x0))
        goto L_1ba2;
    else
        goto L_1b52;

L_1b52:
    if ((pdmgPeopleMin != 0x0))
        goto L_1ba2;
    else
        goto L_1b5d;

L_1b5d:
    if ((*(pdmgPeopleMin + 0x2) != 0x0))
        goto L_1ba2;
    else
        goto L_1b66;

L_1b66:
    if ((pdmgPeopleSmart != 0x0))
        goto L_1ba2;
    else
        goto L_1b71;

L_1b71:
    if ((*(pdmgPeopleSmart + 0x2) != 0x0))
        goto L_1ba2;
    else
        goto L_1b7a;

L_1b7a:
    if ((pdmgBldg != 0x0))
        goto L_1ba2;
    else
        goto L_1b85;

L_1b85:
    if ((*(pdmgBldg + 0x2) != 0x0))
        goto L_1ba2;
    else
        goto L_1b8e;

L_1b8e:
    if ((ppctTerra != 0x0))
        goto L_1ba2;
    else
        goto L_1b99;

L_1b99:
    if ((*(ppctTerra + 0x2) == 0x0))
        goto L_1ba8;
    else
        goto L_1ba2;

L_1ba2:
    return 0x1;

L_1ba8:
    return 0x0;
}

void LinkFleets(int16_t fUnused) {
    FLEET **pSearch;
    POINT   pt;
    FLEET  *rglpflSrc[63];
    FLEET  *lpflTail;
    FLEET  *lpflHead;
    int16_t i;
    int16_t iflTail;
    int16_t iflHead;
    int16_t cSrc;

L_1bb4:
    iflHead = 0x0;
    goto L_1bcb;

L_1bc6:
    iflHead = (iflHead + 0x1);

L_1bcb:
    if ((iflHead >= cFleet))
        goto L_1c33;
    else
        goto L_1bd7;

L_1bd7:
    lpflHead = rglpfl[iflHead];
    if ((LOWORD(rglpfl[iflHead]) != 0x0))
        goto L_1c0a;
    else
        goto L_1c02;

L_1c02:
    if ((*(rglpfl[iflHead] + 0x2) == 0x0))
        goto L_1c33;
    else
        goto L_1c0a;

L_1c0a:
    lpflHead->lpflNext = 0x0;
    lpflHead->fDone = 0x0;
    goto L_1bc6;

L_1c33:
    cSrc = 0x0;
    iflHead = 0x0;
    goto L_1eee;

L_1c42:
    lpflHead = rglpfl[iflHead];
    if ((lpflHead->fDead != 0x0))
        goto L_1ee9;
    else
        goto L_1c7d;

L_1c7d:
    if ((LOWORD(lpflHead->lpflNext) != 0x0))
        goto L_1ee9;
    else
        goto L_1c8b;

L_1c8b:
    if ((HIWORD(lpflHead->lpflNext) != 0x0))
        goto L_1ee9;
    else
        goto L_1c92;

L_1c92:

L_1c98:
    i = 0x0;
    goto L_1d54;

L_1ca1:
    if ((lpflHead->pt.y > rglpflSrc[i]->pt.y))
        goto L_1d4f;
    else
        goto L_1cc6;

L_1cc6:
    if ((lpflHead->pt.y < rglpflSrc[i]->pt.y))
        goto L_1cd4;
    else
        goto L_1ccb;

L_1ccb:
    if ((lpflHead->pt.x > rglpflSrc[i]->pt.x))
        goto L_1d4f;
    else
        goto L_1cd4;

L_1cd4:
    if ((lpflHead->pt.x != rglpflSrc[i]->pt.x))
        goto L_1d61;
    else
        goto L_1cf9;

L_1cf9:
    if ((lpflHead->pt.y != rglpflSrc[i]->pt.y))
        goto L_1d61;
    else
        goto L_1d02;

L_1d02:
    lpflHead->lpflNext = rglpflSrc[i]->lpflNext;
    rglpflSrc[i]->lpflNext = lpflHead;
    i = 0xffff;

L_1d4f:
    i = (i + 0x1);

L_1d54:
    if ((i < cSrc))
        goto L_1ca1;
    else
        goto L_1d61;

L_1d61:
    if ((i == 0xffff))
        goto L_1ee9;
    else
        goto L_1d68;

L_1d68:

L_1d6e:
    lpflHead->lpflNext = lpflHead;
    if ((i >= cSrc))
        goto L_1dc5;
    else
        goto L_1d8f;

L_1d8f:
    memmove(rglpflSrc[(i + 0x1)], rglpflSrc[i], ((cSrc - i) * 0x4));

L_1dc5:
    rglpflSrc[i] = lpflHead;
    cSrc = (cSrc + 0x1);
    if ((cSrc < 0x3f))
        goto L_1ee9;
    else
        goto L_1dec;

L_1dec:

L_1df2:
    iflTail = (iflHead + 0x1);
    goto L_1ed7;

L_1e00:
    lpflTail = rglpfl[iflTail];
    if ((lpflTail->fDead != 0x0))
        goto L_1ed2;
    else
        goto L_1e3b;

L_1e3b:
    if ((LOWORD(lpflTail->lpflNext) != 0x0))
        goto L_1ed2;
    else
        goto L_1e49;

L_1e49:
    if ((HIWORD(lpflTail->lpflNext) != 0x0))
        goto L_1ed2;
    else
        goto L_1e50;

L_1e50:

L_1e56:
    pt.x = lpflTail->pt.x;
    pt.y = lpflTail->pt.y;
    pSearch = bsearch(pt, rglpflSrc, cSrc, 0x4, ICompFleetPoint2);
    if ((pSearch == 0x0))
        goto L_1ed2;
    else
        goto L_1e96;

L_1e96:
    lpflHead = farptr(*(pSearch + 0x2), pSearch);
    lpflTail->lpflNext = lpflHead->lpflNext;
    lpflHead->lpflNext = lpflTail;

L_1ed2:
    iflTail = (iflTail + 0x1);

L_1ed7:
    if ((iflTail < cFleet))
        goto L_1e00;
    else
        goto L_1ee3;

L_1ee3:
    cSrc = 0x0;

L_1ee9:
    iflHead = (iflHead + 0x1);

L_1eee:
    if ((iflHead < cFleet))
        goto L_1c42;
    else
        goto L_1efa;

L_1efa:
    gd.fFleetLinkValid = 0x1;
    return;
}

int16_t ICompFleetPoint(void *arg1, void *arg2) {
    int32_t l2;
    int32_t l1;

L_1f0c:
    LOWORD(l1) = *(arg1 + 0x8);
    HIWORD(l1) = *(arg1 + 0xa);
    LOWORD(l2) = *(arg2 + 0x8);
    HIWORD(l2) = *(arg2 + 0xa);
    l1 = (l1 - l2);
    if ((HIWORD(l1) > 0x0))
        goto L_1f71;
    else
        goto L_1f56;

L_1f56:
    if ((HIWORD(l1) < 0x0))
        goto L_1f64;
    else
        goto L_1f5b;

L_1f5b:
    if ((LOWORD(l1) >= 0x0))
        goto L_1f71;
    else
        goto L_1f64;

L_1f64:
    l1 = 0xffffffff;
    goto L_1f92;

L_1f71:
    if ((HIWORD(l1) < 0x0))
        goto L_1f92;
    else
        goto L_1f7a;

L_1f7a:
    if ((HIWORD(l1) > 0x0))
        goto L_1f88;
    else
        goto L_1f7f;

L_1f7f:
    if ((LOWORD(l1) <= 0x0))
        goto L_1f92;
    else
        goto L_1f88;

L_1f88:
    l1 = 0x1;

L_1f92:

L_1f9b:
    return LOWORD(l1);
}

int16_t ICompFleetPoint2(void *arg1, void *arg2) {
    int32_t l2;
    int32_t l1;

L_1fa2:
    LOWORD(l1) = arg1;
    HIWORD(l1) = *(arg1 + 0x2);
    LOWORD(l2) = *(arg2 + 0x8);
    HIWORD(l2) = *(arg2 + 0xa);
    l1 = (l1 - l2);
    if ((HIWORD(l1) > 0x0))
        goto L_2002;
    else
        goto L_1fe7;

L_1fe7:
    if ((HIWORD(l1) < 0x0))
        goto L_1ff5;
    else
        goto L_1fec;

L_1fec:
    if ((LOWORD(l1) >= 0x0))
        goto L_2002;
    else
        goto L_1ff5;

L_1ff5:
    l1 = 0xffffffff;
    goto L_2023;

L_2002:
    if ((HIWORD(l1) < 0x0))
        goto L_2023;
    else
        goto L_200b;

L_200b:
    if ((HIWORD(l1) > 0x0))
        goto L_2019;
    else
        goto L_2010;

L_2010:
    if ((LOWORD(l1) <= 0x0))
        goto L_2023;
    else
        goto L_2019;

L_2019:
    l1 = 0x1;

L_2023:

L_202c:
    return LOWORD(l1);
}

int16_t FLookupSelShip(FLEET *pfl) {
L_2032:
    if ((sel.scan.grobj == grobjFleet))
        goto L_204b;
    else
        goto L_2045;

L_2045:
    return 0x0;

L_204b:
    return FLookupFleet(LOWORD(rglpfl[sel.scan.ifl]), pfl);
}

FLEET *LpflFromId(int16_t idFleet) {
    int16_t iplr;
    int16_t idGuess;
    int16_t iLo;
    int16_t iGuess;
    int16_t i;
    FLEET  *lpfl;
    int16_t iplrCur;
    int16_t iHi;
    FLEET  *t_merge_215a_0001_wide;

L_2078:
    i = 0x0;
    iplr = ((idFleet >> 0x9) & 0xf);
    idFleet = ((idFleet & 0x1fff) | 0x0);
    iplrCur = 0x0;
    goto L_20c0;

L_20a8:
    /* untranslated: i = (i + (part[4:2](rgplr[iplrCur]) & 0xfff)) */
    iplrCur = (iplrCur + 0x1);

L_20c0:
    if ((iplrCur < iplr))
        goto L_20a8;
    else
        goto L_20cb;

L_20cb:
    iLo = (i + 0xffff);
    iHi = cFleet;

L_20da:
    if (((iLo + 0x1) >= iHi))
        goto L_2151;
    else
        goto L_20e8;

L_20e8:
    iGuess = ((iLo + iHi) >> 0x1);
    lpfl = rglpfl[iGuess];
    idGuess = LOWORD(rglpfl[iGuess]);
    if ((idGuess >= idFleet))
        goto L_2131;
    else
        goto L_2128;

L_2128:
    iLo = iGuess;
    goto L_20da;

L_2131:
    if ((idGuess <= idFleet))
        goto L_2145;
    else
        goto L_213c;

L_213c:
    iHi = iGuess;
    goto L_20da;

L_2145:
    t_merge_215a_0001_wide = lpfl;
    goto L_215a;

L_2151:
    t_merge_215a_0001_wide = 0x0;

L_215a:
    return t_merge_215a_0001_wide;
}

int16_t FLookupFleet(int16_t idFleet, FLEET *pfl) {
    FLEET  *lpfl;
    int16_t fWrite;

L_2160:
    fWrite = 0x0;
    if ((cFleet > 0x0))
        goto L_217e;
    else
        goto L_2178;

L_2178:
    return 0x0;

L_217e:
    if ((idFleet >= 0x0))
        goto L_2194;
    else
        goto L_2187;

L_2187:
    idFleet = pfl;
    fWrite = 0x1;

L_2194:
    lpfl = LpflFromId(idFleet);
    if ((LOWORD(lpfl) != 0x0))
        goto L_21b7;
    else
        goto L_21ae;

L_21ae:
    if ((HIWORD(lpfl) == 0x0))
        goto L_230e;
    else
        goto L_21b7;

L_21b7:
    if ((pfl == 0x0))
        goto L_230e;
    else
        goto L_21c0;

L_21c0:
    if ((fWrite == 0x0))
        goto L_22d9;
    else
        goto L_21c9;

L_21c9:
    InvalidateReport(0x1, 0x0);
    LogChangeFleet(lpfl, pfl);
    if ((LOWORD(lpfl->lpplord) != LOWORD(pfl->lpplord)))
        goto L_2208;
    else
        goto L_21ff;

L_21ff:
    if ((HIWORD(lpfl->lpplord) == HIWORD(pfl->lpplord)))
        goto L_229b;
    else
        goto L_2208;

L_2208:
    if ((lpfl->lpplord->iordMax >= pfl->cord))
        goto L_2249;
    else
        goto L_2221;

L_2221:
    lpfl->lpplord = LpplReAlloc(lpfl->lpplord, (pfl->cord + 0x3));

L_2249:
    fmemcpy(&(lpfl->lpplord->rgord), &(pfl->lpplord->rgord), LOWORD((pfl->lpplord->iordMac * 0x12)));
    lpfl->lpplord->iordMac = pfl->lpplord->iordMac;

L_229b:
    fmemcpy(&(lpfl), &(pfl), 0x64);
    if ((gd.fTutorial == 0x0))
        goto L_230e;
    else
        goto L_22c7;

L_22c7:
    if ((idPlayer != 0x0))
        goto L_230e;
    else
        goto L_22d1;

L_22d1:
    AdvanceTutor();

L_22d9:
    if ((pfl != 0x4972))
        goto L_22f7;
    else
        goto L_22e3;

L_22e3:
    FDupFleet(lpfl, pfl);
    goto L_230e;

L_22f7:
    *(pfl) = *(lpfl);

L_230e:
    if ((LOWORD(lpfl) != 0x0))
        goto L_2320;
    else
        goto L_2317;

L_2317:
    if ((HIWORD(lpfl) == 0x0))
        goto L_2326;
    else
        goto L_2320;

L_2320:
    return 0x1;

L_2326:
    return 0x0;
}

int16_t FDupFleet(FLEET *lpfl, FLEET *pfl) {
    PLORD *lpplordT;

L_2332:
    lpplordT = pfl->lpplord;
    *(pfl) = *(lpfl);
    if ((LOWORD(lpfl->lpplord) != 0x0))
        goto L_239e;
    else
        goto L_236e;

L_236e:
    if ((HIWORD(lpfl->lpplord) != 0x0))
        goto L_239e;
    else
        goto L_2378;

L_2378:
    if ((LOWORD(lpplordT) != 0x0))
        goto L_238a;
    else
        goto L_2381;

L_2381:
    if ((HIWORD(lpplordT) == 0x0))
        goto L_2398;
    else
        goto L_238a;

L_238a:
    FreePl(lpplordT);

L_2398:
    return 0x1;

L_239e:
    pfl->lpplord = lpplordT;
    if ((LOWORD(pfl->lpplord) != 0x0))
        goto L_23ed;
    else
        goto L_23b9;

L_23b9:
    if ((HIWORD(pfl->lpplord) != 0x0))
        goto L_23ed;
    else
        goto L_23c2;

L_23c2:
    pfl->lpplord = LpplAlloc(0x12, lpfl->lpplord->iordMax, htOrd);
    goto L_2440;

L_23ed:
    if ((pfl->lpplord->iordMax >= lpfl->lpplord->iordMac))
        goto L_2440;
    else
        goto L_2417;

L_2417:
    pfl->lpplord = LpplReAlloc(pfl->lpplord, lpfl->lpplord->iordMax);

L_2440:
    fmemcpy(&(pfl->lpplord->rgord), &(lpfl->lpplord->rgord), LOWORD((lpfl->lpplord->iordMac * 0x12)));
    pfl->lpplord->iordMac = lpfl->lpplord->iordMac;
    return 0x1;
}

int16_t FLookupObject(GrobjClass grobj, int16_t id, void *pobj) {
L_24a0:
    if ((grobj != grobjFleet))
        goto L_24c6;
    else
        goto L_24b2;

L_24b2:
    return FLookupFleet(id, pobj);

L_24c6:
    if ((grobj != grobjPlanet))
        goto L_24e3;
    else
        goto L_24cf;

L_24cf:
    return FLookupPlanet(id, pobj);

L_24e3:
    return FLookupThing(id, pobj);
}

int16_t FLookupOrbitingXfer(int16_t idPlanet, int16_t iNth, XFER *pxf, int16_t idSkip) {
    int16_t i;
    THING  *lpth;
    FLEET  *lpfl;
    THING  *lpthMac;
    int16_t t_25ad;
    int16_t t_266a;

L_24fa:
    if ((cFleet > 0x0))
        goto L_2513;
    else
        goto L_250d;

L_250d:
    return 0x0;

L_2513:
    if ((cFleet == 0x0))
        goto L_2606;
    else
        goto L_251d;

L_251d:
    i = 0x0;
    goto L_2529;

L_2525:
    i = (i + 0x1);

L_2529:
    if ((i >= cFleet))
        goto L_2606;
    else
        goto L_2534;

L_2534:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_2564;
    else
        goto L_255c;

L_255c:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_2606;
    else
        goto L_2564;

L_2564:
    if ((lpfl->idPlanet != idPlanet))
        goto L_2525;
    else
        goto L_2573;

L_2573:
    if ((LOWORD(lpfl) == idSkip))
        goto L_2525;
    else
        goto L_2581;

L_2581:
    if ((idSkip == 0xffff))
        goto L_25ad;
    else
        goto L_258a;

L_258a:
    if ((lpfl->pt.x != sel.pt.x))
        goto L_2525;
    else
        goto L_25a4;

L_25a4:
    if ((lpfl->pt.y != sel.pt.y))
        goto L_2525;
    else
        goto L_25ad;

L_25ad:
    t_25ad = iNth;
    iNth = (iNth - 0x1);
    if ((t_25ad != 0x0))
        goto L_2525;
    else
        goto L_25bc;

L_25bc:
    if ((pxf == 0x0))
        goto L_25fd;
    else
        goto L_25c5;

L_25c5:
    pxf->fl = *(lpfl);
    pxf->grobj = grobjFleet;
    pxf->id = LOWORD(lpfl);

L_25fd:
    return 0x1;

L_2606:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_26c4;

L_2631:
    if ((lpth->ith != ithMineralPacket))
        goto L_26c0;
    else
        goto L_2647;

L_2647:
    if ((lpth->pt.x != sel.pt.x))
        goto L_26c0;
    else
        goto L_2661;

L_2661:
    if ((lpth->pt.y != sel.pt.y))
        goto L_26c0;
    else
        goto L_266a;

L_266a:
    t_266a = iNth;
    iNth = (iNth - 0x1);
    if ((t_266a != 0x0))
        goto L_26c0;
    else
        goto L_2679;

L_2679:
    if ((pxf == 0x0))
        goto L_26ba;
    else
        goto L_2682;

L_2682:
    pxf->th = *(lpth);
    pxf->grobj = grobjThing;
    pxf->id = LOWORD(lpth);

L_26ba:
    return 0x1;

L_26c0:
    lpth = (lpth + 0x1);

L_26c4:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_2631;
    else
        goto L_26d2;

L_26d2:
    return 0x0;
}

char *PszGetThingName(int16_t id) {
    THING *lpth;
    char   szPlr[54];

L_26de:
    lpth = LpthFromId(id);
    if ((LOWORD(lpth) != 0x0))
        goto L_2715;
    else
        goto L_2701;

L_2701:
    if ((HIWORD(lpth) != 0x0))
        goto L_2715;
    else
        goto L_270a;

L_270a:
    /* untranslated: part[0:1](szWork) = 0x0 */
    return 0x57a4;

L_2715:
    if ((lpth->ith != ithMinefield))
        goto L_27cf;
    else
        goto L_272b;

L_272b:
    if ((lpth->iplr == idPlayer))
        goto L_278c;
    else
        goto L_2744;

L_2744:
    _wsprintf(&(szPlr), "%s ", PszPlayerName(lpth->iplr, 0x0, 0x0, 0x0, 0x0, 0x0), 0x25);
    goto L_2790;

L_278c:
    /* untranslated: part[0:1](szPlr) = 0x0 */

L_2790:
    /* untranslated: call _wsprintf(szWork, PszGetCompressedString(idsSSMineField), szPlr, ss, rgszMineField[lpth->thm.iType], 0x25) -> callresult(int16_t) */
    goto L_2920;

L_27cf:
    if ((lpth->ith != ithMineralPacket))
        goto L_28a3;
    else
        goto L_27e5;

L_27e5:
    if ((((LOWORD(lpth->thp) >> 0xa) & 0xf) != 0x0))
        goto L_2815;
    else
        goto L_27fc;

L_27fc:
    CchGetString(idsSalvage, 0x57a4);
    return 0x57a4;

L_2815:
    if ((lpth->iplr == idPlayer))
        goto L_2876;
    else
        goto L_282e;

L_282e:
    _wsprintf(&(szPlr), "%s ", PszPlayerName(lpth->iplr, 0x0, 0x0, 0x0, 0x0, 0x0), 0x25);
    goto L_287a;

L_2876:
    /* untranslated: part[0:1](szPlr) = 0x0 */

L_287a:
    /* untranslated: call _wsprintf(szWork, PszGetCompressedString(idsSmineralPacket), szPlr, ss) -> callresult(int16_t) */
    goto L_2920;

L_28a3:
    if ((lpth->ith != ithWormhole))
        goto L_28d5;
    else
        goto L_28b9;

L_28b9:
    strcpy(0x57a4, PszGetCompressedString(idsWormhole));
    goto L_2920;

L_28d5:
    if ((lpth->ith != ithMysteryTrader))
        goto L_2907;
    else
        goto L_28eb;

L_28eb:
    strcpy(0x57a4, PszGetCompressedString(idsMysteryTrader));
    goto L_2920;

L_2907:
    strcpy(0x57a4, PszGetCompressedString(idsMysteryObject));

L_2920:
    return 0x57a4;
}

char *PszGetFleetName(int16_t id) {
    int16_t cshdef;
    int16_t iplr;
    char   *lpsz;
    char    szShdef[34];
    int16_t ifl;
    FLEET  *lpfl;
    char    szPlr[34];
    int16_t ishdef;
    int16_t cch;

L_292c:
    lpfl = LpflFromId((id & 0x7fff));
    id = (id & 0x7fff);
    iplr = ((id >> 0x9) & 0xf);
    ifl = (id & 0x1ff);
    if ((iplr == idPlayer))
        goto L_29ad;
    else
        goto L_2971;

L_2971:
    _wsprintf(&(szPlr), "%s ", PszPlayerName(iplr, 0x0, 0x0, 0x0, 0x0, 0x0), 0x25);
    goto L_29b1;

L_29ad:
    /* untranslated: part[0:1](szPlr) = 0x0 */

L_29b1:
    if ((LOWORD(lpfl) != 0x0))
        goto L_29c3;
    else
        goto L_29ba;

L_29ba:
    if ((HIWORD(lpfl) == 0x0))
        goto L_2a05;
    else
        goto L_29c3;

L_29c3:
    if ((LOWORD(lpfl->lpszName) != 0x0))
        goto L_29da;
    else
        goto L_29d0;

L_29d0:
    if ((HIWORD(lpfl->lpszName) == 0x0))
        goto L_2a05;
    else
        goto L_29da;

L_29da:
    /* untranslated: call _wsprintf(szWork, "%s%s", szPlr, ss, LOWORD(lpfl->lpszName), HIWORD(lpfl->lpszName)) -> callresult(int16_t) */
    goto L_2b04;

L_2a05:
    if ((LOWORD(lpfl) != 0x0))
        goto L_2a17;
    else
        goto L_2a0e;

L_2a0e:
    if ((HIWORD(lpfl) == 0x0))
        goto L_2ac6;
    else
        goto L_2a17;

L_2a17:
    ishdef = IshdefPrimaryFromLpfl(lpfl, &(cshdef));
    if ((ishdef != 0x10))
        goto L_2a4c;
    else
        goto L_2a35;

L_2a35:
    lpsz = PszGetCompressedString(idsFleet);
    goto L_2ada;

L_2a4c:
    /* untranslated: call fstrcpy(&szShdef, &part[8:0](rglpshdef[iplr][ishdef].hul)) -> callresult(char *) */
    cch = strlen(szShdef);
    if ((cch <= 0x1c))
        goto L_2a96;
    else
        goto L_2a91;

L_2a91:
    cch = 0x1c;

L_2a96:
    if ((cshdef <= 0x1))
        goto L_2ab8;
    else
        goto L_2a9f;

L_2a9f:
    szShdef[cch] = 0x2b;
    szShdef[(cch + 0x1)] = 0x0;

L_2ab8:
    LOWORD(lpsz) = szShdef;
    /* untranslated: HIWORD(lpsz) = ss */

L_2ac6:
    lpsz = PszGetCompressedString(idsFleet);

L_2ada:
    /* untranslated: call _wsprintf(szWork, "%s%s #%d", szPlr, ss, LOWORD(lpsz), HIWORD(lpsz), (ifl + 0x1)) -> callresult(int16_t) */

L_2b04:

L_2b0a:
    return 0x57a4;
}

uint16_t WFromLpfl(FLEET *lpfl) {
    int16_t  cshdef;
    uint16_t w;
    int16_t  ishdef;

L_2b10:
    ishdef = IshdefPrimaryFromLpfl(lpfl, &(cshdef));
    w = ((ishdef << 0x9) | lpfl->ifl);
    if ((cshdef <= 0x1))
        goto L_2b52;
    else
        goto L_2b4d;

L_2b4d:
    w = (w | 0x2000);

L_2b52:

L_2b58:
    return w;
}

char *PszFleetNameFromWord(uint16_t w) {
    char   *lpsz;
    char    szShdef[34];
    int16_t ishdef;
    int16_t cch;

L_2b5e:
    ishdef = ((w >> 0x9) & 0xf);
    /* untranslated: branch ((part[123:2](rglpshdef[idPlayer][ishdef]) >> 0x9) & 0x1) == 0x0 ? L_2bba : L_2ba3 */

L_2ba3:
    lpsz = PszGetCompressedString(idsFleet);
    goto L_2c37;

L_2bba:
    /* untranslated: call fstrcpy(&szShdef, &part[8:0](rglpshdef[idPlayer][ishdef].hul)) -> callresult(char *) */
    cch = strlen(szShdef);
    if ((cch <= 0x1c))
        goto L_2c05;
    else
        goto L_2c00;

L_2c00:
    cch = 0x1c;

L_2c05:
    if (((w & 0x2000) == 0x0))
        goto L_2c2c;
    else
        goto L_2c13;

L_2c13:
    szShdef[cch] = 0x2b;
    szShdef[(cch + 0x1)] = 0x0;

L_2c2c:
    LOWORD(lpsz) = szShdef;
    /* untranslated: HIWORD(lpsz) = ss */

L_2c37:
    _wsprintf(szWork, "%s #%d", LOWORD(lpsz), HIWORD(lpsz), ((w & 0x1ff) + 0x1));

L_2c63:
    return 0x57a4;
}

char *PszGetPlanetName(int16_t id) {
    int16_t fInOrbit;
    char   *psz;

L_2c6a:
    fInOrbit = (id & 0x8000);
    id = (id & 0x7fff);
    id = rgidPlan[id];
    psz = PszGetCompressedPlanet(id);
    if ((fInOrbit == 0x0))
        goto L_2ccd;
    else
        goto L_2ca4;

L_2ca4:
    _wsprintf(szWork, PszGetCompressedString(idsOrbitingS), psz, 0x25);
    goto L_2cdc;

L_2ccd:
    strcpy(0x57a4, psz);

L_2cdc:

L_2ce2:
    return 0x57a4;
}

int16_t IflFromLpfl(FLEET *lpfl) {
    int16_t i;

L_2ce8:
    i = 0x0;
    goto L_2d2d;

L_2cf9:
    if ((LOWORD(rglpfl[i]) != LOWORD(lpfl)))
        goto L_2d29;
    else
        goto L_2d1a;

L_2d1a:
    if ((*(rglpfl[i] + 0x2) != HIWORD(lpfl)))
        goto L_2d29;
    else
        goto L_2d23;

L_2d23:
    return i;

L_2d29:
    i = (i + 0x1);

L_2d2d:
    if ((i < cFleet))
        goto L_2cf9;
    else
        goto L_2d38;

L_2d38:
    return 0xffff;
}

int16_t FDeleteFleet(int16_t idFleet, GrobjClass grobjSel, int16_t idSel) {
    int16_t i;
    FLEET  *lpfl;
    int16_t iPlr;
    int16_t idDel;
    PLANET *lppl;

L_2d44:
    i = 0x0;
    goto L_2d59;

L_2d55:
    i = (i + 0x1);

L_2d59:
    if ((i >= cFleet))
        goto L_2dbc;
    else
        goto L_2d64;

L_2d64:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_2d94;
    else
        goto L_2d8c;

L_2d8c:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_2dbc;
    else
        goto L_2d94;

L_2d94:
    if ((LOWORD(lpfl) == idFleet))
        goto L_2dbc;
    else
        goto L_2d9f;

L_2d9f:

L_2da5:
    if ((LOWORD(lpfl) <= idFleet))
        goto L_2d55;
    else
        goto L_2db3;

L_2db3:
    return 0x0;

L_2dbc:
    if ((i != cFleet))
        goto L_2dcd;
    else
        goto L_2dc7;

L_2dc7:
    return 0x0;

L_2dcd:
    if ((idFleet != sel.fl.id))
        goto L_2de8;
    else
        goto L_2dd8;

L_2dd8:
    RedrawScanSel(0x0, 0x0);

L_2de8:
    lpfl->fDead = 0x1;
    FleetOrdersChangeTarget(lpfl);
    FreePl(lpfl->lpplord);
    if ((LOWORD(lpfl->lpszName) != 0x0))
        goto L_2e34;
    else
        goto L_2e2a;

L_2e2a:
    if ((HIWORD(lpfl->lpszName) == 0x0))
        goto L_2e4b;
    else
        goto L_2e34;

L_2e34:
    FreeLp(&(lpfl->lpszName), htString);

L_2e4b:
    cFleet = (cFleet - 0x1);
    iPlr = lpfl->iPlayer;
    idDel = LOWORD(lpfl);
    /* untranslated: ss:[bp-0xe] = ((part[4:2](rgplr[iPlr]) + 0xffff) & 0xfff) */
    /* untranslated: part[4:2](rgplr[iPlr]) = (part[4:2](rgplr[iPlr]) & 0xf000) */
    /* untranslated: part[4:2](rgplr[iPlr]) = (part[4:2](rgplr[iPlr]) | ss:[bp-0xe]) */
    if ((grobjSel != grobjNone))
        goto L_2efc;
    else
        goto L_2eaa;

L_2eaa:
    if ((lpfl->idPlanet == 0xffff))
        goto L_2efc;
    else
        goto L_2eb7;

L_2eb7:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((LOWORD(lppl) != 0x0))
        goto L_2ede;
    else
        goto L_2ed5;

L_2ed5:
    if ((HIWORD(lppl) == 0x0))
        goto L_2efc;
    else
        goto L_2ede;

L_2ede:
    if ((lppl->iPlayer != idPlayer))
        goto L_2efc;
    else
        goto L_2eed;

L_2eed:
    grobjSel = grobjPlanet;
    idSel = lpfl->idPlanet;

L_2efc:
    if ((cFleet == i))
        goto L_2f44;
    else
        goto L_2f08;

L_2f08:
    fmemmove(&(rglpfl[i]), &(rglpfl[(i + 0x1)]), ((cFleet - i) * 0x4));

L_2f44:
    FreeLp(&(lpfl), htFleets);
    gd.fFleetLinkValid = 0x0;
    if ((sel.fl.id == 0xffff))
        goto L_2fda;
    else
        goto L_2f6c;

L_2f6c:
    if ((i >= sel.scan.ifl))
        goto L_2f7f;
    else
        goto L_2f77;

L_2f77:
    sel.scan.ifl = (sel.scan.ifl - 0x1);
    goto L_2fda;

L_2f7f:
    if ((idDel != sel.fl.id))
        goto L_2fda;
    else
        goto L_2f8a;

L_2f8a:
    if ((grobjSel != grobjNone))
        goto L_2faa;
    else
        goto L_2f93;

L_2f93:
    sel.grobj = grobjNone;
    sel.scan.grobj = grobjNone;
    FFindSomethingAndSelectIt();
    return 0x1;

L_2faa:
    sel.grobj = grobjNone;
    if ((grobjSel != grobjFleet))
        goto L_2fcb;
    else
        goto L_2fb9;

L_2fb9:
    SelectAdjFleet(0x0, idSel);
    goto L_2fda;

L_2fcb:
    SelectAdjPlanet(0x0, idSel);

L_2fda:
    if ((gd.fGeneratingTurn != 0x0))
        goto L_3000;
    else
        goto L_2fea;

L_2fea:
    if ((hwndMessage == 0x0))
        goto L_3000;
    else
        goto L_2ff4;

L_2ff4:
    SetMsgTitle(hwndMessage);

L_3000:
    return 0x1;
}

FLEET *LpflNew(int16_t iPlr, int16_t idPl) {
    int16_t  i;
    ORDER   *lpord;
    FLEET   *lpfl;
    int16_t  iflPrev;
    uint16_t t_merge_32dc_0001;

L_300c:
    iflPrev = 0xffff;
    i = 0x0;
    goto L_3026;

L_3022:
    i = (i + 0x1);

L_3026:
    if ((i >= cFleet))
        goto L_30ad;
    else
        goto L_3031;

L_3031:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_3061;
    else
        goto L_3059;

L_3059:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_30ad;
    else
        goto L_3061;

L_3061:
    if ((lpfl->iPlayer < iPlr))
        goto L_3022;
    else
        goto L_306d;

L_306d:

L_3073:
    if ((lpfl->iPlayer > iPlr))
        goto L_30ad;
    else
        goto L_307f;

L_307f:

L_3085:
    if ((lpfl->ifl != (iflPrev + 0x1)))
        goto L_30ad;
    else
        goto L_3098;

L_3098:

L_309e:
    iflPrev = lpfl->ifl;
    goto L_3022;

L_30ad:
    rglpfl = LpReAlloc(&(rglpfl), ((cFleet + 0x1) * 0x4), htMisc);
    if ((cFleet == i))
        goto L_311b;
    else
        goto L_30df;

L_30df:
    fmemmove(&(rglpfl[(i + 0x1)]), &(rglpfl[i]), ((cFleet - i) * 0x4));

L_311b:
    LpAlloc(0x7c, htFleets);
    /* untranslated: lpfl = callresult(void *) */
    /* untranslated: LOWORD(rglpfl[i]) = faroff(callresult(void *)) */
    /* untranslated: *(rglpfl[i]+0x2) = farseg(callresult(void *)) */
    cFleet = (cFleet + 0x1);
    /* untranslated: ss:[bp-0x10] = ((part[4:2](rgplr[iPlr]) + 0x1) & 0xfff) */
    /* untranslated: part[4:2](rgplr[iPlr]) = (part[4:2](rgplr[iPlr]) & 0xf000) */
    /* untranslated: part[4:2](rgplr[iPlr]) = (part[4:2](rgplr[iPlr]) | ss:[bp-0x10]) */
    fmemset(&(lpfl), 0x0, 0x7c);
    LOWORD(lpfl) = ((LOWORD(lpfl) & 0xfe00) | ((iflPrev + 0x1) & 0x1ff));
    lpfl->iPlayer = iPlr;
    LOWORD(lpfl) = ((LOWORD(lpfl) & 0xe1ff) | ((iPlr & 0xf) << 0x9));
    lpfl->det = 0x7;
    lpfl->idPlanet = idPl;
    if ((idPl == 0xffff))
        goto L_3231;
    else
        goto L_3217;

L_3217:
    lpfl->pt.x = rgptPlan[idPl].x;
    lpfl->pt.y = rgptPlan[idPl].y;

L_3231:
    lpfl->cord = 0x1;
    lpfl->fRepOrders = 0x0;
    lpfl->lpplord = LpplAlloc(0x12, 0x3, htOrd);
    lpfl->lpplord->iordMac = 0x1;
    lpfl->fdirValid = 0x0;
    lpord = &(lpfl->lpplord->rgord);
    lpord->pt.x = lpfl->pt.x;
    lpord->pt.y = lpfl->pt.y;
    lpord->id = lpfl->idPlanet;
    if ((lpfl->idPlanet == 0xffff))
        goto L_32d9;
    else
        goto L_32d3;

L_32d3:
    t_merge_32dc_0001 = 0x1;
    goto L_32dc;

L_32d9:
    t_merge_32dc_0001 = 0x4;

L_32dc:
    *(lpord + 0x6) = ((*(lpord + 0x6) & 0xf0ff) | ((t_merge_32dc_0001 & 0xf) << 0x8));
    lpord->iWarp = 0x0;
    lpord->fValidTask = 0x1;
    lpord->grTask = grTaskNone;
    if ((sel.scan.ifl == 0xffff))
        goto L_3356;
    else
        goto L_3346;

L_3346:
    if ((i > sel.scan.ifl))
        goto L_3356;
    else
        goto L_3351;

L_3351:
    sel.scan.ifl = (sel.scan.ifl + 0x1);

L_3356:
    gd.fFleetLinkValid = 0x0;

L_336b:
    return lpfl;
}

FLEET *LpflNewSplit(FLEET *pfl) {
    int16_t iordMac;
    FLEET  *lpflNew;

L_3372:
    lpflNew = LpflNew(pfl->iPlayer, pfl->idPlanet);
    if ((pfl->idPlanet != 0xffff))
        goto L_33b5;
    else
        goto L_33a1;

L_33a1:
    lpflNew->pt.x = pfl->pt.x;
    lpflNew->pt.y = pfl->pt.y;

L_33b5:
    *(lpflNew + 0x4) = ((*(lpflNew + 0x4) & 0xfdff) | ((pfl->fRepOrders & 0x1) << 0x9));
    *(lpflNew + 0x4) = ((*(lpflNew + 0x4) & 0xff00) | (pfl->det & 0xff));
    lpflNew->iplan = pfl->iplan;
    iordMac = pfl->lpplord->iordMac;
    if ((lpflNew->lpplord->iordMax >= iordMac))
        goto L_346e;
    else
        goto L_3442;

L_3442:
    lpflNew->lpplord = LpplReAlloc(lpflNew->lpplord, pfl->lpplord->iordMax);

L_346e:
    fmemcpy(&(lpflNew->lpplord->rgord), &(pfl->lpplord->rgord), LOWORD((iordMac * 0x12)));
    lpflNew->lpplord->iordMac = LOBYTE(iordMac);
    lpflNew->cord = pfl->cord;
    LogSplitFleet(pfl);

L_34d2:
    return lpflNew;
}

int16_t FFleetMergeAll(FLEET *pfl) {
    int16_t iplr;
    int32_t dpT;
    int16_t fCshOverflow;
    int16_t rgcshDamaged[16];
    int16_t cflMerge;
    int16_t i;
    FLEET  *lpfl;
    int16_t cshT;
    SHDEF  *lpshdef;
    FLEET  *lpflMerge;
    int32_t rgdp[16];
    int16_t j;

L_34d8:
    lpflMerge = 0x0;
    cflMerge = 0x0;
    fCshOverflow = 0x0;
    memset(rgdp, 0x0, 0x40);
    memset(rgcshDamaged, 0x0, 0x20);
    iplr = pfl->iPlayer;
    i = 0x0;
    goto L_37b1;

L_352e:
    if ((vrgiflMerge[i] == 0xffff))
        goto L_37ad;
    else
        goto L_3541;

L_3541:
    lpfl = LpflFromId(vrgiflMerge[i]);
    if ((LOWORD(lpfl) != 0x0))
        goto L_3571;
    else
        goto L_3565;

L_3565:
    if ((HIWORD(lpfl) == 0x0))
        goto L_37ad;
    else
        goto L_356b;

L_356b:

L_3571:
    lpshdef = rglpshdef[iplr];
    j = 0x0;
    goto L_35a2;

L_358e:
    j = (j + 0x1);
    /* untranslated: LOWORD(lpshdef) = part[3:0](game.szName[LOWORD(lpshdef)]) */

L_35a2:
    if ((j >= 0x10))
        goto L_3723;
    else
        goto L_35ab;

L_35ab:
    if ((lpfl->rgcsh[j] == 0x0))
        goto L_358e;
    else
        goto L_35c8;

L_35c8:
    /* untranslated: branch HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (j * 0x2))] == 0x0 ? L_369c : L_35e5 */

L_35e5:
    /* untranslated: cshT = loword((int32_t)(words(hiword((uint32_t)(sext16to32(lpfl->rgcsh[j]) * words(signhiword((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (j *
     * 0x2))] & 0x7f)), (HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (j * 0x2))] & 0x7f)))), loword((uint32_t)(sext16to32(lpfl->rgcsh[j]) *
     * words(signhiword((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (j * 0x2))] & 0x7f)), (HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (j * 0x2))] & 0x7f))))) / 0x64))
     */
    if ((cshT != 0x0))
        goto L_3641;
    else
        goto L_363c;

L_363c:
    cshT = 0x1;

L_3641:
    rgcshDamaged[j] = (rgcshDamaged[j] + cshT);
    /* untranslated: LOWORD(dpT) = loword((uint32_t)(words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (j * 0x2))] >> 0x7) & 0x1ff)) * sext16to32(cshT))) */
    /* untranslated: HIWORD(dpT) = hiword((uint32_t)(words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (j * 0x2))] >> 0x7) & 0x1ff)) * sext16to32(cshT))) */
    rgdp[j] = (rgdp[j] + dpT);

L_369c:
    if ((lpfl->ifl == pfl->ifl))
        goto L_358e;
    else
        goto L_36b5;

L_36b5:
    pfl->rgcsh[j] = (pfl->rgcsh[j] + lpfl->rgcsh[j]);
    if ((pfl->rgcsh[j] >= 0x0))
        goto L_3707;
    else
        goto L_36f4;

L_36f4:
    pfl->rgcsh[j] = 0x7ffe;

L_3707:
    lpfl->rgcsh[j] = 0x0;

L_3723:
    if ((lpfl->ifl != pfl->ifl))
        goto L_374b;
    else
        goto L_373c;

L_373c:
    lpflMerge = lpfl;
    goto L_37ad;

L_374b:
    j = 0x0;
    goto L_378a;

L_3753:
    pfl->rgwtMin[j] = (pfl->rgwtMin[j] + lpfl->rgwtMin[j]);
    j = (j + 0x1);

L_378a:
    if ((j < 0x5))
        goto L_3753;
    else
        goto L_3793;

L_3793:
    FDeleteFleet(LOWORD(lpfl), grobjNone, 0xffff);
    cflMerge = (cflMerge + 0x1);

L_37ad:
    i = (i + 0x1);

L_37b1:
    if ((i < vcflMerge))
        goto L_352e;
    else
        goto L_37bc;

L_37bc:
    lpshdef = rglpshdef[iplr];
    j = 0x0;
    goto L_37ed;

L_37d9:
    j = (j + 0x1);
    /* untranslated: LOWORD(lpshdef) = part[3:0](game.szName[LOWORD(lpshdef)]) */

L_37ed:
    if ((j >= 0x10))
        goto L_3980;
    else
        goto L_37f6;

L_37f6:
    lpflMerge->rgcsh[j] = pfl->rgcsh[j];
    if ((rgcshDamaged[j] == 0x0))
        goto L_384d;
    else
        goto L_3830;

L_3830:
    if ((lpflMerge->rgcsh[j] != 0x0))
        goto L_3869;
    else
        goto L_384d;

L_384d:
    /* untranslated: HIWORD(lpflMerge):[((LOWORD(lpflMerge) + 0x2c) + (j * 0x2))] = 0x0 */
    goto L_37d9;

L_3869:
    /* untranslated: HIWORD(lpflMerge):[((LOWORD(lpflMerge) + 0x2c) + (j * 0x2))] = ((HIWORD(lpflMerge):[((LOWORD(lpflMerge) + 0x2c) + (j * 0x2))] & 0xff80) |
     * (loword((int32_t)(words((hiword((uint32_t)(sext16to32(rgcshDamaged[j]) * 0x64)) + signhiword((lpflMerge->rgcsh[j] + 0xffff))),
     * (loword((uint32_t)(sext16to32(rgcshDamaged[j]) * 0x64)) + (lpflMerge->rgcsh[j] + 0xffff))) / sext16to32(lpflMerge->rgcsh[j]))) & 0x7f)) */
    /* untranslated: HIWORD(lpflMerge):[((LOWORD(lpflMerge) + 0x2c) + (j * 0x2))] = ((HIWORD(lpflMerge):[((LOWORD(lpflMerge) + 0x2c) + (j * 0x2))] & 0x7f) |
     * ((loword((int32_t)(rgdp[j] / sext16to32(rgcshDamaged[j]))) & 0x1ff) << 0x7)) */

L_3980:
    j = 0x0;
    goto L_39bf;

L_3988:
    lpflMerge->rgwtMin[j] = pfl->rgwtMin[j];
    j = (j + 0x1);

L_39bf:
    if ((j < 0x5))
        goto L_3988;
    else
        goto L_39c8;

L_39c8:
    LogMergeFleet(pfl);
    InvalidateReport(0x1, 0x2);
    if ((cflMerge == 0x0))
        goto L_39f4;
    else
        goto L_39ee;

L_39ee:
    return 0x1;

L_39f4:
    return 0x0;
}

int16_t FFleetSplitAll(FLEET *pfl) {
    FLEET   flNew;
    int16_t cSplit;
    int16_t c;
    int16_t i;
    FLEET  *lpflNew;
    int16_t t_3a2d;
    int16_t t_3a3e;

L_3a00:
    cSplit = 0x0;
    i = 0x0;
    goto L_3ad3;

L_3a17:
    c = pfl->rgcsh[i];

L_3a2d:
    t_3a2d = c;
    c = (c - 0x1);
    if ((t_3a2d == 0x0))
        goto L_3ace;
    else
        goto L_3a3e;

L_3a3e:
    t_3a3e = cSplit;
    cSplit = (cSplit + 0x1);
    if ((t_3a3e == 0x0))
        goto L_3a2d;
    else
        goto L_3a4a;

L_3a4a:

L_3a50:
    lpflNew = LpflNewSplit(pfl);
    flNew = *(lpflNew);
    pfl->rgcsh[i] = (pfl->rgcsh[i] - 0x1);
    flNew.rgcsh[i] = (flNew.rgcsh[i] + 0x1);
    FleetTransferCargoBalance(pfl, &(flNew));
    FLookupFleet(0xffff, pfl);
    FLookupFleet(0xffff, &(flNew));
    goto L_3a2d;

L_3ace:
    i = (i + 0x1);

L_3ad3:
    if ((i < 0x10))
        goto L_3a17;
    else
        goto L_3add;

L_3add:
    InvalidateReport(0x1, 0x2);
    if ((cSplit <= 0x1))
        goto L_3afc;
    else
        goto L_3af6;

L_3af6:
    return 0x1;

L_3afc:
    return 0x0;
}

char *PszGetLocName(GrobjClass grobj, int16_t id, int16_t x, int16_t y) {
L_3b08:
    if ((id == 0xffff))
        goto NoObj;
    else
        goto L_3b17;

L_3b17:

L_3b1d:
    if ((grobj != grobjPlanet))
        goto L_3b37;
    else
        goto L_3b26;

L_3b26:
    return PszGetPlanetName(id);

L_3b37:
    if ((grobj != grobjFleet))
        goto L_3b51;
    else
        goto L_3b40;

L_3b40:
    return PszGetFleetName(id);

L_3b51:
    if ((grobj != grobjThing))
        goto NoObj;
    else
        goto L_3b5a;

L_3b5a:
    return PszGetThingName(id);

NoObj:
    if ((x != 0xffff))
        goto L_3b96;
    else
        goto L_3b71;

L_3b71:
    if ((y != 0xffff))
        goto L_3b96;
    else
        goto L_3b7a;

L_3b7a:
    strcpy(0x57a4, PszGetCompressedString(idsDeepSpace));
    goto L_3bbb;

L_3b96:
    _wsprintf(szWork, PszGetCompressedString(idsSpaceDD), x, y);

L_3bbb:
    return 0x57a4;
}

int16_t CchGetETA(HDC hdc, FLEET *lpfl, char *sz, int16_t iwp, int16_t fSmall) {
    int16_t  iWarp;
    double   dbl;
    ORDER   *lpord;
    int16_t  i;
    int16_t  c;
    int16_t  iSpeed;
    int16_t  j;
    int16_t  cYears;
    StringId ids;
    uint16_t t_merge_3dcf_0001;

L_3bc8:
    cYears = 0x0;
    lpord = &(lpfl->lpplord->rgord);
    i = 0x0;
    goto L_3c07;

L_3bf4:
    i = (i + 0x1);
    lpord = (lpord + 0x1);

L_3c07:
    if ((i >= iwp))
        goto L_3dba;
    else
        goto L_3c12;

L_3c12:
    dbl = DGetDistance(lpord->pt.x, lpord->pt.y, lpord[0x1].pt.x, *(lpord + 0x14));
    iWarp = ((*(lpord + 0x18) >> 0x4) & 0xf);
    if ((iWarp >= 0xb))
        goto L_3c6a;
    else
        goto L_3c5e;

L_3c5e:
    iSpeed = LOWORD((iWarp * iWarp));
    goto L_3cde;

L_3c6a:
    /* untranslated: j = FCanFleetUseStargates(lpfl, words(lpord->pt.y, lpord->pt.x), words(*(lpord+0x14), lpord[0x1].pt.x)) */
    if ((j != 0xffff))
        goto L_3ca1;
    else
        goto L_3c99;

L_3c99:
    iSpeed = 0xfffd;
    goto L_3cde;

L_3ca1:
    if ((j != 0x0))
        goto L_3cb2;
    else
        goto L_3caa;

L_3caa:
    iSpeed = 0x0;
    goto L_3cde;

L_3cb2:
    if ((j != 0x1))
        goto L_3cc3;
    else
        goto L_3cbb;

L_3cbb:
    iSpeed = 0x1f40;
    goto L_3cde;

L_3cc3:
    if (((j & 0x2) == 0x0))
        goto L_3cd9;
    else
        goto L_3cd1;

L_3cd1:
    iSpeed = 0xffff;
    goto L_3cde;

L_3cd9:
    iSpeed = 0xfffe;

L_3cde:
    if ((iSpeed != 0x0))
        goto L_3d1b;
    else
        goto L_3ce7;

L_3ce7:
    if ((hdc == 0x0))
        goto L_3d00;
    else
        goto L_3cf0;

L_3cf0:
    SetTextColor(hdc, 0xff);

L_3d00:
    c = CchGetString(idsNever, sz);
    return c;

L_3d1b:
    if ((iSpeed >= 0x0))
        goto L_3d7e;
    else
        goto L_3d24;

L_3d24:
    if ((hdc == 0x0))
        goto L_3d3d;
    else
        goto L_3d2d;

L_3d2d:
    SetTextColor(hdc, 0x7f7f);

L_3d3d:
    if ((iSpeed != 0xffff))
        goto L_3d4e;
    else
        goto L_3d46;

L_3d46:
    ids = idsDanger;
    goto L_3d64;

L_3d4e:
    if ((iSpeed != 0xfffe))
        goto L_3d5f;
    else
        goto L_3d57;

L_3d57:
    ids = idsUnload2;
    goto L_3d64;

L_3d5f:
    ids = idsUncertain;

L_3d64:
    c = CchGetString(ids, sz);
    return c;

L_3d7e:
    if ((iSpeed < LOWORD(__ftol())))
        goto L_3d99;
    else
        goto L_3d91;

L_3d91:
    iSpeed = 0x1;
    goto L_3db1;

L_3d99:
    iSpeed = ((uint32_t)(((LOWORD(__ftol()) + iSpeed) + 0xffff)) / iSpeed);

L_3db1:
    cYears = (cYears + iSpeed);
    goto L_3bf4;

L_3dba:
    if ((fSmall == 0x0))
        goto L_3dcc;
    else
        goto L_3dc6;

L_3dc6:
    t_merge_3dcf_0001 = 0x49e;
    goto L_3dcf;

L_3dcc:
    t_merge_3dcf_0001 = 0x331;

L_3dcf:
    c = _wsprintf(sz, PszGetCompressedString(t_merge_3dcf_0001), cYears);
    if ((cYears == 0x1))
        goto L_3e0f;
    else
        goto L_3df7;

L_3df7:
    if ((fSmall != 0x0))
        goto L_3e0f;
    else
        goto L_3e00;

L_3e00:
    c = (c + 0x1);
    sz[c] = 0x73;

L_3e0f:
    return c;
}

int16_t IshdefPrimaryFromLpfl(FLEET *lpfl, int16_t *pcDiff) {
    int16_t cDiff;
    int16_t ish;
    int16_t i;
    int16_t csh;
    int16_t ihul;

L_3e1c:
    cDiff = 0x0;
    csh = 0x0;
    ish = 0x10;
    i = 0x0;
    goto L_3eda;

L_3e3d:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_3ed6;
    else
        goto L_3e5a;

L_3e5a:
    cDiff = (cDiff + 0x1);
    if ((lpfl->rgcsh[i] <= csh))
        goto L_3ed6;
    else
        goto L_3e7d;

L_3e7d:
    ish = i;
    csh = lpfl->rgcsh[i];
    ihul = rglpshdef[lpfl->iPlayer][i].hul.ihuldef;
    if ((ihul == 0x19))
        goto L_3ed2;
    else
        goto L_3ec9;

L_3ec9:
    if ((ihul != 0x1a))
        goto L_3ed6;
    else
        goto L_3ed2;

L_3ed2:
    csh = (csh - 0x1);

L_3ed6:
    i = (i + 0x1);

L_3eda:
    if ((i < 0x10))
        goto L_3e3d;
    else
        goto L_3ee3;

L_3ee3:
    if ((pcDiff == 0x0))
        goto L_3ef4;
    else
        goto L_3eec;

L_3eec:
    *(pcDiff) = cDiff;

L_3ef4:

L_3efa:
    return ish;
}

char *PszGetDistance(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    int32_t d;
    int16_t fStarted;
    int32_t d2;

L_3f00:
    fStarted = 0x0;
    DGetDistance(x1, y1, x2, y2);
    d = __ftol();
    d2 = (int32_t)((d / 0x64));
    d = (d - (uint32_t)((d2 * 0x64)));
    if ((dyArial8 > 0xe))
        goto L_3fac;
    else
        goto L_3f7e;

L_3f7e:
    _wsprintf(szWork, PszGetCompressedString(idsLdLdLightYears), LOWORD(d2), HIWORD(d2), LOWORD(d), HIWORD(d));
    goto L_3fd7;

L_3fac:
    _wsprintf(szWork, PszGetCompressedString(idsLdLdLY), LOWORD(d2), HIWORD(d2), LOWORD(d), HIWORD(d));

L_3fd7:

L_3fdd:
    return 0x57a4;
}

double DGetDistance(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    int32_t dy;
    int32_t dx;
    int32_t l;

L_3fe4:
    dx = (uint32_t)((x2 - x1));
    dy = (uint32_t)((y2 - y1));
    l = ((uint32_t)((dx * dx)) + (uint32_t)((dy * dy)));
    __fac = sqrt((double)(l));

L_4069:
    /* untranslated: return callresult(double) */
}

int16_t FFindNearestObject(POINT pt, GrobjClass grobj, SCAN *pscan) {
    POINT   ptWp;
    POINT  *ppt;
    int16_t dy;
    int32_t lTry;
    THING  *lpth;
    FLEET  *lpfl;
    int16_t i;
    THING  *lpthMac;
    int32_t lSquare;
    SCAN    scanT;
    int16_t iNearest;
    int16_t dx;
    SCAN    scan;

L_4070:
    iNearest = 0xffff;
    lSquare = 0x3b9aca00;
    ppt = 0x2f40;
    scan.grobjFull = grobjNone;
    scan.grobj = grobjNone;
    scan.pt.y = 0x0;
    scan.pt.x = 0x0;
    scan.ith = 0xffff;
    scan.iwp = 0xffff;
    scan.ifl = 0xffff;
    scan.idpl = 0xffff;
    if (((grobj & 0x40) == 0x0))
        goto L_40ef;
    else
        goto L_40c2;

L_40c2:
    lSquare = (uint32_t)(ScanToPt(0x14));
    lSquare = (uint32_t)((lSquare * lSquare));
    goto L_4107;

L_40ef:
    if (((grobj & 0x80) == 0x0))
        goto L_4107;
    else
        goto L_40fd;

L_40fd:
    lSquare = 0x0;

L_4107:
    if (((grobj & 0x1) == 0x0))
        goto L_4207;
    else
        goto L_4115;

L_4115:
    i = 0x0;
    goto L_412b;

L_411d:
    i = (i + 0x1);
    ppt = (ppt + 0x4);

L_412b:
    if ((i >= game.cPlanMax))
        goto L_4207;
    else
        goto L_4136;

L_4136:
    dx = (pt.x - ppt->x);
    dy = (pt.y - ppt->y);
    lTry = (uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)));
    if ((HIWORD((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)))) > HIWORD(lSquare)))
        goto L_411d;
    else
        goto L_416c;

L_416c:
    /* untranslated: branch hiword(callresult(uint32_t)) < HIWORD(lSquare) ? L_4179 : L_4171 */

L_4171:
    /* untranslated: branch loword(callresult(uint32_t)) > LOWORD(lSquare) ? L_411d : L_4179 */

L_4179:
    lTry = (lTry + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lTry) < HIWORD(lSquare)))
        goto L_41ae;
    else
        goto L_419e;

L_419e:
    if ((HIWORD(lTry) > HIWORD(lSquare)))
        goto L_411d;
    else
        goto L_41a3;

L_41a3:
    if ((LOWORD(lTry) > LOWORD(lSquare)))
        goto L_411d;
    else
        goto L_41a8;

L_41a8:

L_41ae:
    if ((LOWORD(lSquare) != LOWORD(lTry)))
        goto L_41d9;
    else
        goto L_41bc;

L_41bc:
    if ((HIWORD(lSquare) != HIWORD(lTry)))
        goto L_41d9;
    else
        goto L_41c4;

L_41c4:
    if ((LOWORD(lSquare) != 0x0))
        goto L_411d;
    else
        goto L_41cd;

L_41cd:
    if ((HIWORD(lSquare) != 0x0))
        goto L_411d;
    else
        goto L_41d3;

L_41d3:

L_41d9:
    lSquare = lTry;
    scan.pt.x = ppt->x;
    scan.pt.y = ppt->y;
    scan.idpl = i;
    scan.grobjFull = grobjPlanet;
    scan.grobj = grobjPlanet;
    goto L_411d;

L_4207:
    if (((grobj & 0x2) == 0x0))
        goto L_43c7;
    else
        goto L_4215;

L_4215:
    i = 0x0;
    goto L_4221;

L_421d:
    i = (i + 0x1);

L_4221:
    if ((i >= cFleet))
        goto L_43c7;
    else
        goto L_422c;

L_422c:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_425c;
    else
        goto L_4254;

L_4254:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_43c7;
    else
        goto L_425c;

L_425c:
    dx = (pt.x - lpfl->pt.x);
    dy = (pt.y - lpfl->pt.y);
    lTry = (uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)));
    if ((HIWORD((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)))) > HIWORD(lSquare)))
        goto L_421d;
    else
        goto L_4295;

L_4295:
    /* untranslated: branch hiword(callresult(uint32_t)) < HIWORD(lSquare) ? L_42a2 : L_429a */

L_429a:
    /* untranslated: branch loword(callresult(uint32_t)) > LOWORD(lSquare) ? L_421d : L_42a2 */

L_42a2:
    lTry = (lTry + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lTry) < HIWORD(lSquare)))
        goto L_42d7;
    else
        goto L_42c7;

L_42c7:
    if ((HIWORD(lTry) > HIWORD(lSquare)))
        goto L_421d;
    else
        goto L_42cc;

L_42cc:
    if ((LOWORD(lTry) > LOWORD(lSquare)))
        goto L_421d;
    else
        goto L_42d1;

L_42d1:

L_42d7:
    if ((LOWORD(lTry) != LOWORD(lSquare)))
        goto L_42fb;
    else
        goto L_42e5;

L_42e5:
    if ((HIWORD(lTry) != HIWORD(lSquare)))
        goto L_42fb;
    else
        goto L_42ed;

L_42ed:
    if (((scan.grobj & 0x1) == 0x0))
        goto L_431e;
    else
        goto L_42fb;

L_42fb:
    /* untranslated: branch lpfl->pt.x != ss:[scan] ? L_4376 : L_4315 */

L_4315:
    /* untranslated: branch lpfl->pt.y != ss:[scan+0x2] ? L_4376 : L_431e */

L_431e:
    if (((scan.grobjFull & 0x2) == 0x0))
        goto L_435d;
    else
        goto L_432c;

L_432c:
    if ((rglpfl[scan.ifl]->iPlayer == idPlayer))
        goto L_421d;
    else
        goto L_434e;

L_434e:
    if ((lpfl->iPlayer != idPlayer))
        goto L_421d;
    else
        goto L_435d;

L_435d:
    scan.ifl = i;
    scan.grobjFull = (scan.grobjFull | 0x2);
    if ((scan.grobj == grobjNone))
        goto SelectShip;
    else
        goto L_436d;

L_436d:

L_4376:
    if ((HIWORD(lTry) > HIWORD(lSquare)))
        goto L_421d;
    else
        goto L_4384;

L_4384:
    if ((HIWORD(lTry) < HIWORD(lSquare)))
        goto L_4391;
    else
        goto L_4389;

L_4389:
    if ((LOWORD(lTry) >= LOWORD(lSquare)))
        goto L_421d;
    else
        goto L_4391;

L_4391:
    lSquare = lTry;

SelectShip:
    scan.pt.x = lpfl->pt.x;
    scan.pt.y = lpfl->pt.y;
    scan.ifl = i;
    scan.idpl = 0xffff;
    scan.grobjFull = grobjFleet;
    scan.grobj = grobjFleet;

L_43c7:
    if (((grobj & 0x8) == 0x0))
        goto L_4575;
    else
        goto L_43d5;

L_43d5:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_4567;

L_4400:
    dx = (pt.x - lpth->pt.x);
    dy = (pt.y - lpth->pt.y);
    lTry = (uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)));
    if ((HIWORD((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)))) > HIWORD(lSquare)))
        goto L_4563;
    else
        goto L_4439;

L_4439:
    /* untranslated: branch hiword(callresult(uint32_t)) < HIWORD(lSquare) ? L_4446 : L_443e */

L_443e:
    /* untranslated: branch loword(callresult(uint32_t)) > LOWORD(lSquare) ? L_4563 : L_4446 */

L_4446:
    lTry = (lTry + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lTry) < HIWORD(lSquare)))
        goto L_447b;
    else
        goto L_446b;

L_446b:
    if ((HIWORD(lTry) > HIWORD(lSquare)))
        goto L_4563;
    else
        goto L_4470;

L_4470:
    if ((LOWORD(lTry) > LOWORD(lSquare)))
        goto L_4563;
    else
        goto L_4475;

L_4475:

L_447b:
    if ((LOWORD(lTry) != LOWORD(lSquare)))
        goto L_449f;
    else
        goto L_4489;

L_4489:
    if ((HIWORD(lTry) != HIWORD(lSquare)))
        goto L_449f;
    else
        goto L_4491;

L_4491:
    if (((scan.grobj & 0x3) == 0x0))
        goto L_44c2;
    else
        goto L_449f;

L_449f:
    /* untranslated: branch lpth->pt.x != ss:[scan] ? L_44fc : L_44b9 */

L_44b9:
    /* untranslated: branch lpth->pt.y != ss:[scan+0x2] ? L_44fc : L_44c2 */

L_44c2:
    if (((scan.grobjFull & 0x8) != 0x0))
        goto L_4563;
    else
        goto L_44d0;

L_44d0:
    scan.ith = ((uint32_t)((LOWORD(lpth) - LOWORD(lpThings))) / 0x12);
    scan.grobjFull = (scan.grobjFull | 0x8);
    if ((scan.grobj == grobjNone))
        goto SelectThing;
    else
        goto L_44f3;

L_44f3:

L_44fc:
    if ((HIWORD(lTry) > HIWORD(lSquare)))
        goto L_4563;
    else
        goto L_450a;

L_450a:
    if ((HIWORD(lTry) < HIWORD(lSquare)))
        goto L_4517;
    else
        goto L_450f;

L_450f:
    if ((LOWORD(lTry) >= LOWORD(lSquare)))
        goto L_4563;
    else
        goto L_4517;

L_4517:
    lSquare = lTry;

SelectThing:
    scan.pt.x = lpth->pt.x;
    scan.pt.y = lpth->pt.y;
    scan.ith = ((uint32_t)((LOWORD(lpth) - LOWORD(lpThings))) / 0x12);
    scan.idpl = 0xffff;
    scan.ifl = 0xffff;
    scan.grobjFull = grobjThing;
    scan.grobj = grobjThing;

L_4563:
    lpth = (lpth + 0x1);

L_4567:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_4400;
    else
        goto L_4575;

L_4575:
    if (((grobj & 0x4) == 0x0))
        goto L_46fb;
    else
        goto L_4583;

L_4583:
    if ((sel.grobj != grobjFleet))
        goto L_46fb;
    else
        goto L_458d;

L_458d:
    i = (sel.fl.cord + 0xffff);
    goto L_46f2;

L_4599:
    ptWp.x = sel.fl.lpplord->rgord[i].pt.x;
    ptWp.y = sel.fl.lpplord->rgord[i].pt.y;
    dx = (pt.x - ptWp.x);
    dy = (pt.y - ptWp.y);
    lTry = (uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)));
    if ((HIWORD((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)))) > HIWORD(lSquare)))
        goto L_46ee;
    else
        goto L_45ee;

L_45ee:
    /* untranslated: branch hiword(callresult(uint32_t)) < HIWORD(lSquare) ? L_45fb : L_45f3 */

L_45f3:
    /* untranslated: branch loword(callresult(uint32_t)) > LOWORD(lSquare) ? L_46ee : L_45fb */

L_45fb:
    lTry = (lTry + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lTry) < HIWORD(lSquare)))
        goto L_4630;
    else
        goto L_4620;

L_4620:
    if ((HIWORD(lTry) > HIWORD(lSquare)))
        goto L_46ee;
    else
        goto L_4625;

L_4625:
    if ((LOWORD(lTry) > LOWORD(lSquare)))
        goto L_46ee;
    else
        goto L_462a;

L_462a:

L_4630:
    if ((LOWORD(lTry) != LOWORD(lSquare)))
        goto L_4654;
    else
        goto L_463e;

L_463e:
    if ((HIWORD(lTry) != HIWORD(lSquare)))
        goto L_4654;
    else
        goto L_4646;

L_4646:
    if (((scan.grobj & 0xb) == 0x0))
        goto L_466a;
    else
        goto L_4654;

L_4654:
    if ((ptWp.x != scan.pt.x))
        goto L_469c;
    else
        goto L_4662;

L_4662:
    if ((ptWp.y != scan.pt.y))
        goto L_469c;
    else
        goto L_466a;

L_466a:
    if (((scan.grobjFull & 0x4) == 0x0))
        goto L_4683;
    else
        goto L_4678;

L_4678:
    if ((i != sel.scan.iwp))
        goto L_46ee;
    else
        goto L_4683;

L_4683:
    scan.iwp = i;
    scan.grobjFull = (scan.grobjFull | 0x4);
    if ((scan.grobj == grobjNone))
        goto SelectSpace;
    else
        goto L_4693;

L_4693:

L_469c:
    if ((HIWORD(lTry) > HIWORD(lSquare)))
        goto L_46ee;
    else
        goto L_46aa;

L_46aa:
    if ((HIWORD(lTry) < HIWORD(lSquare)))
        goto L_46b7;
    else
        goto L_46af;

L_46af:
    if ((LOWORD(lTry) >= LOWORD(lSquare)))
        goto L_46ee;
    else
        goto L_46b7;

L_46b7:
    lSquare = lTry;

SelectSpace:
    scan.pt.x = ptWp.x;
    scan.pt.y = ptWp.y;
    scan.iwp = i;
    scan.ith = 0xffff;
    scan.idpl = 0xffff;
    scan.ifl = 0xffff;
    scan.grobjFull = grobjOther;
    scan.grobj = grobjOther;

L_46ee:
    i = (i - 0x1);

L_46f2:
    if ((i >= 0x0))
        goto L_4599;
    else
        goto L_46fb;

L_46fb:
    if (((grobj & 0x20) != 0x0))
        goto L_477b;
    else
        goto L_4709;

L_4709:
    if ((scan.grobj == grobjNone))
        goto L_477b;
    else
        goto L_4712;

L_4712:
    /* untranslated: branch FFindNearestObject(words(scan.pt.y, scan.pt.x), (((grobj & 0xf) ^ 0xf) | 0xa0), &scanT) == 0x0 ? L_477b : L_4739 */

L_4739:
    if ((scanT.idpl == 0xffff))
        goto L_4748;
    else
        goto L_4742;

L_4742:
    scan.idpl = scanT.idpl;

L_4748:
    if ((scanT.ifl == 0xffff))
        goto L_4757;
    else
        goto L_4751;

L_4751:
    scan.ifl = scanT.ifl;

L_4757:
    if ((scanT.iwp == 0xffff))
        goto L_4766;
    else
        goto L_4760;

L_4760:
    scan.iwp = scanT.iwp;

L_4766:
    if ((scanT.ith == 0xffff))
        goto L_4775;
    else
        goto L_476f;

L_476f:
    scan.ith = scanT.ith;

L_4775:
    scan.grobjFull = (scan.grobjFull | scanT.grobjFull);

L_477b:
    if ((pscan == 0x0))
        goto L_4794;
    else
        goto L_4784;

L_4784:
    *(pscan) = scan;

L_4794:
    if ((scan.grobj == grobjNone))
        goto L_47a3;
    else
        goto L_479d;

L_479d:
    return 0x1;

L_47a3:
    return 0x0;
}

void UpdateShdefCost(SHDEF *lpshdef) {
    int16_t  dpT;
    uint32_t wt;
    int16_t  k;
    int16_t  c;
    uint16_t rgCosts[4];
    int16_t  fWeakArmor;
    HUL     *lphul;
    uint32_t resCost;
    uint32_t rgMin[3];
    PART     part;
    uint16_t t_merge_47f5_0001;

L_47b0:
    if ((lpshdef->det != 0x7))
        goto L_47fb;
    else
        goto L_47cb;

L_47cb:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceRegeneratingShields) == 0x0))
        goto L_47f2;
    else
        goto L_47ec;

L_47ec:
    t_merge_47f5_0001 = 0x1;
    goto L_47f5;

L_47f2:
    t_merge_47f5_0001 = 0x0;

L_47f5:
    fWeakArmor = t_merge_47f5_0001;
    goto L_4800;

L_47fb:
    fWeakArmor = 0x0;

L_4800:
    lphul = LphuldefFromId(lpshdef->hul.ihuldef);
    part.hs.grhst = hstNone;
    /* untranslated: part[4:2](part) = LOWORD(lphul) */
    /* untranslated: part[6:2](part) = HIWORD(lphul) */
    GetTruePartCost(idPlayer, &(part), &(LOWORD(rgCosts)));
    c = 0x0;
    goto L_4865;

L_4841:
    LOWORD(rgMin[c]) = rgCosts[c];
    HIWORD(rgMin[c]) = 0x0;
    c = (c + 0x1);

L_4865:
    if ((c < 0x3))
        goto L_4841;
    else
        goto L_486e;

L_486e:
    /* untranslated: LOWORD(resCost) = part[6:2](rgCosts) */
    HIWORD(resCost) = 0x0;
    LOWORD(wt) = lphul->wtEmpty;
    HIWORD(wt) = 0x0;
    lpshdef->hul.dp = lphul->dp;
    lphul = lpshdef;
    c = 0x0;
    goto L_48b0;

L_48ac:
    c = (c + 0x1);

L_48b0:
    if ((c >= lphul->chs))
        goto L_4b3c;
    else
        goto L_48c2;

L_48c2:
    /* untranslated: branch ((lphul->rgTech[part[12:0](rgcrPlrHistory[c])] >> 0x8) & 0xff) <= 0x0 ? L_48ac : L_48ec */

L_48ec:
    part.hs.grhst = lphul->rghs[c].grhst;
    /* untranslated: HIWORD(part) = lphul->rgTech[part[12:0](rgcrPlrHistory[c])] */
    FLookupPart(&(part));
    GetTruePartCost(idPlayer, &(part), &(LOWORD(rgCosts)));
    k = 0x0;
    goto L_497d;

L_4937:
    /* untranslated: LOWORD(rgMin[k]) = (LOWORD(rgMin[k]) + loword((((lphul->rgTech[part[12:0](rgcrPlrHistory[c])] >> 0x8) & 0xff) * rgCosts[k]))) */
    HIWORD(rgMin[k]) = (HIWORD(rgMin[k]) + 0x0);
    k = (k + 0x1);

L_497d:
    if ((k < 0x3))
        goto L_4937;
    else
        goto L_4986;

L_4986:
    /* untranslated: LOWORD(resCost) = (LOWORD(resCost) + loword((((lphul->rgTech[part[12:0](rgcrPlrHistory[c])] >> 0x8) & 0xff) * part[6:2](rgCosts)))) */
    HIWORD(resCost) = (HIWORD(resCost) + 0x0);
    /* untranslated: LOWORD(wt) = (LOWORD(wt) + loword((((lphul->rgTech[part[12:0](rgcrPlrHistory[c])] >> 0x8) & 0xff) * part[40:2](part[4:4](part))))) */
    HIWORD(wt) = (HIWORD(wt) + 0x0);
    goto L_4b1e;

L_4a02:
    /* untranslated: dpT = loword((((lphul->rgTech[part[12:0](rgcrPlrHistory[c])] >> 0x8) & 0xff) * part[52:2](part[4:4](part)))) */
    if ((fWeakArmor == 0x0))
        goto L_4a3d;
    else
        goto L_4a37;

L_4a37:
    dpT = (dpT >> 0x1);

L_4a3d:
    lphul->dp = (lphul->dp + dpT);
    goto L_48ac;

L_4a4a:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[c])] & 0xff) == 0x3 ? L_4a94 : L_4a6f */

L_4a6f:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[c])] & 0xff) != 0x6 ? L_48ac : L_4a94 */

L_4a94:
    /* untranslated: lphul->dp = (lphul->dp + loword((((lphul->rgTech[part[12:0](rgcrPlrHistory[c])] >> 0x8) & 0xff) * 0x41))) */

L_4ac5:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[c])] & 0xff) != 0x4 ? L_48ac : L_4aea */

L_4aea:
    /* untranslated: lphul->dp = (lphul->dp + loword((((lphul->rgTech[part[12:0](rgcrPlrHistory[c])] >> 0x8) & 0xff) * 0x32))) */

L_4b1e:
    if ((lphul->rghs[c].grhst == hstShield))
        goto L_4a4a;
    else
        goto L_4b26;

L_4b26:
    if ((lphul->rghs[c].grhst == hstArmor))
        goto L_4a02;
    else
        goto L_4b2e;

L_4b2e:
    if ((lphul->rghs[c].grhst != hstSpecialM))
        goto L_48ac;
    else
        goto L_4b33;

L_4b33:

L_4b3c:
    c = 0x0;
    goto L_4b6d;

L_4b44:
    lphul->rgwtOreCost[c] = LOWORD(rgMin[c]);
    c = (c + 0x1);

L_4b6d:
    if ((c < 0x3))
        goto L_4b44;
    else
        goto L_4b76;

L_4b76:
    lphul->resCost = LOWORD(resCost);
    lphul->wtEmpty = LOWORD(wt);
    *(lpshdef + 0x87) = 0xffff;
    *(lpshdef + 0x89) = 0xffff;
    return;
}

uint16_t WPackLong(int32_t l) {
    uint16_t exp;

L_4ba2:
    exp = 0x0;

L_4bb0:
    if (((LOWORD(l) & 0xe000) != 0x0))
        goto L_4bcc;
    else
        goto L_4bc4;

L_4bc4:
    if (((HIWORD(l) & 0xffff) == 0x0))
        goto L_4be7;
    else
        goto L_4bcc;

L_4bcc:
    l = (uint32_t)((l >> 0x2));
    exp = (exp + 0x1);
    goto L_4bb0;

L_4be7:

L_4bfb:
    return ((exp << 0xd) | LOWORD(l));
}

int16_t GetPlanetScannerRange(PLANET *lppl, int16_t *pDeep) {
    int16_t iPlrSav;
    int16_t dRange;
    PART    part;

L_4c02:
    iPlrSav = idPlayer;
    idPlayer = lppl->iPlayer;
    if ((pDeep == 0x0))
        goto L_4c2b;
    else
        goto L_4c24;

L_4c24:
    *(pDeep) = 0x0;

L_4c2b:
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh))
        goto L_4d4d;
    else
        goto L_4c4f;

L_4c4f:
    /* untranslated: ss:[bp-0x16] = loword((uint32_t)(lppl->rgwtMin[0x3] * 0xa)) */
    /* untranslated: ss:[bp-0x14] = hiword((uint32_t)(lppl->rgwtMin[0x3] * 0xa)) */
    /* untranslated: call sqrt((double)dword ss:[bp-0x16]) -> callresult(double) */
    dRange = LOWORD(__ftol());
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceNoAdvScanner) == 0x0))
        goto L_4ced;
    else
        goto L_4cb5;

L_4cb5:
    dRange = LOWORD((int32_t)(((uint32_t)(((uint32_t)(dRange) * 0x584)) / 0x3e8)));
    if ((pDeep == 0x0))
        goto LFinishUp;
    else
        goto L_4ce3;

L_4ce3:
    *(pDeep) = 0x0;

L_4ced:
    if ((pDeep == 0x0))
        goto LFinishUp;
    else
        goto L_4cf6;

L_4cf6:
    if ((lppl->fStarbase == 0x0))
        goto LFinishUp;
    else
        goto L_4d0d;

L_4d0d:
    if ((rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef < ihuldefUltraStation))
        goto LFinishUp;
    else
        goto L_4d3c;

L_4d3c:
    *(pDeep) = ((uint32_t)(dRange) / 0x2);

L_4d4d:
    /* untranslated: branch (loword((uint32_t)(words(*(lppl+0x18), *(lppl+0x1a)) >> 0xc)) & 0x1f) != 0x1f ? L_4d82 : L_4d6e */

L_4d6e:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_4d82 : L_4d76 */

L_4d76:
    idPlayer = iPlrSav;
    return 0x0;

L_4d82:
    LookupBestPlanetaryScanner(&(part));
    if ((pDeep == 0x0))
        goto L_4db4;
    else
        goto L_4d97;

L_4d97:
    /* untranslated: branch part[52:2](part[4:4](part)) >= 0x0 ? L_4db4 : L_4da4 */

L_4da4:
    /* untranslated: *pDeep = ((part[52:2](part[4:4](part)) neg 0x0) >> 0x1) */

L_4db4:
    /* untranslated: dRange = abs(part[52:2](part[4:4](part))) */
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceNoAdvScanner) == 0x0))
        goto LFinishUp;
    else
        goto L_4de7;

L_4de7:
    dRange = (dRange * 0x2);

LFinishUp:
    idPlayer = iPlrSav;
    return dRange;
}

int16_t GetCachedFleetScannerRange(FLEET *lpfl, int16_t *pdPlanRange, int16_t *ppctDetect, int16_t *piSteal) {
    int16_t dT;
    int16_t dPlanRange;
    int16_t i;
    int16_t iPlr;
    int16_t dRange;
    int16_t iSteal;
    int16_t pctDetect;

L_4e02:
    iPlr = lpfl->iPlayer;
    dRange = 0xffff;
    dPlanRange = 0x0;
    iSteal = 0x0;
    pctDetect = 0x64;
    if ((gd.fGeneratingTurn != 0x0))
        goto L_4e53;
    else
        goto L_4e39;

L_4e39:
    return GetFleetScannerRange(lpfl, pdPlanRange, ppctDetect, piSteal);

L_4e53:
    i = 0x0;
    goto L_4f70;

L_4e5b:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_4f6c;
    else
        goto L_4e78;

L_4e78:
    dT = rglpshdef[iPlr][i].dScanRange;
    if ((dT == 0x7ff))
        goto L_4eb4;
    else
        goto L_4ea3;

L_4ea3:
    if ((dT <= dRange))
        goto L_4eb4;
    else
        goto L_4eae;

L_4eae:
    dRange = dT;

L_4eb4:
    if ((rglpshdef[iPlr][i].dScanRange2 <= dPlanRange))
        goto L_4efb;
    else
        goto L_4eda;

L_4eda:
    dPlanRange = rglpshdef[iPlr][i].dScanRange2;

L_4efb:
    if ((rglpshdef[iPlr][i].pctDetect >= pctDetect))
        goto L_4f48;
    else
        goto L_4f24;

L_4f24:
    pctDetect = rglpshdef[iPlr][i].pctDetect;

L_4f48:
    iSteal = (iSteal | rglpshdef[iPlr][i].iSteal);

L_4f6c:
    i = (i + 0x1);

L_4f70:
    if ((i < 0x10))
        goto L_4e5b;
    else
        goto L_4f79;

L_4f79:
    if ((pdPlanRange == 0x0))
        goto L_4f8a;
    else
        goto L_4f82;

L_4f82:
    *(pdPlanRange) = dPlanRange;

L_4f8a:
    if ((ppctDetect == 0x0))
        goto L_4f9b;
    else
        goto L_4f93;

L_4f93:
    *(ppctDetect) = pctDetect;

L_4f9b:
    if ((piSteal == 0x0))
        goto L_4fac;
    else
        goto L_4fa4;

L_4fa4:
    *(piSteal) = iSteal;

L_4fac:
    return dRange;
}

int16_t GetFleetScannerRange(FLEET *lpfl, int16_t *pdPlanRange, int16_t *ppctDetect, int16_t *piSteal) {
    int16_t iplr;
    int16_t dPlanRange;
    int16_t i;
    int16_t dRange;
    int16_t iSteal;
    int16_t dPlanRangeBest;
    int16_t dRangeBest;
    int16_t pctDetect;

L_4fb8:
    iplr = lpfl->iPlayer;
    dRange = 0xffff;
    dPlanRange = 0x0;
    iSteal = 0x0;
    if ((ppctDetect == 0x0))
        goto L_4fea;
    else
        goto L_4fe3;

L_4fe3:
    *(ppctDetect) = 0x64;

L_4fea:
    i = 0x0;
    goto L_5097;

L_4ff2:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_5093;
    else
        goto L_500f;

L_500f:
    dRangeBest = GetShdefScannerRange(&(rglpshdef[iplr][i]), iplr, &(dPlanRangeBest), &(pctDetect), &(iSteal));
    if ((ppctDetect == 0x0))
        goto L_5060;
    else
        goto L_504b;

L_504b:
    if ((pctDetect >= *(ppctDetect)))
        goto L_5060;
    else
        goto L_5058;

L_5058:
    *(ppctDetect) = pctDetect;

L_5060:
    if ((piSteal == 0x0))
        goto L_5071;
    else
        goto L_5069;

L_5069:
    *(piSteal) = (*(piSteal) | iSteal);

L_5071:
    if ((dRangeBest <= dRange))
        goto L_5082;
    else
        goto L_507c;

L_507c:
    dRange = dRangeBest;

L_5082:
    if ((dPlanRange >= dPlanRangeBest))
        goto L_5093;
    else
        goto L_508d;

L_508d:
    dPlanRange = dPlanRangeBest;

L_5093:
    i = (i + 0x1);

L_5097:
    if ((i < 0x10))
        goto L_4ff2;
    else
        goto L_50a0;

L_50a0:
    if ((pdPlanRange == 0x0))
        goto L_50b1;
    else
        goto L_50a9;

L_50a9:
    *(pdPlanRange) = dPlanRange;

L_50b1:

L_50b7:
    return dRange;
}

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
    uint16_t t_merge_5142_0001;
    uint16_t t_merge_53ea_0001;

L_50d0:
    /* untranslated: ss:[bp-0x4a] = 0x0 */
    /* untranslated: ss:[bp-0x48] = 0x0 */
    /* untranslated: lRange4 = (double)dword ss:[bp-0x4a] */
    /* untranslated: ss:[bp-0x4a] = 0x0 */
    /* untranslated: ss:[bp-0x48] = 0x0 */
    /* untranslated: lPlanRange4 = (double)dword ss:[bp-0x4a] */
    fHasScanner = 0x0;
    iSteal = 0x0;
    cDetectors = 0x0;
    if ((iplr == 0xffff))
        goto L_513f;
    else
        goto L_5119;

L_5119:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raNone))
        goto L_513f;
    else
        goto L_5139;

L_5139:
    t_merge_5142_0001 = 0x1;
    goto L_5142;

L_513f:
    t_merge_5142_0001 = 0x0;

L_5142:
    fBuiltIn = t_merge_5142_0001;
    lBIR4 = -1;
    lBIPR4 = -1;
    if ((ppctDetect == 0x0))
        goto L_516b;
    else
        goto L_5164;

L_5164:
    *(ppctDetect) = 0x64;

L_516b:
    if ((fBuiltIn == 0x0))
        goto L_5272;
    else
        goto L_5174;

L_5174:
    if ((lpshdef->hul.ihuldef == ihuldefScout))
        goto L_5198;
    else
        goto L_5180;

L_5180:
    if ((lpshdef->hul.ihuldef == ihuldefDestroyer))
        goto L_5198;
    else
        goto L_518c;

L_518c:
    if ((lpshdef->hul.ihuldef != ihuldefFrigate))
        goto L_5272;
    else
        goto L_5198;

L_5198:
    /* untranslated: ss:[bp-0x4a] = 0x0 */
    /* untranslated: ss:[bp-0x48] = 0x0 */
    __aFfcompp();
    /* untranslated: branch JNC ? L_525e : L_51b6 */

L_51b6:
    if ((game.fTutorial == 0x0))
        goto L_51e3;
    else
        goto L_51ca;

L_51ca:
    lBIR4 = 2.56e+06;
    lBIPR4 = 160000;
    goto L_525e;

L_51e3:
    /* untranslated: ss:[bp-0x4a] = loword((sext8to16(rgplr[iplr].rgTech[0x4]) * 0xa)) */
    /* untranslated: ss:[bp-0x48] = signhiword(loword((sext8to16(rgplr[iplr].rgTech[0x4]) * 0xa))) */
    /* untranslated: lBIPR4 = (double)dword ss:[bp-0x4a] */
    /* untranslated: ss:[bp-0x4a] = 0x2 */
    /* untranslated: ss:[bp-0x48] = 0x0 */
    /* untranslated: lBIR4 = (lBIPR4 * (double)dword ss:[bp-0x4a]) */
    lBIPR4 = (lBIPR4 * lBIPR4);
    lBIPR4 = (lBIPR4 * lBIPR4);
    lBIR4 = (lBIR4 * lBIR4);
    lBIR4 = (lBIR4 * lBIR4);

L_525e:
    lRange4 = lBIR4;
    lPlanRange4 = lBIPR4;

L_5272:
    /* untranslated: lphs = &part[58:0](lpshdef->hul) */
    chs = lpshdef->hul.chs;
    j = 0x0;
    goto L_52ab;

L_5298:
    j = (j + 0x1);
    lphs = (lphs + 0x1);

L_52ab:
    if ((j >= chs))
        goto L_5591;
    else
        goto L_52b6;

L_52b6:
    if ((lphs->cItem == 0x0))
        goto L_5298;
    else
        goto L_52cd;

L_52cd:
    if ((lphs->grhst != hstScanner))
        goto L_545a;
    else
        goto L_52d9;

L_52d9:
    fHasScanner = 0x1;
    iScanner = lphs->iItem;
    LpscannerFromId(lphs->iItem);
    /* untranslated: dRangeT = callresult(SCANNER *)->dRange */
    /* untranslated: ss:[bp-0x4a] = callresult(SCANNER *)->dRange */
    /* untranslated: ss:[bp-0x48] = signhiword(callresult(SCANNER *)->dRange) */
    /* untranslated: lT = (double)dword ss:[bp-0x4a] */
    lT = (lT * lT);
    lT = (lT * lT);
    /* untranslated: ss:[bp-0x4a] = lphs->cItem */
    /* untranslated: ss:[bp-0x48] = 0x0 */
    /* untranslated: lT = (lT * (double)dword ss:[bp-0x4a]) */
    lRange4 = (lRange4 + lT);
    dRangeT = LpscannerFromId(iScanner)->grfAbilities;
    if ((iScanner != 0x6))
        goto L_5390;
    else
        goto L_5385;

L_5385:
    dRangeT = 0x2d;
    goto LPlanScan;

L_5390:
    if ((iScanner != 0x5))
        goto L_53a8;
    else
        goto L_5399;

L_5399:
    dRangeT = 0x0;
    iSteal = (iSteal | 0x1);
    goto LPlanScan;

L_53a8:
    if ((iScanner != 0xe))
        goto L_53c0;
    else
        goto L_53b1;

L_53b1:
    dRangeT = 0x78;
    iSteal = (iSteal | 0x3);
    goto LPlanScan;

L_53c0:
    if ((dRangeT <= 0x0))
        goto L_5298;
    else
        goto L_53c9;

L_53c9:
    if ((dRangeT != 0x1))
        goto L_53d8;
    else
        goto L_53d2;

L_53d2:
    t_merge_53ea_0001 = 0x32;
    goto L_53ea;

L_53d8:
    if ((dRangeT != 0x2))
        goto L_53e7;
    else
        goto L_53e1;

L_53e1:
    t_merge_53ea_0001 = 0x64;
    goto L_53ea;

L_53e7:
    t_merge_53ea_0001 = 0xc8;

L_53ea:
    dRangeT = t_merge_53ea_0001;

LPlanScan:
    /* untranslated: ss:[bp-0x4a] = dRangeT */
    /* untranslated: ss:[bp-0x48] = signhiword(dRangeT) */
    /* untranslated: lT = (double)dword ss:[bp-0x4a] */
    lT = (lT * lT);
    lT = (lT * lT);
    /* untranslated: ss:[bp-0x4a] = lphs->cItem */
    /* untranslated: ss:[bp-0x48] = 0x0 */
    /* untranslated: lT = (lT * (double)dword ss:[bp-0x4a]) */
    lPlanRange4 = (lPlanRange4 + lT);

L_545a:
    if ((lphs->grhst != hstArmor))
        goto L_54f8;
    else
        goto L_5466;

L_5466:
    if ((lphs->iItem != iarmorMegaPolyShell))
        goto L_54f8;
    else
        goto L_5478;

L_5478:
    dRangeT = 0x50;
    dRangeT2 = 0x28;

LOddBallScanners:
    /* untranslated: ss:[bp-0x4a] = dRangeT */
    /* untranslated: ss:[bp-0x48] = signhiword(dRangeT) */
    /* untranslated: lT = (double)dword ss:[bp-0x4a] */
    lT = (lT * lT);
    lT = (lT * lT);
    /* untranslated: ss:[bp-0x4a] = lphs->cItem */
    /* untranslated: ss:[bp-0x48] = 0x0 */
    /* untranslated: lT = (lT * (double)dword ss:[bp-0x4a]) */
    lRange4 = (lRange4 + lT);
    dRangeT = dRangeT2;
    goto LPlanScan;

L_54f8:
    if ((lphs->grhst != hstBeam))
        goto L_5526;
    else
        goto L_5504;

L_5504:
    if ((lphs->iItem != ibeamMultiContainedMunition))
        goto L_5526;
    else
        goto L_5516;

L_5516:
    dRangeT = 0x96;
    dRangeT2 = 0x4b;
    goto LOddBallScanners;

L_5526:
    if ((lphs->grhst != hstShield))
        goto L_5554;
    else
        goto L_5532;

L_5532:
    if ((lphs->iItem != ishieldLangstonShell))
        goto L_5554;
    else
        goto L_5544;

L_5544:
    dRangeT = 0x32;
    dRangeT2 = 0x19;
    goto LOddBallScanners;

L_5554:
    if ((ppctDetect == 0x0))
        goto L_5298;
    else
        goto L_555d;

L_555d:
    if ((lphs->grhst != hstSpecialE))
        goto L_5298;
    else
        goto L_556a;

L_556a:
    if ((lphs->iItem != ispecialETachyonDetector))
        goto L_5298;
    else
        goto L_557c;

L_557c:
    cDetectors = (cDetectors + lphs->cItem);

L_5591:
    /* untranslated: ss:[bp-0x4a] = 0x0 */
    /* untranslated: ss:[bp-0x48] = 0x0 */
    __aFfcompp();
    /* untranslated: branch JA ? L_55b8 : L_55af */

L_55af:
    if ((fHasScanner == 0x0))
        goto L_5628;
    else
        goto L_55b8;

L_55b8:
    /* untranslated: call sqrt(sqrt(words(part[6:2](lRange4), part[4:2](lRange4), HIWORD(lRange4), LOWORD(lRange4)))) -> callresult(double) */
    dRange = LOWORD(__ftol());
    if ((iplr == 0xffff))
        goto L_562d;
    else
        goto L_55fd;

L_55fd:
    if ((GetRaceGrbit(rgplr[iplr], ibitRaceNoAdvScanner) == 0x0))
        goto L_562d;
    else
        goto L_561d;

L_561d:
    dRange = (dRange * 0x2);

L_5628:
    dRange = 0xffff;

L_562d:
    if ((pdPlanRange == 0x0))
        goto L_5674;
    else
        goto L_5636;

L_5636:
    /* untranslated: call sqrt(sqrt(words(part[6:2](lPlanRange4), part[4:2](lPlanRange4), HIWORD(lPlanRange4), LOWORD(lPlanRange4)))) -> callresult(double) */
    *(pdPlanRange) = LOWORD(__ftol());

L_5674:
    if ((piSteal == 0x0))
        goto L_5685;
    else
        goto L_567d;

L_567d:
    *(piSteal) = iSteal;

L_5685:
    if ((ppctDetect == 0x0))
        goto L_56ac;
    else
        goto L_568e;

L_568e:
    if ((cDetectors < 0x12))
        goto L_569c;
    else
        goto L_5697;

L_5697:
    cDetectors = 0x11;

L_569c:
    /* untranslated: *ppctDetect = byte 0x8:[cDetectors+0x50be] */

L_56ac:

L_56b2:
    return dRange;
}

int32_t LCalcFuelGainFromRamScoops(FLEET *lpfl, int16_t iWarp, int32_t dTravel) {
    int16_t  i;
    int16_t *rgiFuel;
    SHDEF   *lpshdef;
    int32_t  pct10;
    int32_t  pctShip10;

L_56b8:
    pct10 = 0x0;
    if ((iWarp <= 0xa))
        goto L_56dd;
    else
        goto L_56d4;

L_56d4:
    return 0x0;

L_56dd:
    i = 0x0;
    lpshdef = rglpshdef[lpfl->iPlayer];
    goto L_5712;

L_56fe:
    i = (i + 0x1);
    /* untranslated: LOWORD(lpshdef) = part[3:0](game.szName[LOWORD(lpshdef)]) */

L_5712:
    if ((i >= 0x10))
        goto L_5880;
    else
        goto L_571b;

L_571b:
    if ((lpfl->rgcsh[i] == 0x0))
        goto L_56fe;
    else
        goto L_5738;

L_5738:
    rgiFuel = &(*(LpengineFromId((HIWORD(lpshdef->hul.rghs[0x0]) & 0xff)) + 0x36));
    pctShip10 = 0x0;
    if ((iWarp > 0x9))
        goto L_56fe;
    else
        goto L_5766;

L_5766:

L_576c:
    if ((rgiFuel[iWarp] != 0x0))
        goto L_5852;
    else
        goto L_5784;

L_5784:
    LOWORD(pctShip10) = (LOWORD(pctShip10) + ((HIWORD(lpshdef->hul.rghs[0x0]) >> 0x8) & 0xff));
    HIWORD(pctShip10) = (HIWORD(pctShip10) + 0x0);
    if ((rgiFuel[(iWarp + 0x1)] != 0x0))
        goto L_5852;
    else
        goto L_57b7;

L_57b7:
    LOWORD(pctShip10) = (LOWORD(pctShip10) + (((HIWORD(lpshdef->hul.rghs[0x0]) >> 0x8) & 0xff) * 0x2));
    HIWORD(pctShip10) = (HIWORD(pctShip10) + 0x0);
    if ((iWarp >= 0x9))
        goto L_5852;
    else
        goto L_57da;

L_57da:
    if ((rgiFuel[(iWarp + 0x2)] != 0x0))
        goto L_5852;
    else
        goto L_57f5;

L_57f5:
    LOWORD(pctShip10) = (LOWORD(pctShip10) + LOWORD((((HIWORD(lpshdef->hul.rghs[0x0]) >> 0x8) & 0xff) * 0x3)));
    HIWORD(pctShip10) = (HIWORD(pctShip10) + 0x0);
    if ((iWarp >= 0x8))
        goto L_5852;
    else
        goto L_581b;

L_581b:
    if ((rgiFuel[(iWarp + 0x3)] != 0x0))
        goto L_5852;
    else
        goto L_5836;

L_5836:
    LOWORD(pctShip10) = (LOWORD(pctShip10) + (((HIWORD(lpshdef->hul.rghs[0x0]) >> 0x8) & 0xff) * 0x4));
    HIWORD(pctShip10) = (HIWORD(pctShip10) + 0x0);

L_5852:
    pct10 = (pct10 + (uint32_t)((pctShip10 * (uint32_t)(lpfl->rgcsh[i]))));

L_5880:
    pct10 = (uint32_t)((pct10 * dTravel));
    return pct10;
}

int32_t CalcPlayerScore(int16_t iPlr, SCORE *pscore) {
    int32_t  rgcsh[3];
    int32_t  lTemp;
    SCORE    score;
    PLANET  *lpplMac;
    PLANET  *lppl;
    int16_t  i;
    int16_t  ifl;
    FLEET   *lpfl;
    int16_t  iTech;
    int32_t  lPower;
    int16_t  rgType[16];
    uint32_t t_merge_5cf5_0001;
    uint32_t t_merge_5cf5_0002;

L_58a6:
    memset(score, 0x0, 0x14);
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_59c9;

L_58ee:
    if ((lppl->iPlayer != iPlr))
        goto L_59c5;
    else
        goto L_58fa;

L_58fa:

L_5900:
    score.cPlanet = (score.cPlanet + 0x1);
    /* untranslated: lTemp = (int32_t)(words((HIWORD(lppl->rgwtMin[0x3]) + 0x0), (LOWORD(lppl->rgwtMin[0x3]) + 0x3e7)) / 0x3e8) */
    if ((HIWORD(lTemp) < 0x0))
        goto L_594b;
    else
        goto L_5933;

L_5933:
    if ((HIWORD(lTemp) > 0x0))
        goto L_5941;
    else
        goto L_5938;

L_5938:
    if ((LOWORD(lTemp) <= 0x6))
        goto L_594b;
    else
        goto L_5941;

L_5941:
    lTemp = 0x6;

L_594b:
    LOWORD(score) = (LOWORD(score) + LOWORD(lTemp));
    HIWORD(score) = (HIWORD(score) + HIWORD(lTemp));
    if ((lppl->fStarbase == 0x0))
        goto L_59ad;
    else
        goto L_596e;

L_596e:
    if ((LphuldefFromId(rglpshdefSB[iPlr][lppl->isb].hul.ihuldef)->hul.wtCargoMax == 0x0))
        goto L_59ad;
    else
        goto L_59a9;

L_59a9:
    score.cStarbase = (score.cStarbase + 0x1);

L_59ad:
    CResourcesAtPlanet(lppl, iPlr);
    /* untranslated: part[4:2](score) = (part[4:2](score) + callresult(int16_t)) */
    /* untranslated: part[6:2](score) = (part[6:2](score) + signhiword(callresult(int16_t))) */

L_59c5:
    lppl = (lppl + 0x1);

L_59c9:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_58ee;
    else
        goto L_59d7;

L_59d7:
    /* untranslated: LOWORD(score) = (LOWORD(score) + loword((int32_t)(words(part[6:2](score), part[4:2](score)) / 0x1e))) */
    /* untranslated: HIWORD(score) = (HIWORD(score) + hiword((int32_t)(words(part[6:2](score), part[4:2](score)) / 0x1e))) */
    LOWORD(score) = (LOWORD(score) + LOWORD((0x3 * score.cStarbase)));
    HIWORD(score) = (HIWORD(score) + SIGNHIWORD(LOWORD((0x3 * score.cStarbase))));
    /* untranslated: branch (part[84:2](rgplr[iPlr]) & 0x1) != 0x0 ? L_5aa7 : L_5a16 */

L_5a16:
    i = 0x0;
    goto L_5a9e;

L_5a1e:
    iTech = (uint16_t)(rgplr[iPlr].rgTech[i]);
    score.cTechLevels = (score.cTechLevels + (uint16_t)(rgplr[iPlr].rgTech[i]));
    if ((iTech >= 0x4))
        goto L_5a52;
    else
        goto L_5a45;

L_5a45:
    LOWORD(score) = (LOWORD(score) + iTech);
    HIWORD(score) = (HIWORD(score) + SIGNHIWORD(iTech));
    goto L_5a9a;

L_5a52:
    if ((iTech >= 0x7))
        goto L_5a6d;
    else
        goto L_5a5b;

L_5a5b:
    LOWORD(score) = (LOWORD(score) + ((iTech * 0x2) + 0xfffd));
    HIWORD(score) = (HIWORD(score) + SIGNHIWORD(((iTech * 0x2) + 0xfffd)));
    goto L_5a9a;

L_5a6d:
    if ((iTech >= 0xa))
        goto L_5a89;
    else
        goto L_5a76;

L_5a76:
    LOWORD(score) = (LOWORD(score) + (LOWORD((0x3 * iTech)) + 0xfff7));
    HIWORD(score) = (HIWORD(score) + SIGNHIWORD((LOWORD((0x3 * iTech)) + 0xfff7)));
    goto L_5a9a;

L_5a89:
    LOWORD(score) = (LOWORD(score) + ((iTech * 0x4) + 0xffee));
    HIWORD(score) = (HIWORD(score) + SIGNHIWORD(((iTech * 0x4) + 0xffee)));

L_5a9a:
    i = (i + 0x1);

L_5a9e:
    if ((i < 0x6))
        goto L_5a1e;
    else
        goto L_5aa7;

L_5aa7:
    i = 0x0;
    goto L_5b77;

L_5aaf:
    /* untranslated: branch ((part[123:2](rglpshdef[iPlr][i]) >> 0x9) & 0x1) == 0x0 ? L_5aed : L_5adc */

L_5adc:
    rgType[i] = 0xffff;
    goto L_5b73;

L_5aed:
    lPower = LComputePower(&(rglpshdef[iPlr][i]));
    if ((HIWORD(lPower) > 0x0))
        goto L_5b3c;
    else
        goto L_5b1d;

L_5b1d:
    if ((HIWORD(lPower) < 0x0))
        goto L_5b2b;
    else
        goto L_5b22;

L_5b22:
    if ((LOWORD(lPower) > 0x0))
        goto L_5b3c;
    else
        goto L_5b2b;

L_5b2b:
    rgType[i] = 0x0;
    goto L_5b73;

L_5b3c:
    if ((HIWORD(lPower) > 0x0))
        goto L_5b65;
    else
        goto L_5b45;

L_5b45:
    if ((HIWORD(lPower) < 0x0))
        goto L_5b54;
    else
        goto L_5b4a;

L_5b4a:
    if ((LOWORD(lPower) >= 0x7d0))
        goto L_5b65;
    else
        goto L_5b54;

L_5b54:
    rgType[i] = 0x1;
    goto L_5b73;

L_5b65:
    rgType[i] = 0x2;

L_5b73:
    i = (i + 0x1);

L_5b77:
    if ((i < 0x10))
        goto L_5aaf;
    else
        goto L_5b80;

L_5b80:
    i = 0x0;
    goto L_5ba1;

L_5b88:
    rgcsh[i] = 0x0;
    i = (i + 0x1);

L_5ba1:
    if ((i < 0x3))
        goto L_5b88;
    else
        goto L_5baa;

L_5baa:
    ifl = 0x0;
    goto L_5bb6;

L_5bb2:
    ifl = (ifl + 0x1);

L_5bb6:
    if ((ifl >= cFleet))
        goto L_5c93;
    else
        goto L_5bc1;

L_5bc1:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_5bf1;
    else
        goto L_5be9;

L_5be9:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_5c93;
    else
        goto L_5bf1;

L_5bf1:
    if ((lpfl->iPlayer != iPlr))
        goto L_5bb2;
    else
        goto L_5c00;

L_5c00:
    if ((lpfl->fDead != 0x0))
        goto L_5bb2;
    else
        goto L_5c14;

L_5c14:

L_5c1a:
    i = 0x0;
    goto L_5c87;

L_5c22:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_5c83;
    else
        goto L_5c3f;

L_5c3f:
    if ((rgType[i] == 0xffff))
        goto L_5c83;
    else
        goto L_5c51;

L_5c51:
    rgcsh[rgType[i]] = (rgcsh[rgType[i]] + (uint32_t)(lpfl->rgcsh[i]));

L_5c83:
    i = (i + 0x1);

L_5c87:
    if ((i >= 0x10))
        goto L_5bb2;
    else
        goto L_5c8d;

L_5c8d:

L_5c93:
    /* untranslated: branch part[6:2](rgcsh) > signhiword(score.cPlanet) ? L_5cb5 : L_5c9f */

L_5c9f:
    /* untranslated: branch part[6:2](rgcsh) < signhiword(score.cPlanet) ? L_5cac : L_5ca4 */

L_5ca4:
    /* untranslated: branch part[4:2](rgcsh) >= score.cPlanet ? L_5cb5 : L_5cac */

L_5cac:
    goto L_5cb9;

L_5cb5:

L_5cb9:
    if ((HIWORD(rgcsh) > SIGNHIWORD(score.cPlanet)))
        goto L_5cf1;
    else
        goto L_5cdb;

L_5cdb:
    if ((HIWORD(rgcsh) < SIGNHIWORD(score.cPlanet)))
        goto L_5ce8;
    else
        goto L_5ce0;

L_5ce0:
    if ((LOWORD(rgcsh) >= score.cPlanet))
        goto L_5cf1;
    else
        goto L_5ce8;

L_5ce8:
    /* untranslated: t_merge_5cf5_0001 = words(HIWORD(rgcsh), LOWORD(rgcsh)) */
    /* untranslated: t_merge_5cf5_0002 = words(HIWORD(rgcsh), LOWORD(rgcsh)) */
    goto L_5cf5;

L_5cf1:
    t_merge_5cf5_0001 = (uint32_t)(score.cPlanet);
    t_merge_5cf5_0002 = (uint32_t)(score.cPlanet);

L_5cf5:
    /* untranslated: LOWORD(score) = (LOWORD(score) + (loword((int32_t)(t_merge_5cf5_0001 / 0x2)) + ss:[bp-0x60])) */
    /* untranslated: HIWORD(score) = (HIWORD(score) + (hiword((int32_t)(t_merge_5cf5_0002 / 0x2)) + ss:[bp-0x5e])) */
    /* untranslated: branch part[10:2](rgcsh) < 0x0 ? L_5d57 : L_5d15 */

L_5d15:
    /* untranslated: branch part[10:2](rgcsh) > 0x0 ? L_5d23 : L_5d1a */

L_5d1a:
    /* untranslated: branch part[8:2](rgcsh) <= 0x0 ? L_5d57 : L_5d23 */

L_5d23:
    /* untranslated: LOWORD(score) = (LOWORD(score) + loword((int32_t)((uint32_t)((int32_t)(words(part[8:2](rgcsh), part[10:2](rgcsh)) * 0x8) *
     * sext16to32(score.cPlanet)) / words((signhiword(score.cPlanet) + part[10:2](rgcsh)), (score.cPlanet + part[8:2](rgcsh)))))) */
    /* untranslated: HIWORD(score) = (HIWORD(score) + hiword((int32_t)((uint32_t)((int32_t)(words(part[8:2](rgcsh), part[10:2](rgcsh)) * 0x8) *
     * sext16to32(score.cPlanet)) / words((signhiword(score.cPlanet) + part[10:2](rgcsh)), (score.cPlanet + part[8:2](rgcsh)))))) */

L_5d57:
    i = 0x0;
    goto L_5d88;

L_5d5f:
    score.rgcsh[i] = WPackLong(rgcsh[i]);
    i = (i + 0x1);

L_5d88:
    if ((i < 0x3))
        goto L_5d5f;
    else
        goto L_5d91;

L_5d91:
    if ((pscore == 0x0))
        goto L_5daa;
    else
        goto L_5d9a;

L_5d9a:
    *(pscore) = score;

L_5daa:

L_5db3:
    /* untranslated: return words(LOWORD(score), HIWORD(score)) */
}

void GetTrueHullCost(int16_t iPlayer, HUL *lphul, uint16_t *rgCost) {
    int16_t i;

L_5dba:
    i = 0x0;
    goto L_5df2;

L_5dcb:
    rgCost[i] = lphul->rgwtOreCost[i];
    i = (i + 0x1);

L_5df2:
    if ((i < 0x3))
        goto L_5dcb;
    else
        goto L_5dfb;

L_5dfb:
    rgCost[0x3] = lphul->resCost;
    return;
}

void DecorateHullName(int16_t iplr, int16_t ish, char *psz) {
    int16_t i;
    int16_t c;
    SHDEF  *lpshdef;
    int16_t iVal;

L_5e0e:
    lpshdef = &(rglpshdef[iplr][ish]);
    if ((lpshdef->fFree == 0x0))
        goto L_5e54;
    else
        goto L_5e4b;

L_5e4b:
    *(psz) = 0x0;
    goto L_5f78;

L_5e54:
    /* untranslated: call fstrcpy(psz, &part[8:0](lpshdef->hul)) -> callresult(char *) */
    if ((iplr == idPlayer))
        goto L_5f78;
    else
        goto L_5e78;

L_5e78:

L_5e7e:
    c = 0x0;
    iVal = 0x1;
    i = 0x0;
    goto L_5f17;

L_5e90:
    if ((i == ish))
        goto L_5f13;
    else
        goto L_5e9b;

L_5e9b:
    /* untranslated: branch ((part[123:2](rglpshdef[iplr][i]) >> 0x9) & 0x1) != 0x0 ? L_5f13 : L_5ec5 */

L_5ec5:

L_5ecb:
    /* untranslated: branch fstrcmp(psz, &part[8:0](rglpshdef[iplr][i].hul)) != 0x0 ? L_5f13 : L_5f00 */

L_5f00:
    c = (c + 0x1);
    if ((i >= ish))
        goto L_5f13;
    else
        goto L_5f0f;

L_5f0f:
    iVal = (iVal + 0x1);

L_5f13:
    i = (i + 0x1);

L_5f17:
    if ((i < 0x10))
        goto L_5e90;
    else
        goto L_5f20;

L_5f20:
    if ((c == 0x0))
        goto L_5f78;
    else
        goto L_5f26;

L_5f26:

L_5f2c:
    strlen(psz);
    /* untranslated: c = callresult(uint16_t) */
    /* untranslated: psz[callresult(uint16_t)] = 0x20 */
    /* untranslated: part[1:1](psz[c]) = 0x28 */
    IntToRoman(iVal, &(psz[(c + 0x2)]));
    strcat(psz, 0x539);

L_5f78:
    return;
}

void DrawABunchOfStars(HDC hdc, RECT *prc) {
    int32_t lPixTot;
    int16_t iMax;
    int16_t dy;
    int16_t i;
    int16_t iClr;
    int16_t dx;
    RECT    rcOut;
    RECT    rc;

L_5fe2:
    rc = *(prc);
    PushRandom(0xb0011);
    InflateRect(&(rc), 0xfffd, 0xfffd);
    dx = (rc.right - rc.left);
    dy = (rc.bottom - rc.top);
    lPixTot = (uint32_t)(((uint32_t)(dx) * (uint32_t)((rc.bottom - rc.top))));
    iClr = 0x0;
    goto L_6102;

L_604c:
    /* untranslated: iMax = loword((int32_t)(lPixTot / words(0x8:[(iClr * 0x4)+0x5f80], 0x8:[(iClr * 0x4)+0x5f7e]))) */
    i = 0x0;
    goto L_60f3;

L_6073:
    rcOut.left = (Random(dx) + rc.left);
    rcOut.top = (Random(dy) + rc.top);
    rcOut.right = (rcOut.left + 0x1);
    rcOut.bottom = (rcOut.top + 0x1);
    /* untranslated: call SetBkColor(hdc, words(0x8:[(iClr * 0x4)+0x5f94], 0x8:[(iClr * 0x4)+0x5f92])) -> callresult(COLORREF) */
    ExtTextOut(hdc, 0x0, 0x0, 0x2, &(rcOut), 0x0, 0x0, 0x0);
    i = (i + 0x1);

L_60f3:
    if ((i < iMax))
        goto L_6073;
    else
        goto L_60fe;

L_60fe:
    iClr = (iClr + 0x1);

L_6102:
    if ((iClr < 0x5))
        goto L_604c;
    else
        goto L_610b;

L_610b:
    iClr = 0x0;
    goto L_626b;

L_6113:
    /* untranslated: iMax = (loword((int32_t)(lPixTot / words(0x8:[(iClr * 0x4)+0x5fa8], 0x8:[(iClr * 0x4)+0x5fa6]))) + 0x1) */
    i = 0x0;
    goto L_625c;

L_613d:
    rcOut.left = (Random(dx) + rc.left);
    rcOut.top = (Random(dy) + rc.top);
    rcOut.right = (rcOut.left + 0x3);
    rcOut.bottom = (rcOut.top + 0x3);
    /* untranslated: call SetBkColor(hdc, words(0x8:[(iClr * 0x4)+0x5fd0], 0x8:[(iClr * 0x4)+0x5fce])) -> callresult(COLORREF) */
    ExtTextOut(hdc, 0x0, 0x0, 0x2, &(rcOut), 0x0, 0x0, 0x0);
    /* untranslated: call SetBkColor(hdc, words(0x8:[(iClr * 0x4)+0x5fbc], 0x8:[(iClr * 0x4)+0x5fba])) -> callresult(COLORREF) */
    InflateRect(&(rcOut), 0xffff, 0x0);
    ExtTextOut(hdc, 0x0, 0x0, 0x2, &(rcOut), 0x0, 0x0, 0x0);
    InflateRect(&(rcOut), 0x1, 0xffff);
    ExtTextOut(hdc, 0x0, 0x0, 0x2, &(rcOut), 0x0, 0x0, 0x0);
    i = (i + 0x1);

L_625c:
    if ((i < iMax))
        goto L_613d;
    else
        goto L_6267;

L_6267:
    iClr = (iClr + 0x1);

L_626b:
    if ((iClr < 0x4))
        goto L_6113;
    else
        goto L_6274;

L_6274:
    PopRandom();
    return;
}

int32_t LongFromSerialCh(char ch) {
    int32_t l;

L_6280:
    if (((uint16_t)(ch) < 0x41))
        goto L_62b2;
    else
        goto L_6295;

L_6295:
    if (((uint16_t)(ch) > 0x5a))
        goto L_62b2;
    else
        goto L_62a1;

L_62a1:
    l = (uint32_t)(((uint16_t)(ch) + 0xffbf));
    goto L_62c0;

L_62b2:
    l = (uint32_t)(((uint16_t)(ch) + 0xffea));

L_62c0:
    if ((HIWORD(l) < 0x0))
        goto L_62e3;
    else
        goto L_62c9;

L_62c9:
    if ((HIWORD(l) > 0x0))
        goto L_62d7;
    else
        goto L_62ce;

L_62ce:
    if ((LOWORD(l) < 0x20))
        goto L_62e3;
    else
        goto L_62d7;

L_62d7:
    return l;

L_62e3:
    /* untranslated: return words((LOWORD(l) ^ 0x15), (HIWORD(l) ^ 0x0)) */
}

int16_t FValidSerialNo(char *psz, int32_t *plSerial) {
    int32_t  lBuild;
    int16_t  i;
    int32_t  lCur;
    int32_t  lSerial;
    int32_t  l;
    uint16_t t_6350;
    uint16_t t_6389;
    uint16_t t_63c2;
    uint16_t t_63fb;
    uint16_t t_64d6;

L_62f8:
    lSerial = LongFromSerialCh((uint16_t)(*(psz)));
    if ((HIWORD(lSerial) > 0x0))
        goto L_6335;
    else
        goto L_631f;

L_631f:
    if ((HIWORD(lSerial) < 0x0))
        goto L_632d;
    else
        goto L_6324;

L_6324:
    if ((LOWORD(lSerial) >= 0x20))
        goto L_6335;
    else
        goto L_632d;

L_632d:
    LOWORD(lSerial) = (LOWORD(lSerial) ^ 0x15);
    HIWORD(lSerial) = (HIWORD(lSerial) ^ 0x0);

L_6335:
    LongFromSerialCh((uint16_t)(psz[0x1]));
    /* untranslated: ss:[bp-0x18] = loword(callresult(int32_t)) */
    /* untranslated: ss:[bp-0x16] = hiword(callresult(int32_t)) */
    t_6350 = LOWORD(lSerial);
    /* untranslated: LOWORD(lSerial) = (loword((uint32_t)(lSerial * 0x24)) + ss:[bp-0x18]) */
    /* untranslated: HIWORD(lSerial) = (hiword((uint32_t)(words(HIWORD(lSerial), t_6350) * 0x24)) + ss:[bp-0x16]) */
    LongFromSerialCh((uint16_t)(psz[0x4]));
    /* untranslated: ss:[bp-0x18] = loword(callresult(int32_t)) */
    /* untranslated: ss:[bp-0x16] = hiword(callresult(int32_t)) */
    t_6389 = LOWORD(lSerial);
    /* untranslated: LOWORD(lSerial) = (loword((uint32_t)(lSerial * 0x24)) + ss:[bp-0x18]) */
    /* untranslated: HIWORD(lSerial) = (hiword((uint32_t)(words(HIWORD(lSerial), t_6389) * 0x24)) + ss:[bp-0x16]) */
    LongFromSerialCh((uint16_t)(psz[0x7]));
    /* untranslated: ss:[bp-0x18] = loword(callresult(int32_t)) */
    /* untranslated: ss:[bp-0x16] = hiword(callresult(int32_t)) */
    t_63c2 = LOWORD(lSerial);
    /* untranslated: LOWORD(lSerial) = (loword((uint32_t)(lSerial * 0x24)) + ss:[bp-0x18]) */
    /* untranslated: HIWORD(lSerial) = (hiword((uint32_t)(words(HIWORD(lSerial), t_63c2) * 0x24)) + ss:[bp-0x16]) */
    LongFromSerialCh((uint16_t)(psz[0x3]));
    /* untranslated: ss:[bp-0x18] = loword(callresult(int32_t)) */
    /* untranslated: ss:[bp-0x16] = hiword(callresult(int32_t)) */
    t_63fb = LOWORD(lSerial);
    /* untranslated: LOWORD(lSerial) = (loword((uint32_t)(lSerial * 0x24)) + ss:[bp-0x18]) */
    /* untranslated: HIWORD(lSerial) = (hiword((uint32_t)(words(HIWORD(lSerial), t_63fb) * 0x24)) + ss:[bp-0x16]) */
    if ((plSerial == 0x0))
        goto L_6430;
    else
        goto L_6422;

L_6422:
    plSerial = LOWORD(lSerial);
    *(plSerial + 0x2) = HIWORD(lSerial);

L_6430:
    PushRandom(0x11000b);
    lCur = lSerial;
    Randomize2(lCur);
    lCur = (int32_t)((lCur >> 0xe));
    lBuild = 0x0;
    i = 0x0;
    goto L_6509;

L_6480:
    LOWORD(l) = (LOWORD(lCur) & 0xf);
    HIWORD(l) = (HIWORD(lCur) & 0x0);
    goto L_64a9;

L_6495:
    Random(0x100);
    l = (l - 0x1);

L_64a9:
    if ((HIWORD(l) > 0x0))
        goto L_6495;
    else
        goto L_64b2;

L_64b2:
    if ((HIWORD(l) < 0x0))
        goto L_64c0;
    else
        goto L_64b7;

L_64b7:
    if ((LOWORD(l) >= 0x0))
        goto L_6495;
    else
        goto L_64c0;

L_64c0:
    Random(0x100);
    /* untranslated: ss:[bp-0x18] = callresult(int16_t) */
    /* untranslated: ss:[bp-0x16] = signhiword(callresult(int16_t)) */
    t_64d6 = LOWORD(lBuild);
    /* untranslated: LOWORD(lBuild) = (loword((int32_t)(lBuild * 0x100)) + ss:[bp-0x18]) */
    /* untranslated: HIWORD(lBuild) = (hiword((int32_t)(words(t_64d6, HIWORD(lBuild)) * 0x100)) + ss:[bp-0x16]) */
    lCur = (int32_t)((lCur >> 0x4));
    i = (i + 0x1);

L_6509:
    if ((i < 0x3))
        goto L_6480;
    else
        goto L_6512;

L_6512:
    PopRandom();
    l = LongFromSerialCh((uint16_t)(psz[0x2]));
    if ((LOWORD(l) != LOWORD((int32_t)((lBuild % 0x24)))))
        goto L_6550;
    else
        goto L_6548;

L_6548:
    /* untranslated: branch HIWORD(l) == hiword(callresult(int32_t)) ? L_6556 : L_6550 */

L_6550:
    return 0x0;

L_6556:
    lBuild = (int32_t)((lBuild / 0x24));
    l = LongFromSerialCh((uint16_t)(psz[0x5]));
    if ((LOWORD(l) != LOWORD((int32_t)((lBuild % 0x24)))))
        goto L_65a8;
    else
        goto L_65a0;

L_65a0:
    /* untranslated: branch HIWORD(l) == hiword(callresult(int32_t)) ? L_65ae : L_65a8 */

L_65a8:
    return 0x0;

L_65ae:
    lBuild = (int32_t)((lBuild / 0x24));
    l = LongFromSerialCh((uint16_t)(psz[0x6]));
    if ((LOWORD(l) != LOWORD((int32_t)((lBuild % 0x24)))))
        goto L_6600;
    else
        goto L_65f8;

L_65f8:
    /* untranslated: branch HIWORD(l) == hiword(callresult(int32_t)) ? L_6606 : L_6600 */

L_6600:
    return 0x0;

L_6606:
    return 0x1;
}

int16_t FMatchTarget(FLEET *lpflTarget, int16_t mdTarget, int16_t fExact) {
    int16_t imd;
    int16_t ish;

L_6612:
    goto L_688f;

L_6621:
    if ((fExact == 0x0))
        goto L_68ba;
    else
        goto L_662a;

L_662a:
    return 0x0;

L_6633:
    ish = 0x0;
    goto L_66ac;

L_663b:
    if ((lpflTarget->rgcsh[ish] == 0x0))
        goto L_66a8;
    else
        goto L_6658;

L_6658:
    imd = ((*(LphuldefFromId(rglpshdef[lpflTarget->iPlayer][ish].hul.ihuldef) + 0x7b) >> 0xa) & 0xf);
    if ((imd < 0x2))
        goto L_66a8;
    else
        goto L_669c;

L_669c:
    if ((imd <= 0x4))
        goto L_66b5;
    else
        goto L_66a2;

L_66a2:

L_66a8:
    ish = (ish + 0x1);

L_66ac:
    if ((ish < 0x10))
        goto L_663b;
    else
        goto L_66b5;

L_66b5:
    if ((mdTarget != 0x3))
        goto L_66d0;
    else
        goto L_66be;

L_66be:
    if ((ish != 0x10))
        goto L_68ba;
    else
        goto L_66c7;

L_66c7:
    return 0x0;

L_66d0:
    if ((ish == 0x10))
        goto L_68ba;
    else
        goto L_66d9;

L_66d9:
    return 0x0;

L_66e2:
    ish = 0x0;
    goto L_675b;

L_66ea:
    if ((lpflTarget->rgcsh[ish] == 0x0))
        goto L_6757;
    else
        goto L_6707;

L_6707:
    imd = ((*(LphuldefFromId(rglpshdef[lpflTarget->iPlayer][ish].hul.ihuldef) + 0x7b) >> 0xa) & 0xf);
    if ((imd == 0x1))
        goto L_6764;
    else
        goto L_674b;

L_674b:
    if ((imd == 0x5))
        goto L_6764;
    else
        goto L_6751;

L_6751:

L_6757:
    ish = (ish + 0x1);

L_675b:
    if ((ish < 0x10))
        goto L_66ea;
    else
        goto L_6764;

L_6764:
    if ((ish != 0x10))
        goto L_68ba;
    else
        goto L_676d;

L_676d:
    return 0x0;

L_6776:
    ish = 0x0;
    goto L_67e6;

L_677e:
    if ((lpflTarget->rgcsh[ish] == 0x0))
        goto L_67e2;
    else
        goto L_679b;

L_679b:
    imd = ((*(LphuldefFromId(rglpshdef[lpflTarget->iPlayer][ish].hul.ihuldef) + 0x7b) >> 0xa) & 0xf);
    if ((imd == 0x7))
        goto L_67ef;
    else
        goto L_67dc;

L_67dc:

L_67e2:
    ish = (ish + 0x1);

L_67e6:
    if ((ish < 0x10))
        goto L_677e;
    else
        goto L_67ef;

L_67ef:
    if ((ish != 0x10))
        goto L_68ba;
    else
        goto L_67f8;

L_67f8:
    return 0x0;

L_6801:
    ish = 0x0;
    goto L_6871;

L_6809:
    if ((lpflTarget->rgcsh[ish] == 0x0))
        goto L_686d;
    else
        goto L_6826;

L_6826:
    imd = ((*(LphuldefFromId(rglpshdef[lpflTarget->iPlayer][ish].hul.ihuldef) + 0x7b) >> 0xa) & 0xf);
    if ((imd == 0x1))
        goto L_687a;
    else
        goto L_6867;

L_6867:

L_686d:
    ish = (ish + 0x1);

L_6871:
    if ((ish < 0x10))
        goto L_6809;
    else
        goto L_687a;

L_687a:
    if ((ish != 0x10))
        goto L_68ba;
    else
        goto L_6883;

L_6883:
    return 0x0;

L_688f:
    if ((mdTarget == 0x3))
        goto L_6633;
    else
        goto L_6897;

L_6897:
    if ((mdTarget == 0x4))
        goto L_66e2;
    else
        goto L_689f;

L_689f:
    if ((mdTarget == 0x5))
        goto L_6633;
    else
        goto L_68a7;

L_68a7:
    if ((mdTarget == 0x6))
        goto L_6776;
    else
        goto L_68af;

L_68af:
    if ((mdTarget != 0x7))
        goto L_6621;
    else
        goto L_68b4;

L_68b4:

L_68ba:
    return 0x1;
}

void ValidateWaypoints() {
    int16_t  mdTarget;
    FLEET   *lpflTarget;
    int16_t  ifl2;
    int32_t  wt;
    FLEET   *lpflMatch;
    int32_t  wtMatch;
    ORDER   *lpord;
    int16_t  ifl;
    THING   *lpth;
    FLEET   *lpfl;
    int16_t  cFound;
    int16_t  iord;
    FLEET   *lpfl2;
    int16_t  iplrHi;
    uint16_t t_merge_6aea_0001;

L_68c6:
    ifl = 0x0;
    goto L_68db;

L_68d7:
    ifl = (ifl + 0x1);

L_68db:
    if ((ifl >= cFleet))
        goto L_707b;
    else
        goto L_68e6;

L_68e6:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_6916;
    else
        goto L_690e;

L_690e:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_707b;
    else
        goto L_6916;

L_6916:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x0]) >> 0x8) & 0xf) == 0x1 ? L_69db : L_6931 */

L_6931:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) == 0x1 ? L_69db : L_6947 */

L_6947:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) == 0x4 ? L_69db : L_695d */

L_695d:
    if ((lpfl->fMark != 0x0))
        goto L_69db;
    else
        goto L_6974;

L_6974:
    if ((lpfl->idPlanet != 0xffff))
        goto L_69ad;
    else
        goto L_6981;

L_6981:
    /* untranslated: part[6:2](lpfl->lpplord->rgord[0x0]) = ((part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf0ff) | 0x400) */
    lpfl->lpplord->rgord[0x0].id = 0x0;
    goto L_69db;

L_69ad:
    /* untranslated: part[6:2](lpfl->lpplord->rgord[0x0]) = ((part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf0ff) | 0x100) */
    lpfl->lpplord->rgord[0x0].id = lpfl->idPlanet;

L_69db:
    if ((lpfl->fDead != 0x0))
        goto L_68d7;
    else
        goto L_69ef;

L_69ef:

L_69f5:
    if ((lpfl->pt.x >= 0x3e8))
        goto L_6a1c;
    else
        goto L_6a03;

L_6a03:
    lpfl->pt.x = 0x3e8;
    lpfl->lpplord->rgord[0x0].pt.x = 0x3e8;
    goto L_6a4c;

L_6a1c:
    if ((lpfl->pt.x <= (dGal + 0x3e8)))
        goto L_6a4c;
    else
        goto L_6a2e;

L_6a2e:
    lpfl->pt.x = (dGal + 0x3e8);
    lpfl->lpplord->rgord[0x0].pt.x = (dGal + 0x3e8);

L_6a4c:
    if ((lpfl->pt.y >= 0x3e8))
        goto L_6a73;
    else
        goto L_6a5a;

L_6a5a:
    lpfl->pt.y = 0x3e8;
    lpfl->lpplord->rgord[0x0].pt.y = 0x3e8;
    goto L_6aa3;

L_6a73:
    if ((lpfl->pt.y <= (dGal + 0x3e8)))
        goto L_6aa3;
    else
        goto L_6a85;

L_6a85:
    lpfl->pt.y = (dGal + 0x3e8);
    lpfl->lpplord->rgord[0x0].pt.y = (dGal + 0x3e8);

L_6aa3:
    if ((lpfl->cord > 0x1))
        goto L_6aca;
    else
        goto L_6ab0;

L_6ab0:
    if ((lpfl->fMark == 0x0))
        goto L_68d7;
    else
        goto L_6ac4;

L_6ac4:

L_6aca:
    if ((lpfl->fMark != 0x0))
        goto L_6ae7;
    else
        goto L_6ae1;

L_6ae1:
    t_merge_6aea_0001 = 0x1;
    goto L_6aea;

L_6ae7:
    t_merge_6aea_0001 = 0x0;

L_6aea:
    iord = t_merge_6aea_0001;
    lpord = &(lpfl->lpplord->rgord[iord]);
    goto L_6b2d;

L_6b1a:
    iord = (iord + 0x1);
    lpord = (lpord + 0x1);

L_6b2d:
    if ((iord >= lpfl->cord))
        goto L_68d7;
    else
        goto L_6b3c;

L_6b3c:
    if ((lpord->grobj != grobjThing))
        goto L_6c50;
    else
        goto L_6b53;

L_6b53:
    lpth = LpthFromId(lpord->id);
    if ((LOWORD(lpth) != 0x0))
        goto L_6b7a;
    else
        goto L_6b71;

L_6b71:
    if ((HIWORD(lpth) == 0x0))
        goto L_6bca;
    else
        goto L_6b7a;

L_6b7a:
    if ((lpth->ith != ithWormhole))
        goto L_6c38;
    else
        goto L_6b90;

L_6b90:
    if ((((0x1 << lpfl->iPlayer) & lpth->thw.grbitPlr) != 0x0))
        goto L_6c38;
    else
        goto L_6bab;

L_6bab:
    if ((lpth->pt.x != lpord->pt.x))
        goto L_6bca;
    else
        goto L_6bc1;

L_6bc1:
    if ((lpth->pt.y == lpord->pt.y))
        goto L_6c38;
    else
        goto L_6bca;

L_6bca:
    if ((LOWORD(lpth) != 0x0))
        goto L_6bdc;
    else
        goto L_6bd3;

L_6bd3:
    if ((HIWORD(lpth) == 0x0))
        goto L_6c17;
    else
        goto L_6bdc;

L_6bdc:
    FSendPlrMsg(lpfl->iPlayer, 0xf8, (0x8000 | LOWORD(lpfl)), LOWORD(lpfl), 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);

L_6c17:
    lpord->grobj = grobjOther;
    lpord->id = iord;
    goto L_6b1a;

L_6c38:
    lpord->pt.x = lpth->pt.x;
    lpord->pt.y = lpth->pt.y;

L_6c50:
    if ((lpord->grobj != grobjFleet))
        goto L_6b1a;
    else
        goto L_6c67;

L_6c67:
    if ((lpord->fNoAutoTrack != 0x0))
        goto L_6b1a;
    else
        goto L_6c7b;

L_6c7b:

L_6c81:
    lpflTarget = LpflFromId(lpord->id);
    if ((LOWORD(lpflTarget) != 0x0))
        goto L_6ca8;
    else
        goto L_6c9f;

L_6c9f:
    if ((HIWORD(lpflTarget) == 0x0))
        goto L_6cf6;
    else
        goto L_6ca8;

L_6ca8:
    if ((lpflTarget->pt.x != lpord->pt.x))
        goto L_6cf6;
    else
        goto L_6cbe;

L_6cbe:
    if ((lpflTarget->pt.y != lpord->pt.y))
        goto L_6cf6;
    else
        goto L_6cc7;

L_6cc7:
    if ((lpflTarget->fCompChg == 0x0))
        goto L_704f;
    else
        goto L_6ce3;

L_6ce3:
    if ((lpflTarget->iPlayer == lpfl->iPlayer))
        goto L_704f;
    else
        goto L_6cf6;

L_6cf6:
    lpflTarget = 0x0;
    iplrHi = (lpord->id & 0xfe00);
    cFound = 0x0;
    mdTarget = (HIWORD(rglpbtlplan[lpfl->iPlayer][lpfl->iplan]) & 0xf);
    wtMatch = 0x0;
    lpflMatch = 0x0;
    ifl2 = 0x0;
    goto L_6d62;

L_6d5e:
    ifl2 = (ifl2 + 0x1);

L_6d62:
    if ((ifl2 >= cFleet))
        goto L_6e75;
    else
        goto L_6d6d;

L_6d6d:
    lpfl2 = rglpfl[ifl2];
    if ((LOWORD(rglpfl[ifl2]) != 0x0))
        goto L_6d9d;
    else
        goto L_6d95;

L_6d95:
    if ((*(rglpfl[ifl2] + 0x2) == 0x0))
        goto L_6e75;
    else
        goto L_6d9d;

L_6d9d:
    if ((lpfl2->pt.x != lpord->pt.x))
        goto L_6d5e;
    else
        goto L_6db3;

L_6db3:
    if ((lpfl2->pt.y != lpord->pt.y))
        goto L_6d5e;
    else
        goto L_6dbc;

L_6dbc:
    if ((iplrHi != (LOWORD(lpfl2) & 0xfe00)))
        goto L_6d5e;
    else
        goto L_6dcd;

L_6dcd:
    if ((FMatchTarget(lpfl2, mdTarget, 0x1) == 0x0))
        goto L_6d5e;
    else
        goto L_6dea;

L_6dea:
    wt = WtFromLpfl(lpfl2);
    if ((lpfl2->fTargeted != 0x0))
        goto L_6d5e;
    else
        goto L_6e15;

L_6e15:
    if ((HIWORD(wt) > HIWORD(wtMatch)))
        goto L_6e5a;
    else
        goto L_6e23;

L_6e23:
    if ((HIWORD(wt) < HIWORD(wtMatch)))
        goto L_6e30;
    else
        goto L_6e28;

L_6e28:
    if ((LOWORD(wt) > LOWORD(wtMatch)))
        goto L_6e5a;
    else
        goto L_6e30;

L_6e30:
    if ((LOWORD(wt) != LOWORD(wtMatch)))
        goto L_6d5e;
    else
        goto L_6e3e;

L_6e3e:
    if ((HIWORD(wt) != HIWORD(wtMatch)))
        goto L_6d5e;
    else
        goto L_6e46;

L_6e46:
    if ((Random(0x2) != 0x0))
        goto L_6d5e;
    else
        goto L_6e5a;

L_6e5a:
    wtMatch = wt;
    lpflMatch = lpfl2;

L_6e75:
    if ((LOWORD(lpflMatch) != 0x0))
        goto L_6fe6;
    else
        goto L_6e7e;

L_6e7e:
    if ((HIWORD(lpflMatch) != 0x0))
        goto L_6fe6;
    else
        goto L_6e87;

L_6e87:
    ifl2 = 0x0;
    goto L_6e93;

L_6e8f:
    ifl2 = (ifl2 + 0x1);

L_6e93:
    if ((ifl2 >= cFleet))
        goto L_6fe6;
    else
        goto L_6e9e;

L_6e9e:
    lpfl2 = rglpfl[ifl2];
    if ((LOWORD(rglpfl[ifl2]) != 0x0))
        goto L_6ece;
    else
        goto L_6ec6;

L_6ec6:
    if ((*(rglpfl[ifl2] + 0x2) == 0x0))
        goto L_6fe6;
    else
        goto L_6ece;

L_6ece:
    if ((lpfl2->pt.x != lpord->pt.x))
        goto L_6e8f;
    else
        goto L_6ee4;

L_6ee4:
    if ((lpfl2->pt.y != lpord->pt.y))
        goto L_6e8f;
    else
        goto L_6eed;

L_6eed:
    if ((iplrHi != (LOWORD(lpfl2) & 0xfe00)))
        goto L_6e8f;
    else
        goto L_6efe;

L_6efe:
    if ((FMatchTarget(lpfl2, mdTarget, 0x1) == 0x0))
        goto L_6f8c;
    else
        goto L_6f1b;

L_6f1b:
    wt = WtFromLpfl(lpfl2);
    if ((HIWORD(wt) > HIWORD(wtMatch)))
        goto L_6f74;
    else
        goto L_6f3d;

L_6f3d:
    if ((HIWORD(wt) < HIWORD(wtMatch)))
        goto L_6f4a;
    else
        goto L_6f42;

L_6f42:
    if ((LOWORD(wt) > LOWORD(wtMatch)))
        goto L_6f74;
    else
        goto L_6f4a;

L_6f4a:
    if ((LOWORD(wt) != LOWORD(wtMatch)))
        goto L_6f8c;
    else
        goto L_6f58;

L_6f58:
    if ((HIWORD(wt) != HIWORD(wtMatch)))
        goto L_6f8c;
    else
        goto L_6f60;

L_6f60:
    if ((Random(0x2) != 0x0))
        goto L_6f8c;
    else
        goto L_6f74;

L_6f74:
    wtMatch = wt;
    lpflMatch = lpfl2;

L_6f8c:
    cFound = (cFound + 0x1);
    if ((Random(cFound) != 0x0))
        goto L_6e8f;
    else
        goto L_6fa3;

L_6fa3:
    if ((cFound == 0x1))
        goto L_6fd7;
    else
        goto L_6fac;

L_6fac:
    if ((lpfl2->fTargeted == 0x0))
        goto L_6fd7;
    else
        goto L_6fc3;

L_6fc3:
    if ((Random(0x2) == 0x0))
        goto L_6e8f;
    else
        goto L_6fd7;

L_6fd7:
    lpflTarget = lpfl2;

L_6fe6:
    if ((LOWORD(lpflMatch) != 0x0))
        goto L_6ff8;
    else
        goto L_6fef;

L_6fef:
    if ((HIWORD(lpflMatch) == 0x0))
        goto L_7004;
    else
        goto L_6ff8;

L_6ff8:
    lpflTarget = lpflMatch;

L_7004:
    if ((LOWORD(lpflTarget) != 0x0))
        goto L_7016;
    else
        goto L_700d;

L_700d:
    if ((HIWORD(lpflTarget) == 0x0))
        goto L_6b1a;
    else
        goto L_7016;

L_7016:
    lpord->id = LOWORD(lpflTarget);
    lpord->pt.x = lpflTarget->pt.x;
    lpord->pt.y = lpflTarget->pt.y;
    lpflTarget->fTargeted = 0x1;

L_704f:
    if ((LOWORD(lpflTarget) != 0x0))
        goto L_7061;
    else
        goto L_7058;

L_7058:
    if ((HIWORD(lpflTarget) == 0x0))
        goto L_6b1a;
    else
        goto L_7061;

L_7061:
    lpflTarget->fTargeted = 0x1;

L_707b:
    return;
}

int32_t ChgPopFromPlanet(PLANET *lppl, int16_t fUpdate) {
    int32_t  lMaxPop;
    int16_t  fPopDied;
    int32_t  lPopIncDelta;
    int16_t  DeltaCur;
    int32_t  pctGrow100;
    int16_t  pctDesire;
    int32_t  lPopInc100;
    int32_t  lPopInc;
    int32_t  lPopOld;
    int32_t  pctRetard;
    int32_t  pctFull;
    uint32_t t_merge_7168_0001_wide;
    uint16_t t_merge_7397_0001;
    uint16_t t_merge_7397_0002;

L_7082:
    fPopDied = 0x0;
    if ((lppl->iPlayer == 0xffff))
        goto L_70b4;
    else
        goto L_709d;

L_709d:
    if ((LOWORD(lppl->rgwtMin[0x3]) != 0x0))
        goto L_70bd;
    else
        goto L_70aa;

L_70aa:
    if ((HIWORD(lppl->rgwtMin[0x3]) != 0x0))
        goto L_70bd;
    else
        goto L_70b4;

L_70b4:
    return 0x0;

L_70bd:
    pctDesire = PctPlanetDesirability(lppl, lppl->iPlayer);
    lPopOld = lppl->rgwtMin[0x3];
    fPopDied = 0x1;
    if ((pctDesire >= 0x0))
        goto L_7210;
    else
        goto L_70f4;

L_70f4:
    /* untranslated: branch 0x0 < hiword((int32_t)((uint32_t)(lPopOld * sext16to32((pctDesire neg 0x0))) / 0xa)) ? L_7146 : L_7131 */

L_7131:
    /* untranslated: branch ss:[bp-0x22] > hiword(callresult(int32_t)) ? L_713d : L_7136 */

L_7136:
    /* untranslated: branch ss:[bp-0x24] <= loword(callresult(int32_t)) ? L_7146 : L_713d */

L_713d:
    t_merge_7168_0001_wide = 0x1;
    goto L_7168;

L_7146:
    /* untranslated: t_merge_7168_0001_wide = callresult(int32_t) */

L_7168:
    lPopInc100 = t_merge_7168_0001_wide;
    lPopInc = (int32_t)((lPopInc100 / 0x64));
    lPopIncDelta = (int32_t)((lPopInc100 % 0x64));
    if ((LOWORD(lPopInc) != 0x0))
        goto L_71ce;
    else
        goto L_71a9;

L_71a9:
    if ((HIWORD(lPopInc) != 0x0))
        goto L_71ce;
    else
        goto L_71b2;

L_71b2:
    if ((LOWORD(lPopIncDelta) != 0x0))
        goto L_71ce;
    else
        goto L_71bb;

L_71bb:
    if ((HIWORD(lPopIncDelta) != 0x0))
        goto L_71ce;
    else
        goto L_71c4;

L_71c4:
    lPopIncDelta = 0x1;

L_71ce:
    DeltaCur = (lppl->iDeltaPop - LOWORD(lPopIncDelta));
    if ((DeltaCur >= 0x0))
        goto L_71fa;
    else
        goto L_71ee;

L_71ee:
    lPopInc = (lPopInc + 0x1);
    DeltaCur = (DeltaCur + 0x64);

L_71fa:
    /* untranslated: LOWORD(lPopInc) = (LOWORD(lPopInc) neg 0x0) */
    /* untranslated: HIWORD(lPopInc) = ((HIWORD(lPopInc) + 0x0) neg 0x0) */
    goto LUpdateAndExit;

L_7210:
    lMaxPop = CalcPlanetMaxPop(lppl->id, lppl->iPlayer);
    PctTrueMaxGrowth(lppl->iPlayer);
    /* untranslated: LOWORD(pctGrow100) = loword((callresult(int16_t) * pctDesire)) */
    /* untranslated: HIWORD(pctGrow100) = signhiword(loword((callresult(int16_t) * pctDesire))) */
    if ((gd.fGeneratingTurn == 0x0))
        goto L_7289;
    else
        goto L_7254;

L_7254:
    /* untranslated: branch ((part[84:2](rgplr[lppl->iPlayer]) >> 0x2) & 0x1) == 0x0 ? L_7289 : L_7275 */

L_7275:
    pctGrow100 = (int32_t)((pctGrow100 >> 0x1));

L_7289:
    if ((HIWORD(lPopOld) < HIWORD((int32_t)((lMaxPop / 0x4)))))
        goto L_7462;
    else
        goto L_72a4;

L_72a4:
    /* untranslated: branch HIWORD(lPopOld) > hiword(callresult(int32_t)) ? L_72b1 : L_72a9 */

L_72a9:
    /* untranslated: branch LOWORD(lPopOld) <= loword(callresult(int32_t)) ? L_7462 : L_72b1 */

L_72b1:
    pctFull = (int32_t)(((uint32_t)((lPopOld * 0x3e8)) / lMaxPop));
    if ((HIWORD(lPopOld) < HIWORD(lMaxPop)))
        goto L_73b4;
    else
        goto L_72e5;

L_72e5:
    if ((HIWORD(lPopOld) > HIWORD(lMaxPop)))
        goto L_72f2;
    else
        goto L_72ea;

L_72ea:
    if ((LOWORD(lPopOld) < LOWORD(lMaxPop)))
        goto L_73b4;
    else
        goto L_72f2;

L_72f2:
    if ((HIWORD(lPopOld) > (HIWORD(lMaxPop) + 0x0)))
        goto L_731f;
    else
        goto L_7306;

L_7306:
    if ((HIWORD(lPopOld) < (HIWORD(lMaxPop) + 0x0)))
        goto L_7313;
    else
        goto L_730b;

L_730b:
    if ((LOWORD(lPopOld) >= (LOWORD(lMaxPop) + 0xa)))
        goto L_731f;
    else
        goto L_7313;

L_7313:
    return 0x0;

L_731f:
    if ((0xffff < (0x0 - HIWORD((int32_t)((pctFull / 0xa))))))
        goto L_736a;
    else
        goto L_7355;

L_7355:
    /* untranslated: branch ss:[bp-0x2a] > (0x0 - hiword(callresult(int32_t))) ? L_7361 : L_735a */

L_735a:
    /* untranslated: branch ss:[bp-0x2c] <= (0x63 - loword(callresult(int32_t))) ? L_736a : L_7361 */

L_7361:
    t_merge_7397_0001 = 0xfed4;
    t_merge_7397_0002 = 0xffff;
    goto L_7397;

L_736a:
    /* untranslated: ss:[bp-0x30] = loword((int32_t)(pctFull / 0xa)) */
    /* untranslated: ss:[bp-0x2e] = hiword((int32_t)(pctFull / 0xa)) */
    /* untranslated: t_merge_7397_0001 = (0x63 - ss:[bp-0x30]) */
    /* untranslated: t_merge_7397_0002 = (0x0 - ss:[bp-0x2e]) */

L_7397:
    LOWORD(pctRetard) = t_merge_7397_0001;
    HIWORD(pctRetard) = t_merge_7397_0002;
    pctGrow100 = (int32_t)((pctRetard * 0x4));
    goto L_7462;

L_73b4:
    pctRetard = (0x3e8 - pctFull);
    pctRetard = (uint32_t)((pctRetard * pctRetard));
    if ((HIWORD(pctGrow100) > 0x0))
        goto L_741e;
    else
        goto L_73e6;

L_73e6:
    if ((HIWORD(pctGrow100) < 0x0))
        goto L_73f5;
    else
        goto L_73eb;

L_73eb:
    if ((LOWORD(pctGrow100) >= 0x3e8))
        goto L_741e;
    else
        goto L_73f5;

L_73f5:
    pctGrow100 = (int32_t)(((uint32_t)((pctGrow100 * pctRetard)) / 0x89544));
    goto L_7462;

L_741e:
    pctGrow100 = (uint32_t)(((int32_t)(((uint32_t)(((int32_t)((pctGrow100 / 0xa)) * pctRetard)) / 0x89544)) * 0xa));

L_7462:
    lPopInc100 = (uint32_t)((lPopOld * (int32_t)((pctGrow100 / 0x64))));
    if ((HIWORD(lPopInc100) > 0x98))
        goto L_74c7;
    else
        goto L_7492;

L_7492:
    if ((HIWORD(lPopInc100) < 0x98))
        goto L_74a1;
    else
        goto L_7497;

L_7497:
    if ((LOWORD(lPopInc100) >= 0x9680))
        goto L_74c7;
    else
        goto L_74a1;

L_74a1:
    lPopInc100 = (int32_t)(((uint32_t)((lPopOld * pctGrow100)) / 0x64));

L_74c7:
    lPopInc = (int32_t)((lPopInc100 / 0x64));
    lPopIncDelta = (int32_t)((lPopInc100 % 0x64));
    if ((LOWORD(lPopInc) != 0x0))
        goto L_7527;
    else
        goto L_7502;

L_7502:
    if ((HIWORD(lPopInc) != 0x0))
        goto L_7527;
    else
        goto L_750b;

L_750b:
    if ((LOWORD(lPopIncDelta) != 0x0))
        goto L_7527;
    else
        goto L_7514;

L_7514:
    if ((HIWORD(lPopIncDelta) != 0x0))
        goto L_7527;
    else
        goto L_751d;

L_751d:
    lPopIncDelta = 0x1;

L_7527:
    DeltaCur = (lppl->iDeltaPop + LOWORD(lPopIncDelta));
    if ((DeltaCur < 0x64))
        goto L_7556;
    else
        goto L_7547;

L_7547:
    lPopInc = (lPopInc + 0x1);
    DeltaCur = (DeltaCur - 0x64);
    goto LUpdateAndExit;

L_7556:
    if ((DeltaCur >= 0x0))
        goto LUpdateAndExit;
    else
        goto L_755f;

L_755f:
    lPopInc = (lPopInc - 0x1);
    DeltaCur = (DeltaCur + 0x64);

LUpdateAndExit:
    if ((fUpdate == 0x0))
        goto L_75d2;
    else
        goto L_7574;

L_7574:
    /* untranslated: ss:[bp-0x26] = ((*(lppl+0x16) & 0xffff) | hiword((int32_t)(words((DeltaCur & 0xff), (0x0 & 0x0)) << 0x0))) */
    /* untranslated: *(lppl+0x14) = ((*(lppl+0x14) & 0xff00) | loword((int32_t)(words((DeltaCur & 0xff), (0x0 & 0x0)) << 0x0))) */
    /* untranslated: *(lppl+0x16) = ss:[bp-0x26] */
    lppl->rgwtMin[0x3] = (lppl->rgwtMin[0x3] + lPopInc);

L_75d2:
    return lPopInc;
}

int16_t FCanFleetUseStargates(FLEET *lpfl, POINT ptSrc, POINT ptDst) {
    int16_t  dTravel;
    PLANET  *lpplDst;
    int16_t  pctDmg;
    int16_t  fSrcPlanet;
    int16_t  fUncertain;
    int16_t  i;
    int16_t  fDanger;
    PLANET  *lpplSrc;
    int16_t  isbsDst;
    int16_t  fCargo;
    int16_t  ishdef;
    int16_t  isbsSrc;
    SCAN     scan;
    uint16_t t_merge_7950_0001;

L_75e2:
    fUncertain = 0x0;
    /* untranslated: branch FFindNearestObject(words(ptDst.y, ptDst.x), 0x81, &scan) != 0x0 ? L_7614 : L_760e */

L_760e:
    return 0x0;

L_7614:
    lpplDst = LpplFromId(scan.idpl);
    if ((LOWORD(lpplDst) != 0x0))
        goto L_7637;
    else
        goto L_762e;

L_762e:
    if ((HIWORD(lpplDst) == 0x0))
        goto L_764a;
    else
        goto L_7637;

L_7637:
    if ((lpfl->iPlayer == lpplDst->iPlayer))
        goto L_7686;
    else
        goto L_764a;

L_764a:
    if ((LOWORD(lpplDst) != 0x0))
        goto L_765c;
    else
        goto L_7653;

L_7653:
    if ((HIWORD(lpplDst) == 0x0))
        goto L_767e;
    else
        goto L_765c;

L_765c:
    if ((lpplDst->iPlayer != 0xffff))
        goto L_767e;
    else
        goto L_7669;

L_7669:
    if ((lpplDst->turn != game.turn))
        goto L_767e;
    else
        goto L_7678;

L_7678:
    return 0x0;

L_767e:
    fUncertain = 0x1;
    goto LSrcChk;

L_7686:
    isbsDst = IStargateFromLppl(lpplDst);
    if ((isbsDst >= 0x0))
        goto LSrcChk;
    else
        goto L_76a0;

L_76a0:
    return 0x0;

LSrcChk:
    lpplSrc = 0x0;
    fSrcPlanet = 0x0;
    /* untranslated: branch FFindNearestObject(words(ptSrc.y, ptSrc.x), 0x81, &scan) == 0x0 ? L_7743 : L_76d0 */

L_76d0:

L_76d6:
    fSrcPlanet = 0x1;
    lpplSrc = LpplFromId(scan.idpl);
    if ((LOWORD(lpplSrc) != 0x0))
        goto L_76fe;
    else
        goto L_76f5;

L_76f5:
    if ((HIWORD(lpplSrc) == 0x0))
        goto LJumpgate;
    else
        goto L_76fe;

L_76fe:
    if ((lpfl->iPlayer != lpplSrc->iPlayer))
        goto LJumpgate;
    else
        goto L_770e;

L_770e:

L_7714:
    isbsSrc = IStargateFromLppl(lpplSrc);
    if ((isbsSrc < 0x0))
        goto LJumpgate;
    else
        goto L_772b;

L_772b:

L_7731:
    if ((fUncertain == 0x0))
        goto L_77e5;
    else
        goto L_773a;

L_773a:
    return 0xffff;

LJumpgate:
    if ((FFleetCanJumpgate(lpfl) == 0x0))
        goto L_7771;
    else
        goto L_7759;

L_7759:
    if ((fUncertain == 0x0))
        goto L_7768;
    else
        goto L_7762;

L_7762:
    return 0xffff;

L_7768:
    isbsSrc = isbsDst;
    goto L_77e5;

L_7771:
    if ((fSrcPlanet == 0x0))
        goto L_77a8;
    else
        goto L_777a;

L_777a:
    if ((LOWORD(lpplSrc) != 0x0))
        goto L_778c;
    else
        goto L_7783;

L_7783:
    if ((HIWORD(lpplSrc) == 0x0))
        goto L_77b1;
    else
        goto L_778c;

L_778c:
    if ((lpplSrc->iPlayer != 0xffff))
        goto L_77b1;
    else
        goto L_7799;

L_7799:
    if ((lpplSrc->turn != game.turn))
        goto L_77b1;
    else
        goto L_77a8;

L_77a8:
    return 0x0;

L_77b1:
    if ((LOWORD(lpplSrc) != 0x0))
        goto L_77c3;
    else
        goto L_77ba;

L_77ba:
    if ((HIWORD(lpplSrc) == 0x0))
        goto L_77d6;
    else
        goto L_77c3;

L_77c3:
    if ((lpplSrc->iPlayer == lpfl->iPlayer))
        goto L_77df;
    else
        goto L_77d6;

L_77d6:
    return 0xffff;

L_77df:
    return 0x0;

L_77e5:
    fCargo = 0x0;
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) == raStargate))
        goto L_7856;
    else
        goto L_780e;

L_780e:
    i = 0x0;
    goto L_784d;

L_7816:
    if ((HIWORD(lpfl->rgwtMin[i]) < 0x0))
        goto L_7849;
    else
        goto L_7836;

L_7836:
    if ((HIWORD(lpfl->rgwtMin[i]) > 0x0))
        goto L_7844;
    else
        goto L_783b;

L_783b:
    if ((LOWORD(lpfl->rgwtMin[i]) <= 0x0))
        goto L_7849;
    else
        goto L_7844;

L_7844:
    fCargo = 0x1;

L_7849:
    i = (i + 0x1);

L_784d:
    if ((i <= 0x3))
        goto L_7816;
    else
        goto L_7856;

L_7856:
    DGetDistance(ptSrc.x, ptSrc.y, ptDst.x, ptDst.y);
    dTravel = LOWORD(__ftol());
    fDanger = 0x0;
    ishdef = 0x0;
    goto L_791d;

L_7886:
    if ((lpfl->rgcsh[ishdef] == 0x0))
        goto L_7919;
    else
        goto L_78a3;

L_78a3:
    MdCalcStargateDamage(isbsSrc, isbsDst, dTravel, rglpshdef[lpfl->iPlayer][ishdef].hul.wtEmpty, &(pctDmg));
    goto L_78f6;

L_78dc:
    return 0x0;

L_78e2:
    if ((pctDmg <= 0x0))
        goto L_7919;
    else
        goto L_78eb;

L_78eb:
    fDanger = 0x1;

L_78f6:
    /* untranslated: branch callresult(int16_t) == 0xfffe ? L_78dc : L_78fe */

L_78fe:
    /* untranslated: branch callresult(int16_t) == 0xffff ? L_78dc : L_7906 */

L_7906:
    /* untranslated: branch callresult(int16_t) == 0x0 ? L_78dc : L_790e */

L_790e:
    /* untranslated: branch callresult(int16_t) == 0x1 ? L_78e2 : L_7919 */

L_7919:
    ishdef = (ishdef + 0x1);

L_791d:
    if ((ishdef < 0x10))
        goto L_7886;
    else
        goto L_7926;

L_7926:
    if ((fDanger == 0x0))
        goto L_7935;
    else
        goto L_792f;

L_792f:
    goto L_7938;

L_7935:

L_7938:
    if ((fCargo == 0x0))
        goto L_794d;
    else
        goto L_7947;

L_7947:
    t_merge_7950_0001 = 0x4;
    goto L_7950;

L_794d:
    t_merge_7950_0001 = 0x0;

L_7950:
    /* untranslated: return (ss:[bp-0x30] + t_merge_7950_0001) */
}

int16_t FFleetCanJumpgate(FLEET *lpfl) {
    HS     *lphs;
    int16_t chs;
    int16_t i;
    int16_t j;

L_7960:
    i = 0x0;
    goto L_7a52;

L_7971:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_7a4e;
    else
        goto L_798e;

L_798e:
    /* untranslated: lphs = &part[58:0](rglpshdef[lpfl->iPlayer][i].hul) */
    chs = rglpshdef[lpfl->iPlayer][i].hul.chs;
    j = 0x0;
    goto L_79f6;

L_79e3:
    j = (j + 0x1);
    lphs = (lphs + 0x1);

L_79f6:
    if ((j >= chs))
        goto L_7a3d;
    else
        goto L_7a01;

L_7a01:
    if ((lphs->cItem == 0x0))
        goto L_79e3;
    else
        goto L_7a18;

L_7a18:
    if ((lphs->grhst != hstSpecialM))
        goto L_79e3;
    else
        goto L_7a25;

L_7a25:
    if ((lphs->iItem == ispecialMJumpGate))
        goto L_7a3d;
    else
        goto L_7a34;

L_7a34:

L_7a3d:
    if ((j != chs))
        goto L_7a4e;
    else
        goto L_7a48;

L_7a48:
    return 0x0;

L_7a4e:
    i = (i + 0x1);

L_7a52:
    if ((i < 0x10))
        goto L_7971;
    else
        goto L_7a5b;

L_7a5b:
    return 0x1;
}

int32_t WtFromLpfl(FLEET *lpfl) {
    int32_t cMass;
    int16_t i;

L_7a68:
    cMass = 0x0;
    i = 0x0;
    goto L_7aef;

L_7a83:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_7aeb;
    else
        goto L_7aa0;

L_7aa0:
    /* untranslated: cMass = (cMass + (uint32_t)(sext16to32(lpfl->rgcsh[i]) * words(0x0, rglpshdef[lpfl->iPlayer][i].hul.wtEmpty))) */

L_7aeb:
    i = (i + 0x1);

L_7aef:
    if ((i < 0x10))
        goto L_7a83;
    else
        goto L_7af8;

L_7af8:
    i = 0x0;
    goto L_7b27;

L_7b00:
    cMass = (cMass + lpfl->rgwtMin[i]);
    i = (i + 0x1);

L_7b27:
    if ((i <= 0x3))
        goto L_7b00;
    else
        goto L_7b30;

L_7b30:

L_7b39:
    return cMass;
}

int16_t FCanBuildShdef(SHDEF *lpshdef, int16_t iplr) {
    int16_t j;
    int16_t iplrSav;
    PART    part;

L_7b40:
    iplrSav = idPlayer;
    if ((lpshdef->hul.ihuldef < ihuldefOrbitalFort))
        goto L_7b81;
    else
        goto L_7b5b;

L_7b5b:
    part.hs.grhst = hstSBHull;
    HIWORD(part) = ((HIWORD(part) & 0xff00) | ((lpshdef->hul.ihuldef + 0xffe0) & 0xff));
    goto L_7ba1;

L_7b81:
    part.hs.grhst = hstHull;
    HIWORD(part) = ((HIWORD(part) & 0xff00) | (lpshdef->hul.ihuldef & 0xff));

L_7ba1:
    idPlayer = iplr;
    if ((FLookupPart(&(part)) == 0x1))
        goto L_7bc7;
    else
        goto LFail;

LFail:
    idPlayer = iplrSav;
    return 0x0;

L_7bc7:
    j = 0x0;
    goto L_7bd3;

L_7bcf:
    j = (j + 0x1);

L_7bd3:
    if ((j >= lpshdef->hul.chs))
        goto L_7c4c;
    else
        goto L_7be5;

L_7be5:
    /* untranslated: branch ((lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) <= 0x0 ? L_7bcf : L_7c0f */

L_7c0f:
    part.hs.grhst = lpshdef->hul.rghs[j].grhst;
    /* untranslated: HIWORD(part) = lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] */
    if ((FLookupPart(&(part)) != 0x1))
        goto LFail;
    else
        goto L_7c43;

L_7c43:

L_7c4c:
    idPlayer = iplrSav;
    return 0x1;
}

int16_t IshFindSimilarDesign(HUL *lphul, int16_t iPlrDst) {
    SHDEF  *lpshdefDest;
    int16_t i;
    int16_t j;

L_7c5e:
    lpshdefDest = rglpshdef[iPlrDst];
    i = 0x0;
    goto L_7c98;

L_7c84:
    i = (i + 0x1);
    /* untranslated: LOWORD(lpshdefDest) = part[3:0](game.szName[LOWORD(lpshdefDest)]) */

L_7c98:
    if ((i >= 0x10))
        goto L_7e37;
    else
        goto L_7ca1;

L_7ca1:
    if ((lpshdefDest->fFree != 0x0))
        goto L_7c84;
    else
        goto L_7cb8;

L_7cb8:
    if ((lpshdefDest->fGift == 0x0))
        goto L_7c84;
    else
        goto L_7ccf;

L_7ccf:
    if ((lpshdefDest->hul.ihuldef != lphul->ihuldef))
        goto L_7c84;
    else
        goto L_7ce0;

L_7ce0:
    if ((lpshdefDest->hul.chs != lphul->chs))
        goto L_7c84;
    else
        goto L_7d00;

L_7d00:

L_7d06:
    j = 0x0;
    goto L_7d12;

L_7d0e:
    j = (j + 0x1);

L_7d12:
    if ((j >= lphul->chs))
        goto L_7e1c;
    else
        goto L_7d24;

L_7d24:
    /* untranslated: branch ((lphul->rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) != ((lpshdefDest->hul.rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) &
     * 0xff) ? L_7e1c : L_7d6d */

L_7d6d:

L_7d73:
    /* untranslated: branch ((lphul->rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) <= 0x0 ? L_7d0e : L_7d9d */

L_7d9d:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) != (lpshdefDest->hul.rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) ? L_7e1c :
     * L_7ddf */

L_7ddf:
    if ((lphul->rghs[j].grhst != lpshdefDest->hul.rghs[j].grhst))
        goto L_7e1c;
    else
        goto L_7e13;

L_7e13:

L_7e1c:
    if ((j != lphul->chs))
        goto L_7c84;
    else
        goto L_7e2e;

L_7e2e:
    return i;

L_7e37:
    return 0xffff;
}

void DrawPlanetPrintDot(HDC hdc, int16_t x, int16_t y, int16_t iSize) {
L_7e44:
    if ((iSize != 0x0))
        goto L_7ecb;
    else
        goto L_7e56;

L_7e56:
    PatBlt(hdc, (x + 0xfffd), (y + 0xffff), 0x7, 0x3, BLACKNESS);
    PatBlt(hdc, (x + 0xffff), (y + 0xfffd), 0x3, 0x7, BLACKNESS);
    PatBlt(hdc, (x + 0xfffe), (y + 0xfffe), 0x5, 0x5, BLACKNESS);
    goto L_7f63;

L_7ecb:
    PatBlt(hdc, (x + 0xfffb), (y + 0xfffe), 0xb, 0x5, BLACKNESS);
    PatBlt(hdc, (x + 0xfffe), (y + 0xfffb), 0x5, 0xb, BLACKNESS);
    PatBlt(hdc, (x + 0xfffc), (y + 0xfffd), 0x9, 0x7, BLACKNESS);
    PatBlt(hdc, (x + 0xfffd), (y + 0xfffc), 0x7, 0x9, BLACKNESS);

L_7f63:
    return;
}

void ClearFile(int16_t dt) {
    char *pch;
    char  szFile[256];

L_7f6a:
    strcpy(szFile, 0x56a2);
    pch = strrchr(szFile, 0x2e);
    if ((pch == 0x0))
        goto L_7fab;
    else
        goto L_7fa1;

L_7fa1:
    pch[0x1] = 0x0;
    goto L_7fbc;

L_7fab:
    strcat(szFile, 0x562);

L_7fbc:
    strcat(szFile, mpdtsz[dt]);
    remove(szFile);
    return;
}

void OutputSz(int16_t dt, char *sz) {
    char szTime[100];
    char szFile[256];
    char szDate[100];
    char szTemp[256];

L_7fe6:
    _wsprintf(&(szFile), "%s.%s", 0x56a2, 0x25, mpdtsz[dt], 0x25);
    if ((access(szFile, 0x0) != 0xffff))
        goto L_8065;
    else
        goto L_8033;

L_8033:
    _wsprintf(&(szTemp), "Stars! %s\r\n\r\n", SzVersion(), 0x25);
    OutputFileString(szFile, szTemp);

L_8065:
    strdate(szDate);
    strtime(szTime);
    /* untranslated: call _wsprintf(&szTemp, "%s %s - %s\r\n", szDate, ss, szTime, ss, LOWORD(sz), HIWORD(sz)) -> callresult(int16_t) */
    OutputFileString(szFile, szTemp);
    return;
}

void TurnLog(StringId ids) {
    char szTemp[256];

L_80c2:
    if ((ini.fLogging == 0x0))
        goto L_8120;
    else
        goto L_80de;

L_80de:
    _wsprintf(&(szTemp), PszFormatIds(ids, 0x0), (game.turn + 0x961));
    OutputSz(0x6, &(szTemp));

L_8120:
    return;
}
