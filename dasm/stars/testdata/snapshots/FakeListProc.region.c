LRESULT CALLBACK FakeListProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    POINT16 pt;
    int16_t iSel;

    switch (msg) {
    case WM_SETCURSOR:
        GetCursorPos16(&pt);
        ScreenToClient16(hwnd, &pt);
        if (pt.x >= 64)
            goto L_6924;
        SetCursor(hcurHand);
        return 1;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        if (LOWORD(lParam) < 64 && (mdBuild == mdBuildEdit || msg == WM_RBUTTONDOWN)) {
            CallWindowProc(lpfnRealListProc, hwnd, WM_LBUTTONDOWN, wParam, lParam);
            CallWindowProc(lpfnRealListProc, hwnd, WM_LBUTTONUP, wParam, lParam);
            if (msg == WM_RBUTTONDOWN || mdBuild != mdBuildEdit) {
                iSel = LOWORD(SendMessage(hwnd, LB_GETCURSEL, 0, 0));
                if (iSel == -1) {
                    return 0;
                }
                SendMessage(hwnd, LB_GETTEXT, iSel, (LPARAM)szWork);
                GlobalPD.part.hs.grhst = 1 << (szWork[0] - 'A');
                GlobalPD.part.hs.iItem = szWork[1] - 'A';
                FLookupPart(&GlobalPD.part);
                GlobalPD.grPopup = grPopupComponent;
                Popup(hwnd, LOWORD(lParam), HIWORD(lParam));
                return 0;
            }
            FTrackSlot(hwnd, LOWORD(lParam), HIWORD(lParam), wParam, 1, 0);
            return 0;
        }
    default:
    L_6924:
        return CallWindowProc(lpfnRealListProc, hwnd, msg, wParam, lParam);
    }
}
