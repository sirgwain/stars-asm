#include "common.h"

int32_t vrgAiPacketDist[2] = {7056, 50625};
uint8_t vrgSBAip[85] = {34, 35, 37, 0,  17, 10, 11, 38, 19, 36, 34, 9,  34, 0,  17, 36, 37, 19, 0,  9,  4,  10, 11, 19, 38, 19, 35, 11, 0,
                        37, 10, 19, 38, 20, 36, 20, 17, 36, 37, 19, 3,  34, 35, 11, 11, 37, 10, 19, 0,  20, 38, 34, 17, 19, 9,  20, 36, 19,
                        35, 37, 0,  17, 10, 11, 38, 19, 36, 19, 9,  34, 35, 11, 0,  37, 10, 19, 38, 20, 36, 34, 17, 36, 37, 19, 3};
uint8_t vrgSBMacAisb[6] = {17, 57, 25, 0, 69, 41};
AIPART  vrgAiParts[150] = {{.ibit = 5, .iItem = 7, .cItem = 8},
                           {.ibit = 5, .iItem = 11, .cItem = 4},
                           {.ibit = 4, .iItem = 18, .cItem = 1},
                           {.ibit = 4, .iItem = 20, .cItem = 1},
                           {.ibit = 4, .iItem = 13, .cItem = 1},
                           {.ibit = 4, .iItem = 7, .cItem = 1},
                           {.ibit = 4, .iItem = 23, .cItem = 1},
                           {.ibit = 4, .iItem = 17, .cItem = 1},
                           {.ibit = 4, .iItem = 11, .cItem = 1},
                           {.ibit = 4, .iItem = 5, .cItem = 1},
                           {.ibit = 4, .iItem = 22, .cItem = 1},
                           {.ibit = 4, .iItem = 15, .cItem = 1},
                           {.ibit = 4, .iItem = 9, .cItem = 1},
                           {.ibit = 4, .iItem = 3, .cItem = 1},
                           {.ibit = 4, .iItem = 1, .cItem = 1},
                           {.ibit = 4, .cItem = 1},
                           {.ibit = 4, .iItem = 16, .cItem = 1},
                           {.ibit = 4, .iItem = 10, .cItem = 1},
                           {.ibit = 4, .iItem = 4, .cItem = 1},
                           {.ibit = 4, .iItem = 21, .cItem = 1},
                           {.ibit = 4, .iItem = 14, .cItem = 1},
                           {.ibit = 4, .iItem = 8, .cItem = 1},
                           {.ibit = 4, .iItem = 2, .cItem = 1},
                           {.ibit = 4, .iItem = 19, .cItem = 1},
                           {.ibit = 4, .iItem = 12, .cItem = 1},
                           {.ibit = 4, .iItem = 6, .cItem = 1},
                           {.iItem = 15, .cItem = 1},
                           {.iItem = 8, .cItem = 1},
                           {.iItem = 14, .cItem = 5},
                           {.iItem = 2, .cItem = 1},
                           {.ibit = 3, .iItem = 11, .cItem = 1},
                           {.ibit = 3, .iItem = 9, .cItem = 1},
                           {.ibit = 3, .iItem = 10, .cItem = 1},
                           {.ibit = 3, .iItem = 7, .cItem = 1},
                           {.ibit = 3, .iItem = 8, .cItem = 1},
                           {.ibit = 3, .iItem = 6, .cItem = 7},
                           {.ibit = 2, .iItem = 9, .cItem = 2},
                           {.ibit = 2, .iItem = 6, .cItem = 1},
                           {.ibit = 2, .iItem = 7, .cItem = 1},
                           {.ibit = 2, .iItem = 3, .cItem = 1},
                           {.ibit = 2, .iItem = 4, .cItem = 1},
                           {.ibit = 2, .iItem = 5, .cItem = 1},
                           {.ibit = 2, .iItem = 2, .cItem = 3},
                           {.ibit = 11, .iItem = 7, .cItem = 3},
                           {.ibit = 11, .iItem = 4, .cItem = 1},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 12, .iItem = 10, .cItem = 1},
                           {.ibit = 12, .iItem = 8, .cItem = 2},
                           {.ibit = 11, .iItem = 4, .cItem = 1},
                           {.ibit = 12, .iItem = 8, .cItem = 2},
                           {.ibit = 11, .iItem = 3, .cItem = 1},
                           {.ibit = 12, .iItem = 10, .cItem = 1},
                           {.ibit = 11, .iItem = 2, .cItem = 2},
                           {.ibit = 11, .iItem = 13, .cItem = 6},
                           {.ibit = 12, .iItem = 6, .cItem = 2},
                           {.ibit = 12, .iItem = 10, .cItem = 1},
                           {.ibit = 11, .iItem = 13, .cItem = 6},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 12, .iItem = 6, .cItem = 2},
                           {.ibit = 12, .iItem = 4, .cItem = 3},
                           {.ibit = 3, .iItem = 9, .cItem = 1},
                           {.ibit = 3, .iItem = 11, .cItem = 12},
                           {.ibit = 12, .iItem = 8, .cItem = 2},
                           {.ibit = 12, .iItem = 10, .cItem = 1},
                           {.ibit = 12, .iItem = 6, .cItem = 2},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 11, .iItem = 7, .cItem = 3},
                           {.ibit = 11, .iItem = 13, .cItem = 6},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 11, .iItem = 4, .cItem = 4},
                           {.ibit = 11, .iItem = 7, .cItem = 3},
                           {.ibit = 6, .iItem = 8, .cItem = 1},
                           {.ibit = 6, .iItem = 4, .cItem = 5},
                           {.ibit = 6, .iItem = 8, .cItem = 1},
                           {.ibit = 6, .iItem = 9, .cItem = 1},
                           {.ibit = 6, .iItem = 14, .cItem = 5},
                           {.ibit = 6, .iItem = 8, .cItem = 1},
                           {.ibit = 6, .iItem = 14, .cItem = 5},
                           {.ibit = 6, .iItem = 4, .cItem = 5},
                           {.ibit = 6, .iItem = 9, .cItem = 1},
                           {.iItem = 15, .cItem = 1},
                           {.iItem = 10, .cItem = 1},
                           {.ibit = 8, .iItem = 3, .cItem = 4},
                           {.ibit = 1, .iItem = 12, .cItem = 1},
                           {.ibit = 1, .iItem = 14, .cItem = 1},
                           {.ibit = 1, .iItem = 8, .cItem = 1},
                           {.ibit = 1, .iItem = 6, .cItem = 1},
                           {.ibit = 1, .iItem = 7, .cItem = 1},
                           {.ibit = 1, .iItem = 9, .cItem = 1},
                           {.ibit = 1, .iItem = 4, .cItem = 1},
                           {.ibit = 1, .iItem = 14, .cItem = 1},
                           {.ibit = 1, .iItem = 5, .cItem = 1},
                           {.ibit = 1, .iItem = 6, .cItem = 7},
                           {.ibit = 7, .iItem = 6, .cItem = 7},
                           {.ibit = 7, .iItem = 7, .cItem = 1},
                           {.iItem = 9, .cItem = 2},
                           {.iItem = 10, .cItem = 1},
                           {.iItem = 13, .cItem = 3},
                           {.iItem = 6, .cItem = 4},
                           {.ibit = 12, .iItem = 1, .cItem = 2},
                           {.ibit = 8, .iItem = 9, .cItem = 3},
                           {.ibit = 4, .iItem = 18, .cItem = 1},
                           {.ibit = 9, .iItem = 15, .cItem = 9},
                           {.ibit = 11, .iItem = 4, .cItem = 4},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 11, .iItem = 7, .cItem = 3},
                           {.ibit = 5, .iItem = 11, .cItem = 1},
                           {.ibit = 5, .iItem = 6, .cItem = 1},
                           {.ibit = 5, .iItem = 10, .cItem = 1},
                           {.ibit = 5, .iItem = 5, .cItem = 6},
                           {.ibit = 4, .iItem = 23, .cItem = 2},
                           {.ibit = 4, .iItem = 17, .cItem = 1},
                           {.ibit = 4, .iItem = 15, .cItem = 1},
                           {.ibit = 4, .iItem = 11, .cItem = 1},
                           {.ibit = 4, .iItem = 9, .cItem = 1},
                           {.ibit = 4, .iItem = 5, .cItem = 1},
                           {.ibit = 4, .iItem = 3, .cItem = 1},
                           {.ibit = 4, .iItem = 1, .cItem = 2},
                           {.ibit = 2, .iItem = 6, .cItem = 1},
                           {.ibit = 2, .iItem = 9, .cItem = 10},
                           {.ibit = 4, .iItem = 20, .cItem = 1},
                           {.ibit = 4, .iItem = 13, .cItem = 1},
                           {.ibit = 4, .iItem = 7, .cItem = 1},
                           {.ibit = 4, .iItem = 5, .cItem = 1},
                           {.ibit = 4, .cItem = 1},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 11, .iItem = 4, .cItem = 4},
                           {.ibit = 12, .iItem = 1, .cItem = 1},
                           {.ibit = 3, .iItem = 9, .cItem = 1},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 12, .iItem = 8, .cItem = 2},
                           {.ibit = 12, .iItem = 10, .cItem = 1},
                           {.ibit = 12, .iItem = 6, .cItem = 2},
                           {.ibit = 7, .iItem = 6, .cItem = 4},
                           {.ibit = 7, .iItem = 6, .cItem = 2},
                           {.ibit = 7, .cItem = 1},
                           {.iItem = 15, .cItem = 5},
                           {.iItem = 2, .cItem = 1}};

int16_t FCreateAiShdef(int16_t ishdef, int16_t ihul, uint8_t *rgaip) {
    StringId ids;
    int32_t  grbitHull;
    int16_t  cItem;
    int16_t  ihs;
    HUL     *lphul;
    PART     part;
    SHDEF    shdef;
    uint16_t t_merge_0160_0001;

L_012c:
    memset(shdef, 0x0, 0x93);
    if ((ishdef < 0x0))
        goto L_015d;
    else
        goto L_0157;

L_0157:
    t_merge_0160_0001 = 0x4000;
    goto L_0160;

L_015d:
    t_merge_0160_0001 = 0x400;

L_0160:
    if ((FLookupPartX(&(part), t_merge_0160_0001, ihul) == 0x1))
        goto L_017b;
    else
        goto L_0175;

L_0175:
    return 0x0;

L_017b:
    lphul = part.pcom;
    shdef.hul = *(lphul);
    shdef.hul.ihuldef = lphul->ihuldef;
    ihs = 0x0;
    goto L_01b7;

L_01b3:
    ihs = (ihs + 0x1);

L_01b7:
    if ((ihs >= lphul->chs))
        goto L_0254;
    else
        goto L_01c9;

L_01c9:
    if ((FGetAIPart(rgaip[ihs], &(part)) == 0x0))
        goto L_024b;
    else
        goto L_01f1;

L_01f1:
    /* untranslated: HIWORD(part.hs) = (part.hs.iItem | ((((lphul->rgTech[part[12:0](rgcrPlrHistory[ihs])] >> 0x8) & 0xff) & 0xff) << 0x8)) */
    shdef.hul.rghs[ihs].grhst = part.hs.grhst;
    HIWORD(shdef.hul.rghs[ihs]) = HIWORD(part.hs);
    goto L_01b3;

L_024b:
    return 0x0;

L_0254:
    shdef.hul.chs = LOBYTE(ihs);
    shdef.fFree = 0x0;
    if ((ishdef >= 0x0))
        goto L_0287;
    else
        goto L_026f;

L_026f:
    shdefBuild = shdef;
    return 0x1;

L_0287:
    ids = idsUniverseDefinitionFileSeemsMissingCorrupt;
    grbitHull = (int32_t)((0x10000 << ihul));
    if (((LOWORD(grbitHull) & 0x780) != 0x0))
        goto L_02bc;
    else
        goto L_02b4;

L_02b4:
    if ((0x0 == 0x0))
        goto L_02c9;
    else
        goto L_02bc;

L_02bc:
    ids = idsLyingBastard;
    cItem = 0x10;
    goto L_03f3;

L_02c9:
    if (((LOWORD(grbitHull) & 0x40) != 0x0))
        goto L_02e5;
    else
        goto L_02dd;

L_02dd:
    if ((0x0 == 0x0))
        goto L_02f2;
    else
        goto L_02e5;

L_02e5:
    ids = idsScrapper;
    cItem = 0x10;
    goto L_03f3;

L_02f2:
    if (((LOWORD(grbitHull) & 0x70) != 0x0))
        goto L_030e;
    else
        goto L_0306;

L_0306:
    if ((0x0 == 0x0))
        goto L_031b;
    else
        goto L_030e;

L_030e:
    ids = idsEasterBunny;
    cItem = 0xa;
    goto L_03f3;

L_031b:
    if ((0x0 != 0x0))
        goto L_0337;
    else
        goto L_032f;

L_032f:
    if (((HIWORD(grbitHull) & 0xf) == 0x0))
        goto L_0344;
    else
        goto L_0337;

L_0337:
    ids = idsPidgeon;
    cItem = 0xc;
    goto L_03f3;

L_0344:
    if (((LOWORD(grbitHull) & 0xf) != 0x0))
        goto L_0360;
    else
        goto L_0358;

L_0358:
    if ((0x0 == 0x0))
        goto L_036d;
    else
        goto L_0360;

L_0360:
    ids = idsGlovebox;
    cItem = 0x8;
    goto L_03f3;

L_036d:
    if ((0x0 != 0x0))
        goto L_038a;
    else
        goto L_0382;

L_0382:
    if (((HIWORD(grbitHull) & 0x1f0) == 0x0))
        goto L_0397;
    else
        goto L_038a;

L_038a:
    ids = idsGroundHog;
    cItem = 0x8;
    goto L_03f3;

L_0397:
    if (((LOWORD(grbitHull) & 0x3800) != 0x0))
        goto L_03b3;
    else
        goto L_03ab;

L_03ab:
    if ((0x0 == 0x0))
        goto L_03c0;
    else
        goto L_03b3;

L_03b3:
    ids = idsPricklyPear;
    cItem = 0x8;
    goto L_03f3;

L_03c0:
    if (((LOWORD(grbitHull) & 0xc000) != 0x0))
        goto L_03dc;
    else
        goto L_03d4;

L_03d4:
    if ((0x0 == 0x0))
        goto L_03e9;
    else
        goto L_03dc;

L_03dc:
    ids = idsEgg;
    cItem = 0x8;
    goto L_03f3;

L_03e9:
    ids = idsZombie;
    cItem = 0x8;

L_03f3:
    shdef.wFlags = ((shdef.wFlags & 0x83ff) | ((ishdef & 0x1f) << 0xa));
    PickANameAndBmp(&(shdef), ids, cItem, lphul->ibmp);
    return FChangeAiShdef(&(shdef), ishdef);
}

int16_t FGetAIPart(int16_t aip, PART *ppart) {
    int16_t cTry;
    int16_t iOffset;
    int16_t i;
    int16_t cItem;
    PART    part;
    int16_t t_047d;
    int16_t t_04ee;

L_043e:
    iOffset = 0x0;
    i = 0x0;
    goto L_0465;

L_0454:
    iOffset = (iOffset + vrgcAiParts[i]);
    i = (i + 0x1);

L_0465:
    if ((i < aip))
        goto L_0454;
    else
        goto L_0470;

L_0470:
    cTry = vrgcAiParts[aip];

L_047d:
    t_047d = cTry;
    cTry = (cTry - 0x1);
    if ((t_047d <= 0x0))
        goto L_054d;
    else
        goto L_048c;

L_048c:
    /* untranslated: part.hs.grhst = (0x1 << (cs:[(iOffset * 0x2)] & 0xf)) */
    /* untranslated: part.hs.iItem = ((cs:[(iOffset * 0x2)] >> 0x4) & 0x1f) */
    part.hs.cItem = 0x0;
    /* untranslated: cItem = ((cs:[(iOffset * 0x2)] >> 0x9) & 0xf) */

L_04ee:
    t_04ee = cItem;
    cItem = (cItem - 0x1);
    if ((t_04ee <= 0x0))
        goto L_0546;
    else
        goto L_04fd;

L_04fd:
    if ((FLookupPart(&(part)) != 0x1))
        goto L_0526;
    else
        goto L_0511;

L_0511:
    *(ppart) = part;
    return 0x1;

L_0526:
    /* untranslated: ss:[bp-0x14] = ((HIWORD(part.hs) + 0xffff) & 0xff) */
    part.hs.iItem = 0x0;
    /* untranslated: HIWORD(part.hs) = (HIWORD(part.hs) | ss:[bp-0x14]) */
    goto L_04ee;

L_0546:
    iOffset = (iOffset + 0x1);
    goto L_047d;

L_054d:
    return 0x0;
}

void PickANameAndBmp(SHDEF *pshdef, StringId ids, int16_t cids, int16_t ibmpStart) {
    int16_t i;
    int16_t rgfBmpUsed[4];
    int16_t ishdef;

L_055a:
    memset(rgfBmpUsed, 0x0, 0x8);
    if ((pshdef->ishdef < 0x10))
        goto L_062c;
    else
        goto L_058d;

L_058d:
    ishdef = 0x0;
    goto L_0620;

L_0595:
    if ((((rglpshdefSB[idPlayer][ishdef].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_061c;
    else
        goto L_05c3;

L_05c3:
    if ((rglpshdefSB[idPlayer][ishdef].hul.ihuldef != pshdef->hul.ihuldef))
        goto L_061c;
    else
        goto L_05f0;

L_05f0:
    rgfBmpUsed[(rglpshdefSB[idPlayer][ishdef].hul.ibmp - ibmpStart)] = 0x1;

L_061c:
    ishdef = (ishdef + 0x1);

L_0620:
    if ((ishdef >= 0xa))
        goto L_0697;
    else
        goto L_0626;

L_0626:

L_062c:
    ishdef = 0x0;
    goto L_068e;

L_0634:
    if ((((rgshdef[ishdef].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_068a;
    else
        goto L_0652;

L_0652:
    if ((rgshdef[ishdef].hul.ihuldef != pshdef->hul.ihuldef))
        goto L_068a;
    else
        goto L_066e;

L_066e:
    rgfBmpUsed[(rgshdef[ishdef].hul.ibmp - ibmpStart)] = 0x1;

L_068a:
    ishdef = (ishdef + 0x1);

L_068e:
    if ((ishdef < 0x10))
        goto L_0634;
    else
        goto L_0697;

L_0697:
    i = 0x0;
    goto L_06b8;

L_069f:
    if ((rgfBmpUsed[i] == 0x0))
        goto L_06c1;
    else
        goto L_06ae;

L_06ae:

L_06b4:
    i = (i + 0x1);

L_06b8:
    if ((i < 0x4))
        goto L_069f;
    else
        goto L_06c1;

L_06c1:
    if ((i != 0x4))
        goto L_06d9;
    else
        goto L_06ca;

L_06ca:
    i = Random(0x4);

L_06d9:
    pshdef->hul.ibmp = (ibmpStart + i);
    i = 0x0;
    goto L_0850;

L_06ed:
    CchGetString((Random(cids) + ids), ((uint8_t *)(pshdef) + 0x8));
    if ((pshdef->ishdef < 0x10))
        goto L_07e0;
    else
        goto L_0723;

L_0723:
    ishdef = 0x0;
    goto L_07c8;

L_072b:
    if ((((rglpshdefSB[idPlayer][ishdef].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_07c4;
    else
        goto L_0759;

L_0759:
    if ((rglpshdefSB[idPlayer][ishdef].hul.ihuldef != pshdef->hul.ihuldef))
        goto L_07c4;
    else
        goto L_0786;

L_0786:
    if ((fstrcmp(((uint8_t *)(pshdef) + 0x8), &(rglpshdefSB[idPlayer][ishdef].hul.szClass)) == 0x0))
        goto L_07d1;
    else
        goto L_07be;

L_07be:

L_07c4:
    ishdef = (ishdef + 0x1);

L_07c8:
    if ((ishdef < 0xa))
        goto L_072b;
    else
        goto L_07d1;

L_07d1:
    if ((ishdef == 0xa))
        goto L_089c;
    else
        goto L_07d7;

L_07d7:

L_07e0:
    ishdef = 0x0;
    goto L_0837;

L_07e8:
    if ((((rgshdef[ishdef].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_0833;
    else
        goto L_0806;

L_0806:
    if ((strcmp(((uint8_t *)(pshdef) + 0x8), rgshdef[ishdef].hul.szClass) == 0x0))
        goto L_0840;
    else
        goto L_082d;

L_082d:

L_0833:
    ishdef = (ishdef + 0x1);

L_0837:
    if ((ishdef < 0x10))
        goto L_07e8;
    else
        goto L_0840;

L_0840:
    if ((ishdef == 0x10))
        goto L_089c;
    else
        goto L_0846;

L_0846:

L_084c:
    i = (i + 0x1);

L_0850:
    if ((i < 0x14))
        goto L_06ed;
    else
        goto L_0859;

L_0859:
    _wsprintf(((uint8_t *)(pshdef) + 0x8), "%s %d", PszGetCompressedString((Random(cids) + ids)), Random(0x64));

L_089c:
    return;
}

int16_t FChangeAiShdef(SHDEF *pshdef, int16_t ishdef) {
    SHDEF   *lpshdefBase;
    int16_t  iDir;
    int16_t  ishdefWork;
    SHDEF    shdef;
    uint16_t t_merge_099a_0001;

L_08a2:
    shdef = *(pshdef);
    shdef.wFlags = ((shdef.wFlags & 0x83ff) | ((ishdef & 0x1f) << 0xa));
    shdef.turn = game.turn;
    shdef.cExist = 0x0;
    shdef.cBuilt = 0x0;
    if ((ishdef < 0x10))
        goto L_091c;
    else
        goto L_08fa;

L_08fa:
    lpshdefBase = rglpshdefSB[idPlayer];
    ishdefWork = (ishdef + 0xfff0);
    goto L_0938;

L_091c:
    lpshdefBase = rglpshdef[idPlayer];
    ishdefWork = ishdef;

L_0938:
    UpdateShdefCost(&(shdef));
    if ((shdef.fFree == ((lpshdefBase[ishdefWork].wFlags >> 0x9) & 0x1)))
        goto L_0a16;
    else
        goto L_097e;

L_097e:
    if ((shdef.fFree == 0x0))
        goto L_0997;
    else
        goto L_0991;

L_0991:
    t_merge_099a_0001 = 0xffff;
    goto L_099a;

L_0997:
    t_merge_099a_0001 = 0x1;

L_099a:
    iDir = t_merge_099a_0001;
    if ((ishdef < 0x10))
        goto L_09f9;
    else
        goto L_09a6;

L_09a6:
    /* untranslated: ss:[bp-0xa0] = (((iDir * 0x1000) + part[4:2](rgplr[idPlayer])) & 0xf000) */
    /* untranslated: part[4:2](rgplr[idPlayer]) = (part[4:2](rgplr[idPlayer]) & 0xfff) */
    /* untranslated: part[4:2](rgplr[idPlayer]) = (part[4:2](rgplr[idPlayer]) | ss:[bp-0xa0]) */
    goto L_0a88;

L_09f9:
    rgplr[idPlayer].cShDef = (rgplr[idPlayer].cShDef + LOBYTE(iDir));

L_0a16:
    if ((((lpshdefBase[ishdefWork].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_0a88;
    else
        goto L_0a3a;

L_0a3a:
    lpshdefBase[ishdefWork].wFlags = ((lpshdefBase[ishdefWork].wFlags & 0xfdff) | 0x200);
    LogChangeShDef(&(lpshdefBase[ishdefWork]));

L_0a88:
    lpshdefBase[ishdefWork] = shdef;
    LogChangeShDef(&(shdef));

L_0aca:
    return 0x1;
}

int16_t XferAiSupply(GrobjClass grobjSrc, int16_t idSrc, GrobjClass grobjDst, int16_t idDst, int16_t iSupply, int16_t cQuan) {
    int16_t dChg;
    int16_t iT;
    int32_t cAvailable;

L_0ad0:
    if ((cQuan != 0x0))
        goto L_0ae8;
    else
        goto L_0ae2;

L_0ae2:
    return 0x0;

L_0ae8:
    if ((cQuan >= 0x0))
        goto L_0b1d;
    else
        goto L_0af1;

L_0af1:
    iT = grobjSrc;
    grobjSrc = grobjDst;
    grobjDst = iT;
    iT = idSrc;
    idSrc = idDst;
    idDst = iT;
    /* untranslated: cQuan = (cQuan neg 0x0) */

L_0b1d:
    cAvailable = ChgCargo(grobjSrc, idSrc, iSupply, 0x0, 0x0);
    if ((SIGNHIWORD(cQuan) < HIWORD(cAvailable)))
        goto L_0b62;
    else
        goto L_0b4c;

L_0b4c:
    if ((SIGNHIWORD(cQuan) > HIWORD(cAvailable)))
        goto L_0b59;
    else
        goto L_0b51;

L_0b51:
    if ((cQuan <= LOWORD(cAvailable)))
        goto L_0b62;
    else
        goto L_0b59;

L_0b59:
    cQuan = LOWORD(cAvailable);

L_0b62:
    if ((cQuan != 0x0))
        goto L_0b71;
    else
        goto L_0b6b;

L_0b6b:
    return 0x0;

L_0b71:
    dChg = LOWORD(ChgCargo(grobjDst, idDst, iSupply, (uint32_t)(cQuan), 0x0));
    if ((dChg == 0x0))
        goto L_0bb5;
    else
        goto L_0b98;

L_0b98:
    /* untranslated: call ChgCargo(grobjSrc, idSrc, iSupply, sext16to32((dChg neg 0x0)), 0x0) -> callresult(int32_t) */

L_0bb5:
    return dChg;
}

int16_t XferAiTroopers(int16_t idSrc, int16_t idDst, int16_t cQuan) {
    int32_t cAvailable;

L_0bc2:
    if ((cQuan != 0x0))
        goto L_0bda;
    else
        goto L_0bd4;

L_0bd4:
    return 0x0;

L_0bda:
    cAvailable = ChgCargo(grobjFleet, idSrc, 0x3, 0x0, 0x0);
    if ((SIGNHIWORD(cQuan) < HIWORD(cAvailable)))
        goto L_0c21;
    else
        goto L_0c0b;

L_0c0b:
    if ((SIGNHIWORD(cQuan) > HIWORD(cAvailable)))
        goto L_0c18;
    else
        goto L_0c10;

L_0c10:
    if ((cQuan <= LOWORD(cAvailable)))
        goto L_0c21;
    else
        goto L_0c18;

L_0c18:
    cQuan = LOWORD(cAvailable);

L_0c21:
    if ((cQuan != 0x0))
        goto L_0c30;
    else
        goto L_0c2a;

L_0c2a:
    return 0x0;

L_0c30:
    /* untranslated: call ChgCargo(grobjFleet, idSrc, 0x3, sext16to32((cQuan neg 0x0)), 0x0) -> callresult(int32_t) */
    ChgCargo(grobjPlanet, idDst, 0x3, (uint32_t)(cQuan), 0x0);
    return cQuan;
}

int16_t FColonizeAiFleet(FLEET *lpfl, int16_t idPlanet) {
    ORDER ord;

L_0c78:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    memset(ord, 0x0, 0x12);
    ord.pt.x = rgptPlan[idPlanet].x;
    ord.pt.y = rgptPlan[idPlanet].y;
    ord.grobj = grobjPlanet;
    ord.id = idPlanet;
    ord.grTask = grTaskColonize;
    ord.fValidTask = 0x1;
    ord.iWarp = IFindIdealWarp(lpfl, 0x1);
    if ((FMoveAiFleet(lpfl, &(ord), 0x0) != 0x0))
        goto L_0d3b;
    else
        goto L_0d35;

L_0d35:
    return 0x0;

L_0d3b:
    lpfl->fMark = 0x1;
    return 0x1;
}

int16_t FGotoWormholeAiFleet(FLEET *lpfl, THING *lpthWorm) {
    ORDER ord;

L_0d5c:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    memset(ord, 0x0, 0x12);
    ord.pt.x = lpthWorm->pt.x;
    ord.pt.y = lpthWorm->pt.y;
    ord.grobj = grobjThing;
    ord.id = lpthWorm->idFull;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x0;
    ord.iWarp = IFindIdealWarp(lpfl, 0x1);
    if ((FMoveAiFleet(lpfl, &(ord), 0x0) != 0x0))
        goto L_0e1e;
    else
        goto L_0e18;

L_0e18:
    return 0x0;

L_0e1e:
    lpfl->fMark = 0x1;
    return 0x1;
}

int16_t IdNearestColonizablePlanet(FLEET *lpflCol, THING **plpthWorm) {
    PLANET  *lpplMac;
    POINT    pt;
    int32_t  dy;
    int32_t  d2;
    PLANET  *lppl;
    int16_t  idBest;
    int16_t  ifl;
    FLEET   *lpfl;
    int16_t  i;
    int16_t  iVal;
    int32_t  dx;
    uint8_t *lpb;
    int32_t  d2Cur;

L_0e3e:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x0))
        goto L_0e9a;
    else
        goto L_0e66;

L_0e66:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x5))
        goto L_0e9a;
    else
        goto L_0e85;

L_0e85:
    iVal = 0x10;
    if ((fMarkedPlanets != 0x0))
        goto LFindNearest;
    else
        goto L_0e91;

L_0e91:

L_0e9a:
    iVal = 0x0;

L_0e9f:
    lpb = &(vlpbAiPlanet[0xf]);
    i = 0x0;
    goto L_0ecb;

L_0eba:
    i = (i + 0x1);
    lpb = (lpb + 0x10);

L_0ecb:
    if ((i >= game.cPlanMax))
        goto L_0ee2;
    else
        goto L_0ed6;

L_0ed6:
    *(lpb) = LOBYTE(iVal);
    goto L_0eba;

L_0ee2:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_0fc5;

L_0f0d:
    if ((lppl->iPlayer == 0xffff))
        goto L_0f39;
    else
        goto L_0f1a;

L_0f1a:
    if ((lppl->iPlayer != idPlayer))
        goto L_0f31;
    else
        goto L_0f29;

L_0f29:
    iVal = 0x1;
    goto L_0f9e;

L_0f31:
    iVal = 0x2;

L_0f39:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x0))
        goto L_0f99;
    else
        goto L_0f58;

L_0f58:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x5))
        goto L_0f99;
    else
        goto L_0f77;

L_0f77:
    if ((PctPlanetOptValue(lppl, idPlayer) >= 0x0))
        goto L_0f99;
    else
        goto L_0f91;

L_0f91:
    iVal = 0x8;
    goto L_0f9e;

L_0f99:
    iVal = 0x0;

L_0f9e:
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0xf))] = lobyte(iVal) */
    lppl = (lppl + 0x1);

L_0fc5:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_0f0d;
    else
        goto L_0fd3;

L_0fd3:
    ifl = 0x0;
    goto L_0fdf;

L_0fdb:
    ifl = (ifl + 0x1);

L_0fdf:
    if ((ifl >= cFleet))
        goto LFindNearest;
    else
        goto L_0fea;

L_0fea:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_101a;
    else
        goto L_1012;

L_1012:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto LFindNearest;
    else
        goto L_101a;

L_101a:
    if ((lpfl->iPlayer != idPlayer))
        goto L_0fdb;
    else
        goto L_1029;

L_1029:
    if ((lpfl->cord <= 0x1))
        goto L_0fdb;
    else
        goto L_1036;

L_1036:
    if ((LOWORD(lpfl) != LOWORD(lpflCol)))
        goto L_104c;
    else
        goto L_1044;

L_1044:
    if ((HIWORD(lpfl) == HIWORD(lpflCol)))
        goto L_0fdb;
    else
        goto L_104c;

L_104c:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_0fdb : L_1064 */

L_1064:

L_106a:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x0))
        goto L_10a8;
    else
        goto L_1089;

L_1089:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x5))
        goto L_10c1;
    else
        goto L_10a8;

L_10a8:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x1]) & 0xf) != 0x2 ? L_0fdb : L_10bb */

L_10bb:

L_10c1:
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lpfl->lpplord->rgord[0x1].id * 0x10) + 0xf))] = 0x4 */
    goto L_0fdb;

LFindNearest:
    lpb = &(vlpbAiPlanet[0xf]);
    d2 = 0x5f5e0ff;
    pt.x = lpflCol->pt.x;
    pt.y = lpflCol->pt.y;
    idBest = 0xffff;
    i = 0x0;
    goto L_1136;

L_1125:
    i = (i + 0x1);
    lpb = (lpb + 0x10);

L_1136:
    if ((i >= game.cPlanMax))
        goto L_123e;
    else
        goto L_1141;

L_1141:
    if ((*(lpb) > 0x0))
        goto L_1125;
    else
        goto L_114f;

L_114f:

L_1155:
    dx = (uint32_t)(abs((pt.x - rgptPlan[i].x)));
    dy = (uint32_t)(abs((pt.y - rgptPlan[i].y)));
    if ((HIWORD(dx) > HIWORD(d2)))
        goto L_1125;
    else
        goto L_11a3;

L_11a3:
    if ((HIWORD(dx) < HIWORD(d2)))
        goto L_11b0;
    else
        goto L_11a8;

L_11a8:
    if ((LOWORD(dx) >= LOWORD(d2)))
        goto L_1125;
    else
        goto L_11b0;

L_11b0:
    if ((HIWORD(dy) > HIWORD(d2)))
        goto L_1125;
    else
        goto L_11be;

