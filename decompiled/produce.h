#ifndef STARS_DECOMPILED_PRODUCE_H
#define STARS_DECOMPILED_PRODUCE_H

#include <stdint.h>
#include <windows.h>

int16_t          ChangeProduction(int16_t fClear);
void             InitProduction(PROD *rgprod);
void             FinishProduction(int16_t fWrite);
INT_PTR CALLBACK ProductionDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void             ProdCommandHandler(HWND hwnd, WPARAM wParam, LPARAM lParam);
void             InitializeProductionDlg(HWND hwnd);
void             DrawProductionDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iDraw);
void             FillProdSrcLB(HWND hwndLB, int16_t mdFill);
char            *PszNameProdItem(PROD *lpprod);
void             GetProductionCosts(PLANET *lppl, PROD *lpprod, uint32_t *rgCost, int16_t iplr, int16_t fOnlyOne);
void             EstimateItemProdSched(PLANET *lppl, PLPROD *lpplprod, ProdItemType iItem, int16_t *piFirst, int16_t *piLast);
INT_PTR CALLBACK ZipProdDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void             EnableZipProdBtns(HWND hwnd, int16_t iSel);
void             FillZipProdLB(HWND hwndDlg, ZIPPRODQ *pzpq);

#endif
