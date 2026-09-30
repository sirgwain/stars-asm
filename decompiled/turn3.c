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

    if (cFleet > 0) {
        for (ifltcur = 0; ifltcur < cFleet; ifltcur++) {
            lpfl = rglpfl[ifltcur];
            if (rglpfl[ifltcur] == 0)
                break;
            if (iPass == 1) {
                lpfl->fCompChg = 0;
                lpfl->fTargeted = 0;
            }
            if (lpfl->fDead == 0 && lpfl->fSkipped == 0) {
                ord = lpfl->lpplord->rgord[0];
                if (ord.grTask == grTaskXfer) {
                    fFulfilled = 1;
                    fDone = 1;
                    fSentBadFleetXfer = 0;
                    fHasPermission = 1;
                    fMining = 0;
                    fFueling = 0;
                    fStealing = 0;
                    fDunnage = 0;
                    lpflWP = NULL;
                    lpthWP = NULL;
                    fAtPlanet = lpfl->idPlanet != -1 && FLookupPlanet(lpfl->idPlanet, &pl) != 0;
                    idWP = ord.id;
                    switch (ord.grobj) {
                    case grobjFleet:
                        lpflWP = LpflFromId(ord.id);
                        idWP |= 0x8000;
                        xWP = 0xffff;
                        fHasPermission = lpfl->iPlayer == lpflWP->iPlayer ? 1 : 0;
                        if (fHasPermission == 0) {
                            GetFleetScannerRange(lpfl, NULL, NULL, &fHasPermission);
                            if (fHasPermission == 0)
                                break;
                            fStealing = 1;
                            break;
                        }
                        if (lpflWP->fHereAllTurn == 0 || fAtPlanet == 0)
                            break;
                        if (CMineFromLpfl(lpflWP) > 0 && (pl.iPlayer == -1 || pl.iPlayer == lpfl->iPlayer)) {
                            fMining = 1;
                            xWP = 0xffff;
                            idWP = lpflWP->id | 0x8000;
                            break;
                        }
                        if (LGetFleetStat(lpflWP, 2) != 0 || pl.iPlayer != lpfl->iPlayer)
                            break;
                        fFueling = 1;
                        xWP = 0xffff;
                        idWP = lpflWP->id | 0x8000;
                        break;
                    case grobjThing:
                        lpthWP = LpthFromId(ord.id);
                        xWP = 0xfffe;
                        break;
                    case grobjPlanet:
                        xWP = 0xffff;
                        fHasPermission = lpfl->iPlayer == pl.iPlayer ? 3 : 0;
                        if (fHasPermission != 0)
                            break;
                        GetFleetScannerRange(lpfl, NULL, NULL, &fHasPermission);
                        if (fHasPermission == 1) {
                            fHasPermission = 0;
                        }
                        if (fHasPermission != 0) {
                            fStealing = 1;
                        }
                        if (fHasPermission != 0 || pl.iPlayer != -1)
                            break;
                        iflWP = 0;
                        while (1) {
                            if (iflWP >= cFleet)
                                goto L_6c14;
                            lpflWP = rglpfl[iflWP];
                            if (rglpfl[iflWP] == 0)
                                goto L_6c14;
                            if (lpflWP->fDead == 0 && lpflWP->iPlayer == lpfl->iPlayer && lpfl->idPlanet == lpflWP->idPlanet && lpflWP != lpfl &&
                                lpflWP->fHereAllTurn != 0 && CMineFromLpfl(lpflWP) > 0)
                                break;
                            iflWP++;
                        }
                        fMining = 2;
                        fHasPermission = 1;
                        ord.grobj = grobjFleet;
                        ord.id = lpflWP->id;
                        xWP = 0xffff;
                        idWP = lpflWP->id | 0x8000;
                        break;
                    case grobjOther:
                        xWP = ord.pt.x;
                        idWP = ord.pt.y;
                    }
                L_6c14:
                    iLoad = iPass - 1 & 1;
                    fOptFuel = 0;
                    if (ord.grobj == grobjThing && (lpthWP == 0 || lpthWP->ith != ithMineralPacket)) {
                        if (lpthWP != 0) {
                            FSendPlrMsg2(lpfl->iPlayer, 286, lpfl->id | 0x8000, lpfl->id, lpthWP->ith);
                        }
                    } else {
                        while (1) {
                            for (j = 0; j < 5; j++) {
                                action = ord.txp.rgia[j].iAction;
                                if (action != iActionNone && (fDunnage != 2 || action == iActionLoadDunnage) &&
                                    (j != 4 || ord.grobj == grobjFleet || ord.grobj == grobjOther) && (j < 3 || ord.grobj != grobjThing)) {
                                    amountWP = 0;
                                    switch (ord.grobj) {
                                    case grobjFleet:
                                        if ((fMining == 0 && fFueling == 0) || j == 4) {
                                            amountWP = lpflWP->rgwtMin[j];
                                            break;
                                        }
                                    case grobjPlanet:
                                        if (j >= 4)
                                            break;
                                        amountWP = pl.rgwtMin[j];
                                        break;
                                    case grobjThing:
                                        if (j < 3) {
                                            amountWP = lpthWP->thp.rgwtMin[j];
                                        }
                                    }
                                    if (action - 1 <= 8) {
                                        switch (action) {
                                        case 5:
                                        case 6:
                                            if (iLoad == 0)
                                                continue;
                                            if (j == 4) {
                                                amount = LGetFleetStat(lpfl, 1);
                                            } else {
                                                amount = LGetFleetStat(lpfl, 2);
                                            }
                                            amount = 2000000 < amount ? 2000000 : amount;
                                            if (amount < 65536) {
                                                amountEdit = (uint32_t)((uint32_t)(amount * ord.txp.rgia[j].cQuan) / 100);
                                            } else {
                                                amountEdit = (uint32_t)((int32_t)(amount / 100) * ord.txp.rgia[j].cQuan);
                                            }
                                            amountEdit -= lpfl->rgwtMin[j];
                                            if (amountEdit >= 0)
                                                goto L_7b4e;
                                            amountEdit = 0;
                                            goto L_7b4e;
                                        case 3:
                                            if (iLoad == 0)
                                                continue;
                                            amountEdit = ord.txp.rgia[j].cQuan;
                                            goto L_7b4e;
                                        case 4:
                                            if (iLoad != 0)
                                                continue;
                                        case 8:
                                        case 9:
                                            amountEdit = ord.txp.rgia[j].cQuan;
                                            goto L_7b4e;
                                        case 1:
                                            if (iLoad == 0)
                                                continue;
                                        case 7:
                                            amountEdit = amountWP;
                                            goto L_7b4e;
                                        case 2:
                                            if (iLoad != 0)
                                                continue;
                                            amountEdit = lpfl->rgwtMin[j];
                                            goto L_7b4e;
                                        }
                                        goto L_67b6;
                                    }
                                L_7b4e:
                                    if (action - 1 <= 8) {
                                        switch (action) {
                                        case 9:
                                            amount = amountWP - amountEdit;
                                            if (amount >= 0) {
                                                if (iLoad == 0)
                                                    continue;
                                                goto Load;
                                            }
                                            if (iLoad != 0)
                                                continue;
                                            amount = -amount;
                                            amount = amount < lpfl->rgwtMin[j] ? amount : lpfl->rgwtMin[j];
                                            goto Unload;
                                        case 8:
                                            amount = amountEdit - lpfl->rgwtMin[j];
                                            if (amount >= 0) {
                                                if (iLoad == 0)
                                                    continue;
                                                if (amount <= amountWP)
                                                    goto Load;
                                                fDone = 0;
                                                if (iPass != 4)
                                                    goto Load;
                                                idm = j == 3 ? idmAttemptedSetNumberBoardUnfortunatelyCouldntProvi
                                                             : idmAttemptedSetAmountBoardUnfortunatelyCouldntProvi;
                                                FSendPlrMsg(lpfl->iPlayer, idm, lpfl->id | 0x8000, lpfl->id, j, ord.txp.rgia[j].cQuan, 0, xWP, idWP, 0);
                                                goto Load;
                                            }
                                            if (iLoad != 0)
                                                continue;
                                            amount = -amount;
                                            goto Unload;
                                        case 7:
                                            if (j == 4) {
                                                wtOptimalFuel = 0;
                                                fOptFuel = 1;
                                                continue;
                                            }
                                            if (iLoad == 0)
                                                continue;
                                            if (fDunnage < 2) {
                                                fDunnage = 1;
                                                continue;
                                            }
                                            amount = amountWP;
                                            if (amount != 0)
                                                goto Load;
                                            continue;
                                        case 1:
                                        case 3:
                                        case 5:
                                        case 6:
                                            amount = amountEdit;
                                            goto Load;
                                        case 2:
                                        case 4:
                                            amount = (uint32_t)lpfl->rgwtMin[j] < (uint32_t)amountEdit ? lpfl->rgwtMin[j] : amountEdit;
                                            goto Unload;
                                        }
                                        goto L_67b6;
                                    Unload:
                                        if (iLoad != 0)
                                            continue;
                                        if (j == 3 && ord.grobj == grobjPlanet && pl.iPlayer != lpfl->iPlayer) {
                                            if (pl.iPlayer == -1 && pl.fWasInhabited == 0) {
                                                idm = idmHasTriedBeamColonistsPlanetUninhabitedMust;
                                                goto LCantDrop;
                                            }
                                            if (GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) == raMacintosh) {
                                                idm = idmCaptainHasAttemptedBeamColonistsOverruledBridge;
                                                goto LCantDrop;
                                            }
                                            if (pl.fStarbase != 0) {
                                                idm = idmHasTriedBeamColonistsPlanetsStarbaseWould;
                                                goto LCantDrop;
                                            }
                                            FQueueColonistDrop(lpfl, &pl, amount);
                                        } else {
                                            if (j == 3 && ord.grobj == grobjFleet && lpfl->iPlayer != ((uint16_t)ord.id >> 9 & 0xf))
                                                goto L_78e4;
                                            if (ord.grobj == grobjFleet && rgplr[lpflWP->iPlayer].rgmdRelation[lpfl->iPlayer] == 2) {
                                                amount = 0;
                                            } else {
                                                if (j == 3 && ord.grobj == grobjOther) {
                                                    FSendPlrMsg2(lpfl->iPlayer, 357, lpfl->id | 0x8000, lpfl->id, 0);
                                                    goto CancelOrder;
                                                }
                                                if (amount != 0) {
                                                    if (fFueling != 0 && j != 4) {
                                                        l = ChgCargo(grobjPlanet, pl.id, j, amount, NULL);
                                                        FSendPlrMsg(lpfl->iPlayer, j == 3 ? 46 : 45, lpfl->id | 0x8000, lpfl->id, LOWORD(l), HIWORD(l), j, xWP,
                                                                    pl.id, 0);
                                                    } else {
                                                        l = ChgCargo(ord.grobj, ord.id, j, amount, NULL);
                                                        if (l > 0) {
                                                            FSendPlrMsg(lpfl->iPlayer, j == 3 ? 46 : 45, lpfl->id | 0x8000, lpfl->id, LOWORD(l), HIWORD(l), j,
                                                                        xWP, idWP, 0);
                                                        }
                                                        amount = l;
                                                    }
                                                }
                                            }
                                        }
                                        if (amount != 0) {
                                            l = ChgCargo(grobjFleet, lpfl->id, j, -amount, NULL);
                                        }
                                        ord.txp.rgia[j].iAction = iActionNone;
                                        continue;
                                    Load:
                                        if (iLoad != 0 && amount != 0) {
                                            if (j == 4) {
                                                l = GetFuelFree(lpfl);
                                            } else {
                                                l = GetCargoFree(lpfl);
                                            }
                                            amount = l < amount ? l : amount;
                                            if (fHasPermission == 0 || ord.grobj == grobjOther) {
                                                if (j == 4 && fOptFuel != 0) {
                                                    amount = 0;
                                                } else {
                                                    if (iPass == 4)
                                                        goto L_733a;
                                                    fDone = 0;
                                                }
                                            } else {
                                                if (fStealing != 0 && (j == 3 || j == 4)) {
                                                    fDone = 1;
                                                }
                                                if (amount != 0) {
                                                    l = amount < amountWP ? amount : amountWP;
                                                    l2 = ChgCargo(ord.grobj, ord.id, j, -l, NULL);
                                                    if (l2 != 0) {
                                                        l = ChgCargo(grobjFleet, lpfl->id, j, -l2, NULL);
                                                        if (l != 0) {
                                                            if (ord.grobj == grobjFleet && lpfl->iPlayer != lpflWP->iPlayer) {
                                                                FSendPlrMsg(lpfl->iPlayer, 281, lpfl->id | 0x8000, lpfl->id, LOWORD(l), HIWORD(l), j,
                                                                            idWP & 0x7fff, 0, 0);
                                                            } else {
                                                                FSendPlrMsg(lpfl->iPlayer, j == 3 ? 44 : 43, lpfl->id | 0x8000, lpfl->id, LOWORD(l), HIWORD(l),
                                                                            j, xWP, idWP, 0);
                                                            }
                                                        }
                                                    }
                                                    if ((fFueling != 0 || fMining != 0) && amount != -l2) {
                                                        l = amount + l2;
                                                        l2 = ChgCargo(grobjPlanet, pl.id, j, -l, NULL);
                                                        if (l2 != 0) {
                                                            l = ChgCargo(grobjFleet, lpfl->id, j, -l2, NULL);
                                                            if (fMining != 0) {
                                                                FSendPlrMsg(lpfl->iPlayer, 125, lpfl->id | 0x8000, lpfl->id, LOWORD(l), HIWORD(l), j,
                                                                            lpflWP->id, pl.id, 0);
                                                            } else {
                                                                FSendPlrMsg(lpfl->iPlayer, j == 3 ? 44 : 43, lpfl->id | 0x8000, lpfl->id, LOWORD(l), HIWORD(l),
                                                                            j, xWP, pl.id, 0);
                                                            }
                                                        } else {
                                                            l = 0;
                                                        }
                                                    }
                                                    if (amount != l && action == iActionWaitPercent) {
                                                        fFulfilled = 0;
                                                    }
                                                    if (l != 0 && action == iActionLoadDunnage && j == 4) {
                                                        wtOptimalFuel = l;
                                                    }
                                                } else if (action == iActionWaitPercent) {
                                                    if (j == 4) {
                                                        fDone = 0;
                                                    } else {
                                                        fFulfilled = 0;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            if (fOptFuel != 0 && iLoad != 0 && fDunnage != 1) {
                                if (lpfl->cord <= 1) {
                                    amount = 0;
                                    iExcess = lpfl->rgwtMin[4];
                                } else {
                                    amount = EstFuelUse(lpfl, 0, -1, -1, 0);
                                    if (amount > lpfl->rgwtMin[4]) {
                                        fDone = 0;
                                        if (wtOptimalFuel != 0) {
                                            FSendPlrMsg(lpfl->iPlayer, 43, lpfl->id | 0x8000, lpfl->id, LOWORD(wtOptimalFuel), HIWORD(wtOptimalFuel), 4, xWP,
                                                        idWP, 0);
                                        }
                                        if (iPass == 4 && fHasPermission == 0) {
                                            FSendPlrMsg(lpfl->iPlayer, 294, lpfl->id | 0x8000, lpfl->id, xWP, idWP, 0, 0, 0, 0);
                                            goto FinishFleet;
                                        }
                                        if (iPass != 2)
                                            goto FinishFleet;
                                        lMaxFuel = LGetFleetStat(lpfl, 1);
                                        if (lMaxFuel < amount) {
                                            FSendPlrMsg(lpfl->iPlayer, 61, lpfl->id | 0x8000, lpfl->id, LOWORD(lMaxFuel), HIWORD(lMaxFuel), LOWORD(amount),
                                                        HIWORD(amount), 0, 0);
                                            goto FinishFleet;
                                        }
                                        cFuel2 = amount - lpfl->rgwtMin[4];
                                        if (fMining == 2) {
                                            idWP = pl.id;
                                        }
                                        FSendPlrMsg(lpfl->iPlayer, 60, lpfl->id | 0x8000, xWP, idWP, lpfl->id, LOWORD(cFuel2), HIWORD(cFuel2), 0, 0);
                                        goto FinishFleet;
                                    }
                                    if (amount >= lpfl->rgwtMin[4])
                                        goto L_804f;
                                    wtFuelOrig = lpfl->rgwtMin[4];
                                    do {
                                        lpfl->rgwtMin[4] = amount;
                                        amount = EstFuelUse(lpfl, 0, -1, -1, 0);
                                    } while (amount < lpfl->rgwtMin[4]);
                                    iExcess = wtFuelOrig - amount;
                                    lpfl->rgwtMin[4] = wtFuelOrig;
                                }
                                if (iExcess != 0) {
                                    l2 = ChgCargo(ord.grobj, ord.id, 4, iExcess, NULL);
                                    if (l2 != 0) {
                                        l = ChgCargo(grobjFleet, lpfl->id, 4, -l2, NULL);
                                    }
                                } else {
                                    l = 0;
                                }
                                if (l2 != 0) {
                                    l += wtOptimalFuel;
                                    if (l > 0) {
                                        FSendPlrMsg(lpfl->iPlayer, 43, lpfl->id | 0x8000, lpfl->id, LOWORD(l), HIWORD(l), 4, xWP, idWP, 0);
                                    } else if (l < 0) {
                                        FSendPlrMsg(lpfl->iPlayer, 45, lpfl->id | 0x8000, lpfl->id, -LOWORD(l), LOWORD((uint32_t)((uint32_t)-l >> 0x10)), 4,
                                                    xWP, idWP, 0);
                                    }
                                }
                            L_804f:
                                if (fDone != 0 && fFulfilled != 0) {
                                    ord.txp.rgia[4].iAction = iActionNone;
                                    ord.txp.rgia[4].cQuan = 0;
                                }
                            }
                        FinishFleet:
                            if (fFulfilled == 0 && GetCargoFree(lpfl) > 0) {
                                fDone = 0;
                            }
                            if (fDone == 0 || fDunnage != 1 || (GetCargoFree(lpfl) <= 0 && fOptFuel == 0))
                                break;
                            fDunnage = 2;
                        }
                        if (fDone != 0 && iLoad != 0)
                            goto CancelOrder;
                        if (fMining == 2) {
                            ord.id = pl.id;
                            ord.grobj = grobjPlanet;
                        }
                        lpfl->lpplord->rgord[0] = ord;
                        goto L_67b6;
                    L_78e4:
                        idm = idmAllowedTransferColonistsAnotherPlayer;
                    LCantDrop:
                        FSendPlrMsg2(lpfl->iPlayer, idm, lpfl->id | 0x8000, lpfl->id, pl.id);
                        goto CancelOrder;
                    L_733a:
                        switch (ord.grobj) {
                        case grobjPlanet:
                            idm = idmAttemptedLoadPlanetDontControlOrderHas;
                            break;
                        case grobjFleet:
                            idm = idmAttemptedLoadFleetDontControlOrderHas;
                            break;
                        case grobjOther:
                            idm = idmAttemptedLoadDeepSpaceAttemptUnsuccessful;
                        }
                        FSendPlrMsg2(lpfl->iPlayer, idm, lpfl->id | 0x8000, lpfl->id, j);
                    }
                } else {
                    if (ord.grTask != grTaskScrap || iPass != 1) {
                        switch (ord.grTask) {
                        default:
                            break;
                        case grTaskColonize:
                            goto L_817a;
                        case grTaskMine:
                            cMine = 0;
                            if (iPass != 3 || lpfl->fHereAllTurn == 0)
                                break;
                            if (lpfl->idPlanet == -1) {
                                FSendPlrMsg2(lpfl->iPlayer, 119, lpfl->id | 0x8000, lpfl->id, 0);
                                goto CancelOrder;
                            }
                            lppl = LpplFromId(lpfl->idPlanet);
                            if (lppl == 0)
                                goto CancelOrder;
                            cMine = CMineFromLpfl(lpfl);
                            if (cMine == 0) {
                                FSendPlrMsg2(lpfl->iPlayer, 117, lpfl->id | 0x8000, lpfl->id, lppl->id);
                                goto CancelOrder;
                            }
                            if (lppl->iPlayer != -1) {
                                if (GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) == raMacintosh)
                                    break;
                                FSendPlrMsg2(lpfl->iPlayer, 118, lpfl->id | 0x8000, lpfl->id, lppl->id);
                                goto CancelOrder;
                            }
                            EstMineralsMined(lppl, rglQuan, cMine, 1);
                            break;
                        case grTaskAutoRoute:
                            if (lpfl->cord != 1 || lpfl->idPlanet == -1)
                                break;
                            lppl = LpplFromId(lpfl->idPlanet);
                            if (lppl->iPlayer == lpfl->iPlayer && lppl->idRoute != 0) {
                                AutoRouteFleet(lpfl, lppl);
                                FSendPlrMsg(lpfl->iPlayer, lpfl->lpplord->rgord[1].iWarp == 0 ? 296 : 295, lpfl->id | 0x8000, lpfl->id, lpfl->idPlanet,
                                            lppl->idRoute - 1, 0, 0, 0, 0);
                                break;
                            }
                            if (iPass != 4)
                                break;
                            AutoFleetOrder(lpfl, lppl);
                            ord = lpfl->lpplord->rgord[0];
                            if (ord.grTask == grTaskMerge)
                                goto LDoMerge;
                            break;
                        case grTaskMerge:
                            if ((iPass & 1) != 0)
                                break;
                            goto LDoMerge;
                        case grTaskGive:
                            if (iPass != 4)
                                break;
                            iplrDest = ord.tsell.iPlrX;
                            if (iplrDest >= lpfl->iPlayer) {
                                iplrDest++;
                            }
                            if (iplrDest < 0 || iplrDest >= game.cPlayer || rgplr[iplrDest].fDead != 0) {
                                FSendPlrMsg2(lpfl->iPlayer, 328, lpfl->id | 0x8000, lpfl->id, 0);
                                goto CancelOrder;
                            }
                            if (rgplr[iplrDest].fAi != 0 || rgplr[iplrDest].rgmdRelation[lpfl->iPlayer] == 2) {
                                FSendPlrMsg2(lpfl->iPlayer, 332, lpfl->id | 0x8000, 0x30 | iplrDest, 0);
                                goto CancelOrder;
                            }
                            if (lpfl->rgwtMin[3] > 0) {
                                FSendPlrMsg2(lpfl->iPlayer, 329, lpfl->id | 0x8000, lpfl->id, 0);
                                goto CancelOrder;
                            }
                            ishLastFree = -1;
                            for (ish = 0; ish < 16; ish++) {
                                if (lpfl->rgcsh[ish] == 0) {
                                    rgishMap[ish] = -1;
                                } else {
                                    ishMatch = IshFindSimilarDesign(&rglpshdef[lpfl->iPlayer][ish].hul, iplrDest);
                                    if (ishMatch != -1) {
                                        rgishMap[ish] = ishMatch;
                                    } else {
                                        do {
                                            ishLastFree++;
                                        } while (ishLastFree < 16 && rglpshdef[iplrDest][ishLastFree].fFree == 0);
                                        if (ishLastFree >= 16)
                                            goto SellNoCap;
                                        rgishMap[ish] = ishLastFree;
                                    }
                                }
                            }
                            if (rgplr[iplrDest].cFleet < 0x200) {
                                lpflNew = LpflNew(iplrDest, lpfl->idPlanet);
                                if (lpflNew != 0) {
                                    if (iplrDest < lpfl->iPlayer) {
                                        ifltcur++;
                                    }
                                    lpflNew->pt = lpfl->pt;
                                    lpflNew->lpplord->rgord[0].pt = lpfl->pt;
                                    for (ish = 0; ish <= 4; ish++) {
                                        lpflNew->rgwtMin[ish] = lpfl->rgwtMin[ish];
                                    }
                                    for (ish = 0; ish < 16; ish++) {
                                        if (lpfl->rgcsh[ish] > 0) {
                                            lpshdefDest = rglpshdef[iplrDest] + rgishMap[ish];
                                            if (lpshdefDest->fFree != 0) {
                                                *lpshdefDest = rglpshdef[lpfl->iPlayer][ish];
                                                lpshdefDest->ishdef = rgishMap[ish];
                                                lpshdefDest->fGift = 1;
                                                lpshdefDest->cBuilt = 0;
                                                lpshdefDest->cExist = 0;
                                                rgplr[iplrDest].cShDef++;
                                            }
                                            lpflNew->rgcsh[rgishMap[ish]] = lpflNew->rgcsh[rgishMap[ish]] + lpfl->rgcsh[ish];
                                            lpflNew->rgdv[rgishMap[ish]].dp = lpfl->rgdv[ish].dp;
                                            lpshdefDest->cBuilt += lpfl->rgcsh[ish];
                                            lpshdefDest->cExist += lpfl->rgcsh[ish];
                                        }
                                    }
                                    lpfl->fDead = 1;
                                    FSendPlrMsg2(lpfl->iPlayer, 333, lpflNew->id | 0x8000, WFromLpfl(lpfl), 0x30 | iplrDest);
                                    FSendPlrMsg2(iplrDest, 334, lpflNew->id | 0x8000, 0x30 | lpfl->iPlayer, WFromLpfl(lpflNew));
                                    lpfl->fDead = 1;
                                    break;
                                }
                            }
                        SellNoCap:
                            FSendPlrMsg2(lpfl->iPlayer, 330, lpfl->id | 0x8000, lpfl->id, 0x30 | iplrDest);
                            FSendPlrMsg2(iplrDest, 331, -1, 0x30 | lpfl->iPlayer, 0);
                            goto CancelOrder;
                        case grTaskNone:
                            if (lpfl->cord <= 1 || GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) != raMines ||
                                lpfl->lpplord->rgord[1].grTask != grTaskLayMines)
                                break;
                        case grTaskLayMines:
                            idm = idmHasDispersedMines;
                            if (iPass == 3 && (lpfl->fHereAllTurn != 0 || GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) == raMines)) {
                                cMine = CLayMinesFromLpfl(lpfl, -1, -1);
                                if (cMine == 0) {
                                    FSendPlrMsg2(lpfl->iPlayer, 191, lpfl->id | 0x8000, lpfl->id, 0);
                                    goto CancelOrder;
                                }
                                if (ord.grTask == grTaskLayMines) {
                                    if (lpfl->lpplord->rgord[0].tsell.iPlrX == 0) {
                                        lpfl->lpplord->rgord[0].grTask = grTaskNone;
                                    } else if (lpfl->lpplord->rgord[0].tsell.iPlrX != 5) {
                                        lpfl->lpplord->rgord[0].tsell.iPlrX--;
                                    }
                                }
                                for (j = 0; j < 3; j++) {
                                    cMine = CLayMinesFromLpfl(lpfl, j, -1);
                                    if (cMine != 0) {
                                        if (lpfl->fHereAllTurn == 0) {
                                            cMine = (int32_t)(cMine / 2);
                                        }
                                        lpthBest = NULL;
                                        lBest = 10000000;
                                        lpth = lpThings;
                                        lpthMac = lpThings + cThing;
                                        for (; lpth < lpthMac; lpth++) {
                                            if (lpth->iplr == lpfl->iPlayer && lpth->ith == ithMinefield && lpth->thm.iType == j) {
                                                dx = (int16_t)(lpfl->pt.x - lpth->pt.x);
                                                dy = (int16_t)(lpfl->pt.y - lpth->pt.y);
                                                l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                                if (lpth->thm.cMines >= l && l < lBest) {
                                                    lBest = l;
                                                    lpthBest = lpth;
                                                }
                                            }
                                        }
                                        if (lpthBest != 0 && lpthBest->thm.cMines < 1000000) {
                                            lpthBest->pt.x =
                                                LOWORD((int32_t)((int32_t)((uint32_t)(lpthBest->pt.x * lpthBest->thm.cMines) + (uint32_t)(lpfl->pt.x * cMine)) /
                                                                 (cMine + lpthBest->thm.cMines)));
                                            lpthBest->pt.y =
                                                LOWORD((int32_t)((int32_t)((uint32_t)(lpthBest->pt.y * lpthBest->thm.cMines) + (uint32_t)(lpfl->pt.y * cMine)) /
                                                                 (cMine + lpthBest->thm.cMines)));
                                            lpthBest->thm.cMines += cMine;
                                            idm = idmHasIncreasedMinefieldMines;
                                        } else {
                                            lpth = LpthNew(lpfl->iPlayer, ithMinefield);
                                            if (lpth == 0) {
                                                FSendPlrMsg2(lpfl->iPlayer, 382, lpfl->id | 0x8000, lpfl->id, 0);
                                                continue;
                                            }
                                            lpth->pt = lpfl->pt;
                                            lpth->thm.cMines = cMine;
                                            lpth->thm.iType = LOBYTE(j);
                                        }
                                        FSendPlrMsg(lpfl->iPlayer, idm, lpfl->id | 0x8000, lpfl->id, LOWORD(cMine), HIWORD(cMine), 0, 0, 0, 0);
                                    }
                                }
                            }
                        }
                        goto L_67b6;
                    LDoMerge:
                        if (ord.grobj == grobjFleet) {
                            lpflDest = LpflFromId(ord.id);
                            if (lpflDest != 0 && lpflDest->fDead == 0) {
                                if (lpfl == lpflDest)
                                    goto CancelOrder;
                                if (lpflDest->iPlayer != lpfl->iPlayer) {
                                    FSendPlrMsg2(lpfl->iPlayer, 246, lpfl->id | 0x8000, lpfl->id, 0);
                                    goto CancelOrder;
                                }
                                FSendPlrMsg2(lpfl->iPlayer, 247, lpflDest->id | 0x8000, WFromLpfl(lpfl), lpflDest->id);
                                FRemovePlayerMessage(lpfl->iPlayer, 78, lpfl->id | 0x8000);
                                Merge2Fleets(lpflDest, lpfl, 1);
                                goto CancelOrder;
                            }
                        }
                        FSendPlrMsg2(lpfl->iPlayer, 245, lpfl->id | 0x8000, lpfl->id, 0);
                        goto CancelOrder;
                    }
                L_817a:
                    if (lpfl->idPlanet == -1) {
                        if (ord.grTask == grTaskColonize) {
                            FSendPlrMsg2(lpfl->iPlayer, 81, lpfl->id | 0x8000, lpfl->id, 0);
                            goto CancelOrder;
                        }
                        pl.id = -1;
                        pl.iPlayer = -1;
                        pl.fStarbase = 0;
                    } else if (FLookupPlanet(lpfl->idPlanet, &pl) == 0 && ord.grTask == grTaskColonize) {
                        goto CancelOrder;
                    }
                    if (ord.grTask == grTaskColonize) {
                        if (pl.iPlayer != -1) {
                            FSendPlrMsg(lpfl->iPlayer, 82, lpfl->id | 0x8000, lpfl->id, pl.id, pl.id, 0, 0, 0, 0);
                            goto CancelOrder;
                        }
                        if (lpfl->rgwtMin[3] == 0) {
                            FSendPlrMsg2(lpfl->iPlayer, 83, lpfl->id | 0x8000, lpfl->id, pl.id);
                            goto CancelOrder;
                        }
                    }
                    for (i = 0; i < 3; i++) {
                        rgwt[i] = 0;
                    }
                    fColonize = 0;
                    csh = 0;
                    memset(rgTechBattle, 0, 6);
                    memset(rgTechTrader, 0, 13);
                    for (i = 0; i < 16; i++) {
                        if (lpfl->rgcsh[i] > 0) {
                            csh += lpfl->rgcsh[i];
                            MarkTechsSeen(&rglpshdef[lpfl->iPlayer][i].hul, lpfl->iPlayer);
                            for (j = 0; j < rglpshdef[lpfl->iPlayer][i].hul.chs; j++) {
                                if (rglpshdef[lpfl->iPlayer][i].hul.rghs[j].grhst == hstSpecialM &&
                                    (rglpshdef[lpfl->iPlayer][i].hul.rghs[j].iItem == 0 || rglpshdef[lpfl->iPlayer][i].hul.rghs[j].iItem == 1)) {
                                    fColonize = 1;
                                }
                            }
                        }
                    }
                    if (ord.grTask == grTaskColonize && fColonize == 0) {
                        FSendPlrMsg(lpfl->iPlayer, 84, lpfl->id | 0x8000, lpfl->id, pl.id, lpfl->id, 0, 0, 0, 0);
                    } else {
                        lXferMinerals = 0;
                        fUltimate = ord.grTask == grTaskScrap && pl.iPlayer != -1 && GetRaceGrbit(&rgplr[pl.iPlayer], 5) != 0;
                        fBleeding = GetRaceGrbit(&rgplr[lpfl->iPlayer], ibitRaceBleedingEdgeTech);
                        gd.fDontCalcBleed = 1;
                        idPlayer = lpfl->iPlayer;
                        for (i = 0; i <= 2; i++) {
                            lAmt = 0;
                            for (j = 0; j < 16; j++) {
                                if (lpfl->rgcsh[j] > 0) {
                                    if (fBleeding != 0) {
                                        shdefT = rglpshdef[lpfl->iPlayer][j];
                                        UpdateShdefCost(&shdefT);
                                        lpshdefT = &shdefT;
                                    } else {
                                        lpshdefT = rglpshdef[lpfl->iPlayer] + j;
                                    }
                                    lT = (uint32_t)(lpfl->rgcsh[j] * (uint32_t)lpshdefT->hul.rgwtOreCost[i]);
                                    if (lpshdefT->fGift != 0) {
                                        lT = (int32_t)(lT / 4);
                                    }
                                    lAmt += lT;
                                }
                            }
                            if (ord.grTask == grTaskColonize) {
                                lAmt = (int32_t)(lAmt * 3) / 4;
                            } else if (pl.id == -1) {
                                lAmt = (int32_t)(lAmt / 3);
                            } else if (pl.fStarbase != 0) {
                                if (fUltimate != 0) {
                                    lAmt = (int32_t)(lAmt * 9) / 10;
                                } else {
                                    lAmt = (int32_t)(lAmt * 4) / 5;
                                }
                            } else if (fUltimate != 0) {
                                lAmt = (int32_t)(lAmt * 9) / 20;
                            } else {
                                lAmt = (int32_t)(lAmt / 3);
                            }
                            lAmt += lpfl->rgwtMin[i];
                            lXferMinerals += lAmt;
                            if (pl.id == -1) {
                                pl.rgwtMin[i] = lAmt;
                            } else {
                                lpPlanets[pl.id].rgwtMin[i] = lpPlanets[pl.id].rgwtMin[i] + lAmt;
                            }
                        }
                        iGoto = pl.id;
                        if (fUltimate != 0) {
                            lResUltimate = 0;
                            for (j = 0; j < 16; j++) {
                                if (lpfl->rgcsh[j] > 0) {
                                    if (fBleeding != 0) {
                                        shdefT = rglpshdef[lpfl->iPlayer][j];
                                        UpdateShdefCost(&shdefT);
                                        lpshdefT = &shdefT;
                                    } else {
                                        lpshdefT = rglpshdef[lpfl->iPlayer] + j;
                                    }
                                    lT = (uint32_t)(lpfl->rgcsh[j] * (uint32_t)lpshdefT->hul.resCost);
                                    if (lpshdefT->fGift != 0) {
                                        lT = (int32_t)(lT / 4);
                                    }
                                    lResUltimate += lT;
                                }
                            }
                            if (lResUltimate > 65535) {
                                lResUltimate = 65535;
                            }
                            vrgPlanResExtra[pl.id] = vrgPlanResExtra[pl.id] + LOWORD(lResUltimate);
                            if (lResUltimate > 0 && pl.iPlayer != -1) {
                                lAmt = CResourcesAtPlanet(&pl, pl.iPlayer);
                                lResUltimate = (int32_t)((int32_t)(lResUltimate * lAmt) / (lResUltimate + lAmt));
                            }
                            idm = pl.fStarbase + 92;
                            FSendPlrMsg(lpfl->iPlayer, idm, iGoto, WFromLpfl(lpfl), LOWORD(lXferMinerals), HIWORD(lXferMinerals), pl.id, LOWORD(lResUltimate),
                                        HIWORD(lResUltimate), 0);
                            idm = pl.fStarbase + 322;
                            if (pl.fStarbase != 0) {
                                i = ITechLearnATech(pl.iPlayer, 0, 0, 0xffff, &iGoto);
                                if (i != 0) {
                                    idm = idmHasDismantledKtMineralsWhichHaveDeposited3;
                                    if (i < 0) {
                                        i = -(i + 1);
                                    } else {
                                        i--;
                                        idm++;
                                    }
                                } else {
                                    iGoto = pl.id;
                                }
                            } else {
                                iGoto = pl.id;
                            }
                            FSendPlrMsg(pl.iPlayer, idm, iGoto, lpfl->id, LOWORD(lXferMinerals), HIWORD(lXferMinerals), pl.id, LOWORD(lResUltimate),
                                        HIWORD(lResUltimate), i);
                        } else if (pl.id == -1) {
                            lpthWP = NULL;
                            DropSalvage(&lpthWP, pl.rgwtMin, lpfl->iplr, &ord.pt);
                            FSendPlrMsg2(lpfl->iPlayer, 91, -6, lpthWP->idFull, WFromLpfl(lpfl));
                        } else {
                            idm = pl.fStarbase + 89;
                            FSendPlrMsg(lpfl->iPlayer, idm, iGoto, WFromLpfl(lpfl), LOWORD(lXferMinerals), HIWORD(lXferMinerals), pl.id, 0, 0, 0);
                            idm = pl.fStarbase + 320;
                            if (pl.fStarbase != 0) {
                                i = ITechLearnATech(pl.iPlayer, 0, 0, 0xffff, &iGoto);
                                if (i != 0) {
                                    idm = idmHasDismantledKtMineralsStarbaseOrbitingProcess;
                                    if (i < 0) {
                                        i = -(i + 1);
                                    } else {
                                        i--;
                                        idm++;
                                    }
                                } else {
                                    iGoto = pl.id;
                                }
                            }
                            FSendPlrMsg(pl.iPlayer, idm, iGoto, lpfl->id, LOWORD(lXferMinerals), HIWORD(lXferMinerals), pl.id, i, 0, 0);
                        }
                        idPlayer = -1;
                        gd.fDontCalcBleed = 0;
                        FRemovePlayerMessage(lpfl->iPlayer, 78, lpfl->id | 0x8000);
                        lpfl->fDead = 1;
                        if (ord.grTask == grTaskColonize) {
                            FQueueColonistDrop(lpfl, &pl, lpfl->rgwtMin[3]);
                        } else if (lpfl->idPlanet != -1 && lpPlanets[lpfl->idPlanet].iPlayer == lpfl->iPlayer) {
                            lpPlanets[lpfl->idPlanet].rgwtMin[3] = lpPlanets[lpfl->idPlanet].rgwtMin[3] + lpfl->rgwtMin[3];
                        }
                    }
                }
            CancelOrder:
                if (lpfl->cord == 1 && lpfl->fDead == 0 && lpfl->lpplord->rgord[0].grTask != grTaskNone) {
                    FRemovePlayerMessage(lpfl->iPlayer, 78, lpfl->id | 0x8000);
                    FSendPlrMsg2(lpfl->iPlayer, 78, lpfl->id | 0x8000, lpfl->id, 0);
                }
                lpfl->lpplord->rgord[0].grTask = grTaskNone;
            }
        L_67b6:;
        }
    }
    return;
}
