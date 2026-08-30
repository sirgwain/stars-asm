#ifndef STARS_DECOMPILED_SHIP2_H
#define STARS_DECOMPILED_SHIP2_H

#include <stdint.h>
#include <windows.h>

int16_t ZipOrderDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void    EnableZipBtns(HWND hwnd, int16_t iSel);
int16_t RenameZipDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t RenameDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t FStargateJump(FLEET *lpfl, int16_t isbsSrc, int16_t isbsDst, int16_t dDist);
int16_t MdCalcStargateDamage(int16_t isbsSrc, int16_t isbsDst, int16_t dDist, int16_t wt, int16_t *ppctDmg);
void    KillUsedWaypoints();
void    NoAutoTrackFleet(FLEET *lpflTarget);
void    AutoRouteFleet(FLEET *lpfl, PLANET *lppl);
int16_t FColonizer(FLEET *lpfl);
int16_t FScout(FLEET *lpfl);
void    AutoFleetOrder(FLEET *lpfl, PLANET *lppl);
int32_t CMineFromLpfl(FLEET *lpfl);
int32_t PctTerraFromLpfl(FLEET *lpfl);
int32_t CLayMinesFromLpfl(FLEET *lpfl, int16_t iType, int16_t ishdef);
int32_t CMineSweepFromLpfl(FLEET *lpfl);
int32_t CMineSweepFromLphul(HUL *lphul);
int16_t PctCloakFromLpfl(FLEET *lpfl);
int16_t CPtsCloakFromLphs(HS *lphs);
int16_t MergeFleetsDlg(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam);
void    MarkTechsSeen(HUL *lphul, int16_t iplr);

#endif
