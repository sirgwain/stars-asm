#include "common.h"

void LogSplitFleet(int16_t id) {
    if (gd.fGeneratingTurn != 0) {
        LpflFromId(id)->fCompChg = 1;
    } else {
        WriteMemRt(24, 2, &id);
    }
    return;
}

void LogMergeFleet(int16_t id) {
    uint16_t idCur;
    int16_t  i;
    uint16_t rgid[512];
    int16_t  j;

    if (gd.fGeneratingTurn == 0) {
        i = 0;
        while (i < vcflMerge) {
            rgid[0] = id;
            j = 1;
            for (; j < 511 && i < vcflMerge; i++) {
                if (vrgiflMerge[i] != -1) {
                    idCur = vrgiflMerge[i];
                    if (idCur != id) {
                        rgid[j++] = idCur;
                    }
                }
            }
            WriteMemRt(37, j * 2, rgid);
        }
    }
    return;
}

void LogChangeShDef(SHDEF *lpshdefNew) {
    uint8_t  rgb[149];
    uint8_t *pb;

    if (gd.fGeneratingTurn == 0) {
        RawStore16(rgb, (RawLoad16(rgb) & 0xe0ff) | (lpshdefNew->ishdef & 0x1f) << 8);
        RawStore16(rgb, (RawLoad16(rgb) & 0xff0f) | (idPlayer & 0xf) << 4);
        if (lpshdefNew->fFree != 0) {
            RawStore16(rgb, RawLoad16(rgb) & 0xfff0);
            WriteMemRt(27, 2, rgb);
        } else {
            RawStore16(rgb, (RawLoad16(rgb) & 0xfff0) | 1);
            lpshdefNew->det = detAll;
            pb = &rgb[2];
            WriteRtShDef(lpshdefNew, &pb);
            WriteMemRt(27, pb - rgb, rgb);
        }
        if (gd.fTutorial != 0 && idPlayer == 0) {
            tutor.fChange = 1;
            AdvanceTutor();
        }
    }
    return;
}

void LogChangeName(GrobjClass grobj, int16_t id, char *szName) {
    FLEET    *lpfl;
    int16_t   cOut;
    RTCHGNAME rtchgname;

    lpfl = LpflFromId(id);
    if (lpfl != 0) {
        if (lpfl->lpszName != 0) {
            FreeLp(lpfl->lpszName, htString);
        }
        if (szName == 0 || (int16_t)(int8_t)*szName == 0) {
            rtchgname.rgb[0] = 0;
            rtchgname.rgb[1] = 0;
            cOut = 1;
            lpfl->lpszName = NULL;
        } else {
            cOut = strlen(szName);
            lpfl->lpszName = LpAlloc(strlen(szName) + 1, htString);
            fstrcpy(lpfl->lpszName, szName);
            if (FCompressUserString(szName, &rtchgname.rgb[1], &cOut) != 0) {
                rtchgname.rgb[0] = LOBYTE(cOut);
            } else {
                rtchgname.rgb[0] = 0;
                strcpy(&rtchgname.rgb[1], szName);
                cOut++;
            }
        }
        rtchgname.grobj = grobj;
        rtchgname.id = id;
        WriteMemRt(44, cOut + 5, &rtchgname);
        if (gd.fTutorial != 0) {
            AdvanceTutor();
        }
    }
    return;
}

void LogChangeFleet(FLEET *pfl, FLEET *pflNew) {
    int16_t   d;
    int16_t   i;
    int16_t   fChg;
    LOGXFERF  lxfNew;
    LOGXFER   lxNew;
    RTWAYPT   rtwp;
    RTSHIPINT rtsi;
    int16_t   iordNew;
    int16_t   iordOld;
    int16_t   cbWp;
    char     *pbWp;
    HDR       hdr;

    fChg = 0;
    if (gd.fGeneratingTurn == 0) {
        lxfNew.id = pfl->id;
        lxfNew.grobj = grobjFleet;
        for (i = 0; i < 16; i++) {
            lxfNew.rgdItem[i] = pflNew->rgcsh[i] - pfl->rgcsh[i];
            if (pflNew->rgcsh[i] - pfl->rgcsh[i] != 0) {
                fChg = 1;
            }
        }
        if (fChg != 0) {
            if (fValidLxf != 0) {
                LogMakeValidXferf(&lxf, &lxfNew);
                fValidLxf = 0;
            } else {
                lxf = lxfNew;
                fValidLxf = 1;
            }
        } else {
            lxNew.id = pfl->id;
            lxNew.grobj = grobjFleet;
            for (i = 0; i < 5; i++) {
                lxNew.rgdItem[i] = pflNew->rgwtMin[i] - pfl->rgwtMin[i];
                if (pflNew->rgwtMin[i] - pfl->rgwtMin[i] != 0) {
                    fChg = 1;
                }
            }
            if (fChg != 0) {
                if (fValidLx != 0) {
                    LogMakeValidXfer(&lx, &lxNew);
                    fValidLx = 0;
                } else {
                    lx = lxNew;
                    fValidLx = 1;
                }
            } else {
                if (pfl->iplan != pflNew->iplan) {
                    rtsi.id = pflNew->id;
                    rtsi.i = pflNew->iplan;
                    WriteMemRt(42, 4, &rtsi);
                }
                if (pfl->fRepOrders != pflNew->fRepOrders) {
                    rtsi.id = pflNew->id;
                    rtsi.i = pflNew->fRepOrders;
                    WriteMemRt(10, 4, &rtsi);
                }
                d = pflNew->cord - pfl->cord;
                for (iordOld = 0; iordOld < pfl->cord && iordOld < pflNew->cord &&
                                  fmemcmp(&pfl->lpplord->rgord[iordOld], &pflNew->lpplord->rgord[iordOld], sizeof(ORDER)) == 0;
                     iordOld++) {
                }
                iordNew = iordOld;
                if (iordOld != pfl->cord || d != 0) {
                    if (d < 0) {
                        rtsi.id = pflNew->id;
                        rtsi.i = iordOld;
                        if (d == -2) {
                            rtsi.i |= 0x8000;
                        }
                        WriteMemRt(3, 4, &rtsi);
                    } else if (d > 0) {
                        cbWp = 22;
                        rtwp.id = pflNew->id;
                        rtwp.iWaypt = iordNew;
                        rtwp.order = pflNew->lpplord->rgord[iordNew];
                        pbWp = (char *)&rtwp;
                        while (cbWp-- > 0 && (int16_t)(int8_t)pbWp[cbWp] == 0) {
                        }
                        cbWp++;
                        WriteMemRt(4, cbWp, &rtwp);
                    } else {
                        cbWp = 22;
                        if (FGetPrevLogRt(&hdr, rgbCur) != 0 && hdr.rt == rtLogFleetOrderUpdate && RawLoad16(rgbCur) == pflNew->id &&
                            RawLoad16(&rgbCur[2]) == iordNew) {
                            imemLogCur = imemLogPrev;
                        }
                        rtwp.id = pflNew->id;
                        rtwp.iWaypt = iordNew;
                        rtwp.order = pflNew->lpplord->rgord[iordNew];
                        pbWp = (char *)&rtwp;
                        while (cbWp-- > 0 && (int16_t)(int8_t)pbWp[cbWp] == 0) {
                        }
                        cbWp++;
                        WriteMemRt(5, cbWp, &rtwp);
                    }
                }
            }
        }
    }
    return;
}

