int16_t FFleetMightHaveTeeth(FLEET *lpfl) {
    HUL    *lphul;
    int16_t ishdef;

    for (ishdef = 0; ishdef < 16; ishdef++) {
        if (lpfl->rgcsh[ishdef] != 0) {
            lphul = &rglpshdef[lpfl->iplr][ishdef].hul;
            if (FHullHasTeeth(lphul) != 0) {
                return 1;
            }
        }
    }
    return 0;
}
