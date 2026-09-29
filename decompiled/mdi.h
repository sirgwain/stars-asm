#ifndef STARS_DECOMPILED_MDI_H
#define STARS_DECOMPILED_MDI_H

#include <stdint.h>
#include <windows.h>

extern uint8_t vrgbShuffleSerial[21];
extern char    rgTOWidth[2][2];

int16_t          InitMDIApp();
void             CreateChildWindows();
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
POINT16          InvertPaneBorder(HDC hdc, int16_t grSel, POINT16 dpt, POINT16 *pdptPrev);
HCURSOR          HcrsFromFrameWindowPt(POINT16 pt, int16_t *pgrSel);
void             RestoreSelection();
void             FormatSerialAndEnv(int32_t lSerial, uint8_t *pbEnv, char *pszOut);
int16_t          FSerialAndEnvFromSz(int32_t *plSerial, uint8_t *pbEnv, char *pszIn);
int16_t          FFindSomethingAndSelectIt();
void             CommandHandler(HWND hwnd, WPARAM wParam);
void             InitializeMenu(HMENU hmenu);
void             EnsureAis();
HMENU            GetASubMenu(HWND hwnd, int16_t iMenu);
int16_t          FOpenGame(HWND hwnd, int16_t fRaceOnly);
int16_t          FWasRaceFile(char *szFile, int16_t fChkPass);
void             BringUpHostDlg();
void             DrawHostDialog2(HWND hwnd, HDC hdcIn);
void             VerifyTurns();
int16_t          CTurnsOutSafe();
int16_t          CFindTurnsOutstanding();
INT_PTR CALLBACK HostModeDialog(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK HostOptionsDialog(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void             DrawHostOptions(HWND hwnd, HDC hdc, int16_t iDraw);
VOID CALLBACK    HostTimerProc(HWND hwnd, UINT msg, UINT_PTR idTimer, DWORD dwTime);
void             GetWindowRc(HWND hwnd, RECT *prc);
void             SetWindowIniString(char *sz, HWND hwnd);
void             WriteIniSettings();
void             RefitFrameChildren();
LRESULT CALLBACK TitleWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
