int16_t FIsAiAttack(FLEET *lpfl) {
    HulDef  ihul;
    int16_t i;

L_4a72:
    i = 0;
    goto L_4b85;

L_4a83:
    if (lpfl->rgcsh[i] <= 0)
        goto L_4b81;
    else
        goto L_4aa0;

L_4aa0:
    ihul = rgshdef[i].hul.ihuldef;
    if ((int16_t)ihul <= ihuldefFrigate)
        goto L_4ac7;
    else
        goto L_4ab8;

L_4ab8:
    if ((int16_t)ihul > ihuldefDreadnought)
        goto L_4ac7;
    else
        goto L_4ac1;

L_4ac1:
    return 1;

L_4ac7:
    if (ihul != ihuldefFrigate)
        goto L_4b11;
    else
        goto L_4ad0;

L_4ad0:
    if (rglpshdef[idPlayer][i].lPower <= 0)
        goto L_4b0b;
    else
        goto L_4b05;

L_4b05:
    return 1;

L_4b0b:

L_4b0e:
    return 0;

L_4b11:
    if (ihul == ihuldefMetaMorph)
        goto L_4b23;
    else
        goto L_4b1a;

L_4b1a:
    if (ihul != ihuldefNubian)
        goto L_4b81;
    else
        goto L_4b23;

L_4b23:
    if (WtMaxShdefStat(&rgshdef[i], 2) >= 500)
        goto L_4b81;
    else
        goto L_4b46;

L_4b46:
    if (rglpshdef[idPlayer][i].lPower <= 0)
        goto L_4b81;
    else
        goto L_4b7b;

L_4b7b:
    return 1;

L_4b81:
    i++;

L_4b85:
    if (i < 16)
        goto L_4a83;
    else
        goto L_4b8e;

L_4b8e:
    return 0;
}
