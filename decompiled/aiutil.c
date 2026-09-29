#include "common.h"

int32_t vrgAiPacketDist[2] = {7056, 50625};
uint8_t vrgSBAip[85] = {34, 35, 37, 0,  17, 10, 11, 38, 19, 36, 34, 9,  34, 0,  17, 36, 37, 19, 0,  9,  4,  10, 11, 19, 38, 19, 35, 11, 0,
                        37, 10, 19, 38, 20, 36, 20, 17, 36, 37, 19, 3,  34, 35, 11, 11, 37, 10, 19, 0,  20, 38, 34, 17, 19, 9,  20, 36, 19,
                        35, 37, 0,  17, 10, 11, 38, 19, 36, 19, 9,  34, 35, 11, 0,  37, 10, 19, 38, 20, 36, 34, 17, 36, 37, 19, 3};
uint8_t vrgSBMacAisb[6] = {17, 57, 25, 0, 69, 41};
AIPART  vrgAiParts[150] = {{.ibit = 5, .iItem = 7, .cItem = 8},
                           {.ibit = 5, .iItem = 11, .cItem = 4},
                           {.ibit = 4, .iItem = 18, .cItem = 1},
                           {.ibit = 4, .iItem = 20, .cItem = 1},
                           {.ibit = 4, .iItem = 13, .cItem = 1},
                           {.ibit = 4, .iItem = 7, .cItem = 1},
                           {.ibit = 4, .iItem = 23, .cItem = 1},
                           {.ibit = 4, .iItem = 17, .cItem = 1},
                           {.ibit = 4, .iItem = 11, .cItem = 1},
                           {.ibit = 4, .iItem = 5, .cItem = 1},
                           {.ibit = 4, .iItem = 22, .cItem = 1},
                           {.ibit = 4, .iItem = 15, .cItem = 1},
                           {.ibit = 4, .iItem = 9, .cItem = 1},
                           {.ibit = 4, .iItem = 3, .cItem = 1},
                           {.ibit = 4, .iItem = 1, .cItem = 1},
                           {.ibit = 4, .cItem = 1},
                           {.ibit = 4, .iItem = 16, .cItem = 1},
                           {.ibit = 4, .iItem = 10, .cItem = 1},
                           {.ibit = 4, .iItem = 4, .cItem = 1},
                           {.ibit = 4, .iItem = 21, .cItem = 1},
                           {.ibit = 4, .iItem = 14, .cItem = 1},
                           {.ibit = 4, .iItem = 8, .cItem = 1},
                           {.ibit = 4, .iItem = 2, .cItem = 1},
                           {.ibit = 4, .iItem = 19, .cItem = 1},
                           {.ibit = 4, .iItem = 12, .cItem = 1},
                           {.ibit = 4, .iItem = 6, .cItem = 1},
                           {.iItem = 15, .cItem = 1},
                           {.iItem = 8, .cItem = 1},
                           {.iItem = 14, .cItem = 5},
                           {.iItem = 2, .cItem = 1},
                           {.ibit = 3, .iItem = 11, .cItem = 1},
                           {.ibit = 3, .iItem = 9, .cItem = 1},
                           {.ibit = 3, .iItem = 10, .cItem = 1},
                           {.ibit = 3, .iItem = 7, .cItem = 1},
                           {.ibit = 3, .iItem = 8, .cItem = 1},
                           {.ibit = 3, .iItem = 6, .cItem = 7},
                           {.ibit = 2, .iItem = 9, .cItem = 2},
                           {.ibit = 2, .iItem = 6, .cItem = 1},
                           {.ibit = 2, .iItem = 7, .cItem = 1},
                           {.ibit = 2, .iItem = 3, .cItem = 1},
                           {.ibit = 2, .iItem = 4, .cItem = 1},
                           {.ibit = 2, .iItem = 5, .cItem = 1},
                           {.ibit = 2, .iItem = 2, .cItem = 3},
                           {.ibit = 11, .iItem = 7, .cItem = 3},
                           {.ibit = 11, .iItem = 4, .cItem = 1},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 12, .iItem = 10, .cItem = 1},
                           {.ibit = 12, .iItem = 8, .cItem = 2},
                           {.ibit = 11, .iItem = 4, .cItem = 1},
                           {.ibit = 12, .iItem = 8, .cItem = 2},
                           {.ibit = 11, .iItem = 3, .cItem = 1},
                           {.ibit = 12, .iItem = 10, .cItem = 1},
                           {.ibit = 11, .iItem = 2, .cItem = 2},
                           {.ibit = 11, .iItem = 13, .cItem = 6},
                           {.ibit = 12, .iItem = 6, .cItem = 2},
                           {.ibit = 12, .iItem = 10, .cItem = 1},
                           {.ibit = 11, .iItem = 13, .cItem = 6},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 12, .iItem = 6, .cItem = 2},
                           {.ibit = 12, .iItem = 4, .cItem = 3},
                           {.ibit = 3, .iItem = 9, .cItem = 1},
                           {.ibit = 3, .iItem = 11, .cItem = 12},
                           {.ibit = 12, .iItem = 8, .cItem = 2},
                           {.ibit = 12, .iItem = 10, .cItem = 1},
                           {.ibit = 12, .iItem = 6, .cItem = 2},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 11, .iItem = 7, .cItem = 3},
                           {.ibit = 11, .iItem = 13, .cItem = 6},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 11, .iItem = 4, .cItem = 4},
                           {.ibit = 11, .iItem = 7, .cItem = 3},
                           {.ibit = 6, .iItem = 8, .cItem = 1},
                           {.ibit = 6, .iItem = 4, .cItem = 5},
                           {.ibit = 6, .iItem = 8, .cItem = 1},
                           {.ibit = 6, .iItem = 9, .cItem = 1},
                           {.ibit = 6, .iItem = 14, .cItem = 5},
                           {.ibit = 6, .iItem = 8, .cItem = 1},
                           {.ibit = 6, .iItem = 14, .cItem = 5},
                           {.ibit = 6, .iItem = 4, .cItem = 5},
                           {.ibit = 6, .iItem = 9, .cItem = 1},
                           {.iItem = 15, .cItem = 1},
                           {.iItem = 10, .cItem = 1},
                           {.ibit = 8, .iItem = 3, .cItem = 4},
                           {.ibit = 1, .iItem = 12, .cItem = 1},
                           {.ibit = 1, .iItem = 14, .cItem = 1},
                           {.ibit = 1, .iItem = 8, .cItem = 1},
                           {.ibit = 1, .iItem = 6, .cItem = 1},
                           {.ibit = 1, .iItem = 7, .cItem = 1},
                           {.ibit = 1, .iItem = 9, .cItem = 1},
                           {.ibit = 1, .iItem = 4, .cItem = 1},
                           {.ibit = 1, .iItem = 14, .cItem = 1},
                           {.ibit = 1, .iItem = 5, .cItem = 1},
                           {.ibit = 1, .iItem = 6, .cItem = 7},
                           {.ibit = 7, .iItem = 6, .cItem = 7},
                           {.ibit = 7, .iItem = 7, .cItem = 1},
                           {.iItem = 9, .cItem = 2},
                           {.iItem = 10, .cItem = 1},
                           {.iItem = 13, .cItem = 3},
                           {.iItem = 6, .cItem = 4},
                           {.ibit = 12, .iItem = 1, .cItem = 2},
                           {.ibit = 8, .iItem = 9, .cItem = 3},
                           {.ibit = 4, .iItem = 18, .cItem = 1},
                           {.ibit = 9, .iItem = 15, .cItem = 9},
                           {.ibit = 11, .iItem = 4, .cItem = 4},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 11, .iItem = 7, .cItem = 3},
                           {.ibit = 5, .iItem = 11, .cItem = 1},
                           {.ibit = 5, .iItem = 6, .cItem = 1},
                           {.ibit = 5, .iItem = 10, .cItem = 1},
                           {.ibit = 5, .iItem = 5, .cItem = 6},
                           {.ibit = 4, .iItem = 23, .cItem = 2},
                           {.ibit = 4, .iItem = 17, .cItem = 1},
                           {.ibit = 4, .iItem = 15, .cItem = 1},
                           {.ibit = 4, .iItem = 11, .cItem = 1},
                           {.ibit = 4, .iItem = 9, .cItem = 1},
                           {.ibit = 4, .iItem = 5, .cItem = 1},
                           {.ibit = 4, .iItem = 3, .cItem = 1},
                           {.ibit = 4, .iItem = 1, .cItem = 2},
                           {.ibit = 2, .iItem = 6, .cItem = 1},
                           {.ibit = 2, .iItem = 9, .cItem = 10},
                           {.ibit = 4, .iItem = 20, .cItem = 1},
                           {.ibit = 4, .iItem = 13, .cItem = 1},
                           {.ibit = 4, .iItem = 7, .cItem = 1},
                           {.ibit = 4, .iItem = 5, .cItem = 1},
                           {.ibit = 4, .cItem = 1},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 11, .iItem = 4, .cItem = 4},
                           {.ibit = 12, .iItem = 1, .cItem = 1},
                           {.ibit = 3, .iItem = 9, .cItem = 1},
                           {.ibit = 11, .iItem = 11, .cItem = 4},
                           {.ibit = 12, .iItem = 8, .cItem = 2},
                           {.ibit = 12, .iItem = 10, .cItem = 1},
                           {.ibit = 12, .iItem = 6, .cItem = 2},
                           {.ibit = 7, .iItem = 6, .cItem = 4},
                           {.ibit = 7, .iItem = 6, .cItem = 2},
                           {.ibit = 7, .cItem = 1},
                           {.iItem = 15, .cItem = 5},
                           {.iItem = 2, .cItem = 1}};

int16_t FCreateAiShdef(int16_t ishdef, int16_t ihul, uint8_t *rgaip) {
    StringId ids;
    int32_t  grbitHull;
    int16_t  cItem;
    int16_t  ihs;
    HUL     *lphul;
    PART     part;
    SHDEF    shdef;

    memset(&shdef, 0, sizeof(SHDEF));
    if (FLookupPartX(&part, ishdef < 0 ? 0x400 : 0x4000, ihul) == 1) {
        lphul = part.phul;
        shdef.hul = *lphul;
        shdef.hul.ihuldef = lphul->ihuldef;
        ihs = 0;
        while (1) {
            if (ihs >= lphul->chs)
                goto L_0254;
            if (FGetAIPart(rgaip[ihs], &part) == 0)
                break;
            part.hs.cItem = lphul->rghs[ihs].cItem;
            shdef.hul.rghs[ihs] = part.hs;
            ihs = ihs + 1;
        }
        return 0;
    L_0254:
        shdef.hul.chs = LOBYTE(ihs);
        shdef.fFree = 0x0;
        if (ishdef >= 0) {
            ids = idsUniverseDefinitionFileSeemsMissingCorrupt;
            grbitHull = (int32_t)(0x1 << ihul);
            if ((grbitHull & 0x780) != 0x0) {
                ids = idsLyingBastard;
                cItem = 16;
            } else if ((grbitHull & 0x40) != 0x0) {
                ids = idsScrapper;
                cItem = 16;
            } else if ((grbitHull & 0x70) != 0x0) {
                ids = idsEasterBunny;
                cItem = 10;
            } else if ((grbitHull & 0xf0000) != 0x0) {
                ids = idsPidgeon;
                cItem = 12;
            } else if ((grbitHull & 0xf) != 0x0) {
                ids = idsGlovebox;
                cItem = 8;
            } else if ((grbitHull & 0x1f00000) != 0x0) {
                ids = idsGroundHog;
                cItem = 8;
            } else if ((grbitHull & 0x3800) != 0x0) {
                ids = idsPricklyPear;
                cItem = 8;
            } else if ((grbitHull & 0xc000) != 0x0) {
                ids = idsEgg;
                cItem = 8;
            } else {
                ids = idsZombie;
                cItem = 8;
            }
            shdef.ishdef = ishdef;
            PickANameAndBmp(&shdef, ids, cItem, lphul->ibmp);
            return FChangeAiShdef(&shdef, ishdef);
        }
        shdefBuild = shdef;
        return 1;
    }
    return 0;
}

int16_t FGetAIPart(int16_t aip, PART *ppart) {
    int16_t cTry;
    int16_t iOffset;
    int16_t i;
    int16_t cItem;
    PART    part;
    int16_t t_047d;
    int16_t t_04ee;

    iOffset = 0;
    for (i = 0; i < aip; i++) {
        iOffset = iOffset + vrgcAiParts[i];
    }
    cTry = vrgcAiParts[aip];
    while (1) {
        t_047d = cTry;
        cTry = cTry - 1;
        if (t_047d <= 0) {
            return 0;
        }
        part.hs.grhst = 0x1 << vrgAiParts[iOffset].ibit;
        part.hs.iItem = vrgAiParts[iOffset].iItem;
        part.hs.cItem = 0x0;
        cItem = vrgAiParts[iOffset].cItem;
        while (1) {
            t_04ee = cItem;
            cItem = cItem - 1;
            if (t_04ee <= 0)
                break;
            if (FLookupPart(&part) == 1)
                goto L_0511;
            part.hs.iItem = part.hs.iItem - 0x1;
        }
        iOffset = iOffset + 1;
    }
L_0511:
    *ppart = part;
    return 1;
}

void PickANameAndBmp(SHDEF *pshdef, StringId ids, int16_t cids, int16_t ibmpStart) {
    int16_t i;
    int16_t rgfBmpUsed[4];
    int16_t ishdef;

    memset(rgfBmpUsed, 0, 0x8);
    if (pshdef->ishdef < 0x10) {
        for (ishdef = 0; ishdef < 16; ishdef++) {
            if (rgshdef[ishdef].fFree == 0x0 && rgshdef[ishdef].hul.ihuldef == pshdef->hul.ihuldef) {
                rgfBmpUsed[rgshdef[ishdef].hul.ibmp + -ibmpStart] = 1;
            }
        }
    } else {
        for (ishdef = 0; ishdef < 10; ishdef++) {
            if (rglpshdefSB[idPlayer][ishdef].fFree == 0x0 && rglpshdefSB[idPlayer][ishdef].hul.ihuldef == pshdef->hul.ihuldef) {
                rgfBmpUsed[rglpshdefSB[idPlayer][ishdef].hul.ibmp + -ibmpStart] = 1;
            }
        }
    }
    for (i = 0; i < 4 && rgfBmpUsed[i] != 0; i++) {
    }
    if (i == 4) {
        i = Random(4);
    }
    pshdef->hul.ibmp = ibmpStart + i;
    for (i = 0; i < 20; i++) {
        CchGetString(Random(cids) + ids, pshdef->hul.szClass);
        if (pshdef->ishdef < 0x10) {
            for (ishdef = 0; ishdef < 16 && (rgshdef[ishdef].fFree != 0x0 || strcmp(pshdef->hul.szClass, rgshdef[ishdef].hul.szClass) != 0); ishdef++) {
            }
            if (ishdef == 16) {
                return;
            }
        } else {
            for (ishdef = 0; ishdef < 10 && (rglpshdefSB[idPlayer][ishdef].fFree != 0x0 || rglpshdefSB[idPlayer][ishdef].hul.ihuldef != pshdef->hul.ihuldef ||
                                             fstrcmp(pshdef->hul.szClass, rglpshdefSB[idPlayer][ishdef].hul.szClass) != 0);
                 ishdef++) {
            }
            if (ishdef == 10) {
                return;
            }
        }
    }
    _wsprintf(pshdef->hul.szClass, "%s %d", PszGetCompressedString(Random(cids) + ids), Random(100));
    return;
}

