#include "common.h"

int16_t ChangeProduction(int16_t fClear) {
    jmp_buf  env[9];
    jmp_buf *penvMemSav[9];
    int16_t (**lpProcProd)();
    PROD    rgprod[64];
    int16_t fSuccess;

L_0000:
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_00a9;
    else
        goto L_0029;

L_0029:
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_003d;
    else
        goto L_0033;

L_0033:
    if ((HIWORD(lpplProdGlob) == 0x0))
        goto L_004d;
    else
        goto L_003d;

L_003d:
    FreePl(lpplProdGlob);

L_004d:
    lpplProdGlob = 0x0;
    if ((hwndProdDlg == 0x0))
        goto L_0070;
    else
        goto L_0063;

L_0063:
    EndDialog(hwndProdDlg, 0x0);

L_0070:
    hwndProdDlg = 0x0;
    fDlgUp = 0x0;
    AlertSz(PszFormatIds(idsThereIsntEnoughFreeMemoryModifyProduction, 0x0), MB_ICONHAND);
    penvMem = penvMemSav;
    return 0x0;

L_00a9:
    if ((fClear == 0x0))
        goto L_00bb;
    else
        goto L_00b2;

L_00b2:
    fSuccess = 0x1;
    goto LWriteProdQ;

L_00bb:
    InitProduction(rgprod);
    fDlgUp = 0x1;
    lpProcProd = MakeProcInstance(ProductionDlg, hInst);
    fSuccess = DialogBox(hInst, IDD_PRODUCTION, hwndFrame, &(lpProcProd));
    FreeProcInstance(&(lpProcProd));
    hwndProdDlg = 0x0;
    fDlgUp = 0x0;

LWriteProdQ:
    FinishProduction(fSuccess);
    if ((fSuccess == 0x0))
        goto L_014b;
    else
        goto L_0131;

L_0131:
    if ((sel.grobj != grobjPlanet))
        goto L_014b;
    else
        goto L_013b;

L_013b:
    DrawPlanShip(0x0, 0x8);

L_014b:
    penvMem = penvMemSav;
    return 0x1;
}

void InitProduction(PROD *rgprod) {
    int16_t  iWarp;
    int16_t  iSrc;
    uint16_t u;
    int16_t  i;
    int16_t  ipl;
    PART     part;
    PROD    *lpprod;
    uint16_t t_merge_0733_0001;
    uint32_t t_merge_0733_0002_wide;
    uint16_t t_merge_0733_0003;
    uint16_t t_merge_083a_0001;
    uint32_t t_merge_083a_0002_wide;
    uint16_t t_merge_083a_0003;

L_015e:
    /* untranslated: LOWORD(gd) = ((LOWORD(gd) & 0xffdf) | (((loword((uint32_t)(words(part[176:2](sel), part[178:2](sel)) >> 0x17)) & 0x1) & 0x1) << 0x5)) */
    if ((rgprod != 0x0))
        goto L_01ad;
    else
        goto L_01a7;

L_01a7:
    rgprod = pProdGlob;

L_01ad:
    if ((LOWORD(sel.pl.lpplprod) != 0x0))
        goto L_01c1;
    else
        goto L_01b7;

L_01b7:
    if ((HIWORD(sel.pl.lpplprod) == 0x0))
        goto L_01d2;
    else
        goto L_01c1;

L_01c1:
    i = sel.pl.lpplprod->iprodMac;
    goto L_01d7;

L_01d2:
    i = 0x2;

L_01d7:
    lpplProdGlob = LpplAlloc(0x4, i, htOrd);
    if ((LOWORD(sel.pl.lpplprod) != 0x0))
        goto L_0206;
    else
        goto L_01fc;

L_01fc:
    if ((HIWORD(sel.pl.lpplprod) == 0x0))
        goto L_0237;
    else
        goto L_0206;

L_0206:
    fmemcpy(&(lpplProdGlob->rgprod), &(sel.pl.lpplprod->rgprod), (i * 0x4));
    goto L_023c;

L_0237:
    i = 0x0;

L_023c:
    lpplProdGlob->iprodMac = LOBYTE(i);
    cProdGlob = 0x0;
    pProdGlob = rgprod;
    memset(rgprod, 0x0, 0x100);
    if ((sel.pl.fStarbase == 0x0))
        goto L_03f0;
    else
        goto L_0279;

L_0279:
    if ((LphuldefFromId(rglpshdefSB[idPlayer][sel.pl.isb].hul.ihuldef)->hul.wtCargoMax == 0x0))
        goto L_03f0;
    else
        goto L_02b1;

L_02b1:
    i = 0x0;
    goto L_03e7;

L_02b9:
    /* untranslated: branch ((part[123:2](rgshdef[i]) >> 0x9) & 0x1) != 0x0 ? L_03e3 : L_02d7 */

L_02d7:
    /* untranslated: branch ((part[123:2](rgshdef[i]) >> 0xf) & 0x1) != 0x0 ? L_03e3 : L_02f5 */

L_02f5:
    /* untranslated: ss:[bp-0x1a] = rgshdef[i].hul.wtEmpty */
    /* untranslated: branch LphuldefFromId(rglpshdefSB[idPlayer][sel.pl.isb].hul.ihuldef)->hul.wtCargoMax < ss:[bp-0x1a] ? L_03e3 : L_0340 */

L_0340:
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xfc00) | 0x3ff);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    /* untranslated: LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0x3ff) | loword((int32_t)(words((i & 0x7f), (signhiword(i) & 0x0)) << 0xa))) */
    /* untranslated: HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfffe) | hiword((int32_t)(words((i & 0x7f), (signhiword(i) & 0x0)) << 0xa))) */
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfff1) | 0x4);
    cProdGlob = (cProdGlob + 0x1);

L_03e3:
    i = (i + 0x1);

L_03e7:
    if ((i < 0x10))
        goto L_02b9;
    else
        goto L_03f0;

L_03f0:
    i = 0x0;
    goto L_0531;

L_03f8:
    /* untranslated: branch ((part[123:2](rglpshdefSB[idPlayer][i]) >> 0x9) & 0x1) != 0x0 ? L_052d : L_0426 */

L_0426:
    /* untranslated: branch ((part[123:2](rglpshdefSB[idPlayer][i]) >> 0xf) & 0x1) != 0x0 ? L_052d : L_0454 */

L_0454:
    if ((sel.pl.isb != i))
        goto L_0477;
    else
        goto L_0464;

L_0464:
    if ((sel.pl.fStarbase != 0x0))
        goto L_052d;
    else
        goto L_0477;

L_0477:
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xfc00) | 0x1);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    /* untranslated: LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0x3ff) | loword((int32_t)(words(((i + 0x10) & 0x7f), (signhiword((i + 0x10)) &
     * 0x0)) << 0xa))) */
    /* untranslated: HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfffe) | hiword((int32_t)(words(((i + 0x10) & 0x7f), (signhiword((i + 0x10)) &
     * 0x0)) << 0xa))) */
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfff1) | 0x4);
    cProdGlob = (cProdGlob + 0x1);

L_052d:
    i = (i + 0x1);

L_0531:
    if ((i < 0xa))
        goto L_03f8;
    else
        goto L_053a;

L_053a:
    part.hs.grhst = hstPlanetary;
    part.hs.iItem = 0xe;
    if ((FLookupPart(&(part)) != 0x1))
        goto L_05f1;
    else
        goto L_055f;

L_055f:
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xfc00) | 0x1);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0x3ff) | 0x3400);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfffe) | 0x0);
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfff1) | 0x2);
    cProdGlob = (cProdGlob + 0x1);

L_05f1:
    iWarp = IWarpMAFromLppl(&(sel.pl), 0x0);
    if ((iWarp <= 0x0))
        goto L_06db;
    else
        goto L_0610;

L_0610:
    i = 0x0;
    goto L_06d2;

L_0618:
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xfc00) | 0x3ff);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    /* untranslated: LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0x3ff) | loword((int32_t)(words(((i + 0xe) & 0x7f), (signhiword((i + 0xe)) &
     * 0x0)) << 0xa))) */
    /* untranslated: HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfffe) | hiword((int32_t)(words(((i + 0xe) & 0x7f), (signhiword((i + 0xe)) &
     * 0x0)) << 0xa))) */
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfff1) | 0x2);
    cProdGlob = (cProdGlob + 0x1);
    i = (i + 0x1);

L_06d2:
    if ((i < 0x4))
        goto L_0618;
    else
        goto L_06db;

L_06db:
    /* untranslated: ss:[bp-0x1a] = (loword((uint32_t)(words(part[172:2](sel), part[174:2](sel)) >> 0x14)) & 0xfff) */
    /* untranslated: u = (CMaxFactories(&sel.pl, idPlayer) - ss:[bp-0x1a]) */
    if ((u <= 0x0))
        goto L_07e2;
    else
        goto L_0719;

L_0719:
    if ((0x3fc >= u))
        goto L_072d;
    else
        goto L_0724;

L_0724:
    t_merge_0733_0001 = 0x3fc;
    t_merge_0733_0002_wide = 0x0;
    t_merge_0733_0003 = 0x3fc;
    goto L_0733;

L_072d:
    t_merge_0733_0001 = u;
    t_merge_0733_0002_wide = 0x0;
    t_merge_0733_0003 = u;

L_0733:
    /* untranslated: LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xfc00) | loword((int32_t)(words((t_merge_0733_0001 & 0x3ff),
     * (loword(t_merge_0733_0002_wide) & 0x0)) << 0x0))) */
    /* untranslated: HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xffff) | hiword((int32_t)(words((t_merge_0733_0003 & 0x3ff),
     * (hiword(t_merge_0733_0002_wide) & 0x0)) << 0x0))) */
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0x3ff) | 0x1c00);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfffe) | 0x0);
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfff1) | 0x2);
    cProdGlob = (cProdGlob + 0x1);

L_07e2:
    /* untranslated: ss:[bp-0x1a] = (loword((uint32_t)(words(part[172:2](sel), part[174:2](sel)) >> 0x8)) & 0xfff) */
    /* untranslated: u = (CMaxMines(&sel.pl, idPlayer) - ss:[bp-0x1a]) */
    if ((u <= 0x0))
        goto L_08e9;
    else
        goto L_0820;

L_0820:
    if ((0x3fc >= u))
        goto L_0834;
    else
        goto L_082b;

L_082b:
    t_merge_083a_0001 = 0x3fc;
    t_merge_083a_0002_wide = 0x0;
    t_merge_083a_0003 = 0x3fc;
    goto L_083a;

L_0834:
    t_merge_083a_0001 = u;
    t_merge_083a_0002_wide = 0x0;
    t_merge_083a_0003 = u;

L_083a:
    /* untranslated: LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xfc00) | loword((int32_t)(words((t_merge_083a_0001 & 0x3ff),
     * (loword(t_merge_083a_0002_wide) & 0x0)) << 0x0))) */
    /* untranslated: HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xffff) | hiword((int32_t)(words((t_merge_083a_0003 & 0x3ff),
     * (hiword(t_merge_083a_0002_wide) & 0x0)) << 0x0))) */
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0x3ff) | 0x2000);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfffe) | 0x0);
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfff1) | 0x2);
    cProdGlob = (cProdGlob + 0x1);

L_08e9:
    /* untranslated: ss:[bp-0x1a] = sel.pl.cDefenses */
    /* untranslated: u = (CMaxDefenses(&sel.pl, idPlayer) - ss:[bp-0x1a]) */
    if ((u <= 0x0))
        goto L_09c4;
    else
        goto L_091f;

L_091f:
    /* untranslated: LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xfc00) | loword((int32_t)(words((u & 0x3ff), 0x0) << 0x0))) */
    /* untranslated: HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xffff) | hiword((int32_t)(words((u & 0x3ff), 0x0) << 0x0))) */
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0x3ff) | 0x2400);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfffe) | 0x0);
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfff1) | 0x2);
    cProdGlob = (cProdGlob + 0x1);

L_09c4:
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xfc00) | 0x3ff);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0x3ff) | 0x2c00);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfffe) | 0x0);
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfff1) | 0x2);
    cProdGlob = (cProdGlob + 0x1);
    /* untranslated: branch (loword((uint32_t)(words(part[176:2](sel), part[178:2](sel)) >> 0xc)) & 0x1f) != 0x1f ? L_0b2e : L_0a73 */

L_0a73:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_0b2e : L_0a7b */

L_0a7b:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) == raMacintosh))
        goto L_0b2e;
    else
        goto L_0a9c;

L_0a9c:
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xfc00) | 0x1);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0x3ff) | 0x6c00);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfffe) | 0x0);
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfff1) | 0x2);
    cProdGlob = (cProdGlob + 0x1);

L_0b2e:
    i = IpctCanTerraformLppl(&(sel.pl));
    if ((i <= 0x0))
        goto L_0bfe;
    else
        goto L_0b49;

L_0b49:
    /* untranslated: LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xfc00) | loword((int32_t)(words((i & 0x3ff), (0x0 & 0x0)) << 0x0))) */
    /* untranslated: HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xffff) | hiword((int32_t)(words((i & 0x3ff), (0x0 & 0x0)) << 0x0))) */
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0x3ff) | 0x3000);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfffe) | 0x0);
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfff1) | 0x2);
    cProdGlob = (cProdGlob + 0x1);

L_0bfe:
    i = 0x0;
    goto L_0d32;

L_0c06:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_0c45;
    else
        goto L_0c27;

L_0c27:
    if ((i == 0x0))
        goto L_0d2e;
    else
        goto L_0c30;

L_0c30:
    if ((i == 0x1))
        goto L_0d2e;
    else
        goto L_0c39;

L_0c39:
    if ((i == 0x2))
        goto L_0d2e;
    else
        goto L_0c3f;

L_0c3f:

L_0c45:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raTerra))
        goto L_0c7b;
    else
        goto L_0c66;

L_0c66:
    if ((i == 0x4))
        goto L_0d2e;
    else
        goto L_0c6f;

L_0c6f:
    if ((i == 0x5))
        goto L_0d2e;
    else
        goto L_0c75;

L_0c75:

L_0c7b:
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xfc00) | 0x3ff);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    /* untranslated: LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0x3ff) | loword((int32_t)(words((i & 0x7f), (signhiword(i) & 0x0)) << 0xa))) */
    /* untranslated: HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfffe) | hiword((int32_t)(words((i & 0x7f), (signhiword(i) & 0x0)) << 0xa))) */
    LOWORD(rgprod[cProdGlob]) = ((LOWORD(rgprod[cProdGlob]) & 0xffff) | 0x0);
    HIWORD(rgprod[cProdGlob]) = ((HIWORD(rgprod[cProdGlob]) & 0xfff1) | 0x2);
    cProdGlob = (cProdGlob + 0x1);

L_0d2e:
    i = (i + 0x1);

L_0d32:
    if ((i < 0x7))
        goto L_0c06;
    else
        goto L_0d3b;

L_0d3b:
    ipl = 0x0;
    lpprod = &(lpplProdGlob->rgprod);
    goto L_0d6b;

L_0d58:
    ipl = (ipl + 0x1);
    lpprod = (lpprod + 0x1);

L_0d6b:
    if ((ipl >= lpplProdGlob->iprodMac))
        goto L_1067;
    else
        goto L_0d7e;

L_0d7e:
    iSrc = 0x0;
    goto L_0e49;

L_0d86:
    if (((LOWORD((uint32_t)((pProdGlob[iSrc] >> 0x11))) & 0x7) != lpprod->grobj))
        goto L_0e45;
    else
        goto L_0ddd;

L_0ddd:
    /* untranslated: branch ss:[bp-0x1e] != (hiword(callresult(uint32_t)) & 0x0) ? L_0e45 : L_0de4 */

L_0de4:
    if (((LOWORD((uint32_t)((pProdGlob[iSrc] >> 0xa))) & 0x7f) != lpprod->iItem))
        goto L_0e45;
    else
        goto L_0e3b;

L_0e3b:
    /* untranslated: branch ss:[bp-0x1e] == (hiword(callresult(uint32_t)) & 0x0) ? L_0e54 : L_0e3f */

L_0e3f:

L_0e45:
    iSrc = (iSrc + 0x1);

L_0e49:
    if ((iSrc < cProdGlob))
        goto L_0d86;
    else
        goto L_0e54;

L_0e54:
    if ((iSrc < cProdGlob))
        goto L_0eca;
    else
        goto L_0e5f;

L_0e5f:
    if (((ipl + 0x1) >= lpplProdGlob->iprodMac))
        goto L_0ebe;
    else
        goto L_0e7f;

L_0e7f:
    fmemcpy(&(lpprod), &(lpprod[0x1]), ((lpplProdGlob->iprodMac - (ipl + 0x1)) * 0x4));
    ipl = (ipl - 0x1);

L_0ebe:
    lpplProdGlob->iprodMac = (lpplProdGlob->iprodMac - 0x1);
    goto L_0d58;

L_0eca:
    if (((HIWORD(pProdGlob[iSrc]) & 0x0) > (*(lpprod + 0x2) & 0x0)))
        goto L_0f5b;
    else
        goto L_0efa;

L_0efa:
    if (((HIWORD(pProdGlob[iSrc]) & 0x0) < (*(lpprod + 0x2) & 0x0)))
        goto L_0f06;
    else
        goto L_0eff;

L_0eff:
    if (((LOWORD(pProdGlob[iSrc]) & 0x3ff) >= lpprod->cItem))
        goto L_0f5b;
    else
        goto L_0f06;

L_0f06:
    /* untranslated: LOWORD(lpprod) = ((LOWORD(lpprod) & 0xfc00) | loword((int32_t)(words(((LOWORD(pProdGlob[iSrc]) & 0x3ff) & 0x3ff), ((HIWORD(pProdGlob[iSrc])
     * & 0x0) & 0x0)) << 0x0))) */
    /* untranslated: *(lpprod+0x2) = ((*(lpprod+0x2) & 0xffff) | hiword((int32_t)(words((ss:[bp-0x1c] & 0x3ff), (ss:[bp-0x1a] & 0x0)) << 0x0))) */

L_0f5b:
    if (((LOWORD(pProdGlob[iSrc]) & 0x3ff) != 0x3ff))
        goto L_0f83;
    else
        goto L_0f7b;

L_0f7b:
    if (((HIWORD(pProdGlob[iSrc]) & 0x0) == 0x0))
        goto L_0d58;
    else
        goto L_0f83;

L_0f83:
    if (((HIWORD(pProdGlob[iSrc]) & 0x0) > (*(lpprod + 0x2) & 0x0)))
        goto L_0ff2;
    else
        goto L_0fb3;

