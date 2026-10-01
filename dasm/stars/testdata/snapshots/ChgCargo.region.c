int32_t ChgCargo(GrobjClass grobj, int16_t id, int16_t iSupply, int32_t dChg, void *pobj) {
    THING  *pth;
    XFER    xfer;
    int16_t i;
    FLEET  *pfl;
    PLANET *ppl;
    int32_t wtFree;
    int32_t t_merge_6425_0001;

    switch (grobj) {
    case grobjPlanet:
    case grobjOther:
        if (pobj != 0) {
            ppl = pobj;
        } else if (grobj == grobjPlanet) {
            FLookupPlanet(id, &xfer.pl);
            ppl = &xfer.pl;
        } else {
            memset(&xfer.pl, 0, sizeof(PLANET));
            ppl = &xfer.pl;
        }
        if (iSupply <= 4) {
            if (iSupply == 4) {
                return 0;
            }
            if (dChg == 0) {
                return ppl->rgwtMin[iSupply];
            }
            if (ppl->rgwtMin[iSupply] + dChg < 0) {
                dChg = -ppl->rgwtMin[iSupply];
            }
            ppl->rgwtMin[iSupply] += dChg;
        }
        if (dChg == 0 || pobj != 0 || grobj == grobjOther)
            break;
        FLookupPlanet(-1, &xfer.pl);
        break;
    case grobjThing:
        if (pobj != 0) {
            pth = pobj;
        } else {
            FLookupThing(id, &xfer.th);
            pth = &xfer.th;
        }
        if (iSupply >= 3) {
            return 0;
        }
        if (iSupply <= 4) {
            if (dChg == 0) {
                return pth->thp.rgwtMin[iSupply];
            }
            if (pth->thp.rgwtMin[iSupply] + dChg < 0) {
                dChg = (int16_t)-pth->thp.rgwtMin[iSupply];
            }
            wtFree = (uint32_t)(pth->thp.wtMax * 10);
            for (i = 0; i < 3; i++) {
                wtFree -= pth->thp.rgwtMin[i];
            }
            if (dChg > wtFree) {
                dChg = wtFree;
            }
            pth->thp.rgwtMin[iSupply] += LOWORD(dChg);
        }
        if (dChg == 0 || pobj != 0)
            break;
        FLookupThing(-1, pth);
        break;
    default:
        if (pobj != 0) {
            pfl = pobj;
        } else {
            FLookupFleet(id, &xfer.fl);
            pfl = &xfer.fl;
        }
        if (iSupply <= 4) {
            if (dChg == 0) {
                return pfl->rgwtMin[iSupply];
            }
            if (pfl->rgwtMin[iSupply] + dChg < 0) {
                dChg = -pfl->rgwtMin[iSupply];
            }
            if (iSupply == 3 && pfl->det != detAll) {
                dChg = 0;
            }
            t_merge_6425_0001 = iSupply == 4 ? GetFuelFree(pfl) : GetCargoFree(pfl);
            if (dChg >= t_merge_6425_0001) {
                if (iSupply == 4) {
                    dChg = GetFuelFree(pfl);
                } else {
                    dChg = GetCargoFree(pfl);
                }
            }
            pfl->rgwtMin[iSupply] += dChg;
        }
        if (dChg != 0 && pobj == 0) {
            FLookupFleet(-1, pfl);
        }
    }
    return dChg;
}
