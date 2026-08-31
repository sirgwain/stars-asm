#include "common.h"

void SatisfyOrders(int16_t iPass) {
    int16_t        fMining;
    int32_t        amountWP;
    XferActionType action;
    PLANET         pl;
    int32_t        l2;
    int16_t        j;
    int32_t        amount;
    int16_t        iflWP;
    int16_t        fSentBadFleetXfer;
    int16_t        ifltcur;
    FLEET         *lpfl;
    int16_t        fAtPlanet;
    MessageId      idm;
    int16_t        iLoad;
    uint16_t       xWP;
    int16_t        fOptFuel;
    int16_t        fStealing;
    FLEET         *lpflWP;
    int16_t        fHasPermission;
    int16_t        fFulfilled;
    ORDER          ord;
    int16_t        fFueling;
    int32_t        wtOptimalFuel;
    int16_t        fDunnage;
    int32_t        amountEdit;
    int32_t        l;
    int16_t        fDone;
    uint16_t       idWP;
    THING         *lpthWP;
    int32_t        cFuel2;
    int32_t        iExcess;
    int32_t        lMaxFuel;
    int32_t        wtFuelOrig;
    int32_t        lT;
    uint16_t       iGoto;
    SHDEF         *lpshdefT;
    int32_t        lXferMinerals;
    int16_t        i;
    int32_t        lAmt;
    SHDEF          shdefT;
    int16_t        csh;
    int16_t        fBleeding;
    int32_t        lResUltimate;
    int16_t        fUltimate;
    int16_t        fColonize;
    int32_t        rgwt[3];
    int32_t        cMine;
    PLANET        *lppl;
    int32_t        rglQuan[4];
    FLEET         *lpflDest;
    int16_t        ishLastFree;
    int16_t        rgishMap[16];
    int16_t        ishMatch;
    int16_t        ish;
    FLEET         *lpflNew;
    int16_t        iplrDest;
    SHDEF         *lpshdefDest;
    THING         *lpthMac;
    int32_t        dy;
    THING         *lpthBest;
    THING         *lpth;
    int32_t        lBest;
    int32_t        dx;
    uint16_t       t_merge_68fa_0001;
    uint16_t       t_merge_694a_0001;
    uint16_t       t_merge_6a79_0001;
    int32_t        t_merge_6e41_0001_wide;
    uint32_t       t_merge_6e71_0001_wide;
    int32_t        t_merge_70c3_0001_wide;
    uint16_t       t_merge_7165_0001;
    int32_t        t_merge_72c7_0001_wide;
    int32_t        t_merge_73e3_0001_wide;
    uint16_t       t_merge_7547_0001;
    uint16_t       t_merge_76b8_0001;
    int32_t        t_merge_77be_0001_wide;
    uint16_t       t_merge_7a20_0001;
    uint16_t       t_merge_7aca_0001;
    uint16_t       t_merge_8513_0001;
    uint16_t       t_merge_915a_0001;

L_6798:
    if ((cFleet <= 0x0))
        goto L_9eb2;
    else
        goto L_67a8;

L_67a8:

L_67ae:
    ifltcur = 0x0;
    goto L_67ba;

L_67b6:
    ifltcur = (ifltcur + 0x1);

L_67ba:
    if ((ifltcur >= cFleet))
        goto L_9eb2;
    else
        goto L_67c5;

L_67c5:
    lpfl = rglpfl[ifltcur];
    if ((LOWORD(rglpfl[ifltcur]) != 0x0))
        goto L_67f5;
    else
        goto L_67ed;

L_67ed:
    if ((*(rglpfl[ifltcur] + 0x2) == 0x0))
        goto L_9eb2;
    else
        goto L_67f5;

L_67f5:
    if ((iPass != 0x1))
        goto L_6826;
    else
        goto L_67fe;

L_67fe:
    lpfl->fCompChg = 0x0;
    lpfl->fTargeted = 0x0;

L_6826:
    if ((lpfl->fDead != 0x0))
        goto L_67b6;
    else
        goto L_683d;

L_683d:
    if ((lpfl->fSkipped != 0x0))
        goto L_67b6;
    else
        goto L_6851;

L_6851:

L_6857:
    ord = lpfl->lpplord->rgord[0x0];
    if ((ord.grTask != grTaskXfer))
        goto L_8155;
    else
        goto L_6888;

L_6888:
    fFulfilled = 0x1;
    fDone = 0x1;
    fSentBadFleetXfer = 0x0;
    fHasPermission = 0x1;
    fMining = 0x0;
    fFueling = 0x0;
    fStealing = 0x0;
    fDunnage = 0x0;
    lpflWP = 0x0;
    lpthWP = 0x0;
    if ((lpfl->idPlanet == 0xffff))
        goto L_68f7;
    else
        goto L_68d6;

L_68d6:
    if ((FLookupPlanet(lpfl->idPlanet, &(pl)) == 0x0))
        goto L_68f7;
    else
        goto L_68f1;

L_68f1:
    t_merge_68fa_0001 = 0x1;
    goto L_68fa;

L_68f7:
    t_merge_68fa_0001 = 0x0;

L_68fa:
    fAtPlanet = t_merge_68fa_0001;
    idWP = ord.id;
    goto L_6bf1;

L_6912:
    lpflWP = LpflFromId(ord.id);
    idWP = (idWP | 0x8000);
    xWP = 0xffff;
    if ((lpfl->iPlayer != lpflWP->iPlayer))
        goto L_6947;
    else
        goto L_6941;

L_6941:
    t_merge_694a_0001 = 0x1;
    goto L_694a;

L_6947:
    t_merge_694a_0001 = 0x0;

L_694a:
    fHasPermission = t_merge_694a_0001;
    if ((fHasPermission != 0x0))
        goto L_6981;
    else
        goto L_6956;

L_6956:
    GetFleetScannerRange(lpfl, 0x0, 0x0, &(fHasPermission));
    if ((fHasPermission == 0x0))
        goto L_6c14;
    else
        goto L_6979;

L_6979:
    fStealing = 0x1;

L_6981:
    if ((lpflWP->fHereAllTurn == 0x0))
        goto L_6c14;
    else
        goto L_6998;

L_6998:
    if ((fAtPlanet == 0x0))
        goto L_6c14;
    else
        goto L_69a1;

L_69a1:
    if ((HIWORD(CMineFromLpfl(lpflWP)) < 0x0))
        goto L_69f6;
    else
        goto L_69b7;

L_69b7:
    /* untranslated: branch hiword(callresult(int32_t)) > 0x0 ? L_69c4 : L_69bc */

L_69bc:
    /* untranslated: branch loword(callresult(int32_t)) <= 0x0 ? L_69f6 : L_69c4 */

L_69c4:
    if ((pl.iPlayer == 0xffff))
        goto L_69dc;
    else
        goto L_69cd;

L_69cd:
    if ((pl.iPlayer != lpfl->iPlayer))
        goto L_69f6;
    else
        goto L_69dc;

L_69dc:
    fMining = 0x1;
    xWP = 0xffff;
    idWP = (lpflWP->id | 0x8000);
    goto L_6c14;

L_69f6:
    if ((LOWORD(LGetFleetStat(lpflWP, 0x2)) != 0x0))
        goto L_6c14;
    else
        goto L_6a10;

L_6a10:
    /* untranslated: branch hiword(callresult(int32_t)) != 0x0 ? L_6c14 : L_6a18 */

L_6a18:
    if ((pl.iPlayer != lpfl->iPlayer))
        goto L_6c14;
    else
        goto L_6a27;

L_6a27:
    fFueling = 0x1;
    xWP = 0xffff;
    idWP = (lpflWP->id | 0x8000);

L_6a41:
    lpthWP = LpthFromId(ord.id);
    xWP = 0xfffe;
    goto L_6c14;

L_6a5c:
    xWP = 0xffff;
    if ((lpfl->iPlayer != pl.iPlayer))
        goto L_6a76;
    else
        goto L_6a70;

L_6a70:
    t_merge_6a79_0001 = 0x3;
    goto L_6a79;

L_6a76:
    t_merge_6a79_0001 = 0x0;

L_6a79:
    fHasPermission = t_merge_6a79_0001;
    if ((fHasPermission != 0x0))
        goto L_6c14;
    else
        goto L_6a85;

L_6a85:
    GetFleetScannerRange(lpfl, 0x0, 0x0, &(fHasPermission));
    if ((fHasPermission != 0x1))
        goto L_6aad;
    else
        goto L_6aa8;

L_6aa8:
    fHasPermission = 0x0;

L_6aad:
    if ((fHasPermission == 0x0))
        goto L_6abb;
    else
        goto L_6ab6;

L_6ab6:
    fStealing = 0x1;

L_6abb:
    if ((fHasPermission != 0x0))
        goto L_6c14;
    else
        goto L_6ac4;

L_6ac4:
    if ((pl.iPlayer != 0xffff))
        goto L_6c14;
    else
        goto L_6acd;

L_6acd:
    iflWP = 0x0;
    goto L_6ad9;

L_6ad5:
    iflWP = (iflWP + 0x1);

L_6ad9:
    if ((iflWP >= cFleet))
        goto L_6c14;
    else
        goto L_6ae4;

L_6ae4:
    lpflWP = rglpfl[iflWP];
    if ((LOWORD(rglpfl[iflWP]) != 0x0))
        goto L_6b14;
    else
        goto L_6b0c;

L_6b0c:
    if ((*(rglpfl[iflWP] + 0x2) == 0x0))
        goto L_6c14;
    else
        goto L_6b14;

L_6b14:
    if ((lpflWP->fDead != 0x0))
        goto L_6ad5;
    else
        goto L_6b2b;

L_6b2b:
    if ((lpflWP->iPlayer != lpfl->iPlayer))
        goto L_6ad5;
    else
        goto L_6b3e;

L_6b3e:
    if ((lpfl->idPlanet != lpflWP->idPlanet))
        goto L_6ad5;
    else
        goto L_6b51;

L_6b51:
    if ((LOWORD(lpflWP) != LOWORD(lpfl)))
        goto L_6b6a;
    else
        goto L_6b5f;

L_6b5f:
    if ((HIWORD(lpflWP) == HIWORD(lpfl)))
        goto L_6ad5;
    else
        goto L_6b64;

L_6b64:

L_6b6a:
    if ((lpflWP->fHereAllTurn == 0x0))
        goto L_6ad5;
    else
        goto L_6b81;

L_6b81:
    if ((HIWORD(CMineFromLpfl(lpflWP)) < 0x0))
        goto L_6ad5;
    else
        goto L_6b97;

L_6b97:
    /* untranslated: branch hiword(callresult(int32_t)) > 0x0 ? L_6ba4 : L_6b9c */

L_6b9c:
    /* untranslated: branch loword(callresult(int32_t)) <= 0x0 ? L_6ad5 : L_6ba4 */

L_6ba4:
    fMining = 0x2;
    fHasPermission = 0x1;
    ord.grobj = grobjFleet;
    ord.id = lpflWP->id;
    xWP = 0xffff;
    idWP = (lpflWP->id | 0x8000);
    goto L_6c14;

L_6bde:
    xWP = ord.pt.x;
    idWP = ord.pt.y;
    goto L_6c14;

L_6bf1:
    if ((ord.grobj == grobjPlanet))
        goto L_6a5c;
    else
        goto L_6bf9;

L_6bf9:
    if ((ord.grobj == grobjFleet))
        goto L_6912;
    else
        goto L_6c01;

L_6c01:
    if ((ord.grobj == grobjOther))
        goto L_6bde;
    else
        goto L_6c09;

L_6c09:
    if ((ord.grobj == grobjThing))
        goto L_6a41;
    else
        goto L_6c14;

L_6c14:
    iLoad = ((iPass + 0xffff) & 0x1);
    fOptFuel = 0x0;
    if ((ord.grobj != grobjThing))
        goto LTryDunnage;
    else
        goto L_6c38;

L_6c38:
    if ((LOWORD(lpthWP) != 0x0))
        goto L_6c4c;
    else
        goto L_6c42;

L_6c42:
    if ((HIWORD(lpthWP) == 0x0))
        goto L_6c63;
    else
        goto L_6c4c;

L_6c4c:
    if ((lpthWP->ith == ithMineralPacket))
        goto LTryDunnage;
    else
        goto L_6c63;

L_6c63:
    if ((LOWORD(lpthWP) != 0x0))
        goto L_6c77;
    else
        goto L_6c6d;

L_6c6d:
    if ((HIWORD(lpthWP) == 0x0))
        goto CancelOrder;
    else
        goto L_6c77;

L_6c77:
    FSendPlrMsg2(lpfl->iPlayer, 0x11e, (lpfl->id | 0x8000), lpfl->id, lpthWP->ith);

LTryDunnage:
    j = 0x0;
    goto L_7b78;

L_6cb5:
    /* untranslated: action = ((ss:[bp+(j * 0x2)-0x74] >> 0xc) & 0xf) */
    if ((action == iActionNone))
        goto L_7b74;
    else
        goto L_6cd5;

L_6cd5:
    if ((fDunnage != 0x2))
        goto L_6ceb;
    else
        goto L_6cdf;

L_6cdf:
    if ((action != iActionLoadDunnage))
        goto L_7b74;
    else
        goto L_6ce5;

L_6ce5:

L_6ceb:
    if ((j != 0x4))
        goto L_6d1a;
    else
        goto L_6cf4;

L_6cf4:
    if ((ord.grobj == grobjFleet))
        goto L_6d1a;
    else
        goto L_6d07;

L_6d07:
    if ((ord.grobj != grobjOther))
        goto L_7b74;
    else
        goto L_6d1a;

L_6d1a:
    if ((j < 0x3))
        goto L_6d39;
    else
        goto L_6d23;

L_6d23:
    if ((ord.grobj == grobjThing))
        goto L_7b74;
    else
        goto L_6d33;

L_6d33:

L_6d39:
    amountWP = 0x0;
    goto L_6de4;

L_6d51:
    if ((fMining != 0x0))
        goto L_6d63;
    else
        goto L_6d5a;

L_6d5a:
    if ((fFueling == 0x0))
        goto L_6d6c;
    else
        goto L_6d63;

L_6d63:
    if ((j != 0x4))
        goto L_6d92;
    else
        goto L_6d6c;

L_6d6c:
    amountWP = lpflWP->rgwtMin[j];
    goto L_6dff;

L_6d92:
    if ((j >= 0x4))
        goto L_6dff;
    else
        goto L_6d9b;

L_6d9b:
    amountWP = pl.rgwtMin[j];

L_6db5:
    if ((j >= 0x3))
        goto L_6dff;
    else
        goto L_6dbe;

L_6dbe:
    /* untranslated: LOWORD(amountWP) = HIWORD(lpthWP):[((LOWORD(lpthWP) + 0x8) + (j * 0x2))] */
    /* untranslated: HIWORD(amountWP) = signhiword(HIWORD(lpthWP):[((LOWORD(lpthWP) + 0x8) + (j * 0x2))]) */

L_6de4:
    if ((ord.grobj == grobjPlanet))
        goto L_6d92;
    else
        goto L_6dec;

L_6dec:
    if ((ord.grobj == grobjFleet))
        goto L_6d51;
    else
        goto L_6df4;

L_6df4:
    if ((ord.grobj == grobjThing))
        goto L_6db5;
    else
        goto L_6dff;

L_6dff:
    goto L_6ff2;
    if ((iLoad == 0x0))
        goto L_7b74;
    else
        goto L_6e0b;

L_6e0b:

L_6e11:
    if ((j != 0x4))
        goto L_6e2f;
    else
        goto L_6e1a;

L_6e1a:
    t_merge_6e41_0001_wide = LGetFleetStat(lpfl, 0x1);
    goto L_6e41;

L_6e2f:
    t_merge_6e41_0001_wide = LGetFleetStat(lpfl, 0x2);

L_6e41:
    amount = t_merge_6e41_0001_wide;
    if ((0x1e > HIWORD(amount)))
        goto L_6e6b;
    else
        goto L_6e55;

L_6e55:
    if ((0x1e < HIWORD(amount)))
        goto L_6e62;
    else
        goto L_6e5a;

L_6e5a:
    if ((0x8480 >= LOWORD(amount)))
        goto L_6e6b;
    else
        goto L_6e62;

L_6e62:
    t_merge_6e71_0001_wide = 0x1e8480;
    goto L_6e71;

L_6e6b:
    t_merge_6e71_0001_wide = amount;

L_6e71:
    amount = t_merge_6e71_0001_wide;
    if ((HIWORD(amount) > 0x1))
        goto L_6ec9;
    else
        goto L_6e80;

L_6e80:
    if ((HIWORD(amount) < 0x1))
        goto L_6e8e;
    else
        goto L_6e85;

L_6e85:
    if ((LOWORD(amount) >= 0x0))
        goto L_6ec9;
    else
        goto L_6e8e;

L_6e8e:
    /* untranslated: LOWORD(amountEdit) = loword((uint32_t)(words(hiword((uint32_t)(amount * words(0x0, (ss:[bp+(j * 0x2)-0x74] & 0xfff)))),
     * loword((uint32_t)(amount * words(0x0, (ss:[bp+(j * 0x2)-0x74] & 0xfff))))) / 0x64)) */
    /* untranslated: HIWORD(amountEdit) = hiword((uint32_t)(words(hiword((uint32_t)(amount * words(0x0, (ss:[bp+(j * 0x2)-0x74] & 0xfff)))),
     * loword((uint32_t)(amount * words(0x0, (ss:[bp+(j * 0x2)-0x74] & 0xfff))))) / 0x64)) */
    goto L_6eff;

L_6ec9:
    /* untranslated: LOWORD(amountEdit) = loword((uint32_t)((int32_t)(amount / 0x64) * words(0x0, (ss:[bp+(j * 0x2)-0x74] & 0xfff)))) */
    /* untranslated: HIWORD(amountEdit) = hiword((uint32_t)((int32_t)(amount / 0x64) * words(0x0, (ss:[bp+(j * 0x2)-0x74] & 0xfff)))) */

L_6eff:
    amountEdit = (amountEdit - lpfl->rgwtMin[j]);
    if ((HIWORD(amountEdit) > 0x0))
        goto L_6f49;
    else
        goto L_6f2e;

L_6f2e:
    if ((HIWORD(amountEdit) < 0x0))
        goto L_6f3d;
    else
        goto L_6f33;

L_6f33:
    if ((LOWORD(amountEdit) >= 0x0))
        goto L_6f49;
    else
        goto L_6f3d;

L_6f3d:
    amountEdit = 0x0;

L_6f49:
    goto L_7018;
    if ((iLoad == 0x0))
        goto L_7b74;
    else
        goto L_6f52;

L_6f52:

L_6f58:
    /* untranslated: LOWORD(amountEdit) = (ss:[bp+(j * 0x2)-0x74] & 0xfff) */
    HIWORD(amountEdit) = 0x0;
    goto L_7018;
    if ((iLoad != 0x0))
        goto L_7b74;
    else
        goto L_6f7b;

L_6f7b:

L_6f81:
    /* untranslated: LOWORD(amountEdit) = (ss:[bp+(j * 0x2)-0x74] & 0xfff) */
    HIWORD(amountEdit) = 0x0;
    goto L_7018;
    if ((iLoad == 0x0))
        goto L_7b74;
    else
        goto L_6fa4;

L_6fa4:

L_6faa:
    amountEdit = amountWP;
    goto L_7018;
    if ((iLoad != 0x0))
        goto L_7b74;
    else
        goto L_6fc1;

L_6fc1:

L_6fc7:
    amountEdit = lpfl->rgwtMin[j];
    goto L_7018;

L_6ff2:

L_7018:
    goto L_7b4e;

L_701e:
    goto L_7b74;
    amount = (amountWP - amountEdit);
    if ((HIWORD(amount) > 0x0))
        goto L_70cc;
    else
        goto L_703e;

L_703e:
    if ((HIWORD(amount) < 0x0))
        goto L_704c;
    else
        goto L_7043;

L_7043:
    if ((LOWORD(amount) >= 0x0))
        goto L_70cc;
    else
        goto L_704c;

L_704c:
    if ((iLoad != 0x0))
        goto L_7b74;
    else
        goto L_7052;

L_7052:

L_7058:
    /* untranslated: LOWORD(amount) = (LOWORD(amount) neg 0x0) */
    /* untranslated: HIWORD(amount) = ((HIWORD(amount) + 0x0) neg 0x0) */
    if ((HIWORD(amount) > HIWORD(lpfl->rgwtMin[j])))
        goto L_70a6;
    else
        goto L_7090;

L_7090:
    if ((HIWORD(amount) < HIWORD(lpfl->rgwtMin[j])))
        goto L_709d;
    else
        goto L_7095;

L_7095:
    if ((LOWORD(amount) >= LOWORD(lpfl->rgwtMin[j])))
        goto L_70a6;
    else
        goto L_709d;

L_709d:
    t_merge_70c3_0001_wide = amount;
    goto L_70c3;

L_70a6:
    t_merge_70c3_0001_wide = lpfl->rgwtMin[j];

L_70c3:
    amount = t_merge_70c3_0001_wide;
    goto Unload;

L_70cc:
    if ((iLoad == 0x0))
        goto L_7b74;
    else
        goto L_70d2;

L_70d2:

L_70d8:
    goto Load;
    amount = (amountEdit - lpfl->rgwtMin[j]);
    if ((HIWORD(amount) < 0x0))
        goto L_71b1;
    else
        goto L_710f;

L_710f:
    if ((HIWORD(amount) > 0x0))
        goto L_711d;
    else
        goto L_7114;

L_7114:
    if ((LOWORD(amount) < 0x0))
        goto L_71b1;
    else
        goto L_711d;

L_711d:
    if ((iLoad == 0x0))
        goto L_7b74;
    else
        goto L_7123;

L_7123:

L_7129:
    if ((HIWORD(amount) < HIWORD(amountWP)))
        goto Load;
    else
        goto L_7137;

L_7137:
    if ((HIWORD(amount) > HIWORD(amountWP)))
        goto L_7144;
    else
        goto L_713c;

L_713c:
    if ((LOWORD(amount) <= LOWORD(amountWP)))
        goto Load;
    else
        goto L_7144;

L_7144:
    fDone = 0x0;
    if ((iPass != 0x4))
        goto Load;
    else
        goto L_7153;

L_7153:
    if ((j != 0x3))
        goto L_7162;
    else
        goto L_715c;

L_715c:
    t_merge_7165_0001 = 0x122;
    goto L_7165;

L_7162:
    t_merge_7165_0001 = 0x121;

L_7165:
    idm = t_merge_7165_0001;
    /* untranslated: call FSendPlrMsg(lpfl->iPlayer, idm, (lpfl->id | 0x8000), lpfl->id, j, (ss:[bp+(j * 0x2)-0x74] & 0xfff), 0x0, xWP, idWP, 0x0) ->
     * callresult(int16_t) */

L_71b1:
    if ((iLoad != 0x0))
        goto L_7b74;
    else
        goto L_71b7;

L_71b7:

L_71bd:
    /* untranslated: LOWORD(amount) = (LOWORD(amount) neg 0x0) */
    /* untranslated: HIWORD(amount) = ((HIWORD(amount) + 0x0) neg 0x0) */
    goto Unload;
    if ((j != 0x4))
        goto L_71ef;
    else
        goto L_71dc;

L_71dc:
    wtOptimalFuel = 0x0;
    fOptFuel = 0x1;
    goto L_7b74;

L_71ef:
    if ((iLoad == 0x0))
        goto L_7b74;
    else
        goto L_71f5;

L_71f5:

L_71fb:
    if ((fDunnage >= 0x2))
        goto L_720e;
    else
        goto L_7205;

L_7205:
    fDunnage = 0x1;
    goto L_7b74;

L_720e:
    amount = amountWP;
    if ((LOWORD(amount) != 0x0))
        goto Load;
    else
        goto L_7223;

L_7223:
    if ((HIWORD(amount) != 0x0))
        goto Load;
    else
        goto L_7229;

L_7229:

L_722f:
    goto L_7b74;
    amount = amountEdit;

Load:
    if ((iLoad == 0x0))
        goto L_7b74;
    else
        goto L_7246;

L_7246:

L_724c:
    if ((LOWORD(amount) != 0x0))
        goto L_7261;
    else
        goto L_7255;

L_7255:
    if ((HIWORD(amount) == 0x0))
        goto L_7b74;
    else
        goto L_725b;

L_725b:

L_7261:
    if ((j != 0x4))
        goto L_7283;
    else
        goto L_726a;

L_726a:
    l = GetFuelFree(lpfl);
    goto L_7299;

L_7283:
    l = GetCargoFree(lpfl);

L_7299:
    if ((HIWORD(l) > HIWORD(amount)))
        goto L_72c1;
    else
        goto L_72a8;

L_72a8:
    if ((HIWORD(l) < HIWORD(amount)))
        goto L_72b6;
    else
        goto L_72ad;

L_72ad:
    if ((LOWORD(l) >= LOWORD(amount)))
        goto L_72c1;
    else
        goto L_72b6;

L_72b6:
    t_merge_72c7_0001_wide = l;
    goto L_72c7;

L_72c1:
    t_merge_72c7_0001_wide = amount;

L_72c7:
    amount = t_merge_72c7_0001_wide;
    if ((fHasPermission == 0x0))
        goto L_72e9;
    else
        goto L_72d6;

L_72d6:
    if ((ord.grobj != grobjOther))
        goto L_7386;
    else
        goto L_72e9;

L_72e9:
    if ((j != 0x4))
        goto L_7308;
    else
        goto L_72f2;

L_72f2:
    if ((fOptFuel == 0x0))
        goto L_7308;
    else
        goto L_72fb;

L_72fb:
    amount = 0x0;
    goto L_7b74;

L_7308:
    if ((iPass != 0x4))
        goto L_737d;
    else
        goto L_7311;

L_7311:
    goto L_733a;

L_731f:
    idm = idmAttemptedLoadPlanetDontControlOrderHas;
    goto L_7355;

L_7327:
    idm = idmAttemptedLoadFleetDontControlOrderHas;
    goto L_7355;

L_732f:
    idm = idmAttemptedLoadDeepSpaceAttemptUnsuccessful;
    goto L_7355;

L_733a:
    if ((ord.grobj == grobjPlanet))
        goto L_731f;
    else
        goto L_7342;

L_7342:
    if ((ord.grobj == grobjFleet))
        goto L_7327;
    else
        goto L_734a;

L_734a:
    if ((ord.grobj == grobjOther))
        goto L_732f;
    else
        goto L_7355;

L_7355:
    FSendPlrMsg2(lpfl->iPlayer, idm, (lpfl->id | 0x8000), lpfl->id, j);
    goto CancelOrder;

L_737d:
    fDone = 0x0;

L_7386:
    if ((fStealing == 0x0))
        goto L_73a7;
    else
        goto L_738f;

L_738f:
    if ((j == 0x3))
        goto L_73a1;
    else
        goto L_7398;

L_7398:
    if ((j != 0x4))
        goto L_73a7;
    else
        goto L_73a1;

L_73a1:
    fDone = 0x1;

L_73a7:
    if ((LOWORD(amount) != 0x0))
        goto L_73b9;
    else
        goto L_73b0;

L_73b0:
    if ((HIWORD(amount) == 0x0))
        goto L_7738;
    else
        goto L_73b9;

L_73b9:
    if ((HIWORD(amount) > HIWORD(amountWP)))
        goto L_73dd;
    else
        goto L_73c7;

L_73c7:
    if ((HIWORD(amount) < HIWORD(amountWP)))
        goto L_73d4;
    else
        goto L_73cc;

L_73cc:
    if ((LOWORD(amount) >= LOWORD(amountWP)))
        goto L_73dd;
    else
        goto L_73d4;

L_73d4:
    t_merge_73e3_0001_wide = amount;
    goto L_73e3;

L_73dd:
    t_merge_73e3_0001_wide = amountWP;

L_73e3:
    l = t_merge_73e3_0001_wide;
    /* untranslated: l2 = ChgCargo(ord.grobj, ord.id, j, words(((HIWORD(l) + 0x0) neg 0x0), (LOWORD(l) neg 0x0)), 0x0) */
    if ((LOWORD(l2) != 0x0))
        goto L_7432;
    else
        goto L_7429;

L_7429:
    if ((HIWORD(l2) == 0x0))
        goto L_7557;
    else
        goto L_7432;

L_7432:
    /* untranslated: l = ChgCargo(grobjFleet, lpfl->id, j, words(((HIWORD(l2) + 0x0) neg 0x0), (LOWORD(l2) neg 0x0)), 0x0) */
    if ((LOWORD(l) != 0x0))
        goto L_7476;
    else
        goto L_746c;

L_746c:
    if ((HIWORD(l) == 0x0))
        goto L_7557;
    else
        goto L_7476;

L_7476:
    if ((ord.grobj != grobjFleet))
        goto L_74f5;
    else
        goto L_7489;

L_7489:
    if ((lpfl->iPlayer == lpflWP->iPlayer))
        goto L_74f5;
    else
        goto L_749c;

L_749c:
    FSendPlrMsg(lpfl->iPlayer, 0x119, (lpfl->id | 0x8000), lpfl->id, LOWORD(l), (LOWORD((uint32_t)((l >> 0x10))) & 0xffff), j, (idWP & 0x7fff), 0x0, 0x0);
    goto L_7557;

L_74f5:
    if ((j == 0x3))
        goto L_7544;
    else
        goto L_753e;

L_753e:
    t_merge_7547_0001 = 0x2b;
    goto L_7547;

L_7544:
    t_merge_7547_0001 = 0x2c;

L_7547:
    FSendPlrMsg(lpfl->iPlayer, t_merge_7547_0001, (lpfl->id | 0x8000), lpfl->id, LOWORD(l), (LOWORD((uint32_t)((l >> 0x10))) & 0xffff), j, xWP, idWP, 0x0);

L_7557:
    if ((fFueling != 0x0))
        goto L_7569;
    else
        goto L_7560;

L_7560:
    if ((fMining == 0x0))
        goto L_76d7;
    else
        goto L_7569;

L_7569:
    /* untranslated: branch LOWORD(amount) != (LOWORD(l2) neg 0x0) ? L_7586 : L_757e */

L_757e:
    /* untranslated: branch HIWORD(amount) == ((HIWORD(l2) + 0x0) neg 0x0) ? L_76d7 : L_7586 */

L_7586:
    l = (amount + l2);
    /* untranslated: l2 = ChgCargo(grobjPlanet, pl.id, j, words(((HIWORD(l) + 0x0) neg 0x0), (LOWORD(l) neg 0x0)), 0x0) */
    if ((LOWORD(l2) != 0x0))
        goto L_75d9;
    else
        goto L_75d0;

L_75d0:
    if ((HIWORD(l2) == 0x0))
        goto L_76cb;
    else
        goto L_75d9;

L_75d9:
    /* untranslated: l = ChgCargo(grobjFleet, lpfl->id, j, words(((HIWORD(l2) + 0x0) neg 0x0), (LOWORD(l2) neg 0x0)), 0x0) */
    if ((fMining == 0x0))
        goto L_7668;
    else
        goto L_7612;

L_7612:
    FSendPlrMsg(lpfl->iPlayer, 0x7d, (lpfl->id | 0x8000), lpfl->id, LOWORD(l), (LOWORD((uint32_t)((l >> 0x10))) & 0xffff), j, lpflWP->id, pl.id, 0x0);
    goto L_76d7;

L_7668:
    if ((j == 0x3))
        goto L_76b5;
    else
        goto L_76af;

L_76af:
    t_merge_76b8_0001 = 0x2b;
    goto L_76b8;

L_76b5:
    t_merge_76b8_0001 = 0x2c;

L_76b8:
    FSendPlrMsg(lpfl->iPlayer, t_merge_76b8_0001, (lpfl->id | 0x8000), lpfl->id, LOWORD(l), (LOWORD((uint32_t)((l >> 0x10))) & 0xffff), j, xWP, pl.id, 0x0);

L_76cb:
    l = 0x0;

L_76d7:
    if ((LOWORD(amount) != LOWORD(l)))
        goto L_76ef;
    else
        goto L_76e7;

L_76e7:
    if ((HIWORD(amount) == HIWORD(l)))
        goto L_76fd;
    else
        goto L_76ef;

L_76ef:
    if ((action != iActionWaitPercent))
        goto L_76fd;
    else
        goto L_76f8;

L_76f8:
    fFulfilled = 0x0;

L_76fd:
    if ((LOWORD(l) != 0x0))
        goto L_7711;
    else
        goto L_7707;

L_7707:
    if ((HIWORD(l) == 0x0))
        goto L_775b;
    else
        goto L_7711;

L_7711:
    if ((action != iActionLoadDunnage))
        goto L_775b;
    else
        goto L_771a;

L_771a:
    if ((j != 0x4))
        goto L_775b;
    else
        goto L_7723;

L_7723:
    wtOptimalFuel = l;
    goto L_7b74;

L_7738:
    if ((action != iActionWaitPercent))
        goto L_775b;
    else
        goto L_7741;

L_7741:
    if ((j != 0x4))
        goto L_7753;
    else
        goto L_774a;

L_774a:
    fDone = 0x0;
    goto L_7b74;

L_7753:
    fFulfilled = 0x0;

L_775b:
    goto L_7b74;
    if ((HIWORD(lpfl->rgwtMin[j]) > HIWORD(amountEdit)))
        goto L_77b6;
    else
        goto L_778a;

L_778a:
    if ((HIWORD(lpfl->rgwtMin[j]) < HIWORD(amountEdit)))
        goto L_7796;
    else
        goto L_778f;

L_778f:
    if ((LOWORD(lpfl->rgwtMin[j]) >= LOWORD(amountEdit)))
        goto L_77b6;
    else
        goto L_7796;

L_7796:
    t_merge_77be_0001_wide = lpfl->rgwtMin[j];
    goto L_77be;

L_77b6:
    t_merge_77be_0001_wide = amountEdit;

L_77be:
    amount = t_merge_77be_0001_wide;

Unload:
    if ((iLoad != 0x0))
        goto L_7b74;
    else
        goto L_77ca;

L_77ca:

L_77d0:
    if ((j != 0x3))
        goto L_78af;
    else
        goto L_77d9;

L_77d9:
    if ((ord.grobj != grobjPlanet))
        goto L_78af;
    else
        goto L_77ec;

L_77ec:
    if ((pl.iPlayer == lpfl->iPlayer))
        goto L_78af;
    else
        goto L_77fb;

L_77fb:
    if ((pl.iPlayer != 0xffff))
        goto L_7847;
    else
        goto L_7804;

L_7804:
    if ((pl.fWasInhabited != 0x0))
        goto L_7847;
    else
        goto L_7817;

L_7817:
    idm = idmHasTriedBeamColonistsPlanetUninhabitedMust;

LCantDrop:
    FSendPlrMsg2(lpfl->iPlayer, idm, (lpfl->id | 0x8000), lpfl->id, pl.id);
    goto CancelOrder;

L_7847:
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) != raMacintosh))
        goto L_7876;
    else
        goto L_786b;

