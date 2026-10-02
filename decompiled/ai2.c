#include "common.h"

uint8_t vrgISIshAip[19] = {isOffsetColonizingMediumFreighter,
                           isOffsetScoutStreamingBeam,
                           isOffsetUnusedThreeSlotAntiMatterBeam,
                           isOffsetUnusedThreeSlotMcmBeam,
                           isOffsetDestroyerTorpedo,
                           isOffsetUnusedSevenSlotMissile,
                           isOffsetUnusedSevenSlotBeamA,
                           isOffsetUnusedSevenSlotTorpedo,
                           isOffsetUnusedSevenSlotBeamB,
                           isOffsetUnusedSevenSlotMissileB,
                           isOffsetBattleshipBeamA,
                           isOffsetBattleshipTorpedo,
                           isOffsetBattleshipBeamB,
                           isOffsetBattleshipMissile,
                           isOffsetMediumFreighter,
                           isOffsetB17Bomber,
                           isOffsetB52Bomber,
                           isOffsetPrivateerMineLayer,
                           isOffsetSuperFreighter};
uint8_t vrgISAip[182] = {aiPartEnginePreferTransStar,
                         aiPartColonyPreferOrbitalConstruction,
                         aiPartShieldPreferCompletePhase,
                         aiPartEnginePreferTransStar,
                         aiPartScannerPreferElephant,
                         aiPartBeamPreferStreamingPulverizer,
                         aiPartEnginePreferTransStar,
                         aiPartScannerPreferElephant,
                         aiPartBeamPreferAntiMatterPulverizer,
                         aiPartEnginePreferTransStar,
                         aiPartScannerPreferElephant,
                         aiPartBeamPreferMultiContainedMunition,
                         aiPartEnginePreferTransStar,
                         aiPartTorpedo,
                         aiPartTorpedo,
                         aiPartSpecialPodThrustCloak,
                         aiPartArmorPreferSuperlatanium,
                         aiPartSpecialThrustDeflectorFuel,
                         aiPartBattleComputer,
                         aiPartEnginePreferTransStar,
                         aiPartMissile,
                         aiPartMissile,
                         aiPartBattleComputer,
                         aiPartArmorPreferSuperlatanium,
                         aiPartSpecialThrustDeflectorFuel,
                         aiPartBattleComputer,
                         aiPartEnginePreferGalaxyScoop,
                         aiPartShieldPreferCompletePhase,
                         aiPartSpecialDeflectorCapacitorFuel,
                         aiPartSapper,
                         aiPartBeamPreferStreamingPulverizer,
                         aiPartBeamPreferBigMuthaCannon,
                         aiPartArmorPreferSuperlatanium,
                         aiPartEnginePreferGalaxyScoop,
                         aiPartBattleComputer,
                         aiPartSpecialPodThrustCloak,
                         aiPartTorpedo,
                         aiPartTorpedo,
                         aiPartTorpedo,
                         aiPartArmorPreferSuperlatanium,
                         aiPartEnginePreferGalaxyScoop,
                         aiPartShieldPreferCompletePhase,
                         aiPartSpecialDeflectorCapacitorFuel,
                         aiPartSapper,
                         aiPartBeamPreferAntiMatterPulverizer,
                         aiPartBeamPreferBigMuthaCannon,
                         aiPartArmorPreferMegaPolyShell,
                         aiPartEnginePreferGalaxyScoop,
                         aiPartBattleComputer,
                         aiPartSpecialPodThrustCloak,
                         aiPartMissile,
                         aiPartMissile,
                         aiPartMissile,
                         aiPartArmorPreferMegaPolyShell,
                         aiPartEnginePreferGalaxyScoop,
                         aiPartSpecialPodJammerDeflectorThrust,
                         aiPartShieldPreferCompletePhase,
                         aiPartBeamPreferBigMuthaCannon,
                         aiPartBeamPreferAntiMatterPulverizer,
                         aiPartBeamPreferBlunderbuss,
                         aiPartBeamPreferAntiMatterPulverizer,
                         aiPartSapper,
                         aiPartArmorPreferSuperlatanium,
                         aiPartSpecialCapacitorJammerPodComputer,
                         aiPartSpecialCapacitorJammerPodComputer,
                         aiPartEnginePreferGalaxyScoop,
                         aiPartSpecialPodJammerDeflectorThrust,
                         aiPartShieldPreferCompletePhase,
                         aiPartTorpedo,
                         aiPartTorpedo,
                         aiPartTorpedo,
                         aiPartTorpedo,
                         aiPartTorpedo,
                         aiPartArmorPreferSuperlatanium,
                         aiPartSpecialJammerComputer,
                         aiPartBattleComputer,
                         aiPartEnginePreferGalaxyScoop,
                         aiPartSpecialPodJammerDeflectorThrust,
                         aiPartShieldPreferCompletePhase,
                         aiPartBeamPreferBigMuthaCannon,
                         aiPartBeamPreferAntiMatterPulverizer,
                         aiPartBeamPreferStreamingPulverizer,
                         aiPartBeamPreferMultiContainedMunition,
                         aiPartSapper,
                         aiPartArmorPreferMegaPolyShell,
                         aiPartSpecialCapacitorJammerPodComputer,
                         aiPartSpecialCapacitorJammerPodComputer,
                         aiPartEnginePreferGalaxyScoop,
                         aiPartSpecialPodJammerDeflectorThrust,
                         aiPartShieldPreferCompletePhase,
                         aiPartMissile,
                         aiPartMissile,
                         aiPartMissile,
                         aiPartMissile,
                         aiPartMissile,
                         aiPartArmorPreferMegaPolyShell,
                         aiPartSpecialJammerComputer,
                         aiPartBattleComputer,
                         aiPartEnginePreferGalaxyScoop,
                         aiPartCargoPod,
                         aiPartShieldPreferCompletePhase,
                         aiPartEnginePreferGalaxyScoop,
                         aiPartBombHushThenNormal,
                         aiPartBombHushThenSmartThenNormalThenRetro,
                         aiPartSpecialPodJammerDeflectorThrust,
                         aiPartEnginePreferGalaxyScoop,
                         aiPartBombHushThenNormal,
                         aiPartBombHushThenSmartThenNormalThenRetro,
                         aiPartBombHushThenSmartThenNormalThenRetro,
                         aiPartBombHushThenSmartThenNormalThenRetro,
                         aiPartSpecialPodJammerDeflectorThrust,
                         aiPartShieldPreferCompletePhase,
                         aiPartEnginePreferTransStar,
                         aiPartShieldPreferCompletePhase,
                         aiPartSpecialPodJammerDeflectorThrust,
                         aiPartStandardMineDispenser,
                         aiPartSpeedTrapMineDispenser,
                         aiPartEnginePreferGalaxyScoop,
                         aiPartCargoPod,
                         aiPartShieldPreferCompletePhase,
                         aiPartSpecialJammerComputer};
