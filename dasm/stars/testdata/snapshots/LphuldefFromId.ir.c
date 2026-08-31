HULDEF *LphuldefFromId(int16_t id) {
L_512c:
    if ((id < 0x20))
        goto L_5150;
    else
        goto L_513e;

L_513e:
    return LphuldefSBFromId((id + 0xffe0));

L_5150:
    return &(rghuldef[id]);
}
