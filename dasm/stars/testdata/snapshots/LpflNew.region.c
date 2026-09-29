FLEET *LpflNew(int16_t iPlr, int16_t idPl) {
    int16_t i;
    ORDER  *lpord;
    FLEET  *lpfl;
    int16_t iflPrev;
    void   *t_call_3123;

    iflPrev = -1;
    for (i = 0; i < cFleet; i++) {
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0x0)
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
    cFleet = cFleet + 1;
    rgplr[iPlr].cFleet = rgplr[iPlr].cFleet + 0x1;
    fmemset(lpfl, 0, sizeof(FLEET));
    lpfl->ifl = iflPrev + 1;
    lpfl->iPlayer = iPlr;
    lpfl->iplr = iPlr;
    lpfl->det = 0x7;
    lpfl->idPlanet = idPl;
    if (idPl != -1) {
        lpfl->pt = rgptPlan[idPl];
    }
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
    if (sel.scan.ifl != -1 && i <= sel.scan.ifl) {
        sel.scan.ifl = sel.scan.ifl + 1;
    }
    gd.fFleetLinkValid = 0x0;
    return lpfl;
}