L_0fb3:
    if (((HIWORD(pProdGlob[iSrc]) & 0x0) < (*(lpprod + 0x2) & 0x0)))
        goto L_0fbf;
    else
        goto L_0fb8;

L_0fb8:
    if (((LOWORD(pProdGlob[iSrc]) & 0x3ff) >= lpprod->cItem))
        goto L_0ff2;
    else
        goto L_0fbf;

L_0fbf:
    LOWORD(pProdGlob[iSrc]) = ((LOWORD(pProdGlob[iSrc]) & 0xfc00) | 0x0);
    HIWORD(pProdGlob[iSrc]) = ((HIWORD(pProdGlob[iSrc]) & 0xffff) | 0x0);
    goto L_0d58;

L_0ff2:
    /* untranslated: ss:[bp-0x1c] = ((LOWORD(pProdGlob[iSrc]) - loword((int32_t)(words(lpprod->cItem, (*(lpprod+0x2) & 0x0)) << 0x0))) & 0x3ff) */
    /* untranslated: ss:[bp-0x1a] = ((HIWORD(pProdGlob[iSrc]) - hiword((int32_t)(words(lpprod->cItem, (*(lpprod+0x2) & 0x0)) << 0x0))) & 0x0) */
    LOWORD(pProdGlob[iSrc]) = (LOWORD(pProdGlob[iSrc]) & 0xfc00);
    HIWORD(pProdGlob[iSrc]) = (HIWORD(pProdGlob[iSrc]) & 0xffff);
    /* untranslated: LOWORD(pProdGlob[iSrc]) = (LOWORD(pProdGlob[iSrc]) | ss:[bp-0x1c]) */
    /* untranslated: HIWORD(pProdGlob[iSrc]) = (HIWORD(pProdGlob[iSrc]) | ss:[bp-0x1a]) */

L_1067:
    if ((gd.fTutorial == 0x0))
        goto L_1089;
    else
        goto L_107a;

L_107a:
    if ((idPlayer != 0x0))
        goto L_1089;
    else
        goto L_1084;

L_1084:
    AdvanceTutor();

L_1089:
    return;
}

void FinishProduction(int16_t fWrite) {
L_1090:
    if ((fWrite == 0x0))
        goto L_1168;
    else
        goto L_10a2;

L_10a2:
    FreePl(sel.pl.lpplprod);
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_10c6;
    else
        goto L_10bc;

L_10bc:
    if ((HIWORD(lpplProdGlob) == 0x0))
        goto L_10f5;
    else
        goto L_10c6;

L_10c6:
    if ((lpplProdGlob->iprodMac != 0x0))
        goto L_10f5;
    else
        goto L_10d9;

L_10d9:
    FreePl(lpplProdGlob);
    lpplProdGlob = 0x0;

L_10f5:
    sel.pl.lpplprod = lpplProdGlob;
    lpplProdGlob = 0x0;
    FLookupPlanet(0xffff, 0x49ee);
    FLookupPlanet(sel.pl.id, 0x49ee);
    if ((fAi != 0x0))
        goto L_11c4;
    else
        goto L_1139;

L_1139:
    FillPlanetProdLB(0x0, 0x0, 0x0);
    DrawPlanShip(0x0, 0x40);

L_1168:
    /* untranslated: part[176:2](sel) = ((part[176:2](sel) & 0xffff) | loword((int32_t)(words((gd.fNoResearchSav & 0x1), (0x0 & 0x0)) << 0x17))) */
    /* untranslated: part[178:2](sel) = ((part[178:2](sel) & 0xff7f) | hiword((int32_t)(words((ss:[bp-0x6] & 0x1), (ss:[bp-0x4] & 0x0)) << 0x17))) */
    FreePl(lpplProdGlob);

L_11c4:
    lpplProdGlob = 0x0;
    if ((gd.fTutorial == 0x0))
        goto L_11fe;
    else
        goto L_11e3;

L_11e3:
    if ((idPlayer != 0x0))
        goto L_11fe;
    else
        goto L_11ed;

L_11ed:
    tutor.fProgress = 0x1;
    AdvanceTutor();

L_11fe:
    return;
}

int16_t ProductionDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    HDC                hdc;
    PAINTSTRUCT        ps;
    RECT               rc;
    int16_t            dxPBtn;
    int16_t            dy;
    RECT               rcT;
    int16_t            i;
    int16_t            xCtr;
    int16_t            dx;
    int16_t            dyLB;
    int16_t            rgidProdBtns[10];
    DRAWITEMSTRUCT    *lpdis;
    MEASUREITEMSTRUCT *lpmis;
    POINT              pt;
    int16_t            cMax;
    char               sz255[2];
    char              *rgszZip[6];
    ZIPPRODQ           rgzp[4];
    int16_t (**lpProc)();
    int16_t  fRet;
    uint16_t hcs;
    int16_t  t_184a;

L_1204:
    goto L_1933;

L_1213:
    LOWORD(rgidProdBtns) = 0x42e;
    HIWORD(rgidProdBtns) = 0x42f;
    /* untranslated: part[4:2](rgidProdBtns) = 0x1 */
    /* untranslated: part[6:2](rgidProdBtns) = 0x2 */
    /* untranslated: part[8:2](rgidProdBtns) = 0x418 */
    /* untranslated: part[10:2](rgidProdBtns) = 0x419 */
    /* untranslated: part[12:2](rgidProdBtns) = 0x439 */
    /* untranslated: part[14:2](rgidProdBtns) = 0x43a */
    /* untranslated: part[16:2](rgidProdBtns) = 0x42d */
    /* untranslated: part[18:2](rgidProdBtns) = 0x76 */
    hwndProdDlg = hwnd;
    if ((rgplr[idPlayer].cPlanet > 0x1))
        goto L_128c;
    else
        goto L_1260;

L_1260:
    EnableWindow(GetDlgItem(hwnd, IDC_NEXT), 0x0);
    EnableWindow(GetDlgItem(hwnd, IDC_U16_0x042E), 0x0);

L_128c:
    if ((gd.mdScreenSize < 0x1))
        goto L_12ac;
    else
        goto L_129f;

L_129f:
    dx = 0x2f8;
    dy = 0x244;
    goto L_12be;

L_12ac:
    dx = 0x262;
    dy = (LOWORD((0x18 * dyArial8)) + 0x18);

L_12be:
    SetWindowPos(hwnd, 0x0, 0x0, 0x0, dx, dy, 0x6);
    GetClientRect(hwnd, &(rc));
    xCtr = (rc.right >> 0x1);
    /* untranslated: dyLB = ((rc.bottom - (words(loword((0x11 * dyArial8)), signhiword(loword((0x11 * dyArial8)))) / 0x2)) + 0xffe8) */
    /* untranslated: rc.left = ((words(loword((0xb * rc.right)), signhiword(loword((0xb * rc.right)))) / 0x14) + 0x10) */
    dxPBtn = ((uint32_t)((rc.right - rc.left)) / 0x4);
    /* untranslated: rc.bottom = (rc.bottom - ((words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2) + 0x6)) */
    /* untranslated: call SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x008B), 0x0, 0x6, rc.bottom, (rc.left + 0xfff4), (words(loword((0x3 * dyArial8)),
     * signhiword(loword((0x3 * dyArial8)))) / 0x2), SWP_NOZORDER) -> callresult(int16_t) */
    i = 0x0;
    goto L_13c4;

L_137d:
    /* untranslated: call SetWindowPos(GetDlgItem(hwnd, rgidProdBtns[i]), 0x0, rc.left, rc.bottom, (dxPBtn + 0xfffa), (words(loword((0x3 * dyArial8)),
     * signhiword(loword((0x3 * dyArial8)))) / 0x2), SWP_NOZORDER) -> callresult(int16_t) */
    rc.left = (rc.left + dxPBtn);
    i = (i + 0x1);

L_13c4:
    if ((i < 0x4))
        goto L_137d;
    else
        goto L_13cd;

L_13cd:
    dxPBtn = (dxPBtn + 0x18);
    rc.left = (xCtr - ((dxPBtn + 0xfffa) >> 0x1));
    /* untranslated: dy = (((sext16to32((dyLB - loword((0x9 * dyArial8)))) / 0x5) + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) /
     * 0x2)) + 0xfffd) */
    rc.top = 0x8;
    if ((dy <= 0x32))
        goto L_1437;
    else
        goto L_141e;

L_141e:
    /* untranslated: rc.top = (rc.top + (words(loword(((dy + 0xffce) * 0x5)), signhiword(loword(((dy + 0xffce) * 0x5)))) / 0x2)) */
    dy = 0x32;

L_1437:
    i = 0x4;
    goto L_1486;

L_143f:
    /* untranslated: call SetWindowPos(GetDlgItem(hwnd, rgidProdBtns[i]), 0x0, rc.left, rc.top, (dxPBtn + 0xfffa), (words(loword((0x3 * dyArial8)),
     * signhiword(loword((0x3 * dyArial8)))) / 0x2), SWP_NOZORDER) -> callresult(int16_t) */
    rc.top = (rc.top + dy);
    i = (i + 0x1);

L_1486:
    if ((i < 0xa))
        goto L_143f;
    else
        goto L_148f;

L_148f:
    SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x0416), 0x0, 0x6, 0x6, (rc.left + 0xfff4), dyLB, SWP_NOZORDER);
    GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0416), &(rcT));
    SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x0417), 0x0, (rc.left + dxPBtn), 0x6, (rc.left + 0xfff4), (rcT.bottom - rcT.top), SWP_NOZORDER);
    ScreenToClient(hwnd, &(rcT.right));
    yTopFutureTech = rcT.bottom;
    InitializeProductionDlg(hwnd);
    if ((gd.mdScreenSize != 0x1))
        goto L_154a;
    else
        goto L_153a;

L_153a:
    if ((ptStickyProduceDlg.y != 0xffff))
        goto L_154a;
    else
        goto L_1544;

L_1544:
    ptStickyProduceDlg.y = 0x0;

L_154a:
    StickyDlgPos(hwnd, 0xcd6, 0x1);
    return 0x1;

L_1563:
    lpdis = lParam;
    if ((lpdis->itemID != 0xffff))
        goto L_1591;
    else
        goto L_157c;

L_157c:
    HandleFocusState(lpdis, 0xfffe);
    goto L_15ce;

L_1591:
    goto L_15b3;

L_159b:
    DrawCBEntireItem(lpdis, 0x4);
    goto L_15ce;

L_15b3:
    if ((lpdis->itemAction == 0x1))
        goto L_159b;
    else
        goto L_15bb;

L_15bb:
    if ((lpdis->itemAction == 0x2))
        goto L_159b;
    else
        goto L_15c3;

L_15c3:
    if ((lpdis->itemAction == 0x4))
        goto L_159b;
    else
        goto L_15ce;

L_15ce:
    return 0x1;

L_15d4:
    lpmis = lParam;
    lpmis->itemHeight = (dyArial8 + 0x2);
    return 0x1;

L_15f3:
    /* untranslated: ss:[bp-0x2e] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0x2e] == GetDlgItem(hwnd, IDC_U16_0x008B) ? L_162e : L_1612 */

L_1612:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_1986;
    else
        goto L_162e;

L_162e:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_1647:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_166f:
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    /* untranslated: branch PtInRect(rcProdDiamond, words(pt.y, pt.x)) == 0x0 ? L_1986 : L_16a9 */

L_16a9:
    if ((message != WM_LBUTTONDOWN))
        goto L_16e9;
    else
        goto L_16b3;

L_16b3:
    GlobalPD.grPopup = grPopupString;
    HIWORD(GlobalPD) = 0xb4;
    /* untranslated: part[4:2](GlobalPD) = 0x4a6c */
    CchGetString(idsRightClickBlueDiamondApplyProductionTemplate, 0x4a6c);
    Popup(hwnd, pt.x, pt.y);
    goto L_1986;

L_16e9:
    /* untranslated: part[0:1](sz255) = 0xff */
    /* untranslated: part[1:1](sz255) = 0x0 */
    cMax = 0x0;
    i = 0x0;
    goto L_1736;

L_16fe:
    if ((vrgZipProd[i].fValid == 0x0))
        goto L_1732;
    else
        goto L_1717;

L_1717:
    cMax = (cMax + 0x1);
    rgszZip[cMax] = vrgZipProd[i];

L_1732:
    i = (i + 0x1);

L_1736:
    if ((i < 0x4))
        goto L_16fe;
    else
        goto L_173f;

L_173f:
    cMax = (cMax + 0x1);
    rgszZip[cMax] = sz255;
    cMax = (cMax + 0x1);
    rgszZip[cMax] = PszGetCompressedString(idsCustomize);
    i = PopupMenu(hwnd, pt.x, pt.y, cMax, 0x0, rgszZip, 0xffff, 0x1);
    if ((i != (cMax + 0xffff)))
        goto L_1820;
    else
        goto L_17a3;

L_17a3:
    memcpy(rgzp, 0x22f6, 0xa0);
    lpProc = MakeProcInstance(ZipProdDlg, hInst);
    fRet = DialogBox(hInst, IDD_ZIP_PROD, hwnd, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((fRet != 0x0))
        goto L_1986;
    else
        goto L_1808;

L_1808:
    memcpy(0x22f6, rgzp, 0xa0);

L_1820:
    if ((i < 0x0))
        goto L_1986;
    else
        goto L_1829;

L_1829:
    cMax = 0x0;
    goto L_1860;

L_1831:
    if ((vrgZipProd[cMax].fValid == 0x0))
        goto L_185c;
    else
        goto L_184a;

L_184a:
    t_184a = i;
    i = (i - 0x1);
    if ((t_184a == 0x0))
        goto L_1869;
    else
        goto L_1856;

L_1856:

L_185c:
    cMax = (cMax + 0x1);

L_1860:
    if ((cMax < 0x4))
        goto L_1831;
    else
        goto L_1869;

L_1869:
    ProdCommandHandler(hwnd, 0x816, (uint32_t)(cMax));

L_1881:
    hcs = 0x0;
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    /* untranslated: branch PtInRect(rcProdDiamond, words(pt.y, pt.x)) == 0x0 ? L_1986 : L_18bb */

L_18bb:
    SetCursor(hcurArrowHelp);
    return 0x1;

L_18cd:
    hdc = BeginPaint(hwnd, &(ps));
    GetClientRect(hwnd, &(rc));
    DrawProductionDlg(hwnd, hdc, &(rc), 0xffff);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_1919:
    ProdCommandHandler(hwnd, wParam, lParam);
    goto L_1986;

L_1933:
    if ((message == WM_PAINT))
        goto L_18cd;
    else
        goto L_193b;

L_193b:
    if ((message == WM_ERASEBKGND))
        goto L_1647;
    else
        goto L_1943;

L_1943:
    if ((message == WM_CTLCOLOR))
        goto L_15f3;
    else
        goto L_194b;

L_194b:
    if ((message == WM_SETCURSOR))
        goto L_1881;
    else
        goto L_1953;

L_1953:
    if ((message == WM_DRAWITEM))
        goto L_1563;
    else
        goto L_195b;

L_195b:
    if ((message == WM_MEASUREITEM))
        goto L_15d4;
    else
        goto L_1963;

L_1963:
    if ((message == WM_INITDIALOG))
        goto L_1213;
    else
        goto L_196b;

L_196b:
    if ((message == WM_COMMAND))
        goto L_1919;
    else
        goto L_1973;

L_1973:
    if ((message == WM_LBUTTONDOWN))
        goto L_166f;
    else
        goto L_197b;

L_197b:
    if ((message == WM_RBUTTONDOWN))
        goto L_166f;
    else
        goto L_1986;

L_1986:
    return 0x0;
}

void ProdCommandHandler(HWND hwnd, uint16_t wParam, int32_t lParam) {
    int32_t  lSel;
    int16_t  iSrc;
    HWND     hwndLB;
    int16_t  c;
    int16_t  iDst;
    PROD     prodLast;
    int16_t  ipl;
    int16_t  fRefillSrc;
    int16_t  iMac;
    RECT     rc;
    PROD    *lpprod;
    PROD     prod;
    int16_t  cMax;
    PLPROD  *lpplprodT;
    uint16_t t_1a18;
    uint32_t t_merge_1abe_0001;
    uint16_t t_merge_1abe_0002;
    uint32_t t_merge_1abe_0003;
    uint16_t t_merge_1abe_0004;
    uint32_t t_merge_1b2c_0001;
    uint16_t t_merge_1b2c_0002;
    uint32_t t_merge_1b2c_0003;
    uint16_t t_merge_1b2c_0004;
    uint32_t t_merge_1bae_0001;
    uint16_t t_merge_1bae_0002;
    uint32_t t_merge_1bae_0003;
    uint16_t t_merge_1bae_0004;
    uint16_t t_merge_1e76_0001;
    uint16_t t_merge_1e76_0002;
    uint16_t t_merge_1e76_0003;
    uint16_t t_merge_1e76_0004;
    uint16_t t_merge_2363_0001;
    int16_t  t_merge_2432_0001;
    uint16_t t_merge_2f8a_0001;
    uint16_t t_merge_2fa6_0001;
    uint16_t t_merge_2fd9_0001;

L_1994:
    goto L_33b6;

AddItem:
    lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0416), CB_GETLBTEXTLEN, 0x0, 0x0);
    if ((HIWORD(lSel) > 0x0))
        goto L_19e5;
    else
        goto L_19d4;

L_19d4:
    if ((HIWORD(lSel) < 0x0))
        goto L_3429;
    else
        goto L_19d9;

L_19d9:
    if ((LOWORD(lSel) < 0x0))
        goto L_3429;
    else
        goto L_19df;

L_19df:

L_19e5:
    iSrc = 0x0;
    goto L_1a3d;

L_19ed:
    if (((LOWORD(pProdGlob[iSrc]) & 0x3ff) != 0x0))
        goto L_1a18;
    else
        goto L_1a0d;

L_1a0d:
    if (((HIWORD(pProdGlob[iSrc]) & 0x0) == 0x0))
        goto L_1a39;
    else
        goto L_1a12;

L_1a12:

L_1a18:
    t_1a18 = LOWORD(lSel);
    lSel = (lSel - 0x1);
    if ((t_1a18 != 0x0))
        goto L_1a39;
    else
        goto L_1a2e;

L_1a2e:
    if ((HIWORD(lSel) == 0x0))
        goto L_1a48;
    else
        goto L_1a33;

L_1a33:

L_1a39:
    iSrc = (iSrc + 0x1);

L_1a3d:
    if ((iSrc < cProdGlob))
        goto L_19ed;
    else
        goto L_1a48;

