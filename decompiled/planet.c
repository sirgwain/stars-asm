#include "common.h"

int32_t PlanetWndProc(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    HDC                hdc;
    PAINTSTRUCT        ps;
    XFER               xf;
    int16_t            i;
    char              *psz;
    int32_t            lSel;
    RECT               rc;
    POINT              pt;
    uint16_t           hcs;
    DRAWITEMSTRUCT    *lpdis;
    MEASUREITEMSTRUCT *lpmis;
    PLANET            *lpplMac;
    PLANET            *lppl;
    FLEET             *lpfl;
    HWND               t_merge_0021_0001;
    uint32_t           t_merge_0062_0001_wide;
    uint16_t           t_merge_008a_0001;
    uint16_t           t_merge_0543_0001;

L_0000:
    goto L_0c91;

L_000f:
    if ((wParam == 0x0))
        goto L_001e;
    else
        goto L_0018;

L_0018:
    t_merge_0021_0001 = hwnd;
    goto L_0021;

L_001e:
    t_merge_0021_0001 = 0x0;

L_0021:
    hwndActive = t_merge_0021_0001;
    goto L_0d04;

L_0027:
    SetPlanetTitleBar(hwnd);
    i = 0x0;
    goto L_00d1;

L_003b:
    if ((i != 0x1))
        goto L_005c;
    else
        goto L_0053;

L_0053:
    t_merge_0062_0001_wide = 0x2100000;
    goto L_0062;

L_005c:
    t_merge_0062_0001_wide = 0x0;

L_0062:
    if ((i != 0x2))
        goto L_0087;
    else
        goto L_0081;

L_0081:
    t_merge_008a_0001 = 0xa0;
    goto L_008a;

L_0087:
    t_merge_008a_0001 = 0x50;

L_008a:
    /* untranslated: rghwndOrderDD[i] = CreateWindow(szCombobox, "OrdDD", words((loword(t_merge_0062_0001_wide) | 0x4020), (hiword(t_merge_0062_0001_wide) |
     * 0x3)), 0x64, 0x64, 0xc8, t_merge_008a_0001, hwnd, 0x0, hInst, 0x0) */
    SendMessage(rghwndOrderDD[i], WM_SETFONT, rghfontArial8[0x1], 0x0);
    i = (i + 0x1);

L_00d1:
    if ((i < 0x3))
        goto L_003b;
    else
        goto L_00db;

L_00db:
    i = 0x63;
    goto L_0112;

L_00e4:
    psz = PszGetCompressedString(i);
    /* untranslated: call SendMessage(rghwndOrderDD[0x0], CB_ADDSTRING, 0x0, words(ds, psz)) -> callresult(LRESULT) */
    i = (i + 0x1);

L_0112:
    if ((i < 0x6d))
        goto L_00e4;
    else
        goto L_011c;

L_011c:
    hwndOrderED = CreateWindow(szEdit, 0x0, 0x40800002, 0x64, 0x64, 0xc8, 0x32, hwnd, 0x0, hInst, 0x0);
    SendMessage(hwndOrderED, CB_SETEXTENDEDUI, 0x4, 0x0);
    SendMessage(hwndOrderED, WM_SETFONT, rghfontArial8[0x1], 0x0);
    lpfnRealEditProc = GetWindowLong(hwndOrderED, 0xfffc);
    SetWindowLong(hwndOrderED, 0xfffc, lpfnFakeEditProc);
    hwndBattleDD = CreateWindow(szCombobox, "BattleDD", 0x40200003, 0x64, 0x64, 0xc8, 0x50, hwnd, 0x0, hInst, 0x0);
    SendMessage(hwndBattleDD, WM_SETFONT, rghfontArial8[0x1], 0x0);
    hwndShipDD = CreateWindow(szCombobox, "ShipDD", 0x40200213, 0x64, 0x64, 0xc8, 0x50, hwnd, 0x0, hInst, 0x0);
    SendMessage(hwndShipDD, WM_SETFONT, rghfontArial8[0x1], 0x0);
    GetClientRect(hwndShipDD, &(rc));
    dyShipDD = rc.bottom;
    hwndShipLB = CreateWindow(szListbox, "ShipLB", 0x40a01001, 0x64, 0x64, 0xc8, 0x50, hwnd, 0x0, hInst, 0x0);
    SendMessage(hwndShipLB, WM_SETFONT, rghfontArial8[0x1], 0x0);
    hwndFleetCompLB = CreateWindow(szListbox, "FleetCompLB", 0x40a00051, 0x64, 0x64, 0xc8, 0x50, hwnd, 0x0, hInst, 0x0);
    SendMessage(hwndFleetCompLB, WM_SETFONT, rghfontArial8[0x0], 0x0);
    hwndPlanetProdLB = CreateWindow(szListbox, "PlanetProdLB", 0x40a00051, 0x64, 0x64, 0xc8, 0x50, hwnd, 0x0, hInst, 0x0);
    SendMessage(hwndPlanetProdLB, WM_SETFONT, rghfontArial8[0x0], 0x0);
    i = 0x0;
    goto L_0420;

L_039d:
    psz = PszGetCompressedString((i + 0x19f));
    rghwndBtn[i] = CreateWindow(szButton, psz, WS_CHILD, 0x64, 0x64, 0x64, (dyArial8 * 0x2), hwnd, 0x0, hInst, 0x0);
    SendMessage(rghwndBtn[i], WM_SETFONT, rghfontArial8[0x1], 0x0);
    i = (i + 0x1);

L_0420:
    if ((i < 0xd))
        goto L_039d;
    else
        goto L_042a;

L_042a:
    hwndRepCB = CreateWindow(szButton, PszGetCompressedString(idsRepeatOrders), 0x40000003, 0x64, 0x64, 0x96, dyArial8, hwnd, 0x0, hInst, 0x0);
    SendMessage(hwndRepCB, WM_SETFONT, rghfontArial8[0x1], 0x0);
    goto L_0d04;

L_0490:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x10000;

L_04bd:
    if ((LOWORD(lParam) != hwndRepCB))
        goto Default;
    else
        goto L_04cc;

L_04cc:
    SetBkColor(wParam, crButtonFace);
    SetTextColor(wParam, crButtonText);
    /* untranslated: return words(hbrButtonFace, 0x0) */

L_04f8:
    /* untranslated: part[12:2](lParam) = ((dxWinFrame * 0x2) + 0xc6) */
    /* untranslated: part[14:2](lParam) = (((dyWinFrame * 0x2) + 0xc6) + dyTitleBar) */
    goto Default;

L_0527:
    SetFocus(hwndFrame);
    if ((message != WM_RBUTTONDOWN))
        goto L_0540;
    else
        goto L_053a;

L_053a:
    t_merge_0543_0001 = 0x1;
    goto L_0543;

L_0540:
    t_merge_0543_0001 = 0x0;

L_0543:
    PlanetClick(LOWORD(lParam), (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), wParam, t_merge_0543_0001);
    goto L_0d04;

L_056f:
    hdc = BeginPaint(hwnd, &(ps));
    DrawPlanShip(hdc, 0xfff);
    EndPaint(hwnd, &(ps));
    goto L_0d04;

L_05a2:
    hcs = 0x0;
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    GetClientRect(hwnd, &(rc));
    /* untranslated: branch PtInRect(&rc, words(pt.y, pt.x)) == 0x0 ? L_0c77 : L_05f2 */

L_05f2:
    /* untranslated: hcs = ClickInShipOrders(words(pt.y, pt.x), 0x0, 0x1, 0x0) */
    if ((hcs != 0x0))
        goto L_063c;
    else
        goto L_061c;

L_061c:
    /* untranslated: hcs = ClickInPlanetOrders(words(pt.y, pt.x), 0x0, 0x1, 0x0) */

L_063c:
    if ((hcs == 0x0))
        goto Default;
    else
        goto L_0646;

L_0646:
    SetCursor(hcs);
    return 0x10000;

L_065b:
    lpdis = lParam;
    if ((lpdis->itemID != 0xffff))
        goto L_068e;
    else
        goto L_0677;

L_0677:
    HandleFocusState(lpdis, 0xfffe);
    goto L_06fc;

L_068e:
    goto L_06e1;

L_0699:
    DrawCBEntireItem(lpdis, 0xfffc);
    goto L_06fc;

L_06b0:
    DrawCBEntireItem(lpdis, 0xfffc);
    goto L_06fc;

L_06c7:
    DrawCBEntireItem(lpdis, 0xfffc);
    goto L_06fc;

L_06e1:
    if ((lpdis->itemAction == 0x1))
        goto L_0699;
    else
        goto L_06e9;

L_06e9:
    if ((lpdis->itemAction == 0x2))
        goto L_06b0;
    else
        goto L_06f1;

L_06f1:
    if ((lpdis->itemAction == 0x4))
        goto L_06c7;
    else
        goto L_06fc;

L_06fc:
    return 0x10000;

L_0705:
    lpmis = lParam;
    lpmis->itemHeight = (dyArial8 + 0x2);
    return 0x10000;

L_072a:
    if ((wParam == 0x66))
        goto L_073c;
    else
        goto L_0733;

L_0733:
    if ((wParam != 0x46))
        goto L_0d04;
    else
        goto L_073c;

L_073c:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_07bd;

L_076f:
    if ((lppl->iPlayer != idPlayer))
        goto L_07b8;
    else
        goto L_077f;

L_077f:
    if ((sel.grobj != grobjPlanet))
        goto L_079c;
    else
        goto L_0789;

L_0789:
    if ((sel.id == lppl->id))
        goto L_07ce;
    else
        goto L_0796;

L_0796:

L_079c:
    SelectAdjPlanet(0x0, lppl->id);
    return 0x0;

L_07b8:
    lppl = (lppl + 0x1);

L_07bd:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_076f;
    else
        goto L_07ce;

L_07ce:
    i = 0x0;
    goto L_07dc;

L_07d7:
    i = (i + 0x1);

L_07dc:
    if ((i >= cFleet))
        goto L_0d04;
    else
        goto L_07e8;

L_07e8:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_081b;
    else
        goto L_0813;

L_0813:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_0d04;
    else
        goto L_081b;

L_081b:
    if ((lpfl->iPlayer != idPlayer))
        goto L_07d7;
    else
        goto L_082b;

L_082b:
    if ((sel.grobj != grobjFleet))
        goto L_0848;
    else
        goto L_0835;

L_0835:
    if ((sel.id == lpfl->id))
        goto L_0d04;
    else
        goto L_0842;

L_0842:

L_0848:
    SelectAdjFleet(0x0, lpfl->id);
    return 0x0;

L_086a:
    if ((sel.grobj != grobjFleet))
        goto L_088b;
    else
        goto L_0874;

L_0874:
    ShipCommandProc(hwnd, wParam, lParam);
    goto L_0d04;

L_088b:
    if ((LOWORD(lParam) != hwndShipDD))
        goto L_08d5;
    else
        goto L_089a;

L_089a:
    goto L_08c7;

L_08b1:
    DrawPlanShip(0x0, 0x8004);
    goto Default;

L_08c7:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x1))
        goto Default;
    else
        goto L_08cc;

L_08cc:

L_08d5:
    if ((LOWORD(lParam) != rghwndBtn[0x4]))
        goto L_0950;
    else
        goto L_08e4;

L_08e4:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_0950;
    else
        goto L_0900;

L_0900:
    if ((GetKeyState(0x10) >= 0x0))
        goto L_0931;
    else
        goto L_0911;

L_0911:
    SelectAdjPlanet(0x0, IdFindAdjStarbase(sel.pl.id, 0x0));
    goto LRefocus;

L_0931:
    SelectAdjPlanet(0xffff, 0x0);

LRefocus:
    SetFocus(hwndFrame);
    goto L_0d04;

L_0950:
    if ((LOWORD(lParam) != rghwndBtn[0x5]))
        goto L_09c2;
    else
        goto L_095f;

L_095f:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_09c2;
    else
        goto L_097b;

L_097b:
    if ((GetKeyState(0x10) >= 0x0))
        goto L_09ac;
    else
        goto L_098c;

L_098c:
    SelectAdjPlanet(0x0, IdFindAdjStarbase(sel.pl.id, 0x1));
    goto LRefocus;

L_09ac:
    SelectAdjPlanet(0x1, 0x0);

L_09c2:
    if ((LOWORD(lParam) != rghwndBtn[0x0]))
        goto L_0a70;
    else
        goto L_09d1;

L_09d1:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_0a70;
    else
        goto L_09ed;

L_09ed:
    lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0x0);
    if ((LOWORD(lSel) != 0xffff))
        goto L_0a25;
    else
        goto L_0a18;

L_0a18:
    if ((HIWORD(lSel) == 0xffff))
        goto L_0d04;
    else
        goto L_0a1f;

L_0a1f:

L_0a25:
    if ((FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &(xf), 0xffff) == 0x0))
        goto L_0d04;
    else
        goto L_0a48;

L_0a48:

L_0a4e:
    TransferStuff(sel.pl.id, grobjPlanet, xf.id, xf.grobj, 0x0);
    goto LRefocus;

L_0a70:
    if ((LOWORD(lParam) != rghwndBtn[0x1]))
        goto L_0b19;
    else
        goto L_0a7f;

L_0a7f:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_0b19;
    else
        goto L_0a9b;

L_0a9b:
    lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0x0);
    if ((LOWORD(lSel) != 0xffff))
        goto L_0ad3;
    else
        goto L_0ac6;

L_0ac6:
    if ((HIWORD(lSel) == 0xffff))
        goto LRefocus;
    else
        goto L_0acd;

L_0acd:

L_0ad3:
    if ((FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &(xf), 0xffff) == 0x0))
        goto LRefocus;
    else
        goto L_0af9;

L_0af9:
    if ((xf.grobj != grobjFleet))
        goto LRefocus;
    else
        goto L_0b03;

L_0b03:
    SelectAdjFleet(0x0, xf.id);

L_0b19:
    if ((LOWORD(lParam) != rghwndBtn[0x2]))
        goto L_0b66;
    else
        goto L_0b28;

L_0b28:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_0b66;
    else
        goto L_0b44;

L_0b44:
    pt.x = 0x262;
    pt.y = 0x1d6;
    /* untranslated: call ShipBuilder(words(pt.y, pt.x)) -> callresult(int16_t) */
    goto LRefocus;

L_0b66:
    if ((LOWORD(lParam) != rghwndBtn[0xb]))
        goto L_0ba3;
    else
        goto L_0b75;

L_0b75:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_0ba3;
    else
        goto L_0b91;

L_0b91:
    ChangeProduction(0x0);
    goto LRefocus;

L_0ba3:
    if ((LOWORD(lParam) != rghwndBtn[0xc]))
        goto L_0c0c;
    else
        goto L_0bb2;

L_0bb2:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_0c0c;
    else
        goto L_0bce;

L_0bce:
    if ((AlertSz(PszFormatIds(idsSureWantDeleteEverythingPlanetsProductionQueue, 0x0), 0x2024) != 0x6))
        goto L_0d04;
    else
        goto L_0bf4;

L_0bf4:

L_0bfa:
    ChangeProduction(0x1);
    goto LRefocus;

L_0c0c:
    if ((LOWORD(lParam) != hwndPlanetProdLB))
        goto Default;
    else
        goto L_0c1b;

L_0c1b:
    goto L_0c6c;

L_0c32:
    DrawPlanShip(0x0, 0x40);
    if ((gd.fTutorial == 0x0))
        goto Default;
    else
        goto L_0c55;

L_0c55:
    tutor.fProgress = 0x1;
    AdvanceTutor();

L_0c6c:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) == 0x1))
        goto L_0c32;
    else
        goto Default;

Default:
    return DefWindowProc(hwnd, message, wParam, lParam);

L_0c91:
    if ((message == WM_CREATE))
        goto L_0027;
    else
        goto L_0c99;

L_0c99:
    if ((message == WM_PAINT))
        goto L_056f;
    else
        goto L_0ca1;

L_0ca1:
    if ((message == WM_ERASEBKGND))
        goto L_0490;
    else
        goto L_0ca9;

L_0ca9:
    if ((message == WM_CTLCOLOR))
        goto L_04bd;
    else
        goto L_0cb1;

L_0cb1:
    if ((message == WM_SETCURSOR))
        goto L_05a2;
    else
        goto L_0cb9;

L_0cb9:
    if ((message == WM_GETMINMAXINFO))
        goto L_04f8;
    else
        goto L_0cc1;

L_0cc1:
    if ((message == WM_DRAWITEM))
        goto L_065b;
    else
        goto L_0cc9;

L_0cc9:
    if ((message == WM_MEASUREITEM))
        goto L_0705;
    else
        goto L_0cd1;

L_0cd1:
    if ((message == WM_CHAR))
        goto L_072a;
    else
        goto L_0cd9;

L_0cd9:
    if ((message == WM_COMMAND))
        goto L_086a;
    else
        goto L_0ce1;

L_0ce1:
    if ((message == WM_LBUTTONDOWN))
        goto L_0527;
    else
        goto L_0ce9;

L_0ce9:
    if ((message == WM_LBUTTONDBLCLK))
        goto L_0527;
    else
        goto L_0cf1;

L_0cf1:
    if ((message == WM_RBUTTONDOWN))
        goto L_0527;
    else
        goto L_0cf9;

L_0cf9:
    if ((message != WM_MDIACTIVATE))
        goto Default;
    else
        goto L_0cfe;

L_0cfe:

L_0d04:
    return 0x0;
}

void DrawPlanShip(HDC hdc, int16_t grbit) {
    HFONT    hfontSav;
    OBJ      objNull;
    int16_t  ctile;
    COLORREF crFore;
    OBJ      obj;
    int16_t  fMin;
    int16_t  i;
    COLORREF crBack;
    int16_t  fErase;
    TILE    *ptile;
    int16_t  fDC;
    RECT     rc;
    uint16_t t_merge_0f2d_0001;
    uint16_t t_merge_0f47_0001;
    OBJ      t_merge_100a_0001;

L_0d16:
    fDC = 0x0;
    objNull = 0x0;
    if ((sel.id != 0xffff))
        goto L_0e96;
    else
        goto L_0d33;

L_0d33:
    i = 0x0;
    goto L_0d51;

L_0d3b:
    ShowWindow(rghwndBtn[i], SW_HIDE);
    i = (i + 0x1);

L_0d51:
    if ((i < 0xd))
        goto L_0d3b;
    else
        goto L_0d5a;

L_0d5a:
    i = 0x0;
    goto L_0d78;

L_0d62:
    ShowWindow(rghwndOrderDD[i], SW_HIDE);
    i = (i + 0x1);

L_0d78:
    if ((i < 0x3))
        goto L_0d62;
    else
        goto L_0d81;

L_0d81:
    ShowWindow(hwndOrderED, SW_HIDE);
    ShowWindow(hwndShipDD, SW_HIDE);
    ShowWindow(hwndBattleDD, SW_HIDE);
    ShowWindow(hwndShipLB, SW_HIDE);
    ShowWindow(hwndFleetCompLB, SW_HIDE);
    ShowWindow(hwndPlanetProdLB, SW_HIDE);
    ShowWindow(hwndRepCB, SW_HIDE);
    i = 0x0;
    goto L_0e0e;

L_0de4:
    rgrcRef[i].bottom = 0xfffa;
    rgrcRef[i].top = 0xfffb;
    i = (i + 0x1);

L_0e0e:
    if ((i < 0x13))
        goto L_0de4;
    else
        goto L_0e17;

L_0e17:
    if (((rgplr[idPlayer].wFlags & 0x1) == 0x0))
        goto L_1080;
    else
        goto L_0e31;

L_0e31:
    if ((hdc == 0x0))
        goto L_1080;
    else
        goto L_0e3a;

L_0e3a:
    GetClientRect(hwndPlanet, &(rc));
    SetBkColor(hdc, crButtonFace);
    SetTextColor(hdc, crButtonText);
    i = CchGetString(idsDeceased, szWork);
    DiaganolTextOut(hdc, &(rc), szWork, i);

L_0e96:
    if ((sel.grobj != grobjFleet))
        goto L_0eb2;
    else
        goto L_0ea0;

L_0ea0:
    ptile = 0x90e;
    ctile = 0x7;
    obj = 0x4972;
    goto L_0ec1;

L_0eb2:
    ptile = 0x7fc;
    ctile = 0x6;
    obj = 0x49ee;

L_0ec1:
    if ((hdc != 0x0))
        goto L_0edb;
    else
        goto L_0eca;

L_0eca:
    fDC = 0x1;
    hdc = GetDC(hwndPlanet);

L_0edb:
    hfontSav = SelectObject(hdc, rghfontArial8[0x0]);
    crBack = SetBkColor(hdc, crButtonFace);
    crFore = SetTextColor(hdc, crButtonText);
    if (((grbit & 0x8000) == 0x0))
        goto L_0f2a;
    else
        goto L_0f24;

L_0f24:
    t_merge_0f2d_0001 = 0x1;
    goto L_0f2d;

L_0f2a:
    t_merge_0f2d_0001 = 0x0;

L_0f2d:
    fErase = t_merge_0f2d_0001;
    if (((grbit & 0x4000) == 0x0))
        goto L_0f44;
    else
        goto L_0f3e;

L_0f3e:
    t_merge_0f47_0001 = 0x1;
    goto L_0f47;

L_0f44:
    t_merge_0f47_0001 = 0x0;

L_0f47:
    fMin = t_merge_0f47_0001;
    i = 0x0;
    goto L_1039;

L_0f52:
    if (((grbit & ptile[i].grbit) == 0x0))
        goto L_1035;
    else
        goto L_0f70;

L_0f70:
    /* untranslated: part[10:2](ptile[i]) = ((part[10:2](ptile[i]) & 0xfbff) | ((fErase & 0x1) << 0xa)) */
    /* untranslated: part[10:2](ptile[i]) = ((part[10:2](ptile[i]) & 0xefff) | ((fMin & 0x1) << 0xc)) */
    /* untranslated: branch ((part[10:2](ptile[i]) >> 0x8) & 0x1) == 0x0 ? L_1007 : L_1001 */

L_1001:
    t_merge_100a_0001 = objNull;
    goto L_100a;

L_1007:
    t_merge_100a_0001 = obj;

L_100a:
    ptile[i].pfn(hdc, &(ptile[i]), t_merge_100a_0001);

L_1035:
    i = (i + 0x1);

L_1039:
    if ((i < ctile))
        goto L_0f52;
    else
        goto L_1044;

L_1044:
    SetTextColor(hdc, crFore);
    SetBkColor(hdc, crBack);
    SelectObject(hdc, hfontSav);
    if ((fDC == 0x0))
        goto L_1080;
    else
        goto L_1074;

L_1074:
    ReleaseDC(hwndPlanet, hdc);

L_1080:
    return;
}

int16_t FDrawTileNC(HDC hdc, TILE *ptile, RECT *prc, char *pszTitle) {
    int16_t bt;
    RECT    rcT;
    int16_t t_merge_10e8_0001;

L_1086:
    bt = 0x70;
    prc->left = (LOWORD((ptile->iCol * 0xc6)) + 0x4);
    prc->right = (prc->left + 0xbe);
    prc->top = ptile->yTop;
    if ((ptile->fPopped == 0x0))
        goto L_10e2;
    else
        goto L_10d9;

L_10d9:
    t_merge_10e8_0001 = ptile->dyFull;
    goto L_10e8;

L_10e2:
    t_merge_10e8_0001 = (dyArial8 + 0x3);

L_10e8:
    prc->bottom = (t_merge_10e8_0001 + prc->top);
    if ((ptile->fMinDraw == 0x0))
        goto L_1123;
    else
        goto L_110a;

L_110a:
    if ((ptile->fMinTitle == 0x0))
        goto FinishUp;
    else
        goto L_111d;

L_111d:

L_1123:
    if ((ptile->fMinDraw != 0x0))
        goto L_114b;
    else
        goto L_1139;

L_1139:
    _Draw3dFrame(hdc, prc, 0x0);

L_114b:
    rcT = *(prc);
    ExpandRc(&(rcT), 0xffff, 0xffff);
    rcT.bottom = ((rcT.top + dyArial8) + 0x2);
    SelectObject(hdc, rghfontArial8[0x1]);
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    if ((ptile->fMinDraw != 0x0))
        goto L_11d0;
    else
        goto L_11bd;

L_11bd:
    _Draw3dFrame(hdc, &(rcT), 0x0);

L_11d0:
    RcCtrTextOut(hdc, &(rcT), pszTitle, 0xffff);
    SetRect(&(rcT), (prc->right + 0xffef), (prc->top + 0x1), prc->right, (rcT.bottom + 0x1));
    if ((ptile->fPopped == 0x0))
        goto L_1230;
    else
        goto L_1229;

L_1229:
    bt = (bt | 0x0);
    goto L_1234;

L_1230:
    bt = (bt | 0x1);

L_1234:
    DrawBtn(hdc, &(rcT), bt, 0x0, 0x0);
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, (prc->right + 0xffee), (prc->top + 0x1), 0x1, ((rcT.bottom - prc->top) + 0xffff), PATCOPY);

FinishUp:
    prc->top = (prc->top + (dyArial8 + 0x4));

L_12ac:
    return ptile->fPopped;
}

