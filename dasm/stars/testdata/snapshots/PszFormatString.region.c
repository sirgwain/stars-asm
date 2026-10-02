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

    iMineral = -1;
    pParams = pParamsReal;
    pch = szMsgBuf;
    for (; *pszFormat != 0; pszFormat++) {
        if (*pszFormat != '\\') {
            *pch++ = *pszFormat;
        } else {
            pszFormat++;
            if ((uint16_t)(*pszFormat - 69) <= 53) {
                switch (*pszFormat) {
                case 'w':
                    strcpy(pch, szWork);
                    pch += strlen(szWork);
                    break;
                case 'f':
                case 'h':
                case 'r':
                case 't':
                case 'y':
                    strcpy(pch, szBase);
                    pch += strlen(szBase);
                    switch (*pszFormat) {
                    case 'f':
                        if (idPlayer != -1) {
                            c = _wsprintf(pch, ".x%d", idPlayer + 1);
                            goto DoInt;
                        }
                    case 't':
                        if (idPlayer != -1) {
                            c = _wsprintf(pch, ".m%d", idPlayer + 1);
                            goto DoInt;
                        }
                    case 'h':
                        strcat(pch, ".hst");
                        pch += 4;
                        break;
                    case 'r':
                        c = _wsprintf(pch, ".h%d", idPlayer + 1);
                        goto DoInt;
                    case 'y':
                        strcat(pch, ".xy");
                        pch += 3;
                    }
                    break;
                case 'e':
                    pchT = rgszPlanetAttr[*pParams];
                    goto FinishString;
                case 'E':
                    pchT = PszCalcEnvVar((uint16_t)*pParams >> 8 & 0xff & 0xff, *pParams & 0xff);
                    goto FinishString;
                case 'I':
                    pchT = PszGetCompressedString(*pParams + 1348);
                    goto FinishString;
                case 'i':
                    c = _wsprintf(pch, PCTD, *pParams);
                    goto DoInt;
                case 'L':
                case 'l':
                    pchT = PszPlayerName(*pParams & 0xf, *pszFormat == 'L' ? 1 : 0, (*pParams & 0x10) == 0 ? 0 : 1, (*pParams & 0x20) == 0 ? 0 : 1,
                                         (*pParams & 0xc0) >> 6, NULL);
                    goto FinishString;
                case 'Z':
                    w = *pParams;
                    if (w == 0)
                        break;
                    if ((w - 1 & w) == 0) {
                        c = 0;
                        for (; (w & 1) == 0; w >>= 1) {
                            c++;
                        }
                        pchT = PszPlayerName(c, 0, 1, 1, 0, NULL);
                        goto FinishString;
                    }
                    cOut = 0;
                    i = 0;
                    while (i < game.cPlayer) {
                        if ((w & 1) != 0) {
                            if (cOut > 0) {
                                if ((w & 0xfffe) != 0) {
                                    *pch++ = ',';
                                    *pch++ = ' ';
                                } else {
                                    pch += CchGetString(idsAnd, pch);
                                }
                            }
                            pchT = PszPlayerName(i, 0, 1, 1, 0, NULL);
                            strcpy(pch, pchT);
                            pch += strlen(pchT);
                            cOut++;
                        }
                        i++;
                        w >>= 1;
                    }
                    goto DoNothing;
                case 'S':
                    if (*pParams == idPlayer)
                        goto DoNothing;
                    CchGetString(idsOf2, szBuf);
                    pchT = PszPlayerName(*pParams, 0, 0, 0, 0, NULL);
                    strcat(szBuf, pchT);
                    strcat(szBuf, PszGetCompressedString(idsOrigin));
                    pchT = szBuf;
                    goto FinishString;
                case 'm':
                    iMineral = *pParams;
                    pchT = rgszMinerals[iMineral];
                    goto FinishString;
                case 'M':
                    pchT = rgszMineField[*pParams];
                    goto FinishString;
                case 'P':
                    if ((long double)(int16_t)(*pParams / 100) >= (long double)10.0) {
                        c = _wsprintf(pch, PCTDPCTPCT, *pParams / 100);
                    } else {
                        c = _wsprintf(pch, PCTDXPCTDPCTPCT, *pParams / 100, *pParams - *pParams / 100 * 100);
                    }
                    pch += c;
                    pParams++;
                    break;
                case 'p':
                DoPlanet:
                    pchT = PszGetPlanetName(*pParams);
                    goto FinishString;
                case 'X':
                DoNothing:
                    pParams++;
                    break;
                case 'F':
                    pchT = PszFleetNameFromWord(*pParams);
                    goto FinishString;
                case 's':
                DoFleet:
                    w = *pParams | 0x8000;
                    pchT = PszGetFleetName(w);
                    goto FinishString;
                case 'j':
                    pchT = PszGetCompressedString(*pParams + 84);
                    goto FinishString;
                case 'k':
                    part.hs.grhst = *pParams;
                    pParams++;
                    part.hs.iItem = *pParams;
                    if (FLookupPart(&part) <= 0) {
                    }
                    fstrcpy(pch, part.pcom->szName);
                    pch += fstrlen(part.pcom->szName);
                    pParams++;
                    break;
                case 'g':
                LThingName:
                    pchT = PszGetThingName(*pParams);
                    goto FinishString;
                case 'G':
                    w = *pParams;
                    c = CchGetString(w + 1250, pch);
                    pch += c;
                    pParams++;
                    break;
                case 'n':
                    if (*pParams == -2) {
                        pParams++;
                        goto LThingName;
                    }
                    if (*pParams != -1) {
                        pchT = PszGetLocName(grobjNone, -1, *pParams, pParams[1]);
                        pParams++;
                        goto FinishString;
                    }
                    pParams++;
                case 'o':
                    if ((*pParams & 0x8000) != 0)
                        goto DoFleet;
                    goto DoPlanet;
                case 'O':
                    w = (uint16_t)*pParams >> 9 & 0xf;
                    pchT = PszPlayerName(w, 0, 0, 0, 0, NULL);
                    goto FinishString;
                case 'u':
                    c = _wsprintf(pch, "%u", *pParams);
                    pch += c;
                    pParams++;
                    break;
                case 'U':
                case 'V':
                case 'v':
                    l = (int32_t)((uint32_t)pParams[1] << 0x10) | (uint32_t)*pParams;
                    pParams += 2;
                    c = _wsprintf(pch, PCTLD, l);
                    pch += c;
                    if (*pszFormat == 'v')
                        break;
                    if (*pszFormat == 'V') {
                        iMineral = *pParams;
                    }
                    pchT = vrgszUnits[iMineral];
                    strcpy(pch, pchT);
                    pch += strlen(pchT);
                    break;
                case 'z':
                    c = *pParams >> 5;
                    w = *pParams & 0x1f;
                    if (w >= 16) {
                        lpshdef = rglpshdefSB[c] + (w - 16);
                    } else {
                        lpshdef = rglpshdef[c] + w;
                    }
                    if (c != idPlayer) {
                        pchT = PszPlayerName(c, 0, 0, 1, 0, NULL);
                        _wsprintf(pch, "%s %s", pchT, lpshdef->hul.szClass);
                    } else {
                        fstrcpy(pch, lpshdef->hul.szClass);
                    }
                    pch += strlen(pch);
                    pParams++;
                    break;
                case 'H':
                case 'J':
                case 'K':
                case 'N':
                case 'Q':
                case 'R':
                case 'T':
                case 'W':
                case 'Y':
                case '[':
                case '\\':
                case ']':
                case '^':
                case '_':
                case '`':
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'q':
                case 'x':
                    goto L_8eba;
                }
                continue;
            DoInt:
                pch += c;
                pParams++;
                continue;
            FinishString:
                strcpy(pch, pchT);
                pch += strlen(pchT);
                goto DoNothing;
            }
        L_8eba:
            *pch++ = *pszFormat;
        }
    }
    *pch = 0;
    return szMsgBuf;
}
