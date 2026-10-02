void DeleteWpFar(FLEET *lpfl, int16_t iDel, int16_t fRecycle) {
    ORDER ord;

    if (fRecycle != 0) {
        if (iDel == 86 || lpfl->cord == 2 ||
            (lpfl->lpplord->rgord[lpfl->cord - 1].pt.x == lpfl->lpplord->rgord[iDel].pt.x &&
             lpfl->lpplord->rgord[lpfl->cord - 1].pt.y == lpfl->lpplord->rgord[iDel].pt.y)) {
            fRecycle = FALSE;
        } else {
            ord = lpfl->lpplord->rgord[iDel];
        }
    }
    fmemmove(&lpfl->lpplord->rgord[iDel], &lpfl->lpplord->rgord[iDel + 1], (lpfl->cord - iDel - 1) * sizeof(ORDER));
    if (fRecycle != 0) {
        lpfl->lpplord->rgord[lpfl->cord - 1] = ord;
    } else {
        lpfl->cord--;
        lpfl->lpplord->iordMac--;
    }
    return;
}
