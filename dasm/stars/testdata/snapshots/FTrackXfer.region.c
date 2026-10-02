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

    GetClientRect(hwnd, &rc);
    pt.x = x;
    pt.y = y;
    for (i = 0; i < crgbtnXfer && ((rgbtnXfer[i].bt & 4) != 0 || PtInRect(&rgbtnXfer[i].rc, PointFrom16(pt)) == 0); i++) {
    }
    if (i == crgbtnXfer) {
        return 0;
    }
    iBtn = i >> 1;
    btn = rgbtnXfer[i];
    iVal = btn.iVal & 0x7f;
    if (btn.fVisible != 0) {
        InitBtnTrack(&btnt, hwnd, NULL, &btn.rc, btn.bt, 80, 0, 0, NULL);
        if ((fkb & 8) != 0) {
            dChg = (uint32_t)((fkb & 4) == 0 ? 100 : 1000);
        } else if ((fkb & 4) != 0) {
            dChg = 10;
        } else {
            dChg = 1;
        }
        while (FTrackBtn(&btnt) != 0) {
            if (mdXferDlg == mdXferShips) {
                i = (int16_t)LOWORD(dChg) < pxfer[btn.iSide == 0 ? 1 : 0].fl.rgcsh[iVal] ? LOWORD(dChg) : pxfer[btn.iSide == 0 ? 1 : 0].fl.rgcsh[iVal];
                if (i != 0) {
                    if (pxfer[btn.iSide].fl.rgcsh[iVal] >= 32766 - i) {
                        i = 1;
                    }
                    pxfer[btn.iSide].fl.rgcsh[iVal] = pxfer[btn.iSide].fl.rgcsh[iVal] + i;
                    t_merge_5ecd_0001 = btn.iSide == 0 ? 1 : 0;
                    pxfer[t_merge_5ecd_0001].fl.rgcsh[iVal] -= i;
                    DrawXferDlg(hwnd, btnt.hdc, &rc, iBtn);
                }
            } else if (iVal >= 0 && iVal <= 4 && XferSupply(iVal, btn.iSide == 0 ? dChg : -dChg) != 0) {
                DrawXferDlg(hwnd, btnt.hdc, &rc, iVal);
            }
        }
    } else if (iVal <= 4) {
        if (pxfer[1].grobj == grobjThing) {
            if (iVal == 4 || iVal == 3)
                goto FinishUp;
        } else if (pxfer[btn.iSide].fl.iPlayer != idPlayer) {
            goto FinishUp;
        }
        SetCapture(hwnd);
        ptOld.y = -1;
        ptOld.x = -1;
        while (FGetMouseMove(&pt) != 0) {
            if (pt.x != ptOld.x || pt.y != ptOld.y) {
                ptOld = pt;
                if (btn.iSide == 1 && pxfer[1].grobj == grobjThing) {
                    cNew = (uint32_t)(pxfer[1].th.thp.wtMax * 10);
                } else if (iVal == 4) {
                    cNew = LGetFleetStat(&pxfer[btn.iSide].fl, 1);
                } else {
                    cNew = LGetFleetStat(&pxfer[btn.iSide].fl, 2);
                }
                cNew = (int32_t)((int16_t)(pt.x - btn.rc.left) * cNew) / (int16_t)(btn.rc.right - btn.rc.left - 2);
                cCur = ChgCargo(pxfer[btn.iSide].grobj, pxfer[btn.iSide].id, iVal, 0, (uint8_t *)(pxfer + btn.iSide) + 4);
                dChg = cNew - cCur;
                if (XferSupply(iVal, btn.iSide == 0 ? dChg : -dChg) != 0) {
                    DrawXferDlg(hwnd, NULL, &rc, iVal);
                }
            }
        }
        ReleaseCapture();
    }
FinishUp:
    UpdateXferBtns();
    DrawXferDlg(hwnd, NULL, &rc, SupplyButtonsOnly);
    return 1;
}