L_786b:
    idm = idmCaptainHasAttemptedBeamColonistsOverruledBridge;
    goto LCantDrop;

L_7876:
    if ((pl.fStarbase == 0x0))
        goto L_7891;
    else
        goto L_7889;

L_7889:
    idm = idmHasTriedBeamColonistsPlanetsStarbaseWould;
    goto LCantDrop;

L_7891:
    FQueueColonistDrop(lpfl, &(pl), amount);
    goto L_7ae8;

L_78af:
    if ((j != 0x3))
        goto L_78ef;
    else
        goto L_78b8;

L_78b8:
    if ((ord.grobj != grobjFleet))
        goto L_78ef;
    else
        goto L_78cb;

L_78cb:
    if ((lpfl->iPlayer == ((ord.id >> 0x9) & 0xf)))
        goto L_78ef;
    else
        goto L_78e4;

L_78e4:
    idm = idmAllowedTransferColonistsAnotherPlayer;
    goto LCantDrop;

L_78ef:
    if ((ord.grobj != grobjFleet))
        goto L_793f;
    else
        goto L_7902;

L_7902:
    /* untranslated: branch sext8to16(byte ds:[(rgplr[lpflWP->iPlayer].rgmdRelation + lpfl->iPlayer)]) != 0x2 ? L_793f : L_7932 */

