#include "common.h"

uint16_t mpicolgrbitBU[12] = {255, 255, 255, 255, 255, 255, 255, 8, 16, 32, 64, 128};

int32_t ReportDlg(HWND hwnd, uint16_t msg, uint16_t wParam, int32_t lParam) {
    HDC         hdc;
    HMENU       hmenu;
    RECT        rc;
    int16_t     i;
    int16_t     dx;
    uint16_t    swp;
    int16_t     cRow;
    POINT       pt;
    int16_t     ibit;
    int16_t     iCol;
    int16_t     iRow;
    int16_t     xCur;
    int16_t     iCur;
    int16_t     iNew;
    PAINTSTRUCT ps;
    MessageId   idm;
    int16_t     t_merge_01d9_0001;
    uint16_t    t_merge_04b1_0001;
    int16_t     t_07d4;

L_0018:
    goto L_0965;

L_0027:
    hwndReportDlg = hwnd;
    hdc = GetDC(hwnd);
    SelectObject(hdc, rghfontArial8[1]);
    i = 0;
    goto L_0050;

L_004c:
    i = (i + 1);

L_0050:
    if ((i >= vprptCur->cFields))
        goto L_0097;
    else
        goto L_005f;

L_005f:
    dx = DxReportColHdr(vprptCur->irpt, i, szWork, hdc);
    vprptCur->rgbdx[i] = LOBYTE(((uint32_t)(dx) / 0x2));
    goto L_004c;

L_0097:
    ReleaseDC(hwnd, hdc);
    SortReportCache(vprptCur->irpt, vprptCur->icolSort);
    SetWindowPos(hwnd, 0x0, 0x0, 0x0, vprptCur->ptSize.x, vprptCur->ptSize.y, 0xe);
    StickyDlgPos(hwnd, ((uint8_t *)(vprptCur) + 0x10), 0x1);
    vprptCur->hwndVScroll = CreateWindow("SCROLLBAR", 0x0, 0x40000001, 0x0, 0x0, 0x32, 0x32, hwnd, 0x0, hInst, 0x0);
    vprptCur->hwndHScroll = CreateWindow("SCROLLBAR", 0x0, WS_CHILD, 0x0, 0x0, 0x32, 0x32, hwnd, 0x0, hInst, 0x0);
    if ((gd.fTutorial == 0x0))
        goto L_019b;
    else
        goto L_0196;

L_0196:
    AdvanceTutor();

L_019b:
    GetClientRect(hwnd, &(rc));
    cRow = ((uint32_t)((rc.bottom + 0xffdc)) / (dyArial8 + 4));
    if ((cRow >= vprptCur->cRows))
        goto L_01d2;
    else
        goto L_01cc;

L_01cc:
    t_merge_01d9_0001 = cRow;
    goto L_01d9;

L_01d2:
    t_merge_01d9_0001 = vprptCur->cRows;

L_01d9:
    vprptCur->cRowsVis = t_merge_01d9_0001;
    if ((vprptCur->cRowsVis < vprptCur->cRows))
        goto L_021c;
    else
        goto L_01f3;

L_01f3:
    swp = 0x84;
    vprptCur->irowFirst = 0;
    SetScrollPos(vprptCur->hwndVScroll, 0x2, 0x0, 0x0);
    goto L_02b5;

L_021c:
    swp = 0x44;
    if (((vprptCur->irowFirst + vprptCur->cRowsVis) <= vprptCur->cRows))
        goto L_0273;
    else
        goto L_023b;

L_023b:
    if ((vprptCur->irowFirst <= 0))
        goto L_0273;
    else
        goto L_0248;

L_0248:
    vprptCur->irowFirst = (vprptCur->cRows - vprptCur->cRowsVis);
    if ((vprptCur->irowFirst >= 0))
        goto L_0273;
    else
        goto L_026a;

L_026a:
    vprptCur->irowFirst = 0;

L_0273:
    SetScrollPos(vprptCur->hwndVScroll, 0x2, vprptCur->irowFirst, 0x0);
    SetScrollRange(vprptCur->hwndVScroll, 0x2, 0x0, (vprptCur->cRows - vprptCur->cRowsVis), 0x1);

L_02b5:
    dx = GetSystemMetrics(SM_CXVSCROLL);
    SetWindowPos(vprptCur->hwndVScroll, 0x0, (rc.right - dx), (dyArial8 + 6), dx, (LOWORD(((dyArial8 + 4) * vprptCur->cRowsVis)) + 0x1), swp);
    SetHScrollBar();
    if ((msg != 0x1))
        goto L_030b;
    else
        goto L_0304;

L_0304:
    return 0x10000;

L_030b:
    return 0x0;

L_0312:
    /* untranslated: part[12:2](lParam) = 0x12c */
    /* untranslated: part[14:2](lParam) = 0xdc */
    return 0x0;

L_0337:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x10000;

L_0362:
    xCur = 2;
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    if ((pt.y < 2))
        goto L_09c8;
    else
        goto L_0390;

L_0390:
    if ((pt.x < 2))
        goto L_09c8;
    else
        goto L_0396;

L_0396:

L_039c:
    if ((pt.y >= (dyArial8 + 6)))
        goto L_03b2;
    else
        goto L_03aa;

L_03aa:
    iRow = -1;
    goto L_03f1;

L_03b2:
    iRow = ((uint32_t)(((pt.y + 0xfffe) - (dyArial8 + 0x4))) / (dyArial8 + 4));
    if ((iRow >= vprptCur->cRowsVis))
        goto L_09c8;
    else
        goto L_03e1;

L_03e1:

L_03e7:
    iRow = (iRow + vprptCur->irowFirst);

L_03f1:
    iCol = -1;
    i = 0;
    ibit = 1;
    goto L_0416;

L_0406:
    i = (i + 1);
    ibit = (ibit * 2);

L_0416:
    if ((i >= vprptCur->cFields))
        goto L_0489;
    else
        goto L_0425;

L_0425:
    if (((ibit & LOWORD(vprptCur->grbitVisible)) != 0x0))
        goto L_0442;
    else
        goto L_043a;

L_043a:
    if (((SIGNHIWORD(ibit) & HIWORD(vprptCur->grbitVisible)) == 0x0))
        goto L_0406;
    else
        goto L_0442;

L_0442:
    if ((i == 0))
        goto L_045a;
    else
        goto L_044b;

L_044b:
    if ((i < vprptCur->cFieldFirst))
        goto L_0406;
    else
        goto L_045a;

L_045a:
    xCur = (xCur + (vprptCur->rgbdx[i] * 2));
    if ((xCur <= pt.x))
        goto L_0406;
    else
        goto L_047d;

L_047d:
    iCol = i;
    goto L_0489;

L_0489:
    if ((iCol == -1))
        goto L_09c8;
    else
        goto L_048f;

L_048f:

L_0495:
    if ((iRow != -1))
        goto L_04c6;
    else
        goto L_049e;

L_049e:
    if ((msg != 0x204))
        goto L_04ae;
    else
        goto L_04a8;

L_04a8:
    t_merge_04b1_0001 = 0x1;
    goto L_04b1;

L_04ae:
    t_merge_04b1_0001 = 0x0;

L_04b1:
    ReportColumnPopup(pt, iCol, t_merge_04b1_0001);
    goto L_04e1;

L_04c6:
    ExecuteReportClick(pt, vprptCur->irpt, iCol, iRow);

L_04e1:
    if ((gd.fTutorial == 0x0))
        goto L_09c8;
    else
        goto L_04f4;

L_04f4:
    AdvanceTutor();

L_04fc:
    iCur = GetScrollPos((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), 0x2);
    iNew = iCur;
    goto L_0576;
    iNew = 2000;
    goto L_0597;
    iNew = (iNew + 1);
    goto L_0597;
    iNew = (iNew - 1);
    goto L_0597;
    iNew = (iNew + (vprptCur->cRowsVis - 1));
    goto L_0597;
    iNew = (iNew - (vprptCur->cRowsVis - 1));
    goto L_0597;
    iNew = LOWORD(lParam);
    goto L_0597;
    iNew = 0;
    goto L_0597;

L_0576:

L_0597:
    if ((iNew <= (vprptCur->cRows - vprptCur->cRowsVis)))
        goto L_05be;
    else
        goto L_05ad;

L_05ad:
    iNew = (vprptCur->cRows - vprptCur->cRowsVis);

L_05be:
    if ((iNew >= 0))
        goto L_05cc;
    else
        goto L_05c7;

L_05c7:
    iNew = 0;

L_05cc:
    if ((iNew == iCur))
        goto L_0673;
    else
        goto L_05d7;

L_05d7:
    vprptCur->irowFirst = iNew;
    GetClientRect(hwnd, &(rc));
    rc.left = 2;
    rc.right = (rc.right - GetSystemMetrics(SM_CXVSCROLL));
    rc.top = (dyArial8 + 6);
    rc.bottom = (LOWORD(((dyArial8 + 0x4) * vprptCur->cRowsVis)) + rc.top);
    ScrollWindow(hwnd, 0x0, LOWORD(((dyArial8 + 4) * (iCur - iNew))), &(rc), &(rc));
    SetScrollPos((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), 0x2, iNew, 0x1);
    UpdateWindow(hwnd);

L_0673:
    return 0x0;

L_067c:
    iCur = GetScrollPos((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), 0x2);
    iNew = iCur;
    goto L_06e4;
    iNew = 2000;
    goto L_0705;
    iNew = (iNew + 1);
    goto L_0705;
    iNew = (iNew - 1);
    goto L_0705;
    iNew = (iNew + 3);
    goto L_0705;
    iNew = (iNew - 3);
    goto L_0705;
    iNew = LOWORD(lParam);
    goto L_0705;
    iNew = 0;
    goto L_0705;

L_06e4:

L_0705:
    if ((iNew <= vprptCur->cColScroll))
        goto L_071e;
    else
        goto L_0714;

L_0714:
    iNew = vprptCur->cColScroll;

L_071e:
    if ((iNew >= 0))
        goto L_072c;
    else
        goto L_0727;

L_0727:
    iNew = 0;

L_072c:
    if ((iNew == iCur))
        goto L_080f;
    else
        goto L_0737;

L_0737:
    SetScrollPos((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), 0x2, iNew, 0x1);
    iNew = GetScrollPos((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), 0x2);
    if ((iNew == iCur))
        goto L_080f;
    else
        goto L_0788;

L_0788:
    i = 1;
    ibit = 2;
    goto L_07a8;

L_0798:
    i = (i + 1);
    ibit = (ibit * 2);

L_07a8:
    if ((i >= vprptCur->cFields))
        goto L_07e9;
    else
        goto L_07b7;

L_07b7:
    if (((ibit & LOWORD(vprptCur->grbitVisible)) != 0x0))
        goto L_07d4;
    else
        goto L_07cc;

L_07cc:
    if (((SIGNHIWORD(ibit) & HIWORD(vprptCur->grbitVisible)) == 0x0))
        goto L_0798;
    else
        goto L_07d4;

L_07d4:
    t_07d4 = iNew;
    iNew = (iNew - 1);
    if ((t_07d4 <= 0))
        goto L_07e9;
    else
        goto L_07e0;

L_07e0:

L_07e9:
    vprptCur->cFieldFirst = i;
    InvalidateRect(hwnd, 0x0, 0x1);
    UpdateWindow(hwnd);

L_080f:
    return 0x0;

L_0818:
    hdc = BeginPaint(hwnd, &(ps));
    DrawReport(hwnd, hdc, &(ps.rcPaint));
    EndPaint(hwnd, &(ps));
    gd.fRptSafeDraw = 0x0;
    return 0x10000;

L_0860:
    StickyDlgPos(hwnd, ((uint8_t *)(vprptCur) + 0x10), 0x0);
    GetWindowRect(hwnd, &(rc));
    vprptCur->ptSize.x = (rc.right - rc.left);
    vprptCur->ptSize.y = (rc.bottom - rc.top);
    hwndReportDlg = 0x0;
    fBrowserValid = 0;
    hmenu = GetASubMenu(hwndFrame, 0x4);
    goto L_08f1;

L_08ce:
    idm = 0x8ff;
    goto L_0914;

L_08d6:
    idm = 0x900;
    goto L_0914;

L_08de:
    idm = 0x8fd;
    goto L_0914;

L_08e6:
    idm = 0x901;
    goto L_0914;

L_08f1:
    if ((vprptCur->irpt == 0))
        goto L_08de;
    else
        goto L_08f9;

L_08f9:
    if ((vprptCur->irpt == 1))
        goto L_08ce;
    else
        goto L_0901;

L_0901:
    if ((vprptCur->irpt == 2))
        goto L_08d6;
    else
        goto L_0909;

L_0909:
    if ((vprptCur->irpt == 3))
        goto L_08e6;
    else
        goto L_0914;

L_0914:
    CheckMenuItem(hmenu, idm, 0x0);
    vprptCur = 0x0;
    if ((gd.fTutorial == 0x0))
        goto L_09c8;
    else
        goto L_093d;

L_093d:
    AdvanceTutor();

L_0945:
    if ((wParam != 0x2))
        goto L_09c8;
    else
        goto L_094e;

L_094e:
    DestroyWindow(hwnd);
    return 0x10000;

L_0965:
    if ((msg == 0x1))
        goto L_0027;
    else
        goto L_096d;

L_096d:
    if ((msg == 0x2))
        goto L_0860;
    else
        goto L_0975;

L_0975:
    if ((msg == 0x5))
        goto L_019b;
    else
        goto L_097d;

L_097d:
    if ((msg == 0xf))
        goto L_0818;
    else
        goto L_0985;

L_0985:
    if ((msg == 0x14))
        goto L_0337;
    else
        goto L_098d;

L_098d:
    if ((msg == 0x24))
        goto L_0312;
    else
        goto L_0995;

L_0995:
    if ((msg == 0x111))
        goto L_0945;
    else
        goto L_099d;

L_099d:
    if ((msg == 0x114))
        goto L_067c;
    else
        goto L_09a5;

L_09a5:
    if ((msg == 0x115))
        goto L_04fc;
    else
        goto L_09ad;

L_09ad:
    if ((msg == 0x201))
        goto L_0362;
    else
        goto L_09b5;

L_09b5:
    if ((msg == 0x203))
        goto L_0362;
    else
        goto L_09bd;

L_09bd:
    if ((msg == 0x204))
        goto L_0362;
    else
        goto L_09c8;

L_09c8:
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void SetHScrollBar() {
    uint16_t swp;
    int16_t  dy;
    int16_t  ccolSkipped;
    int16_t  ccolHidden;
    int16_t  i;
    int16_t  ibit;
    int16_t  xRight;
    int16_t  dx;
    int16_t  xTitle;
    RECT     rc;

L_09e8:
    GetClientRect(hwndReportDlg, &(rc));
    dx = GetSystemMetrics(SM_CXVSCROLL);
    xTitle = ((vprptCur->rgbdx[0] * 2) + 2);
    xRight = (((rc.right - xTitle) - 2) - dx);
    ccolSkipped = 0;
    ccolHidden = 0;
    i = (vprptCur->cFields - 1);
    ibit = (0x1 << vprptCur->cFields);
    goto L_0a68;

L_0a58:
    i = (i - 1);
    ibit = (ibit >> 0x1);

L_0a68:
    if ((i <= 0))
        goto L_0ac9;
    else
        goto L_0a71;

L_0a71:
    if (((ibit & LOWORD(vprptCur->grbitVisible)) != 0x0))
        goto L_0a8e;
    else
        goto L_0a86;

L_0a86:
    if (((SIGNHIWORD(ibit) & HIWORD(vprptCur->grbitVisible)) == 0x0))
        goto L_0a58;
    else
        goto L_0a8e;

L_0a8e:
    if ((i >= vprptCur->cFieldFirst))
        goto L_0aa1;
    else
        goto L_0a9d;

L_0a9d:
    ccolSkipped = (ccolSkipped + 1);

L_0aa1:
    xRight = (xRight - (vprptCur->rgbdx[i] * 2));
    if ((xRight >= 0))
        goto L_0a58;
    else
        goto L_0ac2;

L_0ac2:
    ccolHidden = (ccolHidden + 1);

L_0ac9:
    vprptCur->cColScroll = 0;
    if ((ccolHidden != 0))
        goto L_0b04;
    else
        goto L_0adb;

L_0adb:
    swp = 0x84;
    vprptCur->cFieldFirst = 1;
    SetScrollPos(vprptCur->hwndHScroll, 0x2, 0x0, 0x0);
    goto L_0b5e;

L_0b04:
    swp = 0x44;
    if ((ccolSkipped <= ccolHidden))
        goto L_0b22;
    else
        goto L_0b14;

L_0b14:
    ccolSkipped = 0;
    vprptCur->cFieldFirst = 1;

L_0b22:
    SetScrollPos(vprptCur->hwndHScroll, 0x2, ccolSkipped, 0x0);
    SetScrollRange(vprptCur->hwndHScroll, 0x2, 0x0, ccolHidden, 0x1);
    vprptCur->cColScroll = ccolHidden;

L_0b5e:
    dy = GetSystemMetrics(SM_CYHSCROLL);
    SetWindowPos(vprptCur->hwndHScroll, 0x0, xTitle, (((dyArial8 + 6) + LOWORD(((dyArial8 + 4) * vprptCur->cRowsVis))) + 0x1), ((rc.right - dx) - xTitle), dy,
                 swp);
    return;
}

void DrawReport(HWND hwnd, HDC hdc, RECT *prc) {
    char    szTit[40];
    int16_t irowLast;
    int16_t j;
    int16_t i;
    int16_t yRow;
    int16_t ibit;
    int16_t dx;
    int16_t xCol;
    RECT    rc;

L_0bae:
    ibit = 1;
    xCol = 2;
    yRow = 2;
    SelectObject(hdc, rghfontArial8[1]);
    SetBkMode(hdc, TRANSPARENT);
    if ((prc->top > ((dyArial8 + 4) + yRow)))
        goto NoHdrDraw;
    else
        goto L_0bef;

L_0bef:

L_0bf5:
    i = 0;
    goto L_0c0d;

L_0bfd:
    i = (i + 1);
    ibit = (ibit * 2);

L_0c0d:
    if ((i >= vprptCur->cFields))
        goto NoHdrDraw;
    else
        goto L_0c1c;

L_0c1c:
    if (((ibit & LOWORD(vprptCur->grbitVisible)) != 0x0))
        goto L_0c39;
    else
        goto L_0c31;

L_0c31:
    if (((SIGNHIWORD(ibit) & HIWORD(vprptCur->grbitVisible)) == 0x0))
        goto L_0bfd;
    else
        goto L_0c39;

L_0c39:
    if ((i == 0))
        goto L_0c51;
    else
        goto L_0c42;

L_0c42:
    if ((i < vprptCur->cFieldFirst))
        goto L_0bfd;
    else
        goto L_0c51;

L_0c51:
    dx = DxReportColHdr(vprptCur->irpt, i, szTit, hdc);
    vprptCur->rgbdx[i] = LOBYTE(((uint32_t)(dx) / 0x2));
    SetRect(&(rc), xCol, yRow, ((xCol + dx) + 0xffff), ((dyArial8 + 4) + yRow));
    if ((gd.fRptSafeDraw == 0x0))
        goto L_0cd2;
    else
        goto L_0cbf;

L_0cbf:
    FillRect(hdc, &(rc), hbrButtonFace);

L_0cd2:
    if ((i != 0))
        goto L_0d08;
    else
        goto L_0cdb;

L_0cdb:
    TextOut(hdc, (rc.left + 3), (rc.top + 2), szTit, strlen(szTit));
    goto L_0d32;

L_0d08:
    CtrTextOut(hdc, (((uint32_t)((rc.right - rc.left)) / 0x2) + rc.left), (rc.top + 2), szTit, 0x0);

L_0d32:
    _Draw3dFrame(hdc, &(rc), 0x0);
    xCol = (xCol + dx);

NoHdrDraw:
    irowLast = (vprptCur->irowFirst + vprptCur->cRowsVis);
    if ((irowLast <= vprptCur->cRows))
        goto L_0d78;
    else
        goto L_0d6e;

L_0d6e:
    irowLast = vprptCur->cRows;

L_0d78:
    i = vprptCur->irowFirst;
    goto L_0f54;

L_0d85:
    yRow = (yRow + (dyArial8 + 4));
    xCol = 2;
    if ((yRow < prc->top))
        goto L_0ddc;
    else
        goto L_0da1;

L_0da1:
    if ((yRow > prc->bottom))
        goto L_0ddc;
    else
        goto L_0daf;

L_0daf:
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, xCol, yRow, 0x1, (dyArial8 + 4), PATCOPY);

L_0ddc:
    j = 0;
    ibit = 1;
    goto L_0dfc;

L_0dec:
    j = (j + 1);
    ibit = (ibit * 2);

L_0dfc:
    if ((j >= vprptCur->cFields))
        goto L_0f50;
    else
        goto L_0e0b;

L_0e0b:
    if (((ibit & LOWORD(vprptCur->grbitVisible)) != 0x0))
        goto L_0e28;
    else
        goto L_0e20;

L_0e20:
    if (((SIGNHIWORD(ibit) & HIWORD(vprptCur->grbitVisible)) == 0x0))
        goto L_0dec;
    else
        goto L_0e28;

L_0e28:
    if ((j == 0))
        goto L_0e40;
    else
        goto L_0e31;

L_0e31:
    if ((j < vprptCur->cFieldFirst))
        goto L_0dec;
    else
        goto L_0e40;

L_0e40:
    dx = (vprptCur->rgbdx[j] * 2);
    if ((yRow < (prc->top - (dyArial8 + 4))))
        goto L_0f47;
    else
        goto L_0e6e;

L_0e6e:
    if ((yRow > prc->bottom))
        goto L_0f47;
    else
        goto L_0e7c;

L_0e7c:
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, ((xCol + dx) + 0xffff), yRow, 0x1, (dyArial8 + 4), PATCOPY);
    PatBlt(hdc, xCol, ((dyArial8 + 4) + yRow), dx, 0x1, PATCOPY);
    SetRect(&(rc), (xCol + 2), (yRow + 2), ((xCol + dx) + 0xfffd), (((dyArial8 + 4) + yRow) + 0xffff));
    if ((gd.fRptSafeDraw == 0x0))
        goto L_0f2b;
    else
        goto L_0f18;

L_0f18:
    FillRect(hdc, &(rc), hbrButtonFace);

L_0f2b:
    DrawReportItem(hdc, &(rc), vprptCur->irpt, i, j);

L_0f47:
    xCol = (xCol + dx);

L_0f50:
    i = (i + 1);

L_0f54:
    if ((i < irowLast))
        goto L_0d85;
    else
        goto L_0f5f;

L_0f5f:
    return;
}

int16_t ScoreXDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t     i;
    RECT        rc;
    HDC         hdc;
    PAINTSTRUCT ps;
    POINT       pt;
    char        szT[40];
    int16_t     cchHistory;
    char       *rgszScan[20];
    int16_t     c;
    int32_t     rgid[12];
    char       *psz;
    int16_t     iSel;
    int16_t     cch;
    uint16_t    t_merge_11a9_0001;

L_0f66:
    goto L_136d;

L_0f75:
    InitScoreDlg(hwnd, gd.fScoreVictory);
    fInScoreDialog = 1;
    StickyDlgPos(hwnd, &(ptStickyScoreXDlg), 0x1);
    hwndScoreXDlg = hwnd;
    if ((gd.fTutorial == 0x0))
        goto L_0fc3;
    else
        goto L_0fbe;

L_0fbe:
    AdvanceTutor();

L_0fc3:
    return 0x1;

L_0fc9:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_0ff1:
    hdc = BeginPaint(hwnd, &(ps));
    if ((gd.fScoreVictory != 0x2))
        goto L_1024;
    else
        goto L_1016;

L_1016:
    DrawHistoryReport(hdc);
    goto L_1050;

L_1024:
    if ((gd.fScoreVictory == 0x0))
        goto L_1045;
    else
        goto L_1037;

L_1037:
    DrawVCReport(hdc);
    goto L_1050;

L_1045:
    DrawScoreReport(hdc);

L_1050:
    EndPaint(hwnd, &(ps));
    return 0x1;

L_1065:
    if ((gd.fScoreVictory != 0x2))
        goto L_13a8;
    else
        goto L_1075;

L_1075:

L_107b:
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    if ((pt.y >= ((dyArial10 + dyArial8) + 0xfffe)))
        goto L_13a8;
    else
        goto L_10a8;

L_10a8:
    if ((pt.y <= 2))
        goto L_13a8;
    else
        goto L_10b1;

L_10b1:
    SetCursor(hcurHand);
    return 0x1;

L_10c3:
    c = 0;
    if ((gd.fScoreVictory != 0x2))
        goto L_13a8;
    else
        goto L_10db;

L_10db:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) <= 0x2))
        goto L_13a8;
    else
        goto L_10f7;

L_10f7:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) >= ((dyArial10 + dyArial8) + 0xfffe)))
        goto L_13a8;
    else
        goto L_111a;

L_111a:

L_1120:
    cchHistory = CchGetString(idsHistory, szT);
    i = 0;
    goto L_11da;

L_113b:
    strcpy(&(szWork[(40 * i)]), szT);
    psz = ((0x57a4 + LOWORD((0x28 * i))) + cchHistory);
    cch = CchGetString((i + 435), psz);
    psz[(cch - 1)] = 0;
    if ((gd.iCurGraph != i))
        goto L_11a5;
    else
        goto L_119e;

L_119e:
    t_merge_11a9_0001 = 0x1;
    goto L_11a9;

L_11a5:
    t_merge_11a9_0001 = 0x0;

L_11a9:
    LOWORD(rgid[c]) = t_merge_11a9_0001;
    HIWORD(rgid[c]) = 0x0;
    c = (c + 1);
    rgszScan[c] = (0x57a4 + LOWORD((0x28 * i)));
    i = (i + 1);

