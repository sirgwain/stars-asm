int16_t FIsAiAttack(FLEET *lpfl) {
    int16_t ihul;
    int16_t i;

    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            ihul = rgshdef[i].hul.ihuldef;
            if (ihul > 5 && ihul <= 10) {
                return 1;
            }
            switch (ihul) {
            case 5:
                goto L_4ad0;
            case 31:
            case 29:
                if (WtMaxShdefStat(&rgshdef[i], 2) < 500 && rglpshdef[idPlayer][i].lPower > 0) {
                    return 1;
                }
            default:
            }
        }
    }
    return 0;
L_4ad0:
    if (rglpshdef[idPlayer][i].lPower <= 0) {
        return 0;
    }
    return 1;
}
