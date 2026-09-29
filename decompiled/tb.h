#ifndef STARS_DECOMPILED_TB_H
#define STARS_DECOMPILED_TB_H

#include <stdint.h>
#include <windows.h>

extern char    vrgTBBtn[29];
extern int16_t vrgpctZoom[9];

LRESULT CALLBACK TbWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void             DrawToolbar(HDC hdc, RECT *prc);
void             DrawBitmapButton(HDC hdc, POINT16 pt, int16_t ibtn, int16_t fDown);
int16_t          ItbFromPpt(POINT16 *ppt);
int16_t          DxOfBtn(int16_t itb);
int16_t          FIsButtonDown(int16_t itb);
void             ExecuteButton(int16_t itb, int16_t fDown);
void             TerminateToolbarFocus(int16_t fCancel);
void             ShowTooltip(StringId ids, RECT *prc);
LRESULT CALLBACK TooltipWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FakeComboProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FakeCEProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
