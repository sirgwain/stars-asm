#include "common.h"

ITEMACTION rgiaUnloadAllCol[5] = {{0}, {0}, {0}, {.iAction = iActionUnloadAll}};
ITEMACTION rgiaQuikDrop[5] = {{.iAction = iActionUnloadAll},
                              {.iAction = iActionUnloadAll},
                              {.iAction = iActionUnloadAll},
                              {.iAction = iActionUnloadAll},
                              {.iAction = iActionLoadDunnage}};
ITEMACTION rgiaQuikLoad[5] = {{.iAction = iActionLoadAll}, {.iAction = iActionLoadAll}, {.iAction = iActionLoadAll}, {0}, {.iAction = iActionLoadDunnage}};
ITEMACTION rgiaLoadAllCol[5] = {{0}, {0}, {0}, {.iAction = iActionLoadAll}};
ZIPPRODQ1  rgzpqTut[2] = {{
                              .fNoResearch = 1,
                              .cpq = 2,
                              .rgpq = {{.w = 193, .mdIdle = 1, .cQuan = 3}, {.w = 192, .cQuan = 3}},
                         },
                          {
                              .fNoResearch = 1,
                              .cpq = 3,
                              .rgpq = {{.w = 132, .mdIdle = 4, .cQuan = 2}, {.w = 193, .mdIdle = 1, .cQuan = 3}, {.w = 192, .cQuan = 3}},
                         }};

INT_PTR CALLBACK TutorDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HMENU   hmenu;
    RECT    rc;
    FARPROC lpProc;
    int16_t fRet;

    switch (message) {
    case WM_INITDIALOG:
        tutor.hwnd = hwnd;
        SetWindowPos(hwnd, (HWND)-1, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        StickyDlgPos(hwnd, &ptStickyTutorDlg, 1);
        return 1;
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        DrawTutorText(hwnd);
        return 1;
    case WM_CHAR:
        PostMessage(hwndFrame, WM_CHAR, wParam, lParam);
        return 0;
    case WM_COMMAND:
        switch (GET_WM_COMMAND_ID(wParam, lParam)) {
        case 0x9c7:
            lpProc = MakeProcInstance(PanicDlg, hInst);
            fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_PANIC), hwnd, lpProc);
            FreeProcInstance(lpProc);
            if (fRet == 0) {
                return 0;
            }
            tutor.fAutoComplete = fRet == 2506 ? 1 : 0;
            PostMessage(hwndFrame, WM_STARS_CONTINUE, fRet, 0);
            return 0;
        case IDCANCEL:
            ShowTutor(0);
            if (tutor.fShowHidMsg != 0) {
                AlertSz(PszFormatIds(idsMakeTutorialReappearCompleteTaskChooseTutorial, NULL), MB_ICONASTERISK);
                tutor.fShowHidMsg = 0;
            }
            return 1;
        case IDC_HELP:
            WinHelp(hwnd, szHelpFile, 1, tutor.idh);
            return 1;
        default:
            return 0;
        }
    case WM_DESTROY:
        StickyDlgPos(hwnd, &ptStickyTutorDlg, 0);
        tutor.hwnd = 0;
        hmenu = GetASubMenu(hwndFrame, 5);
        CheckMenuItem(hmenu, 2501, MF_UNCHECKED);
        EndDialog(hwnd, 1);
        EndTutor(1);
        return 1;
    default:
        return 0;
    }
}

INT_PTR CALLBACK PanicDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    RECT rc;

    if (message != WM_ERASEBKGND) {
        if (IS_WM_CTLCOLOR(message) != 0) {
            if (HIWORD(lParam) == 6) {
                SetBkColor((HDC)wParam, crButtonFace);
                return (INT_PTR)hbrButtonFace;
            }
        } else if (message == WM_COMMAND) {
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDCANCEL:
                EndDialog(hwnd, 0);
                return 1;
            case IDC_HELP:
                WinHelp(hwnd, szHelpFile, 1, tutor.idh);
                return 1;
            case 0x9c9:
            case 0x9ca:
                EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam));
                return 1;
            }
        }
        return 0;
    }
    GetClientRect(hwnd, &rc);
    FillRect((HDC)wParam, &rc, hbrButtonFace);
    return 1;
}

void ShowTutor(int16_t fShow) {
    if (tutor.hwnd != 0) {
        ShowWindow(tutor.hwnd, fShow == 0 ? SW_HIDE : SW_SHOW);
        tutor.fVisible = fShow;
    }
    return;
}

void DrawTutorText(HWND hwnd) {
    HDC         hdc;
    int16_t     yTop;
    int16_t     fPara;
    PAINTSTRUCT ps;
    int16_t     didt;
    int16_t     xLeft;
    int16_t     cch;
    char        rgch[256];
    RECT        rcBtn;
    RECT        rc;

    hdc = BeginPaint(hwnd, &ps);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[0]);
    GetWindowRect(hwnd, &rc);
    GetWindowRect(GetDlgItem(hwnd, IDCANCEL), &rcBtn);
    rc.bottom = rcBtn.top;
    ScreenToClient(hwnd, (POINT *)&rc);
    ScreenToClient(hwnd, (POINT *)&rc.right);
    rc.top += dyArial8 * 2;
    rc.bottom -= (int16_t)(dyArial8 * 2) / 3;
    rc.left += (int16_t)(dyArial8 * 2) / 3;
    rc.right -= (int16_t)(dyArial8 * 2) / 3;
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, rc.left, rc.top, rc.right - rc.left, 1, PATCOPY);
    PatBlt(hdc, rc.left, rc.top, 1, rc.bottom - rc.top, PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, rc.left, rc.bottom - 1, rc.right - rc.left, 1, PATCOPY);
    PatBlt(hdc, rc.right - 1, rc.top, 1, rc.bottom - rc.top, PATCOPY);
    ExpandRc(&rc, -(dyArial8 / 2), -(dyArial8 / 2));
    yTop = rc.top;
    FillRect(hdc, &rc, hbrButtonFace);
    SetTextColor(hdc, crButtonText);
    for (didt = 0; didt < 8; didt++) {
        cch = CchTutorString(rgch, tutor.idt + didt);
        if (cch == 1)
            break;
        fPara = isupper((int16_t)(int8_t)rgch[0]);
        if (fPara != 0) {
            xLeft = rc.left;
            if (didt != 0) {
                yTop += dyArial8 / 2;
            }
        }
        if (tutor.idt + didt == tutor.idtBold) {
            SetTextColor(hdc, crButtonFace);
            SetBkColor(hdc, crButtonText);
        }
        WrapTextOut(hdc, &xLeft, &yTop, rgch, cch, rc.left, rc.right - rc.left, NULL, didt != 0 && fPara != 0, 1);
        if (tutor.idt + didt == tutor.idtBold) {
            SetTextColor(hdc, crButtonText);
            SetBkColor(hdc, crButtonFace);
        }
    }
    EndPaint(hwnd, &ps);
    return;
}

void StartTutor(int16_t fRestart) {
    int16_t cx;
    int16_t cch;

    if (gd.fTutorial == 0) {
        memset(&tutor, 0, sizeof(TUTOR));
        if (lpfnTutorDlgProc == 0) {
            lpfnTutorDlgProc = MakeProcInstance(TutorDlg, hInst);
            if (lpfnTutorDlgProc == 0) {
                AlertSz(PszFormatIds(idsMemory, NULL), MB_ICONHAND);
                return;
            }
        }
        gd.fTutorial = 1;
        SaveGameState();
        if (fFreeingTitle == 0) {
            fFreeingTitle = 1;
            DestroyWindow(hwndTitle);
            hwndTitle = 0;
            ShowWindow(hwndFrame, SW_SHOW);
        }
        grbitScan = 1248;
        cx = GetSystemMetrics(SM_CXSCREEN);
        if (cx >= 1280) {
            iScanZoom = 3;
        } else if (cx >= 1024) {
            iScanZoom = 2;
        } else if (cx >= 800) {
            iScanZoom = 1;
        } else {
            iScanZoom = 0;
        }
        if (game.lid == 0) {
            cch = CchGetString(idsTutorial, szBase);
            if (fRestart == 0) {
                strcat(szBase, ".xy");
                if (access(szBase, 0) != -1 &&
                    AlertSz(PszFormatIds(idsTutorialHasRunBeforeWouldLikeDestroy, NULL), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) != IDYES) {
                    szBase[cch] = 0;
                    strcat(szBase, ".m1");
                    ini.fStartupFile = 1;
                    if (FOpenGame(hwndFrame, 0) > 0) {
                        CreateChildWindows();
                    }
                    ini.fStartupFile = 0;
                }
                szBase[cch] = 0;
            }
            if (game.lid == 0) {
                CreateTutorWorld();
                memset((uint8_t *)(ZIPPRODQ *)vrgZipProd + 14, 0, 26);
                vrgZipProd[0].fValid = 1;
                gd.fChgZipProd = 1;
            }
            InitializeMenu(NULL);
            PostMessage(hwndFrame, WM_COMMAND, 0xfa1, 0);
            if (fFreeingTitle == 0) {
                fFreeingTitle = 1;
                DestroyWindow(hwndTitle);
                hwndTitle = 0;
            }
            ShowWindow(hwndFrame, SW_SHOW);
        }
        tutor.idsError = -1;
        tutor.fShowHidMsg = 1;
        tutor.idt = 0;
        tutor.idtBold = 0;
        tutor.fProgress = 0;
        while (FTutorTaskDone() != 0 && tutor.fTurnDone == 0) {
            tutor.idt += 8;
        }
        if (tutor.fTutorDone != 0) {
            EndTutor(0);
            return;
        }
        if (tutor.hwnd == 0) {
            CreateDialog(hInst, MAKEINTRESOURCE(IDD_TUTOR), hwndFrame, lpfnTutorDlgProc);
        }
        if (tutor.idt != 0) {
            if (tutor.fTurnDone == 0) {
                tutor.idt -= 8;
            }
            tutor.idtBold = tutor.idt;
            AdvanceTutor();
        }
    }
    ShowTutor(1);
    InvalidateRect(tutor.hwnd, NULL, 1);
    return;
}

void AdvanceTutor() {
    char    szTitle[50];
    int16_t fRedraw;
    int16_t idtT;
    int16_t fTaskDone;
    RECT    rc;

    fRedraw = 0;
    tutor.fChange = 0;
    idtT = tutor.idtBold;
    fTaskDone = FTutorTaskDone();
    fRedraw = idtT == tutor.idtBold ? 0 : 1;
    if (fTaskDone == 0) {
        if (fRedraw == 0) {
            return;
        }
    } else {
        if (tutor.fTurnDone == 0) {
            do {
                tutor.idt += 8;
                tutor.idtBold = tutor.idt;
                tutor.idsError = -1;
                tutor.fNoErrors = 1;
                tutor.fProgress = 0;
            } while (FTutorTaskDone() != 0 && tutor.fTurnDone == 0);
            fRedraw = 1;
            tutor.fNoErrors = 0;
        } else {
            tutor.idh = 3510;
        }
        if (tutor.idt >= 640 || tutor.fTutorDone != 0) {
            if (tutor.idsError != 522) {
                TutorError(522);
            }
            EndTutor(0);
            return;
        }
        if (fRedraw == 0) {
            return;
        }
    }
    _wsprintf(szTitle, PszGetCompressedString(idsStarsTutorPageD80), tutor.idt / 8 + 1);
    SetWindowText(tutor.hwnd, szTitle);
    ShowTutor(1);
    GetWindowRect(tutor.hwnd, &rc);
    ScreenToClient(tutor.hwnd, (POINT *)&rc);
    ScreenToClient(tutor.hwnd, (POINT *)&rc.right);
    ExpandRc(&rc, -dyArial8, -2 * dyArial8);
    InvalidateRect(tutor.hwnd, &rc, 1);
    return;
}

void EndTutor(int16_t fClose) {
    if (gd.fTutorial != 0) {
        gd.fTutorial = 0;
        if (tutor.hwnd != 0) {
            DestroyWindow(tutor.hwnd);
        }
        game.fTutorial = 0;
        if (fClose != 0) {
            RestoreGameState();
        } else {
            tutor.fFreeing = 1;
        }
        memset(&tutor, 0, sizeof(TUTOR));
        Randomize2(GetTickCount());
    }
    return;
}

void SaveGameState() {
    HMENU hmenu;

    tutor.fGameSaved = 0;
    tutor.grbitScan = grbitScan;
    tutor.iScanZoom = iScanZoom;
    tutor.fTBVis = gd.fToolbar;
    tutor.zpq = vrgZipProd[0].zpq1;
    tutor.fValidQ = vrgZipProd[0].fValid;
    vrgZipProd[0].zpq1 = vrgZipProd[4].zpq1;
    vrgZipProd[0].fValid = vrgZipProd[4].fValid;
    if (gd.fToolbar == 0) {
        hmenu = GetASubMenu(hwndFrame, 1);
        gd.fToolbar = gd.fToolbar == 0 ? 1 : 0;
        CheckMenuItem(hmenu, 179, gd.fToolbar == 0 ? MF_UNCHECKED : MF_CHECKED);
        RefitFrameChildren();
    }
    tutor.icolFSort = vrptFleet.icolSort;
    if (vrptFleet.icolSort != 1 || vrptFleet.fAscending == 0) {
        vrptFleet.icolSort = 1;
        vrptFleet.fAscending = 1;
        InvalidateReport(1, 0);
    }
    if (game.lid != 0 && game.fTutorial == 0) {
        DestroyCurGame();
    }
    return;
}

