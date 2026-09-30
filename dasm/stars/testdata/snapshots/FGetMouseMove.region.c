int16_t FGetMouseMove(POINT16 *ppt) {
    MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, 1) != 0) {
        if (msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONUP) {
            ppt->x = LOWORD(msg.lParam);
            ppt->y = HIWORD(msg.lParam);
            if (msg.message != WM_LBUTTONUP) {
                return 1;
            }
            return 0;
        }
    }
    return 1;
}
