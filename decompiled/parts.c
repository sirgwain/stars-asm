#include "common.h"

SHDEF     rgshdefSBT[4] = {{
                               .hul =
                               {
                                       .ihuldef = ihuldefSpaceStation,
                                       .szClass = "Starbase",
                                       .resCost = 276,
                                       .rgwtOreCost = {67, 80, 53},
                                       .ibmp = 8,
                                       .wtCargoMax = 65535,
                                       .dp = 1000,
                                       .rghs = {{.grhst = hstSpecialSB},
                                                {.grhst = hstBeam, .cItem = 8},
                                                {.grhst = hstShield, .cItem = 8},
                                                {.grhst = hstBeam, .cItem = 8},
                                                {.grhst = hstShield, .cItem = 8},
                                                {.grhst = hstShield, .cItem = 8},
                                                {.grhst = hstSpecialEM},
                                                {.grhst = hstBeam, .cItem = 8},
                                                {.grhst = hstSpecialEM},
                                                {.grhst = hstBeam, .cItem = 8},
                                                {.grhst = hstSpecialSB, .iItem = 1},
                                                {.grhst = hstShield, .cItem = 8}},
                                       .chs = 12,
                               },
                               .det = detMinimal,
                               .fInclude = 1,
                               .ishdef = 16,
                               .wFlags = 16641,
                               .turn = 1,
                       },
                           {
                               .hul =
                               {
                                       .ihuldef = ihuldefOrbitalFort,
                                       .szClass = "Accelerator Platform",
                                       .resCost = 276,
                                       .rgwtOreCost = {67, 80, 53},
                                       .wtCargoMax = 65535,
                                       .dp = 1000,
                                       .rghs = {{.grhst = hstSpecialSB, .iItem = 7, .cItem = 1},
                                                {.grhst = hstBeam, .cItem = 6},
                                                {.grhst = hstShield, .iItem = 1, .cItem = 6},
                                                {.grhst = hstBeam, .cItem = 6},
                                                {.grhst = hstShield, .iItem = 1, .cItem = 6}},
                                       .chs = 5,
                               },
                               .det = detMinimal,
                               .fInclude = 1,
                               .ishdef = 17,
                               .wFlags = 17665,
                               .turn = 1,
                       },
                           {
                               .hul =
                               {
                                       .ihuldef = ihuldefOrbitalFort,
                                       .szClass = "Porthole to Beyond",
                                       .resCost = 276,
                                       .rgwtOreCost = {67, 80, 53},
                                       .ibmp = 1,
                                       .wtCargoMax = 65535,
                                       .dp = 1000,
                                       .rghs = {{.grhst = hstSpecialSB, .cItem = 1},
                                                {.grhst = hstBeam, .cItem = 6},
                                                {.grhst = hstShield, .cItem = 6},
                                                {.grhst = hstBeam, .cItem = 6},
                                                {.grhst = hstShield, .cItem = 6}},
                                       .chs = 5,
                               },
                               .det = detMinimal,
                               .fInclude = 1,
                               .ishdef = 18,
                               .wFlags = 18689,
                               .turn = 1,
                       },
                           {
                               .hul =
                               {
                                       .ihuldef = ihuldefOrbitalFort,
                                       .szClass = "Starter Colony",
                                       .resCost = 276,
                                       .rgwtOreCost = {67, 80, 53},
                                       .ibmp = 1,
                                       .wtCargoMax = 65535,
                                       .dp = 1000,
                                       .rghs = {{.grhst = hstSpecialSB}, {.grhst = hstBeam}, {.grhst = hstShield}, {.grhst = hstBeam}, {.grhst = hstShield}},
                                       .chs = 5,
                               },
                               .det = detMinimal,
                               .fInclude = 1,
                               .ishdef = 19,
                               .wFlags = 19713,
                               .turn = 1,
                       }};
BEAM      rgbeam[24] = {{
                            .id = 1,
                            .szName = "Laser",
                            .cMass = 1,
                            .resCost = 5,
                            .rgwtOreCost = {0, 6},
                            .ibmp = 28,
                            .dRangeMax = 1,
                            .dp = 10,
                            .init = 9,
                   },
                        {
                            .id = 2,
                            .rgTech = {0, 3},
                            .szName = "X-Ray Laser",
                            .cMass = 1,
                            .resCost = 6,
                            .rgwtOreCost = {0, 6},
                            .ibmp = 29,
                            .dRangeMax = 1,
                            .dp = 16,
                            .init = 9,
                   },
                        {
                            .id = 3,
                            .rgTech = {0, 5},
                            .szName = "Mini Gun",
                            .cMass = 3,
                            .resCost = 10,
                            .rgwtOreCost = {0, 16},
                            .ibmp = 20,
                            .dRangeMax = 2,
                            .dp = 13,
                            .init = 12,
                            .grfAbilities = 2,
                   },
                        {
                            .id = 4,
                            .rgTech = {0, 6},
                            .szName = "Yakimora Light Phaser",
                            .cMass = 1,
                            .resCost = 7,
                            .rgwtOreCost = {0, 8},
                            .ibmp = 19,
                            .dRangeMax = 1,
                            .dp = 26,
                            .init = 9,
                   },
                        {
                            .id = 5,
                            .rgTech = {0, 7},
                            .szName = "Blackjack",
                            .cMass = 10,
                            .resCost = 7,
                            .rgwtOreCost = {0, 16},
                            .ibmp = 14,
                            .dp = 90,
                            .init = 10,
                   },
                        {
                            .id = 6,
                            .rgTech = {0, 8},
                            .szName = "Phaser Bazooka",
                            .cMass = 2,
                            .resCost = 11,
                            .rgwtOreCost = {0, 8},
                            .ibmp = 21,
                            .dRangeMax = 2,
                            .dp = 26,
                            .init = 7,
                   },
                        {
                            .id = 7,
                            .rgTech = {5, 9},
                            .szName = "Pulsed Sapper",
                            .cMass = 1,
                            .resCost = 12,
                            .rgwtOreCost = {0, 0, 4},
                            .ibmp = 17,
                            .dRangeMax = 3,
                            .dp = 82,
                            .init = 14,
                            .grfAbilities = 1,
                   },
                        {
                            .id = 8,
                            .rgTech = {0, 10},
                            .szName = "Colloidal Phaser",
                            .cMass = 2,
                            .resCost = 18,
                            .rgwtOreCost = {0, 14},
                            .ibmp = 192,
                            .dRangeMax = 3,
                            .dp = 26,
                            .init = 5,
                   },
                        {
                            .id = 9,
                            .rgTech = {0, 11},
                            .szName = "Gatling Gun",
                            .cMass = 3,
                            .resCost = 13,
                            .rgwtOreCost = {0, 20},
                            .ibmp = 26,
                            .dRangeMax = 2,
                            .dp = 31,
                            .init = 12,
                            .grfAbilities = 2,
                   },
                        {
                            .id = 10,
                            .rgTech = {0, 12},
                            .szName = "Mini Blaster",
                            .cMass = 1,
                            .resCost = 9,
                            .rgwtOreCost = {0, 10},
                            .ibmp = 24,
                            .dRangeMax = 1,
                            .dp = 66,
                            .init = 9,
                   },
                        {
                            .id = 11,
                            .rgTech = {0, 13},
                            .szName = "Bludgeon",
                            .cMass = 10,
                            .resCost = 9,
                            .rgwtOreCost = {0, 22},
                            .ibmp = 15,
                            .dp = 231,
                            .init = 10,
                   },
                        {
                            .id = 12,
                            .rgTech = {0, 14},
                            .szName = "Mark IV Blaster",
                            .cMass = 2,
                            .resCost = 15,
                            .rgwtOreCost = {0, 12},
                            .ibmp = 25,
                            .dRangeMax = 2,
                            .dp = 66,
                            .init = 7,
                   },
                        {
                            .id = 13,
                            .rgTech = {8, 15},
                            .szName = "Phased Sapper",
                            .cMass = 1,
                            .resCost = 16,
                            .rgwtOreCost = {0, 0, 6},
                            .ibmp = 18,
                            .dRangeMax = 3,
                            .dp = 211,
                            .init = 14,
                            .grfAbilities = 1,
                   },
                        {
                            .id = 14,
                            .rgTech = {0, 16},
                            .szName = "Heavy Blaster",
                            .cMass = 2,
                            .resCost = 25,
                            .rgwtOreCost = {0, 20},
                            .ibmp = 193,
                            .dRangeMax = 3,
                            .dp = 66,
                            .init = 5,
                   },
                        {
                            .id = 15,
                            .rgTech = {0, 17},
                            .szName = "Gatling Neutrino Cannon",
                            .cMass = 3,
                            .resCost = 17,
                            .rgwtOreCost = {0, 28},
                            .ibmp = 30,
                            .dRangeMax = 2,
                            .dp = 80,
                            .init = 13,
                            .grfAbilities = 2,
                   },
                        {
                            .id = 16,
                            .rgTech = {0, 18},
                            .szName = "Myopic Disruptor",
                            .cMass = 1,
                            .resCost = 12,
                            .rgwtOreCost = {0, 14},
                            .ibmp = 194,
                            .dRangeMax = 1,
                            .dp = 169,
                            .init = 9,
                   },
                        {
                            .id = 17,
                            .rgTech = {0, 19},
                            .szName = "Blunderbuss",
                            .cMass = 10,
                            .resCost = 13,
                            .rgwtOreCost = {0, 30},
                            .ibmp = 13,
                            .dp = 592,
                            .init = 11,
                   },
                        {
                            .id = 18,
                            .rgTech = {0, 20},
                            .szName = "Disruptor",
                            .cMass = 2,
                            .resCost = 20,
                            .rgwtOreCost = {0, 16},
                            .ibmp = 27,
                            .dRangeMax = 2,
                            .dp = 169,
                            .init = 8,
                   },
                        {
                            .id = 19,
                            .rgTech = {21, 21, 0, 0, 16, 12},
                            .szName = "Multi Contained Munition",
                            .cMass = 8,
                            .resCost = 40,
                            .rgwtOreCost = {6, 40, 6},
                            .ibmp = 111,
                            .dRangeMax = 3,
                            .dp = 140,
                            .init = 6,
                   },
                        {
                            .id = 20,
                            .rgTech = {11, 21},
                            .szName = "Syncro Sapper",
                            .cMass = 1,
                            .resCost = 21,
                            .rgwtOreCost = {0, 0, 8},
                            .ibmp = 16,
                            .dRangeMax = 3,
                            .dp = 541,
                            .init = 14,
                            .grfAbilities = 1,
                   },
                        {
                            .id = 21,
                            .rgTech = {0, 22},
                            .szName = "Mega Disruptor",
                            .cMass = 2,
                            .resCost = 33,
                            .rgwtOreCost = {0, 30},
                            .ibmp = 195,
                            .dRangeMax = 3,
                            .dp = 169,
                            .init = 6,
                   },
                        {
                            .id = 22,
                            .rgTech = {0, 23},
                            .szName = "Big Mutha Cannon",
                            .cMass = 3,
                            .resCost = 23,
                            .rgwtOreCost = {0, 36},
                            .ibmp = 31,
                            .dRangeMax = 2,
                            .dp = 204,
                            .init = 13,
                            .grfAbilities = 2,
                   },
                        {
                            .id = 23,
                            .rgTech = {0, 24},
                            .szName = "Streaming Pulverizer",
                            .cMass = 1,
                            .resCost = 16,
                            .rgwtOreCost = {0, 20},
                            .ibmp = 22,
                            .dRangeMax = 1,
                            .dp = 433,
                            .init = 9,
                   },
                        {
                            .id = 24,
                            .rgTech = {0, 26},
                            .szName = "Anti-Matter Pulverizer",
                            .cMass = 2,
                            .resCost = 27,
                            .rgwtOreCost = {0, 22},
                            .ibmp = 23,
                            .dRangeMax = 2,
                            .dp = 433,
                            .init = 8,
                   }};
SCANNER   rgscanner[16] = {{
                               .id = 1,
                               .szName = "Bat Scanner",
                               .cMass = 2,
                               .resCost = 1,
                               .rgwtOreCost = {1, 0, 1},
                               .ibmp = 59,
                         },
                           {
                               .id = 2,
                               .rgTech = {0, 0, 0, 0, 1},
                               .szName = "Rhino Scanner",
                               .cMass = 5,
                               .resCost = 3,
                               .rgwtOreCost = {3, 0, 2},
                               .ibmp = 48,
                               .dRange = 50,
                         },
                           {
                               .id = 3,
                               .rgTech = {0, 0, 0, 0, 4},
                               .szName = "Mole Scanner",
                               .cMass = 2,
                               .resCost = 9,
                               .rgwtOreCost = {2, 0, 2},
                               .ibmp = 49,
                               .dRange = 100,
                         },
                           {
                               .id = 4,
                               .rgTech = {0, 0, 3, 0, 0, 6},
                               .szName = "DNA Scanner",
                               .cMass = 2,
                               .resCost = 5,
                               .rgwtOreCost = {1, 1, 1},
                               .ibmp = 52,
                               .dRange = 125,
                         },
                           {
                               .id = 5,
                               .rgTech = {0, 0, 0, 0, 5},
                               .szName = "Possum Scanner",
                               .cMass = 3,
                               .resCost = 18,
                               .rgwtOreCost = {3, 0, 3},
                               .ibmp = 61,
                               .dRange = 150,
                         },
                           {
                               .id = 6,
                               .rgTech = {4, 0, 0, 0, 4, 4},
                               .szName = "Pick Pocket Scanner",
                               .cMass = 15,
                               .resCost = 35,
                               .rgwtOreCost = {8, 10, 6},
                               .ibmp = 56,
                               .dRange = 80,
                               .grfAbilities = 4,
                         },
                           {
                               .id = 7,
                               .rgTech = {3, 0, 0, 0, 6},
                               .szName = "Chameleon Scanner",
                               .cMass = 6,
                               .resCost = 25,
                               .rgwtOreCost = {4, 6, 4},
                               .ibmp = 63,
                               .dRange = 160,
                               .grfAbilities = 4,
                         },
                           {
                               .id = 8,
                               .rgTech = {3, 0, 0, 0, 7, 2},
                               .szName = "Ferret Scanner",
                               .cMass = 2,
                               .resCost = 36,
                               .rgwtOreCost = {2, 0, 8},
                               .ibmp = 53,
                               .dRange = 185,
                               .grfAbilities = 1,
                         },
                           {
                               .id = 9,
                               .rgTech = {5, 0, 0, 0, 10, 4},
                               .szName = "Dolphin Scanner",
                               .cMass = 4,
                               .resCost = 40,
                               .rgwtOreCost = {5, 5, 10},
                               .ibmp = 54,
                               .dRange = 220,
                               .grfAbilities = 2,
                         },
                           {
                               .id = 10,
                               .rgTech = {4, 0, 0, 0, 8},
                               .szName = "Gazelle Scanner",
                               .cMass = 5,
                               .resCost = 24,
                               .rgwtOreCost = {4, 0, 5},
                               .ibmp = 50,
                               .dRange = 225,
                         },
                           {
                               .id = 11,
                               .rgTech = {0, 0, 5, 0, 0, 10},
                               .szName = "RNA Scanner",
                               .cMass = 2,
                               .resCost = 20,
                               .rgwtOreCost = {1, 1, 2},
                               .ibmp = 60,
                               .dRange = 230,
                         },
                           {
                               .id = 12,
                               .rgTech = {5, 0, 0, 0, 11},
                               .szName = "Cheetah Scanner",
                               .cMass = 4,
                               .resCost = 50,
                               .rgwtOreCost = {3, 1, 13},
                               .ibmp = 62,
                               .dRange = 275,
                         },
                           {
                               .id = 13,
                               .rgTech = {6, 0, 0, 0, 16, 7},
                               .szName = "Elephant Scanner",
                               .cMass = 6,
                               .resCost = 70,
                               .rgwtOreCost = {8, 5, 14},
                               .ibmp = 55,
                               .dRange = 300,
                               .grfAbilities = 3,
                         },
                           {
                               .id = 14,
                               .rgTech = {6, 0, 0, 0, 14},
                               .szName = "Eagle Eye Scanner",
                               .cMass = 3,
                               .resCost = 64,
                               .rgwtOreCost = {3, 2, 21},
                               .ibmp = 51,
                               .dRange = 335,
                         },
                           {
                               .id = 15,
                               .rgTech = {10, 0, 0, 0, 15, 10},
                               .szName = "Robber Baron Scanner",
                               .cMass = 20,
                               .resCost = 90,
                               .rgwtOreCost = {10, 10, 10},
                               .ibmp = 57,
                               .dRange = 220,
                               .grfAbilities = 4,
                         },
                           {
                               .id = 16,
                               .rgTech = {7, 0, 0, 0, 24},
                               .szName = "Peerless Scanner",
                               .cMass = 4,
                               .resCost = 90,
                               .rgwtOreCost = {3, 2, 30},
                               .ibmp = 58,
                               .dRange = 500,
                         }};
