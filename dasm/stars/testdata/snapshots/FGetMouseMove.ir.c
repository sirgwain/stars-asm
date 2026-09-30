int16_t FGetMouseMove(POINT16 *ppt) {
    MSG msg;

L_4146:

L_4152:
    if (PeekMessage(&msg, NULL, 0, 0, 1) != 0)
        goto L_417c;
    else
        goto L_4176;

L_4176:
    return 1;

L_417c:
    goto L_41c3;

L_4185:
    ppt->x = LOWORD(msg.lParam);
    ppt->y = HIWORD(msg.lParam);
    if (msg.message == WM_LBUTTONUP)
        goto L_41ba;
    else
        goto L_41b4;

L_41b4:
    return 1;

L_41ba:

L_41bd:
    return 0;

L_41c3:
    if (msg.message == WM_MOUSEMOVE)
        goto L_4185;
    else
        goto L_41cb;

L_41cb:
    if (msg.message != WM_LBUTTONUP)
        goto L_4152;
    else
        goto L_41d0;

L_41d0:
    goto L_4185;
}
