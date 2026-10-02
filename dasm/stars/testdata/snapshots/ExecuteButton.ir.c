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

L_0db6:
    gd.fChgScanner = 1;
    goto L_160f;

L_0dd4:
    if (fDown == 0)
        goto L_167a;
    else
        goto L_0de0;

L_0de0:
    grbitScan = itb + (grbitScan & grbitScanToggleMask);
    goto L_1644;

L_0df2:
    grbitNew = 16;

LBitDiddle:
    if (fDown == 0)
        goto L_0e0a;
    else
        goto L_0e00;

L_0e00:
    grbitScan |= grbitNew;
    goto L_1644;

L_0e0a:
    grbitScan &= ~grbitNew;

L_0e13:
    goto L_1644;

L_0e16:
    grbitNew = 32;
    goto LBitDiddle;

L_0e1e:
    grbitNew = 128;
    goto LBitDiddle;

L_0e26:
    grbitNew = 0x100;
    goto LBitDiddle;

L_0e2e:
    grbitNew = 0x400;
    goto LBitDiddle;

L_0e36:
    grbitNew = 0x1000;
    goto LBitDiddle;

L_0e3e:
    grbitNew = 0x200;
    goto LBitDiddle;

L_0e46:
    grbitNew = 0x800;
    goto LBitDiddle;

L_0e4e:
    grbit = 1;
    c = 0;
    if ((grbitScan & grbitScanMineFields) != 0)
        goto L_0e6c;
    else
        goto L_0e66;

L_0e66:
    grbitScanMines = 0;

L_0e6c:
    i = 1278;
    goto L_0f0d;

L_0e74:
    if (i != 1278)
        goto L_0ea7;
    else
        goto L_0e7e;

L_0e7e:
    rgid[c] = (uint32_t)(grbitScanMines == 15 ? 1 : 0);
    goto L_0ecd;

L_0ea7:
    rgid[c] = (uint32_t)(grbitScanMines == 0 ? 1 : 0);

L_0ecd:
    CchGetString(i, &szWork[(i - 1278) * 30 + 160]);
    rgszScan[c++] = &szWork[(i - 1278) * 30 + 160];
    i++;

L_0f0d:
    if (i <= 1279)
        goto L_0e74;
    else
        goto L_0f17;

L_0f17:
    rgid[c] = 0;
    szWork[250] = -1;
    szWork[251] = 0;
    rgszScan[c++] = &szWork[250];
    i = 0;
    goto L_0fba;

L_0f50:
    rgid[c] = (uint32_t)((1 << i & grbitScanMines) == 0 ? 0 : 1);
    CchGetString(i + 1280, &szWork[i * 30]);
    rgszScan[c++] = &szWork[i * 30];
    i++;

L_0fba:
    if (i < 4)
        goto L_0f50;
    else
        goto L_0fc3;

L_0fc3:
    GetCursorPos16(&pt);
    ScreenToClient16(hwndTb, &pt);
    iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, 0);
    if (iSel == -1)
        goto L_167a;
    else
        goto L_1013;

L_1013:
    if (iSel >= 3)
        goto L_1037;
    else
        goto L_101c;

L_101c:
    if (iSel != 0)
        goto L_102e;
    else
        goto L_1025;

L_1025:
    grbitScanMines = 15;
    goto L_1047;

L_102e:
    grbitScanMines = 0;

L_1034:
    goto L_1047;

L_1037:
    iSel -= 3;
    grbitScanMines ^= 1 << iSel;

L_1047:
    if (grbitScanMines == 0)
        goto L_1059;
    else
        goto L_1051;

L_1051:
    grbitScan |= grbitScanMineFields;
    goto L_105e;

L_1059:
    grbitScan &= 0xffbf;

L_105e:
    InvalidateRect(hwndTb, NULL, 1);
    goto L_1644;

L_1076:
    c = 0;
    i = 1275;
    goto L_10d9;