int16_t FChangeAiShdef(SHDEF *pshdef, int16_t ishdef) {
    SHDEF  *lpshdefBase;
    int16_t iDir;
    int16_t ishdefWork;
    SHDEF   shdef;

    shdef = *pshdef;
    shdef.ishdef = ishdef;
    shdef.turn = game.turn;
    shdef.cExist = 0x0;
    shdef.cBuilt = 0x0;
    if (ishdef < 16) {
        lpshdefBase = rglpshdef[idPlayer];
        ishdefWork = ishdef;
    } else {
        lpshdefBase = rglpshdefSB[idPlayer];
        ishdefWork = ishdef - 16;
    }
    UpdateShdefCost(&shdef);
    if (shdef.fFree == lpshdefBase[ishdefWork].fFree) {
        if (lpshdefBase[ishdefWork].fFree == 0x0) {
            lpshdefBase[ishdefWork].wFlags = (lpshdefBase[ishdefWork].wFlags & 0xfdff) | 0x200;
            LogChangeShDef(lpshdefBase + ishdefWork);
        }
    } else {
        iDir = shdef.fFree == 0x0 ? 1 : -1;
        if (ishdef < 16) {
            rgplr[idPlayer].cShDef = rgplr[idPlayer].cShDef + LOBYTE(iDir);
        } else {
            rgplr[idPlayer].cshdefSB = rgplr[idPlayer].cshdefSB + iDir;
        }
    }
    lpshdefBase[ishdefWork] = shdef;
    LogChangeShDef(&shdef);
    return 1;
}

int16_t XferAiSupply(GrobjClass grobjSrc, int16_t idSrc, GrobjClass grobjDst, int16_t idDst, int16_t iSupply, int16_t cQuan) {
    int16_t dChg;
    int16_t iT;
    int32_t cAvailable;

    if (cQuan != 0) {
        if (cQuan < 0) {
            iT = grobjSrc;
            grobjSrc = grobjDst;
            grobjDst = iT;
            iT = idSrc;
            idSrc = idDst;
            idDst = iT;
            cQuan = -cQuan;
        }
        cAvailable = ChgCargo(grobjSrc, idSrc, iSupply, 0, 0x0);
        if ((int32_t)cQuan > cAvailable) {
            cQuan = LOWORD(cAvailable);
        }
        if (cQuan != 0) {
            dChg = LOWORD(ChgCargo(grobjDst, idDst, iSupply, (int32_t)cQuan, 0x0));
            if (dChg != 0) {
                ChgCargo(grobjSrc, idSrc, iSupply, (int32_t)-dChg, 0x0);
            }
            return dChg;
        }
        return 0;
    }
    return 0;
}

int16_t XferAiTroopers(int16_t idSrc, int16_t idDst, int16_t cQuan) {
    int32_t cAvailable;

    if (cQuan != 0) {
        cAvailable = ChgCargo(grobjFleet, idSrc, 3, 0, 0x0);
        if ((int32_t)cQuan > cAvailable) {
            cQuan = LOWORD(cAvailable);
        }
        if (cQuan != 0) {
            ChgCargo(grobjFleet, idSrc, 3, (int32_t)-cQuan, 0x0);
            ChgCargo(grobjPlanet, idDst, 3, (int32_t)cQuan, 0x0);
            return cQuan;
        }
        return 0;
    }
    return 0;
}

int16_t FColonizeAiFleet(FLEET *lpfl, int16_t idPlanet) {
    ORDER ord;

    ChangeMainObjSel(grobjFleet, lpfl->id);
    memset(&ord, 0, sizeof(ORDER));
    ord.pt = rgptPlan[idPlanet];
    ord.grobj = grobjPlanet;
    ord.id = idPlanet;
    ord.grTask = grTaskColonize;
    ord.fValidTask = 0x1;
    ord.iWarp = (uint16_t)IFindIdealWarp(lpfl, 1);
    if (FMoveAiFleet(lpfl, &ord, 0) != 0) {
        lpfl->fMark = 0x1;
        return 1;
    }
    return 0;
}

int16_t FGotoWormholeAiFleet(FLEET *lpfl, THING *lpthWorm) {
    ORDER ord;

    ChangeMainObjSel(grobjFleet, lpfl->id);
    memset(&ord, 0, sizeof(ORDER));
    ord.pt = lpthWorm->pt;
    ord.grobj = grobjThing;
    ord.id = lpthWorm->idFull;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x0;
    ord.iWarp = (uint16_t)IFindIdealWarp(lpfl, 1);
    if (FMoveAiFleet(lpfl, &ord, 0) != 0) {
        lpfl->fMark = 0x1;
        return 1;
    }
    return 0;
}

int16_t IdNearestColonizablePlanet(FLEET *lpflCol, THING **plpthWorm) {
    PLANET  *lpplMac;
    POINT16  pt;
    int32_t  dy;
    int32_t  d2;
    PLANET  *lppl;
    int16_t  idBest;
    int16_t  ifl;
    FLEET   *lpfl;
    int16_t  i;
    int16_t  iVal;
    int32_t  dx;
    uint8_t *lpb;
    int32_t  d2Cur;

    if (rgplr[idPlayer].idAi == 0x0 || rgplr[idPlayer].idAi == 0x5) {
        iVal = 0;
    } else {
        iVal = 16;
        if (fMarkedPlanets != 0)
            goto LFindNearest;
    }
    lpb = vlpbAiPlanet + 15;
    i = 0;
    while (i < game.cPlanMax) {
        *lpb = LOBYTE(iVal);
        i = i + 1;
        lpb = lpb + 16;
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == -1) {
            if (rgplr[idPlayer].idAi == 0x0 || rgplr[idPlayer].idAi == 0x5 || PctPlanetOptValue(lppl, idPlayer) >= 0) {
                iVal = 0;
            } else {
                iVal = 8;
            }
        } else if (lppl->iPlayer != idPlayer) {
            iVal = 2;
        } else {
            iVal = 1;
        }
        vlpbAiPlanet[lppl->id * 16 + 15] = LOBYTE(iVal);
    }
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->iPlayer == idPlayer && lpfl->cord > 1 && lpfl != lpflCol && lpfl->lpplord->rgord[1].grobj == grobjPlanet &&
            ((rgplr[idPlayer].idAi != 0x0 && rgplr[idPlayer].idAi != 0x5) || lpfl->lpplord->rgord[1].grTask == grTaskColonize)) {
            vlpbAiPlanet[lpfl->lpplord->rgord[1].id * 16 + 15] = 0x4;
        }
    }
LFindNearest:
    lpb = vlpbAiPlanet + 15;
    d2 = 99999999;
    pt = lpflCol->pt;
    idBest = -1;
    i = 0;
    while (i < game.cPlanMax) {
        if (*lpb <= 0x0) {
            dx = (int32_t)abs(pt.x - rgptPlan[i].x);
            dy = (int32_t)abs(pt.y - rgptPlan[i].y);
            if (dx < d2 && dy < d2) {
                d2Cur = (uint32_t)(dx * dx);
                if ((int32_t)(uint32_t)(dx * dx) < d2) {
                    d2Cur = d2Cur + (uint32_t)(dy * dy);
                    if (d2Cur < d2) {
                        d2 = d2Cur;
                        idBest = i;
                    }
                }
            }
        }
        i = i + 1;
        lpb = lpb + 16;
    }
    fMarkedPlanets = 1;
    if (plpthWorm != 0x0) {
        *plpthWorm = 0x0;
        if (lpflCol->idPlanet != -1 && game.turn < 0x78) {
            lppl = LpplFromId(lpflCol->idPlanet);
            if (lppl != 0x0 && lppl->iPlayer == idPlayer) {
                *plpthWorm = LpthWormFind(&pt, d2);
            }
        }
        if (*plpthWorm != 0x0) {
            idBest = -1;
        }
    }
    return idBest;
}

THING *LpthWormFind(POINT16 *ppt, int32_t d2) {
    int16_t  pctGood;
    int16_t  dy;
    int32_t  d2Worm;
    uint16_t grbitplr;
    THING   *lpth;
    THING   *lpthBest;
    int16_t  iVal;
    int16_t  dx;
    THING   *lpthMac;
    int32_t  d2Cur;

    lpthBest = 0x0;
    grbitplr = 0x1 << idPlayer;
    d2Worm = 1000000;
    pctGood = 0;
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (lpth->ith == ithWormhole) {
            dx = abs(ppt->x - lpth->pt.x);
            dy = abs(ppt->y - lpth->pt.y);
            d2Cur = (int32_t)(dx * dx + dy * dy);
            if (d2Cur <= (int32_t)(d2 * 4) && d2Cur <= 46656) {
                if ((lpth->thw.grbitPlrTrav & grbitplr) == 0x0) {
                    if (d2Cur <= d2) {
                        iVal = 90;
                    } else {
                        iVal = 50;
                    }
                } else {
                    iVal = PctWormholeMoves(lpth);
                    iVal = 70 - 10 * iVal;
                }
                if (iVal > pctGood || (iVal == pctGood && d2Cur < d2Worm)) {
                    pctGood = iVal;
                    lpthBest = lpth;
                    d2Worm = d2Cur;
                }
            }
        }
    }
    if (lpthBest == 0x0 || Random(100) >= pctGood) {
        return 0x0;
    }
    return lpthBest;
}

uint32_t UlFleetPower(FLEET *lpfl) {
    uint32_t ul;
    int16_t  iplr;
    int16_t  ishdef;

    ul = 0x0;
    iplr = lpfl->iPlayer;
    for (ishdef = 0; ishdef < 16; ishdef++) {
        if (lpfl->rgcsh[ishdef] != 0) {
            ul = ul + (uint32_t)(rglpshdef[iplr][ishdef].lPower * (int32_t)lpfl->rgcsh[ishdef]);
            if ((ul & 0x80000000) != 0x0)
                break;
        }
    }
    return ul;
}

int16_t IdNearestUnknownPlanet(FLEET *lpfl, THING **plpthWorm) {
    POINT16  pt;
    int32_t  dy;
    int32_t  d2;
    int16_t  idBest;
    int16_t  i;
    int32_t  dx;
    uint8_t *lpb;
    int32_t  d2Cur;

    if (fMarkedPlanets == 0) {
        IdNearestColonizablePlanet(lpfl, 0x0);
    }
    lpb = vlpbAiPlanet + 15;
    d2 = 99999999;
    pt = lpfl->pt;
    idBest = -1;
    i = 0;
    while (i < game.cPlanMax) {
        if (*lpb == 0x10) {
            dx = (int32_t)abs(pt.x - rgptPlan[i].x);
            dy = (int32_t)abs(pt.y - rgptPlan[i].y);
            if (dx < d2 && dy < d2) {
                d2Cur = (uint32_t)(dx * dx);
                if ((int32_t)(uint32_t)(dx * dx) < d2) {
                    d2Cur = d2Cur + (uint32_t)(dy * dy);
                    if (d2Cur < d2) {
                        d2 = d2Cur;
                        idBest = i;
                    }
                }
            }
        }
        i = i + 1;
        lpb = lpb + 16;
    }
    if (plpthWorm != 0x0) {
        *plpthWorm = 0x0;
        if (lpfl->idPlanet != -1 && Random(100) < 5) {
            *plpthWorm = LpthWormFind(&pt, d2);
        }
        if (*plpthWorm != 0x0) {
            idBest = -1;
        }
    }
    return idBest;
}

void AddMinesToBlockedQueues() {
    PROD     prod;
    int32_t  cMaxBuild;
    int16_t  etaBetterAlchemy;
    int32_t  cBuild;
    int16_t  etaFirst;
    PLANET  *lppl;
    int32_t  cResMine;
    int32_t  cRes;
    int16_t  ipl;
    int32_t  rgCost[4];
    PROD     rgprod[64];
    int16_t  etaBetterMines;
    uint32_t t_scratch_m136;

    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0x0)
            break;
        if (lppl->lpplprod != 0x0) {
            prod = lppl->lpplprod->rgprod[0];
            if (prod.grobj == grobjPlanet) {
                switch (prod.iItem) {
                case mdIdleMine:
                case iobjAlchemy:
                case mdIdleAlchemy:
                case mdIdleTerraform:
                    break;
                default:
                    goto L_18c8;
                }
                continue;
            }
        L_18c8:
            ChangeMainObjSel(grobjPlanet, lppl->id);
            PszProductionETA(&sel.pl, sel.pl.lpplprod, iobjMine, &etaFirst, 0x0);
            if (etaFirst != 1) {
                if (etaFirst == -1) {
                    etaFirst = 600;
                }
                GetProductionCosts(lppl, &prod, rgCost, idPlayer, 1);
                cRes = (int32_t)CResourcesAtPlanet(&sel.pl, idPlayer);
                if (sel.pl.fNoResearch == 0x0) {
                    cRes = cRes - (int32_t)((int32_t)(cRes * (int32_t)(int16_t)rgplr[idPlayer].pctResearch) / 0x64);
                }
                if (rgCost[3] <= (int32_t)(uint32_t)(cRes * (int32_t)(etaFirst - 1))) {
                    t_scratch_m136 = (uint32_t)sel.pl.cMines;
                    cMaxBuild = (int32_t)CMaxOperableMines(&sel.pl, idPlayer, 1) - t_scratch_m136;
                    if (cMaxBuild < 0) {
                        cMaxBuild = 0;
                    }
                    cResMine = (int32_t)GetRaceStat(&rgplr[idPlayer], rsMineBuild);
                    if ((int32_t)(uint32_t)(cResMine * cMaxBuild) <= cRes) {
                        cBuild = cMaxBuild;
                    } else {
                        cBuild = (int32_t)(cRes / cResMine);
                    }
                    InitProduction(rgprod);
                    if (cBuild <= 0) {
                        etaBetterMines = 700;
                        AddItemToQueue(0x3, 0x1, grobjPlanet, 0);
                        FinishProduction(1);
                    } else {
                        AddItemToQueue(0x8, LOWORD(cBuild), grobjPlanet, 0);
                        FinishProduction(1);
                        PszProductionETA(&sel.pl, sel.pl.lpplprod, iobjFactory, &etaBetterMines, 0x0);
                        if (etaBetterMines == -1) {
                            etaBetterMines = 700;
                        }
                        sel.pl.lpplprod->rgprod[0].cItem = 0x1;
                        sel.pl.lpplprod->rgprod[0].iItem = iobjAlchemy;
                    }
                    PszProductionETA(&sel.pl, sel.pl.lpplprod, iobjFactory, &etaBetterAlchemy, 0x0);
                    if (etaBetterAlchemy == -1) {
                        etaBetterAlchemy = 700;
                    }
                    if ((etaBetterAlchemy >= etaFirst || etaBetterAlchemy >= etaBetterMines) && cBuild >= 1) {
                        sel.pl.lpplprod->rgprod[0].iItem = mdIdleMine;
                        if (etaFirst < etaBetterMines || cBuild <= 0) {
                            sel.pl.lpplprod->iprodMac = sel.pl.lpplprod->iprodMac - 0x1;
                            fmemmove(sel.pl.lpplprod->rgprod, &sel.pl.lpplprod->rgprod[1], sel.pl.lpplprod->iprodMac * sizeof(PROD));
                        } else {
                            sel.pl.lpplprod->rgprod[0].cItem = LOWORD((uint32_t)LOWORD(cBuild));
                        }
                    }
                }
            }
        }
    }
    return;
}

int16_t FFleetInField(FLEET *lpfl, THING *lpth) {
    int16_t dy;
    int16_t dx;
    int32_t dxy2;

    dx = abs(lpfl->pt.x - lpth->pt.x);
    dy = abs(lpfl->pt.y - lpth->pt.y);
    dxy2 = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
    if (dxy2 < lpth->thm.cMines) {
        return 1;
    }
    return 0;
}

