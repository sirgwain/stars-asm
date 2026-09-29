int16_t FGetMouseMove(POINT16 *ppt) {
    MSG msg;

    while (PeekMessage(&msg, 0x0, 0x0, 0x0, 0x1) != 0) {
        if (msg.message == 0x200 || msg.message == 0x202)
            goto L_4185;
    }
    return 1;
L_4185:
    ppt->x = LOWORD(msg.lParam);
    ppt->y = HIWORD(msg.lParam);
    if (msg.message == 0x202) {
        return 0;
    }
    return 1;
}
