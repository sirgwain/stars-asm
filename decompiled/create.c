#include "common.h"

int16_t vrgvcMax[10] = {16, 18, 4, 19, 28, 49, 29, 87, 7, 47};
char    rgNG3Width[9][2] = {{-3}, {2, 1}, {5}, {-3}, {3}, {3}, {3}, {1}, {3}};
uint8_t vrgWormholeMin[5] = {0, 1, 1, 3, 4};
BTLPLAN rgbtlplanT[5] = {{
                             .mdTactic = mdTacticMaxDamageRatio,
                             .mdTarget1 = mdTargetArmedShips,
                             .mdTarget2 = mdTargetAny,
                             .iplrAttack = iplrAttackNeutralsEnemies,
                             .szName = "Default",
                         },
                         {
                             .iplan = 1,
                             .mdTactic = mdTacticMaxDamageRatio,
                             .mdTarget1 = mdTargetStarbase,
                             .mdTarget2 = mdTargetArmedShips,
                             .iplrAttack = iplrAttackNeutralsEnemies,
                             .szName = "Kill Starbase",
                         },
                         {
                             .iplan = 2,
                             .mdTactic = mdTacticMaxNetDamage,
                             .mdTarget1 = mdTargetArmedShips,
                             .mdTarget2 = mdTargetBombersFreighters,
                             .iplrAttack = iplrAttackNeutralsEnemies,
                             .szName = "Max-Defense",
                         },
                         {
                             .iplan = 3,
                             .mdTactic = mdTacticDisengageIfChallenged,
                             .mdTarget1 = mdTargetUnarmedShips,
                             .iplrAttack = iplrAttackNeutralsEnemies,
                             .szName = "Sniper",
                         },
                         {
                             .iplan = 4,
                             .mdTarget1 = mdTargetAny,
                             .iplrAttack = iplrAttackNeutralsEnemies,
                             .szName = "Chicken",
                         }};
