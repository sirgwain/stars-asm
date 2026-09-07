HFONT HfontPrinterCreate(HDC hdc, int16_t iSize, int16_t *pdyFont) {
    HFONT      hfontNew;
    LOGFONT   *plf;
    TEXTMETRIC tm;
    HFONT      hfontSav;
    int16_t    scratch_bp_p8;
    HDC        scratch_bp_p6;
    uint16_t   scratch_bp_pa;

L_6aa6:
    plf = LocalAlloc(0x40, 0x32);
    memset(plf, 0, 0x32);
    /* untranslated: plf->lfHeight = (MulDiv(scratch_bp_p8, GetDeviceCaps(scratch_bp_p6, LOGPIXELSY), 72) neg 0) */
    strcpy(plf->lfFaceName, LOWORD(rgszArial[0x1]));
    hfontNew = CreateFontIndirect(plf);
    if ((scratch_bp_pa == 0x0))
        goto L_6b5b;
    else
        goto L_6b1f;

L_6b1f:
    if ((hfontNew == 0x0))
        goto L_6b5b;
    else
        goto L_6b28;

L_6b28:
    hfontSav = SelectObject(scratch_bp_p6, hfontNew);
    GetTextMetrics(scratch_bp_p6, &(tm));
    /* untranslated: ds:[scratch_bp_pa] = (tm.tmHeight + tm.tmExternalLeading) */
    SelectObject(scratch_bp_p6, hfontSav);

L_6b5b:
    LocalFree(plf);

L_6b6a:
    return hfontNew;
}
