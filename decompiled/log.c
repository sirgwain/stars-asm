#include "common.h"

void LogSplitFleet(int16_t id) {
L_8b0e:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_8b52;
    else
        goto L_8b27;

L_8b27:
    LpflFromId(id);
    goto L_8b66;

L_8b52:
    WriteMemRt(24, 2, &(id));

L_8b66:
    return;
}

void LogMergeFleet(int16_t id) {
    uint16_t idCur;
    int16_t  i;
    uint16_t rgid[512];
    int16_t  j;

L_8b6c:
    if ((gd.fGeneratingTurn != 0x0))
        goto L_8c21;
    else
        goto L_8b88;

L_8b88:
    i = 0;
    goto L_8c16;

L_8b90:
    rgid[0] = id;
    j = 1;

L_8b9d:
    if ((j >= 511))
        goto L_8bfe;
    else
        goto L_8ba8;

L_8ba8:
    if ((i >= vcflMerge))
        goto L_8bfe;
    else
        goto L_8bb3;

L_8bb3:
    if ((vrgiflMerge[i] == -1))
        goto L_8bf7;
    else
        goto L_8bc6;

L_8bc6:
    idCur = vrgiflMerge[i];
    if ((idCur == id))
        goto L_8bf7;
    else
        goto L_8be1;

L_8be1:
    j = (j + 1);
    rgid[j] = idCur;

L_8bf7:
    i = (i + 1);
    goto L_8b9d;

L_8bfe:
    WriteMemRt(37, (j * 2), &(rgid));

L_8c16:
    if ((i < vcflMerge))
        goto L_8b90;
    else
        goto L_8c21;

L_8c21:
    return;
}

void LogChangeShDef(SHDEF *lpshdefNew) {
    uint8_t  rgb[149];
    uint8_t *pb;

L_8c28:
    if ((gd.fGeneratingTurn != 0x0))
        goto L_8d53;
    else
        goto L_8c44;

L_8c44:
    LOWORD(rgb) = ((LOWORD(rgb) & 0xe0ff) | ((lpshdefNew->ishdef & 0x1f) << 0x8));
    LOWORD(rgb) = ((LOWORD(rgb) & 0xff0f) | ((idPlayer & 0xf) << 0x4));
    if ((lpshdefNew->fFree == 0x0))
        goto L_8ccc;
    else
        goto L_8ca6;

L_8ca6:
    LOWORD(rgb) = ((LOWORD(rgb) & 0xfff0) | 0x0);
    WriteMemRt(27, 2, &(rgb));
    goto L_8d25;

L_8ccc:
    LOWORD(rgb) = ((LOWORD(rgb) & 0xfff0) | 0x1);
    lpshdefNew->det = 0x7;
    pb = &(rgb[2]);
    WriteRtShDef(lpshdefNew, &(pb));
    WriteMemRt(27, (pb + (-&(rgb))), &(rgb));

L_8d25:
    if ((gd.fTutorial == 0x0))
        goto L_8d53;
    else
        goto L_8d38;

L_8d38:
    if ((idPlayer != 0))
        goto L_8d53;
    else
        goto L_8d42;

L_8d42:
    tutor.fChange = 0x1;
    AdvanceTutor();

L_8d53:
    return;
}

void LogChangeName(GrobjClass grobj, int16_t id, char *szName) {
    FLEET    *lpfl;
    int16_t   cOut;
    RTCHGNAME rtchgname;

L_8d5a:
    lpfl = LpflFromId(id);
    if ((LOWORD(lpfl) != 0x0))
        goto L_8d89;
    else
        goto L_8d7d;

L_8d7d:
    if ((HIWORD(lpfl) == 0x0))
        goto L_8eb8;
    else
        goto L_8d89;

L_8d89:
    if ((LOWORD(lpfl->lpszName) != 0x0))
        goto L_8da0;
    else
        goto L_8d96;

L_8d96:
    if ((HIWORD(lpfl->lpszName) == 0x0))
        goto L_8db7;
    else
        goto L_8da0;

L_8da0:
    FreeLp(lpfl->lpszName, htString);

L_8db7:
    if ((szName == 0x0))
        goto L_8dce;
    else
        goto L_8dc0;

L_8dc0:
    if (((uint16_t)(*(szName)) != 0))
        goto L_8ded;
    else
        goto L_8dce;

L_8dce:
    rtchgname.rgb[0] = 0x0;
    rtchgname.rgb[1] = 0x0;
    cOut = 1;
    lpfl->lpszName = 0x0;
    goto L_8e7d;

L_8ded:
    cOut = strlen(szName);
    lpfl->lpszName = LpAlloc((strlen(szName) + 0x1), htString);
    fstrcpy(lpfl->lpszName, szName);
    if ((FCompressUserString(szName, &(rtchgname.rgb[1]), &(cOut)) == 0))
        goto L_8e66;
    else
        goto L_8e5d;

L_8e5d:
    rtchgname.rgb[0] = LOBYTE(cOut);
    goto L_8e7d;

L_8e66:
    rtchgname.rgb[0] = 0x0;
    strcpy(&(rtchgname.rgb[1]), szName);
    cOut = (cOut + 1);

L_8e7d:
    rtchgname.grobj = grobj;
    rtchgname.id = id;
    WriteMemRt(44, (cOut + 5), &(rtchgname));
    if ((gd.fTutorial == 0x0))
        goto L_8eb8;
    else
        goto L_8eb3;

L_8eb3:
    AdvanceTutor();

L_8eb8:
    return;
}

void LogChangeFleet(FLEET *pfl, FLEET *pflNew) {
    int16_t   d;
    int16_t   i;
    int16_t   fChg;
    LOGXFERF  lxfNew;
    LOGXFER   lxNew;
    RTWAYPT   rtwp;
    RTSHIPINT rtsi;
    int16_t   iordNew;
    int16_t   iordOld;
    int16_t   cbWp;
    char     *pbWp;
    HDR       hdr;
    uint16_t  t_scratch_m28;
    int16_t   t_9229;
    int16_t   t_92f8;

L_8ebe:
    fChg = 0;
    if ((gd.fGeneratingTurn != 0x0))
        goto L_933a;
    else
        goto L_8edf;

L_8edf:
    lxfNew.id = pfl->id;
    lxfNew.grobj = grobjFleet;
    i = 0;
    goto L_8f3a;

L_8ef5:
    lxfNew.rgdItem[i] = (pflNew->rgcsh[i] - pfl->rgcsh[i]);
    if (((pflNew->rgcsh[i] - pfl->rgcsh[i]) == 0))
        goto L_8f36;
    else
        goto L_8f31;

L_8f31:
    fChg = 1;

L_8f36:
    i = (i + 1);

L_8f3a:
    if ((i < 16))
        goto L_8ef5;
    else
        goto L_8f43;

L_8f43:
    if ((fChg == 0))
        goto L_8f88;
    else
        goto L_8f4c;

L_8f4c:
    if ((fValidLxf == 0))
        goto L_8f6f;
    else
        goto L_8f56;

L_8f56:
    LogMakeValidXferf(lxf.id, &(lxfNew));
    fValidLxf = 0;
    goto L_933a;

L_8f6f:
    lxf = lxfNew;
    fValidLxf = 1;

L_8f85:
    goto L_933a;

L_8f88:
    lxNew.id = pfl->id;
    lxNew.grobj = grobjFleet;
    i = 0;
    goto L_8ffb;

L_8f9e:
    LOWORD(lxNew.rgdItem[i]) = (LOWORD(pflNew->rgwtMin[i]) - LOWORD(pfl->rgwtMin[i]));
    HIWORD(lxNew.rgdItem[i]) = (HIWORD(pflNew->rgwtMin[i]) - HIWORD(pfl->rgwtMin[i]));
    if (((LOWORD(pflNew->rgwtMin[i]) - LOWORD(pfl->rgwtMin[i])) != 0x0))
        goto L_8ff2;
    else
        goto L_8fea;

L_8fea:
    if (((HIWORD(pflNew->rgwtMin[i]) - HIWORD(pfl->rgwtMin[i])) == 0x0))
        goto L_8ff7;
    else
        goto L_8ff2;

L_8ff2:
    fChg = 1;

L_8ff7:
    i = (i + 1);

L_8ffb:
    if ((i < 5))
        goto L_8f9e;
    else
        goto L_9004;

L_9004:
    if ((fChg == 0))
        goto L_9049;
    else
        goto L_900d;

L_900d:
    if ((fValidLx == 0))
        goto L_9030;
    else
        goto L_9017;

L_9017:
    LogMakeValidXfer(lx.id, &(lxNew));
    fValidLx = 0;
    goto L_933a;

L_9030:
    lx = lxNew;
    fValidLx = 1;

L_9046:
    goto L_933a;

L_9049:
    t_scratch_m28 = pfl->iplan;
    if ((t_scratch_m28 == pflNew->iplan))
        goto L_9093;
    else
        goto L_906b;

L_906b:
    rtsi.id = pflNew->id;
    rtsi.i = pflNew->iplan;
    WriteMemRt(42, 4, &(rtsi));

L_9093:
    if ((pfl->fRepOrders == pflNew->fRepOrders))
        goto L_90e4;
    else
        goto L_90b7;

L_90b7:
    rtsi.id = pflNew->id;
    rtsi.i = pflNew->fRepOrders;
    WriteMemRt(10, 4, &(rtsi));

L_90e4:
    d = (pflNew->cord - pfl->cord);
    iordOld = 0;
    goto L_9100;

L_90fc:
    iordOld = (iordOld + 1);

L_9100:
    if ((iordOld >= pfl->cord))
        goto L_9181;
    else
        goto L_910f;

L_910f:
    if ((iordOld >= pflNew->cord))
        goto L_9181;
    else
        goto L_911d;

L_911d:
    if ((fmemcmp(&(pfl->lpplord->rgord[iordOld]), &(pflNew->lpplord->rgord[iordOld]), 0x12) == 0))
        goto L_90fc;
    else
        goto L_9181;

L_9181:
    iordNew = iordOld;
    if ((iordOld != pfl->cord))
        goto L_91a2;
    else
        goto L_9196;

L_9196:
    if ((d == 0))
        goto L_933a;
    else
        goto L_91a2;

L_91a2:
    if ((d >= 0))
        goto L_91de;
    else
        goto L_91ab;

L_91ab:
    rtsi.id = pflNew->id;
    rtsi.i = iordOld;
    if ((d != -2))
        goto L_91c7;
    else
        goto L_91c2;

L_91c2:
    rtsi.i = (rtsi.i | 0x8000);

L_91c7:
    WriteMemRt(3, 4, &(rtsi));
    goto L_933a;

L_91de:
    if ((d <= 0))
        goto L_926b;
    else
        goto L_91e7;

L_91e7:
    cbWp = 22;
    rtwp.id = pflNew->id;
    rtwp.iWaypt = iordNew;
    rtwp.order = pflNew->lpplord->rgord[iordNew];
    pbWp = &(rtwp);

L_9229:
    t_9229 = cbWp;
    cbWp = (cbWp - 1);
    if ((t_9229 <= 0))
        goto L_9251;
    else
        goto L_9238;

L_9238:
    if (((uint16_t)(pbWp[cbWp]) == 0))
        goto L_9229;
    else
        goto L_9251;

L_9251:
    cbWp = (cbWp + 1);
    WriteMemRt(4, cbWp, &(rtwp));
    goto L_933a;

L_926b:
    cbWp = 22;
    if ((FGetPrevLogRt(&(hdr), rgbCur) == 0))
        goto L_92bb;
    else
        goto L_9288;

L_9288:
    if ((hdr.rt != rtLogFleetOrderUpdate))
        goto L_92bb;
    else
        goto L_929b;

L_929b:
    if ((LOWORD(rgbCur) != pflNew->id))
        goto L_92bb;
    else
        goto L_92a9;

L_92a9:
    if ((rgbCur[2] != iordNew))
        goto L_92bb;
    else
        goto L_92b5;

L_92b5:
    imemLogCur = imemLogPrev;

L_92bb:
    rtwp.id = pflNew->id;
    rtwp.iWaypt = iordNew;
    rtwp.order = pflNew->lpplord->rgord[iordNew];
    pbWp = &(rtwp);

L_92f8:
    t_92f8 = cbWp;
    cbWp = (cbWp - 1);
    if ((t_92f8 <= 0))
        goto L_9320;
    else
        goto L_9307;

L_9307:
    if (((uint16_t)(pbWp[cbWp]) == 0))
        goto L_92f8;
    else
        goto L_9320;

L_9320:
    cbWp = (cbWp + 1);
    WriteMemRt(5, cbWp, &(rtwp));

L_933a:
    return;
}

void LogChangeRelations() {
    HDR hdr;

L_9340:
    if ((FGetPrevLogRt(&(hdr), rgbCur) == 0))
        goto L_937a;
    else
        goto L_9361;

L_9361:
    if ((hdr.rt != rtLogRelations))
        goto L_937a;
    else
        goto L_9374;

L_9374:
    imemLogCur = imemLogPrev;

L_937a:
    WriteMemRt(38, game.cPlayer, ((0x59a2 + LOWORD((192 * idPlayer))) + 0x70));
    if ((gd.fTutorial == 0x0))
        goto L_93ca;
    else
        goto L_93af;

L_93af:
    if ((idPlayer != 0))
        goto L_93ca;
    else
        goto L_93b9;

L_93b9:
    tutor.fChange = 0x1;
    AdvanceTutor();

L_93ca:
    return;
}

void LogChangeBtlplan(BTLPLAN *pbtlplan) {
L_93d0:
    WriteBattlePlan(pbtlplan, 1);
    if ((gd.fTutorial == 0x0))
        goto L_941a;
    else
        goto L_93ff;

L_93ff:
    if ((idPlayer != 0))
        goto L_941a;
    else
        goto L_9409;

L_9409:
    tutor.fChange = 0x1;
    AdvanceTutor();

L_941a:
    return;
}

