#ifndef STARS_DECOMPILED_BUILD_H
#define STARS_DECOMPILED_BUILD_H

#include <stdint.h>
#include <windows.h>

extern uint16_t     rggrbitPartsSB[8];
extern int16_t      rgidsPartsSB[8];
extern uint16_t     rggrbitParts[13];
extern int16_t      rgidsParts[13];
extern HullSlotType rghstCat[14];
extern StringId     rgidsCat[14];

int16_t ShipBuilder(POINT ptDlgSize);
void    ShowMainControls(HWND hwnd, ShowWindowCmd sw);
int16_t FCheckQueuedShip(HWND hwnd, SHDEF *lpshdef, int16_t fEdit);
int16_t SlotDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void    DrawSlotDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iDraw);
int16_t FTrackSlot(HWND hwnd, int16_t x, int16_t y, int16_t fkb, int16_t fListBox, int16_t fRightBtn);
void    DrawBuildSelComp(HWND hwnd, HDC hdc, int16_t iDraw);
int16_t PctJammerFromHul(HUL *lphul);
void    DrawBuildSelHull(HWND hwnd, HDC hdc, int16_t iDraw, RECT *prc);
void    SetBuildSelection(int16_t iSrc);
int16_t IDropPart(POINT pt, HS hsSrc, int16_t iSrc, int16_t fNoModify);
void    DrawDlgLBEntireItem(DRAWITEMSTRUCT *lpdis, int16_t inflate);
SHDEF  *NthValidShdef(int16_t n);
SHDEF  *NthValidEnemyShdef(int16_t n);
void    FillBuildDD(HWND hwndDD, MdBuild md);
void    FillBuildPartsLB(HWND hwndLB, int16_t grbit);
void    UpdateSlotGlobals();
int16_t IEmptyBmpFromGrhst(int16_t grhst);
int32_t FakeListProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam);
void    MakeNewName(char *lpsz);
void    KillQueuedMassPackets(PLANET *lppl);
void    KillQueuedShips(PLANET *lppl);

#endif
