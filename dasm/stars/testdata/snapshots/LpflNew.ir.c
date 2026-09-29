FLEET *LpflNew(int16_t iPlr, int16_t idPl) {
    int16_t i;
    ORDER  *lpord;
    FLEET  *lpfl;
    int16_t iflPrev;
    void   *t_call_3123;

L_300c:
    iflPrev = -1;
    i = 0;
    goto L_3026;

L_3022:
    i = i + 1;

L_3026:
    if (i >= cFleet)
        goto L_30ad;
    else
        goto L_3031;

L_3031:
    lpfl = rglpfl[i];
    if (rglpfl[i] != 0x0)
        goto L_3061;
    else
        goto L_30ad;

L_3061:
    if (lpfl->iPlayer < iPlr)
        goto L_3022;
    else
        goto L_3073;

L_3073:
    if (lpfl->iPlayer > iPlr)
        goto L_30ad;
    else
        goto L_3085;

L_3085:
    if (lpfl->ifl != iflPrev + 1)
        goto L_30ad;
    else
        goto L_309e;

L_309e:
    iflPrev = lpfl->ifl;
    goto L_3022;

L_30ad:
    rglpfl = LpReAlloc(rglpfl, (cFleet + 1) * sizeof(FLEET *), htMisc);
    if (cFleet == i)
        goto L_311b;
    else
        goto L_30df;

L_30df:
    fmemmove(rglpfl + (i + 1), rglpfl + i, (cFleet - i) * sizeof(FLEET *));

L_311b:
    t_call_3123 = LpAlloc(sizeof(FLEET), htFleets);
    lpfl = t_call_3123;
    rglpfl[i] = t_call_3123;
    cFleet = cFleet + 1;
    rgplr[iPlr].cFleet = rgplr[iPlr].cFleet + 0x1;
    fmemset(lpfl, 0, sizeof(FLEET));
    lpfl->ifl = iflPrev + 1;
    lpfl->iPlayer = iPlr;
    lpfl->iplr = iPlr;
    lpfl->det = 0x7;
    lpfl->idPlanet = idPl;
    if (idPl == -1)
        goto L_3231;
    else
        goto L_3217;

L_3217:
    lpfl->pt = rgptPlan[idPl];

L_3231:
    lpfl->cord = 1;
    lpfl->fRepOrders = 0x0;
    lpfl->lpplord = (PLORD *)LpplAlloc(0x12, 0x3, htOrd);
    lpfl->lpplord->iordMac = 0x1;
    lpfl->fdirValid = 0x0;
    lpord = lpfl->lpplord->rgord;
    lpord->pt = lpfl->pt;
    lpord->id = lpfl->idPlanet;
    lpord->grobj = lpfl->idPlanet == -1 ? 0x4 : 0x1;
    lpord->iWarp = 0x0;
    lpord->fValidTask = 0x1;
    lpord->grTask = grTaskNone;
    if (sel.scan.ifl == -1)
        goto L_3356;
    else
        goto L_3346;

L_3346:
    if (i > sel.scan.ifl)
        goto L_3356;
    else
        goto L_3351;

L_3351:
    sel.scan.ifl = sel.scan.ifl + 1;

L_3356:
    gd.fFleetLinkValid = 0x0;

L_336b:
    return lpfl;
}