void LogChangeRelations() {
    HDR hdr;

    if (FGetPrevLogRt(&hdr, rgbCur) != 0 && hdr.rt == rtLogRelations) {
        imemLogCur = imemLogPrev;
    }
    WriteMemRt(38, game.cPlayer, rgplr[idPlayer].rgmdRelation);
    if (gd.fTutorial != 0 && idPlayer == 0) {
        tutor.fChange = 1;
        AdvanceTutor();
    }
    return;
}

void LogChangeBtlplan(BTLPLAN *pbtlplan) {
    WriteBattlePlan(pbtlplan, 1);
    if (gd.fTutorial != 0 && idPlayer == 0) {
        tutor.fChange = 1;
        AdvanceTutor();
    }
    return;
}

void LogChangePlanet(PLANET *ppl, PLANET *pplNew) {
    int16_t i;
    int16_t fChg;
    HDR     hdr;
    LOGXFER lxNew;

    fChg = 0;
    if (gd.fGeneratingTurn == 0) {
        if (ppl == 0) {
            if (fValidLx == 0) {
                return;
            }
            lxNew.id = -1;
            lxNew.grobj = grobjOther;
            for (i = 0; i < 5; i++) {
                lxNew.rgdItem[i] = -lx.rgdItem[i];
            }
        } else {
            lxNew.id = ppl->id;
            lxNew.grobj = grobjPlanet;
            for (i = 0; i < 4; i++) {
                lxNew.rgdItem[i] = pplNew->rgwtMin[i] - ppl->rgwtMin[i];
                if (pplNew->rgwtMin[i] - ppl->rgwtMin[i] != 0) {
                    fChg = 1;
                }
            }
            lxNew.rgdItem[4] = 0;
            if (fChg == 0)
                goto L_9573;
        }
        if (fValidLx != 0) {
            LogMakeValidXfer(&lx, &lxNew);
            fValidLx = 0;
        } else {
            lx = lxNew;
            fValidLx = 1;
        }
    L_9573:
        if (ppl != 0) {
            if (pplNew->lpplprod == 0 && ppl->lpplprod != 0) {
                WriteMemRt(29, 2, &lxNew);
            } else if (pplNew->lpplprod != 0 && (ppl->lpplprod == 0 || ppl->lpplprod->iprodMac != pplNew->lpplprod->iprodMac ||
                                                 fmemcmp(ppl->lpplprod->rgprod, pplNew->lpplprod->rgprod, ppl->lpplprod->iprodMac * 4) != 0)) {
                if (FGetPrevLogRt(&hdr, rgbCur) != 0 && hdr.rt == rtLogPlanetProdQ && RawLoad16(rgbCur) == ppl->id) {
                    imemLogCur = imemLogPrev;
                }
                RawStore16(rgbCur, ppl->id);
                fmemmove(&rgbCur[2], pplNew->lpplprod->rgprod, pplNew->lpplprod->iprodMac * 4);
                WriteMemRt(29, pplNew->lpplprod->iprodMac * 4 + 2, rgbCur);
            }
            if ((uint32_t)ppl->fNoResearch != pplNew->fNoResearch || ppl->idFling != pplNew->idFling || ppl->iWarpFling != pplNew->iWarpFling ||
                ppl->idRoute != pplNew->idRoute) {
                RawStore32(rgbCur, (uint32_t)pplNew->id);
                RawStore16(&rgbCur[4], 0);
                RawStore32(&rgbCur[2], (RawLoad32(&rgbCur[2]) & 0xfffffffe) | (int32_t)((uint32_t)(LOWORD((uint32_t)pplNew->fNoResearch) & 1) << 0));
                RawStore32(&rgbCur[2], (RawLoad32(&rgbCur[2]) & 0xfffff801) | (int32_t)((uint32_t)(LOWORD((uint32_t)pplNew->idFling) & 0x3ff) << 1));
                RawStore32(&rgbCur[2], (RawLoad32(&rgbCur[2]) & 0xffff87ff) | (int32_t)((uint32_t)(LOWORD((uint32_t)pplNew->iWarpFling) & 0xf) << 0xb));
                RawStore32(&rgbCur[2], (RawLoad32(&rgbCur[2]) & 0xfe007fff) | (int32_t)((uint32_t)(LOWORD((uint32_t)pplNew->idRoute) & 0x3ff) << 0xf));
                WriteMemRt(35, 6, rgbCur);
            }
        }
    }
    return;
}

void LogChangeThing(THING *lpth, THING *pthNew) {
    int16_t i;
    int16_t fChg;
    LOGXFER lxNew;

    fChg = 0;
    if (gd.fGeneratingTurn == 0) {
        memset(&lxNew, 0, sizeof(LOGXFER));
        lxNew.id = pthNew->idFull;
        lxNew.grobj = grobjThing;
        for (i = 0; i < 3; i++) {
            lxNew.rgdItem[i] = (int16_t)(pthNew->thp.rgwtMin[i] - lpth->thp.rgwtMin[i]);
            if ((int16_t)(pthNew->thp.rgwtMin[i] - lpth->thp.rgwtMin[i]) != 0) {
                fChg = 1;
            }
        }
        if (fChg != 0) {
            if (fValidLx != 0) {
                LogMakeValidXfer(&lx, &lxNew);
                fValidLx = 0;
            } else {
                lx = lxNew;
                fValidLx = 1;
            }
        }
    }
    return;
}

void LogMakeValidXfer(LOGXFER *plx1, LOGXFER *plx2) {
    int32_t  rgQuan[5];
    RTXFER  *prt;
    int16_t  iOff;
    RTXFERL *prtl;
    int16_t  rt;
    int16_t  i;
    char     rgbuf[28];
    int16_t  grbit;
    RTXFERX *prtx;
    int16_t  grFlag;
    int32_t  iBiggest;
    int16_t  cb;

    iBiggest = 0;
    grbit = 0;
    grFlag = 1;
    rgQuan[0] = 0;
    rgQuan[1] = 0;
    rgQuan[2] = 0;
    rgQuan[3] = 0;
    rgQuan[4] = 0;
    switch (hdrPrev.rt) {
    case rtLogCargoXfer8:
    case rtLogCargoXfer16:
    case rtLogCargoXfer32:
        prt = (RTXFER *)(lpLog + (imemLogCur + -hdrPrev.cb));
        break;
    default:
        prt = NULL;
    }
    if (prt != 0 && prt->grobj1 == (plx1->grobj & 0xff) && prt->grobj2 == (plx2->grobj & 0xff) && prt->id1 == plx1->id && prt->id2 == plx2->id) {
        grbit = prt->grbitItems;
        iOff = 0;
        switch (hdrPrev.rt) {
        case rtLogCargoXfer8:
            for (i = 0; i < 5; i++) {
                if ((1 << i & grbit) != 0) {
                    rgQuan[i] = (int16_t)prt->rgcQuan[iOff];
                    iOff++;
                }
            }
            break;
        case rtLogCargoXfer16:
            prtx = (RTXFERX *)prt;
            for (i = 0; i < 5; i++) {
                if ((1 << i & grbit) != 0) {
                    rgQuan[i] = prtx->rgcQuan[iOff];
                    iOff++;
                }
            }
            break;
        case rtLogCargoXfer32:
            prtl = (RTXFERL *)prt;
            for (i = 0; i < 5; i++) {
                if ((1 << i & grbit) != 0) {
                    rgQuan[i] = prtl->rgcQuan[iOff];
                    iOff++;
                }
            }
        }
        CancelMemRt(hdrPrev.rt);
    }
    i = 0;
    grbit = 0;
    while (i < 5) {
        rgQuan[i] += plx1->rgdItem[i];
        if (iBiggest <= labs(rgQuan[i])) {
            iBiggest = labs(rgQuan[i]);
        }
        if (rgQuan[i] != 0) {
            grbit |= grFlag;
        }
        i++;
        grFlag *= 2;
    }
    if (grbit != 0) {
        prt = (RTXFER *)rgbuf;
        prt->grobj1 = (uint32_t)plx1->grobj & 0xf;
        prt->grobj2 = (uint32_t)plx2->grobj & 0xf;
        prt->id1 = plx1->id;
        prt->id2 = plx2->id;
        prt->grbitItems = LOBYTE(grbit);
        cb = 6;
        iOff = 0;
        if (iBiggest <= 127) {
            rt = 1;
            for (i = 0; i < 5; i++) {
                if (rgQuan[i] != 0) {
                    prt->rgcQuan[iOff++] = LOBYTE(LOWORD(rgQuan[i]));
                    cb++;
                }
            }
        } else if (iBiggest <= 32767) {
            rt = 2;
            prtx = (RTXFERX *)rgbuf;
            for (i = 0; i < 5; i++) {
                if (rgQuan[i] != 0) {
                    prtx->rgcQuan[iOff++] = LOWORD(rgQuan[i]);
                    cb += 2;
                }
            }
        } else {
            rt = 25;
            prtl = (RTXFERL *)rgbuf;
            for (i = 0; i < 5; i++) {
                if (rgQuan[i] != 0) {
                    prtl->rgcQuan[iOff++] = rgQuan[i];
                    cb += 4;
                }
            }
        }
        WriteMemRt(rt, cb, rgbuf);
    }
    return;
}

