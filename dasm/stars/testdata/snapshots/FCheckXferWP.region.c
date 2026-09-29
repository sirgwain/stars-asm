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
    if ((id & 0x8000) == 0x0) {
        grobj = grobjPlanet;
    } else {
        id = id & 0x7fff;
        grobj = grobjFleet;
    }
    lpfl = LpflFromId(ifl);
    if (lpfl != 0x0) {
        if (FCheckFleetWP(ifl, iord, grobj, id, 0x1, iWarp) != 0) {
            ord = lpfl->lpplord->rgord[iord];
            piaCur = ord.txp.rgia;
            tutor.idh = 1519;
            i = 0;
            while (1) {
                if (i >= 5)
                    goto L_73f5;
                if (piaCur->iAction != lpiaGoal->iAction)
                    break;
                if ((piaCur->iAction == iActionUnloadExact || piaCur->iAction == iActionSetAmount) && piaCur->cQuan != lpiaGoal->cQuan)
                    goto LReturn;
                i = i + 1;
                piaCur = piaCur + 1;
                lpiaGoal = lpiaGoal + 1;
            }
            if (piaCur->iAction == iActionNone)
                goto LReturn;
            TutorError(616);
            goto LReturn;
        L_73f5:
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
        return 0;
    }
    return 0;
}
