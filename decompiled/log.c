#include "common.h"

void LogSplitFleet(int16_t id) {
L_8b0e:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_8b52;
    else
        goto L_8b27;

L_8b27:
    LpflFromId(id);
    /* untranslated: ss:[bp-0x6] = (faroff(callresult(FLEET *)) + 0x76) */
    /* untranslated: ss:[bp-0x4] = farseg(callresult(FLEET *)) */
    /* untranslated: farseg(dword ss:[bp-0x6]):[faroff(dword ss:[bp-0x6])] = ((farseg(dword ss:[bp-0x6]):[faroff(dword ss:[bp-0x6])] & 0xffdf) | 0x20) */
    goto L_8b66;

L_8b52:
    WriteMemRt(0x18, 0x2, id);

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
        goto L_8b82;

L_8b82:

L_8b88:
    i = 0x0;
    goto L_8c16;

L_8b90:
    LOWORD(rgid) = id;
    j = 0x1;

L_8b9d:
    if ((j >= 0x1ff))
        goto L_8bfe;
    else
        goto L_8ba8;

L_8ba8:
    if ((i >= vcflMerge))
        goto L_8bfe;
    else
        goto L_8bb3;

L_8bb3:
    if ((vrgiflMerge[i] == 0xffff))
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
    j = (j + 0x1);
    rgid[j] = idCur;

L_8bf7:
    i = (i + 0x1);
    goto L_8b9d;

L_8bfe:
    WriteMemRt(0x25, (j * 0x2), rgid);

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
        goto L_8c3e;

L_8c3e:

L_8c44:
    LOWORD(rgb) = ((LOWORD(rgb) & 0xe0ff) | ((lpshdefNew->ishdef & 0x1f) << 0x8));
    LOWORD(rgb) = ((LOWORD(rgb) & 0xff0f) | ((idPlayer & 0xf) << 0x4));
    if ((lpshdefNew->fFree == 0x0))
        goto L_8ccc;
    else
        goto L_8ca6;

L_8ca6:
    LOWORD(rgb) = ((LOWORD(rgb) & 0xfff0) | 0x0);
    WriteMemRt(0x1b, 0x2, rgb);
    goto L_8d25;

L_8ccc:
    LOWORD(rgb) = ((LOWORD(rgb) & 0xfff0) | 0x1);
    lpshdefNew->det = 0x7;
    pb = &(HIWORD(rgb));
    WriteRtShDef(lpshdefNew, &(pb));
    WriteMemRt(0x1b, rgb[pb], rgb);

L_8d25:
    if ((gd.fTutorial == 0x0))
        goto L_8d53;
    else
        goto L_8d38;

L_8d38:
    if ((idPlayer != 0x0))
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
        goto L_8d83;

L_8d83:

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
    FreeLp(&(lpfl->lpszName), htString);

L_8db7:
    if ((szName == 0x0))
        goto L_8dce;
    else
        goto L_8dc0;

L_8dc0:
    if (((uint16_t)(*(szName)) != 0x0))
        goto L_8ded;
    else
        goto L_8dce;

L_8dce:
    /* untranslated: part[4:1](rtchgname) = 0x0 */
    /* untranslated: part[5:1](rtchgname) = 0x0 */
    cOut = 0x1;
    lpfl->lpszName = 0x0;
    goto L_8e7d;

L_8ded:
    cOut = strlen(szName);
    lpfl->lpszName = LpAlloc((strlen(szName) + 0x1), htString);
    fstrcpy(lpfl->lpszName, szName);
    /* untranslated: branch FCompressUserString(szName, &part[5:2](rtchgname), &cOut) == 0x0 ? L_8e66 : L_8e5d */

L_8e5d:
    /* untranslated: part[4:1](rtchgname) = lobyte(cOut) */
    goto L_8e7d;

L_8e66:
    /* untranslated: part[4:1](rtchgname) = 0x0 */
    /* untranslated: call strcpy(&part[5:2](rtchgname), szName) -> callresult(char *) */
    cOut = (cOut + 0x1);

L_8e7d:
    rtchgname.grobj = grobj;
    rtchgname.id = id;
    WriteMemRt(0x2c, (cOut + 0x5), rtchgname);
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
    int16_t   t_9229;
    int16_t   t_92f8;

L_8ebe:
    fChg = 0x0;
    if ((gd.fGeneratingTurn != 0x0))
        goto L_933a;
    else
        goto L_8ed9;

L_8ed9:

L_8edf:
    lxfNew.id = LOWORD(pfl);
    lxfNew.grobj = grobjFleet;
    i = 0x0;
    goto L_8f3a;

L_8ef5:
    lxfNew.rgdItem[i] = (pflNew->rgcsh[i] - pfl->rgcsh[i]);
    if (((pflNew->rgcsh[i] - pfl->rgcsh[i]) == 0x0))
        goto L_8f36;
    else
        goto L_8f31;

L_8f31:
    fChg = 0x1;

L_8f36:
    i = (i + 0x1);

L_8f3a:
    if ((i < 0x10))
        goto L_8ef5;
    else
        goto L_8f43;

L_8f43:
    if ((fChg == 0x0))
        goto L_8f88;
    else
        goto L_8f4c;

L_8f4c:
    if ((fValidLxf == 0x0))
        goto L_8f6f;
    else
        goto L_8f56;

L_8f56:
    LogMakeValidXferf(0x24f4, &(lxfNew));
    fValidLxf = 0x0;
    goto L_933a;

L_8f6f:
    lxf = lxfNew;
    fValidLxf = 0x1;

L_8f88:
    lxNew.id = LOWORD(pfl);
    lxNew.grobj = grobjFleet;
    i = 0x0;
    goto L_8ffb;

L_8f9e:
    lxNew.rgdItem[i] = (pflNew->rgwtMin[i] - pfl->rgwtMin[i]);
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
    fChg = 0x1;

L_8ff7:
    i = (i + 0x1);

L_8ffb:
    if ((i < 0x5))
        goto L_8f9e;
    else
        goto L_9004;

L_9004:
    if ((fChg == 0x0))
        goto L_9049;
    else
        goto L_900d;

L_900d:
    if ((fValidLx == 0x0))
        goto L_9030;
    else
        goto L_9017;

L_9017:
    LogMakeValidXfer(0x24ca, &(lxNew));
    fValidLx = 0x0;
    goto L_933a;

L_9030:
    lx = lxNew;
    fValidLx = 0x1;

L_9049:
    if ((pfl->iplan == pflNew->iplan))
        goto L_9093;
    else
        goto L_906b;

L_906b:
    rtsi.id = pflNew;
    rtsi.i = pflNew->iplan;
    WriteMemRt(0x2a, 0x4, rtsi);

L_9093:
    if ((pfl->fRepOrders == pflNew->fRepOrders))
        goto L_90e4;
    else
        goto L_90b7;

L_90b7:
    rtsi.id = pflNew;
    rtsi.i = pflNew->fRepOrders;
    WriteMemRt(0xa, 0x4, rtsi);

L_90e4:
    d = (pflNew->cord - pfl->cord);
    iordOld = 0x0;
    goto L_9100;

L_90fc:
    iordOld = (iordOld + 0x1);

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
    if ((fmemcmp(&(pfl->lpplord->rgord[iordOld]), &(pflNew->lpplord->rgord[iordOld]), 0x12) != 0x0))
        goto L_9181;
    else
        goto L_9178;

L_9178:

L_9181:
    iordNew = iordOld;
    if ((iordOld != pfl->cord))
        goto L_91a2;
    else
        goto L_9196;

L_9196:
    if ((d == 0x0))
        goto L_933a;
    else
        goto L_919c;

L_919c:

L_91a2:
    if ((d >= 0x0))
        goto L_91de;
    else
        goto L_91ab;

L_91ab:
    rtsi.id = pflNew;
    rtsi.i = iordOld;
    if ((d != 0xfffe))
        goto L_91c7;
    else
        goto L_91c2;

L_91c2:
    rtsi.i = (rtsi.i | 0x8000);

L_91c7:
    WriteMemRt(0x3, 0x4, rtsi);
    goto L_933a;

L_91de:
    if ((d <= 0x0))
        goto L_926b;
    else
        goto L_91e7;

L_91e7:
    cbWp = 0x16;
    rtwp.id = pflNew;
    rtwp.iWaypt = iordNew;
    rtwp.order = pflNew->lpplord->rgord[iordNew];
    pbWp = rtwp;

L_9229:
    t_9229 = cbWp;
    cbWp = (cbWp - 0x1);
    if ((t_9229 <= 0x0))
        goto L_9251;
    else
        goto L_9238;

L_9238:
    if (((uint16_t)(pbWp[cbWp]) != 0x0))
        goto L_9251;
    else
        goto L_9248;

L_9248:

L_9251:
    cbWp = (cbWp + 0x1);
    WriteMemRt(0x4, cbWp, rtwp);
    goto L_933a;

L_926b:
    cbWp = 0x16;
    if ((FGetPrevLogRt(&(hdr), 0x4b98) == 0x0))
        goto L_92bb;
    else
        goto L_9288;

L_9288:
    if ((hdr.rt != rtLogFleetOrderUpdate))
        goto L_92bb;
    else
        goto L_929b;

L_929b:
    if ((LOWORD(rgbCur) != pflNew))
        goto L_92bb;
    else
        goto L_92a9;

L_92a9:
    if ((HIWORD(rgbCur) != iordNew))
        goto L_92bb;
    else
        goto L_92b5;

L_92b5:
    imemLogCur = imemLogPrev;

L_92bb:
    rtwp.id = pflNew;
    rtwp.iWaypt = iordNew;
    rtwp.order = pflNew->lpplord->rgord[iordNew];
    pbWp = rtwp;

L_92f8:
    t_92f8 = cbWp;
    cbWp = (cbWp - 0x1);
    if ((t_92f8 <= 0x0))
        goto L_9320;
    else
        goto L_9307;

L_9307:
    if (((uint16_t)(pbWp[cbWp]) != 0x0))
        goto L_9320;
    else
        goto L_9317;

L_9317:

L_9320:
    cbWp = (cbWp + 0x1);
    WriteMemRt(0x5, cbWp, rtwp);

L_933a:
    return;
}