L_11be:
    if ((HIWORD(dy) < HIWORD(d2)))
        goto L_11cb;
    else
        goto L_11c3;

L_11c3:
    if ((LOWORD(dy) >= LOWORD(d2)))
        goto L_1125;
    else
        goto L_11cb;

L_11cb:
    d2Cur = (uint32_t)((dx * dx));
    if ((HIWORD((uint32_t)((dx * dx))) > HIWORD(d2)))
        goto L_1125;
    else
        goto L_11ea;

L_11ea:
    if ((HIWORD((uint32_t)((dx * dx))) < HIWORD(d2)))
        goto L_11f7;
    else
        goto L_11ef;

L_11ef:
    if ((LOWORD((uint32_t)((dx * dx))) >= LOWORD(d2)))
        goto L_1125;
    else
        goto L_11f7;

L_11f7:
    d2Cur = (d2Cur + (uint32_t)((dy * dy)));
    if ((HIWORD(d2Cur) > HIWORD(d2)))
        goto L_1125;
    else
        goto L_121c;

L_121c:
    if ((HIWORD(d2Cur) < HIWORD(d2)))
        goto L_1229;
    else
        goto L_1221;

L_1221:
    if ((LOWORD(d2Cur) >= LOWORD(d2)))
        goto L_1125;
    else
        goto L_1229;

L_1229:
    d2 = d2Cur;
    idBest = i;

L_123e:
    fMarkedPlanets = 0x1;
    if ((plpthWorm == 0x0))
        goto L_12d9;
    else
        goto L_124d;

L_124d:
    *(plpthWorm) = 0x0;
    if ((lpflCol->idPlanet == 0xffff))
        goto L_12c0;
    else
        goto L_1266;

L_1266:
    if ((game.turn >= 0x78))
        goto L_12c0;
    else
        goto L_1270;

L_1270:
    lppl = LpplFromId(lpflCol->idPlanet);
    if ((LOWORD(lppl) != 0x0))
        goto L_1297;
    else
        goto L_128e;

L_128e:
    if ((HIWORD(lppl) == 0x0))
        goto L_12c0;
    else
        goto L_1297;

L_1297:
    if ((lppl->iPlayer != idPlayer))
        goto L_12c0;
    else
        goto L_12a6;

L_12a6:
    *(plpthWorm) = LpthWormFind(&(pt), d2);

L_12c0:
    if ((LOWORD(*(plpthWorm)) != 0x0))
        goto L_12d4;
    else
        goto L_12cb;

L_12cb:
    if ((HIWORD(*(plpthWorm)) == 0x0))
        goto L_12d9;
    else
        goto L_12d4;

L_12d4:
    idBest = 0xffff;

L_12d9:

L_12df:
    return idBest;
}

THING *LpthWormFind(POINT *ppt, int32_t d2) {
    int16_t  pctGood;
    int16_t  dy;
    int32_t  d2Worm;
    uint16_t grbitplr;
    THING   *lpth;
    THING   *lpthBest;
    int16_t  iVal;
    int16_t  dx;
    THING   *lpthMac;
    int32_t  d2Cur;

L_12e6:
    lpthBest = 0x0;
    grbitplr = (0x1 << idPlayer);
    d2Worm = 0xf4240;
    pctGood = 0x0;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_1491;

L_133f:
    if ((lpth->ith != ithWormhole))
        goto L_148d;
    else
        goto L_1352;

L_1352:

L_1358:
    dx = abs((ppt->x - lpth->pt.x));
    dy = abs((ppt->y - lpth->pt.y));
    d2Cur = (uint32_t)((LOWORD((dx * dx)) + LOWORD((dy * dy))));
    if ((HIWORD(d2Cur) > HIWORD((int32_t)((d2 * 0x4)))))
        goto L_148d;
    else
        goto L_13ba;

L_13ba:
    if ((HIWORD(d2Cur) < HIWORD((int32_t)((d2 * 0x4)))))
        goto L_13c7;
    else
        goto L_13bf;

L_13bf:
    if ((LOWORD(d2Cur) > LOWORD((int32_t)((d2 * 0x4)))))
        goto L_148d;
    else
        goto L_13c7;

L_13c7:
    if ((HIWORD(d2Cur) < 0x0))
        goto L_13e2;
    else
        goto L_13d0;

L_13d0:
    if ((HIWORD(d2Cur) > 0x0))
        goto L_148d;
    else
        goto L_13d5;

L_13d5:
    if ((LOWORD(d2Cur) > 0xb640))
        goto L_148d;
    else
        goto L_13dc;

L_13dc:

L_13e2:
    if (((lpth->thw.grbitPlrTrav & grbitplr) == 0x0))
        goto L_1416;
    else
        goto L_13f4;

L_13f4:
    iVal = PctWormholeMoves(lpth);
    iVal = (0x46 - LOWORD((0xa * iVal)));
    goto L_143e;

L_1416:
    if ((HIWORD(d2Cur) > HIWORD(d2)))
        goto L_1439;
    else
        goto L_1424;

L_1424:
    if ((HIWORD(d2Cur) < HIWORD(d2)))
        goto L_1431;
    else
        goto L_1429;

L_1429:
    if ((LOWORD(d2Cur) > LOWORD(d2)))
        goto L_1439;
    else
        goto L_1431;

L_1431:
    iVal = 0x5a;
    goto L_143e;

L_1439:
    iVal = 0x32;

L_143e:
    if ((iVal > pctGood))
        goto L_146f;
    else
        goto L_1449;

L_1449:
    if ((iVal != pctGood))
        goto L_148d;
    else
        goto L_1454;

L_1454:
    if ((HIWORD(d2Cur) > HIWORD(d2Worm)))
        goto L_148d;
    else
        goto L_1462;

L_1462:
    if ((HIWORD(d2Cur) < HIWORD(d2Worm)))
        goto L_146f;
    else
        goto L_1467;

L_1467:
    if ((LOWORD(d2Cur) >= LOWORD(d2Worm)))
        goto L_148d;
    else
        goto L_146f;

L_146f:
    pctGood = iVal;
    lpthBest = lpth;
    d2Worm = d2Cur;

L_148d:
    lpth = (lpth + 0x1);

L_1491:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_133f;
    else
        goto L_149f;

L_149f:
    if ((LOWORD(lpthBest) != 0x0))
        goto L_14b1;
    else
        goto L_14a8;

L_14a8:
    if ((HIWORD(lpthBest) == 0x0))
        goto L_14ce;
    else
        goto L_14b1;

L_14b1:
    if ((Random(0x64) >= pctGood))
        goto L_14ce;
    else
        goto L_14c5;

L_14c5:
    return lpthBest;

L_14ce:
    return 0x0;
}

uint32_t UlFleetPower(FLEET *lpfl) {
    uint32_t ul;
    int16_t  iplr;
    int16_t  ishdef;

L_14de:
    ul = 0x0;
    iplr = lpfl->iPlayer;
    ishdef = 0x0;
    goto L_158c;

L_1503:
    if ((lpfl->rgcsh[ishdef] == 0x0))
        goto L_1588;
    else
        goto L_1520;

L_1520:
    /* untranslated: ul = (ul + (uint32_t)(words(part[137:2](rglpshdef[iplr][ishdef]), part[135:2](rglpshdef[iplr][ishdef])) * sext16to32(lpfl->rgcsh[ishdef])))
     */
    if ((0x0 != 0x0))
        goto L_1595;
    else
        goto L_157d;

L_157d:
    if (((HIWORD(ul) & 0x8000) != 0x0))
        goto L_1595;
    else
        goto L_1582;

L_1582:

L_1588:
    ishdef = (ishdef + 0x1);

L_158c:
    if ((ishdef < 0x10))
        goto L_1503;
    else
        goto L_1595;

L_1595:

L_159e:
    return ul;
}

int16_t IdNearestUnknownPlanet(FLEET *lpfl, THING **plpthWorm) {
    POINT    pt;
    int32_t  dy;
    int32_t  d2;
    int16_t  idBest;
    int16_t  i;
    int32_t  dx;
    uint8_t *lpb;
    int32_t  d2Cur;

L_15a4:
    if ((fMarkedPlanets != 0x0))
        goto L_15c9;
    else
        goto L_15b7;

L_15b7:
    IdNearestColonizablePlanet(lpfl, 0x0);

L_15c9:
    lpb = &(vlpbAiPlanet[0xf]);
    d2 = 0x5f5e0ff;
    pt.x = lpfl->pt.x;
    pt.y = lpfl->pt.y;
    idBest = 0xffff;
    i = 0x0;
    goto L_1615;

L_1604:
    i = (i + 0x1);
    lpb = (lpb + 0x10);

L_1615:
    if ((i >= game.cPlanMax))
        goto L_171d;
    else
        goto L_1620;

L_1620:
    if ((*(lpb) != 0x10))
        goto L_1604;
    else
        goto L_162e;

L_162e:

L_1634:
    dx = (uint32_t)(abs((pt.x - rgptPlan[i].x)));
    dy = (uint32_t)(abs((pt.y - rgptPlan[i].y)));
    if ((HIWORD(dx) > HIWORD(d2)))
        goto L_1604;
    else
        goto L_1682;

L_1682:
    if ((HIWORD(dx) < HIWORD(d2)))
        goto L_168f;
    else
        goto L_1687;

L_1687:
    if ((LOWORD(dx) >= LOWORD(d2)))
        goto L_1604;
    else
        goto L_168f;

L_168f:
    if ((HIWORD(dy) > HIWORD(d2)))
        goto L_1604;
    else
        goto L_169d;

L_169d:
    if ((HIWORD(dy) < HIWORD(d2)))
        goto L_16aa;
    else
        goto L_16a2;

L_16a2:
    if ((LOWORD(dy) >= LOWORD(d2)))
        goto L_1604;
    else
        goto L_16aa;

L_16aa:
    d2Cur = (uint32_t)((dx * dx));
    if ((HIWORD((uint32_t)((dx * dx))) > HIWORD(d2)))
        goto L_1604;
    else
        goto L_16c9;

L_16c9:
    if ((HIWORD((uint32_t)((dx * dx))) < HIWORD(d2)))
        goto L_16d6;
    else
        goto L_16ce;

L_16ce:
    if ((LOWORD((uint32_t)((dx * dx))) >= LOWORD(d2)))
        goto L_1604;
    else
        goto L_16d6;

L_16d6:
    d2Cur = (d2Cur + (uint32_t)((dy * dy)));
    if ((HIWORD(d2Cur) > HIWORD(d2)))
        goto L_1604;
    else
        goto L_16fb;

L_16fb:
    if ((HIWORD(d2Cur) < HIWORD(d2)))
        goto L_1708;
    else
        goto L_1700;

L_1700:
    if ((LOWORD(d2Cur) >= LOWORD(d2)))
        goto L_1604;
    else
        goto L_1708;

L_1708:
    d2 = d2Cur;
    idBest = i;

L_171d:
    if ((plpthWorm == 0x0))
        goto L_1786;
    else
        goto L_1726;

L_1726:
    *(plpthWorm) = 0x0;
    if ((lpfl->idPlanet == 0xffff))
        goto L_176d;
    else
        goto L_173f;

L_173f:
    if ((Random(0x64) >= 0x5))
        goto L_176d;
    else
        goto L_1753;

L_1753:
    *(plpthWorm) = LpthWormFind(&(pt), d2);

L_176d:
    if ((LOWORD(*(plpthWorm)) != 0x0))
        goto L_1781;
    else
        goto L_1778;

L_1778:
    if ((HIWORD(*(plpthWorm)) == 0x0))
        goto L_1786;
    else
        goto L_1781;

L_1781:
    idBest = 0xffff;

L_1786:

L_178c:
    return idBest;
}

void AddMinesToBlockedQueues() {
    PROD    prod;
    int32_t cMaxBuild;
    int16_t etaBetterAlchemy;
    int32_t cBuild;
    int16_t etaFirst;
    PLANET *lppl;
    int32_t cResMine;
    int32_t cRes;
    int16_t ipl;
    int32_t rgCost[4];
    PROD    rgprod[64];
    int16_t etaBetterMines;

L_1792:
    ipl = 0x0;
    goto L_17a7;

L_17a3:
    ipl = (ipl + 0x1);

L_17a7:
    if ((ipl >= vclpplAi))
        goto L_1cef;
    else
        goto L_17b2;

L_17b2:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_17e2;
    else
        goto L_17da;

L_17da:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_1cef;
    else
        goto L_17e2;

L_17e2:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_17fc;
    else
        goto L_17ef;

L_17ef:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_17a3;
    else
        goto L_17f6;

L_17f6:

L_17fc:
    prod = lppl->lpplprod->rgprod[0x0];
    if ((prod.grobj != 0x1))
        goto L_18c8;
    else
        goto L_182d;

L_182d:
    if ((0x0 != 0x0))
        goto L_18c8;
    else
        goto L_1835;

L_1835:
    if ((prod.iItem != 0x8))
        goto L_1859;
    else
        goto L_1851;

L_1851:
    if ((0x0 == 0x0))
        goto L_17a3;
    else
        goto L_1859;

L_1859:
    if ((prod.iItem != 0x3))
        goto L_187d;
    else
        goto L_1875;

L_1875:
    if ((0x0 == 0x0))
        goto L_17a3;
    else
        goto L_187d;

L_187d:
    if ((prod.iItem != 0xb))
        goto L_18a1;
    else
        goto L_1899;

L_1899:
    if ((0x0 == 0x0))
        goto L_17a3;
    else
        goto L_18a1;

L_18a1:
    if ((prod.iItem != 0xc))
        goto L_18c8;
    else
        goto L_18bd;

L_18bd:
    if ((0x0 == 0x0))
        goto L_17a3;
    else
        goto L_18c2;

L_18c2:

L_18c8:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    PszProductionETA(&(sel.pl), sel.pl.lpplprod, iobjMine, &(etaFirst), 0x0);
    if ((etaFirst == 0x1))
        goto L_17a3;
    else
        goto L_1903;

L_1903:

L_1909:
    if ((etaFirst != 0xffff))
        goto L_1917;
    else
        goto L_1912;

L_1912:
    etaFirst = 0x258;

L_1917:
    GetProductionCosts(lppl, &(prod), rgCost, idPlayer, 0x1);
    cRes = (uint32_t)(CResourcesAtPlanet(&(sel.pl), idPlayer));
    if ((sel.pl.fNoResearch != 0x0))
        goto L_19aa;
    else
        goto L_196f;

L_196f:
    if ((0x0 != 0x0))
        goto L_19aa;
    else
        goto L_1977;

L_1977:
    cRes = (cRes - (int32_t)(((uint32_t)((cRes * (uint32_t)((uint16_t)(rgplr[idPlayer].pctResearch)))) / 0x64)));

L_19aa:
    if ((HIWORD(rgCost[0x3]) < HIWORD((uint32_t)((cRes * (uint32_t)((etaFirst + 0xffff)))))))
        goto L_19d6;
    else
        goto L_19c6;

L_19c6:
    if ((HIWORD(rgCost[0x3]) > HIWORD((uint32_t)((cRes * (uint32_t)((etaFirst + 0xffff)))))))
        goto L_17a3;
    else
        goto L_19cb;

L_19cb:
    if ((LOWORD(rgCost[0x3]) > LOWORD((uint32_t)((cRes * (uint32_t)((etaFirst + 0xffff)))))))
        goto L_17a3;
    else
        goto L_19d0;

L_19d0:

L_19d6:
    /* untranslated: ss:[bp-0x136] = sel.pl.cMines */
    /* untranslated: ss:[bp-0x134] = 0x0 */
    CMaxOperableMines(&(sel.pl), idPlayer, 0x1);
    /* untranslated: LOWORD(cMaxBuild) = (callresult(int16_t) - ss:[bp-0x136]) */
    /* untranslated: HIWORD(cMaxBuild) = (signhiword(callresult(int16_t)) - ss:[bp-0x134]) */
    if ((HIWORD(cMaxBuild) > 0x0))
        goto L_1a3e;
    else
        goto L_1a26;

L_1a26:
    if ((HIWORD(cMaxBuild) < 0x0))
        goto L_1a34;
    else
        goto L_1a2b;

L_1a2b:
    if ((LOWORD(cMaxBuild) >= 0x0))
        goto L_1a3e;
    else
        goto L_1a34;

L_1a34:
    cMaxBuild = 0x0;

L_1a3e:
    cResMine = (uint32_t)(GetRaceStat(rgplr[idPlayer], rsMineBuild));
    if ((HIWORD((uint32_t)((cResMine * cMaxBuild))) > HIWORD(cRes)))
        goto L_1a93;
    else
        goto L_1a77;

L_1a77:
    if ((HIWORD((uint32_t)((cResMine * cMaxBuild))) < HIWORD(cRes)))
        goto L_1a84;
    else
        goto L_1a7c;

L_1a7c:
    if ((LOWORD((uint32_t)((cResMine * cMaxBuild))) > LOWORD(cRes)))
        goto L_1a93;
    else
        goto L_1a84;

L_1a84:
    cBuild = cMaxBuild;
    goto L_1aaa;

L_1a93:
    cBuild = (int32_t)((cRes / cResMine));

L_1aaa:
    InitProduction(rgprod);
    if ((HIWORD(cBuild) < 0x0))
        goto L_1b74;
    else
        goto L_1ac0;

L_1ac0:
    if ((HIWORD(cBuild) > 0x0))
        goto L_1ace;
    else
        goto L_1ac5;

L_1ac5:
    if ((LOWORD(cBuild) <= 0x0))
        goto L_1b74;
    else
        goto L_1ace;

L_1ace:
    AddItemToQueue(0x8, LOWORD(cBuild), grobjPlanet, 0x0);
    FinishProduction(0x1);
    PszProductionETA(&(sel.pl), sel.pl.lpplprod, iobjFactory, &(etaBetterMines), 0x0);
    if ((etaBetterMines != 0xffff))
        goto L_1b29;
    else
        goto L_1b23;

L_1b23:
    etaBetterMines = 0x2bc;

L_1b29:
    LOWORD(sel.pl.lpplprod->rgprod[0x0]) = ((LOWORD(sel.pl.lpplprod->rgprod[0x0]) & 0xfc00) | 0x1);
    HIWORD(sel.pl.lpplprod->rgprod[0x0]) = ((HIWORD(sel.pl.lpplprod->rgprod[0x0]) & 0xffff) | 0x0);
    LOWORD(sel.pl.lpplprod->rgprod[0x0]) = ((LOWORD(sel.pl.lpplprod->rgprod[0x0]) & 0x3ff) | 0xc00);
    HIWORD(sel.pl.lpplprod->rgprod[0x0]) = ((HIWORD(sel.pl.lpplprod->rgprod[0x0]) & 0xfffe) | 0x0);
    goto L_1b9e;

L_1b74:
    etaBetterMines = 0x2bc;
    AddItemToQueue(0x3, 0x1, grobjPlanet, 0x0);
    FinishProduction(0x1);

L_1b9e:
    PszProductionETA(&(sel.pl), sel.pl.lpplprod, iobjFactory, &(etaBetterAlchemy), 0x0);
    if ((etaBetterAlchemy != 0xffff))
        goto L_1bcf;
    else
        goto L_1bca;

L_1bca:
    etaBetterAlchemy = 0x2bc;

L_1bcf:
    if ((etaBetterAlchemy >= etaFirst))
        goto L_1be6;
    else
        goto L_1bda;

L_1bda:
    if ((etaBetterAlchemy < etaBetterMines))
        goto L_17a3;
    else
        goto L_1be6;

L_1be6:
    if ((HIWORD(cBuild) > 0x0))
        goto L_1c03;
    else
        goto L_1bef;

L_1bef:
    if ((HIWORD(cBuild) < 0x0))
        goto L_17a3;
    else
        goto L_1bf4;

L_1bf4:
    if ((LOWORD(cBuild) < 0x1))
        goto L_17a3;
    else
        goto L_1bfa;

L_1bfa:

L_1c03:
    LOWORD(sel.pl.lpplprod->rgprod[0x0]) = ((LOWORD(sel.pl.lpplprod->rgprod[0x0]) & 0x3ff) | 0x2000);
    HIWORD(sel.pl.lpplprod->rgprod[0x0]) = ((HIWORD(sel.pl.lpplprod->rgprod[0x0]) & 0xfffe) | 0x0);
    if ((etaFirst < etaBetterMines))
        goto L_1c4a;
    else
        goto L_1c33;

L_1c33:
    if ((HIWORD(cBuild) > 0x0))
        goto L_1c8f;
    else
        goto L_1c3c;

L_1c3c:
    if ((HIWORD(cBuild) < 0x0))
        goto L_1c4a;
    else
        goto L_1c41;

L_1c41:
    if ((LOWORD(cBuild) > 0x0))
        goto L_1c8f;
    else
        goto L_1c4a;

L_1c4a:
    sel.pl.lpplprod->iprodMac = (sel.pl.lpplprod->iprodMac - 0x1);
    fmemmove(&(sel.pl.lpplprod->rgprod), &(sel.pl.lpplprod->rgprod[0x1]), (sel.pl.lpplprod->iprodMac * 0x4));
    goto L_17a3;

L_1c8f:
    /* untranslated: ss:[bp-0x134] = 0x0 */
    /* untranslated: LOWORD(sel.pl.lpplprod->rgprod[0x0]) = ((LOWORD(sel.pl.lpplprod->rgprod[0x0]) & 0xfc00) | loword((int32_t)(words((LOWORD(cBuild) & 0x3ff),
     * 0x0) << 0x0))) */
    /* untranslated: HIWORD(sel.pl.lpplprod->rgprod[0x0]) = ((HIWORD(sel.pl.lpplprod->rgprod[0x0]) & 0xffff) | hiword((int32_t)(words((LOWORD(cBuild) & 0x3ff),
     * 0x0) << 0x0))) */
    goto L_17a3;

L_1cef:
    return;
}

int16_t FFleetInField(FLEET *lpfl, THING *lpth) {
    int16_t dy;
    int16_t dx;
    int32_t dxy2;

L_1cf6:
    dx = abs((lpfl->pt.x - lpth->pt.x));
    dy = abs((lpfl->pt.y - lpth->pt.y));
    dxy2 = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(dxy2) > *(lpth + 0x8)))
        goto L_1d91;
    else
        goto L_1d7e;

L_1d7e:
    if ((HIWORD(dxy2) < *(lpth + 0x8)))
        goto L_1d8b;
    else
        goto L_1d83;

L_1d83:
    if ((LOWORD(dxy2) >= *(lpth + 0x6)))
        goto L_1d91;
    else
        goto L_1d8b;

L_1d8b:
    return 0x1;

L_1d91:
    return 0x0;
}

void SetAiFleetIdealSpeed(FLEET *lpfl, int16_t wtFuelMax, int16_t cMinefields, THING **rglpth) {
    THING  *lpth;
    int16_t i;
    int16_t j;
    int16_t ith;
    THING  *lpthMac;
    int16_t fMinefield;

L_1d9e:
    fMinefield = 0x0;
    if ((cMinefields == 0x0))
        goto LSelectFleet;
    else
        goto L_1db5;

L_1db5:
    if ((cMinefields != 0xffff))
        goto L_1e63;
    else
        goto L_1dbe;

L_1dbe:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_1e52;

L_1de9:
    if ((lpth->ith != ithMinefield))
        goto L_1e4e;
    else
        goto L_1dff;

L_1dff:
    if ((lpth->iplr == idPlayer))
        goto L_1e4e;
    else
        goto L_1e18;

L_1e18:
    if ((lpth->thm.iType == 0x2))
        goto L_1e4e;
    else
        goto L_1e2a;

L_1e2a:
    if ((FFleetInField(lpfl, lpth) == 0x0))
        goto L_1e4e;
    else
        goto L_1e46;

L_1e46:
    fMinefield = 0x1;
    goto LSelectFleet;

L_1e4e:
    lpth = (lpth + 0x1);

L_1e52:
    if ((LOWORD(lpth) >= LOWORD(lpthMac)))
        goto LSelectFleet;
    else
        goto L_1e5d;

L_1e5d:

L_1e63:
    ith = 0x0;
    goto L_1eb5;

L_1e6b:
    if ((FFleetInField(lpfl, rglpth[ith]) == 0x0))
        goto L_1eb1;
    else
        goto L_1e92;

L_1e92:
    lpth = rglpth[ith];
    fMinefield = 0x1;
    goto LSelectFleet;

L_1eb1:
    ith = (ith + 0x1);

L_1eb5:
    if ((ith < cMinefields))
        goto L_1e6b;
    else
        goto LSelectFleet;

LSelectFleet:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((fMinefield == 0x0))
        goto L_1f82;
    else
        goto L_1edb;

L_1edb:
    goto L_1f3b;

L_1ee8:
    i = 0x6;
    goto L_1f4e;

L_1ef0:
    i = 0x5;
    j = Random(0xa);
    if ((j < 0x7))
        goto L_1f4e;
    else
        goto L_1f0d;

L_1f0d:
    i = (i + 0x1);

L_1f14:
    i = 0x4;
    j = Random(0xa);
    if ((j < 0x4))
        goto L_1f4e;
    else
        goto L_1f31;

L_1f31:
    i = (i + 0x1);

L_1f3b:
    if ((*(lpth + 0xc) == 0x1))
        goto L_1ee8;
    else
        goto L_1f43;

L_1f43:
    if ((*(lpth + 0xc) != 0x2))
        goto L_1f14;
    else
        goto L_1f48;

L_1f48:

L_1f4e:
    GetRaceStat(rgplr[idPlayer], rsMajorAdv);
    goto L_1f74;

L_1f6a:
    i = (i + 0x1);
    goto WarpSet;

L_1f74:
    /* untranslated: branch callresult(RaceAttribute) != raStealth ? L_1fa5 : L_1f79 */

L_1f79:

L_1f82:
    i = IWarpBestForWaypoint(lpfl, &(lpfl->lpplord->rgord[0x1]));

WarpSet:
    /* untranslated: branch i == ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x4) & 0xf) ? L_1ff7 : L_1fc3 */

L_1fc3:
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x1]) = ((part[6:2](sel.fl.lpplord->rgord[0x1]) & 0xff0f) | ((i & 0xf) << 0x4)) */
    FLookupFleet(0xffff, &(sel.fl));

L_1ff7:
    return;
}

int16_t IdTargetAttack(FLEET *lpfl, FLEET *lpflAtk, FLEET *lpflEnemy, int16_t fOnlyHumans) {
    FLEET   *lpflClosest;
    FLEET   *lpflT;
    int32_t  lDistBest;
    PLANET  *lpplMac;
    POINT    pt;
    int16_t  dy;
    PLANET  *lppl;
    int32_t  lDist;
    int16_t  idClosest;
    int16_t  i;
    int16_t  cShipsAtk;
    int16_t  cShipsDst;
    PLANET  *lpplClosest;
    uint8_t *lpb;
    int16_t  dx;
    FLEET   *lpflAtk2;
    ORDER    ord;

L_1ffe:
    cShipsDst = 0x0;
    cShipsAtk = 0x0;
    i = 0x0;
    goto L_2037;

L_2019:
    cShipsAtk = (cShipsAtk + lpfl->rgcsh[i]);
    i = (i + 0x1);

L_2037:
    if ((i < 0x10))
        goto L_2019;
    else
        goto L_2040;

L_2040:
    lpflClosest = 0x0;
    lpflT = lpflEnemy;
    lDistBest = 0xf4240;
    pt.x = lpfl->pt.x;
    pt.y = lpfl->pt.y;
    memset(ord, 0x0, 0x12);
    goto L_2248;

L_2088:
    if ((fOnlyHumans == 0x0))
        goto L_20b6;
    else
        goto L_2091;

L_2091:
    if ((((rgplr[lpflT->iPlayer].wMdPlr >> 0x9) & 0x1) != 0x0))
        goto NextTarg;
    else
        goto L_20b0;

L_20b0:

L_20b6:
    lpflAtk2 = lpflAtk;
    cShipsDst = 0x0;
    goto L_215e;

L_20ca:
    if ((LOWORD(lpflAtk2) != LOWORD(lpfl)))
        goto L_20e0;
    else
        goto L_20d8;

L_20d8:
    if ((HIWORD(lpflAtk2) == HIWORD(lpfl)))
        goto L_214d;
    else
        goto L_20e0;

L_20e0:
    if ((lpflAtk2->cord < 0x2))
        goto L_214d;
    else
        goto L_20ed;

L_20ed:
    if ((lpflAtk2->lpplord->rgord[0x1].id != lpflT->id))
        goto L_214d;
    else
        goto L_2103;

L_2103:
    /* untranslated: branch ((part[6:2](lpflAtk2->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x2 ? L_214d : L_211e */

L_211e:
    i = 0x0;
    goto L_2144;

L_2126:
    cShipsDst = (cShipsDst + lpflAtk2->rgcsh[i]);
    i = (i + 0x1);

L_2144:
    if ((i < 0x10))
        goto L_2126;
    else
        goto L_214d;

L_214d:
    lpflAtk2 = lpflAtk2->lpflNext;

L_215e:
    if ((LOWORD(lpflAtk2) != 0x0))
        goto L_20ca;
    else
        goto L_2167;

L_2167:
    if ((HIWORD(lpflAtk2) != 0x0))
        goto L_20ca;
    else
        goto L_2170;

L_2170:
    if ((cShipsDst <= 0x0))
        goto L_218d;
    else
        goto L_2179;

L_2179:
    if ((Random(0x3) == 0x0))
        goto NextTarg;
    else
        goto L_218d;

L_218d:
    if ((LOWORD((0x5 * cShipsDst)) <= cShipsAtk))
        goto L_21b2;
    else
        goto L_219b;

L_219b:
    if ((Random(0xf) == 0x0))
        goto NextTarg;
    else
        goto L_21ac;

L_21ac:

L_21b2:
    dx = (lpflT->pt.x - pt.x);
    dy = (lpflT->pt.y - pt.y);
    lDist = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lDist) > HIWORD(lDistBest)))
        goto NextTarg;
    else
        goto L_2212;

L_2212:
    if ((HIWORD(lDist) < HIWORD(lDistBest)))
        goto L_221f;
    else
        goto L_2217;

L_2217:
    if ((LOWORD(lDist) >= LOWORD(lDistBest)))
        goto NextTarg;
    else
        goto L_221f;

L_221f:
    lDistBest = lDist;
    lpflClosest = lpflT;

NextTarg:
    lpflT = lpflT->lpflNext;

L_2248:
    if ((LOWORD(lpflT) != 0x0))
        goto L_2088;
    else
        goto L_2251;

L_2251:
    if ((HIWORD(lpflT) != 0x0))
        goto L_2088;
    else
        goto L_225a;

L_225a:
    if ((HIWORD(lDistBest) > 0x0))
        goto L_229b;
    else
        goto L_2263;

L_2263:
    if ((HIWORD(lDistBest) < 0x0))
        goto L_2272;
    else
        goto L_2268;

L_2268:
    if ((LOWORD(lDistBest) >= 0x7e90))
        goto L_229b;
    else
        goto L_2272;

L_2272:
    ord.pt.x = lpflClosest->pt.x;
    ord.pt.y = lpflClosest->pt.y;
    ord.grobj = grobjFleet;
    ord.id = lpflClosest->id;
    goto ThwakSumthin;

L_229b:
    if ((LOWORD(lpflClosest) != 0x0))
        goto L_22ad;
    else
        goto L_22a4;

L_22a4:
    if ((HIWORD(lpflClosest) == 0x0))
        goto L_24f6;
    else
        goto L_22ad;

L_22ad:
    if ((HIWORD(lpfl->rgwtMin[0x4]) > HIWORD((int32_t)((LGetFleetStat(lpfl, 0x1) / 0x2)))))
        goto L_2308;
    else
        goto L_22da;

L_22da:
    /* untranslated: branch HIWORD(lpfl->rgwtMin[0x4]) < hiword((int32_t)(callresult(int32_t) / 0x2)) ? L_22e8 : L_22df */

L_22df:
    /* untranslated: branch LOWORD(lpfl->rgwtMin[0x4]) >= loword((int32_t)(callresult(int32_t) / 0x2)) ? L_2308 : L_22e8 */

L_22e8:
    if ((FMoveToNearestStarbase(lpfl, 0x0) == 0x0))
        goto L_2308;
    else
        goto L_2302;

L_2302:
    return 0x0;

L_2308:
    pt.x = lpflClosest->pt.x;
    pt.y = lpflClosest->pt.y;
    lpplClosest = 0x0;
    lDistBest = 0xf4240;
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_2491;

L_2358:
    lpflAtk2 = lpflAtk;
    goto L_23cf;

L_2367:
    if ((LOWORD(lpflAtk2) != LOWORD(lpfl)))
        goto L_237d;
    else
        goto L_2375;

L_2375:
    if ((HIWORD(lpflAtk2) == HIWORD(lpfl)))
        goto L_23be;
    else
        goto L_237d;

L_237d:
    if ((lpflAtk2->cord < 0x2))
        goto L_23be;
    else
        goto L_238a;

L_238a:
    if ((lpflAtk2->lpplord->rgord[0x1].id != lppl->id))
        goto L_23be;
    else
        goto L_23a0;

L_23a0:
    /* untranslated: branch ((part[6:2](lpflAtk2->lpplord->rgord[0x1]) >> 0x8) & 0xf) == 0x1 ? L_23e1 : L_23b8 */

L_23b8:

L_23be:
    lpflAtk2 = lpflAtk2->lpflNext;

