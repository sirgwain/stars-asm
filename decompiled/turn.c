#include "common.h"

int16_t rgpctMineHit[3] = {3, 10, 35};
int16_t rgiWarpSafe[3] = {4, 6, 5};
int16_t rgrgdmgMinMine[3][2] = {{500, 600}, {2000, 2500}};
int16_t rgrgdmgMine[3][2] = {{100, 125}, {500, 600}};

int16_t FGenerateTurn() {
    int16_t  fErrSav;
    char    *pchT;
    int16_t  ish;
    int16_t  j;
    uint8_t  mpiplr2[16];
    uint8_t  rgfNoXFile[16];
    jmp_buf *penvMemSav[9];
    int16_t  ifl;
    FLEET   *lpfl;
    char    *pchCur;
    int16_t  i;
    jmp_buf  env[9];
    char     szT[256];
    HCURSOR  hcurSav;
    int16_t  idCur;
    int16_t  fFollow;
    char    *pchBak;
    int16_t  fSuccess;
    int16_t  fDone;
    FLEET   *lpflTarget;
    ORDER    ord;
    int16_t  cAdv;
    PLANET  *lppl;
    PLANET  *lpplMac;
    int16_t  dPlanRange;
    int16_t  dRange;
    int16_t  iSteal;
    int16_t  pctDetect;
    uint16_t t_merge_06e8_0001;
    uint16_t t_merge_1670_0001;

L_0000:
    idCur = idPlayer;
    fSuccess = 0x0;
    hcurSav = SetCursor(LoadCursor(0x0, MAKEINTRESOURCE(0x7f02)));
    DestroyCurGame();
    if ((gd.fTutorial == 0x0))
        goto L_0059;
    else
        goto L_0049;

L_0049:
    Randomize(0x499602d2);

L_0059:
    fErrSav = fFileErrSilent;
    fFileErrSilent = 0x1;
    UpdateProgressGauge(0x168);
    if ((FLoadGame(0x56a2, 0x9c4) != 0x0))
        goto L_00aa;
    else
        goto L_0089;

L_0089:
    fFileErrSilent = fErrSav;
    SetCursor(hcurSav);
    TurnLog(idsCantFindHostFile);
    return 0x0;

L_00aa:
    TurnLog(idsGeneratingYearD);
    fFileErrSilent = fErrSav;
    if ((((wVersFile >> 0xc) & 0xf) > 0x0))
        goto L_016d;
    else
        goto L_00cf;

L_00cf:
    i = 0x0;
    goto L_00db;

L_00d7:
    i = (i + 0x1);

L_00db:
    if ((i >= game.cPlayer))
        goto L_010b;
    else
        goto L_00e6;

L_00e6:
    /* untranslated: branch ((part[6:2](rgplr[i]) >> 0x3) & 0x1f) != 0x0 ? L_010b : L_0102 */

L_0102:

L_010b:
    if ((i != game.cPlayer))
        goto L_016d;
    else
        goto L_0116;

L_0116:
    i = 0x0;
    goto L_0122;

L_011e:
    i = (i + 0x1);

L_0122:
    if ((i >= game.cPlayer))
        goto L_016d;
    else
        goto L_012d;

L_012d:
    /* untranslated: part[6:2](rgplr[i]) = ((part[6:2](rgplr[i]) & 0xff07) | ((i & 0x1f) * 0x8)) */
    goto L_011e;

L_016d:
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) != 0x0))
        goto FreeStuffUp;
    else
        goto L_018a;

L_018a:

L_0190:
    lpcd = LpAlloc(0x2ee0, htMisc);
    lpxf = LpAlloc(0x61a8, htMisc);
    vrgPlanResExtra = LpAlloc((game.cPlanMax * 0x2), htMisc);
    fmemset(&(vrgPlanResExtra), 0x0, (game.cPlanMax * 0x2));
    vrgts = LpAlloc((game.cPlayer * 0x10), htMisc);
    UpdateProgressGauge(0x172);
    cColDrop = 0x0;
    cXferFull = 0x0;
    gd.fGeneratingTurn = 0x1;
    gd.fRetryOpens = 0x1;
    imemMsgCur = 0x0;
    i = 0x0;
    goto L_0252;

L_024e:
    i = (i + 0x1);

L_0252:
    if ((i >= game.cPlayer))
        goto L_026d;
    else
        goto L_025d;

L_025d:
    mpiplr2[i] = LOBYTE(i);
    goto L_024e;

L_026d:
    i = 0x0;
    goto L_0279;

L_0275:
    i = (i + 0x1);

L_0279:
    if ((i >= game.cPlayer))
        goto L_02da;
    else
        goto L_0284;

L_0284:
    j = (Random((game.cPlayer - i)) + i);
    if ((j == i))
        goto L_0275;
    else
        goto L_02a4;

L_02a4:
    idCur = mpiplr2[j];
    mpiplr2[j] = mpiplr2[i];
    mpiplr2[i] = LOBYTE(idCur);

L_02da:
    i = 0x0;
    goto L_02e6;

L_02e2:
    i = (i + 0x1);

L_02e6:
    if ((i >= game.cPlayer))
        goto L_03b5;
    else
        goto L_02f1;

L_02f1:
    j = mpiplr2[i];
    _wsprintf(szWork, "%s.x%d", 0x56a2, 0x25, (j + 0x1));
    idPlayer = j;
    LOWORD(vrgts[j]) = 0xffff;
    HIWORD(vrgts[j]) = 0xffff;
    if ((FLoadLogFile(0x57a4) == 0x0))
        goto L_0392;
    else
        goto L_0361;

L_0361:
    if ((FRunLogFile() != 0x0))
        goto L_0392;
    else
        goto L_036e;

L_036e:
    AlertSz(PszFormatIds(idsPlayerLogFileAppearsCorruptUnableLoad, 0x0), MB_ICONHAND);
    goto FreeStuffUp;

L_0392:
    UpdateProgressGauge((MulDiv(0x3c, (i + 0x1), game.cPlayer) + 0x172));
    goto L_02e2;

L_03b5:
    idPlayer = 0xffff;
    i = 0x0;
    goto L_03c7;

L_03c3:
    i = (i + 0x1);

L_03c7:
    if ((i >= game.cPlayer))
        goto L_0689;
    else
        goto L_03d2;

L_03d2:
    /* untranslated: branch ((part[84:2](rgplr[i]) >> 0x1) & 0x1) != 0x0 ? L_0427 : L_03ed */

L_03ed:
    /* untranslated: branch ((part[6:2](rgplr[i]) >> 0x9) & 0x1) != 0x0 ? L_0427 : L_040b */

L_040b:
    if ((gd.fTutorial == 0x0))
        goto L_0454;
    else
        goto L_041e;

L_041e:
    if ((i != 0x0))
        goto L_0454;
    else
        goto L_0427;

L_0427:
    /* untranslated: part[84:2](rgplr[i]) = ((part[84:2](rgplr[i]) & 0xfffb) | 0x0) */
    goto L_03c3;

L_0454:
    if ((LOWORD(vrgts[i]) != 0xffff))
        goto L_047e;
    else
        goto L_0474;

L_0474:
    if ((HIWORD(vrgts[i]) == 0xffff))
        goto L_04db;
    else
        goto L_047e;

L_047e:
    /* untranslated: branch FValidSerialLong(words(HIWORD(vrgts[i]), LOWORD(vrgts[i]))) != 0x0 ? L_04db : L_04ae */

L_04ae:
    /* untranslated: part[84:2](rgplr[i]) = ((part[84:2](rgplr[i]) & 0xfffb) | 0x4) */
    goto L_03c3;

L_04db:
    if ((LOWORD(vrgts[i]) != 0xffff))
        goto L_0505;
    else
        goto L_04fb;

L_04fb:
    if ((HIWORD(vrgts[i]) == 0xffff))
        goto L_03c3;
    else
        goto L_0505;

L_0505:
    /* untranslated: part[84:2](rgplr[i]) = ((part[84:2](rgplr[i]) & 0xfffb) | 0x0) */
    j = 0x0;
    goto L_067b;

L_0537:
    /* untranslated: branch ((part[84:2](rgplr[j]) >> 0x1) & 0x1) != 0x0 ? L_0677 : L_0552 */

L_0552:
    /* untranslated: branch ((part[6:2](rgplr[j]) >> 0x9) & 0x1) != 0x0 ? L_0677 : L_0570 */

L_0570:
    if ((LOWORD(vrgts[i]) != LOWORD(vrgts[j])))
        goto L_0677;
    else
        goto L_05af;

L_05af:
    if ((HIWORD(vrgts[i]) != HIWORD(vrgts[j])))
        goto L_0677;
    else
        goto L_05b8;

L_05b8:
    /* untranslated: branch fmemcmp(&part[4:0](vrgts[i]), &part[4:0](vrgts[j]), 0xb) == 0x0 ? L_0677 : L_0604 */

L_0604:
    /* untranslated: part[84:2](rgplr[j]) = ((part[84:2](rgplr[j]) & 0xfffb) | 0x4) */
    /* untranslated: part[84:2](rgplr[i]) = ((part[84:2](rgplr[i]) & 0xfffb) | ((0x1 & 0x1) * 0x4)) */

L_0677:
    j = (j + 0x1);

L_067b:
    if ((j >= i))
        goto L_03c3;
    else
        goto L_0683;

L_0683:

L_0689:
    i = 0x0;
    goto L_0695;

L_0691:
    i = (i + 0x1);

L_0695:
    if ((i >= game.cPlayer))
        goto L_0732;
    else
        goto L_06a0;

L_06a0:
    /* untranslated: branch ((part[84:2](rgplr[i]) >> 0x2) & 0x1) == 0x0 ? L_0691 : L_06bd */

L_06bd:
    j = IPlrAlsoCheater(i);
    if ((j == 0xffff))
        goto L_06e5;
    else
        goto L_06df;

L_06df:
    t_merge_06e8_0001 = 0x1;
    goto L_06e8;

L_06e5:
    t_merge_06e8_0001 = 0x0;

L_06e8:
    FSendPlrMsg2(i, (t_merge_06e8_0001 + 0x100), 0xfffb, j, 0x0);
    if ((game.turn <= 0xa))
        goto L_0691;
    else
        goto L_0701;

L_0701:
    if (((game.turn & 0x7) != (i & 0x7)))
        goto L_0691;
    else
        goto L_0714;

L_0714:
    FSendPlrMsg2(i, 0x103, 0xfffb, 0x0, 0x0);

L_0732:
    i = 0x0;
    goto L_073e;

L_073a:
    i = (i + 0x1);

L_073e:
    if ((i >= game.cPlayer))
        goto L_0890;
    else
        goto L_0749;

L_0749:
    ish = 0x0;
    goto L_0884;

L_0751:
    /* untranslated: branch ((part[123:2](rglpshdef[i][ish]) >> 0x9) & 0x1) != 0x0 ? L_0880 : L_077b */

L_077b:

L_0781:
    if ((rglpshdef[i][ish].hul.rghs[0x0].grhst == hstEngine))
        goto L_0880;
    else
        goto L_07a4;

L_07a4:
    rglpshdef[i][ish].hul.rghs[0x0].grhst = hstEngine;
    HIWORD(rglpshdef[i][ish].hul.rghs[0x0]) = ((HIWORD(rglpshdef[i][ish].hul.rghs[0x0]) & 0xff00) | 0x1);
    if ((((HIWORD(rglpshdef[i][ish].hul.rghs[0x0]) >> 0x8) & 0xff) >= 0x1))
        goto L_0880;
    else
        goto L_0838;

L_0838:
    HIWORD(rglpshdef[i][ish].hul.rghs[0x0]) = ((HIWORD(rglpshdef[i][ish].hul.rghs[0x0]) & 0xff) | 0x100);

L_0880:
    ish = (ish + 0x1);

L_0884:
    if ((ish >= 0x10))
        goto L_073a;
    else
        goto L_088a;

L_088a:

L_0890:
    fFollow = 0x0;
    ifl = 0x0;
    goto L_08a2;

L_089e:
    ifl = (ifl + 0x1);

L_08a2:
    if ((ifl >= cFleet))
        goto L_09b0;
    else
        goto L_08ad;

L_08ad:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_08dd;
    else
        goto L_08d5;

L_08d5:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_09b0;
    else
        goto L_08dd;

L_08dd:
    lpfl->fNoHeal = 0x0;
    if ((lpfl->cord != 0x1))
        goto L_0936;
    else
        goto L_08fe;

L_08fe:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x0]) >> 0x8) & 0xf) != 0x2 ? L_0936 : L_0919 */

L_0919:
    fFollow = 0x1;
    lpfl->fMark = 0x1;
    goto L_089e;

L_0936:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x0]) >> 0x8) & 0xf) != 0x2 ? L_0999 : L_0951 */

L_0951:
    if ((lpfl->cord != 0x1))
        goto L_0999;
    else
        goto L_095e;

L_095e:
    FSendPlrMsg(lpfl->iPlayer, 0x138, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);

L_0999:
    lpfl->fMark = 0x0;

L_09b0:
    ValidateWaypoints();
    if ((fFollow == 0x0))
        goto L_0c1e;
    else
        goto L_09bf;

L_09bf:
    fFollow = 0x1;
    i = 0x0;
    goto L_09d1;

L_09cd:
    i = (i + 0x1);

L_09d1:
    if ((i >= 0x8))
        goto L_0c1e;
    else
        goto L_09da;

L_09da:
    if ((fFollow == 0x0))
        goto L_0c1e;
    else
        goto L_09e4;

L_09e4:
    fFollow = 0x0;
    ifl = 0x0;
    goto L_09f6;

L_09f2:
    ifl = (ifl + 0x1);

L_09f6:
    if ((ifl >= cFleet))
        goto L_09cd;
    else
        goto L_0a01;

L_0a01:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_0a31;
    else
        goto L_0a29;

L_0a29:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_09cd;
    else
        goto L_0a31;

L_0a31:
    if ((lpfl->fMark == 0x0))
        goto L_09f2;
    else
        goto L_0a48;

L_0a48:
    if ((lpfl->cord != 0x1))
        goto L_09f2;
    else
        goto L_0a55;

L_0a55:
    ord = lpfl->lpplord->rgord[0x0];
    if ((ord.grobj == grobjFleet))
        goto L_0aa5;
    else
        goto LUnmark;

LUnmark:
    lpfl->fMark = 0x0;
    goto L_09f2;

L_0aa5:
    lpflTarget = LpflFromId(ord.id);
    if ((LOWORD(lpflTarget) != 0x0))
        goto L_0acd;
    else
        goto L_0ac3;

L_0ac3:
    if ((HIWORD(lpflTarget) == 0x0))
        goto L_0af7;
    else
        goto L_0acd;

L_0acd:
    if ((lpflTarget->cord != 0x1))
        goto L_0b35;
    else
        goto L_0adb;

L_0adb:
    /* untranslated: branch ((part[6:2](lpflTarget->lpplord->rgord[0x0]) >> 0x8) & 0xf) == 0x2 ? L_0b35 : L_0af7 */