void SetAiFleetIdealSpeed(FLEET *lpfl, int16_t wtFuelMax, int16_t cMinefields, THING **rglpth) {
    THING        *lpth;
    int16_t       i;
    int16_t       j;
    int16_t       ith;
    THING        *lpthMac;
    int16_t       fMinefield;
    RaceAttribute t_call_1f5f;

    fMinefield = 0;
    if (cMinefields != 0) {
        if (cMinefields != -1) {
            ith = 0;
            while (1) {
                if (ith >= cMinefields)
                    goto LSelectFleet;
                if (FFleetInField(lpfl, rglpth[ith]) != 0)
                    break;
                ith = ith + 1;
            }
            lpth = rglpth[ith];
            fMinefield = 1;
        } else {
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            while (1) {
                if (lpth >= lpthMac)
                    goto LSelectFleet;
                if (lpth->ith == ithMinefield && lpth->iplr != idPlayer && lpth->thm.iType != 0x2 && FFleetInField(lpfl, lpth) != 0)
                    break;
                lpth = lpth + 1;
            }
            fMinefield = 1;
        }
    }
LSelectFleet:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if (fMinefield == 0) {
        i = IWarpBestForWaypoint(lpfl, &lpfl->lpplord->rgord[1]);
    } else {
        if (lpth->thm.iType == 0x1) {
            i = 6;
        } else if (lpth->thm.iType != 0x2) {
            i = 4;
            j = Random(10);
            if (j >= 4) {
                i = i + 1;
            }
        } else {
            i = 5;
            j = Random(10);
            if (j >= 7) {
                i = i + 1;
            }
        }
        t_call_1f5f = GetRaceStat(&rgplr[idPlayer], rsMajorAdv);
        if (t_call_1f5f == raStealth) {
            i = i + 1;
        }
    }
    if (i != lpfl->lpplord->rgord[1].iWarp) {
        sel.fl.lpplord->rgord[1].iWarp = i;
        FLookupFleet(-1, &sel.fl);
    }
    return;
}

int16_t IdTargetAttack(FLEET *lpfl, FLEET *lpflAtk, FLEET *lpflEnemy, int16_t fOnlyHumans) {
    FLEET   *lpflClosest;
    FLEET   *lpflT;
    int32_t  lDistBest;
    PLANET  *lpplMac;
    POINT16  pt;
    int16_t  dy;
    PLANET  *lppl;
    int32_t  lDist;
    int16_t  idClosest;
    int16_t  i;
    int16_t  cShipsAtk;
    int16_t  cShipsDst;
    PLANET  *lpplClosest;
    uint8_t *lpb;
    int16_t  dx;
    FLEET   *lpflAtk2;
    ORDER    ord;

    cShipsDst = 0;
    cShipsAtk = 0;
    for (i = 0; i < 16; i++) {
        cShipsAtk = cShipsAtk + lpfl->rgcsh[i];
    }
    lpflClosest = 0x0;
    lpflT = lpflEnemy;
    lDistBest = 1000000;
    pt = lpfl->pt;
    memset(&ord, 0, sizeof(ORDER));
    for (; lpflT != 0x0; lpflT = lpflT->lpflNext) {
        if (fOnlyHumans == 0 || rgplr[lpflT->iPlayer].fAi == 0x0) {
            lpflAtk2 = lpflAtk;
            cShipsDst = 0;
            for (; lpflAtk2 != 0x0; lpflAtk2 = lpflAtk2->lpflNext) {
                if (lpflAtk2 != lpfl && lpflAtk2->cord >= 2 && lpflAtk2->lpplord->rgord[1].id == lpflT->id && lpflAtk2->lpplord->rgord[1].grobj == grobjFleet) {
                    for (i = 0; i < 16; i++) {
                        cShipsDst = cShipsDst + lpflAtk2->rgcsh[i];
                    }
                }
            }
            if ((cShipsDst <= 0 || Random(3) != 0) && (5 * cShipsDst <= cShipsAtk || Random(15) != 0)) {
                dx = lpflT->pt.x - pt.x;
                dy = lpflT->pt.y - pt.y;
                lDist = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                if (lDist < lDistBest) {
                    lDistBest = lDist;
                    lpflClosest = lpflT;
                }
            }
        }
    }
    if (lDistBest < 32400) {
        ord.pt = lpflClosest->pt;
        ord.grobj = grobjFleet;
        ord.id = lpflClosest->id;
    } else {
        if (lpflClosest != 0x0) {
            if (lpfl->rgwtMin[4] < (int32_t)(LGetFleetStat(lpfl, 1) / 2) && FMoveToNearestStarbase(lpfl, 0) != 0) {
                return 0;
            }
            pt = lpflClosest->pt;
            lpplClosest = 0x0;
            lDistBest = 1000000;
            lppl = lpPlanets;
            lpplMac = lpPlanets + cPlanet;
            for (; lppl < lpplMac; lppl++) {
                for (lpflAtk2 = lpflAtk; lpflAtk2 != 0x0 && (lpflAtk2 == lpfl || lpflAtk2->cord < 2 || lpflAtk2->lpplord->rgord[1].id != lppl->id ||
                                                             lpflAtk2->lpplord->rgord[1].grobj != grobjPlanet);
                     lpflAtk2 = lpflAtk2->lpflNext) {
                }
                if (lpflAtk2 == 0x0) {
                    dx = rgptPlan[lppl->id].x - pt.x;
                    dy = rgptPlan[lppl->id].y - pt.y;
                    lDist = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                    if (lDist < lDistBest) {
                        lDistBest = lDist;
                        lpplClosest = lppl;
                    }
                }
            }
            if (lpplClosest != 0x0 && lpplClosest->id != lpfl->idPlanet) {
                ord.pt = rgptPlan[lpplClosest->id];
                ord.grobj = grobjPlanet;
                ord.id = lpplClosest->id;
                goto ThwakSumthin;
            }
        } else if (fOnlyHumans != 0) {
            return IdTargetAttack(lpfl, lpflAtk, lpflEnemy, 0);
        }
        pt = lpfl->pt;
        lpplClosest = 0x0;
        lDistBest = 1000000;
        lppl = lpPlanets;
        lpplMac = lpPlanets + cPlanet;
        for (; lppl < lpplMac; lppl++) {
            if (lppl->iPlayer != -1 && lppl->iPlayer != idPlayer) {
                dx = rgptPlan[lppl->id].x - pt.x;
                dy = rgptPlan[lppl->id].y - pt.y;
                lDist = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                if (lDist < lDistBest) {
                    lDistBest = lDist;
                    lpplClosest = lppl;
                }
            }
        }
        if (lpplClosest == 0x0 || lpplClosest->id == lpfl->idPlanet) {
            idClosest = -1;
            lDistBest = 1000000;
            lpb = vlpbAiPlanet + 9;
            i = 0;
            while (i < game.cPlanMax) {
                if (*lpb <= 0x0) {
                    dx = rgptPlan[i].x - pt.x;
                    dy = rgptPlan[i].y - pt.y;
                    lDist = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                    if (lDist < lDistBest) {
                        lDistBest = lDist;
                        idClosest = i;
                    }
                }
                i = i + 1;
                lpb = lpb + 16;
            }
            if (idClosest == -1) {
                ord.id = Random(game.cPlanMax);
                ord.grobj = grobjPlanet;
                ord.pt = rgptPlan[ord.id];
            } else {
                ord.pt = rgptPlan[idClosest];
                ord.grobj = grobjPlanet;
                ord.id = idClosest;
            }
        } else {
            ord.pt = rgptPlan[lpplClosest->id];
            ord.grobj = grobjPlanet;
            ord.id = lpplClosest->id;
        }
    }
ThwakSumthin:
    if (lpfl->cord <= 1 || lpfl->lpplord->rgord[1].grobj != grobjPlanet || ord.grobj != grobjPlanet) {
        ord.grTask = grTaskNone;
        ord.fValidTask = 0x1;
        ord.iWarp = 0x4;
        if (FMoveAiFleet(lpfl, &ord, 0) != 0) {
            return 0;
        }
        return -1;
    }
    return -1;
}

int16_t IdTargetFreighter(FLEET *lpflFr, PLANET *lpplHome) {
    int32_t  lWorst2;
    int32_t  scoreBest;
    PLANET  *lpplMac;
    int32_t  score;
    POINT16  pt;
    int16_t  dy;
    int32_t  lWorst;
    int16_t  pctFull;
    int16_t  idBest;
    PLANET  *lppl;
    int32_t  wtPlanCargo;
    FLEET   *lpfl;
    int32_t  wtCargoMax;
    int16_t  ifl;
    int16_t  i;
    int16_t  iWorst2;
    int32_t  wtCargoFree;
    THING   *lpthBest;
    int16_t  iWorst;
    PLANET  *lpplBest;
    int16_t  ishFreighter;
    int16_t  pctHere;
    int16_t  dx;
    uint8_t *lpb;
    int16_t  fNeedy;
    ORDER    ord;
    int16_t  fSalvage;
    int32_t  l;
    int16_t  t_merge_29b2_0001;
    uint16_t t_merge_33cf_0001;

    fSalvage = 0;
    lpb = vlpbAiPlanet + 14;
    i = 0;
    while (i < game.cPlanMax) {
        *lpb = 0x0;
        i = i + 1;
        lpb = lpb + 16;
    }
    for (ishFreighter = 0; ishFreighter < 16 && lpflFr->rgcsh[ishFreighter] <= 0; ishFreighter++) {
    }
    lWorst = 1000000000;
    lWorst2 = 1000000000;
    for (i = 0; i < 3; i++) {
        if (lpplHome->rgwtMin[i] < lWorst) {
            lWorst2 = lWorst;
            iWorst2 = iWorst;
            lWorst = lpplHome->rgwtMin[i];
            iWorst = i;
        }
    }
    t_merge_29b2_0001 = lWorst < (int32_t)(lWorst2 >> 0x1) ? 1 : 0;
    fNeedy = t_merge_29b2_0001;
    if (lWorst < (int32_t)(lWorst2 >> 0x2)) {
        fNeedy = fNeedy + 1;
    }
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->iPlayer == idPlayer && lpfl->cord > 1 && lpfl != lpflFr && lpfl->lpplord->rgord[1].grobj == grobjPlanet && lpfl->rgcsh[ishFreighter] != 0 &&
            lpfl->lpplord->rgord[1].id != lpplHome->id) {
            vlpbAiPlanet[lpfl->lpplord->rgord[1].id * 16 + 14] = 0x1;
        }
    }
    wtCargoMax = LGetFleetStat(lpflFr, 2);
    wtCargoFree = GetCargoFree(lpflFr);
    if (wtCargoMax <= 0) {
        return -1;
    }
    pctFull = 100 - LOWORD((int32_t)((int32_t)(wtCargoFree * 100) / wtCargoMax));
    scoreBest = 0;
    idBest = -1;
    pt = lpflFr->pt;
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->id != lpflFr->idPlanet && vlpbAiPlanet[lppl->id * 16 + 14] == 0x0) {
            if (lppl->iPlayer != -1 || (vlpbAiPlanet[lppl->id * 16 + 1] & 0x80) == 0x0) {
                if (lppl->iPlayer != idPlayer) {
                    if (rgplr[idPlayer].idAi == 0x0 && lpplHome->id == lpflFr->idPlanet &&
                        ((lpplHome->rgwtMin[3] > 1100 && lppl->uPopGuess < 0x3e) || (lpplHome->rgwtMin[3] > 300 && lppl->uPopGuess < 0xf))) {
                        pctHere = 65;
                        goto ScorePctHere;
                    }
                    if (vlpbAiPlanet[lppl->id * 16 + 3] == 0x0 || (vlpbAiPlanet[lppl->id * 16 + 3] & 0x80) != 0x0 || lpplHome->id != lpflFr->idPlanet)
                        continue;
                    score = 25000;
                }
                if (lppl == lpplHome) {
                    if (pctFull < 35)
                        continue;
                    if (pctFull != 100) {
                        dx = pt.x - rgptPlan[lppl->id].x;
                        dy = pt.y - rgptPlan[lppl->id].y;
                        l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                        l = (int32_t)(((int32_t)sqrt((double)l) + 0x18) / 0x19);
                        if (l != 0) {
                            score = (int32_t)((int32_t)(20 * pctFull) / l);
                            goto LScore;
                        }
                        continue;
                    }
                    score = 25000;
                    goto LScore;
                }
                if (lppl->fStarbase != 0x0 ||
                    (lppl->lpplprod != 0x0 && lppl->lpplprod->rgprod[0].grobj == grobjFleet && lppl->lpplprod->rgprod[0].iItem >= iobjPacketGerm))
                    continue;
                if (vlpbAiPlanet[lppl->id * 16 + 2] != 0x0 && (vlpbAiPlanet[lppl->id * 16 + 2] & 0x80) == 0x0 && lpplHome->id == lpflFr->idPlanet) {
                    score = 25000;
                    goto LScore;
                }
                if (fNeedy != 2) {
                    wtPlanCargo = 0;
                    for (i = 0; i < 3; i++) {
                        if (fNeedy == 0 || i == iWorst) {
                            wtPlanCargo = wtPlanCargo + lppl->rgwtMin[i];
                        } else {
                            wtPlanCargo = wtPlanCargo + (int32_t)(lppl->rgwtMin[i] >> 0x1);
                        }
                    }
                } else {
                    wtPlanCargo = lppl->rgwtMin[iWorst];
                }
                if (wtPlanCargo < 10)
                    continue;
                pctHere = LOWORD((int32_t)((int32_t)(wtPlanCargo * 100) / wtCargoMax));
                if (pctHere > 100 - pctFull) {
                    pctHere = 100 - pctFull;
                }
            ScorePctHere:
                dx = pt.x - rgptPlan[lppl->id].x;
                dy = pt.y - rgptPlan[lppl->id].y;
                l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                l = (int32_t)(((int32_t)sqrt((double)l) + 0x18) / 0x19);
                if (l < 1) {
                    l = 1;
                }
                score = (int32_t)((int32_t)(100 * pctHere) / l);
            } else {
                dx = pt.x - rgptPlan[lppl->id].x;
                dy = pt.y - rgptPlan[lppl->id].y;
                l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                l = (int32_t)(((int32_t)sqrt((double)l) + 0x18) / 0x19);
                if (l < 1) {
                    l = 1;
                }
                score = (int32_t)((int32_t)((vlpbAiPlanet[lppl->id * 16 + 0x1] & 0x7f) * 0x1f4) / l);
            }
        LScore:
            if (score > scoreBest) {
                scoreBest = score;
                lpplBest = lppl;
                idBest = lppl->id;
            }
        }
    }
    lpthBest = 0x0;
    if (FSalvageTargetFreighter2(lpflFr, fNeedy, iWorst, pctFull, wtCargoMax, scoreBest, &lpthBest, &idBest) != 0 && GetCargoFree(lpflFr) == 0) {
        lpthBest = 0x0;
        lpplBest = lpplHome;
        idBest = lpplHome->id;
    }
    if (idBest != -1) {
        memset(&ord, 0, sizeof(ORDER));
        if (lpthBest != 0x0) {
            ord.grobj = grobjThing;
            ord.pt = lpthBest->pt;
        } else {
            ord.grobj = grobjPlanet;
            ord.pt = rgptPlan[idBest];
        }
        ord.id = idBest;
        ord.grTask = grTaskXfer;
        ord.fValidTask = 0x1;
        ord.iWarp = 0x4;
        for (i = 0; i <= 2; i++) {
            if (lpthBest != 0x0 || idBest != lpplHome->id) {
                t_merge_33cf_0001 = 0x1;
            } else {
                t_merge_33cf_0001 = 0x2;
            }
            ord.txp.rgia[i].iAction = t_merge_33cf_0001;
        }
        if (rgplr[idPlayer].idAi != 0x1) {
            if (rgplr[idPlayer].idAi == 0x0 && lpplHome->id == lpflFr->idPlanet && lpthBest == 0x0 && lpplBest->iPlayer != -1) {
                if (lpplBest->iPlayer == idPlayer) {
                    if (lpplBest->rgwtMin[3] < 1000 && lpplHome->rgwtMin[3] > (int32_t)((int32_t)(lpplBest->rgwtMin[3] * 3) / 2) &&
                        lpplHome->rgwtMin[3] > 500) {
                        if (lpplHome->rgwtMin[3] <= 2000) {
                            if (lpplHome->rgwtMin[3] <= 1500) {
                                if (lpplHome->rgwtMin[3] <= 1000) {
                                    l = (int32_t)((lpplHome->rgwtMin[3] - 400) / 0x64);
                                } else {
                                    l = 5;
                                }
                            } else {
                                l = 8;
                            }
                        } else {
                            l = 10;
                        }
                        l = (int32_t)((int32_t)(lpplHome->rgwtMin[3] * l) / 100);
                        ChangeMainObjSel(grobjFleet, lpflFr->id);
                        XferAiSupply(grobjPlanet, lpflFr->idPlanet, grobjFleet, lpflFr->id, 3, LOWORD(l));
                        FLookupFleet(lpflFr->id, &sel.fl);
                        ord.txp.rgia[3].iAction = iActionUnloadAll;
                    }
                } else {
                    ChangeMainObjSel(grobjFleet, lpflFr->id);
                    if (lpplHome->rgwtMin[3] <= 900) {
                        XferAiSupply(grobjPlanet, lpflFr->idPlanet, grobjFleet, lpflFr->id, 3, 100);
                    } else {
                        XferAiSupply(grobjPlanet, lpflFr->idPlanet, grobjFleet, lpflFr->id, 3, 300);
                    }
                    FLookupFleet(lpflFr->id, &sel.fl);
                    ord.txp.rgia[3].iAction = iActionUnloadAll;
                }
            }
        } else {
            if (lpplHome->id == lpflFr->idPlanet) {
                ChangeMainObjSel(grobjFleet, lpflFr->id);
                if (lpplHome->rgwtMin[3] > 1200 && lpthBest == 0x0 && lpplBest->iPlayer != -1 && lpplBest->rgwtMin[3] < lpplHome->rgwtMin[3]) {
                    XferAiSupply(grobjPlanet, lpflFr->idPlanet, grobjFleet, lpflFr->id, 3, 1000);
                }
                FLookupFleet(lpflFr->id, &sel.fl);
            }
            if (lpthBest == 0x0 && lpplBest->iPlayer != -1 && lpplBest != lpplHome) {
                ord.txp.rgia[3].iAction = iActionUnloadAll;
            }
        }
        if (lpthBest != 0x0) {
            ord.grTask = grTaskNone;
            ord.fValidTask = 0x0;
        } else if (lpplHome->id != idBest && fNeedy != 0) {
            if (fNeedy != 2 && lpplBest->rgwtMin[iWorst] < wtCargoFree) {
                if (lpthBest != 0x0 || lpplBest->iPlayer != -1) {
                    for (i = 0; i <= 2; i++) {
                        ord.txp.rgia[i].iAction = iActionFillPercent;
                        if (i != iWorst) {
                            l = 33;
                        } else {
                            l = 66;
                        }
                        ord.txp.rgia[i].cQuan = LOWORD(l);
                    }
                }
            } else {
                for (i = 0; i <= 2; i++) {
                    if (i != iWorst) {
                        ord.txp.rgia[i].iAction = iActionNone;
                    }
                }
            }
        }
        if (FMoveAiFleet(lpflFr, &ord, 0) != 0) {
            return idBest;
        }
        return -1;
    }
    return -1;
}

