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

L_74d4:
    cSubsort = 0;
    fccolChange = 0;
    hdc = GetDC(hwndReportDlg);
    DxReportColHdr(vprptCur->irpt, icol, szColTitle, hdc);
    cItems = 0;
    i = 0;
    goto L_76bb;

L_751e:
    cch = CchGetString(i == 0 ? idsSort : idsReverseSort, rgsz[cItems]);
    strcpy(&rgsz[cItems][cch], szColTitle);
    cItems++;
    if (vprptCur->irpt != rptPlanets)
        goto L_759a;
    else
        goto L_757f;

L_757f:
    if (icol == colPlanetMinConc)
        goto L_75b0;
    else
        goto L_7588;

L_7588:
    if (icol == colPlanetMinerals)
        goto L_75b0;
    else
        goto L_7591;

L_7591:
    if (icol == colPlanetMiningRate)
        goto L_75b0;
    else
        goto L_759a;

L_759a:
    if (vprptCur->irpt != rptFleets)
        goto L_76b6;
    else
        goto L_75a7;

L_75a7:
    if (icol != colFleetCargo)
        goto L_76b6;
    else
        goto L_75b0;

L_75b0:
    strcpy(rgsz[cItems], rgsz[cItems - 1]);
    rgsz[cItems - 1][0] = 0;
    cItems++;
    j = 0;
    goto L_7601;

L_75fc:
    j++;

L_7601:
    if (j >= (vprptCur->irpt == rptFleets ? 1 : 0) + 3)
        goto L_764b;
    else
        goto L_7623;

L_7623:
    strcpy(rgsz[cItems], rgszMinerals[j]);
    cItems++;
    goto L_75fc;

L_764b:
    rgsz[cItems][0] = -1;
    rgsz[cItems][1] = 0;
    cItems++;
    strcpy(rgsz[cItems], PszGetCompressedString(idsWeightedAverage));
    cItems++;
    rgsz[cItems++][0] = 0;
    cSubsort = 5;

L_76b6:
    i++;

L_76bb:
    if (i < 2)
        goto L_751e;
    else
        goto L_76c5;

L_76c5:
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
    if (icol != 0)
        goto L_77a3;
    else
        goto L_7798;

L_7798:
    cItems -= 2;
    iHide = -1;

L_77a3:
    iBase = cItems;
    i = 0;
    ibit = 1;
    goto L_77d1;

L_77bd:
    i++;
    ibit *= 2;

L_77d1:
    if (i >= vprptCur->cFields)
        goto L_78a1;
    else
        goto L_77e1;

L_77e1:
    if ((ibit & vprptCur->grbitVisible) != 0)
        goto L_77bd;
    else
        goto L_77ff;

L_77ff:
    DxReportColHdr(vprptCur->irpt, i, szColTitle, hdc);
    cch = CchGetString(idsShow, rgsz[cItems]);
    strcpy(&rgsz[cItems][cch], szColTitle);
    cch = CchGetString(idsColumn, szT);
    strcat(rgsz[cItems], szT);
    rgcol[cItems] = i;
    cItems++;

L_789e:
    goto L_77bd;

L_78a1:
    if (cItems != iBase)
        goto L_78b3;
    else
        goto L_78ae;

L_78ae:
    cItems--;

L_78b3:
    ReleaseDC(hwndReportDlg, hdc);
    i = 0;
    goto L_7913;

L_78c8:
    if ((int16_t)(int8_t)rgsz[i][0] == 0)
        goto L_78fe;
    else
        goto L_78e0;

L_78e0:
    psz[i] = rgsz[i];
    goto L_790e;

L_78fe:
    psz[i] = 0;

L_790e:
    i++;

L_7913:
    if (i < cItems)
        goto L_78c8;
    else
        goto L_7920;

L_7920:
    iRet = PopupMenu(hwndReportDlg, pt.x, pt.y, cItems, NULL, psz, -1, fRightBtn);
    if (iRet < 0)
        goto L_7aef;
    else
        goto L_7957;

L_7957:
    gd.fChgReports = 1;
    if (iRet >= iSortLast)
        goto L_7a71;
    else
        goto L_7970;

L_7970:
    vicolSortPrev = vprptCur->icolSort;
    viSubsortPrev = vprptCur->iSubsort;
    vfAscendingPrev = vprptCur->fAscending;
    vprptCur->icolSort = icol;
    if (cSubsort != 0)
        goto L_79bf;
    else
        goto L_79a2;

L_79a2:
    vprptCur->fAscending = iRet == 0 ? 1 : 0;
    goto L_7a5c;

L_79bf:
    vprptCur->iSubsort = (int16_t)(iRet - 2) % (cSubsort + 3 + (vprptCur->irpt == rptFleets ? 1 : 0));
    if (vprptCur->iSubsort <= (vprptCur->irpt == rptFleets ? 1 : 0) + 3)
        goto L_7a3c;
    else
        goto L_7a1c;

L_7a1c:
    vprptCur->iSubsort = (vprptCur->irpt == rptFleets ? 1 : 0) + 3;

L_7a3c:
    vprptCur->fAscending = iRet >= cSubsort + 2 ? 0 : 1;

L_7a5c:
    SortReportCache(vprptCur->irpt, icol);
    goto L_7acb;

L_7a71:
    if (iRet != iHide)
        goto L_7a9b;
    else
        goto L_7a7e;

L_7a7e:
    fccolChange = 1;
    vprptCur->grbitVisible &= (int16_t)~(1 << icol);
    goto L_7acb;

L_7a9b:
    if (iRet < iBase)
        goto L_7acb;
    else
        goto L_7aa8;

L_7aa8:
    fccolChange = 1;
    vprptCur->grbitVisible |= (int16_t)(1 << rgcol[iRet]);

L_7acb:
    if (fccolChange == 0)
        goto L_7ada;
    else
        goto L_7ad5;

L_7ad5:
    SetHScrollBar();

L_7ada:
    InvalidateRect(hwndReportDlg, NULL, 1);

L_7aef:
    return;
}