L_23cf:
    if ((LOWORD(lpflAtk2) != 0x0))
        goto L_2367;
    else
        goto L_23d8;

L_23d8:
    if ((HIWORD(lpflAtk2) != 0x0))
        goto L_2367;
    else
        goto L_23e1;

L_23e1:
    if ((LOWORD(lpflAtk2) != 0x0))
        goto L_248d;
    else
        goto L_23ea;

L_23ea:
    if ((HIWORD(lpflAtk2) != 0x0))
        goto L_248d;
    else
        goto L_23f0;

L_23f0:

L_23f6:
    dx = (rgptPlan[lppl->id].x - pt.x);
    dy = (rgptPlan[lppl->id].y - pt.y);
    lDist = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lDist) > HIWORD(lDistBest)))
        goto L_248d;
    else
        goto L_2468;

L_2468:
    if ((HIWORD(lDist) < HIWORD(lDistBest)))
        goto L_2475;
    else
        goto L_246d;

L_246d:
    if ((LOWORD(lDist) >= LOWORD(lDistBest)))
        goto L_248d;
    else
        goto L_2475;

L_2475:
    lDistBest = lDist;
    lpplClosest = lppl;

L_248d:
    lppl = (lppl + 0x1);

L_2491:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_2358;
    else
        goto L_249f;

L_249f:
    if ((LOWORD(lpplClosest) != 0x0))
        goto L_24b1;
    else
        goto L_24a8;

L_24a8:
    if ((HIWORD(lpplClosest) == 0x0))
        goto L_2520;
    else
        goto L_24b1;

L_24b1:
    if ((lpplClosest->id == lpfl->idPlanet))
        goto L_2520;
    else
        goto L_24c3;

L_24c3:
    ord.pt.x = rgptPlan[lpplClosest->id].x;
    ord.pt.y = rgptPlan[lpplClosest->id].y;
    ord.grobj = grobjPlanet;
    ord.id = lpplClosest->id;
    goto ThwakSumthin;

L_24f6:
    if ((fOnlyHumans == 0x0))
        goto L_2520;
    else
        goto L_24ff;

L_24ff:
    return IdTargetAttack(lpfl, lpflAtk, lpflEnemy, 0x0);

L_2520:
    pt.x = lpfl->pt.x;
    pt.y = lpfl->pt.y;
    lpplClosest = 0x0;
    lDistBest = 0xf4240;
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_262a;

L_2570:
    if ((lppl->iPlayer == 0xffff))
        goto L_2626;
    else
        goto L_257d;

L_257d:
    if ((lppl->iPlayer == idPlayer))
        goto L_2626;
    else
        goto L_2589;

L_2589:

L_258f:
    dx = (rgptPlan[lppl->id].x - pt.x);
    dy = (rgptPlan[lppl->id].y - pt.y);
    lDist = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lDist) > HIWORD(lDistBest)))
        goto L_2626;
    else
        goto L_2601;

L_2601:
    if ((HIWORD(lDist) < HIWORD(lDistBest)))
        goto L_260e;
    else
        goto L_2606;

L_2606:
    if ((LOWORD(lDist) >= LOWORD(lDistBest)))
        goto L_2626;
    else
        goto L_260e;

L_260e:
    lDistBest = lDist;
    lpplClosest = lppl;

L_2626:
    lppl = (lppl + 0x1);

L_262a:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_2570;
    else
        goto L_2638;

L_2638:
    if ((LOWORD(lpplClosest) != 0x0))
        goto L_264a;
    else
        goto L_2641;

L_2641:
    if ((HIWORD(lpplClosest) == 0x0))
        goto L_268c;
    else
        goto L_264a;

L_264a:
    if ((lpplClosest->id == lpfl->idPlanet))
        goto L_268c;
    else
        goto L_265c;

L_265c:
    ord.pt.x = rgptPlan[lpplClosest->id].x;
    ord.pt.y = rgptPlan[lpplClosest->id].y;
    ord.grobj = grobjPlanet;
    ord.id = lpplClosest->id;
    goto ThwakSumthin;

L_268c:
    idClosest = 0xffff;
    lDistBest = 0xf4240;
    lpb = &(vlpbAiPlanet[0x9]);
    i = 0x0;
    goto L_26c7;

L_26b6:
    i = (i + 0x1);
    lpb = (lpb + 0x10);

L_26c7:
    if ((i >= game.cPlanMax))
        goto L_2774;
    else
        goto L_26d2;

L_26d2:
    if ((*(lpb) > 0x0))
        goto L_26b6;
    else
        goto L_26e0;

L_26e0:

L_26e6:
    dx = (rgptPlan[i].x - pt.x);
    dy = (rgptPlan[i].y - pt.y);
    lDist = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lDist) > HIWORD(lDistBest)))
        goto L_26b6;
    else
        goto L_2752;

L_2752:
    if ((HIWORD(lDist) < HIWORD(lDistBest)))
        goto L_275f;
    else
        goto L_2757;

L_2757:
    if ((LOWORD(lDist) >= LOWORD(lDistBest)))
        goto L_26b6;
    else
        goto L_275f;

L_275f:
    lDistBest = lDist;
    idClosest = i;

L_2774:
    if ((idClosest == 0xffff))
        goto L_27a7;
    else
        goto L_277d;

L_277d:
    ord.pt.x = rgptPlan[idClosest].x;
    ord.pt.y = rgptPlan[idClosest].y;
    ord.grobj = grobjPlanet;
    ord.id = idClosest;
    goto ThwakSumthin;

L_27a7:
    ord.id = Random(game.cPlanMax);
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[ord.id].x;
    ord.pt.y = rgptPlan[ord.id].y;

ThwakSumthin:
    if ((lpfl->cord <= 0x1))
        goto L_2818;
    else
        goto L_27e4;

L_27e4:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_2818 : L_27ff */

L_27ff:
    if ((ord.grobj != grobjPlanet))
        goto L_2818;
    else
        goto L_2812;

L_2812:
    return 0xffff;

L_2818:
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    if ((FMoveAiFleet(lpfl, &(ord), 0x0) != 0x0))
        goto L_2860;
    else
        goto L_285a;

L_285a:
    return 0xffff;

L_2860:
    return 0x0;
}

int16_t IdTargetFreighter(FLEET *lpflFr, PLANET *lpplHome) {
    int32_t  lWorst2;
    int32_t  scoreBest;
    PLANET  *lpplMac;
    int32_t  score;
    POINT    pt;
    int16_t  dy;
    int32_t  lWorst;
    int16_t  pctFull;
    int16_t  idBest;
    PLANET  *lppl;
    int32_t  wtPlanCargo;
    FLEET   *lpfl;
    int32_t  wtCargoMax;
    int16_t  ifl;
    int16_t  i;
    int16_t  iWorst2;
    int32_t  wtCargoFree;
    THING   *lpthBest;
    int16_t  iWorst;
    PLANET  *lpplBest;
    int16_t  ishFreighter;
    int16_t  pctHere;
    int16_t  dx;
    uint8_t *lpb;
    int16_t  fNeedy;
    ORDER    ord;
    int16_t  fSalvage;
    int32_t  l;
    uint16_t t_merge_29b2_0001;
    uint16_t t_merge_33cf_0001;

L_286c:
    fSalvage = 0x0;
    lpb = &(vlpbAiPlanet[0xe]);
    i = 0x0;
    goto L_28a6;

L_2895:
    i = (i + 0x1);
    lpb = (lpb + 0x10);

L_28a6:
    if ((i >= game.cPlanMax))
        goto L_28bb;
    else
        goto L_28b1;

L_28b1:
    *(lpb) = 0x0;
    goto L_2895;

L_28bb:
    ishFreighter = 0x0;
    goto L_28e7;

L_28c3:
    if ((lpflFr->rgcsh[ishFreighter] > 0x0))
        goto L_28f0;
    else
        goto L_28dd;

L_28dd:

L_28e3:
    ishFreighter = (ishFreighter + 0x1);

L_28e7:
    if ((ishFreighter < 0x10))
        goto L_28c3;
    else
        goto L_28f0;

L_28f0:
    lWorst = 0x3b9aca00;
    lWorst2 = 0x3b9aca00;
    i = 0x0;
    goto L_297d;

L_290c:
    if ((HIWORD(lpplHome->rgwtMin[i]) > HIWORD(lWorst)))
        goto L_2979;
    else
        goto L_2931;

L_2931:
    if ((HIWORD(lpplHome->rgwtMin[i]) < HIWORD(lWorst)))
        goto L_293e;
    else
        goto L_2936;

L_2936:
    if ((LOWORD(lpplHome->rgwtMin[i]) >= LOWORD(lWorst)))
        goto L_2979;
    else
        goto L_293e;

L_293e:
    lWorst2 = lWorst;
    iWorst2 = iWorst;
    lWorst = lpplHome->rgwtMin[i];
    iWorst = i;

L_2979:
    i = (i + 0x1);

L_297d:
    if ((i < 0x3))
        goto L_290c;
    else
        goto L_2986;

L_2986:
    if ((HIWORD(lWorst) > HIWORD((int32_t)((lWorst2 >> 0x1)))))
        goto L_29af;
    else
        goto L_299c;

L_299c:
    if ((HIWORD(lWorst) < HIWORD((int32_t)((lWorst2 >> 0x1)))))
        goto L_29a9;
    else
        goto L_29a1;

L_29a1:
    if ((LOWORD(lWorst) >= LOWORD((int32_t)((lWorst2 >> 0x1)))))
        goto L_29af;
    else
        goto L_29a9;

L_29a9:
    t_merge_29b2_0001 = 0x1;
    goto L_29b2;

L_29af:
    t_merge_29b2_0001 = 0x0;

L_29b2:
    fNeedy = t_merge_29b2_0001;
    if ((HIWORD(lWorst) > HIWORD((int32_t)((lWorst2 >> 0x2)))))
        goto L_29dc;
    else
        goto L_29cb;

L_29cb:
    if ((HIWORD(lWorst) < HIWORD((int32_t)((lWorst2 >> 0x2)))))
        goto L_29d8;
    else
        goto L_29d0;

L_29d0:
    if ((LOWORD(lWorst) >= LOWORD((int32_t)((lWorst2 >> 0x2)))))
        goto L_29dc;
    else
        goto L_29d8;

L_29d8:
    fNeedy = (fNeedy + 0x1);

L_29dc:
    ifl = 0x0;
    goto L_29e8;

L_29e4:
    ifl = (ifl + 0x1);

L_29e8:
    if ((ifl >= cFleet))
        goto L_2ad2;
    else
        goto L_29f3;

L_29f3:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_2a23;
    else
        goto L_2a1b;

L_2a1b:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_2ad2;
    else
        goto L_2a23;

L_2a23:
    if ((lpfl->iPlayer != idPlayer))
        goto L_29e4;
    else
        goto L_2a32;

L_2a32:
    if ((lpfl->cord <= 0x1))
        goto L_29e4;
    else
        goto L_2a3f;

L_2a3f:
    if ((LOWORD(lpfl) != LOWORD(lpflFr)))
        goto L_2a55;
    else
        goto L_2a4d;

L_2a4d:
    if ((HIWORD(lpfl) == HIWORD(lpflFr)))
        goto L_29e4;
    else
        goto L_2a55;

L_2a55:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_29e4 : L_2a6d */

L_2a6d:

L_2a73:
    if ((lpfl->rgcsh[ishFreighter] == 0x0))
        goto L_29e4;
    else
        goto L_2a90;

L_2a90:
    if ((lpfl->lpplord->rgord[0x1].id == lpplHome->id))
        goto L_29e4;
    else
        goto L_2aa3;

L_2aa3:

L_2aa9:
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lpfl->lpplord->rgord[0x1].id * 0x10) + 0xe))] = 0x1 */
    goto L_29e4;

L_2ad2:
    wtCargoMax = LGetFleetStat(lpflFr, 0x2);
    wtCargoFree = GetCargoFree(lpflFr);
    if ((HIWORD(wtCargoMax) > 0x0))
        goto L_2b1b;
    else
        goto L_2b07;

L_2b07:
    if ((HIWORD(wtCargoMax) < 0x0))
        goto L_2b15;
    else
        goto L_2b0c;

L_2b0c:
    if ((LOWORD(wtCargoMax) > 0x0))
        goto L_2b1b;
    else
        goto L_2b15;

L_2b15:
    return 0xffff;

L_2b1b:
    pctFull = (0x64 - LOWORD((int32_t)(((uint32_t)((wtCargoFree * 0x64)) / wtCargoMax))));
    scoreBest = 0x0;
    idBest = 0xffff;
    pt.x = lpflFr->pt.x;
    pt.y = lpflFr->pt.y;
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_3276;

L_2b8e:
    if ((lppl->id == lpflFr->idPlanet))
        goto L_3272;
    else
        goto L_2b9d;

L_2b9d:

L_2ba3:
    /* untranslated: branch byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0xe))] != 0x0 ? L_3272 : L_2bcb */

L_2bcb:

L_2bd1:
    if ((lppl->iPlayer != 0xffff))
        goto L_2d15;
    else
        goto L_2bde;

L_2bde:
    /* untranslated: branch (byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x1))] & 0x80) == 0x0 ? L_2d15 : L_2c0c */

L_2c0c:
    dx = (pt.x - rgptPlan[lppl->id].x);
    dy = (pt.y - rgptPlan[lppl->id].y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    sqrt((double)(l));
    __ftol();
    /* untranslated: l = (int32_t)(words((hiword(callresult(int32_t)) + 0x0), (loword(callresult(int32_t)) + 0x18)) / 0x19) */
    if ((HIWORD(l) > 0x0))
        goto L_2cd0;
    else
        goto L_2cb8;

L_2cb8:
    if ((HIWORD(l) < 0x0))
        goto L_2cc6;
    else
        goto L_2cbd;

L_2cbd:
    if ((LOWORD(l) >= 0x1))
        goto L_2cd0;
    else
        goto L_2cc6;

L_2cc6:
    l = 0x1;

L_2cd0:
    /* untranslated: LOWORD(score) = loword((int32_t)(words(signhiword(loword(((byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x1))] &
     * 0x7f) * 0x1f4))), loword(((byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x1))] & 0x7f) * 0x1f4))) / l)) */
    /* untranslated: HIWORD(score) = hiword((int32_t)(words(signhiword(loword(((byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x1))] &
     * 0x7f) * 0x1f4))), loword(((byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x1))] & 0x7f) * 0x1f4))) / l)) */
    goto LScore;

L_2d15:
    if ((lppl->iPlayer == idPlayer))
        goto L_2e39;
    else
        goto L_2d24;

L_2d24:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x0))
        goto L_2dbe;
    else
        goto L_2d43;

L_2d43:
    if ((lpplHome->id != lpflFr->idPlanet))
        goto L_2dbe;
    else
        goto L_2d55;

L_2d55:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) < 0x0))
        goto L_2d84;
    else
        goto L_2d62;

L_2d62:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) > 0x0))
        goto L_2d72;
    else
        goto L_2d67;

L_2d67:
    if ((LOWORD(lpplHome->rgwtMin[0x3]) <= 0x44c))
        goto L_2d84;
    else
        goto L_2d72;

L_2d72:
    if ((lppl->uPopGuess < 0x3e))
        goto L_2db3;
    else
        goto L_2d84;

L_2d84:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) < 0x0))
        goto L_2dbe;
    else
        goto L_2d91;

L_2d91:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) > 0x0))
        goto L_2da1;
    else
        goto L_2d96;

L_2d96:
    if ((LOWORD(lpplHome->rgwtMin[0x3]) <= 0x12c))
        goto L_2dbe;
    else
        goto L_2da1;

L_2da1:
    if ((lppl->uPopGuess >= 0xf))
        goto L_2dbe;
    else
        goto L_2db3;

L_2db3:
    pctHere = 0x41;
    goto ScorePctHere;

L_2dbe:
    /* untranslated: branch byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x3))] == 0x0 ? L_3272 : L_2de9 */

L_2de9:
    /* untranslated: branch (byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x3))] & 0x80) != 0x0 ? L_3272 : L_2e17 */

L_2e17:
    if ((lpplHome->id != lpflFr->idPlanet))
        goto L_3272;
    else
        goto L_2e29;

L_2e29:
    score = 0x61a8;
    goto L_2e39;

L_2e39:
    if ((LOWORD(lppl) != LOWORD(lpplHome)))
        goto L_2f46;
    else
        goto L_2e47;

L_2e47:
    if ((HIWORD(lppl) != HIWORD(lpplHome)))
        goto L_2f46;
    else
        goto L_2e4f;

L_2e4f:
    if ((pctFull < 0x23))
        goto L_3272;
    else
        goto L_2e55;

L_2e55:

L_2e5b:
    if ((pctFull != 0x64))
        goto L_2e71;
    else
        goto L_2e64;

L_2e64:
    score = 0x61a8;
    goto LScore;

L_2e71:
    dx = (pt.x - rgptPlan[lppl->id].x);
    dy = (pt.y - rgptPlan[lppl->id].y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    sqrt((double)(l));
    __ftol();
    /* untranslated: l = (int32_t)(words((hiword(callresult(int32_t)) + 0x0), (loword(callresult(int32_t)) + 0x18)) / 0x19) */
    if ((LOWORD(l) != 0x0))
        goto L_2f29;
    else
        goto L_2f1d;

L_2f1d:
    if ((HIWORD(l) == 0x0))
        goto L_3272;
    else
        goto L_2f23;

L_2f23:

L_2f29:
    /* untranslated: score = (int32_t)(words(signhiword(loword((0x14 * pctFull))), loword((0x14 * pctFull))) / l) */

L_2f46:
    if ((lppl->fStarbase != 0x0))
        goto L_3272;
    else
        goto L_2f5d;

L_2f5d:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_2f74;
    else
        goto L_2f6a;

L_2f6a:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_2fd9;
    else
        goto L_2f74;

L_2f74:
    if (((LOWORD((uint32_t)((lppl->lpplprod->rgprod[0x0] >> 0x11))) & 0x7) != 0x2))
        goto L_2fd9;
    else
        goto L_2f99;

L_2f99:
    if ((0x0 != 0x0))
        goto L_2fd9;
    else
        goto L_2fa1;

L_2fa1:
    if ((0x0 < 0x0))
        goto L_2fd9;
    else
        goto L_2fc6;

L_2fc6:
    if ((0x0 > 0x0))
        goto L_3272;
    else
        goto L_2fcb;

L_2fcb:
    if (((LOWORD((uint32_t)((lppl->lpplprod->rgprod[0x0] >> 0xa))) & 0x7f) >= 0x10))
        goto L_3272;
    else
        goto L_2fd0;

L_2fd0:

L_2fd9:
    /* untranslated: branch byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x2))] == 0x0 ? L_3051 : L_3004 */

L_3004:
    /* untranslated: branch (byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x2))] & 0x80) != 0x0 ? L_3051 : L_3032 */

L_3032:
    if ((lpplHome->id != lpflFr->idPlanet))
        goto L_3051;
    else
        goto L_3044;

L_3044:
    score = 0x61a8;
    goto LScore;

L_3051:
    if ((fNeedy != 0x2))
        goto L_3080;
    else
        goto L_305a;

L_305a:
    wtPlanCargo = lppl->rgwtMin[iWorst];
    goto L_3104;

L_3080:
    wtPlanCargo = 0x0;
    i = 0x0;
    goto L_30fb;

L_3092:
    if ((fNeedy == 0x0))
        goto L_30d4;
    else
        goto L_309b;

L_309b:
    if ((i == iWorst))
        goto L_30d4;
    else
        goto L_30a6;

L_30a6:
    wtPlanCargo = (wtPlanCargo + (int32_t)((lppl->rgwtMin[i] >> 0x1)));
    goto L_30f7;

L_30d4:
    wtPlanCargo = (wtPlanCargo + lppl->rgwtMin[i]);

L_30f7:
    i = (i + 0x1);

L_30fb:
    if ((i < 0x3))
        goto L_3092;
    else
        goto L_3104;

L_3104:
    if ((HIWORD(wtPlanCargo) > 0x0))
        goto L_311e;
    else
        goto L_310d;

L_310d:
    if ((HIWORD(wtPlanCargo) < 0x0))
        goto L_3272;
    else
        goto L_3112;

L_3112:
    if ((LOWORD(wtPlanCargo) < 0xa))
        goto L_3272;
    else
        goto L_3118;

L_3118:

L_311e:
    pctHere = LOWORD((int32_t)(((uint32_t)((wtPlanCargo * 0x64)) / wtCargoMax)));
    if ((pctHere <= (0x64 - pctFull)))
        goto ScorePctHere;
    else
        goto L_314f;

L_314f:
    pctHere = (0x64 - pctFull);

ScorePctHere:
    dx = (pt.x - rgptPlan[lppl->id].x);
    dy = (pt.y - rgptPlan[lppl->id].y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    sqrt((double)(l));
    __ftol();
    /* untranslated: l = (int32_t)(words((hiword(callresult(int32_t)) + 0x0), (loword(callresult(int32_t)) + 0x18)) / 0x19) */
    if ((HIWORD(l) > 0x0))
        goto L_321c;
    else
        goto L_3204;

L_3204:
    if ((HIWORD(l) < 0x0))
        goto L_3212;
    else
        goto L_3209;

L_3209:
    if ((LOWORD(l) >= 0x1))
        goto L_321c;
    else
        goto L_3212;

L_3212:
    l = 0x1;

L_321c:
    /* untranslated: score = (int32_t)(words(signhiword(loword((0x64 * pctHere))), loword((0x64 * pctHere))) / l) */

LScore:
    if ((HIWORD(score) < HIWORD(scoreBest)))
        goto L_3272;
    else
        goto L_3244;

L_3244:
    if ((HIWORD(score) > HIWORD(scoreBest)))
        goto L_3251;
    else
        goto L_3249;

L_3249:
    if ((LOWORD(score) <= LOWORD(scoreBest)))
        goto L_3272;
    else
        goto L_3251;

L_3251:
    scoreBest = score;
    lpplBest = lppl;
    idBest = lppl->id;

L_3272:
    lppl = (lppl + 0x1);

L_3276:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_2b8e;
    else
        goto L_3284;

L_3284:
    lpthBest = 0x0;
    if ((FSalvageTargetFreighter2(lpflFr, fNeedy, iWorst, pctFull, wtCargoMax, scoreBest, &(lpthBest), &(idBest)) == 0x0))
        goto L_32fe;
    else
        goto L_32c1;

L_32c1:
    if ((LOWORD(GetCargoFree(lpflFr)) != 0x0))
        goto L_32fe;
    else
        goto L_32d7;

L_32d7:
    /* untranslated: branch hiword(callresult(int32_t)) != 0x0 ? L_32fe : L_32df */

L_32df:
    lpthBest = 0x0;
    lpplBest = lpplHome;
    idBest = lpplHome->id;

L_32fe:
    if ((idBest != 0xffff))
        goto L_330d;
    else
        goto L_3307;

L_3307:
    return 0xffff;

L_330d:
    memset(ord, 0x0, 0x12);
    if ((LOWORD(lpthBest) != 0x0))
        goto L_3333;
    else
        goto L_332a;

L_332a:
    if ((HIWORD(lpthBest) == 0x0))
        goto L_3353;
    else
        goto L_3333;

L_3333:
    ord.grobj = grobjThing;
    ord.pt.x = lpthBest->pt.x;
    ord.pt.y = lpthBest->pt.y;
    goto L_3374;

L_3353:
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[idBest].x;
    ord.pt.y = rgptPlan[idBest].y;

L_3374:
    ord.id = idBest;
    ord.grTask = grTaskXfer;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    i = 0x0;
    goto L_3401;

L_33a6:
    if ((LOWORD(lpthBest) != 0x0))
        goto L_33cc;
    else
        goto L_33af;

L_33af:
    if ((HIWORD(lpthBest) != 0x0))
        goto L_33cc;
    else
        goto L_33b8;

L_33b8:
    if ((idBest != lpplHome->id))
        goto L_33cc;
    else
        goto L_33c6;

L_33c6:
    t_merge_33cf_0001 = 0x2;
    goto L_33cf;

L_33cc:
    t_merge_33cf_0001 = 0x1;

L_33cf:
    /* untranslated: ss:[bp+(i * 0x2)-0x5a] = ((ss:[bp+(i * 0x2)-0x5a] & 0xfff) | ((t_merge_33cf_0001 & 0xf) << 0xc)) */
    i = (i + 0x1);

L_3401:
    if ((i <= 0x2))
        goto L_33a6;
    else
        goto L_340a;

L_340a:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x1))
        goto L_3529;
    else
        goto L_3429;

L_3429:
    if ((lpplHome->id != lpflFr->idPlanet))
        goto L_34e5;
    else
        goto L_343b;

L_343b:
    ChangeMainObjSel(grobjFleet, lpflFr->id);
    if ((HIWORD(lpplHome->rgwtMin[0x3]) < 0x0))
        goto L_34d3;
    else
        goto L_345a;

L_345a:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) > 0x0))
        goto L_346a;
    else
        goto L_345f;

L_345f:
    if ((LOWORD(lpplHome->rgwtMin[0x3]) <= 0x4b0))
        goto L_34d3;
    else
        goto L_346a;

L_346a:
    if ((LOWORD(lpthBest) != 0x0))
        goto L_34d3;
    else
        goto L_3473;

L_3473:
    if ((HIWORD(lpthBest) != 0x0))
        goto L_34d3;
    else
        goto L_347c;

L_347c:
    if ((lpplBest->iPlayer == 0xffff))
        goto L_34d3;
    else
        goto L_3489;

L_3489:
    if ((HIWORD(lpplBest->rgwtMin[0x3]) > HIWORD(lpplHome->rgwtMin[0x3])))
        goto L_34d3;
    else
        goto L_34a0;

L_34a0:
    if ((HIWORD(lpplBest->rgwtMin[0x3]) < HIWORD(lpplHome->rgwtMin[0x3])))
        goto L_34ae;
    else
        goto L_34a5;

L_34a5:
    if ((LOWORD(lpplBest->rgwtMin[0x3]) >= LOWORD(lpplHome->rgwtMin[0x3])))
        goto L_34d3;
    else
        goto L_34ae;

L_34ae:
    XferAiSupply(grobjPlanet, lpflFr->idPlanet, grobjFleet, lpflFr->id, 0x3, 0x3e8);

L_34d3:
    FLookupFleet(lpflFr->id, &(sel.fl));

L_34e5:
    if ((LOWORD(lpthBest) != 0x0))
        goto L_37c7;
    else
        goto L_34ee;

L_34ee:
    if ((HIWORD(lpthBest) != 0x0))
        goto L_37c7;
    else
        goto L_34f7;

L_34f7:
    if ((lpplBest->iPlayer == 0xffff))
        goto L_37c7;
    else
        goto L_3504;

L_3504:
    if ((LOWORD(lpplBest) != LOWORD(lpplHome)))
        goto L_351a;
    else
        goto L_3512;

L_3512:
    if ((HIWORD(lpplBest) == HIWORD(lpplHome)))
        goto L_37c7;
    else
        goto L_351a;

L_351a:
    ord.txp.rgia[0x3] = ((ord.txp.rgia[0x3] & 0xfff) | 0x2000);

L_3529:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x0))
        goto L_37c7;
    else
        goto L_3548;

L_3548:
    if ((lpplHome->id != lpflFr->idPlanet))
        goto L_37c7;
    else
        goto L_355a;

L_355a:
    if ((LOWORD(lpthBest) != 0x0))
        goto L_37c7;
    else
        goto L_3563;

L_3563:
    if ((HIWORD(lpthBest) != 0x0))
        goto L_37c7;
    else
        goto L_356c;

L_356c:
    if ((lpplBest->iPlayer == 0xffff))
        goto L_37c7;
    else
        goto L_3579;

L_3579:
    if ((lpplBest->iPlayer == idPlayer))
        goto L_3625;
    else
        goto L_3588;

L_3588:
    ChangeMainObjSel(grobjFleet, lpflFr->id);
    if ((HIWORD(lpplHome->rgwtMin[0x3]) < 0x0))
        goto L_35df;
    else
        goto L_35a7;

L_35a7:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) > 0x0))
        goto L_35b7;
    else
        goto L_35ac;

L_35ac:
    if ((LOWORD(lpplHome->rgwtMin[0x3]) <= 0x384))
        goto L_35df;
    else
        goto L_35b7;

L_35b7:
    XferAiSupply(grobjPlanet, lpflFr->idPlanet, grobjFleet, lpflFr->id, 0x3, 0x12c);
    goto L_3604;

L_35df:
    XferAiSupply(grobjPlanet, lpflFr->idPlanet, grobjFleet, lpflFr->id, 0x3, 0x64);

L_3604:
    FLookupFleet(lpflFr->id, &(sel.fl));
    ord.txp.rgia[0x3] = ((ord.txp.rgia[0x3] & 0xfff) | 0x2000);
    goto L_37c7;

L_3625:
    if ((HIWORD(lpplBest->rgwtMin[0x3]) > 0x0))
        goto L_37c7;
    else
        goto L_3632;

L_3632:
    if ((HIWORD(lpplBest->rgwtMin[0x3]) < 0x0))
        goto L_3642;
    else
        goto L_3637;

L_3637:
    if ((LOWORD(lpplBest->rgwtMin[0x3]) >= 0x3e8))
        goto L_37c7;
    else
        goto L_3642;

L_3642:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) < HIWORD((int32_t)(((uint32_t)((lpplBest->rgwtMin[0x3] * 0x3)) / 0x2)))))
        goto L_37c7;
    else
        goto L_3675;

L_3675:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) > HIWORD((int32_t)(((uint32_t)((lpplBest->rgwtMin[0x3] * 0x3)) / 0x2)))))
        goto L_3683;
    else
        goto L_367a;

L_367a:
    if ((LOWORD(lpplHome->rgwtMin[0x3]) <= LOWORD((int32_t)(((uint32_t)((lpplBest->rgwtMin[0x3] * 0x3)) / 0x2)))))
        goto L_37c7;
    else
        goto L_3683;

L_3683:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) < 0x0))
        goto L_37c7;
    else
        goto L_3690;

L_3690:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) > 0x0))
        goto L_36a0;
    else
        goto L_3695;

L_3695:
    if ((LOWORD(lpplHome->rgwtMin[0x3]) <= 0x1f4))
        goto L_37c7;
    else
        goto L_36a0;

L_36a0:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) < 0x0))
        goto L_36ca;
    else
        goto L_36ad;

L_36ad:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) > 0x0))
        goto L_36bd;
    else
        goto L_36b2;

L_36b2:
    if ((LOWORD(lpplHome->rgwtMin[0x3]) <= 0x7d0))
        goto L_36ca;
    else
        goto L_36bd;

L_36bd:
    l = 0xa;
    goto L_3744;

L_36ca:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) < 0x0))
        goto L_36f4;
    else
        goto L_36d7;

L_36d7:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) > 0x0))
        goto L_36e7;
    else
        goto L_36dc;

L_36dc:
    if ((LOWORD(lpplHome->rgwtMin[0x3]) <= 0x5dc))
        goto L_36f4;
    else
        goto L_36e7;

L_36e7:
    l = 0x8;
    goto L_3744;

L_36f4:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) < 0x0))
        goto L_371e;
    else
        goto L_3701;

L_3701:
    if ((HIWORD(lpplHome->rgwtMin[0x3]) > 0x0))
        goto L_3711;
    else
        goto L_3706;

L_3706:
    if ((LOWORD(lpplHome->rgwtMin[0x3]) <= 0x3e8))
        goto L_371e;
    else
        goto L_3711;

L_3711:
    l = 0x5;
    goto L_3744;

L_371e:
    /* untranslated: l = (int32_t)(words((HIWORD(lpplHome->rgwtMin[0x3]) + 0xffff), (LOWORD(lpplHome->rgwtMin[0x3]) + 0xfe70)) / 0x64) */

L_3744:
    l = (int32_t)(((uint32_t)((lpplHome->rgwtMin[0x3] * l)) / 0x64));
    ChangeMainObjSel(grobjFleet, lpflFr->id);
    XferAiSupply(grobjPlanet, lpflFr->idPlanet, grobjFleet, lpflFr->id, 0x3, LOWORD(l));
    FLookupFleet(lpflFr->id, &(sel.fl));
    ord.txp.rgia[0x3] = ((ord.txp.rgia[0x3] & 0xfff) | 0x2000);

L_37c7:
    if ((LOWORD(lpthBest) != 0x0))
        goto L_37d9;
    else
        goto L_37d0;

L_37d0:
    if ((HIWORD(lpthBest) == 0x0))
        goto L_37f4;
    else
        goto L_37d9;

L_37d9:
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x0;
    goto L_392a;

L_37f4:
    if ((lpplHome->id == idBest))
        goto L_392a;
    else
        goto L_3802;

L_3802:
    if ((fNeedy == 0x0))
        goto L_392a;
    else
        goto L_380b;

L_380b:
    if ((fNeedy == 0x2))
        goto L_3846;
    else
        goto L_3814;

L_3814:
    if ((HIWORD(lpplBest->rgwtMin[iWorst]) < HIWORD(wtCargoFree)))
        goto L_3887;
    else
        goto L_3839;

L_3839:
    if ((HIWORD(lpplBest->rgwtMin[iWorst]) > HIWORD(wtCargoFree)))
        goto L_3846;
    else
        goto L_383e;

L_383e:
    if ((LOWORD(lpplBest->rgwtMin[iWorst]) < LOWORD(wtCargoFree)))
        goto L_3887;
    else
        goto L_3846;

L_3846:
    i = 0x0;
    goto L_387b;

