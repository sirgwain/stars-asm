int16_t CBattles() {
    BTLDATA *lpbd;
    HB      *lphb;
    int16_t  cBattles;

    cBattles = 0;
    lphb = rglphb[11];
    if (lphb != 0x0) {
        lpbd = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
        while (1) {
            if (lpbd->id != 0xffff) {
                if (lpbd->cbData == 0x0) {
                    return cBattles;
                }
                lpbd = (BTLDATA *)((uint8_t *)lpbd + lpbd->cbData);
                cBattles = cBattles + 1;
            } else {
                lphb = lphb->lphbNext;
                if (lphb == 0x0 || lphb->ibTop <= sizeof(HB))
                    break;
                lpbd = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
            }
        }
        return cBattles;
    }
    return 0;
}
