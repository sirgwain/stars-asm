#include "common.h"

char     mpishdefishTutor[6] = {3, 4, 9, 6, 7, 14};
uint32_t bogi[25] = {0,          2758532406, 2759089752, 2759620184, 2772193450, 2772310925, 2772620565, 2774814161, 2775015431,
                     2776435914, 2777735210, 2777770221, 2777770865, 2781735025, 2781887514, 2782225790, 2782673087, 2783066231,
                     2811254769, 2811333876, 2811336841, 2811341117, 2816596978, 2816992636, 4294967295};

int16_t FReadShDef(RTSHDEF *lprt, SHDEF *lpshdef, int16_t iplrLoad) {
    char     szTemp[40];
    SHDEF    shdef;
    uint8_t *lpb;
    int16_t  ishdef;
    int16_t  cch;
    int16_t  iFirst;
    int16_t  cOut;
    int16_t  fOkay;
    HUL     *lphulBase;
    uint32_t wt;
    int16_t  c;
    HUL     *lphul;
    PART     part;

L_0006:
    memset(shdef, 0x0, 0x93);
    shdef.hul.ihuldef = lprt->ihuldef;
    shdef.wFlags = LOWORD(lprt);
    shdef.hul.chs = lprt->chs;
    shdef.hul.ibmp = lprt->ibmp;
    if ((shdef.det != 0x7))
        goto L_00e7;
    else
        goto L_0061;

L_0061:
    shdef.hul.dp = *(lprt + 0x4);
    shdef.turn = lprt->turn;
    /* untranslated: part[127:2](shdef) = LOWORD(lprt->cBuilt) */
    /* untranslated: part[129:2](shdef) = HIWORD(lprt->cBuilt) */
    /* untranslated: part[131:2](shdef) = LOWORD(lprt->cExist) */
    /* untranslated: part[133:2](shdef) = HIWORD(lprt->cExist) */
    lpb = &(lprt->rghs);
    fmemmove(&(shdef.hul.rghs), &(lpb), (lprt->chs * 0x4));
    lpb = ((uint8_t *)(lpb) + (lprt->chs * 0x4));
    goto L_0105;

L_00e7:
    shdef.hul.wtEmpty = *(lprt + 0x4);
    lpb = &(*(lprt + 0x6));

L_0105:
    iFirst = LphuldefFromId(shdef.hul.ihuldef)->hul.ibmp;
    if ((shdef.hul.ibmp < iFirst))
        goto L_013a;
    else
        goto L_012a;

L_012a:
    if ((shdef.hul.ibmp < (iFirst + 0x4)))
        goto L_0149;
    else
        goto L_013a;

L_013a:
    shdef.hul.ibmp = ((shdef.hul.ibmp & 0x3) | iFirst);

L_0149:
    cch = *(lpb);
    lpb = (lpb + 0x1);
    if ((cch != 0x0))
        goto L_0181;
    else
        goto L_0166;

L_0166:
    fstrcpy(&(shdef.hul.szClass), &(lpb));
    goto L_01d3;

L_0181:
    cOut = 0x20;
    if ((cch <= 0x20))
        goto L_0197;
    else
        goto L_0191;

L_0191:
    return 0x0;

L_0197:
    fmemmove(&(szTemp), &(lpb), cch);
    FDecompressUserString(&(szTemp), cch, &(shdef.hul.szClass), &(cOut));

L_01d3:
    ishdef = shdef.ishdef;
    if ((ishdef < 0x10))
        goto L_01f1;
    else
        goto L_01ec;

L_01ec:
    ishdef = (ishdef - 0x10);

L_01f1:
    if ((shdef.det == 0x7))
        goto L_0244;
    else
        goto L_01ff;

L_01ff:
    /* untranslated: branch ((part[123:2](lpshdef[ishdef]) >> 0x9) & 0x1) != 0x0 ? L_0244 : L_0224 */

L_0224:
    /* untranslated: branch (part[123:2](lpshdef[ishdef]) & 0xff) >= 0x7 ? L_0275 : L_0244 */

L_0244:
    lpshdef[ishdef] = shdef;
    goto L_02de;

L_0275:
    if ((shdef.hul.ihuldef != lpshdef[ishdef].hul.ihuldef))
        goto L_02b0;
    else
        goto L_0292;

L_0292:
    if ((shdef.hul.ibmp == lpshdef[ishdef].hul.ibmp))
        goto L_02de;
    else
        goto L_02b0;

L_02b0:
    lpshdef[ishdef] = shdef;

L_02de:
    if ((idPlayer == 0xffff))
        goto L_0301;
    else
        goto L_02e8;

L_02e8:
    UpdateShdefCost(&(lpshdef[ishdef]));

L_0301:
    /* untranslated: branch (part[123:2](lpshdef[ishdef]) & 0xff) != 0x7 ? L_05d5 : L_0321 */

L_0321:
    lphul = &(lpshdef[ishdef]);
    lphulBase = LphuldefFromId(lphul->ihuldef);
    LOWORD(wt) = lphulBase->wtEmpty;
    HIWORD(wt) = 0x0;
    c = 0x0;
    goto L_0370;

L_036b:
    c = (c + 0x1);

L_0370:
    if ((c >= lphul->chs))
        goto L_05c5;
    else
        goto L_0384;

L_0384:
    /* untranslated: branch ((lphul->rgTech[part[12:0](rgcrPlrHistory[c])] >> 0x8) & 0xff) <= 0x0 ? L_04f3 : L_03b1 */

L_03b1:
    part.hs.grhst = lphul->rghs[c].grhst;
    /* untranslated: HIWORD(part) = lphul->rgTech[part[12:0](rgcrPlrHistory[c])] */
    fOkay = FLookupPart(&(part));
    if ((idPlayer != 0xffff))
        goto L_03fa;
    else
        goto L_03f4;

L_03f4:
    fOkay = 0x0;

L_03fa:
    if (((part.hs.grhst & lphulBase->rghs[c].grhst) == 0x0))
        goto L_0478;
    else
        goto L_0422;

L_0422:
    if ((fOkay <= 0x1))
        goto L_043f;
    else
        goto L_042c;

L_042c:
    if ((shdef.fGift == 0x0))
        goto L_0478;
    else
        goto L_043f;

L_043f:
    /* untranslated: branch part.hs.cItem <= ((lphulBase->rgTech[part[12:0](rgcrPlrHistory[c])] >> 0x8) & 0xff) ? L_04b8 : L_0478 */

L_0478:
    /* untranslated: lphul->rgTech[part[12:0](rgcrPlrHistory[c])] = ((lphul->rgTech[part[12:0](rgcrPlrHistory[c])] & 0xff) | 0x0) */

L_04b8:
    /* untranslated: LOWORD(wt) = (LOWORD(wt) + loword((part[40:2](part[4:4](part)) * ((lphul->rgTech[part[12:0](rgcrPlrHistory[c])] >> 0x8) & 0xff)))) */
    HIWORD(wt) = (HIWORD(wt) + 0x0);

L_04f3:
    if ((c != 0x0))
        goto L_036b;
    else
        goto L_04fd;

L_04fd:
    if ((((HIWORD(lphul->rghs[0x0]) >> 0x8) & 0xff) != 0x0))
        goto L_036b;
    else
        goto L_0515;

L_0515:
    if ((lphulBase->rghs[0x0].grhst != hstEngine))
        goto L_036b;
    else
        goto L_0523;

L_0523:
    lphul->rghs[0x0].grhst = hstEngine;
    HIWORD(lphul->rghs[0x0]) = ((HIWORD(lphul->rghs[0x0]) & 0xff00) | 0x1);
    HIWORD(lphul->rghs[0x0]) = ((HIWORD(lphul->rghs[0x0]) & 0xff) | ((((HIWORD(lphulBase->rghs[0x0]) >> 0x8) & 0xff) & 0xff) << 0x8));
    part.hs.grhst = lphul->rghs[0x0].grhst;
    HIWORD(part) = HIWORD(lphul->rghs[0x0]);
    FLookupPart(&(part));
    /* untranslated: LOWORD(wt) = (LOWORD(wt) + loword((part[40:2](part[4:4](part)) * ((HIWORD(lphul->rghs[0x0]) >> 0x8) & 0xff)))) */
    HIWORD(wt) = (HIWORD(wt) + 0x0);

L_05c5:
    lphul->wtEmpty = LOWORD(wt);

L_05d5:
    return 0x1;
}

void ReadRtPlr(PLAYER *pplr, uint8_t *pbIn) {
    int16_t iOff;
    PLAYER *pplrRaw;
    int16_t cOut;
    char   *psz;

L_05e2:
    pplrRaw = pbIn;
    memset(pplr, 0x0, 0xc0);
    if ((pplrRaw->det != 0x7))
        goto L_066d;
    else
        goto L_0615;

L_0615:
    memmove(pplr, pbIn, 0x70);
    memmove((pplr + 0x70), (pbIn + 0x71), pbIn[0x70]);
    iOff = ((0x70 + pbIn[0x70]) + 0x1);
    goto L_0684;

L_066d:
    memmove(pplr, pbIn, 0x8);
    iOff = 0x8;

L_0684:
    if ((pbIn[iOff] != 0x0))
        goto L_06d2;
    else
        goto L_0699;

L_0699:
    strcpy((pplr + 0x80), ((pbIn + iOff) + 0x1));
    iOff = (iOff + (strlen((pplr + 0x80)) + 0x2));
    goto L_0721;

L_06d2:
    cOut = 0x20;
    FDecompressUserString(((pbIn + iOff) + 0x1), pbIn[iOff], (pplr + 0x80), &(cOut));
    iOff = (iOff + (pbIn[iOff] + 0x1));

L_0721:
    if ((((wVersFile >> 0x5) & 0x7f) >= 0x37))
        goto L_0784;
    else
        goto L_0739;

L_0739:
    /* untranslated: psz = PszPlayerName(0x0, (byte 0x25:[(0x175f + sext8to16(pplr->szName[0x0]))] & 0x1), 0x1, 0x0, 0x0, pplr) */
    /* untranslated: call strcpy(part[16:0](game.szName[pplr]), psz) -> callresult(char *) */
    goto L_07f7;

L_0784:
    if ((pbIn[iOff] != 0x0))
        goto L_07bb;
    else
        goto L_0799;

L_0799:
    /* untranslated: call strcpy(part[16:0](game.szName[pplr]), ((pbIn + iOff) + 0x1)) -> callresult(char *) */
    goto L_07f7;

L_07bb:
    cOut = 0x20;
    /* untranslated: call FDecompressUserString(((pbIn + iOff) + 0x1), pbIn[iOff], &part[16:0](game.szName[pplr]), &cOut) -> callresult(int16_t) */

L_07f7:
    pplr->fLearned = 0x0;
    return;
}

