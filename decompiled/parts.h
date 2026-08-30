#ifndef STARS_DECOMPILED_PARTS_H
#define STARS_DECOMPILED_PARTS_H

#include <stdint.h>
#include <windows.h>

extern SHDEF     rgshdefSBT[4];
extern BEAM      rgbeam[24];
extern SCANNER   rgscanner[16];
extern HULDEF    rghuldefSB[5];
extern SHDEF     rgshdefT[22];
extern ENGINE    rgengine[16];
extern HULDEF    rghuldef[32];
extern MINING    rgmining[8];
extern SHIELD    rgshield[10];
extern TORP      rgtorp[12];
extern TERRA     rgterra[20];
extern PLANETARY rgplanetary[15];
extern BOMB      rgbomb[15];
extern ARMOR     rgarmor[12];
extern SPECIALSB rgspecialSB[16];
extern SPECIAL   rgspecialM[11];
extern SPECIAL   rgspecialE[17];
extern MINES     rgmines[10];

HULDEF    *LphuldefSBFromId(int16_t id);
HULDEF    *LphuldefFromId(int16_t id);
ENGINE    *LpengineFromId(int16_t id);
SCANNER   *LpscannerFromId(int16_t id);
SHDEF     *LpshdefT();
SHDEF     *LpshdefSBT();
PLANETARY *LpplanetaryFromId(int16_t id);
int16_t    FLookupPartX(PART *ppart, uint16_t grhst, uint16_t iItem);
int16_t    FLookupPart(PART *ppart);
void       LookupBestPlanetaryScanner(PART *ppart);
int16_t    TechStatus(char *rgTech);

#endif