L_384e:
    if ((i == iWorst))
        goto L_3877;
    else
        goto L_3859;

L_3859:
    /* untranslated: ss:[bp+(i * 0x2)-0x5a] = ((ss:[bp+(i * 0x2)-0x5a] & 0xfff) | 0x0) */

L_3877:
    i = (i + 0x1);

L_387b:
    if ((i > 0x2))
        goto L_392a;
    else
        goto L_3881;

L_3881:

L_3887:
    if ((LOWORD(lpthBest) != 0x0))
        goto L_38a6;
    else
        goto L_3890;

L_3890:
    if ((HIWORD(lpthBest) != 0x0))
        goto L_38a6;
    else
        goto L_3899;

L_3899:
    if ((lpplBest->iPlayer == 0xffff))
        goto L_392a;
    else
        goto L_38a6;

L_38a6:
    i = 0x0;
    goto L_3921;

L_38ae:
    /* untranslated: ss:[bp+(i * 0x2)-0x5a] = ((ss:[bp+(i * 0x2)-0x5a] & 0xfff) | 0x5000) */
    if ((i != iWorst))
        goto L_38e4;
    else
        goto L_38d7;

L_38d7:
    l = 0x42;
    goto L_38ee;

L_38e4:
    l = 0x21;

L_38ee:
    /* untranslated: ss:[bp+(i * 0x2)-0x5a] = ((ss:[bp+(i * 0x2)-0x5a] & 0xf000) | (LOWORD(l) & 0xfff)) */
    i = (i + 0x1);

L_3921:
    if ((i <= 0x2))
        goto L_38ae;
    else
        goto L_392a;

L_392a:
    if ((FMoveAiFleet(lpflFr, &(ord), 0x0) != 0x0))
        goto L_394e;
    else
        goto L_3948;

L_3948:
    return 0xffff;

L_394e:
    return idBest;
}

int16_t FSalvageTargetFreighter2(FLEET *lpflFr, int16_t fNeedy, int16_t iWorst, int16_t pctFull, int32_t wtCargoMax, int32_t scoreBest, THING **plpthBest,
                                 int16_t *pidBest) {
    POINT   pt;
    int32_t score;
    int16_t dy;
    int16_t i;
    int32_t wtPlanCargo;
    THING  *lpth;
    int16_t pctHere;
    int16_t dx;
    THING  *lpthMac;
    int16_t fSalvage;
    int32_t l;
    int16_t t_merge_3afd_0001;

L_395a:
    fSalvage = 0x0;
    pt.x = lpflFr->pt.x;
    pt.y = lpflFr->pt.y;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_3cf0;

L_39a4:
    if ((lpth->ith != ithMineralPacket))
        goto L_3cec;
    else
        goto L_39ba;

L_39ba:
    if ((lpth->thp.iWarp != 0x0))
        goto L_3cec;
    else
        goto L_39d1;

L_39d1:
    dx = (pt.x - lpth->pt.x);
    dy = (pt.y - lpth->pt.y);
    if ((dx != 0x0))
        goto L_3ae9;
    else
        goto L_39f4;

L_39f4:
    if ((dy != 0x0))
        goto L_3ae9;
    else
        goto L_39fd;

L_39fd:
    if ((fNeedy == 0x0))
        goto L_3a5a;
    else
        goto L_3a06;

L_3a06:
    /* untranslated: branch HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (iWorst * 0x2))] == 0x0 ? L_3a5a : L_3a23 */

L_3a23:
    /* untranslated: call XferAiSupply(grobjThing, lpth->idFull, grobjFleet, lpflFr->id, iWorst, HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (iWorst * 0x2))]) ->
     * callresult(int16_t) */

L_3a5a:
    if ((fNeedy == 0x2))
        goto L_3acc;
    else
        goto L_3a63;

L_3a63:
    i = 0x0;
    goto L_3ac3;

L_3a6b:
    /* untranslated: branch HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] == 0x0 ? L_3abf : L_3a88 */

L_3a88:
    /* untranslated: call XferAiSupply(grobjThing, lpth->idFull, grobjFleet, lpflFr->id, i, HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) ->
     * callresult(int16_t) */

L_3abf:
    i = (i + 0x1);

L_3ac3:
    if ((i < 0x3))
        goto L_3a6b;
    else
        goto L_3acc;

L_3acc:
    fSalvage = 0x1;
    FLookupFleet(lpflFr->id, &(sel.fl));
    goto L_3cec;

L_3ae9:
    if ((dx <= dy))
        goto L_3afa;
    else
        goto L_3af4;

L_3af4:
    t_merge_3afd_0001 = dx;
    goto L_3afd;

L_3afa:
    t_merge_3afd_0001 = dy;

L_3afd:
    if ((t_merge_3afd_0001 > 0xc8))
        goto L_3cec;
    else
        goto L_3b02;

L_3b02:

L_3b08:
    if ((fNeedy != 0x2))
        goto L_3b32;
    else
        goto L_3b11;

L_3b11:
    /* untranslated: LOWORD(wtPlanCargo) = HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (iWorst * 0x2))] */
    /* untranslated: HIWORD(wtPlanCargo) = signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (iWorst * 0x2))]) */
    goto L_3ba6;

L_3b32:
    wtPlanCargo = 0x0;
    i = 0x0;
    goto L_3b9d;

L_3b44:
    if ((fNeedy == 0x0))
        goto L_3b7b;
    else
        goto L_3b4d;

L_3b4d:
    if ((i == iWorst))
        goto L_3b7b;
    else
        goto L_3b58;

L_3b58:
    /* untranslated: LOWORD(wtPlanCargo) = (LOWORD(wtPlanCargo) + (HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] >> 0x1)) */
    /* untranslated: HIWORD(wtPlanCargo) = (HIWORD(wtPlanCargo) + signhiword((HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] >> 0x1))) */
    goto L_3b99;

L_3b7b:
    /* untranslated: LOWORD(wtPlanCargo) = (LOWORD(wtPlanCargo) + HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) */
    /* untranslated: HIWORD(wtPlanCargo) = (HIWORD(wtPlanCargo) + signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))])) */

L_3b99:
    i = (i + 0x1);

L_3b9d:
    if ((i < 0x3))
        goto L_3b44;
    else
        goto L_3ba6;

L_3ba6:
    if ((HIWORD(wtPlanCargo) > 0x0))
        goto L_3bc0;
    else
        goto L_3baf;

L_3baf:
    if ((HIWORD(wtPlanCargo) < 0x0))
        goto L_3cec;
    else
        goto L_3bb4;

L_3bb4:
    if ((LOWORD(wtPlanCargo) < 0xa))
        goto L_3cec;
    else
        goto L_3bba;

L_3bba:

L_3bc0:
    pctHere = LOWORD((int32_t)(((uint32_t)((wtPlanCargo * 0x64)) / wtCargoMax)));
    if ((pctHere <= (0x64 - pctFull)))
        goto L_3bfa;
    else
        goto L_3bf1;

L_3bf1:
    pctHere = (0x64 - pctFull);

L_3bfa:
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    sqrt((double)(l));
    __ftol();
    /* untranslated: l = (int32_t)(words((hiword(callresult(int32_t)) + 0x0), (loword(callresult(int32_t)) + 0x18)) / 0x19) */
    if ((HIWORD(l) > 0x0))
        goto L_3c92;
    else
        goto L_3c7a;

L_3c7a:
    if ((HIWORD(l) < 0x0))
        goto L_3c88;
    else
        goto L_3c7f;

L_3c7f:
    if ((LOWORD(l) >= 0x1))
        goto L_3c92;
    else
        goto L_3c88;

L_3c88:
    l = 0x1;

L_3c92:
    /* untranslated: score = (int32_t)(words(signhiword(loword((0x64 * pctHere))), loword((0x64 * pctHere))) / l) */
    if ((HIWORD(score) < HIWORD(scoreBest)))
        goto L_3cec;
    else
        goto L_3cba;

L_3cba:
    if ((HIWORD(score) > HIWORD(scoreBest)))
        goto L_3cc7;
    else
        goto L_3cbf;

L_3cbf:
    if ((LOWORD(score) <= LOWORD(scoreBest)))
        goto L_3cec;
    else
        goto L_3cc7;

L_3cc7:
    scoreBest = score;
    *(plpthBest) = lpth;
    *(pidBest) = lpth->idFull;

L_3cec:
    lpth = (lpth + 0x1);

L_3cf0:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_39a4;
    else
        goto L_3cfe;

L_3cfe:

L_3d04:
    return fSalvage;
}

int16_t FMoveAiFleet(FLEET *lpfl, ORDER *pord, int16_t fAppend) {
    int16_t  iord;
    uint16_t t_merge_3d6c_0001;

L_3d0a:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((sel.fl.lpplord->iordMax != sel.fl.cord))
        goto L_3d57;
    else
        goto L_3d39;

L_3d39:
    sel.fl.lpplord = LpplReAlloc(sel.fl.lpplord, (sel.fl.cord + 0x1));

L_3d57:
    if ((fAppend == 0x0))
        goto L_3d69;
    else
        goto L_3d60;

L_3d60:
    t_merge_3d6c_0001 = (sel.fl.cord + 0xffff);
    goto L_3d6c;

L_3d69:
    t_merge_3d6c_0001 = 0x0;

L_3d6c:
    iord = t_merge_3d6c_0001;
    if ((pord->pt.x != sel.fl.lpplord->rgord[iord].pt.x))
        goto L_3db9;
    else
        goto L_3d93;

L_3d93:
    if ((pord->pt.y == sel.fl.lpplord->rgord[iord].pt.y))
        goto L_3dc0;
    else
        goto L_3db9;

L_3db9:
    iord = (iord + 0x1);
    goto L_3de0;

L_3dc0:
    if ((sel.fl.cord <= 0x1))
        goto L_3de0;
    else
        goto L_3dca;

L_3dca:
    sel.fl.cord = 0x1;
    FLookupFleet(0xffff, &(sel.fl));

L_3de0:
    sel.fl.lpplord->rgord[iord] = *(pord);
    /* untranslated: ss:[bp-0x8] = (LOWORD(sel.fl.lpplord) + 0x4) */
    /* untranslated: ss:[bp-0x6] = HIWORD(sel.fl.lpplord) */
    sel.fl.cord = (iord + 0x1);
    sel.fl.lpplord->iordMac = LOBYTE((iord + 0x1));
    FLookupFleet(0xffff, &(sel.fl));

L_3e4a:
    /* untranslated: return callresult(int16_t) */
}

void AddItemToQueue(uint16_t iItem, uint16_t cItem, GrobjClass grobj, int16_t mdAddItem) {
    int16_t  fSingle;
    int16_t  iprod;
    int16_t  i;
    PROD     rgprod[64];
    uint16_t t_merge_3f81_0001;
    uint16_t t_merge_424b_0001;

L_3e50:
    fSingle = 0x0;
    if ((LOWORD(sel.pl.lpplprod) != 0x0))
        goto L_3e72;
    else
        goto L_3e68;

L_3e68:
    if ((HIWORD(sel.pl.lpplprod) == 0x0))
        goto L_3e88;
    else
        goto L_3e72;

L_3e72:
    if ((sel.pl.lpplprod->iprodMac > 0xc8))
        goto L_4254;
    else
        goto L_3e82;

L_3e82:

L_3e88:
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_3f84;
    else
        goto L_3e92;

L_3e92:
    if ((HIWORD(lpplProdGlob) != 0x0))
        goto L_3f84;
    else
        goto L_3e9c;

L_3e9c:
    fSingle = 0x1;
    InitProduction(rgprod);
    i = 0x0;
    goto L_3f25;

L_3eb6:
    if (((LOWORD((uint32_t)((rgprod[i] >> 0xa))) & 0x7f) != iItem))
        goto L_3f21;
    else
        goto L_3ee3;

L_3ee3:
    if ((0x0 != 0x0))
        goto L_3f21;
    else
        goto L_3eea;

L_3eea:
    if (((LOWORD((uint32_t)((rgprod[i] >> 0x11))) & 0x7) != grobj))
        goto L_3f21;
    else
        goto L_3f17;

L_3f17:
    if ((0x0 == 0x0))
        goto L_3f30;
    else
        goto L_3f1b;

L_3f1b:

L_3f21:
    i = (i + 0x1);

L_3f25:
    if ((i < cProdGlob))
        goto L_3eb6;
    else
        goto L_3f30;

L_3f30:
    if ((i < cProdGlob))
        goto L_3f43;
    else
        goto L_3f3b;

L_3f3b:
    cItem = 0x0;
    goto L_3f84;

L_3f43:
    if ((cItem >= (LOWORD(rgprod[i]) & 0x3ff)))
        goto L_3f69;
    else
        goto L_3f63;

L_3f63:
    t_merge_3f81_0001 = cItem;
    goto L_3f81;

L_3f69:
    t_merge_3f81_0001 = (LOWORD(rgprod[i]) & 0x3ff);

L_3f81:
    cItem = t_merge_3f81_0001;

L_3f84:
    if ((cItem <= 0x0))
        goto L_4230;
    else
        goto L_3f8d;

L_3f8d:
    if ((mdAddItem == 0x2))
        goto L_3fe3;
    else
        goto L_3f96;

L_3f96:
    if ((lpplProdGlob->iprodMac != lpplProdGlob->iprodMax))
        goto L_3fe3;
    else
        goto L_3fbd;

L_3fbd:
    lpplProdGlob = LpplReAlloc(lpplProdGlob, (lpplProdGlob->iprodMac + 0x3));

L_3fe3:
    goto L_4060;

L_3fe9:
    lpplProdGlob->iprodMac = 0x0;

L_3ff2:
    iprod = lpplProdGlob->iprodMac;
    goto L_407b;

L_4003:
    iprod = 0x0;
    fmemmove(&(lpplProdGlob->rgprod[0x1]), &(lpplProdGlob->rgprod), (lpplProdGlob->iprodMac * 0x4));
    fmemset(&(lpplProdGlob->rgprod), 0x0, 0x4);
    goto L_407b;

L_4060:
    if ((mdAddItem == 0x0))
        goto L_4003;
    else
        goto L_4068;

L_4068:
    if ((mdAddItem == 0x1))
        goto L_3ff2;
    else
        goto L_4070;

L_4070:
    if ((mdAddItem == 0x2))
        goto L_3fe9;
    else
        goto L_407b;

L_407b:
    /* untranslated: LOWORD(lpplProdGlob[iprod].rgprod[0x0]) = ((LOWORD(lpplProdGlob[iprod].rgprod[0x0]) & 0xfc00) | loword((int32_t)(words((cItem & 0x3ff),
     * 0x0) << 0x0))) */
    /* untranslated: HIWORD(lpplProdGlob[iprod].rgprod[0x0]) = ((HIWORD(lpplProdGlob[iprod].rgprod[0x0]) & 0xffff) | hiword((int32_t)(words((cItem & 0x3ff),
     * 0x0) << 0x0))) */
    /* untranslated: LOWORD(lpplProdGlob[iprod].rgprod[0x0]) = ((LOWORD(lpplProdGlob[iprod].rgprod[0x0]) & 0x3ff) | loword((int32_t)(words((iItem & 0x7f), 0x0)
     * << 0xa))) */
    /* untranslated: HIWORD(lpplProdGlob[iprod].rgprod[0x0]) = ((HIWORD(lpplProdGlob[iprod].rgprod[0x0]) & 0xfffe) | hiword((int32_t)(words((iItem & 0x7f), 0x0)
     * << 0xa))) */
    /* untranslated: LOWORD(lpplProdGlob[iprod].rgprod[0x0]) = ((LOWORD(lpplProdGlob[iprod].rgprod[0x0]) & 0xffff) | loword((int32_t)(words((grobj & 0x7), 0x0)
     * << 0x11))) */
    /* untranslated: HIWORD(lpplProdGlob[iprod].rgprod[0x0]) = ((HIWORD(lpplProdGlob[iprod].rgprod[0x0]) & 0xfff1) | hiword((int32_t)(words((grobj & 0x7), 0x0)
     * << 0x11))) */
    LOWORD(lpplProdGlob[iprod].rgprod[0x0]) = ((LOWORD(lpplProdGlob[iprod].rgprod[0x0]) & 0xffff) | 0x0);
    HIWORD(lpplProdGlob[iprod].rgprod[0x0]) = ((HIWORD(lpplProdGlob[iprod].rgprod[0x0]) & 0xf80f) | 0x0);
    LOWORD(lpplProdGlob[iprod].rgprod[0x0]) = ((LOWORD(lpplProdGlob[iprod].rgprod[0x0]) & 0xffff) | 0x0);
    HIWORD(lpplProdGlob[iprod].rgprod[0x0]) = ((HIWORD(lpplProdGlob[iprod].rgprod[0x0]) & 0x7ff) | 0x0);
    lpplProdGlob->iprodMac = (lpplProdGlob->iprodMac + 0x1);

L_4230:
    if ((fSingle == 0x0))
        goto L_4254;
    else
        goto L_4239;

L_4239:
    if ((cItem == 0x0))
        goto L_4248;
    else
        goto L_4242;

L_4242:
    t_merge_424b_0001 = 0x1;
    goto L_424b;

L_4248:
    t_merge_424b_0001 = 0x0;

L_424b:
    FinishProduction(t_merge_424b_0001);

L_4254:
    return;
}

int16_t IroEnsureAi(uint8_t *lpbRes, int16_t cRes, int16_t *pishdefSBLatest, int16_t pct) {
    int16_t iSmallest;
    int16_t i;
    int16_t pctTech;
    int16_t ilvl;

L_425a:
    if ((pishdefSBLatest == 0x0))
        goto L_4280;
    else
        goto L_426c;

L_426c:
    EnsureAiStarbaseDesigns();
    *(pishdefSBLatest) = IshdefAiSBLatest();
    ValidateStarbaseHistory();

L_4280:
    rgplr[idPlayer].pctResearch = LOBYTE(pct);
    i = 0x0;
    goto L_42c8;

L_42a0:
    if (((uint16_t)(rgplr[idPlayer].rgTech[i]) < 0x18))
        goto L_42d1;
    else
        goto L_42be;

L_42be:

L_42c4:
    i = (i + 0x1);

L_42c8:
    if ((i < 0x6))
        goto L_42a0;
    else
        goto L_42d1;

L_42d1:
    if ((i != 0x6))
        goto L_4316;
    else
        goto L_42da;

L_42da:
    rgplr[idPlayer].pctResearch = 0x0;
    pctTech = ((uint16_t)(rgplr[idPlayer].iTechCur) * 0x100);
    WriteMemRt(0x22, 0x2, pctTech);

L_4316:
    i = 0x0;
    goto L_449e;

L_431e:
    ilvl = (uint16_t)(rgplr[idPlayer].rgTech[(lpbRes[i] >> 0x5)]);
    if ((ilvl >= (lpbRes[i] & 0x1f)))
        goto L_449a;
    else
        goto L_4372;

L_4372:
    rgplr[idPlayer].iTechCur = LOBYTE((((uint16_t)(rgplr[idPlayer].iTechCur) & 0xfff0) | (lpbRes[i] >> 0x5)));
    if ((i >= (cRes + 0xffff)))
        goto L_4450;
    else
        goto L_43cd;

L_43cd:
    if (((ilvl + 0x1) != (lpbRes[i] & 0x1f)))
        goto L_4450;
    else
        goto L_43f8;

L_43f8:
    /* untranslated: rgplr[idPlayer].iTechCur = lobyte(((sext8to16(rgplr[idPlayer].iTechCur) & 0xff0f) | ((part[1:1](lpbRes[i]) >> 0x5) * 0x10))) */

L_4450:
    pctTech = ((uint16_t)(rgplr[idPlayer].pctResearch) + ((uint16_t)(rgplr[idPlayer].iTechCur) * 0x100));
    WriteMemRt(0x22, 0x2, pctTech);
    return i;

L_449a:
    i = (i + 0x1);

L_449e:
    if ((i < cRes))
        goto L_431e;
    else
        goto L_44a9;

L_44a9:
    iSmallest = 0x0;
    i = 0x1;
    goto L_44ff;

L_44b6:
    if (((uint16_t)(rgplr[idPlayer].rgTech[i]) >= (uint16_t)(rgplr[idPlayer].rgTech[iSmallest])))
        goto L_44fb;
    else
        goto L_44f5;

L_44f5:
    iSmallest = i;

L_44fb:
    i = (i + 0x1);

L_44ff:
    if ((i < 0x6))
        goto L_44b6;
    else
        goto L_4508;

L_4508:
    if ((((uint16_t)(rgplr[idPlayer].iTechCur) & 0xf) == iSmallest))
        goto L_45bd;
    else
        goto L_4523;

L_4523:
    rgplr[idPlayer].iTechCur = LOBYTE((((uint16_t)(rgplr[idPlayer].iTechCur) & 0xfff0) | iSmallest));
    if ((((uint16_t)(rgplr[idPlayer].iTechCur) & 0xf) != 0x1a))
        goto L_4579;
    else
        goto L_4569;

L_4569:
    rgplr[idPlayer].pctResearch = 0x0;

L_4579:
    pctTech = ((uint16_t)(rgplr[idPlayer].pctResearch) + ((uint16_t)(rgplr[idPlayer].iTechCur) * 0x100));
    WriteMemRt(0x22, 0x2, pctTech);

L_45bd:
    return 0x39e;
}

void KeepFleetsMoving() {
    int16_t i;
    int16_t ifl;
    FLEET  *lpfl;
    THING  *lpth;
    THING  *rglpth[100];
    int16_t ith;
    THING  *lpthMac;

L_45ca:
    ith = 0x0;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_4680;

L_460a:
    if ((lpth->ith != ithMinefield))
        goto L_467c;
    else
        goto L_4620;

L_4620:
    if ((lpth->iplr == idPlayer))
        goto L_467c;
    else
        goto L_4639;

L_4639:
    if ((lpth->thm.iType == 0x2))
        goto L_467c;
    else
        goto L_464b;

L_464b:
    if ((ith != 0x64))
        goto L_465e;
    else
        goto L_4655;

L_4655:
    ith = 0xffff;
    goto LKeepMovn;

L_465e:
    ith = (ith + 0x1);
    rglpth[ith] = lpth;

L_467c:
    lpth = (lpth + 0x1);

L_4680:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_460a;
    else
        goto LKeepMovn;

LKeepMovn:
    ifl = 0x0;
    goto L_469c;

L_4698:
    ifl = (ifl + 0x1);

L_469c:
    if ((ifl >= cFleet))
        goto L_4764;
    else
        goto L_46a7;

L_46a7:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_46d7;
    else
        goto L_46cf;

L_46cf:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_4764;
    else
        goto L_46d7;

L_46d7:
    if ((lpfl->iPlayer != idPlayer))
        goto L_4698;
    else
        goto L_46e6;

L_46e6:
    if ((lpfl->cord <= 0x1))
        goto L_4698;
    else
        goto L_46f0;

L_46f0:

L_46f6:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x0))
        goto L_4727;
    else
        goto L_4715;

L_4715:
    if ((game.turn >= 0x5))
        goto L_4727;
    else
        goto L_471f;

L_471f:
    i = 0x10;
    goto L_4747;

L_4727:
    i = 0xffff;
    if ((FIsAiTransport(lpfl) == 0x0))
        goto L_4747;
    else
        goto L_4742;

L_4742:
    i = 0x1e;

L_4747:
    SetAiFleetIdealSpeed(lpfl, i, ith, rglpth);
    goto L_4698;

L_4764:
    return;
}

int16_t FShouldWeBuildColonizers(int16_t *pcCol) {
    int16_t  iMax;
    int16_t  cColFl;
    uint32_t cBuilt;
    int16_t  iMin;
    int16_t  ifl;
    int16_t  i;
    FLEET   *lpfl;
    int16_t  rgish[16];
    int16_t  cColPl;

L_476a:
    cColFl = 0x0;
    cColPl = 0x0;
    cBuilt = 0x0;
    iMax = 0xffff;
    iMin = 0x10;
    if ((pcCol == 0x0))
        goto L_47a1;
    else
        goto L_479a;

L_479a:
    *(pcCol) = 0x0;

L_47a1:
    if ((((rgplr[idPlayer].wMdPlr >> 0xa) & 0x7) != 0x0))
        goto L_47d4;
    else
        goto L_47c0;

L_47c0:
    if (((game.turn & 0x1) == 0x0))
        goto L_47d4;
    else
        goto L_47ce;

L_47ce:
    return 0x0;

L_47d4:
    if ((game.turn >= 0x1e))
        goto L_47e4;
    else
        goto L_47de;

L_47de:
    return 0x1;

L_47e4:
    i = 0x0;
    goto L_4873;

L_47ec:
    if ((((rgshdef[i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_4861;
    else
        goto L_480a;

L_480a:
    if ((rgshdef[i].hul.ihuldef == ihuldefMiniColonyShip))
        goto L_482e;
    else
        goto L_481c;

L_481c:
    if ((rgshdef[i].hul.ihuldef != ihuldefColonyShip))
        goto L_4861;
    else
        goto L_482e;

L_482e:
    rgish[i] = 0x1;
    if ((i <= iMax))
        goto L_484d;
    else
        goto L_4847;

L_4847:
    iMax = i;

L_484d:
    if ((i >= iMin))
        goto L_486f;
    else
        goto L_4858;

L_4858:
    iMin = i;

L_4861:
    rgish[i] = 0x0;

L_486f:
    i = (i + 0x1);

L_4873:
    if ((i < 0x10))
        goto L_47ec;
    else
        goto L_487c;

L_487c:
    iMax = (iMax + 0x1);
    if ((iMax > 0x0))
        goto L_488f;
    else
        goto L_4889;

L_4889:
    return 0x0;

L_488f:
    ifl = 0x0;
    goto L_489b;

L_4897:
    ifl = (ifl + 0x1);

L_489b:
    if ((ifl >= cFleet))
        goto L_4939;
    else
        goto L_48a6;

L_48a6:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_48d6;
    else
        goto L_48ce;

L_48ce:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_4939;
    else
        goto L_48d6;

L_48d6:
    if ((lpfl->iPlayer != idPlayer))
        goto L_4897;
    else
        goto L_48e2;

L_48e2:

L_48e8:
    i = iMin;
    goto L_492b;

L_48f1:
    if ((rgish[i] <= 0x0))
        goto L_4927;
    else
        goto L_4903;

L_4903:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_4927;
    else
        goto L_4920;

L_4920:
    cColFl = (cColFl + 0x1);
    goto L_4897;

L_4927:
    i = (i + 0x1);

L_492b:
    if ((i >= iMax))
        goto L_4897;
    else
        goto L_4933;

L_4933:

L_4939:
    if ((pcCol == 0x0))
        goto L_494a;
    else
        goto L_4942;

L_4942:
    *(pcCol) = cColFl;

L_494a:
    if ((cColFl <= (LOWORD((0x14 * game.mdSize)) + 0xa)))
        goto L_4962;
    else
        goto L_495c;

L_495c:
    return 0x0;

L_4962:
    i = 0x0;
    goto L_496e;

L_496a:
    i = (i + 0x1);

L_496e:
    if ((i >= game.cPlayer))
        goto L_49ab;
    else
        goto L_4979;

L_4979:
    if ((((rgplr[i].wMdPlr >> 0x8) & 0x1) == 0x0))
        goto L_496a;
    else
        goto L_4997;

L_4997:
    cColPl = (cColPl + rgplr[i].cPlanet);

L_49ab:
    if (((cColFl + cColPl) <= ((uint32_t)((game.cPlanMax * 0x4)) / 0x5)))
        goto L_49d1;
    else
        goto L_49cb;

L_49cb:
    return 0x0;

L_49d1:
    cBuilt = 0x0;
    i = iMin;
    goto L_4a12;

L_49e4:
    if ((rgish[i] <= 0x0))
        goto L_4a0e;
    else
        goto L_49f6;

L_49f6:
    cBuilt = (cBuilt + rgshdef[i].cBuilt);

L_4a0e:
    i = (i + 0x1);

L_4a12:
    if ((i < iMax))
        goto L_49e4;
    else
        goto L_4a1d;

L_4a1d:
    if (((HIWORD(cBuilt) - SIGNHIWORD((cColPl + cColFl))) < 0x0))
        goto L_4a66;
    else
        goto L_4a36;

L_4a36:
    if (((HIWORD(cBuilt) - SIGNHIWORD((cColPl + cColFl))) > 0x0))
        goto L_4a43;
    else
        goto L_4a3b;

L_4a3b:
    if (((LOWORD(cBuilt) - (cColPl + cColFl)) <= 0x19))
        goto L_4a66;
    else
        goto L_4a43;

L_4a43:
    if ((cColFl > 0x4))
        goto L_4a60;
    else
        goto L_4a4c;

L_4a4c:
    if ((Random(0x2) == 0x0))
        goto L_4a66;
    else
        goto L_4a60;

L_4a60:
    return 0x0;

L_4a66:
    return 0x1;
}

int16_t FIsAiAttack(FLEET *lpfl) {
    int16_t ihul;
    int16_t i;

L_4a72:
    i = 0x0;
    goto L_4b85;

L_4a83:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_4b81;
    else
        goto L_4aa0;

L_4aa0:
    ihul = rgshdef[i].hul.ihuldef;
    if ((ihul <= 0x5))
        goto L_4ac7;
    else
        goto L_4ab8;

L_4ab8:
    if ((ihul > 0xa))
        goto L_4ac7;
    else
        goto L_4ac1;

L_4ac1:
    return 0x1;

L_4ac7:
    if ((ihul != 0x5))
        goto L_4b11;
    else
        goto L_4ad0;

L_4ad0:
    /* untranslated: branch part[137:2](rglpshdef[idPlayer][i]) < 0x0 ? L_4b0b : L_4af5 */

L_4af5:
    /* untranslated: branch part[137:2](rglpshdef[idPlayer][i]) > 0x0 ? L_4b05 : L_4afa */

L_4afa:
    /* untranslated: branch part[135:2](rglpshdef[idPlayer][i]) <= 0x0 ? L_4b0b : L_4b05 */

L_4b05:
    return 0x1;

L_4b0b:
    return 0x0;

L_4b11:
    if ((ihul == 0x1f))
        goto L_4b23;
    else
        goto L_4b1a;

L_4b1a:
    if ((ihul != 0x1d))
        goto L_4b81;
    else
        goto L_4b23;

L_4b23:
    if ((WtMaxShdefStat(&(rgshdef[i]), 0x2) >= 0x1f4))
        goto L_4b81;
    else
        goto L_4b46;

L_4b46:
    /* untranslated: branch part[137:2](rglpshdef[idPlayer][i]) < 0x0 ? L_4b81 : L_4b6b */

L_4b6b:
    /* untranslated: branch part[137:2](rglpshdef[idPlayer][i]) > 0x0 ? L_4b7b : L_4b70 */

L_4b70:
    /* untranslated: branch part[135:2](rglpshdef[idPlayer][i]) <= 0x0 ? L_4b81 : L_4b7b */

L_4b7b:
    return 0x1;

L_4b81:
    i = (i + 0x1);

L_4b85:
    if ((i < 0x10))
        goto L_4a83;
    else
        goto L_4b8e;

L_4b8e:
    return 0x0;
}

int16_t FIsTurinDroneAiAttack(FLEET *lpfl) {
    int16_t ihul;
    int16_t i;

L_4b9a:
    i = 0x0;
    goto L_4bf3;

L_4bab:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_4bef;
    else
        goto L_4bc8;

L_4bc8:
    ihul = rgshdef[i].hul.ihuldef;
    if ((ihul < 0x4))
        goto L_4bef;
    else
        goto L_4be0;

L_4be0:
    if ((ihul > 0xa))
        goto L_4bef;
    else
        goto L_4be9;

L_4be9:
    return 0x1;

L_4bef:
    i = (i + 0x1);

L_4bf3:
    if ((i < 0x10))
        goto L_4bab;
    else
        goto L_4bfc;

L_4bfc:
    return 0x0;
}

int16_t FIsAiTransport(FLEET *lpfl) {
    int16_t ihul;
    int16_t i;

L_4c08:
    i = 0x0;
    goto L_4cda;

L_4c19:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_4cd6;
    else
        goto L_4c36;

L_4c36:
    ihul = rgshdef[i].hul.ihuldef;
    if ((ihul < 0x0))
        goto L_4c57;
    else
        goto L_4c4e;

L_4c4e:
    if ((ihul <= 0x3))
        goto L_4c69;
    else
        goto L_4c57;

L_4c57:
    if ((ihul < 0xb))
        goto L_4c6f;
    else
        goto L_4c60;

L_4c60:
    if ((ihul > 0xd))
        goto L_4c6f;
    else
        goto L_4c69;

L_4c69:
    return 0x1;

L_4c6f:
    if ((ihul != 0x1f))
        goto L_4cd6;
    else
        goto L_4c78;

L_4c78:
    if ((WtMaxShdefStat(&(rgshdef[i]), 0x2) < 0x1f4))
        goto L_4cd6;
    else
        goto L_4c9b;

L_4c9b:
    /* untranslated: branch part[137:2](rglpshdef[idPlayer][i]) < 0x0 ? L_4cd6 : L_4cc0 */

L_4cc0:
    /* untranslated: branch part[137:2](rglpshdef[idPlayer][i]) > 0x0 ? L_4cd0 : L_4cc5 */

L_4cc5:
    /* untranslated: branch part[135:2](rglpshdef[idPlayer][i]) <= 0x0 ? L_4cd6 : L_4cd0 */

L_4cd0:
    return 0x1;

L_4cd6:
    i = (i + 0x1);

L_4cda:
    if ((i < 0x10))
        goto L_4c19;
    else
        goto L_4ce3;

L_4ce3:
    return 0x0;
}

void ValidateStarbaseHistory() {
    int16_t iWrite;
    int16_t iBest;
    PLANET *lpplMac;
    POINT   pt;
    int16_t id;
    int16_t dy;
    int16_t cFr2;
    PLANET *lppl;
    int16_t ifl;
    FLEET  *lpfl;
    int16_t i;
    int16_t j;
    int16_t ipl;
    int16_t cFr;
    int16_t dx;
    int32_t lBest;
    int32_t l;

L_4cf0:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x4))
        goto L_56ca;
    else
        goto L_4d18;

L_4d18:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x5))
        goto L_56ca;
    else
        goto L_4d34;

L_4d34:

L_4d3a:
    if ((LOWORD(vlpbAiData) > 0x2))
        goto L_4d51;
    else
        goto L_4d47;

L_4d47:
    vlpbAiData[0x2] = 0x0;

L_4d51:
    if ((game.turn < 0x14))
        goto L_56ca;
    else
        goto L_4d58;

L_4d58:

L_4d5e:
    iWrite = 0x0;
    if ((vlpbAiData[0x2] < 0x0))
        goto L_4d7f;
    else
        goto L_4d71;

L_4d71:
    if ((vlpbAiData[0x2] <= 0x40))
        goto L_4d89;
    else
        goto L_4d7f;

L_4d7f:
    vlpbAiData[0x2] = 0x0;

L_4d89:
    i = 0x0;
    goto L_4d95;

L_4d91:
    i = (i + 0x1);

L_4d95:
    if ((i >= vlpbAiData[0x2]))
        goto L_4ec0;
    else
        goto L_4da5;

L_4da5:
    if ((HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]) < 0x0))
        goto L_4de7;
    else
        goto L_4dc6;