HULDEF    rghuldefSB[5] = {{
                               .hul =
                                {
                                       .ihuldef = ihuldefOrbitalFort,
                                       .szClass = "Orbital Fort",
                                       .resCost = 80,
                                       .rgwtOreCost = {24, 0, 34},
                                       .ibmp = 128,
                                       .dp = 100,
                                       .rghs = {{.grhst = hstSomeSB, .cItem = 1},
                                                {.grhst = hstWeapon, .cItem = 12},
                                                {.grhst = hstShArm, .cItem = 12},
                                                {.grhst = hstWeapon, .cItem = 12},
                                                {.grhst = hstShArm, .cItem = 12}},
                                       .chs = 5,
                                },
                               .init = 10,
                               .imdAttack = 2,
                               .wrcCargo = 65535,
                               .rgbrc = {68, 36, 70, 100, 66},
                        },
                           {
                               .hul =
                                {
                                       .ihuldef = ihuldefSpaceDock,
                                       .rgTech = {0, 0, 0, 4},
                                       .szClass = "Space Dock",
                                       .resCost = 200,
                                       .rgwtOreCost = {40, 10, 50},
                                       .ibmp = 132,
                                       .wtCargoMax = 200,
                                       .dp = 250,
                                       .rghs = {{.grhst = hstSomeSB, .cItem = 1},
                                                {.grhst = hstWeapon, .cItem = 16},
                                                {.grhst = hstShArm, .cItem = 24},
                                                {.grhst = hstWeapon, .cItem = 16},
                                                {.grhst = hstShield, .cItem = 24},
                                                {.grhst = hstSpecialE, .cItem = 2},
                                                {.grhst = hstSpecialE, .cItem = 2},
                                                {.grhst = hstWeapon, .cItem = 16}},
                                       .chs = 8,
                                },
                               .init = 12,
                               .imdAttack = 2,
                               .wrcCargo = 17510,
                               .rgbrc = {34, 20, 65, 71, 116, 38, 102, 98},
                        },
                           {
                               .hul =
                                {
                                       .ihuldef = ihuldefSpaceStation,
                                       .szClass = "Space Station",
                                       .resCost = 1200,
                                       .rgwtOreCost = {240, 160, 500},
                                       .ibmp = 136,
                                       .wtCargoMax = 65535,
                                       .dp = 500,
                                       .rghs = {{.grhst = hstSomeSB, .cItem = 1},
                                                {.grhst = hstWeapon, .cItem = 16},
                                                {.grhst = hstShield, .cItem = 16},
                                                {.grhst = hstWeapon, .cItem = 16},
                                                {.grhst = hstShArm, .cItem = 16},
                                                {.grhst = hstShield, .cItem = 16},
                                                {.grhst = hstSpecialE, .cItem = 3},
                                                {.grhst = hstWeapon, .cItem = 16},
                                                {.grhst = hstSpecialE, .cItem = 3},
                                                {.grhst = hstWeapon, .cItem = 16},
                                                {.grhst = hstSomeSB, .cItem = 1},
                                                {.grhst = hstShArm, .cItem = 16}},
                                       .chs = 12,
                                },
                               .init = 14,
                               .imdAttack = 2,
                               .wrcCargo = 17510,
                               .rgbrc = {66, 5, 3, 88, 80, 133, 100, 131, 36, 48, 70, 56},
                        },
                           {
                               .hul =
                                {
                                       .ihuldef = ihuldefUltraStation,
                                       .rgTech = {0, 0, 0, 12},
                                       .szClass = "Ultra Station",
                                       .resCost = 1200,
                                       .rgwtOreCost = {240, 160, 600},
                                       .ibmp = 140,
                                       .wtCargoMax = 65535,
                                       .dp = 1000,
                                       .rghs = {{.grhst = hstSomeSB, .cItem = 1},
                                                {.grhst = hstWeapon, .cItem = 16},
                                                {.grhst = hstSpecialE, .cItem = 3},
                                                {.grhst = hstWeapon, .cItem = 16},
                                                {.grhst = hstShield, .cItem = 20},
                                                {.grhst = hstShield, .cItem = 20},
                                                {.grhst = hstSpecialE, .cItem = 3},
                                                {.grhst = hstWeapon, .cItem = 16},
                                                {.grhst = hstSpecialE, .cItem = 3},
                                                {.grhst = hstWeapon, .cItem = 16},
                                                {.grhst = hstSomeSB, .cItem = 1},
                                                {.grhst = hstShArm, .cItem = 20},
                                                {.grhst = hstWeapon, .cItem = 16},
                                                {.grhst = hstShArm, .cItem = 20},
                                                {.grhst = hstSpecialE, .cItem = 3},
                                                {.grhst = hstWeapon, .cItem = 16}},
                                       .chs = 16,
                                },
                               .init = 16,
                               .imdAttack = 2,
                               .wrcCargo = 17510,
                               .rgbrc = {36, 80, 66, 88, 98, 38, 70, 3, 131, 56, 100, 102, 48, 34, 5, 133},
                        },
                           {
                               .hul =
                                {
                                       .ihuldef = ihuldefDeathStart,
                                       .rgTech = {0, 0, 0, 17},
                                       .szClass = "Death Star",
                                       .resCost = 1500,
                                       .rgwtOreCost = {240, 160, 700},
                                       .ibmp = 144,
                                       .wtCargoMax = 65535,
                                       .dp = 1500,
                                       .rghs = {{.grhst = hstSomeSB, .cItem = 1},
                                                {.grhst = hstWeapon, .cItem = 32},
                                                {.grhst = hstSpecialE, .cItem = 4},
                                                {.grhst = hstSpecialE, .cItem = 4},
                                                {.grhst = hstShield, .cItem = 30},
                                                {.grhst = hstShield, .cItem = 30},
                                                {.grhst = hstSpecialE, .cItem = 4},
                                                {.grhst = hstWeapon, .cItem = 32},
                                                {.grhst = hstSpecialE, .cItem = 4},
                                                {.grhst = hstWeapon, .cItem = 32},
                                                {.grhst = hstSomeSB, .cItem = 1},
                                                {.grhst = hstShArm, .cItem = 20},
                                                {.grhst = hstSpecialE, .cItem = 4},
                                                {.grhst = hstShArm, .cItem = 20},
                                                {.grhst = hstSpecialE, .cItem = 4},
                                                {.grhst = hstWeapon, .cItem = 32}},
                                       .chs = 16,
                                },
                               .init = 18,
                               .imdAttack = 2,
                               .wrcCargo = 17510,
                               .rgbrc = {20, 96, 65, 104, 98, 38, 71, 2, 130, 40, 116, 102, 32, 34, 6, 134},
                        }};
SHDEF     rgshdefT[22] = {{
                              .hul =
                              {
                                      .szClass = "Lilliputian Freighter",
                                      .wtCargoMax = 70,
                                      .wtFuelMax = 200,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1}, {.grhst = hstScanner, .cItem = 1}, {.grhst = hstShield, .cItem = 1}},
                                      .chs = 3,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .wFlags = 263,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .szClass = "Shadow Transport",
                                      .ibmp = 2,
                                      .wtCargoMax = 70,
                                      .wtFuelMax = 200,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1}, {.grhst = hstSpecialE, .cItem = 1}, {.grhst = hstShield, .cItem = 1}},
                                      .chs = 3,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 1,
                              .wFlags = 1287,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefScout,
                                      .szClass = "Smaugarian Peeping Tom",
                                      .ibmp = 16,
                                      .wtFuelMax = 50,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1},
                                               {.grhst = hstScanner, .cItem = 1},
                                               {.grhst = hstSpecialM, .iItem = 5, .cItem = 1}},
                                      .chs = 3,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 2,
                              .wFlags = 2311,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefScout,
                                      .szClass = "Armed Probe",
                                      .ibmp = 17,
                                      .wtFuelMax = 50,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1},
                                               {.grhst = hstScanner, .cItem = 1},
                                               {.grhst = hstBeam, .iItem = 1, .cItem = 1}},
                                      .chs = 3,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 3,
                              .wFlags = 3335,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefScout,
                                      .szClass = "Long Range Scout",
                                      .ibmp = 18,
                                      .wtFuelMax = 50,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1},
                                               {.grhst = hstScanner, .cItem = 1},
                                               {.grhst = hstSpecialM, .iItem = 5, .cItem = 1}},
                                      .chs = 3,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 4,
                              .wFlags = 4359,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefScout,
                                      .szClass = "Shadow Sleuth",
                                      .ibmp = 19,
                                      .wtFuelMax = 50,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1},
                                               {.grhst = hstScanner, .cItem = 1},
                                               {.grhst = hstSpecialE, .iItem = 1, .cItem = 1}},
                                      .chs = 3,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 5,
                              .wFlags = 5383,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefMediumFreighter,
                                      .szClass = "Teamster",
                                      .ibmp = 4,
                                      .wtFuelMax = 50,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1}, {.grhst = hstScanner, .cItem = 1}, {.grhst = hstArmor, .cItem = 1}},
                                      .chs = 3,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 6,
                              .wFlags = 6407,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefDestroyer,
                                      .szClass = "Stalwart Defender",
                                      .ibmp = 24,
                                      .wtCargoMax = 25,
                                      .wtFuelMax = 100,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1},
                                               {.grhst = hstBeam, .cItem = 1},
                                               {.grhst = hstTorp, .cItem = 1},
                                               {.grhst = hstScanner, .cItem = 1},
                                               {.grhst = hstArmor, .cItem = 2},
                                               {.grhst = hstSpecialM, .iItem = 5, .cItem = 1},
                                               {.grhst = hstSpecialE, .iItem = 5, .cItem = 1}},
                                      .chs = 7,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 7,
                              .wFlags = 7431,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefPrivateer,
                                      .szClass = "Swashbuckler",
                                      .ibmp = 44,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1},
                                               {.grhst = hstArmor, .iItem = 1, .cItem = 2},
                                               {.grhst = hstScanner, .cItem = 1},
                                               {.grhst = hstBeam, .cItem = 1},
                                               {.grhst = hstTorp, .cItem = 1}},
                                      .chs = 5,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 8,
                              .wFlags = 8455,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefColonyShip,
                                      .szClass = "Santa Maria",
                                      .ibmp = 60,
                                      .wtCargoMax = 25,
                                      .wtFuelMax = 100,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1}, {.grhst = hstSpecialM, .cItem = 1}},
                                      .chs = 2,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 9,
                              .wFlags = 9479,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefColonyShip,
                                      .szClass = "Pinta",
                                      .ibmp = 61,
                                      .wtCargoMax = 25,
                                      .wtFuelMax = 100,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1}, {.grhst = hstSpecialM, .iItem = 1, .cItem = 1}},
                                      .chs = 2,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 10,
                              .wFlags = 10503,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefColonyShip,
                                      .szClass = "Mayflower",
                                      .ibmp = 62,
                                      .wtCargoMax = 25,
                                      .wtFuelMax = 100,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1}, {.grhst = hstSpecialM, .cItem = 1}},
                                      .chs = 2,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 11,
                              .wFlags = 11527,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefMiniColonyShip,
                                      .szClass = "Spore Cloud",
                                      .ibmp = 56,
                                      .wtCargoMax = 10,
                                      .wtFuelMax = 30,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1}, {.grhst = hstSpecialM, .cItem = 1}},
                                      .chs = 2,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 12,
                              .wFlags = 12551,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefMiniBomber,
                                      .szClass = "Gadfly",
                                      .ibmp = 65,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1}, {.grhst = hstBomb, .cItem = 2}},
                                      .chs = 2,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 13,
                              .wFlags = 13575,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefMiniMiner,
                                      .szClass = "Cotton Picker",
                                      .ibmp = 85,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1},
                                               {.grhst = hstScanner, .cItem = 1},
                                               {.grhst = hstMining, .iItem = 1, .cItem = 1},
                                               {.grhst = hstMining, .iItem = 1, .cItem = 1}},
                                      .chs = 4,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 14,
                              .wFlags = 14599,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefMidgetMiner,
                                      .szClass = "Potato Bug",
                                      .ibmp = 82,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1}, {.grhst = hstMining, .cItem = 2}},
                                      .chs = 2,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 15,
                              .wFlags = 15623,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefMiniMineLayer,
                                      .szClass = "Little Hen",
                                      .ibmp = 108,
                                      .wtFuelMax = 100,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1},
                                               {.grhst = hstMines, .cItem = 2},
                                               {.grhst = hstMines, .cItem = 2},
                                               {.grhst = hstScanner, .cItem = 1}},
                                      .chs = 4,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 16,
                              .wFlags = 16647,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefMiniMiner,
                                      .szClass = "Change of Heart",
                                      .ibmp = 86,
                                      .wtFuelMax = 100,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1},
                                               {.grhst = hstScanner, .cItem = 1},
                                               {.grhst = hstMining, .iItem = 7, .cItem = 1},
                                               {.grhst = hstMining, .iItem = 7, .cItem = 1}},
                                      .chs = 4,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 17,
                              .wFlags = 17671,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefMiniMineLayer,
                                      .szClass = "Speed Turtle",
                                      .ibmp = 108,
                                      .wtFuelMax = 100,
                                      .rghs = {{.grhst = hstEngine, .iItem = 1, .cItem = 1},
                                               {.grhst = hstMines, .iItem = 7, .cItem = 2},
                                               {.grhst = hstMines, .iItem = 7, .cItem = 2},
                                               {.grhst = hstScanner, .cItem = 1}},
                                      .chs = 4,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 18,
                              .wFlags = 18695,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefNubian,
                                      .szClass = "M.T. Lifeboat",
                                      .ibmp = 108,
                                      .wtFuelMax = 100,
                                      .rghs = {{.grhst = hstEngine, .iItem = 8, .cItem = 3},
                                               {.grhst = hstArmor, .iItem = 9, .cItem = 3},
                                               {.grhst = hstArmor, .iItem = 9, .cItem = 3},
                                               {.grhst = hstTorp, .iItem = 7, .cItem = 3},
                                               {.grhst = hstTorp, .iItem = 7, .cItem = 3},
                                               {.grhst = hstShield, .iItem = 6, .cItem = 3},
                                               {.grhst = hstShield, .iItem = 6, .cItem = 3},
                                               {.grhst = hstSpecialE, .iItem = 4, .cItem = 3},
                                               {.grhst = hstSpecialE, .iItem = 4, .cItem = 3},
                                               {.grhst = hstSpecialM, .iItem = 4, .cItem = 3},
                                               {.grhst = hstBeam, .iItem = 18, .cItem = 3},
                                               {.grhst = hstBeam, .iItem = 18, .cItem = 3},
                                               {.grhst = hstBeam, .iItem = 18, .cItem = 3}},
                                      .chs = 13,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 19,
                              .wFlags = 19719,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefMiniMorph,
                                      .szClass = "M.T. Scout",
                                      .ibmp = 122,
                                      .wtFuelMax = 100,
                                      .rghs = {{.grhst = hstEngine, .iItem = 8, .cItem = 2},
                                               {.grhst = hstShield, .iItem = 6, .cItem = 3},
                                               {.grhst = hstSpecialE, .iItem = 4, .cItem = 1},
                                               {.grhst = hstSpecialM, .iItem = 4, .cItem = 1},
                                               {.grhst = hstSpecialM, .iItem = 9, .cItem = 1},
                                               {.grhst = hstTorp, .iItem = 7, .cItem = 2},
                                               {.grhst = hstTorp, .iItem = 7, .cItem = 2}},
                                      .chs = 7,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 20,
                              .wFlags = 20743,
                              .turn = 1,
                      },
                          {
                              .hul =
                              {
                                      .ihuldef = ihuldefMiniMorph,
                                      .szClass = "M.T. Probe",
                                      .ibmp = 123,
                                      .wtFuelMax = 100,
                                      .rghs = {{.grhst = hstEngine, .iItem = 8, .cItem = 2},
                                               {.grhst = hstArmor, .iItem = 9, .cItem = 3},
                                               {.grhst = hstSpecialE, .iItem = 4, .cItem = 1},
                                               {.grhst = hstSpecialM, .iItem = 4, .cItem = 1},
                                               {.grhst = hstSpecialM, .iItem = 9, .cItem = 1},
                                               {.grhst = hstTorp, .iItem = 7, .cItem = 2},
                                               {.grhst = hstTorp, .iItem = 7, .cItem = 2}},
                                      .chs = 7,
                              },
                              .det = detAll,
                              .fInclude = 1,
                              .ishdef = 21,
                              .wFlags = 21767,
                              .turn = 1,
                      }};
