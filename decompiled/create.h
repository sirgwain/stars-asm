#ifndef STARS_DECOMPILED_CREATE_H
#define STARS_DECOMPILED_CREATE_H

#include <stdint.h>
#include <windows.h>

extern int16_t vrgvcMax[10];
extern char    rgNG3Width[9][2];
extern uint8_t vrgWormholeMin[5];
extern BTLPLAN rgbtlplanT[5];
extern PLAYER  vrgplrComp[6][4];
extern uint8_t vrgWormholeVar[5];

void    InitBattlePlan(BTLPLAN *lpbtlplan, int16_t iplan, int16_t iplr);
int16_t GenerateWorld(int16_t fBatchMode);
int16_t CreateStartupShip(int16_t iplr, int16_t idPlanet, int16_t ishdef, int16_t fAddShdef);
int16_t GenNewGameFromFile(char *pszFile);
void    CreateTutorWorld();
void    NewGameWizard(HWND hwnd, int16_t fReadOnly);
void    InitNewGamePlr(int16_t iStepMaxSoFar, int16_t lvlAi);
void    InitNewGame3();
int16_t FGetNewGameName(char *szFileSuggest);
int16_t SimpleNewGameDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t NewGameDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t NewGameDlg2(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void    DrawNewGame2(HWND hwnd, HDC hdc, int16_t iDraw);
int16_t NewGameDlg3(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void    DrawNewGame3(HWND hwnd, HDC hdc, int16_t iDraw);
int16_t FTrackNewGameDlg3(HWND hwnd, POINT pt, int16_t kbd);
void    SetNGWTitle(HWND hwnd, int16_t iStep);
PLAYER *LpplrComp(int16_t idAi, int16_t lvlAi);
void    SetVCCheck(GAME *pgame, int16_t vc, int16_t fChecked);
int16_t GetVCCheck(GAME *pgame, int16_t vc);
int16_t SetVCVal(GAME *pgame, int16_t vc, int16_t val);
int16_t GetVCVal(GAME *pgame, int16_t vc, int16_t fRaw);

#endif
