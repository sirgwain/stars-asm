#ifndef STARS_DECOMPILED_WIN16DEFINES_H
#define STARS_DECOMPILED_WIN16DEFINES_H

#include <direct.h>
#include <io.h>

// Old CRT names
#define fmemcmp  memcmp
#define fmemcpy  memcpy
#define fmemmove memmove
#define fmemset  memset
#define fstrcat  strcat
#define fstrcmp  strcmp
#define fstrcpy  strcpy
#define fstricmp stricmp
#define fstrlen  strlen

#define strtime         _strtime
#define access          _access
#define filelength      filelength16
#define lseek           lseek16
#define mkdir           _mkdir
#define tell            tell16
#define dos_getdiskfree _getdiskfree

typedef struct _diskfree_t _diskfree_t;

// SIGNHIWORD is the high word from signed 16-to-32 extension (the x86 CWD
// instruction), not the upper half of an already-wide value.
#define SIGNHIWORD(value) ((int16_t)(((uint16_t)(value) & 0x8000) ? -1 : 0))

// Old Windows names
#define _wsprintf wsprintfA

// Win16 APIs whose Win32 equivalents changed signature
#define GetTextExtent GetTextExtent16
#define MoveTo        MoveTo16
#define SetWindowOrg  SetWindowOrg16
#define SetBrushOrg   SetBrushOrg16
#undef GetWindowLong
#undef SetWindowLong
#define GetWindowLong GetWindowLong16
#define SetWindowLong SetWindowLong16
#undef GetDriveType
#define GetDriveType  GetDriveType16
#define AllocResource AllocResource16
#undef CreateWindow
#define CreateWindow CreateWindow16

// shims

static inline char *strdate(char *buf) {
    time_t     t = time(NULL);
    struct tm *tm = localtime(&t);

    sprintf(buf, "%02d/%02d/%02d", tm->tm_mon + 1, tm->tm_mday, (tm->tm_year + 1900) % 100);

    return buf;
}

static inline DWORD GetTextExtent16(HDC hdc, LPCSTR str, int len) {
    SIZE size;

    if (!GetTextExtentPoint32A(hdc, str, len, &size))
        return 0;

    return MAKELONG((WORD)size.cx, (WORD)size.cy);
}

/*
 * Win16 window creation
 *
 * Win16 CW_USEDEFAULT is the 16-bit 0x8000, which Stars stores in its window
 * rectangles as -32768 when Stars.ini holds no position. Win32 reads that as
 * a real coordinate and places the window far off screen, so it is mapped to
 * the native CW_USEDEFAULT.
 */

// CreateWindow16 creates a window, mapping Win16 CW_USEDEFAULT positions and
// sizes to the native value.
static inline HWND CreateWindow16(LPCSTR cls, LPCSTR name, DWORD style, int x, int y, int cx, int cy, HWND parent, HMENU menu, HINSTANCE inst, LPVOID param) {
    if (x == -32768 || x == 0x8000)
        x = CW_USEDEFAULT;
    if (cx == -32768 || cx == 0x8000)
        cx = CW_USEDEFAULT;
    return CreateWindowExA(0, cls, name, style, x, y, cx, cy, parent, menu, inst, param);
}

/*
 * Win16 GDI compatibility
 *
 * The Win32 Ex versions added an optional output parameter containing
 * the previous position/origin. The Win16 functions returned the
 * previous value packed into a DWORD.
 */

static inline DWORD MoveTo16(HDC hdc, int x, int y) {
    POINT old;

    if (!MoveToEx(hdc, x, y, &old))
        return 0;

    return MAKELONG((WORD)old.x, (WORD)old.y);
}

static inline DWORD SetWindowOrg16(HDC hdc, int x, int y) {
    POINT old;

    if (!SetWindowOrgEx(hdc, x, y, &old))
        return 0;

    return MAKELONG((WORD)old.x, (WORD)old.y);
}

static inline DWORD SetBrushOrg16(HDC hdc, int x, int y) {
    POINT old;

    if (!SetBrushOrgEx(hdc, x, y, &old))
        return 0;

    return MAKELONG((WORD)old.x, (WORD)old.y);
}

/*
 * Win16 file handles
 *
 * Stars opens its files with OpenFile and then measures and seeks them with
 * the C runtime's filelength, lseek and tell. Win16 HFILEs were DOS handles
 * the C runtime accepted; Win32 HFILEs are kernel handles, which the C
 * runtime's descriptor functions reject, so these work on the HFILE.
 */

// filelength16 returns the size of an open HFILE, or -1 on error.
static inline long filelength16(HFILE hf) {
    DWORD size = GetFileSize((HANDLE)(INT_PTR)hf, NULL);
    return size == INVALID_FILE_SIZE ? -1L : (long)size;
}

// lseek16 moves an HFILE's position, origin being 0 (start), 1 (current) or
// 2 (end) as in lseek, and returns the new position or -1 on error.
static inline long lseek16(HFILE hf, long offset, int origin) { return _llseek(hf, offset, origin); }

// tell16 returns an HFILE's position.
static inline long tell16(HFILE hf) { return _llseek(hf, 0, FILE_CURRENT); }

/*
 * Win16 points
 *
 * Win16 POINT held 16-bit ints, and Stars writes records holding points to
 * its files. Stars' points are POINT16 to keep that layout; Win32 POINT holds
 * LONGs, so points convert where they pass into or out of the Win32 API.
 */

typedef struct tagPOINT16 {
    int16_t x;
    int16_t y;
} POINT16;

// PointFrom16 widens a Stars point to a Win32 point.
static inline POINT PointFrom16(POINT16 pt) {
    POINT out = {pt.x, pt.y};
    return out;
}

// PointTo16 narrows a Win32 point to a Stars point.
static inline POINT16 PointTo16(POINT pt) {
    POINT16 out = {(int16_t)pt.x, (int16_t)pt.y};
    return out;
}

/*
 * Win16 window longs
 *
 * Stars only uses the window long to subclass controls through
 * GWL_WNDPROC. Win16 indexes are 16-bit, so the negative GWL_ indexes arrive
 * as 0xfffc and are narrowed back here; the procedure pointer is stored at
 * full native width because Win64 has no 32-bit GWL_WNDPROC.
 */

// GetWindowLong16 retrieves the window procedure using the Win16 index.
static inline WNDPROC GetWindowLong16(HWND hwnd, short index) { return (WNDPROC)GetWindowLongPtrA(hwnd, index); }

// SetWindowLong16 replaces the window procedure and returns its predecessor.
static inline WNDPROC SetWindowLong16(HWND hwnd, short index, WNDPROC lpfn) { return (WNDPROC)SetWindowLongPtrA(hwnd, index, (LONG_PTR)lpfn); }

/*
 * Win16 drive and resource APIs
 *
 * Win16 GetDriveType took a 0-based drive number; Win32 takes a root path.
 * Win16 AllocResource allocated a moveable block for a resource's data
 * (cb 0 meaning the resource's size) and callers locked it with
 * LockResource. Win32 LockResource returns its handle unchanged, so the block
 * is allocated GMEM_FIXED, whose handle is the data pointer; LockResource,
 * GlobalLock and GlobalUnlock then all behave as they did on Win16.
 */

static inline UINT GetDriveType16(int drive) {
    char root[] = "A:\\";

    root[0] = (char)('A' + drive);
    return GetDriveTypeA(root);
}

static inline HGLOBAL AllocResource16(HINSTANCE hinst, HRSRC hrsrc, DWORD cb) { return GlobalAlloc(GMEM_FIXED, cb ? cb : SizeofResource(hinst, hrsrc)); }

// tagTIMERINFO retains the 12-byte ToolHelp layout from utils/TOOLHELP.H.
typedef struct tagTIMERINFO {
    DWORD dwSize;
    DWORD dwmsSinceStart;
    DWORD dwmsThisVM;
} tagTIMERINFO, TIMERINFO;

// TimerCount reports elapsed system milliseconds. Win32 has no separate
// Win16 VM clock, so both counters use the same tick count.
static inline BOOL TimerCount(TIMERINFO *timer) {
    if (timer->dwSize != sizeof(*timer)) {
        SetLastError(ERROR_BAD_LENGTH);
        return FALSE;
    }
    timer->dwmsSinceStart = GetTickCount();
    timer->dwmsThisVM = timer->dwmsSinceStart;
    return TRUE;
}

// _find_t provides the DOS search fields consumed by GetDiskSerialNumber.
typedef struct _find_t {
    unsigned char reserved[21];
    unsigned char attrib;
    uint16_t      wr_time;
    uint16_t      wr_date;
    uint32_t      size;
    char          name[13];
} _find_t;