L_1a48:
    prod = pProdGlob[iSrc];
    if (((GetAsyncKeyState(0x11) & 0xfffe) == 0x0))
        goto L_1b64;
    else
        goto L_1a74;

L_1a74:
    if (((GetAsyncKeyState(0x10) & 0xfffe) == 0x0))
        goto L_1af6;
    else
        goto L_1a88;

L_1a88:
    if (((HIWORD(prod) & 0x0) > 0x0))
        goto L_1ab8;
    else
        goto L_1a9c;

L_1a9c:
    if (((HIWORD(prod) & 0x0) < 0x0))
        goto L_1aa9;
    else
        goto L_1aa1;

L_1aa1:
    if ((prod.cItem >= 0x3fc))
        goto L_1ab8;
    else
        goto L_1aa9;

L_1aa9:
    t_merge_1abe_0001 = prod.cItem;
    t_merge_1abe_0002 = (HIWORD(prod) & 0x0);
    t_merge_1abe_0003 = prod.cItem;
    t_merge_1abe_0004 = (HIWORD(prod) & 0x0);
    goto L_1abe;

L_1ab8:
    t_merge_1abe_0001 = 0x3fc;
    t_merge_1abe_0002 = 0x0;
    t_merge_1abe_0003 = 0x3fc;
    t_merge_1abe_0004 = 0x0;

L_1abe:
    /* untranslated: LOWORD(prod) = ((LOWORD(prod) & 0xfc00) | loword((int32_t)(words((t_merge_1abe_0001 & 0x3ff), (t_merge_1abe_0002 & 0x0)) << 0x0))) */
    /* untranslated: HIWORD(prod) = ((HIWORD(prod) & 0xffff) | hiword((int32_t)(words((t_merge_1abe_0003 & 0x3ff), (t_merge_1abe_0004 & 0x0)) << 0x0))) */
    goto L_1bfe;

L_1af6:
    if (((HIWORD(prod) & 0x0) > 0x0))
        goto L_1b26;
    else
        goto L_1b0a;

L_1b0a:
    if (((HIWORD(prod) & 0x0) < 0x0))
        goto L_1b17;
    else
        goto L_1b0f;

L_1b0f:
    if ((prod.cItem >= 0x64))
        goto L_1b26;
    else
        goto L_1b17;

L_1b17:
    t_merge_1b2c_0001 = prod.cItem;
    t_merge_1b2c_0002 = (HIWORD(prod) & 0x0);
    t_merge_1b2c_0003 = prod.cItem;
    t_merge_1b2c_0004 = (HIWORD(prod) & 0x0);
    goto L_1b2c;

L_1b26:
    t_merge_1b2c_0001 = 0x64;
    t_merge_1b2c_0002 = 0x0;
    t_merge_1b2c_0003 = 0x64;
    t_merge_1b2c_0004 = 0x0;

L_1b2c:
    /* untranslated: LOWORD(prod) = ((LOWORD(prod) & 0xfc00) | loword((int32_t)(words((t_merge_1b2c_0001 & 0x3ff), (t_merge_1b2c_0002 & 0x0)) << 0x0))) */
    /* untranslated: HIWORD(prod) = ((HIWORD(prod) & 0xffff) | hiword((int32_t)(words((t_merge_1b2c_0003 & 0x3ff), (t_merge_1b2c_0004 & 0x0)) << 0x0))) */

L_1b64:
    if (((GetAsyncKeyState(0x10) & 0xfffe) == 0x0))
        goto L_1be6;
    else
        goto L_1b78;

L_1b78:
    if (((HIWORD(prod) & 0x0) > 0x0))
        goto L_1ba8;
    else
        goto L_1b8c;

L_1b8c:
    if (((HIWORD(prod) & 0x0) < 0x0))
        goto L_1b99;
    else
        goto L_1b91;

L_1b91:
    if ((prod.cItem >= 0xa))
        goto L_1ba8;
    else
        goto L_1b99;

L_1b99:
    t_merge_1bae_0001 = prod.cItem;
    t_merge_1bae_0002 = (HIWORD(prod) & 0x0);
    t_merge_1bae_0003 = prod.cItem;
    t_merge_1bae_0004 = (HIWORD(prod) & 0x0);
    goto L_1bae;

L_1ba8:
    t_merge_1bae_0001 = 0xa;
    t_merge_1bae_0002 = 0x0;
    t_merge_1bae_0003 = 0xa;
    t_merge_1bae_0004 = 0x0;

L_1bae:
    /* untranslated: LOWORD(prod) = ((LOWORD(prod) & 0xfc00) | loword((int32_t)(words((t_merge_1bae_0001 & 0x3ff), (t_merge_1bae_0002 & 0x0)) << 0x0))) */
    /* untranslated: HIWORD(prod) = ((HIWORD(prod) & 0xffff) | hiword((int32_t)(words((t_merge_1bae_0003 & 0x3ff), (t_merge_1bae_0004 & 0x0)) << 0x0))) */
    goto L_1bfe;

L_1be6:
    prod.cItem = 0x1;
    HIWORD(prod) = ((HIWORD(prod) & 0xffff) | 0x0);

L_1bfe:
    if (((LOWORD(pProdGlob[iSrc]) & 0x3ff) != 0x3ff))
        goto L_1c26;
    else
        goto L_1c1e;

L_1c1e:
    if (((HIWORD(pProdGlob[iSrc]) & 0x0) == 0x0))
        goto L_1c94;
    else
        goto L_1c26;

L_1c26:
    /* untranslated: ss:[bp-0x2c] = ((LOWORD(pProdGlob[iSrc]) - loword((int32_t)(words(prod.cItem, (HIWORD(prod) & 0x0)) << 0x0))) & 0x3ff) */
    /* untranslated: ss:[bp-0x2a] = ((HIWORD(pProdGlob[iSrc]) - hiword((int32_t)(words(prod.cItem, (HIWORD(prod) & 0x0)) << 0x0))) & 0x0) */
    LOWORD(pProdGlob[iSrc]) = (LOWORD(pProdGlob[iSrc]) & 0xfc00);
    HIWORD(pProdGlob[iSrc]) = (HIWORD(pProdGlob[iSrc]) & 0xffff);
    /* untranslated: LOWORD(pProdGlob[iSrc]) = (LOWORD(pProdGlob[iSrc]) | ss:[bp-0x2c]) */
    /* untranslated: HIWORD(pProdGlob[iSrc]) = (HIWORD(pProdGlob[iSrc]) | ss:[bp-0x2a]) */

L_1c94:
    iMac = lpplProdGlob->iprodMac;
    lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), CB_GETLBTEXTLEN, 0x0, 0x0);
    if ((HIWORD(lSel) > 0x0))
        goto L_1cfb;
    else
        goto L_1cd3;

L_1cd3:
    if ((HIWORD(lSel) < 0x0))
        goto L_1ce1;
    else
        goto L_1cd8;

L_1cd8:
    if ((LOWORD(lSel) >= 0x0))
        goto L_1cfb;
    else
        goto L_1ce1;

L_1ce1:
    iDst = (iMac + 0xffff);
    if ((iDst >= 0x0))
        goto L_1d07;
    else
        goto L_1cf3;

L_1cf3:
    iDst = 0x0;

L_1cfb:
    iDst = (LOWORD(lSel) + 0xffff);

L_1d07:
    if ((iDst >= iMac))
        goto L_2099;
    else
        goto L_1d12;

L_1d12:
    if ((iDst < 0x0))
        goto L_1fde;
    else
        goto L_1d1b;

L_1d1b:
    prodLast = lpplProdGlob[iDst].rgprod[0x0];
    if ((prodLast.iItem != prod.iItem))
        goto L_1fde;
    else
        goto L_1d7b;

L_1d7b:
    /* untranslated: branch ss:[bp-0x2a] != (hiword(callresult(uint32_t)) & 0x0) ? L_1fde : L_1d82 */

L_1d82:
    if ((prodLast.grobj != prod.grobj))
        goto L_1fde;
    else
        goto L_1dbd;

L_1dbd:
    /* untranslated: branch ss:[bp-0x2a] != (hiword(callresult(uint32_t)) & 0x0) ? L_1fde : L_1dc4 */

RingItUp:
    if ((0x0 > (0x0 + (HIWORD(prod) & 0x0))))
        goto L_1e2e;
    else
        goto L_1e19;

L_1e19:
    /* untranslated: branch 0x0 < (0x0 + ss:[bp-0x2e]) ? L_1e25 : L_1e1e */

L_1e1e:
    /* untranslated: branch 0x3fc >= ((LOWORD(lpplProdGlob[iDst].rgprod[0x0]) & 0x3ff) + ss:[bp-0x30]) ? L_1e2e : L_1e25 */

L_1e25:
    t_merge_1e76_0001 = 0x3fc;
    t_merge_1e76_0002 = 0x0;
    t_merge_1e76_0003 = 0x3fc;
    t_merge_1e76_0004 = 0x0;
    goto L_1e76;

L_1e2e:
    /* untranslated: ss:[bp-0x34] = prod.cItem */
    /* untranslated: ss:[bp-0x32] = (HIWORD(prod) & 0x0) */
    /* untranslated: t_merge_1e76_0001 = ((LOWORD(lpplProdGlob[iDst].rgprod[0x0]) & 0x3ff) + ss:[bp-0x34]) */
    /* untranslated: t_merge_1e76_0002 = (0x0 + ss:[bp-0x32]) */
    /* untranslated: t_merge_1e76_0003 = ((LOWORD(lpplProdGlob[iDst].rgprod[0x0]) & 0x3ff) + ss:[bp-0x34]) */
    /* untranslated: t_merge_1e76_0004 = (0x0 + ss:[bp-0x32]) */

L_1e76:
    /* untranslated: LOWORD(lpplProdGlob[iDst].rgprod[0x0]) = ((LOWORD(lpplProdGlob[iDst].rgprod[0x0]) & 0xfc00) | loword((int32_t)(words((t_merge_1e76_0001 &
     * 0x3ff), (t_merge_1e76_0002 & 0x0)) << 0x0))) */
    /* untranslated: HIWORD(lpplProdGlob[iDst].rgprod[0x0]) = ((HIWORD(lpplProdGlob[iDst].rgprod[0x0]) & 0xffff) | hiword((int32_t)(words((t_merge_1e76_0003 &
     * 0x3ff), (t_merge_1e76_0004 & 0x0)) << 0x0))) */
    if (((HIWORD(lpplProdGlob[iDst].rgprod[0x0]) & 0x0) < 0x0))
        goto FixedUp;
    else
        goto L_1f0a;

L_1f0a:
    if (((HIWORD(lpplProdGlob[iDst].rgprod[0x0]) & 0x0) > 0x0))
        goto L_1f17;
    else
        goto L_1f0f;

L_1f0f:
    if (((LOWORD(lpplProdGlob[iDst].rgprod[0x0]) & 0x3ff) <= 0x1))
        goto FixedUp;
    else
        goto L_1f17;

L_1f17:
    if (((LOWORD((uint32_t)((lpplProdGlob[iDst].rgprod[0x0] >> 0xa))) & 0x7f) != 0x3))
        goto FixedUp;
    else
        goto L_1f4c;

L_1f4c:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_215e : L_1f54 */

L_1f54:
    if (((LOWORD((uint32_t)((lpplProdGlob[iDst].rgprod[0x0] >> 0x11))) & 0x7) != 0x1))
        goto FixedUp;
    else
        goto L_1f89;

L_1f89:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_215e : L_1f91 */

L_1f91:
    LOWORD(lpplProdGlob[iDst].rgprod[0x0]) = ((LOWORD(lpplProdGlob[iDst].rgprod[0x0]) & 0xfc00) | 0x1);
    HIWORD(lpplProdGlob[iDst].rgprod[0x0]) = ((HIWORD(lpplProdGlob[iDst].rgprod[0x0]) & 0xffff) | 0x0);

L_1fde:
    iDst = (iDst + 0x1);
    if ((iDst >= iMac))
        goto L_2099;
    else
        goto L_1fed;

L_1fed:
    prodLast = lpplProdGlob[iDst].rgprod[0x0];
    if ((prodLast.iItem != prod.iItem))
        goto L_2099;
    else
        goto L_204d;

L_204d:
    /* untranslated: branch ss:[bp-0x2a] != (hiword(callresult(uint32_t)) & 0x0) ? L_2099 : L_2054 */

L_2054:
    if ((prodLast.grobj != prod.grobj))
        goto L_2099;
    else
        goto L_208f;

L_208f:
    /* untranslated: branch ss:[bp-0x2a] == (hiword(callresult(uint32_t)) & 0x0) ? L_1dc4 : L_2093 */

L_2093:

L_2099:
    if ((iMac < 0x28))
        goto L_20ae;
    else
        goto L_20a2;

L_20a2:
    MessageBeep(0x0);
    goto RedrawText;

L_20ae:
    if ((iMac != lpplProdGlob->iprodMax))
        goto L_20df;
    else
        goto L_20c1;

L_20c1:
    lpplProdGlob = LpplReAlloc(lpplProdGlob, (iMac + 0x4));

L_20df:
    if ((iDst == iMac))
        goto L_2130;
    else
        goto L_20ea;

L_20ea:
    fmemmove(&(lpplProdGlob[(iDst + 0x1)].rgprod), &(lpplProdGlob[iDst].rgprod), ((iMac - iDst) * 0x4));

L_2130:
    lpplProdGlob[iDst].rgprod[0x0] = prod;
    lpplProdGlob->iprodMac = (lpplProdGlob->iprodMac + 0x1);

FixedUp:
    FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), CB_GETCURSEL, (iDst + 0x1), 0x0);
    if (((LOWORD(pProdGlob[iSrc]) & 0x3ff) != 0x0))
        goto RedrawText;
    else
        goto L_21c8;

L_21c8:
    if (((HIWORD(pProdGlob[iSrc]) & 0x0) != 0x0))
        goto RedrawText;
    else
        goto L_21d0;

L_21d0:
    FillProdSrcLB(GetDlgItem(hwnd, IDC_U16_0x0416), 0xffff);

RemoveItem:
    lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), CB_GETLBTEXTLEN, 0x0, 0x0);
    if ((HIWORD(lSel) > 0x0))
        goto L_222e;
    else
        goto L_221d;

L_221d:
    if ((HIWORD(lSel) < 0x0))
        goto L_3429;
    else
        goto L_2222;

L_2222:
    if ((LOWORD(lSel) <= 0x0))
        goto L_3429;
    else
        goto L_2228;

L_2228:

L_222e:
    iMac = lpplProdGlob->iprodMac;
    lSel = (lSel - 0x1);
    /* untranslated: LOWORD(prod) = HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))] */
    /* untranslated: HIWORD(prod) = HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))+0x2] */
    iSrc = 0x0;
    goto L_2327;

L_2284:
    if (((LOWORD((uint32_t)((pProdGlob[iSrc] >> 0x11))) & 0x7) != prod.grobj))
        goto L_2323;
    else
        goto L_22cb;

L_22cb:
    /* untranslated: branch ss:[bp-0x2a] != (hiword(callresult(uint32_t)) & 0x0) ? L_2323 : L_22d2 */

L_22d2:
    if (((LOWORD((uint32_t)((pProdGlob[iSrc] >> 0xa))) & 0x7f) != prod.iItem))
        goto L_2323;
    else
        goto L_2319;

L_2319:
    /* untranslated: branch ss:[bp-0x2a] == (hiword(callresult(uint32_t)) & 0x0) ? L_2332 : L_231d */

L_231d:

L_2323:
    iSrc = (iSrc + 0x1);

L_2327:
    if ((iSrc < cProdGlob))
        goto L_2284;
    else
        goto L_2332;

L_2332:
    if (((LOWORD(pProdGlob[iSrc]) & 0x3ff) != 0x0))
        goto L_2360;
    else
        goto L_2352;

L_2352:
    if (((HIWORD(pProdGlob[iSrc]) & 0x0) != 0x0))
        goto L_2360;
    else
        goto L_235a;

L_235a:
    t_merge_2363_0001 = 0x1;
    goto L_2363;

L_2360:
    t_merge_2363_0001 = 0x0;

L_2363:
    fRefillSrc = t_merge_2363_0001;
    if (((GetAsyncKeyState(0x11) & 0xfffe) == 0x0))
        goto L_239e;
    else
        goto L_237a;

L_237a:
    if (((GetAsyncKeyState(0x10) & 0xfffe) == 0x0))
        goto L_2396;
    else
        goto L_238e;

L_238e:
    c = 0x3fc;
    goto L_23bf;

L_2396:
    c = 0x64;

L_239e:
    if (((GetAsyncKeyState(0x10) & 0xfffe) == 0x0))
        goto L_23ba;
    else
        goto L_23b2;

L_23b2:
    c = 0xa;
    goto L_23bf;

L_23ba:
    c = 0x1;

L_23bf:
    if ((prod.grobj != 0x1))
        goto L_240c;
    else
        goto L_23db;

L_23db:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_240c : L_23e3 */

L_23e3:
    if ((prod.iItem != 0x3))
        goto L_240c;
    else
        goto L_23ff;

L_23ff:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_240c : L_2407 */

L_2407:
    c = 0x3fc;

L_240c:
    if ((c >= prod.cItem))
        goto L_2426;
    else
        goto L_2420;

L_2420:
    t_merge_2432_0001 = c;
    goto L_2432;

L_2426:
    t_merge_2432_0001 = prod.cItem;

L_2432:
    c = t_merge_2432_0001;
    if (((LOWORD(pProdGlob[iSrc]) & 0x3ff) != 0x3ff))
        goto L_245d;
    else
        goto L_2455;

L_2455:
    if (((HIWORD(pProdGlob[iSrc]) & 0x0) == 0x0))
        goto L_24b6;
    else
        goto L_245d;

L_245d:
    /* untranslated: ss:[bp-0x2c] = ((c + LOWORD(pProdGlob[iSrc])) & 0x3ff) */
    /* untranslated: ss:[bp-0x2a] = ((signhiword(c) + HIWORD(pProdGlob[iSrc])) & 0x0) */
    LOWORD(pProdGlob[iSrc]) = (LOWORD(pProdGlob[iSrc]) & 0xfc00);
    HIWORD(pProdGlob[iSrc]) = (HIWORD(pProdGlob[iSrc]) & 0xffff);
    /* untranslated: LOWORD(pProdGlob[iSrc]) = (LOWORD(pProdGlob[iSrc]) | ss:[bp-0x2c]) */
    /* untranslated: HIWORD(pProdGlob[iSrc]) = (HIWORD(pProdGlob[iSrc]) | ss:[bp-0x2a]) */

