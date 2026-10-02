int32_t DpShieldOfShdef(SHDEF *lpshdef, int16_t iplr) {
    int16_t chs;
    HS     *lphs;
    int16_t ihs;
    int32_t dpShdef;
    HUL    *lphul;
    PART    part;

L_0f24:
    dpShdef = 0;
    lphul = &lpshdef->hul;
    lphs = lphul->rghs;
    chs = lphul->chs;
    ihs = 0;
    goto L_0f7c;

L_0f69:
    ihs++;
    lphs++;

L_0f7c:
    if (ihs >= chs)
        goto L_107e;
    else
        goto L_0f87;

L_0f87:
    if (lphs->grhst != hstShield)
        goto L_0feb;
    else
        goto L_0f93;

L_0f93:
    if (lphs->cItem <= 0)
        goto L_0feb;
    else
        goto L_0faa;

L_0faa:
    part.hs = *lphs;
    FLookupPart(&part);
    dpShdef += (uint32_t)(part.pshield->dp * lphs->cItem);
    goto L_0f69;

L_0feb:
    if (lphs->grhst != hstArmor)
        goto L_1040;
    else
        goto L_0ff7;

L_0ff7:
    if (lphs->cItem <= 0)
        goto L_1040;
    else
        goto L_100e;

L_100e:
    if (lphs->iItem != iarmorFieldedKelarium)
        goto L_1040;
    else
        goto L_1020;

L_1020:
    dpShdef += (uint32_t)(lphs->cItem * 50);
    goto L_0f69;

L_1040:
    if (lphs->grhst != hstArmor)
        goto L_0f69;
    else
        goto L_104c;

L_104c:
    if (lphs->iItem != iarmorMegaPolyShell)
        goto L_0f69;
    else
        goto L_105e;

L_105e:
    dpShdef += (uint32_t)(lphs->cItem * 100);

L_107b:
    goto L_0f69;

L_107e:
    if (GetRaceGrbit(&rgplr[iplr], ibitRaceRegeneratingShields) == 0)
        goto L_10c1;
    else
        goto L_109e;

L_109e:
    dpShdef += (int32_t)(dpShdef * 2) / 5;

L_10c1:
    if ((dpShdef & 0xffff0000) != 0)
        goto L_10dd;
    else
        goto L_10e7;

L_10dd:
    dpShdef = 65535;

L_10e7:

L_10f3:
    return (uint32_t)LOWORD(dpShdef);
}
