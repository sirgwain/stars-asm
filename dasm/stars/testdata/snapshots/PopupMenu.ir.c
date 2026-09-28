int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn) {
    char    *pszTitle;
    int16_t  tpm;
    POINT    pt;
    int16_t  i;
    char     szTemp[128];
    HMENU    hmenuSub;
    HMENU    hmenuPopup;
    char    *pszT;
    char    *psz;
    MSG      msg;
    int16_t  fChecked;
    int16_t  fCheckedCur;
    uint16_t t_merge_1592_0001;
    uint16_t t_merge_15f4_0001;
    uint16_t t_merge_163a_0001;
    uint16_t t_merge_171c_0001;
    uint16_t t_merge_175a_0001;
    int32_t  t_merge_184f_0001;
    uint16_t t_merge_186e_0001;

L_136c:
    hmenuSub = 0x0;
    pt.x = x;
    pt.y = y;
    ClientToScreen(hwnd, &(pt));
    hmenuPopup = CreatePopupMenu();
    iPopMenuSel = -1;
    i = 0;
    goto L_1887;

L_13ad:
    if ((rgids == 0x0))
        goto L_15ad;
    else
        goto L_13b6;

L_13b6:
    if ((iChecked != -2))
        goto L_13c8;
    else
        goto L_13bf;

L_13bf:
    if ((rgsz != 0x0))
        goto L_15ad;
    else
        goto L_13c8;

L_13c8:
    if ((rgids[i] != -1))
        goto L_1401;
    else
        goto L_13e5;

L_13e5:
    AppendMenu(hmenuPopup, 0x800, 0x0, 0x0);
    goto L_1883;

L_1401:
    if (((rgids[i] & 0x10000000) != 0x0))
        goto L_1429;
    else
        goto L_1432;

L_1429:
    psz = "Deep Space";
    goto L_152e;

L_1432:
    if (((rgids[i] & 0x40000000) != 0x0))
        goto L_145a;
    else
        goto L_147b;

L_145a:
    psz = PszGetCompressedString(LOWORD(rgids[i]));
    goto L_152e;

L_147b:
    if (((rgids[i] & 0x20000000) != 0x0))
        goto L_14a3;
    else
        goto L_14c4;

L_14a3:
    psz = PszGetThingName(LOWORD(rgids[i]));
    goto L_152e;

L_14c4:
    if (((rgids[i] & 0x80000000) != 0x0))
        goto L_14ec;
    else
        goto L_1510;

L_14ec:
    psz = PszGetFleetName((LOWORD(rgids[i]) | 0x8000));
    goto L_152e;

L_1510:
    psz = PszGetPlanetName(LOWORD(rgids[i]));

L_152e:
    pszT = szTemp;

L_1536:
    if (((int16_t)(*(psz)) == 0))
        goto L_1573;
    else
        goto L_1545;

L_1545:
    psz = (psz + 1);
    pszT = (pszT + 1);
    *(pszT) = *(psz);
    if (((int16_t)(*(psz)) != 38))
        goto L_1536;
    else
        goto L_1564;

L_1564:
    pszT = (pszT + 1);
    *(pszT) = 38;

L_1570:
    goto L_1536;

L_1573:
    *(pszT) = 0;
    if ((i != iChecked))
        goto L_158f;
    else
        goto L_1589;

L_1589:
    t_merge_1592_0001 = 0x8;
    goto L_1592;

L_158f:
    t_merge_1592_0001 = 0x0;

L_1592:
    AppendMenu(hmenuPopup, t_merge_1592_0001, (i + 15000), szTemp);

L_15aa:
    goto L_1883;

L_15ad:
    if ((rgsz[i] != 0x0))
        goto L_1771;
    else
        goto L_15bf;

L_15bf:
    pszTitle = rgsz[(i + 1)];
    if ((rgids == 0x0))
        goto L_15f1;
    else
        goto L_15da;

L_15da:
    t_merge_15f4_0001 = LOWORD(rgids[(i + 1)]);
    goto L_15f4;

L_15f1:
    t_merge_15f4_0001 = 0x0;

L_15f4:
    fChecked = t_merge_15f4_0001;
    hmenuSub = CreatePopupMenu();
    i = (i + 2);
    goto L_1738;

L_1608:
    if ((rgsz[i] == 0x0))
        goto L_1743;
    else
        goto L_161d;

L_161d:
    if ((rgids != 0x0))
        goto L_1649;
    else
        goto L_1626;

L_1626:
    if ((i != iChecked))
        goto L_1637;
    else
        goto L_1631;

L_1631:
    t_merge_163a_0001 = 0x1;
    goto L_163a;

L_1637:
    t_merge_163a_0001 = 0x0;

L_163a:
    fCheckedCur = t_merge_163a_0001;
    fChecked = (fChecked | fCheckedCur);
    goto L_165e;

L_1649:
    fCheckedCur = LOWORD(rgids[i]);

L_165e:
    if (((int16_t)(*(rgsz[i])) != -1))
        goto L_16a9;
    else
        goto L_1675;

L_1675:
    if (((int16_t)(rgsz[i][1]) != 0))
        goto L_16a9;
    else
        goto L_168d;

L_168d:
    AppendMenu(hmenuSub, 0x800, 0x0, 0x0);
    goto L_1734;

L_16a9:
    pszT = szTemp;
    psz = rgsz[i];

L_16c1:
    if (((int16_t)(*(psz)) == 0))
        goto L_16fe;
    else
        goto L_16d0;

L_16d0:
    psz = (psz + 1);
    pszT = (pszT + 1);
    *(pszT) = *(psz);
    if (((int16_t)(*(psz)) != 38))
        goto L_16c1;
    else
        goto L_16ef;

L_16ef:
    pszT = (pszT + 1);
    *(pszT) = 38;

L_16fb:
    goto L_16c1;

L_16fe:
    *(pszT) = 0;
    if ((fCheckedCur == 0))
        goto L_1719;
    else
        goto L_1713;

L_1713:
    t_merge_171c_0001 = 0x8;
    goto L_171c;

L_1719:
    t_merge_171c_0001 = 0x0;

L_171c:
    AppendMenu(hmenuSub, t_merge_171c_0001, (i + 15000), szTemp);

L_1734:
    i = (i + 1);

L_1738:
    if ((i < cString))
        goto L_1608;
    else
        goto L_1743;

L_1743:
    if ((fChecked == 0))
        goto L_1757;
    else
        goto L_1751;

L_1751:
    t_merge_175a_0001 = 0x8;
    goto L_175a;

L_1757:
    t_merge_175a_0001 = 0x0;

L_175a:
    AppendMenu(hmenuPopup, (t_merge_175a_0001 | 0x10), (UINT_PTR)(hmenuSub), pszTitle);
    goto L_1883;

L_1771:
    if (((int16_t)(*(rgsz[i])) != -1))
        goto L_17bc;
    else
        goto L_1788;

L_1788:
    if (((int16_t)(rgsz[i][1]) != 0))
        goto L_17bc;
    else
        goto L_17a0;

L_17a0:
    AppendMenu(hmenuPopup, 0x800, 0x0, 0x0);
    goto L_1883;

L_17bc:
    pszT = szTemp;
    psz = rgsz[i];

L_17d4:
    if (((int16_t)(*(psz)) == 0))
        goto L_1811;
    else
        goto L_17e3;

L_17e3:
    psz = (psz + 1);
    pszT = (pszT + 1);
    *(pszT) = *(psz);
    if (((int16_t)(*(psz)) != 38))
        goto L_17d4;
    else
        goto L_1802;

L_1802:
    pszT = (pszT + 1);
    *(pszT) = 38;

L_180e:
    goto L_17d4;

L_1811:
    *(pszT) = 0;
    if ((iChecked != -2))
        goto L_1839;
    else
        goto L_1825;

L_1825:
    t_merge_184f_0001 = rgids[i];
    goto L_184f;

L_1839:
    if ((i != iChecked))
        goto L_184b;
    else
        goto L_1844;

L_1844:
    t_merge_184f_0001 = 1;
    goto L_184f;

L_184b:
    t_merge_184f_0001 = 0;

L_184f:
    if ((t_merge_184f_0001 != 0))
        goto L_1865;
    else
        goto L_186b;

L_1865:
    t_merge_186e_0001 = 0x8;
    goto L_186e;

L_186b:
    t_merge_186e_0001 = 0x0;

L_186e:
    AppendMenu(hmenuPopup, t_merge_186e_0001, (i + 15000), szTemp);

L_1883:
    i = (i + 1);

L_1887:
    if ((i < cString))
        goto L_13ad;
    else
        goto L_1892;

L_1892:
    if ((fRightBtn == 0))
        goto L_18a3;
    else
        goto L_189b;

L_189b:
    tpm = 2;
    goto L_18a8;

L_18a3:
    tpm = 0;

L_18a8:
    TrackPopupMenu(hmenuPopup, tpm, pt.x, pt.y, 0, hwndFrame, 0x0);
    DestroyMenu(hmenuPopup);
    if ((hmenuSub == 0x0))
        goto L_18e7;
    else
        goto L_18de;

L_18de:
    DestroyMenu(hmenuSub);

L_18e7:
    if ((PeekMessage(&(msg), hwndFrame, 0x111, 0x111, 0x2) == 0))
        goto L_192c;
    else
        goto L_190c;

L_190c:
    if ((msg.wParam < 0x3a98))
        goto L_192c;
    else
        goto L_1917;

L_1917:
    if ((msg.wParam >= 0x3afc))
        goto L_192c;
    else
        goto L_1922;

L_1922:
    iPopMenuSel = (msg.wParam - 15000);

L_192c:

L_1932:
    return iPopMenuSel;
}
