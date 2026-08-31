#include "common.h"

void BattleVCR(int16_t iBattle) {
    int16_t (**lpProc)();
    jmp_buf *penvMemSav[9];
    jmp_buf  env[9];
    HB      *lphb;

L_0000:
    lpProc = 0x0;
    penvMemSav = penvMem;
    viStepVCRCur = 0xffff;
    gd.fVCRTimer = 0x0;
    if ((gd.mdScreenSize < 0x2))
        goto L_004d;
    else
        goto L_003e;

L_003e:
    dxyVCRBoard = 0x2a1;
    dxyVCRSquare = 0x40;
    goto L_0059;

L_004d:
    dxyVCRBoard = 0x161;
    dxyVCRSquare = 0x20;

L_0059:
    lphb = rglphb[0xb];
    vlpbdVCR = &(*(lphb + 0x12));

L_0079:
    if ((vlpbdVCR->id == iBattle))
        goto L_00ee;
    else
        goto L_0088;

L_0088:
    if ((vlpbdVCR->id != 0xffff))
        goto L_00d1;
    else
        goto L_0095;

L_0095:
    lphb = lphb->lphbNext;
    if ((LOWORD(lphb) != 0x0))
        goto L_00bb;
    else
        goto L_00af;

L_00af:
    if ((HIWORD(lphb) == 0x0))
        goto L_0286;
    else
        goto L_00b5;

L_00b5:

L_00bb:
    vlpbdVCR = &(*(lphb + 0x12));
    goto L_0079;

L_00d1:
    vlpbdVCR = ((uint8_t *)(vlpbdVCR) + vlpbdVCR->cbData);

L_00ee:
    vlpbdVCRNext = ((uint8_t *)(vlpbdVCR) + vlpbdVCR->cbData);
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_014c;
    else
        goto L_0122;

L_0122:
    penvMem = penvMemSav;
    AlertSz(PszFormatIds(idsMemory, 0x0), MB_ICONHAND);
    goto LCleanup;

L_014c:
    vrgtok = LpAlloc(LOWORD((vlpbdVCR->ctok * 0x1d)), htMisc);
    vrgdpVCR = LpAlloc((vlpbdVCR->ctok * 0x4), htMisc);
    vcStepVCR = (SetVCRBoard(0x7530) + 0xffff);
    vcRound = viRound;
    SetVCRBoard(0xffff);
    if ((gd.fTutorial == 0x0))
        goto L_01cf;
    else
        goto L_01ca;

L_01ca:
    AdvanceTutor();

L_01cf:
    lpProc = MakeProcInstance(VCRDlg, hInst);
    DialogBox(hInst, IDD_VCR, hwndFrame, &(lpProc));

LCleanup:
    if ((LOWORD(lpProc) != 0x0))
        goto L_0213;
    else
        goto L_020a;

L_020a:
    if ((HIWORD(lpProc) == 0x0))
        goto L_021e;
    else
        goto L_0213;

L_0213:
    FreeProcInstance(&(lpProc));

L_021e:
    if ((LOWORD(vrgtok) != 0x0))
        goto L_0232;
    else
        goto L_0228;

L_0228:
    if ((HIWORD(vrgtok) == 0x0))
        goto L_0246;
    else
        goto L_0232;

L_0232:
    FreeLp(&(vrgtok), htMisc);

L_0246:
    if ((LOWORD(vrgdpVCR) != 0x0))
        goto L_025a;
    else
        goto L_0250;

L_0250:
    if ((HIWORD(vrgdpVCR) == 0x0))
        goto L_026e;
    else
        goto L_025a;

L_025a:
    FreeLp(&(vrgdpVCR), htMisc);

L_026e:
    vrgtok = 0x0;
    vrgdpVCR = 0x0;

L_0286:
    return;
}

int16_t CBattles() {
    BTLDATA *lpbd;
    HB      *lphb;
    int16_t  cBattles;

L_028c:
    cBattles = 0x0;
    lphb = rglphb[0xb];
    if ((LOWORD(lphb) != 0x0))
        goto L_02bf;
    else
        goto L_02b0;

L_02b0:
    if ((HIWORD(lphb) != 0x0))
        goto L_02bf;
    else
        goto L_02b9;

L_02b9:
    return 0x0;

L_02bf:
    lpbd = &(*(lphb + 0x12));

L_02d3:
    if ((lpbd->id != 0xffff))
        goto L_0329;
    else
        goto L_02df;

L_02df:
    lphb = lphb->lphbNext;
    if ((LOWORD(lphb) != 0x0))
        goto L_0302;
    else
        goto L_02f9;

L_02f9:
    if ((HIWORD(lphb) == 0x0))
        goto L_030f;
    else
        goto L_0302;

L_0302:
    if ((lphb->ibTop > 0x10))
        goto L_0315;
    else
        goto L_030f;

L_030f:
    return cBattles;

L_0315:
    lpbd = &(*(lphb + 0x12));
    goto L_02d3;

L_0329:
    if ((lpbd->cbData != 0x0))
        goto L_033f;
    else
        goto L_0336;

L_0336:
    return cBattles;

L_033f:
    lpbd = ((uint8_t *)(lpbd) + lpbd->cbData);
    cBattles = (cBattles + 0x1);
}

BTLDATA *BtlDataGet(int16_t i) {
    BTLDATA *lpbd;
    HB      *lphb;
    int16_t  t_0419;

L_0362:
    lphb = rglphb[0xb];
    if ((LOWORD(lphb) != 0x0))
        goto L_0393;
    else
        goto L_0381;

L_0381:
    if ((HIWORD(lphb) != 0x0))
        goto L_0393;
    else
        goto L_038a;

L_038a:
    return 0x0;

L_0393:
    lpbd = &(*(lphb + 0x12));

L_03a7:
    if ((lpbd->id != 0xffff))
        goto L_0400;
    else
        goto L_03b3;

L_03b3:
    lphb = lphb->lphbNext;
    if ((LOWORD(lphb) != 0x0))
        goto L_03d6;
    else
        goto L_03cd;

L_03cd:
    if ((HIWORD(lphb) == 0x0))
        goto L_03e3;
    else
        goto L_03d6;

L_03d6:
    if ((lphb->ibTop > 0x10))
        goto L_03ec;
    else
        goto L_03e3;

L_03e3:
    return 0x0;

L_03ec:
    lpbd = &(*(lphb + 0x12));
    goto L_03a7;

L_0400:
    if ((lpbd->cbData != 0x0))
        goto L_0419;
    else
        goto L_040d;

L_040d:
    return 0x0;

L_0419:
    t_0419 = i;
    i = (i - 0x1);
    if ((t_0419 > 0x0))
        goto L_0431;
    else
        goto L_0428;

L_0428:
    return lpbd;

L_0431:
    lpbd = ((uint8_t *)(lpbd) + lpbd->cbData);
}

int32_t CBattleUnits(BTLDATA *lpbd, uint16_t grbitBU) {
    TOK    *lptok;
    int16_t ctok;
    int32_t lUnits;
    int16_t i;
    int16_t imd;

L_0450:
    ctok = lpbd->ctok;
    lUnits = 0x0;
    i = 0x0;
    goto L_0614;

L_0478:
    lptok = &(lpbd->rgtok[i]);
    if ((lptok->iplr != idPlayer))
        goto L_04c4;
    else
        goto L_04b0;

L_04b0:
    if (((grbitBU & 0x1) == 0x0))
        goto L_0610;
    else
        goto L_04bb;

L_04bb:

L_04c4:
    if (((grbitBU & 0x2) == 0x0))
        goto L_0610;
    else
        goto L_04cf;

L_04cf:

L_04d5:
    if (((grbitBU & 0x4) != 0x0))
        goto L_04f8;
    else
        goto L_04e3;

L_04e3:
    if ((lptok->ishdef >= 0x10))
        goto L_0610;
    else
        goto L_04f2;

L_04f2:

L_04f8:
    if (((grbitBU & 0xf8) == 0xf8))
        goto L_0600;
    else
        goto L_0506;

L_0506:
    if ((lptok->ishdef >= 0x10))
        goto L_0600;
    else
        goto L_0518;

L_0518:
    imd = ((*(LphuldefFromId(rglpshdef[lptok->iplr][lptok->ishdef].hul.ihuldef) + 0x7b) >> 0xa) & 0xf);
    if ((imd <= 0x1))
        goto L_057b;
    else
        goto L_0572;

L_0572:
    if ((imd < 0x6))
        goto L_058f;
    else
        goto L_057b;

L_057b:
    if (((grbitBU & 0x8) == 0x0))
        goto L_0610;
    else
        goto L_0586;

L_0586:

L_058f:
    if ((imd != 0x2))
        goto L_05ac;
    else
        goto L_0598;

L_0598:
    if (((grbitBU & 0x10) == 0x0))
        goto L_0610;
    else
        goto L_05a3;

L_05a3:

L_05ac:
    if ((imd != 0x3))
        goto L_05c9;
    else
        goto L_05b5;

L_05b5:
    if (((grbitBU & 0x20) == 0x0))
        goto L_0610;
    else
        goto L_05c0;

L_05c0:

L_05c9:
    if ((imd != 0x5))
        goto L_05e6;
    else
        goto L_05d2;

L_05d2:
    if (((grbitBU & 0x40) == 0x0))
        goto L_0610;
    else
        goto L_05dd;

L_05dd:

L_05e6:
    if ((imd != 0x4))
        goto L_0600;
    else
        goto L_05ef;

L_05ef:
    if (((grbitBU & 0x80) == 0x0))
        goto L_0610;
    else
        goto L_05fa;

L_05fa:

L_0600:
    LOWORD(lUnits) = (LOWORD(lUnits) + lptok->csh);
    HIWORD(lUnits) = (HIWORD(lUnits) + 0x0);

L_0610:
    i = (i + 0x1);

L_0614:
    if ((i < ctok))
        goto L_0478;
    else
        goto L_061f;

L_061f:

L_0628:
    return lUnits;
}

int32_t CBattleKills(BTLDATA *lpbd, int16_t fOurDead) {
    int32_t  cKilled;
    BTLDATA *lpbdNext;
    int16_t  i;
    BTLREC  *lpbr;
    int16_t  cKill;

L_062e:
    lpbr = &(lpbd->rgtok[lpbd->ctok]);
    lpbdNext = ((uint8_t *)(lpbd) + lpbd->cbData);
    cKilled = 0x0;

L_0684:
    if ((LOWORD(lpbr) >= LOWORD(lpbdNext)))
        goto L_0799;
    else
        goto L_0692;

L_0692:
    cKill = lpbr->ctok;
    i = 0x0;
    goto L_076b;

L_06a4:
    if ((lpbr->rgkill[i].cshKill <= 0x0))
        goto L_0767;
    else
        goto L_06c6;

L_06c6:
    if ((lpbd->rgtok[lpbr->rgkill[i].itok].iplr != idPlayer))
        goto L_0739;
    else
        goto L_0708;

L_0708:
    if ((fOurDead == 0x0))
        goto L_0767;
    else
        goto L_0711;

L_0711:
    LOWORD(cKilled) = (LOWORD(cKilled) + lpbr->rgkill[i].cshKill);
    HIWORD(cKilled) = (HIWORD(cKilled) + 0x0);

L_0739:
    if ((fOurDead != 0x0))
        goto L_0767;
    else
        goto L_0742;

L_0742:
    LOWORD(cKilled) = (LOWORD(cKilled) + lpbr->rgkill[i].cshKill);
    HIWORD(cKilled) = (HIWORD(cKilled) + 0x0);

L_0767:
    i = (i + 0x1);

L_076b:
    if ((i < cKill))
        goto L_06a4;
    else
        goto L_0776;

L_0776:
    lpbr = &(lpbr->rgkill[lpbr->ctok]);
    goto L_0684;

L_0799:

L_07a2:
    return cKilled;
}