int16_t FLoadGame(char *pszFileName, char *pszExt) {
    int16_t  iplrSav;
    int16_t  cPlanetHist;
    STARPACK sp;
    int16_t  cPlanetAlloc;
    int16_t  fHaveHistoryData;
    jmp_buf *penvMemSav[9];
    int16_t  fSilentSav;
    PLANET  *lppl;
    int16_t  i;
    THING   *lpth;
    FLEET   *lpfl;
    jmp_buf  env[9];
    int16_t  cturn;
    THING   *lpthMac;
    int16_t  iPlayer;
    int16_t  j;
    PLANET  *lpplMac;
    int16_t  dt;
    int16_t  grf;
    int16_t  x;
    POINT    pt;
    int16_t  iplr;
    SCOREX   sx;
    int16_t  isx;
    uint16_t turnCur;
    uint8_t *lpb;
    int16_t  cThingFile;
    int16_t  fHist;
    int16_t  iP;
    int16_t  fWorking;
    int16_t  iprod;
    int16_t  iFirst;
    int16_t  iLast;
    PROD    *lpprod;
    int16_t  iWarp;
    int16_t  fTwo;
    char     szT[256];
    char     szIniFile[16];
    char     szSection[16];
    char    *psz;
    char     szEntry[16];
    uint16_t t_merge_0b9b_0001;
    uint16_t t_merge_1877_0001;
    uint16_t t_merge_1e06_0001;
    uint16_t t_merge_2580_0001;
    uint16_t t_merge_259d_0001;
    uint16_t t_merge_2e23_0001;

L_0810:
    grf = 0x0;
    cturn = 0x0;
    strcpy(0x56a2, pszFileName);
    gd.fFleetLinkValid = 0x0;
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_090f;
    else
        goto LError;

LError:
    game.fDirty = 0x0;
    DestroyCurGame();
    StreamClose();
    if ((ini.fValidate != 0x0))
        goto L_0909;
    else
        goto L_0881;

L_0881:
    if ((ini.fLogging != 0x0))
        goto L_0909;
    else
        goto L_0894;

L_0894:
    if ((hwndTitle != 0x0))
        goto L_0909;
    else
        goto L_089e;

L_089e:
    pt.x = GetSystemMetrics(SM_CXSCREEN);
    pt.y = GetSystemMetrics(SM_CYSCREEN);
    hwndTitle = CreateWindow(szTitle, "Stars!", 0x90000000, 0x0, 0x0, pt.x, pt.y, hwndFrame, 0x0, hInst, 0x0);
    fFreeingTitle = 0x0;
    ShowWindow(hwndFrame, SW_HIDE);

L_0909:
    return 0x0;

L_090f:
    if ((FOpenFile(dtXY, 0xffff, 0x20) == 0x0))
        goto LError;
    else
        goto L_0928;

L_0928:

L_092e:
    ReadRt();
    if ((hdrCur.rt == rtGame))
        goto L_096a;
    else
        goto XYCorrupt;

XYCorrupt:
    AlertSz(PszFormatIds(idsUniverseDefinitionFileSeemsMissingCorrupt, 0x0), MB_ICONHAND);
    goto LError;

L_096a:
    /* untranslated: game = part[0:64](rgbCur) */
    game.fDirty = 0x0;
    dGal = (LOWORD((0x190 * game.mdSize)) + 0x190);
    dGalInv = (dGal + 0x7d0);
    x = 0x3e8;
    i = 0x0;
    goto L_09a7;

L_09a3:
    i = (i + 0x1);

L_09a7:
    if ((i >= game.cPlanMax))
        goto L_0a60;
    else
        goto L_09b2;

L_09b2:
    RgFromStream(&(sp), 0x4);
    x = (x + sp.dx);
    rgptPlan[i].x = x;
    rgptPlan[i].y = sp.y;
    rgidPlan[i] = sp.id;
    if ((x >= (dGal + 0x3e8)))
        goto XYCorrupt;
    else
        goto L_0a30;

L_0a30:
    if ((rgptPlan[i].y >= (dGal + 0x3e8)))
        goto XYCorrupt;
    else
        goto L_0a4a;

L_0a4a:
    if ((rgidPlan[i] > 0x3e7))
        goto XYCorrupt;
    else
        goto L_0a57;

L_0a57:

L_0a60:
    ReadRt();
    if ((hdrCur.rt != rtEOF))
        goto XYCorrupt;
    else
        goto L_0a75;

L_0a75:

L_0a7b:
    StreamClose();
    if (((uint16_t)(*(pszExt)) == 0x68))
        goto L_0a9c;
    else
        goto L_0a8e;

L_0a8e:
    if (((uint16_t)(*(pszExt)) != 0x48))
        goto L_0ac7;
    else
        goto L_0a9c;

L_0a9c:
    if (((uint16_t)(pszExt[0x1]) == 0x73))
        goto L_0aba;
    else
        goto L_0aab;

L_0aab:
    if (((uint16_t)(pszExt[0x1]) != 0x53))
        goto L_0ac7;
    else
        goto L_0aba;

L_0aba:
    dt = 0x2;
    iPlayer = 0xffff;
    goto L_0ae9;

L_0ac7:
    dt = 0x3;
    grf = (grf | 0x3000);
    iPlayer = atoi(&(pszExt[0x1]));
    iPlayer = (iPlayer - 0x1);

L_0ae9:
    ResetMessages();
    memset(0x59a2, 0x0, LOWORD((game.cPlayer * 0xc0)));
    ResetHb(htShips);
    idPlayer = iPlayer;
    fSilentSav = fFileErrSilent;
    fFileErrSilent = 0x1;
    if ((iPlayer == 0xffff))
        goto LNoHistFile;
    else
        goto L_0b2e;

L_0b2e:
    if ((FOpenFile(dtHist, iPlayer, 0x20) == 0x0))
        goto LNoHistFile;
    else
        goto L_0b49;

L_0b49:
    ReadRt();
    if ((hdrCur.rt != rtHistHdr))
        goto CorruptHist;
    else
        goto L_0b5e;

L_0b5e:

L_0b64:
    cPlanetHist = LOWORD(rgbCur);
    cPlanetAlloc = (cPlanetHist + HIWORD(rgbCur));
    if ((cPlanetAlloc <= 0x3e8))
        goto L_0b83;
    else
        goto L_0b7e;

L_0b7e:
    cPlanetAlloc = 0x3e8;

L_0b83:
    if ((0x1 <= cPlanetAlloc))
        goto L_0b98;
    else
        goto L_0b92;

L_0b92:
    t_merge_0b9b_0001 = 0x1;
    goto L_0b9b;

L_0b98:
    t_merge_0b9b_0001 = cPlanetAlloc;

L_0b9b:
    lpPlanets = LpAlloc(LOWORD((t_merge_0b9b_0001 * 0x38)), htPlanets);
    ReadRt();
    i = 0x0;
    lppl = lpPlanets;
    goto L_0bdd;

L_0bca:
    i = (i + 0x1);
    lppl = (lppl + 0x1);

L_0bdd:
    if ((i >= cPlanetHist))
        goto L_0c7f;
    else
        goto L_0be8;

L_0be8:
    if ((hdrCur.rt != rtPlanetB))
        goto CorruptHist;
    else
        goto L_0bfb;

L_0bfb:
    if ((FReadPlanet(iPlayer, lppl, 0x1, 0x0) == 0x0))
        goto CorruptHist;
    else
        goto L_0c19;

L_0c19:

L_0c1f:
    if ((lppl->iPlayer != iPlayer))
        goto L_0c77;
    else
        goto L_0c2e;

L_0c2e:
    lppl->iPlayer = 0xffff;
    lppl->det = 0x3;

CorruptHist:
    StreamClose();
    AlertSz(PszFormatIds(idsHistoryFileAppearsCorruptHistoricalDataWill, 0x0), MB_ICONHAND);
    goto LNoHistFile;

L_0c77:
    ReadRt();
    goto L_0bca;

L_0c7f:
    if ((hdrCur.rt != rtMsgFilt))
        goto L_0cc2;
    else
        goto L_0c92;

L_0c92:
    if ((hdrCur.cb > cbbitfMsg))
        goto CorruptHist;
    else
        goto L_0ca0;

L_0ca0:

L_0ca6:
    memcpy(0x5324, 0x4b98, hdrCur.cb);
    ReadRt();

L_0cc2:
    if ((hdrCur.rt != rtPlr))
        goto L_0d34;
    else
        goto L_0cd5;

L_0cd5:
    i = (uint16_t)(LOWORD(rgbCur));
    ReadRtPlr(rgplr[i], 0x4b98);
    rgplr[i].cPlanet = 0x0;
    /* untranslated: part[4:2](rgplr[i]) = ((part[4:2](rgplr[i]) & 0xf000) | 0x0) */
    ReadRt();
    goto L_0cc2;

L_0d34:
    i = 0x0;

L_0d39:
    if ((hdrCur.rt != rtShDef))
        goto L_0ea0;
    else
        goto L_0d4c;

L_0d4c:
    if (((uint16_t)(rgplr[i].cShDef) != 0x0))
        goto L_0d75;
    else
        goto L_0d63;

L_0d63:
    if ((i >= game.cPlayer))
        goto L_0d75;
    else
        goto L_0d6e;

L_0d6e:
    i = (i + 0x1);
    goto L_0d4c;

L_0d75:
    if ((i == game.cPlayer))
        goto L_0ea0;
    else
        goto L_0d7d;

L_0d7d:

L_0d83:
    if ((LOWORD(rglpshdef[i]) != 0x0))
        goto L_0e38;
    else
        goto L_0d94;

L_0d94:
    if ((HIWORD(rglpshdef[i]) != 0x0))
        goto L_0e38;
    else
        goto L_0d9e;

L_0d9e:
    rglpshdef[i] = LpAlloc(0x930, htShips);
    j = 0x0;
    goto L_0e2f;

L_0dc5:
    /* untranslated: part[123:2](rglpshdef[i][j]) = ((part[123:2](rglpshdef[i][j]) & 0xfdff) | 0x200) */
    rglpshdef[i][j].grbitPlr = 0x0;
    j = (j + 0x1);

L_0e2f:
    if ((j < 0x10))
        goto L_0dc5;
    else
        goto L_0e38;

L_0e38:
    iplrSav = idPlayer;
    if ((idPlayer != 0xffff))
        goto L_0e51;
    else
        goto L_0e48;

L_0e48:
    idPlayer = i;
    goto L_0e57;

L_0e51:
    idPlayer = 0xffff;

L_0e57:
    if ((FReadShDef(rgbCur, rglpshdef[i], iplrSav) == 0x0))
        goto CorruptHist;
    else
        goto L_0e7d;

L_0e7d:

L_0e83:
    idPlayer = iplrSav;
    rgplr[i].cShDef = (rgplr[i].cShDef - 0x1);
    ReadRt();
    goto L_0d39;

L_0ea0:
    i = 0x0;

L_0ea5:
    if ((hdrCur.rt != rtShDef))
        goto L_1042;
    else
        goto L_0eb8;

L_0eb8:
    /* untranslated: branch ((part[4:2](rgplr[i]) >> 0xc) & 0xf) != 0x0 ? L_0ee8 : L_0ed6 */

L_0ed6:
    if ((i >= game.cPlayer))
        goto L_0ee8;
    else
        goto L_0ee1;

L_0ee1:
    i = (i + 0x1);
    goto L_0eb8;

L_0ee8:
    if ((i == game.cPlayer))
        goto L_1042;
    else
        goto L_0ef0;

L_0ef0:

L_0ef6:
    if ((LOWORD(rglpshdefSB[i]) != 0x0))
        goto L_0fab;
    else
        goto L_0f07;

L_0f07:
    if ((HIWORD(rglpshdefSB[i]) != 0x0))
        goto L_0fab;
    else
        goto L_0f11;

L_0f11:
    rglpshdefSB[i] = LpAlloc(0x5be, htShips);
    j = 0x0;
    goto L_0fa2;

L_0f38:
    /* untranslated: part[123:2](rglpshdefSB[i][j]) = ((part[123:2](rglpshdefSB[i][j]) & 0xfdff) | 0x200) */
    rglpshdefSB[i][j].grbitPlr = 0x0;
    j = (j + 0x1);

L_0fa2:
    if ((j < 0xa))
        goto L_0f38;
    else
        goto L_0fab;

L_0fab:
    iplrSav = idPlayer;
    if ((idPlayer != 0xffff))
        goto L_0fc4;
    else
        goto L_0fbb;

L_0fbb:
    idPlayer = i;
    goto L_0fca;

L_0fc4:
    idPlayer = 0xffff;

L_0fca:
    if ((FReadShDef(rgbCur, rglpshdefSB[i], iplrSav) == 0x0))
        goto CorruptHist;
    else
        goto L_0ff0;

L_0ff0:

L_0ff6:
    idPlayer = iplrSav;
    /* untranslated: ss:[bp-0x48] = ((part[4:2](rgplr[i]) + 0xf000) & 0xf000) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) & 0xfff) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) | ss:[bp-0x48]) */
    ReadRt();
    goto L_0ea5;

L_1042:
    if ((hdrCur.rt != rtScore))
        goto L_12ff;
    else
        goto L_1055;

L_1055:
    iplr = (LOWORD(rgbCur) & 0x1f);
    /* untranslated: sx = part[0:24](rgbCur) */
    if ((LOWORD(rgsxPlr[iplr]) != 0x0))
        goto L_10b3;
    else
        goto L_107f;

L_107f:
    if ((HIWORD(rgsxPlr[iplr]) != 0x0))
        goto L_10b3;
    else
        goto L_1089;

L_1089:
    rgsxPlr[iplr] = LpAlloc(0x978, htMisc);
    rgcsxPlr[iplr] = 0x0;

L_10b3:
    if ((LOWORD(rgsxPlr[iplr]) != 0x0))
        goto L_10ce;
    else
        goto L_10c4;

L_10c4:
    if ((HIWORD(rgsxPlr[iplr]) == 0x0))
        goto L_12f7;
    else
        goto L_10ce;

L_10ce:
    if ((sx.fHistory == 0x0))
        goto L_10ea;
    else
        goto L_10e1;

L_10e1:
    turnCur = HIWORD(sx);
    goto L_10f0;

L_10ea:
    turnCur = game.turn;

L_10f0:
    isx = 0x0;
    goto L_10fc;

L_10f8:
    isx = (isx + 0x1);

L_10fc:
    if ((isx >= rgcsxPlr[iplr]))
        goto L_1138;
    else
        goto L_110d;

L_110d:
    if ((turnCur <= HIWORD(rgsxPlr[iplr][isx])))
        goto L_1138;
    else
        goto L_112f;

L_112f:

L_1138:
    if ((isx >= rgcsxPlr[iplr]))
        goto L_116e;
    else
        goto L_1149;

L_1149:
    if ((turnCur != HIWORD(rgsxPlr[iplr][isx])))
        goto L_1177;
    else
        goto L_116e;

L_116e:
    if ((isx < 0x65))
        goto L_1236;
    else
        goto L_1177;

L_1177:
    if ((rgcsxPlr[iplr] < 0x65))
        goto L_11d8;
    else
        goto L_1186;

L_1186:
    if ((isx <= 0x0))
        goto L_1251;
    else
        goto L_118f;

L_118f:
    if ((isx <= 0x1))
        goto L_11d1;
    else
        goto L_1198;

L_1198:
    fmemmove(rgsxPlr[iplr], &(rgsxPlr[iplr][0x1]), LOWORD(((isx + 0xffff) * 0x18)));

L_11d1:
    isx = (isx - 0x1);

L_11d8:
    fmemmove(&(rgsxPlr[iplr][(isx + 0x1)]), &(rgsxPlr[iplr][isx]), LOWORD(((rgcsxPlr[iplr] - isx) * 0x18)));
    rgcsxPlr[iplr] = (rgcsxPlr[iplr] + 0x1);

L_1236:
    if ((isx != rgcsxPlr[iplr]))
        goto L_1251;
    else
        goto L_1247;

L_1247:
    rgcsxPlr[iplr] = (rgcsxPlr[iplr] + 0x1);

L_1251:
    rgsxPlr[iplr][isx] = sx;
    /* untranslated: ss:[bp-0x66] = loword((0x18 * isx)) */
    HIWORD(rgsxPlr[iplr][isx]) = turnCur;
    LOWORD(rgsxPlr[iplr][isx]) = ((LOWORD(rgsxPlr[iplr][isx]) & 0x7fff) | 0x8000);

L_12f7:
    ReadRt();
    goto L_1042;

L_12ff:
    if ((hdrCur.rt != rtAiData))
        goto L_13de;
    else
        goto L_1312;

L_1312:
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0x9) & 0x1) == 0x0 ? L_13c3 : L_1331 */

L_1331:
    if ((LOWORD(vlpbAiData) != 0x0))
        goto L_1373;
    else
        goto L_133b;

L_133b:
    if ((HIWORD(vlpbAiData) != 0x0))
        goto L_1373;
    else
        goto L_1345;

L_1345:
    vlpbAiData = LpAlloc(0x1fa0, htMisc);
    if ((LOWORD(vlpbAiData) != 0x0))
        goto L_1373;
    else
        goto L_1366;

L_1366:
    if ((HIWORD(vlpbAiData) == 0x0))
        goto CorruptHist;
    else
        goto L_136d;

L_136d:

L_1373:
    lpb = vlpbAiData;

L_1380:
    if ((hdrCur.rt != rtAiData))
        goto L_13de;
    else
        goto L_1393;

L_1393:
    fmemmove(&(lpb), rgbCur, hdrCur.cb);
    lpb = ((uint8_t *)(lpb) + hdrCur.cb);
    ReadRt();
    goto L_1380;

L_13c3:
    if ((hdrCur.rt != rtAiData))
        goto L_13de;
    else
        goto L_13d6;

L_13d6:
    ReadRt();
    goto L_13c3;

L_13de:
    if ((hdrCur.rt != rtThing))
        goto L_14d3;
    else
        goto L_13f1;

L_13f1:
    cThing = LOWORD(rgbCur);
    cThingAlloc = (cThing + 0xa);
    if ((cThingAlloc <= 0xfd2))
        goto L_1411;
    else
        goto L_140b;

L_140b:
    cThingAlloc = 0xfd2;

L_1411:
    lpThings = LpAlloc(LOWORD((cThingAlloc * 0x12)), htThings);
    if ((LOWORD(lpThings) != 0x0))
        goto L_1444;
    else
        goto L_1437;

L_1437:
    if ((HIWORD(lpThings) == 0x0))
        goto CorruptHist;
    else
        goto L_143e;

L_143e:

L_1444:
    fmemset(&(lpThings), 0x0, LOWORD((cThingAlloc * 0x12)));
    ReadRt();
    i = 0x0;
    lpth = lpThings;
    goto L_148e;

L_147b:
    i = (i + 0x1);
    lpth = (lpth + 0x1);

L_148e:
    if ((i >= cThing))
        goto L_14d3;
    else
        goto L_1499;

L_1499:
    if ((hdrCur.rt != rtThing))
        goto CorruptHist;
    else
        goto L_14a9;

L_14a9:

L_14af:
    fmemcpy(&(lpth), rgbCur, hdrCur.cb);
    ReadRt();
    goto L_147b;

L_14d3:
    StreamClose();
    goto L_1526;

LNoHistFile:
    cPlanetHist = 0x0;
    FreeLp(&(lpPlanets), htPlanets);
    lpPlanets = 0x0;
    cThing = 0x0;
    FreeLp(&(lpThings), htThings);
    lpThings = 0x0;

L_1526:
    fFileErrSilent = fSilentSav;
    GetFileStatus(dt, iPlayer);
    if ((FOpenFile((dt | grf), iPlayer, 0x20) == 0x0))
        goto LError;
    else
        goto L_1555;

L_1555:

L_155b:
    if ((iPlayer != 0xffff))
        goto LNextTurn;
    else
        goto L_1564;

L_1564:
    /* untranslated: HIWORD(gd) = ((HIWORD(gd) & 0xfffe) | (((part[14:2](rgbCur) >> 0xb) & 0x1) & 0x1)) */

LNextTurn:
    cturn = (cturn + 0x1);
    cPlanet = 0x0;
    cFleet = 0x0;
    ReadRt();

L_159c:
    if ((hdrCur.rt == rtBtlData))
        goto L_15c2;
    else
        goto L_15af;

L_15af:
    if ((hdrCur.rt != rtContinue))
        goto L_168e;
    else
        goto L_15c2;

L_15c2:
    if ((hdrCur.rt == rtContinue))
        goto L_165e;
    else
        goto L_15d5;

L_15d5:
    if ((LOWORD(lpbBattleLog) != 0x0))
        goto L_160e;
    else
        goto L_15df;

L_15df:
    if ((HIWORD(lpbBattleLog) != 0x0))
        goto L_160e;
    else
        goto L_15e9;

L_15e9:
    lpbBattleLog = LpAlloc(0xffc8, htBattle);
    lpbBattleCur = lpbBattleLog;

L_160e:
    if (((0x0 - (HIWORD(lpbBattleCur) & 0x0)) > 0x0))
        goto L_165e;
    else
        goto L_1632;

