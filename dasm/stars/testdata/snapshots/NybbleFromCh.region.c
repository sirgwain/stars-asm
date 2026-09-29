int16_t NybbleFromCh(uint8_t ch) {
    char *pch;

    if (ch < 0x61 || ch > 0x7a) {
        if (ch != 0x20) {
            if (ch < 0x41 || ch > 0x50) {
                if (ch < 0x51 || ch > 0x5a) {
                    if (ch < 0x30 || ch > 0x35) {
                        if (ch < 0x36 || ch > 0x39) {
                            pch = strchr(rgchcomp, ch);
                            if (pch == 0x0) {
                                return ch << 0x4 | 0xf;
                            }
                            return (pch - rgchcomp + 0x4) << 0x4 | 0xe;
                        }
                        return (ch - 0x36) << 0x4 | 0xd;
                    }
                    return (ch - 0x26) << 0x4 | 0xc;
                }
                return (ch - 0x51) << 0x4 | 0xc;
            }
            return (ch - 0x41) << 0x4 | 0xb;
        }
        return 0;
    }
    return rgcompstrlower[ch - 97];
}