int32_t LdpFromItokDv(int16_t itok, DV *lpdv) {
    DV       dv;
    uint16_t dpShdef;
    int16_t  csh;
    int32_t  dp;

L_07a8:
    if ((LOWORD(lpdv) != 0x0))
        goto L_07c3;
    else
        goto L_07ba;

L_07ba:
    if ((HIWORD(lpdv) == 0x0))
        goto L_07cf;
    else
        goto L_07c3;

L_07c3:
    dv.dp = lpdv->dp;
    goto L_07d4;

L_07cf:
    dv.dp = 0x0;

L_07d4:
    dpShdef = LpshdefFromTok(&(vrgtok[itok]))->hul.dp;
    /* untranslated: dp = (uint32_t)(words(0x0, dpShdef) * words(0x0, vrgtok[itok].csh)) */
    if ((dv.dp == 0x0))
        goto L_08c9;
    else
        goto L_0830;

L_0830:
    /* untranslated: csh = loword((int32_t)((uint32_t)(words(0x0, vrgtok[itok].csh) * words(0x0, dv.pctSh)) / 0x64)) */
    if ((csh > 0x0))
        goto L_087b;
    else
        goto L_0876;

L_0876:
    csh = 0x1;

L_087b:
    /* untranslated: dp = (dp - (int32_t)((uint32_t)((int32_t)((uint32_t)(words(0x0, dpShdef) * words(0x0, dv.pctDp)) / 0xa) * sext16to32(csh)) / 0x32)) */

L_08c9:

L_08d2:
    return dp;
}

int16_t SetVCRBoard(int16_t iStep) {
    TOK    *ptok;
    int16_t i;
    int16_t itok;

L_08d8:
    if ((iStep != viStepVCRCur))
        goto L_08fc;
    else
        goto L_08ec;

L_08ec:
    if ((viStepVCRCur == 0xffff))
        goto L_08fc;
    else
        goto L_08f6;

L_08f6:
    return iStep;

L_08fc:
    if ((viStepVCRCur > iStep))
        goto L_0912;
    else
        goto L_0908;

L_0908:
    if ((viStepVCRCur != 0xffff))
        goto L_0a11;
    else
        goto L_0912;

L_0912:
    i = 0x0;
    goto L_091e;

L_091a:
    i = (i + 0x1);

L_091e:
    if ((i >= vlpbdVCR->ctok))
        goto L_09b9;
    else
        goto L_0931;

L_0931:
    vrgtok[i] = vlpbdVCR->rgtok[i];
    vrgdpVCR[i] = LdpFromItokDv(i, &(vrgtok[i].dv));
    goto L_091a;

L_09b9:
    viStepVCRCur = 0xffff;
    viRound = 0x0;
    viVCRFocus = 0x0;
    vbrcVCRFocus = vrgtok->brc;
    vlpbrVCR = &(vlpbdVCR->rgtok[vlpbdVCR->ctok]);

L_0a0c:
    viStepVCRCur = (viStepVCRCur + 0x1);

L_0a11:
    if ((viStepVCRCur >= iStep))
        goto L_0e7f;
    else
        goto L_0a1d;

L_0a1d:
    if ((LOWORD(vlpbrVCR) >= LOWORD(vlpbdVCRNext)))
        goto L_0e7f;
    else
        goto L_0a2d;

L_0a2d:
    if ((viStepVCRCur < 0x0))
        goto L_0d15;
    else
        goto L_0a37;

L_0a37:
    i = 0x0;
    goto L_0a43;

L_0a3f:
    i = (i + 0x1);

L_0a43:
    if ((i >= vlpbrVCR->ctok))
        goto L_0c54;
    else
        goto L_0a53;

L_0a53:
    itok = vlpbrVCR->rgkill[i].itok;
    ptok = &(vrgtok[itok]);
    ptok->csh = (ptok->csh - vlpbrVCR->rgkill[i].cshKill);
    ptok->dv.dp = vlpbrVCR->rgkill[i].dv.dp;
    vrgdpVCR[itok] = LdpFromItokDv(itok, &(vlpbrVCR->rgkill[i].dv));
    if ((ptok->dpShield == 0x0))
        goto L_0a3f;
    else
        goto L_0b29;

L_0b29:
    if ((vlpbrVCR->rgkill[i].dpShield == 0x0))
        goto L_0a3f;
    else
        goto L_0b4d;

L_0b4d:
    /* untranslated: branch hiword((uint32_t)(words(0x0, ptok->dpShield) * words(0x0, ptok->csh))) < hiword((int32_t)(words((vlpbrVCR->rgkill[i].dpShield &
     * 0x1fff), 0x0) << ((vlpbrVCR->rgkill[i].dpShield >> 0xd) << 0x1))) ? L_0c48 : L_0bcf */

L_0bcf:
    /* untranslated: branch ss:[bp-0xc] > hiword((int32_t)(words((vlpbrVCR->rgkill[i].dpShield & 0x1fff), 0x0) << ((vlpbrVCR->rgkill[i].dpShield >> 0xd) <<
     * 0x1))) ? L_0bdb : L_0bd4 */

L_0bd4:
    /* untranslated: branch ss:[bp-0xe] <= loword((int32_t)(words((vlpbrVCR->rgkill[i].dpShield & 0x1fff), 0x0) << ((vlpbrVCR->rgkill[i].dpShield >> 0xd) <<
     * 0x1))) ? L_0c48 : L_0bdb */

L_0bdb:
    /* untranslated: ptok->dpShield = (ptok->dpShield - loword((int32_t)((int32_t)(words((vlpbrVCR->rgkill[i].dpShield & 0x1fff), 0x0) <<
     * ((vlpbrVCR->rgkill[i].dpShield >> 0xd) << 0x1)) / words(0x0, ptok->csh)))) */
    goto L_0a3f;

L_0c48:
    ptok->dpShield = 0x0;

L_0c54:
    vlpbrVCR = &(vlpbrVCR->rgkill[vlpbrVCR->ctok]);
    if ((vlpbrVCR->iRound <= viRound))
        goto L_0d15;
    else
        goto L_0c8f;

L_0c8f:
    viRound = vlpbrVCR->iRound;
    ptok = vrgtok;
    i = 0x0;
    goto L_0cc5;

L_0cb2:
    i = (i + 0x1);
    ptok = (ptok + 0x1);

L_0cc5:
    if ((i >= vlpbdVCR->ctok))
        goto L_0d15;
    else
        goto L_0cd8;

L_0cd8:
    if ((ptok->fRegen == 0x0))
        goto L_0cfe;
    else
        goto L_0cf0;

L_0cf0:
    RegenShield(ptok);

L_0cfe:
    ptok->fMoved = 0x0;
    goto L_0cb2;

L_0d15:
    if ((LOWORD(vlpbrVCR) >= LOWORD(vlpbdVCRNext)))
        goto L_0a0c;
    else
        goto L_0d25;

L_0d25:
    /* untranslated: vrgtok[vlpbrVCR->itok].brc = lobyte(setlobyte(LOWORD(vlpbdVCRNext), vlpbrVCR->brcDest)) */
    vbrcVCRFocus = vrgtok[vlpbrVCR->itok].brc;
    viVCRFocus = vlpbrVCR->itok;
    vrgtok[vlpbrVCR->itok].wFlags = ((vrgtok[vlpbrVCR->itok].wFlags & 0xfc1f) | ((vlpbrVCR->dzDis & 0x1f) * 0x20));
    if ((((vrgtok[vlpbrVCR->itok].wFlags >> 0x5) & 0x1f) != 0x4))
        goto L_0a0c;
    else
        goto L_0e30;

L_0e30:
    /* untranslated: part[23:2](vrgtok[vlpbrVCR->itok]) = ((part[23:2](vrgtok[vlpbrVCR->itok]) & 0xf0ff) | 0x0) */

L_0e7f:
    EnableVCRButtons();
    return viStepVCRCur;
}

int16_t VCRDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    HDC         hdc;
    int16_t     i;
    int16_t     ibtn;
    RECT        rc;
    int16_t     dyFrame;
    RECT        rcWindow;
    int16_t     dx;
    POINT       pt;
    uint8_t     brc;
    int16_t     bkMode;
    int16_t     bt;
    RECT       *prc;
    int16_t     iDir;
    int16_t     iCur;
    COLORREF    crBkSav;
    BTNT        btnt;
    int16_t     iSel;
    PAINTSTRUCT ps;
    int16_t     iStep;
    int16_t     dStep;
    uint16_t    t_merge_12fd_0001;
    uint16_t    t_merge_1480_0001;
    uint16_t    t_merge_176a_0001;
    uint16_t    t_merge_17ca_0001;

L_0e90:
    goto L_18e5;

L_0e9f:
    hwndVCRDlg = hwnd;
    GetWindowRect(hwnd, &(rcWindow));
    GetClientRect(hwnd, &(rc));
    dyFrame = ((rcWindow.bottom - rcWindow.top) - rc.bottom);
    GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x00A1), &(rc));
    SetWindowPos(hwnd, 0x0, 0x0, 0x0, (dxyVCRBoard + 0xfa), (((dyFrame + 0x18) + dxyVCRBoard) + (rc.bottom - rc.top)), 0x6);
    i = 0x0;
    goto L_1021;

L_0f22:
    if ((i >= 0x5))
        goto L_0f37;
    else
        goto L_0f2b;

L_0f2b:
    ibtn = (i + 0xa1);
    goto L_0f56;

L_0f37:
    if ((i != 0x5))
        goto L_0f48;
    else
        goto L_0f40;

L_0f40:
    ibtn = 0x1;
    goto L_0f56;

L_0f48:
    if ((i != 0x6))
        goto L_0f56;
    else
        goto L_0f51;

L_0f51:
    ibtn = 0x76;

L_0f56:
    GetWindowRect(GetDlgItem(hwnd, ibtn), &(rc));
    MapWindowPoints(0x0, hwnd, &(rc), 0x2);
    if ((dxyVCRSquare < 0x40))
        goto L_0fd8;
    else
        goto L_0f8f;

L_0f8f:
    dx = (rc.right - rc.left);
    /* untranslated: dx = (((((sext16to32(dxyVCRBoard) / 0x2) + 0x8) - (words(loword((0x7 * dx)), signhiword(loword((0x7 * dx)))) / 0x2)) + 0xffe8) +
     * loword(((dx + 0x8) * i))) */
    dx = (dx - rc.left);
    goto L_0fdd;

L_0fd8:
    dx = 0x0;

L_0fdd:
    OffsetRect(&(rc), dx, ((dxyVCRBoard + 0x10) - rc.top));
    SetWindowPos(GetDlgItem(hwnd, ibtn), 0x0, rc.left, rc.top, 0x0, 0x0, 0x5);
    i = (i + 0x1);

L_1021:
    if ((i < 0x7))
        goto L_0f22;
    else
        goto L_102a;

L_102a:
    EnableVCRButtons();
    StickyDlgPos(hwnd, &(ptStickyVCRDlg), 0x1);
    fAnimate = 0x1;
    return 0x1;

L_104e:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_1076:
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    if ((pt.x <= 0x8))
        goto L_10de;
    else
        goto L_109a;

L_109a:
    if ((pt.x >= (LOWORD(((dxyVCRSquare + 0x3) * 0xa)) + 0x8)))
        goto L_10de;
    else
        goto L_10b0;

L_10b0:
    if ((pt.y < 0x8))
        goto L_10de;
    else
        goto L_10b9;

L_10b9:
    if ((pt.y >= (LOWORD(((dxyVCRSquare + 0x3) * 0xa)) + 0x8)))
        goto L_10de;
    else
        goto L_10cf;

L_10cf:
    SetCursor(hcurHand);
    return 0x1;

L_10de:
    return 0x0;

L_10e4:
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    /* untranslated: branch PtInRect(rgrcBuildSpin, words(pt.y, pt.x)) != 0x0 ? L_1138 : L_111e */

L_111e:
    /* untranslated: branch PtInRect(&rgrcBuildSpin[0x1], words(pt.y, pt.x)) == 0x0 ? L_1321 : L_1138 */

L_1138:
    /* untranslated: branch PtInRect(rgrcBuildSpin, words(pt.y, pt.x)) == 0x0 ? L_1164 : L_1152 */

L_1152:
    iDir = 0xffff;
    bt = 0x22;
    prc = 0x5926;
    goto L_1173;

L_1164:
    iDir = 0x1;
    bt = 0x23;
    prc = 0x592e;

L_1173:
    iCur = viSpeedVCR;
    if ((iCur <= 0x4))
        goto L_118a;
    else
        goto L_1182;

L_1182:
    iCur = 0x4;
    goto L_1198;

L_118a:
    if ((iCur >= 0x0))
        goto L_1198;
    else
        goto L_1193;

L_1193:
    iCur = 0x0;

L_1198:
    hdc = GetDC(hwnd);
    bkMode = SetBkMode(hdc, OPAQUE);
    crBkSav = SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[0x1]);
    InitBtnTrack(&(btnt), hwnd, 0x0, prc, bt, 0x50, 0x0, 0x0, 0x0);

