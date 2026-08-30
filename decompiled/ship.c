#include "common.h"

void DrawShipOrders(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t  swp;
    int16_t  dxRight;
    int16_t  iWarp;
    int16_t  yTop;
    POINT    pt;
    RECT     rcT;
    int16_t  dWrong;
    int32_t  lTot;
    int16_t  c;
    FLEET   *pfl;
    int16_t  xRight;
    int16_t  iScanActual;
    RECT     rcGauge;
    char    *psz;
    int16_t  xLeft;
    ORDER    ord;
    int32_t  l;
    RECT     rc;
    uint16_t t_merge_00dc_0001;
    uint16_t t_merge_0129_0001;
    uint16_t t_merge_01da_0001;
    uint16_t t_merge_02ed_0001;

L_0000:
    pfl = obj;
    if ((ptile->fFixCtls == 0x0))
        goto L_0069;
    else
        goto L_0025;

L_0025:
    HIWORD(rgrcRef) = 0xfffb;
    /* untranslated: part[6:2](rgrcRef) = 0xfffa */
    /* untranslated: part[98:2](rgrcRef) = 0xfffb */
    /* untranslated: part[102:2](rgrcRef) = 0xfffa */
    ShowWindow(hwndShipLB, SW_HIDE);
    ShowWindow(hwndRepCB, SW_HIDE);
    ptile->fFixCtls = 0x0;

L_0069:
    if ((FDrawTileNC(hdc, ptile, &(rc), PszGetCompressedString(idsFleetWaypoints)) != 0x0))
        goto L_00ad;
    else
        goto L_0090;

L_0090:
    ShowWindow(hwndShipLB, SW_HIDE);
    ShowWindow(hwndRepCB, SW_HIDE);
    goto L_090c;

L_00ad:
    xLeft = (rc.left + 0x4);
    xRight = (rc.right + 0xfffc);
    if ((gd.fSmallTileMode == 0x0))
        goto L_00d9;
    else
        goto L_00d3;

L_00d3:
    t_merge_00dc_0001 = 0x2;
    goto L_00dc;

L_00d9:
    t_merge_00dc_0001 = 0x4;

L_00dc:
    yTop = (t_merge_00dc_0001 + rc.top);
    /* untranslated: part[98:2](rgrcRef) = 0xfffb */
    /* untranslated: part[102:2](rgrcRef) = 0xfffa */
    GetClientRect(hwndShipLB, &(rcT));
    swp = 0x14;
    if ((gd.fSmallTileMode == 0x0))
        goto L_0126;
    else
        goto L_0120;

L_0120:
    t_merge_0129_0001 = 0x3;
    goto L_0129;

L_0126:
    t_merge_0129_0001 = 0x4;

L_0129:
    /* untranslated: dyShipLB = loword((ss:[bp-0x50] * t_merge_0129_0001)) */
    dWrong = (dyShipLB - (rcT.bottom - rcT.top));
    if ((dxShipLB != (xRight - xLeft)))
        goto L_016c;
    else
        goto L_0151;

L_0151:
    if ((dWrong < 0x0))
        goto L_016c;
    else
        goto L_015a;

L_015a:
    if ((dWrong >= dyArial8))
        goto L_016c;
    else
        goto L_0165;

L_0165:
    swp = (swp | 0x1);
    goto L_0175;

L_016c:
    dxShipLB = (xRight - xLeft);

L_0175:
    SetWindowPos(hwndShipLB, 0x0, xLeft, yTop, (xRight - xLeft), dyShipLB, swp);
    ShowWindow(hwndShipLB, SW_SHOW);
    GetClientRect(hwndShipLB, &(rcT));
    dyShipLB = (rcT.bottom - rcT.top);
    if ((gd.fSmallTileMode == 0x0))
        goto L_01d7;
    else
        goto L_01d1;

L_01d1:
    t_merge_01da_0001 = 0x2;
    goto L_01da;

L_01d7:
    t_merge_01da_0001 = 0x4;

L_01da:
    yTop = (yTop + (t_merge_01da_0001 + dyShipLB));
    SelectObject(hdc, HIWORD(rghfontArial8));
    if ((ptile->fMinDraw != 0x0))
        goto L_0232;
    else
        goto L_0203;

L_0203:
    rcT.top = yTop;
    rcT.bottom = (yTop + dyArial8);
    rcT.left = xLeft;
    rcT.right = xRight;
    FillRect(hdc, &(rcT), hbrButtonFace);

L_0232:
    c = CchGetString(idsComing, 0x57a4);
    l = GetTextExtent(hdc, szWork, c);
    c = CchGetString(idsWayPt, 0x57a4);
    lTot = GetTextExtent(hdc, szWork, c);
    if ((HIWORD(lTot) < HIWORD(l)))
        goto L_02af;
    else
        goto L_0296;

L_0296:
    if ((HIWORD(lTot) > HIWORD(l)))
        goto L_02a3;
    else
        goto L_029b;

L_029b:
    if ((LOWORD(lTot) <= LOWORD(l)))
        goto L_02af;
    else
        goto L_02a3;

L_02a3:
    l = lTot;

L_02af:
    dxRight = ((xRight - xLeft) - LOWORD(l));
    if ((ptile->fMinDraw != 0x0))
        goto L_0311;
    else
        goto L_02d6;

L_02d6:
    if ((sel.iwpAct <= 0x0))
        goto L_02ea;
    else
        goto L_02e4;

L_02e4:
    t_merge_02ed_0001 = 0x382;
    goto L_02ed;

L_02ea:
    t_merge_02ed_0001 = 0x383;

L_02ed:
    c = CchGetString(t_merge_02ed_0001, 0x57a4);
    TextOut(hdc, xLeft, yTop, szWork, c);

L_0311:
    SelectObject(hdc, LOWORD(rghfontArial8));
    if ((sel.iwpAct <= 0x0))
        goto L_037d;
    else
        goto L_0327;

L_0327:
    ord = sel.fl.lpplord->rgord[(sel.iwpAct - 0x1)];
    psz = PszGetLocName(ord.grobj, ord.id, ord.pt.x, ord.pt.y);
    iScanActual = sel.iwpAct;
    goto L_03f4;

L_037d:
    if ((sel.fl.cord > 0x1))
        goto L_038f;
    else
        goto L_0387;

L_0387:
    psz = 0x97e;
    goto L_03f4;

L_038f:
    ord = sel.fl.lpplord->rgord[0x1];
    psz = PszGetLocName(ord.grobj, ord.id, ord.pt.x, ord.pt.y);
    ord = sel.fl.lpplord->rgord[0x0];
    iScanActual = 0x1;

L_03f4:
    RightTextOut(hdc, xRight, yTop, psz, 0x0, dxRight);
    yTop = (yTop + dyArial8);
    if ((sel.fl.cord <= 0x1))
        goto L_0429;
    else
        goto L_041f;

L_041f:
    if ((sel.iwpAct != 0x0))
        goto DoDistance;
    else
        goto L_0429;

L_0429:
    HIWORD(rgrcRef) = 0xfffb;
    /* untranslated: part[6:2](rgrcRef) = 0xfffa */
    if ((sel.fl.cord > 0x1))
        goto DoDistance;
    else
        goto L_043c;

L_043c:

L_0442:
    SetRect(&(rc), (xLeft + 0xffff), yTop, (xRight + 0xffff), ((dyArial8 * 0x4) + yTop));
    FillRect(hdc, &(rc), hbrButtonFace);
    yTop = (yTop + (((dyArial8 - gd.fSmallTileMode) * 0x4) + (gd.fSmallTileMode * 0x2)));
    goto DoCheckBox;

DoDistance:
    if ((ptile->fMinDraw != 0x0))
        goto L_04f7;
    else
        goto L_04c0;

L_04c0:
    SelectObject(hdc, HIWORD(rghfontArial8));
    c = CchGetString(idsDistance, 0x57a4);
    TextOut(hdc, xLeft, yTop, szWork, c);

L_04f7:
    SelectObject(hdc, LOWORD(rghfontArial8));
    pt.x = sel.fl.lpplord->rgord[iScanActual].pt.x;
    pt.y = sel.fl.lpplord->rgord[iScanActual].pt.y;
    RightTextOut(hdc, xRight, yTop, PszGetDistance(ord.pt.x, ord.pt.y, pt.x, pt.y), 0x0, dxRight);
    yTop = (yTop + (dyArial8 - gd.fSmallTileMode));
    SelectObject(hdc, HIWORD(rghfontArial8));
    if ((ptile->fMinDraw != 0x0))
        goto L_05b6;
    else
        goto L_058b;

L_058b:
    c = CchGetString(idsWarpFactor, 0x57a4);
    TextOut(hdc, xLeft, yTop, szWork, c);

L_05b6:
    /* untranslated: iWarp = ((part[6:2](sel.fl.lpplord->rgord[iScanActual]) >> 0x4) & 0xf) */
    if ((sel.iwpAct == 0x0))
        goto L_0640;
    else
        goto L_05e9;

L_05e9:
    SetRect(&(rcGauge), (xLeft + LOWORD(l)), yTop, (xRight + 0xffff), (yTop + dyArial8));
    /* untranslated: part[0:8](rgrcRef) = rcGauge */
    DrawFleetGauge(hdc, &(rcGauge), 0x0, 0x6);
    goto L_06ab;

L_0640:
    SelectObject(hdc, LOWORD(rghfontArial8));
    if ((iWarp >= 0xb))
        goto L_067d;
    else
        goto L_0655;

L_0655:
    c = _wsprintf(szWork, PszGetCompressedString(idsWarpD2), iWarp);
    goto L_0690;

L_067d:
    c = CchGetString(idsUseStargate, 0x57a4);

L_0690:
    RightTextOut(hdc, xRight, yTop, 0x57a4, c, dxRight);

L_06ab:
    yTop = (yTop + dyArial8);
    if ((ptile->fMinDraw != 0x0))
        goto L_06fe;
    else
        goto L_06c7;

L_06c7:
    SelectObject(hdc, HIWORD(rghfontArial8));
    c = CchGetString(idsTravelTime, 0x57a4);
    TextOut(hdc, xLeft, yTop, szWork, c);

L_06fe:
    SelectObject(hdc, LOWORD(rghfontArial8));
    c = CchGetETA(hdc, pfl, 0x57a4, iScanActual, 0x0);
    RightTextOut(hdc, xRight, yTop, 0x57a4, c, dxRight);
    SetTextColor(hdc, 0x0);
    yTop = (yTop + (dyArial8 - gd.fSmallTileMode));
    if ((ptile->fMinDraw != 0x0))
        goto L_07b7;
    else
        goto L_0780;

L_0780:
    SelectObject(hdc, HIWORD(rghfontArial8));
    c = CchGetString(idsEstFuelUsage, 0x57a4);
    TextOut(hdc, xLeft, yTop, szWork, c);

L_07b7:
    SelectObject(hdc, LOWORD(rghfontArial8));
    lTot = LFuelUseToWaypoint(&(sel.fl), iScanActual, 0x0);
    c = _wsprintf(szWork, PszGetCompressedString(idsLdmg), LOWORD(lTot), HIWORD(lTot));
    /* untranslated: branch HIWORD(lTot) < part[122:2](sel) ? L_0833 : L_0816 */

L_0816:
    /* untranslated: branch HIWORD(lTot) > part[122:2](sel) ? L_0823 : L_081b */

L_081b:
    /* untranslated: branch LOWORD(lTot) <= part[120:2](sel) ? L_0833 : L_0823 */

L_0823:
    SetTextColor(hdc, 0xff);

L_0833:
    RightTextOut(hdc, xRight, yTop, 0x57a4, c, (dxRight + 0xffec));
    /* untranslated: branch HIWORD(lTot) < part[122:2](sel) ? L_087e : L_0861 */

L_0861:
    /* untranslated: branch HIWORD(lTot) > part[122:2](sel) ? L_086e : L_0866 */

L_0866:
    /* untranslated: branch LOWORD(lTot) <= part[120:2](sel) ? L_087e : L_086e */

L_086e:
    SetTextColor(hdc, 0x0);

L_087e:
    yTop = (yTop + dyArial8);

DoCheckBox:
    SendMessage(hwndRepCB, CB_LIMITTEXT, sel.fl.fRepOrders, 0x0);
    SetWindowPos(hwndRepCB, 0x0, xLeft, yTop, 0x0, 0x0, 0x15);
    ShowWindow(hwndRepCB, SW_SHOW);
    SetRect(0x4910, (xRight - (dyArial8 | 0x1)), yTop, xRight, ((dyArial8 | 0x1) + yTop));
    DrawDiamond(hdc, 0x4910, hbrBBlue);

L_090c:
    return;
}

void DrawShipWayPtOrders(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t  dxKt;
    int16_t  dxT;
    int16_t  swp;
    int16_t  dxRight;
    int16_t  yTop;
    int16_t  yTopMsg;
    StringId ids;
    int16_t  edWid;
    PLANET  *lppl;
    ORDER   *lpord;
    FLEET   *pfl;
    int16_t  i;
    int16_t  fActive;
    int16_t  xRight;
    uint16_t grtask;
    char     szT[8];
    int16_t  yBot;
    int16_t  dxRight2;
    char    *psz;
    int16_t  cch;
    int16_t  xLeft;
    int32_t  l;
    RECT     rc;
    RECT     rcT;
    char    *pszT;
    int16_t  j;
    int32_t  cMine;
    int16_t  dyCur;
    int16_t  c;
    int32_t  rgl[4];
    uint16_t t_merge_0a22_0001;
    uint32_t t_merge_14cb_0001;
    uint32_t t_merge_1748_0001;

L_0912:
    if ((ptile->fFixCtls == 0x0))
        goto L_098f;
    else
        goto L_0931;

L_0931:
    i = 0x0;
    goto L_094f;

L_0939:
    ShowWindow(rghwndOrderDD[i], SW_HIDE);
    i = (i + 0x1);

L_094f:
    if ((i < 0x3))
        goto L_0939;
    else
        goto L_0958;

L_0958:
    ShowWindow(hwndOrderED, SW_HIDE);
    ptile->fFixCtls = 0x0;
    /* untranslated: part[42:2](rgrcRef) = 0xfffb */
    /* untranslated: part[46:2](rgrcRef) = 0xfffa */
    /* untranslated: part[146:2](rgrcRef) = 0xfffb */
    /* untranslated: part[150:2](rgrcRef) = 0xfffa */

L_098f:
    if ((FDrawTileNC(hdc, ptile, &(rc), PszGetCompressedString(idsWaypointTask)) != 0x0))
        goto L_09ed;
    else
        goto L_09b6;

L_09b6:
    i = 0x0;
    goto L_09d4;

L_09be:
    ShowWindow(rghwndOrderDD[i], SW_HIDE);
    i = (i + 0x1);

L_09d4:
    if ((i < 0x3))
        goto L_09be;
    else
        goto L_09dd;

L_09dd:
    ShowWindow(hwndOrderED, SW_HIDE);
    goto L_17af;

L_09ed:
    pfl = obj;
    xLeft = (rc.left + 0x4);
    xRight = (rc.right + 0xfffc);
    if ((gd.fSmallTileMode == 0x0))
        goto L_0a1f;
    else
        goto L_0a19;

L_0a19:
    t_merge_0a22_0001 = 0x2;
    goto L_0a22;

L_0a1f:
    t_merge_0a22_0001 = 0x4;

L_0a22:
    yTop = (t_merge_0a22_0001 + rc.top);
    yBot = (rc.bottom + 0xfffc);
    dxRight = (xRight - xLeft);
    /* untranslated: part[42:2](rgrcRef) = 0xfffb */
    /* untranslated: part[46:2](rgrcRef) = 0xfffa */
    swp = 0x14;
    if ((LOWORD(rgdxOrderDD) != dxRight))
        goto L_0a5e;
    else
        goto L_0a57;

L_0a57:
    swp = (swp | 0x1);
    goto L_0a64;

L_0a5e:
    LOWORD(rgdxOrderDD) = dxRight;

L_0a64:
    SetWindowPos(LOWORD(rghwndOrderDD), 0x0, xLeft, yTop, dxRight, (LOWORD((0xa * dyArial8)) + dyShipDD), swp);
    ShowWindow(LOWORD(rghwndOrderDD), SW_SHOW);
    yTop = (yTop + (dyShipDD + 0x3));
    yTopMsg = yTop;
    l = SendMessage(LOWORD(rghwndOrderDD), CB_GETCURSEL, 0x0, 0x0);
    grtask = LOWORD(l);
    if ((IsWindowVisible(HIWORD(rghwndOrderDD)) != 0x0))
        goto L_0b19;
    else
        goto L_0adf;

L_0adf:
    SetRect(&(rc), (xLeft + 0xffff), yTop, (xRight + 0x1), (yBot + 0x2));
    FillRect(hdc, &(rc), hbrButtonFace);
    goto L_0b69;

L_0b19:
    /* untranslated: branch IsWindowVisible(part[4:2](rghwndOrderDD)) != 0x0 ? L_0b69 : L_0b2a */

L_0b2a:
    SetRect(&(rc), (xLeft + 0xffff), ((yTop + dyShipDD) + 0x3), (xRight + 0x1), (yBot + 0x2));
    FillRect(hdc, &(rc), hbrButtonFace);

L_0b69:
    if ((grtask == 0x1))
        goto L_0b8d;
    else
        goto L_0b72;

L_0b72:
    if ((grtask == 0x6))
        goto L_0b8d;
    else
        goto L_0b7b;

L_0b7b:
    if ((grtask == 0x7))
        goto L_0b8d;
    else
        goto L_0b84;

L_0b84:
    if ((grtask != 0x9))
        goto L_0ea7;
    else
        goto L_0b8d;

L_0b8d:
    swp = 0x14;
    if ((grtask != 0x1))
        goto L_0bab;
    else
        goto L_0b9b;

L_0b9b:
    dxRight2 = ((dxRight - dyShipDD) + 0x2);
    goto L_0d42;

L_0bab:
    if ((grtask != 0x7))
        goto L_0c7a;
    else
        goto L_0bb4;

L_0bb4:
    SelectObject(hdc, HIWORD(rghfontArial8));
    psz = PszGetCompressedString(idsWarpFactor);
    cch = strlen(psz);
    dxT = (LOWORD(GetTextExtent(hdc, psz, cch)) + 0x2);
    psz = PszGetCompressedString(idsIntercept);
    cch = strlen(psz);
    SetRect(&(rc), xLeft, (yTop + 0x4), (xLeft + dxT), yBot);
    FillRect(hdc, &(rc), hbrButtonFace);
    DrawText(hdc, psz, cch, &(rc), 0x800);
    dxRight2 = ((dxRight - dxT) + 0xfffe);
    xLeft = (xLeft + (dxT + 0x2));
    goto L_0d42;

L_0c7a:
    if ((grtask != 0x9))
        goto L_0d3c;
    else
        goto L_0c83;

L_0c83:
    SelectObject(hdc, HIWORD(rghfontArial8));
    cch = CchGetString(idsTo3, szT);
    szT[cch] = 0x20;
    cch = (cch + 0x1);
    szT[cch] = 0x0;
    dxT = (LOWORD(GetTextExtent(hdc, psz, cch)) + 0x2);
    SetRect(&(rc), xLeft, (yTop + 0x4), (xLeft + dxT), yBot);
    FillRect(hdc, &(rc), hbrButtonFace);
    DrawText(hdc, &(szT), cch, &(rc), 0x800);
    dxRight2 = ((dxRight - dxT) + 0xfffe);
    xLeft = (xLeft + (dxT + 0x2));
    goto L_0d42;

L_0d3c:
    dxRight2 = dxRight;

L_0d42:
    if ((HIWORD(rgdxOrderDD) != dxRight2))
        goto L_0d55;
    else
        goto L_0d4e;

L_0d4e:
    swp = (swp | 0x1);
    goto L_0d5b;

L_0d55:
    HIWORD(rgdxOrderDD) = dxRight2;

L_0d5b:
    SetWindowPos(HIWORD(rghwndOrderDD), 0x0, xLeft, yTop, dxRight2, LOWORD((0x6 * dyShipDD)), swp);
    ShowWindow(HIWORD(rghwndOrderDD), SW_SHOW);
    if ((grtask != 0x1))
        goto L_0df1;
    else
        goto L_0d93;

L_0d93:
    SetRect(&(rcT), (((xLeft + dxRight) - (dyShipDD | 0x1)) + 0x8), (yTop + 0x3), (xLeft + dxRight), (((dyShipDD | 0x1) + yTop) + 0xfffb));
    /* untranslated: part[40:8](rgrcRef) = rcT */
    DrawDiamond(hdc, &(rcT), hbrBBlue);
    goto L_0efc;

L_0df1:
    if ((grtask != 0x7))
        goto L_0efc;
    else
        goto L_0dfa;

L_0dfa:
    yTop = (yTop + (dyShipDD + 0x4));
    SetRect(&(rcT), ((xLeft - dxT) + 0xfffe), yTop, (xRight + 0xffff), (yTop + dyArial8));
    SelectObject(hdc, HIWORD(rghfontArial8));
    psz = PszGetCompressedString(idsWarpFactor);
    cch = strlen(psz);
    DrawText(hdc, psz, cch, &(rcT), 0x800);
    rcT.left = (rcT.left + (dxT + 0x2));
    /* untranslated: part[144:8](rgrcRef) = rcT */
    DrawFleetGauge(hdc, &(rcT), 0x0, 0x7);

L_0ea7:
    if ((IsWindowVisible(HIWORD(rghwndOrderDD)) == 0x0))
        goto L_0efc;
    else
        goto L_0eb8;

L_0eb8:
    ShowWindow(HIWORD(rghwndOrderDD), SW_HIDE);
    SetRect(&(rc), (xLeft + 0xffff), yTop, (xRight + 0x1), (yBot + 0x2));
    FillRect(hdc, &(rc), hbrButtonFace);

L_0efc:
    yTop = (yTop + (dyShipDD + 0x3));
    lpord = &(sel.fl.lpplord->rgord[sel.iwpAct]);
    if ((lpord->grobj != grobjPlanet))
        goto L_0f5c;
    else
        goto L_0f44;

L_0f44:
    lppl = LpplFromId(lpord->id);
    goto L_0f66;

L_0f5c:
    lppl = 0x0;

L_0f66:
    if ((grtask != 0x1))
        goto L_11f1;
    else
        goto L_0f6f;

L_0f6f:
    dxKt = 0x0;
    i = 0x0;
    goto L_0fbd;

L_0f7c:
    if ((LOWORD(GetTextExtent(hdc, vrgszUnits[i], 0x2)) <= dxKt))
        goto L_0fb9;
    else
        goto L_0f9d;

L_0f9d:
    dxKt = LOWORD(GetTextExtent(hdc, vrgszUnits[i], 0x2));

L_0fb9:
    i = (i + 0x1);

L_0fbd:
    if ((i < 0x5))
        goto L_0f7c;
    else
        goto L_0fc6;

L_0fc6:
    i = LOWORD(SendMessage(HIWORD(rghwndOrderDD), CB_GETCURSEL, 0x0, 0x0));
    if ((i != 0x0))
        goto L_0ff3;
    else
        goto L_0feb;

L_0feb:
    i = 0x4;
    goto L_0ff7;

L_0ff3:
    i = (i - 0x1);

L_0ff7:
    SelectObject(hdc, HIWORD(rghfontArial8));
    edWid = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN99999Kt), 0x9));
    dxRight = (dxRight - (edWid + 0x8));
    swp = 0x14;
    /* untranslated: branch part[4:2](rgdxOrderDD) != dxRight ? L_1043 : L_103c */

L_103c:
    swp = (swp | 0x1);
    goto L_1049;

L_1043:
    /* untranslated: part[4:2](rgdxOrderDD) = dxRight */

L_1049:
    /* untranslated: call SetWindowPos(part[4:2](rghwndOrderDD), 0x0, xLeft, yTop, dxRight, loword((0x9 * dyShipDD)), swp) -> callresult(int16_t) */
    /* untranslated: call ShowWindow(part[4:2](rghwndOrderDD), SW_SHOW) -> callresult(int16_t) */
    /* untranslated: l = SendMessage(part[4:2](rghwndOrderDD), CB_GETCURSEL, 0x0, 0x0) */
    fActive = 0x1;
    goto L_10b8;

L_10a5:
    fActive = 0x0;
    goto L_111b;

L_10ad:
    i = 0x5;
    goto L_111b;

L_10b8:
    if ((LOWORD(l) != 0x0))
        goto L_10c8;
    else
        goto L_10c0;

L_10c0:
    if ((HIWORD(l) == 0x0))
        goto L_10a5;
    else
        goto L_10c8;

L_10c8:
    if ((LOWORD(l) != 0x1))
        goto L_10d8;
    else
        goto L_10d0;

L_10d0:
    if ((HIWORD(l) == 0x0))
        goto L_10a5;
    else
        goto L_10d8;

L_10d8:
    if ((LOWORD(l) != 0x2))
        goto L_10e8;
    else
        goto L_10e0;

L_10e0:
    if ((HIWORD(l) == 0x0))
        goto L_10a5;
    else
        goto L_10e8;

L_10e8:
    if ((LOWORD(l) != 0x5))
        goto L_10f8;
    else
        goto L_10f0;

L_10f0:
    if ((HIWORD(l) == 0x0))
        goto L_10ad;
    else
        goto L_10f8;

L_10f8:
    if ((LOWORD(l) != 0x6))
        goto L_1108;
    else
        goto L_1100;

L_1100:
    if ((HIWORD(l) == 0x0))
        goto L_10ad;
    else
        goto L_1108;

L_1108:
    if ((LOWORD(l) != 0x7))
        goto L_111b;
    else
        goto L_1110;

L_1110:
    if ((HIWORD(l) == 0x0))
        goto L_10a5;
    else
        goto L_111b;

L_111b:
    psz = vrgszUnits[i];
    swp = 0x14;
    if ((dxOrderED != (edWid - dxKt)))
        goto L_1142;
    else
        goto L_113b;

L_113b:
    swp = (swp | 0x1);
    goto L_114b;

L_1142:
    dxOrderED = (edWid - dxKt);

L_114b:
    SetWindowPos(hwndOrderED, 0x0, ((xRight - edWid) + 0xfffc), yTop, (edWid - dxKt), dyShipDD, swp);
    EnableWindow(hwndOrderED, fActive);
    ShowWindow(hwndOrderED, SW_SHOW);
    SetRect(&(rc), ((xRight - dxKt) + 0xffff), (yTop + 0x2), (xRight + 0xffff), yBot);
    FillRect(hdc, &(rc), hbrButtonFace);
    if ((fActive == 0x0))
        goto L_120b;
    else
        goto L_11d0;

L_11d0:
    DrawText(hdc, psz, 0x2, &(rc), 0x800);

L_11f1:
    /* untranslated: call ShowWindow(part[4:2](rghwndOrderDD), SW_HIDE) -> callresult(int16_t) */
    ShowWindow(hwndOrderED, SW_HIDE);

L_120b:
    if ((grtask != 0x5))
        goto L_121f;
    else
        goto L_1214;

L_1214:
    ids = idsNoteShipsFleetWillDismantledMineralsCan;
    goto LDisplayMsg;

L_121f:
    if ((grtask != 0x4))
        goto L_12d8;
    else
        goto L_1228;

L_1228:
    /* untranslated: branch ((part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) >> 0x8) & 0xf) == 0x2 ? L_17af : L_1254 */

L_1254:
    ids = idsWarningDestinationWaypointFleetMergeWillSucessfu;
    psz = PszGetCompressedString(ids);
    SetTextColor(hdc, 0x7f);
    SetRect(&(rc), xLeft, yTopMsg, xRight, (yBot + 0x2));
    SelectObject(hdc, rghfontArial7);
    DrawText(hdc, psz, strlen(psz), &(rc), 0x810);
    SetTextColor(hdc, crButtonText);

L_12d8:
    if ((grtask != 0x6))
        goto L_135a;
    else
        goto L_12e1;

L_12e1:
    yTopMsg = yTop;
    CLayMinesFromLpfl(&(sel.fl), 0xffff, 0xffff);
    /* untranslated: l = callresult(int32_t) */
    /* untranslated: branch hiword(callresult(int32_t)) > 0x0 ? L_1324 : L_130c */

L_130c:
    /* untranslated: branch hiword(callresult(int32_t)) < 0x0 ? L_1319 : L_1311 */

L_1311:
    /* untranslated: branch loword(callresult(int32_t)) > 0x0 ? L_1324 : L_1319 */

L_1319:
    ids = idsWarningFleetHasMineLayingPods;
    goto LDisplayMsg;

L_1324:
    pszT = PszGetCompressedString(idsFleetCanLayLdMinesPerYear);
    _wsprintf(szWork, pszT, LOWORD(l), HIWORD(l));
    psz = 0x57a4;
    goto LDisplayMsg2;

L_135a:
    if ((grtask != 0x2))
        goto L_1533;
    else
        goto L_1363;

L_1363:
    fActive = 0x0;
    i = 0x0;
    goto L_1466;

L_1370:
    if ((sel.fl.rgcsh[i] <= 0x0))
        goto L_1462;
    else
        goto L_1382;

L_1382:
    j = 0x0;
    goto L_138e;

L_138a:
    j = (j + 0x1);

L_138e:
    if ((j >= rglpshdef[sel.fl.iPlayer][i].hul.chs))
        goto L_1462;
    else
        goto L_13b7;

L_13b7:
    if ((rglpshdef[sel.fl.iPlayer][i].hul.rghs[j].grhst != hstSpecialM))
        goto L_138a;
    else
        goto L_13e9;

L_13e9:
    if (((HIWORD(rglpshdef[sel.fl.iPlayer][i].hul.rghs[j]) & 0xff) == 0x0))
        goto L_1457;
    else
        goto L_1420;

L_1420:
    if (((HIWORD(rglpshdef[sel.fl.iPlayer][i].hul.rghs[j]) & 0xff) != 0x1))
        goto L_138a;
    else
        goto L_1457;

L_1457:
    fActive = 0x1;
    goto FoundColony;

L_1462:
    i = (i + 0x1);

L_1466:
    if ((i < 0x10))
        goto L_1370;
    else
        goto FoundColony;

FoundColony:
    if ((fActive == 0x0))
        goto L_149c;
    else
        goto L_1478;

L_1478:
    /* untranslated: branch part[116:2](sel) != 0x0 ? L_1494 : L_1482 */

L_1482:
    /* untranslated: branch part[118:2](sel) != 0x0 ? L_1494 : L_148c */

L_148c:
    ids = idsRememberLoadColonistsBeforeEmbarkingMission;
    goto LDisplayMsg;

L_1494:
    ids = idsNoteShipsFleetWillDismantledProvideSupplies;

L_149c:
    ids = idsWarningColonizeMissionCannotCarriedBecauseNone;

LDisplayMsg:
    psz = PszGetCompressedString(ids);

LDisplayMsg2:
    if ((ids != idsNoteShipsFleetWillDismantledProvideSupplies))
        goto L_14c5;
    else
        goto L_14bb;

L_14bb:
    t_merge_14cb_0001 = crButtonText;
    goto L_14cb;

L_14c5:
    t_merge_14cb_0001 = 0x7f;

L_14cb:
    SetTextColor(hdc, t_merge_14cb_0001);
    SetRect(&(rc), xLeft, yTopMsg, xRight, (yBot + 0x2));
    SelectObject(hdc, rghfontArial7);
    DrawText(hdc, psz, strlen(psz), &(rc), 0x810);
    SetTextColor(hdc, crButtonText);
    goto L_17af;

L_1533:
    if ((grtask != 0x3))
        goto L_17af;
    else
        goto L_153c;

L_153c:
    cMine = CMineFromLpfl(&(sel.fl));
    if ((HIWORD(cMine) < 0x0))
        goto L_1701;
    else
        goto L_155a;

L_155a:
    if ((HIWORD(cMine) > 0x0))
        goto L_1568;
    else
        goto L_155f;

L_155f:
    if ((LOWORD(cMine) <= 0x0))
        goto L_1701;
    else
        goto L_1568;

L_1568:
    if ((LOWORD(lppl) != 0x0))
        goto L_157a;
    else
        goto L_1571;

L_1571:
    if ((HIWORD(lppl) == 0x0))
        goto L_16f9;
    else
        goto L_157a;

L_157a:
    if ((lppl->iPlayer == 0xffff))
        goto L_15b7;
    else
        goto L_1587;

L_1587:
    if ((lppl->iPlayer != sel.fl.iPlayer))
        goto L_16f9;
    else
        goto L_1596;

L_1596:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_16f9;
    else
        goto L_15b7;

L_15b7:
    if ((dyArial8 <= 0xe))
        goto L_15d6;
    else
        goto L_15c1;

L_15c1:
    SelectObject(hdc, rghfontArial7);
    dyCur = dyArial7;
    goto L_15e8;

L_15d6:
    SelectObject(hdc, LOWORD(rghfontArial8));
    dyCur = dyArial8;

L_15e8:
    if ((lppl->det > 0x1))
        goto L_1602;
    else
        goto L_15fa;

L_15fa:
    ids = 0xf3;
    goto ShowString;

L_1602:
    EstMineralsMined(lppl, rgl, cMine, 0x0);
    c = CchGetString(idsMiningRatePerYear, 0x57a4);
    TextOut(hdc, xLeft, yTopMsg, szWork, c);
    yTopMsg = (yTopMsg + dyCur);
    dxRight = xLeft;
    i = 0x0;
    goto L_16ed;

L_165d:
    SetTextColor(hdc, rgcrMinerals[i]);
    c = _wsprintf(szWork, PCTLD, LOWORD(rgl[i]), HIWORD(rgl[i]));
    DxStreamTextOut(hdc, &(dxRight), yTopMsg, 0x57a4, c, 0x1);
    SetTextColor(hdc, crButtonText);
    DxStreamTextOut(hdc, &(dxRight), yTopMsg, 0x97f, 0x4, 0x1);
    i = (i + 0x1);

L_16ed:
    if ((i >= 0x3))
        goto DoneMine;
    else
        goto L_16f3;

L_16f3:

L_16f9:
    ids = 0xe5;
    goto L_1706;

L_1701:
    ids = 0xe2;

L_1706:
    if ((dyArial8 <= 0xe))
        goto L_171f;
    else
        goto L_1710;

L_1710:
    SelectObject(hdc, rghfontArial6);
    goto ShowString;

L_171f:
    SelectObject(hdc, rghfontArial7);

ShowString:
    if ((ids != 0xe5))
        goto L_1742;
    else
        goto L_1738;

L_1738:
    t_merge_1748_0001 = crButtonText;
    goto L_1748;

L_1742:
    t_merge_1748_0001 = 0x7f;

L_1748:
    SetTextColor(hdc, t_merge_1748_0001);
    psz = PszGetCompressedString(ids);
    SetRect(&(rc), xLeft, yTopMsg, xRight, (yBot + 0x2));
    DrawText(hdc, psz, strlen(psz), &(rc), 0x810);

DoneMine:
    SetTextColor(hdc, crButtonText);

L_17af:
    return;
}

void DrawShipPlanet(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t  yTop;
    int16_t  dy;
    int16_t  i;
    int16_t  xRight;
    char    *psz;
    int16_t  dx;
    int16_t  xLeft;
    RECT     rc;
    THING   *lpth;
    THING   *lpthMac;
    uint16_t t_merge_189d_0001;
    uint16_t t_merge_18e6_0001;
    uint16_t t_merge_1905_0001;
    uint16_t t_merge_19c4_0001;

L_17b6:
    /* untranslated: branch part[6:2](obj) == 0xffff ? L_17e3 : L_17cb */

L_17cb:
    /* untranslated: psz = PszGetPlanetName((part[6:2](obj) | 0x8000)) */
    goto L_17f2;

L_17e3:
    psz = PszGetCompressedString(idsDeepSpace2);

L_17f2:
    if ((ptile->fFixCtls == 0x0))
        goto L_1834;
    else
        goto L_1808;

L_1808:
    /* untranslated: call ShowWindow(part[6:2](rghwndBtn), SW_HIDE) -> callresult(int16_t) */
    /* untranslated: call ShowWindow(part[14:2](rghwndBtn), SW_HIDE) -> callresult(int16_t) */
    ptile->fFixCtls = 0x0;

L_1834:
    if ((FDrawTileNC(hdc, ptile, &(rc), psz) != 0x0))
        goto L_186e;
    else
        goto L_1851;

L_1851:
    /* untranslated: call ShowWindow(part[6:2](rghwndBtn), SW_HIDE) -> callresult(int16_t) */
    /* untranslated: call ShowWindow(part[14:2](rghwndBtn), SW_HIDE) -> callresult(int16_t) */
    goto L_1a4d;

L_186e:
    xLeft = (rc.left + 0x4);
    xRight = (rc.right + 0xfffc);
    if ((gd.fSmallTileMode == 0x0))
        goto L_189a;
    else
        goto L_1894;

L_1894:
    t_merge_189d_0001 = 0x1;
    goto L_189d;

L_189a:
    t_merge_189d_0001 = 0x4;

L_189d:
    yTop = (t_merge_189d_0001 + rc.top);
    dx = ((uint32_t)(((xRight - xLeft) + 0xfff0)) / 0x3);
    dy = (LOWORD((0x3 * dyArial8)) >> 0x1);
    /* untranslated: branch part[6:2](obj) == 0xffff ? L_18e3 : L_18d1 */

L_18d1:
    if ((sel.pl.iPlayer != idPlayer))
        goto L_18e3;
    else
        goto L_18dd;

L_18dd:
    t_merge_18e6_0001 = 0x1;
    goto L_18e6;

L_18e3:
    t_merge_18e6_0001 = 0x0;

L_18e6:
    /* untranslated: call EnableWindow(part[6:2](rghwndBtn), t_merge_18e6_0001) -> callresult(int16_t) */
    /* untranslated: branch part[6:2](obj) == 0xffff ? L_1902 : L_18fc */

L_18fc:
    t_merge_1905_0001 = 0x376;
    goto L_1905;

L_1902:
    t_merge_1905_0001 = 0x375;

L_1905:
    /* untranslated: call SetWindowText(part[14:2](rghwndBtn), PszGetCompressedString(t_merge_1905_0001)) -> callresult(void) */
    /* untranslated: branch part[6:2](obj) != 0xffff ? L_19cd : L_1923 */

L_1923:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_1993;

L_194e:
    if ((lpth->ith != ithMineralPacket))
        goto L_198f;
    else
        goto L_1964;

L_1964:
    /* untranslated: branch 0x25:[(obj + 0x8)] != lpth->pt.x ? L_198f : L_1983 */

L_1983:
    /* untranslated: branch 0x25:[(obj + 0x8)+0x2] == lpth->pt.y ? L_19a1 : L_1989 */

L_1989:

L_198f:
    lpth = (lpth + 0x1);

L_1993:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_194e;
    else
        goto L_19a1;

L_19a1:
    if ((LOWORD(lpth) != LOWORD(lpthMac)))
        goto L_19c1;
    else
        goto L_19b3;

L_19b3:
    if ((HIWORD(lpth) != HIWORD(lpthMac)))
        goto L_19c1;
    else
        goto L_19bb;

L_19bb:
    t_merge_19c4_0001 = 0x1;
    goto L_19c4;

L_19c1:
    t_merge_19c4_0001 = 0x0;

L_19c4:
    /* untranslated: call EnableWindow(part[14:2](rghwndBtn), t_merge_19c4_0001) -> callresult(int16_t) */
    goto L_19da;

L_19cd:
    /* untranslated: call EnableWindow(part[14:2](rghwndBtn), 0x1) -> callresult(int16_t) */

L_19da:
    if ((ptile->fMinDraw != 0x0))
        goto L_1a4d;
    else
        goto L_19ed;

L_19ed:

L_19f3:
    i = 0x3;
    goto L_1a0d;

L_19fb:
    i = (i + 0x4);
    xLeft = (xLeft + ((dx * 0x2) + 0x10));

L_1a0d:
    if ((i > 0x7))
        goto L_1a4d;
    else
        goto L_1a16;

L_1a16:
    SetWindowPos(rghwndBtn[i], 0x0, xLeft, yTop, dx, dy, 0x14);
    ShowWindow(rghwndBtn[i], SW_SHOW);
    goto L_19fb;

L_1a4d:
    return;
}

void DrawShipCargo(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t  dxRight;
    int32_t  l2;
    int16_t  yTop;
    int16_t  i;
    int16_t  c;
    FLEET   *pfl;
    int16_t  xRight;
    RECT     rcGauge;
    int16_t  xLeft;
    int32_t  l;
    RECT     rc;
    uint16_t t_merge_1bff_0001;

L_1a54:
    pfl = obj;
    if ((ptile->fFixCtls == 0x0))
        goto L_1a91;
    else
        goto L_1a79;

L_1a79:
    /* untranslated: part[18:2](rgrcRef) = 0xfffb */
    /* untranslated: part[22:2](rgrcRef) = 0xfffa */
    /* untranslated: part[26:2](rgrcRef) = 0xfffb */
    /* untranslated: part[30:2](rgrcRef) = 0xfffa */

L_1a91:
    if ((FDrawTileNC(hdc, ptile, &(rc), PszGetCompressedString(idsFuelCargo)) == 0x0))
        goto L_1e6c;
    else
        goto L_1ab5;

L_1ab5:

L_1abb:
    xLeft = (rc.left + 0x4);
    xRight = (rc.right + 0xfffc);
    yTop = (rc.top + 0x1);
    dxRight = dxMaxMineralQuan;
    SelectObject(hdc, HIWORD(rghfontArial8));
    c = CchGetString(idsCargo3, 0x57a4);
    l = GetTextExtent(hdc, szWork, c);
    c = CchGetString(idsFuel3, 0x57a4);
    l2 = GetTextExtent(hdc, szWork, c);
    if ((HIWORD(l2) < HIWORD(l)))
        goto L_1b65;
    else
        goto L_1b4c;

L_1b4c:
    if ((HIWORD(l2) > HIWORD(l)))
        goto L_1b59;
    else
        goto L_1b51;

L_1b51:
    if ((LOWORD(l2) <= LOWORD(l)))
        goto L_1b65;
    else
        goto L_1b59;

L_1b59:
    l = l2;

L_1b65:
    if ((ptile->fMinDraw != 0x0))
        goto L_1b93;
    else
        goto L_1b7b;

L_1b7b:
    TextOut(hdc, xLeft, yTop, szWork, c);

L_1b93:
    SetRect(&(rcGauge), (xLeft + LOWORD(l)), yTop, xRight, (yTop + dyArial8));
    /* untranslated: part[16:8](rgrcRef) = rcGauge */
    DrawFleetGauge(hdc, &(rcGauge), pfl, 0x4);
    if ((gd.fSmallTileMode == 0x0))
        goto L_1bfc;
    else
        goto L_1bf6;

L_1bf6:
    t_merge_1bff_0001 = 0x2;
    goto L_1bff;

L_1bfc:
    t_merge_1bff_0001 = 0x4;

L_1bff:
    yTop = (yTop + (t_merge_1bff_0001 + dyArial8));
    if ((ptile->fMinDraw != 0x0))
        goto L_1c47;
    else
        goto L_1c1c;

L_1c1c:
    c = CchGetString(idsCargo3, 0x57a4);
    TextOut(hdc, xLeft, yTop, szWork, c);

L_1c47:
    SetRect(&(rcGauge), (xLeft + LOWORD(l)), yTop, xRight, (yTop + dyArial8));
    /* untranslated: part[24:8](rgrcRef) = rcGauge */
    DrawFleetGauge(hdc, &(rcGauge), pfl, 0x5);
    yTop = (yTop + (dyArial8 + 0x4));
    if ((gd.fSmallTileMode != 0x0))
        goto L_1e6c;
    else
        goto L_1cb0;

L_1cb0:

L_1cb6:
    i = 0x0;
    goto L_1d9e;

L_1cbe:
    if ((ptile->fMinDraw != 0x0))
        goto L_1d25;
    else
        goto L_1cd4;

L_1cd4:
    SelectObject(hdc, HIWORD(rghfontArial8));
    SetTextColor(hdc, rgcrMinerals[i]);
    TextOut(hdc, xLeft, yTop, rgszMinerals[i], lstrlen(rgszMinerals[i]));

L_1d25:
    SelectObject(hdc, LOWORD(rghfontArial8));
    SetTextColor(hdc, crButtonText);
    c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), LOWORD(pfl->rgwtMin[i]), HIWORD(pfl->rgwtMin[i]));
    RightTextOut(hdc, xRight, yTop, 0x57a4, c, dxRight);
    yTop = (yTop + dyArial8);
    i = (i + 0x1);

L_1d9e:
    if ((i <= 0x2))
        goto L_1cbe;
    else
        goto L_1da7;

L_1da7:
    if ((ptile->fMinDraw != 0x0))
        goto L_1e20;
    else
        goto L_1dbd;

L_1dbd:
    SelectObject(hdc, HIWORD(rghfontArial8));
    SetTextColor(hdc, 0xffffff);
    c = CchGetString(idsColonists2, 0x57a4);
    TextOut(hdc, xLeft, yTop, szWork, c);
    SelectObject(hdc, LOWORD(rghfontArial8));
    SetTextColor(hdc, crButtonText);

L_1e20:
    c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), LOWORD(pfl->rgwtMin[0x3]), HIWORD(pfl->rgwtMin[0x3]));
    RightTextOut(hdc, xRight, yTop, 0x57a4, c, dxRight);
    yTop = (yTop + dyArial8);

L_1e6c:
    return;
}

void DrawFleetComp(HDC hdc, TILE *ptile, OBJ obj) {
    int32_t  cBoat;
    int16_t  swp;
    int16_t  dxRight;
    int16_t  yTop;
    RECT     rcT;
    int16_t  dyWrong;
    int16_t  c;
    int16_t  i;
    FLEET   *pfl;
    int16_t  xStart;
    int16_t  xRight;
    int16_t  dxLabel;
    int16_t  xLeft;
    int32_t  l;
    RECT     rc;
    uint16_t t_merge_1fab_0001;
    uint16_t t_merge_205c_0001;

L_1e72:
    pfl = obj;
    if ((ptile->fFixCtls == 0x0))
        goto L_1eea;
    else
        goto L_1e97;

L_1e97:
    ShowWindow(hwndFleetCompLB, SW_HIDE);
    /* untranslated: call ShowWindow(part[16:2](rghwndBtn), SW_HIDE) -> callresult(int16_t) */
    /* untranslated: call ShowWindow(part[18:2](rghwndBtn), SW_HIDE) -> callresult(int16_t) */
    /* untranslated: call ShowWindow(part[20:2](rghwndBtn), SW_HIDE) -> callresult(int16_t) */
    ShowWindow(hwndBattleDD, SW_HIDE);
    ptile->fFixCtls = 0x0;

L_1eea:
    if ((FDrawTileNC(hdc, ptile, &(rc), PszGetCompressedString(idsFleetComposition)) != 0x0))
        goto L_1f55;
    else
        goto L_1f11;

L_1f11:
    ShowWindow(hwndFleetCompLB, SW_HIDE);
    /* untranslated: call ShowWindow(part[16:2](rghwndBtn), SW_HIDE) -> callresult(int16_t) */
    /* untranslated: call ShowWindow(part[18:2](rghwndBtn), SW_HIDE) -> callresult(int16_t) */
    /* untranslated: call ShowWindow(part[20:2](rghwndBtn), SW_HIDE) -> callresult(int16_t) */
    ShowWindow(hwndBattleDD, SW_HIDE);
    goto L_2455;

L_1f55:
    xLeft = (rc.left + 0x4);
    xRight = (rc.right + 0xfffc);
    yTop = (rc.top + 0x3);
    GetClientRect(hwndFleetCompLB, &(rcT));
    swp = 0x14;
    if ((gd.fSmallTileMode == 0x0))
        goto L_1fa8;
    else
        goto L_1fa2;

L_1fa2:
    t_merge_1fab_0001 = 0x3;
    goto L_1fab;

L_1fa8:
    t_merge_1fab_0001 = 0x5;

L_1fab:
    /* untranslated: dyFleetCompLB = loword((ss:[bp-0x32] * t_merge_1fab_0001)) */
    dyWrong = (dyFleetCompLB - (rcT.bottom - rcT.top));
    if ((dxFleetCompLB != (xRight - xLeft)))
        goto L_1fee;
    else
        goto L_1fd3;

L_1fd3:
    if ((dyWrong < 0x0))
        goto L_1fee;
    else
        goto L_1fdc;

L_1fdc:
    if ((dyWrong >= dyArial8))
        goto L_1fee;
    else
        goto L_1fe7;

L_1fe7:
    swp = (swp | 0x1);
    goto L_1ff7;

L_1fee:
    dxFleetCompLB = (xRight - xLeft);

L_1ff7:
    SetWindowPos(hwndFleetCompLB, 0x0, xLeft, yTop, (xRight - xLeft), dyFleetCompLB, swp);
    ShowWindow(hwndFleetCompLB, SW_SHOW);
    GetClientRect(hwndFleetCompLB, &(rcT));
    dyFleetCompLB = (rcT.bottom - rcT.top);
    if ((gd.fSmallTileMode == 0x0))
        goto L_2059;
    else
        goto L_2053;

L_2053:
    t_merge_205c_0001 = 0x2;
    goto L_205c;

L_2059:
    t_merge_205c_0001 = 0x4;

L_205c:
    yTop = (yTop + (t_merge_205c_0001 + dyFleetCompLB));
    SelectObject(hdc, HIWORD(rghfontArial8));
    c = CchGetString(idsBattlePlan2, 0x57a4);
    dxLabel = LOWORD(GetTextExtent(hdc, szWork, c));
    if ((ptile->fMinDraw != 0x0))
        goto L_20c9;
    else
        goto L_20ad;

L_20ad:
    TextOut(hdc, xLeft, (yTop + 0x4), szWork, c);

L_20c9:
    swp = 0x14;
    if ((dxBattleDD != ((xRight - xLeft) - dxLabel)))
        goto L_20e7;
    else
        goto L_20e0;

L_20e0:
    swp = (swp | 0x1);
    goto L_20f3;

L_20e7:
    dxBattleDD = ((xRight - xLeft) - dxLabel);

L_20f3:
    SetWindowPos(hwndBattleDD, 0x0, (xLeft + dxLabel), yTop, dxBattleDD, LOWORD((0x5 * dyShipDD)), swp);
    ShowWindow(hwndBattleDD, SW_SHOW);
    yTop = (yTop + (dyShipDD + 0x3));
    c = CchGetString(idsEstRange, 0x57a4);
    l = GetTextExtent(hdc, szWork, c);
    if ((ptile->fMinDraw != 0x0))
        goto L_2189;
    else
        goto L_2171;

L_2171:
    TextOut(hdc, xLeft, yTop, szWork, c);

L_2189:
    SelectObject(hdc, LOWORD(rghfontArial8));
    c = CchGetString(idsN9999LY, 0x57a4);
    dxRight = (LOWORD(GetTextExtent(hdc, szWork, c)) + 0x6);
    i = IFindIdealWarp(0x0, 0x0);
    l = EstFuelUse(&(sel.fl), 0x0, i, 0xffffffff, 0x1);
    if ((HIWORD(l) < 0x3b9a))
        goto L_222e;
    else
        goto L_2209;

L_2209:
    if ((HIWORD(l) > 0x3b9a))
        goto L_2218;
    else
        goto L_220e;

L_220e:
    if ((LOWORD(l) < 0xca00))
        goto L_222e;
    else
        goto L_2218;

L_2218:
    c = CchGetString(idsInfinite, 0x57a4);
    goto L_2256;

L_222e:
    c = _wsprintf(szWork, PszGetCompressedString(idsLdLY), LOWORD(l), HIWORD(l));

L_2256:
    RightTextOut(hdc, xRight, yTop, 0x57a4, c, dxRight);
    if ((gd.fSmallTileMode != 0x0))
        goto L_234c;
    else
        goto L_2285;

L_2285:
    yTop = (yTop + dyArial8);
    if ((ptile->fMinDraw != 0x0))
        goto L_22d8;
    else
        goto L_22a1;

L_22a1:
    SelectObject(hdc, HIWORD(rghfontArial8));
    c = CchGetString(idsPercentCloaked, 0x57a4);
    TextOut(hdc, xLeft, yTop, szWork, c);

L_22d8:
    SelectObject(hdc, LOWORD(rghfontArial8));
    i = PctCloakFromLpfl(&(sel.fl));
    if ((i != 0x0))
        goto L_2315;
    else
        goto L_22ff;

L_22ff:
    c = CchGetString(idsNone2, 0x57a4);
    goto L_2331;

L_2315:
    c = _wsprintf(szWork, PCTDPCTPCT, i);

L_2331:
    RightTextOut(hdc, xRight, yTop, 0x57a4, c, dxRight);

L_234c:
    yTop = (yTop + ((dyArial8 + 0x2) - gd.fSmallTileMode));
    xStart = xLeft;
    c = ((uint32_t)(((xRight - xLeft) + 0xfff6)) / 0x3);
    i = 0x8;
    goto L_2397;

L_2384:
    i = (i + 0x1);
    xStart = (xStart + (c + 0x6));

L_2397:
    if ((i > 0xa))
        goto L_23de;
    else
        goto L_23a0;

L_23a0:
    SetWindowPos(rghwndBtn[i], 0x0, xStart, yTop, c, ((dyArial8 >> 0x1) + dyArial8), 0x14);
    ShowWindow(rghwndBtn[i], SW_SHOW);
    goto L_2384;

L_23de:
    cBoat = 0x0;
    i = 0x0;
    goto L_2407;

L_23f0:
    cBoat = (cBoat + (uint32_t)(sel.fl.rgcsh[i]));
    i = (i + 0x1);

L_2407:
    if ((i < 0x10))
        goto L_23f0;
    else
        goto L_2410;

L_2410:
    /* untranslated: call EnableWindow(part[16:2](rghwndBtn), FCanSplit(cBoat)) -> callresult(int16_t) */
    /* untranslated: call EnableWindow(part[18:2](rghwndBtn), FCanSplitAll(cBoat)) -> callresult(int16_t) */
    /* untranslated: call EnableWindow(part[20:2](rghwndBtn), FCanMerge(pfl)) -> callresult(int16_t) */

L_2455:
    return;
}

int16_t FCanSplit(int32_t cBoat) {
L_245c:
    /* untranslated: branch (part[4:2](rgplr[idPlayer]) & 0xfff) != 0x200 ? L_2485 : L_247f */

L_247f:
    return 0x0;

L_2485:
    if ((HIWORD(cBoat) < 0x0))
        goto L_24a2;
    else
        goto L_248e;

L_248e:
    if ((HIWORD(cBoat) > 0x0))
        goto L_249c;
    else
        goto L_2493;

L_2493:
    if ((LOWORD(cBoat) <= 0x1))
        goto L_24a2;
    else
        goto L_249c;

L_249c:
    return 0x1;

L_24a2:
    return 0x0;
}

int16_t FCanSplitAll(int32_t cBoat) {
L_24ae:
    if (((HIWORD(cBoat) + 0xffff) < 0x0))
        goto L_24f8;
    else
        goto L_24e4;

L_24e4:
    if (((HIWORD(cBoat) + 0xffff) > 0x0))
        goto L_24f2;
    else
        goto L_24e9;

L_24e9:
    /* untranslated: branch ((LOWORD(cBoat) + 0xffff) + (part[4:2](rgplr[idPlayer]) & 0xfff)) <= 0x200 ? L_24f8 : L_24f2 */

L_24f2:
    return 0x0;

L_24f8:
    if ((HIWORD(cBoat) < 0x0))
        goto L_2515;
    else
        goto L_2501;

L_2501:
    if ((HIWORD(cBoat) > 0x0))
        goto L_250f;
    else
        goto L_2506;

L_2506:
    if ((LOWORD(cBoat) <= 0x1))
        goto L_2515;
    else
        goto L_250f;

L_250f:
    return 0x1;

L_2515:
    return 0x0;
}

int16_t FCanMerge(FLEET *pfl) {
    int16_t i;
    FLEET  *lpfl;
    int32_t csh;
    int16_t cfl;
    int16_t ishdef;

L_2522:
    cfl = 0x0;
    csh = 0x0;
    i = 0x0;
    goto L_2546;

L_2542:
    i = (i + 0x1);

L_2546:
    if ((i >= cFleet))
        goto L_25f0;
    else
        goto L_2551;

L_2551:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_2581;
    else
        goto L_2579;

L_2579:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_25f0;
    else
        goto L_2581;

L_2581:
    if ((lpfl->iPlayer != pfl->iPlayer))
        goto L_2542;
    else
        goto L_2593;

L_2593:
    if ((lpfl->pt.x != sel.fl.pt.x))
        goto L_2542;
    else
        goto L_25ad;

L_25ad:
    if ((lpfl->pt.y != sel.fl.pt.y))
        goto L_2542;
    else
        goto L_25b6;

L_25b6:
    cfl = (cfl + 0x1);
    ishdef = 0x0;
    goto L_25e4;

L_25c2:
    csh = (csh + (uint32_t)(lpfl->rgcsh[ishdef]));
    ishdef = (ishdef + 0x1);

L_25e4:
    if ((ishdef >= 0x10))
        goto L_2542;
    else
        goto L_25ea;

L_25ea:

L_25f0:
    if ((cfl == 0x1))
        goto L_262d;
    else
        goto L_25f9;

L_25f9:
    if ((HIWORD(csh) < 0x0))
        goto L_2633;
    else
        goto L_2620;

L_2620:
    if ((HIWORD(csh) > 0x0))
        goto L_262d;
    else
        goto L_2625;

L_2625:
    /* untranslated: branch LOWORD(csh) <= (0x7ffe - ((part[4:2](rgplr[pfl->iPlayer]) & 0xfff) + 0xffff)) ? L_2633 : L_262d */

L_262d:
    return 0x0;

L_2633:
    return 0x1;
}

void ShipCommandProc(HWND hwnd, uint16_t wParam, int32_t lParam) {
    int16_t fPercent;
    int16_t (**lpProc)();
    int32_t  lSel;
    XFER     xf;
    char     szT[34];
    int32_t  lMin;
    int16_t  ishdef;
    int16_t  grbit;
    int16_t  ifl;
    FLEET   *lpfl;
    int16_t  rgifl[512];
    int16_t  ish;
    int16_t  ishPrimary;
    FLEET   *lpflBest;
    char     rgb[8];
    int16_t  i;
    int16_t  iInit;
    uint16_t t_2a32;
    uint16_t t_merge_3b92_0001;
    uint16_t t_merge_3bb5_0001;
    uint16_t t_merge_3bd2_0001;

L_2640:
    fPercent = 0x0;
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_2673;
    else
        goto L_266a;

L_266a:
    SetFocus(hwndFrame);

L_2673:
    /* untranslated: branch LOWORD(lParam) != part[8:2](rghwndBtn) ? L_26b1 : L_2682 */

L_2682:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_26b1;
    else
        goto L_269e;

L_269e:
    SelectAdjFleet(0xffff, 0x0);
    goto L_3d2c;

L_26b1:
    /* untranslated: branch LOWORD(lParam) != part[10:2](rghwndBtn) ? L_26ef : L_26c0 */

L_26c0:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_26ef;
    else
        goto L_26dc;

L_26dc:
    SelectAdjFleet(0x1, 0x0);
    goto L_3d2c;

L_26ef:
    /* untranslated: branch LOWORD(lParam) != part[12:2](rghwndBtn) ? L_2845 : L_26fe */

L_26fe:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_2845;
    else
        goto L_271a;

L_271a:
    strcpy(0x57a4, PszGetFleetName(sel.fl.id));
    StickyDlgPos(hwnd, 0x8e0, 0x0);
    lpProc = MakeProcInstance(RenameDlg, hInst);
    if ((DialogBox(hInst, IDD_RENAME, hwndFrame, &(lpProc)) == 0x0))
        goto L_27c4;
    else
        goto L_2780;

L_2780:
    FreeProcInstance(&(lpProc));
    strcpy(szT, 0x57a4);
    if ((strcmp(szT, PszGetFleetName(sel.fl.id)) == 0x0))
        goto L_3d2c;
    else
        goto L_27bb;

L_27bb:

L_27c4:
    FreeProcInstance(&(lpProc));
    goto L_3d2c;

L_27d2:
    LogChangeName(grobjFleet, sel.fl.id, szT);
    InvalidateReport(0x1, 0x1);
    FillOrdersLB();
    DrawPlanShip(0x0, 0x80a0);
    InvalidateRect(hwndMessage, 0x0, 0x1);
    InvalidateRect(hwndScanner, 0x0, 0x1);
    SetMineralTitleBar(hwndMine);
    goto L_3d2c;

L_2845:
    /* untranslated: branch LOWORD(lParam) != part[6:2](rghwndBtn) ? L_288f : L_2854 */

L_2854:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_288f;
    else
        goto L_2870;

L_2870:
    SelectAdjPlanet(0x0, sel.fl.idPlanet);
    SetFleetDropDownSel(sel.fl.id);
    goto L_3d2c;

L_288f:
    /* untranslated: branch LOWORD(lParam) != part[14:2](rghwndBtn) ? L_2905 : L_289e */

L_289e:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_2905;
    else
        goto L_28ba;

L_28ba:
    if ((sel.fl.idPlanet == 0xffff))
        goto L_28e3;
    else
        goto L_28c4;

L_28c4:
    TransferStuff(sel.fl.id, grobjFleet, sel.fl.idPlanet, grobjPlanet, 0x0);
    goto L_3d2c;

L_28e3:
    TransferStuff(sel.fl.id, grobjFleet, 0xffff, grobjOther, 0x0);

L_2905:
    if ((LOWORD(lParam) != hwndShipDD))
        goto L_294f;
    else
        goto L_2914;

L_2914:
    goto L_2941;

L_292b:
    DrawPlanShip(0x0, 0x8004);
    goto L_3d2c;

L_2941:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0xffff) != 0x1 ? L_3d2c : L_2946 */

L_2946:

L_294f:
    if ((LOWORD(lParam) != hwndShipLB))
        goto L_29b7;
    else
        goto L_295e;

L_295e:
    goto L_29a9;

L_2975:
    lSel = SendMessage(hwndShipLB, CB_GETLBTEXTLEN, 0x0, 0x0);
    SetScanWp(LOWORD(lSel));
    goto L_3d2c;

L_29a9:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0xffff) != 0x1 ? L_3d2c : L_29ae */

L_29ae:

L_29b7:
    if ((LOWORD(lParam) != hwndFleetCompLB))
        goto L_2ac0;
    else
        goto L_29c6;

L_29c6:
    goto L_2ab2;

L_29dd:
    lSel = SendMessage(hwndFleetCompLB, CB_GETLBTEXTLEN, 0x0, 0x0);
    if ((HIWORD(lSel) < 0x0))
        goto L_3d2c;
    else
        goto L_2a05;

L_2a05:
    if ((HIWORD(lSel) > 0x0))
        goto L_2a13;
    else
        goto L_2a0a;

L_2a0a:
    if ((LOWORD(lSel) < 0x0))
        goto L_3d2c;
    else
        goto L_2a13;

L_2a13:
    ishdef = 0x0;
    goto L_2a5a;

L_2a1c:
    if ((sel.fl.rgcsh[ishdef] <= 0x0))
        goto L_2a55;
    else
        goto L_2a2f;

L_2a2f:
    LOWORD(lSel) = (LOWORD(lSel) - 0x1);
    t_2a32 = HIWORD(lSel);
    HIWORD(lSel) = (HIWORD(lSel) - 0x0);
    if ((t_2a32 > 0x0))
        goto L_2a55;
    else
        goto L_2a45;

L_2a45:
    if ((HIWORD(lSel) < 0x0))
        goto L_2a64;
    else
        goto L_2a4a;

L_2a4a:
    if ((LOWORD(lSel) <= 0x0))
        goto L_2a64;
    else
        goto L_2a4f;

L_2a4f:

L_2a55:
    ishdef = (ishdef + 0x1);

L_2a5a:
    if ((ishdef < 0x10))
        goto L_2a1c;
    else
        goto L_2a64;

L_2a64:
    GlobalPD.grPopup = grPopupShdef;
    HIWORD(GlobalPD) = rgshdef[ishdef];
    /* untranslated: part[4:2](GlobalPD) = 0x25 */
    /* untranslated: part[8:2](GlobalPD) = 0x0 */
    /* untranslated: part[6:2](GlobalPD) = 0x1 */
    /* untranslated: part[10:2](GlobalPD) = 0x0 */
    /* untranslated: part[12:2](GlobalPD) = 0x0 */
    Popup(hwndFleetCompLB, 0xa, 0xa);

L_2ab2:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0xffff) != 0x1 ? L_3d2c : L_2ab7 */

L_2ab7:

L_2ac0:
    if ((LOWORD(lParam) != hwndBattleDD))
        goto L_2baf;
    else
        goto L_2acf;

L_2acf:
    goto L_2b9e;

L_2ae6:
    lSel = SendMessage(hwndBattleDD, CB_GETCURSEL, 0x0, 0x0);
    if ((LOWORD(lSel) != 0xffff))
        goto L_2b1a;
    else
        goto L_2b0e;

L_2b0e:
    if ((HIWORD(lSel) == 0xffff))
        goto L_3d2c;
    else
        goto L_2b14;

L_2b14:

L_2b1a:
    if ((LOWORD(lSel) != 0x0))
        goto L_2b7c;
    else
        goto L_2b23;

L_2b23:
    if ((HIWORD(lSel) != 0x0))
        goto L_2b7c;
    else
        goto L_2b2c;

L_2b2c:
    lpProc = MakeProcInstance(BattlePlansDlg, hInst);
    lSel = (uint32_t)(DialogBox(hInst, IDD_BATTLE_PLANS, hwndFrame, &(lpProc)));
    FreeProcInstance(&(lpProc));
    /* untranslated: part[124:2](sel) = lobyte(LOWORD(lSel)) */
    goto L_2b88;

L_2b7c:
    /* untranslated: part[124:2](sel) = lobyte((LOWORD(lSel) + 0xffff)) */

L_2b88:
    FLookupFleet(0xffff, 0x4972);
    goto L_3d2c;

L_2b9e:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0xffff) != 0x1 ? L_3d2c : L_2ba3 */

L_2ba3:

L_2baf:
    if ((LOWORD(lParam) != LOWORD(rghwndBtn)))
        goto L_2c54;
    else
        goto L_2bbe;

L_2bbe:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_2c54;
    else
        goto L_2bda;

L_2bda:
    lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0x0);
    if ((LOWORD(lSel) != 0xffff))
        goto L_2c0e;
    else
        goto L_2c02;

L_2c02:
    if ((HIWORD(lSel) == 0xffff))
        goto L_3d2c;
    else
        goto L_2c08;

L_2c08:

L_2c0e:
    if ((FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &(xf), sel.fl.id) == 0x0))
        goto L_3d2c;
    else
        goto L_2c32;

L_2c32:
    TransferStuff(sel.fl.id, grobjFleet, xf.id, xf.grobj, 0x0);

L_2c54:
    if ((LOWORD(lParam) != HIWORD(rghwndBtn)))
        goto L_2cf7;
    else
        goto L_2c63;

L_2c63:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_2cf7;
    else
        goto L_2c7f;

L_2c7f:
    lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0x0);
    if ((LOWORD(lSel) != 0xffff))
        goto L_2cb3;
    else
        goto L_2ca7;

L_2ca7:
    if ((HIWORD(lSel) == 0xffff))
        goto L_3d2c;
    else
        goto L_2cad;

L_2cad:

L_2cb3:
    if ((FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &(xf), sel.fl.id) == 0x0))
        goto L_3d2c;
    else
        goto L_2cd7;

L_2cd7:
    if ((xf.grobj != grobjFleet))
        goto L_3d2c;
    else
        goto L_2ce1;

L_2ce1:
    SelectAdjFleet(0x0, xf.id);

L_2cf7:
    /* untranslated: branch LOWORD(lParam) != part[4:2](rghwndBtn) ? L_2ddc : L_2d06 */

L_2d06:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_2ddc;
    else
        goto L_2d22;

L_2d22:
    lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0x0);
    if ((LOWORD(lSel) != 0xffff))
        goto L_2d56;
    else
        goto L_2d4a;

L_2d4a:
    if ((HIWORD(lSel) == 0xffff))
        goto L_3d2c;
    else
        goto L_2d50;

L_2d50:

L_2d56:
    if ((FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &(xf), sel.fl.id) == 0x0))
        goto L_3d2c;
    else
        goto L_2d7a;

L_2d7a:
    if ((xf.grobj != grobjFleet))
        goto L_3d2c;
    else
        goto L_2d81;

L_2d81:

L_2d87:
    TransferStuff(sel.fl.id, grobjFleet, xf.id, grobjFleet, 0x1);
    if (((grbitScan & 0x80) == 0x0))
        goto L_2dc6;
    else
        goto L_2db1;

L_2db1:
    InvalidateRect(hwndScanner, 0x0, 0x1);

L_2dc6:
    InvalidateReport(0x1, 0x1);
    goto L_3d2c;

L_2ddc:
    /* untranslated: branch LOWORD(lParam) != part[16:2](rghwndBtn) ? L_2e51 : L_2deb */

L_2deb:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_2e51;
    else
        goto L_2e07;

L_2e07:
    TransferStuff(sel.fl.id, grobjFleet, 0xffff, grobjOther, 0x1);
    InvalidateReport(0x1, 0x1);
    if ((gd.fTutorial == 0x0))
        goto L_3d2c;
    else
        goto L_2e46;

L_2e46:
    AdvanceTutor();

L_2e51:
    /* untranslated: branch LOWORD(lParam) != part[18:2](rghwndBtn) ? L_2eda : L_2e60 */

L_2e60:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_2eda;
    else
        goto L_2e7c;

L_2e7c:
    FFleetSplitAll(0x4972);
    FillShipDD(sel.fl.id);
    grbit = 0x83b5;
    FLookupFleet(sel.fl.id, 0x4972);
    FillFleetCompLB();
    DrawPlanShip(0x0, grbit);
    InvalidateRect(hwndMine, 0x0, 0x1);
    goto L_3d2c;

L_2eda:
    /* untranslated: branch LOWORD(lParam) != part[20:2](rghwndBtn) ? L_3247 : L_2ee9 */

L_2ee9:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_3247;
    else
        goto L_2f05;

L_2f05:
    vrgiflMerge = rgifl;
    vcflMerge = 0x0;
    ifl = 0x0;
    goto L_2f20;

L_2f1b:
    ifl = (ifl + 0x1);

L_2f20:
    if ((ifl >= cFleet))
        goto L_2fc4;
    else
        goto L_2f2c;

L_2f2c:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_2f5f;
    else
        goto L_2f57;

L_2f57:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_2fc4;
    else
        goto L_2f5f;

L_2f5f:
    if ((lpfl->iPlayer != idPlayer))
        goto L_2f1b;
    else
        goto L_2f6f;

L_2f6f:
    if ((lpfl->fDead != 0x0))
        goto L_2f1b;
    else
        goto L_2f87;

L_2f87:
    if ((lpfl->pt.x != sel.fl.pt.x))
        goto L_2f1b;
    else
        goto L_2fa2;

L_2fa2:
    if ((lpfl->pt.y != sel.fl.pt.y))
        goto L_2f1b;
    else
        goto L_2fab;

L_2fab:
    vcflMerge = (vcflMerge + 0x1);
    rgifl[vcflMerge] = ifl;

L_2fc4:
    lpfl = 0x0;
    lpProc = MakeProcInstance(MergeFleetsDlg, hInst);
    if ((DialogBox(hInst, IDD_MERGE_FLEETS, hwndFrame, &(lpProc)) == 0x0))
        goto L_3180;
    else
        goto L_300a;

L_300a:
    ifl = 0x0;
    goto L_30b7;

L_3013:
    if ((vrgiflMerge[ifl] == 0xffff))
        goto L_30b2;
    else
        goto L_3027;

L_3027:
    if ((LOWORD(rglpfl[vrgiflMerge[ifl]]) != sel.fl.id))
        goto L_3080;
    else
        goto L_3053;

L_3053:
    lpfl = rglpfl[vrgiflMerge[ifl]];

L_3080:
    vrgiflMerge[ifl] = LOWORD(rglpfl[vrgiflMerge[ifl]]);

L_30b2:
    ifl = (ifl + 0x1);

L_30b7:
    if ((ifl < vcflMerge))
        goto L_3013;
    else
        goto L_30c3;

L_30c3:
    ifl = 0x0;
    goto L_3137;

L_30cc:
    if ((vrgiflMerge[ifl] == 0xffff))
        goto L_3132;
    else
        goto L_30e0;

L_30e0:
    if ((LOWORD(lpfl) != 0x0))
        goto L_3115;
    else
        goto L_30ea;

L_30ea:
    if ((HIWORD(lpfl) != 0x0))
        goto L_3115;
    else
        goto L_30f4;

L_30f4:
    lpfl = LpflFromId(vrgiflMerge[ifl]);
    goto L_3132;

L_3115:
    if ((vrgiflMerge[ifl] != LOWORD(lpfl)))
        goto L_3143;
    else
        goto L_312c;

L_312c:

L_3132:
    ifl = (ifl + 0x1);

L_3137:
    if ((ifl < vcflMerge))
        goto L_30cc;
    else
        goto L_3143;

L_3143:
    if ((ifl == vcflMerge))
        goto L_3174;
    else
        goto L_314f;

L_314f:
    if ((LOWORD(lpfl) == sel.fl.id))
        goto L_3180;
    else
        goto L_315e;

L_315e:
    SelectAdjFleet(0x0, LOWORD(lpfl));

L_3174:
    lpfl = 0x0;

L_3180:
    FreeProcInstance(&(lpProc));
    if ((LOWORD(lpfl) != 0x0))
        goto L_31a2;
    else
        goto L_3195;

L_3195:
    if ((HIWORD(lpfl) == 0x0))
        goto L_3d2c;
    else
        goto L_319c;

L_319c:

L_31a2:
    FFleetMergeAll(0x4972);
    FillShipDD(sel.fl.id);
    grbit = 0x83b5;
    FLookupFleet(sel.fl.id, 0x4972);
    FillFleetCompLB();
    DrawPlanShip(0x0, grbit);
    InvalidateRect(hwndMine, 0x0, 0x1);
    if (((grbitScan & 0x80) == 0x0))
        goto L_321d;
    else
        goto L_3208;

L_3208:
    InvalidateRect(hwndScanner, 0x0, 0x1);

L_321d:
    vrgiflMerge = 0x0;
    vcflMerge = 0x0;
    if ((gd.fTutorial == 0x0))
        goto L_3d2c;
    else
        goto L_323c;

L_323c:
    AdvanceTutor();

L_3247:
    if ((LOWORD(lParam) != hwndRepCB))
        goto L_32be;
    else
        goto L_3256;

L_3256:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_32be;
    else
        goto L_3272;

L_3272:
    /* untranslated: part[32:2](sel) = ((part[32:2](sel) & 0xfdff) | ((loword(SendMessage(hwndRepCB, WM_USER, 0x0, 0x0)) & 0x1) << 0x9)) */
    FLookupFleet(0xffff, 0x4972);
    goto L_3d2c;

L_32be:
    if ((LOWORD(lParam) != LOWORD(rghwndOrderDD)))
        goto L_3770;
    else
        goto L_32cd;

L_32cd:
    goto L_3762;

L_32e4:
    lSel = SendMessage(LOWORD(rghwndOrderDD), CB_GETCURSEL, 0x0, 0x0);
    /* untranslated: branch LOWORD(lSel) == (part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) & 0xf) ? L_3d2c : L_3337 */

L_3337:
    if ((LOWORD(lSel) != 0x3))
        goto L_3349;
    else
        goto L_3340;

L_3340:
    if ((HIWORD(lSel) == 0x0))
        goto L_3370;
    else
        goto L_3349;

L_3349:
    /* untranslated: branch (part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) & 0xf) != 0x3 ? L_3385 : L_3370 */

L_3370:
    InvalidateRect(hwndMine, 0x0, 0x1);

L_3385:
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) = ((part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) & 0xfff0) | (LOWORD(lSel) & 0xf)) */
    /* untranslated: call fmemset(&part[8:0](sel.fl.lpplord->rgord[sel.iwpAct]), 0x0, 0xa) -> callresult(void *) */
    if ((LOWORD(lSel) != 0x7))
        goto L_346d;
    else
        goto L_342e;

L_342e:
    /* untranslated: part[10:2](sel.fl.lpplord->rgord[sel.iwpAct]) = 0x0 */
    /* untranslated: part[8:2](sel.fl.lpplord->rgord[sel.iwpAct]) = 0x0 */
    goto L_3730;

L_346d:
    if ((LOWORD(lSel) != 0x9))
        goto L_349c;
    else
        goto L_347b;

L_347b:
    /* untranslated: part[8:2](sel.fl.lpplord->rgord[sel.iwpAct]) = 0x0 */
    goto L_3730;

L_349c:
    if ((LOWORD(lSel) != 0x6))
        goto L_34cb;
    else
        goto L_34aa;

L_34aa:
    /* untranslated: part[8:2](sel.fl.lpplord->rgord[sel.iwpAct]) = 0x5 */
    goto L_3730;

L_34cb:
    if ((LOWORD(lSel) != 0x4))
        goto L_3730;
    else
        goto L_34d9;

L_34d9:
    /* untranslated: branch ((part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) >> 0x8) & 0xf) == 0x2 ? L_3730 : L_3505 */

L_3505:
    lpflBest = 0x0;
    ishPrimary = 0x0;
    ish = 0x1;
    goto L_354c;

L_3520:
    if ((sel.fl.rgcsh[ish] <= sel.fl.rgcsh[ishPrimary]))
        goto L_3547;
    else
        goto L_353f;

L_353f:
    ishPrimary = ish;

L_3547:
    ish = (ish + 0x1);

L_354c:
    if ((ish < 0x10))
        goto L_3520;
    else
        goto L_3556;

L_3556:
    ifl = 0x0;
    goto L_3564;

L_355f:
    ifl = (ifl + 0x1);

L_3564:
    if ((ifl >= cFleet))
        goto L_3696;
    else
        goto L_3570;

L_3570:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_35a3;
    else
        goto L_359b;

L_359b:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_3696;
    else
        goto L_35a3;

L_35a3:
    if ((lpfl->pt.x != sel.fl.lpplord->rgord[sel.iwpAct].pt.x))
        goto L_355f;
    else
        goto L_35cf;

L_35cf:
    if ((lpfl->pt.y != sel.fl.lpplord->rgord[sel.iwpAct].pt.y))
        goto L_355f;
    else
        goto L_35d8;

L_35d8:
    if ((lpfl->iPlayer != idPlayer))
        goto L_355f;
    else
        goto L_35e8;

L_35e8:
    if ((lpfl->fDead != 0x0))
        goto L_355f;
    else
        goto L_3600;

L_3600:
    if ((LOWORD(lpfl) == sel.fl.id))
        goto L_355f;
    else
        goto L_360f;

L_360f:
    if ((lpfl->rgcsh[ishPrimary] <= 0x0))
        goto L_3653;
    else
        goto L_362f;

L_362f:
    lpflBest = lpfl;
    if ((lpfl->cord == 0x1))
        goto L_3696;
    else
        goto L_364a;

L_364a:

L_3653:
    if ((LOWORD(lpflBest) != 0x0))
        goto L_3667;
    else
        goto L_365d;

L_365d:
    if ((HIWORD(lpflBest) == 0x0))
        goto L_3683;
    else
        goto L_3667;

L_3667:
    if ((lpflBest->cord <= 0x1))
        goto L_355f;
    else
        goto L_3675;

L_3675:
    if ((lpfl->cord != 0x1))
        goto L_355f;
    else
        goto L_3683;

L_3683:
    lpflBest = lpfl;

L_3696:
    if ((LOWORD(lpflBest) != 0x0))
        goto L_36aa;
    else
        goto L_36a0;

L_36a0:
    if ((HIWORD(lpflBest) == 0x0))
        goto L_3730;
    else
        goto L_36aa;

L_36aa:
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) = ((part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) & 0xf0ff) | 0x200) */
    sel.fl.lpplord->rgord[sel.iwpAct].id = LOWORD(lpflBest);
    FLookupFleet(0xffff, 0x4972);
    FillOrdersLB();

L_3730:
    FLookupFleet(0xffff, 0x4972);
    UpdateOrdersDDs(0x1);
    DrawPlanShip(0x0, 0x8100);

L_3762:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0xffff) != 0x1 ? L_3d2c : L_3767 */

L_3767:

L_3770:
    if ((LOWORD(lParam) != HIWORD(rghwndOrderDD)))
        goto L_3953;
    else
        goto L_377f;

L_377f:
    goto L_3945;

L_3796:
    lSel = SendMessage(HIWORD(rghwndOrderDD), CB_GETCURSEL, 0x0, 0x0);
    /* untranslated: branch (part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) & 0xf) != 0x7 ? L_3825 : L_37dc */

L_37dc:
    /* untranslated: part[10:2](sel.fl.lpplord->rgord[sel.iwpAct]) = LOWORD(lSel) */
    FLookupFleet(0xffff, 0x4972);
    UpdateOrdersDDs(0x1);
    goto L_3d2c;

L_3825:
    /* untranslated: branch (part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) & 0xf) != 0x9 ? L_3895 : L_384c */

L_384c:
    /* untranslated: part[8:2](sel.fl.lpplord->rgord[sel.iwpAct]) = LOWORD(lSel) */
    FLookupFleet(0xffff, 0x4972);
    UpdateOrdersDDs(0x1);
    goto L_3d2c;

L_3895:
    /* untranslated: branch (part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) & 0xf) != 0x1 ? L_38db : L_38bc */

L_38bc:
    UpdateOrdersDDs(0x2);
    DrawPlanShip(0x0, 0x100);
    goto L_3d2c;

L_38db:
    /* untranslated: part[8:2](sel.fl.lpplord->rgord[sel.iwpAct]) = LOWORD(lSel) */
    /* untranslated: part[10:2](sel.fl.lpplord->rgord[sel.iwpAct]) = LOWORD(lSel) */
    FLookupFleet(0xffff, 0x4972);

L_3945:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0xffff) != 0x1 ? L_3d2c : L_394a */

L_394a:

L_3953:
    /* untranslated: branch LOWORD(lParam) != part[4:2](rghwndOrderDD) ? L_3ad5 : L_3962 */

L_3962:
    goto L_3ac7;

L_3979:
    /* untranslated: lSel = SendMessage(part[4:2](rghwndOrderDD), CB_GETCURSEL, 0x0, 0x0) */
    lMin = SendMessage(HIWORD(rghwndOrderDD), CB_GETCURSEL, 0x0, 0x0);
    if ((LOWORD(lMin) != 0x0))
        goto L_39dc;
    else
        goto L_39c3;

L_39c3:
    if ((HIWORD(lMin) != 0x0))
        goto L_39dc;
    else
        goto L_39cd;

L_39cd:
    lMin = 0x4;
    goto L_39e6;

L_39dc:
    lMin = (lMin - 0x1);

L_39e6:
    /* untranslated: HIWORD(sel.fl.lpplord):[((((LOWORD(sel.fl.lpplord) + 0x4) + loword((0x12 * sel.iwpAct))) + 0x8) + loword((int32_t)(lMin * 0x2)))] =
     * ((HIWORD(sel.fl.lpplord):[((((LOWORD(sel.fl.lpplord) + 0x4) + loword((0x12 * sel.iwpAct))) + 0x8) + loword((int32_t)(lMin * 0x2)))] & 0xfff) |
     * ((LOWORD(lSel) & 0xf) * 0x1000)) */
    FLookupFleet(0xffff, 0x4972);
    UpdateOrdersDDs(0x3);
    DrawPlanShip(0x0, 0x100);
    goto L_3d2c;

L_3ac7:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0xffff) != 0x1 ? L_3d2c : L_3acc */

L_3acc:

L_3ad5:
    if ((LOWORD(lParam) != hwndOrderED))
        goto L_3d2c;
    else
        goto L_3ae4;

L_3ae4:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x300))
        goto L_3d2c;
    else
        goto L_3b00;

L_3b00:
    /* untranslated: lSel = SendMessage(part[4:2](rghwndOrderDD), CB_GETCURSEL, 0x0, 0x0) */
    goto L_3b33;

L_3b28:
    fPercent = 0x1;
    goto L_3b56;

L_3b33:
    if ((LOWORD(lSel) != 0x5))
        goto L_3b43;
    else
        goto L_3b3b;

L_3b3b:
    if ((HIWORD(lSel) == 0x0))
        goto L_3b28;
    else
        goto L_3b43;

L_3b43:
    if ((LOWORD(lSel) != 0x6))
        goto L_3b56;
    else
        goto L_3b4b;

L_3b4b:
    if ((HIWORD(lSel) == 0x0))
        goto L_3b28;
    else
        goto L_3b56;

L_3b56:
    GetWindowText(hwndOrderED, &(rgb), 0x8);
    iInit = atoi(rgb);
    if ((0x0 <= iInit))
        goto L_3b8e;
    else
        goto L_3b88;

L_3b88:
    t_merge_3b92_0001 = 0x0;
    goto L_3b92;

L_3b8e:
    t_merge_3b92_0001 = iInit;

L_3b92:
    i = t_merge_3b92_0001;
    if ((fPercent == 0x0))
        goto L_3bbc;
    else
        goto L_3b9f;

L_3b9f:
    if ((0x64 >= i))
        goto L_3bb1;
    else
        goto L_3bab;

L_3bab:
    t_merge_3bb5_0001 = 0x64;
    goto L_3bb5;

L_3bb1:
    t_merge_3bb5_0001 = i;

L_3bb5:
    i = t_merge_3bb5_0001;
    goto L_3bd6;

L_3bbc:
    if ((0xfa0 >= i))
        goto L_3bce;
    else
        goto L_3bc8;

L_3bc8:
    t_merge_3bd2_0001 = 0xfa0;
    goto L_3bd2;

L_3bce:
    t_merge_3bd2_0001 = i;

L_3bd2:
    i = t_merge_3bd2_0001;

L_3bd6:
    if ((iInit == i))
        goto L_3c2e;
    else
        goto L_3be3;

L_3be3:
    AlertSz(PszFormatIds(idsAmountCargoMaySpecifyHereMustBetween, 0x0), MB_ICONHAND);
    _wsprintf(szWork, PCTD, i);
    SetWindowText(hwndOrderED, szWork);

L_3c2e:
    lMin = SendMessage(HIWORD(rghwndOrderDD), CB_GETCURSEL, 0x0, 0x0);
    if ((LOWORD(lMin) != 0x0))
        goto L_3c72;
    else
        goto L_3c59;

L_3c59:
    if ((HIWORD(lMin) != 0x0))
        goto L_3c72;
    else
        goto L_3c63;

L_3c63:
    lMin = 0x4;
    goto L_3c7c;

L_3c72:
    lMin = (lMin - 0x1);

L_3c7c:
    /* untranslated: HIWORD(sel.fl.lpplord):[((((LOWORD(sel.fl.lpplord) + 0x4) + loword((0x12 * sel.iwpAct))) + 0x8) + loword((int32_t)(lMin * 0x2)))] =
     * ((HIWORD(sel.fl.lpplord):[((((LOWORD(sel.fl.lpplord) + 0x4) + loword((0x12 * sel.iwpAct))) + 0x8) + loword((int32_t)(lMin * 0x2)))] & 0xf000) | (i &
     * 0xfff)) */
    FLookupFleet(0xffff, 0x4972);

L_3d2c:
    return;
}

void SelectAdjFleet(int16_t dInc, int16_t idFleet) {
    POINT   pt;
    int16_t idOld;
    int16_t i;
    FLEET  *lpfl;
    int16_t idNew;
    FLEET  *lpflT;
    SCAN    scan;

L_3d32:
    idOld = 0xffff;
    if ((cFleet <= 0x0))
        goto L_4008;
    else
        goto L_3d47;

L_3d47:

L_3d4d:
    if ((dInc == 0x0))
        goto L_3d5c;
    else
        goto L_3d56;

L_3d56:
    idFleet = sel.fl.id;

L_3d5c:
    if ((vrptFleet.fCached != 0x0))
        goto L_3d76;
    else
        goto L_3d66;

L_3d66:
    InvalidateReport(0x1, 0x1);

L_3d76:
    i = 0x0;
    goto L_3d82;

L_3d7e:
    i = (i + 0x1);

L_3d82:
    if ((i >= cFleet))
        goto L_3dd1;
    else
        goto L_3d8d;

L_3d8d:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_3dbd;
    else
        goto L_3db5;

L_3db5:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_3dd1;
    else
        goto L_3dbd;

L_3dbd:
    if ((LOWORD(lpfl) == idFleet))
        goto L_3dd1;
    else
        goto L_3dc8;

L_3dc8:

L_3dd1:
    if ((i == cFleet))
        goto L_3deb;
    else
        goto L_3ddc;

L_3ddc:
    if ((lpfl->iPlayer == idPlayer))
        goto L_3e28;
    else
        goto L_3deb;

L_3deb:
    if ((i == cFleet))
        goto L_4008;
    else
        goto L_3df3;

L_3df3:

L_3df9:
    pt.x = lpfl->pt.x;
    pt.y = lpfl->pt.y;
    scan.pt.x = lpfl->pt.x;
    scan.pt.y = lpfl->pt.y;
    scan.grobj = 0x83;
    ChangeScanSel(&(scan), 0x0);
    goto FinishUp;

L_3e28:
    if ((dInc == 0x0))
        goto L_3ef2;
    else
        goto L_3e31;

L_3e31:
    i = 0x0;
    goto L_3e3d;

L_3e39:
    i = (i + 0x1);

L_3e3d:
    /* untranslated: branch i >= (part[4:2](rgplr[idPlayer]) & 0xfff) ? L_3e8f : L_3e57 */

L_3e57:
    if ((LOWORD(rglpfl[vlprgidFleet[i]]) == idFleet))
        goto L_3e8f;
    else
        goto L_3e86;

L_3e86:

L_3e8f:
    i = (i + dInc);
    /* untranslated: branch i < (part[4:2](rgplr[idPlayer]) & 0xfff) ? L_3eb7 : L_3eaf */

L_3eaf:
    i = 0x0;
    goto L_3ed8;

L_3eb7:
    if ((i >= 0x0))
        goto L_3ed8;
    else
        goto L_3ec0;

L_3ec0:
    /* untranslated: i = ((part[4:2](rgplr[idPlayer]) & 0xfff) + 0xffff) */

L_3ed8:
    i = vlprgidFleet[i];
    goto L_3f25;

L_3ef2:
    if ((sel.grobj != grobjFleet))
        goto L_3f25;
    else
        goto L_3efc;

L_3efc:
    if ((sel.fl.pt.x != lpfl->pt.x))
        goto L_3f25;
    else
        goto L_3f16;

L_3f16:
    if ((sel.fl.pt.y != lpfl->pt.y))
        goto L_3f25;
    else
        goto L_3f1f;

L_3f1f:
    idOld = sel.fl.id;

L_3f25:
    lpflT = rglpfl[i];
    idNew = LOWORD(lpflT);
    pt.x = lpflT->pt.x;
    pt.y = lpflT->pt.y;
    scan.pt.x = lpflT->pt.x;
    scan.pt.y = lpflT->pt.y;
    scan.grobj = 0x82;
    ChangeScanSel(&(scan), 0x0);
    RedrawScanSel(0x0, 0x0);
    ChangeMainObjSel(grobjFleet, idNew);
    RedrawScanSel(0x0, 0x1);

FinishUp:
    /* untranslated: call CtrPointScan(words(pt.y, pt.x), 0x1) -> callresult(void) */
    DrawScannerSBar(0x0, 0x0, 0x0, 0x0);
    InvalidateRect(hwndMine, 0x0, 0x1);
    SetMineralTitleBar(hwndMine);
    if ((idOld == 0xffff))
        goto L_4008;
    else
        goto L_3ffd;

L_3ffd:
    SetFleetDropDownSel(idOld);

L_4008:
    return;
}

void SetFleetDropDownSel(int16_t id) {
    int16_t idSkip;
    int16_t i;
    FLEET  *lpfl;
    int16_t iOffset;
    int16_t t_merge_402f_0001;

L_400e:
    iOffset = 0x0;
    if ((sel.grobj != grobjFleet))
        goto L_402c;
    else
        goto L_4026;

L_4026:
    t_merge_402f_0001 = sel.fl.id;
    goto L_402f;

L_402c:
    t_merge_402f_0001 = 0xffff;

L_402f:
    idSkip = t_merge_402f_0001;
    i = 0x0;
    goto L_403e;

L_403a:
    i = (i + 0x1);

L_403e:
    if ((i >= cFleet))
        goto L_40c2;
    else
        goto L_4049;

L_4049:
    if ((LOWORD(rglpfl[i]) == id))
        goto L_40c2;
    else
        goto L_406a;

L_406a:
    lpfl = rglpfl[i];
    if ((sel.pt.x != lpfl->pt.x))
        goto L_403a;
    else
        goto L_40a4;

L_40a4:
    if ((sel.pt.y != lpfl->pt.y))
        goto L_403a;
    else
        goto L_40ad;

L_40ad:
    if ((LOWORD(lpfl) == idSkip))
        goto L_403a;
    else
        goto L_40bb;

L_40bb:
    iOffset = (iOffset + 0x1);

L_40c2:
    SendMessage(hwndShipDD, CB_SETCURSEL, iOffset, 0x0);
    DrawPlanShip(0x0, 0x4004);
    return;
}

int32_t LGetFleetStat(FLEET *lpfl, int16_t grStat) {
    int16_t i;
    int32_t l;

L_40f2:
    l = 0x0;
    if ((lpfl->det == 0x7))
        goto L_4120;
    else
        goto L_4117;

L_4117:
    return 0x7d000000;

L_4120:
    i = 0x0;
    goto L_4199;

L_4128:
    if ((lpfl->rgcsh[i] == 0x0))
        goto L_4195;
    else
        goto L_4145;

L_4145:
    l = (l + (uint32_t)(((uint32_t)(lpfl->rgcsh[i]) * (uint32_t)(WtMaxShdefStat(&(rglpshdef[lpfl->iPlayer][i]), grStat)))));

L_4195:
    i = (i + 0x1);

L_4199:
    if ((i < 0x10))
        goto L_4128;
    else
        goto L_41a2;

L_41a2:
    return l;
}

int16_t WtMaxShdefStat(SHDEF *lpshdef, int16_t grStat) {
    int16_t wt;
    int16_t j;
    HUL    *lphul;

L_41b2:
    lphul = lpshdef;
    goto L_4496;

L_41cd:
    wt = LphuldefFromId(lphul->ihuldef)->hul.wtFuelMax;
    j = 0x0;
    goto L_41f2;

L_41ee:
    j = (j + 0x1);

L_41f2:
    if ((j >= lphul->chs))
        goto L_44a9;
    else
        goto L_4204;

L_4204:
    if ((lphul->rghs[j].grhst != hstSpecialM))
        goto L_42c8;
    else
        goto L_4224;

L_4224:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) != 0x5 ? L_4276 : L_4249 */

L_4249:
    /* untranslated: wt = (wt + loword((((lphul->rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) * 0xfa))) */
    goto L_41ee;

L_4276:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) != 0x6 ? L_41ee : L_429b */

L_429b:
    /* untranslated: wt = (wt + loword((((lphul->rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) * 0x1f4))) */

L_42c8:
    if ((lphul->rghs[j].grhst != hstSpecialE))
        goto L_41ee;
    else
        goto L_42e8;

L_42e8:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) != 0x10 ? L_41ee : L_430d */

L_430d:
    /* untranslated: wt = (wt + loword((((lphul->rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) * 0xc8))) */

L_433d:
    wt = LphuldefFromId(lphul->ihuldef)->hul.wtCargoMax;
    j = 0x0;
    goto L_4362;

L_435e:
    j = (j + 0x1);

L_4362:
    if ((j >= lphul->chs))
        goto L_44a9;
    else
        goto L_4374;

L_4374:
    if ((lphul->rghs[j].grhst != hstSpecialM))
        goto L_435e;
    else
        goto L_4394;

L_4394:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) != 0x2 ? L_43e6 : L_43b9 */

L_43b9:
    /* untranslated: wt = (wt + loword((((lphul->rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) * 0x32))) */
    goto L_435e;

L_43e6:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) != 0x3 ? L_4438 : L_440b */

L_440b:
    /* untranslated: wt = (wt + loword((((lphul->rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) * 0x64))) */
    goto L_435e;

L_4438:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) != 0x4 ? L_435e : L_445d */

L_445d:
    /* untranslated: wt = (wt + loword((((lphul->rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) * 0xfa))) */

L_448d:
    return 0x0;

L_4496:
    if ((grStat == 0x1))
        goto L_41cd;
    else
        goto L_449e;

L_449e:
    if ((grStat != 0x2))
        goto L_448d;
    else
        goto L_44a3;

L_44a3:

L_44a9:
    return wt;
}

void DrawFleetGauge(HDC hdc, RECT *prc, FLEET *lpfl, int16_t grbit) {
    uint16_t rghbr[5];
    int32_t  lMax;
    int16_t  c;
    int16_t  i;
    int32_t  rgSize[5];
    int16_t  iMode;
    int16_t  cSections;
    int32_t  l;

L_44b6:
    if ((LOWORD(lpfl) != 0x0))
        goto L_44dc;
    else
        goto L_44c8;

L_44c8:
    if ((HIWORD(lpfl) != 0x0))
        goto L_44dc;
    else
        goto L_44d1;

L_44d1:
    /* untranslated: lpfl = &part[28:0](sel) */

L_44dc:
    SelectObject(hdc, HIWORD(rghfontArial8));
    cSections = 0x1;
    lMax = LGetFleetStat(lpfl, 0x2);
    if ((grbit < 0x0))
        goto L_456a;
    else
        goto L_450e;

L_450e:
    if ((grbit > 0x4))
        goto L_456a;
    else
        goto L_4517;

L_4517:
    LOWORD(rghbr) = rghbrMineral[grbit];
    LOWORD(rgSize) = LOWORD(lpfl->rgwtMin[grbit]);
    HIWORD(rgSize) = HIWORD(lpfl->rgwtMin[grbit]);
    if ((grbit != 0x4))
        goto L_46f7;
    else
        goto L_454f;

L_454f:
    lMax = LGetFleetStat(lpfl, 0x1);

L_456a:
    if ((grbit != 0x5))
        goto L_45d3;
    else
        goto L_4573;

L_4573:
    i = 0x0;
    goto L_45c2;

L_457b:
    rghbr[i] = rghbrMineral[i];
    rgSize[i] = lpfl->rgwtMin[i];
    i = (i + 0x1);

L_45c2:
    if ((i <= 0x3))
        goto L_457b;
    else
        goto L_45cb;

L_45cb:
    cSections = 0x4;
    goto L_46f7;

L_45d3:
    if ((grbit != 0x6))
        goto L_466c;
    else
        goto L_45dc;

L_45dc:
    lMax = 0xb;
    /* untranslated: LOWORD(rgSize) = ((part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) >> 0x4) & 0xf) */
    HIWORD(rgSize) = 0x0;
    if ((HIWORD(rgSize) > 0x0))
        goto L_465a;
    else
        goto L_461f;

L_461f:
    if ((HIWORD(rgSize) < 0x0))
        goto L_462d;
    else
        goto L_4624;

L_4624:
    if ((LOWORD(rgSize) > 0xa))
        goto L_465a;
    else
        goto L_462d;

L_462d:
    if ((LOWORD(rgSize) != 0xa))
        goto L_4663;
    else
        goto L_4636;

L_4636:
    if ((HIWORD(rgSize) != 0x0))
        goto L_4663;
    else
        goto L_463f;

L_463f:
    if ((IFindIdealWarp(&(sel.fl), 0x0) >= 0xa))
        goto L_4663;
    else
        goto L_465a;

L_465a:
    /* untranslated: LOWORD(rghbr) = part[4:2](rghbrMineral) */
    goto L_46f7;

L_4663:
    /* untranslated: LOWORD(rghbr) = part[8:2](rghbrMineral) */

L_466c:
    if ((grbit != 0x7))
        goto L_46f7;
    else
        goto L_4675;

L_4675:
    lMax = 0xa;
    /* untranslated: LOWORD(rgSize) = part[8:2](sel.fl.lpplord->rgord[sel.iwpAct]) */
    HIWORD(rgSize) = 0x0;
    if ((HIWORD(rgSize) > 0x0))
        goto L_46e8;
    else
        goto L_46ad;

L_46ad:
    if ((HIWORD(rgSize) < 0x0))
        goto L_46bb;
    else
        goto L_46b2;

L_46b2:
    if ((LOWORD(rgSize) > 0xa))
        goto L_46e8;
    else
        goto L_46bb;

L_46bb:
    if ((LOWORD(rgSize) != 0xa))
        goto L_46f1;
    else
        goto L_46c4;

L_46c4:
    if ((HIWORD(rgSize) != 0x0))
        goto L_46f1;
    else
        goto L_46cd;

L_46cd:
    if ((IFindIdealWarp(&(sel.fl), 0x0) >= 0xa))
        goto L_46f1;
    else
        goto L_46e8;

L_46e8:
    /* untranslated: LOWORD(rghbr) = part[4:2](rghbrMineral) */
    goto L_46f7;

L_46f1:
    /* untranslated: LOWORD(rghbr) = part[8:2](rghbrMineral) */

L_46f7:
    l = LDrawGauge(hdc, prc, cSections, rgSize, &(LOWORD(rghbr)), lMax);
    iMode = SetBkMode(hdc, TRANSPARENT);
    if ((grbit != 0x6))
        goto L_47b4;
    else
        goto L_4734;

L_4734:
    if ((LOWORD(l) != 0x0))
        goto L_4746;
    else
        goto L_473d;

L_473d:
    if ((HIWORD(l) == 0x0))
        goto L_479e;
    else
        goto L_4746;

L_4746:
    if ((HIWORD(l) > 0x0))
        goto L_4788;
    else
        goto L_474f;

L_474f:
    if ((HIWORD(l) < 0x0))
        goto L_475d;
    else
        goto L_4754;

L_4754:
    if ((LOWORD(l) >= 0xb))
        goto L_4788;
    else
        goto L_475d;

L_475d:
    c = _wsprintf(szWork, PszGetCompressedString(idsWarpLd), LOWORD(l), HIWORD(l));
    goto L_48b5;

L_4788:
    c = CchGetString(idsUseStargate, 0x57a4);

L_479e:
    c = CchGetString(idsStopped, 0x57a4);

L_47b4:
    if ((grbit != 0x7))
        goto L_4810;
    else
        goto L_47bd;

L_47bd:
    if ((LOWORD(l) != 0x0))
        goto L_47cf;
    else
        goto L_47c6;

L_47c6:
    if ((HIWORD(l) == 0x0))
        goto L_47fa;
    else
        goto L_47cf;

L_47cf:
    c = _wsprintf(szWork, PszGetCompressedString(idsWarpLd), LOWORD(l), HIWORD(l));
    goto L_48b5;

L_47fa:
    c = CchGetString(idsAutomatic, 0x57a4);

L_4810:
    if ((cSections != 0x1))
        goto L_484d;
    else
        goto L_4819;

L_4819:
    if ((grbit == 0x4))
        goto L_484d;
    else
        goto L_4822;

L_4822:
    c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), LOWORD(l), HIWORD(l));
    goto L_48b5;

L_484d:
    if ((grbit != 0x4))
        goto L_4887;
    else
        goto L_4856;

L_4856:
    c = _wsprintf(szWork, PszGetCompressedString(idsLdLdmg), LOWORD(l), HIWORD(l), LOWORD(lMax), HIWORD(lMax));
    goto L_48b5;

L_4887:
    c = _wsprintf(szWork, PszGetCompressedString(idsLdLdkt), LOWORD(l), HIWORD(l), LOWORD(lMax), HIWORD(lMax));

L_48b5:
    l = GetTextExtent(hdc, szWork, c);
    if ((LOWORD(l) >= ((prc->right - prc->left) + 0xfffd)))
        goto L_48fd;
    else
        goto L_48e8;

L_48e8:
    RcCtrTextOut(hdc, prc, 0x57a4, c);

L_48fd:
    SetBkMode(hdc, iMode);
    return;
}

void DrawFleetBitmap(FLEET *lpfl, HDC hdc, int16_t x, int16_t y, int16_t fFrame, int16_t ibmp, int16_t cDiff, int16_t fShrink, int16_t ibmpRace, int16_t csh) {
    int16_t  dxyPlus;
    int16_t  yCur;
    int16_t  c;
    int16_t  i;
    int16_t  dxy;
    int16_t  dx;
    int16_t  xCur;
    int16_t  dxyPlusWidth;
    HBRUSH   hbrSav;
    uint16_t t_merge_4c7b_0001;
    uint16_t t_merge_4ca0_0001;
    uint16_t t_merge_4cc6_0001;

L_490e:
    if ((fFrame == 0x0))
        goto L_4a22;
    else
        goto L_4920;

L_4920:
    hbrSav = SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, x, y, 0x44, 0x2, PATCOPY);
    PatBlt(hdc, x, y, 0x2, 0x44, PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, (x + 0x2), (y + 0x42), 0x42, 0x1, PATCOPY);
    PatBlt(hdc, (x + 0x3), (y + 0x42), 0x41, 0x1, PATCOPY);
    PatBlt(hdc, (x + 0x42), (y + 0x2), 0x1, 0x40, PATCOPY);
    PatBlt(hdc, (x + 0x43), (y + 0x1), 0x1, 0x41, PATCOPY);
    SelectObject(hdc, hbrSav);
    x = (x + 0x2);
    y = (y + 0x2);

L_4a22:
    if ((ibmp >= 0x0))
        goto L_4a64;
    else
        goto L_4a2b;

L_4a2b:
    i = IshdefPrimaryFromLpfl(lpfl, &(cDiff));
    ibmp = rglpshdef[lpfl->iPlayer][i].hul.ibmp;

L_4a64:
    ibmp = ((uint32_t)(ibmp) % 0x94);
    SelectPalette(hdc, vhpal, 0x0);
    RealizePalette(hdc);
    if ((fShrink == 0x0))
        goto L_4b69;
    else
        goto L_4a91;

L_4a91:
    DibBlt(hdc, x, y, 0x20, 0x20, rghdibShipsT[(ibmp >> 0x5)], (((ibmp & 0x1f) >> 0x2) * 0x20), ((0x3 - (ibmp & 0x3)) * 0x20), 0x20, 0x20, 0xcc0020);
    dxy = 0x20;
    dxyPlus = 0x5;
    dxyPlusWidth = 0x1;
    if ((ibmpRace < 0x0))
        goto L_4c38;
    else
        goto L_4b10;

L_4b10:
    DibBlt(hdc, x, (y + 0x18), 0x8, 0x8, hdibRacesX, ((ibmpRace & 0x7) * 0x8), ((0x3 - ((ibmpRace & 0x1f) >> 0x3)) * 0x8), 0x8, 0x8, 0xcc0020);

L_4b69:
    DibBlt(hdc, x, y, 0x40, 0x40, rghdibShips[(ibmp >> 0x5)], (((ibmp & 0x1f) >> 0x2) * 0x40), ((0x3 - (ibmp & 0x3)) * 0x40), 0x40, 0x40, 0xcc0020);
    dxy = 0x40;
    dxyPlus = 0x8;
    dxyPlusWidth = 0x2;
    if ((ibmpRace < 0x0))
        goto L_4c38;
    else
        goto L_4bde;

L_4bde:
    DibBlt(hdc, x, (y + 0x30), 0x10, 0x10, hdibRacesT, ((ibmpRace & 0x7) * 0x10), ((0x3 - ((ibmpRace & 0x1f) >> 0x3)) * 0x10), 0x10, 0x10, 0xcc0020);

L_4c38:
    if ((csh == 0x0))
        goto L_4c4a;
    else
        goto L_4c41;

L_4c41:
    if ((fShrink == 0x0))
        goto L_4d2d;
    else
        goto L_4c4a;

L_4c4a:
    if ((cDiff <= 0x4))
        goto L_4c58;
    else
        goto L_4c53;

L_4c53:
    cDiff = 0x4;

L_4c58:
    cDiff = (cDiff - 0x1);
    i = 0x0;
    goto L_4d1f;

L_4c64:
    if (((i & 0x2) != 0x2))
        goto L_4c78;
    else
        goto L_4c72;

L_4c72:
    t_merge_4c7b_0001 = 0x1;
    goto L_4c7b;

L_4c78:
    t_merge_4c7b_0001 = 0x0;

L_4c7b:
    if ((((i & 0x1) ^ t_merge_4c7b_0001) != 0x0))
        goto L_4c94;
    else
        goto L_4c8b;

L_4c8b:
    t_merge_4ca0_0001 = (x + 0x2);
    goto L_4ca0;

L_4c94:
    t_merge_4ca0_0001 = (((x + dxy) + 0xfffe) - dxyPlus);

L_4ca0:
    xCur = t_merge_4ca0_0001;
    if (((i & 0x2) != 0x0))
        goto L_4cba;
    else
        goto L_4cb1;

L_4cb1:
    t_merge_4cc6_0001 = (y + 0x2);
    goto L_4cc6;

L_4cba:
    t_merge_4cc6_0001 = (((y + dxy) + 0xfffe) - dxyPlus);

L_4cc6:
    yCur = t_merge_4cc6_0001;
    PatBlt(hdc, xCur, (((uint32_t)((dxyPlus + 0xffff)) / 0x2) + yCur), dxyPlus, dxyPlusWidth, 0xff0062);
    PatBlt(hdc, (((uint32_t)((dxyPlus + 0xffff)) / 0x2) + xCur), yCur, dxyPlusWidth, dxyPlus, 0xff0062);
    i = (i + 0x1);

L_4d1f:
    if ((i >= cDiff))
        goto L_4dee;
    else
        goto L_4d27;

L_4d27:

L_4d2d:
    SelectObject(hdc, rghfontArial7);
    SetTextColor(hdc, 0xffffff);
    if ((cDiff <= 0x1))
        goto L_4d8e;
    else
        goto L_4d52;

L_4d52:
    c = _wsprintf(szWork, PCTD, cDiff);
    TextOut(hdc, (x + 0x1), (y + 0x1), szWork, c);

L_4d8e:
    if ((csh <= 0x1))
        goto L_4dee;
    else
        goto L_4d97;

L_4d97:
    c = _wsprintf(szWork, PCTD, csh);
    dx = LOWORD(GetTextExtent(hdc, szWork, c));
    TextOut(hdc, (((x + dxy) + 0xffff) - dx), (y + 0x1), szWork, c);

L_4dee:
    return;
}

int16_t FEnumCalcJettison(void *lprt, RecordType rt, int16_t cb, PLANET *lppl, int16_t iFleet) {
    POINT    pt;
    int16_t  i;
    int16_t  grbit;
    FLEET    fl;
    int16_t  j;
    RTXFERX *prtxferx;
    RTXFER  *prtxfer;

L_4df4:
    if ((rt == rtLogCargoXfer8))
        goto L_4e0f;
    else
        goto L_4e06;

L_4e06:
    if ((rt != rtLogCargoXfer16))
        goto L_4f9e;
    else
        goto L_4e0f;

L_4e0f:
    prtxfer = lprt;
    if ((prtxfer->grobj1 != 0x2))
        goto L_4e51;
    else
        goto L_4e33;

L_4e33:
    if ((((*(prtxfer + 0x4) >> 0x4) & 0xf) == 0x4))
        goto L_4e57;
    else
        goto L_4e51;

L_4e51:
    return 0x1;

L_4e57:
    if ((FLookupFleet(iFleet, &(fl)) != 0x0))
        goto L_4e75;
    else
        goto L_4e6f;

L_4e6f:
    return 0x1;

L_4e75:
    pt.x = fl.pt.x;
    pt.y = fl.pt.y;
    if ((FLookupFleet(prtxfer->id1, &(fl)) != 0x0))
        goto L_4ea4;
    else
        goto L_4e9e;

L_4e9e:
    return 0x1;

L_4ea4:
    if ((fl.pt.x != pt.x))
        goto L_4eba;
    else
        goto L_4eb2;

L_4eb2:
    if ((fl.pt.y == pt.y))
        goto L_4ec0;
    else
        goto L_4eba;

L_4eba:
    return 0x1;

L_4ec0:
    grbit = prtxfer->grbitItems;
    if ((rt != rtLogCargoXfer16))
        goto L_4ee5;
    else
        goto L_4ed7;

L_4ed7:
    prtxferx = lprt;

L_4ee5:
    j = 0x0;
    i = 0x0;
    goto L_4f04;

L_4ef4:
    i = (i + 0x1);
    grbit = (grbit >> 0x1);

L_4f04:
    if ((i >= 0x5))
        goto L_4f9e;
    else
        goto L_4f0d;

L_4f0d:
    if (((grbit & 0x1) == 0x0))
        goto L_4ef4;
    else
        goto L_4f1b;

L_4f1b:
    if ((rt != rtLogCargoXfer8))
        goto L_4f5e;
    else
        goto L_4f24;

L_4f24:
    lppl->rgwtMin[i] = (lppl->rgwtMin[i] - (uint32_t)((uint16_t)(prtxfer->rgcQuan[j])));
    goto L_4f96;

L_4f5e:
    lppl->rgwtMin[i] = (lppl->rgwtMin[i] - (uint32_t)(prtxferx->rgcQuan[j]));

L_4f96:
    j = (j + 0x1);

L_4f9e:
    return 0x1;
}

int16_t TransferStuff(int16_t id1, GrobjClass grobj1, int16_t id2, GrobjClass grobj2, int16_t mdXfer) {
    XFER xfer[2];
    int16_t (**lpProcXfer)();
    int16_t  rgValidHull[16];
    int32_t  lPopPrev;
    int16_t  iDelFleet;
    int16_t  i;
    FLEET   *lpfl;
    int16_t  fSuccess;
    int16_t  grbit;
    int16_t  j;
    BTN      rgbtn[32];
    POINT    pt;
    RECT     rc;
    uint16_t t_merge_5456_0001;

L_4faa:
    lPopPrev = 0xffffffff;
    LOWORD(xfer) = id1;
    HIWORD(xfer) = grobj1;
    /* untranslated: part[128:2](xfer) = id2 */
    /* untranslated: part[130:2](xfer) = grobj2 */
    pxfer = xfer;
    mdXferDlg = mdXfer;
    i = 0x0;
    goto L_515b;

L_4ff0:
    if ((xfer[i].grobj == grobjOther))
        goto L_508e;
    else
        goto L_5008;

L_5008:
    /* untranslated: branch FLookupObject(xfer[i].grobj, xfer[i].id, part[4:0](xfer[i])) != 0x0 ? L_5056 : L_5050 */

L_5050:
    return 0x0;

L_5056:
    if ((xfer[i].grobj != grobjPlanet))
        goto L_5156;
    else
        goto L_506e;

L_506e:
    /* untranslated: LOWORD(lPopPrev) = part[44:2](xfer[i]) */
    /* untranslated: HIWORD(lPopPrev) = part[46:2](xfer[i]) */

L_508e:
    if ((mdXfer != 0x1))
        goto L_50d8;
    else
        goto L_5097;

L_5097:
    /* untranslated: lpfl = LpflNewSplit(&part[4:2](xfer)) */
    /* untranslated: part[132:124](xfer) = *lpfl */
    /* untranslated: part[128:2](xfer) = LOWORD(lpfl) */
    /* untranslated: part[130:2](xfer) = 0x2 */
    goto L_5156;

L_50d8:
    /* untranslated: part[4:2](xfer[i]) = 0xffff */
    j = 0x0;
    goto L_5121;

L_50f5:
    /* untranslated: 0x25:[(part[32:0](xfer[i]) + (j * 0x4))] = 0x0 */
    /* untranslated: 0x25:[(part[32:0](xfer[i]) + (j * 0x4))+0x2] = 0x0 */
    j = (j + 0x1);

L_5121:
    if ((j < 0x4))
        goto L_50f5;
    else
        goto L_512b;

L_512b:
    /* untranslated: call EnumLogRts(FEnumCalcJettison, &part[4:0](xfer[i]), id1) -> callresult(void) */

L_5156:
    i = (i + 0x1);

L_515b:
    if ((i < 0x2))
        goto L_4ff0;
    else
        goto L_5165;

L_5165:
    if ((mdXfer != 0x1))
        goto L_51d9;
    else
        goto L_516e;

L_516e:
    cXferValidHulls = 0x0;
    i = 0x0;
    goto L_51c8;

L_517d:
    /* untranslated: branch ss:[bp+(i * 0x2)-0xf2] != 0x0 ? L_51ad : L_5191 */

L_5191:
    /* untranslated: branch part[130:2](xfer) != 0x2 ? L_51c3 : L_519a */

L_519a:
    /* untranslated: branch ss:[bp+(i * 0x2)-0x72] == 0x0 ? L_51c3 : L_51ad */

L_51ad:
    cXferValidHulls = (cXferValidHulls + 0x1);
    rgValidHull[cXferValidHulls] = i;

L_51c3:
    i = (i + 0x1);

L_51c8:
    if ((i < 0x10))
        goto L_517d;
    else
        goto L_51d2;

L_51d2:
    rgXferValidHulls = rgValidHull;

L_51d9:
    rgbtnXfer = rgbtn;
    LOWORD(crgbtnXfer) = 0x20;
    if ((gd.fTutorial == 0x0))
        goto L_51fe;
    else
        goto L_51f9;

L_51f9:
    AdvanceTutor();

L_51fe:
    lpProcXfer = MakeProcInstance(TransferDlg, hInst);
    fSuccess = DialogBox(hInst, IDD_TRANSFER, hwndFrame, &(lpProcXfer));
    FreeProcInstance(&(lpProcXfer));
    if ((fSuccess == 0x0))
        goto L_5642;
    else
        goto L_524f;

L_524f:
    iDelFleet = 0xffff;
    if ((mdXfer != 0x1))
        goto L_52b3;
    else
        goto L_525e;

L_525e:
    i = 0x0;
    goto L_5282;

L_5267:
    /* untranslated: branch ss:[bp+(i * 0x2)-0x72] > 0x0 ? L_528c : L_5277 */

L_5277:

L_527d:
    i = (i + 0x1);

L_5282:
    if ((i < 0x10))
        goto L_5267;
    else
        goto L_528c;

L_528c:
    if ((i != 0x10))
        goto L_52a2;
    else
        goto L_5296;

L_5296:
    if ((grobj2 == grobjOther))
        goto CancelSplit;
    else
        goto L_529c;

L_529c:

L_52a2:
    /* untranslated: call FleetTransferCargoBalance(&part[4:2](xfer), &part[132:2](xfer)) -> callresult(void) */

L_52b3:
    i = 0x0;
    goto L_542f;

L_52bc:
    if ((xfer[i].grobj == grobjPlanet))
        goto L_52ec;
    else
        goto L_52d4;

L_52d4:
    if ((xfer[i].grobj != grobjOther))
        goto L_535b;
    else
        goto L_52ec;

L_52ec:
    /* untranslated: call FLookupPlanet(0xffff, part[4:0](xfer[i])) -> callresult(int16_t) */
    if ((xfer[i].grobj != grobjPlanet))
        goto L_542a;
    else
        goto L_5325;

L_5325:
    /* untranslated: branch LOWORD(lPopPrev) != part[44:2](xfer[i]) ? L_542a : L_5343 */

L_5343:
    /* untranslated: branch HIWORD(lPopPrev) != part[46:2](xfer[i]) ? L_542a : L_534c */

L_534c:
    lPopPrev = 0xffffffff;

L_535b:
    if ((xfer[i].grobj != grobjFleet))
        goto L_53f1;
    else
        goto L_5373;

L_5373:
    /* untranslated: call FLookupFleet(0xffff, part[4:0](xfer[i])) -> callresult(int16_t) */
    if ((mdXfer != 0x1))
        goto L_542a;
    else
        goto L_539d;

L_539d:
    j = 0x0;
    goto L_53d2;

L_53a6:
    /* untranslated: branch 0x25:[(part[16:0](xfer[i]) + (j * 0x2))] != 0x0 ? L_53dc : L_53c7 */

L_53c7:

L_53cd:
    j = (j + 0x1);

L_53d2:
    if ((j < 0x10))
        goto L_53a6;
    else
        goto L_53dc;

L_53dc:
    if ((j != 0x10))
        goto L_542a;
    else
        goto L_53e6;

L_53e6:
    iDelFleet = i;

L_53f1:
    if ((xfer[i].grobj != grobjThing))
        goto L_542a;
    else
        goto L_5409;

L_5409:
    /* untranslated: call FLookupThing(0xffff, part[4:0](xfer[i])) -> callresult(int16_t) */

L_542a:
    i = (i + 0x1);

L_542f:
    if ((i < 0x2))
        goto L_52bc;
    else
        goto L_5439;

L_5439:
    if ((iDelFleet == 0xffff))
        goto L_5482;
    else
        goto L_5443;

L_5443:
    if ((iDelFleet != 0x0))
        goto L_5453;
    else
        goto L_544d;

L_544d:
    t_merge_5456_0001 = 0x1;
    goto L_5456;

L_5453:
    t_merge_5456_0001 = 0x0;

L_5456:
    /* untranslated: call FDeleteFleet(part[4:2](xfer[iDelFleet]), grobjFleet, part[4:2](xfer[t_merge_5456_0001])) -> callresult(int16_t) */

L_5482:
    if ((mdXfer != 0x1))
        goto L_54ba;
    else
        goto L_548b;

L_548b:
    FillShipDD(sel.fl.id);
    if (((grbitScan & 0x80) == 0x0))
        goto L_54ba;
    else
        goto L_54a5;

L_54a5:
    InvalidateRect(hwndScanner, 0x0, 0x1);

L_54ba:
    if ((sel.grobj != grobjPlanet))
        goto L_54f9;
    else
        goto L_54c4;

L_54c4:
    grbit = 0x804d;
    FLookupPlanet(sel.pl.id, 0x49ee);
    FillPlanetProdLB(0x0, 0x0, 0x0);
    goto L_5514;

L_54f9:
    grbit = 0x83b5;
    FLookupFleet(sel.fl.id, 0x4972);
    FillFleetCompLB();

L_5514:
    DrawPlanShip(0x0, grbit);
    if ((sel.scan.grobj != grobjFleet))
        goto L_5546;
    else
        goto L_552e;

L_552e:
    InvalidateRect(hwndMine, 0x0, 0x1);
    goto L_554b;

L_5546:
    InvalidateMineralBars();

L_554b:
    if ((LOWORD(lPopPrev) != 0xffff))
        goto L_555f;
    else
        goto L_5555;

L_5555:
    if ((HIWORD(lPopPrev) == 0xffff))
        goto L_55a6;
    else
        goto L_555f;

L_555f:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_55a6;
    else
        goto L_5580;

L_5580:
    if (((grbitScan & 0x20) == 0x0))
        goto L_55a6;
    else
        goto L_558e;

L_558e:
    InvalidateRect(hwndScanner, 0x0, 0x0);
    goto L_5673;

L_55a6:
    if ((LOWORD(lPopPrev) != 0xffff))
        goto L_55ba;
    else
        goto L_55b0;

L_55b0:
    if ((HIWORD(lPopPrev) == 0xffff))
        goto L_562e;
    else
        goto L_55ba;

L_55ba:
    if (((grbitScan & 0xf) != 0x4))
        goto L_562e;
    else
        goto LInvalScanPlan;

LInvalScanPlan:
    pt.x = sel.pt.x;
    pt.y = sel.pt.y;
    LogicalToScan(&(pt));
    rc.right = pt.x;
    rc.bottom = pt.y;
    rc.left = pt.x;
    rc.top = pt.y;
    InflateRect(&(rc), 0x14, 0x14);
    rc.top = (rc.top - 0x14);
    InvalidateRect(hwndScanner, &(rc), 0x0);
    goto L_5673;

L_562e:
    if (((grbitScan & 0xf) == 0x1))
        goto LInvalScanPlan;
    else
        goto L_5639;

L_5639:

L_5642:
    if ((mdXfer != 0x1))
        goto L_5673;
    else
        goto L_564b;

L_564b:
    if ((grobj2 != grobjOther))
        goto L_5673;
    else
        goto CancelSplit;

CancelSplit:
    /* untranslated: call FDeleteFleet(part[132:2](xfer), grobjNone, 0x0) -> callresult(int16_t) */
    CancelMemRt(rtLogFleetSplit);

L_5673:
    mdXferDlg = 0xffff;
    return 0x0;
}

int16_t TransferDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    HDC         hdc;
    int16_t     dyMore;
    PAINTSTRUCT ps;
    POINT       pt;
    HWND        hwndBtn;
    RECT        rcBtn;
    int16_t     dx;
    RECT        rc;
    uint16_t    t_merge_596f_0001;
    uint16_t    t_merge_59c2_0001;

L_5686:
    goto L_59d5;

L_5695:
    StickyDlgPos(hwnd, 0x986, 0x1);
    GetClientRect(hwnd, &(rc));
    if ((mdXferDlg != 0x1))
        goto L_5869;
    else
        goto L_56c1;

L_56c1:
    SetWindowText(hwnd, PszGetCompressedString(idsShipTransfer));
    if ((cXferValidHulls <= 0xa))
        goto L_5869;
    else
        goto L_56e3;

L_56e3:
    dyMore = ((uint32_t)(rc.bottom) / 0x2);
    rc.bottom = (rc.bottom + dyMore);
    SetWindowPos(hwnd, 0x0, 0x0, 0x0, rc.right, rc.bottom, 0x6);
    GetClientRect(hwnd, &(rc));
    dyMore = (dyMore - (GetSystemMetrics(SM_CYCAPTION) + 0x2));
    dx = (GetSystemMetrics(SM_CXDLGFRAME) + 0x4);
    hwndBtn = GetDlgItem(hwnd, IDOK);
    GetWindowRect(hwndBtn, &(rcBtn));
    pt.x = rcBtn.left;
    pt.y = rcBtn.top;
    ScreenToClient(hwnd, &(pt));
    pt.y = (pt.y + dyMore);
    pt.x = (pt.x - dx);
    SetWindowPos(hwndBtn, 0x0, pt.x, pt.y, 0x0, 0x0, SWP_NOSIZE);
    hwndBtn = GetDlgItem(hwnd, IDC_HELP);
    GetWindowRect(hwndBtn, &(rcBtn));
    pt.x = rcBtn.left;
    pt.y = rcBtn.top;
    ScreenToClient(hwnd, &(pt));
    pt.y = (pt.y + dyMore);
    pt.x = (pt.x - dx);
    SetWindowPos(hwndBtn, 0x0, pt.x, pt.y, 0x0, 0x0, SWP_NOSIZE);
    hwndBtn = GetDlgItem(hwnd, IDCANCEL);
    GetWindowRect(hwndBtn, &(rcBtn));
    pt.x = rcBtn.left;
    pt.y = rcBtn.top;
    ScreenToClient(hwnd, &(pt));
    pt.y = (pt.y + dyMore);
    pt.x = (pt.x - dx);
    SetWindowPos(hwndBtn, 0x0, pt.x, pt.y, 0x0, 0x0, SWP_NOSIZE);

L_5869:
    FSetupXferBtns(&(rc));
    if ((gd.fTutorial == 0x0))
        goto L_588d;
    else
        goto L_5888;

L_5888:
    AdvanceTutor();

L_588d:
    return 0x1;

L_5893:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_58bb:
    hdc = BeginPaint(hwnd, &(ps));
    GetClientRect(hwnd, &(rc));
    DrawXferDlg(hwnd, hdc, &(rc), 0xffff);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_5907:
    return FTrackXfer(hwnd, LOWORD(lParam), (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), wParam);

L_5935:
    if ((wParam == 0x1))
        goto L_5947;
    else
        goto L_593e;

L_593e:
    if ((wParam != 0x2))
        goto L_5996;
    else
        goto L_5947;

L_5947:
    StickyDlgPos(hwnd, 0x986, 0x0);
    if ((wParam != 0x1))
        goto L_596c;
    else
        goto L_5966;

L_5966:
    t_merge_596f_0001 = 0x1;
    goto L_596f;

L_596c:
    t_merge_596f_0001 = 0x0;

L_596f:
    EndDialog(hwnd, t_merge_596f_0001);
    if ((gd.fTutorial == 0x0))
        goto L_598d;
    else
        goto L_5988;

L_5988:
    AdvanceTutor();

L_598d:
    return 0x1;

L_5996:
    if ((wParam != 0x76))
        goto L_5a08;
    else
        goto L_599f;

L_599f:
    if ((mdXferDlg != 0x1))
        goto L_59be;
    else
        goto L_59b7;

L_59b7:
    t_merge_59c2_0001 = 0x438;
    goto L_59c2;

L_59be:
    t_merge_59c2_0001 = 0x433;

L_59c2:
    /* untranslated: call WinHelp(hwnd, szHelpFile, 0x1, words(0x0, t_merge_59c2_0001)) -> callresult(int16_t) */
    return 0x1;

L_59d5:
    if ((message == WM_PAINT))
        goto L_58bb;
    else
        goto L_59dd;

L_59dd:
    if ((message == WM_ERASEBKGND))
        goto L_5893;
    else
        goto L_59e5;

L_59e5:
    if ((message == WM_INITDIALOG))
        goto L_5695;
    else
        goto L_59ed;

L_59ed:
    if ((message == WM_COMMAND))
        goto L_5935;
    else
        goto L_59f5;

L_59f5:
    if ((message == WM_LBUTTONDOWN))
        goto L_5907;
    else
        goto L_59fd;

L_59fd:
    if ((message == WM_LBUTTONDBLCLK))
        goto L_5907;
    else
        goto L_5a08;

L_5a08:
    return 0x0;
}

int16_t FTrackXfer(HWND hwnd, int16_t x, int16_t y, int16_t fkb) {
    POINT    ptOld;
    POINT    pt;
    int32_t  dChg;
    BTNT     btnt;
    int32_t  cCur;
    int16_t  i;
    int16_t  iBtn;
    int16_t  iVal;
    BTN      btn;
    int32_t  cNew;
    RECT     rc;
    int32_t  t_merge_5c20_0001_wide;
    uint32_t t_merge_5cf0_0001;
    uint16_t t_merge_5d7f_0001;
    uint16_t t_merge_5def_0001;
    uint16_t t_merge_5e4d_0001;
    uint16_t t_merge_5e34_0001;
    uint32_t t_merge_5ecd_0001_wide;
    uint32_t t_merge_5f3b_0001;

L_5a16:
    GetClientRect(hwnd, &(rc));
    pt.x = x;
    pt.y = y;
    i = 0x0;
    goto L_5a86;

L_5a42:
    if (((rgbtnXfer[i].bt & 0x4) != 0x0))
        goto L_5a82;
    else
        goto L_5a5c;

L_5a5c:
    /* untranslated: branch PtInRect((rgbtnXfer + loword((0xe * i))), words(pt.y, pt.x)) != 0x0 ? L_5a91 : L_5a7c */

L_5a7c:

L_5a82:
    i = (i + 0x1);

L_5a86:
    if ((i < LOWORD(crgbtnXfer)))
        goto L_5a42;
    else
        goto L_5a91;

L_5a91:
    if ((i != LOWORD(crgbtnXfer)))
        goto L_5aa2;
    else
        goto L_5a9c;

L_5a9c:
    return 0x0;

L_5aa2:
    iBtn = (i >> 0x1);
    btn = rgbtnXfer[i];
    iVal = (btn.iVal & 0x7f);
    if ((btn.fVisible != 0x0))
        goto L_5d2e;
    else
        goto L_5adc;

L_5adc:
    if ((iVal > 0x4))
        goto FinishUp;
    else
        goto L_5ae2;

L_5ae2:

L_5ae8:
    if ((*(pxfer + 0x82) != 0x8))
        goto L_5b0e;
    else
        goto L_5af6;

L_5af6:
    if ((iVal == 0x4))
        goto FinishUp;
    else
        goto L_5aff;

L_5aff:
    if ((iVal == 0x3))
        goto FinishUp;
    else
        goto L_5b05;

L_5b05:

L_5b0e:
    /* untranslated: branch part[6:2](pxfer[btn.iSide]) != idPlayer ? L_5f70 : L_5b2b */

L_5b2b:

L_5b31:
    SetCapture(hwnd);
    ptOld.y = 0xffff;
    ptOld.x = 0xffff;

L_5b44:
    if ((FGetMouseMove(&(pt)) == 0x0))
        goto L_5d26;
    else
        goto L_5b58;

L_5b58:
    if ((pt.x != ptOld.x))
        goto L_5b6e;
    else
        goto L_5b63;

L_5b63:
    if ((pt.y == ptOld.y))
        goto L_5b44;
    else
        goto L_5b6e;

L_5b6e:
    ptOld.x = pt.x;
    ptOld.y = pt.y;
    if ((btn.iSide != 0x1))
        goto L_5bc0;
    else
        goto L_5b8c;

L_5b8c:
    if ((*(pxfer + 0x82) != 0x8))
        goto L_5bc0;
    else
        goto L_5b9a;

L_5b9a:
    /* untranslated: cNew = (uint32_t)(words(0x0, (*(pxfer+0x92) & 0x3fff)) * 0xa) */
    goto L_5c26;

L_5bc0:
    if ((iVal != 0x4))
        goto L_5bf6;
    else
        goto L_5bc9;

L_5bc9:
    t_merge_5c20_0001_wide = LGetFleetStat(((pxfer + (btn.iSide * 0x80)) + 0x4), 0x1);
    goto L_5c20;

L_5bf6:
    t_merge_5c20_0001_wide = LGetFleetStat(((pxfer + (btn.iSide * 0x80)) + 0x4), 0x2);

L_5c20:
    cNew = t_merge_5c20_0001_wide;

L_5c26:
    cNew = (int32_t)(((uint32_t)(((uint32_t)((pt.x - btn.rc.left)) * cNew)) / (uint32_t)(((btn.rc.right - btn.rc.left) + 0xfffe))));
    cCur = ChgCargo(pxfer[btn.iSide].grobj, pxfer[btn.iSide].id, iVal, 0x0, ((pxfer + (btn.iSide * 0x80)) + 0x4));
    dChg = (cNew - cCur);
    if ((btn.iSide != 0x0))
        goto L_5ce3;
    else
        goto L_5cda;

L_5cda:
    t_merge_5cf0_0001 = dChg;
    goto L_5cf0;

L_5ce3:
    /* untranslated: t_merge_5cf0_0001 = words(((HIWORD(dChg) + 0x0) neg 0x0), (LOWORD(dChg) neg 0x0)) */

L_5cf0:
    if ((LOWORD(XferSupply(iVal, t_merge_5cf0_0001)) != 0x0))
        goto L_5d0d;
    else
        goto L_5d05;

L_5d05:
    /* untranslated: branch hiword(callresult(int32_t)) == 0x0 ? L_5b44 : L_5d0d */

L_5d0d:
    DrawXferDlg(hwnd, 0x0, &(rc), iVal);

L_5d26:
    ReleaseCapture();
    goto FinishUp;

L_5d2e:
    InitBtnTrack(&(btnt), hwnd, 0x0, &(btn.rc), btn.bt, 0x50, 0x0, 0x0, 0x0);
    if (((fkb & 0x8) == 0x0))
        goto L_5d88;
    else
        goto L_5d66;

L_5d66:
    if (((fkb & 0x4) == 0x0))
        goto L_5d7b;
    else
        goto L_5d74;

L_5d74:
    t_merge_5d7f_0001 = 0x3e8;
    goto L_5d7f;

L_5d7b:
    t_merge_5d7f_0001 = 0x64;

L_5d7f:
    LOWORD(dChg) = t_merge_5d7f_0001;
    HIWORD(dChg) = 0x0;
    goto L_5dad;

L_5d88:
    if (((fkb & 0x4) == 0x0))
        goto L_5da3;
    else
        goto L_5d96;

L_5d96:
    dChg = 0xa;
    goto L_5dad;

L_5da3:
    dChg = 0x1;

L_5dad:
    if ((FTrackBtn(&(btnt)) == 0x0))
        goto FinishUp;
    else
        goto L_5dc1;

L_5dc1:
    if ((mdXferDlg != 0x1))
        goto L_5f01;
    else
        goto L_5dcb;

L_5dcb:
    if ((btn.iSide != 0x0))
        goto L_5dec;
    else
        goto L_5de6;

L_5de6:
    t_merge_5def_0001 = 0x1;
    goto L_5def;

L_5dec:
    t_merge_5def_0001 = 0x0;

L_5def:
    /* untranslated: branch ss:[bp-0x4c] >= 0x25:[(((pxfer + (t_merge_5def_0001 * 0x80)) + 0x10) + (iVal * 0x2))] ? L_5e19 : L_5e10 */

L_5e10:
    t_merge_5e4d_0001 = LOWORD(dChg);
    goto L_5e4d;

L_5e19:
    if ((btn.iSide != 0x0))
        goto L_5e31;
    else
        goto L_5e2b;

L_5e2b:
    t_merge_5e34_0001 = 0x1;
    goto L_5e34;

L_5e31:
    t_merge_5e34_0001 = 0x0;

L_5e34:
    /* untranslated: t_merge_5e4d_0001 = 0x25:[(((pxfer + (t_merge_5e34_0001 * 0x80)) + 0x10) + (iVal * 0x2))] */

L_5e4d:
    i = t_merge_5e4d_0001;
    if ((i == 0x0))
        goto L_5dad;
    else
        goto L_5e59;

L_5e59:
    /* untranslated: branch 0x25:[(((pxfer + (btn.iSide * 0x80)) + 0x10) + (iVal * 0x2))] < (0x7ffe - i) ? L_5e8c : L_5e87 */

L_5e87:
    i = 0x1;

L_5e8c:
    /* untranslated: 0x25:[(((pxfer + (btn.iSide * 0x80)) + 0x10) + (iVal * 0x2))] = (0x25:[(((pxfer + (btn.iSide * 0x80)) + 0x10) + (iVal * 0x2))] + i) */
    if ((btn.iSide != 0x0))
        goto L_5eca;
    else
        goto L_5ec4;

L_5ec4:
    t_merge_5ecd_0001_wide = 0x10001;
    goto L_5ecd;

L_5eca:
    t_merge_5ecd_0001_wide = 0x0;

L_5ecd:
    /* untranslated: 0x25:[(((pxfer + (loword(t_merge_5ecd_0001_wide) * 0x80)) + 0x10) + (iVal * 0x2))] = (0x25:[(((pxfer + (hiword(t_merge_5ecd_0001_wide) *
     * 0x80)) + 0x10) + (iVal * 0x2))] - i) */
    DrawXferDlg(hwnd, btnt.hdc, &(rc), iBtn);

L_5f01:
    if ((iVal < 0x0))
        goto L_5dad;
    else
        goto L_5f0a;

L_5f0a:
    if ((iVal > 0x4))
        goto L_5dad;
    else
        goto L_5f13;

L_5f13:
    if ((btn.iSide != 0x0))
        goto L_5f2e;
    else
        goto L_5f25;

L_5f25:
    t_merge_5f3b_0001 = dChg;
    goto L_5f3b;

L_5f2e:
    /* untranslated: t_merge_5f3b_0001 = words(((HIWORD(dChg) + 0x0) neg 0x0), (LOWORD(dChg) neg 0x0)) */

L_5f3b:
    if ((LOWORD(XferSupply(iVal, t_merge_5f3b_0001)) != 0x0))
        goto L_5f58;
    else
        goto L_5f50;

L_5f50:
    /* untranslated: branch hiword(callresult(int32_t)) == 0x0 ? L_5dad : L_5f58 */

L_5f58:
    DrawXferDlg(hwnd, btnt.hdc, &(rc), iVal);

FinishUp:
    UpdateXferBtns();
    DrawXferDlg(hwnd, 0x0, &(rc), 0xfffe);
    return 0x1;
}

int32_t GetCargoFree(FLEET *lpfl) {
    int32_t cHave;
    int16_t i;

L_5f98:
    cHave = 0x0;
    i = 0x0;
    goto L_5fda;

L_5fb3:
    cHave = (cHave + lpfl->rgwtMin[i]);
    i = (i + 0x1);

L_5fda:
    if ((i <= 0x3))
        goto L_5fb3;
    else
        goto L_5fe3;

L_5fe3:
    LGetFleetStat(lpfl, 0x2);

L_5ffe:
    /* untranslated: return words((loword(callresult(int32_t)) - LOWORD(cHave)), (hiword(callresult(int32_t)) - HIWORD(cHave))) */
}

int32_t GetFuelFree(FLEET *lpfl) {
L_6004:
    LGetFleetStat(lpfl, 0x1);

L_602d:
    /* untranslated: return words((loword(callresult(int32_t)) - LOWORD(lpfl->rgwtMin[0x4])), (hiword(callresult(int32_t)) - HIWORD(lpfl->rgwtMin[0x4]))) */
}

int32_t ChgCargo(GrobjClass grobj, int16_t id, int16_t iSupply, int32_t dChg, void *pobj) {
    THING  *pth;
    XFER    xfer;
    int16_t i;
    FLEET  *pfl;
    PLANET *ppl;
    int32_t wtFree;
    int32_t t_merge_6425_0003_wide;
    int32_t t_merge_646f_0001_wide;

L_6034:
    if ((grobj == grobjPlanet))
        goto L_604f;
    else
        goto L_6046;

L_6046:
    if ((grobj != grobjOther))
        goto L_6190;
    else
        goto L_604f;

L_604f:
    if ((pobj == 0x0))
        goto L_6062;
    else
        goto L_6058;

L_6058:
    ppl = pobj;
    goto L_609f;

L_6062:
    if ((grobj != grobjPlanet))
        goto L_6084;
    else
        goto L_606b;

L_606b:
    FLookupPlanet(id, &(xfer.pl));
    /* untranslated: ppl = &part[4:2](xfer) */
    goto L_609f;

L_6084:
    /* untranslated: call memset(&part[4:2](xfer), 0x0, 0x38) -> callresult(void *) */
    /* untranslated: ppl = &part[4:2](xfer) */

L_609f:
    if ((iSupply > 0x4))
        goto L_6159;
    else
        goto L_60a8;

L_60a8:
    if ((iSupply != 0x4))
        goto L_60ba;
    else
        goto L_60b1;

L_60b1:
    return 0x0;

L_60ba:
    if ((LOWORD(dChg) != 0x0))
        goto L_60e6;
    else
        goto L_60c3;

L_60c3:
    if ((HIWORD(dChg) != 0x0))
        goto L_60e6;
    else
        goto L_60cc;

L_60cc:
    return ppl->rgwtMin[iSupply];

L_60e6:
    if (((HIWORD(ppl->rgwtMin[iSupply]) + HIWORD(dChg)) > 0x0))
        goto L_613c;
    else
        goto L_610b;

L_610b:
    if (((HIWORD(ppl->rgwtMin[iSupply]) + HIWORD(dChg)) < 0x0))
        goto L_6118;
    else
        goto L_6110;

L_6110:
    if (((LOWORD(ppl->rgwtMin[iSupply]) + LOWORD(dChg)) >= 0x0))
        goto L_613c;
    else
        goto L_6118;

L_6118:
    /* untranslated: LOWORD(dChg) = (LOWORD(ppl->rgwtMin[iSupply]) neg 0x0) */
    /* untranslated: HIWORD(dChg) = ((HIWORD(ppl->rgwtMin[iSupply]) + 0x0) neg 0x0) */

L_613c:
    ppl->rgwtMin[iSupply] = (ppl->rgwtMin[iSupply] + dChg);

L_6159:
    if ((LOWORD(dChg) != 0x0))
        goto L_616b;
    else
        goto L_6162;

L_6162:
    if ((HIWORD(dChg) == 0x0))
        goto L_64bd;
    else
        goto L_616b;

L_616b:
    if ((pobj != 0x0))
        goto L_64bd;
    else
        goto L_6174;

L_6174:
    if ((grobj == grobjOther))
        goto L_64bd;
    else
        goto L_617d;

L_617d:
    FLookupPlanet(0xffff, &(xfer.pl));

L_6190:
    if ((grobj != grobjThing))
        goto L_6320;
    else
        goto L_6199;

L_6199:
    if ((pobj == 0x0))
        goto L_61ab;
    else
        goto L_61a2;

L_61a2:
    pth = pobj;
    goto L_61c0;

L_61ab:
    FLookupThing(id, &(xfer.th));
    /* untranslated: pth = &part[4:2](xfer) */

L_61c0:
    if ((iSupply < 0x3))
        goto L_61d2;
    else
        goto L_61c9;

L_61c9:
    return 0x0;

L_61d2:
    if ((iSupply > 0x4))
        goto L_62f3;
    else
        goto L_61db;

L_61db:
    if ((LOWORD(dChg) != 0x0))
        goto L_6202;
    else
        goto L_61e4;

L_61e4:
    if ((HIWORD(dChg) != 0x0))
        goto L_6202;
    else
        goto L_61ed;

L_61ed:
    /* untranslated: return words(0x25:[((pth + 0x8) + (iSupply * 0x2))], signhiword(0x25:[((pth + 0x8) + (iSupply * 0x2))])) */

L_6202:
    /* untranslated: branch (signhiword(0x25:[((pth + 0x8) + (iSupply * 0x2))]) + HIWORD(dChg)) > 0x0 ? L_6249 : L_6222 */

L_6222:
    /* untranslated: branch (signhiword(0x25:[((pth + 0x8) + (iSupply * 0x2))]) + HIWORD(dChg)) < 0x0 ? L_622f : L_6227 */

L_6227:
    /* untranslated: branch (0x25:[((pth + 0x8) + (iSupply * 0x2))] + LOWORD(dChg)) >= 0x0 ? L_6249 : L_622f */

L_622f:
    /* untranslated: LOWORD(dChg) = (0x25:[((pth + 0x8) + (iSupply * 0x2))] neg 0x0) */
    /* untranslated: HIWORD(dChg) = signhiword((0x25:[((pth + 0x8) + (iSupply * 0x2))] neg 0x0)) */

L_6249:
    /* untranslated: wtFree = (uint32_t)(words(0x0, pth->thp.wtMax) * 0xa) */
    i = 0x0;
    goto L_6295;

L_6275:
    /* untranslated: LOWORD(wtFree) = (LOWORD(wtFree) - 0x25:[((pth + 0x8) + (i * 0x2))]) */
    /* untranslated: HIWORD(wtFree) = (HIWORD(wtFree) - signhiword(0x25:[((pth + 0x8) + (i * 0x2))])) */
    i = (i + 0x1);

L_6295:
    if ((i < 0x3))
        goto L_6275;
    else
        goto L_629f;

L_629f:
    if ((HIWORD(dChg) < HIWORD(wtFree)))
        goto L_62ca;
    else
        goto L_62af;

L_62af:
    if ((HIWORD(dChg) > HIWORD(wtFree)))
        goto L_62bc;
    else
        goto L_62b4;

L_62b4:
    if ((LOWORD(dChg) <= LOWORD(wtFree)))
        goto L_62ca;
    else
        goto L_62bc;

L_62bc:
    dChg = wtFree;

L_62ca:
    /* untranslated: 0x25:[((pth + 0x8) + (iSupply * 0x2))] = (0x25:[((pth + 0x8) + (iSupply * 0x2))] + LOWORD(dChg)) */

L_62f3:
    if ((LOWORD(dChg) != 0x0))
        goto L_6305;
    else
        goto L_62fc;

L_62fc:
    if ((HIWORD(dChg) == 0x0))
        goto L_64bd;
    else
        goto L_6305;

L_6305:
    if ((pobj != 0x0))
        goto L_64bd;
    else
        goto L_630e;

L_630e:
    FLookupThing(0xffff, pth);

L_6320:
    if ((pobj == 0x0))
        goto L_6333;
    else
        goto L_6329;

L_6329:
    pfl = pobj;
    goto L_6349;

L_6333:
    FLookupFleet(id, &(xfer.fl));
    /* untranslated: pfl = &part[4:2](xfer) */

L_6349:
    if ((iSupply > 0x4))
        goto L_6492;
    else
        goto L_6352;

L_6352:
    if ((LOWORD(dChg) != 0x0))
        goto L_637e;
    else
        goto L_635b;

L_635b:
    if ((HIWORD(dChg) != 0x0))
        goto L_637e;
    else
        goto L_6364;

L_6364:
    return pfl->rgwtMin[iSupply];

L_637e:
    if (((HIWORD(pfl->rgwtMin[iSupply]) + HIWORD(dChg)) > 0x0))
        goto L_63d4;
    else
        goto L_63a3;

L_63a3:
    if (((HIWORD(pfl->rgwtMin[iSupply]) + HIWORD(dChg)) < 0x0))
        goto L_63b0;
    else
        goto L_63a8;

L_63a8:
    if (((LOWORD(pfl->rgwtMin[iSupply]) + LOWORD(dChg)) >= 0x0))
        goto L_63d4;
    else
        goto L_63b0;

L_63b0:
    /* untranslated: LOWORD(dChg) = (LOWORD(pfl->rgwtMin[iSupply]) neg 0x0) */
    /* untranslated: HIWORD(dChg) = ((HIWORD(pfl->rgwtMin[iSupply]) + 0x0) neg 0x0) */

L_63d4:
    if ((iSupply != 0x3))
        goto L_63f9;
    else
        goto L_63dd;

L_63dd:
    if ((pfl->det == 0x7))
        goto L_63f9;
    else
        goto L_63ef;

L_63ef:
    dChg = 0x0;

L_63f9:
    if ((iSupply != 0x4))
        goto L_6415;
    else
        goto L_6402;

L_6402:
    t_merge_6425_0003_wide = GetFuelFree(pfl);
    goto L_6425;

L_6415:
    t_merge_6425_0003_wide = GetCargoFree(pfl);

L_6425:
    if ((HIWORD(dChg) > HIWORD(t_merge_6425_0003_wide)))
        goto L_6443;
    else
        goto L_642d;

L_642d:
    if ((HIWORD(dChg) < HIWORD(t_merge_6425_0003_wide)))
        goto L_643a;
    else
        goto L_6432;

L_6432:
    if ((LOWORD(dChg) >= LOWORD(t_merge_6425_0003_wide)))
        goto L_6443;
    else
        goto L_643a;

L_643a:
    t_merge_646f_0001_wide = dChg;
    goto L_646f;

L_6443:
    if ((iSupply != 0x4))
        goto L_645f;
    else
        goto L_644c;

L_644c:
    t_merge_646f_0001_wide = GetFuelFree(pfl);
    goto L_646f;

L_645f:
    t_merge_646f_0001_wide = GetCargoFree(pfl);

L_646f:
    dChg = t_merge_646f_0001_wide;
    pfl->rgwtMin[iSupply] = (pfl->rgwtMin[iSupply] + dChg);

L_6492:
    if ((LOWORD(dChg) != 0x0))
        goto L_64a4;
    else
        goto L_649b;

L_649b:
    if ((HIWORD(dChg) == 0x0))
        goto L_64bd;
    else
        goto L_64a4;

L_64a4:
    if ((pobj != 0x0))
        goto L_64bd;
    else
        goto L_64ad;

L_64ad:
    FLookupFleet(0xffff, pfl);

L_64bd:
    return dChg;
}

int32_t XferSupply(int16_t iSupply, int32_t cQuan) {
    int16_t  iSrc;
    int32_t  dChg;
    int32_t  cAvailable;
    uint16_t t_merge_6510_0001;
    uint16_t t_merge_65d1_0001;
    uint16_t t_merge_65fd_0001;
    uint16_t t_merge_661c_0001;

L_64cc:
    if ((LOWORD(cQuan) != 0x0))
        goto L_64f0;
    else
        goto L_64de;

L_64de:
    if ((HIWORD(cQuan) != 0x0))
        goto L_64f0;
    else
        goto L_64e7;

L_64e7:
    return 0x0;

L_64f0:
    if ((HIWORD(cQuan) < 0x0))
        goto L_650d;
    else
        goto L_64f9;

L_64f9:
    if ((HIWORD(cQuan) > 0x0))
        goto L_6507;
    else
        goto L_64fe;

L_64fe:
    if ((LOWORD(cQuan) <= 0x0))
        goto L_650d;
    else
        goto L_6507;

L_6507:
    t_merge_6510_0001 = 0x1;
    goto L_6510;

L_650d:
    t_merge_6510_0001 = 0x0;

L_6510:
    iSrc = t_merge_6510_0001;
    if ((iSrc != 0x0))
        goto L_652f;
    else
        goto L_651c;

L_651c:
    /* untranslated: LOWORD(cQuan) = (LOWORD(cQuan) neg 0x0) */
    /* untranslated: HIWORD(cQuan) = ((HIWORD(cQuan) + 0x0) neg 0x0) */

L_652f:
    cAvailable = ChgCargo(pxfer[iSrc].grobj, pxfer[iSrc].id, iSupply, 0x0, ((pxfer + (iSrc * 0x80)) + 0x4));
    if ((HIWORD(cQuan) < HIWORD(cAvailable)))
        goto L_65a4;
    else
        goto L_658b;

L_658b:
    if ((HIWORD(cQuan) > HIWORD(cAvailable)))
        goto L_6598;
    else
        goto L_6590;

L_6590:
    if ((LOWORD(cQuan) <= LOWORD(cAvailable)))
        goto L_65a4;
    else
        goto L_6598;

L_6598:
    cQuan = cAvailable;

L_65a4:
    if ((LOWORD(cQuan) != 0x0))
        goto L_65bf;
    else
        goto L_65ad;

L_65ad:
    if ((HIWORD(cQuan) != 0x0))
        goto L_65bf;
    else
        goto L_65b6;

L_65b6:
    return 0x0;

L_65bf:
    if ((iSrc != 0x0))
        goto L_65ce;
    else
        goto L_65c8;

L_65c8:
    t_merge_65d1_0001 = 0x1;
    goto L_65d1;

L_65ce:
    t_merge_65d1_0001 = 0x0;

L_65d1:
    if ((iSrc != 0x0))
        goto L_65fa;
    else
        goto L_65f4;

L_65f4:
    t_merge_65fd_0001 = 0x1;
    goto L_65fd;

L_65fa:
    t_merge_65fd_0001 = 0x0;

L_65fd:
    if ((iSrc != 0x0))
        goto L_6619;
    else
        goto L_6613;

L_6613:
    t_merge_661c_0001 = 0x1;
    goto L_661c;

L_6619:
    t_merge_661c_0001 = 0x0;

L_661c:
    dChg = ChgCargo(pxfer[t_merge_661c_0001].grobj, pxfer[t_merge_65fd_0001].id, iSupply, cQuan, ((pxfer + (t_merge_65d1_0001 * 0x80)) + 0x4));
    if ((LOWORD(dChg) != 0x0))
        goto L_664a;
    else
        goto L_6641;

L_6641:
    if ((HIWORD(dChg) == 0x0))
        goto L_6699;
    else
        goto L_664a;

L_664a:
    /* untranslated: call ChgCargo(pxfer[iSrc].grobj, pxfer[iSrc].id, iSupply, words(((HIWORD(dChg) + 0x0) neg 0x0), (LOWORD(dChg) neg 0x0)), ((pxfer + (iSrc *
     * 0x80)) + 0x4)) -> callresult(int32_t) */

L_6699:
    return dChg;
}

void UpdateXferBtns() {
    int16_t  iSide;
    int16_t  i;
    int16_t  iLastButton;
    int16_t  iVal;
    int32_t  lLeft;
    uint16_t t_merge_66c6_0001;
    uint32_t t_merge_677a_0001_wide;
    uint16_t t_merge_67af_0001;
    uint16_t t_merge_67dd_0001;
    uint16_t t_merge_67fc_0001;
    int32_t  t_merge_688b_0001_wide;

L_66a8:
    if ((mdXferDlg != 0x1))
        goto L_66c3;
    else
        goto L_66bb;

L_66bb:
    t_merge_66c6_0001 = (cXferValidHulls * 0x2);
    goto L_66c6;

L_66c3:
    t_merge_66c6_0001 = 0x4;

L_66c6:
    iLastButton = t_merge_66c6_0001;
    i = 0x0;
    goto L_68f7;

L_66d1:
    iVal = rgbtnXfer[i].iVal;
    /* untranslated: branch (part[12:2](rgbtnXfer[i]) & 0x1) == 0x0 ? L_68f3 : L_66fd */

L_66fd:
    if ((iVal <= iLastButton))
        goto L_6715;
    else
        goto L_6708;

L_6708:
    if ((mdXferDlg != 0x1))
        goto L_68f3;
    else
        goto L_670f;

L_670f:

L_6715:
    /* untranslated: iSide = ((part[12:2](rgbtnXfer[i]) >> 0x2) & 0x3) */
    if ((mdXferDlg != 0x1))
        goto L_679d;
    else
        goto L_6738;

L_6738:
    /* untranslated: branch 0x25:[(((pxfer + (iSide * 0x80)) + 0x10) + (iVal * 0x2))] != 0x7ffe ? L_6768 : L_675b */

L_675b:
    lLeft = 0x0;
    goto L_68be;

L_6768:
    if ((iSide != 0x0))
        goto L_6777;
    else
        goto L_6771;

L_6771:
    t_merge_677a_0001_wide = 0x10001;
    goto L_677a;

L_6777:
    t_merge_677a_0001_wide = 0x0;

L_677a:
    /* untranslated: LOWORD(lLeft) = 0x25:[(((pxfer + (loword(t_merge_677a_0001_wide) * 0x80)) + 0x10) + (iVal * 0x2))] */
    /* untranslated: HIWORD(lLeft) = signhiword(0x25:[(((pxfer + (hiword(t_merge_677a_0001_wide) * 0x80)) + 0x10) + (iVal * 0x2))]) */

L_679d:
    if ((iSide != 0x0))
        goto L_67ac;
    else
        goto L_67a6;

L_67a6:
    t_merge_67af_0001 = 0x1;
    goto L_67af;

L_67ac:
    t_merge_67af_0001 = 0x0;

L_67af:
    if ((iSide != 0x0))
        goto L_67da;
    else
        goto L_67d4;

L_67d4:
    t_merge_67dd_0001 = 0x1;
    goto L_67dd;

L_67da:
    t_merge_67dd_0001 = 0x0;

L_67dd:
    if ((iSide != 0x0))
        goto L_67f9;
    else
        goto L_67f3;

L_67f3:
    t_merge_67fc_0001 = 0x1;
    goto L_67fc;

L_67f9:
    t_merge_67fc_0001 = 0x0;

L_67fc:
    lLeft = ChgCargo(pxfer[t_merge_67fc_0001].grobj, pxfer[t_merge_67dd_0001].id, iVal, 0x0, ((pxfer + (t_merge_67af_0001 * 0x80)) + 0x4));
    if ((LOWORD(lLeft) != 0x0))
        goto L_682a;
    else
        goto L_6821;

L_6821:
    if ((HIWORD(lLeft) == 0x0))
        goto L_6894;
    else
        goto L_682a;

L_682a:
    if ((pxfer[iSide].grobj != grobjFleet))
        goto L_6894;
    else
        goto L_6841;

L_6841:
    if ((iVal != 0x4))
        goto L_686c;
    else
        goto L_684a;

L_684a:
    t_merge_688b_0001_wide = GetFuelFree(((pxfer + (iSide * 0x80)) + 0x4));
    goto L_688b;

L_686c:
    t_merge_688b_0001_wide = GetCargoFree(((pxfer + (iSide * 0x80)) + 0x4));

L_688b:
    lLeft = t_merge_688b_0001_wide;
    goto L_68be;

L_6894:
    if ((pxfer[iSide].grobj != grobjPlanet))
        goto L_68be;
    else
        goto L_68ab;

L_68ab:
    if ((iVal != 0x4))
        goto L_68be;
    else
        goto L_68b4;

L_68b4:
    lLeft = 0x0;

L_68be:
    if ((LOWORD(lLeft) != 0x0))
        goto L_68e3;
    else
        goto L_68c7;

L_68c7:
    if ((HIWORD(lLeft) != 0x0))
        goto L_68e3;
    else
        goto L_68d0;

L_68d0:
    rgbtnXfer[i].bt = (rgbtnXfer[i].bt | 0x4);
    goto L_68f3;

L_68e3:
    rgbtnXfer[i].bt = (rgbtnXfer[i].bt & 0xfffb);

L_68f3:
    i = (i + 0x1);

L_68f7:
    if ((i < LOWORD(crgbtnXfer)))
        goto L_66d1;
    else
        goto L_6902;

L_6902:
    return;
}

void DrawXferDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iSupply) {
    RECT    rgrc[2];
    int16_t fCreatedDC;
    int16_t i;
    int16_t dxCtr;

L_6908:
    fCreatedDC = 0x0;
    if ((hdc != 0x0))
        goto L_692f;
    else
        goto L_691f;

L_691f:
    fCreatedDC = 0x1;
    hdc = GetDC(hwnd);

L_692f:
    dxCtr = (prc->right >> 0x1);
    if ((iSupply >= 0x0))
        goto L_69d0;
    else
        goto L_6943;

L_6943:
    PatBlt(hdc, dxCtr, 0x0, 0x1, prc->bottom, BLACKNESS);
    i = 0x0;
    goto L_69b9;

L_696c:
    /* untranslated: branch (part[12:2](rgbtnXfer[i]) & 0x1) == 0x0 ? L_69b5 : L_6986 */

L_6986:
    DrawBtn(hdc, (rgbtnXfer + LOWORD((0xe * i))), rgbtnXfer[i].bt, 0x0, 0x0);

L_69b5:
    i = (i + 0x1);

L_69b9:
    if ((i < LOWORD(crgbtnXfer)))
        goto L_696c;
    else
        goto L_69c4;

L_69c4:
    if ((iSupply == 0xfffe))
        goto RelDC;
    else
        goto L_69ca;

L_69ca:

L_69d0:
    /* untranslated: call GetXferLeftRightRcs(prc, rgrc, &part[8:2](rgrc)) -> callresult(void) */
    i = 0x0;
    goto L_6b22;

L_69eb:
    if ((mdXferDlg != 0x1))
        goto L_6a29;
    else
        goto L_69f5;

L_69f5:
    DrawFleetShipsXferSide(hdc, rgrc[i], ((pxfer + (i * 0x80)) + 0x4), iSupply);
    goto L_6b1e;

L_6a29:
    if ((pxfer[i].grobj != grobjFleet))
        goto L_6a74;
    else
        goto L_6a40;

L_6a40:
    DrawFleetCargoXferSide(hdc, rgrc[i], ((pxfer + (i * 0x80)) + 0x4), iSupply);
    goto L_6b1e;

L_6a74:
    if ((pxfer[i].grobj == grobjPlanet))
        goto L_6aa2;
    else
        goto L_6a8b;

L_6a8b:
    if ((pxfer[i].grobj != grobjOther))
        goto L_6ad6;
    else
        goto L_6aa2;

L_6aa2:
    DrawPlanetXferSide(hdc, rgrc[i], ((pxfer + (i * 0x80)) + 0x4), iSupply);
    goto L_6b1e;

L_6ad6:
    if ((pxfer[i].grobj != grobjThing))
        goto L_6b1e;
    else
        goto L_6aed;

L_6aed:
    DrawThingXferSide(hdc, rgrc[i], ((pxfer + (i * 0x80)) + 0x4), iSupply);

L_6b1e:
    i = (i + 0x1);

L_6b22:
    if ((i < 0x2))
        goto L_69eb;
    else
        goto RelDC;

RelDC:
    if ((fCreatedDC == 0x0))
        goto L_6b3f;
    else
        goto L_6b34;

L_6b34:
    ReleaseDC(hwnd, hdc);

L_6b3f:
    return;
}

void GetXferLeftRightRcs(RECT *prcWhole, RECT *prcLeft, RECT *prcRight) {
L_6b46:
    SetRect(prcLeft, 0x0, 0x0, (prcWhole->right >> 0x1), prcWhole->bottom);
    /* untranslated: call ExpandRc(prcLeft, (((dyArial8 + 0x3) neg 0x0) + 0xfffc), 0xfffc) -> callresult(void) */
    prcLeft->left = (prcLeft->left - (dyArial8 + 0x1));
    SetRect(prcRight, (prcWhole->right >> 0x1), 0x0, prcWhole->right, prcWhole->bottom);
    /* untranslated: call ExpandRc(prcRight, (((dyArial8 + 0x3) neg 0x0) + 0xfffc), 0xfffc) -> callresult(void) */
    prcRight->right = (prcRight->right + (dyArial8 + 0x1));
    return;
}

int16_t FSetupXferBtns(RECT *prc) {
    int16_t  cBtn;
    int16_t  iMax;
    int16_t  dy;
    int16_t  iMin;
    int16_t  i;
    int16_t  fThingXfer;
    int16_t  j;
    int16_t  dxCtr;
    RECT     rcRight;
    int16_t  dxLabels;
    RECT     rcBtn;
    RECT     rcLeft;
    RECT     rc;
    uint16_t t_merge_6c0b_0001;
    uint16_t t_merge_6c25_0001;
    int16_t  t_merge_6c4f_0001;
    uint16_t t_merge_6d1c_0001;
    int16_t  t_merge_6e01_0001;

L_6bea:
    cBtn = 0x0;
    if ((mdXferDlg != 0x1))
        goto L_6c08;
    else
        goto L_6c02;

L_6c02:
    t_merge_6c0b_0001 = 0x8c;
    goto L_6c0b;

L_6c08:
    t_merge_6c0b_0001 = 0x4b;

L_6c0b:
    dxLabels = t_merge_6c0b_0001;
    if ((*(pxfer + 0x82) != 0x8))
        goto L_6c22;
    else
        goto L_6c1c;

L_6c1c:
    t_merge_6c25_0001 = 0x1;
    goto L_6c25;

L_6c22:
    t_merge_6c25_0001 = 0x0;

L_6c25:
    fThingXfer = t_merge_6c25_0001;
    dxCtr = (prc->right >> 0x1);
    dy = (dyArial8 + 0xa);
    if ((mdXferDlg != 0x1))
        goto L_6c4c;
    else
        goto L_6c46;

L_6c46:
    t_merge_6c4f_0001 = cXferValidHulls;
    goto L_6c4f;

L_6c4c:
    t_merge_6c4f_0001 = 0x5;

L_6c4f:
    iMax = t_merge_6c4f_0001;
    if ((mdXferDlg == 0x1))
        goto L_6c67;
    else
        goto L_6c5c;

L_6c5c:
    dy = (dy + ((dyArial8 + 0x6) * 0x2));

L_6c67:
    i = 0x0;
    goto L_6c82;

L_6c6f:
    i = (i + 0x1);
    dy = (dy + (dyArial8 + 0x6));

L_6c82:
    if ((i >= iMax))
        goto L_6e41;
    else
        goto L_6c8d;

L_6c8d:
    if ((i != 0x4))
        goto L_6cae;
    else
        goto L_6c96;

L_6c96:
    if ((mdXferDlg == 0x1))
        goto L_6cae;
    else
        goto L_6ca0;

L_6ca0:
    dy = (dy - LOWORD(((dyArial8 + 0x6) * 0x6)));

L_6cae:
    SetRect(&(rcBtn), ((dxCtr - (dyArial8 + 0x3)) + 0x1), dy, (dxCtr + 0x1), ((dyArial8 + 0x3) + dy));
    j = 0x0;
    goto L_6e35;

L_6ce5:
    rgbtnXfer[cBtn].rc = rcBtn;
    if ((j != 0x0))
        goto L_6d19;
    else
        goto L_6d13;

L_6d13:
    t_merge_6d1c_0001 = 0x2;
    goto L_6d1c;

L_6d19:
    t_merge_6d1c_0001 = 0x3;

L_6d1c:
    rgbtnXfer[cBtn].bt = t_merge_6d1c_0001;
    if ((fThingXfer == 0x0))
        goto L_6d81;
    else
        goto L_6d3a;

L_6d3a:
    if ((i < 0x4))
        goto L_6d81;
    else
        goto L_6d43;

L_6d43:
    /* untranslated: part[12:2](rgbtnXfer[cBtn]) = ((part[12:2](rgbtnXfer[cBtn]) & 0xfffe) | 0x0) */
    rgbtnXfer[cBtn].rc.bottom = 0xff9c;
    goto L_6dab;

L_6d81:
    /* untranslated: part[12:2](rgbtnXfer[cBtn]) = ((part[12:2](rgbtnXfer[cBtn]) & 0xfffe) | 0x1) */

L_6dab:
    /* untranslated: part[12:2](rgbtnXfer[cBtn]) = ((part[12:2](rgbtnXfer[cBtn]) & 0xfff3) | ((j & 0x3) * 0x4)) */
    if ((mdXferDlg != 0x1))
        goto L_6dfe;
    else
        goto L_6dee;

L_6dee:
    t_merge_6e01_0001 = rgXferValidHulls[i];
    goto L_6e01;

L_6dfe:
    t_merge_6e01_0001 = i;

L_6e01:
    rgbtnXfer[cBtn].iVal = t_merge_6e01_0001;
    cBtn = (cBtn + 0x1);
    OffsetRc(&(rcBtn), (dyArial8 + 0x2), 0x0);
    j = (j + 0x1);

L_6e35:
    if ((j >= 0x2))
        goto L_6c6f;
    else
        goto L_6e3b;

L_6e3b:

L_6e41:
    GetXferLeftRightRcs(prc, &(rcLeft), &(rcRight));
    if ((mdXferDlg == 0x1))
        goto NoGauges;
    else
        goto L_6e5b;

L_6e5b:

L_6e64:
    if ((pxfer->grobj == grobjPlanet))
        goto L_6e88;
    else
        goto L_6e71;

L_6e71:
    rc = rcLeft;
    i = 0x0;
    goto L_7067;

L_6e88:
    if ((*(pxfer + 0x82) == 0x1))
        goto NoGauges;
    else
        goto L_6e93;

L_6e93:

L_6e99:
    rc = rcRight;
    i = 0x1;

L_6eb0:
    SetRect(&(rcBtn), ((rc.left + dxLabels) + 0xa), ((rc.top + dyArial8) + 0x6), (rc.right + 0xfffc), (((dyArial8 * 0x2) + rc.top) + 0x6));
    if ((mdXferDlg != 0x1))
        goto L_6efc;
    else
        goto L_6eee;

L_6eee:
    iMin = 0x0;
    iMax = cXferValidHulls;
    goto L_6f06;

L_6efc:
    iMin = 0x0;
    iMax = 0x5;

L_6f06:
    j = iMin;
    goto L_7038;

L_6f0f:
    if ((mdXferDlg == 0x1))
        goto L_6f65;
    else
        goto L_6f19;

L_6f19:
    if ((j != 0x0))
        goto L_6f3e;
    else
        goto L_6f22;

L_6f22:
    OffsetRc(&(rcBtn), 0x0, ((dyArial8 + 0x6) * 0x2));
    goto L_6f65;

L_6f3e:
    if ((j != 0x4))
        goto L_6f65;
    else
        goto L_6f47;

L_6f47:
    /* untranslated: call OffsetRc(&rcBtn, 0x0, loword((((dyArial8 + 0x6) neg 0x0) * 0x6))) -> callresult(void) */

L_6f65:
    rgbtnXfer[cBtn].rc = rcBtn;
    rgbtnXfer[cBtn].bt = 0x0;
    /* untranslated: part[12:2](rgbtnXfer[cBtn]) = ((part[12:2](rgbtnXfer[cBtn]) & 0xfffe) | 0x0) */
    /* untranslated: part[12:2](rgbtnXfer[cBtn]) = ((part[12:2](rgbtnXfer[cBtn]) & 0xfff3) | ((i & 0x3) * 0x4)) */
    rgbtnXfer[cBtn].iVal = (j + 0x80);
    cBtn = (cBtn + 0x1);
    OffsetRc(&(rcBtn), 0x0, (dyArial8 + 0x6));
    j = (j + 0x1);

L_7038:
    if ((j < iMax))
        goto L_6f0f;
    else
        goto L_7043;

L_7043:
    if ((*(pxfer + 0x82) == 0x1))
        goto NoGauges;
    else
        goto L_704e;

L_704e:

L_7054:
    rc = rcRight;
    i = (i + 0x1);

L_7067:
    if ((i < 0x2))
        goto L_6eb0;
    else
        goto NoGauges;

NoGauges:
    LOWORD(crgbtnXfer) = cBtn;
    UpdateXferBtns();

L_7081:
    return 0x1;
}

void DrawThingXferSide(HDC hdc, RECT *prc, THING *pth, int16_t iSupply) {
    int16_t  yTop;
    int16_t  i;
    int16_t  xRight;
    int16_t  dxLabels;
    RECT     rcGauge;
    int16_t  xLeft;
    RECT     rc;
    uint16_t t_merge_71d5_0001;

L_7088:
    dxLabels = 0x4b;
    rc = *(prc);
    rc.bottom = ((rc.top + rc.right) - rc.left);
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, HIWORD(rghfontArial8));
    if ((iSupply != 0xffff))
        goto L_70f9;
    else
        goto L_70e6;

L_70e6:
    _Draw3dFrame(hdc, &(rc), 0x0);

L_70f9:
    ExpandRc(&(rc), 0xffff, 0xffff);
    rc.bottom = ((rc.top + dyArial8) + 0x2);
    if ((iSupply != 0xffff))
        goto L_7136;
    else
        goto L_7123;

L_7123:
    _Draw3dFrame(hdc, &(rc), 0x0);

L_7136:
    if ((iSupply != 0xffff))
        goto L_7161;
    else
        goto L_713f;

L_713f:
    RcCtrTextOut(hdc, &(rc), PszGetThingName(pth), 0x0);

L_7161:
    xLeft = (prc->left + 0x4);
    xRight = (prc->right + 0xfffc);
    yTop = ((rc.bottom + 0x3) + (dyArial8 + 0x6));
    if ((iSupply != 0xffff))
        goto L_720a;
    else
        goto L_7192;

L_7192:
    SelectObject(hdc, HIWORD(rghfontArial8));
    i = 0xffff;
    goto L_7201;

L_71a6:
    if ((i == 0x4))
        goto L_71f4;
    else
        goto L_71af;

L_71af:
    if ((i == 0x4))
        goto L_71f4;
    else
        goto L_71b8;

L_71b8:
    if ((i != 0xffff))
        goto L_71cf;
    else
        goto L_71c9;

L_71c9:
    t_merge_71d5_0001 = 0x1b2;
    goto L_71d5;

L_71cf:
    t_merge_71d5_0001 = (i + 0x1ae);

L_71d5:
    RightTextOut(hdc, (xLeft + dxLabels), yTop, PszGetCompressedString(t_merge_71d5_0001), 0x0, 0x0);

L_71f4:
    yTop = (yTop + (dyArial8 + 0x6));
    i = (i + 0x1);

L_7201:
    if ((i < 0x3))
        goto L_71a6;
    else
        goto L_720a;

L_720a:
    if ((iSupply == 0x4))
        goto L_72d7;
    else
        goto L_7213;

L_7213:
    if ((iSupply == 0x3))
        goto L_72d7;
    else
        goto L_7219;

L_7219:

L_721f:
    yTop = ((rc.bottom + 0x3) + (dyArial8 + 0x6));
    xLeft = (xLeft + (dxLabels + 0x6));
    SetRect(&(rcGauge), xLeft, yTop, xRight, (yTop + dyArial8));
    DrawThingGauge(hdc, &(rcGauge), pth, 0x5);
    i = 0x0;
    goto L_72ce;

L_7278:
    OffsetRc(&(rcGauge), 0x0, (dyArial8 + 0x6));
    if ((iSupply == 0xffff))
        goto L_72a3;
    else
        goto L_7298;

L_7298:
    if ((iSupply != i))
        goto L_72ca;
    else
        goto L_72a3;

L_72a3:
    DrawThingGauge(hdc, &(rcGauge), pth, i);
    if ((iSupply == i))
        goto L_72d7;
    else
        goto L_72c4;

L_72c4:

L_72ca:
    i = (i + 0x1);

L_72ce:
    if ((i < 0x3))
        goto L_7278;
    else
        goto L_72d7;

L_72d7:
    return;
}

void DrawFleetCargoXferSide(HDC hdc, RECT *prc, FLEET *pfl, int16_t iSupply) {
    int16_t  yTop;
    int16_t  fOtherPlr;
    int16_t  c;
    int16_t  i;
    int16_t  xRight;
    FLEET    fl;
    int16_t  dxLabels;
    RECT     rcGauge;
    int16_t  xLeft;
    RECT     rc;
    int16_t  iMap;
    uint16_t t_merge_72fe_0001;
    uint16_t t_merge_756e_0001;
    uint16_t t_merge_75a4_0001;

L_72de:
    if ((pfl->iPlayer == idPlayer))
        goto L_72fb;
    else
        goto L_72f5;

L_72f5:
    t_merge_72fe_0001 = 0x1;
    goto L_72fe;

L_72fb:
    t_merge_72fe_0001 = 0x0;

L_72fe:
    fOtherPlr = t_merge_72fe_0001;
    dxLabels = 0x4b;
    fl = *(pfl);
    rc = *(prc);
    rc.bottom = ((rc.top + rc.right) - rc.left);
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, HIWORD(rghfontArial8));
    if ((iSupply != 0xffff))
        goto L_7383;
    else
        goto L_736f;

L_736f:
    _Draw3dFrame(hdc, &(rc), 0x0);

L_7383:
    ExpandRc(&(rc), 0xffff, 0xffff);
    rc.bottom = ((rc.top + dyArial8) + 0x2);
    if ((iSupply != 0xffff))
        goto L_73c4;
    else
        goto L_73b0;

L_73b0:
    _Draw3dFrame(hdc, &(rc), 0x0);

L_73c4:
    if ((iSupply != 0xffff))
        goto L_73ee;
    else
        goto L_73cd;

L_73cd:
    RcCtrTextOut(hdc, &(rc), PszGetFleetName(fl.id), 0x0);

L_73ee:
    xLeft = (prc->left + 0x4);
    xRight = (prc->right + 0xfffc);
    yTop = (rc.bottom + 0x3);
    if ((iSupply != 0xffff))
        goto L_7484;
    else
        goto L_7419;

L_7419:
    SelectObject(hdc, HIWORD(rghfontArial8));
    i = 0x0;
    goto L_747b;

L_742d:
    if ((i != 0x6))
        goto L_743f;
    else
        goto L_7436;

L_7436:
    if ((fOtherPlr != 0x0))
        goto L_746e;
    else
        goto L_743f;

L_743f:
    RightTextOut(hdc, (xLeft + dxLabels), yTop, PszGetCompressedString((i + 0x1ac)), 0x0, 0x0);

L_746e:
    yTop = (yTop + (dyArial8 + 0x6));
    i = (i + 0x1);

L_747b:
    if ((i < 0x6))
        goto L_742d;
    else
        goto L_7484;

L_7484:
    yTop = (rc.bottom + 0x3);
    xLeft = (xLeft + (dxLabels + 0x6));
    if ((fOtherPlr == 0x0))
        goto L_760e;
    else
        goto L_74a2;

L_74a2:
    xRight = (xLeft + dxMaxMineralQuan);
    SetRect(&(rc), (xLeft + 0xfffe), (yTop + 0xffff), ((xLeft + dxMaxMineralQuan) + 0x2), ((yTop + dyArial8) + 0x1));
    i = 0x0;
    goto L_74fb;

L_74e8:
    i = (i + 0x1);
    yTop = (yTop + (dyArial8 + 0x6));

L_74fb:
    if ((i >= 0x6))
        goto L_7720;
    else
        goto L_7504;

L_7504:
    if ((i != 0x1))
        goto L_7528;
    else
        goto L_750d;

L_750d:
    OffsetRc(&(rc), 0x0, (dyArial8 + 0x6));
    goto L_74e8;

L_7528:
    if ((i != 0x0))
        goto L_753a;
    else
        goto L_7531;

L_7531:
    iMap = 0x4;
    goto L_7544;

L_753a:
    iMap = (i + 0xfffe);

L_7544:
    if ((iSupply == 0xffff))
        goto L_7559;
    else
        goto L_754d;

L_754d:
    if ((iSupply != iMap))
        goto L_75f0;
    else
        goto L_7559;

L_7559:
    if ((iSupply != iMap))
        goto L_756b;
    else
        goto L_7565;

L_7565:
    t_merge_756e_0001 = 0x1;
    goto L_756e;

L_756b:
    t_merge_756e_0001 = 0x0;

L_756e:
    _Draw3dFrame(hdc, &(rc), t_merge_756e_0001);
    if ((iMap != 0x4))
        goto L_75a1;
    else
        goto L_759b;

L_759b:
    t_merge_75a4_0001 = 0x1;
    goto L_75a4;

L_75a1:
    t_merge_75a4_0001 = 0x0;

L_75a4:
    c = _wsprintf(szWork, PszGetCompressedString((t_merge_75a4_0001 + 0x37c)), LOWORD(fl.rgwtMin[iMap]), HIWORD(fl.rgwtMin[iMap]));
    RightTextOut(hdc, xRight, yTop, 0x57a4, c, 0x0);
    if ((iSupply == i))
        goto L_7720;
    else
        goto L_75ea;

L_75ea:

L_75f0:
    OffsetRc(&(rc), 0x0, (dyArial8 + 0x6));
    goto L_74e8;

L_760e:
    SetRect(&(rcGauge), xLeft, yTop, xRight, (yTop + dyArial8));
    if ((iSupply == 0xffff))
        goto L_763f;
    else
        goto L_7636;

L_7636:
    if ((iSupply != 0x4))
        goto L_765b;
    else
        goto L_763f;

L_763f:
    DrawFleetGauge(hdc, &(rcGauge), &(fl), 0x4);

L_765b:
    if ((iSupply == 0x4))
        goto L_7720;
    else
        goto L_7661;

L_7661:

L_7667:
    yTop = (yTop + (dyArial8 + 0x6));
    OffsetRc(&(rcGauge), 0x0, (dyArial8 + 0x6));
    DrawFleetGauge(hdc, &(rcGauge), &(fl), 0x5);
    yTop = (yTop + (dyArial8 + 0x6));
    i = 0x0;
    goto L_7717;

L_76b5:
    OffsetRc(&(rcGauge), 0x0, (dyArial8 + 0x6));
    if ((iSupply == 0xffff))
        goto L_76e1;
    else
        goto L_76d6;

L_76d6:
    if ((iSupply != i))
        goto L_770a;
    else
        goto L_76e1;

L_76e1:
    DrawFleetGauge(hdc, &(rcGauge), &(fl), i);
    if ((iSupply == i))
        goto L_7720;
    else
        goto L_7704;

L_7704:

L_770a:
    yTop = (yTop + (dyArial8 + 0x6));
    i = (i + 0x1);

L_7717:
    if ((i <= 0x3))
        goto L_76b5;
    else
        goto L_7720;

L_7720:
    return;
}

void DrawFleetShipsXferSide(HDC hdc, RECT *prc, FLEET *pfl, int16_t iSupply) {
    int16_t  yTop;
    int16_t  fOtherPlr;
    int16_t  c;
    int16_t  i;
    int16_t  xRight;
    FLEET    fl;
    int16_t  xLeft;
    RECT     rc;
    uint16_t t_merge_7746_0001;
    uint16_t t_merge_7924_0001;

L_7726:
    if ((pfl->iPlayer == idPlayer))
        goto L_7743;
    else
        goto L_773d;

L_773d:
    t_merge_7746_0001 = 0x1;
    goto L_7746;

L_7743:
    t_merge_7746_0001 = 0x0;

L_7746:
    fOtherPlr = t_merge_7746_0001;
    fl = *(pfl);
    rc = *(prc);
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, HIWORD(rghfontArial8));
    if ((iSupply != 0xffff))
        goto L_77b5;
    else
        goto L_77a1;

L_77a1:
    _Draw3dFrame(hdc, &(rc), 0x0);

L_77b5:
    ExpandRc(&(rc), 0xffff, 0xffff);
    rc.bottom = ((rc.top + dyArial8) + 0x2);
    if ((iSupply != 0xffff))
        goto L_77f6;
    else
        goto L_77e2;

L_77e2:
    _Draw3dFrame(hdc, &(rc), 0x0);

L_77f6:
    if ((iSupply != 0xffff))
        goto L_7820;
    else
        goto L_77ff;

L_77ff:
    RcCtrTextOut(hdc, &(rc), PszGetFleetName(fl.id), 0x0);

L_7820:
    xLeft = (((prc->right + 0xfffc) - dxMaxMineralQuan) + 0xfffe);
    xRight = (xLeft + dxMaxMineralQuan);
    yTop = (rc.bottom + 0x3);
    if ((iSupply != 0xffff))
        goto L_78b7;
    else
        goto L_7852;

L_7852:
    SelectObject(hdc, HIWORD(rghfontArial8));
    i = 0x0;
    goto L_78ac;

L_7866:
    /* untranslated: call RightTextOut(hdc, (xLeft + 0xfff8), yTop, part[8:0](rgshdef[rgXferValidHulls[i]].hul), 0x0, 0x0) -> callresult(void) */
    yTop = (yTop + (dyArial8 + 0x6));
    i = (i + 0x1);

L_78ac:
    if ((i < cXferValidHulls))
        goto L_7866;
    else
        goto L_78b7;

L_78b7:
    yTop = (rc.bottom + 0x3);
    SetRect(&(rc), (xLeft + 0xfffe), (yTop + 0xffff), ((xLeft + dxMaxMineralQuan) + 0x2), ((yTop + dyArial8) + 0x1));
    i = 0x0;
    goto L_79b8;

L_78fc:
    if ((iSupply == 0xffff))
        goto L_7910;
    else
        goto L_7905;

L_7905:
    if ((iSupply != i))
        goto L_7993;
    else
        goto L_7910;

L_7910:
    if ((iSupply != i))
        goto L_7921;
    else
        goto L_791b;

L_791b:
    t_merge_7924_0001 = 0x1;
    goto L_7924;

L_7921:
    t_merge_7924_0001 = 0x0;

L_7924:
    _Draw3dFrame(hdc, &(rc), t_merge_7924_0001);
    c = _wsprintf(szWork, PCTD, pfl->rgcsh[rgXferValidHulls[i]]);
    RightTextOut(hdc, xRight, yTop, 0x57a4, c, 0x0);
    if ((iSupply == i))
        goto L_79c3;
    else
        goto L_798d;

L_798d:

L_7993:
    OffsetRc(&(rc), 0x0, (dyArial8 + 0x6));
    yTop = (yTop + (dyArial8 + 0x6));
    i = (i + 0x1);

L_79b8:
    if ((i < cXferValidHulls))
        goto L_78fc;
    else
        goto L_79c3;

L_79c3:
    return;
}

void DrawPlanetXferSide(HDC hdc, RECT *prc, PLANET *ppl, int16_t iSupply) {
    PLANET   pl;
    int16_t  yTop;
    int16_t  c;
    int16_t  i;
    int16_t  xRight;
    char    *psz;
    int16_t  xLeft;
    RECT     rc;
    uint16_t t_merge_7c4d_0001;

L_79ca:
    pl = *(ppl);
    rc = *(prc);
    rc.bottom = ((rc.top + rc.right) - rc.left);
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, HIWORD(rghfontArial8));
    if ((iSupply != 0xffff))
        goto L_7a46;
    else
        goto L_7a33;

L_7a33:
    _Draw3dFrame(hdc, &(rc), 0x0);

L_7a46:
    ExpandRc(&(rc), 0xffff, 0xffff);
    rc.bottom = ((rc.top + dyArial8) + 0x2);
    if ((iSupply != 0xffff))
        goto L_7ac2;
    else
        goto L_7a70;

L_7a70:
    _Draw3dFrame(hdc, &(rc), 0x0);
    if ((pl.id == 0xffff))
        goto L_7a9d;
    else
        goto L_7a8c;

L_7a8c:
    psz = PszGetPlanetName(pl.id);
    goto L_7aac;

L_7a9d:
    psz = PszGetCompressedString(idsDeepSpace);

L_7aac:
    RcCtrTextOut(hdc, &(rc), psz, 0x0);

L_7ac2:
    xLeft = (prc->left + 0x4);
    xRight = (prc->right + 0xfffc);
    yTop = (rc.bottom + 0x3);
    if ((iSupply != 0xffff))
        goto L_7b4b;
    else
        goto L_7aeb;

L_7aeb:
    SelectObject(hdc, HIWORD(rghfontArial8));
    i = 0x0;
    goto L_7b42;

L_7aff:
    if ((i <= 0x1))
        goto L_7b35;
    else
        goto L_7b08;

L_7b08:
    RightTextOut(hdc, (xLeft + 0x4b), yTop, PszGetCompressedString((i + 0x1ac)), 0x0, 0x0);

L_7b35:
    yTop = (yTop + (dyArial8 + 0x6));
    i = (i + 0x1);

L_7b42:
    if ((i < 0x6))
        goto L_7aff;
    else
        goto L_7b4b;

L_7b4b:
    yTop = (rc.bottom + 0x3);
    xLeft = (xLeft + 0x51);
    xRight = ((xLeft + dxMaxMineralQuan) + 0xc);
    SetRect(&(rc), (xLeft + 0xfffe), (yTop + 0xffff), ((xLeft + dxMaxMineralQuan) + 0xe), ((yTop + dyArial8) + 0x1));
    i = 0x0;
    goto L_7bb0;

L_7b9d:
    i = (i + 0x1);
    yTop = (yTop + (dyArial8 + 0x6));

L_7bb0:
    if ((i > 0x4))
        goto L_7cd4;
    else
        goto L_7bb9;

L_7bb9:
    if ((i != 0x0))
        goto L_7be9;
    else
        goto L_7bc2;

L_7bc2:
    yTop = (yTop + ((dyArial8 + 0x6) * 0x2));
    OffsetRc(&(rc), 0x0, ((dyArial8 + 0x6) * 0x2));
    goto L_7c1c;

L_7be9:
    if ((i != 0x4))
        goto L_7c1c;
    else
        goto L_7bf2;

L_7bf2:
    yTop = (yTop - LOWORD(((dyArial8 + 0x6) * 0x6)));
    OffsetRc(&(rc), 0x0, LOWORD(((dyArial8 + 0x6) * 0x6)));

L_7c1c:
    if ((iSupply == 0xffff))
        goto L_7c30;
    else
        goto L_7c25;

L_7c25:
    if ((iSupply != i))
        goto L_7cba;
    else
        goto L_7c30;

L_7c30:
    if ((i == 0x4))
        goto L_7cba;
    else
        goto L_7c39;

L_7c39:
    if ((iSupply != i))
        goto L_7c4a;
    else
        goto L_7c44;

L_7c44:
    t_merge_7c4d_0001 = 0x1;
    goto L_7c4d;

L_7c4a:
    t_merge_7c4d_0001 = 0x0;

L_7c4d:
    _Draw3dFrame(hdc, &(rc), t_merge_7c4d_0001);
    c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), LOWORD(pl.rgwtMin[i]), HIWORD(pl.rgwtMin[i]));
    RightTextOut(hdc, xRight, yTop, 0x57a4, c, 0x0);
    if ((iSupply == i))
        goto L_7cd4;
    else
        goto L_7cb4;

L_7cb4:

L_7cba:
    OffsetRc(&(rc), 0x0, (dyArial8 + 0x6));
    goto L_7b9d;

L_7cd4:
    return;
}

uint16_t ClickInShipOrders(POINT pt, int16_t sks, int16_t fCursor, int16_t fRightBtn) {
    int32_t  lCur;
    HDC      hdc;
    PLANET   pl;
    int16_t  iWarp;
    POINT    ptOld;
    int16_t  idPlan;
    int32_t  lMax;
    int32_t  lSel;
    int16_t  iSkip;
    int32_t  xRnd;
    int16_t  grbit;
    XFER     xf;
    int32_t  lNew;
    int16_t  irc;
    int32_t  dx;
    int32_t  lTempMin;
    int16_t  fFirst;
    int16_t  fTwoMAs;
    int32_t  lTempMax;
    int16_t  cMax;
    char     sz255[2];
    int16_t  i;
    char    *rgszZip[11];
    ZIPORDER rgzo[4];
    int16_t (**lpProc)();
    int16_t    fRet;
    TASKXPORT *lptxp;
    int16_t    fSep;
    int16_t    c;
    ORDER     *lpord;
    THING     *lpth;
    FLEET     *lpfl;
    int32_t    rgid[100];
    int16_t    iChecked;
    THING     *lpthMac;
    SCAN       scan;
    uint16_t   t_merge_8439_0001;
    int16_t    t_merge_8a98_0001;
    int32_t    t_merge_8c47_0001_wide;
    uint32_t   t_merge_8c7d_0001_wide;
    int32_t    t_merge_8d3f_0003_wide;
    int32_t    t_merge_8d93_0001_wide;
    uint16_t   t_merge_8eac_0001;
    GrobjClass t_merge_8faf_0001;

L_7cda:
    lTempMin = 0x0;
    irc = 0xffff;
    if ((sel.grobj != grobjNone))
        goto L_7d05;
    else
        goto L_7cff;

L_7cff:
    return 0x0;

L_7d05:
    /* untranslated: branch PtInRect(0x48d8, words(pt.y, pt.x)) == 0x0 ? L_823a : L_7d1f */

L_7d1f:
    if ((fCursor == 0x0))
        goto L_7d2e;
    else
        goto L_7d28;

L_7d28:
    return hcurArrowHelp;

L_7d2e:
    if ((fRightBtn == 0x0))
        goto L_821f;
    else
        goto L_7d37;

L_7d37:
    /* untranslated: part[0:1](sz255) = 0xff */
    /* untranslated: part[1:1](sz255) = 0x0 */
    i = 0x0;
    goto L_7d67;

L_7d4a:
    rgszZip[i] = rgszZipOrder[i];
    i = (i + 0x1);

L_7d67:
    if ((i < 0x4))
        goto L_7d4a;
    else
        goto L_7d71;

L_7d71:
    /* untranslated: part[8:2](rgszZip) = sz255 */
    cMax = 0x5;
    i = 0x0;
    goto L_7dcb;

L_7d88:
    if ((vrgZip[i].fValid == 0x0))
        goto L_7dc6;
    else
        goto L_7da2;

L_7da2:
    cMax = (cMax + 0x1);
    /* untranslated: rgszZip[cMax] = part[10:0](vrgZip[i]) */

L_7dc6:
    i = (i + 0x1);

L_7dcb:
    if ((i < 0x4))
        goto L_7d88;
    else
        goto L_7dd5;

L_7dd5:
    if ((cMax <= 0x5))
        goto L_7df6;
    else
        goto L_7ddf;

L_7ddf:
    cMax = (cMax + 0x1);
    rgszZip[cMax] = sz255;

L_7df6:
    cMax = (cMax + 0x1);
    rgszZip[cMax] = PszGetCompressedString(idsCustomize);
    i = PopupMenu(hwndPlanet, pt.x, pt.y, cMax, 0x0, rgszZip, 0xffff, 0x1);
    if ((i != (cMax + 0xffff)))
        goto L_7ece;
    else
        goto L_7e50;

L_7e50:
    memcpy(rgzo, 0x5264, 0x60);
    lpProc = MakeProcInstance(ZipOrderDlg, hInst);
    fRet = DialogBox(hInst, IDD_ZIP_PROD, hwndFrame, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((fRet != 0x0))
        goto L_8b82;
    else
        goto L_7eb6;

L_7eb6:
    memcpy(0x5264, rgzo, 0x60);

L_7ece:
    if ((i <= 0x4))
        goto L_7f8c;
    else
        goto L_7ed8;

L_7ed8:
    i = (i + 0xfffc);
    iSkip = 0x0;
    goto L_7f1c;

L_7eeb:
    if ((vrgZip[iSkip].fValid == 0x0))
        goto L_7f18;
    else
        goto L_7f04;

L_7f04:
    i = (i - 0x1);
    if ((i == 0x0))
        goto L_7f26;
    else
        goto L_7f12;

L_7f12:

L_7f18:
    iSkip = (iSkip + 0x1);

L_7f1c:
    if ((i != 0x0))
        goto L_7eeb;
    else
        goto L_7f26;

L_7f26:
    /* untranslated: part[8:10](sel.fl.lpplord->rgord[sel.iwpAct]) = vrgZip[iSkip].txp.rgia */
    goto LWriteZip;

L_7f8c:
    if ((i == 0xffff))
        goto L_8b82;
    else
        goto L_7f96;

L_7f96:
    /* untranslated: lptxp = &part[8:0](sel.fl.lpplord->rgord[sel.iwpAct]) */
    goto L_81cd;

L_7fd0:
    lptxp->rgia[0x4] = ((lptxp->rgia[0x4] & 0xfff) | 0x7000);
    i = 0x0;
    goto L_8024;

L_7fef:
    lptxp->rgia[i] = ((lptxp->rgia[i] & 0xfff) | 0x1000);
    i = (i + 0x1);

L_8024:
    if ((i < 0x3))
        goto L_7fef;
    else
        goto L_802e;

L_802e:
    lptxp->rgia[0x3] = ((lptxp->rgia[0x3] & 0xfff) | 0x0);
    goto LWriteZip;

L_8047:
    lptxp->rgia[0x4] = ((lptxp->rgia[0x4] & 0xfff) | 0x7000);
    i = 0x0;
    goto L_809b;

L_8066:
    lptxp->rgia[i] = ((lptxp->rgia[i] & 0xfff) | 0x2000);
    i = (i + 0x1);

L_809b:
    if ((i > 0x3))
        goto LWriteZip;
    else
        goto L_80a2;

L_80a2:

L_80a8:
    lptxp->rgia[0x4] = ((lptxp->rgia[0x4] & 0xfff) | 0x7000);
    i = 0x0;
    goto L_812c;

L_80c7:
    lptxp->rgia[i] = ((lptxp->rgia[i] & 0xfff) | 0x6000);
    lptxp->rgia[i] = ((lptxp->rgia[i] & 0xf000) | 0x64);
    i = (i + 0x1);

L_812c:
    if ((i < 0x3))
        goto L_80c7;
    else
        goto L_8136;

L_8136:
    lptxp->rgia[0x3] = ((lptxp->rgia[0x3] & 0xfff) | 0x0);
    goto LWriteZip;

L_814f:
    i = 0x0;
    goto L_81bd;

L_8158:
    lptxp->rgia[i] = ((lptxp->rgia[i] & 0xfff) | 0x0);
    lptxp->rgia[i] = ((lptxp->rgia[i] & 0xf000) | 0x0);
    i = (i + 0x1);

L_81bd:
    if ((i >= 0x5))
        goto LWriteZip;
    else
        goto L_81c4;

L_81c4:

L_81cd:
    if ((i == 0x0))
        goto L_7fd0;
    else
        goto L_81d5;

L_81d5:
    if ((i == 0x1))
        goto L_8047;
    else
        goto L_81dd;

L_81dd:
    if ((i == 0x2))
        goto L_80a8;
    else
        goto L_81e5;

L_81e5:
    if ((i == 0x3))
        goto L_814f;
    else
        goto LWriteZip;

LWriteZip:
    FLookupFleet(0xffff, 0x4972);
    UpdateOrdersDDs(0x1);
    DrawPlanShip(0x0, 0x100);

L_821f:
    GlobalPD.grPopup = grPopupShipOrders;
    Popup(hwndPlanet, pt.x, pt.y);

L_823a:
    /* untranslated: branch PtInRect(0x4910, words(pt.y, pt.x)) == 0x0 ? L_86ce : L_8254 */

L_8254:
    if ((fCursor == 0x0))
        goto L_8263;
    else
        goto L_825d;

L_825d:
    return hcurArrowHelp;

L_8263:
    if ((fRightBtn == 0x0))
        goto L_8697;
    else
        goto L_826c;

L_826c:
    iChecked = 0xffff;
    lpord = &(sel.fl.lpplord->rgord[sel.iwpAct]);
    /* untranslated: call FFindNearestObject(words(lpord->pt.y, lpord->pt.x), 0x8f, &scan) -> callresult(int16_t) */
    if ((scan.idpl == 0xffff))
        goto L_82d6;
    else
        goto L_82c6;

L_82c6:
    LOWORD(rgid) = scan.idpl;
    HIWORD(rgid) = SIGNHIWORD(scan.idpl);
    goto L_82e2;

L_82d6:
    LOWORD(rgid) = 0x0;
    HIWORD(rgid) = 0x1000;

L_82e2:
    /* untranslated: part[4:2](rgid) = 0xffff */
    /* untranslated: part[6:2](rgid) = 0xffff */
    c = 0x2;
    if ((lpord->grobj == grobjPlanet))
        goto L_8324;
    else
        goto L_830c;

L_830c:
    if ((lpord->grobj != grobjOther))
        goto L_832a;
    else
        goto L_8324;

L_8324:
    iChecked = 0x0;

L_832a:
    i = 0x0;
    goto L_8338;

L_8333:
    i = (i + 0x1);

L_8338:
    if ((i >= cFleet))
        goto L_8416;
    else
        goto L_8344;

L_8344:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_8377;
    else
        goto L_836f;

L_836f:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_8416;
    else
        goto L_8377;

L_8377:
    /* untranslated: branch ss:[scan] != lpfl->pt.x ? L_8333 : L_8393 */

L_8393:
    /* untranslated: branch ss:[scan+0x2] != lpfl->pt.y ? L_8333 : L_839c */

L_839c:
    if ((LOWORD(lpfl) == sel.fl.id))
        goto L_8333;
    else
        goto L_83ab;

L_83ab:
    if ((lpord->grobj != grobjFleet))
        goto L_83df;
    else
        goto L_83c3;

L_83c3:
    if ((lpord->id != LOWORD(lpfl)))
        goto L_83df;
    else
        goto L_83d7;

L_83d7:
    iChecked = c;

L_83df:
    c = (c + 0x1);
    LOWORD(rgid[c]) = (LOWORD(lpfl) | 0x0);
    HIWORD(rgid[c]) = (SIGNHIWORD(LOWORD(lpfl)) | 0x8000);
    if ((c >= 0x64))
        goto L_8416;
    else
        goto L_840d;

L_840d:

L_8416:
    if ((c != 0x2))
        goto L_8426;
    else
        goto L_8420;

L_8420:
    c = 0x1;

L_8426:
    if ((c != 0x0))
        goto L_8436;
    else
        goto L_8430;

L_8430:
    t_merge_8439_0001 = 0x1;
    goto L_8439;

L_8436:
    t_merge_8439_0001 = 0x0;

L_8439:
    fSep = t_merge_8439_0001;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_8509;

L_8470:
    /* untranslated: branch ss:[scan] != lpth->pt.x ? L_8504 : L_848c */

L_848c:
    /* untranslated: branch ss:[scan+0x2] != lpth->pt.y ? L_8504 : L_8495 */

L_8495:
    if ((fSep != 0x0))
        goto L_84ce;
    else
        goto L_849f;

L_849f:
    if ((c >= 0x64))
        goto L_851a;
    else
        goto L_84a6;

L_84a6:

L_84ac:
    c = (c + 0x1);
    rgid[c] = 0xffffffff;
    fSep = 0x1;

L_84ce:
    if ((c >= 0x64))
        goto L_851a;
    else
        goto L_84d5;

L_84d5:

L_84db:
    c = (c + 0x1);
    LOWORD(rgid[c]) = (LOWORD(lpth) | 0x0);
    HIWORD(rgid[c]) = 0x2000;

L_8504:
    lpth = (lpth + 0x1);

L_8509:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_8470;
    else
        goto L_851a;

L_851a:
    i = PopupMenu(hwndPlanet, pt.x, pt.y, c, rgid, 0x0, iChecked, 0x1);
    if ((i < 0x0))
        goto L_8b82;
    else
        goto L_854f;

L_854f:
    if ((i != 0x0))
        goto L_8591;
    else
        goto L_8559;

L_8559:
    if ((LOWORD(rgid) != 0x0))
        goto L_8591;
    else
        goto L_8563;

L_8563:
    if ((HIWORD(rgid) != 0x1000))
        goto L_8591;
    else
        goto L_856e;

L_856e:
    lpord->grobj = grobjOther;
    lpord->id = 0xffff;
    goto L_8673;

L_8591:
    if (((LOWORD(rgid[i]) & 0x0) != 0x0))
        goto L_85bb;
    else
        goto L_85b3;

L_85b3:
    if (((HIWORD(rgid[i]) & 0x2000) == 0x0))
        goto L_85ef;
    else
        goto L_85bb;

L_85bb:
    lpord->grobj = grobjThing;
    lpord->id = LOWORD(rgid[i]);
    goto L_8673;

L_85ef:
    if (((LOWORD(rgid[i]) & 0x0) != 0x0))
        goto L_8619;
    else
        goto L_8611;

L_8611:
    if (((HIWORD(rgid[i]) & 0x8000) == 0x0))
        goto L_864d;
    else
        goto L_8619;

L_8619:
    lpord->grobj = grobjFleet;
    lpord->id = LOWORD(rgid[i]);
    goto L_8673;

L_864d:
    lpord->grobj = grobjPlanet;
    lpord->id = LOWORD(rgid);

L_8673:
    FLookupFleet(0xffff, 0x4972);
    FillOrdersLB();
    SetOrdersLbSel(sel.iwpAct);

L_8697:
    GlobalPD.grPopup = grPopupString;
    HIWORD(GlobalPD) = 0xb4;
    /* untranslated: part[4:2](GlobalPD) = 0x4a6c */
    CchGetString(idsRightClickBlueDiamondBringPopupMenu, 0x4a6c);
    Popup(hwndPlanet, pt.x, pt.y);

L_86ce:
    if ((fRightBtn == 0x0))
        goto L_86e0;
    else
        goto L_86d7;

L_86d7:
    return 0x0;

L_86e0:
    /* untranslated: branch PtInRect(rgrcRef, words(pt.y, pt.x)) == 0x0 ? L_8769 : L_86fa */

L_86fa:
    if ((sel.grobj == grobjFleet))
        goto L_8713;
    else
        goto L_8704;

L_8704:
    if ((fCursor == 0x0))
        goto L_8713;
    else
        goto L_870d;

L_870d:
    return 0x0;

L_8713:
    irc = 0x0;
    lTempMax = 0xb;
    lMax = 0xb;
    /* untranslated: LOWORD(lCur) = ((part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) >> 0x4) & 0xf) */
    HIWORD(lCur) = 0x0;
    grbit = 0x6;
    goto L_8b82;

L_8769:
    /* untranslated: branch PtInRect(0x4928, words(pt.y, pt.x)) == 0x0 ? L_87d8 : L_8783 */

L_8783:
    irc = 0xf;
    iWarp = IWarpMAFromLppl(&(sel.pl), &(fTwoMAs));
    lTempMax = (uint32_t)((iWarp + 0xffff));
    lMax = (uint32_t)((iWarp + 0xffff));
    lTempMin = 0x1;
    LOWORD(lCur) = sel.pl.iWarpFling;
    HIWORD(lCur) = 0x0;
    goto L_8b82;

L_87d8:
    /* untranslated: branch PtInRect(0x48b8, words(pt.y, pt.x)) == 0x0 ? L_88fd : L_87f2 */

L_87f2:
    if ((sel.grobj != grobjFleet))
        goto L_880b;
    else
        goto L_87fc;

L_87fc:
    idPlan = sel.fl.idPlanet;
    iSkip = sel.fl.id;
    goto L_8816;

L_880b:
    iSkip = 0xffff;
    idPlan = sel.pl.id;

L_8816:
    lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0x0);
    FLookupOrbitingXfer(idPlan, LOWORD(lSel), &(xf), iSkip);
    if ((xf.grobj != grobjFleet))
        goto L_8865;
    else
        goto L_8859;

L_8859:
    /* untranslated: branch part[6:2](xf) == idPlayer ? L_886b : L_8865 */

L_8865:
    return 0x0;

L_886b:
    irc = 0x1;
    lMax = LGetFleetStat(&(xf.fl), 0x1);
    /* untranslated: LOWORD(lCur) = part[96:2](xf) */
    /* untranslated: HIWORD(lCur) = part[98:2](xf) */
    grbit = 0x4;
    if ((sel.grobj != grobjFleet))
        goto L_88ec;
    else
        goto L_88a6;

L_88a6:
    /* untranslated: LOWORD(lTempMax) = (LOWORD(lCur) + part[120:2](sel)) */
    /* untranslated: HIWORD(lTempMax) = (HIWORD(lCur) + part[122:2](sel)) */
    LGetFleetStat(&(sel.fl), 0x1);
    /* untranslated: LOWORD(lTempMin) = (LOWORD(lCur) - (loword(callresult(int32_t)) - part[120:2](sel))) */
    /* untranslated: HIWORD(lTempMin) = (HIWORD(lCur) - (hiword(callresult(int32_t)) - part[122:2](sel))) */
    goto L_8b82;

L_88ec:
    lTempMax = lCur;

L_88fd:
    /* untranslated: branch PtInRect(0x48c8, words(pt.y, pt.x)) == 0x0 ? L_8a21 : L_8917 */

L_8917:
    if ((fCursor == 0x0))
        goto L_8926;
    else
        goto L_8920;

L_8920:
    return hcurHand;

L_8926:
    if ((sel.fl.idPlanet == 0xffff))
        goto L_894f;
    else
        goto L_8930;

L_8930:
    TransferStuff(sel.fl.id, grobjFleet, sel.fl.idPlanet, grobjPlanet, 0x0);
    goto L_8a18;

L_894f:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_89c5;

L_8982:
    if ((lpth->ith != ithMineralPacket))
        goto L_89c0;
    else
        goto L_8999;

L_8999:
    if ((sel.fl.pt.x != lpth->pt.x))
        goto L_89c0;
    else
        goto L_89b4;

L_89b4:
    if ((sel.fl.pt.y == lpth->pt.y))
        goto L_89d6;
    else
        goto L_89ba;

L_89ba:

L_89c0:
    lpth = (lpth + 0x1);

L_89c5:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_8982;
    else
        goto L_89d6;

L_89d6:
    if ((LOWORD(lpth) != LOWORD(lpthMac)))
        goto L_8a0f;
    else
        goto L_89e7;

L_89e7:
    if ((HIWORD(lpth) != HIWORD(lpthMac)))
        goto L_8a0f;
    else
        goto L_89f0;

L_89f0:
    TransferStuff(sel.fl.id, grobjFleet, 0xffff, grobjOther, 0x0);
    goto L_8a18;

L_8a0f:
    MessageBeep(0x0);

L_8a18:
    return 0x0;

L_8a21:
    /* untranslated: branch PtInRect(0x48d0, words(pt.y, pt.x)) == 0x0 ? L_8ae0 : L_8a3b */

L_8a3b:
    if ((fCursor == 0x0))
        goto L_8a4a;
    else
        goto L_8a44;

L_8a44:
    return hcurHand;

L_8a4a:
    lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0x0);
    if ((LOWORD(lSel) != 0xffff))
        goto L_8a85;
    else
        goto L_8a75;

L_8a75:
    if ((HIWORD(lSel) != 0xffff))
        goto L_8a85;
    else
        goto L_8a7f;

L_8a7f:
    return 0x0;

L_8a85:
    if ((sel.grobj != grobjFleet))
        goto L_8a95;
    else
        goto L_8a8f;

L_8a8f:
    t_merge_8a98_0001 = sel.fl.id;
    goto L_8a98;

L_8a95:
    t_merge_8a98_0001 = 0xffff;

L_8a98:
    if ((FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &(xf), t_merge_8a98_0001) == 0x0))
        goto L_8ad7;
    else
        goto L_8abb;

L_8abb:
    TransferStuff(sel.id, sel.grobj, xf.id, xf.grobj, 0x0);

L_8ad7:
    return 0x0;

L_8ae0:
    /* untranslated: branch PtInRect(0x4940, words(pt.y, pt.x)) == 0x0 ? L_8b82 : L_8afa */

L_8afa:
    if ((sel.grobj == grobjFleet))
        goto L_8b0d;
    else
        goto L_8b04;

L_8b04:
    if ((fCursor != 0x0))
        goto L_8b34;
    else
        goto L_8b0d;

L_8b0d:
    /* untranslated: branch (part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) & 0xf) == 0x7 ? L_8b3a : L_8b34 */

L_8b34:
    return 0x0;

L_8b3a:
    irc = 0x12;
    lTempMax = 0xa;
    lMax = 0xa;
    /* untranslated: LOWORD(lCur) = part[8:2](sel.fl.lpplord->rgord[sel.iwpAct]) */
    HIWORD(lCur) = 0x0;
    grbit = 0x7;

L_8b82:
    if ((irc != 0xffff))
        goto L_8b95;
    else
        goto L_8b8c;

L_8b8c:
    return 0x0;

L_8b95:
    if ((fCursor == 0x0))
        goto L_8ba4;
    else
        goto L_8b9e;

L_8b9e:
    return hcurHand;

L_8ba4:
    dx = (uint32_t)(((rgrcRef[irc].right - rgrcRef[irc].left) + 0xfffe));
    /* untranslated: xRnd = (int32_t)((int32_t)(dx / words((HIWORD(lMax) + 0x0), (LOWORD(lMax) + 0x1))) >> 0x1) */
    hdc = GetDC(hwndPlanet);
    SetCapture(hwndPlanet);
    ptOld.y = 0xffff;
    ptOld.x = 0xffff;
    if ((HIWORD(lMax) > HIWORD(lTempMax)))
        goto L_8c3f;
    else
        goto L_8c29;

L_8c29:
    if ((HIWORD(lMax) < HIWORD(lTempMax)))
        goto L_8c36;
    else
        goto L_8c2e;

L_8c2e:
    if ((LOWORD(lMax) >= LOWORD(lTempMax)))
        goto L_8c3f;
    else
        goto L_8c36;

L_8c36:
    t_merge_8c47_0001_wide = lMax;
    goto L_8c47;

L_8c3f:
    t_merge_8c47_0001_wide = lTempMax;

L_8c47:
    lTempMax = t_merge_8c47_0001_wide;
    if ((0x0 < HIWORD(lTempMin)))
        goto L_8c75;
    else
        goto L_8c5e;

L_8c5e:
    if ((0x0 > HIWORD(lTempMin)))
        goto L_8c6c;
    else
        goto L_8c63;

L_8c63:
    if ((0x0 <= LOWORD(lTempMin)))
        goto L_8c75;
    else
        goto L_8c6c;

L_8c6c:
    t_merge_8c7d_0001_wide = 0x0;
    goto L_8c7d;

L_8c75:
    t_merge_8c7d_0001_wide = lTempMin;

L_8c7d:
    lTempMin = t_merge_8c7d_0001_wide;
    fFirst = 0x1;

L_8c8b:
    if ((fFirst != 0x0))
        goto L_8ca9;
    else
        goto L_8c95;

L_8c95:
    if ((FGetMouseMove(&(pt)) == 0x0))
        goto L_8f9c;
    else
        goto L_8ca9;

L_8ca9:
    fFirst = 0x0;
    if ((pt.x != ptOld.x))
        goto L_8cc5;
    else
        goto L_8cba;

L_8cba:
    if ((pt.y == ptOld.y))
        goto L_8c8b;
    else
        goto L_8cc5;

L_8cc5:
    ptOld.x = pt.x;
    ptOld.y = pt.y;
    /* untranslated: lNew = (int32_t)((uint32_t)(words((signhiword((pt.x - rgrcRef[irc].left)) + HIWORD(xRnd)), ((pt.x - rgrcRef[irc].left) + LOWORD(xRnd))) *
     * lMax) / dx) */
    if ((HIWORD(lNew) > HIWORD(lTempMax)))
        goto L_8d37;
    else
        goto L_8d1e;

L_8d1e:
    if ((HIWORD(lNew) < HIWORD(lTempMax)))
        goto L_8d2c;
    else
        goto L_8d23;

L_8d23:
    if ((LOWORD(lNew) >= LOWORD(lTempMax)))
        goto L_8d37;
    else
        goto L_8d2c;

L_8d2c:
    t_merge_8d3f_0003_wide = lNew;
    goto L_8d3f;

L_8d37:
    t_merge_8d3f_0003_wide = lTempMax;

L_8d3f:
    if ((HIWORD(lTempMin) < HIWORD(t_merge_8d3f_0003_wide)))
        goto L_8d61;
    else
        goto L_8d48;

L_8d48:
    if ((HIWORD(lTempMin) > HIWORD(t_merge_8d3f_0003_wide)))
        goto L_8d56;
    else
        goto L_8d4d;

L_8d4d:
    if ((LOWORD(lTempMin) <= LOWORD(t_merge_8d3f_0003_wide)))
        goto L_8d61;
    else
        goto L_8d56;

L_8d56:
    t_merge_8d93_0001_wide = lTempMin;
    goto L_8d93;

L_8d61:
    if ((HIWORD(lNew) > HIWORD(lTempMax)))
        goto L_8d8b;
    else
        goto L_8d72;

L_8d72:
    if ((HIWORD(lNew) < HIWORD(lTempMax)))
        goto L_8d80;
    else
        goto L_8d77;

L_8d77:
    if ((LOWORD(lNew) >= LOWORD(lTempMax)))
        goto L_8d8b;
    else
        goto L_8d80;

L_8d80:
    t_merge_8d93_0001_wide = lNew;
    goto L_8d93;

L_8d8b:
    t_merge_8d93_0001_wide = lTempMax;

L_8d93:
    lNew = t_merge_8d93_0001_wide;
    if ((LOWORD(lNew) != LOWORD(lCur)))
        goto L_8db3;
    else
        goto L_8daa;

L_8daa:
    if ((HIWORD(lNew) == HIWORD(lCur)))
        goto L_8c8b;
    else
        goto L_8db3;

L_8db3:
    if ((irc != 0x0))
        goto L_8e38;
    else
        goto L_8dbd;

L_8dbd:
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) = ((part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) & 0xff0f) | ((LOWORD(lNew) & 0xf) * 0x10)) */
    DrawPlanShip(0x0, 0x4020);
    goto L_8f8b;

L_8e38:
    if ((irc != 0x12))
        goto L_8e81;
    else
        goto L_8e42;

L_8e42:
    /* untranslated: part[8:2](sel.fl.lpplord->rgord[sel.iwpAct]) = LOWORD(lNew) */
    DrawPlanShip(0x0, 0x4100);
    goto L_8f8b;

L_8e81:
    if ((irc != 0xf))
        goto L_8ebf;
    else
        goto L_8e8b;

L_8e8b:
    if ((fTwoMAs == 0x0))
        goto L_8ea9;
    else
        goto L_8ea1;

L_8ea1:
    /* untranslated: t_merge_8eac_0001 = (iWarp neg 0x0) */
    goto L_8eac;

L_8ea9:
    t_merge_8eac_0001 = iWarp;

L_8eac:
    DrawMassWarpGauge(hdc, 0x4928, t_merge_8eac_0001, (LOWORD(lNew) + 0x4));
    goto L_8f8b;

L_8ebf:
    if ((irc != 0x2))
        goto L_8f01;
    else
        goto L_8ec9;

L_8ec9:
    /* untranslated: part[120:2](sel) = LOWORD(lNew) */
    /* untranslated: part[122:2](sel) = HIWORD(lNew) */
    DrawFleetGauge(hdc, rgrcRef[irc], 0x0, grbit);
    goto L_8f8b;

L_8f01:
    if ((irc != 0x1))
        goto L_8f8b;
    else
        goto L_8f0b;

L_8f0b:
    if ((sel.grobj != grobjFleet))
        goto L_8f47;
    else
        goto L_8f15;

L_8f15:
    /* untranslated: part[120:2](sel) = (part[120:2](sel) - (LOWORD(lNew) - LOWORD(lCur))) */
    /* untranslated: part[122:2](sel) = (part[122:2](sel) - (HIWORD(lNew) - HIWORD(lCur))) */
    DrawFleetGauge(hdc, 0x48c0, &(sel.fl), grbit);
    goto L_8f57;

L_8f47:
    DrawPlanShip(0x0, 0x4001);

L_8f57:
    /* untranslated: part[96:2](xf) = LOWORD(lNew) */
    /* untranslated: part[98:2](xf) = HIWORD(lNew) */
    DrawFleetGauge(hdc, rgrcRef[irc], &(xf.fl), grbit);

L_8f8b:
    lCur = lNew;

L_8f9c:
    if ((sel.scan.grobj != grobjOther))
        goto L_8fac;
    else
        goto L_8fa6;

L_8fa6:
    t_merge_8faf_0001 = sel.scan.grobjFull;
    goto L_8faf;

L_8fac:
    t_merge_8faf_0001 = sel.scan.grobj;

L_8faf:
    grbit = t_merge_8faf_0001;
    if ((irc != 0x2))
        goto L_9056;
    else
        goto L_8fbc;

L_8fbc:
    FLookupFleet(0xffff, 0x4972);
    FLookupPlanet(0xffff, &(pl));
    DrawPlanShip(0x0, 0x4220);
    if (((grbit & 0x1) == 0x0))
        goto L_900e;
    else
        goto L_8ffa;

L_8ffa:
    if ((sel.fl.idPlanet != sel.scan.idpl))
        goto L_900e;
    else
        goto FixMinWin;

FixMinWin:
    InvalidateMineralBars();
    goto L_914e;

L_900e:
    if (((grbit & 0x2) == 0x0))
        goto L_914e;
    else
        goto L_901c;

L_901c:
    if ((sel.fl.id != LOWORD(rglpfl[sel.scan.ifl])))
        goto L_914e;
    else
        goto L_903e;

L_903e:
    InvalidateRect(hwndMine, 0x0, 0x1);

L_9056:
    if ((irc != 0x1))
        goto L_90ce;
    else
        goto L_9060;

L_9060:
    FLookupFleet(0xffff, &(xf.fl));
    if ((sel.grobj != grobjFleet))
        goto L_909e;
    else
        goto L_907b;

L_907b:
    FLookupFleet(0xffff, 0x4972);
    DrawPlanShip(0x0, 0x4221);
    goto L_914e;

L_909e:
    FLookupPlanet(0xffff, 0x49ee);
    if (((grbit & 0x1) == 0x0))
        goto L_914e;
    else
        goto L_90bc;

L_90bc:
    if ((sel.pl.id == sel.scan.idpl))
        goto FixMinWin;
    else
        goto L_90c5;

L_90c5:

L_90ce:
    if ((irc == 0x0))
        goto L_90e2;
    else
        goto L_90d8;

L_90d8:
    if ((irc != 0x12))
        goto L_90f5;
    else
        goto L_90e2;

L_90e2:
    FLookupFleet(0xffff, 0x4972);
    goto L_914e;

L_90f5:
    if ((irc != 0xf))
        goto L_914e;
    else
        goto L_90ff;

L_90ff:
    if ((LOWORD(lCur) == sel.pl.iWarpFling))
        goto L_914e;
    else
        goto L_9118;

L_9118:
    /* untranslated: part[198:2](sel) = ((part[198:2](sel) & 0xc3ff) | ((LOWORD(lCur) & 0xf) << 0xa)) */
    FLookupPlanet(0xffff, 0x49ee);

L_914e:
    ReleaseCapture();
    return ReleaseDC(hwndPlanet, hdc);
}

void FillFleetCompLB() {
    int16_t  i;
    int32_t  pctDmg;
    uint16_t t_merge_922e_0001;
    uint16_t t_merge_924a_0001;

L_9166:
    SendMessage(hwndFleetCompLB, CB_DIR, 0x0, 0x0);
    i = 0x0;
    goto L_927d;

L_9190:
    if ((sel.fl.rgcsh[i] <= 0x0))
        goto L_9279;
    else
        goto L_91a2;

L_91a2:
    /* untranslated: pctDmg = (int32_t)(words((hiword((uint32_t)(words(0x0, (part[18846:2](mpicolgrbitBU[i]) & 0x7f)) * words(0x0,
     * ((part[18846:2](mpicolgrbitBU[i]) >> 0x7) & 0x1ff)))) + 0x0), (loword((uint32_t)(words(0x0, (part[18846:2](mpicolgrbitBU[i]) & 0x7f)) * words(0x0,
     * ((part[18846:2](mpicolgrbitBU[i]) >> 0x7) & 0x1ff)))) + 0xfa)) / 0x1f4) */
    if ((LOWORD(pctDmg) != 0x0))
        goto L_9221;
    else
        goto L_9218;

L_9218:
    if ((HIWORD(pctDmg) == 0x0))
        goto L_922b;
    else
        goto L_9221;

L_9221:
    t_merge_922e_0001 = (uint16_t)(LOBYTE(LOWORD(pctDmg)));
    goto L_922e;

L_922b:
    t_merge_922e_0001 = 0x20;

L_922e:
    if ((LOWORD(pctDmg) != 0x0))
        goto L_9241;
    else
        goto L_9238;

L_9238:
    if ((HIWORD(pctDmg) == 0x0))
        goto L_9247;
    else
        goto L_9241;

L_9241:
    t_merge_924a_0001 = 0x50;
    goto L_924a;

L_9247:
    t_merge_924a_0001 = 0x51;

L_924a:
    /* untranslated: call _wsprintf(szWork, "%c%c%5d%s", t_merge_924a_0001, t_merge_922e_0001, sel.fl.rgcsh[i], part[8:0](rgshdef[i].hul), 0x25) ->
     * callresult(int16_t) */
    SendMessage(hwndFleetCompLB, CB_LIMITTEXT, 0x0, 0x2557a4);

L_9279:
    i = (i + 0x1);

L_927d:
    if ((i < 0x10))
        goto L_9190;
    else
        goto L_9286;

L_9286:
    return;
}

void FillOrdersLB() {
    int16_t i;
    char   *psz;
    ORDER   ord;

L_928c:
    SendMessage(hwndShipLB, CB_DIR, 0x0, 0x0);
    i = 0x0;
    goto L_92ba;

L_92b6:
    i = (i + 0x1);

L_92ba:
    if ((i >= sel.fl.cord))
        goto L_9328;
    else
        goto L_92c5;

L_92c5:
    ord = sel.fl.lpplord->rgord[i];
    psz = PszGetLocName(ord.grobj, ord.id, ord.pt.x, ord.pt.y);
    /* untranslated: call SendMessage(hwndShipLB, CB_LIMITTEXT, 0x0, words(0x25, psz)) -> callresult(LRESULT) */
    goto L_92b6;

L_9328:
    SetOrdersLbSel(sel.iwpAct);
    if ((sel.grobj != grobjFleet))
        goto L_934e;
    else
        goto L_933e;

L_933e:
    DrawPlanShip(0x0, 0x120);

L_934e:
    return;
}

void SetOrdersLbSel(int16_t iSel) {
    uint16_t t_merge_9393_0001;
    uint16_t t_merge_93c0_0001;

L_9354:
    SendMessage(hwndShipLB, CB_GETCURSEL, iSel, 0x0);
    if ((gd.fSmallTileMode == 0x0))
        goto L_9390;
    else
        goto L_938a;

L_938a:
    t_merge_9393_0001 = 0x1;
    goto L_9393;

L_9390:
    t_merge_9393_0001 = 0x2;

L_9393:
    if ((iSel <= t_merge_9393_0001))
        goto L_93d3;
    else
        goto L_939b;

L_939b:
    if ((gd.fSmallTileMode == 0x0))
        goto L_93bd;
    else
        goto L_93b7;

L_93b7:
    t_merge_93c0_0001 = 0x1;
    goto L_93c0;

L_93bd:
    t_merge_93c0_0001 = 0x2;

L_93c0:
    SendMessage(hwndShipLB, CB_FINDSTRINGEXACT, (iSel - t_merge_93c0_0001), 0x0);

L_93d3:
    UpdateWindow(hwndShipLB);
    UpdateOrdersDDs(0x0);
    return;
}

void UpdateOrdersDDs(int16_t iLevel) {
    int32_t  rglSel[3];
    int16_t  iMin;
    int16_t  i;
    char    *psz;
    int16_t  iSel;
    int16_t  iMax;
    char     szT[80];
    uint16_t t_merge_94c2_0001;
    uint16_t t_merge_977d_0001;

L_93ee:
    iSel = 0xffff;
    if ((iLevel != 0x0))
        goto L_9443;
    else
        goto L_9405;

L_9405:
    /* untranslated: call SendMessage(LOWORD(rghwndOrderDD), CB_SETCURSEL, (part[6:2](sel.fl.lpplord->rgord[sel.iwpAct]) & 0xf), 0x0) -> callresult(LRESULT) */
    /* untranslated: LOWORD(rglSel) = loword(callresult(LRESULT)) */
    /* untranslated: HIWORD(rglSel) = hiword(callresult(LRESULT)) */
    goto L_9462;

L_9443:
    SendMessage(LOWORD(rghwndOrderDD), CB_GETCURSEL, 0x0, 0x0);
    /* untranslated: LOWORD(rglSel) = loword(callresult(LRESULT)) */
    /* untranslated: HIWORD(rglSel) = hiword(callresult(LRESULT)) */

L_9462:
    if ((iLevel > 0x1))
        goto L_9824;
    else
        goto DoMinerals;

DoMinerals:
    SendMessage(HIWORD(rghwndOrderDD), CB_RESETCONTENT, 0x0, 0x0);
    if ((LOWORD(rglSel) != 0x1))
        goto L_95b6;
    else
        goto L_948d;

L_948d:
    if ((HIWORD(rglSel) != 0x0))
        goto L_95b6;
    else
        goto L_9496;

L_9496:
    if ((LOWORD(LGetFleetStat(&(sel.fl), 0x2)) != 0x0))
        goto L_94b9;
    else
        goto L_94b1;

L_94b1:
    /* untranslated: branch hiword(callresult(int32_t)) == 0x0 ? L_94bf : L_94b9 */

L_94b9:
    t_merge_94c2_0001 = 0x5;
    goto L_94c2;

L_94bf:
    t_merge_94c2_0001 = 0x1;

L_94c2:
    iMax = t_merge_94c2_0001;
    i = 0x0;
    goto L_956b;

L_94cd:
    if ((i != 0x0))
        goto L_94de;
    else
        goto L_94d6;

L_94d6:
    iMin = 0x4;
    goto L_94e7;

L_94de:
    iMin = (i + 0xffff);

L_94e7:
    strcpy(0x57a5, rgszMinerals[iMin]);
    /* untranslated: branch ((HIWORD(sel.fl.lpplord):[((((LOWORD(sel.fl.lpplord) + 0x4) + loword((0x12 * sel.iwpAct))) + 0x8) + (iMin * 0x2))] >> 0xc) & 0xf) ==
     * 0x0 ? L_954a : L_9533 */

L_9533:
    /* untranslated: part[0:1](szWork) = 0x2a */
    if ((iSel != 0xffff))
        goto L_954f;
    else
        goto L_9541;

L_9541:
    iSel = iMin;

L_954a:
    /* untranslated: part[0:1](szWork) = 0x20 */

L_954f:
    SendMessage(HIWORD(rghwndOrderDD), CB_ADDSTRING, 0x0, 0x2557a4);
    i = (i + 0x1);

L_956b:
    if ((i < iMax))
        goto L_94cd;
    else
        goto L_9576;

L_9576:
    if ((iSel == 0xffff))
        goto L_9588;
    else
        goto L_957f;

L_957f:
    if ((iSel != 0x4))
        goto L_9590;
    else
        goto L_9588;

L_9588:
    iSel = 0x0;
    goto L_9594;

L_9590:
    iSel = (iSel + 0x1);

L_9594:
    SendMessage(HIWORD(rghwndOrderDD), CB_SETCURSEL, iSel, 0x0);
    /* untranslated: part[4:2](rglSel) = loword(callresult(LRESULT)) */
    /* untranslated: part[6:2](rglSel) = hiword(callresult(LRESULT)) */
    goto L_987f;

L_95b6:
    if ((LOWORD(rglSel) != 0x7))
        goto L_968c;
    else
        goto L_95bf;

L_95bf:
    if ((HIWORD(rglSel) != 0x0))
        goto L_968c;
    else
        goto L_95c8;

L_95c8:
    psz = PszGetCompressedString(idsWithinDLY);
    i = 0x0;
    goto L_961b;

L_95df:
    _wsprintf(szWork, psz, (LOWORD((0x32 * i)) + 0x32));
    SendMessage(HIWORD(rghwndOrderDD), CB_ADDSTRING, 0x0, 0x2557a4);
    i = (i + 0x1);

L_961b:
    if ((i < 0xb))
        goto L_95df;
    else
        goto L_9624;

L_9624:
    psz = PszGetCompressedString(idsAnyEnemy);
    /* untranslated: call SendMessage(HIWORD(rghwndOrderDD), CB_ADDSTRING, 0x0, words(0x25, psz)) -> callresult(LRESULT) */
    /* untranslated: iSel = part[10:2](sel.fl.lpplord->rgord[sel.iwpAct]) */
    SendMessage(HIWORD(rghwndOrderDD), CB_SETCURSEL, iSel, 0x0);
    /* untranslated: part[4:2](rglSel) = loword(callresult(LRESULT)) */
    /* untranslated: part[6:2](rglSel) = hiword(callresult(LRESULT)) */
    goto L_987f;

L_968c:
    if ((LOWORD(rglSel) != 0x9))
        goto L_9751;
    else
        goto L_9695;

L_9695:
    if ((HIWORD(rglSel) != 0x0))
        goto L_9751;
    else
        goto L_969e;

L_969e:
    /* untranslated: part[0:1](szT) = 0x20 */
    i = 0x0;
    goto L_96ae;

L_96aa:
    i = (i + 0x1);

L_96ae:
    if ((i >= game.cPlayer))
        goto L_9710;
    else
        goto L_96b9;

L_96b9:
    if ((i == idPlayer))
        goto L_96aa;
    else
        goto L_96c4;

L_96c4:
    psz = PszPlayerName(i, 0x1, 0x1, 0x1, 0x0, 0x0);
    /* untranslated: call strcpy(&part[1:2](szT), psz) -> callresult(char *) */
    /* untranslated: call SendMessage(HIWORD(rghwndOrderDD), CB_ADDSTRING, 0x0, words(ss, szT)) -> callresult(LRESULT) */

L_9710:
    /* untranslated: iSel = part[8:2](sel.fl.lpplord->rgord[sel.iwpAct]) */
    SendMessage(HIWORD(rghwndOrderDD), CB_SETCURSEL, iSel, 0x0);
    /* untranslated: part[4:2](rglSel) = loword(callresult(LRESULT)) */
    /* untranslated: part[6:2](rglSel) = hiword(callresult(LRESULT)) */
    goto L_987f;

L_9751:
    if ((LOWORD(rglSel) != 0x6))
        goto L_987f;
    else
        goto L_975a;

L_975a:
    if ((HIWORD(rglSel) != 0x0))
        goto L_987f;
    else
        goto L_9763;

L_9763:
    i = 0x0;
    goto L_97c0;

L_976b:
    if ((i != 0x0))
        goto L_977a;
    else
        goto L_9774;

L_9774:
    t_merge_977d_0001 = 0x20;
    goto L_977d;

L_977a:
    t_merge_977d_0001 = 0x73;

L_977d:
    _wsprintf(szWork, PszGetCompressedString(idsDYearC), (i + 0x1), t_merge_977d_0001);
    SendMessage(HIWORD(rghwndOrderDD), CB_ADDSTRING, 0x0, 0x2557a4);
    i = (i + 0x1);

L_97c0:
    if ((i < 0x5))
        goto L_976b;
    else
        goto L_97c9;

L_97c9:
    /* untranslated: call SendMessage(HIWORD(rghwndOrderDD), CB_ADDSTRING, 0x0, words(0x25, PszGetCompressedString(idsIindefinitely))) -> callresult(LRESULT) */
    /* untranslated: call SendMessage(HIWORD(rghwndOrderDD), CB_SETCURSEL, part[8:2](sel.fl.lpplord->rgord[sel.iwpAct]), 0x0) -> callresult(LRESULT) */
    /* untranslated: part[4:2](rglSel) = loword(callresult(LRESULT)) */
    /* untranslated: part[6:2](rglSel) = hiword(callresult(LRESULT)) */

L_9824:
    SendMessage(HIWORD(rghwndOrderDD), CB_GETCURSEL, 0x0, 0x0);
    /* untranslated: part[4:2](rglSel) = loword(callresult(LRESULT)) */
    /* untranslated: part[6:2](rglSel) = hiword(callresult(LRESULT)) */
    if ((LOWORD(rglSel) != 0x1))
        goto L_987f;
    else
        goto L_984c;

L_984c:
    if ((HIWORD(rglSel) != 0x0))
        goto L_987f;
    else
        goto L_9855;

L_9855:
    if ((iLevel > 0x3))
        goto L_987f;
    else
        goto L_985e;

L_985e:
    /* untranslated: iSel = part[4:2](rglSel) */
    if ((iSel != 0x0))
        goto L_9878;
    else
        goto L_9870;

L_9870:
    iSel = 0x4;
    goto DoMinerals;

L_9878:
    iSel = (iSel - 0x1);

L_987f:
    if ((iLevel > 0x2))
        goto L_9987;
    else
        goto L_9888;

L_9888:
    /* untranslated: call SendMessage(part[4:2](rghwndOrderDD), CB_RESETCONTENT, 0x0, 0x0) -> callresult(LRESULT) */
    if ((LOWORD(rglSel) != 0x1))
        goto L_99a6;
    else
        goto L_98aa;

L_98aa:
    if ((HIWORD(rglSel) != 0x0))
        goto L_99a6;
    else
        goto L_98b3;

L_98b3:
    i = 0x6d;
    goto L_9912;

L_98bb:
    if ((i != 0x74))
        goto L_98e8;
    else
        goto L_98c4;

L_98c4:
    /* untranslated: branch part[4:2](rglSel) != 0x0 ? L_98e8 : L_98cd */

L_98cd:
    /* untranslated: branch part[6:2](rglSel) != 0x0 ? L_98e8 : L_98d6 */

L_98d6:
    psz = PszGetCompressedString(idsLoadOptimal);
    goto L_98f6;

L_98e8:
    psz = PszGetCompressedString(i);

L_98f6:
    /* untranslated: call SendMessage(part[4:2](rghwndOrderDD), CB_ADDSTRING, 0x0, words(0x25, psz)) -> callresult(LRESULT) */
    i = (i + 0x1);

L_9912:
    if ((i < 0x77))
        goto L_98bb;
    else
        goto L_991b;

L_991b:
    /* untranslated: iSel = part[4:2](rglSel) */
    if ((iSel != 0x0))
        goto L_9935;
    else
        goto L_992d;

L_992d:
    iSel = 0x4;
    goto L_9939;

L_9935:
    iSel = (iSel - 0x1);

L_9939:
    /* untranslated: call SendMessage(part[4:2](rghwndOrderDD), CB_SETCURSEL, ((HIWORD(sel.fl.lpplord):[((((LOWORD(sel.fl.lpplord) + 0x4) + loword((0x12 *
     * sel.iwpAct))) + 0x8) + (iSel * 0x2))] >> 0xc) & 0xf), 0x0) -> callresult(LRESULT) */
    /* untranslated: part[8:2](rglSel) = loword(callresult(LRESULT)) */
    /* untranslated: part[10:2](rglSel) = hiword(callresult(LRESULT)) */

L_9987:
    /* untranslated: call SendMessage(part[4:2](rghwndOrderDD), CB_GETCURSEL, 0x0, 0x0) -> callresult(LRESULT) */
    /* untranslated: part[8:2](rglSel) = loword(callresult(LRESULT)) */
    /* untranslated: part[10:2](rglSel) = hiword(callresult(LRESULT)) */

L_99a6:
    if ((iLevel > 0x3))
        goto L_9a30;
    else
        goto L_99af;

L_99af:
    if ((LOWORD(rglSel) != 0x1))
        goto L_9a30;
    else
        goto L_99b8;

L_99b8:
    if ((HIWORD(rglSel) != 0x0))
        goto L_9a30;
    else
        goto L_99c1;

L_99c1:
    /* untranslated: iSel = part[4:2](rglSel) */
    if ((iSel != 0x0))
        goto L_99db;
    else
        goto L_99d3;

L_99d3:
    iSel = 0x4;
    goto L_99df;

L_99db:
    iSel = (iSel - 0x1);

L_99df:
    /* untranslated: call _wsprintf(szWork, "%u", (HIWORD(sel.fl.lpplord):[((((LOWORD(sel.fl.lpplord) + 0x4) + loword((0x12 * sel.iwpAct))) + 0x8) + (iSel *
     * 0x2))] & 0xfff)) -> callresult(int16_t) */
    SetWindowText(hwndOrderED, szWork);

L_9a30:
    return;
}

void FillBattleDD(int16_t iSel) {
    int16_t i;

L_9a36:
    SendMessage(hwndBattleDD, CB_RESETCONTENT, 0x0, 0x0);
    CchGetString(idsBattlePlans, 0x57a4);
    SendMessage(hwndBattleDD, CB_ADDSTRING, 0x0, 0x2557a4);
    i = 0x0;
    goto L_9a8c;

L_9a88:
    i = (i + 0x1);

L_9a8c:
    /* untranslated: branch i >= part[18480:1](idPlayer) ? L_9ae8 : L_9a9f */

L_9a9f:
    /* untranslated: call fstrcpy(szWork, &part[4:0](rglpbtlplan[idPlayer][i])) -> callresult(char *) */
    SendMessage(hwndBattleDD, CB_ADDSTRING, 0x0, 0x2557a4);
    goto L_9a88;

L_9ae8:
    SendMessage(hwndBattleDD, CB_SETCURSEL, iSel, 0x0);
    return;
}

void DeleteCurWayPoint(int16_t fBackup) {
    POINT   pt;
    POINT   rgpt[3];
    int16_t cpt;
    SCAN    scan;
    int16_t ipt;
    RECT    rc;

L_9b08:
    if ((sel.fl.cord < 0x2))
        goto L_9b25;
    else
        goto L_9b1b;

L_9b1b:
    if ((sel.iwpAct != 0x0))
        goto L_9b31;
    else
        goto L_9b25;

L_9b25:
    MessageBeep(0x40);
    goto L_9e21;

L_9b31:
    if (((grbitScan & 0x80) == 0x0))
        goto L_9bd2;
    else
        goto L_9b3f;

L_9b3f:
    LOWORD(rgpt) = sel.fl.lpplord->rgord[sel.iwpAct].pt.x;
    HIWORD(rgpt) = sel.fl.lpplord->rgord[sel.iwpAct].pt.y;
    /* untranslated: part[4:2](rgpt) = sel.fl.lpplord->rgord[(sel.iwpAct - 0x1)].pt.x */
    /* untranslated: part[6:2](rgpt) = sel.fl.lpplord->rgord[(sel.iwpAct - 0x1)].pt.y */
    if ((sel.iwpAct >= (sel.fl.cord + 0xffff)))
        goto L_9bcd;
    else
        goto L_9b9c;

L_9b9c:
    cpt = 0x3;
    /* untranslated: part[8:2](rgpt) = sel.fl.lpplord->rgord[(sel.iwpAct + 0x1)].pt.x */
    /* untranslated: part[10:2](rgpt) = sel.fl.lpplord->rgord[(sel.iwpAct + 0x1)].pt.y */
    goto L_9bd2;

L_9bcd:
    cpt = 0x2;

L_9bd2:
    RedrawScanSel(0x0, 0x0);
    fmemmove(&(sel.fl.lpplord->rgord[sel.iwpAct]), &(sel.fl.lpplord->rgord[(sel.iwpAct + 0x1)]), LOWORD((((sel.fl.cord - sel.iwpAct) + 0xffff) * 0x12)));
    sel.fl.cord = (sel.fl.cord - 0x1);
    sel.fl.lpplord->iordMac = (sel.fl.lpplord->iordMac - 0x1);
    sel.iwpAct = (sel.iwpAct - 0x1);
    if ((sel.iwpAct >= (sel.fl.cord + 0xffff)))
        goto L_9d3e;
    else
        goto L_9c68;

L_9c68:
    pt.x = sel.fl.lpplord->rgord[sel.iwpAct].pt.x;
    pt.y = sel.fl.lpplord->rgord[sel.iwpAct].pt.y;
    /* untranslated: branch ss:[pt] != sel.fl.lpplord->rgord[(sel.iwpAct + 0x1)].pt.x ? L_9d3e : L_9cbf */

L_9cbf:
    /* untranslated: branch ss:[pt+0x2] != sel.fl.lpplord->rgord[(sel.iwpAct + 0x1)].pt.y ? L_9d3e : L_9cc8 */

L_9cc8:
    fmemmove(&(sel.fl.lpplord->rgord[(sel.iwpAct + 0x1)]), &(sel.fl.lpplord->rgord[(sel.iwpAct + 0x2)]),
             LOWORD((((sel.fl.cord - sel.iwpAct) + 0xfffe) * 0x12)));
    sel.fl.cord = (sel.fl.cord - 0x1);
    sel.fl.lpplord->iordMac = (sel.fl.lpplord->iordMac - 0x1);

L_9d3e:
    if ((fBackup != 0x0))
        goto L_9d5b;
    else
        goto L_9d47;

L_9d47:
    if ((sel.iwpAct >= (sel.fl.cord + 0xffff)))
        goto L_9d5b;
    else
        goto L_9d56;

L_9d56:
    sel.iwpAct = (sel.iwpAct + 0x1);

L_9d5b:
    RedrawScanSel(0x0, 0x0);
    FLookupFleet(0xffff, 0x4972);
    /* untranslated: call FFindNearestObject(words(sel.fl.lpplord->rgord[sel.iwpAct].pt.y, sel.fl.lpplord->rgord[sel.iwpAct].pt.x), 0x8f, &scan) ->
     * callresult(int16_t) */
    sel.iwpAct = 0xfffe;
    ChangeScanSel(&(scan), 0x1);
    if (((grbitScan & 0x80) == 0x0))
        goto L_9e21;
    else
        goto L_9dce;

L_9dce:
    ipt = 0x0;
    goto L_9def;

L_9dd6:
    LogicalToScan(rgpt[ipt]);
    ipt = (ipt + 0x1);

L_9def:
    if ((ipt < cpt))
        goto L_9dd6;
    else
        goto L_9dfa;

L_9dfa:
    BoundPoints(&(rc), rgpt, cpt);
    InvalidateRect(hwndScanner, &(rc), 0x1);

L_9e21:
    return;
}

void DeleteWpFar(FLEET *lpfl, int16_t iDel, int16_t fRecycle) {
    ORDER ord;

L_9e28:
    if ((fRecycle == 0x0))
        goto L_9ef2;
    else
        goto L_9e3a;

L_9e3a:
    if ((iDel == 0x56))
        goto L_9ebf;
    else
        goto L_9e43;

L_9e43:
    if ((lpfl->cord == 0x2))
        goto L_9ebf;
    else
        goto L_9e50;

L_9e50:
    if ((lpfl->lpplord->rgord[(lpfl->cord - 0x1)].pt.x != lpfl->lpplord->rgord[iDel].pt.x))
        goto L_9ec7;
    else
        goto L_9eb6;

L_9eb6:
    /* untranslated: branch ss:[bp-0x16]:[(ss:[bp-0x18] + loword(((lpfl->cord + 0xffff) * 0x12)))+0x2] != ss:[bp-0x1a] ? L_9ec7 : L_9ebf */

L_9ebf:
    fRecycle = 0x0;
    goto L_9ef2;

L_9ec7:
    ord = lpfl->lpplord->rgord[iDel];

L_9ef2:
    fmemmove(&(lpfl->lpplord->rgord[iDel]), &(lpfl->lpplord->rgord[(iDel + 0x1)]), LOWORD((((lpfl->cord - iDel) + 0xffff) * 0x12)));
    if ((fRecycle == 0x0))
        goto L_9fca;
    else
        goto L_9f69;

L_9f69:
    lpfl->lpplord->rgord[(lpfl->cord - 0x1)] = ord;
    goto L_9fde;

L_9fca:
    lpfl->cord = (lpfl->cord - 0x1);
    lpfl->lpplord->iordMac = (lpfl->lpplord->iordMac - 0x1);

L_9fde:
    return;
}

int32_t EstFuelUse(FLEET *lpfl, int16_t iOrd, int16_t iWarp, int32_t dTravel, int16_t fRangeOnly) {
    int32_t iEffNext;
    int32_t lT;
    int16_t fEfficient;
    double  d;
    int32_t iEffCur;
    int32_t wtCargoT;
    int32_t lFuel;
    ORDER  *lpord;
    int16_t i;
    SHDEF  *lpshdef;
    int32_t wtCargo;
    int16_t j;
    int32_t wtMass;
    int32_t rgieff[16];
    int32_t t_merge_a434_0001_wide;

L_9fe4:
    iEffCur = 0x0;
    gd.fRadiatingEngine = 0x0;
    if ((iWarp != 0xffff))
        goto L_a03d;
    else
        goto L_a00c;

L_a00c:
    /* untranslated: iWarp = ((part[6:2](lpfl->lpplord->rgord[(iOrd + 0x1)]) >> 0x4) & 0xf) */

L_a03d:
    fEfficient = GetRaceGrbit(rgplr[lpfl->iPlayer], ibitRaceIFE);
    i = 0x0;
    lpshdef = rglpshdef[lpfl->iPlayer];
    goto L_a091;

L_a07d:
    i = (i + 0x1);
    /* untranslated: LOWORD(lpshdef) = part[3:0](game.szName[LOWORD(lpshdef)]) */

L_a091:
    if ((i >= 0x10))
        goto L_a249;
    else
        goto L_a09a;

L_a09a:
    if ((lpfl->rgcsh[i] == 0x0))
        goto L_a07d;
    else
        goto L_a0b7;

L_a0b7:
    j = 0x0;
    goto L_a0c3;

L_a0bf:
    j = (j + 0x1);

L_a0c3:
    if ((j >= lpshdef->hul.chs))
        goto L_a0fa;
    else
        goto L_a0d5;

L_a0d5:
    if ((lpshdef->hul.rghs[j].grhst == hstEngine))
        goto L_a0fa;
    else
        goto L_a0f1;

L_a0f1:

L_a0fa:
    if ((j == lpshdef->hul.chs))
        goto L_a169;
    else
        goto L_a10c;

L_a10c:
    /* untranslated: ss:[bp-0x76] = ((lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) */
    /* untranslated: branch ss:[bp-0x76] >= ((HIWORD(LphuldefFromId(lpshdef->hul.ihuldef)->hul.rghs[j]) >> 0x8) & 0xff) ? L_a181 : L_a169 */

L_a169:
    rgieff[i] = 0x1869f;
    goto L_a07d;

L_a181:
    /* untranslated: rgieff[i] = sext16to32(LpengineFromId((lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff))->rgcFuelUsed[iWarp]) */
    if ((fEfficient == 0x0))
        goto L_a215;
    else
        goto L_a1d7;

L_a1d7:
    rgieff[i] = (rgieff[i] - (int32_t)(((uint32_t)((rgieff[i] * 0xf)) / 0x64)));

L_a215:
    /* untranslated: branch (lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) != 0xa ? L_a07d : L_a23a */

L_a23a:
    gd.fRadiatingEngine = 0x1;

L_a249:
    wtCargo = 0x0;
    i = 0x0;
    goto L_a282;

L_a25b:
    wtCargo = (wtCargo + lpfl->rgwtMin[i]);
    i = (i + 0x1);

L_a282:
    if ((i <= 0x3))
        goto L_a25b;
    else
        goto L_a28b;

L_a28b:
    if ((LOWORD(dTravel) != 0xffff))
        goto L_a323;
    else
        goto L_a294;

L_a294:
    if ((HIWORD(dTravel) != 0xffff))
        goto L_a323;
    else
        goto L_a29d;

L_a29d:
    if ((fRangeOnly == 0x0))
        goto L_a2b3;
    else
        goto L_a2a6;

L_a2a6:
    dTravel = 0x3e8;
    goto L_a323;

L_a2b3:
    lpord = &(lpfl->lpplord->rgord[iOrd]);
    d = DGetDistance(lpord->pt.x, lpord->pt.y, lpord[0x1].pt.x, *(lpord + 0x14));
    __ftol();
    /* untranslated: LOWORD(dTravel) = loword(callresult(int32_t)) */
    /* untranslated: HIWORD(dTravel) = signhiword(loword(callresult(int32_t))) */

L_a323:
    lFuel = 0x0;

L_a330:
    iEffNext = 0xf423f;
    i = 0x0;
    goto L_a5ff;

L_a342:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_a5fb;
    else
        goto L_a35f;

L_a35f:
    if ((LOWORD(rgieff[i]) != LOWORD(iEffCur)))
        goto L_a598;
    else
        goto L_a378;

L_a378:
    if ((HIWORD(rgieff[i]) != HIWORD(iEffCur)))
        goto L_a598;
    else
        goto L_a380;

L_a380:
    if ((HIWORD(wtCargo) > HIWORD((uint32_t)(((uint32_t)(lpfl->rgcsh[i]) * (uint32_t)(WtMaxShdefStat(&(rglpshdef[lpfl->iPlayer][i]), 0x2)))))))
        goto L_a3e9;
    else
        goto L_a3d3;

L_a3d3:
    /* untranslated: branch HIWORD(wtCargo) < hiword(callresult(uint32_t)) ? L_a3e0 : L_a3d8 */

L_a3d8:
    /* untranslated: branch LOWORD(wtCargo) >= loword(callresult(uint32_t)) ? L_a3e9 : L_a3e0 */

L_a3e0:
    t_merge_a434_0001_wide = wtCargo;
    goto L_a434;

L_a3e9:
    WtMaxShdefStat(&(rglpshdef[lpfl->iPlayer][i]), 0x2);
    /* untranslated: t_merge_a434_0001_wide = callresult(uint32_t) */

L_a434:
    wtCargoT = t_merge_a434_0001_wide;
    wtCargo = (wtCargo - wtCargoT);
    if ((HIWORD(rgieff[i]) < 0x0))
        goto L_a5fb;
    else
        goto L_a45b;

L_a45b:
    if ((HIWORD(rgieff[i]) > 0x0))
        goto L_a468;
    else
        goto L_a460;

L_a460:
    if ((LOWORD(rgieff[i]) <= 0x0))
        goto L_a5fb;
    else
        goto L_a468;

L_a468:
    /* untranslated: wtMass = ((uint32_t)(sext16to32(lpfl->rgcsh[i]) * words(0x0, rglpshdef[lpfl->iPlayer][i].hul.wtEmpty)) + wtCargoT) */
    lT = (uint32_t)((iEffCur * dTravel));
    if ((HIWORD(wtMass) < 0x0))
        goto L_a548;
    else
        goto L_a4d9;

L_a4d9:
    if ((HIWORD(wtMass) > 0x0))
        goto L_a4e8;
    else
        goto L_a4de;

L_a4de:
    if ((LOWORD(wtMass) < 0xc8))
        goto L_a548;
    else
        goto L_a4e8;

L_a4e8:
    if ((HIWORD(lT) > 0x7))
        goto L_a518;
    else
        goto L_a4f1;

L_a4f1:
    if ((HIWORD(lT) < 0x7))
        goto L_a500;
    else
        goto L_a4f6;

L_a4f6:
    if ((LOWORD(lT) >= 0xa120))
        goto L_a518;
    else
        goto L_a500;

L_a500:
    if ((HIWORD(wtMass) < 0x0))
        goto L_a548;
    else
        goto L_a509;

L_a509:
    if ((HIWORD(wtMass) > 0x0))
        goto L_a518;
    else
        goto L_a50e;

L_a50e:
    if ((LOWORD(wtMass) < 0xfa0))
        goto L_a548;
    else
        goto L_a518;

L_a518:
    if ((HIWORD(lT) > 0x1))
        goto L_a571;
    else
        goto L_a521;

L_a521:
    if ((HIWORD(lT) < 0x1))
        goto L_a530;
    else
        goto L_a526;

L_a526:
    if ((LOWORD(lT) >= 0x86a0))
        goto L_a571;
    else
        goto L_a530;

L_a530:
    if ((HIWORD(wtMass) > 0x0))
        goto L_a571;
    else
        goto L_a539;

L_a539:
    if ((HIWORD(wtMass) < 0x0))
        goto L_a548;
    else
        goto L_a53e;

L_a53e:
    if ((LOWORD(wtMass) >= 0x4e20))
        goto L_a571;
    else
        goto L_a548;

L_a548:
    lFuel = (lFuel + (int32_t)(((uint32_t)((wtMass * lT)) / 0x7d0)));
    goto L_a5fb;

L_a571:
    lFuel = (__ftol() + lFuel);

L_a598:
    if ((HIWORD(rgieff[i]) < HIWORD(iEffCur)))
        goto L_a5fb;
    else
        goto L_a5b2;

L_a5b2:
    if ((HIWORD(rgieff[i]) > HIWORD(iEffCur)))
        goto L_a5be;
    else
        goto L_a5b7;

L_a5b7:
    if ((LOWORD(rgieff[i]) <= LOWORD(iEffCur)))
        goto L_a5fb;
    else
        goto L_a5be;

L_a5be:
    if ((HIWORD(rgieff[i]) > HIWORD(iEffNext)))
        goto L_a5fb;
    else
        goto L_a5d8;

L_a5d8:
    if ((HIWORD(rgieff[i]) < HIWORD(iEffNext)))
        goto L_a5e4;
    else
        goto L_a5dd;

L_a5dd:
    if ((LOWORD(rgieff[i]) >= LOWORD(iEffNext)))
        goto L_a5fb;
    else
        goto L_a5e4;

L_a5e4:
    iEffNext = rgieff[i];

L_a5fb:
    i = (i + 0x1);

L_a5ff:
    if ((i < 0x10))
        goto L_a342;
    else
        goto L_a608;

L_a608:
    if ((LOWORD(iEffNext) != 0x423f))
        goto L_a61e;
    else
        goto L_a612;

L_a612:
    if ((HIWORD(iEffNext) == 0xf))
        goto L_a62d;
    else
        goto L_a618;

L_a618:

L_a61e:
    iEffCur = iEffNext;

L_a62d:
    if ((fRangeOnly != 0x0))
        goto L_a63e;
    else
        goto L_a636;

L_a636:
    lFuel = (lFuel + 0x9);

L_a63e:
    lFuel = (int32_t)((lFuel / 0xa));
    if ((fRangeOnly == 0x0))
        goto L_a6f0;
    else
        goto L_a660;

L_a660:
    if ((LOWORD(lFuel) != 0x0))
        goto L_a672;
    else
        goto L_a669;

L_a669:
    if ((HIWORD(lFuel) == 0x0))
        goto L_a6e6;
    else
        goto L_a672;

L_a672:
    if ((HIWORD(lFuel) < 0x1))
        goto L_a6b8;
    else
        goto L_a67b;

L_a67b:
    if ((HIWORD(lFuel) > 0x1))
        goto L_a68a;
    else
        goto L_a680;

L_a680:
    if ((LOWORD(lFuel) <= 0x86a0))
        goto L_a6b8;
    else
        goto L_a68a;

L_a68a:
    lFuel = (int32_t)((lpfl->rgwtMin[0x4] / (int32_t)((lFuel / 0x3e8))));
    goto L_a6f0;

L_a6b8:
    lFuel = (int32_t)(((uint32_t)((lpfl->rgwtMin[0x4] * 0x3e8)) / lFuel));

L_a6e6:
    lFuel = 0x3b9aca00;

L_a6f0:

L_a6f9:
    return lFuel;
}

int32_t FakeEditProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam) {
L_a700:
    goto L_a73c;

L_a712:
    if ((wParam < 0x30))
        goto L_a724;
    else
        goto L_a71b;

L_a71b:
    if ((wParam <= 0x39))
        goto L_a747;
    else
        goto L_a724;

L_a724:
    if ((wParam == 0x8))
        goto L_a747;
    else
        goto L_a72a;

L_a72a:

L_a730:
    return 0x0;

L_a73c:
    if ((msg == WM_CHAR))
        goto L_a712;
    else
        goto L_a747;

L_a747:
    return CallWindowProc(lpfnRealEditProc, hwnd, msg, wParam, lParam);
}

int16_t IFindIdealWarp(FLEET *lpfl, int16_t fIgnoreScoops) {
    int16_t i;
    int16_t j;
    int16_t iWorst;
    ENGINE *lpengine;

L_a76e:
    iWorst = 0xa;
    if ((LOWORD(lpfl) != 0x0))
        goto L_a799;
    else
        goto L_a785;

L_a785:
    if ((HIWORD(lpfl) != 0x0))
        goto L_a799;
    else
        goto L_a78e;

L_a78e:
    /* untranslated: lpfl = &part[28:0](sel) */

L_a799:
    i = 0x0;
    goto L_a9de;

L_a7a1:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_a9da;
    else
        goto L_a7be;

L_a7be:
    j = 0x0;
    goto L_a7ca;

L_a7c6:
    j = (j + 0x1);

L_a7ca:
    if ((j >= rglpshdef[lpfl->iPlayer][i].hul.chs))
        goto L_a830;
    else
        goto L_a7f6;

L_a7f6:
    if ((rglpshdef[lpfl->iPlayer][i].hul.rghs[j].grhst == hstEngine))
        goto L_a830;
    else
        goto L_a827;

L_a827:

L_a830:
    if ((j != rglpshdef[lpfl->iPlayer][i].hul.chs))
        goto L_a864;
    else
        goto L_a85c;

L_a85c:
    iWorst = 0x0;
    goto L_a9e7;

L_a864:
    j = (HIWORD(rglpshdef[lpfl->iPlayer][i].hul.rghs[j]) & 0xff);
    lpengine = LpengineFromId(j);
    goto L_a9d1;

L_a8ad:
    if ((lpengine->rgcFuelUsed[iWorst] > 0x78))
        goto L_a9cd;
    else
        goto L_a8ca;

L_a8ca:
    if ((lpengine->rgcFuelUsed[iWorst] <= 0x0))
        goto L_a98f;
    else
        goto L_a8e7;

L_a8e7:
    if ((fIgnoreScoops != 0x0))
        goto L_a98f;
    else
        goto L_a8f0;

L_a8f0:
    if ((j == 0xe))
        goto L_a98f;
    else
        goto L_a8f9;

L_a8f9:
    if ((j == 0xf))
        goto L_a98f;
    else
        goto L_a902;

L_a902:
    if ((iWorst < 0x5))
        goto L_a932;
    else
        goto L_a90b;

L_a90b:
    if ((lpengine->rgcFuelUsed[(iWorst - 0x1)] != 0x0))
        goto L_a932;
    else
        goto L_a92b;

L_a92b:
    iWorst = (iWorst - 0x1);
    goto L_a98f;

L_a932:
    if ((iWorst < 0x6))
        goto L_a962;
    else
        goto L_a93b;

L_a93b:
    if ((lpengine->rgcFuelUsed[(iWorst - 0x2)] != 0x0))
        goto L_a962;
    else
        goto L_a95b;

L_a95b:
    iWorst = (iWorst - 0x2);
    goto L_a98f;

L_a962:
    if ((iWorst < 0x7))
        goto L_a98f;
    else
        goto L_a96b;

L_a96b:
    if ((lpengine->rgcFuelUsed[(iWorst - 0x3)] != 0x0))
        goto L_a98f;
    else
        goto L_a98b;

L_a98b:
    iWorst = (iWorst - 0x3);

L_a98f:
    if ((iWorst != 0xa))
        goto L_a9da;
    else
        goto L_a998;

L_a998:
    if ((j == 0x7))
        goto L_a9da;
    else
        goto L_a9a1;

L_a9a1:
    if ((j == 0x8))
        goto L_a9da;
    else
        goto L_a9aa;

L_a9aa:
    if ((j == 0x9))
        goto L_a9da;
    else
        goto L_a9b3;

L_a9b3:
    if ((j == 0xe))
        goto L_a9da;
    else
        goto L_a9bc;

L_a9bc:
    if ((j == 0xf))
        goto L_a9da;
    else
        goto L_a9c5;

L_a9c5:
    iWorst = 0x9;

L_a9cd:
    iWorst = (iWorst - 0x1);

L_a9d1:
    if ((iWorst > 0x0))
        goto L_a8ad;
    else
        goto L_a9da;

L_a9da:
    i = (i + 0x1);

L_a9de:
    if ((i < 0x10))
        goto L_a7a1;
    else
        goto L_a9e7;

L_a9e7:

L_a9ed:
    return iWorst;
}

int32_t LFuelUseToWaypoint(FLEET *lpfl, int16_t iwp, int16_t fMaxCargo) {
    int32_t lCur;
    int16_t iWarp;
    int16_t dist;
    PLANET *lppl;
    int16_t i;
    int32_t lTot;
    ORDER  *lpord;
    int16_t cYears;
    SHDEF  *lpshdef;
    int16_t j;
    double  dbl;
    int32_t l;
    int32_t lOneYearUse;
    int32_t lFuelGain;

L_a9f4:
    lTot = 0x0;
    lCur = 0x0;
    lpord = &(lpfl->lpplord->rgord);
    i = 0x0;
    goto L_ae59;

L_aa2f:
    /* untranslated: iWarp = ((part[6:2](lpord[(i + 0x1)]) >> 0x4) & 0xf) */
    if ((iWarp <= 0x0))
        goto L_ab55;
    else
        goto L_aa5f;

L_aa5f:
    if ((iWarp >= 0xb))
        goto L_ab55;
    else
        goto L_aa68;

L_aa68:
    dbl = (DGetDistance(lpord[i].pt.x, lpord[i].pt.y, lpord[(i + 0x1)].pt.x, lpord[(i + 0x1)].pt.y) + 0.99999);
    dist = LOWORD(__ftol());
    /* untranslated: ss:[bp-0x34] = iWarp */
    /* untranslated: ss:[bp-0x32] = signhiword(iWarp) */
    /* untranslated: ss:[bp-0x3c] = iWarp */
    /* untranslated: ss:[bp-0x3a] = signhiword(iWarp) */
    /* untranslated: dbl = ((dbl / (double)dword ss:[bp-0x34]) / (double)dword ss:[bp-0x3c]) */
    cYears = LOWORD(__ftol());
    l = EstFuelUse(lpfl, i, iWarp, 0xffffffff, 0x0);
    goto L_ab64;

L_ab55:
    cYears = 0x1;
    l = 0x0;

L_ab64:
    if ((cYears <= 0x1))
        goto L_ad56;
    else
        goto L_ab6d;

L_ab6d:
    /* untranslated: lOneYearUse = EstFuelUse(lpfl, i, iWarp, words(signhiword(loword((iWarp * iWarp))), loword((iWarp * iWarp))), 0x0) */
    lFuelGain = (uint32_t)((lOneYearUse * (uint32_t)((cYears + 0xffff))));
    lFuelGain = (lFuelGain + EstFuelUse(lpfl, i, iWarp, (uint32_t)((dist - LOWORD((LOWORD((iWarp * iWarp)) * (cYears + 0xffff))))), 0x0));
    if ((HIWORD(lFuelGain) < HIWORD(l)))
        goto L_ac0b;
    else
        goto L_abf2;

L_abf2:
    if ((HIWORD(lFuelGain) > HIWORD(l)))
        goto L_abff;
    else
        goto L_abf7;

L_abf7:
    if ((LOWORD(lFuelGain) <= LOWORD(l)))
        goto L_ac0b;
    else
        goto L_abff;

L_abff:
    l = lFuelGain;

L_ac0b:
    /* untranslated: lFuelGain = LCalcFuelGainFromRamScoops(lpfl, iWarp, words(signhiword(loword((iWarp * iWarp))), loword((iWarp * iWarp)))) */
    j = 0x0;
    goto L_acbd;

L_ac33:
    if ((lpfl->rgcsh[j] == 0x0))
        goto L_acb9;
    else
        goto L_ac4d;

L_ac4d:

L_ac53:
    lpshdef = &(rglpshdef[idPlayer][j]);
    if ((lpshdef->hul.ihuldef == ihuldefFuelTransport))
        goto L_ac8c;
    else
        goto L_ac7d;

L_ac7d:
    if ((lpshdef->hul.ihuldef != ihuldefSuperFuelXport))
        goto L_acb9;
    else
        goto L_ac86;

L_ac86:

L_ac8c:
    lFuelGain = (lFuelGain + (uint32_t)(((uint32_t)(lpfl->rgcsh[j]) * 0xc8)));

L_acb9:
    j = (j + 0x1);

L_acbd:
    if ((j < 0x10))
        goto L_ac33;
    else
        goto L_acc6;

L_acc6:
    if ((HIWORD(lFuelGain) < 0x0))
        goto L_ad56;
    else
        goto L_accf;

L_accf:
    if ((HIWORD(lFuelGain) > 0x0))
        goto L_acdd;
    else
        goto L_acd4;

L_acd4:
    if ((LOWORD(lFuelGain) <= 0x0))
        goto L_ad56;
    else
        goto L_acdd;

L_acdd:
    if ((HIWORD(lOneYearUse) > HIWORD(lFuelGain)))
        goto L_ad07;
    else
        goto L_aceb;

L_aceb:
    if ((HIWORD(lOneYearUse) < HIWORD(lFuelGain)))
        goto L_acf8;
    else
        goto L_acf0;

L_acf0:
    if ((LOWORD(lOneYearUse) > LOWORD(lFuelGain)))
        goto L_ad07;
    else
        goto L_acf8;

L_acf8:
    l = lOneYearUse;
    goto L_ad56;

L_ad07:
    /* untranslated: lOneYearUse = ((uint32_t)(words((HIWORD(lOneYearUse) - HIWORD(lFuelGain)), (LOWORD(lOneYearUse) - LOWORD(lFuelGain))) * sext16to32((cYears
     * + 0xffff))) + lOneYearUse) */
    if ((HIWORD(lOneYearUse) > HIWORD(l)))
        goto L_ad56;
    else
        goto L_ad3d;

L_ad3d:
    if ((HIWORD(lOneYearUse) < HIWORD(l)))
        goto L_ad4a;
    else
        goto L_ad42;

L_ad42:
    if ((LOWORD(lOneYearUse) >= LOWORD(l)))
        goto L_ad56;
    else
        goto L_ad4a;

L_ad4a:
    l = lOneYearUse;

L_ad56:
    lCur = (lCur + l);
    if ((HIWORD(lCur) < HIWORD(lTot)))
        goto L_ad89;
    else
        goto L_ad70;

L_ad70:
    if ((HIWORD(lCur) > HIWORD(lTot)))
        goto L_ad7d;
    else
        goto L_ad75;

L_ad75:
    if ((LOWORD(lCur) <= LOWORD(lTot)))
        goto L_ad89;
    else
        goto L_ad7d;

L_ad7d:
    lTot = lCur;

L_ad89:
    /* untranslated: branch ((part[6:2](lpord[(i + 0x1)]) >> 0x8) & 0xf) != 0x1 ? L_ae55 : L_adb2 */

L_adb2:
    LpplFromId(lpord[(i + 0x1)].id);
    /* untranslated: lppl = callresult(PLANET *) */
    /* untranslated: branch faroff(callresult(PLANET *)) != 0x0 ? L_ade9 : L_ade1 */

L_ade1:
    /* untranslated: branch farseg(callresult(PLANET *)) == 0x0 ? L_ae55 : L_ade9 */

L_ade9:
    if ((lppl->iPlayer != idPlayer))
        goto L_ae55;
    else
        goto L_adf8;

L_adf8:
    if ((lppl->fStarbase == 0x0))
        goto L_ae55;
    else
        goto L_ae0f;

L_ae0f:
    if ((LphuldefFromId(rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef)->hul.wtCargoMax == 0x0))
        goto L_ae55;
    else
        goto L_ae4b;

L_ae4b:
    lCur = 0x0;

L_ae55:
    i = (i + 0x1);

L_ae59:
    if ((i < iwp))
        goto L_aa2f;
    else
        goto L_ae64;

L_ae64:

L_ae6d:
    return lTot;
}

void FleetTransferCargoBalance(FLEET *pflNew1, FLEET *pflNew2) {
    int16_t  iplr;
    int32_t  rgCargoCapLoss[2];
    int32_t  wtCargoXfer;
    int16_t  fDeadFleet;
    int32_t  wtCargoTot;
    int16_t  rgrgcshLoss[2][16];
    int32_t  rgrgCargoDelta[2][5];
    int32_t  rgFuelCapacity[2];
    FLEET   *rgpflNew[2];
    int16_t  wtCargoMax;
    int16_t  wtFuelMax;
    int16_t  i;
    int32_t  lChg;
    int32_t  rgFuelCapLoss[2];
    FLEET    rgflCur[2];
    int16_t  j;
    SHDEF   *lpshdef;
    int32_t  rgCargoCapacity[2];
    int16_t  ishdef;
    int32_t  l;
    int32_t  cshDmgDst;
    int32_t  cshDmgSrc;
    int16_t  iSrc;
    int32_t  pctNew;
    int32_t  cshDmgMoved;
    uint16_t t_merge_b243_0001;
    uint16_t t_merge_b2ea_0001;
    uint32_t t_merge_b325_0001_wide;
    uint32_t t_merge_b355_0001_wide;
    uint32_t t_merge_b443_0001_wide;
    uint32_t t_merge_b4e5_0001_wide;
    uint16_t t_merge_b5ae_0001;
    uint32_t t_merge_b5de_0001_wide;
    uint32_t t_merge_b60d_0001_wide;
    uint16_t t_merge_b6d2_0001;
    uint16_t t_merge_b90f_0001;
    uint32_t t_merge_b93f_0001_wide;
    uint32_t t_merge_b96e_0001_wide;
    uint16_t t_merge_ba29_0001;
    uint32_t t_merge_bb86_0001_wide;
    uint32_t t_merge_bbb5_0001_wide;
    uint16_t t_merge_bc70_0001;
    uint16_t t_merge_bcd9_0001;
    int32_t  t_merge_c082_0001_wide;
    uint32_t t_merge_c236_0001_wide;

L_ae74:
    fDeadFleet = 0x0;
    LOWORD(rgpflNew) = pflNew1;
    HIWORD(rgpflNew) = pflNew2;
    iplr = pflNew1->iPlayer;
    i = 0x0;
    goto L_afb3;

L_aea2:
    if ((((*(rgpflNew[i] + 0x4) >> 0xa) & 0x1) == 0x0))
        goto L_aefc;
    else
        goto L_aec3;

L_aec3:
    fDeadFleet = 0x1;
    memset(rgflCur[i], 0x0, 0x7c);
    rgflCur[i].iPlayer = iplr;
    goto L_af22;

L_aefc:
    FLookupFleet(rgpflNew[i], rgflCur[i]);

L_af22:
    rgCargoCapLoss[i] = 0x0;
    rgCargoCapacity[i] = 0x0;
    rgFuelCapLoss[i] = 0x0;
    rgFuelCapacity[i] = 0x0;
    j = 0x0;
    goto L_afa4;

L_af80:
    rgrgCargoDelta[i][j] = 0x0;
    j = (j + 0x1);

L_afa4:
    if ((j < 0x5))
        goto L_af80;
    else
        goto L_afae;

L_afae:
    i = (i + 0x1);

L_afb3:
    if ((i < 0x2))
        goto L_aea2;
    else
        goto L_afbd;

L_afbd:
    ishdef = 0x0;
    goto L_bcfb;

L_afc6:
    if ((rgflCur[0x0].rgcsh[ishdef] != 0x0))
        goto L_afee;
    else
        goto L_afda;

L_afda:
    if ((rgflCur[0x1].rgcsh[ishdef] == 0x0))
        goto L_bcf6;
    else
        goto L_afee;

L_afee:
    lpshdef = &(rglpshdef[iplr][ishdef]);
    wtFuelMax = WtMaxShdefStat(lpshdef, 0x1);
    wtCargoMax = WtMaxShdefStat(lpshdef, 0x2);
    i = 0x0;
    goto L_b1e0;

L_b047:
    rgrgcshLoss[i][ishdef] = (rgflCur[i].rgcsh[ishdef] - rgpflNew[i]->rgcsh[ishdef]);
    if ((rgflCur[i].rgcsh[ishdef] == 0x0))
        goto L_b1db;
    else
        goto L_b0bf;

L_b0bf:
    rgFuelCapacity[i] = (rgFuelCapacity[i] + (uint32_t)(((uint32_t)(rgflCur[i].rgcsh[ishdef]) * (uint32_t)(wtFuelMax))));
    rgCargoCapacity[i] = (rgCargoCapacity[i] + (uint32_t)(((uint32_t)(rgflCur[i].rgcsh[ishdef]) * (uint32_t)(wtCargoMax))));
    if ((rgrgcshLoss[i][ishdef] <= 0x0))
        goto L_b1db;
    else
        goto L_b15e;

L_b15e:
    rgFuelCapLoss[i] = (rgFuelCapLoss[i] + (uint32_t)(((uint32_t)(rgrgcshLoss[i][ishdef]) * (uint32_t)(wtFuelMax))));
    rgCargoCapLoss[i] = (rgCargoCapLoss[i] + (uint32_t)(((uint32_t)(rgrgcshLoss[i][ishdef]) * (uint32_t)(wtCargoMax))));

L_b1db:
    i = (i + 0x1);

L_b1e0:
    if ((i < 0x2))
        goto L_b047;
    else
        goto L_b1ea;

L_b1ea:
    if ((fDeadFleet != 0x0))
        goto L_bcf6;
    else
        goto L_b1f3;

L_b1f3:
    /* untranslated: branch rgrgcshLoss[0x0][ishdef] != (rgrgcshLoss[0x1][ishdef] neg 0x0) ? L_bcf6 : L_b214 */

L_b214:
    if ((rgrgcshLoss[0x0][ishdef] == 0x0))
        goto L_bcf6;
    else
        goto L_b227;

L_b227:
    if ((rgrgcshLoss[0x0][ishdef] >= 0x0))
        goto L_b240;
    else
        goto L_b23a;

L_b23a:
    t_merge_b243_0001 = 0x1;
    goto L_b243;

L_b240:
    t_merge_b243_0001 = 0x0;

L_b243:
    iSrc = t_merge_b243_0001;
    if ((rgflCur[iSrc].rgcsh[ishdef] <= 0x0))
        goto L_b2cb;
    else
        goto L_b269;

L_b269:
    /* untranslated: LOWORD(cshDmgSrc) = loword((int32_t)(words(hiword((uint32_t)(words(0x0, (0x25:[(part[44:0](rgflCur[iSrc]) + (ishdef * 0x2))] & 0x7f)) *
     * sext16to32(rgflCur[iSrc].rgcsh[ishdef]))), loword((uint32_t)(words(0x0, (0x25:[(part[44:0](rgflCur[iSrc]) + (ishdef * 0x2))] & 0x7f)) *
     * sext16to32(rgflCur[iSrc].rgcsh[ishdef])))) / 0x64)) */
    /* untranslated: HIWORD(cshDmgSrc) = hiword((int32_t)(words(hiword((uint32_t)(words(0x0, (0x25:[(part[44:0](rgflCur[iSrc]) + (ishdef * 0x2))] & 0x7f)) *
     * sext16to32(rgflCur[iSrc].rgcsh[ishdef]))), loword((uint32_t)(words(0x0, (0x25:[(part[44:0](rgflCur[iSrc]) + (ishdef * 0x2))] & 0x7f)) *
     * sext16to32(rgflCur[iSrc].rgcsh[ishdef])))) / 0x64)) */
    goto L_b2d7;

L_b2cb:
    cshDmgSrc = 0x0;

L_b2d7:
    if ((iSrc != 0x0))
        goto L_b2e7;
    else
        goto L_b2e1;

L_b2e1:
    t_merge_b2ea_0001 = 0x1;
    goto L_b2ea;

L_b2e7:
    t_merge_b2ea_0001 = 0x0;

L_b2ea:
    if ((rgflCur[t_merge_b2ea_0001].rgcsh[ishdef] <= 0x0))
        goto L_b38e;
    else
        goto L_b30a;

L_b30a:
    if ((iSrc != 0x0))
        goto L_b322;
    else
        goto L_b31c;

L_b31c:
    t_merge_b325_0001_wide = 0x10001;
    goto L_b325;

L_b322:
    t_merge_b325_0001_wide = 0x0;

L_b325:
    if ((iSrc != 0x0))
        goto L_b352;
    else
        goto L_b34c;

L_b34c:
    t_merge_b355_0001_wide = 0x10001;
    goto L_b355;

L_b352:
    t_merge_b355_0001_wide = 0x0;

L_b355:
    /* untranslated: LOWORD(cshDmgDst) = loword((int32_t)(words(hiword((uint32_t)(words(0x0, (0x25:[(part[44:0](rgflCur[loword(t_merge_b355_0001_wide)]) +
     * (ishdef * 0x2))] & 0x7f)) * words(signhiword(rgflCur[loword(t_merge_b325_0001_wide)].rgcsh[ishdef]),
     * rgflCur[hiword(t_merge_b325_0001_wide)].rgcsh[ishdef]))), loword((uint32_t)(words(0x0, (0x25:[(part[44:0](rgflCur[hiword(t_merge_b355_0001_wide)]) +
     * (ishdef * 0x2))] & 0x7f)) * sext16to32(rgflCur[hiword(t_merge_b325_0001_wide)].rgcsh[ishdef])))) / 0x64)) */
    /* untranslated: HIWORD(cshDmgDst) = hiword((int32_t)(words(hiword((uint32_t)(words(0x0, (0x25:[(part[44:0](rgflCur[hiword(t_merge_b355_0001_wide)]) +
     * (ishdef * 0x2))] & 0x7f)) * sext16to32(rgflCur[hiword(t_merge_b325_0001_wide)].rgcsh[ishdef]))), loword((uint32_t)(words(0x0,
     * (0x25:[(part[44:0](rgflCur[hiword(t_merge_b355_0001_wide)]) + (ishdef * 0x2))] & 0x7f)) *
     * sext16to32(rgflCur[hiword(t_merge_b325_0001_wide)].rgcsh[ishdef])))) / 0x64)) */
    goto L_b39a;

L_b38e:
    cshDmgDst = 0x0;

L_b39a:
    if ((LOWORD(cshDmgSrc) != 0x0))
        goto L_b3ae;
    else
        goto L_b3a4;

L_b3a4:
    if ((HIWORD(cshDmgSrc) == 0x0))
        goto L_b808;
    else
        goto L_b3ae;

L_b3ae:
    if ((LOWORD(cshDmgDst) != 0x0))
        goto L_b3c2;
    else
        goto L_b3b8;

L_b3b8:
    if ((HIWORD(cshDmgDst) == 0x0))
        goto L_b808;
    else
        goto L_b3c2;

L_b3c2:
    if ((HIWORD(cshDmgSrc) < SIGNHIWORD(rgrgcshLoss[iSrc][ishdef])))
        goto L_b420;
    else
        goto L_b3e9;

L_b3e9:
    if ((HIWORD(cshDmgSrc) > SIGNHIWORD(rgrgcshLoss[iSrc][ishdef])))
        goto L_b3f7;
    else
        goto L_b3ee;

L_b3ee:
    if ((LOWORD(cshDmgSrc) <= rgrgcshLoss[iSrc][ishdef]))
        goto L_b420;
    else
        goto L_b3f7;

L_b3f7:
    cshDmgMoved = (uint32_t)(rgrgcshLoss[iSrc][ishdef]);
    goto L_b430;

L_b420:
    cshDmgMoved = cshDmgSrc;

L_b430:
    if ((iSrc != 0x0))
        goto L_b440;
    else
        goto L_b43a;

L_b43a:
    t_merge_b443_0001_wide = 0x10001;
    goto L_b443;

L_b440:
    t_merge_b443_0001_wide = 0x0;

L_b443:
    if ((iSrc != 0x0))
        goto L_b46f;
    else
        goto L_b469;

L_b469:
    goto L_b472;

L_b46f:

L_b472:
    if ((iSrc != 0x0))
        goto L_b4e2;
    else
        goto L_b4dc;

L_b4dc:
    t_merge_b4e5_0001_wide = 0x10001;
    goto L_b4e5;

L_b4e2:
    t_merge_b4e5_0001_wide = 0x0;

L_b4e5:
    /* untranslated: LOWORD(pctNew) = loword((int32_t)(words((((hiword((uint32_t)(cshDmgDst * words(0x0,
     * ((0x25:[(part[44:0](rgflCur[loword(t_merge_b4e5_0001_wide)]) + (ishdef * 0x2))] >> 0x7) & 0x1ff)))) + ss:[bp-0x1c0]) + ss:[bp-0x1bc]) + 0xffff),
     * (((loword((uint32_t)(cshDmgDst * words(0x0, ((0x25:[(part[44:0](rgflCur[hiword(t_merge_b4e5_0001_wide)]) + (ishdef * 0x2))] >> 0x7) & 0x1ff)))) +
     * ss:[bp-0x1c2]) + ss:[bp-0x1be]) + 0xffff)) / words(signhiword(rgpflNew[loword(t_merge_b443_0001_wide)]->rgcsh[ishdef]),
     * rgpflNew[hiword(t_merge_b443_0001_wide)]->rgcsh[ishdef]))) */
    /* untranslated: HIWORD(pctNew) = hiword((int32_t)(words((((hiword((uint32_t)(cshDmgDst * words(0x0,
     * ((0x25:[(part[44:0](rgflCur[hiword(t_merge_b4e5_0001_wide)]) + (ishdef * 0x2))] >> 0x7) & 0x1ff)))) + ss:[bp-0x1c0]) + ss:[bp-0x1bc]) + 0xffff),
     * (((loword((uint32_t)(cshDmgDst * words(0x0, ((0x25:[(part[44:0](rgflCur[hiword(t_merge_b4e5_0001_wide)]) + (ishdef * 0x2))] >> 0x7) & 0x1ff)))) +
     * ss:[bp-0x1c2]) + ss:[bp-0x1be]) + 0xffff)) / sext16to32(rgpflNew[hiword(t_merge_b443_0001_wide)]->rgcsh[ishdef]))) */
    if ((iSrc != 0x0))
        goto L_b572;
    else
        goto L_b56c;

L_b56c:
    goto L_b575;

L_b572:

L_b575:
    if ((iSrc != 0x0))
        goto L_b5ab;
    else
        goto L_b5a5;

L_b5a5:
    t_merge_b5ae_0001 = 0x1;
    goto L_b5ae;

L_b5ab:
    t_merge_b5ae_0001 = 0x0;

L_b5ae:
    /* untranslated: 0x25:[((rgpflNew[t_merge_b5ae_0001] + 0x2c) + (ishdef * 0x2))] = ss:[bp-0x1c0] */
    if ((iSrc != 0x0))
        goto L_b5db;
    else
        goto L_b5d5;

L_b5d5:
    t_merge_b5de_0001_wide = 0x10001;
    goto L_b5de;

L_b5db:
    t_merge_b5de_0001_wide = 0x0;

L_b5de:
    if ((iSrc != 0x0))
        goto L_b60a;
    else
        goto L_b604;

L_b604:
    t_merge_b60d_0001_wide = 0x10001;
    goto L_b60d;

L_b60a:
    t_merge_b60d_0001_wide = 0x0;

L_b60d:
    /* untranslated: LOWORD(pctNew) = loword((int32_t)(words(((hiword((uint32_t)(words((HIWORD(cshDmgDst) + HIWORD(cshDmgMoved)), (LOWORD(cshDmgDst) +
     * LOWORD(cshDmgMoved))) * 0x64)) + signhiword(rgpflNew[loword(t_merge_b60d_0001_wide)]->rgcsh[ishdef])) + 0xffff),
     * ((loword((uint32_t)(words((HIWORD(cshDmgDst) + HIWORD(cshDmgMoved)), (LOWORD(cshDmgDst) + LOWORD(cshDmgMoved))) * 0x64)) +
     * rgpflNew[hiword(t_merge_b60d_0001_wide)]->rgcsh[ishdef]) + 0xffff)) / words(signhiword(rgpflNew[loword(t_merge_b5de_0001_wide)]->rgcsh[ishdef]),
     * rgpflNew[hiword(t_merge_b5de_0001_wide)]->rgcsh[ishdef]))) */
    /* untranslated: HIWORD(pctNew) = hiword((int32_t)(words(((hiword((uint32_t)(words((HIWORD(cshDmgDst) + HIWORD(cshDmgMoved)), (LOWORD(cshDmgDst) +
     * LOWORD(cshDmgMoved))) * 0x64)) + signhiword(rgpflNew[hiword(t_merge_b60d_0001_wide)]->rgcsh[ishdef])) + 0xffff),
     * ((loword((uint32_t)(words((HIWORD(cshDmgDst) + HIWORD(cshDmgMoved)), (LOWORD(cshDmgDst) + LOWORD(cshDmgMoved))) * 0x64)) +
     * rgpflNew[hiword(t_merge_b60d_0001_wide)]->rgcsh[ishdef]) + 0xffff)) / sext16to32(rgpflNew[hiword(t_merge_b5de_0001_wide)]->rgcsh[ishdef]))) */
    if ((iSrc != 0x0))
        goto L_b696;
    else
        goto L_b690;

L_b690:
    goto L_b699;

L_b696:

L_b699:
    if ((iSrc != 0x0))
        goto L_b6cf;
    else
        goto L_b6c9;

L_b6c9:
    t_merge_b6d2_0001 = 0x1;
    goto L_b6d2;

L_b6cf:
    t_merge_b6d2_0001 = 0x0;

L_b6d2:
    /* untranslated: 0x25:[((rgpflNew[t_merge_b6d2_0001] + 0x2c) + (ishdef * 0x2))] = ss:[bp-0x1c0] */
    if ((LOWORD(cshDmgMoved) != LOWORD(cshDmgSrc)))
        goto L_b72b;
    else
        goto L_b700;

L_b700:
    if ((HIWORD(cshDmgMoved) != HIWORD(cshDmgSrc)))
        goto L_b72b;
    else
        goto L_b709;

L_b709:
    /* untranslated: 0x25:[((rgpflNew[iSrc] + 0x2c) + (ishdef * 0x2))] = 0x0 */
    goto L_bcf6;

L_b72b:
    /* untranslated: pctNew = (int32_t)(words(((hiword((uint32_t)(words((HIWORD(cshDmgSrc) - HIWORD(cshDmgMoved)), (LOWORD(cshDmgSrc) - LOWORD(cshDmgMoved))) *
     * 0x64)) + signhiword(rgpflNew[iSrc]->rgcsh[ishdef])) + 0xffff), ((loword((uint32_t)(words((HIWORD(cshDmgSrc) - HIWORD(cshDmgMoved)), (LOWORD(cshDmgSrc) -
     * LOWORD(cshDmgMoved))) * 0x64)) + rgpflNew[iSrc]->rgcsh[ishdef]) + 0xffff)) / sext16to32(rgpflNew[iSrc]->rgcsh[ishdef])) */
    /* untranslated: 0x25:[((rgpflNew[iSrc] + 0x2c) + (ishdef * 0x2))] = ((0x25:[((rgpflNew[iSrc] + 0x2c) + (ishdef * 0x2))] & 0xff80) | (LOWORD(pctNew) &
     * 0x7f)) */

L_b808:
    if ((LOWORD(cshDmgSrc) != 0x0))
        goto L_b81c;
    else
        goto L_b812;

L_b812:
    if ((HIWORD(cshDmgSrc) == 0x0))
        goto L_bb5f;
    else
        goto L_b81c;

L_b81c:
    if ((HIWORD(cshDmgSrc) < SIGNHIWORD(rgrgcshLoss[iSrc][ishdef])))
        goto L_b87a;
    else
        goto L_b843;

L_b843:
    if ((HIWORD(cshDmgSrc) > SIGNHIWORD(rgrgcshLoss[iSrc][ishdef])))
        goto L_b851;
    else
        goto L_b848;

L_b848:
    if ((LOWORD(cshDmgSrc) <= rgrgcshLoss[iSrc][ishdef]))
        goto L_b87a;
    else
        goto L_b851;

L_b851:
    cshDmgMoved = (uint32_t)(rgrgcshLoss[iSrc][ishdef]);
    goto L_b88a;

L_b87a:
    cshDmgMoved = cshDmgSrc;

L_b88a:
    if ((iSrc != 0x0))
        goto L_b8d3;
    else
        goto L_b8cd;

L_b8cd:
    goto L_b8d6;

L_b8d3:

L_b8d6:
    if ((iSrc != 0x0))
        goto L_b90c;
    else
        goto L_b906;

L_b906:
    t_merge_b90f_0001 = 0x1;
    goto L_b90f;

L_b90c:
    t_merge_b90f_0001 = 0x0;

L_b90f:
    /* untranslated: 0x25:[((rgpflNew[t_merge_b90f_0001] + 0x2c) + (ishdef * 0x2))] = ss:[bp-0x1c0] */
    if ((iSrc != 0x0))
        goto L_b93c;
    else
        goto L_b936;

L_b936:
    t_merge_b93f_0001_wide = 0x10001;
    goto L_b93f;

L_b93c:
    t_merge_b93f_0001_wide = 0x0;

L_b93f:
    if ((iSrc != 0x0))
        goto L_b96b;
    else
        goto L_b965;

L_b965:
    t_merge_b96e_0001_wide = 0x10001;
    goto L_b96e;

L_b96b:
    t_merge_b96e_0001_wide = 0x0;

L_b96e:
    /* untranslated: LOWORD(pctNew) = loword((int32_t)(words(((hiword((uint32_t)(cshDmgMoved * 0x64)) +
     * signhiword(rgpflNew[loword(t_merge_b96e_0001_wide)]->rgcsh[ishdef])) + 0xffff), ((loword((uint32_t)(cshDmgMoved * 0x64)) +
     * rgpflNew[hiword(t_merge_b96e_0001_wide)]->rgcsh[ishdef]) + 0xffff)) / words(signhiword(rgpflNew[loword(t_merge_b93f_0001_wide)]->rgcsh[ishdef]),
     * rgpflNew[hiword(t_merge_b93f_0001_wide)]->rgcsh[ishdef]))) */
    /* untranslated: HIWORD(pctNew) = hiword((int32_t)(words(((hiword((uint32_t)(cshDmgMoved * 0x64)) +
     * signhiword(rgpflNew[hiword(t_merge_b96e_0001_wide)]->rgcsh[ishdef])) + 0xffff), ((loword((uint32_t)(cshDmgMoved * 0x64)) +
     * rgpflNew[hiword(t_merge_b96e_0001_wide)]->rgcsh[ishdef]) + 0xffff)) / sext16to32(rgpflNew[hiword(t_merge_b93f_0001_wide)]->rgcsh[ishdef]))) */
    if ((iSrc != 0x0))
        goto L_b9ed;
    else
        goto L_b9e7;

L_b9e7:
    goto L_b9f0;

L_b9ed:

L_b9f0:
    if ((iSrc != 0x0))
        goto L_ba26;
    else
        goto L_ba20;

L_ba20:
    t_merge_ba29_0001 = 0x1;
    goto L_ba29;

L_ba26:
    t_merge_ba29_0001 = 0x0;

L_ba29:
    /* untranslated: 0x25:[((rgpflNew[t_merge_ba29_0001] + 0x2c) + (ishdef * 0x2))] = ss:[bp-0x1c0] */
    if ((LOWORD(cshDmgMoved) != LOWORD(cshDmgSrc)))
        goto L_ba82;
    else
        goto L_ba57;

L_ba57:
    if ((HIWORD(cshDmgMoved) != HIWORD(cshDmgSrc)))
        goto L_ba82;
    else
        goto L_ba60;

L_ba60:
    /* untranslated: 0x25:[((rgpflNew[iSrc] + 0x2c) + (ishdef * 0x2))] = 0x0 */
    goto L_bcf6;

L_ba82:
    /* untranslated: pctNew = (int32_t)(words(((hiword((uint32_t)(words((HIWORD(cshDmgSrc) - HIWORD(cshDmgMoved)), (LOWORD(cshDmgSrc) - LOWORD(cshDmgMoved))) *
     * 0x64)) + signhiword(rgpflNew[iSrc]->rgcsh[ishdef])) + 0xffff), ((loword((uint32_t)(words((HIWORD(cshDmgSrc) - HIWORD(cshDmgMoved)), (LOWORD(cshDmgSrc) -
     * LOWORD(cshDmgMoved))) * 0x64)) + rgpflNew[iSrc]->rgcsh[ishdef]) + 0xffff)) / sext16to32(rgpflNew[iSrc]->rgcsh[ishdef])) */
    /* untranslated: 0x25:[((rgpflNew[iSrc] + 0x2c) + (ishdef * 0x2))] = ((0x25:[((rgpflNew[iSrc] + 0x2c) + (ishdef * 0x2))] & 0xff80) | (LOWORD(pctNew) &
     * 0x7f)) */

L_bb5f:
    if ((LOWORD(cshDmgDst) != 0x0))
        goto L_bb73;
    else
        goto L_bb69;

L_bb69:
    if ((HIWORD(cshDmgDst) == 0x0))
        goto L_bc90;
    else
        goto L_bb73;

L_bb73:
    if ((iSrc != 0x0))
        goto L_bb83;
    else
        goto L_bb7d;

L_bb7d:
    t_merge_bb86_0001_wide = 0x10001;
    goto L_bb86;

L_bb83:
    t_merge_bb86_0001_wide = 0x0;

L_bb86:
    if ((iSrc != 0x0))
        goto L_bbb2;
    else
        goto L_bbac;

L_bbac:
    t_merge_bbb5_0001_wide = 0x10001;
    goto L_bbb5;

L_bbb2:
    t_merge_bbb5_0001_wide = 0x0;

L_bbb5:
    /* untranslated: LOWORD(pctNew) = loword((int32_t)(words(((hiword((uint32_t)(cshDmgDst * 0x64)) +
     * signhiword(rgpflNew[loword(t_merge_bbb5_0001_wide)]->rgcsh[ishdef])) + 0xffff), ((loword((uint32_t)(cshDmgDst * 0x64)) +
     * rgpflNew[hiword(t_merge_bbb5_0001_wide)]->rgcsh[ishdef]) + 0xffff)) / words(signhiword(rgpflNew[loword(t_merge_bb86_0001_wide)]->rgcsh[ishdef]),
     * rgpflNew[hiword(t_merge_bb86_0001_wide)]->rgcsh[ishdef]))) */
    /* untranslated: HIWORD(pctNew) = hiword((int32_t)(words(((hiword((uint32_t)(cshDmgDst * 0x64)) +
     * signhiword(rgpflNew[hiword(t_merge_bbb5_0001_wide)]->rgcsh[ishdef])) + 0xffff), ((loword((uint32_t)(cshDmgDst * 0x64)) +
     * rgpflNew[hiword(t_merge_bbb5_0001_wide)]->rgcsh[ishdef]) + 0xffff)) / sext16to32(rgpflNew[hiword(t_merge_bb86_0001_wide)]->rgcsh[ishdef]))) */
    if ((iSrc != 0x0))
        goto L_bc34;
    else
        goto L_bc2e;

L_bc2e:
    goto L_bc37;

L_bc34:

L_bc37:
    if ((iSrc != 0x0))
        goto L_bc6d;
    else
        goto L_bc67;

L_bc67:
    t_merge_bc70_0001 = 0x1;
    goto L_bc70;

L_bc6d:
    t_merge_bc70_0001 = 0x0;

L_bc70:
    /* untranslated: 0x25:[((rgpflNew[t_merge_bc70_0001] + 0x2c) + (ishdef * 0x2))] = ss:[bp-0x1bc] */
    goto L_bcf6;

L_bc90:
    if ((iSrc != 0x0))
        goto L_bca0;
    else
        goto L_bc9a;

L_bc9a:
    goto L_bca3;

L_bca0:

L_bca3:
    if ((iSrc != 0x0))
        goto L_bcd6;
    else
        goto L_bcd0;

L_bcd0:
    t_merge_bcd9_0001 = 0x1;
    goto L_bcd9;

L_bcd6:
    t_merge_bcd9_0001 = 0x0;

L_bcd9:
    /* untranslated: 0x25:[((rgpflNew[t_merge_bcd9_0001] + 0x2c) + (ishdef * 0x2))] = ss:[bp-0x1b4] */

L_bcf6:
    ishdef = (ishdef + 0x1);

L_bcfb:
    if ((ishdef < 0x10))
        goto L_afc6;
    else
        goto L_bd05;

L_bd05:
    i = 0x0;
    goto L_c1a7;

L_bd0e:
    if ((LOWORD(rgFuelCapacity[i]) != 0x0))
        goto L_bd2d;
    else
        goto L_bd24;

L_bd24:
    if ((HIWORD(rgFuelCapacity[i]) == 0x0))
        goto L_be2c;
    else
        goto L_bd2d;

L_bd2d:
    if ((HIWORD(rgpflNew[i]->rgwtMin[0x4]) > 0x0))
        goto L_bd78;
    else
        goto L_bd44;

L_bd44:
    if ((HIWORD(rgpflNew[i]->rgwtMin[0x4]) < 0x0))
        goto L_bd53;
    else
        goto L_bd49;

L_bd49:
    if ((LOWORD(rgpflNew[i]->rgwtMin[0x4]) > 0xafc8))
        goto L_bd78;
    else
        goto L_bd53;

L_bd53:
    if ((HIWORD(rgFuelCapLoss[i]) < 0x0))
        goto L_bdc4;
    else
        goto L_bd6a;

L_bd6a:
    if ((HIWORD(rgFuelCapLoss[i]) > 0x0))
        goto L_bd78;
    else
        goto L_bd6f;

L_bd6f:
    if ((LOWORD(rgFuelCapLoss[i]) <= 0xafc8))
        goto L_bdc4;
    else
        goto L_bd78;

L_bd78:
    lChg = __ftol();
    goto L_be12;

L_bdc4:
    lChg = (int32_t)(((uint32_t)((rgpflNew[i]->rgwtMin[0x4] * rgFuelCapLoss[i])) / rgFuelCapacity[i]));

L_be12:
    rgrgCargoDelta[i][0x4] = (rgrgCargoDelta[i][0x4] - lChg);

L_be2c:
    if ((LOWORD(rgCargoCapacity[i]) != 0x0))
        goto L_be4b;
    else
        goto L_be42;

L_be42:
    if ((HIWORD(rgCargoCapacity[i]) == 0x0))
        goto L_c1a2;
    else
        goto L_be4b;

L_be4b:
    wtCargoTot = 0x0;
    j = 0x0;
    goto L_be8b;

L_be5e:
    wtCargoTot = (wtCargoTot + rgpflNew[i]->rgwtMin[j]);
    j = (j + 0x1);

L_be8b:
    if ((j <= 0x3))
        goto L_be5e;
    else
        goto L_be95;

L_be95:
    if ((HIWORD(wtCargoTot) > 0x0))
        goto L_bed1;
    else
        goto L_be9e;

L_be9e:
    if ((HIWORD(wtCargoTot) < 0x0))
        goto L_bead;
    else
        goto L_bea3;

L_bea3:
    if ((LOWORD(wtCargoTot) > 0xafc8))
        goto L_bed1;
    else
        goto L_bead;

L_bead:
    if ((HIWORD(rgCargoCapLoss[i]) < 0x0))
        goto L_bf0c;
    else
        goto L_bec3;

L_bec3:
    if ((HIWORD(rgCargoCapLoss[i]) > 0x0))
        goto L_bed1;
    else
        goto L_bec8;

L_bec8:
    if ((LOWORD(rgCargoCapLoss[i]) <= 0xafc8))
        goto L_bf0c;
    else
        goto L_bed1;

L_bed1:
    wtCargoXfer = __ftol();
    goto L_bf49;

L_bf0c:
    wtCargoXfer = (int32_t)(((uint32_t)((wtCargoTot * rgCargoCapLoss[i])) / rgCargoCapacity[i]));

L_bf49:
    lChg = wtCargoXfer;
    if ((LOWORD(wtCargoXfer) != 0x0))
        goto L_bf69;
    else
        goto L_bf60;

L_bf60:
    if ((HIWORD(wtCargoXfer) == 0x0))
        goto L_c1a2;
    else
        goto L_bf69;

L_bf69:
    if ((LOWORD(wtCargoTot) != 0x0))
        goto L_bf7b;
    else
        goto L_bf72;

L_bf72:
    if ((HIWORD(wtCargoTot) == 0x0))
        goto L_c1a2;
    else
        goto L_bf7b;

L_bf7b:
    j = 0x0;
    goto L_c0c2;

L_bf84:
    if ((HIWORD(rgpflNew[i]->rgwtMin[j]) > 0x0))
        goto L_bfd0;
    else
        goto L_bfaa;

L_bfaa:
    if ((HIWORD(rgpflNew[i]->rgwtMin[j]) < 0x0))
        goto L_bfb8;
    else
        goto L_bfaf;

L_bfaf:
    if ((LOWORD(rgpflNew[i]->rgwtMin[j]) > 0xafc8))
        goto L_bfd0;
    else
        goto L_bfb8;

L_bfb8:
    if ((HIWORD(wtCargoXfer) < 0x0))
        goto L_c00e;
    else
        goto L_bfc1;

L_bfc1:
    if ((HIWORD(wtCargoXfer) > 0x0))
        goto L_bfd0;
    else
        goto L_bfc6;

L_bfc6:
    if ((LOWORD(wtCargoXfer) <= 0xafc8))
        goto L_c00e;
    else
        goto L_bfd0;

L_bfd0:
    l = __ftol();
    goto L_c050;

L_c00e:
    l = (int32_t)(((uint32_t)((rgpflNew[i]->rgwtMin[j] * wtCargoXfer)) / wtCargoTot));

L_c050:
    if ((HIWORD(l) > HIWORD(lChg)))
        goto L_c07a;
    else
        goto L_c061;

L_c061:
    if ((HIWORD(l) < HIWORD(lChg)))
        goto L_c06f;
    else
        goto L_c066;

L_c066:
    if ((LOWORD(l) >= LOWORD(lChg)))
        goto L_c07a;
    else
        goto L_c06f;

L_c06f:
    t_merge_c082_0001_wide = l;
    goto L_c082;

L_c07a:
    t_merge_c082_0001_wide = lChg;

L_c082:
    l = t_merge_c082_0001_wide;
    rgrgCargoDelta[i][j] = (rgrgCargoDelta[i][j] - l);
    lChg = (lChg - l);
    j = (j + 0x1);

L_c0c2:
    if ((j <= 0x3))
        goto L_bf84;
    else
        goto L_c0cc;

L_c0cc:
    if ((HIWORD(lChg) < 0x0))
        goto L_c1a2;
    else
        goto L_c0d6;

L_c0d6:
    if ((HIWORD(lChg) > 0x0))
        goto L_c0e5;
    else
        goto L_c0db;

L_c0db:
    if ((LOWORD(lChg) <= 0x0))
        goto L_c1a2;
    else
        goto L_c0e5;

L_c0e5:
    j = 0x0;
    goto L_c0f3;

L_c0ee:
    j = (j + 0x1);

L_c0f3:
    if ((j > 0x3))
        goto L_c1a2;
    else
        goto L_c0fd;

L_c0fd:
    if ((HIWORD(lChg) < 0x0))
        goto L_c1a2;
    else
        goto L_c107;

L_c107:
    if ((HIWORD(lChg) > 0x0))
        goto L_c116;
    else
        goto L_c10c;

L_c10c:
    if ((LOWORD(lChg) <= 0x0))
        goto L_c1a2;
    else
        goto L_c116;

L_c116:
    if (((HIWORD(rgpflNew[i]->rgwtMin[j]) + HIWORD(rgrgCargoDelta[i][j])) < 0x0))
        goto L_c0ee;
    else
        goto L_c16b;

L_c16b:
    /* untranslated: branch (ss:[bp-0x1aa] + HIWORD(rgrgCargoDelta[i][j])) > 0x0 ? L_c178 : L_c170 */

L_c170:
    /* untranslated: branch (ss:[bp-0x1ac] + LOWORD(rgrgCargoDelta[i][j])) <= 0x0 ? L_c0ee : L_c178 */

L_c178:
    rgrgCargoDelta[i][j] = (rgrgCargoDelta[i][j] - 0x1);
    lChg = (lChg - 0x1);

L_c1a2:
    i = (i + 0x1);

L_c1a7:
    if ((i < 0x2))
        goto L_bd0e;
    else
        goto L_c1b1;

L_c1b1:
    i = 0x0;
    goto L_c270;

L_c1ba:
    j = 0x0;
    goto L_c261;

L_c1c3:
    rgpflNew[i]->rgwtMin[j] = (rgpflNew[i]->rgwtMin[j] + rgrgCargoDelta[i][j]);
    if ((i != 0x0))
        goto L_c233;
    else
        goto L_c22d;

L_c22d:
    t_merge_c236_0001_wide = 0x10001;
    goto L_c236;

L_c233:
    t_merge_c236_0001_wide = 0x0;

L_c236:
    /* untranslated: LOWORD(rgpflNew[loword(t_merge_c236_0001_wide)]->rgwtMin[j]) = (LOWORD(rgpflNew[hiword(t_merge_c236_0001_wide)]->rgwtMin[j]) -
     * ss:[bp-0x1ac]) */
    /* untranslated: HIWORD(rgpflNew[hiword(t_merge_c236_0001_wide)]->rgwtMin[j]) = (HIWORD(rgpflNew[hiword(t_merge_c236_0001_wide)]->rgwtMin[j]) -
     * ss:[bp-0x1aa]) */
    j = (j + 0x1);

L_c261:
    if ((j < 0x5))
        goto L_c1c3;
    else
        goto L_c26b;

L_c26b:
    i = (i + 0x1);

L_c270:
    if ((i < 0x2))
        goto L_c1ba;
    else
        goto L_c27a;

L_c27a:
    return;
}

void DestroyAllIshdefSB(int16_t ishdefSB, int16_t iplr) {
    PLANET *lppl;
    PLANET *lpplMac;

L_c280:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_c322;

L_c2b4:
    if ((lppl->iPlayer != iplr))
        goto L_c31e;
    else
        goto L_c2c3;

L_c2c3:
    if ((lppl->fStarbase == 0x0))
        goto L_c31e;
    else
        goto L_c2da;

L_c2da:
    if ((lppl->isb != ishdefSB))
        goto L_c31e;
    else
        goto L_c2ee;

L_c2ee:
    lppl->fStarbase = 0x0;
    KillQueuedShips(lppl);
    KillQueuedMassPackets(lppl);

L_c31e:
    lppl = (lppl + 0x1);

L_c322:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_c2b4;
    else
        goto L_c330;

L_c330:
    return;
}

void DestroyAllIshdef(int16_t ishdef, int16_t iplr) {
    FLEET   flDead;
    int16_t cKill;
    FLEET  *lpfl;
    int16_t i;
    int16_t grbit;
    int16_t j;
    int16_t cDel;
    FLEET   flNew;

L_c336:
    cDel = 0x0;
    if ((ishdef < 0x10))
        goto L_c373;
    else
        goto L_c34e;

L_c34e:
    DestroyAllIshdefSB((ishdef + 0xfff0), iplr);
    InvalidateReport(0x0, 0x1);
    goto L_c5d0;

L_c373:
    lpfl = rglpfl;
    i = 0x0;
    goto L_c3b5;

L_c392:
    lpfl = rglpfl[i];

L_c3b5:
    if ((i >= cFleet))
        goto L_c5c0;
    else
        goto L_c3c1;

L_c3c1:
    if ((lpfl->iPlayer != iplr))
        goto IncrementI;
    else
        goto L_c3ce;

L_c3ce:

L_c3d4:
    if ((lpfl->rgcsh[ishdef] <= 0x0))
        goto IncrementI;
    else
        goto L_c3f3;

L_c3f3:
    memset(flDead, 0x0, 0x7c);
    cKill = lpfl->rgcsh[ishdef];
    cDel = (cDel + cKill);
    j = 0x0;
    goto L_c467;

L_c433:
    if ((j == ishdef))
        goto L_c462;
    else
        goto L_c43f;

L_c43f:
    if ((lpfl->rgcsh[j] != 0x0))
        goto L_c471;
    else
        goto L_c45c;

L_c45c:

L_c462:
    j = (j + 0x1);

L_c467:
    if ((j < 0x10))
        goto L_c433;
    else
        goto L_c471;

L_c471:
    if ((j != 0x10))
        goto L_c4b3;
    else
        goto L_c47b;

L_c47b:
    lpfl->rgcsh[ishdef] = 0x0;
    FDeleteFleet(LOWORD(lpfl), grobjNone, 0xffff);
    goto L_c392;

L_c4b3:
    flDead.id = ((flDead.id & 0xe1ff) | ((iplr & 0xf) << 0x9));
    flDead.fDead = 0x1;
    flDead.rgcsh[ishdef] = cKill;
    flNew = *(lpfl);
    flNew.rgcsh[ishdef] = 0x0;
    FleetTransferCargoBalance(&(flNew), &(flDead));
    *(lpfl) = flNew;
    if ((sel.grobj != grobjFleet))
        goto IncrementI;
    else
        goto L_c53f;

L_c53f:
    if ((sel.fl.id != flNew.id))
        goto IncrementI;
    else
        goto L_c54c;

L_c54c:
    FLookupFleet(flNew.id, 0x4972);
    RedrawScanSel(0x0, 0x0);
    FillShipDD(sel.fl.id);
    grbit = 0x83b5;
    FLookupFleet(sel.fl.id, 0x4972);
    FillFleetCompLB();
    DrawPlanShip(0x0, grbit);
    InvalidateRect(hwndMine, 0x0, 0x1);

IncrementI:
    i = (i + 0x1);
    goto L_c392;

L_c5c0:
    InvalidateReport(0x1, 0x1);

L_c5d0:
    RemoveIshdefFromAllQueues(ishdef, 0x0);
    return;
}

void RemoveIshdefFromAllQueues(int16_t ishdef, int16_t fSpaceDocks) {
    int16_t iprod;
    PLANET *lppl;
    int16_t iDst;
    PLANET *lpplMac;
    PROD   *lpprod;

L_c5e6:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_c7ce;

L_c61a:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_c631;
    else
        goto L_c627;

L_c627:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_c7ca;
    else
        goto L_c631;

L_c631:
    if ((lppl->lpplprod->iprodMac == 0x0))
        goto L_c7ca;
    else
        goto L_c647;

L_c647:
    if ((lppl->iPlayer != idPlayer))
        goto L_c7ca;
    else
        goto L_c656;

L_c656:
    if ((lppl->fStarbase == 0x0))
        goto L_c7ca;
    else
        goto L_c66d;

L_c66d:
    if ((fSpaceDocks == 0x0))
        goto L_c6a5;
    else
        goto L_c676;

L_c676:
    if ((rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef != ihuldefSpaceDock))
        goto L_c7ca;
    else
        goto L_c69f;

L_c69f:

L_c6a5:
    iDst = 0x0;
    iprod = 0x0;
    lpprod = &(lppl->lpplprod->rgprod);
    goto L_c6db;

L_c6c8:
    iprod = (iprod + 0x1);
    lpprod = (lpprod + 0x1);

L_c6db:
    if ((iprod >= lppl->lpplprod->iprodMac))
        goto L_c783;
    else
        goto L_c6f1;

L_c6f1:
    if ((lpprod->grobj != 0x2))
        goto L_c745;
    else
        goto L_c711;

L_c711:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_c745 : L_c719 */

L_c719:
    if ((lpprod->iItem != ishdef))
        goto L_c745;
    else
        goto L_c73e;

L_c73e:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) == 0x0 ? L_c6c8 : L_c745 */

L_c745:
    if ((iDst == iprod))
        goto L_c77c;
    else
        goto L_c750;

L_c750:
    lppl->lpplprod[iDst].rgprod[0x0] = lpprod;

L_c77c:
    iDst = (iDst + 0x1);

L_c783:
    if ((iDst != 0x0))
        goto L_c7b1;
    else
        goto L_c78c;

L_c78c:
    FreePl(lppl->lpplprod);
    lppl->lpplprod = 0x0;
    goto L_c7ca;

L_c7b1:
    if ((iDst == iprod))
        goto L_c7ca;
    else
        goto L_c7bc;

L_c7bc:
    lppl->lpplprod->iprodMac = LOBYTE(iDst);

L_c7ca:
    lppl = (lppl + 0x1);

L_c7ce:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_c61a;
    else
        goto L_c7dc;

L_c7dc:
    if ((sel.grobj != grobjPlanet))
        goto L_c826;
    else
        goto L_c7e6;

L_c7e6:
    if ((LOWORD(sel.pl.lpplprod) != 0x0))
        goto L_c7fa;
    else
        goto L_c7f0;

L_c7f0:
    if ((HIWORD(sel.pl.lpplprod) == 0x0))
        goto L_c826;
    else
        goto L_c7fa;

L_c7fa:
    FLookupPlanet(sel.pl.id, 0x49ee);
    FillPlanetProdLB(hwndPlanetProdLB, sel.pl.lpplprod, 0x0);

L_c826:
    return;
}

int16_t CshQueued(int16_t ishdef, int16_t *pfProgress, int16_t fSpaceDocks) {
    int16_t iprod;
    PLANET *lppl;
    int16_t csh;
    PLANET *lpplMac;
    PROD   *lpprod;

L_c82c:
    csh = 0x0;
    *(pfProgress) = 0x0;
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_c9db;

L_c86c:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_c883;
    else
        goto L_c879;

L_c879:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_c9d7;
    else
        goto L_c883;

L_c883:
    if ((lppl->lpplprod->iprodMac == 0x0))
        goto L_c9d7;
    else
        goto L_c899;

L_c899:
    if ((lppl->iPlayer != idPlayer))
        goto L_c9d7;
    else
        goto L_c8a8;

L_c8a8:
    if ((lppl->fStarbase == 0x0))
        goto L_c9d7;
    else
        goto L_c8bf;

L_c8bf:
    if ((fSpaceDocks == 0x0))
        goto L_c8f7;
    else
        goto L_c8c8;

L_c8c8:
    if ((rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef != ihuldefSpaceDock))
        goto L_c9d7;
    else
        goto L_c8f1;

L_c8f1:

L_c8f7:
    iprod = 0x0;
    lpprod = &(lppl->lpplprod->rgprod);
    goto L_c928;

L_c915:
    iprod = (iprod + 0x1);
    lpprod = (lpprod + 0x1);

L_c928:
    if ((iprod >= lppl->lpplprod->iprodMac))
        goto L_c9d7;
    else
        goto L_c93e;

L_c93e:
    if ((lpprod->grobj != 0x2))
        goto L_c915;
    else
        goto L_c95e;

L_c95e:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_c915 : L_c966 */

L_c966:
    if ((lpprod->iItem != ishdef))
        goto L_c915;
    else
        goto L_c98b;

L_c98b:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_c915 : L_c992 */

L_c992:
    csh = (csh + lpprod->cItem);
    if ((lpprod->pct != 0x0))
        goto L_c9cd;
    else
        goto L_c9c5;

L_c9c5:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) == 0x0 ? L_c915 : L_c9cd */

L_c9cd:
    *(pfProgress) = 0x1;

L_c9d7:
    lppl = (lppl + 0x1);

L_c9db:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_c86c;
    else
        goto L_c9e9;

L_c9e9:

L_c9ef:
    return csh;
}

void Merge2Fleets(FLEET *lpflDst, FLEET *lpflDel, int16_t fNoDelete) {
    FLEET   rgfl[2];
    int16_t i;

L_c9f6:
    /* untranslated: part[0:124](rgfl) = *lpflDst */
    /* untranslated: part[124:124](rgfl) = *lpflDel */
    i = 0x0;
    goto L_ca67;

L_ca38:
    rgfl[0x0].rgcsh[i] = (rgfl[0x0].rgcsh[i] + rgfl[0x1].rgcsh[i]);
    rgfl[0x1].rgcsh[i] = 0x0;
    i = (i + 0x1);

L_ca67:
    if ((i < 0x10))
        goto L_ca38;
    else
        goto L_ca71;

L_ca71:
    /* untranslated: call FleetTransferCargoBalance(rgfl, &part[124:2](rgfl)) -> callresult(void) */
    i = 0x0;
    goto L_caaa;

L_ca8b:
    FLookupFleet(0xffff, rgfl[i]);
    i = (i + 0x1);

L_caaa:
    if ((i < 0x2))
        goto L_ca8b;
    else
        goto L_cab4;

L_cab4:
    if ((fNoDelete == 0x0))
        goto L_cad4;
    else
        goto L_cabd;

L_cabd:
    lpflDel->fDead = 0x1;
    goto L_caf7;

L_cad4:
    /* untranslated: call FDeleteFleet(part[124:2](rgfl), grobjFleet, LOWORD(rgfl)) -> callresult(int16_t) */
    InvalidateReport(0x1, 0x2);

L_caf7:
    return;
}

void FleetOrdersChangeTarget(FLEET *lpflOld) {
    int16_t    id;
    POINT      pt;
    int16_t    fChg;
    FLEET     *lpfl;
    int16_t    iord;
    int16_t    iflMac;
    SCAN       scan;
    GrobjClass grobj;

L_cafe:
    fChg = 0x0;
    iflMac = 0x0;
    goto L_cb18;

L_cb14:
    iflMac = (iflMac + 0x1);

L_cb18:
    if ((iflMac >= cFleet))
        goto L_ccf9;
    else
        goto L_cb23;

L_cb23:
    lpfl = rglpfl[iflMac];
    if ((LOWORD(rglpfl[iflMac]) != 0x0))
        goto L_cb53;
    else
        goto L_cb4b;

L_cb4b:
    if ((*(rglpfl[iflMac] + 0x2) == 0x0))
        goto L_ccf9;
    else
        goto L_cb53;

L_cb53:
    if ((LOWORD(lpfl->lpplord) != 0x0))
        goto L_cb6d;
    else
        goto L_cb60;

L_cb60:
    if ((HIWORD(lpfl->lpplord) == 0x0))
        goto L_cb14;
    else
        goto L_cb67;

L_cb67:

L_cb6d:
    iord = (lpfl->cord + 0xffff);
    goto L_cced;

L_cb7d:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[iord]) >> 0x8) & 0xf) != 0x2 ? L_cce9 : L_cbab */

L_cbab:
    if ((lpfl->lpplord->rgord[iord].id != LOWORD(lpflOld)))
        goto L_cce9;
    else
        goto L_cbd7;

L_cbd7:

L_cbdd:
    if ((fChg != 0x0))
        goto L_cc70;
    else
        goto L_cbe6;

L_cbe6:
    pt.x = lpflOld->pt.x;
    pt.y = lpflOld->pt.y;
    lpflOld->pt.x = (lpflOld->pt.x + 0x1);
    /* untranslated: branch FFindNearestObject(words(pt.y, pt.x), 0x83, &scan) == 0x0 ? L_cc5d : L_cc1d */

L_cc1d:
    if (((scan.grobjFull & 0x2) == 0x0))
        goto L_cc4f;
    else
        goto L_cc2b;

L_cc2b:
    grobj = grobjFleet;
    id = LOWORD(rglpfl[scan.ifl]);
    goto L_cc68;

L_cc4f:
    grobj = grobjPlanet;
    id = scan.idpl;

L_cc5d:
    grobj = grobjOther;
    id = iord;

L_cc68:
    lpflOld->pt.x = (lpflOld->pt.x - 0x1);

L_cc70:
    lpfl->lpplord->rgord[iord].id = id;
    /* untranslated: part[6:2](lpfl->lpplord->rgord[iord]) = ((part[6:2](lpfl->lpplord->rgord[iord]) & 0xf0ff) | ((grobj & 0xf) * 0x100)) */

L_cce9:
    iord = (iord - 0x1);

L_cced:
    if ((iord < 0x0))
        goto L_cb14;
    else
        goto L_ccf3;

L_ccf3:

L_ccf9:
    return;
}

void GetTruePartCost(int16_t iPlayer, PART *ppart, uint16_t *rgCost) {
    int16_t  cExcess;
    int16_t  cCur;
    int16_t  i;
    COMPART *lpcom;

L_cd00:
    lpcom = ppart->pcom;
    i = 0x0;
    goto L_cd47;

L_cd20:
    rgCost[i] = lpcom->rgwtOreCost[i];
    i = (i + 0x1);

L_cd47:
    if ((i < 0x3))
        goto L_cd20;
    else
        goto L_cd50;

L_cd50:
    rgCost[0x3] = lpcom->resCost;
    if ((iPlayer == 0xffff))
        goto L_d28b;
    else
        goto L_cd63;

L_cd63:

L_cd69:
    if (((ppart->hs.grhst & 0x2000) != 0x0))
        goto LOtherDiddles;
    else
        goto L_cd76;

L_cd76:

L_cd7c:
    if (((ppart->hs.grhst & 0x8000) == 0x0))
        goto L_cdb1;
    else
        goto L_cd8c;

L_cd8c:
    if ((ppart->hs.iItem < 0x9))
        goto L_cdb1;
    else
        goto L_cd9d;

L_cd9d:
    if ((ppart->hs.iItem <= 0xd))
        goto LOtherDiddles;
    else
        goto L_cdab;

L_cdab:

L_cdb1:
    if (((ppart->hs.grhst & 0x8000) == 0x0))
        goto L_cde6;
    else
        goto L_cdc1;

L_cdc1:
    if ((ppart->hs.iItem < 0x0))
        goto L_cde6;
    else
        goto L_cdd2;

L_cdd2:
    if ((ppart->hs.iItem <= 0x8))
        goto LOtherDiddles;
    else
        goto L_cde0;

L_cde0:

L_cde6:
    cExcess = 0x64;
    i = 0x0;
    goto L_ce5f;

L_cdf3:
    cCur = ((uint16_t)(rgplr[iPlayer].rgTech[i]) - (uint16_t)(lpcom->rgTech[i]));
    if (((uint16_t)(lpcom->rgTech[i]) <= 0x0))
        goto L_ce5b;
    else
        goto L_ce4a;

L_ce4a:
    if ((cCur >= cExcess))
        goto L_ce5b;
    else
        goto L_ce55;

L_ce55:
    cExcess = cCur;

L_ce5b:
    i = (i + 0x1);

L_ce5f:
    if ((i < 0x6))
        goto L_cdf3;
    else
        goto L_ce68;

L_ce68:
    if ((cExcess != 0x64))
        goto L_cec1;
    else
        goto L_ce71;

L_ce71:
    i = 0x0;
    goto L_ceb8;

L_ce79:
    if (((uint16_t)(rgplr[iPlayer].rgTech[i]) >= cExcess))
        goto L_ceb4;
    else
        goto L_ce99;

L_ce99:
    cExcess = (uint16_t)(rgplr[iPlayer].rgTech[i]);

L_ceb4:
    i = (i + 0x1);

L_ceb8:
    if ((i < 0x6))
        goto L_ce79;
    else
        goto L_cec1;

L_cec1:
    if ((cExcess < 0x1))
        goto LOtherDiddles;
    else
        goto L_cec7;

L_cec7:

L_cecd:
    if ((cExcess <= 0x13))
        goto L_cedb;
    else
        goto L_ced6;

L_ced6:
    cExcess = 0x13;

L_cedb:
    if ((GetRaceGrbit(rgplr[iPlayer], ibitRaceBleedingEdgeTech) == 0x0))
        goto L_cf15;
    else
        goto L_cefb;

L_cefb:
    cExcess = LOWORD((0x5 * cExcess));
    if ((cExcess <= 0x50))
        goto L_cf2d;
    else
        goto L_cf0d;

L_cf0d:
    cExcess = 0x50;

L_cf15:
    cExcess = (cExcess * 0x4);
    if ((cExcess <= 0x4b))
        goto L_cf2d;
    else
        goto L_cf28;

L_cf28:
    cExcess = 0x4b;

L_cf2d:
    i = 0x0;
    goto L_cf90;

L_cf35:
    if ((rgCost[i] <= 0x0))
        goto L_cf8c;
    else
        goto L_cf47;

L_cf47:
    rgCost[i] = (rgCost[i] - MulDiv(rgCost[i], cExcess, 0x64));
    if ((rgCost[i] != 0x0))
        goto L_cf8c;
    else
        goto L_cf7e;

L_cf7e:
    rgCost[i] = 0x1;

L_cf8c:
    i = (i + 0x1);

L_cf90:
    if ((i < 0x4))
        goto L_cf35;
    else
        goto LOtherDiddles;

LOtherDiddles:
    if ((ppart->hs.grhst != hstSpecialSB))
        goto L_d03b;
    else
        goto L_cfa5;

L_cfa5:
    if ((GetRaceStat(rgplr[iPlayer], rsMajorAdv) != raStargate))
        goto L_d03b;
    else
        goto L_cfc5;

L_cfc5:
    if ((GetRaceStat(rgplr[iPlayer], rsMajorAdv) != raStargate))
        goto L_d03b;
    else
        goto L_cfe5;

L_cfe5:
    if ((ppart->hs.iItem < ispecialSBStargate100250))
        goto L_d03b;
    else
        goto L_cff6;

L_cff6:
    if ((ppart->hs.iItem > ispecialSBStargateAnyAny))
        goto L_d03b;
    else
        goto L_d007;

L_d007:
    i = 0x0;
    goto L_d02f;

L_d00f:
    rgCost[i] = (rgCost[i] - (rgCost[i] >> 0x2));
    i = (i + 0x1);

L_d02f:
    if ((i >= 0x4))
        goto L_d1c1;
    else
        goto L_d035;

L_d035:

L_d03b:
    if ((ppart->hs.grhst == hstBeam))
        goto L_d05c;
    else
        goto L_d046;

L_d046:
    if ((ppart->hs.grhst == hstTorp))
        goto L_d05c;
    else
        goto L_d051;

L_d051:
    if ((ppart->hs.grhst != hstBomb))
        goto L_d0b0;
    else
        goto L_d05c;

L_d05c:
    if ((GetRaceStat(rgplr[iPlayer], rsMajorAdv) != raAttack))
        goto L_d0b0;
    else
        goto L_d07c;

L_d07c:
    i = 0x0;
    goto L_d0a4;

L_d084:
    rgCost[i] = (rgCost[i] - (rgCost[i] >> 0x2));
    i = (i + 0x1);

L_d0a4:
    if ((i >= 0x4))
        goto L_d1c1;
    else
        goto L_d0aa;

L_d0aa:

L_d0b0:
    if ((ppart->hs.grhst == hstBeam))
        goto L_d0d1;
    else
        goto L_d0bb;

L_d0bb:
    if ((ppart->hs.grhst == hstTorp))
        goto L_d0d1;
    else
        goto L_d0c6;

L_d0c6:
    if ((ppart->hs.grhst != hstBomb))
        goto L_d125;
    else
        goto L_d0d1;

L_d0d1:
    if ((GetRaceStat(rgplr[iPlayer], rsMajorAdv) != raDefend))
        goto L_d125;
    else
        goto L_d0f1;

L_d0f1:
    i = 0x0;
    goto L_d119;

L_d0f9:
    rgCost[i] = (rgCost[i] + (rgCost[i] >> 0x2));
    i = (i + 0x1);

L_d119:
    if ((i >= 0x4))
        goto L_d1c1;
    else
        goto L_d11f;

L_d11f:

L_d125:
    if ((ppart->hs.grhst != hstTerra))
        goto L_d167;
    else
        goto L_d131;

L_d131:
    if ((GetRaceStat(rgplr[iPlayer], rsMajorAdv) != raTerra))
        goto L_d167;
    else
        goto L_d151;

L_d151:
    /* untranslated: rgCost[0x3] = (words(rgCost[0x3], 0x0) / 0x2) */
    goto L_d1c1;

L_d167:
    if ((ppart->hs.grhst != hstEngine))
        goto L_d1c1;
    else
        goto L_d172;

L_d172:
    if ((GetRaceGrbit(rgplr[iPlayer], ibitRaceCheapEngines) == 0x0))
        goto L_d1c1;
    else
        goto L_d192;

L_d192:
    i = 0x0;
    goto L_d1b8;

L_d19a:
    rgCost[i] = (rgCost[i] - (rgCost[i] >> 0x1));
    i = (i + 0x1);

L_d1b8:
    if ((i < 0x4))
        goto L_d19a;
    else
        goto L_d1c1;

L_d1c1:
    if ((cExcess >= 0x1))
        goto L_d27f;
    else
        goto L_d1ca;

L_d1ca:
    if ((GetRaceGrbit(rgplr[iPlayer], ibitRaceBleedingEdgeTech) == 0x0))
        goto L_d27f;
    else
        goto L_d1ea;

L_d1ea:
    if ((gd.fDontCalcBleed != 0x0))
        goto L_d27f;
    else
        goto L_d1fe;

L_d1fe:
    i = 0x0;
    goto L_d22b;

L_d206:
    if (((uint16_t)(lpcom->rgTech[i]) > 0x0))
        goto L_d234;
    else
        goto L_d221;

L_d221:

L_d227:
    i = (i + 0x1);

L_d22b:
    if ((i < 0x6))
        goto L_d206;
    else
        goto L_d234;

L_d234:
    if ((i >= 0x6))
        goto L_d270;
    else
        goto L_d23d;

L_d23d:
    gd.fBleedingEdge = 0x1;
    i = 0x0;
    goto L_d264;

L_d251:
    rgCost[i] = (rgCost[i] * 0x2);
    i = (i + 0x1);

L_d264:
    if ((i >= 0x4))
        goto L_d28b;
    else
        goto L_d26a;

L_d26a:

L_d270:
    gd.fBleedingEdge = 0x0;

L_d27f:
    gd.fBleedingEdge = 0x0;

L_d28b:
    return;
}
