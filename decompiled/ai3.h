#ifndef STARS_DECOMPILED_AI3_H
#define STARS_DECOMPILED_AI3_H

#include <stdint.h>
#include <windows.h>

extern uint16_t vrgMacIshAip[31];
extern uint8_t  vrgMacAip[248];
extern uint8_t  vrgAiMacintiResOrder[8];

void    DoMacintiAiTurn(PROD *rgprod);
void    EnsureMacintiShdefs();
int16_t FRetargetMiner(FLEET *lpfl);
int16_t IdTargetMacFreighter(FLEET *lpfl);
void    TargetMacArmada(FLEET *lpfl);
int16_t FPotentMacWarFleet(FLEET *lpfl, int16_t *pcEquiv);

#endif
