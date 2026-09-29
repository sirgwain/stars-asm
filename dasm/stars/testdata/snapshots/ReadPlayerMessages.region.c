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

    imemMsgT = 0x0;
    fOOM = 0;
    lpb = (uint8_t *)lpMsg + imemMsgCur;
    while (hdrCur.rt == rtMsg) {
        if (hdrCur.cb != 0x0 && imemMsgCur + imemMsgT < 0xffc8 - hdrCur.cb) {
            fmemmove(lpb + imemMsgT, rgbCur, hdrCur.cb);
            imemMsgT = imemMsgT + hdrCur.cb;
        }
        ReadRt();
    }
    imemMsgCur = imemMsgCur + imemMsgT;
    lpbMax = lpb + imemMsgT;
    while (lpb < lpbMax) {
        lpmh = (MSGHDR *)lpb;
        bitfMsgSent[lpmh->iMsg >> 0x3] = LOBYTE((bitfMsgSent[lpmh->iMsg >> 0x3] & ~(0x1 << (lpmh->iMsg & 0x7))) | 0x1 << (lpmh->iMsg & 0x7));
        cMsg = cMsg + 1;
        u = lpmh->grWord;
        lpb = lpb + 4;
        iMax = (int16_t)rgcMsgArgs[lpmh->iMsg];
        for (i = 0; i < iMax; i++) {
            lpb = lpb + (1 + ((u & 0x1) == 0x1 ? 1 : 0));
            u = u >> 0x1;
        }
    }
    for (lpmp = (MSGPLR *)&vlpmsgplrIn; lpmp->lpmsgplrNext != 0x0; lpmp = lpmp->lpmsgplrNext) {
    }
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) == 0) {
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
            lpmp->lpmsgplrNext = 0x0;
            vcmsgplrIn = vcmsgplrIn + 1;
        }
    } else {
        penvMem = penvMemSav;
        fOOM = 1;
    }
    ReadRt();
    goto L_9b3a;
}