PLAYER  vrgplrComp[6][4] = {{{
                                 .iPlayer = -1,
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
                                 .det = detAll,
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
    *lpbtlplan = rgbtlplanT[iplan];
    lpbtlplan->iplr = iplr;
    if (game.fSinglePlr != 0 && iplan == 0) {
        lpbtlplan->iplrAttack = iplrAttackEveryone;
    }
    return;
}

int16_t GenerateWorld(int16_t fBatchMode) {
    int32_t      *pl;
    int16_t       iBest;
    int16_t       cKill;
    char          grUsed[128];
    jmp_buf      *penvMemSav;
    POINT16      *ppt;
    int16_t       raMajor;
    int16_t       k;
    POINT16       pt;
    int16_t       fFound;
    int16_t       iMax;
    STARPACK      starpack;
    int16_t       dy;
    int16_t       dGalMinSq;
    int16_t       iLow;
    PLANET       *lppl;
    int16_t       iMin;
    int16_t       i;
    jmp_buf       env;
    int16_t       xOld;
    int16_t       iplrSingle;
    POINT16      *pptMax;
    int16_t       dMin;
    int16_t       ktLeft;
    SHDEF        *lpshdef;
    int32_t       lDistMax2;
    int32_t       lDistIdeal2;
    int16_t       rgi[16];
    int16_t       iNewLine;
    uint8_t      *pb;
    int16_t       dMax;
    int32_t       lDistMin2;
    int16_t       j;
    int16_t       cPlanMax;
    int16_t       dx;
    int16_t       cKillMax;
    POINT16      *pptT;
    int32_t       lBest;
    int32_t       l;
    int16_t       iT;
    int16_t       jj;
    int16_t       iTechMin;
    int16_t       pct10;
    int16_t       idHome;
    int16_t       ishRet;
    PART          part;
    int16_t       cFit;
    PLANET       *lpplClosest;
    POINT16       ptHome;
    PLANET       *lpplPicked;
    int32_t       lDistCur2;
    HS           *lphs;
    int16_t       chs;
    int16_t       cTry;
    int16_t       rgTry[5];
    THING        *lpth;
    uint16_t      idLast;
    THING        *lpthLast;
    char          szExt[4];
    POINT16      *t_03f1;
    int16_t       t_call_0a1b;
    int16_t       t_scratch_m116_2;
    RaceAttribute t_call_1734;
    uint16_t      t_scratch_m116_15;
    uint16_t      t_scratch_m116_17;
    uint16_t      t_scratch_m116_19;
    int16_t       t_call_361d;
    HS           *t_fields_1;
    uint32_t      t_fields_2;
    uint32_t      t_fields_3;
    int16_t       t_scratch_m116_24;
    uint16_t      t_scratch_m120;

    iMin = 0;
    cKill = 0;
    dGal = 400 * game.mdSize + 400;
    dGalInv = dGal + 2000;
    cPlanMax = LOWORD((int32_t)(dGal * dGal) / 5000);
    cPlanMax += cPlanMax / 4 * (game.mdDensity - 1);
    if (game.mdDensity >= 3) {
        cPlanMax += cPlanMax / 4;
    }
    cPlanMax = cPlanMax >= 999 ? 999 : cPlanMax;
    dGalMinSq = dGalMinDist * dGalMinDist;
    iMax = cPlanMax / 7 + cPlanMax >= 999 ? 999 : cPlanMax / 7 + cPlanMax;
    dx = 1010;
    dy = dGal - 19;
    for (i = 0; i < iMax; i++) {
        rgptPlan[i].x = Random(dy) + dx;
        rgptPlan[i].y = Random(dy) + dx;
    }
    qsort(rgptPlan, iMax, sizeof(POINT16), (QSORTCOMPARE)ICompLong);
    pptMax = &rgptPlan[iMax];
    for (ppt = rgptPlan; ppt < pptMax; ppt++) {
        if (ppt->y >= 0) {
            pptT = ppt + 1;
            iNewLine = ppt->x + dGalMinDist;
            for (; pptT < pptMax && pptT->x <= iNewLine; pptT++) {
                dy = abs(ppt->y - pptT->y);
                if (dy <= dGalMinDist) {
                    dx = ppt->x - pptT->x;
                    if (dx * dx + dy * dy <= dGalMinSq) {
                        pptT->y = -100;
                        cKill++;
                    }
                }
            }
        }
    }
    cKillMax = iMax - cPlanMax;
    while (cKill < cKillMax) {
        i = Random(iMax);
        if (rgptPlan[i].y >= 0) {
            rgptPlan[i].y = -100;
            cKill++;
        }
    }
    pptT = rgptPlan;
    for (ppt = rgptPlan; ppt < pptMax; ppt++) {
        if (ppt->y >= 0) {
            t_03f1 = pptT;
            pptT++;
            t_03f1->x = ppt->x;
            t_03f1->y = ppt->y;
        }
    }
    cPlanMax = iMax - cKill;
    if (game.fClumping != 0) {
        for (i = 0; i < cPlanMax; i++) {
            lBest = 10000000;
            iBest = 0;
            j = Random(cPlanMax);
            pt = rgptPlan[j];
            for (k = 0; k < cPlanMax; k++) {
                if (k != j) {
                    dx = pt.x - rgptPlan[k].x;
                    dy = pt.y - rgptPlan[k].y;
                    l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                    if (l < lBest) {
                        lBest = l;
                        iBest = k;
                    }
                }
            }
            if (lBest > 144) {
                if (lBest > 1600) {
                    rgptPlan[j].x = (int16_t)(rgptPlan[iBest].x * 2 + rgptPlan[j].x) / 3;
                    rgptPlan[j].y = (int16_t)(rgptPlan[iBest].y * 2 + rgptPlan[j].y) / 3;
                } else if (lBest > 625) {
                    rgptPlan[j].x = (int16_t)(rgptPlan[j].x + rgptPlan[iBest].x) / 2;
                    rgptPlan[j].y = (int16_t)(rgptPlan[j].y + rgptPlan[iBest].y) / 2;
                } else if (lBest > 324) {
                    rgptPlan[j].x = (int16_t)(rgptPlan[j].x * 2 + rgptPlan[iBest].x) / 3;
                    rgptPlan[j].y = (int16_t)(rgptPlan[j].y * 2 + rgptPlan[iBest].y) / 3;
                } else {
                    rgptPlan[j].x = (int16_t)(rgptPlan[j].x * 4 + rgptPlan[iBest].x) / 5;
                    rgptPlan[j].y = (int16_t)(rgptPlan[j].y * 4 + rgptPlan[iBest].y) / 5;
                }
            }
        }
        qsort(rgptPlan, cPlanMax, sizeof(POINT16), (QSORTCOMPARE)ICompLong);
    }
    memset(grUsed, 0, 128);
    for (i = 0; i < cPlanMax; i++) {
        dx = Random(999);
        while (((int16_t)(int8_t)grUsed[dx >> 3] & bitTbl[dx & 7]) != 0) {
            dx++;
            if (dx >= game.fTutorial + 999) {
                dx = 0;
            }
        }
        grUsed[dx >> 3] = grUsed[dx >> 3] | LOBYTE(bitTbl[dx & 7]);
        rgidPlan[i] = dx;
    }
    cPlanet = cPlanMax;
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0) {
        DestroyCurGame();
        return 0;
    }
    lpPlanets = LpAlloc(cPlanMax * sizeof(PLANET), htPlanets);
    fmemset(lpPlanets, 0, cPlanMax * sizeof(PLANET));
    i = 0;
    lppl = lpPlanets;
    while (i < cPlanMax) {
        lppl->id = i;
        lppl->iPlayer = -1;
        lppl->det = detAll;
        lppl->iScanner = 31;
        if (game.fNoRandom == 0) {
            lppl->fArtifact = (uint32_t)(Random(3) == 0 ? 1 : 0) & 1;
        }
        lppl->rgEnvVar[0] = LOBYTE(Random(90) + 1);
        lppl->rgEnvVar[0] += LOBYTE(Random(10));
        lppl->rgEnvVarOrig[0] = lppl->rgEnvVar[0];
        lppl->rgEnvVar[1] = LOBYTE(Random(90) + 1);
        lppl->rgEnvVar[1] += LOBYTE(Random(10));
        lppl->rgEnvVarOrig[1] = lppl->rgEnvVar[1];
        t_call_0a1b = Random(99);
        lppl->rgEnvVarOrig[2] = LOBYTE(t_call_0a1b + 1);
        lppl->rgEnvVar[2] = LOBYTE(t_call_0a1b + 1);
        if (game.fTutorial != 0) {
            if (i != 5) {
                if (i == 11) {
                    lppl->rgEnvVar[0] += 20;
                    lppl->rgEnvVarOrig[0] = lppl->rgEnvVar[0];
                }
            } else {
                for (j = 0; j < 3; j++) {
                    lppl->rgEnvVar[j] -= 5;
                    lppl->rgEnvVarOrig[j] = lppl->rgEnvVar[j];
                }
            }
        }
        for (j = 0; j < 3; j++) {
            if (game.fExtraFuel != 0) {
                lppl->rgMinConc[j] = 100;
            } else {
                lppl->rgwtMin[j] = 0;
                t_scratch_m116_2 = Random(45);
                lppl->rgMinConc[j] = LOBYTE(Random(45) + t_scratch_m116_2 + 31);
                if (lppl->rgEnvVar[2] >= 90) {
                    lppl->rgMinConc[j] += LOBYTE(Random(99 - lppl->rgMinConc[j]) / 2);
                }
            }
            lppl->rgpctMinLevel[j] = 0;
            lppl->rgwtMin[j] = 0;
            if (game.fBBSPlay != 0 && lppl->rgMinConc[j] < 40) {
                lppl->rgMinConc[j] += 5;
            }
        }
        if (game.fExtraFuel != 0) {
            iT = 100;
        } else {
            iT = Random(27);
        }
        if (iT < 18) {
            if (iT >= 9) {
                jj = Random(30);
                j = Random(3);
                lppl->rgMinConc[j] = LOBYTE(jj + 1);
            } else {
                for (iT++; iT < 16; iT *= 2) {
                    jj = Random(30);
                    j = Random(3);
                    lppl->rgMinConc[j] = LOBYTE(jj + 1);
                }
            }
        }
        i++;
        lppl++;
    }
    for (j = 0; j < 3; j++) {
        lpPlanets->rgwtMin[j] = (int16_t)(Random(lpPlanets->rgMinConc[j] * 10) + 10);
        if (lpPlanets->rgwtMin[j] < 200) {
            lpPlanets->rgwtMin[j] += (int16_t)(Random(150) + 155);
        }
        if (game.fBBSPlay != 0) {
            lpPlanets->rgwtMin[j] += (int32_t)(lpPlanets->rgwtMin[j] / 4);
        }
    }
    l = (uint32_t)(dGal * 6);
    lDistIdeal2 = (int32_t)((int32_t)(dGal * dGal) / game.cPlayer) - l;
    if (lDistIdeal2 < 0) {
        lDistIdeal2 = 0;
    } else {
        lDistIdeal2 = (int32_t)(lDistIdeal2 * 9) / 10;
    }
    lDistIdeal2 = (int32_t)(lDistIdeal2 * game.mdStartDist) / 3 + l;
    lDistMin2 = (int32_t)(lDistIdeal2 * 9) / 10;
    lDistMax2 = (int32_t)(lDistIdeal2 * 7) / 6;
    while (1) {
        lBest = 100000000;
        dMin = dGal / 4 + 1000;
        dMax = (int16_t)(3 * dGal) / 4 + 1000;
        for (i = 0; i < 50; i++) {
            rgi[0] = Random(cPlanMax);
            pt = rgptPlan[rgi[0]];
            if (pt.x < dMin) {
                dx = dMin - pt.x;
            } else if (pt.x > dMax) {
                dx = pt.x - dMax;
            } else {
                dx = 0;
            }
            if (pt.y < dMin) {
                dy = dMin - pt.y;
            } else if (pt.y > dMax) {
                dy = pt.y - dMax;
            } else {
                dy = 0;
            }
            if (dx == 0 && dy == 0)
                break;
            l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
            if (l < lBest) {
                lBest = l;
                iBest = rgi[0];
            }
        }
        if (i == 50) {
            rgi[0] = iBest;
        }
        if (game.cPlayer > 4) {
            dMin = dGal / 20 + 1000;
            dMax = MulDiv(dGal, 19, 20) + 1000;
        } else if (game.cPlayer > 2) {
            dMin = dGal / 10 + 1000;
            dMax = MulDiv(dGal, 9, 10) + 1000;
        } else {
            dMin = MulDiv(dGal, 3, 20) + 1000;
            dMax = MulDiv(dGal, 17, 20) + 1000;
        }
        i = 1;
        while (1) {
            if (i >= game.cPlayer)
                goto L_15bc;
            for (j = 0; j < 50; j++) {
                rgi[i] = Random(cPlanMax);
                pt = rgptPlan[rgi[i]];
                if (pt.x >= dMin && pt.y >= dMin && pt.x <= dMax && pt.y <= dMax) {
                    fFound = 0;
                    for (k = 0; k < i; k++) {
                        dx = pt.x - rgptPlan[rgi[k]].x;
                        dy = pt.y - rgptPlan[rgi[k]].y;
                        l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                        if (l <= 0 || l < lDistMin2)
                            break;
                        if (l <= lDistMax2) {
                            fFound = 1;
                        }
                    }
                    if (k == i && fFound != 0)
                        break;
                }
            }
            if (j == 50) {
                iBest = rgi[i];
                while (1) {
                    rgi[i]++;
                    if (rgi[i] == iBest)
                        break;
                    if (rgi[i] >= cPlanMax) {
                        rgi[i] = 0;
                        if (iBest == 0)
                            break;
                    }
                    pt = rgptPlan[rgi[i]];
                    if (pt.x >= dMin && pt.y >= dMin && pt.x <= dMax && pt.y <= dMax) {
                        fFound = 0;
                        for (k = 0; k < i; k++) {
                            dx = pt.x - rgptPlan[rgi[k]].x;
                            dy = pt.y - rgptPlan[rgi[k]].y;
                            l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                            if (l <= 0 || l < lDistMin2)
                                break;
                            if (l <= lDistMax2) {
                                fFound = 1;
                            }
                        }
                        if (k == i && fFound != 0)
                            break;
                    }
                }
                if (iBest == rgi[i])
                    break;
            }
            i++;
        }
        lDistMin2 -= (int32_t)(lDistIdeal2 / 35);
        lDistMax2 += (int32_t)(lDistIdeal2 / 35);
    }
L_15bc:
    for (i = 0; i < game.cPlayer; i++) {
        j = Random(game.cPlayer - i) + i;
        k = rgi[j];
        rgi[j] = rgi[i];
        rgi[i] = k;
        if (GetRaceGrbit(&rgplr[i], ibitRaceAIPlayer) != 0) {
            CreateRandomRace(&rgplr[i]);
        }
        rgplr[i].wFlags &= 0xfffe;
        rgplr[i].wFlags &= 0xfff7;
        rgplr[i].grbitTrader = 0;
        for (j = 0; j < 6; j++) {
            rgplr[i].rgTech[j] = 0;
            rgplr[i].rgResSpent[j] = 0;
        }
        t_call_1734 = GetRaceStat(&rgplr[i], rsMajorAdv);
        if ((uint16_t)(t_call_1734 - 1) <= 8) {
            switch (t_call_1734) {
            case 2:
                rgplr[i].rgTech[1] = 6;
                rgplr[i].rgTech[2] = 1;
                rgplr[i].rgTech[0] = 1;
                break;
            case 5:
                rgplr[i].rgTech[2] = 2;
                rgplr[i].rgTech[5] = 2;
                break;
            case 1:
                rgplr[i].rgTech[4] = 5;
                break;
            case 6:
                rgplr[i].rgTech[0] = 4;
                break;
            case 7:
                rgplr[i].rgTech[2] = 5;
                rgplr[i].rgTech[3] = 5;
                break;
            case 3:
                rgplr[i].rgTech[5] = 6;
                rgplr[i].rgTech[3] = 2;
                rgplr[i].rgTech[0] = 1;
                rgplr[i].rgTech[1] = 1;
                rgplr[i].rgTech[2] = 1;
                break;
            case 8:
                rgplr[i].rgTech[0] = 1;
                break;
            case 9:
                for (j = 0; j < 6; j++) {
                    rgplr[i].rgTech[j] = 3;
                }
            }
        }
        if (GetRaceGrbit(&rgplr[i], ibitRaceTech3) != 0) {
            iTechMin = (GetRaceStat(&rgplr[i], rsMajorAdv) == raNone ? 1 : 0) + 3;
            for (j = 0; j < 6; j++) {
                if (rgplr[i].rgTech[j] < iTechMin && GetRaceStat(&rgplr[i], j + 8) == 0) {
                    rgplr[i].rgTech[j] = LOBYTE(iTechMin);
                }
            }
        }
        if (GetRaceGrbit(&rgplr[i], ibitRaceCheapEngines) != 0) {
            rgplr[i].rgTech[2]++;
        }
        if (GetRaceGrbit(&rgplr[i], ibitRaceIFE) != 0 && game.fTutorial == 0) {
            rgplr[i].rgTech[2]++;
        }
        for (j = 0; j < 4; j++) {
            FSendPlrMsg(i, j + 127, -1, 0, 0, 0, 0, 0, 0, 0);
        }
    }
    for (i = 0; i < game.cPlayer; i++) {
        iMin = rgi[i];
        lpPlanets[iMin].iPlayer = i;
        lpPlanets[iMin].fStarbase = 1;
        lpPlanets[iMin].isb = 0;
        lpPlanets[iMin].fArtifact = 0;
        lpPlanets[iMin].cFactories = 10;
        lpPlanets[iMin].cMines = 10;
        lpPlanets[iMin].cDefenses = 10;
        lpPlanets[iMin].fHomeworld = 1;
        if (GetRaceGrbit(&rgplr[i], ibitRaceLowStartingPop) != 0) {
            lpPlanets[iMin].rgwtMin[3] = 175;
        } else {
            lpPlanets[iMin].rgwtMin[3] = 250;
        }
        lpPlanets[iMin].uGuesses = (lpPlanets[iMin].uGuesses & 0xf000) | ((uint32_t)LOWORD(lpPlanets[iMin].rgwtMin[3]) / 4 & 0xfff);
        for (j = 0; j < 3; j++) {
            lpPlanets[iMin].rgwtMin[j] = lpPlanets->rgwtMin[j];
            if (gd.fTutorial != 0) {
                lpPlanets[iMin].rgMinConc[j] = LOBYTE(25 <= lpPlanets->rgMinConc[j] ? lpPlanets->rgMinConc[j] : 25);
            } else {
                lpPlanets[iMin].rgMinConc[j] = LOBYTE(30 <= lpPlanets->rgMinConc[j] ? lpPlanets->rgMinConc[j] : 30);
            }
        }
        lpPlanets[iMin].iScanner = 0;
        FSendPlrMsg(i, 169, iMin, iMin, 0, 0, 0, 0, 0, 0);
        if (50 < CAdvantagePoints(&rgplr[i])) {
            iT = 50;
        } else {
            iT = CAdvantagePoints(&rgplr[i]);
        }
        if (rgplr[i].fAi != 0) {
            iT = 50;
            if (rgplr[i].lvlAi >= lvlAiExpert) {
                lpPlanets[iMin].rgwtMin[3] += (int32_t)(lpPlanets[iMin].rgwtMin[3] / 10);
            }
        }
        if (game.fBBSPlay != 0) {
            pct10 = PctTrueMaxGrowth(i) * 2 + 10;
            lpPlanets[iMin].rgwtMin[3] = (uint32_t)(lpPlanets[iMin].rgwtMin[3] * pct10);
            lpPlanets[iMin].rgwtMin[3] = (int32_t)(lpPlanets[iMin].rgwtMin[3] / 10);
            lpPlanets[iMin].uPopGuess <<= 2;
        }
        j = GetRaceStat(&rgplr[i], rsUseLeftover);
        switch (j) {
        case 0:
        default:
            ktLeft = 10 * iT;
            pl = lpPlanets[iMin].rgwtMin;
            if (*pl < pl[1]) {
                if (*pl < pl[2]) {
                    iLow = 0;
                } else {
                    iLow = 2;
                }
            } else if (pl[1] < pl[2]) {
                iLow = 1;
            } else {
                iLow = 2;
            }
            pl[iLow] += (int16_t)((ktLeft >> 2) + (ktLeft & 3));
            ktLeft >>= 2;
            for (j = 0; j < 3; j++) {
                pl[j] += ktLeft;
            }
            if (rgplr[i].fAi == 0 || rgplr[i].lvlAi < lvlAiTough)
                break;
        case 1:
            if (iT > 0 && iT < 3) {
                ktLeft = 1;
            } else {
                ktLeft = iT / 2;
            }
            pb = lpPlanets[iMin].rgMinConc;
            iLow = 0;
            for (j = 0; j < 3; j++) {
                if (pb[j] < pb[iLow]) {
                    iLow = j;
                }
            }
            pb[iLow] += LOBYTE(ktLeft);
            ktLeft = (int16_t)(ktLeft + 1) / 2;
            for (j = 0; j < 3; j++) {
                pb[j] += LOBYTE(ktLeft);
            }
            break;
        case 2:
            lpPlanets[iMin].cMines += iT >> 1;
            break;
        case 3:
            lpPlanets[iMin].cFactories += iT / 5;
            break;
        case 4:
            lpPlanets[iMin].cDefenses += (int16_t)(iT + 5) / 10;
        }
        if (GetRaceStat(&rgplr[i], rsMajorAdv) == raMacintosh) {
            lpPlanets[iMin].cMines = 0;
            lpPlanets[iMin].cFactories = 0;
            lpPlanets[iMin].cDefenses = 0;
        }
        rgplr[i].iPlayer = LOBYTE(i);
        rgplr[i].idPlanetHome = iMin;
        if (rgplr[i].rgEnvVarMax[0] == -1) {
            iT = Random(99) + 1;
        } else {
            iT = rgplr[i].rgEnvVarMin[0] + (int16_t)(rgplr[i].rgEnvVarMax[0] - rgplr[i].rgEnvVarMin[0]) / 2;
        }
        t_scratch_m116_15 = iT;
        lpPlanets[iMin].rgEnvVarOrig[0] = LOBYTE(t_scratch_m116_15);
        lpPlanets[iMin].rgEnvVar[0] = LOBYTE(t_scratch_m116_15);
        if (rgplr[i].rgEnvVarMax[1] == -1) {
            iT = Random(99) + 1;
        } else {
            iT = rgplr[i].rgEnvVarMin[1] + (int16_t)(rgplr[i].rgEnvVarMax[1] - rgplr[i].rgEnvVarMin[1]) / 2;
        }
        t_scratch_m116_17 = iT;
        lpPlanets[iMin].rgEnvVarOrig[1] = LOBYTE(t_scratch_m116_17);
        lpPlanets[iMin].rgEnvVar[1] = LOBYTE(t_scratch_m116_17);
        if (rgplr[i].rgEnvVarMax[2] == -1) {
            iT = Random(99) + 1;
        } else {
            iT = rgplr[i].rgEnvVarMin[2] + (int16_t)(rgplr[i].rgEnvVarMax[2] - rgplr[i].rgEnvVarMin[2]) / 2;
        }
        t_scratch_m116_19 = iT;
        lpPlanets[iMin].rgEnvVarOrig[2] = LOBYTE(t_scratch_m116_19);
        lpPlanets[iMin].rgEnvVar[2] = LOBYTE(t_scratch_m116_19);
        if (rgplr[i].fAi == 0) {
            rgplr[i].pctResearch = 15;
        }
        rgplr[i].iTechCur = LOBYTE(rgplr[i].iTechCur & 0xfff0);
        rgplr[i].iTechCur = LOBYTE((rgplr[i].iTechCur & 0xff0f) | 0x60);
        rgplr[i].lResLastYear = 0;
        rgplr[i].wScore = 0;
        for (j = 0; j < game.cPlayer; j++) {
            rgplr[i].rgmdRelation[j] = 0;
        }
        rgplr[i].cshdefSB = 1;
        lpshdef = LpAlloc(10 * sizeof(SHDEF), htShips);
        fmemmove(lpshdef, LpshdefSBT(), 4 * sizeof(SHDEF));
        fmemset(lpshdef + 4, 0, 6 * sizeof(SHDEF));
        lpshdef->cBuilt = 1;
        lpshdef->cExist = 1;
        rglpshdefSB[i] = lpshdef;
        for (j = 1; j < 10; j++) {
            lpshdef[j].wFlags = (lpshdef[j].wFlags & 0xfdff) | 0x200;
        }
        if (GetRaceStat(&rgplr[i], rsMajorAdv) == raMassAccel) {
            lpshdef->hul.rghs[0].iItem = 7;
            lpshdef->hul.rghs[0].cItem = 1;
            if (game.mdSize > 0) {
                lpshdef[1].fFree = 0;
                rgplr[i].cshdefSB++;
                lpshdef[1].cExist = 1;
                lpshdef[1].cBuilt = 1;
            }
        } else if (GetRaceStat(&rgplr[i], rsMajorAdv) == raStargate && game.fTutorial == 0) {
            lpshdef->hul.rghs[0].iItem = 0;
            lpshdef->hul.rghs[0].cItem = 1;
            if (game.mdSize > 0) {
                lpshdef[1] = lpshdef[2];
                lpshdef[1].ishdef = 17;
                lpshdef[1].fFree = 0;
                rgplr[i].cshdefSB++;
                lpshdef[1].cExist = 1;
                lpshdef[1].cBuilt = 1;
            }
        } else if (GetRaceStat(&rgplr[i], rsMajorAdv) == raMacintosh) {
            idHome = rgplr[i].idPlanetHome;
            lpshdef[1] = *lpshdef;
            lpshdef[1].ishdef = 17;
            *lpshdef = lpshdef[3];
            lpshdef->ishdef = 16;
            lpshdef->fFree = 0;
            rgplr[i].cshdefSB++;
            lpshdef->cExist = 0;
            lpshdef->cBuilt = 0;
            lpPlanets[idHome].isb = 1;
        }
    }
    cFleet = 0;
    rglpfl = LpAlloc(sizeof(FLEET *), htMisc);
    for (i = 0; i < game.cPlayer; i++) {
        idPlayer = i;
        lpshdef = LpAlloc(16 * sizeof(SHDEF), htShips);
        fmemset(lpshdef, 0, 16 * sizeof(SHDEF));
        for (j = 0; j < 16; j++) {
            lpshdef[j].wFlags = (lpshdef[j].wFlags & 0xfdff) | 0x200;
        }
        rglpshdef[i] = lpshdef;
        idHome = rgplr[i].idPlanetHome;
        raMajor = GetRaceStat(&rgplr[i], rsMajorAdv);
        switch (raMajor) {
        case 6:
            CreateStartupShip(i, idHome, 4, 1);
            lpPlanets[idHome].iWarpFling = 1;
            break;
        case 2:
            CreateStartupShip(i, idHome, 3, 1);
            if (rgplr[i].rgTech[3] < 3)
                break;
            CreateStartupShip(i, idHome, 7, 1);
            CreateStartupShip(i, idHome, 13, 1);
            break;
        case 9:
            CreateStartupShip(i, idHome, 3, 1);
            CreateStartupShip(i, idHome, 4, 1);
            break;
        case 1:
            CreateStartupShip(i, idHome, rgplr[i].rgTech[0] < 2 ? 2 : 5, 1);
            if (rgplr[i].fAi != 0)
                break;
            CreateStartupShip(i, idHome, 1, 1);
            break;
        default:
            CreateStartupShip(i, idHome, 2, 1);
        }
        switch (raMajor) {
        case 0:
            ishRet = CreateStartupShip(i, idHome, 12, 1);
            for (j = 1; j < 3; j++) {
                CreateStartupShip(i, idHome, ishRet, 0);
            }
            break;
        case 7:
            CreateStartupShip(i, idHome, 11, 1);
            break;
        case 8:
            CreateStartupShip(i, idHome, 10, 1);
            break;
        default:
            ishRet = CreateStartupShip(i, idHome, 9, 1);
        }
        switch (raMajor) {
        case 5:
            CreateStartupShip(i, idHome, 16, 1);
            CreateStartupShip(i, idHome, 18, 1);
            break;
        case 3:
            CreateStartupShip(i, idHome, 17, 1);
            break;
        case 7:
            CreateStartupShip(i, idHome, 7, 1);
            CreateStartupShip(i, idHome, 8, 1);
            if (game.mdSize > 0)
                goto LGive2ndPlanet;
            break;
        case 6:
            if (game.mdSize > 0)
                goto LGive2ndPlanet;
        default:
            if (raMajor == 9) {
                CreateStartupShip(i, idHome, rgplr[i].rgTech[3] < 4 ? 6 : 8, 1);
                CreateStartupShip(i, idHome, 7, 1);
                CreateStartupShip(i, idHome, 14, 1);
            }
        }
        goto L_39dd;
    LGive2ndPlanet:
        lpplPicked = NULL;
        lpplClosest = NULL;
        ptHome = rgptPlan[idHome];
        cFit = 0;
        lDistMin2 = (int32_t)(dGal * 15) / 100;
        lDistMin2 = (uint32_t)(lDistMin2 * lDistMin2);
        lDistMax2 = (int32_t)(dGal * 23) / 100;
        lDistMax2 = (uint32_t)(lDistMax2 * lDistMax2);
        lDistIdeal2 = (int32_t)(dGal * 20) / 100;
        lDistIdeal2 = (uint32_t)(lDistIdeal2 * lDistIdeal2);
        lBest = 10000000;
        pptMax = &rgptPlan[cPlanMax];
        ppt = rgptPlan;
        lppl = lpPlanets;
        while (ppt < pptMax) {
            if (lppl->iPlayer == -1) {
                dx = ppt->x - ptHome.x;
                dy = ppt->y - ptHome.y;
                lDistCur2 = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                if (lDistCur2 >= lDistMin2 && lDistCur2 <= lDistMax2) {
                    cFit++;
                    if (Random(cFit) == 0) {
                        lpplPicked = lppl;
                    }
                } else if (lpplPicked == 0 && lDistCur2 < lBest) {
                    lBest = lDistCur2;
                    lpplClosest = lppl;
                }
            }
            ppt++;
            lppl++;
        }
        if (lpplPicked == 0) {
            lpplPicked = lpplClosest;
        }
        lpplPicked->iWarpFling = 1;
        cFit = 0;
        while (PctPlanetDesirability(lpplPicked, i) < 10 && cFit++ < 100) {
            for (j = 0; j < 3; j++) {
                t_call_361d = Random(97);
                lpplPicked->rgEnvVarOrig[j] = LOBYTE(t_call_361d + 2);
                lpplPicked->rgEnvVar[j] = LOBYTE(t_call_361d + 2);
            }
        }
        if (cFit >= 100) {
            for (j = 0; j < 3; j++) {
                lpplPicked->rgEnvVar[j] = lpPlanets[idHome].rgEnvVar[j];
                lpplPicked->rgEnvVarOrig[j] = lpPlanets[idHome].rgEnvVarOrig[j];
            }
        }
        lpplPicked->iPlayer = i;
        lpplPicked->fStarbase = 1;
        lpplPicked->isb = 1;
        lpplPicked->fArtifact = 0;
        lpplPicked->cFactories = 4;
        lpplPicked->cMines = 10;
        lpplPicked->rgwtMin[3] = (int32_t)(lpPlanets[idHome].rgwtMin[3] * 2) / 5;
        lpplPicked->uPopGuess = (uint32_t)LOWORD(lpplPicked->rgwtMin[3]) / 4;
        for (j = 0; j < 3; j++) {
            lpplPicked->rgwtMin[j] = (int16_t)(Random(200) + 100);
        }
        lpplPicked->iScanner = 0;
        lpPlanets[idHome].rgwtMin[3] = (int32_t)(lpPlanets[idHome].rgwtMin[3] * 4) / 5;
        lpPlanets[idHome].uGuesses = (lpPlanets[idHome].uGuesses & 0xf000) | ((uint32_t)LOWORD(lpPlanets[idHome].rgwtMin[3]) / 4 & 0xfff);
        CreateStartupShip(i, lpplPicked->id, 0, 0);
    L_39dd:
        if (GetRaceGrbit(&rgplr[i], ibitRaceOBRM) == 0 && GetRaceGrbit(&rgplr[i], ibitRaceARM) != 0) {
            ishRet = CreateStartupShip(i, idHome, 15, 1);
            CreateStartupShip(i, idHome, ishRet, 0);
        }
        for (j = 0; j < rgplr[i].cShDef; j++) {
            chs = rglpshdef[i][j].hul.chs;
            lphs = rglpshdef[i][j].hul.rghs;
            k = 0;
            while (k < chs) {
                cTry = 0;
                part.hs.grhst = lphs->grhst;
                t_fields_1 = &part.hs;
                t_fields_2 = lphs->iItem;
                t_fields_3 = lphs->cItem;
                t_fields_1->iItem = t_fields_2;
                t_fields_1->cItem = t_fields_3;
                switch (part.hs.grhst) {
                case hstEngine:
                    if (part.hs.iItem != iengineQuickJump5)
                        break;
                    if (rgplr[i].rgEnvVar[2] == -1 || rglpshdef[i][j].hul.ihuldef != ihuldefColonyShip || rgplr[i].rgEnvVar[2] >= 85) {
                        rgTry[cTry++] = 10;
                    }
                    rgTry[cTry++] = 5;
                    rgTry[cTry++] = 4;
                    rgTry[cTry++] = 2;
                    rgTry[cTry++] = 3;
                    break;
                case hstShield:
                    if (part.hs.iItem != ishieldMoleSkinShield && part.hs.iItem != ishieldCowHideShield)
                        break;
                    rgTry[cTry++] = 2;
                    rgTry[cTry++] = 1;
                    break;
                case hstArmor:
                    if (part.hs.iItem != iarmorTritanium && part.hs.iItem != iarmorCrobmnium)
                        break;
                    rgTry[cTry++] = 2;
                    rgTry[cTry++] = 1;
                    break;
                case hstBeam:
                    if (part.hs.iItem != ibeamLaser && part.hs.iItem != ibeamXRayLaser)
                        break;
                    rgTry[cTry++] = 3;
                    rgTry[cTry++] = 1;
                    break;
                case hstTorp:
                    if (part.hs.iItem != itorpAlphaTorpedo)
                        break;
                    rgTry[cTry++] = 1;
                    break;
                case hstBomb:
                    if (part.hs.iItem != ibombLadyFingerBomb)
                        break;
                    rgTry[cTry++] = 1;
                    break;
                case hstMining:
                    if (part.hs.iItem != iminingRoboMidgetMiner && part.hs.iItem != iminingRoboMiniMiner)
                        break;
                    rgTry[cTry++] = 2;
                    rgTry[cTry++] = 0;
                    break;
                case hstScanner:
                    if (part.hs.iItem == iscannerBatScanner || part.hs.iItem == iscannerRhinoScanner) {
                        rgTry[cTry++] = 4;
                        rgTry[cTry++] = 2;
                        rgTry[cTry++] = 1;
                    }
                }
                for (l = 0; l < cTry; l++) {
                    part.hs.iItem = rgTry[l];
                    if (FLookupPart(&part) == 1) {
                        lphs->iItem = rgTry[l];
                        break;
                    }
                }
                k++;
                lphs++;
            }
        }
        if (GetRaceStat(&rgplr[i], rsMajorAdv) == raMacintosh) {
            lpPlanets[idHome].iScanner = 31;
        }
    }
    idPlayer = -1;
    if (lpPlanets->iPlayer == -1) {
        for (j = 0; j < 3; j++) {
            lpPlanets->rgwtMin[j] = 0;
        }
    }
    for (i = 0; i < game.cPlayer; i++) {
        rgcbtlplan[i] = 5;
        rglpbtlplan[i] = LpAlloc(16 * sizeof(BTLPLAN), htShips);
        for (j = 0; j < 5; j++) {
            InitBattlePlan(rglpbtlplan[i] + j, j, i);
        }
    }
    game.cPlanMax = cPlanMax;
    if (game.fNoRandom != 0) {
        iBest = 0;
    } else {
        t_scratch_m116_24 = Random(vrgWormholeVar[game.mdSize]);
        iBest = vrgWormholeMin[game.mdSize] + t_scratch_m116_24;
    }
    if (iBest > 0) {
        for (i = 0; i < iBest; i++) {
            for (j = 0; j < 2; j++) {
                lpth = LpthNew(0, ithWormhole);
                t_scratch_m120 = Random(3);
                lpth->thw.iStable = t_scratch_m120;
                if (j == 1) {
                    lpthLast = LpthFromId(idLast);
                    lpth->thw.idPartner = lpthLast->idFull;
                    lpthLast->thw.idPartner = lpth->idFull;
                } else {
                    idLast = lpth->idFull;
                }
                k = 0;
                iMax = 16;
                while (k++ < 100) {
                    lpth->pt.x = Random(dGal) + 1000;
                    lpth->pt.y = Random(dGal) + 1000;
                    iLow = IValidateWormholePos(lpth);
                    if (iLow == 0)
                        break;
                    if (iLow < iMax) {
                        iMax = iLow;
                        pt = lpth->pt;
                    }
                }
                if (iLow != 0) {
                    lpth->pt = pt;
                }
            }
        }
    }
    for (i = 0; i < game.cPlayer; i++) {
        if (rgplr[i].fHacker != 0) {
            FSendPlrMsg2(i, 279, -1, 0, 0);
            for (j = 0; j < game.cPlayer; j++) {
                if (i != j && rgplr[i].fAi == 0) {
                    FSendPlrMsg2(j, 386, -1, i, 0);
                }
            }
        }
    }
    iplrSingle = -1;
    for (i = 0; i < game.cPlayer; i++) {
        if (rgplr[i].fAi == 0 || rgplr[i].idAi == 7) {
            if (iplrSingle != -1)
                break;
            iplrSingle = i;
        } else {
            rgplr[i].lSalt = 156085230;
        }
    }
    game.fSinglePlr = i == game.cPlayer && iplrSingle != -1;
    if (game.fSinglePlr != 0) {
        for (i = 0; i < game.cPlayer; i++) {
            for (j = 0; j < game.cPlayer; j++) {
                if (i != j) {
                    rgplr[i].rgmdRelation[j] = 2;
                }
            }
        }
    }
    if (game.fTutorial == 0) {
        game.lid = GetTickCount();
    }
    _wsprintf(szWork, "%s.xy", szBase);
    if (FCreateFile(dtXY, -1, NULL) == 0) {
        AlertSz(PszFormatIds(idsUnableCreateUniverseDefinitionFile, NULL), MB_ICONHAND);
        DestroyCurGame();
        return 0;
    }
    WriteRt(rtGame, 64, &game);
    xOld = 1000;
    for (i = 0; i < cPlanMax; i++) {
        starpack.y = LOWORD(rgptPlan[i].y);
        starpack.id = LOWORD(rgidPlan[i]);
        dx = rgptPlan[i].x - xOld;
        starpack.dx = dx;
        RgToStream(&starpack, 4);
        xOld = rgptPlan[i].x;
    }
    i = game.cPlayer;
    WriteRt(rtEOF, 2, &i);
    StreamClose();
    for (i = -1; i < game.cPlayer; i++) {
        FWriteDataFile(szBase, i, 0);
    }
    if (fBatchMode != 0) {
        return 1;
    }
    if (game.fSinglePlr != 0) {
        DestroyCurGame();
        _wsprintf(szExt, MPCTD, iplrSingle + 1);
        if (FLoadGame(szBase, szExt) == 0) {
            AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, NULL), MB_ICONHAND);
            return 0;
        }
        idPlayer = iplrSingle;
        CreateChildWindows();
        SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0);
    } else {
        idPlayer = -1;
        imemLogCur = 0;
        CreateChildWindows();
    }
    return 1;
}

