#ifndef STARS_DECOMPILED_COMMON_H
#define STARS_DECOMPILED_COMMON_H

#include <stdint.h>
#include <windows.h>

#include <setjmp.h>

#define LOWORD(x)        ((uint16_t)((uint32_t)(x) & 0xffffu))
#define HIWORD(x)        ((uint16_t)(((uint32_t)(x) >> 16) & 0xffffu))
#define MAKELONG(lo, hi) ((uint32_t)(uint16_t)(lo) | ((uint32_t)(uint16_t)(hi) << 16))

#include "enums.h"
#include "structs.h"

#include "ai.h"
#include "ai2.h"
#include "ai3.h"
#include "ai4.h"
#include "aiutil.h"
#include "battle.h"
#include "build.h"
#include "create.h"
#include "file.h"
#include "globals.h"
#include "init.h"
#include "log.h"
#include "mdi.h"
#include "memory.h"
#include "mine.h"
#include "msg.h"
#include "parts.h"
#include "planet.h"
#include "popup.h"
#include "produce.h"
#include "race.h"
#include "report.h"
#include "research.h"
#include "save.h"
#include "scan.h"
#include "ship.h"
#include "ship2.h"
#include "stars.h"
#include "strings.h"
#include "tb.h"
#include "thing.h"
#include "turn.h"
#include "turn2.h"
#include "turn3.h"
#include "tutor.h"
#include "tutor2.h"
#include "util.h"
#include "utilgen.h"
#include "vcr.h"

typedef jmp_buf ENV;

#endif
