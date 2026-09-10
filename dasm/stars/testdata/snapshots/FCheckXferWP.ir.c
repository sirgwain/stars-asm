int16_t FCheckXferWP(uint16_t ifl, int16_t iord, int16_t id, uint16_t iWarp, ITEMACTION *lpiaGoal) {
    ORDER       ord;
    int16_t     fRet;
    ITEMACTION *piaCur;
    int16_t     i;
    FLEET      *lpfl;
    int16_t     idh;
    GrobjClass  grobj;
    int16_t     idhSav;

L_7280:
    fRet = 0;
    idhSav = tutor.idh;
    if (((id & 0x8000) == 0x0))
        goto L_72af;
    else
        goto L_72a2;

L_72a2:
    id = (id & 0x7fff);
    grobj = grobjFleet;
    goto L_72b4;

L_72af:
    grobj = grobjPlanet;

L_72b4:
    lpfl = LpflFromId(ifl);
    if ((LOWORD(lpfl) != 0x0))
        goto L_72de;
    else
        goto L_72cf;

L_72cf:
    if ((HIWORD(lpfl) != 0x0))
        goto L_72de;
    else
        goto L_72d8;

L_72d8:
    return 0;

L_72de:
    if ((FCheckFleetWP(ifl, iord, grobj, id, 0x1, iWarp) != 0))
        goto L_7308;
    else
        goto L_7302;

L_7302:
    return 0;

L_7308:
    ord = lpfl->lpplord->rgord[iord];
    piaCur = ord.txp.rgia;
    tutor.idh = 1519;
    i = 0;
    goto L_7361;

L_7347:
    i = (i + 1);
    piaCur = (piaCur + 1);
    lpiaGoal = (lpiaGoal + 1);

L_7361:
    if ((i >= 5))
        goto L_73f5;
    else
        goto L_736a;

L_736a:
    if ((piaCur->iAction == lpiaGoal->iAction))
        goto L_73b0;
    else
        goto L_738c;

L_738c:
    if ((piaCur->iAction == iActionNone))
        goto LReturn;
    else
        goto L_73a1;

L_73a1:
    TutorError(616);

L_73ad:
    goto LReturn;

L_73b0:
    goto L_73df;

L_73c0:
    if ((piaCur->cQuan != lpiaGoal->cQuan))
        goto LReturn;
    else
        goto L_73d6;

L_73d6:
    goto L_7347;

L_73df:
    if ((piaCur->iAction == iActionUnloadExact))
        goto L_73c0;
    else
        goto L_73e7;

L_73e7:
    if ((piaCur->iAction != iActionSetAmount))
        goto L_7347;
    else
        goto L_73ec;

L_73ec:
    goto L_73c0;

L_73f5:
    fRet = 1;

LReturn:
    idh = tutor.idh;
    if ((fRet != 0))
        goto L_7427;
    else
        goto L_7409;

L_7409:
    if ((FCheckSelection(grobjFleet, ifl) == 0))
        goto L_7427;
    else
        goto L_7421;

L_7421:
    tutor.idh = idh;

L_7427:
    if ((fRet == 0))
        goto L_7436;
    else
        goto L_7430;

L_7430:
    tutor.idh = idhSav;

L_7436:
    return fRet;
}
