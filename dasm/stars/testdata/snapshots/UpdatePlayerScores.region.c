void UpdatePlayerScores() {
    int32_t  lScoreTot;
    int16_t  cFirst;
    SCORE    score;
    int16_t  cDead;
    int16_t  c;
    int16_t  i;
    uint8_t  rgcCond[16];
    uint16_t wWinners2;
    int32_t  rglScore[16];
    int16_t  iScoreMax;
    int16_t  j;
    uint16_t wWinners;
    int16_t  imsg;
    int32_t  lScore2nd;
    int32_t  lScoreMax;
    int32_t  t_scratch_m88_2;
    int16_t  t_scratch_m86_8;
    int32_t  t_scratch_m88_6;

    cDead = 0;
    cFirst = 0;
    iScoreMax = 0;
    lScore2nd = 0;
    lScoreTot = 0;
    gd.fGameOverMan = FALSE;
    memset(rgcCond, 0, 16);
    for (i = 0; i < game.cPlayer; i++) {
        rglScore[i] = CalcPlayerScore(i, &score);
        vlprgScoreX[i].score = score;
        vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xffe0) | (i & 0x1f);
        vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xffdf) | 0x20;
        vlprgScoreX[i].wWord &= 0xc03f;
        lScoreTot += rglScore[i];
        if (score.cPlanet == 0 && score.rgcsh[0] == 0 && score.rgcsh[1] == 0 && score.rgcsh[2] == 0 && rgplr[i].fDead == 0) {
            rgplr[i].wFlags = (rgplr[i].wFlags & 0xfffe) | 1;
            for (j = 0; j < game.cPlayer; j++) {
                if (j != i) {
                    FSendPrependedPlrMsg(j, idmTracesHaveEliminatedGalaxyMayRestPeace, gotoScore, i | 0x30, 0, 0, 0, 0, 0, 0);
                }
            }
        }
        if (score.cPlanet >= MulDiv(cPlanet, GetVCVal(&game, vcOwnsPercentPlanets, FALSE), 100)) {
            vlprgScoreX[i].grbitVC |= 1;
            if (GetVCCheck(&game, vcOwnsPercentPlanets) != 0) {
                rgcCond[i]++;
            }
        }
        t_scratch_m88_2 = (int32_t)((uint32_t)(score.rgcsh[2] & 0x1fff) << (score.rgcsh[2] >> 0xd << 1));
        if ((int32_t)t_scratch_m88_2 >= GetVCVal(&game, vcOwnsCapitalShips, FALSE)) {
            vlprgScoreX[i].grbitVC |= 0x20;
            if (GetVCCheck(&game, vcOwnsCapitalShips) != 0) {
                rgcCond[i]++;
            }
        }
        if (rglScore[i] >= GetVCVal(&game, vcExceedsScore, FALSE)) {
            vlprgScoreX[i].grbitVC |= 4;
            if (GetVCCheck(&game, vcExceedsScore) != 0) {
                rgcCond[i]++;
            }
        }
        c = 0;
        for (j = 0; j < 6; j++) {
            t_scratch_m86_8 = rgplr[i].rgTech[j];
            if (t_scratch_m86_8 >= GetVCVal(&game, vcAttainsTechLevel, FALSE)) {
                c++;
            }
        }
        if (c >= GetVCVal(&game, vcAttainsTechFields, FALSE)) {
            vlprgScoreX[i].grbitVC |= 2;
            if (GetVCCheck(&game, vcAttainsTechLevel) != 0) {
                rgcCond[i]++;
            }
        }
        t_scratch_m88_6 = (int32_t)(score.cResources / 1000);
        if ((int32_t)t_scratch_m88_6 >= GetVCVal(&game, vcProductionCapacity, FALSE)) {
            vlprgScoreX[i].grbitVC |= 0x10;
            if (GetVCCheck(&game, vcProductionCapacity) != 0) {
                rgcCond[i]++;
            }
        }
    }
    if (game.cPlayer == 1) {
        vlprgScoreX->iRank = 1;
    } else {
        for (i = 0; i < game.cPlayer; i++) {
            if (rgplr[i].fDead != 0) {
                cDead++;
            }
            rgplr[i].wScore = 1;
            for (j = 0; j < game.cPlayer; j++) {
                if (rglScore[j] > rglScore[i]) {
                    rgplr[i].wScore++;
                }
            }
            if (rgplr[i].wScore == 1) {
                iScoreMax = i;
                lScoreMax = rglScore[i];
                cFirst++;
            } else if (rgplr[i].wScore == 2) {
                lScore2nd = rglScore[i];
            }
        }
        if (cFirst > 1) {
            lScore2nd = lScoreMax;
        }
        for (i = 0; i < game.cPlayer; i++) {
            vlprgScoreX[i].turn = rgplr[i].wScore;
        }
        if ((int16_t)game.turn >= GetVCVal(&game, vcHighestScoreAfterYears, FALSE) && cFirst == 1) {
            vlprgScoreX[iScoreMax].grbitVC |= 0x40;
            if (GetVCCheck(&game, vcHighestScoreAfterYears) != 0) {
                rgcCond[iScoreMax]++;
            }
        }
        if (cDead + 1 >= game.cPlayer) {
            gd.fGameOverMan = TRUE;
            if (rgplr[iScoreMax].fDead == 0) {
                FSendPrependedPlrMsg(iScoreMax, idmTracesEveryOtherRivalHaveEliminatedGalaxy, gotoScore, 0, 0, 0, 0, 0, 0, 0);
            }
            for (i = 0; i < game.cPlayer; i++) {
                if (i != iScoreMax) {
                    FSendPrependedPlrMsg(i, idmDeadPlanetsHaveOverrunSpaceshipsDefeated, gotoScore, 0, 0, 0, 0, 0, 0, 0);
                }
            }
        } else {
            if (lScoreMax >= (int32_t)(lScore2nd * (int16_t)(GetVCVal(&game, vcExceedsSecondPlaceBy, FALSE) + 100)) / 100) {
                vlprgScoreX[iScoreMax].grbitVC |= 8;
                if (GetVCCheck(&game, vcExceedsSecondPlaceBy) != 0) {
                    rgcCond[iScoreMax]++;
                }
            }
            if (game.turn >= (uint16_t)GetVCVal(&game, vcMinYearsBeforeWin, FALSE)) {
                wWinners = 0;
                j = GetVCVal(&game, vcMeetsNumCriteria, FALSE);
                if (j >= 1) {
                    for (i = game.cPlayer - 1; i >= 0; i--) {
                        wWinners *= 2;
                        if (rgcCond[i] >= j) {
                            vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xbfff) | 0x4000;
                            wWinners |= 1;
                        }
                    }
                    if (wWinners != 0) {
                        gd.fGameOverMan = TRUE;
                    }
                }
                if (gd.fGameOverMan != 0) {
                    i = 0;
                    j = 1;
                    while (i < game.cPlayer) {
                        wWinners2 = wWinners;
                        if (rgplr[i].fDead != 0) {
                            imsg = 184;
                        } else if ((j & wWinners) == 0) {
                            imsg = 181;
                        } else if ((j ^ wWinners) != 0) {
                            imsg = 183;
                            wWinners2 &= ~j;
                        } else {
                            imsg = 182;
                        }
                        FSendPrependedPlrMsg(i, imsg, gotoScore, wWinners2, 0, 0, 0, 0, 0, 0);
                        i++;
                        j *= 2;
                    }
                }
            }
        }
    }
    return;
}