L_0af7:
    FSendPlrMsg(lpfl->iPlayer, 0x138, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
    goto LUnmark;

L_0b35:
    if ((lpflTarget->cord == 0x1))
        goto L_09f2;
    else
        goto L_0b40;

L_0b40:

L_0b46:
    fFollow = 0x1;
    if ((lpfl->lpplord->iordMax > 0x1))
        goto L_0b84;
    else
        goto L_0b62;

L_0b62:
    lpfl->lpplord = LpplReAlloc(lpfl->lpplord, 0x2);

L_0b84:
    lpfl->lpplord->rgord[0x1] = lpflTarget->lpplord->rgord[0x1];
    /* untranslated: part[8:10](lpfl->lpplord->rgord[0x1]) = part[8:10](lpfl->lpplord->rgord[0x0]) */
    lpfl->cord = 0x2;
    lpfl->lpplord->iordMac = 0x2;

L_0c1e:
    UpdateProgressGauge(0x1b8);
    DoOrders(0x0);
    UpdateProgressGauge(0x212);
    i = 0x0;
    goto L_0c4e;

L_0c4a:
    i = (i + 0x1);

L_0c4e:
    if ((i >= game.cPlayer))
        goto L_0f6a;
    else
        goto L_0c59;

L_0c59:
    j = 0x0;
    goto L_0c98;

L_0c62:
    SetRaceStat(rgplr[i], j, GetRaceStat(rgplr[i], j));
    j = (j + 0x1);

L_0c98:
    if ((j < 0x10))
        goto L_0c62;
    else
        goto L_0ca2;

L_0ca2:
    if (((uint16_t)(rgplr[i].pctResearch) < 0x0))
        goto L_0cd0;
    else
        goto L_0cb9;

L_0cb9:
    if (((uint16_t)(rgplr[i].pctResearch) <= 0x64))
        goto L_0cdf;
    else
        goto L_0cd0;

L_0cd0:
    rgplr[i].pctResearch = 0xf;

L_0cdf:
    if (((uint16_t)(rgplr[i].pctIdealGrowth) >= 0x0))
        goto L_0d05;
    else
        goto L_0cf6;

L_0cf6:
    rgplr[i].pctIdealGrowth = 0x1;

L_0d05:
    if (((uint16_t)(rgplr[i].pctIdealGrowth) <= 0x14))
        goto L_0d2b;
    else
        goto L_0d1c;

L_0d1c:
    rgplr[i].pctIdealGrowth = 0x14;

L_0d2b:
    /* untranslated: j = ((part[84:2](rgplr[i]) >> 0x4) & 0x1) */
    cAdv = CAdvantagePoints(rgplr[i]);
    if ((cAdv < 0x0))
        goto L_0d8c;
    else
        goto L_0d6a;

L_0d6a:
    /* untranslated: branch j == ((part[84:2](rgplr[i]) >> 0x4) & 0x1) ? L_0c4a : L_0d8c */

L_0d8c:
    /* untranslated: branch ((part[6:2](rgplr[i]) >> 0x9) & 0x1) != 0x0 ? L_0c4a : L_0daa */

L_0daa:
    FSendPlrMsg2(i, 0x117, 0xffff, 0x0, 0x0);
    j = 0x0;
    goto L_0dd3;

L_0dce:
    j = (j + 0x1);

L_0dd3:
    if ((j >= game.cPlayer))
        goto L_0e27;
    else
        goto L_0ddf;

L_0ddf:
    if ((i == j))
        goto L_0dce;
    else
        goto L_0deb;

L_0deb:
    /* untranslated: branch ((part[6:2](rgplr[i]) >> 0x9) & 0x1) != 0x0 ? L_0dce : L_0e09 */

L_0e09:
    FSendPlrMsg2(j, 0x182, 0xffff, i, 0x0);

L_0e27:
    /* untranslated: part[84:2](rgplr[i]) = ((part[84:2](rgplr[i]) & 0xffef) | 0x10) */
    if ((cAdv >= 0x1f4))
        goto L_0eab;
    else
        goto L_0e5c;

L_0e5c:
    if (((uint16_t)(rgplr[i].rgAttr[0x0]) >= 0x19))
        goto L_0eab;
    else
        goto L_0e73;

L_0e73:
    rgplr[i].rgAttr[0x0] = (rgplr[i].rgAttr[0x0] + 0x1);
    cAdv = CAdvantagePoints(rgplr[i]);
    if ((cAdv >= 0x1f4))
        goto L_0eab;
    else
        goto L_0ea2;

L_0ea2:

L_0eab:
    if ((cAdv >= 0x1f4))
        goto L_0f05;
    else
        goto L_0eb6;

L_0eb6:
    if (((uint16_t)(rgplr[i].pctIdealGrowth) <= 0x1))
        goto L_0f05;
    else
        goto L_0ecd;

L_0ecd:
    rgplr[i].pctIdealGrowth = (rgplr[i].pctIdealGrowth - 0x1);
    cAdv = CAdvantagePoints(rgplr[i]);
    if ((cAdv >= 0x1f4))
        goto L_0f05;
    else
        goto L_0efc;

L_0efc:

L_0f05:
    if ((cAdv >= 0x1f4))
        goto L_0c4a;
    else
        goto L_0f10;

L_0f10:
    j = 0x8;
    goto L_0f5d;

L_0f19:
    rgplr[i].rgAttr[j] = 0x0;
    cAdv = CAdvantagePoints(rgplr[i]);
    if ((cAdv >= 0x1f4))
        goto L_0c4a;
    else
        goto L_0f52;

L_0f52:

L_0f58:
    j = (j + 0x1);

L_0f5d:
    if ((j > 0xd))
        goto L_0c4a;
    else
        goto L_0f64;

L_0f64:

L_0f6a:
    UnmarkMineFields();
    MoveThings(0x0);
    UpdateProgressGauge(0x226);
    MoveFleets();
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_0fda;

L_0fbf:
    lppl->fHomeworld = 0x0;
    lppl = (lppl + 0x1);

L_0fda:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_0fbf;
    else
        goto L_0feb;

L_0feb:
    i = 0x0;
    goto L_0ff7;

L_0ff3:
    i = (i + 0x1);

L_0ff7:
    if ((i >= game.cPlayer))
        goto L_1055;
    else
        goto L_1002;

L_1002:
    /* untranslated: part[4:2](lpPlanets[rgplr[i].idPlanetHome]) = ((part[4:2](lpPlanets[rgplr[i].idPlanetHome]) & 0xfbff) | 0x400) */
    goto L_0ff3;

L_1055:
    UpdateProgressGauge(0x28a);
    ThingDecay();
    BreedColonistsInTransit();
    UpdateProgressGauge(0x2bc);
    Produce();
    UpdateProgressGauge(0x2ee);
    MoveThings(0x1);
    UpdateProgressGauge(0x302);
    FuelFleets();
    DoOrders(0x1);
    SweepForMines();
    HealShips();
    AutoTerraform();
    RemoteTerraforming();
    UpdateProgressGauge(0x352);
    SpankTheCheaters();
    ValidateWaypoints();
    UpdateGuesses();
    UpdateProgressGauge(0x354);
    FMarkFile(dtHost, 0xffff, 0x1, 0x0);
    CreateBackupDir();
    game.turn = (game.turn + 0x1);
    pchCur = szBase[strlen(0x56a2)];
    pchT = strrchr(0x56a2, 0x5c);
    strcpy(szT, 0x557a);
    if ((pchT != 0x0))
        goto L_1163;
    else
        goto L_114f;

L_114f:
    strcat(szT, 0x56a2);
    goto L_1179;

L_1163:
    strcat(szT, &(pchT[0x1]));

L_1179:
    pchBak = szT[strlen(szT)];
    UpdateProgressGauge(0x356);
    UpdatePlayerScores();
    i = 0x0;
    goto L_11ad;

L_11a9:
    i = (i + 0x1);

L_11ad:
    if ((i >= game.cPlayer))
        goto L_1473;
    else
        goto L_11b8;

L_11b8:
    j = 0x0;
    goto L_12d2;

L_11c0:
    /* untranslated: branch ((part[123:2](rglpshdefSB[i][j]) >> 0x9) & 0x1) != 0x0 ? L_12ce : L_11ed */

L_11ed:
    PctCloakFromHuldef(&(rglpshdefSB[i][j]), i, 0x0);
    /* untranslated: ss:[bp-0x156] = signhiword((0x64 - callresult(int16_t))) */
    /* untranslated: part[135:2](rglpshdefSB[i][j]) = (0x64 - callresult(int16_t)) */
    /* untranslated: part[137:2](rglpshdefSB[i][j]) = ss:[bp-0x156] */
    /* untranslated: ss:[bp-0x156] = hiword((uint32_t)(words(part[137:2](rglpshdefSB[i][j]), part[135:2](rglpshdefSB[i][j])) *
     * words(part[137:2](rglpshdefSB[i][j]), part[135:2](rglpshdefSB[i][j])))) */
    /* untranslated: part[135:2](rglpshdefSB[i][j]) = loword((uint32_t)(words(part[137:2](rglpshdefSB[i][j]), part[135:2](rglpshdefSB[i][j])) *
     * words(part[137:2](rglpshdefSB[i][j]), part[135:2](rglpshdefSB[i][j])))) */
    /* untranslated: part[137:2](rglpshdefSB[i][j]) = ss:[bp-0x156] */

L_12ce:
    j = (j + 0x1);

L_12d2:
    if ((j < 0xa))
        goto L_11c0;
    else
        goto L_12db;

L_12db:
    j = 0x0;
    goto L_1467;

L_12e3:
    /* untranslated: branch ((part[123:2](rglpshdef[i][j]) >> 0x9) & 0x1) != 0x0 ? L_1463 : L_1310 */

L_1310:
    dRange = GetShdefScannerRange(&(rglpshdef[i][j]), i, &(dPlanRange), &(pctDetect), &(iSteal));
    rglpshdef[i][j].dScanRange = dRange;
    rglpshdef[i][j].dScanRange2 = dPlanRange;
    rglpshdef[i][j].pctDetect = LOBYTE(pctDetect);
    rglpshdef[i][j].iSteal = LOBYTE(iSteal);
    if ((FCanBuildShdef(&(rglpshdef[i][j]), i) != 0x0))
        goto L_1463;
    else
        goto L_141b;

L_141b:
    /* untranslated: part[123:2](rglpshdef[i][j]) = ((part[123:2](rglpshdef[i][j]) & 0x7fff) | 0x8000) */

L_1463:
    j = (j + 0x1);

L_1467:
    if ((j >= 0x10))
        goto L_11a9;
    else
        goto L_146d;

L_146d:

L_1473:
    j = 0x358;
    fDone = 0x0;
    memset(rgfNoXFile, 0x0, 0x10);
    i = 0x0;
    goto L_15cf;

L_149a:
    UpdateProgressGauge(j);
    j = (j + (0x110000 / (game.cPlayer + 0x1)));
    if ((i < game.cPlayer))
        goto L_14cb;
    else
        goto L_14c0;

L_14c0:
    i = 0xffff;
    fDone = 0x1;

L_14cb:
    if ((i < 0x0))
        goto L_1556;
    else
        goto L_14d4;

L_14d4:
    _wsprintf(pchCur, ".x%d", (i + 0x1));
    strcpy(pchBak, pchCur);
    remove(szT);
    if ((access(0x56a2, 0x0) != 0xffff))
        goto L_1533;
    else
        goto L_1525;

L_1525:
    rgfNoXFile[i] = 0x1;
    goto L_1544;

L_1533:
    rename(0x56a2, szT);

L_1544:
    pchBak[0x1] = 0x6d;
    pchCur[0x1] = 0x6d;
    goto L_1575;

L_1556:
    strcpy(pchCur, 0x9d4);
    strcpy(pchBak, 0x9d9);

L_1575:
    remove(szT);
    if ((i < 0x0))
        goto L_15b4;
    else
        goto L_158b;

L_158b:
    if ((rgfNoXFile[i] == 0x0))
        goto L_15b4;
    else
        goto L_15a0;

L_15a0:
    CopyFile(0x56a2, szT);
    goto L_15c5;

L_15b4:
    rename(0x56a2, szT);

L_15c5:
    *(pchCur) = 0x0;
    i = (i + 0x1);

L_15cf:
    if ((fDone == 0x0))
        goto L_149a;
    else
        goto L_15d9;

L_15d9:
    j = 0x36b;
    fDone = 0x0;
    game.wCrap = ((game.wCrap & 0xf1ff) | ((Random(0x8) & 0x7) << 0x9));
    i = 0x0;
    goto L_1684;

L_1618:
    UpdateProgressGauge(j);
    j = (j + (0x7a0000 / (game.cPlayer + 0x1)));
    if ((i < game.cPlayer))
        goto L_1649;
    else
        goto L_163e;

L_163e:
    i = 0xffff;
    fDone = 0x1;

L_1649:
    if ((i == 0xffff))
        goto L_166d;
    else
        goto L_1652;

L_1652:
    if ((rgfNoXFile[i] == 0x0))
        goto L_166d;
    else
        goto L_1667;

L_1667:
    t_merge_1670_0001 = 0x1;
    goto L_1670;

L_166d:
    t_merge_1670_0001 = 0x0;

L_1670:
    FWriteDataFile(0x56a2, i, t_merge_1670_0001);
    i = (i + 0x1);

L_1684:
    if ((fDone == 0x0))
        goto L_1618;
    else
        goto L_168e;

L_168e:
    UpdateProgressGauge(0x3e6);
    imemLogCur = 0x0;
    fSuccess = 0x1;

FreeStuffUp:
    UpdateProgressGauge(0x3e8);
    FreeLp(&(vrgPlanResExtra), htMisc);
    vrgPlanResExtra = 0x0;
    FreeLp(&(vrgts), htMisc);
    vrgts = 0x0;
    FreeLp(&(lpcd), htMisc);
    lpcd = 0x0;
    FreeLp(&(lpxf), htMisc);
    lpxf = 0x0;
    gd.fGeneratingTurn = 0x0;
    gd.fRetryOpens = 0x0;
    idPlayer = 0xffff;
    if ((fSuccess == 0x0))
        goto L_1774;
    else
        goto L_175a;

L_175a:
    if ((ini.fGen == 0x0))
        goto L_1774;
    else
        goto L_176e;

L_176e:
    vretExitValue = 0x1;

L_1774:
    SetCursor(hcurSav);
    TurnLog((fSuccess + 0x564));
    return fSuccess;
}

void DoOrders(int16_t fPostMovement) {
    PLANET  *lppl;
    PLANET  *lpplMac;
    uint16_t t_merge_17e4_0001;
    uint16_t t_merge_18ab_0001;
    uint16_t t_merge_18d7_0001;

L_179a:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_180c;

L_17ce:
    if ((lppl->iPlayer == 0xffff))
        goto L_17e1;
    else
        goto L_17db;

L_17db:
    t_merge_17e4_0001 = 0x1;
    goto L_17e4;

L_17e1:
    t_merge_17e4_0001 = 0x0;

L_17e4:
    *(lppl + 0x4) = ((*(lppl + 0x4) & 0xdfff) | ((t_merge_17e4_0001 & 0x1) << 0xd));
    lppl = (lppl + 0x1);

L_180c:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_17ce;
    else
        goto L_181a;

L_181a:
    if ((fPostMovement == 0x0))
        goto L_183f;
    else
        goto L_1823;

L_1823:
    idBattle = (((game.turn & 0xf) * 0x100) + 0x1);
    DoBattles(fPostMovement);

L_183f:
    DoThingInteractions(fPostMovement);
    if ((fPostMovement == 0x0))
        goto L_1899;
    else
        goto L_1853;

L_1853:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_188b;

L_187e:
    lppl->turn = 0x0;
    lppl = (lppl + 0x1);

L_188b:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_187e;
    else
        goto L_1899;

L_1899:
    if ((fPostMovement == 0x0))
        goto L_18a8;
    else
        goto L_18a2;

L_18a2:
    t_merge_18ab_0001 = 0x3;
    goto L_18ab;

L_18a8:
    t_merge_18ab_0001 = 0x1;

L_18ab:
    SatisfyOrders(t_merge_18ab_0001);
    DropColonists();
    UpdateResearchStatus(0x0);
    if ((fPostMovement == 0x0))
        goto L_18d4;
    else
        goto L_18ce;

L_18ce:
    t_merge_18d7_0001 = 0x4;
    goto L_18d7;

L_18d4:
    t_merge_18d7_0001 = 0x2;

L_18d7:
    SatisfyOrders(t_merge_18d7_0001);
    if ((fPostMovement != 0x0))
        goto L_18ee;
    else
        goto L_18e9;

L_18e9:
    TransferToOthers();

L_18ee:
    return;
}

void MoveThings(int16_t fPostProd) {
    int16_t   k;
    int16_t   dUni;
    double    d;
    POINT     pt;
    int16_t   iMax;
    POINT     ptDst;
    int16_t   dLeft;
    THING    *lpth;
    int16_t   fAnythingMoved;
    int16_t   fMajorMove;
    MessageId idm;
    int16_t   iLow;
    POINT     ptSrc;
    THING    *lpthMac;
    int16_t   dRange;
    POINT     ptBase;
    int16_t   iX;
    int16_t   rgC[2];
    int16_t   rgwtTerra[3];
    int32_t   wtTot;
    int16_t   iWarp2;
    int16_t   iWarp;
    int16_t   fTerra;
    PLANET   *lppl;
    int16_t   wtCur;
    int16_t   pctMinKeep;
    int16_t   fTwoMAs;
    int32_t   lDefKilled;
    int32_t   lColKilled;
    int16_t   i;
    int16_t   pctCaught;
    float     pct;
    int32_t   dmgRaw;
    int16_t   iWarpPacket;
    int16_t   iWarpPacket2;
    THING    *lpth2;
    THING    *lpth2Mac;
    int16_t   pctRate;
    int16_t   iplr;
    int16_t   rgMin[3];
    int16_t   cTerraPerm;
    int16_t   cTerraTemp;
    int16_t   rgMax[3];
    int16_t   rgCost[3];
    double    dyRound;
    double    dxRound;
    double    r;
    uint16_t  t_merge_1992_0001;
    int16_t   t_19fb;
    uint16_t  t_merge_1bd2_0001;
    uint16_t  t_merge_200d_0001;
    int16_t   t_merge_226e_0001;
    int16_t   t_merge_2338_0001;
    int16_t   t_merge_2386_0001;
    uint16_t  t_merge_2542_0001;
    uint16_t  t_merge_25a7_0001;
    int16_t   t_merge_26a3_0001;
    int16_t   t_merge_26f1_0001;
    uint16_t  t_merge_283e_0001;
    uint16_t  t_merge_28b8_0001;
    uint16_t  t_merge_29a5_0001;
    uint16_t  t_merge_2b4d_0001;
    uint16_t  t_merge_2bf5_0001;
    uint16_t  t_merge_2c65_0001;
    float     t_merge_2daf_0001;
    float     t_merge_2dd1_0001;

L_18f4:
    fAnythingMoved = 0x0;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_2ed7;

L_192d:
    if ((lpth->ith != ithWormhole))
        goto L_1af7;
    else
        goto L_1943;

L_1943:
    if ((fPostProd == 0x0))
        goto L_1af7;
    else
        goto L_194c;

L_194c:
    k = 0x0;
    ptBase.x = lpth->pt.x;
    ptBase.y = lpth->pt.y;
    /* untranslated: ss:[bp-0x36] = Random(0x64) */
    /* untranslated: branch ss:[bp-0x36] >= PctWormholeMoves(lpth) ? L_198f : L_1989 */

L_1989:
    t_merge_1992_0001 = 0x1;
    goto L_1992;

L_198f:
    t_merge_1992_0001 = 0x0;

L_1992:
    fMajorMove = t_merge_1992_0001;
    if ((fMajorMove == 0x0))
        goto L_19cb;
    else
        goto L_199e;

L_199e:
    lpth->thw.grbitPlr = 0x0;
    dUni = (LOWORD((0x190 * game.mdSize)) + 0x190);
    LOWORD(lpth->thw) = ((LOWORD(lpth->thw) & 0xf003) | 0x0);
    goto L_19f6;

L_19cb:
    /* untranslated: ss:[bp-0x36] = ((LOWORD(lpth->thw) + 0x4) & 0xffc) */
    LOWORD(lpth->thw) = (LOWORD(lpth->thw) & 0xf003);
    /* untranslated: LOWORD(lpth->thw) = (LOWORD(lpth->thw) | ss:[bp-0x36]) */

L_19f6:
    iMax = 0x10;

L_19fb:
    t_19fb = k;
    k = (k + 0x1);
    if ((t_19fb >= 0x64))
        goto L_1ada;
    else
        goto L_1a0a;

L_1a0a:
    if ((fMajorMove == 0x0))
        goto L_1a40;
    else
        goto L_1a13;

L_1a13:
    lpth->pt.x = (Random(dUni) + 0x3e8);
    lpth->pt.y = (Random(dUni) + 0x3e8);
    goto L_1a72;

L_1a40:
    lpth->pt.x = ((Random(0x19) + ptBase.x) + 0xfff4);
    lpth->pt.y = ((Random(0x19) + ptBase.y) + 0xfff4);

L_1a72:
    /* untranslated: branch lpth->pt.x != ss:[ptBase] ? L_1a98 : L_1a8c */

L_1a8c:
    /* untranslated: branch lpth->pt.y == ss:[ptBase+0x2] ? L_19fb : L_1a92 */

L_1a92:

L_1a98:
    iLow = IValidateWormholePos(lpth);
    if ((iLow == 0x0))
        goto L_1ada;
    else
        goto L_1aaf;

L_1aaf:

L_1ab5:
    if ((iLow >= iMax))
        goto L_19fb;
    else
        goto L_1ac0;

L_1ac0:
    iMax = iLow;
    pt.x = lpth->pt.x;
    pt.y = lpth->pt.y;

L_1ada:
    if ((iLow == 0x0))
        goto L_2ed3;
    else
        goto L_1ae3;

L_1ae3:
    lpth->pt.x = pt.x;
    lpth->pt.y = pt.y;

L_1af7:
    if ((lpth->ith != ithMysteryTrader))
        goto L_1c7a;
    else
        goto L_1b0d;

L_1b0d:
    if ((fPostProd != 0x0))
        goto L_1c7a;
    else
        goto L_1b16;

L_1b16:
    dRange = lpth->tht.iWarp;
    if ((dRange >= 0xd))
        goto L_1c3e;
    else
        goto L_1b2c;

L_1b2c:
    if ((Random(0x19) != 0x0))
        goto L_1c3e;
    else
        goto L_1b40;

L_1b40:
    idm = idmMysteryTraderHasUnexplicablyChangedHisCourse;
    if ((Random(0x3) != 0x0))
        goto LSpeedUpOnly;
    else
        goto L_1b56;

L_1b56:

LRetargetFreighter:
    if ((Random(0x2) != 0x0))
        goto L_1b80;
    else
        goto L_1b70;

L_1b70:
    LOWORD(rgC) = (LOWORD((0x190 * game.mdSize)) + 0x564);
    goto L_1b85;

L_1b80:
    LOWORD(rgC) = 0x3fc;

L_1b85:
    HIWORD(rgC) = (Random((LOWORD((0x190 * game.mdSize)) + 0x169)) + 0x3fc);
    iX = Random(0x2);
    lpth->tht.ptDest.x = rgC[iX];
    if ((iX != 0x0))
        goto L_1bcf;
    else
        goto L_1bc9;

L_1bc9:
    t_merge_1bd2_0001 = 0x1;
    goto L_1bd2;

L_1bcf:
    t_merge_1bd2_0001 = 0x0;

L_1bd2:
    lpth->tht.ptDest.y = rgC[t_merge_1bd2_0001];

LSpeedUpOnly:
    dRange = (dRange + 0x1);
    *(lpth + 0xa) = ((*(lpth + 0xa) & 0xfff0) | (dRange & 0xf));
    k = 0x0;
    goto L_1c13;

L_1c0f:
    k = (k + 0x1);

L_1c13:
    if ((k >= game.cPlayer))
        goto L_1c3e;
    else
        goto L_1c1e;

L_1c1e:
    FSendPlrMsg2(k, idm, 0xfffa, LOWORD(lpth), 0x0);
    goto L_1c0f;

L_1c3e:
    dRange = lpth->tht.iWarp;
    dRange = LOWORD((dRange * dRange));
    ptDst.x = lpth->tht.ptDest.x;
    ptDst.y = lpth->tht.ptDest.y;
    fAnythingMoved = 0x1;
    if ((idm == idmMysteryTraderHasDecidedMakeAnotherPass))
        goto L_2ed3;
    else
        goto L_1c71;

L_1c71:

L_1c7a:
    if ((lpth->ith != ithMineralPacket))
        goto L_2ed3;
    else
        goto L_1c90;

L_1c90:
    if ((((LOWORD(lpth->thp) >> 0xa) & 0xf) == 0x0))
        goto L_2ed3;
    else
        goto L_1ca4;

L_1ca4:

L_1caa:
    if ((fPostProd == 0x0))
        goto L_1ccd;
    else
        goto L_1cb3;

L_1cb3:
    if ((((LOWORD(lpth->thp) >> 0xe) & 0x1) != 0x0))
        goto L_2ed3;
    else
        goto L_1cc7;

L_1cc7:

L_1ccd:
    if ((LOWORD(lpth->thp.rgwtMin) != 0x0))
        goto L_1cf7;
    else
        goto L_1cda;

L_1cda:
    if ((HIWORD(lpth->thp.rgwtMin) != 0x0))
        goto L_1cf7;
    else
        goto L_1ce7;

L_1ce7:
    /* untranslated: branch part[4:2](lpth->thp.rgwtMin) == 0x0 ? L_2d7a : L_1cf1 */

L_1cf1:

L_1cf7:
    LOWORD(lpth->thp) = ((LOWORD(lpth->thp) & 0xbfff) | 0x4000);
    fAnythingMoved = 0x1;
    dRange = (((LOWORD(lpth->thp) >> 0xa) & 0xf) + 0x4);
    dRange = LOWORD((dRange * dRange));
    if ((fPostProd == 0x0))
        goto L_1d3d;
    else
        goto L_1d37;

L_1d37:
    dRange = (dRange >> 0x1);

L_1d3d:
    ptDst.x = rgptPlan[(LOWORD(lpth->thp) & 0x3ff)].x;
    ptDst.y = rgptPlan[(LOWORD(lpth->thp) & 0x3ff)].y;

MoveTh:
    ptSrc.x = lpth->pt.x;
    ptSrc.y = lpth->pt.y;
    d = DGetDistance(ptSrc.x, ptSrc.y, ptDst.x, ptDst.y);
    dLeft = LOWORD(__ftol());
    if ((dLeft > dRange))
        goto L_2d93;
    else
        goto MadeItThere;

MadeItThere:
    if ((lpth->ith != ithMysteryTrader))
        goto L_1e91;
    else
        goto L_1db9;

L_1db9:
    lpth2 = lpThings;
    lpth2Mac = &(lpThings[cThing]);
    goto L_1e17;

L_1de4:
    if ((lpth2->ith != ithMysteryTrader))
        goto L_1e13;
    else
        goto L_1dfa;

L_1dfa:
    if ((LOWORD(lpth2) != LOWORD(lpth)))
        goto L_1e25;
    else
        goto L_1e08;

L_1e08:
    if ((HIWORD(lpth2) != HIWORD(lpth)))
        goto L_1e25;
    else
        goto L_1e0d;

L_1e0d:

L_1e13:
    lpth2 = (lpth2 + 0x1);

L_1e17:
    if ((LOWORD(lpth2) < LOWORD(lpth2Mac)))
        goto L_1de4;
    else
        goto L_1e25;

L_1e25:
    if ((LOWORD(lpth2) != LOWORD(lpth2Mac)))
        goto LFreeThePacket;
    else
        goto L_1e33;

L_1e33:
    if ((HIWORD(lpth2) != HIWORD(lpth2Mac)))
        goto LFreeThePacket;
    else
        goto L_1e3b;

L_1e3b:
    if ((Random(0x2) == 0x0))
        goto LFreeThePacket;
    else
        goto L_1e4c;

L_1e4c:

L_1e52:
    lpth->pt.x = lpth->tht.ptDest.x;
    lpth->pt.y = lpth->tht.ptDest.y;
    dRange = (lpth->tht.iWarp + 0xfffe);
    if ((dRange >= 0x6))
        goto L_1e86;
    else
        goto L_1e81;

L_1e81:
    dRange = 0x6;

L_1e86:
    idm = idmMysteryTraderHasDecidedMakeAnotherPass;
    goto LRetargetFreighter;

L_1e91:
    if ((lpth->ith != ithMineralPacket))
        goto L_1f03;
    else
        goto L_1ea7;

L_1ea7:
    pctRate = MulDiv(dLeft, 0x64, dRange);
    if ((pctRate >= 0x0))
        goto L_1eca;
    else
        goto L_1ec2;

L_1ec2:
    pctRate = 0x0;
    goto L_1ed8;

L_1eca:
    if ((pctRate <= 0x64))
        goto L_1ed8;
    else
        goto L_1ed3;

L_1ed3:
    pctRate = 0x64;

L_1ed8:
    if ((fPostProd == 0x0))
        goto L_1ee7;
    else
        goto L_1ee1;

L_1ee1:
    pctRate = (pctRate >> 0x1);

L_1ee7:
    if ((FPacketDecay(lpth, pctRate) != 0x0))
        goto LPacketAlreadyFreed;
    else
        goto L_1efd;

L_1efd:

L_1f03:
    lppl = &(lpPlanets[(*(lpth + 0x6) & 0x3ff)]);
    iWarpPacket = (((*(lpth + 0x6) >> 0xa) & 0xf) + 0x4);
    iWarp = IWarpMAFromLppl(lppl, &(fTwoMAs));
    if ((fTwoMAs == 0x0))
        goto L_1f59;
    else
        goto L_1f55;

L_1f55:
    iWarp = (iWarp + 0x1);

L_1f59:
    if ((iWarp <= 0x0))
        goto L_1fd7;
    else
        goto L_1f62;

L_1f62:
    if ((GetRaceStat(rgplr[lpth->iplr], rsMajorAdv) != raMassAccel))
        goto L_1fd7;
    else
        goto L_1f8f;

L_1f8f:
    rglpshdefSB[lppl->iPlayer][lppl->isb].grbitPlr = (rglpshdefSB[lppl->iPlayer][lppl->isb].grbitPlr | (0x1 << lpth->iplr));

L_1fd7:
    if ((GetRaceStat(rgplr[lpth->iplr], rsMajorAdv) != raMassAccel))
        goto L_200a;
    else
        goto L_2004;

L_2004:
    t_merge_200d_0001 = 0x1;
    goto L_200d;

L_200a:
    t_merge_200d_0001 = 0x0;

L_200d:
    fTerra = t_merge_200d_0001;
    iWarp2 = LOWORD((iWarp * iWarp));
    iWarpPacket2 = LOWORD((iWarpPacket * iWarpPacket));
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) != raStargate))
        goto L_2052;
    else
        goto L_2046;

