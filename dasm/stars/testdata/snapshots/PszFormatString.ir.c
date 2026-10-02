char *PszFormatString(char *pszFormat, int16_t *pParamsReal) {
    int16_t  iMineral;
    int16_t  cOut;
    int16_t  c;
    int16_t  i;
    int16_t *pParams;
    char    *pchT;
    char     szBuf[480];
    uint16_t w;
    char    *pch;
    PART     part;
    int32_t  l;
    SHDEF   *lpshdef;

L_85cc:
    iMineral = -1;
    pParams = pParamsReal;
    pch = szMsgBuf;

L_85ec:
    if (*pszFormat == 0)
        goto L_8f54;
    else
        goto L_85fa;

L_85fa:
    if (*pszFormat == '\\')
        goto L_861b;
    else
        goto L_8608;

L_8608:
    *pch++ = *pszFormat;
    goto L_8f4d;

L_861b:
    pszFormat++;
    goto L_8ecd;

L_8628:
    strcpy(pch, szWork);
    pch += strlen(szWork);
    goto L_8f4d;

L_864b:
    strcpy(pch, szBase);
    pch += strlen(szBase);
    goto L_8727;

L_8674:
    if (idPlayer == -1)
        goto L_86a2;
    else
        goto L_867e;

L_867e:
    c = _wsprintf(pch, ".x%d", idPlayer + 1);
    goto DoInt;

L_86a2:
    if (idPlayer == -1)
        goto L_86d0;
    else
        goto L_86ac;

L_86ac:
    c = _wsprintf(pch, ".m%d", idPlayer + 1);
    goto DoInt;

L_86d0:
    strcat(pch, ".hst");
    pch += 4;
    goto L_8f4d;

L_86e8:
    c = _wsprintf(pch, ".h%d", idPlayer + 1);
    goto DoInt;

L_870c:
    strcat(pch, ".xy");
    pch += 3;
    goto L_8f4d;

L_8727:
    if (*pszFormat == 'f')
        goto L_8674;
    else
        goto L_872f;

L_872f:
    if (*pszFormat == 'h')
        goto L_86d0;
    else
        goto L_8737;

L_8737:
    if (*pszFormat == 'r')
        goto L_86e8;
    else
        goto L_873f;

L_873f:
    if (*pszFormat == 't')
        goto L_86a2;
    else
        goto L_8747;

L_8747:
    if (*pszFormat != 'y')
        goto L_8f4d;
    else
        goto L_874c;

L_874c:
    goto L_870c;

L_8755:
    pchT = rgszPlanetAttr[*pParams];
    goto FinishString;

L_8767:
    pchT = PszCalcEnvVar((uint16_t)*pParams >> 8 & 0xff & 0xff, *pParams & 0xff);
    goto FinishString;

L_8791:
    pchT = PszGetCompressedString(*pParams + 1348);
    goto FinishString;

L_87a9:
    c = _wsprintf(pch, PCTD, *pParams);

DoInt:
    pch += c;
    pParams++;
    goto L_8f4d;

L_87d7:
    pchT = PszPlayerName(*pParams & 0xf, *pszFormat == 'L', (*pParams & 0x10) != 0, (*pParams & 0x20) != 0, (*pParams & 0xc0) >> 6, NULL);
    goto FinishString;

L_8850:
    w = *pParams;
    if (w == 0)
        goto L_8f4d;
    else
        goto L_8867;

L_8867:
    if ((w - 1 & w) != 0)
        goto L_88c1;
    else
        goto L_887a;

L_887a:
    c = 0;

L_887f:
    if ((w & 1) != 0)
        goto L_889c;
    else
        goto L_888e;

L_888e:
    c++;
    w >>= 1;
    goto L_887f;

L_889c:
    pchT = PszPlayerName(c, FALSE, TRUE, TRUE, 0, NULL);
    goto FinishString;

L_88c1:
    cOut = 0;
    i = 0;
    goto L_88e0;

L_88ce:
    i++;
    w >>= 1;

L_88e0:
    if (i >= game.cPlayer)
        goto DoNothing;
    else
        goto L_88eb;

L_88eb:
    if ((w & 1) == 0)
        goto L_88ce;
    else
        goto L_88fd;

L_88fd:
    if (cOut <= 0)
        goto L_8944;
    else
        goto L_8906;

L_8906:
    if ((w & 0xfffe) == 0)
        goto L_8930;
    else
        goto L_8915;

L_8915:
    *pch++ = ',';
    *pch++ = ' ';
    goto L_8944;

L_8930:
    pch += CchGetString(idsAnd, pch);

L_8944:
    pchT = PszPlayerName(i, FALSE, TRUE, TRUE, 0, NULL);
    strcpy(pch, pchT);
    pch += strlen(pchT);
    cOut++;
    goto L_88ce;

L_898e:
    if (*pParams == idPlayer)
        goto DoNothing;
    else
        goto L_899f;

L_899f:
    CchGetString(idsOf2, szBuf);
    pchT = PszPlayerName(*pParams, FALSE, FALSE, FALSE, 0, NULL);
    strcat(szBuf, pchT);
    strcat(szBuf, PszGetCompressedString(idsOrigin));
    pchT = szBuf;
    goto FinishString;

L_8a09:
    iMineral = *pParams;
    pchT = rgszMinerals[iMineral];
    goto FinishString;

L_8a21:
    pchT = rgszMineField[*pParams];
    goto FinishString;

L_8a33:
    if ((long double)(int16_t)(*pParams / 100) < (long double)10.0)
        goto L_8a89;
    else
        goto L_8a5f;

L_8a5f:
    c = _wsprintf(pch, PCTDPCTPCT, *pParams / 100);
    goto L_8aca;

L_8a89:
    c = _wsprintf(pch, PCTDXPCTDPCTPCT, *pParams / 100, *pParams - *pParams / 100 * 100);

L_8aca:
    pch += c;
    pParams++;
    goto L_8f4d;

DoPlanet:
    pchT = PszGetPlanetName(*pParams);

FinishString:
    strcpy(pch, pchT);
    pch += strlen(pchT);

DoNothing:
    pParams++;
    goto L_8f4d;

L_8b11:
    pchT = PszFleetNameFromWord(*pParams);
    goto FinishString;

DoFleet:
    w = *pParams | 0x8000;
    pchT = PszGetFleetName(w);
    goto FinishString;

L_8b46:
    pchT = PszGetCompressedString(*pParams + 84);
    goto FinishString;

L_8b5e:
    part.hs.grhst = *pParams;
    pParams++;
    part.hs.iItem = *pParams;
    if (FLookupPart(&part) <= 0)
        goto L_8ba8;
    else
        goto L_8ba2;

L_8ba2:
    goto L_8bab;

L_8ba8:

L_8bab:
    fstrcpy(pch, part.pcom->szName);
    pch += fstrlen(part.pcom->szName);
    pParams++;
    goto L_8f4d;

LThingName:
    pchT = PszGetThingName(*pParams);
    goto FinishString;

L_8c00:
    w = *pParams;
    c = CchGetString(w + 1250, pch);
    pch += c;
    pParams++;
    goto L_8f4d;

L_8c2f:
    if (*pParams != -2)
        goto L_8c45;
    else
        goto L_8c3b;

L_8c3b:
    pParams++;
    goto LThingName;

L_8c45:
    if (*pParams == -1)
        goto L_8c78;
    else
        goto L_8c51;

L_8c51:
    pchT = PszGetLocName(grobjNone, -1, *pParams, pParams[1]);
    pParams++;
    goto FinishString;

L_8c78:
    pParams++;

L_8c7c:
    if ((*pParams & 0x8000) != 0)
        goto DoFleet;
    else
        goto L_8c8a;

L_8c8a:
    goto DoPlanet;

L_8c96:
    w = (uint16_t)*pParams >> 9 & 0xf;
    pchT = PszPlayerName(w, FALSE, FALSE, FALSE, 0, NULL);
    goto FinishString;

L_8ccf:
    c = _wsprintf(pch, "%u", *pParams);
    pch += c;
    pParams++;
    goto L_8f4d;

L_8cfd:
    l = (int32_t)((uint32_t)pParams[1] << 0x10) | (uint32_t)*pParams;
    pParams += 2;
    c = _wsprintf(pch, PCTLD, l);
    pch += c;
    if (*pszFormat == 'v')
        goto L_8f4d;
    else
        goto L_8d71;

L_8d71:
    if (*pszFormat != 'V')
        goto L_8d88;
    else
        goto L_8d7f;

L_8d7f:
    iMineral = *pParams;

L_8d88:
    pchT = vrgszUnits[iMineral];
    strcpy(pch, pchT);
    pch += strlen(pchT);

L_8db2:
    goto L_8f4d;

L_8db5:
    c = *pParams >> 5;
    w = *pParams & 0x1f;
    if (w < 16)
        goto L_8e07;
    else
        goto L_8ddf;

L_8ddf:
    lpshdef = rglpshdefSB[c] + (w - 16);
    goto L_8e27;

L_8e07:
    lpshdef = rglpshdef[c] + w;

L_8e27:
    if (c == idPlayer)
        goto L_8e84;
    else
        goto L_8e32;

L_8e32:
    pchT = PszPlayerName(c, FALSE, FALSE, TRUE, 0, NULL);
    _wsprintf(pch, "%s %s", pchT, lpshdef->hul.szClass);
    goto L_8ea3;

L_8e84:
    fstrcpy(pch, lpshdef->hul.szClass);

L_8ea3:
    pch += strlen(pch);
    pParams++;
    goto L_8f4d;

L_8eba:
    *pch++ = *pszFormat;
    goto L_8f4d;

L_8ecd:
    if ((uint16_t)(*pszFormat - 'E') > 53)
        goto L_8eba;
    else
        goto L_8ed8;

L_8ed8:
    switch ((*pszFormat - 'E') * 2) {
    case 0x0:
        goto L_8767;
    case 0x2:
        goto L_8b11;
    case 0x4:
        goto L_8c00;
    case 0x6:
        goto L_8eba;
    case 0x8:
        goto L_8791;
    case 0xa:
        goto L_8eba;
    case 0xc:
        goto L_8eba;
    case 0xe:
        goto L_87d7;
    case 0x10:
        goto L_8a21;
    case 0x12:
        goto L_8eba;
    case 0x14:
        goto L_8c96;
    case 0x16:
        goto L_8a33;
    case 0x18:
        goto L_8eba;
    case 0x1a:
        goto L_8eba;
    case 0x1c:
        goto L_898e;
    case 0x1e:
        goto L_8eba;
    case 0x20:
        goto L_8cfd;
    case 0x22:
        goto L_8cfd;
    case 0x24:
        goto L_8eba;
    case 0x26:
        goto DoNothing;
    case 0x28:
        goto L_8eba;
    case 0x2a:
        goto L_8850;
    case 0x2c:
        goto L_8eba;
    case 0x2e:
        goto L_8eba;
    case 0x30:
        goto L_8eba;
    case 0x32:
        goto L_8eba;
    case 0x34:
        goto L_8eba;
    case 0x36:
        goto L_8eba;
    case 0x38:
        goto L_8eba;
    case 0x3a:
        goto L_8eba;
    case 0x3c:
        goto L_8eba;
    case 0x3e:
        goto L_8eba;
    case 0x40:
        goto L_8755;
    case 0x42:
        goto L_864b;
    case 0x44:
        goto LThingName;
    case 0x46:
        goto L_864b;
    case 0x48:
        goto L_87a9;
    case 0x4a:
        goto L_8b46;
    case 0x4c:
        goto L_8b5e;
    case 0x4e:
        goto L_87d7;
    case 0x50:
        goto L_8a09;
    case 0x52:
        goto L_8c2f;
    case 0x54:
        goto L_8c7c;
    case 0x56:
        goto DoPlanet;
    case 0x58:
        goto L_8eba;
    case 0x5a:
        goto L_864b;
    case 0x5c:
        goto DoFleet;
    case 0x5e:
        goto L_864b;
    case 0x60:
        goto L_8ccf;
    case 0x62:
        goto L_8cfd;
    case 0x64:
        goto L_8628;
    case 0x66:
        goto L_8eba;
    case 0x68:
        goto L_864b;
    case 0x6a:
        goto L_8db5;
    }

L_8f4d:
    pszFormat++;
    goto L_85ec;

L_8f54:
    *pch = 0;

L_8f61:
    return szMsgBuf;
}
