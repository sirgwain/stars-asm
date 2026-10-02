#include "common.h"

MacintiRecipeOffset vrgMacIshAip[31] = {macOffsetDestroyerBeamA,
                                        macOffsetDestroyerBeamB,
                                        macOffsetDestroyerBeamC,
                                        macOffsetDestroyerBeamD,
                                        macOffsetDestroyerTorpedoShield,
                                        macOffsetDestroyerTorpedoComputer,
                                        macOffsetDestroyerMissileJammer,
                                        macOffsetDestroyerMissileComputer,
                                        macOffsetB52SmartThenNormal,
                                        macOffsetB52RetroThenSmart,
                                        macOffsetFrigateScoutMineLayer,
                                        macOffsetBattleshipMissileBeam,
                                        macOffsetBattleshipMissileTorpedoA,
                                        macOffsetBattleshipMissileTorpedoB,
                                        macOffsetBattleshipTorpedoBeam,
                                        macOffsetBattleshipBeamA,
                                        macOffsetBattleshipBeamB,
                                        macOffsetBattleshipBeamC,
                                        macOffsetBattleshipBeamD,
                                        macOffsetBattleshipMcm,
                                        macOffsetOrbitalConstructionColonizer,
                                        macOffsetMaxiMiner,
                                        macOffsetUltraMinerOrMiner,
                                        macOffsetMiniMiner,
                                        macOffsetFreighter,
                                        macOffsetCruiserAntiMatterMcm,
                                        macOffsetCruiserMissile,
                                        macOffsetCruiserStreamingSapper,
                                        macOffsetCruiserTorpedo,
                                        macOffsetNubianMixedBeamsMcm,
                                        macOffsetNubianMixedBeams};
uint8_t             vrgMacAip[248] = {aiPartEnginePreferGalaxyScoop,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartArmorPreferMegaPolyShell,
                                      aiPartSpecialThrustDeflectorFuel,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartSpecialCapacitorFuel,
                                      aiPartArmorPreferMegaPolyShell,
                                      aiPartSpecialThrustDeflectorFuel,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartBeamPreferMultiContainedMunition,
                                      aiPartArmorPreferMegaPolyShell,
                                      aiPartSpecialThrustDeflectorFuel,
                                      aiPartSpecialCapacitorJammerPodComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferBlunderbuss,
                                      aiPartArmorPreferMegaPolyShell,
                                      aiPartSpecialThrustDeflectorFuel,
                                      aiPartSpecialCapacitorJammerPodComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartTorpedo,
                                      aiPartTorpedo,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartArmorPreferMegaPolyShell,
                                      aiPartSpecialThrustDeflectorFuel,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartTorpedo,
                                      aiPartTorpedo,
                                      aiPartBattleComputer,
                                      aiPartArmorPreferMegaPolyShell,
                                      aiPartSpecialThrustDeflectorFuel,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartMissile,
                                      aiPartMissile,
                                      aiPartBattleComputer,
                                      aiPartArmorPreferMegaPolyShell,
                                      aiPartSpecialThrustDeflectorFuel,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartMissile,
                                      aiPartMissile,
                                      aiPartBattleComputer,
                                      aiPartArmorPreferMegaPolyShell,
                                      aiPartSpecialThrustDeflectorFuel,
                                      aiPartBattleComputer,
                                      aiPartEngineGalaxyScoopOrHydroRamScoop,
                                      aiPartBombHushThenNormal,
                                      aiPartBombHushThenSmartThenNormalThenRetro,
                                      aiPartBombHushThenSmartThenNormalThenRetro,
                                      aiPartBombHushThenSmartThenNormalThenRetro,
                                      aiPartSpecialPodJammerDeflectorThrust,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartEngineGalaxyScoopOrHydroRamScoop,
                                      aiPartBombHushThenNormal,
                                      aiPartBombHushThenRetroThenSmart,
                                      aiPartBombHushThenRetroThenSmart,
                                      aiPartBombHushThenRetroThenSmart,
                                      aiPartSpecialPodJammerDeflectorThrust,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartEngineGalaxyScoopOrHydroRamScoop,
                                      aiPartScannerPreferElephant,
                                      aiPartStandardMineDispenser,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartBattleComputer,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartMissile,
                                      aiPartMissile,
                                      aiPartMissile,
                                      aiPartMissile,
                                      aiPartBeamPreferMultiContainedMunition,
                                      aiPartArmorPreferSuperlatanium,
                                      aiPartBattleComputer,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartSpecialPodThrustCloak,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartMissile,
                                      aiPartMissile,
                                      aiPartTorpedo,
                                      aiPartTorpedo,
                                      aiPartTorpedo,
                                      aiPartArmorPreferSuperlatanium,
                                      aiPartBattleComputer,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartSpecialPodThrustCloak,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartTorpedo,
                                      aiPartTorpedo,
                                      aiPartMissile,
                                      aiPartMissile,
                                      aiPartTorpedo,
                                      aiPartArmorPreferSuperlatanium,
                                      aiPartBattleComputer,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartSpecialPodThrustCloak,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartTorpedo,
                                      aiPartTorpedo,
                                      aiPartTorpedo,
                                      aiPartTorpedo,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartArmorPreferSuperlatanium,
                                      aiPartBattleComputer,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartSpecialPodThrustCloak,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartArmorPreferSuperlatanium,
                                      aiPartSpecialCapacitorJammerPodComputer,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartSpecialPodThrustCloak,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartSapper,
                                      aiPartBeamPreferMultiContainedMunition,
                                      aiPartArmorPreferSuperlatanium,
                                      aiPartSpecialCapacitorJammerPodComputer,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartSpecialPodThrustCloak,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartBeamPreferMultiContainedMunition,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartSapper,
                                      aiPartSapper,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartArmorPreferSuperlatanium,
                                      aiPartSpecialCapacitorJammerPodComputer,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartSpecialPodThrustCloak,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartBeamPreferBlunderbuss,
                                      aiPartArmorPreferSuperlatanium,
                                      aiPartSpecialCapacitorJammerPodComputer,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartSpecialCapacitorFuel,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartMultiContainedMunitionOnly,
                                      aiPartMultiContainedMunitionOnly,
                                      aiPartMultiContainedMunitionOnly,
                                      aiPartMultiContainedMunitionOnly,
                                      aiPartMultiContainedMunitionOnly,
                                      aiPartArmorPreferMegaPolyShell,
                                      aiPartSpecialCapacitorJammerPodComputer,
                                      aiPartSpecialJammerComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartOrbitalConstructionModuleOnly,
                                      aiPartEngineGalaxyScoopOrHydroRamScoop,
                                      aiPartMiningSupportPreferMegaPolyShell,
                                      aiPartMiningRobotMaxiOrBetter,
                                      aiPartMiningRobotMaxiOrBetter,
                                      aiPartMiningRobotMaxiOrBetter,
                                      aiPartMiningRobotMaxiOrBetter,
                                      aiPartEngineGalaxyScoopOrHydroRamScoop,
                                      aiPartMiningSupportPreferMegaPolyShell,
                                      aiPartMiningRobotUltraOrMidget,
                                      aiPartMiningRobotUltraOrMidget,
                                      aiPartMiningRobotUltraOrMidget,
                                      aiPartMiningRobotUltraOrMidget,
                                      aiPartEngineGalaxyScoopOrHydroRamScoop,
                                      aiPartMiningSupportPreferMegaPolyShell,
                                      aiPartMiningRobotMaxiOrBetter,
                                      aiPartMiningRobotMaxiOrBetter,
                                      aiPartEngineScoopOrFuelMizer,
                                      aiPartCargoPod,
                                      aiPartShieldPreferLangstonShell,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartSpecialPodThrustCloak,
                                      aiPartSpecialPodJammerDeflectorThrust,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartBeamPreferMultiContainedMunition,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartShieldPreferLangstonShell,
                                      aiPartSpecialPodJammerDeflectorThrust,
                                      aiPartMissile,
                                      aiPartMissile,
                                      aiPartBattleComputer,
                                      aiPartArmorPreferSuperlatanium,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartSpecialPodThrustCloak,
                                      aiPartSpecialPodJammerDeflectorThrust,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartSapper,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartBattleComputer,
                                      aiPartSpecialPodJammerDeflectorThrust,
                                      aiPartTorpedo,
                                      aiPartTorpedo,
                                      aiPartTorpedo,
                                      aiPartArmorPreferMegaPolyShell,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartArmorPreferSuperlatanium,
                                      aiPartArmorPreferSuperlatanium,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartBeamPreferAntiMatterPulverizer,
                                      aiPartMultiContainedMunitionOnly,
                                      aiPartMultiContainedMunitionOnly,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartArmorPreferMegaPolyShell,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartBeamPreferBigMuthaCannon,
                                      aiPartSapper,
                                      aiPartSpecialCapacitorJammerPodComputer,
                                      aiPartEnginePreferGalaxyScoop,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartShieldPreferCompletePhase,
                                      aiPartSapper,
                                      aiPartBeamPreferBlunderbuss,
                                      aiPartSpecialCapacitorJammerPodComputer,
                                      aiPartSpecialCapacitorJammerPodComputer,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartSpecialJammerComputer,
                                      aiPartBeamPreferStreamingPulverizer,
                                      aiPartBeamPreferMultiContainedMunition,
                                      aiPartBeamPreferAntiMatterPulverizer};