ENGINE    rgengine[16] = {{
                              .id = 1,
                              .szName = "Settler's Delight",
                              .cMass = 2,
                              .resCost = 2,
                              .rgwtOreCost = {1, 0, 1},
                              .ibmp = 8,
                              .grfAbilities = 1,
                              .rgcFuelUsed = {0, 0, 0, 0, 0, 0, 0, 140, 275, 480, 576},
                       },
                          {
                              .id = 2,
                              .szName = "Quick Jump 5",
                              .cMass = 4,
                              .resCost = 3,
                              .rgwtOreCost = {3, 0, 1},
                              .rgcFuelUsed = {0, 0, 25, 100, 100, 100, 180, 500, 800, 900, 1080},
                       },
                          {
                              .id = 3,
                              .rgTech = {0, 0, 2},
                              .szName = "Fuel Mizer",
                              .cMass = 6,
                              .resCost = 11,
                              .rgwtOreCost = {8},
                              .ibmp = 9,
                              .grfAbilities = 3,
                              .rgcFuelUsed = {0, 0, 0, 0, 0, 35, 120, 175, 235, 360, 420},
                       },
                          {
                              .id = 4,
                              .rgTech = {0, 0, 3},
                              .szName = "Long Hump 6",
                              .cMass = 9,
                              .resCost = 6,
                              .rgwtOreCost = {5, 0, 1},
                              .ibmp = 1,
                              .rgcFuelUsed = {0, 0, 20, 60, 100, 100, 105, 450, 750, 900, 1080},
                       },
                          {
                              .id = 5,
                              .rgTech = {0, 0, 5},
                              .szName = "Daddy Long Legs 7",
                              .cMass = 13,
                              .resCost = 12,
                              .rgwtOreCost = {11, 0, 3},
                              .ibmp = 2,
                              .rgcFuelUsed = {0, 0, 20, 60, 70, 100, 100, 110, 600, 750, 900},
                       },
                          {
                              .id = 6,
                              .rgTech = {0, 0, 7},
                              .szName = "Alpha Drive 8",
                              .cMass = 17,
                              .resCost = 28,
                              .rgwtOreCost = {16, 0, 3},
                              .ibmp = 3,
                              .rgcFuelUsed = {0, 0, 15, 50, 60, 70, 100, 100, 115, 700, 840},
                       },
                          {
                              .id = 7,
                              .rgTech = {0, 0, 9},
                              .szName = "Trans-Galactic Drive",
                              .cMass = 25,
                              .resCost = 50,
                              .rgwtOreCost = {20, 20, 9},
                              .ibmp = 4,
                              .rgcFuelUsed = {0, 0, 15, 35, 45, 55, 70, 80, 90, 100, 120},
                       },
                          {
                              .id = 8,
                              .rgTech = {0, 0, 11},
                              .szName = "Interspace-10",
                              .cMass = 25,
                              .resCost = 60,
                              .rgwtOreCost = {18, 25, 10},
                              .ibmp = 12,
                              .grfAbilities = 5,
                              .rgcFuelUsed = {0, 0, 10, 30, 40, 50, 60, 70, 80, 90, 100},
                       },
                          {
                              .id = 9,
                              .rgTech = {7, 0, 13, 5, 9},
                              .szName = "Enigma Pulsar",
                              .cMass = 20,
                              .resCost = 40,
                              .rgwtOreCost = {12, 15, 11},
                              .ibmp = 109,
                              .grfAbilities = 6,
                              .rgcFuelUsed = {0, 0, 0, 0, 0, 0, 65, 75, 85, 95, 105},
                       },
                          {
                              .id = 10,
                              .rgTech = {0, 0, 23},
                              .szName = "Trans-Star 10",
                              .cMass = 5,
                              .resCost = 10,
                              .rgwtOreCost = {3, 0, 3},
                              .ibmp = 117,
                              .rgcFuelUsed = {0, 0, 5, 15, 20, 25, 30, 35, 40, 45, 50},
                       },
                          {
                              .id = 11,
                              .rgTech = {2, 0, 6},
                              .szName = "Radiating Hydro-Ram Scoop",
                              .cMass = 10,
                              .resCost = 8,
                              .rgwtOreCost = {3, 2, 9},
                              .ibmp = 7,
                              .grfAbilities = 2,
                              .rgcFuelUsed = {0, 0, 0, 0, 0, 0, 0, 165, 375, 600, 720},
                       },
                          {
                              .id = 12,
                              .rgTech = {2, 0, 8},
                              .szName = "Sub-Galactic Fuel Scoop",
                              .cMass = 20,
                              .resCost = 12,
                              .rgwtOreCost = {4, 4, 7},
                              .ibmp = 5,
                              .rgcFuelUsed = {0, 0, 0, 0, 0, 0, 85, 105, 210, 380, 456},
                       },
                          {
                              .id = 13,
                              .rgTech = {3, 0, 9},
                              .szName = "Trans-Galactic Fuel Scoop",
                              .cMass = 19,
                              .resCost = 18,
                              .rgwtOreCost = {5, 4, 12},
                              .ibmp = 6,
                              .rgcFuelUsed = {0, 0, 0, 0, 0, 0, 0, 88, 100, 145, 174},
                       },
                          {
                              .id = 14,
                              .rgTech = {4, 0, 12},
                              .szName = "Trans-Galactic Super Scoop",
                              .cMass = 18,
                              .resCost = 24,
                              .rgwtOreCost = {6, 4, 16},
                              .ibmp = 10,
                              .rgcFuelUsed = {0, 0, 0, 0, 0, 0, 0, 0, 65, 90, 108},
                       },
                          {
                              .id = 15,
                              .rgTech = {4, 0, 16},
                              .szName = "Trans-Galactic Mizer Scoop",
                              .cMass = 11,
                              .resCost = 20,
                              .rgwtOreCost = {5, 2, 13},
                              .ibmp = 11,
                              .rgcFuelUsed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 70, 84},
                       },
                          {
                              .id = 16,
                              .rgTech = {5, 0, 20},
                              .szName = "Galaxy Scoop",
                              .cMass = 8,
                              .resCost = 12,
                              .rgwtOreCost = {4, 2, 9},
                              .ibmp = 191,
                              .grfAbilities = 4,
                              .rgcFuelUsed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60},
                       }};