int16_t FSalvageTargetFreighter2(FLEET *lpflFr, int16_t fNeedy, int16_t iWorst, int16_t pctFull, int32_t wtCargoMax, int32_t scoreBest, THING **plpthBest,
                                 int16_t *pidBest) {
    POINT16 pt;
    int32_t score;
    int16_t dy;
    int16_t i;
    int32_t wtPlanCargo;
    THING  *lpth;
    int16_t pctHere;
    int16_t dx;
    THING  *lpthMac;
    int16_t fSalvage;
    int32_t l;

    fSalvage = 0;
    pt = lpflFr->pt;
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (lpth->ith == ithMineralPacket && lpth->thp.iWarp == 0x0) {
            dx = pt.x - lpth->pt.x;
            dy = pt.y - lpth->pt.y;
            if (dx != 0 || dy != 0) {
                if ((dx <= dy ? dy : dx) <= 200) {
                    if (fNeedy != 2) {
                        wtPlanCargo = 0;
                        for (i = 0; i < 3; i++) {
                            if (fNeedy == 0 || i == iWorst) {
                                wtPlanCargo = wtPlanCargo + (int32_t)lpth->thp.rgwtMin[i];
                            } else {
                                wtPlanCargo = wtPlanCargo + (int32_t)(lpth->thp.rgwtMin[i] >> 0x1);
                            }
                        }
                    } else {
                        wtPlanCargo = (int32_t)lpth->thp.rgwtMin[iWorst];
                    }
                    if (wtPlanCargo >= 10) {
                        pctHere = LOWORD((int32_t)((int32_t)(wtPlanCargo * 100) / wtCargoMax));
                        if (pctHere > 100 - pctFull) {
                            pctHere = 100 - pctFull;
                        }
                        l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                        l = (int32_t)(((int32_t)sqrt((double)l) + 0x18) / 0x19);
                        if (l < 1) {
                            l = 1;
                        }
                        score = (int32_t)((int32_t)(100 * pctHere) / l);
                        if (score > scoreBest) {
                            scoreBest = score;
                            *plpthBest = lpth;
                            *pidBest = lpth->idFull;
                        }
                    }
                }
            } else {
                if (fNeedy != 0 && lpth->thp.rgwtMin[iWorst] != 0) {
                    XferAiSupply(grobjThing, lpth->idFull, grobjFleet, lpflFr->id, iWorst, lpth->thp.rgwtMin[iWorst]);
                }
                if (fNeedy != 2) {
                    for (i = 0; i < 3; i++) {
                        if (lpth->thp.rgwtMin[i] != 0) {
                            XferAiSupply(grobjThing, lpth->idFull, grobjFleet, lpflFr->id, i, lpth->thp.rgwtMin[i]);
                        }
                    }
                }
                fSalvage = 1;
                FLookupFleet(lpflFr->id, &sel.fl);
            }
        }
    }
    return fSalvage;
}

int16_t FMoveAiFleet(FLEET *lpfl, ORDER *pord, int16_t fAppend) {
    int16_t iord;
    int16_t t_call_3e3f;

    ChangeMainObjSel(grobjFleet, lpfl->id);
    if (sel.fl.lpplord->iordMax == sel.fl.cord) {
        sel.fl.lpplord = (PLORD *)LpplReAlloc((PL *)sel.fl.lpplord, sel.fl.cord + 1);
    }
    iord = fAppend == 0 ? 0 : sel.fl.cord - 1;
    if (pord->pt.x == sel.fl.lpplord->rgord[iord].pt.x && pord->pt.y == sel.fl.lpplord->rgord[iord].pt.y) {
        if (sel.fl.cord > 1) {
            sel.fl.cord = 1;
            FLookupFleet(-1, &sel.fl);
        }
    } else {
        iord = iord + 1;
    }
    sel.fl.lpplord->rgord[iord] = *pord;
    sel.fl.cord = iord + 1;
    sel.fl.lpplord->iordMac = LOBYTE(iord + 1);
    t_call_3e3f = FLookupFleet(-1, &sel.fl);
    return t_call_3e3f;
}

void AddItemToQueue(uint16_t iItem, uint16_t cItem, GrobjClass grobj, int16_t mdAddItem) {
    int16_t  fSingle;
    int16_t  iprod;
    int16_t  i;
    PROD     rgprod[64];
    uint16_t t_scratch_m10a;

    fSingle = 0;
    if (sel.pl.lpplprod == 0x0 || sel.pl.lpplprod->iprodMac <= 0xc8) {
        if (lpplProdGlob == 0x0) {
            fSingle = 1;
            InitProduction(rgprod);
            for (i = 0; i < cProdGlob && (rgprod[i].iItem != (uint32_t)iItem || rgprod[i].grobj != (uint32_t)grobj); i++) {
            }
            if (i < cProdGlob) {
                cItem = cItem >= rgprod[i].cItem ? rgprod[i].cItem : cItem;
            } else {
                cItem = 0x0;
            }
        }
        if (cItem > 0x0) {
            if (mdAddItem != 2) {
                t_scratch_m10a = lpplProdGlob->iprodMac;
                if (t_scratch_m10a == lpplProdGlob->iprodMax) {
                    lpplProdGlob = (PLPROD *)LpplReAlloc((PL *)lpplProdGlob, lpplProdGlob->iprodMac + 0x3);
                }
            }
            switch (mdAddItem) {
            case 2:
                lpplProdGlob->iprodMac = 0x0;
            case 1:
                iprod = lpplProdGlob->iprodMac;
                break;
            case 0:
                iprod = 0;
                fmemmove(&lpplProdGlob->rgprod[1], lpplProdGlob->rgprod, lpplProdGlob->iprodMac * sizeof(PROD));
                fmemset(lpplProdGlob->rgprod, 0, 0x4);
            default:
            }
            lpplProdGlob->rgprod[iprod].cItem = cItem;
            lpplProdGlob->rgprod[iprod].iItem = iItem;
            lpplProdGlob->rgprod[iprod].grobj = grobj;
            lpplProdGlob->rgprod[iprod].pct = 0x0;
            lpplProdGlob->rgprod[iprod].unused = 0x0;
            lpplProdGlob->iprodMac = lpplProdGlob->iprodMac + 0x1;
        }
        if (fSingle != 0) {
            FinishProduction(cItem == 0x0 ? 0 : 1);
        }
    }
    return;
}

int16_t IroEnsureAi(uint8_t *lpbRes, int16_t cRes, int16_t *pishdefSBLatest, int16_t pct) {
    int16_t iSmallest;
    int16_t i;
    int16_t pctTech;
    int16_t ilvl;

    if (pishdefSBLatest != 0x0) {
        EnsureAiStarbaseDesigns();
        *pishdefSBLatest = IshdefAiSBLatest();
        ValidateStarbaseHistory();
    }
    rgplr[idPlayer].pctResearch = LOBYTE(pct);
    for (i = 0; i < 6 && (int16_t)rgplr[idPlayer].rgTech[i] >= 24; i++) {
    }
    if (i == 6) {
        rgplr[idPlayer].pctResearch = 0;
        pctTech = (int16_t)rgplr[idPlayer].iTechCur * 0;
        WriteMemRt(34, 2, &pctTech);
    }
    for (i = 0; i < cRes; i++) {
        ilvl = (int16_t)rgplr[idPlayer].rgTech[lpbRes[i] >> 0x5];
        if (ilvl < (lpbRes[i] & 0x1f))
            goto L_4372;
    }
    iSmallest = 0;
    for (i = 1; i < 6; i++) {
        if ((int16_t)rgplr[idPlayer].rgTech[i] < (int16_t)rgplr[idPlayer].rgTech[iSmallest]) {
            iSmallest = i;
        }
    }
    if (((int16_t)rgplr[idPlayer].iTechCur & 0xf) != iSmallest) {
        rgplr[idPlayer].iTechCur = LOBYTE(((int16_t)rgplr[idPlayer].iTechCur & 0xfff0) | iSmallest);
        if (((int16_t)rgplr[idPlayer].iTechCur & 0xf) == 0x1a) {
            rgplr[idPlayer].pctResearch = 0;
        }
        pctTech = (int16_t)rgplr[idPlayer].pctResearch + (int16_t)rgplr[idPlayer].iTechCur * 0;
        WriteMemRt(34, 2, &pctTech);
    }
    return 926;
L_4372:
    rgplr[idPlayer].iTechCur = LOBYTE(((int16_t)rgplr[idPlayer].iTechCur & 0xfff0) | lpbRes[i] >> 0x5);
    if (i < cRes - 1 && ilvl + 1 == (lpbRes[i] & 0x1f)) {
        rgplr[idPlayer].iTechCur = LOBYTE(((int16_t)rgplr[idPlayer].iTechCur & 0xff0f) | (lpbRes[i + 0x1] >> 0x5) * 0x10);
    }
    pctTech = (int16_t)rgplr[idPlayer].pctResearch + (int16_t)rgplr[idPlayer].iTechCur * 0;
    WriteMemRt(34, 2, &pctTech);
    return i;
}

void KeepFleetsMoving() {
    int16_t i;
    int16_t ifl;
    FLEET  *lpfl;
    THING  *lpth;
    THING  *rglpth[100];
    int16_t ith;
    THING  *lpthMac;
    int16_t t_465e;

    ith = 0;
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    while (1) {
        if (lpth >= lpthMac)
            goto LKeepMovn;
        if (lpth->ith == ithMinefield && lpth->iplr != idPlayer && lpth->thm.iType != 0x2) {
            if (ith == 100)
                break;
            t_465e = ith;
            ith = ith + 1;
            rglpth[t_465e] = lpth;
        }
        lpth = lpth + 1;
    }
    ith = -1;
LKeepMovn:
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->iPlayer == idPlayer && lpfl->cord > 1) {
            if (rgplr[idPlayer].idAi == 0x0 || game.turn >= 0x5) {
                i = -1;
                if (FIsAiTransport(lpfl) != 0) {
                    i = 30;
                }
            } else {
                i = 16;
            }
            SetAiFleetIdealSpeed(lpfl, i, ith, rglpth);
        }
    }
    return;
}

int16_t FShouldWeBuildColonizers(int16_t *pcCol) {
    int16_t  iMax;
    int16_t  cColFl;
    uint32_t cBuilt;
    int16_t  iMin;
    int16_t  ifl;
    int16_t  i;
    FLEET   *lpfl;
    int16_t  rgish[16];
    int16_t  cColPl;

    cColFl = 0;
    cColPl = 0;
    cBuilt = 0x0;
    iMax = -1;
    iMin = 16;
    if (pcCol != 0x0) {
        *pcCol = 0;
    }
    if (rgplr[idPlayer].lvlAi != 0x0 || (game.turn & 0x1) == 0x0) {
        if (game.turn >= 0x1e) {
            for (i = 0; i < 16; i++) {
                if (rgshdef[i].fFree != 0x0 || (rgshdef[i].hul.ihuldef != ihuldefMiniColonyShip && rgshdef[i].hul.ihuldef != ihuldefColonyShip)) {
                    rgish[i] = 0;
                } else {
                    rgish[i] = 1;
                    if (i > iMax) {
                        iMax = i;
                    }
                    if (i < iMin) {
                        iMin = i;
                    }
                }
            }
            iMax = iMax + 1;
            if (iMax > 0) {
                for (ifl = 0; ifl < cFleet; ifl++) {
                    lpfl = rglpfl[ifl];
                    if (rglpfl[ifl] == 0x0)
                        break;
                    if (lpfl->iPlayer == idPlayer) {
                        i = iMin;
                        while (1) {
                            if (i >= iMax)
                                goto L_4897;
                            if (rgish[i] > 0 && lpfl->rgcsh[i] > 0)
                                break;
                            i = i + 1;
                        }
                        cColFl = cColFl + 1;
                    }
                L_4897:;
                }
                if (pcCol != 0x0) {
                    *pcCol = cColFl;
                }
                if (cColFl <= 20 * game.mdSize + 0xa) {
                    for (i = 0; i < game.cPlayer; i++) {
                        if (rgplr[i].fInclude != 0x0) {
                            cColPl = cColPl + rgplr[i].cPlanet;
                        }
                    }
                    if (cColFl + cColPl <= (int32_t)(game.cPlanMax * 4) / 5) {
                        cBuilt = 0x0;
                        for (i = iMin; i < iMax; i++) {
                            if (rgish[i] > 0) {
                                cBuilt = cBuilt + rgshdef[i].cBuilt;
                            }
                        }
                        if (cBuilt - (int32_t)(cColPl + cColFl) <= 0x19 || (cColFl <= 4 && Random(2) == 0)) {
                            return 1;
                        }
                        return 0;
                    }
                    return 0;
                }
                return 0;
            }
            return 0;
        }
        return 1;
    }
    return 0;
}

