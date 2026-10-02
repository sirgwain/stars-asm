void ReadPlayerMessages() {
    uint8_t *lpbMax;
    int16_t  iMax;
    int16_t  fOOM;
    jmp_buf *penvMemSav;
    MSGHDR  *lpmh;
    uint16_t imemMsgT;
    int16_t  i;
    jmp_buf  env;
    MSGPLR  *lpmp;
    uint8_t *lpb;
    uint16_t u;

    imemMsgT = 0;
    fOOM = 0;
    lpb = (uint8_t *)lpMsg + imemMsgCur;
    while (hdrCur.rt == rtMsg) {
        if (hdrCur.cb != 0 && (uint16_t)(imemMsgCur + imemMsgT) < (uint16_t)(0xffc8 - hdrCur.cb)) {
            fmemmove(lpb + imemMsgT, rgbCur, hdrCur.cb);
            imemMsgT += hdrCur.cb;
        }
        ReadRt();
    }
    imemMsgCur += imemMsgT;
    lpbMax = lpb + imemMsgT;
    while (lpb < lpbMax) {
        lpmh = (MSGHDR *)lpb;
        bitfMsgSent[lpmh->iMsg >> 3] = (bitfMsgSent[lpmh->iMsg >> 3] & ~(1 << (lpmh->iMsg & 7))) | 1 << (lpmh->iMsg & 7);
        cMsg++;
        u = lpmh->grWord;
        lpb += 4;
        iMax = rgcMsgArgs[lpmh->iMsg];
        for (i = 0; i < iMax; i++) {
            lpb += 1 + ((u & 1) == 1);
            u >>= 1;
        }
    }
    for (lpmp = (MSGPLR *)&vlpmsgplrIn; lpmp->lpmsgplrNext != 0; lpmp = lpmp->lpmsgplrNext) {
    }
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0) {
        penvMem = penvMemSav;
        fOOM = 1;
    } else {
    L_9b3a:
        if (hdrCur.rt != rtPlrMsg) {
            iMsgCur = -1;
            iMsgCur = IMsgNext(0);
            return;
        }
        if (fOOM == 0) {
            lpmp->lpmsgplrNext = LpAlloc(hdrCur.cb + (sizeof(MSGPLR) - 12), htPlrMsg);
            lpmp = lpmp->lpmsgplrNext;
            fmemcpy((uint8_t *)&lpmp->iPlrFrom - 4, rgbCur, hdrCur.cb);
            lpmp->lpmsgplrNext = NULL;
            vcmsgplrIn++;
        }
    }
    ReadRt();
    goto L_9b3a;
}