void LogChangeRelations() {
    HDR hdr;

L_9340:
    if ((FGetPrevLogRt(&(hdr), 0x4b98) == 0x0))
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
    /* untranslated: call WriteMemRt(0x26, game.cPlayer, part[112:0](rgplr[idPlayer])) -> callresult(void) */
    if ((gd.fTutorial == 0x0))
        goto L_93ca;
    else
        goto L_93af;

L_93af:
    if ((idPlayer != 0x0))
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
    WriteBattlePlan(pbtlplan, 0x1);
    if ((gd.fTutorial == 0x0))
        goto L_941a;
    else
        goto L_93ff;

L_93ff:
    if ((idPlayer != 0x0))
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
    int16_t i;
    int16_t fChg;
    HDR     hdr;
    LOGXFER lxNew;

L_9420:
    fChg = 0x0;
    if ((gd.fGeneratingTurn != 0x0))
        goto L_9901;
    else
        goto L_943b;

L_943b:

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
    if ((fValidLx == 0x0))
        goto L_9901;
    else
        goto L_945a;

L_945a:

L_9460:
    lxNew.id = 0xffff;
    lxNew.grobj = grobjOther;
    i = 0x0;
    goto L_949f;

L_9472:
    /* untranslated: LOWORD(lxNew.rgdItem[i]) = (LOWORD(lx.rgdItem[i]) neg 0x0) */
    /* untranslated: HIWORD(lxNew.rgdItem[i]) = ((HIWORD(lx.rgdItem[i]) + 0x0) neg 0x0) */
    i = (i + 0x1);

L_949f:
    if ((i >= 0x5))
        goto ChgIt;
    else
        goto L_94a5;

L_94a5:

L_94ab:
    lxNew.id = ppl->id;
    lxNew.grobj = grobjPlanet;
    i = 0x0;
    goto L_951e;

L_94c1:
    lxNew.rgdItem[i] = (pplNew->rgwtMin[i] - ppl->rgwtMin[i]);
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
    fChg = 0x1;

L_951a:
    i = (i + 0x1);

L_951e:
    if ((i < 0x4))
        goto L_94c1;
    else
        goto L_9527;

L_9527:
    /* untranslated: part[20:2](lxNew) = 0x0 */
    /* untranslated: part[22:2](lxNew) = 0x0 */
    if ((fChg == 0x0))
        goto L_9573;
    else
        goto ChgIt;

ChgIt:
    if ((fValidLx == 0x0))
        goto L_955d;
    else
        goto L_9544;

L_9544:
    LogMakeValidXfer(0x24ca, &(lxNew));
    fValidLx = 0x0;
    goto L_9573;

L_955d:
    lx = lxNew;
    fValidLx = 0x1;

L_9573:
    if ((LOWORD(ppl) != 0x0))
        goto L_9588;
    else
        goto L_957c;

L_957c:
    if ((HIWORD(ppl) == 0x0))
        goto L_9901;
    else
        goto L_9582;

L_9582:

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
    WriteMemRt(0x1d, 0x2, lxNew);
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
    if ((ppl->lpplprod->iprodMac != pplNew->lpplprod->iprodMac))
        goto L_9666;
    else
        goto L_9621;

L_9621:
    if ((fmemcmp(&(ppl->lpplprod->rgprod), &(pplNew->lpplprod->rgprod), (ppl->lpplprod->iprodMac * 0x4)) == 0x0))
        goto L_9705;
    else
        goto L_9666;

L_9666:
    if ((FGetPrevLogRt(&(hdr), 0x4b98) == 0x0))
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
    fmemmove(0x4b9a, &(pplNew->lpplprod->rgprod), (pplNew->lpplprod->iprodMac * 0x4));
    WriteMemRt(0x1d, ((pplNew->lpplprod->iprodMac * 0x4) + 0x2), 0x4b98);

L_9705:
    /* untranslated: branch (loword((uint32_t)(words(*(ppl+0x18), *(ppl+0x1a)) >> 0x17)) & 0x1) != (loword((uint32_t)(words(*(pplNew+0x18), *(pplNew+0x1a)) >>
     * 0x17)) & 0x1) ? L_97b5 : L_9754 */

L_9754:
    /* untranslated: branch ss:[bp-0x26] != (hiword(callresult(uint32_t)) & 0x0) ? L_97b5 : L_975b */

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
    LOWORD(rgbCur) = pplNew->id;
    HIWORD(rgbCur) = 0x0;
    /* untranslated: part[4:2](rgbCur) = 0x0 */
    /* untranslated: HIWORD(rgbCur) = ((HIWORD(rgbCur) & 0xfffe) | loword((int32_t)(words(((loword((uint32_t)(words(*(pplNew+0x18), *(pplNew+0x1a)) >> 0x17)) &
     * 0x1) & 0x1), ((hiword((uint32_t)(words(*(pplNew+0x18), *(pplNew+0x1a)) >> 0x17)) & 0x0) & 0x0)) << 0x0))) */
    /* untranslated: part[4:2](rgbCur) = ((part[4:2](rgbCur) & 0xffff) | hiword((int32_t)(words((ss:[bp-0x24] & 0x1), (ss:[bp-0x22] & 0x0)) << 0x0))) */
    /* untranslated: HIWORD(rgbCur) = ((HIWORD(rgbCur) & 0xf801) | loword((int32_t)(words((pplNew->idFling & 0x3ff), (0x0 & 0x0)) << 0x1))) */
    /* untranslated: part[4:2](rgbCur) = ((part[4:2](rgbCur) & 0xffff) | hiword((int32_t)(words((ss:[bp-0x24] & 0x3ff), (ss:[bp-0x22] & 0x0)) << 0x1))) */
    /* untranslated: HIWORD(rgbCur) = ((HIWORD(rgbCur) & 0x87ff) | loword((int32_t)(words((pplNew->iWarpFling & 0xf), (0x0 & 0x0)) << 0xb))) */
    /* untranslated: part[4:2](rgbCur) = ((part[4:2](rgbCur) & 0xffff) | hiword((int32_t)(words((ss:[bp-0x24] & 0xf), (ss:[bp-0x22] & 0x0)) << 0xb))) */
    /* untranslated: HIWORD(rgbCur) = ((HIWORD(rgbCur) & 0x7fff) | loword((int32_t)(words((pplNew->idRoute & 0x3ff), (0x0 & 0x0)) << 0xf))) */
    /* untranslated: part[4:2](rgbCur) = ((part[4:2](rgbCur) & 0xfe00) | hiword((int32_t)(words((ss:[bp-0x24] & 0x3ff), (ss:[bp-0x22] & 0x0)) << 0xf))) */
    WriteMemRt(0x23, 0x6, 0x4b98);

L_9901:
    return;
}

