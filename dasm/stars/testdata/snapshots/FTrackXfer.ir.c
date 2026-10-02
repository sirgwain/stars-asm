int16_t FTrackXfer(HWND hwnd, int16_t x, int16_t y, int16_t fkb) {
    POINT16  ptOld;
    POINT16  pt;
    int32_t  dChg;
    BTNT     btnt;
    int32_t  cCur;
    int16_t  i;
    int16_t  iBtn;
    int16_t  iVal;
    BTN      btn;
    int32_t  cNew;
    RECT     rc;
    uint16_t t_merge_5ecd_0001;

L_5a16:
    GetClientRect(hwnd, &rc);
    pt.x = x;
    pt.y = y;
    i = 0;
    goto L_5a86;

L_5a42:
    if ((rgbtnXfer[i].bt & 4) != 0)
        goto L_5a82;
    else
        goto L_5a5c;

L_5a5c:
    if (PtInRect(&rgbtnXfer[i].rc, PointFrom16(pt)) != 0)
        goto L_5a91;
    else
        goto L_5a82;

L_5a82:
    i++;

L_5a86:
    if (i < crgbtnXfer)
        goto L_5a42;
    else
        goto L_5a91;

L_5a91:
    if (i != crgbtnXfer)
        goto L_5aa2;
    else
        goto L_5a9c;

L_5a9c:
    return 0;

L_5aa2:
    iBtn = i >> 1;
    btn = rgbtnXfer[i];
    iVal = btn.iVal & 0x7f;
    if (btn.fVisible != 0)
        goto L_5d2e;
    else
        goto L_5adc;

L_5adc:
    if (iVal > 4)
        goto FinishUp;
    else
        goto L_5ae8;

L_5ae8:
    if (pxfer[1].grobj != grobjThing)
        goto L_5b0e;
    else
        goto L_5af6;

L_5af6:
    if (iVal == 4)
        goto FinishUp;
    else
        goto L_5aff;

L_5aff:
    if (iVal == 3)
        goto FinishUp;
    else
        goto L_5b05;

L_5b05:
    goto L_5b31;

L_5b0e:
    if (pxfer[btn.iSide].fl.iPlayer != idPlayer)
        goto FinishUp;
    else
        goto L_5b31;

L_5b31:
    SetCapture(hwnd);
    ptOld.y = -1;
    ptOld.x = -1;

L_5b44:
    if (FGetMouseMove(&pt) == 0)
        goto L_5d26;
    else
        goto L_5b58;

L_5b58:
    if (pt.x != ptOld.x)
        goto L_5b6e;
    else
        goto L_5b63;

L_5b63:
    if (pt.y == ptOld.y)
        goto L_5b44;
    else
        goto L_5b6e;

L_5b6e:
    ptOld = pt;
    if (btn.iSide != 1)
        goto L_5bc0;
    else
        goto L_5b8c;

L_5b8c:
    if (pxfer[1].grobj != grobjThing)
        goto L_5bc0;
    else
        goto L_5b9a;

L_5b9a:
    cNew = (uint32_t)(pxfer[1].th.thp.wtMax * 10);
    goto L_5c26;

L_5bc0:
    if (iVal != 4)
        goto L_5bf6;
    else
        goto L_5bc9;

L_5bc9:
    cNew = LGetFleetStat(&pxfer[btn.iSide].fl, 1);
    goto L_5c20;

L_5bf6:
    cNew = LGetFleetStat(&pxfer[btn.iSide].fl, 2);

L_5c20:

L_5c26:
    cNew = (int32_t)((int16_t)(pt.x - btn.rc.left) * cNew) / (int16_t)(btn.rc.right - btn.rc.left - 2);
    cCur = ChgCargo(pxfer[btn.iSide].grobj, pxfer[btn.iSide].id, iVal, 0, (uint8_t *)(pxfer + btn.iSide) + 4);
    dChg = cNew - cCur;
    if (XferSupply(iVal, btn.iSide == 0 ? dChg : -dChg) != 0)
        goto L_5d0d;
    else
        goto L_5b44;

L_5d0d:
    DrawXferDlg(hwnd, NULL, &rc, iVal);

L_5d23:
    goto L_5b44;

L_5d26:
    ReleaseCapture();
    goto FinishUp;

L_5d2e:
    InitBtnTrack(&btnt, hwnd, NULL, &btn.rc, btn.bt, 80, 0, 0, NULL);
    if ((fkb & 8) == 0)
        goto L_5d88;
    else
        goto L_5d66;

L_5d66:
    dChg = (uint32_t)((fkb & 4) == 0 ? 100 : 1000);
    goto L_5dad;

L_5d88:
    if ((fkb & 4) == 0)
        goto L_5da3;
    else
        goto L_5d96;

L_5d96:
    dChg = 10;
    goto L_5dad;

L_5da3:
    dChg = 1;

L_5dad:
    if (FTrackBtn(&btnt) == 0)
        goto FinishUp;
    else
        goto L_5dc1;

L_5dc1:
    if (mdXferDlg != mdXferShips)
        goto L_5f01;
    else
        goto L_5dcb;

L_5dcb:
    if ((int16_t)LOWORD(dChg) >= pxfer[btn.iSide == 0 ? 1 : 0].fl.rgcsh[iVal])
        goto L_5e19;
    else
        goto L_5e10;

L_5e10:
    i = LOWORD(dChg);
    goto L_5e4d;

L_5e19:
    i = pxfer[btn.iSide == 0 ? 1 : 0].fl.rgcsh[iVal];

L_5e4d:
    if (i == 0)
        goto L_5dad;
    else
        goto L_5e59;

L_5e59:
    if (pxfer[btn.iSide].fl.rgcsh[iVal] < 32766 - i)
        goto L_5e8c;
    else
        goto L_5e87;

L_5e87:
    i = 1;

L_5e8c:
    pxfer[btn.iSide].fl.rgcsh[iVal] = pxfer[btn.iSide].fl.rgcsh[iVal] + i;
    t_merge_5ecd_0001 = btn.iSide == 0 ? 1 : 0;
    pxfer[t_merge_5ecd_0001].fl.rgcsh[iVal] -= i;
    DrawXferDlg(hwnd, btnt.hdc, &rc, iBtn);

L_5efe:
    goto L_5dad;

L_5f01:
    if (iVal < 0)
        goto L_5dad;
    else
        goto L_5f0a;

L_5f0a:
    if (iVal > 4)
        goto L_5dad;
    else
        goto L_5f13;

L_5f13:
    if (XferSupply(iVal, btn.iSide == 0 ? dChg : -dChg) != 0)
        goto L_5f58;
    else
        goto L_5dad;

L_5f58:
    DrawXferDlg(hwnd, btnt.hdc, &rc, iVal);

L_5f6d:
    goto L_5dad;

FinishUp:
    UpdateXferBtns();
    DrawXferDlg(hwnd, NULL, &rc, SupplyButtonsOnly);
    return 1;
}