void DrawPlanetMinSum(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t dxRight;
    int16_t yTop;
    int16_t xRight;
    int16_t c;
    int16_t i;
    int16_t xLeft;
    HBRUSH  hbrSav;
    PLANET *ppl;
    RECT    rc;
    int16_t t_149b;

L_12b2:
    ppl = obj;
    if ((ptile->fFixCtls == 0x0))
        goto L_131f;
    else
        goto L_12d7;

L_12d7:
    rgrcRef[0x6].top = 0xfffb;
    rgrcRef[0x6].bottom = 0xfffa;
    rgrcRef[0x7].top = 0xfffb;
    rgrcRef[0x7].bottom = 0xfffa;
    rgrcRef[0x8].top = 0xfffb;
    rgrcRef[0x8].bottom = 0xfffa;
    rgrcRef[0x9].top = 0xfffb;
    rgrcRef[0x9].bottom = 0xfffa;
    rgrcRef[0xb].top = 0xfffb;
    rgrcRef[0xb].bottom = 0xfffa;
    rgrcRef[0xa].top = 0xfffb;
    rgrcRef[0xa].bottom = 0xfffa;

L_131f:
    if ((FDrawTileNC(hdc, ptile, &(rc), PszGetCompressedString(idsMineralsHand)) == 0x0))
        goto L_170f;
    else
        goto L_1343;

L_1343:

L_1349:
    if ((ppl != 0x0))
        goto L_1357;
    else
        goto L_1352;

L_1352:
    ppl = 0x49ee;

L_1357:
    xLeft = (rc.left + 0x4);
    xRight = (rc.right + 0xfffc);
    yTop = rc.top;
    dxRight = dxMaxMineralQuan;
    SetRect(&(rgrcRef[0x6]), xLeft, yTop, xRight, (LOWORD((0x3 * dyArial8)) + yTop));
    i = 0x0;
    goto L_147d;

L_139d:
    if ((ptile->fMinDraw != 0x0))
        goto L_1404;
    else
        goto L_13b3;

L_13b3:
    SelectObject(hdc, rghfontArial8[0x1]);
    SetTextColor(hdc, rgcrMinerals[i]);
    TextOut(hdc, xLeft, yTop, rgszMinerals[i], lstrlen(rgszMinerals[i]));

L_1404:
    SelectObject(hdc, rghfontArial8[0x0]);
    SetTextColor(hdc, crButtonText);
    c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), LOWORD(ppl->rgwtMin[i]), HIWORD(ppl->rgwtMin[i]));
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop = (yTop + dyArial8);
    i = (i + 0x1);

L_147d:
    if ((i <= 0x2))
        goto L_139d;
    else
        goto L_1486;

L_1486:
    hbrSav = SelectObject(hdc, hbrButtonHilite);
    t_149b = yTop;
    yTop = (yTop + 0x1);
    PatBlt(hdc, rc.left, t_149b, (rc.right - rc.left), 0x1, PATCOPY);
    SelectObject(hdc, hbrSav);
    SetRect(&(rgrcRef[0x7]), xLeft, yTop, xRight, ((dyArial8 * 0x2) + yTop));
    if ((ptile->fMinDraw != 0x0))
        goto L_153d;
    else
        goto L_14fa;

L_14fa:
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsMines4, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    SelectObject(hdc, rghfontArial8[0x0]);

L_153d:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_1593;
    else
        goto L_155e;

L_155e:
    c = _wsprintf(szWork, PszGetCompressedString(idsD), CMinesOperating(ppl));
    goto L_15e5;

L_1593:
    /* untranslated: c = _wsprintf(szWork, PszGetCompressedString(idsDD), (loword((uint32_t)(words(*(ppl+0x14), *(ppl+0x16)) >> 0x8)) & 0xfff),
     * CMaxOperableMines(ppl, idPlayer, 0x0)) */

L_15e5:
    RightTextOut(hdc, xRight, yTop, szWork, c, (dxRight * 0x2));
    yTop = (yTop + dyArial8);
    if ((ptile->fMinDraw != 0x0))
        goto L_1662;
    else
        goto L_161f;

L_161f:
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsFactories4, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    SelectObject(hdc, rghfontArial8[0x0]);

L_1662:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_1699;
    else
        goto L_1683;

L_1683:
    c = CchGetString(idsN, szWork);
    goto L_16eb;

L_1699:
    /* untranslated: c = _wsprintf(szWork, PszGetCompressedString(idsDD), (loword((uint32_t)(words(*(ppl+0x14), *(ppl+0x16)) >> 0x14)) & 0xfff),
     * CMaxOperableFactories(ppl, idPlayer, 0x0)) */

L_16eb:
    RightTextOut(hdc, xRight, yTop, szWork, c, (dxRight * 0x2));
    yTop = (yTop + dyArial8);

L_170f:
    return;
}

void DrawPlanetStats(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t  dxRight;
    int32_t  l2;
    int16_t  yTop;
    int16_t  xRight;
    int16_t  c;
    int16_t  cRes;
    int16_t  dRangeP;
    float    pct;
    int16_t  cResAvail;
    int16_t  dRange;
    char    *psz;
    int16_t  xLeft;
    HBRUSH   hbrSav;
    int32_t  l;
    RECT     rc;
    PART     part;
    int16_t  t_1b78;
    int16_t  t_1e3b;
    uint16_t t_merge_2023_0001;
    uint16_t t_merge_21c3_0001;

L_1716:
    if ((FDrawTileNC(hdc, ptile, &(rc), PszGetCompressedString(idsStatus)) == 0x0))
        goto L_21ef;
    else
        goto L_1743;

L_1743:

L_1749:
    xLeft = (rc.left + 0x4);
    xRight = (rc.right + 0xfffc);
    yTop = rc.top;
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsPopulation4, szWork);
    l = GetTextExtent(hdc, szWork, c);
    c = CchGetString(idsScannerType, szWork);
    l2 = GetTextExtent(hdc, szWork, c);
    if ((HIWORD(l2) < HIWORD(l)))
        goto L_17ea;
    else
        goto L_17d1;

L_17d1:
    if ((HIWORD(l2) > HIWORD(l)))
        goto L_17de;
    else
        goto L_17d6;

L_17d6:
    if ((LOWORD(l2) <= LOWORD(l)))
        goto L_17ea;
    else
        goto L_17de;

L_17de:
    l = l2;

L_17ea:
    c = CchGetString(idsScannerRange, szWork);
    l2 = GetTextExtent(hdc, szWork, c);
    if ((HIWORD(l2) < HIWORD(l)))
        goto L_183c;
    else
        goto L_1823;

L_1823:
    if ((HIWORD(l2) > HIWORD(l)))
        goto L_1830;
    else
        goto L_1828;

L_1828:
    if ((LOWORD(l2) <= LOWORD(l)))
        goto L_183c;
    else
        goto L_1830;

L_1830:
    l = l2;

L_183c:
    c = CchGetString(idsDefenses4, szWork);
    l2 = GetTextExtent(hdc, szWork, c);
    if ((HIWORD(l2) < HIWORD(l)))
        goto L_188e;
    else
        goto L_1875;

L_1875:
    if ((HIWORD(l2) > HIWORD(l)))
        goto L_1882;
    else
        goto L_187a;

L_187a:
    if ((LOWORD(l2) <= LOWORD(l)))
        goto L_188e;
    else
        goto L_1882;

L_1882:
    l = l2;

L_188e:
    c = CchGetString(idsDefenseType, szWork);
    l2 = GetTextExtent(hdc, szWork, c);
    if ((HIWORD(l2) < HIWORD(l)))
        goto L_18e0;
    else
        goto L_18c7;

L_18c7:
    if ((HIWORD(l2) > HIWORD(l)))
        goto L_18d4;
    else
        goto L_18cc;

L_18cc:
    if ((LOWORD(l2) <= LOWORD(l)))
        goto L_18e0;
    else
        goto L_18d4;

L_18d4:
    l = l2;

L_18e0:
    c = CchGetString(idsDefCoverage, szWork);
    l2 = GetTextExtent(hdc, szWork, c);
    if ((HIWORD(l2) < HIWORD(l)))
        goto L_1932;
    else
        goto L_1919;

L_1919:
    if ((HIWORD(l2) > HIWORD(l)))
        goto L_1926;
    else
        goto L_191e;

L_191e:
    if ((LOWORD(l2) <= LOWORD(l)))
        goto L_1932;
    else
        goto L_1926;

L_1926:
    l = l2;

L_1932:
    c = CchGetString(idsResourcesYear, szWork);
    l2 = GetTextExtent(hdc, szWork, c);
    if ((HIWORD(l2) < HIWORD(l)))
        goto L_1984;
    else
        goto L_196b;

L_196b:
    if ((HIWORD(l2) > HIWORD(l)))
        goto L_1978;
    else
        goto L_1970;

L_1970:
    if ((LOWORD(l2) <= LOWORD(l)))
        goto L_1984;
    else
        goto L_1978;

L_1978:
    l = l2;

L_1984:
    dxRight = ((xRight - xLeft) - LOWORD(l));
    if ((ptile->fMinDraw != 0x0))
        goto L_19d6;
    else
        goto L_19ab;

L_19ab:
    c = CchGetString(idsPopulation4, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);

L_19d6:
    SelectObject(hdc, rghfontArial8[0x0]);
    SetRect(&(rgrcRef[0x9]), xLeft, yTop, xRight, (yTop + dyArial8));
    c = CommaFormatLong(szWork, (uint32_t)((sel.pl.rgwtMin[0x3] * 0x64)));
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop = (yTop + dyArial8);
    if ((ptile->fMinDraw != 0x0))
        goto L_1a9f;
    else
        goto L_1a5c;

L_1a5c:
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsResourcesYear, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    SelectObject(hdc, rghfontArial8[0x0]);

L_1a9f:
    SetRect(&(rgrcRef[0x8]), xLeft, yTop, xRight, (yTop + dyArial8));
    CResourcesAtPlanet(&(sel.pl), idPlayer);
    /* untranslated: cResAvail = callresult(int16_t) */
    /* untranslated: cRes = callresult(int16_t) */
    if ((sel.pl.fNoResearch != 0x0))
        goto L_1b1a;
    else
        goto L_1af2;

L_1af2:
    if ((0x0 != 0x0))
        goto L_1b1a;
    else
        goto L_1afa;

L_1afa:
    cResAvail = (cResAvail - MulDiv(cRes, (uint16_t)(rgplr[idPlayer].pctResearch), 0x64));

L_1b1a:
    c = _wsprintf(szWork, PszGetCompressedString(idsDD), cResAvail, cRes);
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop = (yTop + dyArial8);
    hbrSav = SelectObject(hdc, hbrButtonHilite);
    t_1b78 = yTop;
    yTop = (yTop + 0x1);
    PatBlt(hdc, rc.left, t_1b78, (rc.right - rc.left), 0x1, PATCOPY);
    SelectObject(hdc, hbrSav);
    if ((ptile->fMinDraw != 0x0))
        goto L_1bfc;
    else
        goto L_1bb9;

L_1bb9:
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsScannerType, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    SelectObject(hdc, rghfontArial8[0x0]);

L_1bfc:
    SetRect(&(rgrcRef[0xb]), xLeft, yTop, xRight, ((dyArial8 * 0x2) + yTop));
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_1c64;
    else
        goto L_1c3b;

L_1c3b:
    CchGetString(idsOrganic, szWork);
    dRange = GetPlanetScannerRange(&(sel.pl), &(dRangeP));
    goto L_1cdf;

L_1c64:
    if ((sel.pl.iScanner != 0x1f))
        goto L_1ca1;
    else
        goto L_1c81;

L_1c81:
    if ((0x0 != 0x0))
        goto L_1ca1;
    else
        goto L_1c89;

L_1c89:
    CchGetString(idsNone4, szWork);
    dRange = 0x0;
    goto L_1cdf;

L_1ca1:
    LookupBestPlanetaryScanner(&(part));
    /* untranslated: call fstrcpy(szWork, &part[8:0](part)) -> callresult(char *) */
    dRange = GetPlanetScannerRange(&(sel.pl), &(dRangeP));

L_1cdf:
    RightTextOut(hdc, xRight, yTop, szWork, 0x0, dxRight);
    yTop = (yTop + dyArial8);
    if ((ptile->fMinDraw != 0x0))
        goto L_1d5a;
    else
        goto L_1d17;

L_1d17:
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsScannerRange, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    SelectObject(hdc, rghfontArial8[0x0]);

L_1d5a:
    if ((dRange <= 0x0))
        goto L_1df0;
    else
        goto L_1d63;

L_1d63:
    if ((dRangeP <= 0x0))
        goto L_1d97;
    else
        goto L_1d6c;

L_1d6c:
    c = _wsprintf(szWork, PszGetCompressedString(idsDDLY), dRangeP, dRange);
    goto L_1e05;

L_1d97:
    if ((dRange >= 0x64))
        goto L_1dc8;
    else
        goto L_1da0;

L_1da0:
    c = _wsprintf(szWork, PszGetCompressedString(idsDLightYears), dRange);
    goto L_1e05;

L_1dc8:
    c = _wsprintf(szWork, PszGetCompressedString(idsDLY), dRange);

L_1df0:
    CchGetString(idsNone4, szWork);
    c = 0x0;

L_1e05:
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop = (yTop + dyArial8);
    hbrSav = SelectObject(hdc, hbrButtonHilite);
    t_1e3b = yTop;
    yTop = (yTop + 0x1);
    PatBlt(hdc, rc.left, t_1e3b, (rc.right - rc.left), 0x1, PATCOPY);
    SelectObject(hdc, hbrSav);
    if ((ptile->fMinDraw != 0x0))
        goto L_1eb3;
    else
        goto L_1e7c;

L_1e7c:
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsDefenses4, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);

L_1eb3:
    SelectObject(hdc, rghfontArial8[0x0]);
    SetRect(&(rgrcRef[0xa]), xLeft, yTop, xRight, (LOWORD((0x3 * dyArial8)) + yTop));
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_1f16;
    else
        goto L_1f00;

L_1f00:
    c = CchGetString(idsN, szWork);
    goto L_1f5e;

L_1f16:
    c = _wsprintf(szWork, PszGetCompressedString(idsDD), sel.pl.cDefenses, CMaxOperableDefenses(&(sel.pl), idPlayer, 0x0));

L_1f5e:
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop = (yTop + dyArial8);
    if ((ptile->fMinDraw != 0x0))
        goto L_1fd8;
    else
        goto L_1f95;

L_1f95:
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsDefenseType, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    SelectObject(hdc, rghfontArial8[0x0]);

L_1fd8:
    if ((sel.pl.cDefenses != 0x0))
        goto L_2034;
    else
        goto L_1fed;

L_1fed:
    if ((0x0 != 0x0))
        goto L_2034;
    else
        goto L_1ff5;

L_1ff5:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_2020;
    else
        goto L_201a;

L_201a:
    t_merge_2023_0001 = 0x4ea;
    goto L_2023;

L_2020:
    t_merge_2023_0001 = 0x560;

L_2023:
    CchGetString(t_merge_2023_0001, szWork);
    dRange = 0x0;
    goto L_2061;

L_2034:
    FGetBestDefensePart(&(part));
    /* untranslated: call fstrcpy(szWork, &part[8:0](part)) -> callresult(char *) */
    dRange = 0x1;

L_2061:
    RightTextOut(hdc, xRight, yTop, szWork, 0x0, dxRight);
    yTop = (yTop + dyArial8);
    if ((ptile->fMinDraw != 0x0))
        goto L_20dc;
    else
        goto L_2099;

L_2099:
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsDefCoverage, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    SelectObject(hdc, rghfontArial8[0x0]);

L_20dc:
    if ((dRange == 0x0))
        goto L_2195;
    else
        goto L_20e5;

L_20e5:
    CalcPctSurvive(&(sel.pl), &(pct), 0x0);
    pct = (1 - pct);
    /* untranslated: ss:[bp-0x34] = 0x64 */
    /* untranslated: ss:[bp-0x32] = 0x0 */
    __ftol();
    /* untranslated: ss:[bp-0x3c] = loword(callresult(int32_t)) */
    /* untranslated: ss:[bp-0x3a] = signhiword(loword(callresult(int32_t))) */
    /* untranslated: ss:[bp-0x44] = 0x2710 */
    /* untranslated: ss:[bp-0x42] = 0x0 */
    /* untranslated: ss:[bp-0x4c] = 0x64 */
    /* untranslated: ss:[bp-0x4a] = 0x0 */
    c = _wsprintf(szWork, PCTDXPCTDPCTPCT, LOWORD(__ftol()), LOWORD(__ftol()));
    goto L_21d4;

L_2195:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_21c0;
    else
        goto L_21ba;

L_21ba:
    t_merge_21c3_0001 = 0x4ea;
    goto L_21c3;

L_21c0:
    t_merge_21c3_0001 = 0x560;

L_21c3:
    c = CchGetString(t_merge_21c3_0001, szWork);
    psz = 0x57a4;

L_21d4:
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);

L_21ef:
    return;
}

int16_t FGetBestDefensePart(PART *ppart) {
    int16_t fRet;
    int16_t i;
    PART    part;

L_21f6:
    fRet = 0x1;
    part.hs.grhst = hstPlanetary;
    part.hs.iItem = 0x9;
    i = 0x0;
    goto L_2250;

L_221d:
    i = (i + 0x1);
    /* untranslated: ss:[bp-0x10] = part.hs.iItem */
    /* untranslated: ss:[bp-0x12] = ((HIWORD(part.hs) + 0x1) & 0xff) */
    part.hs.iItem = 0x0;
    /* untranslated: HIWORD(part.hs) = (HIWORD(part.hs) | ss:[bp-0x12]) */

L_2250:
    if ((i >= 0x5))
        goto L_2273;
    else
        goto L_2259;

L_2259:
    if ((FLookupPart(&(part)) != 0x1))
        goto L_2273;
    else
        goto L_226a;

L_226a:

L_2273:
    if ((i <= 0x0))
        goto L_2283;
    else
        goto L_227c;

L_227c:
    i = (i - 0x1);
    goto L_2288;

L_2283:
    fRet = 0x0;

L_2288:
    part.hs.iItem = (i + 0x9);
    FLookupPart(&(part));
    *(ppart) = part;

L_22c6:
    return fRet;
}

void DrawPlanetStarbase(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t  fTwo;
    int16_t  dxRight;
    int16_t  iWarp;
    int16_t  bt;
    int16_t  yTop;
    int16_t  xRight;
    int16_t  c;
    SHDEF   *lpshdef;
    COLORREF crForeSav;
    uint16_t w;
    char    *psz;
    int16_t  xLeft;
    HBRUSH   hbrSav;
    int32_t  l;
    RECT     rc;
    int16_t  t_280d;
    uint16_t t_merge_2ab6_0001;

L_22cc:
    if ((ptile->fFixCtls == 0x0))
        goto L_232d;
    else
        goto L_22eb;

L_22eb:
    rgrcRef[0xd].top = 0xfffb;
    rgrcRef[0xd].bottom = 0xfffa;
    rgrcRef[0xe].top = 0xfffb;
    rgrcRef[0xe].bottom = 0xfffa;
    rgrcRef[0x10].top = 0xfffb;
    rgrcRef[0x10].bottom = 0xfffa;
    rgrcRef[0xf].top = 0xfffb;
    rgrcRef[0xf].bottom = 0xfffa;
    ptile->fFixCtls = 0x0;

L_232d:
    if ((sel.pl.fStarbase == 0x0))
        goto L_2387;
    else
        goto L_2340;

L_2340:
    lpshdef = &(rglpshdefSB[idPlayer][sel.pl.isb]);
    fstrcpy(szWork, &(lpshdef->hul.szClass));
    psz = 0x57a4;
    goto L_2396;

L_2387:
    psz = PszGetCompressedString(idsStarbase2);

L_2396:
    if ((FDrawTileNC(hdc, ptile, &(rc), psz) == 0x0))
        goto L_2af4;
    else
        goto L_23b0;

L_23b0:

L_23b6:
    xLeft = (rc.left + 0x4);
    xRight = (rc.right + 0xfffc);
    yTop = rc.top;
    SetRect(&(rc), (xLeft + 0xfffe), yTop, (xRight + 0x2), (rc.bottom + 0xfffe));
    FillRect(hdc, &(rc), hbrButtonFace);
    if ((sel.pl.fStarbase != 0x0))
        goto L_2455;
    else
        goto L_2418;

L_2418:
    SetRect(&(rgrcRef[0xe]), 0xfffb, 0xfffb, 0xfffa, 0xfffa);
    rgrcRef[0xf] = rgrcRef[0xe];
    rgrcRef[0x10] = rgrcRef[0xe];
    goto L_2af4;

L_2455:
    SetRect(&(rgrcRef[0xe]), xLeft, yTop, xRight, ((dyArial8 * 0x4) + yTop));
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsDockCapacity, szWork);
    l = GetTextExtent(hdc, szWork, c);
    dxRight = ((xRight - xLeft) - LOWORD(l));
    TextOut(hdc, xLeft, yTop, szWork, c);
    SelectObject(hdc, rghfontArial8[0x0]);
    w = LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax;
    if ((w != 0x0))
        goto L_2519;
    else
        goto L_2503;

L_2503:
    c = CchGetString(idsNone4, szWork);
    goto L_2561;

L_2519:
    if ((w != 0xffff))
        goto L_2545;
    else
        goto L_2527;

L_2527:
    if ((0x0 != 0x0))
        goto L_2545;
    else
        goto L_252f;

L_252f:
    c = CchGetString(idsUnlimited, szWork);
    goto L_2561;

L_2545:
    c = _wsprintf(szWork, PCTDKT, w);

L_2561:
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop = (yTop + dyArial8);
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsArmor2, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    w = lpshdef->hul.dp;
    if ((w != 0x0))
        goto L_25e2;
    else
        goto L_25cc;

L_25cc:
    c = CchGetString(idsNone4, szWork);
    goto L_260c;

L_25e2:
    c = _wsprintf(szWork, PszGetCompressedString(idsLddp), w, 0x0);

L_260c:
    SelectObject(hdc, rghfontArial8[0x0]);
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop = (yTop + dyArial8);
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsShields2, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    l = DpShieldOfShdef(lpshdef, idPlayer);
    if ((LOWORD(l) != 0x0))
        goto L_26b0;
    else
        goto L_2691;

L_2691:
    if ((HIWORD(l) != 0x0))
        goto L_26b0;
    else
        goto L_269a;

L_269a:
    c = CchGetString(idsNone4, szWork);
    goto L_26d8;

L_26b0:
    c = _wsprintf(szWork, PszGetCompressedString(idsLddp), LOWORD(l), HIWORD(l));

L_26d8:
    SelectObject(hdc, rghfontArial8[0x0]);
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop = (yTop + dyArial8);
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsDamage2, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    SelectObject(hdc, rghfontArial8[0x0]);
    w = sel.pl.pctDp;
    if ((w == 0x0))
        goto L_27ad;
    else
        goto L_2762;

L_2762:
    if ((w >= 0x5))
        goto L_2770;
    else
        goto L_276b;

L_276b:
    w = 0x5;

L_2770:
    /* untranslated: c = _wsprintf(szWork, PCTDPCTPCT, (words(w, 0x0) / 0x5)) */
    crForeSav = SetTextColor(hdc, 0x7f);
    goto L_27c0;

L_27ad:
    c = CchGetString(idsNone4, szWork);

L_27c0:
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop = (yTop + dyArial8);
    if ((w == 0x0))
        goto L_27f8;
    else
        goto L_27ea;

L_27ea:
    SetTextColor(hdc, crForeSav);

L_27f8:
    hbrSav = SelectObject(hdc, hbrButtonHilite);
    t_280d = yTop;
    yTop = (yTop + 0x1);
    PatBlt(hdc, rc.left, t_280d, (rc.right - rc.left), 0x1, PATCOPY);
    SelectObject(hdc, hbrSav);
    SelectObject(hdc, rghfontArial8[0x1]);
    SetRect(&(rgrcRef[0x10]), xLeft, yTop, xRight, (yTop + dyArial8));
    c = CchGetString(idsMassDriver, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    iWarp = IWarpMAFromLppl(&(sel.pl), &(fTwo));
    if ((iWarp <= 0x0))
        goto L_28e8;
    else
        goto L_28ab;

L_28ab:
    c = _wsprintf(szWork, PszGetCompressedString(idsWarpD), iWarp);
    if ((fTwo == 0x0))
        goto L_290a;
    else
        goto L_28d9;

L_28d9:
    c = (c + 0x1);
    szWork[c] = 0x2b;

L_28e8:
    c = _wsprintf(szWork, PszGetCompressedString(idsNone4));

L_290a:
    SelectObject(hdc, rghfontArial8[0x0]);
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop = (yTop + dyArial8);
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsDestination3, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    if ((sel.pl.idFling != 0x0))
        goto L_2992;
    else
        goto L_297c;

L_297c:
    c = CchGetString(idsNone4, szWork);
    goto L_29c2;

L_2992:
    psz = PszGetCompressedPlanet(rgidPlan[(sel.pl.idFling - 0x1)]);
    c = 0x0;
    strcpy(szWork, psz);

L_29c2:
    SelectObject(hdc, rghfontArial8[0x0]);
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop = (yTop + dyArial8);
    c = ((uint32_t)((xRight - xLeft)) / 0x3);
    SetRect(&(rgrcRef[0xd]), xLeft, yTop, (xLeft + c), ((yTop + dyArial8) + 0x6));
    bt = 0x8;
    if ((iWarp != 0x0))
        goto L_2a34;
    else
        goto L_2a30;

L_2a30:
    bt = (bt | 0x4);

L_2a34:
    DrawBtn(hdc, &(rgrcRef[0xd]), bt, gd.fSetMassMode, PszGetCompressedString(idsSetDest));
    if ((iWarp <= 0x0))
        goto L_2ac9;
    else
        goto L_2a68;

L_2a68:
    SetRect(&(rgrcRef[0xf]), ((xLeft + c) + 0x4), (yTop + 0x3), xRight, ((yTop + dyArial8) + 0x3));
    if ((fTwo == 0x0))
        goto L_2ab3;
    else
        goto L_2aab;

L_2aab:
    /* untranslated: t_merge_2ab6_0001 = (iWarp neg 0x0) */
    goto L_2ab6;

L_2ab3:
    t_merge_2ab6_0001 = iWarp;

L_2ab6:
    DrawMassWarpGauge(hdc, &(rgrcRef[0xf]), t_merge_2ab6_0001, (sel.pl.iWarpFling + 0x4));
    goto L_2af4;

L_2ac9:
    SetRect(&(rgrcRef[0xf]), 0xfffb, 0xfffb, 0xfffa, 0xfffa);
    rgrcRef[0x10] = rgrcRef[0xf];

L_2af4:
    return;
}

