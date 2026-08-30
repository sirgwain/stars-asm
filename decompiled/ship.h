#ifndef STARS_DECOMPILED_SHIP_H
#define STARS_DECOMPILED_SHIP_H

#include <stdint.h>
#include <windows.h>

void    DrawShipOrders(HDC hdc, TILE *ptile, OBJ obj);
void    DrawShipWayPtOrders(HDC hdc, TILE *ptile, OBJ obj);
void    DrawShipPlanet(HDC hdc, TILE *ptile, OBJ obj);
void    DrawShipCargo(HDC hdc, TILE *ptile, OBJ obj);
void    DrawFleetComp(HDC hdc, TILE *ptile, OBJ obj);
int16_t FCanSplit(int32_t cBoat);
int16_t FCanSplitAll(int32_t cBoat);
int16_t FCanMerge(FLEET *pfl);
void    ShipCommandProc(HWND hwnd, uint16_t wParam, int32_t lParam);
void    SelectAdjFleet(int16_t dInc, int16_t idFleet);
void    SetFleetDropDownSel(int16_t id);
int32_t LGetFleetStat(FLEET *lpfl, int16_t grStat);
int16_t WtMaxShdefStat(SHDEF *lpshdef, int16_t grStat);
void    DrawFleetGauge(HDC hdc, RECT *prc, FLEET *lpfl, int16_t grbit);
void DrawFleetBitmap(FLEET *lpfl, HDC hdc, int16_t x, int16_t y, int16_t fFrame, int16_t ibmp, int16_t cDiff, int16_t fShrink, int16_t ibmpRace, int16_t csh);
int16_t  FEnumCalcJettison(void *lprt, RecordType rt, int16_t cb, PLANET *lppl, int16_t iFleet);
int16_t  TransferStuff(int16_t id1, GrobjClass grobj1, int16_t id2, GrobjClass grobj2, int16_t mdXfer);
int16_t  TransferDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t  FTrackXfer(HWND hwnd, int16_t x, int16_t y, int16_t fkb);
int32_t  GetCargoFree(FLEET *lpfl);
int32_t  GetFuelFree(FLEET *lpfl);
int32_t  ChgCargo(GrobjClass grobj, int16_t id, int16_t iSupply, int32_t dChg, void *pobj);
int32_t  XferSupply(int16_t iSupply, int32_t cQuan);
void     UpdateXferBtns();
void     DrawXferDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iSupply);
void     GetXferLeftRightRcs(RECT *prcWhole, RECT *prcLeft, RECT *prcRight);
int16_t  FSetupXferBtns(RECT *prc);
void     DrawThingXferSide(HDC hdc, RECT *prc, THING *pth, int16_t iSupply);
void     DrawFleetCargoXferSide(HDC hdc, RECT *prc, FLEET *pfl, int16_t iSupply);
void     DrawFleetShipsXferSide(HDC hdc, RECT *prc, FLEET *pfl, int16_t iSupply);
void     DrawPlanetXferSide(HDC hdc, RECT *prc, PLANET *ppl, int16_t iSupply);
uint16_t ClickInShipOrders(POINT pt, int16_t sks, int16_t fCursor, int16_t fRightBtn);
void     FillFleetCompLB();
void     FillOrdersLB();
void     SetOrdersLbSel(int16_t iSel);
void     UpdateOrdersDDs(int16_t iLevel);
void     FillBattleDD(int16_t iSel);
void     DeleteCurWayPoint(int16_t fBackup);
void     DeleteWpFar(FLEET *lpfl, int16_t iDel, int16_t fRecycle);
int32_t  EstFuelUse(FLEET *lpfl, int16_t iOrd, int16_t iWarp, int32_t dTravel, int16_t fRangeOnly);
int32_t  FakeEditProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam);
int16_t  IFindIdealWarp(FLEET *lpfl, int16_t fIgnoreScoops);
int32_t  LFuelUseToWaypoint(FLEET *lpfl, int16_t iwp, int16_t fMaxCargo);
void     FleetTransferCargoBalance(FLEET *pflNew1, FLEET *pflNew2);
void     DestroyAllIshdefSB(int16_t ishdefSB, int16_t iplr);
void     DestroyAllIshdef(int16_t ishdef, int16_t iplr);
void     RemoveIshdefFromAllQueues(int16_t ishdef, int16_t fSpaceDocks);
int16_t  CshQueued(int16_t ishdef, int16_t *pfProgress, int16_t fSpaceDocks);
void     Merge2Fleets(FLEET *lpflDst, FLEET *lpflDel, int16_t fNoDelete);
void     FleetOrdersChangeTarget(FLEET *lpflOld);
void     GetTruePartCost(int16_t iPlayer, PART *ppart, uint16_t *rgCost);

#endif
