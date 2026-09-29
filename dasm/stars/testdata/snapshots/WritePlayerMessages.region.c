void WritePlayerMessages(int16_t iPlayer) {
    uint8_t *lpbMax;
    uint8_t  rgb[1024];
    int16_t  cbMsg;
    MSGPLR  *lpmp;
    uint8_t *lpb;

    cbMsg = 0;
    if (iPlayer != -1) {
        lpb = (uint8_t *)lpMsg;
        lpbMax = lpb + imemMsgCur;
        for (; lpb < lpbMax; lpb = lpb + (5 + (*lpb >> 0x4 & 0xf))) {
            if (cbMsg + 20 >= 1024) {
                WriteRt(rtMsg, cbMsg, rgb);
                cbMsg = 0;
            }
            if ((*lpb & 0xf) == iPlayer && (RawLoad16((uint8_t *)lpb + 0x1) & 0x1ff) != 0x1ff) {
                fmemmove(&rgb[cbMsg], lpb + 1, (*lpb >> 0x4 & 0xf) + 0x4);
                cbMsg = cbMsg + ((*lpb >> 0x4 & 0xf) + 0x4);
            }
        }
        if (cbMsg != 0) {
            WriteRt(rtMsg, cbMsg, rgb);
        }
        for (lpmp = vlpmsgplrOut; lpmp != 0x0; lpmp = lpmp->lpmsgplrNext) {
            if ((lpmp->iPlrTo == 0 && lpmp->iPlrFrom != iPlayer) || lpmp->iPlrTo - 1 == iPlayer) {
                WriteRt(rtPlrMsg, abs(lpmp->cLen) + 12, (uint8_t *)&lpmp->iPlrFrom - 4);
            }
        }
    }
    return;
}