void LogChangeThing(THING *lpth, THING *pthNew) {
    int16_t i;
    int16_t fChg;
    LOGXFER lxNew;

L_9908:
    fChg = 0x0;
    if ((gd.fGeneratingTurn != 0x0))
        goto L_99f0;
    else
        goto L_9923;

L_9923:

L_9929:
    memset(lxNew, 0x0, 0x18);
    lxNew.id = pthNew;
    lxNew.grobj = grobjThing;
    i = 0x0;
    goto L_99a5;

L_9952:
    /* untranslated: LOWORD(lxNew.rgdItem[i]) = (0x25:[((pthNew + 0x8) + (i * 0x2))] - HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) */
    /* untranslated: HIWORD(lxNew.rgdItem[i]) = signhiword((0x25:[((pthNew + 0x8) + (i * 0x2))] - HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))])) */
    /* untranslated: branch (0x25:[((pthNew + 0x8) + (i * 0x2))] - HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) != 0x0 ? L_999c : L_9994 */

L_9994:
    /* untranslated: branch signhiword((0x25:[((pthNew + 0x8) + (i * 0x2))] - HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))])) == 0x0 ? L_99a1 : L_999c */

L_999c:
    fChg = 0x1;

L_99a1:
    i = (i + 0x1);

L_99a5:
    if ((i < 0x3))
        goto L_9952;
    else
        goto L_99ae;

L_99ae:
    if ((fChg == 0x0))
        goto L_99f0;
    else
        goto L_99b7;

L_99b7:
    if ((fValidLx == 0x0))
        goto L_99da;
    else
        goto L_99c1;

L_99c1:
    LogMakeValidXfer(0x24ca, &(lxNew));
    fValidLx = 0x0;
    goto L_99f0;

L_99da:
    lx = lxNew;
    fValidLx = 0x1;

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
    int32_t  t_merge_9d34_0001_wide;

L_99f6:
    iBiggest = 0x0;
    grbit = 0x0;
    grFlag = 0x1;
    LOWORD(rgQuan) = 0x0;
    HIWORD(rgQuan) = 0x0;
    /* untranslated: part[4:2](rgQuan) = 0x0 */
    /* untranslated: part[6:2](rgQuan) = 0x0 */
    /* untranslated: part[8:2](rgQuan) = 0x0 */
    /* untranslated: part[10:2](rgQuan) = 0x0 */
    /* untranslated: part[12:2](rgQuan) = 0x0 */
    /* untranslated: part[14:2](rgQuan) = 0x0 */
    /* untranslated: part[16:2](rgQuan) = 0x0 */
    /* untranslated: part[18:2](rgQuan) = 0x0 */
    goto L_9a7e;

L_9a53:
    prt = ((uint8_t *)(lpLog) + (imemLogCur - hdrPrev.cb));
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
    if ((prt->grobj1 != (plx1->grobj & 0xff)))
        goto L_9c94;
    else
        goto L_9ad0;

L_9ad0:
    if ((((*(prt + 0x4) >> 0x4) & 0xf) != (plx2->grobj & 0xff)))
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
    iOff = 0x0;
    goto L_9c65;

L_9b3e:
    i = 0x0;
    goto L_9b89;

L_9b46:
    if ((((0x1 << i) & grbit) == 0x0))
        goto L_9b85;
    else
        goto L_9b59;

L_9b59:
    rgQuan[i] = (uint32_t)((uint16_t)(prt->rgcQuan[iOff]));
    iOff = (iOff + 0x1);

L_9b85:
    i = (i + 0x1);

L_9b89:
    if ((i >= 0x5))
        goto L_9c80;
    else
        goto L_9b8f;

L_9b8f:

L_9b95:
    prtx = prt;
    i = 0x0;
    goto L_9bed;

L_9ba9:
    if ((((0x1 << i) & grbit) == 0x0))
        goto L_9be9;
    else
        goto L_9bbc;

L_9bbc:
    rgQuan[i] = (uint32_t)(prtx->rgcQuan[iOff]);
    iOff = (iOff + 0x1);

L_9be9:
    i = (i + 0x1);

L_9bed:
    if ((i >= 0x5))
        goto L_9c80;
    else
        goto L_9bf3;

L_9bf3:

L_9bf9:
    prtl = prt;
    i = 0x0;
    goto L_9c56;

L_9c0d:
    if ((((0x1 << i) & grbit) == 0x0))
        goto L_9c52;
    else
        goto L_9c20;

L_9c20:
    rgQuan[i] = prtl->rgcQuan[iOff];
    iOff = (iOff + 0x1);

L_9c52:
    i = (i + 0x1);

L_9c56:
    if ((i >= 0x5))
        goto L_9c80;
    else
        goto L_9c5c;

L_9c5c:

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
    i = 0x0;
    grbit = 0x0;
    goto L_9cb4;

L_9ca4:
    i = (i + 0x1);
    grFlag = (grFlag * 0x2);

L_9cb4:
    if ((i >= 0x5))
        goto L_9d60;
    else
        goto L_9cbd;

L_9cbd:
    rgQuan[i] = (rgQuan[i] + plx1->rgdItem[i]);
    if ((HIWORD(iBiggest) < HIWORD(labs(rgQuan[i]))))
        goto L_9d1b;
    else
        goto L_9d05;

L_9d05:
    /* untranslated: branch HIWORD(iBiggest) > hiword(callresult(int32_t)) ? L_9d12 : L_9d0a */

L_9d0a:
    /* untranslated: branch LOWORD(iBiggest) <= loword(callresult(int32_t)) ? L_9d1b : L_9d12 */

L_9d12:
    t_merge_9d34_0001_wide = iBiggest;
    goto L_9d34;

L_9d1b:
    t_merge_9d34_0001_wide = labs(rgQuan[i]);

L_9d34:
    iBiggest = t_merge_9d34_0001_wide;
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

L_9d60:
    if ((grbit == 0x0))
        goto L_9fa0;
    else
        goto L_9d66;

L_9d66:

L_9d6c:
    LOWORD(prt) = rgbuf;
    /* untranslated: HIWORD(prt) = ss */
    *(prt + 0x4) = LOBYTE(((*(prt + 0x4) & 0xf0) | (LOBYTE(plx1->grobj) & 0xf)));
    *(prt + 0x4) = LOBYTE((prt->grobj1 | ((LOBYTE(plx2->grobj) & 0xf) * 0x10)));
    prt->id1 = plx1->id;
    prt->id2 = plx2->id;
    prt->grbitItems = LOBYTE(grbit);
    cb = 0x6;
    iOff = 0x0;
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
    rt = 0x1;
    i = 0x0;
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
    iOff = (iOff + 0x1);
    prt->rgcQuan[iOff] = LOBYTE(LOWORD(rgQuan[i]));
    cb = (cb + 0x1);

L_9e79:
    i = (i + 0x1);

L_9e7d:
    if ((i >= 0x5))
        goto L_9f8e;
    else
        goto L_9e83;

L_9e83:

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
    rt = 0x2;
    LOWORD(prtx) = rgbuf;
    /* untranslated: HIWORD(prtx) = ss */
    i = 0x0;
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
    iOff = (iOff + 0x1);
    prtx->rgcQuan[iOff] = LOWORD(rgQuan[i]);
    cb = (cb + 0x2);

L_9f06:
    i = (i + 0x1);

L_9f0a:
    if ((i >= 0x5))
        goto L_9f8e;
    else
        goto L_9f10;

L_9f10:

L_9f16:
    rt = 0x19;
    LOWORD(prtl) = rgbuf;
    /* untranslated: HIWORD(prtl) = ss */
    i = 0x0;
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
    iOff = (iOff + 0x1);
    prtl->rgcQuan[iOff] = rgQuan[i];
    cb = (cb + 0x4);

L_9f81:
    i = (i + 0x1);

L_9f85:
    if ((i < 0x5))
        goto L_9f2e;
    else
        goto L_9f8e;

L_9f8e:
    WriteMemRt(rt, cb, rgbuf);

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
    grFlag = 0x1;
    i = 0x0;
    goto L_9fd1;

L_9fc1:
    i = (i + 0x1);
    grFlag = (grFlag * 0x2);

L_9fd1:
    if ((i >= 0x10))
        goto L_9ffa;
    else
        goto L_9fda;

L_9fda:
    if ((plxf1->rgdItem[i] == 0x0))
        goto L_9fc1;
    else
        goto L_9ff1;

L_9ff1:
    grbit = (grbit | grFlag);

L_9ffa:
    if ((grbit == 0x0))
        goto L_a101;
    else
        goto L_a000;

L_a000:

L_a006:
    prt = rgbuf;
    *(prt + 0x4) = LOBYTE(((*(prt + 0x4) & 0xf0) | (LOBYTE(plxf1->grobj) & 0xf)));
    *(prt + 0x4) = LOBYTE((prt->grobj1 | ((LOBYTE(plxf2->grobj) & 0xf) * 0x10)));
    prt->id1 = plxf1->id;
    prt->id2 = plxf2->id;
    prt->grbitItems = grbit;
    cb = 0x7;
    iOff = 0x0;
    i = 0x0;
    goto L_a0e5;

L_a0a0:
    if ((plxf1->rgdItem[i] == 0x0))
        goto L_a0e1;
    else
        goto L_a0b7;

L_a0b7:
    iOff = (iOff + 0x1);
    prt->rgcQuan[iOff] = plxf1->rgdItem[i];
    cb = (cb + 0x2);

L_a0e1:
    i = (i + 0x1);

L_a0e5:
    if ((i < 0x10))
        goto L_a0a0;
    else
        goto L_a0ee;

L_a0ee:
    WriteMemRt(0x17, cb, rgbuf);

L_a101:
    return;
}

