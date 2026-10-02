void WriteRtShDef(SHDEF *lpshdef, uint8_t **ppbStore) {
    uint8_t  rgb[147];
    char     szHulName[32];
    uint8_t *pb;
    int16_t  cOut;

    ((RTSHDEF *)rgb)->ihuldef = LOBYTE(lpshdef->hul.ihuldef);
    ((RTSHDEF *)rgb)->wFlags = lpshdef->wFlags;
    ((RTSHDEF *)rgb)->chs = lpshdef->hul.chs;
    ((RTSHDEF *)rgb)->ibmp = LOBYTE(lpshdef->hul.ibmp);
    if (lpshdef->det == detAll) {
        ((RTSHDEF *)rgb)->dp = lpshdef->hul.dp;
        ((RTSHDEF *)rgb)->turn = lpshdef->turn;
        ((RTSHDEF *)rgb)->cBuilt = lpshdef->cBuilt;
        ((RTSHDEF *)rgb)->cExist = lpshdef->cExist;
        pb = (uint8_t *)&((RTSHDEF *)rgb)->rghs;
        fmemmove(pb, lpshdef->hul.rghs, ((RTSHDEF *)rgb)->chs * 4);
        pb += ((RTSHDEF *)rgb)->chs * 4;
    } else {
        ((RTSHDEF *)rgb)->wtEmpty = lpshdef->hul.wtEmpty;
        pb = &((RTSHDEF *)rgb)->chs;
    }
    if (lpshdef->det == detAll) {
        fstrcpy(szHulName, lpshdef->hul.szClass);
    } else {
        fstrcpy(szHulName, LphuldefFromId(lpshdef->hul.ihuldef)->hul.szClass);
    }
    cOut = 31;
    if (szHulName[0] != 0 && FCompressUserString(szHulName, pb + 1, &cOut) != 0) {
        *pb = LOBYTE(cOut);
        pb += 1 + cOut;
    } else {
        strcpy(pb + 1, szHulName);
        *pb = 0;
        pb += 2 + strlen(szHulName);
    }
    if (ppbStore != 0) {
        memmove(*ppbStore, rgb, pb - rgb);
        *ppbStore += pb - rgb;
    } else {
        WriteRt(rtShDef, pb - rgb, rgb);
    }
    return;
}
