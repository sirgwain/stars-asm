void WritePlayerMessages(int16_t iPlayer) {
    uint8_t *lpbMax;
    uint8_t  rgb[1024];
    int16_t  cbMsg;
    MSGPLR  *lpmp;
    uint8_t *lpb;

L_9702:
    cbMsg = 0;
    if (iPlayer == -1)
        goto L_98d0;
    else
        goto L_971d;

L_971d:
    lpb = (uint8_t *)lpMsg;
    lpbMax = lpb + imemMsgCur;
    goto L_980d;

L_9742:
    if (cbMsg + 20 < 1024)
        goto L_976f;
    else
        goto L_9751;

L_9751:
    WriteRt(rtMsg, cbMsg, rgb);
    cbMsg = 0;

L_976f:
    if ((*lpb & 0xf) != iPlayer)
        goto L_97f1;
    else
        goto L_9784;

L_9784:
    if ((RawLoad16((uint8_t *)lpb + 0x1) & 0x1ff) == 0x1ff)
        goto L_97f1;
    else
        goto L_9797;

L_9797:
    fmemmove(&rgb[cbMsg], lpb + 1, (*lpb >> 4 & 0xf) + 4);
    cbMsg += (*lpb >> 4 & 0xf) + 4;

L_97f1:
    lpb += 5 + (*lpb >> 4 & 0xf);

L_980d:
    if (lpb < lpbMax)
        goto L_9742;
    else
        goto L_981c;

L_981c:
    if (cbMsg == 0)
        goto L_983e;
    else
        goto L_9826;

L_9826:
    WriteRt(rtMsg, cbMsg, rgb);

L_983e:
    lpmp = vlpmsgplrOut;
    goto L_98bc;

L_9850:
    if (lpmp->iPlrTo != 0)
        goto L_986e;
    else
        goto L_985e;

L_985e:
    if (lpmp->iPlrFrom != iPlayer)
        goto L_9881;
    else
        goto L_986e;

L_986e:
    if (lpmp->iPlrTo - 1 != iPlayer)
        goto L_98a9;
    else
        goto L_9881;

L_9881:
    WriteRt(rtPlrMsg, abs(lpmp->cLen) + 12, (uint8_t *)&lpmp->iPlrFrom - 4);

L_98a9:
    lpmp = lpmp->lpmsgplrNext;

L_98bc:
    if (lpmp != 0)
        goto L_9850;
    else
        goto L_98d0;

L_98d0:
    return;
}