L_4dc6:
    if ((HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]) <= 0x8))
        goto L_4e04;
    else
        goto L_4de7;

L_4de7:
    HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]) = 0x0;

L_4e04:
    lppl = LpplFromId(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]);
    if ((LOWORD(lppl) != 0x0))
        goto L_4e3e;
    else
        goto L_4e35;

L_4e35:
    if ((HIWORD(lppl) == 0x0))
        goto L_4d91;
    else
        goto L_4e3e;

L_4e3e:
    if ((lppl->iPlayer != idPlayer))
        goto L_4d91;
    else
        goto L_4e4a;

L_4e4a:

L_4e50:
    if ((iWrite == i))
        goto L_4eb9;
    else
        goto L_4e5b;

L_4e5b:
    vlpbAiData[(0x4 + LOWORD((0x14 * iWrite)))] = vlpbAiData[(0x4 + LOWORD((0x14 * i)))];
    /* untranslated: ss:[bp-0x34] = (LOWORD(vlpbAiData) + 0x4) */
    /* untranslated: ss:[bp-0x32] = HIWORD(vlpbAiData) */

L_4eb9:
    iWrite = (iWrite + 0x1);

L_4ec0:
    vlpbAiData[0x2] = iWrite;
    ipl = 0x0;
    goto L_4ed7;

L_4ed3:
    ipl = (ipl + 0x1);

L_4ed7:
    if ((ipl >= vclpplAi))
        goto L_4fe1;
    else
        goto L_4ee2;

L_4ee2:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_4f12;
    else
        goto L_4f0a;

L_4f0a:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_4fe1;
    else
        goto L_4f12;

L_4f12:
    if ((lppl->fStarbase == 0x0))
        goto L_4ed3;
    else
        goto L_4f26;

L_4f26:

L_4f2c:
    i = 0x0;
    goto L_4f38;

L_4f34:
    i = (i + 0x1);

L_4f38:
    if ((i >= vlpbAiData[0x2]))
        goto L_4f79;
    else
        goto L_4f48;

L_4f48:
    if ((vlpbAiData[(0x4 + LOWORD((0x14 * i)))] == lppl->id))
        goto L_4f79;
    else
        goto L_4f70;

L_4f70:

L_4f79:
    if ((i != vlpbAiData[0x2]))
        goto L_4ed3;
    else
        goto L_4f89;

L_4f89:
    if ((i >= 0x40))
        goto L_4ed3;
    else
        goto L_4f92;

L_4f92:
    vlpbAiData[(0x4 + LOWORD((0x14 * i)))] = lppl->id;
    HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]) = 0x0;
    vlpbAiData[0x2] = (vlpbAiData[0x2] + 0x1);

L_4fe1:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_52ea;

L_500c:
    if ((vlpbAiData[0x2] >= 0x40))
        goto L_52f8;
    else
        goto L_5017;

L_5017:

L_501d:
    if ((lppl->iPlayer != idPlayer))
        goto L_52e6;
    else
        goto L_502c;

L_502c:
    if ((lppl->fStarbase != 0x0))
        goto L_52e6;
    else
        goto L_5040;

L_5040:

L_5046:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_5065;
    else
        goto L_5053;

L_5053:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_52e6;
    else
        goto L_5058;

L_5058:
    if ((LOWORD(lppl->rgwtMin[0x3]) < 0x50))
        goto L_52e6;
    else
        goto L_505f;

L_505f:

L_5065:
    l = 0x0;
    i = 0x0;
    goto L_50e5;

L_5077:
    /* untranslated: l = (l + ((int32_t)((uint32_t)(words(0x0, lppl->rgMinConc[i]) * words(0x0, lppl->rgMinConc[i])) * 0x4) + lppl->rgwtMin[i])) */
    i = (i + 0x1);

L_50e5:
    if ((i < 0x3))
        goto L_5077;
    else
        goto L_50ee;

L_50ee:
    if ((HIWORD(l) < 0x0))
        goto L_52e6;
    else
        goto L_50f7;

L_50f7:
    if ((HIWORD(l) > 0x0))
        goto L_5106;
    else
        goto L_50fc;

L_50fc:
    if ((LOWORD(l) < 0x1b58))
        goto L_52e6;
    else
        goto L_5106;

L_5106:
    if ((0x0 < 0x0))
        goto L_52e6;
    else
        goto L_5127;

L_5127:
    if ((0x0 > 0x0))
        goto L_5134;
    else
        goto L_512c;

L_512c:
    /* untranslated: branch (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff) < 0x14 ? L_52e6 : L_5134 */

L_5134:
    if ((0x0 > 0x0))
        goto L_5165;
    else
        goto L_5155;

L_5155:
    if ((0x0 < 0x0))
        goto L_52e6;
    else
        goto L_515a;

L_515a:
    /* untranslated: branch (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff) < 0x14 ? L_52e6 : L_515f */

L_515f:

L_5165:
    i = 0x0;
    goto L_5171;

L_516d:
    i = (i + 0x1);

L_5171:
    if ((i >= vlpbAiData[0x2]))
        goto L_5265;
    else
        goto L_5181;

L_5181:
    id = vlpbAiData[(0x4 + LOWORD((0x14 * i)))];
    if ((id == lppl->id))
        goto L_5265;
    else
        goto L_51a9;

L_51a9:

L_51af:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x0))
        goto L_516d;
    else
        goto L_51ce;

L_51ce:
    pt.x = rgptPlan[id].x;
    pt.y = rgptPlan[id].y;
    dx = (pt.x - rgptPlan[lppl->id].x);
    dy = (pt.y - rgptPlan[lppl->id].y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(l) > 0x0))
        goto L_516d;
    else
        goto L_5250;

L_5250:
    if ((HIWORD(l) < 0x0))
        goto L_5265;
    else
        goto L_5255;

L_5255:
    if ((LOWORD(l) < 0x9c4))
        goto L_5265;
    else
        goto L_525c;

L_525c:

L_5265:
    if ((i < vlpbAiData[0x2]))
        goto L_52e6;
    else
        goto L_5272;

L_5272:

L_5278:
    /* untranslated: HIWORD(vlpbAiData):[((LOWORD(vlpbAiData) + 0x4) + loword((0x14 * vlpbAiData[0x2])))] = lppl->id */
    /* untranslated: HIWORD(vlpbAiData):[((LOWORD(vlpbAiData) + 0x4) + loword((0x14 * vlpbAiData[0x2])))+0x2] = 0x0 */
    vlpbAiData[0x2] = (vlpbAiData[0x2] + 0x1);

L_52e6:
    lppl = (lppl + 0x1);

L_52ea:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_500c;
    else
        goto L_52f8;

L_52f8:
    ifl = 0x0;
    goto L_5304;

L_5300:
    ifl = (ifl + 0x1);

L_5304:
    if ((ifl >= cFleet))
        goto L_5580;
    else
        goto L_530f;

L_530f:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_533f;
    else
        goto L_5337;

L_5337:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_5580;
    else
        goto L_533f;

L_533f:
    if ((lpfl->iPlayer != idPlayer))
        goto L_5300;
    else
        goto L_534e;

L_534e:
    if ((FIsAiTransport(lpfl) == 0x0))
        goto L_5300;
    else
        goto L_5361;

L_5361:

L_5367:
    i = 0x0;
    goto L_5373;

L_536f:
    i = (i + 0x1);

L_5373:
    if ((i >= vlpbAiData[0x2]))
        goto L_5418;
    else
        goto L_5383;

L_5383:
    j = 0x0;
    goto L_538f;

L_538b:
    j = (j + 0x1);

L_538f:
    if ((j >= HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))])))
        goto L_53ef;
    else
        goto L_53b2;

L_53b2:
    if ((vlpbAiData[(((0x4 + LOWORD((0x14 * i))) + 0x4) + (j * 0x2))] == lpfl->id))
        goto L_53ef;
    else
        goto L_53e6;

L_53e6:

L_53ef:
    if ((j < HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))])))
        goto L_5418;
    else
        goto L_540f;

L_540f:

L_5418:
    if ((i != vlpbAiData[0x2]))
        goto L_5300;
    else
        goto L_5428;

L_5428:
    iBest = 0xffff;
    lBest = 0x989680;
    i = 0x0;
    goto L_5443;

L_543f:
    i = (i + 0x1);

L_5443:
    if ((i >= vlpbAiData[0x2]))
        goto L_5525;
    else
        goto L_5453;

L_5453:
    if ((HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]) >= 0x8))
        goto L_543f;
    else
        goto L_5471;

L_5471:

L_5477:
    pt.x = rgptPlan[vlpbAiData[(0x4 + LOWORD((0x14 * i)))]].x;
    pt.y = rgptPlan[vlpbAiData[(0x4 + LOWORD((0x14 * i)))]].y;
    dx = (pt.x - lpfl->pt.x);
    dy = (pt.y - lpfl->pt.y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(l) > HIWORD(lBest)))
        goto L_543f;
    else
        goto L_5503;

L_5503:
    if ((HIWORD(l) < HIWORD(lBest)))
        goto L_5510;
    else
        goto L_5508;

L_5508:
    if ((LOWORD(l) >= LOWORD(lBest)))
        goto L_543f;
    else
        goto L_5510;

L_5510:
    lBest = l;
    iBest = i;

L_5525:
    if ((iBest == 0xffff))
        goto L_5300;
    else
        goto L_552e;

L_552e:
    /* untranslated: ss:[bp-0x32] = lpfl->id */
    HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * iBest)))]) = (HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * iBest)))]) + 0x1);
    /* untranslated: vlpbAiData[(((0x4 + loword((0x14 * iBest))) + 0x4) + (HIWORD(vlpbAiData[(0x4 + loword((0x14 * iBest)))]) * 0x2))] = ss:[bp-0x32] */

L_5580:
    i = 0x0;
    goto L_558c;

L_5588:
    i = (i + 0x1);

L_558c:
    if ((i >= vlpbAiData[0x2]))
        goto L_56b3;
    else
        goto L_559c;

L_559c:
    cFr = HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]);
    if ((cFr >= 0x4))
        goto L_5588;
    else
        goto L_55c0;

L_55c0:

L_55c6:
    j = 0x0;
    goto L_55d2;

L_55ce:
    j = (j + 0x1);

L_55d2:
    if ((j >= vlpbAiData[0x2]))
        goto L_5614;
    else
        goto L_55e2;

L_55e2:
    cFr2 = HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * j)))]);
    if ((cFr2 >= (cFr + 0x2)))
        goto L_5614;
    else
        goto L_560b;

L_560b:

L_5614:
    if ((j >= vlpbAiData[0x2]))
        goto L_5588;
    else
        goto L_5624;

L_5624:
    HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * j)))]) = (HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * j)))]) - 0x1);
    /* untranslated: ss:[bp-0x32] = vlpbAiData[(((0x4 + loword((0x14 * j))) + 0x4) + (HIWORD(vlpbAiData[(0x4 + loword((0x14 * j)))]) * 0x2))] */
    HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]) = (HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]) + 0x1);
    /* untranslated: vlpbAiData[(((0x4 + loword((0x14 * i))) + 0x4) + (HIWORD(vlpbAiData[(0x4 + loword((0x14 * i)))]) * 0x2))] = ss:[bp-0x32] */

L_56b3:
    LOWORD(vlpbAiData) = (LOWORD((vlpbAiData[0x2] * 0x14)) + 0x4);

L_56ca:
    return;
}

void GetResourcesAvailable(PLANET *lppl, int32_t *rgRes) {
    int16_t i;
    int32_t cRes;

L_56d0:
    EstMineralsMined(lppl, rgRes, 0xffffffff, 0x0);
    i = 0x0;
    goto L_5730;

L_56fe:
    rgRes[i] = (rgRes[i] + lppl->rgwtMin[i]);
    i = (i + 0x1);

L_5730:
    if ((i < 0x3))
        goto L_56fe;
    else
        goto L_5739;

L_5739:
    cRes = (uint32_t)(CResourcesAtPlanet(lppl, idPlayer));
    if ((sel.pl.fNoResearch != 0x0))
        goto L_57aa;
    else
        goto L_576f;

L_576f:
    if ((0x0 != 0x0))
        goto L_57aa;
    else
        goto L_5777;

L_5777:
    cRes = (cRes - (int32_t)(((uint32_t)((cRes * (uint32_t)((uint16_t)(rgplr[idPlayer].pctResearch)))) / 0x64)));

L_57aa:
    LOWORD(rgRes[0x3]) = LOWORD(cRes);
    *(rgRes + 0xe) = HIWORD(cRes);
    return;
}

void GetProdQCost(PLANET *lppl, int32_t *rgCost) {
    int32_t rgCostCur[4];
    PLPROD *lpplprod;
    int16_t i;
    int16_t j;
    PROD   *lpprod;

L_57c0:
    i = 0x0;
    goto L_57ea;

L_57d1:
    rgCost[i] = 0x0;
    i = (i + 0x1);

L_57ea:
    if ((i < 0x4))
        goto L_57d1;
    else
        goto L_57f3;

L_57f3:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_580d;
    else
        goto L_5800;

L_5800:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_58b8;
    else
        goto L_5807;

L_5807:

L_580d:
    lpplprod = lppl->lpplprod;
    i = 0x0;
    lpprod = lpplprod->rgprod;
    goto L_584c;

L_5839:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_584c:
    if ((i >= lpplprod->iprodMac))
        goto L_58b8;
    else
        goto L_585e;

L_585e:
    GetProductionCosts(lppl, lpprod, rgCostCur, idPlayer, 0x0);
    j = 0x0;
    goto L_58ac;

L_5886:
    rgCost[j] = (rgCost[j] + rgCostCur[j]);
    j = (j + 0x1);

L_58ac:
    if ((j >= 0x4))
        goto L_5839;
    else
        goto L_58b2;

L_58b2:

L_58b8:
    return;
}

void MergeAllShdefs(int16_t grbitish) {
    COLORREF crglpflW;
    FLEET   *rglpflW[32];
    int16_t  iMax;
    int16_t  iMin;
    int16_t  i;
    int16_t  ifl;
    FLEET   *lpfl;
    int16_t  grbit;
    int16_t  rgish[16];
    FLEET   *lpflNextPass;
    int16_t  iflNextPass;

L_58be:
    iMax = 0xffff;
    iMin = 0x10;
    i = 0x0;
    grbit = 0x1;
    goto L_58f9;

L_58e5:
    i = (i + 0x1);
    grbit = (grbit * 0x2);

L_58f9:
    if ((i >= 0x10))
        goto L_5962;
    else
        goto L_5903;

L_5903:
    if (((grbitish & grbit) == 0x0))
        goto L_594f;
    else
        goto L_5912;

L_5912:
    rgish[i] = 0x1;
    if ((i <= iMax))
        goto L_5937;
    else
        goto L_592f;

L_592f:
    iMax = i;

L_5937:
    if ((i >= iMin))
        goto L_58e5;
    else
        goto L_5944;

L_5944:
    iMin = i;

L_594f:
    rgish[i] = 0x0;

L_5962:
    iMax = (iMax + 0x1);
    if ((iMax <= 0x0))
        goto L_5ba7;
    else
        goto L_596e;

L_596e:

L_5974:
    lpflNextPass = 0x0;
    LOWORD(crglpflW) = 0x0;
    ifl = 0x0;
    goto L_5993;

L_598e:
    ifl = (ifl + 0x1);

L_5993:
    if ((ifl >= cFleet))
        goto L_5b67;
    else
        goto L_599f;

L_599f:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto NextPass;
    else
        goto L_59ca;

L_59ca:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_5b67;
    else
        goto NextPass;

NextPass:
    if ((lpfl->iPlayer != idPlayer))
        goto L_598e;
    else
        goto L_59df;

L_59df:

L_59e5:
    i = iMin;
    goto L_5a2c;

L_59f0:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_5a27;
    else
        goto L_5a10;

L_5a10:
    if ((rgish[i] > 0x0))
        goto L_5a39;
    else
        goto L_5a21;

L_5a21:

L_5a27:
    i = (i + 0x1);

L_5a2c:
    if ((i < iMax))
        goto L_59f0;
    else
        goto L_5a39;

L_5a39:
    if ((i == iMax))
        goto L_598e;
    else
        goto L_5a43;

L_5a43:

L_5a49:
    if ((LOWORD(CMineFromLpfl(lpfl)) != 0xfa0))
        goto L_5a6c;
    else
        goto L_5a61;

L_5a61:
    /* untranslated: branch hiword(callresult(int32_t)) == 0x0 ? L_598e : L_5a66 */

L_5a66:

L_5a6c:
    i = 0x0;
    goto L_5acc;

L_5a75:
    if ((rglpflW[i]->idPlanet != lpfl->idPlanet))
        goto L_5ac7;
    else
        goto L_5a96;

L_5a96:
    if ((rglpflW[i]->pt.x != lpfl->pt.x))
        goto L_5ac7;
    else
        goto L_5abb;

L_5abb:
    if ((rglpflW[i]->pt.y == lpfl->pt.y))
        goto L_5ad8;
    else
        goto L_5ac1;

L_5ac1:

L_5ac7:
    i = (i + 0x1);

L_5acc:
    if ((i < LOWORD(crglpflW)))
        goto L_5a75;
    else
        goto L_5ad8;

L_5ad8:
    if ((i >= LOWORD(crglpflW)))
        goto L_5b0e;
    else
        goto L_5ae4;

L_5ae4:
    Merge2Fleets(rglpflW[i], lpfl, 0x0);
    goto L_598e;

L_5b0e:
    if ((LOWORD(crglpflW) != 0x20))
        goto L_5b46;
    else
        goto L_5b17;

L_5b17:
    if ((LOWORD(lpflNextPass) != 0x0))
        goto L_598e;
    else
        goto L_5b21;

L_5b21:
    if ((HIWORD(lpflNextPass) != 0x0))
        goto L_598e;
    else
        goto L_5b2b;

L_5b2b:
    lpflNextPass = lpfl;
    iflNextPass = ifl;

L_5b46:
    LOWORD(crglpflW) = (LOWORD(crglpflW) + 0x1);
    rglpflW[LOWORD(crglpflW)] = lpfl;

L_5b67:
    if ((LOWORD(lpflNextPass) != 0x0))
        goto L_5b7b;
    else
        goto L_5b71;

L_5b71:
    if ((HIWORD(lpflNextPass) == 0x0))
        goto L_5ba7;
    else
        goto L_5b7b;

L_5b7b:
    lpfl = lpflNextPass;
    lpflNextPass = 0x0;
    ifl = iflNextPass;
    LOWORD(crglpflW) = 0x0;
    goto NextPass;

L_5ba7:
    return;
}

FLEET *LpflFindClosestEnum(FLEET *lpfl, int16_t (**pfn)(FLEET *, FLEET *)) {
    FLEET  *lpflT;
    POINT   pt;
    int16_t dy;
    int16_t ish;
    int16_t dx;
    FLEET  *lpflBest;
    int32_t l;
    int32_t lBest;

L_5bae:
    lBest = 0x989680;
    lpflBest = 0x0;
    pt.x = lpfl->pt.x;
    pt.y = lpfl->pt.y;
    ish = 0x0;
    goto L_5be8;

L_5be4:
    ish = (ish + 0x1);

L_5be8:
    if ((ish >= cFleet))
        goto L_5cc5;
    else
        goto L_5bf3;

L_5bf3:
    lpflT = rglpfl[ish];
    if ((LOWORD(rglpfl[ish]) != 0x0))
        goto L_5c23;
    else
        goto L_5c1b;

L_5c1b:
    if ((*(rglpfl[ish] + 0x2) == 0x0))
        goto L_5cc5;
    else
        goto L_5c23;

L_5c23:
    dx = (pt.x - lpflT->pt.x);
    dy = (pt.y - lpflT->pt.y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(l) > HIWORD(lBest)))
        goto L_5be4;
    else
        goto L_5c83;

L_5c83:
    if ((HIWORD(l) < HIWORD(lBest)))
        goto L_5c90;
    else
        goto L_5c88;

L_5c88:
    if ((LOWORD(l) >= LOWORD(lBest)))
        goto L_5be4;
    else
        goto L_5c90;

L_5c90:
    if ((pfn(lpfl, lpflT) == 0x0))
        goto L_5be4;
    else
        goto L_5caa;

L_5caa:
    lpflBest = lpflT;
    lBest = l;

L_5cc5:

L_5cce:
    return lpflBest;
}

PLANET *LpplFindClosestEnum(PLANET *lppl, int16_t (**pfn)(PLANET *, PLANET *)) {
    POINT   pt;
    int16_t dy;
    PLANET *lpplTMac;
    PLANET *lpplBest;
    PLANET *lpplT;
    int16_t dx;
    int32_t l;
    int32_t lBest;

L_5cd4:
    lBest = 0x989680;
    lpplBest = 0x0;
    pt.x = rgptPlan[lppl->id].x;
    pt.y = rgptPlan[lppl->id].y;
    lpplT = lpPlanets;
    lpplTMac = &(lpPlanets[cPlanet]);
    goto L_5de9;

L_5d34:
    dx = (pt.x - rgptPlan[lpplT->id].x);
    dy = (pt.y - rgptPlan[lpplT->id].y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(l) > HIWORD(lBest)))
        goto L_5de5;
    else
        goto L_5da6;

L_5da6:
    if ((HIWORD(l) < HIWORD(lBest)))
        goto L_5db3;
    else
        goto L_5dab;

L_5dab:
    if ((LOWORD(l) >= LOWORD(lBest)))
        goto L_5de5;
    else
        goto L_5db3;

L_5db3:
    if ((pfn(lppl, lpplT) == 0x0))
        goto L_5de5;
    else
        goto L_5dcd;

L_5dcd:
    lpplBest = lpplT;
    lBest = l;

L_5de5:
    lpplT = (lpplT + 0x1);

L_5de9:
    if ((LOWORD(lpplT) < LOWORD(lpplTMac)))
        goto L_5d34;
    else
        goto L_5df7;

L_5df7:

L_5e00:
    return lpplBest;
}

PLANET *LpplFindBestEnum(PLANET *lppl, int16_t (**pfn)(PLANET *, PLANET *)) {
    int16_t iBest;
    POINT   pt;
    int16_t dy;
    PLANET *lpplTMac;
    PLANET *lpplBest;
    PLANET *lpplT;
    int16_t iCur;
    int16_t dx;
    int32_t l;
    int32_t lBest;

L_5e06:
    iBest = 0x1;
    lBest = 0x989680;
    lpplBest = 0x0;
    pt.x = rgptPlan[lppl->id].x;
    pt.y = rgptPlan[lppl->id].y;
    lpplT = lpPlanets;
    lpplTMac = &(lpPlanets[cPlanet]);
    goto L_5f37;

L_5e6b:
    dx = (pt.x - rgptPlan[lpplT->id].x);
    dy = (pt.y - rgptPlan[lpplT->id].y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    iCur = pfn(lppl, lpplT);
    if ((iCur > iBest))
        goto L_5f15;
    else
        goto L_5eef;

L_5eef:
    if ((iCur != iBest))
        goto L_5f33;
    else
        goto L_5efa;

L_5efa:
    if ((HIWORD(l) > HIWORD(lBest)))
        goto L_5f33;
    else
        goto L_5f08;

L_5f08:
    if ((HIWORD(l) < HIWORD(lBest)))
        goto L_5f15;
    else
        goto L_5f0d;

L_5f0d:
    if ((LOWORD(l) >= LOWORD(lBest)))
        goto L_5f33;
    else
        goto L_5f15;

L_5f15:
    iBest = iCur;
    lBest = l;
    lpplBest = lpplT;

L_5f33:
    lpplT = (lpplT + 0x1);

L_5f37:
    if ((LOWORD(lpplT) < LOWORD(lpplTMac)))
        goto L_5e6b;
    else
        goto L_5f45;

L_5f45:

L_5f4e:
    return lpplBest;
}

int16_t IdRandomPlanetNearby(POINT pt, int16_t cDist, int16_t fAvoidStarbases) {
    int16_t  iChance;
    int32_t  lDistMax;
    int32_t  dy;
    int16_t  idBest;
    int16_t  i;
    int32_t  dx;
    int16_t  cExtraAttempts;
    int32_t  d2Cur;
    PLANET  *lppl;
    uint16_t t_merge_5f6f_0001;
    int16_t  t_603d;
    int16_t  t_6067;

L_5f54:
    if ((fAvoidStarbases == 0x0))
        goto L_5f6c;
    else
        goto L_5f66;

L_5f66:
    t_merge_5f6f_0001 = 0x2;
    goto L_5f6f;

L_5f6c:
    t_merge_5f6f_0001 = 0x0;

L_5f6f:
    cExtraAttempts = t_merge_5f6f_0001;

LRetry:
    lDistMax = (uint32_t)(((uint32_t)(cDist) * (uint32_t)(cDist)));
    iChance = 0x1;
    idBest = 0xffff;
    i = 0x0;
    goto L_5f9f;

L_5f9b:
    i = (i + 0x1);

L_5f9f:
    if ((i >= game.cPlanMax))
        goto L_605e;
    else
        goto L_5faa;

L_5faa:
    dx = (uint32_t)(abs((pt.x - rgptPlan[i].x)));
    dy = (uint32_t)(abs((pt.y - rgptPlan[i].y)));
    d2Cur = ((uint32_t)((dx * dx)) + (uint32_t)((dy * dy)));
    if ((HIWORD(d2Cur) > HIWORD(lDistMax)))
        goto L_5f9b;
    else
        goto L_6030;

L_6030:
    if ((HIWORD(d2Cur) < HIWORD(lDistMax)))
        goto L_603d;
    else
        goto L_6035;

L_6035:
    if ((LOWORD(d2Cur) > LOWORD(lDistMax)))
        goto L_5f9b;
    else
        goto L_603d;

L_603d:
    t_603d = iChance;
    iChance = (iChance + 0x1);
    if ((Random(t_603d) != 0x0))
        goto L_5f9b;
    else
        goto L_6055;

L_6055:
    idBest = i;

L_605e:
    if ((idBest == 0xffff))
        goto L_60b3;
    else
        goto L_6067;

L_6067:
    t_6067 = cExtraAttempts;
    cExtraAttempts = (cExtraAttempts - 0x1);
    if ((t_6067 <= 0x0))
        goto L_60b3;
    else
        goto L_6076;

L_6076:
    lppl = LpplFromId(idBest);
    if ((LOWORD(lppl) != 0x0))
        goto L_6099;
    else
        goto L_6090;

L_6090:
    if ((HIWORD(lppl) == 0x0))
        goto L_60b3;
    else
        goto L_6099;

L_6099:
    if ((lppl->fStarbase != 0x0))
        goto LRetry;
    else
        goto L_60ad;

L_60ad:

L_60b3:

L_60b9:
    return idBest;
}

void ClearAiCurrentTask(FLEET *lpfl, int16_t fChangeSel) {
L_60c0:
    if ((fChangeSel == 0x0))
        goto L_60e4;
    else
        goto L_60d2;

L_60d2:
    ChangeMainObjSel(grobjFleet, lpfl->id);

L_60e4:
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x0) */
    FLookupFleet(0xffff, &(sel.fl));
    return;
}

int16_t FEnumOurStarbase(PLANET *lpplSrc, PLANET *lpplTest) {
L_6110:
    if ((lpplTest->iPlayer != idPlayer))
        goto L_6145;
    else
        goto L_6128;

L_6128:
    if ((lpplTest->fStarbase == 0x0))
        goto L_6145;
    else
        goto L_613f;

L_613f:
    return 0x1;

L_6145:
    return 0x0;
}

int16_t FFleetMightHaveTeeth(FLEET *lpfl) {
    HUL    *lphul;
    int16_t ishdef;

L_6152:
    ishdef = 0x0;
    goto L_61c8;

L_6163:
    if ((lpfl->rgcsh[ishdef] == 0x0))
        goto L_61c4;
    else
        goto L_6180;

L_6180:
    lphul = &(rglpshdef[lpfl->iplr][ishdef]);
    if ((FHullHasTeeth(lphul) == 0x0))
        goto L_61c4;
    else
        goto L_61be;

L_61be:
    return 0x1;

L_61c4:
    ishdef = (ishdef + 0x1);

L_61c8:
    if ((ishdef < 0x10))
        goto L_6163;
    else
        goto L_61d1;

L_61d1:
    return 0x0;
}

int16_t IdTargetScout(FLEET *lpfl, FLEET *lpflAtk, FLEET *lpflEnemy, int16_t fOnlyHumans, THING **plpthWorm) {
    FLEET  *lpflClosest;
    FLEET  *lpflT;
    int32_t lDistBest;
    PLANET *lpplMac;
    POINT   pt;
    int16_t dy;
    PLANET *lppl;
    int32_t lDist;
    int16_t idClosest;
    PLANET *lpplClosest;
    int16_t dx;
    FLEET  *lpflAtk2;
    ORDER   ord;

L_61de:
    lpflClosest = 0x0;
    lpflT = lpflEnemy;
    lDistBest = 0xf4240;
    pt.x = lpfl->pt.x;
    pt.y = lpfl->pt.y;
    memset(ord, 0x0, 0x12);
    if ((FFleetMightHaveTeeth(lpfl) == 0x0))
        goto LFindPlanet;
    else
        goto L_623f;

L_623f:

L_6248:
    if ((fOnlyHumans == 0x0))
        goto L_6276;
    else
        goto L_6251;

L_6251:
    if ((((rgplr[lpflT->iPlayer].wMdPlr >> 0x9) & 0x1) != 0x0))
        goto NextTarg;
    else
        goto L_6270;

L_6270:

L_6276:
    lpflAtk2 = lpflAtk;
    goto L_62ed;

L_6285:
    if ((LOWORD(lpflAtk2) != LOWORD(lpfl)))
        goto L_629b;
    else
        goto L_6293;

L_6293:
    if ((HIWORD(lpflAtk2) == HIWORD(lpfl)))
        goto L_62dc;
    else
        goto L_629b;

L_629b:
    if ((lpflAtk2->cord < 0x2))
        goto L_62dc;
    else
        goto L_62a8;

L_62a8:
    if ((lpflAtk2->lpplord->rgord[0x1].id != lpflT->id))
        goto L_62dc;
    else
        goto L_62be;

L_62be:
    /* untranslated: branch ((part[6:2](lpflAtk2->lpplord->rgord[0x1]) >> 0x8) & 0xf) == 0x2 ? L_62ff : L_62d6 */

L_62d6:

L_62dc:
    lpflAtk2 = lpflAtk2->lpflNext;

L_62ed:
    if ((LOWORD(lpflAtk2) != 0x0))
        goto L_6285;
    else
        goto L_62f6;

L_62f6:
    if ((HIWORD(lpflAtk2) != 0x0))
        goto L_6285;
    else
        goto L_62ff;

L_62ff:
    if ((LOWORD(lpflAtk2) != 0x0))
        goto L_6311;
    else
        goto L_6308;

L_6308:
    if ((HIWORD(lpflAtk2) == 0x0))
        goto L_6328;
    else
        goto L_6311;

L_6311:
    if ((Random(0x3) == 0x0))
        goto NextTarg;
    else
        goto L_6322;

L_6322:

L_6328:
    dx = (lpflT->pt.x - pt.x);
    dy = (lpflT->pt.y - pt.y);
    lDist = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lDist) > HIWORD(lDistBest)))
        goto NextTarg;
    else
        goto L_6388;

L_6388:
    if ((HIWORD(lDist) < HIWORD(lDistBest)))
        goto L_6395;
    else
        goto L_638d;

L_638d:
    if ((LOWORD(lDist) >= LOWORD(lDistBest)))
        goto NextTarg;
    else
        goto L_6395;