L_11fd:
    if ((FTrackBtn(&(btnt)) == 0x0))
        goto L_1281;
    else
        goto L_1211;

L_1211:
    if ((iDir != 0xffff))
        goto L_1223;
    else
        goto L_121a;

L_121a:
    if ((iCur > 0x0))
        goto L_1235;
    else
        goto L_1223;

L_1223:
    if ((iDir != 0x1))
        goto L_11fd;
    else
        goto L_122c;

L_122c:
    if ((iCur >= 0x4))
        goto L_11fd;
    else
        goto L_1235;

L_1235:
    iCur = (iCur + iDir);
    bt = _wsprintf(szWork, PszGetCompressedString(idsPlaybackSpeedD), (iCur + 0x1));
    TextOut(hdc, ptSpeedVCR.x, ptSpeedVCR.y, szWork, bt);

L_1281:
    viSpeedVCR = iCur;
    SelectObject(hdc, rghfontArial8[0x0]);
    SetBkColor(hdc, crBkSav);
    ReleaseDC(hwnd, hdc);
    if ((gd.fVCRTimer == 0x0))
        goto L_131b;
    else
        goto L_12bf;

L_12bf:
    KillTimer(hwnd, 0xa6c);
    if ((SetTimer(hwnd, 0xa6c, (0x23a - LOWORD((0x78 * viSpeedVCR))), 0x0) == 0x0))
        goto L_12fa;
    else
        goto L_12f4;

L_12f4:
    t_merge_12fd_0001 = 0x1;
    goto L_12fd;

L_12fa:
    t_merge_12fd_0001 = 0x0;

L_12fd:
    gd.fVCRTimer = t_merge_12fd_0001;

L_131b:
    return 0x1;

L_1321:
    pt.x = ((uint32_t)((pt.x + 0xfff8)) / (dxyVCRSquare + 0x3));
    pt.y = ((uint32_t)((pt.y + 0xfff8)) / (dxyVCRSquare + 0x3));
    if ((pt.x < 0xa))
        goto L_14ad;
    else
        goto L_1350;

L_1350:
    if ((pt.y < 0x2))
        goto L_14ad;
    else
        goto L_1359;

L_1359:
    if ((pt.y >= 0xa))
        goto L_14ad;
    else
        goto L_1362;

L_1362:
    if ((viVCRFocus < 0x0))
        goto L_14ad;
    else
        goto L_136c;

L_136c:
    GlobalPD.grPopup = grPopupShdef;
    if ((vrgtok[viVCRFocus].grobj != grobjPlanet))
        goto L_13f0;
    else
        goto L_1394;

L_1394:
    HIWORD(GlobalPD) = (LOWORD(rglpshdefSB[vrgtok[viVCRFocus].iplr]) + LOWORD(((vrgtok[viVCRFocus].ishdef + 0xfff0) * 0x93)));
    /* untranslated: part[4:2](GlobalPD) = HIWORD(rglpshdefSB[vrgtok[viVCRFocus].iplr]) */
    goto L_1446;

L_13f0:
    HIWORD(GlobalPD) = (LOWORD(rglpshdef[vrgtok[viVCRFocus].iplr]) + LOWORD((vrgtok[viVCRFocus].ishdef * 0x93)));
    /* untranslated: part[4:2](GlobalPD) = HIWORD(rglpshdef[vrgtok[viVCRFocus].iplr]) */

L_1446:
    /* untranslated: part[6:2](GlobalPD) = 0x1 */
    /* untranslated: part[10:2](GlobalPD) = 0x1 */
    if ((vrgtok[viVCRFocus].iplr == idPlayer))
        goto L_147d;
    else
        goto L_1477;

L_1477:
    t_merge_1480_0001 = 0x1;
    goto L_1480;

L_147d:
    t_merge_1480_0001 = 0x0;

L_1480:
    /* untranslated: part[8:2](GlobalPD) = t_merge_1480_0001 */
    Popup(hwnd, LOWORD(lParam), (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff));
    goto L_1930;

L_14ad:
    if ((pt.x < 0x0))
        goto L_1930;
    else
        goto L_14b6;

L_14b6:
    if ((pt.y < 0x0))
        goto L_1930;
    else
        goto L_14bf;

L_14bf:
    if ((pt.x >= 0xa))
        goto L_1930;
    else
        goto L_14c8;

L_14c8:
    if ((pt.y >= 0xa))
        goto L_1930;
    else
        goto L_14ce;

L_14ce:

L_14d4:
    brc = LOBYTE((((pt.y & 0xf) << 0x4) | (pt.x & 0xf)));
    if ((message != WM_RBUTTONDOWN))
        goto L_1543;
    else
        goto L_14f7;

L_14f7:
    iSel = PopupVCRMenu(hwnd, LOWORD(lParam), (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), brc);
    if ((iSel < 0x0))
        goto L_1930;
    else
        goto L_1531;

L_1531:
    vbrcVCRFocus = brc;
    viVCRFocus = iSel;
    goto GoodSel;

L_1543:
    if ((brc != (vbrcVCRFocus & 0xff)))
        goto L_1566;
    else
        goto L_155c;

L_155c:
    if ((viVCRFocus != 0xffff))
        goto L_157d;
    else
        goto L_1566;

L_1566:
    vbrcVCRFocus = brc;
    viVCRFocus = (vlpbdVCR->ctok + 0xffff);

L_157d:
    i = viVCRFocus;

L_1583:
    i = (i + 0x1);
    if ((i != vlpbdVCR->ctok))
        goto L_159f;
    else
        goto L_159a;

L_159a:
    i = 0x0;

L_159f:
    if ((vrgtok[i].brc != brc))
        goto L_15f0;
    else
        goto L_15cb;

L_15cb:
    if ((vrgtok[i].csh <= 0x0))
        goto L_15f0;
    else
        goto L_15e7;

L_15e7:
    viVCRFocus = i;
    goto GoodSel;

L_15f0:
    if ((i != viVCRFocus))
        goto L_1583;
    else
        goto L_15fb;

L_15fb:
    viVCRFocus = 0xffff;

GoodSel:
    DrawVCR(0x0, 0xfffe, 0xffff);
    goto L_1930;

L_1618:
    hdc = BeginPaint(hwnd, &(ps));
    DrawVCR(hdc, 0xffff, 0xffff);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_1652:
    if ((gd.fVCRTimer == 0x0))
        goto L_1930;
    else
        goto L_1665;

L_1665:
    if ((viStepVCRCur != vcStepVCR))
        goto NextBtn;
    else
        goto L_1671;

L_1671:
    i = 0x2;
    goto KillTime;

L_167f:
    if ((wParam < 0xa1))
        goto L_1829;
    else
        goto L_1689;

L_1689:
    if ((wParam > 0xa5))
        goto L_1829;
    else
        goto L_1693;

L_1693:
    i = (wParam + 0xff5f);
    if ((gd.fVCRTimer == 0x0))
        goto L_16d3;
    else
        goto KillTime;

KillTime:
    KillTimer(hwnd, 0xa6c);
    gd.fVCRTimer = 0x0;
    if ((i == 0x2))
        goto L_1930;
    else
        goto L_16cd;

L_16cd:

L_16d3:
    if ((GetAsyncKeyState(0x11) >= 0x0))
        goto L_16ec;
    else
        goto L_16e4;

L_16e4:
    dStep = 0x64;
    goto L_170a;

L_16ec:
    if ((GetAsyncKeyState(0x10) >= 0x0))
        goto L_1705;
    else
        goto L_16fd;

L_16fd:
    dStep = 0xa;
    goto L_170a;

L_1705:
    dStep = 0x1;

L_170a:
    fAnimate = 0x0;
    goto L_17dc;

L_1716:
    iStep = 0xffff;
    goto L_1807;

L_171e:
    iStep = (viStepVCRCur - dStep);
    if ((iStep >= 0xffff))
        goto L_1807;
    else
        goto L_1730;

L_1730:
    iStep = 0xffff;

L_1738:
    if ((SetTimer(hwnd, 0xa6c, (0x23a - LOWORD((0x78 * viSpeedVCR))), 0x0) == 0x0))
        goto L_1767;
    else
        goto L_1761;

L_1761:
    t_merge_176a_0001 = 0x1;
    goto L_176a;

L_1767:
    t_merge_176a_0001 = 0x0;

L_176a:
    gd.fVCRTimer = t_merge_176a_0001;

NextBtn:
    if ((i != 0x3))
        goto L_179d;
    else
        goto L_1791;

L_1791:
    iStep = (viStepVCRCur + dStep);
    goto L_17a6;

L_179d:
    iStep = (viStepVCRCur + 0x1);

L_17a6:
    if ((iStep <= vcStepVCR))
        goto L_17b7;
    else
        goto L_17b1;

L_17b1:
    iStep = vcStepVCR;

L_17b7:
    if ((viSpeedVCR >= 0x4))
        goto L_17c7;
    else
        goto L_17c1;

L_17c1:
    t_merge_17ca_0001 = 0x1;
    goto L_17ca;

L_17c7:
    t_merge_17ca_0001 = 0x0;

L_17ca:
    fAnimate = t_merge_17ca_0001;
    goto L_1807;

L_17d0:
    iStep = vcStepVCR;
    goto L_1807;

L_17dc:
    if ((i == 0x0))
        goto L_1716;
    else
        goto L_17e4;

L_17e4:
    if ((i == 0x1))
        goto L_171e;
    else
        goto L_17ec;

L_17ec:
    if ((i == 0x2))
        goto L_1738;
    else
        goto L_17f4;

L_17f4:
    if ((i == 0x3))
        goto NextBtn;
    else
        goto L_17fc;

L_17fc:
    if ((i == 0x4))
        goto L_17d0;
    else
        goto L_1807;

L_1807:
    SetVCRBoard(iStep);
    DrawVCR(0x0, 0xfffe, 0xffff);
    goto L_1930;

L_1829:
    if ((wParam == 0x1))
        goto L_183b;
    else
        goto L_1832;

L_1832:
    if ((wParam != 0x2))
        goto L_18ac;
    else
        goto L_183b;

L_183b:
    if ((gd.fVCRTimer == 0x0))
        goto L_1866;
    else
        goto L_184e;

L_184e:
    gd.fVCRTimer = 0x0;
    KillTimer(hwnd, 0xa6c);

L_1866:
    if ((gd.fTutorial == 0x0))
        goto L_1885;
    else
        goto L_1879;

L_1879:
    tutor.fProgress = 0x1;

L_1885:
    StickyDlgPos(hwnd, &(ptStickyVCRDlg), 0x0);
    EndDialog(hwnd, i);
    return 0x1;

L_18ac:
    if ((wParam != 0x76))
        goto L_1930;
    else
        goto L_18b5;

L_18b5:
    WinHelp(hwnd, szHelpFile, 0x1, 0x43a);
    return 0x1;

L_18d9:
    hwndVCRDlg = 0x0;
    goto L_1930;

L_18e5:
    if ((message == WM_DESTROY))
        goto L_18d9;
    else
        goto L_18ed;

L_18ed:
    if ((message == WM_PAINT))
        goto L_1618;
    else
        goto L_18f5;

L_18f5:
    if ((message == WM_ERASEBKGND))
        goto L_104e;
    else
        goto L_18fd;

L_18fd:
    if ((message == WM_SETCURSOR))
        goto L_1076;
    else
        goto L_1905;

L_1905:
    if ((message == WM_INITDIALOG))
        goto L_0e9f;
    else
        goto L_190d;

L_190d:
    if ((message == WM_COMMAND))
        goto L_167f;
    else
        goto L_1915;

L_1915:
    if ((message == WM_TIMER))
        goto L_1652;
    else
        goto L_191d;

L_191d:
    if ((message == WM_LBUTTONDOWN))
        goto L_10e4;
    else
        goto L_1925;

L_1925:
    if ((message == WM_RBUTTONDOWN))
        goto L_10e4;
    else
        goto L_1930;

L_1930:
    return 0x0;
}