void DrawMassWarpGauge(HDC hdc, RECT *prc, int16_t iBest, int16_t iCur) {
    int32_t  lMax;
    int16_t  c;
    int16_t  fTwoMAs;
    int16_t  iMode;
    HBRUSH   hbr;
    int32_t  lCur;
    int32_t  l;
    uint16_t t_merge_2b15_0001;

L_2afa:
    if ((iBest >= 0x0))
        goto L_2b12;
    else
        goto L_2b0c;

L_2b0c:
    t_merge_2b15_0001 = 0x1;
    goto L_2b15;

L_2b12:
    t_merge_2b15_0001 = 0x0;

L_2b15:
    fTwoMAs = t_merge_2b15_0001;
    SelectObject(hdc, rghfontArial8[0x1]);
    if ((iCur >= 0x5))
        goto L_2b32;
    else
        goto L_2b2d;

L_2b2d:
    iCur = 0x5;

L_2b32:
    if ((iBest >= 0x0))
        goto L_2b43;
    else
        goto L_2b3b;

L_2b3b:
    /* untranslated: iBest = (iBest neg 0x0) */

L_2b43:
    lMax = (uint32_t)((iBest + 0xffff));
    if ((iCur > (iBest + fTwoMAs)))
        goto L_2b67;
    else
        goto L_2b5e;

L_2b5e:
    hbr = hbrPurple;
    goto L_2b87;

L_2b67:
    if ((iCur >= ((iBest + fTwoMAs) + 0x3)))
        goto L_2b81;
    else
        goto L_2b78;

L_2b78:
    hbr = hbrYellow;
    goto L_2b87;

L_2b81:
    hbr = hbrRed;

L_2b87:
    lCur = (uint32_t)((iCur + 0xfffc));
    l = LDrawGauge(hdc, prc, 0x1, &(lCur), &(hbr), lMax);
    iMode = SetBkMode(hdc, TRANSPARENT);
    c = _wsprintf(szWork, PszGetCompressedString(idsWarpLd), (LOWORD(l) + 0x4), (HIWORD(l) + 0x0));
    l = GetTextExtent(hdc, szWork, c);
    RcCtrTextOut(hdc, prc, szWork, c);
    SetBkMode(hdc, iMode);
    return;
}

void DrawPlanetProduction(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t  swp;
    int16_t  dxRight;
    int16_t  yTop;
    int16_t  xStart;
    int16_t  xRight;
    char     szT[40];
    int16_t  i;
    int16_t  c;
    int16_t  dyWrong;
    char    *psz;
    int16_t  iSel;
    int16_t  cch;
    int16_t  xLeft;
    RECT     rcT;
    PLANET  *ppl;
    RECT     rc;
    uint16_t t_merge_2d4a_0001;
    uint16_t t_merge_30ab_0001;

L_2c38:
    ppl = obj;
    if ((ptile->fFixCtls == 0x0))
        goto L_2ca2;
    else
        goto L_2c5d;

L_2c5d:
    ShowWindow(hwndPlanetProdLB, SW_HIDE);
    ShowWindow(rghwndBtn[0xb], SW_HIDE);
    ShowWindow(rghwndBtn[0xc], SW_HIDE);
    rgrcRef[0x11].top = 0xfffb;
    rgrcRef[0x11].bottom = 0xfffa;
    ptile->fFixCtls = 0x0;

L_2ca2:
    if ((FDrawTileNC(hdc, ptile, &(rc), PszGetCompressedString(idsProduction)) != 0x0))
        goto L_2cf3;
    else
        goto L_2cc9;

L_2cc9:
    ShowWindow(hwndPlanetProdLB, SW_HIDE);
    ShowWindow(rghwndBtn[0xb], SW_HIDE);
    ShowWindow(rghwndBtn[0xc], SW_HIDE);
    goto L_3105;

L_2cf3:
    xLeft = (rc.left + 0x4);
    xRight = (rc.right + 0xfffc);
    yTop = rc.top;
    yTop = (yTop + 0x4);
    GetClientRect(hwndPlanetProdLB, &(rcT));
    swp = 0x14;
    if ((gd.fSmallTileMode == 0x0))
        goto L_2d47;
    else
        goto L_2d41;

L_2d41:
    t_merge_2d4a_0001 = 0x3;
    goto L_2d4a;

L_2d47:
    t_merge_2d4a_0001 = 0x5;

L_2d4a:
    /* untranslated: dyPlanetProdLB = loword((ss:[bp-0x56] * t_merge_2d4a_0001)) */
    dyWrong = (dyPlanetProdLB - (rcT.bottom - rcT.top));
    if ((dxPlanetProdLB != (xRight - xLeft)))
        goto L_2d8d;
    else
        goto L_2d72;

L_2d72:
    if ((dyWrong < 0x0))
        goto L_2d8d;
    else
        goto L_2d7b;

L_2d7b:
    if ((dyWrong >= dyArial8))
        goto L_2d8d;
    else
        goto L_2d86;

L_2d86:
    swp = (swp | 0x1);
    goto L_2d96;

L_2d8d:
    dxPlanetProdLB = (xRight - xLeft);

L_2d96:
    SetWindowPos(hwndPlanetProdLB, 0x0, xLeft, yTop, (xRight - xLeft), dyPlanetProdLB, swp);
    ShowWindow(hwndPlanetProdLB, SW_SHOW);
    GetClientRect(hwndPlanetProdLB, &(rcT));
    dyPlanetProdLB = (rcT.bottom - rcT.top);
    yTop = (yTop + (dyPlanetProdLB + 0x4));
    iSel = LOWORD(SendMessage(hwndPlanetProdLB, CB_GETLBTEXTLEN, 0x0, 0x0));
    if ((iSel >= 0x0))
        goto L_2e11;
    else
        goto L_2e0c;

L_2e0c:
    iSel = 0x0;

L_2e11:
    if ((LOWORD(ppl->lpplprod) != 0x0))
        goto L_2e26;
    else
        goto L_2e1d;

L_2e1d:
    if ((HIWORD(ppl->lpplprod) == 0x0))
        goto L_2e3b;
    else
        goto L_2e26;

L_2e26:
    if ((ppl->lpplprod->iprodMac != 0x0))
        goto L_2e48;
    else
        goto L_2e3b;

L_2e3b:
    c = 0x0;
    szWork[0x0] = 0x0;
    goto L_2e7b;

L_2e48:
    psz = PszProductionETA(ppl, 0x0, iSel, 0x0, 0x0);
    c = strlen(psz);

L_2e7b:
    if ((c == 0x0))
        goto L_2f02;
    else
        goto L_2e84;

L_2e84:
    SelectObject(hdc, rghfontArial8[0x1]);
    cch = CchGetString(idsCompletion, szT);
    TextOut(hdc, xLeft, yTop, szT, cch);
    dxRight = ((xRight - xLeft) - LOWORD(GetTextExtent(hdc, szT, cch)));
    SelectObject(hdc, rghfontArial8[0x0]);
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    goto L_2f22;

L_2f02:
    RightTextOut(hdc, xRight, yTop, szWork, 0x0, (xRight - xLeft));

L_2f22:
    yTop = (yTop + dyArial8);
    SelectObject(hdc, rghfontArial8[0x1]);
    cch = CchGetString(idsRoute3, szT);
    TextOut(hdc, xLeft, yTop, szT, cch);
    dxRight = ((xRight - xLeft) - LOWORD(GetTextExtent(hdc, szT, cch)));
    if ((sel.pl.idRoute != 0x0))
        goto L_2fa0;
    else
        goto L_2f8a;

L_2f8a:
    c = CchGetString(idsNone4, szWork);
    goto L_2fd0;

L_2fa0:
    psz = PszGetCompressedPlanet(rgidPlan[(sel.pl.idRoute - 0x1)]);
    c = 0x0;
    strcpy(szWork, psz);

L_2fd0:
    SelectObject(hdc, rghfontArial8[0x0]);
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop = (yTop + dyArial8);
    c = ((uint32_t)(((xRight - xLeft) + 0xfff0)) / 0x3);
    xStart = xLeft;
    i = 0xb;
    goto L_3030;

L_301d:
    i = (i + 0x1);
    xStart = (xStart + (c + 0x8));

L_3030:
    if ((i > 0xc))
        goto L_3077;
    else
        goto L_3039;

L_3039:
    SetWindowPos(rghwndBtn[i], 0x0, xStart, yTop, c, ((dyArial8 >> 0x1) + dyArial8), 0x14);
    ShowWindow(rghwndBtn[i], SW_SHOW);
    goto L_301d;

L_3077:
    if ((LOWORD(sel.pl.lpplprod) != 0x0))
        goto L_308f;
    else
        goto L_3085;

L_3085:
    if ((HIWORD(sel.pl.lpplprod) == 0x0))
        goto L_30a8;
    else
        goto L_308f;

L_308f:
    if ((sel.pl.lpplprod->iprodMac == 0x0))
        goto L_30a8;
    else
        goto L_30a2;

L_30a2:
    t_merge_30ab_0001 = 0x1;
    goto L_30ab;

L_30a8:
    t_merge_30ab_0001 = 0x0;

L_30ab:
    EnableWindow(rghwndBtn[0xc], t_merge_30ab_0001);
    SetRect(&(rgrcRef[0x11]), xStart, yTop, (xStart + c), ((yTop + dyArial8) + (dyArial8 >> 0x1)));
    DrawBtn(hdc, &(rgrcRef[0x11]), 0x8, gd.fSetRouteMode, PszGetCompressedString(idsRoute2));

L_3105:
    return;
}

char *PszProductionETA(PLANET *lppl, PLPROD *lpplprod, ProdItemType iItem, int16_t *etaFirst, int16_t *etaLast) {
    int16_t  iTurnEnd;
    int16_t  iTurnBegin;
    int16_t  c;
    StringId ids;

L_310c:
    if ((LOWORD(lpplprod) != 0x0))
        goto L_3138;
    else
        goto L_311e;

L_311e:
    if ((HIWORD(lpplprod) != 0x0))
        goto L_3138;
    else
        goto L_3127;

L_3127:
    lpplprod = lppl->lpplprod;

L_3138:
    EstimateItemProdSched(lppl, lpplprod, iItem, &(iTurnBegin), &(iTurnEnd));
    if ((iTurnBegin != 0x64))
        goto L_3221;
    else
        goto L_3160;

L_3160:
    if ((LOWORD(lpplprod) != 0x0))
        goto L_3172;
    else
        goto L_3169;

L_3169:
    if ((HIWORD(lpplprod) == 0x0))
        goto L_3207;
    else
        goto L_3172;

L_3172:
    if ((lpplprod->iprodMac <= iItem))
        goto L_3207;
    else
        goto L_3184;

L_3184:
    if (((LOWORD((uint32_t)((lpplprod[iItem].rgprod[0x0] >> 0x11))) & 0x7) != 0x1))
        goto L_3207;
    else
        goto L_31b7;

L_31b7:
    if ((0x0 != 0x0))
        goto L_3207;
    else
        goto L_31bf;

L_31bf:
    if ((0x0 > 0x0))
        goto L_3207;
    else
        goto L_31f2;

L_31f2:
    if ((0x0 < 0x0))
        goto L_31ff;
    else
        goto L_31f7;

L_31f7:
    if (((LOWORD((uint32_t)((lpplprod[iItem].rgprod[0x0] >> 0xa))) & 0x7f) >= 0x7))
        goto L_3207;
    else
        goto L_31ff;

L_31ff:
    ids = idsUnknown2;
    goto L_320c;

L_3207:
    ids = idsNever;

L_320c:
    c = CchGetString(ids, szWork);
    goto L_3308;

L_3221:
    if ((iTurnEnd != 0x64))
        goto L_3252;
    else
        goto L_322a;

L_322a:
    c = _wsprintf(szWork, PszGetCompressedString(idsDYears), iTurnBegin);
    goto L_3308;

L_3252:
    if ((iTurnBegin != iTurnEnd))
        goto L_32e0;
    else
        goto L_325d;

L_325d:
    if ((iTurnBegin != 0x0))
        goto L_327c;
    else
        goto L_3266;

L_3266:
    c = CchGetString(idsSkipped, szWork);
    goto L_3308;

L_327c:
    if ((iTurnBegin != 0xffff))
        goto L_329b;
    else
        goto L_3285;

L_3285:
    c = CchGetString(idsNeeded, szWork);
    goto L_3308;

L_329b:
    c = _wsprintf(szWork, PszGetCompressedString(idsDYear), iTurnBegin);
    if ((iTurnBegin == 0x1))
        goto L_3308;
    else
        goto L_32c9;

L_32c9:
    szWork[c] = 0x73;
    c = (c + 0x1);
    szWork[c] = 0x0;

L_32e0:
    c = _wsprintf(szWork, PszGetCompressedString(idsDDYears), iTurnBegin, iTurnEnd);

L_3308:
    if ((etaFirst == 0x0))
        goto L_3319;
    else
        goto L_3311;

L_3311:
    *(etaFirst) = iTurnBegin;

L_3319:
    if ((etaLast == 0x0))
        goto L_332a;
    else
        goto L_3322;

L_3322:
    *(etaLast) = iTurnEnd;

L_332a:

L_3330:
    return 0x57a4;
}

void DrawPlanShipBitmap(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t  yTop;
    int16_t  dy;
    int16_t  xRight;
    int16_t  i;
    char    *psz;
    int16_t  dx;
    int16_t  xLeft;
    HBRUSH   hbrSav;
    int16_t  iOffset;
    RECT     rc;
    uint16_t t_merge_3449_0001;
    uint16_t t_merge_36fd_0001;
    uint16_t t_merge_372c_0001;

L_3336:
    if ((sel.grobj != grobjPlanet))
        goto L_3374;
    else
        goto L_3349;

L_3349:
    /* untranslated: psz = PszGetPlanetName(ds:[obj]) */
    /* untranslated: i = ds:[obj] */
    i = (i + 0x8);
    iOffset = ((uint32_t)(i) % 0x1c);
    goto L_3384;

L_3374:
    /* untranslated: psz = PszGetFleetName(ds:[obj]) */

L_3384:
    if ((ptile->fFixCtls == 0x0))
        goto L_33d3;
    else
        goto L_339a;

L_339a:
    i = 0x4;
    goto L_33b8;

L_33a2:
    ShowWindow(rghwndBtn[i], SW_HIDE);
    i = (i + 0x1);

L_33b8:
    if ((i <= 0x6))
        goto L_33a2;
    else
        goto L_33c1;

L_33c1:
    ptile->fFixCtls = 0x0;

L_33d3:
    if ((FDrawTileNC(hdc, ptile, &(rc), psz) != 0x0))
        goto L_341a;
    else
        goto L_33f0;

L_33f0:
    i = 0x4;
    goto L_340e;

L_33f8:
    ShowWindow(rghwndBtn[i], SW_HIDE);
    i = (i + 0x1);

L_340e:
    if ((i > 0x6))
        goto L_3777;
    else
        goto L_3414;

L_3414:

L_341a:
    xLeft = (rc.left + 0xc);
    xRight = (rc.right + 0xfff4);
    if ((gd.fSmallTileMode == 0x0))
        goto L_3446;
    else
        goto L_3440;

L_3440:
    t_merge_3449_0001 = 0x2;
    goto L_3449;

L_3446:
    t_merge_3449_0001 = 0x6;

L_3449:
    yTop = (t_merge_3449_0001 + rc.top);
    if ((sel.grobj != grobjFleet))
        goto L_348c;
    else
        goto L_3459;

L_3459:
    DrawFleetBitmap(&(sel.fl), hdc, xLeft, yTop, 0x1, 0xffff, 0x0, 0x0, 0xffff, 0x0);
    goto DoBtns;

L_348c:
    hbrSav = SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, xLeft, yTop, 0x46, 0x2, PATCOPY);
    PatBlt(hdc, xLeft, (yTop + 0x2), 0x2, 0x44, PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, (xLeft + 0x2), (yTop + 0x44), 0x44, 0x2, PATCOPY);
    PatBlt(hdc, (xLeft + 0x44), (yTop + 0x2), 0x2, 0x42, PATCOPY);
    PatBlt(hdc, (xLeft + 0x1), (yTop + 0x45), 0x1, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x45), (yTop + 0x1), 0x1, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x2), (yTop + 0x2), 0x42, 0x1, BLACKNESS);
    PatBlt(hdc, (xLeft + 0x2), (yTop + 0x3), 0x1, 0x41, BLACKNESS);
    PatBlt(hdc, (xLeft + 0x3), (yTop + 0x43), 0x41, 0x1, BLACKNESS);
    PatBlt(hdc, (xLeft + 0x43), (yTop + 0x3), 0x1, 0x40, BLACKNESS);
    SelectObject(hdc, hbrSav);
    SelectPalette(hdc, vhpal, 0x0);
    RealizePalette(hdc);
    DibBlt(hdc, (xLeft + 0x3), (yTop + 0x3), 0x40, 0x40, hdibPlanets, (((uint32_t)(iOffset) % 0x7) * 0x40), (((uint32_t)(iOffset) / 0x7) * 0x40), 0x40, 0x40,
           0xcc0020);

DoBtns:
    dx = ((xRight - xLeft) + 0xffa1);
    dy = (LOWORD((0x3 * dyArial8)) >> 0x1);
    xLeft = (xRight - dx);
    if ((ptile->fMinDraw != 0x0))
        goto L_3777;
    else
        goto L_36c1;

L_36c1:

L_36c7:
    if ((gd.fSmallTileMode == 0x0))
        goto L_36e6;
    else
        goto L_36db;

L_36db:
    yTop = (yTop - 0x2);
    dy = (dy - 0x2);
    goto L_36ea;

L_36e6:
    yTop = (yTop - 0x4);

L_36ea:
    if ((sel.grobj != grobjFleet))
        goto L_36fa;
    else
        goto L_36f4;

L_36f4:
    t_merge_36fd_0001 = 0x6;
    goto L_36fd;

L_36fa:
    t_merge_36fd_0001 = 0x5;

L_36fd:
    iOffset = t_merge_36fd_0001;
    i = 0x4;
    goto L_3735;

L_3708:
    i = (i + 0x1);
    if ((gd.fSmallTileMode == 0x0))
        goto L_3729;
    else
        goto L_3723;

L_3723:
    t_merge_372c_0001 = 0x2;
    goto L_372c;

L_3729:
    t_merge_372c_0001 = 0x3;

L_372c:
    yTop = (yTop + (t_merge_372c_0001 + dy));

L_3735:
    if ((i > iOffset))
        goto L_3777;
    else
        goto L_3740;

L_3740:
    SetWindowPos(rghwndBtn[i], 0x0, xLeft, yTop, dx, dy, 0x14);
    ShowWindow(rghwndBtn[i], SW_SHOW);
    goto L_3708;

L_3777:
    return;
}

void DrawPlanetShipList(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t  swp;
    int16_t  fDoneDrawing;
    int32_t  l2;
    int16_t  yTop;
    int16_t  fObjIsThing;
    int16_t  fUnknown;
    int16_t  idSkip;
    int16_t  xStart;
    int16_t  xRight;
    int16_t  i;
    int16_t  c;
    RECT     rcGauge;
    XFER     xf;
    FLEET   *pfl;
    int32_t  lSel;
    int16_t  xLeft;
    int32_t  l;
    RECT     rc;
    uint16_t t_merge_381f_0001;
    uint16_t t_merge_3940_0001;
    int16_t  t_merge_39af_0001;
    uint16_t t_merge_39f1_0001;
    uint16_t t_merge_3a16_0001;
    uint16_t t_merge_3a55_0001;
    uint16_t t_merge_3de0_0001;

L_377e:
    pfl = obj;
    fDoneDrawing = 0x0;
    fObjIsThing = 0x0;
    if ((ptile->fFixCtls == 0x0))
        goto L_380c;
    else
        goto L_37ae;

L_37ae:
    i = 0x0;
    goto L_37cc;

L_37b6:
    ShowWindow(rghwndBtn[i], SW_HIDE);
    i = (i + 0x1);

L_37cc:
    if ((i <= 0x2))
        goto L_37b6;
    else
        goto L_37d5;

L_37d5:
    ShowWindow(hwndShipDD, SW_HIDE);
    ptile->fFixCtls = 0x0;
    rgrcRef[0x1].top = 0xfffb;
    rgrcRef[0x1].bottom = 0xfffa;
    rgrcRef[0x4].top = 0xfffb;
    rgrcRef[0x4].bottom = 0xfffa;

L_380c:
    if ((pfl != 0x0))
        goto L_381c;
    else
        goto L_3816;

L_3816:
    t_merge_381f_0001 = 0x338;
    goto L_381f;

L_381c:
    t_merge_381f_0001 = 0x339;

L_381f:
    if ((FDrawTileNC(hdc, ptile, &(rc), PszGetCompressedString(t_merge_381f_0001)) != 0x0))
        goto L_387b;
    else
        goto L_3844;

L_3844:
    i = 0x0;
    goto L_3862;

L_384c:
    ShowWindow(rghwndBtn[i], SW_HIDE);
    i = (i + 0x1);

L_3862:
    if ((i <= 0x2))
        goto L_384c;
    else
        goto L_386b;

L_386b:
    ShowWindow(hwndShipDD, SW_HIDE);
    goto L_3de6;

L_387b:
    xLeft = (rc.left + 0x4);
    xRight = (rc.right + 0xfffc);
    yTop = (rc.top + 0x2);
    swp = 0x14;
    if ((dxShipDD != (xRight - xLeft)))
        goto L_38b6;
    else
        goto L_38af;

L_38af:
    swp = (swp | 0x1);
    goto L_38c0;

L_38b6:
    dxShipDD = (xRight - xLeft);

L_38c0:
    SetWindowPos(hwndShipDD, 0x0, xLeft, yTop, (xRight - xLeft), LOWORD((0x5 * dyShipDD)), swp);
    ShowWindow(hwndShipDD, SW_SHOW);
    yTop = (yTop + (dyShipDD + 0x3));
    lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0x0);
    if ((LOWORD(lSel) != 0xffff))
        goto L_3937;
    else
        goto L_392d;

L_392d:
    if ((HIWORD(lSel) == 0xffff))
        goto L_393d;
    else
        goto L_3937;

L_3937:
    t_merge_3940_0001 = 0x1;
    goto L_3940;

L_393d:
    t_merge_3940_0001 = 0x0;

L_3940:
    EnableWindow(rghwndBtn[0x0], t_merge_3940_0001);
    if ((LOWORD(lSel) != 0xffff))
        goto L_3964;
    else
        goto L_3950;

L_3950:
    if ((HIWORD(lSel) != 0xffff))
        goto L_3964;
    else
        goto L_395a;

L_395a:
    fDoneDrawing = 0x1;
    fUnknown = 0x1;

L_3964:
    if ((pfl == 0x0))
        goto L_397a;
    else
        goto L_396e;

L_396e:
    idSkip = pfl->id;
    goto L_397f;

L_397a:
    idSkip = 0xffff;

L_397f:
    if ((fDoneDrawing != 0x0))
        goto L_39c5;
    else
        goto L_3988;

L_3988:
    if ((idSkip == 0xffff))
        goto L_39ac;
    else
        goto L_39a2;

L_39a2:
    t_merge_39af_0001 = pfl->idPlanet;
    goto L_39af;

L_39ac:
    t_merge_39af_0001 = sel.pl.id;

L_39af:
    if ((FLookupOrbitingXfer(t_merge_39af_0001, LOWORD(lSel), &(xf), idSkip) != 0x0))
        goto L_39c5;
    else
        goto L_39c0;

L_39c0:
    fDoneDrawing = 0x1;

L_39c5:
    if ((fDoneDrawing == 0x0))
        goto L_39de;
    else
        goto L_39ce;

L_39ce:
    EnableWindow(rghwndBtn[0x1], 0x0);
    goto L_3a1c;

L_39de:
    if ((xf.grobj != grobjThing))
        goto L_39ee;
    else
        goto L_39e8;

L_39e8:
    t_merge_39f1_0001 = 0x1;
    goto L_39f1;

L_39ee:
    t_merge_39f1_0001 = 0x0;

L_39f1:
    fObjIsThing = t_merge_39f1_0001;
    if ((fObjIsThing != 0x0))
        goto L_3a13;
    else
        goto L_3a01;

L_3a01:
    /* untranslated: branch part[6:2](xf) != idPlayer ? L_3a13 : L_3a0d */

L_3a0d:
    t_merge_3a16_0001 = 0x1;
    goto L_3a16;

L_3a13:
    t_merge_3a16_0001 = 0x0;

L_3a16:
    EnableWindow(rghwndBtn[0x1], t_merge_3a16_0001);

L_3a1c:
    rgrcRef[0x1].top = 0xfffb;
    rgrcRef[0x1].bottom = 0xfffa;
    rgrcRef[0x4].top = 0xfffb;
    rgrcRef[0x4].bottom = 0xfffa;
    if ((fObjIsThing != 0x0))
        goto L_3a52;
    else
        goto L_3a3d;

L_3a3d:
    /* untranslated: branch (part[8:2](xf) & 0xff) == 0x7 ? L_3a52 : L_3a4c */

L_3a4c:
    t_merge_3a55_0001 = 0x1;
    goto L_3a55;

L_3a52:
    t_merge_3a55_0001 = 0x0;

L_3a55:
    fUnknown = t_merge_3a55_0001;
    if ((gd.fSmallTileMode != 0x0))
        goto L_3d1e;
    else
        goto L_3a6c;

L_3a6c:
    if ((fDoneDrawing != 0x0))
        goto L_3ab7;
    else
        goto L_3a75;

L_3a75:
    if ((fUnknown != 0x0))
        goto L_3ab7;
    else
        goto L_3a7e;

L_3a7e:
    SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsCargo3, szWork);
    l = GetTextExtent(hdc, szWork, c);

L_3ab7:
    if ((fDoneDrawing != 0x0))
        goto L_3ba0;
    else
        goto L_3ac0;