L_11da:
    if ((i < 8))
        goto L_113b;
    else
        goto L_11e3;

L_11e3:
    iSel = PopupMenu(hwnd, LOWORD(lParam), (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), c, rgid, rgszScan, 0xfffe, 0x0);
    if ((iSel == -1))
        goto L_13a8;
    else
        goto L_1229;

L_1229:

L_122f:
    gd.iCurGraph = iSel;
    gd.fChgReports = 0x1;
    InvalidateRect(hwnd, 0x0, 0x1);
    goto L_13a8;

L_1266:
    if ((wParam != 0x2))
        goto L_12ba;
    else
        goto L_126f;

L_126f:
    StickyDlgPos(hwnd, &(ptStickyScoreXDlg), 0x0);
    EndDialog(hwnd, i);
    fInScoreDialog = 0;
    hwndScoreXDlg = 0x0;
    if ((gd.fTutorial == 0x0))
        goto L_12b1;
    else
        goto L_12ac;

L_12ac:
    AdvanceTutor();

L_12b1:
    return 0x1;

L_12ba:
    if ((wParam != 0x76))
        goto L_12e7;
    else
        goto L_12c3;

L_12c3:
    WinHelp(hwnd, szHelpFile, 0x1, 0x455);
    return 0x1;

L_12e7:
    if ((wParam != 0xc6))
        goto L_13a8;
    else
        goto L_12f1;

L_12f1:
    /* untranslated: gd.fScoreVictory = (words((gd.fScoreVictory + 0x1), 0x0) % 0x3) */
    InvalidateRect(hwnd, 0x0, 0x1);
    InitScoreDlg(hwnd, gd.fScoreVictory);
    if ((gd.fTutorial == 0x0))
        goto L_13a8;
    else
        goto L_1362;

L_1362:
    AdvanceTutor();

L_136d:
    if ((message == WM_PAINT))
        goto L_0ff1;
    else
        goto L_1375;

L_1375:
    if ((message == WM_ERASEBKGND))
        goto L_0fc9;
    else
        goto L_137d;

L_137d:
    if ((message == WM_SETCURSOR))
        goto L_1065;
    else
        goto L_1385;

L_1385:
    if ((message == WM_INITDIALOG))
        goto L_0f75;
    else
        goto L_138d;

L_138d:
    if ((message == WM_COMMAND))
        goto L_1266;
    else
        goto L_1395;

L_1395:
    if ((message == WM_LBUTTONDOWN))
        goto L_10c3;
    else
        goto L_139d;

L_139d:
    if ((message == WM_RBUTTONDOWN))
        goto L_10c3;
    else
        goto L_13a8;

L_13a8:
    return 0x0;
}

void InitScoreDlg(HWND hwnd, int16_t fVictory) {
    HDC      hdc;
    int16_t  dxDig;
    int16_t  dy;
    int16_t  dyFrame;
    int16_t  dxFrame;
    RECT     rcWindow;
    char    *psz;
    int16_t  dx;
    RECT     rc;
    uint16_t t_merge_1467_0001;
    uint16_t t_merge_1467_0002;
    uint16_t t_merge_14e0_0001;

L_13b6:
    hdc = GetDC(hwnd);
    SelectObject(hdc, rghfontArial8[1]);
    dxDig = LOWORD(GetTextExtent(hdc, "9", 0x1));
    if ((fVictory != 2))
        goto L_1402;
    else
        goto L_13f5;

L_13f5:
    dx = 600;
    dy = 400;
    goto L_150a;

L_1402:
    if ((fVictory == 0))
        goto L_149b;
    else
        goto L_140b;

L_140b:
    psz = PszGetCompressedString(idsExceedsSecondPlaceScore);
    /* untranslated: ss:[bp-0x22] = loword((0x6 * dxDig)) */
    /* untranslated: vdxScoreX = ((words(loword((loword(GetTextExtent(hdc, psz, strlen(psz))) * 0x3)), 0x0) / 2) + ss:[bp-0x22]) */
    if ((4 <= game.cPlayer))
        goto L_1464;
    else
        goto L_145e;

L_145e:
    t_merge_1467_0001 = 0x4;
    t_merge_1467_0002 = 0x4;
    goto L_1467;

L_1464:
    t_merge_1467_0001 = game.cPlayer;
    t_merge_1467_0002 = game.cPlayer;

L_1467:
    /* untranslated: dx = (((words(loword((loword((t_merge_1467_0001 * dyArial8)) * 0x3)), signhiword(loword((loword((t_merge_1467_0002 * dyArial8)) * 0x3)))) /
     * 2) + vdxScoreX) + 8) */
    /* untranslated: dy = ((words(loword((loword((0xb * dyArial8)) * 0x3)), signhiword(loword((loword((0xb * dyArial8)) * 0x3)))) / 2) + 88) */
    goto L_150a;

L_149b:
    psz = PszGetCompressedString(idsUnarmedShips2);
    vdxScoreX = (LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8);
    if ((4 <= game.cPlayer))
        goto L_14dd;
    else
        goto L_14d7;

L_14d7:
    t_merge_14e0_0001 = 0x4;
    goto L_14e0;

L_14dd:
    t_merge_14e0_0001 = game.cPlayer;

L_14e0:
    dx = ((LOWORD((LOWORD((t_merge_14e0_0001 * dxDig)) * 0x5)) + vdxScoreX) + 8);
    /* untranslated: dy = ((words(loword((loword((0xb * dyArial8)) * 0x3)), signhiword(loword((loword((0xb * dyArial8)) * 0x3)))) / 2) + 88) */

L_150a:
    ReleaseDC(hwnd, hdc);
    GetWindowRect(hwnd, &(rcWindow));
    GetClientRect(hwnd, &(rc));
    dxFrame = ((rcWindow.right - rcWindow.left) - rc.right);
    dyFrame = ((rcWindow.bottom - rcWindow.top) - rc.bottom);
    SetWindowPos(hwnd, 0x0, 0x0, 0x0, (dxFrame + dx), (dyFrame + dy), 0x6);
    GetWindowRect(GetDlgItem(hwnd, IDCANCEL), &(rc));
    MapWindowPoints(0x0, hwnd, &(rc), 0x2);
    OffsetRect(&(rc), 0x0, ((dy - 4) - rc.bottom));
    dx = ((uint32_t)((dx - LOWORD(((rc.right - rc.left) * 0x3)))) / 4);
    SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x00C6), 0x0, dx, rc.top, 0x0, 0x0, 0x5);
    SetWindowPos(GetDlgItem(hwnd, IDCANCEL), 0x0, ((dx * 2) + (rc.right - rc.left)), rc.top, 0x0, 0x0, 0x5);
    SetWindowPos(GetDlgItem(hwnd, IDC_HELP), 0x0, (LOWORD((3 * dx)) + ((rc.right - rc.left) * 0x2)), rc.top, 0x0, 0x0, 0x5);
    SetWindowText(hwnd, PszGetCompressedString((fVictory + 1210)));
    return;
}

void DrawVCReport(HDC hdc) {
    int16_t  grbitVC;
    int16_t  xStart;
    int16_t  dxDig;
    int16_t  yTop;
    POINT    pt;
    int16_t  cCurSav;
    StringId ids;
    COLORREF cr;
    int16_t  cCur;
    HDC      hdcMem;
    int16_t  j;
    int16_t  i;
    int16_t  iPass;
    char    *psz;
    HBITMAP  hbmpSav;
    int16_t  cch;
    int16_t  xLeft;
    int32_t  l;
    int16_t  idsT;
    int16_t  vcVal;
    uint16_t t_merge_19cc_0001;
    StringId t_1bfe;
    uint32_t t_merge_1c4c_0001;

L_168e:
    yTop = 88;
    SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, rghfontArial8[4]);
    xLeft = vdxScoreX;
    i = 0;
    goto L_16c6;

L_16c2:
    i = (i + 1);

L_16c6:
    if ((i >= game.cPlayer))
        goto L_181a;
    else
        goto L_16d1;

L_16d1:
    psz = PszPlayerName(i, 0x1, 0x1, 0x1, 0x0, 0x0);
    cch = strlen(psz);
    l = GetTextExtent(hdc, psz, cch);
    /* untranslated: ss:[bp-0x34] = LOWORD(l) */
    /* untranslated: ss:[bp-0x32] = 0x0 */
    dxDig = LOWORD(__ftol());
    if ((((rgplr[i].wMdPlr >> 0x8) & 0x1) == 0x0))
        goto L_1780;
    else
        goto L_175a;

L_175a:
    if (((rgplr[i].wFlags & 0x1) == 0x0))
        goto L_1780;
    else
        goto L_1773;

L_1773:
    cr = 0x7f7f7f;
    goto L_17bc;

L_1780:
    if ((((vlprgScoreX[i].wWord >> 0xe) & 0x1) == 0x0))
        goto L_17b2;
    else
        goto L_17a5;

L_17a5:
    cr = 0xff0000;
    goto L_17bc;

L_17b2:
    cr = 0x0;

L_17bc:
    SetTextColor(hdc, cr);
    /* untranslated: call TextOut(hdc, ((xLeft - dxDig) + (words(loword((3 * dyArial8)), signhiword(loword((3 * dyArial8)))) / 0x2)), (((yTop - dxDig) -
     * (sext16to32(dyArial8) / 2)) + 0xfffc), psz, cch) -> callresult(int16_t) */
    /* untranslated: xLeft = (xLeft + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 2)) */
    goto L_16c2;

L_181a:
    SelectObject(hdc, rghfontArial8[1]);
    SelectObject(hdc, hbrButtonShadow);
    dxDig = LOWORD(GetTextExtent(hdc, "9", 0x1));
    ids = idsOwns;
    cCur = 0;
    hdcMem = CreateCompatibleDC(hdc);
    hbmpSav = SelectObject(hdcMem, hbmpMono);
    SetTextColor(hdc, 0x0);
    SetBkColor(hdc, 0xffffff);
    xLeft = (vdxScoreX + 4);
    /* untranslated: pt.x = (sext16to32(((words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2) + 0xfff4)) / 2) */
    /* untranslated: pt.y = ((sext16to32(((words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2) + 0xfff5)) / 2) - (sext16to32(dyArial8)
     * / 4)) */
    i = 0;
    goto L_18e4;

L_18e0:
    i = (i + 1);

L_18e4:
    if ((i >= game.cPlayer))
        goto L_1af6;
    else
        goto L_18ef;

L_18ef:
    grbitVC = ((vlprgScoreX[i].wWord >> 0x6) & 0xff);
    j = 0;
    goto L_1927;

L_1917:
    j = (j + 1);
    grbitVC = (grbitVC >> 0x1);

L_1927:
    if ((j >= 7))
        goto L_1aa7;
    else
        goto L_1930;

L_1930:
    if (((grbitVC & 0x1) == 0x0))
        goto L_1917;
    else
        goto L_193e;

L_193e:
    /* untranslated: call BitBlt(hdc, (xLeft + pt.x), ((yTop + pt.y) + (words(loword((loword((j * dyArial8)) * 0x3)), signhiword(loword((loword((j * dyArial8))
     * * 0x3)))) / 0x2)), 0xe, 0xc, hdcMem, 0x0, 0x0, SRCAND) -> callresult(int16_t) */
    if ((((rgplr[i].wMdPlr >> 0x8) & 0x1) == 0x0))
        goto L_19ba;
    else
        goto L_19a1;

L_19a1:
    if (((rgplr[i].wFlags & 0x1) != 0x0))
        goto L_19e4;
    else
        goto L_19ba;

L_19ba:
    if ((j <= 1))
        goto L_19c9;
    else
        goto L_19c3;

L_19c3:
    t_merge_19cc_0001 = 0x1;
    goto L_19cc;

L_19c9:
    t_merge_19cc_0001 = 0x0;

L_19cc:
    if ((GetVCCheck(&(game), (t_merge_19cc_0001 + j)) != 0))
        goto L_1a6c;
    else
        goto L_19e4;

L_19e4:
    SetTextColor(hdc, crButtonShadow);

LOrIt:
    SetBkColor(hdc, 0x0);
    /* untranslated: call BitBlt(hdc, (xLeft + pt.x), ((yTop + pt.y) + (words(loword((loword((j * dyArial8)) * 0x3)), signhiword(loword((loword((j * dyArial8))
     * * 0x3)))) / 0x2)), 0xe, 0xc, hdcMem, 0x0, 0x0, SRCPAINT) -> callresult(int16_t) */
    SetTextColor(hdc, 0x0);
    SetBkColor(hdc, 0xffffff);
    goto L_1917;

L_1a6c:
    if ((((vlprgScoreX[i].wWord >> 0xe) & 0x1) == 0x0))
        goto L_1917;
    else
        goto L_1a91;

L_1a91:
    SetTextColor(hdc, 0xff0000);
    goto LOrIt;

L_1aa7:
    /* untranslated: call PatBlt(hdc, xLeft, (yTop - (sext16to32(dyArial8) / 4)), 0x1, ((words(loword((loword((7 * dyArial8)) * 0x3)),
     * signhiword(loword((loword((7 * dyArial8)) * 0x3)))) / 0x2) + 0x1), PATCOPY) -> callresult(int16_t) */
    /* untranslated: xLeft = (xLeft + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 2)) */
    goto L_18e0;

L_1af6:
    /* untranslated: call PatBlt(hdc, xLeft, (yTop - (sext16to32(dyArial8) / 4)), 0x1, ((words(loword((loword((7 * dyArial8)) * 0x3)),
     * signhiword(loword((loword((7 * dyArial8)) * 0x3)))) / 0x2) + 0x1), PATCOPY) -> callresult(int16_t) */
    SelectObject(hdcMem, hbmpSav);
    DeleteDC(hdcMem);
    i = 0;
    goto L_1dfd;

L_1b4d:
    iPass = 0;
    goto L_1df0;

L_1b55:
    if ((iPass != 0))
        goto L_1b6c;
    else
        goto L_1b5e;

L_1b5e:
    cCurSav = cCur;
    xStart = 0;
    goto L_1b90;

L_1b6c:
    ids = (ids - 3);
    cCur = cCurSav;
    if ((i >= 7))
        goto L_1b8b;
    else
        goto L_1b7f;

L_1b7f:
    xStart = (vdxScoreX - xLeft);
    goto L_1b90;

L_1b8b:
    xStart = 8;

L_1b90:
    xLeft = xStart;
    if ((i > 7))
        goto L_1bfa;
    else
        goto L_1b9f;

L_1b9f:
    if ((iPass != 1))
        goto L_1bfa;
    else
        goto L_1ba8;

L_1ba8:
    /* untranslated: call PatBlt(hdc, (vdxScoreX + 4), (yTop - (sext16to32(dyArial8) / 4)), (words(loword((loword((dyArial8 * game.cPlayer)) * 0x3)),
     * signhiword(loword((loword((dyArial8 * game.cPlayer)) * 0x3)))) / 0x2), 0x1, PATCOPY) -> callresult(int16_t) */
    if ((i != 7))
        goto L_1bfa;
    else
        goto L_1bee;

L_1bee:
    yTop = (yTop + ((uint32_t)(dyArial8) / 2));

L_1bfa:
    t_1bfe = ids;
    ids = (ids + 1);
    cch = CchGetString(t_1bfe, szWork);
    if ((iPass != 1))
        goto L_1c6b;
    else
        goto L_1c1a;

L_1c1a:
    if ((i >= 7))
        goto L_1c3d;
    else
        goto L_1c26;

L_1c26:
    if ((GetVCCheck(&(game), cCur) == 0))
        goto L_1c46;
    else
        goto L_1c3d;

L_1c3d:
    t_merge_1c4c_0001 = 0x0;
    goto L_1c4c;

L_1c46:
    t_merge_1c4c_0001 = 0x7f7f7f;

L_1c4c:
    SetTextColor(hdc, t_merge_1c4c_0001);
    TextOut(hdc, xLeft, yTop, szWork, cch);

L_1c6b:
    xLeft = (xLeft + LOWORD(GetTextExtent(hdc, szWork, cch)));
    j = 0;
    goto L_1dca;

L_1c88:
    if ((j != 1))
        goto L_1ca1;
    else
        goto L_1c91;

L_1c91:
    if ((i == 1))
        goto L_1ca1;
    else
        goto L_1c9a;

L_1c9a:
    ids = (ids + 1);
    goto L_1dd3;

L_1ca1:
    vcVal = GetVCVal(&(game), cCur, 0x0);
    if ((i != 0))
        goto L_1ceb;
    else
        goto L_1cc0;

L_1cc0:
    idsT = 965;
    vcVal = LOWORD((int32_t)(((uint32_t)(((uint32_t)(vcVal) * (uint32_t)(game.cPlanMax))) / 0x64)));
    goto L_1cf1;

L_1ceb:
    idsT = ids;

L_1cf1:
    cch = _wsprintf(szWork, PCTD, vcVal);
    if ((i != 3))
        goto L_1d2a;
    else
        goto L_1d16;

L_1d16:
    strcat(szWork, "%");
    cch = (cch + 1);

L_1d2a:
    if ((iPass != 1))
        goto L_1d4b;
    else
        goto L_1d33;

L_1d33:
    TextOut(hdc, xLeft, yTop, szWork, cch);

L_1d4b:
    xLeft = (xLeft + LOWORD(GetTextExtent(hdc, szWork, cch)));
    if ((i == 2))
        goto L_1d76;
    else
        goto L_1d69;

L_1d69:
    if ((i == 3))
        goto L_1d76;
    else
        goto L_1d72;

L_1d72:
    xLeft = (xLeft + 4);

L_1d76:
    cch = CchGetString(idsT, szWork);
    if ((iPass != 1))
        goto L_1da9;
    else
        goto L_1d91;

L_1d91:
    TextOut(hdc, xLeft, yTop, szWork, cch);

L_1da9:
    xLeft = (xLeft + LOWORD(GetTextExtent(hdc, szWork, cch)));
    cCur = (cCur + 1);
    ids = (ids + 1);
    j = (j + 1);

L_1dca:
    if ((j < 2))
        goto L_1c88;
    else
        goto L_1dd3;

L_1dd3:
    if ((iPass != 1))
        goto L_1dec;
    else
        goto L_1ddc;

L_1ddc:
    /* untranslated: yTop = (yTop + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 2)) */

L_1dec:
    iPass = (iPass + 1);

L_1df0:
    if ((iPass < 2))
        goto L_1b55;
    else
        goto L_1df9;

L_1df9:
    i = (i + 1);

L_1dfd:
    if ((i < 9))
        goto L_1b4d;
    else
        goto L_1e06;

L_1e06:
    return;
}

void DrawScoreReport(HDC hdc) {
    int16_t  dxDig;
    int16_t  yTop;
    POINT    pt;
    int16_t  dx45;
    StringId ids;
    COLORREF cr;
    int32_t  lMax;
    int16_t  j;
    int16_t  i;
    int16_t  iPass;
    char    *psz;
    int32_t  lVal;
    int16_t  cch;
    int16_t  xLeft;
    int32_t  l;
    StringId t_20ef;
    uint32_t t_merge_23b0_0001;
    uint32_t t_merge_23e2_0001;

L_1e0c:
    yTop = 88;
    SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, rghfontArial8[1]);
    dxDig = LOWORD(GetTextExtent(hdc, "9", 0x1));
    SelectObject(hdc, rghfontArial8[4]);
    /* untranslated: xLeft = ((sext16to32((loword((0x5 * dxDig)) - (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2))) / 2) +
     * vdxScoreX) */
    i = 0;
    goto L_1e88;

L_1e84:
    i = (i + 1);

L_1e88:
    if ((i >= game.cPlayer))
        goto L_1fd5;
    else
        goto L_1e93;

L_1e93:
    psz = PszPlayerName(i, 0x1, 0x1, 0x1, 0x0, 0x0);
    cch = strlen(psz);
    l = GetTextExtent(hdc, psz, cch);
    /* untranslated: ss:[bp-0x32] = LOWORD(l) */
    /* untranslated: ss:[bp-0x30] = 0x0 */
    dx45 = LOWORD(__ftol());
    if ((((rgplr[i].wMdPlr >> 0x8) & 0x1) == 0x0))
        goto L_1f42;
    else
        goto L_1f1c;

L_1f1c:
    if (((rgplr[i].wFlags & 0x1) == 0x0))
        goto L_1f42;
    else
        goto L_1f35;

L_1f35:
    cr = 0x7f7f7f;
    goto L_1f7e;

L_1f42:
    if ((((vlprgScoreX[i].wWord >> 0xe) & 0x1) == 0x0))
        goto L_1f74;
    else
        goto L_1f67;

L_1f67:
    cr = 0xff0000;
    goto L_1f7e;

L_1f74:
    cr = 0x0;

L_1f7e:
    SetTextColor(hdc, cr);
    /* untranslated: call TextOut(hdc, ((xLeft - dx45) + (words(loword((3 * dyArial8)), signhiword(loword((3 * dyArial8)))) / 0x2)), (((yTop - dx45) -
     * (sext16to32(dyArial8) / 2)) + 0xfffc), psz, cch) -> callresult(int16_t) */
    xLeft = (xLeft + LOWORD((0x5 * dxDig)));
    goto L_1e84;

L_1fd5:
    SelectObject(hdc, rghfontArial8[1]);
    SelectObject(hdc, hbrButtonShadow);
    ids = idsPlanets;
    xLeft = (vdxScoreX + 4);
    /* untranslated: pt.x = (sext16to32(((words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2) + 0xfff4)) / 2) */
    /* untranslated: pt.y = ((sext16to32(((words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2) + 0xfff5)) / 2) - (sext16to32(dyArial8)
     * / 4)) */
    i = 0;
    goto L_204a;

L_2046:
    i = (i + 1);

L_204a:
    if ((i > game.cPlayer))
        goto L_209d;
    else
        goto L_2055;

L_2055:
    /* untranslated: call PatBlt(hdc, xLeft, (yTop - (sext16to32(dyArial8) / 4)), 0x1, ((words(loword((loword((9 * dyArial8)) * 0x3)),
     * signhiword(loword((loword((9 * dyArial8)) * 0x3)))) / 0x2) + 0x1), PATCOPY) -> callresult(int16_t) */
    xLeft = (xLeft + LOWORD((0x5 * dxDig)));
    goto L_2046;

L_209d:
    SetTextColor(hdc, 0x0);
    i = 0;
    goto L_244f;

L_20b5:
    PatBlt(hdc, (vdxScoreX + 4), (yTop - ((uint32_t)(dyArial8) / 4)), LOWORD((LOWORD((game.cPlayer * dxDig)) * 0x5)), 0x1, PATCOPY);
    t_20ef = ids;
    ids = (ids + 1);
    cch = CchGetString(t_20ef, szWork);
    SetTextColor(hdc, 0x0);
    RightTextOut(hdc, vdxScoreX, yTop, szWork, cch, 0x0);
    xLeft = ((LOWORD((0x5 * dxDig)) + vdxScoreX) + 2);
    lMax = 0;
    iPass = 0;
    goto L_2432;

L_2151:
    j = 0;
    goto L_215d;

L_2159:
    j = (j + 1);

L_215d:
    if ((j >= game.cPlayer))
        goto L_242e;
    else
        goto L_2168;

L_2168:
    if ((((vlprgScoreX[j].wWord >> 0x5) & 0x1) == 0x0))
        goto L_22f8;
    else
        goto L_2192;

L_2192:
    goto L_22d2;
    lVal = (uint32_t)(vlprgScoreX[j].score.cPlanet);
    goto L_22f5;
    lVal = (uint32_t)(vlprgScoreX[j].score.cStarbase);
    goto L_22f5;
    /* untranslated: lVal = (int32_t)(words((vlprgScoreX[j].score.rgcsh[(i - 0x2)] & 0x1fff), 0x0) << ((vlprgScoreX[j].score.rgcsh[(i - 0x2)] >> 0xd) << 0x1))
     */
    goto L_22f5;
    lVal = (uint32_t)(vlprgScoreX[j].score.cTechLevels);
    goto L_22f5;
    lVal = vlprgScoreX[j].score.cResources;
    goto L_22f5;
    lVal = vlprgScoreX[j].score.lScore;
    goto L_22f5;
    lVal = (uint32_t)(HIWORD(vlprgScoreX[j]));
    goto L_2302;

L_22d2:

L_22f5:

L_22f8:
    lVal = -1;

L_2302:
    if ((iPass != 0))
        goto L_2335;
    else
        goto L_230b;

L_230b:
    if ((HIWORD(lVal) < HIWORD(lMax)))
        goto L_2419;
    else
        goto L_2319;

L_2319:
    if ((HIWORD(lVal) > HIWORD(lMax)))
        goto L_2326;
    else
        goto L_231e;

L_231e:
    if ((LOWORD(lVal) <= LOWORD(lMax)))
        goto L_2419;
    else
        goto L_2326;

L_2326:
    lMax = lVal;

L_2335:
    if ((HIWORD(lVal) < 0x0))
        goto L_2419;
    else
        goto L_233e;

L_233e:
    if ((HIWORD(lVal) > 0x0))
        goto L_234c;
    else
        goto L_2343;

L_2343:
    if ((LOWORD(lVal) < 0x0))
        goto L_2419;
    else
        goto L_234c;

L_234c:
    if ((((rgplr[j].wMdPlr >> 0x8) & 0x1) == 0x0))
        goto L_2383;
    else
        goto L_236a;

L_236a:
    if (((rgplr[j].wFlags & 0x1) != 0x0))
        goto L_2419;
    else
        goto L_2383;

L_2383:
    if ((i != 8))
        goto L_23ba;
    else
        goto L_238c;

L_238c:
    if ((LOWORD(lVal) != 0x1))
        goto L_23aa;
    else
        goto L_2398;

L_2398:
    if ((HIWORD(lVal) != 0x0))
        goto L_23aa;
    else
        goto L_23a1;

L_23a1:
    t_merge_23b0_0001 = 0xff0000;
    goto L_23b0;

