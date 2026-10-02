int16_t PackageUpMsg(uint8_t *pb, int16_t iPlr, MessageId iMsg, MsgGoto iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6,
                     int16_t p7) {
    int16_t *pi;
    int16_t  i;
    uint16_t grbit;
    MSGTURN *lpmt;
    uint8_t *lpb;
    uint8_t *lpbBase;
    int16_t  rgArgs[7];

    if (iPlr == -1) {
        return 0;
    }
    if (rgplr[iPlr].fAi != 0 && rgplr[iPlr].idAi != idAiMaid) {
        switch (iMsg) {
        default:
            return 0;
        case idmHasBombedKillingOffEnemyColonists:
        case idmHaveAttackedFirstRateStormTroopersThough:
        case idmColonistsHaveDiedOffLongerControlPlanet:
        case idmColonistsHaveJumpedShipLongerControlPlanet:
            break;
        }
    }
    if ((uint16_t)(imemMsgCur + 20) > 0xffc8) {
        return -1;
    }
    lpb = pb;
    lpmt = (MSGTURN *)lpb;
    lpmt->iPlr = (uint32_t)iPlr & 0xf;
    lpmt->msghdr.iMsg = iMsg;
    lpmt->msghdr.grWord = 0;
    lpmt->msghdr.wGoto = iObj;
    lpb += 5;
    lpbBase = lpb;
    grbit = 1;
    rgArgs[0] = p1;
    rgArgs[1] = p2;
    rgArgs[2] = p3;
    rgArgs[3] = p4;
    rgArgs[4] = p5;
    rgArgs[5] = p6;
    rgArgs[6] = p7;
    pi = rgArgs;
    i = 0;
    while (i < rgcMsgArgs[iMsg]) {
        if ((*pi & 0xff00) != 0) {
            lpmt->msghdr.grWord |= grbit;
            RawStore16(lpb, *pi);
            lpb += 2;
        } else {
            *lpb = LOBYTE(*pi);
            lpb++;
        }
        i++;
        pi++;
        grbit *= 2;
    }
    lpmt->cbParams = (uint32_t)(lpb - lpbBase) & 0xf;
    return lpb - pb;
}
