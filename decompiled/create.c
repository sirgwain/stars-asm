#include "common.h"

int16_t vrgvcMax[10] = {16, 18, 4, 19, 28, 49, 29, 87, 7, 47};
char    rgNG3Width[9][2] = {{-3}, {2, 1}, {5}, {-3}, {3}, {3}, {3}, {1}, {3}};
uint8_t vrgWormholeMin[5] = {0, 1, 1, 3, 4};
BTLPLAN rgbtlplanT[5] = {{
                             .mdTactic = mdTacticMaxDamageRatio,
                             .mdTarget1 = mdTargetArmedShips,
                             .mdTarget2 = mdTargetAny,
                             .iplrAttack = 2,
                             .szName = "Default",
                         },
                         {
                             .iplan = 1,
                             .mdTactic = mdTacticMaxDamageRatio,
                             .mdTarget1 = mdTargetStarbase,
                             .mdTarget2 = mdTargetArmedShips,
                             .iplrAttack = 2,
                             .szName = "Kill Starbase",
                         },
                         {
                             .iplan = 2,
                             .mdTactic = mdTacticMaxNetDamage,
                             .mdTarget1 = mdTargetArmedShips,
                             .mdTarget2 = mdTargetBombersFreighters,
                             .iplrAttack = 2,
                             .szName = "Max-Defense",
                         },
                         {
                             .iplan = 3,
                             .mdTactic = mdTacticDisengageIfChallenged,
                             .mdTarget1 = mdTargetUnarmedShips,
                             .iplrAttack = 2,
                             .szName = "Sniper",
                         },
                         {
                             .iplan = 4,
                             .mdTarget1 = mdTargetAny,
                             .iplrAttack = 2,
                             .szName = "Chicken",
                         }};
PLAYER  vrgplrComp[6][4] = {{{
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {-1, -1, -1},
                                 .rgEnvVarMin = {-1, -1, -1},
                                 .rgEnvVarMax = {-1, -1, -1},
                                 .pctIdealGrowth = 5,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 12, 10, 16, 10, 5, 10, 0, 1, 1, 2, 1, 1},
                                 .grbitAttr = 4929,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {-1, -1, -1},
                                 .rgEnvVarMin = {-1, -1, -1},
                                 .rgEnvVarMax = {-1, -1, -1},
                                 .pctIdealGrowth = 6,
                                 .pctResearch = 15,
                                 .rgAttr = {9, 13, 9, 16, 10, 4, 11, 0, 1, 1, 2, 1, 1},
                                 .grbitAttr = 833,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {-1, -1, -1},
                                 .rgEnvVarMin = {-1, -1, -1},
                                 .rgEnvVarMax = {-1, -1, -1},
                                 .pctIdealGrowth = 6,
                                 .pctResearch = 15,
                                 .rgAttr = {8, 13, 9, 18, 10, 4, 12, 0, 1, 2, 2, 2, 1},
                                 .grbitAttr = 2147484257,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {-1, -1, -1},
                                 .rgEnvVarMin = {-1, -1, -1},
                                 .rgEnvVarMax = {-1, -1, -1},
                                 .pctIdealGrowth = 7,
                                 .pctResearch = 15,
                                 .rgAttr = {8, 13, 9, 16, 10, 4, 8, 0, 1, 2, 1, 2, 1},
                                 .grbitAttr = 2147484257,
                            }},
                            {{
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {58, 35, 65},
                                 .rgEnvVarMin = {27, 7, 35},
                                 .rgEnvVarMax = {89, 63, 95},
                                 .pctIdealGrowth = 14,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 9, 10, 9, 9, 5, 8, 0, 1, 0, 1, 1, 1, 0, 1},
                                 .grbitAttr = 8261,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {62, 33, 61},
                                 .rgEnvVarMin = {32, 6, 26},
                                 .rgEnvVarMax = {92, 60, 96},
                                 .pctIdealGrowth = 14,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 10, 10, 10, 10, 5, 9, 0, 1, 1, 1, 1, 1, 1, 1},
                                 .grbitAttr = 2147491909,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {63, 28, 62},
                                 .rgEnvVarMin = {31, 4, 30},
                                 .rgEnvVarMax = {95, 52, 94},
                                 .pctIdealGrowth = 14,
                                 .pctResearch = 15,
                                 .rgAttr = {9, 11, 10, 10, 10, 5, 9, 0, 0, 1, 0, 1, 1, 1, 1},
                                 .grbitAttr = 2147491909,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {62, 29, -1},
                                 .rgEnvVarMin = {31, 5, -1},
                                 .rgEnvVarMax = {93, 53, -1},
                                 .pctIdealGrowth = 15,
                                 .pctResearch = 15,
                                 .rgAttr = {8, 15, 10, 25, 10, 5, 9, 0, 0, 0, 0, 0, 0, 0, 1},
                                 .grbitAttr = 2684362821,
                            }},
                            {{
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {35, 60, 38},
                                 .rgEnvVarMin = {7, 26, 5},
                                 .rgEnvVarMax = {63, 94, 71},
                                 .pctIdealGrowth = 15,
                                 .pctResearch = 15,
                                 .rgAttr = {9, 11, 10, 14, 11, 6, 14, 1, 0, 0, 0, 0, 0, 0, 4},
                                 .grbitAttr = 536874768,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {35, 60, 38},
                                 .rgEnvVarMin = {7, 26, 5},
                                 .rgEnvVarMax = {63, 94, 71},
                                 .pctIdealGrowth = 15,
                                 .pctResearch = 15,
                                 .rgAttr = {8, 13, 9, 14, 10, 6, 14, 1, 0, 0, 0, 0, 0, 0, 4},
                                 .grbitAttr = 2684358416,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {35, 60, 38},
                                 .rgEnvVarMin = {7, 26, 5},
                                 .rgEnvVarMax = {63, 94, 71},
                                 .pctIdealGrowth = 15,
                                 .pctResearch = 15,
                                 .rgAttr = {8, 14, 9, 15, 14, 5, 15, 1, 0, 0, 0, 0, 0, 0, 4},
                                 .grbitAttr = 2684358160,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {35, -1, 50},
                                 .rgEnvVarMin = {7, -1},
                                 .rgEnvVarMax = {63, -1, 100},
                                 .pctIdealGrowth = 16,
                                 .pctResearch = 15,
                                 .rgAttr = {8, 14, 9, 14, 14, 5, 14, 1, 0, 0, 0, 0, 0, 0, 4},
                                 .grbitAttr = 2684358160,
                            }},
                            {{
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {32, 31, 31},
                                 .rgEnvVarMax = {68, 69, 69},
                                 .pctIdealGrowth = 15,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 10, 10, 10, 10, 5, 10, 1, 0, 0, 0, 0, 0, 2, 3},
                                 .grbitAttr = 536878850,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {32, 31, 31},
                                 .rgEnvVarMax = {68, 69, 69},
                                 .pctIdealGrowth = 15,
                                 .pctResearch = 15,
                                 .rgAttr = {8, 12, 10, 12, 14, 5, 12, 1, 0, 0, 0, 0, 0, 2, 3},
                                 .grbitAttr = 536878594,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {23, 24, 25},
                                 .rgEnvVarMax = {77, 76, 75},
                                 .pctIdealGrowth = 15,
                                 .pctResearch = 15,
                                 .rgAttr = {8, 12, 10, 12, 14, 5, 12, 1, 0, 0, 0, 0, 0, 2, 3},
                                 .grbitAttr = 536878594,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {-1, 50, 50},
                                 .rgEnvVarMin = {-1, 24, 25},
                                 .rgEnvVarMax = {-1, 76, 75},
                                 .pctIdealGrowth = 15,
                                 .pctResearch = 15,
                                 .rgAttr = {8, 15, 10, 15, 15, 5, 15, 1, 0, 0, 0, 0, 0, 2, 3},
                                 .grbitAttr = 536878594,
                            }},
                            {{
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {22, 22, 22},
                                 .rgEnvVarMax = {78, 78, 78},
                                 .pctIdealGrowth = 12,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 9, 18, 9, 9, 10, 8, 1, 1, 0, 0, 1, 0, 0, 6},
                                 .grbitAttr = 536873475,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {19, 19, 19},
                                 .rgEnvVarMax = {81, 81, 81},
                                 .pctIdealGrowth = 17,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 10, 13, 19, 10, 10, 7, 1, 1, 0, 0, 1, 1, 1, 6},
                                 .grbitAttr = 536874499,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {18, 18, 18},
                                 .rgEnvVarMax = {82, 82, 82},
                                 .pctIdealGrowth = 17,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 14, 10, 20, 10, 10, 6, 1, 1, 1, 0, 1, 1, 2, 6},
                                 .grbitAttr = 2684358211,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {17, 17, 17},
                                 .rgEnvVarMax = {83, 83, 83},
                                 .pctIdealGrowth = 19,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 15, 9, 25, 10, 10, 5, 1, 2, 2, 0, 2, 1, 1, 6},
                                 .grbitAttr = 2684358211,
                            }},
                            {{
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {20, 20, 20},
                                 .rgEnvVarMax = {80, 80, 80},
                                 .pctIdealGrowth = 10,
                                 .pctResearch = 15,
                                 .rgAttr = {16, 10, 10, 10, 10, 5, 10, 0, 1, 1, 1, 1, 0, 1, 8},
                                 .grbitAttr = 283,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {15, 15, 15},
                                 .rgEnvVarMax = {85, 85, 85},
                                 .pctIdealGrowth = 14,
                                 .pctResearch = 15,
                                 .rgAttr = {12, 10, 10, 10, 10, 5, 10, 0, 2, 1, 1, 1, 0, 1, 8},
                                 .grbitAttr = 27,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {15, 15, 15},
                                 .rgEnvVarMax = {85, 85, 85},
                                 .pctIdealGrowth = 17,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 10, 10, 10, 10, 5, 10, 0, 2, 1, 1, 1, 1, 1, 8},
                                 .grbitAttr = 127,
                            },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 7,
                                 .wMdPlr = 7,
                                 .lSalt = -1,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {15, 15, 15},
                                 .rgEnvVarMax = {85, 85, 85},
                                 .pctIdealGrowth = 20,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 10, 10, 10, 10, 5, 10, 0, 2, 1, 1, 2, 1, 1, 8},
                                 .grbitAttr = 127,
                            }}};
uint8_t vrgWormholeVar[5] = {3, 3, 5, 4, 5};

void InitBattlePlan(BTLPLAN *lpbtlplan, int16_t iplan, int16_t iplr) {
L_00c0:
    /* untranslated: *lpbtlplan = 36-byte cs:[(0xc + loword((0x24 * iplan)))] */
    lpbtlplan->iplr = iplr;
    if ((game.fSinglePlr == 0x0))
        goto L_0130;
    else
        goto L_0113;

L_0113:
    if ((iplan != 0x0))
        goto L_0130;
    else
        goto L_011c;

L_011c:
    lpbtlplan->iplrAttack = 0x3;

L_0130:
    return;
}

int16_t GenerateWorld(int16_t fBatchMode) {
    int32_t *pl;
    int16_t  iBest;
    int16_t  cKill;
    char     grUsed[128];
    jmp_buf *penvMemSav[9];
    POINT   *ppt;
    int16_t  raMajor;
    int16_t  k;
    POINT    pt;
    int16_t  fFound;
    int16_t  iMax;
    STARPACK starpack;
    int16_t  dy;
    int16_t  dGalMinSq;
    int16_t  iLow;
    PLANET  *lppl;
    int16_t  iMin;
    int16_t  i;
    jmp_buf  env[9];
    int16_t  xOld;
    int16_t  iplrSingle;
    POINT   *pptMax;
    int16_t  dMin;
    int16_t  ktLeft;
    SHDEF   *lpshdef;
    int32_t  lDistMax2;
    int32_t  lDistIdeal2;
    int16_t  rgi[16];
    int16_t  iNewLine;
    uint8_t *pb;
    int16_t  dMax;
    int32_t  lDistMin2;
    int16_t  j;
    int16_t  cPlanMax;
    int16_t  dx;
    int16_t  cKillMax;
    POINT   *pptT;
    int32_t  lBest;
    int32_t  l;
    int16_t  iT;
    int16_t  jj;
    int16_t  iTechMin;
    int16_t  pct10;
    int16_t  idHome;
    int16_t  ishRet;
    PART     part;
    int16_t  cFit;
    PLANET  *lpplClosest;
    POINT    ptHome;
    PLANET  *lpplPicked;
    int32_t  lDistCur2;
    HS      *lphs;
    int16_t  chs;
    int16_t  cTry;
    int16_t  rgTry[5];
    THING   *lpth;
    uint16_t idLast;
    THING   *lpthLast;
    char     szExt[4];
    int16_t  t_merge_01c8_0001;
    uint16_t t_merge_0201_0001;
    uint32_t t_merge_0951_0001_wide;
    uint16_t t_merge_18ed_0001;
    uint16_t t_merge_1e0f_0001;
    uint16_t t_merge_1e81_0001;
    uint16_t t_merge_1f75_0001;
    uint16_t t_merge_312c_0001;
    int16_t  t_35ff;
    uint16_t t_merge_399c_0001;
    int16_t  t_40ef;
    uint16_t t_merge_4327_0001;

L_0136:
    iMin = 0x0;
    cKill = 0x0;
    dGal = (LOWORD((0x190 * game.mdSize)) + 0x190);
    dGalInv = (dGal + 0x7d0);
    cPlanMax = LOWORD((int32_t)(((uint32_t)(((uint32_t)(dGal) * (uint32_t)(dGal))) / 0x1388)));
    cPlanMax = (cPlanMax + LOWORD((((uint32_t)(cPlanMax) / 0x4) * (game.mdDensity + 0xffff))));
    if ((game.mdDensity < 0x3))
        goto L_01b3;
    else
        goto L_01a5;

L_01a5:
    cPlanMax = (cPlanMax + ((uint32_t)(cPlanMax) / 0x4));

L_01b3:
    if ((cPlanMax >= 0x3e7))
        goto L_01c5;
    else
        goto L_01be;

L_01be:
    t_merge_01c8_0001 = cPlanMax;
    goto L_01c8;

L_01c5:
    t_merge_01c8_0001 = 0x3e7;

L_01c8:
    cPlanMax = t_merge_01c8_0001;
    dGalMinSq = LOWORD((dGalMinDist * dGalMinDist));
    if (((((uint32_t)(cPlanMax) / 0x7) + cPlanMax) >= 0x3e7))
        goto L_01fe;
    else
        goto L_01ed;

L_01ed:
    t_merge_0201_0001 = (((uint32_t)(cPlanMax) / 0x7) + cPlanMax);
    goto L_0201;

L_01fe:
    t_merge_0201_0001 = 0x3e7;

L_0201:
    iMax = t_merge_0201_0001;
    dx = 0x3f2;
    dy = (dGal + 0xffed);
    i = 0x0;
    goto L_025f;

L_021e:
    rgptPlan[i].x = (Random(dy) + dx);
    rgptPlan[i].y = (Random(dy) + dx);
    i = (i + 0x1);

L_025f:
    if ((i < iMax))
        goto L_021e;
    else
        goto L_026c;

L_026c:
    qsort(&(rgptPlan), iMax, 0x4, ICompLong);
    pptMax = rgptPlan[iMax];
    ppt = 0x2f40;
    goto L_0364;

L_02a3:
    if ((ppt->y < 0x0))
        goto L_035f;
    else
        goto L_02ad;

L_02ad:

L_02b3:
    pptT = (ppt + 0x4);
    iNewLine = (ppt->x + dGalMinDist);
    goto L_02d6;

L_02d1:
    pptT = (pptT + 0x4);

L_02d6:
    if ((pptT >= pptMax))
        goto L_035f;
    else
        goto L_02e3;

L_02e3:
    if ((pptT->x > iNewLine))
        goto L_035f;
    else
        goto L_02f2;

L_02f2:
    dy = abs((ppt->y - pptT->y));
    if ((dy > dGalMinDist))
        goto L_02d1;
    else
        goto L_0316;

L_0316:

L_031c:
    dx = (ppt->x - pptT->x);
    if (((LOWORD((dx * dx)) + LOWORD((dy * dy))) > dGalMinSq))
        goto L_02d1;
    else
        goto L_034f;

L_034f:
    pptT->y = 0xff9c;
    cKill = (cKill + 0x1);

L_035f:
    ppt = (ppt + 0x4);

L_0364:
    if ((ppt < pptMax))
        goto L_02a3;
    else
        goto L_0371;

L_0371:
    cKillMax = (iMax - cPlanMax);
    goto L_03bf;

L_0380:
    i = Random(iMax);
    if ((rgptPlan[i].y < 0x0))
        goto L_03bf;
    else
        goto L_03a3;

L_03a3:

L_03a9:
    rgptPlan[i].y = 0xff9c;
    cKill = (cKill + 0x1);

L_03bf:
    if ((cKill < cKillMax))
        goto L_0380;
    else
        goto L_03cb;

L_03cb:
    pptT = 0x2f40;
    ppt = 0x2f40;
    goto L_0404;

L_03db:
    if ((ppt->y < 0x0))
        goto L_03ff;
    else
        goto L_03e8;

L_03e8:
    pptT = (pptT + 0x4);
    pptT->x = ppt->x;
    pptT->y = ppt->y;

L_03ff:
    ppt = (ppt + 0x4);

L_0404:
    if ((ppt < pptMax))
        goto L_03db;
    else
        goto L_0411;

L_0411:
    cPlanMax = (iMax - cKill);
    if ((game.fClumping == 0x0))
        goto L_0768;
    else
        goto L_042f;

L_042f:
    i = 0x0;
    goto L_073e;

L_0438:
    lBest = 0x989680;
    iBest = 0x0;
    j = Random(cPlanMax);
    pt.x = rgptPlan[j].x;
    pt.y = rgptPlan[j].y;
    k = 0x0;
    goto L_0533;

L_047a:
    if ((k == j))
        goto L_052e;
    else
        goto L_0484;

L_0484:

L_048a:
    dx = (pt.x - rgptPlan[k].x);
    dy = (pt.y - rgptPlan[k].y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(l) > HIWORD(lBest)))
        goto L_052e;
    else
        goto L_0509;

L_0509:
    if ((HIWORD(l) < HIWORD(lBest)))
        goto L_0517;
    else
        goto L_050e;

L_050e:
    if ((LOWORD(l) >= LOWORD(lBest)))
        goto L_052e;
    else
        goto L_0517;

L_0517:
    lBest = l;
    iBest = k;

L_052e:
    k = (k + 0x1);

L_0533:
    if ((k < cPlanMax))
        goto L_047a;
    else
        goto L_0540;

L_0540:
    if ((HIWORD(lBest) < 0x0))
        goto L_0739;
    else
        goto L_054a;

L_054a:
    if ((HIWORD(lBest) > 0x0))
        goto L_055a;
    else
        goto L_054f;

L_054f:
    if ((LOWORD(lBest) <= 0x90))
        goto L_0739;
    else
        goto L_055a;

L_055a:
    if ((HIWORD(lBest) < 0x0))
        goto L_05d9;
    else
        goto L_0564;

L_0564:
    if ((HIWORD(lBest) > 0x0))
        goto L_0574;
    else
        goto L_0569;

L_0569:
    if ((LOWORD(lBest) <= 0x640))
        goto L_05d9;
    else
        goto L_0574;

L_0574:
    rgptPlan[j].x = ((uint32_t)(((rgptPlan[iBest].x * 0x2) + rgptPlan[j].x)) / 0x3);
    rgptPlan[j].y = ((uint32_t)(((rgptPlan[iBest].y * 0x2) + rgptPlan[j].y)) / 0x3);
    goto L_0739;

L_05d9:
    if ((HIWORD(lBest) < 0x0))
        goto L_0654;
    else
        goto L_05e3;

L_05e3:
    if ((HIWORD(lBest) > 0x0))
        goto L_05f3;
    else
        goto L_05e8;

L_05e8:
    if ((LOWORD(lBest) <= 0x271))
        goto L_0654;
    else
        goto L_05f3;

L_05f3:
    rgptPlan[j].x = ((uint32_t)((rgptPlan[j].x + rgptPlan[iBest].x)) / 0x2);
    rgptPlan[j].y = ((uint32_t)((rgptPlan[j].y + rgptPlan[iBest].y)) / 0x2);
    goto L_0739;

L_0654:
    if ((HIWORD(lBest) < 0x0))
        goto L_06d3;
    else
        goto L_065e;

L_065e:
    if ((HIWORD(lBest) > 0x0))
        goto L_066e;
    else
        goto L_0663;

L_0663:
    if ((LOWORD(lBest) <= 0x144))
        goto L_06d3;
    else
        goto L_066e;

L_066e:
    rgptPlan[j].x = ((uint32_t)(((rgptPlan[j].x * 0x2) + rgptPlan[iBest].x)) / 0x3);
    rgptPlan[j].y = ((uint32_t)(((rgptPlan[j].y * 0x2) + rgptPlan[iBest].y)) / 0x3);
    goto L_0739;

L_06d3:
    rgptPlan[j].x = ((uint32_t)(((rgptPlan[j].x * 0x4) + rgptPlan[iBest].x)) / 0x5);
    rgptPlan[j].y = ((uint32_t)(((rgptPlan[j].y * 0x4) + rgptPlan[iBest].y)) / 0x5);

L_0739:
    i = (i + 0x1);

L_073e:
    if ((i < cPlanMax))
        goto L_0438;
    else
        goto L_074b;

L_074b:
    qsort(&(rgptPlan), cPlanMax, 0x4, ICompLong);

L_0768:
    memset(grUsed, 0x0, 0x80);
    i = 0x0;
    goto L_0816;

L_0786:
    dx = Random(0x3e7);

L_0796:
    if ((((uint16_t)(grUsed[(dx >> 0x3)]) & bitTbl[(dx & 0x7)]) == 0x0))
        goto L_07e4;
    else
        goto L_07be;

L_07be:
    dx = (dx + 0x1);
    if ((dx < (game.fTutorial + 0x3e7)))
        goto L_0796;
    else
        goto L_07db;

L_07db:
    dx = 0x0;

L_07e4:
    grUsed[(dx >> 0x3)] = (grUsed[(dx >> 0x3)] | LOBYTE(bitTbl[(dx & 0x7)]));
    rgidPlan[i] = dx;
    i = (i + 0x1);

L_0816:
    if ((i < cPlanMax))
        goto L_0786;
    else
        goto L_0823;

L_0823:
    cPlanet = cPlanMax;
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_0858;
    else
        goto L_084d;

L_084d:
    DestroyCurGame();
    return 0x0;

L_0858:
    lpPlanets = LpAlloc(LOWORD((cPlanMax * 0x38)), htPlanets);
    fmemset(&(lpPlanets), 0x0, LOWORD((cPlanMax * 0x38)));
    i = 0x0;
    lppl = lpPlanets;
    goto L_08c3;

L_08ab:
    i = (i + 0x1);
    lppl = (lppl + 0x1);

L_08c3:
    if ((i >= cPlanMax))
        goto L_0d4b;
    else
        goto L_08d0;

L_08d0:
    lppl->id = i;
    lppl->iPlayer = 0xffff;
    lppl->det = 0x7;
    lppl->iScanner = 0x1f;
    if ((game.fNoRandom != 0x0))
        goto L_09a1;
    else
        goto L_0932;

L_0932:
    if ((Random(0x3) != 0x0))
        goto L_094d;
    else
        goto L_0946;

L_0946:
    t_merge_0951_0001_wide = 0x10001;
    goto L_0951;

L_094d:
    t_merge_0951_0001_wide = 0x0;

L_0951:
    /* untranslated: ss:[bp-0x116] = 0x0 */
    /* untranslated: ss:[bp-0x11a] = ((*(lppl+0x1a) & 0xffbf) | hiword((int32_t)(words((loword(t_merge_0951_0001_wide) & 0x1), 0x0) << 0x16))) */
    /* untranslated: *(lppl+0x18) = ((*(lppl+0x18) & 0xffff) | loword((int32_t)(words((hiword(t_merge_0951_0001_wide) & 0x1), 0x0) << 0x16))) */
    /* untranslated: *(lppl+0x1a) = ss:[bp-0x11a] */

L_09a1:
    lppl->rgEnvVar[0x0] = LOBYTE((Random(0x5a) + 0x1));
    lppl->rgEnvVar[0x0] = (lppl->rgEnvVar[0x0] + LOBYTE(Random(0xa)));
    lppl->rgEnvVarOrig[0x0] = lppl->rgEnvVar[0x0];
    lppl->rgEnvVar[0x1] = LOBYTE((Random(0x5a) + 0x1));
    lppl->rgEnvVar[0x1] = (lppl->rgEnvVar[0x1] + LOBYTE(Random(0xa)));
    lppl->rgEnvVarOrig[0x1] = lppl->rgEnvVar[0x1];
    Random(0x63);
    /* untranslated: lppl->rgEnvVarOrig[0x2] = lobyte((callresult(int16_t) + 0x1)) */
    /* untranslated: lppl->rgEnvVar[0x2] = lobyte((callresult(int16_t) + 0x1)) */
    if ((game.fTutorial == 0x0))
        goto L_0ae7;
    else
        goto L_0a4a;

L_0a4a:
    goto L_0ad4;

L_0a51:
    lppl->rgEnvVar[0x0] = (lppl->rgEnvVar[0x0] + 0x14);
    lppl->rgEnvVarOrig[0x0] = lppl->rgEnvVar[0x0];
    goto L_0ae7;

L_0a6d:
    j = 0x0;
    goto L_0ac4;

L_0a76:
    lppl->rgEnvVar[j] = (lppl->rgEnvVar[j] - 0x5);
    lppl->rgEnvVarOrig[j] = lppl->rgEnvVar[j];
    j = (j + 0x1);

L_0ac4:
    if ((j >= 0x3))
        goto L_0ae7;
    else
        goto L_0acb;

L_0acb:

L_0ad4:
    if ((i == 0x5))
        goto L_0a6d;
    else
        goto L_0adc;

L_0adc:
    if ((i == 0xb))
        goto L_0a51;
    else
        goto L_0ae7;

L_0ae7:
    j = 0x0;
    goto L_0c69;

L_0af0:
    if ((game.fExtraFuel == 0x0))
        goto L_0b1a;
    else
        goto L_0afe;

L_0afe:
    lppl->rgMinConc[j] = 0x64;
    goto L_0bd3;

L_0b1a:
    lppl->rgwtMin[j] = 0x0;
    /* untranslated: ss:[bp-0x116] = Random(0x2d) */
    /* untranslated: lppl->rgMinConc[j] = lobyte(((Random(0x2d) + ss:[bp-0x116]) + 0x1f)) */
    if (((uint16_t)(lppl->rgEnvVar[0x2]) < 0x5a))
        goto L_0bd3;
    else
        goto L_0b8c;

L_0b8c:
    lppl->rgMinConc[j] = (lppl->rgMinConc[j] + LOBYTE(((uint32_t)(Random((0x63 - lppl->rgMinConc[j]))) / 0x2)));

L_0bd3:
    lppl->rgpctMinLevel[j] = 0x0;
    lppl->rgwtMin[j] = 0x0;
    if ((game.fBBSPlay == 0x0))
        goto L_0c64;
    else
        goto L_0c28;

L_0c28:
    if ((lppl->rgMinConc[j] >= 0x28))
        goto L_0c64;
    else
        goto L_0c4b;

L_0c4b:
    lppl->rgMinConc[j] = (lppl->rgMinConc[j] + 0x5);

L_0c64:
    j = (j + 0x1);

L_0c69:
    if ((j < 0x3))
        goto L_0af0;
    else
        goto L_0c73;

L_0c73:
    if ((game.fExtraFuel == 0x0))
        goto L_0c8a;
    else
        goto L_0c81;

L_0c81:
    iT = 0x64;
    goto L_0c9a;

L_0c8a:
    iT = Random(0x1b);

L_0c9a:
    if ((iT >= 0x12))
        goto L_08ab;
    else
        goto L_0ca4;

L_0ca4:
    if ((iT < 0x9))
        goto L_0cf0;
    else
        goto L_0cae;

L_0cae:
    jj = Random(0x1e);
    j = Random(0x3);
    lppl->rgMinConc[j] = LOBYTE((jj + 0x1));
    goto L_08ab;

L_0cf0:
    iT = (iT + 0x1);
    goto L_0d3e;

L_0cf8:
    jj = Random(0x1e);
    j = Random(0x3);
    lppl->rgMinConc[j] = LOBYTE((jj + 0x1));
    iT = (iT * 0x2);

L_0d3e:
    if ((iT >= 0x10))
        goto L_08ab;
    else
        goto L_0d45;

L_0d45:

L_0d4b:
    j = 0x0;
    goto L_0e6d;

L_0d54:
    lpPlanets->rgwtMin[j] = (uint32_t)((Random(LOWORD((lpPlanets->rgMinConc[j] * 0xa))) + 0xa));
    if ((HIWORD(lpPlanets->rgwtMin[j]) > 0x0))
        goto L_0e03;
    else
        goto L_0dc4;

L_0dc4:
    if ((HIWORD(lpPlanets->rgwtMin[j]) < 0x0))
        goto L_0dd3;
    else
        goto L_0dc9;

