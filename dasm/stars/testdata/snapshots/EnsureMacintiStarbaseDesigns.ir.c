void EnsureMacintiStarbaseDesigns(uint8_t *rgSB) {
    int16_t k;
    int16_t iOld;
    int16_t cAge;
    int16_t i;
    int16_t j;
    int16_t iNew;

L_76e4:
    *rgSB = 0;
    i = 1;
    goto L_7858;

L_76fb:
    if (rglpshdefSB[idPlayer][i].fFree != 0)
        goto L_7759;
    else
        goto L_7729;

L_7729:
    if (rglpshdefSB[idPlayer][i].cExist != 0)
        goto L_77ad;
    else
        goto L_7759;

L_7759:
    if (FCreateAiStarbase(i, i >= 3 ? 2 : 1, vrgSBMacAisb[i - 1], i) == 0)
        goto L_779f;
    else
        goto L_7791;

L_7791:
    rgSB[i] = 0;
    goto L_7854;

L_779f:
    rgSB[i] = 1;

L_77aa:
    goto L_7854;

L_77ad:
    cAge = game.turn - rglpshdefSB[idPlayer][i].turn;
    if (cAge >= 35)
        goto L_7832;
    else
        goto L_77da;

L_77da:
    if (game.turn <= 25)
        goto L_7824;
    else
        goto L_77e4;

L_77e4:
    if (i != 1)
        goto L_7824;
    else
        goto L_77ed;

L_77ed:
    if (rglpshdefSB[idPlayer][i].hul.chs == 8)
        goto L_7824;
    else
        goto L_7816;

L_7816:
    rgSB[i] = 3;
    goto L_7854;

L_7824:
    rgSB[i] = 0;

L_782f:
    goto L_7854;

L_7832:
    if (cAge >= 50)
        goto L_7849;
    else
        goto L_783b;

L_783b:
    rgSB[i] = 2;
    goto L_7854;

L_7849:
    rgSB[i] = 3;

L_7854:
    i++;

L_7858:
    if (i <= 3)
        goto L_76fb;
    else
        goto L_7861;

L_7861:
    iOld = -1;
    i = 1;
    goto L_792f;

L_786e:
    if (rgSB[i] < 2)
        goto L_792b;
    else
        goto L_7883;

L_7883:
    if (iOld == -1)
        goto L_7925;
    else
        goto L_788c;

L_788c:
    if ((int16_t)rgSB[i] > rgSB[iOld])
        goto L_7925;
    else
        goto L_78b5;

L_78b5:
    if (rgSB[i] != rgSB[iOld])
        goto L_792b;
    else
        goto L_78de;

L_78de:
    if (rglpshdefSB[idPlayer][i].turn >= rglpshdefSB[idPlayer][iOld].turn)
        goto L_792b;
    else
        goto L_7925;

L_7925:
    iOld = i;

L_792b:
    i++;

L_792f:
    if (i <= 3)
        goto L_786e;
    else
        goto L_7938;

L_7938:
    i = 1;
    goto L_796f;

L_7940:
    if (rgSB[i] < 2)
        goto L_796b;
    else
        goto L_7955;

L_7955:
    if (i == iOld)
        goto L_796b;
    else
        goto L_7960;

L_7960:
    rgSB[i] = 0;

L_796b:
    i++;

L_796f:
    if (i <= 3)
        goto L_7940;
    else
        goto L_7978;

L_7978:
    i = 0;
    goto L_7a82;

L_7980:
    j = 0;
    goto L_7a08;

L_7988:
    if (rglpshdefSB[idPlayer][3 * i + 4 + j].fFree != 0)
        goto L_7a04;
    else
        goto L_79c1;

L_79c1:
    if (rglpshdefSB[idPlayer][3 * i + 4 + j].cExist <= 0)
        goto L_7a04;
    else
        goto L_7a11;

L_7a04:
    j++;

L_7a08:
    if (j < 3)
        goto L_7988;
    else
        goto L_7a11;

L_7a11:
    if (j != 3)
        goto L_7a7e;
    else
        goto L_7a1a;

L_7a1a:
    j = 0;
    goto L_7a75;

L_7a22:
    k = 5;
    goto L_7a68;

L_7a2a:
    if (FCreateAiStarbase(3 * i + 4 + j, j + 1, vrgSBMacAisb[k], k - 1) != 0)
        goto L_7a71;
    else
        goto L_7a64;

L_7a64:
    k--;

L_7a68:
    if (k >= 3)
        goto L_7a2a;
    else
        goto L_7a71;

L_7a71:
    j++;

L_7a75:
    if (j < 3)
        goto L_7a22;
    else
        goto L_7a7e;

L_7a7e:
    i++;

L_7a82:
    if (i < 2)
        goto L_7980;
    else
        goto L_7a8b;

L_7a8b:
    i = 4;
    goto L_7ad8;

L_7a93:
    rgSB[i] = rglpshdefSB[idPlayer][i].fFree != 0;
    i++;

L_7ad8:
    if (i < 10)
        goto L_7a93;
    else
        goto L_7ae1;

L_7ae1:
    if (rglpshdefSB[idPlayer][4].turn < rglpshdefSB[idPlayer][7].turn)
        goto L_7b15;
    else
        goto L_7b08;

L_7b08:
    iNew = 4;
    iOld = 7;
    goto L_7b1f;

L_7b15:
    iNew = 7;
    iOld = 4;

L_7b1f:
    if ((uint16_t)(game.turn - rglpshdefSB[idPlayer][iOld].turn) >= 30)
        goto L_7b50;
    else
        goto L_7b48;

L_7b48:
    j = 2;
    goto L_7b55;

L_7b50:
    j = 3;

L_7b55:
    i = iOld;
    goto L_7b62;

L_7b5e:
    i++;

L_7b62:
    if (i >= iOld + 3)
        goto L_7b80;
    else
        goto L_7b70;

L_7b70:
    rgSB[i] = j;
    goto L_7b5e;

L_7b80:
    i = rglpshdefSB[idPlayer][iNew].hul.ihuldef - 32;
    if (i >= 4)
        goto L_7bc3;
    else
        goto L_7bac;

L_7bac:
    rgSB[3] = 2;
    if (i >= 3)
        goto L_7bc3;
    else
        goto L_7bbc;

L_7bbc:
    rgSB[2] = 2;

L_7bc3:
    return;
}