void LogMakeValidXferf(LOGXFERF *plxf1, LOGXFERF *plxf2) {
    RTXFERF *prt;
    int16_t  iOff;
    int16_t  i;
    char     rgbuf[41];
    uint16_t grbit;
    int16_t  grFlag;
    int16_t  cb;

    grbit = 0;
    grFlag = 1;
    i = 0;
    while (i < 16) {
        if (plxf1->rgdItem[i] != 0) {
            grbit |= grFlag;
        }
        i++;
        grFlag *= 2;
    }
    if (grbit != 0) {
        prt = (RTXFERF *)&rgbuf;
        prt->grobj1 = (uint32_t)plxf1->grobj & 0xf;
        prt->grobj2 = (uint32_t)plxf2->grobj & 0xf;
        prt->id1 = plxf1->id;
        prt->id2 = plxf2->id;
        prt->grbitItems = grbit;
        cb = 7;
        iOff = 0;
        for (i = 0; i < 16; i++) {
            if (plxf1->rgdItem[i] != 0) {
                prt->rgcQuan[iOff++] = plxf1->rgdItem[i];
                cb += 2;
            }
        }
        WriteMemRt(23, cb, rgbuf);
    }
    return;
}

void CancelMemRt(RecordType rt) {
    imemLogCur -= hdrPrev.cb + 2;
    hdrPrev.rt = rtEOF;
    return;
}

void WriteMemRt(int16_t rt, int16_t cb, void *rg) {
    HDR      hdr;
    uint8_t *lpv;

    if (fLogOff == 0) {
        if (imemLogCur + cb + 2 > 32000) {
            AlertSz(PszFormatIds(idsLogFileHasReachedMaximumAllowableSize, NULL), MB_ICONHAND);
        }
        DirtyGame(1);
        imemLogPrev = imemLogCur;
        hdr.cb = cb;
        hdr.rt = rt;
        lpv = lpLog;
        lpv += imemLogCur;
        RawStore16(lpv, *(uint16_t *)&hdr);
        if (cb > 0) {
            fmemcpy(lpv + 2, rg, cb);
        }
        imemLogCur += cb + 2;
        if (rt != 0) {
            hdrPrev = hdr;
        }
    }
    return;
}

void DirtyGame(int16_t fDirty) {
    if (fDirty != game.fDirty) {
        game.fDirty = fDirty;
        if (fAi == 0) {
            SetMsgTitle(hwndMessage);
        }
    }
    return;
}

int16_t FGetPrevLogRt(HDR *phdr, uint8_t *pb) {
    uint8_t *lpv;

    if (imemLogPrev == -1) {
        return 0;
    }
    lpv = lpLog + imemLogPrev;
    *phdr = *(HDR *)lpv;
    if (phdr->cb > 0) {
        fmemcpy(pb, lpv + 2, phdr->cb);
    }
    return 1;
}

int16_t FRunLogFile() {
    int16_t fLogOld;
    int16_t fRet;
    int16_t iCur;
    HDR    *lprts;

    iCur = 0;
    fRet = 1;
    fLogOld = fLogOff;
    if (imemLogCur == 0) {
        return 1;
    }
    fLogOff = 1;
    for (; iCur < imemLogCur; iCur += lprts->cb + 2) {
        lprts = (HDR *)(lpLog + iCur);
        fRet &= FRunLogRecord(lprts->rt, lprts->cb, lpLog + (2 + iCur));
    }
    fLogOff = fLogOld;
    gd.fFleetLinkValid = 0;
    return fRet;
}

