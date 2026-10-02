void CreateChildWindows() {
    char    szData[100];
    POINT16 pt;
    char   *psz;
    char    szGame[15];

L_038c:
    if (idPlayer == -1)
        goto L_0480;
    else
        goto L_039f;

L_039f:
    psz = &szBase[strlen(szBase) - 1];

L_03b8:
    if (psz <= szBase)
        goto L_03e7;
    else
        goto L_03c2;

L_03c2:
    if ((int16_t)(int8_t)psz[-1] == '\\')
        goto L_03e7;
    else
        goto L_03d1;

L_03d1:
    if ((int16_t)(int8_t)psz[-1] == ':')
        goto L_03e7;
    else
        goto L_03e0;

L_03e0:
    psz--;
    goto L_03b8;

L_03e7:
    szGame[8] = 0;
    strncpy(szGame, psz, 8);
    strlwr(szGame);
    _wsprintf(&szGame[strlen(szGame)], ".m%d", idPlayer + 1);
    _wsprintf(szData, "Stars! -- %s -- %s -- %s", game.szName, PszPlayerName(idPlayer, 0, 1, 0, 0, NULL), szGame);
    goto L_04ad;

L_0480:
    CchGetString(idsStarsSHostMode, szWork);
    _wsprintf(szData, szWork, game.szName);

L_04ad:
    SetWindowText(hwndFrame, szData);
    if (idPlayer == -1)
        goto L_06cf;
    else
        goto L_04ca;

L_04ca:
    if (hwndScanner != 0)
        goto L_051a;
    else
        goto L_04d4;

L_04d4:
    hwndScanner = CreateWindow(szScan, NULL, WS_CHILD | WS_VISIBLE, -200, -200, 10, 10, hwndFrame, NULL, hInst, NULL);
    goto L_0547;

L_051a:
    InvalidateRect(hwndScanner, NULL, 1);
    yScanTop = 1000;
    xScanTop = 1000;
    SetScanScrollBars(hwndScanner);

L_0547:
    if (hwndMine != 0)
        goto L_0595;
    else
        goto L_0551;

L_0551:
    hwndMine = CreateWindow(szMine, NULL, WS_CHILD | WS_VISIBLE, -500, -500, pt.x, pt.y, hwndFrame, NULL, hInst, NULL);
    goto L_05aa;

L_0595:
    InvalidateRect(hwndMine, NULL, 1);

L_05aa:
    if (hwndPlanet != 0)
        goto L_05fa;
    else
        goto L_05b4;

L_05b4:
    hwndPlanet = CreateWindow(szPlanet, NULL, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, NULL, hInst, NULL);
    goto L_060f;

L_05fa:
    InvalidateRect(hwndPlanet, NULL, 1);

L_060f:
    if (hwndTb != 0)
        goto L_065f;
    else
        goto L_0619;

L_0619:
    hwndTb = CreateWindow(szTb, NULL, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, NULL, hInst, NULL);
    goto L_0674;

L_065f:
    InvalidateRect(hwndTb, NULL, 1);

L_0674:
    if (hwndMessage == 0)
        goto L_0687;
    else
        goto L_067e;

L_067e:
    DestroyWindow(hwndMessage);

L_0687:
    hwndMessage = CreateWindow(szMessage, NULL, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, NULL, hInst, NULL);
    RefitFrameChildren();

L_06cf:
    return;
}