L_3ac0:
    if ((fUnknown != 0x0))
        goto L_3ba0;
    else
        goto L_3ac9;

L_3ac9:
    if ((fObjIsThing != 0x0))
        goto L_3ba0;
    else
        goto L_3ad2;

L_3ad2:
    c = CchGetString(idsFuel3, szWork);
    l2 = GetTextExtent(hdc, szWork, c);
    if ((HIWORD(l2) < HIWORD(l)))
        goto L_3b28;
    else
        goto L_3b0d;

L_3b0d:
    if ((HIWORD(l2) > HIWORD(l)))
        goto L_3b1a;
    else
        goto L_3b12;

L_3b12:
    if ((LOWORD(l2) <= LOWORD(l)))
        goto L_3b28;
    else
        goto L_3b1a;

L_3b1a:
    l = l2;

L_3b28:
    TextOut(hdc, xLeft, yTop, szWork, c);
    SetRect(&(rcGauge), (xLeft + LOWORD(l)), yTop, xRight, (yTop + dyArial8));
    if ((idSkip == 0xffff))
        goto L_3b82;
    else
        goto L_3b73;

L_3b73:
    rgrcRef[0x1] = rcGauge;

L_3b82:
    DrawFleetGauge(hdc, &(rcGauge), &(xf.fl), 0x4);
    goto L_3bfc;

L_3ba0:
    if ((ptile->fMinDraw != 0x0))
        goto L_3bc7;
    else
        goto L_3bb6;

L_3bb6:
    if ((fUnknown == gd.fUnknownShip))
        goto L_3bfc;
    else
        goto L_3bc7;

L_3bc7:
    SetRect(&(rcGauge), xLeft, yTop, xRight, (((dyArial8 * 0x2) + yTop) + 0x8));
    FillRect(hdc, &(rcGauge), hbrButtonFace);

L_3bfc:
    gd.fUnknownShip = fUnknown;
    if ((fObjIsThing != 0x0))
        goto L_3c21;
    else
        goto L_3c18;

L_3c18:
    yTop = (yTop + (dyArial8 + 0x4));

L_3c21:
    if ((fDoneDrawing != 0x0))
        goto L_3d15;
    else
        goto L_3c2a;

L_3c2a:
    if ((fUnknown != 0x0))
        goto L_3d15;
    else
        goto L_3c33;

L_3c33:
    c = CchGetString(idsCargo3, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    SetRect(&(rcGauge), (xLeft + LOWORD(l)), yTop, xRight, (yTop + dyArial8));
    rgrcRef[0x4] = rcGauge;
    if ((fObjIsThing == 0x0))
        goto L_3cbe;
    else
        goto L_3ca0;

L_3ca0:
    DrawThingGauge(hdc, &(rcGauge), &(xf.th), 0x5);
    goto L_3cd9;

L_3cbe:
    DrawFleetGauge(hdc, &(rcGauge), &(xf.fl), 0x5);

L_3cd9:
    if ((fObjIsThing == 0x0))
        goto L_3d15;
    else
        goto L_3ce2;

L_3ce2:
    OffsetRect(&(rcGauge), 0x0, (dyArial8 + 0x4));
    FillRect(hdc, &(rcGauge), hbrButtonFace);
    yTop = (yTop + (dyArial8 + 0x4));

L_3d15:
    yTop = (yTop + (dyArial8 + 0x4));

L_3d1e:
    c = ((uint32_t)(((xRight - xLeft) + 0xfff6)) / 0x3);
    xStart = xLeft;
    i = 0x0;
    goto L_3d53;

L_3d40:
    i = (i + 0x1);
    xStart = (xStart + (c + 0x6));

L_3d53:
    if ((i > 0x2))
        goto L_3db8;
    else
        goto L_3d5c;

L_3d5c:
    SetWindowPos(rghwndBtn[((uint32_t)((i + 0x1)) % 0x3)], 0x0, xStart, yTop, c, ((dyArial8 >> 0x1) + dyArial8), 0x14);
    if ((i != 0x2))
        goto L_3da3;
    else
        goto L_3d99;

L_3d99:
    if ((pfl == 0x0))
        goto L_3d40;
    else
        goto L_3da3;

L_3da3:
    ShowWindow(rghwndBtn[i], SW_SHOW);

L_3db8:
    if ((idSkip == 0xffff))
        goto L_3dce;
    else
        goto L_3dc5;

L_3dc5:
    if ((fUnknown != 0x0))
        goto L_3ddd;
    else
        goto L_3dce;

L_3dce:
    if ((fObjIsThing != 0x0))
        goto L_3ddd;
    else
        goto L_3dd7;

L_3dd7:
    t_merge_3de0_0001 = 0x1;
    goto L_3de0;

L_3ddd:
    t_merge_3de0_0001 = 0x0;

L_3de0:
    EnableWindow(rghwndBtn[0x2], t_merge_3de0_0001);

L_3de6:
    return;
}

void SetPlanetTitleBar(HWND hwnd) {
    char  szTitle[30];
    char *psz;

L_3dec:
    if ((sel.grobj != grobjPlanet))
        goto L_3e3a;
    else
        goto L_3dff;

L_3dff:
    psz = PszGetPlanetName(sel.pl.id);
    CchGetString(idsPlanet2, szTitle);
    lstrcat(szTitle, psz);
    psz = szTitle;
    goto L_3e65;

L_3e3a:
    if ((sel.grobj != grobjFleet))
        goto L_3e56;
    else
        goto L_3e44;

L_3e44:
    psz = PszGetFleetName(sel.fl.id);
    goto L_3e65;

L_3e56:
    psz = PszGetCompressedString(idsPlanetView);

L_3e65:
    SetWindowText(hwnd, psz);
    return;
}

void ChangeMainObjSel(GrobjClass grobjNew, int16_t iObjSel) {
    int16_t  fSameType;
    int16_t  idSkip;
    int16_t  i;
    FLEET   *lpfl;
    uint16_t t_merge_3e9c_0001;
    uint16_t t_merge_3ed7_0001;
    uint16_t t_merge_4093_0001;

L_3e7a:
    idSkip = 0xffff;
    if ((grobjNew != sel.grobj))
        goto L_3e99;
    else
        goto L_3e93;

L_3e93:
    t_merge_3e9c_0001 = 0x1;
    goto L_3e9c;

L_3e99:
    t_merge_3e9c_0001 = 0x0;

L_3e9c:
    fSameType = t_merge_3e9c_0001;
    if ((fAi == 0x0))
        goto L_3ec0;
    else
        goto L_3ea9;

L_3ea9:
    if ((fSameType == 0x0))
        goto L_3ec0;
    else
        goto L_3eb2;

L_3eb2:
    if ((iObjSel == sel.id))
        goto L_4299;
    else
        goto L_3eba;

L_3eba:

L_3ec0:
    if ((sel.grobj != grobjPlanet))
        goto L_3ed4;
    else
        goto L_3ece;

L_3ece:
    t_merge_3ed7_0001 = 0x0;
    goto L_3ed7;

L_3ed4:
    t_merge_3ed7_0001 = 0x1;

L_3ed7:
    InvalidateReport(t_merge_3ed7_0001, 0x0);
    if ((grobjNew != grobjPlanet))
        goto L_4014;
    else
        goto L_3ee9;

L_3ee9:
    InvalidateReport(0x0, 0x0);
    if ((FLookupPlanet(iObjSel, &(sel.pl)) == 0x0))
        goto L_4299;
    else
        goto L_3f0d;

L_3f0d:

L_3f13:
    sel.pt.x = rgptPlan[iObjSel].x;
    sel.pt.y = rgptPlan[iObjSel].y;
    sel.scan.iwp = 0xffff;
    sel.iwpAct = 0xffff;
    i = 0x0;
    goto L_3f41;

L_3f3d:
    i = (i + 0x1);

L_3f41:
    if ((i >= cFleet))
        goto L_3fa0;
    else
        goto L_3f4c;

L_3f4c:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_3f7c;
    else
        goto L_3f74;

L_3f74:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_3fa0;
    else
        goto L_3f7c;

L_3f7c:
    if ((lpfl->idPlanet != iObjSel))
        goto L_3f3d;
    else
        goto L_3f8b;

L_3f8b:
    if ((lpfl->iPlayer == idPlayer))
        goto L_3fa0;
    else
        goto L_3f97;

L_3f97:

L_3fa0:
    if ((i == cFleet))
        goto L_3fc6;
    else
        goto L_3fab;

L_3fab:
    FDupFleet(lpfl, &(sel.fl));
    sel.grobjFull = 0x3;
    goto L_3fd2;

L_3fc6:
    sel.fl.id = 0xffff;
    sel.grobjFull = grobjPlanet;

L_3fd2:
    if ((fAi != 0x0))
        goto L_40ec;
    else
        goto L_3fdc;

L_3fdc:
    FillPlanetProdLB(0x0, 0x0, 0x0);
    SendMessage(hwndPlanetProdLB, CB_GETCURSEL, 0x0, 0x0);

L_4014:
    InvalidateReport(0x1, 0x0);
    if ((FLookupFleet(iObjSel, &(sel.fl)) == 0x0))
        goto L_4299;
    else
        goto L_4038;

L_4038:

L_403e:
    sel.pt.x = sel.fl.pt.x;
    sel.pt.y = sel.fl.pt.y;
    if ((sel.fl.idPlanet == 0xffff))
        goto L_407a;
    else
        goto L_4056;

L_4056:
    if ((sel.fl.idPlanet == sel.pl.id))
        goto L_4080;
    else
        goto L_4062;

L_4062:
    if ((FLookupPlanet(sel.fl.idPlanet, &(sel.pl)) != 0x0))
        goto L_4080;
    else
        goto L_407a;

L_407a:
    sel.pl.id = 0xffff;

L_4080:
    if ((sel.pl.id == 0xffff))
        goto L_4090;
    else
        goto L_408a;

L_408a:
    t_merge_4093_0001 = 0x1;
    goto L_4093;

L_4090:
    t_merge_4093_0001 = 0x0;

L_4093:
    sel.grobjFull = (t_merge_4093_0001 | 0x2);
    sel.iwpAct = 0x0;
    if ((fAi != 0x0))
        goto L_40ec;
    else
        goto L_40a9;

L_40a9:
    FillOrdersLB();
    FillFleetCompLB();
    /* untranslated: call FillBattleDD(((part[96:2](sel.fl) & 0xff) + 0x1)) -> callresult(void) */
    idSkip = iObjSel;
    /* untranslated: call SendMessage(rghwndOrderDD[0x0], CB_SETCURSEL, (part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xf), 0x0) -> callresult(LRESULT) */

L_40ec:
    sel.grobj = grobjNew;
    sel.id = iObjSel;
    gd.fSetMassMode = 0x0;
    gd.fSetRouteMode = 0x0;
    if ((fAi != 0x0))
        goto L_4299;
    else
        goto L_411a;

L_411a:
    if ((fSameType != 0x0))
        goto L_4207;
    else
        goto L_4123;

L_4123:
    i = 0x0;
    goto L_4141;

L_412b:
    ShowWindow(rghwndBtn[i], SW_HIDE);
    i = (i + 0x1);

L_4141:
    if ((i < 0xd))
        goto L_412b;
    else
        goto L_414a;

L_414a:
    i = 0x0;
    goto L_4168;

L_4152:
    ShowWindow(rghwndOrderDD[i], SW_HIDE);
    i = (i + 0x1);

L_4168:
    if ((i < 0x3))
        goto L_4152;
    else
        goto L_4171;

L_4171:
    ShowWindow(hwndOrderED, SW_HIDE);
    ShowWindow(hwndShipDD, SW_HIDE);
    ShowWindow(hwndBattleDD, SW_HIDE);
    ShowWindow(hwndShipLB, SW_HIDE);
    ShowWindow(hwndFleetCompLB, SW_HIDE);
    ShowWindow(hwndPlanetProdLB, SW_HIDE);
    ShowWindow(hwndRepCB, SW_HIDE);
    i = 0x0;
    goto L_41fe;

L_41d4:
    rgrcRef[i].bottom = 0xfffa;
    rgrcRef[i].top = 0xfffb;
    i = (i + 0x1);

L_41fe:
    if ((i < 0x13))
        goto L_41d4;
    else
        goto L_4207;

L_4207:
    FillShipDD(idSkip);
    if ((fSameType == 0x0))
        goto L_422e;
    else
        goto L_421b;

L_421b:
    DrawPlanShip(0x0, 0x4fff);
    goto L_4275;

L_422e:
    InvalidateRect(hwndPlanet, 0x0, 0x1);
    if (((grbitScan & 0x10) == 0x0))
        goto L_4275;
    else
        goto L_4251;

L_4251:
    if ((sel.grobj != grobjPlanet))
        goto L_4275;
    else
        goto L_425b;

L_425b:
    grbitScan = (grbitScan & 0xffef);
    InvalidateRect(hwndTb, 0x0, 0x1);

L_4275:
    SetPlanetTitleBar(hwndPlanet);
    if ((gd.fTutorial == 0x0))
        goto L_4299;
    else
        goto L_4294;

L_4294:
    AdvanceTutor();

L_4299:
    return;
}

void FillShipDD(int16_t idSkip) {
    THING   *lpthMac;
    int16_t  i;
    THING   *lpth;
    FLEET   *lpfl;
    POINT    ptSel;
    uint16_t t_merge_43c6_0001;
    uint16_t t_merge_448d_0001;

L_42a0:
    SendMessage(hwndShipDD, CB_RESETCONTENT, 0x0, 0x0);
    if ((sel.grobj != grobjPlanet))
        goto L_42e5;
    else
        goto L_42cc;

L_42cc:
    ptSel.x = rgptPlan[sel.id].x;
    ptSel.y = rgptPlan[sel.id].y;
    goto L_42f2;

L_42e5:
    ptSel.x = sel.fl.pt.x;
    ptSel.y = sel.fl.pt.y;

L_42f2:
    i = 0x0;
    goto L_42fe;

L_42fa:
    i = (i + 0x1);

L_42fe:
    if ((i >= cFleet))
        goto L_43e4;
    else
        goto L_4309;

L_4309:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_4339;
    else
        goto L_4331;

L_4331:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_43e4;
    else
        goto L_4339;

L_4339:
    if ((idSkip != 0xffff))
        goto L_4352;
    else
        goto L_4342;

L_4342:
    if ((sel.id == lpfl->idPlanet))
        goto L_438c;
    else
        goto L_4352;

L_4352:
    if ((idSkip == 0xffff))
        goto L_42fa;
    else
        goto L_435b;

L_435b:
    if ((lpfl->id == idSkip))
        goto L_42fa;
    else
        goto L_4369;

L_4369:
    if ((lpfl->pt.x != sel.fl.pt.x))
        goto L_42fa;
    else
        goto L_4383;

L_4383:
    if ((lpfl->pt.y != sel.fl.pt.y))
        goto L_42fa;
    else
        goto L_438c;

L_438c:
    PszGetFleetName(lpfl->id);
    memmove(&(szWork[0x1]), &(szWork), 0x32);
    if ((lpfl->iPlayer != idPlayer))
        goto L_43c3;
    else
        goto L_43bd;

L_43bd:
    t_merge_43c6_0001 = 0x20;
    goto L_43c6;

L_43c3:
    t_merge_43c6_0001 = 0x78;

L_43c6:
    LOWORD(szWork) = LOBYTE(t_merge_43c6_0001);
    /* untranslated: call SendMessage(hwndShipDD, CB_ADDSTRING, 0x0, words(ds, 0x57a4)) -> callresult(LRESULT) */

L_43e4:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_44ac;

L_440f:
    if ((lpth->ith != ithMineralPacket))
        goto L_44a8;
    else
        goto L_4425;

L_4425:
    /* untranslated: branch lpth->pt.x != ss:[ptSel] ? L_44a8 : L_443f */

L_443f:
    /* untranslated: branch lpth->pt.y != ss:[ptSel+0x2] ? L_44a8 : L_4448 */

L_4448:
    PszGetThingName(lpth->idFull);
    memmove(&(szWork[0x1]), &(szWork), 0x32);
    if ((lpth->iplr != idPlayer))
        goto L_448a;
    else
        goto L_4484;

L_4484:
    t_merge_448d_0001 = 0x20;
    goto L_448d;

L_448a:
    t_merge_448d_0001 = 0x78;

L_448d:
    LOWORD(szWork) = LOBYTE(t_merge_448d_0001);
    /* untranslated: call SendMessage(hwndShipDD, CB_ADDSTRING, 0x0, words(ds, 0x57a4)) -> callresult(LRESULT) */

L_44a8:
    lpth = (lpth + 0x1);

L_44ac:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_440f;
    else
        goto L_44ba;

L_44ba:
    SendMessage(hwndShipDD, CB_SETCURSEL, 0x0, 0x0);
    return;
}

void SelectAdjPlanet(int16_t dInc, int16_t idPlanet) {
    PLANET *lpPlT;
    int16_t i;
    PLANET *lpPl;
    SCAN    scan;
    int16_t fWrap;

L_44da:
    fWrap = 0x0;
    if ((cPlanet <= 0x0))
        goto L_4747;
    else
        goto L_44f2;

L_44f2:
    if ((idPlanet == 0xffff))
        goto L_4747;
    else
        goto L_44f8;

L_44f8:

L_44fe:
    if ((dInc == 0x0))
        goto L_450d;
    else
        goto L_4507;

L_4507:
    idPlanet = sel.pl.id;

L_450d:
    if ((vrptPlanet.fCached != 0x0))
        goto L_4527;
    else
        goto L_4517;

L_4517:
    InvalidateReport(0x0, 0x1);

L_4527:
    lpPlT = lpPlanets;
    lpPl = lpPlanets;
    i = 0x0;
    goto L_4555;

L_4542:
    i = (i + 0x1);
    lpPl = (lpPl + 0x1);

L_4555:
    if ((i >= cPlanet))
        goto L_4574;
    else
        goto L_4560;

L_4560:
    if ((lpPl->id == idPlanet))
        goto L_4574;
    else
        goto L_456b;

L_456b:

L_4574:
    if ((i == cPlanet))
        goto L_4591;
    else
        goto L_457f;

L_457f:
    if ((lpPl->det == 0x7))
        goto L_45be;
    else
        goto L_4591;

L_4591:
    scan.pt.x = rgptPlan[idPlanet].x;
    scan.pt.y = rgptPlan[idPlanet].y;
    scan.grobj = 0x81;
    ChangeScanSel(&(scan), 0x0);
    goto FinishUp;

L_45be:
    if ((dInc == 0x0))
        goto L_4676;
    else
        goto L_45c7;

L_45c7:
    i = 0x0;
    goto L_45d3;

L_45cf:
    i = (i + 0x1);

L_45d3:
    if ((i >= rgplr[idPlayer].cPlanet))
        goto L_461c;
    else
        goto L_45ea;

L_45ea:
    if ((lpPlT[vlprgidPlanet[i]].id == idPlanet))
        goto L_461c;
    else
        goto L_4613;

L_4613:

L_461c:
    i = (i + dInc);
    if ((i < rgplr[idPlayer].cPlanet))
        goto L_4641;
    else
        goto L_4639;

L_4639:
    i = 0x0;
    goto L_465f;

L_4641:
    if ((i >= 0x0))
        goto L_465f;
    else
        goto L_464a;

L_464a:
    i = (rgplr[idPlayer].cPlanet + 0xffff);

L_465f:
    i = vlprgidPlanet[i];

L_4676:
    lpPlT = ((uint8_t *)(lpPlT) + LOWORD((0x38 * i)));
    idPlanet = lpPlT->id;
    if ((lpPlT->iPlayer != idPlayer))
        goto L_4747;
    else
        goto L_4694;

L_4694:

L_469a:
    scan.pt.x = rgptPlan[idPlanet].x;
    scan.pt.y = rgptPlan[idPlanet].y;
    scan.grobj = 0x81;
    ChangeScanSel(&(scan), 0x0);
    RedrawScanSel(0x0, 0x0);
    ChangeMainObjSel(grobjPlanet, idPlanet);
    RedrawScanSel(0x0, 0x1);

FinishUp:
    /* untranslated: call CtrPointScan(words(rgptPlan[idPlanet].y, rgptPlan[idPlanet].x), 0x1) -> callresult(void) */
    DrawScannerSBar(0x0, 0x0, 0x0, 0x0);
    InvalidateRect(hwndMine, 0x0, 0x1);
    SetMineralTitleBar(hwndMine);

L_4747:
    return;
}

int16_t IdFindAdjStarbase(int16_t idPlanet, int16_t fNext) {
    PLANET *lpplMac;
    int16_t idLast;
    int16_t idFirst;
    PLANET *lppl;
    int16_t idAfter;
    int16_t idBefore;

L_474e:
    idLast = 0xffff;
    idFirst = 0xffff;
    idAfter = 0xffff;
    idBefore = 0xffff;
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_4854;

L_4793:
    if ((lppl->iPlayer != idPlayer))
        goto L_4850;
    else
        goto L_47a2;

L_47a2:
    if ((lppl->fStarbase == 0x0))
        goto L_4850;
    else
        goto L_47b6;

L_47b6:

L_47bc:
    if ((LphuldefFromId(rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef)->hul.wtCargoMax == 0x0))
        goto L_4850;
    else
        goto L_47f5;

L_47f5:

L_47fb:
    if ((lppl->id <= idPlanet))
        goto L_481e;
    else
        goto L_4809;

L_4809:
    if ((idAfter != 0xffff))
        goto L_4835;
    else
        goto L_4812;

L_4812:
    idAfter = lppl->id;

L_481e:
    if ((lppl->id >= idPlanet))
        goto L_4835;
    else
        goto L_482c;

L_482c:
    idBefore = lppl->id;

L_4835:
    if ((idFirst != 0xffff))
        goto L_4847;
    else
        goto L_483e;

L_483e:
    idFirst = lppl->id;

L_4847:
    idLast = lppl->id;

L_4850:
    lppl = (lppl + 0x1);

L_4854:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_4793;
    else
        goto L_4862;

L_4862:
    if ((fNext == 0x0))
        goto L_4883;
    else
        goto L_486b;

L_486b:
    if ((idAfter != 0xffff))
        goto L_487a;
    else
        goto L_4874;

L_4874:
    return idFirst;

L_487a:
    return idAfter;

L_4883:
    if ((idBefore != 0xffff))
        goto L_4892;
    else
        goto L_488c;

L_488c:
    return idLast;

L_4892:
    return idBefore;
}

