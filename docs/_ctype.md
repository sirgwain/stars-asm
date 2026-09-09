`_ctype` is a ctype.h table, used by these macros

```
(_ctype[x + 1] & 0x01) != 0  -> isupper(x)
(_ctype[x + 1] & 0x03) != 0  -> isalpha(x)
(_ctype[x + 1] & 0x04) != 0  -> isdigit(x)
```

```
❯ grep _ctype decompiled/*.c
decompiled/battle.c:    if (((_ctype[(btlplan.szName[(cLen - 2)] + 1)] & 0x4) == 0x0))
decompiled/build.c:    /* untranslated: branch (_ctype[(sext8to16(part[0xfffe:1](lpsz[cLen])) + 1)] & 0x4) == 0x0 ? L_69d3 : L_69b7 */
decompiled/file.c:    psz = PszPlayerName(0, (_ctype[((uint16_t)(pplr->szName[0x0]) + 0x1)] & 0x1), 1, 0, 0, pplr);
decompiled/globals.c:uint8_t   _ctype[0];
decompiled/init.c:    if (((_ctype[((uint16_t)(*(pszFormat)) + 1)] & 0x3) == 0x0))
decompiled/init.c:    fPopped = (_ctype[((uint16_t)(*(pszFormat)) + 0x1)] & 0x1);
decompiled/scan.c:    if (((_ctype[((uint16_t)(*(pch)) + 1)] & 0x4) == 0x0))
decompiled/tutor.c:    fPara = (_ctype[((uint16_t)(rgch[0x0]) + 0x1)] & 0x1);
decompiled/utilgen.c:    if (((_ctype[((uint16_t)(*(pch)) + 1)] & 0x4) == 0x0))
```