L_2046:
    iWarp2 = ((uint32_t)(iWarp2) / 0x2);

L_2052:
    if ((iWarp2 < iWarpPacket2))
        goto L_2065;
    else
        goto L_205d;

L_205d:
    pctCaught = 0x3e8;
    goto L_209f;

L_2065:
    if ((iWarp <= 0x0))
        goto L_209a;
    else
        goto L_206e;

L_206e:
    iWarp = iWarp2;
    pctCaught = LOWORD((int32_t)(((uint32_t)(((uint32_t)(iWarp) * 0x3e8)) / (uint32_t)(iWarpPacket2))));
    goto L_209f;

L_209a:
    pctCaught = 0x0;

L_209f:
    pctMinKeep = (0x3e8 - pctCaught);
    i = 0x0;
    goto L_20f4;

L_20b0:
    /* untranslated: rgwtTerra[i] = loword((int32_t)(words(hiword((uint32_t)(words(signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]),
     * HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) * sext16to32(pctMinKeep))), loword((uint32_t)(words(signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) +
     * (i * 0x2))]), HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) * sext16to32(pctMinKeep)))) / 0x3e8)) */
    i = (i + 0x1);

L_20f4:
    if ((i < 0x3))
        goto L_20b0;
    else
        goto L_20fd;

L_20fd:
    pctMinKeep = (((uint32_t)((0x3e8 - pctCaught)) / 0x9) + pctCaught);
    wtTot = 0x0;
    i = 0x0;
    goto L_21ca;

L_2121:
    /* untranslated: branch HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] >= 0x0 ? L_2157 : L_213e */

L_213e:
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] = 0x0 */

L_2157:
    /* untranslated: LOWORD(wtTot) = (LOWORD(wtTot) + HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) */
    /* untranslated: HIWORD(wtTot) = (HIWORD(wtTot) + signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))])) */
    /* untranslated: LOWORD(lppl->rgwtMin[i]) = (LOWORD(lppl->rgwtMin[i]) +
     * loword((int32_t)(words(hiword((uint32_t)(words(signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]), HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i *
     * 0x2))]) * sext16to32(pctMinKeep))), loword((uint32_t)(words(signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]), HIWORD(lpth):[((LOWORD(lpth) +
     * 0x8) + (i * 0x2))]) * sext16to32(pctMinKeep)))) / 0x3e8))) */
    /* untranslated: HIWORD(lppl->rgwtMin[i]) = (HIWORD(lppl->rgwtMin[i]) +
     * hiword((int32_t)(words(hiword((uint32_t)(words(signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]), HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i *
     * 0x2))]) * sext16to32(pctMinKeep))), loword((uint32_t)(words(signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]), HIWORD(lpth):[((LOWORD(lpth) +
     * 0x8) + (i * 0x2))]) * sext16to32(pctMinKeep)))) / 0x3e8))) */
    i = (i + 0x1);

L_21ca:
    if ((i < 0x3))
        goto L_2121;
    else
        goto L_21d3;

L_21d3:
    if ((pctCaught == 0x3e8))
        goto LAllSafe;
    else
        goto L_21da;

L_21da:

L_21e0:
    dmgRaw = (int32_t)(((uint32_t)(((uint32_t)((LOWORD((iWarpPacket * iWarpPacket)) - iWarp)) * wtTot)) / 0xa0));
    if ((fTerra == 0x0))
        goto L_28db;
    else
        goto L_2215;

L_2215:
    iplr = lpth->iplr;
    i = 0x0;
    goto L_28d2;

L_222e:
    cTerraTemp = 0x0;
    cTerraPerm = 0x0;

L_2238:
    if ((rgwtTerra[i] <= 0x0))
        goto L_22b1;
    else
        goto L_224a;

L_224a:
    if ((rgwtTerra[i] >= 0x64))
        goto L_226b;
    else
        goto L_225c;

L_225c:
    t_merge_226e_0001 = rgwtTerra[i];
    goto L_226e;

L_226b:
    t_merge_226e_0001 = 0x64;

L_226e:
    wtCur = t_merge_226e_0001;
    if ((Random(0xc8) >= wtCur))
        goto L_22a1;
    else
        goto L_2285;

L_2285:
    cTerraTemp = (cTerraTemp + 0x1);
    if ((Random(0xa) != 0x0))
        goto L_22a1;
    else
        goto L_229d;

L_229d:
    cTerraPerm = (cTerraPerm + 0x1);

L_22a1:
    rgwtTerra[i] = (rgwtTerra[i] - 0x64);
    goto L_2238;

L_22b1:
    if ((cTerraPerm <= 0x0))
        goto L_25d5;
    else
        goto L_22ba;

L_22ba:
    if (((uint16_t)(rgplr[iplr].rgEnvVarMin[i]) >= 0x0))
        goto L_238c;
    else
        goto L_22da;

L_22da:
    if (((uint16_t)(lppl->rgEnvVarOrig[i]) >= 0x32))
        goto L_2340;
    else
        goto L_22f8;

L_22f8:
    if ((cTerraPerm >= ((uint16_t)(lppl->rgEnvVarOrig[i]) + 0xffff)))
        goto L_231f;
    else
        goto L_2319;

L_2319:
    t_merge_2338_0001 = cTerraPerm;
    goto L_2338;

L_231f:
    t_merge_2338_0001 = ((uint16_t)(lppl->rgEnvVarOrig[i]) + 0xffff);

L_2338:
    /* untranslated: cTerraPerm = (t_merge_2338_0001 neg 0x0) */
    goto L_2506;

L_2340:
    if ((cTerraPerm >= (0x63 - (uint16_t)(lppl->rgEnvVarOrig[i]))))
        goto L_2369;
    else
        goto L_2363;

L_2363:
    t_merge_2386_0001 = cTerraPerm;
    goto L_2386;

L_2369:
    t_merge_2386_0001 = (0x63 - (uint16_t)(lppl->rgEnvVarOrig[i]));

L_2386:
    cTerraPerm = t_merge_2386_0001;

L_238c:
    if (((uint16_t)(lppl->rgEnvVarOrig[i]) >= (uint16_t)(rgplr[iplr].rgEnvVar[i])))
        goto L_2441;
    else
        goto L_23c7;

L_23c7:
    if ((((uint16_t)(lppl->rgEnvVarOrig[i]) + cTerraPerm) <= (uint16_t)(rgplr[iplr].rgEnvVar[i])))
        goto L_2506;
    else
        goto L_2405;

L_2405:
    cTerraPerm = ((uint16_t)(rgplr[iplr].rgEnvVar[i]) - (uint16_t)(lppl->rgEnvVarOrig[i]));

L_2441:
    if (((uint16_t)(lppl->rgEnvVarOrig[i]) <= (uint16_t)(rgplr[iplr].rgEnvVar[i])))
        goto L_2501;
    else
        goto L_247c;

L_247c:
    if ((((uint16_t)(lppl->rgEnvVarOrig[i]) - cTerraPerm) >= (uint16_t)(rgplr[iplr].rgEnvVar[i])))
        goto L_24f6;
    else
        goto L_24ba;

L_24ba:
    cTerraPerm = ((uint16_t)(rgplr[iplr].rgEnvVar[i]) - (uint16_t)(lppl->rgEnvVarOrig[i]));
    goto L_2506;

L_24f6:
    /* untranslated: cTerraPerm = (cTerraPerm neg 0x0) */

L_2501:
    cTerraPerm = 0x0;

L_2506:
    if ((cTerraPerm == 0x0))
        goto L_25d5;
    else
        goto L_250f;

L_250f:
    abs(cTerraPerm);
    if ((cTerraPerm <= 0x0))
        goto L_253f;
    else
        goto L_2539;

L_2539:
    t_merge_2542_0001 = 0x1;
    goto L_2542;

L_253f:
    t_merge_2542_0001 = 0x0;

L_2542:
    /* untranslated: call FSendPlrMsg(iplr, 0x131, lppl->id, t_merge_2542_0001, i, lppl->id, callresult(int16_t), 0x0, 0x0, 0x0) -> callresult(int16_t) */
    if ((lppl->iPlayer == 0xffff))
        goto L_25bd;
    else
        goto L_2565;

L_2565:
    if ((lppl->iPlayer == iplr))
        goto L_25bd;
    else
        goto L_2574;

L_2574:
    abs(cTerraPerm);
    if ((cTerraPerm <= 0x0))
        goto L_25a4;
    else
        goto L_259e;

L_259e:
    t_merge_25a7_0001 = 0x1;
    goto L_25a7;

L_25a4:
    t_merge_25a7_0001 = 0x0;

L_25a7:
    /* untranslated: call FSendPlrMsg(iplr, 0x132, lppl->id, t_merge_25a7_0001, i, lppl->id, callresult(int16_t), 0x0, 0x0, 0x0) -> callresult(int16_t) */

L_25bd:
    lppl->rgEnvVarOrig[i] = (lppl->rgEnvVarOrig[i] + LOBYTE(cTerraPerm));

L_25d5:
    if ((cTerraTemp <= 0x0))
        goto L_28ce;
    else
        goto L_25de;

L_25de:
    idPlayer = iplr;
    if ((FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, 0x1) != 0x0))
        goto L_2613;
    else
        goto L_260a;

L_260a:
    idPlayer = 0xffff;
    goto L_28ce;

L_2613:
    idPlayer = 0xffff;
    if (((uint16_t)(rgplr[iplr].rgEnvVarMin[i]) >= 0x0))
        goto L_26f7;
    else
        goto L_2639;

L_2639:
    cTerraTemp = ((uint32_t)(cTerraTemp) / 0x2);
    if (((uint16_t)(lppl->rgEnvVarOrig[i]) >= 0x32))
        goto L_26ab;
    else
        goto L_2663;

L_2663:
    if ((cTerraTemp >= ((uint16_t)(lppl->rgEnvVar[i]) + 0xffff)))
        goto L_268a;
    else
        goto L_2684;

L_2684:
    t_merge_26a3_0001 = cTerraTemp;
    goto L_26a3;

L_268a:
    t_merge_26a3_0001 = ((uint16_t)(lppl->rgEnvVar[i]) + 0xffff);

L_26a3:
    /* untranslated: cTerraTemp = (t_merge_26a3_0001 neg 0x0) */
    goto L_27d5;

L_26ab:
    if ((cTerraTemp >= (0x63 - (uint16_t)(lppl->rgEnvVar[i]))))
        goto L_26d4;
    else
        goto L_26ce;

L_26ce:
    t_merge_26f1_0001 = cTerraTemp;
    goto L_26f1;

L_26d4:
    t_merge_26f1_0001 = (0x63 - (uint16_t)(lppl->rgEnvVar[i]));

L_26f1:
    cTerraTemp = t_merge_26f1_0001;

L_26f7:
    if ((rgMin[i] == 0xffff))
        goto L_2768;
    else
        goto L_2709;

L_2709:
    if ((cTerraTemp <= ((uint16_t)(lppl->rgEnvVar[i]) - rgMin[i])))
        goto L_275d;
    else
        goto L_2733;

L_2733:
    cTerraTemp = (rgMin[i] - (uint16_t)(lppl->rgEnvVar[i]));
    goto L_27d5;

L_275d:
    /* untranslated: cTerraTemp = (cTerraTemp neg 0x0) */

L_2768:
    if ((rgMax[i] == 0xffff))
        goto L_27d0;
    else
        goto L_277a;

L_277a:
    if ((cTerraTemp <= (rgMax[i] - (uint16_t)(lppl->rgEnvVar[i]))))
        goto L_27d5;
    else
        goto L_27a6;

L_27a6:
    cTerraTemp = (rgMax[i] - (uint16_t)(lppl->rgEnvVar[i]));

L_27d0:
    cTerraTemp = 0x0;

L_27d5:
    if ((cTerraTemp == 0x0))
        goto L_28ce;
    else
        goto L_27de;

L_27de:
    lppl->rgEnvVar[i] = (lppl->rgEnvVar[i] + LOBYTE(cTerraTemp));
    if ((cTerraTemp <= 0x0))
        goto L_283b;
    else
        goto L_2835;

L_2835:
    t_merge_283e_0001 = 0x1;
    goto L_283e;

L_283b:
    t_merge_283e_0001 = 0x0;

L_283e:
    FSendPlrMsg(iplr, 0x133, lppl->id, t_merge_283e_0001, i, lppl->id, ((i << 0x8) | (uint16_t)(lppl->rgEnvVar[i])), 0x0, 0x0, 0x0);
    if ((lppl->iPlayer == 0xffff))
        goto L_28ce;
    else
        goto L_2861;

L_2861:
    if ((lppl->iPlayer == iplr))
        goto L_28ce;
    else
        goto L_2870;

L_2870:
    if ((cTerraTemp <= 0x0))
        goto L_28b5;
    else
        goto L_28af;

L_28af:
    t_merge_28b8_0001 = 0x1;
    goto L_28b8;

L_28b5:
    t_merge_28b8_0001 = 0x0;

L_28b8:
    FSendPlrMsg(iplr, 0x134, lppl->id, t_merge_28b8_0001, i, lppl->id, ((i << 0x8) | (uint16_t)(lppl->rgEnvVar[i])), 0x0, 0x0, 0x0);

L_28ce:
    i = (i + 0x1);

L_28d2:
    if ((i < 0x3))
        goto L_222e;
    else
        goto L_28db;

L_28db:
    if ((lppl->iPlayer == 0xffff))
        goto LFreeThePacket;
    else
        goto L_28e5;

L_28e5:

L_28eb:
    CalcPctSurvive(lppl, &(pct), 0x0);
    dmgRaw = __ftol();
    if ((LOWORD(dmgRaw) != 0x0))
        goto L_292c;
    else
        goto L_2923;

L_2923:
    if ((HIWORD(dmgRaw) == 0x0))
        goto LAllSafe;
    else
        goto L_292c;

L_292c:
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh))
        goto L_29b8;
    else
        goto LAllSafe;

LAllSafe:
    if ((iWarp <= 0x0))
        goto L_29a2;
    else
        goto L_299c;

L_299c:
    t_merge_29a5_0001 = 0xd5;
    goto L_29a5;

L_29a2:
    t_merge_29a5_0001 = 0x146;

L_29a5:
    /* untranslated: call FSendPlrMsg(lppl->iPlayer, t_merge_29a5_0001, lppl->id, lppl->id, lpth->iplr, LOWORD(wtTot), (loword(callresult(uint32_t)) & 0xffff),
     * 0x0, 0x0, 0x0) -> callresult(int16_t) */
    goto LFreeThePacket;

L_29b8:
    lColKilled = lppl->rgwtMin[0x3];
    if ((LOWORD(lColKilled) != 0x0))
        goto L_29db;
    else
        goto L_29d2;

L_29d2:
    if ((HIWORD(lColKilled) == 0x0))
        goto L_2d1a;
    else
        goto L_29db;

L_29db:
    lColKilled = (int32_t)(((uint32_t)((lColKilled * dmgRaw)) / 0x3e8));
    if ((HIWORD(lColKilled) > HIWORD(dmgRaw)))
        goto L_2a28;
    else
        goto L_2a0f;

