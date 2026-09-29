void DeleteWpFar(FLEET *lpfl, int16_t iDel, int16_t fRecycle) {
    ORDER ord;

    if (fRecycle != 0) {
        if (iDel != 86 && lpfl->cord != 2 &&
            (lpfl->lpplord->rgord[lpfl->cord - 1].pt.x != lpfl->lpplord->rgord[iDel].pt.x ||
             lpfl->lpplord->rgord[lpfl->cord - 1].pt.y != lpfl->lpplord->rgord[iDel].pt.y)) {
            ord = lpfl->lpplord->rgord[iDel];
        } else {
            fRecycle = 0;
        }
    }
    fmemmove(&lpfl->lpplord->rgord[iDel], &lpfl->lpplord->rgord[iDel + 1], (lpfl->cord - iDel - 1) * sizeof(ORDER));
    if (fRecycle == 0) {
        lpfl->cord = lpfl->cord - 1;
        lpfl->lpplord->iordMac = lpfl->lpplord->iordMac - 0x1;
    } else {
        lpfl->lpplord->rgord[lpfl->cord - 1] = ord;
    }
    return;
}
