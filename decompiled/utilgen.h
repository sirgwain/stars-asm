#ifndef STARS_DECOMPILED_UTILGEN_H
#define STARS_DECOMPILED_UTILGEN_H

#include <stdint.h>
#include <windows.h>

extern int16_t aiPNChunkOffset[16];
extern uint8_t acPN[999];
extern int16_t rgPrimes[128];
extern char    aPNCmpr[4099];
extern char    rgPNLookupTable[52];
extern int32_t lFileSeed1;
extern int32_t lFileSeed2;

void    PushRandom(int32_t lNew1, int32_t lNew2);
void    PopRandom();
void    Randomize(uint32_t dw);
void    Randomize2(uint32_t dw);
int16_t Random(int16_t c);
int16_t RandomSeedDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void    GetFileSeeds(int32_t *pl1, int32_t *pl2);
void    SetFileSeeds(int32_t l1, int32_t l2);
void    SetFileXorStream(int32_t lid, int16_t lSalt, int16_t turn, int16_t iPlayer, int16_t fCrippled);
int32_t LGetNextFileXor();
void    XorFileBuf(char *rgb, int16_t cb);
int16_t ICompLong(void *arg1, void *arg2);
char   *PszGetCompressedPlanet(int16_t id);
void    OutputFileString(char *szFile, char *sz);
void    CopyFile(char *szSrc, char *szDst);
int16_t AlertSz(char *sz, MessageBoxType mbType);
int16_t CchGetString(StringId ids, char *psz);
char   *PszFromInt(int16_t i, int16_t *pcch);
char   *PszFromLong(int32_t l, int16_t *pcch);
char   *PszFromLongK(int32_t l, int16_t *pcch);
int16_t CommaFormatLong(char *psz, int32_t l);
void    CtrTextOut(HDC hdc, int16_t x, int16_t y, char *psz, int16_t cLen);
int16_t DxStreamTextOut(HDC hdc, int16_t *px, int16_t y, char *psz, int16_t cLen, int16_t fPrint);
void WrapTextOut(HDC hdc, int16_t *px, int16_t *py, char *psz, int16_t cLen, int16_t xLeft, int16_t dxWidth, int16_t *pxMax, int16_t fNewLine, int16_t fPrint);
void AddBackTrailingSpaces(char **ppch, char *pchEnd);
void ChopLastWord(char *pBeg, char **ppEnd);
void ChopTrailingSpaces(char *pBeg, char **ppEnd);
void RcCtrTextOut(HDC hdc, RECT *prc, char *psz, int16_t cLen);
void RightTextOut(HDC hdc, int16_t x, int16_t y, char *psz, int16_t cLen, int16_t dxErase);
void DiaganolTextOut(HDC hdc, RECT *prc, char *psz, int16_t cLen);
void ExpandRc(RECT *prc, int16_t dx, int16_t dy);
void OffsetRc(RECT *prc, int16_t dx, int16_t dy);
void BoundPoints(RECT *prc, POINT *rgpt, int16_t cpt);
void StickyDlgPos(HWND hwnd, POINT *ppt, int16_t fInit);
int32_t  LDrawGauge(HDC hdc, RECT *prc, int16_t cSegs, int32_t *rgSize, uint16_t *rghbr, int32_t cTot);
void     _Draw3dFrame(HDC hdc, RECT *prc, int16_t fErase);
void     InitBtnTrack(BTNT *pbtnt, HWND hwnd, HDC hdc, RECT *prc, int16_t btf, int16_t dTimer, int16_t fInitDown, int16_t fNoEndRedraw, char *szText);
int16_t  FTrackBtn(BTNT *pbtnt);
void     DrawBtn(HDC hdc, RECT *prc, int16_t bt, int16_t fDown, char *szText);
int16_t  FGetMouseMove(POINT *ppt);
int16_t  FGetRMouseMove(POINT *ppt);
void     DrawFuzzyBorder(HDC hdc, RECT *prc);
int16_t  FStringFitsScreen(char *lpsz, int16_t dxMax);
HBRUSH   HbrGet(COLORREF cr);
void     FreeHbr(HBRUSH hbr);
int16_t  FCompressUserString(char *szIn, char *szOut, int16_t *pcOut);
int16_t  FDecompressUserString(char *szIn, int16_t cIn, char *szOut, int16_t *pcOut);
int16_t  NybbleFromCh(uint8_t ch);
char     ChFromNybble(int16_t nyb);
uint16_t DibNumColors(void *pv);
HPALETTE HpalFromDib(HGLOBAL hdib);
HPALETTE HpalBlackReserved();
uint16_t PaletteSize(void *pv);
int16_t  DibBlt(HDC hdc, int16_t x0, int16_t y0, int16_t dx, int16_t dy, HGLOBAL hdib, int16_t x1, int16_t y1, int16_t dxSrc, int16_t dySrc, int32_t rop);
HGLOBAL  DibFromBitmap(uint16_t hbm, uint32_t biStyle, uint16_t biBits, HPALETTE hpal);
HGLOBAL  HdibLoadBigResource(int16_t idb);
int16_t  ReadBigBlock(int16_t hFile, char *lpBuffer, uint32_t dwSize);
int16_t  FIntersectCircleLine(POINT ptL1, POINT ptL2, POINT ptC, int32_t r2, int16_t dMax, int16_t *pdStart, int16_t *pdEnd);
void     IntToRoman(int16_t i, char *pszOut);
int16_t  FCheckPassword();
int32_t  LSaltFromSz(char *psz);
int16_t  PasswordDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
int16_t  NewPasswordDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
uint32_t GetDiskSerialNumber();
void     ShowProgressGauge();
void     HideProgressGauge();
void     UpdateProgressGauge(int16_t pctX10);
int16_t  ProgressGaugeDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void     DrawProgressGauge(HDC hdcOrig, int16_t fFull, int16_t iNumOnly);
int32_t  LDistance2(POINT pt1, POINT pt2);
char    *PszGetLine(char **ppszBeg);
int16_t  CParseNumbers(char *psz, int32_t *pl, int16_t cMax);
HFONT    HfontPrinterCreate(HDC hdc, int16_t iSize, int16_t *pdyFont);

#endif