L_7932:
    amount = 0x0;
    goto L_7ae8;

L_793f:
    if ((j != 0x3))
        goto L_7988;
    else
        goto L_7948;

L_7948:
    if ((ord.grobj != grobjOther))
        goto L_7988;
    else
        goto L_795b;

L_795b:
    FSendPlrMsg2(lpfl->iPlayer, 0x165, (lpfl->id | 0x8000), lpfl->id, 0x0);
    goto CancelOrder;

L_7988:
    if ((LOWORD(amount) != 0x0))
        goto L_799a;
    else
        goto L_7991;

L_7991:
    if ((HIWORD(amount) == 0x0))
        goto L_7ae8;
    else
        goto L_799a;

L_799a:
    if ((fFueling == 0x0))
        goto L_7a33;
    else
        goto L_79a3;

L_79a3:
    if ((j == 0x4))
        goto L_7a33;
    else
        goto L_79ac;

L_79ac:
    l = ChgCargo(grobjPlanet, pl.id, j, amount, 0x0);
    if ((j == 0x3))
        goto L_7a1d;
    else
        goto L_7a17;

L_7a17:
    t_merge_7a20_0001 = 0x2d;
    goto L_7a20;

L_7a1d:
    t_merge_7a20_0001 = 0x2e;

L_7a20:
    FSendPlrMsg(lpfl->iPlayer, t_merge_7a20_0001, (lpfl->id | 0x8000), lpfl->id, LOWORD(l), (LOWORD((uint32_t)((l >> 0x10))) & 0xffff), j, xWP, pl.id, 0x0);
    goto L_7ae8;

