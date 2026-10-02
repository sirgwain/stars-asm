int16_t FSendPlrMsg2XGen(int16_t fPrepend, MessageId iMsg, MsgGoto iObj, int16_t p1, int16_t p2) {
    uint8_t  rgb[64];
    int16_t *pi;
    int16_t  i;
    uint16_t grbit;
    uint8_t *pb;
    uint16_t cSize;
    MSGHDR  *pmsghdr;
    int16_t  rgArgs[2];

    if ((uint16_t)(imemMsgCur + 20) > 0xffc8) {
        return 0;
    }
    pb = rgb;
    pmsghdr = (MSGHDR *)pb;
    pmsghdr->iMsg = iMsg;
    bitfMsgSent[iMsg >> 3] = LOBYTE((bitfMsgSent[iMsg >> 3] & ~(1 << (iMsg & 7))) | 1 << (iMsg & 7));
    pmsghdr->grWord = 0;
    pmsghdr->wGoto = iObj;
    pb += 4;
    grbit = 1;
    rgArgs[0] = p1;
    rgArgs[1] = p2;
    pi = rgArgs;
    i = 0;
    while (i < rgcMsgArgs[iMsg]) {
        if ((*pi & 0xff00) != 0) {
            pmsghdr->grWord |= grbit;
            RawStore16(pb, *pi);
            pb += 2;
        } else {
            *pb = LOBYTE(*pi);
            pb++;
        }
        i++;
        pi++;
        grbit *= 2;
    }
    cSize = pb - rgb;
    if (fPrepend != 0) {
        fmemmove((uint8_t *)lpMsg + cSize, lpMsg, imemMsgCur);
        fmemmove(lpMsg, rgb, cSize);
    } else {
        fmemmove((uint8_t *)lpMsg + imemMsgCur, rgb, cSize);
    }
    imemMsgCur += cSize;
    cMsg++;
    iMsgCur = -1;
    iMsgCur = IMsgNext(0);
    return 1;
}
