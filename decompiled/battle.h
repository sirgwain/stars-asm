#ifndef STARS_DECOMPILED_BATTLE_H
#define STARS_DECOMPILED_BATTLE_H

#include <stdint.h>
#include <windows.h>

extern uint8_t rgbrcStart[136];

int16_t RelationsDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t NewPlanNameDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t BattlePlansDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t FDeleteBattlePlan(int16_t iplan, int16_t fWarn);
void    SpankTheCheaters();
int16_t FFleetHasBombs(FLEET *lpfl);
int16_t FHullHasBombs(HUL *lphul);
int16_t FFleetHasTeeth(FLEET *lpfl);
int16_t FHullHasTeeth(HUL *lphul);
int16_t FFuelTanker(SHDEF *lpshdef);
void    CheckTarget(TOK *ptok, FLEET *lpfl, int16_t ishdef);
int16_t FDumpCargo(FLEET *lpfl);
void    DropSalvage(THING **plpth, int32_t *rgwtMinerals, int16_t iplr, POINT *ppt);
int16_t CplrBattle(FLEET *lpfl, uint16_t *rggrfAttack, uint16_t *pgrfPlayer, uint16_t *pgrfSpectator);
int16_t SpdOfShip(FLEET *lpfl, int16_t ishdef, TOK *ptok, int16_t fDumpCargo, SHDEF *lpshdef);
SHDEF  *LpshdefFromTok(TOK *ptok);
int16_t FCanKillTok(TOK *ptok1, TOK *ptok2);
void    DoBattles(int16_t fPostMovement);
void    RegenShield(TOK *ptok);
int16_t InitFromHuldef(HUL *lphul, int16_t *ppctBC);
void    CheckInitiative(TOK *ptok);
void    CheckWeapons(TOK *ptok, int16_t *pfDampeningField, uint8_t *pinit);
void    RandomizeTokOrder();
void    InitializeBoard(FLEET *lpfl, int16_t ibrc, uint16_t grfPlayer, uint8_t *pinit, int16_t *pinitMin, int16_t *pinitMac);
int16_t DzFromBrcBrc(uint8_t brc1, uint8_t brc2);
int32_t DpFromPtokBrcToBrc(TOK *ptok, uint8_t brcSrc, uint8_t brcTarget, TOK *ptokTarget, int16_t fProximity);
int16_t DzMoveRangeToConsider(TOK *ptok, uint16_t grfAttack, uint8_t *pbrc);
int16_t FDoesPrimaryTargetTypeExist(TOK *ptok, uint16_t grfAttack);
int16_t FIsTargetOfMdTarget(TOK *ptok, int16_t mdTarget);
int32_t ScoreGuessBattleDamage(TOK *ptokSrc, uint8_t brc, int16_t fPrimary, uint16_t grfAttack);
int32_t ScoreFromGiveAndTakeAndTactic(int32_t dpGive, int32_t dpTake, int16_t mdTactic);
int16_t DxyMoveTokTo(TOK *ptok, int16_t spdMove, uint16_t grfAttack);
int32_t CTorpHit(int32_t cTorpBase, TOK *ptok, int16_t pctBase, int16_t pctBC);
int16_t FAttack(int16_t itokAttacker, int16_t init, BTLREC *lpbtlrec, uint16_t grfAttack);
void    KillShips(TOK *ptok, int16_t cshKill, int16_t ishdef, FLEET *lpfl, int16_t fFallout);
void    CreateSalvage(FLEET *pfl, THING **plpth);
int16_t FDamageTok(TOK *ptok, int16_t itok, int32_t *pdpBeam, int32_t dpTorp, uint16_t grfWeapon, int16_t fShieldsOnly, int32_t *pcTorp);
int16_t DxyFromSpdRound(uint16_t spd, int16_t iRound);
int16_t FDoCoolBattle(FLEET *lpfl, int16_t cplr, uint16_t *rggrfAttack, uint16_t grfPlayer, uint16_t grfSpectator);
int16_t ITechLearnATech(int16_t iplr, int16_t x, int16_t y, MessageId idm, uint16_t *piGoto);
void SendBattleMessages(FLEET *lpflBtl, int16_t cplr, int16_t idBtl, uint16_t *rgPlrLosses, int16_t grfPlayer, int16_t cShipsInvolved, int16_t cShdefsInvolved,
                        uint16_t grfSpectator);
int16_t FAttackPlayer(FLEET *lpfl, int16_t iplr);
void    DoBombing();

#endif