// dos_findfirst supports Stars' volume-label search (attribute 0x08).
// Win32 exposes the label but not its DOS directory-entry timestamp; those
// fields are zero, so the legacy disk fingerprint will differ from Win16.
static inline unsigned dos_findfirst(const char *path, unsigned attrib, _find_t *info) {
    char root[] = "A:\\";
    char label[MAX_PATH + 1];

    if (attrib != 0x08)
        return ERROR_NOT_SUPPORTED;
    if (path[0] == '\0' || path[1] != ':')
        return ERROR_INVALID_DRIVE;
    root[0] = path[0];
    if (!GetVolumeInformationA(root, label, sizeof(label), NULL, NULL, NULL, NULL, 0))
        return GetLastError();
    if (label[0] == '\0')
        return ERROR_NO_MORE_FILES;

    memset(info, 0, sizeof(*info));
    info->attrib = 0x08;
    // DOS returns a volume label as an 8.3 name, with a dot after byte eight.
    size_t len = strlen(label);
    if (len > 11)
        len = 11;
    if (len > 8) {
        memcpy(info->name, label, 8);
        info->name[8] = '.';
        memcpy(info->name + 9, label + 8, len - 8);
    } else {
        memcpy(info->name, label, len);
    }
    return 0;
}

// AccessResource opens the module's PE file at the resource's raw data offset
// for the existing _lread/_lclose callers. The module must be a loaded PE image.
static inline HFILE AccessResource(HINSTANCE instance, HRSRC resource) {
    HMODULE module = instance ? instance : GetModuleHandleA(NULL);
    HGLOBAL loaded = LoadResource(module, resource);
    if (!loaded)
        return HFILE_ERROR;
    const BYTE *data = (const BYTE *)LockResource(loaded);
    if (!data)
        return HFILE_ERROR;

    const BYTE                 *base = (const BYTE *)module;
    const IMAGE_DOS_HEADER     *dos = (const IMAGE_DOS_HEADER *)base;
    const IMAGE_NT_HEADERS     *nt = (const IMAGE_NT_HEADERS *)(base + dos->e_lfanew);
    const IMAGE_SECTION_HEADER *section = IMAGE_FIRST_SECTION(nt);
    ULONG_PTR                   rva = (ULONG_PTR)data - (ULONG_PTR)base;
    DWORD                       size = SizeofResource(module, resource);

    for (WORD i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++section) {
        if (rva < section->VirtualAddress)
            continue;
        ULONG_PTR offset = rva - section->VirtualAddress;
        if (offset >= section->SizeOfRawData || size > section->SizeOfRawData - offset)
            continue;

        char  path[MAX_PATH];
        DWORD len = GetModuleFileNameA(module, path, sizeof(path));
        if (len == 0 || len >= sizeof(path))
            return HFILE_ERROR;
        HFILE file = _lopen(path, OF_READ | OF_SHARE_DENY_NONE);
        if (file == HFILE_ERROR)
            return HFILE_ERROR;
        if (_llseek(file, (LONG)(section->PointerToRawData + offset), FILE_BEGIN) == HFILE_ERROR) {
            _lclose(file);
            return HFILE_ERROR;
        }
        return file;
    }
    SetLastError(ERROR_RESOURCE_DATA_NOT_FOUND);
    return HFILE_ERROR;
}

/*
 * Win16 message crackers
 *
 * Win32 repacked the parameters of these messages: a handle Win16 carried
 * in a word of lParam fills lParam, and the word it displaced moved to the
 * high word of wParam. The decompiled code reads each repacked parameter
 * through the cracker named in its message rule, defined here with the
 * Win32 packing. Win32 replaced WM_CTLCOLOR with one message per control
 * type, which IS_WM_CTLCOLOR recognizes.
 */
#define GET_WM_ACTIVATE_STATE(wp, lp)         LOWORD(wp)
#define GET_WM_ACTIVATE_FMINIMIZED(wp, lp)    ((BOOL)HIWORD(wp))
#define GET_WM_ACTIVATE_HWND(wp, lp)          ((HWND)(lp))
#define GET_WM_CHARTOITEM_HWND(wp, lp)        ((HWND)(lp))
#define GET_WM_COMMAND_ID(wp, lp)             LOWORD(wp)
#define GET_WM_COMMAND_CMD(wp, lp)            HIWORD(wp)
#define GET_WM_COMMAND_HWND(wp, lp)           ((HWND)(lp))
#define GET_WM_CTLCOLOR_HWND(wp, lp)          ((HWND)(lp))
#define GET_WM_ENTERIDLE_HWND(wp, lp)         ((HWND)(lp))
#define GET_WM_HSCROLL_CODE(wp, lp)           LOWORD(wp)
#define GET_WM_HSCROLL_POS(wp, lp)            ((short)HIWORD(wp))
#define GET_WM_HSCROLL_HWND(wp, lp)           ((HWND)(lp))
#define GET_WM_MENUSELECT_CMD(wp, lp)         LOWORD(wp)
#define GET_WM_MENUSELECT_FLAGS(wp, lp)       HIWORD(wp)
#define GET_WM_MENUSELECT_HMENU(wp, lp)       ((HMENU)(lp))
#define GET_WM_PARENTNOTIFY_MSG(wp, lp)       LOWORD(wp)
#define GET_WM_PARENTNOTIFY_ID(wp, lp)        HIWORD(wp)
#define GET_WM_PARENTNOTIFY_HWNDCHILD(wp, lp) ((HWND)(lp))
#define GET_WM_VKEYTOITEM_HWND(wp, lp)        ((HWND)(lp))
#define GET_WM_VSCROLL_CODE(wp, lp)           LOWORD(wp)
#define GET_WM_VSCROLL_POS(wp, lp)            ((short)HIWORD(wp))
#define GET_WM_VSCROLL_HWND(wp, lp)           ((HWND)(lp))
#define IS_WM_CTLCOLOR(msg)                   ((msg) >= WM_CTLCOLORMSGBOX && (msg) <= WM_CTLCOLORSTATIC)