HULDEF    rghuldef[32] = {{
                              .hul =
                               {
                                      .szClass = "Small Freighter",
                                      .wtEmpty = 25,
                                      .resCost = 20,
                                      .rgwtOreCost = {12, 0, 17},
                                      .wtCargoMax = 70,
                                      .wtFuelMax = 130,
                                      .dp = 25,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1}, {.grhst = hstScanSpec, .cItem = 1}, {.grhst = hstShArm, .cItem = 1}},
                                      .chs = 3,
                               },
                              .imdCategory = 1,
                              .wrcCargo = 13141,
                              .rgbrc = {49, 55, 53},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefMediumFreighter,
                                      .rgTech = {0, 0, 0, 3},
                                      .szClass = "Medium Freighter",
                                      .wtEmpty = 60,
                                      .resCost = 40,
                                      .rgwtOreCost = {20, 0, 19},
                                      .ibmp = 4,
                                      .wtCargoMax = 210,
                                      .wtFuelMax = 450,
                                      .dp = 50,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1}, {.grhst = hstScanSpec, .cItem = 1}, {.grhst = hstShArm, .cItem = 1}},
                                      .chs = 3,
                               },
                              .imdCategory = 1,
                              .wrcCargo = 12886,
                              .rgbrc = {48, 56, 54},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefLargeFreighter,
                                      .rgTech = {0, 0, 0, 8},
                                      .szClass = "Large Freighter",
                                      .wtEmpty = 125,
                                      .resCost = 100,
                                      .rgwtOreCost = {35, 0, 21},
                                      .ibmp = 8,
                                      .wtCargoMax = 1200,
                                      .wtFuelMax = 2600,
                                      .dp = 150,
                                      .rghs = {{.grhst = hstEngine, .cItem = 2}, {.grhst = hstScanSpec, .cItem = 2}, {.grhst = hstShArm, .cItem = 2}},
                                      .chs = 3,
                               },
                              .imdCategory = 1,
                              .wrcCargo = 8806,
                              .rgbrc = {48, 38, 70},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefSuperFreighter,
                                      .rgTech = {0, 0, 0, 13},
                                      .szClass = "Super Freighter",
                                      .wtEmpty = 175,
                                      .resCost = 125,
                                      .rgwtOreCost = {45, 0, 21},
                                      .ibmp = 12,
                                      .wtCargoMax = 3000,
                                      .wtFuelMax = 8000,
                                      .dp = 400,
                                      .rghs = {{.grhst = hstEngine, .cItem = 3},
                                               {.grhst = hstScanSpec, .cItem = 3},
                                               {.grhst = hstShArm, .cItem = 5},
                                               {.grhst = hstSpecialE, .cItem = 2}},
                                      .chs = 4,
                               },
                              .imdCategory = 1,
                              .wrcCargo = 8840,
                              .rgbrc = {64, 40, 72, 104},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefScout,
                                      .szClass = "Scout",
                                      .wtEmpty = 8,
                                      .resCost = 10,
                                      .rgwtOreCost = {4, 2, 4},
                                      .ibmp = 16,
                                      .wtFuelMax = 50,
                                      .dp = 20,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1}, {.grhst = hstScanner, .cItem = 1}, {.grhst = hstSome, .cItem = 1}},
                                      .chs = 3,
                               },
                              .init = 1,
                              .imdAttack = 1,
                              .imdCategory = 2,
                              .wrcCargo = 65535,
                              .rgbrc = {50, 54, 52},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefFrigate,
                                      .rgTech = {0, 0, 0, 6},
                                      .szClass = "Frigate",
                                      .wtEmpty = 8,
                                      .resCost = 12,
                                      .rgwtOreCost = {4, 2, 4},
                                      .ibmp = 20,
                                      .wtFuelMax = 125,
                                      .dp = 45,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1},
                                               {.grhst = hstScanner, .cItem = 2},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstShArm, .cItem = 2}},
                                      .chs = 4,
                               },
                              .init = 4,
                              .imdAttack = 1,
                              .imdCategory = 2,
                              .wrcCargo = 65535,
                              .rgbrc = {49, 55, 53, 51},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefDestroyer,
                                      .rgTech = {0, 0, 0, 3},
                                      .szClass = "Destroyer",
                                      .wtEmpty = 30,
                                      .resCost = 35,
                                      .rgwtOreCost = {15, 3, 5},
                                      .ibmp = 24,
                                      .wtFuelMax = 280,
                                      .dp = 200,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1},
                                               {.grhst = hstWeapon, .cItem = 1},
                                               {.grhst = hstWeapon, .cItem = 1},
                                               {.grhst = hstSome, .cItem = 1},
                                               {.grhst = hstArmor, .cItem = 2},
                                               {.grhst = hstSpecialM, .cItem = 1},
                                               {.grhst = hstSpecialE, .cItem = 1}},
                                      .chs = 7,
                               },
                              .init = 3,
                              .imdAttack = 1,
                              .imdCategory = 2,
                              .wrcCargo = 65535,
                              .rgbrc = {66, 21, 117, 70, 68, 35, 99},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefCruiser,
                                      .rgTech = {0, 0, 0, 9},
                                      .szClass = "Cruiser",
                                      .wtEmpty = 90,
                                      .resCost = 85,
                                      .rgwtOreCost = {40, 5, 8},
                                      .ibmp = 28,
                                      .wtFuelMax = 600,
                                      .dp = 700,
                                      .rghs = {{.grhst = hstEngine, .cItem = 2},
                                               {.grhst = hstShSpec, .cItem = 1},
                                               {.grhst = hstShSpec, .cItem = 1},
                                               {.grhst = hstWeapon, .cItem = 2},
                                               {.grhst = hstWeapon, .cItem = 2},
                                               {.grhst = hstSome, .cItem = 2},
                                               {.grhst = hstShArm, .cItem = 2}},
                                      .chs = 7,
                               },
                              .init = 5,
                              .imdAttack = 2,
                              .imdCategory = 3,
                              .wrcCargo = 65535,
                              .rgbrc = {49, 35, 67, 21, 85, 55, 53},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefBattleCruiser,
                                      .rgTech = {0, 0, 0, 10},
                                      .szClass = "Battle Cruiser",
                                      .wtEmpty = 120,
                                      .resCost = 120,
                                      .rgwtOreCost = {55, 8, 12},
                                      .ibmp = 32,
                                      .wtFuelMax = 1400,
                                      .dp = 1000,
                                      .rghs = {{.grhst = hstEngine, .cItem = 2},
                                               {.grhst = hstShSpec, .cItem = 2},
                                               {.grhst = hstShSpec, .cItem = 2},
                                               {.grhst = hstWeapon, .cItem = 3},
                                               {.grhst = hstWeapon, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstShArm, .cItem = 4}},
                                      .chs = 7,
                               },
                              .init = 5,
                              .imdAttack = 2,
                              .imdCategory = 3,
                              .wrcCargo = 65535,
                              .rgbrc = {49, 35, 67, 21, 85, 55, 53},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefBattleship,
                                      .rgTech = {0, 0, 0, 13},
                                      .szClass = "Battleship",
                                      .wtEmpty = 222,
                                      .resCost = 225,
                                      .rgwtOreCost = {120, 25, 20},
                                      .ibmp = 36,
                                      .wtFuelMax = 2800,
                                      .dp = 2000,
                                      .rghs = {{.grhst = hstEngine, .cItem = 4},
                                               {.grhst = hstScanSpec, .cItem = 1},
                                               {.grhst = hstShield, .cItem = 8},
                                               {.grhst = hstWeapon, .cItem = 6},
                                               {.grhst = hstWeapon, .cItem = 6},
                                               {.grhst = hstWeapon, .cItem = 2},
                                               {.grhst = hstWeapon, .cItem = 2},
                                               {.grhst = hstWeapon, .cItem = 4},
                                               {.grhst = hstArmor, .cItem = 6},
                                               {.grhst = hstSpecialE, .cItem = 3},
                                               {.grhst = hstSpecialE, .cItem = 3}},
                                      .chs = 11,
                               },
                              .init = 10,
                              .imdAttack = 2,
                              .imdCategory = 3,
                              .wrcCargo = 65535,
                              .rgbrc = {48, 56, 38, 20, 84, 2, 98, 70, 52, 34, 66},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefDreadnought,
                                      .rgTech = {0, 0, 0, 16},
                                      .szClass = "Dreadnought",
                                      .wtEmpty = 250,
                                      .resCost = 275,
                                      .rgwtOreCost = {140, 30, 25},
                                      .ibmp = 40,
                                      .wtFuelMax = 4500,
                                      .dp = 4500,
                                      .rghs = {{.grhst = hstEngine, .cItem = 5},
                                               {.grhst = hstShArm, .cItem = 4},
                                               {.grhst = hstShArm, .cItem = 4},
                                               {.grhst = hstWeapon, .cItem = 6},
                                               {.grhst = hstWeapon, .cItem = 6},
                                               {.grhst = hstSpecialE, .cItem = 4},
                                               {.grhst = hstSpecialE, .cItem = 4},
                                               {.grhst = hstWeapon, .cItem = 8},
                                               {.grhst = hstWeapon, .cItem = 8},
                                               {.grhst = hstArmor, .cItem = 8},
                                               {.grhst = hstShWeap, .cItem = 5},
                                               {.grhst = hstShWeap, .cItem = 5},
                                               {.grhst = hstSome, .cItem = 2}},
                                      .chs = 13,
                               },
                              .init = 10,
                              .imdAttack = 2,
                              .imdCategory = 3,
                              .wrcCargo = 65535,
                              .rgbrc = {64, 32, 96, 18, 114, 50, 82, 36, 100, 68, 54, 86, 72},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefPrivateer,
                                      .rgTech = {0, 0, 0, 4},
                                      .szClass = "Privateer",
                                      .wtEmpty = 65,
                                      .resCost = 50,
                                      .rgwtOreCost = {50, 3, 2},
                                      .ibmp = 44,
                                      .wtCargoMax = 250,
                                      .wtFuelMax = 650,
                                      .dp = 150,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1},
                                               {.grhst = hstShArm, .cItem = 2},
                                               {.grhst = hstScanSpec, .cItem = 1},
                                               {.grhst = hstSome, .cItem = 1},
                                               {.grhst = hstSome, .cItem = 1}},
                                      .chs = 5,
                               },
                              .init = 3,
                              .imdAttack = 1,
                              .imdCategory = 4,
                              .wrcCargo = 17255,
                              .rgbrc = {65, 55, 87, 37, 101},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefRogue,
                                      .rgTech = {0, 0, 0, 8},
                                      .szClass = "Rogue",
                                      .wtEmpty = 75,
                                      .resCost = 60,
                                      .rgwtOreCost = {80, 5, 5},
                                      .ibmp = 48,
                                      .wtCargoMax = 500,
                                      .wtFuelMax = 2250,
                                      .dp = 450,
                                      .rghs = {{.grhst = hstEngine, .cItem = 2},
                                               {.grhst = hstShArm, .cItem = 3},
                                               {.grhst = hstSpecMine, .cItem = 2},
                                               {.grhst = hstScanner, .cItem = 1},
                                               {.grhst = hstSome, .cItem = 2},
                                               {.grhst = hstSome, .cItem = 2},
                                               {.grhst = hstSpecMine, .cItem = 2},
                                               {.grhst = hstSpecialE, .cItem = 1},
                                               {.grhst = hstSpecialE, .cItem = 1}},
                                      .chs = 9,
                               },
                              .init = 4,
                              .imdAttack = 2,
                              .imdCategory = 4,
                              .wrcCargo = 13174,
                              .rgbrc = {65, 70, 102, 72, 20, 116, 38, 18, 114},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefGalleon,
                                      .rgTech = {0, 0, 0, 11},
                                      .szClass = "Galleon",
                                      .wtEmpty = 125,
                                      .resCost = 105,
                                      .rgwtOreCost = {70, 5, 5},
                                      .ibmp = 52,
                                      .wtCargoMax = 1000,
                                      .wtFuelMax = 2500,
                                      .dp = 900,
                                      .rghs = {{.grhst = hstEngine, .cItem = 4},
                                               {.grhst = hstShArm, .cItem = 2},
                                               {.grhst = hstShArm, .cItem = 2},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSpecMine, .cItem = 2},
                                               {.grhst = hstSpecialEM, .cItem = 2},
                                               {.grhst = hstScanner, .cItem = 2}},
                                      .chs = 8,
                               },
                              .init = 4,
                              .imdAttack = 2,
                              .imdCategory = 4,
                              .wrcCargo = 12918,
                              .rgbrc = {64, 19, 115, 21, 117, 54, 86, 72},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefMiniColonyShip,
                                      .szClass = "Mini-Colony Ship",
                                      .wtEmpty = 8,
                                      .resCost = 3,
                                      .rgwtOreCost = {2, 0, 2},
                                      .ibmp = 56,
                                      .wtCargoMax = 10,
                                      .wtFuelMax = 150,
                                      .dp = 10,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1}, {.grhst = hstSpecialM, .cItem = 1}},
                                      .chs = 2,
                               },
                              .wrcCargo = 13398,
                              .rgbrc = {50, 54},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefColonyShip,
                                      .szClass = "Colony Ship",
                                      .wtEmpty = 20,
                                      .resCost = 20,
                                      .rgwtOreCost = {10, 0, 15},
                                      .ibmp = 60,
                                      .wtCargoMax = 25,
                                      .wtFuelMax = 200,
                                      .dp = 20,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1}, {.grhst = hstSpecialM, .cItem = 1}},
                                      .chs = 2,
                               },
                              .wrcCargo = 13398,
                              .rgbrc = {50, 54},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefMiniBomber,
                                      .rgTech = {0, 0, 0, 1},
                                      .szClass = "Mini Bomber",
                                      .wtEmpty = 28,
                                      .resCost = 35,
                                      .rgwtOreCost = {20, 5, 10},
                                      .ibmp = 64,
                                      .wtFuelMax = 120,
                                      .dp = 50,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1}, {.grhst = hstBomb, .cItem = 2}},
                                      .chs = 2,
                               },
                              .imdAttack = 3,
                              .imdCategory = 5,
                              .wrcCargo = 65535,
                              .rgbrc = {51, 53},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefB17Bomber,
                                      .rgTech = {0, 0, 0, 6},
                                      .szClass = "B-17 Bomber",
                                      .wtEmpty = 69,
                                      .resCost = 150,
                                      .rgwtOreCost = {55, 10, 10},
                                      .ibmp = 68,
                                      .wtFuelMax = 400,
                                      .dp = 175,
                                      .rghs = {{.grhst = hstEngine, .cItem = 2},
                                               {.grhst = hstBomb, .cItem = 4},
                                               {.grhst = hstBomb, .cItem = 4},
                                               {.grhst = hstScanSpec, .cItem = 1}},
                                      .chs = 4,
                               },
                              .imdAttack = 3,
                              .imdCategory = 5,
                              .wrcCargo = 65535,
                              .rgbrc = {49, 51, 53, 55},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefStealthBomber,
                                      .rgTech = {0, 0, 0, 8},
                                      .szClass = "Stealth Bomber",
                                      .wtEmpty = 70,
                                      .resCost = 175,
                                      .rgwtOreCost = {55, 10, 15},
                                      .ibmp = 72,
                                      .wtFuelMax = 750,
                                      .dp = 225,
                                      .rghs = {{.grhst = hstEngine, .cItem = 2},
                                               {.grhst = hstBomb, .cItem = 4},
                                               {.grhst = hstBomb, .cItem = 4},
                                               {.grhst = hstScanSpec, .cItem = 1},
                                               {.grhst = hstSpecialE, .cItem = 3}},
                                      .chs = 5,
                               },
                              .imdAttack = 3,
                              .imdCategory = 5,
                              .wrcCargo = 65535,
                              .rgbrc = {50, 36, 68, 38, 70},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefB52Bomber,
                                      .rgTech = {0, 0, 0, 15},
                                      .szClass = "B-52 Bomber",
                                      .wtEmpty = 110,
                                      .resCost = 280,
                                      .rgwtOreCost = {90, 15, 10},
                                      .ibmp = 76,
                                      .wtFuelMax = 750,
                                      .dp = 450,
                                      .rghs = {{.grhst = hstEngine, .cItem = 3},
                                               {.grhst = hstBomb, .cItem = 4},
                                               {.grhst = hstBomb, .cItem = 4},
                                               {.grhst = hstBomb, .cItem = 4},
                                               {.grhst = hstBomb, .cItem = 4},
                                               {.grhst = hstScanSpec, .cItem = 2},
                                               {.grhst = hstShield, .cItem = 2}},
                                      .chs = 7,
                               },
                              .imdAttack = 3,
                              .imdCategory = 5,
                              .wrcCargo = 65535,
                              .rgbrc = {49, 19, 83, 37, 69, 55, 51},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefMidgetMiner,
                                      .szClass = "Midget Miner",
                                      .wtEmpty = 10,
                                      .resCost = 20,
                                      .rgwtOreCost = {10, 0, 3},
                                      .ibmp = 80,
                                      .wtFuelMax = 210,
                                      .dp = 100,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1}, {.grhst = hstMining, .cItem = 2}},
                                      .chs = 2,
                               },
                              .imdCategory = 6,
                              .wrcCargo = 65535,
                              .rgbrc = {51, 53},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefMiniMiner,
                                      .rgTech = {0, 0, 0, 2},
                                      .szClass = "Mini-Miner",
                                      .wtEmpty = 80,
                                      .resCost = 50,
                                      .rgwtOreCost = {25, 0, 6},
                                      .ibmp = 84,
                                      .wtFuelMax = 210,
                                      .dp = 130,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1},
                                               {.grhst = hstScanSpec, .cItem = 1},
                                               {.grhst = hstMining, .cItem = 1},
                                               {.grhst = hstMining, .cItem = 1}},
                                      .chs = 4,
                               },
                              .imdCategory = 6,
                              .wrcCargo = 65535,
                              .rgbrc = {50, 54, 36, 68},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefMiner,
                                      .rgTech = {0, 0, 0, 6},
                                      .szClass = "Miner",
                                      .wtEmpty = 110,
                                      .resCost = 110,
                                      .rgwtOreCost = {32, 0, 6},
                                      .ibmp = 88,
                                      .wtFuelMax = 500,
                                      .dp = 475,
                                      .rghs = {{.grhst = hstEngine, .cItem = 2},
                                               {.grhst = hstScanSpecArm, .cItem = 2},
                                               {.grhst = hstMining, .cItem = 2},
                                               {.grhst = hstMining, .cItem = 1},
                                               {.grhst = hstMining, .cItem = 2},
                                               {.grhst = hstMining, .cItem = 1}},
                                      .chs = 6,
                               },
                              .imdCategory = 6,
                              .wrcCargo = 65535,
                              .rgbrc = {49, 55, 35, 37, 67, 69},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefMaxiMiner,
                                      .rgTech = {0, 0, 0, 11},
                                      .szClass = "Maxi-Miner",
                                      .wtEmpty = 110,
                                      .resCost = 140,
                                      .rgwtOreCost = {32, 0, 6},
                                      .ibmp = 92,
                                      .wtFuelMax = 850,
                                      .dp = 1400,
                                      .rghs = {{.grhst = hstEngine, .cItem = 3},
                                               {.grhst = hstScanSpecArm, .cItem = 2},
                                               {.grhst = hstMining, .cItem = 4},
                                               {.grhst = hstMining, .cItem = 1},
                                               {.grhst = hstMining, .cItem = 4},
                                               {.grhst = hstMining, .cItem = 1}},
                                      .chs = 6,
                               },
                              .imdCategory = 6,
                              .wrcCargo = 65535,
                              .rgbrc = {49, 55, 35, 37, 67, 69},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefUltraMiner,
                                      .rgTech = {0, 0, 0, 14},
                                      .szClass = "Ultra-Miner",
                                      .wtEmpty = 100,
                                      .resCost = 130,
                                      .rgwtOreCost = {30, 0, 6},
                                      .ibmp = 96,
                                      .wtFuelMax = 1300,
                                      .dp = 1500,
                                      .rghs = {{.grhst = hstEngine, .cItem = 2},
                                               {.grhst = hstScanSpecArm, .cItem = 3},
                                               {.grhst = hstMining, .cItem = 4},
                                               {.grhst = hstMining, .cItem = 2},
                                               {.grhst = hstMining, .cItem = 4},
                                               {.grhst = hstMining, .cItem = 2}},
                                      .chs = 6,
                               },
                              .imdCategory = 6,
                              .wrcCargo = 65535,
                              .rgbrc = {49, 55, 35, 37, 67, 69},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefFuelTransport,
                                      .rgTech = {0, 0, 0, 4},
                                      .szClass = "Fuel Transport",
                                      .wtEmpty = 12,
                                      .resCost = 50,
                                      .rgwtOreCost = {10, 0, 5},
                                      .ibmp = 100,
                                      .wtFuelMax = 750,
                                      .dp = 5,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1}, {.grhst = hstShield, .cItem = 1}},
                                      .chs = 2,
                               },
                              .imdCategory = 7,
                              .wrcCargo = 65535,
                              .rgbrc = {51, 53},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefSuperFuelXport,
                                      .rgTech = {0, 0, 0, 7},
                                      .szClass = "Super-Fuel Xport",
                                      .wtEmpty = 111,
                                      .resCost = 70,
                                      .rgwtOreCost = {20, 0, 8},
                                      .ibmp = 104,
                                      .wtFuelMax = 2250,
                                      .dp = 12,
                                      .rghs = {{.grhst = hstEngine, .cItem = 2}, {.grhst = hstShield, .cItem = 2}, {.grhst = hstScanner, .cItem = 1}},
                                      .chs = 3,
                               },
                              .imdCategory = 7,
                              .wrcCargo = 65535,
                              .rgbrc = {50, 52, 54},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefMiniMineLayer,
                                      .szClass = "Mini Mine Layer",
                                      .wtEmpty = 10,
                                      .resCost = 20,
                                      .rgwtOreCost = {8, 2, 5},
                                      .ibmp = 108,
                                      .wtFuelMax = 400,
                                      .dp = 60,
                                      .rghs = {{.grhst = hstEngine, .cItem = 1},
                                               {.grhst = hstMines, .cItem = 2},
                                               {.grhst = hstMines, .cItem = 2},
                                               {.grhst = hstScanSpec, .cItem = 1}},
                                      .chs = 4,
                               },
                              .imdCategory = 4,
                              .wrcCargo = 65535,
                              .rgbrc = {50, 36, 68, 54},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefSuperMineLayer,
                                      .rgTech = {0, 0, 0, 15},
                                      .szClass = "Super Mine Layer",
                                      .wtEmpty = 30,
                                      .resCost = 30,
                                      .rgwtOreCost = {20, 3, 9},
                                      .ibmp = 112,
                                      .wtFuelMax = 2200,
                                      .dp = 1200,
                                      .rghs = {{.grhst = hstEngine, .cItem = 3},
                                               {.grhst = hstMines, .cItem = 8},
                                               {.grhst = hstMines, .cItem = 8},
                                               {.grhst = hstShArm, .cItem = 3},
                                               {.grhst = hstScanSpec, .cItem = 3},
                                               {.grhst = hstSpecMine, .cItem = 3}},
                                      .chs = 6,
                               },
                              .imdCategory = 4,
                              .wrcCargo = 65535,
                              .rgbrc = {49, 35, 67, 53, 39, 71},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefNubian,
                                      .rgTech = {0, 0, 0, 26},
                                      .szClass = "Nubian",
                                      .wtEmpty = 100,
                                      .resCost = 150,
                                      .rgwtOreCost = {75, 12, 12},
                                      .ibmp = 124,
                                      .wtFuelMax = 5000,
                                      .dp = 5000,
                                      .rghs = {{.grhst = hstEngine, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 3}},
                                      .chs = 13,
                               },
                              .init = 2,
                              .imdAttack = 2,
                              .imdCategory = 4,
                              .wrcCargo = 65535,
                              .rgbrc = {64, 32, 96, 18, 114, 50, 82, 36, 100, 68, 54, 86, 72},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefMiniMorph,
                                      .rgTech = {0, 0, 0, 8},
                                      .szClass = "Mini Morph",
                                      .wtEmpty = 70,
                                      .resCost = 100,
                                      .rgwtOreCost = {30, 8, 8},
                                      .ibmp = 120,
                                      .wtCargoMax = 150,
                                      .wtFuelMax = 400,
                                      .dp = 250,
                                      .rghs = {{.grhst = hstEngine, .cItem = 2},
                                               {.grhst = hstSome, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 1},
                                               {.grhst = hstSome, .cItem = 1},
                                               {.grhst = hstSome, .cItem = 1},
                                               {.grhst = hstSome, .cItem = 2},
                                               {.grhst = hstSome, .cItem = 2}},
                                      .chs = 7,
                               },
                              .init = 2,
                              .imdAttack = 2,
                              .imdCategory = 4,
                              .wrcCargo = 9318,
                              .rgbrc = {48, 50, 38, 70, 56, 18, 82},
                       },
                          {
                              .hul =
                               {
                                      .ihuldef = ihuldefMetaMorph,
                                      .rgTech = {0, 0, 0, 10},
                                      .szClass = "Meta Morph",
                                      .wtEmpty = 85,
                                      .resCost = 120,
                                      .rgwtOreCost = {50, 12, 12},
                                      .ibmp = 116,
                                      .wtCargoMax = 300,
                                      .wtFuelMax = 700,
                                      .dp = 500,
                                      .rghs = {{.grhst = hstEngine, .cItem = 3},
                                               {.grhst = hstSome, .cItem = 8},
                                               {.grhst = hstSome, .cItem = 2},
                                               {.grhst = hstSome, .cItem = 2},
                                               {.grhst = hstSome, .cItem = 1},
                                               {.grhst = hstSome, .cItem = 2},
                                               {.grhst = hstSome, .cItem = 2}},
                                      .chs = 7,
                               },
                              .init = 2,
                              .imdAttack = 2,
                              .imdCategory = 4,
                              .wrcCargo = 9318,
                              .rgbrc = {48, 50, 38, 70, 56, 18, 82},
                       }};
