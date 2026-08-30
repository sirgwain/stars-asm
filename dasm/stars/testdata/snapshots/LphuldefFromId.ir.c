HULDEF *LphuldefFromId(int16_t id) {
    HULDEF *t_merge_5162_0001_wide;

L_512c:
    if ((id < 0x20))
        goto L_5150;
    else
        goto L_513e;

L_513e:
    t_merge_5162_0001_wide = LphuldefSBFromId((id + 0xffe0));
    goto L_5162;

L_5150:
    t_merge_5162_0001_wide = &(rghuldef[id]);

L_5162:
    return t_merge_5162_0001_wide;
}
