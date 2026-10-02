void EnsureAis() {
    int16_t fHostSav;
    int16_t fErrSav;
    int16_t fOpened;
    int16_t fWorkDone;
    int16_t fSubmitSav;
    int16_t iPlayer;
    MDPLR   rgmdplr[16];

    fSubmitSav = gd.fSubmit;
    fWorkDone = FALSE;
    if (gd.fAisDone == 0) {
        fHostSav = gd.fHostMode;
        if (gd.fHostMode == 0) {
            DestroyCurGame();
            FLoadGame(szBase, "hst");
        }
        for (iPlayer = 0; iPlayer < game.cPlayer; iPlayer++) {
            *(uint16_t *)&rgmdplr[iPlayer] = rgplr[iPlayer].wMdPlr;
        }
        gd.fSubmit = TRUE;
        fErrSav = fFileErrSilent;
        fFileErrSilent = TRUE;
        for (iPlayer = 0; iPlayer < game.cPlayer; iPlayer++) {
            UpdateProgressGauge(MulDiv(340, iPlayer + 1, game.cPlayer));
            if (rgmdplr[iPlayer].fAi != 0) {
                fWorkDone = TRUE;
                gd.fGeneratingTurn = TRUE;
                gd.fHostMode = TRUE;
                fOpened = FOpenFile(dtLog, iPlayer, 32);
                gd.fGeneratingTurn = FALSE;
                gd.fHostMode = fHostSav;
                if (fOpened != 0) {
                    StreamClose();
                } else {
                    DoAiTurn(iPlayer, *(uint16_t *)&rgmdplr[iPlayer]);
                }
            }
        }
        gd.fSubmit = fSubmitSav;
        if (fWorkDone != 0) {
            DestroyCurGame();
            FLoadGame(szBase, "hst");
        }
        fFileErrSilent = fErrSav;
        gd.fAisDone = TRUE;
    }
    return;
}
