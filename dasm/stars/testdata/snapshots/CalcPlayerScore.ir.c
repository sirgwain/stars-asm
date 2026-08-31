int32_t CalcPlayerScore(int16_t iPlr, SCORE *pscore) {
    int32_t  rgcsh[3];
    int32_t  lTemp;
    SCORE    score;
    PLANET  *lpplMac;
    PLANET  *lppl;
    int16_t  i;
    int16_t  ifl;
    FLEET   *lpfl;
    int16_t  iTech;
    int32_t  lPower;
    int16_t  rgType[16];
    uint32_t t_merge_5cf5_0001;
    uint32_t t_merge_5cf5_0002;

L_58a6:
    memset(score, 0x0, 0x14);
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_59c9;

L_58ee:
    if ((lppl->iPlayer != iPlr))
        goto L_59c5;
    else
        goto L_58fa;

L_58fa:

L_5900:
    score.cPlanet = (score.cPlanet + 0x1);
    /* untranslated: lTemp = (int32_t)(words((HIWORD(lppl->rgwtMin[0x3]) + 0x0), (LOWORD(lppl->rgwtMin[0x3]) + 0x3e7)) / 0x3e8) */
    if ((HIWORD(lTemp) < 0x0))
        goto L_594b;
    else
        goto L_5933;

L_5933:
    if ((HIWORD(lTemp) > 0x0))
        goto L_5941;
    else
        goto L_5938;

L_5938:
    if ((LOWORD(lTemp) <= 0x6))
        goto L_594b;
    else
        goto L_5941;

L_5941:
    lTemp = 0x6;

L_594b:
    score.lScore = (score.lScore + lTemp);
    if ((lppl->fStarbase == 0x0))
        goto L_59ad;
    else
        goto L_596e;

L_596e:
    if ((LphuldefFromId(rglpshdefSB[iPlr][lppl->isb].hul.ihuldef)->hul.wtCargoMax == 0x0))
        goto L_59ad;
    else
        goto L_59a9;

L_59a9:
    score.cStarbase = (score.cStarbase + 0x1);

L_59ad:
    score.cResources = (score.cResources + (uint32_t)(CResourcesAtPlanet(lppl, iPlr)));

L_59c5:
    lppl = (lppl + 0x1);

L_59c9:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_58ee;
    else
        goto L_59d7;

L_59d7:
    score.lScore = (score.lScore + (int32_t)((score.cResources / 0x1e)));
    LOWORD(score.lScore) = (LOWORD(score.lScore) + LOWORD((0x3 * score.cStarbase)));
    HIWORD(score.lScore) = (HIWORD(score.lScore) + SIGNHIWORD(LOWORD((0x3 * score.cStarbase))));
    if (((rgplr[iPlr].wFlags & 0x1) != 0x0))
        goto L_5aa7;
    else
        goto L_5a16;

L_5a16:
    i = 0x0;
    goto L_5a9e;

L_5a1e:
    iTech = (uint16_t)(rgplr[iPlr].rgTech[i]);
    score.cTechLevels = (score.cTechLevels + (uint16_t)(rgplr[iPlr].rgTech[i]));
    if ((iTech >= 0x4))
        goto L_5a52;
    else
        goto L_5a45;

L_5a45:
    score.lScore = (score.lScore + (uint32_t)(iTech));
    goto L_5a9a;

L_5a52:
    if ((iTech >= 0x7))
        goto L_5a6d;
    else
        goto L_5a5b;

L_5a5b:
    score.lScore = (score.lScore + (uint32_t)(((iTech * 0x2) + 0xfffd)));
    goto L_5a9a;

L_5a6d:
    if ((iTech >= 0xa))
        goto L_5a89;
    else
        goto L_5a76;

L_5a76:
    score.lScore = (score.lScore + (uint32_t)((LOWORD((0x3 * iTech)) + 0xfff7)));
    goto L_5a9a;

L_5a89:
    score.lScore = (score.lScore + (uint32_t)(((iTech * 0x4) + 0xffee)));

L_5a9a:
    i = (i + 0x1);

L_5a9e:
    if ((i < 0x6))
        goto L_5a1e;
    else
        goto L_5aa7;

L_5aa7:
    i = 0x0;
    goto L_5b77;

L_5aaf:
    if ((((rglpshdef[iPlr][i].wFlags >> 0x9) & 0x1) == 0x0))
        goto L_5aed;
    else
        goto L_5adc;

L_5adc:
    rgType[i] = 0xffff;
    goto L_5b73;

L_5aed:
    lPower = LComputePower(&(rglpshdef[iPlr][i]));
    if ((HIWORD(lPower) > 0x0))
        goto L_5b3c;
    else
        goto L_5b1d;

L_5b1d:
    if ((HIWORD(lPower) < 0x0))
        goto L_5b2b;
    else
        goto L_5b22;

L_5b22:
    if ((LOWORD(lPower) > 0x0))
        goto L_5b3c;
    else
        goto L_5b2b;

L_5b2b:
    rgType[i] = 0x0;
    goto L_5b73;

L_5b3c:
    if ((HIWORD(lPower) > 0x0))
        goto L_5b65;
    else
        goto L_5b45;

L_5b45:
    if ((HIWORD(lPower) < 0x0))
        goto L_5b54;
    else
        goto L_5b4a;

L_5b4a:
    if ((LOWORD(lPower) >= 0x7d0))
        goto L_5b65;
    else
        goto L_5b54;

L_5b54:
    rgType[i] = 0x1;
    goto L_5b73;

L_5b65:
    rgType[i] = 0x2;

L_5b73:
    i = (i + 0x1);

L_5b77:
    if ((i < 0x10))
        goto L_5aaf;
    else
        goto L_5b80;

L_5b80:
    i = 0x0;
    goto L_5ba1;

L_5b88:
    rgcsh[i] = 0x0;
    i = (i + 0x1);

L_5ba1:
    if ((i < 0x3))
        goto L_5b88;
    else
        goto L_5baa;

L_5baa:
    ifl = 0x0;
    goto L_5bb6;

L_5bb2:
    ifl = (ifl + 0x1);

L_5bb6:
    if ((ifl >= cFleet))
        goto L_5c93;
    else
        goto L_5bc1;

L_5bc1:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_5bf1;
    else
        goto L_5be9;

L_5be9:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_5c93;
    else
        goto L_5bf1;

L_5bf1:
    if ((lpfl->iPlayer != iPlr))
        goto L_5bb2;
    else
        goto L_5c00;

L_5c00:
    if ((lpfl->fDead != 0x0))
        goto L_5bb2;
    else
        goto L_5c14;

L_5c14:

L_5c1a:
    i = 0x0;
    goto L_5c87;

L_5c22:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_5c83;
    else
        goto L_5c3f;

L_5c3f:
    if ((rgType[i] == 0xffff))
        goto L_5c83;
    else
        goto L_5c51;

L_5c51:
    rgcsh[rgType[i]] = (rgcsh[rgType[i]] + (uint32_t)(lpfl->rgcsh[i]));

L_5c83:
    i = (i + 0x1);

L_5c87:
    if ((i >= 0x10))
        goto L_5bb2;
    else
        goto L_5c8d;

L_5c8d:

L_5c93:
    if ((HIWORD(rgcsh[0x1]) > SIGNHIWORD(score.cPlanet)))
        goto L_5cb5;
    else
        goto L_5c9f;

L_5c9f:
    if ((HIWORD(rgcsh[0x1]) < SIGNHIWORD(score.cPlanet)))
        goto L_5cac;
    else
        goto L_5ca4;

L_5ca4:
    if ((LOWORD(rgcsh[0x1]) >= score.cPlanet))
        goto L_5cb5;
    else
        goto L_5cac;

L_5cac:
    goto L_5cb9;

L_5cb5:

L_5cb9:
    if ((HIWORD(rgcsh[0x0]) > SIGNHIWORD(score.cPlanet)))
        goto L_5cf1;
    else
        goto L_5cdb;

L_5cdb:
    if ((HIWORD(rgcsh[0x0]) < SIGNHIWORD(score.cPlanet)))
        goto L_5ce8;
    else
        goto L_5ce0;

L_5ce0:
    if ((LOWORD(rgcsh[0x0]) >= score.cPlanet))
        goto L_5cf1;
    else
        goto L_5ce8;

L_5ce8:
    t_merge_5cf5_0001 = rgcsh[0x0];
    t_merge_5cf5_0002 = rgcsh[0x0];
    goto L_5cf5;

L_5cf1:
    t_merge_5cf5_0001 = (uint32_t)(score.cPlanet);
    t_merge_5cf5_0002 = (uint32_t)(score.cPlanet);

L_5cf5:
    /* untranslated: LOWORD(score.lScore) = (LOWORD(score.lScore) + (loword((int32_t)(t_merge_5cf5_0001 / 0x2)) + ss:[bp-0x60])) */
    /* untranslated: HIWORD(score.lScore) = (HIWORD(score.lScore) + (hiword((int32_t)(t_merge_5cf5_0002 / 0x2)) + ss:[bp-0x5e])) */
    if ((HIWORD(rgcsh[0x2]) < 0x0))
        goto L_5d57;
    else
        goto L_5d15;

L_5d15:
    if ((HIWORD(rgcsh[0x2]) > 0x0))
        goto L_5d23;
    else
        goto L_5d1a;

L_5d1a:
    if ((LOWORD(rgcsh[0x2]) <= 0x0))
        goto L_5d57;
    else
        goto L_5d23;

L_5d23:
    /* untranslated: score.lScore = (score.lScore + (int32_t)((uint32_t)((int32_t)(rgcsh[0x2] * 0x8) * sext16to32(score.cPlanet)) /
     * words((signhiword(score.cPlanet) + HIWORD(rgcsh[0x2])), (score.cPlanet + LOWORD(rgcsh[0x2]))))) */

L_5d57:
    i = 0x0;
    goto L_5d88;

L_5d5f:
    score.rgcsh[i] = WPackLong(rgcsh[i]);
    i = (i + 0x1);

L_5d88:
    if ((i < 0x3))
        goto L_5d5f;
    else
        goto L_5d91;

L_5d91:
    if ((pscore == 0x0))
        goto L_5daa;
    else
        goto L_5d9a;

L_5d9a:
    *(pscore) = score;

L_5daa:

L_5db3:
    return score.lScore;
}
