int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn) {
    char    *pszTitle;
    int16_t  tpm;
    POINT16  pt;
    int16_t  i;
    char     szTemp[128];
    HMENU    hmenuSub;
    HMENU    hmenuPopup;
    char    *pszT;
    char    *psz;
    MSG      msg;
    int16_t  fChecked;
    int16_t  fCheckedCur;
    POINT    t_pt_1391_1;
    char    *t_1545;
    char    *t_1550;
    char    *t_1564;
    char    *t_16d0;
    char    *t_16db;
    char    *t_16ef;
    char    *t_17e3;
    char    *t_17ee;
    char    *t_1802;
    int32_t  t_merge_184f_0001;
    uint16_t t_merge_186e_0001;

    hmenuSub = 0x0;
    pt.x = x;
    pt.y = y;
    t_pt_1391_1 = PointFrom16(pt);
    ClientToScreen(hwnd, &t_pt_1391_1);
    pt = PointTo16(t_pt_1391_1);
    hmenuPopup = CreatePopupMenu();
    iPopMenuSel = -1;
    for (i = 0; i < cString; i++) {
        if (rgids == 0x0 || (iChecked == -2 && rgsz != 0x0)) {
            if (rgsz[i] != 0x0) {
                if ((int16_t)*rgsz[i] != -1 || (int16_t)rgsz[i][1] != 0) {
                    pszT = szTemp;
                    psz = rgsz[i];
                    while ((int16_t)*psz != 0) {
                        t_17e3 = psz;
                        psz = psz + 1;
                        t_17ee = pszT;
                        pszT = pszT + 1;
                        *t_17ee = *t_17e3;
                        if ((int16_t)*t_17e3 == '&') {
                            t_1802 = pszT;
                            pszT = pszT + 1;
                            *t_1802 = '&';
                        }
                    }
                    *pszT = 0;
                    if (iChecked != -2) {
                        if (i != iChecked) {
                            t_merge_184f_0001 = 0;
                        } else {
                            t_merge_184f_0001 = 1;
                        }
                    } else {
                        t_merge_184f_0001 = rgids[i];
                    }
                    t_merge_186e_0001 = t_merge_184f_0001 == 0 ? 0x0 : 0x8;
                    AppendMenu(hmenuPopup, t_merge_186e_0001, i + 15000, szTemp);
                } else {
                    AppendMenu(hmenuPopup, 0x800, 0x0, 0x0);
                }
            } else {
                pszTitle = rgsz[i + 1];
                fChecked = rgids == 0x0 ? 0 : LOWORD(rgids[i + 1]);
                hmenuSub = CreatePopupMenu();
                for (i = i + 2; i < cString && rgsz[i] != 0x0; i++) {
                    if (rgids != 0x0) {
                        fCheckedCur = LOWORD(rgids[i]);
                    } else {
                        fCheckedCur = i == iChecked ? 1 : 0;
                        fChecked = fChecked | fCheckedCur;
                    }
                    if ((int16_t)*rgsz[i] != -1 || (int16_t)rgsz[i][1] != 0) {
                        pszT = szTemp;
                        psz = rgsz[i];
                        while ((int16_t)*psz != 0) {
                            t_16d0 = psz;
                            psz = psz + 1;
                            t_16db = pszT;
                            pszT = pszT + 1;
                            *t_16db = *t_16d0;
                            if ((int16_t)*t_16d0 == '&') {
                                t_16ef = pszT;
                                pszT = pszT + 1;
                                *t_16ef = '&';
                            }
                        }
                        *pszT = 0;
                        AppendMenu(hmenuSub, fCheckedCur == 0 ? 0x0 : 0x8, i + 15000, szTemp);
                    } else {
                        AppendMenu(hmenuSub, 0x800, 0x0, 0x0);
                    }
                }
                AppendMenu(hmenuPopup, (fChecked == 0 ? 0x0 : 0x8) | 0x10, (UINT_PTR)hmenuSub, pszTitle);
            }
        } else if (rgids[i] != -1) {
            if ((rgids[i] & 0x10000000) != 0x0) {
                psz = "Deep Space";
            } else if ((rgids[i] & 0x40000000) != 0x0) {
                psz = PszGetCompressedString(LOWORD(rgids[i]));
            } else if ((rgids[i] & 0x20000000) != 0x0) {
                psz = PszGetThingName(LOWORD(rgids[i]));
            } else if ((rgids[i] & 0x80000000) != 0x0) {
                psz = PszGetFleetName(LOWORD(rgids[i]) | 0x8000);
            } else {
                psz = PszGetPlanetName(LOWORD(rgids[i]));
            }
            pszT = szTemp;
            while ((int16_t)*psz != 0) {
                t_1545 = psz;
                psz = psz + 1;
                t_1550 = pszT;
                pszT = pszT + 1;
                *t_1550 = *t_1545;
                if ((int16_t)*t_1545 == '&') {
                    t_1564 = pszT;
                    pszT = pszT + 1;
                    *t_1564 = '&';
                }
            }
            *pszT = 0;
            AppendMenu(hmenuPopup, i == iChecked ? 0x8 : 0x0, i + 15000, szTemp);
        } else {
            AppendMenu(hmenuPopup, 0x800, 0x0, 0x0);
        }
    }
    if (fRightBtn == 0) {
        tpm = 0;
    } else {
        tpm = 2;
    }
    TrackPopupMenu(hmenuPopup, tpm, pt.x, pt.y, 0, hwndFrame, 0x0);
    DestroyMenu(hmenuPopup);
    if (hmenuSub != 0x0) {
        DestroyMenu(hmenuSub);
    }
    if (PeekMessage(&msg, hwndFrame, 0x111, 0x111, 0x2) != 0 && msg.wParam >= 0x3a98 && msg.wParam < 0x3afc) {
        iPopMenuSel = msg.wParam - 15000;
    }
    return iPopMenuSel;
}
