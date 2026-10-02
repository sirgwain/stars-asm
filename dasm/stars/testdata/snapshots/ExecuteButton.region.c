void ExecuteButton(ToolbarButton itb, int16_t fDown) {
    uint16_t grbitNew;
    POINT16  pt;
    char    *rgszScan[12];
    int16_t  c;
    int16_t  i;
    uint16_t grbit;
    int32_t  rgid[12];
    int16_t  iSel;
    uint16_t grbitSh;
    int16_t  ish;

    gd.fChgScanner = 1;
    if ((uint16_t)itb <= tbShipCounts) {
        switch (itb) {
        case tbNormalView:
        case tbSurfaceMineralView:
        case tbMineralConcView:
        case tbPlanetValueView:
        case tbPopulationView:
        case tbNoPlayerInfoView:
            if (fDown == 0)
                break;
            grbitScan = itb + (grbitScan & grbitScanToggleMask);
            goto L_1644;
        case tbAddWaypoints:
            grbitNew = 16;
            goto LBitDiddle;
        case tbScannerCoverage:
            grbitNew = 32;
            goto LBitDiddle;
        case tbFleetPaths:
            grbitNew = 128;
            goto LBitDiddle;
        case tbIdleFleets:
            grbitNew = 0x100;
            goto LBitDiddle;
        case tbPlanetNames:
            grbitNew = 0x400;
            goto LBitDiddle;
        case tbShipCounts:
            grbitNew = 0x1000;
            goto LBitDiddle;
        case tbShipDesignFilter:
            grbitNew = 0x200;
            goto LBitDiddle;
        case tbEnemyClassFilter:
            grbitNew = 0x800;
            goto LBitDiddle;
        case tbMineFields:
            grbit = 1;
            c = 0;
            if ((grbitScan & grbitScanMineFields) == 0) {
                grbitScanMines = 0;
            }
            for (i = 1278; i <= 1279; i++) {
                if (i == 1278) {
                    rgid[c] = (uint32_t)(grbitScanMines == 15 ? 1 : 0);
                } else {
                    rgid[c] = (uint32_t)(grbitScanMines == 0 ? 1 : 0);
                }
                CchGetString(i, &szWork[(i - 1278) * 30 + 160]);
                rgszScan[c++] = &szWork[(i - 1278) * 30 + 160];
            }
            rgid[c] = 0;
            szWork[250] = -1;
            szWork[251] = 0;
            rgszScan[c++] = &szWork[250];
            for (i = 0; i < 4; i++) {
                rgid[c] = (uint32_t)((1 << i & grbitScanMines) == 0 ? 0 : 1);
                CchGetString(i + 1280, &szWork[i * 30]);
                rgszScan[c++] = &szWork[i * 30];
            }
            GetCursorPos16(&pt);
            ScreenToClient16(hwndTb, &pt);
            iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, 0);
            if (iSel == -1)
                break;
            if (iSel >= 3) {
                iSel -= 3;
                grbitScanMines ^= 1 << iSel;
            } else if (iSel == 0) {
                grbitScanMines = 15;
            } else {
                grbitScanMines = 0;
            }
            if (grbitScanMines != 0) {
                grbitScan |= grbitScanMineFields;
            } else {
                grbitScan &= 0xffbf;
            }
            InvalidateRect(hwndTb, NULL, 1);
            goto L_1644;
        case tbShipDesignFilterMenu:
            c = 0;
            for (i = 1275; i <= 1277; i++) {
                rgid[c] = 0;
                CchGetString(i, &szWork[(i - 1275) * 20]);
                rgszScan[c++] = &szWork[(i - 1275) * 20];
            }
            rgid[c] = 0;
            szWork[200] = -1;
            szWork[201] = 0;
            rgszScan[c++] = &szWork[200];
            ish = 0;
            grbitSh = 1;
            while (ish < 16) {
                if (rgshdef[ish].fFree == 0) {
                    rgid[c] = (uint32_t)((grbitSh & grbitScanShip) == 0 ? 0 : 1);
                    rgszScan[c++] = rgshdef[ish].hul.szClass;
                }
                ish++;
                grbitSh *= 2;
            }
            GetCursorPos16(&pt);
            ScreenToClient16(hwndTb, &pt);
            iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, 0);
            if (iSel == -1)
                break;
            if (iSel < 4) {
                if (iSel == 0) {
                    grbitScanShip = 0xffff;
                } else if (iSel == 1) {
                    grbitScanShip ^= 0xffff;
                } else {
                    grbitScanShip = 0;
                }
                if ((grbitScan & grbitScanDesignFilter) != 0 || grbitScanShip == 0)
                    goto L_12e6;
            } else {
                iSel -= 4;
                for (ish = 0; ish < 16; ish++) {
                    if (rgshdef[ish].fFree == 0) {
                        iSel--;
                        if (iSel < 0)
                            break;
                    }
                }
                grbitScanShip ^= 1 << ish;
                if ((grbitScan & grbitScanDesignFilter) != 0 || (1 << ish & grbitScanShip) == 0)
                    goto L_12e6;
            }
            grbitScan |= grbitScanDesignFilter;
            InvalidateRect(hwndTb, NULL, 1);
        L_12e6:
            if ((grbitScan & grbitScanDesignFilter) == 0)
                break;
            goto L_1644;
        case tbEnemyClassFilterMenu:
            grbit = 1;
            c = 0;
            for (i = 1275; i <= 1277; i++) {
                rgid[c] = 0;
                CchGetString(i, &szWork[(i - 1275) * 25 + 200]);
                rgszScan[c++] = &szWork[(i - 1275) * 25 + 200];
            }
            rgid[c] = 0;
            szWork[300] = -1;
            szWork[301] = 0;
            rgszScan[c++] = &szWork[300];
            for (i = 0; i < 8; i++) {
                rgid[c] = (uint32_t)((1 << i & grbitScanEShip) == 0 ? 0 : 1);
                CchGetString(i + 381, &szWork[i * 25]);
                rgszScan[c++] = &szWork[i * 25];
            }
            GetCursorPos16(&pt);
            ScreenToClient16(hwndTb, &pt);
            iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, 0);
            if (iSel == -1)
                break;
            if (iSel < 4) {
                if (iSel == 0) {
                    grbitScanEShip = 0xff;
                } else if (iSel == 1) {
                    grbitScanEShip ^= 0xff;
                } else {
                    grbitScanEShip = 0;
                }
                if ((grbitScan & grbitScanEnemyFilter) != 0 || grbitScanEShip == 0)
                    goto L_1505;
            } else {
                iSel -= 4;
                grbitScanEShip ^= 1 << iSel;
                if ((grbitScan & grbitScanEnemyFilter) != 0 || (1 << iSel & grbitScanEShip) == 0)
                    goto L_1505;
            }
            grbitScan |= grbitScanEnemyFilter;
            InvalidateRect(hwndTb, NULL, 1);
        L_1505:
            if ((grbitScan & grbitScanEnemyFilter) == 0)
                break;
            goto L_1644;
        case tbZoomMenu:
            c = 0;
            for (i = 0; i < 9; i++) {
                rgid[c] = (uint32_t)(iScanZoom + 4 == i ? 1 : 0);
                _wsprintf(&szWork[i * 8], PCTDPCTPCT, vrgpctZoom[i]);
                rgszScan[c++] = &szWork[i * 8];
            }
            GetCursorPos16(&pt);
            ScreenToClient16(hwndTb, &pt);
            iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, 0);
            if (iSel != -1) {
                CommandHandler(hwndFrame, iSel + 3901);
            }
        }
        return;
    LBitDiddle:
        if (fDown != 0) {
            grbitScan |= grbitNew;
        } else {
            grbitScan &= ~grbitNew;
        }
    L_1644:
        if (itb != tbAddWaypoints) {
            InvalidateRect(hwndScanner, NULL, 1);
        }
        if (gd.fTutorial != 0) {
            AdvanceTutor();
        }
    }
    return;
}