int16_t FIsAiAttack(FLEET *lpfl) {
    int16_t ihul;
    int16_t i;

    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            ihul = rgshdef[i].hul.ihuldef;
            if (ihul > 5 && ihul <= 10) {
                return 1;
            }
            switch (ihul) {
            case 5:
                goto L_4ad0;
            case 31:
            case 29:
                if (WtMaxShdefStat(&rgshdef[i], 2) < 500 && rglpshdef[idPlayer][i].lPower > 0) {
                    return 1;
                }
            default:
            }
        }
    }
    return 0;
L_4ad0:
    if (rglpshdef[idPlayer][i].lPower <= 0) {
        return 0;
    }
    return 1;
}

int16_t FIsTurinDroneAiAttack(FLEET *lpfl) {
    int16_t ihul;
    int16_t i;

    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            ihul = rgshdef[i].hul.ihuldef;
            if (ihul >= 4 && ihul <= 10) {
                return 1;
            }
        }
    }
    return 0;
}

int16_t FIsAiTransport(FLEET *lpfl) {
    int16_t ihul;
    int16_t i;

    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            ihul = rgshdef[i].hul.ihuldef;
            if ((ihul >= 0 && ihul <= 3) || (ihul >= 11 && ihul <= 13)) {
                return 1;
            }
            if (ihul == 31 && WtMaxShdefStat(&rgshdef[i], 2) >= 500 && rglpshdef[idPlayer][i].lPower > 0) {
                return 1;
            }
        }
    }
    return 0;
}

void ValidateStarbaseHistory() {
    int16_t  iWrite;
    int16_t  iBest;
    PLANET  *lpplMac;
    POINT16  pt;
    int16_t  id;
    int16_t  dy;
    int16_t  cFr2;
    PLANET  *lppl;
    int16_t  ifl;
    FLEET   *lpfl;
    int16_t  i;
    int16_t  j;
    int16_t  ipl;
    int16_t  cFr;
    int16_t  dx;
    int32_t  lBest;
    int32_t  l;
    uint16_t t_scratch_m32_4;
    uint16_t t_5568;
    uint16_t t_scratch_m32_5;
    uint16_t t_569b;

    if (rgplr[idPlayer].idAi != 0x4 && rgplr[idPlayer].idAi != 0x5) {
        if (RawLoad16(vlpbAiData) <= 0x2) {
            RawStore16((uint8_t *)vlpbAiData + 0x2, 0x0);
        }
        if (game.turn >= 0x14) {
            iWrite = 0;
            if (RawLoad16((uint8_t *)vlpbAiData + 0x2) < 0x0 || RawLoad16((uint8_t *)vlpbAiData + 0x2) > 0x40) {
                RawStore16((uint8_t *)vlpbAiData + 0x2, 0x0);
            }
            for (i = 0; i < RawLoad16((uint8_t *)vlpbAiData + 0x2); i++) {
                if (RawLoad16(vlpbAiData + (i * 20 + 6)) < 0x0 || RawLoad16(vlpbAiData + (i * 20 + 6)) > 0x8) {
                    RawStore16(vlpbAiData + (i * 20 + 6), 0x0);
                }
                lppl = LpplFromId(RawLoad16(vlpbAiData + (i * 20 + 4)));
                if (lppl != 0x0 && lppl->iPlayer == idPlayer) {
                    if (iWrite != i) {
                        vlpbAiData[iWrite * 20 + 4] = vlpbAiData[i * 20 + 4];
                    }
                    iWrite = iWrite + 1;
                }
            }
            RawStore16((uint8_t *)vlpbAiData + 0x2, iWrite);
            for (ipl = 0; ipl < vclpplAi; ipl++) {
                lppl = vrglpplAi[ipl];
                if (vrglpplAi[ipl] == 0x0)
                    break;
                if (lppl->fStarbase != 0x0) {
                    for (i = 0; i < RawLoad16((uint8_t *)vlpbAiData + 0x2) && RawLoad16(vlpbAiData + (i * 20 + 4)) != lppl->id; i++) {
                    }
                    if (i == RawLoad16((uint8_t *)vlpbAiData + 0x2) && i < 64) {
                        RawStore16(vlpbAiData + (i * 20 + 4), lppl->id);
                        RawStore16(vlpbAiData + (i * 20 + 6), 0x0);
                        RawStore16((uint8_t *)vlpbAiData + 0x2, RawLoad16((uint8_t *)vlpbAiData + 0x2) + 0x1);
                    }
                }
            }
            lppl = lpPlanets;
            lpplMac = lpPlanets + cPlanet;
            for (; lppl < lpplMac && RawLoad16((uint8_t *)vlpbAiData + 0x2) < 0x40; lppl++) {
                if (lppl->iPlayer == idPlayer && lppl->fStarbase == 0x0 && lppl->rgwtMin[3] >= 80) {
                    l = 0;
                    for (i = 0; i < 3; i++) {
                        l = l + ((int32_t)((uint32_t)((uint32_t)lppl->rgMinConc[i] * (uint32_t)lppl->rgMinConc[i]) * 0x4) + lppl->rgwtMin[i]);
                    }
                    if (l >= 7000 && lppl->cMines >= 0x14 && lppl->cFactories >= 0x14) {
                        for (i = 0; i < RawLoad16((uint8_t *)vlpbAiData + 0x2); i++) {
                            id = RawLoad16(vlpbAiData + (i * 20 + 4));
                            if (id == lppl->id)
                                break;
                            if (rgplr[idPlayer].idAi == 0x0) {
                                pt = rgptPlan[id];
                                dx = pt.x - rgptPlan[lppl->id].x;
                                dy = pt.y - rgptPlan[lppl->id].y;
                                l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                                if (l < 2500)
                                    break;
                            }
                        }
                        if (i >= RawLoad16((uint8_t *)vlpbAiData + 0x2)) {
                            RawStore16(vlpbAiData + (RawLoad16((uint8_t *)vlpbAiData + 0x2) * 20 + 4), lppl->id);
                            RawStore16(vlpbAiData + (RawLoad16((uint8_t *)vlpbAiData + 0x2) * 20 + 6), 0x0);
                            RawStore16((uint8_t *)vlpbAiData + 0x2, RawLoad16((uint8_t *)vlpbAiData + 0x2) + 0x1);
                        }
                    }
                }
            }
            for (ifl = 0; ifl < cFleet; ifl++) {
                lpfl = rglpfl[ifl];
                if (rglpfl[ifl] == 0x0)
                    break;
                if (lpfl->iPlayer == idPlayer && FIsAiTransport(lpfl) != 0) {
                    for (i = 0; i < RawLoad16((uint8_t *)vlpbAiData + 0x2); i++) {
                        for (j = 0; j < RawLoad16(vlpbAiData + (i * 20 + 6)) && RawLoad16(vlpbAiData + (i * 20 + j * 2 + 8)) != lpfl->id; j++) {
                        }
                        if (j < RawLoad16(vlpbAiData + (i * 20 + 6)))
                            break;
                    }
                    if (i == RawLoad16((uint8_t *)vlpbAiData + 0x2)) {
                        iBest = -1;
                        lBest = 10000000;
                        for (i = 0; i < RawLoad16((uint8_t *)vlpbAiData + 0x2); i++) {
                            if (RawLoad16(vlpbAiData + (i * 20 + 6)) < 0x8) {
                                pt = rgptPlan[RawLoad16(vlpbAiData + (i * 20 + 4))];
                                dx = pt.x - lpfl->pt.x;
                                dy = pt.y - lpfl->pt.y;
                                l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                                if (l < lBest) {
                                    lBest = l;
                                    iBest = i;
                                }
                            }
                        }
                        if (iBest != -1) {
                            t_scratch_m32_4 = lpfl->id;
                            t_5568 = RawLoad16(vlpbAiData + (iBest * 20 + 6));
                            RawStore16(vlpbAiData + (iBest * 20 + 6), RawLoad16(vlpbAiData + (iBest * 20 + 6)) + 0x1);
                            RawStore16(vlpbAiData + (iBest * 20 + t_5568 * 2 + 8), t_scratch_m32_4);
                        }
                    }
                }
            }
            for (i = 0; i < RawLoad16((uint8_t *)vlpbAiData + 0x2); i++) {
                cFr = RawLoad16(vlpbAiData + (i * 20 + 6));
                if (cFr < 4) {
                    for (j = 0; j < RawLoad16((uint8_t *)vlpbAiData + 0x2); j++) {
                        cFr2 = RawLoad16(vlpbAiData + (j * 20 + 6));
                        if (cFr2 >= cFr + 2)
                            break;
                    }
                    if (j < RawLoad16((uint8_t *)vlpbAiData + 0x2)) {
                        RawStore16(vlpbAiData + (j * 20 + 6), RawLoad16(vlpbAiData + (j * 20 + 6)) - 0x1);
                        t_scratch_m32_5 = RawLoad16(vlpbAiData + (j * 20 + RawLoad16(vlpbAiData + (j * 20 + 6)) * 2 + 8));
                        t_569b = RawLoad16(vlpbAiData + (i * 20 + 6));
                        RawStore16(vlpbAiData + (i * 20 + 6), RawLoad16(vlpbAiData + (i * 20 + 6)) + 0x1);
                        RawStore16(vlpbAiData + (i * 20 + t_569b * 2 + 8), t_scratch_m32_5);
                    }
                }
            }
            RawStore16(vlpbAiData, RawLoad16((uint8_t *)vlpbAiData + 0x2) * 0x14 + 0x4);
        }
    }
    return;
}

void GetResourcesAvailable(PLANET *lppl, int32_t *rgRes) {
    int16_t i;
    int32_t cRes;

    EstMineralsMined(lppl, rgRes, -1, 0);
    for (i = 0; i < 3; i++) {
        rgRes[i] = rgRes[i] + lppl->rgwtMin[i];
    }
    cRes = (int32_t)CResourcesAtPlanet(lppl, idPlayer);
    if (sel.pl.fNoResearch == 0x0) {
        cRes = cRes - (int32_t)((int32_t)(cRes * (int32_t)(int16_t)rgplr[idPlayer].pctResearch) / 0x64);
    }
    rgRes[3] = cRes;
    return;
}

void GetProdQCost(PLANET *lppl, int32_t *rgCost) {
    int32_t rgCostCur[4];
    PLPROD *lpplprod;
    int16_t i;
    int16_t j;
    PROD   *lpprod;

    for (i = 0; i < 4; i++) {
        rgCost[i] = 0;
    }
    if (lppl->lpplprod != 0x0) {
        lpplprod = lppl->lpplprod;
        i = 0;
        lpprod = lpplprod->rgprod;
        while (i < lpplprod->iprodMac) {
            GetProductionCosts(lppl, lpprod, rgCostCur, idPlayer, 0);
            for (j = 0; j < 4; j++) {
                rgCost[j] = rgCost[j] + rgCostCur[j];
            }
            i = i + 1;
            lpprod = lpprod + 1;
        }
    }
    return;
}

void MergeAllShdefs(int16_t grbitish) {
    int16_t crglpflW;
    FLEET  *rglpflW[32];
    int16_t iMax;
    int16_t iMin;
    int16_t i;
    int16_t ifl;
    FLEET  *lpfl;
    int16_t grbit;
    int16_t rgish[16];
    FLEET  *lpflNextPass;
    int16_t iflNextPass;
    int16_t t_5b46;

    iMax = -1;
    iMin = 16;
    i = 0;
    grbit = 1;
    while (i < 16) {
        if ((grbitish & grbit) == 0x0) {
            rgish[i] = 0;
        } else {
            rgish[i] = 1;
            if (i > iMax) {
                iMax = i;
            }
            if (i < iMin) {
                iMin = i;
            }
        }
        i = i + 1;
        grbit = grbit * 2;
    }
    iMax = iMax + 1;
    if (iMax > 0) {
        lpflNextPass = 0x0;
        crglpflW = 0;
        ifl = 0;
        while (1) {
            if (ifl < cFleet) {
                lpfl = rglpfl[ifl];
                if (rglpfl[ifl] != 0x0)
                    goto NextPass;
            }
            if (lpflNextPass == 0x0)
                break;
            lpfl = lpflNextPass;
            lpflNextPass = 0x0;
            ifl = iflNextPass;
            crglpflW = 0;
        NextPass:
            if (lpfl->iPlayer == idPlayer) {
                for (i = iMin; i < iMax && (lpfl->rgcsh[i] <= 0 || rgish[i] <= 0); i++) {
                }
                if (i != iMax && CMineFromLpfl(lpfl) != 4000) {
                    for (i = 0; i < crglpflW && (rglpflW[i]->idPlanet != lpfl->idPlanet || rglpflW[i]->pt.x != lpfl->pt.x || rglpflW[i]->pt.y != lpfl->pt.y);
                         i++) {
                    }
                    if (i >= crglpflW) {
                        if (crglpflW != 32) {
                            t_5b46 = crglpflW;
                            crglpflW = crglpflW + 1;
                            rglpflW[t_5b46] = lpfl;
                        } else if (lpflNextPass == 0x0) {
                            lpflNextPass = lpfl;
                            iflNextPass = ifl;
                        }
                    } else {
                        Merge2Fleets(rglpflW[i], lpfl, 0);
                    }
                }
            }
            ifl = ifl + 1;
        }
    }
    return;
}

FLEET *LpflFindClosestEnum(FLEET *lpfl, int16_t (*pfn)(FLEET *, FLEET *)) {
    FLEET  *lpflT;
    POINT16 pt;
    int16_t dy;
    int16_t ish;
    int16_t dx;
    FLEET  *lpflBest;
    int32_t l;
    int32_t lBest;

    lBest = 10000000;
    lpflBest = 0x0;
    pt = lpfl->pt;
    for (ish = 0; ish < cFleet; ish++) {
        lpflT = rglpfl[ish];
        if (rglpfl[ish] == 0x0)
            break;
        dx = pt.x - lpflT->pt.x;
        dy = pt.y - lpflT->pt.y;
        l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
        if (l < lBest && pfn(lpfl, lpflT) != 0) {
            lpflBest = lpflT;
            lBest = l;
        }
    }
    return lpflBest;
}

PLANET *LpplFindClosestEnum(PLANET *lppl, int16_t (*pfn)(PLANET *, PLANET *)) {
    POINT16 pt;
    int16_t dy;
    PLANET *lpplTMac;
    PLANET *lpplBest;
    PLANET *lpplT;
    int16_t dx;
    int32_t l;
    int32_t lBest;

    lBest = 10000000;
    lpplBest = 0x0;
    pt = rgptPlan[lppl->id];
    lpplT = lpPlanets;
    lpplTMac = lpPlanets + cPlanet;
    for (; lpplT < lpplTMac; lpplT++) {
        dx = pt.x - rgptPlan[lpplT->id].x;
        dy = pt.y - rgptPlan[lpplT->id].y;
        l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
        if (l < lBest && pfn(lppl, lpplT) != 0) {
            lpplBest = lpplT;
            lBest = l;
        }
    }
    return lpplBest;
}

