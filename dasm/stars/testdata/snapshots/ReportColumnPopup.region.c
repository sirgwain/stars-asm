void ReportColumnPopup(POINT16 pt, int16_t icol, int16_t fRightBtn) {
    HDC     hdc;
    char    szT[50];
    char    rgsz[32][50];
    int16_t iBase;
    int16_t cSubsort;
    int16_t j;
    int16_t i;
    int16_t ibit;
    int16_t fccolChange;
    int16_t rgcol[32];
    char    szColTitle[50];
    int16_t cItems;
    char   *psz[32];
    int16_t cch;
    int16_t iRet;
    int16_t iHide;
    int16_t iSortLast;

    cSubsort = 0;
    fccolChange = 0;
    hdc = GetDC(hwndReportDlg);
    DxReportColHdr(vprptCur->irpt, icol, szColTitle, hdc);
    cItems = 0;
    for (i = 0; i < 2; i++) {
        cch = CchGetString(i == 0 ? idsSort : idsReverseSort, rgsz[cItems]);
        strcpy(&rgsz[cItems][cch], szColTitle);
        cItems++;
        if (vprptCur->irpt == rptPlanets) {
            switch (icol) {
            default:
                goto L_759a;
            case colPlanetMinConc:
            case colPlanetMinerals:
            case colPlanetMiningRate:
                goto L_75b0;
            }
            continue;
        }
    L_759a:
        if (vprptCur->irpt != rptFleets || icol != colFleetCargo)
            continue;
    L_75b0:
        strcpy(rgsz[cItems], rgsz[cItems - 1]);
        rgsz[cItems - 1][0] = 0;
        cItems++;
        for (j = 0; j < (vprptCur->irpt == rptFleets) + 3; j++) {
            strcpy(rgsz[cItems], rgszMinerals[j]);
            cItems++;
        }
        rgsz[cItems][0] = -1;
        rgsz[cItems][1] = 0;
        cItems++;
        strcpy(rgsz[cItems], PszGetCompressedString(idsWeightedAverage));
        cItems++;
        rgsz[cItems++][0] = 0;
        cSubsort = 5;
    }
    rgsz[cItems][0] = -1;
    rgsz[cItems][1] = 0;
    cItems++;
    cch = CchGetString(idsHide, rgsz[cItems]);
    strcpy(&rgsz[cItems][cch], szColTitle);
    cch = CchGetString(idsColumn, szT);
    strcat(rgsz[cItems], szT);
    iHide = cItems;
    iSortLast = cItems;
    cItems++;
    rgsz[cItems][0] = -1;
    rgsz[cItems][1] = 0;
    cItems++;
    if (icol == 0) {
        cItems -= 2;
        iHide = -1;
    }
    iBase = cItems;
    i = 0;
    ibit = 1;
    while (i < vprptCur->cFields) {
        if ((ibit & vprptCur->grbitVisible) == 0) {
            DxReportColHdr(vprptCur->irpt, i, szColTitle, hdc);
            cch = CchGetString(idsShow, rgsz[cItems]);
            strcpy(&rgsz[cItems][cch], szColTitle);
            cch = CchGetString(idsColumn, szT);
            strcat(rgsz[cItems], szT);
            rgcol[cItems] = i;
            cItems++;
        }
        i++;
        ibit *= 2;
    }
    if (cItems == iBase) {
        cItems--;
    }
    ReleaseDC(hwndReportDlg, hdc);
    for (i = 0; i < cItems; i++) {
        if (rgsz[i][0] != 0) {
            psz[i] = rgsz[i];
        } else {
            psz[i] = 0;
        }
    }
    iRet = PopupMenu(hwndReportDlg, pt.x, pt.y, cItems, NULL, psz, -1, fRightBtn);
    if (iRet >= 0) {
        gd.fChgReports = TRUE;
        if (iRet < iSortLast) {
            vicolSortPrev = vprptCur->icolSort;
            viSubsortPrev = vprptCur->iSubsort;
            vfAscendingPrev = vprptCur->fAscending;
            vprptCur->icolSort = icol;
            if (cSubsort == 0) {
                vprptCur->fAscending = iRet == 0;
            } else {
                vprptCur->iSubsort = (int16_t)(iRet - 2) % (cSubsort + 3 + (vprptCur->irpt == rptFleets));
                if (vprptCur->iSubsort > (vprptCur->irpt == rptFleets) + 3) {
                    vprptCur->iSubsort = (vprptCur->irpt == rptFleets) + 3;
                }
                vprptCur->fAscending = iRet < cSubsort + 2;
            }
            SortReportCache(vprptCur->irpt, icol);
        } else if (iRet == iHide) {
            fccolChange = 1;
            vprptCur->grbitVisible &= (int16_t)~(1 << icol);
        } else if (iRet >= iBase) {
            fccolChange = 1;
            vprptCur->grbitVisible |= (int16_t)(1 << rgcol[iRet]);
        }
        if (fccolChange != 0) {
            SetHScrollBar();
        }
        InvalidateRect(hwndReportDlg, NULL, TRUE);
    }
    return;
}
