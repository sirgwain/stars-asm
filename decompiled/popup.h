#ifndef STARS_DECOMPILED_POPUP_H
#define STARS_DECOMPILED_POPUP_H

#include <stdint.h>
#include <windows.h>

extern BattleUnitFlags mpimdgrbitBU[8];

LRESULT CALLBACK PopupWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int16_t          FIsPopupHullType(int16_t ishdef);
void             DrawPopup(HWND hwnd, HDC hdc);
void             Popup(HWND hwnd, int16_t x, int16_t y);
int16_t          PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn);
POINT16          PtDisplayPlanetStateInfo(HDC hdc, int16_t fPrint);
POINT16          PtDisplayPlanetPopInfo(HDC hdc, int16_t fPrint);
POINT16          PtDisplayZipOrdInfo(HDC hdc, int16_t xCtr, int16_t fPrint);
POINT16          PtDisplayFactoryMineInfo(HDC hdc, int16_t dx, int16_t fPrint);
POINT16          PtDisplayResourceInfo(HDC hdc, int16_t dx, int16_t fPrint);
POINT16          PtDisplayString(HDC hdc, int16_t dx, int16_t fPrint);

#endif