MINING    rgmining[8] = {{
                             .id = 1,
                             .szName = "Robo-Midget Miner",
                             .cMass = 80,
                             .resCost = 50,
                             .rgwtOreCost = {14, 0, 4},
                             .ibmp = 138,
                             .grAbility = 5,
                      },
                         {
                             .id = 2,
                             .rgTech = {0, 0, 0, 2, 1},
                             .szName = "Robo-Mini-Miner",
                             .cMass = 240,
                             .resCost = 100,
                             .rgwtOreCost = {30, 0, 7},
                             .ibmp = 139,
                             .grAbility = 4,
                      },
                         {
                             .id = 3,
                             .rgTech = {0, 0, 0, 4, 2},
                             .szName = "Robo-Miner",
                             .cMass = 240,
                             .resCost = 100,
                             .rgwtOreCost = {30, 0, 7},
                             .ibmp = 140,
                             .grAbility = 12,
                      },
                         {
                             .id = 4,
                             .rgTech = {0, 0, 0, 7, 4},
                             .szName = "Robo-Maxi-Miner",
                             .cMass = 240,
                             .resCost = 100,
                             .rgwtOreCost = {30, 0, 7},
                             .ibmp = 141,
                             .grAbility = 18,
                      },
                         {
                             .id = 5,
                             .rgTech = {0, 0, 0, 12, 6},
                             .szName = "Robo-Super-Miner",
                             .cMass = 240,
                             .resCost = 100,
                             .rgwtOreCost = {30, 0, 7},
                             .ibmp = 142,
                             .grAbility = 27,
                      },
                         {
                             .id = 6,
                             .rgTech = {0, 0, 0, 15, 8},
                             .szName = "Robo-Ultra-Miner",
                             .cMass = 80,
                             .resCost = 50,
                             .rgwtOreCost = {14, 0, 4},
                             .ibmp = 143,
                             .grAbility = 25,
                      },
                         {
                             .id = 7,
                             .rgTech = {5, 0, 0, 10, 5, 5},
                             .szName = "Alien Miner",
                             .cMass = 20,
                             .resCost = 20,
                             .rgwtOreCost = {8, 0, 2},
                             .ibmp = 181,
                             .grAbility = 10,
                      },
                         {
                             .id = 8,
                             .rgTech = {0, 0, 0, 0, 0, 6},
                             .szName = "Orbital Adjuster",
                             .cMass = 80,
                             .resCost = 50,
                             .rgwtOreCost = {25, 25, 25},
                             .ibmp = 173,
                      }};
SHIELD    rgshield[10] = {{
                              .id = 1,
                              .szName = "Mole-skin Shield",
                              .cMass = 1,
                              .resCost = 4,
                              .rgwtOreCost = {1, 0, 1},
                              .ibmp = 42,
                              .dp = 25,
                       },
                          {
                              .id = 2,
                              .rgTech = {3},
                              .szName = "Cow-hide Shield",
                              .cMass = 1,
                              .resCost = 5,
                              .rgwtOreCost = {2, 0, 2},
                              .ibmp = 43,
                              .dp = 40,
                       },
                          {
                              .id = 3,
                              .rgTech = {6},
                              .szName = "Wolverine Diffuse Shield",
                              .cMass = 1,
                              .resCost = 6,
                              .rgwtOreCost = {3, 0, 3},
                              .ibmp = 44,
                              .dp = 60,
                       },
                          {
                              .id = 4,
                              .rgTech = {7, 0, 0, 4},
                              .szName = "Croby Sharmor",
                              .cMass = 10,
                              .resCost = 15,
                              .rgwtOreCost = {7, 0, 4},
                              .ibmp = 40,
                              .dp = 60,
                       },
                          {
                              .id = 5,
                              .rgTech = {7, 0, 0, 0, 3},
                              .szName = "Shadow Shield",
                              .cMass = 2,
                              .resCost = 7,
                              .rgwtOreCost = {3, 0, 3},
                              .ibmp = 41,
                              .dp = 75,
                       },
                          {
                              .id = 6,
                              .rgTech = "\n",
                              .szName = "Bear Neutrino Barrier",
                              .cMass = 1,
                              .resCost = 8,
                              .rgwtOreCost = {4, 0, 4},
                              .ibmp = 45,
                              .dp = 100,
                       },
                          {
                              .id = 7,
                              .rgTech = {12, 0, 9, 0, 9},
                              .szName = "Langston Shell",
                              .cMass = 10,
                              .resCost = 20,
                              .rgwtOreCost = {10, 2, 6},
                              .ibmp = 183,
                              .dp = 125,
                       },
                          {
                              .id = 8,
                              .rgTech = {14},
                              .szName = "Gorilla Delagator",
                              .cMass = 1,
                              .resCost = 11,
                              .rgwtOreCost = {5, 0, 6},
                              .ibmp = 46,
                              .dp = 175,
                       },
                          {
                              .id = 9,
                              .rgTech = {18},
                              .szName = "Elephant Hide Fortress",
                              .cMass = 1,
                              .resCost = 15,
                              .rgwtOreCost = {8, 0, 10},
                              .ibmp = 47,
                              .dp = 300,
                       },
                          {
                              .id = 10,
                              .rgTech = {22},
                              .szName = "Complete Phase Shield",
                              .cMass = 1,
                              .resCost = 20,
                              .rgwtOreCost = {12, 0, 15},
                              .ibmp = 119,
                              .dp = 500,
                       }};
TORP      rgtorp[12] = {{
                            .id = 1,
                            .szName = "Alpha Torpedo",
                            .cMass = 25,
                            .resCost = 5,
                            .rgwtOreCost = {9, 3, 3},
                            .ibmp = 87,
                            .dRangeMax = 4,
                            .dp = 5,
                            .dHitChance = 35,
                   },
                        {
                            .id = 2,
                            .rgTech = {0, 5, 1},
                            .szName = "Beta Torpedo",
                            .cMass = 25,
                            .resCost = 6,
                            .rgwtOreCost = {18, 6, 4},
                            .ibmp = 88,
                            .dRangeMax = 4,
                            .dp = 12,
                            .init = 1,
                            .dHitChance = 45,
                   },
                        {
                            .id = 3,
                            .rgTech = {0, 10, 2},
                            .szName = "Delta Torpedo",
                            .cMass = 25,
                            .resCost = 8,
                            .rgwtOreCost = {22, 8, 5},
                            .ibmp = 89,
                            .dRangeMax = 4,
                            .dp = 26,
                            .init = 1,
                            .dHitChance = 60,
                   },
                        {
                            .id = 4,
                            .rgTech = {0, 14, 3},
                            .szName = "Epsilon Torpedo",
                            .cMass = 25,
                            .resCost = 10,
                            .rgwtOreCost = {30, 10, 6},
                            .ibmp = 92,
                            .dRangeMax = 5,
                            .dp = 48,
                            .init = 2,
                            .dHitChance = 65,
                   },
                        {
                            .id = 5,
                            .rgTech = {0, 18, 4},
                            .szName = "Rho Torpedo",
                            .cMass = 25,
                            .resCost = 12,
                            .rgwtOreCost = {34, 12, 8},
                            .ibmp = 93,
                            .dRangeMax = 5,
                            .dp = 90,
                            .init = 2,
                            .dHitChance = 75,
                   },
                        {
                            .id = 6,
                            .rgTech = {0, 22, 5},
                            .szName = "Upsilon Torpedo",
                            .cMass = 25,
                            .resCost = 15,
                            .rgwtOreCost = {40, 14, 9},
                            .ibmp = 94,
                            .dRangeMax = 5,
                            .dp = 169,
                            .init = 3,
                            .dHitChance = 75,
                   },
                        {
                            .id = 7,
                            .rgTech = {0, 26, 6},
                            .szName = "Omega Torpedo",
                            .cMass = 25,
                            .resCost = 18,
                            .rgwtOreCost = {52, 18, 12},
                            .ibmp = 95,
                            .dRangeMax = 5,
                            .dp = 316,
                            .init = 4,
                            .dHitChance = 80,
                   },
                        {
                            .id = 8,
                            .rgTech = {0, 11, 12, 0, 0, 21},
                            .szName = "Anti Matter Torpedo",
                            .cMass = 8,
                            .resCost = 50,
                            .rgwtOreCost = {3, 8, 1},
                            .ibmp = 108,
                            .dRangeMax = 6,
                            .dp = 60,
                            .dHitChance = 85,
                   },
                        {
                            .id = 9,
                            .rgTech = {0, 12, 6},
                            .szName = "Jihad Missile",
                            .cMass = 35,
                            .resCost = 13,
                            .rgwtOreCost = {37, 13, 9},
                            .ibmp = 200,
                            .dRangeMax = 5,
                            .dp = 85,
                            .dHitChance = 20,
                   },
                        {
                            .id = 10,
                            .rgTech = {0, 16, 8},
                            .szName = "Juggernaut Missile",
                            .cMass = 35,
                            .resCost = 16,
                            .rgwtOreCost = {48, 16, 11},
                            .ibmp = 201,
                            .dRangeMax = 5,
                            .dp = 150,
                            .init = 1,
                            .dHitChance = 20,
                   },
                        {
                            .id = 11,
                            .rgTech = {0, 20, 10},
                            .szName = "Doomsday Missile",
                            .cMass = 35,
                            .resCost = 20,
                            .rgwtOreCost = {60, 20, 13},
                            .ibmp = 202,
                            .dRangeMax = 6,
                            .dp = 280,
                            .init = 2,
                            .dHitChance = 25,
                   },
                        {
                            .id = 12,
                            .rgTech = {0, 24, 10},
                            .szName = "Armageddon Missile",
                            .cMass = 35,
                            .resCost = 24,
                            .rgwtOreCost = {67, 23, 16},
                            .ibmp = 203,
                            .dRangeMax = 6,
                            .dp = 525,
                            .init = 3,
                            .dHitChance = 30,
                   }};