void PlanetClick(int16_t x, int16_t y, int16_t sks, int16_t fRightBtn) {
    int16_t  bt;
    POINT    pt;
    int16_t  ctile;
    int16_t  dy;
    RECT     rcTitle;
    int16_t  i;
    int16_t  xRel;
    uint16_t iCol;
    int16_t  iCur;
    TILE    *prgtile;
    RECT     rc;
    BTNT     btnt;
    HDC      hdc;
    TILE     tile;
    POINT    ptNew;
    int16_t  t_merge_49b6_0001;
    uint16_t t_merge_4b50_0001;
    uint16_t t_merge_4cf6_0001;
    uint16_t t_merge_4d28_0001;
    int16_t  t_merge_4dd8_0001;
    int16_t  t_merge_4fef_0001;

L_489e:
    bt = 0x70;
    if ((sel.grobj != grobjPlanet))
        goto L_48c3;
    else
        goto L_48b6;

L_48b6:
    prgtile = 0x7fc;
    ctile = 0x6;
    goto L_48dd;

L_48c3:
    if ((sel.grobj != grobjFleet))
        goto L_5155;
    else
        goto L_48cd;

L_48cd:
    prgtile = 0x90e;
    ctile = 0x7;
    goto L_48dd;

L_48dd:
    /* untranslated: iCol = (words(x, 0x0) / 0xc6) */
    xRel = (x - LOWORD((iCol * 0xc6)));
    if ((xRel < 0x4))
        goto L_5155;
    else
        goto L_4903;

L_4903:
    if ((xRel >= 0xc2))
        goto L_5155;
    else
        goto L_490a;

L_490a:

L_4910:
    i = 0x0;
    goto L_49d7;

L_4918:
    /* untranslated: branch (part[10:2](prgtile[i]) & 0x7) < iCol ? L_49d3 : L_4933 */

L_4933:

L_493c:
    /* untranslated: branch (part[10:2](prgtile[i]) & 0x7) > iCol ? L_5155 : L_4957 */

L_4957:

L_495d:
    if ((y < prgtile[i].yTop))
        goto L_49d3;
    else
        goto L_4977;

L_4977:
    /* untranslated: branch ((part[10:2](prgtile[i]) >> 0x7) & 0x1) == 0x0 ? L_49b0 : L_499a */

L_499a:
    t_merge_49b6_0001 = prgtile[i].dyFull;
    goto L_49b6;

L_49b0:
    t_merge_49b6_0001 = (dyArial8 + 0x3);

L_49b6:
    if ((y < (t_merge_49b6_0001 + prgtile[i].yTop)))
        goto L_49e2;
    else
        goto L_49cd;

L_49cd:

L_49d3:
    i = (i + 0x1);

L_49d7:
    if ((i < ctile))
        goto L_4918;
    else
        goto L_49e2;

L_49e2:
    if ((i == ctile))
        goto L_5155;
    else
        goto L_49ea;

L_49ea:

L_49f0:
    pt.x = x;
    pt.y = y;
    rcTitle.top = prgtile[i].yTop;
    rcTitle.bottom = (((dyArial8 + 0x3) + rcTitle.top) + 0x1);
    rcTitle.left = (LOWORD((iCol * 0xc6)) + 0x4);
    rcTitle.right = (rcTitle.left + 0xbf);
    /* untranslated: branch PtInRect(&rcTitle, words(pt.y, pt.x)) == 0x0 ? L_502b : L_4a51 */

L_4a51:
    if ((fRightBtn != 0x0))
        goto L_502b;
    else
        goto L_4a5a;

L_4a5a:
    rc = rcTitle;
    rc.top = (rc.top + 0x1);
    rc.left = (rc.right + 0xffef);
    /* untranslated: branch PtInRect(&rc, words(pt.y, pt.x)) == 0x0 ? L_4bb8 : L_4a90 */

L_4a90:
    OffsetRc(&(rc), 0xffff, 0x0);
    /* untranslated: branch ((part[10:2](prgtile[i]) >> 0x7) & 0x1) == 0x0 ? L_4ace : L_4ac7 */

L_4ac7:
    bt = (bt | 0x0);
    goto L_4ad2;

L_4ace:
    bt = (bt | 0x1);

L_4ad2:
    InitBtnTrack(&(btnt), hwndPlanet, 0x0, &(rc), bt, 0x0, 0x0, 0x0, 0x0);

L_4afd:
    if ((FTrackBtn(&(btnt)) != 0x0))
        goto L_4afd;
    else
        goto L_4b0e;

L_4b0e:

L_4b14:
    if ((btnt.fDown == 0x0))
        goto L_5155;
    else
        goto L_4b24;

L_4b24:
    /* untranslated: branch ((part[10:2](prgtile[i]) >> 0x7) & 0x1) != 0x0 ? L_4b4d : L_4b47 */

L_4b47:
    t_merge_4b50_0001 = 0x1;
    goto L_4b50;

L_4b4d:
    t_merge_4b50_0001 = 0x0;

L_4b50:
    /* untranslated: part[10:2](prgtile[i]) = ((part[10:2](prgtile[i]) & 0xff7f) | ((t_merge_4b50_0001 & 0x1) << 0x7)) */
    /* untranslated: call ReflowColumn((part[10:2](prgtile[i]) & 0x7), i, 0x1) -> callresult(void) */

L_4bb8:
    /* untranslated: branch ((part[10:2](prgtile[i]) >> 0x7) & 0x1) == 0x0 ? L_4bf7 : L_4bdb */

L_4bdb:
    rcTitle.bottom = ((rcTitle.top + prgtile[i].dyFull) + 0x1);

L_4bf7:
    hdc = GetDC(hwndPlanet);
    DrawFuzzyBorder(hdc, &(rcTitle));
    SetCapture(hwndPlanet);
    ptNew.x = pt.x;
    ptNew.y = pt.y;

L_4c27:
    if ((FGetMouseMove(&(ptNew)) == 0x0))
        goto L_4c98;
    else
        goto L_4c3b;

L_4c3b:
    if ((pt.x != ptNew.x))
        goto L_4c51;
    else
        goto L_4c46;

L_4c46:
    if ((pt.y == ptNew.y))
        goto L_4c27;
    else
        goto L_4c51;

L_4c51:
    DrawFuzzyBorder(hdc, &(rcTitle));
    OffsetRc(&(rcTitle), (ptNew.x - pt.x), (ptNew.y - pt.y));
    pt.x = ptNew.x;
    pt.y = ptNew.y;
    DrawFuzzyBorder(hdc, &(rcTitle));

L_4c98:
    DrawFuzzyBorder(hdc, &(rcTitle));
    ReleaseCapture();
    ReleaseDC(hwndPlanet, hdc);
    pt.x = (((rcTitle.right - rcTitle.left) >> 0x1) + rcTitle.left);
    pt.y = (((rcTitle.bottom - rcTitle.top) >> 0x1) + rcTitle.top);
    if ((0x1 >= ((uint32_t)(pt.x) / 0xc6)))
        goto L_4ced;
    else
        goto L_4ce7;

L_4ce7:
    t_merge_4cf6_0001 = 0x1;
    goto L_4cf6;

L_4ced:
    t_merge_4cf6_0001 = ((uint32_t)(pt.x) / 0xc6);

L_4cf6:
    if ((0x0 <= t_merge_4cf6_0001))
        goto L_4d06;
    else
        goto L_4d00;

L_4d00:
    t_merge_4d28_0001 = 0x0;
    goto L_4d28;

L_4d06:
    if ((0x1 >= ((uint32_t)(pt.x) / 0xc6)))
        goto L_4d1f;
    else
        goto L_4d19;

L_4d19:
    t_merge_4d28_0001 = 0x1;
    goto L_4d28;

L_4d1f:
    t_merge_4d28_0001 = ((uint32_t)(pt.x) / 0xc6);

L_4d28:
    iCol = t_merge_4d28_0001;
    iCur = i;
    i = 0x0;
    goto L_4d3d;

L_4d39:
    i = (i + 0x1);

L_4d3d:
    if ((i >= ctile))
        goto L_4d70;
    else
        goto L_4d48;

L_4d48:
    /* untranslated: branch (part[10:2](prgtile[i]) & 0x7) < iCol ? L_4d39 : L_4d63 */

L_4d63:

L_4d6c:
    i = (i + 0x1);

L_4d70:
    if ((i >= ctile))
        goto L_4e00;
    else
        goto L_4d7b;

L_4d7b:
    /* untranslated: branch (part[10:2](prgtile[i]) & 0x7) != iCol ? L_4e00 : L_4d99 */

L_4d99:
    /* untranslated: branch ((part[10:2](prgtile[i]) >> 0x7) & 0x1) == 0x0 ? L_4dd2 : L_4dbc */

L_4dbc:
    t_merge_4dd8_0001 = prgtile[i].dyFull;
    goto L_4dd8;

L_4dd2:
    t_merge_4dd8_0001 = (dyArial8 + 0x3);

L_4dd8:
    dy = t_merge_4dd8_0001;
    if ((pt.y < ((dy >> 0x1) + prgtile[i].yTop)))
        goto L_4e00;
    else
        goto L_4df7;

L_4df7:

L_4e00:
    if ((i == iCur))
        goto L_4e19;
    else
        goto L_4e0b;

L_4e0b:
    if ((i != (iCur + 0x1)))
        goto L_4eba;
    else
        goto L_4e19;

L_4e19:
    /* untranslated: i = (part[10:2](prgtile[iCur]) & 0x7) */
    if ((iCol == i))
        goto L_5155;
    else
        goto L_4e3d;

L_4e3d:
    /* untranslated: part[10:2](prgtile[iCur]) = ((part[10:2](prgtile[iCur]) & 0xfff8) | (iCol & 0x7)) */
    ReflowColumn(iCol, iCur, 0x1);
    if ((iCol >= i))
        goto L_4ea5;
    else
        goto L_4e8c;

L_4e8c:
    ReflowColumn(i, (iCur + 0x1), 0x1);
    goto L_5155;

L_4ea5:
    ReflowColumn(i, iCur, 0x1);

L_4eba:
    tile = prgtile[iCur];
    if ((i >= iCur))
        goto L_4f27;
    else
        goto L_4ee4;

L_4ee4:
    memmove(((uint8_t *)(prgtile) + ((i + 0x1) * 0x10)), ((uint8_t *)(prgtile) + (i * 0x10)), ((iCur - i) * 0x10));
    iCur = (iCur + 0x1);
    goto L_4f6a;

L_4f27:
    memmove(((uint8_t *)(prgtile) + (iCur * 0x10)), ((uint8_t *)(prgtile) + ((iCur + 0x1) * 0x10)), (((i - iCur) + 0xffff) * 0x10));
    i = (i - 0x1);

L_4f6a:
    prgtile[i] = tile;
    /* untranslated: part[10:2](prgtile[i]) = ((part[10:2](prgtile[i]) & 0xfff8) | (iCol & 0x7)) */
    if ((tile.iCol != iCol))
        goto L_4fff;
    else
        goto L_4fd7;

L_4fd7:
    if ((i >= iCur))
        goto L_4fec;
    else
        goto L_4fe6;

L_4fe6:
    t_merge_4fef_0001 = i;
    goto L_4fef;

L_4fec:
    t_merge_4fef_0001 = iCur;

L_4fef:
    ReflowColumn(iCol, t_merge_4fef_0001, 0x1);
    goto L_5155;

L_4fff:
    ReflowColumn(iCol, i, 0x1);
    ReflowColumn(tile.iCol, iCur, 0x1);

L_502b:
    if ((sel.grobj != grobjFleet))
        goto L_509a;
    else
        goto L_5035;

L_5035:
    if ((prgtile[i].grbit == 0x20))
        goto L_50b3;
    else
        goto L_504e;

L_504e:
    if ((prgtile[i].grbit == 0x100))
        goto L_50b3;
    else
        goto L_5068;

L_5068:
    if ((prgtile[i].grbit == 0x1))
        goto L_50b3;
    else
        goto L_5081;

L_5081:
    if ((prgtile[i].grbit == 0x10))
        goto L_50b3;
    else
        goto L_509a;

L_509a:
    if ((prgtile[i].grbit != 0x4))
        goto L_50ce;
    else
        goto L_50b3;

L_50b3:
    /* untranslated: call ClickInShipOrders(words(pt.y, pt.x), sks, 0x0, fRightBtn) -> callresult(uint16_t) */
    goto L_5155;

L_50ce:
    if ((sel.grobj != grobjPlanet))
        goto L_5155;
    else
        goto L_50d8;

L_50d8:
    if ((prgtile[i].grbit == 0x1))
        goto L_513d;
    else
        goto L_50f1;

L_50f1:
    if ((prgtile[i].grbit == 0x100))
        goto L_513d;
    else
        goto L_510b;

L_510b:
    if ((prgtile[i].grbit == 0x40))
        goto L_513d;
    else
        goto L_5124;

L_5124:
    if ((prgtile[i].grbit != 0x8))
        goto L_5155;
    else
        goto L_513d;

L_513d:
    /* untranslated: call ClickInPlanetOrders(words(pt.y, pt.x), sks, 0x0, fRightBtn) -> callresult(uint16_t) */

L_5155:
    return;
}

uint16_t ClickInPlanetOrders(POINT pt, int16_t sks, int16_t fCursor, int16_t fRightBtn) {
    int16_t  i;
    int32_t  rglQuan[3];
    int16_t  iWarp;
    BTNT     btnt;
    uint16_t t_merge_52ad_0001;

L_515c:
    if ((sel.grobj == grobjPlanet))
        goto L_5175;
    else
        goto L_516f;

L_516f:
    return 0x0;

L_5175:
    if ((fRightBtn == 0x0))
        goto L_5187;
    else
        goto L_517e;

L_517e:
    return 0x0;

L_5187:
    /* untranslated: branch PtInRect(&rgrcRef[0x6], words(pt.y, pt.x)) == 0x0 ? L_5260 : L_51a1 */

L_51a1:
    if ((fCursor == 0x0))
        goto L_51b0;
    else
        goto L_51aa;

L_51aa:
    return hcurArrowHelp;

L_51b0:
    i = ((uint32_t)((pt.y - rgrcRef[0x6].top)) / dyArial8);
    GlobalPD.grPopup = grPopupMineral;
    HIWORD(GlobalPD) = i;
    /* untranslated: part[4:2](GlobalPD) = signhiword(i) */
    /* untranslated: part[10:2](GlobalPD) = LOWORD(sel.pl.rgwtMin[i]) */
    /* untranslated: part[12:2](GlobalPD) = HIWORD(sel.pl.rgwtMin[i]) */
    /* untranslated: part[14:2](GlobalPD) = sel.pl.rgMinConc[i] */
    GlobalPD.iPlrMax = 0x0;
    EstMineralsMined(&(sel.pl), rglQuan, 0xffffffff, 0x0);
    GlobalPD.rgi[0x4] = rglQuan[i];
    /* untranslated: part[6:2](GlobalPD) = sel.pl.fHomeworld */
    /* untranslated: part[8:2](GlobalPD) = 0x0 */
    Popup(hwndPlanet, pt.x, pt.y);
    goto L_5871;

L_5260:
    /* untranslated: branch PtInRect(&rgrcRef[0x7], words(pt.y, pt.x)) == 0x0 ? L_5362 : L_527a */

L_527a:
    if ((fCursor == 0x0))
        goto L_5289;
    else
        goto L_5283;

L_5283:
    return hcurArrowHelp;

L_5289:
    GlobalPD.grPopup = grPopupPlanetIndustry;
    HIWORD(GlobalPD) = sel.pl.id;
    if ((pt.y < (rgrcRef[0x7].top + dyArial8)))
        goto L_52aa;
    else
        goto L_52a4;

L_52a4:
    t_merge_52ad_0001 = 0x1;
    goto L_52ad;

L_52aa:
    t_merge_52ad_0001 = 0x0;

L_52ad:
    /* untranslated: part[10:2](GlobalPD) = t_merge_52ad_0001 */
    /* untranslated: branch part[10:2](GlobalPD) == 0x0 ? L_5305 : L_52ba */

L_52ba:
    /* untranslated: part[4:2](GlobalPD) = CMaxFactories(&sel.pl, idPlayer) */
    /* untranslated: part[6:2](GlobalPD) = sel.pl.cFactories */
    /* untranslated: part[8:2](GlobalPD) = CMaxOperableFactories(&sel.pl, idPlayer, 0x0) */
    goto L_534d;

L_5305:
    /* untranslated: part[4:2](GlobalPD) = CMaxMines(&sel.pl, idPlayer) */
    /* untranslated: part[6:2](GlobalPD) = sel.pl.cMines */
    /* untranslated: part[8:2](GlobalPD) = CMaxOperableMines(&sel.pl, idPlayer, 0x0) */

L_534d:
    Popup(hwndPlanet, pt.x, pt.y);
    goto L_5871;

L_5362:
    /* untranslated: branch PtInRect(&rgrcRef[0x8], words(pt.y, pt.x)) == 0x0 ? L_540c : L_537c */

L_537c:
    if ((fCursor == 0x0))
        goto L_538b;
    else
        goto L_5385;

L_5385:
    return hcurArrowHelp;

L_538b:
    GlobalPD.grPopup = grPopupResources;
    HIWORD(GlobalPD) = sel.pl.id;
    CResourcesAtPlanet(&(sel.pl), idPlayer);
    /* untranslated: part[6:2](GlobalPD) = callresult(int16_t) */
    /* untranslated: part[4:2](GlobalPD) = callresult(int16_t) */
    if ((sel.pl.fNoResearch != 0x0))
        goto L_53f7;
    else
        goto L_53cd;

L_53cd:
    if ((0x0 != 0x0))
        goto L_53f7;
    else
        goto L_53d5;

L_53d5:
    /* untranslated: part[6:2](GlobalPD) = (part[6:2](GlobalPD) - MulDiv(part[4:2](GlobalPD), sext8to16(rgplr[idPlayer].pctResearch), 0x64)) */

L_53f7:
    Popup(hwndPlanet, pt.x, pt.y);
    goto L_5871;

L_540c:
    /* untranslated: branch PtInRect(&rgrcRef[0x9], words(pt.y, pt.x)) == 0x0 ? L_5456 : L_5426 */

L_5426:
    if ((fCursor == 0x0))
        goto L_5435;
    else
        goto L_542f;

L_542f:
    return hcurArrowHelp;

L_5435:
    GlobalPD.grPopup = grPopupPlanet;
    HIWORD(GlobalPD) = sel.pl.id;
    Popup(hwndPlanet, pt.x, pt.y);
    goto L_5871;

L_5456:
    /* untranslated: branch PtInRect(&rgrcRef[0xa], words(pt.y, pt.x)) == 0x0 ? L_54c9 : L_5470 */

L_5470:
    if ((sel.pl.cDefenses != 0x0))
        goto L_5493;
    else
        goto L_5485;

L_5485:
    if ((0x0 != 0x0))
        goto L_5493;
    else
        goto L_548d;

L_548d:
    return 0x0;

L_5493:
    if ((fCursor == 0x0))
        goto L_54a2;
    else
        goto L_549c;

L_549c:
    return hcurArrowHelp;

L_54a2:
    /* untranslated: call FGetBestDefensePart(&part[2:0](GlobalPD)) -> callresult(int16_t) */
    GlobalPD.grPopup = grPopupComponent;
    Popup(hwndPlanet, pt.x, pt.y);
    goto L_5871;

L_54c9:
    /* untranslated: branch PtInRect(&rgrcRef[0xb], words(pt.y, pt.x)) == 0x0 ? L_55ab : L_54e3 */

L_54e3:
    if ((sel.pl.iScanner != 0x1f))
        goto L_552f;
    else
        goto L_5500;

L_5500:
    if ((0x0 != 0x0))
        goto L_552f;
    else
        goto L_5508;

L_5508:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) == raMacintosh))
        goto L_552f;
    else
        goto L_5529;

L_5529:
    return 0x0;

L_552f:
    if ((fCursor == 0x0))
        goto L_553e;
    else
        goto L_5538;

L_5538:
    return hcurArrowHelp;

L_553e:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_5584;
    else
        goto L_555f;

L_555f:
    GlobalPD.grPopup = grPopupString;
    HIWORD(GlobalPD) = 0xb4;
    /* untranslated: part[4:2](GlobalPD) = 0x4a6c */
    CchGetString(idsRaceCannotBuildPlanetaryScannersStarbasesHave, szPopupBuffer);
    goto L_5596;

L_5584:
    /* untranslated: call LookupBestPlanetaryScanner(&part[2:0](GlobalPD)) -> callresult(void) */
    GlobalPD.grPopup = grPopupComponent;

L_5596:
    Popup(hwndPlanet, pt.x, pt.y);
    goto L_5871;

L_55ab:
    /* untranslated: branch PtInRect(&rgrcRef[0xd], words(pt.y, pt.x)) == 0x0 ? L_567a : L_55c5 */

L_55c5:
    iWarp = IWarpMAFromLppl(&(sel.pl), 0x0);
    if ((iWarp != 0x0))
        goto L_55ea;
    else
        goto L_55e4;

L_55e4:
    return 0x0;

L_55ea:
    if ((fCursor == 0x0))
        goto L_55f9;
    else
        goto L_55f3;

L_55f3:
    return hcurHand;

L_55f9:
    InitBtnTrack(&(btnt), hwndPlanet, 0x0, &(rgrcRef[0xd]), 0x8, 0x50, gd.fSetMassMode, 0x1, PszGetCompressedString(idsSetDest));

L_5636:
    if ((FTrackBtn(&(btnt)) != 0x0))
        goto L_5636;
    else
        goto L_5647;

L_5647:

L_564d:
    /* untranslated: ss:[bp-0x2c] = (((btnt.fDown << 0x8) ^ HIWORD(gd)) & 0x100) */
    gd.fSetMassMode = 0x0;
    /* untranslated: HIWORD(gd) = (HIWORD(gd) | ss:[bp-0x2c]) */
    goto L_5871;

L_567a:
    /* untranslated: branch PtInRect(&rgrcRef[0xe], words(pt.y, pt.x)) == 0x0 ? L_56fb : L_5694 */

L_5694:
    if ((fCursor == 0x0))
        goto L_56a3;
    else
        goto L_569d;

L_569d:
    return hcurArrowHelp;

L_56a3:
    GlobalPD.grPopup = grPopupShdef;
    HIWORD(GlobalPD) = (LOWORD(rglpshdefSB[idPlayer]) + LOWORD((sel.pl.isb * 0x93)));
    /* untranslated: part[4:2](GlobalPD) = HIWORD(rglpshdefSB[idPlayer]) */
    /* untranslated: part[8:2](GlobalPD) = 0x0 */
    /* untranslated: part[6:2](GlobalPD) = 0x1 */
    /* untranslated: part[10:2](GlobalPD) = 0x0 */
    /* untranslated: part[12:2](GlobalPD) = 0x0 */
    Popup(hwndPlanet, pt.x, pt.y);
    goto L_5871;

L_56fb:
    /* untranslated: branch PtInRect(&rgrcRef[0x10], words(pt.y, pt.x)) == 0x0 ? L_5795 : L_5715 */

L_5715:
    iWarp = IWarpMAFromLppl(&(sel.pl), 0x0);
    if ((iWarp != 0x0))
        goto L_573a;
    else
        goto L_5734;

L_5734:
    return 0x0;

L_573a:
    if ((fCursor == 0x0))
        goto L_5749;
    else
        goto L_5743;

L_5743:
    return hcurArrowHelp;

L_5749:
    HIWORD(GlobalPD) = 0x200;
    GlobalPD.part.hs.iItem = (iWarp + 0x2);
    /* untranslated: call FLookupPart(&part[2:0](GlobalPD)) -> callresult(int16_t) */
    GlobalPD.grPopup = grPopupComponent;
    Popup(hwndPlanet, pt.x, pt.y);
    goto L_5871;

L_5795:
    /* untranslated: branch PtInRect(&rgrcRef[0xf], words(pt.y, pt.x)) == 0x0 ? L_57ca : L_57af */

L_57af:
    /* untranslated: call ClickInShipOrders(words(pt.y, pt.x), sks, 0x0, fRightBtn) -> callresult(uint16_t) */
    goto L_5871;

L_57ca:
    /* untranslated: branch PtInRect(&rgrcRef[0x11], words(pt.y, pt.x)) == 0x0 ? L_5871 : L_57e4 */

L_57e4:
    if ((fCursor == 0x0))
        goto L_57f3;
    else
        goto L_57ed;

L_57ed:
    return hcurHand;

L_57f3:
    InitBtnTrack(&(btnt), hwndPlanet, 0x0, &(rgrcRef[0x11]), 0x8, 0x50, gd.fSetRouteMode, 0x1, PszGetCompressedString(idsRoute2));

L_5830:
    if ((FTrackBtn(&(btnt)) != 0x0))
        goto L_5830;
    else
        goto L_5841;

L_5841:

L_5847:
    /* untranslated: ss:[bp-0x2a] = (((btnt.fDown << 0xd) ^ HIWORD(gd)) & 0x2000) */
    gd.fSetRouteMode = 0x0;
    /* untranslated: HIWORD(gd) = (HIWORD(gd) | ss:[bp-0x2a]) */

L_5871:
    return 0x0;
}

void EnsureTileSize(int16_t fSmallTiles) {
    int16_t  iMul;
    int16_t  i;
    int16_t  grobjSav;
    uint16_t t_merge_58c9_0001;

L_587e:
    if ((fSmallTiles == gd.fSmallTileMode))
        goto L_5b7a;
    else
        goto L_5898;

L_5898:

L_589e:
    gd.fSmallTileMode = fSmallTiles;
    if ((fSmallTiles == 0x0))
        goto L_58c6;
    else
        goto L_58c0;

L_58c0:
    t_merge_58c9_0001 = 0xffff;
    goto L_58c9;

L_58c6:
    t_merge_58c9_0001 = 0x1;

L_58c9:
    iMul = t_merge_58c9_0001;
    i = 0x0;
    goto L_5979;

L_58d4:
    if ((rgtilePlanet[i].grbit != 0x40))
        goto L_590b;
    else
        goto L_58ed;

L_58ed:
    rgtilePlanet[i].dyFull = (rgtilePlanet[i].dyFull + LOWORD((((dyArial8 + 0x2) * 0x2) * iMul)));

L_590b:
    if ((rgtilePlanet[i].grbit != 0x4))
        goto L_5942;
    else
        goto L_5924;

L_5924:
    rgtilePlanet[i].dyFull = (rgtilePlanet[i].dyFull + LOWORD((((dyArial8 + 0x4) * 0x2) * iMul)));

L_5942:
    if ((rgtilePlanet[i].grbit != 0x80))
        goto L_5975;
    else
        goto L_595c;

L_595c:
    rgtilePlanet[i].dyFull = (rgtilePlanet[i].dyFull + LOWORD((0xa * iMul)));

L_5975:
    i = (i + 0x1);

L_5979:
    if ((i < 0x6))
        goto L_58d4;
    else
        goto L_5982;

L_5982:
    i = 0x0;
    goto L_5b09;

L_598a:
    if ((rgtileShip[i].grbit != 0x1))
        goto L_59c3;
    else
        goto L_59a3;

L_59a3:
    rgtileShip[i].dyFull = (rgtileShip[i].dyFull + LOWORD((((dyArial8 * 0x4) + 0x2) * iMul)));

L_59c3:
    if ((rgtileShip[i].grbit != 0x200))
        goto L_5a02;
    else
        goto L_59dd;

L_59dd:
    rgtileShip[i].dyFull = (rgtileShip[i].dyFull + LOWORD((((((dyArial8 + 0x2) * 0x2) + 0x4) + dyArial8) * iMul)));

L_5a02:
    if ((rgtileShip[i].grbit != 0x20))
        goto L_5a37;
    else
        goto L_5a1b;

L_5a1b:
    rgtileShip[i].dyFull = (rgtileShip[i].dyFull + LOWORD(((dyArial8 + 0x9) * iMul)));

L_5a37:
    if ((rgtileShip[i].grbit != 0x4))
        goto L_5a6e;
    else
        goto L_5a50;

L_5a50:
    rgtileShip[i].dyFull = (rgtileShip[i].dyFull + LOWORD((((dyArial8 + 0x4) * 0x2) * iMul)));

L_5a6e:
    if ((rgtileShip[i].grbit != 0x80))
        goto L_5aa1;
    else
        goto L_5a88;

L_5a88:
    rgtileShip[i].dyFull = (rgtileShip[i].dyFull + LOWORD((0xa * iMul)));

L_5aa1:
    if ((rgtileShip[i].grbit != 0x100))
        goto L_5ad3;
    else
        goto L_5abb;

L_5abb:
    rgtileShip[i].dyFull = (rgtileShip[i].dyFull + (iMul * 0x2));

L_5ad3:
    if ((rgtileShip[i].grbit != 0x40))
        goto L_5b05;
    else
        goto L_5aec;

L_5aec:
    rgtileShip[i].dyFull = (rgtileShip[i].dyFull + LOWORD((0x6 * iMul)));

L_5b05:
    i = (i + 0x1);

L_5b09:
    if ((i < 0x7))
        goto L_598a;
    else
        goto L_5b12;

L_5b12:
    grobjSav = sel.grobj;
    sel.grobj = grobjPlanet;
    i = 0x0;
    goto L_5b3d;

L_5b26:
    ReflowColumn(i, 0xffff, 0x0);
    i = (i + 0x1);

L_5b3d:
    if ((i < 0x4))
        goto L_5b26;
    else
        goto L_5b46;

L_5b46:
    sel.grobj = grobjFleet;
    i = 0x0;
    goto L_5b6b;

L_5b54:
    ReflowColumn(i, 0xffff, 0x0);
    i = (i + 0x1);

L_5b6b:
    if ((i < 0x4))
        goto L_5b54;
    else
        goto L_5b74;

L_5b74:
    sel.grobj = grobjSav;

L_5b7a:
    return;
}

