int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn) {
    char   *pszTitle;
    int16_t tpm;
    POINT16 pt;
    int16_t i;
    char    szTemp[128];
    HMENU   hmenuSub;
    HMENU   hmenuPopup;
    char   *pszT;
    char   *psz;
    MSG     msg;
    int16_t fChecked;
    int16_t fCheckedCur;
    char   *t_1545;
    char   *t_16d0;
    char   *t_17e3;
    int32_t t_merge_184f_0001;

L_136c:
    hmenuSub = 0;
    pt.x = x;
    pt.y = y;
    ClientToScreen16(hwnd, &pt);
    hmenuPopup = CreatePopupMenu();
    iPopMenuSel = -1;
    i = 0;
    goto L_1887;

L_13ad:
    if (rgids == 0)
        goto L_15ad;
    else
        goto L_13b6;

L_13b6:
    if (iChecked != -2)
        goto L_13c8;
    else
        goto L_13bf;

L_13bf:
    if (rgsz != 0)
        goto L_15ad;
    else
        goto L_13c8;

L_13c8:
    if (rgids[i] != -1)
        goto L_1401;
    else
        goto L_13e5;

L_13e5:
    AppendMenu(hmenuPopup, MF_SEPARATOR, 0, NULL);
    goto L_1883;

L_1401:
    if ((rgids[i] & 0x10000000) != 0)
        goto L_1429;
    else
        goto L_1432;

L_1429:
    psz = "Deep Space";
    goto L_152e;

L_1432:
    if ((rgids[i] & 0x40000000) != 0)
        goto L_145a;
    else
        goto L_147b;

L_145a:
    psz = PszGetCompressedString(LOWORD(rgids[i]));
    goto L_152e;

L_147b:
    if ((rgids[i] & 0x20000000) != 0)
        goto L_14a3;
    else
        goto L_14c4;

L_14a3:
    psz = PszGetThingName(LOWORD(rgids[i]));
    goto L_152e;

L_14c4:
    if ((rgids[i] & 0x80000000) != 0)
        goto L_14ec;
    else
        goto L_1510;

L_14ec:
    psz = PszGetFleetName(LOWORD(rgids[i]) | 0x8000);
    goto L_152e;

L_1510:
    psz = PszGetPlanetName(LOWORD(rgids[i]));

L_152e:
    pszT = szTemp;

L_1536:
    if (*psz == 0)
        goto L_1573;
    else
        goto L_1545;

L_1545:
    t_1545 = psz;
    psz++;
    *pszT++ = *t_1545;
    if (*t_1545 != '&')
        goto L_1536;
    else
        goto L_1564;

L_1564:
    *pszT++ = '&';

L_1570:
    goto L_1536;

L_1573:
    *pszT = 0;
    AppendMenu(hmenuPopup, i == iChecked ? MF_CHECKED : MF_BYCOMMAND, i + 15000, szTemp);

L_15aa:
    goto L_1883;

L_15ad:
    if (rgsz[i] != 0)
        goto L_1771;
    else
        goto L_15bf;

L_15bf:
    pszTitle = rgsz[i + 1];
    fChecked = rgids == 0 ? FALSE : LOWORD(rgids[i + 1]);
    hmenuSub = CreatePopupMenu();
    i += 2;
    goto L_1738;

L_1608:
    if (rgsz[i] == 0)
        goto L_1743;
    else
        goto L_161d;

L_161d:
    if (rgids != 0)
        goto L_1649;
    else
        goto L_1626;

L_1626:
    fCheckedCur = i == iChecked;
    fChecked |= fCheckedCur;
    goto L_165e;

L_1649:
    fCheckedCur = LOWORD(rgids[i]);

L_165e:
    if (*rgsz[i] != -1)
        goto L_16a9;
    else
        goto L_1675;

L_1675:
    if (rgsz[i][1] != 0)
        goto L_16a9;
    else
        goto L_168d;

L_168d:
    AppendMenu(hmenuSub, MF_SEPARATOR, 0, NULL);
    goto L_1734;

L_16a9:
    pszT = szTemp;
    psz = rgsz[i];

L_16c1:
    if (*psz == 0)
        goto L_16fe;
    else
        goto L_16d0;

L_16d0:
    t_16d0 = psz;
    psz++;
    *pszT++ = *t_16d0;
    if (*t_16d0 != '&')
        goto L_16c1;
    else
        goto L_16ef;

L_16ef:
    *pszT++ = '&';

L_16fb:
    goto L_16c1;

L_16fe:
    *pszT = 0;
    AppendMenu(hmenuSub, fCheckedCur == 0 ? MF_BYCOMMAND : MF_CHECKED, i + 15000, szTemp);

L_1734:
    i++;

L_1738:
    if (i < cString)
        goto L_1608;
    else
        goto L_1743;

L_1743:
    AppendMenu(hmenuPopup, (fChecked == 0 ? 0 : 8) | 0x10, (UINT_PTR)hmenuSub, pszTitle);
    goto L_1883;

L_1771:
    if (*rgsz[i] != -1)
        goto L_17bc;
    else
        goto L_1788;

L_1788:
    if (rgsz[i][1] != 0)
        goto L_17bc;
    else
        goto L_17a0;

L_17a0:
    AppendMenu(hmenuPopup, MF_SEPARATOR, 0, NULL);
    goto L_1883;

L_17bc:
    pszT = szTemp;
    psz = rgsz[i];

L_17d4:
    if (*psz == 0)
        goto L_1811;
    else
        goto L_17e3;

L_17e3:
    t_17e3 = psz;
    psz++;
    *pszT++ = *t_17e3;
    if (*t_17e3 != '&')
        goto L_17d4;
    else
        goto L_1802;

L_1802:
    *pszT++ = '&';

L_180e:
    goto L_17d4;

L_1811:
    *pszT = 0;
    if (iChecked != -2)
        goto L_1839;
    else
        goto L_1825;

L_1825:
    t_merge_184f_0001 = rgids[i];
    goto L_184f;

L_1839:
    if (i != iChecked)
        goto L_184b;
    else
        goto L_1844;

L_1844:
    t_merge_184f_0001 = 1;
    goto L_184f;

L_184b:
    t_merge_184f_0001 = 0;

L_184f:
    goto L_186e;

L_186e:
    AppendMenu(hmenuPopup, t_merge_184f_0001 == 0 ? MF_BYCOMMAND : MF_CHECKED, i + 15000, szTemp);

L_1883:
    i++;

L_1887:
    if (i < cString)
        goto L_13ad;
    else
        goto L_1892;

L_1892:
    if (fRightBtn == 0)
        goto L_18a3;
    else
        goto L_189b;

L_189b:
    tpm = TPM_RIGHTBUTTON;
    goto L_18a8;

L_18a3:
    tpm = TPM_LEFTBUTTON;

L_18a8:
    TrackPopupMenu(hmenuPopup, tpm, pt.x, pt.y, 0, hwndFrame, NULL);
    DestroyMenu(hmenuPopup);
    if (hmenuSub == 0)
        goto L_18e7;
    else
        goto L_18de;

L_18de:
    DestroyMenu(hmenuSub);

L_18e7:
    if (PeekMessage(&msg, hwndFrame, 273, 273, 2) == 0)
        goto L_192c;
    else
        goto L_190c;

L_190c:
    if (msg.wParam < 15000)
        goto L_192c;
    else
        goto L_1917;

L_1917:
    if (msg.wParam >= 15100)
        goto L_192c;
    else
        goto L_1922;

L_1922:
    iPopMenuSel = msg.wParam - 15000;

L_192c:

L_1932:
    return iPopMenuSel;
}