L_23aa:
    t_merge_23b0_0001 = 0x0;

L_23b0:
    SetTextColor(hdc, t_merge_23b0_0001);
    goto L_23e9;

L_23ba:
    if ((LOWORD(lVal) != LOWORD(lMax)))
        goto L_23dc;
    else
        goto L_23cb;

L_23cb:
    if ((HIWORD(lVal) != HIWORD(lMax)))
        goto L_23dc;
    else
        goto L_23d3;

L_23d3:
    t_merge_23e2_0001 = 0xff0000;
    goto L_23e2;

L_23dc:
    t_merge_23e2_0001 = 0x0;

L_23e2:
    SetTextColor(hdc, t_merge_23e2_0001);

L_23e9:
    psz = PszFromLongK(lVal, &(cch));
    RightTextOut(hdc, xLeft, yTop, psz, cch, 0x0);

L_2419:
    if ((iPass != 1))
        goto L_2159;
    else
        goto L_2422;

L_2422:
    xLeft = (xLeft + LOWORD((0x5 * dxDig)));

L_242e:
    iPass = (iPass + 1);

L_2432:
    if ((iPass < 2))
        goto L_2151;
    else
        goto L_243b;

L_243b:
    /* untranslated: yTop = (yTop + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 2)) */
    i = (i + 1);

L_244f:
    if ((i < 9))
        goto L_20b5;
    else
        goto L_2458;

L_2458:
    PatBlt(hdc, (vdxScoreX + 4), (yTop - ((uint32_t)(dyArial8) / 4)), LOWORD((LOWORD((game.cPlayer * dxDig)) * 0x5)), 0x1, PATCOPY);
    return;
}

void DrawHistoryReport(HDC hdc) {
    char     szT[100];
    RECT     rcChart;
    uint16_t dYear;
    POINT    pt;
    int16_t  dy;
    int32_t  cYears;
    int32_t  cCur;
    uint16_t iYearBase;
    int16_t  j;
    int16_t  i;
    int16_t  yCur;
    int16_t  cDrawn;
    char    *psz;
    int16_t  dx;
    int32_t  cScaleMax;
    int16_t  xCur;
    int32_t  cInc;
    int16_t  cch;
    RECT     rcDiamond;
    RECT     rc;
    HPEN     hpenSav;
    HPEN     hpen;
    SCOREX  *lpsx;
    uint32_t t_merge_2d63_0001;
    uint32_t t_merge_2e05_0001;
    uint32_t t_merge_2f6e_0001;

L_2494:
    SetBkMode(hdc, TRANSPARENT);
    GetClientRect(hwndScoreXDlg, &(rcChart));
    ExpandRc(&(rcChart), 0xffd0, 0x0);
    rcChart.top = (dyArial10 + dyArial8);
    rcChart.bottom = (rcChart.bottom - (dyArial8 * 4));
    rc = rcChart;
    rc.top = ((uint32_t)(dyArial8) / 2);
    rc.bottom = (rc.top + dyArial10);
    cch = CchGetString(idsHistory, szT);
    psz = szT[cch];
    cch = CchGetString((gd.iCurGraph + 0x1b3), psz);
    psz[(cch - 1)] = 0;
    SelectObject(hdc, rghfontArial10[1]);
    SetTextColor(hdc, crButtonText);
    RcCtrTextOut(hdc, &(rc), szT, 0x0);
    SetRect(&(rcDiamond), 0xf, rc.top, (dyArial10 + 15), (rc.top + dyArial10));
    DrawDiamond(hdc, &(rcDiamond), hbrBBlue);
    dx = (rcChart.right - rcChart.left);
    dy = (rcChart.bottom - rcChart.top);
    PatBlt(hdc, rcChart.left, rcChart.top, dx, dy, BLACKNESS);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, (rcChart.left - 1), rcChart.bottom, (dx + 3), 0x1, PATCOPY);
    PatBlt(hdc, (rcChart.left - 2), (rcChart.bottom + 1), (dx + 4), 0x1, PATCOPY);
    PatBlt(hdc, rcChart.right, (rcChart.top - 1), 0x1, (dy + 3), PATCOPY);
    PatBlt(hdc, (rcChart.right + 1), (rcChart.top - 2), 0x1, (dy + 4), PATCOPY);
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, (rcChart.left - 2), (rcChart.top - 2), (dx + 3), 0x1, PATCOPY);
    PatBlt(hdc, (rcChart.left - 2), (rcChart.top - 1), (dx + 2), 0x1, PATCOPY);
    PatBlt(hdc, (rcChart.left - 2), (rcChart.top - 2), 0x1, (dy + 3), PATCOPY);
    PatBlt(hdc, (rcChart.left - 1), (rcChart.top - 2), 0x1, (dy + 2), PATCOPY);
    dx = (dx - 4);
    dy = (dy - 4);
    ExpandRc(&(rcChart), 0xfffe, 0xfffe);
    if ((game.turn > 0x64))
        goto L_277e;
    else
        goto L_2776;

L_2776:
    iYearBase = 0x0;
    goto L_2787;

L_277e:
    iYearBase = (game.turn + 0xff9c);

L_2787:
    /* untranslated: cYears = (uint32_t)((int32_t)(words(0x0, (game.turn + 0x4)) / 0x5) * 0x5) */
    if ((HIWORD(cYears) < 0x0))
        goto L_27d8;
    else
        goto L_27bd;

L_27bd:
    if ((HIWORD(cYears) > 0x0))
        goto L_27cb;
    else
        goto L_27c2;

L_27c2:
    if ((LOWORD(cYears) <= 0x64))
        goto L_27d8;
    else
        goto L_27cb;

L_27cb:
    cYears = 100;
    goto L_281f;

L_27d8:
    if ((HIWORD(cYears) < 0x0))
        goto L_281f;
    else
        goto L_27e1;

L_27e1:
    if ((HIWORD(cYears) > 0x0))
        goto L_27ef;
    else
        goto L_27e6;

L_27e6:
    if ((LOWORD(cYears) <= 0x32))
        goto L_281f;
    else
        goto L_27ef;

L_27ef:
    /* untranslated: cYears = (uint32_t)((int32_t)(words((HIWORD(cYears) + 0x0), (LOWORD(cYears) + 0x5)) / 0xa) * 0xa) */

L_281f:
    xCur = rcChart.left;
    SelectObject(hdc, rghfontArial8[1]);
    if ((HIWORD(cYears) > 0x0))
        goto L_2852;
    else
        goto L_283b;

L_283b:
    if ((HIWORD(cYears) < 0x0))
        goto L_2849;
    else
        goto L_2840;

L_2840:
    if ((LOWORD(cYears) > 0x32))
        goto L_2852;
    else
        goto L_2849;

L_2849:
    j = 5;
    goto L_2858;

L_2852:
    j = 10;

L_2858:
    /* untranslated: cDrawn = (words(LOWORD(cYears), signhiword(LOWORD(cYears))) / j) */
    if ((cDrawn <= 0))
        goto L_2927;
    else
        goto L_2871;

L_2871:
    i = 0;
    goto L_291a;

L_287a:
    /* untranslated: xCur = ((words(loword((dx * i)), signhiword(loword((dx * i)))) / cDrawn) + rcChart.left) */
    cch = _wsprintf(szT, PCTD, ((iYearBase + 0x960) + LOWORD((i * j))));
    CtrTextOut(hdc, xCur, (rcChart.bottom + 6), szT, cch);
    if ((i <= 0))
        goto L_2915;
    else
        goto L_28e2;

L_28e2:
    if ((i >= cDrawn))
        goto L_2915;
    else
        goto L_28ef;

L_28ef:
    PatBlt(hdc, xCur, (rcChart.top - 2), 0x1, (dy + 4), PATCOPY);

L_2915:
    i = (i + 1);

L_291a:
    if ((i <= cDrawn))
        goto L_287a;
    else
        goto L_2927;

L_2927:
    cScaleMax = -1;
    i = 0;
    goto L_2941;

L_293c:
    i = (i + 1);

L_2941:
    if ((i >= game.cPlayer))
        goto L_29eb;
    else
        goto L_294d;

L_294d:
    if ((LOWORD(rgsxPlr[i]) != 0x0))
        goto L_2969;
    else
        goto L_295f;

L_295f:
    if ((HIWORD(rgsxPlr[i]) == 0x0))
        goto L_293c;
    else
        goto L_2969;

L_2969:
    j = 0;
    goto L_2977;

L_2972:
    j = (j + 1);

L_2977:
    if ((j >= rgcsxPlr[i]))
        goto L_293c;
    else
        goto L_298a;

L_298a:
    cCur = LFetchScoreXVal(&(rgsxPlr[i][j]), gd.iCurGraph);
    if ((HIWORD(cCur) < HIWORD(cScaleMax)))
        goto L_2972;
    else
        goto L_29ca;

L_29ca:
    if ((HIWORD(cCur) > HIWORD(cScaleMax)))
        goto L_29d7;
    else
        goto L_29cf;

L_29cf:
    if ((LOWORD(cCur) <= LOWORD(cScaleMax)))
        goto L_2972;
    else
        goto L_29d7;

L_29d7:
    cScaleMax = cCur;

L_29eb:
    if ((HIWORD(cScaleMax) > 0x0))
        goto L_2a07;
    else
        goto L_29f5;

L_29f5:
    if ((HIWORD(cScaleMax) < 0x0))
        goto L_2f8d;
    else
        goto L_29fa;

L_29fa:
    if ((LOWORD(cScaleMax) < 0x0))
        goto L_2f8d;
    else
        goto L_2a01;

L_2a01:

L_2a07:
    if ((HIWORD(cScaleMax) > 0x0))
        goto L_2a2c;
    else
        goto L_2a11;

L_2a11:
    if ((HIWORD(cScaleMax) < 0x0))
        goto L_2a20;
    else
        goto L_2a16;

L_2a16:
    if ((LOWORD(cScaleMax) >= 0x5))
        goto L_2a2c;
    else
        goto L_2a20;

L_2a20:
    cScaleMax = 5;

L_2a2c:
    if ((HIWORD(cScaleMax) > 0x0))
        goto L_2a54;
    else
        goto L_2a36;

L_2a36:
    if ((HIWORD(cScaleMax) < 0x0))
        goto L_2a45;
    else
        goto L_2a3b;

L_2a3b:
    if ((LOWORD(cScaleMax) > 0xc))
        goto L_2a54;
    else
        goto L_2a45;

L_2a45:
    cInc = 1;
    goto L_2bd4;

L_2a54:
    if ((HIWORD(cScaleMax) > 0x0))
        goto L_2a7c;
    else
        goto L_2a5e;

L_2a5e:
    if ((HIWORD(cScaleMax) < 0x0))
        goto L_2a6d;
    else
        goto L_2a63;

L_2a63:
    if ((LOWORD(cScaleMax) > 0x19))
        goto L_2a7c;
    else
        goto L_2a6d;

L_2a6d:
    cInc = 2;
    goto L_2bd4;

L_2a7c:
    if ((HIWORD(cScaleMax) > 0x0))
        goto L_2aa4;
    else
        goto L_2a86;

L_2a86:
    if ((HIWORD(cScaleMax) < 0x0))
        goto L_2a95;
    else
        goto L_2a8b;

L_2a8b:
    if ((LOWORD(cScaleMax) > 0x3c))
        goto L_2aa4;
    else
        goto L_2a95;

L_2a95:
    cInc = 5;
    goto L_2bd4;

L_2aa4:
    if ((HIWORD(cScaleMax) > 0x0))
        goto L_2acc;
    else
        goto L_2aae;

L_2aae:
    if ((HIWORD(cScaleMax) < 0x0))
        goto L_2abd;
    else
        goto L_2ab3;

L_2ab3:
    if ((LOWORD(cScaleMax) > 0x78))
        goto L_2acc;
    else
        goto L_2abd;

L_2abd:
    cInc = 10;
    goto L_2bd4;

L_2acc:
    if ((HIWORD(cScaleMax) > 0x0))
        goto L_2af5;
    else
        goto L_2ad6;

L_2ad6:
    if ((HIWORD(cScaleMax) < 0x0))
        goto L_2ae6;
    else
        goto L_2adb;

L_2adb:
    if ((LOWORD(cScaleMax) > 0x12c))
        goto L_2af5;
    else
        goto L_2ae6;

L_2ae6:
    cInc = 25;
    goto L_2bd4;

L_2af5:
    if ((HIWORD(cScaleMax) > 0x0))
        goto L_2b1e;
    else
        goto L_2aff;

L_2aff:
    if ((HIWORD(cScaleMax) < 0x0))
        goto L_2b0f;
    else
        goto L_2b04;

L_2b04:
    if ((LOWORD(cScaleMax) > 0x258))
        goto L_2b1e;
    else
        goto L_2b0f;

L_2b0f:
    cInc = 50;
    goto L_2bd4;

L_2b1e:
    if ((HIWORD(cScaleMax) > 0x0))
        goto L_2b47;
    else
        goto L_2b28;

L_2b28:
    if ((HIWORD(cScaleMax) < 0x0))
        goto L_2b38;
    else
        goto L_2b2d;

L_2b2d:
    if ((LOWORD(cScaleMax) > 0x4b0))
        goto L_2b47;
    else
        goto L_2b38;

L_2b38:
    cInc = 100;
    goto L_2bd4;

L_2b47:
    if ((HIWORD(cScaleMax) > 0x0))
        goto L_2b70;
    else
        goto L_2b51;

L_2b51:
    if ((HIWORD(cScaleMax) < 0x0))
        goto L_2b61;
    else
        goto L_2b56;

L_2b56:
    if ((LOWORD(cScaleMax) > 0x1770))
        goto L_2b70;
    else
        goto L_2b61;

L_2b61:
    cInc = 500;
    goto L_2bd4;

L_2b70:
    if ((HIWORD(cScaleMax) > 0x0))
        goto L_2b99;
    else
        goto L_2b7a;

L_2b7a:
    if ((HIWORD(cScaleMax) < 0x0))
        goto L_2b8a;
    else
        goto L_2b7f;

L_2b7f:
    if ((LOWORD(cScaleMax) > 0x2ee0))
        goto L_2b99;
    else
        goto L_2b8a;

L_2b8a:
    cInc = 1000;
    goto L_2bd4;

L_2b99:
    cInc = (uint32_t)(((int32_t)(((int32_t)((cScaleMax / 12)) / 0x1f4)) * 0x1f4));

L_2bd4:
    xCur = (rcChart.left - 6);
    cCur = cInc;
    goto L_2cb0;

L_2bef:
    yCur = (rcChart.bottom - LOWORD((int32_t)(((uint32_t)(((uint32_t)(dy)*cCur)) / cScaleMax))));
    if ((yCur < (((uint32_t)(dyArial8) / 2) + rcChart.top)))
        goto L_2ccd;
    else
        goto L_2c2a;

L_2c2a:

L_2c30:
    cch = _wsprintf(szWork, PCTLD, LOWORD(cCur), HIWORD(cCur));
    RightTextOut(hdc, xCur, (yCur - ((uint32_t)(dyArial8) / 2)), szWork, cch, 0x0);
    PatBlt(hdc, (rcChart.left - 2), yCur, (dx + 4), 0x1, PATCOPY);
    cCur = (cCur + cInc);

L_2cb0:
    if ((HIWORD(cCur) < HIWORD(cScaleMax)))
        goto L_2bef;
    else
        goto L_2cc0;

L_2cc0:
    if ((HIWORD(cCur) > HIWORD(cScaleMax)))
        goto L_2ccd;
    else
        goto L_2cc5;

L_2cc5:
    if ((LOWORD(cCur) < LOWORD(cScaleMax)))
        goto L_2bef;
    else
        goto L_2ccd;

L_2ccd:
    xCur = (rcChart.left + 6);
    yCur = (rcChart.top + 6);
    i = 0;
    goto L_2cef;

L_2cea:
    i = (i + 1);

L_2cef:
    if ((i >= game.cPlayer))
        goto L_2d99;
    else
        goto L_2cfb;

L_2cfb:
    if ((LOWORD(rgsxPlr[i]) != 0x0))
        goto L_2d17;
    else
        goto L_2d0d;

L_2d0d:
    if ((HIWORD(rgsxPlr[i]) == 0x0))
        goto L_2cea;
    else
        goto L_2d17;

L_2d17:
    psz = PszPlayerName(i, 0x1, 0x1, 0x0, 0x0, 0x0);
    if ((i != idPlayer))
        goto L_2d53;
    else
        goto L_2d4a;

L_2d4a:
    t_merge_2d63_0001 = 0xffffff;
    goto L_2d63;

L_2d53:
    t_merge_2d63_0001 = rgcrPlrHistory[i];

L_2d63:
    SetTextColor(hdc, t_merge_2d63_0001);
    TextOut(hdc, xCur, yCur, psz, strlen(psz));
    yCur = (yCur + dyArial8);

L_2d99:
    i = 0;
    goto L_2da7;

L_2da2:
    i = (i + 1);

L_2da7:
    if ((i >= game.cPlayer))
        goto L_2f8d;
    else
        goto L_2db3;

L_2db3:
    if ((LOWORD(rgsxPlr[i]) != 0x0))
        goto L_2dd2;
    else
        goto L_2dc5;

L_2dc5:
    if ((HIWORD(rgsxPlr[i]) == 0x0))
        goto L_2da2;
    else
        goto L_2dcc;

L_2dcc:

L_2dd2:
    cDrawn = 0;
    if ((i != idPlayer))
        goto L_2df5;
    else
        goto L_2dec;

L_2dec:
    t_merge_2e05_0001 = 0xffffff;
    goto L_2e05;

L_2df5:
    t_merge_2e05_0001 = rgcrPlrHistory[i];

L_2e05:
    hpen = CreatePen(0x0, 0x1, t_merge_2e05_0001);
    hpenSav = SelectObject(hdc, hpen);
    j = 0;
    goto L_2e2e;

L_2e29:
    j = (j + 1);

L_2e2e:
    if ((j >= rgcsxPlr[i]))
        goto L_2f36;
    else
        goto L_2e41;

L_2e41:
    lpsx = &(rgsxPlr[i][j]);
    if ((*(lpsx + 0x2) < iYearBase))
        goto L_2e29;
    else
        goto L_2e72;

L_2e72:
    if ((*(lpsx + 0x2) > (LOWORD(cYears) + iYearBase)))
        goto L_2e29;
    else
        goto L_2e85;

L_2e85:

L_2e8b:
    dYear = (*(lpsx + 0x2) - iYearBase);
    cCur = LFetchScoreXVal(lpsx, gd.iCurGraph);
    /* untranslated: pt.x = (loword((int32_t)((uint32_t)(words(0x0, dYear) * sext16to32(dx)) / cYears)) + rcChart.left) */
    pt.y = (rcChart.bottom - LOWORD((int32_t)(((uint32_t)((cCur * (uint32_t)(dy))) / cScaleMax))));
    if ((cDrawn != 0))
        goto L_2f20;
    else
        goto L_2f0f;

L_2f0f:
    MoveTo(hdc, pt.x, pt.y);
    goto L_2f2e;

L_2f20:
    LineTo(hdc, pt.x, pt.y);

L_2f2e:
    cDrawn = (cDrawn + 1);
    goto L_2e29;

L_2f36:
    if ((cDrawn != 1))
        goto L_2f75;
    else
        goto L_2f40;

L_2f40:
    if ((i != idPlayer))
        goto L_2f5e;
    else
        goto L_2f55;

L_2f55:
    t_merge_2f6e_0001 = 0xffffff;
    goto L_2f6e;

L_2f5e:
    t_merge_2f6e_0001 = rgcrPlrHistory[i];

L_2f6e:
    SetPixel(hdc, pt.x, pt.y, t_merge_2f6e_0001);

L_2f75:
    SelectObject(hdc, hpenSav);
    DeleteObject(hpen);
    goto L_2da2;

L_2f8d:
    return;
}

int32_t LFetchScoreXVal(SCOREX *lpsx, int16_t iVal) {
L_2f94:
    goto L_302e;
    goto L_3058;
    goto L_3058;
    goto L_3058;
    goto L_3058;
    goto L_3058;
    goto L_3058;

L_302e:

L_3058:
    return lpsx->score.lScore;
}

int16_t DxReportColHdr(int16_t irpt, int16_t iCol, char *psz, HDC hdc) {
    char     szT[40];
    StringId ids;
    int16_t  dxDigit;
    int16_t  dx;
    int16_t  cch;
    int16_t  dx2;

L_305e:
    szT[0] = 56;
    dxDigit = LOWORD(GetTextExtent(hdc, szT, 0x1));
    goto L_335a;

L_3087:
    *(psz) = 0;
    return 0x0;

L_3093:
    cch = CchGetString((iCol + 1113), psz);
    dx = LOWORD(GetTextExtent(hdc, psz, cch));
    goto L_3168;
    dx2 = LOWORD((0xf * dxDigit));
    goto DxChk;
    dx = (dx * 2);
    goto L_3197;
    dx = (LOWORD((0x3 * dx)) + 20);
    goto L_3197;
    ids = idsN100100;

ChkAltString:
    cch = CchGetString(ids, szT);
    dx2 = LOWORD(GetTextExtent(hdc, szT, cch));

DxChk:
    if ((dx2 <= dx))
        goto L_3126;
    else
        goto L_3120;

L_3120:
    dx = dx2;

L_3126:
    goto L_3197;
    ids = idsN10001000;
    goto ChkAltString;
    dx2 = LOWORD((0x5 * dxDigit));
    goto DxChk;
    dx2 = LOWORD((0xb * dxDigit));
    goto DxChk;
    dx2 = LOWORD((0xe * dxDigit));
    goto DxChk;
    dx2 = ((dxDigit * 4) + 2);
    goto DxChk;

L_3168:

L_3197:

L_319a:
    cch = CchGetString((iCol + 1138), psz);
    dx = LOWORD(GetTextExtent(hdc, psz, cch));
    goto L_324e;
    dx = ((dx * 2) + ((uint32_t)(dx) / 2));
    goto L_3277;
    dx = (dx * 2);
    goto L_3277;
    dx = ((dx * 2) - dxDigit);
    goto L_3277;
    dx = ((dx * 4) + ((uint32_t)(dx) / 2));
    goto L_3277;
    dx2 = LOWORD((0x5 * dxDigit));
    if ((dx2 <= dx))
        goto L_322b;
    else
        goto L_3225;

L_3225:
    dx = dx2;

L_322b:
    goto L_3277;
    dx2 = LOWORD((0x13 * dxDigit));
    if ((dx2 <= dx))
        goto L_337d;
    else
        goto L_3242;

L_3242:
    dx = dx2;

L_324e:

L_3277:

L_327a:
    cch = CchGetString((iCol + 1150), psz);
    dx = LOWORD(GetTextExtent(hdc, psz, cch));
    goto L_32da;

L_32aa:
    dx = ((dx * 2) + ((uint32_t)(dx) / 2));
    goto L_337d;

L_32c0:
    dx = LOWORD((0x3 * dx));
    goto L_337d;

L_32cc:
    dx = (dx * 2);
    goto L_337d;

L_32da:
    if ((iCol == 0))
        goto L_32c0;
    else
        goto L_32e2;

L_32e2:
    if ((iCol == 1))
        goto L_32cc;
    else
        goto L_32ea;

L_32ea:
    if ((iCol == 2))
        goto L_32aa;
    else
        goto L_32f2;

L_32f2:
    if ((iCol != 5))
        goto L_337d;
    else
        goto L_32f7;

L_32f7:

L_3300:
    cch = CchGetString((iCol + 1162), psz);
    dx = LOWORD(GetTextExtent(hdc, psz, cch));
    goto L_3349;

L_3330:
    dx = ((dx * 2) + ((uint32_t)(dx) / 2));
    goto L_337d;

L_3349:
    if ((iCol != 0))
        goto L_337d;
    else
        goto L_334e;

L_334e:

L_335a:
    if ((irpt == 0))
        goto L_3093;
    else
        goto L_3362;

L_3362:
    if ((irpt == 1))
        goto L_319a;
    else
        goto L_336a;

L_336a:
    if ((irpt == 2))
        goto L_327a;
    else
        goto L_3372;

L_3372:
    if ((irpt != 3))
        goto L_3087;
    else
        goto L_3377;

L_3377:

L_337d:
    dx = (dx + 5);
    return (((uint32_t)((dx + 1)) / 0x2) * 0x2);
}

void DrawReportItem(HDC hdc, RECT *prc, int16_t irpt, int16_t irow, int16_t icol) {
    BTLDATA *lpbd;
    char     szT[100];
    char     chT;
    char    *lpsz;
    PLANET  *lppl;
    int16_t  j;
    int16_t  i;
    FLEET   *lpfl;
    int16_t  dx;
    char    *psz;
    int16_t  xCur;
    int16_t  cch;
    int32_t  l;
    HBRUSH   hbr;
    RECT     rc;
    float    pct;
    int32_t  rgl[4];
    int16_t  iItem;
    PLANET   pl;
    int16_t  fEnough;
    int32_t  t_call_3758;
    uint32_t t_merge_39e1_0001;
    uint32_t t_merge_3a8e_0001;
    uint32_t t_merge_3aff_0001;
    int32_t  t_call_4201;
    uint16_t t_merge_422c_0001;
    int32_t  t_call_4669;
    int32_t  t_call_4692;
    uint16_t t_merge_476d_0001;
    uint16_t t_merge_47df_0001;

L_3398:
    szT[0] = 56;
    dx = LOWORD(GetTextExtent(hdc, szT, 0x1));
    goto L_4e92;

L_33c4:
    lppl = &(lpPlanets[vlprgidPlanet[irow]]);
    SelectObject(hdc, rghfontArial8[1]);
    SetTextColor(hdc, 0x0);
    SetBkMode(hdc, TRANSPARENT);
    goto L_3e32;
    if ((lppl->fStarbase != 0x0))
        goto L_343e;
    else
        goto L_3430;

L_3430:
    lpsz = szDblDash;
    goto L_346a;

L_343e:
    lpsz = rglpshdefSB[idPlayer][lppl->isb].hul.szClass;

L_346a:
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, lpsz, fstrlen(lpsz), 0x0);
    goto L_3e61;
    if ((lppl->idRoute != 0x0))
        goto L_34c3;
    else
        goto L_34ba;

