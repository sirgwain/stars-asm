void CreateChildWindows() {
    char    szData[100];
    POINT16 pt;
    char   *psz;
    char    szGame[15];

    if (idPlayer != -1) {
        for (psz = &szBase[strlen(szBase) - 1]; psz > szBase && (int16_t)(int8_t)psz[-1] != '\\' && (int16_t)(int8_t)psz[-1] != ':'; psz--) {
        }
        szGame[8] = 0;
        strncpy(szGame, psz, 8);
        strlwr(szGame);
        _wsprintf(&szGame[strlen(szGame)], ".m%d", idPlayer + 1);
        _wsprintf(szData, "Stars! -- %s -- %s -- %s", game.szName, PszPlayerName(idPlayer, 0, 1, 0, 0, NULL), szGame);
    } else {
        CchGetString(idsStarsSHostMode, szWork);
        _wsprintf(szData, szWork, game.szName);
    }
    SetWindowText(hwndFrame, szData);
    if (idPlayer != -1) {
        if (hwndScanner == 0) {
            hwndScanner = CreateWindow(szScan, NULL, WS_CHILD | WS_VISIBLE, -200, -200, 10, 10, hwndFrame, NULL, hInst, NULL);
        } else {
            InvalidateRect(hwndScanner, NULL, 1);
            yScanTop = 1000;
            xScanTop = 1000;
            SetScanScrollBars(hwndScanner);
        }
        if (hwndMine == 0) {
            hwndMine = CreateWindow(szMine, NULL, WS_CHILD | WS_VISIBLE, -500, -500, pt.x, pt.y, hwndFrame, NULL, hInst, NULL);
        } else {
            InvalidateRect(hwndMine, NULL, 1);
        }
        if (hwndPlanet == 0) {
            hwndPlanet = CreateWindow(szPlanet, NULL, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, NULL, hInst, NULL);
        } else {
            InvalidateRect(hwndPlanet, NULL, 1);
        }
        if (hwndTb == 0) {
            hwndTb = CreateWindow(szTb, NULL, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, NULL, hInst, NULL);
        } else {
            InvalidateRect(hwndTb, NULL, 1);
        }
        if (hwndMessage != 0) {
            DestroyWindow(hwndMessage);
        }
        hwndMessage = CreateWindow(szMessage, NULL, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, NULL, hInst, NULL);
        RefitFrameChildren();
    }
    return;
}
