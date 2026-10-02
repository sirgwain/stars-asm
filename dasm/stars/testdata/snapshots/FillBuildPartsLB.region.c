void FillBuildPartsLB(HWND hwndLB, int16_t grbit) {
    int16_t      mdAvail;
    int16_t      i;
    char         sz[200];
    HullSlotType grbitCur;
    PART         part;

    grbitCur = hstEngine;
    sz[0] = 'A';
    SendMessage(hwndLB, LB_RESETCONTENT, 0, 0);
    while (grbitCur != hstNone) {
        if ((grbitCur & grbit) != 0) {
            i = 0;
            part.hs.grhst = grbitCur;
            while (1) {
                part.hs.iItem = i;
                mdAvail = FLookupPart(&part);
                if (mdAvail == 0)
                    break;
                if (fStarbaseMode != 0 && grbitCur == hstSpecialE && (i == 15 || i == 16)) {
                    mdAvail = -1;
                }
                if (mdAvail == 1) {
                    sz[1] = i + 'A';
                    sz[2] = part.pcom->ibmp % 26 + 'A';
                    sz[3] = part.pcom->ibmp / 26 + 'A';
                    fstrcpy(&sz[4], part.pcom->szName);
                    SendMessage(hwndLB, LB_ADDSTRING, 0, (LPARAM)sz);
                }
                i++;
            }
        }
        grbitCur *= 2;
        sz[0]++;
    }
    return;
}
