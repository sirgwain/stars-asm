void CreateChildWindows() {
    char    szData[100];
    POINT16 pt;
    char   *psz;
    char    szGame[15];

    if (idPlayer == -1) {
        CchGetString(idsStarsSHostMode, szWork);
        _wsprintf(szData, szWork, game.szName);
    } else {
        for (psz = &szBase[strlen(szBase) - 1]; psz > szBase && (int16_t)psz[-1] != '\\' && (int16_t)psz[-1] != ':'; psz--) {
        }
        szGame[8] = 0;
        strncpy(szGame, psz, 0x8);
        strlwr(szGame);
        _wsprintf(&szGame[strlen(szGame)], ".m%d", idPlayer + 1);
        _wsprintf(szData, "Stars! -- %s -- %s -- %s", game.szName, PszPlayerName(idPlayer, 0, 1, 0, 0, 0x0), szGame);
    }
    SetWindowText(hwndFrame, szData);
    if (idPlayer != -1) {
        if (hwndScanner != 0x0) {
            InvalidateRect(hwndScanner, 0x0, 1);
            yScanTop = 1000;
            xScanTop = 1000;
            SetScanScrollBars(hwndScanner);
        } else {
            hwndScanner = CreateWindow(szScan, 0x0, WS_CHILD | WS_VISIBLE, -200, -200, 10, 10, hwndFrame, 0x0, hInst, 0x0);
        }
        if (hwndMine != 0x0) {
            InvalidateRect(hwndMine, 0x0, 1);
        } else {
            hwndMine = CreateWindow(szMine, 0x0, WS_CHILD | WS_VISIBLE, -500, -500, pt.x, pt.y, hwndFrame, 0x0, hInst, 0x0);
        }
        if (hwndPlanet != 0x0) {
            InvalidateRect(hwndPlanet, 0x0, 1);
        } else {
            hwndPlanet = CreateWindow(szPlanet, 0x0, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, 0x0, hInst, 0x0);
        }
        if (hwndTb != 0x0) {
            InvalidateRect(hwndTb, 0x0, 1);
        } else {
            hwndTb = CreateWindow(szTb, 0x0, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, 0x0, hInst, 0x0);
        }
        if (hwndMessage != 0x0) {
            DestroyWindow(hwndMessage);
        }
        hwndMessage = CreateWindow(szMessage, 0x0, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, 0x0, hInst, 0x0);
        RefitFrameChildren();
    }
    return;
}
