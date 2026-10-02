int32_t DpShieldOfShdef(SHDEF *lpshdef, int16_t iplr) {
    int16_t chs;
    HS     *lphs;
    int16_t ihs;
    int32_t dpShdef;
    HUL    *lphul;
    PART    part;

    dpShdef = 0;
    lphul = &lpshdef->hul;
    lphs = lphul->rghs;
    chs = lphul->chs;
    ihs = 0;
    while (ihs < chs) {
        if (lphs->grhst == hstShield && lphs->cItem > 0) {
            part.hs = *lphs;
            FLookupPart(&part);
            dpShdef += (uint32_t)(part.pshield->dp * lphs->cItem);
        } else if (lphs->grhst == hstArmor && lphs->cItem > 0 && lphs->iItem == iarmorFieldedKelarium) {
            dpShdef += (uint32_t)(lphs->cItem * 50);
        } else if (lphs->grhst == hstArmor && lphs->iItem == iarmorMegaPolyShell) {
            dpShdef += (uint32_t)(lphs->cItem * 100);
        }
        ihs++;
        lphs++;
    }
    if (GetRaceGrbit(&rgplr[iplr], ibitRaceRegeneratingShields) != 0) {
        dpShdef += (int32_t)(dpShdef * 2) / 5;
    }
    if ((dpShdef & 0xffff0000) != 0) {
        dpShdef = 65535;
    }
    return (uint32_t)LOWORD(dpShdef);
}
