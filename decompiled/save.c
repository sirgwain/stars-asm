#include "common.h"

void WriteOrders(FLEET *lpfl) {
    int16_t cord;
    ORDER  *lpord;

L_547e:
    if ((lpfl->cord == 0x0))
        goto L_5515;
    else
        goto L_5494;

L_5494:
    cord = lpfl->cord;
    lpord = lpfl->lpplord->rgord;
    goto L_54c8;

L_54b7:
    lpord = (lpord + 0x1);
    cord = (cord - 0x1);

L_54c8:
    if ((cord == 0x0))
        goto L_5515;
    else
        goto L_54d1;

L_54d1:
    if ((lpord->grTask != grTaskNone))
        goto L_54fc;
    else
        goto L_54e3;

L_54e3:
    WriteRt(rtOrderB, 0x8, &(lpord));
    goto L_54b7;

L_54fc:
    WriteRt(rtOrderA, 0x12, &(lpord));

L_5515:
    return;
}

void WriteRtPlr(PLAYER *pplr, uint8_t *pbStore) {
    uint8_t  rgb[264];
    int16_t  i;
    uint8_t *pb;
    int16_t  cOut;

L_551c:
    if ((pbStore != 0x0))
        goto L_5535;
    else
        goto L_552e;

L_552e:
    pbStore = rgb;

L_5535:
    if ((pplr->fDead == 0x0))
        goto L_5558;
    else
        goto L_5546;

L_5546:
    pplr->det = 0x7;

L_5558:
    memmove(pbStore, pplr, 0xc0);
    if ((pplr->det != 0x7))
        goto L_55f4;
    else
        goto L_557b;

L_557b:
    i = 0xf;
    goto L_55a5;

L_5584:
    if (((uint16_t)(pplr->rgmdRelation[i]) != 0x0))
        goto L_55af;
    else
        goto L_559a;

L_559a:

L_55a0:
    i = (i - 0x1);

L_55a5:
    if ((i >= 0x0))
        goto L_5584;
    else
        goto L_55af;

L_55af:
    i = (i + 0x1);
    pb = (pbStore + 0x70);
    pb = (pb + 0x1);
    *(pb) = LOBYTE(i);
    memmove(pb, ((uint8_t *)(pplr) + 0x70), i);
    pb = (pb + i);
    goto L_5600;

L_55f4:
    pb = (pbStore + 0x8);

L_5600:
    cOut = 0x1f;
    if (((uint16_t)(pplr->szName[0x0]) == 0x0))
        goto L_565c;
    else
        goto L_5616;

L_5616:
    if ((FCompressUserString(((uint8_t *)(pplr) + 0x80), ((uint8_t *)(pb) + 0x1), &(cOut)) == 0x0))
        goto L_565c;
    else
        goto L_5644;

L_5644:
    *(pb) = LOBYTE(cOut);
    pb = (pb + (cOut + 0x1));
    goto L_5696;

L_565c:
    strcpy(((uint8_t *)(pb) + 0x1), ((uint8_t *)(pplr) + 0x80));
    *(pb) = 0x0;
    pb = (pb + (strlen(((uint8_t *)(pplr) + 0x80)) + 0x2));

L_5696:
    cOut = 0x1f;
    if (((uint16_t)(pplr->szNames[0x0]) == 0x0))
        goto L_56f2;
    else
        goto L_56ac;

L_56ac:
    if ((FCompressUserString(((uint8_t *)(pplr) + 0xa0), ((uint8_t *)(pb) + 0x1), &(cOut)) == 0x0))
        goto L_56f2;
    else
        goto L_56da;

L_56da:
    *(pb) = LOBYTE(cOut);
    pb = (pb + (cOut + 0x1));
    goto L_572c;

L_56f2:
    strcpy(((uint8_t *)(pb) + 0x1), ((uint8_t *)(pplr) + 0xa0));
    *(pb) = 0x0;
    pb = (pb + (strlen(((uint8_t *)(pplr) + 0xa0)) + 0x2));

L_572c:
    WriteRt(rtPlr, (pb - pbStore), &(pbStore));
    return;
}

void WriteRtShDef(SHDEF *lpshdef, uint8_t **ppbStore) {
    uint8_t  rgb[147];
    char     szHulName[32];
    uint8_t *pb;
    int16_t  cOut;

L_574e:
    rgb[0x2] = LOBYTE(lpshdef->hul.ihuldef);
    LOWORD(rgb) = lpshdef->wFlags;
    rgb[0x6] = lpshdef->hul.chs;
    rgb[0x3] = LOBYTE(lpshdef->hul.ibmp);
    if ((lpshdef->det != 0x7))
        goto L_5816;
    else
        goto L_5794;

L_5794:
    rgb[0x4] = lpshdef->hul.dp;
    rgb[0x7] = lpshdef->turn;
    rgb[0x9] = LOWORD(lpshdef->cBuilt);
    rgb[0xb] = HIWORD(lpshdef->cBuilt);
    rgb[0xd] = LOWORD(lpshdef->cExist);
    rgb[0xf] = HIWORD(lpshdef->cExist);
    pb = &(rgb[0x11]);
    fmemmove(&(pb), &(lpshdef->hul.rghs), (rgb[0x6] * 0x4));
    pb = (pb + (rgb[0x6] * 0x4));
    goto L_5829;

L_5816:
    rgb[0x4] = lpshdef->hul.wtEmpty;
    pb = &(rgb[0x6]);

L_5829:
    if ((lpshdef->det != 0x7))
        goto L_585b;
    else
        goto L_583b;

L_583b:
    fstrcpy(szHulName, &(lpshdef->hul.szClass));
    goto L_5880;

L_585b:
    fstrcpy(szHulName, &(LphuldefFromId(lpshdef->hul.ihuldef)->hul.szClass));

L_5880:
    cOut = 0x1f;
    if (((uint16_t)(szHulName[0x0]) == 0x0))
        goto L_58d5;
    else
        goto L_5893;

L_5893:
    if ((FCompressUserString(szHulName, ((uint8_t *)(pb) + 0x1), &(cOut)) == 0x0))
        goto L_58d5;
    else
        goto L_58bd;

L_58bd:
    *(pb) = LOBYTE(cOut);
    pb = (pb + (cOut + 0x1));
    goto L_5907;

L_58d5:
    strcpy(((uint8_t *)(pb) + 0x1), szHulName);
    *(pb) = 0x0;
    pb = (pb + (strlen(szHulName) + 0x2));

L_5907:
    if ((ppbStore == 0x0))
        goto L_593f;
    else
        goto L_5910;

L_5910:
    memmove(*(ppbStore), rgb, rgb[pb]);
    *(ppbStore) = (*(ppbStore) + rgb[pb]);
    goto L_595e;

L_593f:
    WriteRt(rtShDef, rgb[pb], &(rgb));

L_595e:
    return;
}

int16_t FWriteDataFile(char *pszFileBase, int16_t iPlayer, int16_t fAppend) {
    int16_t  iMax;
    FLEET   *lpflT;
    int16_t  fNoAutoTrack;
    BTLPLAN *lpbtlplan;
    int16_t  j;
    jmp_buf *penvMemSav[9];
    int16_t  i;
    ORDER   *lpord;
    THING   *lpth;
    FLEET   *lpfl;
    jmp_buf  env[9];
    int16_t  iord;
    SHDEF   *lpshdef;
    THING   *lpthMac;
    int16_t  fRet;
    PLANET  *lpplT;
    SCAN     scan;
    int16_t  mdTarget;
    FLEET   *lpflTarget;
    POINT    pt;
    int32_t  dy;
    int16_t  iflT;
    FLEET   *lpflBest;
    int16_t  fFoundIdeal;
    int32_t  dx;
    int32_t  lBest;
    int32_t  l;
    PLANET   pl;
    int16_t  t_merge_5be5_0001;
    uint16_t t_merge_5be5_0002;
    uint16_t t_merge_6797_0001;

L_5964:
    fRet = 0x1;
    SetVisiblePlanFleet(iPlayer);
    if ((gd.fGeneratingTurn == 0x0))
        goto L_6647;
    else
        goto L_598d;

L_598d:
    if ((iPlayer == 0xffff))
        goto L_6647;
    else
        goto L_5996;

L_5996:
    i = 0x0;
    goto L_59a2;

L_599e:
    i = (i + 0x1);

L_59a2:
    if ((i >= cFleet))
        goto L_6647;
    else
        goto L_59ad;

L_59ad:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_59dd;
    else
        goto L_59d5;

L_59d5:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_6647;
    else
        goto L_59dd;

L_59dd:
    if ((lpfl->fDead != 0x0))
        goto L_599e;
    else
        goto L_59f4;

L_59f4:
    if ((lpfl->iplr != iPlayer))
        goto L_599e;
    else
        goto L_5a09;

L_5a09:

L_5a0f:
    j = 0x0;
    goto L_5a3b;

L_5a17:
    if ((lpfl->rgcsh[j] != 0x0))
        goto L_5a44;
    else
        goto L_5a31;

L_5a31:

L_5a37:
    j = (j + 0x1);

L_5a3b:
    if ((j < 0x10))
        goto L_5a17;
    else
        goto L_5a44;

L_5a44:
    if ((j != 0x10))
        goto L_5a64;
    else
        goto L_5a4d;

L_5a4d:
    lpfl->fDead = 0x1;
    goto L_599e;

L_5a64:
    lpord = lpfl->lpplord->rgord;
    if ((lpord->grobj != grobjFleet))
        goto L_5af1;
    else
        goto L_5a91;

L_5a91:
    /* untranslated: branch FFindNearestObject(words(lpord->pt.y, lpord->pt.x), 0x81, &scan) == 0x0 ? L_5ad4 : L_5ab3 */

L_5ab3:
    lpord->grobj = grobjPlanet;
    lpord->id = scan.idpl;
    goto L_5af1;

L_5ad4:
    lpord->grobj = grobjOther;
    lpord->id = 0x0;

L_5af1:
    if ((lpord->grTask != grTaskNone))
        goto L_5b4c;
    else
        goto L_5b03;

L_5b03:
    if ((lpfl->cord <= 0x1))
        goto L_5b4c;
    else
        goto L_5b10;

L_5b10:
    if (((*(lpord + 0x18) & 0xf) != 0x7))
        goto L_5b4c;
    else
        goto L_5b22;

L_5b22:
    lpord->grTask = grTaskPatrol;
    lpord->tsell.iPlrX = *(lpord + 0x1a);
    *(lpord + 0xa) = *(lpord + 0x1c);

L_5b4c:
    if ((lpord->grTask != grTaskPatrol))
        goto L_6160;
    else
        goto L_5b5e;

L_5b5e:
    if ((lpfl->cord <= 0x1))
        goto L_5b82;
    else
        goto L_5b6b;

L_5b6b:
    if ((((*(lpord + 0x18) >> 0x8) & 0xf) == 0x2))
        goto L_6160;
    else
        goto L_5b82;

L_5b82:
    lpflBest = 0x0;
    lBest = 0x5f5e100;
    fFoundIdeal = 0x0;
    if ((lpfl->idPlanet != 0xffff))
        goto L_5bda;
    else
        goto L_5ba8;

L_5ba8:
    if ((lpfl->cord < 0x2))
        goto L_5bda;
    else
        goto L_5bb5;

L_5bb5:
    if ((lpfl->fRepOrders == 0x0))
        goto L_5bda;
    else
        goto L_5bcc;

L_5bcc:
    t_merge_5be5_0001 = lpord[0x1].pt.x;
    t_merge_5be5_0002 = *(lpord + 0x14);
    goto L_5be5;

L_5bda:
    t_merge_5be5_0001 = lpfl->pt.x;
    t_merge_5be5_0002 = lpfl->pt.y;

L_5be5:
    pt.x = t_merge_5be5_0001;
    pt.y = t_merge_5be5_0002;
    lpbtlplan = &(rglpbtlplan[lpfl->iPlayer][lpfl->iplan]);
    mdTarget = lpbtlplan->mdTarget1;
    iflT = 0x0;
    goto L_5c2e;

L_5c2a:
    iflT = (iflT + 0x1);

L_5c2e:
    if ((iflT >= cFleet))
        goto L_5dbe;
    else
        goto L_5c39;

L_5c39:
    lpflTarget = rglpfl[iflT];
    if ((LOWORD(rglpfl[iflT]) != 0x0))
        goto L_5c69;
    else
        goto L_5c61;

L_5c61:
    if ((*(rglpfl[iflT] + 0x2) == 0x0))
        goto L_5dbe;
    else
        goto L_5c69;

L_5c69:
    if ((lpflTarget->fInclude == 0x0))
        goto L_5c2a;
    else
        goto L_5c80;

L_5c80:
    if ((lpflTarget->iPlayer == iPlayer))
        goto L_5c2a;
    else
        goto L_5c8c;

L_5c8c:

L_5c92:
    dx = (uint32_t)((lpflTarget->pt.x - pt.x));
    dy = (uint32_t)((lpflTarget->pt.y - pt.y));
    l = ((uint32_t)((dx * dx)) + (uint32_t)((dy * dy)));
    if ((fFoundIdeal != 0x0))
        goto L_5d0c;
    else
        goto L_5cf5;

L_5cf5:
    if ((lpflTarget->fMark == 0x0))
        goto L_5d47;
    else
        goto L_5d0c;

L_5d0c:
    if ((HIWORD(l) > HIWORD(lBest)))
        goto L_5c2a;
    else
        goto L_5d1a;

L_5d1a:
    if ((HIWORD(l) < HIWORD(lBest)))
        goto L_5d27;
    else
        goto L_5d1f;

L_5d1f:
    if ((LOWORD(l) >= LOWORD(lBest)))
        goto L_5c2a;
    else
        goto L_5d27;

L_5d27:
    if ((fFoundIdeal == 0x0))
        goto L_5d47;
    else
        goto L_5d30;

L_5d30:
    if ((lpflTarget->fMark != 0x0))
        goto L_5c2a;
    else
        goto L_5d47;

L_5d47:
    if ((FMatchTarget(lpflTarget, mdTarget, 0x0) == 0x0))
        goto L_5c2a;
    else
        goto L_5d61;

L_5d61:

L_5d67:
    if ((FAttackPlayer(lpfl, lpflTarget->iPlayer) == 0x0))
        goto L_5c2a;
    else
        goto L_5d81;

L_5d81:

L_5d87:
    lpflBest = lpflTarget;
    lBest = l;
    if ((lpflTarget->fMark != 0x0))
        goto L_5c2a;
    else
        goto L_5db6;

L_5db6:
    fFoundIdeal = 0x1;

L_5dbe:
    if ((fFoundIdeal == 0x0))
        goto L_5dee;
    else
        goto L_5dc7;

L_5dc7:
    if ((gd.fTutorial != 0x0))
        goto L_5dee;
    else
        goto L_5dda;

L_5dda:
    lpflBest->fMark = 0x1;

L_5dee:
    j = (LOWORD((0x32 * *(lpord + 0xa))) + 0x32);
    if ((j != 0x226))
        goto L_5e0d;
    else
        goto L_5e08;

L_5e08:
    j = 0x2710;

L_5e0d:
    if ((LOWORD(lpflBest) != 0x0))
        goto L_5e1f;
    else
        goto L_5e16;

L_5e16:
    if ((HIWORD(lpflBest) == 0x0))
        goto L_6160;
    else
        goto L_5e1f;

L_5e1f:
    if ((LOWORD(lBest) != 0x0))
        goto L_5e31;
    else
        goto L_5e28;

L_5e28:
    if ((HIWORD(lBest) == 0x0))
        goto L_6160;
    else
        goto L_5e31;

L_5e31:
    if ((HIWORD(lBest) > HIWORD((uint32_t)(((uint32_t)(j) * (uint32_t)(j))))))
        goto L_6160;
    else
        goto L_5e4a;

L_5e4a:
    if ((HIWORD(lBest) < HIWORD((uint32_t)(((uint32_t)(j) * (uint32_t)(j))))))
        goto L_5e57;
    else
        goto L_5e4f;

L_5e4f:
    if ((LOWORD(lBest) > LOWORD((uint32_t)(((uint32_t)(j) * (uint32_t)(j))))))
        goto L_6160;
    else
        goto L_5e57;

L_5e57:
    if ((lpfl->lpplord->iordMax > (lpfl->cord + 0x1)))
        goto L_5eb5;
    else
        goto L_5e76;

L_5e76:
    lpfl->lpplord = LpplReAlloc(lpfl->lpplord, (lpfl->cord + 0x2));
    lpord = lpfl->lpplord->rgord;

L_5eb5:
    if ((lpfl->cord <= 0x1))
        goto L_5ef4;
    else
        goto L_5ec2;

L_5ec2:
    fmemmove(&(lpord[0x2]), &(lpord[0x1]), LOWORD(((lpfl->cord + 0xffff) * 0x12)));

L_5ef4:
    if ((lpfl->cord != 0x1))
        goto L_6065;
    else
        goto L_5f01;

L_5f01:
    fmemset(&(lpord[0x1]), 0x0, 0x12);
    *(lpord + 0x18) = ((*(lpord + 0x18) & 0xefff) | 0x1000);
    *(lpord + 0x18) = ((*(lpord + 0x18) & 0xfff0) | 0x7);
    *(lpord + 0x1a) = lpord->tsell.iPlrX;
    *(lpord + 0x1c) = *(lpord + 0xa);
    if ((*(lpord + 0x1a) != 0x0))
        goto L_5fa5;
    else
        goto L_5f69;

L_5f69:
    *(lpord + 0x18) = ((*(lpord + 0x18) & 0xff0f) | ((IFindIdealWarp(lpfl, 0x0) & 0xf) << 0x4));
    goto L_5fd3;

L_5fa5:
    *(lpord + 0x18) = ((*(lpord + 0x18) & 0xff0f) | ((*(lpord + 0x1a) & 0xf) << 0x4));

L_5fd3:
    if ((lpfl->fRepOrders == 0x0))
        goto L_60dc;
    else
        goto L_5fea;

L_5fea:
    lpord[0x2] = *(lpord);
    *(lpord + 0x2a) = ((*(lpord + 0x2a) & 0xff0f) | ((IFindIdealWarp(lpfl, 0x0) & 0xf) << 0x4));
    lpfl->cord = (lpfl->cord + 0x1);
    lpfl->lpplord->iordMac = (lpfl->lpplord->iordMac + 0x1);

L_6065:
    if ((*(lpord + 0x1a) != 0x0))
        goto L_60ae;
    else
        goto L_6072;

L_6072:
    *(lpord + 0x18) = ((*(lpord + 0x18) & 0xff0f) | ((IFindIdealWarp(lpfl, 0x0) & 0xf) << 0x4));
    goto L_60dc;

L_60ae:
    *(lpord + 0x18) = ((*(lpord + 0x18) & 0xff0f) | ((*(lpord + 0x1a) & 0xf) << 0x4));

L_60dc:
    lpord[0x1].pt.x = lpflBest->pt.x;
    *(lpord + 0x14) = lpflBest->pt.y;
    *(lpord + 0x16) = lpflBest->id;
    *(lpord + 0x18) = ((*(lpord + 0x18) & 0xf0ff) | 0x200);
    lpfl->cord = (lpfl->cord + 0x1);
    lpfl->lpplord->iordMac = (lpfl->lpplord->iordMac + 0x1);
    FSendPlrMsg(iPlayer, 0xff, (lpfl->id | 0x8000), lpfl->id, lpflBest->id, 0x0, 0x0, 0x0, 0x0, 0x0);

L_6160:
    if ((lpord->grTask == grTaskXfer))
        goto L_599e;
    else
        goto L_6172;

L_6172:
    if ((lpfl->cord <= 0x1))
        goto L_599e;
    else
        goto L_617c;

L_617c:

L_6182:
    iord = 0x1;
    goto L_618e;

L_618a:
    iord = (iord + 0x1);

L_618e:
    if ((iord >= lpfl->cord))
        goto L_599e;
    else
        goto L_619d;

L_619d:
    /* untranslated: branch ((part[6:2](lpord[iord]) >> 0x8) & 0xf) != 0x8 ? L_63c0 : L_61c1 */

L_61c1:
    lpth = LpthFromId(lpord[iord].id);
    if ((LOWORD(lpth) != 0x0))
        goto L_61f5;
    else
        goto L_61ec;

L_61ec:
    if ((HIWORD(lpth) == 0x0))
        goto L_627f;
    else
        goto L_61f5;

L_61f5:
    if ((lpth->ith != ithMysteryTrader))
        goto L_6225;
    else
        goto L_620b;

L_620b:
    if ((lpth->tht.fInclude == 0x0))
        goto L_627f;
    else
        goto L_6225;

L_6225:
    if ((lpth->ith != ithMinefield))
        goto L_6252;
    else
        goto L_623b;

L_623b:
    if ((((0x1 << iPlayer) & lpth->thm.grbitPlrNow) == 0x0))
        goto L_627f;
    else
        goto L_6252;

L_6252:
    if ((lpth->ith != ithWormhole))
        goto L_618a;
    else
        goto L_6268;

L_6268:
    if ((lpth->thw.fInclude != 0x0))
        goto L_618a;
    else
        goto L_627f;

L_627f:
    if ((LOWORD(lpth) != 0x0))
        goto L_6291;
    else
        goto L_6288;

L_6288:
    if ((HIWORD(lpth) == 0x0))
        goto L_62d1;
    else
        goto L_6291;

L_6291:
    if ((lpth->ith != ithWormhole))
        goto L_62d1;
    else
        goto L_62a7;

L_62a7:
    FSendPlrMsg2(lpfl->iPlayer, 0xf8, (lpfl->id | 0x8000), lpfl->id, 0x0);
    goto L_6372;

L_62d1:
    if ((LOWORD(lpth) != 0x0))
        goto L_62e3;
    else
        goto L_62da;

L_62da:
    if ((HIWORD(lpth) == 0x0))
        goto L_6323;
    else
        goto L_62e3;

L_62e3:
    if ((lpth->ith != ithMysteryTrader))
        goto L_6323;
    else
        goto L_62f9;

L_62f9:
    FSendPlrMsg2(lpfl->iPlayer, 0x110, (lpfl->id | 0x8000), lpfl->id, 0x0);
    goto L_6372;

L_6323:
    if ((LOWORD(lpth) != 0x0))
        goto L_6335;
    else
        goto L_632c;

L_632c:
    if ((HIWORD(lpth) == 0x0))
        goto L_6372;
    else
        goto L_6335;

L_6335:
    if ((lpth->ith != ithMinefield))
        goto L_6372;
    else
        goto L_634b;

L_634b:
    FSendPlrMsg2(lpfl->iPlayer, 0x111, (lpfl->id | 0x8000), lpfl->id, 0x0);

L_6372:
    /* untranslated: part[6:2](lpord[iord]) = ((part[6:2](lpord[iord]) & 0xf0ff) | 0x400) */
    lpord[iord].id = iord;

L_63c0:
    /* untranslated: branch ((part[6:2](lpord[iord]) >> 0x8) & 0xf) != 0x2 ? L_618a : L_63e1 */

L_63e1:

L_63e7:
    /* untranslated: fNoAutoTrack = ((part[6:2](lpord[iord]) >> 0xd) & 0x1) */
    if ((fNoAutoTrack == 0x0))
        goto L_6443;
    else
        goto L_640f;

L_640f:
    /* untranslated: part[6:2](lpord[iord]) = ((part[6:2](lpord[iord]) & 0xdfff) | 0x0) */

L_6443:
    lpflT = LpflFromId(lpord[iord].id);
    if ((LOWORD(lpflT) != 0x0))
        goto L_6477;
    else
        goto L_646e;

L_646e:
    if ((HIWORD(lpflT) == 0x0))
        goto L_648e;
    else
        goto L_6477;

L_6477:
    if ((lpflT->fDead == 0x0))
        goto L_659d;
    else
        goto L_648e;

L_648e:
    FSendPlrMsg(iPlayer, 0x28, (lpfl->id | 0x8000), lpfl->id, lpord[iord].id, 0x0, 0x0, 0x0, 0x0, 0x0);

FixupCoords:
    /* untranslated: part[6:2](lpord[iord]) = ((part[6:2](lpord[iord]) & 0xf0ff) | 0x400) */
    lpord[iord].id = iord;
    /* untranslated: branch FFindNearestObject(words(lpord[iord].pt.y, lpord[iord].pt.x), 0x81, &scan) == 0x0 ? L_618a : L_654f */

L_654f:
    /* untranslated: part[6:2](lpord[iord]) = ((part[6:2](lpord[iord]) & 0xf0ff) | 0x100) */
    lpord[iord].id = scan.idpl;

L_659d:
    if ((lpflT->fInclude != 0x0))
        goto L_618a;
    else
        goto L_65b4;

L_65b4:
    if ((lpflT->idPlanet == 0xffff))
        goto L_6607;
    else
        goto L_65c1;

L_65c1:
    if ((fNoAutoTrack != 0x0))
        goto L_6607;
    else
        goto L_65ca;

L_65ca:
    FSendPlrMsg(iPlayer, 0x29, (lpfl->id | 0x8000), lpfl->id, lpflT->idPlanet, 0x0, 0x0, 0x0, 0x0, 0x0);
    goto FixupCoords;

L_6607:
    FSendPlrMsg(iPlayer, 0x2a, (lpfl->id | 0x8000), lpfl->id, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);

L_6647:
    MarkPlayersThatSentMsgs(iPlayer);
    MarkPlanetsPlayerLost(iPlayer);
    if ((iPlayer != 0xffff))
        goto L_6686;
    else
        goto L_6666;

L_6666:
    _wsprintf(szWork, "%s.hst", pszFileBase);
    goto L_66aa;

L_6686:
    _wsprintf(szWork, "%s.m%d", pszFileBase, (iPlayer + 0x1));

L_66aa:
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_675f;
    else
        goto LFail;

LFail:
    idPlayer = iPlayer;
    if ((fAppend == 0x0))
        goto L_66fd;
    else
        goto L_66d9;

L_66d9:
    AlertSz(PszFormatIds(idsUnableUpdateTurnFile, 0x0), MB_ICONHAND);
    goto L_6751;

L_66fd:
    if ((iPlayer == 0xffff))
        goto L_672a;
    else
        goto L_6706;

L_6706:
    AlertSz(PszFormatIds(idsUnableCreateNewTurnFile, 0x0), MB_ICONHAND);
    goto L_6751;

L_672a:
    idPlayer = 0xffff;
    AlertSz(PszFormatIds(idsUnableCreateHostFile, 0x0), MB_ICONHAND);

L_6751:
    idPlayer = 0xffff;
    fRet = 0x0;
    goto FreeUp;

L_675f:
    if ((fAppend == 0x0))
        goto L_677e;
    else
        goto L_6768;

L_6768:
    if ((FAppendFile(iPlayer) != 0x0))
        goto LAppend;
    else
        goto L_6778;

L_6778:

L_677e:
    if ((iPlayer != 0xffff))
        goto L_6794;
    else
        goto L_678e;

L_678e:
    t_merge_6797_0001 = 0x2;
    goto L_6797;

L_6794:
    t_merge_6797_0001 = 0x3;

L_6797:
    if ((FCreateFile(t_merge_6797_0001, iPlayer, 0x0) == 0x0))
        goto LFail;
    else
        goto L_67a5;

L_67a5:

LAppend:
    WriteBattles(iPlayer);
    i = 0x0;
    goto L_67c2;

L_67be:
    i = (i + 0x1);

L_67c2:
    if ((i >= game.cPlayer))
        goto L_6867;
    else
        goto L_67cd;

L_67cd:
    if ((((rgplr[i].wMdPlr >> 0x8) & 0x1) != 0x0))
        goto L_6804;
    else
        goto L_67eb;

L_67eb:
    if (((rgplr[i].wFlags & 0x1) == 0x0))
        goto L_67be;
    else
        goto L_6804;

L_6804:
    if ((GetRaceStat(rgplr[iPlayer], rsMajorAdv) != raTerra))
        goto L_684c;
    else
        goto L_6824;

L_6824:
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfff8) | 0x7);

