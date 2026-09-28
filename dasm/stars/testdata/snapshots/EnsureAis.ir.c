void EnsureAis() {
    int16_t fHostSav;
    int16_t fErrSav;
    int16_t fOpened;
    int16_t fWorkDone;
    int16_t fSubmitSav;
    int16_t iPlayer;
    MDPLR   rgmdplr[16];

L_56bc:
    fSubmitSav = gd.fSubmit;
    fWorkDone = 0;
    if ((gd.fAisDone != 0x0))
        goto L_5893;
    else
        goto L_56f1;

L_56f1:
    fHostSav = gd.fHostMode;
    if ((gd.fHostMode != 0x0))
        goto L_5729;
    else
        goto L_5714;

L_5714:
    DestroyCurGame();
    FLoadGame(szBase, "hst");

L_5729:
    iPlayer = 0;
    goto L_5735;

L_5731:
    iPlayer = (iPlayer + 1);

L_5735:
    if ((iPlayer >= game.cPlayer))
        goto L_575d;
    else
        goto L_5740;

L_5740:
    *((uint16_t *)(&(rgmdplr[iPlayer]))) = rgplr[iPlayer].wMdPlr;
    goto L_5731;

L_575d:
    gd.fSubmit = 0x1;
    fErrSav = fFileErrSilent;
    fFileErrSilent = 1;
    iPlayer = 0;
    goto L_5781;

L_577d:
    iPlayer = (iPlayer + 1);

L_5781:
    if ((iPlayer >= game.cPlayer))
        goto L_5848;
    else
        goto L_578c;

L_578c:
    UpdateProgressGauge(MulDiv(340, (iPlayer + 1), game.cPlayer));
    if ((rgmdplr[iPlayer].fAi == 0x0))
        goto L_577d;
    else
        goto L_57c5;

L_57c5:
    fWorkDone = 1;
    gd.fGeneratingTurn = 0x1;
    gd.fHostMode = 0x1;
    fOpened = FOpenFile(dtLog, iPlayer, 32);
    gd.fGeneratingTurn = 0x0;
    gd.fHostMode = fHostSav;
    if ((fOpened == 0))
        goto L_582e;
    else
        goto L_5826;

L_5826:
    StreamClose();
    goto L_577d;

L_582e:
    DoAiTurn(iPlayer, *((uint16_t *)(&(rgmdplr[iPlayer]))));

L_5845:
    goto L_577d;

L_5848:
    gd.fSubmit = fSubmitSav;
    if ((fWorkDone == 0))
        goto L_5881;
    else
        goto L_586c;

L_586c:
    DestroyCurGame();
    FLoadGame(szBase, "hst");

L_5881:
    fFileErrSilent = fErrSav;
    gd.fAisDone = 0x1;

L_5893:
    return;
}
