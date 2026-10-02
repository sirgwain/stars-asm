void EnsureMacintiStarbaseDesigns(uint8_t *rgSB) {
    int16_t k;
    int16_t iOld;
    int16_t cAge;
    int16_t i;
    int16_t j;
    int16_t iNew;

    *rgSB = 0;
    for (i = 1; i <= 3; i++) {
        if (rglpshdefSB[idPlayer][i].fFree != 0 || rglpshdefSB[idPlayer][i].cExist == 0) {
            if (FCreateAiStarbase(i, i >= 3 ? 2 : 1, vrgSBMacAisb[i - 1], i) != 0) {
                rgSB[i] = 0;
            } else {
                rgSB[i] = 1;
            }
        } else {
            cAge = game.turn - rglpshdefSB[idPlayer][i].turn;
            if (cAge < 35) {
                if (game.turn > 25 && i == 1 && rglpshdefSB[idPlayer][i].hul.chs != 8) {
                    rgSB[i] = 3;
                } else {
                    rgSB[i] = 0;
                }
            } else if (cAge < 50) {
                rgSB[i] = 2;
            } else {
                rgSB[i] = 3;
            }
        }
    }
    iOld = -1;
    for (i = 1; i <= 3; i++) {
        if (rgSB[i] >= 2 &&
            (iOld == -1 || (int16_t)rgSB[i] > rgSB[iOld] || (rgSB[i] == rgSB[iOld] && rglpshdefSB[idPlayer][i].turn < rglpshdefSB[idPlayer][iOld].turn))) {
            iOld = i;
        }
    }
    for (i = 1; i <= 3; i++) {
        if (rgSB[i] >= 2 && i != iOld) {
            rgSB[i] = 0;
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3 && (rglpshdefSB[idPlayer][3 * i + 4 + j].fFree != 0 || rglpshdefSB[idPlayer][3 * i + 4 + j].cExist <= 0); j++) {
        }
        if (j == 3) {
            for (j = 0; j < 3; j++) {
                for (k = 5; k >= 3 && FCreateAiStarbase(3 * i + 4 + j, j + 1, vrgSBMacAisb[k], k - 1) == 0; k--) {
                }
            }
        }
    }
    for (i = 4; i < 10; i++) {
        rgSB[i] = rglpshdefSB[idPlayer][i].fFree != 0;
    }
    if (rglpshdefSB[idPlayer][4].turn >= rglpshdefSB[idPlayer][7].turn) {
        iNew = 4;
        iOld = 7;
    } else {
        iNew = 7;
        iOld = 4;
    }
    if ((uint16_t)(game.turn - rglpshdefSB[idPlayer][iOld].turn) < 30) {
        j = 2;
    } else {
        j = 3;
    }
    for (i = iOld; i < iOld + 3; i++) {
        rgSB[i] = j;
    }
    i = rglpshdefSB[idPlayer][iNew].hul.ihuldef - 32;
    if (i < 4) {
        rgSB[3] = 2;
        if (i < 3) {
            rgSB[2] = 2;
        }
    }
    return;
}
