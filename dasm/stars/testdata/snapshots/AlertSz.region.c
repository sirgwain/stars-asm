int16_t AlertSz(char *sz, int16_t mbType) {
    char szT[256];

    if (ini.fValidate != 0 || (ini.fLogging != 0 && ini.fGen != 0)) {
        _wsprintf(szT, "Error: %s", sz);
        OutputSz(ini.fValidate == 0 ? 6 : 7, szT);
        return IDYES;
    }
    return MessageBox(GetFocus(), sz, "Stars!", mbType);
}
