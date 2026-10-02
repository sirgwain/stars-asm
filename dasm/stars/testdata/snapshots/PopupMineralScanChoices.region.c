void PopupMineralScanChoices(HWND hwnd, int16_t x, int16_t y) {
    int16_t fSep;
    int16_t id;
    int16_t fOurs;
    PLANET *lppl;
    int16_t i;
    int16_t c;
    THING  *lpth;
    FLEET  *lpfl;
    THING  *lpthMac;
    int32_t rgid[100];
    int16_t idNew;
    int16_t iChecked;
    SCAN    scan;

    iChecked = -1;
    if (sel.scan.idpl != -1) {
        rgid[0] = sel.scan.idpl;
        rgid[1] = -1;
        c = 2;
        if (sel.scan.grobj == grobjPlanet) {
            iChecked = 0;
        }
    } else {
        c = 0;
    }
    for (i = 0; i < cFleet; i++) {
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0)
            break;
        if (sel.scan.pt.x == lpfl->pt.x && sel.scan.pt.y == lpfl->pt.y) {
            if (sel.scan.grobj == grobjFleet && rglpfl[sel.scan.ifl]->id == lpfl->id) {
                iChecked = c;
            }
            rgid[c++] = lpfl->id | 0x80000000;
            if (c >= 100)
                break;
        }
    }
    if (c == 2 && sel.scan.idpl != -1) {
        c = 1;
    }
    fSep = c == 0;
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (sel.scan.pt.x == lpth->pt.x && sel.scan.pt.y == lpth->pt.y) {
            if (fSep == 0) {
                rgid[c++] = -1;
                fSep = TRUE;
            }
            if (sel.scan.grobj == grobjThing && (int16_t)((uint8_t *)lpth - (uint8_t *)lpThings) / 18 == sel.scan.ith) {
                iChecked = c;
            }
            rgid[c++] = (uint32_t)(uint16_t)lpth->idFull | 0x20000000;
        }
    }
    i = PopupMenu(hwnd, x, y, c, rgid, NULL, iChecked, TRUE);
    if (i >= 0) {
        scan = sel.scan;
        if ((rgid[i] & 0x80000000) != 0) {
            scan.grobj = grobjFleet;
            id = LOWORD(rgid[i]);
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0 || lpfl->id == id)
                    break;
            }
            scan.ifl = i;
            idNew = id;
            fOurs = lpfl->iPlayer == idPlayer;
        } else if ((rgid[i] & 0x20000000) != 0) {
            scan.grobj = grobjThing;
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac && lpth->idFull != LOWORD(rgid[i]); lpth++) {
            }
            scan.ith = (int16_t)((uint8_t *)lpth - (uint8_t *)lpThings) / 18;
            idNew = lpth->idFull;
            fOurs = FALSE;
        } else {
            scan.grobj = grobjPlanet;
            idNew = sel.scan.idpl;
            lppl = LpplFromId(idNew);
            fOurs = lppl->iPlayer == idPlayer;
        }
        ChangeScanSel(&scan, 2);
        if (fOurs != 0) {
            RedrawScanSel(NULL, 0);
            ChangeMainObjSel(scan.grobj, idNew);
            RedrawScanSel(NULL, 1);
        }
    }
    return;
}
