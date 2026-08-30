#ifndef STARS_DECOMPILED_TB_H
#define STARS_DECOMPILED_TB_H

#include <stdint.h>
#include <windows.h>

extern char    vrgTBBtn[29];
extern int16_t vrgpctZoom[9];

int32_t TbWndProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam);
void    DrawToolbar(HDC hdc, RECT *prc);
void    DrawBitmapButton(HDC hdc, POINT pt, int16_t ibtn, int16_t fDown);
int16_t ItbFromPpt(POINT *ppt);
int16_t DxOfBtn(int16_t itb);
int16_t FIsButtonDown(int16_t itb);
void    ExecuteButton(int16_t itb, int16_t fDown);
void    TerminateToolbarFocus(int16_t fCancel);
void    ShowTooltip(StringId ids, RECT *prc);
int32_t TooltipWndProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam);
int32_t FakeComboProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam);
int32_t FakeCEProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam);

#endif
