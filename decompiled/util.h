#ifndef STARS_DECOMPILED_UTIL_H
#define STARS_DECOMPILED_UTIL_H

#include <stdint.h>
#include <windows.h>

extern uint32_t rgcrDrawStars2b[5];
extern uint32_t rgcrDrawStars2a[5];
extern uint32_t rgcrDrawStars[5];
extern int32_t  rgDSDivCnt2[5];
extern int32_t  rgDSDivCnt[5];
extern uint8_t  vrgbTachyon[18];

int16_t  FLookupSelPlanet(PLANET *ppl);
int16_t  FDupPlanet(PLANET *lppl, PLANET *ppl);
THING   *LpthFromId(int16_t idth);
PLANET  *LpplFromId(int16_t idPlanet);
void     CalcPctSurvive(PLANET *lppl, float *ppct, float *ppctSmart);
int16_t  FLookupPlanet(int16_t iPlanet, PLANET *ppl);
int32_t  DpOfLpflIshdef(FLEET *lpfl, int16_t ishdef);
int16_t  FLookupThing(int16_t idth, THING *pth);
void     SelectOursAtObject(POINT *ppt);
int32_t  LComputePower(SHDEF *lpshdef);
void     ComputeShdefPowers();
int32_t  DpShieldOfShdef(SHDEF *lpshdef, int16_t iplr);
int16_t  IStargateFromLppl(PLANET *lppl);
char    *PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr);
int16_t  FCalcFleetBombDamage(FLEET *lpfl, int32_t *pdmgPeople, int32_t *pdmgPeopleMin, int32_t *pdmgPeopleSmart, int32_t *pdmgBldg, int32_t *ppctTerra,
                              int16_t *pfMulti);
void     LinkFleets(int16_t fUnused);
int16_t  ICompFleetPoint(void *arg1, void *arg2);
int16_t  ICompFleetPoint2(void *arg1, void *arg2);
int16_t  FLookupSelShip(FLEET *pfl);
FLEET   *LpflFromId(int16_t idFleet);
int16_t  FLookupFleet(int16_t idFleet, FLEET *pfl);
int16_t  FDupFleet(FLEET *lpfl, FLEET *pfl);
int16_t  FLookupObject(GrobjClass grobj, int16_t id, void *pobj);
int16_t  FLookupOrbitingXfer(int16_t idPlanet, int16_t iNth, XFER *pxf, int16_t idSkip);
char    *PszGetThingName(int16_t id);
char    *PszGetFleetName(int16_t id);
uint16_t WFromLpfl(FLEET *lpfl);
char    *PszFleetNameFromWord(uint16_t w);
char    *PszGetPlanetName(int16_t id);
int16_t  IflFromLpfl(FLEET *lpfl);
int16_t  FDeleteFleet(int16_t idFleet, GrobjClass grobjSel, int16_t idSel);
FLEET   *LpflNew(int16_t iPlr, int16_t idPl);
FLEET   *LpflNewSplit(FLEET *pfl);
int16_t  FFleetMergeAll(FLEET *pfl);
int16_t  FFleetSplitAll(FLEET *pfl);
char    *PszGetLocName(GrobjClass grobj, int16_t id, int16_t x, int16_t y);
int16_t  CchGetETA(HDC hdc, FLEET *lpfl, char *sz, int16_t iwp, int16_t fSmall);
int16_t  IshdefPrimaryFromLpfl(FLEET *lpfl, int16_t *pcDiff);
char    *PszGetDistance(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
double   DGetDistance(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
int16_t  FFindNearestObject(POINT pt, GrobjClass grobj, SCAN *pscan);
void     UpdateShdefCost(SHDEF *lpshdef);
uint16_t WPackLong(int32_t l);
int16_t  GetPlanetScannerRange(PLANET *lppl, int16_t *pDeep);
int16_t  GetCachedFleetScannerRange(FLEET *lpfl, int16_t *pdPlanRange, int16_t *ppctDetect, int16_t *piSteal);
int16_t  GetFleetScannerRange(FLEET *lpfl, int16_t *pdPlanRange, int16_t *ppctDetect, int16_t *piSteal);
int16_t  GetShdefScannerRange(SHDEF *lpshdef, int16_t iplr, int16_t *pdPlanRange, int16_t *ppctDetect, int16_t *piSteal);
int32_t  LCalcFuelGainFromRamScoops(FLEET *lpfl, int16_t iWarp, int32_t dTravel);
int32_t  CalcPlayerScore(int16_t iPlr, SCORE *pscore);
void     GetTrueHullCost(int16_t iPlayer, HUL *lphul, uint16_t *rgCost);
void     DecorateHullName(int16_t iplr, int16_t ish, char *psz);
void     DrawABunchOfStars(HDC hdc, RECT *prc);
int32_t  LongFromSerialCh(char ch);
int16_t  FValidSerialNo(char *psz, int32_t *plSerial);
int16_t  FMatchTarget(FLEET *lpflTarget, int16_t mdTarget, int16_t fExact);
void     ValidateWaypoints();
int32_t  ChgPopFromPlanet(PLANET *lppl, int16_t fUpdate);
int16_t  FCanFleetUseStargates(FLEET *lpfl, POINT ptSrc, POINT ptDst);
int16_t  FFleetCanJumpgate(FLEET *lpfl);
int32_t  WtFromLpfl(FLEET *lpfl);
int16_t  FCanBuildShdef(SHDEF *lpshdef, int16_t iplr);
int16_t  IshFindSimilarDesign(HUL *lphul, int16_t iPlrDst);
void     DrawPlanetPrintDot(HDC hdc, int16_t x, int16_t y, int16_t iSize);
void     ClearFile(int16_t dt);
void     OutputSz(int16_t dt, char *sz);
void     TurnLog(StringId ids);

#endif
