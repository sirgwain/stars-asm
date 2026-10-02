int16_t FLookupPart(PART *ppart) {
    RaceAttribute raMajor;
    HS            hs;

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
        if (hs.iItem == iengineSettlersDelight && raMajor != raCheapCol) {
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
        if (hs.iItem == ishieldShadowShield && raMajor != raStealth) {
            return -1;
        }
        if (hs.iItem == ishieldCrobySharmor && raMajor != raDefend) {
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
        if ((hs.iItem == ihuldefMiniColonyShip || hs.iItem == ihuldefMetaMorph) && raMajor != raCheapCol) {
            return -1;
        }
        if ((hs.iItem == ihuldefFuelTransport || hs.iItem == ihuldefSuperFreighter) && raMajor != raDefend) {
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
                if ((hs.iItem == ihuldefDreadnought || hs.iItem == ihuldefBattleCruiser) && raMajor != raAttack) {
                    return -1;
                }
                if (hs.iItem == ihuldefRogue && raMajor != raStealth) {
                    return -1;
                }
                if (hs.iItem == ihuldefStealthBomber && raMajor != raStealth) {
                    return -1;
                }
                if ((hs.iItem == ihuldefMiniMineLayer || hs.iItem == ihuldefSuperMineLayer) && raMajor != raMines) {
                    return -1;
                }
                if (FShouldPartBeHidden(ppart) == 0)
                    goto L_609c;
                return -1;
            }
        }
    case hstSBHull:
        if (hs.iItem >= isbhullCount) {
            return 0;
        }
        ppart->phul = &rghuldefSB[hs.iItem].hul;
        if (idPlayer == -1)
            break;
        if ((hs.iItem == isbhullSpaceDock || hs.iItem == isbhullUltraStation) && GetRaceGrbit(&rgplr[idPlayer], ibitRaceISB) == 0) {
            return -1;
        }
        if (hs.iItem != isbhullDeathStar || raMajor == raMacintosh)
            break;
        return -1;
    case hstArmor:
        if (hs.iItem >= iarmorCount) {
            return 0;
        }
        ppart->parmor = &rgarmor[hs.iItem];
        if (hs.iItem == iarmorDepletedNeutronium && raMajor != raStealth) {
            return -1;
        }
        if (hs.iItem == iarmorFieldedKelarium && raMajor != raDefend) {
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
        case ispecialETransportCloaking:
        case ispecialEUltraStealthCloak:
            if (raMajor == raStealth)
                goto L_609c;
            return -1;
        case ispecialEEnergyDampener:
            if (raMajor == raMines)
                goto L_609c;
            return -1;
        case ispecialEAntiMatterGenerator:
            if (raMajor == raStargate)
                goto L_609c;
            return -1;
        case ispecialEFluxCapacitor:
            if (raMajor == raCheapCol)
                goto L_609c;
            return -1;
        case ispecialEJammer10:
        case ispecialEJammer50:
        case ispecialETachyonDetector:
            if (raMajor != raDefend) {
                return -1;
            }
        case ispecialEStealthCloak:
        case ispecialESuperStealthCloak:
        case ispecialEMultiFunctionPod:
        case ispecialEBattleComputer:
        case ispecialEBattleSuperComputer:
        case ispecialEBattleNexus:
        case ispecialEJammer20:
        case ispecialEJammer30:
        case ispecialEEnergyCapacitor:
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
        if (hs.iItem == ispecialMColonizationModule && raMajor == raMacintosh) {
            return -1;
        }
        if (hs.iItem != ispecialMOrbitalConstructionModule || raMajor == raMacintosh)
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
            if (hs.iItem == ispecialSBMassDriver7 || hs.iItem == ispecialSBUltraDriver10 || raMajor == raMassAccel)
                break;
            return -1;
        }
        if (hs.iItem < ispecialSBStargate100250 || hs.iItem > ispecialSBStargateAnyAny)
            break;
        if (raMajor != raStargate && (hs.iItem == ispecialSBStargateAny300 || hs.iItem >= ispecialSBStargate100Any)) {
            return -1;
        }
        if (raMajor != raCheapCol)
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
            if (raMajor != raMines) {
                return -1;
            }
        default:
            if (hs.iItem == iminesSpeedTrap20 && raMajor != raMines && raMajor != raDefend) {
                return -1;
            }
            if (hs.iItem != iminesMineDispenser50 || raMajor != raAttack)
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
            if (hs.iItem == iminingOrbitalAdjuster && raMajor != raTerra) {
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
                if (raMajor != raStealth) {
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
        if (hs.iItem == ibeamMiniGun && raMajor != raDefend) {
            return -1;
        }
        if ((hs.iItem == ibeamBlunderbuss || hs.iItem == ibeamGatlingNeutrinoCannon) && raMajor != raAttack) {
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
        if (hs.iItem >= ibombSmartBomb && hs.iItem <= ibombAnnihilatorBomb && raMajor == raDefend) {
            return -1;
        }
        if (hs.iItem == ibombRetroBomb && raMajor != raTerra) {
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
        if (hs.iItem >= iplanetaryViewer50 && hs.iItem <= iplanetarySnooper620X && raMajor == raMacintosh) {
            return -1;
        }
        if (hs.iItem >= iplanetarySDI && hs.iItem <= iplanetaryNeutronShield && raMajor == raMacintosh) {
            return -1;
        }
        if (hs.iItem >= iplanetaryLaserBattery && hs.iItem <= iplanetaryNeutronShield && raMajor == raAttack) {
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