L_2a0f:
    if ((HIWORD(lColKilled) < HIWORD(dmgRaw)))
        goto L_2a1c;
    else
        goto L_2a14;

L_2a14:
    if ((LOWORD(lColKilled) >= LOWORD(dmgRaw)))
        goto L_2a28;
    else
        goto L_2a1c;

L_2a1c:
    lColKilled = dmgRaw;

L_2a28:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_2aba;
    else
        goto L_2a35;

L_2a35:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_2a44;
    else
        goto L_2a3a;

L_2a3a:
    if ((LOWORD(lppl->rgwtMin[0x3]) <= 0x0))
        goto L_2aba;
    else
        goto L_2a44;

L_2a44:
    if ((HIWORD(lColKilled) > HIWORD(lppl->rgwtMin[0x3])))
        goto L_2a7b;
    else
        goto L_2a57;

L_2a57:
    if ((HIWORD(lColKilled) < HIWORD(lppl->rgwtMin[0x3])))
        goto L_2a64;
    else
        goto L_2a5c;

L_2a5c:
    if ((LOWORD(lColKilled) >= LOWORD(lppl->rgwtMin[0x3])))
        goto L_2a7b;
    else
        goto L_2a64;

L_2a64:
    if ((HIWORD(lColKilled) > 0x0))
        goto L_2aba;
    else
        goto L_2a6d;

L_2a6d:
    if ((HIWORD(lColKilled) < 0x0))
        goto L_2a7b;
    else
        goto L_2a72;

L_2a72:
    if ((LOWORD(lColKilled) >= 0x0))
        goto L_2aba;
    else
        goto L_2a7b;

L_2a7b:
    FSendPlrMsg2(lppl->iPlayer, 0xda, lppl->id, lppl->id, lpth->iplr);
    UninhabitPlanet(lppl);
    goto LFreeThePacket;

L_2aba:
    /* untranslated: lDefKilled = (int32_t)((uint32_t)(words((*(lppl+0x1a) & 0x0), lppl->cDefenses) * dmgRaw) / 0x3e8) */
    if ((LOWORD(lDefKilled) != 0x0))
        goto L_2b53;
    else
        goto L_2af6;

L_2af6:
    if ((HIWORD(lDefKilled) != 0x0))
        goto L_2b53;
    else
        goto L_2aff;

L_2aff:
    if ((lppl->cDefenses != 0x0))
        goto L_2b20;
    else
        goto L_2b18;

L_2b18:
    if (((*(lppl + 0x1a) & 0x0) == 0x0))
        goto L_2b53;
    else
        goto L_2b20;

L_2b20:
    if ((SIGNHIWORD(Random(0x14)) > HIWORD(dmgRaw)))
        goto L_2b49;
    else
        goto L_2b35;

L_2b35:
    /* untranslated: branch signhiword(callresult(int16_t)) < HIWORD(dmgRaw) ? L_2b42 : L_2b3a */

L_2b3a:
    /* untranslated: branch callresult(int16_t) >= LOWORD(dmgRaw) ? L_2b49 : L_2b42 */

L_2b42:
    t_merge_2b4d_0001 = 0x1;
    goto L_2b4d;

L_2b49:
    t_merge_2b4d_0001 = 0x0;

L_2b4d:
    LOWORD(lDefKilled) = t_merge_2b4d_0001;
    HIWORD(lDefKilled) = 0x0;

L_2b53:
    if ((HIWORD(lDefKilled) > HIWORD((int32_t)((dmgRaw / 0x14)))))
        goto L_2b94;
    else
        goto L_2b6e;

L_2b6e:
    /* untranslated: branch HIWORD(lDefKilled) < hiword(callresult(int32_t)) ? L_2b7b : L_2b73 */

L_2b73:
    /* untranslated: branch LOWORD(lDefKilled) >= loword(callresult(int32_t)) ? L_2b94 : L_2b7b */

L_2b7b:
    lDefKilled = (int32_t)((dmgRaw / 0x14));

L_2b94:
    if ((HIWORD(lDefKilled) < (*(lppl + 0x1a) & 0x0)))
        goto L_2bd1;
    else
        goto L_2bad;

L_2bad:
    if ((HIWORD(lDefKilled) > (*(lppl + 0x1a) & 0x0)))
        goto L_2bba;
    else
        goto L_2bb2;

L_2bb2:
    if ((LOWORD(lDefKilled) <= lppl->cDefenses))
        goto L_2bd1;
    else
        goto L_2bba;

L_2bba:
    LOWORD(lDefKilled) = lppl->cDefenses;
    HIWORD(lDefKilled) = (*(lppl + 0x1a) & 0x0);

L_2bd1:
    if ((LOWORD(lDefKilled) != 0x0))
        goto L_2c53;
    else
        goto L_2bda;

L_2bda:
    if ((HIWORD(lDefKilled) != 0x0))
        goto L_2c53;
    else
        goto L_2be3;

L_2be3:
    if ((iWarp == 0x0))
        goto L_2bf2;
    else
        goto L_2bec;

L_2bec:
    t_merge_2bf5_0001 = 0xd6;
    goto L_2bf5;

L_2bf2:
    t_merge_2bf5_0001 = 0xd8;

L_2bf5:
    idm = t_merge_2bf5_0001;
    FSendPlrMsg(lppl->iPlayer, idm, lppl->id, lppl->id, LOWORD(wtTot), (LOWORD((uint32_t)((wtTot >> 0x10))) & 0xffff), lpth->iplr, LOWORD(lColKilled), 0x0,
                0x0);
    goto L_2d69;

L_2c53:
    if ((iWarp == 0x0))
        goto L_2c62;
    else
        goto L_2c5c;

L_2c5c:
    t_merge_2c65_0001 = 0xd7;
    goto L_2c65;

L_2c62:
    t_merge_2c65_0001 = 0xd9;

L_2c65:
    idm = t_merge_2c65_0001;
    FSendPlrMsg(lppl->iPlayer, idm, lppl->id, lppl->id, LOWORD(wtTot), (LOWORD((uint32_t)((wtTot >> 0x10))) & 0xffff), lpth->iplr, LOWORD(lColKilled),
                LOWORD(lDefKilled), 0x0);
    /* untranslated: ss:[bp-0x6a] = ((*(lppl+0x18) - LOWORD(lDefKilled)) & 0xfff) */
    /* untranslated: ss:[bp-0x68] = ((*(lppl+0x1a) - 0x0) & 0x0) */
    lppl->cDefenses = 0x0;
    *(lppl + 0x1a) = (*(lppl + 0x1a) & 0xffff);
    /* untranslated: *(lppl+0x18) = (*(lppl+0x18) | ss:[bp-0x6a]) */
    /* untranslated: *(lppl+0x1a) = (*(lppl+0x1a) | ss:[bp-0x68]) */

L_2d1a:
    FSendPlrMsg2(lppl->iPlayer, 0x181, lppl->id, lppl->id, lpth->iplr);
    LOWORD(lDefKilled) = lppl->cDefenses;
    HIWORD(lDefKilled) = (*(lppl + 0x1a) & 0x0);
    lColKilled = 0x0;

L_2d69:
    lppl->rgwtMin[0x3] = (lppl->rgwtMin[0x3] - lColKilled);

LFreeThePacket:
    FreeLpth(lpth);

LPacketAlreadyFreed:
    lpth = (lpth - 0x1);
    lpthMac = (lpthMac - 0x1);
    goto L_2ed3;

L_2d93:
    if ((ptDst.x <= ptSrc.x))
        goto L_2da8;
    else
        goto L_2d9e;

L_2d9e:
    t_merge_2daf_0001 = 0.5;
    goto L_2daf;

L_2da8:
    t_merge_2daf_0001 = -0.5;

L_2daf:
    dxRound = t_merge_2daf_0001;
    if ((ptDst.y <= ptSrc.y))
        goto L_2dca;
    else
        goto L_2dc0;

L_2dc0:
    t_merge_2dd1_0001 = 0.5;
    goto L_2dd1;

L_2dca:
    t_merge_2dd1_0001 = -0.5;

L_2dd1:
    dyRound = t_merge_2dd1_0001;
    __aFfcompp();
    /* untranslated: branch JA ? L_2e01 : L_2dec */

L_2dec:
    __aFfcompp();
    /* untranslated: branch JNC ? L_2e97 : L_2e01 */

L_2e01:
    /* untranslated: ss:[bp-0x54] = dRange */
    /* untranslated: ss:[bp-0x52] = signhiword(dRange) */
    /* untranslated: r = ((double)dword ss:[bp-0x54] / d) */
    /* untranslated: ss:[bp-0x54] = (ptDst.x - ptSrc.x) */
    /* untranslated: ss:[bp-0x52] = signhiword((ptDst.x - ptSrc.x)) */
    ptSrc.x = (LOWORD(__ftol()) + ptSrc.x);
    /* untranslated: ss:[bp-0x54] = (ptDst.y - ptSrc.y) */
    /* untranslated: ss:[bp-0x52] = signhiword((ptDst.y - ptSrc.y)) */
    ptSrc.y = (LOWORD(__ftol()) + ptSrc.y);
    if ((ptSrc.x != ptDst.x))
        goto L_2e86;
    else
        goto L_2e7b;

L_2e7b:
    if ((ptSrc.y == ptDst.y))
        goto MadeItThere;
    else
        goto L_2e80;

L_2e80:

L_2e86:
    lpth->pt.x = ptSrc.x;
    lpth->pt.y = ptSrc.y;

L_2e97:
    if ((fPostProd == 0x0))
        goto L_2ed3;
    else
        goto L_2ea0;

L_2ea0:
    if ((lpth->ith != ithMineralPacket))
        goto L_2ed3;
    else
        goto L_2eb6;

L_2eb6:
    if ((FPacketDecay(lpth, 0x32) != 0x0))
        goto LPacketAlreadyFreed;
    else
        goto L_2ecd;

L_2ecd:

L_2ed3:
    lpth = (lpth + 0x1);

L_2ed7:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_192d;
    else
        goto L_2ee5;

L_2ee5:
    if ((fAnythingMoved == 0x0))
        goto L_2ef3;
    else
        goto L_2eee;

L_2eee:
    ValidateWaypoints();

L_2ef3:
    return;
}

void FuelFleets() {
    int16_t  j;
    int32_t  cPods;
    PLANET  *lppl;
    int16_t  i;
    int16_t  ifl;
    FLEET   *lpfl;
    SHDEF   *lpshdef;
    int32_t  csh;
    HUL     *lphul;
    uint16_t t_merge_32b9_0001;
    uint16_t t_merge_32b9_0002;

L_2efa:
    ifl = 0x0;
    goto L_2f0f;

L_2f0b:
    ifl = (ifl + 0x1);

L_2f0f:
    if ((ifl >= cFleet))
        goto L_32c7;
    else
        goto L_2f1a;

L_2f1a:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_2f4a;
    else
        goto L_2f42;

L_2f42:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_32c7;
    else
        goto L_2f4a;

L_2f4a:
    if ((lpfl->fDead != 0x0))
        goto L_2f0b;
    else
        goto L_2f5e;

L_2f5e:

L_2f64:
    if ((lpfl->idPlanet == 0xffff))
        goto LChkFuelTransport;
    else
        goto L_2f71;

L_2f71:
    /* untranslated: branch ((part[4:2](lpPlanets[lpfl->idPlanet]) >> 0x9) & 0x1) == 0x0 ? L_306e : L_2f98 */

L_2f98:

L_2f9e:
    lppl = &(lpPlanets[lpfl->idPlanet]);
    if ((lppl->iPlayer == 0xffff))
        goto LChkFuelTransport;
    else
        goto L_2fc2;

L_2fc2:

L_2fc8:
    if ((lpfl->iPlayer == lppl->iPlayer))
        goto L_300c;
    else
        goto L_2fdb;

L_2fdb:
    /* untranslated: branch sext8to16(byte 0x25:[(part[112:0](rgplr[lppl->iPlayer]) + lpfl->iPlayer)]) != 0x1 ? L_306e : L_3006 */

L_3006:

L_300c:
    if ((LphuldefFromId(rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef)->hul.wtCargoMax == 0x0))
        goto LChkFuelTransport;
    else
        goto L_3048;

L_3048:

L_304e:
    lpfl->rgwtMin[0x4] = LGetFleetStat(lpfl, 0x1);
    goto L_2f0b;

LChkFuelTransport:
    csh = 0x0;
    cPods = 0x0;
    i = 0x0;
    goto L_31e6;

L_308a:
    if ((lpfl->rgcsh[i] == 0x0))
        goto L_31e2;
    else
        goto L_30a4;

L_30a4:

L_30aa:
    lphul = &(rglpshdef[lpfl->iPlayer][i]);
    j = (lphul->chs + 0xffff);
    goto L_3173;

L_30de:
    if ((lphul->rghs[j].grhst != hstSpecialE))
        goto L_316f;
    else
        goto L_30fe;

L_30fe:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) != 0x10 ? L_316f : L_3123 */

L_3123:
    /* untranslated: cPods = (cPods + (uint32_t)(sext16to32(lpfl->rgcsh[i]) * words(0x0, ((lphul->rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff)))) */

L_316f:
    j = (j - 0x1);

L_3173:
    if ((j >= 0x0))
        goto L_30de;
    else
        goto L_317c;

L_317c:
    lpshdef = &(rglpshdef[lpfl->iPlayer][i]);
    if ((lpshdef->hul.ihuldef == ihuldefFuelTransport))
        goto L_31b5;
    else
        goto L_31a9;

L_31a9:
    if ((lpshdef->hul.ihuldef != ihuldefSuperFuelXport))
        goto L_31e2;
    else
        goto L_31b5;

L_31b5:
    csh = (csh + (uint32_t)(((uint32_t)(lpfl->rgcsh[i]) * 0xc8)));

L_31e2:
    i = (i + 0x1);

L_31e6:
    if ((i < 0x10))
        goto L_308a;
    else
        goto L_31ef;

L_31ef:
    if ((LOWORD(csh) != 0x0))
        goto L_3213;
    else
        goto L_31f8;

L_31f8:
    if ((HIWORD(csh) != 0x0))
        goto L_3213;
    else
        goto L_3201;

L_3201:
    if ((LOWORD(cPods) != 0x0))
        goto L_3213;
    else
        goto L_320a;

L_320a:
    if ((HIWORD(cPods) == 0x0))
        goto L_2f0b;
    else
        goto L_3213;

L_3213:
    if ((HIWORD(LGetFleetStat(lpfl, 0x1)) > ((HIWORD(lpfl->rgwtMin[0x4]) + HIWORD(csh)) + HIWORD((uint32_t)((cPods * 0x32))))))
        goto L_3281;
    else
        goto L_3260;

L_3260:
    /* untranslated: branch ss:[bp-0x22] < ((HIWORD(lpfl->rgwtMin[0x4]) + HIWORD(csh)) + hiword(callresult(uint32_t))) ? L_326c : L_3265 */

L_3265:
    /* untranslated: branch ss:[bp-0x24] >= ((LOWORD(lpfl->rgwtMin[0x4]) + LOWORD(csh)) + loword(callresult(uint32_t))) ? L_3281 : L_326c */

L_326c:
    LGetFleetStat(lpfl, 0x1);
    /* untranslated: t_merge_32b9_0001 = loword(callresult(int32_t)) */
    /* untranslated: t_merge_32b9_0002 = hiword(callresult(int32_t)) */
    goto L_32b9;

L_3281:
    /* untranslated: ss:[bp-0x28] = loword((uint32_t)(cPods * 0x32)) */
    /* untranslated: ss:[bp-0x26] = hiword((uint32_t)(cPods * 0x32)) */
    /* untranslated: t_merge_32b9_0001 = ((LOWORD(lpfl->rgwtMin[0x4]) + LOWORD(csh)) + ss:[bp-0x28]) */
    /* untranslated: t_merge_32b9_0002 = ((HIWORD(lpfl->rgwtMin[0x4]) + HIWORD(csh)) + ss:[bp-0x26]) */

L_32b9:
    LOWORD(lpfl->rgwtMin[0x4]) = t_merge_32b9_0001;
    HIWORD(lpfl->rgwtMin[0x4]) = t_merge_32b9_0002;

L_32c7:
    return;
}

void MoveFleets() {
    int32_t  dTravel;
    int16_t  cPass;
    int32_t  wtFuel2Dest;
    double   d;
    int16_t  fGotEnufFuel;
    int16_t  fRanOutOfFuel;
    ORDER   *lpord;
    POINT    ptEnd;
    int16_t  ifl;
    FLEET   *lpfl;
    double   r;
    int32_t  pct;
    int16_t  dMineTravel;
    int32_t  dRange;
    POINT    ptBeg;
    int32_t  wtFuelUsed;
    int32_t  dActTravel;
    int32_t  lFuelGain;
    int16_t  fDone;
    SCAN     scan;
    PLANET  *lpplDst;
    int32_t  wtColonists;
    int16_t  i;
    PLANET  *lpplSrc;
    int16_t  fJumpgate;
    int16_t  isbsDst;
    int16_t  isbsSrc;
    POINT    ptMsg;
    int32_t  wtMinerals;
    int32_t  cDie;
    int16_t  cKill;
    int16_t  ish;
    FLEET    flSrc;
    int16_t  cTry;
    FLEET    flDead;
    int16_t  cKillTot;
    int16_t  fDead;
    int16_t  dy;
    int16_t  dx;
    int32_t  lFuelGainAct;
    double   dyRound;
    double   dxRound;
    int16_t  iCtr;
    THING   *lpthDest;
    THING   *lpth;
    int16_t  grbitPlr;
    uint16_t t_merge_3de5_0001;
    uint16_t t_merge_3e66_0001;
    uint16_t t_merge_3e66_0002;
    int16_t  t_40c6;
    uint32_t t_merge_436d_0001_wide;
    uint16_t t_merge_4403_0001;
    uint16_t t_merge_4403_0002;
    uint32_t t_merge_450b_0001_wide;
    uint16_t t_merge_47c3_0001;
    uint32_t t_merge_48a1_0001;
    float    t_merge_4a09_0001;
    float    t_merge_4a2b_0001;
    uint32_t t_merge_4c51_0003_wide;
    int32_t  t_merge_4ca3_0001_wide;
    uint16_t t_merge_4e4b_0001;
    int16_t  t_4f1e;

L_32ce:
    cPass = 0x0;
    if ((cFleet <= 0x0))
        goto L_4f35;
    else
        goto L_32e3;

L_32e3:

MoveUnfinishedFleets:
    fDone = 0x1;
    ifl = 0x0;
    goto L_32fa;

L_32f6:
    ifl = (ifl + 0x1);

L_32fa:
    if ((ifl >= cFleet))
        goto L_4f15;
    else
        goto L_3305;

L_3305:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_3335;
    else
        goto L_332d;

L_332d:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_4f15;
    else
        goto L_3335;

L_3335:
    if ((cPass != 0x0))
        goto L_3361;
    else
        goto L_333e;

L_333e:
    lpfl->dirLong = 0x0;
    lpfl->fHereAllTurn = 0x1;

L_3361:
    if ((lpfl->fDead != 0x0))
        goto L_32f6;
    else
        goto L_3378;

L_3378:
    if ((cPass <= 0x0))
        goto L_339b;
    else
        goto L_3381;

L_3381:
    if ((lpfl->fDone != 0x0))
        goto L_32f6;
    else
        goto L_3395;

L_3395:

L_339b:
    lpfl->fDone = 0x1;
    lpord = &(lpfl->lpplord->rgord);
    if ((lpord->grTask == grTaskXfer))
        goto L_32f6;
    else
        goto L_33d7;

L_33d7:
    if ((lpord->grTask == grTaskLayMines))
        goto L_32f6;
    else
        goto L_33e6;

L_33e6:

L_33ec:
    if ((lpfl->cord <= 0x1))
        goto L_32f6;
    else
        goto L_33f9;

L_33f9:
    if ((((*(lpord + 0x18) >> 0x4) & 0xf) == 0x0))
        goto L_32f6;
    else
        goto L_3410;

L_3410:

L_3416:
    /* untranslated: branch ((part[84:2](rgplr[lpfl->iPlayer]) >> 0x2) & 0x1) == 0x0 ? L_3496 : L_3437 */

L_3437:
    if ((game.turn <= 0xa))
        goto L_345e;
    else
        goto L_3441;

L_3441:
    if (((game.turn & 0x7) == (lpfl->iPlayer & 0x7)))
        goto L_32f6;
    else
        goto L_3455;

L_3455:

L_345e:
    if ((Random(0x4) != 0x0))
        goto L_3496;
    else
        goto L_3472;

L_3472:
    FSendPlrMsg2(lpfl->iPlayer, 0x102, 0xfffb, LOWORD(lpfl), 0x0);
    goto L_32f6;

L_3496:
    if ((cPass != 0x0))
        goto L_3535;
    else
        goto L_349f;

L_349f:
    if ((((*(lpord + 0x18) >> 0x4) & 0xf) <= 0x6))
        goto L_3535;
    else
        goto L_34b9;

L_34b9:
    if ((((*(lpord + 0x18) >> 0x4) & 0xf) == 0xb))
        goto L_3535;
    else
        goto L_34d3;

L_34d3:
    if ((GetRaceGrbit(rgplr[lpfl->iPlayer], ibitRaceCheapEngines) == 0x0))
        goto L_3535;
    else
        goto L_34f7;

L_34f7:
    if ((Random(0xa) != 0x0))
        goto L_3535;
    else
        goto L_350b;

L_350b:
    FSendPlrMsg2(lpfl->iPlayer, 0xf2, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), 0x0);
    goto L_32f6;