void LogChangePlanet(PLANET *ppl, PLANET *pplNew) {
    int16_t  i;
    int16_t  fChg;
    HDR      hdr;
    LOGXFER  lxNew;
    uint16_t t_scratch_m22;
    uint16_t t_scratch_m28;

L_9420:
    fChg = 0;
    if ((gd.fGeneratingTurn != 0x0))
        goto L_9901;
    else
        goto L_9441;

L_9441:
    if ((LOWORD(ppl) != 0x0))
        goto L_94ab;
    else
        goto L_944a;

L_944a:
    if ((HIWORD(ppl) != 0x0))
        goto L_94ab;
    else
        goto L_9453;

L_9453:
    if ((fValidLx == 0))
        goto L_9901;
    else
        goto L_9460;

L_9460:
    lxNew.id = -1;
    lxNew.grobj = grobjOther;
    i = 0;
    goto L_949f;

L_9472:
    LOWORD(lxNew.rgdItem[i]) = (-LOWORD(lx.rgdItem[i]));
    HIWORD(lxNew.rgdItem[i]) = (-(HIWORD(lx.rgdItem[i]) + 0x0));
    i = (i + 1);

L_949f:
    if ((i >= 5))
        goto ChgIt;
    else
        goto L_94a5;

L_94a5:
    goto L_9472;

L_94ab:
    lxNew.id = ppl->id;
    lxNew.grobj = grobjPlanet;
    i = 0;
    goto L_951e;

L_94c1:
    LOWORD(lxNew.rgdItem[i]) = (LOWORD(pplNew->rgwtMin[i]) - LOWORD(ppl->rgwtMin[i]));
    HIWORD(lxNew.rgdItem[i]) = (HIWORD(pplNew->rgwtMin[i]) - HIWORD(ppl->rgwtMin[i]));
    if (((LOWORD(pplNew->rgwtMin[i]) - LOWORD(ppl->rgwtMin[i])) != 0x0))
        goto L_9515;
    else
        goto L_950d;

L_950d:
    if (((HIWORD(pplNew->rgwtMin[i]) - HIWORD(ppl->rgwtMin[i])) == 0x0))
        goto L_951a;
    else
        goto L_9515;

L_9515:
    fChg = 1;

L_951a:
    i = (i + 1);

L_951e:
    if ((i < 4))
        goto L_94c1;
    else
        goto L_9527;

L_9527:
    lxNew.rgdItem[4] = 0;
    if ((fChg == 0))
        goto L_9573;
    else
        goto ChgIt;

ChgIt:
    if ((fValidLx == 0))
        goto L_955d;
    else
        goto L_9544;

L_9544:
    LogMakeValidXfer(lx.id, &(lxNew));
    fValidLx = 0;
    goto L_9573;

L_955d:
    lx = lxNew;
    fValidLx = 1;

L_9573:
    if ((LOWORD(ppl) != 0x0))
        goto L_9588;
    else
        goto L_957c;

L_957c:
    if ((HIWORD(ppl) == 0x0))
        goto L_9901;
    else
        goto L_9588;

L_9588:
    if ((LOWORD(pplNew->lpplprod) != 0x0))
        goto L_95cb;
    else
        goto L_9594;

L_9594:
    if ((HIWORD(pplNew->lpplprod) != 0x0))
        goto L_95cb;
    else
        goto L_959d;

L_959d:
    if ((LOWORD(ppl->lpplprod) != 0x0))
        goto L_95b4;
    else
        goto L_95aa;

L_95aa:
    if ((HIWORD(ppl->lpplprod) == 0x0))
        goto L_95cb;
    else
        goto L_95b4;

L_95b4:
    WriteMemRt(29, 2, &(lxNew));
    goto L_9705;

L_95cb:
    if ((LOWORD(pplNew->lpplprod) != 0x0))
        goto L_95e0;
    else
        goto L_95d7;

L_95d7:
    if ((HIWORD(pplNew->lpplprod) == 0x0))
        goto L_9705;
    else
        goto L_95e0;

L_95e0:
    if ((LOWORD(ppl->lpplprod) != 0x0))
        goto L_95f7;
    else
        goto L_95ed;

L_95ed:
    if ((HIWORD(ppl->lpplprod) == 0x0))
        goto L_9666;
    else
        goto L_95f7;

L_95f7:
    t_scratch_m22 = ppl->lpplprod->iprodMac;
    if ((t_scratch_m22 != pplNew->lpplprod->iprodMac))
        goto L_9666;
    else
        goto L_9621;

L_9621:
    if ((fmemcmp(ppl->lpplprod->rgprod, pplNew->lpplprod->rgprod, (ppl->lpplprod->iprodMac * 0x4)) == 0))
        goto L_9705;
    else
        goto L_9666;

L_9666:
    if ((FGetPrevLogRt(&(hdr), rgbCur) == 0))
        goto L_96a6;
    else
        goto L_967e;

L_967e:
    if ((hdr.rt != rtLogPlanetProdQ))
        goto L_96a6;
    else
        goto L_9691;

L_9691:
    if ((LOWORD(rgbCur) != ppl->id))
        goto L_96a6;
    else
        goto L_96a0;

L_96a0:
    imemLogCur = imemLogPrev;

L_96a6:
    LOWORD(rgbCur) = ppl->id;
    fmemmove(rgbCur[2], pplNew->lpplprod->rgprod, (pplNew->lpplprod->iprodMac * 0x4));
    WriteMemRt(29, ((pplNew->lpplprod->iprodMac * 4) + 2), rgbCur);

L_9705:
    t_scratch_m28 = ppl->fNoResearch;
    if ((t_scratch_m28 != pplNew->fNoResearch))
        goto L_97b5;
    else
        goto L_9754;

L_9754:
    if ((0x0 != 0x0))
        goto L_97b5;
    else
        goto L_975b;

L_975b:
    if ((ppl->idFling != pplNew->idFling))
        goto L_97b5;
    else
        goto L_9776;

L_9776:
    if ((ppl->iWarpFling != pplNew->iWarpFling))
        goto L_97b5;
    else
        goto L_979a;

L_979a:
    if ((ppl->idRoute == pplNew->idRoute))
        goto L_9901;
    else
        goto L_97b5;

L_97b5:
    /* untranslated: part[0x0:4](rgbCur) = (uint32_t)pplNew->id */
    rgbCur[4] = 0;
    rgbCur[2] = ((rgbCur[0x2] & 0xfffffffe) | (int32_t)(((uint32_t)((LOWORD((uint32_t)(pplNew->fNoResearch)) & 0x1)) << 0x0)));
    rgbCur[2] = ((rgbCur[0x2] & 0xfffff801) | (int32_t)(((uint32_t)((LOWORD((uint32_t)(pplNew->idFling)) & 0x3ff)) << 0x1)));
    rgbCur[2] = ((rgbCur[0x2] & 0xffff87ff) | (int32_t)(((uint32_t)((LOWORD((uint32_t)(pplNew->iWarpFling)) & 0xf)) << 0xb)));
    rgbCur[2] = ((rgbCur[0x2] & 0xfe007fff) | (int32_t)(((uint32_t)((LOWORD((uint32_t)(pplNew->idRoute)) & 0x3ff)) << 0xf)));
    WriteMemRt(35, 6, rgbCur);

L_9901:
    return;
}

void LogChangeThing(THING *lpth, THING *pthNew) {
    int16_t i;
    int16_t fChg;
    LOGXFER lxNew;

L_9908:
    fChg = 0;
    if ((gd.fGeneratingTurn != 0x0))
        goto L_99f0;
    else
        goto L_9929;

L_9929:
    memset(&(lxNew), 0, 0x18);
    lxNew.id = pthNew->idFull;
    lxNew.grobj = grobjThing;
    i = 0;
    goto L_99a5;

L_9952:
    LOWORD(lxNew.rgdItem[i]) = (pthNew->thp.rgwtMin[i] - lpth->thp.rgwtMin[i]);
    HIWORD(lxNew.rgdItem[i]) = SIGNHIWORD((pthNew->thp.rgwtMin[i] - lpth->thp.rgwtMin[i]));
    if (((pthNew->thp.rgwtMin[i] - lpth->thp.rgwtMin[i]) != 0))
        goto L_999c;
    else
        goto L_9994;

L_9994:
    if ((SIGNHIWORD((pthNew->thp.rgwtMin[i] - lpth->thp.rgwtMin[i])) == 0x0))
        goto L_99a1;
    else
        goto L_999c;

L_999c:
    fChg = 1;

L_99a1:
    i = (i + 1);

L_99a5:
    if ((i < 3))
        goto L_9952;
    else
        goto L_99ae;

L_99ae:
    if ((fChg == 0))
        goto L_99f0;
    else
        goto L_99b7;

L_99b7:
    if ((fValidLx == 0))
        goto L_99da;
    else
        goto L_99c1;

L_99c1:
    LogMakeValidXfer(lx.id, &(lxNew));
    fValidLx = 0;
    goto L_99f0;

L_99da:
    lx = lxNew;
    fValidLx = 1;

L_99f0:
    return;
}

void LogMakeValidXfer(LOGXFER *plx1, LOGXFER *plx2) {
    int32_t  rgQuan[5];
    RTXFER  *prt;
    int16_t  iOff;
    RTXFERL *prtl;
    int16_t  rt;
    int16_t  i;
    char     rgbuf[28];
    int16_t  grbit;
    RTXFERX *prtx;
    int16_t  grFlag;
    int32_t  iBiggest;
    int16_t  cb;
    uint16_t t_scratch_m50;
    uint16_t t_scratch_m50_2;
    int32_t  t_call_9cf5;
    int32_t  t_merge_9d34_0001;
    int32_t  t_call_9d2c;

L_99f6:
    iBiggest = 0;
    grbit = 0;
    grFlag = 1;
    rgQuan[0] = 0;
    rgQuan[1] = 0;
    rgQuan[2] = 0;
    rgQuan[3] = 0;
    rgQuan[4] = 0;
    goto L_9a7e;

L_9a53:
    prt = (lpLog + (imemLogCur + (-hdrPrev.cb)));
    goto L_9a99;

L_9a71:
    prt = 0x0;
    goto L_9a99;

L_9a7e:
    if ((hdrPrev.rt == rtLogCargoXfer8))
        goto L_9a53;
    else
        goto L_9a86;

L_9a86:
    if ((hdrPrev.rt == rtLogCargoXfer16))
        goto L_9a53;
    else
        goto L_9a8e;

L_9a8e:
    if ((hdrPrev.rt != rtLogCargoXfer32))
        goto L_9a71;
    else
        goto L_9a93;

L_9a93:
    goto L_9a53;

L_9a99:
    if ((LOWORD(prt) != 0x0))
        goto L_9aab;
    else
        goto L_9aa2;

L_9aa2:
    if ((HIWORD(prt) == 0x0))
        goto L_9c94;
    else
        goto L_9aab;

L_9aab:
    t_scratch_m50 = prt->grobj1;
    if ((t_scratch_m50 != (plx1->grobj & 0xff)))
        goto L_9c94;
    else
        goto L_9ad0;

L_9ad0:
    t_scratch_m50_2 = prt->grobj2;
    if ((t_scratch_m50_2 != (plx2->grobj & 0xff)))
        goto L_9c94;
    else
        goto L_9afd;

L_9afd:
    if ((prt->id1 != plx1->id))
        goto L_9c94;
    else
        goto L_9b0d;

L_9b0d:
    if ((prt->id2 != plx2->id))
        goto L_9c94;
    else
        goto L_9b1e;

L_9b1e:
    grbit = prt->grbitItems;
    iOff = 0;
    goto L_9c65;

L_9b3e:
    i = 0;
    goto L_9b89;

L_9b46:
    if ((((0x1 << i) & grbit) == 0x0))
        goto L_9b85;
    else
        goto L_9b59;

L_9b59:
    rgQuan[i] = (uint32_t)((uint16_t)(prt->rgcQuan[iOff]));
    iOff = (iOff + 1);

L_9b85:
    i = (i + 1);

L_9b89:
    if ((i >= 5))
        goto L_9c80;
    else
        goto L_9b8f;

L_9b8f:
    goto L_9b46;

L_9b95:
    prtx = prt;
    i = 0;
    goto L_9bed;

L_9ba9:
    if ((((0x1 << i) & grbit) == 0x0))
        goto L_9be9;
    else
        goto L_9bbc;

L_9bbc:
    rgQuan[i] = (uint32_t)(prtx->rgcQuan[iOff]);
    iOff = (iOff + 1);

L_9be9:
    i = (i + 1);

L_9bed:
    if ((i >= 5))
        goto L_9c80;
    else
        goto L_9bf3;

L_9bf3:
    goto L_9ba9;

L_9bf9:
    prtl = prt;
    i = 0;
    goto L_9c56;

L_9c0d:
    if ((((0x1 << i) & grbit) == 0x0))
        goto L_9c52;
    else
        goto L_9c20;

L_9c20:
    rgQuan[i] = prtl->rgcQuan[iOff];
    iOff = (iOff + 1);

L_9c52:
    i = (i + 1);

L_9c56:
    if ((i >= 5))
        goto L_9c80;
    else
        goto L_9c5c;

L_9c5c:
    goto L_9c0d;

L_9c65:
    if ((hdrPrev.rt == rtLogCargoXfer8))
        goto L_9b3e;
    else
        goto L_9c6d;

L_9c6d:
    if ((hdrPrev.rt == rtLogCargoXfer16))
        goto L_9b95;
    else
        goto L_9c75;

L_9c75:
    if ((hdrPrev.rt == rtLogCargoXfer32))
        goto L_9bf9;
    else
        goto L_9c80;

L_9c80:
    CancelMemRt(hdrPrev.rt);

L_9c94:
    i = 0;
    grbit = 0;
    goto L_9cb4;

L_9ca4:
    i = (i + 1);
    grFlag = (grFlag * 2);

L_9cb4:
    if ((i >= 5))
        goto L_9d60;
    else
        goto L_9cbd;

L_9cbd:
    rgQuan[i] = (rgQuan[i] + plx1->rgdItem[i]);
    t_call_9cf5 = labs(rgQuan[i]);
    if ((HIWORD(iBiggest) < HIWORD(t_call_9cf5)))
        goto L_9d1b;
    else
        goto L_9d05;

L_9d05:
    if ((HIWORD(iBiggest) > HIWORD(t_call_9cf5)))
        goto L_9d12;
    else
        goto L_9d0a;

L_9d0a:
    if ((LOWORD(iBiggest) <= LOWORD(t_call_9cf5)))
        goto L_9d1b;
    else
        goto L_9d12;

L_9d12:
    t_merge_9d34_0001 = iBiggest;
    goto L_9d34;

L_9d1b:
    t_call_9d2c = labs(rgQuan[i]);
    t_merge_9d34_0001 = t_call_9d2c;

L_9d34:
    iBiggest = t_merge_9d34_0001;
    if ((LOWORD(rgQuan[i]) != 0x0))
        goto L_9d57;
    else
        goto L_9d4e;

L_9d4e:
    if ((HIWORD(rgQuan[i]) == 0x0))
        goto L_9ca4;
    else
        goto L_9d57;

L_9d57:
    grbit = (grbit | grFlag);

L_9d5d:
    goto L_9ca4;

L_9d60:
    if ((grbit == 0))
        goto L_9fa0;
    else
        goto L_9d6c;

L_9d6c:
    prt = &(rgbuf);
    /* untranslated: part[0x4:1](prt) = lobyte(((*(prt+0x4) & 0xf0) | (lobyte(plx1->grobj) & 0xf))) */
    /* untranslated: part[0x4:1](prt) = lobyte((prt->grobj1 | ((lobyte(plx2->grobj) & 0xf) * 0x10))) */
    LOWORD(prt) = plx1->id;
    prt->id2 = plx2->id;
    prt->grbitItems = LOBYTE(grbit);
    cb = 6;
    iOff = 0;
    if ((HIWORD(iBiggest) > 0x0))
        goto L_9e89;
    else
        goto L_9e13;

L_9e13:
    if ((HIWORD(iBiggest) < 0x0))
        goto L_9e21;
    else
        goto L_9e18;

L_9e18:
    if ((LOWORD(iBiggest) > 0x7f))
        goto L_9e89;
    else
        goto L_9e21;

L_9e21:
    rt = 1;
    i = 0;
    goto L_9e7d;

L_9e2e:
    if ((LOWORD(rgQuan[i]) != 0x0))
        goto L_9e4b;
    else
        goto L_9e42;

L_9e42:
    if ((HIWORD(rgQuan[i]) == 0x0))
        goto L_9e79;
    else
        goto L_9e4b;

L_9e4b:
    iOff = (iOff + 1);
    prt->rgcQuan[iOff] = LOBYTE(LOWORD(rgQuan[i]));
    cb = (cb + 1);

L_9e79:
    i = (i + 1);

L_9e7d:
    if ((i >= 5))
        goto L_9f8e;
    else
        goto L_9e83;

L_9e83:
    goto L_9e2e;

L_9e89:
    if ((HIWORD(iBiggest) > 0x0))
        goto L_9f16;
    else
        goto L_9e92;

L_9e92:
    if ((HIWORD(iBiggest) < 0x0))
        goto L_9ea1;
    else
        goto L_9e97;

L_9e97:
    if ((LOWORD(iBiggest) > 0x7fff))
        goto L_9f16;
    else
        goto L_9ea1;

L_9ea1:
    rt = 2;
    prtx = &(rgbuf);
    i = 0;
    goto L_9f0a;

L_9eb9:
    if ((LOWORD(rgQuan[i]) != 0x0))
        goto L_9ed6;
    else
        goto L_9ecd;

L_9ecd:
    if ((HIWORD(rgQuan[i]) == 0x0))
        goto L_9f06;
    else
        goto L_9ed6;

L_9ed6:
    iOff = (iOff + 1);
    prtx->rgcQuan[iOff] = LOWORD(rgQuan[i]);
    cb = (cb + 2);

L_9f06:
    i = (i + 1);

L_9f0a:
    if ((i >= 5))
        goto L_9f8e;
    else
        goto L_9f10;

L_9f10:
    goto L_9eb9;

L_9f16:
    rt = 25;
    prtl = &(rgbuf);
    i = 0;
    goto L_9f85;

L_9f2e:
    if ((LOWORD(rgQuan[i]) != 0x0))
        goto L_9f4b;
    else
        goto L_9f42;

L_9f42:
    if ((HIWORD(rgQuan[i]) == 0x0))
        goto L_9f81;
    else
        goto L_9f4b;

L_9f4b:
    iOff = (iOff + 1);
    prtl->rgcQuan[iOff] = rgQuan[i];
    cb = (cb + 4);

L_9f81:
    i = (i + 1);

L_9f85:
    if ((i < 5))
        goto L_9f2e;
    else
        goto L_9f8e;

L_9f8e:
    WriteMemRt(rt, cb, &(rgbuf));

L_9fa0:
    return;
}