L_1083:
    rgid[c] = 0;
    CchGetString(i, &szWork[(i - 1275) * 20]);
    rgszScan[c++] = &szWork[(i - 1275) * 20];
    i++;

L_10d9:
    if (i <= 1277)
        goto L_1083;
    else
        goto L_10e3;

L_10e3:
    rgid[c] = 0;
    szWork[200] = -1;
    szWork[201] = 0;
    rgszScan[c++] = &szWork[200];
    ish = 0;
    grbitSh = 1;
    goto L_1135;

L_1125:
    ish++;
    grbitSh *= 2;

L_1135:
    if (ish >= 16)
        goto L_11ab;
    else
        goto L_113e;

L_113e:
    if (rgshdef[ish].fFree != 0)
        goto L_1125;
    else
        goto L_115c;

L_115c:
    rgid[c] = (uint32_t)((grbitSh & grbitScanShip) == 0 ? 0 : 1);
    rgszScan[c++] = rgshdef[ish].hul.szClass;

L_11a8:
    goto L_1125;

L_11ab:
    GetCursorPos16(&pt);
    ScreenToClient16(hwndTb, &pt);
    iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, 0);
    if (iSel == -1)
        goto L_167a;
    else
        goto L_11fe;

L_11fe:
    if (iSel >= 4)
        goto L_1251;
    else
        goto L_1208;

L_1208:
    if (iSel != 0)
        goto L_121b;
    else
        goto L_1212;

L_1212:
    grbitScanShip = 0xffff;
    goto L_1233;

L_121b:
    if (iSel != 1)
        goto L_122d;
    else
        goto L_1225;

L_1225:
    grbitScanShip ^= 0xffff;
    goto L_1233;

L_122d:
    grbitScanShip = 0;

L_1233:
    if ((grbitScan & grbitScanDesignFilter) != 0)
        goto L_12e6;
    else
        goto L_1241;

L_1241:
    if (grbitScanShip != 0)
        goto LInvalS;
    else
        goto L_1248;

L_1248:
    goto L_12e6;

L_1251:
    iSel -= 4;
    ish = 0;
    goto L_1294;

L_125e:
    if (rgshdef[ish].fFree != 0)
        goto L_1290;
    else
        goto L_127c;

L_127c:
    iSel--;
    if (iSel < 0)
        goto L_129d;
    else
        goto L_1290;

L_1290:
    ish++;

L_1294:
    if (ish < 16)
        goto L_125e;
    else
        goto L_129d;

L_129d:
    grbitScanShip ^= 1 << ish;
    if ((grbitScan & grbitScanDesignFilter) != 0)
        goto L_12e6;
    else
        goto L_12b7;

L_12b7:
    if ((1 << ish & grbitScanShip) == 0)
        goto L_12e6;
    else
        goto LInvalS;

LInvalS:
    grbitScan |= grbitScanDesignFilter;
    InvalidateRect(hwndTb, NULL, 1);

L_12e6:
    if ((grbitScan & grbitScanDesignFilter) == 0)
        goto L_167a;
    else
        goto L_12f1;

L_12f1:
    goto L_1644;

L_12fa:
    grbit = 1;
    c = 0;
    i = 1275;
    goto L_1361;

L_130c:
    rgid[c] = 0;
    CchGetString(i, &szWork[(i - 1275) * 25 + 200]);
    rgszScan[c++] = &szWork[(i - 1275) * 25 + 200];
    i++;

L_1361:
    if (i <= 1277)
        goto L_130c;
    else
        goto L_136b;

L_136b:
    rgid[c] = 0;
    szWork[300] = -1;
    szWork[301] = 0;
    rgszScan[c++] = &szWork[300];
    i = 0;
    goto L_140e;

L_13a4:
    rgid[c] = (uint32_t)((1 << i & grbitScanEShip) == 0 ? 0 : 1);
    CchGetString(i + 381, &szWork[i * 25]);
    rgszScan[c++] = &szWork[i * 25];
    i++;

