void AnimateAttack(HDC hdc) {
    TOK         *ptokSrc;
    TOK         *ptokAttack;
    POINT16      ptBeam1;
    int16_t      cFrame;
    int16_t      dyFrame;
    POINT16      ptRay2;
    POINT16      ptTop;
    uint32_t     dwTickLast;
    int16_t      dxFrame;
    uint32_t     dwTickCur;
    POINT16      ptBase;
    int16_t      dy;
    POINT16      ptRay1;
    int16_t      y;
    POINT16      ptRight;
    POINT16      ptDest;
    int16_t      iHit;
    GrfWeapon    grfWeapon;
    POINT16      ptSrc;
    POINT16      ptTorp;
    POINT16      ptLeft;
    tagTIMERINFO ti;
    int16_t      iFrame;
    int16_t      dx;
    int16_t      fKill;
    POINT16      ptDestBottom;
    POINT16      ptBeam2;
    POINT16      ptBottom;
    POINT16      ptDestTop;
    POINT16      ptDestRight;
    POINT16      ptDestLeft;
    int16_t      x;
    HDC          hdcMem;
    HBITMAP      hbmpSav;
    HBITMAP      hbmpScreen;
    uint16_t     t_scratch_m76_3;
    uint16_t     t_scratch_m76_4;
    uint16_t     t_scratch_m76_8;
    uint16_t     t_scratch_m76_9;
    int16_t      t_scratch_m7c;
    int16_t      t_merge_41df_0001;

    grfWeapon = 0;
    fKill = 0;
    if (viStepVCRCur >= 0) {
        ptokSrc = vrgtok + vlpbrVCR->itok;
        x = ptokSrc->brc & 0xf;
        y = ptokSrc->brc >> 4;
        ptSrc.x = (dxyVCRSquare + 3) * x + 10 + dxyVCRSquare / 2 + 1;
        ptSrc.y = (dxyVCRSquare + 3) * y + 10 + dxyVCRSquare / 2 + 1;
        t_scratch_m76_3 = dxyVCRSquare / 2;
        ptTop.x = (dxyVCRSquare + 3) * x + 10 + t_scratch_m76_3 + 1;
        ptBottom.x = (dxyVCRSquare + 3) * x + 10 + t_scratch_m76_3 + 1;
        t_scratch_m76_4 = dxyVCRSquare / 2;
        ptRight.y = (dxyVCRSquare + 3) * y + 10 + t_scratch_m76_4 + 1;
        ptLeft.y = (dxyVCRSquare + 3) * y + 10 + t_scratch_m76_4 + 1;
        ptBottom.y = dxyVCRSquare / 3 + ptLeft.y;
        ptTop.y = ptLeft.y - dxyVCRSquare / 3;
        ptLeft.x = ptBottom.x - dxyVCRSquare / 3;
        ptRight.x = dxyVCRSquare / 3 + ptBottom.x;
        iHit = 0;
        do {
            grfWeapon = vlpbrVCR->rgkill[iHit].grfWeapon;
            if (vlpbrVCR->rgkill[iHit].cshKill != 0) {
                fKill = 1;
            }
            for (iFrame = iHit + 1; iFrame < vlpbrVCR->ctok && vlpbrVCR->rgkill[iFrame].itok == vlpbrVCR->rgkill[iHit].itok; iFrame++) {
                grfWeapon |= vlpbrVCR->rgkill[iFrame].grfWeapon;
                if (vlpbrVCR->rgkill[iFrame].cshKill != 0) {
                    fKill = 1;
                }
            }
            ptokAttack = vrgtok + vlpbrVCR->rgkill[iHit].itok;
            x = ptokAttack->brc & 0xf;
            y = ptokAttack->brc >> 4;
            dx = (ptokSrc->brc & 0xf) - x;
            dy = (ptokSrc->brc >> 4) - y;
            ptDest.x = (dxyVCRSquare + 3) * x + 10 + dxyVCRSquare / 2 + 1;
            ptDest.y = (dxyVCRSquare + 3) * y + 10 + dxyVCRSquare / 2 + 1;
            t_scratch_m76_8 = dxyVCRSquare / 2;
            ptDestTop.x = (dxyVCRSquare + 3) * x + 10 + t_scratch_m76_8 + 1;
            ptDestBottom.x = (dxyVCRSquare + 3) * x + 10 + t_scratch_m76_8 + 1;
            t_scratch_m76_9 = dxyVCRSquare / 2;
            ptDestRight.y = (dxyVCRSquare + 3) * y + 10 + t_scratch_m76_9 + 1;
            ptDestLeft.y = (dxyVCRSquare + 3) * y + 10 + t_scratch_m76_9 + 1;
            ptDestBottom.y = dxyVCRSquare / 3 + ptDestLeft.y;
            ptDestTop.y = ptDestLeft.y - dxyVCRSquare / 3;
            ptDestLeft.x = ptDestBottom.x - dxyVCRSquare / 3;
            ptDestRight.x = dxyVCRSquare / 3 + ptDestBottom.x;
            iHit = iFrame;
            if (dx != 0 || dy != 0) {
                if (dx == 0 || (abs(dx) == 1 && abs(dy) > 2)) {
                    ptBeam1 = ptRight;
                    ptBeam2 = ptLeft;
                    if (dy > 0) {
                        ptTorp = ptTop;
                    } else {
                        ptTorp = ptBottom;
                    }
                    if (dy > 0) {
                        ptRay2 = ptDestLeft;
                    } else {
                        ptRay2 = ptDestRight;
                    }
                    if (dy > 0) {
                        ptRay1 = ptDestRight;
                    } else {
                        ptRay1 = ptDestLeft;
                    }
                } else if (dy == 0 || (abs(dy) == 1 && abs(dx) > 2)) {
                    ptBeam1 = ptTop;
                    ptBeam2 = ptBottom;
                    if (dx > 0) {
                        ptTorp = ptLeft;
                    } else {
                        ptTorp = ptRight;
                    }
                    if (dx > 0) {
                        ptRay1 = ptDestTop;
                    } else {
                        ptRay1 = ptDestBottom;
                    }
                    if (dx > 0) {
                        ptRay2 = ptDestBottom;
                    } else {
                        ptRay2 = ptDestTop;
                    }
                } else if (dx > 0) {
                    if (dy > 0) {
                        ptRay2 = ptDestBottom;
                        ptRay1 = ptDestRight;
                        ptBeam1 = ptTop;
                    } else {
                        ptRay2 = ptDestRight;
                        ptRay1 = ptDestTop;
                        ptBeam1 = ptBottom;
                    }
                    ptBeam2 = ptLeft;
                    ptTorp.x = ptLeft.x;
                    ptTorp.y = ptBeam1.y;
                } else {
                    if (dy > 0) {
                        ptBeam1 = ptTop;
                        ptRay1 = ptDestBottom;
                        ptRay2 = ptDestLeft;
                    } else {
                        ptBeam1 = ptBottom;
                        ptRay1 = ptDestLeft;
                        ptRay2 = ptDestTop;
                    }
                    ptBeam2 = ptRight;
                    ptTorp.x = ptRight.x;
                    ptTorp.y = ptBeam1.y;
                }
                if ((grfWeapon & (bitFBeamLow | bitFBeamHigh)) != 0) {
                    SelectObject(hdc, (grfWeapon & bitFBeamHigh) == 0 ? hpenEnemy : hpenStarbase);
                    MoveTo(hdc, ptBeam1.x, ptBeam1.y);
                    LineTo(hdc, ptDest.x, ptDest.y);
                    MoveTo(hdc, ptBeam2.x, ptBeam2.y);
                    LineTo(hdc, ptDest.x, ptDest.y);
                    DrawIcon(hdc, ptDest.x - 16, ptDest.y - 16, rghiconVCR[0]);
                }
                if ((grfWeapon & bitFTorp) != 0) {
                    if (fAnimate != 0) {
                        hdcMem = CreateCompatibleDC(hdc);
                        if (hdcMem == 0)
                            goto LFinishUp;
                        hbmpScreen = CreateCompatibleBitmap(hdc, 32, 32);
                        if (hbmpScreen == 0) {
                            DeleteDC(hdcMem);
                            goto LFinishUp;
                        }
                        hbmpSav = SelectObject(hdcMem, hbmpScreen);
                        t_scratch_m7c = abs(dx);
                        t_merge_41df_0001 = t_scratch_m7c > abs(dy) ? abs(dx) : abs(dy);
                        cFrame = t_merge_41df_0001 * ((grfWeapon & bitFTorp) == 0 ? 4 : 8);
                        ptBase = ptTorp;
                        dxFrame = ptTorp.x - ptDest.x;
                        dyFrame = ptTorp.y - ptDest.y;
                        ti.dwSize = 12;
                        TimerCount(&ti);
                        dwTickLast = ti.dwmsSinceStart;
                        for (iFrame = 0; iFrame < cFrame; iFrame++) {
                            BitBlt(hdcMem, 0, 0, 32, 32, hdc, ptTorp.x - 16, ptTorp.y - 16, SRCCOPY);
                            DrawIcon(hdc, ptTorp.x - 16, ptTorp.y - 16, rghiconVCR[(iFrame & 3) + 3]);
                            do {
                                TimerCount(&ti);
                                dwTickCur = ti.dwmsSinceStart;
                            } while (dwTickCur >= dwTickLast && dwTickCur < dwTickLast + 35 - (int16_t)(10 * viSpeedVCR));
                            dwTickLast = dwTickCur;
                            BitBlt(hdc, ptTorp.x - 16, ptTorp.y - 16, 32, 32, hdcMem, 0, 0, SRCCOPY);
                            ptTorp.x = ptBase.x - LOWORD((int32_t)((int32_t)(dxFrame * iFrame) / cFrame));
                            ptTorp.y = ptBase.y - LOWORD((int32_t)((int32_t)(dyFrame * iFrame) / cFrame));
                        }
                        SelectObject(hdcMem, hbmpSav);
                        DeleteObject(hbmpScreen);
                        DeleteDC(hdcMem);
                    }
                    if ((grfWeapon & bitFNoHit) == 0) {
                        DrawIcon(hdc, ptDest.x - 16, ptDest.y - 16, rghiconVCR[1]);
                    }
                }
            }
        LFinishUp:;
        } while (iHit < vlpbrVCR->ctok);
        for (iFrame = 0; iFrame < vlpbrVCR->ctok; iFrame++) {
            fKill = vlpbrVCR->rgkill[iFrame].cshKill <= 0 ? 0 : 1;
            ptokAttack = vrgtok + vlpbrVCR->rgkill[iFrame].itok;
            ptDest.x = (ptokAttack->brc & 0xf) * (dxyVCRSquare + 3) + 10 + dxyVCRSquare / 2 + 1;
            ptDest.y = (ptokAttack->brc >> 4) * (dxyVCRSquare + 3) + 10 + dxyVCRSquare / 2 + 1;
            DrawIcon(hdc, ptDest.x - 16, ptDest.y - 16, rghiconVCR[fKill == 0 ? 0 : 2]);
        }
        fAnimate = 0;
    }
    return;
}
