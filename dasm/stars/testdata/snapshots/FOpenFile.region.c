int16_t FOpenFile(DtFileType dt, int16_t iPlayer, int16_t md) {
    RTBOF    rtbof;
    StringId ids;
    int16_t  fCheckMulti;
    int16_t  fRewind;
    int16_t  fSilentSav;
    jmp_buf *penvMemSav;
    jmp_buf  env;

    fSilentSav = fFileErrSilent;
    ids = idsCantOpenFile;
    gd.fPartialTurn = 0;
    fCheckMulti = dt & 0x2000;
    fRewind = dt & 0x1000;
    dt &= 0xff;
    SetSzWorkFromDt(dt, iPlayer);
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0) {
        fFileErrSilent = fSilentSav;
        FileError(ids);
        StreamClose();
        penvMem = penvMemSav;
        return 0;
    }
    fFileErrSilent = 1;
    StreamOpen(szWork, md);
    fFileErrSilent = fSilentSav;
    ids = idsGameFileAppearsCorruptUnableLoadFile;
    ReadRt();
    if (hdrCur.rt != rtBOF || ((RTBOF *)rgbCur)->verMajor != 2 || ((RTBOF *)rgbCur)->verMinor < 49 || ((RTBOF *)rgbCur)->verMinor >= 84) {
        if (hdrCur.rt == rtBOF) {
            FileError(((RTBOF *)rgbCur)->verMajor > 2 || (((RTBOF *)rgbCur)->verMajor == 2 && ((RTBOF *)rgbCur)->verMinor > 84) ? 714 : 1235);
        } else {
            FileError(idmColonistsDroppedDestroyedSpiritedFighting);
        }
    } else {
        rtbof = *((RTBOF *)rgbCur);
        if (rtbof.iPlayer != iPlayer) {
            FileError(idmGroundTroopsValiantlyDestroyedAttackingBarbarian);
        } else {
            if (game.lid != 0) {
                if (rtbof.lidGame != game.lid) {
                    FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
                    goto LBadFile;
                }
                if (dt != dtHist) {
                    if (fCheckMulti != 0 && rtbof.fMulti != 0) {
                        lseek(hf, -4, 2);
                        ReadRt();
                        if (hdrCur.rt != rtEOF && hdrCur.cb != 2)
                            goto LBadFile;
                        rtbof.turn = RawLoad16(rgbCur);
                        game.wGen = rtbof.wGen;
                    }
                    if (game.turn == 0 && game.turn != rtbof.turn) {
                        game.turn = rtbof.turn;
                        game.wGen = rtbof.wGen;
                    } else {
                        if (rtbof.turn != game.turn) {
                            FileError(idmVigilantFleetsManagedDefeatSavageVerminWithout);
                            goto LBadFile;
                        }
                        if (dt == dtHost && gd.fHostMode == 0 && rtbof.fInUse != 0) {
                            if (AlertSz(PszFormatIds(idsHostFileMarkedUseAnotherInstanceStars, NULL), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) != IDYES)
                                goto LBadFile;
                        } else {
                            if (rtbof.fDone == 0 && gd.fGeneratingTurn != 0 && gd.fForceTurn == 0) {
                                gd.fPartialTurn = 1;
                                goto LBadFile;
                            }
                            if (dt == dtLog && game.fTutorial == 0 && rtbof.wGen != game.wGen) {
                                FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
                                goto LBadFile;
                            }
                        }
                    }
                } else if (rtbof.iPlayer != iPlayer) {
                    goto LBadFile;
                }
            }
            if (fRewind != 0) {
                lseek(hf, 0, 0);
                ReadRt();
            }
            penvMem = penvMemSav;
            wVersFile = rtbof.wVersion;
            gd.fFileCrippled = rtbof.fCrippled;
            return 1;
        }
    }
LBadFile:
    StreamClose();
    penvMem = penvMemSav;
    return 0;
}
