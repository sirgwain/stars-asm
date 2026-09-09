void GetFileStatus(int16_t dt, int16_t iPlayer) {
    uint16_t t_merge_4a99_0001;

L_4a60:
    SetSzWorkFromDt(dt, iPlayer);
    if ((access(szWork, 2) == 0))
        goto L_4a96;
    else
        goto L_4a90;

L_4a90:
    t_merge_4a99_0001 = 0x1;
    goto L_4a99;

L_4a96:
    t_merge_4a99_0001 = 0x0;

L_4a99:
    gd.fReadOnly = t_merge_4a99_0001;
    return;
}