TERRA     rgterra[20] = {{
                             .id = 1,
                             .szName = {84, 111, 116, 97, 108, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 51},
                             .resCost = 70,
                             .ibmp = 184,
                             .grAbility = 3,
                     },
                         {
                             .id = 2,
                             .rgTech = {0, 0, 0, 0, 0, 3},
                             .szName = {84, 111, 116, 97, 108, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 53},
                             .resCost = 70,
                             .ibmp = 185,
                             .grAbility = 5,
                     },
                         {
                             .id = 3,
                             .rgTech = {0, 0, 0, 0, 0, 6},
                             .szName = {84, 111, 116, 97, 108, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 55},
                             .resCost = 70,
                             .ibmp = 186,
                             .grAbility = 7,
                     },
                         {
                             .id = 4,
                             .rgTech = {0, 0, 0, 0, 0, 9},
                             .szName = {84, 111, 116, 97, 108, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 49, 48},
                             .resCost = 70,
                             .ibmp = 187,
                             .grAbility = 10,
                     },
                         {
                             .id = 5,
                             .rgTech = {0, 0, 0, 0, 0, 13},
                             .szName = {84, 111, 116, 97, 108, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 49, 53},
                             .resCost = 70,
                             .ibmp = 188,
                             .grAbility = 15,
                     },
                         {
                             .id = 6,
                             .rgTech = {0, 0, 0, 0, 0, 17},
                             .szName = {84, 111, 116, 97, 108, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 50, 48},
                             .resCost = 70,
                             .ibmp = 180,
                             .grAbility = 20,
                     },
                         {
                             .id = 7,
                             .rgTech = {0, 0, 0, 0, 0, 22},
                             .szName = {84, 111, 116, 97, 108, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 50, 53},
                             .resCost = 70,
                             .ibmp = 172,
                             .grAbility = 25,
                     },
                         {
                             .id = 8,
                             .rgTech = {0, 0, 0, 0, 0, 25},
                             .szName = {84, 111, 116, 97, 108, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 51, 48},
                             .resCost = 70,
                             .ibmp = 164,
                             .grAbility = 30,
                     },
                         {
                             .id = 9,
                             .rgTech = {0, 0, 1, 0, 0, 1},
                             .szName = {71, 114, 97, 118, 105, 116, 121, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 51},
                             .resCost = 100,
                             .ibmp = 160,
                             .grAbility = 3,
                     },
                         {
                             .id = 10,
                             .rgTech = {0, 0, 5, 0, 0, 2},
                             .szName = {71, 114, 97, 118, 105, 116, 121, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 55},
                             .resCost = 100,
                             .ibmp = 161,
                             .grAbility = 7,
                     },
                         {
                             .id = 11,
                             .rgTech = {0, 0, 10, 0, 0, 3},
                             .szName = {71, 114, 97, 118, 105, 116, 121, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 49, 49},
                             .resCost = 100,
                             .ibmp = 162,
                             .grAbility = 11,
                     },
                         {
                             .id = 12,
                             .rgTech = {0, 0, 16, 0, 0, 4},
                             .szName = {71, 114, 97, 118, 105, 116, 121, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 49, 53},
                             .resCost = 100,
                             .ibmp = 163,
                             .grAbility = 15,
                     },
                         {
                             .id = 13,
                             .rgTech = {1, 0, 0, 0, 0, 1},
                             .szName = {84, 101, 109, 112, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 51},
                             .resCost = 100,
                             .ibmp = 168,
                             .grAbility = 3,
                     },
                         {
                             .id = 14,
                             .rgTech = {5, 0, 0, 0, 0, 2},
                             .szName = {84, 101, 109, 112, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 55},
                             .resCost = 100,
                             .ibmp = 169,
                             .grAbility = 7,
                     },
                         {
                             .id = 15,
                             .rgTech = {10, 0, 0, 0, 0, 3},
                             .szName = {84, 101, 109, 112, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 49, 49},
                             .resCost = 100,
                             .ibmp = 170,
                             .grAbility = 11,
                     },
                         {
                             .id = 16,
                             .rgTech = {16, 0, 0, 0, 0, 4},
                             .szName = {84, 101, 109, 112, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 49, 53},
                             .resCost = 100,
                             .ibmp = 171,
                             .grAbility = 15,
                     },
                         {
                             .id = 17,
                             .rgTech = {0, 1, 0, 0, 0, 1},
                             .szName = {82, 97, 100, 105, 97, 116, 105, 111, 110, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 51},
                             .resCost = 100,
                             .ibmp = 176,
                             .grAbility = 3,
                     },
                         {
                             .id = 18,
                             .rgTech = {0, 5, 0, 0, 0, 2},
                             .szName = {82, 97, 100, 105, 97, 116, 105, 111, 110, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 55},
                             .resCost = 100,
                             .ibmp = 177,
                             .grAbility = 7,
                     },
                         {
                             .id = 19,
                             .rgTech = {0, 10, 0, 0, 0, 3},
                             .szName = {82, 97, 100, 105, 97, 116, 105, 111, 110, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 49, 49},
                             .resCost = 100,
                             .ibmp = 178,
                             .grAbility = 11,
                     },
                         {
                             .id = 20,
                             .rgTech = {0, 16, 0, 0, 0, 4},
                             .szName = {82, 97, 100, 105, 97, 116, 105, 111, 110, 32, 84, 101, 114, 114, 97, 102, 111, 114, 109, 32, -79, 49, 53},
                             .resCost = 100,
                             .ibmp = 179,
                             .grAbility = 15,
                     }};
PLANETARY rgplanetary[15] = {{
                                 .id = 1,
                                 .szName = "Viewer 50",
                                 .resCost = 100,
                                 .rgwtOreCost = {10, 10, 70},
                                 .ibmp = 80,
                                 .grAbility = 50,
                             },
                             {
                                 .id = 2,
                                 .rgTech = {0, 0, 0, 0, 1},
                                 .szName = "Viewer 90",
                                 .resCost = 100,
                                 .rgwtOreCost = {10, 10, 70},
                                 .ibmp = 81,
                                 .grAbility = 90,
                             },
                             {
                                 .id = 3,
                                 .rgTech = {0, 0, 0, 0, 3},
                                 .szName = "Scoper 150",
                                 .resCost = 100,
                                 .rgwtOreCost = {10, 10, 70},
                                 .ibmp = 82,
                                 .grAbility = 150,
                             },
                             {
                                 .id = 4,
                                 .rgTech = {0, 0, 0, 0, 6},
                                 .szName = "Scoper 220",
                                 .resCost = 100,
                                 .rgwtOreCost = {10, 10, 70},
                                 .ibmp = 83,
                                 .grAbility = 220,
                             },
                             {
                                 .id = 5,
                                 .rgTech = {0, 0, 0, 0, 8},
                                 .szName = "Scoper 280",
                                 .resCost = 100,
                                 .rgwtOreCost = {10, 10, 70},
                                 .ibmp = 90,
                                 .grAbility = 280,
                             },
                             {
                                 .id = 6,
                                 .rgTech = {3, 0, 0, 0, 10, 3},
                                 .szName = "Snooper 320X",
                                 .resCost = 100,
                                 .rgwtOreCost = {10, 10, 70},
                                 .ibmp = 84,
                                 .grAbility = -320,
                             },
                             {
                                 .id = 7,
                                 .rgTech = {4, 0, 0, 0, 13, 6},
                                 .szName = "Snooper 400X",
                                 .resCost = 100,
                                 .rgwtOreCost = {10, 10, 70},
                                 .ibmp = 85,
                                 .grAbility = -400,
                             },
                             {
                                 .id = 8,
                                 .rgTech = {5, 0, 0, 0, 16, 7},
                                 .szName = "Snooper 500X",
                                 .resCost = 100,
                                 .rgwtOreCost = {10, 10, 70},
                                 .ibmp = 86,
                                 .grAbility = -500,
                             },
                             {
                                 .id = 9,
                                 .rgTech = {7, 0, 0, 0, 23, 9},
                                 .szName = "Snooper 620X",
                                 .resCost = 100,
                                 .rgwtOreCost = {10, 10, 70},
                                 .ibmp = 91,
                                 .grAbility = -620,
                             },
                             {
                                 .id = 10,
                                 .szName = "SDI",
                                 .resCost = 15,
                                 .rgwtOreCost = {5, 5, 5},
                                 .ibmp = 72,
                                 .grAbility = 10,
                             },
                             {
                                 .id = 11,
                                 .rgTech = {5},
                                 .szName = "Missile Battery",
                                 .resCost = 15,
                                 .rgwtOreCost = {5, 5, 5},
                                 .ibmp = 73,
                                 .grAbility = 20,
                             },
                             {
                                 .id = 12,
                                 .rgTech = "\n",
                                 .szName = "Laser Battery",
                                 .resCost = 15,
                                 .rgwtOreCost = {5, 5, 5},
                                 .ibmp = 74,
                                 .grAbility = 24,
                             },
                             {
                                 .id = 13,
                                 .rgTech = {16},
                                 .szName = "Planetary Shield",
                                 .resCost = 15,
                                 .rgwtOreCost = {5, 5, 5},
                                 .ibmp = 75,
                                 .grAbility = 30,
                             },
                             {
                                 .id = 14,
                                 .rgTech = {23},
                                 .szName = "Neutron Shield",
                                 .resCost = 15,
                                 .rgwtOreCost = {5, 5, 5},
                                 .ibmp = 76,
                                 .grAbility = 38,
                             },
                             {
                                 .id = 15,
                                 .rgTech = {20, 10, 10, 20, 10, 20},
                                 .szName = "Genesis Device",
                                 .resCost = 5000,
                                 .ibmp = 175,
                             }};
BOMB      rgbomb[15] = {{
                            .id = 1,
                            .rgTech = {0, 2},
                            .szName = "Lady Finger Bomb",
                            .cMass = 40,
                            .resCost = 5,
                            .rgwtOreCost = {1, 20},
                            .ibmp = 35,
                            .cRounds = 1,
                            .dDmgCol = 6,
                            .dDmgBldg = 2,
                   },
                        {
                            .id = 2,
                            .rgTech = {0, 5},
                            .szName = "Black Cat Bomb",
                            .cMass = 45,
                            .resCost = 7,
                            .rgwtOreCost = {1, 22},
                            .ibmp = 36,
                            .cRounds = 1,
                            .dDmgCol = 9,
                            .dDmgBldg = 4,
                   },
                        {
                            .id = 3,
                            .rgTech = {0, 8},
                            .szName = "M-70 Bomb",
                            .cMass = 50,
                            .resCost = 9,
                            .rgwtOreCost = {1, 24},
                            .ibmp = 37,
                            .cRounds = 1,
                            .dDmgCol = 12,
                            .dDmgBldg = 6,
                   },
                        {
                            .id = 4,
                            .rgTech = {0, 11},
                            .szName = "M-80 Bomb",
                            .cMass = 55,
                            .resCost = 12,
                            .rgwtOreCost = {1, 25},
                            .ibmp = 38,
                            .cRounds = 1,
                            .dDmgCol = 17,
                            .dDmgBldg = 7,
                   },
                        {
                            .id = 5,
                            .rgTech = {0, 14},
                            .szName = "Cherry Bomb",
                            .cMass = 52,
                            .resCost = 11,
                            .rgwtOreCost = {1, 25},
                            .ibmp = 39,
                            .cRounds = 1,
                            .dDmgCol = 25,
                            .dDmgBldg = 10,
                   },
                        {
                            .id = 6,
                            .rgTech = {0, 5, 0, 0, 8},
                            .szName = "LBU-17 Bomb",
                            .cMass = 30,
                            .resCost = 7,
                            .rgwtOreCost = {1, 15, 15},
                            .ibmp = 32,
                            .cRounds = 1,
                            .dDmgCol = 2,
                            .dDmgBldg = 16,
                   },
                        {
                            .id = 7,
                            .rgTech = {0, 10, 0, 0, 10},
                            .szName = "LBU-32 Bomb",
                            .cMass = 35,
                            .resCost = 10,
                            .rgwtOreCost = {1, 24, 15},
                            .ibmp = 33,
                            .cRounds = 1,
                            .dDmgCol = 3,
                            .dDmgBldg = 28,
                   },
                        {
                            .id = 8,
                            .rgTech = {0, 15, 0, 0, 12},
                            .szName = "LBU-74 Bomb",
                            .cMass = 45,
                            .resCost = 14,
                            .rgwtOreCost = {1, 33, 12},
                            .ibmp = 34,
                            .cRounds = 1,
                            .dDmgCol = 4,
                            .dDmgBldg = 45,
                   },
                        {
                            .id = 9,
                            .rgTech = {0, 12, 0, 0, 12, 12},
                            .szName = "Hush-a-Boom",
                            .cMass = 5,
                            .resCost = 5,
                            .rgwtOreCost = {1, 5},
                            .ibmp = 182,
                            .cRounds = 1,
                            .dDmgCol = 30,
                            .dDmgBldg = 2,
                   },
                        {
                            .id = 10,
                            .rgTech = {0, 10, 0, 0, 0, 12},
                            .szName = "Retro Bomb",
                            .cMass = 45,
                            .resCost = 50,
                            .rgwtOreCost = {15, 15, 10},
                            .ibmp = 174,
                            .cRounds = 1,
                   },
                        {
                            .id = 11,
                            .rgTech = {0, 5, 0, 0, 0, 7},
                            .szName = "Smart Bomb",
                            .cMass = 50,
                            .resCost = 27,
                            .rgwtOreCost = {1, 22},
                            .ibmp = 112,
                            .cRounds = 1,
                            .dDmgCol = 13,
                   },
                        {
                            .id = 12,
                            .rgTech = {0, 10, 0, 0, 0, 10},
                            .szName = "Neutron Bomb",
                            .cMass = 57,
                            .resCost = 30,
                            .rgwtOreCost = {1, 30},
                            .ibmp = 113,
                            .cRounds = 1,
                            .dDmgCol = 22,
                   },
                        {
                            .id = 13,
                            .rgTech = {0, 15, 0, 0, 0, 12},
                            .szName = "Enriched Neutron Bomb",
                            .cMass = 64,
                            .resCost = 25,
                            .rgwtOreCost = {1, 36},
                            .ibmp = 114,
                            .cRounds = 1,
                            .dDmgCol = 35,
                   },
                        {
                            .id = 14,
                            .rgTech = {0, 22, 0, 0, 0, 15},
                            .szName = "Peerless Bomb",
                            .cMass = 55,
                            .resCost = 32,
                            .rgwtOreCost = {1, 33},
                            .ibmp = 115,
                            .cRounds = 1,
                            .dDmgCol = 50,
                   },
                        {
                            .id = 15,
                            .rgTech = {0, 26, 0, 0, 0, 17},
                            .szName = "Annihilator Bomb",
                            .cMass = 50,
                            .resCost = 28,
                            .rgwtOreCost = {1, 30},
                            .ibmp = 116,
                            .cRounds = 1,
                            .dDmgCol = 70,
                   }};
ARMOR     rgarmor[12] = {{
                             .id = 1,
                             .szName = "Tritanium",
                             .cMass = 60,
                             .resCost = 10,
                             .rgwtOreCost = {5},
                             .ibmp = 64,
                             .dp = 50,
                     },
                         {
                             .id = 2,
                             .rgTech = {0, 0, 0, 3},
                             .szName = "Crobmnium",
                             .cMass = 56,
                             .resCost = 13,
                             .rgwtOreCost = {6},
                             .ibmp = 65,
                             .dp = 75,
                     },
                         {
                             .id = 3,
                             .rgTech = {0, 0, 0, 0, 0, 4},
                             .szName = "Carbonic Armor",
                             .cMass = 25,
                             .resCost = 15,
                             .rgwtOreCost = {0, 0, 5},
                             .ibmp = 70,
                             .dp = 100,
                     },
                         {
                             .id = 4,
                             .rgTech = {0, 0, 0, 6},
                             .szName = "Strobnium",
                             .cMass = 54,
                             .resCost = 18,
                             .rgwtOreCost = {8},
                             .ibmp = 68,
                             .dp = 120,
                     },
                         {
                             .id = 5,
                             .rgTech = {0, 0, 0, 0, 0, 7},
                             .szName = "Organic Armor",
                             .cMass = 15,
                             .resCost = 20,
                             .rgwtOreCost = {0, 0, 6},
                             .ibmp = 71,
                             .dp = 175,
                     },
                         {
                             .id = 6,
                             .rgTech = {0, 0, 0, 9},
                             .szName = "Kelarium",
                             .cMass = 50,
                             .resCost = 25,
                             .rgwtOreCost = {9, 1},
                             .ibmp = 67,
                             .dp = 180,
                     },
                         {
                             .id = 7,
                             .rgTech = {4, 0, 0, 10},
                             .szName = "Fielded Kelarium",
                             .cMass = 50,
                             .resCost = 28,
                             .rgwtOreCost = {10, 0, 2},
                             .ibmp = 78,
                             .dp = 175,
                     },
                         {
                             .id = 8,
                             .rgTech = {0, 0, 0, 10, 3},
                             .szName = "Depleted Neutronium",
                             .cMass = 50,
                             .resCost = 28,
                             .rgwtOreCost = {10, 0, 2},
                             .ibmp = 79,
                             .dp = 200,
                     },
                         {
                             .id = 9,
                             .rgTech = {0, 0, 0, 12},
                             .szName = "Neutronium",
                             .cMass = 45,
                             .resCost = 30,
                             .rgwtOreCost = {11, 2, 1},
                             .ibmp = 69,
                             .dp = 275,
                     },
                         {
                             .id = 10,
                             .rgTech = {14, 0, 0, 14, 14, 6},
                             .szName = "Mega Poly Shell",
                             .cMass = 20,
                             .resCost = 65,
                             .rgwtOreCost = {18, 6, 6},
                             .ibmp = 110,
                             .dp = 400,
                     },
                         {
                             .id = 11,
                             .rgTech = {0, 0, 0, 16},
                             .szName = "Valanium",
                             .cMass = 40,
                             .resCost = 50,
                             .rgwtOreCost = {15},
                             .ibmp = 66,
                             .dp = 500,
                     },
                         {
                             .id = 12,
                             .rgTech = {0, 0, 0, 24},
                             .szName = "Superlatanium",
                             .cMass = 30,
                             .resCost = 100,
                             .rgwtOreCost = {25},
                             .ibmp = 77,
                             .dp = 1500,
                     }};
