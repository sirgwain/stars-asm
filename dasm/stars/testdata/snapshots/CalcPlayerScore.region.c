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
