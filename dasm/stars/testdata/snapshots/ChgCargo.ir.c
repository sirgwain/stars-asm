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
