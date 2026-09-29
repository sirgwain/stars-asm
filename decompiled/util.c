#include "common.h"

uint32_t rgcrDrawStars2b[5] = {8355711, 127, 32512, 8323072};
uint32_t rgcrDrawStars2a[5] = {12632256, 255, 65280, 16711680};
uint32_t rgcrDrawStars[5] = {8355711, 16777215, 255, 65280, 16711680};
int32_t  rgDSDivCnt2[5] = {80000, 210000, 310000, 260000};
int32_t  rgDSDivCnt[5] = {28000, 28000, 63000, 95000, 73000};
uint8_t  vrgbTachyon[18] = {100, 95, 93, 91, 90, 89, 88, 87, 86, 86, 85, 84, 84, 83, 83, 82, 82, 81};

int16_t FLookupSelPlanet(PLANET *ppl) {
    if (sel.scan.grobj == grobjPlanet) {
        return FLookupPlanet(sel.scan.idpl, ppl);
    }
    return 0;
}

int16_t FDupPlanet(PLANET *lppl, PLANET *ppl) {
    PLPROD  *lpplprodT;
    uint16_t t_scratch_m8;

    lpplprodT = ppl->lpplprod;
    *ppl = *lppl;
    ppl->lpplprod = lpplprodT;
    if (lppl->lpplprod != 0x0) {
        if (ppl->lpplprod != 0x0) {
            t_scratch_m8 = ppl->lpplprod->iprodMax;
            if (t_scratch_m8 < lppl->lpplprod->iprodMac) {
                ppl->lpplprod = (PLPROD *)LpplReAlloc((PL *)ppl->lpplprod, lppl->lpplprod->iprodMax);
            }
        } else {
            ppl->lpplprod = (PLPROD *)LpplAlloc(0x4, lppl->lpplprod->iprodMax, htOrd);
        }
        fmemcpy(ppl->lpplprod->rgprod, lppl->lpplprod->rgprod, lppl->lpplprod->iprodMac * 0x4);
        ppl->lpplprod->iprodMac = lppl->lpplprod->iprodMac;
        return 1;
    }
    if (ppl->lpplprod != 0x0) {
        FreePl((PL *)ppl->lpplprod);
        ppl->lpplprod = 0x0;
    }
    return 1;
}

THING *LpthFromId(int16_t idth) {
    THING *lpth;
    THING *lpthMac;

    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (lpth->idFull == idth) {
            return lpth;
        }
    }
    return 0x0;
}

PLANET *LpplFromId(int16_t idPlanet) {
    int16_t idGuess;
    int16_t iLo;
    PLANET *lppl;
    int16_t iGuess;
    int16_t iHi;

    if (idPlanet >= 0 && idPlanet < game.cPlanMax) {
        if (cPlanet != game.cPlanMax) {
            iLo = -1;
            iHi = cPlanet;
            while (1) {
                if (iLo + 1 >= iHi) {
                    return 0x0;
                }
                iGuess = (iLo + iHi) >> 0x1;
                lppl = lpPlanets + iGuess;
                idGuess = lpPlanets[iGuess].id;
                if (idGuess >= idPlanet) {
                    if (idGuess <= idPlanet)
                        break;
                    iHi = iGuess;
                } else {
                    iLo = iGuess;
                }
            }
            return lppl;
        }
        lppl = lpPlanets + idPlanet;
        return lppl;
    }
    return 0x0;
}

void CalcPctSurvive(PLANET *lppl, float *ppct, float *ppctSmart) {
    int16_t iPlrSav;
    int32_t cDefenses;
    float   pct;
    PART    part;
    int16_t cMax;

    if (ppctSmart != 0x0) {
        *ppctSmart = 1.0;
    }
    if (lppl->iPlayer != -1 && lppl->cDefenses != 0x0) {
        iPlrSav = idPlayer;
        idPlayer = lppl->iPlayer;
        if (FGetBestDefensePart(&part) == 0) {
            pct = 1.0;
        } else {
            cDefenses = lppl->cDefenses;
            cMax = CMaxOperableDefenses(lppl, lppl->iPlayer, 0);
            if ((int32_t)cMax < cDefenses) {
                cDefenses = (int32_t)cMax;
            }
            pct = pow(1.0 - (double)(int32_t)part.pplanetary->grAbility / 1000.0, (double)cDefenses);
            if (ppctSmart != 0x0) {
                *ppctSmart = pow(1.0 - (double)(int32_t)part.pplanetary->grAbility / 2000.0, (double)cDefenses);
            }
        }
        idPlayer = iPlrSav;
    } else {
        pct = 1.0;
    }
    *ppct = pct;
    return;
}

int16_t FLookupPlanet(int16_t iPlanet, PLANET *ppl) {
    PLANET  *lpPl;
    int16_t  fWrite;
    uint16_t t_scratch_ma;

    fWrite = 0;
    if (cPlanet > 0) {
        if (iPlanet < 0) {
            iPlanet = ppl->id;
            if (iPlanet == -1) {
                LogChangePlanet(0x0, ppl);
                return 1;
            }
            fWrite = 1;
        }
        lpPl = LpplFromId(iPlanet);
        if (lpPl != 0x0 && ppl != 0x0) {
            if (fWrite == 0) {
                if (ppl != &sel.pl) {
                    *ppl = *lpPl;
                } else {
                    FDupPlanet(lpPl, ppl);
                }
            } else {
                InvalidateReport(0, 0);
                LogChangePlanet(lpPl, ppl);
                if (lpPl->lpplprod != ppl->lpplprod) {
                    if (lpPl->lpplprod != 0x0) {
                        if (ppl->lpplprod == 0x0) {
                            FreePl((PL *)lpPl->lpplprod);
                            lpPl->lpplprod = 0x0;
                            goto FinishCopy;
                        }
                    } else if (ppl->lpplprod != 0x0) {
                        lpPl->lpplprod = (PLPROD *)LpplAlloc(0x4, ppl->lpplprod->iprodMac, htOrd);
                    }
                    t_scratch_ma = lpPl->lpplprod->iprodMax;
                    if (t_scratch_ma < ppl->lpplprod->iprodMac) {
                        lpPl->lpplprod = (PLPROD *)LpplReAlloc((PL *)lpPl->lpplprod, ppl->lpplprod->iprodMac + 0x2);
                    }
                    fmemcpy(lpPl->lpplprod->rgprod, ppl->lpplprod->rgprod, ppl->lpplprod->iprodMac * 0x4);
                    lpPl->lpplprod->iprodMac = ppl->lpplprod->iprodMac;
                }
            FinishCopy:
                fmemcpy(lpPl, ppl, 0x34);
                if (gd.fTutorial != 0x0 && idPlayer == 0) {
                    AdvanceTutor();
                }
            }
        }
        if (lpPl != 0x0) {
            return 1;
        }
        return 0;
    }
    return 0;
}

int32_t DpOfLpflIshdef(FLEET *lpfl, int16_t ishdef) {
    int16_t dpShdef;
    int32_t dp;

    dpShdef = rglpshdef[lpfl->iPlayer][ishdef].hul.dp;
    dp =
        (int32_t)((int32_t)((int32_t)lpfl->rgcsh[ishdef] * (int32_t)((int32_t)(lpfl->rgdv[ishdef].pctSh * dpShdef) / 0xa * lpfl->rgdv[ishdef].pctDp)) / 0x1388);
    return dp;
}

int16_t FLookupThing(int16_t idth, THING *pth) {
    THING  *lpth;
    int16_t fWrite;

    fWrite = 0;
    if (cThing > 0) {
        if (idth < 0) {
            idth = pth->idFull;
            fWrite = 1;
        }
        lpth = LpthFromId(idth);
        if (lpth != 0x0 && pth != 0x0) {
            if (fWrite == 0) {
                *pth = *lpth;
            } else {
                LogChangeThing(lpth, pth);
                fmemcpy(lpth, pth, sizeof(THING));
                if (gd.fTutorial != 0x0 && idPlayer == 0) {
                    AdvanceTutor();
                }
            }
        }
        if (lpth != 0x0) {
            return 1;
        }
        return 0;
    }
    return 0;
}

void SelectOursAtObject(POINT16 *ppt) {
    int16_t id;
    POINT16 pt;
    int16_t ish;
    int16_t i;
    FLEET  *lpfl;
    SCAN    scan;

    if (ppt->x != -1) {
        pt.x = ppt->x;
        pt.y = ppt->y;
    } else {
        if ((ppt->y & 0x8000) != 0x0) {
            SelectAdjFleet(0, ppt->y & 0x7fff);
            return;
        }
        pt = rgptPlan[ppt->y];
    }
    id = -1;
    ish = 0;
    while (1) {
        if (ish >= cFleet)
            goto L_0a01;
        lpfl = rglpfl[ish];
        if (rglpfl[ish] == 0x0)
            goto L_0a01;
        if (pt.x == lpfl->pt.x && pt.y == lpfl->pt.y) {
            if (lpfl->iPlayer == idPlayer)
                break;
            if (id == -1) {
                id = lpfl->id;
            }
        }
        ish = ish + 1;
    }
    SelectAdjFleet(0, lpfl->id);
    return;
L_0a01:
    i = 0;
    while (1) {
        if (i >= game.cPlanMax)
            goto L_0a4c;
        if (rgptPlan[i].x == pt.x && rgptPlan[i].y == pt.y)
            break;
        i = i + 1;
    }
    SelectAdjPlanet(0, i);
    return;
L_0a4c:
    scan.iwp = -1;
    if (FFindNearestObject(pt, grobjThing, &scan) == 0 || scan.grobj != grobjThing || scan.ith == -1 || lpThings[scan.ith].ith != ithMineralPacket ||
        lpThings[scan.ith].thp.iWarp != 0x0) {
        if (id != -1) {
            SelectAdjFleet(0, id);
        }
    } else {
        ChangeScanSel(&scan, 1);
        FEnsurePointOnScreen(scan.pt, 1);
        UpdateWindow(hwndScanner);
        SendMessage(hwndScanner, WM_CHAR, 0x76, 0);
    }
    return;
}

int32_t LComputePower(SHDEF *lpshdef) {
    int16_t dSpeed;
    int16_t dxRange;
    int16_t ihs;
    int32_t dpTorps;
    int16_t i;
    int32_t pctCap;
    int32_t dpBeams;
    int32_t dpBombs;
    int32_t dp;
    PART    part;

    dpBombs = 0;
    dpBeams = 0;
    dpTorps = 0;
    dxRange = 999;
    pctCap = 1000;
    for (ihs = 0; ihs < lpshdef->hul.chs; ihs++) {
        part.hs = lpshdef->hul.rghs[ihs];
        if (part.hs.cItem != 0x0 && FLookupPart(&part) != 0) {
            switch (part.hs.grhst) {
            default:
                break;
            case hstBeam:
                dp = (int32_t)((int32_t)((uint32_t)((int32_t)part.pbeam->dp * part.hs.cItem) * (int32_t)(part.pbeam->dRangeMax + 3)) / 0x4);
                if ((part.pbeam->grfAbilities & 0x1) != 0x0) {
                    dp = (int32_t)(dp / 3);
                }
                dpBeams = dpBeams + dp;
                break;
            case hstTorp:
                dpTorps = dpTorps + (int32_t)((int32_t)((uint32_t)((int32_t)part.ptorp->dp * part.hs.cItem) * (int32_t)(part.ptorp->dRangeMax - 2)) / 0x2);
                break;
            case hstBomb:
                dpBombs = dpBombs + (uint32_t)((part.pbomb->dDmgCol + part.pbomb->dDmgBldg) * part.hs.cItem * 0x2);
                break;
            case hstSpecialE:
                if (part.hs.iItem == ispecialEEnergyCapacitor || part.hs.iItem == ispecialEFluxCapacitor) {
                    for (i = part.hs.cItem; i > 0; i--) {
                        pctCap = (int32_t)((int32_t)(pctCap * ((int32_t)part.pspecial->grAbility + 100)) / 0x64);
                    }
                }
            }
        }
    }
    if (pctCap != 1000) {
        pctCap = (int32_t)(pctCap / 10);
        if (pctCap > 255) {
            pctCap = 255;
        }
        dpBeams = (int32_t)((int32_t)(dpBeams * pctCap) / 100);
    }
    dSpeed = SpdOfShip(0x0, 0, 0x0, 0, lpshdef);
    dpBeams = dpBeams + (int32_t)((int32_t)(dpBeams * (int32_t)(dSpeed - 4)) / 0xa);
    return dpBombs + dpBeams + dpTorps;
}

void ComputeShdefPowers() {
    int16_t iplr;
    int16_t ishdef;
    int32_t t_call_0ed7;

    for (iplr = 0; iplr < game.cPlayer; iplr++) {
        if (rglpshdef[iplr] != 0x0) {
            for (ishdef = 0; ishdef < 16; ishdef++) {
                if (rglpshdef[iplr][ishdef].fFree == 0x0) {
                    t_call_0ed7 = LComputePower(rglpshdef[iplr] + ishdef);
                    rglpshdef[iplr][ishdef].lPower = t_call_0ed7;
                }
            }
        }
    }
    return;
}

