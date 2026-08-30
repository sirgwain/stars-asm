#ifndef STARS_DECOMPILED_AI4_H
#define STARS_DECOMPILED_AI4_H

#include <stdint.h>
#include <windows.h>

extern uint16_t vrgCyberIshAip[36];
extern uint8_t  vrgCyberAip[301];
extern uint8_t  vrgAiCybertronResOrder[42];

void    DoCyberAiTurn(PROD *rgprod);
void    DoCyberPackets();
int16_t IdGetBestScannerDest(PLANET *lppl, int16_t iDir);
int16_t FAddPacketToQueue(PLANET *lppl);
void    FillProductionQueue();
int16_t FFillProdMinesAndFactories(PLANET *lppl);
void    DoCyberFreighter(FLEET *lpfl, CYBERINFOTEMP *lpciPlanTemp);
int16_t FEnumDropOffStage1(PLANET *lpplSrc, PLANET *lpplTest);
int16_t FEnumDropOffStage2(PLANET *lpplSrc, PLANET *lpplTest);
int16_t FEnumPickUp(PLANET *lpplSrc, PLANET *lpplTest);
int16_t FEnumNeedMinerals(PLANET *lpplSrc, PLANET *lpplTest);
int16_t FEnumPktAttack(PLANET *lpplSrc, PLANET *lpplTest);
int16_t FEnumCalcEnemyPlanets(PLANET *lpplSrc, PLANET *lpplTest);
int16_t iBuildCyberStarbase(PLANET *lppl);
void    EnsureCyberAiShdefs(int16_t iroCur);
int16_t iAddAttackFleet(PLANET *lppl, int16_t iAttackStr, int16_t iBestDestroyer, int16_t iBestBattle, int16_t iBestSBDefender);
void    TargetCyberArmada(FLEET *lpfl);

#endif
