int16_t FGetBestDefensePart(PART *ppart) {
    int16_t fRet;
    int16_t i;
    PART    part;

    fRet = TRUE;
    part.hs.grhst = hstPlanetary;
    part.hs.iItem = iplanetarySDI;
    i = 0;
    while (i < 5 && FLookupPart(&part) == 1) {
        i++;
        part.hs.iItem++;
    }
    if (i > 0) {
        i--;
    } else {
        fRet = FALSE;
    }
    part.hs.iItem = i + 9;
    FLookupPart(&part);
    *ppart = part;
    return fRet;
}
