int32_t DpShieldOfShdef(SHDEF *lpshdef, int16_t iplr) {
    int16_t  chs;
    HS      *lphs;
    int16_t  ihs;
    int32_t  dpShdef;
    HUL     *lphul;
    PART     part;
    HS      *t_fields_1;
    uint32_t t_fields_2;
    uint32_t t_fields_3;

    dpShdef = 0;
    lphul = &lpshdef->hul;
    lphs = lphul->rghs;
    chs = lphul->chs;
    ihs = 0;
    while (ihs < chs) {
        if (lphs->grhst != hstShield || lphs->cItem <= 0x0) {
            if (lphs->grhst != hstArmor || lphs->cItem <= 0x0 || lphs->iItem != iarmorFieldedKelarium) {
                if (lphs->grhst == hstArmor && lphs->iItem == iarmorMegaPolyShell) {
                    dpShdef = dpShdef + (uint32_t)(lphs->cItem * 0x64);
                }
            } else {
                dpShdef = dpShdef + (uint32_t)(lphs->cItem * 0x32);
            }
        } else {
            part.hs.grhst = lphs->grhst;
            t_fields_1 = &part.hs;
            t_fields_2 = lphs->iItem;
            t_fields_3 = lphs->cItem;
            t_fields_1->iItem = t_fields_2;
            t_fields_1->cItem = t_fields_3;
            FLookupPart(&part);
            dpShdef = dpShdef + (uint32_t)(part.pshield->dp * lphs->cItem);
        }
        ihs = ihs + 1;
        lphs = lphs + 1;
    }
    if (GetRaceGrbit(&rgplr[iplr], ibitRaceRegeneratingShields) != 0) {
        dpShdef = dpShdef + (int32_t)((int32_t)(dpShdef * 2) / 5);
    }
    if ((dpShdef & 0xffff0000) != 0x0) {
        dpShdef = 65535;
    }
    return (uint32_t)LOWORD(dpShdef);
}
