int16_t FGetBestDefensePart(PART *ppart) {
    int16_t fRet;
    int16_t i;
    PART    part;

L_21f6:
    fRet = 1;
    part.hs.grhst = hstPlanetary;
    part.hs.iItem = 0x9;
    i = 0;
    goto L_2250;

L_221d:
    i = (i + 1);
    /* untranslated: ss:[bp-0x10] = part.hs.iItem */
    /* untranslated: ss:[bp-0x12] = ((HIWORD(part.hs) + 0x1) & 0xff) */
    part.hs.iItem = iplanetaryViewer50;
    /* untranslated: HIWORD(part.hs) = (HIWORD(part.hs) | ss:[bp-0x12]) */

L_2250:
    if ((i >= 5))
        goto L_2273;
    else
        goto L_2259;

L_2259:
    if ((FLookupPart(&(part)) != 1))
        goto L_2273;
    else
        goto L_226a;

L_226a:

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
