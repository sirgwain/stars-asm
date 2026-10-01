FLEET *LpflNew(int16_t iPlr, int16_t idPl) {
    int16_t i;
    ORDER  *lpord;
    FLEET  *lpfl;
    int16_t iflPrev;
    void   *t_call_3123;

    iflPrev = -1;
    for (i = 0; i < cFleet; i++) {
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0)
            break;
        if (lpfl->iPlayer >= iPlr) {
            if (lpfl->iPlayer > iPlr || lpfl->ifl != iflPrev + 1)
                break;
            iflPrev = lpfl->ifl;
        }
    }
    rglpfl = LpReAlloc(rglpfl, (cFleet + 1) * sizeof(FLEET *), htMisc);
    if (cFleet != i) {
        fmemmove(rglpfl + (i + 1), rglpfl + i, (cFleet - i) * sizeof(FLEET *));
    }
    t_call_3123 = LpAlloc(sizeof(FLEET), htFleets);
    lpfl = t_call_3123;
    rglpfl[i] = t_call_3123;
    cFleet++;
    rgplr[iPlr].cFleet++;
    fmemset(lpfl, 0, sizeof(FLEET));
    lpfl->ifl = iflPrev + 1;
    lpfl->iPlayer = iPlr;
    lpfl->iplr = iPlr;
    lpfl->det = detAll;
    lpfl->idPlanet = idPl;
    if (idPl != -1) {
        lpfl->pt = rgptPlan[idPl];
    }
    lpfl->cord = 1;
    lpfl->fRepOrders = 0;
    lpfl->lpplord = (PLORD *)LpplAlloc(18, 3, htOrd);
    lpfl->lpplord->iordMac = 1;
    lpfl->fdirValid = 0;
    lpord = lpfl->lpplord->rgord;
    lpord->pt = lpfl->pt;
    lpord->id = lpfl->idPlanet;
    lpord->grobj = lpfl->idPlanet == -1 ? 4 : 1;
    lpord->iWarp = 0;
    lpord->fValidTask = 1;
    lpord->grTask = grTaskNone;
    if (sel.scan.ifl != -1 && i <= sel.scan.ifl) {
        sel.scan.ifl++;
    }
    gd.fFleetLinkValid = 0;
    return lpfl;
}