int32_t DpShieldOfShdef(SHDEF *lpshdef, int16_t iplr) {
    int16_t  chs;
    HS      *lphs;
    int16_t  ihs;
    int32_t  dpShdef;
    HUL     *lphul;
    PART     part;
    HS      *t_fields_1;
    uint32_t t_fields_2;
    uint32_t t_fields_3;

    dpShdef = 0;
    lphul = &lpshdef->hul;
    lphs = lphul->rghs;
    chs = lphul->chs;
    ihs = 0;
    while (ihs < chs) {
        if (lphs->grhst != hstShield || lphs->cItem <= 0x0) {
            if (lphs->grhst != hstArmor || lphs->cItem <= 0x0 || lphs->iItem != iarmorFieldedKelarium) {
                if (lphs->grhst == hstArmor && lphs->iItem == iarmorMegaPolyShell) {
                    dpShdef = dpShdef + (uint32_t)(lphs->cItem * 0x64);
                }
            } else {
                dpShdef = dpShdef + (uint32_t)(lphs->cItem * 0x32);
            }
        } else {
            part.hs.grhst = lphs->grhst;
            t_fields_1 = &part.hs;
            t_fields_2 = lphs->iItem;
            t_fields_3 = lphs->cItem;
            t_fields_1->iItem = t_fields_2;
            t_fields_1->cItem = t_fields_3;
            FLookupPart(&part);
            dpShdef = dpShdef + (uint32_t)(part.pshield->dp * lphs->cItem);
        }
        ihs = ihs + 1;
        lphs = lphs + 1;
    }
    if (GetRaceGrbit(&rgplr[iplr], ibitRaceRegeneratingShields) != 0) {
        dpShdef = dpShdef + (int32_t)((int32_t)(dpShdef * 2) / 5);
    }
    if ((dpShdef & 0xffff0000) != 0x0) {
        dpShdef = 65535;
    }
    return (uint32_t)LOWORD(dpShdef);
}

int16_t IStargateFromLppl(PLANET *lppl) {
    int16_t chs;
    HS     *lphs;
    int16_t ihs;
    HUL    *lphul;

    if (lppl != 0x0 && lppl->fStarbase != 0x0) {
        lphul = &rglpshdefSB[lppl->iPlayer][lppl->isb].hul;
        lphs = lphul->rghs;
        chs = lphul->chs;
        ihs = 0;
        while (1) {
            if (ihs >= chs) {
                return -1;
            }
            if (lphs->grhst == hstSpecialSB && lphs->cItem > 0x0 && lphs->iItem < ispecialSBMassDriver5)
                break;
            ihs = ihs + 1;
            lphs = lphs + 1;
        }
        return lphs->iItem;
    }
    return -1;
}

char *PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr) {
    char *pchEnd;
    char  szName[50];
    char *t_12c7;

    if (pplr == 0x0) {
        pplr = &rgplr[iPlayer];
    }
    if ((int16_t)pplr->szName[0] == 0) {
        _wsprintf(szName, PszGetCompressedString(idsPlayerD2), iPlayer + 1);
        if (fPlural == 0) {
            strcat(szName, "'s");
        }
        if (grWord != 1) {
            if (grWord == 2) {
                CchGetString(idsIs2, &szName[strlen(szName)]);
            }
        } else {
            CchGetString(idsHas, &szName[strlen(szName)]);
        }
    } else {
        if (fThe == 0) {
            szName[0] = 0;
        } else {
            strcpy(szName, "the ");
            if (fCapital != 0) {
                szName[0] = 'T';
            }
        }
        if (fPlural == 0 || (int16_t)pplr->szNames[0] == 0) {
            strcat(szName, pplr->szName);
        } else {
            strcat(szName, pplr->szNames);
        }
        pchEnd = &szName[strlen(szName) - 1];
        while ((int16_t)*pchEnd == ' ' && pchEnd >= szName) {
            t_12c7 = pchEnd;
            pchEnd = pchEnd - 1;
            *t_12c7 = 0;
        }
        if (pchEnd < szName) {
            CchGetString(idsName, szName);
        }
        if (fPlural != 0 && (int16_t)pplr->szNames[0] == 0) {
            pchEnd = &szName[strlen(szName) - 1];
            if ((int16_t)*pchEnd != 's' && ((int16_t)*pchEnd != 'e' || (int16_t)pchEnd[-1] != 's')) {
                strcat(szName, "s");
            }
        }
        if (grWord != 1) {
            if (grWord == 2) {
                CchGetString(idsAre, &szName[strlen(szName)]);
            }
        } else {
            CchGetString(idsHave2, &szName[strlen(szName)]);
        }
    }
    strcpy(szWork, szName);
    return szWork;
}

int16_t FCalcFleetBombDamage(FLEET *lpfl, int32_t *pdmgPeople, int32_t *pdmgPeopleMin, int32_t *pdmgPeopleSmart, int32_t *pdmgBldg, int32_t *ppctTerra,
                             int16_t *pfMulti) {
    int16_t iplr;
    int16_t cfl;
    FLEET  *lpflNext;
    int16_t dmgFloor;
    FLEET  *lpflHead;
    double  dmgSmart;
    int16_t fBomber;
    int16_t j;
    int16_t ishdef;
    PART    part;
    int32_t cIter;
    double  dmgT;
    int32_t t_1675;

    iplr = lpfl->iPlayer;
    cfl = 0;
    dmgSmart = 1.0;
    lpflHead = lpfl;
    *ppctTerra = 0;
    *pdmgPeopleMin = 0;
    *pdmgBldg = 0;
    *pdmgPeopleSmart = 0;
    *pdmgPeople = 0;
    while (lpfl != 0x0) {
        fBomber = 0;
        lpfl->fBombed = 0x1;
        for (ishdef = 0; ishdef < 16; ishdef++) {
            if (lpfl->rgcsh[ishdef] > 0) {
                for (j = 0; j < rglpshdef[iplr][ishdef].hul.chs; j++) {
                    if (rglpshdef[iplr][ishdef].hul.rghs[j].grhst != hstBomb) {
                        if (rglpshdef[iplr][ishdef].hul.rghs[j].grhst != hstBeam || rglpshdef[iplr][ishdef].hul.rghs[j].iItem != 0x12) {
                            if (rglpshdef[iplr][ishdef].hul.rghs[j].grhst == hstSpecialM && rglpshdef[iplr][ishdef].hul.rghs[j].iItem == 0x1) {
                                part.hs = rglpshdef[iplr][ishdef].hul.rghs[j];
                                fBomber = 1;
                                *pdmgPeopleMin = *pdmgPeopleMin + (uint32_t)((uint32_t)(part.hs.cItem * (int32_t)lpfl->rgcsh[ishdef]) * 0x14);
                            }
                        } else {
                            part.hs = rglpshdef[iplr][ishdef].hul.rghs[j];
                            fBomber = 1;
                            *pdmgPeople = *pdmgPeople + (uint32_t)((uint32_t)(part.hs.cItem * (int32_t)lpfl->rgcsh[ishdef]) * 0x14);
                            *pdmgBldg = *pdmgBldg + (uint32_t)((uint32_t)(part.hs.cItem * (int32_t)lpfl->rgcsh[ishdef]) * 0x5);
                            *pdmgPeopleMin = *pdmgPeopleMin + (uint32_t)((uint32_t)(part.hs.cItem * (int32_t)lpfl->rgcsh[ishdef]) * 0x3);
                        }
                    } else {
                        part.hs = rglpshdef[iplr][ishdef].hul.rghs[j];
                        FLookupPart(&part);
                        fBomber = 1;
                        if (part.hs.iItem != 0x9) {
                            if (part.hs.cItem > 0x0) {
                                if (part.pbomb->dDmgBldg != 0) {
                                    *pdmgPeople =
                                        *pdmgPeople + (uint32_t)((uint32_t)(part.hs.cItem * (int32_t)lpfl->rgcsh[ishdef]) * (int32_t)part.pbomb->dDmgCol);
                                    *pdmgBldg =
                                        *pdmgBldg + (uint32_t)((uint32_t)(part.hs.cItem * (int32_t)lpfl->rgcsh[ishdef]) * (int32_t)part.pbomb->dDmgBldg);
                                    if (part.hs.iItem >= ibombLadyFingerBomb && part.hs.iItem <= ibombCherryBomb) {
                                        dmgFloor = 3;
                                        *pdmgPeopleMin =
                                            *pdmgPeopleMin + (uint32_t)((uint32_t)(part.hs.cItem * (int32_t)lpfl->rgcsh[ishdef]) * (int32_t)dmgFloor);
                                    }
                                } else {
                                    cIter = (uint32_t)(part.hs.cItem * (int32_t)lpfl->rgcsh[ishdef]);
                                    dmgT = 1.0 - (double)(int32_t)part.pbomb->dDmgCol / 1000.0;
                                    while (1) {
                                        t_1675 = cIter;
                                        cIter = cIter - 1;
                                        if (t_1675 <= 0)
                                            break;
                                        dmgSmart = dmgSmart * dmgT;
                                    }
                                }
                            }
                        } else {
                            *ppctTerra = *ppctTerra + (uint32_t)(part.hs.cItem * (int32_t)lpfl->rgcsh[ishdef]);
                        }
                    }
                }
            }
        }
        cfl = cfl + 1;
        for (lpflNext = lpfl->lpflNext; lpflNext != 0x0 && (lpflNext->fDead != 0x0 || lpflNext->iPlayer != iplr); lpflNext = lpflNext->lpflNext) {
        }
        if (lpflNext == 0x0 || lpflNext == lpflHead || lpflNext->idPlanet != lpfl->idPlanet) {
            lpfl = 0x0;
        } else {
            lpfl = lpflNext;
        }
    }
    *pdmgPeopleSmart = (int32_t)(1000.0 - dmgSmart * 1000.0 + 0.5);
    if (*pdmgPeopleSmart >= 1000) {
        *pdmgPeopleSmart = 1000;
    }
    *pfMulti = cfl <= 1 ? 0 : 1;
    if (*pdmgPeople != 0 || *pdmgPeopleMin != 0 || *pdmgPeopleSmart != 0 || *pdmgBldg != 0 || *ppctTerra != 0) {
        return 1;
    }
    return 0;
}

void LinkFleets(int16_t fUnused) {
    FLEET **pSearch;
    POINT16 pt;
    FLEET  *rglpflSrc[63];
    FLEET  *lpflTail;
    FLEET  *lpflHead;
    int16_t i;
    int16_t iflTail;
    int16_t iflHead;
    int16_t cSrc;

    for (iflHead = 0; iflHead < cFleet; iflHead++) {
        lpflHead = rglpfl[iflHead];
        if (rglpfl[iflHead] == 0x0)
            break;
        lpflHead->lpflNext = 0x0;
        lpflHead->fDone = 0x0;
    }
    cSrc = 0;
    for (iflHead = 0; iflHead < cFleet; iflHead++) {
        lpflHead = rglpfl[iflHead];
        if (lpflHead->fDead == 0x0 && lpflHead->lpflNext == 0x0) {
            i = 0;
            while (1) {
                if (i >= cSrc)
                    goto L_1d61;
                if (lpflHead->pt.y <= rglpflSrc[i]->pt.y && (lpflHead->pt.y < rglpflSrc[i]->pt.y || (uint16_t)lpflHead->pt.x <= (uint16_t)rglpflSrc[i]->pt.x))
                    break;
                i = i + 1;
            }
            if (lpflHead->pt.x == rglpflSrc[i]->pt.x && lpflHead->pt.y == rglpflSrc[i]->pt.y) {
                lpflHead->lpflNext = rglpflSrc[i]->lpflNext;
                rglpflSrc[i]->lpflNext = lpflHead;
                i = -1;
            }
        L_1d61:
            if (i != -1) {
                lpflHead->lpflNext = lpflHead;
                if (i < cSrc) {
                    memmove(&rglpflSrc[i + 1], &rglpflSrc[i], (cSrc - i) * sizeof(FLEET *));
                }
                rglpflSrc[i] = lpflHead;
                cSrc = cSrc + 1;
                if (cSrc >= 63) {
                    for (iflTail = iflHead + 1; iflTail < cFleet; iflTail++) {
                        lpflTail = rglpfl[iflTail];
                        if (lpflTail->fDead == 0x0 && lpflTail->lpflNext == 0x0) {
                            pt = lpflTail->pt;
                            pSearch = bsearch(&pt, rglpflSrc, cSrc, 0x4, (QSORTCOMPARE)ICompFleetPoint2);
                            if (pSearch != 0x0) {
                                lpflHead = *pSearch;
                                lpflTail->lpflNext = lpflHead->lpflNext;
                                lpflHead->lpflNext = lpflTail;
                            }
                        }
                    }
                    cSrc = 0;
                }
            }
        }
    }
    gd.fFleetLinkValid = 0x1;
    return;
}

int ICompFleetPoint(FLEET **ppfl1, FLEET **ppfl2) {
    int32_t l2;
    int32_t l1;

    l1 = ((uint32_t)(*ppfl1)->pt.x & 0xffff) | ((uint32_t)(*ppfl1)->pt.y & 0xffff) << 0x10;
    l2 = ((uint32_t)(*ppfl2)->pt.x & 0xffff) | ((uint32_t)(*ppfl2)->pt.y & 0xffff) << 0x10;
    l1 = l1 - l2;
    if (l1 < 0) {
        l1 = -1;
    } else if (l1 > 0) {
        l1 = 1;
    }
    return (int16_t)LOWORD(l1);
}