L_0dc9:
    if ((LOWORD(lpPlanets->rgwtMin[j]) >= 0xc8))
        goto L_0e03;
    else
        goto L_0dd3;

L_0dd3:
    lpPlanets->rgwtMin[j] = (lpPlanets->rgwtMin[j] + (uint32_t)((Random(0x96) + 0x9b)));

L_0e03:
    if ((game.fBBSPlay == 0x0))
        goto L_0e68;
    else
        goto L_0e1b;

L_0e1b:
    lpPlanets->rgwtMin[j] = (lpPlanets->rgwtMin[j] + (int32_t)((lpPlanets->rgwtMin[j] / 0x4)));

L_0e68:
    j = (j + 0x1);

L_0e6d:
    if ((j < 0x3))
        goto L_0d54;
    else
        goto L_0e77;

L_0e77:
    l = (uint32_t)(((uint32_t)(dGal) * 0x6));
    lDistIdeal2 = ((int32_t)(((uint32_t)(((uint32_t)(dGal) * (uint32_t)(dGal))) / (uint32_t)(game.cPlayer))) - l);
    if ((HIWORD(lDistIdeal2) > 0x0))
        goto L_0ee8;
    else
        goto L_0eca;

L_0eca:
    if ((HIWORD(lDistIdeal2) < 0x0))
        goto L_0ed9;
    else
        goto L_0ecf;

L_0ecf:
    if ((LOWORD(lDistIdeal2) >= 0x0))
        goto L_0ee8;
    else
        goto L_0ed9;

L_0ed9:
    lDistIdeal2 = 0x0;
    goto L_0f14;

L_0ee8:
    lDistIdeal2 = (int32_t)(((uint32_t)((lDistIdeal2 * 0x9)) / 0xa));

L_0f14:
    lDistIdeal2 = ((int32_t)(((uint32_t)((lDistIdeal2 * (uint32_t)(game.mdStartDist))) / 0x3)) + l);
    lDistMin2 = (int32_t)(((uint32_t)((lDistIdeal2 * 0x9)) / 0xa));
    lDistMax2 = (int32_t)(((uint32_t)((lDistIdeal2 * 0x7)) / 0x6));

RetryAll:
    lBest = 0x5f5e100;
    dMin = (((uint32_t)(dGal) / 0x4) + 0x3e8);
    /* untranslated: dMax = ((words(loword((0x3 * dGal)), signhiword(loword((0x3 * dGal)))) / 0x4) + 0x3e8) */
    i = 0x0;
    goto L_110f;

L_0fd7:
    rgi[0x0] = Random(cPlanMax);
    pt.x = rgptPlan[rgi[0x0]].x;
    pt.y = rgptPlan[rgi[0x0]].y;
    if ((pt.x >= dMin))
        goto L_101b;
    else
        goto L_100c;

L_100c:
    dx = (dMin - pt.x);
    goto L_103d;

L_101b:
    if ((pt.x <= dMax))
        goto L_1037;
    else
        goto L_1028;

L_1028:
    dx = (pt.x - dMax);
    goto L_103d;

L_1037:
    dx = 0x0;

L_103d:
    if ((pt.y >= dMin))
        goto L_1059;
    else
        goto L_104a;

L_104a:
    dy = (dMin - pt.y);
    goto L_107b;

L_1059:
    if ((pt.y <= dMax))
        goto L_1075;
    else
        goto L_1066;

L_1066:
    dy = (pt.y - dMax);
    goto L_107b;

L_1075:
    dy = 0x0;

L_107b:
    if ((dx != 0x0))
        goto L_1092;
    else
        goto L_1085;

L_1085:
    if ((dy == 0x0))
        goto L_1119;
    else
        goto L_108c;

L_108c:

L_1092:
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(l) > HIWORD(lBest)))
        goto L_110a;
    else
        goto L_10e5;

L_10e5:
    if ((HIWORD(l) < HIWORD(lBest)))
        goto L_10f3;
    else
        goto L_10ea;

L_10ea:
    if ((LOWORD(l) >= LOWORD(lBest)))
        goto L_110a;
    else
        goto L_10f3;

L_10f3:
    lBest = l;
    iBest = rgi[0x0];

L_110a:
    i = (i + 0x1);

L_110f:
    if ((i < 0x32))
        goto L_0fd7;
    else
        goto L_1119;

L_1119:
    if ((i != 0x32))
        goto L_112a;
    else
        goto L_1123;

L_1123:
    rgi[0x0] = iBest;

L_112a:
    if ((game.cPlayer <= 0x4))
        goto L_115f;
    else
        goto L_1134;

L_1134:
    dMin = (((uint32_t)(dGal) / 0x14) + 0x3e8);
    dMax = (MulDiv(dGal, 0x13, 0x14) + 0x3e8);
    goto L_11c4;

L_115f:
    if ((game.cPlayer <= 0x2))
        goto L_1194;
    else
        goto L_1169;

L_1169:
    dMin = (((uint32_t)(dGal) / 0xa) + 0x3e8);
    dMax = (MulDiv(dGal, 0x9, 0xa) + 0x3e8);
    goto L_11c4;

L_1194:
    dMin = (MulDiv(dGal, 0x3, 0x14) + 0x3e8);
    dMax = (MulDiv(dGal, 0x11, 0x14) + 0x3e8);

L_11c4:
    i = 0x1;
    goto L_11d2;

L_11cd:
    i = (i + 0x1);

L_11d2:
    if ((i >= game.cPlayer))
        goto L_15bc;
    else
        goto L_11de;

L_11de:
    j = 0x0;
    goto L_137c;

L_11e7:
    rgi[i] = Random(cPlanMax);
    pt.x = rgptPlan[rgi[i]].x;
    pt.y = rgptPlan[rgi[i]].y;
    if ((pt.x < dMin))
        goto L_1377;
    else
        goto L_1230;

L_1230:
    if ((pt.y < dMin))
        goto L_1377;
    else
        goto L_123d;

L_123d:
    if ((pt.x > dMax))
        goto L_1377;
    else
        goto L_124a;

L_124a:
    if ((pt.y > dMax))
        goto L_1377;
    else
        goto L_1254;

L_1254:

L_125a:
    fFound = 0x0;
    k = 0x0;
    goto L_1350;

L_1269:
    dx = (pt.x - rgptPlan[rgi[k]].x);
    dy = (pt.y - rgptPlan[rgi[k]].y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(l) < 0x0))
        goto L_135d;
    else
        goto L_12f5;

L_12f5:
    if ((HIWORD(l) > 0x0))
        goto L_1304;
    else
        goto L_12fa;

L_12fa:
    if ((LOWORD(l) <= 0x0))
        goto L_135d;
    else
        goto L_1304;

L_1304:
    if ((HIWORD(l) > HIWORD(lDistMin2)))
        goto L_1326;
    else
        goto L_1315;

L_1315:
    if ((HIWORD(l) < HIWORD(lDistMin2)))
        goto L_135d;
    else
        goto L_131a;

L_131a:
    if ((LOWORD(l) < LOWORD(lDistMin2)))
        goto L_135d;
    else
        goto L_1320;

L_1320:

L_1326:
    if ((HIWORD(l) > HIWORD(lDistMax2)))
        goto L_134b;
    else
        goto L_1337;

L_1337:
    if ((HIWORD(l) < HIWORD(lDistMax2)))
        goto L_1345;
    else
        goto L_133c;

L_133c:
    if ((LOWORD(l) > LOWORD(lDistMax2)))
        goto L_134b;
    else
        goto L_1345;

L_1345:
    fFound = 0x1;

L_134b:
    k = (k + 0x1);

L_1350:
    if ((k < i))
        goto L_1269;
    else
        goto L_135d;

L_135d:
    if ((k != i))
        goto L_1377;
    else
        goto L_136a;

L_136a:
    if ((fFound != 0x0))
        goto L_1386;
    else
        goto L_1371;

L_1371:

L_1377:
    j = (j + 0x1);

L_137c:
    if ((j < 0x32))
        goto L_11e7;
    else
        goto L_1386;

L_1386:
    if ((j != 0x32))
        goto L_11cd;
    else
        goto L_1390;

L_1390:
    iBest = rgi[i];

L_13a1:
    rgi[i] = (rgi[i] + 0x1);
    if ((rgi[i] == iBest))
        goto L_1566;
    else
        goto L_13ba;

L_13ba:
    if ((rgi[i] < cPlanMax))
        goto L_13ed;
    else
        goto L_13d1;

L_13d1:
    rgi[i] = 0x0;
    if ((iBest == 0x0))
        goto L_1566;
    else
        goto L_13e7;

L_13e7:

L_13ed:
    pt.x = rgptPlan[rgi[i]].x;
    pt.y = rgptPlan[rgi[i]].y;
    if ((pt.x < dMin))
        goto L_13a1;
    else
        goto L_141c;

L_141c:
    if ((pt.y < dMin))
        goto L_13a1;
    else
        goto L_1429;

L_1429:
    if ((pt.x > dMax))
        goto L_13a1;
    else
        goto L_1436;

L_1436:
    if ((pt.y > dMax))
        goto L_13a1;
    else
        goto L_1440;

L_1440:

L_1446:
    fFound = 0x0;
    k = 0x0;
    goto L_153c;

L_1455:
    dx = (pt.x - rgptPlan[rgi[k]].x);
    dy = (pt.y - rgptPlan[rgi[k]].y);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(l) < 0x0))
        goto L_1549;
    else
        goto L_14e1;

L_14e1:
    if ((HIWORD(l) > 0x0))
        goto L_14f0;
    else
        goto L_14e6;

L_14e6:
    if ((LOWORD(l) <= 0x0))
        goto L_1549;
    else
        goto L_14f0;

L_14f0:
    if ((HIWORD(l) > HIWORD(lDistMin2)))
        goto L_1512;
    else
        goto L_1501;

L_1501:
    if ((HIWORD(l) < HIWORD(lDistMin2)))
        goto L_1549;
    else
        goto L_1506;

L_1506:
    if ((LOWORD(l) < LOWORD(lDistMin2)))
        goto L_1549;
    else
        goto L_150c;

L_150c:

L_1512:
    if ((HIWORD(l) > HIWORD(lDistMax2)))
        goto L_1537;
    else
        goto L_1523;

L_1523:
    if ((HIWORD(l) < HIWORD(lDistMax2)))
        goto L_1531;
    else
        goto L_1528;

L_1528:
    if ((LOWORD(l) > LOWORD(lDistMax2)))
        goto L_1537;
    else
        goto L_1531;

L_1531:
    fFound = 0x1;

L_1537:
    k = (k + 0x1);

L_153c:
    if ((k < i))
        goto L_1455;
    else
        goto L_1549;

L_1549:
    if ((k != i))
        goto L_13a1;
    else
        goto L_1556;

L_1556:
    if ((fFound != 0x0))
        goto L_1566;
    else
        goto L_155d;

L_155d:

L_1566:
    if ((iBest != rgi[i]))
        goto L_11cd;
    else
        goto L_157c;

L_157c:
    lDistMin2 = (lDistMin2 - (int32_t)((lDistIdeal2 / 0x23)));
    lDistMax2 = (lDistMax2 + (int32_t)((lDistIdeal2 / 0x23)));
    goto RetryAll;

L_15bc:
    i = 0x0;
    goto L_15ca;

L_15c5:
    i = (i + 0x1);

L_15ca:
    if ((i >= game.cPlayer))
        goto L_1a3e;
    else
        goto L_15d6;

L_15d6:
    j = (Random((game.cPlayer - i)) + i);
    k = rgi[j];
    rgi[j] = rgi[i];
    rgi[i] = k;
    if ((GetRaceGrbit(rgplr[i], ibitRaceAIPlayer) == 0x0))
        goto L_1664;
    else
        goto L_164f;

L_164f:
    CreateRandomRace(rgplr[i]);

L_1664:
    rgplr[i].wFlags = ((rgplr[i].wFlags & 0xfffe) | 0x0);
    rgplr[i].wFlags = ((rgplr[i].wFlags & 0xfff7) | 0x0);
    rgplr[i].grbitTrader = 0x0;
    j = 0x0;
    goto L_1719;

L_16d6:
    rgplr[i].rgTech[j] = 0x0;
    rgplr[i].rgResSpent[j] = 0x0;
    j = (j + 0x1);

L_1719:
    if ((j < 0x6))
        goto L_16d6;
    else
        goto L_1723;

L_1723:
    GetRaceStat(rgplr[i], rsMajorAdv);
    goto L_187c;
    rgplr[i].rgTech[0x1] = 0x6;
    rgplr[i].rgTech[0x2] = 0x1;
    rgplr[i].rgTech[0x0] = 0x1;
    goto L_18a2;
    rgplr[i].rgTech[0x2] = 0x2;
    rgplr[i].rgTech[0x5] = 0x2;
    goto L_18a2;
    rgplr[i].rgTech[0x4] = 0x5;
    goto L_18a2;
    rgplr[i].rgTech[0x0] = 0x4;
    goto L_18a2;
    rgplr[i].rgTech[0x2] = 0x5;
    rgplr[i].rgTech[0x3] = 0x5;
    goto L_18a2;
    rgplr[i].rgTech[0x5] = 0x6;
    rgplr[i].rgTech[0x3] = 0x2;
    rgplr[i].rgTech[0x0] = 0x1;
    rgplr[i].rgTech[0x1] = 0x1;
    rgplr[i].rgTech[0x2] = 0x1;
    goto L_18a2;
    rgplr[i].rgTech[0x0] = 0x1;
    goto L_18a2;
    j = 0x0;
    goto L_186c;

L_184d:
    rgplr[i].rgTech[j] = 0x3;
    j = (j + 0x1);

L_186c:
    if ((j >= 0x6))
        goto L_18a2;
    else
        goto L_1873;

L_1873:

L_187c:

L_18a2:
    if ((GetRaceGrbit(rgplr[i], ibitRaceTech3) == 0x0))
        goto L_1979;
    else
        goto L_18c3;

L_18c3:
    if ((GetRaceStat(rgplr[i], rsMajorAdv) != raNone))
        goto L_18ea;
    else
        goto L_18e4;

L_18e4:
    t_merge_18ed_0001 = 0x1;
    goto L_18ed;

L_18ea:
    t_merge_18ed_0001 = 0x0;

L_18ed:
    iTechMin = (t_merge_18ed_0001 + 0x3);
    j = 0x0;
    goto L_196f;

L_18fd:
    if (((uint16_t)(rgplr[i].rgTech[j]) >= iTechMin))
        goto L_196a;
    else
        goto L_1920;

L_1920:
    if ((GetRaceStat(rgplr[i], (j + 0x8)) != 0x0))
        goto L_196a;
    else
        goto L_1945;

L_1945:
    rgplr[i].rgTech[j] = LOBYTE(iTechMin);

L_196a:
    j = (j + 0x1);

L_196f:
    if ((j < 0x6))
        goto L_18fd;
    else
        goto L_1979;

L_1979:
    if ((GetRaceGrbit(rgplr[i], ibitRaceCheapEngines) == 0x0))
        goto L_19aa;
    else
        goto L_199a;

L_199a:
    rgplr[i].rgTech[0x2] = (rgplr[i].rgTech[0x2] + 0x1);

L_19aa:
    if ((GetRaceGrbit(rgplr[i], ibitRaceIFE) == 0x0))
        goto L_19ef;
    else
        goto L_19cb;

L_19cb:
    if ((game.fTutorial != 0x0))
        goto L_19ef;
    else
        goto L_19df;

L_19df:
    rgplr[i].rgTech[0x2] = (rgplr[i].rgTech[0x2] + 0x1);

L_19ef:
    j = 0x0;
    goto L_1a31;