// Win16 constants. windows.h provides most of them; the rest, such as
// Win16-only messages and application WM_USER messages, are defined here.
#ifndef BN_CLICKED
#define BN_CLICKED 0x0000
#endif
#ifndef BN_PAINT
#define BN_PAINT 0x0001
#endif
#ifndef BN_HILITE
#define BN_HILITE 0x0002
#endif
#ifndef BN_UNHILITE
#define BN_UNHILITE 0x0003
#endif
#ifndef BN_DISABLE
#define BN_DISABLE 0x0004
#endif
#ifndef BN_DOUBLECLICKED
#define BN_DOUBLECLICKED 0x0005
#endif
#ifndef EN_SETFOCUS
#define EN_SETFOCUS 0x0100
#endif
#ifndef EN_KILLFOCUS
#define EN_KILLFOCUS 0x0200
#endif
#ifndef EN_CHANGE
#define EN_CHANGE 0x0300
#endif
#ifndef EN_UPDATE
#define EN_UPDATE 0x0400
#endif
#ifndef EN_ERRSPACE
#define EN_ERRSPACE 0x0500
#endif
#ifndef EN_MAXTEXT
#define EN_MAXTEXT 0x0501
#endif
#ifndef EN_HSCROLL
#define EN_HSCROLL 0x0601
#endif
#ifndef EN_VSCROLL
#define EN_VSCROLL 0x0602
#endif
#ifndef LBN_ERRSPACE
#define LBN_ERRSPACE (-2)
#endif
#ifndef LBN_SELCHANGE
#define LBN_SELCHANGE 0x0001
#endif
#ifndef LBN_DBLCLK
#define LBN_DBLCLK 0x0002
#endif
#ifndef LBN_SELCANCEL
#define LBN_SELCANCEL 0x0003
#endif
#ifndef LBN_SETFOCUS
#define LBN_SETFOCUS 0x0004
#endif
#ifndef LBN_KILLFOCUS
#define LBN_KILLFOCUS 0x0005
#endif
#ifndef CBN_ERRSPACE
#define CBN_ERRSPACE (-1)
#endif
#ifndef CBN_SELCHANGE
#define CBN_SELCHANGE 0x0001
#endif
#ifndef CBN_DBLCLK
#define CBN_DBLCLK 0x0002
#endif
#ifndef CBN_SETFOCUS
#define CBN_SETFOCUS 0x0003
#endif
#ifndef CBN_KILLFOCUS
#define CBN_KILLFOCUS 0x0004
#endif
#ifndef CBN_EDITCHANGE
#define CBN_EDITCHANGE 0x0005
#endif
#ifndef CBN_EDITUPDATE
#define CBN_EDITUPDATE 0x0006
#endif
#ifndef CBN_DROPDOWN
#define CBN_DROPDOWN 0x0007
#endif
#ifndef CBN_CLOSEUP
#define CBN_CLOSEUP 0x0008
#endif
#ifndef CBN_SELENDOK
#define CBN_SELENDOK 0x0009
#endif
#ifndef CBN_SELENDCANCEL
#define CBN_SELENDCANCEL 0x000A
#endif
#ifndef SB_LINEUP
#define SB_LINEUP 0x0000
#endif
#ifndef SB_LINELEFT
#define SB_LINELEFT 0x0000
#endif
#ifndef SB_LINEDOWN
#define SB_LINEDOWN 0x0001
#endif
#ifndef SB_LINERIGHT
#define SB_LINERIGHT 0x0001
#endif
#ifndef SB_PAGEUP
#define SB_PAGEUP 0x0002
#endif
#ifndef SB_PAGELEFT
#define SB_PAGELEFT 0x0002
#endif
#ifndef SB_PAGEDOWN
#define SB_PAGEDOWN 0x0003
#endif
#ifndef SB_PAGERIGHT
#define SB_PAGERIGHT 0x0003
#endif
#ifndef SB_THUMBPOSITION
#define SB_THUMBPOSITION 0x0004
#endif
#ifndef SB_THUMBTRACK
#define SB_THUMBTRACK 0x0005
#endif
#ifndef SB_TOP
#define SB_TOP 0x0006
#endif
#ifndef SB_LEFT
#define SB_LEFT 0x0006
#endif
#ifndef SB_BOTTOM
#define SB_BOTTOM 0x0007
#endif
#ifndef SB_RIGHT
#define SB_RIGHT 0x0007
#endif
#ifndef SB_ENDSCROLL
#define SB_ENDSCROLL 0x0008
#endif
#ifndef WM_NULL
#define WM_NULL 0x0000
#endif
#ifndef WM_CREATE
#define WM_CREATE 0x0001
#endif
#ifndef WM_DESTROY
#define WM_DESTROY 0x0002
#endif
#ifndef WM_MOVE
#define WM_MOVE 0x0003
#endif
#ifndef WM_SIZE
#define WM_SIZE 0x0005
#endif
#ifndef WM_ACTIVATE
#define WM_ACTIVATE 0x0006
#endif
#ifndef WM_SETFOCUS
#define WM_SETFOCUS 0x0007
#endif
#ifndef WM_KILLFOCUS
#define WM_KILLFOCUS 0x0008
#endif
#ifndef WM_ENABLE
#define WM_ENABLE 0x000A
#endif
#ifndef WM_SETREDRAW
#define WM_SETREDRAW 0x000B
#endif
#ifndef WM_SETTEXT
#define WM_SETTEXT 0x000C
#endif
#ifndef WM_GETTEXT
#define WM_GETTEXT 0x000D
#endif
#ifndef WM_GETTEXTLENGTH
#define WM_GETTEXTLENGTH 0x000E
#endif
#ifndef WM_PAINT
#define WM_PAINT 0x000F
#endif
#ifndef WM_CLOSE
#define WM_CLOSE 0x0010
#endif
#ifndef WM_QUERYENDSESSION
#define WM_QUERYENDSESSION 0x0011
#endif
#ifndef WM_QUIT
#define WM_QUIT 0x0012
#endif
#ifndef WM_QUERYOPEN
#define WM_QUERYOPEN 0x0013
#endif
#ifndef WM_ERASEBKGND
#define WM_ERASEBKGND 0x0014
#endif
#ifndef WM_SYSCOLORCHANGE
#define WM_SYSCOLORCHANGE 0x0015
#endif
#ifndef WM_ENDSESSION
#define WM_ENDSESSION 0x0016
#endif
#ifndef WM_SYSTEMERROR
#define WM_SYSTEMERROR 0x0017
#endif
#ifndef WM_SHOWWINDOW
#define WM_SHOWWINDOW 0x0018
#endif
#ifndef WM_CTLCOLOR
#define WM_CTLCOLOR 0x0019
#endif
#ifndef WM_WININICHANGE
#define WM_WININICHANGE 0x001A
#endif
#ifndef WM_DEVMODECHANGE
#define WM_DEVMODECHANGE 0x001B
#endif
#ifndef WM_ACTIVATEAPP
#define WM_ACTIVATEAPP 0x001C
#endif
#ifndef WM_FONTCHANGE
#define WM_FONTCHANGE 0x001D
#endif
#ifndef WM_TIMECHANGE
#define WM_TIMECHANGE 0x001E
#endif
#ifndef WM_CANCELMODE
#define WM_CANCELMODE 0x001F
#endif
#ifndef WM_SETCURSOR
#define WM_SETCURSOR 0x0020
#endif
#ifndef WM_MOUSEACTIVATE
#define WM_MOUSEACTIVATE 0x0021
#endif
#ifndef WM_CHILDACTIVATE
#define WM_CHILDACTIVATE 0x0022
#endif
#ifndef WM_QUEUESYNC
#define WM_QUEUESYNC 0x0023
#endif
#ifndef WM_GETMINMAXINFO
#define WM_GETMINMAXINFO 0x0024
#endif
#ifndef WM_ICONERASEBKGND
#define WM_ICONERASEBKGND 0x0027
#endif
#ifndef WM_NEXTDLGCTL
#define WM_NEXTDLGCTL 0x0028
#endif
#ifndef WM_SPOOLERSTATUS
#define WM_SPOOLERSTATUS 0x002A
#endif
#ifndef WM_DRAWITEM
#define WM_DRAWITEM 0x002B
#endif
#ifndef WM_MEASUREITEM
#define WM_MEASUREITEM 0x002C
#endif
#ifndef WM_DELETEITEM
#define WM_DELETEITEM 0x002D
#endif
#ifndef WM_VKEYTOITEM
#define WM_VKEYTOITEM 0x002E
#endif
#ifndef WM_CHARTOITEM
#define WM_CHARTOITEM 0x002F
#endif
#ifndef WM_SETFONT
#define WM_SETFONT 0x0030
#endif
#ifndef WM_GETFONT
#define WM_GETFONT 0x0031
#endif
#ifndef WM_QUERYDRAGICON
#define WM_QUERYDRAGICON 0x0037
#endif
#ifndef WM_COMPAREITEM
#define WM_COMPAREITEM 0x0039
#endif
#ifndef WM_COMPACTING
#define WM_COMPACTING 0x0041
#endif
#ifndef WM_COMMNOTIFY
#define WM_COMMNOTIFY 0x0044
#endif
#ifndef WM_WINDOWPOSCHANGING
#define WM_WINDOWPOSCHANGING 0x0046
#endif
#ifndef WM_WINDOWPOSCHANGED
#define WM_WINDOWPOSCHANGED 0x0047
#endif
#ifndef WM_POWER
#define WM_POWER 0x0048
#endif
#ifndef WM_NCMOUSEMOVE
#define WM_NCMOUSEMOVE 0x00A0
#endif
#ifndef WM_NCLBUTTONDOWN
#define WM_NCLBUTTONDOWN 0x00A1
#endif
#ifndef WM_NCLBUTTONUP
#define WM_NCLBUTTONUP 0x00A2
#endif
#ifndef WM_NCLBUTTONDBLCLK
#define WM_NCLBUTTONDBLCLK 0x00A3
#endif
#ifndef WM_NCRBUTTONDOWN
#define WM_NCRBUTTONDOWN 0x00A4
#endif
#ifndef WM_NCRBUTTONUP
#define WM_NCRBUTTONUP 0x00A5
#endif
#ifndef WM_NCRBUTTONDBLCLK
#define WM_NCRBUTTONDBLCLK 0x00A6
#endif
#ifndef WM_NCMBUTTONDOWN
#define WM_NCMBUTTONDOWN 0x00A7
#endif
#ifndef WM_NCMBUTTONUP
#define WM_NCMBUTTONUP 0x00A8
#endif
#ifndef WM_NCMBUTTONDBLCLK
#define WM_NCMBUTTONDBLCLK 0x00A9
#endif
#ifndef WM_KEYDOWN
#define WM_KEYDOWN 0x0100
#endif
#ifndef WM_KEYUP
#define WM_KEYUP 0x0101
#endif
#ifndef WM_CHAR
#define WM_CHAR 0x0102
#endif
#ifndef WM_DEADCHAR
#define WM_DEADCHAR 0x0103
#endif
#ifndef WM_SYSKEYDOWN
#define WM_SYSKEYDOWN 0x0104
#endif
#ifndef WM_SYSKEYUP
#define WM_SYSKEYUP 0x0105
#endif
#ifndef WM_SYSCHAR
#define WM_SYSCHAR 0x0106
#endif
#ifndef WM_SYSDEADCHAR
#define WM_SYSDEADCHAR 0x0107
#endif
#ifndef WM_INITDIALOG
#define WM_INITDIALOG 0x0110
#endif
#ifndef WM_COMMAND
#define WM_COMMAND 0x0111
#endif
#ifndef WM_SYSCOMMAND
#define WM_SYSCOMMAND 0x0112
#endif
#ifndef WM_TIMER
#define WM_TIMER 0x0113
#endif
#ifndef WM_HSCROLL
#define WM_HSCROLL 0x0114
#endif
#ifndef WM_VSCROLL
#define WM_VSCROLL 0x0115
#endif
#ifndef WM_INITMENU
#define WM_INITMENU 0x0116
#endif
#ifndef WM_INITMENUPOPUP
#define WM_INITMENUPOPUP 0x0117
#endif
#ifndef WM_MENUSELECT
#define WM_MENUSELECT 0x011F
#endif
#ifndef WM_MENUCHAR
#define WM_MENUCHAR 0x0120
#endif
#ifndef WM_ENTERIDLE
#define WM_ENTERIDLE 0x0121
#endif
#ifndef WM_MOUSEMOVE
#define WM_MOUSEMOVE 0x0200
#endif
#ifndef WM_LBUTTONDOWN
#define WM_LBUTTONDOWN 0x0201
#endif
#ifndef WM_LBUTTONUP
#define WM_LBUTTONUP 0x0202
#endif
#ifndef WM_LBUTTONDBLCLK
#define WM_LBUTTONDBLCLK 0x0203
#endif
#ifndef WM_RBUTTONDOWN
#define WM_RBUTTONDOWN 0x0204
#endif
#ifndef WM_RBUTTONUP
#define WM_RBUTTONUP 0x0205
#endif
#ifndef WM_RBUTTONDBLCLK
#define WM_RBUTTONDBLCLK 0x0206
#endif
#ifndef WM_MBUTTONDOWN
#define WM_MBUTTONDOWN 0x0207
#endif
#ifndef WM_MBUTTONUP
#define WM_MBUTTONUP 0x0208
#endif
#ifndef WM_MBUTTONDBLCLK
#define WM_MBUTTONDBLCLK 0x0209
#endif
#ifndef WM_PARENTNOTIFY
#define WM_PARENTNOTIFY 0x0210
#endif
#ifndef WM_MDICREATE
#define WM_MDICREATE 0x0220
#endif
#ifndef WM_MDIDESTROY
#define WM_MDIDESTROY 0x0221
#endif
#ifndef WM_MDIACTIVATE
#define WM_MDIACTIVATE 0x0222
#endif
#ifndef WM_MDIRESTORE
#define WM_MDIRESTORE 0x0223
#endif
#ifndef WM_MDINEXT
#define WM_MDINEXT 0x0224
#endif
#ifndef WM_MDIMAXIMIZE
#define WM_MDIMAXIMIZE 0x0225
#endif
#ifndef WM_MDITILE
#define WM_MDITILE 0x0226
#endif
#ifndef WM_MDICASCADE
#define WM_MDICASCADE 0x0227
#endif
#ifndef WM_MDIICONARRANGE
#define WM_MDIICONARRANGE 0x0228
#endif
#ifndef WM_MDIGETACTIVE
#define WM_MDIGETACTIVE 0x0229
#endif
#ifndef WM_MDISETMENU
#define WM_MDISETMENU 0x0230
#endif
#ifndef WM_DROPFILES
#define WM_DROPFILES 0x0233
#endif
#ifndef WM_CUT
#define WM_CUT 0x0300
#endif
#ifndef WM_COPY
#define WM_COPY 0x0301
#endif
#ifndef WM_PASTE
#define WM_PASTE 0x0302
#endif
#ifndef WM_CLEAR
#define WM_CLEAR 0x0303
#endif
#ifndef WM_UNDO
#define WM_UNDO 0x0304
#endif
#ifndef WM_RENDERFORMAT
#define WM_RENDERFORMAT 0x0305
#endif
#ifndef WM_RENDERALLFORMATS
#define WM_RENDERALLFORMATS 0x0306
#endif
#ifndef WM_DESTROYCLIPBOARD
#define WM_DESTROYCLIPBOARD 0x0307
#endif
#ifndef WM_DRAWCLIPBOARD
#define WM_DRAWCLIPBOARD 0x0308
#endif
#ifndef WM_PAINTCLIPBOARD
#define WM_PAINTCLIPBOARD 0x0309
#endif
#ifndef WM_VSCROLLCLIPBOARD
#define WM_VSCROLLCLIPBOARD 0x030A
#endif
#ifndef WM_SIZECLIPBOARD
#define WM_SIZECLIPBOARD 0x030B
#endif
#ifndef WM_ASKCBFORMATNAME
#define WM_ASKCBFORMATNAME 0x030C
#endif
#ifndef WM_CHANGECBCHAIN
#define WM_CHANGECBCHAIN 0x030D
#endif
#ifndef WM_HSCROLLCLIPBOARD
#define WM_HSCROLLCLIPBOARD 0x030E
#endif
#ifndef WM_QUERYNEWPALETTE
#define WM_QUERYNEWPALETTE 0x030F
#endif
#ifndef WM_PALETTEISCHANGING
#define WM_PALETTEISCHANGING 0x0310
#endif
#ifndef WM_PALETTECHANGED
#define WM_PALETTECHANGED 0x0311
#endif
#ifndef WM_PENWINFIRST
#define WM_PENWINFIRST 0x0380
#endif
#ifndef WM_PENWINLAST
#define WM_PENWINLAST 0x038F
#endif
#ifndef WM_COALESCE_FIRST
#define WM_COALESCE_FIRST 0x0390
#endif
#ifndef WM_COALESCE_LAST
#define WM_COALESCE_LAST 0x039F
#endif
#ifndef WM_USER
#define WM_USER 0x0400
#endif
#ifndef WM_STARS_STARTUP
#define WM_STARS_STARTUP 0x0464
#endif
#ifndef WM_STARS_HOST
#define WM_STARS_HOST 0x0465
#endif
#ifndef WM_STARS_CONTINUE
#define WM_STARS_CONTINUE 0x0466
#endif
#ifndef BM_GETCHECK
#define BM_GETCHECK 0x0400
#endif
#ifndef BM_SETCHECK
#define BM_SETCHECK 0x0401
#endif
#ifndef BM_GETSTATE
#define BM_GETSTATE 0x0402
#endif
#ifndef BM_SETSTATE
#define BM_SETSTATE 0x0403
#endif
#ifndef BM_SETSTYLE
#define BM_SETSTYLE 0x0404
#endif
#ifndef LB_ADDSTRING
#define LB_ADDSTRING 0x0401
#endif
#ifndef LB_INSERTSTRING
#define LB_INSERTSTRING 0x0402
#endif
#ifndef LB_DELETESTRING
#define LB_DELETESTRING 0x0403
#endif
#ifndef LB_RESETCONTENT
#define LB_RESETCONTENT 0x0405
#endif
#ifndef LB_SETSEL
#define LB_SETSEL 0x0406
#endif
#ifndef LB_SETCURSEL
#define LB_SETCURSEL 0x0407
#endif
#ifndef LB_GETSEL
#define LB_GETSEL 0x0408
#endif
#ifndef LB_GETCURSEL
#define LB_GETCURSEL 0x0409
#endif
#ifndef LB_GETTEXT
#define LB_GETTEXT 0x040A
#endif
#ifndef LB_GETTEXTLEN
#define LB_GETTEXTLEN 0x040B
#endif
#ifndef LB_GETCOUNT
#define LB_GETCOUNT 0x040C
#endif
#ifndef LB_SELECTSTRING
#define LB_SELECTSTRING 0x040D
#endif
#ifndef LB_DIR
#define LB_DIR 0x040E
#endif
#ifndef LB_GETTOPINDEX
#define LB_GETTOPINDEX 0x040F
#endif
#ifndef LB_FINDSTRING
#define LB_FINDSTRING 0x0410
#endif
#ifndef LB_GETSELCOUNT
#define LB_GETSELCOUNT 0x0411
#endif
#ifndef LB_GETSELITEMS
#define LB_GETSELITEMS 0x0412
#endif
#ifndef LB_SETTABSTOPS
#define LB_SETTABSTOPS 0x0413
#endif
#ifndef LB_GETHORIZONTALEXTENT
#define LB_GETHORIZONTALEXTENT 0x0414
#endif
#ifndef LB_SETHORIZONTALEXTENT
#define LB_SETHORIZONTALEXTENT 0x0415
#endif
#ifndef LB_SETCOLUMNWIDTH
#define LB_SETCOLUMNWIDTH 0x0416
#endif
#ifndef LB_SETTOPINDEX
#define LB_SETTOPINDEX 0x0418
#endif
#ifndef LB_GETITEMRECT
#define LB_GETITEMRECT 0x0419
#endif
#ifndef LB_GETITEMDATA
#define LB_GETITEMDATA 0x041A
#endif
#ifndef LB_SETITEMDATA
#define LB_SETITEMDATA 0x041B
#endif
#ifndef LB_SELITEMRANGE
#define LB_SELITEMRANGE 0x041C
#endif
#ifndef LB_SETCARETINDEX
#define LB_SETCARETINDEX 0x041F
#endif
#ifndef LB_GETCARETINDEX
#define LB_GETCARETINDEX 0x0420
#endif
#ifndef LB_SETITEMHEIGHT
#define LB_SETITEMHEIGHT 0x0421
#endif
#ifndef LB_GETITEMHEIGHT
#define LB_GETITEMHEIGHT 0x0422
#endif
#ifndef LB_FINDSTRINGEXACT
#define LB_FINDSTRINGEXACT 0x0423
#endif
#ifndef CB_GETEDITSEL
#define CB_GETEDITSEL 0x0400
#endif
#ifndef CB_LIMITTEXT
#define CB_LIMITTEXT 0x0401
#endif
#ifndef CB_SETEDITSEL
#define CB_SETEDITSEL 0x0402
#endif
#ifndef CB_ADDSTRING
#define CB_ADDSTRING 0x0403
#endif
#ifndef CB_DELETESTRING
#define CB_DELETESTRING 0x0404
#endif
#ifndef CB_DIR
#define CB_DIR 0x0405
#endif
#ifndef CB_GETCOUNT
#define CB_GETCOUNT 0x0406
#endif
#ifndef CB_GETCURSEL
#define CB_GETCURSEL 0x0407
#endif
#ifndef CB_GETLBTEXT
#define CB_GETLBTEXT 0x0408
#endif
#ifndef CB_GETLBTEXTLEN
#define CB_GETLBTEXTLEN 0x0409
#endif
#ifndef CB_INSERTSTRING
#define CB_INSERTSTRING 0x040A
#endif
#ifndef CB_RESETCONTENT
#define CB_RESETCONTENT 0x040B
#endif
#ifndef CB_FINDSTRING
#define CB_FINDSTRING 0x040C
#endif
#ifndef CB_SELECTSTRING
#define CB_SELECTSTRING 0x040D
#endif
#ifndef CB_SETCURSEL
#define CB_SETCURSEL 0x040E
#endif
#ifndef CB_SHOWDROPDOWN
#define CB_SHOWDROPDOWN 0x040F
#endif
#ifndef CB_GETITEMDATA
#define CB_GETITEMDATA 0x0410
#endif
#ifndef CB_SETITEMDATA
#define CB_SETITEMDATA 0x0411
#endif
#ifndef CB_GETDROPPEDCONTROLRECT
#define CB_GETDROPPEDCONTROLRECT 0x0412
#endif
#ifndef CB_SETITEMHEIGHT
#define CB_SETITEMHEIGHT 0x0413
#endif
#ifndef CB_GETITEMHEIGHT
#define CB_GETITEMHEIGHT 0x0414
#endif
#ifndef CB_SETEXTENDEDUI
#define CB_SETEXTENDEDUI 0x0415
#endif
#ifndef CB_GETEXTENDEDUI
#define CB_GETEXTENDEDUI 0x0416
#endif
#ifndef CB_GETDROPPEDSTATE
#define CB_GETDROPPEDSTATE 0x0417
#endif
#ifndef CB_FINDSTRINGEXACT
#define CB_FINDSTRINGEXACT 0x0418
#endif
#ifndef EM_GETSEL
#define EM_GETSEL 0x0400
#endif
#ifndef EM_SETSEL
#define EM_SETSEL 0x0401
#endif
#ifndef EM_GETRECT
#define EM_GETRECT 0x0402
#endif
#ifndef EM_SETRECT
#define EM_SETRECT 0x0403
#endif
#ifndef EM_SETRECTNP
#define EM_SETRECTNP 0x0404
#endif
#ifndef EM_LINESCROLL
#define EM_LINESCROLL 0x0406
#endif
#ifndef EM_GETMODIFY
#define EM_GETMODIFY 0x0408
#endif
#ifndef EM_SETMODIFY
#define EM_SETMODIFY 0x0409
#endif
#ifndef EM_GETLINECOUNT
#define EM_GETLINECOUNT 0x040A
#endif
#ifndef EM_LINEINDEX
#define EM_LINEINDEX 0x040B
#endif
#ifndef EM_SETHANDLE
#define EM_SETHANDLE 0x040C
#endif
#ifndef EM_GETHANDLE
#define EM_GETHANDLE 0x040D
#endif
#ifndef EM_LINELENGTH
#define EM_LINELENGTH 0x0411
#endif
#ifndef EM_REPLACESEL
#define EM_REPLACESEL 0x0412
#endif
#ifndef EM_SETFONT
#define EM_SETFONT 0x0413
#endif
#ifndef EM_GETLINE
#define EM_GETLINE 0x0414
#endif
#ifndef EM_LIMITTEXT
#define EM_LIMITTEXT 0x0415
#endif
#ifndef EM_CANUNDO
#define EM_CANUNDO 0x0416
#endif
#ifndef EM_UNDO
#define EM_UNDO 0x0417
#endif
#ifndef EM_FMTLINES
#define EM_FMTLINES 0x0418
#endif
#ifndef EM_LINEFROMCHAR
#define EM_LINEFROMCHAR 0x0419
#endif
#ifndef EM_SETWORDBREAK
#define EM_SETWORDBREAK 0x041A
#endif
#ifndef EM_SETTABSTOPS
#define EM_SETTABSTOPS 0x041B
#endif
#ifndef EM_SETPASSWORDCHAR
#define EM_SETPASSWORDCHAR 0x041C
#endif
#ifndef EM_EMPTYUNDOBUFFER
#define EM_EMPTYUNDOBUFFER 0x041D
#endif
#ifndef EM_GETFIRSTVISIBLELINE
#define EM_GETFIRSTVISIBLELINE 0x041E
#endif
#ifndef EM_SETREADONLY
#define EM_SETREADONLY 0x041F
#endif
#ifndef EM_SETWORDBREAKPROC
#define EM_SETWORDBREAKPROC 0x0420
#endif
#ifndef EM_GETWORDBREAKPROC
#define EM_GETWORDBREAKPROC 0x0421
#endif
#ifndef EM_GETPASSWORDCHAR
#define EM_GETPASSWORDCHAR 0x0422
#endif
#ifndef VK_BACK
#define VK_BACK 0x0008
#endif
#ifndef VK_TAB
#define VK_TAB 0x0009
#endif
#ifndef VK_RETURN
#define VK_RETURN 0x000D
#endif
#ifndef VK_ESCAPE
#define VK_ESCAPE 0x001B
#endif
#ifndef VK_SPACE
#define VK_SPACE 0x0020
#endif
#ifndef VK_PRIOR
#define VK_PRIOR 0x0021
#endif
#ifndef VK_NEXT
#define VK_NEXT 0x0022
#endif
#ifndef VK_END
#define VK_END 0x0023
#endif
#ifndef VK_HOME
#define VK_HOME 0x0024
#endif
#ifndef VK_LEFT
#define VK_LEFT 0x0025
#endif
#ifndef VK_UP
#define VK_UP 0x0026
#endif
#ifndef VK_RIGHT
#define VK_RIGHT 0x0027
#endif
#ifndef VK_DOWN
#define VK_DOWN 0x0028
#endif
#ifndef VK_INSERT
#define VK_INSERT 0x002D
#endif
#ifndef VK_DELETE
#define VK_DELETE 0x002E
#endif
#ifndef VK_F1
#define VK_F1 0x0070
#endif
#ifndef VK_F2
#define VK_F2 0x0071
#endif
#ifndef VK_F3
#define VK_F3 0x0072
#endif
#ifndef VK_F4
#define VK_F4 0x0073
#endif
#ifndef VK_F5
#define VK_F5 0x0074
#endif
#ifndef VK_F6
#define VK_F6 0x0075
#endif
#ifndef VK_F7
#define VK_F7 0x0076
#endif
#ifndef VK_F8
#define VK_F8 0x0077
#endif
#ifndef VK_F9
#define VK_F9 0x0078
#endif
#ifndef VK_F10
#define VK_F10 0x0079
#endif
#ifndef VK_F11
#define VK_F11 0x007A
#endif
#ifndef VK_F12
#define VK_F12 0x007B
#endif
#ifndef CTLCOLOR_MSGBOX
#define CTLCOLOR_MSGBOX 0x0000
#endif
#ifndef CTLCOLOR_EDIT
#define CTLCOLOR_EDIT 0x0001
#endif
#ifndef CTLCOLOR_LISTBOX
#define CTLCOLOR_LISTBOX 0x0002
#endif
#ifndef CTLCOLOR_BTN
#define CTLCOLOR_BTN 0x0003
#endif
#ifndef CTLCOLOR_DLG
#define CTLCOLOR_DLG 0x0004
#endif
#ifndef CTLCOLOR_SCROLLBAR
#define CTLCOLOR_SCROLLBAR 0x0005
#endif
#ifndef CTLCOLOR_STATIC
#define CTLCOLOR_STATIC 0x0006
#endif
#ifndef MB_OK
#define MB_OK 0x0000
#endif
#ifndef MB_RETRYCANCEL
#define MB_RETRYCANCEL 0x0005
#endif
#ifndef MB_YESNO
#define MB_YESNO 0x0004
#endif
#ifndef MB_YESNOCANCEL
#define MB_YESNOCANCEL 0x0003
#endif
#ifndef MB_ABORTRETRYIGNORE
#define MB_ABORTRETRYIGNORE 0x0002
#endif
#ifndef MB_OKCANCEL
#define MB_OKCANCEL 0x0001
#endif
#ifndef MB_ICONASTERISK
#define MB_ICONASTERISK 0x0040
#endif
#ifndef MB_ICONEXCLAMATION
#define MB_ICONEXCLAMATION 0x0030
#endif
#ifndef MB_ICONQUESTION
#define MB_ICONQUESTION 0x0020
#endif
#ifndef MB_ICONHAND
#define MB_ICONHAND 0x0010
#endif
#ifndef MB_DEFBUTTON3
#define MB_DEFBUTTON3 0x0200
#endif
#ifndef MB_DEFBUTTON2
#define MB_DEFBUTTON2 0x0100
#endif
#ifndef MB_TASKMODAL
#define MB_TASKMODAL 0x2000
#endif
#ifndef MB_SYSTEMMODAL
#define MB_SYSTEMMODAL 0x1000
#endif
#ifndef MB_NOFOCUS
#define MB_NOFOCUS 0x8000
#endif
#ifndef IDOK
#define IDOK 0x0001
#endif
#ifndef IDCANCEL
#define IDCANCEL 0x0002
#endif
#ifndef IDABORT
#define IDABORT 0x0003
#endif
#ifndef IDRETRY
#define IDRETRY 0x0004
#endif
#ifndef IDIGNORE
#define IDIGNORE 0x0005
#endif
#ifndef IDYES
#define IDYES 0x0006
#endif
#ifndef IDNO
#define IDNO 0x0007
#endif
#ifndef BLACKNESS
#define BLACKNESS 0x0042
#endif
#ifndef WHITENESS
#define WHITENESS 0xFF0062
#endif
#ifndef PATCOPY
#define PATCOPY 0xF00021
#endif
#ifndef PATINVERT
#define PATINVERT 0x5A0049
#endif
#ifndef DSTINVERT
#define DSTINVERT 0x550009
#endif
#ifndef SM_CXSCREEN
#define SM_CXSCREEN 0x0000
#endif
#ifndef SM_CYSCREEN
#define SM_CYSCREEN 0x0001
#endif
#ifndef SM_CXVSCROLL
#define SM_CXVSCROLL 0x0002
#endif
#ifndef SM_CYHSCROLL
#define SM_CYHSCROLL 0x0003
#endif
#ifndef SM_CYCAPTION
#define SM_CYCAPTION 0x0004
#endif
#ifndef SM_CXDLGFRAME
#define SM_CXDLGFRAME 0x0007
#endif
#ifndef SM_CYDLGFRAME
#define SM_CYDLGFRAME 0x0008
#endif
#ifndef SM_CXFRAME
#define SM_CXFRAME 0x0020
#endif
#ifndef SM_CYFRAME
#define SM_CYFRAME 0x0021
#endif
#ifndef HORZRES
#define HORZRES 0x0008
#endif
#ifndef VERTRES
#define VERTRES 0x000A
#endif
#ifndef BITSPIXEL
#define BITSPIXEL 0x000C
#endif
#ifndef PLANES
#define PLANES 0x000E
#endif
#ifndef LOGPIXELSX
#define LOGPIXELSX 0x0058
#endif
#ifndef LOGPIXELSY
#define LOGPIXELSY 0x005A
#endif
#ifndef SW_HIDE
#define SW_HIDE 0x0000
#endif
#ifndef SW_SHOWNORMAL
#define SW_SHOWNORMAL 0x0001
#endif
#ifndef SW_SHOWMINIMIZED
#define SW_SHOWMINIMIZED 0x0002
#endif
#ifndef SW_SHOWMAXIMIZED
#define SW_SHOWMAXIMIZED 0x0003
#endif
#ifndef SW_SHOWNOACTIVATE
#define SW_SHOWNOACTIVATE 0x0004
#endif
#ifndef SW_SHOW
#define SW_SHOW 0x0005
#endif
#ifndef SW_MINIMIZE
#define SW_MINIMIZE 0x0006
#endif
#ifndef SW_SHOWMINNOACTIVE
#define SW_SHOWMINNOACTIVE 0x0007
#endif
#ifndef SW_SHOWNA
#define SW_SHOWNA 0x0008
#endif
#ifndef SW_RESTORE
#define SW_RESTORE 0x0009
#endif
#ifndef WS_OVERLAPPED
#define WS_OVERLAPPED 0x0000
#endif
#ifndef WS_POPUP
#define WS_POPUP 0x80000000
#endif
#ifndef WS_CHILD
#define WS_CHILD 0x40000000
#endif
#ifndef WS_CLIPSIBLINGS
#define WS_CLIPSIBLINGS 0x4000000
#endif
#ifndef WS_CLIPCHILDREN
#define WS_CLIPCHILDREN 0x2000000
#endif
#ifndef WS_VISIBLE
#define WS_VISIBLE 0x10000000
#endif
#ifndef WS_DISABLED
#define WS_DISABLED 0x8000000
#endif
#ifndef WS_MINIMIZE
#define WS_MINIMIZE 0x20000000
#endif
#ifndef WS_MAXIMIZE
#define WS_MAXIMIZE 0x1000000
#endif
#ifndef WS_CAPTION
#define WS_CAPTION 0xC00000
#endif
#ifndef WS_BORDER
#define WS_BORDER 0x800000
#endif
#ifndef WS_DLGFRAME
#define WS_DLGFRAME 0x400000
#endif
#ifndef WS_VSCROLL
#define WS_VSCROLL 0x200000
#endif
#ifndef WS_HSCROLL
#define WS_HSCROLL 0x100000
#endif
#ifndef WS_SYSMENU
#define WS_SYSMENU 0x80000
#endif
#ifndef WS_THICKFRAME
#define WS_THICKFRAME 0x40000
#endif
#ifndef WS_MINIMIZEBOX
#define WS_MINIMIZEBOX 0x20000
#endif
#ifndef WS_MAXIMIZEBOX
#define WS_MAXIMIZEBOX 0x10000
#endif
#ifndef WS_GROUP
#define WS_GROUP 0x20000
#endif
#ifndef WS_TABSTOP
#define WS_TABSTOP 0x10000
#endif
#ifndef WS_OVERLAPPEDWINDOW
#define WS_OVERLAPPEDWINDOW 0xCF0000
#endif
#ifndef WS_POPUPWINDOW
#define WS_POPUPWINDOW 0x80880000
#endif
#ifndef WS_CHILDWINDOW
#define WS_CHILDWINDOW 0x40000000
#endif
#ifndef WS_TILED
#define WS_TILED 0x0000
#endif
#ifndef WS_ICONIC
#define WS_ICONIC 0x20000000
#endif
#ifndef WS_SIZEBOX
#define WS_SIZEBOX 0x40000
#endif
#ifndef WS_TILEDWINDOW
#define WS_TILEDWINDOW 0xCF0000
#endif
#ifndef LBS_NOTIFY
#define LBS_NOTIFY 0x0001
#endif
#ifndef LBS_SORT
#define LBS_SORT 0x0002
#endif
#ifndef LBS_NOREDRAW
#define LBS_NOREDRAW 0x0004
#endif
#ifndef LBS_MULTIPLESEL
#define LBS_MULTIPLESEL 0x0008
#endif
#ifndef LBS_OWNERDRAWFIXED
#define LBS_OWNERDRAWFIXED 0x0010
#endif
#ifndef LBS_OWNERDRAWVARIABLE
#define LBS_OWNERDRAWVARIABLE 0x0020
#endif
#ifndef LBS_HASSTRINGS
#define LBS_HASSTRINGS 0x0040
#endif
#ifndef LBS_USETABSTOPS
#define LBS_USETABSTOPS 0x0080
#endif
#ifndef LBS_NOINTEGRALHEIGHT
#define LBS_NOINTEGRALHEIGHT 0x0100
#endif
#ifndef LBS_MULTICOLUMN
#define LBS_MULTICOLUMN 0x0200
#endif
#ifndef LBS_WANTKEYBOARDINPUT
#define LBS_WANTKEYBOARDINPUT 0x0400
#endif
#ifndef LBS_EXTENDEDSEL
#define LBS_EXTENDEDSEL 0x0800
#endif
#ifndef LBS_DISABLENOSCROLL
#define LBS_DISABLENOSCROLL 0x1000
#endif
#ifndef CBS_DROPDOWNLIST
#define CBS_DROPDOWNLIST 0x0003
#endif
#ifndef CBS_DROPDOWN
#define CBS_DROPDOWN 0x0002
#endif
#ifndef CBS_SIMPLE
#define CBS_SIMPLE 0x0001
#endif
#ifndef CBS_OWNERDRAWFIXED
#define CBS_OWNERDRAWFIXED 0x0010
#endif
#ifndef CBS_OWNERDRAWVARIABLE
#define CBS_OWNERDRAWVARIABLE 0x0020
#endif
#ifndef CBS_AUTOHSCROLL
#define CBS_AUTOHSCROLL 0x0040
#endif
#ifndef CBS_OEMCONVERT
#define CBS_OEMCONVERT 0x0080
#endif
#ifndef CBS_SORT
#define CBS_SORT 0x0100
#endif
#ifndef CBS_HASSTRINGS
#define CBS_HASSTRINGS 0x0200
#endif
#ifndef CBS_NOINTEGRALHEIGHT
#define CBS_NOINTEGRALHEIGHT 0x0400
#endif
#ifndef CBS_DISABLENOSCROLL
#define CBS_DISABLENOSCROLL 0x0800
#endif
#ifndef ES_CENTER
#define ES_CENTER 0x0001
#endif
#ifndef ES_RIGHT
#define ES_RIGHT 0x0002
#endif
#ifndef ES_MULTILINE
#define ES_MULTILINE 0x0004
#endif
#ifndef ES_UPPERCASE
#define ES_UPPERCASE 0x0008
#endif
#ifndef ES_LOWERCASE
#define ES_LOWERCASE 0x0010
#endif
#ifndef ES_PASSWORD
#define ES_PASSWORD 0x0020
#endif
#ifndef ES_AUTOVSCROLL
#define ES_AUTOVSCROLL 0x0040
#endif
#ifndef ES_AUTOHSCROLL
#define ES_AUTOHSCROLL 0x0080
#endif
#ifndef ES_NOHIDESEL
#define ES_NOHIDESEL 0x0100
#endif
#ifndef ES_OEMCONVERT
#define ES_OEMCONVERT 0x0400
#endif
#ifndef ES_READONLY
#define ES_READONLY 0x0800
#endif
#ifndef ES_WANTRETURN
#define ES_WANTRETURN 0x1000
#endif
#ifndef BS_OWNERDRAW
#define BS_OWNERDRAW 0x000B
#endif
#ifndef BS_AUTORADIOBUTTON
#define BS_AUTORADIOBUTTON 0x0009
#endif
#ifndef BS_USERBUTTON
#define BS_USERBUTTON 0x0008
#endif
#ifndef BS_GROUPBOX
#define BS_GROUPBOX 0x0007
#endif
#ifndef BS_AUTO3STATE
#define BS_AUTO3STATE 0x0006
#endif
#ifndef BS_3STATE
#define BS_3STATE 0x0005
#endif
#ifndef BS_RADIOBUTTON
#define BS_RADIOBUTTON 0x0004
#endif
#ifndef BS_AUTOCHECKBOX
#define BS_AUTOCHECKBOX 0x0003
#endif
#ifndef BS_CHECKBOX
#define BS_CHECKBOX 0x0002
#endif
#ifndef BS_DEFPUSHBUTTON
#define BS_DEFPUSHBUTTON 0x0001
#endif
#ifndef BS_LEFTTEXT
#define BS_LEFTTEXT 0x0020
#endif
#ifndef SBS_SIZEBOX
#define SBS_SIZEBOX 0x0008
#endif
#ifndef SBS_VERT
#define SBS_VERT 0x0001
#endif
#ifndef SBS_TOPALIGN
#define SBS_TOPALIGN 0x0002
#endif
#ifndef SBS_BOTTOMALIGN
#define SBS_BOTTOMALIGN 0x0004
#endif
#ifndef WS_EX_DLGMODALFRAME
#define WS_EX_DLGMODALFRAME 0x0001
#endif
#ifndef WS_EX_NOPARENTNOTIFY
#define WS_EX_NOPARENTNOTIFY 0x0004
#endif
#ifndef WS_EX_TOPMOST
#define WS_EX_TOPMOST 0x0008
#endif
#ifndef WS_EX_ACCEPTFILES
#define WS_EX_ACCEPTFILES 0x0010
#endif
#ifndef WS_EX_TRANSPARENT
#define WS_EX_TRANSPARENT 0x0020
#endif
#ifndef SWP_NOSIZE
#define SWP_NOSIZE 0x0001
#endif
#ifndef SWP_NOMOVE
#define SWP_NOMOVE 0x0002
#endif
#ifndef SWP_NOZORDER
#define SWP_NOZORDER 0x0004
#endif
#ifndef SWP_NOREDRAW
#define SWP_NOREDRAW 0x0008
#endif
#ifndef SWP_NOACTIVATE
#define SWP_NOACTIVATE 0x0010
#endif
#ifndef SWP_FRAMECHANGED
#define SWP_FRAMECHANGED 0x0020
#endif
#ifndef SWP_SHOWWINDOW
#define SWP_SHOWWINDOW 0x0040
#endif
#ifndef SWP_HIDEWINDOW
#define SWP_HIDEWINDOW 0x0080
#endif
#ifndef SWP_NOCOPYBITS
#define SWP_NOCOPYBITS 0x0100
#endif
#ifndef SWP_NOOWNERZORDER
#define SWP_NOOWNERZORDER 0x0200
#endif
#ifndef GWL_WNDPROC
#define GWL_WNDPROC (-4)
#endif
#ifndef GWW_HINSTANCE
#define GWW_HINSTANCE (-6)
#endif
#ifndef GWW_HWNDPARENT
#define GWW_HWNDPARENT (-8)
#endif
#ifndef GWW_ID
#define GWW_ID (-12)
#endif
#ifndef GWL_STYLE
#define GWL_STYLE (-16)
#endif
#ifndef GWL_EXSTYLE
#define GWL_EXSTYLE (-20)
#endif
#ifndef GW_HWNDFIRST
#define GW_HWNDFIRST 0x0000
#endif
#ifndef GW_HWNDLAST
#define GW_HWNDLAST 0x0001
#endif
#ifndef GW_HWNDNEXT
#define GW_HWNDNEXT 0x0002
#endif
#ifndef GW_HWNDPREV
#define GW_HWNDPREV 0x0003
#endif
#ifndef GW_OWNER
#define GW_OWNER 0x0004
#endif
#ifndef GW_CHILD
#define GW_CHILD 0x0005
#endif
#ifndef HELP_CONTEXT
#define HELP_CONTEXT 0x0001
#endif
#ifndef HELP_QUIT
#define HELP_QUIT 0x0002
#endif
#ifndef HELP_INDEX
#define HELP_INDEX 0x0003
#endif
#ifndef WHITE_BRUSH
#define WHITE_BRUSH 0x0000
#endif
#ifndef LTGRAY_BRUSH
#define LTGRAY_BRUSH 0x0001
#endif
#ifndef GRAY_BRUSH
#define GRAY_BRUSH 0x0002
#endif
#ifndef DKGRAY_BRUSH
#define DKGRAY_BRUSH 0x0003
#endif
#ifndef BLACK_BRUSH
#define BLACK_BRUSH 0x0004
#endif
#ifndef NULL_BRUSH
#define NULL_BRUSH 0x0005
#endif
#ifndef WHITE_PEN
#define WHITE_PEN 0x0006
#endif
#ifndef BLACK_PEN
#define BLACK_PEN 0x0007
#endif
#ifndef NULL_PEN
#define NULL_PEN 0x0008
#endif
#ifndef OEM_FIXED_FONT
#define OEM_FIXED_FONT 0x000A
#endif
#ifndef ANSI_FIXED_FONT
#define ANSI_FIXED_FONT 0x000B
#endif
#ifndef ANSI_VAR_FONT
#define ANSI_VAR_FONT 0x000C
#endif
#ifndef SYSTEM_FONT
#define SYSTEM_FONT 0x000D
#endif
#ifndef DEVICE_DEFAULT_FONT
#define DEVICE_DEFAULT_FONT 0x000E
#endif
#ifndef DEFAULT_PALETTE
#define DEFAULT_PALETTE 0x000F
#endif
#ifndef SYSTEM_FIXED_FONT
#define SYSTEM_FIXED_FONT 0x0010
#endif
#ifndef TRANSPARENT
#define TRANSPARENT 0x0001
#endif
#ifndef OPAQUE
#define OPAQUE 0x0002
#endif
#ifndef SRCCOPY
#define SRCCOPY 0xCC0020
#endif
#ifndef SRCPAINT
#define SRCPAINT 0xEE0086
#endif
#ifndef SRCAND
#define SRCAND 0x8800C6
#endif
#ifndef SRCINVERT
#define SRCINVERT 0x660046
#endif
#ifndef SRCERASE
#define SRCERASE 0x440328
#endif
#ifndef NOTSRCCOPY
#define NOTSRCCOPY 0x330008
#endif
#ifndef NOTSRCERASE
#define NOTSRCERASE 0x1100A6
#endif
#ifndef MERGECOPY
#define MERGECOPY 0xC000CA
#endif
#ifndef MERGEPAINT
#define MERGEPAINT 0xBB0226
#endif
#ifndef PATPAINT
#define PATPAINT 0xFB0A09
#endif
#ifndef MF_BYCOMMAND
#define MF_BYCOMMAND 0x0000
#endif
#ifndef MF_GRAYED
#define MF_GRAYED 0x0001
#endif
#ifndef MF_DISABLED
#define MF_DISABLED 0x0002
#endif
#ifndef MF_BITMAP
#define MF_BITMAP 0x0004
#endif
#ifndef MF_CHECKED
#define MF_CHECKED 0x0008
#endif
#ifndef MF_POPUP
#define MF_POPUP 0x0010
#endif
#ifndef MF_MENUBARBREAK
#define MF_MENUBARBREAK 0x0020
#endif
#ifndef MF_MENUBREAK
#define MF_MENUBREAK 0x0040
#endif
#ifndef MF_HILITE
#define MF_HILITE 0x0080
#endif
#ifndef MF_OWNERDRAW
#define MF_OWNERDRAW 0x0100
#endif
#ifndef MF_BYPOSITION
#define MF_BYPOSITION 0x0400
#endif
#ifndef MF_SEPARATOR
#define MF_SEPARATOR 0x0800
#endif
#ifndef TPM_LEFTBUTTON
#define TPM_LEFTBUTTON 0x0000
#endif
#ifndef TPM_RIGHTBUTTON
#define TPM_RIGHTBUTTON 0x0002
#endif
#ifndef TPM_LEFTALIGN
#define TPM_LEFTALIGN 0x0000
#endif
#ifndef TPM_CENTERALIGN
#define TPM_CENTERALIGN 0x0004
#endif
#ifndef TPM_RIGHTALIGN
#define TPM_RIGHTALIGN 0x0008
#endif
#ifndef IDC_ARROW
#define IDC_ARROW 0x7F00
#endif
#ifndef IDC_IBEAM
#define IDC_IBEAM 0x7F01
#endif
#ifndef IDC_WAIT
#define IDC_WAIT 0x7F02
#endif
#ifndef IDC_CROSS
#define IDC_CROSS 0x7F03
#endif
#ifndef OFN_READONLY
#define OFN_READONLY 0x0001
#endif
#ifndef OFN_OVERWRITEPROMPT
#define OFN_OVERWRITEPROMPT 0x0002
#endif
#ifndef OFN_HIDEREADONLY
#define OFN_HIDEREADONLY 0x0004
#endif
#ifndef OFN_NOCHANGEDIR
#define OFN_NOCHANGEDIR 0x0008
#endif
#ifndef OFN_SHOWHELP
#define OFN_SHOWHELP 0x0010
#endif
#ifndef OFN_ENABLEHOOK
#define OFN_ENABLEHOOK 0x0020
#endif
#ifndef OFN_ENABLETEMPLATE
#define OFN_ENABLETEMPLATE 0x0040
#endif
#ifndef OFN_ENABLETEMPLATEHANDLE
#define OFN_ENABLETEMPLATEHANDLE 0x0080
#endif
#ifndef OFN_NOVALIDATE
#define OFN_NOVALIDATE 0x0100
#endif
#ifndef OFN_ALLOWMULTISELECT
#define OFN_ALLOWMULTISELECT 0x0200
#endif
#ifndef OFN_EXTENSIONDIFFERENT
#define OFN_EXTENSIONDIFFERENT 0x0400
#endif
#ifndef OFN_PATHMUSTEXIST
#define OFN_PATHMUSTEXIST 0x0800
#endif
#ifndef OFN_FILEMUSTEXIST
#define OFN_FILEMUSTEXIST 0x1000
#endif
#ifndef OFN_CREATEPROMPT
#define OFN_CREATEPROMPT 0x2000
#endif
#ifndef OFN_SHAREAWARE
#define OFN_SHAREAWARE 0x4000
#endif
#ifndef OFN_NOREADONLYRETURN
#define OFN_NOREADONLYRETURN 0x8000
#endif
#ifndef OFN_NOTESTFILECREATE
#define OFN_NOTESTFILECREATE 0x10000
#endif
#ifndef PD_ALLPAGES
#define PD_ALLPAGES 0x0000
#endif
#ifndef PD_SELECTION
#define PD_SELECTION 0x0001
#endif
#ifndef PD_PAGENUMS
#define PD_PAGENUMS 0x0002
#endif
#ifndef PD_NOSELECTION
#define PD_NOSELECTION 0x0004
#endif
#ifndef PD_NOPAGENUMS
#define PD_NOPAGENUMS 0x0008
#endif
#ifndef PD_COLLATE
#define PD_COLLATE 0x0010
#endif
#ifndef PD_PRINTTOFILE
#define PD_PRINTTOFILE 0x0020
#endif
#ifndef PD_PRINTSETUP
#define PD_PRINTSETUP 0x0040
#endif
#ifndef PD_NOWARNING
#define PD_NOWARNING 0x0080
#endif
#ifndef PD_RETURNDC
#define PD_RETURNDC 0x0100
#endif
#ifndef PD_RETURNIC
#define PD_RETURNIC 0x0200
#endif
#ifndef PD_RETURNDEFAULT
#define PD_RETURNDEFAULT 0x0400
#endif
#ifndef PD_SHOWHELP
#define PD_SHOWHELP 0x0800
#endif
#ifndef PD_ENABLEPRINTHOOK
#define PD_ENABLEPRINTHOOK 0x1000
#endif
#ifndef PD_ENABLESETUPHOOK
#define PD_ENABLESETUPHOOK 0x2000
#endif
#ifndef PD_ENABLEPRINTTEMPLATE
#define PD_ENABLEPRINTTEMPLATE 0x4000
#endif
#ifndef PD_ENABLESETUPTEMPLATE
#define PD_ENABLESETUPTEMPLATE 0x8000
#endif
#ifndef PD_ENABLEPRINTTEMPLATEHANDLE
#define PD_ENABLEPRINTTEMPLATEHANDLE 0x10000
#endif
#ifndef PD_ENABLESETUPTEMPLATEHANDLE
#define PD_ENABLESETUPTEMPLATEHANDLE 0x20000
#endif
#ifndef PD_USEDEVMODECOPIES
#define PD_USEDEVMODECOPIES 0x40000
#endif
#ifndef PD_DISABLEPRINTTOFILE
#define PD_DISABLEPRINTTOFILE 0x80000
#endif
#ifndef PD_HIDEPRINTTOFILE
#define PD_HIDEPRINTTOFILE 0x100000
#endif

#endif
