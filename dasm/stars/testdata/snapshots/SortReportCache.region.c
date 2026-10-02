void SortReportCache(ReportType irpt, int16_t icol) {
    uint16_t rgidRep[1024];
    PLANET  *lpplMac;
    int16_t  cRows;
    uint16_t iItem;
    PLANET  *lppl;
    FLEET   *lpfl;
    int16_t  i;

    cRows = 0;
    iItem = 0;
    if (vprptCur->icolSort != icol) {
        vicolSortPrev = vprptCur->icolSort;
        viSubsortPrev = vprptCur->iSubsort;
        vfAscendingPrev = vprptCur->fAscending;
        vprptCur->icolSort = icol;
        gd.fChgReports = 1;
    }
    if (hwndReportDlg != 0 || vprptCur->fCached == 0) {
        switch (irpt) {
        case rptFleets:
            vlprgidRep = vlprgidFleet;
            for (iItem = 0; (int16_t)iItem < cFleet; iItem++) {
                lpfl = rglpfl[iItem];
                if (rglpfl[iItem] == 0)
                    break;
                if (lpfl->iplr == idPlayer) {
                    rgidRep[cRows++] = iItem;
                }
            }
            break;
        case rptEnemyFleets:
            vlprgidRep = vlprgidMisc;
            vrptBattle.fCached = 0;
            for (iItem = 0; (int16_t)iItem < cFleet; iItem++) {
                lpfl = rglpfl[iItem];
                if (rglpfl[iItem] == 0)
                    break;
                if (lpfl->iplr != idPlayer) {
                    rgidRep[cRows++] = iItem;
                }
                if (cRows >= 1020)
                    break;
            }
            break;
        case rptPlanets:
            vlprgidRep = vlprgidPlanet;
            lppl = lpPlanets;
            lpplMac = lpPlanets + cPlanet;
            for (; lppl < lpplMac; lppl++) {
                if (lppl->iPlayer == idPlayer && lppl->det == detAll) {
                    rgidRep[cRows++] = iItem;
                }
                iItem++;
            }
            break;
        case rptBattles:
            vlprgidRep = vlprgidMisc;
            vrptEFleet.fCached = 0;
            cRows = CBattles();
            for (i = 0; i < cRows; i++) {
                rgidRep[i] = i;
            }
            break;
        default:
            return;
        }
        vprptCur->cRows = cRows;
        qsort(rgidRep, cRows, sizeof(uint16_t), (QSORTCOMPARE)ICompReport);
        fmemcpy(vlprgidRep, rgidRep, cRows * 2);
        vprptCur->fCached = 1;
    }
    return;
}