void LogMakeValidXferf(LOGXFERF *plxf1, LOGXFERF *plxf2) {
    RTXFERF *prt;
    int16_t  iOff;
    int16_t  i;
    char     rgbuf[41];
    uint16_t grbit;
    int16_t  grFlag;
    int16_t  cb;

L_9fa6:
    grbit = 0x0;
    grFlag = 1;
    i = 0;
    goto L_9fd1;

L_9fc1:
    i = (i + 1);
    grFlag = (grFlag * 2);

L_9fd1:
    if ((i >= 16))
        goto L_9ffa;
    else
        goto L_9fda;

L_9fda:
    if ((plxf1->rgdItem[i] == 0))
        goto L_9fc1;
    else
        goto L_9ff1;

L_9ff1:
    grbit = (grbit | grFlag);

L_9ff7:
    goto L_9fc1;

L_9ffa:
    if ((grbit == 0x0))
        goto L_a101;
    else
        goto L_a006;

L_a006:
    prt = &(rgbuf);
    *(prt + 0x4) = LOBYTE(((*(prt + 0x4) & 0xf0) | (LOBYTE(plxf1->grobj) & 0xf)));
    *(prt + 0x4) = LOBYTE((prt->grobj1 | ((LOBYTE(plxf2->grobj) & 0xf) * 0x10)));
    prt->id1 = plxf1->id;
    prt->id2 = plxf2->id;
    prt->grbitItems = grbit;
    cb = 7;
    iOff = 0;
    i = 0;
    goto L_a0e5;

L_a0a0:
    if ((plxf1->rgdItem[i] == 0))
        goto L_a0e1;
    else
        goto L_a0b7;

L_a0b7:
    iOff = (iOff + 1);
    prt->rgcQuan[iOff] = plxf1->rgdItem[i];
    cb = (cb + 2);

L_a0e1:
    i = (i + 1);

L_a0e5:
    if ((i < 16))
        goto L_a0a0;
    else
        goto L_a0ee;

L_a0ee:
    WriteMemRt(23, cb, &(rgbuf));

L_a101:
    return;
}

void CancelMemRt(RecordType rt) {
L_a108:
    imemLogCur = (imemLogCur - (hdrPrev.cb + 2));
    hdrPrev.rt = rtEOF;
    return;
}

void WriteMemRt(int16_t rt, int16_t cb, void *rg) {
    HDR      hdr;
    uint8_t *lpv;

L_a130:
    if ((fLogOff != 0))
        goto L_a221;
    else
        goto L_a146;

L_a146:
    if ((((imemLogCur + cb) + 2) <= 32000))
        goto L_a178;
    else
        goto L_a157;

L_a157:
    AlertSz(PszFormatIds(idsLogFileHasReachedMaximumAllowableSize, 0x0), MB_ICONHAND);

L_a178:
    DirtyGame(1);
    imemLogPrev = imemLogCur;
    hdr.cb = cb;
    hdr.rt = rt;
    lpv = lpLog;
    lpv = (lpv + imemLogCur);
    LOWORD(lpv) = hdr;
    if ((cb <= 0))
        goto L_a208;
    else
        goto L_a1e8;

L_a1e8:
    fmemcpy(lpv[2], ((uint8_t *)(rg) + 0), cb);

L_a208:
    imemLogCur = (imemLogCur + (cb + 2));
    if ((rt == 0))
        goto L_a221;
    else
        goto L_a21b;

L_a21b:
    hdrPrev = hdr;

L_a221:
    return;
}

void DirtyGame(int16_t fDirty) {
L_a228:
    if ((fDirty == game.fDirty))
        goto L_a258;
    else
        goto L_a23c;

L_a23c:
    game.fDirty = fDirty;
    if ((fAi != 0))
        goto L_a258;
    else
        goto L_a24c;

L_a24c:
    SetMsgTitle(hwndMessage);

L_a258:
    return;
}

int16_t FGetPrevLogRt(HDR *phdr, uint8_t *pb) {
    uint8_t *lpv;

L_a25e:
    if ((imemLogPrev != -1))
        goto L_a277;
    else
        goto L_a271;

L_a271:
    return 0;

L_a277:
    lpv = (lpLog + imemLogPrev);
    *(phdr) = LOWORD(lpv);
    if ((phdr->cb <= 0x0))
        goto L_a2ca;
    else
        goto L_a2a5;

L_a2a5:
    fmemcpy(pb, lpv[2], phdr->cb);

L_a2ca:
    return 1;
}

int16_t FRunLogFile() {
    int16_t fLogOld;
    int16_t fRet;
    int16_t iCur;
    HDR    *lprts;

L_a2d6:
    iCur = 0;
    fRet = 1;
    fLogOld = fLogOff;
    if ((imemLogCur != 0))
        goto L_a2ff;
    else
        goto L_a2f9;

L_a2f9:
    return 1;

L_a2ff:
    fLogOff = 1;
    goto L_a362;

L_a308:
    lprts = (lpLog + iCur);
    fRet = (fRet & FRunLogRecord(lprts->rt, lprts->cb, (lpLog + (iCur + 0x2))));
    iCur = (iCur + (lprts->cb + 2));

L_a362:
    if ((iCur < imemLogCur))
        goto L_a308;
    else
        goto L_a36d;

L_a36d:
    fLogOff = fLogOld;
    gd.fFleetLinkValid = 0x0;
    return fRet;
}