L_7a33:
    l = ChgCargo(ord.grobj, ord.id, j, amount, 0x0);
    if ((HIWORD(l) < 0x0))
        goto L_7ada;
    else
        goto L_7a69;

L_7a69:
    if ((HIWORD(l) > 0x0))
        goto L_7a78;
    else
        goto L_7a6e;

L_7a6e:
    if ((LOWORD(l) <= 0x0))
        goto L_7ada;
    else
        goto L_7a78;

L_7a78:
    if ((j == 0x3))
        goto L_7ac7;
    else
        goto L_7ac1;

L_7ac1:
    t_merge_7aca_0001 = 0x2d;
    goto L_7aca;

L_7ac7:
    t_merge_7aca_0001 = 0x2e;

L_7aca:
    FSendPlrMsg(lpfl->iPlayer, t_merge_7aca_0001, (lpfl->id | 0x8000), lpfl->id, LOWORD(l), (LOWORD((uint32_t)((l >> 0x10))) & 0xffff), j, xWP, idWP, 0x0);

L_7ada:
    amount = l;

L_7ae8:
    if ((LOWORD(amount) != 0x0))
        goto L_7afa;
    else
        goto L_7af1;

L_7af1:
    if ((HIWORD(amount) == 0x0))
        goto L_7b2a;
    else
        goto L_7afa;

L_7afa:
    /* untranslated: l = ChgCargo(grobjFleet, lpfl->id, j, words(((HIWORD(amount) + 0x0) neg 0x0), (LOWORD(amount) neg 0x0)), 0x0) */

L_7b2a:
    /* untranslated: ss:[bp+(j * 0x2)-0x74] = ((ss:[bp+(j * 0x2)-0x74] & 0xfff) | 0x0) */
    goto L_7b74;

L_7b4e:
    if (((action - 0x1) > 0x8))
        goto L_701e;
    else
        goto L_7b59;

L_7b59:
    goto L_ffffffff;

L_7b74:
    j = (j + 0x1);

L_7b78:
    if ((j < 0x5))
        goto L_6cb5;
    else
        goto L_7b81;

L_7b81:
    if ((fOptFuel == 0x0))
        goto FinishFleet;
    else
        goto L_7b8a;

L_7b8a:
    if ((iLoad == 0x0))
        goto FinishFleet;
    else
        goto L_7b93;

L_7b93:
    if ((fDunnage == 0x1))
        goto FinishFleet;
    else
        goto L_7b9d;

L_7b9d:
    if ((lpfl->cord > 0x1))
        goto L_7bca;
    else
        goto L_7baa;

L_7baa:
    amount = 0x0;
    iExcess = lpfl->rgwtMin[0x4];
    goto SetOptAmount;

L_7bca:
    amount = EstFuelUse(lpfl, 0x0, 0xffff, 0xffffffff, 0x0);
    if ((HIWORD(amount) < HIWORD(lpfl->rgwtMin[0x4])))
        goto L_7dfb;
    else
        goto L_7c05;

L_7c05:
    if ((HIWORD(amount) > HIWORD(lpfl->rgwtMin[0x4])))
        goto L_7c12;
    else
        goto L_7c0a;

L_7c0a:
    if ((LOWORD(amount) <= LOWORD(lpfl->rgwtMin[0x4])))
        goto L_7dfb;
    else
        goto L_7c12;

L_7c12:
    fDone = 0x0;
    if ((LOWORD(wtOptimalFuel) != 0x0))
        goto L_7c2b;
    else
        goto L_7c22;

L_7c22:
    if ((HIWORD(wtOptimalFuel) == 0x0))
        goto L_7c7d;
    else
        goto L_7c2b;

L_7c2b:
    FSendPlrMsg(lpfl->iPlayer, 0x2b, (lpfl->id | 0x8000), lpfl->id, LOWORD(wtOptimalFuel), (LOWORD((uint32_t)((wtOptimalFuel >> 0x10))) & 0xffff), 0x4, xWP,
                idWP, 0x0);

L_7c7d:
    if ((iPass != 0x4))
        goto L_7cce;
    else
        goto L_7c86;

L_7c86:
    if ((fHasPermission != 0x0))
        goto L_7cce;
    else
        goto L_7c8f;

L_7c8f:
    FSendPlrMsg(lpfl->iPlayer, 0x126, (lpfl->id | 0x8000), lpfl->id, xWP, idWP, 0x0, 0x0, 0x0, 0x0);
    goto FinishFleet;

L_7cce:
    if ((iPass != 0x2))
        goto FinishFleet;
    else
        goto L_7cd7;

L_7cd7:
    lMaxFuel = LGetFleetStat(lpfl, 0x1);
    if ((HIWORD(lMaxFuel) > HIWORD(amount)))
        goto L_7d78;
    else
        goto L_7d00;

L_7d00:
    if ((HIWORD(lMaxFuel) < HIWORD(amount)))
        goto L_7d0e;
    else
        goto L_7d05;

L_7d05:
    if ((LOWORD(lMaxFuel) >= LOWORD(amount)))
        goto L_7d78;
    else
        goto L_7d0e;

L_7d0e:
    FSendPlrMsg(lpfl->iPlayer, 0x3d, (lpfl->id | 0x8000), lpfl->id, LOWORD(lMaxFuel), (LOWORD((uint32_t)((lMaxFuel >> 0x10))) & 0xffff), LOWORD(amount),
                (LOWORD((uint32_t)((amount >> 0x10))) & 0xffff), 0x0, 0x0);
    goto FinishFleet;

L_7d78:
    cFuel2 = (amount - lpfl->rgwtMin[0x4]);
    if ((fMining != 0x2))
        goto L_7da1;
    else
        goto L_7d9a;

L_7d9a:
    idWP = pl.id;

L_7da1:
    FSendPlrMsg(lpfl->iPlayer, 0x3c, (lpfl->id | 0x8000), xWP, idWP, lpfl->id, LOWORD(cFuel2), (LOWORD((uint32_t)((cFuel2 >> 0x10))) & 0xffff), 0x0, 0x0);

