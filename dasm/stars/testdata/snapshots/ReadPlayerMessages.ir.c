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
    uint16_t t_merge_9aaf_0001;

L_994a:
    imemMsgT = 0x0;
    fOOM = 0;
    lpb = ((uint8_t *)(lpMsg) + imemMsgCur);

L_9970:
    if ((hdrCur.rt != rtMsg))
        goto L_99df;
    else
        goto L_9983;

L_9983:
    if ((hdrCur.cb == 0x0))
        goto L_99d7;
    else
        goto L_9991;

L_9991:
    if (((imemMsgCur + imemMsgT) >= (0xffc8 - hdrCur.cb)))
        goto L_99d7;
    else
        goto L_99ab;

L_99ab:
    fmemmove((lpb + imemMsgT), rgbCur, hdrCur.cb);
    imemMsgT = (imemMsgT + hdrCur.cb);

L_99d7:
    ReadRt();
    goto L_9970;

L_99df:
    imemMsgCur = (imemMsgCur + imemMsgT);
    lpbMax = (lpb + imemMsgT);
    goto L_9aca;

L_99fa:
    lpmh = (MSGHDR *)(lpb);
    bitfMsgSent[(lpmh->iMsg >> 0x3)] = LOBYTE(((bitfMsgSent[(lpmh->iMsg >> 0x3)] & (~(0x1 << (lpmh->iMsg & 0x7)))) | (0x1 << (lpmh->iMsg & 0x7))));
    cMsg = (cMsg + 1);
    u = lpmh->grWord;
    lpb = (lpb + 4);
    iMax = (int16_t)(rgcMsgArgs[lpmh->iMsg]);
    i = 0;
    goto L_9abf;

L_9a98:
    if (((u & 0x1) != 0x1))
        goto L_9aac;
    else
        goto L_9aa6;

L_9aa6:
    t_merge_9aaf_0001 = 0x1;
    goto L_9aaf;

L_9aac:
    t_merge_9aaf_0001 = 0x0;

L_9aaf:
    lpb = (lpb + (1 + t_merge_9aaf_0001));
    u = (u >> 0x1);
    i = (i + 1);

L_9abf:
    if ((i < iMax))
        goto L_9a98;
    else
        goto L_9aca;

L_9aca:
    if ((lpb < lpbMax))
        goto L_99fa;
    else
        goto L_9ad8;

L_9ad8:
    lpmp = (MSGPLR *)(&(vlpmsgplrIn));

L_9ae3:
    if ((lpmp->lpmsgplrNext != 0x0))
        goto L_9af9;
    else
        goto L_9b0c;

L_9af9:
    lpmp = lpmp->lpmsgplrNext;
    goto L_9ae3;

L_9b0c:
    penvMemSav = penvMem;
    penvMem = &(env);
    if ((setjmp(env) == 0))
        goto L_9b3a;
    else
        goto L_9b2c;

L_9b2c:
    penvMem = penvMemSav;
    fOOM = 1;
    goto LOutOfMem;

L_9b3a:
    if ((hdrCur.rt != rtPlrMsg))
        goto L_9bba;
    else
        goto L_9b4d;

L_9b4d:
    if ((fOOM != 0))
        goto LOutOfMem;
    else
        goto L_9b56;

L_9b56:
    lpmp->lpmsgplrNext = LpAlloc((hdrCur.cb + (sizeof(MSGPLR) - 12)), htPlrMsg);
    lpmp = lpmp->lpmsgplrNext;
    fmemcpy(((uint8_t *)(&(lpmp->iPlrFrom)) - 4), rgbCur, hdrCur.cb);
    lpmp->lpmsgplrNext = 0x0;
    vcmsgplrIn = (vcmsgplrIn + 1);

LOutOfMem:
    ReadRt();
    goto L_9b3a;

L_9bba:
    iMsgCur = -1;
    iMsgCur = IMsgNext(0);
    return;
}