L_24b6:
    /* untranslated: ss:[bp-0x2c] = ((HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))] - c) & 0x3ff) */
    /* untranslated: ss:[bp-0x2a] = ((HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))+0x2] - 0x0) & 0x0) */
    /* untranslated: HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))] = (HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) +
     * 0x4) + loword((int32_t)(lSel * 0x4)))] & 0xfc00) */
    /* untranslated: ss:[bp-0x36]:[(ss:[bp-0x38] + loword((int32_t)(lSel * 0x4)))+0x2] = (ss:[bp-0x36]:[(ss:[bp-0x38] + loword((int32_t)(lSel * 0x4)))+0x2] &
     * 0xffff) */
    /* untranslated: HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))] = (HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) +
     * 0x4) + loword((int32_t)(lSel * 0x4)))] | ss:[bp-0x2c]) */
    /* untranslated: ss:[bp-0x3a]:[(ss:[bp-0x3c] + loword((int32_t)(lSel * 0x4)))+0x2] = (ss:[bp-0x3a]:[(ss:[bp-0x3c] + loword((int32_t)(lSel * 0x4)))+0x2] |
     * ss:[bp-0x2a]) */
    /* untranslated: branch (HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))] & 0x3ff) != 0x0 ? L_2689 : L_25c5 */

L_25c5:
    /* untranslated: branch (ss:[bp-0x2a]:[(ss:[bp-0x2c] + loword(callresult(int32_t)))+0x2] & 0x0) != 0x0 ? L_2689 : L_25cd */

L_25cd:
    if (((HIWORD(lSel) + 0x0) > SIGNHIWORD(iMac)))
        goto L_2678;
    else
        goto L_25f0;

L_25f0:
    /* untranslated: branch ss:[bp-0x2a] < signhiword(iMac) ? L_25fc : L_25f5 */

L_25f5:
    /* untranslated: branch ss:[bp-0x2c] >= iMac ? L_2678 : L_25fc */

L_25fc:
    /* untranslated: call fmemmove(ptroff(lpplProdGlob, (0x4 + loword((int32_t)(lSel * 0x4)))), ptroff(lpplProdGlob, (0x4 + loword((int32_t)(words((LOWORD(lSel)
     * + 0x1), (HIWORD(lSel) + 0x0)) * 0x4)))), (((iMac - LOWORD(lSel)) + 0xffff) * 0x4)) -> callresult(void *) */
    goto L_2680;

L_2678:
    lSel = (lSel - 0x1);

L_2680:
    lpplProdGlob->iprodMac = (lpplProdGlob->iprodMac - 0x1);

L_2689:
    FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, 0x0);
    if ((HIWORD(lSel) < 0x0))
        goto L_26ed;
    else
        goto L_26b7;

L_26b7:
    if ((HIWORD(lSel) > 0x0))
        goto L_26c5;
    else
        goto L_26bc;

L_26bc:
    if ((LOWORD(lSel) < 0x0))
        goto L_26ed;
    else
        goto L_26c5;

L_26c5:
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), CB_GETCURSEL, (LOWORD(lSel) + 0x1), 0x0);

L_26ed:
    if ((fRefillSrc == 0x0))
        goto RedrawText;
    else
        goto L_26f6;

L_26f6:
    FillProdSrcLB(GetDlgItem(hwnd, IDC_U16_0x0416), 0xffff);

L_2712:
    ipl = 0x0;
    lpprod = &(lpplProdGlob->rgprod);
    goto L_2742;

L_272f:
    ipl = (ipl + 0x1);
    lpprod = (lpprod + 0x1);

L_2742:
    if ((ipl >= lpplProdGlob->iprodMac))
        goto L_28c3;
    else
        goto L_2755;

L_2755:
    iSrc = 0x0;
    goto L_2820;

L_275d:
    if (((LOWORD((uint32_t)((pProdGlob[iSrc] >> 0x11))) & 0x7) != lpprod->grobj))
        goto L_281c;
    else
        goto L_27b4;

L_27b4:
    /* untranslated: branch ss:[bp-0x2e] != (hiword(callresult(uint32_t)) & 0x0) ? L_281c : L_27bb */

L_27bb:
    if (((LOWORD((uint32_t)((pProdGlob[iSrc] >> 0xa))) & 0x7f) != lpprod->iItem))
        goto L_281c;
    else
        goto L_2812;

L_2812:
    /* untranslated: branch ss:[bp-0x2e] == (hiword(callresult(uint32_t)) & 0x0) ? L_282b : L_2816 */

L_2816:

L_281c:
    iSrc = (iSrc + 0x1);

L_2820:
    if ((iSrc < cProdGlob))
        goto L_275d;
    else
        goto L_282b;

L_282b:
    if (((LOWORD(pProdGlob[iSrc]) & 0x3ff) != 0x3ff))
        goto L_2853;
    else
        goto L_284b;

L_284b:
    if (((HIWORD(pProdGlob[iSrc]) & 0x0) == 0x0))
        goto L_272f;
    else
        goto L_2853;

L_2853:
    /* untranslated: ss:[bp-0x2c] = ((loword((int32_t)(words(lpprod->cItem, (*(lpprod+0x2) & 0x0)) << 0x0)) + LOWORD(pProdGlob[iSrc])) & 0x3ff) */
    /* untranslated: ss:[bp-0x2a] = ((hiword((int32_t)(words(lpprod->cItem, (*(lpprod+0x2) & 0x0)) << 0x0)) + HIWORD(pProdGlob[iSrc])) & 0x0) */
    LOWORD(pProdGlob[iSrc]) = (LOWORD(pProdGlob[iSrc]) & 0xfc00);
    HIWORD(pProdGlob[iSrc]) = (HIWORD(pProdGlob[iSrc]) & 0xffff);
    /* untranslated: LOWORD(pProdGlob[iSrc]) = (LOWORD(pProdGlob[iSrc]) | ss:[bp-0x2c]) */
    /* untranslated: HIWORD(pProdGlob[iSrc]) = (HIWORD(pProdGlob[iSrc]) | ss:[bp-0x2a]) */

L_28c3:
    if ((wParam != 0x816))
        goto L_2d68;
    else
        goto L_28cd;

L_28cd:
    /* untranslated: cMax = (lpplProdGlob->iprodMac + byte 0x25:[(0x22f6 + loword((uint32_t)(lParam * 0x28)))+0xf]) */
    if ((cMax >= 0x1))
        goto L_2911;
    else
        goto L_290c;

L_290c:
    cMax = 0x1;

L_2911:
    lpplprodT = LpplAlloc(0x4, cMax, htOrd);
    fmemset(&(lpplprodT->rgprod), 0x0, (cMax * 0x4));
    iDst = 0x0;
    iSrc = 0x0;
    goto L_295e;

L_295a:
    iSrc = (iSrc + 0x1);

L_295e:
    if ((iSrc >= lpplProdGlob->iprodMac))
        goto L_2a33;
    else
        goto L_2971;

L_2971:
    if (((LOWORD((uint32_t)((lpplProdGlob[iSrc].rgprod[0x0] >> 0x11))) & 0x7) != 0x1))
        goto L_29f0;
    else
        goto L_29a6;

L_29a6:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_29f0 : L_29ae */

L_29ae:
    if (((HIWORD((uint32_t)((lpplProdGlob[iSrc].rgprod[0x0] >> 0xa))) & 0x0) < 0x0))
        goto L_295a;
    else
        goto L_29e3;

L_29e3:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) > 0x0 ? L_29f0 : L_29e8 */

L_29e8:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) < 0x7 ? L_295a : L_29f0 */

L_29f0:
    lpplprodT[iDst].rgprod[0x0] = lpplProdGlob[iSrc].rgprod[0x0];
    iDst = (iDst + 0x1);

L_2a33:
    iSrc = 0x0;
    goto L_2a3f;

L_2a3b:
    iSrc = (iSrc + 0x1);

L_2a3f:
    /* untranslated: branch iSrc >= byte 0x25:[(0x22f6 + loword((uint32_t)(lParam * 0x28)))+0xf] ? L_2cb0 : L_2a65 */

L_2a65:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_2aba;
    else
        goto L_2a86;

L_2a86:
    /* untranslated: branch (0x25:[(((0x22f6 + loword((uint32_t)(lParam * 0x28))) + 0x10) + (iSrc * 0x2))] & 0x3f) <= 0x2 ? L_2a3b : L_2ab4 */

L_2ab4:

L_2aba:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raTerra))
        goto L_2b40;
    else
        goto L_2adb;

L_2adb:
    /* untranslated: branch (0x25:[(((0x22f6 + loword((uint32_t)(lParam * 0x28))) + 0x10) + (iSrc * 0x2))] & 0x3f) == 0x4 ? L_2a3b : L_2b0c */

L_2b0c:
    /* untranslated: branch (0x25:[(((0x22f6 + loword((uint32_t)(lParam * 0x28))) + 0x10) + (iSrc * 0x2))] & 0x3f) == 0x5 ? L_2a3b : L_2b3a */

L_2b3a:

L_2b40:
    LOWORD(lpplprodT[iDst].rgprod[0x0]) = ((LOWORD(lpplprodT[iDst].rgprod[0x0]) & 0xffff) | 0x0);
    HIWORD(lpplprodT[iDst].rgprod[0x0]) = ((HIWORD(lpplprodT[iDst].rgprod[0x0]) & 0xfff1) | 0x2);
    /* untranslated: LOWORD(lpplprodT[iDst].rgprod[0x0]) = ((LOWORD(lpplprodT[iDst].rgprod[0x0]) & 0x3ff) | loword((int32_t)(words(((0x25:[(((0x22f6 +
     * loword((uint32_t)(lParam * 0x28))) + 0x10) + (iSrc * 0x2))] & 0x3f) & 0x7f), (0x0 & 0x0)) << 0xa))) */
    /* untranslated: HIWORD(lpplprodT[iDst].rgprod[0x0]) = ((HIWORD(lpplprodT[iDst].rgprod[0x0]) & 0xfffe) | hiword((int32_t)(words(((0x25:[(((0x22f6 +
     * loword((uint32_t)(lParam * 0x28))) + 0x10) + (iSrc * 0x2))] & 0x3f) & 0x7f), (0x0 & 0x0)) << 0xa))) */
    /* untranslated: LOWORD(lpplprodT[iDst].rgprod[0x0]) = ((LOWORD(lpplprodT[iDst].rgprod[0x0]) & 0xfc00) | loword((int32_t)(words((((0x25:[(((0x22f6 +
     * loword((uint32_t)(lParam * 0x28))) + 0x10) + (iSrc * 0x2))] >> 0x6) & 0x3ff) & 0x3ff), (0x0 & 0x0)) << 0x0))) */
    /* untranslated: HIWORD(lpplprodT[iDst].rgprod[0x0]) = ((HIWORD(lpplprodT[iDst].rgprod[0x0]) & 0xffff) | hiword((int32_t)(words((((0x25:[(((0x22f6 +
     * loword((uint32_t)(lParam * 0x28))) + 0x10) + (iSrc * 0x2))] >> 0x6) & 0x3ff) & 0x3ff), (0x0 & 0x0)) << 0x0))) */
    iDst = (iDst + 0x1);
    goto L_2a3b;

L_2cb0:
    lpplprodT->iprodMac = LOBYTE(iDst);
    FreePl(lpplProdGlob);
    lpplProdGlob = lpplprodT;
    /* untranslated: part[176:2](sel) = ((part[176:2](sel) & 0xffff) | loword((int32_t)(words((byte 0x25:[(0x22f6 + loword((uint32_t)(lParam * 0x28)))+0xe] &
     * 0x1), (0x0 & 0x0)) << 0x17))) */
    /* untranslated: part[178:2](sel) = ((part[178:2](sel) & 0xff7f) | hiword((int32_t)(words((ss:[bp-0x32] & 0x1), (ss:[bp-0x30] & 0x0)) << 0x17))) */
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x008B), CB_LIMITTEXT, (loword((uint32_t)(words(part[176:2](sel), part[178:2](sel)) >> 0x17)) &
     * 0x1), 0x0) -> callresult(LRESULT) */
    goto L_2d71;

L_2d68:
    lpplProdGlob->iprodMac = 0x0;

L_2d71:
    FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), CB_GETCURSEL, 0x0, 0x0);
    FillProdSrcLB(GetDlgItem(hwnd, IDC_U16_0x0416), 0xffff);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0416), CB_GETCURSEL, 0x0, 0x0);
    goto RedrawText;

L_2df6:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x1))
        goto L_2e66;
    else
        goto RedrawText;

RedrawText:
    GetClientRect(hwnd, &(rc));
    rc.top = yTopFutureTech;
    rc.bottom = (LOWORD((0x7 * dyArial8)) + rc.top);
    rc.left = (rc.left + 0x82);
    InvalidateRect(hwnd, &(rc), 0x1);
    DrawProductionDlg(hwnd, 0x0, &(rc), 0xffff);
    goto L_3429;

L_2e66:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x2))
        goto L_3429;
    else
        goto L_2e82;

L_2e82:
    if ((wParam == 0x416))
        goto AddItem;
    else
        goto L_2e89;

L_2e89:

L_2e98:
    hwndLB = GetDlgItem(hwnd, IDC_U16_0x0417);
    /* untranslated: part[176:2](sel) = ((part[176:2](sel) & 0xffff) | loword((int32_t)(words((loword(SendMessage(GetDlgItem(hwnd, IDC_U16_0x008B), WM_USER,
     * 0x0, 0x0)) & 0x1), (0x0 & 0x0)) << 0x17))) */
    /* untranslated: part[178:2](sel) = ((part[178:2](sel) & 0xff7f) | hiword((int32_t)(words((ss:[bp-0x2c] & 0x1), (ss:[bp-0x2a] & 0x0)) << 0x17))) */
    lSel = SendMessage(hwndLB, CB_GETLBTEXTLEN, 0x0, 0x0);
    FillPlanetProdLB(hwndLB, lpplProdGlob, 0x0);
    SendMessage(hwndLB, CB_GETCURSEL, LOWORD(lSel), 0x0);
    goto RedrawText;

L_2f5c:
    hwndProdDlg = 0x0;
    StickyDlgPos(hwnd, 0xcd6, 0x0);
    if ((wParam != 0x1))
        goto L_2f87;
    else
        goto L_2f81;

L_2f81:
    t_merge_2f8a_0001 = 0x1;
    goto L_2f8a;

L_2f87:
    t_merge_2f8a_0001 = 0x0;

L_2f8a:
    EndDialog(hwnd, t_merge_2f8a_0001);
    goto L_3429;

L_2f93:
    if ((wParam != 0x42f))
        goto L_2fa3;
    else
        goto L_2f9d;

L_2f9d:
    t_merge_2fa6_0001 = 0x1;
    goto L_2fa6;

L_2fa3:
    t_merge_2fa6_0001 = 0xffff;

L_2fa6:
    c = t_merge_2fa6_0001;
    FinishProduction(0x1);
    if ((GetKeyState(0x10) >= 0x0))
        goto L_2ff6;
    else
        goto L_2fc6;

L_2fc6:
    if ((wParam != 0x42f))
        goto L_2fd6;
    else
        goto L_2fd0;

L_2fd0:
    t_merge_2fd9_0001 = 0x1;
    goto L_2fd9;

L_2fd6:
    t_merge_2fd9_0001 = 0x0;

L_2fd9:
    SelectAdjPlanet(0x0, IdFindAdjStarbase(sel.pl.id, t_merge_2fd9_0001));
    goto L_3005;

L_2ff6:
    SelectAdjPlanet(c, 0x0);

L_3005:
    InitProduction(0x0);
    InitializeProductionDlg(hwnd);
    GetClientRect(hwnd, &(rc));
    rc.top = yTopFutureTech;
    rc.bottom = (LOWORD((0x9 * dyArial8)) + rc.top);
    InvalidateRect(hwnd, &(rc), 0x1);
    goto L_3429;

L_3054:
    lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), CB_GETLBTEXTLEN, 0x0, 0x0);
    iMac = lpplProdGlob->iprodMac;
    if ((HIWORD(lSel) < 0x0))
        goto L_3429;
    else
        goto L_3093;

L_3093:
    if ((HIWORD(lSel) > 0x0))
        goto L_30a1;
    else
        goto L_3098;

L_3098:
    if ((LOWORD(lSel) <= 0x0))
        goto L_3429;
    else
        goto L_30a1;

L_30a1:
    if ((HIWORD(lSel) < SIGNHIWORD(iMac)))
        goto L_30bd;
    else
        goto L_30ad;

L_30ad:
    if ((HIWORD(lSel) > SIGNHIWORD(iMac)))
        goto L_3429;
    else
        goto L_30b2;

L_30b2:
    if ((LOWORD(lSel) >= iMac))
        goto L_3429;
    else
        goto L_30b7;

L_30b7:

L_30bd:
    lSel = (lSel - 0x1);
    /* untranslated: LOWORD(prod) = HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))] */
    /* untranslated: HIWORD(prod) = HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))+0x2] */
    /* untranslated: ss:[bp-0x2e] = HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(words((LOWORD(lSel) + 0x1), (HIWORD(lSel) + 0x0)) *
     * 0x4)))+0x2] */
    /* untranslated: HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))] = HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4)
     * + loword((int32_t)(words((LOWORD(lSel) + 0x1), (HIWORD(lSel) + 0x0)) * 0x4)))] */
    /* untranslated: ss:[bp-0x32]:[(ss:[bp-0x34] + loword((int32_t)(lSel * 0x4)))+0x2] = ss:[bp-0x2e] */
    /* untranslated: HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(words((LOWORD(lSel) + 0x1), (HIWORD(lSel) + 0x0)) * 0x4)))] =
     * LOWORD(prod) */
    /* untranslated: ss:[bp-0x2a]:[(ss:[bp-0x2c] + loword((int32_t)(words((LOWORD(lSel) + 0x1), (HIWORD(lSel) + 0x0)) * 0x4)))+0x2] = HIWORD(prod) */
    FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), CB_GETCURSEL, (LOWORD(lSel) + 0x2), 0x0);
    goto RedrawText;

L_3201:
    lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), CB_GETLBTEXTLEN, 0x0, 0x0);
    if ((HIWORD(lSel) > 0x0))
        goto L_3243;
    else
        goto L_3232;

L_3232:
    if ((HIWORD(lSel) < 0x0))
        goto L_3429;
    else
        goto L_3237;

L_3237:
    if ((LOWORD(lSel) <= 0x1))
        goto L_3429;
    else
        goto L_323d;

L_323d:

L_3243:
    iMac = lpplProdGlob->iprodMac;
    lSel = (lSel - 0x2);
    /* untranslated: LOWORD(prod) = HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))] */
    /* untranslated: HIWORD(prod) = HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))+0x2] */
    /* untranslated: ss:[bp-0x2e] = HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(words((LOWORD(lSel) + 0x1), (HIWORD(lSel) + 0x0)) *
     * 0x4)))+0x2] */
    /* untranslated: HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))] = HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4)
     * + loword((int32_t)(words((LOWORD(lSel) + 0x1), (HIWORD(lSel) + 0x0)) * 0x4)))] */
    /* untranslated: ss:[bp-0x32]:[(ss:[bp-0x34] + loword((int32_t)(lSel * 0x4)))+0x2] = ss:[bp-0x2e] */
    /* untranslated: HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(words((LOWORD(lSel) + 0x1), (HIWORD(lSel) + 0x0)) * 0x4)))] =
     * LOWORD(prod) */
    /* untranslated: ss:[bp-0x2a]:[(ss:[bp-0x2c] + loword((int32_t)(words((LOWORD(lSel) + 0x1), (HIWORD(lSel) + 0x0)) * 0x4)))+0x2] = HIWORD(prod) */
    FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), CB_GETCURSEL, (LOWORD(lSel) + 0x1), 0x0);
    goto RedrawText;

L_3395:
    WinHelp(hwnd, szHelpFile, 0x1, 0x423);
    goto L_3429;

L_33b6:
    if ((wParam == 0x1))
        goto L_2f5c;
    else
        goto L_33be;

L_33be:
    if ((wParam == 0x2))
        goto L_2f5c;
    else
        goto L_33c6;

L_33c6:
    if ((wParam == 0x76))
        goto L_3395;
    else
        goto L_33ce;

L_33ce:
    if ((wParam == 0x8b))
        goto L_2e98;
    else
        goto L_33d6;

L_33d6:
    if ((wParam == 0x416))
        goto L_2df6;
    else
        goto L_33de;

L_33de:
    if ((wParam == 0x417))
        goto L_2df6;
    else
        goto L_33e6;

L_33e6:
    if ((wParam == 0x418))
        goto AddItem;
    else
        goto L_33ee;

L_33ee:
    if ((wParam == 0x419))
        goto RemoveItem;
    else
        goto L_33f6;

L_33f6:
    if ((wParam == 0x42d))
        goto L_2712;
    else
        goto L_33fe;

L_33fe:
    if ((wParam == 0x42e))
        goto L_2f93;
    else
        goto L_3406;

L_3406:
    if ((wParam == 0x42f))
        goto L_2f93;
    else
        goto L_340e;

L_340e:
    if ((wParam == 0x439))
        goto L_3201;
    else
        goto L_3416;

L_3416:
    if ((wParam == 0x43a))
        goto L_3054;
    else
        goto L_341e;

L_341e:
    if ((wParam == 0x816))
        goto L_2712;
    else
        goto L_3429;

L_3429:
    return;
}

void InitializeProductionDlg(HWND hwnd) {
    char    rgch[86];
    int16_t i;
    int16_t iSel;
    PROD   *lpprod;

L_3430:
    iSel = 0xffff;
    _wsprintf(&(rgch), PszGetCompressedString(idsProductionQueueS), PszGetPlanetName(sel.pl.id), 0x25);
    SetWindowText(hwnd, &(rgch));
    FillProdSrcLB(GetDlgItem(hwnd, IDC_U16_0x0416), 0xffff);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0416), CB_GETCURSEL, 0x0, 0x0);
    i = 0x0;
    lpprod = &(lpplProdGlob->rgprod);
    goto L_34e7;

L_34d4:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_34e7:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_3558;
    else
        goto L_34fa;

L_34fa:
    if ((lpprod->grobj != 0x1))
        goto L_354f;
    else
        goto L_351a;

L_351a:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_354f : L_3522 */

L_3522:
    if (((HIWORD((uint32_t)((lpprod >> 0xa))) & 0x0) < 0x0))
        goto L_34d4;
    else
        goto L_3542;

L_3542:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) > 0x0 ? L_354f : L_3547 */

L_3547:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) < 0x7 ? L_34d4 : L_354f */

L_354f:
    iSel = i;

L_3558:
    FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), CB_GETCURSEL, (iSel + 0x1), 0x0);
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x008B), CB_LIMITTEXT, (loword((uint32_t)(words(part[176:2](sel), part[178:2](sel)) >> 0x17)) &
     * 0x1), 0x0) -> callresult(LRESULT) */
    return;
}

void DrawProductionDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iDraw) {
    int32_t  lSel;
    int16_t  iSrc;
    int16_t  idc;
    int16_t  fCreatedDC;
    int16_t  i;
    int16_t  c;
    int32_t  rgCost[4];
    int16_t  dxkT;
    int16_t  k;
    RECT     rc;
    PROD     prod;
    char     szT[100];
    uint16_t t_merge_3664_0001;
    uint16_t t_3735;
    uint16_t t_merge_385c_0001;

L_35dc:
    fCreatedDC = 0x0;
    if ((hdc != 0x0))
        goto L_3603;
    else
        goto L_35f3;

L_35f3:
    fCreatedDC = 0x1;
    hdc = GetDC(hwnd);

L_3603:
    SelectObject(hdc, LOWORD(rghfontArial8));
    dxkT = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsKt), 0x2));
    SelectObject(hdc, HIWORD(rghfontArial8));
    SetBkColor(hdc, crButtonFace);
    i = 0x0;
    goto L_3a3d;

L_3652:
    if ((i == 0x0))
        goto L_3661;
    else
        goto L_365b;

L_365b:
    t_merge_3664_0001 = 0x417;
    goto L_3664;

L_3661:
    t_merge_3664_0001 = 0x416;

L_3664:
    idc = t_merge_3664_0001;
    GetWindowRect(GetDlgItem(hwnd, idc), &(rc));
    ScreenToClient(hwnd, &(rc));
    ScreenToClient(hwnd, &(rc.right));
    lSel = SendMessage(GetDlgItem(hwnd, idc), CB_GETLBTEXTLEN, 0x0, 0x0);
    if ((HIWORD(lSel) < 0x0))
        goto L_3a39;
    else
        goto L_36cd;

L_36cd:
    if ((HIWORD(lSel) > 0x0))
        goto L_36db;
    else
        goto L_36d2;

L_36d2:
    if ((LOWORD(lSel) < 0x0))
        goto L_3a39;
    else
        goto L_36db;

L_36db:
    if ((LOWORD(lSel) != 0x0))
        goto L_36f9;
    else
        goto L_36e4;

L_36e4:
    if ((HIWORD(lSel) != 0x0))
        goto L_36f9;
    else
        goto L_36ed;

L_36ed:
    if ((i == 0x1))
        goto L_3a39;
    else
        goto L_36f3;

L_36f3:

L_36f9:
    if ((i != 0x0))
        goto L_3798;
    else
        goto L_3702;

L_3702:
    iSrc = 0x0;
    goto L_375a;

L_370a:
    if (((LOWORD(pProdGlob[iSrc]) & 0x3ff) != 0x0))
        goto L_3735;
    else
        goto L_372a;

L_372a:
    if (((HIWORD(pProdGlob[iSrc]) & 0x0) == 0x0))
        goto L_3756;
    else
        goto L_372f;

L_372f:

L_3735:
    t_3735 = LOWORD(lSel);
    lSel = (lSel - 0x1);
    if ((t_3735 != 0x0))
        goto L_3756;
    else
        goto L_374b;

L_374b:
    if ((HIWORD(lSel) == 0x0))
        goto L_3765;
    else
        goto L_3750;

L_3750:

L_3756:
    iSrc = (iSrc + 0x1);

L_375a:
    if ((iSrc < cProdGlob))
        goto L_370a;
    else
        goto L_3765;

L_3765:
    prod = pProdGlob[iSrc];
    prod.cItem = 0x1;
    HIWORD(prod) = ((HIWORD(prod) & 0xffff) | 0x0);
    goto L_37d8;

L_3798:
    lSel = (lSel - 0x1);
    /* untranslated: LOWORD(prod) = HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))] */
    /* untranslated: HIWORD(prod) = HIWORD(lpplProdGlob):[((LOWORD(lpplProdGlob) + 0x4) + loword((int32_t)(lSel * 0x4)))+0x2] */

L_37d8:
    /* untranslated: call GetProductionCosts(&sel.pl, &prod, &part[0:4](rgCost), idPlayer, 0x0) -> callresult(void) */
    rc.bottom = (yTopFutureTech + 0x4);
    SelectObject(hdc, HIWORD(rghfontArial8));
    c = CchGetString(idsRequiredMinerals, 0x57a4);
    TextOut(hdc, rc.left, rc.bottom, szWork, c);
    rc.left = (rc.left + 0x14);
    rc.right = (rc.right - 0x14);
    k = 0x0;
    goto L_3952;

L_384a:
    if ((k != 0x3))
        goto L_3859;
    else
        goto L_3853;

L_3853:
    t_merge_385c_0001 = 0x5;
    goto L_385c;

L_3859:
    t_merge_385c_0001 = k;

L_385c:
    c = t_merge_385c_0001;
    rc.bottom = (rc.bottom + dyArial8);
    SelectObject(hdc, HIWORD(rghfontArial8));
    SetTextColor(hdc, rgcrMinerals[c]);
    TextOut(hdc, rc.left, rc.bottom, rgszMinerals[c], lstrlen(rgszMinerals[c]));
    SelectObject(hdc, LOWORD(rghfontArial8));
    SetTextColor(hdc, crWindowText);
    c = _wsprintf(szWork, PCTLD, LOWORD(rgCost[k]), HIWORD(rgCost[k]));
    RightTextOut(hdc, ((rc.right - dxkT) + 0xfffe), rc.bottom, 0x57a4, c, dxMaxMineralQuan);
    if ((k > 0x2))
        goto L_394e;
    else
        goto L_3928;

L_3928:
    TextOut(hdc, (rc.right - dxkT), rc.bottom, PszGetCompressedString(idsKt), 0x2);

L_394e:
    k = (k + 0x1);

L_3952:
    if ((k <= 0x3))
        goto L_384a;
    else
        goto L_395b;

L_395b:
    if ((i == 0x0))
        goto L_3a39;
    else
        goto L_3964;

L_3964:
    /* untranslated: rc.bottom = (rc.bottom + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    SelectObject(hdc, HIWORD(rghfontArial8));
    c = _wsprintf(&(szT), PszGetCompressedString(idsDDoneCompletion), prod.pct);
    if ((PszProductionETA(&(sel.pl), lpplProdGlob, LOWORD(lSel), 0x0, 0x0) != 0x57a4))
        goto L_39ec;
    else
        goto L_39e6;

L_39e6:
    goto L_39ef;

L_39ec:

L_39ef:
    strcpy(szT[c], 0x57a4);
    TextOut(hdc, (rc.left + 0xffec), rc.bottom, &(szT), strlen(szT));
    SelectObject(hdc, LOWORD(rghfontArial8));

L_3a39:
    i = (i + 0x1);

L_3a3d:
    if ((i < 0x2))
        goto L_3652;
    else
        goto L_3a46;

L_3a46:
    GetClientRect(hwnd, &(rc));
    /* untranslated: rc.top = (rc.bottom - ((words(loword((0x5 * dyArial8)), signhiword(loword((0x5 * dyArial8)))) / 0x2) + 0xc)) */
    rc.bottom = ((dyArial8 | 0x1) + rc.top);
    rc.left = 0x6;
    rc.right = (rc.left + dyArial8);
    DrawDiamond(hdc, &(rc), hbrBBlue);
    rcProdDiamond = rc;
    SelectObject(hdc, HIWORD(rghfontArial8));
    c = CchGetString(idsApplyDefineProductionTemplate, 0x57a4);
    TextOut(hdc, (rc.right + 0x4), rc.top, szWork, c);
    if ((fCreatedDC == 0x0))
        goto L_3af9;
    else
        goto L_3aee;

L_3aee:
    ReleaseDC(hwnd, hdc);

L_3af9:
    return;
}

void FillProdSrcLB(HWND hwndLB, int16_t mdFill) {
    char     szT[80];
    int16_t  i;
    char    *psz;
    uint16_t t_merge_3c0f_0001;

L_3b00:
    i = 0x0;
    goto L_3b20;

L_3b11:
    szT[i] = 0x20;
    i = (i + 0x1);

L_3b20:
    if ((i < 0x6))
        goto L_3b11;
    else
        goto L_3b29;

L_3b29:
    SendMessage(hwndLB, CB_DIR, 0x0, 0x0);
    i = 0x0;
    goto L_3c80;

L_3b49:
    if (((HIWORD(pProdGlob[i]) & 0x0) < 0x0))
        goto L_3c7c;
    else
        goto L_3b69;

L_3b69:
    if (((HIWORD(pProdGlob[i]) & 0x0) > 0x0))
        goto L_3b76;
    else
        goto L_3b6e;

L_3b6e:
    if (((LOWORD(pProdGlob[i]) & 0x3ff) <= 0x0))
        goto L_3c7c;
    else
        goto L_3b76;

L_3b76:
    psz = PszNameProdItem((pProdGlob + (i * 0x4)));
    /* untranslated: call strcpy(&part[6:2](szT), psz) -> callresult(char *) */
    if (((LOWORD((uint32_t)((pProdGlob[i] >> 0x11))) & 0x7) != 0x2))
        goto L_3c15;
    else
        goto L_3bc9;

L_3bc9:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_3c15 : L_3bd1 */

L_3bd1:
    if (((HIWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x0) > 0x0))
        goto L_3c0c;
    else
        goto L_3bf9;

L_3bf9:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) < 0x0 ? L_3c06 : L_3bfe */

L_3bfe:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) >= 0x10 ? L_3c0c : L_3c06 */

L_3c06:
    t_merge_3c0f_0001 = 0x2a;
    goto L_3c0f;

L_3c0c:
    t_merge_3c0f_0001 = 0x23;

L_3c0f:
    /* untranslated: part[0:1](szT) = lobyte(t_merge_3c0f_0001) */
    goto L_3c65;

L_3c15:
    if (((HIWORD((uint32_t)((pProdGlob[i] >> 0xa))) & 0x0) > 0x0))
        goto L_3c61;
    else
        goto L_3c3d;

L_3c3d:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) < 0x0 ? L_3c4a : L_3c42 */

L_3c42:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) >= 0x7 ? L_3c61 : L_3c4a */

L_3c4a:
    /* untranslated: part[0:1](szT) = 0x49 */
    /* untranslated: call strcat(&part[6:2](szT), 0xcda) -> callresult(char *) */
    goto L_3c65;

L_3c61:
    /* untranslated: part[0:1](szT) = 0x20 */

L_3c65:
    /* untranslated: call SendMessage(hwndLB, CB_LIMITTEXT, 0x0, words(ss, szT)) -> callresult(LRESULT) */

L_3c7c:
    i = (i + 0x1);

L_3c80:
    if ((i < cProdGlob))
        goto L_3b49;
    else
        goto L_3c8b;

L_3c8b:
    return;
}

char *PszNameProdItem(PROD *lpprod) {
    uint32_t iItem;
    int16_t  iDelta;

L_3c92:
    LOWORD(iItem) = lpprod->iItem;
    HIWORD(iItem) = (HIWORD((uint32_t)((lpprod >> 0xa))) & 0x0);
    if ((lpprod->grobj != 0x2))
        goto L_3e7c;
    else
        goto L_3cd9;

L_3cd9:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_3e7c : L_3ce1 */

L_3ce1:
    if ((HIWORD(iItem) < 0x0))
        goto L_3e21;
    else
        goto L_3cea;

L_3cea:
    if ((HIWORD(iItem) > 0x0))
        goto L_3cf8;
    else
        goto L_3cef;

L_3cef:
    if ((LOWORD(iItem) < 0x10))
        goto L_3e21;
    else
        goto L_3cf8;

L_3cf8:
    iItem = (iItem - 0x10);
    /* untranslated: branch ((HIWORD(rglpshdefSB[idPlayer]):[(LOWORD(rglpshdefSB[idPlayer]) + loword((uint32_t)(iItem * 0x93)))+0x7b] >> 0x9) & 0x1) == 0x0 ?
     * L_3d46 : L_3d3b */

LBogus:
    /* untranslated: part[0:1](szWork) = 0x0 */
    return 0x57a4;

L_3d46:
    fstrcpy(szWork, ((uint8_t *)(rglpshdefSB[idPlayer]) + (LOWORD((uint32_t)((iItem * 0x93))) + 0x8)));
    if ((sel.pl.fStarbase == 0x0))
        goto L_3f13;
    else
        goto L_3d94;

L_3d94:
    /* untranslated: iDelta = (rglpshdefSB[idPlayer][sel.pl.isb].hul.ihuldef - HIWORD(rglpshdefSB[idPlayer]):[(LOWORD(rglpshdefSB[idPlayer]) +
     * loword((uint32_t)(iItem * 0x93)))]) */
    if ((iDelta <= 0x0))
        goto L_3e05;
    else
        goto L_3df2;

L_3df2:
    strcat(0x57a4, 0xce8);
    goto L_3f13;

L_3e05:
    if ((iDelta >= 0x0))
        goto L_3f13;
    else
        goto L_3e0e;

L_3e0e:
    strcat(0x57a4, 0xcf5);

L_3e21:
    /* untranslated: branch ((0x25:[(0x3f00 + loword((uint32_t)(iItem * 0x93)))+0x7b] >> 0x9) & 0x1) != 0x0 ? L_3d3b : L_3e49 */

L_3e49:

L_3e4f:
    strcpy(0x57a4, ((0x3f00 + LOWORD((uint32_t)((iItem * 0x93)))) + 0x8));

L_3e7c:
    if ((HIWORD(iItem) < 0x0))
        goto L_3ed8;
    else
        goto L_3e85;

L_3e85:
    if ((HIWORD(iItem) > 0x0))
        goto L_3e93;
    else
        goto L_3e8a;

L_3e8a:
    if ((LOWORD(iItem) < 0x12))
        goto L_3ed8;
    else
        goto L_3e93;

L_3e93:
    if ((HIWORD(iItem) > 0x0))
        goto L_3ed8;
    else
        goto L_3e9c;

L_3e9c:
    if ((HIWORD(iItem) < 0x0))
        goto L_3eaa;
    else
        goto L_3ea1;

L_3ea1:
    if ((LOWORD(iItem) > 0x1a))
        goto L_3ed8;
    else
        goto L_3eaa;

L_3eaa:
    fstrcpy(szWork, &(*(LpplanetaryFromId((LOWORD(iItem) + 0xffee)) + 0x8)));
    goto L_3f13;

L_3ed8:
    if ((LOWORD(iItem) != 0x1b))
        goto L_3efd;
    else
        goto L_3ee1;

L_3ee1:
    if ((HIWORD(iItem) != 0x0))
        goto L_3efd;
    else
        goto L_3eea;

L_3eea:
    CchGetString(idsPlanetaryScanner, 0x57a4);
    goto L_3f13;

L_3efd:
    CchGetString((LOWORD(iItem) + 0x7e), 0x57a4);

L_3f13:
    return 0x57a4;
}