L_1632:
    if (((0x0 - (HIWORD(lpbBattleCur) & 0x0)) < 0x0))
        goto L_163e;
    else
        goto L_1637;

L_1637:
    /* untranslated: branch (0xffc8 - (LOWORD(lpbBattleCur) & 0xffff)) >= part[6:2](rgbCur) ? L_165e : L_163e */

L_163e:
    LOWORD(lpbBattleCur) = 0xffff;
    lpbBattleCur = LpAlloc(0xffc8, htBattle);

L_165e:
    fmemmove(&(lpbBattleCur), rgbCur, hdrCur.cb);
    lpbBattleCur = ((uint8_t *)(lpbBattleCur) + hdrCur.cb);
    ReadRt();
    goto L_159c;

L_168e:
    if ((LOWORD(lpbBattleCur) != 0x0))
        goto L_16a2;
    else
        goto L_1698;

L_1698:
    if ((HIWORD(lpbBattleCur) == 0x0))
        goto L_16c8;
    else
        goto L_16a2;

L_16a2:
    LOWORD(lpbBattleCur) = 0xffff;
    if ((((wVersFile >> 0x5) & 0x7f) >= 0x50))
        goto L_16c8;
    else
        goto L_16c3;

L_16c3:
    UpdateBattleRecords();

L_16c8:
    if ((hdrCur.rt != rtPlr))
        goto L_1761;
    else
        goto L_16db;

L_16db:
    i = (uint16_t)(LOWORD(rgbCur));
    ReadRtPlr(rgplr[i], 0x4b98);
    cPlanet = (cPlanet + rgplr[i].cPlanet);
    rgplr[i].cPlanet = 0x0;
    /* untranslated: cFleet = (cFleet + (part[4:2](rgplr[i]) & 0xfff)) */
    /* untranslated: part[4:2](rgplr[i]) = ((part[4:2](rgplr[i]) & 0xf000) | 0x0) */
    ReadRt();
    goto L_16c8;

L_1761:
    if ((dt == 0x2))
        goto L_1785;
    else
        goto L_176a;

L_176a:
    /* untranslated: LOWORD(lSaltCur) = part[12:2](rgplr[iPlayer]) */
    /* untranslated: HIWORD(lSaltCur) = part[14:2](rgplr[iPlayer]) */
    goto L_17ba;

L_1785:
    if ((hdrCur.rt != rtChgPassword))
        goto L_17ae;
    else
        goto L_1798;

L_1798:
    LOWORD(lSaltCur) = LOWORD(rgbCur);
    HIWORD(lSaltCur) = HIWORD(rgbCur);
    ReadRt();
    goto L_17ba;

L_17ae:
    lSaltCur = 0x0;

L_17ba:
    if ((FCheckPassword() != 0x0))
        goto L_1811;
    else
        goto L_17c7;

L_17c7:
    if ((ini.fValidate != 0x0))
        goto L_17ed;
    else
        goto L_17da;

L_17da:
    if ((ini.fLogging == 0x0))
        goto LError;
    else
        goto L_17ed;

L_17ed:
    AlertSz(PszFormatIds(idsPasswordHaveEnteredIncorrectPleaseTry, 0x0), MB_ICONHAND);

L_1811:
    ReadPlayerMessages();
    ResetHb(htFleets);
    ResetHb(htOrd);
    FreeLp(&(rglpfl), htMisc);
    rglpfl = 0x0;
    if ((LOWORD(lpPlanets) != 0x0))
        goto L_1896;
    else
        goto L_1858;

L_1858:
    if ((HIWORD(lpPlanets) != 0x0))
        goto L_1896;
    else
        goto L_1862;

L_1862:
    if ((0x1 <= cPlanet))
        goto L_1874;
    else
        goto L_186e;

L_186e:
    t_merge_1877_0001 = 0x1;
    goto L_1877;

L_1874:
    t_merge_1877_0001 = cPlanet;

L_1877:
    cPlanetAlloc = t_merge_1877_0001;
    lpPlanets = LpAlloc(LOWORD((cPlanetAlloc * 0x38)), htPlanets);

L_1896:
    lppl = lpPlanets;
    j = 0x0;
    i = 0x0;
    goto L_1b03;

L_18b0:
    fHaveHistoryData = 0x0;
    if ((cPlanetHist == 0x0))
        goto LFoundPlanet;
    else
        goto L_18be;

L_18be:
    if ((j >= cPlanetHist))
        goto L_18f6;
    else
        goto L_18c9;

L_18c9:
    if ((((LOWORD(rgbCur) << 0x5) >> 0x5) <= lppl->id))
        goto L_18f6;
    else
        goto L_18eb;

L_18eb:
    j = (j + 0x1);
    lppl = (lppl + 0x1);
    goto L_18be;

L_18f6:
    if ((j >= cPlanetHist))
        goto L_192b;
    else
        goto L_1901;

L_1901:
    if ((((LOWORD(rgbCur) << 0x5) >> 0x5) != lppl->id))
        goto L_192b;
    else
        goto L_1923;

L_1923:
    fHaveHistoryData = 0x1;
    goto LFoundPlanet;

L_192b:
    if ((cPlanetAlloc != cPlanetHist))
        goto L_1974;
    else
        goto L_1936;

L_1936:
    cPlanetAlloc = (cPlanetAlloc + 0x8);
    lpPlanets = LpReAlloc(&(lpPlanets), LOWORD((cPlanetAlloc * 0x38)), htPlanets);
    lppl = &(lpPlanets[j]);

L_1974:
    if ((j >= cPlanetHist))
        goto L_19a6;
    else
        goto L_197f;

L_197f:
    fmemmove(&(lppl[0x1]), &(lppl), LOWORD(((cPlanetHist - j) * 0x38)));

L_19a6:
    cPlanetHist = (cPlanetHist + 0x1);

LFoundPlanet:
    if ((FReadPlanet(iPlayer, lppl, 0x0, fHaveHistoryData) == 0x0))
        goto Corrupt;
    else
        goto L_19c7;

L_19c7:

L_19cd:
    if ((lppl->iPlayer == 0xffff))
        goto L_19ed;
    else
        goto L_19da;

L_19da:
    rgplr[lppl->iPlayer].cPlanet = (rgplr[lppl->iPlayer].cPlanet + 0x1);

L_19ed:
    ReadRt();
    if ((hdrCur.rt != rtProdQ))
        goto L_1af2;
    else
        goto L_1a05;

L_1a05:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_1a1c;
    else
        goto L_1a12;

L_1a12:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_1a6a;
    else
        goto L_1a1c;

L_1a1c:
    /* untranslated: branch lppl->lpplprod->iprodMax > (words(hdrCur.cb, 0x0) / 0x4) ? L_1a6a : L_1a48 */

L_1a48:
    FreePl(lppl->lpplprod);
    lppl->lpplprod = 0x0;

L_1a6a:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_1aad;
    else
        goto L_1a77;

L_1a77:
    if ((HIWORD(lppl->lpplprod) != 0x0))
        goto L_1aad;
    else
        goto L_1a81;

L_1a81:
    /* untranslated: lppl->lpplprod = LpplAlloc(0x4, ((words(hdrCur.cb, 0x0) / 0x4) + 0x2), htOrd) */

L_1aad:
    fmemmove(&(lppl->lpplprod->rgprod), rgbCur, hdrCur.cb);
    /* untranslated: lppl->lpplprod->iprodMac = lobyte((words(hdrCur.cb, 0x0) / 0x4)) */
    ReadRt();

L_1af2:
    if ((cPlanetHist != 0x0))
        goto L_1aff;
    else
        goto L_1afb;

L_1afb:
    lppl = (lppl + 0x1);

L_1aff:
    i = (i + 0x1);

L_1b03:
    if ((i < cPlanet))
        goto L_18b0;
    else
        goto L_1b0e;

L_1b0e:
    if ((cPlanetHist == 0x0))
        goto L_1b1d;
    else
        goto L_1b17;

L_1b17:
    cPlanet = cPlanetHist;

L_1b1d:
    i = 0x0;
    goto L_1b29;

L_1b25:
    i = (i + 0x1);

L_1b29:
    if ((i >= game.cPlayer))
        goto L_1cdb;
    else
        goto L_1b34;

L_1b34:
    if ((i != iPlayer))
        goto L_1b59;
    else
        goto L_1b3f;

L_1b3f:
    rglpshdef[i] = &(rgshdef);
    goto FreeShdef;

L_1b59:
    /* untranslated: branch ((part[6:2](rgplr[i]) >> 0x8) & 0x1) == 0x0 ? L_1b25 : L_1b77 */

L_1b77:
    if ((LOWORD(rglpshdef[i]) != 0x0))
        goto L_1c32;
    else
        goto L_1b88;

L_1b88:
    if ((HIWORD(rglpshdef[i]) != 0x0))
        goto L_1c32;
    else
        goto L_1b92;

L_1b92:
    rglpshdef[i] = LpAlloc(0x930, htShips);

FreeShdef:
    j = 0x0;
    goto L_1c23;

L_1bb9:
    /* untranslated: part[123:2](rglpshdef[i][j]) = ((part[123:2](rglpshdef[i][j]) & 0xfdff) | 0x200) */
    rglpshdef[i][j].grbitPlr = 0x0;
    j = (j + 0x1);

L_1c23:
    if ((j >= 0x10))
        goto L_1c32;
    else
        goto L_1c29;

L_1c29:

L_1c32:
    iplrSav = idPlayer;
    if ((idPlayer != 0xffff))
        goto L_1c4b;
    else
        goto L_1c42;

L_1c42:
    idPlayer = i;
    goto L_1c5c;

L_1c4b:
    if ((i == idPlayer))
        goto L_1c5c;
    else
        goto L_1c56;

L_1c56:
    idPlayer = 0xffff;

L_1c5c:
    j = 0x0;
    goto L_1c68;

L_1c64:
    j = (j + 0x1);

L_1c68:
    if ((j >= (uint16_t)(rgplr[i].cShDef)))
        goto L_1cd2;
    else
        goto L_1c7f;

L_1c7f:
    if ((hdrCur.rt != rtShDef))
        goto L_1cc1;
    else
        goto L_1c92;

L_1c92:
    if ((FReadShDef(rgbCur, rglpshdef[i], iplrSav) == 0x0))
        goto Corrupt;
    else
        goto L_1cb8;

L_1cb8:

L_1cc1:
    idPlayer = iplrSav;
    goto Corrupt;

L_1cca:
    ReadRt();
    goto L_1c64;

L_1cd2:
    idPlayer = iplrSav;
    goto L_1b25;

L_1cdb:
    i = 0x0;
    goto L_1ce7;

L_1ce3:
    i = (i + 0x1);

L_1ce7:
    if ((i >= game.cPlayer))
        goto L_1ded;
    else
        goto L_1cf2;

L_1cf2:
    rgplr[i].cShDef = 0x0;
    if ((LOWORD(rglpshdef[i]) != 0x0))
        goto L_1d1c;
    else
        goto L_1d12;

L_1d12:
    if ((HIWORD(rglpshdef[i]) == 0x0))
        goto L_1ce3;
    else
        goto L_1d1c;

L_1d1c:
    j = 0x0;
    goto L_1de1;

L_1d24:
    /* untranslated: branch ((part[123:2](rglpshdef[i][j]) >> 0x9) & 0x1) != 0x0 ? L_1ddd : L_1d51 */

L_1d51:
    if ((i == idPlayer))
        goto L_1dce;
    else
        goto L_1d5c;

L_1d5c:
    if ((gd.fGeneratingTurn != 0x0))
        goto L_1dce;
    else
        goto L_1d6c;

L_1d6c:
    /* untranslated: branch (part[84:2](rgplr[i]) & 0x1) == 0x0 ? L_1dce : L_1d85 */

L_1d85:
    /* untranslated: part[123:2](rglpshdef[i][j]) = ((part[123:2](rglpshdef[i][j]) & 0xfdff) | 0x200) */
    goto L_1ddd;

L_1dce:
    rgplr[i].cShDef = (rgplr[i].cShDef + 0x1);

L_1ddd:
    j = (j + 0x1);

L_1de1:
    if ((j >= 0x10))
        goto L_1ce3;
    else
        goto L_1de7;

L_1de7:

L_1ded:
    if ((0x1 <= cFleet))
        goto L_1e03;
    else
        goto L_1dfd;

L_1dfd:
    t_merge_1e06_0001 = 0x1;
    goto L_1e06;

L_1e03:
    t_merge_1e06_0001 = cFleet;

L_1e06:
    rglpfl = LpAlloc((t_merge_1e06_0001 * 0x4), htMisc);
    i = 0x0;
    goto L_1ebb;

L_1e22:
    LpAlloc(0x7c, htFleets);
    /* untranslated: LOWORD(rglpfl[i]) = faroff(callresult(void *)) */
    /* untranslated: *(rglpfl[i]+0x2) = farseg(callresult(void *)) */
    /* untranslated: lpfl = callresult(void *) */
    if ((FReadFleet(lpfl) == 0x0))
        goto LError;
    else
        goto L_1e67;

L_1e67:

L_1e6d:
    /* untranslated: ss:[bp-0x48] = ((part[4:2](rgplr[lpfl->iPlayer]) + 0x1) & 0xfff) */
    /* untranslated: part[4:2](rgplr[lpfl->iPlayer]) = (part[4:2](rgplr[lpfl->iPlayer]) & 0xf000) */
    /* untranslated: part[4:2](rgplr[lpfl->iPlayer]) = (part[4:2](rgplr[lpfl->iPlayer]) | ss:[bp-0x48]) */
    i = (i + 0x1);

L_1ebb:
    if ((i < cFleet))
        goto L_1e22;
    else
        goto L_1ec6;

L_1ec6:
    i = 0x0;
    goto L_1ed2;

L_1ece:
    i = (i + 0x1);

L_1ed2:
    if ((i >= game.cPlayer))
        goto L_2066;
    else
        goto L_1edd;

L_1edd:
    /* untranslated: branch ((part[6:2](rgplr[i]) >> 0x8) & 0x1) == 0x0 ? L_1ece : L_1efb */

L_1efb:
    if ((LOWORD(rglpshdefSB[i]) != 0x0))
        goto L_1fb6;
    else
        goto L_1f0c;

L_1f0c:
    if ((HIWORD(rglpshdefSB[i]) != 0x0))
        goto L_1fb6;
    else
        goto L_1f16;

L_1f16:
    rglpshdefSB[i] = LpAlloc(0x5be, htShips);
    j = 0x0;
    goto L_1fa7;

L_1f3d:
    /* untranslated: part[123:2](rglpshdefSB[i][j]) = ((part[123:2](rglpshdefSB[i][j]) & 0xfdff) | 0x200) */
    rglpshdefSB[i][j].grbitPlr = 0x0;
    j = (j + 0x1);

L_1fa7:
    if ((j >= 0xa))
        goto L_1fb6;
    else
        goto L_1fad;

L_1fad:

L_1fb6:
    iplrSav = idPlayer;
    if ((idPlayer != 0xffff))
        goto L_1fcf;
    else
        goto L_1fc6;

L_1fc6:
    idPlayer = i;
    goto L_1fe0;

L_1fcf:
    if ((i == idPlayer))
        goto L_1fe0;
    else
        goto L_1fda;

L_1fda:
    idPlayer = 0xffff;

L_1fe0:
    j = 0x0;
    goto L_1fec;

L_1fe8:
    j = (j + 0x1);

L_1fec:
    /* untranslated: branch j >= ((part[4:2](rgplr[i]) >> 0xc) & 0xf) ? L_205d : L_200a */

L_200a:
    if ((hdrCur.rt != rtShDef))
        goto L_204c;
    else
        goto L_201d;

L_201d:
    if ((FReadShDef(rgbCur, rglpshdefSB[i], iplrSav) == 0x0))
        goto Corrupt;
    else
        goto L_2043;

L_2043:

L_204c:
    idPlayer = iplrSav;
    goto Corrupt;

L_2055:
    ReadRt();
    goto L_1fe8;

