#ifndef STARS_DECOMPILED_RACE_H
#define STARS_DECOMPILED_RACE_H

#include <stdint.h>
#include <windows.h>

extern int16_t rgRacePrimaryTrait[10];
extern char    rgRW3Spacing[7];
extern int16_t rgRaceAdvDisPts[14];
extern char    rgRW3IStat[7];
extern int16_t rgRaceDisEnvPts[6];
extern char    rgRW3Width[7];
extern char    rgRaceStatMax[16];
extern char    rgRaceStatMin[16];

int16_t          RaceCreationWizard(HWND hwndParent, int16_t fReadOnly, int16_t fDontWrite);
INT_PTR CALLBACK RaceWizardDlg1(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK RaceWizardDlg2(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void             DrawRace2(HWND hwnd, HDC hdc, int16_t iDraw);
int16_t          IrcRaceDlgHitTest(POINT pt);
int16_t          FTrackRaceDlg2(HWND hwnd, POINT pt, int16_t kbd);
INT_PTR CALLBACK RaceWizardDlg3(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void             DrawRace3(HWND hwnd, HDC hdc, int16_t iDraw);
int16_t          FTrackRaceDlg3(HWND hwnd, POINT pt, int16_t kbd);
int16_t          GetRaceStat(PLAYER *pplr, RaceStat iStat);
int16_t          SetRaceStat(PLAYER *pplr, RaceStat iStat, int16_t iVal);
int16_t          GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit);
void             SetRaceGrbit(PLAYER *pplr, RaceGrbit ibit, int16_t fSet);
INT_PTR CALLBACK RaceWizardDlg4(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK RaceWizardDlg5(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK RaceWizardDlg6(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void             BoundsCheckPlayer(PLAYER *pplr);
int16_t          CAdvantagePoints(PLAYER *pplr);
int32_t          LInnateRaceHabitability(PLAYER *pplr);
void             InvalidateAdvPtsRect(HWND hwnd);
void             DrawRaceAdvantagePoints(HDC hdc, RECT *prc, PLAYER *pplr);
uint16_t         IRaceChecksum(PLAYER *pplr);
int16_t          FSaveRace(char *szFileSuggest, PLAYER *pplr);
void             SetRCWTitle(HWND hwnd, int16_t iStep);
void             CreateRandomRace(PLAYER *pplr);
int16_t          PctTrueMaxGrowth(int16_t iplr);

#endif
