int32_t ChgCargo(GrobjClass grobj, int16_t id, int16_t iSupply, int32_t dChg, void *pobj) {
    THING  *pth;
    XFER    xfer;
    int16_t i;
    FLEET  *pfl;
    PLANET *ppl;
    int32_t wtFree;
    int32_t t_call_640a;
    int32_t t_merge_6425_0001;
    int32_t t_call_641d;
    int32_t t_merge_646f_0001;
    int32_t t_call_6454;
    int32_t t_call_6467;

    switch (grobj) {
    case grobjPlanet:
    case grobjOther:
        if (pobj == 0x0) {
            if (grobj != grobjPlanet) {
                memset(&xfer.pl, 0, sizeof(PLANET));
                ppl = &xfer.pl;
            } else {
                FLookupPlanet(id, &xfer.pl);
                ppl = &xfer.pl;
            }
        } else {
            ppl = pobj;
        }
        if (iSupply <= 4) {
            if (iSupply == 4) {
                return 0;
            }
            if (dChg == 0) {
                return ppl->rgwtMin[iSupply];
            }
            if (ppl->rgwtMin[iSupply] + dChg < 0x0) {
                dChg = -ppl->rgwtMin[iSupply];
            }
            ppl->rgwtMin[iSupply] = ppl->rgwtMin[iSupply] + dChg;
        }
        if (dChg == 0 || pobj != 0x0 || grobj == grobjOther)
            break;
        FLookupPlanet(-1, &xfer.pl);
        break;
    case grobjThing:
        if (pobj == 0x0) {
            FLookupThing(id, &xfer.th);
            pth = &xfer.th;
        } else {
            pth = pobj;
        }
        if (iSupply < 3) {
            if (iSupply <= 4) {
                if (dChg == 0) {
                    return (int32_t)pth->thp.rgwtMin[iSupply];
                }
                if ((int32_t)pth->thp.rgwtMin[iSupply] + dChg < 0x0) {
                    dChg = (int32_t)-pth->thp.rgwtMin[iSupply];
                }
                wtFree = (uint32_t)(pth->thp.wtMax * 0xa);
                for (i = 0; i < 3; i++) {
                    wtFree = wtFree - (int32_t)pth->thp.rgwtMin[i];
                }
                if (dChg > wtFree) {
                    dChg = wtFree;
                }
                pth->thp.rgwtMin[iSupply] = pth->thp.rgwtMin[iSupply] + LOWORD(dChg);
            }
            if (dChg == 0 || pobj != 0x0)
                break;
            FLookupThing(-1, pth);
            break;
        }
        return 0;
    default:
        if (pobj == 0x0) {
            FLookupFleet(id, &xfer.fl);
            pfl = &xfer.fl;
        } else {
            pfl = pobj;
        }
        if (iSupply <= 4) {
            if (dChg == 0) {
                return pfl->rgwtMin[iSupply];
            }
            if (pfl->rgwtMin[iSupply] + dChg < 0x0) {
                dChg = -pfl->rgwtMin[iSupply];
            }
            if (iSupply == 3 && pfl->det != 0x7) {
                dChg = 0;
            }
            if (iSupply != 4) {
                t_call_641d = GetCargoFree(pfl);
                t_merge_6425_0001 = t_call_641d;
            } else {
                t_call_640a = GetFuelFree(pfl);
                t_merge_6425_0001 = t_call_640a;
            }
            if (dChg < t_merge_6425_0001) {
                t_merge_646f_0001 = dChg;
            } else if (iSupply != 4) {
                t_call_6467 = GetCargoFree(pfl);
                t_merge_646f_0001 = t_call_6467;
            } else {
                t_call_6454 = GetFuelFree(pfl);
                t_merge_646f_0001 = t_call_6454;
            }
            dChg = t_merge_646f_0001;
            pfl->rgwtMin[iSupply] = pfl->rgwtMin[iSupply] + dChg;
        }
        if (dChg != 0 && pobj == 0x0) {
            FLookupFleet(-1, pfl);
        }
    }
    return dChg;
}