void GetProductionCosts(PLANET *lppl, PROD *lpprod, uint32_t *rgCost, int16_t iplr, int16_t fOnlyOne) {
    uint16_t     rgCostsCur[4];
    ProdItemType iItem;
    uint16_t     rgCosts[4];
    int16_t      i;
    int16_t      j;
    SHDEF       *lpshdef;
    int16_t      raMajor;
    uint32_t     cItem;
    int16_t      fStarbase;
    PART         part;
    int16_t      cost;
    int16_t      chs;
    HUL         *lphulNew;
    HUL         *lphulCur;
    int16_t      costUpg;
    int16_t      costHalf;
    HUL         *lphulT;
    int16_t      rgCostsPartCur[4];
    int16_t      rgCostsPartNew[4];
    uint16_t     t_merge_4424_0001;
    uint16_t     t_merge_4465_0001;
    uint16_t     t_merge_45d3_0001;
    uint16_t     t_merge_4614_0001;
    uint16_t     t_merge_46f2_0001;
    uint16_t     t_merge_4733_0001;
    uint16_t     t_merge_4a9d_0001;
    uint16_t     t_merge_4b32_0001;
    uint32_t     t_merge_4b68_0001_wide;
    uint16_t     t_merge_4b9a_0001;
    uint16_t     t_merge_4bcf_0001;
    uint16_t     t_merge_4c10_0001;
    uint16_t     t_4c9c;

L_3f20:
    raMajor = GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv);
    fStarbase = 0x0;
    LOWORD(iItem) = lpprod->iItem;
    HIWORD(iItem) = (HIWORD((uint32_t)((lpprod >> 0xa))) & 0x0);
    LOWORD(cItem) = lpprod->cItem;
    HIWORD(cItem) = (*(lpprod + 0x2) & 0x0);
    if ((lpprod->grobj != 0x2))
        goto L_485b;
    else
        goto L_3fa1;

L_3fa1:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_485b : L_3fa9 */

L_3fa9:
    if ((HIWORD(iItem) < 0x0))
        goto L_3fe5;
    else
        goto L_3fb2;

L_3fb2:
    if ((HIWORD(iItem) > 0x0))
        goto L_3fc0;
    else
        goto L_3fb7;

L_3fb7:
    if ((LOWORD(iItem) < 0x10))
        goto L_3fe5;
    else
        goto L_3fc0;

L_3fc0:
    lpshdef = rglpshdefSB[iplr];
    iItem = (iItem - 0x10);
    fStarbase = 0x1;
    goto L_3ffa;

L_3fe5:
    lpshdef = rglpshdef[iplr];

L_3ffa:
    /* untranslated: branch ((HIWORD(lpshdef):[(LOWORD(lpshdef) + loword((uint32_t)(iItem * 0x93)))+0x7b] >> 0x9) & 0x1) == 0x0 ? L_4058 : L_402b */

L_402b:
    i = 0x0;
    goto L_404c;

L_4033:
    rgCost[i] = 0x0;
    i = (i + 0x1);

L_404c:
    if ((i >= 0x4))
        goto L_4f3a;
    else
        goto L_4052;

L_4052:

L_4058:
    GetTrueHullCost(iplr, ((uint8_t *)(lpshdef) + LOWORD((uint32_t)((iItem * 0x93)))), &(LOWORD(rgCosts)));
    if ((fStarbase == 0x0))
        goto L_475b;
    else
        goto L_408d;

L_408d:
    if ((lppl->fStarbase == 0x0))
        goto L_475b;
    else
        goto L_40a4;

L_40a4:
    lphulCur = &(rglpshdefSB[iplr][lppl->isb]);
    lphulNew = ((uint8_t *)(lpshdef) + LOWORD((uint32_t)((iItem * 0x93))));
    GetTrueHullCost(iplr, lphulCur, &(LOWORD(rgCostsCur)));
    if ((lphulCur->ihuldef == lphulNew->ihuldef))
        goto L_418e;
    else
        goto L_4111;

L_4111:
    i = 0x0;
    goto L_4182;

L_4119:
    /* untranslated: costHalf = (words(rgCosts[i], 0x0) / 0x2) */
    costUpg = (rgCosts[i] - ((uint32_t)(rgCostsCur[i]) / 0x2));
    if ((costHalf <= costUpg))
        goto L_416f;
    else
        goto L_415d;

L_415d:
    rgCosts[i] = costHalf;
    goto L_417e;

L_416f:
    rgCosts[i] = costUpg;

L_417e:
    i = (i + 0x1);

L_4182:
    if ((i >= 0x4))
        goto L_475b;
    else
        goto L_4188;

L_4188:

L_418e:
    lphulT = LphuldefFromId(lphulCur->ihuldef);
    part.hs.grhst = hstNone;
    /* untranslated: part[4:2](part) = LOWORD(lphulT) */
    /* untranslated: part[6:2](part) = HIWORD(lphulT) */
    GetTruePartCost(iplr, &(part), &(LOWORD(rgCostsPartCur)));
    i = 0x0;
    goto L_41ea;

L_41ce:
    rgCosts[i] = (rgCosts[i] - rgCostsPartCur[i]);
    i = (i + 0x1);

L_41ea:
    if ((i < 0x4))
        goto L_41ce;
    else
        goto L_41f3;

L_41f3:
    chs = lphulCur->chs;
    i = 0x0;
    goto L_4750;

L_4208:
    /* untranslated: branch ((lphulCur->rgTech[part[12:0](rgcrPlrHistory[i])] >> 0x8) & 0xff) == 0x0 ? L_474c : L_4232 */

L_4232:
    /* untranslated: branch ((lphulNew->rgTech[part[12:0](rgcrPlrHistory[i])] >> 0x8) & 0xff) == 0x0 ? L_474c : L_4259 */

L_4259:

L_425f:
    part.hs.grhst = lphulCur->rghs[i].grhst;
    /* untranslated: HIWORD(part) = lphulCur->rgTech[part[12:0](rgcrPlrHistory[i])] */
    FLookupPart(&(part));
    GetTruePartCost(iplr, &(part), &(LOWORD(rgCostsPartCur)));
    part.hs.grhst = lphulNew->rghs[i].grhst;
    /* untranslated: HIWORD(part) = lphulNew->rgTech[part[12:0](rgcrPlrHistory[i])] */
    FLookupPart(&(part));
    GetTruePartCost(iplr, &(part), &(LOWORD(rgCostsPartNew)));
    if ((lphulCur->rghs[i].grhst == lphulNew->rghs[i].grhst))
        goto L_4481;
    else
        goto L_431a;

L_431a:
    j = 0x0;
    goto L_4475;

L_4322:
    /* untranslated: rgCostsPartCur[j] = loword((rgCostsPartCur[j] * ((lphulCur->rgTech[part[12:0](rgcrPlrHistory[i])] >> 0x8) & 0xff))) */
    /* untranslated: rgCostsPartNew[j] = loword((rgCostsPartNew[j] * ((lphulNew->rgTech[part[12:0](rgcrPlrHistory[i])] >> 0x8) & 0xff))) */
    /* untranslated: branch (words(loword((0x3 * rgCostsPartNew[j])), signhiword(loword((0x3 * rgCostsPartNew[j])))) / 0xa) <= (rgCostsPartNew[j] -
     * (words(loword((0x7 * rgCostsPartCur[j])), signhiword(loword((0x7 * rgCostsPartCur[j])))) / 0xa)) ? L_43f9 : L_43e1 */

L_43e1:
    /* untranslated: t_merge_4424_0001 = (words(loword((0x3 * rgCostsPartNew[j])), signhiword(loword((0x3 * rgCostsPartNew[j])))) / 0xa) */
    goto L_4424;

L_43f9:
    /* untranslated: ss:[bp-0x52] = (words(loword((0x7 * rgCostsPartCur[j])), signhiword(loword((0x7 * rgCostsPartCur[j])))) / 0xa) */
    /* untranslated: t_merge_4424_0001 = (rgCostsPartNew[j] - ss:[bp-0x52]) */

L_4424:
    cost = t_merge_4424_0001;
    if ((rgCosts[j] >= (rgCostsPartNew[j] - cost)))
        goto L_4456;
    else
        goto L_4447;

L_4447:
    t_merge_4465_0001 = rgCosts[j];
    goto L_4465;

L_4456:
    t_merge_4465_0001 = (rgCostsPartNew[j] - cost);

L_4465:
    rgCosts[j] = (rgCosts[j] - t_merge_4465_0001);
    j = (j + 0x1);

L_4475:
    if ((j >= 0x4))
        goto L_474c;
    else
        goto L_447b;

L_447b:

L_4481:
    /* untranslated: branch (lphulCur->rgTech[part[12:0](rgcrPlrHistory[i])] & 0xff) == (lphulNew->rgTech[part[12:0](rgcrPlrHistory[i])] & 0xff) ? L_4630 :
     * L_44c3 */

L_44c3:
    j = 0x0;
    goto L_4624;

L_44cb:
    /* untranslated: rgCostsPartCur[j] = loword((rgCostsPartCur[j] * ((lphulCur->rgTech[part[12:0](rgcrPlrHistory[i])] >> 0x8) & 0xff))) */
    /* untranslated: rgCostsPartNew[j] = loword((rgCostsPartNew[j] * ((lphulNew->rgTech[part[12:0](rgcrPlrHistory[i])] >> 0x8) & 0xff))) */
    if ((((uint32_t)((rgCostsPartNew[j] * 0x2)) / 0xa) <= (rgCostsPartNew[j] - ((uint32_t)((rgCostsPartCur[j] * 0x8)) / 0xa))))
        goto L_45a5;
    else
        goto L_458e;

L_458e:
    t_merge_45d3_0001 = ((uint32_t)((rgCostsPartNew[j] * 0x2)) / 0xa);
    goto L_45d3;

L_45a5:
    /* untranslated: ss:[bp-0x52] = (sext16to32((rgCostsPartCur[j] * 0x8)) / 0xa) */
    /* untranslated: t_merge_45d3_0001 = (rgCostsPartNew[j] - ss:[bp-0x52]) */

L_45d3:
    cost = t_merge_45d3_0001;
    if ((rgCosts[j] >= (rgCostsPartNew[j] - cost)))
        goto L_4605;
    else
        goto L_45f6;

L_45f6:
    t_merge_4614_0001 = rgCosts[j];
    goto L_4614;

L_4605:
    t_merge_4614_0001 = (rgCostsPartNew[j] - cost);

L_4614:
    rgCosts[j] = (rgCosts[j] - t_merge_4614_0001);
    j = (j + 0x1);

L_4624:
    if ((j >= 0x4))
        goto L_474c;
    else
        goto L_462a;

L_462a:

L_4630:
    j = 0x0;
    goto L_4743;

L_4638:
    /* untranslated: rgCostsPartCur[j] = loword((rgCostsPartCur[j] * ((lphulCur->rgTech[part[12:0](rgcrPlrHistory[i])] >> 0x8) & 0xff))) */
    /* untranslated: rgCostsPartNew[j] = loword((rgCostsPartNew[j] * ((lphulNew->rgTech[part[12:0](rgcrPlrHistory[i])] >> 0x8) & 0xff))) */
    if ((0x0 <= (rgCostsPartNew[j] - rgCostsPartCur[j])))
        goto L_46da;
    else
        goto L_46d4;

L_46d4:
    t_merge_46f2_0001 = 0x0;
    goto L_46f2;

L_46da:
    t_merge_46f2_0001 = (rgCostsPartNew[j] - rgCostsPartCur[j]);

L_46f2:
    cost = t_merge_46f2_0001;
    if ((rgCosts[j] >= (rgCostsPartNew[j] - cost)))
        goto L_4724;
    else
        goto L_4715;

L_4715:
    t_merge_4733_0001 = rgCosts[j];
    goto L_4733;

L_4724:
    t_merge_4733_0001 = (rgCostsPartNew[j] - cost);

L_4733:
    rgCosts[j] = (rgCosts[j] - t_merge_4733_0001);
    j = (j + 0x1);

L_4743:
    if ((j < 0x4))
        goto L_4638;
    else
        goto L_474c;

L_474c:
    i = (i + 0x1);

L_4750:
    if ((i < chs))
        goto L_4208;
    else
        goto L_475b;

L_475b:
    if ((fStarbase == 0x0))
        goto L_47d8;
    else
        goto L_4764;

L_4764:
    if ((GetRaceGrbit(rgplr[iplr], ibitRaceISB) != 0x0))
        goto L_47a4;
    else
        goto L_4784;

L_4784:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raMacintosh))
        goto L_47d8;
    else
        goto L_47a4;

L_47a4:
    i = 0x0;
    goto L_47cf;

L_47ac:
    /* untranslated: rgCosts[i] = (rgCosts[i] - (words(rgCosts[i], 0x0) / 0x5)) */
    i = (i + 0x1);

L_47cf:
    if ((i < 0x4))
        goto L_47ac;
    else
        goto L_47d8;

L_47d8:
    if ((fStarbase == 0x0))
        goto L_4823;
    else
        goto L_47e1;

L_47e1:
    i = 0x0;
    goto L_4817;

L_47e9:
    /* untranslated: LOWORD(rgCost[i]) = (words((rgCosts[i] + 0x1), 0x0) / 0x2) */
    HIWORD(rgCost[i]) = 0x0;
    i = (i + 0x1);

L_4817:
    if ((i >= 0x4))
        goto L_4eef;
    else
        goto L_481d;

L_481d:

L_4823:
    i = 0x0;
    goto L_484f;

L_482b:
    LOWORD(rgCost[i]) = rgCosts[i];
    HIWORD(rgCost[i]) = 0x0;
    i = (i + 0x1);

L_484f:
    if ((i >= 0x4))
        goto L_4eef;
    else
        goto L_4855;

L_4855:

L_485b:
    goto L_4d3c;

L_4867:
    cost = GetRaceGrbit(rgplr[iplr], ibitRaceCheapFact);
    if ((gd.fTutorial == 0x0))
        goto L_48c5;
    else
        goto L_4895;

L_4895:
    i = 0x0;
    goto L_48b9;

L_489d:
    rgCost[i] = (uint32_t)((0x2 - cost));
    i = (i + 0x1);

L_48b9:
    if ((i >= 0x3))
        goto L_48f0;
    else
        goto L_48bf;

L_48bf:

L_48c5:
    LOWORD(rgCost[0x1]) = 0x0;
    *(rgCost + 0x6) = 0x0;
    rgCost = 0x0;
    *(rgCost + 0x2) = 0x0;
    LOWORD(rgCost[0x2]) = (0x4 - cost);
    *(rgCost + 0xa) = SIGNHIWORD((0x4 - cost));

L_48f0:
    GetRaceStat(rgplr[iplr], rsFactBuild);
    /* untranslated: LOWORD(rgCost[0x3]) = callresult(int16_t) */
    /* untranslated: *(rgCost+0xe) = signhiword(callresult(int16_t)) */
    goto L_4eef;

L_4915:
    i = 0x0;
    goto L_4936;

L_491d:
    rgCost[i] = 0x0;
    i = (i + 0x1);

L_4936:
    if ((i < 0x3))
        goto L_491d;
    else
        goto L_493f;

L_493f:
    GetRaceStat(rgplr[iplr], rsMineBuild);
    /* untranslated: LOWORD(rgCost[0x3]) = callresult(int16_t) */
    /* untranslated: *(rgCost+0xe) = signhiword(callresult(int16_t)) */
    goto L_4eef;

L_4964:
    part.hs.grhst = hstPlanetary;
    part.hs.iItem = 0x9;
    FLookupPart(&(part));
    i = 0x0;
    goto L_49b6;

L_4989:
    /* untranslated: LOWORD(rgCost[i]) = part[6:2](part):[((part[4:2](part) + 0x2c) + (i * 0x2))] */
    /* untranslated: HIWORD(rgCost[i]) = signhiword(part[6:2](part):[((part[4:2](part) + 0x2c) + (i * 0x2))]) */
    i = (i + 0x1);

L_49b6:
    if ((i < 0x3))
        goto L_4989;
    else
        goto L_49bf;

L_49bf:
    /* untranslated: LOWORD(rgCost[0x3]) = part[42:2](part[4:4](part)) */
    *(rgCost + 0xe) = 0x0;
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raDefend))
        goto L_4eef;
    else
        goto L_49f2;

L_49f2:
    i = 0x0;
    goto L_4a3c;

L_49fa:
    rgCost[i] = (uint32_t)(((uint32_t)((rgCost[i] * 0x3)) / 0x5));
    i = (i + 0x1);

L_4a3c:
    if ((i >= 0x4))
        goto L_4eef;
    else
        goto L_4a42;

L_4a42:

L_4a48:
    i = 0x0;
    goto L_4a69;

L_4a50:
    rgCost[i] = 0x0;
    i = (i + 0x1);

L_4a69:
    if ((i < 0x3))
        goto L_4a50;
    else
        goto L_4a72;

L_4a72:
    if ((GetRaceGrbit(rgplr[iplr], ibitRaceMineralAlchemy) == 0x0))
        goto L_4a99;
    else
        goto L_4a92;

L_4a92:
    t_merge_4a9d_0001 = 0x19;
    goto L_4a9d;

L_4a99:
    t_merge_4a9d_0001 = 0x64;

L_4a9d:
    LOWORD(rgCost[0x3]) = t_merge_4a9d_0001;
    *(rgCost + 0xe) = 0x0;
    goto L_4eef;

L_4aa9:
    part.hs.grhst = hstPlanetary;
    part.hs.iItem = 0xe;
    FLookupPart(&(part));
    GetTruePartCost(iplr, &(part), &(LOWORD(rgCosts)));
    i = 0x0;
    goto L_4b05;

L_4ae1:
    LOWORD(rgCost[i]) = rgCosts[i];
    HIWORD(rgCost[i]) = 0x0;
    i = (i + 0x1);

L_4b05:
    if ((i >= 0x4))
        goto L_4eef;
    else
        goto L_4b0b;

