int16_t NybbleFromCh(uint8_t ch) {
    char *pch;

    if (ch >= 97 && ch <= 122) {
        return rgcompstrlower[ch - 97];
    }
    if (ch == 32) {
        return 0;
    }
    if (ch >= 65 && ch <= 80) {
        return (ch - 0x41) << 4 | 0xb;
    }
    if (ch >= 81 && ch <= 90) {
        return (ch - 0x51) << 4 | 0xc;
    }
    if (ch >= 48 && ch <= 53) {
        return (ch - 0x26) << 4 | 0xc;
    }
    if (ch >= 54 && ch <= 57) {
        return (ch - 0x36) << 4 | 0xd;
    }
    pch = strchr(rgchcomp, ch);
    if (pch != 0) {
        return (pch - rgchcomp + 4) << 4 | 0xe;
    }
    return ch << 4 | 0xf;
}
