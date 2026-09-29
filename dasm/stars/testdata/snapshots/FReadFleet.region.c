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
    uint8_t  *t_3ae5;
    uint16_t *t_3b64;
    uint16_t *t_3d91;
    uint8_t  *t_3e15;
    uint8_t  *t_3e25;

    cish = 0;
    fmemset(lpfl, 0, sizeof(FLEET));
    fmemmove(lpfl, rgbCur, 0xc);
    fByte = lpfl->fDone;
    us = RawLoad16(&rgbCur[12]);
    pb = &rgbCur[14];
    if (fByte == 0) {
        pus = (uint16_t *)pb;
        i = 0;
        for (; us != 0x0; us = us >> 0x1) {
            if ((us & 0x1) != 0x0) {
                t_3b64 = pus;
                pus = pus + 1;
                lpfl->rgcsh[i] = *t_3b64;
                if (lpfl->rgcsh[i] != 0) {
                    cish = cish + 1;
                }
            }
            i = i + 1;
        }
        pb = (uint8_t *)pus;
    } else {
        i = 0;
        for (; us != 0x0; us = us >> 0x1) {
            if ((us & 0x1) != 0x0) {
                t_3ae5 = pb;
                pb = pb + 1;
                lpfl->rgcsh[i] = *t_3ae5;
                if (lpfl->rgcsh[i] != 0) {
                    cish = cish + 1;
                }
            }
            i = i + 1;
        }
    }
    if (cish == 0) {
        lpfl->fDead = 0x1;
    }
    if (lpfl->det >= 0x4) {
        us = RawLoad16(pb);
        pb = pb + 2;
        i = 0;
        while (i < 5) {
            switch (us & 0x3) {
            default:
                break;
            case 0x1:
                lpfl->rgwtMin[i] = (uint32_t)*pb;
                pb = pb + 1;
                break;
            case 0x2:
                lpfl->rgwtMin[i] = (uint32_t)RawLoad16(pb);
                pb = pb + 2;
                break;
            case 0x3:
                lpfl->rgwtMin[i] = RawLoad32(pb);
                pb = pb + 4;
            }
            i = i + 1;
            us = us >> 0x2;
        }
    }
    if (lpfl->det >= 0x7) {
        if (hdrCur.rt == rtFleetA) {
            us = RawLoad16(pb);
            pb = pb + 2;
            pus = (uint16_t *)pb;
            i = 0;
            for (; us != 0x0; us = us >> 0x1) {
                if ((us & 0x1) != 0x0) {
                    t_3d91 = pus;
                    pus = pus + 1;
                    lpfl->rgdv[i].dp = *t_3d91;
                    if (lpfl->rgdv[i].pctDp >= 0x1f4) {
                        lpfl->rgdv[i].pctDp = 0x1f3;
                    }
                }
                i = i + 1;
            }
            pb = (uint8_t *)pus;
            t_3e15 = pb;
            pb = pb + 1;
            lpfl->iplan = *t_3e15;
            t_3e25 = pb;
            pb = pb + 1;
            lpfl->cord = *t_3e25;
            lpfl->lpplord = (PLORD *)LpplAlloc(0x12, lpfl->cord + 1, htOrd);
            fmemset(lpfl->lpplord->rgord, 0, (lpfl->cord + 1) * 18);
            cord = lpfl->cord;
            lpord = lpfl->lpplord->rgord;
            for (; cord != 0; cord--) {
                memset(rgbCur, 0, 0x12);
                ReadRt();
                if (hdrCur.rt != rtOrderA && hdrCur.rt != rtOrderB)
                    goto Corrupt;
                *lpord = *(ORDER *)rgbCur;
                lpord->fNoAutoTrack = 0x0;
                lpord = lpord + 1;
            }
            lpfl->lpplord->iordMac = LOBYTE(lpfl->cord);
            if (lpfl->idPlanet != -1) {
                if (lpfl->idPlanet > game.cPlanMax) {
                    lpfl->idPlanet = -1;
                }
                if (lpfl->pt.x != rgptPlan[lpfl->idPlanet].x || lpfl->pt.y != rgptPlan[lpfl->idPlanet].y) {
                    if (i != 0 || game.turn != 0x0)
                        goto Corrupt;
                    lpfl->pt = rgptPlan[lpfl->idPlanet];
                }
            }
            ReadRt();
            if (hdrCur.rt != rtString) {
                lpfl->lpszName = 0x0;
            } else {
                cch = (int16_t)rgbCur[0];
                if (cch != 0) {
                    cOut = 32;
                    FDecompressUserString(&rgbCur[1], cch, szT, &cOut);
                    lpfl->lpszName = LpAlloc(strlen(szT) + 0x1, htString);
                    fstrcpy(lpfl->lpszName, szT);
                } else {
                    lpfl->lpszName = LpAlloc(strlen(&rgbCur[1]) + 0x1, htString);
                    fstrcpy(lpfl->lpszName, &rgbCur[1]);
                }
                ReadRt();
            }
            return 1;
        }
    Corrupt:
        AlertSz(PszFormatIds(idsGameFileAppearsCorruptUnableLoadFile, 0x0), MB_ICONHAND);
        return 0;
    }
    lpfl->dirLong = RawLoad32(pb);
    pb = pb + 4;
    lpfl->wtFleet = RawLoad32(pb);
    pb = pb + 4;
    ReadRt();
    return 1;
}
