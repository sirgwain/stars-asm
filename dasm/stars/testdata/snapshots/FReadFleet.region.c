int16_t FReadFleet(FLEET *lpfl) {
    uint16_t  us;
    int16_t   cord;
    int16_t   fByte;
    ORDER    *lpord;
    int16_t   i;
    int16_t   cish;
    uint8_t  *pb;
    int16_t   cch;
    uint16_t *pus;
    char      szT[33];
    int16_t   cOut;

    cish = 0;
    fmemset(lpfl, 0, sizeof(FLEET));
    fmemmove(lpfl, rgbCur, 12);
    fByte = lpfl->fDone;
    us = RawLoad16(&rgbCur[12]);
    pb = &rgbCur[14];
    if (fByte != 0) {
        i = 0;
        for (; us != 0; us >>= 1) {
            if ((us & 1) != 0) {
                lpfl->rgcsh[i] = *pb++;
                if (lpfl->rgcsh[i] != 0) {
                    cish++;
                }
            }
            i++;
        }
    } else {
        pus = (uint16_t *)pb;
        i = 0;
        for (; us != 0; us >>= 1) {
            if ((us & 1) != 0) {
                lpfl->rgcsh[i] = *pus++;
                if (lpfl->rgcsh[i] != 0) {
                    cish++;
                }
            }
            i++;
        }
        pb = (uint8_t *)pus;
    }
    if (cish == 0) {
        lpfl->fDead = TRUE;
    }
    if (lpfl->det >= detMore) {
        us = RawLoad16(pb);
        pb += 2;
        i = 0;
        while (i < 5) {
            switch (us & 3) {
            default:
                break;
            case 1:
                lpfl->rgwtMin[i] = (uint32_t)*pb;
                pb++;
                break;
            case 2:
                lpfl->rgwtMin[i] = (uint32_t)RawLoad16(pb);
                pb += 2;
                break;
            case 3:
                lpfl->rgwtMin[i] = RawLoad32(pb);
                pb += 4;
            }
            i++;
            us >>= 2;
        }
    }
    if (lpfl->det < detAll) {
        lpfl->dirLong = RawLoad32(pb);
        pb += 4;
        lpfl->wtFleet = RawLoad32(pb);
        pb += 4;
        ReadRt();
        return TRUE;
    }
    if (hdrCur.rt == rtFleetA) {
        us = RawLoad16(pb);
        pb += 2;
        pus = (uint16_t *)pb;
        i = 0;
        for (; us != 0; us >>= 1) {
            if ((us & 1) != 0) {
                lpfl->rgdv[i].dp = *pus++;
                if (lpfl->rgdv[i].pctDp >= 500) {
                    lpfl->rgdv[i].pctDp = 499;
                }
            }
            i++;
        }
        pb = (uint8_t *)pus;
        lpfl->iplan = *pb++;
        lpfl->cord = *pb++;
        lpfl->lpplord = (PLORD *)LpplAlloc(18, lpfl->cord + 1, htOrd);
        fmemset(lpfl->lpplord->rgord, 0, (lpfl->cord + 1) * 18);
        cord = lpfl->cord;
        lpord = lpfl->lpplord->rgord;
        for (; cord != 0; cord--) {
            memset(rgbCur, 0, 18);
            ReadRt();
            if (hdrCur.rt != rtOrderA && hdrCur.rt != rtOrderB)
                goto Corrupt;
            *lpord = *(ORDER *)rgbCur;
            lpord->fNoAutoTrack = FALSE;
            lpord++;
        }
        lpfl->lpplord->iordMac = lpfl->cord;
        if (lpfl->idPlanet != -1) {
            if (lpfl->idPlanet > game.cPlanMax) {
                lpfl->idPlanet = -1;
            }
            if (lpfl->pt.x != rgptPlan[lpfl->idPlanet].x || lpfl->pt.y != rgptPlan[lpfl->idPlanet].y) {
                if (i != 0 || game.turn != 0)
                    goto Corrupt;
                lpfl->pt = rgptPlan[lpfl->idPlanet];
            }
        }
        ReadRt();
        if (hdrCur.rt == rtString) {
            cch = rgbCur[0];
            if (cch == 0) {
                lpfl->lpszName = LpAlloc(strlen(&rgbCur[1]) + 1, htString);
                fstrcpy(lpfl->lpszName, &rgbCur[1]);
            } else {
                cOut = 32;
                FDecompressUserString(&rgbCur[1], cch, szT, &cOut);
                lpfl->lpszName = LpAlloc(strlen(szT) + 1, htString);
                fstrcpy(lpfl->lpszName, szT);
            }
            ReadRt();
        } else {
            lpfl->lpszName = NULL;
        }
        return TRUE;
    }
Corrupt:
    AlertSz(PszFormatIds(idsGameFileAppearsCorruptUnableLoadFile, NULL), MB_ICONHAND);
    return FALSE;
}