L_6395:
    lDistBest = lDist;
    lpflClosest = lpflT;

NextTarg:
    lpflT = lpflT->lpflNext;

L_63be:
    if ((LOWORD(lpflT) != 0x0))
        goto L_6248;
    else
        goto L_63c7;

L_63c7:
    if ((HIWORD(lpflT) != 0x0))
        goto L_6248;
    else
        goto L_63d0;

L_63d0:
    if ((HIWORD(lDistBest) > 0x0))
        goto L_6411;
    else
        goto L_63d9;

L_63d9:
    if ((HIWORD(lDistBest) < 0x0))
        goto L_63e8;
    else
        goto L_63de;

L_63de:
    if ((LOWORD(lDistBest) >= 0x7e90))
        goto L_6411;
    else
        goto L_63e8;

L_63e8:
    ord.pt.x = lpflClosest->pt.x;
    ord.pt.y = lpflClosest->pt.y;
    ord.grobj = grobjFleet;
    ord.id = lpflClosest->id;
    goto ThwakSumthin;

L_6411:
    if ((LOWORD(lpflClosest) != 0x0))
        goto L_6423;
    else
        goto L_641a;

L_641a:
    if ((HIWORD(lpflClosest) == 0x0))
        goto L_666c;
    else
        goto L_6423;

L_6423:
    if ((HIWORD(lpfl->rgwtMin[0x4]) > HIWORD((int32_t)((LGetFleetStat(lpfl, 0x1) / 0x2)))))
        goto L_647e;
    else
        goto L_6450;

L_6450:
    /* untranslated: branch HIWORD(lpfl->rgwtMin[0x4]) < hiword((int32_t)(callresult(int32_t) / 0x2)) ? L_645e : L_6455 */

L_6455:
    /* untranslated: branch LOWORD(lpfl->rgwtMin[0x4]) >= loword((int32_t)(callresult(int32_t) / 0x2)) ? L_647e : L_645e */

L_645e:
    if ((FMoveToNearestStarbase(lpfl, 0x0) == 0x0))
        goto L_647e;
    else
        goto L_6478;

L_6478:
    return 0x0;

L_647e:
    pt.x = lpflClosest->pt.x;
    pt.y = lpflClosest->pt.y;
    lpplClosest = 0x0;
    lDistBest = 0xf4240;
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_6607;

L_64ce:
    lpflAtk2 = lpflAtk;
    goto L_6545;

L_64dd:
    if ((LOWORD(lpflAtk2) != LOWORD(lpfl)))
        goto L_64f3;
    else
        goto L_64eb;

L_64eb:
    if ((HIWORD(lpflAtk2) == HIWORD(lpfl)))
        goto L_6534;
    else
        goto L_64f3;

L_64f3:
    if ((lpflAtk2->cord < 0x2))
        goto L_6534;
    else
        goto L_6500;

L_6500:
    if ((lpflAtk2->lpplord->rgord[0x1].id != lppl->id))
        goto L_6534;
    else
        goto L_6516;

L_6516:
    /* untranslated: branch ((part[6:2](lpflAtk2->lpplord->rgord[0x1]) >> 0x8) & 0xf) == 0x1 ? L_6557 : L_652e */

L_652e:

L_6534:
    lpflAtk2 = lpflAtk2->lpflNext;

L_6545:
    if ((LOWORD(lpflAtk2) != 0x0))
        goto L_64dd;
    else
        goto L_654e;

L_654e:
    if ((HIWORD(lpflAtk2) != 0x0))
        goto L_64dd;
    else
        goto L_6557;

L_6557:
    if ((LOWORD(lpflAtk2) != 0x0))
        goto L_6603;
    else
        goto L_6560;

L_6560:
    if ((HIWORD(lpflAtk2) != 0x0))
        goto L_6603;
    else
        goto L_6566;

L_6566:

L_656c:
    dx = (rgptPlan[lppl->id].x - pt.x);
    dy = (rgptPlan[lppl->id].y - pt.y);
    lDist = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lDist) > HIWORD(lDistBest)))
        goto L_6603;
    else
        goto L_65de;

L_65de:
    if ((HIWORD(lDist) < HIWORD(lDistBest)))
        goto L_65eb;
    else
        goto L_65e3;

L_65e3:
    if ((LOWORD(lDist) >= LOWORD(lDistBest)))
        goto L_6603;
    else
        goto L_65eb;

L_65eb:
    lDistBest = lDist;
    lpplClosest = lppl;

L_6603:
    lppl = (lppl + 0x1);

L_6607:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_64ce;
    else
        goto L_6615;

L_6615:
    if ((LOWORD(lpplClosest) != 0x0))
        goto L_6627;
    else
        goto L_661e;

L_661e:
    if ((HIWORD(lpplClosest) == 0x0))
        goto LFindPlanet;
    else
        goto L_6627;

L_6627:
    if ((lpplClosest->id == lpfl->idPlanet))
        goto LFindPlanet;
    else
        goto L_6639;

L_6639:
    ord.pt.x = rgptPlan[lpplClosest->id].x;
    ord.pt.y = rgptPlan[lpplClosest->id].y;
    ord.grobj = grobjPlanet;
    ord.id = lpplClosest->id;
    goto ThwakSumthin;

L_666c:
    if ((fOnlyHumans == 0x0))
        goto LFindPlanet;
    else
        goto L_6675;

L_6675:
    return IdTargetScout(lpfl, lpflAtk, lpflEnemy, 0x0, plpthWorm);

LFindPlanet:
    idClosest = IdNearestUnknownPlanet(lpfl, plpthWorm);
    if ((plpthWorm == 0x0))
        goto L_66f7;
    else
        goto L_66b6;

L_66b6:
    if ((LOWORD(*(plpthWorm)) != 0x0))
        goto L_66ca;
    else
        goto L_66c1;

L_66c1:
    if ((HIWORD(*(plpthWorm)) == 0x0))
        goto L_66f7;
    else
        goto L_66ca;

L_66ca:
    ord.pt.x = *(plpthWorm)->pt.x;
    ord.pt.y = *(plpthWorm)->pt.y;
    ord.grobj = grobjThing;
    ord.id = *(plpthWorm)->idFull;
    goto ThwakSumthin;

L_66f7:
    if ((idClosest != 0xffff))
        goto L_6783;
    else
        goto L_6700;

L_6700:
    if ((rgplr[idPlayer].idPlanetHome != 0xffff))
        goto L_6727;
    else
        goto L_6715;

L_6715:
    idClosest = Random(game.cPlanMax);
    goto L_6783;

L_6727:
    lpplClosest = LpplFindBestEnum(LpplFromId(rgplr[idPlayer].idPlanetHome), FEnumCalcArmadaDest);
    if ((LOWORD(lpplClosest) != 0x0))
        goto L_677a;
    else
        goto L_675f;

L_675f:
    if ((HIWORD(lpplClosest) != 0x0))
        goto L_677a;
    else
        goto L_6768;

L_6768:
    idClosest = Random(game.cPlanMax);
    goto L_6783;

L_677a:
    idClosest = lpplClosest->id;

L_6783:
    ord.id = idClosest;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[idClosest].x;
    ord.pt.y = rgptPlan[idClosest].y;

ThwakSumthin:
    if ((lpfl->cord <= 0x1))
        goto L_67eb;
    else
        goto L_67b7;

L_67b7:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_67eb : L_67d2 */

L_67d2:
    if ((ord.grobj != grobjPlanet))
        goto L_67eb;
    else
        goto L_67e5;

L_67e5:
    return 0xffff;

L_67eb:
    if ((ord.grobj != grobjPlanet))
        goto L_681c;
    else
        goto L_67fe;

L_67fe:
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((ord.id * 0x10) + 0xf))] = 0x4 */

L_681c:
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = IFindIdealWarp(lpfl, 0x1);
    if ((FMoveAiFleet(lpfl, &(ord), 0x0) != 0x0))
        goto L_6889;
    else
        goto L_6883;

L_6883:
    return 0xffff;

L_6889:
    return 0x0;
}

void MarkPlanetsUnderAttack() {
    PLANET *lppl;
    int16_t i;
    int16_t ifl;
    FLEET  *lpfl;
    int16_t j;

L_6896:
    ifl = 0x0;
    goto L_68ab;

L_68a7:
    ifl = (ifl + 0x1);

L_68ab:
    if ((ifl >= cFleet))
        goto L_69df;
    else
        goto L_68b6;

L_68b6:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_68e6;
    else
        goto L_68de;

L_68de:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_69df;
    else
        goto L_68e6;

L_68e6:
    if ((lpfl->iPlayer == idPlayer))
        goto L_68a7;
    else
        goto L_68f5;

L_68f5:
    if ((lpfl->idPlanet == 0xffff))
        goto L_68a7;
    else
        goto L_68ff;

L_68ff:

L_6905:
    j = lpfl->iPlayer;
    i = 0x0;
    goto L_697f;

L_6917:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_697b;
    else
        goto L_6934;

L_6934:
    if ((rglpshdef[j][i].hul.ihuldef < ihuldefMiniBomber))
        goto L_697b;
    else
        goto L_6956;

L_6956:
    if ((rglpshdef[j][i].hul.ihuldef <= ihuldefB52Bomber))
        goto L_6988;
    else
        goto L_6975;

L_6975:

L_697b:
    i = (i + 0x1);

L_697f:
    if ((i < 0x10))
        goto L_6917;
    else
        goto L_6988;

L_6988:
    if ((i == 0x10))
        goto L_68a7;
    else
        goto L_698e;

L_698e:

L_6994:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((lppl->iPlayer != idPlayer))
        goto L_68a7;
    else
        goto L_69b5;

L_69b5:

L_69bb:
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x8))] = 0x1 */
    goto L_68a7;

L_69df:
    return;
}

void FixPlanetsUnderAttack(PROD *rgprod) {
    PLANET *lppl;
    int16_t ipl;

L_69e6:
    ipl = 0x0;
    goto L_69fb;

L_69f7:
    ipl = (ipl + 0x1);

L_69fb:
    if ((ipl >= vclpplAi))
        goto L_6a78;
    else
        goto L_6a06;

L_6a06:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_6a36;
    else
        goto L_6a2e;

L_6a2e:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_6a78;
    else
        goto L_6a36;

L_6a36:
    /* untranslated: branch byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x8))] == 0x0 ? L_69f7 : L_6a5e */

L_6a5e:

L_6a64:
    QuickBuildDefenses(lppl, rgprod);
    goto L_69f7;

L_6a78:
    return;
}

void QuickBuildDefenses(PLANET *lppl, PROD *rgprod) {
    int16_t  cMax;
    int16_t  cAlch;
    int16_t  cCur;
    int16_t  i;
    int16_t  cRes;
    int32_t  lVal;
    int16_t  cDef;
    int32_t  rgRes[4];
    PROD    *lpprod;
    uint32_t t_merge_6d52_0001_wide;
    uint16_t t_merge_6df5_0001;

L_6a7e:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_6a9e;
    else
        goto L_6a94;

L_6a94:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_6b3b;
    else
        goto L_6a9e;

L_6a9e:
    i = 0x0;
    lpprod = lppl->lpplprod->rgprod;
    goto L_6acf;

L_6abc:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_6acf:
    if ((i >= lppl->lpplprod->iprodMac))
        goto L_6b3b;
    else
        goto L_6ae5;

L_6ae5:
    if ((lpprod->grobj != 0x1))
        goto L_6abc;
    else
        goto L_6b05;

L_6b05:
    if ((0x0 != 0x0))
        goto L_6abc;
    else
        goto L_6b0d;

L_6b0d:
    if ((lpprod->iItem != 0x9))
        goto L_6abc;
    else
        goto L_6b2d;

L_6b2d:
    if ((0x0 == 0x0))
        goto L_6dfe;
    else
        goto L_6b32;

L_6b32:

L_6b3b:
    GetResourcesAvailable(lppl, rgRes);
    if ((HIWORD(rgRes[0x3]) > 0x0))
        goto L_6b67;
    else
        goto L_6b56;

L_6b56:
    if ((HIWORD(rgRes[0x3]) < 0x0))
        goto L_6dfe;
    else
        goto L_6b5b;

L_6b5b:
    if ((LOWORD(rgRes[0x3]) < 0x32))
        goto L_6dfe;
    else
        goto L_6b61;

L_6b61:

L_6b67:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    i = 0x0;
    goto L_6c20;

L_6b8c:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0x11))) & 0x7) != 0x1))
        goto L_6c1c;
    else
        goto L_6bb4;

L_6bb4:
    if ((0x0 != 0x0))
        goto L_6c1c;
    else
        goto L_6bbc;

L_6bbc:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x7f) != 0x9))
        goto L_6c1c;
    else
        goto L_6be4;

L_6be4:
    if ((0x0 != 0x0))
        goto L_6c1c;
    else
        goto L_6bec;

L_6bec:
    if ((0x0 < 0x0))
        goto L_6c1c;
    else
        goto L_6c0c;

L_6c0c:
    if ((0x0 > 0x0))
        goto L_6c2b;
    else
        goto L_6c11;

L_6c11:
    if (((LOWORD(pProdGlob[i]) & 0x3ff) >= 0x1))
        goto L_6c2b;
    else
        goto L_6c16;

L_6c16:

L_6c1c:
    i = (i + 0x1);

L_6c20:
    if ((i < cProdGlob))
        goto L_6b8c;
    else
        goto L_6c2b;

L_6c2b:
    if ((i != cProdGlob))
        goto L_6c45;
    else
        goto L_6c36;

L_6c36:
    FinishProduction(0x0);
    goto L_6dfe;

L_6c45:
    cMax = (LOWORD(pProdGlob[i]) & 0x3ff);
    cCur = 0x64;
    i = 0x0;
    goto L_6cb7;

L_6c6d:
    lVal = (int32_t)((rgRes[i] / 0x5));
    if ((HIWORD(lVal) > SIGNHIWORD(cCur)))
        goto L_6cb3;
    else
        goto L_6c9d;

L_6c9d:
    if ((HIWORD(lVal) < SIGNHIWORD(cCur)))
        goto L_6caa;
    else
        goto L_6ca2;

L_6ca2:
    if ((LOWORD(lVal) >= cCur))
        goto L_6cb3;
    else
        goto L_6caa;

L_6caa:
    cCur = LOWORD(lVal);

L_6cb3:
    i = (i + 0x1);

L_6cb7:
    if ((i < 0x3))
        goto L_6c6d;
    else
        goto L_6cc0;

L_6cc0:
    cRes = LOWORD((int32_t)((rgRes[0x3] / 0x19)));
    if ((cRes <= 0x5))
        goto L_6ceb;
    else
        goto L_6cdf;

L_6cdf:
    cRes = (cRes - ((uint32_t)(cRes) / 0x6));

L_6ceb:
    rgRes[0x3] = (rgRes[0x3] - (int32_t)((rgRes[0x3] / 0xa)));
    if ((cRes <= cMax))
        goto L_6d15;
    else
        goto L_6d0f;

L_6d0f:
    cRes = cMax;

L_6d15:
    if ((cRes <= cCur))
        goto L_6d8d;
    else
        goto L_6d20;

L_6d20:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceMineralAlchemy) == 0x0))
        goto L_6d4f;
    else
        goto L_6d49;

L_6d49:
    t_merge_6d52_0001_wide = 0x190019;
    goto L_6d52;

L_6d4f:
    t_merge_6d52_0001_wide = 0x640064;

L_6d52:
    /* untranslated: cAlch = loword((int32_t)(words((HIWORD(rgRes[0x3]) - signhiword(loword((loword(t_merge_6d52_0001_wide) * cCur)))), (LOWORD(rgRes[0x3]) -
     * loword((hiword(t_merge_6d52_0001_wide) * cCur)))) / 0x96)) */
    if ((cAlch >= 0x0))
        goto L_6d78;
    else
        goto L_6d73;

L_6d73:
    cAlch = 0x0;

L_6d78:
    cDef = (cCur + cAlch);
    cAlch = LOWORD((0x5 * cAlch));
    goto L_6d98;

L_6d8d:
    cAlch = 0x0;
    cDef = cRes;

L_6d98:
    if ((cDef <= 0x0))
        goto L_6db9;
    else
        goto L_6da1;

L_6da1:
    AddItemToQueue(0x9, cDef, grobjPlanet, 0x0);

L_6db9:
    if ((cAlch <= 0x0))
        goto L_6dda;
    else
        goto L_6dc2;

L_6dc2:
    AddItemToQueue(0xb, cAlch, grobjPlanet, 0x0);

L_6dda:
    if ((cDef > 0x0))
        goto L_6dec;
    else
        goto L_6de3;

L_6de3:
    if ((cAlch <= 0x0))
        goto L_6df2;
    else
        goto L_6dec;

L_6dec:
    t_merge_6df5_0001 = 0x1;
    goto L_6df5;

L_6df2:
    t_merge_6df5_0001 = 0x0;

L_6df5:
    FinishProduction(t_merge_6df5_0001);

L_6dfe:
    return;
}

int16_t IdplFindClosestStarbase(FLEET *lpfl, int16_t fBigOnes) {
    POINT   pt;
    int16_t dy;
    PLANET *lpplTMac;
    PLANET *lpplBest;
    PLANET *lpplT;
    int16_t dx;
    int32_t l;
    int32_t lBest;

L_6e04:
    lBest = 0x989680;
    lpplBest = 0x0;
    pt.x = lpfl->lpplord->rgord[0x0].pt.x;
    pt.y = lpfl->lpplord->rgord[0x0].pt.y;
    lpplT = lpPlanets;
    lpplTMac = &(lpPlanets[cPlanet]);
    goto L_6f48;

L_6e61:
    dx = (pt.x - rgptPlan[lpplT->id].x);
    dy = (pt.y - rgptPlan[lpplT->id].y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(l) > HIWORD(lBest)))
        goto L_6f44;
    else
        goto L_6ed3;

L_6ed3:
    if ((HIWORD(l) < HIWORD(lBest)))
        goto L_6ee0;
    else
        goto L_6ed8;

L_6ed8:
    if ((LOWORD(l) >= LOWORD(lBest)))
        goto L_6f44;
    else
        goto L_6ee0;

L_6ee0:
    if ((lpplT->iPlayer != idPlayer))
        goto L_6f44;
    else
        goto L_6eef;

L_6eef:
    if ((lpplT->fStarbase == 0x0))
        goto L_6f44;
    else
        goto L_6f06;

L_6f06:
    if ((fBigOnes == 0x0))
        goto L_6f2c;
    else
        goto L_6f0f;

L_6f0f:
    if ((HIWORD(lpplT->rgwtMin[0x3]) < 0x0))
        goto L_6f44;
    else
        goto L_6f1c;

L_6f1c:
    if ((HIWORD(lpplT->rgwtMin[0x3]) > 0x0))
        goto L_6f2c;
    else
        goto L_6f21;

L_6f21:
    if ((LOWORD(lpplT->rgwtMin[0x3]) <= 0xfa))
        goto L_6f44;
    else
        goto L_6f2c;

L_6f2c:
    lpplBest = lpplT;
    lBest = l;

L_6f44:
    lpplT = (lpplT + 0x1);

L_6f48:
    if ((LOWORD(lpplT) < LOWORD(lpplTMac)))
        goto L_6e61;
    else
        goto L_6f56;

L_6f56:
    if ((LOWORD(lpplBest) != 0x0))
        goto L_6f6e;
    else
        goto L_6f5f;

L_6f5f:
    if ((HIWORD(lpplBest) != 0x0))
        goto L_6f6e;
    else
        goto L_6f68;

L_6f68:
    return 0xffff;

L_6f6e:
    return lpplBest->id;
}

int16_t FMoveToNearestStarbase(FLEET *lpfl, int16_t fBigOnes) {
    int16_t id;
    ORDER   ord;

L_6f7e:
    id = IdplFindClosestStarbase(lpfl, fBigOnes);
    if ((id != 0xffff))
        goto L_6faa;
    else
        goto L_6fa4;

L_6fa4:
    return 0x0;

L_6faa:
    ord.id = id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[id].x;
    ord.pt.y = rgptPlan[id].y;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    return FMoveAiFleet(lpfl, &(ord), 0x0);
}

void MoveToNearestPlanetOrEnemy(FLEET *lpfl, int16_t dEnemyRange) {
    POINT   pt;
    int16_t id;
    int16_t dy;
    PLANET *lpplTMac;
    PLANET *lpplBest;
    PLANET *lpplT;
    int16_t dx;
    ORDER   ord;
    int32_t l;
    int32_t lBest;
    SCAN    scan;

L_7014:
    lBest = 0x989680;
    lpplBest = 0x0;
    pt.x = lpfl->lpplord->rgord[0x0].pt.x;
    pt.y = lpfl->lpplord->rgord[0x0].pt.y;
    lpplT = lpPlanets;
    lpplTMac = &(lpPlanets[cPlanet]);
    goto L_7128;

L_7071:
    dx = (pt.x - rgptPlan[lpplT->id].x);
    dy = (pt.y - rgptPlan[lpplT->id].y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(l) > HIWORD(lBest)))
        goto L_7124;
    else
        goto L_70e3;

L_70e3:
    if ((HIWORD(l) < HIWORD(lBest)))
        goto L_70f0;
    else
        goto L_70e8;

L_70e8:
    if ((LOWORD(l) >= LOWORD(lBest)))
        goto L_7124;
    else
        goto L_70f0;

L_70f0:
    if ((lpplT->iPlayer == idPlayer))
        goto L_7124;
    else
        goto L_70ff;

L_70ff:
    if ((lpplT->iPlayer == 0xffff))
        goto L_7124;
    else
        goto L_710c;

L_710c:
    lpplBest = lpplT;
    lBest = l;

L_7124:
    lpplT = (lpplT + 0x1);

L_7128:
    if ((LOWORD(lpplT) < LOWORD(lpplTMac)))
        goto L_7071;
    else
        goto L_7136;

L_7136:
    if ((LOWORD(lpplBest) != 0x0))
        goto L_7148;
    else
        goto L_713f;

L_713f:
    if ((HIWORD(lpplBest) == 0x0))
        goto L_717a;
    else
        goto L_7148;

L_7148:
    if ((HIWORD(lBest) > HIWORD((uint32_t)(((uint32_t)(dEnemyRange) * (uint32_t)(dEnemyRange))))))
        goto L_717a;
    else
        goto L_7161;

L_7161:
    if ((HIWORD(lBest) < HIWORD((uint32_t)(((uint32_t)(dEnemyRange) * (uint32_t)(dEnemyRange))))))
        goto L_716e;
    else
        goto L_7166;

L_7166:
    if ((LOWORD(lBest) > LOWORD((uint32_t)(((uint32_t)(dEnemyRange) * (uint32_t)(dEnemyRange))))))
        goto L_717a;
    else
        goto L_716e;

L_716e:
    id = lpplBest->id;
    goto L_71a9;

L_717a:
    /* untranslated: branch FFindNearestObject(words(lpfl->pt.y, lpfl->pt.x), 0x21, &scan) == 0x0 ? L_721c : L_719d */

L_719d:
    id = scan.idpl;
    goto L_71a9;

L_71a9:
    if ((lpfl->idPlanet == id))
        goto L_721c;
    else
        goto L_71b5;

L_71b5:

L_71bb:
    ord.id = id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[id].x;
    ord.pt.y = rgptPlan[id].y;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    FMoveAiFleet(lpfl, &(ord), 0x0);

L_721c:
    return;
}

