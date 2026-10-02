int16_t FCheckXferWP(uint16_t ifl, int16_t iord, int16_t id, uint16_t iWarp, ITEMACTION *lpiaGoal) {
    ORDER       ord;
    int16_t     fRet;
    ITEMACTION *piaCur;
    int16_t     i;
    FLEET      *lpfl;
    int16_t     idh;
    GrobjClass  grobj;
    int16_t     idhSav;

    fRet = 0;
    idhSav = tutor.idh;
    if ((id & 0x8000) != 0) {
        id &= 0x7fff;
        grobj = grobjFleet;
    } else {
        grobj = grobjPlanet;
    }
    lpfl = LpflFromId(ifl);
    if (lpfl == 0) {
        return 0;
    }
    if (FCheckFleetWP(ifl, iord, grobj, id, grTaskXfer, iWarp) == 0) {
        return 0;
    }
    ord = lpfl->lpplord->rgord[iord];
    piaCur = ord.txp.rgia;
    tutor.idh = idhWaypointTaskTile;
    i = 0;
    while (i < 5) {
        if (piaCur->iAction != lpiaGoal->iAction) {
            if (piaCur->iAction == iActionNone)
                goto LReturn;
            TutorError(idsTutorialHaveGivenIncorrectTransferOrderPlease);
            goto LReturn;
        }
        if ((piaCur->iAction == iActionUnloadExact || piaCur->iAction == iActionSetAmount) && piaCur->cQuan != lpiaGoal->cQuan)
            goto LReturn;
        i++;
        piaCur++;
        lpiaGoal++;
    }
    fRet = 1;
LReturn:
    idh = tutor.idh;
    if (fRet == 0 && FCheckSelection(grobjFleet, ifl) != 0) {
        tutor.idh = idh;
    }
    if (fRet != 0) {
        tutor.idh = idhSav;
    }
    return fRet;
}