L_19f8:
    FSendPlrMsg(i, (j + 0x7f), 0xffff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
    j = (j + 0x1);

L_1a31:
    if ((j >= 0x4))
        goto L_15c5;
    else
        goto L_1a38;

L_1a38:

L_1a3e:
    i = 0x0;
    goto L_1a4c;

L_1a47:
    i = (i + 0x1);

L_1a4c:
    if ((i >= game.cPlayer))
        goto L_2ee8;
    else
        goto L_1a58;

L_1a58:
    iMin = rgi[i];
    lpPlanets[iMin].iPlayer = i;
    /* untranslated: part[4:2](lpPlanets[iMin]) = ((part[4:2](lpPlanets[iMin]) & 0xfdff) | 0x200) */
    /* untranslated: part[44:2](lpPlanets[iMin]) = ((part[44:2](lpPlanets[iMin]) & 0xfff0) | 0x0) */
    /* untranslated: ss:[bp-0x116] = ((part[26:2](lpPlanets[iMin]) & 0xffbf) | 0x0) */
    /* untranslated: part[24:2](lpPlanets[iMin]) = ((part[24:2](lpPlanets[iMin]) & 0xffff) | 0x0) */
    /* untranslated: part[26:2](lpPlanets[iMin]) = ss:[bp-0x116] */
    /* untranslated: ss:[bp-0x116] = ((part[22:2](lpPlanets[iMin]) & 0xf) | 0xa0) */
    /* untranslated: part[20:2](lpPlanets[iMin]) = ((part[20:2](lpPlanets[iMin]) & 0xffff) | 0x0) */
    /* untranslated: part[22:2](lpPlanets[iMin]) = ss:[bp-0x116] */
    /* untranslated: ss:[bp-0x116] = ((part[22:2](lpPlanets[iMin]) & 0xfff0) | 0x0) */
    /* untranslated: part[20:2](lpPlanets[iMin]) = ((part[20:2](lpPlanets[iMin]) & 0xff) | 0xa00) */
    /* untranslated: part[22:2](lpPlanets[iMin]) = ss:[bp-0x116] */
    /* untranslated: ss:[bp-0x116] = ((part[26:2](lpPlanets[iMin]) & 0xffff) | 0x0) */
    /* untranslated: part[24:2](lpPlanets[iMin]) = ((part[24:2](lpPlanets[iMin]) & 0xf000) | 0xa) */
    /* untranslated: part[26:2](lpPlanets[iMin]) = ss:[bp-0x116] */
    /* untranslated: part[4:2](lpPlanets[iMin]) = ((part[4:2](lpPlanets[iMin]) & 0xfbff) | 0x400) */
    if ((GetRaceGrbit(rgplr[i], ibitRaceLowStartingPop) == 0x0))
        goto L_1cc5;
    else
        goto L_1ca3;

L_1ca3:
    lpPlanets[iMin].rgwtMin[0x3] = 0xaf;
    goto L_1ce4;

L_1cc5:
    lpPlanets[iMin].rgwtMin[0x3] = 0xfa;

L_1ce4:
    /* untranslated: lpPlanets[iMin].uGuesses = ((lpPlanets[iMin].uGuesses & 0xf000) | ((words(LOWORD(lpPlanets[iMin].rgwtMin[0x3]), 0x0) / 0x4) & 0xfff)) */
    j = 0x0;
    goto L_1eaf;

L_1d5d:
    lpPlanets[iMin].rgwtMin[j] = lpPlanets->rgwtMin[j];
    if ((gd.fTutorial == 0x0))
        goto L_1e3b;
    else
        goto L_1dc9;

L_1dc9:
    if ((0x19 <= lpPlanets->rgMinConc[j]))
        goto L_1df4;
    else
        goto L_1dee;

L_1dee:
    t_merge_1e0f_0001 = 0x19;
    goto L_1e0f;

L_1df4:
    t_merge_1e0f_0001 = lpPlanets->rgMinConc[j];

L_1e0f:
    lpPlanets[iMin].rgMinConc[j] = LOBYTE(t_merge_1e0f_0001);
    goto L_1eaa;

L_1e3b:
    if ((0x1e <= lpPlanets->rgMinConc[j]))
        goto L_1e66;
    else
        goto L_1e60;

L_1e60:
    t_merge_1e81_0001 = 0x1e;
    goto L_1e81;

L_1e66:
    t_merge_1e81_0001 = lpPlanets->rgMinConc[j];

L_1e81:
    lpPlanets[iMin].rgMinConc[j] = LOBYTE(t_merge_1e81_0001);

L_1eaa:
    j = (j + 0x1);

L_1eaf:
    if ((j < 0x3))
        goto L_1d5d;
    else
        goto L_1eb9;

L_1eb9:
    /* untranslated: ss:[bp-0x116] = ((part[26:2](lpPlanets[iMin]) & 0xfffe) | 0x0) */
    /* untranslated: part[24:2](lpPlanets[iMin]) = ((part[24:2](lpPlanets[iMin]) & 0xfff) | 0x0) */
    /* untranslated: part[26:2](lpPlanets[iMin]) = ss:[bp-0x116] */
    FSendPlrMsg(i, 0xa9, iMin, iMin, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
    if ((0x32 >= CAdvantagePoints(rgplr[i])))
        goto L_1f60;
    else
        goto L_1f5a;

L_1f5a:
    t_merge_1f75_0001 = 0x32;
    goto L_1f75;

L_1f60:
    t_merge_1f75_0001 = CAdvantagePoints(rgplr[i]);

L_1f75:
    iT = t_merge_1f75_0001;
    if ((((rgplr[i].wMdPlr >> 0x9) & 0x1) == 0x0))
        goto L_2010;
    else
        goto L_1f98;

L_1f98:
    iT = 0x32;
    if ((((rgplr[i].wMdPlr >> 0xa) & 0x7) < 0x3))
        goto L_2010;
    else
        goto L_1fbd;

L_1fbd:
    lpPlanets[iMin].rgwtMin[0x3] = (lpPlanets[iMin].rgwtMin[0x3] + (int32_t)((lpPlanets[iMin].rgwtMin[0x3] / 0xa)));

L_2010:
    if ((game.fBBSPlay == 0x0))
        goto L_2143;
    else
        goto L_2028;

L_2028:
    pct10 = ((PctTrueMaxGrowth(i) * 0x2) + 0xa);
    lpPlanets[iMin].rgwtMin[0x3] = (uint32_t)((lpPlanets[iMin].rgwtMin[0x3] * (uint32_t)(pct10)));
    lpPlanets[iMin].rgwtMin[0x3] = (int32_t)((lpPlanets[iMin].rgwtMin[0x3] / 0xa));
    /* untranslated: ss:[bp-0x118] = (((lpPlanets[iMin].uGuesses & 0xfff) << 0x2) & 0xfff) */
    lpPlanets[iMin].uGuesses = (lpPlanets[iMin].uGuesses & 0xf000);
    /* untranslated: lpPlanets[iMin].uGuesses = (lpPlanets[iMin].uGuesses | ss:[bp-0x118]) */

L_2143:
    j = GetRaceStat(rgplr[i], rsUseLeftover);
    goto L_25bb;

L_2167:
    ktLeft = LOWORD((0xa * iT));
    pl = lpPlanets[iMin].rgwtMin;
    if ((*(pl + 0x2) > *(pl + 0x6)))
        goto L_21e8;
    else
        goto L_21a5;

L_21a5:
    if ((*(pl + 0x2) < *(pl + 0x6)))
        goto L_21b2;
    else
        goto L_21aa;

L_21aa:
    if ((LOWORD(pl) >= LOWORD(pl[0x1])))
        goto L_21e8;
    else
        goto L_21b2;

L_21b2:
    if ((*(pl + 0x2) > *(pl + 0xa)))
        goto L_21df;
    else
        goto L_21c9;

L_21c9:
    if ((*(pl + 0x2) < *(pl + 0xa)))
        goto L_21d6;
    else
        goto L_21ce;

L_21ce:
    if ((LOWORD(pl) >= LOWORD(pl[0x2])))
        goto L_21df;
    else
        goto L_21d6;

L_21d6:
    iLow = 0x0;
    goto L_221c;

L_21df:
    iLow = 0x2;

L_21e8:
    if ((*(pl + 0x6) > *(pl + 0xa)))
        goto L_2216;
    else
        goto L_21ff;

L_21ff:
    if ((*(pl + 0x6) < *(pl + 0xa)))
        goto L_220d;
    else
        goto L_2204;

L_2204:
    if ((LOWORD(pl[0x1]) >= LOWORD(pl[0x2])))
        goto L_2216;
    else
        goto L_220d;

L_220d:
    iLow = 0x1;
    goto L_221c;

L_2216:
    iLow = 0x2;

L_221c:
    pl[iLow] = (pl[iLow] + (uint32_t)(((ktLeft >> 0x2) + (ktLeft & 0x3))));
    ktLeft = (ktLeft >> 0x2);
    j = 0x0;
    goto L_2286;

L_2261:
    pl[j] = (pl[j] + (uint32_t)(ktLeft));
    j = (j + 0x1);

L_2286:
    if ((j < 0x3))
        goto L_2261;
    else
        goto L_2290;

L_2290:
    if ((((rgplr[i].wMdPlr >> 0x9) & 0x1) == 0x0))
        goto L_25e6;
    else
        goto L_22af;

L_22af:
    if ((((rgplr[i].wMdPlr >> 0xa) & 0x7) >= 0x2))
        goto LConcentrations;
    else
        goto L_22cb;

L_22cb:

LConcentrations:
    if ((iT <= 0x0))
        goto L_22f1;
    else
        goto L_22de;

L_22de:
    if ((iT >= 0x3))
        goto L_22f1;
    else
        goto L_22e8;

L_22e8:
    ktLeft = 0x1;
    goto L_22ff;

L_22f1:
    ktLeft = ((uint32_t)(iT) / 0x2);

L_22ff:
    pb = lpPlanets[iMin].rgMinConc;
    iLow = 0x0;
    j = 0x0;
    goto L_2376;

L_232c:
    if ((pb[j] >= pb[iLow]))
        goto L_2371;
    else
        goto L_2369;

L_2369:
    iLow = j;

L_2371:
    j = (j + 0x1);

L_2376:
    if ((j < 0x3))
        goto L_232c;
    else
        goto L_2380;

L_2380:
    pb[iLow] = (pb[iLow] + LOBYTE(ktLeft));
    ktLeft = ((uint32_t)((ktLeft + 0x1)) / 0x2);
    j = 0x0;
    goto L_23cd;

L_23b1:
    pb[j] = (pb[j] + LOBYTE(ktLeft));
    j = (j + 0x1);

L_23cd:
    if ((j >= 0x3))
        goto L_25e6;
    else
        goto L_23d4;

L_23d4:

L_23da:
    /* untranslated: ss:[bp-0x118] = ((loword((int32_t)(sext16to32((iT >> 0x1)) * 0x100)) + part[20:2](lpPlanets[iMin])) & 0xff00) */
    /* untranslated: ss:[bp-0x116] = ((hiword((int32_t)(sext16to32((iT >> 0x1)) * 0x100)) + part[22:2](lpPlanets[iMin])) & 0xf) */
    /* untranslated: part[20:2](lpPlanets[iMin]) = (part[20:2](lpPlanets[iMin]) & 0xff) */
    /* untranslated: part[22:2](lpPlanets[iMin]) = (part[22:2](lpPlanets[iMin]) & 0xfff0) */
    /* untranslated: part[20:2](lpPlanets[iMin]) = (part[20:2](lpPlanets[iMin]) | ss:[bp-0x118]) */
    /* untranslated: part[22:2](lpPlanets[iMin]) = (part[22:2](lpPlanets[iMin]) | ss:[bp-0x116]) */
    goto L_25e6;

L_2476:
    /* untranslated: ss:[bp-0x11c] = loword((int32_t)(sext16to32((sext16to32(iT) / 0x5)) * 0x100000)) */
    /* untranslated: ss:[bp-0x118] = 0x0 */
    /* untranslated: ss:[bp-0x116] = ((hiword((int32_t)(sext16to32((sext16to32(iT) / 0x5)) * 0x100000)) + part[22:2](lpPlanets[iMin])) & 0xfff0) */
    /* untranslated: part[20:2](lpPlanets[iMin]) = (part[20:2](lpPlanets[iMin]) & 0xffff) */
    /* untranslated: part[22:2](lpPlanets[iMin]) = (part[22:2](lpPlanets[iMin]) & 0xf) */
    /* untranslated: part[20:2](lpPlanets[iMin]) = (part[20:2](lpPlanets[iMin]) | ss:[bp-0x118]) */
    /* untranslated: part[22:2](lpPlanets[iMin]) = (part[22:2](lpPlanets[iMin]) | ss:[bp-0x116]) */
    goto L_25e6;

L_2515:
    /* untranslated: ss:[bp-0x11a] = hiword((int32_t)(sext16to32((sext16to32((iT + 0x5)) / 0xa)) * 0x1)) */
    /* untranslated: ss:[bp-0x118] = ((loword((int32_t)(sext16to32((sext16to32((iT + 0x5)) / 0xa)) * 0x1)) + part[24:2](lpPlanets[iMin])) & 0xfff) */
    /* untranslated: ss:[bp-0x116] = 0x0 */
    /* untranslated: part[24:2](lpPlanets[iMin]) = (part[24:2](lpPlanets[iMin]) & 0xf000) */
    /* untranslated: part[26:2](lpPlanets[iMin]) = (part[26:2](lpPlanets[iMin]) & 0xffff) */
    /* untranslated: part[24:2](lpPlanets[iMin]) = (part[24:2](lpPlanets[iMin]) | ss:[bp-0x118]) */
    /* untranslated: part[26:2](lpPlanets[iMin]) = (part[26:2](lpPlanets[iMin]) | ss:[bp-0x116]) */
    goto L_25e6;

L_25bb:
    if ((j == 0x0))
        goto L_2167;
    else
        goto L_25c3;

L_25c3:
    if ((j == 0x1))
        goto LConcentrations;
    else
        goto L_25cb;

L_25cb:
    if ((j == 0x2))
        goto L_23da;
    else
        goto L_25d3;

L_25d3:
    if ((j == 0x3))
        goto L_2476;
    else
        goto L_25db;

L_25db:
    if ((j != 0x4))
        goto L_2167;
    else
        goto L_25e0;

L_25e0:

L_25e6:
    if ((GetRaceStat(rgplr[i], rsMajorAdv) != raMacintosh))
        goto L_26fd;
    else
        goto L_2607;

L_2607:
    /* untranslated: ss:[bp-0x116] = ((part[22:2](lpPlanets[iMin]) & 0xfff0) | 0x0) */
    /* untranslated: part[20:2](lpPlanets[iMin]) = ((part[20:2](lpPlanets[iMin]) & 0xff) | 0x0) */
    /* untranslated: part[22:2](lpPlanets[iMin]) = ss:[bp-0x116] */
    /* untranslated: ss:[bp-0x116] = ((part[22:2](lpPlanets[iMin]) & 0xf) | 0x0) */
    /* untranslated: part[20:2](lpPlanets[iMin]) = ((part[20:2](lpPlanets[iMin]) & 0xffff) | 0x0) */
    /* untranslated: part[22:2](lpPlanets[iMin]) = ss:[bp-0x116] */
    /* untranslated: ss:[bp-0x116] = ((part[26:2](lpPlanets[iMin]) & 0xffff) | 0x0) */
    /* untranslated: part[24:2](lpPlanets[iMin]) = ((part[24:2](lpPlanets[iMin]) & 0xf000) | 0x0) */
    /* untranslated: part[26:2](lpPlanets[iMin]) = ss:[bp-0x116] */

L_26fd:
    rgplr[i].iPlayer = LOBYTE(i);
    rgplr[i].idPlanetHome = iMin;
    if (((uint16_t)(rgplr[i].rgEnvVarMax[0x0]) != 0xffff))
        goto L_2757;
    else
        goto L_2741;

L_2741:
    iT = (Random(0x63) + 0x1);
    goto L_27a5;

L_2757:
    iT = ((uint16_t)(rgplr[i].rgEnvVarMin[0x0]) + ((uint32_t)(((uint16_t)(rgplr[i].rgEnvVarMax[0x0]) - (uint16_t)(rgplr[i].rgEnvVarMin[0x0]))) / 0x2));

L_27a5:
    lpPlanets[iMin].rgEnvVarOrig[0x0] = LOBYTE(iT);
    /* untranslated: lpPlanets[iMin].rgEnvVar[0x0] = lobyte(ss:[bp-0x116]) */
    if (((uint16_t)(rgplr[i].rgEnvVarMax[0x1]) != 0xffff))
        goto L_2815;
    else
        goto L_27ff;

L_27ff:
    iT = (Random(0x63) + 0x1);
    goto L_2863;

L_2815:
    iT = ((uint16_t)(rgplr[i].rgEnvVarMin[0x1]) + ((uint32_t)(((uint16_t)(rgplr[i].rgEnvVarMax[0x1]) - (uint16_t)(rgplr[i].rgEnvVarMin[0x1]))) / 0x2));

L_2863:
    lpPlanets[iMin].rgEnvVarOrig[0x1] = LOBYTE(iT);
    /* untranslated: lpPlanets[iMin].rgEnvVar[0x1] = lobyte(ss:[bp-0x116]) */
    if (((uint16_t)(rgplr[i].rgEnvVarMax[0x2]) != 0xffff))
        goto L_28d3;
    else
        goto L_28bd;

L_28bd:
    iT = (Random(0x63) + 0x1);
    goto L_2921;

L_28d3:
    iT = ((uint16_t)(rgplr[i].rgEnvVarMin[0x2]) + ((uint32_t)(((uint16_t)(rgplr[i].rgEnvVarMax[0x2]) - (uint16_t)(rgplr[i].rgEnvVarMin[0x2]))) / 0x2));

L_2921:
    lpPlanets[iMin].rgEnvVarOrig[0x2] = LOBYTE(iT);
    /* untranslated: lpPlanets[iMin].rgEnvVar[0x2] = lobyte(ss:[bp-0x116]) */
    if ((((rgplr[i].wMdPlr >> 0x9) & 0x1) != 0x0))
        goto L_2992;
    else
        goto L_2982;

L_2982:
    rgplr[i].pctResearch = 0xf;

L_2992:
    rgplr[i].iTechCur = LOBYTE((((uint16_t)(rgplr[i].iTechCur) & 0xfff0) | 0x0));
    rgplr[i].iTechCur = LOBYTE((((uint16_t)(rgplr[i].iTechCur) & 0xff0f) | 0x60));
    rgplr[i].lResLastYear = 0x0;
    rgplr[i].wScore = 0x0;
    j = 0x0;
    goto L_2a21;

L_2a1c:
    j = (j + 0x1);

L_2a21:
    if ((j >= game.cPlayer))
        goto L_2a4a;
    else
        goto L_2a2d;

L_2a2d:
    rgplr[i].rgmdRelation[j] = 0x0;
    goto L_2a1c;

L_2a4a:
    /* untranslated: part[4:2](rgplr[i]) = ((part[4:2](rgplr[i]) & 0xfff) | 0x1000) */
    lpshdef = LpAlloc(0x5be, htShips);
    fmemmove(&(lpshdef), LpshdefSBT(), 0x24c);
    fmemset(&(lpshdef[0x4]), 0x0, 0x372);
    lpshdef->cBuilt = 0x1;
    lpshdef->cExist = 0x1;
    rglpshdefSB[i] = lpshdef;
    j = 0x1;
    goto L_2b4d;

L_2b0c:
    lpshdef[j].wFlags = ((lpshdef[j].wFlags & 0xfdff) | 0x200);
    j = (j + 0x1);

L_2b4d:
    if ((j < 0xa))
        goto L_2b0c;
    else
        goto L_2b57;

L_2b57:
    if ((GetRaceStat(rgplr[i], rsMajorAdv) != raMassAccel))
        goto L_2c32;
    else
        goto L_2b78;

L_2b78:
    HIWORD(lpshdef->hul.rghs[0x0]) = ((HIWORD(lpshdef->hul.rghs[0x0]) & 0xff00) | 0x7);
    HIWORD(lpshdef->hul.rghs[0x0]) = ((HIWORD(lpshdef->hul.rghs[0x0]) & 0xff) | 0x100);
    if ((game.mdSize <= 0x0))
        goto L_1a47;
    else
        goto L_2bae;

L_2bae:
    *(lpshdef + 0x10e) = ((*(lpshdef + 0x10e) & 0xfdff) | 0x0);
    /* untranslated: ss:[bp-0x116] = ((part[4:2](rgplr[i]) + 0x1000) & 0xf000) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) & 0xfff) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) | ss:[bp-0x116]) */
    *(lpshdef + 0x116) = 0x1;
    *(lpshdef + 0x118) = 0x0;
    *(lpshdef + 0x112) = 0x1;
    *(lpshdef + 0x114) = 0x0;

L_2c32:
    if ((GetRaceStat(rgplr[i], rsMajorAdv) != raStargate))
        goto L_2d72;
    else
        goto L_2c53;

L_2c53:
    if ((game.fTutorial != 0x0))
        goto L_2d72;
    else
        goto L_2c67;

L_2c67:
    HIWORD(lpshdef->hul.rghs[0x0]) = ((HIWORD(lpshdef->hul.rghs[0x0]) & 0xff00) | 0x0);
    HIWORD(lpshdef->hul.rghs[0x0]) = ((HIWORD(lpshdef->hul.rghs[0x0]) & 0xff) | 0x100);
    if ((game.mdSize <= 0x0))
        goto L_1a47;
    else
        goto L_2c9d;

L_2c9d:
    lpshdef[0x1] = lpshdef[0x2];
    *(lpshdef + 0x10e) = ((*(lpshdef + 0x10e) & 0x83ff) | 0x4400);
    *(lpshdef + 0x10e) = ((*(lpshdef + 0x10e) & 0xfdff) | 0x0);
    /* untranslated: ss:[bp-0x116] = ((part[4:2](rgplr[i]) + 0x1000) & 0xf000) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) & 0xfff) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) | ss:[bp-0x116]) */
    *(lpshdef + 0x116) = 0x1;
    *(lpshdef + 0x118) = 0x0;
    *(lpshdef + 0x112) = 0x1;
    *(lpshdef + 0x114) = 0x0;

L_2d72:
    if ((GetRaceStat(rgplr[i], rsMajorAdv) != raMacintosh))
        goto L_1a47;
    else
        goto L_2d93;

L_2d93:
    idHome = rgplr[i].idPlanetHome;
    lpshdef[0x1] = *(lpshdef);
    *(lpshdef + 0x10e) = ((*(lpshdef + 0x10e) & 0x83ff) | 0x4400);
    *(lpshdef) = lpshdef[0x3];
    lpshdef->ishdef = 0x10;
    lpshdef->fFree = 0x0;
    /* untranslated: ss:[bp-0x118] = ((part[4:2](rgplr[i]) + 0x1000) & 0xf000) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) & 0xfff) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) | ss:[bp-0x118]) */
    lpshdef->cExist = 0x0;
    lpshdef->cBuilt = 0x0;
    /* untranslated: part[44:2](lpPlanets[idHome]) = ((part[44:2](lpPlanets[idHome]) & 0xfff0) | 0x1) */

L_2ee8:
    cFleet = 0x0;
    rglpfl = LpAlloc(0x4, htMisc);
    i = 0x0;
    goto L_2f13;

L_2f0e:
    i = (i + 0x1);

L_2f13:
    if ((i >= game.cPlayer))
        goto L_3f0c;
    else
        goto L_2f1f;

L_2f1f:
    idPlayer = i;
    lpshdef = LpAlloc(0x930, htShips);
    fmemset(&(lpshdef), 0x0, 0x930);
    j = 0x0;
    goto L_2fa0;

L_2f5f:
    lpshdef[j].wFlags = ((lpshdef[j].wFlags & 0xfdff) | 0x200);
    j = (j + 0x1);

L_2fa0:
    if ((j < 0x10))
        goto L_2f5f;
    else
        goto L_2faa;

L_2faa:
    rglpshdef[i] = lpshdef;
    idHome = rgplr[i].idPlanetHome;
    raMajor = GetRaceStat(rgplr[i], rsMajorAdv);
    if ((raMajor != 0x6))
        goto L_3053;
    else
        goto L_2ffc;

L_2ffc:
    CreateStartupShip(i, idHome, 0x4, 0x1);
    /* untranslated: part[46:2](lpPlanets[idHome]) = ((part[46:2](lpPlanets[idHome]) & 0xc3ff) | 0x400) */
    goto L_318f;

L_3053:
    if ((raMajor != 0x2))
        goto L_30c0;
    else
        goto L_305d;

L_305d:
    CreateStartupShip(i, idHome, 0x3, 0x1);
    if (((uint16_t)(rgplr[i].rgTech[0x3]) < 0x3))
        goto L_318f;
    else
        goto L_308d;

L_308d:
    CreateStartupShip(i, idHome, 0x7, 0x1);
    CreateStartupShip(i, idHome, 0xd, 0x1);

L_30c0:
    if ((raMajor != 0x9))
        goto L_30fd;
    else
        goto L_30ca;

L_30ca:
    CreateStartupShip(i, idHome, 0x3, 0x1);
    CreateStartupShip(i, idHome, 0x4, 0x1);
    goto L_318f;

L_30fd:
    if ((raMajor != 0x1))
        goto L_3177;
    else
        goto L_3107;

L_3107:
    if (((uint16_t)(rgplr[i].rgTech[0x0]) < 0x2))
        goto L_3129;
    else
        goto L_3123;

L_3123:
    t_merge_312c_0001 = 0x5;
    goto L_312c;

L_3129:
    t_merge_312c_0001 = 0x2;

L_312c:
    CreateStartupShip(i, idHome, t_merge_312c_0001, 0x1);
    if ((((rgplr[i].wMdPlr >> 0x9) & 0x1) != 0x0))
        goto L_318f;
    else
        goto L_315c;

L_315c:
    CreateStartupShip(i, idHome, 0x1, 0x1);

L_3177:
    CreateStartupShip(i, idHome, 0x2, 0x1);

L_318f:
    if ((raMajor != 0x0))
        goto L_31e8;
    else
        goto L_3199;

L_3199:
    ishRet = CreateStartupShip(i, idHome, 0xc, 0x1);
    j = 0x1;
    goto L_31db;

L_31be:
    CreateStartupShip(i, idHome, ishRet, 0x0);
    j = (j + 0x1);

L_31db:
    if ((j >= 0x3))
        goto L_324e;
    else
        goto L_31e2;

L_31e2:

L_31e8:
    if ((raMajor != 0x7))
        goto L_320d;
    else
        goto L_31f2;

L_31f2:
    CreateStartupShip(i, idHome, 0xb, 0x1);
    goto L_324e;

L_320d:
    if ((raMajor != 0x8))
        goto L_3232;
    else
        goto L_3217;

L_3217:
    CreateStartupShip(i, idHome, 0xa, 0x1);
    goto L_324e;

L_3232:
    ishRet = CreateStartupShip(i, idHome, 0x9, 0x1);

L_324e:
    if ((raMajor != 0x5))
        goto L_328b;
    else
        goto L_3258;

L_3258:
    CreateStartupShip(i, idHome, 0x10, 0x1);
    CreateStartupShip(i, idHome, 0x12, 0x1);
    goto L_39dd;

L_328b:
    if ((raMajor != 0x3))
        goto L_32b0;
    else
        goto L_3295;

L_3295:
    CreateStartupShip(i, idHome, 0x11, 0x1);
    goto L_39dd;

L_32b0:
    if ((raMajor != 0x7))
        goto L_32fa;
    else
        goto L_32ba;

L_32ba:
    CreateStartupShip(i, idHome, 0x7, 0x1);
    CreateStartupShip(i, idHome, 0x8, 0x1);
    if ((game.mdSize > 0x0))
        goto LGive2ndPlanet;
    else
        goto L_32f1;

L_32f1:

L_32fa:
    if ((raMajor != 0x6))
        goto L_396d;
    else
        goto L_3304;

L_3304:
    if ((game.mdSize <= 0x0))
        goto L_396d;
    else
        goto LGive2ndPlanet;

LGive2ndPlanet:
    lpplPicked = 0x0;
    lpplClosest = 0x0;
    ptHome.x = rgptPlan[idHome].x;
    ptHome.y = rgptPlan[idHome].y;
    cFit = 0x0;
    lDistMin2 = (int32_t)(((uint32_t)(((uint32_t)(dGal) * 0xf)) / 0x64));
    lDistMin2 = (uint32_t)((lDistMin2 * lDistMin2));
    lDistMax2 = (int32_t)(((uint32_t)(((uint32_t)(dGal) * 0x17)) / 0x64));
    lDistMax2 = (uint32_t)((lDistMax2 * lDistMax2));
    lDistIdeal2 = (int32_t)(((uint32_t)(((uint32_t)(dGal) * 0x14)) / 0x64));
    lDistIdeal2 = (uint32_t)((lDistIdeal2 * lDistIdeal2));
    lBest = 0x989680;
    pptMax = rgptPlan[cPlanMax];
    ppt = 0x2f40;
    lppl = lpPlanets;
    goto L_3468;

L_3450:
    ppt = (ppt + 0x4);
    lppl = (lppl + 0x1);

L_3468:
    if ((ppt >= pptMax))
        goto L_35a3;
    else
        goto L_3475;

L_3475:
    if ((lppl->iPlayer != 0xffff))
        goto L_3450;
    else
        goto L_3483;

L_3483:
    dx = (ppt->x - ptHome.x);
    dy = (ppt->y - ptHome.y);
    lDistCur2 = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lDistCur2) < HIWORD(lDistMin2)))
        goto L_354d;
    else
        goto L_34f3;

L_34f3:
    if ((HIWORD(lDistCur2) > HIWORD(lDistMin2)))
        goto L_3501;
    else
        goto L_34f8;

L_34f8:
    if ((LOWORD(lDistCur2) < LOWORD(lDistMin2)))
        goto L_354d;
    else
        goto L_3501;

L_3501:
    if ((HIWORD(lDistCur2) > HIWORD(lDistMax2)))
        goto L_354d;
    else
        goto L_3512;

L_3512:
    if ((HIWORD(lDistCur2) < HIWORD(lDistMax2)))
        goto L_3520;
    else
        goto L_3517;

L_3517:
    if ((LOWORD(lDistCur2) > LOWORD(lDistMax2)))
        goto L_354d;
    else
        goto L_3520;

L_3520:
    cFit = (cFit + 0x1);
    if ((Random(cFit) != 0x0))
        goto L_3450;
    else
        goto L_353a;

L_353a:
    lpplPicked = lppl;

L_354d:
    if ((LOWORD(lpplPicked) != 0x0))
        goto L_3450;
    else
        goto L_3557;

L_3557:
    if ((HIWORD(lpplPicked) != 0x0))
        goto L_3450;
    else
        goto L_3561;

L_3561:
    if ((HIWORD(lDistCur2) > HIWORD(lBest)))
        goto L_3450;
    else
        goto L_3572;

L_3572:
    if ((HIWORD(lDistCur2) < HIWORD(lBest)))
        goto L_3580;
    else
        goto L_3577;

L_3577:
    if ((LOWORD(lDistCur2) >= LOWORD(lBest)))
        goto L_3450;
    else
        goto L_3580;

L_3580:
    lBest = lDistCur2;
    lpplClosest = lppl;

L_35a3:
    if ((LOWORD(lpplPicked) != 0x0))
        goto L_35c7;
    else
        goto L_35ad;

L_35ad:
    if ((HIWORD(lpplPicked) != 0x0))
        goto L_35c7;
    else
        goto L_35b7;

L_35b7:
    lpplPicked = lpplClosest;

L_35c7:
    lpplPicked->iWarpFling = 0x1;
    cFit = 0x0;

L_35e3:
    if ((PctPlanetDesirability(lpplPicked, i) >= 0xa))
        goto L_366a;
    else
        goto L_35ff;

L_35ff:
    t_35ff = cFit;
    cFit = (cFit + 0x1);
    if ((t_35ff >= 0x64))
        goto L_366a;
    else
        goto L_3610;

L_3610:
    j = 0x0;
    goto L_365d;

L_3619:
    Random(0x61);
    /* untranslated: lpplPicked->rgEnvVarOrig[j] = lobyte((callresult(int16_t) + 0x2)) */
    /* untranslated: lpplPicked->rgEnvVar[j] = lobyte((callresult(int16_t) + 0x2)) */
    j = (j + 0x1);

L_365d:
    if ((j >= 0x3))
        goto L_35e3;
    else
        goto L_3664;

L_3664:

L_366a:
    if ((cFit < 0x64))
        goto L_36fe;
    else
        goto L_3674;

L_3674:
    j = 0x0;
    goto L_36f4;

L_367d:
    lpplPicked->rgEnvVar[j] = lpPlanets[idHome].rgEnvVar[j];
    lpplPicked->rgEnvVarOrig[j] = lpPlanets[idHome].rgEnvVarOrig[j];
    j = (j + 0x1);

L_36f4:
    if ((j < 0x3))
        goto L_367d;
    else
        goto L_36fe;

L_36fe:
    lpplPicked->iPlayer = i;
    lpplPicked->fStarbase = 0x1;
    lpplPicked->isb = 0x1;
    *(lpplPicked + 0x18) = ((*(lpplPicked + 0x18) & 0xffff) | 0x0);
    lpplPicked->fArtifact = 0x0;
    *(lpplPicked + 0x14) = ((*(lpplPicked + 0x14) & 0xffff) | 0x0);
    lpplPicked->cFactories = 0x4;
    lpplPicked->cMines = 0xa;
    lpplPicked->rgwtMin[0x3] = (int32_t)(((int32_t)((lpPlanets[idHome].rgwtMin[0x3] * 0x2)) / 0x5));
    /* untranslated: lpplPicked->uGuesses = ((lpplPicked->uGuesses & 0xf000) | ((words(LOWORD(lpplPicked->rgwtMin[0x3]), 0x0) / 0x4) & 0xfff)) */
    j = 0x0;
    goto L_3854;

L_381f:
    lpplPicked->rgwtMin[j] = (uint32_t)((Random(0xc8) + 0x64));
    j = (j + 0x1);

L_3854:
    if ((j < 0x3))
        goto L_381f;
    else
        goto L_385e;

L_385e:
    lpplPicked->iScanner = 0x0;
    lpPlanets[idHome].rgwtMin[0x3] = (int32_t)(((int32_t)((lpPlanets[idHome].rgwtMin[0x3] * 0x4)) / 0x5));
    /* untranslated: lpPlanets[idHome].uGuesses = ((lpPlanets[idHome].uGuesses & 0xf000) | ((words(LOWORD(lpPlanets[idHome].rgwtMin[0x3]), 0x0) / 0x4) & 0xfff))
     */
    CreateStartupShip(i, lpplPicked->id, 0x0, 0x0);
    goto L_39dd;

L_396d:
    if ((raMajor != 0x9))
        goto L_39dd;
    else
        goto L_3977;

L_3977:
    if (((uint16_t)(rgplr[i].rgTech[0x3]) < 0x4))
        goto L_3999;
    else
        goto L_3993;

L_3993:
    t_merge_399c_0001 = 0x8;
    goto L_399c;

L_3999:
    t_merge_399c_0001 = 0x6;

L_399c:
    CreateStartupShip(i, idHome, t_merge_399c_0001, 0x1);
    CreateStartupShip(i, idHome, 0x7, 0x1);
    CreateStartupShip(i, idHome, 0xe, 0x1);

L_39dd:
    if ((GetRaceGrbit(rgplr[i], ibitRaceOBRM) != 0x0))
        goto L_3a53;
    else
        goto L_39fe;

L_39fe:
    if ((GetRaceGrbit(rgplr[i], ibitRaceARM) == 0x0))
        goto L_3a53;
    else
        goto L_3a1f;

L_3a1f:
    ishRet = CreateStartupShip(i, idHome, 0xf, 0x1);
    CreateStartupShip(i, idHome, ishRet, 0x0);

L_3a53:
    j = 0x0;
    goto L_3a61;

L_3a5c:
    j = (j + 0x1);

L_3a61:
    if ((j >= (uint16_t)(rgplr[i].cShDef)))
        goto L_3e96;
    else
        goto L_3a7a;

L_3a7a:
    chs = rglpshdef[i][j].hul.chs;
    lphs = rglpshdef[i][j].hul.rghs;
    k = 0x0;
    goto L_3ae7;

L_3acf:
    k = (k + 0x1);
    lphs = (lphs + 0x1);

L_3ae7:
    if ((k >= chs))
        goto L_3a5c;
    else
        goto L_3af4;

L_3af4:
    cTry = 0x0;
    part.hs.grhst = lphs->grhst;
    HIWORD(part.hs) = *(lphs + 0x2);
    goto L_3dc0;

L_3b14:
    if ((part.hs.iItem != iengineQuickJump5))
        goto L_3e03;
    else
        goto L_3b23;

L_3b23:
    if (((uint16_t)(rgplr[i].rgEnvVar[0x2]) == 0xffff))
        goto L_3b77;
    else
        goto L_3b3b;

L_3b3b:
    if ((rglpshdef[i][j].hul.ihuldef != ihuldefColonyShip))
        goto L_3b77;
    else
        goto L_3b5f;

L_3b5f:
    if (((uint16_t)(rgplr[i].rgEnvVar[0x2]) < 0x55))
        goto L_3b8c;
    else
        goto L_3b77;

L_3b77:
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0xa;

L_3b8c:
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x5;
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x4;
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x2;
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x3;

L_3be3:
    if ((part.hs.iItem == ishieldMoleSkinShield))
        goto L_3c01;
    else
        goto L_3bf2;

L_3bf2:
    if ((part.hs.iItem != ishieldCowHideShield))
        goto L_3e03;
    else
        goto L_3c01;

L_3c01:
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x2;
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x1;

L_3c2e:
    if ((part.hs.iItem == iarmorTritanium))
        goto L_3c4c;
    else
        goto L_3c3d;

L_3c3d:
    if ((part.hs.iItem != iarmorCrobmnium))
        goto L_3e03;
    else
        goto L_3c4c;

L_3c4c:
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x2;
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x1;

L_3c79:
    if ((part.hs.iItem == ibeamLaser))
        goto L_3c97;
    else
        goto L_3c88;

L_3c88:
    if ((part.hs.iItem != ibeamXRayLaser))
        goto L_3e03;
    else
        goto L_3c97;

L_3c97:
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x3;
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x1;

L_3cc4:
    if ((part.hs.iItem != itorpAlphaTorpedo))
        goto L_3e03;
    else
        goto L_3cd3;

L_3cd3:
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x1;

L_3ceb:
    if ((part.hs.iItem != ibombLadyFingerBomb))
        goto L_3e03;
    else
        goto L_3cfa;

L_3cfa:
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x1;

L_3d12:
    if ((part.hs.iItem == iminingRoboMidgetMiner))
        goto L_3d30;
    else
        goto L_3d21;

L_3d21:
    if ((part.hs.iItem != iminingRoboMiniMiner))
        goto L_3e03;
    else
        goto L_3d30;

L_3d30:
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x2;
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x0;

L_3d5d:
    if ((part.hs.iItem == iscannerBatScanner))
        goto L_3d7b;
    else
        goto L_3d6c;

L_3d6c:
    if ((part.hs.iItem != iscannerRhinoScanner))
        goto L_3e03;
    else
        goto L_3d7b;

L_3d7b:
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x4;
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x2;
    cTry = (cTry + 0x1);
    rgTry[cTry] = 0x1;

L_3dc0:
    if ((part.hs.grhst == hstEngine))
        goto L_3b14;
    else
        goto L_3dc8;

L_3dc8:
    if ((part.hs.grhst == hstScanner))
        goto L_3d5d;
    else
        goto L_3dd0;

L_3dd0:
    if ((part.hs.grhst == hstShield))
        goto L_3be3;
    else
        goto L_3dd8;

L_3dd8:
    if ((part.hs.grhst == hstArmor))
        goto L_3c2e;
    else
        goto L_3de0;

L_3de0:
    if ((part.hs.grhst == hstBeam))
        goto L_3c79;
    else
        goto L_3de8;

L_3de8:
    if ((part.hs.grhst == hstTorp))
        goto L_3cc4;
    else
        goto L_3df0;

L_3df0:
    if ((part.hs.grhst == hstBomb))
        goto L_3ceb;
    else
        goto L_3df8;

L_3df8:
    if ((part.hs.grhst == hstMining))
        goto L_3d12;
    else
        goto L_3e03;

L_3e03:
    l = 0x0;
    goto L_3e83;

L_3e0c:
    part.hs.iItem = rgTry[l];
    if ((FLookupPart(&(part)) != 0x1))
        goto L_3e7e;
    else
        goto L_3e4a;

L_3e4a:
    *(lphs + 0x2) = ((*(lphs + 0x2) & 0xff00) | (rgTry[l] & 0xff));
    goto L_3acf;

L_3e7e:
    l = (l + 0x1);

L_3e83:
    if ((l >= cTry))
        goto L_3acf;
    else
        goto L_3e8d;

L_3e8d:

L_3e96:
    if ((GetRaceStat(rgplr[i], rsMajorAdv) != raMacintosh))
        goto L_2f0e;
    else
        goto L_3eb7;

L_3eb7:
    /* untranslated: ss:[bp-0x122] = ((part[26:2](lpPlanets[idHome]) & 0xfffe) | 0x1) */
    /* untranslated: part[24:2](lpPlanets[idHome]) = ((part[24:2](lpPlanets[idHome]) & 0xfff) | 0xf000) */
    /* untranslated: part[26:2](lpPlanets[idHome]) = ss:[bp-0x122] */

L_3f0c:
    idPlayer = 0xffff;
    if ((lpPlanets->iPlayer != 0xffff))
        goto L_3f5c;
    else
        goto L_3f20;

L_3f20:
    j = 0x0;
    goto L_3f52;

L_3f29:
    lpPlanets->rgwtMin[j] = 0x0;
    j = (j + 0x1);

L_3f52:
    if ((j < 0x3))
        goto L_3f29;
    else
        goto L_3f5c;

L_3f5c:
    i = 0x0;
    goto L_3f6a;

L_3f65:
    i = (i + 0x1);

L_3f6a:
    if ((i >= game.cPlayer))
        goto L_3fe5;
    else
        goto L_3f76;

L_3f76:
    rgcbtlplan[i] = 0x5;
    rglpbtlplan[i] = LpAlloc(0x240, htShips);
    j = 0x0;
    goto L_3fd8;

L_3fa8:
    InitBattlePlan(&(rglpbtlplan[i][j]), j, i);
    j = (j + 0x1);

L_3fd8:
    if ((j >= 0x5))
        goto L_3f65;
    else
        goto L_3fdf;

L_3fdf:

