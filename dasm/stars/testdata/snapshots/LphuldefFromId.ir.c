HULDEF *LphuldefFromId(int16_t id) {
L_512c:
    if ((id < 32))
        goto L_5150;
    else
        goto L_513e;

L_513e:
    return LphuldefSBFromId((id - 32));

L_5150:
    return &(rghuldef[id]);
}