L_205d:
    idPlayer = iplrSav;
    goto L_1ece;

L_2066:
    i = 0x0;
    goto L_2072;

L_206e:
    i = (i + 0x1);

L_2072:
    if ((i >= game.cPlayer))
        goto L_21c0;
    else
        goto L_207d;

L_207d:
    /* untranslated: part[4:2](rgplr[i]) = ((part[4:2](rgplr[i]) & 0xfff) | 0x0) */
    if ((LOWORD(rglpshdefSB[i]) != 0x0))
        goto L_20c0;
    else
        goto L_20b6;

L_20b6:
    if ((HIWORD(rglpshdefSB[i]) == 0x0))
        goto L_206e;
    else
        goto L_20c0;

L_20c0:
    j = 0x0;
    goto L_21b4;

L_20c8:
    /* untranslated: branch ((part[123:2](rglpshdefSB[i][j]) >> 0x9) & 0x1) != 0x0 ? L_21b0 : L_20f5 */

L_20f5:
    if ((i == idPlayer))
        goto L_2172;
    else
        goto L_2100;

L_2100:
    if ((gd.fGeneratingTurn != 0x0))
        goto L_2172;
    else
        goto L_2110;

L_2110:
    /* untranslated: branch (part[84:2](rgplr[i]) & 0x1) == 0x0 ? L_2172 : L_2129 */

L_2129:
    /* untranslated: part[123:2](rglpshdefSB[i][j]) = ((part[123:2](rglpshdefSB[i][j]) & 0xfdff) | 0x200) */
    goto L_21b0;

L_2172:
    /* untranslated: ss:[bp-0x48] = ((part[4:2](rgplr[i]) + 0x1000) & 0xf000) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) & 0xfff) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) | ss:[bp-0x48]) */

L_21b0:
    j = (j + 0x1);

L_21b4:
    if ((j >= 0xa))
        goto L_206e;
    else
        goto L_21ba;

L_21ba:

L_21c0:
    if ((LOWORD(vlprgScoreX) != 0x0))
        goto L_220d;
    else
        goto L_21ca;

L_21ca:
    if ((HIWORD(vlprgScoreX) != 0x0))
        goto L_220d;
    else
        goto L_21d4;

L_21d4:
    vlprgScoreX = LpAlloc(LOWORD((game.cPlayer * 0x18)), htMisc);
    fmemset(&(vlprgScoreX), 0x0, LOWORD((game.cPlayer * 0x18)));

L_220d:
    if ((hdrCur.rt != rtScore))
        goto L_2520;
    else
        goto L_2220;

L_2220:
    iplr = (LOWORD(rgbCur) & 0x1f);
    /* untranslated: vlprgScoreX[iplr] = part[0:24](rgbCur) */
    if ((LOWORD(rgsxPlr[iplr]) != 0x0))
        goto L_229c;
    else
        goto L_2268;

L_2268:
    if ((HIWORD(rgsxPlr[iplr]) != 0x0))
        goto L_229c;
    else
        goto L_2272;

L_2272:
    rgsxPlr[iplr] = LpAlloc(0x978, htMisc);
    rgcsxPlr[iplr] = 0x0;

L_229c:
    if ((LOWORD(rgsxPlr[iplr]) != 0x0))
        goto L_22b7;
    else
        goto L_22ad;

L_22ad:
    if ((HIWORD(rgsxPlr[iplr]) == 0x0))
        goto L_2518;
    else
        goto L_22b7;

L_22b7:
    if ((((LOWORD(vlprgScoreX[iplr]) >> 0xf) & 0x1) == 0x0))
        goto L_22f8;
    else
        goto L_22dc;

L_22dc:
    turnCur = HIWORD(vlprgScoreX[iplr]);
    goto L_22fe;

L_22f8:
    turnCur = game.turn;

L_22fe:
    isx = 0x0;
    goto L_230a;

L_2306:
    isx = (isx + 0x1);

L_230a:
    if ((isx >= rgcsxPlr[iplr]))
        goto L_2346;
    else
        goto L_231b;

L_231b:
    if ((turnCur <= HIWORD(rgsxPlr[iplr][isx])))
        goto L_2346;
    else
        goto L_233d;

L_233d:

L_2346:
    if ((isx >= rgcsxPlr[iplr]))
        goto L_237c;
    else
        goto L_2357;

L_2357:
    if ((turnCur != HIWORD(rgsxPlr[iplr][isx])))
        goto L_2385;
    else
        goto L_237c;

L_237c:
    if ((isx < 0x65))
        goto L_2444;
    else
        goto L_2385;

L_2385:
    if ((rgcsxPlr[iplr] < 0x65))
        goto L_23e6;
    else
        goto L_2394;

L_2394:
    if ((isx <= 0x0))
        goto L_245f;
    else
        goto L_239d;

L_239d:
    if ((isx <= 0x1))
        goto L_23df;
    else
        goto L_23a6;

L_23a6:
    fmemmove(rgsxPlr[iplr], &(rgsxPlr[iplr][0x1]), LOWORD(((isx + 0xffff) * 0x18)));

L_23df:
    isx = (isx - 0x1);

L_23e6:
    fmemmove(&(rgsxPlr[iplr][(isx + 0x1)]), &(rgsxPlr[iplr][isx]), LOWORD(((rgcsxPlr[iplr] - isx) * 0x18)));
    rgcsxPlr[iplr] = (rgcsxPlr[iplr] + 0x1);

L_2444:
    if ((isx != rgcsxPlr[iplr]))
        goto L_245f;
    else
        goto L_2455;

L_2455:
    rgcsxPlr[iplr] = (rgcsxPlr[iplr] + 0x1);

L_245f:
    rgsxPlr[iplr][isx] = vlprgScoreX[iplr];
    /* untranslated: ss:[bp-0x4e] = loword((0x18 * isx)) */
    HIWORD(rgsxPlr[iplr][isx]) = turnCur;
    LOWORD(rgsxPlr[iplr][isx]) = ((LOWORD(rgsxPlr[iplr][isx]) & 0x7fff) | 0x8000);

L_2518:
    ReadRt();
    goto L_220d;

L_2520:
    if ((LOWORD(lpThings) != 0x0))
        goto L_2534;
    else
        goto L_252a;

L_252a:
    if ((HIWORD(lpThings) == 0x0))
        goto L_255a;
    else
        goto L_2534;

L_2534:
    FreeLp(&(lpThings), htThings);
    lpThings = 0x0;
    cThing = 0x0;

L_255a:
    if ((hdrCur.rt != rtThing))
        goto L_2755;
    else
        goto L_256d;

L_256d:
    if ((cThing <= 0x0))
        goto L_257d;
    else
        goto L_2577;

L_2577:
    t_merge_2580_0001 = 0x1;
    goto L_2580;

L_257d:
    t_merge_2580_0001 = 0x0;

L_2580:
    fHist = t_merge_2580_0001;
    cThingFile = LOWORD(rgbCur);
    if ((0xa <= cThingFile))
        goto L_259a;
    else
        goto L_2594;

L_2594:
    t_merge_259d_0001 = 0xa;
    goto L_259d;

L_259a:
    t_merge_259d_0001 = cThingFile;

L_259d:
    cThingAlloc = t_merge_259d_0001;
    if ((LOWORD(lpThings) != 0x0))
        goto L_2604;
    else
        goto L_25aa;

L_25aa:
    if ((HIWORD(lpThings) != 0x0))
        goto L_2604;
    else
        goto L_25b4;

L_25b4:
    lpThings = LpAlloc(LOWORD((cThingAlloc * 0x12)), htThings);
    if ((LOWORD(lpThings) != 0x0))
        goto L_25e7;
    else
        goto L_25da;

L_25da:
    if ((HIWORD(lpThings) == 0x0))
        goto LError;
    else
        goto L_25e1;

L_25e1:

L_25e7:
    fmemset(&(lpThings), 0x0, LOWORD((cThingAlloc * 0x12)));

L_2604:
    ReadRt();
    lpth = lpThings;
    j = 0x0;
    i = 0x0;
    goto L_2747;

L_2623:
    fHaveHistoryData = 0x0;
    if ((fHist == 0x0))
        goto L_270b;
    else
        goto L_2631;

L_2631:
    if ((j >= cThing))
        goto L_2656;
    else
        goto L_263c;

L_263c:
    if ((LOWORD(rgbCur) <= LOWORD(lpth)))
        goto L_2656;
    else
        goto L_264b;

L_264b:
    j = (j + 0x1);
    lpth = (lpth + 0x1);
    goto L_2631;

L_2656:
    if ((j >= cThing))
        goto L_2678;
    else
        goto L_2661;

L_2661:
    if ((LOWORD(rgbCur) != LOWORD(lpth)))
        goto L_2678;
    else
        goto L_2670;

L_2670:
    fHaveHistoryData = 0x1;
    goto LFoundThing;

L_2678:
    if ((cThingAlloc != cThing))
        goto L_26c3;
    else
        goto L_2684;

L_2684:
    cThingAlloc = (cThingAlloc + 0x8);
    lpThings = LpReAlloc(&(lpThings), LOWORD((cThingAlloc * 0x12)), htThings);
    lpth = &(lpThings[j]);

L_26c3:
    if ((j >= cThing))
        goto L_270b;
    else
        goto L_26ce;

L_26ce:
    fmemmove(&(lpth[0x1]), &(lpth), LOWORD(((cThing - j) * 0x12)));
    fmemset(&(lpth), 0x0, 0x12);

L_270b:
    cThing = (cThing + 0x1);

LFoundThing:
    fmemcpy(&(lpth), rgbCur, hdrCur.cb);
    lpth->turn = game.turn;
    lpth = (lpth + 0x1);
    j = (j + 0x1);
    ReadRt();
    i = (i + 0x1);

L_2747:
    if ((i >= cThingFile))
        goto L_277d;
    else
        goto L_274f;

L_274f:

L_2755:
    cThing = 0x0;
    cThingAlloc = 0xa;
    lpThings = LpAlloc(LOWORD((cThingAlloc * 0x12)), htThings);

L_277d:
    if ((hdrCur.rt != rtSel))
        goto L_2795;
    else
        goto L_2790;

L_2790:
    ReadRt();

L_2795:
    iplrSav = idPlayer;

L_279b:
    if ((hdrCur.rt != rtBtlPlan))
        goto L_2843;
    else
        goto L_27ae;

L_27ae:
    iP = (LOWORD(rgbCur) & 0xf);
    idPlayer = iP;
    if ((LOWORD(rglpbtlplan[iP]) != 0x0))
        goto L_27f7;
    else
        goto L_27ce;

L_27ce:
    if ((HIWORD(rglpbtlplan[iP]) != 0x0))
        goto L_27f7;
    else
        goto L_27d8;

L_27d8:
    rglpbtlplan[iP] = LpAlloc(0x240, htShips);

L_27f7:
    /* untranslated: call UnpackBattlePlan(rgbCur, &rglpbtlplan[iP][part[18480:1](iP)], part[18480:1](iP)) -> callresult(void) */
    /* untranslated: part[18480:1](iP) = (part[18480:1](iP) + 0x1) */
    ReadRt();
    goto L_279b;

L_2843:
    idPlayer = iplrSav;
    if ((hdrCur.rt == rtEOF))
        goto L_2880;
    else
        goto Corrupt;

Corrupt:
    AlertSz(PszFormatIds(idsGameFileAppearsCorruptUnableLoadFile, 0x0), MB_ICONHAND);
    goto LError;

L_2880:
    filelength(hf);
    /* untranslated: ss:[bp-0x4a] = loword(callresult(int32_t)) */
    /* untranslated: ss:[bp-0x48] = hiword(callresult(int32_t)) */
    /* untranslated: branch ss:[bp-0x4a] != loword(tell(hf)) ? L_28b2 : L_28ab */

L_28ab:
    /* untranslated: branch ss:[bp-0x48] == hiword(callresult(int32_t)) ? L_2a56 : L_28b2 */

L_28b2:
    ReadRt();
    if ((hdrCur.rt != rtBOF))
        goto L_2a35;
    else
        goto L_28ca;

L_28ca:
    /* untranslated: game.turn = part[10:2](rgbCur) */
    /* untranslated: game.wCrap = ((game.wCrap & 0xf1ff) | ((((part[14:2](rgbCur) >> 0xd) & 0x7) & 0x7) << 0x9)) */
    i = 0x0;
    goto L_2905;

L_2901:
    i = (i + 0x1);

L_2905:
    if ((i >= game.cPlayer))
        goto L_298a;
    else
        goto L_2910;

L_2910:
    rgplr[i].cShDef = 0x0;
    /* untranslated: part[4:2](rgplr[i]) = ((part[4:2](rgplr[i]) & 0xf000) | 0x0) */
    rgplr[i].cPlanet = 0x0;
    /* untranslated: part[4:2](rgplr[i]) = ((part[4:2](rgplr[i]) & 0xfff) | 0x0) */
    /* untranslated: part[18480:1](i) = 0x0 */
    goto L_2901;

L_298a:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_2a1e;

L_29b5:
    if ((lppl->iPlayer != iPlayer))
        goto L_2a1a;
    else
        goto L_29c4;

L_29c4:
    lppl->iPlayer = 0xffff;
    lppl->det = 0x3;
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_29f8;
    else
        goto L_29ee;

L_29ee:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_2a1a;
    else
        goto L_29f8;

L_29f8:
    FreePl(lppl->lpplprod);
    lppl->lpplprod = 0x0;

L_2a1a:
    lppl = (lppl + 0x1);

L_2a1e:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_29b5;
    else
        goto L_2a2c;

L_2a2c:
    cPlanetHist = cPlanet;
    goto LNextTurn;

L_2a35:
    AlertSz(PszFormatIds(idsWarningIgnoringUnexpectedDataAfterEof, 0x0), MB_ICONHAND);

L_2a56:
    StreamClose();
    if ((cturn <= 0x1))
        goto L_2aed;
    else
        goto L_2a64;

L_2a64:
    /* untranslated: branch ((part[6:2](rgplr[iPlayer]) >> 0x9) & 0x1) != 0x0 ? L_2aed : L_2a82 */

L_2a82:
    if ((ini.fDumpPlanets != 0x0))
        goto L_2aed;
    else
        goto L_2a95;

L_2a95:
    if ((ini.fDumpFleets != 0x0))
        goto L_2aed;
    else
        goto L_2aa8;

L_2aa8:
    if ((ini.fDumpMap != 0x0))
        goto L_2aed;
    else
        goto L_2abb;

L_2abb:
    _wsprintf(szWork, PszGetCompressedString(idsNoteDYearsDataRead), cturn);
    AlertSz(0x57a4, MB_ICONASTERISK);

L_2aed:
    if ((strnicmp(pszExt, 0x759, 0x3) == 0x0))
        goto DoneNow;
    else
        goto L_2b05;

L_2b05:

L_2b0b:
    /* untranslated: branch ((part[6:2](rgplr[iPlayer]) >> 0x9) & 0x1) != 0x0 ? L_2e33 : L_2b29 */

L_2b29:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_2c17;

L_2b54:
    if ((lpth->ith != ithMineralPacket))
        goto L_2c13;
    else
        goto L_2b6a;

L_2b6a:
    if ((((LOWORD(lpth->thp) >> 0xa) & 0xf) == 0x0))
        goto L_2c13;
    else
        goto L_2b81;

L_2b81:
    lppl = LpplFromId((LOWORD(lpth->thp) & 0x3ff));
    if ((LOWORD(lppl) != 0x0))
        goto L_2bac;
    else
        goto L_2ba3;

L_2ba3:
    if ((HIWORD(lppl) == 0x0))
        goto L_2c13;
    else
        goto L_2bac;

L_2bac:
    if ((lppl->iPlayer != iPlayer))
        goto L_2c13;
    else
        goto L_2bb8;

L_2bb8:

L_2bbe:
    iWarp = IWarpMAFromLppl(lppl, &(fTwo));
    if (((iWarp + fTwo) >= (((LOWORD(lpth->thp) >> 0xa) & 0xf) + 0x4)))
        goto L_2c13;
    else
        goto L_2bf2;