L_7dfb:
    if ((HIWORD(amount) > HIWORD(lpfl->rgwtMin[0x4])))
        goto L_804f;
    else
        goto L_7e0e;

L_7e0e:
    if ((HIWORD(amount) < HIWORD(lpfl->rgwtMin[0x4])))
        goto L_7e1b;
    else
        goto L_7e13;

L_7e13:
    if ((LOWORD(amount) >= LOWORD(lpfl->rgwtMin[0x4])))
        goto L_804f;
    else
        goto L_7e1b;

L_7e1b:
    wtFuelOrig = lpfl->rgwtMin[0x4];

L_7e2e:
    lpfl->rgwtMin[0x4] = amount;
    amount = EstFuelUse(lpfl, 0x0, 0xffff, 0xffffffff, 0x0);
    if ((HIWORD(amount) < HIWORD(lpfl->rgwtMin[0x4])))
        goto L_7e2e;
    else
        goto L_7e7a;

L_7e7a:
    if ((HIWORD(amount) > HIWORD(lpfl->rgwtMin[0x4])))
        goto L_7e87;
    else
        goto L_7e7f;

L_7e7f:
    if ((LOWORD(amount) < LOWORD(lpfl->rgwtMin[0x4])))
        goto L_7e2e;
    else
        goto L_7e87;

L_7e87:
    iExcess = (wtFuelOrig - amount);
    lpfl->rgwtMin[0x4] = wtFuelOrig;

SetOptAmount:
    if ((LOWORD(iExcess) != 0x0))
        goto L_7ec4;
    else
        goto L_7eba;

L_7eba:
    if ((HIWORD(iExcess) == 0x0))
        goto L_7f37;
    else
        goto L_7ec4;

L_7ec4:
    l2 = ChgCargo(ord.grobj, ord.id, 0x4, iExcess, 0x0);
    if ((LOWORD(l2) != 0x0))
        goto L_7f03;
    else
        goto L_7efa;

L_7efa:
    if ((HIWORD(l2) == 0x0))
        goto L_7f43;
    else
        goto L_7f03;

L_7f03:
    /* untranslated: l = ChgCargo(grobjFleet, lpfl->id, 0x4, words(((HIWORD(l2) + 0x0) neg 0x0), (LOWORD(l2) neg 0x0)), 0x0) */

L_7f37:
    l = 0x0;

L_7f43:
    if ((LOWORD(l2) != 0x0))
        goto L_7f55;
    else
        goto L_7f4c;

L_7f4c:
    if ((HIWORD(l2) == 0x0))
        goto L_804f;
    else
        goto L_7f55;

L_7f55:
    l = (l + wtOptimalFuel);
    if ((HIWORD(l) < 0x0))
        goto L_7fd4;
    else
        goto L_7f6e;

L_7f6e:
    if ((HIWORD(l) > 0x0))
        goto L_7f7d;
    else
        goto L_7f73;

L_7f73:
    if ((LOWORD(l) <= 0x0))
        goto L_7fd4;
    else
        goto L_7f7d;

L_7f7d:
    FSendPlrMsg(lpfl->iPlayer, 0x2b, (lpfl->id | 0x8000), lpfl->id, LOWORD(l), (LOWORD((uint32_t)((l >> 0x10))) & 0xffff), 0x4, xWP, idWP, 0x0);
    goto L_804f;

L_7fd4:
    if ((HIWORD(l) > 0x0))
        goto L_804f;
    else
        goto L_7fde;

L_7fde:
    if ((HIWORD(l) < 0x0))
        goto L_7fed;
    else
        goto L_7fe3;

L_7fe3:
    if ((LOWORD(l) >= 0x0))
        goto L_804f;
    else
        goto L_7fed;

L_7fed:
    /* untranslated: call FSendPlrMsg(lpfl->iPlayer, 0x2d, (lpfl->id | 0x8000), lpfl->id, (LOWORD(l) neg 0x0), (loword((uint32_t)(words((LOWORD(l) neg 0x0),
     * ((HIWORD(l) + 0x0) neg 0x0)) >> 0x10)) & 0xffff), 0x4, xWP, idWP, 0x0) -> callresult(int16_t) */

L_804f:
    if ((fDone == 0x0))
        goto FinishFleet;
    else
        goto L_8059;

L_8059:
    if ((fFulfilled == 0x0))
        goto FinishFleet;
    else
        goto L_8062;

L_8062:
    ord.txp.rgia[0x4] = ((ord.txp.rgia[0x4] & 0xfff) | 0x0);
    ord.txp.rgia[0x4] = ((ord.txp.rgia[0x4] & 0xf000) | 0x0);

FinishFleet:
    if ((fFulfilled != 0x0))
        goto L_80ac;
    else
        goto L_8083;

L_8083:
    if ((HIWORD(GetCargoFree(lpfl)) < 0x0))
        goto L_80ac;
    else
        goto L_8099;

L_8099:
    /* untranslated: branch hiword(callresult(int32_t)) > 0x0 ? L_80a6 : L_809e */

L_809e:
    /* untranslated: branch loword(callresult(int32_t)) <= 0x0 ? L_80ac : L_80a6 */

L_80a6:
    fDone = 0x0;

L_80ac:
    if ((fDone == 0x0))
        goto L_80f5;
    else
        goto L_80b6;

L_80b6:
    if ((fDunnage != 0x1))
        goto L_80f5;
    else
        goto L_80c0;

L_80c0:
    if ((HIWORD(GetCargoFree(lpfl)) > 0x0))
        goto L_80ec;
    else
        goto L_80d6;

L_80d6:
    /* untranslated: branch hiword(callresult(int32_t)) < 0x0 ? L_80e3 : L_80db */

L_80db:
    /* untranslated: branch loword(callresult(int32_t)) > 0x0 ? L_80ec : L_80e3 */

L_80e3:
    if ((fOptFuel == 0x0))
        goto L_80f5;
    else
        goto L_80ec;

L_80ec:
    fDunnage = 0x2;
    goto LTryDunnage;

L_80f5:
    if ((fDone == 0x0))
        goto L_810b;
    else
        goto L_80ff;

L_80ff:
    if ((iLoad != 0x0))
        goto CancelOrder;
    else
        goto L_8105;

L_8105:

L_810b:
    if ((fMining != 0x2))
        goto L_8126;
    else
        goto L_8114;

L_8114:
    ord.id = pl.id;
    ord.grobj = grobjPlanet;

L_8126:
    lpfl->lpplord->rgord[0x0] = ord;
    goto L_67b6;

L_8155:
    if ((ord.grTask != grTaskScrap))
        goto L_816c;
    else
        goto L_8163;

L_8163:
    if ((iPass == 0x1))
        goto L_817a;
    else
        goto L_816c;

L_816c:
    if ((ord.grTask != grTaskColonize))
        goto L_8f02;
    else
        goto L_817a;

L_817a:
    if ((lpfl->idPlanet != 0xffff))
        goto L_81db;
    else
        goto L_8187;

L_8187:
    if ((ord.grTask != grTaskColonize))
        goto L_81c2;
    else
        goto L_8195;

L_8195:
    FSendPlrMsg2(lpfl->iPlayer, 0x51, (lpfl->id | 0x8000), lpfl->id, 0x0);
    goto CancelOrder;

L_81c2:
    pl.id = 0xffff;
    pl.iPlayer = 0xffff;
    pl.fStarbase = 0x0;
    goto LScrap;

L_81db:
    if ((FLookupPlanet(lpfl->idPlanet, &(pl)) != 0x0))
        goto LScrap;
    else
        goto L_81f6;

L_81f6:
    if ((ord.grTask == grTaskColonize))
        goto CancelOrder;
    else
        goto L_8201;

L_8201:

LScrap:
    if ((ord.grTask != grTaskColonize))
        goto L_829a;
    else
        goto L_8215;

L_8215:
    if ((pl.iPlayer == 0xffff))
        goto L_825a;
    else
        goto L_821e;

L_821e:
    FSendPlrMsg(lpfl->iPlayer, 0x52, (lpfl->id | 0x8000), lpfl->id, pl.id, pl.id, 0x0, 0x0, 0x0, 0x0);
    goto CancelOrder;