int ICompFleetPoint2(int32_t *pl, FLEET **ppfl) {
    int32_t l2;
    int32_t l1;

    l1 = *pl;
    l2 = ((uint32_t)(*ppfl)->pt.x & 0xffff) | ((uint32_t)(*ppfl)->pt.y & 0xffff) << 0x10;
    l1 = l1 - l2;
    if (l1 < 0) {
        l1 = -1;
    } else if (l1 > 0) {
        l1 = 1;
    }
    return (int16_t)LOWORD(l1);
}

int16_t FLookupSelShip(FLEET *pfl) {
    if (sel.scan.grobj == grobjFleet) {
        return FLookupFleet(rglpfl[sel.scan.ifl]->id, pfl);
    }
    return 0;
}

FLEET *LpflFromId(int16_t idFleet) {
    int16_t iplr;
    int16_t idGuess;
    int16_t iLo;
    int16_t iGuess;
    int16_t i;
    FLEET  *lpfl;
    int16_t iplrCur;
    int16_t iHi;

    i = 0;
    iplr = (uint16_t)idFleet >> 0x9 & 0xf;
    idFleet = idFleet & 0x1fff;
    for (iplrCur = 0; iplrCur < iplr; iplrCur++) {
        i = i + rgplr[iplrCur].cFleet;
    }
    iLo = i - 1;
    iHi = cFleet;
    while (1) {
        if (iLo + 1 >= iHi) {
            return 0x0;
        }
        iGuess = (iLo + iHi) >> 0x1;
        lpfl = rglpfl[iGuess];
        idGuess = rglpfl[iGuess]->id;
        if (idGuess >= idFleet) {
            if (idGuess <= idFleet)
                break;
            iHi = iGuess;
        } else {
            iLo = iGuess;
        }
    }
    return lpfl;
}

int16_t FLookupFleet(int16_t idFleet, FLEET *pfl) {
    FLEET  *lpfl;
    int16_t fWrite;

    fWrite = 0;
    if (cFleet > 0) {
        if (idFleet < 0) {
            idFleet = pfl->id;
            fWrite = 1;
        }
        lpfl = LpflFromId(idFleet);
        if (lpfl != 0x0 && pfl != 0x0) {
            if (fWrite == 0) {
                if (pfl != &sel.fl) {
                    *pfl = *lpfl;
                } else {
                    FDupFleet(lpfl, pfl);
                }
            } else {
                InvalidateReport(1, 0);
                LogChangeFleet(lpfl, pfl);
                if (lpfl->lpplord != pfl->lpplord) {
                    if (lpfl->lpplord->iordMax < pfl->cord) {
                        lpfl->lpplord = (PLORD *)LpplReAlloc((PL *)lpfl->lpplord, pfl->cord + 3);
                    }
                    fmemcpy(lpfl->lpplord->rgord, pfl->lpplord->rgord, pfl->lpplord->iordMac * 0x12);
                    lpfl->lpplord->iordMac = pfl->lpplord->iordMac;
                }
                fmemcpy(lpfl, pfl, 0x64);
                if (gd.fTutorial != 0x0 && idPlayer == 0) {
                    AdvanceTutor();
                }
            }
        }
        if (lpfl != 0x0) {
            return 1;
        }
        return 0;
    }
    return 0;
}

int16_t FDupFleet(FLEET *lpfl, FLEET *pfl) {
    PLORD   *lpplordT;
    uint16_t t_scratch_m8;

    lpplordT = pfl->lpplord;
    *pfl = *lpfl;
    if (lpfl->lpplord != 0x0) {
        pfl->lpplord = lpplordT;
        if (pfl->lpplord != 0x0) {
            t_scratch_m8 = pfl->lpplord->iordMax;
            if (t_scratch_m8 < lpfl->lpplord->iordMac) {
                pfl->lpplord = (PLORD *)LpplReAlloc((PL *)pfl->lpplord, lpfl->lpplord->iordMax);
            }
        } else {
            pfl->lpplord = (PLORD *)LpplAlloc(0x12, lpfl->lpplord->iordMax, htOrd);
        }
        fmemcpy(pfl->lpplord->rgord, lpfl->lpplord->rgord, lpfl->lpplord->iordMac * 0x12);
        pfl->lpplord->iordMac = lpfl->lpplord->iordMac;
        return 1;
    }
    if (lpplordT != 0x0) {
        FreePl((PL *)lpplordT);
    }
    return 1;
}

int16_t FLookupObject(GrobjClass grobj, int16_t id, void *pobj) {
    if (grobj != grobjFleet) {
        if (grobj != grobjPlanet) {
            return FLookupThing(id, pobj);
        }
        return FLookupPlanet(id, pobj);
    }
    return FLookupFleet(id, pobj);
}

int16_t FLookupOrbitingXfer(int16_t idPlanet, int16_t iNth, XFER *pxf, int16_t idSkip) {
    int16_t i;
    THING  *lpth;
    FLEET  *lpfl;
    THING  *lpthMac;
    int16_t t_25ad;
    int16_t t_266a;

    if (cFleet > 0) {
        if (cFleet != 0) {
            i = 0;
            while (1) {
                if (i >= cFleet)
                    goto L_2606;
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0x0)
                    goto L_2606;
                if (lpfl->idPlanet == idPlanet && lpfl->id != idSkip && (idSkip == -1 || (lpfl->pt.x == sel.pt.x && lpfl->pt.y == sel.pt.y))) {
                    t_25ad = iNth;
                    iNth = iNth - 1;
                    if (t_25ad == 0)
                        break;
                }
                i = i + 1;
            }
            if (pxf != 0x0) {
                pxf->fl = *lpfl;
                pxf->grobj = grobjFleet;
                pxf->id = lpfl->id;
            }
            return 1;
        }
    L_2606:
        lpth = lpThings;
        lpthMac = lpThings + cThing;
        for (; lpth < lpthMac; lpth++) {
            if (lpth->ith == ithMineralPacket && lpth->pt.x == sel.pt.x && lpth->pt.y == sel.pt.y) {
                t_266a = iNth;
                iNth = iNth - 1;
                if (t_266a == 0)
                    goto L_2679;
            }
        }
        return 0;
    L_2679:
        if (pxf != 0x0) {
            pxf->th = *lpth;
            pxf->grobj = grobjThing;
            pxf->id = lpth->idFull;
        }
        return 1;
    }
    return 0;
}

char *PszGetThingName(int16_t id) {
    THING *lpth;
    char   szPlr[54];

    lpth = LpthFromId(id);
    if (lpth != 0x0) {
        switch (lpth->ith) {
        case ithMinefield:
            if (lpth->iplr == idPlayer) {
                szPlr[0] = 0;
            } else {
                _wsprintf(szPlr, "%s ", PszPlayerName(lpth->iplr, 0, 0, 0, 0, 0x0));
            }
            _wsprintf(szWork, PszGetCompressedString(idsSSMineField), szPlr, rgszMineField[lpth->thm.iType]);
            break;
        case ithMineralPacket:
            if (lpth->thp.iWarp != 0x0) {
                if (lpth->iplr == idPlayer) {
                    szPlr[0] = 0;
                } else {
                    _wsprintf(szPlr, "%s ", PszPlayerName(lpth->iplr, 0, 0, 0, 0, 0x0));
                }
                _wsprintf(szWork, PszGetCompressedString(idsSmineralPacket), szPlr);
                break;
            }
            CchGetString(idsSalvage, szWork);
            return szWork;
        case ithWormhole:
            strcpy(szWork, PszGetCompressedString(idsWormhole));
            break;
        case ithMysteryTrader:
            strcpy(szWork, PszGetCompressedString(idsMysteryTrader));
            break;
        default:
            strcpy(szWork, PszGetCompressedString(idsMysteryObject));
        }
        return szWork;
    }
    szWork[0] = 0;
    return szWork;
}

char *PszGetFleetName(int16_t id) {
    int16_t cshdef;
    int16_t iplr;
    char   *lpsz;
    char    szShdef[34];
    int16_t ifl;
    FLEET  *lpfl;
    char    szPlr[34];
    int16_t ishdef;
    int16_t cch;

    lpfl = LpflFromId(id & 0x7fff);
    id = id & 0x7fff;
    iplr = (uint16_t)id >> 0x9 & 0xf;
    ifl = id & 0x1ff;
    if (iplr == idPlayer) {
        szPlr[0] = 0;
    } else {
        _wsprintf(szPlr, "%s ", PszPlayerName(iplr, 0, 0, 0, 0, 0x0));
    }
    if (lpfl != 0x0 && lpfl->lpszName != 0x0) {
        _wsprintf(szWork, "%s%s", szPlr, lpfl->lpszName);
    } else {
        if (lpfl != 0x0) {
            ishdef = IshdefPrimaryFromLpfl(lpfl, &cshdef);
            if (ishdef != 16) {
                fstrcpy(szShdef, rglpshdef[iplr][ishdef].hul.szClass);
                cch = strlen(szShdef);
                if (cch > 28) {
                    cch = 28;
                }
                if (cshdef > 1) {
                    szShdef[cch] = '+';
                    szShdef[cch + 1] = 0;
                }
                lpsz = szShdef;
            } else {
                lpsz = PszGetCompressedString(idsFleet);
            }
        } else {
            lpsz = PszGetCompressedString(idsFleet);
        }
        _wsprintf(szWork, "%s%s #%d", szPlr, lpsz, ifl + 1);
    }
    return szWork;
}

uint16_t WFromLpfl(FLEET *lpfl) {
    int16_t  cshdef;
    uint16_t w;
    int16_t  ishdef;

    ishdef = IshdefPrimaryFromLpfl(lpfl, &cshdef);
    w = ishdef << 0x9 | lpfl->ifl;
    if (cshdef > 1) {
        w = w | 0x2000;
    }
    return w;
}

char *PszFleetNameFromWord(uint16_t w) {
    char   *lpsz;
    char    szShdef[34];
    int16_t ishdef;
    int16_t cch;

    ishdef = w >> 0x9 & 0xf;
    if (rglpshdef[idPlayer][ishdef].fFree == 0x0) {
        fstrcpy(szShdef, rglpshdef[idPlayer][ishdef].hul.szClass);
        cch = strlen(szShdef);
        if (cch > 28) {
            cch = 28;
        }
        if ((w & 0x2000) != 0x0) {
            szShdef[cch] = '+';
            szShdef[cch + 1] = 0;
        }
        lpsz = szShdef;
    } else {
        lpsz = PszGetCompressedString(idsFleet);
    }
    _wsprintf(szWork, "%s #%d", lpsz, (w & 0x1ff) + 0x1);
    return szWork;
}

char *PszGetPlanetName(int16_t id) {
    int16_t fInOrbit;
    char   *psz;

    fInOrbit = id & 0x8000;
    id = id & 0x7fff;
    id = rgidPlan[id];
    psz = PszGetCompressedPlanet(id);
    if (fInOrbit == 0) {
        strcpy(szWork, psz);
    } else {
        _wsprintf(szWork, PszGetCompressedString(idsOrbitingS), psz);
    }
    return szWork;
}

int16_t IflFromLpfl(FLEET *lpfl) {
    int16_t i;

    for (i = 0; i < cFleet; i++) {
        if (rglpfl[i] == lpfl) {
            return i;
        }
    }
    return -1;
}

int16_t FDeleteFleet(int16_t idFleet, GrobjClass grobjSel, int16_t idSel) {
    int16_t i;
    FLEET  *lpfl;
    int16_t iPlr;
    int16_t idDel;
    PLANET *lppl;

    i = 0;
    while (1) {
        if (i >= cFleet)
            goto L_2dbc;
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0x0 || lpfl->id == idFleet)
            goto L_2dbc;
        if (lpfl->id > idFleet)
            break;
        i = i + 1;
    }
    return 0;
L_2dbc:
    if (i != cFleet) {
        if (idFleet == sel.fl.id) {
            RedrawScanSel(0x0, 0);
        }
        lpfl->fDead = 0x1;
        FleetOrdersChangeTarget(lpfl);
        FreePl((PL *)lpfl->lpplord);
        if (lpfl->lpszName != 0x0) {
            FreeLp(lpfl->lpszName, htString);
        }
        cFleet = cFleet - 1;
        iPlr = lpfl->iPlayer;
        idDel = lpfl->id;
        rgplr[iPlr].cFleet = rgplr[iPlr].cFleet - 0x1;
        if (grobjSel == grobjNone && lpfl->idPlanet != -1) {
            lppl = LpplFromId(lpfl->idPlanet);
            if (lppl != 0x0 && lppl->iPlayer == idPlayer) {
                grobjSel = grobjPlanet;
                idSel = lpfl->idPlanet;
            }
        }
        if (cFleet != i) {
            fmemmove(rglpfl + i, rglpfl + (i + 1), (cFleet - i) * sizeof(FLEET *));
        }
        FreeLp(lpfl, htFleets);
        gd.fFleetLinkValid = 0x0;
        if (sel.fl.id != -1) {
            if (i >= sel.scan.ifl) {
                if (idDel == sel.fl.id) {
                    if (grobjSel == grobjNone) {
                        sel.grobj = grobjNone;
                        sel.scan.grobj = grobjNone;
                        FFindSomethingAndSelectIt();
                        return 1;
                    }
                    sel.grobj = grobjNone;
                    if (grobjSel != grobjFleet) {
                        SelectAdjPlanet(0, idSel);
                    } else {
                        SelectAdjFleet(0, idSel);
                    }
                }
            } else {
                sel.scan.ifl = sel.scan.ifl - 1;
            }
        }
        if (gd.fGeneratingTurn == 0x0 && hwndMessage != 0x0) {
            SetMsgTitle(hwndMessage);
        }
        return 1;
    }
    return 0;
}