void RestoreGameState() {
    HMENU hmenu;

    if (tutor.fGameSaved != 0) {
    }
    grbitScan = tutor.grbitScan;
    iScanZoom = tutor.iScanZoom;
    vrgZipProd[4].zpq1 = vrgZipProd[0].zpq1;
    vrgZipProd[4].fValid = vrgZipProd[0].fValid;
    vrgZipProd[0].zpq1 = tutor.zpq;
    vrgZipProd[0].fValid = LOBYTE(tutor.fValidQ);
    if (gd.fToolbar != tutor.fTBVis) {
        hmenu = GetASubMenu(hwndFrame, 1);
        gd.fToolbar = gd.fToolbar == 0 ? 1 : 0;
        CheckMenuItem(hmenu, 179, gd.fToolbar == 0 ? MF_UNCHECKED : MF_CHECKED);
        RefitFrameChildren();
    }
    if (vrptFleet.icolSort != tutor.icolFSort) {
        vrptFleet.icolSort = tutor.icolFSort;
        InvalidateReport(1, 1);
    }
    return;
}

int16_t FAskKillTutor() {
    if (game.turn >= 30 || AlertSz(PszFormatIds(idsCurrentlyRunningStarsTutorialDoWantExit, NULL), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) == IDYES) {
        EndTutor(1);
        return 1;
    }
    return 0;
}