L_825a:
    if ((LOWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_829a;
    else
        goto L_8267;

L_8267:
    if ((HIWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_829a;
    else
        goto L_8271;

L_8271:
    FSendPlrMsg2(lpfl->iPlayer, 0x53, (lpfl->id | 0x8000), lpfl->id, pl.id);
    goto CancelOrder;

L_829a:
    i = 0x0;
    goto L_82bf;

L_82a3:
    rgwt[i] = 0x0;
    i = (i + 0x1);

L_82bf:
    if ((i < 0x3))
        goto L_82a3;
    else
        goto L_82c9;

L_82c9:
    fColonize = 0x0;
    csh = 0x0;
    memset(&(rgTechBattle), 0x0, 0x6);
    memset(&(rgTechTrader), 0x0, 0xd);
    i = 0x0;
    goto L_8466;

L_8306:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_8461;
    else
        goto L_8324;

L_8324:
    csh = (csh + lpfl->rgcsh[i]);
    MarkTechsSeen(&(rglpshdef[lpfl->iPlayer][i]), lpfl->iPlayer);
    j = 0x0;
    goto L_837b;

L_8376:
    j = (j + 0x1);

L_837b:
    if ((j >= rglpshdef[lpfl->iPlayer][i].hul.chs))
        goto L_8461;
    else
        goto L_83a9;

L_83a9:
    if ((rglpshdef[lpfl->iPlayer][i].hul.rghs[j].grhst != hstSpecialM))
        goto L_8376;
    else
        goto L_83e0;

L_83e0:
    if (((HIWORD(rglpshdef[lpfl->iPlayer][i].hul.rghs[j]) & 0xff) == 0x0))
        goto L_8458;
    else
        goto L_841c;

L_841c:
    if (((HIWORD(rglpshdef[lpfl->iPlayer][i].hul.rghs[j]) & 0xff) != 0x1))
        goto L_8376;
    else
        goto L_8458;

L_8458:
    fColonize = 0x1;

L_8461:
    i = (i + 0x1);

L_8466:
    if ((i < 0x10))
        goto L_8306;
    else
        goto L_8470;

L_8470:
    if ((ord.grTask != grTaskColonize))
        goto L_84c7;
    else
        goto L_847e;

L_847e:
    if ((fColonize != 0x0))
        goto L_84c7;
    else
        goto L_8488;

L_8488:
    FSendPlrMsg(lpfl->iPlayer, 0x54, (lpfl->id | 0x8000), lpfl->id, pl.id, lpfl->id, 0x0, 0x0, 0x0, 0x0);
    goto CancelOrder;

L_84c7:
    lXferMinerals = 0x0;
    if ((ord.grTask != grTaskScrap))
        goto L_8510;
    else
        goto L_84e1;

L_84e1:
    if ((pl.iPlayer == 0xffff))
        goto L_8510;
    else
        goto L_84ea;

L_84ea:
    if ((GetRaceGrbit(rgplr[pl.iPlayer], 0x5) == 0x0))
        goto L_8510;
    else
        goto L_850a;

L_850a:
    t_merge_8513_0001 = 0x1;
    goto L_8513;

L_8510:
    t_merge_8513_0001 = 0x0;

L_8513:
    fUltimate = t_merge_8513_0001;
    fBleeding = GetRaceGrbit(rgplr[lpfl->iPlayer], ibitRaceBleedingEdgeTech);
    gd.fDontCalcBleed = 0x1;
    idPlayer = lpfl->iPlayer;
    i = 0x0;
    goto L_886b;

L_8556:
    lAmt = 0x0;
    j = 0x0;
    goto L_869e;

L_856b:
    if ((lpfl->rgcsh[j] <= 0x0))
        goto L_8699;
    else
        goto L_8586;

L_8586:

L_858c:
    if ((fBleeding == 0x0))
        goto L_85e9;
    else
        goto L_8596;

L_8596:
    shdefT = rglpshdef[lpfl->iPlayer][j];
    UpdateShdefCost(&(shdefT));
    LOWORD(lpshdefT) = shdefT;
    /* untranslated: HIWORD(lpshdefT) = ss */
    goto L_860d;

L_85e9:
    lpshdefT = &(rglpshdef[lpfl->iPlayer][j]);

L_860d:
    /* untranslated: lT = (uint32_t)(sext16to32(lpfl->rgcsh[j]) * words(0x0, lpshdefT->hul.rgwtOreCost[i])) */
    if ((lpshdefT->fGift == 0x0))
        goto L_8689;
    else
        goto L_866c;

L_866c:
    lT = (int32_t)((lT / 0x4));

L_8689:
    lAmt = (lAmt + lT);

L_8699:
    j = (j + 0x1);

L_869e:
    if ((j < 0x10))
        goto L_856b;
    else
        goto L_86a8;

L_86a8:
    if ((ord.grTask != grTaskColonize))
        goto L_86e5;
    else
        goto L_86b6;

L_86b6:
    lAmt = (int32_t)(((uint32_t)((lAmt * 0x3)) / 0x4));
    goto L_87da;

L_86e5:
    if ((pl.id != 0xffff))
        goto L_870e;
    else
        goto L_86ee;

L_86ee:
    lAmt = (int32_t)((lAmt / 0x3));
    goto L_87da;

L_870e:
    if ((pl.fStarbase == 0x0))
        goto L_8784;
    else
        goto L_8721;

L_8721:
    if ((fUltimate == 0x0))
        goto L_875a;
    else
        goto L_872b;

L_872b:
    lAmt = (int32_t)(((uint32_t)((lAmt * 0x9)) / 0xa));
    goto L_87da;

L_875a:
    lAmt = (int32_t)(((int32_t)((lAmt * 0x4)) / 0x5));

L_8784:
    if ((fUltimate == 0x0))
        goto L_87bd;
    else
        goto L_878e;

L_878e:
    lAmt = (int32_t)(((uint32_t)((lAmt * 0x9)) / 0x14));
    goto L_87da;

L_87bd:
    lAmt = (int32_t)((lAmt / 0x3));

L_87da:
    lAmt = (lAmt + lpfl->rgwtMin[i]);
    lXferMinerals = (lXferMinerals + lAmt);
    if ((pl.id != 0xffff))
        goto L_8836;
    else
        goto L_8819;

L_8819:
    pl.rgwtMin[i] = lAmt;
    goto L_8866;

L_8836:
    lpPlanets[pl.id].rgwtMin[i] = (lpPlanets[pl.id].rgwtMin[i] + lAmt);

L_8866:
    i = (i + 0x1);

L_886b:
    if ((i <= 0x2))
        goto L_8556;
    else
        goto L_8875;

L_8875:
    iGoto = pl.id;
    if ((fUltimate == 0x0))
        goto L_8be2;
    else
        goto L_8886;

L_8886:
    lResUltimate = 0x0;
    j = 0x0;
    goto L_89b9;

L_889b:
    if ((lpfl->rgcsh[j] <= 0x0))
        goto L_89b4;
    else
        goto L_88b9;

L_88b9:
    if ((fBleeding == 0x0))
        goto L_8916;
    else
        goto L_88c3;

L_88c3:
    shdefT = rglpshdef[lpfl->iPlayer][j];
    UpdateShdefCost(&(shdefT));
    LOWORD(lpshdefT) = shdefT;
    /* untranslated: HIWORD(lpshdefT) = ss */
    goto L_893a;

L_8916:
    lpshdefT = &(rglpshdef[lpfl->iPlayer][j]);

L_893a:
    /* untranslated: lT = (uint32_t)(sext16to32(lpfl->rgcsh[j]) * words(0x0, lpshdefT->hul.resCost)) */
    if ((lpshdefT->fGift == 0x0))
        goto L_89a4;
    else
        goto L_8987;

L_8987:
    lT = (int32_t)((lT / 0x4));

L_89a4:
    lResUltimate = (lResUltimate + lT);

L_89b4:
    j = (j + 0x1);

L_89b9:
    if ((j < 0x10))
        goto L_889b;
    else
        goto L_89c3;

L_89c3:
    if ((HIWORD(lResUltimate) < 0x0))
        goto L_89e8;
    else
        goto L_89cd;

L_89cd:
    if ((HIWORD(lResUltimate) > 0x0))
        goto L_89dc;
    else
        goto L_89d2;

L_89d2:
    if ((LOWORD(lResUltimate) <= 0xffff))
        goto L_89e8;
    else
        goto L_89dc;

L_89dc:
    lResUltimate = 0xffff;

L_89e8:
    vrgPlanResExtra[pl.id] = (vrgPlanResExtra[pl.id] + LOWORD(lResUltimate));
    if ((HIWORD(lResUltimate) < 0x0))
        goto L_8a77;
    else
        goto L_8a0e;

L_8a0e:
    if ((HIWORD(lResUltimate) > 0x0))
        goto L_8a1d;
    else
        goto L_8a13;

L_8a13:
    if ((LOWORD(lResUltimate) <= 0x0))
        goto L_8a77;
    else
        goto L_8a1d;

L_8a1d:
    if ((pl.iPlayer == 0xffff))
        goto L_8a77;
    else
        goto L_8a26;

L_8a26:
    lAmt = (uint32_t)(CResourcesAtPlanet(&(pl), pl.iPlayer));
    /* untranslated: lResUltimate = (int32_t)((uint32_t)(lResUltimate * lAmt) / words((HIWORD(lResUltimate) + HIWORD(lAmt)), (LOWORD(lResUltimate) +
     * LOWORD(lAmt)))) */

L_8a77:
    idm = (pl.fStarbase + 0x5c);
    FSendPlrMsg(lpfl->iPlayer, idm, iGoto, WFromLpfl(lpfl), LOWORD(lXferMinerals), (LOWORD((uint32_t)((lXferMinerals >> 0x10))) & 0xffff), pl.id,
                LOWORD(lResUltimate), (LOWORD((uint32_t)((lResUltimate >> 0x10))) & 0xffff), 0x0);
    idm = (pl.fStarbase + 0x142);
    if ((pl.fStarbase == 0x0))
        goto L_8b78;
    else
        goto L_8b19;

L_8b19:
    i = ITechLearnATech(pl.iPlayer, 0x0, 0x0, 0xffff, &(iGoto));
    if ((i == 0x0))
        goto L_8b6e;
    else
        goto L_8b43;

L_8b43:
    idm = idmHasDismantledKtMineralsWhichHaveDeposited3;
    if ((i >= 0x0))
        goto L_8b62;
    else
        goto L_8b52;

L_8b52:
    /* untranslated: i = ((i + 0x1) neg 0x0) */
    goto L_8b7f;

L_8b62:
    i = (i - 0x1);
    idm = (idm + 0x1);

L_8b6e:
    iGoto = pl.id;

L_8b78:
    iGoto = pl.id;

L_8b7f:
    FSendPlrMsg(pl.iPlayer, idm, iGoto, lpfl->id, LOWORD(lXferMinerals), (LOWORD((uint32_t)((lXferMinerals >> 0x10))) & 0xffff), pl.id, LOWORD(lResUltimate),
                (LOWORD((uint32_t)((lResUltimate >> 0x10))) & 0xffff), i);
    goto L_8d7d;

L_8be2:
    if ((pl.id != 0xffff))
        goto L_8c4f;
    else
        goto L_8beb;

L_8beb:
    lpthWP = 0x0;
    DropSalvage(&(lpthWP), pl.rgwtMin, lpfl->iplr, &(ord.pt));
    FSendPlrMsg2(lpfl->iPlayer, 0x5b, 0xfffa, lpthWP->idFull, WFromLpfl(lpfl));
    goto L_8d7d;

L_8c4f:
    idm = (pl.fStarbase + 0x59);
    FSendPlrMsg(lpfl->iPlayer, idm, iGoto, WFromLpfl(lpfl), LOWORD(lXferMinerals), (LOWORD((uint32_t)((lXferMinerals >> 0x10))) & 0xffff), pl.id, 0x0, 0x0,
                0x0);
    idm = (pl.fStarbase + 0x140);
    if ((pl.fStarbase == 0x0))
        goto L_8d35;
    else
        goto L_8cd9;

L_8cd9:
    i = ITechLearnATech(pl.iPlayer, 0x0, 0x0, 0xffff, &(iGoto));
    if ((i == 0x0))
        goto L_8d2e;
    else
        goto L_8d03;

L_8d03:
    idm = idmHasDismantledKtMineralsStarbaseOrbitingProcess;
    if ((i >= 0x0))
        goto L_8d22;
    else
        goto L_8d12;

L_8d12:
    /* untranslated: i = ((i + 0x1) neg 0x0) */
    goto L_8d35;

L_8d22:
    i = (i - 0x1);
    idm = (idm + 0x1);

L_8d2e:
    iGoto = pl.id;

L_8d35:
    FSendPlrMsg(pl.iPlayer, idm, iGoto, lpfl->id, LOWORD(lXferMinerals), (LOWORD((uint32_t)((lXferMinerals >> 0x10))) & 0xffff), pl.id, i, 0x0, 0x0);

L_8d7d:
    idPlayer = 0xffff;
    gd.fDontCalcBleed = 0x0;
    FRemovePlayerMessage(lpfl->iPlayer, 0x4e, (lpfl->id | 0x8000));
    lpfl->fDead = 0x1;
    if ((ord.grTask != grTaskColonize))
        goto L_8df1;
    else
        goto L_8dce;

L_8dce:
    FQueueColonistDrop(lpfl, &(pl), lpfl->rgwtMin[0x3]);
    goto CancelOrder;

L_8df1:
    if ((lpfl->idPlanet == 0xffff))
        goto CancelOrder;
    else
        goto L_8dfe;

L_8dfe:
    if ((lpPlanets[lpfl->idPlanet].iPlayer != lpfl->iPlayer))
        goto CancelOrder;
    else
        goto L_8e2c;

L_8e2c:
    lpPlanets[lpfl->idPlanet].rgwtMin[0x3] = (lpPlanets[lpfl->idPlanet].rgwtMin[0x3] + lpfl->rgwtMin[0x3]);

CancelOrder:
    if ((lpfl->cord != 0x1))
        goto L_8ee3;
    else
        goto L_8e72;

L_8e72:
    if ((lpfl->fDead != 0x0))
        goto L_8ee3;
    else
        goto L_8e89;

L_8e89:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) == 0x0 ? L_8ee3 : L_8e9f */

L_8e9f:
    FRemovePlayerMessage(lpfl->iPlayer, 0x4e, (lpfl->id | 0x8000));
    FSendPlrMsg2(lpfl->iPlayer, 0x4e, (lpfl->id | 0x8000), lpfl->id, 0x0);

L_8ee3:
    /* untranslated: part[6:2](lpfl->lpplord->rgord[0x0]) = ((part[6:2](lpfl->lpplord->rgord[0x0]) & 0xfff0) | 0x0) */
    goto L_67b6;

L_8f02:
    if ((ord.grTask != grTaskMine))
        goto L_9081;
    else
        goto L_8f10;

L_8f10:
    cMine = 0x0;
    if ((iPass != 0x3))
        goto L_67b6;
    else
        goto L_8f25;

L_8f25:
    if ((lpfl->fHereAllTurn == 0x0))
        goto L_67b6;
    else
        goto L_8f39;

L_8f39:

L_8f3f:
    if ((lpfl->idPlanet != 0xffff))
        goto L_8f76;
    else
        goto L_8f4c;

L_8f4c:
    FSendPlrMsg2(lpfl->iPlayer, 0x77, (lpfl->id | 0x8000), lpfl->id, 0x0);
    goto CancelOrder;

L_8f76:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((LOWORD(lppl) != 0x0))
        goto L_8fa4;
    else
        goto L_8f97;

L_8f97:
    if ((HIWORD(lppl) == 0x0))
        goto CancelOrder;
    else
        goto L_8f9e;

L_8f9e:

L_8fa4:
    cMine = CMineFromLpfl(lpfl);
    if ((LOWORD(cMine) != 0x0))
        goto L_8ffb;
    else
        goto L_8fc4;

L_8fc4:
    if ((HIWORD(cMine) != 0x0))
        goto L_8ffb;
    else
        goto L_8fce;

L_8fce:
    FSendPlrMsg2(lpfl->iPlayer, 0x75, (lpfl->id | 0x8000), lpfl->id, lppl->id);
    goto CancelOrder;

L_8ffb:
    if ((lppl->iPlayer == 0xffff))
        goto L_905d;
    else
        goto L_9009;

L_9009:
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) == raMacintosh))
        goto L_67b6;
    else
        goto L_902d;

