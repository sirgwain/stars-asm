#ifndef STARS_DECOMPILED_COMMON_H
#define STARS_DECOMPILED_COMMON_H

#include <stdint.h>
#include <windows.h>

#include <setjmp.h>

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

// enums that are actually win defines
typedef uint16_t ScrollCode;
typedef uint16_t WMType;
typedef uint16_t WM_TYPE;
typedef uint16_t WM_TYPE;
typedef uint16_t CtlColorType;
typedef uint16_t MessageBoxType;
typedef uint16_t PatBltRop;
typedef uint16_t SystemMetric;
typedef uint16_t DeviceCapsIndex;
typedef uint16_t ShowWindowCmd;
typedef uint16_t WindowStyle;
typedef uint16_t WindowExStyle;
typedef uint16_t SetWindowPosFlags;
typedef uint16_t WindowLongOffset;
typedef uint16_t GetWindowCmd;
typedef uint16_t WinHelpCommand;
typedef uint16_t StockObjectId;
typedef uint16_t BkMode;
typedef uint16_t BitBltRop;

// WM_CTLCOLOR is replaced with many, default to dlg to now
#define WM_CTLCOLOR WM_CTLCOLORDLG

#endif
