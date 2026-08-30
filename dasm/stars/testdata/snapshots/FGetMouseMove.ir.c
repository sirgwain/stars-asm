int16_t FGetMouseMove(POINT *ppt) {
    MSG msg;

L_4146:

L_4152:
    if ((PeekMessage(&(msg), 0x0, 0x0, 0x0, 0x1) != 0x0))
        goto L_417c;
    else
        goto L_4176;

L_4176:
    return 0x1;

L_417c:
    goto L_41c3;

L_4185:
    /* untranslated: ppt->x = part[6:2](msg) */
    /* untranslated: ppt->y = (loword((uint32_t)(words(part[6:2](msg), part[8:2](msg)) >> 0x10)) & 0xffff) */
    if ((msg.message == 0x202))
        goto L_41ba;
    else
        goto L_41b4;

L_41b4:
    return 0x1;

L_41ba:
    return 0x0;

L_41c3:
    if ((msg.message == 0x200))
        goto L_4185;
    else
        goto L_41cb;

L_41cb:
    if ((msg.message != 0x202))
        goto L_4152;
    else
        goto L_41d0;

L_41d0:
}
