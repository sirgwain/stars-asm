void GetFileStatus(int16_t dt, int16_t iPlayer) {
    SetSzWorkFromDt(dt, iPlayer);
    gd.fReadOnly = access(szWork, 2) != 0;
    return;
}
