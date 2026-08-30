#ifndef STARS_DECOMPILED_AIUTIL_H
#define STARS_DECOMPILED_AIUTIL_H

#include <stdint.h>
#include <windows.h>

extern int32_t vrgAiPacketDist[2];
extern uint8_t vrgSBAip[85];
extern uint8_t vrgSBMacAisb[6];
extern AIPART  vrgAiParts[150];

int16_t  FCreateAiShdef(int16_t ishdef, int16_t ihul, uint8_t *rgaip);
int16_t  FGetAIPart(int16_t aip, PART *ppart);
void     PickANameAndBmp(SHDEF *pshdef, StringId ids, int16_t cids, int16_t ibmpStart);
int16_t  FChangeAiShdef(SHDEF *pshdef, int16_t ishdef);
int16_t  XferAiSupply(GrobjClass grobjSrc, int16_t idSrc, GrobjClass grobjDst, int16_t idDst, int16_t iSupply, int16_t cQuan);
int16_t  XferAiTroopers(int16_t idSrc, int16_t idDst, int16_t cQuan);
int16_t  FColonizeAiFleet(FLEET *lpfl, int16_t idPlanet);
int16_t  FGotoWormholeAiFleet(FLEET *lpfl, THING *lpthWorm);
int16_t  IdNearestColonizablePlanet(FLEET *lpflCol, THING **plpthWorm);
THING   *LpthWormFind(POINT *ppt, int32_t d2);
uint32_t UlFleetPower(FLEET *lpfl);
int16_t  IdNearestUnknownPlanet(FLEET *lpfl, THING **plpthWorm);
void     AddMinesToBlockedQueues();
int16_t  FFleetInField(FLEET *lpfl, THING *lpth);
void     SetAiFleetIdealSpeed(FLEET *lpfl, int16_t wtFuelMax, int16_t cMinefields, THING **rglpth);
int16_t  IdTargetAttack(FLEET *lpfl, FLEET *lpflAtk, FLEET *lpflEnemy, int16_t fOnlyHumans);
int16_t  IdTargetFreighter(FLEET *lpflFr, PLANET *lpplHome);
int16_t  FSalvageTargetFreighter2(FLEET *lpflFr, int16_t fNeedy, int16_t iWorst, int16_t pctFull, int32_t wtCargoMax, int32_t scoreBest, THING **plpthBest,
                                  int16_t *pidBest);
int16_t  FMoveAiFleet(FLEET *lpfl, ORDER *pord, int16_t fAppend);
void     AddItemToQueue(uint16_t iItem, uint16_t cItem, GrobjClass grobj, int16_t mdAddItem);
int16_t  IroEnsureAi(uint8_t *lpbRes, int16_t cRes, int16_t *pishdefSBLatest, int16_t pct);
void     KeepFleetsMoving();
int16_t  FShouldWeBuildColonizers(int16_t *pcCol);
int16_t  FIsAiAttack(FLEET *lpfl);
int16_t  FIsTurinDroneAiAttack(FLEET *lpfl);
int16_t  FIsAiTransport(FLEET *lpfl);
void     ValidateStarbaseHistory();
void     GetResourcesAvailable(PLANET *lppl, int32_t *rgRes);
void     GetProdQCost(PLANET *lppl, int32_t *rgCost);
void     MergeAllShdefs(int16_t grbitish);
FLEET   *LpflFindClosestEnum(FLEET *lpfl, int16_t (**pfn)(FLEET *, FLEET *));
PLANET  *LpplFindClosestEnum(PLANET *lppl, int16_t (**pfn)(PLANET *, PLANET *));
PLANET  *LpplFindBestEnum(PLANET *lppl, int16_t (**pfn)(PLANET *, PLANET *));
int16_t  IdRandomPlanetNearby(POINT pt, int16_t cDist, int16_t fAvoidStarbases);
void     ClearAiCurrentTask(FLEET *lpfl, int16_t fChangeSel);
int16_t  FEnumOurStarbase(PLANET *lpplSrc, PLANET *lpplTest);
int16_t  FFleetMightHaveTeeth(FLEET *lpfl);
int16_t  IdTargetScout(FLEET *lpfl, FLEET *lpflAtk, FLEET *lpflEnemy, int16_t fOnlyHumans, THING **plpthWorm);
void     MarkPlanetsUnderAttack();
void     FixPlanetsUnderAttack(PROD *rgprod);
void     QuickBuildDefenses(PLANET *lppl, PROD *rgprod);
int16_t  IdplFindClosestStarbase(FLEET *lpfl, int16_t fBigOnes);
int16_t  FMoveToNearestStarbase(FLEET *lpfl, int16_t fBigOnes);
void     MoveToNearestPlanetOrEnemy(FLEET *lpfl, int16_t dEnemyRange);
void     EnsureAiStarbaseDesigns();
void     EnsureMacintiStarbaseDesigns(uint8_t *rgSB);
int16_t  FCreateAiStarbase(int16_t ishdef, int16_t iLevel, int16_t aisb, int16_t isb);
int16_t  FAIFling(PLANET *lppl, int32_t *rgResAvail);
int16_t  IshdefAiSBLatestOF();
int16_t  IshdefAiSBLatest();
void     QueueAiStarbases(PROD *rgprod, int16_t ishdefSBLatest);
int16_t  FUpgradeAiStarbase(PLANET *lppl, int16_t ishdefSBLatest);
int16_t  FQueueAiTerraforming(PLANET *lppl, int32_t *rgResAvail, int32_t *rgResCost);
int16_t  FQueueAiScanner(PLANET *lppl, int32_t *rgResAvail, int32_t *rgResCost);
int16_t  FQueueAiDefenses(PLANET *lppl, int32_t *rgResAvail, int32_t *rgResCost);
void     HandleBasicAiTasks(int16_t iroCur, PROD *rgprod, int16_t ishdefSBLatest, int32_t *rgResAvail, int32_t *rgResCost);
void     SplitOutShdefs(uint8_t *rgbIsh);
int16_t  CheckAiShdefStatus(int16_t ishBeg, int16_t ishEnd, uint16_t cRecyclePeriod, int16_t *piLatest, uint8_t *rgbOld);
void     IncreaseAIMinefieldSizes();
int16_t  FFindBuddyAndJoinUp(FLEET *lpfl, int16_t ishLo, int16_t ishHi, int32_t lMaxDist1, int32_t lMaxDist2);
int16_t  FShouldPlanetBuildColonizer(PLANET *lpplSrc);
void     InitRandomPlanetList();

#endif