void CancelMemRt(RecordType rt) {
L_a108:
    imemLogCur = (imemLogCur - (hdrPrev.cb + 0x2));
    hdrPrev.rt = rtEOF;
    return;
}

void WriteMemRt(int16_t rt, int16_t cb, void *rg) {
    HDR      hdr;
    uint8_t *lpv;

L_a130:
    if ((fLogOff != 0x0))
        goto L_a221;
    else
        goto L_a140;

L_a140:

L_a146:
    if ((((imemLogCur + cb) + 0x2) <= 0x7d00))
        goto L_a178;
    else
        goto L_a157;

L_a157:
    AlertSz(PszFormatIds(idsLogFileHasReachedMaximumAllowableSize, 0x0), MB_ICONHAND);

L_a178:
    DirtyGame(0x1);
    imemLogPrev = imemLogCur;
    hdr = ((hdr & 0xfc00) | (cb & 0x3ff));
    hdr = (hdr.cb | ((rt & 0x3f) << 0xa));
    lpv = lpLog;
    lpv = ((uint8_t *)(lpv) + imemLogCur);
    LOWORD(lpv) = hdr;
    if ((cb <= 0x0))
        goto L_a208;
    else
        goto L_a1e8;

L_a1e8:
    fmemcpy(&(lpv[0x2]), rg, cb);

L_a208:
    imemLogCur = (imemLogCur + (cb + 0x2));
    if ((rt == 0x0))
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
    if ((fAi != 0x0))
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
    if ((imemLogPrev != 0xffff))
        goto L_a277;
    else
        goto L_a271;

L_a271:
    return 0x0;

L_a277:
    lpv = &(lpLog[imemLogPrev]);
    *(phdr) = LOWORD(lpv);
    if ((phdr->cb <= 0x0))
        goto L_a2ca;
    else
        goto L_a2a5;

L_a2a5:
    fmemcpy(&(pb), &(lpv[0x2]), phdr->cb);

L_a2ca:
    return 0x1;
}

