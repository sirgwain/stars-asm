int16_t FTrackXfer(HWND hwnd, int16_t x, int16_t y, int16_t fkb) {
    POINT    ptOld;
    POINT    pt;
    int32_t  dChg;
    BTNT     btnt;
    int32_t  cCur;
    int16_t  i;
    int16_t  iBtn;
    int16_t  iVal;
    BTN      btn;
    int32_t  cNew;
    RECT     rc;
    int32_t  t_merge_5c20_0001;
    int32_t  t_merge_5cf0_0001;
    int32_t  t_call_5cf5;
    uint16_t t_merge_5d7f_0001;
    uint16_t t_merge_5def_0001;
    uint16_t scratch_bp_m4c;
    uint16_t t_merge_5e4d_0001;
    uint16_t t_merge_5e34_0001;
    uint32_t t_merge_5ecd_0001_wide;
    int32_t  t_merge_5f3b_0001;
    int32_t  t_call_5f40;

L_5a16:
    GetClientRect(hwnd, &(rc));
    pt.x = x;
    pt.y = y;
    i = 0;
    goto L_5a86;

L_5a42:
    if (((rgbtnXfer[i].bt & 0x4) != 0x0))
        goto L_5a82;
    else
        goto L_5a5c;

L_5a5c:
    if ((PtInRect(&(rgbtnXfer[i].rc), pt) != 0))
        goto L_5a91;
    else
        goto L_5a7c;

L_5a7c:

L_5a82:
    i = (i + 1);

L_5a86:
    if ((i < LOWORD(crgbtnXfer)))
        goto L_5a42;
    else
        goto L_5a91;

L_5a91:
    if ((i != LOWORD(crgbtnXfer)))
        goto L_5aa2;
    else
        goto L_5a9c;

L_5a9c:
    return 0x0;

L_5aa2:
    iBtn = (i >> 0x1);
    btn = rgbtnXfer[i];
    iVal = (btn.iVal & 0x7f);
    if ((btn.fVisible != 0x0))
        goto L_5d2e;
    else
        goto L_5adc;

L_5adc:
    if ((iVal > 4))
        goto FinishUp;
    else
        goto L_5ae2;

L_5ae2:

L_5ae8:
    if ((*(pxfer + 0x82) != 0x8))
        goto L_5b0e;
    else
        goto L_5af6;

L_5af6:
    if ((iVal == 4))
        goto FinishUp;
    else
        goto L_5aff;

L_5aff:
    if ((iVal == 3))
        goto FinishUp;
    else
        goto L_5b05;

L_5b05:

L_5b0e:
    /* untranslated: branch part[0x6:2](pxfer[btn.iSide]) != idPlayer ? L_5f70 : L_5b2b */

L_5b2b:

L_5b31:
    SetCapture(hwnd);
    ptOld.y = -1;
    ptOld.x = -1;

L_5b44:
    if ((FGetMouseMove(&(pt)) == 0))
        goto L_5d26;
    else
        goto L_5b58;

L_5b58:
    if ((pt.x != ptOld.x))
        goto L_5b6e;
    else
        goto L_5b63;

L_5b63:
    if ((pt.y == ptOld.y))
        goto L_5b44;
    else
        goto L_5b6e;

L_5b6e:
    ptOld.x = pt.x;
    ptOld.y = pt.y;
    if ((btn.iSide != 0x1))
        goto L_5bc0;
    else
        goto L_5b8c;

L_5b8c:
    if ((*(pxfer + 0x82) != 0x8))
        goto L_5bc0;
    else
        goto L_5b9a;

L_5b9a:
    cNew = (uint32_t)(((uint32_t)((*(pxfer + 0x92) & 0x3fff)) * 0xa));
    goto L_5c26;

L_5bc0:
    if ((iVal != 4))
        goto L_5bf6;
    else
        goto L_5bc9;

L_5bc9:
    LGetFleetStat(&(pxfer[btn.iSide].fl), 1);
    /* untranslated: t_merge_5c20_0001 = callresult(int32_t) */
    goto L_5c20;

L_5bf6:
    LGetFleetStat(&(pxfer[btn.iSide].fl), 2);
    /* untranslated: t_merge_5c20_0001 = callresult(int32_t) */

L_5c20:
    cNew = t_merge_5c20_0001;

L_5c26:
    cNew = (int32_t)(((uint32_t)(((uint32_t)((pt.x - btn.rc.left)) * cNew)) / (uint32_t)(((btn.rc.right - btn.rc.left) + 0xfffe))));
    /* untranslated: cCur = ChgCargo(pxfer[btn.iSide].grobj, pxfer[btn.iSide].id, iVal, 0, part[0x4:2](pxfer[btn.iSide])) */
    dChg = (cNew - cCur);
    if ((btn.iSide != 0x0))
        goto L_5ce3;
    else
        goto L_5cda;

L_5cda:
    t_merge_5cf0_0001 = dChg;
    goto L_5cf0;

L_5ce3:
    /* untranslated: t_merge_5cf0_0001 = words(((HIWORD(dChg) + 0x0) neg 0x0), (LOWORD(dChg) neg 0x0)) */

L_5cf0:
    t_call_5cf5 = XferSupply(iVal, t_merge_5cf0_0001);
    if ((LOWORD(t_call_5cf5) != 0x0))
        goto L_5d0d;
    else
        goto L_5d05;

L_5d05:
    if ((HIWORD(t_call_5cf5) == 0x0))
        goto L_5b44;
    else
        goto L_5d0d;

L_5d0d:
    DrawXferDlg(hwnd, 0x0, &(rc), iVal);

L_5d26:
    ReleaseCapture();
    goto FinishUp;

L_5d2e:
    InitBtnTrack(&(btnt), hwnd, 0x0, btn, btn.bt, 80, 0, 0, 0x0);
    if (((fkb & 0x8) == 0x0))
        goto L_5d88;
    else
        goto L_5d66;

L_5d66:
    if (((fkb & 0x4) == 0x0))
        goto L_5d7b;
    else
        goto L_5d74;

L_5d74:
    t_merge_5d7f_0001 = 0x3e8;
    goto L_5d7f;

L_5d7b:
    t_merge_5d7f_0001 = 0x64;

L_5d7f:
    dChg = (uint32_t)(t_merge_5d7f_0001);
    goto L_5dad;

L_5d88:
    if (((fkb & 0x4) == 0x0))
        goto L_5da3;
    else
        goto L_5d96;

L_5d96:
    dChg = 10;
    goto L_5dad;

L_5da3:
    dChg = 1;

L_5dad:
    if ((FTrackBtn(&(btnt)) == 0))
        goto FinishUp;
    else
        goto L_5dc1;

L_5dc1:
    if ((mdXferDlg != mdXferShips))
        goto L_5f01;
    else
        goto L_5dcb;

L_5dcb:
    if ((btn.iSide != 0x0))
        goto L_5dec;
    else
        goto L_5de6;

L_5de6:
    t_merge_5def_0001 = 0x1;
    goto L_5def;

L_5dec:
    t_merge_5def_0001 = 0x0;

L_5def:
    if ((scratch_bp_m4c >= pxfer[t_merge_5def_0001].fl.rgcsh[iVal]))
        goto L_5e19;
    else
        goto L_5e10;

L_5e10:
    t_merge_5e4d_0001 = LOWORD(dChg);
    goto L_5e4d;

L_5e19:
    if ((btn.iSide != 0x0))
        goto L_5e31;
    else
        goto L_5e2b;

L_5e2b:
    t_merge_5e34_0001 = 0x1;
    goto L_5e34;

L_5e31:
    t_merge_5e34_0001 = 0x0;

L_5e34:
    t_merge_5e4d_0001 = pxfer[t_merge_5e34_0001].fl.rgcsh[iVal];

L_5e4d:
    i = t_merge_5e4d_0001;
    if ((i == 0))
        goto L_5dad;
    else
        goto L_5e59;

L_5e59:
    if ((pxfer[btn.iSide].fl.rgcsh[iVal] < (32766 - i)))
        goto L_5e8c;
    else
        goto L_5e87;

L_5e87:
    i = 1;

L_5e8c:
    pxfer[btn.iSide].fl.rgcsh[iVal] = (pxfer[btn.iSide].fl.rgcsh[iVal] + i);
    if ((btn.iSide != 0x0))
        goto L_5eca;
    else
        goto L_5ec4;

L_5ec4:
    t_merge_5ecd_0001_wide = 0x10001;
    goto L_5ecd;

L_5eca:
    t_merge_5ecd_0001_wide = 0x0;

L_5ecd:
    pxfer[LOWORD(t_merge_5ecd_0001_wide)].fl.rgcsh[iVal] = (pxfer[HIWORD(t_merge_5ecd_0001_wide)].fl.rgcsh[iVal] - i);
    DrawXferDlg(hwnd, btnt.hdc, &(rc), iBtn);

L_5f01:
    if ((iVal < 0))
        goto L_5dad;
    else
        goto L_5f0a;

L_5f0a:
    if ((iVal > 4))
        goto L_5dad;
    else
        goto L_5f13;

L_5f13:
    if ((btn.iSide != 0x0))
        goto L_5f2e;
    else
        goto L_5f25;

L_5f25:
    t_merge_5f3b_0001 = dChg;
    goto L_5f3b;

L_5f2e:
    /* untranslated: t_merge_5f3b_0001 = words(((HIWORD(dChg) + 0x0) neg 0x0), (LOWORD(dChg) neg 0x0)) */

L_5f3b:
    t_call_5f40 = XferSupply(iVal, t_merge_5f3b_0001);
    if ((LOWORD(t_call_5f40) != 0x0))
        goto L_5f58;
    else
        goto L_5f50;

L_5f50:
    if ((HIWORD(t_call_5f40) == 0x0))
        goto L_5dad;
    else
        goto L_5f58;

L_5f58:
    DrawXferDlg(hwnd, btnt.hdc, &(rc), iVal);

FinishUp:
    UpdateXferBtns();
    DrawXferDlg(hwnd, 0x0, &(rc), -2);
    return 0x1;
}
