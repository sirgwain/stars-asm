#ifndef STARS_DECOMPILED_SAVE_H
#define STARS_DECOMPILED_SAVE_H

#include <stdint.h>
#include <windows.h>

void    WriteOrders(FLEET *lpfl);
void    WriteRtPlr(PLAYER *pplr, uint8_t *pbStore);
void    WriteRtShDef(SHDEF *lpshdef, uint8_t **ppbStore);
int16_t FWriteDataFile(char *pszFileBase, int16_t iPlayer, int16_t fAppend);
int16_t FAppendFile(int16_t iPlayer);
void    WriteBattles(int16_t iPlayer);
void    WritePlanet(PLANET *lppl, RecordType rt, int16_t fHistory);
void    WriteFleet(FLEET *lpfl);
void    WriteRtString(char *lpsz);
void    MarkFleet(FLEET *lpfl, int16_t det);
void    WriteBattlePlan(BTLPLAN *lpbtlplan, int16_t fLog);
void    MarkPlanet(PLANET *lppl, int16_t iPlr, uint16_t det);
void    SetSzWorkFromDt(DtFileType dt, int16_t iPlayer);
int16_t FCreateFile(DtFileType dt, int16_t iPlayer, char *szForceName);
void    WriteBOF(int16_t iPlayer, int16_t dt, int16_t fMulti);
int16_t FMarkFile(DtFileType dt, int16_t iPlayer, int16_t mdMark, int16_t f);
void    WriteRt(RecordType rt, int16_t cb, void *rg);
void    RgToStream(void *rg, uint16_t cb);
void    SetVisiblePlanFleet(int16_t iPlr);
void    SetVisPFInit(int16_t iPlr);
void    SetVisPFFleets(int16_t iPlr);
void    SetVisPFPlanets(int16_t iPlr);
void    SetVisPFThings(int16_t iPlr);
void    SetVisPFFinish(int16_t iPlr);

#endif