int16_t CreateStartupShip(int16_t iplr, int16_t idPlanet, int16_t ishdef, int16_t fAddShdef) {
    int16_t ishMac;
    FLEET  *lpfl;
    int8_t  t_46ad;
    SHDEF  *t_call_46b8;

    if (fAddShdef != 0) {
        t_46ad = rgplr[iplr].cShDef;
        rgplr[iplr].cShDef++;
        ishMac = (int16_t)(int8_t)t_46ad;
        t_call_46b8 = LpshdefT();
        rglpshdef[iplr][ishMac] = t_call_46b8[ishdef];
        rglpshdef[iplr][ishMac].wFlags = (rglpshdef[iplr][ishMac].wFlags & 0x83ff) | (ishMac & 0x1f) * 0x400;
        ishdef = ishMac;
    }
    rglpshdef[iplr][ishdef].cExist++;
    rglpshdef[iplr][ishdef].cBuilt++;
    lpfl = LpflNew(iplr, idPlanet);
    lpfl->rgcsh[ishdef] = 1;
    lpfl->rgwtMin[4] = LGetFleetStat(lpfl, 1);
    lpfl->iplan = 0;
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
    jmp_buf env;
    int16_t j;
    char   *lpb;
    char   *lpbDef;
    int16_t cb;
    char   *pchT;
    int16_t idAi;
    int16_t lvlAi;

    fSuccess = 0;
    strcpy(szWork, pszFile);
    penvMem = &env;
    if (setjmp(env) == 0) {
        if (ini.fLogging != 0) {
            strcpy(szBase, pszFile);
            pchT = strrchr(szBase, 46);
            *pchT = 0;
            TurnLog(idsGeneratingYearD);
        }
        memset(&game, 0, sizeof(GAME));
        StreamOpen(pszFile, 32);
        cb = LOWORD(filelength(hf));
        if (cb >= 16000) {
            FileError(idmMultitudeEnemiesHaveMountedProngAttackResulting);
        } else {
            lpbDef = LpAlloc(cb + 1, htPerm);
            lpbDefMac = lpbDef + cb;
            RgFromStream(lpbDef, cb);
            StreamClose();
            lpbDef[cb] = 0;
            lpb = lpbDef;
            lpbStart = PszGetLine(&lpb);
            if ((int16_t)(int8_t)*lpbStart == 0 || fstrlen(lpbStart) > 31) {
                AlertSz(PszFormatIds(idsIllegalGameTitle, NULL), MB_ICONHAND);
            } else {
                fstrcpy(game.szName, lpbStart);
                if (lpb < lpbDefMac) {
                    lpbStart = PszGetLine(&lpb);
                    cNum = CParseNumbers(lpbStart, rgl, 4);
                    if (cNum != -1) {
                        for (i = 0; i < cNum; i++) {
                            if (i < 3 && (rgl[i] < 0 || rgl[i] > 4 || (rgl[i] == 4 && i != 0)))
                                goto LUniDefError;
                        }
                        if (cNum >= 1) {
                            game.mdSize = LOWORD(rgl[0]);
                        }
                        if (cNum >= 2) {
                            game.mdDensity = LOWORD(rgl[1]);
                        }
                        if (cNum >= 3) {
                            game.mdStartDist = LOWORD(rgl[2]);
                        }
                        if (cNum >= 4) {
                            Randomize(rgl[3]);
                        }
                        if (lpb >= lpbDefMac)
                            goto LUniDefShort;
                        lpbStart = PszGetLine(&lpb);
                        cNum = CParseNumbers(lpbStart, rgl, 7);
                        if (cNum != -1) {
                            for (i = 0; i < cNum; i++) {
                                if (rgl[i] < 0 || rgl[i] > 1)
                                    goto LUniDefError3;
                            }
                            if (cNum >= 1) {
                                game.fExtraFuel = LOWORD(rgl[0]);
                            }
                            if (cNum >= 2) {
                                game.fSlowTech = LOWORD(rgl[1]);
                            }
                            if (cNum >= 3) {
                                game.fBBSPlay = LOWORD(rgl[2]);
                            }
                            if (cNum >= 4) {
                                game.fNoRandom = LOWORD(rgl[3]);
                            }
                            if (cNum >= 5) {
                                game.fAisBand = LOWORD(rgl[4]);
                            }
                            if (cNum >= 6) {
                                game.fVisScores = LOWORD(rgl[5]);
                            }
                            if (cNum >= 7) {
                                game.fClumping = LOWORD(rgl[6]);
                            }
                            lpbStart = PszGetLine(&lpb);
                            cNum = CParseNumbers(lpbStart, rgl, 1);
                            if (cNum < 1 || rgl[0] < 1 || rgl[0] > 16) {
                                AlertSz(PszFormatIds(idsLine4HasImproperNumberPlayerFiles, NULL), MB_ICONHAND);
                                goto LError;
                            }
                            if (lpb >= lpbDefMac)
                                goto LUniDefShort;
                            cPlr = LOWORD(rgl[0]);
                            game.cPlayer = cPlr;
                            for (i = 0; i < cPlr; i++) {
                                lpbStart = PszGetLine(&lpb);
                                if (lpb >= lpbDefMac)
                                    goto LUniDefShort;
                                if (i > 0 && (int16_t)(int8_t)*lpbStart == 35) {
                                    cNum = CParseNumbers(lpbStart + 1, rgl, 2);
                                    idAi = LOWORD(rgl[0]);
                                    lvlAi = LOWORD(rgl[1]);
                                    if (cNum < 2 || idAi < 0 || idAi > 6 || lvlAi < 0 || lvlAi > 4)
                                        goto L_4e5d;
                                    if (lvlAi == 0) {
                                        lvlAi = Random(4);
                                    } else {
                                        lvlAi--;
                                    }
                                    if (idAi == 0) {
                                        idAi = Random(6);
                                    } else {
                                        idAi--;
                                    }
                                    rgplr[i] = *LpplrComp(idAi, lvlAi);
                                    rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfdff) | 0x200;
                                    rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0x1fff) | (idAi & 7) * 0x2000;
                                    rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xe3ff) | (lvlAi & 7) * 0x400;
                                } else {
                                    fstrcpy(szWork, lpbStart);
                                    if (FWasRaceFile(szWork, 0) == 0)
                                        goto LCantGetRace;
                                    rgplr[i] = vplr;
                                }
                            }
                            lpbStart = PszGetLine(&lpb);
                            cNum = CParseNumbers(lpbStart, rgl, 2);
                            if (lpb >= lpbDefMac)
                                goto LUniDefShort;
                            i = 0;
                            if (cNum >= 1 && rgl[0] >= 0 && rgl[0] <= 1) {
                                if (rgl[0] == 1) {
                                    if (cNum < 2 || rgl[1] < 20 || rgl[1] > 100)
                                        goto LBadDefVc;
                                    SetVCCheck(&game, 0, 1);
                                    SetVCVal(&game, 0, (int16_t)(LOWORD(rgl[1]) - 20) / 5);
                                }
                                lpbStart = PszGetLine(&lpb);
                                cNum = CParseNumbers(lpbStart, rgl, 3);
                                if (lpb >= lpbDefMac)
                                    goto LUniDefShort;
                                i = 1;
                                if (cNum >= 1 && rgl[0] >= 0 && rgl[0] <= 1) {
                                    if (rgl[0] == 1) {
                                        if (cNum < 3 || rgl[1] < 8 || rgl[1] > 26 || rgl[2] < 2 || rgl[2] > 6)
                                            goto LBadDefVc;
                                        SetVCCheck(&game, 1, 1);
                                        SetVCCheck(&game, 2, 1);
                                        SetVCVal(&game, 1, LOWORD(rgl[1]) - 8);
                                        SetVCVal(&game, 2, LOWORD(rgl[2]) - 2);
                                    }
                                    lpbStart = PszGetLine(&lpb);
                                    cNum = CParseNumbers(lpbStart, rgl, 2);
                                    if (lpb >= lpbDefMac)
                                        goto LUniDefShort;
                                    i = 2;
                                    if (cNum >= 1 && rgl[0] >= 0 && rgl[0] <= 1) {
                                        if (rgl[0] == 1) {
                                            if (cNum < 2 || rgl[1] < 1000 || rgl[1] > 20000)
                                                goto LBadDefVc;
                                            SetVCCheck(&game, 3, 1);
                                            SetVCVal(&game, 3, (int16_t)(LOWORD(rgl[1]) - 1000) / 1000);
                                        }
                                        lpbStart = PszGetLine(&lpb);
                                        cNum = CParseNumbers(lpbStart, rgl, 2);
                                        if (lpb >= lpbDefMac)
                                            goto LUniDefShort;
                                        i = 3;
                                        if (cNum >= 1 && rgl[0] >= 0 && rgl[0] <= 1) {
                                            if (rgl[0] == 1) {
                                                if (cNum < 2 || rgl[1] < 20 || rgl[1] > 300)
                                                    goto LBadDefVc;
                                                SetVCCheck(&game, 4, 1);
                                                SetVCVal(&game, 4, (int16_t)(LOWORD(rgl[1]) - 20) / 10);
                                            }
                                            lpbStart = PszGetLine(&lpb);
                                            cNum = CParseNumbers(lpbStart, rgl, 2);
                                            if (lpb >= lpbDefMac)
                                                goto LUniDefShort;
                                            i = 4;
                                            if (cNum >= 1 && rgl[0] >= 0 && rgl[0] <= 1) {
                                                if (rgl[0] == 1) {
                                                    if (cNum < 2 || rgl[1] < 10 || rgl[1] > 500)
                                                        goto LBadDefVc;
                                                    SetVCCheck(&game, 5, 1);
                                                    SetVCVal(&game, 5, (int16_t)(LOWORD(rgl[1]) - 10) / 10);
                                                }
                                                lpbStart = PszGetLine(&lpb);
                                                cNum = CParseNumbers(lpbStart, rgl, 2);
                                                if (lpb >= lpbDefMac)
                                                    goto LUniDefShort;
                                                i = 5;
                                                if (cNum >= 1 && rgl[0] >= 0 && rgl[0] <= 1) {
                                                    if (rgl[0] == 1) {
                                                        if (cNum < 2 || rgl[1] < 10 || rgl[1] > 300)
                                                            goto LBadDefVc;
                                                        SetVCCheck(&game, 6, 1);
                                                        SetVCVal(&game, 6, (int16_t)(LOWORD(rgl[1]) - 10) / 10);
                                                    }
                                                    lpbStart = PszGetLine(&lpb);
                                                    cNum = CParseNumbers(lpbStart, rgl, 2);
                                                    if (lpb >= lpbDefMac)
                                                        goto LUniDefShort;
                                                    i = 6;
                                                    if (cNum >= 1 && rgl[0] >= 0 && rgl[0] <= 1) {
                                                        if (rgl[0] == 1) {
                                                            if (cNum < 2 || rgl[1] < 30 || rgl[1] > 900)
                                                                goto LBadDefVc;
                                                            SetVCCheck(&game, 7, 1);
                                                            SetVCVal(&game, 7, (int16_t)(LOWORD(rgl[1]) - 30) / 10);
                                                        }
                                                        lpbStart = PszGetLine(&lpb);
                                                        cNum = CParseNumbers(lpbStart, rgl, 2);
                                                        if (lpb >= lpbDefMac)
                                                            goto LUniDefShort;
                                                        i = 7;
                                                        if (cNum >= 1 && rgl[0] >= 0 && rgl[0] <= 7) {
                                                            if (rgl[0] > 0) {
                                                                if (cNum < 2 || rgl[1] < 30 || rgl[1] > 500)
                                                                    goto LBadDefVc;
                                                                SetVCVal(&game, 8, LOWORD(rgl[0]));
                                                                SetVCVal(&game, 9, (int16_t)(LOWORD(rgl[1]) - 30) / 10);
                                                            }
                                                            lpbStart = PszGetLine(&lpb);
                                                            lpb = lpbStart + (-1 + fstrlen(lpbStart));
                                                            if (lpb - lpbStart >= 3 && (int16_t)(int8_t)*lpb == 121 && (int16_t)(int8_t)lpb[-1] == 120 &&
                                                                (int16_t)(int8_t)lpb[-2] == 46) {
                                                                lpb[-2] = 0;
                                                            }
                                                            fstrcpy(szBase, lpbStart);
                                                            if (lpb + 4 < lpbDefMac) {
                                                                lpbDefUni = lpb;
                                                            }
                                                            for (i = 0; i < game.cPlayer; i++) {
                                                                if (rgplr[i].fAi == 0 && CAdvantagePoints(&rgplr[i]) < 0) {
                                                                    rgplr[i] = vrgplrDef[0];
                                                                    rgplr[i].wFlags = (rgplr[i].wFlags & 0xffef) | 0x10;
                                                                }
                                                                if ((int16_t)(int8_t)rgplr[i].szName[0] == 0) {
                                                                    CchGetString(Random(24) + 1390, rgplr[i].szName);
                                                                    _wsprintf(rgplr[i].szNames, "%ss", rgplr[i].szName);
                                                                }
                                                            }
                                                            for (i = 1; i < game.cPlayer; i++) {
                                                                for (j = 0; j < i && strcmp(rgplr[i].szName, rgplr[j].szName) != 0; j++) {
                                                                }
                                                                if (j < i) {
                                                                    c = Random(24);
                                                                    while (1) {
                                                                        for (j = 0;
                                                                             j < game.cPlayer && strcmp(rgplr[j].szName, PszGetCompressedString(c + 1390)) != 0;
                                                                             j++) {
                                                                        }
                                                                        if (j == game.cPlayer)
                                                                            break;
                                                                        c++;
                                                                        if (c >= 24) {
                                                                            c = 0;
                                                                        }
                                                                    }
                                                                    CchGetString(c + 1390, rgplr[i].szName);
                                                                    strcpy(rgplr[i].szNames, rgplr[i].szName);
                                                                    strcat(rgplr[i].szNames, "s");
                                                                }
                                                            }
                                                            for (i = 0; i < game.cPlayer; i++) {
                                                                rgplrbmp[i] = rgplr[i].iPlrBmp;
                                                                if (rgplrbmp[i] < 0 || rgplrbmp[i] >= 32) {
                                                                    rgplrbmp[i] = -1;
                                                                }
                                                            }
                                                            for (i = 1; i < game.cPlayer; i++) {
                                                                if (rgplrbmp[i] != -1) {
                                                                    for (j = 0; j < i && rgplrbmp[j] != rgplrbmp[i]; j++) {
                                                                    }
                                                                    if (j < i) {
                                                                        rgplrbmp[Random(2) == 0 ? j : i] = -1;
                                                                    }
                                                                }
                                                            }
                                                            for (i = 0; i < game.cPlayer; i++) {
                                                                if (rgplrbmp[i] == -1) {
                                                                    rgplrbmp[i] = Random(32);
                                                                    while (1) {
                                                                        for (j = 0; j < game.cPlayer && (j == i || rgplrbmp[i] != rgplrbmp[j]); j++) {
                                                                        }
                                                                        if (j == game.cPlayer)
                                                                            break;
                                                                        rgplrbmp[i]++;
                                                                        if (rgplrbmp[i] >= 32) {
                                                                            rgplrbmp[i] = 0;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                            for (i = 0; i < game.cPlayer; i++) {
                                                                rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xff07) | (rgplrbmp[i] & 0x1f) * 8;
                                                            }
                                                            GenerateWorld(1);
                                                            fSuccess = 1;
                                                            goto LError;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        LBadDefVc:
                            i += cPlr + 5;
                            _wsprintf(szWork, PszGetCompressedString(idsLineDHasImproperVictoryConditionDefinition), i);
                            AlertSz(szWork, MB_ICONHAND);
                            goto LError;
                        L_4e5d:
                            fstrcpy(szWork, lpbStart);
                        LCantGetRace:
                            _wsprintf(szWork, PszGetCompressedString(idsLineDUnableLoadRaceFileS), i + 5, lpbStart);
                            AlertSz(szWork, MB_ICONHAND);
                            goto LError;
                        }
                    LUniDefError3:
                        AlertSz(PszFormatIds(idsLine3HasBadUniverseDefinitionParameter, NULL), MB_ICONHAND);
                        goto LError;
                    }
                LUniDefError:
                    AlertSz(PszFormatIds(idsLine2HasBadUniverseDefinitionParameter, NULL), MB_ICONHAND);
                    goto LError;
                }
            LUniDefShort:
                AlertSz(PszFormatIds(idsUniverseDefinitionFileAppearsTooShort, NULL), MB_ICONHAND);
            }
        }
    }
LError:
    penvMem = 0;
    StreamClose();
    lpbDefUni = NULL;
    TurnLog(fSuccess + 1380);
    return fSuccess;
}

void CreateTutorWorld() {
    int16_t i;

    memset(&game, 0, sizeof(GAME));
    game.cPlayer = 2;
    game.fTutorial = 1;
    game.mdDensity = 0;
    game.mdSize = 0;
    game.mdStartDist = 1;
    game.fBBSPlay = 1;
    game.fVisScores = 1;
    game.fNoRandom = 1;
    game.lid = 9236297;
    game.rgvc[7] = 128;
    game.rgvc[8] = 129;
    CchGetString(idsTutorialGame, game.szName);
    rgplr[0] = vrgplrDef[0];
    CchGetString(idsHumanoid, rgplr[0].szName);
    _wsprintf(rgplr[0].szNames, "%ss", rgplr[0].szName);
    rgplr[1] = *LpplrComp(1, lvlAiEasy);
    rgplr[1].fAi = 1;
    rgplr[1].lvlAi = lvlAiEasy;
    rgplr[1].idAi = 1;
    CchGetString(idsBerserker, rgplr[1].szName);
    Randomize(1234567890);
    for (i = 1; i <= 2; i++) {
        _wsprintf(szWork, PszGetCompressedString(idsSHD), szBase, i);
        remove(szWork);
        _wsprintf(szWork, PszGetCompressedString(idsSXD), szBase, i);
        remove(szWork);
    }
    GenerateWorld(0);
    return;
}

void NewGameWizard(HWND hwnd, int16_t fReadOnly) {
    int16_t iStepMaxSoFar;
    int16_t mdRet;
    FARPROC lpProc;
    int16_t fIdleSav;
    int16_t rgplrbmp[16];
    int16_t i;
    int16_t c;
    char    szFile[256];
    int16_t idAi;
    int16_t fEasy;
    char    szFileLocal[208];
    int16_t j;
    RECT    rgrcStack[20];
    PLAYER  rgplrLocal[16];
    int16_t lvlAi;
    GAME    gameT;

    iStepMaxSoFar = 0;
    fEasy = 0;
    vrgrcRCW = rgrcStack;
    iPanelActive = -1;
    fRCWReadOnly = fReadOnly;
    fIdleSav = gd.fNoIdleChecks;
    gd.fNoIdleChecks = 1;
    vrgplrNew = rgplrLocal;
    vrgszFileNew = szFileLocal;
    vcplrNew = 0;
    memset(vrgplrTypeNew, 0, 16);
    if (fReadOnly == 0) {
        gameT = game;
        memset(&game, 0, sizeof(GAME));
        game.cPlanMax = gameT.cPlanMax;
        game.cPlayer = gameT.cPlayer;
        vrgplrTypeNew[0] = 1;
        vrgplrTypeNew[1] = 35;
        vrgplrTypeNew[2] = 39;
        vrgplrTypeNew[3] = 139;
        lpProc = MakeProcInstance(SimpleNewGameDlg, hInst);
        mdRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_SIMPLE_NEW_GAME), hwnd, lpProc);
        FreeProcInstance(lpProc);
        game.rgvc[0] = 136;
        game.rgvc[1] = 142;
        game.rgvc[2] = 130;
        game.rgvc[3] = 10;
        game.rgvc[4] = 136;
        game.rgvc[5] = 9;
        game.rgvc[6] = 9;
        game.rgvc[7] = 7;
        game.rgvc[8] = 1;
        switch (mdRet) {
        case 212:
            game = gameT;
            StartTutor(0);
            gd.fNoIdleChecks = fIdleSav;
            break;
        case 1072:
        case 211:
            memset(vrgplrTypeNew, 0, 16);
            if (game.turn < 7) {
                vrgplrTypeNew[0] = LOBYTE((game.turn & 0xff) << 2 | 1);
            } else {
                vrgplrTypeNew[0] = 2;
                *vrgszFileNew = 0;
                vcplrNew = 1;
            }
            lvlAi = game.mdDensity;
            InitNewGamePlr(iStepMaxSoFar, lvlAi);
            game.mdDensity = 1;
            game.mdStartDist = lvlAi < 2 ? 1 : 2;
            game.fExtraFuel = 0;
            game.fSlowTech = 0;
            game.fBBSPlay = 0;
            game.fNoRandom = 0;
            game.fAisBand = lvlAi == 3 ? 1 : 0;
            game.fVisScores = 0;
            CchGetString(5 * lvlAi + 459 + game.mdSize, game.szName);
            if (mdRet == 1072) {
                fEasy = 1;
                goto Finish;
            }
        default:
            goto Step1;
        case 2:
            goto Cancel;
        }
        return;
    }
    for (i = 0; i < game.cPlayer; i++) {
        if (rgplr[i].fInclude != 0) {
            vrgplrTypeNew[i] = LOBYTE(i << 2 | 2);
            vrgplrNew[i] = rgplr[i];
            vrgszFileNew[i * 13] = 0;
        } else {
            vrgplrTypeNew[i] = 25;
        }
    }
Step1:
    while (1) {
        lpProc = MakeProcInstance(NewGameDlg, hInst);
        mdRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_NEW_GAME_1), hwnd, lpProc);
        FreeProcInstance(lpProc);
        if (mdRet == 0)
            goto Cancel;
        InitNewGamePlr(iStepMaxSoFar, lvlAi);
        if (mdRet == 3)
            goto Finish;
        while (1) {
            lpProc = MakeProcInstance(NewGameDlg2, hInst);
            mdRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_NEW_GAME_2), hwnd, lpProc);
            FreeProcInstance(lpProc);
            if (mdRet == 0)
                goto Cancel;
            if (iStepMaxSoFar < 1) {
                iStepMaxSoFar = 1;
            }
            InitNewGamePlr(iStepMaxSoFar, lvlAi);
            if (mdRet == 1)
                break;
            if (mdRet == 3)
                goto Finish;
            lpProc = MakeProcInstance(NewGameDlg3, hInst);
            mdRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_NEW_GAME_3), hwnd, lpProc);
            FreeProcInstance(lpProc);
            if (mdRet == 0)
                goto Cancel;
            if (iStepMaxSoFar < 2) {
                iStepMaxSoFar = 2;
            }
            if (mdRet != 1)
                goto L_64da;
        }
    }
L_64da:
    if (mdRet == 3) {
    }
Finish:
    if (fRCWReadOnly == 0) {
        CchGetString(idsGameXy, szFile);
        if (FGetNewGameName(szFile) == 0) {
            if (fEasy == 0)
                goto Step1;
        } else {
            gameT = game;
            gameT.turn = 0;
            DestroyCurGame();
            game = gameT;
            strcpy(szBase, szFile);
            szBase[strlen(szBase) - 3] = 0;
            for (i = 0; i < 16 && vrgplrTypeNew[i] != 0; i++) {
                switch (vrgplrTypeNew[i] & 3) {
                case 1:
                    if (vrgplrTypeNew[i] >> 2 > 6) {
                        c = Random(7);
                        rgplr[i] = vrgplrDef[c];
                        rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfdff) | 0x200;
                        rgplr[i].wMdPlr &= 0xe3ff;
                        rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0x1fff) | 0xe000;
                        rgplr[i].lSalt = -1;
                    } else {
                        c = vrgplrTypeNew[i] >> 2;
                        rgplr[i] = vrgplrDef[c];
                    }
                    CchGetString(c + 1383, rgplr[i].szName);
                    _wsprintf(rgplr[i].szNames, "%ss", rgplr[i].szName);
                    break;
                case 2:
                    rgplr[i] = rgplrLocal[vrgplrTypeNew[i] >> 2];
                    break;
                case 3:
                    idAi = vrgplrTypeNew[i] >> 2 & 7;
                    lvlAi = vrgplrTypeNew[i] >> 5;
                    if (lvlAi >= 4) {
                        lvlAi = Random(4);
                    }
                    if (idAi >= 6) {
                        idAi = Random(6);
                    }
                    rgplr[i] = *LpplrComp(idAi, lvlAi);
                    rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfdff) | 0x200;
                    rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xe3ff) | (lvlAi & 7) * 0x400;
                    rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0x1fff) | (idAi & 7) * 0x2000;
                }
            }
            game.cPlayer = i;
            for (i = 0; i < game.cPlayer; i++) {
                if (rgplr[i].fAi == 0 && CAdvantagePoints(&rgplr[i]) < 0) {
                    rgplr[i] = vrgplrDef[0];
                    rgplr[i].wFlags = (rgplr[i].wFlags & 0xffef) | 0x10;
                }
                if ((int16_t)(int8_t)rgplr[i].szName[0] == 0) {
                    CchGetString(Random(24) + 1390, rgplr[i].szName);
                }
                if ((int16_t)(int8_t)rgplr[i].szNames[0] == 0) {
                    _wsprintf(rgplr[i].szNames, "%ss", rgplr[i].szName);
                }
            }
            for (i = 1; i < game.cPlayer; i++) {
                for (j = 0; j < i && strcmp(rgplr[i].szName, rgplr[j].szName) != 0; j++) {
                }
                if (j < i) {
                    c = Random(24);
                    while (1) {
                        for (j = 0; j < game.cPlayer && strcmp(rgplr[j].szName, PszGetCompressedString(c + 1390)) != 0; j++) {
                        }
                        if (j == game.cPlayer)
                            break;
                        c++;
                        if (c >= 24) {
                            c = 0;
                        }
                    }
                    CchGetString(c + 1390, rgplr[i].szName);
                    strcpy(rgplr[i].szNames, rgplr[i].szName);
                    strcat(rgplr[i].szNames, "s");
                }
            }
            for (i = 0; i < game.cPlayer; i++) {
                rgplrbmp[i] = rgplr[i].iPlrBmp;
                if (rgplrbmp[i] < 0 || rgplrbmp[i] >= 32 || rgplr[i].fAi != 0) {
                    rgplrbmp[i] = -1;
                }
            }
            for (i = 1; i < game.cPlayer; i++) {
                if (rgplrbmp[i] != -1) {
                    for (j = 0; j < i && rgplrbmp[j] != rgplrbmp[i]; j++) {
                    }
                    if (j < i) {
                        rgplrbmp[Random(2) == 0 ? j : i] = -1;
                    }
                }
            }
            for (i = 0; i < game.cPlayer; i++) {
                if (rgplrbmp[i] == -1) {
                    rgplrbmp[i] = Random(32);
                    while (1) {
                        for (j = 0; j < game.cPlayer && (j == i || rgplrbmp[i] != rgplrbmp[j]); j++) {
                        }
                        if (j == game.cPlayer)
                            break;
                        rgplrbmp[i]++;
                        if (rgplrbmp[i] >= 32) {
                            rgplrbmp[i] = 0;
                        }
                    }
                }
            }
            for (i = 0; i < game.cPlayer; i++) {
                rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xff07) | (rgplrbmp[i] & 0x1f) * 8;
            }
            if (fFreeingTitle == 0) {
                fFreeingTitle = 1;
                DestroyWindow(hwndTitle);
                hwndTitle = 0;
                ShowWindow(hwndFrame, SW_SHOW);
            }
            GenerateWorld(0);
            if (idPlayer == -1) {
                BringUpHostDlg();
            }
            gd.fNoIdleChecks = fIdleSav;
            return;
        }
    }
