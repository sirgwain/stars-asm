void DoBattles(int16_t fPostMovement) {
    int16_t  cplr;
    int16_t  ifl;
    FLEET   *lpfl;
    uint16_t grfSpectator;
    uint16_t grfPlayer;
    uint16_t rggrfAttack[16];

    LinkFleets(fPostMovement);
    vrgtok = LpAlloc(256 * sizeof(TOK), htMisc);
    vlpwtCargo = LpAlloc(0x200, htMisc);
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        lpfl->fBombed = 0;
        if (lpfl->fDone == 0 && lpfl->fDead == 0 && lpfl->lpflNext != 0) {
            cplr = CplrBattle(lpfl, rggrfAttack, &grfPlayer, &grfSpectator);
            if (cplr != -1 && cplr != 0 && FDoCoolBattle(lpfl, cplr, rggrfAttack, grfPlayer, grfSpectator) != 0) {
            }
        }
    }
    FreeLp(vlpwtCargo, htMisc);
    FreeLp(vrgtok, htMisc);
    vlpwtCargo = NULL;
    vrgtok = NULL;
    if (lpbBattleT != 0) {
        RawStore16(lpbBattleT, 0xffff);
        FreeLp(lpbBattleT, htBattle);
        lpbBattleT = NULL;
    }
    if (lpbBattleCur != 0) {
        RawStore16(lpbBattleCur, 0xffff);
    }
    DoBombing();
    return;
}
