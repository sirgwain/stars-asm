int16_t FGetBestDefensePart(PART *ppart) {
    int16_t fRet;
    int16_t i;
    PART    part;

L_21f6:
    fRet = 1;
    part.hs.grhst = hstPlanetary;
    part.hs.iItem = iplanetarySDI;
    i = 0;
    goto L_2250;

L_221d:
    i = (i + 1);
    part.hs.iItem = (part.hs.iItem + 0x1);

L_2250:
    if ((i >= 5))
        goto L_2273;
    else
        goto L_2259;

L_2259:
    if ((FLookupPart(&(part)) == 1))
        goto L_221d;
    else
        goto L_2273;

L_2273:
    if ((i <= 0))
        goto L_2283;
    else
        goto L_227c;

L_227c:
    i = (i - 1);
    goto L_2288;

L_2283:
    fRet = 0;

L_2288:
    part.hs.iItem = (i + 9);
    FLookupPart(&(part));
    *(ppart) = part;

L_22c6:
    return fRet;
}
