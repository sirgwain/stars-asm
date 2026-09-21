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
    int32_t t_scratch_m60_2;
    int32_t t_merge_5cf5_0001;

L_58a6:
    memset(&(score), 0, 0x14);
    lppl = lpPlanets;
    lpplMac = (lpPlanets + cPlanet);
    goto L_59c9;

L_58ee:
    if ((lppl->iPlayer != iPlr))
        goto L_59c5;
    else
        goto L_5900;

L_5900:
    score.cPlanet = (score.cPlanet + 1);
    lTemp = (int32_t)(((lppl->rgwtMin[3] + 999) / 0x3e8));
    if ((lTemp <= 6))
        goto L_594b;
    else
        goto L_5941;

L_5941:
    lTemp = 6;

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
    score.cStarbase = (score.cStarbase + 1);

L_59ad:
    score.cResources = (score.cResources + (int32_t)(CResourcesAtPlanet(lppl, iPlr)));

L_59c5:
    lppl = (lppl + 1);

L_59c9:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_58ee;
    else
        goto L_59d7;

L_59d7:
    score.lScore = (score.lScore + (int32_t)((score.cResources / 30)));
    score.lScore = (score.lScore + (int32_t)(LOWORD((3 * score.cStarbase))));
    if ((rgplr[iPlr].fDead != 0x0))
        goto L_5aa7;
    else
        goto L_5a16;

L_5a16:
    i = 0;
    goto L_5a9e;

L_5a1e:
    iTech = (int16_t)(rgplr[iPlr].rgTech[i]);
    score.cTechLevels = (score.cTechLevels + (int16_t)(rgplr[iPlr].rgTech[i]));
    if ((iTech >= 4))
        goto L_5a52;
    else
        goto L_5a45;

L_5a45:
    score.lScore = (score.lScore + (int32_t)(iTech));
    goto L_5a9a;

L_5a52:
    if ((iTech >= 7))
        goto L_5a6d;
    else
        goto L_5a5b;

L_5a5b:
    score.lScore = (score.lScore + (int32_t)(((iTech * 2) - 3)));
    goto L_5a9a;

L_5a6d:
    if ((iTech >= 10))
        goto L_5a89;
    else
        goto L_5a76;

L_5a76:
    score.lScore = (score.lScore + (int32_t)((LOWORD((3 * iTech)) + 0xfff7)));
    goto L_5a9a;

L_5a89:
    score.lScore = (score.lScore + (int32_t)(((iTech * 4) - 18)));

L_5a9a:
    i = (i + 1);

L_5a9e:
    if ((i < 6))
        goto L_5a1e;
    else
        goto L_5aa7;

L_5aa7:
    i = 0;
    goto L_5b77;

L_5aaf:
    if ((rglpshdef[iPlr][i].fFree == 0x0))
        goto L_5aed;
    else
        goto L_5adc;

L_5adc:
    rgType[i] = -1;
    goto L_5b73;

L_5aed:
    lPower = LComputePower(&(rglpshdef[iPlr][i]));
    if ((lPower <= 0))
        goto L_5b2b;
    else
        goto L_5b3c;

L_5b2b:
    rgType[i] = 0;
    goto L_5b73;

L_5b3c:
    if ((lPower < 2000))
        goto L_5b54;
    else
        goto L_5b65;

L_5b54:
    rgType[i] = 1;
    goto L_5b73;

L_5b65:
    rgType[i] = 2;

L_5b73:
    i = (i + 1);

L_5b77:
    if ((i < 16))
        goto L_5aaf;
    else
        goto L_5b80;

L_5b80:
    i = 0;
    goto L_5ba1;

L_5b88:
    rgcsh[i] = 0;
    i = (i + 1);

L_5ba1:
    if ((i < 3))
        goto L_5b88;
    else
        goto L_5baa;

L_5baa:
    ifl = 0;
    goto L_5bb6;

L_5bb2:
    ifl = (ifl + 1);

L_5bb6:
    if ((ifl >= cFleet))
        goto L_5c93;
    else
        goto L_5bc1;

L_5bc1:
    lpfl = rglpfl[ifl];
    if ((rglpfl[ifl] != 0x0))
        goto L_5bf1;
    else
        goto L_5c93;

L_5bf1:
    if ((lpfl->iPlayer != iPlr))
        goto L_5bb2;
    else
        goto L_5c00;

L_5c00:
    if ((lpfl->fDead != 0x0))
        goto L_5bb2;
    else
        goto L_5c1a;

L_5c1a:
    i = 0;
    goto L_5c87;

L_5c22:
    if ((lpfl->rgcsh[i] <= 0))
        goto L_5c83;
    else
        goto L_5c3f;

L_5c3f:
    if ((rgType[i] == -1))
        goto L_5c83;
    else
        goto L_5c51;

L_5c51:
    rgcsh[rgType[i]] = (rgcsh[rgType[i]] + (int32_t)(lpfl->rgcsh[i]));

L_5c83:
    i = (i + 1);

L_5c87:
    if ((i >= 16))
        goto L_5bb2;
    else
        goto L_5c8d;

L_5c8d:
    goto L_5c22;

L_5c93:
    if ((rgcsh[1] < (int32_t)(score.cPlanet)))
        goto L_5cac;
    else
        goto L_5cb5;

L_5cac:
    t_merge_5cb9_0001 = rgcsh[1];
    goto L_5cb9;

L_5cb5:
    t_merge_5cb9_0001 = (int32_t)(score.cPlanet);

L_5cb9:
    t_scratch_m60_2 = (int32_t)((t_merge_5cb9_0001 * 2));
    if ((rgcsh[0] < (int32_t)(score.cPlanet)))
        goto L_5ce8;
    else
        goto L_5cf1;

L_5ce8:
    t_merge_5cf5_0001 = rgcsh[0];
    goto L_5cf5;

L_5cf1:
    t_merge_5cf5_0001 = (int32_t)(score.cPlanet);

L_5cf5:
    score.lScore = (score.lScore + ((int32_t)((t_merge_5cf5_0001 / 2)) + t_scratch_m60_2));
    if ((rgcsh[2] <= 0))
        goto L_5d57;
    else
        goto L_5d23;

L_5d23:
    score.lScore = (score.lScore + (int32_t)(((uint32_t)(((int32_t)((rgcsh[2] * 8)) * (int32_t)(score.cPlanet))) / ((int32_t)(score.cPlanet) + rgcsh[2]))));

L_5d57:
    i = 0;
    goto L_5d88;

L_5d5f:
    score.rgcsh[i] = WPackLong(rgcsh[i]);
    i = (i + 1);

L_5d88:
    if ((i < 3))
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
