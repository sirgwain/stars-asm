int16_t CBattles() {
    BTLDATA *lpbd;
    HB      *lphb;
    int16_t  cBattles;

    cBattles = 0;
    lphb = rglphb[11];
    if (lphb == 0) {
        return 0;
    }
    lpbd = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
    while (1) {
        if (lpbd->id == 0xffff) {
            lphb = lphb->lphbNext;
            if (lphb == 0 || lphb->ibTop <= sizeof(HB))
                break;
            lpbd = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
        } else {
            if (lpbd->cbData == 0) {
                return cBattles;
            }
            lpbd = (BTLDATA *)((uint8_t *)lpbd + lpbd->cbData);
            cBattles++;
        }
    }
    return cBattles;
}
