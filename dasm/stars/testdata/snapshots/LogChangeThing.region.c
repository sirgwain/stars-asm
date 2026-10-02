void LogChangeThing(THING *lpth, THING *pthNew) {
    int16_t i;
    int16_t fChg;
    LOGXFER lxNew;

    fChg = FALSE;
    if (gd.fGeneratingTurn == 0) {
        memset(&lxNew, 0, sizeof(LOGXFER));
        lxNew.id = pthNew->idFull;
        lxNew.grobj = grobjThing;
        for (i = 0; i < 3; i++) {
            lxNew.rgdItem[i] = (int16_t)(pthNew->thp.rgwtMin[i] - lpth->thp.rgwtMin[i]);
            if ((int16_t)(pthNew->thp.rgwtMin[i] - lpth->thp.rgwtMin[i]) != 0) {
                fChg = TRUE;
            }
        }
        if (fChg != 0) {
            if (fValidLx != 0) {
                LogMakeValidXfer(&lx, &lxNew);
                fValidLx = FALSE;
            } else {
                lx = lxNew;
                fValidLx = TRUE;
            }
        }
    }
    return;
}
