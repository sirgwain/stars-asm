#ifndef STARS_DECOMPILED_FILE_H
#define STARS_DECOMPILED_FILE_H

#include <stdint.h>
#include <windows.h>

extern char     mpishdefishTutor[6];
extern uint32_t bogi[25];

int16_t FReadShDef(RTSHDEF *lprt, SHDEF *lpshdef, int16_t iplrLoad);
void    ReadRtPlr(PLAYER *pplr, uint8_t *pbIn);
int16_t FLoadGame(char *pszFileName, char *pszExt);
int16_t FReadPlanet(int16_t iPlayer, PLANET *lppl, int16_t fHistory, int16_t fPreInited);
int16_t FReadFleet(FLEET *lpfl);
void    UnpackBattlePlan(uint8_t *lpb, BTLPLAN *lpbtlplan, int16_t iplan);
void    UpdateBattleRecords();
int16_t AskSaveDialog(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void    PromptSaveGame();
void    DestroyCurGame();
int16_t FBogusLong(uint32_t lSerial);
int16_t FValidSerialLong(uint32_t lSerial);
void    FileError(MessageId ids);
void    GetFileStatus(int16_t dt, int16_t iPlayer);
int16_t FOpenFile(DtFileType dt, int16_t iPlayer, int16_t md);
int16_t FNewTurnAvail(int16_t idPlayer);
int16_t FCheckFile(DtFileType dt, int16_t iPlayer, uint16_t md);
void    ReadRt();
int16_t FBadFileError(StringId ids);
void    StreamOpen(char *szFile, int16_t mdOpen);
void    StreamClose();
void    RgFromStream(void *rg, uint16_t cb);

#endif
