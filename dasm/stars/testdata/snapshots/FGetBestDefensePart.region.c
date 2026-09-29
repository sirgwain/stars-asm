int16_t FGetBestDefensePart(PART *ppart) {
    int16_t fRet;
    int16_t i;
    PART    part;

    fRet = 1;
    part.hs.grhst = hstPlanetary;
    part.hs.iItem = iplanetarySDI;
    i = 0;
    while (i < 5 && FLookupPart(&part) == 1) {
        i = i + 1;
        part.hs.iItem = part.hs.iItem + 0x1;
    }
    if (i <= 0) {
        fRet = 0;
    } else {
        i = i - 1;
    }
    part.hs.iItem = i + 9;
    FLookupPart(&part);
    *ppart = part;
    return fRet;
}
