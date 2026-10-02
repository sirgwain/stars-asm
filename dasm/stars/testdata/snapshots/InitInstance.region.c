int16_t InitInstance(int16_t nCmdShow) {
    int16_t sw;
    RECT    rc;

    ini.fWait = 0;
    ini.fStartupFile = 0;
    ini.grobjSel = 0;
    ini.idPlayer = -1;
    ReadIniSettings();
    rc = ini.wnFrame.rc;
    hwndFrame = CreateWindow(szFrame, "Stars!", WS_OVERLAPPEDWINDOW, rc.left, rc.top, rc.right, rc.bottom, NULL, NULL, hInst, NULL);
    if (hwndFrame == 0) {
        return 0;
    }
    hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDA_MAIN));
    if (hAccel == 0) {
        return 0;
    }
    hAccelTitle = LoadAccelerators(hInst, MAKEINTRESOURCE(IDA_TITLE));
    if (hAccelTitle == 0) {
        return 0;
    }
    if (nCmdShow != 1) {
        sw = nCmdShow;
    } else {
        sw = ini.wnFrame.fMaximized != 0 || ini.wnFrame.fMinimized != 0 ? 3 : 1;
    }
    ShowWindow(hwndFrame, sw);
    ShowWindow(hwndFrame, SW_HIDE);
    return 1;
}