L_140e:
    if (i < 8)
        goto L_13a4;
    else
        goto L_1417;

L_1417:
    GetCursorPos16(&pt);
    ScreenToClient16(hwndTb, &pt);
    iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, 0);
    if (iSel == -1)
        goto L_167a;
    else
        goto L_1467;

L_1467:
    if (iSel >= 4)
        goto L_14b8;
    else
        goto L_1470;

L_1470:
    if (iSel != 0)
        goto L_1482;
    else
        goto L_1479;

L_1479:
    grbitScanEShip = 0xff;
    goto L_149a;

L_1482:
    if (iSel != 1)
        goto L_1494;
    else
        goto L_148b;

L_148b:
    grbitScanEShip ^= 0xff;
    goto L_149a;

L_1494:
    grbitScanEShip = 0;

L_149a:
    if ((grbitScan & grbitScanEnemyFilter) != 0)
        goto L_1505;
    else
        goto L_14a8;

L_14a8:
    if (grbitScanEShip != 0)
        goto LInvalE;
    else
        goto L_14af;

L_14af:
    goto L_1505;

L_14b8:
    iSel -= 4;
    grbitScanEShip ^= 1 << iSel;
    if ((grbitScan & grbitScanEnemyFilter) != 0)
        goto L_1505;
    else
        goto L_14d6;

L_14d6:
    if ((1 << iSel & grbitScanEShip) == 0)
        goto L_1505;
    else
        goto LInvalE;

LInvalE:
    grbitScan |= grbitScanEnemyFilter;
    InvalidateRect(hwndTb, NULL, 1);

L_1505:
    if ((grbitScan & grbitScanEnemyFilter) == 0)
        goto L_167a;
    else
        goto L_1510;

L_1510:
    goto L_1644;

L_1519:
    c = 0;
    i = 0;
    goto L_159d;

L_1526:
    rgid[c] = (uint32_t)(iScanZoom + 4 == i ? 1 : 0);
    _wsprintf(&szWork[i * 8], PCTDPCTPCT, vrgpctZoom[i]);
    rgszScan[c++] = &szWork[i * 8];
    i++;

L_159d:
    if (i < 9)
        goto L_1526;
    else
        goto L_15a6;

L_15a6:
    GetCursorPos16(&pt);
    ScreenToClient16(hwndTb, &pt);
    iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, 0);
    if (iSel == -1)
        goto L_167a;
    else
        goto L_15f6;

L_15f6:
    CommandHandler(hwndFrame, iSel + 3901);
    goto L_167a;

L_160f:
    if ((uint16_t)itb > tbShipCounts)
        goto L_167a;
    else
        goto L_1617;

L_1617:
    switch (itb * 2) {
    case 0x0:
        goto L_0dd4;
    case 0x2:
        goto L_0dd4;
    case 0x4:
        goto L_0dd4;
    case 0x6:
        goto L_0dd4;
    case 0x8:
        goto L_0dd4;
    case 0xa:
        goto L_0dd4;
    case 0xc:
        goto L_0df2;
    case 0xe:
        goto L_0e16;
    case 0x10:
        goto L_0e4e;
    case 0x12:
        goto L_0e1e;
    case 0x14:
        goto L_0e26;
    case 0x16:
        goto L_0e2e;
    case 0x18:
        goto L_0e3e;
    case 0x1a:
        goto L_1076;
    case 0x1c:
        goto L_0e46;
    case 0x1e:
        goto L_12fa;
    case 0x20:
        goto L_1519;
    case 0x22:
        goto L_0e36;
    }

L_1644:
    if (itb == tbAddWaypoints)
        goto L_1662;
    else
        goto L_164d;

L_164d:
    InvalidateRect(hwndScanner, NULL, 1);

L_1662:
    if (gd.fTutorial == 0)
        goto L_167a;
    else
        goto L_1675;

L_1675:
    AdvanceTutor();

L_167a:
    return;
}
