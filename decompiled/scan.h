#ifndef STARS_DECOMPILED_SCAN_H
#define STARS_DECOMPILED_SCAN_H

#include <stdint.h>
#include <windows.h>

extern uint32_t rgcrScanMine[3];
extern int16_t  vrgPopRad[19];

int32_t ScannerWndProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam);
int16_t PtToScan(int16_t d);
int16_t ScanToPt(int16_t d);
int16_t DrawScanner(HDC hdc, RECT *prc);
void    DrawScanFleetCount(FLEET *lpfl, int16_t x, int16_t y, HDC hdc, HDC hdcMem);
int32_t CShipsScanVis(FLEET *lpfl);
void    DrawRadarCircle(DRAWCIR *pdc, RECT *prc);
void    DrawShipScanPath(HDC hdc, int16_t fShow);
void    DrawScannerSBar(HDC hdc, RECT *prc, SBAR *psbar, int16_t fFullRedraw);
void    DrawLockLight(HDC hdc, RECT *prc, int16_t fFullRedraw);
void    SetScanScrollBars(HWND hwnd);
void    ScrollScanner(int16_t dx, int16_t dy);
void    RedrawScanSel(HDC hdc, int16_t fVis);
int16_t FEnsurePointOnScreen(POINT pt, int16_t fScroll);
void    CtrPointScan(POINT pt, int16_t fScroll);
void    LogicalToScan(POINT *ppt);
void    ScanToLogical(POINT *ppt);
int16_t FAddWayPoint(POINT ptIn, SCAN *pscan);
int16_t IWarpBestForWaypoint(FLEET *lpfl, ORDER *lpord);
int16_t FNearAWayPoint(POINT pt, int16_t fLogical);
int16_t FHandleWayPointDrag(POINT pt);
void    DrawScanXorLines(HDC hdc, POINT *rgpt, int16_t cpt);
int16_t SetScanWp(int16_t iNew);
void    ChangeScanSel(SCAN *pscan, int16_t fValidScan);
int16_t FGetNextObjHere(SCAN *pscan, int16_t fOnlyOurs);
int16_t FindDlg(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam);
int16_t FSelectSz(char *szName);
void    GetScanFleetOrientation(FLEET *lpfl, POINT *ppt, POINT *pptD);
void    GetDxDyOrientation(int16_t dx, int16_t dy, POINT *ppt, POINT *pptD);
int16_t FHandleMeasuringTape(SCAN *pscan, POINT pt);

#endif