int16_t FRunLogRecord(RecordType rt, int16_t cb, uint8_t *lpb) {
    int16_t   fExtra;
    int32_t   cXfer;
    XFERFULL *lpxfCur;
    PLANET   *lppl;
    int32_t   rgcXfer[5];
    XFER      rgxf[2];
    FLEET    *lpfl;
    int16_t   ifl;
    int16_t   i;
    uint16_t  grbit;
    int16_t   rgifl[512];
    SHDEF    *lpshdef;
    int16_t   iPass;
    int16_t   iLook;
    PLANET   *lpplMac;
    char      ch;
    int32_t   l;
    char      szT[33];
    int16_t   cOut;
    THING    *lpth;
    int16_t   id;
    int16_t   iColDrop;
    COLDROP  *lpcdT;
    XFERFULL *lpxfMax;
    MessageId idm;
    FLEET    *t_call_a85e;
    uint16_t  t_scratch_m542;
    uint16_t  t_scratch_m542_2;
    uint16_t  t_scratch_m542_3;
    uint16_t  t_merge_b003_0001;
    int32_t   t_merge_b1d8_0001;
    uint16_t  t_merge_b2a1_0001;
    uint16_t  t_scratch_m548_3;
    uint16_t  t_scratch_m546;
    int32_t   t_merge_b464_0001;
    uint16_t  t_merge_b6c5_0001;
    FLEET    *t_call_b84c;
    FLEET    *t_call_bde7;
    uint16_t  t_merge_be51_0001;
    FLEET    *t_call_bf22;
    FLEET    *t_call_c15b;
    FLEET    *t_call_c3bb;
    FLEET    *t_call_c3f6;
    uint16_t  t_scratch_m542_5;
    uint16_t  t_scratch_m542_6;

L_a38c:
    lpxfCur = 0x0;
    goto L_c727;

L_a3a8:
    lppl = lpPlanets;
    lpplMac = (lpPlanets + cPlanet);
    goto L_a3f1;

L_a3d9:
    if ((lppl->id == LOWORD(lpb)))
        goto L_a401;
    else
        goto L_a3ed;

L_a3ed:
    lppl = (lppl + 1);

L_a3f1:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_a3d9;
    else
        goto L_a401;

L_a401:
    if ((LOWORD(lppl) != LOWORD(lpplMac)))
        goto L_a419;
    else
        goto L_a411;

L_a411:
    if ((HIWORD(lppl) == HIWORD(lpplMac)))
        goto L_a7d4;
    else
        goto L_a419;

L_a419:
    if ((lppl->iPlayer != idPlayer))
        goto L_a7d4;
    else
        goto L_a428;

L_a428:
    i = ((uint32_t)((cb - 2)) / 4);
    if ((i <= 0))
        goto L_a78b;
    else
        goto L_a443;

L_a443:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_a480;
    else
        goto L_a450;

L_a450:
    if ((HIWORD(lppl->lpplprod) != 0x0))
        goto L_a480;
    else
        goto L_a45a;

L_a45a:
    lppl->lpplprod = LpplAlloc(0x4, (i + 2), htOrd);
    goto L_a4bd;

L_a480:
    if ((lppl->lpplprod->iprodMax >= i))
        goto L_a4bd;
    else
        goto L_a497;

L_a497:
    lppl->lpplprod = LpplReAlloc(lppl->lpplprod, (i + 2));

L_a4bd:
    iPass = 0;
    goto L_a73c;

L_a4c6:
    if (((LOWORD((uint32_t)((lpb[((iPass * 4) + 2)] >> 0x14))) & 0x7f) != 0x0))
        goto L_a502;
    else
        goto L_a4fa;

L_a4fa:
    if ((0x0 == 0x0))
        goto L_a737;
    else
        goto L_a502;

L_a502:
    iLook = 0;
    goto L_a510;

L_a50b:
    iLook = (iLook + 1);

L_a510:
    if ((iLook >= lppl->lpplprod->iprodMac))
        goto L_a6d7;
    else
        goto L_a527;

L_a527:
    if (((LOWORD((uint32_t)((lppl->lpplprod->rgprod[iLook] >> 0x14))) & 0x7f) != 0x0))
        goto L_a568;
    else
        goto L_a560;

L_a560:
    if ((0x0 == 0x0))
        goto L_a50b;
    else
        goto L_a568;

L_a568:
    if ((LOWORD((uint32_t)((LOWORD((uint32_t)((lppl->lpplprod->rgprod[iLook] >> 0xa))) & 0x7f))) !=
         (LOWORD((uint32_t)(((((uint32_t)((uint16_t)(lpb[((iPass * 4) + 4)])) << 0x10) | (uint16_t)(lpb[((iPass * 4) + 2)])) >> 0xa))) & 0x7f)))
        goto L_a50b;
    else
        goto L_a5ec;

L_a5ec:
    if ((HIWORD((uint32_t)((LOWORD((uint32_t)((lppl->lpplprod->rgprod[iLook] >> 0xa))) & 0x7f))) != 0x0))
        goto L_a50b;
    else
        goto L_a5f3;

L_a5f3:
    if ((LOWORD((uint32_t)((LOWORD((uint32_t)((lppl->lpplprod->rgprod[iLook] >> 0x11))) & 0x7))) !=
         (LOWORD((uint32_t)(((((uint32_t)((uint16_t)(lpb[((iPass * 4) + 4)])) << 0x10) | (uint16_t)(lpb[((iPass * 4) + 2)])) >> 0x11))) & 0x7)))
        goto L_a50b;
    else
        goto L_a677;

L_a677:
    if ((HIWORD((uint32_t)((LOWORD((uint32_t)((lppl->lpplprod->rgprod[iLook] >> 0x11))) & 0x7))) != 0x0))
        goto L_a50b;
    else
        goto L_a67e;

L_a67e:
    lppl->lpplprod->rgprod[iLook] = ((lppl->lpplprod->rgprod[iLook] & 0xf80fffff) | 0x0);

L_a6d7:
    if ((iLook != lppl->lpplprod->iprodMac))
        goto L_a737;
    else
        goto L_a6ee;

L_a6ee:
    lpb[((iPass * 4) + 2)] = ((lpb[((iPass * 4) + 0x2)] & 0xffff) | 0x0);
    lpb[((iPass * 4) + 4)] = ((lpb[((iPass * 4) + 0x4)] & 0xf80f) | 0x0);

L_a737:
    iPass = (iPass + 1);

L_a73c:
    if ((iPass < i))
        goto L_a4c6;
    else
        goto L_a749;

L_a749:
    fmemmove(lppl->lpplprod->rgprod, lpb[2], (i * 4));
    lppl->lpplprod->iprodMac = LOBYTE(i);
    goto L_c796;

L_a78b:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_a7ab;
    else
        goto L_a798;

L_a798:
    if ((HIWORD(lppl->lpplprod) != 0x0))
        goto L_a7ab;
    else
        goto L_a7a2;

L_a7a2:
    return 1;

L_a7ab:
    FreeLp(lppl->lpplprod, htOrd);
    lppl->lpplprod = 0x0;

L_a7d1:
    goto L_c796;

L_a7d4:
    return 0;

L_a7dd:
    fmemcpy(game.lid[(0x59a2 + loword((0xc0 * load(ds : [0x018c])))) * 0x1], lpb, game.cPlayer);
    goto L_c796;

L_a807:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_c796;
    else
        goto L_a817;

L_a817:
    if ((cb <= 26))
        goto L_a828;
    else
        goto L_a822;

L_a822:
    return 0;

L_a828:
    /* untranslated: call fmemcpy(part[0x28:4](rgcrPlrHistory[(0x59a2 + loword((0xc0 * load(ds:[0x018c]))))*0x1]), lpb, cb) -> callresult(void *) */

L_a84f:
    goto L_c796;

L_a852:
    cOut = 32;
    t_call_a85e = LpflFromId(LOWORD(lpb));
    lpfl = t_call_a85e;
    if ((FAROFF(t_call_a85e) != 0x0))
        goto L_a884;
    else
        goto L_a876;

L_a876:
    if ((FARSEG(t_call_a85e) != 0x0))
        goto L_a884;
    else
        goto L_a87e;

L_a87e:
    return 0;

L_a884:
    if ((LOWORD(lpfl->lpszName) != 0x0))
        goto L_a89c;
    else
        goto L_a892;

L_a892:
    if ((HIWORD(lpfl->lpszName) == 0x0))
        goto L_a8b4;
    else
        goto L_a89c;

L_a89c:
    FreeLp(lpfl->lpszName, htString);

L_a8b4:
    i = lpb[4];
    if ((i == 0))
        goto L_a942;
    else
        goto L_a8cc;

L_a8cc:
    if ((FDecompressUserString(lpb[5], i, szT, &(cOut)) == 0))
        goto L_a942;
    else
        goto L_a8fa;

L_a8fa:
    lpfl->lpszName = LpAlloc((strlen(szT) + 0x1), htString);
    fstrcpy(lpfl->lpszName, szT);
    goto L_c796;

L_a942:
    if ((lpb[5] != 0x0))
        goto L_a967;
    else
        goto L_a954;

L_a954:
    lpfl->lpszName = 0x0;
    goto L_c796;

L_a967:
    lpfl->lpszName = LpAlloc((fstrlen(lpb[5]) + 0x1), htString);
    fstrcpy(lpfl->lpszName, lpb[5]);

L_a9b9:
    goto L_c796;

L_a9bc:
    lpth = LpthFromId(LOWORD(lpb));
    if ((LOWORD(lpth) != 0x0))
        goto L_a9e7;
    else
        goto L_a9dd;

L_a9dd:
    if ((HIWORD(lpth) == 0x0))
        goto L_a9fe;
    else
        goto L_a9e7;

L_a9e7:
    if ((lpth->ith == ithMinefield))
        goto L_aa04;
    else
        goto L_a9fe;

L_a9fe:
    return 0;

L_aa04:
    lpth->thm.fDetonate = LOBYTE(lpb[2]);
    goto L_c796;

L_aa16:
    i = ((LOWORD(lpb) >> 0x8) & 0x1f);
    iLook = ((LOWORD(lpb) >> 0x4) & 0xf);
    if ((iLook >= game.cPlayer))
        goto L_aa55;
    else
        goto L_aa49;

L_aa49:
    if ((iLook == idPlayer))
        goto L_aa5b;
    else
        goto L_aa55;

L_aa55:
    return 0;

L_aa5b:
    if ((i < 16))
        goto L_aa9e;
    else
        goto L_aa65;

L_aa65:
    if ((i < 26))
        goto L_aa75;
    else
        goto L_aa6f;

L_aa6f:
    return 0;

L_aa75:
    lpshdef = (rglpshdefSB[iLook] + (i - 16));
    goto L_aabf;

L_aa9e:
    lpshdef = (rglpshdef[iLook] + i);

L_aabf:
    if ((lpshdef->fFree != 0x0))
        goto L_ab08;
    else
        goto L_aad7;

L_aad7:
    if ((LOWORD(lpshdef->cExist) != 0x0))
        goto L_aaf1;
    else
        goto L_aae6;

L_aae6:
    if ((HIWORD(lpshdef->cExist) == 0x0))
        goto L_ab08;
    else
        goto L_aaf1;

L_aaf1:
    if (((LOWORD(lpb) & 0xf) == 0x0))
        goto L_ab08;
    else
        goto L_ab02;

L_ab02:
    return 0;

L_ab08:
    goto L_ad07;

L_ab14:
    if ((lpshdef->fFree != 0x0))
        goto L_c796;
    else
        goto L_ab2c;

L_ab2c:
    DestroyAllIshdef(i, idPlayer);
    lpshdef->fFree = 0x1;
    if ((i < 16))
        goto L_aba2;
    else
        goto L_ab5c;

L_ab5c:
    /* untranslated: t_scratch_m542 = ((part[0x4:2](rgplr[iLook]) + 0xf000) & 0xf000) */
    rgplr[iLook].cshdefSB = 0x0;
    /* untranslated: part[0x4:2](rgplr[iLook]) = (part[0x4:2](rgplr[iLook]) | t_scratch_m542) */
    goto L_c796;

L_aba2:
    rgplr[iLook].cShDef = (rgplr[iLook].cShDef - 1);

L_abb2:
    goto L_c796;

L_abb5:
    if ((lpshdef->fFree == 0x0))
        goto L_ac30;
    else
        goto L_abcd;

L_abcd:
    if ((i < 16))
        goto L_ac1d;
    else
        goto L_abd7;

L_abd7:
    /* untranslated: t_scratch_m542_2 = ((part[0x4:2](rgplr[iLook]) + 0x1000) & 0xf000) */
    rgplr[iLook].cshdefSB = 0x0;
    /* untranslated: part[0x4:2](rgplr[iLook]) = (part[0x4:2](rgplr[iLook]) | t_scratch_m542_2) */
    goto L_ac30;

L_ac1d:
    rgplr[iLook].cShDef = (rgplr[iLook].cShDef + 1);

L_ac30:
    if ((i < 16))
        goto L_acb7;
    else
        goto L_ac3a;

L_ac3a:
    if ((FReadShDef(lpb[2], rglpshdefSB[iLook], idPlayer) != 0))
        goto L_c796;
    else
        goto L_ac6b;

L_ac6b:
    /* untranslated: t_scratch_m542_3 = ((part[0x4:2](rgplr[iLook]) + 0xf000) & 0xf000) */
    rgplr[iLook].cshdefSB = 0x0;
    /* untranslated: part[0x4:2](rgplr[iLook]) = (part[0x4:2](rgplr[iLook]) | t_scratch_m542_3) */
    return 0;

L_acb7:
    if ((FReadShDef(lpb[2], rglpshdef[iLook], idPlayer) != 0))
        goto L_c796;
    else
        goto L_ace8;

L_ace8:
    rgplr[iLook].cShDef = (rgplr[iLook].cShDef - 1);
    return 0;

L_ad07:
    if (((LOWORD(lpb) & 0xf) == 0x0))
        goto L_ab14;
    else
        goto L_ad0f;

L_ad0f:
    if (((LOWORD(lpb) & 0xf) != 0x1))
        goto L_c796;
    else
        goto L_ad14;

L_ad14:
    goto L_abb5;

L_ad1d:
    /* untranslated: branch FLookupObject((lpb[4] & 0xf), LOWORD(lpb), &part[0x4:256](rgxf)) != 0 ? L_ad4d : L_ad47 */

L_ad47:
    return 0;

L_ad4d:
    rgxf[0x1].th.idFull = 0xffff;
    if ((((lpb[4] >> 0x4) & 0xf) == 0x4))
        goto L_ade3;
    else
        goto L_ad70;

L_ad70:
    /* untranslated: branch FLookupObject(((lpb[4] sar 0x4) & 0xf), lpb[2], &part[0x84:256](rgxf)) != 0 ? L_ade3 : L_ada3 */

L_ada3:
    if ((((lpb[4] >> 0x4) & 0xf) != 0x2))
        goto L_addd;
    else
        goto L_adc0;

L_adc0:
    if ((((lpb[2] >> 0x9) & 0xf) != idPlayer))
        goto L_c796;
    else
        goto L_addd;

L_addd:
    return 0;

L_ade3:
    grbit = lpb[5];
    i = 0;
    iLook = 0;
    goto L_ae17;

L_ae03:
    i = (i + 1);
    grbit = (grbit >> 0x1);

L_ae17:
    if ((i >= 5))
        goto L_aeee;
    else
        goto L_ae21;

L_ae21:
    if (((grbit & 0x1) == 0x0))
        goto L_aed5;
    else
        goto L_ae30;

L_ae30:
    if ((rt != rtLogCargoXfer8))
        goto L_ae66;
    else
        goto L_ae39;

L_ae39:
    rgcXfer[i] = (uint32_t)((uint16_t)(lpb[(iLook + 0x6)]));
    goto L_aecd;

L_ae66:
    if ((rt != rtLogCargoXfer16))
        goto L_ae9d;
    else
        goto L_ae6f;

L_ae6f:
    rgcXfer[i] = (uint32_t)(lpb[((iLook * 2) + 0x6)]);
    goto L_aecd;

L_ae9d:
    rgcXfer[i] = lpb[((iLook * 4) + 6)];

L_aecd:
    iLook = (iLook + 1);
    goto L_ae03;

L_aed5:
    rgcXfer[i] = 0;

L_aeeb:
    goto L_ae03;

L_aeee:
    iPass = 0;
    goto L_b725;

L_aef7:
    i = 0;
    goto L_af14;

L_af00:
    i = (i + 1);
    grbit = (grbit >> 0x1);

L_af14:
    if ((i >= 5))
        goto L_b720;
    else
        goto L_af1e;

L_af1e:
    if ((LOWORD(rgcXfer[i]) != 0x0))
        goto L_af3f;
    else
        goto L_af33;

L_af33:
    if ((HIWORD(rgcXfer[i]) == 0x0))
        goto L_af00;
    else
        goto L_af3f;

L_af3f:
    cXfer = rgcXfer[i];
    if ((iPass != 0))
        goto L_af78;
    else
        goto L_af61;

L_af61:
    if ((HIWORD(cXfer) < 0x0))
        goto L_af99;
    else
        goto L_af6a;

L_af6a:
    if ((HIWORD(cXfer) > 0x0))
        goto L_af78;
    else
        goto L_af6f;

L_af6f:
    if ((LOWORD(cXfer) < 0x0))
        goto L_af99;
    else
        goto L_af78;

L_af78:
    if ((iPass != 1))
        goto L_b067;
    else
        goto L_af82;

L_af82:
    if ((HIWORD(cXfer) < 0x0))
        goto L_b067;
    else
        goto L_af8b;

L_af8b:
    if ((HIWORD(cXfer) > 0x0))
        goto L_af99;
    else
        goto L_af90;

L_af90:
    if ((LOWORD(cXfer) < 0x0))
        goto L_b067;
    else
        goto L_af99;

L_af99:
    /* untranslated: l = ChgCargo((lpb[4] & 0xf), LOWORD(lpb), i, cXfer, &part[0x4:256](rgxf)) */
    if ((LOWORD(l) != LOWORD(cXfer)))
        goto L_afe5;
    else
        goto L_afdc;

L_afdc:
    if ((HIWORD(l) == HIWORD(cXfer)))
        goto L_b067;
    else
        goto L_afe5;

L_afe5:
    if (((lpb[4] & 0xf) != 0x2))
        goto L_b000;
    else
        goto L_affa;

L_affa:
    t_merge_b003_0001 = 0x8000;
    goto L_b003;

L_b000:
    t_merge_b003_0001 = 0x0;

L_b003:
    id = t_merge_b003_0001;
    id = (id | rgxf[0x0].th.idFull);
    /* untranslated: call FSendPlrMsg(part[0x6:2](rgxf[0x0]), 221, id, id, (LOWORD(cXfer) - LOWORD(l)), i, LOWORD(cXfer), 0, 0, 0) -> callresult(int16_t) */
    rgcXfer[i] = l;

L_b067:
    if ((iPass != 0))
        goto L_b088;
    else
        goto L_b071;

L_b071:
    if ((HIWORD(cXfer) > 0x0))
        goto L_b0a9;
    else
        goto L_b07a;

L_b07a:
    if ((HIWORD(cXfer) < 0x0))
        goto L_b088;
    else
        goto L_b07f;

L_b07f:
    if ((LOWORD(cXfer) >= 0x0))
        goto L_b0a9;
    else
        goto L_b088;

L_b088:
    if ((iPass != 1))
        goto L_af00;
    else
        goto L_b092;

L_b092:
    if ((HIWORD(cXfer) > 0x0))
        goto L_af00;
    else
        goto L_b09b;

L_b09b:
    if ((HIWORD(cXfer) < 0x0))
        goto L_b0a9;
    else
        goto L_b0a0;

L_b0a0:
    if ((LOWORD(cXfer) >= 0x0))
        goto L_af00;
    else
        goto L_b0a9;

L_b0a9:
    if ((((lpb[4] >> 0x4) & 0xf) == 0x4))
        goto L_af00;
    else
        goto L_b0c6;

L_b0c6:
    if ((i != 3))
        goto L_b2dd;
    else
        goto L_b0d0;

L_b0d0:
    if ((LOWORD(cXfer) != 0x0))
        goto L_b0e2;
    else
        goto L_b0d9;

L_b0d9:
    if ((HIWORD(cXfer) == 0x0))
        goto L_b2dd;
    else
        goto L_b0e2;

L_b0e2:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_b2dd;
    else
        goto L_b0f2;

L_b0f2:
    if ((((lpb[4] >> 0x4) & 0xf) != 0x1))
        goto L_b2dd;
    else
        goto L_b10f;

L_b10f:
    if (((lpb[4] & 0xf) != 0x2))
        goto L_b2dd;
    else
        goto L_b124;

L_b124:
    /* untranslated: branch part[0x6:2](rgxf[0x0]) == part[0x6:2](rgxf[0x1]) ? L_b2dd : L_b131 */

L_b131:
    lpcdT = lpcd;
    iColDrop = 0;
    if ((HIWORD(cXfer) < 0x0))
        goto L_b243;
    else
        goto L_b14f;

L_b14f:
    if ((HIWORD(cXfer) > 0x0))
        goto L_b15d;
    else
        goto L_b154;

L_b154:
    if ((LOWORD(cXfer) <= 0x0))
        goto L_b243;
    else
        goto L_b15d;

L_b15d:
    if ((iColDrop >= cColDrop))
        goto L_af00;
    else
        goto L_b169;

L_b169:
    if ((HIWORD(cXfer) < 0x0))
        goto L_af00;
    else
        goto L_b172;

L_b172:
    if ((HIWORD(cXfer) > 0x0))
        goto L_b180;
    else
        goto L_b177;

L_b177:
    if ((LOWORD(cXfer) <= 0x0))
        goto L_af00;
    else
        goto L_b180;

L_b180:
    if ((lpcdT->idPlanetDst != rgxf[1].th.idFull))
        goto L_b202;
    else
        goto L_b191;

L_b191:
    /* untranslated: branch lpcdT->idPlr != part[0x6:2](rgxf[0x0]) ? L_b202 : L_b1a2 */

L_b1a2:
    if ((HIWORD(cXfer) > HIWORD(lpcdT->cColonist)))
        goto L_b1cc;
    else
        goto L_b1b6;

L_b1b6:
    if ((HIWORD(cXfer) < HIWORD(lpcdT->cColonist)))
        goto L_b1c3;
    else
        goto L_b1bb;

L_b1bb:
    if ((LOWORD(cXfer) >= LOWORD(lpcdT->cColonist)))
        goto L_b1cc;
    else
        goto L_b1c3;

L_b1c3:
    t_merge_b1d8_0001 = cXfer;
    goto L_b1d8;

L_b1cc:
    t_merge_b1d8_0001 = lpcdT->cColonist;

L_b1d8:
    l = t_merge_b1d8_0001;
    cXfer = (cXfer - l);
    lpcdT->cColonist = (lpcdT->cColonist - l);

L_b202:
    lpcdT = (lpcdT + 1);
    iColDrop = (iColDrop + 1);
    goto L_b15d;

L_b215:
    if ((lpcdT->idFleetSrc != rgxf[0].th.idFull))
        goto L_b239;
    else
        goto L_b225;

L_b225:
    if ((lpcdT->idPlanetDst == rgxf[1].th.idFull))
        goto L_b24f;
    else
        goto L_b239;

L_b239:
    lpcdT = (lpcdT + 1);
    iColDrop = (iColDrop + 1);

L_b243:
    if ((iColDrop < cColDrop))
        goto L_b215;
    else
        goto L_b24f;

L_b24f:
    if ((iColDrop != cColDrop))
        goto L_b2c8;
    else
        goto L_b25b;

L_b25b:
    LOWORD(lpcdT) = rgxf[0x0].th.idFull;
    /* untranslated: lpcdT->idPlr = part[0x6:2](rgxf[0x0]) */
    lpcdT->idPlanetDst = rgxf[0x1].th.idFull;
    lpcdT->cColonist = 0;
    /* untranslated: branch part[0x6:2](rgxf[0x1]) == 0xffff ? L_b29e : L_b298 */

L_b298:
    t_merge_b2a1_0001 = 0x1;
    goto L_b2a1;

L_b29e:
    t_merge_b2a1_0001 = 0x0;

L_b2a1:
    t_scratch_m548_3 = t_merge_b2a1_0001;
    lpcdT->fCanColonize = t_scratch_m548_3;
    cColDrop = (cColDrop + 1);

L_b2c8:
    lpcdT->cColonist = (lpcdT->cColonist - cXfer);
    goto L_af00;

L_b2dd:
    if ((LOWORD(cXfer) != 0x0))
        goto L_b2ef;
    else
        goto L_b2e6;

L_b2e6:
    if ((HIWORD(cXfer) == 0x0))
        goto StealCargo;
    else
        goto L_b2ef;

L_b2ef:
    if ((gd.fGeneratingTurn == 0x0))
        goto StealCargo;
    else
        goto L_b2ff;

L_b2ff:
    /* untranslated: branch part[0x6:2](rgxf[0x1]) == part[0x6:2](rgxf[0x0]) ? L_b5a2 : L_b30c */

L_b30c:
    if ((((lpb[4] >> 0x4) & 0xf) == 0x8))
        goto StealCargo;
    else
        goto L_b329;

L_b329:
    lpxfMax = (lpxf + cXferFull);
    if ((HIWORD(cXfer) < 0x0))
        goto L_b4ce;
    else
        goto L_b34b;

L_b34b:
    if ((HIWORD(cXfer) > 0x0))
        goto L_b359;
    else
        goto L_b350;

L_b350:
    if ((LOWORD(cXfer) <= 0x0))
        goto L_b4ce;
    else
        goto L_b359;

L_b359:
    lpxfCur = lpxf;

L_b366:
    if ((LOWORD(lpxfCur) >= LOWORD(lpxfMax)))
        goto L_b4a7;
    else
        goto L_b376;

L_b376:
    if ((HIWORD(cXfer) < 0x0))
        goto L_b4a7;
    else
        goto L_b37f;

L_b37f:
    if ((HIWORD(cXfer) > 0x0))
        goto L_b38d;
    else
        goto L_b384;

L_b384:
    if ((LOWORD(cXfer) <= 0x0))
        goto L_b4a7;
    else
        goto L_b38d;

L_b38d:
    t_scratch_m546 = lpxfCur->grobj2;
    if ((t_scratch_m546 != ((lpb[4] >> 0x4) & 0xf)))
        goto L_b4a0;
    else
        goto L_b3c8;

L_b3c8:
    if ((lpxfCur->id2 != lpb[2]))
        goto L_b4a0;
    else
        goto L_b3db;

L_b3db:
    if ((lpxfCur->grobj1 != 0x2))
        goto L_b4a0;
    else
        goto L_b3f0;

L_b3f0:
    if (((lpxfCur->id1 & 0xfe00) != (LOWORD(lpb) & 0xfe00)))
        goto L_b4a0;
    else
        goto L_b40a;

L_b40a:
    if ((HIWORD(cXfer) > HIWORD(lpxfCur->rgcQuan[i])))
        goto L_b446;
    else
        goto L_b430;

L_b430:
    if ((HIWORD(cXfer) < HIWORD(lpxfCur->rgcQuan[i])))
        goto L_b43d;
    else
        goto L_b435;

L_b435:
    if ((LOWORD(cXfer) >= LOWORD(lpxfCur->rgcQuan[i])))
        goto L_b446;
    else
        goto L_b43d;

L_b43d:
    t_merge_b464_0001 = cXfer;
    goto L_b464;

L_b446:
    t_merge_b464_0001 = lpxfCur->rgcQuan[i];

L_b464:
    l = t_merge_b464_0001;
    cXfer = (cXfer - l);
    lpxfCur->rgcQuan[i] = (lpxfCur->rgcQuan[i] - l);

L_b4a0:
    lpxfCur = (lpxfCur + 1);
    goto L_b366;

L_b4a7:
    lpxfCur = 0x0;
    if ((HIWORD(cXfer) < 0x0))
        goto L_af00;
    else
        goto L_b4ba;

L_b4ba:
    if ((HIWORD(cXfer) > 0x0))
        goto StealCargo;
    else
        goto L_b4bf;

L_b4bf:
    if ((LOWORD(cXfer) > 0x0))
        goto StealCargo;
    else
        goto L_b4c5;

L_b4c5:
    goto L_af00;

L_b4ce:
    if ((iPass == 1))
        goto StealCargo;
    else
        goto L_b4db;

L_b4db:
    if ((LOWORD(lpxfCur) != 0x0))
        goto L_b57b;
    else
        goto L_b4e4;

L_b4e4:
    if ((HIWORD(lpxfCur) != 0x0))
        goto L_b57b;
    else
        goto L_b4ed;

L_b4ed:
    lpxfCur = lpxf;
    goto L_b524;

L_b4fd:
    if ((fmemcmp(lpxfCur, lpb, 0x5) == 0))
        goto L_b534;
    else
        goto L_b520;

L_b520:
    lpxfCur = (lpxfCur + 1);

L_b524:
    if ((LOWORD(lpxfCur) < LOWORD(lpxfMax)))
        goto L_b4fd;
    else
        goto L_b534;

L_b534:
    if ((LOWORD(lpxfCur) != LOWORD(lpxfMax)))
        goto L_b57b;
    else
        goto L_b544;

L_b544:
    if ((HIWORD(lpxfCur) != HIWORD(lpxfMax)))
        goto L_b57b;
    else
        goto L_b54c;

L_b54c:
    cXferFull = (cXferFull + 1);
    fmemset(lpxfCur, 0, 0x19);
    LOWORD(lpxfCur) = LOWORD(lpb);
    lpxfCur->id2 = lpb[2];

L_b57b:
    lpxfCur->rgcQuan[i] = (lpxfCur->rgcQuan[i] - cXfer);
    goto L_af00;

StealCargo:
    /* untranslated: l = ChgCargo(((lpb[4] sar 0x4) & 0xf), lpb[2], i, words(neg((HIWORD(cXfer) + 0x0)), neg(LOWORD(cXfer))), &part[0x84:256](rgxf)) */
    if ((LOWORD(l) != (-LOWORD(cXfer))))
        goto L_b607;
    else
        goto L_b5fe;

L_b5fe:
    if ((HIWORD(l) == (-(HIWORD(cXfer) + 0x0))))
        goto L_af00;
    else
        goto L_b607;

L_b607:
    LOWORD(rgcXfer[i]) = (-LOWORD(l));
    HIWORD(rgcXfer[i]) = (-(HIWORD(l) + 0x0));
    if ((((lpb[4] >> 0x4) & 0xf) != 0x8))
        goto L_b6a7;
    else
        goto L_b645;

L_b645:
    idm = idmDidntGetAttemptedTransferMineralPacketAnother;
    if ((LOWORD(l) != 0x0))
        goto L_b664;
    else
        goto L_b655;

L_b655:
    if ((HIWORD(l) != 0x0))
        goto L_b664;
    else
        goto L_b65f;

L_b65f:
    idm = (idm + 1);

L_b664:
    /* untranslated: call FSendPlrMsg(part[0x6:2](rgxf[0x0]), idm, (rgxf[0].th.idFull | 0x8000), rgxf[0].th.idFull, i, neg(LOWORD(l)), i, 0, 0, 0) ->
     * callresult(int16_t) */
    goto L_af00;

L_b6a7:
    if (((lpb[4] & 0xf) != 0x2))
        goto L_b6c2;
    else
        goto L_b6bc;

L_b6bc:
    t_merge_b6c5_0001 = 0x8000;
    goto L_b6c5;

L_b6c2:
    t_merge_b6c5_0001 = 0x0;

L_b6c5:
    id = t_merge_b6c5_0001;
    id = (id | rgxf[0x0].th.idFull);
    /* untranslated: call FSendPlrMsg(part[0x6:2](rgxf[0x0]), 221, id, id, (neg(LOWORD(l)) - LOWORD(cXfer)), i, neg(LOWORD(cXfer)), 0, 0, 0) ->
     * callresult(int16_t) */

DoNext:
    goto L_af00;

L_b720:
    iPass = (iPass + 1);

L_b725:
    if ((iPass < 2))
        goto L_aef7;
    else
        goto L_b72f;

L_b72f:
    if (((lpb[4] & 0xf) != 0x2))
        goto L_b758;
    else
        goto L_b744;

L_b744:
    FLookupFleet(-1, &(rgxf[0].fl));
    goto L_b769;

L_b758:
    FLookupPlanet(-1, &(rgxf[0].pl));

L_b769:
    if ((((lpb[4] >> 0x4) & 0xf) != 0x2))
        goto L_b79a;
    else
        goto L_b786;

L_b786:
    FLookupFleet(-1, &(rgxf[1].fl));
    goto L_c796;

L_b79a:
    if ((((lpb[4] >> 0x4) & 0xf) == 0x1))
        goto L_b7d4;
    else
        goto L_b7b7;

L_b7b7:
    if ((((lpb[4] >> 0x4) & 0xf) != 0x4))
        goto L_b7e8;
    else
        goto L_b7d4;

L_b7d4:
    FLookupPlanet(-1, &(rgxf[1].pl));
    goto L_c796;

L_b7e8:
    if ((((lpb[4] >> 0x4) & 0xf) != 0x8))
        goto L_c796;
    else
        goto L_b805;

L_b805:
    FLookupThing(-1, &(rgxf[1].th));

L_b816:
    goto L_c796;

L_b819:
    /* untranslated: branch FLookupObject(grobjFleet, LOWORD(lpb), &part[0x4:256](rgxf)) != 0 ? L_b83e : L_b838 */

L_b838:
    return 0;

L_b83e:
    if ((rt != rtLogFleetSplit))
        goto L_b86d;
    else
        goto L_b847;

L_b847:
    t_call_b84c = LpflNewSplit(&(rgxf[0].fl));
    if ((FAROFF(t_call_b84c) != 0x0))
        goto L_c796;
    else
        goto L_b85c;

L_b85c:
    if ((FARSEG(t_call_b84c) != 0x0))
        goto L_c796;
    else
        goto L_b864;

L_b864:
    return 0;

L_b86d:
    vrgiflMerge = rgifl;
    vcflMerge = 0;
    if ((cb != 2))
        goto L_b955;
    else
        goto L_b885;

L_b885:
    ifl = 0;
    goto L_b893;

L_b88e:
    ifl = (ifl + 1);

L_b893:
    if ((ifl >= cFleet))
        goto L_ba12;
    else
        goto L_b89f;

L_b89f:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_b8d2;
    else
        goto L_b8ca;

L_b8ca:
    if ((HIWORD(rglpfl[ifl]) == 0x0))
        goto L_ba12;
    else
        goto L_b8d2;

L_b8d2:
    if ((lpfl->iPlayer != idPlayer))
        goto L_b88e;
    else
        goto L_b8e2;

L_b8e2:
    if ((lpfl->fDead != 0x0))
        goto L_b88e;
    else
        goto L_b8fa;

L_b8fa:
    /* untranslated: branch lpfl->pt.x != LOWORD(part[0xc:256](rgxf)) ? L_b88e : L_b916 */

L_b916:
    /* untranslated: branch lpfl->pt.y != HIWORD(part[0xc:256](rgxf)) ? L_b88e : L_b91f */

L_b91f:
    vcflMerge = (vcflMerge + 1);
    rgifl[vcflMerge] = lpfl->id;
    lpfl->fCompChg = 0x1;

L_b94f:
    goto L_b88e;

L_b955:
    i = 0;
    goto L_b963;

L_b95e:
    i = (i + 1);

L_b963:
    if ((i >= ((uint32_t)(cb) / 2)))
        goto L_ba12;
    else
        goto L_b976;

L_b976:
    ifl = 0;
    goto L_b984;

L_b97f:
    ifl = (ifl + 1);

L_b984:
    if ((ifl >= cFleet))
        goto L_ba0b;
    else
        goto L_b990;

L_b990:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_b9c3;
    else
        goto L_b9bb;

L_b9bb:
    if ((HIWORD(rglpfl[ifl]) == 0x0))
        goto L_ba0b;
    else
        goto L_b9c3;

L_b9c3:
    if ((lpfl->id != LOWORD(lpb)))
        goto L_b97f;
    else
        goto L_b9d5;

L_b9d5:
    vcflMerge = (vcflMerge + 1);
    rgifl[vcflMerge] = lpfl->id;
    lpfl->fCompChg = 0x1;

L_ba0b:
    lpb = (lpb + 2);
    goto L_b95e;

L_ba12:
    if ((FFleetMergeAll(&(rgxf[0].fl)) != 0))
        goto L_c796;
    else
        goto L_ba27;

L_ba27:
    return 0;

L_ba30:
    /* untranslated: branch FLookupObject(grobjFleet, LOWORD(lpb), &part[0x4:256](rgxf)) != 0 ? L_ba56 : L_ba50 */

L_ba50:
    return 0;

L_ba56:
    /* untranslated: branch FLookupObject(grobjFleet, lpb[2], &part[0x84:256](rgxf)) != 0 ? L_ba7d : L_ba77 */

L_ba77:
    return 0;

L_ba7d:
    /* untranslated: branch part[0x6:2](rgxf[0x1]) == part[0x6:2](rgxf[0x0]) ? L_ba90 : L_ba8a */

L_ba8a:
    return 0;

L_ba90:
    iPass = 0;
    goto L_bcd2;

L_ba99:
    grbit = lpb[5];
    i = 0;
    iLook = 0;
    goto L_baca;

L_bab6:
    i = (i + 1);
    grbit = (grbit >> 0x1);

L_baca:
    if ((i >= 16))
        goto L_bccd;
    else
        goto L_bad4;

L_bad4:
    if (((grbit & 0x1) == 0x0))
        goto L_bab6;
    else
        goto L_bae3;

L_bae3:
    cXfer = (uint32_t)(lpb[((iLook * 2) + 0x7)]);
    if ((iPass != 0))
        goto L_bb23;
    else
        goto L_bb0c;

L_bb0c:
    if ((HIWORD(cXfer) < 0x0))
        goto L_bb44;
    else
        goto L_bb15;

L_bb15:
    if ((HIWORD(cXfer) > 0x0))
        goto L_bb23;
    else
        goto L_bb1a;

L_bb1a:
    if ((LOWORD(cXfer) < 0x0))
        goto L_bb44;
    else
        goto L_bb23;

L_bb23:
    if ((iPass != 1))
        goto L_bbde;
    else
        goto L_bb2d;

L_bb2d:
    if ((HIWORD(cXfer) < 0x0))
        goto L_bbde;
    else
        goto L_bb36;

L_bb36:
    if ((HIWORD(cXfer) > 0x0))
        goto L_bb44;
    else
        goto L_bb3b;

L_bb3b:
    if ((LOWORD(cXfer) < 0x0))
        goto L_bbde;
    else
        goto L_bb44;

L_bb44:
    /* untranslated: branch (signhiword(ss:[bp+(i * 2)-0x114]) + HIWORD(cXfer)) > 0x0 ? L_bb88 : L_bb61 */

L_bb61:
    /* untranslated: branch (signhiword(ss:[bp+(i * 2)-0x114]) + HIWORD(cXfer)) < 0x0 ? L_bb6e : L_bb66 */

L_bb66:
    /* untranslated: branch (ss:[bp+(i * 2)-0x114] + LOWORD(cXfer)) >= 0x0 ? L_bb88 : L_bb6e */

L_bb6e:
    /* untranslated: cXfer = sext16to32(neg(ss:[bp+(i * 0x2)-0x114])) */
    goto L_bbca;

L_bb88:
    /* untranslated: branch signhiword((0x7ffe - ss:[bp+(i * 2)-0x114])) > HIWORD(cXfer) ? L_bbca : L_bba2 */

L_bba2:
    /* untranslated: branch signhiword((0x7ffe - ss:[bp+(i * 2)-0x114])) < HIWORD(cXfer) ? L_bbaf : L_bba7 */

L_bba7:
    /* untranslated: branch (0x7ffe - ss:[bp+(i * 2)-0x114]) > LOWORD(cXfer) ? L_bbca : L_bbaf */

L_bbaf:
    /* untranslated: cXfer = sext16to32(((0x7ffe - ss:[bp+(i * 0x2)-0x114]) + 0xffff)) */

L_bbca:
    /* untranslated: ss:[bp+(i * 2)-0x114] = (ss:[bp+(i * 0x2)-0x114] + LOWORD(cXfer)) */

L_bbde:
    if ((iPass != 0))
        goto L_bbff;
    else
        goto L_bbe8;

L_bbe8:
    if ((HIWORD(cXfer) > 0x0))
        goto L_bc20;
    else
        goto L_bbf1;

L_bbf1:
    if ((HIWORD(cXfer) < 0x0))
        goto L_bbff;
    else
        goto L_bbf6;

L_bbf6:
    if ((LOWORD(cXfer) >= 0x0))
        goto L_bc20;
    else
        goto L_bbff;

L_bbff:
    if ((iPass != 1))
        goto L_bcc5;
    else
        goto L_bc09;

L_bc09:
    if ((HIWORD(cXfer) > 0x0))
        goto L_bcc5;
    else
        goto L_bc12;

L_bc12:
    if ((HIWORD(cXfer) < 0x0))
        goto L_bc20;
    else
        goto L_bc17;

L_bc17:
    if ((LOWORD(cXfer) >= 0x0))
        goto L_bcc5;
    else
        goto L_bc20;

L_bc20:
    /* untranslated: branch (signhiword(ss:[bp+(i * 2)-0x94]) - HIWORD(cXfer)) > 0x0 ? L_bc62 : L_bc3d */

L_bc3d:
    /* untranslated: branch (signhiword(ss:[bp+(i * 2)-0x94]) - HIWORD(cXfer)) < 0x0 ? L_bc4a : L_bc42 */

L_bc42:
    /* untranslated: branch (ss:[bp+(i * 2)-0x94] - LOWORD(cXfer)) >= 0x0 ? L_bc62 : L_bc4a */

L_bc4a:
    /* untranslated: cXfer = sext16to32(ss:[bp+(i * 0x2)-0x94]) */
    goto L_bcb1;

L_bc62:
    /* untranslated: branch signhiword((0x7ffe - ss:[bp+(i * 2)-0x94])) > neg((HIWORD(cXfer) + 0x0)) ? L_bcb1 : L_bc88 */

L_bc88:
    /* untranslated: branch signhiword((0x7ffe - ss:[bp+(i * 2)-0x94])) < neg((HIWORD(cXfer) + 0x0)) ? L_bc94 : L_bc8d */

L_bc8d:
    /* untranslated: branch (0x7ffe - ss:[bp+(i * 2)-0x94]) > neg(LOWORD(cXfer)) ? L_bcb1 : L_bc94 */

L_bc94:
    /* untranslated: cXfer = sext16to32(neg(((0x7ffe - ss:[bp+(i * 0x2)-0x94]) + 0xffff))) */

L_bcb1:
    /* untranslated: ss:[bp+(i * 2)-0x94] = (ss:[bp+(i * 0x2)-0x94] - LOWORD(cXfer)) */

L_bcc5:
    iLook = (iLook + 1);

L_bcca:
    goto L_bab6;

L_bccd:
    iPass = (iPass + 1);

L_bcd2:
    if ((iPass < 2))
        goto L_ba99;
    else
        goto L_bcdc;

L_bcdc:
    FleetTransferCargoBalance(&(rgxf[0].fl), &(rgxf[1].fl));
    iPass = 0;
    goto L_bdd4;

L_bcf7:
    FLookupFleet(-1, (&(rgxf[iPass]) + 0x4));
    lpfl = LpflFromId(rgxf[iPass].th.idFull);
    if ((LOWORD(lpfl) != 0x0))
        goto L_bd4e;
    else
        goto L_bd44;

L_bd44:
    if ((HIWORD(lpfl) == 0x0))
        goto L_bd64;
    else
        goto L_bd4e;

L_bd4e:
    lpfl->fCompChg = 0x1;

L_bd64:
    i = 0;
    goto L_bd99;

L_bd6d:
    /* untranslated: branch ds:[((&rgxf[iPass] + 0x10) + (i * 2))] != 0x0 ? L_bda3 : L_bd94 */

L_bd94:
    i = (i + 1);

L_bd99:
    if ((i < 16))
        goto L_bd6d;
    else
        goto L_bda3;

L_bda3:
    if ((i != 16))
        goto L_bdcf;
    else
        goto L_bdad;

L_bdad:
    FDeleteFleet(rgxf[iPass].th.idFull, grobjNone, 0);

L_bdcf:
    iPass = (iPass + 1);

L_bdd4:
    if ((iPass >= 2))
        goto L_c796;
    else
        goto L_bddb;

L_bddb:
    goto L_bcf7;

L_bde1:
    t_call_bde7 = LpflFromId(LOWORD(lpb));
    lpfl = t_call_bde7;
    if ((FAROFF(t_call_bde7) != 0x0))
        goto L_be07;
    else
        goto L_bdff;

L_bdff:
    if ((FARSEG(t_call_bde7) == 0x0))
        goto L_be30;
    else
        goto L_be07;

L_be07:
    if ((lpfl->cord <= 0))
        goto L_be30;
    else
        goto L_be15;

L_be15:
    iLook = (lpb[2] & 0x7fff);
    if (((lpb[2] & 0x7fff) < lpfl->cord))
        goto L_be36;
    else
        goto L_be30;

L_be30:
    return 0;

L_be36:
    if (((lpb[2] & 0x8000) == 0x0))
        goto L_be4e;
    else
        goto L_be48;

L_be48:
    t_merge_be51_0001 = 0x1;
    goto L_be51;

L_be4e:
    t_merge_be51_0001 = 0x0;

L_be51:
    fExtra = t_merge_be51_0001;
    if ((fExtra == 0))
        goto L_be77;
    else
        goto L_be5d;

L_be5d:
    if (((iLook + 1) < lpfl->cord))
        goto L_be77;
    else
        goto L_be71;

L_be71:
    return 0;

L_be77:
    fmemmove(&(lpfl->lpplord->rgord[iLook]), &(lpfl->lpplord->rgord[((iLook + fExtra) + 1)]), ((((lpfl->cord - iLook) - fExtra) - 1) * 18));
    lpfl->cord = (lpfl->cord - (fExtra + 1));
    lpfl->lpplord->iordMac = (lpfl->lpplord->iordMac - LOBYTE((fExtra + 0x1)));
    goto L_c796;

L_bf1c:
    t_call_bf22 = LpflFromId(LOWORD(lpb));
    lpfl = t_call_bf22;
    if ((FAROFF(t_call_bf22) != 0x0))
        goto L_bf42;
    else
        goto L_bf3a;

L_bf3a:
    if ((FARSEG(t_call_bf22) == 0x0))
        goto L_bf63;
    else
        goto L_bf42;

L_bf42:
    if ((lpb[2] < 0x0))
        goto L_bf63;
    else
        goto L_bf4f;

L_bf4f:
    if ((lpb[2] <= lpfl->cord))
        goto L_bf69;
    else
        goto L_bf63;

L_bf63:
    return 0;

L_bf69:
    if ((lpfl->cord != lpfl->lpplord->iordMax))
        goto L_bfb1;
    else
        goto L_bf85;

L_bf85:
    lpfl->lpplord = LpplReAlloc(lpfl->lpplord, (lpfl->cord + 3));

L_bfb1:
    fmemmove(&(lpfl->lpplord->rgord[(lpb[2] + 1)]), &(lpfl->lpplord->rgord[lpb[2]]), ((lpfl->cord - lpb[2]) * 0x12));
    if ((cb >= 22))
        goto L_c07d;
    else
        goto L_c03e;

L_c03e:
    fmemset(&(lpfl->lpplord->rgord[lpb[2]]), 0, 0x12);

L_c07d:
    fmemmove(&(lpfl->lpplord->rgord[lpb[2]]), lpb[4], (cb - 4));
    lpfl->lpplord->rgord[lpb[2]].fNoAutoTrack = 0x0;
    lpfl->cord = (lpfl->cord + 1);
    lpfl->lpplord->iordMac = (lpfl->lpplord->iordMac + 0x1);
    goto L_c796;

L_c155:
    t_call_c15b = LpflFromId(LOWORD(lpb));
    lpfl = t_call_c15b;
    if ((FAROFF(t_call_c15b) != 0x0))
        goto L_c17b;
    else
        goto L_c173;

L_c173:
    if ((FARSEG(t_call_c15b) == 0x0))
        goto L_c1a1;
    else
        goto L_c17b;

L_c17b:
    if ((lpfl->cord < 0))
        goto L_c1a1;
    else
        goto L_c189;

L_c189:
    iLook = lpb[2];
    if ((lpb[2] < lpfl->cord))
        goto L_c1a7;
    else
        goto L_c1a1;

L_c1a1:
    return 0;

L_c1a7:
    if ((cb >= 22))
        goto L_c1ee;
    else
        goto L_c1b2;

L_c1b2:
    fmemset(&(lpfl->lpplord->rgord[iLook]), 0, 0x12);

L_c1ee:
    fmemmove(&(lpfl->lpplord->rgord[iLook]), lpb[4], (cb - 4));
    lpfl->lpplord->rgord[iLook].fNoAutoTrack = 0x0;
    goto L_c796;

L_c287:
    i = ((LOWORD(lpb) >> 0x4) & 0xf);
    if (((LOWORD(lpb) & 0xf) != idPlayer))
        goto L_c2ce;
    else
        goto L_c2b0;

L_c2b0:
    if ((i < 0))
        goto L_c2ce;
    else
        goto L_c2ba;

L_c2ba:
    if ((i <= rgcbtlplan[idPlayer]))
        goto L_c2ed;
    else
        goto L_c2ce;

L_c2ce:
    if ((((LOWORD(lpb) >> 0xe) & 0x1) != 0x0))
        goto L_c796;
    else
        goto L_c2e1;

L_c2e1:
    goto BombOut;

L_c2ed:
    if ((((LOWORD(lpb) >> 0xe) & 0x1) == 0x0))
        goto L_c316;
    else
        goto L_c303;

L_c303:
    FDeleteBattlePlan(i, 0);
    goto L_c796;

L_c316:
    if ((((LOWORD(lpb) >> 0x8) & 0xf) > 0x6))
        goto BombOut;
    else
        goto L_c32c;

L_c32c:
    if (((lpb[2] & 0xf) > 0x8))
        goto BombOut;
    else
        goto L_c33e;

L_c33e:
    if ((((lpb[2] >> 0x4) & 0xf) > 0x8))
        goto BombOut;
    else
        goto L_c35b;

L_c35b:
    if ((i != rgcbtlplan[idPlayer]))
        goto L_c385;
    else
        goto L_c36f;

L_c36f:
    if ((i >= 16))
        goto BombOut;
    else
        goto L_c37c;

L_c37c:
    rgcbtlplan[idPlayer] = (rgcbtlplan[idPlayer] + 0x1);

L_c385:
    UnpackBattlePlan(lpb, &(rglpbtlplan[idPlayer][i]), i);
    goto L_c796;

L_c3b5:
    t_call_c3bb = LpflFromId(LOWORD(lpb));
    lpfl = t_call_c3bb;
    if ((FAROFF(t_call_c3bb) != 0x0))
        goto L_c3de;
    else
        goto L_c3d3;

L_c3d3:
    if ((FARSEG(t_call_c3bb) == 0x0))
        goto BombOut;
    else
        goto L_c3de;

L_c3de:
    lpfl->iplan = LOBYTE(lpb[2]);
    goto L_c796;

L_c3f0:
    t_call_c3f6 = LpflFromId(LOWORD(lpb));
    lpfl = t_call_c3f6;
    if ((FAROFF(t_call_c3f6) != 0x0))
        goto L_c41c;
    else
        goto L_c40e;

L_c40e:
    if ((FARSEG(t_call_c3f6) != 0x0))
        goto L_c41c;
    else
        goto BombOut;

BombOut:
    return 0;

L_c41c:
    if ((rt != rtLogFleetFlagBit9))
        goto L_c457;
    else
        goto L_c425;

L_c425:
    t_scratch_m542_5 = lpb[2];
    lpfl->fRepOrders = t_scratch_m542_5;
    goto L_c796;

L_c457:
    if ((lpfl->cord <= lpb[2]))
        goto BombOut;
    else
        goto L_c46b;

L_c46b:
    if ((lpb[4] >= 0xa))
        goto BombOut;
    else
        goto L_c47b;

L_c47b:
    t_scratch_m542_6 = lpb[4];
    lpfl->lpplord->rgord[lpb[2]].grTask = t_scratch_m542_6;

L_c508:
    goto L_c796;

L_c50b:
    lppl = LpplFromId(LOWORD(lpb));
    if ((LOWORD(lppl) != 0x0))
        goto L_c531;
    else
        goto L_c528;

L_c528:
    if ((HIWORD(lppl) == 0x0))
        goto BombOut;
    else
        goto L_c531;

L_c531:
    if ((lppl->iPlayer != idPlayer))
        goto BombOut;
    else
        goto L_c543;

L_c543:
    /* untranslated: part[0x18:4](lppl) = words(((*(lppl+0x1a) & 0xff7f) | hiword((int32_t)((uint32_t)(loword((uint32_t)(lpb[2] & 0x1)) & 0x1) << 0x17))),
     * ((*(lppl+0x18) & 0xffff) | loword((int32_t)((uint32_t)(loword((uint32_t)(lpb[2] & 0x1)) & 0x1) << 0x17)))) */
    lppl->idFling = (LOWORD((uint32_t)((lpb[2] >> 0x1))) & 0x3ff);
    lppl->iWarpFling = (LOWORD((uint32_t)((lpb[2] >> 0xb))) & 0xf);
    lppl->idRoute = (LOWORD((uint32_t)((lpb[2] >> 0xf))) & 0x3ff);
    goto L_c796;

L_c65c:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_c796;
    else
        goto L_c66c;

L_c66c:
    rgplr[idPlayer].lSalt = (((uint32_t)((uint16_t)(lpb[2])) << 0x10) | (uint16_t)(LOWORD(lpb)));

L_c698:
    goto L_c796;

L_c69b:
    ch = *(lpb);
    if (((uint16_t)(ch) < 0))
        goto BombOut;
    else
        goto L_c6b2;

L_c6b2:
    if (((uint16_t)(ch) > 100))
        goto BombOut;
    else
        goto L_c6c2;

L_c6c2:
    rgplr[idPlayer].pctResearch = ch;
    ch = lpb[1];
    if ((((uint16_t)(ch) & 0xf) >= 0x6))
        goto BombOut;
    else
        goto L_c6f0;

L_c6f0:
    if (((((uint16_t)(ch) >> 0x4) & 0xf) > 0x7))
        goto BombOut;
    else
        goto L_c70b;

L_c70b:
    rgplr[idPlayer].iTechCur = ch;
    goto L_c796;

L_c727:
    if ((rt > rtLogPlayerZpq1))
        goto L_c796;
    else
        goto L_c72f;

L_c72f:
    switch ((rt * 0x2)) {
    case 0x0:
        goto L_c796;
    case 0x2:
        goto L_ad1d;
    case 0x4:
        goto L_ad1d;
    case 0x6:
        goto L_bde1;
    case 0x8:
        goto L_bf1c;
    case 0xa:
        goto L_c155;
    case 0xc:
        goto L_c796;
    case 0xe:
        goto L_c796;
    case 0x10:
        goto L_c796;
    case 0x12:
        goto L_c796;
    case 0x14:
        goto L_c3f0;
    case 0x16:
        goto L_c3f0;
    case 0x18:
        goto L_c796;
    case 0x1a:
        goto L_c796;
    case 0x1c:
        goto L_c796;
    case 0x1e:
        goto L_c796;
    case 0x20:
        goto L_c796;
    case 0x22:
        goto L_c796;
    case 0x24:
        goto L_c796;
    case 0x26:
        goto L_c796;
    case 0x28:
        goto L_c796;
    case 0x2a:
        goto L_c796;
    case 0x2c:
        goto L_c796;
    case 0x2e:
        goto L_ba30;
    case 0x30:
        goto L_b819;
    case 0x32:
        goto L_ad1d;
    case 0x34:
        goto L_c796;
    case 0x36:
        goto L_aa16;
    case 0x38:
        goto L_c796;
    case 0x3a:
        goto L_a3a8;
    case 0x3c:
        goto L_c287;
    case 0x3e:
        goto L_c796;
    case 0x40:
        goto L_c796;
    case 0x42:
        goto L_c796;
    case 0x44:
        goto L_c69b;
    case 0x46:
        goto L_c50b;
    case 0x48:
        goto L_c65c;
    case 0x4a:
        goto L_b819;
    case 0x4c:
        goto L_a7dd;
    case 0x4e:
        goto L_c796;
    case 0x50:
        goto L_c796;
    case 0x52:
        goto L_c796;
    case 0x54:
        goto L_c3b5;
    case 0x56:
        goto L_a9bc;
    case 0x58:
        goto L_a852;
    case 0x5a:
        goto L_c796;
    case 0x5c:
        goto L_a807;
    }

L_c796:
    return 1;
}

