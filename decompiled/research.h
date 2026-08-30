#ifndef STARS_DECOMPILED_RESEARCH_H
#define STARS_DECOMPILED_RESEARCH_H

#include <stdint.h>
#include <windows.h>

extern uint16_t rggrbitBrParts[17];
extern int32_t  rglTechCost[27];

int16_t ResearchDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void    DrawResearchDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t grbitDraw);
int16_t FTrackResearchDlg(HWND hwnd, int16_t x, int16_t y, int16_t fkb);
int32_t GetTechLevelCost(int16_t iTech, int16_t iLevel, int16_t iplr);
int16_t BrowserDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int32_t BrowserWndProc(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void    DisplayComponentInfo(HDC hdc, int16_t dx, int16_t dy, PART *ppart);
int32_t ProjectedResearchSpending(int32_t pct);
int32_t CostOfDevelopingItem(char *rgTech);
int16_t FShouldPartBeHidden(PART *ppart);

#endif