PLANET *LpplFindBestEnum(PLANET *lppl, int16_t (*pfn)(PLANET *, PLANET *)) {
    int16_t iBest;
    POINT16 pt;
    int16_t dy;
    PLANET *lpplTMac;
    PLANET *lpplBest;
    PLANET *lpplT;
    int16_t iCur;
    int16_t dx;
    int32_t l;
    int32_t lBest;

    iBest = 1;
    lBest = 10000000;
    lpplBest = 0x0;
    pt = rgptPlan[lppl->id];
    lpplT = lpPlanets;
    lpplTMac = lpPlanets + cPlanet;
    for (; lpplT < lpplTMac; lpplT++) {
        dx = pt.x - rgptPlan[lpplT->id].x;
        dy = pt.y - rgptPlan[lpplT->id].y;
        l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
        iCur = pfn(lppl, lpplT);
        if (iCur > iBest || (iCur == iBest && l < lBest)) {
            iBest = iCur;
            lBest = l;
            lpplBest = lpplT;
        }
    }
    return lpplBest;
}

int16_t IdRandomPlanetNearby(POINT16 pt, int16_t cDist, int16_t fAvoidStarbases) {
    int16_t iChance;
    int32_t lDistMax;
    int32_t dy;
    int16_t idBest;
    int16_t i;
    int32_t dx;
    int16_t cExtraAttempts;
    int32_t d2Cur;
    PLANET *lppl;
    int16_t t_603d;
    int16_t t_6067;

    cExtraAttempts = fAvoidStarbases == 0 ? 0 : 2;
    do {
        lDistMax = (uint32_t)((int32_t)cDist * (int32_t)cDist);
        iChance = 1;
        idBest = -1;
        for (i = 0; i < game.cPlanMax; i++) {
            dx = (int32_t)abs(pt.x - rgptPlan[i].x);
            dy = (int32_t)abs(pt.y - rgptPlan[i].y);
            d2Cur = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
            if (d2Cur <= lDistMax) {
                t_603d = iChance;
                iChance = iChance + 1;
                if (Random(iChance) == 0) {
                    idBest = i;
                }
            }
        }
        if (idBest == -1)
            break;
        t_6067 = cExtraAttempts;
        cExtraAttempts = cExtraAttempts - 1;
        if (t_6067 <= 0)
            break;
        lppl = LpplFromId(idBest);
    } while (lppl != 0x0 && lppl->fStarbase != 0x0);
    return idBest;
}

void ClearAiCurrentTask(FLEET *lpfl, int16_t fChangeSel) {
    if (fChangeSel != 0) {
        ChangeMainObjSel(grobjFleet, lpfl->id);
    }
    sel.fl.lpplord->rgord[0].grTask = grTaskNone;
    FLookupFleet(-1, &sel.fl);
    return;
}

int16_t FEnumOurStarbase(PLANET *lpplSrc, PLANET *lpplTest) {
    if (lpplTest->iPlayer != idPlayer || lpplTest->fStarbase == 0x0) {
        return 0;
    }
    return 1;
}

int16_t FFleetMightHaveTeeth(FLEET *lpfl) {
    HUL    *lphul;
    int16_t ishdef;

    for (ishdef = 0; ishdef < 16; ishdef++) {
        if (lpfl->rgcsh[ishdef] != 0) {
            lphul = &rglpshdef[lpfl->iplr][ishdef].hul;
            if (FHullHasTeeth(lphul) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

int16_t IdTargetScout(FLEET *lpfl, FLEET *lpflAtk, FLEET *lpflEnemy, int16_t fOnlyHumans, THING **plpthWorm) {
    FLEET  *lpflClosest;
    FLEET  *lpflT;
    int32_t lDistBest;
    PLANET *lpplMac;
    POINT16 pt;
    int16_t dy;
    PLANET *lppl;
    int32_t lDist;
    int16_t idClosest;
    PLANET *lpplClosest;
    int16_t dx;
    FLEET  *lpflAtk2;
    ORDER   ord;

    lpflClosest = 0x0;
    lpflT = lpflEnemy;
    lDistBest = 1000000;
    pt = lpfl->pt;
    memset(&ord, 0, sizeof(ORDER));
    if (FFleetMightHaveTeeth(lpfl) != 0) {
        for (; lpflT != 0x0; lpflT = lpflT->lpflNext) {
            if (fOnlyHumans == 0 || rgplr[lpflT->iPlayer].fAi == 0x0) {
                for (lpflAtk2 = lpflAtk; lpflAtk2 != 0x0 && (lpflAtk2 == lpfl || lpflAtk2->cord < 2 || lpflAtk2->lpplord->rgord[1].id != lpflT->id ||
                                                             lpflAtk2->lpplord->rgord[1].grobj != grobjFleet);
                     lpflAtk2 = lpflAtk2->lpflNext) {
                }
                if (lpflAtk2 == 0x0 || Random(3) != 0) {
                    dx = lpflT->pt.x - pt.x;
                    dy = lpflT->pt.y - pt.y;
                    lDist = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                    if (lDist < lDistBest) {
                        lDistBest = lDist;
                        lpflClosest = lpflT;
                    }
                }
            }
        }
        if (lDistBest < 32400) {
            ord.pt = lpflClosest->pt;
            ord.grobj = grobjFleet;
            ord.id = lpflClosest->id;
            goto ThwakSumthin;
        }
        if (lpflClosest != 0x0) {
            if (lpfl->rgwtMin[4] < (int32_t)(LGetFleetStat(lpfl, 1) / 2) && FMoveToNearestStarbase(lpfl, 0) != 0) {
                return 0;
            }
            pt = lpflClosest->pt;
            lpplClosest = 0x0;
            lDistBest = 1000000;
            lppl = lpPlanets;
            lpplMac = lpPlanets + cPlanet;
            for (; lppl < lpplMac; lppl++) {
                for (lpflAtk2 = lpflAtk; lpflAtk2 != 0x0 && (lpflAtk2 == lpfl || lpflAtk2->cord < 2 || lpflAtk2->lpplord->rgord[1].id != lppl->id ||
                                                             lpflAtk2->lpplord->rgord[1].grobj != grobjPlanet);
                     lpflAtk2 = lpflAtk2->lpflNext) {
                }
                if (lpflAtk2 == 0x0) {
                    dx = rgptPlan[lppl->id].x - pt.x;
                    dy = rgptPlan[lppl->id].y - pt.y;
                    lDist = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                    if (lDist < lDistBest) {
                        lDistBest = lDist;
                        lpplClosest = lppl;
                    }
                }
            }
            if (lpplClosest != 0x0 && lpplClosest->id != lpfl->idPlanet) {
                ord.pt = rgptPlan[lpplClosest->id];
                ord.grobj = grobjPlanet;
                ord.id = lpplClosest->id;
                goto ThwakSumthin;
            }
        } else if (fOnlyHumans != 0) {
            return IdTargetScout(lpfl, lpflAtk, lpflEnemy, 0, plpthWorm);
        }
    }
    idClosest = IdNearestUnknownPlanet(lpfl, plpthWorm);
    if (plpthWorm != 0x0 && *plpthWorm != 0x0) {
        ord.pt = (*plpthWorm)->pt;
        ord.grobj = grobjThing;
        ord.id = (*plpthWorm)->idFull;
    } else {
        if (idClosest == -1) {
            if (rgplr[idPlayer].idPlanetHome != -1) {
                lpplClosest = LpplFindBestEnum(LpplFromId(rgplr[idPlayer].idPlanetHome), FEnumCalcArmadaDest);
                if (lpplClosest != 0x0) {
                    idClosest = lpplClosest->id;
                } else {
                    idClosest = Random(game.cPlanMax);
                }
            } else {
                idClosest = Random(game.cPlanMax);
            }
        }
        ord.id = idClosest;
        ord.grobj = grobjPlanet;
        ord.pt = rgptPlan[idClosest];
    }
ThwakSumthin:
    if (lpfl->cord <= 1 || lpfl->lpplord->rgord[1].grobj != grobjPlanet || ord.grobj != grobjPlanet) {
        if (ord.grobj == grobjPlanet) {
            vlpbAiPlanet[ord.id * 16 + 15] = 0x4;
        }
        ord.grTask = grTaskNone;
        ord.fValidTask = 0x1;
        ord.iWarp = (uint16_t)IFindIdealWarp(lpfl, 1);
        if (FMoveAiFleet(lpfl, &ord, 0) != 0) {
            return 0;
        }
        return -1;
    }
    return -1;
}

void MarkPlanetsUnderAttack() {
    PLANET *lppl;
    int16_t i;
    int16_t ifl;
    FLEET  *lpfl;
    int16_t j;

    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->iPlayer != idPlayer && lpfl->idPlanet != -1) {
            j = lpfl->iPlayer;
            for (i = 0; i < 16 && (lpfl->rgcsh[i] <= 0 || rglpshdef[j][i].hul.ihuldef < ihuldefMiniBomber || rglpshdef[j][i].hul.ihuldef > ihuldefB52Bomber);
                 i++) {
            }
            if (i != 16) {
                lppl = LpplFromId(lpfl->idPlanet);
                if (lppl->iPlayer == idPlayer) {
                    vlpbAiPlanet[lppl->id * 16 + 8] = 0x1;
                }
            }
        }
    }
    return;
}

void FixPlanetsUnderAttack(PROD *rgprod) {
    PLANET *lppl;
    int16_t ipl;

    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0x0)
            break;
        if (vlpbAiPlanet[lppl->id * 16 + 8] != 0x0) {
            QuickBuildDefenses(lppl, rgprod);
        }
    }
    return;
}

void QuickBuildDefenses(PLANET *lppl, PROD *rgprod) {
    int16_t cMax;
    int16_t cAlch;
    int16_t cCur;
    int16_t i;
    int16_t cRes;
    int32_t lVal;
    int16_t cDef;
    int32_t rgRes[4];
    PROD   *lpprod;
    int16_t t_merge_6df5_0001;

    if (lppl->lpplprod != 0x0) {
        i = 0;
        lpprod = lppl->lpplprod->rgprod;
        while (i < lppl->lpplprod->iprodMac) {
            if (lpprod->grobj == grobjPlanet && lpprod->iItem == mdIdleDefense) {
                return;
            }
            i = i + 1;
            lpprod = lpprod + 1;
        }
    }
    GetResourcesAvailable(lppl, rgRes);
    if (rgRes[3] >= 50) {
        ChangeMainObjSel(grobjPlanet, lppl->id);
        InitProduction(rgprod);
        for (i = 0; i < cProdGlob && (pProdGlob[i].grobj != grobjPlanet || pProdGlob[i].iItem != mdIdleDefense || pProdGlob[i].cItem < 0x1); i++) {
        }
        if (i != cProdGlob) {
            cMax = pProdGlob[i].cItem;
            cCur = 100;
            for (i = 0; i < 3; i++) {
                lVal = (int32_t)(rgRes[i] / 5);
                if (lVal < (int32_t)cCur) {
                    cCur = LOWORD(lVal);
                }
            }
            cRes = LOWORD((int32_t)(rgRes[3] / 25));
            if (cRes > 5) {
                cRes = cRes - (int32_t)cRes / 6;
            }
            rgRes[3] = rgRes[3] - (int32_t)(rgRes[3] / 10);
            if (cRes > cMax) {
                cRes = cMax;
            }
            if (cRes <= cCur) {
                cAlch = 0;
                cDef = cRes;
            } else {
                cAlch = LOWORD((int32_t)((rgRes[3] - (int32_t)((GetRaceGrbit(&rgplr[idPlayer], ibitRaceMineralAlchemy) == 0 ? 0x64 : 0x19) * cCur)) / 0x96));
                if (cAlch < 0) {
                    cAlch = 0;
                }
                cDef = cCur + cAlch;
                cAlch = 5 * cAlch;
            }
            if (cDef > 0) {
                AddItemToQueue(0x9, cDef, grobjPlanet, 0);
            }
            if (cAlch > 0) {
                AddItemToQueue(0xb, cAlch, grobjPlanet, 0);
            }
            if (cDef <= 0 && cAlch <= 0) {
                t_merge_6df5_0001 = 0;
            } else {
                t_merge_6df5_0001 = 1;
            }
            FinishProduction(t_merge_6df5_0001);
        } else {
            FinishProduction(0);
        }
    }
    return;
}

int16_t IdplFindClosestStarbase(FLEET *lpfl, int16_t fBigOnes) {
    POINT16 pt;
    int16_t dy;
    PLANET *lpplTMac;
    PLANET *lpplBest;
    PLANET *lpplT;
    int16_t dx;
    int32_t l;
    int32_t lBest;

    lBest = 10000000;
    lpplBest = 0x0;
    pt = lpfl->lpplord->rgord[0].pt;
    lpplT = lpPlanets;
    lpplTMac = lpPlanets + cPlanet;
    for (; lpplT < lpplTMac; lpplT++) {
        dx = pt.x - rgptPlan[lpplT->id].x;
        dy = pt.y - rgptPlan[lpplT->id].y;
        l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
        if (l < lBest && lpplT->iPlayer == idPlayer && lpplT->fStarbase != 0x0 && (fBigOnes == 0 || lpplT->rgwtMin[3] > 250)) {
            lpplBest = lpplT;
            lBest = l;
        }
    }
    if (lpplBest != 0x0) {
        return lpplBest->id;
    }
    return -1;
}

int16_t FMoveToNearestStarbase(FLEET *lpfl, int16_t fBigOnes) {
    int16_t id;
    ORDER   ord;

    id = IdplFindClosestStarbase(lpfl, fBigOnes);
    if (id != -1) {
        ord.id = id;
        ord.grobj = grobjPlanet;
        ord.pt = rgptPlan[id];
        ord.grTask = grTaskNone;
        ord.fValidTask = 0x1;
        ord.iWarp = 0x4;
        return FMoveAiFleet(lpfl, &ord, 0);
    }
    return 0;
}

void MoveToNearestPlanetOrEnemy(FLEET *lpfl, int16_t dEnemyRange) {
    POINT16 pt;
    int16_t id;
    int16_t dy;
    PLANET *lpplTMac;
    PLANET *lpplBest;
    PLANET *lpplT;
    int16_t dx;
    ORDER   ord;
    int32_t l;
    int32_t lBest;
    SCAN    scan;

    lBest = 10000000;
    lpplBest = 0x0;
    pt = lpfl->lpplord->rgord[0].pt;
    lpplT = lpPlanets;
    lpplTMac = lpPlanets + cPlanet;
    for (; lpplT < lpplTMac; lpplT++) {
        dx = pt.x - rgptPlan[lpplT->id].x;
        dy = pt.y - rgptPlan[lpplT->id].y;
        l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
        if (l < lBest && lpplT->iPlayer != idPlayer && lpplT->iPlayer != -1) {
            lpplBest = lpplT;
            lBest = l;
        }
    }
    if (lpplBest != 0x0 && lBest <= (int32_t)(uint32_t)((int32_t)dEnemyRange * (int32_t)dEnemyRange)) {
        id = lpplBest->id;
    } else {
        if (FFindNearestObject(lpfl->pt, 0x21, &scan) == 0) {
            return;
        }
        id = scan.idpl;
    }
    if (lpfl->idPlanet != id) {
        ord.id = id;
        ord.grobj = grobjPlanet;
        ord.pt = rgptPlan[id];
        ord.grTask = grTaskNone;
        ord.fValidTask = 0x1;
        ord.iWarp = 0x4;
        FMoveAiFleet(lpfl, &ord, 0);
    }
    return;
}