L_2bf2:
    FSendPlrMsg2XGen(0x0, 0x151, 0xfffa, LOWORD(lpth), lppl->id);

L_2c13:
    lpth = (lpth + 0x1);

L_2c17:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_2b54;
    else
        goto L_2c25;

L_2c25:
    if ((game.fTutorial != 0x0))
        goto L_2e33;
    else
        goto L_2c39;

L_2c39:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_2de7;

L_2c64:
    if ((lppl->iPlayer != iPlayer))
        goto L_2de3;
    else
        goto L_2c73;

L_2c73:
    if ((lppl->fStarbase == 0x0))
        goto L_2de3;
    else
        goto L_2c8a;

L_2c8a:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_2ca4;
    else
        goto L_2c97;

L_2c97:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_2de3;
    else
        goto L_2c9e;

L_2c9e:

L_2ca4:
    if ((rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef == ihuldefOrbitalFort))
        goto L_2de3;
    else
        goto L_2cd0;

L_2cd0:

L_2cd6:
    fWorking = 0x0;
    iprod = 0x0;
    lpprod = &(lppl->lpplprod->rgprod);
    goto L_2d0c;

L_2cf9:
    iprod = (iprod + 0x1);
    lpprod = (lpprod + 0x1);

L_2d0c:
    if ((iprod >= lppl->lpplprod->iprodMac))
        goto L_2dba;
    else
        goto L_2d22;

L_2d22:
    EstimateItemProdSched(lppl, 0x0, iprod, &(iFirst), &(iLast));
    if ((iLast <= 0x1))
        goto L_2d54;
    else
        goto L_2d4c;

L_2d4c:
    fWorking = 0x0;
    goto L_2dba;

L_2d54:
    if ((iLast != 0x1))
        goto L_2cf9;
    else
        goto L_2d5d;

L_2d5d:
    if ((lpprod->grobj != 0x1))
        goto L_2db2;
    else
        goto L_2d7d;

L_2d7d:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_2db2 : L_2d85 */

L_2d85:
    if (((HIWORD((uint32_t)((lpprod >> 0xa))) & 0x0) < 0x0))
        goto L_2cf9;
    else
        goto L_2da5;

L_2da5:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) > 0x0 ? L_2db2 : L_2daa */

L_2daa:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) < 0x7 ? L_2cf9 : L_2db2 */

L_2db2:
    fWorking = 0x1;

L_2dba:
    if ((fWorking == 0x0))
        goto L_2de3;
    else
        goto L_2dc3;

L_2dc3:
    FSendPlrMsg2XGen(0x0, 0x152, lppl->id, lppl->id, 0x0);

L_2de3:
    lppl = (lppl + 0x1);

L_2de7:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_2c64;
    else
        goto L_2df5;

L_2df5:
    i = CBattles();
    if ((i <= 0x0))
        goto L_2e33;
    else
        goto L_2e06;

L_2e06:
    if ((i <= 0x1))
        goto L_2e20;
    else
        goto L_2e1a;

L_2e1a:
    t_merge_2e23_0001 = 0x1;
    goto L_2e23;

L_2e20:
    t_merge_2e23_0001 = 0x0;

L_2e23:
    FSendPlrMsg2XGen(0x1, (t_merge_2e23_0001 + 0x153), 0xfff9, i, 0x0);

L_2e33:
    if ((gd.fDontDoLogFiles != 0x0))
        goto L_2eb1;
    else
        goto L_2e43;

L_2e43:
    _wsprintf(szWork, "%s.x%s", pszFileName, 0x25, (pszExt + 0x1), 0x25);
    if ((FLoadLogFile(0x57a4) == 0x0))
        goto L_2e8d;
    else
        goto L_2e80;

L_2e80:
    if ((FRunLogFile() != 0x0))
        goto L_2eb1;
    else
        goto L_2e8d;

L_2e8d:
    AlertSz(PszFormatIds(idsPlayerLogFileAppearsCorruptUnableLoad, 0x0), MB_ICONHAND);
    goto LError;

L_2eb1:
    i = 0x0;
    goto L_2ebd;

L_2eb9:
    i = (i + 0x1);

L_2ebd:
    if ((i >= game.cPlayer))
        goto L_2f03;
    else
        goto L_2ec8;

L_2ec8:
    /* untranslated: part[4:2](rgplr[i]) = ((part[4:2](rgplr[i]) & 0xf000) | 0x0) */
    rgplr[i].cPlanet = 0x0;
    goto L_2eb9;

L_2f03:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_2f69;

L_2f2e:
    if ((lppl->iPlayer == 0xffff))
        goto L_2f51;
    else
        goto L_2f3b;

L_2f3b:
    rgplr[lppl->iPlayer].cPlanet = (rgplr[lppl->iPlayer].cPlanet + 0x1);
    goto L_2f65;

L_2f51:
    lppl->fStarbase = 0x0;

L_2f65:
    lppl = (lppl + 0x1);

L_2f69:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_2f2e;
    else
        goto L_2f77;

L_2f77:
    j = 0x0;
    i = 0x0;
    goto L_2f88;

L_2f84:
    i = (i + 0x1);

L_2f88:
    if ((i >= cFleet))
        goto DoneNow;
    else
        goto L_2f93;

L_2f93:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_2fc3;
    else
        goto L_2fbb;

L_2fbb:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto DoneNow;
    else
        goto L_2fc3;

L_2fc3:
    j = lpfl->iPlayer;
    /* untranslated: ss:[bp-0x48] = ((part[4:2](rgplr[j]) + 0x1) & 0xfff) */
    /* untranslated: part[4:2](rgplr[j]) = (part[4:2](rgplr[j]) & 0xf000) */
    /* untranslated: part[4:2](rgplr[j]) = (part[4:2](rgplr[j]) | ss:[bp-0x48]) */
    goto L_2f84;

DoneNow:
    idPlayer = iPlayer;
    if ((idPlayer == 0xffff))
        goto L_31fa;
    else
        goto L_301e;

L_301e:
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0x9) & 0x1) != 0x0 ? L_31fa : L_303d */

L_303d:
    if ((LOWORD(vrgszMRU) != 0x0))
        goto L_3051;
    else
        goto L_3047;

L_3047:
    if ((HIWORD(vrgszMRU) == 0x0))
        goto L_31fa;
    else
        goto L_3051;

L_3051:
    strcpy(szT, pszFileName);
    strcat(szT, 0x764);
    strcat(szT, pszExt);
    if ((fstricmp(&(szT), vrgszMRU) == 0x0))
        goto L_31fa;
    else
        goto L_30a2;

L_30a2:
    i = 0x1;
    goto L_30dd;

L_30aa:
    if ((fstricmp(&(szT), ((uint8_t *)(vrgszMRU) + (i * 0x100))) == 0x0))
        goto L_3120;
    else
        goto L_30d3;

L_30d3:

L_30d9:
    i = (i + 0x1);

L_30dd:
    if ((i >= 0x8))
        goto L_3120;
    else
        goto L_30e3;

L_30e3:

L_30e9:
    fstrcpy(((uint8_t *)(vrgszMRU) + (i * 0x100)), ((uint8_t *)(vrgszMRU) + ((i + 0xffff) * 0x100)));
    i = (i - 0x1);

L_3120:
    if ((i >= 0x1))
        goto L_30e9;
    else
        goto L_3129;

L_3129:
    fstrcpy(vrgszMRU, &(szT));
    CchGetString(idsStarsIni, szIniFile);
    CchGetString(idsFiles, szSection);
    CchGetString(idsFile1, szEntry);
    psz = (szEntry[strlen(szEntry)] + 0xffff);
    i = 0x0;
    goto L_31f1;

L_3198:
    *(psz) = LOBYTE((i + 0x31));
    fstrcpy(&(szT), ((uint8_t *)(vrgszMRU) + (i * 0x100)));
    WritePrivateProfileString(&(szSection), &(szEntry), &(szT), &(szIniFile));
    i = (i + 0x1);

L_31f1:
    if ((i < 0x9))
        goto L_3198;
    else
        goto L_31fa;

L_31fa:
    return 0x1;
}

int16_t FReadPlanet(int16_t iPlayer, PLANET *lppl, int16_t fHistory, int16_t fPreInited) {
    int16_t   fFirstYear;
    int16_t   fRouting;
    uint8_t   bMask;
    int16_t   i;
    uint8_t  *pb;
    MessageId idm;
    int16_t   pctOpt;
    int16_t   pct;

L_3206:
    fFirstYear = 0x0;
    if ((fPreInited != 0x0))
        goto L_3233;
    else
        goto L_321d;

L_321d:
    fmemset(&(lppl), 0x0, 0x38);

L_3233:
    if ((fHistory != 0x0))
        goto L_32af;
    else
        goto L_323c;

L_323c:
    if ((iPlayer == 0xffff))
        goto L_32af;
    else
        goto L_3245;

L_3245:
    if ((fPreInited != 0x0))
        goto L_326a;
    else
        goto L_324e;

L_324e:
    fFirstYear = 0x1;
    lppl->fFirstYear = 0x1;
    goto L_32de;

L_326a:
    if ((lppl->fFirstYear == 0x0))
        goto L_32de;
    else
        goto L_3281;

L_3281:
    if ((lppl->turn == game.turn))
        goto L_32a7;
    else
        goto L_3290;

L_3290:
    lppl->fFirstYear = 0x0;
    goto L_32de;

L_32a7:
    fFirstYear = 0x1;

L_32af:
    *(lppl + 0x4) = ((*(lppl + 0x4) & 0xf7ff) | ((((HIWORD(rgbCur) >> 0xf) & 0x1) & 0x1) << 0xb));

L_32de:
    lppl->id = ((LOWORD(rgbCur) << 0x5) >> 0x5);
    lppl->iPlayer = (LOWORD(rgbCur) >> 0xb);
    if ((lppl->det >= (HIWORD(rgbCur) & 0x7f)))
        goto L_3347;
    else
        goto L_3322;

L_3322:
    *(lppl + 0x4) = ((*(lppl + 0x4) & 0xff00) | ((HIWORD(rgbCur) & 0x7f) & 0xff));

L_3347:
    *(lppl + 0x4) = ((*(lppl + 0x4) & 0xfeff) | ((((HIWORD(rgbCur) >> 0x8) & 0x1) & 0x1) << 0x8));
    *(lppl + 0x4) = ((*(lppl + 0x4) & 0xfdff) | ((((HIWORD(rgbCur) >> 0x9) & 0x1) & 0x1) << 0x9));
    *(lppl + 0x4) = ((*(lppl + 0x4) & 0xfbff) | ((((HIWORD(rgbCur) >> 0x7) & 0x1) & 0x1) << 0xa));
    fRouting = ((HIWORD(rgbCur) >> 0xe) & 0x1);
    if ((lppl->fStarbase == 0x0))
        goto L_341a;
    else
        goto L_33f9;

L_33f9:
    if ((lppl->iPlayer != 0xffff))
        goto L_341a;
    else
        goto L_3406;

L_3406:
    lppl->fStarbase = 0x0;

L_341a:
    if ((fHistory != 0x0))
        goto L_342d;
    else
        goto L_3423;

L_3423:
    lppl->turn = game.turn;

L_342d:
    pb = 0x4b9c;
    if (((HIWORD(rgbCur) & 0x7f) < 0x3))
        goto LFinishBRecord;
    else
        goto L_343d;

L_343d:

L_3443:
    bMask = *(pb);
    pb = (pb + 0x1);
    i = 0x0;
    goto L_346b;

L_3457:
    i = (i + 0x1);
    bMask = LOBYTE((bMask >> 0x2));

L_346b:
    if ((i >= 0x3))
        goto L_34d9;
    else
        goto L_3474;

L_3474:
    goto L_34c3;

L_3480:
    lppl->rgpctMinLevel[i] = 0x0;
    goto L_3457;

L_3499:
    pb = (pb + 0x1);
    lppl->rgpctMinLevel[i] = *(pb);
    goto L_3457;

L_34ba:
    return 0x0;

L_34c3:
    if (((bMask & 0x3) == 0x0))
        goto L_3480;
    else
        goto L_34cb;

L_34cb:
    if (((bMask & 0x3) != 0x1))
        goto L_34ba;
    else
        goto L_34d0;

L_34d0:

L_34d9:
    i = 0x0;
    goto L_34ef;

L_34e1:
    i = (i + 0x1);
    pb = (pb + 0x1);

L_34ef:
    if ((i >= 0x3))
        goto L_3515;
    else
        goto L_34f8;

L_34f8:
    lppl->rgMinConc[i] = *(pb);
    goto L_34e1;

L_3515:
    i = 0x0;
    goto L_356a;

L_351d:
    if ((*(pb) <= 0x64))
        goto L_3533;
    else
        goto L_352d;

L_352d:
    return 0x0;

L_3533:
    pb = (pb + 0x1);
    lppl->rgEnvVarOrig[i] = *(pb);
    lppl->rgEnvVar[i] = *(pb);
    i = (i + 0x1);

L_356a:
    if ((i < 0x3))
        goto L_351d;
    else
        goto L_3573;

L_3573:
    if ((((HIWORD(rgbCur) >> 0xa) & 0x1) == 0x0))
        goto L_35cf;
    else
        goto L_3586;

L_3586:
    i = 0x0;
    goto L_35c6;

L_358e:
    if ((*(pb) <= 0x64))
        goto L_35a4;
    else
        goto L_359e;

L_359e:
    return 0x0;

L_35a4:
    pb = (pb + 0x1);
    lppl->rgEnvVarOrig[i] = *(pb);
    i = (i + 0x1);

L_35c6:
    if ((i < 0x3))
        goto L_358e;
    else
        goto L_35cf;

L_35cf:
    if (((LOWORD(rgbCur) >> 0xb) == 0xffff))
        goto L_35ef;
    else
        goto L_35df;

L_35df:
    lppl->uGuesses = pb;
    pb = (pb + 0x2);

L_35ef:
    if ((lppl->det <= 0x3))
        goto LFinishBRecord;
    else
        goto L_35fe;

L_35fe:

L_3604:
    if ((((HIWORD(rgbCur) >> 0xd) & 0x1) == 0x0))
        goto L_3728;
    else
        goto L_3617;

L_3617:
    bMask = *(pb);
    pb = (pb + 0x1);
    i = 0x0;
    goto L_363f;

L_362b:
    i = (i + 0x1);
    bMask = LOBYTE((bMask >> 0x2));

L_363f:
    if ((i >= 0x4))
        goto L_3728;
    else
        goto L_3648;

L_3648:
    goto L_3702;

L_3654:
    lppl->rgwtMin[i] = 0x0;
    goto L_362b;

L_3678:
    pb = (pb + 0x1);
    LOWORD(lppl->rgwtMin[i]) = *(pb);
    HIWORD(lppl->rgwtMin[i]) = 0x0;
    goto L_362b;

L_36a7:
    LOWORD(lppl->rgwtMin[i]) = pb;
    HIWORD(lppl->rgwtMin[i]) = 0x0;
    pb = (pb + 0x2);
    goto L_362b;

L_36d3:
    LOWORD(lppl->rgwtMin[i]) = pb;
    HIWORD(lppl->rgwtMin[i]) = pb[0x2];
    pb = (pb + 0x4);
    goto L_362b;

L_3702:
    if (((bMask & 0x3) == 0x0))
        goto L_3654;
    else
        goto L_370a;

L_370a:
    if (((bMask & 0x3) == 0x1))
        goto L_3678;
    else
        goto L_3712;

L_3712:
    if (((bMask & 0x3) == 0x2))
        goto L_36a7;
    else
        goto L_371a;

L_371a:
    if (((bMask & 0x3) != 0x3))
        goto L_362b;
    else
        goto L_371f;

L_371f:

L_3728:
    if ((hdrCur.rt != rtPlanetB))
        goto L_390a;
    else
        goto LFinishBRecord;

LFinishBRecord:
    if ((lppl->fStarbase == 0x0))
        goto L_377c;
    else
        goto L_3752;

L_3752:
    *(lppl + 0x2c) = ((*(lppl + 0x2c) & 0xfff0) | (*(pb) & 0xf));
    pb = (pb + 0x1);

L_377c:
    if ((fHistory == 0x0))
        goto L_3798;
    else
        goto L_3785;

L_3785:
    lppl->turn = pb;
    pb = (pb + 0x2);
    goto L_3904;

L_3798:
    if ((fFirstYear == 0x0))
        goto L_3904;
    else
        goto L_37a1;

L_37a1:
    if ((lppl->iPlayer == 0xffff))
        goto L_37d8;
    else
        goto L_37ae;

L_37ae:
    FSendPlrMsg2XGen(0x0, 0xaa, lppl->id, lppl->id, (lppl->iPlayer | 0x30));
    goto L_3904;

L_37d8:
    if ((lppl->det > 0x1))
        goto L_380d;
    else
        goto L_37ea;

L_37ea:
    FSendPlrMsg2XGen(0x0, 0xad, lppl->id, lppl->id, 0x0);
    goto L_3904;

L_380d:
    if ((GetRaceStat(rgplr[iPlayer], rsMajorAdv) != raTerra))
        goto L_3863;
    else
        goto L_382d;

L_382d:
    pctOpt = PctPlanetOptValue(lppl, iPlayer);
    FSendPlrMsg2XGen(0x0, 0x15d, lppl->id, lppl->id, pctOpt);
    goto L_3904;

L_3863:
    pct = PctPlanetDesirability(lppl, iPlayer);
    if ((pct <= 0x0))
        goto L_3899;
    else
        goto L_3880;

L_3880:
    pct = LOWORD((PctTrueMaxGrowth(iPlayer) * pct));
    idm = idmHaveFoundNewHabitablePlanetColonistsWill;
    goto L_38dd;

L_3899:
    pctOpt = PctPlanetOptValue(lppl, iPlayer);
    if ((pctOpt <= 0x0))
        goto L_38cf;
    else
        goto L_38b6;

L_38b6:
    pct = LOWORD((PctTrueMaxGrowth(iPlayer) * pctOpt));
    idm = idmHaveFoundNewPlanetWhichHaveAbility;
    goto L_38dd;

L_38cf:
    pct = LOWORD((0xa * pct));
    idm = idmHaveFoundNewPlanetWhichUnfortunatelyHabitable;

L_38dd:
    FSendPlrMsg2XGen(0x0, idm, lppl->id, abs(pct), lppl->id);

L_3904:
    return 0x1;

L_390a:
    if ((((HIWORD(rgbCur) >> 0xb) & 0x1) == 0x0))
        goto L_3944;
    else
        goto L_391d;

L_391d:
    fmemmove(&(*(lppl + 0x14)), &(pb), 0x8);
    pb = (pb + 0x8);
    goto L_39dc;

L_3944:
    /* untranslated: ss:[bp-0x12] = ((*(lppl+0x1a) & 0xffbf) | hiword((int32_t)(words((((HIWORD(rgbCur) >> 0xc) & 0x1) & 0x1), (0x0 & 0x0)) << 0x16))) */
    /* untranslated: *(lppl+0x18) = ((*(lppl+0x18) & 0xffff) | loword((int32_t)(words((((HIWORD(rgbCur) >> 0xc) & 0x1) & 0x1), (0x0 & 0x0)) << 0x16))) */
    /* untranslated: *(lppl+0x1a) = ss:[bp-0x12] */
    lppl->iScanner = 0x1f;
    lppl->cDefenses = 0x0;
    *(lppl + 0x1a) = ((*(lppl + 0x1a) & 0xffff) | 0x0);

L_39dc:
    if ((lppl->iPlayer == 0xffff))
        goto L_3a40;
    else
        goto L_39e9;

L_39e9:
    if ((lppl->fStarbase == 0x0))
        goto L_3a2b;
    else
        goto L_3a00;

L_3a00:
    *(lppl + 0x2c) = pb;
    *(lppl + 0x2e) = pb[0x2];
    lppl->fNoHeal = 0x0;
    pb = (pb + 0x4);

L_3a2b:
    if ((fRouting == 0x0))
        goto L_3a40;
    else
        goto L_3a34;

L_3a34:
    lppl->wRouting = pb;

L_3a40:
    return 0x1;
}

