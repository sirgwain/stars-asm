#ifndef STARS_DECOMPILED_STARS_H
#define STARS_DECOMPILED_STARS_H

#include <stdint.h>
#include <windows.h>

int16_t WinMain(uint16_t hInstance, uint16_t hPrevInstance, char *lpCmdLine, int16_t nCmdShow);
int16_t FSetUpBatchProcessing();
int16_t IPlrAlsoCheater(int16_t iplr);
int16_t FGetSystemColors();
void    FreeStuff();
char   *SzVersion();
int16_t About(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t OrderInfoDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t FHandleChar(HWND hwnd, uint16_t ch, int32_t lParam);
int16_t FHandleKey(HWND hwnd, int16_t iMsg, int16_t iKey, uint32_t dw);

#endif