void ReflowColumn(int16_t iCol, int16_t iTile, int16_t fRedraw) {
    HDC     hdc;
    int16_t yTop;
    int16_t ctile;
    int16_t i;
    int16_t grbit;
    TILE   *ptile;
    RECT    rc;
    int16_t t_merge_5c71_0001;
    int16_t t_merge_5d9f_0001;

L_5b80:
    grbit = 0x0;
    if ((sel.grobj != grobjFleet))
        goto L_5ba5;
    else
        goto L_5b98;

L_5b98:
    ptile = 0x90e;
    ctile = 0x7;
    goto L_5baf;

L_5ba5:
    ptile = 0x7fc;
    ctile = 0x6;

L_5baf:
    yTop = 0x4;
    if ((iTile != 0xffff))
        goto L_5c0c;
    else
        goto L_5bbd;

L_5bbd:
    i = 0x0;
    goto L_5bea;

L_5bc5:
    /* untranslated: branch (part[10:2](ptile[i]) & 0x7) == iCol ? L_5bf5 : L_5be0 */

L_5be0:

L_5be6:
    i = (i + 0x1);

L_5bea:
    if ((i < ctile))
        goto L_5bc5;
    else
        goto L_5bf5;

L_5bf5:
    if ((i == ctile))
        goto L_5dcb;
    else
        goto L_5bfd;

L_5bfd:

L_5c03:
    iTile = i;
    goto L_5c86;

L_5c0c:
    i = 0x0;
    goto L_5c7b;

L_5c14:
    /* untranslated: branch (part[10:2](ptile[i]) & 0x7) != iCol ? L_5c77 : L_5c32 */

L_5c32:
    /* untranslated: branch ((part[10:2](ptile[i]) >> 0x7) & 0x1) == 0x0 ? L_5c6b : L_5c55 */

L_5c55:
    t_merge_5c71_0001 = ptile[i].dyFull;
    goto L_5c71;

L_5c6b:
    t_merge_5c71_0001 = (dyArial8 + 0x3);

L_5c71:
    yTop = (yTop + (t_merge_5c71_0001 + 0x4));

L_5c77:
    i = (i + 0x1);

L_5c7b:
    if ((i < iTile))
        goto L_5c14;
    else
        goto L_5c86;

L_5c86:
    if ((fRedraw == 0x0))
        goto L_5ce0;
    else
        goto L_5c8f;

L_5c8f:
    GetClientRect(hwndPlanet, &(rc));
    rc.top = yTop;
    rc.left = (LOWORD((0xc6 * iCol)) + 0x4);
    rc.right = (rc.left + 0xbf);
    hdc = GetDC(hwndPlanet);
    FillRect(hdc, &(rc), hbrButtonFace);

L_5cdc:
    iTile = (iTile + 0x1);

L_5ce0:
    if ((iTile >= ctile))
        goto L_5da8;
    else
        goto L_5ceb;

L_5ceb:
    /* untranslated: branch (part[10:2](ptile[iTile]) & 0x7) != iCol ? L_5da8 : L_5d09 */

L_5d09:
    ptile[iTile].yTop = yTop;
    /* untranslated: part[10:2](ptile[iTile]) = ((part[10:2](ptile[iTile]) & 0xf7ff) | 0x800) */
    grbit = (grbit | ptile[iTile].grbit);
    /* untranslated: branch ((part[10:2](ptile[iTile]) >> 0x7) & 0x1) == 0x0 ? L_5d99 : L_5d83 */

L_5d83:
    t_merge_5d9f_0001 = ptile[iTile].dyFull;
    goto L_5d9f;

L_5d99:
    t_merge_5d9f_0001 = (dyArial8 + 0x3);

L_5d9f:
    yTop = (yTop + (t_merge_5d9f_0001 + 0x4));
    goto L_5cdc;

L_5da8:
    if ((fRedraw == 0x0))
        goto L_5dcb;
    else
        goto L_5db1;

L_5db1:
    DrawPlanShip(hdc, grbit);
    ReleaseDC(hwndPlanet, hdc);

L_5dcb:
    return;
}

int16_t IBestTerraform(PLANET *lppl, int16_t fHelp) {
    int16_t iSave;
    int16_t iBest;
    int16_t rgMax[3];
    int16_t pctT;
    int16_t i;
    int16_t iPlr;
    int16_t iEnv;
    int16_t pctCur;
    int16_t rgMin[3];
    int16_t rgpctBest[3];
    int16_t rgCost[3];
    int16_t iPlrSav;

L_5dd2:
    iPlrSav = idPlayer;
    iPlr = lppl->iPlayer;
    if ((iPlr != 0xffff))
        goto L_5dfa;
    else
        goto L_5df4;

L_5df4:
    return 0x0;

L_5dfa:
    idPlayer = iPlr;
    if ((FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, fHelp) != 0x0))
        goto L_5e31;
    else
        goto L_5e25;

L_5e25:
    idPlayer = iPlrSav;
    return 0x0;

L_5e31:
    pctCur = PctPlanetDesirability(lppl, iPlr);
    i = 0x0;
    goto L_5f4a;

L_5e4d:
    if ((rgMin[i] == 0xffff))
        goto L_5e71;
    else
        goto L_5e5f;

L_5e5f:
    iEnv = rgMin[i];
    goto L_5ea6;

L_5e71:
    if ((rgMax[i] == 0xffff))
        goto L_5e95;
    else
        goto L_5e83;

L_5e83:
    iEnv = rgMax[i];
    goto L_5ea6;

L_5e95:
    rgpctBest[i] = 0x0;
    goto L_5f46;

L_5ea6:
    iSave = (uint16_t)(lppl->rgEnvVar[i]);
    lppl->rgEnvVar[i] = LOBYTE(iEnv);
    pctT = (PctPlanetDesirability(lppl, iPlr) - pctCur);
    if ((pctT >= 0x0))
        goto L_5eff;
    else
        goto L_5ef7;

L_5ef7:
    /* untranslated: pctT = (pctT neg 0x0) */

L_5eff:
    /* untranslated: ss:[bp-0x2c] = loword((0x64 * pctT)) */
    /* untranslated: rgpctBest[i] = ((words(ss:[bp-0x2c], signhiword(ss:[bp-0x2c])) / abs((iSave - iEnv))) + 0x1) */
    lppl->rgEnvVar[i] = LOBYTE(iSave);

L_5f46:
    i = (i + 0x1);

L_5f4a:
    if ((i < 0x3))
        goto L_5e4d;
    else
        goto L_5f53;

L_5f53:
    iSave = 0x0;
    i = 0x1;
    goto L_5f87;

L_5f60:
    if ((rgpctBest[i] <= rgpctBest[iSave]))
        goto L_5f83;
    else
        goto L_5f7d;

L_5f7d:
    iSave = i;

L_5f83:
    i = (i + 0x1);

L_5f87:
    if ((i < 0x3))
        goto L_5f60;
    else
        goto L_5f90;

L_5f90:
    if ((rgMin[iSave] == 0xffff))
        goto L_5fb0;
    else
        goto L_5fa2;

L_5fa2:
    /* untranslated: iBest = ((iSave + 0x1) neg 0x0) */
    goto L_5fb9;

L_5fb0:
    iBest = (iSave + 0x1);

L_5fb9:
    idPlayer = iPlrSav;
    return iBest;
}

char *PszCalcEnvVar(int16_t iEnv, int16_t iVar) {
L_5fcc:
    goto L_6030;

L_5fdb:
    return PszCalcGravity(iVar);

L_5fe9:
    _wsprintf(szWork, "%d%cC", ((iVar * 0x4) + 0xff38), 0xba);
    goto L_604b;

L_6011:
    _wsprintf(szWork, "%dmR", iVar);
    goto L_604b;

L_6030:
    if ((iEnv == 0x0))
        goto L_5fdb;
    else
        goto L_6038;

L_6038:
    if ((iEnv == 0x1))
        goto L_5fe9;
    else
        goto L_6040;

L_6040:
    if ((iEnv != 0x2))
        goto L_5fdb;
    else
        goto L_6045;

L_6045:

L_604b:
    return 0x57a4;
}

char *PszCalcGravity(int16_t iGravity) {
    int16_t d;
    int16_t iVal;

L_6058:
    d = abs((iGravity + 0xffce));
    if ((d > 0x19))
        goto L_608c;
    else
        goto L_607c;

L_607c:
    iVal = ((d * 0x4) + 0x64);
    goto L_609d;

L_608c:
    iVal = (LOWORD(((d + 0xffe7) * 0x18)) + 0xc8);

L_609d:
    if ((iGravity >= 0x32))
        goto L_60b0;
    else
        goto L_60a6;

L_60a6:
    iVal = (0x27100000 / iVal);

L_60b0:
    _wsprintf(szWork, "%d.%02dg", ((uint32_t)(iVal) / 0x64), ((uint32_t)(iVal) % 0x64));

L_60e0:
    return 0x57a4;
}

void HandleFocusState(DRAWITEMSTRUCT *lpdis, int16_t inflate) {
L_60e6:
    if (((lpdis->itemState & 0x10) == 0x0))
        goto L_6121;
    else
        goto L_6101;

L_6101:
    FrameRect(lpdis->hDC, &(lpdis->rcItem), hbr50Screen);

L_6121:
    return;
}

void DrawCBEntireItem(DRAWITEMSTRUCT *lpdis, int16_t inflate) {
    int16_t  fListbox;
    int16_t  fSelected;
    RECT     rc;
    uint16_t t_merge_618b_0001;
    uint16_t t_merge_61b8_0001;

L_6128:
    fSelected = (lpdis->itemState & 0x1);
    rc = lpdis->rcItem;
    if ((lpdis->hwndItem == hwndFleetCompLB))
        goto L_6182;
    else
        goto L_616a;

L_616a:
    if ((lpdis->hwndItem == hwndPlanetProdLB))
        goto L_6182;
    else
        goto L_6179;

L_6179:
    if ((inflate <= 0x0))
        goto L_6188;
    else
        goto L_6182;

L_6182:
    t_merge_618b_0001 = 0x1;
    goto L_618b;

L_6188:
    t_merge_618b_0001 = 0x0;

L_618b:
    fListbox = t_merge_618b_0001;
    if ((inflate <= 0x0))
        goto L_619f;
    else
        goto L_6197;

L_6197:
    /* untranslated: inflate = (inflate neg 0x0) */

L_619f:
    if ((fListbox == 0x0))
        goto L_61b5;
    else
        goto L_61af;

L_61af:
    t_merge_61b8_0001 = 0x40a;
    goto L_61b8;

L_61b5:
    t_merge_61b8_0001 = 0x408;

L_61b8:
    /* untranslated: call SendMessage(lpdis->hwndItem, t_merge_61b8_0001, lpdis->itemID, words(ds, 0x57a4)) -> callresult(LRESULT) */
    DrawProductionItem(lpdis->hDC, &(rc), szWork, inflate, fSelected, fListbox);
    HandleFocusState(lpdis, (inflate + 0x2));
    return;
}

void DrawProductionItem(HDC hdc, RECT *prc, char *psz, int16_t inflate, int16_t fSelected, int16_t fListbox) {
    uint16_t hfntSav;
    char    *pch;
    int16_t  ichT;
    COLORREF cr;
    int16_t  pctDmg;
    char     szT[20];
    RECT     rcIn;
    int16_t  ich;
    int16_t  fDoubleDraw;
    COLORREF crForeSav;
    int16_t  fFleet;
    RECT     rcDraw;
    int16_t  dx;
    HBRUSH   hbr;
    int16_t  fItalic;
    int16_t  cch;
    int16_t  bkSav;
    RECT     rc;

L_6208:
    fItalic = 0x0;
    fDoubleDraw = 0x0;
    fFleet = 0x0;
    rcIn = *(prc);
    rc = rcIn;
    if ((fSelected != 0x0))
        goto L_6313;
    else
        goto L_6248;

L_6248:
    hbr = hbrWindow;
    goto L_62d5;

L_6257:
    cr = 0xff;
    goto L_63d9;

L_6264:
    fItalic = 0x1;

LDefCase:
    if ((LOWORD(crWindow) != 0x0))
        goto L_628a;
    else
        goto L_6273;

L_6273:
    if ((HIWORD(crWindow) != 0x0))
        goto L_628a;
    else
        goto L_627d;

L_627d:
    cr = 0xffffff;
    goto L_63d9;

L_628a:
    cr = 0x0;

L_6297:
    fDoubleDraw = 0x1;
    pctDmg = (uint16_t)(psz[0x1]);

L_62a6:
    fFleet = 0x1;
    goto LDefCase;

L_62ae:
    cr = 0x7f00;
    goto L_63d9;

L_62bb:
    cr = 0x7f0000;
    goto L_63d9;

L_62c8:
    cr = 0x7f7f7f;
    goto L_63d9;

L_62d5:
    if (((uint16_t)(*(psz)) == 0x20))
        goto LDefCase;
    else
        goto L_62dd;

L_62dd:
    if (((uint16_t)(*(psz)) == 0x23))
        goto L_62bb;
    else
        goto L_62e5;

L_62e5:
    if (((uint16_t)(*(psz)) == 0x26))
        goto L_62c8;
    else
        goto L_62ed;

L_62ed:
    if (((uint16_t)(*(psz)) == 0x2a))
        goto L_62ae;
    else
        goto L_62f5;

L_62f5:
    if (((uint16_t)(*(psz)) == 0x49))
        goto L_6264;
    else
        goto L_62fd;

L_62fd:
    if (((uint16_t)(*(psz)) == 0x50))
        goto L_6297;
    else
        goto L_6305;

L_6305:
    if (((uint16_t)(*(psz)) != 0x51))
        goto L_6257;
    else
        goto L_630a;

L_630a:

L_6313:
    cr = crWindow;
    goto L_639e;

L_6329:
    hbr = hbrRed;
    goto L_63d9;

L_6332:
    fItalic = 0x1;

LDefCaseSel:
    if ((LOWORD(crWindow) != 0x0))
        goto L_635a;
    else
        goto L_6341;

L_6341:
    if ((HIWORD(crWindow) != 0x0))
        goto L_635a;
    else
        goto L_634b;

L_634b:
    hbr = GetStockObject(WHITE_BRUSH);
    goto L_63d9;

L_635a:
    hbr = GetStockObject(BLACK_BRUSH);

L_6369:
    fDoubleDraw = 0x1;
    pctDmg = (uint16_t)(psz[0x1]);

L_6378:
    fFleet = 0x1;
    goto LDefCaseSel;

L_6380:
    hbr = hbrGreen;
    goto L_63d9;

L_6389:
    hbr = hbrBlue;
    goto L_63d9;

L_6392:
    hbr = hbrGray;
    goto L_63d9;

L_639e:
    if (((uint16_t)(*(psz)) == 0x20))
        goto LDefCaseSel;
    else
        goto L_63a6;

L_63a6:
    if (((uint16_t)(*(psz)) == 0x23))
        goto L_6389;
    else
        goto L_63ae;

L_63ae:
    if (((uint16_t)(*(psz)) == 0x26))
        goto L_6392;
    else
        goto L_63b6;

L_63b6:
    if (((uint16_t)(*(psz)) == 0x2a))
        goto L_6380;
    else
        goto L_63be;

L_63be:
    if (((uint16_t)(*(psz)) == 0x49))
        goto L_6332;
    else
        goto L_63c6;

L_63c6:
    if (((uint16_t)(*(psz)) == 0x50))
        goto L_6369;
    else
        goto L_63ce;

L_63ce:
    if (((uint16_t)(*(psz)) != 0x51))
        goto L_6329;
    else
        goto L_63d3;

L_63d3:

L_63d9:
    if ((fListbox != 0x2))
        goto L_63e8;
    else
        goto L_63e2;

L_63e2:
    hbr = hbrButtonFace;

L_63e8:
    FillRect(hdc, &(rcIn), hbr);
    if ((fDoubleDraw == 0x0))
        goto L_6446;
    else
        goto L_6403;

L_6403:
    rcDraw = rcIn;
    dx = (rcIn.right - rcIn.left);
    /* untranslated: dx = (words(loword((dx * pctDmg)), signhiword(loword((dx * pctDmg)))) / 0x64) */
    rcDraw.right = (rcDraw.left + dx);
    FillRect(hdc, &(rcDraw), hbrRed);

L_6446:
    if ((inflate == 0x0))
        goto L_6463;
    else
        goto L_644f;

L_644f:
    InflateRect(&(rcIn), 0xfffe, 0xffff);

L_6463:
    if ((fListbox == 0x0))
        goto L_649f;
    else
        goto L_646c;

L_646c:
    if ((fFleet == 0x0))
        goto L_647d;
    else
        goto L_6475;

L_6475:
    ich = 0x7;
    goto L_64a4;

L_647d:
    ich = 0x6;
    if (((((uint16_t)(psz[0x1]) + 0xffe0) & 0x2) == 0x0))
        goto L_64a4;
    else
        goto L_6497;

L_6497:
    fItalic = 0x1;

L_649f:
    ich = 0x1;

L_64a4:
    crForeSav = SetTextColor(hdc, cr);
    bkSav = SetBkMode(hdc, TRANSPARENT);
    if ((fItalic == 0x0))
        goto L_64df;
    else
        goto L_64d0;

L_64d0:
    hfntSav = SelectObject(hdc, rghfontArial8[0x3]);

L_64df:
    pch = (psz + ich);
    cch = (strlen(pch) + 0x1);

L_64fb:
    cch = (cch - 0x1);
    dx = LOWORD(GetTextExtent(hdc, pch, cch));
    if ((dx > (rcIn.right - rcIn.left)))
        goto L_64fb;
    else
        goto L_6522;

L_6522:
    TextOut(hdc, rcIn.left, rcIn.top, pch, cch);
    if ((fItalic == 0x0))
        goto L_654e;
    else
        goto L_6543;

L_6543:
    SelectObject(hdc, hfntSav);

L_654e:
    if ((ich < 0x6))
        goto L_6673;
    else
        goto L_6557;

L_6557:
    /* untranslated: branch ((sext8to16(byte ds:[(psz + (ich + 0xfffb))]) + 0xffe0) & 0x2) == 0x0 ? L_65b2 : L_6573 */

L_6573:
    /* untranslated: branch sext8to16(byte ds:[(psz + (ich + 0xffff))]) != 0x2a ? L_659f : L_6589 */

L_6589:
    ich = CchGetString(idsNeeded, szT);
    goto LRightOut;

L_659f:
    CchGetString(idsUpTo, szT);
    goto L_65b6;

L_65b2:
    szT[0x0] = 0x0;

L_65b6:
    ich = strlen(szT);
    ichT = (0x2 - fFleet);

L_65ce:
    if ((ichT >= 0x6))
        goto L_65f4;
    else
        goto L_65d7;

L_65d7:
    /* untranslated: branch sext8to16(byte ds:[(psz + (ichT + fFleet))]) != 0x20 ? L_65f4 : L_65ed */

L_65ed:
    ichT = (ichT + 0x1);
    goto L_65ce;

L_65f4:
    strncpy(szT[ich], &(psz[(ichT + fFleet)]), (0x6 - ichT));
    ich = (ich + (0x6 - ichT));
    if ((fFleet != 0x0))
        goto LRightOut;
    else
        goto L_662c;

L_662c:
    /* untranslated: branch ((sext8to16(part[1:1](psz[fDoubleDraw])) + 0xffe0) & 0x1) == 0x0 ? L_6657 : L_6648 */

L_6648:
    ich = (ich + 0x1);
    szT[ich] = 0x25;

LRightOut:
    RightTextOut(hdc, rcIn.right, rcIn.top, szT, ich, 0x0);

L_6673:
    SetTextColor(hdc, crForeSav);
    SetBkMode(hdc, bkSav);
    return;
}

void FillPlanetProdLB(HWND hwnd, PLPROD *lpplprod, PLANET *lppl) {
    int16_t  fMinimal;
    int32_t  rgwtMin[4];
    int16_t  i;
    int16_t  cItem;
    char     szTemp[80];
    int32_t  resCost;
    char    *psz;
    char     ch;
    PROD    *lpprod;
    int16_t  etaLast;
    int16_t  etaFirst;
    uint16_t t_merge_66b6_0001;

L_6692:
    if ((LOWORD(lppl) != 0x0))
        goto L_66ad;
    else
        goto L_66a4;

L_66a4:
    if ((HIWORD(lppl) == 0x0))
        goto L_66b3;
    else
        goto L_66ad;

L_66ad:
    t_merge_66b6_0001 = 0x1;
    goto L_66b6;

L_66b3:
    t_merge_66b6_0001 = 0x0;

L_66b6:
    fMinimal = t_merge_66b6_0001;
    if ((fMinimal != 0x0))
        goto L_66f4;
    else
        goto L_66c2;

L_66c2:
    lppl = &(sel.pl);
    if ((hwnd != 0x0))
        goto L_66dc;
    else
        goto L_66d6;

L_66d6:
    hwnd = hwndPlanetProdLB;

L_66dc:
    SendMessage(hwnd, CB_DIR, 0x0, 0x0);

L_66f4:
    if ((LOWORD(lpplprod) != 0x0))
        goto L_6717;
    else
        goto L_66fd;

L_66fd:
    if ((HIWORD(lpplprod) != 0x0))
        goto L_6717;
    else
        goto L_6706;

L_6706:
    lpplprod = lppl->lpplprod;

L_6717:
    if ((LOWORD(lpplprod) != 0x0))
        goto L_6729;
    else
        goto L_6720;

L_6720:
    if ((HIWORD(lpplprod) == 0x0))
        goto L_673b;
    else
        goto L_6729;

L_6729:
    if ((lpplprod->iprodMac != 0x0))
        goto L_674d;
    else
        goto L_673b;

L_673b:
    psz = PszGetCompressedString(idsQueueEmpty);
    goto L_676c;

L_674d:
    if ((hwndProdDlg == 0x0))
        goto NoMsg;
    else
        goto L_6757;

L_6757:
    psz = PszGetCompressedString(idsTopQueue);
    goto L_676c;

L_676c:
    if ((fMinimal != 0x0))
        goto L_678f;
    else
        goto L_6775;

L_6775:
    /* untranslated: call SendMessage(hwnd, CB_LIMITTEXT, 0x0, words(ds, psz)) -> callresult(LRESULT) */
    goto NoMsg;

L_678f:
    if ((psz == 0x57a4))
        goto NoMsg;
    else
        goto L_6799;

L_6799:
    strcpy(szWork, psz);

NoMsg:
    if ((LOWORD(lpplprod) != 0x0))
        goto L_67bd;
    else
        goto L_67b1;

L_67b1:
    if ((HIWORD(lpplprod) == 0x0))
        goto L_6ac4;
    else
        goto L_67b7;

L_67b7:

L_67bd:
    resCost = 0x0;
    i = 0x0;
    goto L_67e8;

L_67cf:
    rgwtMin[i] = 0x0;
    i = (i + 0x1);

L_67e8:
    if ((i < 0x4))
        goto L_67cf;
    else
        goto L_67f1;

L_67f1:
    i = 0x0;
    lpprod = lpplprod->rgprod;
    goto L_681f;

L_680c:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_681f:
    if ((i >= lpplprod->iprodMac))
        goto L_6aab;
    else
        goto L_6831;

L_6831:
    psz = PszNameProdItem(lpprod);
    EstimateItemProdSched(lppl, lpplprod, i, &(etaFirst), &(etaLast));
    if ((etaFirst != 0x0))
        goto L_6873;
    else
        goto L_686a;

L_686a:
    if ((etaLast == 0x0))
        goto L_6885;
    else
        goto L_6873;

L_6873:
    if ((etaFirst != 0xffff))
        goto L_6898;
    else
        goto L_687c;

L_687c:
    if ((etaLast != 0xffff))
        goto L_6898;
    else
        goto L_6885;

L_6885:
    if ((fMinimal != 0x0))
        goto L_680c;
    else
        goto L_688b;

L_688b:

L_6891:
    ch = 0x26;
    goto L_693c;

L_6898:
    if ((etaFirst <= 0x1))
        goto L_68aa;
    else
        goto L_68a1;

L_68a1:
    if ((etaFirst < 0x64))
        goto L_6908;
    else
        goto L_68aa;

L_68aa:
    if ((etaFirst != 0x64))
        goto L_690f;
    else
        goto L_68b3;

L_68b3:
    if ((lpprod->grobj != 0x1))
        goto L_690f;
    else
        goto L_68d3;

L_68d3:
    if ((0x0 != 0x0))
        goto L_690f;
    else
        goto L_68db;

L_68db:
    if ((0x0 > 0x0))
        goto L_690f;
    else
        goto L_68fb;

L_68fb:
    if ((0x0 < 0x0))
        goto L_6908;
    else
        goto L_6900;

L_6900:
    if ((lpprod->iItem >= 0x7))
        goto L_690f;
    else
        goto L_6908;

L_6908:
    ch = 0x20;
    goto L_693c;

L_690f:
    if ((etaFirst != 0x1))
        goto L_6928;
    else
        goto L_6918;

L_6918:
    if ((etaLast != 0x1))
        goto L_6928;
    else
        goto L_6921;

L_6921:
    ch = 0x2a;
    goto L_693c;

L_6928:
    if ((etaFirst >= 0x64))
        goto L_6938;
    else
        goto L_6931;

L_6931:
    ch = 0x23;
    goto L_693c;

L_6938:
    ch = 0x21;

L_693c:
    cItem = lpprod->cItem;
    _wsprintf(szTemp, "%c%5d%s", (uint16_t)(ch), cItem, psz);
    if ((lpprod->grobj != 0x1))
        goto L_6a75;
    else
        goto L_6994;

L_6994:
    if ((0x0 != 0x0))
        goto L_6a75;
    else
        goto L_699c;

L_699c:
    if ((0x0 > 0x0))
        goto L_69f9;
    else
        goto L_69bc;

L_69bc:
    if ((0x0 < 0x0))
        goto L_69c9;
    else
        goto L_69c1;

L_69c1:
    if ((lpprod->iItem >= 0x7))
        goto L_69f9;
    else
        goto L_69c9;

L_69c9:
    szTemp[0x1] = (szTemp[0x1] + 0x2);
    if ((lpprod->iItem != 0x3))
        goto L_69f9;
    else
        goto L_69ed;

L_69ed:
    if ((0x0 != 0x0))
        goto L_69f9;
    else
        goto L_69f5;

L_69f5:
    szTemp[0x5] = 0x2a;

L_69f9:
    if ((lpprod->iItem != 0xc))
        goto L_6a21;
    else
        goto L_6a19;

L_6a19:
    if ((0x0 == 0x0))
        goto L_6a71;
    else
        goto L_6a21;

L_6a21:
    if ((lpprod->iItem != 0x4))
        goto L_6a49;
    else
        goto L_6a41;

L_6a41:
    if ((0x0 == 0x0))
        goto L_6a71;
    else
        goto L_6a49;

L_6a49:
    if ((lpprod->iItem != 0x5))
        goto L_6a75;
    else
        goto L_6a69;

L_6a69:
    if ((0x0 != 0x0))
        goto L_6a75;
    else
        goto L_6a71;

L_6a71:
    szTemp[0x1] = (szTemp[0x1] + 0x1);

L_6a75:
    if ((fMinimal == 0x0))
        goto L_6a91;
    else
        goto L_6a7e;

L_6a7e:
    strcpy(szWork, szTemp);
    goto L_6ac4;

L_6a91:
    /* untranslated: call SendMessage(hwnd, CB_LIMITTEXT, 0x0, words(ss, szTemp)) -> callresult(LRESULT) */
    goto L_680c;

L_6aab:
    if ((fMinimal == 0x0))
        goto L_6ac4;
    else
        goto L_6ab4;

L_6ab4:
    CchGetString(idsQueueEmpty, szWork);

L_6ac4:
    return;
}