L_3535:
    if ((((*(lpord + 0x18) >> 0x4) & 0xf) < 0xb))
        goto L_3d10;
    else
        goto L_354f;

L_354f:
    fJumpgate = 0x0;
    gd.fRadiatingEngine = 0x0;
    ptMsg.x = lpord->pt.x;
    ptMsg.y = lpord->pt.y;
    ptBeg.x = lpord->pt.x;
    ptBeg.y = lpord->pt.y;
    if ((lpord->grobj != grobjPlanet))
        goto L_35c5;
    else
        goto L_358d;

L_358d:
    ptMsg.x = 0xffff;
    ptMsg.y = lpord->id;
    lpplSrc = LpplFromId(lpord->id);
    isbsSrc = IStargateFromLppl(lpplSrc);
    goto L_35ca;

L_35c5:
    isbsSrc = 0xffff;

L_35ca:
    if ((isbsSrc != 0xffff))
        goto L_3628;
    else
        goto L_35d3;

L_35d3:
    if ((FFleetCanJumpgate(lpfl) != 0x0))
        goto LNoGateNeeded;
    else
        goto L_35e6;

L_35e6:

L_35ec:
    FSendPlrMsg(lpfl->iPlayer, 0xde, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), ptMsg.x, ptMsg.y, 0x0, 0x0, 0x0, 0x0);
    goto L_32f6;

L_3628:
    if ((lpplSrc->iPlayer == lpfl->iPlayer))
        goto LNoGateNeeded;
    else
        goto L_363b;

L_363b:
    /* untranslated: branch sext8to16(byte 0x25:[(part[112:0](rgplr[lpplSrc->iPlayer]) + lpfl->iPlayer)]) == 0x1 ? L_36ab : L_3669 */

L_3669:
    FSendPlrMsg(lpfl->iPlayer, 0xe6, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), lpplSrc->id, lpplSrc->id, 0x0, 0x0, 0x0, 0x0);
    goto L_32f6;

LNoGateNeeded:
    ptMsg.x = lpord[0x1].pt.x;
    ptMsg.y = *(lpord + 0x14);
    ptEnd.x = lpord[0x1].pt.x;
    ptEnd.y = *(lpord + 0x14);
    if ((((*(lpord + 0x18) >> 0x8) & 0xf) != 0x1))
        goto L_3711;
    else
        goto L_36d9;

L_36d9:
    ptMsg.x = 0xffff;
    ptMsg.y = *(lpord + 0x16);
    lpplDst = LpplFromId(*(lpord + 0x16));
    isbsDst = IStargateFromLppl(lpplDst);
    goto L_37c4;

L_3711:
    i = 0x0;
    goto L_371d;

L_3719:
    i = (i + 0x1);

L_371d:
    if ((i >= game.cPlanMax))
        goto L_374d;
    else
        goto L_3728;

L_3728:
    if ((ptEnd.x != rgptPlan[i].x))
        goto L_3719;
    else
        goto L_373f;

L_373f:
    if ((ptEnd.y == rgptPlan[i].y))
        goto L_374d;
    else
        goto L_3744;

L_3744:

L_374d:
    if ((i >= game.cPlanMax))
        goto L_3788;
    else
        goto L_3758;

L_3758:
    ptMsg.x = 0xffff;
    ptMsg.y = i;
    lpplDst = LpplFromId(i);
    isbsDst = IStargateFromLppl(lpplDst);
    goto L_37c4;

L_3788:
    FSendPlrMsg(lpfl->iPlayer, 0x147, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), ptEnd.x, ptEnd.y, 0x0, 0x0, 0x0, 0x0);
    goto L_32f6;

L_37c4:
    if ((isbsDst != 0xffff))
        goto L_380b;
    else
        goto L_37cd;

L_37cd:
    FSendPlrMsg(lpfl->iPlayer, 0xe2, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), lpplDst->id, ptMsg.x, ptMsg.y, 0x0, 0x0, 0x0);
    goto L_32f6;

L_380b:
    if ((lpplDst->iPlayer == lpfl->iPlayer))
        goto L_3890;
    else
        goto L_381e;

L_381e:
    /* untranslated: branch sext8to16(byte 0x25:[(part[112:0](rgplr[lpplDst->iPlayer]) + lpfl->iPlayer)]) == 0x1 ? L_3890 : L_384c */

L_384c:
    FSendPlrMsg(lpfl->iPlayer, 0xe5, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), lpplDst->id, lpplDst->id, lpplDst->id, 0x0, 0x0, 0x0);
    goto L_32f6;

L_3890:
    if ((isbsSrc != 0xffff))
        goto L_38a4;
    else
        goto L_3899;

L_3899:
    fJumpgate = 0x1;
    isbsSrc = isbsDst;

L_38a4:
    if ((fJumpgate != 0x0))
        goto L_3c9f;
    else
        goto L_38ad;

L_38ad:
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) == raStargate))
        goto L_3c9f;
    else
        goto L_38d1;

L_38d1:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < 0x0))
        goto L_392c;
    else
        goto L_38de;

L_38de:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > 0x0))
        goto L_38ed;
    else
        goto L_38e3;

L_38e3:
    if ((LOWORD(lpfl->rgwtMin[0x3]) <= 0x0))
        goto L_392c;
    else
        goto L_38ed;

L_38ed:
    if ((lpplSrc->iPlayer == lpfl->iPlayer))
        goto L_392c;
    else
        goto L_3900;

L_3900:
    FSendPlrMsg2(lpfl->iPlayer, 0x15e, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), lpplSrc->id);
    goto L_32f6;

L_392c:
    wtMinerals = 0x0;
    i = 0x0;
    goto L_39e9;

L_393e:
    if ((LOWORD(lpfl->rgwtMin[i]) != 0x0))
        goto L_3967;
    else
        goto L_395d;

L_395d:
    if ((HIWORD(lpfl->rgwtMin[i]) == 0x0))
        goto L_39e5;
    else
        goto L_3967;

L_3967:
    wtMinerals = (wtMinerals + lpfl->rgwtMin[i]);
    lpplSrc->rgwtMin[i] = (lpplSrc->rgwtMin[i] + lpfl->rgwtMin[i]);
    lpfl->rgwtMin[i] = 0x0;

L_39e5:
    i = (i + 0x1);

L_39e9:
    if ((i <= 0x2))
        goto L_393e;
    else
        goto L_39f2;

L_39f2:
    wtColonists = lpfl->rgwtMin[0x3];
    lpplSrc->rgwtMin[0x3] = (lpplSrc->rgwtMin[0x3] + lpfl->rgwtMin[0x3]);
    lpfl->rgwtMin[0x3] = 0x0;
    if ((LOWORD(wtColonists) != 0x0))
        goto L_3a3a;
    else
        goto L_3a31;

L_3a31:
    if ((HIWORD(wtColonists) == 0x0))
        goto L_3bdc;
    else
        goto L_3a3a;

L_3a3a:
    if ((LOWORD(wtMinerals) != 0x0))
        goto L_3a4c;
    else
        goto L_3a43;

L_3a43:
    if ((HIWORD(wtMinerals) == 0x0))
        goto L_3b28;
    else
        goto L_3a4c;

L_3a4c:
    FSendPlrMsg(lpfl->iPlayer, 0xee, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), LOWORD(wtColonists), (LOWORD((uint32_t)((wtColonists >> 0x10))) & 0xffff),
                LOWORD(wtMinerals), (LOWORD((uint32_t)((wtMinerals >> 0x10))) & 0xffff), lpplSrc->id, 0x0);
    if ((lpfl->iPlayer == lpplSrc->iPlayer))
        goto L_3c9f;
    else
        goto L_3ac4;

L_3ac4:
    FSendPlrMsg(lpplSrc->iPlayer, 0xee, lpplSrc->id, LOWORD(lpfl), LOWORD(wtColonists), (LOWORD((uint32_t)((wtColonists >> 0x10))) & 0xffff),
                LOWORD(wtMinerals), (LOWORD((uint32_t)((wtMinerals >> 0x10))) & 0xffff), lpplSrc->id, 0x0);

L_3b28:
    FSendPlrMsg(lpfl->iPlayer, 0xed, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), LOWORD(wtColonists), (LOWORD((uint32_t)((wtColonists >> 0x10))) & 0xffff),
                lpplSrc->id, 0x0, 0x0, 0x0);
    if ((lpfl->iPlayer == lpplSrc->iPlayer))
        goto L_3c9f;
    else
        goto L_3b8c;

L_3b8c:
    FSendPlrMsg(lpplSrc->iPlayer, 0xed, lpplSrc->id, LOWORD(lpfl), LOWORD(wtColonists), (LOWORD((uint32_t)((wtColonists >> 0x10))) & 0xffff), lpplSrc->id, 0x0,
                0x0, 0x0);

L_3bdc:
    if ((LOWORD(wtMinerals) != 0x0))
        goto L_3bee;
    else
        goto L_3be5;

L_3be5:
    if ((HIWORD(wtMinerals) == 0x0))
        goto L_3c9f;
    else
        goto L_3bee;

L_3bee:
    FSendPlrMsg(lpfl->iPlayer, 0xec, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), LOWORD(wtMinerals), (LOWORD((uint32_t)((wtMinerals >> 0x10))) & 0xffff),
                lpplSrc->id, 0x0, 0x0, 0x0);
    if ((lpfl->iPlayer == lpplSrc->iPlayer))
        goto L_3c9f;
    else
        goto L_3c52;

L_3c52:
    FSendPlrMsg(lpplSrc->iPlayer, 0xec, lpplSrc->id, LOWORD(lpfl), LOWORD(wtMinerals), (LOWORD((uint32_t)((wtMinerals >> 0x10))) & 0xffff), lpplSrc->id, 0x0,
                0x0, 0x0);

L_3c9f:
    DGetDistance(ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
    dTravel = __ftol();
    if ((FStargateJump(lpfl, isbsSrc, isbsDst, LOWORD(dTravel)) == 0x0))
        goto L_32f6;
    else
        goto L_3ce5;

L_3ce5:

L_3ceb:
    lpfl->fHereAllTurn = 0x0;
    NoAutoTrackFleet(lpfl);
    goto LMakeItToDest;

L_3d10:
    ptBeg.x = lpfl->pt.x;
    ptBeg.y = lpfl->pt.y;
    if ((cPass <= 0x0))
        goto L_3d5b;
    else
        goto L_3d2a;

L_3d2a:
    if ((((*(lpord + 0x18) >> 0xd) & 0x1) != 0x0))
        goto L_3d5b;
    else
        goto L_3d41;

L_3d41:
    lpord[0x1].pt.x = lpfl->lpflNext->pt.x;
    *(lpord + 0x14) = lpfl->lpflNext->pt.y;

L_3d5b:
    ptEnd.x = lpord[0x1].pt.x;
    ptEnd.y = *(lpord + 0x14);
    dRange = EstFuelUse(lpfl, 0x0, 0xffff, 0xffffffff, 0x1);
    wtFuel2Dest = EstFuelUse(lpfl, 0x0, 0xffff, 0xffffffff, 0x0);
    if ((HIWORD(wtFuel2Dest) > HIWORD(lpfl->rgwtMin[0x4])))
        goto L_3de2;
    else
        goto L_3dcf;

L_3dcf:
    if ((HIWORD(wtFuel2Dest) < HIWORD(lpfl->rgwtMin[0x4])))
        goto L_3ddc;
    else
        goto L_3dd4;

L_3dd4:
    if ((LOWORD(wtFuel2Dest) > LOWORD(lpfl->rgwtMin[0x4])))
        goto L_3de2;
    else
        goto L_3ddc;

L_3ddc:
    t_merge_3de5_0001 = 0x1;
    goto L_3de5;

L_3de2:
    t_merge_3de5_0001 = 0x0;

L_3de5:
    fGotEnufFuel = t_merge_3de5_0001;
    fRanOutOfFuel = 0x0;
    if ((fGotEnufFuel == 0x0))
        goto L_3e6c;
    else
        goto L_3df6;

L_3df6:
    if ((HIWORD(dRange) < 0x0))
        goto L_3e3d;
    else
        goto L_3e27;

L_3e27:
    if ((HIWORD(dRange) > 0x0))
        goto L_3e34;
    else
        goto L_3e2c;

L_3e2c:
    if ((LOWORD(dRange) <= LOWORD((((*(lpord + 0x18) >> 0x4) & 0xf) * ((*(lpord + 0x18) >> 0x4) & 0xf)))))
        goto L_3e3d;
    else
        goto L_3e34;

L_3e34:
    t_merge_3e66_0001 = LOWORD(dRange);
    t_merge_3e66_0002 = HIWORD(dRange);
    goto L_3e66;

L_3e3d:
    t_merge_3e66_0001 = LOWORD((((*(lpord + 0x18) >> 0x4) & 0xf) * ((*(lpord + 0x18) >> 0x4) & 0xf)));
    t_merge_3e66_0002 = 0x0;

L_3e66:
    LOWORD(dRange) = t_merge_3e66_0001;
    HIWORD(dRange) = t_merge_3e66_0002;

L_3e6c:
    if ((cPass != 0x0))
        goto L_42fb;
    else
        goto L_3e75;

L_3e75:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < 0x0))
        goto L_3f5f;
    else
        goto L_3e82;

L_3e82:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > 0x0))
        goto L_3e91;
    else
        goto L_3e87;

L_3e87:
    if ((LOWORD(lpfl->rgwtMin[0x3]) <= 0xa))
        goto L_3f5f;
    else
        goto L_3e91;

L_3e91:
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) != raMacintosh))
        goto L_3f5f;
    else
        goto L_3eb5;

L_3eb5:
    /* untranslated: cDie = (int32_t)(words((hiword((uint32_t)(lpfl->rgwtMin[0x3] * 0x3)) + 0x0), (loword((uint32_t)(lpfl->rgwtMin[0x3] * 0x3)) + 0x21)) / 0x64)
     */
    if ((HIWORD(cDie) < 0x0))
        goto L_3f5f;
    else
        goto L_3ef1;

L_3ef1:
    if ((HIWORD(cDie) > 0x0))
        goto L_3eff;
    else
        goto L_3ef6;

L_3ef6:
    if ((LOWORD(cDie) <= 0x0))
        goto L_3f5f;
    else
        goto L_3eff;

L_3eff:
    lpfl->rgwtMin[0x3] = (lpfl->rgwtMin[0x3] - cDie);
    FSendPlrMsg(lpfl->iPlayer, 0xc1, (LOWORD(lpfl) | 0x8000), LOWORD(cDie), (LOWORD((uint32_t)((cDie >> 0x10))) & 0xffff), LOWORD(lpfl), 0x0, 0x0, 0x0, 0x0);

L_3f5f:
    if ((((*(lpord + 0x18) >> 0x4) & 0xf) != 0xa))
        goto L_4240;
    else
        goto L_3f79;

L_3f79:
    flSrc = *(lpfl);
    fDead = 0x1;
    cKillTot = 0x0;
    memset(flDead, 0x0, 0x7c);
    ish = 0x0;
    goto L_413f;

L_3fbb:
    if ((flSrc.rgcsh[ish] == 0x0))
        goto L_413b;
    else
        goto L_3fcb;

L_3fcb:

L_3fd1:
    if (((HIWORD(rglpshdef[lpfl->iPlayer][ish].hul.rghs[0x0]) & 0xff) == 0x7))
        goto LWarp10Kill;
    else
        goto L_3ffd;

L_3ffd:
    if (((HIWORD(rglpshdef[lpfl->iPlayer][ish].hul.rghs[0x0]) & 0xff) == 0x9))
        goto LWarp10Kill;
    else
        goto L_4029;

L_4029:
    if (((HIWORD(rglpshdef[lpfl->iPlayer][ish].hul.rghs[0x0]) & 0xff) == 0xe))
        goto LWarp10Kill;
    else
        goto L_4055;

L_4055:
    if (((HIWORD(rglpshdef[lpfl->iPlayer][ish].hul.rghs[0x0]) & 0xff) == 0xf))
        goto LWarp10Kill;
    else
        goto L_4081;

L_4081:
    if (((HIWORD(rglpshdef[lpfl->iPlayer][ish].hul.rghs[0x0]) & 0xff) == 0x8))
        goto LWarp10Kill;
    else
        goto L_40aa;

L_40aa:

L_40b0:
    cKill = 0x0;
    cTry = flSrc.rgcsh[ish];

L_40c6:
    t_40c6 = cTry;
    cTry = (cTry - 0x1);
    if ((t_40c6 == 0x0))
        goto L_40f2;
    else
        goto L_40d7;

L_40d7:
    if ((Random(0xa) != 0x0))
        goto L_40c6;
    else
        goto L_40eb;

L_40eb:
    cKill = (cKill + 0x1);

L_40f2:
    if ((cKill <= 0x0))
        goto LWarp10Kill;
    else
        goto L_40fb;

L_40fb:
    cKillTot = (cKillTot + cKill);
    flSrc.rgcsh[ish] = (flSrc.rgcsh[ish] - cKill);
    flDead.rgcsh[ish] = cKill;

LWarp10Kill:
    if ((flSrc.rgcsh[ish] <= 0x0))
        goto L_413b;
    else
        goto L_4135;

L_4135:
    fDead = 0x0;

L_413b:
    ish = (ish + 0x1);

L_413f:
    if ((ish < 0x10))
        goto L_3fbb;
    else
        goto L_4148;

L_4148:
    if ((fDead == 0x0))
        goto L_4193;
    else
        goto L_4152;

L_4152:
    lpfl->fDead = 0x1;
    FSendPlrMsg2(lpfl->iPlayer, 0xe1, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), 0x0);
    goto L_32f6;

L_4193:
    if ((cKillTot <= 0x0))
        goto L_4240;
    else
        goto L_419d;

L_419d:
    flDead.iPlayer = flSrc.iPlayer;
    flDead.fDead = 0x1;
    flDead.det = 0x7;
    FleetTransferCargoBalance(&(flSrc), &(flDead));
    *(lpfl) = flSrc;
    if ((cKillTot != 0x1))
        goto L_4219;
    else
        goto L_41ef;

L_41ef:
    FSendPlrMsg2(lpfl->iPlayer, 0xdf, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), 0x0);
    goto L_4240;

L_4219:
    FSendPlrMsg2(lpfl->iPlayer, 0xe0, (LOWORD(lpfl) | 0x8000), cKillTot, LOWORD(lpfl));

L_4240:
    LOWORD(dTravel) = LOWORD((((*(lpord + 0x18) >> 0x4) & 0xf) * ((*(lpord + 0x18) >> 0x4) & 0xf)));
    HIWORD(dTravel) = 0x0;
    if ((((*(lpord + 0x18) >> 0x8) & 0xf) != 0x2))
        goto L_43a2;
    else
        goto L_4286;

L_4286:
    lpfl->lpflNext = LpflFromId(*(lpord + 0x16));
    if ((LOWORD(lpfl->lpflNext) != 0x0))
        goto L_42b7;
    else
        goto L_42ad;

L_42ad:
    if ((HIWORD(lpfl->lpflNext) == 0x0))
        goto L_43a2;
    else
        goto L_42b7;

