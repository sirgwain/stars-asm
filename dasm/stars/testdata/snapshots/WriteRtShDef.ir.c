void WriteRtShDef(SHDEF *lpshdef, uint8_t **ppbStore) {
    uint8_t  rgb[147];
    char     szHulName[32];
    uint8_t *pb;
    int16_t  cOut;

L_574e:
    ((RTSHDEF *)rgb)->ihuldef = LOBYTE(lpshdef->hul.ihuldef);
    ((RTSHDEF *)rgb)->wFlags = lpshdef->wFlags;
    ((RTSHDEF *)rgb)->chs = lpshdef->hul.chs;
    ((RTSHDEF *)rgb)->ibmp = LOBYTE(lpshdef->hul.ibmp);
    if (lpshdef->det != detAll)
        goto L_5816;
    else
        goto L_5794;

L_5794:
    ((RTSHDEF *)rgb)->dp = lpshdef->hul.dp;
    ((RTSHDEF *)rgb)->turn = lpshdef->turn;
    ((RTSHDEF *)rgb)->cBuilt = lpshdef->cBuilt;
    ((RTSHDEF *)rgb)->cExist = lpshdef->cExist;
    pb = (uint8_t *)&((RTSHDEF *)rgb)->rghs;
    fmemmove(pb, lpshdef->hul.rghs, ((RTSHDEF *)rgb)->chs * 4);
    pb += ((RTSHDEF *)rgb)->chs * 4;
    goto L_5829;

L_5816:
    ((RTSHDEF *)rgb)->wtEmpty = lpshdef->hul.wtEmpty;
    pb = &((RTSHDEF *)rgb)->chs;

L_5829:
    if (lpshdef->det != detAll)
        goto L_585b;
    else
        goto L_583b;

L_583b:
    fstrcpy(szHulName, lpshdef->hul.szClass);
    goto L_5880;

L_585b:
    fstrcpy(szHulName, LphuldefFromId(lpshdef->hul.ihuldef)->hul.szClass);

L_5880:
    cOut = 31;
    if (szHulName[0] == 0)
        goto L_58d5;
    else
        goto L_5893;

L_5893:
    if (FCompressUserString(szHulName, pb + 1, &cOut) == 0)
        goto L_58d5;
    else
        goto L_58bd;

L_58bd:
    *pb = LOBYTE(cOut);
    pb += 1 + cOut;
    goto L_5907;

L_58d5:
    strcpy(pb + 1, szHulName);
    *pb = 0;
    pb += 2 + strlen(szHulName);

L_5907:
    if (ppbStore == 0)
        goto L_593f;
    else
        goto L_5910;

L_5910:
    memmove(*ppbStore, rgb, pb - rgb);
    *ppbStore += pb - rgb;
    goto L_595e;

L_593f:
    WriteRt(rtShDef, pb - rgb, rgb);

L_595e:
    return;
}