int16_t PctPlanetCapacity(PLANET *lppl) {
    int32_t pctCap;
    int32_t lPopMax;

L_6aca:
    lPopMax = CalcPlanetMaxPop(lppl->id, idPlayer);
    if ((HIWORD(lPopMax) > 0x0))
        goto L_6b08;
    else
        goto L_6af4;

L_6af4:
    if ((HIWORD(lPopMax) < 0x0))
        goto L_6b02;
    else
        goto L_6af9;

L_6af9:
    if ((LOWORD(lPopMax) > 0x0))
        goto L_6b08;
    else
        goto L_6b02;

L_6b02:
    return 0x0;

L_6b08:
    /* untranslated: pctCap = (int32_t)(words((hiword((uint32_t)(lppl->rgwtMin[0x3] * 0x64)) + hiword((int32_t)(lPopMax / 0x2))),
     * (loword((uint32_t)(lppl->rgwtMin[0x3] * 0x64)) + loword((int32_t)(lPopMax / 0x2)))) / lPopMax) */
    if ((HIWORD(pctCap) < 0x0))
        goto L_6b78;
    else
        goto L_6b5f;

L_6b5f:
    if ((HIWORD(pctCap) > 0x0))
        goto L_6b6e;
    else
        goto L_6b64;

L_6b64:
    if ((LOWORD(pctCap) < 0x3e8))
        goto L_6b78;
    else
        goto L_6b6e;

L_6b6e:
    pctCap = 0x3e7;

L_6b78:
    return LOWORD(pctCap);
}

int16_t PctPlanetOptValue(PLANET *lppl, int16_t iPlr) {
    int16_t  rgMax[3];
    int16_t  i;
    int16_t  rgMin[3];
    int16_t  pctDesire;
    int16_t  rgCost[3];
    int16_t  rgiValSav[3];
    int16_t  iNewVal;
    uint16_t t_merge_6d07_0001;
    uint16_t t_merge_6d96_0001;

L_6b88:
    if ((FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, 0x1) != 0x0))
        goto L_6bcb;
    else
        goto L_6bb7;

L_6bb7:
    return PctPlanetDesirability(lppl, iPlr);

L_6bcb:
    i = 0x0;
    goto L_6dbe;

L_6bd3:
    rgiValSav[i] = (uint16_t)(lppl->rgEnvVar[i]);
    if (((uint16_t)(rgplr[iPlr].rgEnvVarMin[i]) == 0xffff))
        goto L_6dba;
    else
        goto L_6c15;

L_6c15:
    if (((uint16_t)(lppl->rgEnvVar[i]) == (uint16_t)(rgplr[iPlr].rgEnvVar[i])))
        goto L_6dba;
    else
        goto L_6c50;

L_6c50:
    iNewVal = 0xffff;
    if (((uint16_t)(lppl->rgEnvVar[i]) >= (uint16_t)(rgplr[iPlr].rgEnvVar[i])))
        goto L_6d0d;
    else
        goto L_6c90;

L_6c90:
    if ((rgMax[i] <= (uint16_t)(lppl->rgEnvVar[i])))
        goto L_6d99;
    else
        goto L_6cb7;

L_6cb7:
    if (((uint16_t)(rgplr[iPlr].rgEnvVar[i]) >= rgMax[i]))
        goto L_6cfb;
    else
        goto L_6ce0;

L_6ce0:
    t_merge_6d07_0001 = (uint16_t)(rgplr[iPlr].rgEnvVar[i]);
    goto L_6d07;

L_6cfb:
    t_merge_6d07_0001 = rgMax[i];

L_6d07:
    iNewVal = t_merge_6d07_0001;

L_6d0d:
    if ((rgMin[i] == 0xffff))
        goto L_6d99;
    else
        goto L_6d1f;

L_6d1f:
    if ((rgMin[i] >= (uint16_t)(lppl->rgEnvVar[i])))
        goto L_6d99;
    else
        goto L_6d46;

L_6d46:
    if (((uint16_t)(rgplr[iPlr].rgEnvVar[i]) <= rgMin[i]))
        goto L_6d8a;
    else
        goto L_6d6f;

L_6d6f:
    t_merge_6d96_0001 = (uint16_t)(rgplr[iPlr].rgEnvVar[i]);
    goto L_6d96;

L_6d8a:
    t_merge_6d96_0001 = rgMin[i];

L_6d96:
    iNewVal = t_merge_6d96_0001;

L_6d99:
    if ((iNewVal == 0xffff))
        goto L_6dba;
    else
        goto L_6da2;

L_6da2:
    lppl->rgEnvVar[i] = LOBYTE(iNewVal);

L_6dba:
    i = (i + 0x1);

L_6dbe:
    if ((i < 0x3))
        goto L_6bd3;
    else
        goto L_6dc7;

L_6dc7:
    pctDesire = PctPlanetDesirability(lppl, idPlayer);
    i = 0x0;
    goto L_6e09;

L_6de4:
    lppl->rgEnvVar[i] = LOBYTE(rgiValSav[i]);
    i = (i + 0x1);

L_6e09:
    if ((i < 0x3))
        goto L_6de4;
    else
        goto L_6e12;

L_6e12:
    return pctDesire;
}

int16_t PctPlanetDesirability(PLANET *lppl, int16_t iPlr) {
    int16_t  iMin;
    int16_t  d;
    int16_t  iMax;
    int32_t  pctNeg;
    int16_t  iPref;
    int16_t  i;
    int16_t  dPenalty;
    int32_t  pctPos;
    int16_t  pctVar;
    int16_t  iPlanet;
    int32_t  pctMod;
    uint32_t t_merge_6fd3_0001_wide;
    uint32_t t_merge_6ffa_0001_wide;

L_6e1e:
    pctPos = 0x0;
    pctNeg = 0x0;
    pctMod = 0x2710;
    i = 0x0;
    goto L_7004;

L_6e4d:
    iPlanet = (uint16_t)(lppl->rgEnvVar[i]);
    iPref = (uint16_t)(rgplr[iPlr].rgEnvVar[i]);
    iMin = (uint16_t)(rgplr[iPlr].rgEnvVarMin[i]);
    iMax = (uint16_t)(rgplr[iPlr].rgEnvVarMax[i]);
    if ((iMax >= 0x0))
        goto L_6ecc;
    else
        goto L_6ec0;

L_6ec0:
    pctPos = (pctPos + 0x2710);
    goto L_7000;

L_6ecc:
    if ((iPlanet < iMin))
        goto L_6faa;
    else
        goto L_6ed7;

L_6ed7:
    if ((iPlanet > iMax))
        goto L_6faa;
    else
        goto L_6ee2;

L_6ee2:
    pctVar = LOWORD((abs((iPlanet - iPref)) * 0x64));
    if ((iPlanet >= iPref))
        goto L_6f28;
    else
        goto L_6f04;

L_6f04:
    d = (iPref - iMin);
    pctVar = ((uint32_t)(pctVar) / d);
    dPenalty = (((iPref - iPlanet) * 0x2) - d);
    goto L_6f49;

L_6f28:
    d = (iMax - iPref);
    pctVar = ((uint32_t)(pctVar) / d);
    dPenalty = (((iPlanet - iPref) * 0x2) - d);

L_6f49:
    pctVar = (0x64 - pctVar);
    pctPos = (pctPos + (uint32_t)(((uint32_t)(pctVar) * (uint32_t)(pctVar))));
    if ((dPenalty <= 0x0))
        goto L_7000;
    else
        goto L_6f72;

L_6f72:
    pctMod = (uint32_t)((pctMod * (uint32_t)(((d * 0x2) - dPenalty))));
    pctMod = (int32_t)((pctMod / (uint32_t)((d * 0x2))));

L_6faa:
    if ((iPlanet >= iMin))
        goto L_6fdc;
    else
        goto L_6fb5;

L_6fb5:
    if ((0xf >= (iMin - iPlanet)))
        goto L_6fcc;
    else
        goto L_6fc5;

L_6fc5:
    t_merge_6fd3_0001_wide = 0xf;
    goto L_6fd3;

L_6fcc:
    t_merge_6fd3_0001_wide = (uint32_t)((iMin - iPlanet));

L_6fd3:
    pctNeg = (pctNeg + t_merge_6fd3_0001_wide);
    goto L_7000;

L_6fdc:
    if ((0xf >= (iPlanet - iMax)))
        goto L_6ff3;
    else
        goto L_6fec;

L_6fec:
    t_merge_6ffa_0001_wide = 0xf;
    goto L_6ffa;

L_6ff3:
    t_merge_6ffa_0001_wide = (uint32_t)((iPlanet - iMax));

L_6ffa:
    pctNeg = (pctNeg + t_merge_6ffa_0001_wide);

L_7000:
    i = (i + 0x1);

L_7004:
    if ((i < 0x3))
        goto L_6e4d;
    else
        goto L_700d;

L_700d:
    if ((LOWORD(pctNeg) != 0x0))
        goto L_701f;
    else
        goto L_7016;

L_7016:
    if ((HIWORD(pctNeg) == 0x0))
        goto L_702a;
    else
        goto L_701f;

L_701f:
    /* untranslated: return (LOWORD(pctNeg) neg 0x0) */

L_702a:
    sqrt(((double)(pctPos) / 3));
    pctPos = __ftol();
    pctPos = (int32_t)(((uint32_t)((pctPos * pctMod)) / 0x2710));
    return LOWORD(pctPos);
}

int32_t CalcPlanetMaxPop(int16_t idpl, int16_t iplr) {
    PLANET  pl;
    int32_t lMaxPop;
    int32_t pctDesire;
    int16_t ihuldef;

L_7096:
    FLookupPlanet(idpl, &(pl));
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raMacintosh))
        goto L_7134;
    else
        goto L_70ce;

L_70ce:
    if ((pl.iPlayer != iplr))
        goto L_70ec;
    else
        goto L_70d9;

L_70d9:
    if ((pl.fStarbase != 0x0))
        goto L_70f5;
    else
        goto L_70ec;

L_70ec:
    return 0x0;

L_70f5:
    ihuldef = (rglpshdefSB[iplr][pl.isb].hul.ihuldef + 0xffe0);
    lMaxPop = rglPopMac[ihuldef];
    goto L_71ff;

L_7134:
    pctDesire = (uint32_t)(PctPlanetDesirability(&(pl), iplr));
    if ((HIWORD(pctDesire) > 0x0))
        goto L_7171;
    else
        goto L_7156;

L_7156:
    if ((HIWORD(pctDesire) < 0x0))
        goto L_7164;
    else
        goto L_715b;

L_715b:
    if ((LOWORD(pctDesire) >= 0x5))
        goto L_7171;
    else
        goto L_7164;

L_7164:
    lMaxPop = 0x1f4;
    goto L_718a;

L_7171:
    lMaxPop = (uint32_t)((pctDesire * 0x64));

L_718a:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raCheapCol))
        goto L_71c6;
    else
        goto L_71aa;

L_71aa:
    lMaxPop = (lMaxPop - (int32_t)((lMaxPop / 0x2)));
    goto L_71ff;

L_71c6:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raNone))
        goto L_71ff;
    else
        goto L_71e6;

L_71e6:
    lMaxPop = (lMaxPop + (int32_t)((lMaxPop / 0x5)));

L_71ff:
    if ((GetRaceGrbit(rgplr[iplr], ibitRaceOBRM) == 0x0))
        goto L_7238;
    else
        goto L_721f;

L_721f:
    lMaxPop = (lMaxPop + (int32_t)((lMaxPop / 0xa)));

L_7238:
    return lMaxPop;
}

int16_t CMaxMines(PLANET *lppl, int16_t iplr) {
    int32_t cMax;
    int32_t lPopMax;
    int16_t iEff;

L_7248:
    lPopMax = CalcPlanetMaxPop(lppl->id, iplr);
    iEff = GetRaceStat(rgplr[iplr], rsMineOperate);
    cMax = (int32_t)(((uint32_t)((lPopMax * (uint32_t)(iEff))) / 0x64));
    if ((HIWORD(cMax) > 0x0))
        goto L_72ca;
    else
        goto L_72b2;

L_72b2:
    if ((HIWORD(cMax) < 0x0))
        goto L_72c0;
    else
        goto L_72b7;

L_72b7:
    if ((LOWORD(cMax) >= 0xa))
        goto L_72ca;
    else
        goto L_72c0;

L_72c0:
    cMax = 0xa;

L_72ca:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raMacintosh))
        goto L_72f4;
    else
        goto L_72ea;

L_72ea:
    cMax = 0x0;

L_72f4:

L_72fd:
    return LOWORD(cMax);
}

int16_t CMaxOperableMines(PLANET *lppl, int16_t iplr, int16_t fNextYear) {
    int16_t cMax;
    int32_t cCur;
    int32_t lPop;
    int16_t iEff;
    int16_t t_merge_73ba_0001;

L_7304:
    cMax = CMaxMines(lppl, iplr);
    iEff = GetRaceStat(rgplr[iplr], rsMineOperate);
    lPop = lppl->rgwtMin[0x3];
    if ((fNextYear == 0x0))
        goto L_736e;
    else
        goto L_7356;

L_7356:
    lPop = (lPop + ChgPopFromPlanet(lppl, 0x0));

L_736e:
    cCur = (int32_t)(((uint32_t)((lPop * (uint32_t)(iEff))) / 0x64));
    if ((SIGNHIWORD(cMax) > HIWORD(cCur)))
        goto L_73b4;
    else
        goto L_73a0;

L_73a0:
    if ((SIGNHIWORD(cMax) < HIWORD(cCur)))
        goto L_73ad;
    else
        goto L_73a5;

L_73a5:
    if ((cMax >= LOWORD(cCur)))
        goto L_73b4;
    else
        goto L_73ad;

L_73ad:
    t_merge_73ba_0001 = cMax;
    goto L_73ba;

L_73b4:
    t_merge_73ba_0001 = LOWORD(cCur);

L_73ba:
    cMax = t_merge_73ba_0001;
    if ((cMax > 0x0))
        goto L_73cb;
    else
        goto L_73c6;

L_73c6:
    cMax = 0x1;

L_73cb:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raMacintosh))
        goto L_73f0;
    else
        goto L_73eb;

L_73eb:
    cMax = 0x0;

L_73f0:

L_73f6:
    return cMax;
}

int16_t CMinesOperating(PLANET *lppl) {
    int16_t iplr;
    int16_t cMinesOp;
    int16_t cMines;

L_73fc:
    iplr = lppl->iPlayer;
    if ((iplr != 0xffff))
        goto L_741e;
    else
        goto L_7418;

L_7418:
    return 0x0;

L_741e:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raMacintosh))
        goto L_7469;
    else
        goto L_743e;

L_743e:
    sqrt((double)(lppl->rgwtMin[0x3]));
    return LOWORD(__ftol());

L_7469:
    /* untranslated: cMines = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff) */
    cMinesOp = CMaxOperableMines(lppl, lppl->iPlayer, 0x0);
    if ((cMines <= cMinesOp))
        goto L_74b2;
    else
        goto L_74ac;

L_74ac:
    cMines = cMinesOp;

L_74b2:
    return cMines;
}

int16_t CFactoriesOperating(PLANET *lppl) {
    int16_t iplr;
    int16_t cFacts;
    int16_t cFactsOp;

L_74be:
    iplr = lppl->iPlayer;
    if ((iplr != 0xffff))
        goto L_74e0;
    else
        goto L_74da;

L_74da:
    return 0x0;

L_74e0:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raMacintosh))
        goto L_7506;
    else
        goto L_7500;

L_7500:
    return 0x0;

L_7506:
    /* untranslated: cFacts = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff) */
    cFactsOp = CMaxOperableFactories(lppl, lppl->iPlayer, 0x0);
    if ((cFacts <= cFactsOp))
        goto L_754f;
    else
        goto L_7549;

L_7549:
    cFacts = cFactsOp;

L_754f:
    return cFacts;
}

int16_t CMaxFactories(PLANET *lppl, int16_t iplr) {
    int32_t cMax;
    int32_t lPopMax;
    int16_t iEff;

L_755c:
    lPopMax = CalcPlanetMaxPop(lppl->id, iplr);
    iEff = GetRaceStat(rgplr[iplr], rsFactOperate);
    cMax = (int32_t)(((uint32_t)((lPopMax * (uint32_t)(iEff))) / 0x64));
    if ((HIWORD(cMax) > 0x0))
        goto L_75de;
    else
        goto L_75c6;

L_75c6:
    if ((HIWORD(cMax) < 0x0))
        goto L_75d4;
    else
        goto L_75cb;

L_75cb:
    if ((LOWORD(cMax) >= 0xa))
        goto L_75de;
    else
        goto L_75d4;

L_75d4:
    cMax = 0xa;

L_75de:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raMacintosh))
        goto L_7608;
    else
        goto L_75fe;

L_75fe:
    cMax = 0x0;

L_7608:

L_7611:
    return LOWORD(cMax);
}

int16_t CMaxOperableFactories(PLANET *lppl, int16_t iplr, int16_t fNextYear) {
    int16_t cMax;
    int32_t cCur;
    int32_t lPop;
    int16_t iEff;
    int16_t t_merge_76ce_0001;

L_7618:
    cMax = CMaxFactories(lppl, iplr);
    iEff = GetRaceStat(rgplr[iplr], rsFactOperate);
    lPop = lppl->rgwtMin[0x3];
    if ((fNextYear == 0x0))
        goto L_7682;
    else
        goto L_766a;

L_766a:
    lPop = (lPop + ChgPopFromPlanet(lppl, 0x0));

L_7682:
    cCur = (int32_t)(((uint32_t)((lPop * (uint32_t)(iEff))) / 0x64));
    if ((SIGNHIWORD(cMax) > HIWORD(cCur)))
        goto L_76c8;
    else
        goto L_76b4;

L_76b4:
    if ((SIGNHIWORD(cMax) < HIWORD(cCur)))
        goto L_76c1;
    else
        goto L_76b9;

L_76b9:
    if ((cMax >= LOWORD(cCur)))
        goto L_76c8;
    else
        goto L_76c1;

L_76c1:
    t_merge_76ce_0001 = cMax;
    goto L_76ce;

L_76c8:
    t_merge_76ce_0001 = LOWORD(cCur);

L_76ce:
    cMax = t_merge_76ce_0001;
    if ((cMax > 0x0))
        goto L_76df;
    else
        goto L_76da;

L_76da:
    cMax = 0x1;

L_76df:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raMacintosh))
        goto L_7704;
    else
        goto L_76ff;

L_76ff:
    cMax = 0x0;

L_7704:

L_770a:
    return cMax;
}

int16_t CMaxDefenses(PLANET *lppl, int16_t iplr) {
    int16_t  cMax;
    int16_t  pctDesire;
    uint16_t t_merge_774b_0001;
    uint16_t t_merge_7779_0001;

L_7710:
    pctDesire = PctPlanetDesirability(lppl, iplr);
    if ((0xa <= (pctDesire * 0x4)))
        goto L_7744;
    else
        goto L_773e;

L_773e:
    t_merge_774b_0001 = 0xa;
    goto L_774b;

L_7744:
    t_merge_774b_0001 = (pctDesire * 0x4);

L_774b:
    if ((0x64 >= t_merge_774b_0001))
        goto L_775b;
    else
        goto L_7755;

L_7755:
    t_merge_7779_0001 = 0x64;
    goto L_7779;

L_775b:
    if ((0xa <= (pctDesire * 0x4)))
        goto L_7772;
    else
        goto L_776c;

L_776c:
    t_merge_7779_0001 = 0xa;
    goto L_7779;

L_7772:
    t_merge_7779_0001 = (pctDesire * 0x4);

L_7779:
    cMax = t_merge_7779_0001;
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raMacintosh))
        goto L_77a1;
    else
        goto L_779c;

L_779c:
    cMax = 0x0;

L_77a1:

L_77a7:
    return cMax;
}

int16_t CMaxOperableDefenses(PLANET *lppl, int16_t iplr, int16_t fNextYear) {
    int16_t cMax;
    int32_t cCur;
    int32_t lPop;
    int16_t t_merge_785a_0001;

L_77ae:
    cMax = CMaxDefenses(lppl, iplr);
    lPop = lppl->rgwtMin[0x3];
    if ((fNextYear == 0x0))
        goto L_77fd;
    else
        goto L_77e5;

L_77e5:
    lPop = (lPop + ChgPopFromPlanet(lppl, 0x0));

L_77fd:
    /* untranslated: cCur = (int32_t)(words((HIWORD(lPop) + 0x0), (LOWORD(lPop) + 0x18)) / 0x19) */
    if ((HIWORD(cCur) < 0x0))
        goto L_7840;
    else
        goto L_7827;

L_7827:
    if ((HIWORD(cCur) > 0x0))
        goto L_7836;
    else
        goto L_782c;

L_782c:
    if ((LOWORD(cCur) <= 0x3e8))
        goto L_7840;
    else
        goto L_7836;

L_7836:
    cCur = 0x3e8;

L_7840:
    if ((cMax >= LOWORD(cCur)))
        goto L_7854;
    else
        goto L_784e;

L_784e:
    t_merge_785a_0001 = cMax;
    goto L_785a;

L_7854:
    t_merge_785a_0001 = LOWORD(cCur);

L_785a:
    cMax = t_merge_785a_0001;
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raMacintosh))
        goto L_7882;
    else
        goto L_787d;

L_787d:
    cMax = 0x0;

L_7882:

L_7888:
    return cMax;
}

int16_t CResourcesAtPlanet(PLANET *lppl, int16_t iplr) {
    int16_t cRes;
    int32_t lPop;
    int16_t cFact;
    int32_t lPopMax;
    int16_t iEff;
    int16_t pctVal;
    int16_t iEnergy;

L_788e:
    if ((LOWORD(lppl->rgwtMin[0x3]) != 0x0))
        goto L_78b4;
    else
        goto L_78a4;

L_78a4:
    if ((HIWORD(lppl->rgwtMin[0x3]) != 0x0))
        goto L_78b4;
    else
        goto L_78ae;

L_78ae:
    return 0x0;

L_78b4:
    iEff = GetRaceStat(rgplr[iplr], rsResGen);
    lPop = lppl->rgwtMin[0x3];
    lPopMax = CalcPlanetMaxPop(lppl->id, iplr);
    if ((HIWORD(lPop) < HIWORD(lPopMax)))
        goto L_7970;
    else
        goto L_7905;

L_7905:
    if ((HIWORD(lPop) > HIWORD(lPopMax)))
        goto L_7912;
    else
        goto L_790a;

L_790a:
    if ((LOWORD(lPop) <= LOWORD(lPopMax)))
        goto L_7970;
    else
        goto L_7912;

L_7912:
    /* untranslated: lPop = ((int32_t)(words((HIWORD(lPop) - HIWORD(lPopMax)), (LOWORD(lPop) - LOWORD(lPopMax))) / 0x2) + lPopMax) */
    if ((HIWORD(lPop) < HIWORD((int32_t)((lPopMax * 0x2)))))
        goto L_7970;
    else
        goto L_794f;

L_794f:
    if ((HIWORD(lPop) > HIWORD((int32_t)((lPopMax * 0x2)))))
        goto L_795c;
    else
        goto L_7954;

L_7954:
    if ((LOWORD(lPop) <= LOWORD((int32_t)((lPopMax * 0x2)))))
        goto L_7970;
    else
        goto L_795c;

L_795c:
    lPop = (int32_t)((lPopMax * 0x2));

L_7970:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raMacintosh))
        goto L_7a48;
    else
        goto L_7990;

L_7990:
    iEnergy = (uint16_t)(rgplr[iplr].rgTech[0x0]);
    pctVal = PctPlanetDesirability(lppl, iplr);
    if ((iEnergy >= 0x1))
        goto L_79c4;
    else
        goto L_79bf;