Cancel:
    if (fRCWReadOnly == 0) {
        game = gameT;
    }
    gd.fNoIdleChecks = fIdleSav;
    return;
}

void InitNewGamePlr(int16_t iStepMaxSoFar, AiLevel lvlAi) {
    int16_t i;
    int16_t c;
    uint8_t ch;

    if (iStepMaxSoFar < 2 && fRCWReadOnly == 0) {
        SetVCVal(&game, 9, game.mdSize * 2);
        if (iStepMaxSoFar < 1) {
            switch (game.mdSize) {
            case 0:
                if (lvlAi == lvlAiExpert && Random(3) == 0) {
                    game.cPlayer = 3;
                    break;
                }
                game.cPlayer = 2;
                break;
            case 1:
                if (lvlAi == lvlAiExpert && Random(4) == 0) {
                    game.cPlayer = 5;
                    break;
                }
                if (lvlAi >= lvlAiTough && Random(6 - lvlAi) == 0) {
                    game.cPlayer = 4;
                    break;
                }
                game.cPlayer = 3;
                break;
            case 2:
                if (lvlAi == lvlAiExpert && Random(10) == 0) {
                    game.cPlayer = 9;
                    break;
                }
                if (lvlAi == lvlAiExpert && Random(10) == 0) {
                    game.cPlayer = 5;
                    break;
                }
                if (lvlAi >= lvlAiTough && Random(7 - lvlAi) == 0) {
                    game.cPlayer = 8;
                    break;
                }
                if (lvlAi >= lvlAiTough && Random(7 - lvlAi) == 0) {
                    game.cPlayer = 6;
                    break;
                }
                game.cPlayer = 7;
                break;
            case 3:
                if (lvlAi == lvlAiExpert && Random(10) == 0) {
                    game.cPlayer = Random(2) + 14;
                    break;
                }
                if (lvlAi == lvlAiExpert && Random(10) == 0) {
                    game.cPlayer = 10 - Random(2);
                    break;
                }
                if (lvlAi >= lvlAiTough && Random(7 - lvlAi) == 0) {
                    game.cPlayer = 13;
                    break;
                }
                if (lvlAi >= lvlAiTough && Random(7 - lvlAi) == 0) {
                    game.cPlayer = 11;
                    break;
                }
                game.cPlayer = 12;
                break;
            case 4:
                if (lvlAi == lvlAiExpert && Random(10) == 0) {
                    game.cPlayer = 13 - Random(3);
                } else if (lvlAi >= lvlAiTough && Random(9 - lvlAi) == 0) {
                    game.cPlayer = 14;
                } else if (lvlAi >= lvlAiTough && Random(7 - lvlAi) == 0) {
                    game.cPlayer = 15;
                } else {
                    game.cPlayer = 16;
                }
            }
            i = 1;
            switch (lvlAi) {
            case lvlAiEasy:
                while (i < game.cPlayer) {
                    if (i < (int16_t)(game.cPlayer + 1) / 3 + 1) {
                        vrgplrTypeNew[i++] = 11;
                    } else if (i < (int16_t)((game.cPlayer + 1) * 2) / 3 + 1) {
                        vrgplrTypeNew[i++] = 15;
                    } else if (i < (int16_t)((game.cPlayer + 1) * 5) / 6 + 1) {
                        vrgplrTypeNew[i++] = 7;
                    } else {
                        vrgplrTypeNew[i++] = 27;
                    }
                }
                break;
            case lvlAiStandard:
                while (i < game.cPlayer) {
                    if (i < (int16_t)((game.cPlayer + 5) * 2) / 7 + 1) {
                        vrgplrTypeNew[i++] = 39;
                    } else if (i < (int16_t)((game.cPlayer - 1) * 3 + 6) / 7 + 1) {
                        vrgplrTypeNew[i++] = 35;
                    } else if (i < (int16_t)((game.cPlayer - 1) * 4 + 6) / 7 + 1) {
                        vrgplrTypeNew[i++] = 43;
                    } else if (i < (int16_t)((game.cPlayer - 1) * 5 + 6) / 7 + 1) {
                        vrgplrTypeNew[i++] = 47;
                    } else if (i < (int16_t)((game.cPlayer - 1) * 6 + 6) / 7 + 1) {
                        vrgplrTypeNew[i++] = 51;
                    } else {
                        vrgplrTypeNew[i++] = 155;
                    }
                }
                break;
            case lvlAiTough:
                while (i < game.cPlayer) {
                    if (i < (int16_t)((game.cPlayer + 5) * 2) / 7 + 1) {
                        vrgplrTypeNew[i++] = 83;
                    } else if (i < (int16_t)((game.cPlayer - 1) * 3 + 6) / 7 + 1) {
                        vrgplrTypeNew[i++] = 71;
                    } else if (i < (int16_t)((game.cPlayer - 1) * 4 + 6) / 7 + 1) {
                        vrgplrTypeNew[i++] = 67;
                    } else if (i < (int16_t)((game.cPlayer - 1) * 5 + 6) / 7 + 1) {
                        vrgplrTypeNew[i++] = 87;
                    } else if (i < (int16_t)((game.cPlayer - 1) * 6 + 6) / 7 + 1) {
                        vrgplrTypeNew[i++] = 91;
                    } else {
                        vrgplrTypeNew[i++] = 155;
                    }
                }
                break;
            case lvlAiExpert:
                while (i < game.cPlayer) {
                    if (i < (int16_t)(game.cPlayer + 1) / 3 + 1) {
                        vrgplrTypeNew[i++] = 99;
                    } else if (i < (int16_t)((game.cPlayer - 1) * 6 + 11) / 12 + 1) {
                        vrgplrTypeNew[i++] = 119;
                    } else if (i < (int16_t)((game.cPlayer - 1) * 5 + 5) / 6 + 1) {
                        vrgplrTypeNew[i++] = 115;
                    } else {
                        vrgplrTypeNew[i++] = 123;
                    }
                }
            }
            for (i = 1; i < game.cPlayer - 1; i++) {
                c = Random(game.cPlayer - i - 1) + i + 1;
                ch = vrgplrTypeNew[i];
                vrgplrTypeNew[i] = vrgplrTypeNew[c];
                vrgplrTypeNew[c] = ch;
            }
        }
    }
    return;
}