int16_t FRunLogRecord(RecordType rt, int16_t cb, uint8_t *lpb) {
    int16_t   fExtra;
    int32_t   cXfer;
    XFERFULL *lpxfCur;
    PLANET   *lppl;
    int32_t   rgcXfer[5];
    XFER      rgxf[2];
    FLEET    *lpfl;
    int16_t   ifl;
    int16_t   i;
    uint16_t  grbit;
    int16_t   rgifl[512];
    SHDEF    *lpshdef;
    int16_t   iPass;
    int16_t   iLook;
    PLANET   *lpplMac;
    int8_t    ch;
    int32_t   l;
    char      szT[33];
    int16_t   cOut;
    THING    *lpth;
    int16_t   id;
    int16_t   iColDrop;
    COLDROP  *lpcdT;
    XFERFULL *lpxfMax;
    MessageId idm;
    int16_t   t_b926;
    int16_t   t_b9dc;

    lpxfCur = NULL;
    if (rt <= rtLogPlayerZpq1) {
        switch (rt) {
        case 29:
            lppl = lpPlanets;
            lpplMac = lpPlanets + cPlanet;
            for (; lppl < lpplMac && lppl->id != RawLoad16(lpb); lppl++) {
            }
            if (lppl != lpplMac && lppl->iPlayer == idPlayer) {
                i = (uint32_t)(cb - 2) / 4;
                if (i > 0) {
                    if (lppl->lpplprod == 0) {
                        lppl->lpplprod = (PLPROD *)LpplAlloc(4, i + 2, htOrd);
                    } else if (lppl->lpplprod->iprodMax < i) {
                        lppl->lpplprod = (PLPROD *)LpplReAlloc((PL *)lppl->lpplprod, i + 2);
                    }
                    for (iPass = 0; iPass < i; iPass++) {
                        if ((uint32_t)(LOWORD((uint32_t)(RawLoad32(lpb + (iPass * 4 + 2)) >> 0x14)) & 0x7f) != 0) {
                            for (iLook = 0; iLook < lppl->lpplprod->iprodMac; iLook++) {
                                if (lppl->lpplprod->rgprod[iLook].pct != 0 &&
                                    lppl->lpplprod->rgprod[iLook].iItem ==
                                        (uint32_t)(LOWORD((uint32_t)(((uint32_t)(uint16_t)RawLoad16(lpb + (iPass * 4 + 4)) << 0x10 |
                                                                      (uint16_t)RawLoad16(lpb + (iPass * 4 + 2))) >>
                                                                     0xa)) &
                                                   0x7f) &&
                                    lppl->lpplprod->rgprod[iLook].grobj ==
                                        (uint32_t)(LOWORD((uint32_t)(((uint32_t)(uint16_t)RawLoad16(lpb + (iPass * 4 + 4)) << 0x10 |
                                                                      (uint16_t)RawLoad16(lpb + (iPass * 4 + 2))) >>
                                                                     0x11)) &
                                                   7)) {
                                    lppl->lpplprod->rgprod[iLook].pct = 0;
                                    break;
                                }
                            }
                            if (iLook == lppl->lpplprod->iprodMac) {
                                RawStore16(lpb + (iPass * 4 + 2), RawLoad16(lpb + (iPass * 4 + 2)) & 0xffff);
                                RawStore16(lpb + (iPass * 4 + 4), RawLoad16(lpb + (iPass * 4 + 4)) & 0xf80f);
                            }
                        }
                    }
                    fmemmove(lppl->lpplprod->rgprod, lpb + 2, i * 4);
                    lppl->lpplprod->iprodMac = LOBYTE(i);
                    return 1;
                }
                if (lppl->lpplprod == 0) {
                    return 1;
                }
                FreeLp(lppl->lpplprod, htOrd);
                lppl->lpplprod = NULL;
                return 1;
            }
            return 0;
        case 38:
            fmemcpy(rgplr[idPlayer].rgmdRelation, lpb, game.cPlayer);
            return 1;
        case 46:
            if (gd.fGeneratingTurn == 0) {
                return 1;
            }
            if ((uint16_t)cb > 26) {
                return 0;
            }
            fmemcpy(&rgplr[idPlayer].zpq1, lpb, cb);
            return 1;
        case 44:
            cOut = 32;
            lpfl = LpflFromId(RawLoad16(lpb));
            if (lpfl == 0) {
                return 0;
            }
            if (lpfl->lpszName != 0) {
                FreeLp(lpfl->lpszName, htString);
            }
            i = lpb[4];
            if (i != 0 && FDecompressUserString(lpb + 5, i, szT, &cOut) != 0) {
                lpfl->lpszName = LpAlloc(strlen(szT) + 1, htString);
                fstrcpy(lpfl->lpszName, szT);
                return 1;
            }
            if (lpb[5] == 0) {
                lpfl->lpszName = NULL;
                return 1;
            }
            lpfl->lpszName = LpAlloc(fstrlen(lpb + 5) + 1, htString);
            fstrcpy(lpfl->lpszName, lpb + 5);
            return 1;
        case 43:
            lpth = LpthFromId(RawLoad16(lpb));
            if (lpth == 0 || lpth->ith != ithMinefield) {
                return 0;
            }
            lpth->thm.fDetonate = LOBYTE(RawLoad16((uint8_t *)lpb + 0x2));
            return 1;
        case 27:
            i = RawLoad16(lpb) >> 8 & 0x1f;
            iLook = RawLoad16(lpb) >> 4 & 0xf;
            if (iLook >= game.cPlayer || iLook != idPlayer) {
                return 0;
            }
            if (i >= 16) {
                if (i >= 26) {
                    return 0;
                }
                lpshdef = rglpshdefSB[iLook] + (i - 16);
            } else {
                lpshdef = rglpshdef[iLook] + i;
            }
            if (lpshdef->fFree == 0 && lpshdef->cExist != 0 && (RawLoad16(lpb) & 0xf) != 0) {
                return 0;
            }
            if ((RawLoad16(lpb) & 0xf) != 0) {
                if ((RawLoad16(lpb) & 0xf) != 1) {
                    return 1;
                }
                if (lpshdef->fFree != 0) {
                    if (i >= 16) {
                        rgplr[iLook].cshdefSB++;
                    } else {
                        rgplr[iLook].cShDef++;
                    }
                }
                if (i >= 16) {
                    if (FReadShDef((RTSHDEF *)(lpb + 2), rglpshdefSB[iLook], idPlayer) != 0) {
                        return 1;
                    }
                    rgplr[iLook].cshdefSB += 15;
                    return 0;
                }
                if (FReadShDef((RTSHDEF *)(lpb + 2), rglpshdef[iLook], idPlayer) != 0) {
                    return 1;
                }
                rgplr[iLook].cShDef--;
                return 0;
            }
            if (lpshdef->fFree != 0) {
                return 1;
            }
            DestroyAllIshdef(i, idPlayer);
            lpshdef->fFree = 1;
            if (i >= 16) {
                rgplr[iLook].cshdefSB += 15;
                return 1;
            }
            rgplr[iLook].cShDef--;
            return 1;
        case 1:
        case 2:
        case 25:
            if (FLookupObject(lpb[4] & 0xf, RawLoad16(lpb), &rgxf[0].fl) == 0) {
                return 0;
            }
            rgxf[1].fl.id = -1;
            if ((lpb[4] >> 4 & 0xf) != 4 && FLookupObject(lpb[4] >> 4 & 0xf, RawLoad16((uint8_t *)lpb + 0x2), &rgxf[1].fl) == 0) {
                if ((lpb[4] >> 4 & 0xf) == 2 && (RawLoad16((uint8_t *)lpb + 0x2) >> 9 & 0xf) != idPlayer) {
                    return 1;
                }
                return 0;
            }
            grbit = lpb[5];
            i = 0;
            iLook = 0;
            while (i < 5) {
                if ((grbit & 1) != 0) {
                    if (rt == rtLogCargoXfer8) {
                        rgcXfer[i] = (int16_t)(int8_t)lpb[iLook + 6];
                    } else if (rt == rtLogCargoXfer16) {
                        rgcXfer[i] = (int16_t)RawLoad16(lpb + (iLook * 2 + 6));
                    } else {
                        rgcXfer[i] = RawLoad32(lpb + (iLook * 4 + 6));
                    }
                    iLook++;
                } else {
                    rgcXfer[i] = 0;
                }
                i++;
                grbit >>= 1;
            }
            for (iPass = 0; iPass < 2; iPass++) {
                i = 0;
                while (i < 5) {
                    if (rgcXfer[i] != 0) {
                        cXfer = rgcXfer[i];
                        if ((iPass == 0 && cXfer < 0) || (iPass == 1 && cXfer >= 0)) {
                            l = ChgCargo(lpb[4] & 0xf, RawLoad16(lpb), i, cXfer, &rgxf[0].fl);
                            if (l != cXfer) {
                                id = (lpb[4] & 0xf) == 2 ? -32768 : 0;
                                id |= rgxf[0].fl.id;
                                FSendPlrMsg(rgxf[0].fl.iPlayer, 221, id, id, LOWORD(cXfer) - LOWORD(l), i, LOWORD(cXfer), 0, 0, 0);
                                rgcXfer[i] = l;
                            }
                        }
                        if (((iPass == 0 && cXfer >= 0) || (iPass == 1 && cXfer < 0)) && (lpb[4] >> 4 & 0xf) != 4) {
                            if (i == 3 && cXfer != 0 && gd.fGeneratingTurn != 0 && (lpb[4] >> 4 & 0xf) == 1 && (lpb[4] & 0xf) == 2 &&
                                rgxf[0].fl.iPlayer != rgxf[1].fl.iPlayer) {
                                lpcdT = lpcd;
                                iColDrop = 0;
                                if (cXfer > 0) {
                                    for (; iColDrop < cColDrop && cXfer > 0; iColDrop++) {
                                        if (lpcdT->idPlanetDst == rgxf[1].pl.id && lpcdT->idPlr == rgxf[0].fl.iPlayer) {
                                            l = cXfer < lpcdT->cColonist ? cXfer : lpcdT->cColonist;
                                            cXfer -= l;
                                            lpcdT->cColonist -= l;
                                        }
                                        lpcdT++;
                                    }
                                } else {
                                    for (; iColDrop < cColDrop && (lpcdT->idFleetSrc != rgxf[0].fl.id || lpcdT->idPlanetDst != rgxf[1].pl.id); iColDrop++) {
                                        lpcdT++;
                                    }
                                    if (iColDrop == cColDrop) {
                                        lpcdT->idFleetSrc = rgxf[0].fl.id;
                                        lpcdT->idPlr = rgxf[0].fl.iPlayer;
                                        lpcdT->idPlanetDst = rgxf[1].fl.id;
                                        lpcdT->cColonist = 0;
                                        lpcdT->fCanColonize = rgxf[1].fl.iPlayer == -1 ? 0 : 1;
                                        cColDrop++;
                                    }
                                    lpcdT->cColonist -= cXfer;
                                }
                            } else {
                                if (cXfer != 0 && gd.fGeneratingTurn != 0 && rgxf[1].fl.iPlayer != rgxf[0].fl.iPlayer && (lpb[4] >> 4 & 0xf) != 8) {
                                    lpxfMax = lpxf + cXferFull;
                                    if (cXfer > 0) {
                                        for (lpxfCur = lpxf; lpxfCur < lpxfMax && cXfer > 0; lpxfCur++) {
                                            if (lpxfCur->grobj2 == (lpb[4] >> 4 & 0xf) && lpxfCur->id2 == RawLoad16((uint8_t *)lpb + 0x2) &&
                                                lpxfCur->grobj1 == 2 && (lpxfCur->id1 & 0xfe00) == (RawLoad16(lpb) & 0xfe00)) {
                                                l = cXfer < lpxfCur->rgcQuan[i] ? cXfer : lpxfCur->rgcQuan[i];
                                                cXfer -= l;
                                                lpxfCur->rgcQuan[i] -= l;
                                            }
                                        }
                                        lpxfCur = NULL;
                                        if (cXfer <= 0)
                                            goto L_af00;
                                    } else if (iPass != 1) {
                                        if (lpxfCur == 0) {
                                            for (lpxfCur = lpxf; lpxfCur < lpxfMax && fmemcmp(lpxfCur, lpb, 5) != 0; lpxfCur++) {
                                            }
                                            if (lpxfCur == lpxfMax) {
                                                cXferFull++;
                                                fmemset(lpxfCur, 0, sizeof(XFERFULL));
                                                lpxfCur->id1 = RawLoad16(lpb);
                                                lpxfCur->id2 = RawLoad16((uint8_t *)lpb + 0x2);
                                            }
                                        }
                                        lpxfCur->rgcQuan[i] -= cXfer;
                                        goto L_af00;
                                    }
                                }
                                l = ChgCargo(lpb[4] >> 4 & 0xf, RawLoad16((uint8_t *)lpb + 0x2), i, -cXfer, &rgxf[1].fl);
                                if (l != -cXfer) {
                                    rgcXfer[i] = -l;
                                    if ((lpb[4] >> 4 & 0xf) == 8) {
                                        idm = idmDidntGetAttemptedTransferMineralPacketAnother;
                                        if (l == 0) {
                                            idm++;
                                        }
                                        FSendPlrMsg(rgxf[0].fl.iPlayer, idm, rgxf[0].fl.id | 0x8000, rgxf[0].fl.id, i, -LOWORD(l), i, 0, 0, 0);
                                    } else {
                                        id = (lpb[4] & 0xf) == 2 ? -32768 : 0;
                                        id |= rgxf[0].fl.id;
                                        FSendPlrMsg(rgxf[0].fl.iPlayer, 221, id, id, -LOWORD(l) - LOWORD(cXfer), i, -LOWORD(cXfer), 0, 0, 0);
                                    }
                                }
                            }
                        }
                    }
                L_af00:
                    i++;
                    grbit >>= 1;
                }
            }
            if ((lpb[4] & 0xf) == 2) {
                FLookupFleet(-1, &rgxf[0].fl);
            } else {
                FLookupPlanet(-1, &rgxf[0].pl);
            }
            switch (lpb[4] >> 4 & 0xf) {
            case 2:
                FLookupFleet(-1, &rgxf[1].fl);
                break;
            case 1:
            case 4:
                FLookupPlanet(-1, &rgxf[1].pl);
                break;
            case 8:
                FLookupThing(-1, &rgxf[1].th);
            }
            return 1;
        case 24:
        case 37:
            if (FLookupObject(grobjFleet, RawLoad16(lpb), &rgxf[0].fl) == 0) {
                return 0;
            }
            if (rt == rtLogFleetSplit) {
                if (LpflNewSplit(&rgxf[0].fl) != 0) {
                    return 1;
                }
                return 0;
            }
            vrgiflMerge = rgifl;
            vcflMerge = 0;
            if (cb == 2) {
                for (ifl = 0; ifl < cFleet; ifl++) {
                    lpfl = rglpfl[ifl];
                    if (rglpfl[ifl] == 0)
                        break;
                    if (lpfl->iPlayer == idPlayer && lpfl->fDead == 0 && lpfl->pt.x == rgxf[0].fl.pt.x && lpfl->pt.y == rgxf[0].fl.pt.y) {
                        t_b926 = vcflMerge;
                        vcflMerge++;
                        rgifl[t_b926] = lpfl->id;
                        lpfl->fCompChg = 1;
                    }
                }
            } else {
                for (i = 0; i < (uint32_t)cb / 2; i++) {
                    for (ifl = 0; ifl < cFleet; ifl++) {
                        lpfl = rglpfl[ifl];
                        if (rglpfl[ifl] == 0)
                            break;
                        if (lpfl->id == RawLoad16(lpb)) {
                            t_b9dc = vcflMerge;
                            vcflMerge++;
                            rgifl[t_b9dc] = lpfl->id;
                            lpfl->fCompChg = 1;
                            break;
                        }
                    }
                    lpb += 2;
                }
            }
            if (FFleetMergeAll(&rgxf[0].fl) != 0) {
                return 1;
            }
            return 0;
        case 23:
            if (FLookupObject(grobjFleet, RawLoad16(lpb), &rgxf[0].fl) == 0) {
                return 0;
            }
            if (FLookupObject(grobjFleet, RawLoad16((uint8_t *)lpb + 0x2), &rgxf[1].fl) == 0) {
                return 0;
            }
            if (rgxf[1].fl.iPlayer != rgxf[0].fl.iPlayer) {
                return 0;
            }
            for (iPass = 0; iPass < 2; iPass++) {
                grbit = RawLoad16((uint8_t *)lpb + 0x5);
                i = 0;
                iLook = 0;
                while (i < 16) {
                    if ((grbit & 1) != 0) {
                        cXfer = (int16_t)RawLoad16(lpb + (iLook * 2 + 7));
                        if ((iPass == 0 && cXfer < 0) || (iPass == 1 && cXfer >= 0)) {
                            if (rgxf[0].fl.rgcsh[i] + cXfer < 0) {
                                cXfer = (int16_t)-rgxf[0].fl.rgcsh[i];
                            } else if ((int16_t)(32766 - rgxf[0].fl.rgcsh[i]) <= cXfer) {
                                cXfer = (int16_t)(32766 - rgxf[0].fl.rgcsh[i] - 1);
                            }
                            rgxf[0].fl.rgcsh[i] += LOWORD(cXfer);
                        }
                        if ((iPass == 0 && cXfer >= 0) || (iPass == 1 && cXfer < 0)) {
                            if (rgxf[1].fl.rgcsh[i] - cXfer < 0) {
                                cXfer = rgxf[1].fl.rgcsh[i];
                            } else if ((int16_t)(32766 - rgxf[1].fl.rgcsh[i]) <= -cXfer) {
                                cXfer = (int16_t)-(32766 - rgxf[1].fl.rgcsh[i] - 1);
                            }
                            rgxf[1].fl.rgcsh[i] -= LOWORD(cXfer);
                        }
                        iLook++;
                    }
                    i++;
                    grbit >>= 1;
                }
            }
            FleetTransferCargoBalance(&rgxf[0].fl, &rgxf[1].fl);
            for (iPass = 0; iPass < 2; iPass++) {
                FLookupFleet(-1, &rgxf[iPass].fl);
                lpfl = LpflFromId(rgxf[iPass].fl.id);
                if (lpfl != 0) {
                    lpfl->fCompChg = 1;
                }
                for (i = 0; i < 16 && rgxf[iPass].fl.rgcsh[i] == 0; i++) {
                }
                if (i == 16) {
                    FDeleteFleet(rgxf[iPass].fl.id, grobjNone, 0);
                }
            }
            return 1;
        case 3:
            lpfl = LpflFromId(RawLoad16(lpb));
            if (lpfl != 0 && lpfl->cord > 0) {
                iLook = RawLoad16((uint8_t *)lpb + 0x2) & 0x7fff;
                if ((RawLoad16((uint8_t *)lpb + 0x2) & 0x7fff) < lpfl->cord) {
                    fExtra = (RawLoad16((uint8_t *)lpb + 0x2) & 0x8000) == 0 ? 0 : 1;
                    if (fExtra != 0 && iLook + 1 >= lpfl->cord) {
                        return 0;
                    }
                    fmemmove(&lpfl->lpplord->rgord[iLook], &lpfl->lpplord->rgord[iLook + fExtra + 1], (lpfl->cord - iLook - fExtra - 1) * sizeof(ORDER));
                    lpfl->cord -= fExtra + 1;
                    lpfl->lpplord->iordMac -= LOBYTE(fExtra + 1);
                    return 1;
                }
            }
            return 0;
        case 4:
            lpfl = LpflFromId(RawLoad16(lpb));
            if (lpfl == 0 || RawLoad16((uint8_t *)lpb + 0x2) < 0 || RawLoad16((uint8_t *)lpb + 0x2) > lpfl->cord) {
                return 0;
            }
            if (lpfl->cord == lpfl->lpplord->iordMax) {
                lpfl->lpplord = (PLORD *)LpplReAlloc((PL *)lpfl->lpplord, lpfl->cord + 3);
            }
            fmemmove(&lpfl->lpplord->rgord[RawLoad16((uint8_t *)lpb + 0x2) + 1], &lpfl->lpplord->rgord[RawLoad16((uint8_t *)lpb + 0x2)],
                     (lpfl->cord - RawLoad16((uint8_t *)lpb + 0x2)) * sizeof(ORDER));
            if ((uint16_t)cb < 22) {
                fmemset(&lpfl->lpplord->rgord[RawLoad16((uint8_t *)lpb + 0x2)], 0, sizeof(ORDER));
            }
            fmemmove(&lpfl->lpplord->rgord[RawLoad16((uint8_t *)lpb + 0x2)], lpb + 4, cb - 4);
            lpfl->lpplord->rgord[RawLoad16((uint8_t *)lpb + 0x2)].fNoAutoTrack = 0;
            lpfl->cord++;
            lpfl->lpplord->iordMac++;
            return 1;
        case 5:
            lpfl = LpflFromId(RawLoad16(lpb));
            if (lpfl != 0 && lpfl->cord >= 0) {
                iLook = RawLoad16((uint8_t *)lpb + 0x2);
                if (RawLoad16((uint8_t *)lpb + 0x2) < lpfl->cord) {
                    if ((uint16_t)cb < 22) {
                        fmemset(&lpfl->lpplord->rgord[iLook], 0, sizeof(ORDER));
                    }
                    fmemmove(&lpfl->lpplord->rgord[iLook], lpb + 4, cb - 4);
                    lpfl->lpplord->rgord[iLook].fNoAutoTrack = 0;
                    return 1;
                }
            }
            return 0;
        case 30:
            i = RawLoad16(lpb) >> 4 & 0xf;
            if ((RawLoad16(lpb) & 0xf) != idPlayer || i < 0 || i > rgcbtlplan[idPlayer]) {
                if ((RawLoad16(lpb) >> 0xe & 1) != 0) {
                    return 1;
                }
                break;
            }
            if ((RawLoad16(lpb) >> 0xe & 1) != 0) {
                FDeleteBattlePlan(i, 0);
                return 1;
            }
            if ((RawLoad16(lpb) >> 8 & 0xf) > 6 || (RawLoad16((uint8_t *)lpb + 0x2) & 0xf) > 8 || (RawLoad16((uint8_t *)lpb + 0x2) >> 4 & 0xf) > 8)
                break;
            if (i == rgcbtlplan[idPlayer]) {
                if (i >= 16)
                    break;
                rgcbtlplan[idPlayer]++;
            }
            UnpackBattlePlan(lpb, rglpbtlplan[idPlayer] + i, i);
            return 1;
        case 42:
            lpfl = LpflFromId(RawLoad16(lpb));
            if (lpfl == 0)
                break;
            lpfl->iplan = LOBYTE(RawLoad16((uint8_t *)lpb + 0x2));
            return 1;
        case 10:
        case 11:
            lpfl = LpflFromId(RawLoad16(lpb));
            if (lpfl == 0)
                break;
            if (rt == rtLogFleetFlagBit9) {
                lpfl->fRepOrders = RawLoad16((uint8_t *)lpb + 0x2);
                return 1;
            }
            if (lpfl->cord <= RawLoad16((uint8_t *)lpb + 0x2) || RawLoad16((uint8_t *)lpb + 0x4) >= 10)
                break;
            lpfl->lpplord->rgord[RawLoad16((uint8_t *)lpb + 0x2)].grTask = RawLoad16((uint8_t *)lpb + 0x4);
            return 1;
        case 35:
            lppl = LpplFromId(RawLoad16(lpb));
            if (lppl == 0 || lppl->iPlayer != idPlayer)
                break;
            lppl->fNoResearch = (uint32_t)RawLoad16((uint8_t *)lpb + 0x2) & 1;
            lppl->idFling = LOWORD((uint32_t)(RawLoad32((uint8_t *)lpb + 0x2) >> 1)) & 0x3ff;
            lppl->iWarpFling = LOWORD((uint32_t)(RawLoad32((uint8_t *)lpb + 0x2) >> 0xb)) & 0xf;
            lppl->idRoute = LOWORD((uint32_t)(RawLoad32((uint8_t *)lpb + 0x2) >> 0xf)) & 0x3ff;
            return 1;
        case 36:
            if (gd.fGeneratingTurn == 0) {
                return 1;
            }
            rgplr[idPlayer].lSalt = RawLoad32(lpb);
            return 1;
        case 34:
            ch = *lpb;
            if (ch < 0 || ch > 100)
                break;
            rgplr[idPlayer].pctResearch = ch;
            ch = lpb[1];
            if ((ch & 0xf) >= 6 || (ch >> 4 & 0xf) > 7)
                break;
            rgplr[idPlayer].iTechCur = ch;
        case 0:
        case 6:
        case 7:
        case 8:
        case 9:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 26:
        case 28:
        case 31:
        case 32:
        case 33:
        case 39:
        case 40:
        case 41:
        case 45:
            return 1;
        }
        return 0;
    }
    return 1;
}