SPECIALSB rgspecialSB[16] = {{
                                 .id = 1,
                                 .rgTech = {0, 0, 5, 5},
                                 .szName = "Stargate 100/250",
                                 .resCost = 400,
                                 .rgwtOreCost = {100, 40, 40},
                                 .ibmp = 144,
                                 .grAbility = 100,
                                 .grAbility2 = 250,
                             },
                             {
                                 .id = 2,
                                 .rgTech = {0, 0, 6, 10},
                                 .szName = "Stargate any/300",
                                 .resCost = 500,
                                 .rgwtOreCost = {100, 40, 40},
                                 .ibmp = 145,
                                 .grAbility = -1,
                                 .grAbility2 = 300,
                             },
                             {
                                 .id = 3,
                                 .rgTech = {0, 0, 11, 7},
                                 .szName = "Stargate 150/600",
                                 .resCost = 1000,
                                 .rgwtOreCost = {100, 40, 40},
                                 .ibmp = 146,
                                 .grAbility = 150,
                                 .grAbility2 = 600,
                             },
                             {
                                 .id = 4,
                                 .rgTech = {0, 0, 9, 13},
                                 .szName = "Stargate 300/500",
                                 .resCost = 1200,
                                 .rgwtOreCost = {100, 40, 40},
                                 .ibmp = 147,
                                 .grAbility = 300,
                                 .grAbility2 = 500,
                             },
                             {
                                 .id = 5,
                                 .rgTech = {0, 0, 16, 12},
                                 .szName = "Stargate 100/any",
                                 .resCost = 1400,
                                 .rgwtOreCost = {100, 40, 40},
                                 .ibmp = 148,
                                 .grAbility = 100,
                                 .grAbility2 = -1,
                             },
                             {
                                 .id = 6,
                                 .rgTech = {0, 0, 12, 18},
                                 .szName = "Stargate any/800",
                                 .resCost = 1400,
                                 .rgwtOreCost = {100, 40, 40},
                                 .ibmp = 149,
                                 .grAbility = -1,
                                 .grAbility2 = 800,
                             },
                             {
                                 .id = 7,
                                 .rgTech = {0, 0, 19, 24},
                                 .szName = "Stargate any/any",
                                 .resCost = 1600,
                                 .rgwtOreCost = {100, 40, 40},
                                 .ibmp = 150,
                                 .grAbility = -1,
                                 .grAbility2 = -1,
                             },
                             {
                                 .id = 8,
                                 .rgTech = {4},
                                 .szName = "Mass Driver 5",
                                 .resCost = 140,
                                 .rgwtOreCost = {48, 40, 40},
                                 .ibmp = 151,
                                 .grAbility = 5,
                             },
                             {
                                 .id = 9,
                                 .rgTech = {7},
                                 .szName = "Mass Driver 6",
                                 .resCost = 288,
                                 .rgwtOreCost = {48, 40, 40},
                                 .ibmp = 152,
                                 .grAbility = 6,
                             },
                             {
                                 .id = 10,
                                 .rgTech = "\t",
                                 .szName = "Mass Driver 7",
                                 .resCost = 1024,
                                 .rgwtOreCost = {200, 200, 200},
                                 .ibmp = 153,
                                 .grAbility = 7,
                             },
                             {
                                 .id = 11,
                                 .rgTech = {11},
                                 .szName = "Super Driver 8",
                                 .resCost = 512,
                                 .rgwtOreCost = {48, 40, 40},
                                 .ibmp = 154,
                                 .grAbility = 8,
                             },
                             {
                                 .id = 12,
                                 .rgTech = "\r",
                                 .szName = "Super Driver 9",
                                 .resCost = 648,
                                 .rgwtOreCost = {48, 40, 40},
                                 .ibmp = 155,
                                 .grAbility = 9,
                             },
                             {
                                 .id = 13,
                                 .rgTech = {15},
                                 .szName = "Ultra Driver 10",
                                 .resCost = 1936,
                                 .rgwtOreCost = {200, 200, 200},
                                 .ibmp = 156,
                                 .grAbility = 10,
                             },
                             {
                                 .id = 14,
                                 .rgTech = {17},
                                 .szName = "Ultra Driver 11",
                                 .resCost = 968,
                                 .rgwtOreCost = {48, 40, 40},
                                 .ibmp = 157,
                                 .grAbility = 11,
                             },
                             {
                                 .id = 15,
                                 .rgTech = {20},
                                 .szName = "Ultra Driver 12",
                                 .resCost = 1152,
                                 .rgwtOreCost = {48, 40, 40},
                                 .ibmp = 158,
                                 .grAbility = 12,
                             },
                             {
                                 .id = 16,
                                 .rgTech = {24},
                                 .szName = "Ultra Driver 13",
                                 .resCost = 1352,
                                 .rgwtOreCost = {48, 40, 40},
                                 .ibmp = 159,
                                 .grAbility = 13,
                             }};
SPECIAL   rgspecialM[11] = {{
                                .id = 1,
                                .szName = "Colonization Module",
                                .cMass = 32,
                                .resCost = 10,
                                .rgwtOreCost = {12, 10, 10},
                                .ibmp = 106,
                                .grAbility = 1,
                          },
                            {
                                .id = 2,
                                .szName = "Orbital Construction Module",
                                .cMass = 50,
                                .resCost = 20,
                                .rgwtOreCost = {20, 15, 15},
                                .ibmp = 107,
                                .grAbility = 1,
                          },
                            {
                                .id = 3,
                                .rgTech = {0, 0, 0, 3},
                                .szName = "Cargo Pod",
                                .cMass = 5,
                                .resCost = 10,
                                .rgwtOreCost = {5, 0, 2},
                                .ibmp = 96,
                                .grAbility = 2,
                          },
                            {
                                .id = 4,
                                .rgTech = {3, 0, 0, 9},
                                .szName = "Super Cargo Pod",
                                .cMass = 7,
                                .resCost = 15,
                                .rgwtOreCost = {8, 0, 2},
                                .ibmp = 97,
                                .grAbility = 2,
                          },
                            {
                                .id = 5,
                                .rgTech = {5, 0, 0, 11, 5},
                                .szName = "Multi Cargo Pod",
                                .cMass = 9,
                                .resCost = 25,
                                .rgwtOreCost = {12, 0, 3},
                                .ibmp = 118,
                                .grAbility = 2,
                          },
                            {
                                .id = 6,
                                .szName = "Fuel Tank",
                                .cMass = 3,
                                .resCost = 4,
                                .rgwtOreCost = {6},
                                .ibmp = 104,
                                .grAbility = 4,
                          },
                            {
                                .id = 7,
                                .rgTech = {6, 0, 4, 14},
                                .szName = "Super Fuel Tank",
                                .cMass = 8,
                                .resCost = 8,
                                .rgwtOreCost = {8},
                                .ibmp = 105,
                                .grAbility = 4,
                          },
                            {
                                .id = 8,
                                .rgTech = {2, 0, 3},
                                .szName = "Maneuvering Jet",
                                .cMass = 5,
                                .resCost = 10,
                                .rgwtOreCost = {5, 0, 5},
                                .ibmp = 102,
                                .grAbility = 1,
                          },
                            {
                                .id = 9,
                                .rgTech = {5, 0, 12},
                                .szName = "Overthruster",
                                .cMass = 5,
                                .resCost = 20,
                                .rgwtOreCost = {10, 0, 8},
                                .ibmp = 103,
                                .grAbility = 2,
                          },
                            {
                                .id = 10,
                                .rgTech = {16, 0, 20, 20, 16},
                                .szName = "Jump Gate",
                                .cMass = 10,
                                .resCost = 40,
                                .rgwtOreCost = {0, 0, 50},
                                .ibmp = 208,
                          },
                            {
                                .id = 11,
                                .rgTech = {6, 6, 0, 6, 6},
                                .szName = "Beam Deflector",
                                .cMass = 1,
                                .resCost = 8,
                                .rgwtOreCost = {0, 0, 10},
                                .ibmp = 209,
                                .grAbility = 10,
                          }};
SPECIAL   rgspecialE[17] = {{
                                .id = 1,
                                .szName = "Transport Cloaking",
                                .cMass = 1,
                                .resCost = 3,
                                .rgwtOreCost = {2, 0, 2},
                                .ibmp = 98,
                                .grAbility = 300,
                          },
                            {
                                .id = 2,
                                .rgTech = {2, 0, 0, 0, 5},
                                .szName = "Stealth Cloak",
                                .cMass = 2,
                                .resCost = 5,
                                .rgwtOreCost = {2, 0, 2},
                                .ibmp = 99,
                                .grAbility = 70,
                          },
                            {
                                .id = 3,
                                .rgTech = {4, 0, 0, 0, 10},
                                .szName = "Super-Stealth Cloak",
                                .cMass = 3,
                                .resCost = 15,
                                .rgwtOreCost = {8, 0, 8},
                                .ibmp = 100,
                                .grAbility = 140,
                          },
                            {
                                .id = 4,
                                .rgTech = {10, 0, 0, 0, 12},
                                .szName = "Ultra-Stealth Cloak",
                                .cMass = 5,
                                .resCost = 25,
                                .rgwtOreCost = {10, 0, 10},
                                .ibmp = 101,
                                .grAbility = 540,
                          },
                            {
                                .id = 5,
                                .rgTech = {11, 0, 11, 0, 11},
                                .szName = "Multi Function Pod",
                                .cMass = 2,
                                .resCost = 15,
                                .rgwtOreCost = {5, 0, 5},
                                .ibmp = 189,
                                .grAbility = 60,
                          },
                            {
                                .id = 6,
                                .szName = "Battle Computer",
                                .cMass = 1,
                                .resCost = 6,
                                .rgwtOreCost = {0, 0, 15},
                                .ibmp = 165,
                                .grAbility = 20,
                          },
                            {
                                .id = 7,
                                .rgTech = {5, 0, 0, 0, 11},
                                .szName = "Battle Super Computer",
                                .cMass = 1,
                                .resCost = 14,
                                .rgwtOreCost = {0, 0, 25},
                                .ibmp = 166,
                                .grAbility = 30,
                          },
                            {
                                .id = 8,
                                .rgTech = {10, 0, 0, 0, 19},
                                .szName = "Battle Nexus",
                                .cMass = 1,
                                .resCost = 15,
                                .rgwtOreCost = {0, 0, 30},
                                .ibmp = 167,
                                .grAbility = 50,
                          },
                            {
                                .id = 9,
                                .rgTech = {2, 0, 0, 0, 6},
                                .szName = "Jammer 10",
                                .cMass = 1,
                                .resCost = 6,
                                .rgwtOreCost = {0, 0, 2},
                                .ibmp = 120,
                                .grAbility = 10,
                          },
                            {
                                .id = 10,
                                .rgTech = {4, 0, 0, 0, 10},
                                .szName = "Jammer 20",
                                .cMass = 1,
                                .resCost = 20,
                                .rgwtOreCost = {1, 0, 5},
                                .ibmp = 121,
                                .grAbility = 20,
                          },
                            {
                                .id = 11,
                                .rgTech = {8, 0, 0, 0, 16},
                                .szName = "Jammer 30",
                                .cMass = 1,
                                .resCost = 20,
                                .rgwtOreCost = {1, 0, 6},
                                .ibmp = 122,
                                .grAbility = 30,
                          },
                            {
                                .id = 12,
                                .rgTech = {16, 0, 0, 0, 22},
                                .szName = "Jammer 50",
                                .cMass = 1,
                                .resCost = 20,
                                .rgwtOreCost = {2, 0, 7},
                                .ibmp = 123,
                                .grAbility = 50,
                          },
                            {
                                .id = 13,
                                .rgTech = {7, 0, 0, 0, 4},
                                .szName = "Energy Capacitor",
                                .cMass = 1,
                                .resCost = 5,
                                .rgwtOreCost = {0, 0, 8},
                                .ibmp = 127,
                                .grAbility = 10,
                          },
                            {
                                .id = 14,
                                .rgTech = {14, 0, 0, 0, 8},
                                .szName = "Flux Capacitor",
                                .cMass = 1,
                                .resCost = 5,
                                .rgwtOreCost = {0, 0, 8},
                                .ibmp = 190,
                                .grAbility = 20,
                          },
                            {
                                .id = 15,
                                .rgTech = {14, 0, 8},
                                .szName = "Energy Dampener",
                                .cMass = 2,
                                .resCost = 50,
                                .rgwtOreCost = {5, 10},
                                .ibmp = 124,
                                .grAbility = 1,
                          },
                            {
                                .id = 16,
                                .rgTech = {8, 0, 0, 0, 14},
                                .szName = "Tachyon Detector",
                                .cMass = 1,
                                .resCost = 70,
                                .rgwtOreCost = {1, 5},
                                .ibmp = 125,
                                .grAbility = 2,
                          },
                            {
                                .id = 17,
                                .rgTech = {0, 12, 0, 0, 0, 7},
                                .szName = "Anti-matter Generator",
                                .cMass = 10,
                                .resCost = 10,
                                .rgwtOreCost = {8, 3, 3},
                                .ibmp = 126,
                                .grAbility = 3,
                          }};
MINES     rgmines[10] = {{
                             .id = 1,
                             .szName = "Mine Dispenser 40",
                             .cMass = 25,
                             .resCost = 45,
                             .rgwtOreCost = {2, 10, 8},
                             .ibmp = 128,
                             .grAbility = 4,
                     },
                         {
                             .id = 2,
                             .rgTech = {2, 0, 0, 0, 0, 4},
                             .szName = "Mine Dispenser 50",
                             .cMass = 30,
                             .resCost = 55,
                             .rgwtOreCost = {2, 12, 10},
                             .ibmp = 129,
                             .grAbility = 5,
                     },
                         {
                             .id = 3,
                             .rgTech = {3, 0, 0, 0, 0, 7},
                             .szName = "Mine Dispenser 80",
                             .cMass = 30,
                             .resCost = 65,
                             .rgwtOreCost = {2, 14, 10},
                             .ibmp = 130,
                             .grAbility = 8,
                     },
                         {
                             .id = 4,
                             .rgTech = {6, 0, 0, 0, 0, 12},
                             .szName = "Mine Dispenser 130",
                             .cMass = 30,
                             .resCost = 80,
                             .rgwtOreCost = {2, 18, 10},
                             .ibmp = 131,
                             .grAbility = 13,
                     },
                         {
                             .id = 5,
                             .rgTech = {5, 0, 0, 0, 0, 3},
                             .szName = "Heavy Dispenser 50",
                             .cMass = 10,
                             .resCost = 50,
                             .rgwtOreCost = {2, 20, 5},
                             .ibmp = 135,
                             .grAbility = 5,
                     },
                         {
                             .id = 6,
                             .rgTech = {9, 0, 0, 0, 0, 5},
                             .szName = "Heavy Dispenser 110",
                             .cMass = 15,
                             .resCost = 70,
                             .rgwtOreCost = {2, 30, 5},
                             .ibmp = 136,
                             .grAbility = 11,
                     },
                         {
                             .id = 7,
                             .rgTech = {14, 0, 0, 0, 0, 7},
                             .szName = "Heavy Dispenser 200",
                             .cMass = 20,
                             .resCost = 90,
                             .rgwtOreCost = {2, 45, 5},
                             .ibmp = 137,
                             .grAbility = 20,
                     },
                         {
                             .id = 8,
                             .rgTech = {0, 0, 2, 0, 0, 2},
                             .szName = "Speed Trap 20",
                             .cMass = 100,
                             .resCost = 60,
                             .rgwtOreCost = {30, 0, 12},
                             .ibmp = 132,
                             .grAbility = 2,
                     },
                         {
                             .id = 9,
                             .rgTech = {0, 0, 3, 0, 0, 6},
                             .szName = "Speed Trap 30",
                             .cMass = 135,
                             .resCost = 72,
                             .rgwtOreCost = {32, 0, 14},
                             .ibmp = 133,
                             .grAbility = 3,
                     },
                         {
                             .id = 10,
                             .rgTech = {0, 0, 5, 0, 0, 11},
                             .szName = "Speed Trap 50",
                             .cMass = 140,
                             .resCost = 80,
                             .rgwtOreCost = {40, 0, 15},
                             .ibmp = 134,
                             .grAbility = 5,
                     }};