void GetVCRStats(int16_t itok, int32_t *pdpArmor, DV *pdv, int32_t *pdpShields, int16_t *pcsh) {
    int16_t  cshT;
    DV       dv;
    int32_t  dpShields;
    int32_t  dpArmor;
    int16_t  i;
    int16_t  cshKill;
    uint16_t dpShdef;

L_193e:
    dpArmor = 0x0;
    dpShields = 0x0;
    cshKill = 0x0;
    dv.dp = 0xffff;
    dpArmor = vrgdpVCR[itok];
    i = 0x0;
    goto L_1991;

L_198d:
    i = (i + 0x1);

L_1991:
    if ((i >= vlpbrVCR->ctok))
        goto L_1a64;
    else
        goto L_19a1;

L_19a1:
    if ((vlpbrVCR->rgkill[i].itok != itok))
        goto L_198d;
    else
        goto L_19c9;

L_19c9:
    cshKill = (cshKill + vlpbrVCR->rgkill[i].cshKill);
    /* untranslated: dpShields = (dpShields + (int32_t)(words((vlpbrVCR->rgkill[i].dpShield & 0x1fff), 0x0) << ((vlpbrVCR->rgkill[i].dpShield >> 0xd) << 0x1)))
     */
    dv.dp = vlpbrVCR->rgkill[i].dv.dp;

L_1a64:
    if ((dv.dp == 0xffff))
        goto L_1b56;
    else
        goto L_1a6d;

L_1a6d:
    dpShdef = LpshdefFromTok(&(vrgtok[itok]))->hul.dp;
    cshT = (vrgtok[itok].csh - cshKill);
    /* untranslated: dpArmor = (uint32_t)(words(0x0, dpShdef) * sext16to32(cshT)) */
    /* untranslated: cshT = loword((int32_t)((uint32_t)(sext16to32(cshT) * words(0x0, dv.pctSh)) / 0x64)) */
    if ((cshT > 0x0))
        goto L_1b05;
    else
        goto L_1b00;

L_1b00:
    cshT = 0x1;

L_1b05:
    /* untranslated: dpArmor = (dpArmor - (int32_t)((uint32_t)((int32_t)((uint32_t)(words(0x0, dpShdef) * words(0x0, dv.pctDp)) / 0xa) * sext16to32(cshT)) /
     * 0x32)) */
    goto L_1b8e;

L_1b56:
    dv.dp = vrgtok[itok].dv.dp;
    if ((dv.pctDp <= 0x1f3))
        goto L_1b8e;
    else
        goto L_1b82;

L_1b82:
    dv.pctDp = 0x1f3;

L_1b8e:
    cshT = (vrgtok[itok].csh - cshKill);
    if ((cshT >= 0x1))
        goto L_1c0b;
    else
        goto L_1bbb;

L_1bbb:
    cshT = 0x0;
    dpArmor = 0x0;
    /* untranslated: dpShields = (uint32_t)(words(0x0, vrgtok[itok].dpShield) * words(0x0, vrgtok[itok].csh)) */

L_1c0b:
    if ((pdv == 0x0))
        goto L_1c1c;
    else
        goto L_1c14;

L_1c14:
    pdv->dp = dv.dp;

L_1c1c:
    if ((pcsh == 0x0))
        goto L_1c2d;
    else
        goto L_1c25;

L_1c25:
    *(pcsh) = cshT;

L_1c2d:
    if ((pdpArmor == 0x0))
        goto L_1c44;
    else
        goto L_1c36;

L_1c36:
    pdpArmor = LOWORD(dpArmor);
    *(pdpArmor + 0x2) = HIWORD(dpArmor);

L_1c44:
    if ((pdpShields == 0x0))
        goto L_1c5b;
    else
        goto L_1c4d;

L_1c4d:
    pdpShields = LOWORD(dpShields);
    *(pdpShields + 0x2) = HIWORD(dpShields);

L_1c5b:
    return;
}

void DrawVCR(HDC hdc, int16_t iStart, int16_t iEnd) {
    int16_t  ctok;
    int16_t  ibmpRace;
    int16_t  bkMode;
    HBRUSH   hbrSav;
    int32_t  dpShields;
    int16_t  itokT;
    int32_t  dpT;
    int16_t  fCreatedDC;
    int32_t  dpArmor;
    int16_t  y;
    uint8_t  rgfSeen[256];
    int16_t  c;
    int16_t  i;
    uint8_t  brcT;
    SHDEF   *lpshdef;
    int16_t  ibmp;
    int16_t  csh;
    char    *psz;
    int16_t  dx;
    int16_t  j;
    char     szT[96];
    int16_t  fJam;
    RECT     rc;
    int16_t  x;
    int16_t  cshT;
    int32_t  dpShT;
    DV       dv;
    int16_t  xT;
    int16_t  cshNew;
    uint16_t t_merge_1c7d_0001;
    uint16_t t_merge_1fe0_0001;
    char     t_merge_2766_0001[96];
    uint16_t t_merge_2766_0002;
    uint16_t t_merge_27d6_0001;
    uint16_t t_merge_2965_0001;
    uint8_t  t_merge_2d54_0001;
    uint16_t t_merge_37ca_0001;

L_1c62:
    if ((hdc != 0x0))
        goto L_1c7a;
    else
        goto L_1c74;

L_1c74:
    t_merge_1c7d_0001 = 0x1;
    goto L_1c7d;

L_1c7a:
    t_merge_1c7d_0001 = 0x0;

L_1c7d:
    fCreatedDC = t_merge_1c7d_0001;
    if ((fCreatedDC == 0x0))
        goto L_1c95;
    else
        goto L_1c89;

L_1c89:
    hdc = GetDC(hwndVCRDlg);

L_1c95:
    hbrSav = SelectObject(hdc, hbrButtonFace);
    bkMode = SetBkMode(hdc, TRANSPARENT);
    memset(rgfSeen, 0x0, 0x100);
    GetClientRect(hwndVCRDlg, &(rc));
    if ((iStart != 0xfffe))
        goto L_1d15;
    else
        goto L_1ce2;

L_1ce2:
    PatBlt(hdc, (dxyVCRBoard + 0xa), 0x0, ((rc.right - dxyVCRBoard) + 0xfff6), (dxyVCRBoard + 0x8), PATCOPY);
    iStart = 0xffff;

L_1d15:
    SelectObject(hdc, hbrButtonShadow);
    if ((iStart != 0xffff))
        goto L_33a4;
    else
        goto L_1d2a;

L_1d2a:
    PatBlt(hdc, 0x8, 0x8, dxyVCRBoard, 0x2, PATCOPY);
    PatBlt(hdc, 0x8, 0x8, 0x2, dxyVCRBoard, PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, 0x9, (dxyVCRBoard + 0x6), (dxyVCRBoard + 0xffff), 0x1, PATCOPY);
    PatBlt(hdc, 0x8, (dxyVCRBoard + 0x7), dxyVCRBoard, 0x1, PATCOPY);
    PatBlt(hdc, (dxyVCRBoard + 0x6), 0x9, 0x1, (dxyVCRBoard + 0xfffd), PATCOPY);
    PatBlt(hdc, (dxyVCRBoard + 0x7), 0x8, 0x1, (dxyVCRBoard + 0xfffe), PATCOPY);
    SelectObject(hdc, hbrButtonShadow);
    i = 0x1;
    goto L_1e7f;

L_1e20:
    PatBlt(hdc, (LOWORD(((dxyVCRSquare + 0x3) * i)) + 0x9), 0xa, 0x1, (dxyVCRBoard + 0xfffc), PATCOPY);
    PatBlt(hdc, 0xa, (LOWORD(((dxyVCRSquare + 0x3) * i)) + 0x9), (dxyVCRBoard + 0xfffc), 0x1, PATCOPY);
    i = (i + 0x1);

L_1e7f:
    if ((i < 0xa))
        goto L_1e20;
    else
        goto L_1e89;

L_1e89:
    x = (dxyVCRBoard + 0xe);
    y = 0x8;
    SelectObject(hdc, rghfontArial8[0x1]);
    c = _wsprintf(szWork, PszGetCompressedString(idsPhaseDDRoundDD), (viStepVCRCur + 0x2), (vcStepVCR + 0x2), (viRound + 0x1), (vcRound + 0x1));
    TextOut(hdc, x, y, szWork, c);
    y = (y + (dyArial8 + 0x4));
    c = _wsprintf(szWork, PszGetCompressedString(idsPlaybackSpeedD), (viSpeedVCR + 0x1));
    dx = LOWORD(GetTextExtent(hdc, szWork, c));
    TextOut(hdc, x, y, szWork, c);
    ptSpeedVCR.x = x;
    ptSpeedVCR.y = y;
    SetRect(rgrcBuildSpin, (x + dx), y, ((x + dx) + 0xe), (y + 0xe));
    rgrcBuildSpin[0x1] = rgrcBuildSpin[0x0];
    OffsetRect(&(rgrcBuildSpin[0x1]), 0xe, 0x0);
    i = 0x0;
    goto L_2004;

L_1fc5:
    if ((i != 0x0))
        goto L_1fdd;
    else
        goto L_1fd7;

L_1fd7:
    t_merge_1fe0_0001 = 0x2;
    goto L_1fe0;

L_1fdd:
    t_merge_1fe0_0001 = 0x3;

L_1fe0:
    DrawBtn(hdc, rgrcBuildSpin[i], (t_merge_1fe0_0001 | 0x20), 0x0, 0x0);
    i = (i + 0x1);

L_2004:
    if ((i < 0x2))
        goto L_1fc5;
    else
        goto L_200e;

L_200e:
    if ((viStepVCRCur < 0x0))
        goto L_29a2;
    else
        goto L_2018;

L_2018:
    y = (y + (dyArial8 + 0x4));
    psz = PszPlayerName(vrgtok[vlpbrVCR->itok].iplr, 0x1, 0x1, 0x1, 0x0, 0x0);
    TextOut(hdc, x, y, szWork, strlen(psz));
    y = (y + dyArial8);
    if ((vlpbrVCR->itok != viVCRFocus))
        goto L_20b0;
    else
        goto L_20a0;

L_20a0:
    SetTextColor(hdc, 0x7f0000);

L_20b0:
    if ((vrgtok[vlpbrVCR->itok].grobj != grobjPlanet))
        goto L_214b;
    else
        goto L_20da;

L_20da:
    lpshdef = &(rglpshdefSB[vrgtok[vlpbrVCR->itok].iplr][(vrgtok[vlpbrVCR->itok].ishdef - 0x10)]);
    goto L_21b6;

L_214b:
    lpshdef = &(rglpshdef[vrgtok[vlpbrVCR->itok].iplr][vrgtok[vlpbrVCR->itok].ishdef]);

L_21b6:
    csh = vrgtok[vlpbrVCR->itok].csh;
    if ((csh <= 0x1))
        goto L_221c;
    else
        goto L_21e3;

L_21e3:
    c = _wsprintf(szWork, PszGetCompressedString(idsSD), (LOWORD(lpshdef) + 0x8), HIWORD(lpshdef), csh);
    goto L_224a;

L_221c:
    fstrcpy(szWork, &(lpshdef->hul.szClass));
    c = strlen(szWork);

L_224a:
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);
    SetTextColor(hdc, crButtonText);
    fJam = 0x0;
    if ((vlpbrVCR->ctok <= 0x0))
        goto L_291d;
    else
        goto L_228e;

L_228e:
    psz = PszPlayerName(vrgtok[vlpbrVCR->itokAttack].iplr, 0x0, 0x1, 0x1, 0x0, 0x0);
    c = _wsprintf(szT, PszGetCompressedString(idsAttacksS), psz);
    TextOut(hdc, x, y, szT, c);
    y = (y + dyArial8);
    if ((vlpbrVCR->rgkill[0x0].dv.dp == 0x0))
        goto L_2342;
    else
        goto L_2332;

L_2332:
    SetTextColor(hdc, 0x7f);

L_2342:
    if ((vrgtok[vlpbrVCR->itokAttack].grobj != grobjPlanet))
        goto L_23f5;
    else
        goto L_2372;

L_2372:
    lpshdef = &(rglpshdefSB[vrgtok[((*(vlpbrVCR + 0x4) >> 0x8) & 0xff)].iplr][(vrgtok[vlpbrVCR->itokAttack].ishdef - 0x10)]);
    goto L_2472;

L_23f5:
    lpshdef = &(rglpshdef[vrgtok[((*(vlpbrVCR + 0x4) >> 0x8) & 0xff)].iplr][vrgtok[vlpbrVCR->itokAttack].ishdef]);

L_2472:
    csh = vrgtok[vlpbrVCR->itokAttack].csh;
    if ((csh <= 0x1))
        goto L_24de;
    else
        goto L_24a5;