int16_t FLoadLogFile(char *pszLog) {
    HGLOBAL  hres;
    jmp_buf *penvMemSav;
    jmp_buf  env;
    int16_t  fRet;
    int16_t  cbLog;
    int16_t  iCur;
    MSGPLR  *lpmp;
    HRSRC    hrsrc;
    int16_t  cSkip;

    fRet = 1;
    imemLogCur = 0;
    imemLogPrev = -1;
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0) {
        penvMem = penvMemSav;
        if (vlpMemStream != 0) {
            GlobalUnlock(hres);
            FreeResource(hres);
            return 0;
        }
        if (hf == -1) {
            return 1;
        }
        StreamClose();
        return 0;
    }
    if (game.fTutorial != 0 && idPlayer == 0 && gd.fGeneratingTurn != 0) {
        cSkip = game.turn;
        hrsrc = FindResource(hInst, MAKEINTRESOURCE(10001), MAKEINTRESOURCE(10000));
        hres = LoadResource(hInst, hrsrc);
        if (hres != 0) {
            vlpMemStream = LockResource(hres);
            if (vlpMemStream != 0) {
                if (game.turn >= *vlpMemStream) {
                    vlpMemStream = NULL;
                    GlobalUnlock(hres);
                    FreeResource(hres);
                    goto StrOpen;
                }
                vlpMemStream++;
                while (cSkip-- != 0) {
                    do {
                        vlpMemStream += 2 + (RawLoad16(vlpMemStream) & 0x3ff);
                    } while ((RawLoad16(vlpMemStream) >> 0xa & 0x3f) != 8);
                }
                goto L_c944;
            }
        }
        penvMem = penvMemSav;
        return 0;
    }
