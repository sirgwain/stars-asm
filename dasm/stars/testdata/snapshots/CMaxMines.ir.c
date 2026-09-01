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
    cMax = 10;

L_72ca:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raMacintosh))
        goto L_72f4;
    else
        goto L_72ea;

L_72ea:
    cMax = 0;

L_72f4:

L_72fd:
    return LOWORD(cMax);
}
