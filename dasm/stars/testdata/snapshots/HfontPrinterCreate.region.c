HFONT HfontPrinterCreate(HDC hdc, int16_t iSize, int16_t *pdyFont) {
    HFONT      hfontNew;
    LOGFONT   *plf;
    TEXTMETRIC tm;
    HFONT      hfontSav;

    plf = LocalAlloc(0x40, sizeof(LOGFONT));
    memset(plf, 0, sizeof(LOGFONT));
    plf->lfHeight = -MulDiv(iSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    strcpy(plf->lfFaceName, rgszArial[1]);
    hfontNew = CreateFontIndirect(plf);
    if (pdyFont != 0x0 && hfontNew != 0x0) {
        hfontSav = SelectObject(hdc, hfontNew);
        GetTextMetrics(hdc, &tm);
        *pdyFont = tm.tmHeight + tm.tmExternalLeading;
        SelectObject(hdc, hfontSav);
    }
    LocalFree(plf);
    return hfontNew;
}