uint8_t             vrgAiMacintiResOrder[8] = {aiResearchEnergy3,      aiResearchPropulsion2, aiResearchEnergy20,       aiResearchConstruction17,
                                               aiResearchPropulsion20, aiResearchWeapons20,   aiResearchConstruction23, aiResearchWeapons23};

void DoMacintiAiTurn(PROD *rgprod) {
    int16_t  iLatestCargo;
    int16_t  cColFleet;
    int32_t  rgResCost[4];
    int16_t  idPlanDst;
    int16_t  j;
    FLEET   *lpflEnemy;
    int32_t  rgResAvail[4];
    PLANET  *lpplMac;
    int16_t  iLatestCruiser;
    int16_t  ishLastBattle;
    THING   *lpthWorm;
    int16_t  cFlMineLayers;
    int16_t  fShouldColonize;
    int16_t  cFlDestroyers;
    int16_t  cshDestroyer;
    int16_t  iAiLvl;
    int16_t  iLatestBattle;
    PLANET  *lppl;
    int16_t  cFlCargo;
    int16_t  iLatestBomber;
    int16_t  ifl;
    int16_t  i;
    FLEET   *lpfl;
    int16_t  cGenesis;
    int16_t  cFlArmadas;
    int16_t  cRes;
    int16_t  cFr;
    int16_t  iroCur;
    int16_t  iLatestMiner;
    int16_t  fUsingTempColonizer;
    int16_t  iLatestDestroyer;
    int16_t  ipl;
    int16_t  iLatestColony;
    uint16_t cRecyclePeriod;
    uint8_t  rgRecycleShdef[16];
    uint8_t *lpb;
    int16_t  cFlMineLayersBase;
    int16_t  cFlMiners;
    uint16_t rgCosts[4];
    FLEET   *lpflAttack;
    int16_t  iPlanet;
    int32_t  l;
    int16_t  fWrite;
    int16_t  fTonsOfMinerals;
    uint8_t  rgRecycleSBShdef[10];
    PROD    *lpprod;
    PART     part;
    SHDEF    shdef;
    PLANET  *lpplDest;
    int32_t  lLeast;
    PLANET  *lpplBest;
    int16_t  iplDest;
    int32_t  cMine;
    int16_t  id;
    int16_t  cConc;
    int16_t  iLatest;
    int16_t  dy;
    int32_t  lDist;
    int16_t  dx;
    ORDER    ord;
    PLANET  *lpplDrop;
    uint8_t  rgSplitShdef[16];

    iAiLvl = rgplr[idPlayer].lvlAi;
    iPlanet = rgplr[idPlayer].idPlanetHome;
    iroCur = IroEnsureAi((uint8_t *)vrgAiMacintiResOrder, 8, NULL, 15);
    if (game.turn < 40 || (rgshdef[7].fFree == 0 && rgshdef[7].hul.ihuldef == ihuldefColonyShip)) {
        fUsingTempColonizer = TRUE;
        if (FLookupPartX(&part, 1, 15) == 1 && rgshdef[7].cExist == 0) {
            shdef = rgshdef[7];
            shdef.fFree = TRUE;
            FChangeAiShdef(&shdef, 7);
            fUsingTempColonizer = FALSE;
        }
    } else {
        fUsingTempColonizer = FALSE;
    }
    ishLastBattle = 7 - fUsingTempColonizer;
    if (fUsingTempColonizer != 0) {
        MergeAllShdefs(892);
    } else {
        MergeAllShdefs(1020);
    }
    MergeAllShdefs(1);
    MergeAllShdefs(-16384);
    MergeAllShdefs(12288);
    j = 6;
    if (game.turn > 130) {
        j += (uint32_t)(game.turn - 120) / 20;
    }
    if (j > 50) {
        j = 50;
    }
    vrgAiArmadaPotency[0] = j;
    vrgAiArmadaPotency[1] = (int16_t)(j & 0xff) / 2;
    j = 6;
    if (game.turn > 115) {
        j += (uint32_t)(game.turn - 100) / 22;
    }
    if (j > 12) {
        j = 12;
    }
    vrgAiArmadaPotency[2] = j;
    vrgAiArmadaPotency[3] = 3 >= j / 2 - 1 ? j / 2 - 1 : 3;
    memset(rgRecycleShdef, 0, 16);
    if (game.turn < 120) {
        cRecyclePeriod = 50;
    } else {
        cRecyclePeriod = game.turn >= 200 ? 100 : 70;
    }
    cshDestroyer = CheckAiShdefStatus(12, 13, cRecyclePeriod, &iLatestDestroyer, rgRecycleShdef);
    for (i = 12; i <= 13; i++) {
        if (rgRecycleShdef[i] != 0 && rgshdef[i].fFree == 0 && rgshdef[i].hul.ihuldef == ihuldefNubian) {
            rgRecycleShdef[i] = 0;
        }
    }
    CheckAiShdefStatus(14, 15, 5000, &iLatestMiner, rgRecycleShdef);
    if (rgshdef[15].fFree == 0 && FLookupPartX(&part, 128, 6) == 1) {
        i = iLatestMiner == 14 ? 15 : 14;
        if (rgshdef[14].hul.rghs[2].iItem != 6) {
            i = 14;
        } else {
            if (rgshdef[15].hul.rghs[2].iItem == 6 || FLookupPartX(&part, 1, 15) != 1 || rgshdef[14].hul.rghs[0].iItem == 15)
                goto L_03c3;
            i = 15;
        }
        if (rgshdef[i].cExist == 0) {
            shdef = rgshdef[i];
            shdef.fFree = TRUE;
            FChangeAiShdef(&shdef, i);
        } else {
            rgRecycleShdef[14] = 3;
        }
    }
L_03c3:
    CheckAiShdefStatus(10, 11, cRecyclePeriod, &iLatestCargo, rgRecycleShdef);
    CheckAiShdefStatus(8, 9, cRecyclePeriod, &iLatestBomber, rgRecycleShdef);
    CheckAiShdefStatus(2, 4, cRecyclePeriod, &iLatestCruiser, rgRecycleShdef);
    CheckAiShdefStatus(5, ishLastBattle, cRecyclePeriod, &iLatestBattle, rgRecycleShdef);
    if (game.turn > 80) {
        SplitOutShdefs(rgRecycleShdef);
        memset(rgSplitShdef, 0, 16);
        rgSplitShdef[0] = 2;
        SplitOutShdefs(rgSplitShdef);
        memset(rgSplitShdef, 0, 16);
        rgSplitShdef[1] = 2;
        SplitOutShdefs(rgSplitShdef);
        memset(rgSplitShdef, 0, 16);
        rgSplitShdef[15] = 2;
        rgSplitShdef[14] = 2;
        SplitOutShdefs(rgSplitShdef);
        memset(rgSplitShdef, 0, 16);
        rgSplitShdef[11] = 2;
        rgSplitShdef[10] = 2;
        SplitOutShdefs(rgSplitShdef);
    }
    EnsureMacintiShdefs();
    EnsureMacintiStarbaseDesigns(rgRecycleSBShdef);
    vAiMacRecycleSB = rgRecycleSBShdef;
    fShouldColonize = FShouldWeBuildColonizers(&cColFleet);
    if (rgshdef[1].hul.rghs[0].iItem == 15) {
        iLatestColony = 1;
    } else if (rgshdef[7].fFree == 0 && rgshdef[7].hul.ihuldef == ihuldefColonyShip) {
        iLatestColony = 7;
    } else {
        iLatestColony = 1;
    }
    if (iLatestColony == 1 && fUsingTempColonizer != 0 && (uint16_t)(game.turn - rgshdef[1].turn) > 5) {
        rgRecycleShdef[7] = 2;
    }
    lpb = vlpbAiPlanet + 14;
    i = 0;
    while (i < game.cPlanMax) {
        *lpb = 0;
        i++;
        lpb += 16;
    }
    cFlMiners = 0;
    cFlCargo = 0;
    cFlDestroyers = 0;
    cFlArmadas = 0;
    cFlMineLayers = 0;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->iPlayer == idPlayer) {
            if (iLatestMiner != -1 && (lpfl->rgcsh[14] != 0 || lpfl->rgcsh[15] != 0)) {
                cFlMiners++;
            }
            if (lpfl->rgcsh[0] != 0) {
                cFlMineLayers++;
            }
            if (iLatestDestroyer != -1 && (lpfl->rgcsh[12] != 0 || lpfl->rgcsh[13] != 0)) {
                cFlDestroyers++;
            }
            if (iLatestCargo != -1 && (lpfl->rgcsh[10] != 0 || lpfl->rgcsh[11] != 0)) {
                cFlCargo++;
                if (lpfl->idPlanet == -1 && lpfl->cord > 1 && lpfl->lpplord->rgord[1].grobj == grobjPlanet && lpfl->rgwtMin[3] > 0) {
                    vlpbAiPlanet[lpfl->lpplord->rgord[1].id * 16 + 14] = vlpbAiPlanet[lpfl->lpplord->rgord[1].id * 16 + 0xe] | 1;
                }
            }
            for (i = 2; i <= 9; i++) {
                if (lpfl->rgcsh[i] != 0) {
                    cFlArmadas++;
                    break;
                }
            }
        }
    }
    cFlMineLayersBase = cFlMineLayers;
    lpb = vlpbAiPlanet + 13;
    i = 0;
    while (i < game.cPlanMax) {
        *lpb = 0;
        i++;
        lpb += 16;
    }
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0)
            break;
        vlpbAiPlanet[lppl->id * 16 + 13] = 1;
    }
    if (game.turn > 120 && FLookupPartX(&part, 0x8000, 13) == 1) {
        cGenesis = rgplr[idPlayer].cPlanet / 20;
        if (cGenesis > 10) {
            cGenesis = 10;
        }
    } else {
        cGenesis = 0;
    }
    UpdateProgressGauge(progressStep4);
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer != idPlayer && lppl->iPlayer != -1) {
            i = lppl->uPopGuess / 250 + 1;
            if (i > 6) {
                i = 6;
            }
            if (lppl->fStarbase != 0) {
                i++;
            }
            vlpbAiPlanet[lppl->id * 16 + 10] = i;
            vlpbAiPlanet[lppl->id * 16 + 9] = 1;
        }
    }
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0)
            break;
        if (lppl->fStarbase != 0 && lppl->rgwtMin[3] >= 200 && lppl->isb != 0 && (lppl->isb != 1 || game.turn <= 25)) {
            ChangeMainObjSel(grobjPlanet, lppl->id);
            if (lppl->lpplprod == 0 || lppl->lpplprod->iprodMac < 24) {
                InitProduction(rgprod);
                fWrite = FALSE;
                i = 0;
                for (lpprod = lpplProdGlob->rgprod; i < lpplProdGlob->iprodMac && (lpprod->grobj != grobjFleet || lpprod->iItem >= iobjPacketGerm); lpprod++) {
                    i++;
                }
                if (i < lpplProdGlob->iprodMac) {
                    FinishProduction(FALSE);
                } else {
                    cRes = CResourcesAtPlanet(lppl, idPlayer);
                    if (game.turn > 120 && IWarpMAFromLppl(lppl, &j) >= 10 && lppl->rgwtMin[3] > 10000 && Random(4) == 0) {
                        lpplBest = NULL;
                        lLeast = 100000;
                        i = 0;
                        lpprod = lpplProdGlob->rgprod;
                        while (i < lpplProdGlob->iprodMac) {
                            if (lpprod->grobj == grobjPlanet && lpprod->iItem >= iobjPacketIron && lpprod->iItem <= iobjPacketMixed)
                                goto LTryCargo;
                            i++;
                            lpprod++;
                        }
                        j = Random(3);
                        for (i = j; i < j + 3 && lppl->rgwtMin[i % 3] <= 5000; i++) {
                        }
                        if (i != j + 3) {
                            i %= 3;
                            for (iplDest = 0; iplDest < vclpplAi; iplDest++) {
                                lpplDest = vrglpplAi[iplDest];
                                if (vrglpplAi[iplDest] == 0)
                                    break;
                                if (lpplDest->rgwtMin[i] < lLeast && IWarpMAFromLppl(lpplDest, &j) >= 10) {
                                    l = LDistance2(rgptPlan[lppl->id], rgptPlan[lpplDest->id]);
                                    if (l < 91204) {
                                        lLeast = lpplDest->rgwtMin[i];
                                        lpplBest = lpplDest;
                                    }
                                }
                            }
                            if (lpplBest != 0 && lLeast < (int32_t)(lppl->rgwtMin[i] / 5)) {
                                l = (int32_t)(lppl->rgwtMin[i] / 5);
                                if (l > 20000) {
                                    l = 20000;
                                }
                                l = (int32_t)(l / 100);
                                AddItemToQueue(i + 14, LOWORD(l), grobjPlanet, addItemEnd);
                                FinishProduction(TRUE);
                                sel.pl.iWarpFling = 7;
                                sel.pl.idFling = lpplBest->id + 1;
                                FLookupPlanet(-1, &sel.pl);
                                continue;
                            }
                        }
                    }
                LTryCargo:
                    if (iLatestCargo != -1 && cFlCargo < 64 && cFlCargo < rgplr[idPlayer].cPlanet / 4 && Random(3) == 0) {
                        cFlCargo++;
                        AddItemToQueue(iLatestCargo, 1, grobjFleet, addItemEnd);
                        fWrite = TRUE;
                    }
                    if (((fShouldColonize != 0 && (cColFleet <= 40 || (game.turn <= 120 && cColFleet <= 100))) || Random(100) < 8) &&
                        (cColFleet < 50 || game.turn <= 120)) {
                        for (i = 0; i <= 2; i++) {
                            if (lppl->rgwtMin[i] < 30)
                                goto FinishProd;
                        }
                        if (FShouldPlanetBuildColonizer(lppl) != 0) {
                            cColFleet++;
                            AddItemToQueue(iLatestColony, 1, grobjFleet, addItemEnd);
                            fWrite = TRUE;
                            if (game.turn < 5)
                                goto FinishProd;
                            l = (uint32_t)(lppl->rgwtMin[3] * PctTrueMaxGrowth(idPlayer));
                            if (l > 2300 && cRes > 35 && iAiLvl > 0) {
                                cColFleet++;
                                AddItemToQueue(iLatestColony, 1, grobjFleet, addItemEnd);
                                if (l > 3600 && cRes > 50 && iAiLvl > 1) {
                                    AddItemToQueue(iLatestColony, 1, grobjFleet, addItemEnd);
                                }
                            }
                        }
                    }
                    if (iLatestMiner != -1 && cFlMiners < 60 && rgshdef[iLatestMiner].cExist < 5000 && Random(2) == 0) {
                        id = lppl->id;
                        cMine = 0;
                        for (ifl = 0; ifl < cFleet; ifl++) {
                            lpfl = rglpfl[ifl];
                            if (rglpfl[ifl] == 0)
                                break;
                            if (lpfl->idPlanet == id && lpfl->rgcsh[iLatestMiner] > 0 && lpfl->iPlayer == idPlayer) {
                                cMine = CMineFromLpfl(lpfl);
                                break;
                            }
                        }
                        if (cMine <= 1000) {
                            cFr = 0;
                            for (i = 0; i < 3; i++) {
                                cFr += lppl->rgMinConc[i];
                            }
                            cFr = 3 * cFr;
                            if ((cMine < cFr && cFr > 150) || (cFlMiners < 30 && Random(10) != 0)) {
                                cFlMiners++;
                                AddItemToQueue(iLatestMiner, 1, grobjFleet, addItemEnd);
                                fWrite = TRUE;
                            }
                        }
                    }
                    if (rgshdef[0].hul.ihuldef == ihuldefFrigate && cFlMineLayers < 60 &&
                        (iLatestMiner != -1 ? rgshdef[iLatestMiner].cExist
                                            : (uint32_t)((uint32_t)(uint16_t)vtimer.mdForce | (uint32_t)(uint16_t)vtimer.fAutoGenWhenIn << 16)) < 7500 &&
                        Random(4) == 0) {
                        id = lppl->id;
                        cFr = 0;
                        for (ifl = 0; ifl < cFleet; ifl++) {
                            lpfl = rglpfl[ifl];
                            if (rglpfl[ifl] == 0)
                                break;
                            if (lpfl->idPlanet == id && lpfl->rgcsh[0] > 0 && lpfl->iPlayer == idPlayer) {
                                cFr = lpfl->rgcsh[0];
                                break;
                            }
                        }
                        if ((cFr < 10 || (cFr < 17 && Random(10) == 0)) && Random(cFr * 2 + 1) == 0) {
                            cFlMineLayers += 3;
                            AddItemToQueue(0, 4, grobjFleet, addItemEnd);
                            fWrite = TRUE;
                        }
                    }
                    for (i = 0; i <= 2 && lppl->rgwtMin[i] >= 5000; i++) {
                    }
                    fTonsOfMinerals = i == 2;
                    if (iLatestBomber != -1 && cFlArmadas < 140 && (cFlArmadas < 60 || cRes > 2000)) {
                        id = lppl->id;
                        if (cFlArmadas <= 110 || Random(3) != 0) {
                            ifl = 0;
                            while (1) {
                                if (ifl >= cFleet)
                                    goto L_15e8;
                                lpfl = rglpfl[ifl];
                                if (rglpfl[ifl] == 0)
                                    goto L_15e8;
                                if (lpfl->idPlanet == id && lpfl->iPlayer == idPlayer && FPotentMacWarFleet(lpfl, NULL) != 0)
                                    break;
                                ifl++;
                            }
                            if (iLatestBomber == -1 || lpfl->rgcsh[8] + lpfl->rgcsh[9] >= vrgAiArmadaPotency[2])
                                goto L_15e8;
                        }
                        cFlArmadas += 2;
                        AddItemToQueue(iLatestBomber, fTonsOfMinerals == 0 ? 4 : 12, grobjFleet, addItemEnd);
                        fWrite = TRUE;
                        goto FinishProd;
                    }
                L_15e8:
                    if (cGenesis > 0 && lppl->rgwtMin[3] > 10000) {
                        i = 0;
                        for (lpprod = lpplProdGlob->rgprod; i < lpplProdGlob->iprodMac && (lpprod->grobj != grobjPlanet || lpprod->iItem != iobjGenesis);
                             lpprod++) {
                            i++;
                        }
                        if (i >= lpplProdGlob->iprodMac) {
                            for (i = 0; i <= 2 && lppl->rgwtMin[i] >= 2000; i++) {
                            }
                            if (i != 2) {
                                cConc = 0;
                                for (i = 0; i < 3; i++) {
                                    cConc += lppl->rgMinConc[i];
                                }
                                if (cConc < 15 || (cConc < 30 && Random(3) != 0) || (cConc < 60 && Random(5) != 0)) {
                                    cGenesis--;
                                    AddItemToQueue(iobjGenesis, 1, grobjPlanet, addItemEnd);
                                    AddItemToQueue(mdIdleTerraform, 75, grobjPlanet, addItemEnd);
                                    fWrite = TRUE;
                                }
                            }
                        }
                    }
                    if (iLatestCruiser != -1 && cFlArmadas < 130 && game.turn > 20 && (cFlArmadas < 50 || cRes > 2000) &&
                        (fTonsOfMinerals != 0 || Random(3) == 0)) {
                        if (iLatestBattle != -1 && Random(3) == 0) {
                            iLatest = iLatestBattle;
                        } else {
                            iLatest = iLatestCruiser;
                        }
                        cFlArmadas++;
                        AddItemToQueue(iLatest, fTonsOfMinerals == 0 ? 2 : 10, grobjFleet, addItemEnd);
                        fWrite = TRUE;
                        if (fTonsOfMinerals != 0)
                            goto FinishProd;
                    }
                    if (iLatestDestroyer != -1 && cFlDestroyers < (game.turn >= 120 ? 60 : 80) && cshDestroyer < 2000) {
                        GetResourcesAvailable(lppl, rgResAvail);
                        GetProdQCost(lppl, rgResCost);
                        for (i = 0; i < 4; i++) {
                            rgResAvail[i] -= rgResCost[i];
                            if (rgResAvail[i] < 0)
                                goto FinishProd;
                        }
                        GetTrueHullCost(idPlayer, &rgshdef[iLatestDestroyer].hul, rgCosts);
                        for (i = 0; i < 20; i++) {
                            for (j = 0; j < 4; j++) {
                                rgResAvail[j] -= (uint32_t)rgCosts[j];
                                if (rgResAvail[j] < 0)
                                    break;
                            }
                            if (j < 4)
                                break;
                        }
                        if (i > 0) {
                            fWrite = TRUE;
                            AddItemToQueue(iLatestDestroyer, i, grobjFleet, addItemEnd);
                            cFlDestroyers++;
                        }
                    }
                FinishProd:
                    FinishProduction(fWrite);
                }
            }
        }
    }
    UpdateProgressGauge(progressStep4);
    lpflAttack = NULL;
    lpflEnemy = NULL;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->cord > 1 && lpfl->lpplord->rgord[1].grobj == grobjThing) {
            dx = lpfl->pt.x - lpfl->lpplord->rgord[1].pt.x;
            dy = lpfl->pt.y - lpfl->lpplord->rgord[1].pt.y;
            lDist = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
            if (lDist > 40000) {
                ChangeMainObjSel(grobjFleet, lpfl->id);
                sel.fl.cord = 1;
                sel.fl.lpplord->iordMac = 1;
                FLookupFleet(-1, &sel.fl);
            }
        }
        if (lpfl->iPlayer != idPlayer) {
            lpfl->lpflNext = lpflEnemy;
            lpflEnemy = lpfl;
        } else {
            if (lpfl->rgcsh[0] > 0 && game.turn > 40) {
                if (lpfl->cord > 1) {
                    if (lpfl->lpplord->rgord[0].grTask == grTaskNone)
                        continue;
                    ClearAiCurrentTask(lpfl, TRUE);
                    continue;
                }
                if ((cFlMineLayersBase > 55 || (cFlMineLayersBase > 40 && Random(3) != 0)) && FFindBuddyAndJoinUp(lpfl, 0, 0, 72, 108) != 0)
                    continue;
                if (lpfl->rgcsh[0] >= 7 && Random(5) == 0) {
                    idPlanDst = IdRandomPlanetNearby(lpfl->pt, 105, TRUE);
                    if (idPlanDst != -1 && idPlanDst != lpfl->idPlanet) {
                        ClearAiCurrentTask(lpfl, TRUE);
                        ord.id = idPlanDst;
                        ord.grobj = grobjPlanet;
                        ord.pt = rgptPlan[idPlanDst];
                        ord.grTask = grTaskLayMines;
                        ord.fValidTask = TRUE;
                        ord.iWarp = 4;
                        FMoveAiFleet(lpfl, &ord, FALSE);
                        goto L_2154;
                    }
                }
                if (lpfl->lpplord->rgord[0].grTask != grTaskLayMines) {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    sel.fl.lpplord->rgord[0].grTask = grTaskLayMines;
                    sel.fl.lpplord->rgord[0].tlm.cTime = 5;
                    sel.fl.lpplord->rgord[0].tlm.cTimeOld = 5;
                    FLookupFleet(-1, &sel.fl);
                    continue;
                }
            } else if ((lpfl->rgcsh[14] > 0 || lpfl->rgcsh[15] > 0) && lpfl->cord == 1) {
                if (lpfl->lpplord->rgord[0].grTask != grTaskMine) {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    sel.fl.lpplord->rgord[0].grTask = grTaskMine;
                    FLookupFleet(-1, &sel.fl);
                    continue;
                }
                if ((cFlMiners > 58 || (cFlMiners > 48 && Random(3) != 0)) && FFindBuddyAndJoinUp(lpfl, 14, 15, 72, 108) != 0)
                    continue;
                if (lpfl->idPlanet != -1) {
                    lppl = LpplFromId(lpfl->idPlanet);
                    if (lppl != 0 && lppl->iPlayer == idPlayer) {
                        j = 0;
                        for (i = 0; i < 3; i++) {
                            j += lppl->rgMinConc[i];
                        }
                        if (j < 30 || (j < 60 && Random(3) == 0)) {
                            FRetargetMiner(lpfl);
                            continue;
                        }
                    }
                }
                if (Random(10) == 0) {
                    FRetargetMiner(lpfl);
                }
            } else if (FIsAiAttack(lpfl) != 0) {
                lpfl->lpflNext = lpflAttack;
                lpflAttack = lpfl;
                for (j = 2; j <= ishLastBattle && lpfl->rgcsh[j] <= 0; j++) {
                }
                if (j <= ishLastBattle && ((lpfl->cord > 1 && lpfl->lpplord->rgord[1].grobj == grobjPlanet) || lpfl->idPlanet != -1)) {
                    if (lpfl->cord > 1 && lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                        id = lpfl->lpplord->rgord[1].id;
                    } else {
                        id = lpfl->idPlanet;
                    }
                    lpb = vlpbAiPlanet + (10 + 16 * id);
                    if (*lpb != 0) {
                        *lpb |= 0x80;
                    }
                }
            } else if (FIsAiTransport(lpfl) != 0) {
                idPlanDst = -1;
                if (lpfl->cord <= 1 || lpfl->lpplord->rgord[0].grTask != grTaskNone) {
                    idPlanDst = lpfl->idPlanet;
                } else if (lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
                if (idPlanDst != -1) {
                    lppl = LpplFromId(idPlanDst);
                    if ((lppl == 0 || lppl->iPlayer != idPlayer) && lpfl->rgwtMin[3] == 0) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        sel.fl.cord = 1;
                        sel.fl.lpplord->iordMac = 1;
                        FLookupFleet(-1, &sel.fl);
                        ClearAiCurrentTask(lpfl, FALSE);
                    }
                }
            }
        L_2154:
            if (game.turn > 10 || (lpfl->rgcsh[0] <= 0 && lpfl->rgcsh[2] == 0)) {
                if (lpfl->cord > 1) {
                    if (fUsingTempColonizer == 0 || rgRecycleShdef[7] == 0 || lpfl->rgcsh[7] == 0)
                        continue;
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    sel.fl.cord = 1;
                    sel.fl.lpplord->iordMac = 1;
                    FLookupFleet(-1, &sel.fl);
                    FMoveToNearestStarbase(lpfl, FALSE);
                    continue;
                }
                if (lpfl->rgcsh[1] == 0 && (fUsingTempColonizer == 0 || lpfl->rgcsh[7] == 0))
                    continue;
                if (fUsingTempColonizer == 0 || iLatestColony != 1 || lpfl->idPlanet == -1) {
                    if (iAiLvl > 1 && lpfl->idPlanet != -1) {
                        lpplDrop = LpplFromId(lpfl->idPlanet);
                        if (lpplDrop != 0 && lpplDrop->iPlayer != -1 && lpplDrop->iPlayer != idPlayer && lpplDrop->fStarbase == 0 && lpplDrop->uPopGuess < 50)
                            continue;
                        if (lpplDrop != 0 && lpplDrop->iPlayer == -1) {
                            if (lpfl->rgwtMin[3] == 0) {
                                FMoveToNearestStarbase(lpfl, TRUE);
                                continue;
                            }
                            ChangeMainObjSel(grobjFleet, lpfl->id);
                            sel.fl.lpplord->rgord[0].grTask = grTaskColonize;
                            FLookupFleet(-1, &sel.fl);
                            continue;
                        }
                    }
                    idPlanDst = IdNearestColonizablePlanet(lpfl, &lpthWorm);
                    if (idPlanDst == -1 && lpthWorm == 0) {
                        if (lpfl->idPlanet == -1)
                            continue;
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
                        FLookupFleet(-1, &sel.fl);
                        continue;
                    }
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    if (lpfl->idPlanet != -1) {
                        lppl = LpplFromId(lpfl->idPlanet);
                        if (lppl != 0) {
                            l = (int32_t)(lppl->rgwtMin[3] / 10);
                            if (l > 25) {
                                l = 25;
                            }
                        } else {
                            l = 0;
                        }
                        XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, Colonists, LOWORD(l));
                        FLookupFleet(lpfl->id, &sel.fl);
                    }
                    if (idPlanDst != -1) {
                        FColonizeAiFleet(lpfl, idPlanDst);
                        continue;
                    }
                    FGotoWormholeAiFleet(lpfl, lpthWorm);
                    continue;
                }
            }
            ChangeMainObjSel(grobjFleet, lpfl->id);
            sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
            FLookupFleet(-1, &sel.fl);
        }
    }
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->iPlayer == idPlayer && lpfl->cord <= 1 && FIsAiTransport(lpfl) != 0) {
            if (lpfl->iplan != 4) {
                ChangeMainObjSel(grobjFleet, lpfl->id);
                sel.fl.iplan = 4;
                FLookupFleet(-1, &sel.fl);
            }
            IdTargetMacFreighter(lpfl);
        }
    }
    UpdateProgressGauge(progressStep4);
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->iPlayer == idPlayer) {
            for (i = 0; i < 16 && (lpfl->rgcsh[i] <= 0 || rgRecycleShdef[i] != 0); i++) {
            }
            if (i == 16) {
                if (lpfl->idPlanet != -1) {
                    lppl = LpplFromId(lpfl->idPlanet);
                    if (lppl != 0 && lppl->iPlayer == idPlayer && (lppl->fStarbase != 0 || Random(5) == 0)) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
                        FLookupFleet(-1, &sel.fl);
                        continue;
                    }
                }
                if ((lpfl->cord > 1 && lpfl->idPlanet == -1 && lpfl->lpplord->rgord[1].grobj == grobjPlanet) || FMoveToNearestStarbase(lpfl, FALSE) != 0)
                    continue;
            }
            for (i = 2; i <= 9; i++) {
                if (lpfl->rgcsh[i] > 0 && (i != 7 || fUsingTempColonizer == 0)) {
                    if (cFlArmadas > 100 || (cFlArmadas > 90 && Random(3) == 0)) {
                        l = 0;
                        for (j = 2; j <= 9; j++) {
                            l += lpfl->rgcsh[j];
                        }
                        if ((Random(100) > l - 10 || Random(20) == 0) && FFindBuddyAndJoinUp(lpfl, 2, 9, 100, 200) != 0)
                            break;
                    }
                    TargetMacArmada(lpfl);
                    break;
                }
            }
            if (i > 9 && FIsAiAttack(lpfl) != 0 && (lpfl->cord <= 1 || lpfl->lpplord->rgord[1].grobj != grobjFleet) &&
                ((cFlDestroyers <= (game.turn <= 120 ? 70 : 50) && (cFlDestroyers <= (game.turn <= 120 ? 60 : 40) || Random(3) != 0)) ||
                 (((iLatestDestroyer != -1 ? lpfl->rgcsh[iLatestDestroyer] : lpfl->pt.y) >= 20 && Random(20) != 0) ||
                  FFindBuddyAndJoinUp(lpfl, 12, 13, 36, 72) == 0))) {
                IdTargetAttack(lpfl, lpflAttack, lpflEnemy, game.fAisBand);
            }
        }
    }
    HandleBasicAiTasks(iroCur, rgprod, 0, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

void EnsureMacintiShdefs() {
    int16_t ish;
    int16_t i;
    PART    part;
    int16_t fAdvanced;
    SHDEF   shdef;
    int16_t shBase;

    for (ish = 14; ish <= 15; ish++) {
        if (rgshdef[ish].fFree != 0) {
            fAdvanced = rgplr[idPlayer].lvlAi >= lvlAiTough;
            if ((fAdvanced == 0 || ish != 15 || rgplr[idPlayer].rgTech[3] >= 15) &&
                FCreateAiShdef(ish, 24 - (fAdvanced == 0), (uint8_t *)&vrgMacAip[vrgMacIshAip[fAdvanced + 0x15]]) == 0 && ish == 14) {
                if (fAdvanced != 0) {
                    FCreateAiShdef(ish, ihuldefMiner, (uint8_t *)&vrgMacAip[vrgMacIshAip[22]]);
                } else {
                    FCreateAiShdef(ish, ihuldefMiniMiner, (uint8_t *)&vrgMacAip[vrgMacIshAip[23]]);
                }
            }
        }
    }
    if (rgshdef[12].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 5 && rgplr[idPlayer].rgTech[2] >= 6 &&
        FCreateAiShdef(12, ihuldefNubian, (uint8_t *)&vrgMacAip[vrgMacIshAip[30]]) == 0) {
        for (i = 0; i < 5 && FCreateAiShdef(12, ihuldefDestroyer, (uint8_t *)&vrgMacAip[vrgMacIshAip[Random(4)]]) == 0; i++) {
        }
    }
    if (rgshdef[13].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 10 && rgplr[idPlayer].rgTech[2] >= 9 &&
        FCreateAiShdef(13, ihuldefNubian, (uint8_t *)&vrgMacAip[vrgMacIshAip[30]]) == 0) {
        for (i = 0; i < 5 && FCreateAiShdef(13, ihuldefDestroyer, (uint8_t *)&vrgMacAip[vrgMacIshAip[Random(4) + 4]]) == 0; i++) {
        }
    }
    if (rgshdef[10].fFree != 0 && FCreateAiShdef(10, ihuldefLargeFreighter, (uint8_t *)&vrgMacAip[vrgMacIshAip[24]]) == 0) {
        FCreateAiShdef(10, ihuldefMediumFreighter, (uint8_t *)&vrgMacAip[vrgMacIshAip[24]]);
    }
    if (rgshdef[11].fFree != 0) {
        FCreateAiShdef(11, ihuldefLargeFreighter, (uint8_t *)&vrgMacAip[vrgMacIshAip[24]]);
    }
    if (game.turn < 20 && rgshdef[2].fFree == 0 && rgshdef[2].cExist == 0) {
        shdef = rgshdef[2];
        shdef.fFree = TRUE;
        FChangeAiShdef(&shdef, 2);
    }
    for (ish = 2; ish <= 4; ish++) {
        if (rgshdef[ish].fFree != 0 && (ish == 2 || (uint16_t)(game.turn - rgshdef[ish - 1].turn) > 20)) {
            for (i = 0; i < 5 && FCreateAiShdef(ish, ihuldefCruiser, (uint8_t *)&vrgMacAip[vrgMacIshAip[Random(4) + 0x19]]) == 0; i++) {
            }
        }
    }
    if (game.turn < 40 && rgshdef[7].fFree != 0) {
        FCreateAiShdef(7, ihuldefColonyShip, (uint8_t *)&vrgMacAip[vrgMacIshAip[20]]);
    }
    if (FLookupPartX(&part, 1, 15) == 1 && rgshdef[1].fFree == 0 && rgshdef[1].cExist == 0 && rgshdef[1].hul.rghs[0].iItem != 15) {
        shdef = rgshdef[1];
        shdef.fFree = TRUE;
        FChangeAiShdef(&shdef, 1);
        FCreateAiShdef(1, ihuldefColonyShip, (uint8_t *)&vrgMacAip[vrgMacIshAip[20]]);
    }
    for (ish = 5; ish <= 7; ish++) {
        if (rgshdef[ish].fFree != 0 && (ish == 5 || (rgshdef[ish - 1].fFree == 0 && (uint16_t)(game.turn - rgshdef[ish - 1].turn) > 20))) {
            shBase = ish == 5 ? 11 : 15;
            if (ish == 7) {
                shBase = Random(2) == 0 ? 15 : 11;
            }
            if (Random(3) == 0 || FCreateAiShdef(ish, ihuldefNubian, (uint8_t *)&vrgMacAip[vrgMacIshAip[29]]) == 0) {
                for (i = 0; i < 5 && FCreateAiShdef(ish, ihuldefBattleship, (uint8_t *)&vrgMacAip[vrgMacIshAip[Random(4) + shBase]]) == 0; i++) {
                }
            }
        }
    }
    for (ish = 8; ish <= 9; ish++) {
        if (rgshdef[ish].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 14 &&
            ((ish == 8 || (rgshdef[ish - 1].fFree == 0 && (uint16_t)(game.turn - rgshdef[ish - 1].turn) > 15)) &&
             FCreateAiShdef(ish, ihuldefBattleship, (uint8_t *)&vrgMacAip[vrgMacIshAip[19]]) == 0)) {
            FCreateAiShdef(ish, ihuldefB52Bomber, (uint8_t *)&vrgMacAip[vrgMacIshAip[ish == 8 ? 8 : 9]]);
        }
    }
    if (rgshdef[0].hul.ihuldef != ihuldefFrigate && rgplr[idPlayer].lvlAi > lvlAiStandard && rgshdef[0].cExist == 0 && rgplr[idPlayer].rgTech[5] >= 4 &&
        rgplr[idPlayer].rgTech[4] >= 5 && rgplr[idPlayer].rgTech[3] >= 6 && rgplr[idPlayer].rgTech[2] >= 6 && rgplr[idPlayer].rgTech[0] >= 6) {
        shdef = rgshdef[0];
        shdef.fFree = TRUE;
        FChangeAiShdef(&shdef, 0);
        FCreateAiShdef(0, ihuldefFrigate, (uint8_t *)&vrgMacAip[vrgMacIshAip[10]]);
    }
    return;
}

int16_t FRetargetMiner(FLEET *lpfl) {
    int16_t cConc;
    ORDER   ord;
    int16_t cConcBest;
    PLANET *lppl;
    int16_t cConcCur;
    int16_t ipl;
    PLANET *lpplBest;

    lpplBest = NULL;
    cConcCur = 0;
    cConcBest = 0;
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0)
            break;
        if (LDistance2(lpfl->pt, rgptPlan[lppl->id]) < 5184) {
            cConc = lppl->rgMinConc[0] * 8 + lppl->rgMinConc[1] * 10 + lppl->rgMinConc[2] * 7;
            if (lppl->id == lpfl->idPlanet) {
                cConcCur = cConc;
            }
            if (cConc > cConcBest) {
                lpplBest = lppl;
                cConcBest = cConc;
            }
        }
    }
    if (lpplBest == 0 || (int16_t)(6 * cConcCur) / 5 >= cConcBest) {
        return FALSE;
    }
    ord.id = lpplBest->id;
    ord.grobj = grobjPlanet;
    ord.pt = rgptPlan[lpplBest->id];
    ord.grTask = grTaskNone;
    ord.fValidTask = TRUE;
    ord.iWarp = 6;
    return FMoveAiFleet(lpfl, &ord, FALSE);
}