L_3fe5:
    game.cPlanMax = cPlanMax;
    if ((game.fNoRandom == 0x0))
        goto L_4007;
    else
        goto L_3fff;

L_3fff:
    iBest = 0x0;
    goto L_4037;

L_4007:
    /* untranslated: iBest = (byte cs:[game.mdSize] + Random(byte cs:[game.mdSize+0x6])) */

L_4037:
    if ((iBest <= 0x0))
        goto L_41b9;
    else
        goto L_4040;

L_4040:
    i = 0x0;
    goto L_41ad;

L_4049:
    j = 0x0;
    goto L_419e;

L_4052:
    lpth = LpthNew(0x0, ithWormhole);
    *(lpth + 0x6) = ((*(lpth + 0x6) & 0xfffc) | (Random(0x3) & 0x3));
    if ((j != 0x1))
        goto L_40d8;
    else
        goto L_40a2;

L_40a2:
    lpthLast = LpthFromId(idLast);
    lpth->thw.idPartner = lpthLast->idFull;
    *(lpthLast + 0xc) = lpth->idFull;
    goto L_40e3;

L_40d8:
    idLast = lpth->idFull;

L_40e3:
    k = 0x0;
    iMax = 0x10;

L_40ef:
    t_40ef = k;
    k = (k + 0x1);
    if ((t_40ef >= 0x64))
        goto L_417b;
    else
        goto L_4100;

L_4100:
    lpth->pt.x = (Random(dGal) + 0x3e8);
    lpth->pt.y = (Random(dGal) + 0x3e8);
    iLow = IValidateWormholePos(lpth);
    if ((iLow == 0x0))
        goto L_417b;
    else
        goto L_4149;

L_4149:

L_414f:
    if ((iLow >= iMax))
        goto L_40ef;
    else
        goto L_415c;

L_415c:
    iMax = iLow;
    pt.x = lpth->pt.x;
    pt.y = lpth->pt.y;

L_417b:
    if ((iLow == 0x0))
        goto L_4199;
    else
        goto L_4185;

L_4185:
    lpth->pt.x = pt.x;
    lpth->pt.y = pt.y;

L_4199:
    j = (j + 0x1);

L_419e:
    if ((j < 0x2))
        goto L_4052;
    else
        goto L_41a8;

L_41a8:
    i = (i + 0x1);

L_41ad:
    if ((i < iBest))
        goto L_4049;
    else
        goto L_41b9;

L_41b9:
    i = 0x0;
    goto L_41c7;

L_41c2:
    i = (i + 0x1);

L_41c7:
    if ((i >= game.cPlayer))
        goto L_4279;
    else
        goto L_41d3;

L_41d3:
    if ((((rgplr[i].wFlags >> 0x4) & 0x1) == 0x0))
        goto L_41c2;
    else
        goto L_41f5;

L_41f5:
    FSendPlrMsg2(i, 0x117, 0xffff, 0x0, 0x0);
    j = 0x0;
    goto L_421f;

L_421a:
    j = (j + 0x1);

L_421f:
    if ((j >= game.cPlayer))
        goto L_41c2;
    else
        goto L_422b;

L_422b:
    if ((i == j))
        goto L_421a;
    else
        goto L_4238;

L_4238:
    if ((((rgplr[i].wMdPlr >> 0x9) & 0x1) != 0x0))
        goto L_421a;
    else
        goto L_4257;

L_4257:
    FSendPlrMsg2(j, 0x182, 0xffff, i, 0x0);

L_4279:
    iplrSingle = 0xffff;
    i = 0x0;
    goto L_428d;

L_4288:
    i = (i + 0x1);

L_428d:
    if ((i >= game.cPlayer))
        goto L_4308;
    else
        goto L_4299;

L_4299:
    if ((((rgplr[i].wMdPlr >> 0x9) & 0x1) == 0x0))
        goto L_42d7;
    else
        goto L_42b8;

L_42b8:
    if ((((rgplr[i].wMdPlr >> 0xd) & 0x7) != 0x7))
        goto L_42ef;
    else
        goto L_42d7;

L_42d7:
    if ((iplrSingle != 0xffff))
        goto L_4308;
    else
        goto L_42de;

L_42de:

L_42e4:
    iplrSingle = i;
    goto L_4288;

L_42ef:
    rgplr[i].lSalt = 0x94dabee;

L_4308:
    if ((i != game.cPlayer))
        goto L_4324;
    else
        goto L_4314;

L_4314:
    if ((iplrSingle == 0xffff))
        goto L_4324;
    else
        goto L_431e;

L_431e:
    t_merge_4327_0001 = 0x1;
    goto L_4327;

L_4324:
    t_merge_4327_0001 = 0x0;

L_4327:
    game.wCrap = ((game.wCrap & 0xfffb) | ((t_merge_4327_0001 & 0x1) << 0x2));
    if ((game.fSinglePlr == 0x0))
        goto L_43b8;
    else
        goto L_4357;

L_4357:
    i = 0x0;
    goto L_4365;

L_4360:
    i = (i + 0x1);

L_4365:
    if ((i >= game.cPlayer))
        goto L_43b8;
    else
        goto L_4371;

L_4371:
    j = 0x0;
    goto L_437f;

L_437a:
    j = (j + 0x1);

L_437f:
    if ((j >= game.cPlayer))
        goto L_4360;
    else
        goto L_438b;

L_438b:
    if ((i == j))
        goto L_437a;
    else
        goto L_4398;

L_4398:
    rgplr[i].rgmdRelation[j] = 0x2;

L_43b8:
    if ((game.fTutorial != 0x0))
        goto L_43d8;
    else
        goto L_43cc;

L_43cc:
    game.lid = GetTickCount();

L_43d8:
    _wsprintf(szWork, "%s.xy", &(szBase));
    if ((FCreateFile(dtXY, 0xffff, 0x0) != 0x0))
        goto L_443d;
    else
        goto L_4411;

L_4411:
    AlertSz(PszFormatIds(idsUnableCreateUniverseDefinitionFile, 0x0), MB_ICONHAND);
    DestroyCurGame();
    return 0x0;

L_443d:
    WriteRt(rtGame, 0x40, game);
    xOld = 0x3e8;
    i = 0x0;
    goto L_4563;

L_4463:
    /* untranslated: ss:[bp-0x116] = signhiword(rgptPlan[i].y) */
    /* untranslated: LOWORD(starpack) = (starpack.dx | loword((int32_t)(words((rgptPlan[i].y & 0xfff), 0x0) << 0xa))) */
    /* untranslated: HIWORD(starpack) = ((HIWORD(starpack) & 0xffc0) | hiword((int32_t)(words((rgptPlan[i].y & 0xfff), 0x0) << 0xa))) */
    /* untranslated: ss:[bp-0x116] = signhiword(rgidPlan[i]) */
    /* untranslated: LOWORD(starpack) = ((LOWORD(starpack) & 0xffff) | loword((int32_t)(words((rgidPlan[i] & 0x3ff), 0x0) << 0x16))) */
    /* untranslated: HIWORD(starpack) = ((HIWORD(starpack) & 0x3f) | hiword((int32_t)(words((rgidPlan[i] & 0x3ff), 0x0) << 0x16))) */
    dx = (rgptPlan[i].x - xOld);
    /* untranslated: starpack.dx = loword((int32_t)(words((dx & 0x3ff), 0x0) << 0x0)) */
    /* untranslated: HIWORD(starpack) = ((HIWORD(starpack) & 0xffff) | hiword((int32_t)(words((dx & 0x3ff), 0x0) << 0x0))) */
    RgToStream(&(starpack), 0x4);
    xOld = rgptPlan[i].x;
    i = (i + 0x1);

L_4563:
    if ((i < cPlanMax))
        goto L_4463;
    else
        goto L_4570;

L_4570:
    i = game.cPlayer;
    WriteRt(rtEOF, 0x2, &(i));
    StreamClose();
    i = 0xffff;
    goto L_45a2;

L_459d:
    i = (i + 0x1);

L_45a2:
    if ((i >= game.cPlayer))
        goto L_45c5;
    else
        goto L_45ae;

L_45ae:
    FWriteDataFile(szBase, i, 0x0);
    goto L_459d;

L_45c5:
    if ((fBatchMode == 0x0))
        goto L_45d4;
    else
        goto L_45ce;

L_45ce:
    return 0x1;

L_45d4:
    if ((game.fSinglePlr == 0x0))
        goto L_4672;
    else
        goto L_45e6;

L_45e6:
    DestroyCurGame();
    _wsprintf(szExt, MPCTD, (iplrSingle + 0x1));
    if ((FLoadGame(szBase, szExt) != 0x0))
        goto L_464a;
    else
        goto L_4623;

L_4623:
    AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, 0x0), MB_ICONHAND);
    return 0x0;

L_464a:
    idPlayer = iplrSingle;
    CreateChildWindows();
    SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0x0);
    goto L_4683;

L_4672:
    idPlayer = 0xffff;
    imemLogCur = 0x0;
    CreateChildWindows();

L_4683:
    return 0x1;
}

int16_t CreateStartupShip(int16_t iplr, int16_t idPlanet, int16_t ishdef, int16_t fAddShdef) {
    int16_t ishMac;
    FLEET  *lpfl;
    char    t_46ad;

L_4690:
    if ((fAddShdef == 0x0))
        goto L_476b;
    else
        goto L_46a2;

L_46a2:
    t_46ad = rgplr[iplr].cShDef;
    rgplr[iplr].cShDef = (rgplr[iplr].cShDef + 0x1);
    ishMac = (uint16_t)(t_46ad);
    rglpshdef[iplr][ishMac] = LpshdefT()[ishdef];
    rglpshdef[iplr][ishMac].wFlags = ((rglpshdef[iplr][ishMac].wFlags & 0x83ff) | ((ishMac & 0x1f) * 0x400));
    ishdef = ishMac;

L_476b:
    rglpshdef[iplr][ishdef].cExist = (rglpshdef[iplr][ishdef].cExist + 0x1);
    rglpshdef[iplr][ishdef].cBuilt = (rglpshdef[iplr][ishdef].cBuilt + 0x1);
    lpfl = LpflNew(iplr, idPlanet);
    lpfl->rgcsh[ishdef] = 0x1;
    lpfl->rgwtMin[0x4] = LGetFleetStat(lpfl, 0x1);
    lpfl->iplan = 0x0;

L_480c:
    return ishdef;
}

int16_t GenNewGameFromFile(char *pszFile) {
    int32_t rgl[10];
    int16_t cPlr;
    int16_t rgplrbmp[16];
    int16_t cNum;
    int16_t c;
    int16_t i;
    int16_t fSuccess;
    char   *lpbStart;
    jmp_buf env[9];
    int16_t j;
    char   *lpb;
    char   *lpbDef;
    int16_t cb;
    char   *pchT;
    int16_t idAi;
    int16_t lvlAi;
    int16_t t_merge_5cde_0001;

L_4812:
    fSuccess = 0x0;
    strcpy(szWork, pszFile);
    penvMem = env;
    if ((setjmp(env) != 0x0))
        goto LError;
    else
        goto L_4846;

L_4846:

L_484c:
    if ((ini.fLogging == 0x0))
        goto L_4893;
    else
        goto L_485f;

L_485f:
    strcpy(szBase, pszFile);
    pchT = strrchr(szBase, 0x2e);
    *(pchT) = 0x0;
    TurnLog(idsGeneratingYearD);

L_4893:
    memset(&(game), 0x0, 0x40);
    StreamOpen(pszFile, 0x20);
    cb = LOWORD(filelength(hf));
    if ((cb < 0x3e80))
        goto L_48de;
    else
        goto L_48cf;

L_48cf:
    FileError(idmMultitudeEnemiesHaveMountedProngAttackResulting);
    goto LError;

L_48de:
    lpbDef = LpAlloc((cb + 0x1), htPerm);
    lpbDefMac = &(lpbDef[cb]);
    RgFromStream(&(lpbDef), cb);
    StreamClose();
    lpbDef[cb] = 0x0;
    lpb = lpbDef;
    lpbStart = PszGetLine(&(lpb));
    if (((uint16_t)(*(lpbStart)) == 0x0))
        goto L_4975;
    else
        goto L_495f;

L_495f:
    if ((fstrlen(lpbStart) <= 0x1f))
        goto L_4999;
    else
        goto L_4975;

L_4975:
    AlertSz(PszFormatIds(idsIllegalGameTitle, 0x0), MB_ICONHAND);
    goto LError;

L_4999:
    fstrcpy("", lpbStart);
    if ((LOWORD(lpb) < LOWORD(lpbDefMac)))
        goto L_49e1;
    else
        goto LUniDefShort;

LUniDefShort:
    AlertSz(PszFormatIds(idsUniverseDefinitionFileAppearsTooShort, 0x0), MB_ICONHAND);
    goto LError;

L_49e1:
    lpbStart = PszGetLine(&(lpb));
    cNum = CParseNumbers(lpbStart, rgl, 0x4);
    if ((cNum != 0xffff))
        goto L_4a39;
    else
        goto LUniDefError;

LUniDefError:
    AlertSz(PszFormatIds(idsLine2HasBadUniverseDefinitionParameter, 0x0), MB_ICONHAND);
    goto LError;

L_4a39:
    i = 0x0;
    goto L_4abb;

L_4a41:
    if ((i >= 0x3))
        goto L_4ab7;
    else
        goto L_4a4a;

L_4a4a:
    if ((HIWORD(rgl[i]) < 0x0))
        goto LUniDefError;
    else
        goto L_4a5f;

L_4a5f:
    if ((HIWORD(rgl[i]) > 0x0))
        goto L_4a6c;
    else
        goto L_4a64;

L_4a64:
    if ((LOWORD(rgl[i]) < 0x0))
        goto LUniDefError;
    else
        goto L_4a6c;

L_4a6c:
    if ((HIWORD(rgl[i]) > 0x0))
        goto LUniDefError;
    else
        goto L_4a81;

L_4a81:
    if ((HIWORD(rgl[i]) < 0x0))
        goto L_4a8e;
    else
        goto L_4a86;

L_4a86:
    if ((LOWORD(rgl[i]) > 0x4))
        goto LUniDefError;
    else
        goto L_4a8e;

L_4a8e:
    if ((LOWORD(rgl[i]) != 0x4))
        goto L_4ab7;
    else
        goto L_4aa2;

L_4aa2:
    if ((HIWORD(rgl[i]) != 0x0))
        goto L_4ab7;
    else
        goto L_4aab;

L_4aab:
    if ((i != 0x0))
        goto LUniDefError;
    else
        goto L_4ab1;

L_4ab1:

L_4ab7:
    i = (i + 0x1);

L_4abb:
    if ((i < cNum))
        goto L_4a41;
    else
        goto L_4ac6;

L_4ac6:
    if ((cNum < 0x1))
        goto L_4ad8;
    else
        goto L_4acf;

L_4acf:
    game.mdSize = LOWORD(rgl[0x0]);

L_4ad8:
    if ((cNum < 0x2))
        goto L_4aea;
    else
        goto L_4ae1;

L_4ae1:
    game.mdDensity = LOWORD(rgl[0x1]);

L_4aea:
    if ((cNum < 0x3))
        goto L_4afc;
    else
        goto L_4af3;

L_4af3:
    game.mdStartDist = LOWORD(rgl[0x2]);

L_4afc:
    if ((cNum < 0x4))
        goto L_4b15;
    else
        goto L_4b05;

L_4b05:
    Randomize(rgl[0x3]);

L_4b15:
    if ((LOWORD(lpb) >= LOWORD(lpbDefMac)))
        goto LUniDefShort;
    else
        goto L_4b21;

L_4b21:

L_4b27:
    lpbStart = PszGetLine(&(lpb));
    cNum = CParseNumbers(lpbStart, rgl, 0x7);
    if ((cNum != 0xffff))
        goto L_4b7f;
    else
        goto LUniDefError3;

LUniDefError3:
    AlertSz(PszFormatIds(idsLine3HasBadUniverseDefinitionParameter, 0x0), MB_ICONHAND);
    goto LError;

L_4b7f:
    i = 0x0;
    goto L_4bd2;

L_4b87:
    if ((HIWORD(rgl[i]) < 0x0))
        goto LUniDefError3;
    else
        goto L_4b9c;

L_4b9c:
    if ((HIWORD(rgl[i]) > 0x0))
        goto L_4ba9;
    else
        goto L_4ba1;

L_4ba1:
    if ((LOWORD(rgl[i]) < 0x0))
        goto LUniDefError3;
    else
        goto L_4ba9;

L_4ba9:
    if ((HIWORD(rgl[i]) < 0x0))
        goto L_4bce;
    else
        goto L_4bbe;

L_4bbe:
    if ((HIWORD(rgl[i]) > 0x0))
        goto LUniDefError3;
    else
        goto L_4bc3;

L_4bc3:
    if ((LOWORD(rgl[i]) > 0x1))
        goto LUniDefError3;
    else
        goto L_4bc8;

L_4bc8:

L_4bce:
    i = (i + 0x1);

L_4bd2:
    if ((i < cNum))
        goto L_4b87;
    else
        goto L_4bdd;

L_4bdd:
    if ((cNum < 0x1))
        goto L_4c04;
    else
        goto L_4be6;

L_4be6:
    game.wCrap = ((game.wCrap & 0xfffe) | (LOWORD(rgl[0x0]) & 0x1));

L_4c04:
    if ((cNum < 0x2))
        goto L_4c2d;
    else
        goto L_4c0d;

L_4c0d:
    game.wCrap = ((game.wCrap & 0xfffd) | ((LOWORD(rgl[0x1]) & 0x1) << 0x1));

L_4c2d:
    if ((cNum < 0x3))
        goto L_4c5e;
    else
        goto L_4c36;

L_4c36:
    game.wCrap = ((game.wCrap & 0xffdf) | ((LOWORD(rgl[0x2]) & 0x1) << 0x5));

L_4c5e:
    if ((cNum < 0x4))
        goto L_4c8b;
    else
        goto L_4c67;

L_4c67:
    game.wCrap = ((game.wCrap & 0xff7f) | ((LOWORD(rgl[0x3]) & 0x1) << 0x7));

L_4c8b:
    if ((cNum < 0x5))
        goto L_4cba;
    else
        goto L_4c94;

L_4c94:
    game.wCrap = ((game.wCrap & 0xffef) | ((LOWORD(rgl[0x4]) & 0x1) << 0x4));

L_4cba:
    if ((cNum < 0x6))
        goto L_4ce6;
    else
        goto L_4cc3;

L_4cc3:
    game.wCrap = ((game.wCrap & 0xffbf) | ((LOWORD(rgl[0x5]) & 0x1) << 0x6));

L_4ce6:
    if ((cNum < 0x7))
        goto L_4d13;
    else
        goto L_4cef;

L_4cef:
    game.wCrap = ((game.wCrap & 0xfeff) | ((LOWORD(rgl[0x6]) & 0x1) << 0x8));

L_4d13:
    lpbStart = PszGetLine(&(lpb));
    cNum = CParseNumbers(lpbStart, rgl, 0x1);
    if ((cNum < 0x1))
        goto L_4d75;
    else
        goto L_4d47;

L_4d47:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto L_4d75;
    else
        goto L_4d50;

L_4d50:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto L_4d5e;
    else
        goto L_4d55;

L_4d55:
    if ((LOWORD(rgl[0x0]) < 0x1))
        goto L_4d75;
    else
        goto L_4d5e;

L_4d5e:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto L_4d99;
    else
        goto L_4d67;

L_4d67:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto L_4d75;
    else
        goto L_4d6c;

L_4d6c:
    if ((LOWORD(rgl[0x0]) <= 0x10))
        goto L_4d99;
    else
        goto L_4d75;

L_4d75:
    AlertSz(PszFormatIds(idsLine4HasImproperNumberPlayerFiles, 0x0), MB_ICONHAND);
    goto LError;

L_4d99:
    if ((LOWORD(lpb) >= LOWORD(lpbDefMac)))
        goto LUniDefShort;
    else
        goto L_4da5;

L_4da5:

L_4dab:
    cPlr = LOWORD(rgl[0x0]);
    game.cPlayer = cPlr;
    i = 0x0;
    goto L_501b;

L_4dc2:
    lpbStart = PszGetLine(&(lpb));
    if ((LOWORD(lpb) >= LOWORD(lpbDefMac)))
        goto LUniDefShort;
    else
        goto L_4de0;

L_4de0:

L_4de6:
    if ((i <= 0x0))
        goto L_4f87;
    else
        goto L_4def;

L_4def:
    if (((uint16_t)(*(lpbStart)) != 0x23))
        goto L_4f87;
    else
        goto L_4dfe;

L_4dfe:
    cNum = CParseNumbers(&(lpbStart[0x1]), rgl, 0x2);
    idAi = LOWORD(rgl[0x0]);
    lvlAi = LOWORD(rgl[0x1]);
    if ((cNum < 0x2))
        goto L_4e5d;
    else
        goto L_4e39;

L_4e39:
    if ((idAi < 0x0))
        goto L_4e5d;
    else
        goto L_4e42;

L_4e42:
    if ((idAi > 0x6))
        goto L_4e5d;
    else
        goto L_4e4b;

L_4e4b:
    if ((lvlAi < 0x0))
        goto L_4e5d;
    else
        goto L_4e54;

L_4e54:
    if ((lvlAi <= 0x4))
        goto L_4e75;
    else
        goto L_4e5d;

L_4e5d:
    fstrcpy(szWork, lpbStart);
    goto LCantGetRace;

L_4e75:
    if ((lvlAi != 0x0))
        goto L_4e90;
    else
        goto L_4e7e;

L_4e7e:
    lvlAi = Random(0x4);
    goto L_4e94;

L_4e90:
    lvlAi = (lvlAi - 0x1);

L_4e94:
    if ((idAi != 0x0))
        goto L_4eaf;
    else
        goto L_4e9d;

L_4e9d:
    idAi = Random(0x6);
    goto L_4eb3;

L_4eaf:
    idAi = (idAi - 0x1);

L_4eb3:
    rgplr[i] = *(LpplrComp(idAi, lvlAi));
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfdff) | 0x200);
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0x1fff) | ((idAi & 0x7) * 0x2000));
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xe3ff) | ((lvlAi & 0x7) * 0x400));
    goto L_5017;

L_4f87:
    fstrcpy(szWork, lpbStart);
    if ((FWasRaceFile(szWork, 0x0) != 0x0))
        goto L_4ff3;
    else
        goto LCantGetRace;

LCantGetRace:
    _wsprintf(szWork, PszGetCompressedString(idsLineDUnableLoadRaceFileS), (i + 0x5), LOWORD(lpbStart), HIWORD(lpbStart));
    AlertSz(szWork, MB_ICONHAND);
    goto LError;

L_4ff3:
    rgplr[i] = vplr;

L_5017:
    i = (i + 0x1);

L_501b:
    if ((i < cPlr))
        goto L_4dc2;
    else
        goto L_5026;

L_5026:
    lpbStart = PszGetLine(&(lpb));
    cNum = CParseNumbers(lpbStart, rgl, 0x2);
    if ((LOWORD(lpb) >= LOWORD(lpbDefMac)))
        goto LUniDefShort;
    else
        goto L_505d;

L_505d:

L_5063:
    i = 0x0;
    if ((cNum < 0x1))
        goto LBadDefVc;
    else
        goto L_5071;

L_5071:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_507a;

L_507a:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto L_5088;
    else
        goto L_507f;