int16_t FReadFleet(FLEET *lpfl) {
    uint16_t  us;
    int16_t   cord;
    int16_t   fByte;
    ORDER    *lpord;
    int16_t   i;
    int16_t   cish;
    uint8_t  *pb;
    int16_t   cch;
    uint16_t *pus;
    char      szT[33];
    int16_t   cOut;

L_3a4c:
    cish = 0x0;
    fmemset(&(lpfl), 0x0, 0x7c);
    fmemmove(&(lpfl), rgbCur, 0xc);
    fByte = lpfl->fDone;
    /* untranslated: us = part[12:2](rgbCur) */
    pb = 0x4ba6;
    if ((fByte == 0x0))
        goto L_3b2f;
    else
        goto L_3ab6;

L_3ab6:
    i = 0x0;
    goto L_3ace;

L_3abe:
    i = (i + 0x1);
    us = (us >> 0x1);

L_3ace:
    if ((us == 0x0))
        goto L_3bae;
    else
        goto L_3ad7;

L_3ad7:
    if (((us & 0x1) == 0x0))
        goto L_3abe;
    else
        goto L_3ae5;

L_3ae5:
    pb = (pb + 0x1);
    lpfl->rgcsh[i] = *(pb);
    if ((lpfl->rgcsh[i] == 0x0))
        goto L_3abe;
    else
        goto L_3b25;

L_3b25:
    cish = (cish + 0x1);

L_3b2f:
    pus = pb;
    i = 0x0;
    goto L_3b4d;

L_3b3d:
    i = (i + 0x1);
    us = (us >> 0x1);

L_3b4d:
    if ((us == 0x0))
        goto L_3ba8;
    else
        goto L_3b56;

L_3b56:
    if (((us & 0x1) == 0x0))
        goto L_3b3d;
    else
        goto L_3b64;

L_3b64:
    pus = (pus + 0x2);
    lpfl->rgcsh[i] = *(pus);
    if ((lpfl->rgcsh[i] == 0x0))
        goto L_3b3d;
    else
        goto L_3ba1;

L_3ba1:
    cish = (cish + 0x1);

L_3ba8:
    pb = pus;

L_3bae:
    if ((cish != 0x0))
        goto L_3bcb;
    else
        goto L_3bb7;

L_3bb7:
    lpfl->fDead = 0x1;

L_3bcb:
    if ((lpfl->det < 0x4))
        goto L_3cc0;
    else
        goto L_3bdd;

L_3bdd:
    us = pb;
    pb = (pb + 0x2);
    i = 0x0;
    goto L_3c06;

L_3bf6:
    i = (i + 0x1);
    us = (us >> 0x2);

L_3c06:
    if ((i >= 0x5))
        goto L_3cc0;
    else
        goto L_3c0f;

L_3c0f:
    goto L_3ca2;

L_3c18:
    LOWORD(lpfl->rgwtMin[i]) = *(pb);
    HIWORD(lpfl->rgwtMin[i]) = 0x0;
    pb = (pb + 0x1);
    goto L_3bf6;

L_3c47:
    LOWORD(lpfl->rgwtMin[i]) = pb;
    HIWORD(lpfl->rgwtMin[i]) = 0x0;
    pb = (pb + 0x2);
    goto L_3bf6;

L_3c73:
    LOWORD(lpfl->rgwtMin[i]) = pb;
    HIWORD(lpfl->rgwtMin[i]) = pb[0x2];
    pb = (pb + 0x4);
    goto L_3bf6;

L_3ca2:
    if (((us & 0x3) == 0x1))
        goto L_3c18;
    else
        goto L_3caa;

L_3caa:
    if (((us & 0x3) == 0x2))
        goto L_3c47;
    else
        goto L_3cb2;

L_3cb2:
    if (((us & 0x3) != 0x3))
        goto L_3bf6;
    else
        goto L_3cb7;

L_3cb7:

L_3cc0:
    if ((lpfl->det >= 0x7))
        goto L_3d11;
    else
        goto L_3cd2;

L_3cd2:
    *(lpfl + 0x74) = pb;
    *(lpfl + 0x76) = pb[0x2];
    pb = (pb + 0x4);
    *(lpfl + 0x2c) = pb;
    *(lpfl + 0x2e) = pb[0x2];
    pb = (pb + 0x4);
    ReadRt();
    return 0x1;

L_3d11:
    if ((hdrCur.rt == rtFleetA))
        goto L_3d4b;
    else
        goto Corrupt;

Corrupt:
    AlertSz(PszFormatIds(idsGameFileAppearsCorruptUnableLoadFile, 0x0), MB_ICONHAND);
    return 0x0;

L_3d4b:
    us = pb;
    pb = (pb + 0x2);
    pus = pb;
    i = 0x0;
    goto L_3d7a;

L_3d6a:
    i = (i + 0x1);
    us = (us >> 0x1);

L_3d7a:
    if ((us == 0x0))
        goto L_3e0f;
    else
        goto L_3d83;

L_3d83:
    if (((us & 0x1) == 0x0))
        goto L_3d6a;
    else
        goto L_3d91;

L_3d91:
    pus = (pus + 0x2);
    /* untranslated: HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 0x2))] = *pus */
    /* untranslated: branch ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 0x2))] >> 0x7) & 0x1ff) < 0x1f4 ? L_3d6a : L_3dd8 */

L_3dd8:
    /* untranslated: HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 0x2))] = ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 0x2))] & 0x7f) | 0xf980) */

L_3e0f:
    pb = pus;
    pb = (pb + 0x1);
    lpfl->iplan = *(pb);
    pb = (pb + 0x1);
    lpfl->cord = *(pb);
    lpfl->lpplord = LpplAlloc(0x12, (lpfl->cord + 0x1), htOrd);
    fmemset(&(lpfl->lpplord->rgord), 0x0, LOWORD(((lpfl->cord + 0x1) * 0x12)));
    cord = lpfl->cord;
    lpord = &(lpfl->lpplord->rgord);
    goto L_3ec0;

L_3eaf:
    lpord = (lpord + 0x1);
    cord = (cord - 0x1);

L_3ec0:
    if ((cord == 0x0))
        goto L_3f33;
    else
        goto L_3ec9;

L_3ec9:
    memset(0x4b98, 0x0, 0x12);
    ReadRt();
    if ((hdrCur.rt == rtOrderA))
        goto L_3f0b;
    else
        goto L_3ef5;

L_3ef5:
    if ((hdrCur.rt != rtOrderB))
        goto Corrupt;
    else
        goto L_3f05;

L_3f05:

L_3f0b:
    /* untranslated: *lpord = part[0:18](rgbCur) */
    lpord->fNoAutoTrack = 0x0;
    goto L_3eaf;

L_3f33:
    lpfl->lpplord->iordMac = LOBYTE(lpfl->cord);
    if ((lpfl->idPlanet == 0xffff))
        goto L_3fdb;
    else
        goto L_3f52;

L_3f52:
    if ((lpfl->idPlanet <= game.cPlanMax))
        goto L_3f6a;
    else
        goto L_3f61;

L_3f61:
    lpfl->idPlanet = 0xffff;

L_3f6a:
    if ((lpfl->pt.x != rgptPlan[lpfl->idPlanet].x))
        goto L_3fa4;
    else
        goto L_3f85;

L_3f85:
    if ((lpfl->pt.y == rgptPlan[lpfl->idPlanet].y))
        goto L_3fdb;
    else
        goto L_3fa4;

L_3fa4:
    if ((i != 0x0))
        goto Corrupt;
    else
        goto L_3fad;

L_3fad:
    if ((game.turn != 0x0))
        goto Corrupt;
    else
        goto L_3fb7;

L_3fb7:
    lpfl->pt.x = rgptPlan[lpfl->idPlanet].x;
    lpfl->pt.y = rgptPlan[lpfl->idPlanet].y;
    goto L_3fdb;

L_3fdb:
    ReadRt();
    if ((hdrCur.rt != rtString))
        goto L_40b2;
    else
        goto L_3ff3;

L_3ff3:
    cch = (uint16_t)(LOWORD(rgbCur));
    if ((cch != 0x0))
        goto L_4047;
    else
        goto L_4003;

L_4003:
    lpfl->lpszName = LpAlloc((strlen(0x4b99) + 0x1), htString);
    fstrcpy(lpfl->lpszName, "");
    goto L_40aa;

L_4047:
    cOut = 0x20;
    FDecompressUserString("", cch, &(szT), &(cOut));
    lpfl->lpszName = LpAlloc((strlen(szT) + 0x1), htString);
    fstrcpy(lpfl->lpszName, &(szT));

L_40aa:
    ReadRt();
    goto L_40c1;

L_40b2:
    lpfl->lpszName = 0x0;

L_40c1:
    return 0x1;
}

void UnpackBattlePlan(uint8_t *lpb, BTLPLAN *lpbtlplan, int16_t iplan) {
    char    szTemp[33];
    char    szName[33];
    int16_t cch;
    int16_t cOut;

L_40ce:
    fmemmove(&(lpbtlplan), &(lpb), 0x4);
    lpb = (lpb + 0x4);
    cch = *(lpb);
    lpb = (lpb + 0x1);
    if ((cch != 0x0))
        goto L_412a;
    else
        goto L_410c;

L_410c:
    /* untranslated: call fstrcpy(&part[0:0](lpbtlplan->szName), &lpb) -> callresult(char *) */
    goto L_4185;

L_412a:
    cOut = 0x20;
    fmemmove(&(szTemp), &(lpb), cOut);
    FDecompressUserString(&(szTemp), cch, &(szName), &(cOut));
    /* untranslated: call fmemmove(&part[0:0](lpbtlplan->szName), &szName, cOut) -> callresult(void *) */

L_4185:
    LOWORD(lpbtlplan) = ((LOWORD(lpbtlplan) & 0xff0f) | ((iplan & 0xf) << 0x4));
    return;
}

void UpdateBattleRecords() {
    BTLDATA  *lpbd;
    BTLREC   *lpbr;
    int16_t   cKill;
    HB       *lphb;
    BTLREC26 *lpbr26;
    int16_t   itok;

L_41ac:
    lphb = rglphb[0xb];
    if ((LOWORD(lphb) != 0x0))
        goto L_41d7;
    else
        goto L_41cb;

L_41cb:
    if ((HIWORD(lphb) == 0x0))
        goto L_4323;
    else
        goto L_41d1;

L_41d1:

L_41d7:
    lpbd = &(*(lphb + 0x12));

L_41eb:
    if ((lpbd->id != 0xffff))
        goto L_4241;
    else
        goto L_41f7;

L_41f7:
    lphb = lphb->lphbNext;
    if ((LOWORD(lphb) != 0x0))
        goto L_421a;
    else
        goto L_4211;

L_4211:
    if ((HIWORD(lphb) == 0x0))
        goto L_4323;
    else
        goto L_421a;

L_421a:
    if ((lphb->ibTop <= 0x10))
        goto L_4323;
    else
        goto L_4224;

L_4224:

L_422a:
    lpbd = &(*(lphb + 0x12));
    goto L_41eb;

L_4241:
    if ((lpbd->cbData == 0x0))
        goto L_4323;
    else
        goto L_424b;

L_424b:

L_4251:
    lpbr = &(lpbd->rgtok[lpbd->ctok]);
    lpbr26 = lpbr;
    lpbd = ((uint8_t *)(lpbd) + lpbd->cbData);

L_42a0:
    if ((LOWORD(lpbr) >= LOWORD(lpbd)))
        goto L_41eb;
    else
        goto L_42ae;

L_42ae:
    cKill = lpbr26->ctok;
    itok = lpbr26->itokAttack;
    lpbr->ctok = cKill;
    *(lpbr + 0x4) = ((*(lpbr + 0x4) & 0xff) | ((itok & 0xff) << 0x8));
    lpbr = &(lpbr->rgkill[lpbr->ctok]);
    lpbr26 = lpbr;
    goto L_42a0;

L_4323:
    return;
}

