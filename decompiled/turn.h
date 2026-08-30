#ifndef STARS_DECOMPILED_TURN_H
#define STARS_DECOMPILED_TURN_H

#include <stdint.h>
#include <windows.h>

extern int16_t rgpctMineHit[3];
extern int16_t rgiWarpSafe[3];
extern int16_t rgrgdmgMinMine[3][2];
extern int16_t rgrgdmgMine[3][2];

int16_t FGenerateTurn();
void    DoOrders(int16_t fPostMovement);
void    MoveThings(int16_t fPostProd);
void    FuelFleets();
void    MoveFleets();
int16_t FTravelThroughMineFields(FLEET *lpfl, int16_t *pdTravel, THING *lpthHit);

#endif