L_24a5:
    c = _wsprintf(szWork, PszGetCompressedString(idsSD), (LOWORD(lpshdef) + 0x8), HIWORD(lpshdef), csh);
    goto L_250c;

L_24de:
    fstrcpy(szWork, &(lpshdef->hul.szClass));
    c = strlen(szWork);

L_250c:
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);
    SetTextColor(hdc, crButtonText);
    brcT = vrgtok[vlpbrVCR->itokAttack].brc;
    dpArmor = 0x0;
    dpShields = 0x0;
    j = 0x0;
    i = vlpbrVCR->ctok;
    goto L_26a4;

L_258e:
    itokT = vlpbrVCR->rgkill[(i - 0x1)].itok;
    if ((itokT != vlpbrVCR->itokAttack))
        goto L_2603;
    else
        goto L_25cd;

L_25cd:
    if ((fJam != 0x0))
        goto L_2603;
    else
        goto L_25d7;

L_25d7:
    fJam = (fJam | (vlpbrVCR->rgkill[(i - 0x1)].grfWeapon & 0xc0));

L_2603:
    j = (j + vlpbrVCR->rgkill[(i - 0x1)].cshKill);
    if ((rgfSeen[itokT] != 0x0))
        goto L_269f;
    else
        goto L_263c;

L_263c:

L_2642:
    rgfSeen[itokT] = 0x1;
    GetVCRStats(itokT, &(dpT), 0x0, &(dpShT), &(cshT));
    dpArmor = (dpArmor + (vrgdpVCR[itokT] - dpT));
    dpShields = (dpShields + dpShT);

L_269f:
    i = (i - 0x1);

L_26a4:
    if ((i > 0x0))
        goto L_258e;
    else
        goto L_26ae;

L_26ae:
    c = _wsprintf(szWork, PszGetCompressedString(idsDDDoing), (brcT & 0xf), (brcT >> 0x4));
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);
    if ((LOWORD(dpShields) != 0x0))
        goto L_271e;
    else
        goto L_2715;

L_2715:
    if ((HIWORD(dpShields) == 0x0))
        goto L_27b1;
    else
        goto L_271e;

L_271e:
    CchGetString(idsAnd, szT);
    if ((HIWORD(dpArmor) < 0x0))
        goto L_274f;
    else
        goto L_2738;

L_2738:
    if ((HIWORD(dpArmor) > 0x0))
        goto L_2746;
    else
        goto L_273d;

L_273d:
    if ((LOWORD(dpArmor) <= 0x0))
        goto L_274f;
    else
        goto L_2746;

L_2746:
    t_merge_2766_0001 = szT;
    /* untranslated: t_merge_2766_0002 = ss */
    goto L_2766;

L_274f:
    if ((j <= 0x0))
        goto L_2761;
    else
        goto L_2759;

L_2759:
    t_merge_2766_0001 = 0x1420;
    /* untranslated: t_merge_2766_0002 = ds */
    goto L_2766;

L_2761:
    t_merge_2766_0001 = 0x1422;
    /* untranslated: t_merge_2766_0002 = ds */

L_2766:
    c = _wsprintf(szWork, PszGetCompressedString(idsLdDamageShieldsS), LOWORD(dpShields), HIWORD(dpShields), t_merge_2766_0001, t_merge_2766_0002);
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);

L_27b1:
    if ((LOWORD(dpArmor) != 0x0))
        goto L_27c3;
    else
        goto L_27ba;

L_27ba:
    if ((HIWORD(dpArmor) == 0x0))
        goto L_2820;
    else
        goto L_27c3;

L_27c3:
    if ((j <= 0x0))
        goto L_27d3;
    else
        goto L_27cd;

L_27cd:
    t_merge_27d6_0001 = 0x2c;
    goto L_27d6;

L_27d3:
    t_merge_27d6_0001 = 0x2e;

L_27d6:
    c = _wsprintf(szWork, PszGetCompressedString(idsLdDamageArmorC), LOWORD(dpArmor), HIWORD(dpArmor), t_merge_27d6_0001);
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);

L_2820:
    if (((fJam & 0x40) == 0x0))
        goto L_288d;
    else
        goto L_282f;

L_282f:
    if ((LOWORD(dpShields) != 0x0))
        goto L_288d;
    else
        goto L_2838;

L_2838:
    if ((HIWORD(dpShields) != 0x0))
        goto L_288d;
    else
        goto L_2841;

L_2841:
    if ((LOWORD(dpArmor) != 0x0))
        goto L_288d;
    else
        goto L_284a;

L_284a:
    if ((HIWORD(dpArmor) != 0x0))
        goto L_288d;
    else
        goto L_2853;

L_2853:
    psz = PszGetCompressedString(idsDamage3);
    TextOut(hdc, x, y, psz, strlen(psz));
    y = (y + dyArial8);

L_288d:
    if ((j <= 0x0))
        goto L_291d;
    else
        goto L_2897;

L_2897:
    c = _wsprintf(szWork, PszGetCompressedString(idsDestroyingDShip), j);
    if ((j != 0x1))
        goto L_28e4;
    else
        goto L_28c8;

L_28c8:
    strcpy(szWork[c], ".");
    c = (c + 0x1);
    goto L_28fd;

L_28e4:
    strcpy(szWork[c], "s.");
    c = (c + 0x2);

L_28fd:
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);

L_291d:
    if ((fJam == 0x0))
        goto L_29a2;
    else
        goto L_2927;

L_2927:
    SetTextColor(hdc, 0x7f);
    if (((fJam & 0x40) == 0x0))
        goto L_2962;
    else
        goto L_294a;

L_294a:
    if ((LOWORD(dpArmor) != 0x0))
        goto L_2962;
    else
        goto L_2953;

L_2953:
    if ((HIWORD(dpArmor) != 0x0))
        goto L_2962;
    else
        goto L_295c;

L_295c:
    t_merge_2965_0001 = 0x51c;
    goto L_2965;

L_2962:
    t_merge_2965_0001 = 0x4d0;

L_2965:
    c = CchGetString(t_merge_2965_0001, szWork);
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);
    SetTextColor(hdc, crButtonText);

L_29a2:
    if (((vbrcVCRFocus & 0xff) == 0xff))
        goto L_339a;
    else
        goto L_29b0;

L_29b0:
    y = 0xc8;
    c = _wsprintf(szWork, PszGetCompressedString(idsSelectionDD), ((vbrcVCRFocus & 0xff) & 0xf), ((vbrcVCRFocus & 0xff) >> 0x4));
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);
    if ((viVCRFocus < 0x0))
        goto L_339a;
    else
        goto L_2a1b;

L_2a1b:
    csh = 0x0;
    psz = PszPlayerName(vrgtok[viVCRFocus].iplr, 0x1, 0x1, 0x1, 0x0, 0x0);
    c = strlen(psz);
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);
    if ((vrgtok[viVCRFocus].grobj != grobjPlanet))
        goto L_2b0d;
    else
        goto L_2aae;

L_2aae:
    lpshdef = &(rglpshdefSB[vrgtok[viVCRFocus].iplr][(vrgtok[viVCRFocus].ishdef - 0x10)]);
    goto L_2b66;

L_2b0d:
    lpshdef = &(rglpshdef[vrgtok[viVCRFocus].iplr][vrgtok[viVCRFocus].ishdef]);

L_2b66:
    csh = vrgtok[viVCRFocus].csh;
    GetVCRStats(viVCRFocus, &(dpT), dv, &(dpShields), &(cshT));
    cshNew = cshT;
    cshT = (csh - cshT);
    if ((csh > 0x1))
        goto L_2bc8;
    else
        goto L_2bbe;

L_2bbe:
    if ((cshT == 0x0))
        goto L_2c01;
    else
        goto L_2bc8;

L_2bc8:
    c = _wsprintf(szWork, PszGetCompressedString(idsSD), (LOWORD(lpshdef) + 0x8), HIWORD(lpshdef), csh);
    goto L_2c2f;

L_2c01:
    fstrcpy(szWork, &(lpshdef->hul.szClass));
    c = strlen(szWork);

L_2c2f:
    if ((cshT == 0x0))
        goto L_2c5b;
    else
        goto L_2c39;

L_2c39:
    c = (c + _wsprintf(&(szWork[c]), " (-%d)", cshT));

L_2c5b:
    SetTextColor(hdc, 0x7f0000);
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);
    xT = (((uint32_t)((rc.right - (dxyVCRBoard + 0xe))) / 0x2) + x);
    SetTextColor(hdc, crButtonText);
    if ((cshNew <= 0x0))
        goto L_3317;
    else
        goto L_2cc1;

L_2cc1:
    if ((vrgtok[viVCRFocus].grobj != grobjPlanet))
        goto L_2cec;
    else
        goto L_2ce3;

L_2ce3:
    i = 0x0;
    goto L_2d12;

L_2cec:
    /* untranslated: i = (((part[25:2](vrgtok[viVCRFocus]) >> 0x8) & 0xf) + 0x1) */

L_2d12:
    if ((vrgtok[viVCRFocus].initMin >= 0xff))
        goto L_2d51;
    else
        goto L_2d34;

L_2d34:
    t_merge_2d54_0001 = vrgtok[viVCRFocus].initMin;
    goto L_2d54;

L_2d51:
    t_merge_2d54_0001 = 0x0;

L_2d54:
    c = _wsprintf(szWork, PszGetCompressedString(idsInitiativeD), t_merge_2d54_0001);
    TextOut(hdc, x, y, szWork, c);
    c = _wsprintf(szWork, PszGetCompressedString(idsMovementS), &(rgszSpeed[(0x3 * i)]));
    TextOut(hdc, xT, y, szWork, c);
    y = (y + dyArial8);
    c = _wsprintf(szWork, PszGetCompressedString(idsArmorLd), LOWORD(dpT), HIWORD(dpT));
    TextOut(hdc, x, y, szWork, c);
    if ((dv.dp != 0x0))
        goto L_2e4b;
    else
        goto L_2e34;

L_2e34:
    c = CchGetString(idsDamageNone, szWork);
    goto L_2f60;

L_2e4b:
    csh = (csh - cshT);
    /* untranslated: csh = loword((int32_t)((uint32_t)(words(0x0, dv.pctSh) * sext16to32(csh)) / 0x64)) */
    if ((csh > 0x0))
        goto L_2e8e;
    else
        goto L_2e88;

L_2e88:
    csh = 0x1;

L_2e8e:
    /* untranslated: LOWORD(dpT) = (words(dv.pctDp, 0x0) / 0x5) */
    HIWORD(dpT) = 0x0;
    if ((LOWORD(dpT) != 0x0))
        goto L_2ecc;
    else
        goto L_2eb6;

L_2eb6:
    if ((HIWORD(dpT) != 0x0))
        goto L_2ecc;
    else
        goto L_2ec0;

L_2ec0:
    dpT = 0x1;

L_2ecc:
    SetTextColor(hdc, 0x7f);
    if ((vrgtok[viVCRFocus].grobj != grobjPlanet))
        goto L_2f2d;
    else
        goto L_2efe;

L_2efe:
    c = _wsprintf(szWork, PszGetCompressedString(idsDamageD), LOWORD(dpT));
    goto L_2f60;

L_2f2d:
    c = _wsprintf(szWork, PszGetCompressedString(idsDamageLdD), csh, SIGNHIWORD(csh), LOWORD(dpT));

L_2f60:
    TextOut(hdc, xT, y, szWork, c);
    SetTextColor(hdc, crButtonText);
    y = (y + dyArial8);
    /* untranslated: branch (hiword((uint32_t)(words(0x0, vrgtok[viVCRFocus].dpShield) * sext16to32(cshNew))) - HIWORD(dpShields)) > 0x0 ? L_2fea : L_2fc6 */

L_2fc6:
    /* untranslated: branch (hiword((uint32_t)(words(0x0, vrgtok[viVCRFocus].dpShield) * sext16to32(cshNew))) - HIWORD(dpShields)) < 0x0 ? L_2fd3 : L_2fcb */

L_2fcb:
    /* untranslated: branch (loword((uint32_t)(words(0x0, vrgtok[viVCRFocus].dpShield) * sext16to32(cshNew))) - LOWORD(dpShields)) > 0x0 ? L_2fea : L_2fd3 */

L_2fd3:
    c = CchGetString(idsShieldsNone, szWork);
    goto L_303d;

