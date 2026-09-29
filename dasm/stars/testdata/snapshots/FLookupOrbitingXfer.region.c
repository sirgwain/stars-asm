int16_t FLookupOrbitingXfer(int16_t idPlanet, int16_t iNth, XFER *pxf, int16_t idSkip) {
    int16_t i;
    THING  *lpth;
    FLEET  *lpfl;
    THING  *lpthMac;
    int16_t t_25ad;
    int16_t t_266a;

    if (cFleet > 0) {
        if (cFleet != 0) {
            i = 0;
            while (1) {
                if (i >= cFleet)
                    goto L_2606;
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0x0)
                    goto L_2606;
                if (lpfl->idPlanet == idPlanet && lpfl->id != idSkip && (idSkip == -1 || (lpfl->pt.x == sel.pt.x && lpfl->pt.y == sel.pt.y))) {
                    t_25ad = iNth;
                    iNth = iNth - 1;
                    if (t_25ad == 0)
                        break;
                }
                i = i + 1;
            }
            if (pxf != 0x0) {
                pxf->fl = *lpfl;
                pxf->grobj = grobjFleet;
                pxf->id = lpfl->id;
            }
            return 1;
        }
    L_2606:
        lpth = lpThings;
        lpthMac = lpThings + cThing;
        for (; lpth < lpthMac; lpth++) {
            if (lpth->ith == ithMineralPacket && lpth->pt.x == sel.pt.x && lpth->pt.y == sel.pt.y) {
                t_266a = iNth;
                iNth = iNth - 1;
                if (t_266a == 0)
                    goto L_2679;
            }
        }
        return 0;
    L_2679:
        if (pxf != 0x0) {
            pxf->th = *lpth;
            pxf->grobj = grobjThing;
            pxf->id = lpth->idFull;
        }
        return 1;
    }
    return 0;
}