L_507f:
    if ((LOWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5088;

L_5088:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto L_50dd;
    else
        goto L_5091;

L_5091:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto LBadDefVc;
    else
        goto L_5096;

L_5096:
    if ((LOWORD(rgl[0x0]) <= 0x1))
        goto L_50dd;
    else
        goto LBadDefVc;

LBadDefVc:
    i = (i + (cPlr + 0x5));
    _wsprintf(szWork, PszGetCompressedString(idsLineDHasImproperVictoryConditionDefinition), i);
    AlertSz(szWork, MB_ICONHAND);
    goto LError;

L_50dd:
    if ((LOWORD(rgl[0x0]) != 0x1))
        goto L_5160;
    else
        goto L_50e6;

L_50e6:
    if ((HIWORD(rgl[0x0]) != 0x0))
        goto L_5160;
    else
        goto L_50ef;

L_50ef:
    if ((cNum < 0x2))
        goto LBadDefVc;
    else
        goto L_50f8;

L_50f8:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5101;

L_5101:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto L_510f;
    else
        goto L_5106;

L_5106:
    if ((LOWORD(rgl[0x1]) < 0x14))
        goto LBadDefVc;
    else
        goto L_510f;

L_510f:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto L_5129;
    else
        goto L_5118;

L_5118:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto LBadDefVc;
    else
        goto L_511d;

L_511d:
    if ((LOWORD(rgl[0x1]) > 0x64))
        goto LBadDefVc;
    else
        goto L_5123;

L_5123:

L_5129:
    SetVCCheck(&(game), 0x0, 0x1);
    SetVCVal(&(game), 0x0, ((uint32_t)((LOWORD(rgl[0x1]) + 0xffec)) / 0x5));

L_5160:
    lpbStart = PszGetLine(&(lpb));
    cNum = CParseNumbers(lpbStart, rgl, 0x3);
    if ((LOWORD(lpb) >= LOWORD(lpbDefMac)))
        goto LUniDefShort;
    else
        goto L_5197;

L_5197:

L_519d:
    i = 0x1;
    if ((cNum < 0x1))
        goto LBadDefVc;
    else
        goto L_51ab;

L_51ab:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_51b4;

L_51b4:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto L_51c2;
    else
        goto L_51b9;

L_51b9:
    if ((LOWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_51c2;

L_51c2:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto L_51dc;
    else
        goto L_51cb;

L_51cb:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto LBadDefVc;
    else
        goto L_51d0;

L_51d0:
    if ((LOWORD(rgl[0x0]) > 0x1))
        goto LBadDefVc;
    else
        goto L_51d6;

L_51d6:

L_51dc:
    if ((LOWORD(rgl[0x0]) != 0x1))
        goto L_52b2;
    else
        goto L_51e5;

L_51e5:
    if ((HIWORD(rgl[0x0]) != 0x0))
        goto L_52b2;
    else
        goto L_51ee;

L_51ee:
    if ((cNum < 0x3))
        goto LBadDefVc;
    else
        goto L_51f7;

L_51f7:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5200;

L_5200:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto L_520e;
    else
        goto L_5205;

L_5205:
    if ((LOWORD(rgl[0x1]) < 0x8))
        goto LBadDefVc;
    else
        goto L_520e;

L_520e:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto LBadDefVc;
    else
        goto L_5217;

L_5217:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto L_5225;
    else
        goto L_521c;

L_521c:
    if ((LOWORD(rgl[0x1]) > 0x1a))
        goto LBadDefVc;
    else
        goto L_5225;

L_5225:
    if ((HIWORD(rgl[0x2]) < 0x0))
        goto LBadDefVc;
    else
        goto L_522e;

L_522e:
    if ((HIWORD(rgl[0x2]) > 0x0))
        goto L_523c;
    else
        goto L_5233;

L_5233:
    if ((LOWORD(rgl[0x2]) < 0x2))
        goto LBadDefVc;
    else
        goto L_523c;

L_523c:
    if ((HIWORD(rgl[0x2]) < 0x0))
        goto L_5256;
    else
        goto L_5245;

L_5245:
    if ((HIWORD(rgl[0x2]) > 0x0))
        goto LBadDefVc;
    else
        goto L_524a;

L_524a:
    if ((LOWORD(rgl[0x2]) > 0x6))
        goto LBadDefVc;
    else
        goto L_5250;

L_5250:

L_5256:
    SetVCCheck(&(game), 0x1, 0x1);
    SetVCCheck(&(game), 0x2, 0x1);
    SetVCVal(&(game), 0x1, (LOWORD(rgl[0x1]) + 0xfff8));
    SetVCVal(&(game), 0x2, (LOWORD(rgl[0x2]) + 0xfffe));

L_52b2:
    lpbStart = PszGetLine(&(lpb));
    cNum = CParseNumbers(lpbStart, rgl, 0x2);
    if ((LOWORD(lpb) >= LOWORD(lpbDefMac)))
        goto LUniDefShort;
    else
        goto L_52e9;

L_52e9:

L_52ef:
    i = 0x2;
    if ((cNum < 0x1))
        goto LBadDefVc;
    else
        goto L_52fd;

L_52fd:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5306;

L_5306:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto L_5314;
    else
        goto L_530b;

L_530b:
    if ((LOWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5314;

L_5314:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto L_532e;
    else
        goto L_531d;

L_531d:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto LBadDefVc;
    else
        goto L_5322;

L_5322:
    if ((LOWORD(rgl[0x0]) > 0x1))
        goto LBadDefVc;
    else
        goto L_5328;

L_5328:

L_532e:
    if ((LOWORD(rgl[0x0]) != 0x1))
        goto L_53b3;
    else
        goto L_5337;

L_5337:
    if ((HIWORD(rgl[0x0]) != 0x0))
        goto L_53b3;
    else
        goto L_5340;

L_5340:
    if ((cNum < 0x2))
        goto LBadDefVc;
    else
        goto L_5349;

L_5349:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5352;

L_5352:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto L_5361;
    else
        goto L_5357;

L_5357:
    if ((LOWORD(rgl[0x1]) < 0x3e8))
        goto LBadDefVc;
    else
        goto L_5361;

L_5361:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto L_537c;
    else
        goto L_536a;

L_536a:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto LBadDefVc;
    else
        goto L_536f;

L_536f:
    if ((LOWORD(rgl[0x1]) > 0x4e20))
        goto LBadDefVc;
    else
        goto L_5376;

L_5376:

L_537c:
    SetVCCheck(&(game), 0x3, 0x1);
    SetVCVal(&(game), 0x3, ((uint32_t)((LOWORD(rgl[0x1]) + 0xfc18)) / 0x3e8));

L_53b3:
    lpbStart = PszGetLine(&(lpb));
    cNum = CParseNumbers(lpbStart, rgl, 0x2);
    if ((LOWORD(lpb) >= LOWORD(lpbDefMac)))
        goto LUniDefShort;
    else
        goto L_53ea;

L_53ea:

L_53f0:
    i = 0x3;
    if ((cNum < 0x1))
        goto LBadDefVc;
    else
        goto L_53fe;

L_53fe:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5407;

L_5407:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto L_5415;
    else
        goto L_540c;

L_540c:
    if ((LOWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5415;

L_5415:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto L_542f;
    else
        goto L_541e;

L_541e:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto LBadDefVc;
    else
        goto L_5423;

L_5423:
    if ((LOWORD(rgl[0x0]) > 0x1))
        goto LBadDefVc;
    else
        goto L_5429;

L_5429:

L_542f:
    if ((LOWORD(rgl[0x0]) != 0x1))
        goto L_54b3;
    else
        goto L_5438;

L_5438:
    if ((HIWORD(rgl[0x0]) != 0x0))
        goto L_54b3;
    else
        goto L_5441;

L_5441:
    if ((cNum < 0x2))
        goto LBadDefVc;
    else
        goto L_544a;

L_544a:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5453;

L_5453:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto L_5461;
    else
        goto L_5458;

L_5458:
    if ((LOWORD(rgl[0x1]) < 0x14))
        goto LBadDefVc;
    else
        goto L_5461;

L_5461:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto L_547c;
    else
        goto L_546a;

L_546a:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto LBadDefVc;
    else
        goto L_546f;

L_546f:
    if ((LOWORD(rgl[0x1]) > 0x12c))
        goto LBadDefVc;
    else
        goto L_5476;

L_5476:

L_547c:
    SetVCCheck(&(game), 0x4, 0x1);
    SetVCVal(&(game), 0x4, ((uint32_t)((LOWORD(rgl[0x1]) + 0xffec)) / 0xa));

L_54b3:
    lpbStart = PszGetLine(&(lpb));
    cNum = CParseNumbers(lpbStart, rgl, 0x2);
    if ((LOWORD(lpb) >= LOWORD(lpbDefMac)))
        goto LUniDefShort;
    else
        goto L_54ea;

L_54ea:

L_54f0:
    i = 0x4;
    if ((cNum < 0x1))
        goto LBadDefVc;
    else
        goto L_54fe;

L_54fe:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5507;

L_5507:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto L_5515;
    else
        goto L_550c;

L_550c:
    if ((LOWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5515;

L_5515:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto L_552f;
    else
        goto L_551e;

L_551e:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto LBadDefVc;
    else
        goto L_5523;

L_5523:
    if ((LOWORD(rgl[0x0]) > 0x1))
        goto LBadDefVc;
    else
        goto L_5529;

L_5529:

L_552f:
    if ((LOWORD(rgl[0x0]) != 0x1))
        goto L_55b3;
    else
        goto L_5538;

L_5538:
    if ((HIWORD(rgl[0x0]) != 0x0))
        goto L_55b3;
    else
        goto L_5541;

L_5541:
    if ((cNum < 0x2))
        goto LBadDefVc;
    else
        goto L_554a;

L_554a:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5553;

L_5553:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto L_5561;
    else
        goto L_5558;

L_5558:
    if ((LOWORD(rgl[0x1]) < 0xa))
        goto LBadDefVc;
    else
        goto L_5561;

L_5561:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto L_557c;
    else
        goto L_556a;

L_556a:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto LBadDefVc;
    else
        goto L_556f;

L_556f:
    if ((LOWORD(rgl[0x1]) > 0x1f4))
        goto LBadDefVc;
    else
        goto L_5576;

L_5576:

L_557c:
    SetVCCheck(&(game), 0x5, 0x1);
    SetVCVal(&(game), 0x5, ((uint32_t)((LOWORD(rgl[0x1]) + 0xfff6)) / 0xa));

L_55b3:
    lpbStart = PszGetLine(&(lpb));
    cNum = CParseNumbers(lpbStart, rgl, 0x2);
    if ((LOWORD(lpb) >= LOWORD(lpbDefMac)))
        goto LUniDefShort;
    else
        goto L_55ea;

L_55ea:

L_55f0:
    i = 0x5;
    if ((cNum < 0x1))
        goto LBadDefVc;
    else
        goto L_55fe;

L_55fe:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5607;

L_5607:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto L_5615;
    else
        goto L_560c;

L_560c:
    if ((LOWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5615;

L_5615:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto L_562f;
    else
        goto L_561e;

L_561e:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto LBadDefVc;
    else
        goto L_5623;

L_5623:
    if ((LOWORD(rgl[0x0]) > 0x1))
        goto LBadDefVc;
    else
        goto L_5629;

L_5629:

L_562f:
    if ((LOWORD(rgl[0x0]) != 0x1))
        goto L_56b3;
    else
        goto L_5638;

L_5638:
    if ((HIWORD(rgl[0x0]) != 0x0))
        goto L_56b3;
    else
        goto L_5641;

L_5641:
    if ((cNum < 0x2))
        goto LBadDefVc;
    else
        goto L_564a;

L_564a:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5653;

L_5653:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto L_5661;
    else
        goto L_5658;

L_5658:
    if ((LOWORD(rgl[0x1]) < 0xa))
        goto LBadDefVc;
    else
        goto L_5661;

L_5661:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto L_567c;
    else
        goto L_566a;

L_566a:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto LBadDefVc;
    else
        goto L_566f;

L_566f:
    if ((LOWORD(rgl[0x1]) > 0x12c))
        goto LBadDefVc;
    else
        goto L_5676;

L_5676:

L_567c:
    SetVCCheck(&(game), 0x6, 0x1);
    SetVCVal(&(game), 0x6, ((uint32_t)((LOWORD(rgl[0x1]) + 0xfff6)) / 0xa));

L_56b3:
    lpbStart = PszGetLine(&(lpb));
    cNum = CParseNumbers(lpbStart, rgl, 0x2);
    if ((LOWORD(lpb) >= LOWORD(lpbDefMac)))
        goto LUniDefShort;
    else
        goto L_56ea;

L_56ea:

L_56f0:
    i = 0x6;
    if ((cNum < 0x1))
        goto LBadDefVc;
    else
        goto L_56fe;

L_56fe:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5707;

L_5707:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto L_5715;
    else
        goto L_570c;

L_570c:
    if ((LOWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5715;

L_5715:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto L_572f;
    else
        goto L_571e;

L_571e:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto LBadDefVc;
    else
        goto L_5723;

L_5723:
    if ((LOWORD(rgl[0x0]) > 0x1))
        goto LBadDefVc;
    else
        goto L_5729;

L_5729:

L_572f:
    if ((LOWORD(rgl[0x0]) != 0x1))
        goto L_57b3;
    else
        goto L_5738;

L_5738:
    if ((HIWORD(rgl[0x0]) != 0x0))
        goto L_57b3;
    else
        goto L_5741;

L_5741:
    if ((cNum < 0x2))
        goto LBadDefVc;
    else
        goto L_574a;

L_574a:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5753;

L_5753:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto L_5761;
    else
        goto L_5758;

L_5758:
    if ((LOWORD(rgl[0x1]) < 0x1e))
        goto LBadDefVc;
    else
        goto L_5761;

L_5761:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto L_577c;
    else
        goto L_576a;

L_576a:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto LBadDefVc;
    else
        goto L_576f;

L_576f:
    if ((LOWORD(rgl[0x1]) > 0x384))
        goto LBadDefVc;
    else
        goto L_5776;

L_5776:

L_577c:
    SetVCCheck(&(game), 0x7, 0x1);
    SetVCVal(&(game), 0x7, ((uint32_t)((LOWORD(rgl[0x1]) + 0xffe2)) / 0xa));

L_57b3:
    lpbStart = PszGetLine(&(lpb));
    cNum = CParseNumbers(lpbStart, rgl, 0x2);
    if ((LOWORD(lpb) >= LOWORD(lpbDefMac)))
        goto LUniDefShort;
    else
        goto L_57ea;

L_57ea:

L_57f0:
    i = 0x7;
    if ((cNum < 0x1))
        goto LBadDefVc;
    else
        goto L_57fe;

L_57fe:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5807;

L_5807:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto L_5815;
    else
        goto L_580c;

L_580c:
    if ((LOWORD(rgl[0x0]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5815;

L_5815:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto L_582f;
    else
        goto L_581e;

L_581e:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto LBadDefVc;
    else
        goto L_5823;

L_5823:
    if ((LOWORD(rgl[0x0]) > 0x7))
        goto LBadDefVc;
    else
        goto L_5829;

L_5829:

L_582f:
    if ((HIWORD(rgl[0x0]) < 0x0))
        goto L_58bb;
    else
        goto L_5838;

L_5838:
    if ((HIWORD(rgl[0x0]) > 0x0))
        goto L_5846;
    else
        goto L_583d;

L_583d:
    if ((LOWORD(rgl[0x0]) <= 0x0))
        goto L_58bb;
    else
        goto L_5846;

L_5846:
    if ((cNum < 0x2))
        goto LBadDefVc;
    else
        goto L_584f;

L_584f:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto LBadDefVc;
    else
        goto L_5858;

L_5858:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto L_5866;
    else
        goto L_585d;

L_585d:
    if ((LOWORD(rgl[0x1]) < 0x1e))
        goto LBadDefVc;
    else
        goto L_5866;

L_5866:
    if ((HIWORD(rgl[0x1]) < 0x0))
        goto L_5881;
    else
        goto L_586f;

L_586f:
    if ((HIWORD(rgl[0x1]) > 0x0))
        goto LBadDefVc;
    else
        goto L_5874;

L_5874:
    if ((LOWORD(rgl[0x1]) > 0x1f4))
        goto LBadDefVc;
    else
        goto L_587b;

L_587b:

L_5881:
    SetVCVal(&(game), 0x8, LOWORD(rgl[0x0]));
    SetVCVal(&(game), 0x9, ((uint32_t)((LOWORD(rgl[0x1]) + 0xffe2)) / 0xa));

L_58bb:
    lpbStart = PszGetLine(&(lpb));
    lpb = ((uint8_t *)(lpbStart) + (fstrlen(lpbStart) + 0xffff));
    if (((LOWORD(lpb) - LOWORD(lpbStart)) < 0x3))
        goto L_593b;
    else
        goto L_5904;

L_5904:
    if (((uint16_t)(*(lpb)) != 0x79))
        goto L_593b;
    else
        goto L_5913;

L_5913:
    if (((uint16_t)(*(lpb - 0x1)) != 0x78))
        goto L_593b;
    else
        goto L_5923;

L_5923:
    if (((uint16_t)(*(lpb - 0x2)) != 0x2e))
        goto L_593b;
    else
        goto L_5933;

L_5933:
    *(lpb - 0x2) = 0x0;

L_593b:
    fstrcpy(szBase, lpbStart);
    if (((LOWORD(lpb) + 0x4) >= LOWORD(lpbDefMac)))
        goto L_5971;
    else
        goto L_5964;

L_5964:
    lpbDefUni = lpb;

L_5971:
    i = 0x0;
    goto L_597d;

L_5979:
    i = (i + 0x1);

L_597d:
    if ((i >= game.cPlayer))
        goto L_5a89;
    else
        goto L_5988;

L_5988:
    if ((((rgplr[i].wMdPlr >> 0x9) & 0x1) != 0x0))
        goto L_5a0e;
    else
        goto L_59a6;

L_59a6:
    if ((CAdvantagePoints(rgplr[i]) >= 0x0))
        goto L_5a0e;
    else
        goto L_59c2;

L_59c2:
    rgplr[i] = vrgplrDef[0x0];
    rgplr[i].wFlags = ((rgplr[i].wFlags & 0xffef) | 0x10);

L_5a0e:
    if (((uint16_t)(rgplr[i].szName[0x0]) != 0x0))
        goto L_5979;
    else
        goto L_5a26;

L_5a26:
    CchGetString((Random(0x18) + 0x56e), rgplr[i].szName);
    _wsprintf(rgplr[i].szNames, "%ss", rgplr[i].szName);

L_5a89:
    i = 0x1;
    goto L_5a95;

L_5a91:
    i = (i + 0x1);

L_5a95:
    if ((i >= game.cPlayer))
        goto L_5be4;
    else
        goto L_5aa0;

L_5aa0:
    j = 0x0;
    goto L_5ae1;

L_5aa8:
    if ((strcmp(rgplr[i].szName, rgplr[j].szName) == 0x0))
        goto L_5aec;
    else
        goto L_5ad7;

L_5ad7:

L_5add:
    j = (j + 0x1);

L_5ae1:
    if ((j < i))
        goto L_5aa8;
    else
        goto L_5aec;

L_5aec:
    if ((j >= i))
        goto L_5a91;
    else
        goto L_5af7;

L_5af7:
    c = Random(0x18);

L_5b09:
    j = 0x0;
    goto L_5b15;

L_5b11:
    j = (j + 0x1);

L_5b15:
    if ((j >= game.cPlayer))
        goto L_5b57;
    else
        goto L_5b20;

L_5b20:
    if ((strcmp(rgplr[j].szName, PszGetCompressedString((c + 0x56e))) == 0x0))
        goto L_5b57;
    else
        goto L_5b4e;

L_5b4e:

L_5b57:
    if ((j == game.cPlayer))
        goto L_5b7a;
    else
        goto L_5b5f;

L_5b5f:

L_5b65:
    c = (c + 0x1);
    if ((c < 0x18))
        goto L_5b09;
    else
        goto L_5b72;

L_5b72:
    c = 0x0;

L_5b7a:
    CchGetString((c + 0x56e), rgplr[i].szName);
    strcpy(rgplr[i].szNames, rgplr[i].szName);
    strcat(rgplr[i].szNames, "s");

L_5be4:
    i = 0x0;
    goto L_5bf0;

L_5bec:
    i = (i + 0x1);

L_5bf0:
    if ((i >= game.cPlayer))
        goto L_5c53;
    else
        goto L_5bfb;

L_5bfb:
    rgplrbmp[i] = ((rgplr[i].wMdPlr >> 0x3) & 0x1f);
    if ((rgplrbmp[i] < 0x0))
        goto L_5c42;
    else
        goto L_5c30;

L_5c30:
    if ((rgplrbmp[i] < 0x20))
        goto L_5bec;
    else
        goto L_5c42;

L_5c42:
    rgplrbmp[i] = 0xffff;

L_5c53:
    i = 0x1;
    goto L_5c5f;

L_5c5b:
    i = (i + 0x1);

L_5c5f:
    if ((i >= game.cPlayer))
        goto L_5cec;
    else
        goto L_5c6a;

L_5c6a:
    if ((rgplrbmp[i] == 0xffff))
        goto L_5c5b;
    else
        goto L_5c79;

L_5c79:

L_5c7f:
    j = 0x0;
    goto L_5cab;

L_5c87:
    if ((rgplrbmp[j] == rgplrbmp[i]))
        goto L_5cb6;
    else
        goto L_5ca1;

L_5ca1:

L_5ca7:
    j = (j + 0x1);

L_5cab:
    if ((j < i))
        goto L_5c87;
    else
        goto L_5cb6;

L_5cb6:
    if ((j >= i))
        goto L_5c5b;
    else
        goto L_5cc1;

L_5cc1:
    if ((Random(0x2) == 0x0))
        goto L_5cdb;
    else
        goto L_5cd5;

L_5cd5:
    t_merge_5cde_0001 = i;
    goto L_5cde;

L_5cdb:
    t_merge_5cde_0001 = j;

L_5cde:
    rgplrbmp[t_merge_5cde_0001] = 0xffff;

L_5cec:
    i = 0x0;
    goto L_5cf8;

L_5cf4:
    i = (i + 0x1);

L_5cf8:
    if ((i >= game.cPlayer))
        goto L_5db9;
    else
        goto L_5d03;

L_5d03:
    if ((rgplrbmp[i] != 0xffff))
        goto L_5cf4;
    else
        goto L_5d12;

L_5d12:

L_5d18:
    rgplrbmp[i] = Random(0x20);

L_5d33:
    j = 0x0;
    goto L_5d3f;

L_5d3b:
    j = (j + 0x1);

L_5d3f:
    if ((j >= game.cPlayer))
        goto L_5d78;
    else
        goto L_5d4a;

L_5d4a:
    if ((j == i))
        goto L_5d3b;
    else
        goto L_5d55;

L_5d55:
    if ((rgplrbmp[i] == rgplrbmp[j]))
        goto L_5d78;
    else
        goto L_5d6f;

L_5d6f:

L_5d78:
    if ((j == game.cPlayer))
        goto L_5cf4;
    else
        goto L_5d80;

L_5d80:

L_5d86:
    rgplrbmp[i] = (rgplrbmp[i] + 0x1);
    if ((rgplrbmp[i] < 0x20))
        goto L_5d33;
    else
        goto L_5da5;

L_5da5:
    rgplrbmp[i] = 0x0;

L_5db9:
    i = 0x0;
    goto L_5dc5;

L_5dc1:
    i = (i + 0x1);

L_5dc5:
    if ((i >= game.cPlayer))
        goto L_5e1b;
    else
        goto L_5dd0;

L_5dd0:
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xff07) | ((rgplrbmp[i] & 0x1f) * 0x8));
    goto L_5dc1;

L_5e1b:
    GenerateWorld(0x1);
    fSuccess = 0x1;

LError:
    penvMem = 0x0;
    StreamClose();
    lpbDefUni = 0x0;
    TurnLog((fSuccess + 0x564));

L_5e58:
    return fSuccess;
}

void CreateTutorWorld() {
    int16_t i;

L_5e5e:
    memset(&(game), 0x0, 0x40);
    game.cPlayer = 0x2;
    game.fTutorial = 0x1;
    game.mdDensity = 0x0;
    game.mdSize = 0x0;
    game.mdStartDist = 0x1;
    game.fBBSPlay = 0x1;
    game.fVisScores = 0x1;
    game.fNoRandom = 0x1;
    game.lid = 0x8cef49;
    game.rgvc[0x7] = 0x80;
    game.rgvc[0x8] = 0x81;
    CchGetString(idsTutorialGame, game.szName);
    rgplr[0x0] = vrgplrDef[0x0];
    /* untranslated: call CchGetString(idsHumanoid, &part[128:0](rgplr)) -> callresult(int16_t) */
    /* untranslated: call _wsprintf("", "%ss", &part[128:0](rgplr)) -> callresult(int16_t) */
    rgplr[0x1] = *(LpplrComp(0x1, 0x0));
    rgplr[0x1].wMdPlr = ((rgplr[0x1].wMdPlr & 0xfdff) | 0x200);
    rgplr[0x1].wMdPlr = ((rgplr[0x1].wMdPlr & 0xe3ff) | 0x0);
    rgplr[0x1].wMdPlr = ((rgplr[0x1].wMdPlr & 0x1fff) | 0x2000);
    /* untranslated: call CchGetString(idsBerserker, &part[320:0](rgplr)) -> callresult(int16_t) */
    Randomize(0x499602d2);
    i = 0x1;
    goto L_6007;

L_5f99:
    _wsprintf(szWork, PszGetCompressedString(idsSHD), &(szBase), i);
    remove(szWork);
    _wsprintf(szWork, PszGetCompressedString(idsSXD), &(szBase), i);
    remove(szWork);
    i = (i + 0x1);

L_6007:
    if ((i <= 0x2))
        goto L_5f99;
    else
        goto L_6010;

L_6010:
    GenerateWorld(0x0);
    return;
}

void NewGameWizard(HWND hwnd, int16_t fReadOnly) {
    int16_t iStepMaxSoFar;
    int16_t mdRet;
    int16_t (**lpProc)();
    int16_t  fIdleSav;
    int16_t  rgplrbmp[16];
    int16_t  i;
    int16_t  c;
    char     szFile[256];
    int16_t  idAi;
    int16_t  fEasy;
    char     szFileLocal[208];
    int16_t  j;
    RECT     rgrcStack[20];
    PLAYER   rgplrLocal[16];
    int16_t  lvlAi;
    GAME     gameT;
    uint16_t t_merge_62b9_0001;
    uint16_t t_merge_62ff_0001;
    int16_t  t_merge_6c95_0001;

L_6022:
    iStepMaxSoFar = 0x0;
    fEasy = 0x0;
    vrgrcRCW = rgrcStack;
    iPanelActive = 0xffff;
    fRCWReadOnly = fReadOnly;
    fIdleSav = gd.fNoIdleChecks;
    gd.fNoIdleChecks = 0x1;
    vrgplrNew = rgplrLocal;
    vrgszFileNew = szFileLocal;
    vcplrNew = 0x0;
    memset(&(vrgplrTypeNew), 0x0, 0x10);
    if ((fReadOnly == 0x0))
        goto L_612a;
    else
        goto L_6094;

L_6094:
    i = 0x0;
    goto L_60a0;

L_609c:
    i = (i + 0x1);

L_60a0:
    if ((i >= game.cPlayer))
        goto Step1;
    else
        goto L_60ab;

L_60ab:
    if ((((rgplr[i].wMdPlr >> 0x8) & 0x1) == 0x0))
        goto L_611c;
    else
        goto L_60c9;

L_60c9:
    vrgplrTypeNew[i] = LOBYTE(((i << 0x2) | 0x2));
    vrgplrNew[i] = rgplr[i];
    /* untranslated: byte ds:[(vrgszFileNew + loword((0xd * i)))] = 0x0 */
    goto L_609c;

L_611c:
    vrgplrTypeNew[i] = 0x19;

L_612a:
    gameT = game;
    memset(&(game), 0x0, 0x40);
    game.cPlanMax = gameT.cPlanMax;
    game.cPlayer = gameT.cPlayer;
    vrgplrTypeNew[0x0] = 0x1;
    vrgplrTypeNew[0x1] = 0x23;
    vrgplrTypeNew[0x2] = 0x27;
    vrgplrTypeNew[0x3] = 0x8b;
    lpProc = MakeProcInstance(SimpleNewGameDlg, hInst);
    mdRet = DialogBox(hInst, IDD_SIMPLE_NEW_GAME, hwnd, &(lpProc));
    FreeProcInstance(&(lpProc));
    game.rgvc[0x0] = 0x88;
    game.rgvc[0x1] = 0x8e;
    game.rgvc[0x2] = 0x82;
    game.rgvc[0x3] = 0xa;
    game.rgvc[0x4] = 0x88;
    game.rgvc[0x5] = 0x9;
    game.rgvc[0x6] = 0x9;
    game.rgvc[0x7] = 0x7;
    game.rgvc[0x8] = 0x1;
    if ((mdRet == 0x2))
        goto Cancel;
    else
        goto L_61e4;

L_61e4:

L_61ed:
    if ((mdRet != 0xd4))
        goto L_6233;
    else
        goto L_61f7;

L_61f7:
    game = gameT;
    StartTutor(0x0);
    gd.fNoIdleChecks = fIdleSav;
    goto L_6e3e;

L_6233:
    if ((mdRet == 0x430))
        goto L_6247;
    else
        goto L_623d;

L_623d:
    if ((mdRet != 0xd3))
        goto Step1;
    else
        goto L_6247;

L_6247:
    memset(&(vrgplrTypeNew), 0x0, 0x10);
    if ((game.turn >= 0x7))
        goto L_6278;
    else
        goto L_6265;

L_6265:
    LOWORD(vrgplrTypeNew) = LOBYTE((((game.turn & 0xff) << 0x2) | 0x1));
    goto L_628a;

L_6278:
    vrgplrTypeNew[0x0] = 0x2;
    *(vrgszFileNew) = 0x0;
    vcplrNew = 0x1;

L_628a:
    lvlAi = game.mdDensity;
    InitNewGamePlr(iStepMaxSoFar, lvlAi);
    game.mdDensity = 0x1;
    if ((lvlAi < 0x2))
        goto L_62b6;
    else
        goto L_62b0;

L_62b0:
    t_merge_62b9_0001 = 0x2;
    goto L_62b9;

L_62b6:
    t_merge_62b9_0001 = 0x1;

L_62b9:
    game.mdStartDist = t_merge_62b9_0001;
    game.fExtraFuel = 0x0;
    game.fSlowTech = 0x0;
    game.fBBSPlay = 0x0;
    game.fNoRandom = 0x0;
    if ((lvlAi != 0x3))
        goto L_62fc;
    else
        goto L_62f6;

L_62f6:
    t_merge_62ff_0001 = 0x1;
    goto L_62ff;

L_62fc:
    t_merge_62ff_0001 = 0x0;

L_62ff:
    game.wCrap = ((game.wCrap & 0xffef) | ((t_merge_62ff_0001 & 0x1) << 0x4));
    game.fVisScores = 0x0;
    CchGetString(((LOWORD((0x5 * lvlAi)) + 0x1cb) + game.mdSize), game.szName);
    if ((mdRet != 0x430))
        goto Step1;
    else
        goto L_6352;

L_6352:
    fEasy = 0x1;
    goto Finish;

Step1:
    lpProc = MakeProcInstance(NewGameDlg, hInst);
    mdRet = DialogBox(hInst, IDD_NEW_GAME_1, hwnd, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((mdRet != 0x0))
        goto L_63da;
    else
        goto Cancel;

Cancel:
    if ((fRCWReadOnly != 0x0))
        goto L_63be;
    else
        goto L_63ad;

L_63ad:
    game = gameT;

L_63be:
    gd.fNoIdleChecks = fIdleSav;
    goto L_6e3e;

L_63da:
    InitNewGamePlr(iStepMaxSoFar, lvlAi);
    if ((mdRet == 0x3))
        goto Finish;
    else
        goto L_63ef;

L_63ef:

Step2:
    lpProc = MakeProcInstance(NewGameDlg2, hInst);
    mdRet = DialogBox(hInst, IDD_NEW_GAME_2, hwnd, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((mdRet == 0x0))
        goto Cancel;
    else
        goto L_643a;

L_643a:

L_6440:
    if ((iStepMaxSoFar >= 0x1))
        goto L_644e;
    else
        goto L_6449;

L_6449:
    iStepMaxSoFar = 0x1;

L_644e:
    InitNewGamePlr(iStepMaxSoFar, lvlAi);
    if ((mdRet == 0x1))
        goto Step1;
    else
        goto L_6463;

L_6463:

L_6469:
    if ((mdRet == 0x3))
        goto Finish;
    else
        goto L_646f;

L_646f:

L_6475:
    lpProc = MakeProcInstance(NewGameDlg3, hInst);
    mdRet = DialogBox(hInst, IDD_NEW_GAME_3, hwnd, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((mdRet == 0x0))
        goto Cancel;
    else
        goto L_64ba;

L_64ba:

L_64c0:
    if ((iStepMaxSoFar >= 0x2))
        goto L_64ce;
    else
        goto L_64c9;

L_64c9:
    iStepMaxSoFar = 0x2;

L_64ce:
    if ((mdRet == 0x1))
        goto Step2;
    else
        goto L_64d4;

L_64d4:

L_64da:
    if ((mdRet == 0x3))
        goto Finish;
    else
        goto Finish;

Finish:
    if ((fRCWReadOnly != 0x0))
        goto Cancel;
    else
        goto L_64ed;

L_64ed:

L_64f3:
    CchGetString(idsGameXy, szFile);
    if ((FGetNewGameName(szFile) != 0x0))
        goto L_6529;
    else
        goto L_6519;

L_6519:
    if ((fEasy != 0x0))
        goto Cancel;
    else
        goto L_6520;

L_6520:

L_6529:
    gameT = game;
    gameT.turn = 0x0;
    DestroyCurGame();
    game = gameT;
    strcpy(szBase, szFile);
    /* untranslated: byte ds:[(strlen(szBase) + 0xfffd)+0x56a2] = 0x0 */
    i = 0x0;
    goto L_68d1;

L_6586:
    if ((vrgplrTypeNew[i] == 0x0))
        goto L_68da;
    else
        goto L_6595;

L_6595:

L_659b:
    goto L_68b2;

L_65ab:
    if (((vrgplrTypeNew[i] >> 0x2) <= 0x6))
        goto L_6694;
    else
        goto L_65c1;

L_65c1:
    c = Random(0x7);
    rgplr[i] = vrgplrDef[c];
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfdff) | 0x200);
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xe3ff) | 0x0);
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0x1fff) | 0xe000);
    rgplr[i].lSalt = 0xffffffff;
    goto L_66d3;

L_6694:
    c = (vrgplrTypeNew[i] >> 0x2);
    rgplr[i] = vrgplrDef[c];

L_66d3:
    CchGetString((c + 0x567), rgplr[i].szName);
    _wsprintf(rgplr[i].szNames, "%ss", rgplr[i].szName);
    goto L_68cd;

L_672d:
    rgplr[i] = rgplrLocal[(vrgplrTypeNew[i] >> 0x2)];
    goto L_68cd;

L_676c:
    idAi = ((vrgplrTypeNew[i] >> 0x2) & 0x7);
    lvlAi = (vrgplrTypeNew[i] >> 0x5);
    if ((lvlAi < 0x4))
        goto L_67b3;
    else
        goto L_67a3;

L_67a3:
    lvlAi = Random(0x4);

L_67b3:
    if ((idAi < 0x6))
        goto L_67cd;
    else
        goto L_67bd;

L_67bd:
    idAi = Random(0x6);

L_67cd:
    rgplr[i] = *(LpplrComp(idAi, lvlAi));
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfdff) | 0x200);
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xe3ff) | ((lvlAi & 0x7) * 0x400));
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0x1fff) | ((idAi & 0x7) * 0x2000));
    goto L_68cd;

L_68b2:
    if (((vrgplrTypeNew[i] & 0x3) == 0x1))
        goto L_65ab;
    else
        goto L_68ba;

L_68ba:
    if (((vrgplrTypeNew[i] & 0x3) == 0x2))
        goto L_672d;
    else
        goto L_68c2;

L_68c2:
    if (((vrgplrTypeNew[i] & 0x3) == 0x3))
        goto L_676c;
    else
        goto L_68cd;

L_68cd:
    i = (i + 0x1);

L_68d1:
    if ((i < 0x10))
        goto L_6586;
    else
        goto L_68da;

L_68da:
    game.cPlayer = i;
    i = 0x0;
    goto L_68ec;

L_68e8:
    i = (i + 0x1);

L_68ec:
    if ((i >= game.cPlayer))
        goto L_6a12;
    else
        goto L_68f7;

L_68f7:
    if ((((rgplr[i].wMdPlr >> 0x9) & 0x1) != 0x0))
        goto L_697f;
    else
        goto L_6915;

L_6915:
    if ((CAdvantagePoints(rgplr[i]) >= 0x0))
        goto L_697f;
    else
        goto L_6931;

L_6931:
    rgplr[i] = vrgplrDef[0x0];
    rgplr[i].wFlags = ((rgplr[i].wFlags & 0xffef) | 0x10);

L_697f:
    if (((uint16_t)(rgplr[i].szName[0x0]) != 0x0))
        goto L_69c0;
    else
        goto L_6997;

L_6997:
    CchGetString((Random(0x18) + 0x56e), rgplr[i].szName);

L_69c0:
    if (((uint16_t)(rgplr[i].szNames[0x0]) != 0x0))
        goto L_68e8;
    else
        goto L_69d8;

L_69d8:
    _wsprintf(rgplr[i].szNames, "%ss", rgplr[i].szName);

L_6a12:
    i = 0x1;
    goto L_6a1e;

L_6a1a:
    i = (i + 0x1);

L_6a1e:
    if ((i >= game.cPlayer))
        goto L_6b77;
    else
        goto L_6a29;

L_6a29:
    j = 0x0;
    goto L_6a6d;

L_6a32:
    if ((strcmp(rgplr[i].szName, rgplr[j].szName) == 0x0))
        goto L_6a79;
    else
        goto L_6a62;

L_6a62:

L_6a68:
    j = (j + 0x1);

L_6a6d:
    if ((j < i))
        goto L_6a32;
    else
        goto L_6a79;

L_6a79:
    if ((j >= i))
        goto L_6a1a;
    else
        goto L_6a85;

L_6a85:
    c = Random(0x18);

L_6a97:
    j = 0x0;
    goto L_6aa5;

L_6aa0:
    j = (j + 0x1);

L_6aa5:
    if ((j >= game.cPlayer))
        goto L_6ae9;
    else
        goto L_6ab1;

L_6ab1:
    if ((strcmp(rgplr[j].szName, PszGetCompressedString((c + 0x56e))) == 0x0))
        goto L_6ae9;
    else
        goto L_6ae0;

L_6ae0:

L_6ae9:
    if ((j == game.cPlayer))
        goto L_6b0d;
    else
        goto L_6af2;

L_6af2:

L_6af8:
    c = (c + 0x1);
    if ((c < 0x18))
        goto L_6a97;
    else
        goto L_6b05;

L_6b05:
    c = 0x0;

L_6b0d:
    CchGetString((c + 0x56e), rgplr[i].szName);
    strcpy(rgplr[i].szNames, rgplr[i].szName);
    strcat(rgplr[i].szNames, "s");

L_6b77:
    i = 0x0;
    goto L_6b83;

L_6b7f:
    i = (i + 0x1);

L_6b83:
    if ((i >= game.cPlayer))
        goto L_6c04;
    else
        goto L_6b8e;

L_6b8e:
    rgplrbmp[i] = ((rgplr[i].wMdPlr >> 0x3) & 0x1f);
    if ((rgplrbmp[i] < 0x0))
        goto L_6bf3;
    else
        goto L_6bc3;

L_6bc3:
    if ((rgplrbmp[i] >= 0x20))
        goto L_6bf3;
    else
        goto L_6bd5;

L_6bd5:
    if ((((rgplr[i].wMdPlr >> 0x9) & 0x1) == 0x0))
        goto L_6b7f;
    else
        goto L_6bf3;

L_6bf3:
    rgplrbmp[i] = 0xffff;

L_6c04:
    i = 0x1;
    goto L_6c10;

L_6c0c:
    i = (i + 0x1);

L_6c10:
    if ((i >= game.cPlayer))
        goto L_6ca3;
    else
        goto L_6c1b;

L_6c1b:
    if ((rgplrbmp[i] == 0xffff))
        goto L_6c0c;
    else
        goto L_6c2a;

L_6c2a:

L_6c30:
    j = 0x0;
    goto L_6c5f;

L_6c39:
    if ((rgplrbmp[j] == rgplrbmp[i]))
        goto L_6c6b;
    else
        goto L_6c54;

L_6c54:

L_6c5a:
    j = (j + 0x1);

L_6c5f:
    if ((j < i))
        goto L_6c39;
    else
        goto L_6c6b;

L_6c6b:
    if ((j >= i))
        goto L_6c0c;
    else
        goto L_6c77;

L_6c77:
    if ((Random(0x2) == 0x0))
        goto L_6c91;
    else
        goto L_6c8b;

L_6c8b:
    t_merge_6c95_0001 = i;
    goto L_6c95;

L_6c91:
    t_merge_6c95_0001 = j;

L_6c95:
    rgplrbmp[t_merge_6c95_0001] = 0xffff;

L_6ca3:
    i = 0x0;
    goto L_6caf;

L_6cab:
    i = (i + 0x1);

L_6caf:
    if ((i >= game.cPlayer))
        goto L_6d76;
    else
        goto L_6cba;

L_6cba:
    if ((rgplrbmp[i] != 0xffff))
        goto L_6cab;
    else
        goto L_6cc9;

L_6cc9:

L_6ccf:
    rgplrbmp[i] = Random(0x20);

L_6cea:
    j = 0x0;
    goto L_6cf8;

L_6cf3:
    j = (j + 0x1);

L_6cf8:
    if ((j >= game.cPlayer))
        goto L_6d34;
    else
        goto L_6d04;

L_6d04:
    if ((j == i))
        goto L_6cf3;
    else
        goto L_6d10;

L_6d10:
    if ((rgplrbmp[i] == rgplrbmp[j]))
        goto L_6d34;
    else
        goto L_6d2b;

L_6d2b:

L_6d34:
    if ((j == game.cPlayer))
        goto L_6cab;
    else
        goto L_6d3d;

L_6d3d:

L_6d43:
    rgplrbmp[i] = (rgplrbmp[i] + 0x1);
    if ((rgplrbmp[i] < 0x20))
        goto L_6cea;
    else
        goto L_6d62;

L_6d62:
    rgplrbmp[i] = 0x0;

L_6d76:
    i = 0x0;
    goto L_6d82;

L_6d7e:
    i = (i + 0x1);

L_6d82:
    if ((i >= game.cPlayer))
        goto L_6dde;
    else
        goto L_6d8d;

L_6d8d:
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xff07) | ((rgplrbmp[i] & 0x1f) * 0x8));
    goto L_6d7e;

L_6dde:
    if ((fFreeingTitle != 0x0))
        goto L_6e0a;
    else
        goto L_6de8;

L_6de8:
    fFreeingTitle = 0x1;
    DestroyWindow(hwndTitle);
    hwndTitle = 0x0;
    ShowWindow(hwndFrame, SW_SHOW);

L_6e0a:
    GenerateWorld(0x0);
    if ((idPlayer != 0xffff))
        goto L_6e25;
    else
        goto L_6e20;

L_6e20:
    BringUpHostDlg();

L_6e25:
    gd.fNoIdleChecks = fIdleSav;

L_6e3e:
    return;
}

void InitNewGamePlr(int16_t iStepMaxSoFar, int16_t lvlAi) {
    int16_t i;
    int16_t c;
    uint8_t ch;

L_6e44:
    if ((iStepMaxSoFar >= 0x2))
        goto L_74f1;
    else
        goto L_6e56;

L_6e56:
    if ((fRCWReadOnly != 0x0))
        goto L_74f1;
    else
        goto L_6e5d;

L_6e5d:

L_6e63:
    SetVCVal(&(game), 0x9, (game.mdSize * 0x2));
    if ((iStepMaxSoFar >= 0x1))
        goto L_74f1;
    else
        goto L_6e7f;

L_6e7f:

L_6e85:
    goto L_710e;

L_6e8b:
    if ((lvlAi != 0x3))
        goto L_6eb1;
    else
        goto L_6e94;

L_6e94:
    if ((Random(0x3) != 0x0))
        goto L_6eb1;
    else
        goto L_6ea8;

L_6ea8:
    game.cPlayer = 0x3;
    goto L_7139;

L_6eb1:
    game.cPlayer = 0x2;

L_6eba:
    if ((lvlAi != 0x3))
        goto L_6ee0;
    else
        goto L_6ec3;

L_6ec3:
    if ((Random(0x4) != 0x0))
        goto L_6ee0;
    else
        goto L_6ed7;

L_6ed7:
    game.cPlayer = 0x5;
    goto L_7139;

L_6ee0:
    if ((lvlAi < 0x2))
        goto L_6f09;
    else
        goto L_6ee9;

L_6ee9:
    if ((Random((0x6 - lvlAi)) != 0x0))
        goto L_6f09;
    else
        goto L_6f00;

L_6f00:
    game.cPlayer = 0x4;
    goto L_7139;

L_6f09:
    game.cPlayer = 0x3;

L_6f12:
    if ((lvlAi != 0x3))
        goto L_6f38;
    else
        goto L_6f1b;

L_6f1b:
    if ((Random(0xa) != 0x0))
        goto L_6f38;
    else
        goto L_6f2f;

L_6f2f:
    game.cPlayer = 0x9;
    goto L_7139;

L_6f38:
    if ((lvlAi != 0x3))
        goto L_6f5e;
    else
        goto L_6f41;

L_6f41:
    if ((Random(0xa) != 0x0))
        goto L_6f5e;
    else
        goto L_6f55;

L_6f55:
    game.cPlayer = 0x5;
    goto L_7139;

L_6f5e:
    if ((lvlAi < 0x2))
        goto L_6f87;
    else
        goto L_6f67;

L_6f67:
    if ((Random((0x7 - lvlAi)) != 0x0))
        goto L_6f87;
    else
        goto L_6f7e;

L_6f7e:
    game.cPlayer = 0x8;
    goto L_7139;

L_6f87:
    if ((lvlAi < 0x2))
        goto L_6fb0;
    else
        goto L_6f90;

L_6f90:
    if ((Random((0x7 - lvlAi)) != 0x0))
        goto L_6fb0;
    else
        goto L_6fa7;

L_6fa7:
    game.cPlayer = 0x6;
    goto L_7139;

L_6fb0:
    game.cPlayer = 0x7;

L_6fb9:
    if ((lvlAi != 0x3))
        goto L_6feb;
    else
        goto L_6fc2;

L_6fc2:
    if ((Random(0xa) != 0x0))
        goto L_6feb;
    else
        goto L_6fd6;

L_6fd6:
    game.cPlayer = (Random(0x2) + 0xe);
    goto L_7139;

L_6feb:
    if ((lvlAi != 0x3))
        goto L_7020;
    else
        goto L_6ff4;

L_6ff4:
    if ((Random(0xa) != 0x0))
        goto L_7020;
    else
        goto L_7008;

L_7008:
    game.cPlayer = (0xa - Random(0x2));
    goto L_7139;

L_7020:
    if ((lvlAi < 0x2))
        goto L_7049;
    else
        goto L_7029;

L_7029:
    if ((Random((0x7 - lvlAi)) != 0x0))
        goto L_7049;
    else
        goto L_7040;

L_7040:
    game.cPlayer = 0xd;
    goto L_7139;

L_7049:
    if ((lvlAi < 0x2))
        goto L_7072;
    else
        goto L_7052;

L_7052:
    if ((Random((0x7 - lvlAi)) != 0x0))
        goto L_7072;
    else
        goto L_7069;

L_7069:
    game.cPlayer = 0xb;
    goto L_7139;

L_7072:
    game.cPlayer = 0xc;

L_707b:
    if ((lvlAi != 0x3))
        goto L_70b0;
    else
        goto L_7084;

L_7084:
    if ((Random(0xa) != 0x0))
        goto L_70b0;
    else
        goto L_7098;

L_7098:
    game.cPlayer = (0xd - Random(0x3));
    goto L_7139;

L_70b0:
    if ((lvlAi < 0x2))
        goto L_70d9;
    else
        goto L_70b9;

L_70b9:
    if ((Random((0x9 - lvlAi)) != 0x0))
        goto L_70d9;
    else
        goto L_70d0;

L_70d0:
    game.cPlayer = 0xe;
    goto L_7139;

L_70d9:
    if ((lvlAi < 0x2))
        goto L_7102;
    else
        goto L_70e2;

L_70e2:
    if ((Random((0x7 - lvlAi)) != 0x0))
        goto L_7102;
    else
        goto L_70f9;

L_70f9:
    game.cPlayer = 0xf;
    goto L_7139;

L_7102:
    game.cPlayer = 0x10;

L_710e:
    if ((game.mdSize == 0x0))
        goto L_6e8b;
    else
        goto L_7116;

L_7116:
    if ((game.mdSize == 0x1))
        goto L_6eba;
    else
        goto L_711e;

L_711e:
    if ((game.mdSize == 0x2))
        goto L_6f12;
    else
        goto L_7126;

L_7126:
    if ((game.mdSize == 0x3))
        goto L_6fb9;
    else
        goto L_712e;

L_712e:
    if ((game.mdSize == 0x4))
        goto L_707b;
    else
        goto L_7139;

L_7139:
    i = 0x1;
    goto L_7474;

L_7144:
    if ((i >= game.cPlayer))
        goto L_7497;
    else
        goto L_714f;

L_714f:
    if ((i >= (((uint32_t)((game.cPlayer + 0x1)) / 0x3) + 0x1)))
        goto L_7175;
    else
        goto L_7166;

L_7166:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0xb;
    goto L_7144;

L_7175:
    if ((i >= (((uint32_t)(((game.cPlayer + 0x1) * 0x2)) / 0x3) + 0x1)))
        goto L_719d;
    else
        goto L_718e;

L_718e:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0xf;
    goto L_7144;

L_719d:
    /* untranslated: branch i >= ((words(loword(((game.cPlayer + 0x1) * 0x5)), signhiword(loword(((game.cPlayer + 0x1) * 0x5)))) / 0x6) + 0x1) ? L_71c8 : L_71b9
     */

L_71b9:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x7;
    goto L_7144;

L_71c8:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x1b;

L_71da:
    if ((i >= game.cPlayer))
        goto L_7497;
    else
        goto L_71e5;

L_71e5:
    if ((i >= (((uint32_t)(((game.cPlayer + 0x5) * 0x2)) / 0x7) + 0x1)))
        goto L_720d;
    else
        goto L_71fe;

L_71fe:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x27;
    goto L_71da;

L_720d:
    if ((i >= (((uint32_t)((LOWORD(((game.cPlayer + 0xffff) * 0x3)) + 0x6)) / 0x7) + 0x1)))
        goto L_723b;
    else
        goto L_722c;

L_722c:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x23;
    goto L_71da;

L_723b:
    if ((i >= (((uint32_t)((((game.cPlayer + 0xffff) * 0x4) + 0x6)) / 0x7) + 0x1)))
        goto L_7268;
    else
        goto L_7259;

L_7259:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x2b;
    goto L_71da;

L_7268:
    if ((i >= (((uint32_t)((LOWORD(((game.cPlayer + 0xffff) * 0x5)) + 0x6)) / 0x7) + 0x1)))
        goto L_7296;
    else
        goto L_7287;

L_7287:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x2f;
    goto L_71da;

L_7296:
    if ((i >= (((uint32_t)((LOWORD(((game.cPlayer + 0xffff) * 0x6)) + 0x6)) / 0x7) + 0x1)))
        goto L_72c4;
    else
        goto L_72b5;

L_72b5:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x33;
    goto L_71da;

L_72c4:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x9b;

L_72d6:
    if ((i >= game.cPlayer))
        goto L_7497;
    else
        goto L_72e1;

L_72e1:
    if ((i >= (((uint32_t)(((game.cPlayer + 0x5) * 0x2)) / 0x7) + 0x1)))
        goto L_7309;
    else
        goto L_72fa;

L_72fa:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x53;
    goto L_72d6;

L_7309:
    if ((i >= (((uint32_t)((LOWORD(((game.cPlayer + 0xffff) * 0x3)) + 0x6)) / 0x7) + 0x1)))
        goto L_7337;
    else
        goto L_7328;

L_7328:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x47;
    goto L_72d6;

L_7337:
    if ((i >= (((uint32_t)((((game.cPlayer + 0xffff) * 0x4) + 0x6)) / 0x7) + 0x1)))
        goto L_7364;
    else
        goto L_7355;

L_7355:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x43;
    goto L_72d6;

L_7364:
    if ((i >= (((uint32_t)((LOWORD(((game.cPlayer + 0xffff) * 0x5)) + 0x6)) / 0x7) + 0x1)))
        goto L_7392;
    else
        goto L_7383;

L_7383:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x57;
    goto L_72d6;

L_7392:
    if ((i >= (((uint32_t)((LOWORD(((game.cPlayer + 0xffff) * 0x6)) + 0x6)) / 0x7) + 0x1)))
        goto L_73c0;
    else
        goto L_73b1;

L_73b1:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x5b;
    goto L_72d6;

L_73c0:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x9b;

L_73d2:
    if ((i >= game.cPlayer))
        goto L_7497;
    else
        goto L_73dd;

L_73dd:
    if ((i >= (((uint32_t)((game.cPlayer + 0x1)) / 0x3) + 0x1)))
        goto L_7403;
    else
        goto L_73f4;

L_73f4:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x63;
    goto L_73d2;

L_7403:
    if ((i >= (((uint32_t)((LOWORD(((game.cPlayer + 0xffff) * 0x6)) + 0xb)) / 0xc) + 0x1)))
        goto L_7431;
    else
        goto L_7422;

L_7422:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x77;
    goto L_73d2;

L_7431:
    if ((i >= (((uint32_t)((LOWORD(((game.cPlayer + 0xffff) * 0x5)) + 0x5)) / 0x6) + 0x1)))
        goto L_745f;
    else
        goto L_7450;

L_7450:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x73;
    goto L_73d2;

L_745f:
    i = (i + 0x1);
    vrgplrTypeNew[i] = 0x7b;

L_7474:
    if ((lvlAi == 0x0))
        goto L_7144;
    else
        goto L_747c;

L_747c:
    if ((lvlAi == 0x1))
        goto L_71da;
    else
        goto L_7484;

L_7484:
    if ((lvlAi == 0x2))
        goto L_72d6;
    else
        goto L_748c;

L_748c:
    if ((lvlAi == 0x3))
        goto L_73d2;
    else
        goto L_7497;

L_7497:
    i = 0x1;
    goto L_74a3;

L_749f:
    i = (i + 0x1);

L_74a3:
    if ((i >= (game.cPlayer + 0xffff)))
        goto L_74f1;
    else
        goto L_74b1;

L_74b1:
    c = ((Random(((game.cPlayer - i) + 0xffff)) + i) + 0x1);
    ch = vrgplrTypeNew[i];
    vrgplrTypeNew[i] = vrgplrTypeNew[c];
    vrgplrTypeNew[c] = ch;
    goto L_749f;

L_74f1:
    return;
}

void InitNewGame3() {
L_74f8:
    return;
}

int16_t FGetNewGameName(char *szFileSuggest) {
    char         szXY[3];
    uint16_t     i;
    char         szFileTitle[256];
    char         szFile[256];
    char         szFilter[256];
    OPENFILENAME ofn;

L_7508:
    if ((szFileSuggest == 0x0))
        goto L_752d;
    else
        goto L_751a;

L_751a:
    strcpy(szFile, szFileSuggest);
    goto L_7532;

L_752d:
    szFile[0x0] = 0x0;

L_7532:
    CchGetString(idsStarsGameFilesXy, szFilter);
    i = 0x0;
    goto L_754f;

L_754b:
    i = (i + 0x1);

L_754f:
    if (((uint16_t)(szFilter[i]) == 0x0))
        goto L_7586;
    else
        goto L_7563;

L_7563:
    if (((uint16_t)(szFilter[i]) != 0x7c))
        goto L_754b;
    else
        goto L_7577;

L_7577:
    szFilter[i] = 0x0;

L_7586:
    memset(ofn, 0x0, 0x48);
    szXY[0x0] = 0x78;
    szXY[0x1] = 0x79;
    szXY[0x2] = 0x0;
    ofn.lStructSize = 0x48;
    ofn.hwndOwner = hwndFrame;
    LOWORD(ofn.lpstrFilter) = szFilter;
    /* untranslated: HIWORD(ofn.lpstrFilter) = ss */
    ofn.nFilterIndex = 0x1;
    LOWORD(ofn.lpstrFile) = szFile;
    /* untranslated: HIWORD(ofn.lpstrFile) = ss */
    ofn.nMaxFile = 0x100;
    LOWORD(ofn.lpstrFileTitle) = szFileTitle;
    /* untranslated: HIWORD(ofn.lpstrFileTitle) = ss */
    ofn.nMaxFileTitle = 0x100;
    ofn.lpstrInitialDir = szDirName;
    ofn.lpstrTitle = "Choose New Game Name";
    LOWORD(ofn.lpstrDefExt) = szXY;
    /* untranslated: HIWORD(ofn.lpstrDefExt) = ss */
    ofn.Flags = 0x8806;
    if ((GetSaveFileName(&(ofn)) == 0x0))
        goto L_769e;
    else
        goto L_7650;

L_7650:
    if ((ofn.nFileExtension == 0x0))
        goto L_7674;
    else
        goto L_765a;

L_765a:
    strcpy(szFile[ofn.nFileExtension], szXY);
    goto L_7685;

L_7674:
    strcat(szFile, szXY);

L_7685:
    strcpy(szFileSuggest, szFile);
    return 0x1;

L_769e:
    return 0x0;
}

int16_t SimpleNewGameDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t     i;
    RECT        rc;
    HWND        hwndDD;
    HDC         hdc;
    RECT        rcGBox;
    int16_t     dy;
    int16_t     c;
    PAINTSTRUCT ps;
    RECT       *prcSav;

L_76aa:
    goto L_7e58;

L_76b9:
    CheckRadioButton(hwnd, 0xc8, 0xcb, 0xc9);
    CheckRadioButton(hwnd, 0x3e8, 0x3ec, 0x3e9);
    hwndDD = GetDlgItem(hwnd, IDC_COMBOBOX);
    SendMessage(hwndDD, CB_RESETCONTENT, 0x0, 0x0);
    i = 0x0;
    goto L_7737;

L_7710:
    /* untranslated: call SendMessage(hwndDD, CB_ADDSTRING, 0x0, words(ds, PszGetCompressedString((i + 0x567)))) -> callresult(LRESULT) */
    i = (i + 0x1);

L_7737:
    if ((i < 0x7))
        goto L_7710;
    else
        goto L_7740;

L_7740:
    SendMessage(hwndDD, CB_SETCURSEL, 0x0, 0x0);
    StickyDlgPos(hwnd, &(ptStickyNewDlg), 0x1);
    return 0x1;

L_7771:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_7799:
    i = 0xc8;
    goto L_77cb;

L_77a1:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] != GetDlgItem(hwnd, i) ? L_77c7 : L_77bf */

L_77bf:
    i = 0xffff;
    goto L_77d5;

L_77c7:
    i = (i + 0x1);

L_77cb:
    if ((i <= 0xcb))
        goto L_77a1;
    else
        goto L_77d5;

L_77d5:
    if ((i == 0xffff))
        goto L_781a;
    else
        goto L_77de;

L_77de:
    i = 0x3e8;
    goto L_7810;

L_77e6:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] != GetDlgItem(hwnd, i) ? L_780c : L_7804 */

L_7804:
    i = 0xffff;
    goto L_781a;

L_780c:
    i = (i + 0x1);

L_7810:
    if ((i <= 0x3ec))
        goto L_77e6;
    else
        goto L_781a;

L_781a:
    if ((i == 0xffff))
        goto L_783f;
    else
        goto L_7823;

L_7823:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_7e83;
    else
        goto L_783f;

L_783f:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_7858:
    hdc = BeginPaint(hwnd, &(ps));
    GetWindowRect(GetDlgItem(hwnd, 0xc8), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x00CB), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    SelectObject(hdc, rghfontArial8[0x1]);
    SetBkColor(hdc, crButtonFace);
    c = CchGetString(idsDifficultyLevel, szWork);
    TextOut(hdc, (rcGBox.left + 0x8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, c);
    GetWindowRect(GetDlgItem(hwnd, 0x3e8), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, 0x3ec), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsUniverseSize, szWork);
    TextOut(hdc, (rcGBox.left + 0x8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, c);
    rcGBox.top = (rcGBox.bottom + 0x8);
    GetWindowRect(GetDlgItem(hwnd, IDC_COMBOBOX), &(rcGBox));
    MapWindowPoints(0x0, hwnd, &(rcGBox), 0x2);
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    rcGBox.bottom = (rcGBox.bottom + (dyArial8 * 0x2));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsPlayerRace, szWork);
    TextOut(hdc, (rcGBox.left + 0x8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, c);
    rcGBox.top = (LOWORD((0x3 * dyArial8)) + rcGBox.bottom);
    rcGBox.bottom = 0x3e8;
    /* untranslated: call ExpandRc(&rcGBox, (dyArial8 neg 0x0), 0x0) -> callresult(void) */
    c = CchGetString(idsButtonAllowsConfigureMultiPlayerGamesCustom, szWork);
    dy = DrawText(hdc, szWork, c, &(rcGBox), 0x810);
    SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x00D3), 0x0, rcGBox.left, ((rcGBox.top + dy) + ((uint32_t)(dyArial8) / 0x2)), 0x0, 0x0, 0x5);
    rcGBox.bottom = ((rcGBox.top + dy) + (dyArial8 * 0x2));
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xfffe);
    c = CchGetString(idsAdvancedGame, szWork);
    TextOut(hdc, (rcGBox.left + 0x8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, c);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_7bce:
    if ((wParam == 0x430))
        goto L_7beb;
    else
        goto L_7bd8;

L_7bd8:
    if ((wParam == 0x2))
        goto L_7beb;
    else
        goto L_7be1;

L_7be1:
    if ((wParam != 0xd3))
        goto L_7ca2;
    else
        goto L_7beb;

L_7beb:
    i = 0xc8;
    goto L_7c0d;

L_7bf3:
    if ((IsDlgButtonChecked(hwnd, i) != 0x0))
        goto L_7c17;
    else
        goto L_7c03;

L_7c03:

L_7c09:
    i = (i + 0x1);

L_7c0d:
    if ((i <= 0xcb))
        goto L_7bf3;
    else
        goto L_7c17;

L_7c17:
    game.mdDensity = (i + 0xff38);
    i = 0x3e8;
    goto L_7c42;

L_7c28:
    if ((IsDlgButtonChecked(hwnd, i) != 0x0))
        goto L_7c4c;
    else
        goto L_7c38;

L_7c38:

L_7c3e:
    i = (i + 0x1);

L_7c42:
    if ((i <= 0x3ec))
        goto L_7c28;
    else
        goto L_7c4c;

L_7c4c:
    game.mdSize = (i + 0xfc18);
    game.turn = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_COMBOBOX), CB_GETCURSEL, 0x0, 0x0));
    StickyDlgPos(hwnd, &(ptStickyNewDlg), 0x0);
    EndDialog(hwnd, wParam);
    return 0x1;

