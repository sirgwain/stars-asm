int16_t FCanKillTok(TOK *ptok1, TOK *ptok2) {
    int32_t lp1;
    int32_t lp2;

    lp1 = LpshdefFromTok(ptok1)->lPower;
    lp2 = LpshdefFromTok(ptok2)->lPower;
    if (lp2 > lp1) {
        return FALSE;
    }
    if ((lp2 & 0x7ffff000) < (lp1 & 0x7ffff000)) {
        return TRUE;
    }
    if ((lp2 & 0x7fffff00) == (lp1 & 0x7fffff00) && ptok1->spd >= ptok2->spd) {
        return TRUE;
    }
    return FALSE;
}