void EnsureAiStarbaseDesigns() {
    uint16_t wTurnLast;
    int16_t  iSetNew;
    int16_t  i;
    int16_t  iSetLast;
    uint16_t t_merge_73cf_0001;
    uint16_t t_merge_7577_0001;

L_7222:
    if ((((*(rglpshdefSB[idPlayer] + 0x1a1) >> 0x9) & 0x1) == 0x0))
        goto L_727c;
    else
        goto L_724c;

L_724c:
    FCreateAiStarbase(0x2, 0x2, 0xffff, 0xffff);
    FCreateAiStarbase(0x4, 0x3, 0xffff, 0xffff);

L_727c:
    if ((((*(rglpshdefSB[idPlayer] + 0x10e) >> 0x9) & 0x1) == 0x0))
        goto L_72b5;
    else
        goto L_729d;

L_729d:
    FCreateAiStarbase(0x1, 0x1, 0xffff, 0xffff);

L_72b5:
    if ((((*(rglpshdefSB[idPlayer] + 0x234) >> 0x9) & 0x1) == 0x0))
        goto L_72ee;
    else
        goto L_72d6;

L_72d6:
    FCreateAiStarbase(0x3, 0x2, 0xffff, 0xffff);

L_72ee:
    if ((game.turn < 0x32))
        goto L_7682;
    else
        goto L_72f5;

L_72f5:

L_72fb:
    iSetLast = 0xffff;
    wTurnLast = 0x0;
    i = 0x0;
    goto L_7399;

L_730d:
    if ((i != 0x6))
        goto L_731a;
    else
        goto L_7316;

L_7316:
    i = (i - 0x1);

L_731a:
    if ((((rglpshdefSB[idPlayer][i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_7395;
    else
        goto L_7348;

L_7348:
    if ((rglpshdefSB[idPlayer][i].turn < wTurnLast))
        goto L_7395;
    else
        goto L_736e;

L_736e:
    wTurnLast = rglpshdefSB[idPlayer][i].turn;
    iSetLast = i;

L_7395:
    i = (i + 0x2);

L_7399:
    if ((i <= 0x9))
        goto L_730d;
    else
        goto L_73a2;

L_73a2:
    if ((iSetLast == 0xffff))
        goto LOrbital;
    else
        goto L_73ab;

L_73ab:
    if (((wTurnLast + 0x28) > game.turn))
        goto LOrbital;
    else
        goto L_73b7;

L_73b7:

L_73bd:
    if ((iSetLast <= 0x4))
        goto L_73cc;
    else
        goto L_73c6;

L_73c6:
    t_merge_73cf_0001 = 0x0;
    goto L_73cf;

L_73cc:
    t_merge_73cf_0001 = 0x5;

L_73cf:
    iSetNew = t_merge_73cf_0001;
    i = iSetNew;
    goto L_73df;

L_73db:
    i = (i + 0x2);

L_73df:
    if ((i >= (iSetNew + 0x5)))
        goto L_7456;
    else
        goto L_73ed;

L_73ed:
    if ((((rglpshdefSB[idPlayer][i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_73db;
    else
        goto L_741b;

L_741b:
    if ((HIWORD(rglpshdefSB[idPlayer][i].cExist) < 0x0))
        goto L_73db;
    else
        goto L_7440;

L_7440:
    if ((HIWORD(rglpshdefSB[idPlayer][i].cExist) > 0x0))
        goto LOrbital;
    else
        goto L_7445;

L_7445:
    if ((LOWORD(rglpshdefSB[idPlayer][i].cExist) > 0x0))
        goto LOrbital;
    else
        goto L_744d;

L_744d:

L_7456:
    FCreateAiStarbase(iSetNew, 0x1, 0xffff, 0xffff);
    FCreateAiStarbase((iSetNew + 0x2), 0x2, 0xffff, 0xffff);
    FCreateAiStarbase((iSetNew + 0x4), 0x3, 0xffff, 0xffff);

LOrbital:
    iSetLast = 0xffff;
    wTurnLast = 0x0;
    i = 0x1;
    goto L_7541;

L_74b5:
    if ((i != 0x5))
        goto L_74c2;
    else
        goto L_74be;

L_74be:
    i = (i + 0x1);

L_74c2:
    if ((((rglpshdefSB[idPlayer][i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_753d;
    else
        goto L_74f0;

L_74f0:
    if ((rglpshdefSB[idPlayer][i].turn < wTurnLast))
        goto L_753d;
    else
        goto L_7516;

L_7516:
    wTurnLast = rglpshdefSB[idPlayer][i].turn;
    iSetLast = i;

L_753d:
    i = (i + 0x2);

L_7541:
    if ((i < 0x9))
        goto L_74b5;
    else
        goto L_754a;

L_754a:
    if ((iSetLast == 0xffff))
        goto L_7682;
    else
        goto L_7553;

L_7553:
    if (((wTurnLast + 0x28) > game.turn))
        goto L_7682;
    else
        goto L_755f;

L_755f:

L_7565:
    if ((iSetLast <= 0x4))
        goto L_7574;
    else
        goto L_756e;

L_756e:
    t_merge_7577_0001 = 0x1;
    goto L_7577;

L_7574:
    t_merge_7577_0001 = 0x6;

L_7577:
    iSetNew = t_merge_7577_0001;
    if ((((rglpshdefSB[idPlayer][iSetNew].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_75e0;
    else
        goto L_75a8;

L_75a8:
    if ((HIWORD(rglpshdefSB[idPlayer][iSetNew].cExist) < 0x0))
        goto L_75e0;
    else
        goto L_75cd;

L_75cd:
    if ((HIWORD(rglpshdefSB[idPlayer][iSetNew].cExist) > 0x0))
        goto L_7682;
    else
        goto L_75d2;

L_75d2:
    if ((LOWORD(rglpshdefSB[idPlayer][iSetNew].cExist) > 0x0))
        goto L_7682;
    else
        goto L_75da;

L_75da:

L_75e0:
    if ((((rglpshdefSB[idPlayer][(iSetNew + 0x2)].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_7650;
    else
        goto L_7613;

L_7613:
    if ((HIWORD(rglpshdefSB[idPlayer][(iSetNew + 0x2)].cExist) < 0x0))
        goto L_7650;
    else
        goto L_763d;

L_763d:
    if ((HIWORD(rglpshdefSB[idPlayer][(iSetNew + 0x2)].cExist) > 0x0))
        goto L_7682;
    else
        goto L_7642;

L_7642:
    if ((LOWORD(rglpshdefSB[idPlayer][(iSetNew + 0x2)].cExist) > 0x0))
        goto L_7682;
    else
        goto L_764a;

L_764a:

L_7650:
    FCreateAiStarbase(iSetNew, 0x1, 0xffff, 0xffff);
    FCreateAiStarbase((iSetNew + 0x2), 0x2, 0xffff, 0xffff);

L_7682:
    return;
}

void EnsureMacintiStarbaseDesigns(uint8_t *rgSB) {
    int16_t  k;
    int16_t  iOld;
    int16_t  cAge;
    int16_t  i;
    int16_t  j;
    int16_t  iNew;
    uint16_t t_merge_777d_0001;
    uint16_t t_merge_7aca_0001;

L_76e4:
    *(rgSB) = 0x0;
    i = 0x1;
    goto L_7858;

L_76fb:
    if ((((rglpshdefSB[idPlayer][i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_7759;
    else
        goto L_7729;

L_7729:
    if ((LOWORD(rglpshdefSB[idPlayer][i].cExist) != 0x0))
        goto L_77ad;
    else
        goto L_774e;

L_774e:
    if ((HIWORD(rglpshdefSB[idPlayer][i].cExist) != 0x0))
        goto L_77ad;
    else
        goto L_7759;

L_7759:
    if ((i >= 0x3))
        goto L_777a;
    else
        goto L_7774;

L_7774:
    t_merge_777d_0001 = 0x1;
    goto L_777d;

L_777a:
    t_merge_777d_0001 = 0x2;

L_777d:
    /* untranslated: branch FCreateAiStarbase(i, t_merge_777d_0001, byte cs:[(i + 0xffff)+0x76de], i) == 0x0 ? L_779f : L_7791 */

L_7791:
    rgSB[i] = 0x0;
    goto L_7854;

L_779f:
    rgSB[i] = 0x1;

L_77ad:
    cAge = (game.turn - rglpshdefSB[idPlayer][i].turn);
    if ((cAge >= 0x23))
        goto L_7832;
    else
        goto L_77da;

L_77da:
    if ((game.turn <= 0x19))
        goto L_7824;
    else
        goto L_77e4;

L_77e4:
    if ((i != 0x1))
        goto L_7824;
    else
        goto L_77ed;

L_77ed:
    if ((rglpshdefSB[idPlayer][i].hul.chs == 0x8))
        goto L_7824;
    else
        goto L_7816;

L_7816:
    rgSB[i] = 0x3;
    goto L_7854;

L_7824:
    rgSB[i] = 0x0;

L_7832:
    if ((cAge >= 0x32))
        goto L_7849;
    else
        goto L_783b;

L_783b:
    rgSB[i] = 0x2;
    goto L_7854;

L_7849:
    rgSB[i] = 0x3;

L_7854:
    i = (i + 0x1);

L_7858:
    if ((i <= 0x3))
        goto L_76fb;
    else
        goto L_7861;

L_7861:
    iOld = 0xffff;
    i = 0x1;
    goto L_792f;

L_786e:
    if ((rgSB[i] < 0x2))
        goto L_792b;
    else
        goto L_7883;

L_7883:
    if ((iOld == 0xffff))
        goto L_7925;
    else
        goto L_788c;

L_788c:
    if ((rgSB[i] > rgSB[iOld]))
        goto L_7925;
    else
        goto L_78b5;

L_78b5:
    if ((rgSB[i] != rgSB[iOld]))
        goto L_792b;
    else
        goto L_78de;

L_78de:
    if ((rglpshdefSB[idPlayer][i].turn >= rglpshdefSB[idPlayer][iOld].turn))
        goto L_792b;
    else
        goto L_7925;

L_7925:
    iOld = i;

L_792b:
    i = (i + 0x1);

L_792f:
    if ((i <= 0x3))
        goto L_786e;
    else
        goto L_7938;

L_7938:
    i = 0x1;
    goto L_796f;

L_7940:
    if ((rgSB[i] < 0x2))
        goto L_796b;
    else
        goto L_7955;

L_7955:
    if ((i == iOld))
        goto L_796b;
    else
        goto L_7960;

L_7960:
    rgSB[i] = 0x0;

L_796b:
    i = (i + 0x1);

L_796f:
    if ((i <= 0x3))
        goto L_7940;
    else
        goto L_7978;

L_7978:
    i = 0x0;
    goto L_7a82;

L_7980:
    j = 0x0;
    goto L_7a08;

L_7988:
    if ((((rglpshdefSB[idPlayer][((LOWORD((0x3 * i)) + 0x4) + j)].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_7a04;
    else
        goto L_79c1;

L_79c1:
    if ((HIWORD(rglpshdefSB[idPlayer][((LOWORD((0x3 * i)) + 0x4) + j)].cExist) < 0x0))
        goto L_7a04;
    else
        goto L_79f1;

L_79f1:
    if ((HIWORD(rglpshdefSB[idPlayer][((LOWORD((0x3 * i)) + 0x4) + j)].cExist) > 0x0))
        goto L_7a11;
    else
        goto L_79f6;

L_79f6:
    if ((LOWORD(rglpshdefSB[idPlayer][((LOWORD((0x3 * i)) + 0x4) + j)].cExist) > 0x0))
        goto L_7a11;
    else
        goto L_79fe;

L_79fe:

L_7a04:
    j = (j + 0x1);

L_7a08:
    if ((j < 0x3))
        goto L_7988;
    else
        goto L_7a11;

L_7a11:
    if ((j != 0x3))
        goto L_7a7e;
    else
        goto L_7a1a;

L_7a1a:
    j = 0x0;
    goto L_7a75;

L_7a22:
    k = 0x5;
    goto L_7a68;

L_7a2a:
    /* untranslated: branch FCreateAiStarbase(((loword((0x3 * i)) + 0x4) + j), (j + 0x1), byte cs:[k+0x76de], (k + 0xffff)) != 0x0 ? L_7a71 : L_7a5e */

L_7a5e:

L_7a64:
    k = (k - 0x1);

L_7a68:
    if ((k >= 0x3))
        goto L_7a2a;
    else
        goto L_7a71;

L_7a71:
    j = (j + 0x1);

L_7a75:
    if ((j < 0x3))
        goto L_7a22;
    else
        goto L_7a7e;

L_7a7e:
    i = (i + 0x1);

L_7a82:
    if ((i < 0x2))
        goto L_7980;
    else
        goto L_7a8b;

L_7a8b:
    i = 0x4;
    goto L_7ad8;

L_7a93:
    if ((((rglpshdefSB[idPlayer][i].wFlags >> 0x9) & 0x1) == 0x0))
        goto L_7ac7;
    else
        goto L_7ac1;

L_7ac1:
    t_merge_7aca_0001 = 0x1;
    goto L_7aca;

L_7ac7:
    t_merge_7aca_0001 = 0x0;

L_7aca:
    rgSB[i] = LOBYTE(t_merge_7aca_0001);
    i = (i + 0x1);

L_7ad8:
    if ((i < 0xa))
        goto L_7a93;
    else
        goto L_7ae1;

L_7ae1:
    if ((*(rglpshdefSB[idPlayer] + 0x2c9) < *(rglpshdefSB[idPlayer] + 0x482)))
        goto L_7b15;
    else
        goto L_7b08;

L_7b08:
    iNew = 0x4;
    iOld = 0x7;
    goto L_7b1f;

L_7b15:
    iNew = 0x7;
    iOld = 0x4;

L_7b1f:
    if (((game.turn - rglpshdefSB[idPlayer][iOld].turn) >= 0x1e))
        goto L_7b50;
    else
        goto L_7b48;

L_7b48:
    j = 0x2;
    goto L_7b55;

L_7b50:
    j = 0x3;

L_7b55:
    i = iOld;
    goto L_7b62;

L_7b5e:
    i = (i + 0x1);

L_7b62:
    if ((i >= (iOld + 0x3)))
        goto L_7b80;
    else
        goto L_7b70;

L_7b70:
    rgSB[i] = LOBYTE(j);
    goto L_7b5e;

L_7b80:
    i = (rglpshdefSB[idPlayer][iNew].hul.ihuldef + 0xffe0);
    if ((i >= 0x4))
        goto L_7bc3;
    else
        goto L_7bac;

L_7bac:
    rgSB[0x3] = 0x2;
    if ((i >= 0x3))
        goto L_7bc3;
    else
        goto L_7bbc;

L_7bbc:
    rgSB[0x2] = 0x2;

L_7bc3:
    return;
}

int16_t FCreateAiStarbase(int16_t ishdef, int16_t iLevel, int16_t aisb, int16_t isb) {
    int16_t i;
    SHDEF   shdef;
    HS     *lphs;

L_7bca:
    if ((aisb >= 0x0))
        goto L_7c24;
    else
        goto L_7bdc;

L_7bdc:
    if ((ishdef == 0x1))
        goto L_7c00;
    else
        goto L_7be5;

L_7be5:
    if ((ishdef == 0x3))
        goto L_7c00;
    else
        goto L_7bee;

L_7bee:
    if ((ishdef == 0x6))
        goto L_7c00;
    else
        goto L_7bf7;

L_7bf7:
    if ((ishdef != 0x8))
        goto L_7c1a;
    else
        goto L_7c00;

L_7c00:
    aisb = 0xc;
    isb = 0x0;
    if ((iLevel != 0x2))
        goto L_7c24;
    else
        goto L_7c13;

L_7c13:
    iLevel = (iLevel + 0x1);

L_7c1a:
    aisb = 0x0;
    isb = 0x2;

L_7c24:
    if ((FCreateAiShdef(0xffff, isb, &(vrgSBAip[aisb])) != 0x0))
        goto L_7c4b;
    else
        goto L_7c45;

L_7c45:
    return 0x0;

L_7c4b:
    shdef = shdefBuild;
    if ((iLevel >= 0x3))
        goto L_7d75;
    else
        goto L_7c67;

L_7c67:
    i = 0x0;
    goto L_7c73;

L_7c6f:
    i = (i + 0x1);

L_7c73:
    if ((i >= shdef.hul.chs))
        goto L_7d75;
    else
        goto L_7c81;

L_7c81:
    LOWORD(lphs) = shdef.hul.rghs[i];
    /* untranslated: HIWORD(lphs) = ss */
    if ((lphs->cItem < 0x4))
        goto L_7cf4;
    else
        goto L_7caf;

L_7caf:
    /* untranslated: ss:[bp-0x9e] = (((lphs->cItem >> (0x3 - iLevel)) & 0xff) * 0x100) */
    lphs->cItem = 0x0;
    /* untranslated: *(lphs+0x2) = (*(lphs+0x2) | ss:[bp-0x9e]) */
    goto L_7c6f;

L_7cf4:
    if ((iLevel > 0x1))
        goto L_7c6f;
    else
        goto L_7cfd;

L_7cfd:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x4))
        goto L_7c6f;
    else
        goto L_7d1c;

L_7d1c:
    if ((lphs->grhst == hstSpecialSB))
        goto L_7d42;
    else
        goto L_7d2a;

L_7d2a:
    if ((lphs->cItem <= 0x1))
        goto L_7c6f;
    else
        goto L_7d42;

L_7d42:
    /* untranslated: ss:[bp-0x9e] = ((*(lphs+0x2) + 0xff00) & 0xff00) */
    lphs->cItem = 0x0;
    /* untranslated: *(lphs+0x2) = (*(lphs+0x2) | ss:[bp-0x9e]) */

L_7d75:
    shdef.wFlags = ((shdef.wFlags & 0x83ff) | (((ishdef + 0x10) & 0x1f) << 0xa));
    PickANameAndBmp(&(shdef), idsGuardianAngel, 0xd, shdef.hul.ibmp);
    return FChangeAiShdef(&(shdef), (ishdef + 0x10));
}

int16_t FAIFling(PLANET *lppl, int32_t *rgResAvail) {
    PLANET  *lpplHit;
    POINT    pt;
    int32_t  dy;
    int16_t  iT;
    int32_t  d2;
    int32_t  dBigAssPacket;
    int16_t  i;
    int16_t  fTwoMAs;
    int16_t  iLevelBest;
    PLANET  *lpplBest;
    int32_t  dx;
    int16_t  cFound;
    PLANET  *lpplHitMac;
    int32_t  l;
    PROD    *lpprod;
    int16_t  iKeep;
    uint16_t t_merge_8346_0001;
    int32_t  t_merge_83cd_0003_wide;
    uint32_t t_merge_8415_0001_wide;

L_7dd6:
    cFound = 0x0;
    iLevelBest = 0xffff;
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x4))
        goto L_7e0e;
    else
        goto L_7e08;

L_7e08:
    return 0x0;

L_7e0e:
    i = 0x0;
    lpprod = lpplProdGlob->rgprod;
    goto L_7e3e;

L_7e2b:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_7e3e:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_7edc;
    else
        goto L_7e51;

L_7e51:
    if ((lpprod->grobj != 0x1))
        goto L_7e2b;
    else
        goto L_7e71;

L_7e71:
    if ((0x0 != 0x0))
        goto L_7e2b;
    else
        goto L_7e79;

L_7e79:
    if ((0x0 < 0x0))
        goto L_7e2b;
    else
        goto L_7e99;

L_7e99:
    if ((0x0 > 0x0))
        goto L_7ea6;
    else
        goto L_7e9e;

L_7e9e:
    if ((lpprod->iItem < 0xe))
        goto L_7e2b;
    else
        goto L_7ea6;

L_7ea6:
    if ((0x0 > 0x0))
        goto L_7e2b;
    else
        goto L_7ec6;

L_7ec6:
    if ((0x0 < 0x0))
        goto L_7ed3;
    else
        goto L_7ecb;

L_7ecb:
    if ((lpprod->iItem > 0x11))
        goto L_7e2b;
    else
        goto L_7ed3;

L_7ed3:
    return 0x0;

L_7edc:
    if ((((rgplr[idPlayer].wMdPlr >> 0xa) & 0x7) <= 0x1))
        goto L_844c;
    else
        goto L_7efb;

L_7efb:
    if ((((*(rgResAvail + 0x2) + *(rgResAvail + 0x6)) + *(rgResAvail + 0xa)) < 0x0))
        goto L_844c;
    else
        goto L_7f1d;

L_7f1d:
    if ((((*(rgResAvail + 0x2) + *(rgResAvail + 0x6)) + *(rgResAvail + 0xa)) > 0x0))
        goto L_7f2a;
    else
        goto L_7f22;

L_7f22:
    if ((((rgResAvail + LOWORD(rgResAvail[0x1])) + LOWORD(rgResAvail[0x2])) <= 0xbb8))
        goto L_844c;
    else
        goto L_7f2a;

L_7f2a:
    if ((lppl->fStarbase == 0x0))
        goto L_844c;
    else
        goto L_7f41;

L_7f41:
    if ((IWarpMAFromLppl(lppl, &(fTwoMAs)) < 0xa))
        goto L_844c;
    else
        goto L_7f5b;

L_7f5b:
    if ((Random(0x4) != 0x0))
        goto L_844c;
    else
        goto L_7f6f;

L_7f6f:
    pt.x = rgptPlan[lppl->id].x;
    pt.y = rgptPlan[lppl->id].y;
    iT = 0x0;
    goto L_7fc5;

L_7f8f:
    if ((HIWORD(lppl->rgwtMin[iT]) < 0x0))
        goto L_7fc1;
    else
        goto L_7faf;

L_7faf:
    if ((HIWORD(lppl->rgwtMin[iT]) > 0x0))
        goto L_7fce;
    else
        goto L_7fb4;

L_7fb4:
    if ((LOWORD(lppl->rgwtMin[iT]) > 0x30d4))
        goto L_7fce;
    else
        goto L_7fbb;

L_7fbb:

L_7fc1:
    iT = (iT + 0x1);

L_7fc5:
    if ((iT <= 0x2))
        goto L_7f8f;
    else
        goto L_7fce;

L_7fce:
    /* untranslated: LOWORD(dBigAssPacket) = cs:[(fTwoMAs * 0x4)+0x7dce] */
    /* untranslated: HIWORD(dBigAssPacket) = cs:[(fTwoMAs * 0x4)+0x7dd0] */
    if ((iT > 0x2))
        goto L_8007;
    else
        goto L_7fee;

L_7fee:
    dBigAssPacket = (uint32_t)((dBigAssPacket * 0x3));

L_8007:
    lpplHit = lpPlanets;
    lpplHitMac = &(lpPlanets[cPlanet]);
    goto L_81a2;

L_8032:
    if ((lpplHit->iPlayer == 0xffff))
        goto L_819e;
    else
        goto L_803f;

L_803f:
    if ((lpplHit->iPlayer == idPlayer))
        goto L_819e;
    else
        goto L_804e;

L_804e:
    if (((lpplHit->turn + 0x2) < game.turn))
        goto L_819e;
    else
        goto L_8061;

L_8061:
    if ((lpplHit->uDefGuess < 0xe))
        goto L_808a;
    else
        goto L_8078;

L_8078:
    if ((lpplHit->uPopGuess >= 0x2ee))
        goto L_819e;
    else
        goto L_808a;

L_808a:
    if ((GetRaceStat(rgplr[lpplHit->iPlayer], rsMajorAdv) == raMacintosh))
        goto L_819e;
    else
        goto L_80ae;

L_80ae:
    if ((lpplHit->fStarbase == 0x0))
        goto L_80ec;
    else
        goto L_80c5;

L_80c5:
    if ((GetRaceStat(rgplr[lpplHit->iPlayer], rsMajorAdv) == raMassAccel))
        goto L_819e;
    else
        goto L_80e6;

L_80e6:

L_80ec:
    dx = (uint32_t)((pt.x - rgptPlan[lpplHit->id].x));
    dy = (uint32_t)((pt.y - rgptPlan[lpplHit->id].y));
    d2 = ((uint32_t)((dx * dx)) + (uint32_t)((dy * dy)));
    if ((HIWORD(d2) < HIWORD(dBigAssPacket)))
        goto L_8176;
    else
        goto L_8166;

L_8166:
    if ((HIWORD(d2) > HIWORD(dBigAssPacket)))
        goto L_819e;
    else
        goto L_816b;

L_816b:
    if ((LOWORD(d2) > LOWORD(dBigAssPacket)))
        goto L_819e;
    else
        goto L_8170;

L_8170:

L_8176:
    cFound = (cFound + 0x1);
    if ((Random(cFound) != 0x0))
        goto L_819e;
    else
        goto L_818d;

L_818d:
    iLevelBest = 0xd;
    lpplBest = lpplHit;

L_819e:
    lpplHit = (lpplHit + 0x1);

L_81a2:
    if ((LOWORD(lpplHit) < LOWORD(lpplHitMac)))
        goto L_8032;
    else
        goto L_81b0;

L_81b0:
    if ((iLevelBest < 0xa))
        goto L_844c;
    else
        goto L_81b9;

L_81b9:
    sel.pl.iWarpFling = (iLevelBest + 0xfffc);
    sel.pl.idFling = (lpplBest->id + 0x1);
    FLookupPlanet(0xffff, &(sel.pl));
    if ((*(rgResAvail + 0xa) < 0x0))
        goto L_8256;
    else
        goto L_821b;

L_821b:
    if ((*(rgResAvail + 0xa) > 0x0))
        goto L_822a;
    else
        goto L_8220;

L_8220:
    if ((LOWORD(rgResAvail[0x2]) <= 0x4e20))
        goto L_8256;
    else
        goto L_822a;

L_822a:
    if ((Random(0x3) == 0x0))
        goto L_8256;
    else
        goto L_823e;

L_823e:
    AddItemToQueue(0x10, 0x50, grobjPlanet, 0x1);

L_8256:
    if ((*(rgResAvail + 0x2) < 0x0))
        goto L_82c1;
    else
        goto L_8262;

L_8262:
    if ((*(rgResAvail + 0x2) > 0x0))
        goto L_8270;
    else
        goto L_8267;

L_8267:
    if ((rgResAvail <= 0xbb8))
        goto L_82c1;
    else
        goto L_8270;

L_8270:
    if ((*(rgResAvail + 0x6) < 0x0))
        goto L_82c1;
    else
        goto L_827c;

L_827c:
    if ((*(rgResAvail + 0x6) > 0x0))
        goto L_828b;
    else
        goto L_8281;

L_8281:
    if ((LOWORD(rgResAvail[0x1]) <= 0xfa0))
        goto L_82c1;
    else
        goto L_828b;

L_828b:
    if ((*(rgResAvail + 0xa) < 0x0))
        goto L_82c1;
    else
        goto L_8297;

L_8297:
    if ((*(rgResAvail + 0xa) > 0x0))
        goto L_82a6;
    else
        goto L_829c;

L_829c:
    if ((LOWORD(rgResAvail[0x2]) <= 0xbb8))
        goto L_82c1;
    else
        goto L_82a6;

L_82a6:
    AddItemToQueue(0x11, 0x1e, grobjPlanet, 0x1);
    goto L_8446;

L_82c1:
    if ((*(rgResAvail + 0x2) < 0x0))
        goto L_832c;
    else
        goto L_82cd;

L_82cd:
    if ((*(rgResAvail + 0x2) > 0x0))
        goto L_82db;
    else
        goto L_82d2;

L_82d2:
    if ((rgResAvail <= 0x5dc))
        goto L_832c;
    else
        goto L_82db;

L_82db:
    if ((*(rgResAvail + 0x6) < 0x0))
        goto L_832c;
    else
        goto L_82e7;

L_82e7:
    if ((*(rgResAvail + 0x6) > 0x0))
        goto L_82f6;
    else
        goto L_82ec;

L_82ec:
    if ((LOWORD(rgResAvail[0x1]) <= 0x8ca))
        goto L_832c;
    else
        goto L_82f6;

L_82f6:
    if ((*(rgResAvail + 0xa) < 0x0))
        goto L_832c;
    else
        goto L_8302;

L_8302:
    if ((*(rgResAvail + 0xa) > 0x0))
        goto L_8311;
    else
        goto L_8307;

L_8307:
    if ((LOWORD(rgResAvail[0x2]) <= 0x5dc))
        goto L_832c;
    else
        goto L_8311;

L_8311:
    AddItemToQueue(0x11, 0xf, grobjPlanet, 0x1);
    goto L_8446;

L_832c:
    iT = 0x0;
    goto L_843d;

L_8334:
    if ((iT != 0x1))
        goto L_8343;
    else
        goto L_833d;

L_833d:
    t_merge_8346_0001 = 0x9c4;
    goto L_8346;

L_8343:
    t_merge_8346_0001 = 0x4e2;

L_8346:
    iKeep = t_merge_8346_0001;
    if ((HIWORD(rgResAvail[iT]) < SIGNHIWORD(iKeep)))
        goto L_8439;
    else
        goto L_8361;

L_8361:
    if ((HIWORD(rgResAvail[iT]) > SIGNHIWORD(iKeep)))
        goto L_836d;
    else
        goto L_8366;

L_8366:
    if ((LOWORD(rgResAvail[iT]) <= iKeep))
        goto L_8439;
    else
        goto L_836d;

L_836d:
    /* untranslated: l = (int32_t)(words((HIWORD(rgResAvail[iT]) - signhiword(iKeep)), (LOWORD(rgResAvail[iT]) - iKeep)) / 0xc8) */
    if ((HIWORD(l) < 0x0))
        goto L_83c7;
    else
        goto L_83b0;

L_83b0:
    if ((HIWORD(l) > 0x0))
        goto L_83be;
    else
        goto L_83b5;

L_83b5:
    if ((LOWORD(l) <= 0x1))
        goto L_83c7;
    else
        goto L_83be;

L_83be:
    t_merge_83cd_0003_wide = l;
    goto L_83cd;

L_83c7:
    t_merge_83cd_0003_wide = 0x1;

L_83cd:
    /* untranslated: branch ss:[bp-0x38] > hiword(t_merge_83cd_0003_wide) ? L_83ef : L_83da */

L_83da:
    /* untranslated: branch ss:[bp-0x38] < hiword(t_merge_83cd_0003_wide) ? L_83e6 : L_83df */

L_83df:
    /* untranslated: branch ss:[bp-0x3a] >= loword(t_merge_83cd_0003_wide) ? L_83ef : L_83e6 */

L_83e6:
    t_merge_8415_0001_wide = 0x19;
    goto L_8415;

L_83ef:
    if ((HIWORD(l) < 0x0))
        goto L_840f;
    else
        goto L_83f8;

L_83f8:
    if ((HIWORD(l) > 0x0))
        goto L_8406;
    else
        goto L_83fd;

L_83fd:
    if ((LOWORD(l) <= 0x1))
        goto L_840f;
    else
        goto L_8406;

L_8406:
    t_merge_8415_0001_wide = l;
    goto L_8415;

L_840f:
    t_merge_8415_0001_wide = 0x1;

L_8415:
    l = t_merge_8415_0001_wide;
    AddItemToQueue((iT + 0xe), LOWORD(l), grobjPlanet, 0x1);

L_8439:
    iT = (iT + 0x1);

L_843d:
    if ((iT < 0x3))
        goto L_8334;
    else
        goto L_8446;

L_8446:
    return 0x1;

L_844c:
    return 0x0;
}

int16_t IshdefAiSBLatestOF() {
L_8458:
    if ((((*(rglpshdefSB[idPlayer] + 0x3ed) >> 0x9) & 0x1) != 0x0))
        goto L_84b2;
    else
        goto L_8482;

L_8482:
    if ((*(rglpshdefSB[idPlayer] + 0x3ef) <= *(rglpshdefSB[idPlayer] + 0x110)))
        goto L_84b2;
    else
        goto L_84a9;

L_84a9:
    return 0x6;

L_84b2:
    return 0x1;
}

int16_t IshdefAiSBLatest() {
L_84be:
    if ((((*(rglpshdefSB[idPlayer] + 0x35a) >> 0x9) & 0x1) != 0x0))
        goto L_8517;
    else
        goto L_84e8;

L_84e8:
    if ((*(rglpshdefSB[idPlayer] + 0x35c) <= rglpshdefSB[idPlayer]->turn))
        goto L_8517;
    else
        goto L_850e;

L_850e:
    return 0x5;

L_8517:
    return 0x0;
}

void QueueAiStarbases(PROD *rgprod, int16_t ishdefSBLatest) {
    PLANET *lpplMac;
    PLANET *lppl;
    int16_t i;
    PROD   *lpprod;

L_8524:
    if ((ishdefSBLatest == 0xffff))
        goto L_8824;
    else
        goto L_8533;

L_8533:

L_8539:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x5))
        goto L_8824;
    else
        goto L_8555;

L_8555:

L_855b:
    if ((gd.fTutorial != 0x0))
        goto L_85a3;
    else
        goto L_856e;

L_856e:
    if ((LOWORD(game.lid) != 0xef49))
        goto L_85b0;
    else
        goto L_8579;

L_8579:
    if ((HIWORD(game.lid) != 0x8c))
        goto L_85b0;
    else
        goto L_8584;

L_8584:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x1))
        goto L_85b0;
    else
        goto L_85a3;

L_85a3:
    if ((game.turn > 0x1e))
        goto L_8824;
    else
        goto L_85aa;

L_85aa:

L_85b0:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_8816;

L_85db:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x4))
        goto L_862c;
    else
        goto L_85fa;

L_85fa:
    if ((lppl->iPlayer != idPlayer))
        goto L_8812;
    else
        goto L_8606;

L_8606:

L_860c:
    ishdefSBLatest = iBuildCyberStarbase(lppl);
    if ((ishdefSBLatest == 0xffff))
        goto L_8812;
    else
        goto L_8623;

L_8623:

L_862c:
    if ((lppl->iPlayer != idPlayer))
        goto L_8812;
    else
        goto L_863b;

L_863b:
    if ((lppl->fStarbase == 0x0))
        goto L_8664;
    else
        goto L_8652;

L_8652:
    if ((lppl->isb <= 0x9))
        goto L_8812;
    else
        goto L_8664;

L_8664:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_8812;
    else
        goto L_8671;

L_8671:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_8680;
    else
        goto L_8676;

L_8676:
    if ((LOWORD(lppl->rgwtMin[0x3]) < 0x50))
        goto L_8812;
    else
        goto L_8680;

L_8680:
    /* untranslated: branch byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x2))] != 0x0 ? L_8812 : L_86a8 */

L_86a8:

L_86ae:
    i = 0x0;
    goto L_86ba;

L_86b6:
    i = (i + 0x1);

L_86ba:
    if ((i >= vlpbAiData[0x2]))
        goto L_86fb;
    else
        goto L_86ca;

L_86ca:
    if ((vlpbAiData[(0x4 + LOWORD((0x14 * i)))] == lppl->id))
        goto L_86fb;
    else
        goto L_86f2;

L_86f2:

L_86fb:
    if ((i == vlpbAiData[0x2]))
        goto L_8812;
    else
        goto L_8708;

L_8708:

L_870e:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    i = 0x0;
    lpprod = lpplProdGlob->rgprod;
    goto L_875b;

L_8748:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_875b:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_87c9;
    else
        goto L_876e;

L_876e:
    if ((lpprod->grobj != 0x2))
        goto L_8748;
    else
        goto L_878e;

L_878e:
    if ((0x0 != 0x0))
        goto L_8748;
    else
        goto L_8796;

L_8796:
    if ((0x0 < 0x0))
        goto L_8748;
    else
        goto L_87b6;

L_87b6:
    if ((0x0 > 0x0))
        goto L_87c9;
    else
        goto L_87bb;

L_87bb:
    if ((lpprod->iItem >= 0x10))
        goto L_87c9;
    else
        goto L_87c0;

L_87c0:

L_87c9:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_87eb;
    else
        goto L_87dc;

L_87dc:
    FinishProduction(0x0);
    goto L_8812;

L_87eb:
    AddItemToQueue((ishdefSBLatest + 0x10), 0x1, grobjFleet, 0x1);
    FinishProduction(0x1);

L_8812:
    lppl = (lppl + 0x1);

L_8816:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_85db;
    else
        goto L_8824;

L_8824:
    return;
}

int16_t FUpgradeAiStarbase(PLANET *lppl, int16_t ishdefSBLatest) {
    int16_t  isbCur;
    int16_t  iDesigns;
    int16_t  i;
    int16_t  pctUpg;
    int16_t  isbNew;
    PROD    *lpprod;
    int16_t  ishdef;
    uint16_t t_merge_8c15_0001;

L_882a:
    if ((ishdefSBLatest != 0xffff))
        goto L_8842;
    else
        goto L_883c;

L_883c:
    return 0x0;

L_8842:
    if ((lppl->fStarbase == 0x0))
        goto L_88fa;
    else
        goto L_8859;

L_8859:
    i = 0x0;
    lpprod = lpplProdGlob->rgprod;
    goto L_8889;

L_8876:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_8889:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_88fa;
    else
        goto L_889c;

L_889c:
    if ((lpprod->grobj != 0x2))
        goto L_8876;
    else
        goto L_88bc;

L_88bc:
    if ((0x0 != 0x0))
        goto L_8876;
    else
        goto L_88c4;

L_88c4:
    if ((0x0 < 0x0))
        goto L_8876;
    else
        goto L_88e4;

L_88e4:
    if ((0x0 > 0x0))
        goto L_88f1;
    else
        goto L_88e9;

L_88e9:
    if ((lpprod->iItem < 0x10))
        goto L_8876;
    else
        goto L_88f1;

L_88f1:
    return 0x0;

L_88fa:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x5))
        goto L_8a67;
    else
        goto L_8919;

L_8919:
    isbCur = lppl->isb;
    if ((vAiMacRecycleSB[isbCur] != 0x3))
        goto L_89b1;
    else
        goto LDoMacUpgrade;

LDoMacUpgrade:
    if ((isbCur >= 0x4))
        goto L_897a;
    else
        goto L_8945;

L_8945:
    isbNew = (isbCur + 0x1);
    goto L_896e;

L_8951:
    if ((vAiMacRecycleSB[isbNew] == 0x0))
        goto LDoMacUpgrade2;
    else
        goto L_8964;

L_8964:

L_896a:
    isbNew = (isbNew + 0x1);

L_896e:
    if ((isbNew >= 0x9))
        goto LDoMacUpgrade2;
    else
        goto L_8974;

L_8974:

L_897a:
    isbNew = (isbCur + 0x3);
    if ((isbNew < 0xa))
        goto LDoMacUpgrade2;
    else
        goto L_898c;

L_898c:
    isbNew = (isbNew - 0x6);

LDoMacUpgrade2:
    AddItemToQueue((isbNew + 0x10), 0x1, grobjFleet, 0x1);
    return 0x1;

L_89b1:
    if ((vAiMacRecycleSB[isbCur] != 0x2))
        goto L_89de;
    else
        goto L_89c7;

L_89c7:
    if ((Random(0x64) < 0xa))
        goto LDoMacUpgrade;
    else
        goto L_89d8;

L_89d8:

L_89de:
    if ((isbCur < 0x4))
        goto L_8a19;
    else
        goto L_89e7;

L_89e7:
    if ((isbCur == 0x6))
        goto L_8a19;
    else
        goto L_89f0;

L_89f0:
    if ((isbCur == 0x9))
        goto L_8a19;
    else
        goto L_89f9;

L_89f9:
    if ((Random(0x64) >= 0x8))
        goto L_8a19;
    else
        goto L_8a0d;

L_8a0d:
    isbNew = (isbCur + 0x1);
    goto LDoMacUpgrade2;

L_8a19:
    if ((isbCur >= 0x4))
        goto L_8a61;
    else
        goto L_8a22;

L_8a22:
    i = PctPlanetCapacity(lppl);
    if ((i <= 0xf))
        goto L_8a61;
    else
        goto L_8a3c;

L_8a3c:
    i = LOWORD(((i + 0xfff1) * 0x6));
    if ((Random(0x64) < i))
        goto LDoMacUpgrade;
    else
        goto L_8a5b;

L_8a5b:

L_8a61:
    return 0x0;

L_8a67:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x4))
        goto L_8a96;
    else
        goto L_8a86;

L_8a86:
    if ((game.turn >= 0x28))
        goto L_8a96;
    else
        goto L_8a90;

L_8a90:
    return 0x0;

L_8a96:
    if ((lppl->fStarbase == 0x0))
        goto L_8d1c;
    else
        goto L_8aad;

L_8aad:
    if ((lppl->isb == 0x1))
        goto L_8af5;
    else
        goto L_8abf;

L_8abf:
    if ((lppl->isb == 0x3))
        goto L_8af5;
    else
        goto L_8ad1;

L_8ad1:
    if ((lppl->isb == 0x6))
        goto L_8af5;
    else
        goto L_8ae3;

L_8ae3:
    if ((lppl->isb != 0x8))
        goto L_8b05;
    else
        goto L_8af5;

L_8af5:
    iDesigns = 0x2;
    ishdefSBLatest = IshdefAiSBLatestOF();
    goto L_8b0a;

L_8b05:
    iDesigns = 0x3;

L_8b0a:
    if ((lppl->isb < ishdefSBLatest))
        goto L_8b3c;
    else
        goto L_8b1e;

L_8b1e:
    if ((lppl->isb <= (ishdefSBLatest + ((iDesigns + 0xffff) * 0x2))))
        goto L_8c3c;
    else
        goto L_8b3c;

L_8b3c:
    pctUpg = ((game.turn - rglpshdefSB[idPlayer][ishdefSBLatest].turn) + 0xfff6);
    if ((pctUpg >= 0x0))
        goto L_8b74;
    else
        goto L_8b6c;

L_8b6c:
    pctUpg = 0x0;
    goto L_8b83;

L_8b74:
    if ((pctUpg >= 0x32))
        goto L_8b83;
    else
        goto L_8b7d;

L_8b7d:
    pctUpg = (pctUpg >> 0x1);

L_8b83:
    pctUpg = (pctUpg + 0x5);
    if ((Random(0x64) >= pctUpg))
        goto L_8d1c;
    else
        goto L_8b9b;

L_8b9b:
    if ((lppl->isb == 0x1))
        goto L_8be3;
    else
        goto L_8bad;

L_8bad:
    if ((lppl->isb == 0x3))
        goto L_8be3;
    else
        goto L_8bbf;

L_8bbf:
    if ((lppl->isb == 0x6))
        goto L_8be3;
    else
        goto L_8bd1;

L_8bd1:
    if ((lppl->isb != 0x8))
        goto L_8bff;
    else
        goto L_8be3;

L_8be3:
    /* untranslated: t_merge_8c15_0001 = ((ishdefSBLatest + (words(lppl->isb, 0x0) % 0x5)) + 0xffff) */
    goto L_8c15;

L_8bff:
    /* untranslated: t_merge_8c15_0001 = (ishdefSBLatest + (words(lppl->isb, 0x0) % 0x5)) */

L_8c15:
    ishdef = t_merge_8c15_0001;
    AddItemToQueue((ishdef + 0x10), 0x1, grobjFleet, 0x1);
    return 0x1;

L_8c3c:
    /* untranslated: branch (words(lppl->isb, 0x0) % 0x5) >= ((iDesigns + 0xffff) * 0x2) ? L_8d1c : L_8c5c */

L_8c5c:
    if ((((rglpshdefSB[idPlayer][(lppl->isb + 0x2)].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_8d1c;
    else
        goto L_8c96;

L_8c96:
    if ((Random(0x64) >= 0x6))
        goto L_8d1c;
    else
        goto L_8caa;

L_8caa:
    i = 0x0;
    goto L_8ceb;

L_8cb2:
    if ((HIWORD(lppl->rgwtMin[i]) > 0x0))
        goto L_8ce7;
    else
        goto L_8cd2;

L_8cd2:
    if ((HIWORD(lppl->rgwtMin[i]) < 0x0))
        goto L_8ce1;
    else
        goto L_8cd7;

L_8cd7:
    if ((LOWORD(lppl->rgwtMin[i]) >= 0xc8))
        goto L_8ce7;
    else
        goto L_8ce1;

L_8ce1:
    return 0x0;

L_8ce7:
    i = (i + 0x1);

L_8ceb:
    if ((i < 0x3))
        goto L_8cb2;
    else
        goto L_8cf4;

L_8cf4:
    AddItemToQueue((lppl->isb + 0x12), 0x1, grobjFleet, 0x1);
    return 0x1;

L_8d1c:
    return 0x0;
}

int16_t FQueueAiTerraforming(PLANET *lppl, int32_t *rgResAvail, int32_t *rgResCost) {
    int16_t  i;
    int16_t  j;
    int16_t  dEnv;
    int32_t  rgItemCost[4];
    PROD    *lpprod;
    uint16_t t_merge_8faf_0001;
    uint32_t t_merge_907e_0001;
    uint32_t t_merge_907e_0002;

L_8d28:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x4))
        goto L_8d56;
    else
        goto L_8d50;

L_8d50:
    return 0x0;

L_8d56:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_8d79;
    else
        goto L_8d63;

L_8d63:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_8d73;
    else
        goto L_8d68;

L_8d68:
    if ((LOWORD(lppl->rgwtMin[0x3]) >= 0xc8))
        goto L_8d79;
    else
        goto L_8d73;

L_8d73:
    return 0x0;

L_8d79:
    i = 0x0;
    lpprod = lpplProdGlob->rgprod;
    goto L_8da9;

L_8d96:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_8da9:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_8e15;
    else
        goto L_8dbc;

L_8dbc:
    if ((lpprod->grobj != 0x1))
        goto L_8d96;
    else
        goto L_8ddc;

L_8ddc:
    if ((0x0 != 0x0))
        goto L_8d96;
    else
        goto L_8de4;

L_8de4:
    if ((lpprod->iItem != 0xc))
        goto L_8d96;
    else
        goto L_8e04;

L_8e04:
    if ((0x0 != 0x0))
        goto L_8d96;
    else
        goto L_8e0c;

L_8e0c:
    return 0x0;

L_8e15:
    j = 0xffff;
    dEnv = 0x0;
    i = 0x0;
    goto L_8ec0;

L_8e27:
    if ((abs(((uint16_t)(lppl->rgEnvVar[i]) - (uint16_t)(rgplr[idPlayer].rgEnvVar[i]))) <= dEnv))
        goto L_8ebc;
    else
        goto L_8e71;

L_8e71:
    j = i;
    dEnv = abs(((uint16_t)(lppl->rgEnvVar[i]) - (uint16_t)(rgplr[idPlayer].rgEnvVar[i])));

L_8ebc:
    i = (i + 0x1);

L_8ec0:
    if ((i < 0x3))
        goto L_8e27;
    else
        goto L_8ec9;

L_8ec9:
    if ((j == 0xffff))
        goto L_90c9;
    else
        goto L_8ed2;

L_8ed2:
    i = 0x0;
    goto L_90be;

L_8eda:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0x11))) & 0x7) != 0x1))
        goto L_90ba;
    else
        goto L_8f02;

L_8f02:
    if ((0x0 != 0x0))
        goto L_90ba;
    else
        goto L_8f0a;

L_8f0a:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x7f) != 0xc))
        goto L_90ba;
    else
        goto L_8f32;

L_8f32:
    if ((0x0 != 0x0))
        goto L_90ba;
    else
        goto L_8f3a;

L_8f3a:
    if ((0x0 < 0x0))
        goto L_90ba;
    else
        goto L_8f5a;

L_8f5a:
    if ((0x0 > 0x0))
        goto L_8f67;
    else
        goto L_8f5f;

L_8f5f:
    if (((LOWORD(pProdGlob[i]) & 0x3ff) < 0x1))
        goto L_90ba;
    else
        goto L_8f67;

L_8f67:
    if ((0x4 >= (LOWORD(pProdGlob[i]) & 0x3ff)))
        goto L_8f97;
    else
        goto L_8f91;

L_8f91:
    t_merge_8faf_0001 = 0x4;
    goto L_8faf;

L_8f97:
    t_merge_8faf_0001 = (LOWORD(pProdGlob[i]) & 0x3ff);

L_8faf:
    AddItemToQueue((LOWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x7f), t_merge_8faf_0001, grobjPlanet, 0x1);
    LOWORD(pProdGlob[i]) = ((LOWORD(pProdGlob[i]) & 0xfc00) | 0x0);
    HIWORD(pProdGlob[i]) = ((HIWORD(pProdGlob[i]) & 0xffff) | 0x0);
    GetProductionCosts(lppl, ((uint8_t *)(pProdGlob) + (i * 0x4)), rgItemCost, idPlayer, 0x1);
    j = 0x0;
    goto L_90ab;

L_903c:
    if ((0x4 >= (LOWORD(pProdGlob[i]) & 0x3ff)))
        goto L_9065;
    else
        goto L_905e;

L_905e:
    t_merge_907e_0001 = 0x4;
    t_merge_907e_0002 = 0x4;
    goto L_907e;

L_9065:
    t_merge_907e_0001 = (uint32_t)((LOWORD(pProdGlob[i]) & 0x3ff));
    t_merge_907e_0002 = (uint32_t)((LOWORD(pProdGlob[i]) & 0x3ff));

L_907e:
    LOWORD(rgResCost[j]) = (LOWORD(rgResCost[j]) + LOWORD((uint32_t)((rgItemCost[j] * t_merge_907e_0001))));
    HIWORD(rgResCost[j]) = (HIWORD(rgResCost[j]) + HIWORD((uint32_t)((rgItemCost[j] * t_merge_907e_0002))));
    j = (j + 0x1);

L_90ab:
    if ((j < 0x4))
        goto L_903c;
    else
        goto L_90b4;

L_90b4:
    return 0x1;

L_90ba:
    i = (i + 0x1);

L_90be:
    if ((i < cProdGlob))
        goto L_8eda;
    else
        goto L_90c9;

L_90c9:
    return 0x0;
}

int16_t FQueueAiScanner(PLANET *lppl, int32_t *rgResAvail, int32_t *rgResCost) {
    int16_t i;
    int16_t j;
    int32_t rgItemCost[4];
    PROD   *lpprod;

L_90d6:
    i = 0x0;
    lpprod = lpplProdGlob->rgprod;
    goto L_910f;

L_90fc:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_910f:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_91ad;
    else
        goto L_9122;

L_9122:
    if ((lpprod->grobj != 0x1))
        goto L_90fc;
    else
        goto L_9142;

L_9142:
    if ((0x0 != 0x0))
        goto L_90fc;
    else
        goto L_914a;

L_914a:
    if ((0x0 < 0x0))
        goto L_90fc;
    else
        goto L_916a;

L_916a:
    if ((0x0 > 0x0))
        goto L_9177;
    else
        goto L_916f;

L_916f:
    if ((lpprod->iItem < 0x12))
        goto L_90fc;
    else
        goto L_9177;

L_9177:
    if ((0x0 > 0x0))
        goto L_90fc;
    else
        goto L_9197;

L_9197:
    if ((0x0 < 0x0))
        goto L_91a4;
    else
        goto L_919c;

L_919c:
    if ((lpprod->iItem > 0x1a))
        goto L_90fc;
    else
        goto L_91a4;

L_91a4:
    return 0x0;

L_91ad:
    i = (cProdGlob + 0xffff);
    goto L_925a;

L_91b9:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0x11))) & 0x7) != 0x1))
        goto L_9256;
    else
        goto L_91e1;

L_91e1:
    if ((0x0 != 0x0))
        goto L_9256;
    else
        goto L_91e9;

L_91e9:
    if ((0x0 < 0x0))
        goto L_9256;
    else
        goto L_9211;

L_9211:
    if ((0x0 > 0x0))
        goto L_921e;
    else
        goto L_9216;

L_9216:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x7f) < 0x12))
        goto L_9256;
    else
        goto L_921e;

L_921e:
    if ((0x0 > 0x0))
        goto L_9256;
    else
        goto L_9246;

L_9246:
    if ((0x0 < 0x0))
        goto L_9263;
    else
        goto L_924b;

L_924b:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x7f) <= 0x1a))
        goto L_9263;
    else
        goto L_9250;

