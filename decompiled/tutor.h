#ifndef STARS_DECOMPILED_TUTOR_H
#define STARS_DECOMPILED_TUTOR_H

#include <stdint.h>
#include <windows.h>

extern ITEMACTION rgiaUnloadAllCol[5];
extern ITEMACTION rgiaQuikDrop[5];
extern ITEMACTION rgiaQuikLoad[5];
extern ITEMACTION rgiaLoadAllCol[5];
extern ZIPPRODQ1  rgzpqTut[2];

int16_t TutorDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t PanicDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void    ShowTutor(int16_t fShow);
void    DrawTutorText(HWND hwnd);
void    StartTutor(int16_t fRestart);
void    AdvanceTutor();
void    EndTutor(int16_t fClose);
void    SaveGameState();
void    RestoreGameState();
int16_t FAskKillTutor();
int16_t FTutorTaskDone();
int16_t FCheckZip(int16_t iZip, ITEMACTION *lpiaGoal, StringId ids);
int16_t FCheckTemplate(int16_t iTemplate);
void    TutorError(int16_t idsError);
int16_t FCheckScanner(int16_t md, int16_t iZoom);
int16_t FCheckFleetName(int16_t id, StringId ids);
int16_t FCheckSummary(GrobjClass grobj, int16_t id);
int16_t FCheckSelection(GrobjClass grobj, int16_t id);
int16_t FCheckMessages(int16_t imsg, MessageId idm, int16_t fFilter);
int16_t FCheckResearch(int16_t iTech, int16_t iTechNext, int16_t pct);
int16_t FCheckFleetWP(uint16_t ifl, int16_t iord, GrobjClass grobj, int16_t id, uint16_t grTask, uint16_t iWarp);
int16_t FCheckPlanetRoute(int16_t idpl, int16_t idplRoute);
int16_t FCheckLayingWP(uint16_t ifl, int16_t iord, int16_t id, int16_t iYears);
int16_t FCheckColonizeWP(uint16_t ifl, int16_t id, uint16_t iWarp);
int16_t FCheckPatrolWP(uint16_t ifl, int16_t iord, int16_t id, uint16_t iWarp, uint16_t iPlan, uint16_t iDist);
int16_t FCheckXferWP(uint16_t ifl, int16_t iord, int16_t id, uint16_t iWarp, ITEMACTION *lpiaGoal);
int16_t FCheckQueue(int16_t ipl, int16_t iprod, GrobjClass grobj, uint16_t iItem, uint16_t cItem, uint16_t fNoResearch);
int16_t FCheckBtlPlan(int16_t ibp, uint16_t imdTarget, uint16_t fSpread, uint16_t fBomb, uint16_t fDump, uint16_t mdUnarmed, uint16_t mdScout, uint16_t mdWar,
                      uint16_t mdBomber);
int16_t FCheckCargo(FLEET *lpfl, int16_t wtMin1, int16_t wtMin2, int16_t wtMin3, int16_t wtColonists);
int16_t FCheckBuilderPart(int16_t iSlot, HS *phs, uint16_t cInit);
int16_t FCheckShipBuilder(int16_t iCategory, int16_t iShip);
int16_t FTutorialEnabledShipBuilder(int16_t itutsbAction);
int16_t FOKMergeDialog();

#endif