int16_t AskSaveDialog(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    uint16_t t_merge_4383_0001;

L_432a:
    goto L_43c5;

L_4339:
    return 0x1;

L_433f:
    if ((wParam == 0x429))
        goto L_435d;
    else
        goto L_4349;

L_4349:
    if ((wParam == 0x42b))
        goto L_435d;
    else
        goto L_4353;

L_4353:
    if ((wParam != 0x42a))
        goto L_4392;
    else
        goto L_435d;

L_435d:
    if ((wParam != 0x42b))
        goto L_4370;
    else
        goto L_436a;

L_436a:
    t_merge_4383_0001 = 0x0;
    goto L_4383;

L_4370:
    if ((wParam != 0x42a))
        goto L_4380;
    else
        goto L_437a;

L_437a:
    t_merge_4383_0001 = 0xffff;
    goto L_4383;

L_4380:
    t_merge_4383_0001 = 0x1;

L_4383:
    EndDialog(hwnd, t_merge_4383_0001);
    return 0x1;

L_4392:
    if ((wParam != 0x76))
        goto L_43e0;
    else
        goto L_439b;

L_439b:
    WinHelp(hwnd, szHelpFile, 0x1, 0x442);
    return 0x1;

L_43c5:
    if ((message == WM_DESTROY))
        goto L_43e0;
    else
        goto L_43cd;

L_43cd:
    if ((message == WM_INITDIALOG))
        goto L_4339;
    else
        goto L_43d5;

L_43d5:
    if ((message == WM_COMMAND))
        goto L_433f;
    else
        goto L_43e0;

L_43e0:
    return 0x0;
}