L_42b7:
    fDone = 0x0;
    lpfl->fDone = 0x0;
    lpfl->dMoveLeft = LOWORD(dTravel);
    lpfl->dMoveUsed = 0x0;
    lpfl->lFuelUsed = 0x0;
    goto L_32f6;

L_42fb:
    /* untranslated: branch ((part[4:2](lpfl->lpflNext) >> 0xb) & 0x1) == 0x0 ? L_4327 : L_4316 */

L_4316:
    dTravel = (uint32_t)(lpfl->dMoveLeft);
    goto L_4373;

L_4327:
    if ((lpfl->dMoveLeft >= ((uint32_t)(((lpfl->dMoveLeft + lpfl->dMoveUsed) + 0x4)) / 0x5)))
        goto L_4355;
    else
        goto L_434a;

L_434a:
    t_merge_436d_0001_wide = (uint32_t)(lpfl->dMoveLeft);
    goto L_436d;

L_4355:
    t_merge_436d_0001_wide = (uint32_t)(((uint32_t)(((lpfl->dMoveLeft + lpfl->dMoveUsed) + 0x4)) / 0x5));

L_436d:
    dTravel = t_merge_436d_0001_wide;

L_4373:
    dRange = (dRange - (uint32_t)(lpfl->dMoveUsed));
    if ((HIWORD(dRange) > 0x0))
        goto L_43a2;
    else
        goto L_438a;

L_438a:
    if ((HIWORD(dRange) < 0x0))
        goto L_4398;
    else
        goto L_438f;

L_438f:
    if ((LOWORD(dRange) >= 0x0))
        goto L_43a2;
    else
        goto L_4398;

L_4398:
    dRange = 0x0;

L_43a2:
    d = DGetDistance(ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
    if ((HIWORD(dTravel) > SIGNHIWORD(LOWORD(__ftol()))))
        goto L_43f1;
    else
        goto L_43db;

L_43db:
    /* untranslated: branch HIWORD(dTravel) < signhiword(loword(callresult(int32_t))) ? L_43e8 : L_43e0 */

L_43e0:
    /* untranslated: branch LOWORD(dTravel) >= loword(callresult(int32_t)) ? L_43f1 : L_43e8 */

L_43e8:
    t_merge_4403_0001 = LOWORD(dTravel);
    t_merge_4403_0002 = HIWORD(dTravel);
    goto L_4403;

L_43f1:
    __ftol();
    /* untranslated: t_merge_4403_0001 = loword(callresult(int32_t)) */
    /* untranslated: t_merge_4403_0002 = signhiword(loword(callresult(int32_t))) */

L_4403:
    LOWORD(dTravel) = t_merge_4403_0001;
    HIWORD(dTravel) = t_merge_4403_0002;
    if ((HIWORD(dTravel) < HIWORD(dRange)))
        goto L_444c;
    else
        goto L_4417;

L_4417:
    if ((HIWORD(dTravel) > HIWORD(dRange)))
        goto L_4424;
    else
        goto L_441c;

L_441c:
    if ((LOWORD(dTravel) <= LOWORD(dRange)))
        goto L_444c;
    else
        goto L_4424;

L_4424:
    lpfl->rgwtMin[0x4] = 0x0;
    wtFuelUsed = 0x1;
    dTravel = dRange;
    goto L_4516;

L_444c:
    if ((cPass <= 0x0))
        goto L_4479;
    else
        goto L_4455;

L_4455:
    lpfl->rgwtMin[0x4] = (lpfl->rgwtMin[0x4] + lpfl->lFuelUsed);
    dTravel = (dTravel + (uint32_t)(lpfl->dMoveUsed));

L_4479:
    wtFuelUsed = EstFuelUse(lpfl, 0x0, 0xffff, dTravel, 0x0);
    if ((cPass <= 0x0))
        goto L_44c7;
    else
        goto L_44a8;

L_44a8:
    lpfl->lFuelUsed = wtFuelUsed;
    dTravel = (dTravel - (uint32_t)(lpfl->dMoveUsed));

L_44c7:
    if ((0x0 < (HIWORD(lpfl->rgwtMin[0x4]) - HIWORD(wtFuelUsed))))
        goto L_44fa;
    else
        goto L_44e5;

L_44e5:
    if ((0x0 > (HIWORD(lpfl->rgwtMin[0x4]) - HIWORD(wtFuelUsed))))
        goto L_44f1;
    else
        goto L_44ea;

L_44ea:
    if ((0x0 <= (LOWORD(lpfl->rgwtMin[0x4]) - LOWORD(wtFuelUsed))))
        goto L_44fa;
    else
        goto L_44f1;

L_44f1:
    t_merge_450b_0001_wide = 0x0;
    goto L_450b;

L_44fa:
    t_merge_450b_0001_wide = (lpfl->rgwtMin[0x4] - wtFuelUsed);

L_450b:
    lpfl->rgwtMin[0x4] = t_merge_450b_0001_wide;

L_4516:
    if ((LOWORD(lpfl->rgwtMin[0x4]) != 0x0))
        goto L_465d;
    else
        goto L_4523;

L_4523:
    if ((HIWORD(lpfl->rgwtMin[0x4]) != 0x0))
        goto L_465d;
    else
        goto L_452d;

L_452d:
    if ((HIWORD(wtFuelUsed) < 0x0))
        goto L_465d;
    else
        goto L_4536;

L_4536:
    if ((HIWORD(wtFuelUsed) > 0x0))
        goto L_4544;
    else
        goto L_453b;

L_453b:
    if ((LOWORD(wtFuelUsed) <= 0x0))
        goto L_465d;
    else
        goto L_4544;

L_4544:
    __aFfcompp();
    /* untranslated: branch JNC ? L_4576 : L_4564 */

L_4564:
    if ((LOWORD(dRange) != 0x0))
        goto L_465d;
    else
        goto L_456d;

L_456d:
    if ((HIWORD(dRange) != 0x0))
        goto L_465d;
    else
        goto L_4576;

L_4576:
    if ((fGotEnufFuel != 0x0))
        goto L_465d;
    else
        goto L_457f;

L_457f:
    i = 0x0;

L_4584:
    i = (i + 0x1);
    if ((LOWORD(EstFuelUse(lpfl, 0x0, i, 0xffffffff, 0x0)) != 0x0))
        goto L_45c6;
    else
        goto L_45b2;

L_45b2:
    /* untranslated: branch hiword(callresult(int32_t)) != 0x0 ? L_45c6 : L_45b7 */

L_45b7:

L_45bd:
    if ((i < 0xa))
        goto L_4584;
    else
        goto L_45c6;

L_45c6:
    if ((i <= 0x1))
        goto L_4631;
    else
        goto L_45cf;

L_45cf:
    /* untranslated: part[6:2](lpfl->lpplord->rgord[0x1]) = ((part[6:2](lpfl->lpplord->rgord[0x1]) & 0xff0f) | (((i + 0xffff) & 0xf) << 0x4)) */
    FSendPlrMsg2(lpfl->iPlayer, 0x8b, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), (i + 0xffff));
    goto L_4658;

L_4631:
    FSendPlrMsg2(lpfl->iPlayer, 0x27, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), 0x0);

L_4658:
    fRanOutOfFuel = 0x1;

L_465d:
    if ((LOWORD(dRange) != 0x0))
        goto L_4672;
    else
        goto L_4666;

L_4666:
    if ((HIWORD(dRange) == 0x0))
        goto L_32f6;
    else
        goto L_466c;

L_466c:

L_4672:
    lpfl->fHereAllTurn = 0x0;
    dx = (ptEnd.x - ptBeg.x);
    dy = (ptEnd.y - ptBeg.y);
    if ((dx != 0x0))
        goto L_46aa;
    else
        goto L_46a1;

L_46a1:
    if ((dy == 0x0))
        goto L_4782;
    else
        goto L_46aa;

L_46aa:
    lpfl->fdirValid = 0x1;

L_46be:
    if ((abs(dx) > 0x7f))
        goto L_46e4;
    else
        goto L_46d1;

L_46d1:
    if ((abs(dy) <= 0x7f))
        goto L_46ff;
    else
        goto L_46e4;

L_46e4:
    dx = ((uint32_t)(dx) / 0x2);
    dy = ((uint32_t)(dy) / 0x2);
    goto L_46be;

L_46ff:
    *(lpfl + 0x74) = ((*(lpfl + 0x74) & 0xff00) | ((dx + 0x7f) & 0xff));
    *(lpfl + 0x74) = (lpfl->dirFltX | (((dy + 0x7f) & 0xff) << 0x8));
    /* untranslated: *(lpfl+0x76) = ((*(lpfl+0x76) & 0xfff0) | (((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x4) & 0xf) & 0xf)) */

L_4782:
    dActTravel = __ftol();
    if ((HIWORD(dTravel) > HIWORD(dActTravel)))
        goto L_47bd;
    else
        goto L_47a7;

L_47a7:
    if ((HIWORD(dTravel) < HIWORD(dActTravel)))
        goto L_47b4;
    else
        goto L_47ac;

L_47ac:
    if ((LOWORD(dTravel) >= LOWORD(dActTravel)))
        goto L_47bd;
    else
        goto L_47b4;

L_47b4:
    t_merge_47c3_0001 = LOWORD(dTravel);
    goto L_47c3;

L_47bd:
    t_merge_47c3_0001 = LOWORD(dActTravel);

L_47c3:
    dMineTravel = t_merge_47c3_0001;
    if ((((*(lpord + 0x18) >> 0x4) & 0xf) >= 0xb))
        goto L_484b;
    else
        goto L_47e0;

L_47e0:
    if ((FTravelThroughMineFields(lpfl, &(dMineTravel), 0x0) != 0x0))
        goto L_484b;
    else
        goto L_4802;

L_4802:
    lpfl->dMoveLeft = 0x0;
    if ((lpfl->fDead != 0x0))
        goto L_32f6;
    else
        goto L_481f;

L_481f:

L_4825:
    if ((SIGNHIWORD(dMineTravel) > HIWORD(dActTravel)))
        goto L_4951;
    else
        goto L_4831;

L_4831:
    if ((SIGNHIWORD(dMineTravel) < HIWORD(dActTravel)))
        goto L_483e;
    else
        goto L_4836;

L_4836:
    if ((dMineTravel >= LOWORD(dActTravel)))
        goto L_4951;
    else
        goto L_483e;

L_483e:
    dTravel = (uint32_t)(dMineTravel);

L_484b:
    if ((fRanOutOfFuel != 0x0))
        goto L_4951;
    else
        goto L_4854;

L_4854:
    if ((HIWORD(GetFuelFree(lpfl)) < 0x0))
        goto L_4951;
    else
        goto L_486a;

L_486a:
    /* untranslated: branch hiword(callresult(int32_t)) > 0x0 ? L_4877 : L_486f */

L_486f:
    /* untranslated: branch loword(callresult(int32_t)) <= 0x0 ? L_4951 : L_4877 */

L_4877:
    if ((HIWORD(dTravel) > HIWORD(dActTravel)))
        goto L_489b;
    else
        goto L_4885;

L_4885:
    if ((HIWORD(dTravel) < HIWORD(dActTravel)))
        goto L_4892;
    else
        goto L_488a;

L_488a:
    if ((LOWORD(dTravel) >= LOWORD(dActTravel)))
        goto L_489b;
    else
        goto L_4892;

L_4892:
    t_merge_48a1_0001 = dTravel;
    goto L_48a1;

L_489b:
    t_merge_48a1_0001 = dActTravel;

L_48a1:
    LCalcFuelGainFromRamScoops(lpfl, ((*(lpord + 0x18) >> 0x4) & 0xf), t_merge_48a1_0001);
    /* untranslated: lFuelGain = callresult(int32_t) */
    /* untranslated: branch hiword(callresult(int32_t)) < 0x0 ? L_4951 : L_48d2 */

L_48d2:
    /* untranslated: branch hiword(callresult(int32_t)) > 0x0 ? L_48df : L_48d7 */

L_48d7:
    /* untranslated: branch loword(callresult(int32_t)) <= 0x0 ? L_4951 : L_48df */

L_48df:
    lFuelGainAct = ChgCargo(grobjFleet, LOWORD(lpfl), 0x4, lFuelGain, 0x0);
    if ((HIWORD(lFuelGain) < 0x0))
        goto L_4927;
    else
        goto L_490e;

L_490e:
    if ((HIWORD(lFuelGain) > 0x0))
        goto L_491d;
    else
        goto L_4913;

L_4913:
    if ((LOWORD(lFuelGain) <= 0x7ef4))
        goto L_4927;
    else
        goto L_491d;

L_491d:
    lFuelGain = 0x7ef4;

L_4927:
    FSendPlrMsg2(lpfl->iPlayer, 0xf3, (LOWORD(lpfl) | 0x8000), LOWORD(lpfl), LOWORD(lFuelGain));

L_4951:
    if ((HIWORD(dActTravel) < HIWORD(dTravel)))
        goto LMakeItToDest;
    else
        goto L_495f;

L_495f:
    if ((HIWORD(dActTravel) > HIWORD(dTravel)))
        goto L_496c;
    else
        goto L_4964;

L_4964:
    if ((LOWORD(dActTravel) < LOWORD(dTravel)))
        goto LMakeItToDest;
    else
        goto L_496c;

L_496c:
    if ((HIWORD(dActTravel) > 0x0))
        goto L_49ed;
    else
        goto L_4975;

L_4975:
    if ((HIWORD(dActTravel) < 0x0))
        goto LMakeItToDest;
    else
        goto L_497a;

L_497a:
    if ((LOWORD(dActTravel) > 0x0))
        goto L_49ed;
    else
        goto LMakeItToDest;

LMakeItToDest:
    lpfl->pt.x = ptEnd.x;
    lpfl->pt.y = ptEnd.y;
    if ((((*(lpord + 0x18) >> 0x8) & 0xf) != 0x1))
        goto L_49bc;
    else
        goto L_49ab;

L_49ab:
    lpfl->idPlanet = *(lpord + 0x16);
    goto L_49c5;

L_49bc:
    lpfl->idPlanet = 0xffff;

L_49c5:
    if ((cPass <= 0x0))
        goto L_4b2d;
    else
        goto L_49ce;

L_49ce:
    /* untranslated: part[4:2](lpfl->lpflNext) = ((part[4:2](lpfl->lpflNext) & 0xf7ff) | 0x800) */

L_49ed:
    if ((ptEnd.x <= ptBeg.x))
        goto L_4a02;
    else
        goto L_49f8;

L_49f8:
    t_merge_4a09_0001 = 0.5;
    goto L_4a09;

L_4a02:
    t_merge_4a09_0001 = -0.5;

L_4a09:
    dxRound = t_merge_4a09_0001;
    if ((ptEnd.y <= ptBeg.y))
        goto L_4a24;
    else
        goto L_4a1a;

L_4a1a:
    t_merge_4a2b_0001 = 0.5;
    goto L_4a2b;

L_4a24:
    t_merge_4a2b_0001 = -0.5;

L_4a2b:
    dyRound = t_merge_4a2b_0001;
    __aFfcompp();
    /* untranslated: branch JA ? L_4a5b : L_4a46 */

L_4a46:
    __aFfcompp();
    /* untranslated: branch JNC ? L_4ace : L_4a5b */

L_4a5b:
    r = ((double)(dTravel) / d);
    /* untranslated: ss:[bp-0x72] = (ptEnd.x - ptBeg.x) */
    /* untranslated: ss:[bp-0x70] = signhiword((ptEnd.x - ptBeg.x)) */
    lpfl->pt.x = (LOWORD(__ftol()) + ptBeg.x);
    /* untranslated: ss:[bp-0x72] = (ptEnd.y - ptBeg.y) */
    /* untranslated: ss:[bp-0x70] = signhiword((ptEnd.y - ptBeg.y)) */
    lpfl->pt.y = (LOWORD(__ftol()) + ptBeg.y);
    lpfl->idPlanet = 0xffff;

L_4ace:
    if ((cPass <= 0x0))
        goto L_4b2d;
    else
        goto L_4ad7;

L_4ad7:
    if ((lpfl->dMoveLeft <= 0x0))
        goto L_4b2d;
    else
        goto L_4ae4;

L_4ae4:
    lpfl->dMoveUsed = (lpfl->dMoveUsed + LOWORD(dTravel));
    lpfl->dMoveLeft = (lpfl->dMoveLeft - LOWORD(dTravel));
    if ((lpfl->dMoveLeft <= 0x0))
        goto L_4b2d;
    else
        goto L_4b0b;

L_4b0b:
    if ((fRanOutOfFuel != 0x0))
        goto L_4b2d;
    else
        goto L_4b14;

L_4b14:
    fDone = 0x0;
    lpfl->fDone = 0x0;

L_4b2d:
    if ((gd.fRadiatingEngine == 0x0))
        goto L_4ce4;
    else
        goto L_4b40;

L_4b40:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < 0x0))
        goto L_4ce4;
    else
        goto L_4b4d;

L_4b4d:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > 0x0))
        goto L_4b5c;
    else
        goto L_4b52;

L_4b52:
    if ((LOWORD(lpfl->rgwtMin[0x3]) <= 0x0))
        goto L_4ce4;
    else
        goto L_4b5c;

L_4b5c:
    if ((cPass > 0x1))
        goto L_4ce4;
    else
        goto L_4b65;

L_4b65:
    if ((((uint16_t)(rgplr[lpfl->iPlayer].rgEnvVarMin[0x2]) + (uint16_t)(rgplr[lpfl->iPlayer].rgEnvVarMax[0x2])) >= 0xaa))
        goto L_4ce4;
    else
        goto L_4b9b;

L_4b9b:
    if (((uint16_t)(rgplr[lpfl->iPlayer].rgEnvVarMax[0x2]) == 0xffff))
        goto L_4ce4;
    else
        goto L_4bb6;

L_4bb6:
    iCtr = ((uint32_t)(((uint16_t)(rgplr[lpfl->iPlayer].rgEnvVarMin[0x2]) + (uint16_t)(rgplr[lpfl->iPlayer].rgEnvVarMax[0x2]))) / 0x2);
    pct = (uint32_t)(((0x56 - iCtr) >> 0x1));
    pct = (int32_t)(((uint32_t)((pct * lpfl->rgwtMin[0x3])) / 0x64));
    if ((0x0 < HIWORD(pct)))
        goto L_4c4b;
    else
        goto L_4c35;

L_4c35:
    if ((0x0 > HIWORD(pct)))
        goto L_4c42;
    else
        goto L_4c3a;

L_4c3a:
    if ((0x1 <= LOWORD(pct)))
        goto L_4c4b;
    else
        goto L_4c42;

L_4c42:
    t_merge_4c51_0003_wide = 0x1;
    goto L_4c51;

L_4c4b:
    t_merge_4c51_0003_wide = pct;

L_4c51:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > HIWORD(t_merge_4c51_0003_wide)))
        goto L_4c79;
    else
        goto L_4c5d;

L_4c5d:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < HIWORD(t_merge_4c51_0003_wide)))
        goto L_4c6b;
    else
        goto L_4c62;

L_4c62:
    if ((LOWORD(lpfl->rgwtMin[0x3]) >= LOWORD(t_merge_4c51_0003_wide)))
        goto L_4c79;
    else
        goto L_4c6b;

L_4c6b:
    t_merge_4ca3_0001_wide = lpfl->rgwtMin[0x3];
    goto L_4ca3;

L_4c79:
    if ((0x0 < HIWORD(pct)))
        goto L_4c9d;
    else
        goto L_4c87;

L_4c87:
    if ((0x0 > HIWORD(pct)))
        goto L_4c94;
    else
        goto L_4c8c;

L_4c8c:
    if ((0x1 <= LOWORD(pct)))
        goto L_4c9d;
    else
        goto L_4c94;

L_4c94:
    t_merge_4ca3_0001_wide = 0x1;
    goto L_4ca3;

L_4c9d:
    t_merge_4ca3_0001_wide = pct;

L_4ca3:
    pct = t_merge_4ca3_0001_wide;
    FSendPlrMsg2(lpfl->iPlayer, 0x74, (LOWORD(lpfl) | 0x8000), LOWORD(pct), LOWORD(lpfl));
    lpfl->rgwtMin[0x3] = (lpfl->rgwtMin[0x3] - pct);

L_4ce4:
    /* untranslated: branch ss:[ptEnd] != lpfl->pt.x ? L_4dd8 : L_4cfe */

