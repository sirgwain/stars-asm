int16_t AlertSz(char *sz, int16_t mbType) {
    char szT[256];

    if (ini.fValidate == 0x0 && (ini.fLogging == 0x0 || ini.fGen == 0x0)) {
        return MessageBox(GetFocus(), sz, "Stars!", mbType);
    }
    _wsprintf(szT, "Error: %s", sz);
    OutputSz(ini.fValidate == 0x0 ? 6 : 7, szT);
    return IDYES;
}