int16_t FRunLogFile() {
    int16_t fLogOld;
    int16_t fRet;
    int16_t iCur;
    HDR    *lprts;

L_a2d6:
    iCur = 0x0;
    fRet = 0x1;
    fLogOld = fLogOff;
    if ((imemLogCur != 0x0))
        goto L_a2ff;
    else
        goto L_a2f9;

L_a2f9:
    return 0x1;

L_a2ff:
    fLogOff = 0x1;
    goto L_a362;

L_a308:
    lprts = &(lpLog[iCur]);
    /* untranslated: fRet = (fRet & FRunLogRecord(lprts->rt, lprts->cb, &part[2:0](lpLog[iCur]))) */
    iCur = (iCur + (lprts->cb + 0x2));

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

L_a38c:
    lpxfCur = 0x0;

L_c727:
    if ((rt > rtLogPlayerZpq1))
        goto L_c796;
    else
        goto L_c72f;

L_c72f:
    goto L_ffffffff;

L_c796:

L_c79c:
    return 0x1;
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

L_c7a2:
    fRet = 0x1;
    imemLogCur = 0x0;
    imemLogPrev = 0xffff;
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
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
    return 0x0;

L_c80c:
    if ((hf != 0xffff))
        goto L_c81c;
    else
        goto L_c816;

L_c816:
    return 0x1;

L_c81c:
    StreamClose();
    return 0x0;

L_c827:
    if ((game.fTutorial == 0x0))
        goto StrOpen;
    else
        goto L_c83b;

L_c83b:
    if ((idPlayer != 0x0))
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
    return 0x0;

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
        goto L_c8bb;

L_c8bb:

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
    vlpMemStream = (vlpMemStream + 0x1);

L_c8f8:
    t_c8f8 = cSkip;
    cSkip = (cSkip - 0x1);
    if ((t_c8f8 == 0x0))
        goto L_c944;
    else
        goto L_c907;

L_c907:
    vlpMemStream = ((uint8_t *)(vlpMemStream) + ((LOWORD(vlpMemStream) & 0x3ff) + 0x2));
    if ((((LOWORD(vlpMemStream) >> 0xa) & 0x3f) == 0x8))
        goto L_c8f8;
    else
        goto L_c92c;

L_c92c:

StrOpen:
    StreamOpen(pszLog, 0x4020);

L_c944:
    ReadRt();
    /* untranslated: branch LOWORD(game) != part[4:2](rgbCur) ? L_c96e : L_c959 */

L_c959:
    /* untranslated: branch HIWORD(game) != part[6:2](rgbCur) ? L_c96e : L_c962 */

L_c962:
    /* untranslated: branch game.turn <= part[10:2](rgbCur) ? L_c9b2 : L_c96e */

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
    return 0x1;

L_c9b2:
    /* untranslated: branch part[10:2](rgbCur) == game.turn ? L_c9d0 : L_c9be */

L_c9be:
    FileError(idmForcesDiedValiantlyTakingManyVerminThem);
    goto FailSuccess;

L_c9d0:
    /* untranslated: branch ((part[14:2](rgbCur) >> 0xd) & 0x7) == game.wGen ? L_c9fd : L_c9ee */

L_c9ee:
    FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
    goto FailSuccess;

L_c9fd:
    /* untranslated: wVersFile = part[8:2](rgbCur) */
    /* untranslated: HIWORD(gd) = ((HIWORD(gd) & 0xfffb) | ((((part[14:2](rgbCur) >> 0xc) & 0x1) & 0x1) << 0x2)) */
    if ((gd.fGeneratingTurn == 0x0))
        goto L_ca7f;
    else
        goto L_ca3a;

L_ca3a:
    /* untranslated: part[84:2](rgplr[idPlayer]) = ((part[84:2](rgplr[idPlayer]) & 0xfffd) | ((((part[14:2](rgbCur) >> 0xc) & 0x1) & 0x1) * 0x2)) */

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
    fmemset(&(vrgts[idPlayer]), 0x0, 0x10);
    if ((hdrCur.cb != 0x11))
        goto L_cb37;
    else
        goto L_cae3;

L_cae3:
    LOWORD(vrgts[idPlayer]) = HIWORD(rgbCur);
    /* untranslated: HIWORD(vrgts[idPlayer]) = part[4:2](rgbCur) */
    /* untranslated: call fmemcpy(&part[4:0](vrgts[idPlayer]), 0x4b9e, 0xb) -> callresult(void *) */

L_cb37:
    iCur = 0x0;
    goto L_cb9a;

L_cb3f:
    ReadRt();
    fmemmove(&(lpLog[iCur]), hdrCur, 0x2);
    /* untranslated: call fmemmove(&part[2:0](lpLog[iCur]), rgbCur, hdrCur.cb) -> callresult(void *) */
    iCur = (iCur + (hdrCur.cb + 0x2));

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
    lpmp->lpmsgplrNext = LpAlloc(hdrCur.cb, htPlrMsg);
    lpmp = lpmp->lpmsgplrNext;
    fmemcpy(&(lpmp), rgbCur, hdrCur.cb);
    lpmp->lpmsgplrNext = 0x0;
    vcmsgplrOut = (vcmsgplrOut + 0x1);
    ReadRt();
    goto L_cbde;

L_cc55:
    if ((hdrCur.rt == rtEOF))
        goto L_cc70;
    else
        goto L_cc68;

L_cc68:
    fRet = 0x0;
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
    DirtyGame(0x0);
    return fRet;
}