int16_t FTutorTaskDone() {
    HS     hs1;
    HS     hs;
    HS     hs2;
    FLEET *t_call_46b2;

    if (game.turn > 36) {
        tutor.fTurnDone = 1;
        tutor.fTutorDone = 1;
        return 1;
    }
    switch (game.turn) {
    case 0:
        switch (tutor.idt) {
        case 0:
            tutor.idtBold = 5;
            return FCheckMessages(9999, 0xffff, 0);
        case 8:
            tutor.idtBold = 11;
            if (FCheckSelection(grobjFleet, 0) == 0) {
                return 0;
            }
            tutor.idtBold = 15;
            return FCheckFleetWP(0, 1, grobjPlanet, 12, 0, 0xffff);
        case 16:
            tutor.idtBold = 18;
            if (FCheckSelection(grobjFleet, 1) == 0) {
                tutor.idh = 6001;
                return 0;
            }
            tutor.idtBold = 21;
            return FCheckFleetWP(1, 1, grobjPlanet, 16, 0, 0xffff);
        case 24:
            if (FCheckSelection(grobjFleet, 4) != 0) {
                tutor.idtBold = 31;
                return FCheckFleetWP(4, 1, grobjPlanet, 15, 0, 0xffff);
            }
            if (FCheckSelection(grobjFleet, 3) != 0) {
                tutor.idtBold = 29;
            } else if (FCheckSelection(grobjFleet, 2) != 0) {
                tutor.idtBold = 27;
            } else {
                tutor.idtBold = 25;
            }
            return 0;
        case 32:
            if (FCheckSelection(grobjFleet, 4) != 0 && tutor.fProgress == 0 && tutor.fAutoComplete == 0) {
                tutor.idtBold = 32;
            } else if (FCheckSelection(grobjFleet, 5) != 0 && tutor.fProgress == 0 && tutor.fAutoComplete == 0) {
                tutor.idtBold = 34;
            } else {
                if (FCheckResearch(Weapons, TechFieldCount, 15) != 0) {
                    tutor.idtBold = 39;
                    tutor.fTurnDone = 1;
                    return 1;
                }
                tutor.idtBold = pctResGlob == -1 ? 37 : 38;
            }
            return 0;
        default:
            return 0;
        }
    case 1:
        if (tutor.idt != 40) {
            return 1;
        }
        if (FCheckQueue(13, 0, grobjPlanet, 7, 20, 0) == 0) {
            tutor.idtBold = lpplProdGlob == 0 ? 42 : 43;
            return 0;
        }
        tutor.idtBold = 47;
        tutor.fTurnDone = 1;
        return 1;
    case 2:
        switch (tutor.idt) {
        case 48:
            if (FCheckFleetWP(1, 1, grobjPlanet, 21, 0, 0xffff) != 0) {
                return 1;
            }
            tutor.idtBold = FCheckSelection(grobjFleet, 0) == 0 ? 48 : 50;
            if (FCheckFleetWP(0, 1, grobjPlanet, 9, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = 51;
            if (FCheckFleetWP(0, 2, grobjPlanet, 3, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = 52;
            if (FCheckFleetWP(0, 3, grobjPlanet, 8, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = 53;
            if (FCheckFleetWP(0, 4, grobjPlanet, 5, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = 54;
            if (FCheckFleetWP(0, 5, grobjPlanet, 2, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = 55;
            return FCheckSelection(grobjFleet, 1);
        case 56:
            if (FCheckFleetWP(4, 1, grobjPlanet, 14, 0, 0xffff) != 0) {
                return 1;
            }
            tutor.idtBold = 57;
            if (FCheckFleetWP(1, 1, grobjPlanet, 21, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = 58;
            if (FCheckFleetWP(1, 2, grobjPlanet, 19, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = 59;
            if (FCheckFleetWP(1, 3, grobjPlanet, 20, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = 60;
            if (FCheckFleetWP(1, 4, grobjPlanet, 7, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = FCheckMessages(2, 0xffff, 0) == 0 ? 61 : 63;
            return FCheckSelection(grobjFleet, 4);
        case 64:
            if (FCheckFleetWP(5, 1, grobjPlanet, 12, 3, 0xffff) != 0) {
                return 1;
            }
            tutor.idtBold = 64;
            if (FCheckFleetWP(4, 1, grobjPlanet, 14, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = 65;
            if (FCheckFleetWP(4, 2, grobjPlanet, 17, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = 66;
            if (FCheckFleetWP(4, 3, grobjPlanet, 18, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = 67;
            if (FCheckFleetWP(4, 4, grobjPlanet, 23, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = 68;
            if (FCheckFleetWP(4, 5, grobjPlanet, 22, 0, 0xffff) == 0) {
                return 0;
            }
            tutor.idtBold = FCheckMessages(4, 0xffff, 0) == 0 ? 69 : 71;
            return FCheckSummary(grobjPlanet, 12);
        case 72:
            if (FCheckColonizeWP(2, 16, 0xffff) != 0) {
                return 1;
            }
            if (FCheckFleetWP(5, 1, grobjPlanet, 12, 3, 0xffff) != 0) {
                tutor.fProgress = 0;
                return 1;
            }
            if (FCheckFleetWP(5, 1, grobjPlanet, 12, 0, 0xffff) != 0) {
                tutor.idtBold = 79;
            } else if (FCheckSelection(grobjFleet, 5) != 0) {
                tutor.idtBold = 77;
            } else {
                tutor.idtBold = 76;
            }
            return 0;
        case 80:
            if (FCheckColonizeWP(2, 16, 0xffff) != 0) {
                return 1;
            }
            if (FCheckSummary(grobjPlanet, 16) != 0) {
                return 1;
            }
            if (FCheckSummary(grobjPlanet, 15) != 0) {
                if (FCheckMessages(9999, 0xffff, 0) != 0) {
                    tutor.idtBold = 86;
                } else {
                    tutor.idtBold = tutor.fProgress == 0 ? 83 : 84;
                }
            }
            return 0;
        case 88:
            if (FCheckCargo(LpflFromId(2), 0, 0, 0, 25) == 0) {
                if (FCheckSelection(grobjFleet, 2) == 0) {
                    tutor.idtBold = 89;
                } else if (mdXferDlg == mdXferNone) {
                    tutor.idh = 1514;
                    tutor.idtBold = 90;
                } else {
                    tutor.idtBold = 91;
                }
                return 0;
            }
            if (FCheckColonizeWP(2, 16, 0xffff) == 0) {
                if (FCheckFleetWP(2, 1, grobjPlanet, 16, 0, 0xffff) == 0) {
                    tutor.idtBold = FCheckSelection(grobjFleet, 2) == 0 ? 89 : 92;
                } else {
                    tutor.idtBold = 93;
                }
                return 0;
            }
            tutor.idtBold = 95;
            tutor.fTurnDone = 1;
            return 1;
        default:
            return 1;
        }
    case 3:
        switch (tutor.idt) {
        default:
            return 1;
        case 96:
            if (FCheckMessages(-1, idmHaveBuiltFactories, 1) == 0) {
                tutor.idtBold = 97;
                return 0;
            }
            if (FCheckQueue(13, 0, grobjPlanet, 1, 30, 0) == 0) {
                if (FCheckMessages(1, 0xffff, 0) == 0 || FCheckSelection(grobjPlanet, 13) == 0) {
                    tutor.idtBold = 98;
                } else {
                    tutor.idtBold = lpplProdGlob == 0 ? 101 : 102;
                }
                return 0;
            }
            return 1;
        case 104:
            if (FCheckQueue(16, 0, grobjPlanet, 7, 3, 1) == 0 || FCheckQueue(16, 1, grobjPlanet, 8, 3, 1) == 0) {
                if (FCheckMessages(3, 0xffff, 0) == 0 || FCheckSelection(grobjPlanet, 16) == 0) {
                    tutor.idtBold = 104;
                } else {
                    tutor.idtBold = lpplProdGlob == 0 ? 106 : 107;
                }
                return 0;
            }
            if (FCheckCargo(LpflFromId(3), 0, 0, 0, 210) == 0) {
                if (FCheckSelection(grobjFleet, 3) != 0) {
                    tutor.idtBold = mdXferDlg == mdXferNone ? 110 : 111;
                } else {
                    tutor.idtBold = 109;
                }
                return 0;
            }
            return 1;
        case 112:
            if (FCheckXferWP(3, 1, 16, 0xffff, rgiaQuikDrop) == 0) {
                if (FCheckFleetWP(3, 1, grobjPlanet, 16, 1, 0xffff) != 0) {
                    tutor.idtBold = 114;
                } else if (FCheckFleetWP(3, 1, grobjPlanet, 16, 0xffff, 0xffff) == 0) {
                    tutor.idtBold = 112;
                } else {
                    tutor.idtBold = 113;
                }
                return 0;
            }
            if (LpflFromId(0)->cord < 6) {
                return 1;
            }
            if (FCheckSelection(grobjFleet, 0) != 0) {
                return 1;
            }
            tutor.idtBold = FCheckSummary(grobjPlanet, 9) == 0 ? 115 : 118;
            return 0;
        case 120:
            if (LpflFromId(0)->cord == 6) {
                tutor.idtBold = FCheckSummary(grobjPlanet, 9) == 0 ? 121 : 123;
                return 0;
            }
            tutor.idtBold = 127;
            tutor.fTurnDone = 1;
            tutor.fProgress = 0;
            return 1;
        }
    case 4:
        if (tutor.idt != 128) {
            if (tutor.idt != 136) {
                return 1;
            }
            if (LpplFromId(13)->lpplprod->iprodMac != 3 || FCheckQueue(13, 1, grobjFleet, 2, 1, 0) == 0) {
                tutor.idtBold = lpplProdGlob == 0 ? 136 : 137;
                return 0;
            }
            tutor.idtBold = 143;
            tutor.fTurnDone = 1;
            return 1;
        }
        if (LpflFromId(4)->cord == 6) {
            if (FCheckSelection(grobjFleet, 4) != 0) {
                tutor.idtBold = FCheckSummary(grobjPlanet, 14) == 0 ? 131 : 132;
            } else if (FCheckSummary(grobjPlanet, 14) != 0 || FCheckSummary(grobjFleet, 4) != 0) {
                tutor.idtBold = 130;
            } else {
                tutor.idtBold = 128;
            }
            return 0;
        }
        if (LpplFromId(13)->lpplprod->iprodMac == 3) {
            return 1;
        }
        if (FCheckSelection(grobjPlanet, 13) != 0) {
            return 1;
        }
        tutor.idtBold = FCheckSummary(grobjPlanet, 21) == 0 ? 133 : 135;
        return 0;
    case 5:
        if (tutor.idt != 144) {
            if (tutor.idt != 152) {
                return 1;
            }
            if (FCheckColonizeWP(2, 14, 0xffff) == 0) {
                tutor.idtBold = 152;
                return 0;
            }
            if (FCheckFleetWP(3, 1, grobjPlanet, 13, 0xffff, 0xffff) == 0) {
                if (FCheckScanner(0, -1) == 0) {
                    tutor.idtBold = 153;
                } else {
                    tutor.idtBold = FCheckSelection(grobjFleet, 3) == 0 ? 154 : 155;
                }
                return 0;
            }
            if (LpflFromId(0)->cord == 5) {
                if (FCheckMessages(9999, 0xffff, 0) != 0) {
                    tutor.idtBold = 158;
                } else {
                    tutor.idtBold = FCheckSelection(grobjPlanet, 16) == 0 ? 156 : 158;
                }
                return 0;
            }
            tutor.idtBold = 159;
            tutor.fTurnDone = 1;
            return 1;
        }
        if (FCheckCargo(LpflFromId(2), 0, 0, 0, 25) != 0) {
            if (FCheckFleetWP(2, 1, grobjPlanet, 14, 0xffff, 0xffff) == 0) {
                tutor.idtBold = FCheckScanner(3, -1) == 0 ? 150 : 151;
                return 0;
            }
            return 1;
        }
        if (FCheckSelection(grobjFleet, 2) == 0) {
            tutor.idtBold = 144;
        } else {
            tutor.idtBold = mdXferDlg == mdXferNone ? 146 : 148;
        }
        return 0;
    case 6:
        if (tutor.idt != 160) {
            if (tutor.idt != 168) {
                return 1;
            }
            tutor.fNoErrors = 1;
            if (FCheckXferWP(3, 2, 13, 0xffff, rgiaQuikDrop) == 0) {
                tutor.fNoErrors = 0;
                tutor.idtBold = 170;
                return 0;
            }
            tutor.fNoErrors = 0;
            tutor.idh = 1518;
            tutor.idtBold = 171;
            if (LpflFromId(3)->fRepOrders == 0) {
                return 0;
            }
            tutor.idh = 1059;
            if (LpplFromId(13)->lpplprod->iprodMac != 3 || FCheckQueue(13, 1, grobjFleet, 2, 1, 0) == 0) {
                tutor.idtBold = FCheckSelection(grobjPlanet, 13) == 0 ? 173 : 174;
                return 0;
            }
            tutor.idtBold = 175;
            tutor.fTurnDone = 1;
            return 1;
        }
        if (FCheckXferWP(3, 1, 12, 0xffff, rgiaQuikLoad) == 0) {
            if (FCheckFleetWP(3, 1, grobjPlanet, 12, 1, 0xffff) != 0) {
                tutor.idtBold = 165;
            } else if (FCheckFleetWP(3, 1, grobjPlanet, 12, 0xffff, 0xffff) == 0) {
                tutor.idtBold = FCheckSelection(grobjFleet, 3) == 0 ? 160 : 163;
            } else {
                tutor.idtBold = 164;
            }
            return 0;
        }
        tutor.idtBold = 166;
        return FCheckFleetWP(3, 2, grobjPlanet, 13, 0xffff, 0xffff);
    case 7:
        switch (tutor.idt) {
        default:
            return 1;
        case 176:
            if (FCheckCargo(LpflFromId(6), 0, 0, 0, 25) == 0) {
                tutor.idtBold = 176;
                return 0;
            }
            if (FCheckColonizeWP(6, 18, 0xffff) == 0) {
                tutor.idtBold = FCheckScanner(3, -1) == 0 ? 177 : 179;
                return 0;
            }
            tutor.idh = 1507;
            if (LpplFromId(13)->lpplprod->iprodMac < 3 || FCheckQueue(13, 1, grobjFleet, 2, 3, 0) == 0) {
                tutor.idtBold = 181;
                return 0;
            }
            if (FCheckScanner(0, -1) == 0) {
                tutor.idtBold = 182;
                return 0;
            }
            return 1;
        case 184:
            if (FCheckMessages(-1, idmHaveBuiltMines, 1) == 0) {
                tutor.idtBold = FCheckMessages(3, 0xffff, 0) == 0 ? 184 : 186;
                return 0;
            }
            if (FCheckQueue(16, 0, grobjPlanet, 1, 10, 1) == 0 || FCheckQueue(16, 1, grobjPlanet, 0, 10, 1) == 0) {
                if (FCheckSelection(grobjPlanet, 16) == 0) {
                    tutor.idtBold = FCheckMessages(4, 0xffff, 0) == 0 ? 187 : 188;
                } else {
                    tutor.idtBold = lpplProdGlob == 0 ? 188 : 189;
                }
                return 0;
            }
            tutor.idtBold = 190;
            tutor.fProgress = 0;
            return FCheckMessages(9999, 0xffff, 0);
        case 192:
            if (tutor.fProgress == 0 && tutor.fAutoComplete == 0) {
                if (FCheckSummary(grobjFleet, 512) != 0) {
                    tutor.fProgress = 1;
                } else {
                    tutor.idtBold = 192;
                    return 0;
                }
            }
            tutor.idh = 1507;
            if (LpplFromId(13)->lpplprod->iprodMac < 4 || FCheckQueue(13, 2, grobjFleet, 0, 2, 0) == 0) {
                tutor.idtBold = 196;
                return 0;
            }
            tutor.idtBold = 198;
            tutor.fTurnDone = 1;
            return 1;
        }
    case 8:
        switch (tutor.idt) {
        default:
            return 1;
        case 200:
            if (rgplr[0].cFleet == 11 && FCheckColonizeWP(10, 8, 0xffff) != 0) {
                return 1;
            }
            if (rgplr[0].cFleet == 9 || mdXferDlg == mdXferShips) {
                if (FCheckSelection(grobjFleet, 7) == 0) {
                    tutor.idtBold = 200;
                } else {
                    tutor.idtBold = mdXferDlg == mdXferShips ? 203 : 202;
                }
            } else if (FCheckCargo(LpflFromId(7), 0, 0, 0, 50) == 0 || FCheckFleetWP(7, 1, grobjPlanet, 8, 2, 0xffff) == 0) {
                tutor.idtBold = 205;
            } else {
                tutor.idtBold = 206;
            }
            return 0;
        case 208:
            tutor.fNoErrors = 1;
            if (FCheckColonizeWP(7, 17, 0xffff) == 0) {
                tutor.fNoErrors = 0;
                FCheckColonizeWP(7, 8, 0xffff);
                tutor.idtBold = 209;
                return 0;
            }
            tutor.fNoErrors = 0;
            if (FCheckFleetWP(8, 1, grobjPlanet, 9, 0xffff, 0xffff) == 0) {
                tutor.idtBold = FCheckSelection(grobjFleet, 8) == 0 ? 212 : 214;
                return 0;
            }
            tutor.fProgress = 0;
            return 1;
        case 216:
            tutor.fNoErrors = 0;
            if (FCheckMessages(-1, idmHasUnloaded, 1) == 0) {
                tutor.fProgress = 0;
                tutor.idtBold = 216;
                return 0;
            }
            tutor.fNoErrors = 1;
            if (FCheckResearch(Weapons, TechFieldCount, 30) != 0) {
                tutor.fNoErrors = 0;
                return 1;
            }
            if (FCheckMessages(9999, 0xffff, 0) == 0) {
                tutor.idtBold = 217;
                tutor.fProgress = 0;
                return 0;
            }
            if (tutor.fProgress == 0 && tutor.fAutoComplete == 0) {
                if (FCheckSummary(grobjPlanet, 5) != 0) {
                    tutor.idtBold = 219;
                } else {
                    tutor.idtBold = 217;
                }
                return 0;
            }
            tutor.idtBold = 221;
            if (pctResGlob != -1) {
                tutor.fProgress = 0;
                return 1;
            }
            return 0;
        case 224:
            if (pctResGlob != -1) {
                if (tutor.fProgress == 0) {
                    tutor.idtBold = 226;
                } else {
                    tutor.idtBold = 228;
                }
                return 0;
            }
            if (FCheckResearch(Weapons, TechFieldCount, 30) == 0) {
                tutor.idtBold = 228;
                return 0;
            }
            tutor.idtBold = 230;
            tutor.fTurnDone = 1;
            return 1;
        }
    case 9:
        if (tutor.idt != 232) {
            if (tutor.idt != 240) {
                return 1;
            }
            if (FCheckColonizeWP(9, 2, 0xffff) == 0) {
                if (FCheckSelection(grobjFleet, 9) != 0) {
                    tutor.idtBold = FCheckCargo(LpflFromId(9), 0, 0, 0, 25) == 0 ? 242 : 243;
                } else {
                    tutor.idtBold = FCheckSummary(grobjPlanet, 2) == 0 ? 240 : 241;
                }
                return 0;
            }
            tutor.fNoErrors = 1;
            if (FCheckFleetWP(0, 1, grobjPlanet, 4, 0xffff, 0xffff) == 0) {
                FCheckFleetWP(0, 1, grobjPlanet, 2, 0xffff, 0xffff);
                tutor.idtBold = 245;
                tutor.fNoErrors = 0;
                return 0;
            }
            tutor.fNoErrors = 0;
            tutor.idtBold = 247;
            tutor.fTurnDone = 1;
            return 1;
        }
        tutor.fNoErrors = 1;
        if (FCheckResearch(Weapons, Construction, 30) == 0) {
            tutor.idtBold = pctResGlob == -1 ? 232 : 236;
            tutor.fNoErrors = 0;
            return 0;
        }
        tutor.fNoErrors = 0;
        tutor.idtBold = 238;
        return FCheckMessages(-1, idmHasLoadedMiningRobotsWorking, 1);
    case 10:
        if (tutor.idt != 248) {
            if (tutor.idt != 256) {
                return 1;
            }
            if (FCheckTemplate(0) == 0) {
                tutor.idtBold = 256;
                return 0;
            }
            if (tutor.fProgress == 0 && lpplProdGlob != 0) {
                tutor.idtBold = 257;
                return 0;
            }
            if (LpplFromId(13)->lpplprod->iprodMac < 3 || FCheckQueue(13, 1, grobjFleet, 2, 1, 0) == 0) {
                tutor.idh = 1059;
                if (tutor.fProgress != 0 || FCheckSelection(grobjPlanet, 13) != 0) {
                    tutor.idtBold = 260;
                } else if (FCheckSummary(grobjPlanet, 23) != 0) {
                    tutor.fProgress = 1;
                    tutor.idtBold = 260;
                } else {
                    tutor.idtBold = 259;
                }
                return 0;
            }
            if (LpplFromId(13)->lpplprod->iprodMac < 4 || FCheckQueue(13, 2, grobjFleet, 3, 1, 0) == 0) {
                tutor.idh = 1059;
                tutor.idtBold = 262;
                return 0;
            }
            tutor.idtBold = 263;
            tutor.fTurnDone = 1;
            return 1;
        }
        if (FCheckMessages(-1, idmHasDismantledKtMineralsWhichHaveDeposited, 1) == 0) {
            tutor.idtBold = 248;
            tutor.fProgress = 0;
            return 0;
        }
        if (FCheckQueue(14, 0, grobjPlanet, 1, 3, 1) == 0 || FCheckQueue(14, 1, grobjPlanet, 0, 3, 1) == 0) {
            if (FCheckSelection(grobjPlanet, 14) != 0) {
                tutor.idtBold = lpplProdGlob == 0 ? 250 : 251;
            } else {
                tutor.idtBold = 249;
            }
            return 0;
        }
        if (FCheckTemplate(0) != 0) {
            return 1;
        }
        if (lpplProdGlob == 0) {
            tutor.idtBold = 254;
            return 0;
        }
        if (vyZPDStatic != -1) {
            return 1;
        }
        tutor.idtBold = 255;
        return 0;
    case 11:
        switch (tutor.idt) {
        default:
            return 1;
        case 264:
            if (FCheckFleetWP(0, 1, grobjPlanet, 10, 0xffff, 0xffff) == 0) {
                tutor.idtBold = FCheckSelection(grobjFleet, 0) == 0 ? 264 : 265;
                return 0;
            }
            if (FCheckFleetWP(1, 1, grobjPlanet, 13, 5, 0xffff) == 0) {
                tutor.idtBold = FCheckSelection(grobjFleet, 1) == 0 ? 266 : 268;
                return 0;
            }
            if (FCheckFleetWP(4, 1, grobjPlanet, 13, 0xffff, 0xffff) == 0) {
                tutor.idtBold = 269;
                return 0;
            }
            if (FCheckFleetWP(8, 1, grobjFleet, 512, 0xffff, 0xffff) == 0) {
                tutor.idtBold = 271;
                return 0;
            }
            return 1;
        case 272:
            if (FCheckColonizeWP(2, 4, 0xffff) == 0) {
                tutor.idtBold = FCheckSelection(grobjFleet, 2) == 0 ? 272 : 274;
                return 0;
            }
            if (FCheckCargo(LpflFromId(11), 0, 0, 0, 210) == 0) {
                tutor.idtBold = FCheckSelection(grobjFleet, 11) == 0 ? 275 : 276;
                return 0;
            }
            if (FCheckFleetWP(11, 1, grobjPlanet, 5, 0xffff, 0xffff) == 0) {
                tutor.idtBold = 276;
            } else if (FCheckFleetWP(11, 1, grobjPlanet, 5, 1, 0xffff) != 0) {
                if (FCheckXferWP(11, 1, 5, 0xffff, rgiaUnloadAllCol) != 0) {
                    tutor.fProgress = 0;
                    return 1;
                }
                if (FCheckSelection(grobjFleet, 11) != 0 && sel.fl.lpplord->rgord[sel.iwpAct].grTask == grTaskXfer &&
                    SendMessage(rghwndOrderDD[1], CB_GETCURSEL, 0, 0) == 4) {
                    tutor.idtBold = 279;
                } else {
                    tutor.idtBold = 278;
                }
            } else {
                tutor.idtBold = 277;
            }
            return 0;
        case 280:
            if (LpplFromId(13)->lpplprod->iprodMac < 2 || FCheckQueue(13, 0, grobjPlanet, 8, 70, 0) == 0) {
                tutor.idtBold = 280;
            } else if (FCheckResearch(Construction, Biotechnology, 30) == 0) {
                tutor.idtBold = pctResGlob == -1 ? 281 : 282;
            } else {
                if (FCheckSelection(grobjFleet, 8) != 0) {
                    return 1;
                }
                if (tutor.fProgress != 0 && FCheckMessages(11, 0xffff, 0) != 0 && hwndBrowser == 0) {
                    if (FCheckMessages(13, 0xffff, 0) == 0) {
                        tutor.idtBold = 285;
                    } else {
                        tutor.idtBold = 286;
                    }
                } else if (hwndBrowser == 0) {
                    tutor.idtBold = 283;
                } else {
                    tutor.idtBold = 284;
                    tutor.fProgress = 1;
                }
            }
            return 0;
        case 288:
            if (tutor.fProgress == 0 && tutor.fAutoComplete == 0) {
                tutor.idtBold = vrgtok == 0 ? 289 : 290;
                return 0;
            }
            tutor.idtBold = FCheckMessages(9999, 0xffff, 0) == 0 ? 291 : 295;
            tutor.fTurnDone = 1;
            return 1;
        }
    case 12:
        switch (tutor.idt) {
        default:
            return 1;
        case 296:
            if (LpplFromId(8)->lpplprod->iprodMac > 2) {
                return 1;
            }
            if (FCheckSummary(grobjFleet, 3) != 0) {
                return 1;
            }
            if (FCheckSummary(grobjThing, -1) != 0) {
                tutor.idtBold = 303;
            } else if (FCheckMessages(1, 0xffff, 0) != 0 && FCheckSelection(grobjFleet, 8) != 0) {
                tutor.idtBold = 300;
            } else {
                tutor.idtBold = 297;
            }
            return 0;
        case 304:
            if (FCheckMessages(5, 0xffff, 0) == 0) {
                tutor.idtBold = 306;
                return 0;
            }
            if (FCheckMessages(6, 0xffff, 0) == 0) {
                tutor.idtBold = 308;
                return 0;
            }
            tutor.idtBold = 310;
            return FCheckSelection(grobjPlanet, 18);
        case 312:
            if (LpplFromId(13)->lpplprod->iprodMac < 3 || FCheckQueue(13, 1, grobjFleet, 3, 2, 0) == 0) {
                if (LpplFromId(8)->lpplprod->iprodMac < 3 || FCheckQueue(8, 0, grobjPlanet, 12, 2, 1) == 0) {
                    tutor.idtBold = FCheckSelection(grobjPlanet, 8) == 0 ? 313 : 316;
                } else {
                    tutor.idtBold = 318;
                }
                return 0;
            }
            tutor.idtBold = 319;
            tutor.fTurnDone = 1;
            return 1;
        }
    case 13:
        switch (tutor.idt) {
        default:
            return 1;
        case 320:
            if (rgplr[0].cShDef == 7) {
                return 1;
            }
            if (FCheckXferWP(0, 1, 8, 0xffff, rgiaUnloadAllCol) == 0) {
                if (FCheckCargo(LpflFromId(0), 0, 0, 0, 210) == 0) {
                    tutor.idtBold = 320;
                } else {
                    tutor.idtBold = 321;
                }
                tutor.fProgress = 0;
                return 0;
            }
            if (FCheckMessages(3, 0xffff, 0) == 0) {
                tutor.idtBold = 322;
                return 0;
            }
            if (FCheckResearch(Biotechnology, Propulsion, 30) == 0) {
                tutor.idtBold = 324;
                return 0;
            }
            if (tutor.fProgress == 0 && tutor.fAutoComplete == 0) {
                if (hwndBrowser == 0) {
                    tutor.idtBold = 325;
                } else {
                    tutor.idtBold = 327;
                    tutor.fProgress = 1;
                }
                return 0;
            }
            tutor.idtBold = 327;
            if (hwndSlotDlg != 0) {
                return 1;
            }
            return 0;
        case 328:
            hs.grhst = hstEngine;
            hs.iItem = 3;
            hs.cItem = 1;
            hs1.grhst = hstScanner;
            hs1.iItem = 1;
            hs1.cItem = 1;
            if (hwndSlotDlg == 0 && rgplr[0].cShDef == 7) {
                return 1;
            }
            if (FCheckShipBuilder(4, -1) == 0) {
                if (FCheckShipBuilder(1, 7) != 0) {
                    tutor.idtBold = 330;
                } else {
                    tutor.idtBold = FCheckShipBuilder(1, -1) == 0 ? 328 : 329;
                }
                return 0;
            }
            tutor.idh = 3039;
            if (lpshdefBuild->hul.rghs[0].cItem == 0 || FCheckBuilderPart(0, &hs, 1) == 0) {
                tutor.idtBold = 332;
                return 0;
            }
            if (lpshdefBuild->hul.rghs[1].cItem == 0 || FCheckBuilderPart(1, &hs1, 1) == 0) {
                tutor.idtBold = 333;
                return 0;
            }
            if (lpshdefBuild->hul.rghs[2].cItem == 0 || lpshdefBuild->hul.rghs[3].cItem == 0) {
                if (FCheckShipBuilder(4, 8) == 0) {
                    tutor.idtBold = 334;
                    return 0;
                }
                tutor.idtBold = 335;
                return 0;
            }
            return 1;
        case 336:
            if (hwndSlotDlg != 0) {
                tutor.idtBold = FCheckShipBuilder(4, -1) == 0 ? 338 : 337;
                return 0;
            }
            if (LpplFromId(13)->lpplprod->iprodMac < 3 || (LpplFromId(13)->lpplprod->iprodMac == 3 && FCheckQueue(13, 1, grobjFleet, 6, 1, 0) == 0)) {
                tutor.idtBold = 339;
                return 0;
            }
            if (LpplFromId(13)->lpplprod->iprodMac < 4 || FCheckQueue(13, 0, grobjPlanet, 8, 100, 0) == 0) {
                tutor.idtBold = lpplProdGlob == 0 ? 341 : 342;
                tutor.fProgress = 0;
                return 0;
            }
            return 1;
        case 344:
            if (tutor.fProgress == 0 && tutor.fAutoComplete == 0) {
                tutor.idtBold = 344;
                return 0;
            }
            if (FCheckMessages(9999, 0xffff, 0) == 0) {
                tutor.idtBold = 345;
                return 0;
            }
            if (FCheckFleetWP(8, 1, grobjPlanet, 13, 0xffff, 0xffff) == 0) {
                tutor.idtBold = 348;
                return 0;
            }
            tutor.idtBold = 350;
            tutor.fTurnDone = 1;
            return 1;
        }
    case 14:
        if (tutor.idt != 352) {
            return 1;
        }
        if (LpplFromId(2)->lpplprod->iprodMac < 3 || FCheckQueue(2, 0, grobjPlanet, 4, 2, 1) == 0 || FCheckTemplate(1) == 0) {
            if (FCheckSelection(grobjPlanet, 2) != 0) {
                tutor.idtBold = 356;
            } else if (FCheckSelection(grobjPlanet, 17) != 0) {
                tutor.idtBold = 354;
            } else {
                tutor.idtBold = 352;
            }
            return 0;
        }
        tutor.idtBold = 358;
        tutor.fTurnDone = 1;
        return 1;
    case 15:
        if (tutor.idt != 360) {
            if (tutor.idt != 368) {
                return 1;
            }
            if (tutor.fAutoComplete == 0 && tutor.fProgress == 0) {
                if (pctResGlob != -1) {
                    tutor.fProgress = 1;
                    tutor.idtBold = 371;
                } else {
                    tutor.idtBold = 368;
                }
                return 0;
            }
            if (FCheckFleetWP(11, 1, grobjPlanet, 13, 0xffff, 0xffff) == 0) {
                tutor.idtBold = 372;
                return 0;
            }
            tutor.idtBold = 374;
            tutor.fTurnDone = 1;
            return 1;
        }
        if (FCheckFleetWP(8, 1, grobjPlanet, 11, 0xffff, 0xffff) == 0) {
            tutor.idtBold = 360;
            return 0;
        }
        if (FCheckCargo(LpflFromId(6), 0, 0, 0, 210) == 0) {
            tutor.idtBold = 361;
            tutor.fProgress = 0;
            return 0;
        }
        if (tutor.fAutoComplete == 0 && tutor.fProgress == 0) {
            if (vprptCur == 0) {
                tutor.idtBold = 363;
            } else {
                tutor.fProgress = 1;
                if (vprptCur->icolSort == 4) {
                    tutor.idtBold = 366;
                } else {
                    tutor.idtBold = 364;
                }
            }
            return 0;
        }
        if (FCheckXferWP(6, 1, 5, 0xffff, rgiaUnloadAllCol) == 0) {
            tutor.idtBold = 367;
            return 0;
        }
        return 1;
    case 16:
        if (tutor.idt != 376) {
            return 1;
        }
        if (FCheckFleetWP(4, 1, grobjPlanet, 5, 0xffff, 0xffff) == 0) {
            tutor.idtBold = 376;
            return 0;
        }
        tutor.fNoErrors = 1;
        if (LpflFromId(7)->cord == 1) {
            tutor.idtBold = FCheckSelection(grobjFleet, 7) == 0 ? 377 : 378;
            return 0;
        }
        if (FCheckFleetWP(7, 1, grobjPlanet, 12, 0xffff, 0xffff) != 0) {
            tutor.fNoErrors = 0;
            tutor.idtBold = 378;
            return 0;
        }
        tutor.fNoErrors = 0;
        if (FCheckFleetWP(7, 1, grobjFleet, 5, 4, 0xffff) == 0) {
            tutor.idtBold = 378;
            return 0;
        }
        if (LpplFromId(13)->lpplprod->iprodMac < 2 || FCheckQueue(13, 0, grobjPlanet, 1, 60, 0) == 0 || FCheckQueue(13, 1, grobjPlanet, 0, 60, 0) == 0) {
            tutor.idtBold = lpplProdGlob == 0 ? 380 : 381;
            return 0;
        }
        if (FCheckResearch(Propulsion, Construction, 30) == 0) {
            tutor.idtBold = 382;
            return 0;
        }
        tutor.idtBold = 383;
        tutor.fTurnDone = 1;
        return 1;
    case 17:
        if (tutor.idt != 384) {
            return 1;
        }
        if (FCheckFleetWP(0, 1, grobjPlanet, 13, 0xffff, 0xffff) == 0) {
            tutor.idtBold = FCheckMessages(9999, 0xffff, 0) == 0 ? 384 : 385;
            return 0;
        }
        tutor.idtBold = 387;
        tutor.fTurnDone = 1;
        return 1;
    case 18:
        if (tutor.idt != 392) {
            return 1;
        }
        if (LpplFromId(13)->lpplprod->iprodMac < 3 || FCheckQueue(13, 0, grobjFleet, 3, 1, 0) == 0) {
            tutor.idtBold = 392;
            return 0;
        }
        if (FCheckMessages(5, 0xffff, 0) == 0) {
            tutor.fProgress = 0;
            tutor.idtBold = 393;
            return 0;
        }
        if (tutor.fAutoComplete == 0 && tutor.fProgress == 0) {
            if (FCheckSelection(grobjPlanet, 16) == 0) {
                tutor.idtBold = 393;
            } else if (pctResGlob != -1) {
                tutor.idtBold = 396;
            } else {
                tutor.idtBold = 395;
            }
            return 0;
        }
        if (pctResGlob != -1) {
            tutor.idtBold = 398;
            return 0;
        }
        tutor.idtBold = 399;
        tutor.fTurnDone = 1;
        return 1;
    case 19:
        if (tutor.idt != 400) {
            if (tutor.idt != 408) {
                return 1;
            }
            if (FCheckXferWP(1, 1, 2, 0xffff, rgiaUnloadAllCol) == 0) {
                tutor.idtBold = 408;
                return 0;
            }
            tutor.fNoErrors = 1;
            if (FCheckXferWP(1, 2, 13, 0xffff, rgiaLoadAllCol) == 0) {
                tutor.fNoErrors = 0;
                tutor.idtBold = 409;
                return 0;
            }
            tutor.fNoErrors = 0;
            tutor.idh = 1518;
            tutor.idtBold = 410;
            if (LpflFromId(1)->fRepOrders == 0) {
                return 0;
            }
            if (LpplFromId(16)->lpplprod->iprodMac < 3 || FCheckQueue(16, 2, grobjPlanet, 5, 1, 1) == 0) {
                tutor.idtBold = 412;
                return 0;
            }
            if (FCheckFleetWP(6, 1, grobjPlanet, 13, 0xffff, 0xffff) == 0) {
                tutor.idtBold = 413;
                return 0;
            }
            tutor.idtBold = 414;
            tutor.fTurnDone = 1;
            return 1;
        }
        if (FCheckCargo(LpflFromId(11), 0, 0, 0, 210) == 0) {
            tutor.idtBold = 400;
            return 0;
        }
        if (FCheckXferWP(11, 1, 5, 0xffff, rgiaUnloadAllCol) == 0) {
            tutor.idtBold = 401;
            return 0;
        }
        tutor.fNoErrors = 1;
        if (FCheckXferWP(11, 2, 13, 0xffff, rgiaLoadAllCol) == 0) {
            tutor.fNoErrors = 0;
            tutor.idtBold = 402;
            return 0;
        }
        tutor.fNoErrors = 0;
        tutor.idh = 1518;
        tutor.idtBold = 403;
        if (LpflFromId(11)->fRepOrders == 0) {
            return 0;
        }
        if (FCheckCargo(LpflFromId(1), 0, 0, 0, 210) == 0) {
            tutor.idtBold = FCheckSelection(grobjFleet, 1) == 0 ? 406 : 407;
            return 0;
        }
        return 1;
    case 20:
        if (tutor.idt != 416) {
            return 1;
        }
        if (FCheckFleetWP(8, 1, grobjPlanet, 6, 0xffff, 0xffff) == 0) {
            tutor.idtBold = 416;
            return 0;
        }
        if (FCheckResearch(Construction, Weapons, 30) == 0) {
            tutor.idtBold = 417;
            return 0;
        }
        if (rgplr[0].cshdefSB == 1 || hwndSlotDlg != 0) {
            if (hwndSlotDlg == 0 && rgplr[0].cshdefSB == 1) {
                tutor.idtBold = 418;
            } else if ((FCheckShipBuilder(4, -1) == 0 || fStarbaseMode == 0) && rgplr[0].cshdefSB == 1) {
                tutor.idtBold = 419;
            } else if (lpshdefBuild->hul.rghs[0].cItem == 0) {
                tutor.idtBold = 420;
            } else {
                tutor.idtBold = 421;
            }
            return 0;
        }
        if (LpplFromId(13)->lpplprod->iprodMac < 4 || FCheckQueue(13, 1, grobjFleet, 17, 1, 0) == 0) {
            tutor.idtBold = 422;
            return 0;
        }
        tutor.idtBold = 423;
        tutor.fTurnDone = 1;
        return 1;
    case 21:
        if (tutor.idt != 424) {
            if (tutor.idt != 432) {
                return 1;
            }
            if (FCheckMessages(9999, 0xffff, 0) == 0) {
                tutor.idtBold = 432;
                return 0;
            }
            if (tutor.fAutoComplete == 0 && tutor.fProgress == 0) {
                if (vprptCur == 0) {
                    tutor.idtBold = 434;
                } else if (vprptCur->icolSort == 11 && vprptCur->fAscending == 0 && vprptCur->iSubsort == 3) {
                    tutor.idtBold = 439;
                    tutor.fProgress = 1;
                } else {
                    tutor.idtBold = 435;
                }
                return 0;
            }
            tutor.idtBold = 439;
            tutor.fTurnDone = 1;
            return 1;
        }
        if (FCheckCargo(LpflFromId(0), 0, 0, 0, 210) == 0) {
            tutor.idtBold = 424;
            return 0;
        }
        if (FCheckXferWP(0, 1, 5, 0xffff, rgiaUnloadAllCol) == 0) {
            tutor.idtBold = 425;
            return 0;
        }
        if (FCheckZip(0, rgiaUnloadAllCol, idsDropcol) == 0) {
            tutor.idtBold = hwndZipOrderDlg == 0 ? 427 : 428;
            return 0;
        }
        tutor.fNoErrors = 1;
        if (FCheckXferWP(0, 2, 13, 0xffff, rgiaLoadAllCol) == 0) {
            tutor.fNoErrors = 0;
            tutor.idtBold = 430;
            return 0;
        }
        tutor.fNoErrors = 0;
        tutor.idh = 1518;
        tutor.idtBold = 431;
        if (LpflFromId(0)->fRepOrders == 0) {
            return 0;
        }
        return 1;
    case 22:
        switch (tutor.idt) {
        default:
            return 1;
        case 440:
            t_call_46b2 = LpflFromId(3);
            if (LOWORD(t_call_46b2->rgwtMin[4]) != 383 || HIWORD(t_call_46b2->rgwtMin[4]) != 0) {
                if (FCheckSelection(grobjFleet, 3) == 0) {
                    tutor.idtBold = 440;
                } else {
                    tutor.idtBold = 445;
                    tutor.idh = 1517;
                }
                return 0;
            }
            return 1;
        case 448:
            if (LpplFromId(13)->lpplprod->iprodMac < 4 || FCheckQueue(13, 1, grobjFleet, 3, 1, 0) == 0) {
                tutor.idtBold = 448;
                return 0;
            }
            if (FCheckResearch(Weapons, Propulsion, 30) == 0) {
                tutor.idtBold = 449;
                return 0;
            }
            if (FCheckMessages(9999, 0xffff, 0) == 0) {
                tutor.idtBold = 450;
                return 0;
            }
            if (rgplr[0].cShDef < 8 || hwndSlotDlg != 0) {
                hs.grhst = hstEngine;
                hs.iItem = 4;
                hs.cItem = 1;
                hs1.grhst = hstMines;
                hs1.iItem = 1;
                hs1.cItem = 3;
                if (hwndSlotDlg == 0) {
                    tutor.idtBold = 450;
                    tutor.idh = 1001;
                    return 0;
                }
                if (rgplr[0].cShDef < 8 && FCheckShipBuilder(4, -1) == 0) {
                    tutor.idtBold = 451;
                    return 0;
                }
                tutor.idh = 3039;
                if (lpshdefBuild->hul.rghs[0].cItem == 0 || FCheckBuilderPart(0, &hs, 1) == 0) {
                    tutor.idtBold = 452;
                } else if (lpshdefBuild->hul.rghs[2].cItem == 0 || FCheckBuilderPart(2, &hs1, 1) == 0) {
                    tutor.idtBold = 453;
                } else {
                    tutor.idtBold = 454;
                }
                return 0;
            }
            if (LpplFromId(13)->lpplprod->iprodMac < 5 || FCheckQueue(13, 2, grobjFleet, 7, 1, 0) == 0) {
                tutor.idtBold = 455;
                return 0;
            }
            tutor.idtBold = 455;
            return 1;
        case 456:
            if (FCheckFleetWP(4, 1, grobjFleet, 516, 0xffff, 0xffff) == 0) {
                if (FCheckSelection(grobjFleet, 4) != 0) {
                    tutor.idtBold = 459;
                } else if (FCheckSummary(grobjFleet, 516) != 0) {
                    tutor.idtBold = 458;
                } else {
                    tutor.idtBold = 456;
                }
                return 0;
            }
            tutor.idtBold = 461;
            tutor.fTurnDone = 1;
            tutor.fProgress = 0;
            return 1;
        }
    case 23:
        switch (tutor.idt) {
        default:
            return 1;
        case 464:
            if (FCheckCargo(LpflFromId(6), 0, 0, 0, 210) != 0) {
                return 1;
            }
            if (FCheckMessages(1, 0xffff, 0) == 0 && FCheckSelection(grobjFleet, 4) == 0) {
                tutor.idtBold = 464;
                tutor.fProgress = 0;
                return 0;
            }
            if (FCheckMessages(2, 0xffff, 0) == 0 && FCheckSelection(grobjFleet, 6) == 0) {
                tutor.idtBold = 468;
                return 0;
            }
            if (tutor.fAutoComplete == 0 && tutor.fProgress == 0) {
                if (vprptCur == 0) {
                    tutor.idtBold = 469;
                } else if (vprptCur->icolSort == 2) {
                    tutor.idtBold = 471;
                    tutor.fProgress = 1;
                } else {
                    tutor.idtBold = 469;
                }
                return 0;
            }
            return 1;
        case 472:
            if (FCheckCargo(LpflFromId(6), 0, 0, 0, 210) == 0) {
                tutor.idtBold = 472;
                return 0;
            }
            if (FCheckFleetWP(6, 1, grobjPlanet, 17, 0xffff, 0xffff) == 0) {
                tutor.idtBold = 472;
                return 0;
            }
            if (FCheckFleetWP(6, 1, grobjPlanet, 17, 1, 0xffff) == 0) {
                tutor.idtBold = 473;
                return 0;
            }
            if (FCheckXferWP(6, 1, 17, 0xffff, rgiaUnloadAllCol) == 0) {
                tutor.idtBold = 474;
                tutor.idh = 1520;
                return 0;
            }
            if (LpflFromId(2) == 0) {
                tutor.fProgress = 0;
                return 1;
            }
            if (FCheckMessages(3, 0xffff, 0) == 0) {
                tutor.idtBold = 475;
            } else if (FCheckSelection(grobjFleet, 3) == 0) {
                tutor.idtBold = FCheckSelection(grobjFleet, 2) == 0 ? 475 : 477;
                tutor.idh = 1517;
            } else if (vrgiflMerge == 0) {
                tutor.idtBold = 478;
                tutor.idh = 1516;
            } else {
                tutor.idh = 1107;
                tutor.idtBold = 479;
            }
            return 0;
        case 480:
            if (FCheckFleetWP(7, 0, grobjPlanet, 13, 6, 0xffff) == 0) {
                tutor.idtBold = 480;
                return 0;
            }
            if (LpplFromId(13)->lpplprod->iprodMac < 3 || FCheckQueue(13, 0, grobjFleet, 6, 1, 0) == 0) {
                tutor.idtBold = 481;
                tutor.fProgress = 0;
                return 0;
            }
            if (FCheckMessages(14, 0xffff, 0) == 0 || (FCheckSelection(grobjFleet, 4) == 0 && FCheckSelection(grobjFleet, 8) == 0) ||
                (tutor.fAutoComplete == 0 && tutor.fProgress == 0)) {
                if (vrgtok != 0) {
                    tutor.fProgress = 1;
                }
                tutor.idtBold = 482;
                return 0;
            }
            if (LpflFromId(4)->lpplord->rgord[0].grobj != grobjFleet) {
                tutor.idtBold = 484;
                tutor.idh = 1518;
                return 0;
            }
            if (LpflFromId(8)->cord < 3) {
                if (FCheckMessages(9999, 0xffff, 0) == 0 || FCheckSelection(grobjFleet, 8) == 0) {
                    tutor.idtBold = 485;
                } else {
                    tutor.fNoErrors = 1;
                    if (FCheckFleetWP(8, 1, grobjPlanet, 1, 0xffff, 0xffff) != 0) {
                        tutor.idtBold = 487;
                    } else {
                        tutor.fNoErrors = 0;
                        FCheckFleetWP(8, 1, grobjPlanet, 6, 0xffff, 0xffff);
                        tutor.idtBold = 486;
                    }
                    tutor.fNoErrors = 0;
                }
                return 0;
            }
            if (FCheckFleetWP(8, 2, grobjPlanet, 0, 0xffff, 0xffff) == 0) {
                tutor.idtBold = 487;
                return 0;
            }
            tutor.idtBold = 487;
            tutor.fTurnDone = 1;
            return 1;
        }
    case 24:
        if (tutor.idt != 488) {
            return 1;
        }
        if (FCheckFleetWP(4, 1, grobjPlanet, 5, 0xffff, 0xffff) == 0) {
            tutor.idtBold = 488;
            return 0;
        }
        tutor.fNoErrors = 1;
        if (FCheckFleetWP(2, 1, grobjFleet, 5, 4, 0xffff) == 0) {
            tutor.fNoErrors = 0;
            FCheckFleetWP(2, 1, grobjPlanet, 12, 0xffff, 0xffff);
            tutor.idtBold = 489;
            return 0;
        }
        tutor.fNoErrors = 0;
        if (FCheckMessages(9999, 0xffff, 0) == 0) {
            tutor.idtBold = 490;
            return 0;
        }
        tutor.idtBold = 492;
        tutor.fTurnDone = 1;
        return 1;
    case 25:
        if (tutor.idt != 496) {
            if (tutor.idt != 504) {
                return 1;
            }
            if (LpplFromId(13)->lpplprod->iprodMac < 3 || FCheckQueue(13, 0, grobjFleet, 2, 3, 0) == 0) {
                tutor.idtBold = 504;
                return 0;
            }
            if (FCheckMessages(4, 0xffff, 0) == 0) {
                tutor.idtBold = 505;
                return 0;
            }
            if (LpplFromId(17)->lpplprod->iprodMac != 3 || FCheckQueue(17, 2, grobjPlanet, 5, 2, 1) == 0) {
                if (FCheckSelection(grobjPlanet, 17) == 0) {
                    tutor.idtBold = 505;
                } else {
                    tutor.idtBold = 507;
                }
                return 0;
            }
            if (FCheckMessages(6, 0xffff, 0) == 0 || FCheckResearch(Propulsion, Construction, 30) == 0) {
                tutor.idtBold = 508;
                return 0;
            }
            if (FCheckMessages(9999, 0xffff, 0) == 0) {
                tutor.idtBold = 509;
                return 0;
            }
            tutor.idtBold = 511;
            tutor.fTurnDone = 1;
            tutor.fProgress = 0;
            return 1;
        }
        if (FCheckMessages(3, 0xffff, 0) == 0) {
            tutor.idtBold = 496;
            return 0;
        }
        if (rgshdef[2].hul.rghs[0].iItem != 4 || hwndSlotDlg != 0) {
            hs.grhst = hstEngine;
            hs.iItem = 4;
            hs.cItem = 1;
            hs2.grhst = hstSpecialM;
            hs2.iItem = 0;
            hs2.cItem = 1;
            if (FCheckScanner(3, -1) == 0) {
                tutor.idtBold = 497;
                return 0;
            }
            if (hwndSlotDlg == 0) {
                tutor.idtBold = 499;
                tutor.idh = 1001;
                return 0;
            }
            if (rgshdef[2].hul.rghs[0].iItem == 4) {
                tutor.idtBold = 503;
                return 0;
            }
            if (FCheckShipBuilder(4, -1) == 0) {
                if (FCheckShipBuilder(0, 2) == 0) {
                    tutor.idtBold = 500;
                } else {
                    tutor.idtBold = 501;
                }
            } else if (lpshdefBuild->hul.rghs[0].cItem == 0 || lpshdefBuild->hul.rghs[0].iItem != 4 || FCheckBuilderPart(1, &hs2, 1) == 0) {
                tutor.idtBold = 502;
            } else {
                tutor.idtBold = 503;
            }
            return 0;
        }
        return 1;
    case 26:
        if (tutor.idt != 512) {
            if (tutor.idt != 520) {
                return 1;
            }
            if (LpplFromId(13)->lpplprod->iprodMac < 4 || FCheckQueue(13, 1, grobjFleet, 3, 3, 0) == 0) {
                tutor.idtBold = 521;
                return 0;
            }
            tutor.fNoErrors = 1;
            if (FCheckFleetWP(4, 1, grobjFleet, 517, 0xffff, 0xffff) == 0) {
                tutor.fNoErrors = 0;
                FCheckColonizeWP(4, 5, 0xffff);
                if (FCheckSummary(grobjFleet, 517) != 0 || FCheckSummary(grobjFleet, 4) != 0 || FCheckSelection(grobjFleet, 4) != 0) {
                    tutor.idtBold = 525;
                } else {
                    tutor.idtBold = 523;
                }
                return 0;
            }
            tutor.fNoErrors = 0;
            tutor.idtBold = 527;
            tutor.fTurnDone = 1;
            return 1;
        }
        if (FCheckMessages(4, 0xffff, 0) == 0) {
            tutor.idtBold = 512;
            return 0;
        }
        if (rgplr[0].cFleet == 10 && FCheckColonizeWP(9, 0, 0xffff) == 0) {
            tutor.idtBold = FCheckSelection(grobjFleet, 9) == 0 ? 513 : 514;
            return 0;
        }
        if (rgplr[0].cFleet == 10) {
            tutor.idtBold = 515;
            return 0;
        }
        tutor.fNoErrors = 1;
        if (FCheckColonizeWP(9, 1, 0xffff) == 0) {
            tutor.fNoErrors = 0;
            FCheckColonizeWP(9, 0, 0xffff);
            tutor.idtBold = 516;
            return 0;
        }
        if (FCheckColonizeWP(10, 23, 0xffff) == 0) {
            tutor.fNoErrors = 0;
            FCheckColonizeWP(10, 0, 0xffff);
            tutor.idtBold = 517;
            return 0;
        }
        tutor.fNoErrors = 0;
        if (FCheckMessages(9999, 0xffff, 0) == 0) {
            tutor.idtBold = 518;
            return 0;
        }
        return 1;
    case 27:
        switch (tutor.idt) {
        default:
            return 1;
        case 528:
            tutor.fNoErrors = 1;
            if (FCheckFleetWP(4, 0, grobjFleet, 517, 0xffff, 0xffff) == 0) {
                tutor.fNoErrors = 0;
                if (FCheckSelection(grobjFleet, 4) != 0) {
                    tutor.idh = 1518;
                    tutor.idtBold = 530;
                } else {
                    tutor.idtBold = 528;
                }
                return 0;
            }
            tutor.fNoErrors = 0;
            if (FCheckMessages(1, 0xffff, 0) == 0) {
                tutor.idtBold = 532;
                return 0;
            }
            if (LpflFromId(12) == 0 || LpflFromId(12)->fDead != 0) {
                return 1;
            }
            if (FCheckCargo(LpflFromId(12), 0, 0, 0, 630) == 0) {
                if (FCheckSelection(grobjFleet, 12) != 0) {
                    tutor.idtBold = 535;
                } else {
                    tutor.idtBold = FCheckSelection(grobjFleet, 8) == 0 ? 532 : 534;
                }
                return 0;
            }
            return 1;
        case 536:
            if (LpflFromId(12) != 0) {
                if (FCheckSelection(grobjFleet, 11) == 0) {
                    tutor.idtBold = 537;
                } else if (vrgiflMerge == 0) {
                    tutor.idtBold = 537;
                    tutor.idh = 1516;
                } else {
                    tutor.idtBold = 537;
                }
                return 0;
            }
            if (FCheckMessages(8, 0xffff, 0) == 0) {
                tutor.idtBold = 538;
                return 0;
            }
            if (FCheckMessages(9999, 0xffff, 0) == 0) {
                tutor.idtBold = 540;
                return 0;
            }
            tutor.idtBold = 542;
            tutor.idh = 6001;
            if (hwndSlotDlg != 0 || rgplr[0].cShDef == 9) {
                return 1;
            }
            return 0;
        case 544:
            if (hwndSlotDlg != 0 || rgplr[0].cShDef < 9) {
                if (FCheckShipBuilder(4, -1) == 0) {
                    tutor.idtBold = rgplr[0].cShDef >= 9 ? 549 : 545;
                } else if (lpshdefBuild->hul.rghs[0].cItem != 1 || lpshdefBuild->hul.rghs[1].cItem != 1 || lpshdefBuild->hul.rghs[2].cItem != 1 ||
                           lpshdefBuild->hul.rghs[3].cItem != 1 || lpshdefBuild->hul.rghs[4].cItem != 2) {
                    tutor.idtBold = 546;
                } else if (lpshdefBuild->hul.rghs[5].cItem != 1 || lpshdefBuild->hul.rghs[6].cItem != 1) {
                    tutor.idtBold = 547;
                } else {
                    tutor.idtBold = 549;
                }
                return 0;
            }
            if (LpplFromId(13)->lpplprod->iprodMac < 3 || FCheckQueue(13, 0, grobjFleet, 8, 10, 0) == 0) {
                tutor.idtBold = 550;
                return 0;
            }
            tutor.idtBold = 551;
            tutor.fTurnDone = 1;
            return 1;
        }
    case 28:
        if (tutor.idt != 552) {
            return 1;
        }
        if (FCheckFleetWP(4, 1, grobjPlanet, 5, 0xffff, 0xffff) == 0) {
            if (FCheckSelection(grobjFleet, 4) == 0) {
                tutor.idtBold = 552;
            } else {
                tutor.idtBold = 554;
            }
            return 0;
        }
        if (FCheckMessages(6, 0xffff, 0) == 0) {
            tutor.idtBold = 555;
            return 0;
        }
        if (FCheckFleetWP(12, 1, grobjPlanet, 10, 0xffff, 0xffff) == 0) {
            if (FCheckSelection(grobjFleet, 12) == 0) {
                tutor.idtBold = 555;
            } else {
                tutor.idtBold = 556;
            }
            return 0;
        }
        if (FCheckMessages(9999, 0xffff, 0) == 0) {
            tutor.idtBold = 557;
            return 0;
        }
        tutor.idtBold = 559;
        tutor.fTurnDone = 1;
        tutor.fProgress = 0;
        return 1;
    case 29:
        switch (tutor.idt) {
        default:
            return 1;
        case 560:
            tutor.fNoErrors = 1;
            if (FCheckFleetWP(3, 1, grobjPlanet, 13, 0xffff, 5) == 0) {
                tutor.fNoErrors = 0;
                if (FCheckSelection(grobjFleet, 3) == 0) {
                    tutor.idtBold = 560;
                } else {
                    tutor.idtBold = 562;
                }
                return 0;
            }
            tutor.fNoErrors = 0;
            if (FCheckMessages(6, 0xffff, 0) == 0) {
                tutor.idtBold = 564;
                return 0;
            }
            if (FCheckFleetWP(13, 1, grobjPlanet, 10, 0xffff, 0xffff) == 0) {
                tutor.idtBold = 565;
                return 0;
            }
            if (FCheckPlanetRoute(13, 10) == 0) {
                tutor.idtBold = 567;
                return 0;
            }
            return 1;
        case 568:
            if (FCheckMessages(17, 0xffff, 0) == 0) {
                tutor.idtBold = 569;
                return 0;
            }
            if (FCheckResearch(Construction, Energy, 30) == 0) {
                tutor.idtBold = 570;
                return 0;
            }
            if (FCheckMessages(9999, 0xffff, 0) == 0) {
                tutor.idtBold = 571;
                return 0;
            }
            tutor.fNoErrors = 1;
            if (FCheckFleetWP(6, 0, grobjPlanet, 17, 5, 0xffff) == 0) {
                tutor.fNoErrors = 0;
                if (FCheckSelection(grobjFleet, 6) == 0) {
                    tutor.idtBold = 572;
                } else {
                    tutor.idtBold = 574;
                }
                return 0;
            }
            tutor.fNoErrors = 0;
            return 1;
        case 576:
            if (hwndSlotDlg != 0) {
                if (rgshdef[9].fFree == 0 && FCheckShipBuilder(4, -1) == 0) {
                    tutor.idtBold = 581;
                } else if (FCheckShipBuilder(4, -1) == 0) {
                    tutor.idtBold = 578;
                } else if (lpshdefBuild->hul.rghs[0].cItem != 2) {
                    tutor.idtBold = 579;
                } else if (lpshdefBuild->hul.rghs[1].cItem != 4 || lpshdefBuild->hul.rghs[2].cItem != 4 || lpshdefBuild->hul.rghs[3].cItem != 1) {
                    tutor.idtBold = 580;
                } else {
                    tutor.idtBold = 581;
                }
                return 0;
            }
            if (rgplr[0].cShDef < 10) {
                tutor.idtBold = 577;
                tutor.idh = 6001;
                return 0;
            }
            if (LpplFromId(13)->lpplprod->iprodMac < 3 || FCheckQueue(13, 0, grobjFleet, 9, 10, 0) == 0) {
                tutor.idtBold = 582;
                return 0;
            }
            tutor.idtBold = 583;
            tutor.fTurnDone = 1;
            tutor.fProgress = 0;
            return 1;
        }
    case 30:
        if (tutor.idt != 584) {
            return 1;
        }
        if (FCheckMessages(5, 0xffff, 0) == 0) {
            tutor.idtBold = 586;
            return 0;
        }
        if (FCheckMessages(9999, 0xffff, 0) == 0) {
            if (FCheckSelection(grobjFleet, 14) == 0) {
                tutor.idtBold = 586;
            } else {
                tutor.idtBold = 588;
            }
            return 0;
        }
        tutor.idtBold = 590;
        tutor.fTurnDone = 1;
        tutor.fProgress = 0;
        return 1;
    case 31:
        if (tutor.idt != 592) {
            return 1;
        }
        if (FCheckMessages(6, 0xffff, 0) == 0) {
            tutor.idtBold = 592;
            return 0;
        }
        if (FCheckSelection(grobjPlanet, 5) == 0) {
            tutor.idtBold = 592;
            return 0;
        }
        if (LpplFromId(5)->lpplprod->iprodMac < 4 || FCheckQueue(5, 0, grobjPlanet, 8, 100, 1) == 0) {
            tutor.idtBold = 594;
            return 0;
        }
        if (FCheckMessages(9999, 0xffff, 0) == 0) {
            tutor.idtBold = 596;
            return 0;
        }
        tutor.idtBold = 598;
        tutor.fTurnDone = 1;
        tutor.fProgress = 0;
        return 1;
    case 32:
        if (tutor.idt != 600) {
            return 1;
        }
        if (FCheckMessages(2, 0xffff, 0) == 0) {
            tutor.idtBold = 600;
            return 0;
        }
        if (tutor.fProgress == 0 && tutor.fAutoComplete == 0) {
            tutor.idtBold = 602;
            return 0;
        }
        if (FCheckMessages(18, 0xffff, 0) == 0) {
            if (FCheckSelection(grobjFleet, 12) == 0) {
                tutor.idtBold = 600;
            } else {
                tutor.idtBold = 604;
            }
            return 0;
        }
        tutor.idtBold = 606;
        if (FCheckMessages(9999, 0xffff, 0) == 0) {
            return 0;
        }
        tutor.fTurnDone = 1;
        tutor.fProgress = 0;
        return 1;
    case 33:
        if (tutor.idt != 608) {
            return 1;
        }
        if (FCheckMessages(10, 0xffff, 0) == 0) {
            tutor.idtBold = 608;
            return 0;
        }
        if (LpplFromId(13)->lpplprod->iprodMac < 3 || FCheckQueue(13, 0, grobjFleet, 9, 10, 0) == 0) {
            if (FCheckSelection(grobjPlanet, 13) == 0) {
                tutor.idtBold = 608;
            } else {
                tutor.idtBold = 609;
            }
            return 0;
        }
        if (FCheckMessages(9999, 0xffff, 0) == 0) {
            tutor.idtBold = 611;
            return 0;
        }
        tutor.idtBold = 612;
        tutor.fTurnDone = 1;
        tutor.fProgress = 0;
        return 1;
    case 34:
        if (tutor.idt != 616) {
            return 1;
        }
        tutor.idtBold = 619;
        if (FCheckMessages(9999, 0xffff, 0) == 0) {
            return 0;
        }
        tutor.fTurnDone = 1;
        tutor.fProgress = 0;
        return 1;
    case 35:
        if (tutor.idt != 624) {
            return 1;
        }
        if (FCheckMessages(9999, 0xffff, 0) == 0) {
            tutor.idtBold = 624;
            return 0;
        }
        tutor.idtBold = 628;
        tutor.fTurnDone = 1;
        tutor.fProgress = 0;
        return 1;
    case 36:
        if (tutor.idt != 632) {
            return 1;
        }
        if (FCheckMessages(9999, 0xffff, 0) == 0) {
            tutor.idtBold = 636;
            return 0;
        }
        tutor.idtBold = 637;
        tutor.fTurnDone = 1;
        tutor.fProgress = 0;
        return 1;
    }
}

int16_t FCheckZip(int16_t iZip, ITEMACTION *lpiaGoal, StringId ids) {
    ITEMACTION *piaCur;
    int16_t     i;
    char        szT[33];
    int16_t     idhSav;

    idhSav = tutor.idh;
    tutor.idh = 1098;
    if (tutor.fAutoComplete != 0) {
        vrgZip[iZip].fValid = 1;
        piaCur = vrgZip[iZip].txp.rgia;
        i = 0;
        while (i < 5) {
            piaCur->iAction = lpiaGoal->iAction;
            i++;
            piaCur++;
            lpiaGoal++;
        }
        CchGetString(ids, szT);
        strcpy(vrgZip[iZip].szName, szT);
        return 1;
    }
    if (vrgZip[iZip].fValid == 0) {
        if (hwndZipOrderDlg == 0) {
            tutor.idh = 1520;
        }
        return 0;
    }
    piaCur = vrgZip[iZip].txp.rgia;
    tutor.idh = 1519;
    i = 0;
    while (i < 5) {
        if (piaCur->iAction != lpiaGoal->iAction && piaCur->iAction != iActionNone) {
            return 0;
        }
        i++;
        piaCur++;
        lpiaGoal++;
    }
    CchGetString(ids, szT);
    if (strcmpi(szT, vrgZip[iZip].szName) == 0) {
        tutor.idh = idhSav;
        return 1;
    }
    TutorError(520);
    return 0;
}

int16_t FCheckTemplate(int16_t iTemplate) {
    int16_t i;

    tutor.idh = 3117;
    if (tutor.fAutoComplete != 0) {
        vrgZipProd[0].fValid = 1;
        vrgZipProd[0].zpq1 = rgzpqTut[iTemplate];
        gd.fChgZipProd = 1;
        return 1;
    }
    if (vrgZipProd[0].fValid == 0) {
        return 0;
    }
    if (vrgZipProd[0].fNoResearch != rgzpqTut[iTemplate].fNoResearch) {
        return 0;
    }
    if (vrgZipProd[0].cpq != rgzpqTut[iTemplate].cpq) {
        return 0;
    }
    for (i = 0; i < rgzpqTut[iTemplate].cpq; i++) {
        if (vrgZipProd[0].rgpq[i].w != rgzpqTut[iTemplate].rgpq[i].w) {
            return 0;
        }
    }
    gd.fChgZipProd = 1;
    return 1;
}

void TutorError(int16_t idsError) {
    uint16_t t_scratch_m4;

    if (tutor.fNoErrors != 0) {
        tutor.idsError = -1;
    } else {
        if (tutor.idsError == idsError) {
            t_scratch_m4 = tutor.cError;
            tutor.cError++;
            if (t_scratch_m4 < 3) {
                return;
            }
        }
        tutor.cError = 0;
        tutor.idsError = idsError;
        AlertSz(PszFormatIds(idsError, NULL), MB_ICONHAND);
    }
    return;
}

int16_t FCheckScanner(int16_t md, int16_t iZoom) {
    int16_t idhSav;

    idhSav = tutor.idh;
    if (tutor.fAutoComplete != 0) {
        return 1;
    }
    tutor.idh = 14008;
    if (md != -1 && grbitScan != md) {
        if (md < 6) {
            if ((grbitScan & 0xf) != md) {
                return 0;
            }
        } else if ((md & grbitScan) != md) {
            return 0;
        }
    }
    if (iZoom != -1 && iZoom != iScanZoom) {
        tutor.idh = 14022;
        return 0;
    }
    tutor.idh = idhSav;
    return 1;
}

int16_t FCheckFleetName(int16_t id, StringId ids) {
    FLEET  *lpfl;
    char    szT[33];
    int16_t idhSav;

    idhSav = tutor.idh;
    tutor.idh = 3054;
    lpfl = LpflFromId(id);
    if (lpfl == 0) {
        return 1;
    }
    if (lpfl->lpszName == 0) {
        if (ids == 0xffff) {
            tutor.idh = idhSav;
            return 1;
        }
        if (ids == 0xffff) {
            return 1;
        }
        return 0;
    }
    CchGetString(ids, szT);
    if (fstricmp(szT, lpfl->lpszName) == 0) {
        tutor.idh = idhSav;
        return 1;
    }
    TutorError(1231);
    return 0;
}

int16_t FCheckSummary(GrobjClass grobj, int16_t id) {
    int16_t fRet;

    if (gd.fGeneratingTurn != 0) {
        return 1;
    }
    fRet = 0;
    switch (grobj) {
    case grobjFleet:
        if (sel.scan.grobj == grobjFleet && sel.scan.ifl != -1 && rglpfl[sel.scan.ifl]->id == id) {
            fRet = 1;
            break;
        }
        fRet = 0;
        break;
    case grobjPlanet:
        if (sel.scan.grobj == grobjPlanet && sel.scan.idpl == id) {
            fRet = 1;
            break;
        }
        fRet = 0;
        break;
    case grobjThing:
        fRet = sel.scan.grobj == grobjThing && (id == -1 || lpThings[sel.scan.ith].idFull == id);
    }
    if (fRet == 0) {
        tutor.idh = 14034;
    }
    return fRet;
}

int16_t FCheckSelection(GrobjClass grobj, int16_t id) {
    int16_t fRet;
    int16_t idhSav;

    idhSav = tutor.idh;
    if (tutor.fAutoComplete != 0 || gd.fGeneratingTurn != 0) {
        return 1;
    }
    if (mdMsgObj == (grobj == grobjFleet ? 2 : 1) && id == idMsgObj) {
        tutor.idh = 14001;
    } else if (grobj == grobjPlanet && sel.grobj == grobjFleet && sel.fl.idPlanet == id) {
        tutor.idh = 1514;
    } else if (grobj == grobjFleet && sel.grobj == grobjPlanet && LpflFromId(id)->idPlanet == sel.pl.id) {
        tutor.idh = 1510;
    } else {
        tutor.idh = 1526;
    }
    fRet = 0;
    if (grobj == grobjPlanet) {
        fRet = sel.grobj == grobjPlanet && sel.pl.id == id;
    } else if (grobj == grobjFleet) {
        fRet = sel.grobj == grobjFleet && sel.fl.id == id;
    }
    if (fRet != 0) {
        tutor.idh = idhSav;
    }
    return fRet;
}

int16_t FCheckMessages(int16_t imsg, MessageId idm, int16_t fFilter) {
    int16_t idhSav;

    idhSav = tutor.idh;
    if (gd.fGeneratingTurn != 0) {
        return 1;
    }
    tutor.idh = 14001;
    if (imsg == 9999 && IMsgNext(0) != -1) {
        return 0;
    }
    if (imsg != 9999 && imsg != -1 && iMsgCur < imsg) {
        return 0;
    }
    if (idm != 0xffff) {
        if (fFilter != 0 && tutor.fAutoComplete != 0 && (bitfMsgFiltered[idm >> 3] & 1 << (idm & 7)) == 0) {
            SetFilteringGroups(idm, 1);
            tutor.idh = idhSav;
            return 1;
        }
        if (fFilter != 0 && (bitfMsgFiltered[idm >> 3] & 1 << (idm & 7)) == 0) {
            return 0;
        }
        if (fFilter == 0 && IdmGetMessageN(iMsgCur) != idm) {
            return 0;
        }
    }
    tutor.idh = idhSav;
    return 1;
}

int16_t FCheckResearch(TechFieldType iTech, TechFieldType iTechNext, int16_t pct) {
    if ((rgplr[0].iTechCur & 0xf) == iTech && rgplr[0].iTechCur >> 4 == iTechNext && rgplr[0].pctResearch == pct) {
        return 1;
    }
    tutor.idh = 1070;
    return 0;
}

int16_t FCheckFleetWP(uint16_t ifl, int16_t iord, GrobjClass grobj, int16_t id, uint16_t grTask, uint16_t iWarp) {
    ORDER   ord;
    int16_t fRet;
    FLEET  *lpfl;
    int16_t idh;
    int16_t idhSav;

    fRet = 0;
    idhSav = tutor.idh;
    lpfl = LpflFromId(ifl);
    tutor.idh = 1526;
    if (lpfl != 0) {
        tutor.idh = 3062;
        if (lpfl->cord >= iord + 1) {
            ord = lpfl->lpplord->rgord[iord];
            if ((id & 0x7fff) != 0x7fff && (ord.grobj != grobj || ord.id != id)) {
                TutorError(492);
                tutor.idh = 3063;
            } else if (ord.grTask != grTask && grTask != 0xffff) {
                tutor.idh = 1519;
                if (ord.grTask != grTaskNone) {
                    TutorError(grTask == 0 ? 493 : 494);
                }
            } else {
                tutor.idh = 1518;
                if (iWarp != 0xffff) {
                    fRet = ord.iWarp == iWarp ? 1 : 0;
                } else {
                    fRet = 1;
                }
            }
        }
    }
    idh = tutor.idh;
    if (fRet == 0 && FCheckSelection(grobjFleet, ifl) != 0) {
        tutor.idh = idh;
    }
    if (fRet != 0) {
        tutor.idh = idhSav;
    }
    return fRet;
}

int16_t FCheckPlanetRoute(int16_t idpl, int16_t idplRoute) {
    PLANET *lppl;
    int16_t idhSav;

    idhSav = tutor.idh;
    lppl = LpplFromId(idpl);
    if (lppl == 0) {
        return 0;
    }
    tutor.idh = 1531;
    if (lppl->idRoute != idplRoute + 1) {
        return 0;
    }
    tutor.idh = idhSav;
    return 1;
}

int16_t FCheckLayingWP(uint16_t ifl, int16_t iord, int16_t id, int16_t iYears) {
    FLEET     *lpfl;
    int16_t    idhSav;
    GrobjClass grobj;

    idhSav = tutor.idh;
    lpfl = LpflFromId(ifl);
    if (lpfl == 0) {
        return 0;
    }
    tutor.idh = 1528;
    grobj = (id & 0x8000) == 0 ? grobjPlanet : grobjFleet;
    if (FCheckFleetWP(ifl, iord, grobj, id & 0x7fff, 6, 0xffff) == 0) {
        return 0;
    }
    if (lpfl->lpplord->rgord[iord].tsell.iPlrX != iYears) {
        return 0;
    }
    tutor.idh = idhSav;
    return 1;
}

int16_t FCheckColonizeWP(uint16_t ifl, int16_t id, uint16_t iWarp) {
    int16_t ish;
    FLEET  *lpfl;
    int16_t csh;
    int16_t idhSav;

    idhSav = tutor.idh;
    lpfl = LpflFromId(ifl);
    if (lpfl == 0) {
        return 0;
    }
    tutor.idh = 1522;
    csh = 0;
    for (ish = 0; ish < 16; ish++) {
        csh += lpfl->rgcsh[ish];
    }
    if (lpfl->idPlanet == 13 && FCheckCargo(lpfl, 0, 0, 0, 25 * csh) == 0) {
        return 0;
    }
    if (FCheckFleetWP(ifl, 1, grobjPlanet, id, 2, iWarp) != 0) {
        tutor.idh = idhSav;
        return 1;
    }
    return 0;
}

int16_t FCheckPatrolWP(uint16_t ifl, int16_t iord, int16_t id, uint16_t iWarp, uint16_t iPlan, uint16_t iDist) {
    FLEET     *lpfl;
    int16_t    idhSav;
    GrobjClass grobj;

    idhSav = tutor.idh;
    lpfl = LpflFromId(ifl);
    if (lpfl == 0) {
        return 0;
    }
    tutor.idh = 3095;
    grobj = (id & 0x8000) == 0 ? grobjPlanet : grobjFleet;
    if (FCheckFleetWP(ifl, iord, grobj, id & 0x7fff, 7, iWarp) == 0) {
        return 0;
    }
    if (iDist != 0xffff && lpfl->lpplord->rgord[iord].tptl.iDist != iDist) {
        tutor.idh = 1519;
        return 0;
    }
    tutor.idh = idhSav;
    return 1;
}

int16_t FCheckXferWP(uint16_t ifl, int16_t iord, int16_t id, uint16_t iWarp, ITEMACTION *lpiaGoal) {
    ORDER       ord;
    int16_t     fRet;
    ITEMACTION *piaCur;
    int16_t     i;
    FLEET      *lpfl;
    int16_t     idh;
    GrobjClass  grobj;
    int16_t     idhSav;

    fRet = 0;
    idhSav = tutor.idh;
    if ((id & 0x8000) != 0) {
        id &= 0x7fff;
        grobj = grobjFleet;
    } else {
        grobj = grobjPlanet;
    }
    lpfl = LpflFromId(ifl);
    if (lpfl == 0) {
        return 0;
    }
    if (FCheckFleetWP(ifl, iord, grobj, id, 1, iWarp) == 0) {
        return 0;
    }
    ord = lpfl->lpplord->rgord[iord];
    piaCur = ord.txp.rgia;
    tutor.idh = 1519;
    i = 0;
    while (i < 5) {
        if (piaCur->iAction != lpiaGoal->iAction) {
            if (piaCur->iAction == iActionNone)
                goto LReturn;
            TutorError(616);
            goto LReturn;
        }
        if ((piaCur->iAction == iActionUnloadExact || piaCur->iAction == iActionSetAmount) && piaCur->cQuan != lpiaGoal->cQuan)
            goto LReturn;
        i++;
        piaCur++;
        lpiaGoal++;
    }
    fRet = 1;
LReturn:
    idh = tutor.idh;
    if (fRet == 0 && FCheckSelection(grobjFleet, ifl) != 0) {
        tutor.idh = idh;
    }
    if (fRet != 0) {
        tutor.idh = idhSav;
    }
    return fRet;
}

int16_t FCheckQueue(int16_t ipl, int16_t iprod, GrobjClass grobj, uint16_t iItem, uint16_t cItem, uint16_t fNoResearch) {
    int16_t fRet;
    PLANET *lppl;
    PROD    prod;
    int16_t idh;
    int16_t idhSav;

    idhSav = tutor.idh;
    fRet = 0;
    if (game.turn < 2) {
        tutor.idh = 1507;
    } else {
        tutor.idh = 1059;
    }
    lppl = LpplFromId(ipl);
    if (lppl != 0 && lppl->lpplprod != 0 && lppl->lpplprod->iprodMac > iprod) {
        prod = lppl->lpplprod->rgprod[iprod];
        if (prod.grobj != (uint32_t)grobj || prod.iItem != (uint32_t)iItem) {
            TutorError(491);
        } else if (prod.cItem != (uint32_t)cItem) {
            TutorError(496);
        } else if (fNoResearch != 0xffff && lppl->fNoResearch != (uint32_t)fNoResearch) {
            TutorError(1305);
        } else {
            fRet = 1;
        }
    }
    idh = tutor.idh;
    if (fRet == 0 && FCheckSelection(grobjPlanet, ipl) != 0) {
        tutor.idh = idh;
    }
    if (fRet != 0) {
        tutor.idh = idhSav;
    }
    return fRet;
}

int16_t FCheckBtlPlan(int16_t ibp, uint16_t imdTarget, uint16_t fSpread, uint16_t fBomb, uint16_t fDump, uint16_t mdUnarmed, uint16_t mdScout, uint16_t mdWar,
                      uint16_t mdBomber) {
    BTLPLAN *lpbtlplan;
    int16_t  idhSav;

    idhSav = tutor.idh;
    tutor.idh = 3105;
    if (ibp < 0 || ibp > rgcbtlplan[0]) {
        return 0;
    }
    lpbtlplan = rglpbtlplan[0] + ibp;
    tutor.idh = idhSav;
    return 1;
}

int16_t FCheckCargo(FLEET *lpfl, int16_t wtMin1, int16_t wtMin2, int16_t wtMin3, int16_t wtColonists) {
    int16_t fRet;
    int16_t idh;
    int16_t idhSav;

    idhSav = tutor.idh;
    fRet = 0;
    if (lpfl == 0) {
        return 0;
    }
    tutor.idh = 1075;
    if ((wtMin1 == 0 && lpfl->rgwtMin[0] != 0) || (wtMin2 == 0 && lpfl->rgwtMin[1] != 0) || (wtMin3 == 0 && lpfl->rgwtMin[2] != 0) ||
        (wtColonists == 0 && lpfl->rgwtMin[3] != 0)) {
        TutorError(495);
    } else if (lpfl->rgwtMin[3] == wtColonists && lpfl->rgwtMin[0] == wtMin1 && lpfl->rgwtMin[1] == wtMin2 && lpfl->rgwtMin[2] == wtMin3) {
        fRet = 1;
    }
    idh = tutor.idh;
    if (fRet == 0 && FCheckSelection(grobjFleet, lpfl->id) != 0) {
        tutor.idh = idh;
    }
    if (fRet != 0) {
        tutor.idh = idhSav;
    }
    return fRet;
}

int16_t FCheckBuilderPart(int16_t iSlot, HS *phs, uint16_t cInit) {
    uint16_t cItemAct;
    int16_t  idhSav;

    idhSav = tutor.idh;
    if (hwndSlotDlg == 0) {
        tutor.idh = 1066;
        return 0;
    }
    tutor.idh = 3040;
    if (mdBuild != mdBuildEdit) {
        return 0;
    }
    cItemAct = lpshdefBuild->hul.rghs[iSlot].cItem;
    if (phs->cItem == 0 && cItemAct == 0) {
        return 1;
    }
    if (cInit < phs->cItem) {
        if (cItemAct < cInit || cItemAct > phs->cItem)
            goto BadCnt;
    } else if (cItemAct < phs->cItem || cItemAct > cInit) {
        goto BadCnt;
    }
    if (cItemAct != phs->cItem)
        goto BadCntSilent;
    if (phs->grhst != lpshdefBuild->hul.rghs[iSlot].grhst || phs->iItem != lpshdefBuild->hul.rghs[iSlot].iItem) {
        TutorError(502);
        goto BadCntSilent;
    }
    tutor.idh = idhSav;
    return 1;
BadCnt:
    TutorError(501);
BadCntSilent:
    tutor.idh = 3039;
    return 0;
}

int16_t FCheckShipBuilder(int16_t iCategory, int16_t iShip) {
    int16_t iSel;
    int16_t idhSav;

    idhSav = tutor.idh;
    tutor.idh = 1066;
    if (hwndSlotDlg == 0) {
        return 0;
    }
    if (iCategory != -1 && iCategory != mdBuild) {
        return 0;
    }
    iSel = LOWORD(SendMessage(GetDlgItem(hwndSlotDlg, IDC_COMBOBOX), CB_GETCURSEL, 0, 0));
    if (iShip == -1 || iShip == iSel) {
        tutor.idh = idhSav;
        return 1;
    }
    return 0;
}

int16_t FTutorialEnabledShipBuilder(int16_t itutsbAction) {
    HS      hs2;
    HS      hs3;
    HS      hs;
    HS      hs1;
    HS      hs4;
    int16_t t_merge_81d4_0001;
    int16_t t_call_7c62;

    switch (itutsbAction) {
    default:
        t_merge_81d4_0001 = 0;
        break;
    case 0:
        TutorError(497);
        t_merge_81d4_0001 = 0;
        break;
    case 1:
        switch (game.turn) {
        default:
            goto NoCustom;
        case 13:
            if (tutor.idt != 328)
                goto NoCustom;
            if (rgplr[0].cShDef == 7) {
                TutorError(499);
                t_merge_81d4_0001 = 0;
                break;
            }
            if (FCheckShipBuilder(1, 7) != 0)
                goto L_7c40;
            TutorError(510);
            t_merge_81d4_0001 = 0;
            break;
        case 20:
            if (tutor.idt != 416)
                goto NoCustom;
            if (rgplr[0].cshdefSB == 2) {
                TutorError(499);
                t_merge_81d4_0001 = 0;
                break;
            }
            if (FCheckShipBuilder(0, 0) != 0 && fStarbaseMode != 0)
                goto L_7c40;
            TutorError(500);
            t_merge_81d4_0001 = 0;
            break;
        case 22:
            if (tutor.idt != 448)
                goto NoCustom;
            if (rgplr[0].cShDef == 8) {
                TutorError(499);
                t_merge_81d4_0001 = 0;
                break;
            }
            if (FCheckShipBuilder(1, 3) != 0)
                goto L_7c40;
            TutorError(510);
            t_merge_81d4_0001 = 0;
            break;
        case 27:
            if (tutor.idt != 544)
                goto NoCustom;
            if (rgplr[0].cShDef == 9) {
                TutorError(499);
                t_merge_81d4_0001 = 0;
                break;
            }
            if (FCheckShipBuilder(1, 4) != 0)
                goto L_7c40;
            TutorError(510);
            t_merge_81d4_0001 = 0;
            break;
        case 29:
            if (tutor.idt != 576)
                goto NoCustom;
            if (rgplr[0].cShDef == 10) {
                TutorError(499);
                t_merge_81d4_0001 = 0;
            } else {
                if (FCheckShipBuilder(1, 8) != 0)
                    goto L_7c40;
                TutorError(510);
                t_merge_81d4_0001 = 0;
            }
        }
        break;
    L_7c40:
        t_merge_81d4_0001 = 1;
        break;
    case 2:
        if (game.turn != 25 || tutor.idt != 496)
            goto NoCustom;
        t_call_7c62 = FCheckShipBuilder(0, 2);
        if (t_call_7c62 != 0) {
            t_merge_81d4_0001 = t_call_7c62;
            break;
        }
        TutorError(511);
        t_merge_81d4_0001 = 0;
        break;
    case 3:
        if ((uint16_t)(game.turn - 13) > 16)
            goto NoCustom;
        switch (game.turn) {
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 21:
        case 23:
        case 24:
        case 26:
        case 28:
            goto NoCustom;
        case 13:
            hs.grhst = hstScanner;
            hs.iItem = 1;
            hs.cItem = 1;
            hs2.grhst = hstEngine;
            hs2.iItem = 3;
            hs2.cItem = 1;
            hs3.grhst = hstMining;
            hs3.iItem = 2;
            hs3.cItem = 1;
            if (tutor.idt != 336) {
                TutorError(504);
                t_merge_81d4_0001 = 0;
                break;
            }
            if (FCheckBuilderPart(0, &hs2, 1) != 0 && FCheckBuilderPart(1, &hs, 1) != 0 && FCheckBuilderPart(2, &hs3, 1) != 0 &&
                FCheckBuilderPart(3, &hs3, 1) != 0)
                goto L_818e;
            TutorError(507);
            t_merge_81d4_0001 = 0;
            break;
        case 20:
            if (tutor.idt == 416) {
                hs.grhst = hstSpecialSB;
                hs.iItem = 0;
                hs.cItem = 1;
                if (FCheckBuilderPart(0, &hs, 1) == 0) {
                    TutorError(515);
                    t_merge_81d4_0001 = 0;
                    break;
                }
                if (fstricmp(PszGetCompressedString(idsGater), lpshdefBuild->hul.szClass) != 0) {
                    TutorError(512);
                    t_merge_81d4_0001 = 0;
                    break;
                }
                if (lpshdefBuild->hul.ibmp == 137)
                    goto L_818e;
                TutorError(505);
                t_merge_81d4_0001 = 0;
                break;
            }
            TutorError(504);
            t_merge_81d4_0001 = 0;
            break;
        case 22:
            if (tutor.idt == 448) {
                hs.grhst = hstEngine;
                hs.iItem = 4;
                hs.cItem = 1;
                hs1.grhst = hstMines;
                hs1.iItem = 1;
                hs1.cItem = 3;
                if (FCheckBuilderPart(0, &hs, 1) == 0 || FCheckBuilderPart(2, &hs1, 3) == 0) {
                    TutorError(508);
                    t_merge_81d4_0001 = 0;
                    break;
                }
                if (fstricmp(PszGetCompressedString(idsMineLayer), lpshdefBuild->hul.szClass) == 0)
                    goto L_818e;
                TutorError(513);
                t_merge_81d4_0001 = 0;
                break;
            }
            TutorError(504);
            t_merge_81d4_0001 = 0;
            break;
        case 25:
            hs.grhst = hstEngine;
            hs.iItem = 4;
            hs.cItem = 1;
            hs2.grhst = hstSpecialM;
            hs2.iItem = 0;
            hs2.cItem = 1;
            if (FCheckBuilderPart(0, &hs, 1) != 0 && FCheckBuilderPart(1, &hs2, 1) != 0)
                goto L_818e;
            TutorError(507);
            t_merge_81d4_0001 = 0;
            break;
        case 27:
            if (tutor.idt == 544) {
                hs.grhst = hstEngine;
                hs.iItem = 10;
                hs.cItem = 1;
                hs1.grhst = hstSpecialM;
                hs1.iItem = 5;
                hs1.cItem = 1;
                hs2.grhst = hstSpecialE;
                hs2.iItem = 5;
                hs2.cItem = 1;
                hs3.grhst = hstBeam;
                hs3.iItem = 3;
                hs3.cItem = 1;
                hs4.grhst = hstArmor;
                hs4.iItem = 2;
                hs4.cItem = 2;
                if (FCheckBuilderPart(0, &hs, 1) == 0 || FCheckBuilderPart(1, &hs3, 1) == 0 || FCheckBuilderPart(2, &hs3, 1) == 0 ||
                    FCheckBuilderPart(3, &hs3, 1) == 0 || FCheckBuilderPart(4, &hs4, 2) == 0 || FCheckBuilderPart(5, &hs1, 1) == 0 ||
                    FCheckBuilderPart(6, &hs2, 1) == 0) {
                    TutorError(506);
                    t_merge_81d4_0001 = 0;
                    break;
                }
                if (lpshdefBuild->hul.ibmp == 25)
                    goto L_818e;
                TutorError(505);
                t_merge_81d4_0001 = 0;
                break;
            }
            TutorError(504);
            t_merge_81d4_0001 = 0;
            break;
        case 29:
            if (tutor.idt == 576) {
                hs.grhst = hstEngine;
                hs.iItem = 10;
                hs.cItem = 2;
                hs1.grhst = hstSpecialM;
                hs1.iItem = 5;
                hs1.cItem = 1;
                hs2.grhst = hstBomb;
                hs2.iItem = 1;
                hs2.cItem = 4;
                if (FCheckBuilderPart(0, &hs, 2) != 0 && FCheckBuilderPart(1, &hs2, 4) != 0 && FCheckBuilderPart(2, &hs2, 4) != 0 &&
                    FCheckBuilderPart(3, &hs1, 1) != 0)
                    goto L_818e;
                TutorError(509);
                t_merge_81d4_0001 = 0;
            } else {
                TutorError(504);
                t_merge_81d4_0001 = 0;
            }
        }
        break;
    L_818e:
        t_merge_81d4_0001 = 1;
        break;
    case 4:
        TutorError(503);
        t_merge_81d4_0001 = 0;
    }
    return t_merge_81d4_0001;
NoCustom:
    TutorError(498);
    t_merge_81d4_0001 = 0;
    return t_merge_81d4_0001;
}

int16_t FOKMergeDialog() {
    if (game.turn != 23) {
        if (game.turn != 27) {
            TutorError(516);
            return 0;
        }
        if (*vrgiflMerge == -1 && vrgiflMerge[1] == -1 && vrgiflMerge[2] != -1 && vrgiflMerge[3] != -1) {
            return 1;
        }
    } else if (*vrgiflMerge != -1 && vrgiflMerge[1] != -1 && vrgiflMerge[2] == -1 && vrgiflMerge[3] == -1) {
        return 1;
    }
    TutorError(514);
    tutor.idh = 1107;
    return 0;
}