L_684c:
    WriteRtPlr(rgplr[i], 0x0);

L_6867:
    if ((iPlayer != 0xffff))
        goto L_689b;
    else
        goto L_6870;

L_6870:
    if ((LOWORD(lSaltCur) != 0x0))
        goto L_6884;
    else
        goto L_687a;

L_687a:
    if ((HIWORD(lSaltCur) == 0x0))
        goto L_689b;
    else
        goto L_6884;

L_6884:
    WriteRt(rtChgPassword, 0x4, lSaltCur);

L_689b:
    WritePlayerMessages(iPlayer);
    i = 0x0;
    lpplT = lpPlanets;
    goto L_68ce;

L_68bb:
    i = (i + 0x1);
    lpplT = (lpplT + 0x1);

L_68ce:
    if ((i >= cPlanet))
        goto L_69fa;
    else
        goto L_68d9;

L_68d9:
    if ((lpplT->fInclude == 0x0))
        goto L_68bb;
    else
        goto L_68f0;

L_68f0:
    if ((lpplT->det != 0x7))
        goto L_6963;
    else
        goto L_6902;

L_6902:
    WritePlanet(lpplT, rtPlanet, 0x0);
    if ((LOWORD(lpplT->lpplprod) != 0x0))
        goto L_692f;
    else
        goto L_6925;

L_6925:
    if ((HIWORD(lpplT->lpplprod) == 0x0))
        goto L_68bb;
    else
        goto L_692f;

L_692f:
    WriteRt(rtProdQ, (lpplT->lpplprod->iprodMac * 0x4), &(lpplT->lpplprod->rgprod));

L_6963:
    if ((lpplT->det != 0x2))
        goto L_69e1;
    else
        goto L_6975;

L_6975:
    pl = *(lpplT);
    lpplT->fStarbase = 0x0;
    lpplT->det = 0x3;
    WritePlanet(lpplT, rtPlanetB, 0x0);
    *(lpplT) = pl;
    goto L_68bb;

L_69e1:
    WritePlanet(lpplT, rtPlanetB, 0x0);

L_69fa:
    i = 0x0;
    goto L_6a06;

L_6a02:
    i = (i + 0x1);

L_6a06:
    if ((i >= game.cPlayer))
        goto L_6ac0;
    else
        goto L_6a11;

L_6a11:
    if ((((rgplr[i].wMdPlr >> 0x8) & 0x1) == 0x0))
        goto L_6a02;
    else
        goto L_6a2f;

L_6a2f:
    lpshdef = rglpshdef[i];
    j = 0x0;
    goto L_6ab4;