L_34ba:
    psz = szDblDash;
    goto L_34dc;

L_34c3:
    psz = PszGetPlanetName((lppl->idRoute + 0xffff));

L_34dc:
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
    goto L_3e61;
    if ((lppl->idFling != 0x0))
        goto L_3533;
    else
        goto L_352a;

L_352a:
    psz = szDblDash;
    goto L_354c;

L_3533:
    psz = PszGetPlanetName((lppl->idFling + 0xffff));

L_354c:
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
    goto L_3e61;
    if ((sel.grobj != grobjPlanet))
        goto L_35b1;
    else
        goto L_3592;

L_3592:
    if ((sel.pl.id != lppl->id))
        goto L_35b1;
    else
        goto L_35a1;

L_35a1:
    SetTextColor(hdc, 0x7f);

L_35b1:
    psz = PszGetPlanetName(lppl->id);
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
    if ((lppl->fStarbase == 0x0))
        goto L_3748;
    else
        goto L_3612;

L_3612:
    rc = *(prc);
    dx = (((uint32_t)((rc.bottom - rc.top)) / 3) - 1);
    rc.left = (rc.right - dx);
    if ((LphuldefFromId(rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef)->hul.wtCargoMax == 0x0))
        goto L_3688;
    else
        goto L_367e;

L_367e:
    hbr = hbrYellow;
    goto L_368f;

L_3688:
    hbr = hbrBlue;

L_368f:
    rc.top = prc->top;
    rc.bottom = (rc.top + dx);
    FillRect(hdc, &(rc), hbr);
    if ((IWarpMAFromLppl(lppl, 0x0) <= 0))
        goto L_3701;
    else
        goto L_36d2;

L_36d2:
    rc.top = ((prc->top + dx) + 1);
    rc.bottom = (rc.top + dx);
    FillRect(hdc, &(rc), hbrPurple);

L_3701:
    if ((IStargateFromLppl(lppl) == -1))
        goto L_3748;
    else
        goto L_3717;

L_3717:
    rc.top = (((dx + 1) * 2) + prc->top);
    rc.bottom = (rc.top + dx);
    FillRect(hdc, &(rc), hbrGreen);

L_3748:
    goto L_3e61;
    t_call_3758 = CalcPlanetMaxPop(lppl->id, lppl->iPlayer);
    if ((HIWORD(t_call_3758) > HIWORD(lppl->rgwtMin[0x3])))
        goto L_378a;
    else
        goto L_376c;

L_376c:
    if ((HIWORD(t_call_3758) < HIWORD(lppl->rgwtMin[0x3])))
        goto L_377a;
    else
        goto L_3771;

L_3771:
    if ((LOWORD(t_call_3758) >= LOWORD(lppl->rgwtMin[0x3])))
        goto L_378a;
    else
        goto L_377a;

L_377a:
    SetTextColor(hdc, 0xff);

L_378a:
    cch = CommaFormatLong(szT, (uint32_t)((lppl->rgwtMin[3] * 100)));
    RightTextOut(hdc, prc->right, prc->top, szT, cch, 0x0);
    goto L_3e61;
    cch = _wsprintf(szT, PCTDPCTPCT, PctPlanetCapacity(lppl));
    RightTextOut(hdc, prc->right, prc->top, szT, cch, 0x0);
    goto L_3e61;
    i = lppl->cDefenses;
    j = CMaxOperableDefenses(lppl, idPlayer, 0x0);
    if ((i <= 0))
        goto L_3921;
    else
        goto L_385f;

L_385f:
    CalcPctSurvive(lppl, &(pct), 0x0);
    pct = (1 - pct);
    /* untranslated: ss:[bp-0x94] = 0x64 */
    /* untranslated: ss:[bp-0x92] = 0x0 */
    __ftol();
    /* untranslated: ss:[bp-0x9c] = loword(callresult(int32_t)) */
    /* untranslated: ss:[bp-0x9a] = signhiword(loword(callresult(int32_t))) */
    /* untranslated: ss:[bp-0xa4] = 0x2710 */
    /* untranslated: ss:[bp-0xa2] = 0x0 */
    /* untranslated: ss:[bp-0xac] = 0x64 */
    /* untranslated: ss:[bp-0xaa] = 0x0 */
    cch = _wsprintf(szT, PCTDXPCTDPCTPCT, LOWORD(__ftol()), LOWORD(__ftol()));
    goto L_3933;

L_3921:
    szT[2] = 45;
    szT[1] = 45;
    szT[0] = 45;
    cch = 3;

L_3933:
    goto DrawPlusDef;
    /* untranslated: i = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff) */
    j = CMaxOperableFactories(lppl, idPlayer, 0x0);
    goto DrawMineFact;
    /* untranslated: i = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff) */
    j = CMaxOperableMines(lppl, idPlayer, 0x0);

DrawMineFact:
    cch = CommaFormatLong(szT, (uint32_t)(i));

DrawPlusDef:
    if ((i < j))
        goto L_39e8;
    else
        goto L_39c4;

L_39c4:
    if ((i != j))
        goto L_39db;
    else
        goto L_39d2;

L_39d2:
    t_merge_39e1_0001 = 0x7f00;
    goto L_39e1;

L_39db:
    t_merge_39e1_0001 = 0xff;

L_39e1:
    SetTextColor(hdc, t_merge_39e1_0001);

L_39e8:
    RightTextOut(hdc, prc->right, prc->top, szT, cch, 0x0);
    goto L_3e61;
    cch = CchGetString(idsN100, szT);
    dx = LOWORD(GetTextExtent(hdc, szT, cch));
    i = PctPlanetDesirability(lppl, idPlayer);
    cch = _wsprintf(szT, PCTDPCTPCT, i);
    if ((i > 10))
        goto L_3a95;
    else
        goto L_3a73;

L_3a73:
    if ((i >= 0))
        goto L_3a88;
    else
        goto L_3a7f;

L_3a7f:
    t_merge_3a8e_0001 = 0xff;
    goto L_3a8e;

L_3a88:
    t_merge_3a8e_0001 = 0x7f7f;

L_3a8e:
    SetTextColor(hdc, t_merge_3a8e_0001);

L_3a95:
    RightTextOut(hdc, (prc->left + dx), prc->top, szT, cch, 0x0);
    j = PctPlanetOptValue(lppl, idPlayer);
    if ((j <= i))
        goto L_3b46;
    else
        goto L_3adb;

L_3adb:
    if ((j <= 0))
        goto L_3b06;
    else
        goto L_3ae4;

L_3ae4:
    if ((j > 10))
        goto L_3af9;
    else
        goto L_3af0;

L_3af0:
    t_merge_3aff_0001 = 0x7f7f;
    goto L_3aff;

L_3af9:
    t_merge_3aff_0001 = 0x0;

L_3aff:
    SetTextColor(hdc, t_merge_3aff_0001);

L_3b06:
    cch = _wsprintf(szT, "(%d%%)", j);
    RightTextOut(hdc, prc->right, prc->top, szT, cch, 0x0);

L_3b46:
    goto L_3e61;
    xCur = ((dx * 4) + prc->left);
    if ((icol != 10))
        goto L_3b88;
    else
        goto L_3b62;

L_3b62:
    xCur = (xCur - dx);
    EstMineralsMined(lppl, rgl, 0xffffffff, 0x0);

L_3b88:
    i = 0;
    goto L_3c20;

L_3b90:
    if ((icol != 9))
        goto L_3bc1;
    else
        goto L_3b99;

L_3b99:
    l = lppl->rgwtMin[i];
    goto L_3bdb;

L_3bc1:
    l = rgl[i];

L_3bdb:
    DrawMineralItem(hdc, xCur, prc->top, i, l);
    if ((icol != 10))
        goto L_3c12;
    else
        goto L_3c04;

L_3c04:
    xCur = (xCur + (dx * 4));
    goto L_3c1c;

L_3c12:
    xCur = (xCur + LOWORD((0x5 * dx)));

L_3c1c:
    i = (i + 1);

L_3c20:
    if ((i < 3))
        goto L_3b90;
    else
        goto L_3c29;

L_3c29:
    goto L_3e61;
    xCur = (LOWORD((0x3 * dx)) + prc->left);
    i = 0;
    goto L_3c8d;

L_3c43:
    iItem = lppl->rgMinConc[i];
    DrawMineralItem(hdc, xCur, prc->top, i, (uint32_t)(iItem));
    xCur = (xCur + (dx * 4));
    i = (i + 1);

L_3c8d:
    if ((i < 3))
        goto L_3c43;
    else
        goto L_3c96;

L_3c96:
    goto L_3e61;
    pl = sel.pl;
    sel.pl = *(lppl);
    FillPlanetProdLB(0x0, 0x0, lppl);
    sel.pl = pl;
    DrawProductionItem(hdc, prc, szWork, 0x0, 0x0, 0x2);
    goto L_3e61;
    cch = CchGetString(idsN1000, szT);
    dx = LOWORD(GetTextExtent(hdc, szT, cch));
    CResourcesAtPlanet(lppl, idPlayer);
    /* untranslated: j = callresult(int16_t) */
    /* untranslated: i = callresult(int16_t) */
    /* untranslated: branch (loword((uint32_t)(words(*(lppl+0x18), *(lppl+0x1a)) >> 0x17)) & 0x1) != 0x0 ? L_3d99 : L_3d71 */

L_3d71:
    if ((0x0 != 0x0))
        goto L_3d99;
    else
        goto L_3d79;

L_3d79:
    i = (i - MulDiv(i, (uint16_t)(rgplr[idPlayer].pctResearch), 0x64));

L_3d99:
    CchGetString(idsD4, szT);
    cch = _wsprintf(szWork, szT, i);
    RightTextOut(hdc, (prc->left + dx), prc->top, szWork, cch, 0x0);
    cch = _wsprintf(szT, PCTD, j);
    RightTextOut(hdc, prc->right, prc->top, szT, cch, 0x0);
    goto L_4eb8;

L_3e32:

L_3e61:

L_3e64:
    lpfl = rglpfl[vlprgidFleet[irow]];
    SelectObject(hdc, rghfontArial8[1]);
    SetTextColor(hdc, 0x0);
    SetBkMode(hdc, TRANSPARENT);
    goto L_44d7;
    i = lpfl->iplan;
    fstrcpy(szT, rglpbtlplan[lpfl->iplr][lpfl->iplan].szName);
    psz = szT;
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
    goto L_4500;
    if ((sel.grobj != grobjFleet))
        goto L_3f7c;
    else
        goto L_3f5d;

L_3f5d:
    if ((sel.fl.id != lpfl->id))
        goto L_3f7c;
    else
        goto L_3f6c;

L_3f6c:
    SetTextColor(hdc, 0x7f);

L_3f7c:
    psz = PszGetFleetName(lpfl->id);
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
    goto L_4500;
    if ((lpfl->idPlanet == -1))
        goto L_3feb;
    else
        goto L_3fd6;

L_3fd6:
    psz = PszGetPlanetName(lpfl->idPlanet);
    goto L_401e;

L_3feb:
    psz = szT;
    _wsprintf(psz, PszGetCompressedString(idsSpaceDD), lpfl->pt.x, lpfl->pt.y);

L_401e:
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
    goto L_4500;
    psz = PszGetDestName(lpfl, hdc);
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
    goto L_4500;
    psz = PszGetTaskName(lpfl, &(i));
    if ((i == -1))
        goto L_40df;
    else
        goto L_40c8;

L_40c8:
    SetTextColor(hdc, rgcrMinerals[i]);

L_40df:
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
    goto L_4500;
    psz = PszGetETA(hdc, lpfl, 0x0);
    RightTextOut(hdc, prc->right, prc->top, psz, strlen(psz), 0x0);
    goto L_4500;
    xCur = ((dx * 4) + prc->left);
    i = 0;
    goto L_41bb;

L_4178:
    DrawMineralItem(hdc, xCur, prc->top, i, lpfl->rgwtMin[i]);
    xCur = (xCur + LOWORD((0x5 * dx)));
    i = (i + 1);

L_41bb:
    if ((i <= 3))
        goto L_4178;
    else
        goto L_41c4;

L_41c4:
    goto L_4500;
    if ((lpfl->cord <= 1))
        goto L_4223;
    else
        goto L_41d4;

L_41d4:
    t_call_4201 = EstFuelUse(lpfl, 0x0, lpfl->lpplord->rgord[1].iWarp, 0xffffffff, 0x0);
    if ((HIWORD(lpfl->rgwtMin[0x4]) < HIWORD(t_call_4201)))
        goto L_4229;
    else
        goto L_4215;

L_4215:
    if ((HIWORD(lpfl->rgwtMin[0x4]) > HIWORD(t_call_4201)))
        goto L_4223;
    else
        goto L_421a;

L_421a:
    if ((LOWORD(lpfl->rgwtMin[0x4]) < LOWORD(t_call_4201)))
        goto L_4229;
    else
        goto L_4223;

L_4223:
    t_merge_422c_0001 = 0x1;
    goto L_422c;

L_4229:
    t_merge_422c_0001 = 0x0;

L_422c:
    fEnough = t_merge_422c_0001;
    /* untranslated: call DrawMineralItem(hdc, ((dx * 4) + prc->left), prc->top, (fEnough neg 0), lpfl->rgwtMin[4]) -> callresult(void) */
    goto L_4500;
    i = IshdefPrimaryFromLpfl(lpfl, &(j));
    /* untranslated: branch HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (i * 2))] == 0x0 ? L_42a5 : L_4295 */

L_4295:
    SetTextColor(hdc, 0xff);

L_42a5:
    prc->right = (prc->right - LOWORD((0x6 * dx)));
    psz = rgshdef[i].hul.szClass;
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
    psz = szT;
    cch = _wsprintf(psz, PCTD, lpfl->rgcsh[i]);
    chT = 43;
    RightTextOut(hdc, ((LOWORD((6 * dx)) + prc->right) - LOWORD(GetTextExtent(hdc, &(chT), 0x1))), prc->top, psz, cch, 0x0);
    if ((j <= 1))
        goto L_43ca;
    else
        goto L_4387;

L_4387:
    TextOut(hdc, ((LOWORD((6 * dx)) + prc->right) - LOWORD(GetTextExtent(hdc, &(chT), 0x1))), prc->top, &(chT), 0x1);

L_43ca:
    goto L_4500;
    i = PctCloakFromLpfl(lpfl);
    if ((i != 0))
        goto L_4400;
    else
        goto L_43e7;

L_43e7:
    cch = strlen(szDblDash);
    psz = szDblDash;
    goto L_4423;

L_4400:
    psz = szT;
    cch = _wsprintf(psz, PCTDPCTPCT, i);

L_4423:
    RightTextOut(hdc, (prc->right - 5), prc->top, psz, cch, 0x0);
    goto L_4500;
    DrawMineralItem(hdc, (prc->right - 2), prc->top, 0xffff, WtFromLpfl(lpfl));
    goto L_4500;
    psz = szT;
    cch = _wsprintf(psz, "%d", (lpfl->ifl + 0x1));
    RightTextOut(hdc, (prc->right - 2), prc->top, psz, cch, 0x0);
    goto L_4eb8;

L_44d7:

L_4500:

L_4503:
    lpbd = BtlDataGet(vlprgidMisc[irow]);
    if ((LOWORD(lpbd) != 0x0))
        goto L_453b;
    else
        goto L_452f;

L_452f:
    if ((HIWORD(lpbd) == 0x0))
        goto L_4eb8;
    else
        goto L_4535;

L_4535:

L_453b:
    SelectObject(hdc, rghfontArial8[1]);
    SetTextColor(hdc, 0x0);
    SetBkMode(hdc, TRANSPARENT);
    goto L_482b;
    if ((lpbd->idPlanet == 0xffff))
        goto L_458c;
    else
        goto L_4576;

L_4576:
    psz = PszGetPlanetName(lpbd->idPlanet);
    goto L_45bf;

L_458c:
    psz = szT;
    _wsprintf(psz, PszGetCompressedString(idsSpaceDD), lpbd->pt.x, lpbd->pt.y);

L_45bf:
    if ((lpbd->pt.x != sel.scan.pt.x))
        goto L_45f2;
    else
        goto L_45d9;

L_45d9:
    if ((lpbd->pt.y != sel.scan.pt.y))
        goto L_45f2;
    else
        goto L_45e2;

L_45e2:
    SetTextColor(hdc, 0x7f);

L_45f2:
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
    goto L_485a;
    /* untranslated: call DrawMineralItem(hdc, (prc->right - 2), prc->top, 0xffff, words(0x0, lpbd->cplr)) -> callresult(void) */
    goto L_485a;
    t_call_4669 = CBattleUnits(lpbd, 0x5);
    if ((LOWORD(t_call_4669) != 0x0))
        goto L_4681;
    else
        goto L_4679;

L_4679:
    if ((HIWORD(t_call_4669) == 0x0))
        goto L_4688;
    else
        goto L_4681;

L_4681:
    chT = 79;
    goto L_46b5;

L_4688:
    t_call_4692 = CBattleUnits(lpbd, 0x6);
    if ((LOWORD(t_call_4692) != 0x0))
        goto L_46aa;
    else
        goto L_46a2;

L_46a2:
    if ((HIWORD(t_call_4692) == 0x0))
        goto L_46b1;
    else
        goto L_46aa;

L_46aa:
    chT = 84;
    goto L_46b5;

L_46b1:
    chT = 32;

L_46b5:
    CtrTextOut(hdc, (((uint32_t)((prc->right - prc->left)) / 0x2) + prc->left), prc->top, &(chT), 0x1);
    goto L_485a;
    i = 11;
    goto BtlUnitsCom;
    i = 19;
    goto BtlUnitsCom;
    i = 35;
    goto BtlUnitsCom;
    i = 67;
    goto BtlUnitsCom;
    i = 131;
    goto BtlUnitsCom;
    i = 255;
    goto BtlUnitsCom;
    i = 253;
    goto BtlUnitsCom;
    i = 254;

BtlUnitsCom:
    DrawMineralItem(hdc, (prc->right - 2), prc->top, 0xffff, CBattleUnits(lpbd, i));
    goto L_485a;
    if ((icol != 11))
        goto L_476a;
    else
        goto L_4764;

L_4764:
    t_merge_476d_0001 = 0x1;
    goto L_476d;

L_476a:
    t_merge_476d_0001 = 0x0;

L_476d:
    l = CBattleKills(lpbd, t_merge_476d_0001);
    DrawMineralItem(hdc, (prc->right - 2), prc->top, 0xffff, l);
    goto L_485a;
    i = 253;

LUnitsLeft:
    l = CBattleUnits(lpbd, i);
    if ((icol != 13))
        goto L_47dc;
    else
        goto L_47d6;

L_47d6:
    t_merge_47df_0001 = 0x1;
    goto L_47df;

L_47dc:
    t_merge_47df_0001 = 0x0;

L_47df:
    l = (l - CBattleKills(lpbd, t_merge_47df_0001));
    DrawMineralItem(hdc, (prc->right - 2), prc->top, 0xffff, l);
    goto L_485a;
    i = 254;
    goto LUnitsLeft;

L_482b:

L_485a:

L_485d:
    lpfl = rglpfl[vlprgidMisc[irow]];
    SelectObject(hdc, rghfontArial8[1]);
    SetTextColor(hdc, 0x0);
    SetBkMode(hdc, TRANSPARENT);
    goto L_4e66;
    if ((sel.scan.grobj != grobjFleet))
        goto L_48fb;
    else
        goto L_48c6;

L_48c6:
    if ((rglpfl[sel.scan.ifl]->id != lpfl->id))
        goto L_48fb;
    else
        goto L_48eb;

L_48eb:
    SetTextColor(hdc, 0x7f);

L_48fb:
    psz = PszGetFleetName(lpfl->id);
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
    goto L_4e8f;
    if ((lpfl->idPlanet == -1))
        goto L_496a;
    else
        goto L_4955;

L_4955:
    psz = PszGetPlanetName(lpfl->idPlanet);
    goto L_499d;

L_496a:
    psz = szT;
    _wsprintf(psz, PszGetCompressedString(idsSpaceDD), lpfl->pt.x, lpfl->pt.y);

L_499d:
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
    goto L_4e8f;
    i = IshdefPrimaryFromLpfl(lpfl, &(j));
    prc->right = (prc->right - LOWORD((0x6 * dx)));
    lpsz = rglpshdef[lpfl->iPlayer][i].hul.szClass;
    ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, lpsz, fstrlen(lpsz), 0x0);
    psz = szT;
    cch = _wsprintf(psz, PCTD, lpfl->rgcsh[i]);
    chT = 43;
    RightTextOut(hdc, ((LOWORD((6 * dx)) + prc->right) - LOWORD(GetTextExtent(hdc, &(chT), 0x1))), prc->top, psz, cch, 0x0);
    if ((j <= 1))
        goto L_4b28;
    else
        goto L_4ae5;

L_4ae5:
    TextOut(hdc, ((LOWORD((6 * dx)) + prc->right) - LOWORD(GetTextExtent(hdc, &(chT), 0x1))), prc->top, &(chT), 0x1);

L_4b28:
    goto L_4e8f;
    l = 0;
    i = 0;
    goto L_4b63;

L_4b3f:
    l = (l + (uint32_t)(lpfl->rgcsh[i]));
    i = (i + 1);

L_4b63:
    if ((i < 16))
        goto L_4b3f;
    else
        goto L_4b6c;

L_4b6c:
    DrawMineralItem(hdc, (prc->right - 2), prc->top, 0xffff, l);
    goto L_4e8f;
    l = 0;
    i = 0;
    goto L_4c38;

L_4baa:
    if ((lpfl->rgcsh[i] == 0))
        goto L_4c34;
    else
        goto L_4bc7;

L_4bc7:
    j = ((*(LphuldefFromId(rglpshdef[lpfl->iPlayer][i].hul.ihuldef) + 0x7b) >> 0xa) & 0xf);
    if ((j <= 1))
        goto L_4c14;
    else
        goto L_4c0b;

L_4c0b:
    if ((j < 6))
        goto L_4c34;
    else
        goto L_4c14;

L_4c14:
    l = (l + (uint32_t)(lpfl->rgcsh[i]));

L_4c34:
    i = (i + 1);

L_4c38:
    if ((i < 16))
        goto L_4baa;
    else
        goto L_4c41;

L_4c41:
    DrawMineralItem(hdc, (prc->right - 2), prc->top, 0xffff, l);
    goto L_4e8f;
    j = 2;

LEFleetCount:
    l = 0;
    i = 0;
    goto L_4d07;

L_4c84:
    if ((lpfl->rgcsh[i] == 0))
        goto L_4d03;
    else
        goto L_4ca1;

L_4ca1:
    if ((((*(LphuldefFromId(rglpshdef[lpfl->iPlayer][i].hul.ihuldef) + 0x7b) >> 0xa) & 0xf) != j))
        goto L_4d03;
    else
        goto L_4ce3;

L_4ce3:
    l = (l + (uint32_t)(lpfl->rgcsh[i]));

L_4d03:
    i = (i + 1);

L_4d07:
    if ((i < 16))
        goto L_4c84;
    else
        goto L_4d10;

L_4d10:
    DrawMineralItem(hdc, (prc->right - 2), prc->top, 0xffff, l);
    goto L_4e8f;
    j = 3;
    goto LEFleetCount;
    j = 5;
    goto LEFleetCount;
    j = 4;
    goto LEFleetCount;
    /* untranslated: call DrawMineralItem(hdc, (prc->right - 2), prc->top, 0xffff, words(*(lpfl+0x2e), *(lpfl+0x2c))) -> callresult(void) */
    goto L_4e8f;
    psz = szT;
    cch = _wsprintf(psz, "%d", (lpfl->ifl + 0x1));
    RightTextOut(hdc, (prc->right - 2), prc->top, psz, cch, 0x0);
    goto L_4e8f;
    if ((lpfl->fdirValid == 0x0))
        goto L_4e01;
    else
        goto L_4def;

L_4def:
    if ((lpfl->iwarpFlt > 0x0))
        goto L_4e0f;
    else
        goto L_4e01;

L_4e01:
    psz = 0x15fd;
    cch = 2;
    goto L_4e3a;

L_4e0f:
    psz = szT;
    cch = _wsprintf(psz, "%d", lpfl->iwarpFlt);

L_4e3a:
    RightTextOut(hdc, (prc->right - 2), prc->top, psz, cch, 0x0);
    goto L_4eb8;

L_4e66:

L_4e8f:

L_4e92:
    if ((irpt == 0))
        goto L_33c4;
    else
        goto L_4e9a;

L_4e9a:
    if ((irpt == 1))
        goto L_3e64;
    else
        goto L_4ea2;

L_4ea2:
    if ((irpt == 2))
        goto L_485d;
    else
        goto L_4eaa;

L_4eaa:
    if ((irpt == 3))
        goto L_4503;
    else
        goto L_4eb8;

L_4eb8:
    return;
}

void DrawMineralItem(HDC hdc, int16_t x, int16_t y, int16_t iMineral, int32_t l) {
    char   *psz;
    int16_t cch;

L_4ebe:
    if ((iMineral >= 0))
        goto L_4ee3;
    else
        goto L_4ed0;

L_4ed0:
    SetTextColor(hdc, 0x0);
    goto L_4efa;

L_4ee3:
    SetTextColor(hdc, rgcrMinerals[iMineral]);

L_4efa:
    if ((HIWORD(l) < 0x0))
        goto L_4f29;
    else
        goto L_4f03;

L_4f03:
    if ((HIWORD(l) > 0x0))
        goto L_4f11;
    else
        goto L_4f08;

L_4f08:
    if ((LOWORD(l) < 0x0))
        goto L_4f29;
    else
        goto L_4f11;

L_4f11:
    psz = PszFromLongK(l, &(cch));
    goto L_4f3e;

L_4f29:
    cch = strlen(szDblDash);
    psz = szDblDash;

L_4f3e:
    RightTextOut(hdc, x, y, psz, cch, 0x0);
    return;
}