StrOpen:
    StreamOpen(pszLog, 16416);
L_c944:
    ReadRt();
    if (LOWORD(game.lid) == RawLoad16(&rgbCur[4]) && HIWORD(game.lid) == RawLoad16(&rgbCur[6]) && game.turn <= RawLoad16(&rgbCur[10])) {
        if (RawLoad16(&rgbCur[10]) != game.turn) {
            FileError(idmForcesDiedValiantlyTakingManyVerminThem);
        } else if ((RawLoad16(&rgbCur[14]) >> 0xd & 7) != game.wGen) {
            FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
        } else {
            wVersFile = RawLoad16(&rgbCur[8]);
            gd.fFileCrippled = RawLoad16(&rgbCur[14]) >> 0xc & 1;
            if (gd.fGeneratingTurn != 0) {
                rgplr[idPlayer].wFlags = (rgplr[idPlayer].wFlags & 0xfffd) | (RawLoad16(&rgbCur[14]) >> 0xc & 1 & 1) * 2;
            }
            ReadRt();
            cbLog = RawLoad16(rgbCur);
            if (gd.fGeneratingTurn != 0 && vrgts != 0) {
                fmemset(vrgts + idPlayer, 0, sizeof(TURNSERIAL));
                if (hdrCur.cb == 17) {
                    vrgts[idPlayer].lSerialNumber = RawLoad32(&rgbCur[2]);
                    fmemcpy(vrgts[idPlayer].rgbConfig, &rgbCur[6], 11);
                }
            }
            for (iCur = 0; iCur < cbLog; iCur += hdrCur.cb + 2) {
                ReadRt();
                fmemmove(lpLog + iCur, &hdrCur, sizeof(HDR));
                fmemmove(lpLog + (2 + iCur), rgbCur, hdrCur.cb);
            }
            ReadRt();
            for (lpmp = (MSGPLR *)&vlpmsgplrOut; lpmp->lpmsgplrNext != 0; lpmp = lpmp->lpmsgplrNext) {
            }
            while (hdrCur.rt == rtPlrMsg) {
                lpmp->lpmsgplrNext = LpAlloc(hdrCur.cb + (sizeof(MSGPLR) - 12), htPlrMsg);
                lpmp = lpmp->lpmsgplrNext;
                fmemcpy((uint8_t *)&lpmp->iPlrFrom - 4, rgbCur, hdrCur.cb);
                lpmp->lpmsgplrNext = NULL;
                vcmsgplrOut++;
                ReadRt();
            }
            if (hdrCur.rt != rtEOF) {
                fRet = 0;
            } else {
                imemLogCur = cbLog;
            }
            if (vlpMemStream != 0) {
                GlobalUnlock(hres);
                FreeResource(hres);
                vlpMemStream = NULL;
            } else {
                StreamClose();
            }
            penvMem = penvMemSav;
            DirtyGame(0);
            return fRet;
        }
    }
    if (vlpMemStream != 0) {
        vlpMemStream = NULL;
        GlobalUnlock(hres);
        FreeResource(hres);
    } else {
        StreamClose();
    }
    penvMem = penvMemSav;
    return 1;
}