L_6a4c:
    if ((((lpshdef[j].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_6ab0;
    else
        goto L_6a70;

L_6a70:
    if ((((lpshdef[j].wFlags >> 0x8) & 0x1) == 0x0))
        goto L_6ab0;
    else
        goto L_6a94;

L_6a94:
    WriteRtShDef(&(lpshdef[j]), 0x0);

L_6ab0:
    j = (j + 0x1);

L_6ab4:
    if ((j >= 0x10))
        goto L_6a02;
    else
        goto L_6aba;

L_6aba:

L_6ac0:
    i = 0x0;
    goto L_6acc;

L_6ac8:
    i = (i + 0x1);

L_6acc:
    if ((i >= cFleet))
        goto L_6b2f;
    else
        goto L_6ad7;

L_6ad7:
    lpflT = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_6b07;
    else
        goto L_6aff;

L_6aff:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_6b2f;
    else
        goto L_6b07;

L_6b07:
    if ((lpflT->fInclude == 0x0))
        goto L_6ac8;
    else
        goto L_6b1e;

L_6b1e:
    WriteFleet(lpflT);

L_6b2f:
    i = 0x0;
    goto L_6b3b;

L_6b37:
    i = (i + 0x1);

L_6b3b:
    if ((i >= game.cPlayer))
        goto L_6bf5;
    else
        goto L_6b46;

L_6b46:
    if ((((rgplr[i].wMdPlr >> 0x8) & 0x1) == 0x0))
        goto L_6b37;
    else
        goto L_6b64;

L_6b64:
    lpshdef = rglpshdefSB[i];
    j = 0x0;
    goto L_6be9;

L_6b81:
    if ((((lpshdef[j].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_6be5;
    else
        goto L_6ba5;

L_6ba5:
    if ((((lpshdef[j].wFlags >> 0x8) & 0x1) == 0x0))
        goto L_6be5;
    else
        goto L_6bc9;

L_6bc9:
    WriteRtShDef(&(lpshdef[j]), 0x0);

L_6be5:
    j = (j + 0x1);

L_6be9:
    if ((j >= 0xa))
        goto L_6b37;
    else
        goto L_6bef;

L_6bef:

L_6bf5:
    if ((iPlayer == 0xffff))
        goto L_6c9d;
    else
        goto L_6bfe;

L_6bfe:
    if ((LOWORD(vlprgScoreX) != 0x0))
        goto L_6c12;
    else
        goto L_6c08;

L_6c08:
    if ((HIWORD(vlprgScoreX) == 0x0))
        goto L_6c9d;
    else
        goto L_6c12;

L_6c12:
    i = 0x0;
    goto L_6c1e;

L_6c1a:
    i = (i + 0x1);

L_6c1e:
    if ((i >= game.cPlayer))
        goto L_6c9d;
    else
        goto L_6c29;

L_6c29:
    if ((gd.fGameOverMan != 0x0))
        goto L_6c78;
    else
        goto L_6c37;

L_6c37:
    if ((i == iPlayer))
        goto L_6c78;
    else
        goto L_6c42;

L_6c42:
    if (((rgplr[i].wFlags & 0x1) != 0x0))
        goto L_6c78;
    else
        goto L_6c5b;

L_6c5b:
    if ((game.fVisScores == 0x0))
        goto L_6c1a;
    else
        goto L_6c6e;

L_6c6e:
    if ((game.turn < 0x14))
        goto L_6c1a;
    else
        goto L_6c78;

L_6c78:
    WriteRt(rtScore, 0x18, &(vlprgScoreX[i]));

L_6c9d:
    i = 0x0;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_6def;

L_6ccd:
    if ((iPlayer == 0xffff))
        goto L_6de7;
    else
        goto L_6cd6;

L_6cd6:
    if ((iPlayer != lpth->iplr))
        goto L_6d30;
    else
        goto L_6cee;

L_6cee:
    if ((lpth->ith == ithMineralPacket))
        goto L_6d30;
    else
        goto L_6d04;

L_6d04:
    if ((lpth->ith == ithMysteryTrader))
        goto L_6d30;
    else
        goto L_6d1a;

L_6d1a:
    if ((lpth->ith != ithWormhole))
        goto L_6de7;
    else
        goto L_6d30;

L_6d30:
    if ((lpth->ith != ithMinefield))
        goto L_6d5d;
    else
        goto L_6d46;

L_6d46:
    if ((((0x1 << iPlayer) & lpth->thm.grbitPlrNow) != 0x0))
        goto L_6de7;
    else
        goto L_6d5d;

L_6d5d:
    if ((lpth->ith != ithMineralPacket))
        goto L_6d8a;
    else
        goto L_6d73;

L_6d73:
    if ((lpth->thp.fInclude != 0x0))
        goto L_6de7;
    else
        goto L_6d8a;

L_6d8a:
    if ((lpth->ith != ithMysteryTrader))
        goto L_6dba;
    else
        goto L_6da0;

L_6da0:
    if ((lpth->tht.fInclude != 0x0))
        goto L_6de7;
    else
        goto L_6dba;

L_6dba:
    if ((lpth->ith != ithWormhole))
        goto L_6deb;
    else
        goto L_6dd0;

L_6dd0:
    if ((lpth->thw.fInclude == 0x0))
        goto L_6deb;
    else
        goto L_6de7;

L_6de7:
    i = (i + 0x1);

L_6deb:
    lpth = (lpth + 0x1);

L_6def:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_6ccd;
    else
        goto L_6dfd;

L_6dfd:
    if ((i <= 0x0))
        goto L_6f8a;
    else
        goto L_6e06;

L_6e06:
    WriteRt(rtThing, 0x2, &(i));
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_6f7c;

L_6e48:
    if ((iPlayer == 0xffff))
        goto L_6f62;
    else
        goto L_6e51;

L_6e51:
    if ((iPlayer != lpth->iplr))
        goto L_6eab;
    else
        goto L_6e69;

L_6e69:
    if ((lpth->ith == ithMineralPacket))
        goto L_6eab;
    else
        goto L_6e7f;

L_6e7f:
    if ((lpth->ith == ithMysteryTrader))
        goto L_6eab;
    else
        goto L_6e95;

L_6e95:
    if ((lpth->ith != ithWormhole))
        goto L_6f62;
    else
        goto L_6eab;

L_6eab:
    if ((lpth->ith != ithMinefield))
        goto L_6ed8;
    else
        goto L_6ec1;

L_6ec1:
    if ((((0x1 << iPlayer) & lpth->thm.grbitPlrNow) != 0x0))
        goto L_6f62;
    else
        goto L_6ed8;

L_6ed8:
    if ((lpth->ith != ithMineralPacket))
        goto L_6f05;
    else
        goto L_6eee;

L_6eee:
    if ((lpth->thp.fInclude != 0x0))
        goto L_6f62;
    else
        goto L_6f05;

L_6f05:
    if ((lpth->ith != ithMysteryTrader))
        goto L_6f35;
    else
        goto L_6f1b;

L_6f1b:
    if ((lpth->tht.fInclude != 0x0))
        goto L_6f62;
    else
        goto L_6f35;

L_6f35:
    if ((lpth->ith != ithWormhole))
        goto L_6f78;
    else
        goto L_6f4b;

L_6f4b:
    if ((lpth->thw.fInclude == 0x0))
        goto L_6f78;
    else
        goto L_6f62;

L_6f62:
    WriteRt(rtThing, 0x12, &(lpth));

L_6f78:
    lpth = (lpth + 0x1);

L_6f7c:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_6e48;
    else
        goto L_6f8a;

L_6f8a:
    if ((iPlayer != 0xffff))
        goto L_6fa1;
    else
        goto L_6f93;

L_6f93:
    i = 0x0;
    iMax = game.cPlayer;
    goto L_700e;

L_6fa1:
    i = iPlayer;
    iMax = (iPlayer + 0x1);

L_6fb3:
    lpbtlplan = rglpbtlplan[i];
    j = 0x0;
    goto L_6fe3;

L_6fd0:
    j = (j + 0x1);
    lpbtlplan = (lpbtlplan + 0x1);

L_6fe3:
    if ((j >= rgcbtlplan[i]))
        goto L_700a;
    else
        goto L_6ff5;

L_6ff5:
    WriteBattlePlan(lpbtlplan, 0x0);
    goto L_6fd0;

L_700a:
    i = (i + 0x1);

L_700e:
    if ((i < iMax))
        goto L_6fb3;
    else
        goto L_7019;

L_7019:
    WriteRt(rtEOF, 0x2, &(game.turn));
    StreamClose();

FreeUp:
    SetVisiblePlanFleet(0xffff);

L_7047:
    return fRet;
}

int16_t FAppendFile(int16_t iPlayer) {
L_704e:
    if ((FMarkFile(0x2003, iPlayer, 0x4, 0x1) != 0x0))
        goto L_707c;
    else
        goto L_7076;

L_7076:
    return 0x0;

L_707c:
    WriteBOF(iPlayer, 0x3, 0x1);
    return 0x1;
}

void WriteBattles(int16_t iPlayer) {
    int16_t  ctok;
    int16_t  cbRec;
    PLANET  *lppl;
    int16_t  i;
    FLEET   *lpfl;
    int16_t  cbT;
    uint16_t fPlayerCur;
    BTLREC  *lpbtlrec;
    uint8_t *lpbBattle;
    HB      *lphb;
    BTLDATA *lpbtldata;
    int16_t  cb;
    int16_t  iplr;
    int16_t  t_merge_782d_0001;

L_709c:
    cbT = 0x0;
    if ((iPlayer == 0xffff))
        goto L_7a64;
    else
        goto L_70b3;

L_70b3:
    if ((LOWORD(lpbBattleLog) != LOWORD(lpbBattleCur)))
        goto L_70cf;
    else
        goto L_70c3;

L_70c3:
    if ((HIWORD(lpbBattleLog) == HIWORD(lpbBattleCur)))
        goto L_7a64;
    else
        goto L_70c9;

L_70c9:

L_70cf:
    lphb = rglphb[0xb];
    lpbBattle = &(*(lphb + 0x12));
    fPlayerCur = (0x1 << iPlayer);
    goto L_7a52;

L_70fb:
    lpbtldata = lpbBattle;

L_7107:
    if ((lphb->ibTop <= 0x10))
        goto L_7120;
    else
        goto L_7114;

L_7114:
    if ((lpbtldata->id != 0xffff))
        goto L_7166;
    else
        goto L_7120;

L_7120:
    lphb = lphb->lphbNext;
    if ((LOWORD(lphb) != 0x0))
        goto L_7146;
    else
        goto L_713a;

L_713a:
    if ((HIWORD(lphb) == 0x0))
        goto L_7a64;
    else
        goto L_7140;

L_7140:

L_7146:
    lpbBattle = &(*(lphb + 0x12));
    lpbtldata = lpbBattle;
    goto L_7107;

L_7166:
    if (((lpbtldata->grfPlr & fPlayerCur) == 0x0))
        goto L_7a48;
    else
        goto L_7178;

L_7178:
    i = 0x0;
    goto L_7184;

L_7180:
    i = (i + 0x1);

L_7184:
    if ((i >= game.cPlayer))
        goto L_7222;
    else
        goto L_718f;

L_718f:
    if ((i == iPlayer))
        goto L_7180;
    else
        goto L_719a;

L_719a:
    if ((((rgplr[i].wMdPlr >> 0x8) & 0x1) != 0x0))
        goto L_7180;
    else
        goto L_71b8;

L_71b8:
    if ((((0x1 << i) & lpbtldata->grfPlr) == 0x0))
        goto L_7180;
    else
        goto L_71cf;

L_71cf:
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfeff) | 0x100);
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfff8) | 0x3);

L_7222:
    i = 0x0;
    goto L_722e;

L_722a:
    i = (i + 0x1);

L_722e:
    if ((i >= lpbtldata->ctok))
        goto L_774b;
    else
        goto L_7240;

L_7240:
    if ((lpbtldata->rgtok[i].iplr == iPlayer))
        goto L_722a;
    else
        goto L_7266;

L_7266:
    if ((lpbtldata->rgtok[i].grobj != grobjPlanet))
        goto L_7460;
    else
        goto L_728a;

L_728a:
    iplr = lpbtldata->rgtok[i].iplr;
    lppl = LpplFromId(lpbtldata->rgtok[i].id);
    if ((((rglpshdefSB[iplr][(lpbtldata->rgtok[i].ishdef - 0x10)].wFlags >> 0x8) & 0x1) != 0x0))
        goto L_73db;
    else
        goto L_731b;

L_731b:
    rglpshdefSB[iplr][(lpbtldata->rgtok[i].ishdef - 0x10)].wFlags = ((rglpshdefSB[iplr][(lpbtldata->rgtok[i].ishdef - 0x10)].wFlags & 0xfeff) | 0x100);
    /* untranslated: ss:[bp-0x2a] = ((part[4:2](rgplr[iplr]) + 0x1000) & 0xf000) */
    /* untranslated: part[4:2](rgplr[iplr]) = (part[4:2](rgplr[iplr]) & 0xfff) */
    /* untranslated: part[4:2](rgplr[iplr]) = (part[4:2](rgplr[iplr]) | ss:[bp-0x2a]) */

L_73db:
    rglpshdefSB[iplr][(lpbtldata->rgtok[i].ishdef - 0x10)].wFlags = ((rglpshdefSB[iplr][(lpbtldata->rgtok[i].ishdef - 0x10)].wFlags & 0xff00) | 0x7);
    goto L_722a;

L_7460:
    lpfl = LpflFromId(lpbtldata->rgtok[i].id);
    if ((lpfl->iPlayer == iPlayer))
        goto L_7518;
    else
        goto L_7496;

L_7496:
    if ((((rgplr[lpfl->iPlayer].wMdPlr >> 0x8) & 0x1) != 0x0))
        goto L_7518;
    else
        goto L_74b8;

L_74b8:
    rgplr[lpfl->iPlayer].wMdPlr = ((rgplr[lpfl->iPlayer].wMdPlr & 0xfeff) | 0x100);
    rgplr[lpfl->iPlayer].wMdPlr = ((rgplr[lpfl->iPlayer].wMdPlr & 0xfff8) | 0x3);

L_7518:
    if ((((rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags >> 0x8) & 0x1) != 0x0))
        goto L_75fb;
    else
        goto L_7564;

L_7564:
    rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags = ((rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags & 0xfeff) | 0x100);
    rgplr[lpfl->iPlayer].cShDef = (rgplr[lpfl->iPlayer].cShDef + 0x1);

L_75fb:
    rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags = ((rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags & 0xff00) | 0x7);
    if ((lpfl->fDead != 0x0))
        goto L_722a;
    else
        goto L_7693;

L_7693:

L_7699:
    if ((lpfl->fInclude != 0x0))
        goto L_7722;
    else
        goto L_76b0;

L_76b0:
    /* untranslated: ss:[bp-0x28] = ((part[4:2](rgplr[lpfl->iPlayer]) + 0x1) & 0xfff) */
    /* untranslated: part[4:2](rgplr[lpfl->iPlayer]) = (part[4:2](rgplr[lpfl->iPlayer]) & 0xf000) */
    /* untranslated: part[4:2](rgplr[lpfl->iPlayer]) = (part[4:2](rgplr[lpfl->iPlayer]) | ss:[bp-0x28]) */
    lpfl->fInclude = 0x1;
    lpfl->det = 0x0;

L_7722:
    if ((lpfl->det >= 0x3))
        goto L_722a;
    else
        goto L_7734;

L_7734:
    lpfl->det = 0x3;

L_774b:
    if ((lpbtldata->idPlanet == 0xffff))
        goto L_7783;
    else
        goto L_7758;

L_7758:
    lppl = LpplFromId(lpbtldata->idPlanet);
    MarkPlanet(lppl, iPlayer, 0x1);

L_7783:
    if ((lpbtldata->cbData >= 0x400))
        goto L_77b8;
    else
        goto L_7791;

L_7791:
    WriteRt(rtBtlData, lpbtldata->cbData, &(lpbBattle));
    lpbBattle = ((uint8_t *)(lpbBattle) + lpbtldata->cbData);
    goto L_7a52;

L_77b8:
    cb = (LOWORD((lpbtldata->ctok * 0x1d)) + 0xe);
    lpbtlrec = &(lpbBattle[cb]);
    if ((cb >= 0x400))
        goto L_7806;
    else
        goto L_77e8;

L_77e8:
    WriteRt(rtBtlData, cb, &(lpbBattle));
    lpbBattle = ((uint8_t *)(lpbBattle) + cb);
    goto L_78ef;

L_7806:
    ctok = 0x22;
    if ((ctok >= lpbtldata->ctok))
        goto L_7823;
    else
        goto L_781d;

L_781d:
    t_merge_782d_0001 = ctok;
    goto L_782d;

L_7823:
    t_merge_782d_0001 = lpbtldata->ctok;

L_782d:
    ctok = t_merge_782d_0001;
    if ((ctok <= lpbtldata->ctok))
        goto L_784f;
    else
        goto L_7842;

L_7842:
    ctok = lpbtldata->ctok;

L_784f:
    WriteRt(rtBtlData, (LOWORD((ctok * 0x1d)) + 0xe), &(lpbBattle));
    lpbBattle = ((uint8_t *)(lpbBattle) + (LOWORD((ctok * 0x1d)) + 0xe));
    ctok = (lpbtldata->ctok - ctok);
    goto L_78e6;

L_788e:
    if ((ctok <= 0x23))
        goto L_78bb;
    else
        goto L_7899;

L_7899:
    WriteRt(rtContinue, 0x3f7, &(lpbBattle));
    lpbBattle = (lpbBattle + 0x3f7);
    ctok = (ctok - 0x23);
    goto L_78e6;

L_78bb:
    WriteRt(rtContinue, LOWORD((ctok * 0x1d)), &(lpbBattle));
    lpbBattle = ((uint8_t *)(lpbBattle) + LOWORD((ctok * 0x1d)));
    ctok = 0x0;

L_78e6:
    if ((ctok > 0x0))
        goto L_788e;
    else
        goto L_78ef;

L_78ef:
    cb = ((lpbtldata->cbData + 0xfff2) - LOWORD((lpbtldata->ctok * 0x1d)));
    if ((cb >= 0x400))
        goto L_7a3c;
    else
        goto L_7917;

L_7917:
    WriteRt(rtContinue, cb, &(lpbtlrec));
    lpbBattle = ((uint8_t *)(lpbBattle) + cb);
    goto L_7a52;

L_7938:
    cbRec = ((lpbtlrec->ctok * 0x8) + 0x6);
    if ((cbRec < 0x400))
        goto L_79c6;
    else
        goto L_7955;

L_7955:
    cb = (cb - cbRec);
    goto L_798a;

L_795e:
    WriteRt(rtContinue, 0x3ff, &(lpbtlrec));
    /* untranslated: lpbtlrec = &part[1017:0](lpbtlrec->rgkill) */
    cbRec = (cbRec - 0x3ff);

L_798a:
    if ((cbRec >= 0x400))
        goto L_795e;
    else
        goto L_7994;

L_7994:
    if ((cbRec == 0x0))
        goto L_7a30;
    else
        goto L_799d;

L_799d:
    WriteRt(rtContinue, cbRec, &(lpbtlrec));
    lpbtlrec = ((uint8_t *)(lpbtlrec) + cbRec);

L_79c6:
    cbT = 0x0;

L_79cb:
    cbT = (cbT + cbRec);
    lpbtlrec = ((uint8_t *)(lpbtlrec) + cbRec);
    cb = (cb - cbRec);
    if ((cb == 0x0))
        goto L_7a04;
    else
        goto L_79f1;

L_79f1:
    cbRec = ((lpbtlrec->ctok * 0x8) + 0x6);

L_7a04:
    if ((cb == 0x0))
        goto L_7a1b;
    else
        goto L_7a0d;

L_7a0d:
    if (((cbT + cbRec) < 0x400))
        goto L_79cb;
    else
        goto L_7a1b;

L_7a1b:
    WriteRt(rtContinue, cbT, &(lpbBattle));

L_7a30:
    lpbBattle = lpbtlrec;

L_7a3c:
    if ((cb == 0x0))
        goto L_7a52;
    else
        goto L_7a42;

L_7a42:

L_7a48:
    lpbBattle = ((uint8_t *)(lpbBattle) + lpbtldata->cbData);

L_7a52:
    if ((LOWORD(lphb) != 0x0))
        goto L_70fb;
    else
        goto L_7a5b;

L_7a5b:
    if ((HIWORD(lphb) != 0x0))
        goto L_70fb;
    else
        goto L_7a64;

L_7a64:
    return;
}

void WritePlanet(PLANET *lppl, RecordType rt, int16_t fHistory) {
    uint8_t  bMask;
    uint8_t  rgb[80];
    uint8_t *pbBase;
    int16_t  i;
    uint8_t *pb;
    uint16_t t_merge_7b14_0001;
    uint16_t t_merge_7bf1_0001;

L_7a6a:
    memset(rgb, 0x0, 0x50);
    LOWORD(rgb) = ((LOWORD(rgb) & 0xf800) | (lppl->id & 0x7ff));
    LOWORD(rgb) = ((LOWORD(rgb) & 0x7ff) | ((lppl->iPlayer & 0x1f) << 0xb));
    rgb[0x2] = ((rgb[0x2] & 0xff80) | (lppl->det & 0x7f));
    if ((rt != rtPlanetB))
        goto L_7b2a;
    else
        goto L_7af0;

L_7af0:
    if ((lppl->det <= 0x3))
        goto L_7b2a;
    else
        goto L_7b02;

L_7b02:
    if ((fHistory == 0x0))
        goto L_7b11;
    else
        goto L_7b0b;

L_7b0b:
    t_merge_7b14_0001 = 0x3;
    goto L_7b14;

L_7b11:
    t_merge_7b14_0001 = 0x4;

L_7b14:
    rgb[0x2] = ((rgb[0x2] & 0xff80) | (t_merge_7b14_0001 & 0x7f));

L_7b2a:
    rgb[0x2] = ((rgb[0x2] & 0xfeff) | ((lppl->fInclude & 0x1) << 0x8));
    rgb[0x2] = ((rgb[0x2] & 0xfdff) | ((lppl->fStarbase & 0x1) << 0x9));
    rgb[0x2] = ((rgb[0x2] & 0xff7f) | ((lppl->fHomeworld & 0x1) << 0x7));
    rgb[0x2] = ((rgb[0x2] & 0x7fff) | ((lppl->fFirstYear & 0x1) << 0xf));
    if ((lppl->idRoute == 0x0))
        goto L_7bee;
    else
        goto L_7be8;

L_7be8:
    t_merge_7bf1_0001 = 0x1;
    goto L_7bf1;

L_7bee:
    t_merge_7bf1_0001 = 0x0;

L_7bf1:
    rgb[0x2] = ((rgb[0x2] & 0xbfff) | ((t_merge_7bf1_0001 & 0x1) << 0xe));
    pbBase = &(rgb[0x4]);
    pb = pbBase;
    if (((rgb[0x2] & 0x7f) <= 0x1))
        goto LFinishBRecord;
    else
        goto L_7c24;

L_7c24:

L_7c2a:
    pb = (pbBase + 0x1);
    bMask = 0x3;
    i = 0x0;
    goto L_7c55;

L_7c41:
    i = (i + 0x1);
    bMask = LOBYTE((bMask * 0x4));

L_7c55:
    if ((i >= 0x3))
        goto L_7cad;
    else
        goto L_7c5e;

L_7c5e:
    if ((lppl->rgpctMinLevel[i] <= 0x0))
        goto L_7c41;
    else
        goto L_7c7e;

L_7c7e:
    *(pbBase) = (*(pbBase) | LOBYTE((bMask & 0x55)));
    pb = (pb + 0x1);
    *(pb) = lppl->rgpctMinLevel[i];

L_7cad:
    i = 0x0;
    goto L_7cc3;

L_7cb5:
    i = (i + 0x1);
    pb = (pb + 0x1);

L_7cc3:
    if ((i >= 0x3))
        goto L_7ce9;
    else
        goto L_7ccc;

L_7ccc:
    *(pb) = lppl->rgMinConc[i];
    goto L_7cb5;

L_7ce9:
    i = 0x0;
    goto L_7d5a;

L_7cf1:
    pb = (pb + 0x1);
    *(pb) = lppl->rgEnvVar[i];
    if (((uint16_t)(lppl->rgEnvVar[i]) == (uint16_t)(lppl->rgEnvVarOrig[i])))
        goto L_7d56;
    else
        goto L_7d4a;

L_7d4a:
    rgb[0x2] = ((rgb[0x2] & 0xfbff) | 0x400);

L_7d56:
    i = (i + 0x1);

L_7d5a:
    if ((i < 0x3))
        goto L_7cf1;
    else
        goto L_7d63;

L_7d63:
    if ((((rgb[0x2] >> 0xa) & 0x1) == 0x0))
        goto L_7da9;
    else
        goto L_7d76;

L_7d76:
    i = 0x0;
    goto L_7da0;

L_7d7e:
    pb = (pb + 0x1);
    *(pb) = lppl->rgEnvVarOrig[i];
    i = (i + 0x1);

L_7da0:
    if ((i < 0x3))
        goto L_7d7e;
    else
        goto L_7da9;

L_7da9:
    if ((lppl->iPlayer == 0xffff))
        goto L_7dc6;
    else
        goto L_7db6;

L_7db6:
    pb = lppl->uGuesses;
    pb = (pb + 0x2);

L_7dc6:
    if (((rgb[0x2] & 0x7f) <= 0x3))
        goto LFinishBRecord;
    else
        goto L_7dd1;

L_7dd1:

L_7dd7:
    pbBase = pb;
    pb = (pb + 0x1);
    bMask = 0x3;
    i = 0x0;
    goto L_7e01;

L_7ded:
    i = (i + 0x1);
    bMask = LOBYTE((bMask * 0x4));

L_7e01:
    if ((i >= 0x4))
        goto L_7f5b;
    else
        goto L_7e0a;

L_7e0a:
    if ((i != 0x3))
        goto L_7e28;
    else
        goto L_7e13;

L_7e13:
    if ((lppl->det < 0x7))
        goto L_7ded;
    else
        goto L_7e22;

L_7e22:

L_7e28:
    if ((HIWORD(lppl->rgwtMin[i]) < 0x0))
        goto L_7ded;
    else
        goto L_7e48;

L_7e48:
    if ((HIWORD(lppl->rgwtMin[i]) > 0x0))
        goto L_7e56;
    else
        goto L_7e4d;

L_7e4d:
    if ((LOWORD(lppl->rgwtMin[i]) <= 0x0))
        goto L_7ded;
    else
        goto L_7e56;

L_7e56:
    if ((HIWORD(lppl->rgwtMin[i]) < 0x0))
        goto L_7f24;
    else
        goto L_7e76;

L_7e76:
    if ((HIWORD(lppl->rgwtMin[i]) > 0x0))
        goto L_7e85;
    else
        goto L_7e7b;

L_7e7b:
    if ((LOWORD(lppl->rgwtMin[i]) <= 0xff))
        goto L_7f24;
    else
        goto L_7e85;

L_7e85:
    if ((HIWORD(lppl->rgwtMin[i]) < 0x0))
        goto L_7eed;
    else
        goto L_7ea5;

L_7ea5:
    if ((HIWORD(lppl->rgwtMin[i]) > 0x0))
        goto L_7eb3;
    else
        goto L_7eaa;

L_7eaa:
    if ((LOWORD(lppl->rgwtMin[i]) <= 0xffff))
        goto L_7eed;
    else
        goto L_7eb3;

L_7eb3:
    *(pbBase) = (*(pbBase) | LOBYTE((bMask & 0xff)));
    pb = LOWORD(lppl->rgwtMin[i]);
    pb[0x2] = HIWORD(lppl->rgwtMin[i]);
    pb = (pb + 0x4);
    goto L_7ded;

L_7eed:
    *(pbBase) = (*(pbBase) | LOBYTE((bMask & 0xaa)));
    pb = LOWORD(lppl->rgwtMin[i]);
    pb = (pb + 0x2);

L_7f24:
    *(pbBase) = (*(pbBase) | LOBYTE((bMask & 0x55)));
    pb = (pb + 0x1);
    *(pb) = LOBYTE(LOWORD(lppl->rgwtMin[i]));

L_7f5b:
    if ((*(pbBase) != 0x0))
        goto L_7f74;
    else
        goto L_7f6b;

L_7f6b:
    pb = pbBase;
    goto L_7f80;

L_7f74:
    rgb[0x2] = ((rgb[0x2] & 0xdfff) | 0x2000);

L_7f80:
    if ((rt != rtPlanetB))
        goto L_7feb;
    else
        goto LFinishBRecord;

LFinishBRecord:
    if ((lppl->fStarbase == 0x0))
        goto L_7fb3;
    else
        goto L_7fa0;

L_7fa0:
    *(pb) = LOBYTE(lppl->isb);
    pb = (pb + 0x1);

L_7fb3:
    if ((fHistory == 0x0))
        goto L_7fcc;
    else
        goto L_7fbc;

L_7fbc:
    pb = lppl->turn;
    pb = (pb + 0x2);

L_7fcc:
    WriteRt(rtPlanetB, rgb[pb], &(rgb));
    goto L_81bf;

L_7feb:
    /* untranslated: rgb[0x2] = ((rgb[0x2] & 0xefff) | (((loword((uint32_t)(words(*(lppl+0x18), *(lppl+0x1a)) >> 0x16)) & 0x1) & 0x1) << 0xc)) */
    if ((lppl->iPlayer == 0xffff))
        goto L_8077;
    else
        goto L_802d;

L_802d:
    if ((lppl->iDeltaPop != 0x0))
        goto L_8116;
    else
        goto L_8046;

L_8046:
    if ((0x0 != 0x0))
        goto L_8116;
    else
        goto L_804e;

L_804e:
    /* untranslated: branch (loword((uint32_t)(words(*(lppl+0x18), *(lppl+0x1a)) >> 0x17)) & 0x1) != 0x0 ? L_8116 : L_806f */

L_806f:
    if ((0x0 != 0x0))
        goto L_8116;
    else
        goto L_8077;

L_8077:
    /* untranslated: branch (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff) != 0x0 ? L_8116 : L_8098 */

L_8098:
    if ((0x0 != 0x0))
        goto L_8116;
    else
        goto L_80a0;

L_80a0:
    /* untranslated: branch (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff) != 0x0 ? L_8116 : L_80c1 */

L_80c1:
    if ((0x0 != 0x0))
        goto L_8116;
    else
        goto L_80c9;

L_80c9:
    if ((lppl->cDefenses != 0x0))
        goto L_8116;
    else
        goto L_80e2;

L_80e2:
    if ((0x0 != 0x0))
        goto L_8116;
    else
        goto L_80ea;

L_80ea:
    /* untranslated: branch (loword((uint32_t)(words(*(lppl+0x18), *(lppl+0x1a)) >> 0xc)) & 0x1f) != 0x1f ? L_8116 : L_810b */

L_810b:
    if ((0x0 == 0x0))
        goto L_8146;
    else
        goto L_8110;

L_8110:

L_8116:
    rgb[0x2] = ((rgb[0x2] & 0xf7ff) | 0x800);
    fmemmove(&(pb), &(*(lppl + 0x14)), 0x8);
    pb = (pb + 0x8);

L_8146:
    if ((lppl->iPlayer == 0xffff))
        goto L_81a3;
    else
        goto L_8153;

L_8153:
    if ((lppl->fStarbase == 0x0))
        goto L_8181;
    else
        goto L_816a;

L_816a:
    pb = *(lppl + 0x2c);
    pb[0x2] = *(lppl + 0x2e);
    pb = (pb + 0x4);

L_8181:
    if ((lppl->idRoute == 0x0))
        goto L_81a3;
    else
        goto L_8193;

L_8193:
    pb = lppl->wRouting;
    pb = (pb + 0x2);

L_81a3:
    WriteRt(rtPlanet, rgb[pb], &(rgb));

L_81bf:
    return;
}

void WriteFleet(FLEET *lpfl) {
    uint16_t *pus;
    uint8_t   rgb[134];
    uint16_t  us;
    int16_t   i;
    uint8_t  *pb;
    int16_t   fByte;
    uint16_t  grMask;
    int32_t   wt;

L_81c6:
    fmemmove(&(rgb), &(lpfl), 0xc);
    fByte = 0x1;
    grMask = 0x1;
    us = 0x0;
    i = 0x0;
    goto L_8218;

L_8204:
    i = (i + 0x1);
    grMask = (grMask * 0x2);

L_8218:
    if ((i >= 0x10))
        goto L_8270;
    else
        goto L_8222;

L_8222:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_8204;
    else
        goto L_8240;

L_8240:
    us = (us | grMask);
    if ((lpfl->rgcsh[i] <= 0xff))
        goto L_8204;
    else
        goto L_8267;

L_8267:
    fByte = 0x0;

L_8270:
    rgb[0x4] = ((rgb[0x4] & 0xf7ff) | ((fByte & 0x1) << 0xb));
    /* untranslated: ss:[&rgb[0xc]] = us */
    pb = &(rgb[0xe]);
    if ((fByte == 0x0))
        goto L_8305;
    else
        goto L_82a9;

L_82a9:
    i = 0x0;
    goto L_82f8;

L_82b2:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_82f3;
    else
        goto L_82d0;

L_82d0:
    pb = (pb + 0x1);
    *(pb) = LOBYTE(lpfl->rgcsh[i]);

L_82f3:
    i = (i + 0x1);

L_82f8:
    if ((i >= 0x10))
        goto L_836a;
    else
        goto L_82ff;

L_82ff:

L_8305:
    pus = pb;
    i = 0x0;
    goto L_8359;

L_8315:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_8354;
    else
        goto L_8333;

L_8333:
    pus = (pus + 0x2);
    *(pus) = lpfl->rgcsh[i];

L_8354:
    i = (i + 0x1);

L_8359:
    if ((i < 0x10))
        goto L_8315;
    else
        goto L_8363;

L_8363:
    pb = pus;

L_836a:
    if ((lpfl->det < 0x4))
        goto L_8526;
    else
        goto L_837c;

L_837c:
    fByte = 0x0;
    grMask = 0x3;
    pus = pb;
    pb = (pb + 0x2);
    us = 0x0;
    i = 0x0;
    goto L_83b7;

L_83a3:
    i = (i + 0x1);
    grMask = (grMask * 0x4);

L_83b7:
    if ((i >= 0x5))
        goto L_851d;
    else
        goto L_83c1;

L_83c1:
    if ((HIWORD(lpfl->rgwtMin[i]) < 0x0))
        goto L_83a3;
    else
        goto L_83e2;

L_83e2:
    if ((HIWORD(lpfl->rgwtMin[i]) > 0x0))
        goto L_83f0;
    else
        goto L_83e7;

L_83e7:
    if ((LOWORD(lpfl->rgwtMin[i]) <= 0x0))
        goto L_83a3;
    else
        goto L_83f0;

L_83f0:
    if ((lpfl->det == 0x7))
        goto L_8416;
    else
        goto L_8402;

L_8402:
    if ((i == 0x4))
        goto L_83a3;
    else
        goto L_840c;

L_840c:
    if ((i == 0x3))
        goto L_83a3;
    else
        goto L_8416;

L_8416:
    if ((HIWORD(lpfl->rgwtMin[i]) < 0x0))
        goto L_84e6;
    else
        goto L_8437;

L_8437:
    if ((HIWORD(lpfl->rgwtMin[i]) > 0x0))
        goto L_8446;
    else
        goto L_843c;

L_843c:
    if ((LOWORD(lpfl->rgwtMin[i]) <= 0xff))
        goto L_84e6;
    else
        goto L_8446;

L_8446:
    if ((HIWORD(lpfl->rgwtMin[i]) < 0x0))
        goto L_84af;
    else
        goto L_8467;

L_8467:
    if ((HIWORD(lpfl->rgwtMin[i]) > 0x0))
        goto L_8475;
    else
        goto L_846c;

L_846c:
    if ((LOWORD(lpfl->rgwtMin[i]) <= 0xffff))
        goto L_84af;
    else
        goto L_8475;

L_8475:
    us = (us | (grMask & 0x3ff));
    pb = LOWORD(lpfl->rgwtMin[i]);
    pb[0x2] = HIWORD(lpfl->rgwtMin[i]);
    pb = (pb + 0x4);
    goto L_83a3;

L_84af:
    us = (us | (grMask & 0x2aa));
    pb = LOWORD(lpfl->rgwtMin[i]);
    pb = (pb + 0x2);

L_84e6:
    us = (us | (grMask & 0x155));
    *(pb) = LOBYTE(LOWORD(lpfl->rgwtMin[i]));
    pb = (pb + 0x1);

L_851d:
    *(pus) = us;

L_8526:
    if ((lpfl->det >= 0x7))
        goto L_865e;
    else
        goto L_8538;

L_8538:
    wt = 0x0;
    pb = *(lpfl + 0x74);
    pb[0x2] = *(lpfl + 0x76);
    pb = (pb + 0x4);
    i = 0x0;
    goto L_85de;

L_856c:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_85d9;
    else
        goto L_858a;

L_858a:
    /* untranslated: wt = (wt + (uint32_t)(sext16to32(lpfl->rgcsh[i]) * words(0x0, rglpshdef[lpfl->iPlayer][i].hul.wtEmpty))) */

L_85d9:
    i = (i + 0x1);

L_85de:
    if ((i < 0x10))
        goto L_856c;
    else
        goto L_85e8;

L_85e8:
    i = 0x0;
    goto L_861c;

L_85f1:
    wt = (wt + lpfl->rgwtMin[i]);
    i = (i + 0x1);

L_861c:
    if ((i <= 0x3))
        goto L_85f1;
    else
        goto L_8626;

L_8626:
    pb = LOWORD(wt);
    pb[0x2] = HIWORD(wt);
    pb = (pb + 0x4);
    WriteRt(0x11, rgb[pb], &(rgb));
    goto L_87ae;

L_865e:
    grMask = 0x1;
    us = 0x0;
    i = 0x0;
    goto L_8687;

L_8673:
    i = (i + 0x1);
    grMask = (grMask * 0x2);

L_8687:
    if ((i >= 0x10))
        goto L_86ba;
    else
        goto L_8691;

L_8691:
    /* untranslated: branch HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 0x2))] == 0x0 ? L_8673 : L_86af */

L_86af:
    us = (us | grMask);

L_86ba:
    pb = us;
    pb = (pb + 0x2);
    pus = pb;
    i = 0x0;
    goto L_8722;

L_86de:
    /* untranslated: branch HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 0x2))] == 0x0 ? L_871d : L_86fc */

L_86fc:
    pus = (pus + 0x2);
    /* untranslated: *pus = HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 0x2))] */

L_871d:
    i = (i + 0x1);

L_8722:
    if ((i < 0x10))
        goto L_86de;
    else
        goto L_872c;

L_872c:
    pb = pus;
    pb = (pb + 0x1);
    *(pb) = lpfl->iplan;
    pb = (pb + 0x1);
    *(pb) = LOBYTE(lpfl->cord);
    WriteRt(rtFleetA, rgb[pb], &(rgb));
    WriteOrders(lpfl);
    if ((LOWORD(lpfl->lpszName) != 0x0))
        goto L_879b;
    else
        goto L_8791;

L_8791:
    if ((HIWORD(lpfl->lpszName) == 0x0))
        goto L_87ae;
    else
        goto L_879b;

L_879b:
    WriteRtString(lpfl->lpszName);

L_87ae:
    return;
}

void WriteRtString(char *lpsz) {
    uint8_t rgb[33];
    int16_t cOut;

L_87b4:
    if ((LOWORD(lpsz) != 0x0))
        goto L_87cf;
    else
        goto L_87c6;

L_87c6:
    if ((HIWORD(lpsz) == 0x0))
        goto L_8857;
    else
        goto L_87cf;

L_87cf:
    if (((uint16_t)(*(lpsz)) == 0x0))
        goto L_8857;
    else
        goto L_87db;

L_87db:

L_87e1:
    cOut = 0x1f;
    if ((FCompressUserString(lpsz, &(rgb[0x1]), &(cOut)) == 0x0))
        goto L_8810;
    else
        goto L_8807;

L_8807:
    rgb[0x0] = LOBYTE(cOut);
    goto L_883d;

L_8810:
    fstrcpy(&(rgb[0x1]), lpsz);
    rgb[0x0] = 0x0;
    cOut = (fstrlen(lpsz) + 0x1);

L_883d:
    WriteRt(rtString, (cOut + 0x1), &(rgb));

L_8857:
    return;
}

void MarkFleet(FLEET *lpfl, int16_t det) {
    int16_t i;
    SHDEF  *lpshdef;

L_885e:
    if ((lpfl->fInclude != 0x0))
        goto L_8983;
    else
        goto L_887e;

L_887e:
    lpshdef = rglpshdef[lpfl->iPlayer];
    lpfl->fInclude = 0x1;
    lpfl->det = 0x0;
    lpfl->fdirValid = 0x1;
    /* untranslated: ss:[bp-0xa] = ((part[4:2](rgplr[lpfl->iPlayer]) + 0x1) & 0xfff) */
    /* untranslated: part[4:2](rgplr[lpfl->iPlayer]) = (part[4:2](rgplr[lpfl->iPlayer]) & 0xf000) */
    /* untranslated: part[4:2](rgplr[lpfl->iPlayer]) = (part[4:2](rgplr[lpfl->iPlayer]) | ss:[bp-0xa]) */
    i = 0x0;
    goto L_897a;

L_8925:
    if ((lpfl->rgcsh[i] == 0x0))
        goto L_8976;
    else
        goto L_8942;

L_8942:
    lpshdef[i].wFlags = ((lpshdef[i].wFlags & 0xfeff) | 0x100);

L_8976:
    i = (i + 0x1);

L_897a:
    if ((i < 0x10))
        goto L_8925;
    else
        goto L_8983;

L_8983:
    if ((lpfl->det >= det))
        goto L_89b1;
    else
        goto L_8997;

L_8997:
    *(lpfl + 0x4) = ((*(lpfl + 0x4) & 0xff00) | (det & 0xff));

L_89b1:
    return;
}

void WriteBattlePlan(BTLPLAN *lpbtlplan, int16_t fLog) {
    uint8_t  rgb[36];
    uint8_t *pb;
    char     szPlanName[32];
    int16_t  cOut;

L_89b8:
    fmemmove(&(rgb), &(lpbtlplan), 0x4);
    if ((lpbtlplan->fDelete == 0x0))
        goto L_89f9;
    else
        goto L_89f0;

L_89f0:
    pb = &(rgb[0x2]);
    goto L_8a94;

L_89f9:
    pb = &(rgb[0x4]);
    fstrcpy(szPlanName, &(lpbtlplan->szName));
    cOut = 0x1f;
    if (((uint16_t)(szPlanName[0x0]) == 0x0))
        goto L_8a67;
    else
        goto L_8a2c;

L_8a2c:
    if ((FCompressUserString(szPlanName, ((uint8_t *)(pb) + 0x1), &(cOut)) == 0x0))
        goto L_8a67;
    else
        goto L_8a53;

L_8a53:
    *(pb) = LOBYTE(cOut);
    pb = (pb + (cOut + 0x1));
    goto L_8a94;

L_8a67:
    strcpy(((uint8_t *)(pb) + 0x1), szPlanName);
    *(pb) = 0x0;
    pb = (pb + (strlen(szPlanName) + 0x2));

L_8a94:
    if ((fLog == 0x0))
        goto L_8ab9;
    else
        goto L_8a9d;

L_8a9d:
    WriteMemRt(0x1e, rgb[pb], rgb);
    goto L_8ad5;

L_8ab9:
    WriteRt(rtBtlPlan, rgb[pb], &(rgb));

L_8ad5:
    return;
}

void MarkPlanet(PLANET *lppl, int16_t iPlr, uint16_t det) {
    SHDEF *lpshdef;

L_8adc:
    if ((lppl->fInclude != 0x0))
        goto L_8b33;
    else
        goto L_8afc;

L_8afc:
    lppl->fInclude = 0x1;
    lppl->det = 0x0;
    rgplr[iPlr].cPlanet = (rgplr[iPlr].cPlanet + 0x1);

L_8b33:
    if ((lppl->det >= det))
        goto L_8b5f;
    else
        goto L_8b45;

L_8b45:
    *(lppl + 0x4) = ((*(lppl + 0x4) & 0xff00) | (det & 0xff));

L_8b5f:
    if ((lppl->iPlayer == 0xffff))
        goto L_8bee;
    else
        goto L_8b6c;

L_8b6c:
    if ((((rgplr[lppl->iPlayer].wMdPlr >> 0x8) & 0x1) != 0x0))
        goto L_8bee;
    else
        goto L_8b8e;

L_8b8e:
    rgplr[lppl->iPlayer].wMdPlr = ((rgplr[lppl->iPlayer].wMdPlr & 0xfeff) | 0x100);
    rgplr[lppl->iPlayer].wMdPlr = ((rgplr[lppl->iPlayer].wMdPlr & 0xfff8) | 0x3);

L_8bee:
    if ((det == 0x2))
        goto L_8cf7;
    else
        goto L_8bf4;

L_8bf4:

L_8bfa:
    if ((lppl->iPlayer == 0xffff))
        goto L_8cf7;
    else
        goto L_8c07;

L_8c07:
    if ((lppl->fStarbase == 0x0))
        goto L_8cf7;
    else
        goto L_8c1e;

L_8c1e:
    lpshdef = &(rglpshdefSB[lppl->iPlayer][lppl->isb]);
    if ((lpshdef->fInclude != 0x0))
        goto L_8cd1;
    else
        goto L_8c5f;

L_8c5f:
    lpshdef->fInclude = 0x1;
    lpshdef->det = 0x0;
    /* untranslated: ss:[bp-0x8] = ((part[4:2](rgplr[lppl->iPlayer]) + 0x1000) & 0xf000) */
    /* untranslated: part[4:2](rgplr[lppl->iPlayer]) = (part[4:2](rgplr[lppl->iPlayer]) & 0xfff) */
    /* untranslated: part[4:2](rgplr[lppl->iPlayer]) = (part[4:2](rgplr[lppl->iPlayer]) | ss:[bp-0x8]) */

L_8cd1:
    if ((lpshdef->det >= 0x3))
        goto L_8cf7;
    else
        goto L_8ce3;

L_8ce3:
    lpshdef->det = 0x3;

L_8cf7:
    return;
}

void SetSzWorkFromDt(DtFileType dt, int16_t iPlayer) {
    char    *pchSlash;
    int16_t  c;
    char    *pchDot;
    uint16_t t_merge_8dc4_0001;

L_8cfe:
    pchDot = strrchr(szBase, 0x2e);
    if ((pchDot == 0x0))
        goto L_8d50;
    else
        goto L_8d23;

L_8d23:
    pchSlash = strrchr(szBase, 0x5c);
    if ((pchSlash == 0x0))
        goto L_8d4a;
    else
        goto L_8d3f;

L_8d3f:
    if ((pchSlash >= pchDot))
        goto L_8d50;
    else
        goto L_8d4a;

L_8d4a:
    *(pchDot) = 0x0;

L_8d50:
    c = _wsprintf(szWork, "%s.", &(szBase));
    goto L_8de4;

L_8d76:
    strcat(szWork, "xy");
    goto L_8e0f;

L_8d89:
    strcat(szWork, "hst");
    goto L_8e0f;

L_8d9c:
    if ((dt != dtLog))
        goto L_8db2;
    else
        goto L_8dac;

L_8dac:
    t_merge_8dc4_0001 = 0x78;
    goto L_8dc4;

L_8db2:
    if ((dt != dtHist))
        goto L_8dc1;
    else
        goto L_8dbb;

L_8dbb:
    t_merge_8dc4_0001 = 0x68;
    goto L_8dc4;

L_8dc1:
    t_merge_8dc4_0001 = 0x6d;

L_8dc4:
    _wsprintf(&(szWork[c]), "%c%d", t_merge_8dc4_0001, (iPlayer + 0x1));
    goto L_8e0f;

L_8de4:
    if ((dt == dtXY))
        goto L_8d76;
    else
        goto L_8dec;

L_8dec:
    if ((dt == dtLog))
        goto L_8d9c;
    else
        goto L_8df4;

L_8df4:
    if ((dt == dtHost))
        goto L_8d89;
    else
        goto L_8dfc;

L_8dfc:
    if ((dt == dtTurn))
        goto L_8d9c;
    else
        goto L_8e04;

L_8e04:
    if ((dt != dtHist))
        goto L_8d76;
    else
        goto L_8e09;

L_8e09:

L_8e0f:
    return;
}

int16_t FCreateFile(DtFileType dt, int16_t iPlayer, char *szForceName) {
    jmp_buf *penvMemSav[9];
    jmp_buf  env[9];
    char    *psz;

L_8e16:
    if ((szForceName == 0x0))
        goto L_8e31;
    else
        goto L_8e28;

L_8e28:
    psz = szForceName;
    goto L_8e44;

L_8e31:
    SetSzWorkFromDt(dt, iPlayer);
    psz = 0x57a4;

L_8e44:
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_8e70;
    else
        goto L_8e64;

L_8e64:
    penvMem = penvMemSav;
    return 0x0;

L_8e70:
    StreamOpen(psz, 0x1012);
    WriteBOF(iPlayer, dt, 0x0);
    penvMem = penvMemSav;
    return 0x1;
}

void WriteBOF(int16_t iPlayer, int16_t dt, int16_t fMulti) {
    RTBOF    rtbof;
    uint16_t t_merge_900d_0001;

L_8ea4:
    memset(rtbof, 0x0, 0x10);
    strncpy(rtbof.rgid, "J3J3", 0x4);
    rtbof.lidGame = game.lid;
    rtbof.wGen = game.wGen;
    rtbof.verInc = 0x0;
    rtbof.verMinor = 0x53;
    rtbof.verMajor = 0x2;
    rtbof.turn = game.turn;
    rtbof.fCrippled = 0x0;
    rtbof.iPlayer = iPlayer;
    /* untranslated: ss:[bp-0x16] = Random(0x7d0) */
    /* untranslated: part[12:2](rtbof) = (rtbof.iPlayer | (((loword(GetTickCount()) + ss:[bp-0x16]) & 0x7ff) << 0x5)) */
    rtbof.dt = dt;
    rtbof.fDone = gd.fSubmit;
    rtbof.fInUse = gd.fHostMode;
    if ((dt != 0x2))
        goto L_900a;
    else
        goto L_8ff6;

L_8ff6:
    if ((gd.fGameOverMan == 0x0))
        goto L_900a;
    else
        goto L_9004;

L_9004:
    t_merge_900d_0001 = 0x1;
    goto L_900d;

L_900a:
    t_merge_900d_0001 = 0x0;

L_900d:
    rtbof.fGameOverMan = t_merge_900d_0001;
    WriteRt(rtBOF, 0x10, &(rtbof));

L_9043:
    return;
}

int16_t FMarkFile(DtFileType dt, int16_t iPlayer, int16_t mdMark, int16_t f) {
    StringId ids;
    RTBOF    rtbof;
    jmp_buf *penvMemSav[9];
    jmp_buf  env[9];
    int16_t  fChange;
    int16_t  fSuccess;
    int16_t  fSilentSav;
    int32_t  lSeedSav2;
    int32_t  lSeedSav1;
    uint16_t t_merge_93ba_0001;

L_904a:
    fSilentSav = fFileErrSilent;
    fSuccess = 0x0;
    ids = idsUniverseDefinitionFileSeemsMissingCorrupt;
    SetSzWorkFromDt((dt & 0xff), iPlayer);
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_90c0;
    else
        goto L_9095;

L_9095:
    fFileErrSilent = fSilentSav;
    if ((ids == idsUniverseDefinitionFileSeemsMissingCorrupt))
        goto L_90af;
    else
        goto L_90a4;

L_90a4:
    FileError(ids);

L_90af:
    StreamClose();
    penvMem = penvMemSav;
    return 0x0;

L_90c0:
    fFileErrSilent = 0x1;
    StreamOpen(szWork, 0x12);
    fFileErrSilent = fSilentSav;
    ids = idsGameFileAppearsCorruptUnableLoadFile;
    ReadRt();
    if ((hdrCur.rt == rtBOF))
        goto L_910b;
    else
        goto L_90f9;

L_90f9:
    FileError(idmColonistsDroppedDestroyedSpiritedFighting);
    goto LBadFile;

L_910b:
    if ((((rgbCur[0x8] >> 0xc) & 0xf) < 0x2))
        goto L_9149;
    else
        goto L_911e;

L_911e:
    if ((((rgbCur[0x8] >> 0xc) & 0xf) != 0x2))
        goto L_915b;
    else
        goto L_9131;

L_9131:
    if ((((rgbCur[0x8] >> 0x5) & 0x7f) >= 0x31))
        goto L_915b;
    else
        goto L_9149;

L_9149:
    FileError(0x4d3);
    goto LBadFile;

L_915b:
    if ((((rgbCur[0x8] >> 0xc) & 0xf) > 0x2))
        goto L_9199;
    else
        goto L_916e;

L_916e:
    if ((((rgbCur[0x8] >> 0xc) & 0xf) != 0x2))
        goto L_91a8;
    else
        goto L_9181;

L_9181:
    if ((((rgbCur[0x8] >> 0x5) & 0x7f) < 0x54))
        goto L_91a8;
    else
        goto L_9199;

L_9199:
    FileError(0x2ca);
    goto LBadFile;

L_91a8:
    /* untranslated: rtbof = part[0:16](rgbCur) */
    if ((LOWORD(game.lid) != 0x0))
        goto L_91cc;
    else
        goto L_91c2;

L_91c2:
    if ((HIWORD(game.lid) == 0x0))
        goto LBadFile;
    else
        goto L_91cc;

L_91cc:
    if ((LOWORD(rtbof.lidGame) != LOWORD(game.lid)))
        goto L_91e3;
    else
        goto L_91db;

L_91db:
    if ((HIWORD(rtbof.lidGame) == HIWORD(game.lid)))
        goto L_91f2;
    else
        goto L_91e3;

L_91e3:
    FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
    goto LBadFile;

L_91f2:
    fChange = 0x0;
    goto L_93d2;

L_91fd:
    if ((rtbof.fInUse == f))
        goto L_93f5;
    else
        goto L_9212;

L_9212:
    rtbof.fInUse = f;
    fChange = 0x1;

L_9239:
    if ((rtbof.fDone == f))
        goto L_93f5;
    else
        goto L_924e;

L_924e:
    rtbof.fDone = f;
    fChange = 0x1;

L_9275:
    if ((rtbof.fMulti == f))
        goto L_93f5;
    else
        goto L_928a;

L_928a:
    rtbof.fMulti = f;
    fChange = 0x1;

L_92b1:
    GetFileSeeds(&(lSeedSav1), &(lSeedSav2));
    ReadRt();
    if ((hdrCur.rt != rtPlr))
        goto L_92b1;
    else
        goto L_92d9;

L_92d9:
    if (((uint16_t)(LOWORD(rgbCur)) != iPlayer))
        goto L_92b1;
    else
        goto L_92e5;

L_92e5:
    if ((((rgbCur[0x6] >> 0x9) & 0x1) == f))
        goto L_93f5;
    else
        goto L_92fa;

L_92fa:
    if ((((rgbCur[0x6] >> 0x9) & 0x1) == 0x0))
        goto L_9332;
    else
        goto L_930d;

L_930d:
    if ((((rgbCur[0x6] >> 0xd) & 0x7) != 0x7))
        goto LBadFile;
    else
        goto L_931d;

L_931d:

L_9323:
    rgbCur[0x6] = ((rgbCur[0x6] & 0xfdff) | 0x0);
    goto L_934a;

L_9332:
    rgbCur[0x6] = ((rgbCur[0x6] & 0xfdff) | 0x200);
    rgbCur[0x6] = ((rgbCur[0x6] & 0x1fff) | 0xe000);

L_934a:
    /* untranslated: rgbCur[0xc] = (rgbCur[0xc] ~ 0x0) */
    /* untranslated: rgbCur[0xe] = (rgbCur[0xe] ~ 0x0) */
    /* untranslated: call lseek(hf, sext16to32(((hdrCur.cb + 0x2) neg 0x0)), 0x1) -> callresult(int32_t) */
    SetFileSeeds(lSeedSav1, lSeedSav2);
    WriteRt(rtPlr, hdrCur.cb, rgbCur);
    if ((dt != dtTurn))
        goto L_93b7;
    else
        goto L_93b1;

L_93b1:
    t_merge_93ba_0001 = 0x1;
    goto L_93ba;

L_93b7:
    t_merge_93ba_0001 = 0x0;

L_93ba:
    fChange = t_merge_93ba_0001;
    rtbof.fDone = 0x0;

L_93d2:
    if ((mdMark == 0x1))
        goto L_91fd;
    else
        goto L_93da;

L_93da:
    if ((mdMark == 0x2))
        goto L_9239;
    else
        goto L_93e2;

L_93e2:
    if ((mdMark == 0x4))
        goto L_9275;
    else
        goto L_93ea;

L_93ea:
    if ((mdMark == 0x8))
        goto L_92b1;
    else
        goto L_93f5;

L_93f5:
    if ((fChange == 0x0))
        goto L_942d;
    else
        goto L_93fe;

L_93fe:
    lseek(hf, 0x0, 0x0);
    WriteRt(rtBOF, 0x10, &(rtbof));

L_942d:
    fSuccess = 0x1;

LBadFile:
    if (((dt & 0x2000) == 0x0))
        goto L_9464;
    else
        goto L_9440;

L_9440:
    if ((fSuccess == 0x0))
        goto L_9464;
    else
        goto L_9449;

L_9449:
    lseek(hf, 0x0, 0x2);
    goto L_9469;

L_9464:
    StreamClose();

L_9469:
    penvMem = penvMemSav;
    return fSuccess;
}

void WriteRt(RecordType rt, int16_t cb, void *rg) {
    HDR hdr;

L_947c:
    fmemmove(rgbCur, rg, cb);
    if ((rt != rtBOF))
        goto L_94e6;
    else
        goto L_94a7;

L_94a7:
    /* untranslated: call SetFileXorStream(words(rgbCur[0x6], rgbCur[0x4]), (rgbCur[0xc] >> 0x5), rgbCur[0xa], ((rgbCur[0xc] << 0xb) >> 0xb), ((rgbCur[0xe] >>
     * 0xc) & 0x1)) -> callresult(void) */
    goto L_94fe;

L_94e6:
    if ((rt == rtEOF))
        goto L_94fe;
    else
        goto L_94ef;

L_94ef:
    XorFileBuf(rgbCur, cb);

L_94fe:
    hdr.cb = cb;
    hdr = (hdr.cb | ((rt & 0x3f) << 0xa));
    RgToStream(&(hdr), 0x2);
    RgToStream(rgbCur, cb);
    return;
}

void RgToStream(void *rg, uint16_t cb) {
L_9554:
    if ((cb == 0x0))
        goto L_95b6;
    else
        goto L_9563;

L_9563:

L_9569:
    if ((_lwrite(hf, rg, cb) == cb))
        goto L_95b6;
    else
        goto L_9585;

L_9585:
    AlertSz(PszFormatIds(idsErrorWritingFile, 0x0), MB_ICONHAND);
    longjmp(penvMem, 0xffff);

L_95b6:
    return;
}

void SetVisiblePlanFleet(int16_t iPlr) {
L_95bc:
    SetVisPFInit(iPlr);
    if ((iPlr != 0xffff))
        goto L_95e2;
    else
        goto L_95d9;

L_95d9:
    rgplr[0x0].cPlanet = game.cPlanMax;
    goto L_964d;

L_95e2:
    if ((iPlr == 0xffff))
        goto L_95f7;
    else
        goto L_95eb;

L_95eb:
    UpdateProgressGauge(0xfc61);

L_95f7:
    SetVisPFFleets(iPlr);
    if ((iPlr == 0xffff))
        goto L_9617;
    else
        goto L_960b;

L_960b:
    UpdateProgressGauge(0xfc61);

L_9617:
    SetVisPFPlanets(iPlr);
    if ((iPlr == 0xffff))
        goto L_9637;
    else
        goto L_962b;

L_962b:
    UpdateProgressGauge(0xfc61);

L_9637:
    SetVisPFThings(iPlr);
    SetVisPFFinish(iPlr);

L_964d:
    return;
}

void SetVisPFInit(int16_t iPlr) {
    PLANET  *lpplMac;
    uint16_t detNew;
    PLANET  *lppl;
    int16_t  j;
    FLEET   *lpfl;
    THING   *lpth;
    int16_t  ifl;
    int16_t  i;
    THING   *lpthMac;
    int16_t  raMajor;
    uint16_t grbitPlr;
    int16_t  iSteal;
    uint16_t t_merge_968f_0001;
    uint16_t t_merge_9dab_0001;
    uint16_t t_merge_9fb0_0001;

L_9654:
    raMajor = GetRaceStat(rgplr[iPlr], rsMajorAdv);
    if ((iPlr != 0xffff))
        goto L_9687;
    else
        goto L_9681;

L_9681:
    t_merge_968f_0001 = 0x0;
    goto L_968f;

L_9687:
    t_merge_968f_0001 = (0x1 << iPlr);

L_968f:
    grbitPlr = t_merge_968f_0001;
    i = 0x0;
    goto L_969e;

L_969a:
    i = (i + 0x1);

L_969e:
    if ((i >= game.cPlayer))
        goto L_96bc;
    else
        goto L_96a9;

L_96a9:
    rgplr[i].cPlanet = 0x0;
    goto L_969a;

L_96bc:
    i = 0x0;
    goto L_96c8;

L_96c4:
    i = (i + 0x1);

L_96c8:
    if ((i >= game.cPlayer))
        goto L_9ace;
    else
        goto L_96d3;

L_96d3:
    if ((iPlr == 0xffff))
        goto L_9700;
    else
        goto L_96dc;

L_96dc:
    if ((iPlr == i))
        goto L_9700;
    else
        goto L_96e7;

L_96e7:
    if (((rgplr[i].wFlags & 0x1) == 0x0))
        goto L_9753;
    else
        goto L_9700;

L_9700:
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfeff) | 0x100);
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfff8) | 0x7);
    goto L_977b;