char *PszGetDestName(FLEET *lpfl, HDC hdc) {
    int16_t i;
    ORDER   ord;

L_4f60:
    ord = lpfl->lpplord->rgord[0];
    if ((lpfl->cord <= 1))
        goto LNoDest;
    else
        goto L_4f99;

L_4f99:
    if ((ord.fValidTask == 0x0))
        goto L_5064;
    else
        goto L_4fac;

L_4fac:
    goto L_5039;

L_4fb5:
    i = 0;
    goto L_4fe9;

L_4fbd:
    goto L_4fda;

L_4fda:
    /* untranslated: branch ((ss:[bp+(i * 2)-0xe] >> 0xc) & 0xf) == 0x6 ? L_500e : L_4fe5 */

L_4fe5:
    i = (i + 1);

L_4fe9:
    if ((i >= 5))
        goto L_5064;
    else
        goto L_4fef;

L_4fef:

L_4ff5:
    if ((ord.grobj != grobjPlanet))
        goto L_5064;
    else
        goto L_5005;

L_5005:

LDelayed:
    if ((hdc == 0x0))
        goto L_5027;
    else
        goto L_5017;

L_5017:
    SetTextColor(hdc, 0x7f);

L_5027:
    return PszGetCompressedString(idsDelayed);

L_5039:
    if ((ord.grTask == grTaskXfer))
        goto L_4fb5;
    else
        goto L_5041;

L_5041:
    if ((ord.grTask == grTaskColonize))
        goto L_4ff5;
    else
        goto L_5049;

L_5049:
    if ((ord.grTask == grTaskMerge))
        goto LNoDest;
    else
        goto L_5051;

L_5051:
    if ((ord.grTask == grTaskScrap))
        goto LNoDest;
    else
        goto L_5059;

L_5059:
    if ((ord.grTask == grTaskLayMines))
        goto LDelayed;
    else
        goto L_5064;

L_5064:
    ord = lpfl->lpplord->rgord[1];
    return PszGetLocName(ord.grobj, ord.id, ord.pt.x, ord.pt.y);

LNoDest:
    return szDblDash;
}

int16_t FDestIsWP0(FLEET *lpfl) {
    int16_t i;
    ORDER   ord;

L_50b4:
    ord = lpfl->lpplord->rgord[0];
    if ((lpfl->cord <= 1))
        goto L_519c;
    else
        goto L_50ed;

L_50ed:
    if ((ord.fValidTask == 0x0))
        goto L_5196;
    else
        goto L_5100;

L_5100:
    goto L_516b;

L_5109:
    i = 0;
    goto L_5140;

L_5111:
    goto L_5131;

L_5128:
    return 0x1;

L_5131:
    /* untranslated: branch ((ss:[bp+(i * 2)-0xe] >> 0xc) & 0xf) == 0x6 ? L_5128 : L_513c */

L_513c:
    i = (i + 1);

L_5140:
    if ((i >= 5))
        goto L_5196;
    else
        goto L_5146;

L_5146:

L_514c:
    if ((ord.grobj != grobjPlanet))
        goto L_5196;
    else
        goto L_515c;

L_515c:

L_5162:
    return 0x1;

L_516b:
    if ((ord.grTask == grTaskXfer))
        goto L_5109;
    else
        goto L_5173;

L_5173:
    if ((ord.grTask == grTaskColonize))
        goto L_514c;
    else
        goto L_517b;

L_517b:
    if ((ord.grTask == grTaskMerge))
        goto L_5162;
    else
        goto L_5183;

L_5183:
    if ((ord.grTask == grTaskScrap))
        goto L_5162;
    else
        goto L_518b;

L_518b:
    if ((ord.grTask == grTaskLayMines))
        goto L_5162;
    else
        goto L_5196;

L_5196:
    return 0x0;

L_519c:
    return 0x1;
}

char *PszGetETA(HDC hdc, FLEET *lpfl, int16_t *pcYears) {
    POINT   pt;
    int16_t c;
    int16_t i;
    ORDER   ord;
    char   *psz;
    int32_t t_call_52fe;

L_51a8:
    ord = lpfl->lpplord->rgord[0];
    pt.x = ord.pt.x;
    pt.y = ord.pt.y;
    if ((lpfl->cord <= 1))
        goto LNoETA;
    else
        goto L_51ed;

L_51ed:
    if ((ord.fValidTask == 0x0))
        goto L_5290;
    else
        goto L_5200;

L_5200:
    goto L_5265;

L_5209:
    i = 0;
    goto L_523d;

L_5211:
    goto L_522e;

L_522e:
    /* untranslated: branch ((ss:[bp+(i * 2)-0x14] >> 0xc) & 0xf) == 0x6 ? L_539c : L_5239 */

L_5239:
    i = (i + 1);

L_523d:
    if ((i >= 5))
        goto L_5290;
    else
        goto L_5243;

L_5243:

L_5249:
    if ((ord.grobj != grobjPlanet))
        goto L_5290;
    else
        goto L_5259;

L_5259:

L_5265:
    if ((ord.grTask == grTaskXfer))
        goto L_5209;
    else
        goto L_526d;

L_526d:
    if ((ord.grTask == grTaskColonize))
        goto L_5249;
    else
        goto L_5275;

L_5275:
    if ((ord.grTask == grTaskMerge))
        goto LNoETA;
    else
        goto L_527d;

L_527d:
    if ((ord.grTask == grTaskScrap))
        goto LNoETA;
    else
        goto L_5285;

L_5285:
    if ((ord.grTask == grTaskLayMines))
        goto LNoETA;
    else
        goto L_5290;

L_5290:
    ord = lpfl->lpplord->rgord[1];
    CchGetETA(hdc, lpfl, szWork, 0x1, 0x1);
    if ((hdc == 0x0))
        goto L_5330;
    else
        goto L_52d9;

L_52d9:
    t_call_52fe = EstFuelUse(lpfl, 0x0, ord.iWarp, 0xffffffff, 0x0);
    if ((HIWORD(t_call_52fe) < HIWORD(lpfl->rgwtMin[0x4])))
        goto L_5330;
    else
        goto L_5312;

L_5312:
    if ((HIWORD(t_call_52fe) > HIWORD(lpfl->rgwtMin[0x4])))
        goto L_5320;
    else
        goto L_5317;

L_5317:
    if ((LOWORD(t_call_52fe) <= LOWORD(lpfl->rgwtMin[0x4])))
        goto L_5330;
    else
        goto L_5320;

L_5320:
    SetTextColor(hdc, 0xff);

L_5330:
    if ((pcYears == 0x0))
        goto L_5396;
    else
        goto L_5339;

L_5339:
    psz = szWork;
    c = 0;

L_5343:
    if (((uint16_t)(*(psz)) < 48))
        goto L_5380;
    else
        goto L_5351;

L_5351:
    if (((uint16_t)(*(psz)) > 57))
        goto L_5380;
    else
        goto L_535f;

L_535f:
    c = (LOWORD((0xa * c)) + ((uint16_t)(*(psz)) - 48));
    psz = (psz + 0x1);
    goto L_5343;

L_5380:
    if ((c != 0))
        goto L_538e;
    else
        goto L_5389;

L_5389:
    c = 32000;

L_538e:
    *(pcYears) = c;

L_5396:
    return szWork;

LNoETA:
    if ((pcYears == 0x0))
        goto L_53ac;
    else
        goto L_53a5;

L_53a5:
    *(pcYears) = 0;

L_53ac:
    return szDblDash;
}

char *PszGetTaskName(FLEET *lpfl, int16_t *picr) {
    int16_t        icr;
    StringId       ids;
    XferActionType opOrd;
    int16_t        iZip;
    int16_t        i;
    ORDER          ord;
    int16_t        fPercent;
    char          *psz;
    LPCSTR        *t_merge_577a_0001;

L_53b8:
    icr = -1;
    ord = lpfl->lpplord->rgord[0];
    *(picr) = -1;
    if ((ord.fValidTask == 0x0))
        goto L_5476;
    else
        goto L_5403;

L_5403:
    goto L_5452;
    i = 0;
    goto L_5440;

L_5414:
    goto L_5431;

L_5431:
    /* untranslated: branch ((ss:[bp+(i * 2)-0x16] >> 0xc) & 0xf) == 0x6 ? L_54a6 : L_543c */

L_543c:
    i = (i + 1);

L_5440:
    if ((i < 4))
        goto L_5414;
    else
        goto L_5449;

L_5449:
    goto L_5476;
    goto LShowTask;

L_5452:

L_5476:
    if ((lpfl->cord <= 1))
        goto LShowTask;
    else
        goto L_5483;

L_5483:
    ord = lpfl->lpplord->rgord[1];

LShowTask:
    if ((ord.fValidTask == 0x0))
        goto L_5890;
    else
        goto L_54b9;

L_54b9:
    ids = (ord.grTask + 99);
    goto L_586d;

L_54ce:
    i = 0;
    goto L_552c;

L_54d6:
    if ((vrgZip[i].fValid == 0x0))
        goto L_5528;
    else
        goto L_54ef;

L_54ef:
    if ((memcmp(vrgZip[i], &(ord.tsell.iPlrX), 0xa) != 0))
        goto L_5528;
    else
        goto L_5513;

L_5513:
    return vrgZip[i].szName;

L_5528:
    i = (i + 1);

L_552c:
    if ((i < 4))
        goto L_54d6;
    else
        goto L_5535;

L_5535:
    ids = idsAction;
    iZip = -1;
    if ((ord.txp.rgia[4].iAction != 0x7))
        goto L_5644;
    else
        goto L_5552;

L_5552:
    if ((ord.txp.rgia[3].iAction == 0x0))
        goto L_558b;
    else
        goto L_5565;

L_5565:
    if ((ord.txp.rgia[3].iAction != 0x2))
        goto L_5644;
    else
        goto L_5578;

L_5578:
    if ((((ord.tsell.iPlrX >> 0xc) & 0xf) != 0x2))
        goto L_5644;
    else
        goto L_558b;

L_558b:
    opOrd = ((ord.tsell.iPlrX >> 0xc) & 0xf);
    goto L_5629;

L_559f:
    i = 1;
    goto L_55cc;

L_55a7:
    /* untranslated: branch ((ss:[bp+(i * 2)-0x16] >> 0xc) & 0xf) != opOrd ? L_55d5 : L_55c2 */

L_55c2:

L_55c8:
    i = (i + 1);

L_55cc:
    if ((i < 3))
        goto L_55a7;
    else
        goto L_55d5;

L_55d5:
    if ((i != 3))
        goto L_5644;
    else
        goto L_55de;

L_55de:
    goto L_55ff;

L_55e4:
    iZip = 0;
    goto L_561a;

L_55ec:
    iZip = 1;
    goto L_561a;

L_55f4:
    iZip = 2;
    goto L_561a;

L_55ff:
    if ((opOrd == iActionLoadAll))
        goto L_55e4;
    else
        goto L_5607;

L_5607:
    if ((opOrd == iActionUnloadAll))
        goto L_55ec;
    else
        goto L_560f;

L_560f:
    if ((opOrd == iActionWaitPercent))
        goto L_55f4;
    else
        goto L_561a;

L_561a:
    return rgszZipOrder[iZip];

L_5629:
    if ((opOrd == iActionLoadAll))
        goto L_559f;
    else
        goto L_5631;

L_5631:
    if ((opOrd == iActionUnloadAll))
        goto L_559f;
    else
        goto L_5639;

L_5639:
    if ((opOrd == iActionWaitPercent))
        goto L_559f;
    else
        goto L_5644;

L_5644:
    i = 4;
    goto L_5684;

L_564c:
    /* untranslated: opOrd = ((ss:[bp+(i * 0x2)-0x16] >> 0xc) & 0xf) */
    if (((opOrd + 109) <= ids))
        goto L_5680;
    else
        goto L_5671;

L_5671:
    ids = (opOrd + 109);
    icr = i;

L_5680:
    i = (i - 1);

L_5684:
    if ((i >= 0))
        goto L_564c;
    else
        goto L_568d;

L_568d:
    if ((ids != idsAction))
        goto L_56a5;
    else
        goto L_5696;

L_5696:
    return PszGetCompressedString(idsTransport);

L_56a5:
    /* untranslated: opOrd = ((ss:[bp+(icr * 0x2)-0x16] >> 0xc) & 0xf) */
    *(picr) = icr;
    fPercent = 0;
    goto L_5794;
    if ((icr != 4))
        goto L_56dd;
    else
        goto L_56d8;

L_56d8:
    ids = idsLoadOptimal;

L_56dd:
    PszGetCompressedString(ids);
    goto L_5896;
    fPercent = 1;
    psz = PszGetCompressedString(ids);
    psz[(strlen(psz) - 3)] = 0;
    if ((fPercent == 0))
        goto L_574d;
    else
        goto L_571d;

L_571d:
    /* untranslated: call _wsprintf(szWork, "%s %d%%", psz, (ss:[bp+(icr * 2)-0x16] & 0xfff)) -> callresult(int16_t) */
    goto L_578b;

L_574d:
    if ((icr != 4))
        goto L_5775;
    else
        goto L_576d;

L_576d:
    t_merge_577a_0001 = "%s %dmg";
    goto L_577a;

L_5775:
    t_merge_577a_0001 = "%s %dkT";

L_577a:
    /* untranslated: call _wsprintf(szWork, t_merge_577a_0001, psz, (ss:[bp+(icr * 2)-0x16] & 0xfff)) -> callresult(int16_t) */

L_578b:
    return szWork;

L_5794:

L_57bd:
    if ((ord.tsell.iPlrX >= 0x5))
        goto L_57f5;
    else
        goto L_57c6;

L_57c6:
    _wsprintf(szWork, "%s  %dy", PszGetCompressedString(ids), (ord.tsell.iPlrX + 0x1));
    goto L_5804;

L_57f5:
    CchGetString(ids, szWork);

L_5804:
    return szWork;

L_580a:
    if ((ord.tptl.iDist >= 0xb))
        goto L_5847;
    else
        goto L_5813;

L_5813:
    _wsprintf(szWork, "%s  %dly", PszGetCompressedString(ids), LOWORD(((ord.tptl.iDist + 0x1) * 0x32)));
    goto L_5856;

L_5847:
    CchGetString(ids, szWork);

L_5856:
    return szWork;

L_585c:
    return PszGetCompressedString(ids);

L_586d:
    if ((ord.grTask == grTaskXfer))
        goto L_54ce;
    else
        goto L_5875;

L_5875:
    if ((ord.grTask == grTaskLayMines))
        goto L_57bd;
    else
        goto L_587d;

L_587d:
    if ((ord.grTask == grTaskPatrol))
        goto L_580a;
    else
        goto L_5885;

L_5885:
    if ((ord.grTask != grTaskAutoRoute))
        goto L_585c;
    else
        goto L_588a;

L_588a:

L_5890:
    return szDblDash;

L_5896:
}

void SortReportCache(int16_t irpt, int16_t icol) {
    uint16_t rgidRep[1024];
    PLANET  *lpplMac;
    int16_t  cRows;
    uint16_t iItem;
    PLANET  *lppl;
    FLEET   *lpfl;
    int16_t  i;

L_589c:
    cRows = 0;
    iItem = 0x0;
    if ((vprptCur->icolSort == icol))
        goto L_58f4;
    else
        goto L_58c0;

L_58c0:
    vicolSortPrev = vprptCur->icolSort;
    viSubsortPrev = vprptCur->iSubsort;
    vfAscendingPrev = vprptCur->fAscending;
    vprptCur->icolSort = icol;
    gd.fChgReports = 0x1;

L_58f4:
    if ((hwndReportDlg != 0x0))
        goto L_590e;
    else
        goto L_58fe;

L_58fe:
    if ((vprptCur->fCached != 0))
        goto L_5bb1;
    else
        goto L_5908;

L_5908:

L_590e:
    goto L_5b3a;

L_5917:
    vlprgidRep = vlprgidFleet;
    iItem = 0x0;
    goto L_5933;

L_592e:
    iItem = (iItem + 0x1);

L_5933:
    if ((iItem >= cFleet))
        goto L_5b5d;
    else
        goto L_5940;

L_5940:
    lpfl = rglpfl[iItem];
    if ((LOWORD(rglpfl[iItem]) != 0x0))
        goto L_5973;
    else
        goto L_596b;

L_596b:
    if ((*(rglpfl[iItem] + 0x2) == 0x0))
        goto L_5b5d;
    else
        goto L_5973;

L_5973:
    if ((lpfl->iplr != idPlayer))
        goto L_592e;
    else
        goto L_598d;

L_598d:
    cRows = (cRows + 1);
    rgidRep[cRows] = iItem;

L_59aa:
    vlprgidRep = vlprgidMisc;
    vrptBattle.fCached = 0;
    iItem = 0x0;
    goto L_59cc;

L_59c7:
    iItem = (iItem + 0x1);

L_59cc:
    if ((iItem >= cFleet))
        goto L_5b5d;
    else
        goto L_59d9;

L_59d9:
    lpfl = rglpfl[iItem];
    if ((LOWORD(rglpfl[iItem]) != 0x0))
        goto L_5a0c;
    else
        goto L_5a04;

L_5a04:
    if ((*(rglpfl[iItem] + 0x2) == 0x0))
        goto L_5b5d;
    else
        goto L_5a0c;

L_5a0c:
    if ((lpfl->iplr == idPlayer))
        goto L_5a3d;
    else
        goto L_5a26;

L_5a26:
    cRows = (cRows + 1);
    rgidRep[cRows] = iItem;

L_5a3d:
    if ((cRows >= 1020))
        goto L_5b5d;
    else
        goto L_5a45;

L_5a45:

L_5a51:
    vlprgidRep = vlprgidPlanet;
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_5ad6;

L_5a92:
    if ((lppl->iPlayer != idPlayer))
        goto L_5acc;
    else
        goto L_5aa2;

L_5aa2:
    if ((lppl->det != 0x7))
        goto L_5acc;
    else
        goto L_5ab5;

L_5ab5:
    cRows = (cRows + 1);
    rgidRep[cRows] = iItem;

L_5acc:
    iItem = (iItem + 0x1);
    lppl = (lppl + 0x1);

L_5ad6:
    if ((LOWORD(lppl) >= LOWORD(lpplMac)))
        goto L_5b5d;
    else
        goto L_5ae4;

L_5ae4:

L_5aea:
    vlprgidRep = vlprgidMisc;
    vrptEFleet.fCached = 0;
    cRows = CBattles();
    i = 0;
    goto L_5b27;

L_5b10:
    rgidRep[i] = i;
    i = (i + 1);

L_5b27:
    if ((i >= cRows))
        goto L_5b5d;
    else
        goto L_5b31;

L_5b31:

L_5b3a:
    if ((irpt == 0))
        goto L_5a51;
    else
        goto L_5b42;

L_5b42:
    if ((irpt == 1))
        goto L_5917;
    else
        goto L_5b4a;

L_5b4a:
    if ((irpt == 2))
        goto L_59aa;
    else
        goto L_5b52;

L_5b52:
    if ((irpt != 3))
        goto L_5bb1;
    else
        goto L_5b57;

L_5b57:

L_5b5d:
    vprptCur->cRows = cRows;
    qsort(rgidRep, cRows, 0x2, ICompReport);
    fmemcpy(&(vlprgidRep), &(rgidRep), (cRows * 2));
    vprptCur->fCached = 1;

L_5bb1:
    return;
}