L_2fea:
    /* untranslated: c = _wsprintf(szWork, PszGetCompressedString(idsShieldsLd), (loword((uint32_t)(words(0x0, vrgtok[viVCRFocus].dpShield) *
     * sext16to32(cshNew))) - LOWORD(dpShields)), (hiword((uint32_t)(words(0x0, vrgtok[viVCRFocus].dpShield) * sext16to32(cshNew))) - HIWORD(dpShields))) */

L_303d:
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);
    if ((vrgtok[viVCRFocus].pctJam == 0x0))
        goto L_30dd;
    else
        goto L_307f;

L_307f:
    c = _wsprintf(szWork, PszGetCompressedString(idsJammingD), vrgtok[viVCRFocus].pctJam);
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);

L_30dd:
    if ((vrgtok[viVCRFocus].grobj == grobjPlanet))
        goto L_3207;
    else
        goto L_30ff;

L_30ff:
    if (((vbrcVCRFocus & 0xff) == 0xff))
        goto L_3136;
    else
        goto L_310d;

L_310d:
    /* untranslated: i = (((part[23:2](vrgtok[viVCRFocus]) >> 0x8) & 0xf) + 0x198) */
    goto L_313c;

L_3136:
    i = 0x19e;

L_313c:
    if ((i != 0x198))
        goto L_31ab;
    else
        goto L_3147;

L_3147:
    CchGetString(idsTacticSDMoves, szT);
    c = _wsprintf(szWork, szT, PszGetCompressedString(i), ((vrgtok[viVCRFocus].wFlags >> 0x5) & 0x1f));
    goto L_31e7;

L_31ab:
    CchGetString(idsTacticS, szT);
    c = _wsprintf(szWork, szT, PszGetCompressedString(i));

L_31e7:
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);

L_3207:
    if ((i == 0x19e))
        goto L_334f;
    else
        goto L_3212;

L_3212:
    CchGetString(idsPrimayTargetS, szT);
    /* untranslated: i = ((part[23:2](vrgtok[viVCRFocus]) & 0xf) + 0x190) */
    c = _wsprintf(szWork, szT, PszGetCompressedString(i));
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);
    CchGetString(idsSecondaryTargetS, szT);
    /* untranslated: i = (((part[23:2](vrgtok[viVCRFocus]) >> 0x4) & 0xf) + 0x190) */
    c = _wsprintf(szWork, szT, PszGetCompressedString(i));
    TextOut(hdc, x, y, szWork, c);
    y = (y + dyArial8);

L_3317:
    c = CchGetString(idsDead3, szWork);
    TextOut(hdc, x, y, szWork, c);
    y = (y + LOWORD((0x5 * dyArial8)));

L_334f:
    SetRect(&(rc), x, (y + 0x4), ((x + dyArial8) + 0x4), ((y + dyArial8) + 0x8));
    DrawBtn(hdc, &(rc), 0x8, 0x0, "?");

L_339a:
    iStart = 0x0;
    iEnd = 0x63;

L_33a4:
    i = iStart;
    goto L_39e8;

L_33ae:
    x = ((uint32_t)(i) % 0xa);
    y = ((uint32_t)(i) / 0xa);
    PatBlt(hdc, (LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa), (LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa), (dxyVCRSquare + 0x2), 0x1, BLACKNESS);
    PatBlt(hdc, (LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa), (LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa), 0x1, (dxyVCRSquare + 0x2), BLACKNESS);
    PatBlt(hdc, (((LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa) + dxyVCRSquare) + 0x1), (LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa), 0x1, (dxyVCRSquare + 0x2),
           BLACKNESS);
    PatBlt(hdc, (LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa), (((LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa) + dxyVCRSquare) + 0x1), (dxyVCRSquare + 0x2), 0x1,
           BLACKNESS);
    ctok = 0x0;
    ibmp = 0xffff;
    dpT = 0x0;
    j = 0x0;
    goto L_34d2;

L_34cd:
    j = (j + 0x1);

L_34d2:
    if ((j >= vlpbdVCR->ctok))
        goto L_3696;
    else
        goto L_34e6;

L_34e6:
    if ((vrgtok[j].brc != ((((y & 0xf) << 0x4) | (x & 0xf)) & 0xff)))
        goto L_34cd;
    else
        goto L_352b;

L_352b:
    if ((vrgtok[j].csh <= 0x0))
        goto L_34cd;
    else
        goto L_3548;

L_3548:
    ctok = (ctok + 0x1);
    LOWORD(dpT) = (LOWORD(dpT) + vrgtok[j].csh);
    HIWORD(dpT) = (HIWORD(dpT) + 0x0);
    if ((ibmp == 0xffff))
        goto L_3582;
    else
        goto L_3576;

L_3576:
    if ((j != viVCRFocus))
        goto L_34cd;
    else
        goto L_3582;

L_3582:
    if ((vrgtok[j].grobj != grobjPlanet))
        goto L_3605;
    else
        goto L_35a4;

L_35a4:
    ibmp = rglpshdefSB[vrgtok[j].iplr][(vrgtok[j].ishdef - 0x10)].hul.ibmp;
    goto L_3660;

L_3605:
    ibmp = rglpshdef[vrgtok[j].iplr][vrgtok[j].ishdef].hul.ibmp;

L_3660:
    ibmpRace = ((rgplr[vrgtok[j].iplr].wMdPlr >> 0x3) & 0x1f);

L_3696:
    if ((HIWORD(dpT) > 0x0))
        goto L_36bb;
    else
        goto L_369f;

L_369f:
    if ((HIWORD(dpT) < 0x0))
        goto L_36ae;
    else
        goto L_36a4;

L_36a4:
    if ((LOWORD(dpT) >= 0x7fff))
        goto L_36bb;
    else
        goto L_36ae;

L_36ae:
    csh = LOWORD(dpT);
    goto L_36c1;

L_36bb:
    csh = 0x7fff;

L_36c1:
    if ((ctok <= 0x0))
        goto L_3807;
    else
        goto L_36ca;

L_36ca:
    PatBlt(hdc, (LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa), (LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa), (dxyVCRSquare + 0x2), 0x1, BLACKNESS);
    PatBlt(hdc, (LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa), (LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa), 0x1, (dxyVCRSquare + 0x2), BLACKNESS);
    PatBlt(hdc, (((LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa) + dxyVCRSquare) + 0x1), (LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa), 0x1, (dxyVCRSquare + 0x2),
           BLACKNESS);
    PatBlt(hdc, (LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa), (((LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa) + dxyVCRSquare) + 0x1), (dxyVCRSquare + 0x2), 0x1,
           BLACKNESS);
    if ((dxyVCRSquare >= 0x40))
        goto L_37c7;
    else
        goto L_37c1;

L_37c1:
    t_merge_37ca_0001 = 0x1;
    goto L_37ca;

L_37c7:
    t_merge_37ca_0001 = 0x0;

L_37ca:
    DrawFleetBitmap(0x0, hdc, (LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xb), (LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xb), 0x0, ibmp, ctok, t_merge_37ca_0001,
                    ibmpRace, csh);
    goto L_3840;

L_3807:
    PatBlt(hdc, (LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa), (LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa), (dxyVCRSquare + 0x2), (dxyVCRSquare + 0x2), BLACKNESS);

L_3840:
    if (((vbrcVCRFocus & 0xff) != ((((y & 0xf) << 0x4) | (x & 0xf)) & 0xff)))
        goto L_387b;
    else
        goto L_3871;

L_3871:
    if ((viVCRFocus == 0xffff))
        goto L_38e7;
    else
        goto L_387b;

L_387b:
    if ((viVCRFocus < 0x0))
        goto L_39e3;
    else
        goto L_3885;

L_3885:
    if ((vrgtok[viVCRFocus].brc != ((((y & 0xf) << 0x4) | (x & 0xf)) & 0xff)))
        goto L_39e3;
    else
        goto L_38ca;

L_38ca:
    if ((vrgtok[viVCRFocus].csh <= 0x0))
        goto L_39e3;
    else
        goto L_38e7;

L_38e7:
    hbrSav = SelectObject(hdc, hbrBlue);
    PatBlt(hdc, (LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa), (LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa), (dxyVCRSquare + 0x1), 0x2, PATCOPY);
    PatBlt(hdc, (LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa), (LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa), 0x2, (dxyVCRSquare + 0x1), PATCOPY);
    PatBlt(hdc, (LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa), ((LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa) + dxyVCRSquare), (dxyVCRSquare + 0x1), 0x2, PATCOPY);
    PatBlt(hdc, ((LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa) + dxyVCRSquare), (LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa), 0x2, (dxyVCRSquare + 0x1), PATCOPY);
    SelectObject(hdc, hbrSav);

L_39e3:
    i = (i + 0x1);

L_39e8:
    if ((i <= iEnd))
        goto L_33ae;
    else
        goto L_39f4;

L_39f4:
    if ((vlpbrVCR->ctok <= 0x0))
        goto L_3a0d;
    else
        goto L_3a02;

L_3a02:
    AnimateAttack(hdc);

L_3a0d:
    SetBkMode(hdc, bkMode);
    SelectObject(hdc, hbrSav);
    if ((fCreatedDC == 0x0))
        goto L_3a38;
    else
        goto L_3a2c;

L_3a2c:
    ReleaseDC(hwndVCRDlg, hdc);

L_3a38:
    return;
}

void Delay(int16_t ctick) {
    uint32_t     dwTickLast;
    uint32_t     dwTickCur;
    tagTIMERINFO ti;

L_3a3e:
    ti.dwSize = 0xc;
    TimerCount(ti);
    dwTickLast = ti.dwmsSinceStart;

L_3a69:
    TimerCount(ti);
    dwTickCur = ti.dwmsSinceStart;
    if ((HIWORD(dwTickCur) < HIWORD(dwTickLast)))
        goto L_3abb;
    else
        goto L_3a8f;

L_3a8f:
    if ((HIWORD(dwTickCur) > HIWORD(dwTickLast)))
        goto L_3a9c;
    else
        goto L_3a94;

L_3a94:
    if ((LOWORD(dwTickCur) < LOWORD(dwTickLast)))
        goto L_3abb;
    else
        goto L_3a9c;

L_3a9c:
    if ((HIWORD(dwTickCur) < (SIGNHIWORD(ctick) + HIWORD(dwTickLast))))
        goto L_3a69;
    else
        goto L_3aae;

L_3aae:
    if ((HIWORD(dwTickCur) > (SIGNHIWORD(ctick) + HIWORD(dwTickLast))))
        goto L_3abb;
    else
        goto L_3ab3;

L_3ab3:
    if ((LOWORD(dwTickCur) < (ctick + LOWORD(dwTickLast))))
        goto L_3a69;
    else
        goto L_3abb;

L_3abb:
    return;
}