L_9753:
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfeff) | 0x0);

L_977b:
    /* untranslated: part[4:2](rgplr[i]) = ((part[4:2](rgplr[i]) & 0xf000) | 0x0) */
    rgplr[i].cShDef = 0x0;
    /* untranslated: part[4:2](rgplr[i]) = ((part[4:2](rgplr[i]) & 0xfff) | 0x0) */
    j = 0x0;
    goto L_9932;

L_97e2:
    if ((iPlr == 0xffff))
        goto L_97f6;
    else
        goto L_97eb;

L_97eb:
    if ((iPlr != i))
        goto L_98e8;
    else
        goto L_97f6;

L_97f6:
    if ((((rglpshdef[i][j].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_98e8;
    else
        goto L_9823;

L_9823:
    rglpshdef[i][j].wFlags = ((rglpshdef[i][j].wFlags & 0xfeff) | 0x100);
    rglpshdef[i][j].wFlags = ((rglpshdef[i][j].wFlags & 0xff00) | 0x7);
    rglpshdef[i][j].cExist = 0x0;
    rgplr[i].cShDef = (rgplr[i].cShDef + 0x1);
    goto L_992e;

L_98e8:
    rglpshdef[i][j].wFlags = ((rglpshdef[i][j].wFlags & 0xfeff) | 0x0);

L_992e:
    j = (j + 0x1);

L_9932:
    if ((j < 0x10))
        goto L_97e2;
    else
        goto L_993b;

L_993b:
    j = 0x0;
    goto L_9ac2;

L_9943:
    if ((iPlr == 0xffff))
        goto L_9957;
    else
        goto L_994c;

L_994c:
    if ((iPlr != i))
        goto L_9a78;
    else
        goto L_9957;

L_9957:
    if ((((rglpshdefSB[i][j].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_9a78;
    else
        goto L_9984;

L_9984:
    rglpshdefSB[i][j].wFlags = ((rglpshdefSB[i][j].wFlags & 0xfeff) | 0x100);
    rglpshdefSB[i][j].wFlags = ((rglpshdefSB[i][j].wFlags & 0xff00) | 0x7);
    rglpshdefSB[i][j].cExist = 0x0;
    /* untranslated: ss:[bp-0x24] = ((part[4:2](rgplr[i]) + 0x1000) & 0xf000) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) & 0xfff) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) | ss:[bp-0x24]) */
    goto L_9abe;

L_9a78:
    rglpshdefSB[i][j].wFlags = ((rglpshdefSB[i][j].wFlags & 0xfeff) | 0x0);

L_9abe:
    j = (j + 0x1);

L_9ac2:
    if ((j >= 0xa))
        goto L_96c4;
    else
        goto L_9ac8;

L_9ac8:

L_9ace:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_9bb5;

L_9af9:
    if ((iPlr == 0xffff))
        goto L_9b11;
    else
        goto L_9b02;

L_9b02:
    if ((iPlr != lppl->iPlayer))
        goto L_9b9d;
    else
        goto L_9b11;

L_9b11:
    lppl->fInclude = 0x1;
    lppl->det = 0x7;
    if ((iPlr == 0xffff))
        goto L_9b51;
    else
        goto L_9b42;

L_9b42:
    rgplr[iPlr].cPlanet = (rgplr[iPlr].cPlanet + 0x1);

L_9b51:
    if ((lppl->fStarbase == 0x0))
        goto L_9bb1;
    else
        goto L_9b68;

L_9b68:
    rglpshdefSB[lppl->iPlayer][lppl->isb].cExist = (rglpshdefSB[lppl->iPlayer][lppl->isb].cExist + 0x1);

L_9b9d:
    lppl->fInclude = 0x0;

L_9bb1:
    lppl = (lppl + 0x1);

L_9bb5:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_9af9;
    else
        goto L_9bc3;

L_9bc3:
    ifl = 0x0;
    goto L_9bcf;

L_9bcb:
    ifl = (ifl + 0x1);

L_9bcf:
    if ((ifl >= cFleet))
        goto L_9e4c;
    else
        goto L_9bda;

L_9bda:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_9c0a;
    else
        goto L_9c02;

L_9c02:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_9e4c;
    else
        goto L_9c0a;

L_9c0a:
    lpfl->fdirValid = 0x0;
    lpfl->fMark = 0x0;
    if ((iPlr == 0xffff))
        goto L_9c4a;
    else
        goto L_9c3b;

L_9c3b:
    if ((iPlr != lpfl->iPlayer))
        goto L_9e35;
    else
        goto L_9c4a;

L_9c4a:
    if ((lpfl->fDead != 0x0))
        goto L_9e35;
    else
        goto L_9c61;

L_9c61:
    lpfl->fInclude = 0x1;
    lpfl->det = 0x7;
    /* untranslated: ss:[bp-0x24] = ((part[4:2](rgplr[lpfl->iPlayer]) + 0x1) & 0xfff) */
    /* untranslated: part[4:2](rgplr[lpfl->iPlayer]) = (part[4:2](rgplr[lpfl->iPlayer]) & 0xf000) */
    /* untranslated: part[4:2](rgplr[lpfl->iPlayer]) = (part[4:2](rgplr[lpfl->iPlayer]) | ss:[bp-0x24]) */
    j = 0x0;
    goto L_9d47;

L_9cdb:
    if ((lpfl->rgcsh[j] == 0x0))
        goto L_9d43;
    else
        goto L_9cf8;

L_9cf8:
    rglpshdef[lpfl->iPlayer][j].cExist = (rglpshdef[lpfl->iPlayer][j].cExist + (uint32_t)(lpfl->rgcsh[j]));

L_9d43:
    j = (j + 0x1);

L_9d47:
    if ((j < 0x10))
        goto L_9cdb;
    else
        goto L_9d50;

L_9d50:
    if ((iPlr == 0xffff))
        goto L_9bcb;
    else
        goto L_9d59;

L_9d59:
    if ((lpfl->idPlanet == 0xffff))
        goto L_9bcb;
    else
        goto L_9d66;

L_9d66:
    lppl = &(lpPlanets[lpfl->idPlanet]);
    if ((GetCachedFleetScannerRange(lpfl, 0x0, 0x0, &(iSteal)) < 0x0))
        goto L_9da8;
    else
        goto L_9da2;

L_9da2:
    t_merge_9dab_0001 = 0x3;
    goto L_9dab;

L_9da8:
    t_merge_9dab_0001 = 0x1;

L_9dab:
    detNew = t_merge_9dab_0001;
    if ((iSteal < 0x2))
        goto L_9dbc;
    else
        goto L_9db7;

L_9db7:
    detNew = 0x4;

L_9dbc:
    if ((lpfl->fHereAllTurn == 0x0))
        goto L_9e1e;
    else
        goto L_9dd3;

L_9dd3:
    if ((lppl->iPlayer != 0xffff))
        goto L_9e1e;
    else
        goto L_9de0;

L_9de0:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) != 0x3 ? L_9e1e : L_9df6 */

L_9df6:
    if ((HIWORD(CMineFromLpfl(lpfl)) < 0x0))
        goto L_9e1e;
    else
        goto L_9e0c;

L_9e0c:
    /* untranslated: branch hiword(callresult(int32_t)) > 0x0 ? L_9e19 : L_9e11 */

L_9e11:
    /* untranslated: branch loword(callresult(int32_t)) <= 0x0 ? L_9e1e : L_9e19 */

L_9e19:
    detNew = 0x4;

L_9e1e:
    MarkPlanet(lppl, iPlr, detNew);

L_9e35:
    lpfl->fInclude = 0x0;

L_9e4c:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_a0eb;

L_9e77:
    if ((lpth->ith != ithMineralPacket))
        goto L_9f88;
    else
        goto L_9e8d;

L_9e8d:
    if ((iPlr == 0xffff))
        goto L_9e9f;
    else
        goto L_9e96;

L_9e96:
    if ((raMajor != 0x6))
        goto L_9f71;
    else
        goto L_9e9f;

L_9e9f:
    lpth->thp.fInclude = 0x1;
    if ((((rgplr[lpth->iplr].wMdPlr >> 0x8) & 0x1) != 0x0))
        goto L_a0e7;
    else
        goto L_9ede;

L_9ede:
    rgplr[((lpth->idFull >> 0x9) & 0xf)].wMdPlr = ((rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100);
    rgplr[((lpth->idFull >> 0x9) & 0xf)].wMdPlr = ((rgplr[lpth->iplr].wMdPlr & 0xfff8) | 0x3);

L_9f71:
    lpth->thp.fInclude = 0x0;

L_9f88:
    if ((lpth->ith != ithWormhole))
        goto L_9fd7;
    else
        goto L_9f9e;

L_9f9e:
    if ((iPlr != 0xffff))
        goto L_9fad;
    else
        goto L_9fa7;

L_9fa7:
    t_merge_9fb0_0001 = 0x1;
    goto L_9fb0;

L_9fad:
    t_merge_9fb0_0001 = 0x0;

L_9fb0:
    lpth->thw.fInclude = t_merge_9fb0_0001;
    goto L_a0e7;

L_9fd7:
    if ((lpth->ith != ithMysteryTrader))
        goto L_a004;
    else
        goto L_9fed;

L_9fed:
    lpth->tht.fInclude = 0x1;
    goto L_a0e7;

L_a004:
    if ((lpth->ith != ithMinefield))
        goto L_a0e7;
    else
        goto L_a01a;

L_a01a:
    if (((lpth->thm.grbitPlrNow & grbitPlr) == 0x0))
        goto L_a0e7;
    else
        goto L_a02c;

L_a02c:
    if ((((rgplr[lpth->iplr].wMdPlr >> 0x8) & 0x1) != 0x0))
        goto L_a0e7;
    else
        goto L_a057;

L_a057:
    rgplr[((lpth->idFull >> 0x9) & 0xf)].wMdPlr = ((rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100);
    rgplr[((lpth->idFull >> 0x9) & 0xf)].wMdPlr = ((rgplr[lpth->iplr].wMdPlr & 0xfff8) | 0x3);

L_a0e7:
    lpth = (lpth + 0x1);

L_a0eb:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_9e77;
    else
        goto L_a0f9;

L_a0f9:
    return;
}

void SetVisPFFleets(int16_t iPlr) {
    PLANET  *lpplMac;
    POINT    pt;
    int16_t  pctCloak;
    int16_t  dy;
    FLEET   *lpfl2;
    int32_t  d2;
    PLANET  *lppl;
    int16_t  j;
    FLEET   *lpfl;
    THING   *lpth;
    int32_t  lRadius2;
    int16_t  ifl;
    THING   *lpthMac;
    int16_t  iRadius;
    int16_t  dx;
    uint16_t grbitPlr;
    int32_t  lRadPlanet2;
    int16_t  iRadPlanet;
    int16_t  iSteal;
    int16_t  pctDetect;
    int32_t  l;
    int32_t  lVis2;
    uint16_t t_merge_a120_0001;
    uint16_t t_merge_a268_0001;

L_a100:
    if ((iPlr != 0xffff))
        goto L_a118;
    else
        goto L_a112;

L_a112:
    t_merge_a120_0001 = 0x0;
    goto L_a120;

L_a118:
    t_merge_a120_0001 = (0x1 << iPlr);

L_a120:
    grbitPlr = t_merge_a120_0001;
    ifl = 0x0;
    goto L_a12f;

L_a12b:
    ifl = (ifl + 0x1);

L_a12f:
    if ((ifl >= cFleet))
        goto L_abd7;
    else
        goto L_a13a;

L_a13a:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_a16a;
    else
        goto L_a162;

L_a162:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_abd7;
    else
        goto L_a16a;

L_a16a:
    if ((lpfl->fDead != 0x0))
        goto L_a12b;
    else
        goto L_a17e;

L_a17e:

L_a184:
    if ((lpfl->iPlayer == iPlr))
        goto L_a1ee;
    else
        goto L_a193;

L_a193:
    if ((lpfl->fInclude != 0x0))
        goto L_a12b;
    else
        goto L_a1aa;

L_a1aa:
    if ((lpfl->idPlanet == 0xffff))
        goto L_a12b;
    else
        goto L_a1b7;

L_a1b7:
    if ((lpPlanets[lpfl->idPlanet].iPlayer != iPlr))
        goto L_a12b;
    else
        goto L_a1d9;

L_a1d9:
    MarkFleet(lpfl, 0x3);

L_a1ee:
    if ((lpfl->fBombed == 0x0))
        goto L_a237;
    else
        goto L_a205;

L_a205:
    if ((lpfl->idPlanet == 0xffff))
        goto L_a237;
    else
        goto L_a212;

L_a212:
    MarkPlanet(&(lpPlanets[lpfl->idPlanet]), iPlr, 0x3);

L_a237:
    iRadius = GetCachedFleetScannerRange(lpfl, &(iRadPlanet), &(pctDetect), &(iSteal));
    if ((0x0 <= iRadius))
        goto L_a265;
    else
        goto L_a25f;

L_a25f:
    t_merge_a268_0001 = 0x0;
    goto L_a268;

L_a265:
    t_merge_a268_0001 = iRadius;

L_a268:
    iRadius = t_merge_a268_0001;
    lRadius2 = (uint32_t)(((uint32_t)(iRadius) * (uint32_t)(iRadius)));
    lRadPlanet2 = (uint32_t)(((uint32_t)(iRadPlanet) * (uint32_t)(iRadPlanet)));
    pt.x = lpfl->pt.x;
    pt.y = lpfl->pt.y;
    j = 0x0;
    goto L_a2b6;

L_a2b2:
    j = (j + 0x1);

L_a2b6:
    if ((j >= cFleet))
        goto L_a559;
    else
        goto L_a2c1;

L_a2c1:
    lpfl2 = rglpfl[j];
    if ((LOWORD(rglpfl[j]) != 0x0))
        goto L_a2f1;
    else
        goto L_a2e9;

L_a2e9:
    if ((*(rglpfl[j] + 0x2) == 0x0))
        goto L_a559;
    else
        goto L_a2f1;

L_a2f1:
    if ((lpfl2->fDead != 0x0))
        goto L_a2b2;
    else
        goto L_a305;

L_a305:

L_a30b:
    if (((iSteal & 0x1) == 0x0))
        goto L_a377;
    else
        goto L_a319;

L_a319:
    /* untranslated: branch ss:[pt] != lpfl2->pt.x ? L_a377 : L_a333 */

L_a333:
    /* untranslated: branch ss:[pt+0x2] != lpfl2->pt.y ? L_a377 : L_a33c */

L_a33c:
    if ((lpfl2->fInclude == 0x0))
        goto L_a365;
    else
        goto L_a353;

L_a353:
    if ((lpfl2->det >= 0x4))
        goto L_a377;
    else
        goto L_a365;

L_a365:
    MarkFleet(lpfl2, 0x4);

L_a377:
    if ((lpfl2->fInclude != 0x0))
        goto L_a2b2;
    else
        goto L_a38b;

L_a38b:

L_a391:
    abs((pt.x - lpfl2->pt.x));
    /* untranslated: dx = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_a2b2 : L_a3af */

L_a3af:
    abs((pt.y - lpfl2->pt.y));
    /* untranslated: dy = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_a2b2 : L_a3cd */

L_a3cd:
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x44]) > HIWORD(lRadius2) ? L_a2b2 : L_a40d */

L_a40d:
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x44]) < HIWORD(lRadius2) ? L_a41a : L_a412 */

L_a412:
    /* untranslated: branch (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x46]) > LOWORD(lRadius2) ? L_a2b2 : L_a41a */

L_a41a:
    if ((lpfl2->idPlanet == 0xffff))
        goto L_a442;
    else
        goto L_a427;

L_a427:
    if ((HIWORD(l) > HIWORD(lRadPlanet2)))
        goto L_a2b2;
    else
        goto L_a435;

L_a435:
    if ((HIWORD(l) < HIWORD(lRadPlanet2)))
        goto L_a442;
    else
        goto L_a43a;

L_a43a:
    if ((LOWORD(l) > LOWORD(lRadPlanet2)))
        goto L_a2b2;
    else
        goto L_a442;

L_a442:
    pctCloak = PctCloakFromLpfl(lpfl2);
    if ((pctDetect == 0x64))
        goto L_a46b;
    else
        goto L_a45c;

L_a45c:
    /* untranslated: pctCloak = (words(loword((pctCloak * pctDetect)), signhiword(loword((pctCloak * pctDetect)))) / 0x64) */

L_a46b:
    if ((pctCloak != 0x0))
        goto L_a489;
    else
        goto L_a474;

L_a474:
    MarkFleet(lpfl2, 0x3);
    goto L_a2b2;

L_a489:
    if ((HIWORD(l) >
         HIWORD((int32_t)(((uint32_t)(((int32_t)(((uint32_t)((lRadius2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_a2b2;
    else
        goto L_a4d3;

L_a4d3:
    if ((HIWORD(l) <
         HIWORD((int32_t)(((uint32_t)(((int32_t)(((uint32_t)((lRadius2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_a4e0;
    else
        goto L_a4d8;

L_a4d8:
    if ((LOWORD(l) >
         LOWORD((int32_t)(((uint32_t)(((int32_t)(((uint32_t)((lRadius2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_a2b2;
    else
        goto L_a4e0;

L_a4e0:
    if ((lpfl2->idPlanet == 0xffff))
        goto L_a544;
    else
        goto L_a4ed;

L_a4ed:
    if ((HIWORD(l) > HIWORD((int32_t)((
                         (uint32_t)(((int32_t)(((uint32_t)((lRadPlanet2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_a2b2;
    else
        goto L_a537;

L_a537:
    if ((HIWORD(l) < HIWORD((int32_t)((
                         (uint32_t)(((int32_t)(((uint32_t)((lRadPlanet2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_a544;
    else
        goto L_a53c;

L_a53c:
    if ((LOWORD(l) > LOWORD((int32_t)((
                         (uint32_t)(((int32_t)(((uint32_t)((lRadPlanet2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_a2b2;
    else
        goto L_a544;

L_a544:
    MarkFleet(lpfl2, 0x3);

L_a559:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_a971;

L_a584:
    if ((iPlr == 0xffff))
        goto L_a96d;
    else
        goto L_a58d;

L_a58d:
    if ((lpth->ith == ithMinefield))
        goto L_a5e5;
    else
        goto L_a5a3;

L_a5a3:
    if ((lpth->ith == ithMineralPacket))
        goto L_a5e5;
    else
        goto L_a5b9;

L_a5b9:
    if ((lpth->ith == ithMysteryTrader))
        goto L_a5e5;
    else
        goto L_a5cf;

L_a5cf:
    if ((lpth->ith != ithWormhole))
        goto L_a96d;
    else
        goto L_a5e5;

L_a5e5:
    if ((lpth->ith != ithMinefield))
        goto L_a60d;
    else
        goto L_a5fb;

L_a5fb:
    if (((lpth->thm.grbitPlrNow & grbitPlr) != 0x0))
        goto L_a96d;
    else
        goto L_a60d;

L_a60d:
    if ((lpth->ith != ithMysteryTrader))
        goto L_a63d;
    else
        goto L_a623;

L_a623:
    if ((lpth->tht.fInclude != 0x0))
        goto L_a96d;
    else
        goto L_a63d;

L_a63d:
    if ((lpth->ith != ithMineralPacket))
        goto L_a66a;
    else
        goto L_a653;

L_a653:
    if ((lpth->thp.fInclude != 0x0))
        goto L_a96d;
    else
        goto L_a66a;

L_a66a:
    if ((lpth->ith != ithWormhole))
        goto L_a69a;
    else
        goto L_a680;

L_a680:
    if ((lpth->thw.fInclude != 0x0))
        goto L_a96d;
    else
        goto L_a694;

L_a694:

L_a69a:
    dx = abs((pt.x - lpth->pt.x));
    dy = abs((pt.y - lpth->pt.y));
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(l) < HIWORD(lRadius2)))
        goto L_a72f;
    else
        goto L_a70c;

L_a70c:
    if ((HIWORD(l) > HIWORD(lRadius2)))
        goto L_a719;
    else
        goto L_a711;

L_a711:
    if ((LOWORD(l) <= LOWORD(lRadius2)))
        goto L_a72f;
    else
        goto L_a719;

L_a719:
    if ((lpth->ith != ithMinefield))
        goto L_a96d;
    else
        goto L_a72f;

L_a72f:
    if ((lpth->ith != ithMineralPacket))
        goto L_a817;
    else
        goto L_a745;

L_a745:
    lpth->thp.fInclude = 0x1;

LThIncPlr:
    if ((((rgplr[lpth->iplr].wMdPlr >> 0x8) & 0x1) != 0x0))
        goto L_a96d;
    else
        goto L_a784;

L_a784:
    rgplr[((lpth->idFull >> 0x9) & 0xf)].wMdPlr = ((rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100);
    rgplr[((lpth->idFull >> 0x9) & 0xf)].wMdPlr = ((rgplr[lpth->iplr].wMdPlr & 0xfff8) | 0x3);

L_a817:
    if ((lpth->ith != ithMysteryTrader))
        goto L_a844;
    else
        goto L_a82d;

L_a82d:
    lpth->tht.fInclude = 0x1;
    goto L_a96d;

L_a844:
    if ((lpth->ith != ithWormhole))
        goto L_a8cb;
    else
        goto L_a85a;

L_a85a:
    if (((lpth->thw.grbitPlr & grbitPlr) != 0x0))
        goto L_a8aa;
    else
        goto L_a86c;

L_a86c:
    if ((HIWORD(l) < HIWORD((int32_t)((lRadius2 >> 0x4)))))
        goto L_a8aa;
    else
        goto L_a882;

L_a882:
    if ((HIWORD(l) > HIWORD((int32_t)((lRadius2 >> 0x4)))))
        goto L_a88f;
    else
        goto L_a887;

L_a887:
    if ((LOWORD(l) <= LOWORD((int32_t)((lRadius2 >> 0x4)))))
        goto L_a8aa;
    else
        goto L_a88f;

L_a88f:
    if ((HIWORD(l) > HIWORD(lRadPlanet2)))
        goto L_a96d;
    else
        goto L_a89d;

L_a89d:
    if ((HIWORD(l) < HIWORD(lRadPlanet2)))
        goto L_a8aa;
    else
        goto L_a8a2;

L_a8a2:
    if ((LOWORD(l) > LOWORD(lRadPlanet2)))
        goto L_a96d;
    else
        goto L_a8aa;

L_a8aa:
    lpth->thw.grbitPlr = (lpth->thw.grbitPlr | grbitPlr);
    lpth->thw.fInclude = 0x1;

L_a8cb:
    if (((*(lpth + 0xa) & grbitPlr) == 0x0))
        goto L_a8f8;
    else
        goto L_a8dd;

L_a8dd:
    if ((HIWORD(l) < HIWORD(lRadius2)))
        goto L_a956;
    else
        goto L_a8eb;

L_a8eb:
    if ((HIWORD(l) > HIWORD(lRadius2)))
        goto L_a8f8;
    else
        goto L_a8f0;

L_a8f0:
    if ((LOWORD(l) <= LOWORD(lRadius2)))
        goto L_a956;
    else
        goto L_a8f8;

L_a8f8:
    if ((HIWORD(l) < HIWORD(lRadPlanet2)))
        goto L_a956;
    else
        goto L_a906;

L_a906:
    if ((HIWORD(l) > HIWORD(lRadPlanet2)))
        goto L_a913;
    else
        goto L_a90b;

L_a90b:
    if ((LOWORD(l) <= LOWORD(lRadPlanet2)))
        goto L_a956;
    else
        goto L_a913;

L_a913:
    if ((HIWORD(l) < HIWORD((int32_t)((lRadius2 >> 0x4)))))
        goto L_a956;
    else
        goto L_a929;

L_a929:
    if ((HIWORD(l) > HIWORD((int32_t)((lRadius2 >> 0x4)))))
        goto L_a936;
    else
        goto L_a92e;

L_a92e:
    if ((LOWORD(l) <= LOWORD((int32_t)((lRadius2 >> 0x4)))))
        goto L_a956;
    else
        goto L_a936;

L_a936:
    if ((HIWORD(l) > *(lpth + 0x8)))
        goto L_a96d;
    else
        goto L_a949;

L_a949:
    if ((HIWORD(l) < *(lpth + 0x8)))
        goto L_a956;
    else
        goto L_a94e;

L_a94e:
    if ((LOWORD(l) > *(lpth + 0x6)))
        goto L_a96d;
    else
        goto L_a956;

L_a956:
    *(lpth + 0xa) = (*(lpth + 0xa) | grbitPlr);
    *(lpth + 0xe) = (*(lpth + 0xe) | grbitPlr);
    goto LThIncPlr;

L_a96d:
    lpth = (lpth + 0x1);

L_a971:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_a584;
    else
        goto L_a97f;

L_a97f:
    if (((iSteal & 0x2) == 0x0))
        goto L_a9bf;
    else
        goto L_a98d;

L_a98d:
    if ((lpfl->idPlanet == 0xffff))
        goto L_a9bf;
    else
        goto L_a99a;

L_a99a:
    MarkPlanet(&(lpPlanets[lpfl->idPlanet]), iPlr, 0x4);

L_a9bf:
    if ((iRadPlanet <= 0x0))
        goto L_a12b;
    else
        goto L_a9c8;

L_a9c8:
    iRadius = iRadPlanet;
    lRadius2 = (uint32_t)(((uint32_t)(iRadius) * (uint32_t)(iRadius)));
    pt.x = lpfl->pt.x;
    pt.y = lpfl->pt.y;
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_abc6;

L_aa21:
    if ((lppl->fInclude == 0x0))
        goto L_aa4d;
    else
        goto L_aa38;

L_aa38:
    if ((lppl->det >= 0x3))
        goto L_abc2;
    else
        goto L_aa47;

L_aa47:

L_aa4d:
    abs((rgptPlan[lppl->id].x - pt.x));
    /* untranslated: dx = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_abc2 : L_aa72 */

L_aa72:
    abs((rgptPlan[lppl->id].y - pt.y));
    /* untranslated: dy = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_abc2 : L_aa9b */

L_aa9b:
    d2 = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x44]) > HIWORD(lRadius2) ? L_abc2 : L_aadb */

L_aadb:
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x44]) < HIWORD(lRadius2) ? L_aae8 : L_aae0 */

L_aae0:
    /* untranslated: branch (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x46]) > LOWORD(lRadius2) ? L_abc2 : L_aae8 */

L_aae8:
    if ((lppl->fStarbase == 0x0))
        goto LMark101;
    else
        goto L_aaff;

L_aaff:
    if ((lppl->iPlayer == 0xffff))
        goto LMark101;
    else
        goto L_ab0c;

L_ab0c:
    /* untranslated: LOWORD(lVis2) = part[135:2](rglpshdefSB[lppl->iPlayer][lppl->isb]) */
    /* untranslated: HIWORD(lVis2) = part[137:2](rglpshdefSB[lppl->iPlayer][lppl->isb]) */
    if ((HIWORD(lVis2) > 0x0))
        goto LMark101;
    else
        goto L_ab4b;

L_ab4b:
    if ((HIWORD(lVis2) < 0x0))
        goto L_ab5a;
    else
        goto L_ab50;

L_ab50:
    if ((LOWORD(lVis2) >= 0x2710))
        goto LMark101;
    else
        goto L_ab5a;

L_ab5a:
    if ((HIWORD(d2) < HIWORD((int32_t)(((uint32_t)((lRadius2 * lVis2)) / 0x2710)))))
        goto LMark101;
    else
        goto L_ab82;

L_ab82:
    if ((HIWORD(d2) > HIWORD((int32_t)(((uint32_t)((lRadius2 * lVis2)) / 0x2710)))))
        goto L_ab8f;
    else
        goto L_ab87;

L_ab87:
    if ((LOWORD(d2) <= LOWORD((int32_t)(((uint32_t)((lRadius2 * lVis2)) / 0x2710)))))
        goto LMark101;
    else
        goto L_ab8f;

L_ab8f:
    MarkPlanet(lppl, iPlr, 0x2);
    goto L_abc2;

LMark101:
    MarkPlanet(lppl, iPlr, 0x3);

L_abc2:
    lppl = (lppl + 0x1);

L_abc6:
    if ((LOWORD(lppl) >= LOWORD(lpplMac)))
        goto L_a12b;
    else
        goto L_abd1;

L_abd1:

L_abd7:
    return;
}

void SetVisPFPlanets(int16_t iPlr) {
    int32_t  lRadPlanet2;
    int16_t  iRadPlanet;
    PLANET  *lpplMac;
    POINT    pt;
    int16_t  pctCloak;
    PLANET  *lppl2;
    int16_t  dy;
    FLEET   *lpfl2;
    int32_t  d2;
    PLANET  *lppl;
    int16_t  j;
    THING   *lpth;
    int32_t  lRadius2;
    int16_t  i;
    THING   *lpthMac;
    int16_t  iRadius;
    int16_t  fStargateView;
    int16_t  dx;
    int32_t  l;
    PLANET  *lpplMac2;
    uint16_t grbitPlr;
    int16_t  rgStargateRange[16];
    int32_t  lVis2;
    uint16_t t_merge_abfe_0001;

L_abde:
    if ((iPlr != 0xffff))
        goto L_abf6;
    else
        goto L_abf0;

L_abf0:
    t_merge_abfe_0001 = 0x0;
    goto L_abfe;

L_abf6:
    t_merge_abfe_0001 = (0x1 << iPlr);

L_abfe:
    grbitPlr = t_merge_abfe_0001;
    fStargateView = 0x0;
    if ((GetRaceStat(rgplr[iPlr], rsMajorAdv) != raStargate))
        goto L_aca8;
    else
        goto L_ac26;

L_ac26:
    i = 0x0;
    goto L_ac9f;

L_ac2e:
    rgStargateRange[i] = 0x0;
    if ((((rglpshdefSB[iPlr][i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_ac9b;
    else
        goto L_ac66;

L_ac66:

L_ac6c:
    StargateRangeFromLppl(0x0, iPlr, i);
    /* untranslated: rgStargateRange[i] = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) <= 0x0 ? L_ac9b : L_ac96 */

L_ac96:
    fStargateView = 0x1;

L_ac9b:
    i = (i + 0x1);

L_ac9f:
    if ((i < 0xa))
        goto L_ac2e;
    else
        goto L_aca8;

L_aca8:
    if ((iPlr == 0xffff))
        goto L_acbd;
    else
        goto L_acb1;

L_acb1:
    UpdateProgressGauge(0xfc61);

L_acbd:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_af81;

L_ace8:
    if ((lppl->iPlayer != iPlr))
        goto L_af7d;
    else
        goto L_acf4;

L_acf4:

L_acfa:
    iRadius = GetPlanetScannerRange(lppl, &(iRadPlanet));
    lRadius2 = (uint32_t)(((uint32_t)(iRadius) * (uint32_t)(iRadius)));
    lRadPlanet2 = (uint32_t)(((uint32_t)(iRadPlanet) * (uint32_t)(iRadPlanet)));
    pt.x = rgptPlan[lppl->id].x;
    pt.y = rgptPlan[lppl->id].y;
    j = 0x0;
    goto L_ad61;

L_ad5d:
    j = (j + 0x1);

L_ad61:
    if ((j >= cFleet))
        goto L_af7d;
    else
        goto L_ad6c;

L_ad6c:
    lpfl2 = rglpfl[j];
    if ((LOWORD(rglpfl[j]) != 0x0))
        goto L_ad9c;
    else
        goto L_ad94;

L_ad94:
    if ((*(rglpfl[j] + 0x2) == 0x0))
        goto L_af7d;
    else
        goto L_ad9c;

L_ad9c:
    if ((lpfl2->fInclude != 0x0))
        goto L_ad5d;
    else
        goto L_adb3;

L_adb3:
    if ((lpfl2->fDead != 0x0))
        goto L_ad5d;
    else
        goto L_adc7;

L_adc7:

L_adcd:
    abs((pt.x - lpfl2->pt.x));
    /* untranslated: dx = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_ad5d : L_adeb */

L_adeb:
    abs((pt.y - lpfl2->pt.y));
    /* untranslated: dy = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_ad5d : L_ae09 */

L_ae09:
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x66]) > HIWORD(lRadius2) ? L_ad5d : L_ae49 */

L_ae49:
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x66]) < HIWORD(lRadius2) ? L_ae56 : L_ae4e */

L_ae4e:
    /* untranslated: branch (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x68]) > LOWORD(lRadius2) ? L_ad5d : L_ae56 */

L_ae56:
    if ((lpfl2->idPlanet == 0xffff))
        goto L_ae7e;
    else
        goto L_ae63;

L_ae63:
    if ((HIWORD(l) > HIWORD(lRadPlanet2)))
        goto L_ad5d;
    else
        goto L_ae71;

L_ae71:
    if ((HIWORD(l) < HIWORD(lRadPlanet2)))
        goto L_ae7e;
    else
        goto L_ae76;

L_ae76:
    if ((LOWORD(l) > LOWORD(lRadPlanet2)))
        goto L_ad5d;
    else
        goto L_ae7e;

L_ae7e:
    pctCloak = PctCloakFromLpfl(lpfl2);
    if ((pctCloak != 0x0))
        goto L_aead;
    else
        goto L_ae98;

L_ae98:
    MarkFleet(lpfl2, 0x3);
    goto L_ad5d;

L_aead:
    if ((HIWORD(l) >
         HIWORD((int32_t)(((uint32_t)(((int32_t)(((uint32_t)((lRadius2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_ad5d;
    else
        goto L_aef7;

L_aef7:
    if ((HIWORD(l) <
         HIWORD((int32_t)(((uint32_t)(((int32_t)(((uint32_t)((lRadius2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_af04;
    else
        goto L_aefc;

L_aefc:
    if ((LOWORD(l) >
         LOWORD((int32_t)(((uint32_t)(((int32_t)(((uint32_t)((lRadius2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_ad5d;
    else
        goto L_af04;

L_af04:
    if ((lpfl2->idPlanet == 0xffff))
        goto L_af68;
    else
        goto L_af11;

L_af11:
    if ((HIWORD(l) > HIWORD((int32_t)((
                         (uint32_t)(((int32_t)(((uint32_t)((lRadPlanet2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_ad5d;
    else
        goto L_af5b;

L_af5b:
    if ((HIWORD(l) < HIWORD((int32_t)((
                         (uint32_t)(((int32_t)(((uint32_t)((lRadPlanet2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_af68;
    else
        goto L_af60;

L_af60:
    if ((LOWORD(l) > LOWORD((int32_t)((
                         (uint32_t)(((int32_t)(((uint32_t)((lRadPlanet2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_ad5d;
    else
        goto L_af68;

L_af68:
    MarkFleet(lpfl2, 0x3);

L_af7d:
    lppl = (lppl + 0x1);

L_af81:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_ace8;
    else
        goto L_af8f;

L_af8f:
    if ((iPlr == 0xffff))
        goto L_afa4;
    else
        goto L_af98;

L_af98:
    UpdateProgressGauge(0xfc61);

L_afa4:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_b41f;

L_afcf:
    if ((lppl->iPlayer != iPlr))
        goto L_b41b;
    else
        goto L_afdb;

L_afdb:

L_afe1:
    iRadius = GetPlanetScannerRange(lppl, &(iRadPlanet));
    lRadius2 = (uint32_t)(((uint32_t)(iRadius) * (uint32_t)(iRadius)));
    lRadPlanet2 = (uint32_t)(((uint32_t)(iRadPlanet) * (uint32_t)(iRadPlanet)));
    pt.x = rgptPlan[lppl->id].x;
    pt.y = rgptPlan[lppl->id].y;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_b40d;

L_b067:
    if ((iPlr == 0xffff))
        goto L_b409;
    else
        goto L_b070;

L_b070:
    if ((lpth->ith == ithMinefield))
        goto L_b0c8;
    else
        goto L_b086;

L_b086:
    if ((lpth->ith == ithMineralPacket))
        goto L_b0c8;
    else
        goto L_b09c;

L_b09c:
    if ((lpth->ith == ithMysteryTrader))
        goto L_b0c8;
    else
        goto L_b0b2;

L_b0b2:
    if ((lpth->ith != ithWormhole))
        goto L_b409;
    else
        goto L_b0c8;

L_b0c8:
    if ((lpth->ith != ithMinefield))
        goto L_b0f0;
    else
        goto L_b0de;

L_b0de:
    if (((lpth->thm.grbitPlrNow & grbitPlr) != 0x0))
        goto L_b409;
    else
        goto L_b0f0;

L_b0f0:
    if ((lpth->ith != ithMysteryTrader))
        goto L_b120;
    else
        goto L_b106;

L_b106:
    if ((lpth->tht.fInclude != 0x0))
        goto L_b409;
    else
        goto L_b120;

L_b120:
    if ((lpth->ith != ithMineralPacket))
        goto L_b14d;
    else
        goto L_b136;

L_b136:
    if ((lpth->thp.fInclude != 0x0))
        goto L_b409;
    else
        goto L_b14d;

L_b14d:
    if ((lpth->ith != ithWormhole))
        goto L_b17d;
    else
        goto L_b163;

L_b163:
    if ((lpth->thw.fInclude != 0x0))
        goto L_b409;
    else
        goto L_b177;

L_b177:

L_b17d:
    abs((pt.x - lpth->pt.x));
    /* untranslated: dx = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_b409 : L_b19b */

L_b19b:
    abs((pt.y - lpth->pt.y));
    /* untranslated: dy = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_b409 : L_b1b9 */

L_b1b9:
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x66]) > HIWORD(lRadius2) ? L_b409 : L_b1f9 */

L_b1f9:
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x66]) < HIWORD(lRadius2) ? L_b206 : L_b1fe */

L_b1fe:
    /* untranslated: branch (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x68]) > LOWORD(lRadius2) ? L_b409 : L_b206 */

L_b206:
    if ((lpth->ith != ithMineralPacket))
        goto L_b2ee;
    else
        goto L_b21c;

L_b21c:
    lpth->thp.fInclude = 0x1;

LThIncPlr2:
    if ((((rgplr[lpth->iplr].wMdPlr >> 0x8) & 0x1) != 0x0))
        goto L_b409;
    else
        goto L_b25b;

L_b25b:
    rgplr[((lpth->idFull >> 0x9) & 0xf)].wMdPlr = ((rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100);
    rgplr[((lpth->idFull >> 0x9) & 0xf)].wMdPlr = ((rgplr[lpth->iplr].wMdPlr & 0xfff8) | 0x3);

L_b2ee:
    if ((lpth->ith != ithMysteryTrader))
        goto L_b31b;
    else
        goto L_b304;

L_b304:
    lpth->tht.fInclude = 0x1;
    goto L_b409;

L_b31b:
    if ((lpth->ith != ithWormhole))
        goto L_b3a2;
    else
        goto L_b331;

L_b331:
    if (((lpth->thw.grbitPlr & grbitPlr) != 0x0))
        goto L_b381;
    else
        goto L_b343;

L_b343:
    if ((HIWORD(l) < HIWORD((int32_t)((lRadius2 >> 0x4)))))
        goto L_b381;
    else
        goto L_b359;

L_b359:
    if ((HIWORD(l) > HIWORD((int32_t)((lRadius2 >> 0x4)))))
        goto L_b366;
    else
        goto L_b35e;

L_b35e:
    if ((LOWORD(l) <= LOWORD((int32_t)((lRadius2 >> 0x4)))))
        goto L_b381;
    else
        goto L_b366;

L_b366:
    if ((HIWORD(l) > HIWORD(lRadPlanet2)))
        goto L_b409;
    else
        goto L_b374;

L_b374:
    if ((HIWORD(l) < HIWORD(lRadPlanet2)))
        goto L_b381;
    else
        goto L_b379;

L_b379:
    if ((LOWORD(l) > LOWORD(lRadPlanet2)))
        goto L_b409;
    else
        goto L_b381;

L_b381:
    lpth->thw.grbitPlr = (lpth->thw.grbitPlr | grbitPlr);
    lpth->thw.fInclude = 0x1;

L_b3a2:
    if (((*(lpth + 0xa) & grbitPlr) != 0x0))
        goto L_b3f2;
    else
        goto L_b3b4;

L_b3b4:
    if ((HIWORD(l) < HIWORD(lRadPlanet2)))
        goto L_b3f2;
    else
        goto L_b3c2;

L_b3c2:
    if ((HIWORD(l) > HIWORD(lRadPlanet2)))
        goto L_b3cf;
    else
        goto L_b3c7;

L_b3c7:
    if ((LOWORD(l) <= LOWORD(lRadPlanet2)))
        goto L_b3f2;
    else
        goto L_b3cf;

L_b3cf:
    if ((HIWORD(l) > HIWORD((int32_t)((lRadius2 >> 0x4)))))
        goto L_b409;
    else
        goto L_b3e5;

L_b3e5:
    if ((HIWORD(l) < HIWORD((int32_t)((lRadius2 >> 0x4)))))
        goto L_b3f2;
    else
        goto L_b3ea;

L_b3ea:
    if ((LOWORD(l) > LOWORD((int32_t)((lRadius2 >> 0x4)))))
        goto L_b409;
    else
        goto L_b3f2;

L_b3f2:
    *(lpth + 0xa) = (*(lpth + 0xa) | grbitPlr);
    *(lpth + 0xe) = (*(lpth + 0xe) | grbitPlr);
    goto LThIncPlr2;

L_b409:
    lpth = (lpth + 0x1);

L_b40d:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_b067;
    else
        goto L_b41b;

L_b41b:
    lppl = (lppl + 0x1);

L_b41f:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_afcf;
    else
        goto L_b42d;

L_b42d:
    if ((iPlr == 0xffff))
        goto L_b442;
    else
        goto L_b436;

L_b436:
    UpdateProgressGauge(0xfc61);

L_b442:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_b722;

L_b46d:
    if ((lppl->iPlayer != iPlr))
        goto L_b71e;
    else
        goto L_b479;

L_b479:

L_b47f:
    iRadius = GetPlanetScannerRange(lppl, &(iRadPlanet));
    lRadius2 = (uint32_t)(((uint32_t)(iRadius) * (uint32_t)(iRadius)));
    lRadPlanet2 = (uint32_t)(((uint32_t)(iRadPlanet) * (uint32_t)(iRadPlanet)));
    pt.x = rgptPlan[lppl->id].x;
    pt.y = rgptPlan[lppl->id].y;
    if ((fStargateView == 0x0))
        goto L_b71e;
    else
        goto L_b4e3;

L_b4e3:
    if ((lppl->fStarbase == 0x0))
        goto L_b71e;
    else
        goto L_b4fa;

L_b4fa:
    if ((rgStargateRange[lppl->isb] <= 0x0))
        goto L_b71e;
    else
        goto L_b513;

L_b513:
    iRadius = rgStargateRange[lppl->isb];
    lRadius2 = (uint32_t)(((uint32_t)(iRadius) * (uint32_t)(iRadius)));
    lppl2 = lpPlanets;
    lpplMac2 = &(lpPlanets[cPlanet]);
    goto L_b710;

L_b56b:
    if ((lppl2->fInclude == 0x0))
        goto L_b594;
    else
        goto L_b582;

L_b582:
    if ((lppl2->det >= 0x3))
        goto L_b70c;
    else
        goto L_b594;

L_b594:
    if ((lppl2->fStarbase == 0x0))
        goto L_b70c;
    else
        goto L_b5ab;

L_b5ab:
    if ((StargateRangeFromLppl(lppl2, 0x0, 0x0) == 0x0))
        goto L_b70c;
    else
        goto L_b5c6;

L_b5c6:

L_b5cc:
    if ((iRadius >= 0x2710))
        goto LMarkStargate;
    else
        goto L_b5d3;

L_b5d3:

L_b5d9:
    abs((rgptPlan[lppl2->id].x - pt.x));
    /* untranslated: dx = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_b70c : L_b5fe */

L_b5fe:
    abs((rgptPlan[lppl2->id].y - pt.y));
    /* untranslated: dy = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_b70c : L_b627 */

L_b627:
    d2 = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x66]) > HIWORD(lRadius2) ? L_b70c : L_b667 */

L_b667:
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x66]) < HIWORD(lRadius2) ? L_b674 : L_b66c */

L_b66c:
    /* untranslated: branch (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x68]) > LOWORD(lRadius2) ? L_b70c : L_b674 */

L_b674:
    /* untranslated: LOWORD(lVis2) = part[135:2](rglpshdefSB[lppl2->iPlayer][lppl2->isb]) */
    /* untranslated: HIWORD(lVis2) = part[137:2](rglpshdefSB[lppl2->iPlayer][lppl2->isb]) */
    if ((HIWORD(lVis2) > 0x0))
        goto LMarkStargate;
    else
        goto L_b6b3;

L_b6b3:
    if ((HIWORD(lVis2) < 0x0))
        goto L_b6c2;
    else
        goto L_b6b8;

L_b6b8:
    if ((LOWORD(lVis2) >= 0x2710))
        goto LMarkStargate;
    else
        goto L_b6c2;

L_b6c2:
    if ((HIWORD(d2) > HIWORD((int32_t)(((uint32_t)((lRadius2 * lVis2)) / 0x2710)))))
        goto L_b70c;
    else
        goto L_b6ea;

L_b6ea:
    if ((HIWORD(d2) < HIWORD((int32_t)(((uint32_t)((lRadius2 * lVis2)) / 0x2710)))))
        goto LMarkStargate;
    else
        goto L_b6ef;

L_b6ef:
    if ((LOWORD(d2) > LOWORD((int32_t)(((uint32_t)((lRadius2 * lVis2)) / 0x2710)))))
        goto L_b70c;
    else
        goto LMarkStargate;

LMarkStargate:
    MarkPlanet(lppl2, iPlr, 0x3);

L_b70c:
    lppl2 = (lppl2 + 0x1);

L_b710:
    if ((LOWORD(lppl2) < LOWORD(lpplMac2)))
        goto L_b56b;
    else
        goto L_b71e;

L_b71e:
    lppl = (lppl + 0x1);

L_b722:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_b46d;
    else
        goto L_b730;

L_b730:
    if ((iPlr == 0xffff))
        goto L_b745;
    else
        goto L_b739;

L_b739:
    UpdateProgressGauge(0xfc61);

L_b745:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_b9da;

L_b770:
    if ((lppl->iPlayer != iPlr))
        goto L_b9d6;
    else
        goto L_b77c;

L_b77c:

L_b782:
    iRadius = GetPlanetScannerRange(lppl, &(iRadPlanet));
    lRadius2 = (uint32_t)(((uint32_t)(iRadius) * (uint32_t)(iRadius)));
    lRadPlanet2 = (uint32_t)(((uint32_t)(iRadPlanet) * (uint32_t)(iRadPlanet)));
    pt.x = rgptPlan[lppl->id].x;
    pt.y = rgptPlan[lppl->id].y;
    if ((iRadPlanet <= 0x0))
        goto L_b9d6;
    else
        goto L_b7e6;

L_b7e6:
    iRadius = iRadPlanet;
    lRadius2 = lRadPlanet2;
    lppl2 = lpPlanets;
    lpplMac2 = &(lpPlanets[cPlanet]);
    goto L_b9c8;

L_b823:
    if ((lppl2->fInclude == 0x0))
        goto L_b84f;
    else
        goto L_b83a;

L_b83a:
    if ((lppl2->det >= 0x3))
        goto L_b9c4;
    else
        goto L_b849;

L_b849:

L_b84f:
    abs((rgptPlan[lppl2->id].x - pt.x));
    /* untranslated: dx = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_b9c4 : L_b874 */

L_b874:
    abs((rgptPlan[lppl2->id].y - pt.y));
    /* untranslated: dy = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_b9c4 : L_b89d */

L_b89d:
    d2 = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x66]) > HIWORD(lRadius2) ? L_b9c4 : L_b8dd */

L_b8dd:
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x66]) < HIWORD(lRadius2) ? L_b8ea : L_b8e2 */

L_b8e2:
    /* untranslated: branch (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x68]) > LOWORD(lRadius2) ? L_b9c4 : L_b8ea */

L_b8ea:
    if ((lppl2->fStarbase == 0x0))
        goto LMark102;
    else
        goto L_b901;

L_b901:
    if ((lppl2->iPlayer == 0xffff))
        goto LMark102;
    else
        goto L_b90e;

L_b90e:
    /* untranslated: LOWORD(lVis2) = part[135:2](rglpshdefSB[lppl2->iPlayer][lppl2->isb]) */
    /* untranslated: HIWORD(lVis2) = part[137:2](rglpshdefSB[lppl2->iPlayer][lppl2->isb]) */
    if ((HIWORD(lVis2) > 0x0))
        goto LMark102;
    else
        goto L_b94d;

L_b94d:
    if ((HIWORD(lVis2) < 0x0))
        goto L_b95c;
    else
        goto L_b952;

L_b952:
    if ((LOWORD(lVis2) >= 0x2710))
        goto LMark102;
    else
        goto L_b95c;

L_b95c:
    if ((HIWORD(d2) < HIWORD((int32_t)(((uint32_t)((lRadius2 * lVis2)) / 0x2710)))))
        goto LMark102;
    else
        goto L_b984;

L_b984:
    if ((HIWORD(d2) > HIWORD((int32_t)(((uint32_t)((lRadius2 * lVis2)) / 0x2710)))))
        goto L_b991;
    else
        goto L_b989;

L_b989:
    if ((LOWORD(d2) <= LOWORD((int32_t)(((uint32_t)((lRadius2 * lVis2)) / 0x2710)))))
        goto LMark102;
    else
        goto L_b991;

L_b991:
    MarkPlanet(lppl2, iPlr, 0x2);
    goto L_b9c4;

LMark102:
    MarkPlanet(lppl2, iPlr, 0x3);

L_b9c4:
    lppl2 = (lppl2 + 0x1);

L_b9c8:
    if ((LOWORD(lppl2) < LOWORD(lpplMac2)))
        goto L_b823;
    else
        goto L_b9d6;

L_b9d6:
    lppl = (lppl + 0x1);

L_b9da:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_b770;
    else
        goto L_b9e8;

L_b9e8:
    return;
}

void SetVisPFThings(int16_t iPlr) {
    POINT    pt;
    int16_t  pctCloak;
    int16_t  dy;
    FLEET   *lpfl2;
    int32_t  d2;
    int16_t  j;
    THING   *lpth;
    int32_t  lRadius2;
    THING   *lpthMac;
    int16_t  iRadius;
    int16_t  dx;
    uint16_t grbitPlr;
    PLANET  *lppl2;
    THING   *lpthMac2;
    THING   *lpth2;
    int32_t  l;
    PLANET  *lpplMac2;
    int32_t  lVis2;
    uint16_t t_merge_ba0e_0001;

L_b9ee:
    if ((iPlr != 0xffff))
        goto L_ba06;
    else
        goto L_ba00;

L_ba00:
    t_merge_ba0e_0001 = 0x0;
    goto L_ba0e;

L_ba06:
    t_merge_ba0e_0001 = (0x1 << iPlr);

L_ba0e:
    grbitPlr = t_merge_ba0e_0001;
    if ((GetRaceStat(rgplr[iPlr], rsMajorAdv) != raMassAccel))
        goto L_c1f9;
    else
        goto L_ba31;

L_ba31:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_c1e8;

L_ba5c:
    if ((lpth->ith != ithMineralPacket))
        goto L_c1e4;
    else
        goto L_ba72;

L_ba72:
    if ((lpth->iplr != iPlr))
        goto L_c1e4;
    else
        goto L_ba8a;

L_ba8a:
    if ((lpth->thp.iWarp == 0x0))
        goto L_c1e4;
    else
        goto L_ba9e;

L_ba9e:

L_baa4:
    lpth->thp.fInclude = 0x1;
    iRadius = (lpth->thp.iWarp + 0x4);
    iRadius = LOWORD((iRadius * iRadius));
    lRadius2 = (uint32_t)(((uint32_t)(iRadius) * (uint32_t)(iRadius)));
    pt.x = lpth->pt.x;
    pt.y = lpth->pt.y;
    j = 0x0;
    goto L_bb0a;

L_bb06:
    j = (j + 0x1);

L_bb0a:
    if ((j >= cFleet))
        goto L_bc9a;
    else
        goto L_bb15;

L_bb15:
    lpfl2 = rglpfl[j];
    if ((LOWORD(rglpfl[j]) != 0x0))
        goto L_bb45;
    else
        goto L_bb3d;

L_bb3d:
    if ((*(rglpfl[j] + 0x2) == 0x0))
        goto L_bc9a;
    else
        goto L_bb45;

L_bb45:
    if ((lpfl2->fInclude != 0x0))
        goto L_bb06;
    else
        goto L_bb5c;

L_bb5c:
    if ((lpfl2->fDead != 0x0))
        goto L_bb06;
    else
        goto L_bb70;

L_bb70:

L_bb76:
    abs((pt.x - lpfl2->pt.x));
    /* untranslated: dx = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_bb06 : L_bb94 */

L_bb94:
    abs((pt.y - lpfl2->pt.y));
    /* untranslated: dy = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_bb06 : L_bbb2 */

L_bbb2:
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x3c]) > HIWORD(lRadius2) ? L_bb06 : L_bbf2 */

L_bbf2:
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x3c]) < HIWORD(lRadius2) ? L_bbff : L_bbf7 */

L_bbf7:
    /* untranslated: branch (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x3e]) > LOWORD(lRadius2) ? L_bb06 : L_bbff */

L_bbff:
    pctCloak = PctCloakFromLpfl(lpfl2);
    if ((pctCloak != 0x0))
        goto L_bc2e;
    else
        goto L_bc19;

L_bc19:
    MarkFleet(lpfl2, 0x3);
    goto L_bb06;

L_bc2e:
    if ((HIWORD(l) >
         HIWORD((int32_t)(((uint32_t)(((int32_t)(((uint32_t)((lRadius2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_bb06;
    else
        goto L_bc78;

L_bc78:
    if ((HIWORD(l) <
         HIWORD((int32_t)(((uint32_t)(((int32_t)(((uint32_t)((lRadius2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_bc85;
    else
        goto L_bc7d;

L_bc7d:
    if ((LOWORD(l) >
         LOWORD((int32_t)(((uint32_t)(((int32_t)(((uint32_t)((lRadius2 * (uint32_t)((0x64 - pctCloak)))) / 0x64)) * (uint32_t)((0x64 - pctCloak)))) / 0x64)))))
        goto L_bb06;
    else
        goto L_bc85;

L_bc85:
    MarkFleet(lpfl2, 0x3);

L_bc9a:
    lpth2 = lpThings;
    lpthMac2 = &(lpThings[cThing]);
    goto L_bff8;

L_bcc5:
    if ((iPlr == 0xffff))
        goto L_bff4;
    else
        goto L_bcce;

L_bcce:
    if ((lpth2->ith == ithMinefield))
        goto L_bd26;
    else
        goto L_bce4;

L_bce4:
    if ((lpth2->ith == ithMineralPacket))
        goto L_bd26;
    else
        goto L_bcfa;

L_bcfa:
    if ((lpth2->ith == ithMysteryTrader))
        goto L_bd26;
    else
        goto L_bd10;

L_bd10:
    if ((lpth2->ith != ithWormhole))
        goto L_bff4;
    else
        goto L_bd26;

L_bd26:
    if ((lpth2->ith != ithMinefield))
        goto L_bd4e;
    else
        goto L_bd3c;

L_bd3c:
    if (((lpth2->thm.grbitPlrNow & grbitPlr) != 0x0))
        goto L_bff4;
    else
        goto L_bd4e;

L_bd4e:
    if ((lpth2->ith != ithMysteryTrader))
        goto L_bd7e;
    else
        goto L_bd64;

L_bd64:
    if ((lpth2->tht.fInclude != 0x0))
        goto L_bff4;
    else
        goto L_bd7e;

L_bd7e:
    if ((lpth2->ith != ithMineralPacket))
        goto L_bdab;
    else
        goto L_bd94;

L_bd94:
    if ((lpth2->thp.fInclude != 0x0))
        goto L_bff4;
    else
        goto L_bdab;

L_bdab:
    if ((lpth2->ith != ithWormhole))
        goto L_bddb;
    else
        goto L_bdc1;

L_bdc1:
    if ((lpth2->thw.fInclude != 0x0))
        goto L_bff4;
    else
        goto L_bdd5;

L_bdd5:

L_bddb:
    abs((pt.x - lpth2->pt.x));
    /* untranslated: dx = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_bff4 : L_bdf9 */

L_bdf9:
    abs((pt.y - lpth2->pt.y));
    /* untranslated: dy = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_bff4 : L_be17 */

L_be17:
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x3c]) > HIWORD(lRadius2) ? L_bff4 : L_be57 */

L_be57:
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x3c]) < HIWORD(lRadius2) ? L_be64 : L_be5c */

L_be5c:
    /* untranslated: branch (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x3e]) > LOWORD(lRadius2) ? L_bff4 : L_be64 */

L_be64:
    if ((lpth2->ith != ithMineralPacket))
        goto L_bf4c;
    else
        goto L_be7a;

L_be7a:
    lpth2->thp.fInclude = 0x1;

LThIncPlr3:
    if ((((rgplr[lpth2->iplr].wMdPlr >> 0x8) & 0x1) != 0x0))
        goto L_bff4;
    else
        goto L_beb9;

L_beb9:
    rgplr[((lpth2->idFull >> 0x9) & 0xf)].wMdPlr = ((rgplr[lpth2->iplr].wMdPlr & 0xfeff) | 0x100);
    rgplr[((lpth2->idFull >> 0x9) & 0xf)].wMdPlr = ((rgplr[lpth2->iplr].wMdPlr & 0xfff8) | 0x3);

L_bf4c:
    if ((lpth2->ith != ithMysteryTrader))
        goto L_bf79;
    else
        goto L_bf62;

L_bf62:
    lpth2->tht.fInclude = 0x1;
    goto L_bff4;

L_bf79:
    if ((lpth2->ith != ithWormhole))
        goto L_bfdd;
    else
        goto L_bf8f;

L_bf8f:
    if (((lpth2->thw.grbitPlr & grbitPlr) != 0x0))
        goto L_bfbc;
    else
        goto L_bfa1;

L_bfa1:
    if ((HIWORD(l) > HIWORD(lRadius2)))
        goto L_bff4;
    else
        goto L_bfaf;

L_bfaf:
    if ((HIWORD(l) < HIWORD(lRadius2)))
        goto L_bfbc;
    else
        goto L_bfb4;

L_bfb4:
    if ((LOWORD(l) > LOWORD(lRadius2)))
        goto L_bff4;
    else
        goto L_bfbc;

L_bfbc:
    lpth2->thw.grbitPlr = (lpth2->thw.grbitPlr | grbitPlr);
    lpth2->thw.fInclude = 0x1;

L_bfdd:
    *(lpth2 + 0xa) = (*(lpth2 + 0xa) | grbitPlr);
    *(lpth2 + 0xe) = (*(lpth2 + 0xe) | grbitPlr);
    goto LThIncPlr3;

L_bff4:
    lpth2 = (lpth2 + 0x1);

L_bff8:
    if ((LOWORD(lpth2) < LOWORD(lpthMac2)))
        goto L_bcc5;
    else
        goto L_c006;

L_c006:
    lppl2 = lpPlanets;
    lpplMac2 = &(lpPlanets[cPlanet]);
    goto L_c1d6;

L_c031:
    if ((lppl2->fInclude == 0x0))
        goto L_c05d;
    else
        goto L_c048;

L_c048:
    if ((lppl2->det >= 0x3))
        goto L_c1d2;
    else
        goto L_c057;

L_c057:

L_c05d:
    abs((rgptPlan[lppl2->id].x - pt.x));
    /* untranslated: dx = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_c1d2 : L_c082 */

L_c082:
    abs((rgptPlan[lppl2->id].y - pt.y));
    /* untranslated: dy = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) > iRadius ? L_c1d2 : L_c0ab */

L_c0ab:
    d2 = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x3c]) > HIWORD(lRadius2) ? L_c1d2 : L_c0eb */

L_c0eb:
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x3c]) < HIWORD(lRadius2) ? L_c0f8 : L_c0f0 */

L_c0f0:
    /* untranslated: branch (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x3e]) > LOWORD(lRadius2) ? L_c1d2 : L_c0f8 */

L_c0f8:
    if ((lppl2->fStarbase == 0x0))
        goto LMark103;
    else
        goto L_c10f;

L_c10f:
    if ((lppl2->iPlayer == 0xffff))
        goto LMark103;
    else
        goto L_c11c;

L_c11c:
    /* untranslated: LOWORD(lVis2) = part[135:2](rglpshdefSB[lppl2->iPlayer][lppl2->isb]) */
    /* untranslated: HIWORD(lVis2) = part[137:2](rglpshdefSB[lppl2->iPlayer][lppl2->isb]) */
    if ((HIWORD(lVis2) > 0x0))
        goto LMark103;
    else
        goto L_c15b;

L_c15b:
    if ((HIWORD(lVis2) < 0x0))
        goto L_c16a;
    else
        goto L_c160;

L_c160:
    if ((LOWORD(lVis2) >= 0x2710))
        goto LMark103;
    else
        goto L_c16a;

L_c16a:
    if ((HIWORD(d2) < HIWORD((int32_t)(((uint32_t)((lRadius2 * lVis2)) / 0x2710)))))
        goto LMark103;
    else
        goto L_c192;

L_c192:
    if ((HIWORD(d2) > HIWORD((int32_t)(((uint32_t)((lRadius2 * lVis2)) / 0x2710)))))
        goto L_c19f;
    else
        goto L_c197;

L_c197:
    if ((LOWORD(d2) <= LOWORD((int32_t)(((uint32_t)((lRadius2 * lVis2)) / 0x2710)))))
        goto LMark103;
    else
        goto L_c19f;

L_c19f:
    MarkPlanet(lppl2, iPlr, 0x2);
    goto L_c1d2;

LMark103:
    MarkPlanet(lppl2, iPlr, 0x3);

L_c1d2:
    lppl2 = (lppl2 + 0x1);

L_c1d6:
    if ((LOWORD(lppl2) < LOWORD(lpplMac2)))
        goto L_c031;
    else
        goto L_c1e4;

L_c1e4:
    lpth = (lpth + 0x1);

L_c1e8:
    if ((LOWORD(lpth) >= LOWORD(lpthMac)))
        goto L_c416;
    else
        goto L_c1f3;

L_c1f3:

L_c1f9:
    if ((GetRaceStat(rgplr[iPlr], rsMajorAdv) != raMines))
        goto L_c416;
    else
        goto L_c219;

L_c219:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_c408;

L_c244:
    if ((lpth->ith != ithMinefield))
        goto L_c404;
    else
        goto L_c25a;

L_c25a:
    if ((lpth->iplr != iPlr))
        goto L_c404;
    else
        goto L_c26f;

L_c26f:

L_c275:
    LOWORD(lRadius2) = LOWORD(lpth->thm);
    HIWORD(lRadius2) = HIWORD(lpth->thm.cMines);
    pt.x = lpth->pt.x;
    pt.y = lpth->pt.y;
    j = 0x0;
    goto L_c2a3;

L_c29f:
    j = (j + 0x1);

L_c2a3:
    if ((j >= cFleet))
        goto L_c404;
    else
        goto L_c2ae;

L_c2ae:
    lpfl2 = rglpfl[j];
    if ((LOWORD(rglpfl[j]) != 0x0))
        goto L_c2de;
    else
        goto L_c2d6;

L_c2d6:
    if ((*(rglpfl[j] + 0x2) == 0x0))
        goto L_c404;
    else
        goto L_c2de;

L_c2de:
    if ((lpfl2->fInclude != 0x0))
        goto L_c29f;
    else
        goto L_c2f5;

L_c2f5:
    if ((lpfl2->fDead != 0x0))
        goto L_c29f;
    else
        goto L_c30c;

L_c30c:
    if ((lpfl2->idPlanet != 0xffff))
        goto L_c29f;
    else
        goto L_c316;

L_c316:

L_c31c:
    abs((pt.x - lpfl2->pt.x));
    /* untranslated: dx = callresult(int16_t) */
    /* untranslated: branch signhiword(callresult(int16_t)) > HIWORD(lRadius2) ? L_c29f : L_c33b */

L_c33b:
    /* untranslated: branch signhiword(callresult(int16_t)) < HIWORD(lRadius2) ? L_c348 : L_c340 */

L_c340:
    /* untranslated: branch callresult(int16_t) > LOWORD(lRadius2) ? L_c29f : L_c348 */

L_c348:
    abs((pt.y - lpfl2->pt.y));
    /* untranslated: dy = callresult(int16_t) */
    /* untranslated: branch signhiword(callresult(int16_t)) > HIWORD(lRadius2) ? L_c29f : L_c367 */

L_c367:
    /* untranslated: branch signhiword(callresult(int16_t)) < HIWORD(lRadius2) ? L_c374 : L_c36c */

L_c36c:
    /* untranslated: branch callresult(int16_t) > LOWORD(lRadius2) ? L_c29f : L_c374 */

L_c374:
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x2c]) > HIWORD(lRadius2) ? L_c29f : L_c3b4 */

L_c3b4:
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x2c]) < HIWORD(lRadius2) ? L_c3c1 : L_c3b9 */

L_c3b9:
    /* untranslated: branch (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x2e]) > LOWORD(lRadius2) ? L_c29f : L_c3c1 */

L_c3c1:
    pctCloak = PctCloakFromLpfl(lpfl2);
    if ((pctCloak == 0x0))
        goto L_c3ef;
    else
        goto L_c3db;

L_c3db:
    if ((Random(0x64) < pctCloak))
        goto L_c29f;
    else
        goto L_c3ef;

L_c3ef:
    MarkFleet(lpfl2, 0x3);

L_c404:
    lpth = (lpth + 0x1);

L_c408:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_c244;
    else
        goto L_c416;

L_c416:
    return;
}

void SetVisPFFinish(int16_t iPlr) {
    int16_t  detMajor;
    int16_t  j;
    int16_t  i;
    uint16_t t_merge_c44e_0001;

L_c41c:
    if ((GetRaceStat(rgplr[iPlr], rsMajorAdv) != raAttack))
        goto L_c44b;
    else
        goto L_c445;

L_c445:
    t_merge_c44e_0001 = 0x7;
    goto L_c44e;

L_c44b:
    t_merge_c44e_0001 = 0x3;

L_c44e:
    detMajor = t_merge_c44e_0001;
    i = 0x0;
    goto L_c45d;

L_c459:
    i = (i + 0x1);

L_c45d:
    if ((i >= game.cPlayer))
        goto L_c84c;
    else
        goto L_c468;

L_c468:
    if ((i == iPlr))
        goto L_c459;
    else
        goto L_c473;

L_c473:
    rgplr[i].cShDef = 0x0;
    j = 0x0;
    goto L_c631;

L_c48a:
    if ((((0x1 << iPlr) & rglpshdef[i][j].grbitPlr) == 0x0))
        goto L_c550;
    else
        goto L_c4be;

L_c4be:
    rglpshdef[i][j].wFlags = ((rglpshdef[i][j].wFlags & 0xfeff) | 0x100);
    rglpshdef[i][j].wFlags = ((rglpshdef[i][j].wFlags & 0xff00) | 0x7);
    goto LFinShdef;

L_c550:
    if ((((rglpshdef[i][j].wFlags >> 0x8) & 0x1) == 0x0))
        goto L_c62d;
    else
        goto L_c57d;

L_c57d:
    rglpshdef[i][j].wFlags = ((rglpshdef[i][j].wFlags & 0xff00) | (detMajor & 0xff));

LFinShdef:
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfff8) | 0x3);
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfeff) | 0x100);
    rgplr[i].cShDef = (rgplr[i].cShDef + 0x1);

L_c62d:
    j = (j + 0x1);

L_c631:
    if ((j < 0x10))
        goto L_c48a;
    else
        goto L_c63a;

L_c63a:
    /* untranslated: part[4:2](rgplr[i]) = ((part[4:2](rgplr[i]) & 0xfff) | 0x0) */
    j = 0x0;
    goto L_c840;

L_c66a:
    if ((((0x1 << iPlr) & rglpshdefSB[i][j].grbitPlr) == 0x0))
        goto L_c730;
    else
        goto L_c69e;

L_c69e:
    rglpshdefSB[i][j].wFlags = ((rglpshdefSB[i][j].wFlags & 0xfeff) | 0x100);
    rglpshdefSB[i][j].wFlags = ((rglpshdefSB[i][j].wFlags & 0xff00) | 0x7);
    goto LFinShdefSB;

L_c730:
    if ((((rglpshdefSB[i][j].wFlags >> 0x8) & 0x1) == 0x0))
        goto L_c83c;
    else
        goto L_c75d;

L_c75d:
    rglpshdefSB[i][j].wFlags = ((rglpshdefSB[i][j].wFlags & 0xff00) | (detMajor & 0xff));

LFinShdefSB:
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfeff) | 0x100);
    rgplr[i].wMdPlr = ((rgplr[i].wMdPlr & 0xfff8) | 0x3);
    /* untranslated: ss:[bp-0xa] = ((part[4:2](rgplr[i]) + 0x1000) & 0xf000) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) & 0xfff) */
    /* untranslated: part[4:2](rgplr[i]) = (part[4:2](rgplr[i]) | ss:[bp-0xa]) */

L_c83c:
    j = (j + 0x1);

L_c840:
    if ((j >= 0xa))
        goto L_c459;
    else
        goto L_c846;

L_c846:

L_c84c:
    return;
}
