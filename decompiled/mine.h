#ifndef STARS_DECOMPILED_MINE_H
#define STARS_DECOMPILED_MINE_H

#include <stdint.h>
#include <windows.h>

int32_t    MineWndProc(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void       InvalidateMineralBars();
void       GetMineFieldCounts(uint16_t id, int16_t *pithm, int16_t *pcthm);
void       DrawMineSurvey(HDC hdc, RECT *prc);
HtMineType HtMineWindow(HWND hwnd, int16_t x, int16_t y);
void       MineClick(int16_t x, int16_t y, int16_t msg, int16_t sks);
void       SetMineralTitleBar(HWND hwnd);
void       DrawSelectionArrow(HDC hdc, RECT *prc, int16_t fEnabled);
void       DrawDiamond(HDC hdc, RECT *prc, HBRUSH hbr);
int16_t    FOtherStuffAtScanSel();
void       PopupMineralScanChoices(HWND hwnd, int16_t x, int16_t y);
void       EstMineralsMined(PLANET *lppl, int32_t *plQuan, int32_t cMines, int16_t fApply);

#endif