FLEET *LpflNew(int16_t iPlr, int16_t idPl) {
    int16_t i;
    ORDER  *lpord;
    FLEET  *lpfl;
    int16_t iflPrev;
    void   *t_call_3123;

    iflPrev = -1;
    for (i = 0; i < cFleet; i++) {
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0x0)
            break;
        if (lpfl->iPlayer >= iPlr) {
            if (lpfl->iPlayer > iPlr || lpfl->ifl != iflPrev + 1)
                break;
            iflPrev = lpfl->ifl;
        }
    }
    rglpfl = LpReAlloc(rglpfl, (cFleet + 1) * sizeof(FLEET *), htMisc);
    if (cFleet != i) {
        fmemmove(rglpfl + (i + 1), rglpfl + i, (cFleet - i) * sizeof(FLEET *));
    }
    t_call_3123 = LpAlloc(sizeof(FLEET), htFleets);
    lpfl = t_call_3123;
    rglpfl[i] = t_call_3123;
    cFleet = cFleet + 1;
    rgplr[iPlr].cFleet = rgplr[iPlr].cFleet + 0x1;
    fmemset(lpfl, 0, sizeof(FLEET));
    lpfl->ifl = iflPrev + 1;
    lpfl->iPlayer = iPlr;
    lpfl->iplr = iPlr;
    lpfl->det = 0x7;
    lpfl->idPlanet = idPl;
    if (idPl != -1) {
        lpfl->pt = rgptPlan[idPl];
    }
    lpfl->cord = 1;
    lpfl->fRepOrders = 0x0;
    lpfl->lpplord = (PLORD *)LpplAlloc(0x12, 0x3, htOrd);
    lpfl->lpplord->iordMac = 0x1;
    lpfl->fdirValid = 0x0;
    lpord = lpfl->lpplord->rgord;
    lpord->pt = lpfl->pt;
    lpord->id = lpfl->idPlanet;
    lpord->grobj = lpfl->idPlanet == -1 ? 0x4 : 0x1;
    lpord->iWarp = 0x0;
    lpord->fValidTask = 0x1;
    lpord->grTask = grTaskNone;
    if (sel.scan.ifl != -1 && i <= sel.scan.ifl) {
        sel.scan.ifl = sel.scan.ifl + 1;
    }
    gd.fFleetLinkValid = 0x0;
    return lpfl;
}

FLEET *LpflNewSplit(FLEET *pfl) {
    int16_t iordMac;
    FLEET  *lpflNew;

    lpflNew = LpflNew(pfl->iPlayer, pfl->idPlanet);
    if (pfl->idPlanet == -1) {
        lpflNew->pt = pfl->pt;
    }
    lpflNew->fRepOrders = pfl->fRepOrders;
    lpflNew->det = pfl->det;
    lpflNew->iplan = pfl->iplan;
    iordMac = pfl->lpplord->iordMac;
    if (lpflNew->lpplord->iordMax < iordMac) {
        lpflNew->lpplord = (PLORD *)LpplReAlloc((PL *)lpflNew->lpplord, pfl->lpplord->iordMax);
    }
    fmemcpy(lpflNew->lpplord->rgord, pfl->lpplord->rgord, iordMac * 18);
    lpflNew->lpplord->iordMac = LOBYTE(iordMac);
    lpflNew->cord = pfl->cord;
    LogSplitFleet(pfl->id);
    return lpflNew;
}

int16_t FFleetMergeAll(FLEET *pfl) {
    int16_t iplr;
    int32_t dpT;
    int16_t fCshOverflow;
    int16_t rgcshDamaged[16];
    int16_t cflMerge;
    int16_t i;
    FLEET  *lpfl;
    int16_t cshT;
    SHDEF  *lpshdef;
    FLEET  *lpflMerge;
    int32_t rgdp[16];
    int16_t j;

    lpflMerge = 0x0;
    cflMerge = 0;
    fCshOverflow = 0;
    memset(rgdp, 0, 0x40);
    memset(rgcshDamaged, 0, 0x20);
    iplr = pfl->iPlayer;
    for (i = 0; i < vcflMerge; i++) {
        if (vrgiflMerge[i] != -1) {
            lpfl = LpflFromId(vrgiflMerge[i]);
            if (lpfl != 0x0) {
                lpshdef = rglpshdef[iplr];
                j = 0;
                while (j < 16) {
                    if (lpfl->rgcsh[j] != 0) {
                        if (lpfl->rgdv[j].dp != 0x0) {
                            cshT = LOWORD((int32_t)((int32_t)((int32_t)lpfl->rgcsh[j] * (int32_t)lpfl->rgdv[j].pctSh) / 0x64));
                            if (cshT == 0) {
                                cshT = 1;
                            }
                            rgcshDamaged[j] = rgcshDamaged[j] + cshT;
                            dpT = (uint32_t)(lpfl->rgdv[j].pctDp * (int32_t)cshT);
                            rgdp[j] = rgdp[j] + dpT;
                        }
                        if (lpfl->ifl != pfl->ifl) {
                            pfl->rgcsh[j] = pfl->rgcsh[j] + lpfl->rgcsh[j];
                            if (pfl->rgcsh[j] < 0) {
                                pfl->rgcsh[j] = 32766;
                            }
                            lpfl->rgcsh[j] = 0;
                        }
                    }
                    j = j + 1;
                    lpshdef = lpshdef + 1;
                }
                if (lpfl->ifl != pfl->ifl) {
                    for (j = 0; j < 5; j++) {
                        pfl->rgwtMin[j] = pfl->rgwtMin[j] + lpfl->rgwtMin[j];
                    }
                    FDeleteFleet(lpfl->id, grobjNone, -1);
                    cflMerge = cflMerge + 1;
                } else {
                    lpflMerge = lpfl;
                }
            }
        }
    }
    lpshdef = rglpshdef[iplr];
    j = 0;
    while (j < 16) {
        lpflMerge->rgcsh[j] = pfl->rgcsh[j];
        if (rgcshDamaged[j] != 0 && lpflMerge->rgcsh[j] != 0) {
            lpflMerge->rgdv[j].pctSh =
                LOWORD((int32_t)((int32_t)((uint32_t)((int32_t)rgcshDamaged[j] * 100) + (int32_t)(lpflMerge->rgcsh[j] - 1)) / (int32_t)lpflMerge->rgcsh[j]));
            lpflMerge->rgdv[j].pctDp = LOWORD((int32_t)(rgdp[j] / (int32_t)rgcshDamaged[j]));
        } else {
            lpflMerge->rgdv[j].dp = 0x0;
        }
        j = j + 1;
        lpshdef = lpshdef + 1;
    }
    for (j = 0; j < 5; j++) {
        lpflMerge->rgwtMin[j] = pfl->rgwtMin[j];
    }
    LogMergeFleet(pfl->id);
    InvalidateReport(1, 2);
    if (cflMerge == 0) {
        return 0;
    }
    return 1;
}

int16_t FFleetSplitAll(FLEET *pfl) {
    FLEET   flNew;
    int16_t cSplit;
    int16_t c;
    int16_t i;
    FLEET  *lpflNew;
    int16_t t_3a2d;
    int16_t t_3a3e;

    cSplit = 0;
    for (i = 0; i < 16; i++) {
        c = pfl->rgcsh[i];
        while (1) {
            t_3a2d = c;
            c = c - 1;
            if (t_3a2d == 0)
                break;
            t_3a3e = cSplit;
            cSplit = cSplit + 1;
            if (t_3a3e != 0) {
                lpflNew = LpflNewSplit(pfl);
                flNew = *lpflNew;
                pfl->rgcsh[i] = pfl->rgcsh[i] - 1;
                flNew.rgcsh[i] = flNew.rgcsh[i] + 1;
                FleetTransferCargoBalance(pfl, &flNew);
                FLookupFleet(-1, pfl);
                FLookupFleet(-1, &flNew);
            }
        }
    }
    InvalidateReport(1, 2);
    if (cSplit <= 1) {
        return 0;
    }
    return 1;
}

char *PszGetLocName(GrobjClass grobj, int16_t id, int16_t x, int16_t y) {
    if (id != -1) {
        switch (grobj) {
        case grobjPlanet:
            return PszGetPlanetName(id);
        case grobjFleet:
            return PszGetFleetName(id);
        case grobjThing:
            return PszGetThingName(id);
        default:
        }
    }
    if (x != -1 || y != -1) {
        _wsprintf(szWork, PszGetCompressedString(idsSpaceDD), x, y);
    } else {
        strcpy(szWork, PszGetCompressedString(idsDeepSpace));
    }
    return szWork;
}

int16_t CchGetETA(HDC hdc, FLEET *lpfl, char *sz, int16_t iwp, int16_t fSmall) {
    int16_t  iWarp;
    double   dbl;
    ORDER   *lpord;
    int16_t  i;
    int16_t  c;
    int16_t  iSpeed;
    int16_t  j;
    int16_t  cYears;
    StringId ids;
    int16_t  t_3e00;

    cYears = 0;
    lpord = lpfl->lpplord->rgord;
    i = 0;
    while (1) {
        if (i >= iwp)
            goto L_3dba;
        dbl = DGetDistance(lpord->pt.x, lpord->pt.y, lpord[1].pt.x, lpord[1].pt.y);
        iWarp = lpord[1].iWarp;
        if (iWarp >= 11) {
            j = FCanFleetUseStargates(lpfl, lpord->pt, lpord[1].pt);
            switch (j) {
            case -1:
                iSpeed = -3;
                break;
            case 0:
                iSpeed = 0;
                break;
            case 1:
                iSpeed = 8000;
                break;
            default:
                if ((j & 0x2) == 0x0) {
                    iSpeed = -2;
                } else {
                    iSpeed = -1;
                }
            }
        } else {
            iSpeed = iWarp * iWarp;
        }
        if (iSpeed == 0)
            break;
        if (iSpeed < 0)
            goto L_3d24;
        if (iSpeed < LOWORD((int32_t)dbl)) {
            iSpeed = (int32_t)(LOWORD((int32_t)dbl) + iSpeed - 0x1) / iSpeed;
        } else {
            iSpeed = 1;
        }
        cYears = cYears + iSpeed;
        i = i + 1;
        lpord = lpord + 1;
    }
    if (hdc != 0x0) {
        SetTextColor(hdc, 0xff);
    }
    c = CchGetString(idsNever, sz);
    return c;
L_3d24:
    if (hdc != 0x0) {
        SetTextColor(hdc, 0x7f7f);
    }
    if (iSpeed != -1) {
        if (iSpeed != -2) {
            ids = idsUncertain;
        } else {
            ids = idsUnload2;
        }
    } else {
        ids = idsDanger;
    }
    c = CchGetString(ids, sz);
    return c;
L_3dba:
    c = _wsprintf(sz, PszGetCompressedString(fSmall == 0 ? idsDYear : idsDy), cYears);
    if (cYears != 1 && fSmall == 0) {
        t_3e00 = c;
        c = c + 1;
        sz[t_3e00] = 's';
    }
    return c;
}

int16_t IshdefPrimaryFromLpfl(FLEET *lpfl, int16_t *pcDiff) {
    int16_t cDiff;
    int16_t ish;
    int16_t i;
    int16_t csh;
    int16_t ihul;

    cDiff = 0;
    csh = 0;
    ish = 16;
    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            cDiff = cDiff + 1;
            if (lpfl->rgcsh[i] > csh) {
                ish = i;
                csh = lpfl->rgcsh[i];
                ihul = rglpshdef[lpfl->iPlayer][i].hul.ihuldef;
                if (ihul == 25 || ihul == 26) {
                    csh = csh - 1;
                }
            }
        }
    }
    if (pcDiff != 0x0) {
        *pcDiff = cDiff;
    }
    return ish;
}

char *PszGetDistance(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    int32_t d;
    int16_t fStarted;
    int32_t d2;

    fStarted = 0;
    d = (int32_t)(DGetDistance(x1, y1, x2, y2) * 100.0 + 0.5);
    d2 = (int32_t)(d / 100);
    d = d - (uint32_t)(d2 * 100);
    if (dyArial8 > 14) {
        _wsprintf(szWork, PszGetCompressedString(idsLdLdLY), d2, d);
    } else {
        _wsprintf(szWork, PszGetCompressedString(idsLdLdLightYears), d2, d);
    }
    return szWork;
}

double DGetDistance(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    int32_t dy;
    int32_t dx;
    int32_t l;
    double  t_call_404f;

    dx = (int32_t)(x2 - x1);
    dy = (int32_t)(y2 - y1);
    l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
    t_call_404f = sqrt((double)l);
    __fac = t_call_404f;
    return t_call_404f;
}

