int16_t FReadFleet(FLEET *lpfl) {
    uint16_t  us;
    int16_t   cord;
    int16_t   fByte;
    ORDER    *lpord;
    int16_t   i;
    int16_t   cish;
    uint8_t  *pb;
    int16_t   cch;
    uint16_t *pus;
    char      szT[33];
    int16_t   cOut;

L_3a4c:
    cish = 0;
    fmemset(lpfl, 0, 0x7c);
    fmemmove(lpfl, rgbCur, 0xc);
    fByte = lpfl->fDone;
    us = rgbCur[12];
    pb = &(rgbCur[14]);
    if ((fByte == 0))
        goto L_3b2f;
    else
        goto L_3ab6;

L_3ab6:
    i = 0;
    goto L_3ace;

L_3abe:
    i = (i + 1);
    us = (us >> 0x1);

L_3ace:
    if ((us == 0x0))
        goto L_3bae;
    else
        goto L_3ad7;

L_3ad7:
    if (((us & 0x1) == 0x0))
        goto L_3abe;
    else
        goto L_3ae5;

L_3ae5:
    pb = (pb + 1);
    lpfl->rgcsh[i] = *(pb);
    if ((lpfl->rgcsh[i] == 0))
        goto L_3abe;
    else
        goto L_3b25;

L_3b25:
    cish = (cish + 1);

L_3b29:
    goto L_3abe;

L_3b2f:
    pus = pb;
    i = 0;
    goto L_3b4d;

L_3b3d:
    i = (i + 1);
    us = (us >> 0x1);

L_3b4d:
    if ((us == 0x0))
        goto L_3ba8;
    else
        goto L_3b56;

L_3b56:
    if (((us & 0x1) == 0x0))
        goto L_3b3d;
    else
        goto L_3b64;

L_3b64:
    pus = (pus + 1);
    lpfl->rgcsh[i] = *(pus);
    if ((lpfl->rgcsh[i] == 0))
        goto L_3b3d;
    else
        goto L_3ba1;

L_3ba1:
    cish = (cish + 1);

L_3ba5:
    goto L_3b3d;

L_3ba8:
    pb = pus;

L_3bae:
    if ((cish != 0))
        goto L_3bcb;
    else
        goto L_3bb7;

L_3bb7:
    lpfl->fDead = 0x1;

L_3bcb:
    if ((lpfl->det < 0x4))
        goto L_3cc0;
    else
        goto L_3bdd;

L_3bdd:
    us = pb;
    pb = (pb + 2);
    i = 0;
    goto L_3c06;

L_3bf6:
    i = (i + 1);
    us = (us >> 0x2);

L_3c06:
    if ((i >= 5))
        goto L_3cc0;
    else
        goto L_3c0f;

L_3c0f:
    goto L_3ca2;

L_3c18:
    lpfl->rgwtMin[i] = (uint32_t)(*(pb));
    pb = (pb + 1);
    goto L_3bf6;

L_3c47:
    lpfl->rgwtMin[i] = (uint32_t)(pb);
    pb = (pb + 2);
    goto L_3bf6;

L_3c73:
    lpfl->rgwtMin[i] = pb;
    pb = (pb + 4);
    goto L_3bf6;

L_3ca2:
    if (((us & 0x3) == 0x1))
        goto L_3c18;
    else
        goto L_3caa;

L_3caa:
    if (((us & 0x3) == 0x2))
        goto L_3c47;
    else
        goto L_3cb2;

L_3cb2:
    if (((us & 0x3) != 0x3))
        goto L_3bf6;
    else
        goto L_3cb7;

L_3cb7:
    goto L_3c73;

L_3cc0:
    if ((lpfl->det >= 0x7))
        goto L_3d11;
    else
        goto L_3cd2;

L_3cd2:
    lpfl->dirLong = pb;
    pb = (pb + 4);
    lpfl->wtFleet = pb;
    pb = (pb + 4);
    ReadRt();
    return 1;

L_3d11:
    if ((hdrCur.rt == rtFleetA))
        goto L_3d4b;
    else
        goto Corrupt;

Corrupt:
    AlertSz(PszFormatIds(idsGameFileAppearsCorruptUnableLoadFile, 0x0), MB_ICONHAND);
    return 0;

L_3d4b:
    us = pb;
    pb = (pb + 2);
    pus = pb;
    i = 0;
    goto L_3d7a;

L_3d6a:
    i = (i + 1);
    us = (us >> 0x1);

L_3d7a:
    if ((us == 0x0))
        goto L_3e0f;
    else
        goto L_3d83;

L_3d83:
    if (((us & 0x1) == 0x0))
        goto L_3d6a;
    else
        goto L_3d91;

L_3d91:
    pus = (pus + 1);
    lpfl->rgdv[i].dp = *(pus);
    if ((lpfl->rgdv[i].pctDp < 0x1f4))
        goto L_3d6a;
    else
        goto L_3dd8;

L_3dd8:
    lpfl->rgdv[i].pctDp = 0x1f3;

L_3e0c:
    goto L_3d6a;

L_3e0f:
    pb = pus;
    pb = (pb + 1);
    lpfl->iplan = *(pb);
    pb = (pb + 1);
    lpfl->cord = *(pb);
    lpfl->lpplord = LpplAlloc(0x12, (lpfl->cord + 1), htOrd);
    fmemset(lpfl->lpplord->rgord, 0, ((lpfl->cord + 1) * 18));
    cord = lpfl->cord;
    lpord = lpfl->lpplord->rgord;
    goto L_3ec0;

L_3eaf:
    lpord = (lpord + 1);
    cord = (cord - 1);

L_3ec0:
    if ((cord == 0))
        goto L_3f33;
    else
        goto L_3ec9;

L_3ec9:
    memset(rgbCur, 0, 0x12);
    ReadRt();
    if ((hdrCur.rt == rtOrderA))
        goto L_3f0b;
    else
        goto L_3ef5;

L_3ef5:
    if ((hdrCur.rt != rtOrderB))
        goto Corrupt;
    else
        goto L_3f0b;

L_3f0b:
    *(lpord) = *((ORDER *)(rgbCur));
    lpord->fNoAutoTrack = 0x0;
    goto L_3eaf;

L_3f33:
    lpfl->lpplord->iordMac = LOBYTE(lpfl->cord);
    if ((lpfl->idPlanet == -1))
        goto L_3fdb;
    else
        goto L_3f52;

L_3f52:
    if ((lpfl->idPlanet <= game.cPlanMax))
        goto L_3f6a;
    else
        goto L_3f61;

L_3f61:
    lpfl->idPlanet = -1;

L_3f6a:
    if ((lpfl->pt.x != rgptPlan[lpfl->idPlanet].x))
        goto L_3fa4;
    else
        goto L_3f85;

L_3f85:
    if ((lpfl->pt.y == rgptPlan[lpfl->idPlanet].y))
        goto L_3fdb;
    else
        goto L_3fa4;

L_3fa4:
    if ((i != 0))
        goto Corrupt;
    else
        goto L_3fad;

L_3fad:
    if ((game.turn != 0x0))
        goto Corrupt;
    else
        goto L_3fb7;

L_3fb7:
    lpfl->pt = rgptPlan[lpfl->idPlanet];

L_3fdb:
    ReadRt();
    if ((hdrCur.rt != rtString))
        goto L_40b2;
    else
        goto L_3ff3;

L_3ff3:
    cch = (uint16_t)(LOWORD(rgbCur));
    if ((cch != 0))
        goto L_4047;
    else
        goto L_4003;

L_4003:
    lpfl->lpszName = LpAlloc((strlen(rgbCur[1]) + 0x1), htString);
    fstrcpy(lpfl->lpszName, &(rgbCur[1]));
    goto L_40aa;

L_4047:
    cOut = 32;
    FDecompressUserString(&(rgbCur[1]), cch, szT, &(cOut));
    lpfl->lpszName = LpAlloc((strlen(szT) + 0x1), htString);
    fstrcpy(lpfl->lpszName, szT);

L_40aa:
    ReadRt();
    goto L_40c1;

L_40b2:
    lpfl->lpszName = 0x0;

L_40c1:
    return 1;
}
