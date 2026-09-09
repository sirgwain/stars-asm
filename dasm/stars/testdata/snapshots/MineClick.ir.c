void MineClick(int16_t x, int16_t y, int16_t msg, int16_t sks) {
    PLANET    *lppl;
    HtMineType ht;
    PART       part;
    char       rgsz[16][32];
    char      *rgpsz[16];
    int16_t    c;
    FLEET     *lpfl;
    int16_t    rgid[16];
    int16_t    ishdef;
    int16_t    rgMin[3];
    PLANET     pl;
    int16_t    rgCost[3];
    int16_t    rgMax[3];
    int16_t    fOurs;
    int16_t    i;
    int16_t    idNew;
    SCAN       scan;
    int16_t    rgi[9];
    int16_t    iChecked;
    char      *psz[9];
    int32_t    rglQuan[3];
    int32_t    rglT[3];
    int16_t    ifl;
    int32_t    cMines;
    int32_t    lVal;
    uint16_t   t_scratch_m12;
    uint16_t   t_merge_3e20_0001;
    uint16_t   t_merge_3e7b_0001;
    uint16_t   t_merge_3ef6_0001;
    uint16_t   t_merge_41a7_0001;
    uint16_t   t_merge_4272_0001;
    uint16_t   t_merge_42ed_0001;
    uint16_t   t_merge_43d6_0001;

L_3b4e:
    ht = HtMineWindow(hwndMine, x, y);
    if ((msg != 516))
        goto L_3b8b;
    else
        goto L_3b76;

L_3b76:
    if ((ht == htMineScanSel))
        goto L_3b8b;
    else
        goto L_3b7f;

L_3b7f:
    if ((ht != htMineShipOrFleet))
        goto L_47d5;
    else
        goto L_3b8b;

L_3b8b:
    goto L_47a6;

L_3b94:
    if ((sel.scan.grobj != grobjPlanet))
        goto L_3bbd;
    else
        goto L_3b9e;

L_3b9e:
    lppl = LpplFromId(sel.scan.idpl);
    HIWORD(GlobalPD) = lppl->iPlayer;
    goto L_3c0f;

L_3bbd:
    if ((sel.scan.grobj != grobjThing))
        goto L_3beb;
    else
        goto L_3bc7;

L_3bc7:
    HIWORD(GlobalPD) = lpThings[sel.scan.ith].iplr;
    goto L_3c0f;

L_3beb:
    HIWORD(GlobalPD) = rglpfl[sel.scan.ifl]->iplr;

L_3c0f:
    GlobalPD.grPopup = grPopupPlayer;
    Popup(hwndMine, x, y);
    goto L_47d5;

L_3c2a:
    /* untranslated: t_scratch_m12 = mpiTypeiItem[part[0xc:1](lpThings[sel.scan.ith])] */
    part.hs.iItem = t_scratch_m12;
    part.hs.grhst = hstMines;
    FLookupPart(&(part));
    GlobalPD.grPopup = grPopupComponent;
    GlobalPD.part = part;
    Popup(hwndMine, x, y);
    goto L_47d5;

L_3c9f:
    if ((msg != 516))
        goto L_3e38;
    else
        goto L_3ca9;

L_3ca9:
    lpfl = rglpfl[sel.scan.ifl];
    c = 0;
    ishdef = 0;
    goto L_3d7c;

L_3cda:
    if ((lpfl->rgcsh[ishdef] <= 0))
        goto L_3d77;
    else
        goto L_3cfa;

L_3cfa:
    rgid[c] = ishdef;
    fstrcpy(rgsz[c], rglpshdef[lpfl->iPlayer][ishdef].hul.szClass);
    rgpsz[c] = (&(rgsz) + (c * 32));
    c = (c + 1);

L_3d77:
    ishdef = (ishdef + 1);

L_3d7c:
    if ((ishdef < 16))
        goto L_3cda;
    else
        goto L_3d86;

L_3d86:
    if ((c <= 1))
        goto L_3dcb;
    else
        goto L_3d90;

L_3d90:
    c = PopupMenu(hwndMine, x, y, c, 0x0, rgpsz, -1, 1);
    if ((c == -1))
        goto L_47d5;
    else
        goto L_3dc2;

L_3dc2:
    goto L_3dd1;

L_3dcb:
    c = 0;

L_3dd1:
    GlobalPD.grPopup = grPopupShdef;
    GlobalPD.lpshdef = (rglpshdef[lpfl->iPlayer] + LOWORD((0x93 * rgid[c])));
    if ((idPlayer == lpfl->iPlayer))
        goto L_3e1d;
    else
        goto L_3e17;

L_3e17:
    t_merge_3e20_0001 = 0x1;
    goto L_3e20;

L_3e1d:
    t_merge_3e20_0001 = 0x0;

L_3e20:
    /* untranslated: part[0x8:2](GlobalPD) = t_merge_3e20_0001 */
    /* untranslated: part[0x6:2](GlobalPD) = 0x0 */
    /* untranslated: part[0xa:4](GlobalPD) = 0x10000 */
    goto L_3e84;

L_3e38:
    GlobalPD.grPopup = grPopupFleet;
    GlobalPD.lpfl = rglpfl[sel.scan.ifl];
    if ((GlobalPD.lpfl->det != 0x7))
        goto L_3e78;
    else
        goto L_3e72;

L_3e72:
    t_merge_3e7b_0001 = 0x1;
    goto L_3e7b;

L_3e78:
    t_merge_3e7b_0001 = 0x0;

L_3e7b:
    /* untranslated: part[0x6:2](GlobalPD) = t_merge_3e7b_0001 */
    GlobalPD.grbit = 0xff;

L_3e84:
    Popup(hwndMine, x, y);
    goto L_47d5;

L_3e99:
    GlobalPD.grPopup = grPopupShdef;
    lppl = LpplFromId(sel.scan.idpl);
    /* untranslated: part[0x2:4](GlobalPD) = (rglpshdefSB[lppl->iPlayer] + loword((lppl->isb * 0x93))) */
    if ((idPlayer == lppl->iPlayer))
        goto L_3ef3;
    else
        goto L_3eed;

L_3eed:
    t_merge_3ef6_0001 = 0x1;
    goto L_3ef6;

L_3ef3:
    t_merge_3ef6_0001 = 0x0;

L_3ef6:
    /* untranslated: part[0x8:2](GlobalPD) = t_merge_3ef6_0001 */
    /* untranslated: part[0x6:2](GlobalPD) = 0x1 */
    /* untranslated: part[0xa:4](GlobalPD) = 0x10000 */
    Popup(hwndMine, x, y);
    goto L_47d5;

L_3f20:
    GlobalPD.grPopup = grPopupPlanet;
    HIWORD(GlobalPD) = sel.scan.idpl;
    Popup(hwndMine, x, y);
    goto L_47d5;

L_3f41:
    FLookupPlanet(sel.scan.idpl, &(pl));
    GlobalPD.grPopup = grPopupPlanetEnv;
    HIWORD(GlobalPD) = pl.id;
    /* untranslated: part[0x4:2](GlobalPD) = (ht - 6) */
    if ((pl.det < 0x3))
        goto L_3f85;
    else
        goto L_3f74;

L_3f74:
    /* untranslated: part[0x6:2](GlobalPD) = sext8to16(pl.rgEnvVar[part[0x4:2](GlobalPD)]) */
    goto L_3f8b;

L_3f85:
    /* untranslated: part[0x6:2](GlobalPD) = 0xffff */

L_3f8b:
    if ((pl.det < 0x3))
        goto NoTerra;
    else
        goto L_3f99;

L_3f99:
    if ((FCanTerraformLppl(&(pl), rgMin, rgMax, rgCost, 1) == 0))
        goto NoTerra;
    else
        goto L_3fc0;

L_3fc0:
    /* untranslated: part[0x8:2](GlobalPD) = rgMin[part[0x4:2](GlobalPD)] */
    /* untranslated: GlobalPD.grbit = rgMax[part[0x4:2](GlobalPD)] */
    /* untranslated: branch part[0x8:2](GlobalPD) != 0xffff ? L_3fee : L_3fe8 */

L_3fe8:
    /* untranslated: part[0x8:2](GlobalPD) = part[0x6:2](GlobalPD) */

L_3fee:
    if ((GlobalPD.grbit != 0xffff))
        goto L_3ffe;
    else
        goto L_3ff8;

L_3ff8:
    /* untranslated: GlobalPD.grbit = part[0x6:2](GlobalPD) */

L_3ffe:
    /* untranslated: branch part[0x8:2](GlobalPD) != GlobalPD.grbit ? L_401c : L_4010 */

NoTerra:
    /* untranslated: part[0x8:4](GlobalPD) = 0xffffffff */

L_401c:
    /* untranslated: part[0xc:2](GlobalPD) = sext8to16(rgplr[idPlayer].rgEnvVar[part[0x4:2](GlobalPD)]) */
    /* untranslated: part[0xe:2](GlobalPD) = sext8to16(rgplr[idPlayer].rgEnvVarMin[part[0x4:2](GlobalPD)]) */
    /* untranslated: GlobalPD.iPlrMax = sext8to16(rgplr[idPlayer].rgEnvVarMax[part[0x4:2](GlobalPD)]) */
    Popup(hwndMine, x, y);
    goto L_47d5;

L_4085:
    if ((msg != 516))
        goto L_40a4;
    else
        goto L_408f;

L_408f:
    PopupMineralScanChoices(hwndMine, x, y);
    goto L_47d5;

L_40a4:
    scan = sel.scan;
    scan.iwp = 0;
    if ((scan.grobj != grobjThing))
        goto L_4192;
    else
        goto L_40c2;

L_40c2:
    i = (scan.ith + 1);

CheckThing:
    goto L_4113;

L_40ce:
    if ((lpThings[i].pt.x != scan.pt.x))
        goto L_410f;
    else
        goto L_4103;

L_4103:
    if ((lpThings[i].pt.y == scan.pt.y))
        goto L_411e;
    else
        goto L_410f;

L_410f:
    i = (i + 1);

L_4113:
    if ((i < cThing))
        goto L_40ce;
    else
        goto L_411e;

L_411e:
    if ((i >= cThing))
        goto L_4154;
    else
        goto L_4129;

L_4129:
    scan.ith = i;
    scan.grobj = grobjThing;
    idNew = lpThings[i].idFull;
    fOurs = 0;
    goto ChangeIt;

L_4154:
    if (((scan.grobjFull & 0x1) != 0x0))
        goto CheckPlanet;
    else
        goto L_4168;

L_4168:
    if (((scan.grobjFull & 0x2) != 0x0))
        goto CheckFleet;
    else
        goto L_417c;

L_417c:
    if (((scan.grobjFull & 0x8) == 0x0))
        goto L_4192;
    else
        goto L_418a;

L_418a:
    i = 0;
    goto L_4113;

L_4192:
    if ((scan.grobj != grobjFleet))
        goto L_41a4;
    else
        goto L_419b;

L_419b:
    t_merge_41a7_0001 = (scan.ifl + 1);
    goto L_41a7;

L_41a4:
    t_merge_41a7_0001 = 0x0;

L_41a7:
    i = t_merge_41a7_0001;
    goto L_41b1;

L_41ad:
    i = (i + 1);

L_41b1:
    if ((i >= cFleet))
        goto L_4215;
    else
        goto L_41bc;

L_41bc:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_41ec;
    else
        goto L_41e4;

L_41e4:
    if ((HIWORD(rglpfl[i]) == 0x0))
        goto L_4215;
    else
        goto L_41ec;

L_41ec:
    if ((scan.pt.x != lpfl->pt.x))
        goto L_41ad;
    else
        goto L_4206;

L_4206:
    if ((scan.pt.y != lpfl->pt.y))
        goto L_41ad;
    else
        goto L_4215;

L_4215:
    if ((i >= cFleet))
        goto L_4278;
    else
        goto L_4220;

L_4220:
    scan.ifl = i;
    scan.grobj = grobjFleet;
    idNew = rglpfl[i]->id;
    if ((rglpfl[i]->iPlayer != idPlayer))
        goto L_426f;
    else
        goto L_4269;

L_4269:
    t_merge_4272_0001 = 0x1;
    goto L_4272;

L_426f:
    t_merge_4272_0001 = 0x0;

L_4272:
    fOurs = t_merge_4272_0001;
    goto ChangeIt;

L_4278:
    if (((scan.grobjFull & 0x8) == 0x0))
        goto L_4291;
    else
        goto L_4286;

L_4286:
    i = 0;
    goto L_4113;

L_4291:
    if (((scan.grobjFull & 0x1) == 0x0))
        goto CheckFleet;
    else
        goto CheckPlanet;

CheckPlanet:
    scan.grobj = grobjPlanet;
    idNew = scan.idpl;
    lppl = LpplFromId(idNew);
    if ((LOWORD(lppl) != 0x0))
        goto L_42d5;
    else
        goto L_42c4;

L_42c4:
    if ((HIWORD(lppl) != 0x0))
        goto L_42d5;
    else
        goto L_42cd;

L_42cd:
    fOurs = 0;
    goto ChangeIt;

L_42d5:
    if ((lppl->iPlayer != idPlayer))
        goto L_42ea;
    else
        goto L_42e4;

L_42e4:
    t_merge_42ed_0001 = 0x1;
    goto L_42ed;

L_42ea:
    t_merge_42ed_0001 = 0x0;

L_42ed:
    fOurs = t_merge_42ed_0001;

L_42f0:
    goto ChangeIt;

CheckFleet:
    i = 0;
    goto L_42ff;

L_42fb:
    i = (i + 1);

L_42ff:
    if ((i >= cFleet))
        goto L_4363;
    else
        goto L_430a;

L_430a:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_433a;
    else
        goto L_4332;

L_4332:
    if ((HIWORD(rglpfl[i]) == 0x0))
        goto L_4363;
    else
        goto L_433a;

L_433a:
    if ((scan.pt.x != lpfl->pt.x))
        goto L_42fb;
    else
        goto L_4354;

L_4354:
    if ((scan.pt.y != lpfl->pt.y))
        goto L_42fb;
    else
        goto L_4363;

L_4363:
    if ((i != cFleet))
        goto L_4384;
    else
        goto L_436e;

L_436e:
    if (((scan.grobjFull & 0x8) == 0x0))
        goto L_4384;
    else
        goto L_437c;

L_437c:
    i = 0;
    goto L_4113;

L_4384:
    scan.grobj = grobjFleet;
    scan.ifl = i;
    idNew = rglpfl[i]->id;
    if ((rglpfl[i]->iPlayer != idPlayer))
        goto L_43d3;
    else
        goto L_43cd;

L_43cd:
    t_merge_43d6_0001 = 0x1;
    goto L_43d6;

L_43d3:
    t_merge_43d6_0001 = 0x0;

L_43d6:
    fOurs = t_merge_43d6_0001;

ChangeIt:
    if ((fOurs == 0))
        goto L_43ec;
    else
        goto L_43e2;

L_43e2:
    if ((sel.grobj == grobjFleet))
        goto L_43f2;
    else
        goto L_43ec;

L_43ec:
    scan.iwp = sel.scan.iwp;

L_43f2:
    ChangeScanSel(&(scan), 2);
    if ((fOurs == 0))
        goto L_47d5;
    else
        goto L_440b;

L_440b:
    RedrawScanSel(0x0, 0);
    ChangeMainObjSel(scan.grobj, idNew);
    RedrawScanSel(0x0, 1);

L_4439:
    goto L_47d5;

L_443c:
    iChecked = -1;
    /* untranslated: part[0x0:4](rgi) = 0x1f40064 */
    rgi[2] = 1000;
    rgi[4] = 5000;
    rgi[6] = 10000;
    rgi[8] = 30000;
    i = 0;
    goto L_44d6;

L_4476:
    _wsprintf(rgsz[i], PCTDKT, rgi[i]);
    psz[i] = (&(rgsz) + LOWORD((0xa * i)));
    if ((rgi[i] != cMinGrafMax))
        goto L_44d2;
    else
        goto L_44cc;

L_44cc:
    iChecked = i;

L_44d2:
    i = (i + 1);

L_44d6:
    if ((i < 9))
        goto L_4476;
    else
        goto L_44df;

L_44df:
    i = PopupMenu(hwndMine, x, y, 9, 0x0, psz, iChecked, 1);
    if ((i == -1))
        goto L_47d5;
    else
        goto L_4511;

L_4511:
    if ((rgi[i] == cMinGrafMax))
        goto L_47d5;
    else
        goto L_4525;

L_4525:
    cMinGrafMax = rgi[i];
    InvalidateRect(hwndMine, 0x0, 1);
    if (((grbitScan & 0xf) != 0x1))
        goto L_47d5;
    else
        goto L_4557;

L_4557:
    InvalidateRect(hwndScanner, 0x0, 1);

L_456c:
    goto L_47d5;

L_456f:
    FLookupPlanet(sel.scan.idpl, &(pl));
    GlobalPD.grPopup = grPopupMineral;
    /* untranslated: part[0x2:4](GlobalPD) = sext16to32((ht + 0xffff)) */
    i = 1;
    goto L_45b4;

L_459b:
    LOWORD(GlobalPD.rgi[i]) = 0xffff;
    HIWORD(GlobalPD.rgi[i]) = 0xffff;
    i = (i + 1);

L_45b4:
    if ((i <= 4))
        goto L_459b;
    else
        goto L_45bd;

L_45bd:
    if ((pl.det < 0x3))
        goto L_478e;
    else
        goto L_45cb;

L_45cb:
    /* untranslated: part[0xe:2](GlobalPD) = pl.rgpctMinLevel[(ht + 2)] */
    GlobalPD.iPlrMax = 0;
    /* untranslated: part[0x6:4](GlobalPD) = pl.fHomeworld */
    if ((pl.det <= 0x3))
        goto L_478e;
    else
        goto L_4608;

L_4608:
    lVal = 0;
    /* untranslated: part[0xa:4](GlobalPD) = pl.rgwtMin[(ht - 1)] */
    EstMineralsMined(&(pl), rglQuan, -1, 0);
    GlobalPD.rgi[4] = rglQuan[(ht - 1)];
    if ((pl.iPlayer != -1))
        goto L_478e;
    else
        goto L_4670;

L_4670:
    ifl = 0;
    goto L_467c;

L_4678:
    ifl = (ifl + 1);

L_467c:
    if ((ifl >= cFleet))
        goto L_476a;
    else
        goto L_4687;

L_4687:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_46b7;
    else
        goto L_46af;

L_46af:
    if ((HIWORD(rglpfl[ifl]) == 0x0))
        goto L_476a;
    else
        goto L_46b7;

L_46b7:
    if ((lpfl->idPlanet != pl.id))
        goto L_4678;
    else
        goto L_46c6;

L_46c6:
    if ((lpfl->iPlayer != idPlayer))
        goto L_4678;
    else
        goto L_46d5;

L_46d5:
    if ((lpfl->fDead != 0x0))
        goto L_4678;
    else
        goto L_46ec;

L_46ec:
    if ((lpfl->lpplord->rgord[0].grTask != grTaskMine))
        goto L_4678;
    else
        goto L_4705;

L_4705:
    cMines = CMineFromLpfl(lpfl);
    if ((HIWORD(cMines) < 0x0))
        goto L_4678;
    else
        goto L_4722;

L_4722:
    if ((HIWORD(cMines) > 0x0))
        goto L_4730;
    else
        goto L_4727;

L_4727:
    if ((LOWORD(cMines) <= 0x0))
        goto L_4678;
    else
        goto L_4730;

L_4730:
    EstMineralsMined(&(pl), rglT, cMines, 0);
    lVal = (lVal + rglT[(ht - 1)]);

L_4767:
    goto L_4678;

L_476a:
    if ((HIWORD(lVal) < 0x0))
        goto L_478e;
    else
        goto L_4773;

L_4773:
    if ((HIWORD(lVal) > 0x0))
        goto L_4781;
    else
        goto L_4778;

L_4778:
    if ((LOWORD(lVal) <= 0x0))
        goto L_478e;
    else
        goto L_4781;

L_4781:
    GlobalPD.rgi[4] = lVal;

L_478e:
    Popup(hwndMine, x, y);
    goto L_47d5;

L_47a6:
    if ((ht > htMineMinefieldType))
        goto L_47d5;
    else
        goto L_47ae;

L_47ae:
    switch ((ht * 0x2)) {
    case 0x0:
        goto L_47d5;
    case 0x2:
        goto L_456f;
    case 0x4:
        goto L_456f;
    case 0x6:
        goto L_456f;
    case 0x8:
        goto L_47d5;
    case 0xa:
        goto L_443c;
    case 0xc:
        goto L_3f41;
    case 0xe:
        goto L_3f41;
    case 0x10:
        goto L_3f41;
    case 0x12:
        goto L_4085;
    case 0x14:
        goto L_3b94;
    case 0x16:
        goto L_3c9f;
    case 0x18:
        goto L_3f20;
    case 0x1a:
        goto L_3e99;
    case 0x1c:
        goto L_3c2a;
    }

L_47d5:
    return;
}