int16_t IdTargetMacFreighter(FLEET *lpfl) {
    int32_t cMax;
    int32_t cColLeft;
    int16_t cResGainMost;
    int32_t cColHaul;
    int16_t cResGain;
    ORDER   ord;
    PLANET *lpplHere;
    int16_t pctCapMost;
    int16_t pctCapHere;
    int16_t cResLost;
    PLANET *lppl;
    int16_t pctKilled;
    int16_t i;
    int32_t lDist;
    int16_t ipl;
    PLANET *lpplBest;
    int16_t iM;

    if (lpfl->idPlanet == -1) {
        lpplHere = NULL;
    } else {
        lpplHere = LpplFromId(lpfl->idPlanet);
        if (lpplHere != 0 && lpplHere->iPlayer == idPlayer) {
            pctCapHere = PctPlanetCapacity(lpplHere);
            if (pctCapHere > 25 && lpplHere->rgwtMin[3] >= 1000) {
                cColHaul = LGetFleetStat(lpfl, 2);
                for (i = 0; i <= 2; i++) {
                    cColHaul -= lpfl->rgwtMin[i];
                }
                cMax = (int32_t)(lpplHere->rgwtMin[3] / 20);
                if (cColHaul > cMax) {
                    cColHaul = cMax;
                }
                if (cColHaul > 0) {
                    cResLost = CResourcesAtPlanet(lpplHere, idPlayer);
                    lpplHere->rgwtMin[3] -= cColHaul;
                    cResLost -= CResourcesAtPlanet(lpplHere, idPlayer);
                    lpplHere->rgwtMin[3] += cColHaul;
                    lpplBest = NULL;
                    cResGainMost = 0;
                    for (ipl = 0; ipl < vclpplAi; ipl++) {
                        lppl = vrglpplAi[ipl];
                        if (vrglpplAi[ipl] == 0)
                            break;
                        if (lppl != lpplHere && (vlpbAiPlanet[lppl->id * 16 + 0xe] & 1) == 0) {
                            lDist = LDistance2(lpfl->pt, rgptPlan[lppl->id]);
                            if (lDist <= 40000) {
                                if (lDist > 22500) {
                                    pctKilled = 12;
                                } else if (lDist > 10000) {
                                    pctKilled = 9;
                                } else if (lDist > 2500) {
                                    pctKilled = 6;
                                } else {
                                    pctKilled = 3;
                                }
                                cColLeft = cColHaul - (int32_t)(cColHaul * pctKilled) / 100;
                                lppl->rgwtMin[3] += cColLeft;
                                cResGain = CResourcesAtPlanet(lppl, idPlayer);
                                lppl->rgwtMin[3] -= cColLeft;
                                cResGain -= CResourcesAtPlanet(lppl, idPlayer);
                                if (cResGain > cResGainMost) {
                                    cResGainMost = cResGain;
                                    lpplBest = lppl;
                                }
                            }
                        }
                    }
                    if (lpplBest != 0 && cResGainMost >= cResLost + 5 && (cResGainMost >= cResLost + 10 || game.turn <= 80) &&
                        (cResGainMost >= cResLost + 15 || game.turn <= 160)) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, Colonists, LOWORD(cColHaul));
                        FLookupFleet(lpfl->id, &sel.fl);
                        vlpbAiPlanet[lpplBest->id * 16 + 14] = vlpbAiPlanet[lpplBest->id * 16 + 0xe] | 1;
                        goto LMoveToLpplBest;
                    }
                }
            }
        }
    }
    if (lpplHere != 0) {
        for (iM = 0; iM <= 2 && lpplHere->rgwtMin[iM] < 2500; iM++) {
        }
        if (iM <= 2) {
            lpplBest = NULL;
            cMax = 1000;
            for (ipl = 0; ipl < vclpplAi; ipl++) {
                lppl = vrglpplAi[ipl];
                if (vrglpplAi[ipl] == 0)
                    break;
                if (lppl != lpplHere) {
                    lDist = LDistance2(lpfl->pt, rgptPlan[lppl->id]);
                    if (lDist <= 40000 && lppl->rgwtMin[iM] < cMax) {
                        cMax = lppl->rgwtMin[iM];
                        lpplBest = lppl;
                    }
                }
            }
            if (lpplBest != 0 && cMax < 200) {
                cColHaul = LGetFleetStat(lpfl, 2);
                for (i = 0; i <= 2; i++) {
                    cColHaul -= lpfl->rgwtMin[i];
                }
                cMax = (int32_t)(lpplHere->rgwtMin[iM] / 5);
                if (cColHaul > cMax) {
                    cColHaul = cMax;
                }
                if (cColHaul > 0) {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                }
                XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, iM, LOWORD(cColHaul));
                FLookupFleet(lpfl->id, &sel.fl);
                goto LMoveToLpplBest;
            }
        }
    }
    lpplBest = NULL;
    pctCapMost = 0;
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0)
            break;
        if (lppl != lpplHere) {
            lDist = LDistance2(lpfl->pt, rgptPlan[lppl->id]);
            if (lDist <= 40000) {
                pctCapHere = PctPlanetCapacity(lppl);
                if (lDist > 22500) {
                    pctCapHere -= 6;
                } else if (lDist > 10000) {
                    pctCapHere -= 4;
                } else if (lDist > 2500) {
                    pctCapHere -= 2;
                }
                if (pctCapMost < pctCapHere) {
                    lpplBest = lppl;
                    pctCapMost = pctCapHere;
                }
            }
        }
    }
    if (lpplBest == 0) {
        return -1;
    }