void InitNewGame3() { return; }

int16_t FGetNewGameName(char *szFileSuggest) {
    char         szXY[3];
    uint16_t     i;
    char         szFileTitle[256];
    char         szFile[256];
    char         szFilter[256];
    OPENFILENAME ofn;

    if (szFileSuggest != 0) {
        strcpy(szFile, szFileSuggest);
    } else {
        szFile[0] = 0;
    }
    CchGetString(idsStarsGameFilesXy, szFilter);
    for (i = 0; (int16_t)(int8_t)szFilter[i] != 0; i++) {
        if ((int16_t)(int8_t)szFilter[i] == 124) {
            szFilter[i] = 0;
        }
    }
    memset(&ofn, 0, sizeof(OPENFILENAME));
    szXY[0] = 'x';
    szXY[1] = 'y';
    szXY[2] = 0;
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwndFrame;
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = 0x100;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = 0x100;
    ofn.lpstrInitialDir = szDirName;
    ofn.lpstrTitle = "Choose New Game Name";
    ofn.lpstrDefExt = szXY;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_NOREADONLYRETURN;
    if (GetSaveFileName(&ofn) != 0) {
        if (ofn.nFileExtension != 0) {
            strcpy(&szFile[ofn.nFileExtension], szXY);
        } else {
            strcat(szFile, szXY);
        }
        strcpy(szFileSuggest, szFile);
        return 1;
    }
    return 0;
}

