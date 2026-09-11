HFONT HfontPrinterCreate(HDC hdc, int16_t iSize, int16_t *pdyFont) {
    HFONT      hfontNew;
    LOGFONT   *plf;
    TEXTMETRIC tm;
    HFONT      hfontSav;

L_6aa6:
    plf = LocalAlloc(0x40, 0x32);
    memset(plf, 0, 0x32);
    plf->lfHeight = (-MulDiv(iSize, GetDeviceCaps(hdc, LOGPIXELSY), 72));
    strcpy(plf->lfFaceName, LOWORD(rgszArial[1]));
    hfontNew = CreateFontIndirect(plf);
    if ((pdyFont == 0x0))
        goto L_6b5b;
    else
        goto L_6b1f;

L_6b1f:
    if ((hfontNew == 0x0))
        goto L_6b5b;
    else
        goto L_6b28;

L_6b28:
    hfontSav = SelectObject(hdc, hfontNew);
    GetTextMetrics(hdc, &(tm));
    *(pdyFont) = (tm.tmHeight + tm.tmExternalLeading);
    SelectObject(hdc, hfontSav);

L_6b5b:
    LocalFree(plf);

L_6b6a:
    return hfontNew;
}