L_4b0b:

L_4b11:
    if ((raMajor != 0x6))
        goto L_4b20;
    else
        goto L_4b1a;

L_4b1a:
    t_merge_4b32_0001 = 0x46;
    goto L_4b32;

L_4b20:
    if ((raMajor != 0x7))
        goto L_4b2f;
    else
        goto L_4b29;

L_4b29:
    t_merge_4b32_0001 = 0x78;
    goto L_4b32;

L_4b2f:
    t_merge_4b32_0001 = 0x6e;

L_4b32:
    j = t_merge_4b32_0001;
    i = 0x0;
    goto L_4b7d;

L_4b3d:
    if (((LOWORD(iItem) + 0xfff2) != i))
        goto L_4b64;
    else
        goto L_4b56;

L_4b56:
    if (((HIWORD(iItem) + 0xffff) != 0x0))
        goto L_4b64;
    else
        goto L_4b5d;

L_4b5d:
    t_merge_4b68_0001_wide = (uint32_t)(j);
    goto L_4b68;

L_4b64:
    t_merge_4b68_0001_wide = 0x0;

L_4b68:
    rgCost[i] = t_merge_4b68_0001_wide;
    i = (i + 0x1);

L_4b7d:
    if ((i < 0x3))
        goto L_4b3d;
    else
        goto L_4b86;

L_4b86:
    if ((raMajor != 0x6))
        goto L_4b96;
    else
        goto L_4b8f;

L_4b8f:
    t_merge_4b9a_0001 = 0x5;
    goto L_4b9a;

L_4b96:
    t_merge_4b9a_0001 = 0xa;

L_4b9a:
    LOWORD(rgCost[i]) = t_merge_4b9a_0001;
    HIWORD(rgCost[i]) = 0x0;
    goto L_4eef;

L_4bae:
    if ((raMajor != 0x6))
        goto L_4bbd;
    else
        goto L_4bb7;

L_4bb7:
    t_merge_4bcf_0001 = 0x19;
    goto L_4bcf;

L_4bbd:
    if ((raMajor != 0x7))
        goto L_4bcc;
    else
        goto L_4bc6;

L_4bc6:
    t_merge_4bcf_0001 = 0x30;
    goto L_4bcf;

L_4bcc:
    t_merge_4bcf_0001 = 0x2c;

L_4bcf:
    j = t_merge_4bcf_0001;
    i = 0x0;
    goto L_4bf3;

L_4bda:
    rgCost[i] = (uint32_t)(j);
    i = (i + 0x1);

L_4bf3:
    if ((i < 0x3))
        goto L_4bda;
    else
        goto L_4bfc;

L_4bfc:
    if ((raMajor != 0x6))
        goto L_4c0c;
    else
        goto L_4c05;

L_4c05:
    t_merge_4c10_0001 = 0x5;
    goto L_4c10;

L_4c0c:
    t_merge_4c10_0001 = 0xa;

L_4c10:
    LOWORD(rgCost[i]) = t_merge_4c10_0001;
    HIWORD(rgCost[i]) = 0x0;
    goto L_4eef;

L_4c24:
    LOWORD(rgCost[0x2]) = 0x0;
    *(rgCost + 0xa) = 0x0;
    LOWORD(rgCost[0x1]) = 0x0;
    *(rgCost + 0x6) = 0x0;
    rgCost = 0x0;
    *(rgCost + 0x2) = 0x0;
    if ((GetRaceGrbit(rgplr[iplr], ibitRaceTT) == 0x0))
        goto L_4c78;
    else
        goto L_4c68;

L_4c68:
    LOWORD(rgCost[0x3]) = 0x46;
    *(rgCost + 0xe) = 0x0;
    goto L_4c85;

L_4c78:
    LOWORD(rgCost[0x3]) = 0x64;
    *(rgCost + 0xe) = 0x0;

L_4c85:
    if ((raMajor != 0x3))
        goto L_4eef;
    else
        goto L_4c8e;

L_4c8e:
    t_4c9c = LOWORD(rgCost[0x3]);
    /* untranslated: LOWORD(rgCost[0x3]) = loword((uint32_t)(words(*(rgCost+0xe), LOWORD(rgCost[0x3])) / 0x2)) */
    /* untranslated: *(rgCost+0xe) = hiword((uint32_t)(words(*(rgCost+0xe), t_4c9c) / 0x2)) */

L_4cb0:
    iItem = 0x12;

L_4cba:
    part.hs.grhst = hstPlanetary;
    HIWORD(part) = ((HIWORD(part) & 0xff00) | ((LOWORD(iItem) + 0xffee) & 0xff));
    FLookupPart(&(part));
    GetTruePartCost(iplr, &(part), &(LOWORD(rgCosts)));
    i = 0x0;
    goto L_4d2d;

L_4d09:
    LOWORD(rgCost[i]) = rgCosts[i];
    HIWORD(rgCost[i]) = 0x0;
    i = (i + 0x1);

L_4d2d:
    if ((i >= 0x4))
        goto L_4eef;
    else
        goto L_4d33;

L_4d33:

L_4d3c:
    if ((LOWORD(iItem) != 0x0))
        goto L_4d4c;
    else
        goto L_4d44;

L_4d44:
    if ((HIWORD(iItem) == 0x0))
        goto L_4915;
    else
        goto L_4d4c;

L_4d4c:
    if ((LOWORD(iItem) != 0x1))
        goto L_4d5c;
    else
        goto L_4d54;

L_4d54:
    if ((HIWORD(iItem) == 0x0))
        goto L_4867;
    else
        goto L_4d5c;

L_4d5c:
    if ((LOWORD(iItem) != 0x2))
        goto L_4d6c;
    else
        goto L_4d64;

L_4d64:
    if ((HIWORD(iItem) == 0x0))
        goto L_4964;
    else
        goto L_4d6c;

L_4d6c:
    if ((LOWORD(iItem) != 0x3))
        goto L_4d7c;
    else
        goto L_4d74;

L_4d74:
    if ((HIWORD(iItem) == 0x0))
        goto L_4a48;
    else
        goto L_4d7c;

L_4d7c:
    if ((LOWORD(iItem) != 0x4))
        goto L_4d8c;
    else
        goto L_4d84;

L_4d84:
    if ((HIWORD(iItem) == 0x0))
        goto L_4c24;
    else
        goto L_4d8c;

L_4d8c:
    if ((LOWORD(iItem) != 0x5))
        goto L_4d9c;
    else
        goto L_4d94;

L_4d94:
    if ((HIWORD(iItem) == 0x0))
        goto L_4c24;
    else
        goto L_4d9c;

L_4d9c:
    if ((LOWORD(iItem) != 0x6))
        goto L_4dac;
    else
        goto L_4da4;

L_4da4:
    if ((HIWORD(iItem) == 0x0))
        goto L_4bae;
    else
        goto L_4dac;

L_4dac:
    if ((LOWORD(iItem) != 0x7))
        goto L_4dbc;
    else
        goto L_4db4;

L_4db4:
    if ((HIWORD(iItem) == 0x0))
        goto L_4867;
    else
        goto L_4dbc;

L_4dbc:
    if ((LOWORD(iItem) != 0x8))
        goto L_4dcc;
    else
        goto L_4dc4;

L_4dc4:
    if ((HIWORD(iItem) == 0x0))
        goto L_4915;
    else
        goto L_4dcc;

L_4dcc:
    if ((LOWORD(iItem) != 0x9))
        goto L_4ddc;
    else
        goto L_4dd4;

L_4dd4:
    if ((HIWORD(iItem) == 0x0))
        goto L_4964;
    else
        goto L_4ddc;

L_4ddc:
    if ((LOWORD(iItem) != 0xb))
        goto L_4dec;
    else
        goto L_4de4;

L_4de4:
    if ((HIWORD(iItem) == 0x0))
        goto L_4a48;
    else
        goto L_4dec;

L_4dec:
    if ((LOWORD(iItem) != 0xc))
        goto L_4dfc;
    else
        goto L_4df4;

L_4df4:
    if ((HIWORD(iItem) == 0x0))
        goto L_4c24;
    else
        goto L_4dfc;

L_4dfc:
    if ((LOWORD(iItem) != 0xd))
        goto L_4e0c;
    else
        goto L_4e04;

L_4e04:
    if ((HIWORD(iItem) == 0x0))
        goto L_4aa9;
    else
        goto L_4e0c;

L_4e0c:
    if ((LOWORD(iItem) != 0xe))
        goto L_4e1c;
    else
        goto L_4e14;

L_4e14:
    if ((HIWORD(iItem) == 0x0))
        goto L_4b11;
    else
        goto L_4e1c;

L_4e1c:
    if ((LOWORD(iItem) != 0xf))
        goto L_4e2c;
    else
        goto L_4e24;

L_4e24:
    if ((HIWORD(iItem) == 0x0))
        goto L_4b11;
    else
        goto L_4e2c;

L_4e2c:
    if ((LOWORD(iItem) != 0x10))
        goto L_4e3c;
    else
        goto L_4e34;

L_4e34:
    if ((HIWORD(iItem) == 0x0))
        goto L_4b11;
    else
        goto L_4e3c;

L_4e3c:
    if ((LOWORD(iItem) != 0x11))
        goto L_4e4c;
    else
        goto L_4e44;

L_4e44:
    if ((HIWORD(iItem) == 0x0))
        goto L_4bae;
    else
        goto L_4e4c;

L_4e4c:
    if ((LOWORD(iItem) != 0x12))
        goto L_4e5c;
    else
        goto L_4e54;

L_4e54:
    if ((HIWORD(iItem) == 0x0))
        goto L_4cba;
    else
        goto L_4e5c;

L_4e5c:
    if ((LOWORD(iItem) != 0x13))
        goto L_4e6c;
    else
        goto L_4e64;

L_4e64:
    if ((HIWORD(iItem) == 0x0))
        goto L_4cba;
    else
        goto L_4e6c;

L_4e6c:
    if ((LOWORD(iItem) != 0x14))
        goto L_4e7c;
    else
        goto L_4e74;

L_4e74:
    if ((HIWORD(iItem) == 0x0))
        goto L_4cba;
    else
        goto L_4e7c;

L_4e7c:
    if ((LOWORD(iItem) != 0x15))
        goto L_4e8c;
    else
        goto L_4e84;

L_4e84:
    if ((HIWORD(iItem) == 0x0))
        goto L_4cba;
    else
        goto L_4e8c;

L_4e8c:
    if ((LOWORD(iItem) != 0x16))
        goto L_4e9c;
    else
        goto L_4e94;

L_4e94:
    if ((HIWORD(iItem) == 0x0))
        goto L_4cba;
    else
        goto L_4e9c;

L_4e9c:
    if ((LOWORD(iItem) != 0x17))
        goto L_4eac;
    else
        goto L_4ea4;

L_4ea4:
    if ((HIWORD(iItem) == 0x0))
        goto L_4cba;
    else
        goto L_4eac;

L_4eac:
    if ((LOWORD(iItem) != 0x18))
        goto L_4ebc;
    else
        goto L_4eb4;

L_4eb4:
    if ((HIWORD(iItem) == 0x0))
        goto L_4cba;
    else
        goto L_4ebc;

L_4ebc:
    if ((LOWORD(iItem) != 0x19))
        goto L_4ecc;
    else
        goto L_4ec4;

L_4ec4:
    if ((HIWORD(iItem) == 0x0))
        goto L_4cba;
    else
        goto L_4ecc;

L_4ecc:
    if ((LOWORD(iItem) != 0x1a))
        goto L_4edc;
    else
        goto L_4ed4;

L_4ed4:
    if ((HIWORD(iItem) == 0x0))
        goto L_4cba;
    else
        goto L_4edc;

L_4edc:
    if ((LOWORD(iItem) != 0x1b))
        goto L_4eef;
    else
        goto L_4ee4;

L_4ee4:
    if ((HIWORD(iItem) == 0x0))
        goto L_4cb0;
    else
        goto L_4eef;

L_4eef:
    if ((fOnlyOne != 0x0))
        goto L_4f3a;
    else
        goto L_4ef8;

L_4ef8:
    i = 0x0;
    goto L_4f31;

L_4f00:
    rgCost[i] = (uint32_t)((rgCost[i] * cItem));
    i = (i + 0x1);

L_4f31:
    if ((i < 0x4))
        goto L_4f00;
    else
        goto L_4f3a;

L_4f3a:
    return;
}

void EstimateItemProdSched(PLANET *lppl, PLPROD *lpplprod, ProdItemType iItem, int16_t *piFirst, int16_t *piLast) {
    int32_t  cResearch;
    PLANET   pl;
    int32_t  rglQuan[3];
    int16_t  cBuilt;
    PROD     prodPartial;
    int16_t  mdStatus;
    int16_t  i;
    int16_t  j;
    int16_t  iPass;
    int16_t  fAlchemy;
    int16_t  iMac;
    int32_t  rgRes[4];
    PROD    *lpprod;
    uint16_t t_merge_5223_0001;

L_4f40:
    if ((LOWORD(lpplprod) != 0x0))
        goto L_4f6c;
    else
        goto L_4f52;

L_4f52:
    if ((HIWORD(lpplprod) != 0x0))
        goto L_4f6c;
    else
        goto L_4f5b;

L_4f5b:
    lpplprod = lppl->lpplprod;

L_4f6c:
    pl = *(lppl);
    pl.lpplprod = LpplAlloc(0x4, lpplprod->iprodMax, htOrd);
    fmemcpy(&(pl.lpplprod->rgprod), &(lpplprod->rgprod), (lpplprod->iprodMac * 0x4));
    pl.lpplprod->iprodMac = lpplprod->iprodMac;
    iMac = lpplprod->iprodMac;
    prodPartial.cItem = 0x0;
    HIWORD(prodPartial) = ((HIWORD(prodPartial) & 0xffff) | 0x0);
    *(piLast) = 0x0;
    *(piFirst) = 0x0;
    iPass = 0x1;
    goto L_5447;

L_5018:
    EstMineralsMined(&(pl), rglQuan, 0xffffffff, 0x1);
    j = 0x0;
    goto L_5065;

L_503f:
    rgRes[j] = pl.rgwtMin[j];
    j = (j + 0x1);

L_5065:
    if ((j < 0x3))
        goto L_503f;
    else
        goto L_506e;

L_506e:
    CResourcesAtPlanet(&(pl), lppl->iPlayer);
    /* untranslated: part[12:2](rgRes) = callresult(int16_t) */
    /* untranslated: part[14:2](rgRes) = signhiword(callresult(int16_t)) */
    /* untranslated: branch (loword((uint32_t)(words(part[24:2](pl), part[26:2](pl)) >> 0x17)) & 0x1) != 0x0 ? L_50f4 : L_50a7 */

L_50a7:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_50f4 : L_50af */

L_50af:
    /* untranslated: cResearch = (int32_t)((uint32_t)(words(part[14:2](rgRes), part[12:2](rgRes)) * sext16to32(sext8to16(rgplr[lppl->iPlayer].pctResearch))) /
     * 0x64) */
    /* untranslated: part[12:2](rgRes) = (part[12:2](rgRes) - LOWORD(cResearch)) */
    /* untranslated: part[14:2](rgRes) = (part[14:2](rgRes) - HIWORD(cResearch)) */
    goto L_50fe;

L_50f4:
    cResearch = 0x0;

L_50fe:
    fAlchemy = 0x0;
    i = 0xffff;
    goto L_53c3;

L_510b:
    if ((i != 0xffff))
        goto L_5122;
    else
        goto L_5114;

L_5114:
    LOWORD(lpprod) = prodPartial;
    /* untranslated: HIWORD(lpprod) = ss */
    goto L_513c;

L_5122:
    lpprod = &(pl.lpplprod[i].rgprod);

L_513c:
    if ((lpprod->cItem != 0x0))
        goto L_515f;
    else
        goto L_5154;

L_5154:
    if (((*(lpprod + 0x2) & 0x0) == 0x0))
        goto L_53bf;
    else
        goto L_5159;

L_5159:

L_515f:
    if ((lpprod->iItem != 0x3))
        goto L_5202;
    else
        goto L_517f;

L_517f:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_5202 : L_5187 */

L_5187:
    if ((lpprod->grobj != 0x1))
        goto L_5202;
    else
        goto L_51a7;

L_51a7:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_5202 : L_51af */

L_51af:
    if ((i >= (iMac + 0xffff)))
        goto L_51e2;
    else
        goto L_51bd;

L_51bd:
    if ((i != iItem))
        goto L_51da;
    else
        goto L_51c8;

L_51c8:
    *(piLast) = 0xffff;
    *(piFirst) = 0xffff;
    goto LCleanUp;

L_51da:
    fAlchemy = 0x1;
    goto L_53bf;

L_51e2:
    lpprod->cItem = 0x3fc;
    *(lpprod + 0x2) = ((*(lpprod + 0x2) & 0xffff) | 0x0);

L_5202:
    if ((i != 0xffff))
        goto L_5220;
    else
        goto L_521a;

L_521a:
    t_merge_5223_0001 = 0x0;
    goto L_5223;

L_5220:
    t_merge_5223_0001 = prodPartial;

L_5223:
    cBuilt = CBuildProdItem(&(pl), lpprod, t_merge_5223_0001, rgRes, fAlchemy, &(mdStatus), 0x0);
    if ((iItem != i))
        goto L_52a2;
    else
        goto L_5247;

L_5247:
    if ((cBuilt <= 0x0))
        goto L_5263;
    else
        goto L_5250;

L_5250:
    if ((*(piFirst) != 0x0))
        goto L_5263;
    else
        goto L_525b;

L_525b:
    *(piFirst) = iPass;

L_5263:
    if ((mdStatus != 0x2))
        goto L_5285;
    else
        goto L_526c;

L_526c:
    if ((*(piFirst) == 0x0))
        goto LCleanUp;
    else
        goto L_5277;

L_5277:
    *(piLast) = (iPass + 0xffff);

L_5285:
    if ((mdStatus == 0x0))
        goto L_5297;
    else
        goto L_528e;

L_528e:
    if ((mdStatus != 0x1))
        goto L_52a2;
    else
        goto L_5297;

L_5297:
    *(piLast) = iPass;
    goto LCleanUp;

L_52a2:
    fAlchemy = 0x0;
    if ((lpprod->grobj != 0x1))
        goto L_53b3;
    else
        goto L_52c7;

L_52c7:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_53b3 : L_52cf */

L_52cf:
    goto L_5370;

L_52ea:
    /* untranslated: ss:[bp-0x74] = ((loword((int32_t)(sext16to32(cBuilt) << 0x8)) + part[20:2](pl)) & 0xff00) */
    /* untranslated: ss:[bp-0x72] = ((hiword((int32_t)(sext16to32(cBuilt) << 0x8)) + part[22:2](pl)) & 0xf) */
    /* untranslated: part[20:2](pl) = (part[20:2](pl) & 0xff) */
    /* untranslated: part[22:2](pl) = (part[22:2](pl) & 0xfff0) */
    /* untranslated: part[20:2](pl) = (part[20:2](pl) | ss:[bp-0x74]) */
    /* untranslated: part[22:2](pl) = (part[22:2](pl) | ss:[bp-0x72]) */
    goto L_53b3;

L_532c:
    /* untranslated: ss:[bp-0x74] = ((loword((int32_t)(sext16to32(cBuilt) << 0x14)) + part[20:2](pl)) & 0x0) */
    /* untranslated: ss:[bp-0x72] = ((hiword((int32_t)(sext16to32(cBuilt) << 0x14)) + part[22:2](pl)) & 0xfff0) */
    /* untranslated: part[20:2](pl) = (part[20:2](pl) & 0xffff) */
    pl.cFactories = 0x0;
    /* untranslated: part[20:2](pl) = (part[20:2](pl) | ss:[bp-0x74]) */
    /* untranslated: part[22:2](pl) = (part[22:2](pl) | ss:[bp-0x72]) */
    goto L_53b3;

L_5370:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) != 0x0 ? L_5380 : L_5378 */