void AnimateAttack(HDC hdc) {
    TOK         *ptokSrc;
    TOK         *ptokAttack;
    POINT        ptBeam1;
    int16_t      cFrame;
    int16_t      dyFrame;
    POINT        ptRay2;
    POINT        ptTop;
    uint32_t     dwTickLast;
    int16_t      dxFrame;
    uint32_t     dwTickCur;
    POINT        ptBase;
    int16_t      dy;
    POINT        ptRay1;
    int16_t      y;
    POINT        ptRight;
    POINT        ptDest;
    int16_t      iHit;
    uint16_t     grfWeapon;
    POINT        ptSrc;
    POINT        ptTorp;
    POINT        ptLeft;
    tagTIMERINFO ti;
    int16_t      iFrame;
    int16_t      dx;
    int16_t      fKill;
    POINT        ptDestBottom;
    POINT        ptBeam2;
    POINT        ptBottom;
    POINT        ptDestTop;
    POINT        ptDestRight;
    POINT        ptDestLeft;
    int16_t      x;
    HDC          hdcMem;
    HBITMAP      hbmpSav;
    HBITMAP      hbmpScreen;
    int16_t      t_merge_3ef2_0001;
    int16_t      t_merge_3ef2_0002;
    int16_t      t_merge_3f10_0001;
    int16_t      t_merge_3f10_0002;
    int16_t      t_merge_3f2e_0001;
    int16_t      t_merge_3f2e_0002;
    int16_t      t_merge_3f96_0001;
    int16_t      t_merge_3f96_0002;
    int16_t      t_merge_3fb4_0001;
    int16_t      t_merge_3fb4_0002;
    int16_t      t_merge_3fd2_0001;
    int16_t      t_merge_3fd2_0002;
    HPEN         t_merge_40e7_0001;
    uint16_t     t_merge_41f9_0001;
    uint16_t     t_merge_4438_0001;
    uint16_t     t_merge_44fc_0001;

L_3ac2:
    grfWeapon = 0x0;
    fKill = 0x0;
    if ((viStepVCRCur < 0x0))
        goto L_4512;
    else
        goto L_3adc;

L_3adc:

L_3ae2:
    ptokSrc = &(vrgtok[vlpbrVCR->itok]);
    x = (ptokSrc->brc & 0xf);
    y = (ptokSrc->brc >> 0x4);
    ptSrc.x = (((LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa) + ((uint32_t)(dxyVCRSquare) / 0x2)) + 0x1);
    ptSrc.y = (((LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa) + ((uint32_t)(dxyVCRSquare) / 0x2)) + 0x1);
    ptTop.x = (((LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa) + ((uint32_t)(dxyVCRSquare) / 0x2)) + 0x1);
    /* untranslated: ptBottom.x = (((loword(((dxyVCRSquare + 0x3) * x)) + 0xa) + ss:[bp-0x76]) + 0x1) */
    ptRight.y = (((LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa) + ((uint32_t)(dxyVCRSquare) / 0x2)) + 0x1);
    /* untranslated: ptLeft.y = (((loword(((dxyVCRSquare + 0x3) * y)) + 0xa) + ss:[bp-0x76]) + 0x1) */
    ptBottom.y = (((uint32_t)(dxyVCRSquare) / 0x3) + ptLeft.y);
    ptTop.y = (ptLeft.y - ((uint32_t)(dxyVCRSquare) / 0x3));
    ptLeft.x = (ptBottom.x - ((uint32_t)(dxyVCRSquare) / 0x3));
    ptRight.x = (((uint32_t)(dxyVCRSquare) / 0x3) + ptBottom.x);
    iHit = 0x0;

LNextTarget:
    grfWeapon = vlpbrVCR->rgkill[iHit].grfWeapon;
    if ((vlpbrVCR->rgkill[iHit].cshKill == 0x0))
        goto L_3c56;
    else
        goto L_3c51;

L_3c51:
    fKill = 0x1;

L_3c56:
    iFrame = (iHit + 0x1);
    goto L_3c66;

L_3c62:
    iFrame = (iFrame + 0x1);

L_3c66:
    if ((iFrame >= vlpbrVCR->ctok))
        goto L_3d18;
    else
        goto L_3c76;

L_3c76:
    if ((vlpbrVCR->rgkill[iFrame].itok != vlpbrVCR->rgkill[iHit].itok))
        goto L_3d18;
    else
        goto L_3cc2;

L_3cc2:

L_3cc8:
    grfWeapon = (grfWeapon | vlpbrVCR->rgkill[iFrame].grfWeapon);
    if ((vlpbrVCR->rgkill[iFrame].cshKill == 0x0))
        goto L_3c62;
    else
        goto L_3d10;

L_3d10:
    fKill = 0x1;

L_3d18:
    ptokAttack = &(vrgtok[vlpbrVCR->rgkill[iHit].itok]);
    x = (ptokAttack->brc & 0xf);
    y = (ptokAttack->brc >> 0x4);
    dx = ((ptokSrc->brc & 0xf) - x);
    dy = ((ptokSrc->brc >> 0x4) - y);
    ptDest.x = (((LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa) + ((uint32_t)(dxyVCRSquare) / 0x2)) + 0x1);
    ptDest.y = (((LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa) + ((uint32_t)(dxyVCRSquare) / 0x2)) + 0x1);
    ptDestTop.x = (((LOWORD(((dxyVCRSquare + 0x3) * x)) + 0xa) + ((uint32_t)(dxyVCRSquare) / 0x2)) + 0x1);
    /* untranslated: ptDestBottom.x = (((loword(((dxyVCRSquare + 0x3) * x)) + 0xa) + ss:[bp-0x76]) + 0x1) */
    ptDestRight.y = (((LOWORD(((dxyVCRSquare + 0x3) * y)) + 0xa) + ((uint32_t)(dxyVCRSquare) / 0x2)) + 0x1);
    /* untranslated: ptDestLeft.y = (((loword(((dxyVCRSquare + 0x3) * y)) + 0xa) + ss:[bp-0x76]) + 0x1) */
    ptDestBottom.y = (((uint32_t)(dxyVCRSquare) / 0x3) + ptDestLeft.y);
    ptDestTop.y = (ptDestLeft.y - ((uint32_t)(dxyVCRSquare) / 0x3));
    ptDestLeft.x = (ptDestBottom.x - ((uint32_t)(dxyVCRSquare) / 0x3));
    ptDestRight.x = (((uint32_t)(dxyVCRSquare) / 0x3) + ptDestBottom.x);
    iHit = iFrame;
    if ((dx != 0x0))
        goto L_3e93;
    else
        goto L_3e8a;

L_3e8a:
    if ((dy == 0x0))
        goto LFinishUp;
    else
        goto L_3e93;

L_3e93:
    if ((dx == 0x0))
        goto L_3ec2;
    else
        goto L_3e9c;

L_3e9c:
    if ((abs(dx) != 0x1))
        goto L_3f37;
    else
        goto L_3eaf;

L_3eaf:
    if ((abs(dy) <= 0x2))
        goto L_3f37;
    else
        goto L_3ec2;

L_3ec2:
    ptBeam1.x = ptRight.x;
    ptBeam1.y = ptRight.y;
    ptBeam2.x = ptLeft.x;
    ptBeam2.y = ptLeft.y;
    if ((dy <= 0x0))
        goto L_3eec;
    else
        goto L_3ee3;

L_3ee3:
    t_merge_3ef2_0001 = ptTop.x;
    t_merge_3ef2_0002 = ptTop.y;
    goto L_3ef2;

L_3eec:
    t_merge_3ef2_0001 = ptBottom.x;
    t_merge_3ef2_0002 = ptBottom.y;

L_3ef2:
    ptTorp.x = t_merge_3ef2_0001;
    ptTorp.y = t_merge_3ef2_0002;
    if ((dy <= 0x0))
        goto L_3f0a;
    else
        goto L_3f01;

L_3f01:
    t_merge_3f10_0001 = ptDestLeft.x;
    t_merge_3f10_0002 = ptDestLeft.y;
    goto L_3f10;

L_3f0a:
    t_merge_3f10_0001 = ptDestRight.x;
    t_merge_3f10_0002 = ptDestRight.y;

L_3f10:
    ptRay2.x = t_merge_3f10_0001;
    ptRay2.y = t_merge_3f10_0002;
    if ((dy <= 0x0))
        goto L_3f28;
    else
        goto L_3f1f;

L_3f1f:
    t_merge_3f2e_0001 = ptDestRight.x;
    t_merge_3f2e_0002 = ptDestRight.y;
    goto L_3f2e;

L_3f28:
    t_merge_3f2e_0001 = ptDestLeft.x;
    t_merge_3f2e_0002 = ptDestLeft.y;

L_3f2e:
    ptRay1.x = t_merge_3f2e_0001;
    ptRay1.y = t_merge_3f2e_0002;
    goto L_40bf;

L_3f37:
    if ((dy == 0x0))
        goto L_3f66;
    else
        goto L_3f40;

L_3f40:
    if ((abs(dy) != 0x1))
        goto L_3fdb;
    else
        goto L_3f53;

L_3f53:
    if ((abs(dx) <= 0x2))
        goto L_3fdb;
    else
        goto L_3f66;

L_3f66:
    ptBeam1.x = ptTop.x;
    ptBeam1.y = ptTop.y;
    ptBeam2.x = ptBottom.x;
    ptBeam2.y = ptBottom.y;
    if ((dx <= 0x0))
        goto L_3f90;
    else
        goto L_3f87;

L_3f87:
    t_merge_3f96_0001 = ptLeft.x;
    t_merge_3f96_0002 = ptLeft.y;
    goto L_3f96;

L_3f90:
    t_merge_3f96_0001 = ptRight.x;
    t_merge_3f96_0002 = ptRight.y;

L_3f96:
    ptTorp.x = t_merge_3f96_0001;
    ptTorp.y = t_merge_3f96_0002;
    if ((dx <= 0x0))
        goto L_3fae;
    else
        goto L_3fa5;

L_3fa5:
    t_merge_3fb4_0001 = ptDestTop.x;
    t_merge_3fb4_0002 = ptDestTop.y;
    goto L_3fb4;

L_3fae:
    t_merge_3fb4_0001 = ptDestBottom.x;
    t_merge_3fb4_0002 = ptDestBottom.y;

L_3fb4:
    ptRay1.x = t_merge_3fb4_0001;
    ptRay1.y = t_merge_3fb4_0002;
    if ((dx <= 0x0))
        goto L_3fcc;
    else
        goto L_3fc3;

L_3fc3:
    t_merge_3fd2_0001 = ptDestBottom.x;
    t_merge_3fd2_0002 = ptDestBottom.y;
    goto L_3fd2;

L_3fcc:
    t_merge_3fd2_0001 = ptDestTop.x;
    t_merge_3fd2_0002 = ptDestTop.y;

L_3fd2:
    ptRay2.x = t_merge_3fd2_0001;
    ptRay2.y = t_merge_3fd2_0002;
    goto L_40bf;

L_3fdb:
    if ((dx <= 0x0))
        goto L_4053;
    else
        goto L_3fe4;

L_3fe4:
    if ((dy <= 0x0))
        goto L_4014;
    else
        goto L_3fed;

L_3fed:
    ptRay2.x = ptDestBottom.x;
    ptRay2.y = ptDestBottom.y;
    ptRay1.x = ptDestRight.x;
    ptRay1.y = ptDestRight.y;
    ptBeam1.x = ptTop.x;
    ptBeam1.y = ptTop.y;
    goto L_4038;

L_4014:
    ptRay2.x = ptDestRight.x;
    ptRay2.y = ptDestRight.y;
    ptRay1.x = ptDestTop.x;
    ptRay1.y = ptDestTop.y;
    ptBeam1.x = ptBottom.x;
    ptBeam1.y = ptBottom.y;

L_4038:
    ptBeam2.x = ptLeft.x;
    ptBeam2.y = ptLeft.y;
    ptTorp.x = ptLeft.x;
    ptTorp.y = ptBeam1.y;
    goto L_40bf;

L_4053:
    if ((dy <= 0x0))
        goto L_4083;
    else
        goto L_405c;

L_405c:
    ptBeam1.x = ptTop.x;
    ptBeam1.y = ptTop.y;
    ptRay1.x = ptDestBottom.x;
    ptRay1.y = ptDestBottom.y;
    ptRay2.x = ptDestLeft.x;
    ptRay2.y = ptDestLeft.y;
    goto L_40a7;

L_4083:
    ptBeam1.x = ptBottom.x;
    ptBeam1.y = ptBottom.y;
    ptRay1.x = ptDestLeft.x;
    ptRay1.y = ptDestLeft.y;
    ptRay2.x = ptDestTop.x;
    ptRay2.y = ptDestTop.y;

L_40a7:
    ptBeam2.x = ptRight.x;
    ptBeam2.y = ptRight.y;
    ptTorp.x = ptRight.x;
    ptTorp.y = ptBeam1.y;

L_40bf:
    if (((grfWeapon & 0x3) == 0x0))
        goto L_413f;
    else
        goto L_40cd;

L_40cd:
    if (((grfWeapon & 0x2) == 0x0))
        goto L_40e4;
    else
        goto L_40de;

L_40de:
    t_merge_40e7_0001 = hpenStarbase;
    goto L_40e7;

L_40e4:
    t_merge_40e7_0001 = hpenEnemy;

L_40e7:
    SelectObject(hdc, t_merge_40e7_0001);
    MoveTo(hdc, ptBeam1.x, ptBeam1.y);
    LineTo(hdc, ptDest.x, ptDest.y);
    MoveTo(hdc, ptBeam2.x, ptBeam2.y);
    LineTo(hdc, ptDest.x, ptDest.y);
    DrawIcon(hdc, (ptDest.x + 0xfff0), (ptDest.y + 0xfff0), rghiconVCR[0x0]);

L_413f:
    if (((grfWeapon & 0x4) == 0x0))
        goto LFinishUp;
    else
        goto L_414d;

L_414d:
    if ((fAnimate == 0x0))
        goto L_43b4;
    else
        goto L_4157;

L_4157:
    hdcMem = CreateCompatibleDC(hdc);
    if ((hdcMem == 0x0))
        goto LFinishUp;
    else
        goto L_4168;

L_4168:

L_416e:
    hbmpScreen = CreateCompatibleBitmap(hdc, 0x20, 0x20);
    if ((hbmpScreen != 0x0))
        goto L_4195;
    else
        goto L_418a;

L_418a:
    DeleteDC(hdcMem);
    goto LFinishUp;

L_4195:
    hbmpSav = SelectObject(hdcMem, hbmpScreen);
    /* untranslated: ss:[bp-0x7c] = abs(dx) */
    /* untranslated: branch ss:[bp-0x7c] <= abs(dy) ? L_41d4 : L_41c6 */

L_41c6:
    abs(dx);
    goto L_41df;

L_41d4:
    abs(dy);

L_41df:
    if (((grfWeapon & 0x4) == 0x0))
        goto L_41f6;
    else
        goto L_41f0;

L_41f0:
    t_merge_41f9_0001 = 0x8;
    goto L_41f9;

L_41f6:
    t_merge_41f9_0001 = 0x4;

L_41f9:
    /* untranslated: cFrame = loword((ss:[bp-0x7e] * t_merge_41f9_0001)) */
    ptBase.x = ptTorp.x;
    ptBase.y = ptTorp.y;
    dxFrame = (ptTorp.x - ptDest.x);
    dyFrame = (ptTorp.y - ptDest.y);
    ti.dwSize = 0xc;
    TimerCount(ti);
    dwTickLast = ti.dwmsSinceStart;
    iFrame = 0x0;
    goto L_438e;

L_424b:
    BitBlt(hdcMem, 0x0, 0x0, 0x20, 0x20, hdc, (ptTorp.x + 0xfff0), (ptTorp.y + 0xfff0), SRCCOPY);
    DrawIcon(hdc, (ptTorp.x + 0xfff0), (ptTorp.y + 0xfff0), rghiconVCR[((iFrame & 0x3) + 0x3)]);

L_42a1:
    TimerCount(ti);
    dwTickCur = ti.dwmsSinceStart;
    if ((HIWORD(dwTickCur) < HIWORD(dwTickLast)))
        goto L_4301;
    else
        goto L_42c7;

L_42c7:
    if ((HIWORD(dwTickCur) > HIWORD(dwTickLast)))
        goto L_42d4;
    else
        goto L_42cc;

L_42cc:
    if ((LOWORD(dwTickCur) < LOWORD(dwTickLast)))
        goto L_4301;
    else
        goto L_42d4;

L_42d4:
    if ((HIWORD(dwTickCur) < ((HIWORD(dwTickLast) + 0x0) - SIGNHIWORD(LOWORD((0xa * viSpeedVCR))))))
        goto L_42a1;
    else
        goto L_42f4;

L_42f4:
    if ((HIWORD(dwTickCur) > ((HIWORD(dwTickLast) + 0x0) - SIGNHIWORD(LOWORD((0xa * viSpeedVCR))))))
        goto L_4301;
    else
        goto L_42f9;

L_42f9:
    if ((LOWORD(dwTickCur) < ((LOWORD(dwTickLast) + 0x23) - LOWORD((0xa * viSpeedVCR)))))
        goto L_42a1;
    else
        goto L_4301;

L_4301:
    dwTickLast = dwTickCur;
    BitBlt(hdc, (ptTorp.x + 0xfff0), (ptTorp.y + 0xfff0), 0x20, 0x20, hdcMem, 0x0, 0x0, SRCCOPY);
    ptTorp.x = (ptBase.x - LOWORD((int32_t)(((uint32_t)(((uint32_t)(dxFrame) * (uint32_t)(iFrame))) / (uint32_t)(cFrame)))));
    ptTorp.y = (ptBase.y - LOWORD((int32_t)(((uint32_t)(((uint32_t)(dyFrame) * (uint32_t)(iFrame))) / (uint32_t)(cFrame)))));
    iFrame = (iFrame + 0x1);

L_438e:
    if ((iFrame < cFrame))
        goto L_424b;
    else
        goto L_4399;

L_4399:
    SelectObject(hdcMem, hbmpSav);
    DeleteObject(hbmpScreen);
    DeleteDC(hdcMem);

L_43b4:
    if (((grfWeapon & 0x40) != 0x0))
        goto LFinishUp;
    else
        goto L_43c2;

L_43c2:
    DrawIcon(hdc, (ptDest.x + 0xfff0), (ptDest.y + 0xfff0), rghiconVCR[0x1]);

LFinishUp:
    if ((iHit < vlpbrVCR->ctok))
        goto LNextTarget;
    else
        goto L_43e9;

L_43e9:

L_43ef:
    iFrame = 0x0;
    goto L_43fb;

L_43f7:
    iFrame = (iFrame + 0x1);

L_43fb:
    if ((iFrame >= vlpbrVCR->ctok))
        goto L_450c;
    else
        goto L_440b;

L_440b:
    if ((vlpbrVCR->rgkill[iFrame].cshKill <= 0x0))
        goto L_4435;
    else
        goto L_442f;

L_442f:
    t_merge_4438_0001 = 0x1;
    goto L_4438;

L_4435:
    t_merge_4438_0001 = 0x0;

L_4438:
    fKill = t_merge_4438_0001;
    ptokAttack = &(vrgtok[vlpbrVCR->rgkill[iFrame].itok]);
    ptDest.x = (((LOWORD(((ptokAttack->brc & 0xf) * (dxyVCRSquare + 0x3))) + 0xa) + ((uint32_t)(dxyVCRSquare) / 0x2)) + 0x1);
    ptDest.y = (((LOWORD(((ptokAttack->brc >> 0x4) * (dxyVCRSquare + 0x3))) + 0xa) + ((uint32_t)(dxyVCRSquare) / 0x2)) + 0x1);
    if ((fKill == 0x0))
        goto L_44f9;
    else
        goto L_44f3;

L_44f3:
    t_merge_44fc_0001 = 0x2;
    goto L_44fc;

L_44f9:
    t_merge_44fc_0001 = 0x0;

L_44fc:
    DrawIcon(hdc, (ptDest.x + 0xfff0), (ptDest.y + 0xfff0), rghiconVCR[t_merge_44fc_0001]);
    goto L_43f7;

L_450c:
    fAnimate = 0x0;

L_4512:
    return;
}

