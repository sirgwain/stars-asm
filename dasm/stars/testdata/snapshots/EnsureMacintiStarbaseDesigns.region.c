void EnsureMacintiStarbaseDesigns(uint8_t *rgSB) {
    int16_t  k;
    int16_t  iOld;
    int16_t  cAge;
    int16_t  i;
    int16_t  j;
    int16_t  iNew;
    uint16_t t_scratch_m10;
    uint16_t t_scratch_m10_2;

    *rgSB = 0x0;
    for (i = 1; i <= 3; i++) {
        if (rglpshdefSB[idPlayer][i].fFree == 0x0 && rglpshdefSB[idPlayer][i].cExist != 0x0) {
            cAge = game.turn - rglpshdefSB[idPlayer][i].turn;
            if (cAge >= 35) {
                if (cAge >= 50) {
                    rgSB[i] = 0x3;
                } else {
                    rgSB[i] = 0x2;
                }
            } else if (game.turn <= 0x19 || i != 1 || rglpshdefSB[idPlayer][i].hul.chs == 0x8) {
                rgSB[i] = 0x0;
            } else {
                rgSB[i] = 0x3;
            }
        } else if (FCreateAiStarbase(i, i >= 3 ? 2 : 1, vrgSBMacAisb[i - 1], i) == 0) {
            rgSB[i] = 0x1;
        } else {
            rgSB[i] = 0x0;
        }
    }
    iOld = -1;
    for (i = 1; i <= 3; i++) {
        if (rgSB[i] >= 0x2) {
            if (iOld != -1) {
                t_scratch_m10 = rgSB[i];
                if (t_scratch_m10 <= rgSB[iOld]) {
                    t_scratch_m10_2 = rgSB[i];
                    if (t_scratch_m10_2 != rgSB[iOld] || rglpshdefSB[idPlayer][i].turn >= rglpshdefSB[idPlayer][iOld].turn)
                        continue;
                }
            }
            iOld = i;
        }
    }
    for (i = 1; i <= 3; i++) {
        if (rgSB[i] >= 0x2 && i != iOld) {
            rgSB[i] = 0x0;
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3 && (rglpshdefSB[idPlayer][3 * i + 4 + j].fFree != 0x0 || rglpshdefSB[idPlayer][3 * i + 4 + j].cExist <= 0x0); j++) {
        }
        if (j == 3) {
            for (j = 0; j < 3; j++) {
                for (k = 5; k >= 3 && FCreateAiStarbase(3 * i + 4 + j, j + 1, vrgSBMacAisb[k], k - 1) == 0; k--) {
                }
            }
        }
    }
    for (i = 4; i < 10; i++) {
        rgSB[i] = LOBYTE(rglpshdefSB[idPlayer][i].fFree == 0x0 ? 0x0 : 0x1);
    }
    if (rglpshdefSB[idPlayer][4].turn < rglpshdefSB[idPlayer][7].turn) {
        iNew = 7;
        iOld = 4;
    } else {
        iNew = 4;
        iOld = 7;
    }
    if (game.turn - rglpshdefSB[idPlayer][iOld].turn >= 0x1e) {
        j = 3;
    } else {
        j = 2;
    }
    for (i = iOld; i < iOld + 3; i++) {
        rgSB[i] = LOBYTE(j);
    }
    i = rglpshdefSB[idPlayer][iNew].hul.ihuldef - 32;
    if (i < 4) {
        rgSB[3] = 0x2;
        if (i < 3) {
            rgSB[2] = 0x2;
        }
    }
    return;
}