L_7ca2:
    if ((wParam != 0xd4))
        goto L_7cd4;
    else
        goto L_7cac;

L_7cac:
    StickyDlgPos(hwnd, &(ptStickyNewDlg), 0x0);
    EndDialog(hwnd, wParam);
    return 0x1;

L_7cd4:
    if ((wParam != 0xd2))
        goto L_7e28;
    else
        goto L_7cde;

L_7cde:
    hwndDD = GetDlgItem(hwnd, IDC_COMBOBOX);
    game.turn = LOWORD(SendMessage(hwndDD, CB_GETCURSEL, 0x0, 0x0));
    if ((game.turn >= 0x7))
        goto L_7d60;
    else
        goto L_7d12;

L_7d12:
    vplr = vrgplrDef[game.turn];
    CchGetString((game.turn + 0x567), vplr.szName);
    _wsprintf("", "%ss", &(vplr.szName));
    goto L_7d71;

L_7d60:
    vplr = *(vrgplrNew);

L_7d71:
    prcSav = vrgrcRCW;
    if ((RaceCreationWizard(hwnd, 0x0, 0x1) == 0x0))
        goto L_7e17;
    else
        goto L_7d92;

L_7d92:
    if ((HIWORD(SendMessage(hwndDD, CB_GETCOUNT, 0x0, 0x0)) < 0x0))
        goto L_7dd7;
    else
        goto L_7db2;

