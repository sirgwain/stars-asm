int16_t FOpenFile(DtFileType dt, int16_t iPlayer, int16_t md) {
    RTBOF     rtbof;
    StringId  ids;
    int16_t   fCheckMulti;
    int16_t   fRewind;
    int16_t   fSilentSav;
    jmp_buf  *penvMemSav;
    jmp_buf   env;
    MessageId t_merge_4c1e_0001;

    fSilentSav = fFileErrSilent;
    ids = idsCantOpenFile;
    gd.fPartialTurn = 0x0;
    fCheckMulti = dt & 0x2000;
    fRewind = dt & 0x1000;
    dt = dt & 0xff;
    SetSzWorkFromDt(dt, iPlayer);
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) == 0) {
        fFileErrSilent = 1;
        StreamOpen(szWork, md);
        fFileErrSilent = fSilentSav;
        ids = idsGameFileAppearsCorruptUnableLoadFile;
        ReadRt();
        if (hdrCur.rt == rtBOF && (RawLoad16(&rgbCur[8]) >> 0xc & 0xf) == 0x2 && (RawLoad16(&rgbCur[8]) >> 0x5 & 0x7f) >= 0x31 &&
            (RawLoad16(&rgbCur[8]) >> 0x5 & 0x7f) < 0x54) {
            rtbof = *(RTBOF *)rgbCur;
            if (rtbof.iPlayer == iPlayer) {
                if (game.lid != 0) {
                    if (rtbof.lidGame != game.lid) {
                        FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
                        goto LBadFile;
                    }
                    if (dt == dtHist) {
                        if (rtbof.iPlayer != iPlayer)
                            goto LBadFile;
                    } else {
                        if (fCheckMulti != 0 && rtbof.fMulti != 0x0) {
                            lseek(hf, -4, 2);
                            ReadRt();
                            if (hdrCur.rt != rtEOF && hdrCur.cb != 0x2)
                                goto LBadFile;
                            rtbof.turn = RawLoad16(rgbCur);
                            game.wGen = rtbof.wGen;
                        }
                        if (game.turn != 0x0 || game.turn == rtbof.turn) {
                            if (rtbof.turn != game.turn) {
                                FileError(idmVigilantFleetsManagedDefeatSavageVerminWithout);
                                goto LBadFile;
                            }
                            if (dt != dtHost || gd.fHostMode != 0x0 || rtbof.fInUse == 0x0) {
                                if (rtbof.fDone == 0x0 && gd.fGeneratingTurn != 0x0 && gd.fForceTurn == 0x0) {
                                    gd.fPartialTurn = 0x1;
                                    goto LBadFile;
                                }
                                if (dt == dtLog && game.fTutorial == 0x0 && rtbof.wGen != game.wGen) {
                                    FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
                                    goto LBadFile;
                                }
                            } else if (AlertSz(PszFormatIds(idsHostFileMarkedUseAnotherInstanceStars, 0x0), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) !=
                                       IDYES) {
                                goto LBadFile;
                            }
                        } else {
                            game.turn = rtbof.turn;
                            game.wGen = rtbof.wGen;
                        }
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
            FileError(idmGroundTroopsValiantlyDestroyedAttackingBarbarian);
        } else if (hdrCur.rt != rtBOF) {
            FileError(idmColonistsDroppedDestroyedSpiritedFighting);
        } else {
            if ((RawLoad16(&rgbCur[8]) >> 0xc & 0xf) <= 0x2 && ((RawLoad16(&rgbCur[8]) >> 0xc & 0xf) != 0x2 || (RawLoad16(&rgbCur[8]) >> 0x5 & 0x7f) <= 0x54)) {
                t_merge_4c1e_0001 = 0x4d3;
            } else {
                t_merge_4c1e_0001 = 0x2ca;
            }
            FileError(t_merge_4c1e_0001);
        }
    LBadFile:
        StreamClose();
        penvMem = penvMemSav;
        return 0;
    }
    fFileErrSilent = fSilentSav;
    FileError(ids);
    StreamClose();
    penvMem = penvMemSav;
    return 0;
}