int16_t FLoadLogFile(char *pszLog) {
    uint16_t hres;
    jmp_buf *penvMemSav[9];
    jmp_buf  env[9];
    int16_t  fRet;
    int16_t  cbLog;
    int16_t  iCur;
    MSGPLR  *lpmp;
    uint16_t hrsrc;
    int16_t  cSkip;
    int16_t  t_c8f8;
    void    *t_call_cbfc;

L_c7a2:
    fRet = 1;
    imemLogCur = 0;
    imemLogPrev = -1;
    penvMemSav = penvMem;
    penvMem = &(env);
    if ((setjmp(env) == 0))
        goto L_c827;
    else
        goto L_c7dc;

L_c7dc:
    penvMem = penvMemSav;
    if ((LOWORD(vlpMemStream) != 0x0))
        goto L_c7f6;
    else
        goto L_c7ec;

L_c7ec:
    if ((HIWORD(vlpMemStream) == 0x0))
        goto L_c80c;
    else
        goto L_c7f6;

L_c7f6:
    GlobalUnlock(hres);
    FreeResource(hres);
    return 0;

L_c80c:
    if ((hf != -1))
        goto L_c81c;
    else
        goto L_c816;

L_c816:
    return 1;

L_c81c:
    StreamClose();
    return 0;

L_c827:
    if ((game.fTutorial == 0x0))
        goto StrOpen;
    else
        goto L_c83b;

L_c83b:
    if ((idPlayer != 0))
        goto StrOpen;
    else
        goto L_c845;

L_c845:
    if ((gd.fGeneratingTurn == 0x0))
        goto StrOpen;
    else
        goto L_c855;

L_c855:
    cSkip = game.turn;
    hrsrc = FindResource(hInst, MAKEINTRESOURCE(0x2711), MAKEINTRESOURCE(0x2710));
    hres = LoadResource(hInst, hrsrc);
    if ((hres != 0x0))
        goto L_c89b;
    else
        goto BailOut;

BailOut:
    penvMem = penvMemSav;
    return 0;

L_c89b:
    vlpMemStream = LockResource(hres);
    if ((LOWORD(vlpMemStream) != 0x0))
        goto L_c8c1;
    else
        goto L_c8b4;

L_c8b4:
    if ((HIWORD(vlpMemStream) == 0x0))
        goto BailOut;
    else
        goto L_c8c1;

L_c8c1:
    if ((game.turn < *(vlpMemStream)))
        goto L_c8f3;
    else
        goto L_c8d4;

L_c8d4:
    vlpMemStream = 0x0;
    GlobalUnlock(hres);
    FreeResource(hres);
    goto StrOpen;

L_c8f3:
    vlpMemStream = (vlpMemStream + 1);

L_c8f8:
    t_c8f8 = cSkip;
    cSkip = (cSkip - 1);
    if ((t_c8f8 == 0))
        goto L_c944;
    else
        goto L_c907;

L_c907:
    vlpMemStream = (vlpMemStream + ((LOWORD(vlpMemStream) & 0x3ff) + 0x2));
    if ((((LOWORD(vlpMemStream) >> 0xa) & 0x3f) == 0x8))
        goto L_c8f8;
    else
        goto L_c92c;

L_c92c:
    goto L_c907;

StrOpen:
    StreamOpen(pszLog, 16416);

L_c944:
    ReadRt();
    if ((LOWORD(game.lid) != rgbCur[4]))
        goto FailSuccess;
    else
        goto L_c959;

L_c959:
    if ((HIWORD(game.lid) != rgbCur[6]))
        goto FailSuccess;
    else
        goto L_c962;

L_c962:
    if ((game.turn <= rgbCur[10]))
        goto L_c9b2;
    else
        goto FailSuccess;

FailSuccess:
    if ((LOWORD(vlpMemStream) != 0x0))
        goto L_c982;
    else
        goto L_c978;

L_c978:
    if ((HIWORD(vlpMemStream) == 0x0))
        goto L_c9a1;
    else
        goto L_c982;

L_c982:
    vlpMemStream = 0x0;
    GlobalUnlock(hres);
    FreeResource(hres);
    goto L_c9a6;

L_c9a1:
    StreamClose();

L_c9a6:
    penvMem = penvMemSav;
    return 1;

L_c9b2:
    if ((rgbCur[10] == game.turn))
        goto L_c9d0;
    else
        goto L_c9be;

L_c9be:
    FileError(idmForcesDiedValiantlyTakingManyVerminThem);
    goto FailSuccess;

L_c9d0:
    if ((((rgbCur[14] >> 0xd) & 0x7) == game.wGen))
        goto L_c9fd;
    else
        goto L_c9ee;

L_c9ee:
    FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
    goto FailSuccess;

L_c9fd:
    wVersFile = rgbCur[8];
    gd.fFileCrippled = ((rgbCur[0xe] >> 0xc) & 0x1);
    if ((gd.fGeneratingTurn == 0x0))
        goto L_ca7f;
    else
        goto L_ca3a;

L_ca3a:
    rgplr[idPlayer].wFlags = ((rgplr[idPlayer].wFlags & 0xfffd) | ((((rgbCur[0xe] >> 0xc) & 0x1) & 0x1) * 0x2));

L_ca7f:
    ReadRt();
    cbLog = LOWORD(rgbCur);
    if ((gd.fGeneratingTurn == 0x0))
        goto L_cb37;
    else
        goto L_ca9a;

L_ca9a:
    if ((LOWORD(vrgts) != 0x0))
        goto L_caae;
    else
        goto L_caa4;

L_caa4:
    if ((HIWORD(vrgts) == 0x0))
        goto L_cb37;
    else
        goto L_caae;

L_caae:
    fmemset(vrgts[idPlayer], 0, 0x10);
    if ((hdrCur.cb != 0x11))
        goto L_cb37;
    else
        goto L_cae3;

L_cae3:
    vrgts[idPlayer].lSerialNumber = rgbCur[2];
    fmemcpy(vrgts[idPlayer].rgbConfig, rgbCur[6], 0xb);

L_cb37:
    iCur = 0;
    goto L_cb9a;

L_cb3f:
    ReadRt();
    fmemmove(lpLog[iCur], hdrCur, 0x2);
    fmemmove((lpLog + (iCur + 2)), rgbCur, hdrCur.cb);
    iCur = (iCur + (hdrCur.cb + 2));

L_cb9a:
    if ((iCur < cbLog))
        goto L_cb3f;
    else
        goto L_cba5;

L_cba5:
    ReadRt();
    lpmp = vlpmsgplrOut;

L_cbb5:
    if ((LOWORD(lpmp->lpmsgplrNext) != 0x0))
        goto L_cbcb;
    else
        goto L_cbc1;

L_cbc1:
    if ((HIWORD(lpmp->lpmsgplrNext) == 0x0))
        goto L_cbde;
    else
        goto L_cbcb;

L_cbcb:
    lpmp = lpmp->lpmsgplrNext;
    goto L_cbb5;

L_cbde:
    if ((hdrCur.rt != rtPlrMsg))
        goto L_cc55;
    else
        goto L_cbf1;

L_cbf1:
    t_call_cbfc = LpAlloc(hdrCur.cb, htPlrMsg);
    LOWORD(lpmp) = FAROFF(t_call_cbfc);
    HIWORD(lpmp->lpmsgplrNext) = FARSEG(t_call_cbfc);
    lpmp = lpmp->lpmsgplrNext;
    fmemcpy(lpmp, rgbCur, hdrCur.cb);
    LOWORD(lpmp) = 0x0;
    HIWORD(lpmp->lpmsgplrNext) = 0x0;
    vcmsgplrOut = (vcmsgplrOut + 1);
    ReadRt();
    goto L_cbde;

L_cc55:
    if ((hdrCur.rt == rtEOF))
        goto L_cc70;
    else
        goto L_cc68;

L_cc68:
    fRet = 0;
    goto Done;

L_cc70:
    imemLogCur = cbLog;

Done:
    if ((LOWORD(vlpMemStream) != 0x0))
        goto L_cc8a;
    else
        goto L_cc80;

L_cc80:
    if ((HIWORD(vlpMemStream) == 0x0))
        goto L_cca9;
    else
        goto L_cc8a;

L_cc8a:
    GlobalUnlock(hres);
    FreeResource(hres);
    vlpMemStream = 0x0;
    goto L_ccae;

L_cca9:
    StreamClose();

L_ccae:
    penvMem = penvMemSav;
    DirtyGame(0);
    return fRet;
}