int16_t FCheckLogFile(int16_t iplr, int16_t *pfError) {
    jmp_buf *penvMemSav;
    jmp_buf  env;
    int16_t  fRet;
    int16_t  cbLog;
    int16_t  iCur;

    fRet = 1;
    imemLogCur = 0;
    imemLogPrev = -1;
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0) {
        penvMem = penvMemSav;
        if (hf == -1) {
            return 1;
        }
        StreamClose();
        *pfError = 3;
        return 0;
    }
    idsFileError = 0;
    if (FOpenFile(dtLog, iplr, 32) == 0) {
        if (idsFileError != 4) {
            *pfError = idsFileError;
        }
        return 0;
    }
    ReadRt();
    cbLog = RawLoad16(rgbCur);
    for (iCur = 0; iCur < cbLog; iCur += hdrCur.cb + 2) {
        ReadRt();
    }
    ReadRt();
    while (hdrCur.rt == rtPlrMsg) {
        ReadRt();
    }
    if (hdrCur.rt != rtEOF) {
        *pfError = 3;
        fRet = 0;
    } else {
        imemLogCur = cbLog;
    }
    StreamClose();
    penvMem = penvMemSav;
    return fRet;
}

int16_t FWriteLogFile(char *pszFileBase, int16_t iPlayer) {
    jmp_buf *penvMemSav;
    jmp_buf  env;
    int16_t  iCur;
    HDR     *lprts;
    RTLOGHDR rtlh;
    MSGPLR  *lpmp;
    int16_t  cb;

    iCur = 0;
    if (iPlayer == idPlayer && rgplr[iPlayer].fAi == 0 && hdrPrev.rt != rtLogPlayerZpq1) {
        cb = 26 - (12 - vrgZipProd[0].cpq) * 2;
        if (memcmp(&rgplr[iPlayer].zpq1, (uint8_t *)(ZIPPRODQ *)vrgZipProd + 14, cb) != 0) {
            WriteMemRt(46, cb, (uint8_t *)(ZIPPRODQ *)vrgZipProd + 14);
        }
    }
    strcpy(szBase, pszFileBase);
    if (FCreateFile(dtLog, iPlayer, NULL) == 0) {
        AlertSz(PszFormatIds(idsUnableCreateLogFile, NULL), MB_ICONHAND);
        return 0;
    }
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0) {
        penvMem = penvMemSav;
        StreamClose();
        return 0;
    }
    rtlh.cbLog = imemLogCur;
    rtlh.lSerialNumber = vSerialNumber;
    memcpy(rtlh.rgbConfig, vrgbEnvCur, 11);
    WriteRt(9, 17, &rtlh);
    for (; iCur < imemLogCur; iCur += lprts->cb + 2) {
        lprts = (HDR *)(lpLog + iCur);
        WriteRt(lprts->rt, lprts->cb, lpLog + (2 + iCur));
    }
    iCur = vcmsgplrOut;
    lpmp = vlpmsgplrOut;
    while (iCur-- != 0) {
        WriteRt(rtPlrMsg, abs(lpmp->cLen) + 12, (uint8_t *)&lpmp->iPlrFrom - 4);
        lpmp = lpmp->lpmsgplrNext;
    }
    WriteRt(rtEOF, 0, NULL);
    StreamClose();
    penvMem = penvMemSav;
    DirtyGame(0);
    gd.fWriteTurnNum = 1;
    return 1;
}