uint8_t vrgAiISResOrder[18] = {aiResearchElectronics6,   aiResearchConstruction4,  aiResearchPropulsion5,   aiResearchWeapons5,       aiResearchBiotechnology4,
                               aiResearchEnergy4,        aiResearchElectronics7,   aiResearchConstruction6, aiResearchPropulsion7,    aiResearchWeapons8,
                               aiResearchBiotechnology6, aiResearchEnergy7,        aiResearchElectronics12, aiResearchConstruction13, aiResearchPropulsion9,
                               aiResearchWeapons11,      aiResearchBiotechnology7, aiResearchEnergy10};

void DoMaidAiTurn(PROD *rgprod) {
    int32_t rgResCost[4];
    int32_t rgResAvail[4];
    int16_t iroCur;

    iroCur = IroEnsureAi(NULL, 0, NULL, game.turn >= 20 ? 15 : 0);
    fMarkedPlanets = 0;
    HandleBasicAiTasks(iroCur, rgprod, -1, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

int16_t FPotentISWarFleet(FLEET *lpfl, int16_t iPotency) {
    int16_t ish;
    int16_t cEquiv;

    cEquiv = 0;
    for (ish = 11; ish <= 12; ish++) {
        cEquiv += lpfl->rgcsh[ish];
    }
    for (ish = 9; ish <= 10; ish++) {
        cEquiv += lpfl->rgcsh[ish] * 2;
    }
    if (iPotency < 2) {
        return 1;
    }
    if (cEquiv >= vrgAiArmadaPotency[0]) {
        return 1;
    }
    return 0;
}

void DoAutomitronAiTurn(PROD *rgprod) {
    int16_t  cExistCargo;
    uint16_t rgCosts[4];
    int32_t  rgResCost[4];
    int16_t  iLatestCruiser;
    int32_t  rgResAvail[4];
    FLEET   *lpflEnemy;
    ORDER    ord;
    uint8_t  rgRecycleShdef[16];
    PLANET  *lpplDest;
    int16_t  cplNegative;
    THING   *lpthWorm;
    int16_t  cFr;
    int16_t  idPlanDst;
    int16_t  cplBadGuy;
    PLANET  *lpplMac;
    PLANET  *lppl;
    PLANET  *lpplHome;
    FLEET   *lpfl;
    int16_t  ifl;
    int16_t  i;
    int16_t  cRes;
    int16_t  iroCur;
    FLEET   *lpflT;
    FLEET   *lpflAttack;
    uint8_t  b;
    int16_t  ishdefSBLatest;
    uint16_t cRecyclePeriod;
    uint16_t cplanCol;
    int16_t  iLatestCargo;
    int16_t  iLatestBomber;
    int16_t  j;
    int16_t  iLatestBattle;
    int32_t  l;
    PROD    *lpprod;
    int16_t  fWrite;
    int16_t  iPlanet;
    int16_t  id;

    iPlanet = rgplr[idPlayer].idPlanetHome;
    cplBadGuy = 0;
    cplNegative = 0;
    cplanCol = 0;
    iroCur = IroEnsureAi((uint8_t *)vrgAiISResOrder, 18, &ishdefSBLatest, game.turn >= 10 ? 20 : 0);
    if (game.turn > 50) {
        MergeAllShdefs(7692);
        MergeAllShdefs(64);
        MergeAllShdefs(16384);
    } else if (game.turn > 30) {
        MergeAllShdefs(16384);
    }
    j = 3;
    if (game.turn > 130) {
        j += (uint32_t)(game.turn - 120) / 20;
    }
    if (j > 50) {
        j = 50;
    }
    vrgAiArmadaPotency[0] = LOBYTE(j);
    vrgAiArmadaPotency[1] = LOBYTE((int16_t)(j & 0xff) / 2);
    j = 6;
    if (game.turn > 115) {
        j += (uint32_t)(game.turn - 100) / 22;
    }
    if (j > 12) {
        j = 12;
    }
    vrgAiArmadaPotency[2] = LOBYTE(j);
    vrgAiArmadaPotency[3] = LOBYTE(3 >= j / 2 - 1 ? j / 2 - 1 : 3);
    memset(rgRecycleShdef, 0, 16);
    if (game.turn < 120) {
        cRecyclePeriod = 50;
    } else {
        cRecyclePeriod = game.turn >= 200 ? 100 : 70;
    }
    CheckAiShdefStatus(11, 12, cRecyclePeriod, &iLatestCruiser, rgRecycleShdef);
    cExistCargo = CheckAiShdefStatus(4, 5, cRecyclePeriod, &iLatestCargo, rgRecycleShdef);
    CheckAiShdefStatus(2, 3, cRecyclePeriod, &iLatestBomber, rgRecycleShdef);
    CheckAiShdefStatus(9, 10, cRecyclePeriod, &iLatestBattle, rgRecycleShdef);
    if (game.turn > 60) {
        SplitOutShdefs(rgRecycleShdef);
    }
    EnsureISShdefs(iroCur);
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == -1 && lppl->det >= detSome && PctPlanetOptValue(lppl, idPlayer) > 0) {
            cplanCol++;
        }
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        vlpbAiPlanet[lppl->id * 16 + 9] = 1;
        if (lppl->iPlayer != idPlayer && lppl->iPlayer != -1) {
            vlpbAiPlanet[lppl->id * 16 + 10] = LOBYTE((lppl->fStarbase & 0xff) + 1);
            if (PctPlanetOptValue(lppl, idPlayer) > 0) {
                vlpbAiPlanet[lppl->id * 16 + 3] = 1;
            }
            cplBadGuy++;
        } else if (lppl->iPlayer == idPlayer && PctPlanetDesirability(lppl, idPlayer) < 0) {
            cplNegative++;
            vlpbAiPlanet[lppl->id * 16 + 2] = 1;
        } else if (lppl->fStarbase != 0 && lppl->rgwtMin[3] >= 1500) {
            ChangeMainObjSel(grobjPlanet, lppl->id);
            InitProduction(rgprod);
            fWrite = 0;
            b = 0;
            i = 0;
            for (lpprod = lpplProdGlob->rgprod; i < lpplProdGlob->iprodMac && (lpprod->grobj != grobjFleet || lpprod->iItem >= iobjPacketGerm); lpprod++) {
                i++;
            }
            if (i < lpplProdGlob->iprodMac) {
                FinishProduction(0);
            } else {
                cFr = rgplr[idPlayer].cPlanet / 10 <= ((AIHIST *)vlpbAiData)->cStarbase * 2 ? ((AIHIST *)vlpbAiData)->cStarbase * 2
                                                                                            : rgplr[idPlayer].cPlanet / 10;
                if (rgplr[idPlayer].rgTech[2] >= 5 && iLatestCargo != -1 && (cExistCargo < cFr || (cExistCargo < (int16_t)(10 * cFr) / 7 && Random(4) == 0))) {
                    AddItemToQueue(iLatestCargo, 1, grobjFleet, addItemEnd);
                    fWrite = 1;
                }
                if (cplanCol != 0 && rgshdef[1].cExist == 0 && game.turn > 10) {
                    AddItemToQueue(1, 1, grobjFleet, addItemEnd);
                    fWrite = 1;
                }
                l = (uint32_t)(lppl->rgwtMin[3] * PctTrueMaxGrowth(idPlayer));
                cRes = CResourcesAtPlanet(lppl, idPlayer);
                if (rgshdef[6].fFree == 0 && Random(3) == 0) {
                    id = lppl->id;
                    cFr = 0;
                    for (ifl = 0; ifl < cFleet; ifl++) {
                        lpfl = rglpfl[ifl];
                        if (rglpfl[ifl] == 0)
                            break;
                        if (lpfl->idPlanet == id && lpfl->rgcsh[6] > 0 && lpfl->iPlayer == idPlayer) {
                            cFr = lpfl->rgcsh[6];
                            break;
                        }
                    }
                    if ((cFr < 10 || (cFr < 17 && Random(8) == 0)) && Random(cFr * 2 + 1) == 0) {
                        AddItemToQueue(6, 3, grobjFleet, addItemEnd);
                        fWrite = 1;
                    }
                }
                if (iLatestBomber != -1) {
                    id = lppl->id;
                    for (ifl = 0; ifl < cFleet; ifl++) {
                        lpfl = rglpfl[ifl];
                        if (rglpfl[ifl] == 0)
                            break;
                        if (lpfl->idPlanet == id && lpfl->iPlayer == idPlayer && FPotentISWarFleet(lpfl, 2) != 0) {
                            if (iLatestBomber == -1 || lpfl->rgcsh[2] + lpfl->rgcsh[3] < vrgAiArmadaPotency[2])
                                break;
                            AddItemToQueue(iLatestBomber, 4, grobjFleet, addItemEnd);
                            fWrite = 1;
                            goto FinishProd;
                        }
                    }
                }
                if (iLatestCruiser != -1 && rgshdef[iLatestCruiser].cExist < (uint32_t)(game.cPlanMax / 12 + 8)) {
                    GetResourcesAvailable(lppl, rgResAvail);
                    GetProdQCost(lppl, rgResCost);
                    for (i = 0; i < 4; i++) {
                        rgResAvail[i] -= rgResCost[i];
                        if (rgResAvail[i] < 0)
                            goto FinishProd;
                    }
                    for (i = 0; i < 5; i++) {
                        GetTrueHullCost(idPlayer, &rgshdef[iLatestCruiser].hul, rgCosts);
                        for (j = 0; j < 4; j++) {
                            rgResAvail[j] -= (uint32_t)rgCosts[j];
                            if (rgResAvail[j] < 0)
                                goto FinishProd;
                        }
                        fWrite = 1;
                        AddItemToQueue(iLatestCruiser, 1, grobjFleet, addItemEnd);
                    }
                }
                if (iLatestBattle != -1 && rgshdef[iLatestBattle].cExist < (uint32_t)(game.cPlanMax / 24 + 4)) {
                    GetResourcesAvailable(lppl, rgResAvail);
                    GetProdQCost(lppl, rgResCost);
                    for (i = 0; i < 4; i++) {
                        rgResAvail[i] -= rgResCost[i];
                        if (rgResAvail[i] < 0)
                            goto FinishProd;
                    }
                    for (i = 0; i < 5; i++) {
                        GetTrueHullCost(idPlayer, &rgshdef[iLatestBattle].hul, rgCosts);
                        for (j = 0; j < 4; j++) {
                            rgResAvail[j] -= (uint32_t)rgCosts[j];
                            if (rgResAvail[j] < 0)
                                goto FinishProd;
                        }
                        fWrite = 1;
                        AddItemToQueue(iLatestBattle, 1, grobjFleet, addItemEnd);
                    }
                }
            FinishProd:
                FinishProduction(fWrite);
            }
        }
    }
    lpflAttack = NULL;
    lpflEnemy = NULL;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->iPlayer != idPlayer) {
            lpfl->lpflNext = lpflEnemy;
            lpflEnemy = lpfl;
        } else {
            if (FIsAiAttack(lpfl) != 0) {
                lpfl->lpflNext = lpflAttack;
                lpflAttack = lpfl;
            }
            lpfl->fMark = 0;
            if (FIsAiTransport(lpfl) != 0) {
                idPlanDst = -1;
                if (lpfl->cord <= 1) {
                    idPlanDst = lpfl->idPlanet;
                } else if (lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
            } else {
                if (lpfl->rgcsh[1] == 0)
                    continue;
                idPlanDst = -1;
                if (lpfl->cord <= 1) {
                    idPlanDst = lpfl->idPlanet;
                } else if (lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
                if (idPlanDst == -1)
                    continue;
                if (vlpbAiPlanet[idPlanDst * 16 + 3] == 0) {
                    lppl = LpplFromId(idPlanDst);
                    if (lppl != 0 && lppl->iPlayer != -1 && lppl->iPlayer != idPlayer)
                        goto LBlowAwayOrders;
                    continue;
                }
            }
            if (idPlanDst == -1)
                continue;
            lppl = LpplFromId(idPlanDst);
            if (lppl != 0 && (lppl->iPlayer == -1 || lppl->iPlayer == idPlayer))
                continue;
            if (vlpbAiPlanet[idPlanDst * 16 + 3] != 0 && lpfl->rgwtMin[3] > 0 && GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                memset(&ord, 0, sizeof(ORDER));
                ord.pt = rgptPlan[idPlanDst];
                ord.grobj = grobjPlanet;
                ord.id = idPlanDst;
                ord.grTask = grTaskXfer;
                ord.fValidTask = 1;
                ord.txp.rgia[3].iAction = iActionUnloadAll;
                ChangeMainObjSel(grobjFleet, lpfl->id);
                if (sel.fl.lpplord->rgord[0].id == idPlanDst) {
                    sel.fl.lpplord->rgord[0] = ord;
                } else {
                    sel.fl.lpplord->rgord[1] = ord;
                }
                FLookupFleet(-1, &sel.fl);
                vlpbAiPlanet[idPlanDst * 16 + 3] = vlpbAiPlanet[idPlanDst * 16 + 3] | 0x80;
                continue;
            }
        LBlowAwayOrders:
            ChangeMainObjSel(grobjFleet, lpfl->id);
            sel.fl.cord = 1;
            sel.fl.lpplord->iordMac = 1;
            FLookupFleet(-1, &sel.fl);
            ClearAiCurrentTask(lpfl, 0);
        }
    }
    fMarkedPlanets = 0;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->iPlayer == idPlayer) {
            if (lpfl->cord > 1) {
                if (rgshdef[0].hul.rghs[0].iItem >= 10 || lpfl->rgwtMin[4] >= 2)
                    continue;
            } else {
                if (lpfl->rgcsh[6] != 0) {
                    if (lpfl->cord != 1 || lpfl->lpplord->rgord[0].grTask != grTaskNone)
                        continue;
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    sel.fl.lpplord->rgord[0].grTask = grTaskLayMines;
                    sel.fl.lpplord->rgord[0].tlm.cTime = 5;
                    sel.fl.lpplord->rgord[0].tlm.cTimeOld = 5;
                    FLookupFleet(-1, &sel.fl);
                    continue;
                }
                if (lpfl->rgcsh[1] != 0) {
                    if (rgshdef[1].hul.ihuldef == ihuldefMediumFreighter) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        if ((lpfl->idPlanet == -1 || sel.pl.iPlayer != idPlayer || sel.pl.rgwtMin[3] < 200) && lpfl->rgwtMin[3] == 0) {
                            if ((sel.fl.idPlanet != -1 && sel.pl.iPlayer == idPlayer && sel.pl.fStarbase != 0) ||
                                (rgshdef[1].hul.rghs[0].iItem >= 2 && FMoveToNearestStarbase(lpfl, 0) != 0))
                                continue;
                        } else {
                            lpthWorm = NULL;
                            idPlanDst = IdNearestColonizablePlanet(lpfl, NULL);
                            if (lpfl->idPlanet != -1 && sel.pl.iPlayer == idPlayer) {
                                ChangeMainObjSel(grobjFleet, lpfl->id);
                                XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, Colonists, 150);
                                FLookupFleet(lpfl->id, &sel.fl);
                            }
                            if (idPlanDst == -1)
                                continue;
                            FColonizeAiFleet(lpfl, idPlanDst);
                            vlpbAiPlanet[idPlanDst * 16 + 15] = 4;
                            continue;
                        }
                    }
                } else {
                    if (FIsAiTransport(lpfl) != 0) {
                        lppl = lpPlanets;
                        lpplMac = lpPlanets + cPlanet;
                        for (; lppl < lpplMac && (lppl->iPlayer != idPlayer || lppl->fStarbase == 0); lppl++) {
                        }
                        lpplHome = lppl == lpplMac ? NULL : lppl;
                        if (lpplHome == 0)
                            break;
                        lppl = NULL;
                        for (i = 0; i < ((AIHIST *)vlpbAiData)->cStarbase; i++) {
                            for (j = 0; j < ((AIHIST *)vlpbAiData)->rgasb[i].cFreighter && ((AIHIST *)vlpbAiData)->rgasb[i].rgflid[j] != lpfl->id; j++) {
                            }
                            if (j < ((AIHIST *)vlpbAiData)->rgasb[i].cFreighter)
                                break;
                        }
                        if (i < ((AIHIST *)vlpbAiData)->cStarbase) {
                            lppl = LpplFromId(((AIHIST *)vlpbAiData)->rgasb[i].idPlanet);
                        }
                        IdTargetFreighter(lpfl, lppl == 0 ? lpplHome : lppl);
                        continue;
                    }
                    if (lpfl->rgcsh[2] == 0 && lpfl->rgcsh[3] == 0) {
                        if (lpfl->rgcsh[0] == 0)
                            continue;
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        if (rgshdef[0].hul.rghs[0].iItem >= 10 || lpfl->rgwtMin[4] >= 2) {
                            IdTargetScout(lpfl, lpflAttack, lpflEnemy, game.fAisBand, &lpthWorm);
                            continue;
                        }
                    } else {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        if (lpfl->idPlanet != -1) {
                            lppl = LpplFromId(lpfl->idPlanet);
                            if (lppl->iPlayer == idPlayer) {
                                if (lppl->fStarbase != 0 && ((lpfl->rgcsh[2] < 2 && lpfl->rgcsh[3] < 2) || (lpfl->rgcsh[9] < 3 && lpfl->rgcsh[10] < 3)))
                                    continue;
                                FLookupFleet(lpfl->id, &sel.fl);
                            } else if (lppl->iPlayer != -1) {
                                lpflT = lpflEnemy;
                                while (1) {
                                    if (lpflT == 0)
                                        goto L_1162;
                                    if (lpfl->pt.x == lpflT->pt.x && lpfl->pt.y == lpflT->pt.y && FIsAiAttack(lpflT) != 0)
                                        break;
                                    lpflT = lpflT->lpflNext;
                                }
                            }
                        } else {
                            lppl = lpplHome;
                        }
                        if (game.fAisBand != 0) {
                            lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
                        } else {
                            lpplDest = NULL;
                        }
                        if (lpplDest == 0) {
                            lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);
                        }
                        lppl = lpplDest;
                        if (lppl == 0)
                            continue;
                        vlpbAiPlanet[lppl->id * 16 + 10] = vlpbAiPlanet[lppl->id * 16 + 0xa] | 0x80;
                        ord.id = lppl->id;
                        ord.grobj = grobjPlanet;
                        ord.pt = rgptPlan[lppl->id];
                        ord.grTask = grTaskNone;
                        ord.fValidTask = 1;
                        ord.iWarp = 4;
                        FMoveAiFleet(lpfl, &ord, 0);
                        continue;
                    }
                }
            }
            ChangeMainObjSel(grobjFleet, lpfl->id);
            sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
            FLookupFleet(-1, &sel.fl);
        }
    L_1162:;
    }
    HandleBasicAiTasks(iroCur, rgprod, ishdefSBLatest, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

void EnsureISShdefs(int16_t iroCur) {
    SHDEF   shdef;
    int16_t i;

    if (rgshdef[4].fFree != 0 && rgplr[idPlayer].rgTech[2] >= 5) {
        FCreateAiShdef(4, ihuldefMediumFreighter, (uint8_t *)&vrgISAip[vrgISIshAip[14]]);
    }
    if (rgshdef[5].fFree != 0 && rgplr[idPlayer].rgTech[2] >= 7) {
        FCreateAiShdef(5, ihuldefSuperFreighter, (uint8_t *)&vrgISAip[vrgISIshAip[18]]);
    }
    if (rgshdef[14].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 5 && rgplr[idPlayer].rgTech[4] >= 6 && rgplr[idPlayer].rgTech[3] >= 4 &&
        rgplr[idPlayer].rgTech[2] >= 5) {
        for (i = 0; i < 4 && FCreateAiShdef(14, ihuldefDestroyer, (uint8_t *)&vrgISAip[vrgISIshAip[Random(1) + 4]]) == 0; i++) {
        }
    }
    if (rgshdef[1].fFree != 0 || rgshdef[1].cExist == 0) {
        if (rgshdef[1].fFree == 0) {
            shdef = rgshdef[1];
            shdef.fFree = 1;
            FChangeAiShdef(&shdef, 1);
        }
        FCreateAiShdef(1, ihuldefMediumFreighter, (uint8_t *)&vrgISAip[vrgISIshAip[0]]);
    }
    if (rgshdef[0].fFree != 0 || rgshdef[0].cExist == 0) {
        if (rgshdef[0].fFree == 0) {
            shdef = rgshdef[0];
            shdef.fFree = 1;
            FChangeAiShdef(&shdef, 0);
        }
        FCreateAiShdef(0, ihuldefScout, (uint8_t *)&vrgISAip[vrgISIshAip[1]]);
    }
    if (rgshdef[6].fFree != 0 && rgplr[idPlayer].rgTech[3] >= 4 && rgplr[idPlayer].rgTech[2] >= 5 && rgplr[idPlayer].rgTech[5] >= 6) {
        FCreateAiShdef(6, ihuldefPrivateer, (uint8_t *)&vrgISAip[vrgISIshAip[17]]);
    }
    if (rgshdef[2].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 8 && rgplr[idPlayer].rgTech[4] >= 7 && rgplr[idPlayer].rgTech[3] >= 6 &&
        rgplr[idPlayer].rgTech[2] >= 7) {
        FCreateAiShdef(2, ihuldefB17Bomber, (uint8_t *)&vrgISAip[vrgISIshAip[15]]);
    }
    if (rgshdef[3].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 11 && rgplr[idPlayer].rgTech[4] >= 12 && rgplr[idPlayer].rgTech[3] >= 15 &&
        rgplr[idPlayer].rgTech[2] >= 9) {
        FCreateAiShdef(3, ihuldefB52Bomber, (uint8_t *)&vrgISAip[vrgISIshAip[16]]);
    }
    if (rgshdef[9].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 5 && rgplr[idPlayer].rgTech[4] >= 6 && rgplr[idPlayer].rgTech[3] >= 13 &&
        rgplr[idPlayer].rgTech[2] >= 7) {
        for (i = 0; i < 5 && FCreateAiShdef(9, ihuldefBattleship, (uint8_t *)&vrgISAip[vrgISIshAip[Random(4) + 0xa]]) == 0; i++) {
        }
    }
    return;
}

void DoRototillAiTurn(PROD *rgprod) {
    int32_t  rgResCost[4];
    int32_t  rgResAvail[4];
    FLEET   *lpflEnemy;
    ORDER    ord;
    PLANET  *lpplDest;
    int16_t  cplNegative;
    THING   *lpthWorm;
    int16_t  fColonyShipInQueue;
    int16_t  idPlanDst;
    int16_t  cplBadGuy;
    PLANET  *lpplMac;
    PLANET  *lppl;
    PLANET  *lpplHome;
    FLEET   *lpfl;
    int16_t  ifl;
    int16_t  i;
    int16_t  iroCur;
    FLEET   *lpflT;
    FLEET   *lpflAttack;
    uint8_t  b;
    int16_t  ishdefSBLatest;
    int16_t  fBomberInQueue;
    uint16_t cplanCol;
    int16_t  j;
    PROD    *lpprod;
    int16_t  fWrite;
    int16_t  iPlanet;
    uint8_t  bT;

    iPlanet = rgplr[idPlayer].idPlanetHome;
    cplBadGuy = 0;
    cplNegative = 0;
    cplanCol = 0;
    fColonyShipInQueue = 0;
    fBomberInQueue = 0;
    iroCur = IroEnsureAi(NULL, 0, &ishdefSBLatest, game.turn >= 20 ? 15 : 0);
    EnsureCAShdefs(iroCur);
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == -1 && lppl->det >= detSome && PctPlanetOptValue(lppl, idPlayer) > 0) {
            cplanCol++;
        }
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        vlpbAiPlanet[lppl->id * 16 + 9] = 1;
        if (lppl->iPlayer == -1 && lppl->det >= detSome) {
            b = 0;
            for (i = 0; i < 3; i++) {
                if (lppl->rgMinConc[i] > 66) {
                    bT = 75;
                } else {
                    bT = LOBYTE((int16_t)lppl->rgMinConc[i] / 2);
                }
                b += LOBYTE(bT);
            }
            if ((b & 0x80) != 0) {
                b = 127;
            }
            vlpbAiPlanet[lppl->id * 16 + 1] = b;
        }
        if (lppl->iPlayer != idPlayer && lppl->iPlayer != -1) {
            vlpbAiPlanet[lppl->id * 16 + 10] = LOBYTE((lppl->fStarbase & 0xff) + 1);
            if (PctPlanetOptValue(lppl, idPlayer) > 0) {
                vlpbAiPlanet[lppl->id * 16 + 3] = 1;
            }
            cplBadGuy++;
        } else if (lppl->iPlayer == idPlayer && PctPlanetDesirability(lppl, idPlayer) < 0) {
            cplNegative++;
            vlpbAiPlanet[lppl->id * 16 + 2] = 1;
        } else if (lppl->fStarbase != 0 && lppl->rgwtMin[3] >= 1000) {
            ChangeMainObjSel(grobjPlanet, lppl->id);
            InitProduction(rgprod);
            fWrite = 0;
            b = 0;
            i = 0;
            for (lpprod = lpplProdGlob->rgprod; i < lpplProdGlob->iprodMac && (lpprod->grobj != grobjFleet || lpprod->iItem <= iobjPacketGerm); lpprod++) {
                i++;
            }
            if (i < lpplProdGlob->iprodMac) {
                FinishProduction(0);
            } else {
                if (game.turn == 0) {
                    AddItemToQueue(0, 1, grobjFleet, addItemEnd);
                    AddItemToQueue(0, 1, grobjFleet, addItemEnd);
                } else if (fColonyShipInQueue == 0 && (rgshdef[1].cExist == 0 || rgshdef[1].cExist + 1 < (uint32_t)cplanCol)) {
                    fColonyShipInQueue = 1;
                    AddItemToQueue(1, 1, grobjFleet, addItemEnd);
                    fWrite = 1;
                }
                FinishProduction(fWrite);
            }
        }
    }
    lpflAttack = NULL;
    lpflEnemy = NULL;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->iPlayer != idPlayer) {
            lpfl->lpflNext = lpflEnemy;
            lpflEnemy = lpfl;
        } else {
            if (FIsTurinDroneAiAttack(lpfl) != 0) {
                lpfl->lpflNext = lpflAttack;
                lpflAttack = lpfl;
            }
            lpfl->fMark = 0;
            if ((lpfl->rgcsh[7] != 0 || lpfl->rgcsh[8] != 0) && lpfl->cord >= 1) {
                if (lpfl->idPlanet != -1) {
                    if (LpplFromId(lpfl->idPlanet)->iPlayer != -1)
                        goto LBlowAwayOrders;
                    idPlanDst = lpfl->idPlanet;
                } else {
                    if (lpfl->cord <= 1)
                        continue;
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
                vlpbAiPlanet[idPlanDst * 16 + 1] = vlpbAiPlanet[idPlanDst * 16 + 1] | 0x80;
                continue;
            }
            if (FIsAiTransport(lpfl) != 0) {
                idPlanDst = -1;
                if (lpfl->cord <= 1) {
                    idPlanDst = lpfl->idPlanet;
                } else if (lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
            } else {
                if (lpfl->rgcsh[1] == 0)
                    continue;
                idPlanDst = -1;
                if (lpfl->cord <= 1) {
                    idPlanDst = lpfl->idPlanet;
                } else if (lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
                if (idPlanDst == -1)
                    continue;
                if (vlpbAiPlanet[idPlanDst * 16 + 3] == 0) {
                    lppl = LpplFromId(idPlanDst);
                    if (lppl != 0 && lppl->iPlayer != -1 && lppl->iPlayer != idPlayer)
                        goto LBlowAwayOrders;
                    continue;
                }
            }
            if (idPlanDst == -1)
                continue;
            lppl = LpplFromId(idPlanDst);
            if (lppl != 0 && (lppl->iPlayer == -1 || lppl->iPlayer == idPlayer))
                continue;
            if (vlpbAiPlanet[idPlanDst * 16 + 3] != 0 && lpfl->rgwtMin[3] > 0 && GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                memset(&ord, 0, sizeof(ORDER));
                ord.pt = rgptPlan[idPlanDst];
                ord.grobj = grobjPlanet;
                ord.id = idPlanDst;
                ord.grTask = grTaskXfer;
                ord.fValidTask = 1;
                ord.txp.rgia[3].iAction = iActionUnloadAll;
                ChangeMainObjSel(grobjFleet, lpfl->id);
                if (sel.fl.lpplord->rgord[0].id == idPlanDst) {
                    sel.fl.lpplord->rgord[0] = ord;
                } else {
                    sel.fl.lpplord->rgord[1] = ord;
                }
                FLookupFleet(-1, &sel.fl);
                vlpbAiPlanet[idPlanDst * 16 + 3] = vlpbAiPlanet[idPlanDst * 16 + 3] | 0x80;
                continue;
            }
        LBlowAwayOrders:
            ChangeMainObjSel(grobjFleet, lpfl->id);
            sel.fl.cord = 1;
            sel.fl.lpplord->iordMac = 1;
            FLookupFleet(-1, &sel.fl);
            ClearAiCurrentTask(lpfl, 0);
        }
    }
    fMarkedPlanets = 0;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->iPlayer == idPlayer) {
            if (lpfl->rgcsh[7] != 0 || lpfl->rgcsh[8] != 0) {
                if (lpfl->idPlanet != -1) {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    b = vlpbAiPlanet[lpfl->idPlanet * 16 + 1];
                    if (b < 4) {
                        lppl = LpplFindBestEnum(&sel.pl, FEnumCalcMinerDest);
                        if (lppl != 0) {
                            ord.id = lppl->id;
                            ord.grobj = grobjPlanet;
                            ord.pt = rgptPlan[lppl->id];
                            ord.grTask = grTaskMine;
                            ord.fValidTask = 1;
                            ord.iWarp = 6;
                            FMoveAiFleet(lpfl, &ord, 1);
                            vlpbAiPlanet[lppl->id * 16 + 1] = vlpbAiPlanet[lppl->id * 16 + 1] | 0x80;
                            vlpbAiPlanet[lpfl->idPlanet * 16 + 1] = vlpbAiPlanet[lpfl->idPlanet * 16 + 1] & 0x80;
                        }
                    }
                }
            } else if (lpfl->cord <= 1) {
                if (lpfl->rgcsh[1] != 0) {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    if ((lpfl->idPlanet == -1 || sel.pl.iPlayer != idPlayer || sel.pl.rgwtMin[3] < 50) && lpfl->rgwtMin[3] == 0) {
                        if ((sel.fl.idPlanet != -1 && sel.pl.iPlayer == idPlayer && sel.pl.fStarbase != 0) ||
                            (rgshdef[1].hul.rghs[0].iItem >= 2 && FMoveToNearestStarbase(lpfl, 0) != 0))
                            continue;
                    } else {
                        lpthWorm = NULL;
                        idPlanDst = IdNearestColonizablePlanet(lpfl, rgshdef[1].hul.rghs[0].iItem <= 1 ? NULL : &lpthWorm);
                        if (lpfl->idPlanet != -1 && sel.pl.iPlayer == idPlayer) {
                            ChangeMainObjSel(grobjFleet, lpfl->id);
                            XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, Colonists, 25);
                            FLookupFleet(lpfl->id, &sel.fl);
                        }
                        if (idPlanDst != -1) {
                            FColonizeAiFleet(lpfl, idPlanDst);
                            vlpbAiPlanet[idPlanDst * 16 + 15] = 4;
                            continue;
                        }
                        if (lpthWorm == 0)
                            continue;
                        FGotoWormholeAiFleet(lpfl, lpthWorm);
                        continue;
                    }
                } else {
                    if (FIsAiTransport(lpfl) != 0) {
                        lppl = lpPlanets;
                        lpplMac = lpPlanets + cPlanet;
                        for (; lppl < lpplMac && (lppl->iPlayer != idPlayer || lppl->fStarbase == 0); lppl++) {
                        }
                        lpplHome = lppl == lpplMac ? NULL : lppl;
                        if (lpplHome == 0)
                            break;
                        lppl = NULL;
                        for (i = 0; i < ((AIHIST *)vlpbAiData)->cStarbase; i++) {
                            for (j = 0; j < ((AIHIST *)vlpbAiData)->rgasb[i].cFreighter && ((AIHIST *)vlpbAiData)->rgasb[i].rgflid[j] != lpfl->id; j++) {
                            }
                            if (j < ((AIHIST *)vlpbAiData)->rgasb[i].cFreighter)
                                break;
                        }
                        if (i < ((AIHIST *)vlpbAiData)->cStarbase) {
                            lppl = LpplFromId(((AIHIST *)vlpbAiData)->rgasb[i].idPlanet);
                        }
                        IdTargetFreighter(lpfl, lppl == 0 ? lpplHome : lppl);
                        continue;
                    }
                    if (lpfl->rgcsh[13] == 0 && lpfl->rgcsh[14] == 0) {
                        if (lpfl->rgcsh[0] == 0)
                            continue;
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        if (lpfl->rgcsh[0] == 0 || rgshdef[0].hul.rghs[0].iItem != 1 || lpfl->rgwtMin[4] >= 2) {
                            IdTargetScout(lpfl, lpflAttack, lpflEnemy, game.fAisBand, &lpthWorm);
                            continue;
                        }
                    } else {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        if (lpfl->idPlanet != -1) {
                            lppl = LpplFromId(lpfl->idPlanet);
                            if (lppl->iPlayer == idPlayer) {
                                if (lppl->fStarbase != 0 && lpfl->rgcsh[13] < 2 && lpfl->rgcsh[14] < 2)
                                    continue;
                                FLookupFleet(lpfl->id, &sel.fl);
                            } else if (lppl->iPlayer != -1) {
                                lpflT = lpflEnemy;
                                while (1) {
                                    if (lpflT == 0)
                                        goto L_279b;
                                    if (lpfl->pt.x == lpflT->pt.x && lpfl->pt.y == lpflT->pt.y && FIsAiAttack(lpflT) != 0)
                                        break;
                                    lpflT = lpflT->lpflNext;
                                }
                            }
                        } else {
                            lppl = lpplHome;
                        }
                        if (game.fAisBand != 0) {
                            lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
                        } else {
                            lpplDest = NULL;
                        }
                        if (lpplDest == 0) {
                            lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);
                        }
                        lppl = lpplDest;
                        if (lppl == 0)
                            continue;
                        vlpbAiPlanet[lppl->id * 16 + 10] = vlpbAiPlanet[lppl->id * 16 + 0xa] | 0x80;
                        ord.id = lppl->id;
                        ord.grobj = grobjPlanet;
                        ord.pt = rgptPlan[lppl->id];
                        ord.grTask = grTaskNone;
                        ord.fValidTask = 1;
                        ord.iWarp = 4;
                        FMoveAiFleet(lpfl, &ord, 0);
                        continue;
                    }
                }
                ChangeMainObjSel(grobjFleet, lpfl->id);
                sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
                FLookupFleet(-1, &sel.fl);
            }
        }
    L_279b:;
    }
    HandleBasicAiTasks(iroCur, rgprod, ishdefSBLatest, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

void EnsureCAShdefs(int16_t iroCur) { return; }