L_79bf:
    iEnergy = 0x1;

L_79c4:
    if ((pctVal >= 0x19))
        goto L_79d2;
    else
        goto L_79cd;

L_79cd:
    pctVal = 0x19;

L_79d2:
    sqrt((((double)(lPop) * (double)((int32_t)(iEnergy))) / (double)((int32_t)(iEff))));
    /* untranslated: ss:[bp-0x2c] = pctVal */
    /* untranslated: ss:[bp-0x2a] = signhiword(pctVal) */
    /* untranslated: ss:[bp-0x34] = 0xa */
    /* untranslated: ss:[bp-0x32] = 0x0 */
    cRes = LOWORD(__ftol());
    goto LFinishUp;

L_7a48:
    cRes = LOWORD((int32_t)((lPop / (uint32_t)(iEff))));
    cFact = CMaxOperableFactories(lppl, iplr, 0x0);
    /* untranslated: branch (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff) >= cFact ? L_7ab1 : L_7a95 */

L_7a95:
    /* untranslated: cFact = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff) */

L_7ab1:
    iEff = GetRaceStat(rgplr[iplr], rsFactProd);
    /* untranslated: cRes = (cRes + loword((int32_t)(words((hiword((uint32_t)(sext16to32(cFact) * sext16to32(iEff))) + 0x0),
     * (loword((uint32_t)(sext16to32(cFact) * sext16to32(iEff))) + 0x9)) / 0xa))) */

LFinishUp:
    if ((cRes != 0x0))
        goto L_7b03;
    else
        goto L_7afe;

L_7afe:
    cRes = 0x1;

L_7b03:
    return cRes;
}

int16_t IWarpMAFromLppl(PLANET *lppl, int16_t *pfTwo) {
    int16_t fTwo;
    int16_t iWarp;
    int16_t i;
    HUL    *lphul;
    int16_t iNew;

L_7b10:
    iWarp = 0x0;
    fTwo = 0x0;
    if ((pfTwo == 0x0))
        goto L_7b33;
    else
        goto L_7b2c;

L_7b2c:
    *(pfTwo) = 0x0;

L_7b33:
    if ((lppl->iPlayer == 0xffff))
        goto L_7b57;
    else
        goto L_7b40;

L_7b40:
    if ((lppl->fStarbase != 0x0))
        goto L_7b5d;
    else
        goto L_7b57;

L_7b57:
    return 0x0;

L_7b5d:
    if ((lppl->iPlayer == idPlayer))
        goto L_7bb1;
    else
        goto L_7b6c;

L_7b6c:
    if ((idPlayer == 0xffff))
        goto L_7bb1;
    else
        goto L_7b76;

L_7b76:
    if (((rglpshdefSB[lppl->iPlayer][lppl->isb].wFlags & 0xff) == 0x7))
        goto L_7bb1;
    else
        goto L_7bab;

L_7bab:
    return 0x0;

L_7bb1:
    lphul = &(rglpshdefSB[lppl->iPlayer][lppl->isb]);
    i = 0x0;
    goto L_7be7;

L_7be3:
    i = (i + 0x1);

L_7be7:
    if ((i >= lphul->chs))
        goto L_7cdc;
    else
        goto L_7bf9;

L_7bf9:
    if ((lphul->rghs[i].grhst != hstSpecialSB))
        goto L_7be3;
    else
        goto L_7c19;

L_7c19:
    /* untranslated: branch ((lphul->rgTech[part[12:0](rgcrPlrHistory[i])] >> 0x8) & 0xff) <= 0x0 ? L_7be3 : L_7c43 */

L_7c43:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[i])] & 0xff) < 0x7 ? L_7be3 : L_7c68 */

L_7c68:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[i])] & 0xff) > 0xf ? L_7be3 : L_7c8d */

L_7c8d:
    /* untranslated: iNew = ((lphul->rgTech[part[12:0](rgcrPlrHistory[i])] & 0xff) + 0xfffe) */
    if ((iNew <= iWarp))
        goto L_7cc9;
    else
        goto L_7cbb;

L_7cbb:
    fTwo = 0x0;
    iWarp = iNew;
    goto L_7be3;

L_7cc9:
    if ((iNew != iWarp))
        goto L_7be3;
    else
        goto L_7cd4;

L_7cd4:
    fTwo = 0x1;

L_7cdc:
    if ((pfTwo == 0x0))
        goto L_7ced;
    else
        goto L_7ce5;

L_7ce5:
    *(pfTwo) = fTwo;

L_7ced:
    return iWarp;
}

int16_t StargateRangeFromLppl(PLANET *lppl, int16_t iplr, int16_t ish) {
    int16_t i;
    HUL    *lphul;
    PART    part;

L_7cfa:
    if ((LOWORD(lppl) != 0x0))
        goto L_7d15;
    else
        goto L_7d0c;

L_7d0c:
    if ((HIWORD(lppl) == 0x0))
        goto L_7d6c;
    else
        goto L_7d15;

L_7d15:
    if ((lppl->iPlayer == 0xffff))
        goto L_7d39;
    else
        goto L_7d22;

L_7d22:
    if ((lppl->fStarbase != 0x0))
        goto L_7d3f;
    else
        goto L_7d39;

L_7d39:
    return 0x0;

L_7d3f:
    lphul = &(rglpshdefSB[lppl->iPlayer][lppl->isb]);
    goto L_7d89;

L_7d6c:
    lphul = &(rglpshdefSB[iplr][ish]);

L_7d89:
    i = 0x0;
    goto L_7d95;

L_7d91:
    i = (i + 0x1);

L_7d95:
    if ((i >= lphul->chs))
        goto L_7e8d;
    else
        goto L_7da7;

L_7da7:
    if ((lphul->rghs[i].grhst != hstSpecialSB))
        goto L_7d91;
    else
        goto L_7dc7;

L_7dc7:
    /* untranslated: branch ((lphul->rgTech[part[12:0](rgcrPlrHistory[i])] >> 0x8) & 0xff) <= 0x0 ? L_7d91 : L_7df1 */

L_7df1:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[i])] & 0xff) < 0x0 ? L_7d91 : L_7e16 */

L_7e16:
    /* untranslated: branch (lphul->rgTech[part[12:0](rgcrPlrHistory[i])] & 0xff) > 0x6 ? L_7d91 : L_7e3b */

L_7e3b:
    part.hs.grhst = lphul->rghs[i].grhst;
    /* untranslated: HIWORD(part.hs) = lphul->rgTech[part[12:0](rgcrPlrHistory[i])] */
    FLookupPart(&(part));
    /* untranslated: branch part[54:2](part.pcom) != 0xffff ? L_7e80 : L_7e77 */

L_7e77:
    return 0x2710;

L_7e80:
    /* untranslated: return part[54:2](part.pcom) */

L_7e8d:
    return 0x0;
}

int16_t FProdIsTerra(PROD *lpprod) {
L_7e9a:
    if ((lpprod->grobj != 0x1))
        goto L_7f49;
    else
        goto L_7ec3;

L_7ec3:
    if ((0x0 != 0x0))
        goto L_7f49;
    else
        goto L_7ecb;

L_7ecb:
    if ((lpprod->iItem != 0xc))
        goto L_7ef3;
    else
        goto L_7eeb;

L_7eeb:
    if ((0x0 == 0x0))
        goto L_7f43;
    else
        goto L_7ef3;

L_7ef3:
    if ((lpprod->iItem != 0x4))
        goto L_7f1b;
    else
        goto L_7f13;

L_7f13:
    if ((0x0 == 0x0))
        goto L_7f43;
    else
        goto L_7f1b;

L_7f1b:
    if ((lpprod->iItem != 0x5))
        goto L_7f49;
    else
        goto L_7f3b;

L_7f3b:
    if ((0x0 != 0x0))
        goto L_7f49;
    else
        goto L_7f43;

L_7f43:
    return 0x1;

L_7f49:
    return 0x0;
}

int16_t IpctCanTerraformLppl(PLANET *lppl) {
    int16_t rgMax[3];
    int16_t i;
    int16_t rgMin[3];
    int16_t rgCost[3];
    int16_t ipct;

L_7f56:
    if ((FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, 0x1) != 0x0))
        goto L_7f8b;
    else
        goto L_7f85;

L_7f85:
    return 0x0;

L_7f8b:
    ipct = 0x0;
    i = 0x0;
    goto L_800c;

L_7f98:
    if ((rgMin[i] == 0xffff))
        goto L_7fcf;
    else
        goto L_7faa;

L_7faa:
    ipct = (ipct + ((uint16_t)(lppl->rgEnvVar[i]) - rgMin[i]));

L_7fcf:
    if ((rgMax[i] == 0xffff))
        goto L_8008;
    else
        goto L_7fe1;

L_7fe1:
    ipct = (ipct + (rgMax[i] - (uint16_t)(lppl->rgEnvVar[i])));

L_8008:
    i = (i + 0x1);

L_800c:
    if ((i < 0x3))
        goto L_7f98;
    else
        goto L_8015;

L_8015:
    return ipct;
}

int16_t FCanTerraformLppl(PLANET *lppl, int16_t *rgEnvMin, int16_t *rgEnvMax, int16_t *rgEnvCost, int16_t fHelp) {
    int16_t  fRet;
    int16_t  i;
    int16_t  rgMove[3];
    int16_t  iPlrSav;
    PART     part;
    int16_t  dMin;
    int16_t  dMax;
    int16_t  dCur;
    int16_t  ienvIdeal;
    uint16_t t_merge_83a4_0001;
    uint16_t t_merge_840e_0001;
    int16_t  t_merge_852d_0001;
    int16_t  t_merge_85ae_0001;

L_8022:
    iPlrSav = idPlayer;
    if ((idPlayer != 0xffff))
        goto L_8045;
    else
        goto L_803b;

L_803b:
    idPlayer = lppl->iPlayer;

L_8045:
    part.hs.grhst = hstTerra;
    i = 0x7;
    goto L_8087;

L_8052:
    part.hs.iItem = i;
    if ((FLookupPart(&(part)) == 0x1))
        goto L_8090;
    else
        goto L_807d;

L_807d:

L_8083:
    i = (i - 0x1);

L_8087:
    if ((i >= 0x0))
        goto L_8052;
    else
        goto L_8090;

L_8090:
    if ((i < 0x0))
        goto L_80dc;
    else
        goto L_8099;

L_8099:
    fRet = 0x1;
    i = 0x0;
    goto L_80d0;

L_80a6:
    /* untranslated: rgMove[i] = part[52:2](part.pcom) */
    rgEnvCost[i] = part.pcom->resCost;
    i = (i + 0x1);

L_80d0:
    if ((i >= 0x3))
        goto L_8104;
    else
        goto L_80d6;

L_80d6:

L_80dc:
    fRet = 0x0;
    i = 0x0;
    goto L_80fb;

L_80e9:
    rgMove[i] = 0x0;
    i = (i + 0x1);

L_80fb:
    if ((i < 0x3))
        goto L_80e9;
    else
        goto L_8104;

L_8104:
    i = 0x3;
    goto L_8144;

L_810c:
    part.hs.iItem = (i + 0x8);
    if ((FLookupPart(&(part)) == 0x1))
        goto L_814d;
    else
        goto L_813a;

L_813a:

L_8140:
    i = (i - 0x1);

L_8144:
    if ((i >= 0x0))
        goto L_810c;
    else
        goto L_814d;

L_814d:
    if ((i < 0x0))
        goto L_8180;
    else
        goto L_8156;

L_8156:
    /* untranslated: branch part[52:2](part.pcom) <= rgMove[0x0] ? L_8180 : L_8165 */

L_8165:
    fRet = 0x1;
    /* untranslated: rgMove[0x0] = part[52:2](part.pcom) */
    *(rgEnvCost) = part.pcom->resCost;

L_8180:
    i = 0x3;
    goto L_81c0;

L_8188:
    part.hs.iItem = (i + 0xc);
    if ((FLookupPart(&(part)) == 0x1))
        goto L_81c9;
    else
        goto L_81b6;

L_81b6:

L_81bc:
    i = (i - 0x1);

L_81c0:
    if ((i >= 0x0))
        goto L_8188;
    else
        goto L_81c9;

L_81c9:
    if ((i < 0x0))
        goto L_81fd;
    else
        goto L_81d2;

L_81d2:
    /* untranslated: branch part[52:2](part.pcom) <= rgMove[0x1] ? L_81fd : L_81e1 */

L_81e1:
    fRet = 0x1;
    /* untranslated: rgMove[0x1] = part[52:2](part.pcom) */
    rgEnvCost[0x1] = part.pcom->resCost;

L_81fd:
    i = 0x3;
    goto L_823d;

L_8205:
    part.hs.iItem = (i + 0x10);
    if ((FLookupPart(&(part)) == 0x1))
        goto L_8246;
    else
        goto L_8233;

L_8233:

L_8239:
    i = (i - 0x1);

L_823d:
    if ((i >= 0x0))
        goto L_8205;
    else
        goto L_8246;

L_8246:
    if ((i < 0x0))
        goto L_827a;
    else
        goto L_824f;

L_824f:
    /* untranslated: branch part[52:2](part.pcom) <= rgMove[0x2] ? L_827a : L_825e */

L_825e:
    fRet = 0x1;
    /* untranslated: rgMove[0x2] = part[52:2](part.pcom) */
    rgEnvCost[0x2] = part.pcom->resCost;

L_827a:
    if ((fRet != 0x0))
        goto L_828f;
    else
        goto L_8283;

L_8283:
    idPlayer = iPlrSav;
    return 0x0;

L_828f:
    i = 0x0;
    goto L_86cc;

L_8297:
    if ((rgMove[i] == 0x0))
        goto L_82ca;
    else
        goto L_82a9;

L_82a9:
    if (((uint16_t)(rgplr[idPlayer].rgEnvVarMin[i]) != 0xffff))
        goto L_82ea;
    else
        goto L_82ca;

L_82ca:
    rgEnvMax[i] = 0xffff;
    rgEnvMin[i] = 0xffff;
    goto L_86c8;

L_82ea:
    rgEnvMin[i] = ((uint16_t)(lppl->rgEnvVarOrig[i]) - rgMove[i]);
    rgEnvMax[i] = ((uint16_t)(lppl->rgEnvVarOrig[i]) + rgMove[i]);
    if ((rgEnvMin[i] < (uint16_t)(lppl->rgEnvVar[i])))
        goto L_837e;
    else
        goto L_836d;

L_836d:
    rgEnvMin[i] = 0xffff;
    goto L_83b0;

L_837e:
    if ((0x1 <= rgEnvMin[i]))
        goto L_8398;
    else
        goto L_8392;

L_8392:
    t_merge_83a4_0001 = 0x1;
    goto L_83a4;

L_8398:
    t_merge_83a4_0001 = rgEnvMin[i];

L_83a4:
    rgEnvMin[i] = t_merge_83a4_0001;

L_83b0:
    if ((rgEnvMax[i] > (uint16_t)(lppl->rgEnvVar[i])))
        goto L_83e8;
    else
        goto L_83d7;

L_83d7:
    rgEnvMax[i] = 0xffff;
    goto L_841a;

L_83e8:
    if ((0x63 >= rgEnvMax[i]))
        goto L_8402;
    else
        goto L_83fc;

L_83fc:
    t_merge_840e_0001 = 0x63;
    goto L_840e;

L_8402:
    t_merge_840e_0001 = rgEnvMax[i];

L_840e:
    rgEnvMax[i] = t_merge_840e_0001;

L_841a:
    if ((fHelp == 0x0))
        goto L_85bd;
    else
        goto L_8423;

L_8423:
    if (((uint16_t)(lppl->rgEnvVar[i]) != (uint16_t)(rgplr[idPlayer].rgEnvVar[i])))
        goto L_847f;
    else
        goto L_845f;

L_845f:
    rgEnvMax[i] = 0xffff;
    rgEnvMin[i] = 0xffff;
    goto L_86c8;

L_847f:
    if (((uint16_t)(lppl->rgEnvVar[i]) <= (uint16_t)(rgplr[idPlayer].rgEnvVar[i])))
        goto L_853c;
    else
        goto L_84bb;

L_84bb:
    rgEnvMax[i] = 0xffff;
    if ((rgEnvMin[i] == 0xffff))
        goto L_86c8;
    else
        goto L_84db;

L_84db:
    if ((rgEnvMin[i] <= (uint16_t)(rgplr[idPlayer].rgEnvVar[i])))
        goto L_8514;
    else
        goto L_8505;

L_8505:
    t_merge_852d_0001 = rgEnvMin[i];
    goto L_852d;

L_8514:
    t_merge_852d_0001 = (uint16_t)(rgplr[idPlayer].rgEnvVar[i]);

L_852d:
    rgEnvMin[i] = t_merge_852d_0001;

L_853c:
    rgEnvMin[i] = 0xffff;
    if ((rgEnvMax[i] == 0xffff))
        goto L_86c8;
    else
        goto L_855c;

L_855c:
    if ((rgEnvMax[i] >= (uint16_t)(rgplr[idPlayer].rgEnvVar[i])))
        goto L_8595;
    else
        goto L_8586;

L_8586:
    t_merge_85ae_0001 = rgEnvMax[i];
    goto L_85ae;

L_8595:
    t_merge_85ae_0001 = (uint16_t)(rgplr[idPlayer].rgEnvVar[i]);

L_85ae:
    rgEnvMax[i] = t_merge_85ae_0001;

L_85bd:
    ienvIdeal = (uint16_t)(rgplr[idPlayer].rgEnvVar[i]);
    dCur = abs(((uint16_t)(lppl->rgEnvVar[i]) - ienvIdeal));
    if ((rgEnvMin[i] == 0xffff))
        goto L_862e;
    else
        goto L_8610;

L_8610:
    dMin = abs((rgEnvMin[i] - ienvIdeal));
    goto L_8633;

L_862e:
    dMin = 0x0;

L_8633:
    if ((rgEnvMax[i] == 0xffff))
        goto L_8663;
    else
        goto L_8645;

L_8645:
    dMax = abs((rgEnvMax[i] - ienvIdeal));
    goto L_8668;

L_8663:
    dMax = 0x0;

L_8668:
    if ((dCur < dMin))
        goto L_869e;
    else
        goto L_8673;

L_8673:
    if ((dCur < dMax))
        goto L_869e;
    else
        goto L_867e;

L_867e:
    rgEnvMax[i] = 0xffff;
    rgEnvMin[i] = 0xffff;
    goto L_86c8;

L_869e:
    if ((dMin < dMax))
        goto L_86ba;
    else
        goto L_86a9;

L_86a9:
    rgEnvMax[i] = 0xffff;
    goto L_86c8;

L_86ba:
    rgEnvMin[i] = 0xffff;

L_86c8:
    i = (i + 0x1);

L_86cc:
    if ((i < 0x3))
        goto L_8297;
    else
        goto L_86d5;

L_86d5:
    i = 0x0;
    goto L_8708;

L_86dd:
    if ((rgEnvMax[i] != 0xffff))
        goto L_8711;
    else
        goto L_86ef;

L_86ef:
    if ((rgEnvMin[i] != 0xffff))
        goto L_8711;
    else
        goto L_86fe;

L_86fe:

L_8704:
    i = (i + 0x1);

L_8708:
    if ((i < 0x3))
        goto L_86dd;
    else
        goto L_8711;

L_8711:
    idPlayer = iPlrSav;
    if ((i == 0x3))
        goto L_8726;
    else
        goto L_8720;

L_8720:
    return 0x1;

L_8726:
    return 0x0;
}

void UninhabitPlanet(PLANET *lppl) {
    int16_t i;

L_8732:
    if ((lppl->iPlayer < 0x0))
        goto L_87ab;
    else
        goto L_8748;

L_8748:
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) != raTerra))
        goto L_87ab;
    else
        goto L_876c;

L_876c:
    i = 0x0;
    goto L_87a2;

L_8774:
    lppl->rgEnvVar[i] = lppl->rgEnvVarOrig[i];
    i = (i + 0x1);

L_87a2:
    if ((i < 0x3))
        goto L_8774;
    else
        goto L_87ab;

L_87ab:
    lppl->iPlayer = 0xffff;
    lppl->rgwtMin[0x3] = 0x0;
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_87da;
    else
        goto L_87d0;

L_87d0:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_882f;
    else
        goto L_87da;

L_87da:
    FreePl(lpPlanets[lppl->id].lpplprod);
    lpPlanets[lppl->id].lpplprod = 0x0;
    lppl->lpplprod = 0x0;

L_882f:
    *(lppl + 0x18) = ((*(lppl + 0x18) & 0xffff) | 0x0);
    lppl->fNoResearch = 0x0;
    lppl->fStarbase = 0x0;
    lppl->cDefenses = 0x0;
    *(lppl + 0x1a) = ((*(lppl + 0x1a) & 0xffff) | 0x0);
    lppl->iScanner = 0x1f;
    lppl->lStarbase = 0x0;
    return;
}

int16_t PctCloakFromHuldef(HUL *lphul, int16_t iplr, int16_t *ppctSteal) {
    int16_t  chs;
    HS      *lphs;
    int32_t  cPts;
    int16_t  cScore;
    int16_t  j;
    uint16_t t_merge_8afb_0001;

L_88c0:
    cPts = 0x0;
    chs = lphul->chs;
    if ((iplr == 0xffff))
        goto L_8922;
    else
        goto L_88e9;

L_88e9:
    if ((lphul->ihuldef < ihuldefOrbitalFort))
        goto L_8922;
    else
        goto L_88f5;

L_88f5:
    if ((GetRaceGrbit(rgplr[iplr], ibitRaceISB) == 0x0))
        goto L_8922;
    else
        goto L_8915;

L_8915:
    cPts = 0x28;
    goto L_892c;

L_8922:
    cPts = 0x0;

L_892c:
    if ((iplr == 0xffff))
        goto L_895e;
    else
        goto L_8935;

L_8935:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raStealth))
        goto L_895e;
    else
        goto L_8955;

L_8955:
    cPts = (cPts + 0x12c);

L_895e:
    if ((ppctSteal == 0x0))
        goto L_896e;
    else
        goto L_8967;

L_8967:
    *(ppctSteal) = 0x0;

L_896e:
    j = 0x0;
    lphs = lphul->rghs;
    goto L_899c;

L_8989:
    j = (j + 0x1);
    lphs = (lphs + 0x1);

L_899c:
    if ((j >= chs))
        goto L_8a1f;
    else
        goto L_89a7;

L_89a7:
    cPts = (cPts + (uint32_t)(CPtsCloakFromLphs(lphs)));
    if ((lphs->grhst != hstScanner))
        goto L_8989;
    else
        goto L_89c8;

L_89c8:
    if ((ppctSteal == 0x0))
        goto L_8989;
    else
        goto L_89d1;

L_89d1:
    if ((lphs->iItem != iscannerPickPocketScanner))
        goto L_89f8;
    else
        goto L_89e3;

L_89e3:
    if ((*(ppctSteal) >= 0x46))
        goto L_8989;
    else
        goto L_89ee;

L_89ee:
    *(ppctSteal) = 0x46;

L_89f8:
    if ((lphs->iItem != iscannerRobberBaronScanner))
        goto L_8989;
    else
        goto L_8a0a;

L_8a0a:
    if ((*(ppctSteal) >= 0x50))
        goto L_8989;
    else
        goto L_8a15;

L_8a15:
    *(ppctSteal) = 0x50;

L_8a1f:
    if ((LOWORD(cPts) != 0x0))
        goto L_8a37;
    else
        goto L_8a28;

L_8a28:
    if ((HIWORD(cPts) != 0x0))
        goto L_8a37;
    else
        goto L_8a31;

L_8a31:
    return 0x0;

L_8a37:
    if ((HIWORD(cPts) < 0x0))
        goto L_8a66;
    else
        goto L_8a40;

L_8a40:
    if ((HIWORD(cPts) > 0x0))
        goto L_8a4e;
    else
        goto L_8a45;

L_8a45:
    if ((LOWORD(cPts) < 0x0))
        goto L_8a66;
    else
        goto L_8a4e;

L_8a4e:
    if ((HIWORD(cPts) < 0x0))
        goto L_8a6c;
    else
        goto L_8a57;

L_8a57:
    if ((HIWORD(cPts) > 0x0))
        goto L_8a66;
    else
        goto L_8a5c;

L_8a5c:
    if ((LOWORD(cPts) <= 0x61a8))
        goto L_8a6c;
    else
        goto L_8a66;

L_8a66:
    return 0x0;

L_8a6c:
    cScore = LOWORD(cPts);
    if ((cScore > 0x64))
        goto L_8a86;
    else
        goto L_8a7e;

L_8a7e:
    return (cScore >> 0x1);

L_8a86:
    cScore = (cScore - 0x64);
    if ((cScore > 0xc8))
        goto L_8aa3;
    else
        goto L_8a94;

L_8a94:
    return ((cScore >> 0x3) + 0x32);

L_8aa3:
    cScore = (cScore - 0xc8);
    if ((cScore > 0x138))
        goto L_8ac1;
    else
        goto L_8ab2;

L_8ab2:
    return (((uint32_t)(cScore) / 0x18) + 0x4b);

L_8ac1:
    cScore = (cScore - 0x138);
    if ((cScore > 0x200))
        goto L_8ade;
    else
        goto L_8ad0;

L_8ad0:
    return ((cScore >> 0x6) + 0x58);

L_8ade:
    if ((cScore >= 0x3e8))
        goto L_8b01;
    else
        goto L_8ae8;

L_8ae8:
    if ((cScore < 0x300))
        goto L_8af8;
    else
        goto L_8af2;

L_8af2:
    t_merge_8afb_0001 = 0x1;
    goto L_8afb;

L_8af8:
    t_merge_8afb_0001 = 0x0;

L_8afb:
    return (t_merge_8afb_0001 + 0x60);

L_8b01:
    return 0x62;
}
