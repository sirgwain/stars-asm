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
    int32_t  t_call_5beb;
    int32_t  t_merge_5c20_0001;
    int32_t  t_call_5c18;
    int16_t  t_merge_5e4d_0001;
    uint16_t t_merge_5ecd_0001;

    GetClientRect(hwnd, &rc);
    pt.x = x;
    pt.y = y;
    for (i = 0; i < crgbtnXfer && ((rgbtnXfer[i].bt & 0x4) != 0x0 || PtInRect(&rgbtnXfer[i].rc, PointFrom16(pt)) == 0); i++) {
    }
    if (i != crgbtnXfer) {
        iBtn = i >> 0x1;
        btn = rgbtnXfer[i];
        iVal = btn.iVal & 0x7f;
        if (btn.fVisible != 0x0) {
            InitBtnTrack(&btnt, hwnd, 0x0, &btn.rc, btn.bt, 80, 0, 0, 0x0);
            if ((fkb & 0x8) == 0x0) {
                if ((fkb & 0x4) == 0x0) {
                    dChg = 1;
                } else {
                    dChg = 10;
                }
            } else {
                dChg = (uint32_t)((fkb & 0x4) == 0x0 ? 0x64 : 0x3e8);
            }
            while (FTrackBtn(&btnt) != 0) {
                if (mdXferDlg != mdXferShips) {
                    if (iVal >= 0 && iVal <= 4 && XferSupply(iVal, btn.iSide == 0x0 ? dChg : -dChg) != 0) {
                        DrawXferDlg(hwnd, btnt.hdc, &rc, iVal);
                    }
                } else {
                    if (LOWORD(dChg) >= pxfer[btn.iSide == 0x0 ? 1 : 0].fl.rgcsh[iVal]) {
                        t_merge_5e4d_0001 = pxfer[btn.iSide == 0x0 ? 1 : 0].fl.rgcsh[iVal];
                    } else {
                        t_merge_5e4d_0001 = LOWORD(dChg);
                    }
                    i = t_merge_5e4d_0001;
                    if (i != 0) {
                        if (pxfer[btn.iSide].fl.rgcsh[iVal] >= 32766 - i) {
                            i = 1;
                        }
                        pxfer[btn.iSide].fl.rgcsh[iVal] = pxfer[btn.iSide].fl.rgcsh[iVal] + i;
                        t_merge_5ecd_0001 = btn.iSide == 0x0 ? 0x1 : 0x0;
                        pxfer[t_merge_5ecd_0001].fl.rgcsh[iVal] = pxfer[t_merge_5ecd_0001].fl.rgcsh[iVal] - i;
                        DrawXferDlg(hwnd, btnt.hdc, &rc, iBtn);
                    }
                }
            }
        } else if (iVal <= 4) {
            if (pxfer[1].grobj != grobjThing) {
                if (pxfer[btn.iSide].fl.iPlayer != idPlayer)
                    goto FinishUp;
            } else if (iVal == 4 || iVal == 3) {
                goto FinishUp;
            }
            SetCapture(hwnd);
            ptOld.y = -1;
            ptOld.x = -1;
            while (FGetMouseMove(&pt) != 0) {
                if (pt.x != ptOld.x || pt.y != ptOld.y) {
                    ptOld = pt;
                    if (btn.iSide != 0x1 || pxfer[1].grobj != grobjThing) {
                        if (iVal != 4) {
                            t_call_5c18 = LGetFleetStat(&pxfer[btn.iSide].fl, 2);
                            t_merge_5c20_0001 = t_call_5c18;
                        } else {
                            t_call_5beb = LGetFleetStat(&pxfer[btn.iSide].fl, 1);
                            t_merge_5c20_0001 = t_call_5beb;
                        }
                        cNew = t_merge_5c20_0001;
                    } else {
                        cNew = (uint32_t)(pxfer[1].th.thp.wtMax * 0xa);
                    }
                    cNew = (int32_t)((int32_t)((int32_t)(pt.x - btn.rc.left) * cNew) / (int32_t)(btn.rc.right - btn.rc.left - 2));
                    cCur = ChgCargo(pxfer[btn.iSide].grobj, pxfer[btn.iSide].id, iVal, 0, (uint8_t *)(pxfer + btn.iSide) + 4);
                    dChg = cNew - cCur;
                    if (XferSupply(iVal, btn.iSide == 0x0 ? dChg : -dChg) != 0) {
                        DrawXferDlg(hwnd, 0x0, &rc, iVal);
                    }
                }
            }
            ReleaseCapture();
        }
    FinishUp:
        UpdateXferBtns();
        DrawXferDlg(hwnd, 0x0, &rc, -2);
        return 1;
    }
    return 0;
}