int16_t ICompReport(void *arg1, void *arg2) {
    char     szT[80];
    int32_t  l2;
    int16_t  fAscending;
    int16_t  icolSort;
    int16_t  i1;
    int16_t  j;
    int16_t  i;
    int32_t  l1;
    int16_t  iSubsort;
    char    *psz;
    int16_t  iRet;
    int16_t  i2;
    int16_t  fTier2;
    int16_t  irpt;
    PLANET  *lppl2;
    PLANET  *lppl1;
    float    pct2;
    float    pct1;
    int16_t  iFirst;
    int32_t  rgl[4];
    int16_t  iLast;
    FLEET   *lpfl2;
    FLEET   *lpfl1;
    BTLDATA *lpbd1;
    int16_t  ibtl2;
    int16_t  ibtl1;
    BTLDATA *lpbd2;
    uint16_t t_merge_6c5d_0001;
    uint16_t t_merge_6c84_0001;
    uint16_t t_merge_6ce3_0001;
    uint16_t t_merge_6d0a_0001;

L_5bb8:
    iRet = 0;
    iSubsort = vprptCur->iSubsort;
    irpt = vprptCur->irpt;
    icolSort = vprptCur->icolSort;
    fAscending = vprptCur->fAscending;
    fTier2 = 0;

TryTier2:
    goto L_7448;

L_5bfc:
    lppl1 = &(lpPlanets[arg1]);
    lppl2 = &(lpPlanets[arg2]);
    goto L_63a3;

L_5c32:
    goto L_63d2;
    if ((lppl1->fStarbase != 0x0))
        goto L_5c73;
    else
        goto L_5c4c;

L_5c4c:
    if ((lppl2->fStarbase != 0x0))
        goto L_5c6b;
    else
        goto L_5c63;

L_5c63:
    iRet = 0;
    goto L_5ced;

L_5c6b:
    iRet = 1;

L_5c73:
    if ((lppl2->fStarbase != 0x0))
        goto L_5c92;
    else
        goto L_5c8a;

L_5c8a:
    iRet = -1;
    goto L_5ced;

L_5c92:
    iRet = fstrcmp(rglpshdefSB[idPlayer][lppl1->isb].hul.szClass, rglpshdefSB[idPlayer][lppl2->isb].hul.szClass);

L_5ced:
    goto L_63d2;
    if ((lppl1->idRoute != 0x0))
        goto L_5d24;
    else
        goto L_5d02;

L_5d02:
    if ((lppl2->idRoute != 0x0))
        goto L_5d1c;
    else
        goto L_5d14;

L_5d14:
    iRet = 0;
    goto L_5d91;

L_5d1c:
    iRet = 1;

L_5d24:
    if ((lppl2->idRoute != 0x0))
        goto L_5d3e;
    else
        goto L_5d36;

L_5d36:
    iRet = -1;
    goto L_5d91;

L_5d3e:
    psz = PszGetPlanetName((lppl1->idRoute + 0xffff));
    strcpy(szT, psz);
    psz = PszGetPlanetName((lppl2->idRoute + 0xffff));
    iRet = strcmp(szT, psz);

L_5d91:
    goto L_63d2;
    if ((lppl1->idFling != 0x0))
        goto L_5dc8;
    else
        goto L_5da6;

L_5da6:
    if ((lppl2->idFling != 0x0))
        goto L_5dc0;
    else
        goto L_5db8;

L_5db8:
    iRet = 0;
    goto L_5e35;

L_5dc0:
    iRet = 1;

L_5dc8:
    if ((lppl2->idFling != 0x0))
        goto L_5de2;
    else
        goto L_5dda;

L_5dda:
    iRet = -1;
    goto L_5e35;

L_5de2:
    psz = PszGetPlanetName((lppl1->idFling + 0xffff));
    strcpy(szT, psz);
    psz = PszGetPlanetName((lppl2->idFling + 0xffff));
    iRet = strcmp(szT, psz);

L_5e35:
    goto L_63d2;
    psz = PszGetPlanetName(lppl1->id);
    strcpy(szT, psz);
    psz = PszGetPlanetName(lppl2->id);
    iRet = strcmp(szT, psz);
    goto L_63d2;
    FillPlanetProdLB(0x0, 0x0, lppl1);
    strcpy(szT, szWork);
    FillPlanetProdLB(0x0, 0x0, lppl2);
    iRet = strcmp(&(szT[0x6]), &(szWork[0x6]));
    goto L_63d2;
    /* untranslated: ss:[bp-0x7a] = PctPlanetCapacity(lppl2) */
    /* untranslated: iRet = (PctPlanetCapacity(lppl1) - ss:[bp-0x7a]) */
    goto L_63d2;
    iRet = (LOWORD(lppl1->rgwtMin[0x3]) - LOWORD(lppl2->rgwtMin[0x3]));
    goto L_63d2;
    /* untranslated: ss:[bp-0x7a] = PctPlanetDesirability(lppl2, idPlayer) */
    /* untranslated: iRet = (PctPlanetDesirability(lppl1, idPlayer) - ss:[bp-0x7a]) */
    goto L_63d2;
    if ((iSubsort != 3))
        goto L_5fb2;
    else
        goto L_5f59;

L_5f59:
    i2 = 0;
    i1 = 0;
    i = 0;
    goto L_5fa6;

L_5f6c:
    i1 = (i1 + lppl1->rgMinConc[i]);
    i2 = (i2 + lppl2->rgMinConc[i]);
    i = (i + 1);

L_5fa6:
    if ((i >= 3))
        goto L_5fe8;
    else
        goto L_5fac;

L_5fac:

L_5fb2:
    i1 = lppl1->rgMinConc[iSubsort];
    i2 = lppl2->rgMinConc[iSubsort];

L_5fe8:
    iRet = (i1 - i2);
    goto L_63d2;
    if ((lppl1->cDefenses != 0x0))
        goto L_602f;
    else
        goto L_600d;

L_600d:
    if ((0x0 != 0x0))
        goto L_602f;
    else
        goto L_6015;

L_6015:
    pct1 = (double)(0);
    goto L_6054;

L_602f:
    CalcPctSurvive(lppl1, &(pct1), 0x0);
    pct1 = (1 - pct1);

L_6054:
    if ((lppl2->cDefenses != 0x0))
        goto L_608f;
    else
        goto L_606d;

L_606d:
    if ((0x0 != 0x0))
        goto L_608f;
    else
        goto L_6075;

L_6075:
    pct2 = (double)(0);
    goto L_60b4;

L_608f:
    CalcPctSurvive(lppl2, &(pct2), 0x0);
    pct2 = (1 - pct2);

L_60b4:
    if ((pct1 >= pct2))
        goto L_60d0;
    else
        goto L_60c8;

L_60c8:
    iRet = -1;
    goto L_60f1;

L_60d0:
    if ((pct1 <= pct2))
        goto L_60ec;
    else
        goto L_60e4;

L_60e4:
    iRet = 1;
    goto L_60f1;

L_60ec:
    iRet = 0;

L_60f1:
    goto L_63d2;
    /* untranslated: ss:[bp-0x7e] = 0x0 */
    /* untranslated: iRet = ((loword((uint32_t)(words(*(lppl1+0x14), *(lppl1+0x16)) >> 0x14)) & 0xfff) - (loword((uint32_t)(words(*(lppl2+0x14), *(lppl2+0x16))
     * >> 0x14)) & 0xfff)) */
    goto L_63d2;
    /* untranslated: ss:[bp-0x7e] = 0x0 */
    /* untranslated: iRet = ((loword((uint32_t)(words(*(lppl1+0x14), *(lppl1+0x16)) >> 0x8)) & 0xfff) - (loword((uint32_t)(words(*(lppl2+0x14), *(lppl2+0x16))
     * >> 0x8)) & 0xfff)) */
    goto L_63d2;
    if ((iSubsort != 3))
        goto L_61b3;
    else
        goto L_61a5;

L_61a5:
    iFirst = 0;
    iLast = 2;
    goto L_61bd;

L_61b3:
    iLast = iSubsort;
    iFirst = iSubsort;

L_61bd:
    EstMineralsMined(lppl1, rgl, 0xffffffff, 0x0);
    l1 = 0;
    i = iFirst;
    goto L_620b;

L_61ef:
    l1 = (l1 + rgl[i]);
    i = (i + 1);

L_620b:
    if ((i <= iLast))
        goto L_61ef;
    else
        goto L_6217;

L_6217:
    EstMineralsMined(lppl2, rgl, 0xffffffff, 0x0);
    l2 = 0;
    i = iFirst;
    goto L_6265;

L_6249:
    l2 = (l2 + rgl[i]);
    i = (i + 1);

L_6265:
    if ((i <= iLast))
        goto L_6249;
    else
        goto L_6271;

L_6271:
    iRet = (LOWORD(l1) - LOWORD(l2));
    goto L_63d2;
    if ((iSubsort != 3))
        goto L_6299;
    else
        goto L_628c;

L_628c:
    iFirst = 0;
    iLast = 2;
    goto L_62a2;

L_6299:
    iLast = iSubsort;
    iFirst = iSubsort;

L_62a2:
    l2 = 0;
    l1 = 0;
    i = iFirst;
    goto L_630b;

L_62c1:
    l1 = (l1 + lppl1->rgwtMin[i]);
    l2 = (l2 + lppl2->rgwtMin[i]);
    i = (i + 1);

L_630b:
    if ((i <= iLast))
        goto L_62c1;
    else
        goto L_6316;

L_6316:
    l1 = (l1 - l2);
    if ((HIWORD(l1) > 0x0))
        goto L_6347;
    else
        goto L_6331;

L_6331:
    if ((HIWORD(l1) < 0x0))
        goto L_633f;
    else
        goto L_6336;

L_6336:
    if ((LOWORD(l1) >= 0x0))
        goto L_6347;
    else
        goto L_633f;

L_633f:
    iRet = -1;
    goto L_636b;

L_6347:
    if ((HIWORD(l1) < 0x0))
        goto L_6366;
    else
        goto L_6350;

L_6350:
    if ((HIWORD(l1) > 0x0))
        goto L_635e;
    else
        goto L_6355;

L_6355:
    if ((LOWORD(l1) <= 0x0))
        goto L_6366;
    else
        goto L_635e;

L_635e:
    iRet = 1;
    goto L_636b;

L_6366:
    iRet = 0;

L_636b:
    goto L_63d2;
    /* untranslated: ss:[bp-0x7a] = CResourcesAtPlanet(lppl2, idPlayer) */
    /* untranslated: iRet = (CResourcesAtPlanet(lppl1, idPlayer) - ss:[bp-0x7a]) */
    goto L_746b;

L_63a3:
    if ((icolSort > 14))
        goto L_5c32;
    else
        goto L_63ab;

L_63ab:
    goto L_ffffffff;

L_63d2:

L_63d5:
    lpfl1 = rglpfl[arg1];
    lpfl2 = rglpfl[arg2];
    goto L_69bf;

L_641f:
    goto L_69e8;
    psz = PszGetFleetName(lpfl1->id);
    strcpy(szT, psz);
    psz = PszGetFleetName(lpfl2->id);
    iRet = strcmp(szT, psz);
    goto L_69e8;
    if ((lpfl1->idPlanet == -1))
        goto L_6499;
    else
        goto L_6475;

L_6475:
    psz = PszGetPlanetName(lpfl1->idPlanet);
    strcpy(szT, psz);
    goto L_64c6;

L_6499:
    _wsprintf(szT, PszGetCompressedString(idsSpaceDD), lpfl1->pt.x, lpfl1->pt.y);

L_64c6:
    if ((lpfl2->idPlanet == -1))
        goto L_64e8;
    else
        goto L_64d3;

L_64d3:
    psz = PszGetPlanetName(lpfl2->idPlanet);
    goto L_651a;

L_64e8:
    _wsprintf(szWork, PszGetCompressedString(idsSpaceDD), lpfl2->pt.x, lpfl2->pt.y);
    psz = szWork;

L_651a:
    iRet = strcmp(szT, psz);
    goto L_69e8;
    iRet = (lpfl1->iplan - lpfl2->iplan);
    goto L_69e8;
    psz = PszGetDestName(lpfl1, 0x0);
    strcpy(szT, psz);
    psz = PszGetDestName(lpfl2, 0x0);
    iRet = strcmp(szT, psz);
    if ((iRet >= 0))
        goto L_65af;
    else
        goto L_65a7;

L_65a7:
    iRet = -1;
    goto L_65bd;

L_65af:
    if ((iRet <= 0))
        goto L_65bd;
    else
        goto L_65b8;

L_65b8:
    iRet = 1;

L_65bd:
    goto L_69e8;
    PszGetETA(0x0, lpfl1, &(i1));
    PszGetETA(0x0, lpfl2, &(i2));
    iRet = (i1 - i2);
    goto L_69e8;
    l1 = (uint32_t)(PctCloakFromLpfl(lpfl1));
    l2 = (uint32_t)(PctCloakFromLpfl(lpfl2));
    goto LRetDiff;
    l1 = (uint32_t)(IshdefPrimaryFromLpfl(lpfl1, &(i1)));
    l2 = (uint32_t)(IshdefPrimaryFromLpfl(lpfl2, &(i2)));
    if ((LOWORD(l1) != LOWORD(l2)))
        goto L_66d3;
    else
        goto L_6665;

L_6665:
    if ((HIWORD(l1) != HIWORD(l2)))
        goto L_66d3;
    else
        goto L_666d;

L_666d:
    iRet = (lpfl1->rgcsh[l1] - lpfl2->rgcsh[l2]);
    goto L_679e;

L_66d3:
    iRet = strcmp(rgshdef[l1].hul.szClass, rgshdef[l2].hul.szClass);
    if ((iRet != 0))
        goto L_6786;
    else
        goto L_6723;

L_6723:
    iRet = (lpfl1->rgcsh[l1] - lpfl2->rgcsh[l2]);

L_6786:
    if ((iRet != 0))
        goto L_679e;
    else
        goto L_678f;

L_678f:
    iRet = (LOWORD(l1) - LOWORD(l2));

L_679e:
    if ((iRet != 0))
        goto L_67b0;
    else
        goto L_67a7;

L_67a7:
    iRet = (i1 - i2);

L_67b0:
    goto L_69e8;
    psz = PszGetTaskName(lpfl1, &(i1));
    strcpy(szT, psz);
    psz = PszGetTaskName(lpfl2, &(i2));
    iRet = strcmp(szT, psz);
    if ((iRet >= 0))
        goto L_680f;
    else
        goto L_6807;

L_6807:
    iRet = -1;
    goto L_6843;

L_680f:
    if ((iRet <= 0))
        goto L_6820;
    else
        goto L_6818;

L_6818:
    iRet = 1;
    goto L_6843;

L_6820:
    if ((i1 <= i2))
        goto L_6833;
    else
        goto L_682b;

L_682b:
    iRet = 1;
    goto L_6843;

L_6833:
    if ((i1 >= i2))
        goto L_6843;
    else
        goto L_683e;

L_683e:
    iRet = -1;

L_6843:
    goto L_69e8;
    iSubsort = -1;
    if ((iSubsort != 4))
        goto L_68c8;
    else
        goto L_6854;

L_6854:
    l2 = 0;
    l1 = 0;
    i = 0;
    goto L_68bc;

L_6872:
    l1 = (l1 + lpfl1->rgwtMin[i]);
    l2 = (l2 + lpfl2->rgwtMin[i]);
    i = (i + 1);

L_68bc:
    if ((i > 3))
        goto LRetDiff;
    else
        goto L_68c2;

L_68c2:

L_68c8:
    if ((iSubsort != -1))
        goto L_68d6;
    else
        goto L_68d1;

L_68d1:
    iSubsort = 4;

L_68d6:
    l1 = lpfl1->rgwtMin[iSubsort];
    l2 = lpfl2->rgwtMin[iSubsort];

LRetDiff:
    l1 = (l1 - l2);
    if ((HIWORD(l1) > 0x0))
        goto L_694d;
    else
        goto L_6937;

L_6937:
    if ((HIWORD(l1) < 0x0))
        goto L_6945;
    else
        goto L_693c;

L_693c:
    if ((LOWORD(l1) >= 0x0))
        goto L_694d;
    else
        goto L_6945;

L_6945:
    iRet = -1;
    goto L_6971;

L_694d:
    if ((HIWORD(l1) < 0x0))
        goto L_696c;
    else
        goto L_6956;

L_6956:
    if ((HIWORD(l1) > 0x0))
        goto L_6964;
    else
        goto L_695b;

L_695b:
    if ((LOWORD(l1) <= 0x0))
        goto L_696c;
    else
        goto L_6964;

L_6964:
    iRet = 1;
    goto L_6971;

L_696c:
    iRet = 0;

L_6971:
    goto L_69e8;
    l1 = WtFromLpfl(lpfl1);
    l2 = WtFromLpfl(lpfl2);
    goto LRetDiff;
    l1 = (uint32_t)(lpfl1->id);
    l2 = (uint32_t)(lpfl2->id);
    goto LRetDiff;

L_69bf:
    if ((icolSort > 11))
        goto L_641f;
    else
        goto L_69c7;

L_69c7:
    goto L_ffffffff;

L_69e8:

L_69eb:
    ibtl1 = arg1;
    ibtl2 = arg2;
    lpbd1 = BtlDataGet(ibtl1);
    lpbd2 = BtlDataGet(ibtl2);
    if ((LOWORD(lpbd1) != 0x0))
        goto L_6a2f;
    else
        goto L_6a26;

L_6a26:
    if ((HIWORD(lpbd1) == 0x0))
        goto L_6a41;
    else
        goto L_6a2f;

L_6a2f:
    if ((LOWORD(lpbd2) != 0x0))
        goto L_6a49;
    else
        goto L_6a38;

L_6a38:
    if ((HIWORD(lpbd2) != 0x0))
        goto L_6a49;
    else
        goto L_6a41;

L_6a41:
    iRet = 0;
    goto L_746b;

L_6a49:
    goto L_6d2d;

L_6a4f:
    goto L_6d5c;
    if ((lpbd1->idPlanet == 0xffff))
        goto L_6a84;
    else
        goto L_6a5f;

L_6a5f:
    psz = PszGetPlanetName(lpbd1->idPlanet);
    strcpy(szT, psz);
    goto L_6ab1;

L_6a84:
    _wsprintf(szT, PszGetCompressedString(idsSpaceDD), lpbd1->pt.x, lpbd1->pt.y);

L_6ab1:
    if ((lpbd2->idPlanet == 0xffff))
        goto L_6ad4;
    else
        goto L_6abe;

L_6abe:
    psz = PszGetPlanetName(lpbd2->idPlanet);
    goto L_6b06;

L_6ad4:
    _wsprintf(szWork, PszGetCompressedString(idsSpaceDD), lpbd2->pt.x, lpbd2->pt.y);
    psz = szWork;

L_6b06:
    iRet = strcmp(szT, psz);
    goto L_6d5c;
    l1 = CBattleUnits(lpbd1, 0x5);
    if ((LOWORD(l1) != 0x0))
        goto L_6b65;
    else
        goto L_6b3c;

L_6b3c:
    if ((HIWORD(l1) != 0x0))
        goto L_6b65;
    else
        goto L_6b45;

L_6b45:
    l1 = (int32_t)((CBattleUnits(lpbd1, 0x6) * 2));

L_6b65:
    l2 = CBattleUnits(lpbd2, 0x5);
    if ((LOWORD(l2) != 0x0))
        goto L_6baf;
    else
        goto L_6b86;

L_6b86:
    if ((HIWORD(l2) != 0x0))
        goto L_6baf;
    else
        goto L_6b8f;

L_6b8f:
    l2 = (int32_t)((CBattleUnits(lpbd2, 0x6) * 2));

L_6baf:
    goto LRetDiff;
    LOWORD(l1) = lpbd1->cplr;
    HIWORD(l1) = 0x0;
    LOWORD(l2) = lpbd2->cplr;
    HIWORD(l2) = 0x0;
    goto LRetDiff;
    i = 11;
    goto BtlUnitsCom;
    i = 19;
    goto BtlUnitsCom;
    i = 35;
    goto BtlUnitsCom;
    i = 67;
    goto BtlUnitsCom;
    i = 131;
    goto BtlUnitsCom;
    i = 255;
    goto BtlUnitsCom;
    i = 253;
    goto BtlUnitsCom;
    i = 254;

BtlUnitsCom:
    l1 = CBattleUnits(lpbd1, i);
    l2 = CBattleUnits(lpbd2, i);
    goto LRetDiff;
    if ((icolSort != 11))
        goto L_6c5a;
    else
        goto L_6c54;

L_6c54:
    t_merge_6c5d_0001 = 0x1;
    goto L_6c5d;

L_6c5a:
    t_merge_6c5d_0001 = 0x0;

L_6c5d:
    l1 = CBattleKills(lpbd1, t_merge_6c5d_0001);
    if ((icolSort != 11))
        goto L_6c81;
    else
        goto L_6c7b;

L_6c7b:
    t_merge_6c84_0001 = 0x1;
    goto L_6c84;

L_6c81:
    t_merge_6c84_0001 = 0x0;

L_6c84:
    l2 = CBattleKills(lpbd2, t_merge_6c84_0001);
    goto LRetDiff;
    i = 253;

LUnitsLeft:
    l1 = CBattleUnits(lpbd1, i);
    l2 = CBattleUnits(lpbd2, i);
    if ((icolSort != 13))
        goto L_6ce0;
    else
        goto L_6cda;

L_6cda:
    t_merge_6ce3_0001 = 0x1;
    goto L_6ce3;

L_6ce0:
    t_merge_6ce3_0001 = 0x0;

L_6ce3:
    l1 = (l1 - CBattleKills(lpbd1, t_merge_6ce3_0001));
    if ((icolSort != 13))
        goto L_6d07;
    else
        goto L_6d01;

L_6d01:
    t_merge_6d0a_0001 = 0x1;
    goto L_6d0a;

L_6d07:
    t_merge_6d0a_0001 = 0x0;

L_6d0a:
    l2 = (l2 - CBattleKills(lpbd2, t_merge_6d0a_0001));
    goto LRetDiff;
    i = 254;
    goto LUnitsLeft;

L_6d2d:
    if ((icolSort > 14))
        goto L_6a4f;
    else
        goto L_6d35;

L_6d35:
    goto L_ffffffff;

L_6d5c:

L_6d5f:
    lpfl1 = rglpfl[arg1];
    lpfl2 = rglpfl[arg2];
    goto L_741c;

L_6da9:
    goto L_7445;
    psz = PszGetFleetName(lpfl1->id);
    strcpy(szT, psz);
    psz = PszGetFleetName(lpfl2->id);
    iRet = strcmp(szT, psz);
    goto L_7445;
    if ((lpfl1->idPlanet == -1))
        goto L_6e23;
    else
        goto L_6dff;

L_6dff:
    psz = PszGetPlanetName(lpfl1->idPlanet);
    strcpy(szT, psz);
    goto L_6e50;

L_6e23:
    _wsprintf(szT, PszGetCompressedString(idsSpaceDD), lpfl1->pt.x, lpfl1->pt.y);

L_6e50:
    if ((lpfl2->idPlanet == -1))
        goto L_6e72;
    else
        goto L_6e5d;

L_6e5d:
    psz = PszGetPlanetName(lpfl2->idPlanet);
    goto L_6ea4;

L_6e72:
    _wsprintf(szWork, PszGetCompressedString(idsSpaceDD), lpfl2->pt.x, lpfl2->pt.y);
    psz = szWork;

L_6ea4:
    iRet = strcmp(szT, psz);
    goto L_7445;
    l1 = (uint32_t)(IshdefPrimaryFromLpfl(lpfl1, &(i1)));
    l2 = (uint32_t)(IshdefPrimaryFromLpfl(lpfl2, &(i2)));
    if ((LOWORD(l1) != LOWORD(l2)))
        goto L_6f67;
    else
        goto L_6ef9;

L_6ef9:
    if ((HIWORD(l1) != HIWORD(l2)))
        goto L_6f67;
    else
        goto L_6f01;

L_6f01:
    iRet = (lpfl1->rgcsh[l1] - lpfl2->rgcsh[l2]);
    goto L_7054;

L_6f67:
    iRet = fstrcmp(&(rglpshdef[lpfl1->iPlayer]->hul.szClass[(l1 * 0x93)]), &(rglpshdef[lpfl2->iPlayer]->hul.szClass[(l2 * 0x93)]));
    if ((iRet != 0))
        goto L_703c;
    else
        goto L_6fd9;

L_6fd9:
    iRet = (lpfl1->rgcsh[l1] - lpfl2->rgcsh[l2]);

L_703c:
    if ((iRet != 0))
        goto L_7054;
    else
        goto L_7045;

L_7045:
    iRet = (LOWORD(l1) - LOWORD(l2));

L_7054:
    if ((iRet != 0))
        goto L_7066;
    else
        goto L_705d;

L_705d:
    iRet = (i1 - i2);

L_7066:
    goto L_7445;
    LOWORD(l1) = *(lpfl1 + 0x2c);
    HIWORD(l1) = *(lpfl1 + 0x2e);
    LOWORD(l2) = *(lpfl2 + 0x2c);
    HIWORD(l2) = *(lpfl2 + 0x2e);
    goto LRetDiff;
    LOWORD(l1) = lpfl1->ifl;
    HIWORD(l1) = 0x0;
    LOWORD(l2) = lpfl2->ifl;
    HIWORD(l2) = 0x0;
    goto LRetDiff;
    l2 = 0;
    l1 = 0;
    i = 0;
    goto L_7113;

L_70d3:
    l1 = (l1 + (uint32_t)(lpfl1->rgcsh[i]));
    l2 = (l2 + (uint32_t)(lpfl2->rgcsh[i]));
    i = (i + 1);

L_7113:
    if ((i < 16))
        goto L_70d3;
    else
        goto L_711c;

L_711c:
    goto LRetDiff;
    l2 = 0;
    l1 = 0;
    i = 0;
    goto L_7251;

L_713d:
    if ((lpfl1->rgcsh[i] == 0))
        goto L_71c5;
    else
        goto L_715a;

L_715a:
    j = ((*(LphuldefFromId(rglpshdef[lpfl1->iPlayer][i].hul.ihuldef) + 0x7b) >> 0xa) & 0xf);
    if ((j <= 1))
        goto L_71a7;
    else
        goto L_719e;

L_719e:
    if ((j < 6))
        goto L_71c5;
    else
        goto L_71a7;

L_71a7:
    l1 = (l1 + (uint32_t)(lpfl1->rgcsh[i]));

L_71c5:
    if ((lpfl2->rgcsh[i] == 0))
        goto L_724d;
    else
        goto L_71e2;

L_71e2:
    j = ((*(LphuldefFromId(rglpshdef[lpfl2->iPlayer][i].hul.ihuldef) + 0x7b) >> 0xa) & 0xf);
    if ((j <= 1))
        goto L_722f;
    else
        goto L_7226;

L_7226:
    if ((j < 6))
        goto L_724d;
    else
        goto L_722f;

L_722f:
    l2 = (l2 + (uint32_t)(lpfl2->rgcsh[i]));

L_724d:
    i = (i + 1);

L_7251:
    if ((i < 16))
        goto L_713d;
    else
        goto L_725a;

L_725a:
    goto LRetDiff;
    j = 2;

LEFleetCount:
    l2 = 0;
    l1 = 0;
    i = 0;
    goto L_737e;

L_7280:
    if ((lpfl1->rgcsh[i] == 0))
        goto L_72fd;
    else
        goto L_729d;

L_729d:
    if ((((*(LphuldefFromId(rglpshdef[lpfl1->iPlayer][i].hul.ihuldef) + 0x7b) >> 0xa) & 0xf) != j))
        goto L_72fd;
    else
        goto L_72df;

L_72df:
    l1 = (l1 + (uint32_t)(lpfl1->rgcsh[i]));

L_72fd:
    if ((lpfl2->rgcsh[i] == 0))
        goto L_737a;
    else
        goto L_731a;

L_731a:
    if ((((*(LphuldefFromId(rglpshdef[lpfl2->iPlayer][i].hul.ihuldef) + 0x7b) >> 0xa) & 0xf) != j))
        goto L_737a;
    else
        goto L_735c;

L_735c:
    l2 = (l2 + (uint32_t)(lpfl2->rgcsh[i]));

L_737a:
    i = (i + 1);

L_737e:
    if ((i < 16))
        goto L_7280;
    else
        goto L_7387;

L_7387:
    goto LRetDiff;
    j = 3;
    goto LEFleetCount;
    j = 5;
    goto LEFleetCount;
    j = 4;
    goto LEFleetCount;
    if ((lpfl1->fdirValid == 0x0))
        goto L_73d2;
    else
        goto L_73bc;

L_73bc:
    LOWORD(l1) = lpfl1->iwarpFlt;
    HIWORD(l1) = 0x0;
    goto L_73dc;

L_73d2:
    l1 = -1;

L_73dc:
    if ((lpfl2->fdirValid == 0x0))
        goto L_740c;
    else
        goto L_73f6;

L_73f6:
    LOWORD(l2) = lpfl2->iwarpFlt;
    HIWORD(l2) = 0x0;
    goto LRetDiff;

L_740c:
    l2 = -1;

L_741c:
    if ((icolSort > 11))
        goto L_6da9;
    else
        goto L_7424;

L_7424:
    goto L_ffffffff;

L_7445:

L_7448:
    if ((irpt == 0))
        goto L_5bfc;
    else
        goto L_7450;

L_7450:
    if ((irpt == 1))
        goto L_63d5;
    else
        goto L_7458;

L_7458:
    if ((irpt == 2))
        goto L_6d5f;
    else
        goto L_7460;

L_7460:
    if ((irpt == 3))
        goto L_69eb;
    else
        goto L_746b;

L_746b:
    if ((fAscending != 0))
        goto L_747c;
    else
        goto L_7474;

L_7474:
    /* untranslated: iRet = (iRet neg 0) */

L_747c:
    if ((iRet != 0))
        goto L_74c8;
    else
        goto L_7485;

L_7485:
    if ((fTier2 != 0))
        goto L_74c8;
    else
        goto L_748e;

L_748e:
    if ((icolSort != vicolSortPrev))
        goto L_74a4;
    else
        goto L_7499;

L_7499:
    if ((iSubsort == viSubsortPrev))
        goto L_74c8;
    else
        goto L_74a4;

L_74a4:
    if ((vicolSortPrev < 0))
        goto L_74c8;
    else
        goto L_74ae;

L_74ae:
    icolSort = vicolSortPrev;
    iSubsort = viSubsortPrev;
    fAscending = vfAscendingPrev;
    fTier2 = 1;
    goto TryTier2;

L_74c8:

L_74ce:
    return iRet;
}