int16_t FFindNearestObject(POINT16 pt, GrobjClass grobj, SCAN *pscan) {
    POINT16  ptWp;
    POINT16 *ppt;
    int16_t  dy;
    int32_t  lTry;
    THING   *lpth;
    FLEET   *lpfl;
    int16_t  i;
    THING   *lpthMac;
    int32_t  lSquare;
    SCAN     scanT;
    int16_t  iNearest;
    int16_t  dx;
    SCAN     scan;

    iNearest = -1;
    lSquare = 1000000000;
    ppt = rgptPlan;
    scan.grobjFull = grobjNone;
    scan.grobj = grobjNone;
    scan.pt.y = 0;
    scan.pt.x = 0;
    scan.ith = -1;
    scan.iwp = -1;
    scan.ifl = -1;
    scan.idpl = -1;
    if ((grobj & 0x40) == 0x0) {
        if ((grobj & 0x80) != 0x0) {
            lSquare = 0;
        }
    } else {
        lSquare = (int32_t)ScanToPt(20);
        lSquare = (uint32_t)(lSquare * lSquare);
    }
    if ((grobj & 0x1) != 0x0) {
        i = 0;
        while (i < game.cPlanMax) {
            dx = pt.x - ppt->x;
            dy = pt.y - ppt->y;
            lTry = (uint32_t)((int32_t)dx * (int32_t)dx);
            if ((int32_t)(uint32_t)((int32_t)dx * (int32_t)dx) <= lSquare) {
                lTry = lTry + (uint32_t)((int32_t)dy * (int32_t)dy);
                if (lTry <= lSquare && (lSquare != lTry || lSquare == 0)) {
                    lSquare = lTry;
                    scan.pt.x = ppt->x;
                    scan.pt.y = ppt->y;
                    scan.idpl = i;
                    scan.grobjFull = grobjPlanet;
                    scan.grobj = grobjPlanet;
                }
            }
            i = i + 1;
            ppt = ppt + 1;
        }
    }
    if ((grobj & 0x2) != 0x0) {
        for (i = 0; i < cFleet; i++) {
            lpfl = rglpfl[i];
            if (rglpfl[i] == 0x0)
                break;
            dx = pt.x - lpfl->pt.x;
            dy = pt.y - lpfl->pt.y;
            lTry = (uint32_t)((int32_t)dx * (int32_t)dx);
            if ((int32_t)(uint32_t)((int32_t)dx * (int32_t)dx) <= lSquare) {
                lTry = lTry + (uint32_t)((int32_t)dy * (int32_t)dy);
                if (lTry <= lSquare) {
                    if ((lTry != lSquare || (scan.grobj & 0x1) != 0x0) && (lpfl->pt.x != scan.pt.x || lpfl->pt.y != scan.pt.y)) {
                        if (lTry >= lSquare)
                            continue;
                        lSquare = lTry;
                    } else {
                        if ((scan.grobjFull & 0x2) != 0x0 && (rglpfl[scan.ifl]->iPlayer == idPlayer || lpfl->iPlayer != idPlayer))
                            continue;
                        scan.ifl = i;
                        scan.grobjFull = scan.grobjFull | 0x2;
                        if (scan.grobj != grobjNone)
                            continue;
                    }
                    scan.pt = lpfl->pt;
                    scan.ifl = i;
                    scan.idpl = -1;
                    scan.grobjFull = grobjFleet;
                    scan.grobj = grobjFleet;
                }
            }
        }
    }
    if ((grobj & 0x8) != 0x0) {
        lpth = lpThings;
        lpthMac = lpThings + cThing;
        for (; lpth < lpthMac; lpth++) {
            dx = pt.x - lpth->pt.x;
            dy = pt.y - lpth->pt.y;
            lTry = (uint32_t)((int32_t)dx * (int32_t)dx);
            if ((int32_t)(uint32_t)((int32_t)dx * (int32_t)dx) <= lSquare) {
                lTry = lTry + (uint32_t)((int32_t)dy * (int32_t)dy);
                if (lTry <= lSquare) {
                    if ((lTry != lSquare || (scan.grobj & 0x3) != 0x0) && (lpth->pt.x != scan.pt.x || lpth->pt.y != scan.pt.y)) {
                        if (lTry >= lSquare)
                            continue;
                        lSquare = lTry;
                    } else {
                        if ((scan.grobjFull & 0x8) != 0x0)
                            continue;
                        scan.ith = (int32_t)((uint8_t *)lpth - (uint8_t *)lpThings) / 18;
                        scan.grobjFull = scan.grobjFull | 0x8;
                        if (scan.grobj != grobjNone)
                            continue;
                    }
                    scan.pt = lpth->pt;
                    scan.ith = (int32_t)((uint8_t *)lpth - (uint8_t *)lpThings) / 18;
                    scan.idpl = -1;
                    scan.ifl = -1;
                    scan.grobjFull = grobjThing;
                    scan.grobj = grobjThing;
                }
            }
        }
    }
    if ((grobj & 0x4) != 0x0 && sel.grobj == grobjFleet) {
        for (i = sel.fl.cord - 1; i >= 0; i--) {
            ptWp = sel.fl.lpplord->rgord[i].pt;
            dx = pt.x - ptWp.x;
            dy = pt.y - ptWp.y;
            lTry = (uint32_t)((int32_t)dx * (int32_t)dx);
            if ((int32_t)(uint32_t)((int32_t)dx * (int32_t)dx) <= lSquare) {
                lTry = lTry + (uint32_t)((int32_t)dy * (int32_t)dy);
                if (lTry <= lSquare) {
                    if ((lTry != lSquare || (scan.grobj & 0xb) != 0x0) && (ptWp.x != scan.pt.x || ptWp.y != scan.pt.y)) {
                        if (lTry >= lSquare)
                            continue;
                        lSquare = lTry;
                    } else {
                        if ((scan.grobjFull & 0x4) != 0x0 && i != sel.scan.iwp)
                            continue;
                        scan.iwp = i;
                        scan.grobjFull = scan.grobjFull | 0x4;
                        if (scan.grobj != grobjNone)
                            continue;
                    }
                    scan.pt = ptWp;
                    scan.iwp = i;
                    scan.ith = -1;
                    scan.idpl = -1;
                    scan.ifl = -1;
                    scan.grobjFull = grobjOther;
                    scan.grobj = grobjOther;
                }
            }
        }
    }
    if ((grobj & 0x20) == 0x0 && scan.grobj != grobjNone && FFindNearestObject(scan.pt, ((grobj & 0xf) ^ 0xf) | 0xa0, &scanT) != 0) {
        if (scanT.idpl != -1) {
            scan.idpl = scanT.idpl;
        }
        if (scanT.ifl != -1) {
            scan.ifl = scanT.ifl;
        }
        if (scanT.iwp != -1) {
            scan.iwp = scanT.iwp;
        }
        if (scanT.ith != -1) {
            scan.ith = scanT.ith;
        }
        scan.grobjFull = scan.grobjFull | scanT.grobjFull;
    }
    if (pscan != 0x0) {
        *pscan = scan;
    }
    if (scan.grobj == grobjNone) {
        return 0;
    }
    return 1;
}

void UpdateShdefCost(SHDEF *lpshdef) {
    int16_t  dpT;
    uint32_t wt;
    int16_t  k;
    int16_t  c;
    uint16_t rgCosts[4];
    int16_t  fWeakArmor;
    HUL     *lphul;
    uint32_t resCost;
    uint32_t rgMin[3];
    PART     part;

    if (lpshdef->det != 0x7) {
        fWeakArmor = 0;
    } else {
        fWeakArmor = GetRaceGrbit(&rgplr[idPlayer], ibitRaceRegeneratingShields) == 0 ? 0 : 1;
    }
    lphul = &LphuldefFromId(lpshdef->hul.ihuldef)->hul;
    part.hs.grhst = hstNone;
    part.phul = lphul;
    GetTruePartCost(idPlayer, &part, rgCosts);
    for (c = 0; c < 3; c++) {
        rgMin[c] = (uint32_t)rgCosts[c];
    }
    resCost = (uint32_t)rgCosts[3];
    wt = (uint32_t)lphul->wtEmpty;
    lpshdef->hul.dp = lphul->dp;
    lphul = &lpshdef->hul;
    for (c = 0; c < lphul->chs; c++) {
        if (lphul->rghs[c].cItem > 0x0) {
            part.hs = lphul->rghs[c];
            FLookupPart(&part);
            GetTruePartCost(idPlayer, &part, rgCosts);
            for (k = 0; k < 3; k++) {
                rgMin[k] = rgMin[k] + (uint32_t)(lphul->rghs[c].cItem * rgCosts[k]);
            }
            resCost = resCost + (uint32_t)(lphul->rghs[c].cItem * rgCosts[3]);
            wt = wt + (uint32_t)(lphul->rghs[c].cItem * part.pcom->cMass);
            switch (lphul->rghs[c].grhst) {
            default:
                break;
            case hstArmor:
                dpT = lphul->rghs[c].cItem * part.parmor->dp;
                if (fWeakArmor != 0) {
                    dpT = dpT >> 0x1;
                }
                lphul->dp = lphul->dp + dpT;
                break;
            case hstShield:
                if (lphul->rghs[c].iItem != 0x3 && lphul->rghs[c].iItem != 0x6)
                    break;
                lphul->dp = lphul->dp + lphul->rghs[c].cItem * 0x41;
                break;
            case hstSpecialM:
                if (lphul->rghs[c].iItem == 0x4) {
                    lphul->dp = lphul->dp + lphul->rghs[c].cItem * 0x32;
                }
            }
        }
    }
    for (c = 0; c < 3; c++) {
        lphul->rgwtOreCost[c] = LOWORD(rgMin[c]);
    }
    lphul->resCost = LOWORD(resCost);
    lphul->wtEmpty = LOWORD(wt);
    lpshdef->lPower = -1;
    return;
}

uint16_t WPackLong(int32_t l) {
    uint16_t exp;

    exp = 0x0;
    while ((l & 0xffffe000) != 0x0) {
        l = (uint32_t)((uint32_t)l >> 0x2);
        exp = exp + 0x1;
    }
    return exp << 0xd | LOWORD(l);
}

int16_t GetPlanetScannerRange(PLANET *lppl, int16_t *pDeep) {
    int16_t iPlrSav;
    int16_t dRange;
    PART    part;

    iPlrSav = idPlayer;
    idPlayer = lppl->iPlayer;
    if (pDeep != 0x0) {
        *pDeep = 0;
    }
    if (GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
        if (lppl->iScanner == 0x1f) {
            idPlayer = iPlrSav;
            return 0;
        }
        LookupBestPlanetaryScanner(&part);
        if (pDeep != 0x0 && part.pplanetary->grAbility < 0) {
            *pDeep = -part.pplanetary->grAbility >> 0x1;
        }
        dRange = abs(part.pplanetary->grAbility);
        if (GetRaceGrbit(&rgplr[idPlayer], ibitRaceNoAdvScanner) != 0) {
            dRange = dRange * 2;
        }
    } else {
        dRange = LOWORD((int32_t)sqrt((double)(uint32_t)(lppl->rgwtMin[3] * 10)));
        if (GetRaceGrbit(&rgplr[idPlayer], ibitRaceNoAdvScanner) == 0) {
            if (pDeep != 0x0 && lppl->fStarbase != 0x0 && rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef >= ihuldefUltraStation) {
                *pDeep = (int32_t)dRange / 2;
            }
        } else {
            dRange = LOWORD((int32_t)((int32_t)((int32_t)dRange * 1412) / 0x3e8));
            if (pDeep != 0x0) {
                *pDeep = 0;
            }
        }
    }
    idPlayer = iPlrSav;
    return dRange;
}

int16_t GetCachedFleetScannerRange(FLEET *lpfl, int16_t *pdPlanRange, int16_t *ppctDetect, int16_t *piSteal) {
    int16_t dT;
    int16_t dPlanRange;
    int16_t i;
    int16_t iPlr;
    int16_t dRange;
    int16_t iSteal;
    int16_t pctDetect;

    iPlr = lpfl->iPlayer;
    dRange = -1;
    dPlanRange = 0;
    iSteal = 0;
    pctDetect = 100;
    if (gd.fGeneratingTurn != 0x0) {
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] > 0) {
                dT = rglpshdef[iPlr][i].dScanRange;
                if (dT != 2047 && dT > dRange) {
                    dRange = dT;
                }
                if (rglpshdef[iPlr][i].dScanRange2 > dPlanRange) {
                    dPlanRange = rglpshdef[iPlr][i].dScanRange2;
                }
                if (rglpshdef[iPlr][i].pctDetect < pctDetect) {
                    pctDetect = rglpshdef[iPlr][i].pctDetect;
                }
                iSteal = iSteal | rglpshdef[iPlr][i].iSteal;
            }
        }
        if (pdPlanRange != 0x0) {
            *pdPlanRange = dPlanRange;
        }
        if (ppctDetect != 0x0) {
            *ppctDetect = pctDetect;
        }
        if (piSteal != 0x0) {
            *piSteal = iSteal;
        }
        return dRange;
    }
    return GetFleetScannerRange(lpfl, pdPlanRange, ppctDetect, piSteal);
}

