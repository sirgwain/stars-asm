int16_t FOpenFile(DtFileType dt, int16_t iPlayer, int16_t md) {
    RTBOF     rtbof;
    StringId  ids;
    int16_t   fCheckMulti;
    int16_t   fRewind;
    int16_t   fSilentSav;
    jmp_buf  *penvMemSav[9];
    jmp_buf   env[9];
    MessageId t_merge_4c1e_0001;

L_4ac2:
    fSilentSav = fFileErrSilent;
    ids = idsCantOpenFile;
    gd.fPartialTurn = 0x0;
    fCheckMulti = (dt & 0x2000);
    fRewind = (dt & 0x1000);
    dt = (dt & 0xff);
    SetSzWorkFromDt(dt, iPlayer);
    penvMemSav = penvMem;
    penvMem = &(env);
    if ((setjmp(env) == 0))
        goto L_4b49;
    else
        goto L_4b27;

L_4b27:
    fFileErrSilent = fSilentSav;
    FileError(ids);
    StreamClose();
    penvMem = penvMemSav;
    return 0;

L_4b49:
    fFileErrSilent = 1;
    StreamOpen(szWork, md);
    fFileErrSilent = fSilentSav;
    ids = idsGameFileAppearsCorruptUnableLoadFile;
    ReadRt();
    if ((hdrCur.rt != rtBOF))
        goto L_4bc4;
    else
        goto L_4b81;

L_4b81:
    if ((((rgbCur[8] >> 0xc) & 0xf) != 0x2))
        goto L_4bc4;
    else
        goto L_4b94;

L_4b94:
    if ((((rgbCur[8] >> 0x5) & 0x7f) < 0x31))
        goto L_4bc4;
    else
        goto L_4bac;

L_4bac:
    if ((((rgbCur[8] >> 0x5) & 0x7f) < 0x54))
        goto L_4c47;
    else
        goto L_4bc4;

L_4bc4:
    if ((hdrCur.rt != rtBOF))
        goto L_4c2a;
    else
        goto L_4bd7;

L_4bd7:
    if ((((rgbCur[8] >> 0xc) & 0xf) > 0x2))
        goto L_4c15;
    else
        goto L_4bea;

L_4bea:
    if ((((rgbCur[8] >> 0xc) & 0xf) != 0x2))
        goto L_4c1b;
    else
        goto L_4bfd;

L_4bfd:
    if ((((rgbCur[8] >> 0x5) & 0x7f) <= 0x54))
        goto L_4c1b;
    else
        goto L_4c15;

L_4c15:
    t_merge_4c1e_0001 = 0x2ca;
    goto L_4c1e;

L_4c1b:
    t_merge_4c1e_0001 = 0x4d3;

L_4c1e:
    FileError(t_merge_4c1e_0001);
    goto LBadFile;

L_4c2a:
    FileError(idmColonistsDroppedDestroyedSpiritedFighting);

LBadFile:
    StreamClose();
    penvMem = penvMemSav;
    return 0;

L_4c47:
    rtbof = *((RTBOF *)(rgbCur));
    if ((rtbof.iPlayer == iPlayer))
        goto L_4c7b;
    else
        goto L_4c6c;

L_4c6c:
    FileError(idmGroundTroopsValiantlyDestroyedAttackingBarbarian);
    goto LBadFile;

L_4c7b:
    if ((LOWORD(game.lid) != 0x0))
        goto L_4c8f;
    else
        goto L_4c85;

L_4c85:
    if ((HIWORD(game.lid) == 0x0))
        goto L_4ebd;
    else
        goto L_4c8f;

L_4c8f:
    if ((LOWORD(rtbof.lidGame) != LOWORD(game.lid)))
        goto L_4ca6;
    else
        goto L_4c9e;

L_4c9e:
    if ((HIWORD(rtbof.lidGame) == HIWORD(game.lid)))
        goto L_4cb8;
    else
        goto L_4ca6;

L_4ca6:
    FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
    goto LBadFile;

L_4cb8:
    if ((dt == dtHist))
        goto L_4ea5;
    else
        goto L_4cc1;

L_4cc1:
    if ((fCheckMulti == 0))
        goto L_4d4d;
    else
        goto L_4cca;

L_4cca:
    if ((rtbof.fMulti == 0x0))
        goto L_4d4d;
    else
        goto L_4cdd;

L_4cdd:
    lseek(hf, -4, 2);
    ReadRt();
    if ((hdrCur.rt == rtEOF))
        goto L_4d1e;
    else
        goto L_4d0d;

L_4d0d:
    if ((hdrCur.cb != 0x2))
        goto LBadFile;
    else
        goto L_4d1e;

L_4d1e:
    rtbof.turn = LOWORD(rgbCur);
    game.wGen = rtbof.wGen;

L_4d4d:
    if ((game.turn != 0x0))
        goto L_4d95;
    else
        goto L_4d57;

L_4d57:
    if ((game.turn == rtbof.turn))
        goto L_4d95;
    else
        goto L_4d63;

L_4d63:
    game.turn = rtbof.turn;
    game.wGen = rtbof.wGen;
    goto L_4ebd;

L_4d95:
    if ((rtbof.turn == game.turn))
        goto L_4db2;
    else
        goto L_4da0;

L_4da0:
    FileError(idmVigilantFleetsManagedDefeatSavageVerminWithout);
    goto LBadFile;

L_4db2:
    if ((dt != dtHost))
        goto L_4e11;
    else
        goto L_4dbb;

L_4dbb:
    if ((gd.fHostMode != 0x0))
        goto L_4e11;
    else
        goto L_4dcf;

L_4dcf:
    if ((rtbof.fInUse == 0x0))
        goto L_4e11;
    else
        goto L_4de2;

L_4de2:
    if ((AlertSz(PszFormatIds(idsHostFileMarkedUseAnotherInstanceStars, 0x0), 0x2024) != 6))
        goto LBadFile;
    else
        goto L_4e08;

L_4e08:
    goto L_4ebd;

L_4e11:
    if ((rtbof.fDone != 0x0))
        goto L_4e58;
    else
        goto L_4e24;

L_4e24:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_4e58;
    else
        goto L_4e34;

L_4e34:
    if ((gd.fForceTurn != 0x0))
        goto L_4e58;
    else
        goto L_4e46;

L_4e46:
    gd.fPartialTurn = 0x1;
    goto LBadFile;

L_4e58:
    if ((dt != dtLog))
        goto L_4ebd;
    else
        goto L_4e61;

L_4e61:
    if ((game.fTutorial != 0x0))
        goto L_4ebd;
    else
        goto L_4e75;

L_4e75:
    if ((rtbof.wGen == game.wGen))
        goto L_4ebd;
    else
        goto L_4e93;

L_4e93:
    FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
    goto LBadFile;

L_4ea5:
    if ((rtbof.iPlayer != iPlayer))
        goto LBadFile;
    else
        goto L_4ebd;

L_4ebd:
    if ((fRewind == 0))
        goto L_4ee3;
    else
        goto L_4ec6;

L_4ec6:
    lseek(hf, 0, 0);
    ReadRt();

L_4ee3:
    penvMem = penvMemSav;
    wVersFile = rtbof.wVersion;
    gd.fFileCrippled = rtbof.fCrippled;
    return 1;
}