int16_t PopupVCRMenu(HWND hwnd, int16_t x, int16_t y, uint8_t brc) {
    int16_t  fAttack;
    char    *rgsz[40];
    int16_t  i;
    int16_t  c;
    char     rgch[1536];
    SHDEF   *lpshdef;
    int16_t  rgid[40];
    int16_t  iChecked;
    int16_t  iSel;
    int16_t  j;
    char    *psz;
    int16_t  cch;
    int16_t  cKilled;
    uint16_t t_merge_4582_0001;

L_4518:
    c = 0x0;
    iChecked = 0xffff;
    psz = rgch;
    if ((brc != vrgtok[((*(vlpbrVCR + 0x4) >> 0x8) & 0xff)].brc))
        goto L_457f;
    else
        goto L_4579;

L_4579:
    t_merge_4582_0001 = 0x1;
    goto L_4582;

L_457f:
    t_merge_4582_0001 = 0x0;

L_4582:
    fAttack = t_merge_4582_0001;
    i = 0x0;
    goto L_4591;

L_458d:
    i = (i + 0x1);

L_4591:
    if ((i >= vlpbdVCR->ctok))
        goto L_4897;
    else
        goto L_45a4;

L_45a4:
    if ((vrgtok[i].brc != brc))
        goto L_458d;
    else
        goto L_45d2;

L_45d2:
    if ((vrgtok[i].csh <= 0x0))
        goto L_458d;
    else
        goto L_45ee;

L_45ee:
    if ((PszPlayerName(vrgtok[i].iplr, 0x0, 0x0, 0x0, 0x0, 0x0) != 0x57a4))
        goto L_4632;
    else
        goto L_462c;

L_462c:
    goto L_4635;

L_4632:

L_4635:
    cch = strlen(szWork);
    if ((vrgtok[i].grobj != grobjPlanet))
        goto L_46c3;
    else
        goto L_4666;

L_4666:
    lpshdef = &(rglpshdefSB[vrgtok[i].iplr][(vrgtok[i].ishdef - 0x10)]);
    goto L_471a;

L_46c3:
    lpshdef = &(rglpshdef[vrgtok[i].iplr][vrgtok[i].ishdef]);

L_471a:
    cch = (cch + _wsprintf(&(szWork[cch]), " %s * %d", (LOWORD(lpshdef) + 0x8), HIWORD(lpshdef), vrgtok[i].csh));
    if ((fAttack == 0x0))
        goto L_481e;
    else
        goto L_4766;

L_4766:
    if ((vlpbrVCR->ctok <= 0x0))
        goto L_481e;
    else
        goto L_4774;

L_4774:
    if ((viStepVCRCur < 0x0))
        goto L_481e;
    else
        goto L_477e;

L_477e:
    cKilled = 0x0;
    j = 0x0;
    goto L_4792;

L_478d:
    j = (j + 0x1);

L_4792:
    if ((j >= vlpbrVCR->ctok))
        goto L_47f2;
    else
        goto L_47a3;

L_47a3:
    if ((vlpbrVCR->rgkill[j].itok != i))
        goto L_478d;
    else
        goto L_47cc;

L_47cc:
    cKilled = (cKilled + vlpbrVCR->rgkill[j].cshKill);

L_47f2:
    if ((cKilled <= 0x0))
        goto L_481e;
    else
        goto L_47fc;

L_47fc:
    cch = (cch + _wsprintf(&(szWork[cch]), " (-%d)", cKilled));

L_481e:
    if ((((psz + cch) + 0x1) >= &(rgch[0x5ff])))
        goto L_4897;
    else
        goto L_4837;

L_4837:
    if ((c >= 0x28))
        goto L_4897;
    else
        goto L_483d;

L_483d:

L_4843:
    rgsz[c] = psz;
    rgid[c] = i;
    strcpy(psz, szWork);
    psz = (psz + (cch + 0x1));
    if ((i != viVCRFocus))
        goto L_4890;
    else
        goto L_4889;

L_4889:
    iChecked = c;

L_4890:
    c = (c + 0x1);

L_4897:
    if ((c != 0x0))
        goto L_48a6;
    else
        goto L_48a0;

L_48a0:
    return 0xffff;

L_48a6:
    iSel = PopupMenu(hwnd, x, y, c, 0x0, rgsz, iChecked, 0x1);
    if ((iSel != 0xffff))
        goto L_48de;
    else
        goto L_48d8;

L_48d8:
    return 0xffff;

L_48de:
    return rgid[iSel];
}

void EnableVCRButtons() {
    int16_t  i;
    uint16_t t_merge_4927_0001;
    uint16_t t_merge_4965_0001;

L_48f6:
    i = 0xa1;
    goto L_4931;

L_4907:
    GetDlgItem(hwndVCRDlg, i);
    if ((viStepVCRCur <= 0xffff))
        goto L_4924;
    else
        goto L_491e;

L_491e:
    t_merge_4927_0001 = 0x1;
    goto L_4927;

L_4924:
    t_merge_4927_0001 = 0x0;

L_4927:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_4927_0001) -> callresult(int16_t) */
    i = (i + 0x1);

L_4931:
    if ((i < 0xa3))
        goto L_4907;
    else
        goto L_493b;

L_493b:
    i = 0xa3;
    goto L_496f;

L_4943:
    GetDlgItem(hwndVCRDlg, i);
    if ((viStepVCRCur >= vcStepVCR))
        goto L_4962;
    else
        goto L_495c;

L_495c:
    t_merge_4965_0001 = 0x1;
    goto L_4965;

L_4962:
    t_merge_4965_0001 = 0x0;

L_4965:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_4965_0001) -> callresult(int16_t) */
    i = (i + 0x1);

L_496f:
    if ((i < 0xa6))
        goto L_4943;
    else
        goto L_4979;

L_4979:
    if ((viStepVCRCur != 0xffff))
        goto L_4999;
    else
        goto L_4983;

L_4983:
    SetFocus(GetDlgItem(hwndVCRDlg, IDC_U16_0x00A3));
    goto L_49b8;

L_4999:
    if ((viStepVCRCur != vcStepVCR))
        goto L_49b8;
    else
        goto L_49a5;

L_49a5:
    SetFocus(GetDlgItem(hwndVCRDlg, IDOK));

L_49b8:
    return;
}