L_9250:

L_9256:
    i = (i - 0x1);

L_925a:
    if ((i >= 0x0))
        goto L_91b9;
    else
        goto L_9263;

L_9263:
    if ((i < 0x0))
        goto L_938e;
    else
        goto L_926c;

L_926c:
    GetProductionCosts(lppl, ((uint8_t *)(pProdGlob) + (i * 0x4)), rgItemCost, idPlayer, 0x1);
    j = 0x0;
    goto L_930a;

L_929f:
    if ((HIWORD(rgItemCost[j]) < 0x0))
        goto L_9306;
    else
        goto L_92b4;

L_92b4:
    if ((HIWORD(rgItemCost[j]) > 0x0))
        goto L_92c1;
    else
        goto L_92b9;

L_92b9:
    if ((LOWORD(rgItemCost[j]) <= 0x0))
        goto L_9306;
    else
        goto L_92c1;

L_92c1:
    if (((HIWORD(rgItemCost[j]) + HIWORD(rgResCost[j])) < HIWORD(rgResAvail[j])))
        goto L_9306;
    else
        goto L_92f7;

L_92f7:
    if (((HIWORD(rgItemCost[j]) + HIWORD(rgResCost[j])) > HIWORD(rgResAvail[j])))
        goto L_9313;
    else
        goto L_92fc;

L_92fc:
    if (((LOWORD(rgItemCost[j]) + LOWORD(rgResCost[j])) > LOWORD(rgResAvail[j])))
        goto L_9313;
    else
        goto L_9300;

L_9300:

L_9306:
    j = (j + 0x1);

L_930a:
    if ((j < 0x4))
        goto L_929f;
    else
        goto L_9313;

L_9313:
    if ((j != 0x4))
        goto L_938e;
    else
        goto L_931c;

L_931c:
    AddItemToQueue((LOWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x7f), 0x1, grobjPlanet, 0x1);
    j = 0x0;
    goto L_937f;

L_9359:
    rgResCost[j] = (rgResCost[j] + rgItemCost[j]);
    j = (j + 0x1);

L_937f:
    if ((j < 0x4))
        goto L_9359;
    else
        goto L_9388;

L_9388:
    return 0x1;

L_938e:
    return 0x0;
}

int16_t FQueueAiDefenses(PLANET *lppl, int32_t *rgResAvail, int32_t *rgResCost) {
    int16_t i;
    int16_t j;
    PROD   *lpprod;

L_939a:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_93fd;
    else
        goto L_93b0;

L_93b0:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_93c0;
    else
        goto L_93b5;

L_93b5:
    if ((LOWORD(lppl->rgwtMin[0x3]) < 0x640))
        goto L_93fd;
    else
        goto L_93c0;

L_93c0:
    if ((HIWORD((int32_t)((lppl->rgwtMin[0x3] / 0x50))) > 0x0))
        goto L_9403;
    else
        goto L_93f1;

L_93f1:
    if ((HIWORD((int32_t)((lppl->rgwtMin[0x3] / 0x50))) < 0x0))
        goto L_93fd;
    else
        goto L_93f6;

L_93f6:
    if ((LOWORD((int32_t)((lppl->rgwtMin[0x3] / 0x50))) > lppl->cDefenses))
        goto L_9403;
    else
        goto L_93fd;

L_93fd:
    return 0x0;

L_9403:
    i = 0x0;
    lpprod = lpplProdGlob->rgprod;
    goto L_9433;

L_9420:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_9433:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_949f;
    else
        goto L_9446;

L_9446:
    if ((lpprod->grobj != 0x1))
        goto L_9420;
    else
        goto L_9466;

L_9466:
    if ((0x0 != 0x0))
        goto L_9420;
    else
        goto L_946e;

L_946e:
    if ((lpprod->iItem != 0x9))
        goto L_9420;
    else
        goto L_948e;

L_948e:
    if ((0x0 != 0x0))
        goto L_9420;
    else
        goto L_9496;

L_9496:
    return 0x0;

L_949f:
    i = 0x0;
    goto L_953b;

L_94a7:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0x11))) & 0x7) != 0x1))
        goto L_9537;
    else
        goto L_94cf;

L_94cf:
    if ((0x0 != 0x0))
        goto L_9537;
    else
        goto L_94d7;

L_94d7:
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x7f) != 0x9))
        goto L_9537;
    else
        goto L_94ff;

L_94ff:
    if ((0x0 != 0x0))
        goto L_9537;
    else
        goto L_9507;

L_9507:
    if ((0x0 < 0x0))
        goto L_9537;
    else
        goto L_9527;

L_9527:
    if ((0x0 > 0x0))
        goto L_9546;
    else
        goto L_952c;

L_952c:
    if (((LOWORD(pProdGlob[i]) & 0x3ff) >= 0x1))
        goto L_9546;
    else
        goto L_9531;

L_9531:

L_9537:
    i = (i + 0x1);

L_953b:
    if ((i < cProdGlob))
        goto L_94a7;
    else
        goto L_9546;

L_9546:
    if ((i >= cProdGlob))
        goto L_9598;
    else
        goto L_9551;

L_9551:
    j = (LOWORD(pProdGlob[i]) & 0x3ff);
    if ((j <= 0x4))
        goto L_957a;
    else
        goto L_9575;

L_9575:
    j = 0x4;

L_957a:
    AddItemToQueue(0x9, j, grobjPlanet, 0x1);
    return 0x1;

L_9598:
    return 0x0;
}

void HandleBasicAiTasks(int16_t iroCur, PROD *rgprod, int16_t ishdefSBLatest, int32_t *rgResAvail, int32_t *rgResCost) {
    PLANET *lppl;
    int16_t i;
    int16_t ipl;
    int16_t fWrite;

L_95a4:
    KeepFleetsMoving();
    QueueAiStarbases(rgprod, ishdefSBLatest);
    ipl = 0x0;
    goto L_95cc;

L_95c8:
    ipl = (ipl + 0x1);

L_95cc:
    if ((ipl >= vclpplAi))
        goto L_989a;
    else
        goto L_95d7;

L_95d7:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_9607;
    else
        goto L_95ff;

L_95ff:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_989a;
    else
        goto L_9607;

L_9607:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x0))
        goto L_9648;
    else
        goto L_9626;

L_9626:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_9692;
    else
        goto L_9633;

L_9633:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_95c8;
    else
        goto L_9638;

L_9638:
    if ((LOWORD(lppl->rgwtMin[0x3]) < 0x28))
        goto L_95c8;
    else
        goto L_963f;

L_963f:

L_9648:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_9692;
    else
        goto L_9655;

L_9655:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_9664;
    else
        goto L_965a;

L_965a:
    if ((LOWORD(lppl->rgwtMin[0x3]) >= 0x3c))
        goto L_9692;
    else
        goto L_9664;

L_9664:
    /* untranslated: branch byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x2))] == 0x0 ? L_95c8 : L_968c */

L_968c:

L_9692:
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x5))
        goto L_9728;
    else
        goto L_96d3;

L_96d3:
    i = 0x0;
    goto L_9713;

L_96db:
    if ((HIWORD(rgResAvail[i]) > HIWORD(rgResCost[i])))
        goto L_970f;
    else
        goto L_9700;

L_9700:
    if ((HIWORD(rgResAvail[i]) < HIWORD(rgResCost[i])))
        goto L_971c;
    else
        goto L_9705;

L_9705:
    if ((LOWORD(rgResAvail[i]) < LOWORD(rgResCost[i])))
        goto L_971c;
    else
        goto L_9709;

L_9709:

L_970f:
    i = (i + 0x1);

L_9713:
    if ((i < 0x4))
        goto L_96db;
    else
        goto L_971c;

L_971c:
    if ((i < 0x3))
        goto L_95c8;
    else
        goto L_9722;

L_9722:

L_9728:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    fWrite = 0x0;
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x0))
        goto L_9794;
    else
        goto L_9769;

L_9769:
    /* untranslated: branch byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x2))] != 0x0 ? L_9806 : L_9794 */

L_9794:
    if ((FUpgradeAiStarbase(lppl, ishdefSBLatest) != 0x0))
        goto L_97fe;
    else
        goto L_97ad;

L_97ad:
    if ((FAIFling(lppl, rgResAvail) != 0x0))
        goto L_97fe;
    else
        goto L_97c6;

L_97c6:
    if ((FQueueAiScanner(lppl, rgResAvail, rgResCost) != 0x0))
        goto L_97fe;
    else
        goto L_97e2;

L_97e2:
    if ((FQueueAiDefenses(lppl, rgResAvail, rgResCost) == 0x0))
        goto L_9843;
    else
        goto L_97fe;

L_97fe:
    fWrite = 0x1;

L_9806:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) != 0x4))
        goto L_9843;
    else
        goto L_9825;

L_9825:
    if ((FUpgradeAiStarbase(lppl, ishdefSBLatest) == 0x0))
        goto L_9843;
    else
        goto L_983e;

L_983e:
    fWrite = 0x1;

L_9843:
    if ((((rgplr[idPlayer].wMdPlr >> 0xd) & 0x7) == 0x0))
        goto L_988c;
    else
        goto L_9862;

L_9862:
    if ((fWrite != 0x0))
        goto L_988c;
    else
        goto L_986b;

L_986b:
    if ((FQueueAiTerraforming(lppl, rgResAvail, rgResCost) == 0x0))
        goto L_988c;
    else
        goto L_9887;

L_9887:
    fWrite = 0x1;

L_988c:
    FinishProduction(fWrite);
    goto L_95c8;

L_989a:
    if ((game.fTutorial != 0x0))
        goto L_98cc;
    else
        goto L_98ae;

L_98ae:
    if ((game.turn < (LOWORD((0xa * game.mdSize)) + 0x14)))
        goto L_98cc;
    else
        goto L_98c1;

L_98c1:
    FixPlanetsUnderAttack(rgprod);

L_98cc:
    AddMinesToBlockedQueues();
    return;
}

void SplitOutShdefs(uint8_t *rgbIsh) {
    int16_t iLast;
    int16_t iFirst;
    int16_t ifl;
    int16_t i;
    FLEET  *lpfl;
    int16_t fUnmarked;
    FLEET   flNew;
    int16_t fMarked;
    FLEET  *lpflNew;

L_98d8:
    iLast = 0xffff;
    iFirst = 0xffff;
    i = 0x0;
    goto L_9922;

L_98f4:
    if ((rgbIsh[i] <= 0x0))
        goto L_991e;
    else
        goto L_9909;

L_9909:
    iLast = i;
    if ((iFirst != 0xffff))
        goto L_991e;
    else
        goto L_9918;

L_9918:
    iFirst = i;

L_991e:
    i = (i + 0x1);

L_9922:
    if ((i < 0x10))
        goto L_98f4;
    else
        goto L_992b;

L_992b:
    if ((iFirst == 0xffff))
        goto L_9b0a;
    else
        goto L_9931;

L_9931:

LTopOfLoop:
    /* untranslated: branch (part[4:2](rgplr[idPlayer]) & 0xfff) > 0x1f4 ? L_9b0a : L_994e */

L_994e:

L_9954:
    ifl = 0x0;
    goto L_9960;

L_995c:
    ifl = (ifl + 0x1);

L_9960:
    if ((ifl >= cFleet))
        goto L_9b0a;
    else
        goto L_996b;

L_996b:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_999b;
    else
        goto L_9993;

L_9993:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_9b0a;
    else
        goto L_999b;

L_999b:
    if ((lpfl->iPlayer != idPlayer))
        goto L_995c;
    else
        goto L_99aa;

L_99aa:
    if ((lpfl->fDead != 0x0))
        goto L_995c;
    else
        goto L_99be;

L_99be:

L_99c4:
    fUnmarked = 0x0;
    fMarked = 0x0;
    i = 0x0;
    goto L_9afe;

L_99d8:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_9afa;
    else
        goto L_99f5;

L_99f5:
    if ((rgbIsh[i] == 0x0))
        goto L_9ae5;
    else
        goto L_9a0a;

L_9a0a:
    if ((fUnmarked == 0x0))
        goto L_9adc;
    else
        goto LDoTheSplit;

LDoTheSplit:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    lpflNew = LpflNewSplit(&(sel.fl));
    flNew = *(lpflNew);
    i = 0x0;
    goto L_9a9b;

L_9a5c:
    if ((rgbIsh[i] <= 0x0))
        goto L_9a97;
    else
        goto L_9a71;

L_9a71:
    flNew.rgcsh[i] = sel.fl.rgcsh[i];
    sel.fl.rgcsh[i] = 0x0;

L_9a97:
    i = (i + 0x1);

L_9a9b:
    if ((i < 0x10))
        goto L_9a5c;
    else
        goto L_9aa4;

L_9aa4:
    FleetTransferCargoBalance(&(sel.fl), &(flNew));
    FLookupFleet(0xffff, &(sel.fl));
    FLookupFleet(0xffff, &(flNew));
    goto LTopOfLoop;

L_9adc:
    fMarked = 0x1;
    goto L_9afa;

L_9ae5:
    if ((fMarked != 0x0))
        goto LDoTheSplit;
    else
        goto L_9aec;

L_9aec:

L_9af5:
    fUnmarked = 0x1;

L_9afa:
    i = (i + 0x1);

L_9afe:
    if ((i >= 0x10))
        goto L_995c;
    else
        goto L_9b04;

L_9b04:

L_9b0a:
    return;
}

int16_t CheckAiShdefStatus(int16_t ishBeg, int16_t ishEnd, uint16_t cRecyclePeriod, int16_t *piLatest, uint8_t *rgbOld) {
    uint32_t cExist;
    int16_t  i;
    SHDEF    shdef;

L_9b10:
    *(piLatest) = 0xffff;
    cExist = 0x0;
    i = ishBeg;
    goto L_9c29;

L_9b33:
    if ((((rgshdef[i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_9c25;
    else
        goto L_9b51;

L_9b51:
    cExist = (cExist + rgshdef[i].cExist);
    if ((*(piLatest) == 0xffff))
        goto L_9b9e;
    else
        goto L_9b75;

L_9b75:
    if ((rgshdef[*(piLatest)].turn >= rgshdef[i].turn))
        goto L_9ba6;
    else
        goto L_9b9e;

L_9b9e:
    *(piLatest) = i;

L_9ba6:
    if (((game.turn - rgshdef[i].turn) <= cRecyclePeriod))
        goto L_9c25;
    else
        goto L_9bbf;

L_9bbf:
    if ((LOWORD(rgshdef[i].cExist) != 0x0))
        goto L_9c1a;
    else
        goto L_9bd4;

L_9bd4:
    if ((HIWORD(rgshdef[i].cExist) != 0x0))
        goto L_9c1a;
    else
        goto L_9bde;

L_9bde:
    shdef = rgshdef[i];
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), i);
    goto L_9c25;

L_9c1a:
    rgbOld[i] = 0x1;

L_9c25:
    i = (i + 0x1);

L_9c29:
    if ((i <= ishEnd))
        goto L_9b33;
    else
        goto L_9c34;

L_9c34:
    if ((HIWORD(cExist) < 0x0))
        goto L_9c56;
    else
        goto L_9c3d;

L_9c3d:
    if ((HIWORD(cExist) > 0x0))
        goto L_9c4c;
    else
        goto L_9c42;

L_9c42:
    if ((LOWORD(cExist) <= 0x7d00))
        goto L_9c56;
    else
        goto L_9c4c;

L_9c4c:
    cExist = 0x7d00;

L_9c56:

L_9c5f:
    return LOWORD(cExist);
}

void IncreaseAIMinefieldSizes() {
    THING  *lpth;
    int32_t cMines;
    THING  *lpthMac;

L_9c66:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_9d04;

L_9c9a:
    if ((lpth->ith != ithMinefield))
        goto L_9d00;
    else
        goto L_9cb0;

L_9cb0:
    sqrt((double)(lpth->thm.cMines));
    cMines = __ftol();
    LOWORD(lpth->thm) = LOWORD((uint32_t)((cMines * cMines)));
    HIWORD(lpth->thm.cMines) = HIWORD((uint32_t)((cMines * cMines)));

L_9d00:
    lpth = (lpth + 0x1);

L_9d04:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_9c9a;
    else
        goto L_9d12;

L_9d12:
    return;
}

int16_t FFindBuddyAndJoinUp(FLEET *lpfl, int16_t ishLo, int16_t ishHi, int32_t lMaxDist1, int32_t lMaxDist2) {
    int32_t lDistBest;
    int32_t lDist;
    int16_t i;
    int16_t ifl;
    FLEET  *lpflOther;
    FLEET  *lpflBest;
    ORDER   ord;

L_9d18:
    lpflBest = 0x0;
    lDistBest = 0x98967f;
    ifl = 0x0;
    goto L_9d41;

L_9d3d:
    ifl = (ifl + 0x1);

L_9d41:
    if ((ifl >= cFleet))
        goto L_9e39;
    else
        goto L_9d4c;

L_9d4c:
    lpflOther = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_9d7c;
    else
        goto L_9d74;

L_9d74:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_9e39;
    else
        goto L_9d7c;

L_9d7c:
    if ((lpflOther->iPlayer != idPlayer))
        goto L_9d3d;
    else
        goto L_9d88;

L_9d88:

L_9d8e:
    if ((LOWORD(lpflOther) != LOWORD(lpfl)))
        goto L_9da7;
    else
        goto L_9d9c;

L_9d9c:
    if ((HIWORD(lpflOther) == HIWORD(lpfl)))
        goto L_9e39;
    else
        goto L_9da1;

L_9da1:

L_9da7:
    i = ishLo;
    goto L_9e2b;

L_9db0:
    if ((lpflOther->rgcsh[i] == 0x0))
        goto L_9e27;
    else
        goto L_9dcd;

L_9dcd:
    /* untranslated: lDist = LDistance2(words(lpfl->pt.y, lpfl->pt.x), words(lpflOther->pt.y, lpflOther->pt.x)) */
    if ((HIWORD(lDist) > HIWORD(lDistBest)))
        goto L_9d3d;
    else
        goto L_9dff;

L_9dff:
    if ((HIWORD(lDist) < HIWORD(lDistBest)))
        goto L_9e0c;
    else
        goto L_9e04;

L_9e04:
    if ((LOWORD(lDist) >= LOWORD(lDistBest)))
        goto L_9d3d;
    else
        goto L_9e0c;

L_9e0c:
    lpflBest = lpflOther;
    lDistBest = lDist;

L_9e27:
    i = (i + 0x1);

L_9e2b:
    if ((i > ishHi))
        goto L_9d3d;
    else
        goto L_9e33;

L_9e33:

L_9e39:
    if ((LOWORD(lpflBest) != 0x0))
        goto L_9e4b;
    else
        goto L_9e42;

L_9e42:
    if ((HIWORD(lpflBest) == 0x0))
        goto L_9f23;
    else
        goto L_9e4b;

L_9e4b:
    if ((HIWORD(lDistBest) < HIWORD((uint32_t)((lMaxDist1 * lMaxDist1)))))
        goto L_9eab;
    else
        goto L_9e64;

L_9e64:
    if ((HIWORD(lDistBest) > HIWORD((uint32_t)((lMaxDist1 * lMaxDist1)))))
        goto L_9e71;
    else
        goto L_9e69;

L_9e69:
    if ((LOWORD(lDistBest) <= LOWORD((uint32_t)((lMaxDist1 * lMaxDist1)))))
        goto L_9eab;
    else
        goto L_9e71;

L_9e71:
    if ((HIWORD(lDistBest) > HIWORD((uint32_t)((lMaxDist2 * lMaxDist2)))))
        goto L_9f23;
    else
        goto L_9e8a;

L_9e8a:
    if ((HIWORD(lDistBest) < HIWORD((uint32_t)((lMaxDist2 * lMaxDist2)))))
        goto L_9e97;
    else
        goto L_9e8f;

L_9e8f:
    if ((LOWORD(lDistBest) > LOWORD((uint32_t)((lMaxDist2 * lMaxDist2)))))
        goto L_9f23;
    else
        goto L_9e97;

L_9e97:
    if ((Random(0x2) == 0x0))
        goto L_9f23;
    else
        goto L_9eab;

L_9eab:
    ClearAiCurrentTask(lpfl, 0x1);
    ord.id = lpflBest->id;
    ord.grobj = grobjFleet;
    ord.pt.x = lpflBest->pt.x;
    ord.pt.y = lpflBest->pt.y;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x6;
    FMoveAiFleet(lpfl, &(ord), 0x0);
    return 0x1;

L_9f23:
    return 0x0;
}

int16_t FShouldPlanetBuildColonizer(PLANET *lpplSrc) {
    POINT    pt;
    int16_t  i;
    int32_t  lCur;
    uint8_t *lpb;
    int32_t  lBest;

L_9f30:
    lBest = 0x989680;
    pt.x = rgptPlan[lpplSrc->id].x;
    pt.y = rgptPlan[lpplSrc->id].y;
    if ((game.turn >= 0x3c))
        goto L_9f6b;
    else
        goto L_9f65;

L_9f65:
    return 0x1;

L_9f6b:
    lpb = &(vlpbAiPlanet[0xd]);
    i = 0x0;
    goto L_9f97;

L_9f86:
    i = (i + 0x1);
    lpb = (lpb + 0x10);

L_9f97:
    if ((i >= game.cPlanMax))
        goto L_a000;
    else
        goto L_9fa2;

L_9fa2:
    if ((*(lpb) != 0x0))
        goto L_9f86;
    else
        goto L_9fb3;

L_9fb3:
    /* untranslated: lCur = LDistance2(words(pt.y, pt.x), words(rgptPlan[i].y, rgptPlan[i].x)) */
    if ((HIWORD(lCur) > HIWORD(lBest)))
        goto L_9f86;
    else
        goto L_9fe4;

L_9fe4:
    if ((HIWORD(lCur) < HIWORD(lBest)))
        goto L_9ff1;
    else
        goto L_9fe9;

L_9fe9:
    if ((LOWORD(lCur) >= LOWORD(lBest)))
        goto L_9f86;
    else
        goto L_9ff1;

L_9ff1:
    lBest = lCur;

L_a000:
    if ((HIWORD(lBest) > 0x1))
        goto L_a070;
    else
        goto L_a009;

L_a009:
    if ((HIWORD(lBest) < 0x1))
        goto L_a018;
    else
        goto L_a00e;

L_a00e:
    if ((LOWORD(lBest) > 0xde84))
        goto L_a070;
    else
        goto L_a018;

L_a018:
    if ((HIWORD(lBest) < 0x1))
        goto L_a044;
    else
        goto L_a021;

L_a021:
    if ((HIWORD(lBest) > 0x1))
        goto L_a030;
    else
        goto L_a026;

L_a026:
    if ((LOWORD(lBest) <= 0x5f90))
        goto L_a044;
    else
        goto L_a030;

L_a030:
    if ((Random(0x2) != 0x0))
        goto L_a070;
    else
        goto L_a044;

L_a044:
    if ((HIWORD(lBest) < 0x0))
        goto L_a076;
    else
        goto L_a04d;

L_a04d:
    if ((HIWORD(lBest) > 0x0))
        goto L_a05c;
    else
        goto L_a052;

L_a052:
    if ((LOWORD(lBest) <= 0xf424))
        goto L_a076;
    else
        goto L_a05c;

L_a05c:
    if ((Random(0x2) == 0x0))
        goto L_a076;
    else
        goto L_a070;

L_a070:
    return 0x0;

L_a076:
    return 0x1;
}

void InitRandomPlanetList() {
    PLANET *lpplMac;
    int16_t iT;
    PLANET *lppl;
    int16_t i;

L_a082:
    vclpplAi = 0x0;
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_a0f4;

L_a0bc:
    if ((lppl->iPlayer != idPlayer))
        goto L_a0f0;
    else
        goto L_a0cb;

L_a0cb:
    vclpplAi = (vclpplAi + 0x1);
    LOWORD(vrglpplAi[vclpplAi]) = LOWORD(lppl);
    *(vrglpplAi[vclpplAi] + 0x2) = HIWORD(lppl);

L_a0f0:
    lppl = (lppl + 0x1);

L_a0f4:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_a0bc;
    else
        goto L_a102;

L_a102:
    if ((gd.fTutorial != 0x0))
        goto L_a1bd;
    else
        goto L_a115;

L_a115:
    i = 0x0;
    goto L_a121;

L_a11d:
    i = (i + 0x1);

L_a121:
    if ((i >= (vclpplAi + 0xffff)))
        goto L_a1bd;
    else
        goto L_a12f;

L_a12f:
    iT = (Random((vclpplAi - i)) + i);
    lppl = vrglpplAi[i];
    LOWORD(vrglpplAi[i]) = LOWORD(vrglpplAi[iT]);
    *(vrglpplAi[i] + 0x2) = *(vrglpplAi[iT] + 0x2);
    LOWORD(vrglpplAi[iT]) = LOWORD(lppl);
    *(vrglpplAi[iT] + 0x2) = HIWORD(lppl);
    goto L_a11d;

L_a1bd:
    return;
}
