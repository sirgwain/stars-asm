int16_t FIsAiAttack(FLEET *lpfl) {
    int16_t ihul;
    int16_t i;

L_4a72:
    i = 0;
    goto L_4b85;

L_4a83:
    if ((lpfl->rgcsh[i] <= 0))
        goto L_4b81;
    else
        goto L_4aa0;

L_4aa0:
    ihul = rgshdef[i].hul.ihuldef;
    if ((ihul <= 5))
        goto L_4ac7;
    else
        goto L_4ab8;

L_4ab8:
    if ((ihul > 10))
        goto L_4ac7;
    else
        goto L_4ac1;

L_4ac1:
    return 1;

L_4ac7:
    if ((ihul != 5))
        goto L_4b11;
    else
        goto L_4ad0;

L_4ad0:
    if ((HIWORD(rglpshdef[idPlayer][i].lPower) < 0x0))
        goto L_4b0b;
    else
        goto L_4af5;

L_4af5:
    if ((HIWORD(rglpshdef[idPlayer][i].lPower) > 0x0))
        goto L_4b05;
    else
        goto L_4afa;

L_4afa:
    if ((LOWORD(rglpshdef[idPlayer][i].lPower) <= 0x0))
        goto L_4b0b;
    else
        goto L_4b05;

L_4b05:
    return 1;

L_4b0b:

L_4b0e:
    return 0;

L_4b11:
    if ((ihul == 31))
        goto L_4b23;
    else
        goto L_4b1a;

L_4b1a:
    if ((ihul != 29))
        goto L_4b81;
    else
        goto L_4b23;

L_4b23:
    if ((WtMaxShdefStat(&(rgshdef[i]), 2) >= 500))
        goto L_4b81;
    else
        goto L_4b46;

L_4b46:
    if ((HIWORD(rglpshdef[idPlayer][i].lPower) < 0x0))
        goto L_4b81;
    else
        goto L_4b6b;

L_4b6b:
    if ((HIWORD(rglpshdef[idPlayer][i].lPower) > 0x0))
        goto L_4b7b;
    else
        goto L_4b70;

L_4b70:
    if ((LOWORD(rglpshdef[idPlayer][i].lPower) <= 0x0))
        goto L_4b81;
    else
        goto L_4b7b;

L_4b7b:
    return 1;

L_4b81:
    i = (i + 1);

L_4b85:
    if ((i < 16))
        goto L_4a83;
    else
        goto L_4b8e;

L_4b8e:
    return 0;
}
