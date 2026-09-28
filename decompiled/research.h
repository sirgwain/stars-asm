#ifndef STARS_DECOMPILED_RESEARCH_H
#define STARS_DECOMPILED_RESEARCH_H

#include <stdint.h>
#include <windows.h>

extern uint16_t rggrbitBrParts[17];
extern int32_t  rglTechCost[27];

INT_PTR CALLBACK ResearchDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void             DrawResearchDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t grbitDraw);
int16_t          FTrackResearchDlg(HWND hwnd, int16_t x, int16_t y, int16_t fkb);
int32_t          GetTechLevelCost(int16_t iTech, int16_t iLevel, int16_t iplr);
INT_PTR CALLBACK BrowserDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BrowserWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void             DisplayComponentInfo(HDC hdc, int16_t dx, int16_t dy, PART *ppart);
int32_t          ProjectedResearchSpending(int32_t pct);
int32_t          CostOfDevelopingItem(char *rgTech);
int16_t          FShouldPartBeHidden(PART *ppart);

#endif
