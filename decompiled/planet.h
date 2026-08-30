#ifndef STARS_DECOMPILED_PLANET_H
#define STARS_DECOMPILED_PLANET_H

#include <stdint.h>
#include <windows.h>

int32_t  PlanetWndProc(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void     DrawPlanShip(HDC hdc, int16_t grbit);
int16_t  FDrawTileNC(HDC hdc, TILE *ptile, RECT *prc, char *pszTitle);
void     DrawPlanetMinSum(HDC hdc, TILE *ptile, OBJ obj);
void     DrawPlanetStats(HDC hdc, TILE *ptile, OBJ obj);
int16_t  FGetBestDefensePart(PART *ppart);
void     DrawPlanetStarbase(HDC hdc, TILE *ptile, OBJ obj);
void     DrawMassWarpGauge(HDC hdc, RECT *prc, int16_t iBest, int16_t iCur);
void     DrawPlanetProduction(HDC hdc, TILE *ptile, OBJ obj);
char    *PszProductionETA(PLANET *lppl, PLPROD *lpplprod, ProdItemType iItem, int16_t *etaFirst, int16_t *etaLast);
void     DrawPlanShipBitmap(HDC hdc, TILE *ptile, OBJ obj);
void     DrawPlanetShipList(HDC hdc, TILE *ptile, OBJ obj);
void     SetPlanetTitleBar(HWND hwnd);
void     ChangeMainObjSel(GrobjClass grobjNew, int16_t iObjSel);
void     FillShipDD(int16_t idSkip);
void     SelectAdjPlanet(int16_t dInc, int16_t idPlanet);
int16_t  IdFindAdjStarbase(int16_t idPlanet, int16_t fNext);
void     PlanetClick(int16_t x, int16_t y, int16_t sks, int16_t fRightBtn);
uint16_t ClickInPlanetOrders(POINT pt, int16_t sks, int16_t fCursor, int16_t fRightBtn);
void     EnsureTileSize(int16_t fSmallTiles);
void     ReflowColumn(int16_t iCol, int16_t iTile, int16_t fRedraw);
int16_t  IBestTerraform(PLANET *lppl, int16_t fHelp);
char    *PszCalcEnvVar(int16_t iEnv, int16_t iVar);
char    *PszCalcGravity(int16_t iGravity);
void     HandleFocusState(DRAWITEMSTRUCT *lpdis, int16_t inflate);
void     DrawCBEntireItem(DRAWITEMSTRUCT *lpdis, int16_t inflate);
void     DrawProductionItem(HDC hdc, RECT *prc, char *psz, int16_t inflate, int16_t fSelected, int16_t fListbox);
void     FillPlanetProdLB(HWND hwnd, PLPROD *lpplprod, PLANET *lppl);
int16_t  PctPlanetCapacity(PLANET *lppl);
int16_t  PctPlanetOptValue(PLANET *lppl, int16_t iPlr);
int16_t  PctPlanetDesirability(PLANET *lppl, int16_t iPlr);
int32_t  CalcPlanetMaxPop(int16_t idpl, int16_t iplr);
int16_t  CMaxMines(PLANET *lppl, int16_t iplr);
int16_t  CMaxOperableMines(PLANET *lppl, int16_t iplr, int16_t fNextYear);
int16_t  CMinesOperating(PLANET *lppl);
int16_t  CFactoriesOperating(PLANET *lppl);
int16_t  CMaxFactories(PLANET *lppl, int16_t iplr);
int16_t  CMaxOperableFactories(PLANET *lppl, int16_t iplr, int16_t fNextYear);
int16_t  CMaxDefenses(PLANET *lppl, int16_t iplr);
int16_t  CMaxOperableDefenses(PLANET *lppl, int16_t iplr, int16_t fNextYear);
int16_t  CResourcesAtPlanet(PLANET *lppl, int16_t iplr);
int16_t  IWarpMAFromLppl(PLANET *lppl, int16_t *pfTwo);
int16_t  StargateRangeFromLppl(PLANET *lppl, int16_t iplr, int16_t ish);
int16_t  FProdIsTerra(PROD *lpprod);
int16_t  IpctCanTerraformLppl(PLANET *lppl);
int16_t  FCanTerraformLppl(PLANET *lppl, int16_t *rgEnvMin, int16_t *rgEnvMax, int16_t *rgEnvCost, int16_t fHelp);
void     UninhabitPlanet(PLANET *lppl);
int16_t  PctCloakFromHuldef(HUL *lphul, int16_t iplr, int16_t *ppctSteal);

#endif