void ReportColumnPopup(POINT pt, int16_t icol, int16_t fRightBtn) {
    HDC      hdc;
    char     szT[50];
    char     rgsz[32][50];
    int16_t  iBase;
    int16_t  cSubsort;
    int16_t  j;
    int16_t  i;
    int16_t  ibit;
    int16_t  fccolChange;
    int16_t  rgcol[32];
    char     szColTitle[50];
    int16_t  cItems;
    char    *psz[32];
    int16_t  cch;
    int16_t  iRet;
    int16_t  iHide;
    int16_t  iSortLast;
    uint16_t t_merge_753f_0001;
    uint16_t t_merge_7617_0001;
    uint16_t t_merge_79b5_0001;
    uint16_t t_merge_79e0_0001;
    uint16_t t_merge_7a0d_0001;
    uint16_t t_merge_7a32_0001;
    uint16_t t_merge_7a55_0001;

L_74d4:
    cSubsort = 0;
    fccolChange = 0;
    hdc = GetDC(hwndReportDlg);
    DxReportColHdr(vprptCur->irpt, icol, szColTitle, hdc);
    cItems = 0;
    i = 0;
    goto L_76bb;

L_751e:
    if ((i != 0))
        goto L_753c;
    else
        goto L_7536;

L_7536:
    t_merge_753f_0001 = 0x46d;
    goto L_753f;

L_753c:
    t_merge_753f_0001 = 0x46e;

L_753f:
    cch = CchGetString(t_merge_753f_0001, rgsz[cItems]);
    strcpy(rgsz[cItems][cch], szColTitle);
    cItems = (cItems + 1);
    if ((vprptCur->irpt != 0))
        goto L_759a;
    else
        goto L_757f;

L_757f:
    if ((icol == 11))
        goto L_75b0;
    else
        goto L_7588;

L_7588:
    if ((icol == 9))
        goto L_75b0;
    else
        goto L_7591;

L_7591:
    if ((icol == 10))
        goto L_75b0;
    else
        goto L_759a;

L_759a:
    if ((vprptCur->irpt != 1))
        goto L_76b6;
    else
        goto L_75a7;

L_75a7:
    if ((icol != 7))
        goto L_76b6;
    else
        goto L_75b0;

L_75b0:
    strcpy(rgsz[cItems], rgsz[(cItems - 1)]);
    rgsz[(cItems - 1)][0] = 0;
    cItems = (cItems + 1);
    j = 0;
    goto L_7601;

L_75fc:
    j = (j + 1);

L_7601:
    if ((vprptCur->irpt != 1))
        goto L_7614;
    else
        goto L_760e;

L_760e:
    t_merge_7617_0001 = 0x1;
    goto L_7617;

L_7614:
    t_merge_7617_0001 = 0x0;

L_7617:
    if ((j >= (t_merge_7617_0001 + 0x3)))
        goto L_764b;
    else
        goto L_7623;

L_7623:
    strcpy(rgsz[cItems], rgszMinerals[j]);
    cItems = (cItems + 1);
    goto L_75fc;

L_764b:
    rgsz[cItems][0] = -1;
    rgsz[cItems][1] = 0;
    cItems = (cItems + 1);
    strcpy(rgsz[cItems], PszGetCompressedString(idsWeightedAverage));
    cItems = (cItems + 1);
    cItems = (cItems + 1);
    rgsz[cItems][0] = 0;
    cSubsort = 5;

L_76b6:
    i = (i + 1);

L_76bb:
    if ((i < 2))
        goto L_751e;
    else
        goto L_76c5;

L_76c5:
    rgsz[cItems][0] = -1;
    rgsz[cItems][1] = 0;
    cItems = (cItems + 1);
    cch = CchGetString(idsHide, rgsz[cItems]);
    strcpy(rgsz[cItems][cch], szColTitle);
    cch = CchGetString(idsColumn, szT);
    strcat(rgsz[cItems], szT);
    iHide = cItems;
    iSortLast = cItems;
    cItems = (cItems + 1);
    rgsz[cItems][0] = -1;
    rgsz[cItems][1] = 0;
    cItems = (cItems + 1);
    if ((icol != 0))
        goto L_77a3;
    else
        goto L_7798;

L_7798:
    cItems = (cItems - 2);
    iHide = -1;

L_77a3:
    iBase = cItems;
    i = 0;
    ibit = 1;
    goto L_77d1;

L_77bd:
    i = (i + 1);
    ibit = (ibit * 2);

L_77d1:
    if ((i >= vprptCur->cFields))
        goto L_78a1;
    else
        goto L_77e1;

L_77e1:
    if (((ibit & LOWORD(vprptCur->grbitVisible)) != 0x0))
        goto L_77bd;
    else
        goto L_77f7;

L_77f7:
    if (((SIGNHIWORD(ibit) & HIWORD(vprptCur->grbitVisible)) != 0x0))
        goto L_77bd;
    else
        goto L_77ff;

L_77ff:
    DxReportColHdr(vprptCur->irpt, i, szColTitle, hdc);
    cch = CchGetString(idsShow, rgsz[cItems]);
    strcpy(rgsz[cItems][cch], szColTitle);
    cch = CchGetString(idsColumn, szT);
    strcat(rgsz[cItems], szT);
    rgcol[cItems] = i;
    cItems = (cItems + 1);

L_78a1:
    if ((cItems != iBase))
        goto L_78b3;
    else
        goto L_78ae;

L_78ae:
    cItems = (cItems - 1);

L_78b3:
    ReleaseDC(hwndReportDlg, hdc);
    i = 0;
    goto L_7913;

L_78c8:
    if (((uint16_t)(rgsz[i][0]) == 0))
        goto L_78fe;
    else
        goto L_78e0;

L_78e0:
    psz[i] = rgsz[i];
    goto L_790e;

L_78fe:
    psz[i] = 0x0;

L_790e:
    i = (i + 1);

L_7913:
    if ((i < cItems))
        goto L_78c8;
    else
        goto L_7920;

L_7920:
    iRet = PopupMenu(hwndReportDlg, pt.x, pt.y, cItems, 0x0, psz, 0xffff, fRightBtn);
    if ((iRet < 0))
        goto L_7aef;
    else
        goto L_7951;

L_7951:

L_7957:
    gd.fChgReports = 0x1;
    if ((iRet >= iSortLast))
        goto L_7a71;
    else
        goto L_7970;

L_7970:
    vicolSortPrev = vprptCur->icolSort;
    viSubsortPrev = vprptCur->iSubsort;
    vfAscendingPrev = vprptCur->fAscending;
    vprptCur->icolSort = icol;
    if ((cSubsort != 0))
        goto L_79bf;
    else
        goto L_79a2;

L_79a2:
    if ((iRet != 0))
        goto L_79b2;
    else
        goto L_79ac;

L_79ac:
    t_merge_79b5_0001 = 0x1;
    goto L_79b5;

L_79b2:
    t_merge_79b5_0001 = 0x0;

L_79b5:
    vprptCur->fAscending = t_merge_79b5_0001;
    goto L_7a5c;

L_79bf:
    if ((vprptCur->irpt != 1))
        goto L_79dd;
    else
        goto L_79d7;

L_79d7:
    t_merge_79e0_0001 = 0x1;
    goto L_79e0;

L_79dd:
    t_merge_79e0_0001 = 0x0;

L_79e0:
    /* untranslated: vprptCur->iSubsort = (words(ss:[bp-0x740], signhiword(ss:[bp-0x740])) % ((cSubsort + 3) + t_merge_79e0_0001)) */
    if ((vprptCur->irpt != 1))
        goto L_7a0a;
    else
        goto L_7a04;

L_7a04:
    t_merge_7a0d_0001 = 0x1;
    goto L_7a0d;

L_7a0a:
    t_merge_7a0d_0001 = 0x0;

L_7a0d:
    if ((vprptCur->iSubsort <= (t_merge_7a0d_0001 + 0x3)))
        goto L_7a3c;
    else
        goto L_7a1c;

L_7a1c:
    if ((vprptCur->irpt != 1))
        goto L_7a2f;
    else
        goto L_7a29;

L_7a29:
    t_merge_7a32_0001 = 0x1;
    goto L_7a32;

L_7a2f:
    t_merge_7a32_0001 = 0x0;

L_7a32:
    vprptCur->iSubsort = (t_merge_7a32_0001 + 3);

L_7a3c:
    if ((iRet >= (cSubsort + 2)))
        goto L_7a52;
    else
        goto L_7a4c;

L_7a4c:
    t_merge_7a55_0001 = 0x1;
    goto L_7a55;

L_7a52:
    t_merge_7a55_0001 = 0x0;

L_7a55:
    vprptCur->fAscending = t_merge_7a55_0001;

L_7a5c:
    SortReportCache(vprptCur->irpt, icol);
    goto L_7acb;

L_7a71:
    if ((iRet != iHide))
        goto L_7a9b;
    else
        goto L_7a7e;

L_7a7e:
    fccolChange = 1;
    /* untranslated: LOWORD(vprptCur->grbitVisible) = (LOWORD(vprptCur->grbitVisible) & ((0x1 << icol) ~ 0x0)) */
    /* untranslated: HIWORD(vprptCur->grbitVisible) = (HIWORD(vprptCur->grbitVisible) & signhiword(((0x1 << icol) ~ 0x0))) */
    goto L_7acb;

L_7a9b:
    if ((iRet < iBase))
        goto L_7acb;
    else
        goto L_7aa8;

L_7aa8:
    fccolChange = 1;
    LOWORD(vprptCur->grbitVisible) = (LOWORD(vprptCur->grbitVisible) | (0x1 << rgcol[iRet]));
    HIWORD(vprptCur->grbitVisible) = (HIWORD(vprptCur->grbitVisible) | SIGNHIWORD((0x1 << rgcol[iRet])));

L_7acb:
    if ((fccolChange == 0))
        goto L_7ada;
    else
        goto L_7ad5;

L_7ad5:
    SetHScrollBar();

L_7ada:
    InvalidateRect(hwndReportDlg, 0x0, 0x1);

L_7aef:
    return;
}

void InvalidateReport(int16_t irpt, int16_t fReload) {
    int16_t   fResetRpt;
    int16_t   fClearRpt;
    RPT      *prptSav;
    uint16_t *lprgidSav;
    RECT      rc;
    uint16_t  t_merge_7b91_0001;

L_7af6:
    fClearRpt = 0;
    fResetRpt = 0;
    if ((gd.fGeneratingTurn != 0x0))
        goto L_7cd0;
    else
        goto L_7b19;

L_7b19:
    if ((fAi != 0))
        goto L_7cd0;
    else
        goto L_7b20;

L_7b20:

L_7b26:
    if ((hwndReportDlg == 0x0))
        goto L_7ba6;
    else
        goto L_7b30;

L_7b30:
    if ((irpt != vprptCur->irpt))
        goto L_7ba6;
    else
        goto L_7b3f;

L_7b3f:
    GetClientRect(hwndReportDlg, &(rc));
    if ((fReload == 2))
        goto L_7b74;
    else
        goto L_7b58;

L_7b58:
    rc.top = (dyArial8 + 6);
    rc.bottom = (LOWORD(((dyArial8 + 0x4) * vprptCur->cRowsVis)) + rc.top);

L_7b74:
    if ((fReload != 2))
        goto L_7b8e;
    else
        goto L_7b88;

L_7b88:
    t_merge_7b91_0001 = 0x1;
    goto L_7b91;

L_7b8e:
    t_merge_7b91_0001 = 0x0;

L_7b91:
    InvalidateRect(hwndReportDlg, &(rc), t_merge_7b91_0001);
    gd.fRptSafeDraw = 0x1;
    goto L_7c19;

L_7ba6:
    if ((vprptCur != 0x0))
        goto L_7bd1;
    else
        goto L_7bb0;

L_7bb0:
    fClearRpt = fReload;
    if ((irpt != 0))
        goto L_7bc8;
    else
        goto L_7bbf;

L_7bbf:
    vprptCur = vrptPlanet;
    goto L_7c19;

L_7bc8:
    vprptCur = vrptFleet;

L_7bd1:
    if ((fReload == 0))
        goto L_7c19;
    else
        goto L_7bda;

L_7bda:
    if ((vprptCur->irpt == irpt))
        goto L_7c19;
    else
        goto L_7be9;

L_7be9:
    lprgidSav = vlprgidRep;
    prptSav = vprptCur;
    fResetRpt = 1;
    if ((irpt != 0))
        goto L_7c13;
    else
        goto L_7c0a;

L_7c0a:
    vprptCur = vrptPlanet;
    goto L_7c19;

L_7c13:
    vprptCur = vrptFleet;

L_7c19:
    vprptCur->fCached = 0;
    if ((fReload == 0))
        goto L_7c41;
    else
        goto L_7c2b;

L_7c2b:
    SortReportCache(vprptCur->irpt, vprptCur->icolSort);

L_7c41:
    if ((fClearRpt == 0))
        goto L_7c53;
    else
        goto L_7c4a;

L_7c4a:
    vprptCur = 0x0;
    goto L_7cd0;

L_7c53:
    if ((fResetRpt == 0))
        goto L_7c72;
    else
        goto L_7c5c;

L_7c5c:
    vprptCur = prptSav;
    vlprgidRep = lprgidSav;
    goto L_7cd0;

L_7c72:
    if ((fReload == 0))
        goto L_7cd0;
    else
        goto L_7c7b;

L_7c7b:
    if ((hwndReportDlg == 0x0))
        goto L_7cd0;
    else
        goto L_7c85;

L_7c85:
    if ((irpt != vprptCur->irpt))
        goto L_7cd0;
    else
        goto L_7c94;

L_7c94:
    SetScrollRange(vprptCur->hwndVScroll, 0x2, 0x0, (vprptCur->cRows - vprptCur->cRowsVis), 0x0);
    InvalidateRect(hwndReportDlg, 0x0, 0x1);

L_7cd0:
    return;
}

void ExecuteReportClick(POINT pt, int16_t irpt, int16_t icol, int16_t irow) {
    HDC      hdc;
    BTLDATA *lpbd;
    PLANET  *lppl;
    int16_t  i;
    FLEET   *lpfl;
    int16_t  ibit;
    int32_t  rglQuan[4];
    int16_t  xCur;
    int16_t  dxOffset;
    SCAN     scan;

L_7cd6:
    hdc = GetDC(hwndReportDlg);
    goto L_84e9;

L_7cf1:
    lppl = &(lpPlanets[vlprgidPlanet[irow]]);
    if ((hwndProdDlg == 0x0))
        goto L_7d2e;
    else
        goto L_7d22;

L_7d22:
    MessageBeep(0x0);
    goto L_850c;

L_7d2e:
    SelectAdjPlanet(0x0, lppl->id);
    InvalidateReport(0x0, 0x0);

L_8146:
    if ((icol > 12))
        goto L_850c;
    else
        goto L_814e;

L_814e:
    goto L_ffffffff;

L_8174:
    lpfl = rglpfl[vlprgidFleet[irow]];
    if ((mdXferDlg == -1))
        goto L_81bb;
    else
        goto L_81af;

L_81af:
    MessageBeep(0x0);
    goto L_850c;

L_81bb:
    SelectAdjFleet(0x0, lpfl->id);
    InvalidateReport(0x1, 0x0);

L_82aa:
    if (((icol - 3) > 0x5))
        goto L_850c;
    else
        goto L_82b5;

L_82b5:
    goto L_ffffffff;

L_82cd:
    lpbd = BtlDataGet(vlprgidMisc[irow]);
    if ((LOWORD(lpbd) != 0x0))
        goto L_8305;
    else
        goto L_82f9;

L_82f9:
    if ((HIWORD(lpbd) == 0x0))
        goto L_850c;
    else
        goto L_82ff;

L_82ff:

L_8305:
    if ((lpbd->pt.x != sel.scan.pt.x))
        goto L_8328;
    else
        goto L_831f;

L_831f:
    if ((lpbd->pt.y == sel.scan.pt.y))
        goto LDisplayVCRAnyway;
    else
        goto L_8328;

L_8328:
    scan.pt.x = lpbd->pt.x;
    scan.pt.y = lpbd->pt.y;
    scan.grobj = 0x8f;
    ChangeScanSel(&(scan), 0x0);
    CtrPointScan(scan.pt, 0x1);
    InvalidateReport(0x0, 0x0);
    if ((lpbd->pt.x != sel.scan.pt.x))
        goto LDisplayVCRAnyway;
    else
        goto L_838a;

L_838a:
    if ((lpbd->pt.y != sel.scan.pt.y))
        goto LDisplayVCRAnyway;
    else
        goto L_8390;

L_8390:

LDisplayVCRAnyway:
    if ((hwndVCRDlg != 0x0))
        goto L_850c;
    else
        goto L_83a3;

L_83a3:
    BattleVCR(lpbd->id);

L_83b5:
    if ((vprptCur != vrptEFleet))
        goto L_850c;
    else
        goto L_83c0;

L_83c0:
    if ((irow < -2))
        goto L_850c;
    else
        goto L_83c6;

L_83c6:

L_83cc:
    if ((irow >= 0))
        goto L_8421;
    else
        goto L_83d5;

L_83d5:
    if ((irow != -1))
        goto L_83e6;
    else
        goto L_83de;

L_83de:
    irowEFleetCur = (irowEFleetCur + 1);
    goto L_83eb;

L_83e6:
    irowEFleetCur = (irowEFleetCur - 1);

L_83eb:
    if ((irowEFleetCur < vprptCur->cRows))
        goto L_8404;
    else
        goto L_83fb;

L_83fb:
    irowEFleetCur = 0;
    goto L_841b;

L_8404:
    if ((irowEFleetCur >= 0))
        goto L_841b;
    else
        goto L_840e;

L_840e:
    irowEFleetCur = (vprptCur->cRows - 1);

L_841b:
    irow = irowEFleetCur;

L_8421:
    lpfl = rglpfl[vlprgidMisc[irow]];
    if ((mdXferDlg == -1))
        goto L_8468;
    else
        goto L_845c;

L_845c:
    MessageBeep(0x0);
    goto L_850c;

L_8468:
    FFindNearestObject(lpfl->pt, grobjFleet, &(scan));
    scan.ifl = vlprgidMisc[irow];
    ChangeScanSel(&(scan), 0x2);
    FEnsurePointOnScreen(lpfl->pt, 0x1);
    irowEFleetCur = irow;
    InvalidateReport(0x2, 0x0);
    goto L_850c;

L_84e9:
    if ((irpt == 0))
        goto L_7cf1;
    else
        goto L_84f1;

L_84f1:
    if ((irpt == 1))
        goto L_8174;
    else
        goto L_84f9;

L_84f9:
    if ((irpt == 2))
        goto L_83b5;
    else
        goto L_8501;

L_8501:
    if ((irpt == 3))
        goto L_82cd;
    else
        goto L_850c;

L_850c:
    ReleaseDC(hwndReportDlg, hdc);
    return;
}

void DumpUniverse() {
    StringId ids;
    int16_t  i;
    jmp_buf  env[9];
    int16_t  fOpen;
    int16_t  fSuccess;
    int16_t  fSilentSav;
    jmp_buf *penvMemSav[9];
    int16_t  cch;
    uint16_t t_merge_867d_0001;

L_851e:
    fSilentSav = fFileErrSilent;
    fSuccess = 1;
    fOpen = 0;
    if ((LOWORD(game.lid) != 0x0))
        goto L_854b;
    else
        goto L_8541;

L_8541:
    if ((HIWORD(game.lid) == 0x0))
        goto L_8555;
    else
        goto L_854b;

L_854b:
    if ((idPlayer != -1))
        goto L_855d;
    else
        goto L_8555;

L_8555:
    fSuccess = 0;
    goto DisplayStatus;

L_855d:
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0))
        goto L_859f;
    else
        goto L_857d;

L_857d:
    if ((fOpen == 0))
        goto L_858b;
    else
        goto L_8586;

L_8586:
    StreamClose();

L_858b:
    fFileErrSilent = fSilentSav;
    fSuccess = 0;
    penvMem = penvMemSav;
    goto DisplayStatus;

L_859f:
    fFileErrSilent = 1;
    _wsprintf(szWork, "%s.map", &(szBase));
    StreamOpen(szWork, 0x1012);
    fOpen = 1;
    RgToStream(0x163a, 0xc);
    i = 0;
    goto L_85f6;

L_85f2:
    i = (i + 1);

L_85f6:
    if ((i >= game.cPlanMax))
        goto L_8666;
    else
        goto L_8601;

L_8601:
    cch = _wsprintf(szWork, "%d\t%d\t%d\t%s\r\n", (i + 1), rgptPlan[i].x, rgptPlan[i].y, PszGetCompressedPlanet(rgidPlan[i]));
    RgToStream(szWork, cch);
    goto L_85f2;

L_8666:
    StreamClose();

DisplayStatus:
    if ((fSuccess == 0))
        goto L_867a;
    else
        goto L_8674;

L_8674:
    t_merge_867d_0001 = 0x4d6;
    goto L_867d;

L_867a:
    t_merge_867d_0001 = 0x4d7;

L_867d:
    ids = t_merge_867d_0001;
    _wsprintf(szWork, PszGetCompressedString(ids), &(szBase));
    if ((fSuccess == 0))
        goto L_86c1;
    else
        goto L_86ae;

L_86ae:
    AlertSz(szWork, MB_ICONASTERISK);
    goto L_86d1;

L_86c1:
    AlertSz(szWork, MB_ICONHAND);

L_86d1:
    fFileErrSilent = fSilentSav;
    penvMem = penvMemSav;
    return;
}

void DumpPlanets() {
    PLANET  *lpplMac;
    StringId ids;
    PLANET  *lppl;
    char     szFile[256];
    char     szForm[256];
    int16_t  j;
    int16_t  i;
    jmp_buf  env[9];
    int16_t  fOpen;
    int16_t  fSuccess;
    int16_t  fSilentSav;
    jmp_buf *penvMemSav[9];
    char    *psz;
    int16_t  cch;
    int32_t  l;
    float    pct;
    int32_t  rgl[4];
    PART     part;
    uint16_t t_merge_94c5_0001;

L_86e4:
    fSilentSav = fFileErrSilent;
    fSuccess = 1;
    fOpen = 0;
    if ((LOWORD(game.lid) != 0x0))
        goto L_8714;
    else
        goto L_870a;

L_870a:
    if ((HIWORD(game.lid) == 0x0))
        goto L_871e;
    else
        goto L_8714;

L_8714:
    if ((idPlayer != -1))
        goto L_8727;
    else
        goto L_871e;

L_871e:
    fSuccess = 0;
    goto DisplayStatus;

L_8727:
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0))
        goto L_8770;
    else
        goto L_874a;

L_874a:
    if ((fOpen == 0))
        goto L_8759;
    else
        goto L_8754;

L_8754:
    StreamClose();

L_8759:
    fFileErrSilent = fSilentSav;
    fSuccess = 0;
    penvMem = penvMemSav;
    goto DisplayStatus;

L_8770:
    fFileErrSilent = 1;
    if ((gd.fPerPlayerDumps == 0x0))
        goto L_87b6;
    else
        goto L_878e;

L_878e:
    _wsprintf(szFile, "%s.p%d", &(szBase), (idPlayer + 1));
    goto L_87d4;

L_87b6:
    _wsprintf(szFile, "%s.pla", &(szBase));

L_87d4:
    StreamOpen(szFile, 0x1012);
    fOpen = 1;
    j = (gd.fPerPlayerDumps + 2);
    i = 0;
    goto L_8896;

L_880b:
    cch = CchGetString((i + 1244), szForm);
    psz = szForm;

L_882c:
    if (((uint16_t)(*(psz)) == 0))
        goto L_8859;
    else
        goto L_883b;

L_883b:
    if (((uint16_t)(*(psz)) != 42))
        goto L_8851;
    else
        goto L_884a;

L_884a:
    *(psz) = 9;

L_8851:
    psz = (psz + 0x1);
    goto L_882c;

L_8859:
    RgToStream(&(szForm), cch);
    if ((i != (j - 1)))
        goto L_8891;
    else
        goto L_887e;

L_887e:
    RgToStream(szCRLF, 0x2);

L_8891:
    i = (i + 1);

L_8896:
    if ((i < j))
        goto L_880b;
    else
        goto L_88a3;

L_88a3:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_949f;

L_88d2:
    strcpy(szForm, PszGetCompressedPlanet(rgidPlan[lppl->id]));
    RgToStream(&(szForm), strlen(szForm));
    szForm[0] = 9;
    if ((lppl->iPlayer != -1))
        goto L_893b;
    else
        goto L_8924;

L_8924:
    RgToStream(&(szForm), 0x1);
    goto L_898a;

L_893b:
    strcpy(&(szForm[0x1]), PszPlayerName(lppl->iPlayer, 0x1, 0x0, 0x0, 0x0, 0x0));
    RgToStream(&(szForm), strlen(szForm));

L_898a:
    if ((lppl->iPlayer == -1))
        goto L_89ae;
    else
        goto L_8997;

L_8997:
    if ((lppl->fStarbase != 0x0))
        goto L_89b7;
    else
        goto L_89ae;

L_89ae:
    cch = 1;
    goto L_8a03;

L_89b7:
    fstrcpy(&(szForm[0x1]), rglpshdefSB[lppl->iPlayer][lppl->isb].hul.szClass);
    cch = strlen(szForm);

L_8a03:
    RgToStream(&(szForm), cch);
    itoa((game.turn - lppl->turn), &(szForm[0x1]), 0xa);
    RgToStream(&(szForm), strlen(szForm));
    szForm[1] = 0;
    if ((lppl->det != 0x7))
        goto L_8aa3;
    else
        goto L_8a6c;

L_8a6c:
    strcpy(&(szForm[0x1]), PszFromLong((uint32_t)((lppl->rgwtMin[3] * 100)), 0x0));
    goto L_8b08;

L_8aa3:
    if ((lppl->iPlayer == -1))
        goto L_8b08;
    else
        goto L_8ab0;

L_8ab0:
    if ((lppl->det < 0x3))
        goto L_8b08;
    else
        goto L_8ac2;

L_8ac2:
    /* untranslated: l = (uint32_t)(words(0x0, lppl->uPopGuess) * 0x190) */
    strcpy(&(szForm[0x1]), PszFromLong(l, 0x0));

L_8b08:
    RgToStream(&(szForm), strlen(szForm));
    if ((lppl->det >= 0x3))
        goto L_8b40;
    else
        goto L_8b38;

L_8b38:
    szForm[1] = 0;
    goto L_8b71;

L_8b40:
    i = PctPlanetDesirability(lppl, idPlayer);
    _wsprintf(&(szForm[0x1]), PCTDPCTPCT, i);

L_8b71:
    RgToStream(&(szForm), strlen(szForm));
    szForm[1] = 0;
    if ((lppl->det != 0x7))
        goto L_8bd1;
    else
        goto L_8ba6;

L_8ba6:
    FillPlanetProdLB(0x0, 0x0, lppl);
    strcpy(&(szForm[0x1]), szWork);

L_8bd1:
    RgToStream(&(szForm), strlen(szForm));
    if ((lppl->det != 0x7))
        goto L_8cf6;
    else
        goto L_8c01;

