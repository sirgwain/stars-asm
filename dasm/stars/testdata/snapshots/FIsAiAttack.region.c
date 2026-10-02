int16_t FIsAiAttack(FLEET *lpfl) {
    HulDef  ihul;
    int16_t i;

    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            ihul = rgshdef[i].hul.ihuldef;
            if ((int16_t)ihul > ihuldefFrigate && (int16_t)ihul <= ihuldefDreadnought) {
                return 1;
            }
            switch (ihul) {
            case ihuldefFrigate:
                if (rglpshdef[idPlayer][i].lPower > 0) {
                    return 1;
                }
                return 0;
            case ihuldefMetaMorph:
            case ihuldefNubian:
                if (WtMaxShdefStat(&rgshdef[i], 2) < 500 && rglpshdef[idPlayer][i].lPower > 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
