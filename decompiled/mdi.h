#ifndef STARS_DECOMPILED_MDI_H
#define STARS_DECOMPILED_MDI_H

#include <stdint.h>
#include <windows.h>

extern uint8_t vrgbShuffleSerial[21];
extern char    rgTOWidth[2][2];

int16_t  InitMDIApp();
void     CreateChildWindows();
int32_t  FrameWndProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam);
POINT    InvertPaneBorder(HDC hdc, int16_t grSel, POINT dpt, POINT *pdptPrev);
uint16_t HcrsFromFrameWindowPt(POINT pt, int16_t *pgrSel);
void     RestoreSelection();
void     FormatSerialAndEnv(int32_t lSerial, uint8_t *pbEnv, char *pszOut);
int16_t  FSerialAndEnvFromSz(int32_t *plSerial, uint8_t *pbEnv, char *pszIn);
int16_t  FFindSomethingAndSelectIt();
void     CommandHandler(HWND hwnd, uint16_t wParam);
void     InitializeMenu(HMENU hmenu);
void     EnsureAis();
uint16_t GetASubMenu(HWND hwnd, int16_t iMenu);
int16_t  FOpenGame(HWND hwnd, int16_t fRaceOnly);
int16_t  FWasRaceFile(char *szFile, int16_t fChkPass);
void     BringUpHostDlg();
void     DrawHostDialog2(HWND hwnd, HDC hdcIn);
void     VerifyTurns();
int16_t  CTurnsOutSafe();
int16_t  CFindTurnsOutstanding();
int16_t  HostModeDialog(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t  HostOptionsDialog(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void     DrawHostOptions(HWND hwnd, HDC hdc, int16_t iDraw);
void     HostTimerProc(HWND hwnd, WMType msg, uint16_t idTimer, uint32_t dwTime);
void     GetWindowRc(HWND hwnd, RECT *prc);
void     SetWindowIniString(char *sz, HWND hwnd);
void     WriteIniSettings();
void     RefitFrameChildren();
int32_t  TitleWndProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam);

#endif