void PromptSaveGame() {
    int16_t (**lpProc)();
    int16_t  fRet;
    uint32_t t_merge_4433_0001;
    uint16_t t_merge_446d_0001;

L_43ee:
    lpProc = MakeProcInstance(AskSaveDialog, hInst);
    if ((game.fSinglePlr == 0x0))
        goto L_442d;
    else
        goto L_4424;

L_4424:
    t_merge_4433_0001 = 0x7e9;
    goto L_4433;

L_442d:
    t_merge_4433_0001 = 0x42c;

L_4433:
    fRet = DialogBox(hInst, t_merge_4433_0001, hwndFrame, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((fRet == 0x0))
        goto L_44a9;
    else
        goto L_445b;

L_445b:
    if ((fRet != 0xffff))
        goto L_446a;
    else
        goto L_4464;

L_4464:
    t_merge_446d_0001 = 0x1;
    goto L_446d;

L_446a:
    t_merge_446d_0001 = 0x0;

L_446d:
    LOWORD(gd) = ((LOWORD(gd) & 0xffef) | ((t_merge_446d_0001 & 0x1) << 0x4));
    FWriteLogFile(0x56a2, idPlayer);
    FWriteHistFile(idPlayer);

L_44a9:
    return;
}

void DestroyCurGame() {
    int16_t i;

L_44b0:
    if ((gd.fSendMsgMode == 0x0))
        goto L_44d8;
    else
        goto L_44cc;

L_44cc:
    FFinishPlrMsgEntry(0x0);

L_44d8:
    if ((idPlayer == 0xffff))
        goto L_44f1;
    else
        goto L_44e2;

L_44e2:
    if ((game.fDirty == 0x0))
        goto L_44f1;
    else
        goto L_44ec;

L_44ec:
    PromptSaveGame();

L_44f1:
    ResetHb(htPlanets);
    lpPlanets = 0x0;
    cPlanet = 0x0;
    ResetHb(htFleets);
    rglpfl = 0x0;
    cFleet = 0x0;
    ResetHb(htThings);
    lpThings = 0x0;
    cThing = 0x0;
    cThingAlloc = 0x0;
    vlprgScoreX = 0x0;
    vrptFleet.fCached = 0x0;
    vrptPlanet.fCached = 0x0;
    vrptBattle.fCached = 0x0;
    vrptEFleet.fCached = 0x0;
    i = 0x0;
    goto L_4594;

L_457d:
    rgsxPlr[i] = 0x0;
    i = (i + 0x1);

L_4594:
    if ((i < 0x10))
        goto L_457d;
    else
        goto L_459d;

L_459d:
    lpbBattleT = 0x0;
    lpbBattleLog = 0x0;
    lpbBattleCur = 0x0;
    gd.fAisDone = 0x0;
    gd.fGotoVCR = 0x0;
    gd.fFleetLinkValid = 0x0;
    ResetHb(htBattle);
    /* untranslated: branch part[44:2](rglphb) != 0x0 ? L_4601 : L_45f7 */

L_45f7:
    /* untranslated: branch part[46:2](rglphb) == 0x0 ? L_460b : L_4601 */

L_4601:
    /* untranslated: part[18:2](part[44:4](rglphb)) = 0xffff */

L_460b:
    ResetHb(htMisc);
    ResetHb(htString);
    ResetHb(htShips);
    ResetHb(htOrd);
    ResetHb(htPlrMsg);
    i = 0x0;
    goto L_4694;

L_464f:
    rglpshdef[i] = 0x0;
    rglpshdefSB[i] = 0x0;
    rglpbtlplan[i] = 0x0;
    /* untranslated: part[18480:1](i) = 0x0 */
    i = (i + 0x1);

L_4694:
    if ((i < 0x10))
        goto L_464f;
    else
        goto L_469d;

L_469d:
    if ((sel.grobj == grobjNone))
        goto L_46e7;
    else
        goto L_46a7;

L_46a7:
    ini.wFlags = ((ini.wFlags & 0xfe1f) | ((sel.grobj & 0xf) << 0x5));
    ini.iObjSel = sel.id;
    ini.idPlayer = idPlayer;
    /* untranslated: part[18:2](ini) = LOWORD(game) */
    /* untranslated: part[20:2](ini) = HIWORD(game) */

L_46e7:
    idPlayer = 0xffff;
    imemLogCur = 0x0;
    imemLogPrev = 0xffff;
    iMsgCur = 0x0;
    vlpbAiData = 0x0;
    ResetMessages();
    lSaltCur = 0x0;
    ctickLast = 0x0;
    LOWORD(game) = 0x0;
    HIWORD(game) = 0x0;
    game.cPlayer = 0x0;
    game.cPlanMax = 0x0;
    game.fDirty = 0x0;
    game.turn = 0x0;
    /* untranslated: part[32:1](game) = 0x0 */
    gd.fGameOverMan = 0x0;
    gd.fSendMsgMode = 0x0;
    if ((hwndBrowser == 0x0))
        goto L_477c;
    else
        goto L_4773;

L_4773:
    DestroyWindow(hwndBrowser);

L_477c:
    if ((hwndReportDlg == 0x0))
        goto L_478f;
    else
        goto L_4786;

L_4786:
    DestroyWindow(hwndReportDlg);

L_478f:
    if ((hwndPopup == 0x0))
        goto L_47a8;
    else
        goto L_4799;

L_4799:
    DestroyWindow(hwndPopup);
    hwndPopup = 0x0;

L_47a8:
    hwndActive = 0x0;
    sel.scan.grobjFull = grobjNone;
    sel.scan.grobj = grobjNone;
    sel.scan.iwp = 0xffff;
    sel.scan.ifl = 0xffff;
    sel.scan.idpl = 0xffff;
    fOrdersVis = 0x0;
    sel.grobjFull = grobjNone;
    sel.grobj = grobjNone;
    sel.id = 0xffff;
    sel.pt.y = 0x0;
    sel.pt.x = 0x0;
    sel.pl.id = 0xffff;
    sel.fl.id = 0xffff;
    sel.fl.lpplord = 0x0;
    sel.pl.lpplprod = 0x0;
    dxPlanetProdLB = 0x0;
    dxOrderED = 0x0;
    dxFleetCompLB = 0x0;
    dxShipLB = 0x0;
    dxShipDD = 0x0;
    i = 0x0;
    goto L_483d;

L_482e:
    rgdxOrderDD[i] = 0x0;
    i = (i + 0x1);

L_483d:
    if ((i < 0x3))
        goto L_482e;
    else
        goto L_4846;

L_4846:
    return;
}

int16_t FBogusLong(uint32_t lSerial) {
    int16_t i;

L_484c:
    LOWORD(lSerial) = (LOWORD(lSerial) ^ 0xa5a5);
    HIWORD(lSerial) = (HIWORD(lSerial) ^ 0xa5a5);
    i = 0x0;
    goto L_486b;

L_4867:
    i = (i + 0x1);

L_486b:
    if ((HIWORD(lSerial) < HIWORD(bogi[i])))
        goto L_4892;
    else
        goto L_4882;

L_4882:
    if ((HIWORD(lSerial) > HIWORD(bogi[i])))
        goto L_4867;
    else
        goto L_4887;

L_4887:
    if ((LOWORD(lSerial) > LOWORD(bogi[i])))
        goto L_4867;
    else
        goto L_488c;

L_488c:

L_4892:
    if ((LOWORD(lSerial) != LOWORD(bogi[i])))
        goto L_48b7;
    else
        goto L_48a9;

L_48a9:
    if ((HIWORD(lSerial) != HIWORD(bogi[i])))
        goto L_48b7;
    else
        goto L_48b1;

L_48b1:
    return 0x1;

L_48b7:
    return 0x0;
}

int16_t FValidSerialLong(uint32_t lSerial) {
    uint32_t lNumber;
    int16_t  i;
    uint32_t lSeries;

L_48c4:
    if ((FBogusLong(lSerial) == 0x0))
        goto L_48e9;
    else
        goto L_48e3;

L_48e3:
    return 0x0;

L_48e9:
    lSeries = lSerial;
    i = 0x0;
    goto L_491a;

L_48fd:
    lSeries = (uint32_t)((lSeries / 0x24));
    i = (i + 0x1);

L_491a:
    if ((i < 0x4))
        goto L_48fd;
    else
        goto L_4923;

L_4923:
    lNumber = lSeries;
    i = 0x0;
    goto L_4954;

L_4937:
    lNumber = (uint32_t)((lNumber * 0x24));
    i = (i + 0x1);

L_4954:
    if ((i < 0x4))
        goto L_4937;
    else
        goto L_495d;

L_495d:
    lNumber = (lSerial - lNumber);
    if ((HIWORD(lNumber) < 0x0))
        goto L_499e;
    else
        goto L_4978;

L_4978:
    if ((HIWORD(lNumber) > 0x0))
        goto L_4986;
    else
        goto L_497d;

L_497d:
    if ((LOWORD(lNumber) < 0x64))
        goto L_499e;
    else
        goto L_4986;

L_4986:
    if ((HIWORD(lNumber) < 0x16))
        goto L_49a4;
    else
        goto L_498f;

L_498f:
    if ((HIWORD(lNumber) > 0x16))
        goto L_499e;
    else
        goto L_4994;

L_4994:
    if ((LOWORD(lNumber) <= 0xe360))
        goto L_49a4;
    else
        goto L_499e;

L_499e:
    return 0x0;

L_49a4:
    if ((LOWORD(lSeries) != 0x12))
        goto L_49b6;
    else
        goto L_49ad;

L_49ad:
    if ((HIWORD(lSeries) == 0x0))
        goto L_4a04;
    else
        goto L_49b6;

L_49b6:
    if ((LOWORD(lSeries) != 0x16))
        goto L_49c8;
    else
        goto L_49bf;

L_49bf:
    if ((HIWORD(lSeries) == 0x0))
        goto L_4a04;
    else
        goto L_49c8;

L_49c8:
    if ((LOWORD(lSeries) != 0x2))
        goto L_49da;
    else
        goto L_49d1;

L_49d1:
    if ((HIWORD(lSeries) == 0x0))
        goto L_4a04;
    else
        goto L_49da;

L_49da:
    if ((LOWORD(lSeries) != 0x4))
        goto L_49ec;
    else
        goto L_49e3;

L_49e3:
    if ((HIWORD(lSeries) == 0x0))
        goto L_4a04;
    else
        goto L_49ec;

L_49ec:
    if ((LOWORD(lSeries) != 0x6))
        goto L_49fe;
    else
        goto L_49f5;

L_49f5:
    if ((HIWORD(lSeries) == 0x0))
        goto L_4a04;
    else
        goto L_49fe;

L_49fe:
    return 0x0;

L_4a04:
    return 0x1;
}

void FileError(MessageId ids) {
L_4a10:
    idsFileError = ids;
    if ((fFileErrSilent != 0x0))
        goto L_4a59;
    else
        goto L_4a29;

L_4a29:
    if ((gd.fGeneratingTurn != 0x0))
        goto L_4a59;
    else
        goto L_4a39;

L_4a39:
    AlertSz(PszFormatIds(ids, 0x0), MB_ICONHAND);

L_4a59:
    return;
}

void GetFileStatus(int16_t dt, int16_t iPlayer) {
    uint16_t t_merge_4a99_0001;

L_4a60:
    SetSzWorkFromDt(dt, iPlayer);
    if ((access(0x57a4, 0x2) == 0x0))
        goto L_4a96;
    else
        goto L_4a90;

L_4a90:
    t_merge_4a99_0001 = 0x1;
    goto L_4a99;

L_4a96:
    t_merge_4a99_0001 = 0x0;

L_4a99:
    HIWORD(gd) = ((HIWORD(gd) & 0xffdf) | ((t_merge_4a99_0001 & 0x1) << 0x5));
    return;
}

int16_t FOpenFile(DtFileType dt, int16_t iPlayer, int16_t md) {
    RTBOF    rtbof;
    StringId ids;
    int16_t  fCheckMulti;
    int16_t  fRewind;
    int16_t  fSilentSav;
    jmp_buf *penvMemSav[9];
    jmp_buf  env[9];
    uint16_t t_merge_4c1e_0001;

L_4ac2:
    fSilentSav = fFileErrSilent;
    ids = idsCantOpenFile;
    gd.fPartialTurn = 0x0;
    fCheckMulti = (dt & 0x2000);
    fRewind = (dt & 0x1000);
    dt = (dt & 0xff);
    SetSzWorkFromDt(dt, iPlayer);
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_4b49;
    else
        goto L_4b27;

L_4b27:
    fFileErrSilent = fSilentSav;
    FileError(ids);
    StreamClose();
    penvMem = penvMemSav;
    return 0x0;

L_4b49:
    fFileErrSilent = 0x1;
    StreamOpen(0x57a4, md);
    fFileErrSilent = fSilentSav;
    ids = idsGameFileAppearsCorruptUnableLoadFile;
    ReadRt();
    if ((hdrCur.rt != rtBOF))
        goto L_4bc4;
    else
        goto L_4b81;

L_4b81:
    /* untranslated: branch ((part[8:2](rgbCur) >> 0xc) & 0xf) != 0x2 ? L_4bc4 : L_4b94 */

L_4b94:
    /* untranslated: branch ((part[8:2](rgbCur) >> 0x5) & 0x7f) < 0x31 ? L_4bc4 : L_4bac */

L_4bac:
    /* untranslated: branch ((part[8:2](rgbCur) >> 0x5) & 0x7f) < 0x54 ? L_4c47 : L_4bc4 */

L_4bc4:
    if ((hdrCur.rt != rtBOF))
        goto L_4c2a;
    else
        goto L_4bd7;

L_4bd7:
    /* untranslated: branch ((part[8:2](rgbCur) >> 0xc) & 0xf) > 0x2 ? L_4c15 : L_4bea */

L_4bea:
    /* untranslated: branch ((part[8:2](rgbCur) >> 0xc) & 0xf) != 0x2 ? L_4c1b : L_4bfd */

L_4bfd:
    /* untranslated: branch ((part[8:2](rgbCur) >> 0x5) & 0x7f) <= 0x54 ? L_4c1b : L_4c15 */

L_4c15:
    t_merge_4c1e_0001 = 0x2ca;
    goto L_4c1e;

L_4c1b:
    t_merge_4c1e_0001 = 0x4d3;

L_4c1e:
    FileError(t_merge_4c1e_0001);
    goto LBadFile;

L_4c2a:
    FileError(idmColonistsDroppedDestroyedSpiritedFighting);

LBadFile:
    StreamClose();
    penvMem = penvMemSav;
    return 0x0;

L_4c47:
    /* untranslated: rtbof = part[0:16](rgbCur) */
    /* untranslated: branch ((part[12:2](rtbof) << 0xb) >> 0xb) == iPlayer ? L_4c7b : L_4c6c */

L_4c6c:
    FileError(idmGroundTroopsValiantlyDestroyedAttackingBarbarian);
    goto LBadFile;

L_4c7b:
    if ((LOWORD(game) != 0x0))
        goto L_4c8f;
    else
        goto L_4c85;

L_4c85:
    if ((HIWORD(game) == 0x0))
        goto L_4ebd;
    else
        goto L_4c8f;

L_4c8f:
    /* untranslated: branch part[4:2](rtbof) != LOWORD(game) ? L_4ca6 : L_4c9e */

L_4c9e:
    /* untranslated: branch part[6:2](rtbof) == HIWORD(game) ? L_4cb8 : L_4ca6 */

L_4ca6:
    FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
    goto LBadFile;

L_4cb8:
    if ((dt == dtHist))
        goto L_4ea5;
    else
        goto L_4cc1;

L_4cc1:
    if ((fCheckMulti == 0x0))
        goto L_4d4d;
    else
        goto L_4cca;

L_4cca:
    if ((rtbof.fMulti == 0x0))
        goto L_4d4d;
    else
        goto L_4cdd;

L_4cdd:
    lseek(hf, 0xfffffffc, 0x2);
    ReadRt();
    if ((hdrCur.rt == rtEOF))
        goto L_4d1e;
    else
        goto L_4d0d;

L_4d0d:
    if ((hdrCur.cb != 0x2))
        goto LBadFile;
    else
        goto L_4d18;

L_4d18:

L_4d1e:
    rtbof.turn = LOWORD(rgbCur);
    game.wCrap = ((game.wCrap & 0xf1ff) | ((rtbof.wGen & 0x7) << 0x9));

L_4d4d:
    if ((game.turn != 0x0))
        goto L_4d95;
    else
        goto L_4d57;

L_4d57:
    if ((game.turn == rtbof.turn))
        goto L_4d95;
    else
        goto L_4d63;

L_4d63:
    game.turn = rtbof.turn;
    game.wCrap = ((game.wCrap & 0xf1ff) | ((rtbof.wGen & 0x7) << 0x9));
    goto L_4ebd;

L_4d95:
    if ((rtbof.turn == game.turn))
        goto L_4db2;
    else
        goto L_4da0;

L_4da0:
    FileError(idmVigilantFleetsManagedDefeatSavageVerminWithout);
    goto LBadFile;

L_4db2:
    if ((dt != dtHost))
        goto L_4e11;
    else
        goto L_4dbb;

L_4dbb:
    if ((gd.fHostMode != 0x0))
        goto L_4e11;
    else
        goto L_4dcf;

L_4dcf:
    if ((rtbof.fInUse == 0x0))
        goto L_4e11;
    else
        goto L_4de2;

L_4de2:
    if ((AlertSz(PszFormatIds(idsHostFileMarkedUseAnotherInstanceStars, 0x0), 0x2024) != 0x6))
        goto LBadFile;
    else
        goto L_4e08;

L_4e08:

L_4e11:
    if ((rtbof.fDone != 0x0))
        goto L_4e58;
    else
        goto L_4e24;

L_4e24:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_4e58;
    else
        goto L_4e34;

L_4e34:
    if ((gd.fForceTurn != 0x0))
        goto L_4e58;
    else
        goto L_4e46;

L_4e46:
    gd.fPartialTurn = 0x1;
    goto LBadFile;

L_4e58:
    if ((dt != dtLog))
        goto L_4ebd;
    else
        goto L_4e61;

L_4e61:
    if ((game.fTutorial != 0x0))
        goto L_4ebd;
    else
        goto L_4e75;

L_4e75:
    if ((rtbof.wGen == game.wGen))
        goto L_4ebd;
    else
        goto L_4e93;

L_4e93:
    FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
    goto LBadFile;

L_4ea5:
    /* untranslated: branch ((part[12:2](rtbof) << 0xb) >> 0xb) != iPlayer ? L_4c36 : L_4eb7 */

L_4eb7:

L_4ebd:
    if ((fRewind == 0x0))
        goto L_4ee3;
    else
        goto L_4ec6;

L_4ec6:
    lseek(hf, 0x0, 0x0);
    ReadRt();

L_4ee3:
    penvMem = penvMemSav;
    wVersFile = rtbof.wVersion;
    HIWORD(gd) = ((HIWORD(gd) & 0xfffb) | ((rtbof.fCrippled & 0x1) << 0x2));
    return 0x1;
}

int16_t FNewTurnAvail(int16_t idPlayer) {
    uint16_t wGenOld;
    uint16_t turnOld;
    int16_t  fNew;
    uint16_t t_merge_4f84_0001;

L_4f22:
    turnOld = game.turn;
    wGenOld = game.wGen;
    fFileErrSilent = 0x1;
    game.turn = 0x0;
    fNew = FOpenFile(0x2003, idPlayer, 0x20);
    if ((fNew == 0x0))
        goto L_4f87;
    else
        goto L_4f6a;

L_4f6a:
    StreamClose();
    if ((game.turn <= turnOld))
        goto L_4f81;
    else
        goto L_4f7b;

L_4f7b:
    t_merge_4f84_0001 = 0x1;
    goto L_4f84;

L_4f81:
    t_merge_4f84_0001 = 0x0;

L_4f84:
    fNew = t_merge_4f84_0001;

L_4f87:
    game.turn = turnOld;
    game.wCrap = ((game.wCrap & 0xf1ff) | ((wGenOld & 0x7) << 0x9));

L_4fac:
    return fNew;
}

int16_t FCheckFile(DtFileType dt, int16_t iPlayer, uint16_t md) {
    int16_t  fReturn;
    int16_t  fOpened;
    uint16_t wGenOld;
    int16_t  f;
    int16_t  fErrSav;
    uint16_t t_merge_5047_0001;
    uint16_t t_merge_5072_0001;
    uint16_t t_merge_509d_0001;

L_4fb2:
    fErrSav = fFileErrSilent;
    wGenOld = game.wGen;
    goto L_4ff6;

L_4fd5:
    f = gd.fHostMode;
    gd.fHostMode = 0x1;
    goto L_5001;

L_4ff6:
    if ((dt == dtHost))
        goto L_4fd5;
    else
        goto L_5001;

L_5001:
    fFileErrSilent = 0x1;
    fOpened = FOpenFile(dt, iPlayer, 0x20);
    goto L_50e9;

L_5022:
    if ((fOpened == 0x0))
        goto L_503e;
    else
        goto L_502b;

L_502b:
    /* untranslated: branch ((part[14:2](rgbCur) >> 0x9) & 0x1) == 0x0 ? L_5044 : L_503e */

L_503e:
    t_merge_5047_0001 = 0x1;
    goto L_5047;

L_5044:
    t_merge_5047_0001 = 0x0;

L_5047:
    fReturn = t_merge_5047_0001;
    goto L_510c;

L_504d:
    if ((fOpened == 0x0))
        goto L_506f;
    else
        goto L_5056;

L_5056:
    /* untranslated: branch ((part[14:2](rgbCur) >> 0x8) & 0x1) == 0x0 ? L_506f : L_5069 */

L_5069:
    t_merge_5072_0001 = 0x1;
    goto L_5072;

L_506f:
    t_merge_5072_0001 = 0x0;

L_5072:
    fReturn = t_merge_5072_0001;
    goto L_510c;

L_5078:
    if ((fOpened == 0x0))
        goto L_509a;
    else
        goto L_5081;

L_5081:
    /* untranslated: branch ((part[14:2](rgbCur) >> 0xa) & 0x1) == 0x0 ? L_509a : L_5094 */

L_5094:
    t_merge_509d_0001 = 0x1;
    goto L_509d;

L_509a:
    t_merge_509d_0001 = 0x0;

L_509d:
    fReturn = t_merge_509d_0001;
    goto L_510c;

L_50a3:
    if ((fOpened != 0x0))
        goto L_50b4;
    else
        goto L_50ac;

L_50ac:
    fReturn = 0x0;
    goto L_510c;

L_50b4:
    ReadRt();
    if ((hdrCur.rt == rtPlr))
        goto L_50d8;
    else
        goto L_50cc;

L_50cc:
    if (((uint16_t)(LOWORD(rgbCur)) != iPlayer))
        goto L_50b4;
    else
        goto L_50d8;

L_50d8:
    /* untranslated: fReturn = ((part[6:2](rgbCur) >> 0x9) & 0x1) */

L_50e9:
    if ((md == 0x1))
        goto L_5022;
    else
        goto L_50f1;

L_50f1:
    if ((md == 0x2))
        goto L_504d;
    else
        goto L_50f9;

L_50f9:
    if ((md == 0x4))
        goto L_5078;
    else
        goto L_5101;

L_5101:
    if ((md == 0x8))
        goto L_50a3;
    else
        goto L_510c;

L_510c:
    if ((fOpened == 0x0))
        goto L_511a;
    else
        goto L_5115;

L_5115:
    StreamClose();

L_511a:
    if ((dt != dtHost))
        goto L_513c;
    else
        goto L_5123;

L_5123:
    LOWORD(gd) = ((LOWORD(gd) & 0xfff7) | ((f & 0x1) << 0x3));

L_513c:
    fFileErrSilent = fErrSav;
    game.wCrap = ((game.wCrap & 0xf1ff) | ((wGenOld & 0x7) << 0x9));

L_5161:
    return fReturn;
}

void ReadRt() {
L_5168:
    RgFromStream(hdrCur, 0x2);
    if ((hdrCur.cb == 0x0))
        goto L_51a8;
    else
        goto L_5192;

L_5192:
    RgFromStream(rgbCur, hdrCur.cb);

L_51a8:
    if ((hdrCur.rt != rtBOF))
        goto L_5221;
    else
        goto L_51bb;

L_51bb:
    /* untranslated: call SetFileXorStream(words(part[6:2](rgbCur), part[4:2](rgbCur)), (part[12:2](rgbCur) >> 0x5), part[10:2](rgbCur), ((part[12:2](rgbCur) <<
     * 0xb) >> 0xb), ((part[14:2](rgbCur) >> 0xc) & 0x1)) -> callresult(void) */
    goto L_5247;

L_5221:
    if ((hdrCur.rt == rtEOF))
        goto L_5247;
    else
        goto L_5234;

L_5234:
    XorFileBuf(0x4b98, hdrCur.cb);

L_5247:
    return;
}

int16_t FBadFileError(StringId ids) {
L_524e:
    goto L_5266;

L_525d:
    return 0x1;

L_5266:
    if ((ids == idsUniverseDefinitionFileSeemsMissingCorrupt))
        goto L_525d;
    else
        goto L_526e;

L_526e:
    if ((ids == idsPlayerLogFileAppearsCorruptUnableLoad))
        goto L_525d;
    else
        goto L_5276;

L_5276:
    if ((ids == idsHistoryFileAppearsCorruptHistoricalDataWill))
        goto L_525d;
    else
        goto L_527e;

L_527e:
    if ((ids == idsGameFileAppearsCorruptUnableLoadFile))
        goto L_525d;
    else
        goto L_5286;

L_5286:
    if ((ids == idsErrorWritingFile))
        goto L_525d;
    else
        goto L_528e;

L_528e:
    if ((ids == idsFileDate))
        goto L_525d;
    else
        goto L_5296;

L_5296:
    if ((ids == idsFileGame))
        goto L_525d;
    else
        goto L_52a1;

L_52a1:
    return 0x0;
}

void StreamOpen(char *szFile, int16_t mdOpen) {
    uint32_t dwTick;
    OFSTRUCT of;
    int16_t  fNoErr;
    uint32_t dwTickCur;
    uint16_t t_merge_52d8_0001;

L_52ae:
    dwTick = 0x0;
    if (((mdOpen & 0x4000) == 0x0))
        goto L_52d5;
    else
        goto L_52cf;

L_52cf:
    t_merge_52d8_0001 = 0x1;
    goto L_52d8;

L_52d5:
    t_merge_52d8_0001 = 0x0;

L_52d8:
    fNoErr = t_merge_52d8_0001;
    mdOpen = (mdOpen & 0xbfff);

Retry:
    hf = OpenFile(szFile, &(of), mdOpen);
    if ((hf != 0xffff))
        goto L_53c6;
    else
        goto L_5306;

L_5306:
    if ((gd.fRetryOpens == 0x0))
        goto L_53a0;
    else
        goto L_5319;

L_5319:
    if ((of.nErrCode == 0x2))
        goto L_53a0;
    else
        goto L_5323;

L_5323:
    dwTickCur = GetTickCount();
    if ((LOWORD(dwTick) != 0x0))
        goto L_5356;
    else
        goto L_5339;

L_5339:
    if ((HIWORD(dwTick) != 0x0))
        goto L_5356;
    else
        goto L_5342;

L_5342:
    dwTick = (dwTickCur + 0xfa0);

L_5356:
    if ((HIWORD(dwTickCur) > HIWORD(dwTick)))
        goto L_53a0;
    else
        goto L_5365;

L_5365:
    if ((HIWORD(dwTickCur) < HIWORD(dwTick)))
        goto L_5373;
    else
        goto L_536a;

L_536a:
    if ((LOWORD(dwTickCur) >= LOWORD(dwTick)))
        goto L_53a0;
    else
        goto L_5373;

L_5373:
    /* untranslated: LOWORD(dwTickCur) = part[8:0](szMine[LOWORD(dwTickCur)]) */
    HIWORD(dwTickCur) = (HIWORD(dwTickCur) + 0x0);

L_537e:
    if ((HIWORD(GetTickCount()) > HIWORD(dwTickCur)))
        goto Retry;
    else
        goto L_538c;

L_538c:
    /* untranslated: branch hiword(callresult(uint32_t)) < HIWORD(dwTickCur) ? L_537e : L_5391 */

L_5391:
    /* untranslated: branch loword(callresult(uint32_t)) < LOWORD(dwTickCur) ? L_537e : L_5397 */

L_5397:

L_53a0:
    if ((fNoErr != 0x0))
        goto L_53b6;
    else
        goto L_53aa;

L_53aa:
    FileError(idmPlanetaryDefensesGroundTroopsDestroyedInvadingTr);

L_53b6:
    longjmp(penvMem, 0xffff);

L_53c6:
    return;
}

void StreamClose() {
L_53cc:
    if ((hf == 0xffff))
        goto L_53ee;
    else
        goto L_53df;

L_53df:
    _lclose(hf);
    hf = 0xffff;

L_53ee:
    return;
}

void RgFromStream(void *rg, uint16_t cb) {
L_53f4:
    if ((cb == 0x0))
        goto L_5478;
    else
        goto L_5403;

L_5403:

L_5409:
    if ((LOWORD(vlpMemStream) != 0x0))
        goto L_541d;
    else
        goto L_5413;

L_5413:
    if ((HIWORD(vlpMemStream) == 0x0))
        goto L_5440;
    else
        goto L_541d;

L_541d:
    fmemcpy(rg, &(vlpMemStream), cb);
    vlpMemStream = ((uint8_t *)(vlpMemStream) + cb);
    goto L_5478;

L_5440:
    if ((_lread(hf, rg, cb) == cb))
        goto L_5478;
    else
        goto L_545c;

L_545c:
    FileError(idmGroundTroopsValiantlyDestroyedAttackingBarbarian);
    longjmp(penvMem, 0xffff);

L_5478:
    return;
}