int16_t FWriteTutorialMFile(int16_t iTurn) {
    HRSRC    hrsrc;
    char     szT[30];
    HGLOBAL  hres;
    jmp_buf *penvMemSav;
    jmp_buf  env;
    int16_t  cch;
    int16_t  cSkip;

    cSkip = iTurn;
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0) {
        penvMem = penvMemSav;
        if (vlpMemStream != 0) {
            GlobalUnlock(hres);
            FreeResource(hres);
            return 0;
        }
        if (hf == -1) {
            return 1;
        }
        StreamClose();
        return 0;
    }
    if (iTurn < 32) {
        hrsrc = FindResource(hInst, MAKEINTRESOURCE(10003), MAKEINTRESOURCE(10002));
    } else {
        hrsrc = FindResource(hInst, MAKEINTRESOURCE(10005), MAKEINTRESOURCE(10004));
        cSkip -= 32;
    }
    hres = LoadResource(hInst, hrsrc);
    if (hres != 0) {
        vlpMemStream = LockResource(hres);
        if (vlpMemStream != 0) {
            if (cSkip >= *vlpMemStream) {
                vlpMemStream = NULL;
                GlobalUnlock(hres);
                FreeResource(hres);
                penvMem = penvMemSav;
                return 2;
            }
            vlpMemStream++;
            while (cSkip-- != 0) {
                do {
                    vlpMemStream += 2 + (RawLoad16(vlpMemStream) & 0x3ff);
                } while ((RawLoad16(vlpMemStream) >> 0xa & 0x3f) != 8);
            }
            cch = CchGetString(idsTutorial, szT);
            strcpy(&szT[cch], iTurn == 37 ? ".hst" : ".m1");
            StreamOpen(szT, 4114);
            do {
                RgToStream(vlpMemStream, (RawLoad16(vlpMemStream) & 0x3ff) + 2);
                vlpMemStream += 2 + (RawLoad16(vlpMemStream) & 0x3ff);
            } while ((RawLoad16(vlpMemStream) >> 0xa & 0x3f) != 8);
            StreamClose();
            vlpMemStream = NULL;
            GlobalUnlock(hres);
            FreeResource(hres);
            penvMem = penvMemSav;
            return 1;
        }
    }
    penvMem = penvMemSav;
    return 0;
}

int16_t FWriteHistFile(int16_t iPlayer) {
    PLANET   *lppl;
    int16_t   i;
    jmp_buf  *penvMemSav;
    jmp_buf   env;
    uint16_t  cTurnBase;
    SHDEF    *lpshdef;
    int16_t   j;
    RTHISTHDR rthh;
    uint8_t  *lpb;

    if (FCreateFile(dtHist, iPlayer, NULL) == 0) {
        AlertSz(PszFormatIds(idsUnableCreateHistoryFile, NULL), MB_ICONHAND);
        return 0;
    }
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0) {
        penvMem = penvMemSav;
        StreamClose();
        return 0;
    }
    rthh.cPlanet = cPlanet;
    rthh.cPlanetExtra = rgplr[iPlayer].cFleet;
    WriteRt(rtHistHdr, 4, &rthh);
    i = 0;
    lppl = lpPlanets;
    while (i < cPlanet) {
        WritePlanet(lppl, lppl->det < detSome ? 0xf : rtPlanetB, 1);
        i++;
        lppl++;
    }
    WriteRt(rtMsgFilt, cbbitfMsg, bitfMsgFiltered);
    for (i = 0; i < game.cPlayer; i++) {
        if (i != iPlayer && rgplr[i].det != detNone) {
            WriteRtPlr(&rgplr[i], NULL);
        }
    }
    for (i = 0; i < game.cPlayer; i++) {
        if (rgplr[i].fInclude != 0 && i != iPlayer) {
            lpshdef = rglpshdef[i];
            for (j = 0; j < 16; j++) {
                if (lpshdef[j].fFree == 0) {
                    WriteRtShDef(lpshdef + j, NULL);
                }
            }
        }
    }
    for (i = 0; i < game.cPlayer; i++) {
        if (rgplr[i].fInclude != 0 && i != iPlayer) {
            lpshdef = rglpshdefSB[i];
            for (j = 0; j < 10; j++) {
                if (lpshdef[j].fFree == 0) {
                    WriteRtShDef(lpshdef + j, NULL);
                }
            }
        }
    }
    if (game.turn <= 100) {
        cTurnBase = 0;
    } else {
        cTurnBase = game.turn - 100;
    }
    for (i = 0; i < game.cPlayer; i++) {
        if (rgsxPlr[i] != 0) {
            for (j = 0; j < rgcsxPlr[i]; j++) {
                if (rgsxPlr[i][j].turn >= cTurnBase) {
                    WriteRt(rtScore, 24, rgsxPlr[i] + j);
                }
            }
        }
    }
    if (vlpbAiData != 0 && RawLoad16(vlpbAiData) > 2) {
        i = RawLoad16(vlpbAiData);
        lpb = vlpbAiData;
        for (; i >= 1024; i -= 1023) {
            WriteRt(rtAiData, 1023, lpb);
            lpb += 1023;
        }
        WriteRt(rtAiData, i, lpb);
    }
    WriteRt(rtEOF, 0, NULL);
    StreamClose();
    penvMem = penvMemSav;
    return 1;
}

void EnumLogRts(int16_t (*pfn)(void *, int16_t, int16_t, void *, int16_t), void *lpPass, int16_t iPass) {
    int16_t fLogOld;
    int16_t fRet;
    int16_t iCur;
    HDR    *lprts;

    iCur = 0;
    fRet = 1;
    fLogOld = fLogOff;
    if (imemLogCur != 0) {
        for (; iCur < imemLogCur; iCur += lprts->cb + 2) {
            lprts = (HDR *)(lpLog + iCur);
            if (pfn(lpLog + (2 + iCur), lprts->rt, lprts->cb, lpPass, iPass) == 0)
                break;
        }
    }
    return;
}
