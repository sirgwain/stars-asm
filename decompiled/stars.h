#ifndef STARS_DECOMPILED_STARS_H
#define STARS_DECOMPILED_STARS_H

#include <stdint.h>
#include <windows.h>

int WINAPI       WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
int16_t          FSetUpBatchProcessing();
int16_t          IPlrAlsoCheater(int16_t iplr);
int16_t          FGetSystemColors();
void             FreeStuff();
char            *SzVersion();
INT_PTR CALLBACK About(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK OrderInfoDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int16_t          FHandleChar(HWND hwnd, uint16_t ch, int32_t lParam);
int16_t          FHandleKey(HWND hwnd, int16_t iMsg, int16_t iKey, uint32_t dw);

#endif
