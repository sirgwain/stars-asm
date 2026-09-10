void LogChangeThing(THING *lpth, THING *pthNew) {
    int16_t i;
    int16_t fChg;
    LOGXFER lxNew;

L_9908:
    fChg = 0;
    if ((gd.fGeneratingTurn != 0x0))
        goto L_99f0;
    else
        goto L_9929;

L_9929:
    memset(&(lxNew), 0, 0x18);
    lxNew.id = pthNew->idFull;
    lxNew.grobj = grobjThing;
    i = 0;
    goto L_99a5;

L_9952:
    LOWORD(lxNew.rgdItem[i]) = (pthNew->thp.rgwtMin[i] - lpth->thp.rgwtMin[i]);
    HIWORD(lxNew.rgdItem[i]) = SIGNHIWORD((pthNew->thp.rgwtMin[i] - lpth->thp.rgwtMin[i]));
    if (((pthNew->thp.rgwtMin[i] - lpth->thp.rgwtMin[i]) != 0))
        goto L_999c;
    else
        goto L_9994;

L_9994:
    if ((SIGNHIWORD((pthNew->thp.rgwtMin[i] - lpth->thp.rgwtMin[i])) == 0x0))
        goto L_99a1;
    else
        goto L_999c;

L_999c:
    fChg = 1;

L_99a1:
    i = (i + 1);

L_99a5:
    if ((i < 3))
        goto L_9952;
    else
        goto L_99ae;

L_99ae:
    if ((fChg == 0))
        goto L_99f0;
    else
        goto L_99b7;

L_99b7:
    if ((fValidLx == 0))
        goto L_99da;
    else
        goto L_99c1;

L_99c1:
    LogMakeValidXfer(lx.id, &(lxNew));
    fValidLx = 0;
    goto L_99f0;

L_99da:
    lx = lxNew;
    fValidLx = 1;

L_99f0:
    return;
}
