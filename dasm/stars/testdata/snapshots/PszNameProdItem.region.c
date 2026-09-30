char *PszNameProdItem(PROD *lpprod) {
    uint32_t iItem;
    int16_t  iDelta;

    iItem = lpprod->iItem;
    if (lpprod->grobj == grobjFleet) {
        if (iItem >= 16) {
            iItem -= 16;
            if (rglpshdefSB[idPlayer][iItem].fFree == 0) {
                fstrcpy(szWork, rglpshdefSB[idPlayer][iItem].hul.szClass);
                if (sel.pl.fStarbase == 0) {
                    return szWork;
                }
                iDelta = rglpshdefSB[idPlayer][sel.pl.isb].hul.ihuldef - rglpshdefSB[idPlayer][iItem].hul.ihuldef;
                if (iDelta > 0) {
                    strcat(szWork, " (downgrade)");
                    return szWork;
                }
                if (iDelta >= 0) {
                    return szWork;
                }
                strcat(szWork, " (upgrade)");
                return szWork;
            }
        } else if (rgshdef[iItem].fFree == 0) {
            strcpy(szWork, rgshdef[iItem].hul.szClass);
            return szWork;
        }
        szWork[0] = 0;
        return szWork;
    }
    if (iItem >= 18 && iItem <= 26) {
        fstrcpy(szWork, LpplanetaryFromId(LOWORD(iItem) - 18)->szName);
    } else if (iItem == 27) {
        CchGetString(idsPlanetaryScanner, szWork);
    } else {
        CchGetString(LOWORD(iItem) + 126, szWork);
    }
    return szWork;
}