L_5378:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) == 0x0 ? L_52ea : L_5380 */

L_5380:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) != 0x1 ? L_5390 : L_5388 */

L_5388:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) == 0x0 ? L_532c : L_5390 */

L_5390:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) != 0x7 ? L_53a0 : L_5398 */

L_5398:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) == 0x0 ? L_532c : L_53a0 */

L_53a0:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) != 0x8 ? L_53b3 : L_53a8 */

L_53a8:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) == 0x0 ? L_52ea : L_53b3 */

L_53b3:
    if ((mdStatus >= 0x5))
        goto L_53ce;
    else
        goto L_53b9;

L_53b9:

L_53bf:
    i = (i + 0x1);

L_53c3:
    if ((i < iMac))
        goto L_510b;
    else
        goto L_53ce;

L_53ce:
    if ((iItem >= iobjMine))
        goto L_53f9;
    else
        goto L_53d7;

L_53d7:
    /* untranslated: *piFirst = part[12:2](rgRes) */
    if ((iItem != 0xffff))
        goto LCleanUp;
    else
        goto L_53eb;

L_53eb:
    *(piFirst) = (*(piFirst) + LOWORD(cResearch));

L_53f9:
    j = 0x0;
    goto L_5427;

L_5401:
    pl.rgwtMin[j] = rgRes[j];
    j = (j + 0x1);

L_5427:
    if ((j < 0x3))
        goto L_5401;
    else
        goto L_5430;

L_5430:
    ChgPopFromPlanet(&(pl), 0x1);
    iPass = (iPass + 0x1);

L_5447:
    if ((iPass < 0x64))
        goto L_5018;
    else
        goto L_5450;

L_5450:
    if ((*(piFirst) != 0x0))
        goto L_5462;
    else
        goto L_545b;

L_545b:
    *(piFirst) = 0x64;

L_5462:
    *(piLast) = 0x64;

LCleanUp:
    if ((LOWORD(pl.lpplprod) != 0x0))
        goto L_547b;
    else
        goto L_5472;

L_5472:
    if ((HIWORD(pl.lpplprod) == 0x0))
        goto L_5489;
    else
        goto L_547b;

L_547b:
    FreePl(pl.lpplprod);

L_5489:
    return;
}

int16_t ZipProdDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    HDC         hdc;
    PAINTSTRUCT ps;
    int16_t     i;
    int16_t     iBase;
    RECT        rc;
    int16_t     dy;
    RECT        rc2;
    HWND        hwndRad;
    char       *psz;
    char       *pszT;
    RECT        rcGBox;
    int16_t     cch;
    int16_t (**lpProc)();
    int16_t  cpq;
    uint16_t t_merge_5926_0001;

L_5490:
    goto L_5db7;

L_549f:
    SetWindowText(hwnd, PszGetCompressedString(idsCustomizeProductionTemplates));
    GetWindowRect(hwnd, &(rc));
    GetClientRect(hwnd, &(rc2));
    dy = ((rc.bottom - rc.top) - rc2.bottom);
    GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0417), &(rc2));
    MapWindowPoints(0x0, hwnd, &(rc2), 0x2);
    vyZPDStatic = (rc2.bottom + 0x2);
    dy = (dy + ((rc2.bottom + dyArial8) + 0x6));
    SetWindowPos(hwnd, 0x0, 0x0, 0x0, (rc.right - rc.left), dy, 0x6);
    CheckRadioButton(hwnd, 0x431, 0x434, 0x431);
    EnableZipProdBtns(hwnd, 0x0);
    iResTechNow = 0x0;
    FillZipProdLB(hwnd, vrgZipProd[iResTechNow]);
    i = 0x431;
    goto L_565d;

L_5592:
    iBase = (i + 0xfbcf);
    if ((vrgZipProd[iBase].fValid == 0x0))
        goto L_560b;
    else
        goto L_55b4;

L_55b4:
    pszT = 0x57a4;
    psz = vrgZipProd[iBase];

L_55c7:
    if (((uint16_t)(*(psz)) == 0x0))
        goto L_55fd;
    else
        goto L_55d5;

L_55d5:
    psz = (psz + 0x1);
    pszT = (pszT + 0x1);
    *(pszT) = *(psz);
    if (((uint16_t)(*(psz)) != 0x26))
        goto L_55c7;
    else
        goto L_55f0;

L_55f0:
    pszT = (pszT + 0x1);
    *(pszT) = 0x26;

L_55fd:
    *(pszT) = 0x0;
    psz = 0x57a4;
    goto L_563c;

L_560b:
    psz = PszGetCompressedString(idsUnusedD);
    _wsprintf(szWork, psz, (iBase + 0x1));
    psz = 0x57a4;

L_563c:
    hwndRad = GetDlgItem(hwnd, i);
    SetWindowText(hwndRad, psz);
    i = (i + 0x1);

L_565d:
    if ((i <= 0x434))
        goto L_5592;
    else
        goto L_5667;

L_5667:
    StickyDlgPos(hwnd, 0xd00, 0x1);
    if ((gd.fTutorial == 0x0))
        goto L_5692;
    else
        goto L_568d;

L_568d:
    AdvanceTutor();

L_5692:
    return 0x1;

L_5698:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_56c0:
    i = 0x431;
    goto L_56ed;

L_56c8:
    /* untranslated: ss:[bp-0x32] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0x32] == GetDlgItem(hwnd, i) ? L_56f7 : L_56e3 */

L_56e3:

L_56e9:
    i = (i + 0x1);

L_56ed:
    if ((i <= 0x434))
        goto L_56c8;
    else
        goto L_56f7;

L_56f7:
    if ((i > 0x434))
        goto L_5de2;
    else
        goto L_5701;

L_5701:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_571a:
    hdc = BeginPaint(hwnd, &(ps));
    GetClientRect(hwnd, &(rc));
    GetWindowRect(GetDlgItem(hwnd, 0x431), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0434), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    SelectObject(hdc, HIWORD(rghfontArial8));
    SetBkColor(hdc, crButtonFace);
    cch = CchGetString(idsCustomOrders, 0x57a4);
    TextOut(hdc, (rcGBox.left + 0x8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, cch);
    rcGBox.top = (rcGBox.bottom + 0x8);
    if ((vrgZipProd[iResTechNow].fValid == 0x0))
        goto L_5874;
    else
        goto L_5836;

L_5836:
    /* untranslated: cch = CchGetString((part[14:1](vrgZipProd[iResTechNow]) + 0x4c6), 0x57a4) */
    TextOut(hdc, rcGBox.left, vyZPDStatic, szWork, cch);

L_5874:
    EndPaint(hwnd, &(ps));
    return 0x1;

L_5889:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_58ec;
    else
        goto L_58a5;

L_58a5:
    if ((wParam < 0x431))
        goto L_58ec;
    else
        goto L_58af;

L_58af:
    if ((wParam > 0x434))
        goto L_58ec;
    else
        goto L_58b9;

L_58b9:
    iResTechNow = (wParam + 0xfbcf);
    EnableZipProdBtns(hwnd, iResTechNow);
    FillZipProdLB(hwnd, vrgZipProd[iResTechNow]);
    goto L_5de2;

L_58ec:
    if ((wParam == 0x1))
        goto L_58fe;
    else
        goto L_58f5;

L_58f5:
    if ((wParam != 0x2))
        goto L_5953;
    else
        goto L_58fe;

L_58fe:
    StickyDlgPos(hwnd, 0xd00, 0x0);
    if ((wParam != 0x1))
        goto L_5923;
    else
        goto L_591d;

L_591d:
    t_merge_5926_0001 = 0x1;
    goto L_5926;

L_5923:
    t_merge_5926_0001 = 0x0;

L_5926:
    EndDialog(hwnd, t_merge_5926_0001);
    vyZPDStatic = 0xffff;
    if ((gd.fTutorial == 0x0))
        goto L_594a;
    else
        goto L_5945;

L_5945:
    AdvanceTutor();

L_594a:
    return 0x1;

L_5953:
    if ((wParam == 0x816))
        goto L_5967;
    else
        goto L_595d;

L_595d:
    if ((wParam != 0x41b))
        goto L_5d04;
    else
        goto L_5967;

L_5967:
    if ((vrgZipProd[iResTechNow].fValid == 0x0))
        goto L_599d;
    else
        goto L_5981;

L_5981:
    strcpy(0x57a4, vrgZipProd[iResTechNow]);
    goto L_59c0;

L_599d:
    _wsprintf(szWork, PszGetCompressedString(idsCustomD), iResTechNow);

L_59c0:
    lpProc = MakeProcInstance(RenameZipDlg, hInst);
    if ((iResTechNow == 0x0))
        goto LDontRename;
    else
        goto L_59de;

L_59de:

L_59e4:
    if ((DialogBox(hInst, IDD_RENAME, hwndFrame, &(lpProc)) == 0x0))
        goto L_5ce2;
    else
        goto L_5a07;

L_5a07:
    if (((uint16_t)(LOWORD(szWork)) != 0x0))
        goto L_5a36;
    else
        goto L_5a13;

L_5a13:
    _wsprintf(szWork, PszGetCompressedString(idsCustomD), iResTechNow);

L_5a36:
    strcpy(vrgZipProd[iResTechNow], 0x57a4);
    pszT = 0x57e4;
    psz = 0x57a4;

L_5a59:
    if (((uint16_t)(*(psz)) == 0x0))
        goto L_5a8f;
    else
        goto L_5a67;

L_5a67:
    psz = (psz + 0x1);
    pszT = (pszT + 0x1);
    *(pszT) = *(psz);
    if (((uint16_t)(*(psz)) != 0x26))
        goto L_5a59;
    else
        goto L_5a82;

L_5a82:
    pszT = (pszT + 0x1);
    *(pszT) = 0x26;

L_5a8f:
    *(pszT) = 0x0;
    SetWindowText(GetDlgItem(hwnd, (iResTechNow + 0x431)), "");

LDontRename:
    if ((wParam != 0x816))
        goto L_5cd3;
    else
        goto L_5abb;

L_5abb:
    vrgZipProd[iResTechNow].fValid = 0x1;
    cpq = 0x0;
    i = 0x0;
    goto L_5adc;

L_5ad8:
    i = (i + 0x1);

L_5adc:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_5c79;
    else
        goto L_5aef;

L_5aef:
    if (((LOWORD((uint32_t)((lpplProdGlob[i].rgprod[0x0] >> 0x11))) & 0x7) != 0x1))
        goto L_5ad8;
    else
        goto L_5b24;

L_5b24:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_5ad8 : L_5b2c */

L_5b2c:
    if (((HIWORD((uint32_t)((lpplProdGlob[i].rgprod[0x0] >> 0xa))) & 0x0) < 0x0))
        goto L_5b71;
    else
        goto L_5b61;

L_5b61:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) > 0x0 ? L_5ad8 : L_5b66 */

L_5b66:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) >= 0x7 ? L_5ad8 : L_5b6b */

L_5b6b:

L_5b71:
    /* untranslated: 0x25:[(part[16:0](vrgZipProd[iResTechNow]) + (cpq * 0x2))] = ((0x25:[(part[16:0](vrgZipProd[iResTechNow]) + (cpq * 0x2))] & 0xffc0) |
     * ((loword((uint32_t)(lpplProdGlob[i].rgprod[0x0] >> 0xa)) & 0x7f) & 0x3f)) */
    /* untranslated: 0x25:[(part[16:0](vrgZipProd[iResTechNow]) + (cpq * 0x2))] = ((0x25:[(part[16:0](vrgZipProd[iResTechNow]) + (cpq * 0x2))] & 0x3f) |
     * (((LOWORD(lpplProdGlob[i].rgprod[0x0]) & 0x3ff) & 0x3ff) * 0x40)) */
    cpq = (cpq + 0x1);
    if ((cpq >= 0xc))
        goto L_5c79;
    else
        goto L_5c70;

L_5c70:

L_5c79:
    /* untranslated: part[15:1](vrgZipProd[iResTechNow]) = lobyte(cpq) */
    /* untranslated: part[14:1](vrgZipProd[iResTechNow]) = lobyte((loword((uint32_t)(words(part[176:2](sel), part[178:2](sel)) >> 0x17)) & 0x1)) */
    FillZipProdLB(hwnd, vrgZipProd[iResTechNow]);

L_5cd3:
    EnableZipProdBtns(hwnd, iResTechNow);

L_5ce2:
    FreeProcInstance(&(lpProc));
    SetFocus(hwnd);
    gd.fChgZipProd = 0x1;
    goto L_5de2;

L_5d04:
    if ((wParam != 0x817))
        goto L_5d87;
    else
        goto L_5d0e;

L_5d0e:
    vrgZipProd[iResTechNow].fValid = 0x0;
    _wsprintf(szWork, PszGetCompressedString(idsUnusedD), (iResTechNow + 0x1));
    SetWindowText(GetDlgItem(hwnd, (iResTechNow + 0x431)), szWork);
    FillZipProdLB(hwnd, vrgZipProd[iResTechNow]);
    gd.fChgZipProd = 0x1;
    goto L_5de2;

L_5d87:
    if ((wParam != 0x76))
        goto L_5de2;
    else
        goto L_5d90;

L_5d90:
    WinHelp(hwnd, szHelpFile, 0x1, 0x452);
    return 0x1;

L_5db7:
    if ((message == WM_PAINT))
        goto L_571a;
    else
        goto L_5dbf;

L_5dbf:
    if ((message == WM_ERASEBKGND))
        goto L_5698;
    else
        goto L_5dc7;

L_5dc7:
    if ((message == WM_CTLCOLOR))
        goto L_56c0;
    else
        goto L_5dcf;

L_5dcf:
    if ((message == WM_INITDIALOG))
        goto L_549f;
    else
        goto L_5dd7;

L_5dd7:
    if ((message == WM_COMMAND))
        goto L_5889;
    else
        goto L_5de2;

L_5de2:
    return 0x0;
}

void EnableZipProdBtns(HWND hwnd, int16_t iSel) {
    int16_t  fEnabled;
    uint16_t t_merge_5e24_0001;

L_5df0:
    if ((vrgZipProd[iSel].fValid == 0x0))
        goto L_5e21;
    else
        goto L_5e12;

L_5e12:
    if ((iSel <= 0x0))
        goto L_5e21;
    else
        goto L_5e1b;

L_5e1b:
    t_merge_5e24_0001 = 0x1;
    goto L_5e24;

L_5e21:
    t_merge_5e24_0001 = 0x0;

L_5e24:
    fEnabled = t_merge_5e24_0001;
    EnableWindow(GetDlgItem(hwnd, IDC_DELETE), fEnabled);
    EnableWindow(GetDlgItem(hwnd, IDC_RENAME), fEnabled);
    return;
}

void FillZipProdLB(HWND hwndDlg, ZIPPRODQ *pzpq) {
    int16_t i;
    HWND    hwndLB;
    char    szAuto[40];
    char    szFormat[15];
    RECT    rc;

L_5e58:
    hwndLB = GetDlgItem(hwndDlg, IDC_U16_0x0417);
    GetClientRect(hwndDlg, &(rc));
    rc.top = vyZPDStatic;
    rc.bottom = (vyZPDStatic + dyArial8);
    InvalidateRect(hwndDlg, &(rc), 0x1);
    SendMessage(hwndLB, CB_DIR, 0x0, 0x0);
    if ((pzpq->fValid == 0x0))
        goto L_5ed9;
    else
        goto L_5ec8;

L_5ec8:
    if ((pzpq->cpq != 0x0))
        goto L_5efc;
    else
        goto L_5ed9;

L_5ed9:
    /* untranslated: call SendMessage(hwndLB, CB_LIMITTEXT, 0x0, words(0x25, PszGetCompressedString(idsAutoBuildOrders))) -> callresult(LRESULT) */
    goto L_5fee;

L_5efc:
    CchGetString(idsSD2, szFormat);
    i = 0x0;
    goto L_5f18;

L_5f14:
    i = (i + 0x1);

L_5f18:
    if ((i >= pzpq->cpq))
        goto L_5fee;
    else
        goto L_5f29;

L_5f29:
    /* untranslated: call CchGetString(((0x25:[((pzpq + 0x10) + (i * 0x2))] & 0x3f) + 0x7e), szAuto) -> callresult(int16_t) */
    /* untranslated: branch ((0x25:[((pzpq + 0x10) + (i * 0x2))] >> 0x6) & 0x3ff) == 0x1 ? L_5f8a : L_5f6e */

L_5f6e:
    /* untranslated: branch (0x25:[((pzpq + 0x10) + (i * 0x2))] & 0x3f) != 0x3 ? L_5f9d : L_5f8a */

L_5f8a:
    strcpy(0x57a4, szAuto);
    goto L_5fd4;

L_5f9d:
    /* untranslated: call _wsprintf(szWork, &szFormat, szAuto, ss, ((0x25:[((pzpq + 0x10) + (i * 0x2))] >> 0x6) & 0x3ff)) -> callresult(int16_t) */

L_5fd4:
    SendMessage(hwndLB, CB_LIMITTEXT, 0x0, 0x2557a4);
    goto L_5f14;

L_5fee:
    return;
}