L_4cfe:
    /* untranslated: branch ss:[ptEnd+0x2] != lpfl->pt.y ? L_4dd8 : L_4d07 */

L_4d07:
    if ((((*(lpord + 0x18) >> 0x8) & 0xf) != 0x8))
        goto L_4dd8;
    else
        goto L_4d1e;

L_4d1e:
    lpth = LpthFromId(*(lpord + 0x16));
    if ((LOWORD(lpth) != 0x0))
        goto L_4d45;
    else
        goto L_4d3c;

L_4d3c:
    if ((HIWORD(lpth) == 0x0))
        goto L_4dd8;
    else
        goto L_4d45;

L_4d45:
    if ((lpth->ith != ithWormhole))
        goto L_4dd8;
    else
        goto L_4d5b;

L_4d5b:
    grbitPlr = (0x1 << lpfl->iPlayer);
    lpthDest = LpthFromId(lpth->thw.idPartner);
    NoAutoTrackFleet(lpfl);
    lpth->thw.grbitPlrTrav = (lpth->thw.grbitPlrTrav | grbitPlr);
    *(lpthDest + 0xa) = (*(lpthDest + 0xa) | grbitPlr);
    *(lpthDest + 0x8) = (*(lpthDest + 0x8) | grbitPlr);
    lpfl->pt.x = lpthDest->pt.x;
    lpfl->pt.y = lpthDest->pt.y;
    lpord[0x1].pt.x = lpthDest->pt.x;
    *(lpord + 0x14) = lpthDest->pt.y;

L_4dd8:
    if ((lpfl->idPlanet != 0xffff))
        goto L_4e12;
    else
        goto L_4de5;

L_4de5:
    /* untranslated: branch FFindNearestObject(words(lpfl->pt.y, lpfl->pt.x), 0x81, &scan) == 0x0 ? L_4e12 : L_4e08 */

L_4e08:
    lpfl->idPlanet = scan.idpl;

L_4e12:
    lpord->pt.x = lpfl->pt.x;
    lpord->pt.y = lpfl->pt.y;
    lpord->id = lpfl->idPlanet;
    if ((lpfl->idPlanet != 0xffff))
        goto L_4e48;
    else
        goto L_4e42;

L_4e42:
    t_merge_4e4b_0001 = 0x4;
    goto L_4e4b;

L_4e48:
    t_merge_4e4b_0001 = 0x1;

L_4e4b:
    *(lpord + 0x6) = ((*(lpord + 0x6) & 0xf0ff) | ((t_merge_4e4b_0001 & 0xf) << 0x8));
    if ((fGotEnufFuel == 0x0))
        goto L_32f6;
    else
        goto L_4e78;

L_4e78:
    EstFuelUse(lpfl, 0x0, 0xffff, 0xffffffff, 0x0);
    /* untranslated: wtFuel2Dest = callresult(int32_t) */
    /* untranslated: branch hiword(callresult(int32_t)) < HIWORD(lpfl->rgwtMin[0x4]) ? L_32f6 : L_4eac */

L_4eac:
    /* untranslated: branch hiword(callresult(int32_t)) > HIWORD(lpfl->rgwtMin[0x4]) ? L_4eba : L_4eb1 */

L_4eb1:
    /* untranslated: branch loword(callresult(int32_t)) <= LOWORD(lpfl->rgwtMin[0x4]) ? L_32f6 : L_4eba */

L_4eba:
    if ((HIWORD(LGetFleetStat(lpfl, 0x1)) < HIWORD(wtFuel2Dest)))
        goto L_4ef5;
    else
        goto L_4ed4;

L_4ed4:
    /* untranslated: branch hiword(callresult(int32_t)) > HIWORD(wtFuel2Dest) ? L_4ee1 : L_4ed9 */

L_4ed9:
    /* untranslated: branch loword(callresult(int32_t)) <= LOWORD(wtFuel2Dest) ? L_4ef5 : L_4ee1 */

L_4ee1:
    lpfl->rgwtMin[0x4] = wtFuel2Dest;
    goto L_32f6;

L_4ef5:
    lpfl->rgwtMin[0x4] = LGetFleetStat(lpfl, 0x1);

L_4f15:
    if ((fDone != 0x0))
        goto L_4f30;
    else
        goto L_4f1e;

L_4f1e:
    t_4f1e = cPass;
    cPass = (cPass + 0x1);
    if ((t_4f1e < 0xa))
        goto MoveUnfinishedFleets;
    else
        goto L_4f2a;

L_4f2a:

L_4f30:
    KillUsedWaypoints();

L_4f35:
    return;
}

int16_t FTravelThroughMineFields(FLEET *lpfl, int16_t *pdTravel, THING *lpthHit) {
    int32_t  d2Closest;
    int16_t  rgishInc[16];
    int16_t  dTravel;
    POINT    ptAct;
    int16_t  iWarp;
    POINT    ptDst;
    int16_t  dy;
    int32_t  d2;
    int16_t  j;
    int16_t  dEnd;
    FLEET    flSrc;
    int32_t  dpsh;
    int16_t  cshT;
    int32_t  dmgReduce;
    int32_t  dmgToApply;
    int16_t  i;
    THING   *lpth;
    int16_t  dmgExtra;
    int16_t  cshDamaged;
    int16_t  fMineExpert;
    POINT    ptSrc;
    int16_t  iPlayer;
    int16_t  cFields;
    int16_t  dStart;
    FLEET    flDead;
    THING   *lpthMac;
    int32_t  csh;
    int16_t  rgi[3];
    int16_t  pct;
    int32_t  dmgTot;
    int16_t  cshDead;
    int16_t  rgcField[3];
    int16_t  raMajor;
    int16_t  dx;
    int32_t  dpShield;
    int16_t  iType;
    int16_t  rgFieldE[3][8];
    THING   *lpthClosest;
    int16_t  cishInc;
    THING   *lpthSalvage;
    int16_t  fHasRamScoop;
    int16_t  dmgPer;
    int16_t  rgFieldS[3][8];
    int16_t  cEngines;
    int32_t  dmgPerShip;
    uint16_t ibit;
    uint16_t t_merge_4fdd_0001;
    uint16_t t_58cd;
    int32_t  t_merge_5ae4_0001_wide;
    uint16_t t_merge_63a1_0001;
    uint16_t t_merge_63f3_0001;
    uint16_t t_merge_6494_0001;
    uint16_t t_merge_64e6_0001;

L_4f60:
    lpthSalvage = 0x0;
    cshDead = 0x0;
    dTravel = *(pdTravel);
    cishInc = 0x0;
    iPlayer = lpfl->iPlayer;
    raMajor = GetRaceStat(rgplr[iPlayer], rsMajorAdv);
    if ((raMajor != 0x5))
        goto L_4fc1;
    else
        goto L_4fbb;

L_4fbb:
    goto L_4fc4;

L_4fc1:

L_4fc4:
    if ((raMajor != 0x1))
        goto L_4fda;
    else
        goto L_4fd4;

L_4fd4:
    t_merge_4fdd_0001 = 0x1;
    goto L_4fdd;

L_4fda:
    t_merge_4fdd_0001 = 0x0;

L_4fdd:
    /* untranslated: fMineExpert = (ss:[bp-0x1ee] + t_merge_4fdd_0001) */
    if ((LOWORD(lpthHit) != 0x0))
        goto L_4ff9;
    else
        goto L_4ff0;

L_4ff0:
    if ((HIWORD(lpthHit) == 0x0))
        goto L_5001;
    else
        goto L_4ff9;

L_4ff9:
    iWarp = 0x0;
    goto LHitSkip1;

L_5001:
    ptSrc.x = lpfl->pt.x;
    ptSrc.y = lpfl->pt.y;
    ptDst.x = lpfl->lpplord->rgord[0x1].pt.x;
    ptDst.y = lpfl->lpplord->rgord[0x1].pt.y;
    iWarp = 0x3;

L_502e:
    if ((iWarp >= 0xa))
        goto L_5051;
    else
        goto L_5037;

L_5037:
    if ((LOWORD((iWarp * iWarp)) >= (dTravel + 0xffff)))
        goto L_5051;
    else
        goto L_504a;

L_504a:
    iWarp = (iWarp + 0x1);
    goto L_502e;

L_5051:
    if ((iWarp <= (fMineExpert + 0x3)))
        goto L_5078;
    else
        goto L_5060;

L_5060:
    if ((ptSrc.x != ptDst.x))
        goto L_507e;
    else
        goto L_506f;

L_506f:
    if ((ptSrc.y != ptDst.y))
        goto L_507e;
    else
        goto L_5078;

L_5078:
    return 0x1;

L_507e:
    i = 0x0;
    goto L_509c;

L_5087:
    rgcField[i] = 0x0;
    i = (i + 0x1);

L_509c:
    if ((i < 0x3))
        goto L_5087;
    else
        goto L_50a6;

L_50a6:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_5560;

L_50d9:
    if ((lpth->iplr == iPlayer))
        goto L_555b;
    else
        goto L_50f1;

L_50f1:
    if ((lpth->ith != ithMinefield))
        goto L_555b;
    else
        goto L_5108;

L_5108:
    if (((uint16_t)(rgplr[lpth->iplr].rgmdRelation[iPlayer]) == 0x1))
        goto L_555b;
    else
        goto L_5134;

L_5134:

L_513a:
    /* untranslated: branch FIntersectCircleLine(words(ptSrc.y, ptSrc.x), words(ptDst.y, ptDst.x), words(lpth->pt.y, lpth->pt.x),
     * words(HIWORD(lpth->thm.cMines), LOWORD(lpth->thm)), dTravel, &dStart, &dEnd) == 0x0 ? L_555b : L_517c */

L_517c:
    iType = lpth->thm.iType;
    i = 0x0;
    goto L_5199;

L_5194:
    i = (i + 0x1);

L_5199:
    if ((i >= rgcField[iType]))
        goto L_51db;
    else
        goto L_51b0;

L_51b0:
    if ((rgFieldE[iType][i] >= dStart))
        goto L_51db;
    else
        goto L_51d2;

L_51d2:

L_51db:
    if ((i != rgcField[iType]))
        goto L_524d;
    else
        goto L_51f2;

L_51f2:
    if ((i >= 0x8))
        goto L_555b;
    else
        goto L_51fc;

L_51fc:
    rgFieldS[iType][i] = dStart;
    rgFieldE[iType][i] = dEnd;
    rgcField[iType] = (rgcField[iType] + 0x1);

L_524d:
    if ((dEnd >= (rgFieldS[iType][i] + 0xffff)))
        goto L_536f;
    else
        goto L_5274;

L_5274:
    if ((rgcField[iType] >= 0x8))
        goto L_555b;
    else
        goto L_5288;

L_5288:
    j = rgcField[iType];
    goto L_5312;

L_529c:
    rgFieldS[iType][j] = rgFieldS[iType][(j - 0x1)];
    rgFieldE[iType][j] = rgFieldE[iType][(j - 0x1)];
    j = (j - 0x1);

L_5312:
    if ((j > i))
        goto L_529c;
    else
        goto L_531e;

L_531e:
    rgFieldS[iType][i] = dStart;
    rgFieldE[iType][i] = dEnd;
    rgcField[iType] = (rgcField[iType] + 0x1);

L_536f:
    if ((dStart >= rgFieldS[iType][i]))
        goto L_53b4;
    else
        goto L_5394;

L_5394:
    rgFieldS[iType][i] = dStart;

L_53b4:
    if ((dEnd <= rgFieldE[iType][i]))
        goto L_555b;
    else
        goto L_53d8;

L_53d8:
    rgFieldE[iType][i] = dEnd;
    j = (i + 0x1);
    goto L_5408;

L_5404:
    j = (j + 0x1);

L_5408:
    if ((j >= rgcField[iType]))
        goto L_5447;
    else
        goto L_541e;

L_541e:
    if ((rgFieldS[iType][j] > dEnd))
        goto L_5447;
    else
        goto L_543e;

L_543e:

L_5447:
    if ((rgFieldE[iType][(j - 0x1)] <= dEnd))
        goto L_54a7;
    else
        goto L_546d;

L_546d:
    rgFieldE[iType][i] = rgFieldE[iType][(j - 0x1)];

L_54a7:
    i = (i + 0x1);
    goto L_54bf;

L_54af:
    i = (i + 0x1);
    j = (j + 0x1);

L_54bf:
    if ((j >= rgcField[iType]))
        goto L_5546;
    else
        goto L_54d5;

L_54d5:
    rgFieldS[iType][i] = rgFieldS[iType][j];
    rgFieldE[iType][i] = rgFieldE[iType][j];
    goto L_54af;

L_5546:
    rgcField[iType] = (rgcField[iType] - (j - i));

L_555b:
    lpth = (lpth + 0x1);

L_5560:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_50d9;
    else
        goto L_5571;

L_5571:
    /* untranslated: cFields = ((LOWORD(rgcField) + HIWORD(rgcField)) + part[4:2](rgcField)) */
    if ((cFields != 0x0))
        goto LHitSkip1;
    else
        goto L_558b;

L_558b:
    return 0x1;

LHitSkip1:
    fHasRamScoop = 0x0;
    csh = 0x0;
    i = 0x0;
    goto L_5634;

L_55ac:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_562f;
    else
        goto L_55ca;

L_55ca:
    csh = (csh + (uint32_t)(lpfl->rgcsh[i]));
    j = (HIWORD(rglpshdef[iPlayer][i].hul.rghs[0x0]) & 0xff);
    if ((LpengineFromId(j)->rgcFuelUsed[0x4] != 0x0))
        goto L_562f;
    else
        goto L_5629;

L_5629:
    fHasRamScoop = 0x1;

L_562f:
    i = (i + 0x1);

L_5634:
    if ((i < 0x10))
        goto L_55ac;
    else
        goto L_563e;

L_563e:
    if ((LOWORD(lpthHit) != 0x0))
        goto L_5650;
    else
        goto L_5647;

L_5647:
    if ((HIWORD(lpthHit) == 0x0))
        goto L_5661;
    else
        goto L_5650;

L_5650:
    iType = *(lpthHit + 0xc);
    goto LHitSkip2;

L_5661:
    /* untranslated: part[4:2](rgi) = 0x0 */
    HIWORD(rgi) = 0x0;
    LOWORD(rgi) = 0x0;
    goto L_6781;

L_5675:
    dStart = 0x2710;
    iType = 0xffff;
    i = 0x0;
    goto L_5711;

L_568a:
    if ((rgi[i] >= rgcField[i]))
        goto L_570c;
    else
        goto L_56ab;

L_56ab:
    if ((dStart <= rgFieldS[i][rgi[i]]))
        goto L_570c;
    else
        goto L_56da;

L_56da:
    dStart = rgFieldS[i][rgi[i]];
    iType = i;

L_570c:
    i = (i + 0x1);

L_5711:
    if ((i < 0x3))
        goto L_568a;
    else
        goto L_571b;

L_571b:
    dEnd = (rgFieldE[iType][rgi[iType]] - rgFieldS[iType][rgi[iType]]);
    /* untranslated: branch iWarp <= (0x17:[(iType * 0x2)+0x4f5a] + fMineExpert) ? L_676d : L_577e */

L_577e:

L_5784:
    /* untranslated: pct = loword((((iWarp - 0x17:[(iType * 0x2)+0x4f5a]) - fMineExpert) * 0x17:[(iType * 0x2)+0x4f54])) */
    i = 0x0;
    goto L_57cb;

L_57ae:
    if ((Random(0x3e8) < pct))
        goto L_57d7;
    else
        goto L_57c0;

L_57c0:

L_57c6:
    i = (i + 0x1);

L_57cb:
    if ((i < dEnd))
        goto L_57ae;
    else
        goto L_57d7;

L_57d7:
    if ((i == dEnd))
        goto LDoNext;
    else
        goto L_57e0;

L_57e0:

L_57e6:
    dEnd = (dStart + i);

LHitSkip2:
    cshDead = 0x0;
    dmgTot = 0x0;
    /* untranslated: branch 0x17:[((0x4f3c + (iType * 0x4)) + (fHasRamScoop * 0x2))] == 0x0 ? L_5d7f : L_5822 */

L_5822:

L_5828:
    /* untranslated: dmgPer = 0x17:[((0x4f3c + (iType * 0x4)) + (fHasRamScoop * 0x2))] */
    /* untranslated: dmgExtra = (0x17:[((0x4f48 + (iType * 0x4)) + (fHasRamScoop * 0x2))] - loword((0x17:[((0x4f3c + (iType * 0x4)) + (fHasRamScoop * 0x2))] *
     * LOWORD(csh)))) */
    if ((HIWORD(csh) > 0x0))
        goto L_5891;
    else
        goto L_5878;

L_5878:
    if ((HIWORD(csh) < 0x0))
        goto L_5887;
    else
        goto L_587d;

L_587d:
    if ((LOWORD(csh) >= 0x5))
        goto L_5891;
    else
        goto L_5887;

L_5887:
    if ((dmgExtra > 0x0))
        goto L_5897;
    else
        goto L_5891;

L_5891:
    dmgExtra = 0x0;

L_5897:
    flSrc = *(lpfl);
    memset(flDead, 0x0, 0x7c);
    flDead.iPlayer = flSrc.iPlayer;
    /* untranslated: t_58cd = part[4:2](flDead) */
    flDead.fDead = 0x1;
    /* untranslated: part[4:2](flDead) = ((((t_58cd & 0xfbff) | 0x400) & 0xff00) | 0x7) */
    i = 0x0;
    goto L_5ce3;

L_58ee:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_5cde;
    else
        goto L_590c;

L_590c:
    if ((LOWORD(lpthHit) != 0x0))
        goto L_591e;
    else
        goto L_5915;

L_5915:
    if ((HIWORD(lpthHit) == 0x0))
        goto L_5989;
    else
        goto L_591e;

L_591e:
    if ((lpthHit->iplr != lpfl->iPlayer))
        goto L_5989;
    else
        goto L_5938;

L_5938:
    if ((rglpshdef[lpfl->iPlayer][i].hul.ihuldef == ihuldefMiniMineLayer))
        goto L_5cde;
    else
        goto L_595f;

L_595f:
    if ((rglpshdef[lpfl->iPlayer][i].hul.ihuldef == ihuldefSuperMineLayer))
        goto L_5cde;
    else
        goto L_5983;

L_5983:

L_5989:
    cshT = lpfl->rgcsh[i];
    cishInc = (cishInc + 0x1);
    rgishInc[cishInc] = i;
    cEngines = ((HIWORD(rglpshdef[lpfl->iPlayer][i].hul.rghs[0x0]) >> 0x8) & 0xff);
    dpShield = (uint32_t)(((uint32_t)(cshT)*DpShieldOfShdef(&(rglpshdef[iPlayer][i]), iPlayer)));
    LOWORD(dpsh) = rglpshdef[iPlayer][i].hul.dp;
    HIWORD(dpsh) = 0x0;
    /* untranslated: dmgToApply = (uint32_t)(words((hiword((uint32_t)(sext16to32(cshT) * sext16to32(dmgPer))) + signhiword(dmgExtra)),
     * (loword((uint32_t)(sext16to32(cshT) * sext16to32(dmgPer))) + dmgExtra)) * sext16to32(cEngines)) */
    dmgTot = (dmgTot + dmgToApply);
    if ((HIWORD(dpShield) > HIWORD((int32_t)((dmgToApply >> 0x1)))))
        goto L_5ad4;
    else
        goto L_5abb;

L_5abb:
    /* untranslated: branch HIWORD(dpShield) < hiword(callresult(int32_t)) ? L_5ac9 : L_5ac0 */

L_5ac0:
    /* untranslated: branch LOWORD(dpShield) >= loword(callresult(int32_t)) ? L_5ad4 : L_5ac9 */

L_5ac9:
    t_merge_5ae4_0001_wide = dpShield;
    goto L_5ae4;

L_5ad4:
    /* untranslated: t_merge_5ae4_0001_wide = callresult(int32_t) */

L_5ae4:
    dmgReduce = t_merge_5ae4_0001_wide;
    dmgToApply = (dmgToApply - dmgReduce);
    /* untranslated: cshDamaged = loword((int32_t)(words(hiword((uint32_t)(sext16to32(lpfl->rgcsh[i]) * words(0x0, (HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i *
     * 0x2))] & 0x7f)))), loword((uint32_t)(sext16to32(lpfl->rgcsh[i]) * words(0x0, (HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 0x2))] & 0x7f))))) / 0x64)) */
    /* untranslated: LOWORD(dmgToApply) = (LOWORD(dmgToApply) + loword((int32_t)(words(hiword((uint32_t)(words(hiword((uint32_t)(dpsh * words(0x0,
     * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 0x2))] >> 0x7) & 0x1ff)))), loword((uint32_t)(dpsh * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i
     * * 0x2))] >> 0x7) & 0x1ff))))) * sext16to32(cshDamaged))), loword((uint32_t)(words(hiword((uint32_t)(dpsh * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) +
     * 0x2c) + (i * 0x2))] >> 0x7) & 0x1ff)))), loword((uint32_t)(dpsh * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 0x2))] >> 0x7) & 0x1ff))))) *
     * sext16to32(cshDamaged)))) / 0x1f4))) */
    /* untranslated: HIWORD(dmgToApply) = (HIWORD(dmgToApply) + hiword((int32_t)(words(hiword((uint32_t)(words(hiword((uint32_t)(dpsh * words(0x0,
     * ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 0x2))] >> 0x7) & 0x1ff)))), loword((uint32_t)(dpsh * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i
     * * 0x2))] >> 0x7) & 0x1ff))))) * sext16to32(cshDamaged))), loword((uint32_t)(words(hiword((uint32_t)(dpsh * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) +
     * 0x2c) + (i * 0x2))] >> 0x7) & 0x1ff)))), loword((uint32_t)(dpsh * words(0x0, ((HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 0x2))] >> 0x7) & 0x1ff))))) *
     * sext16to32(cshDamaged)))) / 0x1f4))) */
    dmgExtra = 0x0;
    dmgPerShip = (int32_t)((dmgToApply / (uint32_t)(cshT)));
    if ((HIWORD(dmgPerShip) < HIWORD(dpsh)))
        goto L_5c1a;
    else
        goto L_5bd9;

L_5bd9:
    if ((HIWORD(dmgPerShip) > HIWORD(dpsh)))
        goto L_5be7;
    else
        goto L_5bde;

L_5bde:
    if ((LOWORD(dmgPerShip) <= LOWORD(dpsh)))
        goto L_5c1a;
    else
        goto L_5be7;

L_5be7:
    cshDead = (cshDead + cshT);
    flDead.rgcsh[i] = cshT;
    flSrc.rgcsh[i] = 0x0;
    cshT = 0x0;
    goto L_5cd2;

L_5c1a:
    /* untranslated: ss:[bp+(i * 0x2)-0x8c] = ((ss:[bp+(i * 0x2)-0x8c] & 0xff80) | 0x64) */
    /* untranslated: ss:[bp+(i * 0x2)-0x8c] = ((ss:[bp+(i * 0x2)-0x8c] & 0x7f) | ((loword((int32_t)((uint32_t)(dmgPerShip * 0x1f4) / dpsh)) & 0x1ff) << 0x7)) */
    /* untranslated: branch ((ss:[bp+(i * 0x2)-0x8c] >> 0x7) & 0x1ff) != 0x0 ? L_5cd2 : L_5cb0 */

L_5cb0:
    /* untranslated: ss:[bp+(i * 0x2)-0x8c] = ((ss:[bp+(i * 0x2)-0x8c] & 0x7f) | 0x80) */

L_5cd2:
    dmgToApply = 0x0;

L_5cde:
    i = (i + 0x1);

L_5ce3:
    if ((i < 0x10))
        goto L_58ee;
    else
        goto L_5ced;

L_5ced:
    if ((LOWORD(lpthHit) != 0x0))
        goto L_5cff;
    else
        goto L_5cf6;

L_5cf6:
    if ((HIWORD(lpthHit) == 0x0))
        goto L_5d19;
    else
        goto L_5cff;

L_5cff:
    if ((LOWORD(dmgTot) != 0x0))
        goto L_5d19;
    else
        goto L_5d09;

L_5d09:
    if ((HIWORD(dmgTot) != 0x0))
        goto L_5d19;
    else
        goto L_5d13;

L_5d13:
    return 0x0;

L_5d19:
    if ((cshDead != LOWORD(csh)))
        goto L_5d30;
    else
        goto L_5d27;

L_5d27:
    if ((SIGNHIWORD(cshDead) == HIWORD(csh)))
        goto L_5d42;
    else
        goto L_5d30;

L_5d30:
    FleetTransferCargoBalance(&(flSrc), &(flDead));

L_5d42:
    *(lpfl) = flSrc;
    if ((cshDead != LOWORD(csh)))
        goto LFinishHit;
    else
        goto L_5d62;

L_5d62:
    if ((SIGNHIWORD(cshDead) != HIWORD(csh)))
        goto LFinishHit;
    else
        goto L_5d6b;

L_5d6b:
    lpfl->fDead = 0x1;

LFinishHit:
    if ((LOWORD(lpthHit) != 0x0))
        goto L_6137;
    else
        goto L_5d88;

L_5d88:
    if ((HIWORD(lpthHit) != 0x0))
        goto L_6137;
    else
        goto L_5d91;

L_5d91:
    dx = (ptDst.x - ptSrc.x);
    dy = (ptDst.y - ptSrc.y);
    /* untranslated: ss:[bp-0x1f8] = (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + loword((uint32_t)(sext16to32(dy) * sext16to32((ptDst.y -
     * ptSrc.y))))) */
    /* untranslated: ss:[bp-0x1f6] = (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + hiword((uint32_t)(sext16to32(dy) * sext16to32((ptDst.y -
     * ptSrc.y))))) */
    /* untranslated: call sqrt((double)dword ss:[bp-0x1f8]) -> callresult(double) */
    __ftol();
    /* untranslated: dTravel = loword(callresult(int32_t)) */
    /* untranslated: ptAct.x = (MulDiv(dx, dEnd, loword(callresult(int32_t))) + ptSrc.x) */
    ptAct.y = (MulDiv(dy, dEnd, dTravel) + ptSrc.y);
    if ((cshDead == 0x0))
        goto L_5f34;
    else
        goto L_5e44;

L_5e44:
    lpthSalvage = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_5ed2;

L_5e77:
    /* untranslated: branch lpthSalvage->pt.x != ss:[ptAct] ? L_5ecd : L_5e92 */

L_5e92:
    /* untranslated: branch lpthSalvage->pt.y != ss:[ptAct+0x2] ? L_5ecd : L_5e9b */

L_5e9b:
    if ((lpthSalvage->ith != ithMineralPacket))
        goto L_5ecd;
    else
        goto L_5eb2;

L_5eb2:
    if ((((LOWORD(lpthSalvage->thp) >> 0xa) & 0xf) == 0x0))
        goto L_5ee3;
    else
        goto L_5ec7;

L_5ec7:

L_5ecd:
    lpthSalvage = (lpthSalvage + 0x1);

L_5ed2:
    if ((LOWORD(lpthSalvage) < LOWORD(lpthMac)))
        goto L_5e77;
    else
        goto L_5ee3;

L_5ee3:
    if ((LOWORD(lpthSalvage) != LOWORD(lpthMac)))
        goto L_5f09;
    else
        goto L_5ef4;

L_5ef4:
    if ((HIWORD(lpthSalvage) != HIWORD(lpthMac)))
        goto L_5f09;
    else
        goto L_5efd;

L_5efd:
    lpthSalvage = 0x0;

L_5f09:
    /* untranslated: call DropSalvage(&lpthSalvage, &part[0:0](lpfl->rgwtMin), flSrc.iplr, &ptAct) -> callresult(void) */

L_5f34:
    d2Closest = 0x5f5e100;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_6089;

L_5f71:
    if ((lpth->iplr == iPlayer))
        goto L_6084;
    else
        goto L_5f89;

L_5f89:
    if ((lpth->ith != ithMinefield))
        goto L_6084;
    else
        goto L_5fa0;

L_5fa0:
    if (((uint16_t)(rgplr[lpth->iplr].rgmdRelation[iPlayer]) == 0x1))
        goto L_6084;
    else
        goto L_5fcc;

L_5fcc:

L_5fd2:
    if ((lpth->thm.iType != iType))
        goto L_6084;
    else
        goto L_5fe3;

L_5fe3:

L_5fe9:
    dx = (lpth->pt.x - ptAct.x);
    dy = (lpth->pt.y - ptAct.y);
    LOWORD(d2) = ((LOWORD((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)))) + LOWORD((uint32_t)(((uint32_t)(dy) * (uint32_t)((lpth->pt.y - ptAct.y)))))) -
                  LOWORD(lpth->thm));
    HIWORD(d2) = ((HIWORD((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)))) + HIWORD((uint32_t)(((uint32_t)(dy) * (uint32_t)((lpth->pt.y - ptAct.y)))))) -
                  HIWORD(lpth->thm.cMines));
    if ((HIWORD(d2) > HIWORD(d2Closest)))
        goto L_6084;
    else
        goto L_605b;

