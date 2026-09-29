void ExecuteButton(int16_t itb, int16_t fDown) {
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
    int16_t  t_0ef9;
    int16_t  t_0f36;
    int16_t  t_0fa6;
    POINT    t_pt_0fca;
    POINT    t_pt_0fda_1;
    int16_t  t_10c5;
    int16_t  t_1103;
    int16_t  t_1198;
    POINT    t_pt_11b2;
    POINT    t_pt_11c2_1;
    int16_t  t_134d;
    int16_t  t_138a;
    int16_t  t_13fa;
    POINT    t_pt_141e;
    POINT    t_pt_142e_1;
    int16_t  t_1589;
    POINT    t_pt_15ad;
    POINT    t_pt_15bd_1;

    gd.fChgScanner = 0x1;
    if ((uint16_t)itb <= 17) {
        switch (itb) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            if (fDown == 0)
                break;
            grbitScan = itb + (grbitScan & 0x3ff0);
            goto L_1644;
        case 6:
            grbitNew = 0x10;
            goto LBitDiddle;
        case 7:
            grbitNew = 0x20;
            goto LBitDiddle;
        case 9:
            grbitNew = 0x80;
            goto LBitDiddle;
        case 10:
            grbitNew = 0x100;
            goto LBitDiddle;
        case 11:
            grbitNew = 0x400;
            goto LBitDiddle;
        case 17:
            grbitNew = 0x1000;
            goto LBitDiddle;
        case 12:
            grbitNew = 0x200;
            goto LBitDiddle;
        case 14:
            grbitNew = 0x800;
            goto LBitDiddle;
        case 8:
            grbit = 0x1;
            c = 0;
            if ((grbitScan & 0x40) == 0x0) {
                grbitScanMines = 0x0;
            }
            for (i = 1278; i <= 1279; i++) {
                if (i != 1278) {
                    rgid[c] = (uint32_t)(grbitScanMines == 0x0 ? 0x1 : 0x0);
                } else {
                    rgid[c] = (uint32_t)(grbitScanMines == 0xf ? 0x1 : 0x0);
                }
                CchGetString(i, &szWork[(i - 1278) * 30 + 160]);
                t_0ef9 = c;
                c = c + 1;
                rgszScan[t_0ef9] = &szWork[(i - 1278) * 30 + 160];
            }
            rgid[c] = 0;
            szWork[250] = -1;
            szWork[251] = 0;
            t_0f36 = c;
            c = c + 1;
            rgszScan[t_0f36] = &szWork[250];
            for (i = 0; i < 4; i++) {
                rgid[c] = (uint32_t)((0x1 << i & grbitScanMines) == 0x0 ? 0x0 : 0x1);
                CchGetString(i + 1280, &szWork[i * 30]);
                t_0fa6 = c;
                c = c + 1;
                rgszScan[t_0fa6] = &szWork[i * 30];
            }
            GetCursorPos(&t_pt_0fca);
            pt = PointTo16(t_pt_0fca);
            t_pt_0fda_1 = PointFrom16(pt);
            ScreenToClient(hwndTb, &t_pt_0fda_1);
            pt = PointTo16(t_pt_0fda_1);
            iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, 0);
            if (iSel == -1)
                break;
            if (iSel >= 3) {
                iSel = iSel - 3;
                grbitScanMines = grbitScanMines ^ 0x1 << iSel;
            } else if (iSel != 0) {
                grbitScanMines = 0x0;
            } else {
                grbitScanMines = 0xf;
            }
            if (grbitScanMines == 0x0) {
                grbitScan = grbitScan & 0xffbf;
            } else {
                grbitScan = grbitScan | 0x40;
            }
            InvalidateRect(hwndTb, 0x0, 1);
            goto L_1644;
        case 13:
            c = 0;
            for (i = 1275; i <= 1277; i++) {
                rgid[c] = 0;
                CchGetString(i, &szWork[(i - 1275) * 20]);
                t_10c5 = c;
                c = c + 1;
                rgszScan[t_10c5] = &szWork[(i - 1275) * 20];
            }
            rgid[c] = 0;
            szWork[200] = -1;
            szWork[201] = 0;
            t_1103 = c;
            c = c + 1;
            rgszScan[t_1103] = &szWork[200];
            ish = 0;
            grbitSh = 0x1;
            while (ish < 16) {
                if (rgshdef[ish].fFree == 0x0) {
                    rgid[c] = (uint32_t)((grbitSh & grbitScanShip) == 0x0 ? 0x0 : 0x1);
                    t_1198 = c;
                    c = c + 1;
                    rgszScan[t_1198] = rgshdef[ish].hul.szClass;
                }
                ish = ish + 1;
                grbitSh = grbitSh * 0x2;
            }
            GetCursorPos(&t_pt_11b2);
            pt = PointTo16(t_pt_11b2);
            t_pt_11c2_1 = PointFrom16(pt);
            ScreenToClient(hwndTb, &t_pt_11c2_1);
            pt = PointTo16(t_pt_11c2_1);
            iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, 0);
            if (iSel == -1)
                break;
            if (iSel >= 4) {
                iSel = iSel - 4;
                for (ish = 0; ish < 16; ish++) {
                    if (rgshdef[ish].fFree == 0x0) {
                        iSel = iSel - 1;
                        if (iSel < 0)
                            break;
                    }
                }
                grbitScanShip = grbitScanShip ^ 0x1 << ish;
                if ((grbitScan & 0x200) != 0x0 || (0x1 << ish & grbitScanShip) == 0x0)
                    goto L_12e6;
            } else {
                if (iSel != 0) {
                    if (iSel != 1) {
                        grbitScanShip = 0x0;
                    } else {
                        grbitScanShip = grbitScanShip ^ 0xffff;
                    }
                } else {
                    grbitScanShip = 0xffff;
                }
                if ((grbitScan & 0x200) != 0x0 || grbitScanShip == 0x0)
                    goto L_12e6;
            }
            grbitScan = grbitScan | 0x200;
            InvalidateRect(hwndTb, 0x0, 1);
        L_12e6:
            if ((grbitScan & 0x200) == 0x0)
                break;
            goto L_1644;
        case 15:
            grbit = 0x1;
            c = 0;
            for (i = 1275; i <= 1277; i++) {
                rgid[c] = 0;
                CchGetString(i, &szWork[(i - 1275) * 25 + 200]);
                t_134d = c;
                c = c + 1;
                rgszScan[t_134d] = &szWork[(i - 1275) * 25 + 200];
            }
            rgid[c] = 0;
            szWork[300] = -1;
            szWork[301] = 0;
            t_138a = c;
            c = c + 1;
            rgszScan[t_138a] = &szWork[300];
            for (i = 0; i < 8; i++) {
                rgid[c] = (uint32_t)((0x1 << i & grbitScanEShip) == 0x0 ? 0x0 : 0x1);
                CchGetString(i + 381, &szWork[i * 25]);
                t_13fa = c;
                c = c + 1;
                rgszScan[t_13fa] = &szWork[i * 25];
            }
            GetCursorPos(&t_pt_141e);
            pt = PointTo16(t_pt_141e);
            t_pt_142e_1 = PointFrom16(pt);
            ScreenToClient(hwndTb, &t_pt_142e_1);
            pt = PointTo16(t_pt_142e_1);
            iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, 0);
            if (iSel == -1)
                break;
            if (iSel >= 4) {
                iSel = iSel - 4;
                grbitScanEShip = grbitScanEShip ^ 0x1 << iSel;
                if ((grbitScan & 0x800) != 0x0 || (0x1 << iSel & grbitScanEShip) == 0x0)
                    goto L_1505;
            } else {
                if (iSel != 0) {
                    if (iSel != 1) {
                        grbitScanEShip = 0x0;
                    } else {
                        grbitScanEShip = grbitScanEShip ^ 0xff;
                    }
                } else {
                    grbitScanEShip = 0xff;
                }
                if ((grbitScan & 0x800) != 0x0 || grbitScanEShip == 0x0)
                    goto L_1505;
            }
            grbitScan = grbitScan | 0x800;
            InvalidateRect(hwndTb, 0x0, 1);
        L_1505:
            if ((grbitScan & 0x800) == 0x0)
                break;
            goto L_1644;
        case 16:
            c = 0;
            for (i = 0; i < 9; i++) {
                rgid[c] = (uint32_t)(iScanZoom + 4 == i ? 0x1 : 0x0);
                _wsprintf(&szWork[i * 8], PCTDPCTPCT, vrgpctZoom[i]);
                t_1589 = c;
                c = c + 1;
                rgszScan[t_1589] = &szWork[i * 8];
            }
            GetCursorPos(&t_pt_15ad);
            pt = PointTo16(t_pt_15ad);
            t_pt_15bd_1 = PointFrom16(pt);
            ScreenToClient(hwndTb, &t_pt_15bd_1);
            pt = PointTo16(t_pt_15bd_1);
            iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, 0);
            if (iSel != -1) {
                CommandHandler(hwndFrame, iSel + 3901);
            }
        }
        return;
    LBitDiddle:
        if (fDown == 0) {
            grbitScan = grbitScan & ~grbitNew;
        } else {
            grbitScan = grbitScan | grbitNew;
        }
    L_1644:
        if (itb != 6) {
            InvalidateRect(hwndScanner, 0x0, 1);
        }
        if (gd.fTutorial != 0x0) {
            AdvanceTutor();
        }
    }
    return;
}