int16_t GetFleetScannerRange(FLEET *lpfl, int16_t *pdPlanRange, int16_t *ppctDetect, int16_t *piSteal) {
    int16_t iplr;
    int16_t dPlanRange;
    int16_t i;
    int16_t dRange;
    int16_t iSteal;
    int16_t dPlanRangeBest;
    int16_t dRangeBest;
    int16_t pctDetect;

    iplr = lpfl->iPlayer;
    dRange = -1;
    dPlanRange = 0;
    iSteal = 0;
    if (ppctDetect != 0x0) {
        *ppctDetect = 100;
    }
    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            dRangeBest = GetShdefScannerRange(rglpshdef[iplr] + i, iplr, &dPlanRangeBest, &pctDetect, &iSteal);
            if (ppctDetect != 0x0 && pctDetect < *ppctDetect) {
                *ppctDetect = pctDetect;
            }
            if (piSteal != 0x0) {
                *piSteal = *piSteal | iSteal;
            }
            if (dRangeBest > dRange) {
                dRange = dRangeBest;
            }
            if (dPlanRange < dPlanRangeBest) {
                dPlanRange = dPlanRangeBest;
            }
        }
    }
    if (pdPlanRange != 0x0) {
        *pdPlanRange = dPlanRange;
    }
    return dRange;
}

int16_t GetShdefScannerRange(SHDEF *lpshdef, int16_t iplr, int16_t *pdPlanRange, int16_t *ppctDetect, int16_t *piSteal) {
    int16_t  chs;
    HS      *lphs;
    int16_t  dRangeT2;
    double   lBIR4;
    int16_t  dRangeT;
    int16_t  fHasScanner;
    int16_t  iScanner;
    int16_t  fBuiltIn;
    int16_t  cDetectors;
    double   lPlanRange4;
    int16_t  dRange;
    double   lT;
    int16_t  iSteal;
    int16_t  j;
    double   lBIPR4;
    double   lRange4;
    int16_t  t_merge_5142_0001;
    SCANNER *t_call_52ec;
    int16_t  t_merge_53ea_0001;

    lRange4 = 0.0;
    lPlanRange4 = 0.0;
    fHasScanner = 0;
    iSteal = 0;
    cDetectors = 0;
    if (iplr == -1 || GetRaceStat(&rgplr[iplr], rsMajorAdv) != raNone) {
        t_merge_5142_0001 = 0;
    } else {
        t_merge_5142_0001 = 1;
    }
    fBuiltIn = t_merge_5142_0001;
    lBIR4 = -1.0;
    lBIPR4 = -1.0;
    if (ppctDetect != 0x0) {
        *ppctDetect = 100;
    }
    if (fBuiltIn != 0) {
        switch (lpshdef->hul.ihuldef) {
        case ihuldefScout:
        case ihuldefDestroyer:
        case ihuldefFrigate:
            if (lBIR4 < 0.0) {
                if (game.fTutorial == 0x0) {
                    lBIPR4 = (double)(int32_t)((int16_t)rgplr[iplr].rgTech[4] * 10);
                    lBIR4 = lBIPR4 * 2.0;
                    lBIPR4 = lBIPR4 * lBIPR4;
                    lBIPR4 = lBIPR4 * lBIPR4;
                    lBIR4 = lBIR4 * lBIR4;
                    lBIR4 = lBIR4 * lBIR4;
                } else {
                    lBIR4 = 2.56e+06;
                    lBIPR4 = 160000.0;
                }
            }
            lRange4 = lBIR4;
            lPlanRange4 = lBIPR4;
        default:
        }
    }
    lphs = lpshdef->hul.rghs;
    chs = lpshdef->hul.chs;
    j = 0;
    while (j < chs) {
        if (lphs->cItem != 0x0) {
            if (lphs->grhst == hstScanner) {
                fHasScanner = 1;
                iScanner = lphs->iItem;
                t_call_52ec = LpscannerFromId(lphs->iItem);
                dRangeT = t_call_52ec->dRange;
                lT = (double)(int32_t)t_call_52ec->dRange;
                lT = lT * lT;
                lT = lT * lT;
                lT = lT * (double)(uint32_t)lphs->cItem;
                lRange4 = lRange4 + lT;
                dRangeT = LpscannerFromId(iScanner)->grfAbilities;
                switch (iScanner) {
                case 6:
                    dRangeT = 45;
                    goto LPlanScan;
                case 5:
                    dRangeT = 0;
                    iSteal = iSteal | 0x1;
                    goto LPlanScan;
                case 14:
                    dRangeT = 120;
                    iSteal = iSteal | 0x3;
                    goto LPlanScan;
                default:
                    if (dRangeT > 0) {
                        if (dRangeT != 1) {
                            if (dRangeT != 2) {
                                t_merge_53ea_0001 = 200;
                            } else {
                                t_merge_53ea_0001 = 100;
                            }
                        } else {
                            t_merge_53ea_0001 = 50;
                        }
                        dRangeT = t_merge_53ea_0001;
                        goto LPlanScan;
                    }
                }
                goto L_5298;
            }
            if (lphs->grhst != hstArmor || lphs->iItem != iarmorMegaPolyShell) {
                if (lphs->grhst != hstBeam || lphs->iItem != ibeamMultiContainedMunition) {
                    if (lphs->grhst != hstShield || lphs->iItem != ishieldLangstonShell) {
                        if (ppctDetect == 0x0 || lphs->grhst != hstSpecialE || lphs->iItem != ispecialETachyonDetector)
                            goto L_5298;
                        cDetectors = cDetectors + lphs->cItem;
                        goto L_5298;
                    }
                    dRangeT = 50;
                    dRangeT2 = 25;
                } else {
                    dRangeT = 150;
                    dRangeT2 = 75;
                }
            } else {
                dRangeT = 80;
                dRangeT2 = 40;
            }
            lT = (double)(int32_t)dRangeT;
            lT = lT * lT;
            lT = lT * lT;
            lT = lT * (double)(uint32_t)lphs->cItem;
            lRange4 = lRange4 + lT;
            dRangeT = dRangeT2;
        LPlanScan:
            lT = (double)(int32_t)dRangeT;
            lT = lT * lT;
            lT = lT * lT;
            lT = lT * (double)(uint32_t)lphs->cItem;
            lPlanRange4 = lPlanRange4 + lT;
        }
    L_5298:
        j = j + 1;
        lphs = lphs + 1;
    }
    if (lRange4 <= 0.0 && fHasScanner == 0) {
        dRange = -1;
    } else {
        dRange = LOWORD((int32_t)sqrt(sqrt(lRange4)));
        if (iplr != -1 && GetRaceGrbit(&rgplr[iplr], ibitRaceNoAdvScanner) != 0) {
            dRange = dRange * 2;
        }
    }
    if (pdPlanRange != 0x0) {
        *pdPlanRange = LOWORD((int32_t)sqrt(sqrt(lPlanRange4)));
    }
    if (piSteal != 0x0) {
        *piSteal = iSteal;
    }
    if (ppctDetect != 0x0) {
        if (cDetectors >= 18) {
            cDetectors = 17;
        }
        *ppctDetect = vrgbTachyon[cDetectors];
    }
    return dRange;
}

int32_t LCalcFuelGainFromRamScoops(FLEET *lpfl, int16_t iWarp, int32_t dTravel) {
    int16_t  i;
    int16_t *rgiFuel;
    SHDEF   *lpshdef;
    int32_t  pct10;
    int32_t  pctShip10;

    pct10 = 0;
    if (iWarp <= 10) {
        i = 0;
        lpshdef = rglpshdef[lpfl->iPlayer];
        while (i < 16) {
            if (lpfl->rgcsh[i] != 0) {
                rgiFuel = LpengineFromId(lpshdef->hul.rghs[0].iItem)->rgcFuelUsed;
                pctShip10 = 0;
                if (iWarp <= 9) {
                    if (rgiFuel[iWarp] == 0) {
                        pctShip10 = pctShip10 + lpshdef->hul.rghs[0].cItem;
                        if (rgiFuel[iWarp + 1] == 0) {
                            pctShip10 = pctShip10 + (uint32_t)(lpshdef->hul.rghs[0].cItem * 0x2);
                            if (iWarp < 9 && rgiFuel[iWarp + 2] == 0) {
                                pctShip10 = pctShip10 + (uint32_t)(lpshdef->hul.rghs[0].cItem * 0x3);
                                if (iWarp < 8 && rgiFuel[iWarp + 3] == 0) {
                                    pctShip10 = pctShip10 + (uint32_t)(lpshdef->hul.rghs[0].cItem * 0x4);
                                }
                            }
                        }
                    }
                    pct10 = pct10 + (uint32_t)(pctShip10 * (int32_t)lpfl->rgcsh[i]);
                }
            }
            i = i + 1;
            lpshdef = lpshdef + 1;
        }
        pct10 = (uint32_t)(pct10 * dTravel);
        return pct10;
    }
    return 0;
}

int32_t CalcPlayerScore(int16_t iPlr, SCORE *pscore) {
    int32_t rgcsh[3];
    int32_t lTemp;
    SCORE   score;
    PLANET *lpplMac;
    PLANET *lppl;
    int16_t i;
    int16_t ifl;
    FLEET  *lpfl;
    int16_t iTech;
    int32_t lPower;
    int16_t rgType[16];
    int32_t t_merge_5cb9_0001;
    int32_t t_merge_5cf5_0001;

    memset(&score, 0, sizeof(SCORE));
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            score.cPlanet = score.cPlanet + 1;
            lTemp = (int32_t)((lppl->rgwtMin[3] + 999) / 0x3e8);
            if (lTemp > 6) {
                lTemp = 6;
            }
            score.lScore = score.lScore + lTemp;
            if (lppl->fStarbase != 0x0 && LphuldefFromId(rglpshdefSB[iPlr][lppl->isb].hul.ihuldef)->hul.wtCargoMax != 0x0) {
                score.cStarbase = score.cStarbase + 1;
            }
            score.cResources = score.cResources + (int32_t)CResourcesAtPlanet(lppl, iPlr);
        }
    }
    score.lScore = score.lScore + (int32_t)(score.cResources / 30);
    score.lScore = score.lScore + (int32_t)(3 * score.cStarbase);
    if (rgplr[iPlr].fDead == 0x0) {
        for (i = 0; i < 6; i++) {
            iTech = (int16_t)rgplr[iPlr].rgTech[i];
            score.cTechLevels = score.cTechLevels + (int16_t)rgplr[iPlr].rgTech[i];
            if (iTech >= 4) {
                if (iTech >= 7) {
                    if (iTech >= 10) {
                        score.lScore = score.lScore + (int32_t)(iTech * 4 - 18);
                    } else {
                        score.lScore = score.lScore + (int32_t)(3 * iTech - 0x9);
                    }
                } else {
                    score.lScore = score.lScore + (int32_t)(iTech * 2 - 3);
                }
            } else {
                score.lScore = score.lScore + (int32_t)iTech;
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (rglpshdef[iPlr][i].fFree == 0x0) {
            lPower = LComputePower(rglpshdef[iPlr] + i);
            if (lPower <= 0) {
                rgType[i] = 0;
            } else if (lPower < 2000) {
                rgType[i] = 1;
            } else {
                rgType[i] = 2;
            }
        } else {
            rgType[i] = -1;
        }
    }
    for (i = 0; i < 3; i++) {
        rgcsh[i] = 0;
    }
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->iPlayer == iPlr && lpfl->fDead == 0x0) {
            for (i = 0; i < 16; i++) {
                if (lpfl->rgcsh[i] > 0 && rgType[i] != -1) {
                    rgcsh[rgType[i]] = rgcsh[rgType[i]] + (int32_t)lpfl->rgcsh[i];
                }
            }
        }
    }
    t_merge_5cb9_0001 = rgcsh[1] < (int32_t)score.cPlanet ? rgcsh[1] : (int32_t)score.cPlanet;
    t_merge_5cf5_0001 = rgcsh[0] < (int32_t)score.cPlanet ? rgcsh[0] : (int32_t)score.cPlanet;
    score.lScore = score.lScore + ((int32_t)(t_merge_5cf5_0001 / 2) + (int32_t)(t_merge_5cb9_0001 * 2));
    if (rgcsh[2] > 0) {
        score.lScore = score.lScore + (int32_t)((int32_t)((int32_t)(rgcsh[2] * 8) * (int32_t)score.cPlanet) / ((int32_t)score.cPlanet + rgcsh[2]));
    }
    for (i = 0; i < 3; i++) {
        score.rgcsh[i] = WPackLong(rgcsh[i]);
    }
    if (pscore != 0x0) {
        *pscore = score;
    }
    return score.lScore;
}

void GetTrueHullCost(int16_t iPlayer, HUL *lphul, uint16_t *rgCost) {
    int16_t i;

    for (i = 0; i < 3; i++) {
        rgCost[i] = lphul->rgwtOreCost[i];
    }
    rgCost[3] = lphul->resCost;
    return;
}