L_8c01:
    CalcPctSurvive(lppl, &(pct), 0x0);
    pct = (1 - pct);
    /* untranslated: ss:[bp-0x23a] = 0x64 */
    /* untranslated: ss:[bp-0x238] = 0x0 */
    __ftol();
    /* untranslated: ss:[bp-0x242] = loword(callresult(int32_t)) */
    /* untranslated: ss:[bp-0x240] = signhiword(loword(callresult(int32_t))) */
    /* untranslated: ss:[bp-0x24a] = 0x2710 */
    /* untranslated: ss:[bp-0x248] = 0x0 */
    /* untranslated: ss:[bp-0x252] = 0x64 */
    /* untranslated: ss:[bp-0x250] = 0x0 */
    /* untranslated: call _wsprintf(&szForm[0x1], "%ld\t%ld\t%d.%d%%", (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff), 0x0,
     * (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff), 0x0, loword(__ftol()), loword(__ftol())) -> callresult(int16_t) */
    goto L_8d78;

L_8cf6:
    szForm[2] = 9;
    szForm[1] = 9;
    szForm[3] = 0;
    if ((gd.fPerPlayerDumps == 0x0))
        goto L_8d78;
    else
        goto L_8d1e;

L_8d1e:
    if ((lppl->uDefGuess == 0x0))
        goto L_8d78;
    else
        goto L_8d35;

L_8d35:
    cch = _wsprintf(&(szForm[0x3]), "%d%%", (LOWORD((lppl->uDefGuess * 0x6)) + 0x3));
    szForm[(cch + 3)] = 0;

L_8d78:
    RgToStream(&(szForm), strlen(szForm));
    szForm[1] = 0;
    i = 0;
    goto L_8e11;

L_8da4:
    if ((lppl->det < 0x3))
        goto L_8dee;
    else
        goto L_8db6;

L_8db6:
    strcpy(&(szForm[0x1]), PszFromLong(lppl->rgwtMin[i], 0x0));

L_8dee:
    RgToStream(&(szForm), strlen(szForm));
    i = (i + 1);

L_8e11:
    if ((i < 3))
        goto L_8da4;
    else
        goto L_8e1b;

L_8e1b:
    szForm[1] = 0;
    i = 0;
    goto L_8eaa;

L_8e29:
    if ((lppl->det < 0x4))
        goto L_8e87;
    else
        goto L_8e3b;

L_8e3b:
    EstMineralsMined(lppl, rgl, 0xffffffff, 0x0);
    strcpy(&(szForm[0x1]), PszFromLong(rgl[i], 0x0));

L_8e87:
    RgToStream(&(szForm), strlen(szForm));
    i = (i + 1);

L_8eaa:
    if ((i < 3))
        goto L_8e29;
    else
        goto L_8eb4;

L_8eb4:
    szForm[1] = 0;
    i = 0;
    goto L_8f2b;

L_8ec2:
    if ((lppl->det < 0x3))
        goto L_8f08;
    else
        goto L_8ed4;

L_8ed4:
    strcpy(&(szForm[0x1]), PszFromInt(lppl->rgMinConc[i], 0x0));

L_8f08:
    RgToStream(&(szForm), strlen(szForm));
    i = (i + 1);

L_8f2b:
    if ((i < 3))
        goto L_8ec2;
    else
        goto L_8f35;

L_8f35:
    if ((lppl->det != 0x7))
        goto L_8f77;
    else
        goto L_8f47;

L_8f47:
    strcpy(&(szForm[0x1]), PszFromInt(CResourcesAtPlanet(lppl, idPlayer), 0x0));
    goto L_8f7c;

L_8f77:
    szForm[1] = 0;

L_8f7c:
    RgToStream(&(szForm), strlen(szForm));
    if ((gd.fPerPlayerDumps == 0x0))
        goto L_9488;
    else
        goto L_8fb2;

L_8fb2:
    if ((lppl->det < 0x3))
        goto L_90f3;
    else
        goto L_8fc4;

L_8fc4:
    i = 0;
    goto L_9022;

L_8fcd:
    strcpy(&(szForm[0x1]), PszCalcEnvVar(i, (uint16_t)(lppl->rgEnvVar[i])));
    RgToStream(&(szForm), strlen(szForm));
    i = (i + 1);

L_9022:
    if ((i < 3))
        goto L_8fcd;
    else
        goto L_902c;

L_902c:
    i = 0;
    goto L_908a;

L_9035:
    strcpy(&(szForm[0x1]), PszCalcEnvVar(i, (uint16_t)(lppl->rgEnvVarOrig[i])));
    RgToStream(&(szForm), strlen(szForm));
    i = (i + 1);

L_908a:
    if ((i < 3))
        goto L_9035;
    else
        goto L_9094;

L_9094:
    strcpy(&(szForm[0x1]), PszFromInt(PctPlanetOptValue(lppl, idPlayer), 0x0));
    strcat(&(szForm[0x1]), "%");
    RgToStream(&(szForm), strlen(szForm));
    goto L_9124;

L_90f3:
    szForm[1] = 0;
    i = 0;
    goto L_911a;

L_9101:
    RgToStream(&(szForm), 0x1);
    i = (i + 1);

L_911a:
    if ((i < 7))
        goto L_9101;
    else
        goto L_9124;

L_9124:
    if ((lppl->det != 0x7))
        goto L_9488;
    else
        goto L_9136;

L_9136:
    strcpy(&(szForm[0x1]), PszFromInt(PctPlanetCapacity(lppl), 0x0));
    RgToStream(&(szForm), strlen(szForm));
    strcpy(&(szForm[0x1]), PszFromInt(GetPlanetScannerRange(lppl, &(i)), 0x0));
    RgToStream(&(szForm), strlen(szForm));
    strcpy(&(szForm[0x1]), PszFromInt(i, 0x0));
    RgToStream(&(szForm), strlen(szForm));
    if ((lppl->idFling != 0x0))
        goto L_9225;
    else
        goto L_9217;

L_9217:
    i = 0;
    szForm[1] = 0;
    goto L_925f;

L_9225:
    i = (lppl->iWarpFling + 4);
    strcpy(&(szForm[0x1]), PszGetPlanetName((lppl->idFling + 0xffff)));

L_925f:
    RgToStream(&(szForm), strlen(szForm));
    strcpy(&(szForm[0x1]), PszFromInt(i, 0x0));
    RgToStream(&(szForm), strlen(szForm));
    if ((lppl->idRoute != 0x0))
        goto L_92d3;
    else
        goto L_92cb;

L_92cb:
    szForm[1] = 0;
    goto L_92f7;

L_92d3:
    strcpy(&(szForm[0x1]), PszGetPlanetName((lppl->idRoute + 0xffff)));

L_92f7:
    RgToStream(&(szForm), strlen(szForm));
    if ((lppl->fStarbase == 0x0))
        goto L_93f3;
    else
        goto L_932c;

L_932c:
    i = IStargateFromLppl(lppl);
    if ((i == -1))
        goto L_93f9;
    else
        goto L_9348;

L_9348:
    part.hs.grhst = hstSpecialSB;
    part.hs.iItem = i;
    FLookupPart(&(part));
    strcpy(&(szForm[0x1]), PszFromInt(part.pspecialsb->grAbility2, 0x0));
    RgToStream(&(szForm), strlen(szForm));
    strcpy(&(szForm[0x1]), PszFromInt(part.pspecialsb->grAbility, 0x0));
    RgToStream(&(szForm), strlen(szForm));

L_93f3:
    i = -1;

L_93f9:
    if ((i != -1))
        goto L_9416;
    else
        goto L_9403;

L_9403:
    RgToStream(0x167a, 0x4);

L_9416:
    if ((lppl->fStarbase == 0x0))
        goto L_9446;
    else
        goto L_942d;

L_942d:
    i = lppl->pctDp;
    goto L_944c;

L_9446:
    i = 0;

L_944c:
    strcpy(&(szForm[0x1]), PszFromInt(i, 0x0));
    RgToStream(&(szForm), strlen(szForm));

L_9488:
    RgToStream(szCRLF, 0x2);
    lppl = (lppl + 0x1);

L_949f:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_88d2;
    else
        goto L_94ad;

L_94ad:
    StreamClose();

DisplayStatus:
    if ((fSuccess == 0))
        goto L_94c2;
    else
        goto L_94bc;

L_94bc:
    t_merge_94c5_0001 = 0x4d8;
    goto L_94c5;

L_94c2:
    t_merge_94c5_0001 = 0x4d9;

L_94c5:
    ids = t_merge_94c5_0001;
    _wsprintf(szWork, PszGetCompressedString(ids), &(szFile));
    if ((fSuccess == 0))
        goto L_950b;
    else
        goto L_94f8;

L_94f8:
    AlertSz(szWork, MB_ICONASTERISK);
    goto L_951b;

L_950b:
    AlertSz(szWork, MB_ICONHAND);

L_951b:
    fFileErrSilent = fSilentSav;
    penvMem = penvMemSav;
    return;
}

void DumpFleets() {
    int16_t  iplr;
    StringId ids;
    char     szFile[256];
    char     szForm[256];
    int16_t  ifl;
    FLEET   *lpfl;
    int16_t  j;
    int16_t  i;
    jmp_buf  env[9];
    int16_t  fOpen;
    int16_t  fSuccess;
    int16_t  fSilentSav;
    jmp_buf *penvMemSav[9];
    char    *psz;
    int16_t  cch;
    int32_t  l;
    uint16_t t_merge_a157_0001;

L_9530:
    fSilentSav = fFileErrSilent;
    fSuccess = 1;
    fOpen = 0;
    iplr = idPlayer;
    if ((LOWORD(game.lid) != 0x0))
        goto L_9566;
    else
        goto L_955c;

L_955c:
    if ((HIWORD(game.lid) == 0x0))
        goto L_9570;
    else
        goto L_9566;

L_9566:
    if ((idPlayer != -1))
        goto L_9579;
    else
        goto L_9570;

L_9570:
    fSuccess = 0;
    goto DisplayStatus;

L_9579:
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0))
        goto L_95c2;
    else
        goto L_959c;

L_959c:
    if ((fOpen == 0))
        goto L_95ab;
    else
        goto L_95a6;

L_95a6:
    StreamClose();

L_95ab:
    fFileErrSilent = fSilentSav;
    fSuccess = 0;
    penvMem = penvMemSav;
    goto DisplayStatus;

L_95c2:
    fFileErrSilent = 1;
    if ((gd.fPerPlayerDumps == 0x0))
        goto L_9608;
    else
        goto L_95e0;

L_95e0:
    _wsprintf(szFile, "%s.f%d", &(szBase), (idPlayer + 1));
    goto L_9626;

L_9608:
    _wsprintf(szFile, "%s.fle", &(szBase));

L_9626:
    StreamOpen(szFile, 0x1012);
    fOpen = 1;
    j = (gd.fPerPlayerDumps + 2);
    i = 0;
    goto L_96e8;

L_965d:
    cch = CchGetString((i + 1247), szForm);
    psz = szForm;

L_967e:
    if (((uint16_t)(*(psz)) == 0))
        goto L_96ab;
    else
        goto L_968d;

L_968d:
    if (((uint16_t)(*(psz)) != 42))
        goto L_96a3;
    else
        goto L_969c;

L_969c:
    *(psz) = 9;

L_96a3:
    psz = (psz + 0x1);
    goto L_967e;

L_96ab:
    RgToStream(&(szForm), cch);
    if ((i != (j - 1)))
        goto L_96e3;
    else
        goto L_96d0;

L_96d0:
    RgToStream(szCRLF, 0x2);

L_96e3:
    i = (i + 1);

L_96e8:
    if ((i < j))
        goto L_965d;
    else
        goto L_96f5;

L_96f5:
    iplr = idPlayer;
    ifl = 0;
    goto L_9709;

L_9704:
    ifl = (ifl + 1);

L_9709:
    if ((ifl >= cFleet))
        goto L_a13f;
    else
        goto L_9715;

L_9715:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_9748;
    else
        goto L_9740;

L_9740:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_a13f;
    else
        goto L_9748;

L_9748:
    idPlayer = -1;
    psz = PszGetFleetName(lpfl->id);
    idPlayer = iplr;
    RgToStream(&(psz), strlen(psz));
    szForm[0] = 9;
    strcpy(&(szForm[0x1]), PszFromInt(lpfl->pt.x, 0x0));
    RgToStream(&(szForm), strlen(szForm));
    strcpy(&(szForm[0x1]), PszFromInt(lpfl->pt.y, 0x0));
    RgToStream(&(szForm), strlen(szForm));
    if ((lpfl->idPlanet != -1))
        goto L_9820;
    else
        goto L_9817;

L_9817:
    cch = 1;
    goto L_9858;

L_9820:
    psz = PszGetPlanetName(lpfl->idPlanet);
    strcpy(&(szForm[0x1]), psz);
    cch = (strlen(psz) + 1);

L_9858:
    RgToStream(&(szForm), cch);
    if ((lpfl->det != 0x7))
        goto L_98a5;
    else
        goto L_9880;

L_9880:
    strcpy(&(szForm[0x1]), PszGetDestName(lpfl, 0x0));
    goto L_9955;

L_98a5:
    if ((gd.fPerPlayerDumps == 0x0))
        goto L_9950;
    else
        goto L_98bd;

L_98bd:
    if ((lpfl->det >= 0x7))
        goto L_9950;
    else
        goto L_98d0;

L_98d0:
    if ((lpfl->fdirValid == 0x0))
        goto L_9950;
    else
        goto L_98eb;

L_98eb:
    strcpy(&(szForm[0x1]), PszFromInt(lpfl->dirFltX, 0x0));
    strcat(szForm, ".");
    strcat(szForm, PszFromInt(lpfl->dirFltY, 0x0));
    goto L_9955;

L_9950:
    szForm[1] = 0;

L_9955:
    RgToStream(&(szForm), strlen(szForm));
    if ((LOWORD(rglpbtlplan[lpfl->iplr]) != 0x0))
        goto L_999a;
    else
        goto L_9990;

L_9990:
    if ((HIWORD(rglpbtlplan[lpfl->iplr]) == 0x0))
        goto L_99e1;
    else
        goto L_999a;

L_999a:
    fstrcpy(&(szForm[0x1]), rglpbtlplan[lpfl->iplr][lpfl->iplan].szName);
    goto L_99e6;

L_99e1:
    szForm[1] = 0;

L_99e6:
    RgToStream(&(szForm), strlen(szForm));
    l = 0;
    i = 0;
    goto L_9a41;

L_9a19:
    l = (l + (uint32_t)(lpfl->rgcsh[i]));
    i = (i + 1);

L_9a41:
    if ((i < 16))
        goto L_9a19;
    else
        goto L_9a4b;

L_9a4b:
    strcpy(&(szForm[0x1]), PszFromLong(l, 0x0));
    RgToStream(&(szForm), strlen(szForm));
    i = 0;
    goto L_9af1;

L_9a94:
    strcpy(&(szForm[0x1]), PszFromLong(lpfl->rgwtMin[i], 0x0));
    RgToStream(&(szForm), strlen(szForm));
    i = (i + 1);

L_9af1:
    if ((i < 5))
        goto L_9a94;
    else
        goto L_9afb;

L_9afb:
    if ((gd.fPerPlayerDumps == 0x0))
        goto L_a129;
    else
        goto L_9b13;

L_9b13:
    strcpy(&(szForm[0x1]), PszFromInt((lpfl->iPlayer + 1), 0x0));
    RgToStream(&(szForm), strlen(szForm));
    if ((lpfl->cord <= 1))
        goto L_9b8e;
    else
        goto L_9b65;

L_9b65:
    strcpy(&(szForm[0x1]), PszGetETA(0x0, lpfl, 0x0));
    goto L_9b98;

L_9b8e:
    szForm[1] = 48;
    szForm[2] = 0;

L_9b98:
    RgToStream(&(szForm), strlen(szForm));
    if ((lpfl->cord >= 2))
        goto L_9c0d;
    else
        goto L_9bc4;

L_9bc4:
    if ((lpfl->det >= 0x7))
        goto L_9c04;
    else
        goto L_9bd7;

L_9bd7:
    if ((lpfl->fdirValid == 0x0))
        goto L_9c04;
    else
        goto L_9bf2;

L_9bf2:
    i = lpfl->iwarpFlt;
    goto L_9c28;

L_9c04:
    i = 0;

L_9c0d:
    i = lpfl->lpplord->rgord[0x1].iWarp;

L_9c28:
    strcpy(&(szForm[0x1]), PszFromInt(i, 0x0));
    RgToStream(&(szForm), strlen(szForm));
    strcpy(&(szForm[0x1]), PszFromLong(WtFromLpfl(lpfl), 0x0));
    RgToStream(&(szForm), strlen(szForm));
    strcpy(&(szForm[0x1]), PszFromInt(PctCloakFromLpfl(lpfl), 0x0));
    RgToStream(&(szForm), strlen(szForm));
    j = GetFleetScannerRange(lpfl, &(i), 0x0, 0x0);
    if ((j != -1))
        goto L_9d28;
    else
        goto L_9d22;

L_9d22:
    j = 0;

L_9d28:
    strcpy(&(szForm[0x1]), PszFromInt(j, 0x0));
    RgToStream(&(szForm), strlen(szForm));
    strcpy(&(szForm[0x1]), PszFromInt(i, 0x0));
    RgToStream(&(szForm), strlen(szForm));
    if ((lpfl->det != 0x7))
        goto L_9dd9;
    else
        goto L_9db3;

L_9db3:
    strcpy(&(szForm[0x1]), PszGetTaskName(lpfl, &(i)));
    goto L_9dde;

L_9dd9:
    szForm[1] = 0;

L_9dde:
    RgToStream(&(szForm), strlen(szForm));
    strcpy(&(szForm[0x1]), PszFromLong(CMineFromLpfl(lpfl), 0x0));
    RgToStream(&(szForm), strlen(szForm));
    strcpy(&(szForm[0x1]), PszFromLong(CMineSweepFromLpfl(lpfl), 0x0));
    RgToStream(&(szForm), strlen(szForm));
    strcpy(&(szForm[0x1]), PszFromLong(CLayMinesFromLpfl(lpfl, 0xffff, 0xffff), 0x0));
    RgToStream(&(szForm), strlen(szForm));
    strcpy(&(szForm[0x1]), PszFromLong(PctTerraFromLpfl(lpfl), 0x0));
    RgToStream(&(szForm), strlen(szForm));
    l = 0;
    i = 0;
    goto L_9fdb;

L_9f41:
    if ((lpfl->rgcsh[i] == 0))
        goto L_9fd6;
    else
        goto L_9f61;

L_9f61:
    j = ((*(LphuldefFromId(rglpshdef[lpfl->iPlayer][i].hul.ihuldef) + 0x7b) >> 0xa) & 0xf);
    if ((j <= 1))
        goto L_9fb3;
    else
        goto L_9fa9;

L_9fa9:
    if ((j < 6))
        goto L_9fd6;
    else
        goto L_9fb3;

L_9fb3:
    l = (l + (uint32_t)(lpfl->rgcsh[i]));

L_9fd6:
    i = (i + 1);

L_9fdb:
    if ((i < 16))
        goto L_9f41;
    else
        goto L_9fe5;

L_9fe5:
    strcpy(&(szForm[0x1]), PszFromLong(l, 0x0));
    RgToStream(&(szForm), strlen(szForm));
    j = 2;
    goto L_a11f;

L_a02e:
    l = 0;
    i = 0;
    goto L_a0d0;

L_a043:
    if ((lpfl->rgcsh[i] == 0))
        goto L_a0cb;
    else
        goto L_a063;

L_a063:
    if ((((*(LphuldefFromId(rglpshdef[lpfl->iPlayer][i].hul.ihuldef) + 0x7b) >> 0xa) & 0xf) != j))
        goto L_a0cb;
    else
        goto L_a0a8;

L_a0a8:
    l = (l + (uint32_t)(lpfl->rgcsh[i]));

L_a0cb:
    i = (i + 1);

L_a0d0:
    if ((i < 16))
        goto L_a043;
    else
        goto L_a0da;

L_a0da:
    strcpy(&(szForm[0x1]), PszFromLong(l, 0x0));
    RgToStream(&(szForm), strlen(szForm));
    j = (j + 1);

L_a11f:
    if ((j < 6))
        goto L_a02e;
    else
        goto L_a129;

L_a129:
    RgToStream(szCRLF, 0x2);
    goto L_9704;

L_a13f:
    StreamClose();

DisplayStatus:
    if ((fSuccess == 0))
        goto L_a154;
    else
        goto L_a14e;

L_a14e:
    t_merge_a157_0001 = 0x4da;
    goto L_a157;

L_a154:
    t_merge_a157_0001 = 0x4db;

L_a157:
    ids = t_merge_a157_0001;
    _wsprintf(szWork, PszGetCompressedString(ids), &(szFile));
    if ((fSuccess == 0))
        goto L_a19d;
    else
        goto L_a18a;

L_a18a:
    AlertSz(szWork, MB_ICONASTERISK);
    goto L_a1ad;

L_a19d:
    AlertSz(szWork, MB_ICONHAND);

L_a1ad:
    fFileErrSilent = fSilentSav;
    penvMem = penvMemSav;
    return;
}

int16_t PrintMapDlg(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam) {
    int16_t  i;
    RECT     rc;
    HWND     hwndEdit;
    uint16_t t_merge_a3aa_0001;

L_a1c2:
    goto L_a473;

L_a1d1:
    i = 0;
    goto L_a242;

L_a1d9:
    hwndEdit = GetDlgItem(hwnd, (i + 268));
    SendMessage(hwndEdit, CB_SETEXTENDEDUI, 0x1, 0x0);
    SendMessage(hwndEdit, WM_SETFONT, rghfontArial8[1], 0x0);
    LOWORD(szWork) = LOBYTE((vrgcPrintMapPage[i] + 0x30));
    szWork[1] = 0;
    SetWindowText(hwndEdit, szWork);
    i = (i + 1);

L_a242:
    if ((i < 2))
        goto L_a1d9;
    else
        goto L_a24b;

L_a24b:
    StickyDlgPos(hwnd, &(ptStickyPrintMapDlg), 0x1);
    return 0x1;

L_a264:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_a28c:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_a496;
    else
        goto L_a2a8;

L_a2a8:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_a2c1:
    if ((wParam == 0x1))
        goto L_a2d3;
    else
        goto L_a2ca;

L_a2ca:
    if ((wParam != 0x2))
        goto L_a3b9;
    else
        goto L_a2d3;

L_a2d3:
    if ((wParam != 0x1))
        goto L_a382;
    else
        goto L_a2dc;

L_a2dc:
    i = 0;
    goto L_a379;

L_a2e4:
    hwndEdit = GetDlgItem(hwnd, (i + 268));
    GetWindowText(hwndEdit, szWork, 0xa);
    if (((uint16_t)(LOWORD(szWork)) == 0x0))
        goto L_a339;
    else
        goto L_a315;

L_a315:
    if (((uint16_t)(szWork[1]) != 0))
        goto L_a339;
    else
        goto L_a321;

L_a321:
    if (((uint16_t)(LOWORD(szWork)) <= 0x30))
        goto L_a339;
    else
        goto L_a32d;

L_a32d:
    if (((uint16_t)(LOWORD(szWork)) <= 0x39))
        goto L_a365;
    else
        goto L_a339;

L_a339:
    AlertSz(PszFormatIds(idsMustSpecifyNumberBetween19, 0x0), MB_ICONHAND);
    SetFocus(hwndEdit);
    goto L_a382;

L_a365:
    vrgcPrintMapPage[i] = ((uint16_t)(LOWORD(szWork)) - 48);
    i = (i + 1);

L_a379:
    if ((i < 2))
        goto L_a2e4;
    else
        goto L_a382;

L_a382:
    StickyDlgPos(hwnd, &(ptStickyPrintMapDlg), 0x0);
    if ((wParam != 0x1))
        goto L_a3a7;
    else
        goto L_a3a1;

L_a3a1:
    t_merge_a3aa_0001 = 0x1;
    goto L_a3aa;

L_a3a7:
    t_merge_a3aa_0001 = 0x0;

L_a3aa:
    EndDialog(hwnd, t_merge_a3aa_0001);
    return 0x1;

L_a3b9:
    if ((wParam != 0x76))
        goto L_a3e6;
    else
        goto L_a3c2;

L_a3c2:
    WinHelp(hwnd, szHelpFile, 0x1, 0xc3c);
    return 0x1;

L_a3e6:
    if ((wParam == 0x10c))
        goto L_a3fa;
    else
        goto L_a3f0;

L_a3f0:
    if ((wParam != 0x10d))
        goto L_a496;
    else
        goto L_a3fa;

L_a3fa:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x400))
        goto L_a496;
    else
        goto L_a416;

L_a416:
    GetWindowText(LOWORD(lParam), szWork, 0xa);
    if (((uint16_t)(LOWORD(szWork)) == 0x0))
        goto L_a496;
    else
        goto L_a439;

L_a439:
    if (((uint16_t)(LOWORD(szWork)) <= 0x30))
        goto L_a451;
    else
        goto L_a445;

L_a445:
    if (((uint16_t)(LOWORD(szWork)) <= 0x39))
        goto L_a496;
    else
        goto L_a451;

L_a451:
    MessageBeep(0x0);
    SetWindowText(LOWORD(lParam), "");

L_a473:
    if ((msg == WM_ERASEBKGND))
        goto L_a264;
    else
        goto L_a47b;

L_a47b:
    if ((msg == WM_CTLCOLOR))
        goto L_a28c;
    else
        goto L_a483;

L_a483:
    if ((msg == WM_INITDIALOG))
        goto L_a1d1;
    else
        goto L_a48b;

L_a48b:
    if ((msg == WM_COMMAND))
        goto L_a2c1;
    else
        goto L_a496;

L_a496:
    return 0x0;
}
