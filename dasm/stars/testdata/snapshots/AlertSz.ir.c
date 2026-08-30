int16_t AlertSz(char *sz, MessageBoxType mbType) {
    char     szT[256];
    uint16_t t_merge_21e5_0001;

L_2160:
    if ((ini.fValidate != 0x0))
        goto L_21a3;
    else
        goto L_217c;

L_217c:
    if ((ini.fLogging == 0x0))
        goto L_21f7;
    else
        goto L_218f;

L_218f:
    if ((ini.fGen == 0x0))
        goto L_21f7;
    else
        goto L_21a3;

L_21a3:
    _wsprintf(&(szT), "Error: %s", sz, 0x25);
    if ((ini.fValidate == 0x0))
        goto L_21e2;
    else
        goto L_21dc;

L_21dc:
    t_merge_21e5_0001 = 0x7;
    goto L_21e5;

L_21e2:
    t_merge_21e5_0001 = 0x6;

L_21e5:
    OutputSz(t_merge_21e5_0001, &(szT));
    return 0x6;

L_21f7:
    return MessageBox(GetFocus(), sz, "Stars!", mbType);
}
