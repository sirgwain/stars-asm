int16_t FTutorialEnabledShipBuilder(TutorShipBuilderAction itutsbAction) {
    HS      hs2;
    HS      hs3;
    HS      hs;
    HS      hs1;
    HS      hs4;
    int16_t t_call_7c62;

    switch (itutsbAction) {
    default:
        return FALSE;
    case tutsbDelete:
        TutorError(idsTutorialShouldDeleteShipDesignPointTutorial);
        return FALSE;
    case tutsbCopy:
        switch (game.turn) {
        default:
            goto NoCustom;
        case 13:
            if (tutor.idt != idtSelectAvailableHullTypes)
                goto NoCustom;
            if (rgplr[0].cShDef == 7) {
                TutorError(idsTutorialHaveAlreadyCopiedAppropriateShipDesign);
                return FALSE;
            }
            if (FCheckShipBuilder(1, 7) != 0)
                break;
            TutorError(idsTutorialDontHaveCorrectHullSelectedHull);
            return FALSE;
        case 20:
            if (tutor.idt != idtReadFirstTwoMessagesSendArmedProbe)
                goto NoCustom;
            if (rgplr[0].cshdefSB == 2) {
                TutorError(idsTutorialHaveAlreadyCopiedAppropriateShipDesign);
                return FALSE;
            }
            if (FCheckShipBuilder(0, 0) != 0 && fStarbaseMode != 0)
                break;
            TutorError(idsTutorialHaveTriedCopyWrongShipDesign);
            return FALSE;
        case 22:
            if (tutor.idt != idtAddTeamsterStoveTopsQueue)
                goto NoCustom;
            if (rgplr[0].cShDef == 8) {
                TutorError(idsTutorialHaveAlreadyCopiedAppropriateShipDesign);
                return FALSE;
            }
            if (FCheckShipBuilder(1, 3) != 0)
                break;
            TutorError(idsTutorialDontHaveCorrectHullSelectedHull);
            return FALSE;
        case 27:
            if (tutor.idt != idtWeWantPowerfulWeAlsoWantWeigh)
                goto NoCustom;
            if (rgplr[0].cShDef == 9) {
                TutorError(idsTutorialHaveAlreadyCopiedAppropriateShipDesign);
                return FALSE;
            }
            if (FCheckShipBuilder(1, 4) != 0)
                break;
            TutorError(idsTutorialDontHaveCorrectHullSelectedHull);
            return FALSE;
        case 29:
            if (tutor.idt != idtLetsFinishOffBerserkersOnceBuildingBombing)
                goto NoCustom;
            if (rgplr[0].cShDef == 10) {
                TutorError(idsTutorialHaveAlreadyCopiedAppropriateShipDesign);
                return FALSE;
            }
            if (FCheckShipBuilder(1, 8) == 0) {
                TutorError(idsTutorialDontHaveCorrectHullSelectedHull);
                return FALSE;
            }
        }
        return TRUE;
    case tutsbEdit:
        if (game.turn != 25 || tutor.idt != idtReadFirstTwoMessages)
            break;
        t_call_7c62 = FCheckShipBuilder(0, 2);
        if (t_call_7c62 != 0) {
            return t_call_7c62;
        }
        TutorError(idsTutorialDontHaveCorrectShipSelectedShip);
        return FALSE;
    case tutsbAccept:
        switch (game.turn) {
        default:
            goto NoCustom;
        case 13:
            hs.grhst = hstScanner;
            hs.iItem = 1;
            hs.cItem = 1;
            hs2.grhst = hstEngine;
            hs2.iItem = 3;
            hs2.cItem = 1;
            hs3.grhst = hstMining;
            hs3.iItem = 2;
            hs3.cItem = 1;
            if (tutor.idt != idtShipDesignNameImageJustFine) {
                TutorError(idsTutorialHaventYetFinishedCreatingNewDesign);
                return FALSE;
            }
            if (FCheckBuilderPart(0, &hs2, 1) != 0 && FCheckBuilderPart(1, &hs, 1) != 0 && FCheckBuilderPart(2, &hs3, 1) != 0 &&
                FCheckBuilderPart(3, &hs3, 1) != 0)
                break;
            TutorError(idsTutorialDontHaveRightPartsDesignVerify);
            return FALSE;
        case 20:
            if (tutor.idt == idtReadFirstTwoMessagesSendArmedProbe) {
                hs.grhst = hstSpecialSB;
                hs.iItem = 0;
                hs.cItem = 1;
                if (FCheckBuilderPart(0, &hs, 1) == 0) {
                    TutorError(idsTutorialHaventAddedRightPartDesignVerify);
                    return FALSE;
                }
                if (fstricmp(PszGetCompressedString(idsGater), lpshdefBuild->hul.szClass) != 0) {
                    TutorError(idsTutorialNameDesignEditboxMustGaterChange);
                    return FALSE;
                }
                if (lpshdefBuild->hul.ibmp == 137)
                    break;
                TutorError(idsTutorialHaventPickedCorrectImageDesignPress);
                return FALSE;
            }
            TutorError(idsTutorialHaventYetFinishedCreatingNewDesign);
            return FALSE;
        case 22:
            if (tutor.idt == idtAddTeamsterStoveTopsQueue) {
                hs.grhst = hstEngine;
                hs.iItem = 4;
                hs.cItem = 1;
                hs1.grhst = hstMines;
                hs1.iItem = 1;
                hs1.cItem = 3;
                if (FCheckBuilderPart(0, &hs, 1) == 0 || FCheckBuilderPart(2, &hs1, 3) == 0) {
                    TutorError(idsTutorialDontHaveRightPartsDesignVerify2);
                    return FALSE;
                }
                if (fstricmp(PszGetCompressedString(idsMineLayer), lpshdefBuild->hul.szClass) == 0)
                    break;
                TutorError(idsTutorialNameDesignEditboxMustMineLayer);
                return FALSE;
            }
            TutorError(idsTutorialHaventYetFinishedCreatingNewDesign);
            return FALSE;
        case 25:
            hs.grhst = hstEngine;
            hs.iItem = 4;
            hs.cItem = 1;
            hs2.grhst = hstSpecialM;
            hs2.iItem = 0;
            hs2.cItem = 1;
            if (FCheckBuilderPart(0, &hs, 1) != 0 && FCheckBuilderPart(1, &hs2, 1) != 0)
                break;
            TutorError(idsTutorialDontHaveRightPartsDesignVerify);
            return FALSE;
        case 27:
            if (tutor.idt == idtWeWantPowerfulWeAlsoWantWeigh) {
                hs.grhst = hstEngine;
                hs.iItem = 10;
                hs.cItem = 1;
                hs1.grhst = hstSpecialM;
                hs1.iItem = 5;
                hs1.cItem = 1;
                hs2.grhst = hstSpecialE;
                hs2.iItem = 5;
                hs2.cItem = 1;
                hs3.grhst = hstBeam;
                hs3.iItem = 3;
                hs3.cItem = 1;
                hs4.grhst = hstArmor;
                hs4.iItem = 2;
                hs4.cItem = 2;
                if (FCheckBuilderPart(0, &hs, 1) == 0 || FCheckBuilderPart(1, &hs3, 1) == 0 || FCheckBuilderPart(2, &hs3, 1) == 0 ||
                    FCheckBuilderPart(3, &hs3, 1) == 0 || FCheckBuilderPart(4, &hs4, 2) == 0 || FCheckBuilderPart(5, &hs1, 1) == 0 ||
                    FCheckBuilderPart(6, &hs2, 1) == 0) {
                    TutorError(idsTutorialVerifyHaveRadiatingHydroRamScoop);
                    return FALSE;
                }
                if (lpshdefBuild->hul.ibmp == 25)
                    break;
                TutorError(idsTutorialHaventPickedCorrectImageDesignPress);
                return FALSE;
            }
            TutorError(idsTutorialHaventYetFinishedCreatingNewDesign);
            return FALSE;
        case 29:
            if (tutor.idt != idtLetsFinishOffBerserkersOnceBuildingBombing) {
                TutorError(idsTutorialHaventYetFinishedCreatingNewDesign);
                return FALSE;
            }
            hs.grhst = hstEngine;
            hs.iItem = 10;
            hs.cItem = 2;
            hs1.grhst = hstSpecialM;
            hs1.iItem = 5;
            hs1.cItem = 1;
            hs2.grhst = hstBomb;
            hs2.iItem = 1;
            hs2.cItem = 4;
            if (FCheckBuilderPart(0, &hs, 2) == 0 || FCheckBuilderPart(1, &hs2, 4) == 0 || FCheckBuilderPart(2, &hs2, 4) == 0 ||
                FCheckBuilderPart(3, &hs1, 1) == 0) {
                TutorError(idsTutorialDontHaveRightPartsDesignVerify3);
                return FALSE;
            }
        }
        return TRUE;
    case tutsbCancelEdit:
        TutorError(idsTutorialMustFinishTutorialTasksBeforeExiting);
        return FALSE;
    }
NoCustom:
    TutorError(idsTutorialShouldCustomizeShipDesignPointTutorial);
    return FALSE;
}