void EnsureAiStarbaseDesigns() {
    uint16_t wTurnLast;
    int16_t  iSetNew;
    int16_t  i;
    int16_t  iSetLast;

    if (rglpshdefSB[idPlayer][2].fFree != 0x0) {
        FCreateAiStarbase(2, 2, -1, -1);
        FCreateAiStarbase(4, 3, -1, -1);
    }
    if (rglpshdefSB[idPlayer][1].fFree != 0x0) {
        FCreateAiStarbase(1, 1, -1, -1);
    }
    if (rglpshdefSB[idPlayer][3].fFree != 0x0) {
        FCreateAiStarbase(3, 2, -1, -1);
    }
    if (game.turn >= 0x32) {
        iSetLast = -1;
        wTurnLast = 0x0;
        for (i = 0; i <= 9; i = i + 2) {
            if (i == 6) {
                i = i - 1;
            }
            if (rglpshdefSB[idPlayer][i].fFree == 0x0 && rglpshdefSB[idPlayer][i].turn >= wTurnLast) {
                wTurnLast = rglpshdefSB[idPlayer][i].turn;
                iSetLast = i;
            }
        }
        if (iSetLast != -1 && wTurnLast + 0x28 <= game.turn) {
            iSetNew = iSetLast <= 4 ? 5 : 0;
            for (i = iSetNew; i < iSetNew + 5; i = i + 2) {
                if (rglpshdefSB[idPlayer][i].fFree == 0x0 && rglpshdefSB[idPlayer][i].cExist > 0x0)
                    goto LOrbital;
            }
            FCreateAiStarbase(iSetNew, 1, -1, -1);
            FCreateAiStarbase(iSetNew + 2, 2, -1, -1);
            FCreateAiStarbase(iSetNew + 4, 3, -1, -1);
        }
    LOrbital:
        iSetLast = -1;
        wTurnLast = 0x0;
        for (i = 1; i < 9; i = i + 2) {
            if (i == 5) {
                i = i + 1;
            }
            if (rglpshdefSB[idPlayer][i].fFree == 0x0 && rglpshdefSB[idPlayer][i].turn >= wTurnLast) {
                wTurnLast = rglpshdefSB[idPlayer][i].turn;
                iSetLast = i;
            }
        }
        if (iSetLast != -1 && wTurnLast + 0x28 <= game.turn) {
            iSetNew = iSetLast <= 4 ? 6 : 1;
            if ((rglpshdefSB[idPlayer][iSetNew].fFree != 0x0 || rglpshdefSB[idPlayer][iSetNew].cExist <= 0x0) &&
                (rglpshdefSB[idPlayer][iSetNew + 2].fFree != 0x0 || rglpshdefSB[idPlayer][iSetNew + 2].cExist <= 0x0)) {
                FCreateAiStarbase(iSetNew, 1, -1, -1);
                FCreateAiStarbase(iSetNew + 2, 2, -1, -1);
            }
        }
    }
    return;
}

void EnsureMacintiStarbaseDesigns(uint8_t *rgSB) {
    int16_t  k;
    int16_t  iOld;
    int16_t  cAge;
    int16_t  i;
    int16_t  j;
    int16_t  iNew;
    uint16_t t_scratch_m10;
    uint16_t t_scratch_m10_2;

    *rgSB = 0x0;
    for (i = 1; i <= 3; i++) {
        if (rglpshdefSB[idPlayer][i].fFree == 0x0 && rglpshdefSB[idPlayer][i].cExist != 0x0) {
            cAge = game.turn - rglpshdefSB[idPlayer][i].turn;
            if (cAge >= 35) {
                if (cAge >= 50) {
                    rgSB[i] = 0x3;
                } else {
                    rgSB[i] = 0x2;
                }
            } else if (game.turn <= 0x19 || i != 1 || rglpshdefSB[idPlayer][i].hul.chs == 0x8) {
                rgSB[i] = 0x0;
            } else {
                rgSB[i] = 0x3;
            }
        } else if (FCreateAiStarbase(i, i >= 3 ? 2 : 1, vrgSBMacAisb[i - 1], i) == 0) {
            rgSB[i] = 0x1;
        } else {
            rgSB[i] = 0x0;
        }
    }
    iOld = -1;
    for (i = 1; i <= 3; i++) {
        if (rgSB[i] >= 0x2) {
            if (iOld != -1) {
                t_scratch_m10 = rgSB[i];
                if (t_scratch_m10 <= rgSB[iOld]) {
                    t_scratch_m10_2 = rgSB[i];
                    if (t_scratch_m10_2 != rgSB[iOld] || rglpshdefSB[idPlayer][i].turn >= rglpshdefSB[idPlayer][iOld].turn)
                        continue;
                }
            }
            iOld = i;
        }
    }
    for (i = 1; i <= 3; i++) {
        if (rgSB[i] >= 0x2 && i != iOld) {
            rgSB[i] = 0x0;
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3 && (rglpshdefSB[idPlayer][3 * i + 4 + j].fFree != 0x0 || rglpshdefSB[idPlayer][3 * i + 4 + j].cExist <= 0x0); j++) {
        }
        if (j == 3) {
            for (j = 0; j < 3; j++) {
                for (k = 5; k >= 3 && FCreateAiStarbase(3 * i + 4 + j, j + 1, vrgSBMacAisb[k], k - 1) == 0; k--) {
                }
            }
        }
    }
    for (i = 4; i < 10; i++) {
        rgSB[i] = LOBYTE(rglpshdefSB[idPlayer][i].fFree == 0x0 ? 0x0 : 0x1);
    }
    if (rglpshdefSB[idPlayer][4].turn < rglpshdefSB[idPlayer][7].turn) {
        iNew = 7;
        iOld = 4;
    } else {
        iNew = 4;
        iOld = 7;
    }
    if (game.turn - rglpshdefSB[idPlayer][iOld].turn >= 0x1e) {
        j = 3;
    } else {
        j = 2;
    }
    for (i = iOld; i < iOld + 3; i++) {
        rgSB[i] = LOBYTE(j);
    }
    i = rglpshdefSB[idPlayer][iNew].hul.ihuldef - 32;
    if (i < 4) {
        rgSB[3] = 0x2;
        if (i < 3) {
            rgSB[2] = 0x2;
        }
    }
    return;
}

int16_t FCreateAiStarbase(int16_t ishdef, int16_t iLevel, int16_t aisb, int16_t isb) {
    int16_t i;
    SHDEF   shdef;
    HS     *lphs;

    if (aisb < 0) {
        switch (ishdef) {
        case 1:
        case 3:
        case 6:
        case 8:
            aisb = 12;
            isb = 0;
            if (iLevel != 2)
                break;
            iLevel = iLevel + 1;
            break;
        default:
            aisb = 0;
            isb = 2;
        }
    }
    if (FCreateAiShdef(-1, isb, &vrgSBAip[aisb]) != 0) {
        shdef = shdefBuild;
        if (iLevel < 3) {
            for (i = 0; i < shdef.hul.chs; i++) {
                lphs = &shdef.hul.rghs[i];
                if (lphs->cItem < 0x4) {
                    if (iLevel <= 1 && rgplr[idPlayer].idAi != 0x4 && (lphs->grhst == hstSpecialSB || lphs->cItem > 0x1)) {
                        lphs->cItem = lphs->cItem + 0xff;
                    }
                } else {
                    lphs->cItem = lphs->cItem >> (0x3 - iLevel);
                }
            }
        }
        shdef.ishdef = ishdef + 16;
        PickANameAndBmp(&shdef, idsGuardianAngel, 13, shdef.hul.ibmp);
        return FChangeAiShdef(&shdef, ishdef + 16);
    }
    return 0;
}

int16_t FAIFling(PLANET *lppl, int32_t *rgResAvail) {
    PLANET *lpplHit;
    POINT16 pt;
    int32_t dy;
    int16_t iT;
    int32_t d2;
    int32_t dBigAssPacket;
    int16_t i;
    int16_t fTwoMAs;
    int16_t iLevelBest;
    PLANET *lpplBest;
    int32_t dx;
    int16_t cFound;
    PLANET *lpplHitMac;
    int32_t l;
    PROD   *lpprod;
    int16_t iKeep;
    int32_t t_merge_83cd_0001;
    int32_t t_merge_8415_0001;

    cFound = 0;
    iLevelBest = -1;
    if (rgplr[idPlayer].idAi != 0x4) {
        i = 0;
        lpprod = lpplProdGlob->rgprod;
        while (1) {
            if (i >= lpplProdGlob->iprodMac)
                goto L_7edc;
            if (lpprod->grobj == grobjPlanet && lpprod->iItem >= iobjPacketIron && lpprod->iItem <= iobjPacketMixed)
                break;
            i = i + 1;
            lpprod = lpprod + 1;
        }
        return 0;
    L_7edc:
        if (rgplr[idPlayer].lvlAi > 0x1 && *rgResAvail + rgResAvail[1] + rgResAvail[2] > 0xbb8 && lppl->fStarbase != 0x0 &&
            IWarpMAFromLppl(lppl, &fTwoMAs) >= 10 && Random(4) == 0) {
            pt = rgptPlan[lppl->id];
            for (iT = 0; iT <= 2 && lppl->rgwtMin[iT] <= 12500; iT++) {
            }
            dBigAssPacket = vrgAiPacketDist[fTwoMAs];
            if (iT <= 2) {
                dBigAssPacket = (uint32_t)(dBigAssPacket * 3);
            }
            lpplHit = lpPlanets;
            lpplHitMac = lpPlanets + cPlanet;
            for (; lpplHit < lpplHitMac; lpplHit++) {
                if (lpplHit->iPlayer != -1 && lpplHit->iPlayer != idPlayer && (uint16_t)(lpplHit->turn + 2) >= game.turn &&
                    ((lpplHit->uDefGuess < 0xe || lpplHit->uPopGuess < 0x2ee) && GetRaceStat(&rgplr[lpplHit->iPlayer], rsMajorAdv) != raMacintosh) &&
                    (lpplHit->fStarbase == 0x0 || GetRaceStat(&rgplr[lpplHit->iPlayer], rsMajorAdv) != raMassAccel)) {
                    dx = (int32_t)(pt.x - rgptPlan[lpplHit->id].x);
                    dy = (int32_t)(pt.y - rgptPlan[lpplHit->id].y);
                    d2 = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                    if (d2 <= dBigAssPacket) {
                        cFound = cFound + 1;
                        if (Random(cFound) == 0) {
                            iLevelBest = 13;
                            lpplBest = lpplHit;
                        }
                    }
                }
            }
            if (iLevelBest >= 10) {
                sel.pl.iWarpFling = iLevelBest - 4;
                sel.pl.idFling = lpplBest->id + 1;
                FLookupPlanet(-1, &sel.pl);
                if (rgResAvail[2] > 20000 && Random(3) != 0) {
                    AddItemToQueue(0x10, 0x50, grobjPlanet, 1);
                }
                if (*rgResAvail <= 3000 || rgResAvail[1] <= 4000 || rgResAvail[2] <= 3000) {
                    if (*rgResAvail <= 1500 || rgResAvail[1] <= 2250 || rgResAvail[2] <= 1500) {
                        for (iT = 0; iT < 3; iT++) {
                            iKeep = iT == 1 ? 2500 : 1250;
                            if (rgResAvail[iT] > (int32_t)iKeep) {
                                l = (int32_t)((rgResAvail[iT] - (int32_t)iKeep) / 0xc8);
                                t_merge_83cd_0001 = l <= 1 ? 1 : l;
                                if ((int32_t)0x19 < t_merge_83cd_0001) {
                                    t_merge_8415_0001 = 25;
                                } else if (l <= 1) {
                                    t_merge_8415_0001 = 1;
                                } else {
                                    t_merge_8415_0001 = l;
                                }
                                l = t_merge_8415_0001;
                                AddItemToQueue(iT + 14, LOWORD(l), grobjPlanet, 1);
                            }
                        }
                    } else {
                        AddItemToQueue(0x11, 0xf, grobjPlanet, 1);
                    }
                } else {
                    AddItemToQueue(0x11, 0x1e, grobjPlanet, 1);
                }
                return 1;
            }
        }
        return 0;
    }
    return 0;
}

int16_t IshdefAiSBLatestOF() {
    if (rglpshdefSB[idPlayer][6].fFree != 0x0 || rglpshdefSB[idPlayer][6].turn <= rglpshdefSB[idPlayer][1].turn) {
        return 1;
    }
    return 6;
}

int16_t IshdefAiSBLatest() {
    if (rglpshdefSB[idPlayer][5].fFree != 0x0 || rglpshdefSB[idPlayer][5].turn <= rglpshdefSB[idPlayer]->turn) {
        return 0;
    }
    return 5;
}

void QueueAiStarbases(PROD *rgprod, int16_t ishdefSBLatest) {
    PLANET *lpplMac;
    PLANET *lppl;
    int16_t i;
    PROD   *lpprod;

    if (ishdefSBLatest != -1 && rgplr[idPlayer].idAi != 0x5 &&
        ((gd.fTutorial == 0x0 && (game.lid != 9236297 || rgplr[idPlayer].idAi != 0x1)) || game.turn <= 0x1e)) {
        lppl = lpPlanets;
        lpplMac = lpPlanets + cPlanet;
        for (; lppl < lpplMac; lppl++) {
            if (rgplr[idPlayer].idAi != 0x4) {
                if (lppl->iPlayer != idPlayer ||
                    ((lppl->fStarbase != 0x0 && lppl->isb <= 0x9) || lppl->rgwtMin[3] < 80 || vlpbAiPlanet[lppl->id * 16 + 2] != 0x0))
                    continue;
                for (i = 0; i < RawLoad16((uint8_t *)vlpbAiData + 0x2) && RawLoad16(vlpbAiData + (i * 20 + 4)) != lppl->id; i++) {
                }
                if (i == RawLoad16((uint8_t *)vlpbAiData + 0x2))
                    continue;
            } else {
                if (lppl->iPlayer != idPlayer)
                    continue;
                ishdefSBLatest = iBuildCyberStarbase(lppl);
                if (ishdefSBLatest == -1)
                    continue;
            }
            ChangeMainObjSel(grobjPlanet, lppl->id);
            InitProduction(rgprod);
            i = 0;
            for (lpprod = lpplProdGlob->rgprod; i < lpplProdGlob->iprodMac && (lpprod->grobj != grobjFleet || lpprod->iItem < iobjPacketGerm); lpprod++) {
                i = i + 1;
            }
            if (i >= lpplProdGlob->iprodMac) {
                AddItemToQueue(ishdefSBLatest + 16, 0x1, grobjFleet, 1);
                FinishProduction(1);
            } else {
                FinishProduction(0);
            }
        }
    }
    return;
}

