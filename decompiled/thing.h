#ifndef STARS_DECOMPILED_THING_H
#define STARS_DECOMPILED_THING_H

#include <stdint.h>
#include <windows.h>

THING  *LpthNew(int16_t iplr, ThingType ith);
void    FreeLpth(THING *lpth);
int16_t CPlanetsInCircle(POINT pt, int32_t r2);
void    DrawThingGauge(HDC hdc, RECT *prc, THING *lpth, int16_t md);
int16_t IValidateWormholePos(THING *lpthWorm);
int16_t PctWormholeMoves(THING *lpth);
void    DoThingInteractions(int16_t fPostMove);
int16_t IdmGiveTraderPart(GrbitTrader grbitTrader, int16_t iplr, uint16_t *piGoto);

#endif
