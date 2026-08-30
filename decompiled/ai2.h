#ifndef STARS_DECOMPILED_AI2_H
#define STARS_DECOMPILED_AI2_H

#include <stdint.h>
#include <windows.h>

extern uint8_t vrgISIshAip[19];
extern uint8_t vrgISAip[182];
extern uint8_t vrgAiISResOrder[18];

void    DoMaidAiTurn(PROD *rgprod);
int16_t FPotentISWarFleet(FLEET *lpfl, int16_t iPotency);
void    DoAutomitronAiTurn(PROD *rgprod);
void    EnsureISShdefs(int16_t iroCur);
void    DoRototillAiTurn(PROD *rgprod);
void    EnsureCAShdefs(int16_t iroCur);

#endif
