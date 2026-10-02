int16_t FReadShDef(RTSHDEF *lprt, SHDEF *lpshdef, int16_t iplrLoad) {
    char     szTemp[40];
    SHDEF    shdef;
    uint8_t *lpb;
    int16_t  ishdef;
    int16_t  cch;
    int16_t  iFirst;
    int16_t  cOut;
    int16_t  fOkay;
    HUL     *lphulBase;
    uint32_t wt;
    int16_t  c;
    HUL     *lphul;
    PART     part;

    memset(&shdef, 0, sizeof(SHDEF));
    shdef.hul.ihuldef = lprt->ihuldef;
    shdef.wFlags = lprt->wFlags;
    shdef.hul.chs = lprt->chs;
    shdef.hul.ibmp = lprt->ibmp;
    if (shdef.det == detAll) {
        shdef.hul.dp = lprt->dp;
        shdef.turn = lprt->turn;
        shdef.cBuilt = lprt->cBuilt;
        shdef.cExist = lprt->cExist;
        lpb = (uint8_t *)lprt->rghs;
        fmemmove(shdef.hul.rghs, lpb, lprt->chs * 4);
        lpb += 4 * lprt->chs;
    } else {
        shdef.hul.wtEmpty = lprt->wtEmpty;
        lpb = &lprt->chs;
    }
    iFirst = LphuldefFromId(shdef.hul.ihuldef)->hul.ibmp;
    if (shdef.hul.ibmp < iFirst || shdef.hul.ibmp >= iFirst + 4) {
        shdef.hul.ibmp = (shdef.hul.ibmp & 3) | iFirst;
    }
    cch = *lpb;
    lpb++;
    if (cch == 0) {
        fstrcpy(shdef.hul.szClass, lpb);
    } else {
        cOut = 32;
        if (cch > 32) {
            return 0;
        }
        fmemmove(szTemp, lpb, cch);
        FDecompressUserString(szTemp, cch, shdef.hul.szClass, &cOut);
    }
    ishdef = shdef.ishdef;
    if (ishdef >= 16) {
        ishdef -= 16;
    }
    if (shdef.det == detAll || lpshdef[ishdef].fFree != 0 || lpshdef[ishdef].det < detAll) {
        lpshdef[ishdef] = shdef;
    } else if (shdef.hul.ihuldef != lpshdef[ishdef].hul.ihuldef || shdef.hul.ibmp != lpshdef[ishdef].hul.ibmp) {
        lpshdef[ishdef] = shdef;
    }
    if (idPlayer != -1) {
        UpdateShdefCost(lpshdef + ishdef);
    }
    if (lpshdef[ishdef].det == detAll) {
        lphul = &lpshdef[ishdef].hul;
        lphulBase = &LphuldefFromId(lphul->ihuldef)->hul;
        wt = (uint32_t)lphulBase->wtEmpty;
        for (c = 0; c < lphul->chs; c++) {
            if (lphul->rghs[c].cItem > 0) {
                part.hs = lphul->rghs[c];
                fOkay = FLookupPart(&part);
                if (idPlayer == -1) {
                    fOkay = 0;
                }
                if ((part.hs.grhst & lphulBase->rghs[c].grhst) == 0 || ((fOkay > 1 && shdef.fGift == 0) || part.hs.cItem > lphulBase->rghs[c].cItem)) {
                    lphul->rghs[c].cItem = 0;
                }
                wt += (uint32_t)(part.pcom->cMass * lphul->rghs[c].cItem);
            }
            if (c == 0 && lphul->rghs[0].cItem == 0 && lphulBase->rghs[0].grhst == hstEngine) {
                lphul->rghs[0].grhst = hstEngine;
                lphul->rghs[0].iItem = 1;
                lphul->rghs[0].cItem = lphulBase->rghs[0].cItem;
                part.hs = lphul->rghs[0];
                FLookupPart(&part);
                wt += (uint32_t)(part.pcom->cMass * lphul->rghs[0].cItem);
            }
        }
        lphul->wtEmpty = LOWORD(wt);
    }
    return 1;
}
