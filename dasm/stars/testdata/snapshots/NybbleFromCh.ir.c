int16_t NybbleFromCh(uint8_t ch) {
    char *pch;

L_4880:
    if (ch < 97)
        goto L_48b9;
    else
        goto L_4897;

L_4897:
    if (ch > 122)
        goto L_48b9;
    else
        goto L_48a5;

L_48a5:
    return rgcompstrlower[ch - 97];

L_48b9:
    if (ch != 32)
        goto L_48cd;
    else
        goto L_48c7;

L_48c7:
    return 0;

L_48cd:
    if (ch < 65)
        goto L_4900;
    else
        goto L_48db;

L_48db:
    if (ch > 80)
        goto L_4900;
    else
        goto L_48e9;

L_48e9:
    return (ch - 0x41) << 4 | 0xb;

L_4900:
    if (ch < 81)
        goto L_4933;
    else
        goto L_490e;

L_490e:
    if (ch > 90)
        goto L_4933;
    else
        goto L_491c;

L_491c:
    return (ch - 0x51) << 4 | 0xc;

L_4933:
    if (ch < 48)
        goto L_4966;
    else
        goto L_4941;

L_4941:
    if (ch > 53)
        goto L_4966;
    else
        goto L_494f;

L_494f:
    return (ch - 0x26) << 4 | 0xc;

L_4966:
    if (ch < 54)
        goto L_4999;
    else
        goto L_4974;

L_4974:
    if (ch > 57)
        goto L_4999;
    else
        goto L_4982;

L_4982:
    return (ch - 0x36) << 4 | 0xd;

L_4999:
    pch = strchr(rgchcomp, ch);
    if (pch == 0)
        goto L_49cf;
    else
        goto L_49b8;

L_49b8:
    return (pch - rgchcomp + 4) << 4 | 0xe;

L_49cf:
    return ch << 4 | 0xf;
}