int16_t FCheckLogFile(int16_t iplr, int16_t *pfError) {
    jmp_buf *penvMemSav[9];
    jmp_buf  env[9];
    int16_t  fRet;
    int16_t  cbLog;
    int16_t  iCur;

L_cccc:
    fRet = 1;
    imemLogCur = 0;
    imemLogPrev = -1;
    penvMemSav = penvMem;
    penvMem = &(env);
    if ((setjmp(env) == 0))
        goto L_cd2e;
    else
        goto L_cd06;

L_cd06:
    penvMem = penvMemSav;
    if ((hf != -1))
        goto L_cd1c;
    else
        goto L_cd16;

L_cd16:
    return 1;

L_cd1c:
    StreamClose();
    *(pfError) = 3;
    return 0;

L_cd2e:
    idsFileError = 0;
    if ((FOpenFile(dtLog, iplr, 32) != 0))
        goto L_cd67;
    else
        goto L_cd4f;

L_cd4f:
    if ((idsFileError == 4))
        goto L_cd61;
    else
        goto L_cd59;

L_cd59:
    *(pfError) = idsFileError;

L_cd61:
    return 0;

L_cd67:
    ReadRt();
    cbLog = LOWORD(rgbCur);
    iCur = 0;
    goto L_cd8b;

L_cd7a:
    ReadRt();
    iCur = (iCur + (hdrCur.cb + 2));

L_cd8b:
    if ((iCur < cbLog))
        goto L_cd7a;
    else
        goto L_cd96;

L_cd96:
    ReadRt();

L_cd9b:
    if ((hdrCur.rt != rtPlrMsg))
        goto L_cdb6;
    else
        goto L_cdae;

L_cdae:
    ReadRt();
    goto L_cd9b;

L_cdb6:
    if ((hdrCur.rt == rtEOF))
        goto L_cdd8;
    else
        goto L_cdc9;

L_cdc9:
    *(pfError) = 3;
    fRet = 0;
    goto Done;

L_cdd8:
    imemLogCur = cbLog;

Done:
    StreamClose();
    penvMem = penvMemSav;
    return fRet;
}

