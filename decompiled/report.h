#ifndef STARS_DECOMPILED_REPORT_H
#define STARS_DECOMPILED_REPORT_H

#include <stdint.h>
#include <windows.h>

extern uint16_t mpicolgrbitBU[12];

LRESULT CALLBACK ReportDlg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void             SetHScrollBar();
void             DrawReport(HWND hwnd, HDC hdc, RECT *prc);
INT_PTR CALLBACK ScoreXDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void             InitScoreDlg(HWND hwnd, int16_t fVictory);
void             DrawVCReport(HDC hdc);
void             DrawScoreReport(HDC hdc);
void             DrawHistoryReport(HDC hdc);
int32_t          LFetchScoreXVal(SCOREX *lpsx, int16_t iVal);
int16_t          DxReportColHdr(int16_t irpt, int16_t iCol, char *psz, HDC hdc);
void             DrawReportItem(HDC hdc, RECT *prc, int16_t irpt, int16_t irow, int16_t icol);
void             DrawMineralItem(HDC hdc, int16_t x, int16_t y, int16_t iMineral, int32_t l);
char            *PszGetDestName(FLEET *lpfl, HDC hdc);
int16_t          FDestIsWP0(FLEET *lpfl);
char            *PszGetETA(HDC hdc, FLEET *lpfl, int16_t *pcYears);
char            *PszGetTaskName(FLEET *lpfl, int16_t *picr);
void             SortReportCache(int16_t irpt, int16_t icol);
int16_t          ICompReport(uint16_t *pid1, uint16_t *pid2);
void             ReportColumnPopup(POINT pt, int16_t icol, int16_t fRightBtn);
void             InvalidateReport(int16_t irpt, int16_t fReload);
void             ExecuteReportClick(POINT pt, int16_t irpt, int16_t icol, int16_t irow);
void             DumpUniverse();
void             DumpPlanets();
void             DumpFleets();
INT_PTR CALLBACK PrintMapDlg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