L_7db2:
    /* untranslated: branch hiword(callresult(LRESULT)) > 0x0 ? L_7dbf : L_7db7 */

L_7db7:
    /* untranslated: branch loword(callresult(LRESULT)) <= 0x7 ? L_7dd7 : L_7dbf */

L_7dbf:
    SendMessage(hwndDD, CB_DELETESTRING, 0x7, 0x0);

L_7dd7:
    /* untranslated: call SendMessage(hwndDD, CB_ADDSTRING, 0x0, words(ds, 0x501a)) -> callresult(LRESULT) */
    SendMessage(hwndDD, CB_SETCURSEL, 0x7, 0x0);
    *(vrgplrNew) = vplr;

L_7e17:
    vrgrcRCW = prcSav;
    SetFocus(hwnd);
    goto L_7e83;

L_7e28:
    if ((wParam != 0x76))
        goto L_7e83;
    else
        goto L_7e31;

L_7e31:
    WinHelp(hwnd, szHelpFile, 0x1, 0x3ea);
    return 0x1;

L_7e58:
    if ((message == WM_PAINT))
        goto L_7858;
    else
        goto L_7e60;

L_7e60:
    if ((message == WM_ERASEBKGND))
        goto L_7771;
    else
        goto L_7e68;

L_7e68:
    if ((message == WM_CTLCOLOR))
        goto L_7799;
    else
        goto L_7e70;

L_7e70:
    if ((message == WM_INITDIALOG))
        goto L_76b9;
    else
        goto L_7e78;

L_7e78:
    if ((message == WM_COMMAND))
        goto L_7bce;
    else
        goto L_7e83;

L_7e83:
    return 0x0;
}

int16_t NewGameDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t     i;
    RECT        rc;
    HDC         hdc;
    RECT        rcGBox;
    int16_t     c;
    PAINTSTRUCT ps;
    int16_t     iRet;

L_7e92:
    goto L_8798;

L_7ea1:
    SetNGWTitle(hwnd, 0x1);
    CheckRadioButton(hwnd, 0x3e8, 0x3ec, (game.mdSize + 0x3e8));
    CheckRadioButton(hwnd, 0x3ed, 0x3f0, (game.mdDensity + 0x3ed));
    CheckRadioButton(hwnd, 0x3f1, 0x3f4, (game.mdStartDist + 0x3f1));
    SetWindowText(GetDlgItem(hwnd, IDC_U16_0x0406), "");
    SendDlgItemMessage(hwnd, 0x406, 0x415, 0x1f, 0x0);
    SendMessage(GetDlgItem(hwnd, 0x3f8), CB_LIMITTEXT, game.fExtraFuel, 0x0);
    SendMessage(GetDlgItem(hwnd, 0x3f9), CB_LIMITTEXT, game.fSlowTech, 0x0);
    SendMessage(GetDlgItem(hwnd, 0x3fa), CB_LIMITTEXT, game.fBBSPlay, 0x0);
    SendMessage(GetDlgItem(hwnd, 0x3fb), CB_LIMITTEXT, game.fNoRandom, 0x0);
    SendMessage(GetDlgItem(hwnd, 0x3fc), CB_LIMITTEXT, game.fAisBand, 0x0);
    SendMessage(GetDlgItem(hwnd, 0x3fd), CB_LIMITTEXT, game.fVisScores, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041A), CB_LIMITTEXT, game.fClumping, 0x0);
    if ((fRCWReadOnly == 0x0))
        goto L_818b;
    else
        goto L_805a;

L_805a:
    i = 0x3e8;
    goto L_807b;

L_8062:
    EnableWindow(GetDlgItem(hwnd, i), 0x0);
    i = (i + 0x1);

L_807b:
    if ((i <= 0x3ec))
        goto L_8062;
    else
        goto L_8085;

L_8085:
    i = 0x3ed;
    goto L_80a6;

L_808d:
    EnableWindow(GetDlgItem(hwnd, i), 0x0);
    i = (i + 0x1);

L_80a6:
    if ((i <= 0x3f0))
        goto L_808d;
    else
        goto L_80b0;

L_80b0:
    i = 0x3f1;
    goto L_80d1;

L_80b8:
    EnableWindow(GetDlgItem(hwnd, i), 0x0);
    i = (i + 0x1);

L_80d1:
    if ((i <= 0x3f4))
        goto L_80b8;
    else
        goto L_80db;

L_80db:
    EnableWindow(GetDlgItem(hwnd, 0x3f8), 0x0);
    EnableWindow(GetDlgItem(hwnd, 0x3f9), 0x0);
    EnableWindow(GetDlgItem(hwnd, 0x3fa), 0x0);
    EnableWindow(GetDlgItem(hwnd, 0x3fb), 0x0);
    EnableWindow(GetDlgItem(hwnd, IDC_U16_0x041A), 0x0);
    EnableWindow(GetDlgItem(hwnd, 0x3fc), 0x0);
    EnableWindow(GetDlgItem(hwnd, IDC_U16_0x0406), 0x0);
    EnableWindow(GetDlgItem(hwnd, 0x3fd), 0x0);

L_818b:
    StickyDlgPos(hwnd, &(ptStickyNewDlg), 0x1);
    return 0x1;

L_81a4:
    i = 0x3e8;
    goto L_81d1;

L_81ac:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] == GetDlgItem(hwnd, i) ? L_81db : L_81c7 */

L_81c7:

L_81cd:
    i = (i + 0x1);

L_81d1:
    if ((i <= 0x3fd))
        goto L_81ac;
    else
        goto L_81db;

L_81db:
    if ((i <= 0x3fd))
        goto L_8220;
    else
        goto L_81e5;

L_81e5:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) == 0x6))
        goto L_8220;
    else
        goto L_8201;

L_8201:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] != GetDlgItem(hwnd, IDC_U16_0x041A) ? L_87c3 : L_8220 */

L_8220:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_8239:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_8261:
    hdc = BeginPaint(hwnd, &(ps));
    GetWindowRect(GetDlgItem(hwnd, 0x3e8), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, 0x3ec), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    SelectObject(hdc, rghfontArial8[0x1]);
    SetBkColor(hdc, crButtonFace);
    c = CchGetString(idsUniverseSize, szWork);
    TextOut(hdc, (rcGBox.left + 0x8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, c);
    GetWindowRect(GetDlgItem(hwnd, 0x3ed), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, 0x3f0), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    SelectObject(hdc, rghfontArial8[0x1]);
    SetBkColor(hdc, crButtonFace);
    c = CchGetString(idsDensity, szWork);
    TextOut(hdc, (rcGBox.left + 0x8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, c);
    GetWindowRect(GetDlgItem(hwnd, 0x3f1), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, 0x3f4), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    SelectObject(hdc, rghfontArial8[0x1]);
    SetBkColor(hdc, crButtonFace);
    c = CchGetString(idsPlayerPositions, szWork);
    TextOut(hdc, (rcGBox.left + 0x8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, c);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_8510:
    if ((wParam != 0x76))
        goto L_853a;
    else
        goto L_8519;

L_8519:
    WinHelp(hwnd, szHelpFile, 0x1, 0x3f4);
    return 0x1;

L_853a:
    iRet = 0x0;
    goto L_855a;

L_8542:
    if ((wParam == rgidRaceBtn[iRet]))
        goto L_8563;
    else
        goto L_8550;

L_8550:

L_8556:
    iRet = (iRet + 0x1);

L_855a:
    if ((iRet < 0x4))
        goto L_8542;
    else
        goto L_8563;

L_8563:
    if ((iRet >= 0x4))
        goto L_87c3;
    else
        goto L_856c;

L_856c:
    if ((iRet == 0x0))
        goto L_876e;
    else
        goto L_8575;

L_8575:
    i = 0x3e8;
    goto L_8597;

L_857d:
    if ((IsDlgButtonChecked(hwnd, i) != 0x0))
        goto L_85a1;
    else
        goto L_858d;

L_858d:

L_8593:
    i = (i + 0x1);

L_8597:
    if ((i <= 0x3ec))
        goto L_857d;
    else
        goto L_85a1;

L_85a1:
    game.mdSize = (i + 0xfc18);
    i = 0x3ed;
    goto L_85cc;

L_85b2:
    if ((IsDlgButtonChecked(hwnd, i) != 0x0))
        goto L_85d6;
    else
        goto L_85c2;

L_85c2:

L_85c8:
    i = (i + 0x1);

L_85cc:
    if ((i <= 0x3f0))
        goto L_85b2;
    else
        goto L_85d6;

L_85d6:
    game.mdDensity = (i + 0xfc13);
    i = 0x3f1;
    goto L_8601;

L_85e7:
    if ((IsDlgButtonChecked(hwnd, i) != 0x0))
        goto L_860b;
    else
        goto L_85f7;

L_85f7:

L_85fd:
    i = (i + 0x1);

L_8601:
    if ((i <= 0x3f4))
        goto L_85e7;
    else
        goto L_860b;

L_860b:
    game.mdStartDist = (i + 0xfc0f);
    game.wCrap = ((game.wCrap & 0xfffe) | (IsDlgButtonChecked(hwnd, 0x3f8) & 0x1));
    game.wCrap = ((game.wCrap & 0xfffd) | ((IsDlgButtonChecked(hwnd, 0x3f9) & 0x1) << 0x1));
    game.wCrap = ((game.wCrap & 0xffdf) | ((IsDlgButtonChecked(hwnd, 0x3fa) & 0x1) << 0x5));
    game.wCrap = ((game.wCrap & 0xff7f) | ((IsDlgButtonChecked(hwnd, 0x3fb) & 0x1) << 0x7));
    game.wCrap = ((game.wCrap & 0xffef) | ((IsDlgButtonChecked(hwnd, 0x3fc) & 0x1) << 0x4));
    game.wCrap = ((game.wCrap & 0xffbf) | ((IsDlgButtonChecked(hwnd, 0x3fd) & 0x1) << 0x6));
    game.wCrap = ((game.wCrap & 0xfeff) | ((IsDlgButtonChecked(hwnd, 0x41a) & 0x1) << 0x8));
    i = GetWindowText(GetDlgItem(hwnd, IDC_U16_0x0406), "", 0x20);
    if ((i != 0x0))
        goto L_876e;
    else
        goto L_875e;

L_875e:
    strcpy(game.szName, szBase);

L_876e:
    StickyDlgPos(hwnd, &(ptStickyNewDlg), 0x0);
    EndDialog(hwnd, iRet);
    return 0x1;

L_8798:
    if ((message == WM_PAINT))
        goto L_8261;
    else
        goto L_87a0;

L_87a0:
    if ((message == WM_ERASEBKGND))
        goto L_8239;
    else
        goto L_87a8;

L_87a8:
    if ((message == WM_CTLCOLOR))
        goto L_81a4;
    else
        goto L_87b0;

L_87b0:
    if ((message == WM_INITDIALOG))
        goto L_7ea1;
    else
        goto L_87b8;

L_87b8:
    if ((message == WM_COMMAND))
        goto L_8510;
    else
        goto L_87c3;

L_87c3:
    return 0x0;
}

int16_t NewGameDlg2(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t     i;
    RECT        rc;
    RECT        rcT;
    int16_t     dyBut;
    int16_t     dy;
    int16_t     dyCur;
    HWND        hwndBtn;
    POINT       pt;
    int16_t     iDiamond;
    int16_t     iNewVal;
    int16_t     j;
    char       *psz;
    int16_t     tpm;
    int16_t     iChecked;
    uint16_t    rghmenuSubPopup[14];
    HMENU       hmenuPopup;
    MSG         msg;
    int16_t     iCurVal;
    RECT       *prcSav;
    HDC         hdc;
    PAINTSTRUCT ps;
    uint16_t    t_merge_8b4f_0001;
    uint16_t    t_merge_8c45_0001;
    uint16_t    t_merge_8cf6_0001;
    uint16_t    t_merge_8d61_0001;
    uint16_t    t_merge_8dc1_0001;
    uint16_t    t_merge_8dfd_0001;
    uint16_t    t_merge_8e8d_0001;
    uint16_t    t_merge_8ee1_0001;
    uint16_t    t_merge_8f1a_0001;
    int16_t     t_9060;

L_87d2:
    goto L_9505;

L_87e1:
    SetNGWTitle(hwnd, 0x2);
    dy = (((dyArial8 + 0x4) * 0x10) + 0x8);
    GetWindowRect(GetDlgItem(hwnd, rgidRaceBtn[0x0]), &(rc));
    dyBut = (rc.bottom - rc.top);
    GetWindowRect(hwnd, &(rcT));
    dyCur = (rcT.bottom - rcT.top);
    if ((dyCur >= ((dy + dyBut) + 0x6)))
        goto L_8923;
    else
        goto L_884f;

L_884f:
    i = 0x0;
    goto L_88cb;

L_8857:
    hwndBtn = GetDlgItem(hwnd, rgidRaceBtn[i]);
    GetWindowRect(hwndBtn, &(rc));
    MapWindowPoints(0x0, hwnd, &(rc), 0x2);
    OffsetRect(&(rc), 0x0, (dy - rc.top));
    SetWindowPos(hwndBtn, 0x0, rc.left, rc.top, 0x0, 0x0, 0x5);
    i = (i + 0x1);

L_88cb:
    if ((i < 0x5))
        goto L_8857;
    else
        goto L_88d4;

L_88d4:
    dy = (rc.bottom + 0x6);
    GetClientRect(hwnd, &(rc));
    if ((dyCur >= dy))
        goto L_8923;
    else
        goto L_88f7;

L_88f7:
    SetWindowPos(hwnd, 0x0, 0x0, 0x0, (rcT.right - rcT.left), (((dy + rcT.bottom) - rcT.top) - rc.bottom), 0x6);

L_8923:
    StickyDlgPos(hwnd, &(ptStickyNewDlg), 0x1);
    return 0x1;

L_893c:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_8964:
    i = 0x191;
    goto L_8991;

L_896c:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] == GetDlgItem(hwnd, i) ? L_899b : L_8987 */

L_8987:

L_898d:
    i = (i + 0x1);

L_8991:
    if ((i <= 0x1c0))
        goto L_896c;
    else
        goto L_899b;

L_899b:
    if ((i <= 0x1c0))
        goto L_89c1;
    else
        goto L_89a5;

L_89a5:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_9548;
    else
        goto L_89c1;

L_89c1:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_89da:
    if ((fRCWReadOnly != 0x0))
        goto L_9548;
    else
        goto L_89e1;

L_89e1:

L_89e7:
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    if ((pt.x < xNewGameDiamond))
        goto L_9548;
    else
        goto L_8a0d;

L_8a0d:
    if ((pt.x >= ((xNewGameDiamond + dyArial8) + 0x1)))
        goto L_9548;
    else
        goto L_8a1f;

L_8a1f:
    if ((pt.y < 0x6))
        goto L_9548;
    else
        goto L_8a25;

L_8a25:

L_8a2b:
    iDiamond = ((uint32_t)((pt.y + 0xfffa)) / (dyArial8 + 0x4));
    if ((iDiamond >= 0x10))
        goto L_9548;
    else
        goto L_8a47;

L_8a47:
    if ((((uint32_t)((pt.y + 0xfffa)) % (dyArial8 + 0x4)) >= (dyArial8 + 0x1)))
        goto L_9548;
    else
        goto L_8a61;

L_8a61:

L_8a67:
    SetCursor(hcurHand);
    return 0x1;

L_8a76:
    if ((fRCWReadOnly != 0x0))
        goto L_9548;
    else
        goto L_8a7d;

L_8a7d:

L_8a83:
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    if ((pt.x < xNewGameDiamond))
        goto L_9548;
    else
        goto L_8aae;

L_8aae:
    if ((pt.x >= ((xNewGameDiamond + dyArial8) + 0x1)))
        goto L_9548;
    else
        goto L_8ac0;

L_8ac0:
    if ((pt.y < 0x6))
        goto L_9548;
    else
        goto L_8ac6;

L_8ac6:

L_8acc:
    iDiamond = ((uint32_t)((pt.y + 0xfffa)) / (dyArial8 + 0x4));
    if ((iDiamond >= 0x10))
        goto L_9548;
    else
        goto L_8ae8;

L_8ae8:
    if ((((uint32_t)((pt.y + 0xfffa)) % (dyArial8 + 0x4)) >= (dyArial8 + 0x1)))
        goto L_9548;
    else
        goto L_8b02;

L_8b02:

L_8b08:
    iCurVal = vrgplrTypeNew[iDiamond];
    ClientToScreen(hwnd, &(pt));
    rghmenuSubPopup[0x0] = CreatePopupMenu();
    i = 0x0;
    goto L_8b85;

L_8b34:
    if ((iCurVal != ((i * 0x4) + 0x1)))
        goto L_8b4c;
    else
        goto L_8b46;

L_8b46:
    t_merge_8b4f_0001 = 0x8;
    goto L_8b4f;

L_8b4c:
    t_merge_8b4f_0001 = 0x0;

L_8b4f:
    iChecked = t_merge_8b4f_0001;
    psz = PszGetCompressedString((i + 0x567));
    AppendMenu(rghmenuSubPopup[0x0], (0x0 | iChecked), (i + 0x3aa8), psz);
    i = (i + 0x1);

L_8b85:
    if ((i < 0x6))
        goto L_8b34;
    else
        goto L_8b8e;

L_8b8e:
    AppendMenu(rghmenuSubPopup[0x0], (0x0 | iChecked), (i + 0x3aa8), PszGetCompressedString(idsRandom));
    AppendMenu(rghmenuSubPopup[0x0], (0x0 | iChecked), (i + 0x3aa9), PszGetCompressedString(idsExpansionPlayer));
    rghmenuSubPopup[0x1] = CreatePopupMenu();
    AppendMenu(rghmenuSubPopup[0x1], 0x0, 0x3a98, PszGetCompressedString(idsNew));
    AppendMenu(rghmenuSubPopup[0x1], 0x0, 0x3a99, PszGetCompressedString(idsOpen));
    i = 0x0;
    goto L_8c95;

L_8c2a:
    if ((iCurVal != ((i * 0x4) + 0x2)))
        goto L_8c42;
    else
        goto L_8c3c;

L_8c3c:
    t_merge_8c45_0001 = 0x8;
    goto L_8c45;

L_8c42:
    t_merge_8c45_0001 = 0x0;

L_8c45:
    iChecked = t_merge_8c45_0001;
    psz = PszPlayerName(0x0, 0x1, 0x1, 0x1, 0x0, &(vrgplrNew[i]));
    AppendMenu(rghmenuSubPopup[0x1], (0x0 | iChecked), (i + 0x3ab8), psz);
    i = (i + 0x1);

L_8c95:
    if ((i < vcplrNew))
        goto L_8c2a;
    else
        goto L_8ca0;

L_8ca0:
    rghmenuSubPopup[0x2] = CreatePopupMenu();
    i = 0x0;
    goto L_8d93;

L_8cb0:
    rghmenuSubPopup[(i + 0x5)] = CreatePopupMenu();
    j = 0x0;
    goto L_8d38;

L_8ccc:
    if ((iCurVal != (((j * 0x20) + (i * 0x4)) + 0x5)))
        goto L_8cf3;
    else
        goto L_8ced;

L_8ced:
    t_merge_8cf6_0001 = 0x8;
    goto L_8cf6;

L_8cf3:
    t_merge_8cf6_0001 = 0x0;

L_8cf6:
    iChecked = t_merge_8cf6_0001;
    AppendMenu(rghmenuSubPopup[(i + 0x5)], (0x0 | iChecked), ((i + 0x3ac8) + (j * 0x8)), vrgszComputerLevel[j]);
    j = (j + 0x1);

L_8d38:
    if ((j < 0x5))
        goto L_8ccc;
    else
        goto L_8d41;

L_8d41:
    if (((iCurVal & 0x1f) != ((i * 0x4) + 0x5)))
        goto L_8d5e;
    else
        goto L_8d58;

L_8d58:
    t_merge_8d61_0001 = 0x8;
    goto L_8d61;

L_8d5e:
    t_merge_8d61_0001 = 0x0;

L_8d61:
    iChecked = t_merge_8d61_0001;
    AppendMenu(rghmenuSubPopup[0x2], (0x10 | iChecked), rghmenuSubPopup[(i + 0x5)], vrgszComputerPlayers[i]);
    i = (i + 0x1);

L_8d93:
    if ((i <= 0x6))
        goto L_8cb0;
    else
        goto L_8d9c;

L_8d9c:
    hmenuPopup = CreatePopupMenu();
    iPopMenuSel = 0xffff;
    if (((iCurVal & 0x3) != 0x1))
        goto L_8dbe;
    else
        goto L_8db8;

L_8db8:
    t_merge_8dc1_0001 = 0x8;
    goto L_8dc1;

L_8dbe:
    t_merge_8dc1_0001 = 0x0;

L_8dc1:
    iChecked = t_merge_8dc1_0001;
    AppendMenu(hmenuPopup, (0x10 | iChecked), rghmenuSubPopup[0x0], PszGetCompressedString(idsPredefinedRace));
    if (((iCurVal & 0x3) != 0x2))
        goto L_8dfa;
    else
        goto L_8df4;

L_8df4:
    t_merge_8dfd_0001 = 0x8;
    goto L_8dfd;

L_8dfa:
    t_merge_8dfd_0001 = 0x0;

L_8dfd:
    iChecked = t_merge_8dfd_0001;
    AppendMenu(hmenuPopup, (0x10 | iChecked), rghmenuSubPopup[0x1], PszGetCompressedString(idsCustomRace));
    if (((iCurVal & 0x3) == 0x1))
        goto L_8e3e;
    else
        goto L_8e30;

L_8e30:
    if (((iCurVal & 0x3) != 0x2))
        goto L_8e5e;
    else
        goto L_8e3e;

L_8e3e:
    AppendMenu(hmenuPopup, 0x0, 0x3a9a, PszGetCompressedString(idsEditRace));

L_8e5e:
    AppendMenu(hmenuPopup, 0x800, 0x0, 0x0);
    if (((iCurVal & 0x3) != 0x3))
        goto L_8e8a;
    else
        goto L_8e84;

L_8e84:
    t_merge_8e8d_0001 = 0x8;
    goto L_8e8d;

L_8e8a:
    t_merge_8e8d_0001 = 0x0;

L_8e8d:
    iChecked = t_merge_8e8d_0001;
    AppendMenu(hmenuPopup, (0x10 | iChecked), rghmenuSubPopup[0x2], PszGetCompressedString(idsComputerPlayer));
    AppendMenu(hmenuPopup, 0x800, 0x0, 0x0);
    if (((iCurVal & 0x3) != 0x0))
        goto L_8ede;
    else
        goto L_8ed8;

L_8ed8:
    t_merge_8ee1_0001 = 0x8;
    goto L_8ee1;

L_8ede:
    t_merge_8ee1_0001 = 0x0;

L_8ee1:
    iChecked = t_merge_8ee1_0001;
    AppendMenu(hmenuPopup, (0x0 | iChecked), 0x3a9b, PszGetCompressedString(idsPlayer));
    if ((message != WM_LBUTTONDOWN))
        goto L_8f17;
    else
        goto L_8f11;

L_8f11:
    t_merge_8f1a_0001 = 0x0;
    goto L_8f1a;

L_8f17:
    t_merge_8f1a_0001 = 0x2;

L_8f1a:
    tpm = t_merge_8f1a_0001;
    TrackPopupMenu(hmenuPopup, (0x4 | tpm), pt.x, pt.y, 0x0, hwnd, 0x0);
    DestroyMenu(hmenuPopup);
    i = 0x0;
    goto L_8f66;

L_8f51:
    DestroyMenu(rghmenuSubPopup[i]);
    i = (i + 0x1);

L_8f66:
    if ((i < 0x6))
        goto L_8f51;
    else
        goto L_8f6f;

L_8f6f:
    if ((PeekMessage(&(msg), hwnd, 0x111, 0x111, 0x2) == 0x0))
        goto L_8faf;
    else
        goto L_8f92;

L_8f92:
    if ((msg.wParam < 0x3a98))
        goto L_8faf;
    else
        goto L_8f9c;

L_8f9c:
    if ((msg.wParam >= 0x3afc))
        goto L_8faf;
    else
        goto L_8fa6;

L_8fa6:
    iPopMenuSel = (msg.wParam + 0xc568);

L_8faf:
    iNewVal = 0xffff;
    if ((iPopMenuSel == 0xffff))
        goto L_9367;
    else
        goto L_8fbe;

L_8fbe:
    if ((iPopMenuSel != 0x0))
        goto L_9035;
    else
        goto L_8fc8;

L_8fc8:
    vplr = vrgplrDef[0x0];
    CchGetString(idsHumanoid, vplr.szName);
    _wsprintf("", "%ss", &(vplr.szName));
    prcSav = vrgrcRCW;
    if ((RaceCreationWizard(hwnd, 0x0, 0x0) == 0x0))
        goto L_902f;
    else
        goto L_9026;

L_9026:
    vrgrcRCW = prcSav;
    goto PlaceNew;

L_902f:
    vrgrcRCW = prcSav;

L_9035:
    if ((iPopMenuSel != 0x1))
        goto L_9124;
    else
        goto L_903f;

L_903f:
    if ((FOpenGame(hwnd, 0x1) <= 0x0))
        goto L_9367;
    else
        goto PlaceNew;

PlaceNew:
    if ((vcplrNew >= 0x10))
        goto L_906e;
    else
        goto L_9060;

L_9060:
    t_9060 = vcplrNew;
    vcplrNew = (vcplrNew + 0x1);
    iNewVal = t_9060;
    goto L_90d5;

L_906e:
    iNewVal = 0xf;
    goto L_90cc;

L_9076:
    i = 0x0;
    goto L_90b3;

L_907e:
    if (((vrgplrTypeNew[i] & 0x3) != 0x2))
        goto L_90af;
    else
        goto L_9093;

L_9093:
    if ((((vrgplrTypeNew[i] >> 0x2) & 0xf) == iNewVal))
        goto L_90bc;
    else
        goto L_90a9;

L_90a9:

L_90af:
    i = (i + 0x1);

L_90b3:
    if ((i < 0x10))
        goto L_907e;
    else
        goto L_90bc;

L_90bc:
    if ((i == 0x10))
        goto L_90d5;
    else
        goto L_90c2;

L_90c2:

L_90c8:
    iNewVal = (iNewVal - 0x1);

L_90cc:
    if ((iNewVal >= 0x0))
        goto L_9076;
    else
        goto L_90d5;

L_90d5:
    vrgplrNew[iNewVal] = vplr;
    strcpy(&(vrgszFileNew[(0xd * iNewVal)]), szRaceFile);
    iNewVal = ((iNewVal << 0x2) | 0x2);

L_9124:
    if ((iPopMenuSel != 0x2))
        goto L_92ed;
    else
        goto L_912e;

L_912e:
    if (((iCurVal & 0x3) != 0x1))
        goto L_9193;
    else
        goto L_913c;

L_913c:
    vplr = vrgplrDef[(iCurVal >> 0x2)];
    CchGetString(((iCurVal >> 0x2) + 0x567), vplr.szName);
    _wsprintf("", "%ss", &(vplr.szName));
    goto L_91d3;

L_9193:
    vplr = vrgplrNew[(iCurVal >> 0x2)];
    strcpy(szRaceFile, &(vrgszFileNew[((iCurVal >> 0x2) * 0xd)]));

L_91d3:
    lSaltCur = vplr.lSalt;
    lSaltLast = 0x0;
    if ((FCheckPassword() == 0x0))
        goto FinishClick;
    else
        goto L_91f7;

L_91f7:

L_91fd:
    if ((LOWORD(vplr.lSalt) != 0x0))
        goto L_9211;
    else
        goto L_9207;

L_9207:
    if ((HIWORD(vplr.lSalt) == 0x0))
        goto L_9224;
    else
        goto L_9211;

L_9211:
    strcpy(szRacePass, szPassLast);
    goto L_9229;

L_9224:
    szRacePass[0x0] = 0x0;

L_9229:
    prcSav = vrgrcRCW;
    if ((RaceCreationWizard(hwnd, 0x0, 0x0) == 0x0))
        goto L_92e4;
    else
        goto L_924a;

L_924a:
    vrgrcRCW = prcSav;
    if (((iCurVal & 0x3) == 0x1))
        goto PlaceNew;
    else
        goto L_925e;

L_925e:
    if ((strcmp(szRaceFile, &(vrgszFileNew[((iCurVal >> 0x2) * 0xd)])) != 0x0))
        goto PlaceNew;
    else
        goto L_9282;

L_9282:

L_9288:
    vrgplrNew[(iCurVal >> 0x2)] = vplr;
    strcpy(&(vrgszFileNew[((iCurVal >> 0x2) * 0xd)]), szRaceFile);
    iNewVal = iCurVal;
    iCurVal = 0xffff;

L_92e4:
    vrgrcRCW = prcSav;
    goto L_9367;

L_92ed:
    if ((iPopMenuSel != 0x3))
        goto L_92ff;
    else
        goto L_92f7;

L_92f7:
    iNewVal = 0x0;
    goto L_9367;

L_92ff:
    if ((iPopMenuSel < 0x10))
        goto L_9326;
    else
        goto L_9309;

L_9309:
    if ((iPopMenuSel >= 0x20))
        goto L_9326;
    else
        goto L_9313;

L_9313:
    iNewVal = (((iPopMenuSel + 0xfff0) << 0x2) | 0x1);
    goto L_9367;

L_9326:
    if ((iPopMenuSel < 0x20))
        goto L_934d;
    else
        goto L_9330;

L_9330:
    if ((iPopMenuSel >= 0x30))
        goto L_934d;
    else
        goto L_933a;

L_933a:
    iNewVal = (((iPopMenuSel + 0xffe0) << 0x2) | 0x2);
    goto L_9367;

L_934d:
    if ((iPopMenuSel < 0x30))
        goto L_9367;
    else
        goto L_9357;

L_9357:
    iNewVal = (((iPopMenuSel + 0xffd0) << 0x2) | 0x3);

L_9367:
    if ((iNewVal == 0xffff))
        goto FinishClick;
    else
        goto L_9370;

L_9370:
    if ((iNewVal == iCurVal))
        goto FinishClick;
    else
        goto L_937b;

L_937b:
    vrgplrTypeNew[iDiamond] = LOBYTE(iNewVal);
    if ((iNewVal != 0x0))
        goto L_93ab;
    else
        goto L_938e;

L_938e:
    iDiamond = (iDiamond + 0x1);
    if ((iDiamond >= 0x10))
        goto L_93e2;
    else
        goto L_939d;

L_939d:
    vrgplrTypeNew[iDiamond] = 0x0;
    goto L_938e;

L_93ab:
    if ((iDiamond <= 0x0))
        goto L_93e2;
    else
        goto L_93b4;

L_93b4:
    iDiamond = (iDiamond - 0x1);
    if ((iDiamond < 0x0))
        goto L_93e2;
    else
        goto L_93c3;

L_93c3:
    if ((vrgplrTypeNew[iDiamond] != 0x0))
        goto L_93e2;
    else
        goto L_93d5;

L_93d5:
    vrgplrTypeNew[iDiamond] = LOBYTE(iNewVal);
    goto L_93b4;

L_93e2:
    InvalidateRect(hwnd, 0x0, 0x1);

FinishClick:
    SetFocus(hwnd);
    goto L_9548;

L_9401:
    hdc = BeginPaint(hwnd, &(ps));
    DrawNewGame2(hwnd, hdc, 0xffff);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_943a:
    if ((wParam != 0x76))
        goto L_9464;
    else
        goto L_9443;

L_9443:
    WinHelp(hwnd, szHelpFile, 0x1, 0x3fc);
    return 0x1;

L_9464:
    i = 0x0;
    goto L_9484;

L_946c:
    if ((wParam == rgidRaceBtn[i]))
        goto L_948d;
    else
        goto L_947a;

L_947a:

L_9480:
    i = (i + 0x1);

L_9484:
    if ((i < 0x4))
        goto L_946c;
    else
        goto L_948d;

L_948d:
    if ((i >= 0x4))
        goto L_9548;
    else
        goto L_9496;

L_9496:
    if ((i == 0x0))
        goto L_94db;
    else
        goto L_949f;

L_949f:
    if (((LOWORD(vrgplrTypeNew) & 0xff) != 0x0))
        goto L_94db;
    else
        goto L_94ad;

L_94ad:
    if ((fRCWReadOnly != 0x0))
        goto L_94db;
    else
        goto L_94b7;

L_94b7:
    AlertSz(PszFormatIds(idsMustHaveLeastOnePlayerGame, 0x0), MB_ICONHAND);
    goto L_9548;

L_94db:
    StickyDlgPos(hwnd, &(ptStickyNewDlg), 0x0);
    EndDialog(hwnd, i);
    return 0x1;

L_9505:
    if ((message == WM_PAINT))
        goto L_9401;
    else
        goto L_950d;

L_950d:
    if ((message == WM_ERASEBKGND))
        goto L_893c;
    else
        goto L_9515;

L_9515:
    if ((message == WM_CTLCOLOR))
        goto L_8964;
    else
        goto L_951d;

L_951d:
    if ((message == WM_SETCURSOR))
        goto L_89da;
    else
        goto L_9525;

L_9525:
    if ((message == WM_INITDIALOG))
        goto L_87e1;
    else
        goto L_952d;

L_952d:
    if ((message == WM_COMMAND))
        goto L_943a;
    else
        goto L_9535;

L_9535:
    if ((message == WM_LBUTTONDOWN))
        goto L_8a76;
    else
        goto L_953d;

L_953d:
    if ((message == WM_RBUTTONDOWN))
        goto L_8a76;
    else
        goto L_9548;

L_9548:
    return 0x0;
}

void DrawNewGame2(HWND hwnd, HDC hdc, int16_t iDraw) {
    int16_t  fCreatedDC;
    int16_t  yCur;
    int16_t  i;
    int16_t  iPlr;
    int16_t  bkMode;
    int16_t  cch;
    RECT     rcDiamond;
    RECT     rc;
    StringId ids;
    char     szT[20];
    uint16_t t_merge_97a0_0001;
    uint16_t t_merge_989d_0001;

L_9556:
    fCreatedDC = 0x0;
    if ((hdc != 0x0))
        goto L_957d;
    else
        goto L_956d;

L_956d:
    fCreatedDC = 0x1;
    hdc = GetDC(hwnd);

L_957d:
    GetClientRect(hwnd, &(rc));
    bkMode = SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, rghfontArial8[0x1]);
    xNewGameDiamond = (LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsPlayer16), 0xb)) + 0xc);
    yCur = 0x6;
    SetRect(&(rcDiamond), xNewGameDiamond, yCur, ((xNewGameDiamond + dyArial8) + 0x1), ((yCur + dyArial8) + 0x1));
    i = 0x0;
    goto L_99c5;

