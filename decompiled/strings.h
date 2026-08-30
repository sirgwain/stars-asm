#ifndef STARS_DECOMPILED_STRINGS_H
#define STARS_DECOMPILED_STRINGS_H

#include <stdint.h>
#include <windows.h>

extern char    aSTRCmpr[28209];
extern uint8_t acSTR[1414];
extern int16_t aiSTRChunkOffset[23];
extern char    rgSTRLookupTable[84];

char *PszGetCompressedString(StringId ids);

#endif