L_902d:
    FSendPlrMsg2(lpfl->iPlayer, 0x76, (lpfl->id | 0x8000), lpfl->id, lppl->id);
    goto CancelOrder;

L_905d:
    EstMineralsMined(lppl, rglQuan, cMine, 0x1);
    goto L_67b6;

L_9081:
    if ((ord.grTask != grTaskAutoRoute))
        goto L_91c3;
    else
        goto L_908f;

L_908f:
    if ((lpfl->cord != 0x1))
        goto L_67b6;
    else
        goto L_909c;

L_909c:
    if ((lpfl->idPlanet == 0xffff))
        goto L_67b6;
    else
        goto L_90a9;

L_90a9:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((lppl->iPlayer != lpfl->iPlayer))
        goto L_916d;
    else
        goto L_90d4;

L_90d4:
    if ((lppl->idRoute == 0x0))
        goto L_916d;
    else
        goto L_90e7;

L_90e7:
    AutoRouteFleet(lpfl, lppl);
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x4) & 0xf) != 0x0 ? L_9157 : L_9151 */

L_9151:
    t_merge_915a_0001 = 0x128;
    goto L_915a;

L_9157:
    t_merge_915a_0001 = 0x127;

L_915a:
    FSendPlrMsg(lpfl->iPlayer, t_merge_915a_0001, (lpfl->id | 0x8000), lpfl->id, lpfl->idPlanet, (lppl->idRoute + 0xffff), 0x0, 0x0, 0x0, 0x0);
    goto L_67b6;

L_916d:
    if ((iPass != 0x4))
        goto L_67b6;
    else
        goto L_9176;

L_9176:
    AutoFleetOrder(lpfl, lppl);
    ord = lpfl->lpplord->rgord[0x0];
    if ((ord.grTask == grTaskMerge))
        goto LDoMerge;
    else
        goto L_91ba;

L_91ba:

L_91c3:
    if ((ord.grTask != grTaskMerge))
        goto L_932b;
    else
        goto L_91d1;

L_91d1:
    if (((iPass & 0x1) != 0x0))
        goto L_67b6;
    else
        goto L_91dc;

L_91dc:

LDoMerge:
    if ((ord.grobj == grobjFleet))
        goto L_921f;
    else
        goto NMNF;

NMNF:
    FSendPlrMsg2(lpfl->iPlayer, 0xf5, (lpfl->id | 0x8000), lpfl->id, 0x0);
    goto CancelOrder;

L_921f:
    lpflDest = LpflFromId(ord.id);
    if ((LOWORD(lpflDest) != 0x0))
        goto L_9246;
    else
        goto L_923c;

L_923c:
    if ((HIWORD(lpflDest) == 0x0))
        goto NMNF;
    else
        goto L_9246;

L_9246:
    if ((lpflDest->fDead != 0x0))
        goto NMNF;
    else
        goto L_925b;

L_925b:

L_9261:
    if ((LOWORD(lpfl) != LOWORD(lpflDest)))
        goto L_927c;
    else
        goto L_9271;

L_9271:
    if ((HIWORD(lpfl) == HIWORD(lpflDest)))
        goto CancelOrder;
    else
        goto L_9276;

L_9276:

L_927c:
    if ((lpflDest->iPlayer == lpfl->iPlayer))
        goto L_92ba;
    else
        goto L_9290;

L_9290:
    FSendPlrMsg2(lpfl->iPlayer, 0xf6, (lpfl->id | 0x8000), lpfl->id, 0x0);
    goto CancelOrder;

L_92ba:
    FSendPlrMsg2(lpfl->iPlayer, 0xf7, (lpflDest->id | 0x8000), WFromLpfl(lpfl), lpflDest->id);
    FRemovePlayerMessage(lpfl->iPlayer, 0x4e, (lpfl->id | 0x8000));
    Merge2Fleets(lpflDest, lpfl, 0x1);
    goto CancelOrder;

L_932b:
    if ((ord.grTask != grTaskGive))
        goto L_993b;
    else
        goto L_9339;

L_9339:
    if ((iPass != 0x4))
        goto L_67b6;
    else
        goto L_933f;

L_933f:

L_9345:
    iplrDest = ord.tsell.iPlrX;
    if ((iplrDest < lpfl->iPlayer))
        goto L_9361;
    else
        goto L_935c;

L_935c:
    iplrDest = (iplrDest + 0x1);

L_9361:
    if ((iplrDest < 0x0))
        goto L_9391;
    else
        goto L_936b;

L_936b:
    if ((iplrDest >= game.cPlayer))
        goto L_9391;
    else
        goto L_9377;

L_9377:
    if (((rgplr[iplrDest].wFlags & 0x1) == 0x0))
        goto L_93bb;
    else
        goto L_9391;

L_9391:
    FSendPlrMsg2(lpfl->iPlayer, 0x148, (lpfl->id | 0x8000), lpfl->id, 0x0);
    goto CancelOrder;

L_93bb:
    if ((((rgplr[iplrDest].wMdPlr >> 0x9) & 0x1) != 0x0))
        goto L_9407;
    else
        goto L_93da;

L_93da:
    /* untranslated: branch sext8to16(byte ds:[(rgplr[iplrDest].rgmdRelation + lpfl->iPlayer)]) != 0x2 ? L_9433 : L_9407 */

L_9407:
    FSendPlrMsg2(lpfl->iPlayer, 0x14c, (lpfl->id | 0x8000), (0x30 | iplrDest), 0x0);
    goto CancelOrder;

L_9433:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < 0x0))
        goto L_9479;
    else
        goto L_9440;

L_9440:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > 0x0))
        goto L_944f;
    else
        goto L_9445;

L_9445:
    if ((LOWORD(lpfl->rgwtMin[0x3]) <= 0x0))
        goto L_9479;
    else
        goto L_944f;

L_944f:
    FSendPlrMsg2(lpfl->iPlayer, 0x149, (lpfl->id | 0x8000), lpfl->id, 0x0);
    goto CancelOrder;

L_9479:
    ishLastFree = 0xffff;
    ish = 0x0;
    goto L_95be;

L_9488:
    if ((lpfl->rgcsh[ish] != 0x0))
        goto L_94b9;
    else
        goto L_94a6;

L_94a6:
    rgishMap[ish] = 0xffff;
    goto L_95b9;

L_94b9:
    ishMatch = IshFindSimilarDesign(&(rglpshdef[lpfl->iPlayer][ish]), iplrDest);
    if ((ishMatch == 0xffff))
        goto L_9506;
    else
        goto L_94f1;

L_94f1:
    rgishMap[ish] = ishMatch;
    goto L_95b9;

L_9506:
    ishLastFree = (ishLastFree + 0x1);
    if ((ishLastFree >= 0x10))
        goto L_954c;
    else
        goto L_9517;

