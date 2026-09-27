void CreateChildWindows() {
    char  szData[100];
    POINT pt;
    char *psz;
    char  szGame[15];

L_038c:
    if ((idPlayer == -1))
        goto L_0480;
    else
        goto L_039f;

L_039f:
    psz = &(szBase[(strlen(szBase) - 1)]);

L_03b8:
    if ((psz <= szBase))
        goto L_03e7;
    else
        goto L_03c2;

L_03c2:
    if (((int16_t)(psz[(-1)]) == 92))
        goto L_03e7;
    else
        goto L_03d1;

L_03d1:
    if (((int16_t)(psz[(-1)]) == 58))
        goto L_03e7;
    else
        goto L_03e0;

L_03e0:
    psz = (psz - 1);
    goto L_03b8;

L_03e7:
    szGame[8] = 0;
    strncpy(szGame, psz, 0x8);
    strlwr(szGame);
    _wsprintf(&(szGame[strlen(szGame)]), ".m%d", (idPlayer + 1));
    _wsprintf(szData, "Stars! -- %s -- %s -- %s", game.szName, PszPlayerName(idPlayer, 0, 1, 0, 0, 0x0), szGame);
    goto L_04ad;

L_0480:
    CchGetString(idsStarsSHostMode, szWork);
    _wsprintf(szData, szWork, game.szName);

L_04ad:
    SetWindowText(hwndFrame, szData);
    if ((idPlayer == -1))
        goto L_06cf;
    else
        goto L_04ca;

L_04ca:
    if ((hwndScanner != 0x0))
        goto L_051a;
    else
        goto L_04d4;

L_04d4:
    hwndScanner = CreateWindow(szScan, 0x0, WS_CHILD | WS_VISIBLE, -200, -200, 10, 10, hwndFrame, 0x0, hInst, 0x0);
    goto L_0547;

L_051a:
    InvalidateRect(hwndScanner, 0x0, 1);
    yScanTop = 1000;
    xScanTop = 1000;
    SetScanScrollBars(hwndScanner);

L_0547:
    if ((hwndMine != 0x0))
        goto L_0595;
    else
        goto L_0551;

L_0551:
    hwndMine = CreateWindow(szMine, 0x0, WS_CHILD | WS_VISIBLE, -500, -500, pt.x, pt.y, hwndFrame, 0x0, hInst, 0x0);
    goto L_05aa;

L_0595:
    InvalidateRect(hwndMine, 0x0, 1);

L_05aa:
    if ((hwndPlanet != 0x0))
        goto L_05fa;
    else
        goto L_05b4;

L_05b4:
    hwndPlanet = CreateWindow(szPlanet, 0x0, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, 0x0, hInst, 0x0);
    goto L_060f;

L_05fa:
    InvalidateRect(hwndPlanet, 0x0, 1);

L_060f:
    if ((hwndTb != 0x0))
        goto L_065f;
    else
        goto L_0619;

L_0619:
    hwndTb = CreateWindow(szTb, 0x0, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, 0x0, hInst, 0x0);
    goto L_0674;

L_065f:
    InvalidateRect(hwndTb, 0x0, 1);

L_0674:
    if ((hwndMessage == 0x0))
        goto L_0687;
    else
        goto L_067e;

L_067e:
    DestroyWindow(hwndMessage);

L_0687:
    hwndMessage = CreateWindow(szMessage, 0x0, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, 0x0, hInst, 0x0);
    RefitFrameChildren();

L_06cf:
    return;
}
