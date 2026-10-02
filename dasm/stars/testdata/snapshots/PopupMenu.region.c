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

    hmenuSub = 0;
    pt.x = x;
    pt.y = y;
    ClientToScreen16(hwnd, &pt);
    hmenuPopup = CreatePopupMenu();
    iPopMenuSel = -1;
    for (i = 0; i < cString; i++) {
        if (rgids != 0 && (iChecked != -2 || rgsz == 0)) {
            if (rgids[i] == -1) {
                AppendMenu(hmenuPopup, MF_SEPARATOR, 0, NULL);
            } else {
                if ((rgids[i] & 0x10000000) != 0) {
                    psz = "Deep Space";
                } else if ((rgids[i] & 0x40000000) != 0) {
                    psz = PszGetCompressedString(LOWORD(rgids[i]));
                } else if ((rgids[i] & 0x20000000) != 0) {
                    psz = PszGetThingName(LOWORD(rgids[i]));
                } else if ((rgids[i] & 0x80000000) != 0) {
                    psz = PszGetFleetName(LOWORD(rgids[i]) | 0x8000);
                } else {
                    psz = PszGetPlanetName(LOWORD(rgids[i]));
                }
                pszT = szTemp;
                while (*psz != 0) {
                    t_1545 = psz;
                    psz++;
                    *pszT++ = *t_1545;
                    if (*t_1545 == '&') {
                        *pszT++ = '&';
                    }
                }
                *pszT = 0;
                AppendMenu(hmenuPopup, i == iChecked ? MF_CHECKED : MF_BYCOMMAND, i + 15000, szTemp);
            }
        } else if (rgsz[i] == 0) {
            pszTitle = rgsz[i + 1];
            fChecked = rgids == 0 ? 0 : LOWORD(rgids[i + 1]);
            hmenuSub = CreatePopupMenu();
            for (i += 2; i < cString && rgsz[i] != 0; i++) {
                if (rgids == 0) {
                    fCheckedCur = i == iChecked;
                    fChecked |= fCheckedCur;
                } else {
                    fCheckedCur = LOWORD(rgids[i]);
                }
                if (*rgsz[i] == -1 && rgsz[i][1] == 0) {
                    AppendMenu(hmenuSub, MF_SEPARATOR, 0, NULL);
                } else {
                    pszT = szTemp;
                    psz = rgsz[i];
                    while (*psz != 0) {
                        t_16d0 = psz;
                        psz++;
                        *pszT++ = *t_16d0;
                        if (*t_16d0 == '&') {
                            *pszT++ = '&';
                        }
                    }
                    *pszT = 0;
                    AppendMenu(hmenuSub, fCheckedCur == 0 ? MF_BYCOMMAND : MF_CHECKED, i + 15000, szTemp);
                }
            }
            AppendMenu(hmenuPopup, (fChecked == 0 ? 0 : 8) | 0x10, (UINT_PTR)hmenuSub, pszTitle);
        } else if (*rgsz[i] == -1 && rgsz[i][1] == 0) {
            AppendMenu(hmenuPopup, MF_SEPARATOR, 0, NULL);
        } else {
            pszT = szTemp;
            psz = rgsz[i];
            while (*psz != 0) {
                t_17e3 = psz;
                psz++;
                *pszT++ = *t_17e3;
                if (*t_17e3 == '&') {
                    *pszT++ = '&';
                }
            }
            *pszT = 0;
            t_merge_184f_0001 = iChecked == -2 ? rgids[i] : i == iChecked;
            AppendMenu(hmenuPopup, t_merge_184f_0001 == 0 ? MF_BYCOMMAND : MF_CHECKED, i + 15000, szTemp);
        }
    }
    if (fRightBtn != 0) {
        tpm = TPM_RIGHTBUTTON;
    } else {
        tpm = TPM_LEFTBUTTON;
    }
    TrackPopupMenu(hmenuPopup, tpm, pt.x, pt.y, 0, hwndFrame, NULL);
    DestroyMenu(hmenuPopup);
    if (hmenuSub != 0) {
        DestroyMenu(hmenuSub);
    }
    if (PeekMessage(&msg, hwndFrame, 273, 273, 2) != 0 && msg.wParam >= 15000 && msg.wParam < 15100) {
        iPopMenuSel = msg.wParam - 15000;
    }
    return iPopMenuSel;
}
