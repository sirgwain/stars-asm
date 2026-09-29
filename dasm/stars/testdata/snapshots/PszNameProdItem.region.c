char *PszNameProdItem(PROD *lpprod) {
    uint32_t iItem;
    int16_t  iDelta;

    iItem = lpprod->iItem;
    if (lpprod->grobj == grobjFleet) {
        if (iItem < 0x10) {
            if (rgshdef[iItem].fFree == 0x0) {
                strcpy(szWork, rgshdef[iItem].hul.szClass);
                return szWork;
            }
        } else {
            iItem = iItem - 0x10;
            if (rglpshdefSB[idPlayer][iItem].fFree == 0x0) {
                fstrcpy(szWork, rglpshdefSB[idPlayer][iItem].hul.szClass);
                if (sel.pl.fStarbase == 0x0) {
                    return szWork;
                }
                iDelta = rglpshdefSB[idPlayer][sel.pl.isb].hul.ihuldef - rglpshdefSB[idPlayer][iItem].hul.ihuldef;
                if (iDelta <= 0) {
                    if (iDelta >= 0) {
                        return szWork;
                    }
                    strcat(szWork, " (upgrade)");
                    return szWork;
                }
                strcat(szWork, " (downgrade)");
                return szWork;
            }
        }
        szWork[0] = 0;
        return szWork;
    }
    if (iItem >= 0x12 && iItem <= 0x1a) {
        fstrcpy(szWork, LpplanetaryFromId(LOWORD(iItem) - 18)->szName);
    } else if (iItem != 0x1b) {
        CchGetString(LOWORD(iItem) + 0x7e, szWork);
    } else {
        CchGetString(idsPlanetaryScanner, szWork);
    }
    return szWork;
}