INT_PTR CALLBACK SimpleNewGameDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int16_t     i;
    RECT        rc;
    HWND        hwndDD;
    HDC         hdc;
    RECT        rcGBox;
    int16_t     dy;
    int16_t     c;
    PAINTSTRUCT ps;
    RECT       *prcSav;
    HWND        t_scratch_me;
    HWND        t_scratch_me_2;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        GetWindowRect(GetDlgItem(hwnd, 0xc8), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x00CB), &rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        rcGBox.right = rc.right;
        rcGBox.bottom = rc.bottom;
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 1);
        _Draw3dFrame(hdc, &rcGBox, -1);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        c = CchGetString(idsDifficultyLevel, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, c);
        GetWindowRect(GetDlgItem(hwnd, 0x3e8), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        GetWindowRect(GetDlgItem(hwnd, 0x3ec), &rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        rcGBox.right = rc.right;
        rcGBox.bottom = rc.bottom;
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 1);
        _Draw3dFrame(hdc, &rcGBox, -1);
        SelectObject(hdc, rghfontArial8[1]);
        c = CchGetString(idsUniverseSize, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, c);
        rcGBox.top = rcGBox.bottom + 8;
        GetWindowRect(GetDlgItem(hwnd, IDC_COMBOBOX), &rcGBox);
        MapWindowPoints(NULL, hwnd, (POINT *)&rcGBox, 2);
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 1);
        rcGBox.bottom += dyArial8 * 2;
        _Draw3dFrame(hdc, &rcGBox, -1);
        SelectObject(hdc, rghfontArial8[1]);
        c = CchGetString(idsPlayerRace, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, c);
        rcGBox.top = 3 * dyArial8 + rcGBox.bottom;
        rcGBox.bottom = 1000;
        ExpandRc(&rcGBox, -dyArial8, 0);
        c = CchGetString(idsButtonAllowsConfigureMultiPlayerGamesCustom, szWork);
        dy = DrawText(hdc, szWork, c, &rcGBox, DT_WORDBREAK | DT_NOPREFIX);
        SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x00D3), NULL, rcGBox.left, rcGBox.top + dy + dyArial8 / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        rcGBox.bottom = rcGBox.top + dy + dyArial8 * 2;
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 1);
        _Draw3dFrame(hdc, &rcGBox, -2);
        c = CchGetString(idsAdvancedGame, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, c);
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) == 0) {
        if (message == WM_INITDIALOG) {
            CheckRadioButton(hwnd, 200, 203, 201);
            CheckRadioButton(hwnd, 1000, 1004, 1001);
            hwndDD = GetDlgItem(hwnd, IDC_COMBOBOX);
            SendMessage(hwndDD, CB_RESETCONTENT, 0, 0);
            for (i = 0; i < 7; i++) {
                SendMessage(hwndDD, CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(i + 1383));
            }
            SendMessage(hwndDD, CB_SETCURSEL, 0, 0);
            StickyDlgPos(hwnd, &ptStickyNewDlg, 1);
            return 1;
        }
        if (message == WM_COMMAND) {
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDC_FINISH:
            case IDCANCEL:
            case IDC_U16_0x00D3:
                for (i = 200; i <= 203 && IsDlgButtonChecked(hwnd, i) == 0; i++) {
                }
                game.mdDensity = i - 200;
                for (i = 1000; i <= 1004 && IsDlgButtonChecked(hwnd, i) == 0; i++) {
                }
                game.mdSize = i - 1000;
                game.turn = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_COMBOBOX), CB_GETCURSEL, 0, 0));
                StickyDlgPos(hwnd, &ptStickyNewDlg, 0);
                EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam));
                return 1;
            case 0xd4:
                StickyDlgPos(hwnd, &ptStickyNewDlg, 0);
                EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam));
                return 1;
            case 0xd2:
                hwndDD = GetDlgItem(hwnd, IDC_COMBOBOX);
                game.turn = LOWORD(SendMessage(hwndDD, CB_GETCURSEL, 0, 0));
                if (game.turn < 7) {
                    vplr = vrgplrDef[game.turn];
                    CchGetString(game.turn + 1383, vplr.szName);
                    _wsprintf(vplr.szNames, "%ss", vplr.szName);
                } else {
                    vplr = *vrgplrNew;
                }
                prcSav = vrgrcRCW;
                if (RaceCreationWizard(hwnd, 0, 1) != 0) {
                    if (SendMessage(hwndDD, CB_GETCOUNT, 0, 0) > 7) {
                        SendMessage(hwndDD, CB_DELETESTRING, 7, 0);
                    }
                    SendMessage(hwndDD, CB_ADDSTRING, 0, (LPARAM)vplr.szName);
                    SendMessage(hwndDD, CB_SETCURSEL, 7, 0);
                    *vrgplrNew = vplr;
                }
                vrgrcRCW = prcSav;
                SetFocus(hwnd);
                break;
            case IDC_HELP:
                WinHelp(hwnd, szHelpFile, 1, 1002);
                return 1;
            }
        }
    } else {
        for (i = 200; i <= 203; i++) {
            t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me == GetDlgItem(hwnd, i)) {
                i = -1;
                break;
            }
        }
        if (i != -1) {
            for (i = 1000; i <= 1004; i++) {
                t_scratch_me_2 = GET_WM_CTLCOLOR_HWND(wParam, lParam);
                if (t_scratch_me_2 == GetDlgItem(hwnd, i)) {
                    i = -1;
                    break;
                }
            }
        }
        if (i == -1 || HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    }
    return 0;
}

INT_PTR CALLBACK NewGameDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int16_t     i;
    RECT        rc;
    HDC         hdc;
    RECT        rcGBox;
    int16_t     c;
    PAINTSTRUCT ps;
    int16_t     iRet;
    HWND        t_scratch_me;
    HWND        t_scratch_me_2;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        GetWindowRect(GetDlgItem(hwnd, 0x3e8), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        GetWindowRect(GetDlgItem(hwnd, 0x3ec), &rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        rcGBox.right = rc.right;
        rcGBox.bottom = rc.bottom;
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 1);
        _Draw3dFrame(hdc, &rcGBox, -1);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        c = CchGetString(idsUniverseSize, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, c);
        GetWindowRect(GetDlgItem(hwnd, 0x3ed), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        GetWindowRect(GetDlgItem(hwnd, 0x3f0), &rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        rcGBox.right = rc.right;
        rcGBox.bottom = rc.bottom;
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 1);
        _Draw3dFrame(hdc, &rcGBox, -1);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        c = CchGetString(idsDensity, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, c);
        GetWindowRect(GetDlgItem(hwnd, 0x3f1), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        GetWindowRect(GetDlgItem(hwnd, 0x3f4), &rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        rcGBox.right = rc.right;
        rcGBox.bottom = rc.bottom;
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 1);
        _Draw3dFrame(hdc, &rcGBox, -1);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        c = CchGetString(idsPlayerPositions, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, c);
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) != 0) {
        for (i = 1000; i <= 1021; i++) {
            t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me == GetDlgItem(hwnd, i))
                break;
        }
        if (i > 1021 && HIWORD(lParam) != 6) {
            t_scratch_me_2 = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me_2 != GetDlgItem(hwnd, IDC_U16_0x041A)) {
                return 0;
            }
        }
        SetBkColor((HDC)wParam, crButtonFace);
        return (INT_PTR)hbrButtonFace;
    }
    if (message == WM_INITDIALOG) {
        SetNGWTitle(hwnd, 1);
        CheckRadioButton(hwnd, 1000, 1004, game.mdSize + 1000);
        CheckRadioButton(hwnd, 1005, 1008, game.mdDensity + 1005);
        CheckRadioButton(hwnd, 1009, 1012, game.mdStartDist + 1009);
        SetWindowText(GetDlgItem(hwnd, IDC_U16_0x0406), game.szName);
        SendDlgItemMessage(hwnd, 1030, EM_LIMITTEXT, 0x1f, 0);
        SendMessage(GetDlgItem(hwnd, 0x3f8), BM_SETCHECK, game.fExtraFuel, 0);
        SendMessage(GetDlgItem(hwnd, 0x3f9), BM_SETCHECK, game.fSlowTech, 0);
        SendMessage(GetDlgItem(hwnd, 0x3fa), BM_SETCHECK, game.fBBSPlay, 0);
        SendMessage(GetDlgItem(hwnd, 0x3fb), BM_SETCHECK, game.fNoRandom, 0);
        SendMessage(GetDlgItem(hwnd, 0x3fc), BM_SETCHECK, game.fAisBand, 0);
        SendMessage(GetDlgItem(hwnd, 0x3fd), BM_SETCHECK, game.fVisScores, 0);
        SendMessage(GetDlgItem(hwnd, IDC_U16_0x041A), BM_SETCHECK, game.fClumping, 0);
        if (fRCWReadOnly != 0) {
            for (i = 1000; i <= 1004; i++) {
                EnableWindow(GetDlgItem(hwnd, i), 0);
            }
            for (i = 1005; i <= 1008; i++) {
                EnableWindow(GetDlgItem(hwnd, i), 0);
            }
            for (i = 1009; i <= 1012; i++) {
                EnableWindow(GetDlgItem(hwnd, i), 0);
            }
            EnableWindow(GetDlgItem(hwnd, 0x3f8), 0);
            EnableWindow(GetDlgItem(hwnd, 0x3f9), 0);
            EnableWindow(GetDlgItem(hwnd, 0x3fa), 0);
            EnableWindow(GetDlgItem(hwnd, 0x3fb), 0);
            EnableWindow(GetDlgItem(hwnd, IDC_U16_0x041A), 0);
            EnableWindow(GetDlgItem(hwnd, 0x3fc), 0);
            EnableWindow(GetDlgItem(hwnd, IDC_U16_0x0406), 0);
            EnableWindow(GetDlgItem(hwnd, 0x3fd), 0);
        }
        StickyDlgPos(hwnd, &ptStickyNewDlg, 1);
        return 1;
    }
    if (message == WM_COMMAND) {
        if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HELP) {
            WinHelp(hwnd, szHelpFile, 1, 1012);
            return 1;
        }
        for (iRet = 0; iRet < 4 && GET_WM_COMMAND_ID(wParam, lParam) != rgidRaceBtn[iRet]; iRet++) {
        }
        if (iRet < 4) {
            if (iRet != 0) {
                for (i = 1000; i <= 1004 && IsDlgButtonChecked(hwnd, i) == 0; i++) {
                }
                game.mdSize = i - 1000;
                for (i = 1005; i <= 1008 && IsDlgButtonChecked(hwnd, i) == 0; i++) {
                }
                game.mdDensity = i - 1005;
                for (i = 1009; i <= 1012 && IsDlgButtonChecked(hwnd, i) == 0; i++) {
                }
                game.mdStartDist = i - 1009;
                game.fExtraFuel = IsDlgButtonChecked(hwnd, 1016);
                game.fSlowTech = IsDlgButtonChecked(hwnd, 1017);
                game.fBBSPlay = IsDlgButtonChecked(hwnd, 1018);
                game.fNoRandom = IsDlgButtonChecked(hwnd, 1019);
                game.fAisBand = IsDlgButtonChecked(hwnd, 1020);
                game.fVisScores = IsDlgButtonChecked(hwnd, 1021);
                game.fClumping = IsDlgButtonChecked(hwnd, 1050);
                i = GetWindowText(GetDlgItem(hwnd, IDC_U16_0x0406), game.szName, 32);
                if (i == 0) {
                    strcpy(game.szName, szBase);
                }
            }
            StickyDlgPos(hwnd, &ptStickyNewDlg, 0);
            EndDialog(hwnd, iRet);
            return 1;
        }
    }
    return 0;
}

