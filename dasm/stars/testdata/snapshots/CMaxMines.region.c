int16_t CMaxMines(PLANET *lppl, int16_t iplr) {
    int32_t cMax;
    int32_t lPopMax;
    int16_t iEff;

    lPopMax = CalcPlanetMaxPop(lppl->id, iplr);
    iEff = GetRaceStat(&rgplr[iplr], rsMineOperate);
    cMax = (int32_t)(lPopMax * iEff) / 100;
    if (cMax < 10) {
        cMax = 10;
    }
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh) {
        cMax = 0;
    }
    return LOWORD(cMax);
}