void DecorateHullName(int16_t iplr, int16_t ish, char *psz) {
    int16_t  i;
    int16_t  c;
    SHDEF   *lpshdef;
    int16_t  iVal;
    uint16_t t_call_5f2f;

    lpshdef = rglpshdef[iplr] + ish;
    if (lpshdef->fFree == 0x0) {
        fstrcpy(psz, lpshdef->hul.szClass);
        if (iplr != idPlayer) {
            c = 0;
            iVal = 1;
            for (i = 0; i < 16; i++) {
                if (i != ish && rglpshdef[iplr][i].fFree == 0x0 && fstrcmp(psz, rglpshdef[iplr][i].hul.szClass) == 0) {
                    c = c + 1;
                    if (i < ish) {
                        iVal = iVal + 1;
                    }
                }
            }
            if (c != 0) {
                t_call_5f2f = strlen(psz);
                c = t_call_5f2f;
                psz[t_call_5f2f] = ' ';
                psz[c + 1] = '(';
                IntToRoman(iVal, psz + (c + 2));
                strcat(psz, ")");
            }
        }
    } else {
        *psz = 0;
    }
    return;
}

void DrawABunchOfStars(HDC hdc, RECT *prc) {
    int32_t lPixTot;
    int16_t iMax;
    int16_t dy;
    int16_t i;
    int16_t iClr;
    int16_t dx;
    RECT    rcOut;
    RECT    rc;

    rc = *prc;
    PushRandom(17, 11);
    InflateRect(&rc, -3, -3);
    dx = rc.right - rc.left;
    dy = rc.bottom - rc.top;
    lPixTot = (uint32_t)((int32_t)dx * (int32_t)(rc.bottom - rc.top));
    for (iClr = 0; iClr < 5; iClr++) {
        iMax = LOWORD((int32_t)(lPixTot / rgDSDivCnt[iClr]));
        for (i = 0; i < iMax; i++) {
            rcOut.left = Random(dx) + rc.left;
            rcOut.top = Random(dy) + rc.top;
            rcOut.right = rcOut.left + 1;
            rcOut.bottom = rcOut.top + 1;
            SetBkColor(hdc, rgcrDrawStars[iClr]);
            ExtTextOut(hdc, 0, 0, 0x2, &rcOut, 0x0, 0x0, 0x0);
        }
    }
    for (iClr = 0; iClr < 4; iClr++) {
        iMax = LOWORD((int32_t)(lPixTot / rgDSDivCnt2[iClr])) + 1;
        for (i = 0; i < iMax; i++) {
            rcOut.left = Random(dx) + rc.left;
            rcOut.top = Random(dy) + rc.top;
            rcOut.right = rcOut.left + 3;
            rcOut.bottom = rcOut.top + 3;
            SetBkColor(hdc, rgcrDrawStars2b[iClr]);
            ExtTextOut(hdc, 0, 0, 0x2, &rcOut, 0x0, 0x0, 0x0);
            SetBkColor(hdc, rgcrDrawStars2a[iClr]);
            InflateRect(&rcOut, -1, 0);
            ExtTextOut(hdc, 0, 0, 0x2, &rcOut, 0x0, 0x0, 0x0);
            InflateRect(&rcOut, 1, -1);
            ExtTextOut(hdc, 0, 0, 0x2, &rcOut, 0x0, 0x0, 0x0);
        }
    }
    PopRandom();
    return;
}

int32_t LongFromSerialCh(char ch) {
    int32_t l;

    if ((int16_t)ch < 'A' || (int16_t)ch > 'Z') {
        l = (int32_t)((int16_t)ch - 22);
    } else {
        l = (int32_t)((int16_t)ch - 65);
    }
    if (l < 32) {
        return l ^ 0x15;
    }
    return l;
}

int16_t FValidSerialNo(char *psz, int32_t *plSerial) {
    int32_t lBuild;
    int16_t i;
    int32_t lCur;
    int32_t lSerial;
    int32_t l;
    int32_t t_call_633d;
    int32_t t_call_6376;
    int32_t t_call_63af;
    int32_t t_call_63e8;
    int16_t t_call_64c4;

    lSerial = LongFromSerialCh((int16_t)*psz);
    if (lSerial < 32) {
        lSerial = lSerial ^ 0x15;
    }
    t_call_633d = LongFromSerialCh((int16_t)psz[1]);
    lSerial = (uint32_t)(lSerial * 36) + t_call_633d;
    t_call_6376 = LongFromSerialCh((int16_t)psz[4]);
    lSerial = (uint32_t)(lSerial * 36) + t_call_6376;
    t_call_63af = LongFromSerialCh((int16_t)psz[7]);
    lSerial = (uint32_t)(lSerial * 36) + t_call_63af;
    t_call_63e8 = LongFromSerialCh((int16_t)psz[3]);
    lSerial = (uint32_t)(lSerial * 36) + t_call_63e8;
    if (plSerial != 0x0) {
        *plSerial = lSerial;
    }
    PushRandom(11, 17);
    lCur = lSerial;
    Randomize2(lCur);
    lCur = (int32_t)(lCur >> 0xe);
    lBuild = 0;
    for (i = 0; i < 3; i++) {
        for (l = (uint32_t)(LOWORD(lCur) & 0xf); l >= 0; l--) {
            Random(256);
        }
        t_call_64c4 = Random(256);
        lBuild = (int32_t)(lBuild * 256) + (int32_t)t_call_64c4;
        lCur = (int32_t)(lCur >> 0x4);
    }
    PopRandom();
    l = LongFromSerialCh((int16_t)psz[2]);
    if (l != (int32_t)(lBuild % 36)) {
        return 0;
    }
    lBuild = (int32_t)(lBuild / 36);
    l = LongFromSerialCh((int16_t)psz[5]);
    if (l != (int32_t)(lBuild % 36)) {
        return 0;
    }
    lBuild = (int32_t)(lBuild / 36);
    l = LongFromSerialCh((int16_t)psz[6]);
    if (l != (int32_t)(lBuild % 36)) {
        return 0;
    }
    return 1;
}

int16_t FMatchTarget(FLEET *lpflTarget, int16_t mdTarget, int16_t fExact) {
    int16_t imd;
    int16_t ish;

    switch (mdTarget) {
    default:
        if (fExact == 0)
            break;
        return 0;
    case 3:
    case 5:
        for (ish = 0; ish < 16; ish++) {
            if (lpflTarget->rgcsh[ish] != 0) {
                imd = LphuldefFromId(rglpshdef[lpflTarget->iPlayer][ish].hul.ihuldef)->imdCategory;
                if (imd >= 2 && imd <= 4)
                    break;
            }
        }
        if (mdTarget != 3) {
            if (ish == 16)
                break;
            return 0;
        }
        if (ish != 16)
            break;
        return 0;
    case 4:
        for (ish = 0; ish < 16; ish++) {
            if (lpflTarget->rgcsh[ish] != 0) {
                imd = LphuldefFromId(rglpshdef[lpflTarget->iPlayer][ish].hul.ihuldef)->imdCategory;
                if (imd == 1 || imd == 5)
                    break;
            }
        }
        if (ish != 16)
            break;
        return 0;
    case 6:
        for (ish = 0; ish < 16; ish++) {
            if (lpflTarget->rgcsh[ish] != 0) {
                imd = LphuldefFromId(rglpshdef[lpflTarget->iPlayer][ish].hul.ihuldef)->imdCategory;
                if (imd == 7)
                    break;
            }
        }
        if (ish != 16)
            break;
        return 0;
    case 7:
        for (ish = 0; ish < 16; ish++) {
            if (lpflTarget->rgcsh[ish] != 0) {
                imd = LphuldefFromId(rglpshdef[lpflTarget->iPlayer][ish].hul.ihuldef)->imdCategory;
                if (imd == 1)
                    break;
            }
        }
        if (ish == 16) {
            return 0;
        }
    }
    return 1;
}

void ValidateWaypoints() {
    int16_t mdTarget;
    FLEET  *lpflTarget;
    int16_t ifl2;
    int32_t wt;
    FLEET  *lpflMatch;
    int32_t wtMatch;
    ORDER  *lpord;
    int16_t ifl;
    THING  *lpth;
    FLEET  *lpfl;
    int16_t cFound;
    int16_t iord;
    FLEET  *lpfl2;
    int16_t iplrHi;

    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->lpplord->rgord[0].grobj != grobjPlanet && lpfl->lpplord->rgord[0].grTask != grTaskXfer && lpfl->lpplord->rgord[0].grTask != grTaskMerge &&
            lpfl->fMark == 0x0) {
            if (lpfl->idPlanet != -1) {
                lpfl->lpplord->rgord[0].grobj = grobjPlanet;
                lpfl->lpplord->rgord[0].id = lpfl->idPlanet;
            } else {
                lpfl->lpplord->rgord[0].grobj = grobjOther;
                lpfl->lpplord->rgord[0].id = 0;
            }
        }
        if (lpfl->fDead == 0x0) {
            if (lpfl->pt.x >= 1000) {
                if (lpfl->pt.x > dGal + 1000) {
                    lpfl->pt.x = dGal + 1000;
                    lpfl->lpplord->rgord[0].pt.x = dGal + 1000;
                }
            } else {
                lpfl->pt.x = 1000;
                lpfl->lpplord->rgord[0].pt.x = 1000;
            }
            if (lpfl->pt.y >= 1000) {
                if (lpfl->pt.y > dGal + 1000) {
                    lpfl->pt.y = dGal + 1000;
                    lpfl->lpplord->rgord[0].pt.y = dGal + 1000;
                }
            } else {
                lpfl->pt.y = 1000;
                lpfl->lpplord->rgord[0].pt.y = 1000;
            }
            if (lpfl->cord > 1 || lpfl->fMark != 0x0) {
                iord = lpfl->fMark == 0x0 ? 1 : 0;
                lpord = &lpfl->lpplord->rgord[iord];
                while (iord < lpfl->cord) {
                    if (lpord->grobj != grobjThing) {
                        if (lpord->grobj == grobjFleet && lpord->fNoAutoTrack == 0x0) {
                            lpflTarget = LpflFromId(lpord->id);
                            if (lpflTarget != 0x0 && lpflTarget->pt.x == lpord->pt.x && lpflTarget->pt.y == lpord->pt.y &&
                                (lpflTarget->fCompChg == 0x0 || lpflTarget->iPlayer == lpfl->iPlayer)) {
                                if (lpflTarget != 0x0) {
                                    lpflTarget->fTargeted = 0x1;
                                }
                            } else {
                                lpflTarget = 0x0;
                                iplrHi = lpord->id & 0xfe00;
                                cFound = 0;
                                mdTarget = rglpbtlplan[lpfl->iPlayer][lpfl->iplan].mdTarget1;
                                wtMatch = 0;
                                lpflMatch = 0x0;
                                for (ifl2 = 0; ifl2 < cFleet; ifl2++) {
                                    lpfl2 = rglpfl[ifl2];
                                    if (rglpfl[ifl2] == 0x0)
                                        break;
                                    if (lpfl2->pt.x == lpord->pt.x && lpfl2->pt.y == lpord->pt.y && iplrHi == (lpfl2->id & 0xfe00) &&
                                        FMatchTarget(lpfl2, mdTarget, 1) != 0) {
                                        wt = WtFromLpfl(lpfl2);
                                        if (lpfl2->fTargeted == 0x0 && (wt > wtMatch || (wt == wtMatch && Random(2) == 0))) {
                                            wtMatch = wt;
                                            lpflMatch = lpfl2;
                                        }
                                    }
                                }
                                if (lpflMatch == 0x0) {
                                    for (ifl2 = 0; ifl2 < cFleet; ifl2++) {
                                        lpfl2 = rglpfl[ifl2];
                                        if (rglpfl[ifl2] == 0x0)
                                            break;
                                        if (lpfl2->pt.x == lpord->pt.x && lpfl2->pt.y == lpord->pt.y && iplrHi == (lpfl2->id & 0xfe00)) {
                                            if (FMatchTarget(lpfl2, mdTarget, 1) != 0) {
                                                wt = WtFromLpfl(lpfl2);
                                                if (wt > wtMatch || (wt == wtMatch && Random(2) == 0)) {
                                                    wtMatch = wt;
                                                    lpflMatch = lpfl2;
                                                }
                                            }
                                            cFound = cFound + 1;
                                            if (Random(cFound) == 0 && (cFound == 1 || lpfl2->fTargeted == 0x0 || Random(2) != 0)) {
                                                lpflTarget = lpfl2;
                                            }
                                        }
                                    }
                                }
                                if (lpflMatch != 0x0) {
                                    lpflTarget = lpflMatch;
                                }
                                if (lpflTarget != 0x0) {
                                    lpord->id = lpflTarget->id;
                                    lpord->pt = lpflTarget->pt;
                                    lpflTarget->fTargeted = 0x1;
                                }
                            }
                        }
                    } else {
                        lpth = LpthFromId(lpord->id);
                        if (lpth != 0x0 && (lpth->ith != ithWormhole || (0x1 << lpfl->iPlayer & lpth->thw.grbitPlr) != 0x0 ||
                                            (lpth->pt.x == lpord->pt.x && lpth->pt.y == lpord->pt.y))) {
                            lpord->pt = lpth->pt;
                        } else {
                            if (lpth != 0x0) {
                                FSendPlrMsg(lpfl->iPlayer, 248, 0x8000 | lpfl->id, lpfl->id, 0, 0, 0, 0, 0, 0);
                            }
                            lpord->grobj = grobjOther;
                            lpord->id = iord;
                        }
                    }
                    iord = iord + 1;
                    lpord = lpord + 1;
                }
            }
        }
    }
    return;
}

