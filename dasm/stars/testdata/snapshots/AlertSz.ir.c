int16_t AlertSz(char *sz, int16_t mbType) {
    char szT[256];

L_2160:
    if (ini.fValidate != 0)
        goto L_21a3;
    else
        goto L_217c;

L_217c:
    if (ini.fLogging == 0)
        goto L_21f7;
    else
        goto L_218f;

L_218f:
    if (ini.fGen == 0)
        goto L_21f7;
    else
        goto L_21a3;

L_21a3:
    _wsprintf(szT, "Error: %s", sz);
    OutputSz(ini.fValidate == 0 ? 6 : 7, szT);
    return IDYES;

L_21f7:
    return MessageBox(GetFocus(), sz, "Stars!", mbType);
}