L_605b:
    if ((HIWORD(d2) < HIWORD(d2Closest)))
        goto L_6068;
    else
        goto L_6060;

L_6060:
    if ((LOWORD(d2) >= LOWORD(d2Closest)))
        goto L_6084;
    else
        goto L_6068;

L_6068:
    d2Closest = d2;
    lpthClosest = lpth;

L_6084:
    lpth = (lpth + 0x1);

L_6089:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_5f71;
    else
        goto L_609a;

L_609a:
    /* untranslated: d2 = (int32_t)(words(*(lpthClosest+0x8), *(lpthClosest+0x6)) / 0x14) */
    if ((HIWORD(d2) < 0x0))
        goto L_6113;
    else
        goto L_60c2;

L_60c2:
    if ((HIWORD(d2) > 0x0))
        goto L_60d0;
    else
        goto L_60c7;

L_60c7:
    if ((LOWORD(d2) <= 0x32))
        goto L_6113;
    else
        goto L_60d0;

L_60d0:
    /* untranslated: d2 = (int32_t)(words(*(lpthClosest+0x8), *(lpthClosest+0x6)) / 0x64) */
    if ((HIWORD(d2) > 0x0))
        goto L_6156;
    else
        goto L_60f8;

L_60f8:
    if ((HIWORD(d2) < 0x0))
        goto L_6106;
    else
        goto L_60fd;

L_60fd:
    if ((LOWORD(d2) >= 0x32))
        goto L_6156;
    else
        goto L_6106;

L_6106:
    d2 = 0x32;

L_6113:
    if ((HIWORD(d2) > 0x0))
        goto L_6156;
    else
        goto L_611c;

L_611c:
    if ((HIWORD(d2) < 0x0))
        goto L_612a;
    else
        goto L_6121;

L_6121:
    if ((LOWORD(d2) >= 0xa))
        goto L_6156;
    else
        goto L_612a;

L_612a:
    d2 = 0xa;

L_6137:
    ptAct.x = lpthHit->pt.x;
    ptAct.y = lpthHit->pt.y;
    lpthClosest = lpthHit;

L_6156:
    if ((GetRaceStat(rgplr[lpthClosest->iplr], rsMajorAdv) != raMines))
        goto L_624d;
    else
        goto L_6184;

L_6184:
    ibit = (0x1 << lpthClosest->iplr);
    if ((cishInc != 0x0))
        goto L_6205;
    else
        goto L_61a8;

L_61a8:
    i = 0x0;
    goto L_61f8;

L_61b1:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_61f3;
    else
        goto L_61cf;

L_61cf:
    rglpshdef[iPlayer][i].grbitPlr = (rglpshdef[iPlayer][i].grbitPlr | ibit);

L_61f3:
    i = (i + 0x1);

L_61f8:
    if ((i >= 0x10))
        goto L_624d;
    else
        goto L_61ff;

L_61ff:

L_6205:
    i = 0x0;
    goto L_6240;

L_620e:
    rglpshdef[iPlayer][rgishInc[i]].grbitPlr = (rglpshdef[iPlayer][rgishInc[i]].grbitPlr | ibit);
    i = (i + 0x1);

L_6240:
    if ((i < cishInc))
        goto L_620e;
    else
        goto L_624d;

L_624d:
    if ((HIWORD(dmgTot) < 0x0))
        goto L_6273;
    else
        goto L_6257;

L_6257:
    if ((HIWORD(dmgTot) > 0x0))
        goto L_6267;
    else
        goto L_625c;

L_625c:
    if ((LOWORD(dmgTot) <= 0x7ff8))
        goto L_6273;
    else
        goto L_6267;

L_6267:
    dmgTot = 0x7ff8;

L_6273:
    if ((LOWORD(dmgTot) != 0x0))
        goto L_632e;
    else
        goto L_627d;

L_627d:
    if ((HIWORD(dmgTot) != 0x0))
        goto L_632e;
    else
        goto L_6287;

L_6287:
    if ((iPlayer == lpthClosest->iplr))
        goto L_62e5;
    else
        goto L_629f;

L_629f:
    FSendPlrMsg(iPlayer, 0xc5, (LOWORD(lpfl) | 0x8000), (LOWORD(lpfl) | 0x8000), lpthClosest->iplr, iType, ptAct.x, ptAct.y, 0x0, 0x0);

L_62e5:
    FSendPlrMsg(lpthClosest->iplr, 0xc9, (LOWORD(lpfl) | 0x8000), (LOWORD(lpfl) | 0x8000), iType, ptAct.x, ptAct.y, 0x0, 0x0, 0x0);
    goto L_66d1;

L_632e:
    if ((cshDead != 0x0))
        goto L_640f;
    else
        goto L_6338;

L_6338:
    if ((iPlayer == lpthClosest->iplr))
        goto L_63ae;
    else
        goto L_6350;

L_6350:
    if ((LOWORD(lpthHit) != 0x0))
        goto L_6398;
    else
        goto L_638f;

L_638f:
    if ((HIWORD(lpthHit) == 0x0))
        goto L_639e;
    else
        goto L_6398;

L_6398:
    t_merge_63a1_0001 = 0x160;
    goto L_63a1;

L_639e:
    t_merge_63a1_0001 = 0xc6;

L_63a1:
    FSendPlrMsg(iPlayer, t_merge_63a1_0001, (LOWORD(lpfl) | 0x8000), (LOWORD(lpfl) | 0x8000), lpthClosest->iplr, iType, ptAct.x, ptAct.y, LOWORD(dmgTot), 0x0);

L_63ae:
    if ((LOWORD(lpthHit) != 0x0))
        goto L_63ea;
    else
        goto L_63e1;

L_63e1:
    if ((HIWORD(lpthHit) == 0x0))
        goto L_63f0;
    else
        goto L_63ea;

L_63ea:
    t_merge_63f3_0001 = 0x163;
    goto L_63f3;

L_63f0:
    t_merge_63f3_0001 = 0xca;

L_63f3:
    FSendPlrMsg(lpthClosest->iplr, t_merge_63f3_0001, (LOWORD(lpfl) | 0x8000), (LOWORD(lpfl) | 0x8000), iType, ptAct.x, ptAct.y, LOWORD(dmgTot), 0x0, 0x0);
    goto L_66d1;

L_640f:
    if ((SIGNHIWORD(cshDead) > HIWORD(csh)))
        goto L_6502;
    else
        goto L_641d;

L_641d:
    if ((SIGNHIWORD(cshDead) < HIWORD(csh)))
        goto L_642b;
    else
        goto L_6422;

L_6422:
    if ((cshDead >= LOWORD(csh)))
        goto L_6502;
    else
        goto L_642b;

L_642b:
    if ((iPlayer == lpthClosest->iplr))
        goto L_64a1;
    else
        goto L_6443;

L_6443:
    if ((LOWORD(lpthHit) != 0x0))
        goto L_648b;
    else
        goto L_6482;

L_6482:
    if ((HIWORD(lpthHit) == 0x0))
        goto L_6491;
    else
        goto L_648b;

L_648b:
    t_merge_6494_0001 = 0x161;
    goto L_6494;

L_6491:
    t_merge_6494_0001 = 0xc7;

L_6494:
    FSendPlrMsg(iPlayer, t_merge_6494_0001, (LOWORD(lpfl) | 0x8000), (LOWORD(lpfl) | 0x8000), lpthClosest->iplr, iType, ptAct.x, ptAct.y, LOWORD(dmgTot),
                cshDead);

L_64a1:
    if ((LOWORD(lpthHit) != 0x0))
        goto L_64dd;
    else
        goto L_64d4;

L_64d4:
    if ((HIWORD(lpthHit) == 0x0))
        goto L_64e3;
    else
        goto L_64dd;

L_64dd:
    t_merge_64e6_0001 = 0x164;
    goto L_64e6;

L_64e3:
    t_merge_64e6_0001 = 0xcb;

L_64e6:
    FSendPlrMsg(lpthClosest->iplr, t_merge_64e6_0001, (LOWORD(lpfl) | 0x8000), (LOWORD(lpfl) | 0x8000), iType, ptAct.x, ptAct.y, LOWORD(dmgTot), cshDead, 0x0);
    goto L_66d1;

L_6502:
    if ((LOWORD(lpthSalvage) != 0x0))
        goto L_6516;
    else
        goto L_650c;

L_650c:
    if ((HIWORD(lpthSalvage) == 0x0))
        goto L_65c4;
    else
        goto L_6516;

L_6516:
    flDead.id = LOWORD(lpfl);
    if ((iPlayer == lpthClosest->iplr))
        goto L_6582;
    else
        goto L_6538;

L_6538:
    FSendPlrMsg(iPlayer, 0xc8, 0xfffa, LOWORD(lpthSalvage), WFromLpfl(&(flDead)), lpthClosest->iplr, iType, ptAct.x, ptAct.y, 0x0);

L_6582:
    FSendPlrMsg(lpthClosest->iplr, 0xcc, 0xfffa, LOWORD(lpthSalvage), LOWORD(lpfl), iType, ptAct.x, ptAct.y, 0x0, 0x0);
    goto L_66d1;

L_65c4:
    flDead.id = LOWORD(lpfl);
    if ((iPlayer == lpthClosest->iplr))
        goto L_662d;
    else
        goto L_65e6;

L_65e6:
    FSendPlrMsg(iPlayer, 0x15f, 0xffff, WFromLpfl(&(flDead)), lpthClosest->iplr, iType, ptAct.x, ptAct.y, 0x0, 0x0);

L_662d:
    if ((lpfl->iPlayer != lpthClosest->iplr))
        goto L_6692;
    else
        goto L_6648;

L_6648:
    FSendPlrMsg(lpthClosest->iplr, 0x162, 0xffff, WFromLpfl(&(flDead)), iType, ptAct.x, ptAct.y, 0x0, 0x0, 0x0);
    goto L_66d1;

L_6692:
    FSendPlrMsg(lpthClosest->iplr, 0xcc, 0xfffa, LOWORD(lpthClosest), LOWORD(lpfl), iType, ptAct.x, ptAct.y, 0x0, 0x0);

L_66d1:
    if ((LOWORD(lpthHit) != 0x0))
        goto L_6753;
    else
        goto L_66da;

L_66da:
    if ((HIWORD(lpthHit) != 0x0))
        goto L_6753;
    else
        goto L_66e3;

L_66e3:
    if ((HIWORD(d2) < *(lpthClosest + 0x8)))
        goto L_6717;
    else
        goto L_66f7;

L_66f7:
    if ((HIWORD(d2) > *(lpthClosest + 0x8)))
        goto L_6704;
    else
        goto L_66fc;

L_66fc:
    if ((LOWORD(d2) < *(lpthClosest + 0x6)))
        goto L_6717;
    else
        goto L_6704;

L_6704:
    FreeLpth(lpthClosest);
    goto L_674b;

L_6717:
    *(lpthClosest + 0x6) = (*(lpthClosest + 0x6) - LOWORD(d2));
    *(lpthClosest + 0x8) = (*(lpthClosest + 0x8) - HIWORD(d2));
    *(lpthClosest + 0xe) = (*(lpthClosest + 0xe) | (0x1 << iPlayer));
    *(lpthClosest + 0xa) = (*(lpthClosest + 0xa) | (0x1 << iPlayer));

L_674b:
    *(pdTravel) = dEnd;

L_6753:
    lpfl->fNoHeal = 0x1;
    return 0x0;

LDoNext:
    rgi[iType] = (rgi[iType] + 0x1);
    cFields = (cFields - 0x1);

L_6781:
    if ((cFields > 0x0))
        goto L_5675;
    else
        goto L_678b;

L_678b:
    return 0x1;
}