int16_t FWriteLogFile(char *pszFileBase, int16_t iPlayer) {
    jmp_buf *penvMemSav[9];
    jmp_buf  env[9];
    int16_t  iCur;
    HDR     *lprts;
    RTLOGHDR rtlh;
    MSGPLR  *lpmp;
    int16_t  cb;
    int16_t  t_cfc9;

L_cdf6:
    iCur = 0;
    if ((iPlayer != idPlayer))
        goto L_ce91;
    else
        goto L_ce0f;

L_ce0f:
    if ((rgplr[iPlayer].fAi != 0x0))
        goto L_ce91;
    else
        goto L_ce2d;

L_ce2d:
    if ((hdrPrev.rt == rtLogPlayerZpq1))
        goto L_ce91;
    else
        goto L_ce40;

L_ce40:
    cb = (26 - ((12 - vrgZipProd[0x0].cpq) * 2));
    /* untranslated: branch memcmp(((0x59a2 + loword((192 * iPlayer))) + 0x56), part[0xe:2](vrgZipProd[0x0]), cb) == 0 ? L_ce91 : L_ce7e */

L_ce7e:
    /* untranslated: call WriteMemRt(46, cb, part[0xe:2](vrgZipProd[0x0])) -> callresult(void) */

L_ce91:
    strcpy(szBase, pszFileBase);
    if ((FCreateFile(dtLog, iPlayer, 0x0) != 0))
        goto L_cee2;
    else
        goto L_cebb;

L_cebb:
    AlertSz(PszFormatIds(idsUnableCreateLogFile, 0x0), MB_ICONHAND);
    return 0;

L_cee2:
    penvMemSav = penvMem;
    penvMem = &(env);
    if ((setjmp(env) == 0))
        goto L_cf13;
    else
        goto L_cf02;

L_cf02:
    penvMem = penvMemSav;
    StreamClose();
    return 0;

L_cf13:
    rtlh.cbLog = imemLogCur;
    rtlh.lSerialNumber = vSerialNumber;
    memcpy(&(rtlh.rgbConfig), vrgbEnvCur, 0xb);
    WriteRt(0x9, 17, &(rtlh));
    goto L_cfab;

L_cf54:
    lprts = (lpLog + iCur);
    WriteRt(lprts->rt, lprts->cb, (lpLog + (iCur + 2)));
    iCur = (iCur + (lprts->cb + 2));

L_cfab:
    if ((iCur < imemLogCur))
        goto L_cf54;
    else
        goto L_cfb6;

L_cfb6:
    iCur = vcmsgplrOut;
    lpmp = vlpmsgplrOut;

L_cfc9:
    t_cfc9 = iCur;
    iCur = (iCur - 1);
    if ((t_cfc9 == 0))
        goto L_d010;
    else
        goto L_cfd8;

L_cfd8:
    WriteRt(rtPlrMsg, (abs(lpmp->cLen) + 12), lpmp);
    lpmp = lpmp->lpmsgplrNext;
    goto L_cfc9;

L_d010:
    WriteRt(rtEOF, 0, 0x0);
    StreamClose();
    penvMem = penvMemSav;
    DirtyGame(0);
    gd.fWriteTurnNum = 0x1;
    return 1;
}

