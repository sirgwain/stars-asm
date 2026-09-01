int16_t FLookupOrbitingXfer(int16_t idPlanet, int16_t iNth, XFER *pxf, int16_t idSkip) {
    int16_t i;
    THING  *lpth;
    FLEET  *lpfl;
    THING  *lpthMac;
    int16_t t_25ad;
    int16_t t_266a;

L_24fa:
    if ((cFleet > 0))
        goto L_2513;
    else
        goto L_250d;

L_250d:
    return 0x0;

L_2513:
    if ((cFleet == 0))
        goto L_2606;
    else
        goto L_251d;

L_251d:
    i = 0;
    goto L_2529;

L_2525:
    i = (i + 1);

L_2529:
    if ((i >= cFleet))
        goto L_2606;
    else
        goto L_2534;

L_2534:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_2564;
    else
        goto L_255c;

L_255c:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_2606;
    else
        goto L_2564;

L_2564:
    if ((lpfl->idPlanet != idPlanet))
        goto L_2525;
    else
        goto L_2573;

L_2573:
    if ((lpfl->id == idSkip))
        goto L_2525;
    else
        goto L_2581;

L_2581:
    if ((idSkip == -1))
        goto L_25ad;
    else
        goto L_258a;

L_258a:
    if ((lpfl->pt.x != sel.pt.x))
        goto L_2525;
    else
        goto L_25a4;

L_25a4:
    if ((lpfl->pt.y != sel.pt.y))
        goto L_2525;
    else
        goto L_25ad;

L_25ad:
    t_25ad = iNth;
    iNth = (iNth - 1);
    if ((t_25ad != 0))
        goto L_2525;
    else
        goto L_25bc;

L_25bc:
    if ((pxf == 0x0))
        goto L_25fd;
    else
        goto L_25c5;

L_25c5:
    pxf->fl = *(lpfl);
    pxf->grobj = grobjFleet;
    pxf->id = lpfl->id;

L_25fd:
    return 0x1;

L_2606:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_26c4;

L_2631:
    if ((lpth->ith != ithMineralPacket))
        goto L_26c0;
    else
        goto L_2647;

L_2647:
    if ((lpth->pt.x != sel.pt.x))
        goto L_26c0;
    else
        goto L_2661;

L_2661:
    if ((lpth->pt.y != sel.pt.y))
        goto L_26c0;
    else
        goto L_266a;

L_266a:
    t_266a = iNth;
    iNth = (iNth - 1);
    if ((t_266a != 0))
        goto L_26c0;
    else
        goto L_2679;

L_2679:
    if ((pxf == 0x0))
        goto L_26ba;
    else
        goto L_2682;

L_2682:
    pxf->th = *(lpth);
    pxf->grobj = grobjThing;
    pxf->id = lpth->idFull;

L_26ba:
    return 0x1;

L_26c0:
    lpth = (lpth + 0x1);

L_26c4:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_2631;
    else
        goto L_26d2;

L_26d2:
    return 0x0;
}
