void EnsureAis() {
    int16_t fHostSav;
    int16_t fErrSav;
    int16_t fOpened;
    int16_t fWorkDone;
    int16_t fSubmitSav;
    int16_t iPlayer;
    MDPLR   rgmdplr[16];

    fSubmitSav = gd.fSubmit;
    fWorkDone = 0;
    if (gd.fAisDone == 0x0) {
        fHostSav = gd.fHostMode;
        if (gd.fHostMode == 0x0) {
            DestroyCurGame();
            FLoadGame(szBase, "hst");
        }
        for (iPlayer = 0; iPlayer < game.cPlayer; iPlayer++) {
            *(uint16_t *)&rgmdplr[iPlayer] = rgplr[iPlayer].wMdPlr;
        }
        gd.fSubmit = 0x1;
        fErrSav = fFileErrSilent;
        fFileErrSilent = 1;
        for (iPlayer = 0; iPlayer < game.cPlayer; iPlayer++) {
            UpdateProgressGauge(MulDiv(340, iPlayer + 1, game.cPlayer));
            if (rgmdplr[iPlayer].fAi != 0x0) {
                fWorkDone = 1;
                gd.fGeneratingTurn = 0x1;
                gd.fHostMode = 0x1;
                fOpened = FOpenFile(dtLog, iPlayer, 32);
                gd.fGeneratingTurn = 0x0;
                gd.fHostMode = fHostSav;
                if (fOpened == 0) {
                    DoAiTurn(iPlayer, *(uint16_t *)&rgmdplr[iPlayer]);
                } else {
                    StreamClose();
                }
            }
        }
        gd.fSubmit = fSubmitSav;
        if (fWorkDone != 0) {
            DestroyCurGame();
            FLoadGame(szBase, "hst");
        }
        fFileErrSilent = fErrSav;
        gd.fAisDone = 0x1;
    }
    return;
}