int16_t FUpgradeAiStarbase(PLANET *lppl, int16_t ishdefSBLatest) {
    int16_t isbCur;
    int16_t iDesigns;
    int16_t i;
    int16_t pctUpg;
    int16_t isbNew;
    PROD   *lpprod;
    int16_t ishdef;
    int16_t t_merge_8c15_0001;

    if (ishdefSBLatest != -1) {
        if (lppl->fStarbase != 0x0) {
            i = 0;
            lpprod = lpplProdGlob->rgprod;
            while (1) {
                if (i >= lpplProdGlob->iprodMac)
                    goto L_88fa;
                if (lpprod->grobj == grobjFleet && lpprod->iItem >= iobjPacketGerm)
                    break;
                i = i + 1;
                lpprod = lpprod + 1;
            }
            return 0;
        }
    L_88fa:
        if (rgplr[idPlayer].idAi != 0x5) {
            if (rgplr[idPlayer].idAi != 0x4 || game.turn >= 0x28) {
                if (lppl->fStarbase != 0x0) {
                    switch (lppl->isb) {
                    case 0x1:
                    case 0x3:
                    case 0x6:
                    case 0x8:
                        iDesigns = 2;
                        ishdefSBLatest = IshdefAiSBLatestOF();
                        break;
                    default:
                        iDesigns = 3;
                    }
                    if (lppl->isb >= (uint16_t)ishdefSBLatest && lppl->isb <= (uint16_t)(ishdefSBLatest + (iDesigns - 1) * 2)) {
                        if (lppl->isb % 0x5 < (iDesigns - 1) * 2 && rglpshdefSB[idPlayer][lppl->isb + 2].fFree == 0x0 && Random(100) < 6) {
                            for (i = 0; i < 3; i++) {
                                if (lppl->rgwtMin[i] < 200) {
                                    return 0;
                                }
                            }
                            AddItemToQueue(lppl->isb + 0x12, 0x1, grobjFleet, 1);
                            return 1;
                        }
                    } else {
                        pctUpg = game.turn - rglpshdefSB[idPlayer][ishdefSBLatest].turn - 10;
                        if (pctUpg >= 0) {
                            if (pctUpg < 50) {
                                pctUpg = pctUpg >> 0x1;
                            }
                        } else {
                            pctUpg = 0;
                        }
                        pctUpg = pctUpg + 5;
                        if (Random(100) < pctUpg) {
                            switch (lppl->isb) {
                            case 0x1:
                            case 0x3:
                            case 0x6:
                            case 0x8:
                                t_merge_8c15_0001 = ishdefSBLatest + lppl->isb % 5 - 1;
                                break;
                            default:
                                t_merge_8c15_0001 = ishdefSBLatest + lppl->isb % 5;
                            }
                            ishdef = t_merge_8c15_0001;
                            AddItemToQueue(ishdef + 16, 0x1, grobjFleet, 1);
                            return 1;
                        }
                    }
                }
                return 0;
            }
            return 0;
        }
        isbCur = lppl->isb;
        if (vAiMacRecycleSB[isbCur] != 0x3 && (vAiMacRecycleSB[isbCur] != 0x2 || Random(100) >= 10)) {
            if (isbCur < 4 || isbCur == 6 || isbCur == 9 || Random(100) >= 8) {
                if (isbCur < 4) {
                    i = PctPlanetCapacity(lppl);
                    if (i > 15) {
                        i = (i - 15) * 6;
                        if (Random(100) < i)
                            goto LDoMacUpgrade;
                    }
                }
                return 0;
            }
            isbNew = isbCur + 1;
            goto LDoMacUpgrade2;
        }
    LDoMacUpgrade:
        if (isbCur >= 4) {
            isbNew = isbCur + 3;
            if (isbNew >= 10) {
                isbNew = isbNew - 6;
            }
        } else {
            for (isbNew = isbCur + 1; isbNew < 9 && vAiMacRecycleSB[isbNew] != 0x0; isbNew++) {
            }
        }
    LDoMacUpgrade2:
        AddItemToQueue(isbNew + 16, 0x1, grobjFleet, 1);
        return 1;
    }
    return 0;
}

int16_t FQueueAiTerraforming(PLANET *lppl, int32_t *rgResAvail, int32_t *rgResCost) {
    int16_t i;
    int16_t j;
    int16_t dEnv;
    int32_t rgItemCost[4];
    PROD   *lpprod;

    if (rgplr[idPlayer].idAi != 0x4) {
        if (lppl->rgwtMin[3] < 200) {
            return 0;
        }
        i = 0;
        lpprod = lpplProdGlob->rgprod;
        while (1) {
            if (i >= lpplProdGlob->iprodMac)
                goto L_8e15;
            if (lpprod->grobj == grobjPlanet && lpprod->iItem == mdIdleTerraform)
                break;
            i = i + 1;
            lpprod = lpprod + 1;
        }
        return 0;
    L_8e15:
        j = -1;
        dEnv = 0;
        for (i = 0; i < 3; i++) {
            if (abs((int16_t)lppl->rgEnvVar[i] - (int16_t)rgplr[idPlayer].rgEnvVar[i]) > dEnv) {
                j = i;
                dEnv = abs((int16_t)lppl->rgEnvVar[i] - (int16_t)rgplr[idPlayer].rgEnvVar[i]);
            }
        }
        if (j != -1) {
            i = 0;
            while (1) {
                if (i >= cProdGlob) {
                    return 0;
                }
                if (pProdGlob[i].grobj == grobjPlanet && pProdGlob[i].iItem == mdIdleTerraform && pProdGlob[i].cItem >= 0x1)
                    break;
                i = i + 1;
            }
            AddItemToQueue(pProdGlob[i].iItem, 0x4 >= pProdGlob[i].cItem ? pProdGlob[i].cItem : 0x4, grobjPlanet, 1);
            pProdGlob[i].cItem = 0x0;
            GetProductionCosts(lppl, pProdGlob + i, rgItemCost, idPlayer, 1);
            for (j = 0; j < 4; j++) {
                rgResCost[j] = rgResCost[j] + (uint32_t)(rgItemCost[j] * (0x4 >= pProdGlob[i].cItem ? (int32_t)pProdGlob[i].cItem : 0x4));
            }
            return 1;
        }
        return 0;
    }
    return 0;
}

int16_t FQueueAiScanner(PLANET *lppl, int32_t *rgResAvail, int32_t *rgResCost) {
    int16_t i;
    int16_t j;
    int32_t rgItemCost[4];
    PROD   *lpprod;

    i = 0;
    lpprod = lpplProdGlob->rgprod;
    while (1) {
        if (i >= lpplProdGlob->iprodMac)
            goto L_91ad;
        if (lpprod->grobj == grobjPlanet && lpprod->iItem >= iobjPlanetaryScannerFirst && lpprod->iItem <= iobjPlanetaryScannerSnooper620X)
            break;
        i = i + 1;
        lpprod = lpprod + 1;
    }
    return 0;
L_91ad:
    for (i = cProdGlob - 1; i >= 0 && (pProdGlob[i].grobj != grobjPlanet || pProdGlob[i].iItem < iobjPlanetaryScannerFirst ||
                                       pProdGlob[i].iItem > iobjPlanetaryScannerSnooper620X);
         i--) {
    }
    if (i >= 0) {
        GetProductionCosts(lppl, pProdGlob + i, rgItemCost, idPlayer, 1);
        for (j = 0; j < 4 && (rgItemCost[j] <= 0 || rgItemCost[j] + rgResCost[j] <= rgResAvail[j]); j++) {
        }
        if (j == 4) {
            AddItemToQueue(pProdGlob[i].iItem, 0x1, grobjPlanet, 1);
            for (j = 0; j < 4; j++) {
                rgResCost[j] = rgResCost[j] + rgItemCost[j];
            }
            return 1;
        }
    }
    return 0;
}

int16_t FQueueAiDefenses(PLANET *lppl, int32_t *rgResAvail, int32_t *rgResCost) {
    int16_t i;
    int16_t j;
    PROD   *lpprod;

    if (lppl->rgwtMin[3] < 1600 || (int32_t)(lppl->rgwtMin[3] / 80) <= lppl->cDefenses) {
        return 0;
    }
    i = 0;
    lpprod = lpplProdGlob->rgprod;
    while (1) {
        if (i >= lpplProdGlob->iprodMac)
            goto L_949f;
        if (lpprod->grobj == grobjPlanet && lpprod->iItem == mdIdleDefense)
            break;
        i = i + 1;
        lpprod = lpprod + 1;
    }
    return 0;
L_949f:
    for (i = 0; i < cProdGlob && (pProdGlob[i].grobj != grobjPlanet || pProdGlob[i].iItem != mdIdleDefense || pProdGlob[i].cItem < 0x1); i++) {
    }
    if (i >= cProdGlob) {
        return 0;
    }
    j = pProdGlob[i].cItem;
    if (j > 4) {
        j = 4;
    }
    AddItemToQueue(0x9, j, grobjPlanet, 1);
    return 1;
}

void HandleBasicAiTasks(int16_t iroCur, PROD *rgprod, int16_t ishdefSBLatest, int32_t *rgResAvail, int32_t *rgResCost) {
    PLANET *lppl;
    int16_t i;
    int16_t ipl;
    int16_t fWrite;

    KeepFleetsMoving();
    QueueAiStarbases(rgprod, ishdefSBLatest);
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0x0)
            break;
        if (rgplr[idPlayer].idAi != 0x0) {
            if (lppl->rgwtMin[3] < 60 && vlpbAiPlanet[lppl->id * 16 + 2] == 0x0)
                continue;
        } else if (lppl->rgwtMin[3] < 40) {
            continue;
        }
        GetResourcesAvailable(lppl, rgResAvail);
        GetProdQCost(lppl, rgResCost);
        if (rgplr[idPlayer].idAi != 0x5) {
            for (i = 0; i < 4 && rgResAvail[i] >= rgResCost[i]; i++) {
            }
            if (i < 3)
                continue;
        }
        ChangeMainObjSel(grobjPlanet, lppl->id);
        InitProduction(rgprod);
        fWrite = 0;
        if (rgplr[idPlayer].idAi != 0x0 && vlpbAiPlanet[lppl->id * 16 + 2] != 0x0) {
            if (rgplr[idPlayer].idAi == 0x4 && FUpgradeAiStarbase(lppl, ishdefSBLatest) != 0) {
                fWrite = 1;
            }
        } else if (FUpgradeAiStarbase(lppl, ishdefSBLatest) != 0 || FAIFling(lppl, rgResAvail) != 0 || FQueueAiScanner(lppl, rgResAvail, rgResCost) != 0 ||
                   FQueueAiDefenses(lppl, rgResAvail, rgResCost) != 0) {
            fWrite = 1;
        }
        if (rgplr[idPlayer].idAi != 0x0 && fWrite == 0 && FQueueAiTerraforming(lppl, rgResAvail, rgResCost) != 0) {
            fWrite = 1;
        }
        FinishProduction(fWrite);
    }
    if (game.fTutorial == 0x0 && game.turn >= (uint16_t)(10 * game.mdSize + 0x14)) {
        FixPlanetsUnderAttack(rgprod);
    }
    AddMinesToBlockedQueues();
    return;
}

void SplitOutShdefs(uint8_t *rgbIsh) {
    int16_t iLast;
    int16_t iFirst;
    int16_t ifl;
    int16_t i;
    FLEET  *lpfl;
    int16_t fUnmarked;
    FLEET   flNew;
    int16_t fMarked;
    FLEET  *lpflNew;

    iLast = -1;
    iFirst = -1;
    for (i = 0; i < 16; i++) {
        if (rgbIsh[i] > 0x0) {
            iLast = i;
            if (iFirst == -1) {
                iFirst = i;
            }
        }
    }
    if (iFirst != -1) {
        while (rgplr[idPlayer].cFleet <= 0x1f4) {
            ifl = 0;
            while (1) {
                if (ifl >= cFleet) {
                    return;
                }
                lpfl = rglpfl[ifl];
                if (rglpfl[ifl] == 0x0) {
                    return;
                }
                if (lpfl->iPlayer == idPlayer && lpfl->fDead == 0x0) {
                    fUnmarked = 0;
                    fMarked = 0;
                    for (i = 0; i < 16; i++) {
                        if (lpfl->rgcsh[i] > 0) {
                            if (rgbIsh[i] == 0x0) {
                                if (fMarked != 0)
                                    goto LDoTheSplit;
                                fUnmarked = 1;
                            } else {
                                if (fUnmarked != 0)
                                    goto LDoTheSplit;
                                fMarked = 1;
                            }
                        }
                    }
                }
                ifl = ifl + 1;
            }
        LDoTheSplit:
            ChangeMainObjSel(grobjFleet, lpfl->id);
            lpflNew = LpflNewSplit(&sel.fl);
            flNew = *lpflNew;
            for (i = 0; i < 16; i++) {
                if (rgbIsh[i] > 0x0) {
                    flNew.rgcsh[i] = sel.fl.rgcsh[i];
                    sel.fl.rgcsh[i] = 0;
                }
            }
            FleetTransferCargoBalance(&sel.fl, &flNew);
            FLookupFleet(-1, &sel.fl);
            FLookupFleet(-1, &flNew);
        }
    }
    return;
}

int16_t CheckAiShdefStatus(int16_t ishBeg, int16_t ishEnd, uint16_t cRecyclePeriod, int16_t *piLatest, uint8_t *rgbOld) {
    uint32_t cExist;
    int16_t  i;
    SHDEF    shdef;

    *piLatest = -1;
    cExist = 0x0;
    for (i = ishBeg; i <= ishEnd; i++) {
        if (rgshdef[i].fFree == 0x0) {
            cExist = cExist + rgshdef[i].cExist;
            if (*piLatest == -1 || rgshdef[*piLatest].turn < rgshdef[i].turn) {
                *piLatest = i;
            }
            if (game.turn - rgshdef[i].turn > cRecyclePeriod) {
                if (rgshdef[i].cExist != 0x0) {
                    rgbOld[i] = 0x1;
                } else {
                    shdef = rgshdef[i];
                    shdef.fFree = 0x1;
                    FChangeAiShdef(&shdef, i);
                }
            }
        }
    }
    if (cExist > 0x7d00) {
        cExist = 0x7d00;
    }
    return LOWORD(cExist);
}

void IncreaseAIMinefieldSizes() {
    THING  *lpth;
    int32_t cMines;
    THING  *lpthMac;

    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (lpth->ith == ithMinefield) {
            cMines = (int32_t)(sqrt((double)lpth->thm.cMines) + 10.5);
            lpth->thm.cMines = (uint32_t)(cMines * cMines);
        }
    }
    return;
}

int16_t FFindBuddyAndJoinUp(FLEET *lpfl, int16_t ishLo, int16_t ishHi, int32_t lMaxDist1, int32_t lMaxDist2) {
    int32_t lDistBest;
    int32_t lDist;
    int16_t i;
    int16_t ifl;
    FLEET  *lpflOther;
    FLEET  *lpflBest;
    ORDER   ord;

    lpflBest = 0x0;
    lDistBest = 9999999;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpflOther = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpflOther->iPlayer == idPlayer) {
            if (lpflOther == lpfl)
                break;
            i = ishLo;
            while (1) {
                if (i > ishHi)
                    goto L_9d3d;
                if (lpflOther->rgcsh[i] != 0)
                    break;
                i = i + 1;
            }
            lDist = LDistance2(lpfl->pt, lpflOther->pt);
            if (lDist < lDistBest) {
                lpflBest = lpflOther;
                lDistBest = lDist;
            }
        }
    L_9d3d:;
    }
    if (lpflBest == 0x0 ||
        (lDistBest > (int32_t)(uint32_t)(lMaxDist1 * lMaxDist1) && (lDistBest > (int32_t)(uint32_t)(lMaxDist2 * lMaxDist2) || Random(2) == 0))) {
        return 0;
    }
    ClearAiCurrentTask(lpfl, 1);
    ord.id = lpflBest->id;
    ord.grobj = grobjFleet;
    ord.pt = lpflBest->pt;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x6;
    FMoveAiFleet(lpfl, &ord, 0);
    return 1;
}

int16_t FShouldPlanetBuildColonizer(PLANET *lpplSrc) {
    POINT16  pt;
    int16_t  i;
    int32_t  lCur;
    uint8_t *lpb;
    int32_t  lBest;

    lBest = 10000000;
    pt = rgptPlan[lpplSrc->id];
    if (game.turn >= 0x3c) {
        lpb = vlpbAiPlanet + 13;
        i = 0;
        while (i < game.cPlanMax) {
            if (*lpb == 0x0) {
                lCur = LDistance2(pt, rgptPlan[i]);
                if (lCur < lBest) {
                    lBest = lCur;
                }
            }
            i = i + 1;
            lpb = lpb + 16;
        }
        if (lBest <= 122500 && (lBest <= 90000 || Random(2) == 0) && (lBest <= 62500 || Random(2) == 0)) {
            return 1;
        }
        return 0;
    }
    return 1;
}

void InitRandomPlanetList() {
    PLANET *lpplMac;
    int16_t iT;
    PLANET *lppl;
    int16_t i;
    int16_t t_a0cb;

    vclpplAi = 0;
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == idPlayer) {
            t_a0cb = vclpplAi;
            vclpplAi = vclpplAi + 1;
            vrglpplAi[t_a0cb] = lppl;
        }
    }
    if (gd.fTutorial == 0x0) {
        for (i = 0; i < vclpplAi - 1; i++) {
            iT = Random(vclpplAi - i) + i;
            lppl = vrglpplAi[i];
            vrglpplAi[i] = vrglpplAi[iT];
            vrglpplAi[iT] = lppl;
        }
    }
    return;
}
