char *PszNameProdItem(PROD *lpprod) {
    uint32_t iItem;
    int16_t  iDelta;

L_3c92:
    iItem = lpprod->iItem;
    if (lpprod->grobj != grobjFleet)
        goto L_3e7c;
    else
        goto L_3ce1;

L_3ce1:
    if (iItem < 0x10)
        goto L_3e21;
    else
        goto L_3cf8;

L_3cf8:
    iItem = iItem - 0x10;
    if (rglpshdefSB[idPlayer][iItem].fFree == 0x0)
        goto L_3d46;
    else
        goto LBogus;

LBogus:
    szWork[0] = 0;
    return szWork;

L_3d46:
    fstrcpy(szWork, rglpshdefSB[idPlayer][iItem].hul.szClass);
    if (sel.pl.fStarbase == 0x0)
        goto L_3f13;
    else
        goto L_3d94;

L_3d94:
    iDelta = rglpshdefSB[idPlayer][sel.pl.isb].hul.ihuldef - rglpshdefSB[idPlayer][iItem].hul.ihuldef;
    if (iDelta <= 0)
        goto L_3e05;
    else
        goto L_3df2;

L_3df2:
    strcat(szWork, " (downgrade)");
    goto L_3f13;

L_3e05:
    if (iDelta >= 0)
        goto L_3f13;
    else
        goto L_3e0e;

L_3e0e:
    strcat(szWork, " (upgrade)");

L_3e1e:
    goto L_3f13;

L_3e21:
    if (rgshdef[iItem].fFree != 0x0)
        goto LBogus;
    else
        goto L_3e4f;

L_3e4f:
    strcpy(szWork, rgshdef[iItem].hul.szClass);

L_3e79:
    goto L_3f13;

L_3e7c:
    if (iItem < 0x12)
        goto L_3ed8;
    else
        goto L_3e93;

L_3e93:
    if (iItem <= 0x1a)
        goto L_3eaa;
    else
        goto L_3ed8;

L_3eaa:
    fstrcpy(szWork, LpplanetaryFromId(LOWORD(iItem) - 18)->szName);
    goto L_3f13;

L_3ed8:
    if (iItem != 0x1b)
        goto L_3efd;
    else
        goto L_3eea;

L_3eea:
    CchGetString(idsPlanetaryScanner, szWork);
    goto L_3f13;

L_3efd:
    CchGetString(LOWORD(iItem) + 0x7e, szWork);

L_3f13:
    return szWork;
}
