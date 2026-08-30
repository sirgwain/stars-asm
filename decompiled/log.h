#ifndef STARS_DECOMPILED_LOG_H
#define STARS_DECOMPILED_LOG_H

#include <stdint.h>
#include <windows.h>

void    LogSplitFleet(int16_t id);
void    LogMergeFleet(int16_t id);
void    LogChangeShDef(SHDEF *lpshdefNew);
void    LogChangeName(GrobjClass grobj, int16_t id, char *szName);
void    LogChangeFleet(FLEET *pfl, FLEET *pflNew);
void    LogChangeRelations();
void    LogChangeBtlplan(BTLPLAN *pbtlplan);
void    LogChangePlanet(PLANET *ppl, PLANET *pplNew);
void    LogChangeThing(THING *lpth, THING *pthNew);
void    LogMakeValidXfer(LOGXFER *plx1, LOGXFER *plx2);
void    LogMakeValidXferf(LOGXFERF *plxf1, LOGXFERF *plxf2);
void    CancelMemRt(RecordType rt);
void    WriteMemRt(int16_t rt, int16_t cb, void *rg);
void    DirtyGame(int16_t fDirty);
int16_t FGetPrevLogRt(HDR *phdr, uint8_t *pb);
int16_t FRunLogFile();
int16_t FRunLogRecord(RecordType rt, int16_t cb, uint8_t *lpb);
int16_t FLoadLogFile(char *pszLog);
int16_t FCheckLogFile(int16_t iplr, int16_t *pfError);
int16_t FWriteLogFile(char *pszFileBase, int16_t iPlayer);
int16_t FWriteTutorialMFile(int16_t iTurn);
int16_t FWriteHistFile(int16_t iPlayer);
void    EnumLogRts(int16_t (**pfn)(void *, int16_t, int16_t, void *, int16_t), void *lpPass, int16_t iPass);

#endif