int16_t FCheckLogFile(int16_t iplr, int16_t *pfError) {
    jmp_buf *penvMemSav[9];
    jmp_buf  env[9];
    int16_t  fRet;
    int16_t  cbLog;
    int16_t  iCur;

L_cccc:
    fRet = 0x1;
    imemLogCur = 0x0;
    imemLogPrev = 0xffff;
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_cd2e;
    else
        goto L_cd06;

L_cd06:
    penvMem = penvMemSav;
    if ((hf != 0xffff))
        goto L_cd1c;
    else
        goto L_cd16;

L_cd16:
    return 0x1;

L_cd1c:
    StreamClose();
    *(pfError) = 0x3;
    return 0x0;

L_cd2e:
    idsFileError = 0x0;
    if ((FOpenFile(dtLog, iplr, 0x20) != 0x0))
        goto L_cd67;
    else
        goto L_cd4f;

L_cd4f:
    if ((idsFileError == 0x4))
        goto L_cd61;
    else
        goto L_cd59;

L_cd59:
    *(pfError) = idsFileError;

L_cd61:
    return 0x0;

L_cd67:
    ReadRt();
    cbLog = LOWORD(rgbCur);
    iCur = 0x0;
    goto L_cd8b;

L_cd7a:
    ReadRt();
    iCur = (iCur + (hdrCur.cb + 0x2));

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
    *(pfError) = 0x3;
    fRet = 0x0;
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
    iCur = 0x0;
    if ((iPlayer != idPlayer))
        goto L_ce91;
    else
        goto L_ce0f;

L_ce0f:
    /* untranslated: branch ((part[6:2](rgplr[iPlayer]) >> 0x9) & 0x1) != 0x0 ? L_ce91 : L_ce2d */

L_ce2d:
    if ((hdrPrev.rt == rtLogPlayerZpq1))
        goto L_ce91;
    else
        goto L_ce40;

L_ce40:
    /* untranslated: cb = (0x1a - ((0xc - (part[15:2](vrgZipProd) & 0xff)) * 0x2)) */
    /* untranslated: branch memcmp(part[86:0](rgplr[iPlayer]), 0x2304, cb) == 0x0 ? L_ce91 : L_ce7e */

L_ce7e:
    WriteMemRt(0x2e, cb, 0x2304);

L_ce91:
    strcpy(0x56a2, pszFileBase);
    if ((FCreateFile(dtLog, iPlayer, 0x0) != 0x0))
        goto L_cee2;
    else
        goto L_cebb;

L_cebb:
    AlertSz(PszFormatIds(idsUnableCreateLogFile, 0x0), MB_ICONHAND);
    return 0x0;

L_cee2:
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_cf13;
    else
        goto L_cf02;

L_cf02:
    penvMem = penvMemSav;
    StreamClose();
    return 0x0;

L_cf13:
    rtlh.cbLog = imemLogCur;
    HIWORD(rtlh) = LOWORD(vSerialNumber);
    /* untranslated: part[4:2](rtlh) = HIWORD(vSerialNumber) */
    memcpy(rtlh.rgbConfig, 0x5468, 0xb);
    WriteRt(0x9, 0x11, &(rtlh));
    goto L_cfab;

L_cf54:
    lprts = &(lpLog[iCur]);
    /* untranslated: call WriteRt(lprts->rt, lprts->cb, &part[2:0](lpLog[iCur])) -> callresult(void) */
    iCur = (iCur + (lprts->cb + 0x2));

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
    iCur = (iCur - 0x1);
    if ((t_cfc9 == 0x0))
        goto L_d010;
    else
        goto L_cfd8;

L_cfd8:
    WriteRt(rtPlrMsg, (abs(lpmp->cLen) + 0xc), &(lpmp));
    lpmp = lpmp->lpmsgplrNext;
    goto L_cfc9;

L_d010:
    WriteRt(rtEOF, 0x0, 0x0);
    StreamClose();
    penvMem = penvMemSav;
    DirtyGame(0x0);
    gd.fWriteTurnNum = 0x1;
    return 0x1;
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
    uint16_t t_merge_d202_0001;

L_d058:
    cSkip = iTurn;
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
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
    return 0x0;

L_d0b7:
    if ((hf != 0xffff))
        goto L_d0c7;
    else
        goto L_d0c1;

L_d0c1:
    return 0x1;

L_d0c7:
    StreamClose();
    return 0x0;

L_d0d2:
    if ((iTurn >= 0x20))
        goto L_d0fa;
    else
        goto L_d0db;

L_d0db:
    hrsrc = FindResource(hInst, MAKEINTRESOURCE(0x2713), MAKEINTRESOURCE(0x2712));
    goto L_d11a;

L_d0fa:
    hrsrc = FindResource(hInst, MAKEINTRESOURCE(0x2715), MAKEINTRESOURCE(0x2714));
    cSkip = (cSkip - 0x20);

L_d11a:
    hres = LoadResource(hInst, hrsrc);
    if ((hres != 0x0))
        goto L_d13e;
    else
        goto BailOut;

BailOut:
    penvMem = penvMemSav;
    return 0x0;

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
        goto L_d15e;

L_d15e:

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
    return 0x2;

L_d19e:
    vlpMemStream = (vlpMemStream + 0x1);

L_d1a3:
    t_d1a3 = cSkip;
    cSkip = (cSkip - 0x1);
    if ((t_d1a3 == 0x0))
        goto L_d1dd;
    else
        goto L_d1b2;

L_d1b2:
    vlpMemStream = ((uint8_t *)(vlpMemStream) + ((LOWORD(vlpMemStream) & 0x3ff) + 0x2));
    if ((((LOWORD(vlpMemStream) >> 0xa) & 0x3f) == 0x8))
        goto L_d1a3;
    else
        goto L_d1d7;

L_d1d7:

L_d1dd:
    cch = CchGetString(idsTutorial, szT);
    if ((iTurn != 0x25))
        goto L_d1ff;
    else
        goto L_d1f9;

L_d1f9:
    t_merge_d202_0001 = 0x9aa;
    goto L_d202;

L_d1ff:
    t_merge_d202_0001 = 0x9af;

L_d202:
    strcpy(szT[cch], t_merge_d202_0001);
    StreamOpen(szT, 0x1012);

L_d224:
    RgToStream(&(vlpMemStream), ((LOWORD(vlpMemStream) & 0x3ff) + 0x2));
    vlpMemStream = ((uint8_t *)(vlpMemStream) + ((LOWORD(vlpMemStream) & 0x3ff) + 0x2));
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
    return 0x1;
}

