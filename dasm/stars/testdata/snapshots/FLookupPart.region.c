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