LMoveToLpplBest:
    memset(&ord, 0, sizeof(ORDER));
    ord.grobj = grobjPlanet;
    ord.pt = rgptPlan[lpplBest->id];
    ord.id = lpplBest->id;
    ord.grTask = grTaskXfer;
    ord.fValidTask = TRUE;
    ord.iWarp = 4;
    for (i = 0; i <= 3; i++) {
        ord.txp.rgia[i].iAction = iActionUnloadAll;
    }
    if (FMoveAiFleet(lpfl, &ord, FALSE) == 0) {
        return -1;
    }
    return lpplBest->id;
}

void TargetMacArmada(FLEET *lpfl) {
    FLEET  *lpflTarget;
    ORDER   ord;
    int16_t cshBomb;
    PLANET *lppl;
    int16_t cshWar;
    PLANET *lpplTarget;

    if (lpfl->cord > 1) {
        ord = lpfl->lpplord->rgord[1];
        if (LDistance2(lpfl->pt, ord.pt) <= 62500 || ord.grobj != grobjFleet) {
            if (ord.grobj == grobjFleet) {
                return;
            }
            if (ord.grobj == grobjPlanet) {
                lppl = LpplFromId(ord.id);
                if (lppl == 0 || ((lppl->iPlayer != -1 && (lppl->iPlayer != idPlayer || lppl->fStarbase != 0)) || lppl->turn != game.turn)) {
                    return;
                }
            }
        }
    }
    FPotentMacWarFleet(lpfl, &cshWar);
    cshBomb = lpfl->rgcsh[8] + lpfl->rgcsh[9];
    lpfl->fMark = TRUE;
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if (lpfl->idPlanet == -1) {
        MoveToNearestPlanetOrEnemy(lpfl, 450);
    } else {
        lppl = LpplFromId(lpfl->idPlanet);
        if (lppl->iPlayer == idPlayer) {
            if ((cshWar < vrgAiArmadaPotency[0] || cshBomb < vrgAiArmadaPotency[2]) &&
                (rgplr[idPlayer].lvlAi <= lvlAiStandard || (cshWar <= vrgAiArmadaPotency[0] * 2 && cshWar < 60) ||
                 (Random(10) >= 5 && (cshWar <= vrgAiArmadaPotency[0] * 3 || Random(10) >= 7) && (cshWar <= 120 || Random(10) >= 7)))) {
                return;
            }
        } else if (cshWar < vrgAiArmadaPotency[1] || cshBomb < vrgAiArmadaPotency[3]) {
            ClearAiCurrentTask(lpfl, FALSE);
            if (rgplr[idPlayer].lvlAi <= lvlAiStandard || ((cshWar <= vrgAiArmadaPotency[0] * 2 || Random(10) >= 5) &&
                                                           (cshWar <= vrgAiArmadaPotency[0] * 4 || Random(10) >= 7) && (cshWar <= 120 || Random(10) >= 7))) {
                lpplTarget = LpplFindClosestEnum(lppl, FEnumOurStarbase);
                goto TargetEveryArmada;
            }
        } else if (lppl->iPlayer != -1) {
            return;
        }
        if (game.fAisBand != 0) {
            lpplTarget = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
        } else {
            lpplTarget = NULL;
        }
        if (lpplTarget == 0) {
            lpplTarget = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);
        }
    TargetEveryArmada:
        if (lpplTarget != 0) {
            vlpbAiPlanet[lpplTarget->id * 16 + 10] = vlpbAiPlanet[lpplTarget->id * 16 + 0xa] | 0x80;
            ord.id = lpplTarget->id;
            ord.grobj = grobjPlanet;
            ord.pt = rgptPlan[lpplTarget->id];
        } else {
            lpflTarget = LpflFindClosestEnum(lpfl, FEnumCalcEnemyFleets);
            if (lpflTarget == 0) {
                return;
            }
            ord.id = lpflTarget->id;
            ord.grobj = grobjFleet;
            ord.pt = lpflTarget->pt;
        }
        ord.grTask = grTaskNone;
        ord.fValidTask = TRUE;
        ord.iWarp = 4;
        if (FMoveAiFleet(lpfl, &ord, FALSE) == 0) {
        }
    }
    return;
}

int16_t FPotentMacWarFleet(FLEET *lpfl, int16_t *pcEquiv) {
    int16_t ish;
    int16_t cEquiv;

    cEquiv = 0;
    for (ish = 2; ish <= 4; ish++) {
        cEquiv += lpfl->rgcsh[ish];
    }
    for (ish = 5; ish <= 7; ish++) {
        cEquiv += lpfl->rgcsh[ish] * 2;
    }
    if (cEquiv < vrgAiArmadaPotency[0]) {
        for (ish = 8; ish <= 9; ish++) {
            if (lpfl->rgcsh[ish] != 0 && rgshdef[ish].fFree == 0 && rgshdef[ish].hul.ihuldef == ihuldefBattleship) {
                cEquiv += lpfl->rgcsh[ish] * 2;
            }
        }
        if (cEquiv <= vrgAiArmadaPotency[0]) {
            return FALSE;
        }
    }
    if (pcEquiv != 0) {
        *pcEquiv = cEquiv;
    }
    return TRUE;
}