L_95ff:
    if ((fRCWReadOnly == 0x0))
        goto L_9617;
    else
        goto L_9609;

L_9609:
    if ((i >= game.cPlayer))
        goto L_99ce;
    else
        goto L_9611;

L_9611:

L_9617:
    cch = _wsprintf(szWork, PszGetCompressedString(idsPlayerD), (i + 0x1));
    cch = (cch + 0x1);
    szWork[cch] = 0x3a;
    szWork[cch] = 0x0;
    RightTextOut(hdc, (xNewGameDiamond + 0xfffa), yCur, szWork, cch, 0x0);
    DrawDiamond(hdc, &(rcDiamond), hbrBBlue);
    iPlr = ((vrgplrTypeNew[i] >> 0x2) & 0xf);
    if ((fRCWReadOnly == 0x0))
        goto L_9752;
    else
        goto L_96a5;

L_96a5:
    if ((((rgplr[i].wMdPlr >> 0x8) & 0x1) == 0x0))
        goto L_973f;
    else
        goto L_96c3;

L_96c3:
    PszPlayerName(i, 0x1, 0x1, 0x1, 0x0, 0x0);
    if (((rgplr[i].wFlags & 0x1) == 0x0))
        goto DisplayName;
    else
        goto L_96fb;

L_96fb:
    _wsprintf(&(szWork[strlen(szWork)]), " (%s)", PszGetCompressedString(idsDeceased));
    SetTextColor(hdc, 0x7f);

L_973f:
    CchGetString(idsUnknownPlayer, szWork);

L_9752:
    goto L_9948;

L_9762:
    CchGetString(idsPlayer2, szWork);
    goto DisplayName;

L_9775:
    if ((iPlr < 0x6))
        goto L_97b5;
    else
        goto L_977e;

L_977e:
    if ((iPlr != 0x6))
        goto L_979d;
    else
        goto L_9787;

L_9787:
    if ((fRCWReadOnly == 0x0))
        goto L_9797;
    else
        goto L_9791;

L_9791:
    t_merge_97a0_0001 = 0x20c;
    goto L_97a0;

L_9797:
    t_merge_97a0_0001 = 0x20b;

L_979d:
    t_merge_97a0_0001 = 0x20d;

L_97a0:
    ids = t_merge_97a0_0001;
    CchGetString(ids, szWork);
    goto DisplayName;

L_97b5:
    CchGetString(idsS, szT);
    _wsprintf(szWork, szT, PszGetCompressedString((iPlr + 0x567)));

L_97f1:
    if ((fRCWReadOnly != 0x0))
        goto L_984e;
    else
        goto L_97fb;

L_97fb:
    if ((gd.fNoHostNames == 0x0))
        goto L_984e;
    else
        goto L_980e;

L_980e:
    /* untranslated: branch sext8to16(byte ds:[(vrgszFileNew + loword((0xd * iPlr)))]) == 0x0 ? L_984e : L_9825 */

L_9825:
    _wsprintf(szWork, " %s", ((uint8_t *)(vrgszFileNew) + LOWORD((0xd * iPlr))));
    goto DisplayName;

L_984e:
    /* untranslated: branch sext8to16(byte ds:[(vrgszFileNew + loword((0xd * iPlr)))]) == 0x0 ? L_98bc : L_9865 */

L_9865:
    if ((fRCWReadOnly == 0x0))
        goto L_989a;
    else
        goto L_9894;

L_9894:
    t_merge_989d_0001 = 0x310;
    goto L_989d;

L_989a:
    t_merge_989d_0001 = 0x311;

L_989d:
    _wsprintf(szWork, PszGetCompressedString(t_merge_989d_0001), ((uint8_t *)(vrgplrNew) + (LOWORD((0xc0 * iPlr)) + 0xa0)),
              ((uint8_t *)(vrgszFileNew) + LOWORD((0xd * iPlr))));
    goto DisplayName;

L_98bc:
    _wsprintf(szWork, PszGetCompressedString(idsS), ((uint8_t *)(vrgplrNew) + (LOWORD((0xc0 * iPlr)) + 0xa0)));

L_98f3:
    _wsprintf(szWork, PszGetCompressedString(idsSSComputerPlayer), vrgszComputerPlayers[(iPlr & 0x7)], vrgszComputerLevel[(vrgplrTypeNew[i] >> 0x5)]);
    goto DisplayName;

L_9948:
    if (((vrgplrTypeNew[i] & 0x3) == 0x0))
        goto L_9762;
    else
        goto L_9950;

L_9950:
    if (((vrgplrTypeNew[i] & 0x3) == 0x1))
        goto L_9775;
    else
        goto L_9958;

L_9958:
    if (((vrgplrTypeNew[i] & 0x3) == 0x2))
        goto L_97f1;
    else
        goto L_9960;

L_9960:
    if (((vrgplrTypeNew[i] & 0x3) == 0x3))
        goto L_98f3;
    else
        goto DisplayName;

DisplayName:
    TextOut(hdc, (rcDiamond.right + 0x6), yCur, szWork, strlen(szWork));
    SetTextColor(hdc, crButtonText);
    OffsetRect(&(rcDiamond), 0x0, (dyArial8 + 0x4));
    yCur = (yCur + (dyArial8 + 0x4));
    i = (i + 0x1);

L_99c5:
    if ((i < 0x10))
        goto L_95ff;
    else
        goto L_99ce;

L_99ce:
    SetBkMode(hdc, bkMode);
    if ((fCreatedDC == 0x0))
        goto L_99ed;
    else
        goto L_99e2;

L_99e2:
    ReleaseDC(hwnd, hdc);

L_99ed:
    return;
}

int16_t NewGameDlg3(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t     i;
    RECT        rc;
    POINT       pt;
    HDC         hdc;
    PAINTSTRUCT ps;
    uint16_t    t_merge_9a40_0001;
    uint16_t    t_merge_9ccc_0001;

L_99f4:
    goto L_9cfb;

L_9a03:
    SetNGWTitle(hwnd, 0x3);
    i = 0x0;
    goto L_9a85;

L_9a1a:
    GetDlgItem(hwnd, (i + 0x123));
    if ((i < 0x2))
        goto L_9a3d;
    else
        goto L_9a37;

L_9a37:
    t_merge_9a40_0001 = 0x1;
    goto L_9a40;

L_9a3d:
    t_merge_9a40_0001 = 0x0;

L_9a40:
    /* untranslated: call SendMessage(callresult(HWND), CB_LIMITTEXT, GetVCCheck(&game, (t_merge_9a40_0001 + i)), 0x0) -> callresult(LRESULT) */
    if ((fRCWReadOnly == 0x0))
        goto L_9a81;
    else
        goto L_9a68;

L_9a68:
    EnableWindow(GetDlgItem(hwnd, (i + 0x123)), 0x0);

L_9a81:
    i = (i + 0x1);

L_9a85:
    if ((i < 0x7))
        goto L_9a1a;
    else
        goto L_9a8e;

L_9a8e:
    StickyDlgPos(hwnd, &(ptStickyNewDlg), 0x1);
    return 0x1;

L_9aa7:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_9acf:
    i = 0x123;
    goto L_9afc;

L_9ad7:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] == GetDlgItem(hwnd, i) ? L_9b06 : L_9af2 */

L_9af2:

L_9af8:
    i = (i + 0x1);

L_9afc:
    if ((i <= 0x129))
        goto L_9ad7;
    else
        goto L_9b06;

L_9b06:
    if ((i <= 0x129))
        goto L_9b2c;
    else
        goto L_9b10;

L_9b10:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_9d3e;
    else
        goto L_9b2c;

L_9b2c:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_9b45:
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    /* untranslated: branch IrcRaceDlgHitTest(words(pt.y, pt.x)) < 0x0 ? L_9d3e : L_9b76 */

L_9b76:
    SetCursor(hcurHand);
    return 0x1;

L_9b88:
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    /* untranslated: return FTrackNewGameDlg3(hwnd, words(pt.y, pt.x), wParam) */

L_9bc0:
    hdc = BeginPaint(hwnd, &(ps));
    DrawNewGame3(hwnd, hdc, 0xffff);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_9bf9:
    if ((wParam != 0x76))
        goto L_9c23;
    else
        goto L_9c02;

L_9c02:
    WinHelp(hwnd, szHelpFile, 0x1, 0x3fd);
    return 0x1;

L_9c23:
    i = 0x0;
    goto L_9c43;

L_9c2b:
    if ((wParam == rgidRaceBtn[i]))
        goto L_9c4c;
    else
        goto L_9c39;

L_9c39:

L_9c3f:
    i = (i + 0x1);

L_9c43:
    if ((i < 0x4))
        goto L_9c2b;
    else
        goto L_9c4c;

L_9c4c:
    if ((i >= 0x4))
        goto L_9c79;
    else
        goto L_9c55;

L_9c55:
    StickyDlgPos(hwnd, &(ptStickyNewDlg), 0x0);
    EndDialog(hwnd, i);
    return 0x1;

L_9c79:
    if ((wParam < 0x123))
        goto L_9d3e;
    else
        goto L_9c83;

L_9c83:
    if ((wParam > 0x129))
        goto L_9d3e;
    else
        goto L_9c8d;

L_9c8d:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, wParam), WM_USER, 0x0, 0x0));
    if (((wParam + 0xfedd) < 0x2))
        goto L_9cc9;
    else
        goto L_9cc3;

L_9cc3:
    t_merge_9ccc_0001 = 0x1;
    goto L_9ccc;

L_9cc9:
    t_merge_9ccc_0001 = 0x0;

L_9ccc:
    SetVCCheck(&(game), ((wParam + 0xfedd) + t_merge_9ccc_0001), i);
    DrawNewGame3(hwnd, 0x0, 0x8);

L_9cfb:
    if ((message == WM_PAINT))
        goto L_9bc0;
    else
        goto L_9d03;

L_9d03:
    if ((message == WM_ERASEBKGND))
        goto L_9aa7;
    else
        goto L_9d0b;

L_9d0b:
    if ((message == WM_CTLCOLOR))
        goto L_9acf;
    else
        goto L_9d13;

L_9d13:
    if ((message == WM_SETCURSOR))
        goto L_9b45;
    else
        goto L_9d1b;

L_9d1b:
    if ((message == WM_INITDIALOG))
        goto L_9a03;
    else
        goto L_9d23;

L_9d23:
    if ((message == WM_COMMAND))
        goto L_9bf9;
    else
        goto L_9d2b;

L_9d2b:
    if ((message == WM_LBUTTONDOWN))
        goto L_9b88;
    else
        goto L_9d33;

L_9d33:
    if ((message == WM_LBUTTONDBLCLK))
        goto L_9b88;
    else
        goto L_9d3e;

L_9d3e:
    return 0x0;
}

void DrawNewGame3(HWND hwnd, HDC hdc, int16_t iDraw) {
    int16_t  yTop;
    int16_t  bt;
    int16_t  vcCur;
    int16_t  irc;
    StringId ids;
    int16_t  fCreatedDC;
    int16_t  i;
    int16_t  dxItem;
    RECT     rcCBox;
    int16_t  j;
    COLORREF crBkSav;
    int16_t  bkMode;
    int16_t  dxDig;
    int16_t  xLeft;
    int16_t  cch;
    RECT     rc;
    uint16_t t_merge_9d7f_0001;
    StringId t_9ebb;

L_9d5e:
    fCreatedDC = 0x0;
    if ((fRCWReadOnly == 0x0))
        goto L_9d7c;
    else
        goto L_9d76;

L_9d76:
    t_merge_9d7f_0001 = 0x4;
    goto L_9d7f;

L_9d7c:
    t_merge_9d7f_0001 = 0x0;

L_9d7f:
    bt = t_merge_9d7f_0001;
    if ((hdc != 0x0))
        goto L_9d9b;
    else
        goto L_9d8b;

L_9d8b:
    fCreatedDC = 0x1;
    hdc = GetDC(hwnd);

L_9d9b:
    GetClientRect(hwnd, &(rc));
    bkMode = SetBkMode(hdc, OPAQUE);
    crBkSav = SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[0x1]);
    yTop = (LOWORD((0x3 * dyArial8)) + 0x6);
    dxDig = LOWORD(GetTextExtent(hdc, "9", 0x1));
    ids = idsOwns;
    irc = 0x0;
    vcCur = 0x0;
    i = 0x0;
    goto L_a1cd;

L_9e15:
    if ((i >= 0x7))
        goto L_9e7e;
    else
        goto L_9e1e;

L_9e1e:
    GetWindowRect(GetDlgItem(hwnd, (i + 0x123)), &(rcCBox));
    MapWindowPoints(0x0, hwnd, &(rcCBox), 0x2);
    xLeft = (rcCBox.right + 0x2);
    yTop = ((((uint32_t)((rcCBox.bottom - rcCBox.top)) / 0x2) + rcCBox.top) - (dyArial8 >> 0x1));
    goto L_9eb7;

L_9e7e:
    if ((i != 0x7))
        goto L_9e99;
    else
        goto L_9e87;

L_9e87:
    xLeft = rcCBox.left;
    yTop = (rcCBox.bottom + 0x6);
    goto L_9eb7;

L_9e99:
    xLeft = rcCBox.left;
    /* untranslated: yTop = ((rcCBox.bottom + 0x6) + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */

L_9eb7:
    t_9ebb = ids;
    ids = (ids + 0x1);
    cch = CchGetString(t_9ebb, szWork);
    if ((iDraw != 0xffff))
        goto L_9eef;
    else
        goto L_9ed7;

L_9ed7:
    TextOut(hdc, xLeft, yTop, szWork, cch);

L_9eef:
    xLeft = (xLeft + LOWORD(GetTextExtent(hdc, szWork, cch)));
    j = 0x0;
    goto L_a1c0;

L_9f0c:
    /* untranslated: dxItem = loword((abs(sext8to16(byte cs:[((0x9d4c + (i * 0x2)) + j)])) * dxDig)) */
    if ((dxItem != 0x0))
        goto L_9f42;
    else
        goto L_9f3b;

L_9f3b:
    ids = (ids + 0x1);
    goto L_a1c9;

L_9f42:
    _wsprintf(szWork, PCTD, GetVCVal(&(game), vcCur, 0x0));
    /* untranslated: branch sext8to16(byte cs:[((0x9d4c + (i * 0x2)) + j)]) >= 0x0 ? L_9faa : L_9f8b */

L_9f8b:
    /* untranslated: dxItem = (dxItem + (words(loword((0x3 * dxDig)), signhiword(loword((0x3 * dxDig)))) / 0x2)) */
    strcat(szWork, "%");

L_9faa:
    xLeft = (xLeft + dxItem);
    if ((iDraw == 0xffff))
        goto L_9fcd;
    else
        goto L_9fb9;

L_9fb9:
    if ((iDraw == vcCur))
        goto L_9fcd;
    else
        goto L_9fc4;

L_9fc4:
    if ((vcCur != 0x8))
        goto L_9fe9;
    else
        goto L_9fcd;

L_9fcd:
    RightTextOut(hdc, xLeft, yTop, szWork, 0x0, dxItem);

L_9fe9:
    vcCur = (vcCur + 0x1);
    vrgrcRCW[irc].left = (xLeft + 0x4);
    vrgrcRCW[irc].top = (yTop + 0xfffd);
    vrgrcRCW[irc].right = (vrgrcRCW[irc].left + 0xf);
    vrgrcRCW[irc].bottom = (((dyArial8 >> 0x1) + vrgrcRCW[irc].top) + 0x3);
    vrgrcRCW[(irc + 0x1)] = vrgrcRCW[irc];
    OffsetRect(((uint8_t *)(vrgrcRCW) + ((irc + 0x1) * 0x8)), 0x0, ((vrgrcRCW[irc].bottom - vrgrcRCW[irc].top) + 0xffff));
    if ((iDraw != 0xffff))
        goto L_a154;
    else
        goto L_a0fd;

L_a0fd:
    DrawBtn(hdc, &(vrgrcRCW[irc]), (0xa0 | bt), 0x0, 0x0);
    DrawBtn(hdc, &(vrgrcRCW[(irc + 0x1)]), (0xa1 | bt), 0x0, 0x0);

L_a154:
    xLeft = (vrgrcRCW[irc].right + 0x4);
    cch = CchGetString(ids, szWork);
    if ((iDraw != 0xffff))
        goto L_a19f;
    else
        goto L_a187;

L_a187:
    TextOut(hdc, xLeft, yTop, szWork, cch);

L_a19f:
    xLeft = (xLeft + LOWORD(GetTextExtent(hdc, szWork, cch)));
    ids = (ids + 0x1);
    irc = (irc + 0x2);
    j = (j + 0x1);

L_a1c0:
    if ((j < 0x2))
        goto L_9f0c;
    else
        goto L_a1c9;

L_a1c9:
    i = (i + 0x1);

L_a1cd:
    if ((i < 0x9))
        goto L_9e15;
    else
        goto L_a1d6;

L_a1d6:
    LOWORD(crcRCW) = irc;
    SetBkColor(hdc, crBkSav);
    SetBkMode(hdc, bkMode);
    if ((fCreatedDC == 0x0))
        goto L_a209;
    else
        goto L_a1fe;

L_a1fe:
    ReleaseDC(hwnd, hdc);

L_a209:
    return;
}

int16_t FTrackNewGameDlg3(HWND hwnd, POINT pt, int16_t kbd) {
    int16_t bt;
    int16_t irc;
    BTNT    btnt;
    int16_t i;
    int16_t dShift;
    int16_t iMod;
    int16_t iStat;

L_a210:
    /* untranslated: irc = IrcRaceDlgHitTest(words(pt.y, pt.x)) */
    if ((irc >= 0x0))
        goto L_a239;
    else
        goto L_a233;

L_a233:
    return 0x0;

L_a239:
    iMod = (irc & 0x1);
    i = (irc >> 0x1);
    if ((iMod != 0x0))
        goto L_a260;
    else
        goto L_a253;

L_a253:
    dShift = 0x1;
    bt = 0xa0;
    goto L_a26a;

L_a260:
    dShift = 0xffff;
    bt = 0xa1;

L_a26a:
    InitBtnTrack(&(btnt), hwnd, 0x0, &(vrgrcRCW[irc]), bt, 0x50, 0x0, 0x0, 0x0);
    if (((kbd & 0xc) == 0x0))
        goto L_a2b7;
    else
        goto L_a2ae;

L_a2ae:
    dShift = LOWORD((0x5 * dShift));

L_a2b7:
    if ((FTrackBtn(&(btnt)) == 0x0))
        goto L_a313;
    else
        goto L_a2cb;

L_a2cb:
    iStat = GetVCVal(&(game), i, 0x1);
    if ((SetVCVal(&(game), i, (iStat + dShift)) == iStat))
        goto L_a2b7;
    else
        goto L_a2ff;

L_a2ff:
    DrawNewGame3(hwnd, btnt.hdc, i);

L_a313:
    return 0x1;
}

void SetNGWTitle(HWND hwnd, int16_t iStep) {
    int16_t cch;
    char    szBuf[50];

L_a320:
    cch = CchGetString((fRCWReadOnly + 0x110), szBuf);
    cch = _wsprintf(szWork, szBuf, iStep);
    SetWindowText(hwnd, szWork);
    return;
}

PLAYER *LpplrComp(int16_t idAi, int16_t lvlAi) {
L_b570:

L_b5a1:
    return &(vrgplrComp[idAi][lvlAi]);
}

void SetVCCheck(GAME *pgame, int16_t vc, int16_t fChecked) {
    uint16_t t_merge_b5d7_0001;

L_b5bc:
    if ((fChecked == 0x0))
        goto L_b5d4;
    else
        goto L_b5ce;

L_b5ce:
    t_merge_b5d7_0001 = 0x80;
    goto L_b5d7;

L_b5d4:
    t_merge_b5d7_0001 = 0x0;

L_b5d7:
    pgame->rgvc[vc] = LOBYTE(((pgame->rgvc[vc] & 0x7f) | t_merge_b5d7_0001));
    return;
}

int16_t GetVCCheck(GAME *pgame, int16_t vc) {
L_b60c:
    if (((pgame->rgvc[vc] & 0x80) == 0x0))
        goto L_b638;
    else
        goto L_b632;

L_b632:
    return 0x1;

L_b638:
    return 0x0;
}

int16_t SetVCVal(GAME *pgame, int16_t vc, int16_t val) {
    int16_t cur;

L_b644:
    if ((val >= 0x0))
        goto L_b65e;
    else
        goto L_b656;

L_b656:
    val = 0x0;
    goto L_b67d;

L_b65e:
    /* untranslated: branch val <= cs:[(vc * 0x2)-0x4a58] ? L_b67d : L_b670 */

L_b670:
    /* untranslated: val = cs:[(vc * 0x2)-0x4a58] */

L_b67d:
    pgame->rgvc[vc] = LOBYTE(((pgame->rgvc[vc] & 0x80) | (val & 0xff)));
    if ((vc != 0x8))
        goto L_b703;
    else
        goto L_b6ba;

L_b6ba:
    cur = GetVCVal(pgame, 0x8, 0x0);
    if ((cur == val))
        goto L_b703;
    else
        goto L_b6db;

L_b6db:
    val = cur;
    pgame->rgvc[0x8] = LOBYTE(((pgame->rgvc[0x8] & 0x80) | (cur & 0xff)));

L_b703:

L_b709:
    return val;
}

int16_t GetVCVal(GAME *pgame, int16_t vc, int16_t fRaw) {
    int16_t c;
    int16_t i;
    int16_t val;

L_b710:
    val = (pgame->rgvc[vc] & 0x7f);
    if ((fRaw == 0x0))
        goto L_b740;
    else
        goto L_b73a;

L_b73a:
    return val;

L_b740:
    goto L_b80e;

L_b7b8:
    c = 0x0;
    i = 0x0;
    goto L_b7ee;

L_b7c5:
    if ((i == 0x2))
        goto L_b7ea;
    else
        goto L_b7ce;

L_b7ce:
    if (((game.rgvc[i] & 0x80) == 0x0))
        goto L_b7ea;
    else
        goto L_b7e6;

L_b7e6:
    c = (c + 0x1);

L_b7ea:
    i = (i + 0x1);

L_b7ee:
    if ((i < 0x8))
        goto L_b7c5;
    else
        goto L_b7f7;

L_b7f7:
    if ((c >= val))
        goto L_b833;
    else
        goto L_b802;

L_b802:
    val = c;

L_b80e:
    if ((vc > 0x9))
        goto L_b7b8;
    else
        goto L_b816;

L_b816:
    goto L_ffffffff;

L_b833:
    return val;
}
