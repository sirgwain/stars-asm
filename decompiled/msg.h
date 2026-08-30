#ifndef STARS_DECOMPILED_MSG_H
#define STARS_DECOMPILED_MSG_H

#include <stdint.h>
#include <windows.h>

extern char    rgcMsgArgs[387];
extern uint8_t acMSG[387];
extern char    aMSGCmpr[22836];
extern char    rgMSGLookupTable[72];
extern int16_t aiMSGChunkOffset[7];

int32_t   MessageWndProc(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam);
void      SetMsgTitle(HWND hwnd);
int16_t   IMsgNext(int16_t fFilteredOnly);
int16_t   IMsgPrev(int16_t fFilteredOnly);
void      DecorateMsgTitleBar(HDC hdc, RECT *prc);
HtMsgType HtMsgBox(POINT pt);
int16_t   FSendPlrMsg2(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2);
int16_t   FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7);
int16_t   FSendPrependedPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7);
int16_t PackageUpMsg(uint8_t *pb, int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7);
int16_t FSendPlrMsg2XGen(int16_t fPrepend, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2);
int16_t IdmGetMessageN(int16_t iMsg);
int16_t FGetNMsgbig(int16_t iMsg, MSGBIG *pmb);
char   *PszGetMessageN(int16_t iMsg);
char   *PszFormatString(char *pszFormat, int16_t *pParamsReal);
int16_t MsgDlg(HWND hwnd, uint16_t message, uint16_t wParam, int32_t lParam);
char   *PszFormatMessage(MessageId idm, int16_t *pParams);
char   *PszFormatIds(StringId ids, int16_t *pParams);
int16_t FRemovePlayerMessage(int16_t iPlr, int16_t iMsg, int16_t iObj);
int16_t FFindPlayerMessage(int16_t iPlr, int16_t iMsg, int16_t iObj);
void    MarkPlanetsPlayerLost(int16_t iPlayer);
void    MarkPlayersThatSentMsgs(int16_t iPlayer);
void    WritePlayerMessages(int16_t iPlayer);
void    ResetMessages();
void    ReadPlayerMessages();
int16_t FFinishPlrMsgEntry(int16_t dInc);
char   *PszGetCompressedMessage(MessageId idm);
void    SetFilteringGroups(MessageId idm, int16_t fSet);

#endif
