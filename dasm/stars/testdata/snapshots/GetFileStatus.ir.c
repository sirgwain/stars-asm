void GetFileStatus(int16_t dt, int16_t iPlayer) {
L_4a60:
    SetSzWorkFromDt(dt, iPlayer);
    gd.fReadOnly = access(szWork, 2) == 0 ? 0 : 1;
    return;
}