L_9517:
    if ((((rglpshdef[iplrDest][ishLastFree].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_954c;
    else
        goto L_9543;

L_9543:

L_954c:
    if ((ishLastFree < 0x10))
        goto L_95a7;
    else
        goto SellNoCap;

SellNoCap:
    FSendPlrMsg2(lpfl->iPlayer, 0x14a, (lpfl->id | 0x8000), lpfl->id, (0x30 | iplrDest));
    FSendPlrMsg2(iplrDest, 0x14b, 0xffff, (0x30 | lpfl->iPlayer), 0x0);
    goto CancelOrder;

L_95a7:
    rgishMap[ish] = ishLastFree;

L_95b9:
    ish = (ish + 0x1);

L_95be:
    if ((ish < 0x10))
        goto L_9488;
    else
        goto L_95c8;

L_95c8:
    /* untranslated: branch (part[4:2](rgplr[iplrDest]) & 0xfff) >= 0x200 ? L_9556 : L_95df */

L_95df:

L_95e5:
    lpflNew = LpflNew(iplrDest, lpfl->idPlanet);
    if ((LOWORD(lpflNew) != 0x0))
        goto L_9617;
    else
        goto L_960a;

L_960a:
    if ((HIWORD(lpflNew) == 0x0))
        goto SellNoCap;
    else
        goto L_9611;

L_9611:

L_9617:
    if ((iplrDest >= lpfl->iPlayer))
        goto L_962b;
    else
        goto L_9627;

L_9627:
    ifltcur = (ifltcur + 0x1);

L_962b:
    lpflNew->pt.x = lpfl->pt.x;
    lpflNew->pt.y = lpfl->pt.y;
    lpflNew->lpplord->rgord[0x0].pt.x = lpfl->pt.x;
    lpflNew->lpplord->rgord[0x0].pt.y = lpfl->pt.y;
    ish = 0x0;
    goto L_96a9;

L_9666:
    lpflNew->rgwtMin[ish] = lpfl->rgwtMin[ish];
    ish = (ish + 0x1);

L_96a9:
    if ((ish <= 0x4))
        goto L_9666;
    else
        goto L_96b3;

L_96b3:
    ish = 0x0;
    goto L_989a;

L_96bc:
    if ((lpfl->rgcsh[ish] <= 0x0))
        goto L_9895;
    else
        goto L_96da;

L_96da:
    lpshdefDest = &(rglpshdef[iplrDest][rgishMap[ish]]);
    if ((lpshdefDest->fFree == 0x0))
        goto L_97d0;
    else
        goto L_971d;

L_971d:
    *(lpshdefDest) = rglpshdef[lpfl->iPlayer][ish];
    lpshdefDest->wFlags = ((lpshdefDest->wFlags & 0x83ff) | ((rgishMap[ish] & 0x1f) << 0xa));
    lpshdefDest->fGift = 0x1;
    lpshdefDest->cBuilt = 0x0;
    lpshdefDest->cExist = 0x0;
    rgplr[iplrDest].cShDef = (rgplr[iplrDest].cShDef + 0x1);

L_97d0:
    lpflNew->rgcsh[rgishMap[ish]] = (lpflNew->rgcsh[rgishMap[ish]] + lpfl->rgcsh[ish]);
    /* untranslated: HIWORD(lpflNew):[((LOWORD(lpflNew) + 0x2c) + (rgishMap[ish] * 0x2))] = HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ish * 0x2))] */
    lpshdefDest->cBuilt = (lpshdefDest->cBuilt + (uint32_t)(lpfl->rgcsh[ish]));
    lpshdefDest->cExist = (lpshdefDest->cExist + (uint32_t)(lpfl->rgcsh[ish]));

L_9895:
    ish = (ish + 0x1);

L_989a:
    if ((ish < 0x10))
        goto L_96bc;
    else
        goto L_98a4;

L_98a4:
    lpfl->fDead = 0x1;
    FSendPlrMsg2(lpfl->iPlayer, 0x14d, (lpflNew->id | 0x8000), WFromLpfl(lpfl), (0x30 | iplrDest));
    FSendPlrMsg2(iplrDest, 0x14e, (lpflNew->id | 0x8000), (0x30 | lpfl->iPlayer), WFromLpfl(lpflNew));
    lpfl->fDead = 0x1;
    goto L_67b6;

L_993b:
    if ((ord.grTask == grTaskLayMines))
        goto L_999e;
    else
        goto L_9949;

L_9949:
    if ((ord.grTask != grTaskNone))
        goto L_67b6;
    else
        goto L_9957;

L_9957:
    if ((lpfl->cord <= 0x1))
        goto L_67b6;
    else
        goto L_9964;

L_9964:
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) != raMines))
        goto L_67b6;
    else
        goto L_9988;

L_9988:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x1]) & 0xf) != 0x6 ? L_67b6 : L_999e */

L_999e:
    idm = idmHasDispersedMines;
    if ((iPass != 0x3))
        goto L_67b6;
    else
        goto L_99ac;

L_99ac:
    if ((lpfl->fHereAllTurn != 0x0))
        goto L_99ea;
    else
        goto L_99c3;

L_99c3:
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) != raMines))
        goto L_67b6;
    else
        goto L_99e4;

L_99e4:

L_99ea:
    cMine = CLayMinesFromLpfl(lpfl, 0xffff, 0xffff);
    if ((LOWORD(cMine) != 0x0))
        goto L_9a46;
    else
        goto L_9a12;

L_9a12:
    if ((HIWORD(cMine) != 0x0))
        goto L_9a46;
    else
        goto L_9a1c;

L_9a1c:
    FSendPlrMsg2(lpfl->iPlayer, 0xbf, (lpfl->id | 0x8000), lpfl->id, 0x0);
    goto CancelOrder;

L_9a46:
    if ((ord.grTask != grTaskLayMines))
        goto L_9aa1;
    else
        goto L_9a54;

L_9a54:
    if ((lpfl->lpplord->rgord[0x0].tsell.iPlrX != 0x0))
        goto L_9a84;
    else
        goto L_9a65;

L_9a65:
    /* untranslated: part[6:2](lpfl->lpplord->rgord[0x0]) = ((part[6:2](lpfl->lpplord->rgord[0x0]) & 0xfff0) | 0x0) */
    goto L_9aa1;

L_9a84:
    if ((lpfl->lpplord->rgord[0x0].tsell.iPlrX == 0x5))
        goto L_9aa1;
    else
        goto L_9a95;

L_9a95:
    lpfl->lpplord->rgord[0x0].tsell.iPlrX = (lpfl->lpplord->rgord[0x0].tsell.iPlrX - 0x1);

L_9aa1:
    j = 0x0;
    goto L_9ea6;

L_9aa9:
    cMine = CLayMinesFromLpfl(lpfl, j, 0xffff);
    if ((LOWORD(cMine) != 0x0))
        goto L_9add;
    else
        goto L_9ad0;

L_9ad0:
    if ((HIWORD(cMine) == 0x0))
        goto L_9ea2;
    else
        goto L_9ad7;

L_9ad7:

L_9add:
    if ((lpfl->fHereAllTurn != 0x0))
        goto L_9b11;
    else
        goto L_9af4;

L_9af4:
    cMine = (int32_t)((cMine / 0x2));

L_9b11:
    lpthBest = 0x0;
    lBest = 0x989680;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_9c86;

L_9b5c:
    if ((lpth->iplr != lpfl->iPlayer))
        goto L_9c81;
    else
        goto L_9b79;

L_9b79:
    if ((lpth->ith != ithMinefield))
        goto L_9c81;
    else
        goto L_9b8d;

L_9b8d:

L_9b93:
    if ((lpth->thm.iType != j))
        goto L_9c81;
    else
        goto L_9ba3;

L_9ba3:

L_9ba9:
    dx = (uint32_t)((lpfl->pt.x - lpth->pt.x));
    dy = (uint32_t)((lpfl->pt.y - lpth->pt.y));
    l = ((uint32_t)((dx * dx)) + (uint32_t)((dy * dy)));
    if ((HIWORD(lpth->thm.cMines) < HIWORD(l)))
        goto L_9c81;
    else
        goto L_9c34;

L_9c34:
    if ((HIWORD(lpth->thm.cMines) > HIWORD(l)))
        goto L_9c42;
    else
        goto L_9c39;

L_9c39:
    if ((LOWORD(lpth->thm) < LOWORD(l)))
        goto L_9c81;
    else
        goto L_9c42;

L_9c42:
    if ((HIWORD(l) > HIWORD(lBest)))
        goto L_9c81;
    else
        goto L_9c53;

L_9c53:
    if ((HIWORD(l) < HIWORD(lBest)))
        goto L_9c61;
    else
        goto L_9c58;

L_9c58:
    if ((LOWORD(l) >= LOWORD(lBest)))
        goto L_9c81;
    else
        goto L_9c61;

L_9c61:
    lBest = l;
    lpthBest = lpth;

L_9c81:
    lpth = (lpth + 0x1);

L_9c86:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_9b5c;
    else
        goto L_9c97;

L_9c97:
    if ((LOWORD(lpthBest) != 0x0))
        goto L_9cab;
    else
        goto L_9ca1;

L_9ca1:
    if ((HIWORD(lpthBest) == 0x0))
        goto L_9dc1;
    else
        goto L_9cab;

L_9cab:
    if ((HIWORD(lpthBest->thm.cMines) > 0xf))
        goto L_9dc1;
    else
        goto L_9cb9;

L_9cb9:
    if ((HIWORD(lpthBest->thm.cMines) < 0xf))
        goto L_9cc9;
    else
        goto L_9cbe;

L_9cbe:
    if ((LOWORD(lpthBest->thm) >= 0x4240))
        goto L_9dc1;
    else
        goto L_9cc9;

L_9cc9:
    /* untranslated: lpthBest->pt.x = loword((int32_t)(words((hiword((uint32_t)(sext16to32(lpthBest->pt.x) * words(HIWORD(lpthBest->thm.cMines),
     * LOWORD(lpthBest->thm)))) + hiword((uint32_t)(sext16to32(lpfl->pt.x) * cMine))), (loword((uint32_t)(sext16to32(lpthBest->pt.x) *
     * words(HIWORD(lpthBest->thm.cMines), LOWORD(lpthBest->thm)))) + loword((uint32_t)(sext16to32(lpfl->pt.x) * cMine)))) / words((HIWORD(cMine) +
     * HIWORD(lpthBest->thm.cMines)), (LOWORD(cMine) + LOWORD(lpthBest->thm))))) */
    /* untranslated: lpthBest->pt.y = loword((int32_t)(words((hiword((uint32_t)(sext16to32(lpthBest->pt.y) * words(HIWORD(lpthBest->thm.cMines),
     * LOWORD(lpthBest->thm)))) + hiword((uint32_t)(sext16to32(lpfl->pt.y) * cMine))), (loword((uint32_t)(sext16to32(lpthBest->pt.y) *
     * words(HIWORD(lpthBest->thm.cMines), LOWORD(lpthBest->thm)))) + loword((uint32_t)(sext16to32(lpfl->pt.y) * cMine)))) / words((HIWORD(cMine) +
     * HIWORD(lpthBest->thm.cMines)), (LOWORD(cMine) + LOWORD(lpthBest->thm))))) */
    LOWORD(lpthBest->thm) = (LOWORD(lpthBest->thm) + LOWORD(cMine));
    HIWORD(lpthBest->thm.cMines) = (HIWORD(lpthBest->thm.cMines) + HIWORD(cMine));
    idm = idmHasIncreasedMinefieldMines;
    goto L_9e50;

L_9dc1:
    lpth = LpthNew(lpfl->iPlayer, ithMinefield);
    if ((LOWORD(lpth) != 0x0))
        goto L_9e1a;
    else
        goto L_9de6;

L_9de6:
    if ((HIWORD(lpth) != 0x0))
        goto L_9e1a;
    else
        goto L_9df0;

L_9df0:
    FSendPlrMsg2(lpfl->iPlayer, 0x17e, (lpfl->id | 0x8000), lpfl->id, 0x0);
    goto L_9ea2;

L_9e1a:
    lpth->pt.x = lpfl->pt.x;
    lpth->pt.y = lpfl->pt.y;
    LOWORD(lpth->thm) = LOWORD(cMine);
    HIWORD(lpth->thm.cMines) = HIWORD(cMine);
    lpth->thm.iType = LOBYTE(j);

L_9e50:
    FSendPlrMsg(lpfl->iPlayer, idm, (lpfl->id | 0x8000), lpfl->id, LOWORD(cMine), (LOWORD((uint32_t)((cMine >> 0x10))) & 0xffff), 0x0, 0x0, 0x0, 0x0);

L_9ea2:
    j = (j + 0x1);

L_9ea6:
    if ((j >= 0x3))
        goto L_67b6;
    else
        goto L_9eac;

L_9eac:

L_9eb2:
    return;
}
