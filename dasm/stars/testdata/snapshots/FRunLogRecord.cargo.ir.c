int16_t FRunLogRecord(RecordType rt, int16_t cb, uint8_t *lpb) {
    int16_t   fExtra;
    int32_t   cXfer;
    XFERFULL *lpxfCur;
    PLANET   *lppl;
    int32_t   rgcXfer[5];
    XFER      rgxf[2];
    FLEET    *lpfl;
    int16_t   ifl;
    int16_t   i;
    uint16_t  grbit;
    int16_t   rgifl[512];
    SHDEF    *lpshdef;
    int16_t   iPass;
    int16_t   iLook;
    PLANET   *lpplMac;
    int8_t    ch;
    int32_t   l;
    char      szT[33];
    int16_t   cOut;
    THING    *lpth;
    int16_t   id;
    int16_t   iColDrop;
    COLDROP  *lpcdT;
    XFERFULL *lpxfMax;
    MessageId idm;
    int16_t   t_b926;
    int16_t   t_b9dc;

L_ae39:
    rgcXfer[i] = (int16_t)(int8_t)lpb[iLook + 6];
    goto L_aecd;

L_ae66:
    if (rt != rtLogCargoXfer16)
        goto L_ae9d;
    else
        goto L_ae6f;

L_ae6f:
    rgcXfer[i] = (int16_t)RawLoad16(lpb + (iLook * 2 + 6));
    goto L_aecd;

L_ae9d:
    rgcXfer[i] = RawLoad32(lpb + (iLook * 4 + 6));
}