HULDEF *LphuldefSBFromId(int16_t id) { return &rghuldefSB[id]; }

HULDEF *LphuldefFromId(int16_t id) {
    if (id >= 32) {
        return LphuldefSBFromId(id - 32);
    }
    return &rghuldef[id];
}

ENGINE *LpengineFromId(int16_t id) { return &rgengine[id]; }

SCANNER *LpscannerFromId(int16_t id) { return &rgscanner[id]; }

SHDEF *LpshdefT() { return rgshdefT; }

SHDEF *LpshdefSBT() { return rgshdefSBT; }

PLANETARY *LpplanetaryFromId(int16_t id) { return &rgplanetary[id]; }

int16_t FLookupPartX(PART *ppart, uint16_t grhst, uint16_t iItem) {
    ppart->hs.grhst = grhst;
    ppart->hs.iItem = iItem;
    ppart->hs.cItem = 0;
    return FLookupPart(ppart);
}

int16_t FLookupPart(PART *ppart) {
    int16_t raMajor;
    HS      hs;

    raMajor = GetRaceStat(&rgplr[idPlayer], rsMajorAdv);
    hs = ppart->hs;
    switch (hs.grhst) {
    default:
        return 0;
    case hstEngine:
        if (hs.iItem >= iengineCount) {
            return 0;
        }
        ppart->pengine = &rgengine[hs.iItem];
        if (idPlayer == -1)
            break;
        if (hs.iItem == iengineSettlersDelight && raMajor != 0) {
            return -1;
        }
        if (((hs.iItem >= iengineSubGalacticFuelScoop && hs.iItem <= iengineGalaxyScoop) || hs.iItem == iengineRadiatingHydroRamScoop) &&
            GetRaceGrbit(&rgplr[idPlayer], ibitRaceNoRamscoops) != 0) {
            return -1;
        }
        if ((hs.iItem == iengineGalaxyScoop || hs.iItem == iengineFuelMizer) && GetRaceGrbit(&rgplr[idPlayer], ibitRaceIFE) == 0) {
            return -1;
        }
        if (hs.iItem == iengineInterspace10 && GetRaceGrbit(&rgplr[idPlayer], ibitRaceNoRamscoops) == 0) {
            return -1;
        }
        if (FShouldPartBeHidden(ppart) == 0)
            break;
        return -1;
    case hstShield:
        if (hs.iItem >= ishieldCount) {
            return 0;
        }
        ppart->pshield = &rgshield[hs.iItem];
        if (hs.iItem == ishieldShadowShield && raMajor != 1) {
            return -1;
        }
        if (hs.iItem == ishieldCrobySharmor && raMajor != 4) {
            return -1;
        }
        if (FShouldPartBeHidden(ppart) == 0)
            break;
        return -1;
    case hstHull:
        if (hs.iItem >= ihuldefOrbitalFort) {
            return 0;
        }
        ppart->phul = &rghuldef[hs.iItem].hul;
        if (idPlayer == -1)
            break;
        if ((hs.iItem == ihuldefMiniColonyShip || hs.iItem == ihuldefMetaMorph) && raMajor != 0) {
            return -1;
        }
        if ((hs.iItem == ihuldefFuelTransport || hs.iItem == ihuldefSuperFreighter) && raMajor != 4) {
            return -1;
        }
        switch (hs.iItem) {
        case ihuldefMiner:
        case ihuldefMaxiMiner:
        case ihuldefMidgetMiner:
        case ihuldefUltraMiner:
            if (GetRaceGrbit(&rgplr[idPlayer], ibitRaceOBRM) != 0) {
                return -1;
            }
        default:
            switch (hs.iItem) {
            case ihuldefMidgetMiner:
            case ihuldefMiner:
            case ihuldefUltraMiner:
                if (GetRaceGrbit(&rgplr[idPlayer], ibitRaceARM) == 0) {
                    return -1;
                }
            default:
                if ((hs.iItem == ihuldefDreadnought || hs.iItem == ihuldefBattleCruiser) && raMajor != 2) {
                    return -1;
                }
                if (hs.iItem == ihuldefRogue && raMajor != 1) {
                    return -1;
                }
                if (hs.iItem == ihuldefStealthBomber && raMajor != 1) {
                    return -1;
                }
                if ((hs.iItem == ihuldefMiniMineLayer || hs.iItem == ihuldefSuperMineLayer) && raMajor != 5) {
                    return -1;
                }
                if (FShouldPartBeHidden(ppart) == 0)
                    goto L_609c;
                return -1;
            }
        }
    case hstSBHull:
        if (hs.iItem >= 5) {
            return 0;
        }
        ppart->phul = &rghuldefSB[hs.iItem].hul;
        if (idPlayer == -1)
            break;
        if ((hs.iItem == 1 || hs.iItem == 3) && GetRaceGrbit(&rgplr[idPlayer], ibitRaceISB) == 0) {
            return -1;
        }
        if (hs.iItem != 4 || raMajor == 8)
            break;
        return -1;
    case hstArmor:
        if (hs.iItem >= iarmorCount) {
            return 0;
        }
        ppart->parmor = &rgarmor[hs.iItem];
        if (hs.iItem == iarmorDepletedNeutronium && raMajor != 1) {
            return -1;
        }
        if (hs.iItem == iarmorFieldedKelarium && raMajor != 4) {
            return -1;
        }
        if (FShouldPartBeHidden(ppart) == 0)
            break;
        return -1;
    case hstSpecialE:
        if (hs.iItem >= ispecialECount) {
            return 0;
        }
        ppart->pspecial = &rgspecialE[hs.iItem];
        if (idPlayer == -1)
            break;
        if (FShouldPartBeHidden(ppart) != 0) {
            return -1;
        }
        if (hs.iItem > ispecialEAntiMatterGenerator)
            break;
        switch (hs.iItem) {
        case 0:
        case 3:
            if (raMajor == 1)
                goto L_609c;
            return -1;
        case 14:
            if (raMajor == 5)
                goto L_609c;
            return -1;
        case 16:
            if (raMajor == 7)
                goto L_609c;
            return -1;
        case 13:
            if (raMajor == 0)
                goto L_609c;
            return -1;
        case 8:
        case 11:
        case 15:
            if (raMajor != 4) {
                return -1;
            }
        case 1:
        case 2:
        case 4:
        case 5:
        case 6:
        case 7:
        case 9:
        case 10:
        case 12:
            goto L_609c;
        }
    case hstSpecialM:
        if (hs.iItem >= ispecialMCount) {
            return 0;
        }
        ppart->pspecial = &rgspecialM[hs.iItem];
        if (idPlayer == -1)
            break;
        if (FShouldPartBeHidden(ppart) != 0) {
            return -1;
        }
        if (hs.iItem == ispecialMColonizationModule && raMajor == 8) {
            return -1;
        }
        if (hs.iItem != ispecialMOrbitalConstructionModule || raMajor == 8)
            break;
        return -1;
    case hstSpecialSB:
        if (hs.iItem >= ispecialSBCount) {
            return 0;
        }
        ppart->pspecialsb = &rgspecialSB[hs.iItem];
        if (idPlayer == -1)
            break;
        if (hs.iItem >= ispecialSBMassDriver5 && hs.iItem <= ispecialSBUltraDriver13) {
            if (hs.iItem == ispecialSBMassDriver7 || hs.iItem == ispecialSBUltraDriver10 || raMajor == 6)
                break;
            return -1;
        }
        if (hs.iItem < ispecialSBStargate100250 || hs.iItem > ispecialSBStargateAnyAny)
            break;
        if (raMajor != 7 && (hs.iItem == ispecialSBStargateAny300 || hs.iItem >= ispecialSBStargate100Any)) {
            return -1;
        }
        if (raMajor != 0)
            break;
        return -1;
    case hstMines:
        if (hs.iItem >= iminesCount) {
            return 0;
        }
        ppart->pmines = &rgmines[hs.iItem];
        if (idPlayer == -1)
            break;
        switch (hs.iItem) {
        case iminesMineDispenser40:
        case iminesMineDispenser80:
        case iminesMineDispenser130:
        case iminesHeavyDispenser50:
        case iminesHeavyDispenser110:
        case iminesHeavyDispenser200:
        case iminesSpeedTrap30:
        case iminesSpeedTrap50:
            if (raMajor != 5) {
                return -1;
            }
        default:
            if (hs.iItem == iminesSpeedTrap20 && raMajor != 5 && raMajor != 4) {
                return -1;
            }
            if (hs.iItem != iminesMineDispenser50 || raMajor != 2)
                goto L_609c;
            return -1;
        }
    case hstMining:
        if (hs.iItem >= iminingCount) {
            return 0;
        }
        ppart->pmining = &rgmining[hs.iItem];
        if (idPlayer == -1)
            break;
        switch (hs.iItem) {
        case iminingRoboMiner:
        case iminingRoboMaxiMiner:
        case iminingRoboSuperMiner:
        case iminingRoboMidgetMiner:
        case iminingRoboUltraMiner:
            if (GetRaceGrbit(&rgplr[idPlayer], ibitRaceOBRM) != 0) {
                return -1;
            }
        default:
            if ((hs.iItem == iminingRoboMidgetMiner || hs.iItem == iminingRoboUltraMiner) && GetRaceGrbit(&rgplr[idPlayer], ibitRaceARM) == 0) {
                return -1;
            }
            if (hs.iItem == iminingOrbitalAdjuster && raMajor != 3) {
                return -1;
            }
            if (FShouldPartBeHidden(ppart) == 0)
                goto L_609c;
            return -1;
        }
    case hstScanner:
        if (hs.iItem >= iscannerCount) {
            return 0;
        }
        ppart->pscanner = &rgscanner[hs.iItem];
        if (idPlayer == -1)
            break;
        switch (hs.iItem) {
        case iscannerFerretScanner:
        case iscannerDolphinScanner:
        case iscannerElephantScanner:
            if (GetRaceGrbit(&rgplr[idPlayer], ibitRaceNoAdvScanner) != 0) {
                return -1;
            }
        default:
            switch (hs.iItem) {
            case iscannerChameleonScanner:
            case iscannerPickPocketScanner:
            case iscannerRobberBaronScanner:
                if (raMajor != 1) {
                    return -1;
                }
            default:
                goto L_609c;
            }
        }
    case hstBeam:
        if (hs.iItem >= ibeamCount) {
            return 0;
        }
        ppart->pbeam = &rgbeam[hs.iItem];
        if (hs.iItem == ibeamMiniGun && raMajor != 4) {
            return -1;
        }
        if ((hs.iItem == ibeamBlunderbuss || hs.iItem == ibeamGatlingNeutrinoCannon) && raMajor != 2) {
            return -1;
        }
        if (FShouldPartBeHidden(ppart) == 0)
            break;
        return -1;
    case hstTorp:
        if (hs.iItem >= itorpCount) {
            return 0;
        }
        ppart->ptorp = &rgtorp[hs.iItem];
        if (idPlayer == -1 || FShouldPartBeHidden(ppart) == 0)
            break;
        return -1;
    case hstBomb:
        if (hs.iItem >= ibombCount) {
            return 0;
        }
        ppart->pbomb = &rgbomb[hs.iItem];
        if (idPlayer == -1)
            break;
        if (hs.iItem >= ibombSmartBomb && hs.iItem <= ibombAnnihilatorBomb && raMajor == 4) {
            return -1;
        }
        if (hs.iItem == ibombRetroBomb && raMajor != 3) {
            return -1;
        }
        if (FShouldPartBeHidden(ppart) == 0)
            break;
        return -1;
    case hstPlanetary:
        if (hs.iItem >= iplanetaryCount) {
            return 0;
        }
        ppart->pplanetary = &rgplanetary[hs.iItem];
        if (idPlayer == -1)
            break;
        if (hs.iItem >= iplanetaryViewer50 && hs.iItem <= iplanetarySnooper620X && ppart->pplanetary->grAbility < 0 &&
            GetRaceGrbit(&rgplr[idPlayer], ibitRaceNoAdvScanner) != 0) {
            return -1;
        }
        if (hs.iItem >= iplanetaryViewer50 && hs.iItem <= iplanetarySnooper620X && raMajor == 8) {
            return -1;
        }
        if (hs.iItem >= iplanetarySDI && hs.iItem <= iplanetaryNeutronShield && raMajor == 8) {
            return -1;
        }
        if (hs.iItem >= iplanetaryLaserBattery && hs.iItem <= iplanetaryNeutronShield && raMajor == 2) {
            return -1;
        }
        if (FShouldPartBeHidden(ppart) == 0)
            break;
        return -1;
    case hstTerra:
        if (hs.iItem >= iterraCount) {
            return 0;
        }
        ppart->pterra = &rgterra[hs.iItem];
        if (idPlayer != -1 && hs.iItem >= iterraTotalTerraform3 && hs.iItem <= iterraTotalTerraform30 && GetRaceGrbit(&rgplr[idPlayer], ibitRaceTT) == 0) {
            return -1;
        }
    }
L_609c:
    return TechStatus(ppart->pcom->rgTech);
}

void LookupBestPlanetaryScanner(PART *ppart) {
    ppart->hs.iItem = 8;
    ppart->hs.grhst = hstPlanetary;
    while (ppart->hs.iItem >= 0 && FLookupPart(ppart) != 1 && ppart->hs.iItem != 0) {
        ppart->hs.iItem--;
    }
    return;
}

int16_t TechStatus(char *rgTech) {
    int16_t fInAWhile;
    int16_t i;
    int16_t fAlmost;
    int16_t cMiss;

    cMiss = 0;
    fAlmost = 0;
    fInAWhile = 0;
    for (i = 0; i < 6; i++) {
        if (rgplr[idPlayer].rgTech[i] < (int16_t)(int8_t)rgTech[i]) {
            cMiss++;
            if (i == (rgplr[idPlayer].iTechCur & 0xf)) {
                if (rgplr[idPlayer].rgTech[i] + 1 == (int16_t)(int8_t)rgTech[i]) {
                    fAlmost = 1;
                } else {
                    fInAWhile = i + 1;
                }
            }
        }
    }
    if (cMiss == 0) {
        return 1;
    }
    if (cMiss == 1 && fAlmost != 0) {
        return 2;
    }
    if (cMiss == 1 && fInAWhile != 0) {
        return (int16_t)(int8_t)rgTech[fInAWhile - 1] - rgplr[idPlayer].rgTech[fInAWhile - 1] + 1;
    }
    return 99;
}