INT_PTR CALLBACK NewGameDlg2(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int16_t     i;
    RECT        rc;
    RECT        rcT;
    int16_t     dyBut;
    int16_t     dy;
    int16_t     dyCur;
    HWND        hwndBtn;
    POINT16     pt;
    int16_t     iDiamond;
    int16_t     iNewVal;
    int16_t     j;
    char       *psz;
    int16_t     tpm;
    int16_t     iChecked;
    HMENU       rghmenuSubPopup[14];
    HMENU       hmenuPopup;
    MSG         msg;
    int16_t     iCurVal;
    RECT       *prcSav;
    HDC         hdc;
    PAINTSTRUCT ps;
    HWND        t_scratch_me;
    POINT       t_pt_89ee;
    POINT       t_pt_89fd_1;
    POINT       t_pt_8b1f_1;
    int16_t     t_9060;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        DrawNewGame2(hwnd, hdc, -1);
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) == 0) {
        switch (message) {
        case WM_INITDIALOG:
            SetNGWTitle(hwnd, 2);
            dy = (dyArial8 + 4) * 16 + 8;
            GetWindowRect(GetDlgItem(hwnd, rgidRaceBtn[0]), &rc);
            dyBut = rc.bottom - rc.top;
            GetWindowRect(hwnd, &rcT);
            dyCur = rcT.bottom - rcT.top;
            if (dyCur < dy + dyBut + 6) {
                for (i = 0; i < 5; i++) {
                    hwndBtn = GetDlgItem(hwnd, rgidRaceBtn[i]);
                    GetWindowRect(hwndBtn, &rc);
                    MapWindowPoints(NULL, hwnd, (POINT *)&rc, 2);
                    OffsetRect(&rc, 0, dy - rc.top);
                    SetWindowPos(hwndBtn, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                }
                dy = rc.bottom + 6;
                GetClientRect(hwnd, &rc);
                if (dyCur < dy) {
                    SetWindowPos(hwnd, NULL, 0, 0, rcT.right - rcT.left, dy + rcT.bottom - rcT.top - rc.bottom, SWP_NOMOVE | SWP_NOZORDER);
                }
            }
            StickyDlgPos(hwnd, &ptStickyNewDlg, 1);
            return 1;
        case WM_SETCURSOR:
            if (fRCWReadOnly != 0)
                break;
            GetCursorPos(&t_pt_89ee);
            pt = PointTo16(t_pt_89ee);
            t_pt_89fd_1 = PointFrom16(pt);
            ScreenToClient(hwnd, &t_pt_89fd_1);
            pt = PointTo16(t_pt_89fd_1);
            if (pt.x < xNewGameDiamond || pt.x >= xNewGameDiamond + dyArial8 + 1 || pt.y < 6)
                break;
            iDiamond = (int16_t)(pt.y - 6) / (dyArial8 + 4);
            if (iDiamond >= 16 || (int16_t)(pt.y - 6) % (dyArial8 + 4) >= dyArial8 + 1)
                break;
            SetCursor(hcurHand);
            return 1;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            if (fRCWReadOnly != 0)
                break;
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            if (pt.x < xNewGameDiamond || pt.x >= xNewGameDiamond + dyArial8 + 1 || pt.y < 6)
                break;
            iDiamond = (int16_t)(pt.y - 6) / (dyArial8 + 4);
            if (iDiamond >= 16 || (int16_t)(pt.y - 6) % (dyArial8 + 4) >= dyArial8 + 1)
                break;
            iCurVal = vrgplrTypeNew[iDiamond];
            t_pt_8b1f_1 = PointFrom16(pt);
            ClientToScreen(hwnd, &t_pt_8b1f_1);
            pt = PointTo16(t_pt_8b1f_1);
            rghmenuSubPopup[0] = CreatePopupMenu();
            for (i = 0; i < 6; i++) {
                iChecked = iCurVal == i * 4 + 1 ? 8 : 0;
                psz = PszGetCompressedString(i + 1383);
                AppendMenu(rghmenuSubPopup[0], iChecked, i + 15016, psz);
            }
            AppendMenu(rghmenuSubPopup[0], iChecked, i + 15016, PszGetCompressedString(idsRandom));
            AppendMenu(rghmenuSubPopup[0], iChecked, i + 15017, PszGetCompressedString(idsExpansionPlayer));
            rghmenuSubPopup[1] = CreatePopupMenu();
            AppendMenu(rghmenuSubPopup[1], MF_BYCOMMAND, 0x3a98, PszGetCompressedString(idsNew));
            AppendMenu(rghmenuSubPopup[1], MF_BYCOMMAND, 0x3a99, PszGetCompressedString(idsOpen));
            for (i = 0; i < vcplrNew; i++) {
                iChecked = iCurVal == i * 4 + 2 ? 8 : 0;
                psz = PszPlayerName(0, 1, 1, 1, 0, vrgplrNew + i);
                AppendMenu(rghmenuSubPopup[1], iChecked, i + 15032, psz);
            }
            rghmenuSubPopup[2] = CreatePopupMenu();
            for (i = 0; i <= 6; i++) {
                rghmenuSubPopup[i + 5] = CreatePopupMenu();
                for (j = 0; j < 5; j++) {
                    iChecked = iCurVal == j * 32 + i * 4 + 5 ? 8 : 0;
                    AppendMenu(rghmenuSubPopup[i + 5], iChecked, i + 15048 + j * 8, vrgszComputerLevel[j]);
                }
                iChecked = (iCurVal & 0x1f) == i * 4 + 5 ? 8 : 0;
                AppendMenu(rghmenuSubPopup[2], 0x10 | iChecked, (UINT_PTR)rghmenuSubPopup[i + 5], vrgszComputerPlayers[i]);
            }
            hmenuPopup = CreatePopupMenu();
            iPopMenuSel = -1;
            iChecked = (iCurVal & 3) == 1 ? 8 : 0;
            AppendMenu(hmenuPopup, 0x10 | iChecked, (UINT_PTR)rghmenuSubPopup[0], PszGetCompressedString(idsPredefinedRace));
            iChecked = (iCurVal & 3) == 2 ? 8 : 0;
            AppendMenu(hmenuPopup, 0x10 | iChecked, (UINT_PTR)rghmenuSubPopup[1], PszGetCompressedString(idsCustomRace));
            if ((iCurVal & 3) == 1 || (iCurVal & 3) == 2) {
                AppendMenu(hmenuPopup, MF_BYCOMMAND, 0x3a9a, PszGetCompressedString(idsEditRace));
            }
            AppendMenu(hmenuPopup, MF_SEPARATOR, 0, NULL);
            iChecked = (iCurVal & 3) == 3 ? 8 : 0;
            AppendMenu(hmenuPopup, 0x10 | iChecked, (UINT_PTR)rghmenuSubPopup[2], PszGetCompressedString(idsComputerPlayer));
            AppendMenu(hmenuPopup, MF_SEPARATOR, 0, NULL);
            iChecked = (iCurVal & 3) == 0 ? 8 : 0;
            AppendMenu(hmenuPopup, iChecked, 0x3a9b, PszGetCompressedString(idsPlayer));
            tpm = message == WM_LBUTTONDOWN ? TPM_LEFTBUTTON : TPM_RIGHTBUTTON;
            TrackPopupMenu(hmenuPopup, 4 | tpm, pt.x, pt.y, 0, hwnd, NULL);
            DestroyMenu(hmenuPopup);
            for (i = 0; i < 6; i++) {
                DestroyMenu(rghmenuSubPopup[i]);
            }
            if (PeekMessage(&msg, hwnd, 273, 273, 2) != 0 && msg.wParam >= 15000 && msg.wParam < 15100) {
                iPopMenuSel = msg.wParam - 15000;
            }
            iNewVal = -1;
            if (iPopMenuSel != -1) {
                if (iPopMenuSel == 0) {
                    vplr = vrgplrDef[0];
                    CchGetString(idsHumanoid, vplr.szName);
                    _wsprintf(vplr.szNames, "%ss", vplr.szName);
                    prcSav = vrgrcRCW;
                    if (RaceCreationWizard(hwnd, 0, 0) != 0) {
                        vrgrcRCW = prcSav;
                        goto PlaceNew;
                    }
                    vrgrcRCW = prcSav;
                }
                switch (iPopMenuSel) {
                case 1:
                    if (FOpenGame(hwnd, 1) <= 0)
                        goto L_9367;
                    goto PlaceNew;
                case 2:
                    if ((iCurVal & 3) == 1) {
                        vplr = vrgplrDef[iCurVal >> 2];
                        CchGetString((iCurVal >> 2) + 0x567, vplr.szName);
                        _wsprintf(vplr.szNames, "%ss", vplr.szName);
                    } else {
                        vplr = vrgplrNew[iCurVal >> 2];
                        strcpy(szRaceFile, vrgszFileNew + (iCurVal >> 2) * 13);
                    }
                    lSaltCur = vplr.lSalt;
                    lSaltLast = 0;
                    if (FCheckPassword() == 0)
                        break;
                    if (vplr.lSalt != 0) {
                        strcpy(szRacePass, szPassLast);
                    } else {
                        szRacePass[0] = 0;
                    }
                    prcSav = vrgrcRCW;
                    if (RaceCreationWizard(hwnd, 0, 0) != 0) {
                        vrgrcRCW = prcSav;
                        if ((iCurVal & 3) == 1 || strcmp(szRaceFile, vrgszFileNew + (iCurVal >> 2) * 13) != 0)
                            goto PlaceNew;
                        vrgplrNew[iCurVal >> 2] = vplr;
                        strcpy(vrgszFileNew + (iCurVal >> 2) * 13, szRaceFile);
                        iNewVal = iCurVal;
                        iCurVal = -1;
                    }
                    vrgrcRCW = prcSav;
                    goto L_9367;
                case 3:
                    iNewVal = 0;
                    goto L_9367;
                default:
                    if (iPopMenuSel >= 16 && iPopMenuSel < 32) {
                        iNewVal = (iPopMenuSel - 0x10) << 2 | 1;
                        goto L_9367;
                    }
                    if (iPopMenuSel >= 32 && iPopMenuSel < 48) {
                        iNewVal = (iPopMenuSel - 0x20) << 2 | 2;
                        goto L_9367;
                    }
                    if (iPopMenuSel < 48)
                        goto L_9367;
                    iNewVal = (iPopMenuSel - 0x30) << 2 | 3;
                    goto L_9367;
                }
                goto FinishClick;
            PlaceNew:
                if (vcplrNew < 16) {
                    t_9060 = vcplrNew;
                    vcplrNew++;
                    iNewVal = t_9060;
                } else {
                    for (iNewVal = 15; iNewVal >= 0; iNewVal--) {
                        for (i = 0; i < 16 && ((vrgplrTypeNew[i] & 3) != 2 || (vrgplrTypeNew[i] >> 2 & 0xf) != iNewVal); i++) {
                        }
                        if (i == 16)
                            break;
                    }
                }
                vrgplrNew[iNewVal] = vplr;
                strcpy(vrgszFileNew + iNewVal * 13, szRaceFile);
                iNewVal = iNewVal << 2 | 2;
            }
        L_9367:
            if (iNewVal != -1 && iNewVal != iCurVal) {
                vrgplrTypeNew[iDiamond] = LOBYTE(iNewVal);
                if (iNewVal == 0) {
                    while (1) {
                        iDiamond++;
                        if (iDiamond >= 16)
                            break;
                        vrgplrTypeNew[iDiamond] = 0;
                    }
                } else if (iDiamond > 0) {
                    while (1) {
                        iDiamond--;
                        if (iDiamond < 0 || vrgplrTypeNew[iDiamond] != 0)
                            break;
                        vrgplrTypeNew[iDiamond] = LOBYTE(iNewVal);
                    }
                }
                InvalidateRect(hwnd, NULL, 1);
            }
        FinishClick:
            SetFocus(hwnd);
            break;
        case WM_COMMAND:
            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HELP) {
                WinHelp(hwnd, szHelpFile, 1, 1020);
                return 1;
            }
            for (i = 0; i < 4 && GET_WM_COMMAND_ID(wParam, lParam) != rgidRaceBtn[i]; i++) {
            }
            if (i < 4) {
                if (i != 0 && vrgplrTypeNew[0] == 0 && fRCWReadOnly == 0) {
                    AlertSz(PszFormatIds(idsMustHaveLeastOnePlayerGame, NULL), MB_ICONHAND);
                } else {
                    StickyDlgPos(hwnd, &ptStickyNewDlg, 0);
                    EndDialog(hwnd, i);
                    return 1;
                }
            }
        }
    } else {
        for (i = 401; i <= 448; i++) {
            t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 448 || HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    }
    return 0;
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

    fCreatedDC = 0;
    if (hdc == 0) {
        fCreatedDC = 1;
        hdc = GetDC(hwnd);
    }
    GetClientRect(hwnd, &rc);
    bkMode = SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, rghfontArial8[1]);
    xNewGameDiamond = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsPlayer16), 11)) + 12;
    yCur = 6;
    SetRect(&rcDiamond, xNewGameDiamond, yCur, xNewGameDiamond + dyArial8 + 1, yCur + dyArial8 + 1);
    for (i = 0; i < 16 && (fRCWReadOnly == 0 || i < game.cPlayer); i++) {
        cch = _wsprintf(szWork, PszGetCompressedString(idsPlayerD), i + 1);
        szWork[cch++] = ':';
        szWork[cch] = 0;
        RightTextOut(hdc, xNewGameDiamond - 6, yCur, szWork, cch, 0);
        DrawDiamond(hdc, &rcDiamond, hbrBBlue);
        iPlr = vrgplrTypeNew[i] >> 2 & 0xf;
        if (fRCWReadOnly != 0) {
            if (rgplr[i].fInclude != 0) {
                PszPlayerName(i, 1, 1, 1, 0, NULL);
                if (rgplr[i].fDead != 0) {
                    _wsprintf(&szWork[strlen(szWork)], " (%s)", PszGetCompressedString(idsDeceased));
                    SetTextColor(hdc, 127);
                }
            } else {
                CchGetString(idsUnknownPlayer, szWork);
            }
        } else {
            switch (vrgplrTypeNew[i] & 3) {
            case 0:
                CchGetString(idsPlayer2, szWork);
                break;
            case 1:
                if (iPlr >= 6) {
                    ids = iPlr != 6 ? idsExpansionPlayer : fRCWReadOnly != 0 ? idsUnknownPlayer : idsRandom;
                    CchGetString(ids, szWork);
                    break;
                }
                CchGetString(idsS, szT);
                _wsprintf(szWork, szT, PszGetCompressedString(iPlr + 1383));
                break;
            case 2:
                if (fRCWReadOnly == 0 && gd.fNoHostNames != 0 && (int16_t)(int8_t)vrgszFileNew[iPlr * 13] != 0) {
                    _wsprintf(szWork, " %s", vrgszFileNew + iPlr * 13);
                    break;
                }
                if ((int16_t)(int8_t)vrgszFileNew[iPlr * 13] != 0) {
                    _wsprintf(szWork, PszGetCompressedString(fRCWReadOnly == 0 ? idsSS2 : idsS), vrgplrNew[iPlr].szNames, vrgszFileNew + iPlr * 13);
                    break;
                }
                _wsprintf(szWork, PszGetCompressedString(idsS), vrgplrNew[iPlr].szNames);
                break;
            case 3:
                _wsprintf(szWork, PszGetCompressedString(idsSSComputerPlayer), vrgszComputerPlayers[iPlr & 7], vrgszComputerLevel[vrgplrTypeNew[i] >> 5]);
            }
        }
        TextOut(hdc, rcDiamond.right + 6, yCur, szWork, strlen(szWork));
        SetTextColor(hdc, crButtonText);
        OffsetRect(&rcDiamond, 0, dyArial8 + 4);
        yCur += dyArial8 + 4;
    }
    SetBkMode(hdc, bkMode);
    if (fCreatedDC != 0) {
        ReleaseDC(hwnd, hdc);
    }
    return;
}