int32_t ChgPopFromPlanet(PLANET *lppl, int16_t fUpdate) {
    int32_t lMaxPop;
    int16_t fPopDied;
    int32_t lPopIncDelta;
    int16_t DeltaCur;
    int32_t pctGrow100;
    int16_t pctDesire;
    int32_t lPopInc100;
    int32_t lPopInc;
    int32_t lPopOld;
    int32_t pctRetard;
    int32_t pctFull;
    int32_t t_merge_7168_0001;
    int32_t t_merge_7397_0001;

    fPopDied = 0;
    if (lppl->iPlayer != -1 && lppl->rgwtMin[3] != 0) {
        pctDesire = PctPlanetDesirability(lppl, lppl->iPlayer);
        lPopOld = lppl->rgwtMin[3];
        fPopDied = 1;
        if (pctDesire >= 0) {
            lMaxPop = CalcPlanetMaxPop(lppl->id, lppl->iPlayer);
            pctGrow100 = (int32_t)(PctTrueMaxGrowth(lppl->iPlayer) * pctDesire);
            if (gd.fGeneratingTurn != 0x0 && rgplr[lppl->iPlayer].fCheater != 0x0) {
                pctGrow100 = (int32_t)(pctGrow100 >> 0x1);
            }
            if (lPopOld > (int32_t)(lMaxPop / 4)) {
                pctFull = (int32_t)((int32_t)(lPopOld * 1000) / lMaxPop);
                if (lPopOld < lMaxPop) {
                    pctRetard = 1000 - pctFull;
                    pctRetard = (uint32_t)(pctRetard * pctRetard);
                    if (pctGrow100 < 1000) {
                        pctGrow100 = (int32_t)((int32_t)(pctGrow100 * pctRetard) / 562500);
                    } else {
                        pctGrow100 = (uint32_t)((int32_t)((int32_t)((int32_t)(pctGrow100 / 10) * pctRetard) / 562500) * 10);
                    }
                } else {
                    if (lPopOld < lMaxPop + 10) {
                        return 0;
                    }
                    t_merge_7397_0001 = (int32_t)0xfffffed4 <= 99 - (int32_t)(pctFull / 10) ? 99 - (int32_t)(pctFull / 10) : -300;
                    pctRetard = t_merge_7397_0001;
                    pctGrow100 = (int32_t)(pctRetard * 4);
                }
            }
            lPopInc100 = (uint32_t)(lPopOld * (int32_t)(pctGrow100 / 100));
            if (lPopInc100 < 10000000) {
                lPopInc100 = (int32_t)((int32_t)(lPopOld * pctGrow100) / 100);
            }
            lPopInc = (int32_t)(lPopInc100 / 100);
            lPopIncDelta = (int32_t)(lPopInc100 % 100);
            if (lPopInc == 0 && lPopIncDelta == 0) {
                lPopIncDelta = 1;
            }
            DeltaCur = lppl->iDeltaPop + LOWORD(lPopIncDelta);
            if (DeltaCur < 100) {
                if (DeltaCur < 0) {
                    lPopInc = lPopInc - 1;
                    DeltaCur = DeltaCur + 100;
                }
            } else {
                lPopInc = lPopInc + 1;
                DeltaCur = DeltaCur - 100;
            }
        } else {
            t_merge_7168_0001 =
                (int32_t)0x1 <= (int32_t)((int32_t)(lPopOld * (int32_t)-pctDesire) / 0xa) ? (int32_t)((int32_t)(lPopOld * (int32_t)-pctDesire) / 0xa) : 1;
            lPopInc100 = t_merge_7168_0001;
            lPopInc = (int32_t)(lPopInc100 / 100);
            lPopIncDelta = (int32_t)(lPopInc100 % 100);
            if (lPopInc == 0 && lPopIncDelta == 0) {
                lPopIncDelta = 1;
            }
            DeltaCur = lppl->iDeltaPop - LOWORD(lPopIncDelta);
            if (DeltaCur < 0) {
                lPopInc = lPopInc + 1;
                DeltaCur = DeltaCur + 100;
            }
            lPopInc = -lPopInc;
        }
        if (fUpdate != 0) {
            lppl->iDeltaPop = LOWORD((uint32_t)DeltaCur);
            lppl->rgwtMin[3] = lppl->rgwtMin[3] + lPopInc;
        }
        return lPopInc;
    }
    return 0;
}

int16_t FCanFleetUseStargates(FLEET *lpfl, POINT16 ptSrc, POINT16 ptDst) {
    int16_t  dTravel;
    PLANET  *lpplDst;
    int16_t  pctDmg;
    int16_t  fSrcPlanet;
    int16_t  fUncertain;
    int16_t  i;
    int16_t  fDanger;
    PLANET  *lpplSrc;
    int16_t  isbsDst;
    int16_t  fCargo;
    int16_t  ishdef;
    int16_t  isbsSrc;
    SCAN     scan;
    int16_t  t_call_78d1;
    uint16_t t_merge_7938_0001;

    fUncertain = 0;
    if (FFindNearestObject(ptDst, 0x81, &scan) != 0) {
        lpplDst = LpplFromId(scan.idpl);
        if (lpplDst != 0x0 && lpfl->iPlayer == lpplDst->iPlayer) {
            isbsDst = IStargateFromLppl(lpplDst);
            if (isbsDst < 0) {
                return 0;
            }
        } else {
            if (lpplDst != 0x0 && lpplDst->iPlayer == -1 && lpplDst->turn == game.turn) {
                return 0;
            }
            fUncertain = 1;
        }
        lpplSrc = 0x0;
        fSrcPlanet = 0;
        if (FFindNearestObject(ptSrc, 0x81, &scan) != 0) {
            fSrcPlanet = 1;
            lpplSrc = LpplFromId(scan.idpl);
            if (lpplSrc != 0x0 && lpfl->iPlayer == lpplSrc->iPlayer) {
                isbsSrc = IStargateFromLppl(lpplSrc);
                if (isbsSrc >= 0) {
                    if (fUncertain == 0)
                        goto L_77e5;
                    return -1;
                }
            }
        }
        if (FFleetCanJumpgate(lpfl) == 0) {
            if (fSrcPlanet != 0 && (lpplSrc == 0x0 || lpplSrc->iPlayer != -1 || lpplSrc->turn != game.turn)) {
                if (lpplSrc != 0x0 && lpplSrc->iPlayer == lpfl->iPlayer) {
                    return 0;
                }
                return -1;
            }
            return 0;
        }
        if (fUncertain != 0) {
            return -1;
        }
        isbsSrc = isbsDst;
    L_77e5:
        fCargo = 0;
        if (GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) != raStargate) {
            for (i = 0; i <= 3; i++) {
                if (lpfl->rgwtMin[i] > 0) {
                    fCargo = 1;
                }
            }
        }
        dTravel = LOWORD((int32_t)DGetDistance(ptSrc.x, ptSrc.y, ptDst.x, ptDst.y));
        fDanger = 0;
        for (ishdef = 0; ishdef < 16; ishdef++) {
            if (lpfl->rgcsh[ishdef] != 0) {
                t_call_78d1 = MdCalcStargateDamage(isbsSrc, isbsDst, dTravel, rglpshdef[lpfl->iPlayer][ishdef].hul.wtEmpty, &pctDmg);
                switch (t_call_78d1) {
                case -2:
                case -1:
                case 0:
                    return 0;
                case 1:
                    if (pctDmg > 0) {
                        fDanger = 1;
                    }
                default:
                }
            }
        }
        t_merge_7938_0001 = fDanger == 0 ? 0x0 : 0x2;
        return t_merge_7938_0001 + 0x1 + (fCargo == 0 ? 0x0 : 0x4);
    }
    return 0;
}

int16_t FFleetCanJumpgate(FLEET *lpfl) {
    HS     *lphs;
    int16_t chs;
    int16_t i;
    int16_t j;

    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            lphs = rglpshdef[lpfl->iPlayer][i].hul.rghs;
            chs = rglpshdef[lpfl->iPlayer][i].hul.chs;
            j = 0;
            for (; j < chs && (lphs->cItem == 0x0 || lphs->grhst != hstSpecialM || lphs->iItem != ispecialMJumpGate); lphs++) {
                j = j + 1;
            }
            if (j == chs) {
                return 0;
            }
        }
    }
    return 1;
}

int32_t WtFromLpfl(FLEET *lpfl) {
    int32_t cMass;
    int16_t i;

    cMass = 0;
    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            cMass = cMass + (uint32_t)((int32_t)lpfl->rgcsh[i] * (uint32_t)rglpshdef[lpfl->iPlayer][i].hul.wtEmpty);
        }
    }
    for (i = 0; i <= 3; i++) {
        cMass = cMass + lpfl->rgwtMin[i];
    }
    return cMass;
}

int16_t FCanBuildShdef(SHDEF *lpshdef, int16_t iplr) {
    int16_t j;
    int16_t iplrSav;
    PART    part;

    iplrSav = idPlayer;
    if (lpshdef->hul.ihuldef < ihuldefOrbitalFort) {
        part.hs.grhst = hstHull;
        part.hs.iItem = lpshdef->hul.ihuldef;
    } else {
        part.hs.grhst = hstSBHull;
        part.hs.iItem = lpshdef->hul.ihuldef - 32;
    }
    idPlayer = iplr;
    if (FLookupPart(&part) == 1) {
        for (j = 0; j < lpshdef->hul.chs; j++) {
            if (lpshdef->hul.rghs[j].cItem > 0x0) {
                part.hs = lpshdef->hul.rghs[j];
                if (FLookupPart(&part) != 1)
                    goto LFail;
            }
        }
        idPlayer = iplrSav;
        return 1;
    }
LFail:
    idPlayer = iplrSav;
    return 0;
}

int16_t IshFindSimilarDesign(HUL *lphul, int16_t iPlrDst) {
    SHDEF   *lpshdefDest;
    int16_t  i;
    int16_t  j;
    uint16_t t_scratch_mc;

    lpshdefDest = rglpshdef[iPlrDst];
    i = 0;
    while (1) {
        if (i >= 16) {
            return -1;
        }
        if (lpshdefDest->fFree == 0x0 && lpshdefDest->fGift != 0x0 && lpshdefDest->hul.ihuldef == lphul->ihuldef) {
            t_scratch_mc = lpshdefDest->hul.chs;
            if (t_scratch_mc == lphul->chs) {
                for (j = 0; j < lphul->chs && lphul->rghs[j].cItem == lpshdefDest->hul.rghs[j].cItem &&
                            (lphul->rghs[j].cItem <= 0x0 ||
                             (lphul->rghs[j].iItem == lpshdefDest->hul.rghs[j].iItem && lphul->rghs[j].grhst == lpshdefDest->hul.rghs[j].grhst));
                     j++) {
                }
                if (j == lphul->chs)
                    break;
            }
        }
        i = i + 1;
        lpshdefDest = lpshdefDest + 1;
    }
    return i;
}

void DrawPlanetPrintDot(HDC hdc, int16_t x, int16_t y, int16_t iSize) {
    if (iSize != 0) {
        PatBlt(hdc, x - 5, y - 2, 11, 5, BLACKNESS);
        PatBlt(hdc, x - 2, y - 5, 5, 11, BLACKNESS);
        PatBlt(hdc, x - 4, y - 3, 9, 7, BLACKNESS);
        PatBlt(hdc, x - 3, y - 4, 7, 9, BLACKNESS);
    } else {
        PatBlt(hdc, x - 3, y - 1, 7, 3, BLACKNESS);
        PatBlt(hdc, x - 1, y - 3, 3, 7, BLACKNESS);
        PatBlt(hdc, x - 2, y - 2, 5, 5, BLACKNESS);
    }
    return;
}

void ClearFile(int16_t dt) {
    char *pch;
    char  szFile[256];

    strcpy(szFile, szBase);
    pch = strrchr(szFile, 46);
    if (pch == 0x0) {
        strcat(szFile, ".");
    } else {
        pch[1] = 0;
    }
    strcat(szFile, mpdtsz[dt]);
    remove(szFile);
    return;
}

void OutputSz(int16_t dt, char *sz) {
    char szTime[100];
    char szFile[256];
    char szDate[100];
    char szTemp[256];

    _wsprintf(szFile, "%s.%s", szBase, mpdtsz[dt]);
    if (access(szFile, 0) == -1) {
        _wsprintf(szTemp, "Stars! %s\r\n\r\n", SzVersion());
        OutputFileString(szFile, szTemp);
    }
    strdate(szDate);
    strtime(szTime);
    _wsprintf(szTemp, "%s %s - %s\r\n", szDate, szTime, sz);
    OutputFileString(szFile, szTemp);
    return;
}

void TurnLog(StringId ids) {
    char szTemp[256];

    if (ini.fLogging != 0x0) {
        _wsprintf(szTemp, PszFormatIds(ids, 0x0), game.turn + 0x961);
        OutputSz(6, szTemp);
    }
    return;
}
