int16_t FLookupOrbitingXfer(int16_t idPlanet, int16_t iNth, XFER *pxf, int16_t idSkip) {
    int16_t i;
    THING  *lpth;
    FLEET  *lpfl;
    THING  *lpthMac;

    if (cFleet <= 0) {
        return FALSE;
    }
    if (cFleet != 0) {
        for (i = 0; i < cFleet; i++) {
            lpfl = rglpfl[i];
            if (rglpfl[i] == 0)
                break;
            if (lpfl->idPlanet == idPlanet && lpfl->id != idSkip && ((idSkip == -1 || (lpfl->pt.x == sel.pt.x && lpfl->pt.y == sel.pt.y)) && iNth-- == 0)) {
                if (pxf != 0) {
                    pxf->fl = *lpfl;
                    pxf->grobj = grobjFleet;
                    pxf->id = lpfl->id;
                }
                return TRUE;
            }
        }
    }
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (lpth->ith == ithMineralPacket && lpth->pt.x == sel.pt.x && lpth->pt.y == sel.pt.y && iNth-- == 0) {
            if (pxf != 0) {
                pxf->th = *lpth;
                pxf->grobj = grobjThing;
                pxf->id = lpth->idFull;
            }
            return TRUE;
        }
    }
    return FALSE;
}