INT_PTR CALLBACK NewGameDlg3(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int16_t     i;
    RECT        rc;
    POINT16     pt;
    HDC         hdc;
    PAINTSTRUCT ps;
    HWND        t_call_9a24;
    HWND        t_scratch_me;
    POINT       t_pt_9b4c;
    POINT       t_pt_9b5b_1;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        DrawNewGame3(hwnd, hdc, -1);
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) == 0) {
        switch (message) {
        case WM_INITDIALOG:
            SetNGWTitle(hwnd, 3);
            for (i = 0; i < 7; i++) {
                t_call_9a24 = GetDlgItem(hwnd, i + 291);
                SendMessage(t_call_9a24, BM_SETCHECK, GetVCCheck(&game, (i < 2 ? 0 : 1) + i), 0);
                if (fRCWReadOnly != 0) {
                    EnableWindow(GetDlgItem(hwnd, i + 291), 0);
                }
            }
            StickyDlgPos(hwnd, &ptStickyNewDlg, 1);
            return 1;
        case WM_SETCURSOR:
            GetCursorPos(&t_pt_9b4c);
            pt = PointTo16(t_pt_9b4c);
            t_pt_9b5b_1 = PointFrom16(pt);
            ScreenToClient(hwnd, &t_pt_9b5b_1);
            pt = PointTo16(t_pt_9b5b_1);
            if (IrcRaceDlgHitTest(pt) < 0)
                break;
            SetCursor(hcurHand);
            return 1;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            return FTrackNewGameDlg3(hwnd, pt, wParam);
        case WM_COMMAND:
            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HELP) {
                WinHelp(hwnd, szHelpFile, 1, 1021);
                return 1;
            }
            for (i = 0; i < 4 && GET_WM_COMMAND_ID(wParam, lParam) != rgidRaceBtn[i]; i++) {
            }
            if (i < 4) {
                StickyDlgPos(hwnd, &ptStickyNewDlg, 0);
                EndDialog(hwnd, i);
                return 1;
            }
            if (GET_WM_COMMAND_ID(wParam, lParam) >= IDC_U16_0x0123 && GET_WM_COMMAND_ID(wParam, lParam) <= 0x129) {
                i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), BM_GETCHECK, 0, 0));
                SetVCCheck(&game, GET_WM_COMMAND_ID(wParam, lParam) - 291 + ((uint16_t)(GET_WM_COMMAND_ID(wParam, lParam) - 291) < 2 ? 0 : 1), i);
                DrawNewGame3(hwnd, NULL, 8);
            }
        }
    } else {
        for (i = 291; i <= 297; i++) {
            t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 297 || HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    }
    return 0;
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

    fCreatedDC = 0;
    bt = fRCWReadOnly == 0 ? 0 : 4;
    if (hdc == 0) {
        fCreatedDC = 1;
        hdc = GetDC(hwnd);
    }
    GetClientRect(hwnd, &rc);
    bkMode = SetBkMode(hdc, OPAQUE);
    crBkSav = SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[1]);
    yTop = 3 * dyArial8 + 6;
    dxDig = LOWORD(GetTextExtent(hdc, "9", 1));
    ids = idsOwns;
    irc = 0;
    vcCur = 0;
    for (i = 0; i < 9; i++) {
        if (i < 7) {
            GetWindowRect(GetDlgItem(hwnd, i + 291), &rcCBox);
            MapWindowPoints(NULL, hwnd, (POINT *)&rcCBox, 2);
            xLeft = rcCBox.right + 2;
            yTop = (int16_t)(rcCBox.bottom - rcCBox.top) / 2 + rcCBox.top - (dyArial8 >> 1);
        } else if (i == 7) {
            xLeft = rcCBox.left;
            yTop = rcCBox.bottom + 6;
        } else {
            xLeft = rcCBox.left;
            yTop = rcCBox.bottom + 6 + (int16_t)(3 * dyArial8) / 2;
        }
        cch = CchGetString(ids++, szWork);
        if (iDraw == -1) {
            TextOut(hdc, xLeft, yTop, szWork, cch);
        }
        xLeft += LOWORD(GetTextExtent(hdc, szWork, cch));
        for (j = 0; j < 2; j++) {
            dxItem = abs((int16_t)(int8_t)rgNG3Width[i][j]) * dxDig;
            if (dxItem == 0) {
                ids++;
                break;
            }
            _wsprintf(szWork, PCTD, GetVCVal(&game, vcCur, 0));
            if ((int16_t)(int8_t)rgNG3Width[i][j] < 0) {
                dxItem += (int16_t)(3 * dxDig) / 2;
                strcat(szWork, "%");
            }
            xLeft += dxItem;
            if (iDraw == -1 || iDraw == vcCur || vcCur == 8) {
                RightTextOut(hdc, xLeft, yTop, szWork, 0, dxItem);
            }
            vcCur++;
            vrgrcRCW[irc].left = xLeft + 4;
            vrgrcRCW[irc].top = yTop - 3;
            vrgrcRCW[irc].right = vrgrcRCW[irc].left + 15;
            vrgrcRCW[irc].bottom = (dyArial8 >> 1) + vrgrcRCW[irc].top + 3;
            vrgrcRCW[irc + 1] = vrgrcRCW[irc];
            OffsetRect((RECT *)&vrgrcRCW[irc + 1].left, 0, vrgrcRCW[irc].bottom - vrgrcRCW[irc].top - 1);
            if (iDraw == -1) {
                DrawBtn(hdc, vrgrcRCW + irc, 0xa0 | bt, 0, NULL);
                DrawBtn(hdc, vrgrcRCW + (irc + 1), 0xa1 | bt, 0, NULL);
            }
            xLeft = vrgrcRCW[irc].right + 4;
            cch = CchGetString(ids, szWork);
            if (iDraw == -1) {
                TextOut(hdc, xLeft, yTop, szWork, cch);
            }
            xLeft += LOWORD(GetTextExtent(hdc, szWork, cch));
            ids++;
            irc += 2;
        }
    }
    crcRCW = irc;
    SetBkColor(hdc, crBkSav);
    SetBkMode(hdc, bkMode);
    if (fCreatedDC != 0) {
        ReleaseDC(hwnd, hdc);
    }
    return;
}

int16_t FTrackNewGameDlg3(HWND hwnd, POINT16 pt, int16_t kbd) {
    int16_t bt;
    int16_t irc;
    BTNT    btnt;
    int16_t i;
    int16_t dShift;
    int16_t iMod;
    int16_t iStat;

    irc = IrcRaceDlgHitTest(pt);
    if (irc < 0) {
        return 0;
    }
    iMod = irc & 1;
    i = irc >> 1;
    if (iMod == 0) {
        dShift = 1;
        bt = 160;
    } else {
        dShift = -1;
        bt = 161;
    }
    InitBtnTrack(&btnt, hwnd, NULL, vrgrcRCW + irc, bt, 80, 0, 0, NULL);
    if ((kbd & 0xc) != 0) {
        dShift = 5 * dShift;
    }
    while (FTrackBtn(&btnt) != 0) {
        iStat = GetVCVal(&game, i, 1);
        if (SetVCVal(&game, i, iStat + dShift) != iStat) {
            DrawNewGame3(hwnd, btnt.hdc, i);
        }
    }
    return 1;
}

void SetNGWTitle(HWND hwnd, int16_t iStep) {
    int16_t cch;
    char    szBuf[50];

    cch = CchGetString(fRCWReadOnly + 272, szBuf);
    cch = _wsprintf(szWork, szBuf, iStep);
    SetWindowText(hwnd, szWork);
    return;
}

PLAYER *LpplrComp(int16_t idAi, AiLevel lvlAi) { return &vrgplrComp[idAi][lvlAi]; }

void SetVCCheck(GAME *pgame, int16_t vc, int16_t fChecked) {
    pgame->rgvc[vc] = LOBYTE((pgame->rgvc[vc] & 0x7f) | (fChecked == 0 ? 0 : 0x80));
    return;
}

int16_t GetVCCheck(GAME *pgame, int16_t vc) {
    if ((pgame->rgvc[vc] & 0x80) != 0) {
        return 1;
    }
    return 0;
}

int16_t SetVCVal(GAME *pgame, int16_t vc, int16_t val) {
    int16_t cur;

    if (val < 0) {
        val = 0;
    } else if (val > vrgvcMax[vc]) {
        val = vrgvcMax[vc];
    }
    pgame->rgvc[vc] = LOBYTE((pgame->rgvc[vc] & 0x80) | (val & 0xff));
    if (vc == 8) {
        cur = GetVCVal(pgame, 8, 0);
        if (cur != val) {
            val = cur;
            pgame->rgvc[8] = LOBYTE((pgame->rgvc[8] & 0x80) | (cur & 0xff));
        }
    }
    return val;
}

int16_t GetVCVal(GAME *pgame, int16_t vc, int16_t fRaw) {
    int16_t c;
    int16_t i;
    int16_t val;

    val = pgame->rgvc[vc] & 0x7f;
    if (fRaw != 0) {
        return val;
    }
    if ((uint16_t)vc <= 9) {
        switch (vc) {
        case 0:
            val = 5 * val + 20;
            break;
        case 1:
            val += 8;
            break;
        case 2:
            val += 2;
            break;
        case 3:
            val = 1000 * val + 1000;
            break;
        case 4:
            val = 10 * val + 20;
            break;
        case 5:
        case 6:
            val = 10 * val + 10;
            break;
        case 7:
            val = 10 * val + 30;
            break;
        case 9:
            val = 10 * val + 30;
            break;
        case 8:
            goto L_b7b8;
        }
        return val;
    }
L_b7b8:
    c = 0;
    for (i = 0; i < 8; i++) {
        if (i != 2 && (game.rgvc[i] & 0x80) != 0) {
            c++;
        }
    }
    if (c < val) {
        val = c;
    }
    return val;
}