int16_t FWriteHistFile(int16_t iPlayer) {
    PLANET   *lppl;
    int16_t   i;
    jmp_buf  *penvMemSav[9];
    jmp_buf   env[9];
    uint16_t  cTurnBase;
    SHDEF    *lpshdef;
    int16_t   j;
    RTHISTHDR rthh;
    uint8_t  *lpb;
    uint16_t  t_merge_d39d_0001;

L_d29e:
    if ((FCreateFile(dtHist, iPlayer, 0x0) != 0x0))
        goto L_d2e9;
    else
        goto L_d2c2;

L_d2c2:
    AlertSz(PszFormatIds(idsUnableCreateHistoryFile, 0x0), MB_ICONHAND);
    return 0x0;

L_d2e9:
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_d31a;
    else
        goto L_d309;

L_d309:
    penvMem = penvMemSav;
    StreamClose();
    return 0x0;

L_d31a:
    rthh.cPlanet = cPlanet;
    /* untranslated: rthh.cPlanetExtra = (part[4:2](rgplr[iPlayer]) & 0xfff) */
    WriteRt(rtHistHdr, 0x4, &(rthh));
    i = 0x0;
    lppl = lpPlanets;
    goto L_d373;

L_d360:
    i = (i + 0x1);
    lppl = (lppl + 0x1);

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
    t_merge_d39d_0001 = 0xe;
    goto L_d39d;

L_d39a:
    t_merge_d39d_0001 = 0xf;

L_d39d:
    WritePlanet(lppl, t_merge_d39d_0001, 0x1);
    goto L_d360;

L_d3af:
    WriteRt(rtMsgFilt, cbbitfMsg, bitfMsgFiltered);
    i = 0x0;
    goto L_d3d2;

L_d3ce:
    i = (i + 0x1);

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
    /* untranslated: branch (part[6:2](rgplr[i]) & 0x7) == 0x0 ? L_d3ce : L_d3fe */

L_d3fe:

L_d404:
    WriteRtPlr(rgplr[i], 0x0);
    goto L_d3ce;

L_d41f:
    i = 0x0;
    goto L_d42b;

L_d427:
    i = (i + 0x1);

L_d42b:
    if ((i >= game.cPlayer))
        goto L_d4cc;
    else
        goto L_d436;

L_d436:
    /* untranslated: branch ((part[6:2](rgplr[i]) >> 0x8) & 0x1) == 0x0 ? L_d427 : L_d454 */

L_d454:
    if ((i == iPlayer))
        goto L_d427;
    else
        goto L_d45f;

L_d45f:
    lpshdef = rglpshdef[i];
    j = 0x0;
    goto L_d4c0;

L_d47c:
    /* untranslated: branch ((part[123:2](lpshdef[j]) >> 0x9) & 0x1) != 0x0 ? L_d4bc : L_d4a0 */

L_d4a0:
    WriteRtShDef(&(lpshdef[j]), 0x0);

L_d4bc:
    j = (j + 0x1);

L_d4c0:
    if ((j >= 0x10))
        goto L_d427;
    else
        goto L_d4c6;

L_d4c6:

L_d4cc:
    i = 0x0;
    goto L_d4d8;

L_d4d4:
    i = (i + 0x1);

L_d4d8:
    if ((i >= game.cPlayer))
        goto L_d579;
    else
        goto L_d4e3;

L_d4e3:
    /* untranslated: branch ((part[6:2](rgplr[i]) >> 0x8) & 0x1) == 0x0 ? L_d4d4 : L_d501 */

L_d501:
    if ((i == iPlayer))
        goto L_d4d4;
    else
        goto L_d50c;

L_d50c:
    lpshdef = rglpshdefSB[i];
    j = 0x0;
    goto L_d56d;

L_d529:
    /* untranslated: branch ((part[123:2](lpshdef[j]) >> 0x9) & 0x1) != 0x0 ? L_d569 : L_d54d */

L_d54d:
    WriteRtShDef(&(lpshdef[j]), 0x0);

L_d569:
    j = (j + 0x1);

L_d56d:
    if ((j >= 0xa))
        goto L_d4d4;
    else
        goto L_d573;

L_d573:

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
    i = 0x0;
    goto L_d5a0;

L_d59c:
    i = (i + 0x1);

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
    j = 0x0;
    goto L_d5d2;

L_d5ce:
    j = (j + 0x1);

L_d5d2:
    if ((j >= rgcsxPlr[i]))
        goto L_d59c;
    else
        goto L_d5e3;

L_d5e3:
    if ((HIWORD(rgsxPlr[i][j]) < cTurnBase))
        goto L_d5ce;
    else
        goto L_d608;

L_d608:
    WriteRt(rtScore, 0x18, &(rgsxPlr[i][j]));

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
    WriteRt(rtAiData, 0x3ff, &(lpb));
    lpb = (lpb + 0x3ff);
    i = (i - 0x3ff);

L_d692:
    if ((i >= 0x400))
        goto L_d672;
    else
        goto L_d69c;

L_d69c:
    WriteRt(rtAiData, i, &(lpb));

L_d6b1:
    WriteRt(rtEOF, 0x0, 0x0);
    StreamClose();
    penvMem = penvMemSav;
    return 0x1;
}

void EnumLogRts(int16_t (**pfn)(void *, int16_t, int16_t, void *, int16_t), void *lpPass, int16_t iPass) {
    int16_t fLogOld;
    int16_t fRet;
    int16_t iCur;
    HDR    *lprts;

L_d6e0:
    iCur = 0x0;
    fRet = 0x1;
    fLogOld = fLogOff;
    if ((imemLogCur == 0x0))
        goto L_d77d;
    else
        goto L_d700;

L_d700:

L_d709:
    lprts = &(lpLog[iCur]);
    /* untranslated: branch pfn(&part[2:0](lpLog[iCur]), lprts->rt, lprts->cb, lpPass, iPass) == 0x0 ? L_d77d : L_d75d */

L_d75d:

L_d763:
    iCur = (iCur + (lprts->cb + 0x2));

L_d772:
    if ((iCur < imemLogCur))
        goto L_d709;
    else
        goto L_d77d;

L_d77d:
    return;
}