int16_t FWriteTutorialMFile(int16_t iTurn) {
    uint16_t hrsrc;
    char     szT[30];
    uint16_t hres;
    jmp_buf *penvMemSav[9];
    jmp_buf  env[9];
    int16_t  cch;
    int16_t  cSkip;
    int16_t  t_d1a3;
    char    *t_merge_d202_0001;

L_d058:
    cSkip = iTurn;
    penvMemSav = penvMem;
    penvMem = &(env);
    if ((setjmp(env) == 0))
        goto L_d0d2;
    else
        goto L_d087;

L_d087:
    penvMem = penvMemSav;
    if ((LOWORD(vlpMemStream) != 0x0))
        goto L_d0a1;
    else
        goto L_d097;

L_d097:
    if ((HIWORD(vlpMemStream) == 0x0))
        goto L_d0b7;
    else
        goto L_d0a1;

L_d0a1:
    GlobalUnlock(hres);
    FreeResource(hres);
    return 0;

L_d0b7:
    if ((hf != -1))
        goto L_d0c7;
    else
        goto L_d0c1;

L_d0c1:
    return 1;

L_d0c7:
    StreamClose();
    return 0;

L_d0d2:
    if ((iTurn >= 32))
        goto L_d0fa;
    else
        goto L_d0db;

L_d0db:
    hrsrc = FindResource(hInst, MAKEINTRESOURCE(0x2713), MAKEINTRESOURCE(0x2712));
    goto L_d11a;

L_d0fa:
    hrsrc = FindResource(hInst, MAKEINTRESOURCE(0x2715), MAKEINTRESOURCE(0x2714));
    cSkip = (cSkip - 32);

L_d11a:
    hres = LoadResource(hInst, hrsrc);
    if ((hres != 0x0))
        goto L_d13e;
    else
        goto BailOut;

BailOut:
    penvMem = penvMemSav;
    return 0;

L_d13e:
    vlpMemStream = LockResource(hres);
    if ((LOWORD(vlpMemStream) != 0x0))
        goto L_d164;
    else
        goto L_d157;

L_d157:
    if ((HIWORD(vlpMemStream) == 0x0))
        goto BailOut;
    else
        goto L_d164;

L_d164:
    if ((cSkip < *(vlpMemStream)))
        goto L_d19e;
    else
        goto L_d176;

L_d176:
    vlpMemStream = 0x0;
    GlobalUnlock(hres);
    FreeResource(hres);
    penvMem = penvMemSav;
    return 2;

L_d19e:
    vlpMemStream = (vlpMemStream + 1);

L_d1a3:
    t_d1a3 = cSkip;
    cSkip = (cSkip - 1);
    if ((t_d1a3 == 0))
        goto L_d1dd;
    else
        goto L_d1b2;

L_d1b2:
    vlpMemStream = (vlpMemStream + ((LOWORD(vlpMemStream) & 0x3ff) + 0x2));
    if ((((LOWORD(vlpMemStream) >> 0xa) & 0x3f) == 0x8))
        goto L_d1a3;
    else
        goto L_d1d7;

L_d1d7:
    goto L_d1b2;

L_d1dd:
    cch = CchGetString(idsTutorial, szT);
    if ((iTurn != 37))
        goto L_d1ff;
    else
        goto L_d1f9;

L_d1f9:
    t_merge_d202_0001 = 0x9aa;
    goto L_d202;

L_d1ff:
    t_merge_d202_0001 = 0x9af;

L_d202:
    strcpy(&(szT[cch]), t_merge_d202_0001);
    StreamOpen(szT, 4114);

L_d224:
    RgToStream(vlpMemStream, ((LOWORD(vlpMemStream) & 0x3ff) + 0x2));
    vlpMemStream = (vlpMemStream + ((LOWORD(vlpMemStream) & 0x3ff) + 0x2));
    if ((((LOWORD(vlpMemStream) >> 0xa) & 0x3f) != 0x8))
        goto L_d224;
    else
        goto L_d26a;

L_d26a:
    StreamClose();
    vlpMemStream = 0x0;
    GlobalUnlock(hres);
    FreeResource(hres);
    penvMem = penvMemSav;
    return 1;
}

int16_t FWriteHistFile(int16_t iPlayer) {
    PLANET    *lppl;
    int16_t    i;
    jmp_buf   *penvMemSav[9];
    jmp_buf    env[9];
    uint16_t   cTurnBase;
    SHDEF     *lpshdef;
    int16_t    j;
    RTHISTHDR  rthh;
    uint8_t   *lpb;
    RecordType t_merge_d39d_0001;

L_d29e:
    if ((FCreateFile(dtHist, iPlayer, 0x0) != 0))
        goto L_d2e9;
    else
        goto L_d2c2;

L_d2c2:
    AlertSz(PszFormatIds(idsUnableCreateHistoryFile, 0x0), MB_ICONHAND);
    return 0;

L_d2e9:
    penvMemSav = penvMem;
    penvMem = &(env);
    if ((setjmp(env) == 0))
        goto L_d31a;
    else
        goto L_d309;

L_d309:
    penvMem = penvMemSav;
    StreamClose();
    return 0;

L_d31a:
    rthh.cPlanet = cPlanet;
    rthh.cPlanetExtra = rgplr[iPlayer].cFleet;
    WriteRt(rtHistHdr, 4, &(rthh));
    i = 0;
    lppl = lpPlanets;
    goto L_d373;

L_d360:
    i = (i + 1);
    lppl = (lppl + 1);

L_d373:
    if ((i >= cPlanet))
        goto L_d3af;
    else
        goto L_d37e;

L_d37e:
    if ((lppl->det < 0x3))
        goto L_d39a;
    else
        goto L_d394;

L_d394:
    t_merge_d39d_0001 = rtPlanetB;
    goto L_d39d;

L_d39a:
    t_merge_d39d_0001 = 0xf;

L_d39d:
    WritePlanet(lppl, t_merge_d39d_0001, 1);
    goto L_d360;

L_d3af:
    WriteRt(rtMsgFilt, cbbitfMsg, bitfMsgFiltered);
    i = 0;
    goto L_d3d2;

L_d3ce:
    i = (i + 1);

L_d3d2:
    if ((i >= game.cPlayer))
        goto L_d41f;
    else
        goto L_d3dd;

L_d3dd:
    if ((i == iPlayer))
        goto L_d3ce;
    else
        goto L_d3e8;

L_d3e8:
    if ((rgplr[i].det == 0x0))
        goto L_d3ce;
    else
        goto L_d404;

L_d404:
    WriteRtPlr(&(rgplr[i]), 0x0);
    goto L_d3ce;

L_d41f:
    i = 0;
    goto L_d42b;

L_d427:
    i = (i + 1);

L_d42b:
    if ((i >= game.cPlayer))
        goto L_d4cc;
    else
        goto L_d436;

L_d436:
    if ((rgplr[i].fInclude == 0x0))
        goto L_d427;
    else
        goto L_d454;

L_d454:
    if ((i == iPlayer))
        goto L_d427;
    else
        goto L_d45f;

L_d45f:
    lpshdef = rglpshdef[i];
    j = 0;
    goto L_d4c0;

L_d47c:
    if ((lpshdef[j].fFree != 0x0))
        goto L_d4bc;
    else
        goto L_d4a0;

L_d4a0:
    WriteRtShDef(&(lpshdef[j]), 0x0);

L_d4bc:
    j = (j + 1);

L_d4c0:
    if ((j >= 16))
        goto L_d427;
    else
        goto L_d4c6;

L_d4c6:
    goto L_d47c;

L_d4cc:
    i = 0;
    goto L_d4d8;

L_d4d4:
    i = (i + 1);

L_d4d8:
    if ((i >= game.cPlayer))
        goto L_d579;
    else
        goto L_d4e3;

L_d4e3:
    if ((rgplr[i].fInclude == 0x0))
        goto L_d4d4;
    else
        goto L_d501;

L_d501:
    if ((i == iPlayer))
        goto L_d4d4;
    else
        goto L_d50c;

L_d50c:
    lpshdef = rglpshdefSB[i];
    j = 0;
    goto L_d56d;

L_d529:
    if ((lpshdef[j].fFree != 0x0))
        goto L_d569;
    else
        goto L_d54d;

L_d54d:
    WriteRtShDef(&(lpshdef[j]), 0x0);

L_d569:
    j = (j + 1);

L_d56d:
    if ((j >= 10))
        goto L_d4d4;
    else
        goto L_d573;

L_d573:
    goto L_d529;

L_d579:
    if ((game.turn > 0x64))
        goto L_d58b;
    else
        goto L_d583;

L_d583:
    cTurnBase = 0x0;
    goto L_d594;

L_d58b:
    cTurnBase = (game.turn + 0xff9c);

L_d594:
    i = 0;
    goto L_d5a0;

L_d59c:
    i = (i + 1);

L_d5a0:
    if ((i >= game.cPlayer))
        goto L_d637;
    else
        goto L_d5ab;

L_d5ab:
    if ((LOWORD(rgsxPlr[i]) != 0x0))
        goto L_d5c6;
    else
        goto L_d5bc;

L_d5bc:
    if ((HIWORD(rgsxPlr[i]) == 0x0))
        goto L_d59c;
    else
        goto L_d5c6;

L_d5c6:
    j = 0;
    goto L_d5d2;

L_d5ce:
    j = (j + 1);

L_d5d2:
    if ((j >= rgcsxPlr[i]))
        goto L_d59c;
    else
        goto L_d5e3;

L_d5e3:
    if ((rgsxPlr[i][j].turn < cTurnBase))
        goto L_d5ce;
    else
        goto L_d608;

L_d608:
    WriteRt(rtScore, 24, rgsxPlr[i][j]);

L_d631:
    goto L_d5ce;

L_d637:
    if ((LOWORD(vlpbAiData) != 0x0))
        goto L_d64b;
    else
        goto L_d641;

L_d641:
    if ((HIWORD(vlpbAiData) == 0x0))
        goto L_d6b1;
    else
        goto L_d64b;

L_d64b:
    if ((LOWORD(vlpbAiData) <= 0x2))
        goto L_d6b1;
    else
        goto L_d658;

L_d658:
    i = LOWORD(vlpbAiData);
    lpb = vlpbAiData;
    goto L_d692;

L_d672:
    WriteRt(rtAiData, 1023, lpb);
    lpb = (lpb + 1023);
    i = (i - 1023);

L_d692:
    if ((i >= 1024))
        goto L_d672;
    else
        goto L_d69c;

L_d69c:
    WriteRt(rtAiData, i, lpb);

L_d6b1:
    WriteRt(rtEOF, 0, 0x0);
    StreamClose();
    penvMem = penvMemSav;
    return 1;
}

void EnumLogRts(int16_t (**pfn)(void *, int16_t, int16_t, void *, int16_t), void *lpPass, int16_t iPass) {
    int16_t fLogOld;
    int16_t fRet;
    int16_t iCur;
    HDR    *lprts;

L_d6e0:
    iCur = 0;
    fRet = 1;
    fLogOld = fLogOff;
    if ((imemLogCur == 0))
        goto L_d77d;
    else
        goto L_d700;

L_d700:
    goto L_d772;

L_d709:
    lprts = (lpLog + iCur);
    if ((pfn((lpLog + (iCur + 2)), lprts->rt, lprts->cb, lpPass, iPass) == 0))
        goto L_d77d;
    else
        goto L_d763;

L_d763:
    iCur = (iCur + (lprts->cb + 2));

L_d772:
    if ((iCur < imemLogCur))
        goto L_d709;
    else
        goto L_d77d;

L_d77d:
    return;
}
