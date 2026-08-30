#ifndef STARS_DECOMPILED_POPUP_H
#define STARS_DECOMPILED_POPUP_H

#include <stdint.h>
#include <windows.h>

extern uint16_t mpimdgrbitBU[8];

int32_t PopupWndProc(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t FIsPopupHullType(int16_t ishdef);
void    DrawPopup(HWND hwnd, HDC hdc);
void    Popup(HWND hwnd, int16_t x, int16_t y);
int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn);
POINT   PtDisplayPlanetStateInfo(HDC hdc, int16_t fPrint);
POINT   PtDisplayPlanetPopInfo(HDC hdc, int16_t fPrint);
POINT   PtDisplayZipOrdInfo(HDC hdc, int16_t xCtr, int16_t fPrint);
POINT   PtDisplayFactoryMineInfo(HDC hdc, int16_t dx, int16_t fPrint);
POINT   PtDisplayResourceInfo(HDC hdc, int16_t dx, int16_t fPrint);
POINT   PtDisplayString(HDC hdc, int16_t dx, int16_t fPrint);

#endif
