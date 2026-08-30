#ifndef STARS_DECOMPILED_VCR_H
#define STARS_DECOMPILED_VCR_H

#include <stdint.h>
#include <windows.h>

void     BattleVCR(int16_t iBattle);
int16_t  CBattles();
BTLDATA *BtlDataGet(int16_t i);
int32_t  CBattleUnits(BTLDATA *lpbd, uint16_t grbitBU);
int32_t  CBattleKills(BTLDATA *lpbd, int16_t fOurDead);
int32_t  LdpFromItokDv(int16_t itok, DV *lpdv);
int16_t  SetVCRBoard(int16_t iStep);
int16_t  VCRDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void     GetVCRStats(int16_t itok, int32_t *pdpArmor, DV *pdv, int32_t *pdpShields, int16_t *pcsh);
void     DrawVCR(HDC hdc, int16_t iStart, int16_t iEnd);
void     Delay(int16_t ctick);
void     AnimateAttack(HDC hdc);
int16_t  PopupVCRMenu(HWND hwnd, int16_t x, int16_t y, uint8_t brc);
void     EnableVCRButtons();

#endif
