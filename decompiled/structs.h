#ifndef STARS_DECOMPILED_STRUCTS_H
#define STARS_DECOMPILED_STRUCTS_H

#include <stdint.h>
#include <windows.h>

typedef struct _aipart {
    uint16_t ibit : 4, /* +0x0000 (2) @bit0 */
        iItem : 5,     /* @bit4 */
        cItem : 4,     /* @bit9 */
        fRandom : 3;   /* @bit13 */
} AIPART;              /* size=0x2 */

typedef struct _aistarbase {
    int16_t idPlanet;   /* +0x0000 (2) */
    int16_t cFreighter; /* +0x0002 (2) */
    int16_t rgflid[8];  /* +0x0004 (16) */
} AISTARBASE;           /* size=0x14 */

typedef struct _aihist {
    uint16_t   cbAiHist;  /* +0x0000 (2) */
    int16_t    cStarbase; /* +0x0002 (2) */
    AISTARBASE rgasb[64]; /* +0x0004 (1280) */
} AIHIST;                 /* size=0x504 */

typedef struct _armor {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    int16_t  dp;             /* +0x0034 (2) */
} ARMOR;                     /* size=0x36 */

typedef struct _beam {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    int16_t  dRangeMax;      /* +0x0034 (2) */
    int16_t  dp;             /* +0x0036 (2) */
    int16_t  init;           /* +0x0038 (2) */
    int16_t  grfAbilities;   /* +0x003A (2) */
} BEAM;                      /* size=0x3c */

typedef struct _bomb {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    int16_t  cRounds;        /* +0x0034 (2) */
    int16_t  dDmgCol;        /* +0x0036 (2) */
    int16_t  dDmgBldg;       /* +0x0038 (2) */
} BOMB;                      /* size=0x3a */

typedef struct _btlplan {
    uint16_t iplr : 4,      /* +0x0000 (2) @bit0 */
        iplan : 4,          /* @bit4 */
        mdTactic : 4,       /* @bit8 */
        unused1 : 2,        /* @bit12 */
        fDelete : 1,        /* @bit14 */
        fDumpCargo : 1;     /* @bit15 */
    uint16_t mdTarget1 : 4, /* +0x0002 (2) @bit0 */
        mdTarget2 : 4,      /* @bit4 */
        iplrAttack : 5,     /* @bit8 */
        unused2 : 3;        /* @bit13 */
    char szName[32];        /* +0x0004 (32) */
} BTLPLAN;                  /* size=0x24 */

typedef struct _coldrop {
    int16_t  idFleetSrc;       /* +0x0000 (2) */
    int16_t  idPlr;            /* +0x0002 (2) */
    int16_t  idPlanetDst;      /* +0x0004 (2) */
    uint16_t fCanColonize : 1, /* +0x0006 (2) @bit0 */
        unused : 15;           /* @bit1 */
    int32_t cColonist;         /* +0x0008 (4) */
} COLDROP;                     /* size=0xc */

typedef struct _compart {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
} COMPART;                   /* size=0x34 */

typedef struct _cyberinfo {
    union {
        uint16_t iLstPktDir : 3, /* +0x0000 (2) @bit0 */
            fBltColony : 1,      /* @bit3 */
            fLaunchedPkt : 1,    /* @bit4 */
            iPktTarget : 2,      /* @bit5 */
            fNeedScanPkt : 1,    /* @bit7 */
            unused : 8;          /* @bit8 */
        uint16_t wInfo;          /* +0x0000 (2) */
    };
} CYBERINFO; /* size=0x2 */

typedef struct _cyberinfotemp {
    union {
        uint16_t fIdleColonizers : 1, /* +0x0000 (2) @bit0 */
            cIdleFreighters : 2,      /* @bit1 */
            cFreightersDst : 2,       /* @bit3 */
            fNeedDefenders : 1,       /* @bit5 */
            fDefended : 1,            /* @bit6 */
            fUnderAttack : 1,         /* @bit7 */
            fNeedsMin1 : 1,           /* @bit8 */
            fNeedsMin2 : 1,           /* @bit9 */
            fNeedsMin3 : 1,           /* @bit10 */
            unused : 5;               /* @bit11 */
        uint16_t wInfo1;              /* +0x0000 (2) */
    };
} CYBERINFOTEMP; /* size=0x2 */

typedef struct _dv {
    union {
        uint16_t dp;        /* +0x0000 (2) */
        uint16_t pctSh : 7, /* +0x0000 (2) @bit0 */
            pctDp : 9;      /* @bit7 */
    };
} DV; /* size=0x2 */

typedef struct _engine {
    int16_t  id;              /* +0x0000 (2) */
    char     rgTech[6];       /* +0x0002 (6) */
    char     szName[32];      /* +0x0008 (32) */
    int16_t  cMass;           /* +0x0028 (2) */
    uint16_t resCost;         /* +0x002A (2) */
    int16_t  rgwtOreCost[3];  /* +0x002C (6) */
    int16_t  ibmp;            /* +0x0032 (2) */
    int16_t  grfAbilities;    /* +0x0034 (2) */
    int16_t  rgcFuelUsed[12]; /* +0x0036 (24) */
} ENGINE;                     /* size=0x4e */

typedef struct _fleetid {
    uint16_t ifl : 9, /* +0x0000 (2) @bit0 */
        iplr : 4,     /* @bit9 */
        junk : 3;     /* @bit13 */
} FLEETID;            /* size=0x2 */

typedef struct _framestuff {
    int16_t dx;          /* +0x0000 (2) */
    int16_t dy;          /* +0x0002 (2) */
    int16_t xTop;        /* +0x0004 (2) */
    int16_t y1;          /* +0x0006 (2) */
    int16_t y2;          /* +0x0008 (2) */
    int16_t dxPlanWant;  /* +0x000A (2) */
    int16_t dyMsgWant;   /* +0x000C (2) */
    int16_t dyMinWant;   /* +0x000E (2) */
    int16_t dx2PlanWant; /* +0x0010 (2) */
    int16_t dy2MsgWant;  /* +0x0012 (2) */
    int16_t dy2MinWant;  /* +0x0014 (2) */
} FRAMESTUFF;            /* size=0x16 */

typedef struct _game {
    int32_t lid;         /* +0x0000 (4) */
    int16_t mdSize;      /* +0x0004 (2) */
    int16_t mdDensity;   /* +0x0006 (2) */
    int16_t cPlayer;     /* +0x0008 (2) */
    int16_t cPlanMax;    /* +0x000A (2) */
    int16_t mdStartDist; /* +0x000C (2) */
    int16_t fDirty;      /* +0x000E (2) */
    union {
        uint16_t fExtraFuel : 1, /* +0x0010 (2) @bit0 */
            fSlowTech : 1,       /* @bit1 */
            fSinglePlr : 1,      /* @bit2 */
            fTutorial : 1,       /* @bit3 */
            fAisBand : 1,        /* @bit4 */
            fBBSPlay : 1,        /* @bit5 */
            fVisScores : 1,      /* @bit6 */
            fNoRandom : 1,       /* @bit7 */
            fClumping : 1,       /* @bit8 */
            wGen : 3,            /* @bit9 */
            unused : 4;          /* @bit12 */
        uint16_t wCrap;          /* +0x0010 (2) */
    };
    uint16_t turn;       /* +0x0012 (2) */
    uint8_t  rgvc[12];   /* +0x0014 (12) */
    char     szName[32]; /* +0x0020 (32) */
} GAME;                  /* size=0x40 */

typedef struct _gdata {
    union {
        int32_t grBits; /* +0x0000 (4) */
        struct {
            uint16_t fUnknownShip : 1, /* +0x0000 (2) @bit0 */
                fGeneratingTurn : 1,   /* @bit1 */
                fForceTurn : 1,        /* @bit2 */
                fHostMode : 1,         /* @bit3 */
                fSubmit : 1,           /* @bit4 */
                fNoResearchSav : 1,    /* @bit5 */
                fRadiatingEngine : 1,  /* @bit6 */
                fNoIdleChecks : 1,     /* @bit7 */
                fSendMsgMode : 1,      /* @bit8 */
                fRetryOpens : 1,       /* @bit9 */
                fAisDone : 1,          /* @bit10 */
                fTutorial : 1,         /* @bit11 */
                fGotoVCR : 1,          /* @bit12 */
                fVCRTimer : 1,         /* @bit13 */
                mdScreenSize : 2;      /* @bit14 */
            uint16_t fGameOverMan : 1, /* +0x0002 (2) @bit0 */
                fDontDoLogFiles : 1,   /* @bit1 */
                fFileCrippled : 1,     /* @bit2 */
                fSmallTileMode : 1,    /* @bit3 */
                fAllAis : 1,           /* @bit4 */
                fReadOnly : 1,         /* @bit5 */
                fExitWindows : 1,      /* @bit6 */
                fPartialTurn : 1,      /* @bit7 */
                fSetMassMode : 1,      /* @bit8 */
                fRptSafeDraw : 1,      /* @bit9 */
                fProgressTxt : 1,      /* @bit10 */
                fSoundFX : 1,          /* @bit11 */
                fNoSound : 1,          /* @bit12 */
                fSetRouteMode : 1,     /* @bit13 */
                fBleedingEdge : 1,     /* @bit14 */
                fToolbar : 1;          /* @bit15 */
        };
    };
    union {
        int32_t grBits2; /* +0x0004 (4) */
        struct {
            uint16_t fNoScannerDraw : 1, /* +0x0004 (2) @bit0 */
                fTrialPeriodOver : 1,    /* @bit1 */
                fClose : 1,              /* @bit2 */
                fDontCalcBleed : 1,      /* @bit3 */
                fChgZipOrd : 1,          /* @bit4 */
                fChgZipProd : 1,         /* @bit5 */
                fChgScanner : 1,         /* @bit6 */
                fChgReports : 1,         /* @bit7 */
                fWriteTurnNum : 1,       /* @bit8 */
                fHotSeat : 1,            /* @bit9 */
                fFleetLinkValid : 1,     /* @bit10 */
                fScoreVictory : 2;       /* @bit11 */
            uint16_t iCurGraph : 4,      /* +0x0006 (2) @bit0 */
                fMusic : 1,              /* @bit4 */
                fPerPlayerDumps : 1,     /* @bit5 */
                fNoHostNames : 1;        /* @bit6 */
        };
    };
    uint16_t fUnused2 : 14; /* +0x0008 (2) @bit0 */
} GDATA;                    /* size=0xa */

typedef struct _hb {
    uint16_t cbFree;   /* +0x0000 (2) */
    uint16_t cbBlock;  /* +0x0002 (2) */
    uint16_t cbSlop;   /* +0x0004 (2) */
    uint16_t ibTop;    /* +0x0006 (2) */
    HB      *lphbNext; /* +0x0008 (4) */
    uint16_t hmem;     /* +0x000C (2) */
    uint8_t  ht;       /* +0x000E (1) */
    uint8_t  unused1;  /* +0x000F (1) */
} HB;                  /* size=0x10 */

typedef struct _hdr {
    uint16_t cb : 10, /* +0x0000 (2) @bit0 */
        rt : 6;       /* @bit10 */
} HDR;                /* size=0x2 */

typedef struct _hs {
    HullSlotType grhst;     /* +0x0000 (2) */
    uint16_t     iItem : 8, /* +0x0002 (2) @bit0 */
        cItem : 8;          /* @bit8 */
} HS;                       /* size=0x4 */

typedef struct _hul {
    HulDef   ihuldef;        /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szClass[32];    /* +0x0008 (32) */
    uint16_t wtEmpty;        /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    uint16_t rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    uint16_t wtCargoMax;     /* +0x0034 (2) */
    uint16_t wtFuelMax;      /* +0x0036 (2) */
    uint16_t dp;             /* +0x0038 (2) */
    HS       rghs[16];       /* +0x003A (64) */
    uint8_t  chs;            /* +0x007A (1) */
} HUL;                       /* size=0x7b */

typedef struct _huldef {
    HUL      hul;        /* +0x0000 (123) */
    uint16_t init : 6,   /* +0x007B (2) @bit0 */
        imdAttack : 4,   /* @bit6 */
        imdCategory : 4, /* @bit10 */
        unused : 2;      /* @bit14 */
    uint16_t wrcCargo;   /* +0x007D (2) */
    uint8_t  rgbrc[16];  /* +0x007F (16) */
} HULDEF;                /* size=0x8f */

typedef struct _itemaction {
    uint16_t cQuan : 12, /* +0x0000 (2) @bit0 */
        iAction : 4;     /* @bit12 */
} ITEMACTION;            /* size=0x2 */

typedef struct _kill {
    uint8_t  itok;      /* +0x0000 (1) */
    uint8_t  grfWeapon; /* +0x0001 (1) */
    uint16_t cshKill;   /* +0x0002 (2) */
    uint16_t dpShield;  /* +0x0004 (2) */
    DV       dv;        /* +0x0006 (2) */
} KILL;                 /* size=0x8 */

typedef struct _btlrec {
    uint8_t  itok;       /* +0x0000 (1) */
    uint8_t  brcDest;    /* +0x0001 (1) */
    int16_t  ctok;       /* +0x0002 (2) */
    uint16_t iRound : 4, /* +0x0004 (2) @bit0 */
        dzDis : 4,       /* @bit4 */
        itokAttack : 8;  /* @bit8 */
    KILL rgkill[0];      /* +0x0006 (0) */
} BTLREC;                /* size=0x6 */

typedef struct _btlrec26 {
    uint8_t  itok;       /* +0x0000 (1) */
    uint8_t  brcDest;    /* +0x0001 (1) */
    uint8_t  itokAttack; /* +0x0002 (1) */
    uint8_t  ctok;       /* +0x0003 (1) */
    uint16_t iRound : 4, /* +0x0004 (2) @bit0 */
        dzDis : 4,       /* @bit4 */
        unused : 8;      /* @bit8 */
    KILL rgkill[0];      /* +0x0006 (0) */
} BTLREC26;              /* size=0x6 */

typedef struct _logxfer {
    int16_t    id;         /* +0x0000 (2) */
    GrobjClass grobj;      /* +0x0002 (2) */
    int32_t    rgdItem[5]; /* +0x0004 (20) */
} LOGXFER;                 /* size=0x18 */

typedef struct _logxferf {
    int16_t    id;          /* +0x0000 (2) */
    GrobjClass grobj;       /* +0x0002 (2) */
    int16_t    rgdItem[16]; /* +0x0004 (32) */
} LOGXFERF;                 /* size=0x24 */

typedef struct _lsb {
    uint16_t isb : 4,      /* +0x0000 (2) @bit0 */
        pctDp : 12;        /* @bit4 */
    uint16_t idFling : 10, /* +0x0002 (2) @bit0 */
        iWarpFling : 4,    /* @bit10 */
        unused3 : 2;       /* @bit14 */
} LSB;                     /* size=0x4 */

typedef struct _mdplr {
    uint16_t reserved : 9, /* +0x0000 (2) @bit0 */
        fAi : 1,           /* @bit9 */
        lvlAi : 3,         /* @bit10 */
        idAi : 3;          /* @bit13 */
} MDPLR;                   /* size=0x2 */

typedef struct _mines {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    int16_t  grAbility;      /* +0x0034 (2) */
} MINES;                     /* size=0x36 */

typedef struct _mining {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    int16_t  grAbility;      /* +0x0034 (2) */
} MINING;                    /* size=0x36 */

typedef struct _msgbig {
    int16_t iMsg;       /* +0x0000 (2) */
    int16_t wGoto;      /* +0x0002 (2) */
    int16_t rgParam[7]; /* +0x0004 (14) */
} MSGBIG;               /* size=0x12 */

typedef struct _msghdr {
    uint16_t iMsg : 9, /* +0x0000 (2) @bit0 */
        grWord : 7;    /* @bit9 */
    int16_t wGoto;     /* +0x0002 (2) */
} MSGHDR;              /* size=0x4 */

typedef struct _msgplr {
    MSGPLR *lpmsgplrNext; /* +0x0000 (4) */
    int16_t iPlrFrom;     /* +0x0004 (2) */
    int16_t iPlrTo;       /* +0x0006 (2) */
    int16_t iInRe;        /* +0x0008 (2) */
    int16_t cLen;         /* +0x000A (2) */
    uint8_t rgbMsg[0];    /* +0x000C (0) */
} MSGPLR;                 /* size=0xc */

typedef struct _msgturn {
    uint8_t iPlr : 4, /* +0x0000 (1) @bit0 */
        cbParams : 4; /* @bit4 */
    MSGHDR msghdr;    /* +0x0001 (4) */
} MSGTURN;            /* size=0x5 */

typedef struct _obj {
    union {
        PLANET *ppl; /* +0x0000 (2) */
        FLEET  *pfl; /* +0x0000 (2) */
        THING  *pth; /* +0x0000 (2) */
    };
} OBJ; /* size=0x2 */

typedef struct _part {
    HS hs; /* +0x0000 (4) */
    union {
        COMPART   *pcom;       /* +0x0004 (4) */
        ARMOR     *parmor;     /* +0x0004 (4) */
        HUL       *phul;       /* +0x0004 (4) */
        ENGINE    *pengine;    /* +0x0004 (4) */
        SCANNER   *pscanner;   /* +0x0004 (4) */
        BEAM      *pbeam;      /* +0x0004 (4) */
        TORP      *ptorp;      /* +0x0004 (4) */
        BOMB      *pbomb;      /* +0x0004 (4) */
        SHIELD    *pshield;    /* +0x0004 (4) */
        SPECIAL   *pspecial;   /* +0x0004 (4) */
        SPECIALSB *pspecialsb; /* +0x0004 (4) */
        MINES     *pmines;     /* +0x0004 (4) */
        MINING    *pmining;    /* +0x0004 (4) */
        PLANETARY *pplanetary; /* +0x0004 (4) */
        TERRA     *pterra;     /* +0x0004 (4) */
    };
} PART; /* size=0x8 */

typedef struct _pl {
    uint16_t cbItem : 8, /* +0x0000 (2) @bit0 */
        fMark : 1,       /* @bit8 */
        ht : 3,          /* @bit9 */
        cAlloc : 4;      /* @bit12 */
    uint8_t iMax;        /* +0x0002 (1) */
    uint8_t iMac;        /* +0x0003 (1) */
    uint8_t rgb[0];      /* +0x0004 (0) */
} PL;                    /* size=0x4 */

typedef struct _planet {
    int16_t  id;              /* +0x0000 (2) */
    int16_t  iPlayer;         /* +0x0002 (2) */
    uint16_t det : 8,         /* +0x0004 (2) @bit0 */
        fInclude : 1,         /* @bit8 */
        fStarbase : 1,        /* @bit9 */
        fHomeworld : 1,       /* @bit10 */
        fFirstYear : 1,       /* @bit11 */
        unusedC : 1,          /* @bit12 */
        fWasInhabited : 1,    /* @bit13 */
        unusedD : 2;          /* @bit14 */
    uint8_t rgpctMinLevel[3]; /* +0x0006 (3) */
    uint8_t rgMinConc[3];     /* +0x0009 (3) */
    char    rgEnvVar[3];      /* +0x000C (3) */
    char    rgEnvVarOrig[3];  /* +0x000F (3) */
    union {
        uint16_t uPopGuess : 12, /* +0x0012 (2) @bit0 */
            uDefGuess : 4;       /* @bit12 */
        uint16_t uGuesses;       /* +0x0012 (2) */
    };
    union {
        struct {
            uint32_t iDeltaPop : 8,  /* +0x0014 (4) @bit0 */
                cMines : 12,         /* @bit8 */
                cFactories : 12;     /* @bit20 */
            uint32_t cDefenses : 12, /* +0x0018 (4) @bit0 */
                iScanner : 5,        /* @bit12 */
                unused5 : 5,         /* @bit17 */
                fArtifact : 1,       /* @bit22 */
                fNoResearch : 1,     /* @bit23 */
                unused2 : 8;         /* @bit24 */
        };
        uint8_t rgbImp[8]; /* +0x0014 (8) */
    };
    int32_t rgwtMin[4]; /* +0x001C (16) */
    union {
        struct {
            uint16_t isb : 4,      /* +0x002C (2) @bit0 */
                pctDp : 12;        /* @bit4 */
            uint16_t idFling : 10, /* +0x002E (2) @bit0 */
                iWarpFling : 4,    /* @bit10 */
                fNoHeal : 1,       /* @bit14 */
                unused3 : 1;       /* @bit15 */
        };
        int32_t lStarbase; /* +0x002C (4) */
    };
    union {
        uint16_t idRoute : 10, /* +0x0030 (2) @bit0 */
            unused4 : 6;       /* @bit10 */
        uint16_t wRouting;     /* +0x0030 (2) */
    };
    int16_t turn;     /* +0x0032 (2) */
    PLPROD *lpplprod; /* +0x0034 (4) */
} PLANET;             /* size=0x38 */

typedef struct _planetary {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    int16_t  grAbility;      /* +0x0034 (2) */
} PLANETARY;                 /* size=0x36 */

typedef struct _planetminimal {
    int16_t  id;        /* +0x0000 (2) */
    int16_t  iPlayer;   /* +0x0002 (2) */
    uint16_t det : 8,   /* +0x0004 (2) @bit0 */
        fInclude : 1,   /* @bit8 */
        fStarbase : 1,  /* @bit9 */
        unusedA : 1,    /* @bit10 */
        fFirstYear : 1, /* @bit11 */
        unusedB : 4;    /* @bit12 */
} PLANETMINIMAL;        /* size=0x6 */

typedef struct _planetsome {
    int16_t  id;               /* +0x0000 (2) */
    int16_t  iPlayer;          /* +0x0002 (2) */
    uint16_t det : 8,          /* +0x0004 (2) @bit0 */
        fInclude : 1,          /* @bit8 */
        fStarbase : 1,         /* @bit9 */
        unusedA : 1,           /* @bit10 */
        fFirstYear : 1,        /* @bit11 */
        unusedB : 4;           /* @bit12 */
    uint16_t rgpctMinLevel[3]; /* +0x0006 (6) */
    char     rgMinConc[3];     /* +0x000C (3) */
    char     rgEnvVar[3];      /* +0x000F (3) */
    char     rgEnvVarOrig[3];  /* +0x0012 (3) */
    union {
        uint16_t uPopGuess : 12, /* +0x0015 (2) @bit0 */
            uDefGuess : 4;       /* @bit12 */
        uint16_t uGuesses;       /* +0x0015 (2) */
    };
} PLANETSOME; /* size=0x17 */

typedef struct _fleet {
    union {
        int16_t  id;      /* +0x0000 (2) */
        uint16_t ifl : 9, /* +0x0000 (2) @bit0 */
            iplr : 4,     /* @bit9 */
            junk : 3;     /* @bit13 */
    };
    int16_t  iPlayer;     /* +0x0002 (2) */
    uint16_t det : 8,     /* +0x0004 (2) @bit0 */
        fInclude : 1,     /* @bit8 */
        fRepOrders : 1,   /* @bit9 */
        fDead : 1,        /* @bit10 */
        fDone : 1,        /* @bit11 */
        fBombed : 1,      /* @bit12 */
        fHereAllTurn : 1, /* @bit13 */
        fNoHeal : 1,      /* @bit14 */
        fMark : 1;        /* @bit15 */
    int16_t idPlanet;     /* +0x0006 (2) */
    POINT   pt;           /* +0x0008 (4) */
    int16_t rgcsh[16];    /* +0x000C (32) */
    union {
        DV      rgdv[16]; /* +0x002C (32) */
        int32_t wtFleet;  /* +0x002C (4) */
    };
    int32_t rgwtMin[5]; /* +0x004C (20) */
    uint8_t iplan;      /* +0x0060 (1) */
    uint8_t bUnused;    /* +0x0061 (1) */
    int16_t cord;       /* +0x0062 (2) */
    PLORD  *lpplord;    /* +0x0064 (4) */
    FLEET  *lpflNext;   /* +0x0068 (4) */
    union {
        int32_t lPower; /* +0x006C (4) */
        struct {
            int16_t dMoveLeft; /* +0x006C (2) */
            int16_t dMoveUsed; /* +0x006E (2) */
        };
    };
    int32_t lFuelUsed; /* +0x0070 (4) */
    union {
        int32_t dirLong; /* +0x0074 (4) */
        struct {
            uint16_t dirFltX : 8,  /* +0x0074 (2) @bit0 */
                dirFltY : 8;       /* @bit8 */
            uint16_t iwarpFlt : 4, /* +0x0076 (2) @bit0 */
                fdirValid : 1,     /* @bit4 */
                fCompChg : 1,      /* @bit5 */
                fTargeted : 1,     /* @bit6 */
                fSkipped : 1,      /* @bit7 */
                fUnused : 8;       /* @bit8 */
        };
    };
    char *lpszName; /* +0x0078 (4) */
} FLEET;            /* size=0x7c */

typedef struct _fleetsome {
    int16_t  id;        /* +0x0000 (2) */
    int16_t  iPlayer;   /* +0x0002 (2) */
    uint16_t det : 8,   /* +0x0004 (2) @bit0 */
        fInclude : 1,   /* @bit8 */
        fRepOrders : 1, /* @bit9 */
        fDead : 1,      /* @bit10 */
        fByteCsh : 1,   /* @bit11 */
        unused : 4;     /* @bit12 */
    int16_t idPlanet;   /* +0x0006 (2) */
    POINT   pt;         /* +0x0008 (4) */
} FLEETSOME;            /* size=0xc */

typedef struct _popupdata {
    GrPopupType grPopup; /* +0x0000 (2) */
    union {
        int32_t rgi[5]; /* +0x0002 (20) */
        struct {
            int16_t idPlanet;   /* +0x0002 (2) */
            int16_t iPlanetVar; /* +0x0004 (2) */
            int16_t iPlanVal;   /* +0x0006 (2) */
            int16_t iPlanMin;   /* +0x0008 (2) */
            int16_t iPlanMax;   /* +0x000A (2) */
            int16_t iPlrVal;    /* +0x000C (2) */
            int16_t iPlrMin;    /* +0x000E (2) */
            int16_t iPlrMax;    /* +0x0010 (2) */
        };
        struct {
            int16_t idPlan;   /* +0x0002 (2) */
            int16_t cMax;     /* +0x0004 (2) */
            int16_t cCur;     /* +0x0006 (2) */
            int16_t cOperate; /* +0x0008 (2) */
            int16_t fFactory; /* +0x000A (2) */
        };
        struct {
            int16_t dxOut; /* +0x0002 (2) */
            char   *psz;   /* +0x0004 (2) */
        };
        int16_t iPlayer; /* +0x0002 (2) */
        struct {
            FLEET   *lpfl;       /* +0x0002 (4) */
            int16_t  fRedDamage; /* +0x0006 (2) */
            int16_t  dxDamage;   /* +0x0008 (2) */
            uint16_t grbit;      /* +0x000A (2) */
        };
        PART part; /* +0x0002 (8) */
        struct {
            SHDEF  *lpshdef;     /* +0x0002 (4) */
            int16_t fShowDamage; /* +0x0006 (2) */
            int16_t fHideCounts; /* +0x0008 (2) */
            int16_t fToken;      /* +0x000A (2) */
            int16_t fSummary;    /* +0x000C (2) */
            int16_t itok;        /* +0x000E (2) */
        };
    };
} POPUPDATA; /* size=0x16 */

typedef struct _prod {
    uint32_t cItem : 10, /* +0x0000 (4) @bit0 */
        iItem : 7,       /* @bit10 */
        grobj : 3,       /* @bit17 */
        pct : 7,         /* @bit20 */
        unused : 5;      /* @bit27 */
} PROD;                  /* size=0x4 */

typedef struct PLPROD {
    uint16_t cbItem : 8, /* +0x0000 (2) @bit0 */
        fMark : 1,       /* @bit8 */
        ht : 3,          /* @bit9 */
        cAlloc : 4;      /* @bit12 */
    uint8_t iprodMax;    /* +0x0002 (1) */
    uint8_t iprodMac;    /* +0x0003 (1) */
    PROD    rgprod[0];   /* +0x0004 (0) */
} PLPROD;                /* size=0x4 */

typedef struct _prodq1 {
    union {
        uint16_t w;          /* +0x0000 (2) */
        uint16_t mdIdle : 6, /* +0x0000 (2) @bit0 */
            cQuan : 10;      /* @bit6 */
    };
} PRODQ1; /* size=0x2 */

typedef struct _btn {
    RECT     rc;           /* +0x0000 (8) */
    int16_t  bt;           /* +0x0008 (2) */
    int16_t  iVal;         /* +0x000A (2) */
    uint16_t fVisible : 1, /* +0x000C (2) @bit0 */
        fDisabled : 1,     /* @bit1 */
        iSide : 2,         /* @bit2 */
        fUnused : 12;      /* @bit4 */
} BTN;                     /* size=0xe */

typedef struct _btnt {
    HWND     hwnd;        /* +0x0000 (2) */
    HDC      hdc;         /* +0x0002 (2) */
    RECT     rc;          /* +0x0004 (8) */
    int16_t  dTimer;      /* +0x000C (2) */
    int16_t  btf;         /* +0x000E (2) */
    char    *szText;      /* +0x0010 (2) */
    int32_t  lTicks;      /* +0x0012 (4) */
    uint16_t fFirst : 1,  /* +0x0016 (2) @bit0 */
        fDown : 1,        /* @bit1 */
        fInitDown : 1,    /* @bit2 */
        fCreatedDC : 1,   /* @bit3 */
        fNoEndRedraw : 1, /* @bit4 */
        fUnused : 11;     /* @bit5 */
} BTNT;                   /* size=0x18 */

typedef struct _drawcir {
    int16_t *rgx;        /* +0x0000 (2) */
    int16_t *rgy;        /* +0x0002 (2) */
    int16_t *rgrad;      /* +0x0004 (2) */
    int16_t  cCur;       /* +0x0006 (2) */
    int16_t  cMax;       /* +0x0008 (2) */
    HDC      hdc;        /* +0x000A (2) */
    RECT     rcClip;     /* +0x000C (8) */
    int16_t  fCovered;   /* +0x0014 (2) */
    int16_t  fHollowOut; /* +0x0016 (2) */
} DRAWCIR;               /* size=0x18 */

typedef struct _rpt {
    int32_t grbitVisible; /* +0x0000 (4) */
    int16_t irpt;         /* +0x0004 (2) */
    int16_t cFields;      /* +0x0006 (2) */
    int16_t cFieldFirst;  /* +0x0008 (2) */
    int16_t icolSort;     /* +0x000A (2) */
    int16_t fAscending;   /* +0x000C (2) */
    int16_t irowFirst;    /* +0x000E (2) */
    POINT   ptDlg;        /* +0x0010 (4) */
    POINT   ptSize;       /* +0x0014 (4) */
    int16_t fCached;      /* +0x0018 (2) */
    uint8_t rgbdx[16];    /* +0x001A (16) */
    int16_t cRows;        /* +0x002A (2) */
    int16_t cRowsVis;     /* +0x002C (2) */
    int16_t iSubsort;     /* +0x002E (2) */
    HWND    hwndVScroll;  /* +0x0030 (2) */
    HWND    hwndHScroll;  /* +0x0032 (2) */
    int16_t cColScroll;   /* +0x0034 (2) */
} RPT;                    /* size=0x36 */

typedef struct _rtbof {
    char    rgid[4]; /* +0x0000 (4) */
    int32_t lidGame; /* +0x0004 (4) */
    union {
        uint16_t verInc : 5, /* +0x0008 (2) @bit0 */
            verMinor : 7,    /* @bit5 */
            verMajor : 4;    /* @bit12 */
        uint16_t wVersion;   /* +0x0008 (2) */
    };
    uint16_t turn;        /* +0x000A (2) */
    int16_t  iPlayer : 5, /* +0x000C (2) @bit0 */
        lSaltTime : 11;   /* @bit5 */
    uint16_t dt : 8,      /* +0x000E (2) @bit0 */
        fDone : 1,        /* @bit8 */
        fInUse : 1,       /* @bit9 */
        fMulti : 1,       /* @bit10 */
        fGameOverMan : 1, /* @bit11 */
        fCrippled : 1,    /* @bit12 */
        wGen : 3;         /* @bit13 */
} RTBOF;                  /* size=0x10 */

typedef struct _rtchgname {
    int16_t id;      /* +0x0000 (2) */
    int16_t grobj;   /* +0x0002 (2) */
    uint8_t rgb[33]; /* +0x0004 (33) */
} RTCHGNAME;         /* size=0x25 */

typedef struct _rtChgPlanetLong {
    int16_t id; /* +0x0000 (2) */
    union {
        uint32_t ul;              /* +0x0002 (4) */
        uint32_t fNoResearch : 1, /* +0x0002 (4) @bit0 */
            idFling : 10,         /* @bit1 */
            iWarpFling : 4,       /* @bit11 */
            idRoute : 10,         /* @bit15 */
            unused : 7;           /* @bit25 */
    };
} RTCHGPLANETLONG; /* size=0x6 */

typedef struct _rtChgProdQ {
    int16_t id;        /* +0x0000 (2) */
    PROD    rgprod[0]; /* +0x0002 (0) */
} RTCHGPRODQ;          /* size=0x2 */

typedef struct _rthisthdr {
    int16_t cPlanet;      /* +0x0000 (2) */
    int16_t cPlanetExtra; /* +0x0002 (2) */
} RTHISTHDR;              /* size=0x4 */

typedef struct _rtloghdr {
    int16_t cbLog;         /* +0x0000 (2) */
    int32_t lSerialNumber; /* +0x0002 (4) */
    uint8_t rgbConfig[11]; /* +0x0006 (11) */
} RTLOGHDR;                /* size=0x11 */

typedef struct _rtlogthing {
    uint16_t idFull;    /* +0x0000 (2) */
    int16_t  fDetonate; /* +0x0002 (2) */
} RTLOGTHING;           /* size=0x4 */

typedef struct _rtplanet {
    int16_t id : 11,     /* +0x0000 (2) @bit0 */
        iPlayer : 5;     /* @bit11 */
    uint16_t det : 7,    /* +0x0002 (2) @bit0 */
        fHomeworld : 1,  /* @bit7 */
        fInclude : 1,    /* @bit8 */
        fStarbase : 1,   /* @bit9 */
        fIncEVO : 1,     /* @bit10 */
        fIncImp : 1,     /* @bit11 */
        fIsArtifact : 1, /* @bit12 */
        fIncSurfMin : 1, /* @bit13 */
        fRouting : 1,    /* @bit14 */
        fFirstYear : 1;  /* @bit15 */
} RTPLANET;              /* size=0x4 */

typedef struct _rtshdef {
    union {
        uint16_t det : 8, /* +0x0000 (2) @bit0 */
            fInclude : 1, /* @bit8 */
            fFree : 1,    /* @bit9 */
            ishdef : 5,   /* @bit10 */
            fGift : 1;    /* @bit15 */
        uint16_t wFlags;  /* +0x0000 (2) */
    };
    uint8_t ihuldef; /* +0x0002 (1) */
    uint8_t ibmp;    /* +0x0003 (1) */
    union {
        uint16_t wtEmpty; /* +0x0004 (2) */
        uint16_t dp;      /* +0x0004 (2) */
    };
    uint8_t  chs;     /* +0x0006 (1) */
    uint16_t turn;    /* +0x0007 (2) */
    uint32_t cBuilt;  /* +0x0009 (4) */
    uint32_t cExist;  /* +0x000D (4) */
    HS       rghs[0]; /* +0x0011 (0) */
} RTSHDEF;            /* size=0x11 */

typedef struct _rtchgshdef {
    uint16_t mdChg : 4, /* +0x0000 (2) @bit0 */
        iPlr : 4,       /* @bit4 */
        ishdef : 5,     /* @bit8 */
        junk : 3;       /* @bit13 */
    RTSHDEF rtshdef;    /* +0x0002 (17) */
} RTCHGSHDEF;           /* size=0x13 */

typedef struct _rtshipint {
    int16_t id; /* +0x0000 (2) */
    int16_t i;  /* +0x0002 (2) */
} RTSHIPINT;    /* size=0x4 */

typedef struct _rtshipint2 {
    int16_t id; /* +0x0000 (2) */
    int16_t i;  /* +0x0002 (2) */
    int16_t i2; /* +0x0004 (2) */
} RTSHIPINT2;   /* size=0x6 */

typedef struct _rtxfer {
    uint16_t id1;        /* +0x0000 (2) */
    uint16_t id2;        /* +0x0002 (2) */
    uint8_t  grobj1 : 4, /* +0x0004 (1) @bit0 */
        grobj2 : 4;      /* @bit4 */
    uint8_t grbitItems;  /* +0x0005 (1) */
    char    rgcQuan[1];  /* +0x0006 (1) */
} RTXFER;                /* size=0x7 */

typedef struct _rtxferf {
    uint16_t id1;        /* +0x0000 (2) */
    uint16_t id2;        /* +0x0002 (2) */
    uint8_t  grobj1 : 4, /* +0x0004 (1) @bit0 */
        grobj2 : 4;      /* @bit4 */
    uint16_t grbitItems; /* +0x0005 (2) */
    int16_t  rgcQuan[1]; /* +0x0007 (2) */
} RTXFERF;               /* size=0x9 */

typedef struct _rtxferl {
    uint16_t id1;        /* +0x0000 (2) */
    uint16_t id2;        /* +0x0002 (2) */
    uint8_t  grobj1 : 4, /* +0x0004 (1) @bit0 */
        grobj2 : 4;      /* @bit4 */
    uint8_t grbitItems;  /* +0x0005 (1) */
    int32_t rgcQuan[1];  /* +0x0006 (4) */
} RTXFERL;               /* size=0xa */

typedef struct _rtxferx {
    uint16_t id1;        /* +0x0000 (2) */
    uint16_t id2;        /* +0x0002 (2) */
    uint8_t  grobj1 : 4, /* +0x0004 (1) @bit0 */
        grobj2 : 4;      /* @bit4 */
    uint8_t grbitItems;  /* +0x0005 (1) */
    int16_t rgcQuan[1];  /* +0x0006 (2) */
} RTXFERX;               /* size=0x8 */

typedef struct _sbar {
    int16_t grbit; /* +0x0000 (2) */
    int16_t id;    /* +0x0002 (2) */
    POINT   pt;    /* +0x0004 (4) */
    char   *psz;   /* +0x0008 (2) */
    SCAN   *pscan; /* +0x000A (2) */
} SBAR;            /* size=0xc */

typedef struct _scan {
    POINT      pt;        /* +0x0000 (4) */
    GrobjClass grobj;     /* +0x0004 (2) */
    GrobjClass grobjFull; /* +0x0006 (2) */
    int16_t    idpl;      /* +0x0008 (2) */
    int16_t    ifl;       /* +0x000A (2) */
    int16_t    iwp;       /* +0x000C (2) */
    int16_t    ith;       /* +0x000E (2) */
} SCAN;                   /* size=0x10 */

typedef struct _scanner {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    int16_t  dRange;         /* +0x0034 (2) */
    int16_t  grfAbilities;   /* +0x0036 (2) */
} SCANNER;                   /* size=0x38 */

typedef struct _score {
    int32_t  lScore;      /* +0x0000 (4) */
    int32_t  cResources;  /* +0x0004 (4) */
    int16_t  cPlanet;     /* +0x0008 (2) */
    int16_t  cStarbase;   /* +0x000A (2) */
    uint16_t rgcsh[3];    /* +0x000C (6) */
    int16_t  cTechLevels; /* +0x0012 (2) */
} SCORE;                  /* size=0x14 */

typedef struct _scorex {
    union {
        uint16_t wWord;       /* +0x0000 (2) */
        uint16_t iPlayer : 5, /* +0x0000 (2) @bit0 */
            fValid : 1,       /* @bit5 */
            grbitVC : 8,      /* @bit6 */
            fWinner : 1,      /* @bit14 */
            fHistory : 1;     /* @bit15 */
    };
    union {
        int16_t  iRank; /* +0x0002 (2) */
        uint16_t turn;  /* +0x0002 (2) */
    };
    SCORE score; /* +0x0004 (20) */
} SCOREX;        /* size=0x18 */

typedef struct _selSome {
    POINT      pt;        /* +0x0000 (4) */
    int16_t    grobj;     /* +0x0004 (2) */
    GrobjClass grobjFull; /* +0x0006 (2) */
    int16_t    id;        /* +0x0008 (2) */
    int16_t    iwpAct;    /* +0x000A (2) */
    SCAN       scan;      /* +0x000C (16) */
} SELSOME;                /* size=0x1c */

typedef struct _shdef {
    HUL hul; /* +0x0000 (123) */
    union {
        uint16_t det : 8, /* +0x007B (2) @bit0 */
            fInclude : 1, /* @bit8 */
            fFree : 1,    /* @bit9 */
            ishdef : 5,   /* @bit10 */
            fGift : 1;    /* @bit15 */
        uint16_t wFlags;  /* +0x007B (2) */
    };
    uint16_t turn;   /* +0x007D (2) */
    uint32_t cBuilt; /* +0x007F (4) */
    uint32_t cExist; /* +0x0083 (4) */
    union {
        int32_t lPower;   /* +0x0087 (4) */
        int32_t lVisible; /* +0x0087 (4) */
    };
    uint16_t grbitPlr;    /* +0x008B (2) */
    uint16_t dScanRange;  /* +0x008D (2) */
    uint16_t dScanRange2; /* +0x008F (2) */
    uint8_t  pctDetect;   /* +0x0091 (1) */
    uint8_t  iSteal;      /* +0x0092 (1) */
} SHDEF;                  /* size=0x93 */

typedef struct _shield {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    int16_t  dp;             /* +0x0034 (2) */
} SHIELD;                    /* size=0x36 */

typedef struct _special {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    int16_t  grAbility;      /* +0x0034 (2) */
} SPECIAL;                   /* size=0x36 */

typedef struct _specialsb {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    int16_t  grAbility;      /* +0x0034 (2) */
    int16_t  grAbility2;     /* +0x0036 (2) */
} SPECIALSB;                 /* size=0x38 */

typedef struct _starpack {
    uint32_t dx : 10, /* +0x0000 (4) @bit0 */
        y : 12,       /* @bit10 */
        id : 10;      /* @bit22 */
} STARPACK;           /* size=0x4 */

typedef struct _tasklaymines {
    uint16_t cTime;    /* +0x0000 (2) */
    uint16_t cTimeOld; /* +0x0002 (2) */
} TASKLAYMINES;        /* size=0x4 */

typedef struct _taskpatrol {
    uint16_t iWarp; /* +0x0000 (2) */
    uint16_t iDist; /* +0x0002 (2) */
} TASKPATROL;       /* size=0x4 */

typedef struct _tasksell {
    uint16_t iPlrX; /* +0x0000 (2) */
} TASKSELL;         /* size=0x2 */

typedef struct _taskxport {
    ITEMACTION rgia[5]; /* +0x0000 (10) */
} TASKXPORT;            /* size=0xa */

typedef struct _order {
    POINT    pt;          /* +0x0000 (4) */
    int16_t  id;          /* +0x0004 (2) */
    uint16_t grTask : 4,  /* +0x0006 (2) @bit0 */
        iWarp : 4,        /* @bit4 */
        grobj : 4,        /* @bit8 */
        fValidTask : 1,   /* @bit12 */
        fNoAutoTrack : 1, /* @bit13 */
        fUnused : 2;      /* @bit14 */
    union {
        TASKXPORT    txp;   /* +0x0008 (10) */
        TASKLAYMINES tlm;   /* +0x0008 (4) */
        TASKPATROL   tptl;  /* +0x0008 (4) */
        TASKSELL     tsell; /* +0x0008 (2) */
    };
} ORDER; /* size=0x12 */

typedef struct PLORD {
    uint16_t cbItem : 8, /* +0x0000 (2) @bit0 */
        fMark : 1,       /* @bit8 */
        ht : 3,          /* @bit9 */
        cAlloc : 4;      /* @bit12 */
    uint8_t iordMax;     /* +0x0002 (1) */
    uint8_t iordMac;     /* +0x0003 (1) */
    ORDER   rgord[0];    /* +0x0004 (0) */
} PLORD;                 /* size=0x4 */

typedef struct _rtwaypt {
    int16_t id;     /* +0x0000 (2) */
    int16_t iWaypt; /* +0x0002 (2) */
    ORDER   order;  /* +0x0004 (18) */
} RTWAYPT;          /* size=0x16 */

typedef struct _terra {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    int16_t  grAbility;      /* +0x0034 (2) */
} TERRA;                     /* size=0x36 */

typedef struct _thmine {
    int32_t  cMines;      /* +0x0000 (4) */
    uint16_t grbitPlr;    /* +0x0004 (2) */
    uint8_t  iType;       /* +0x0006 (1) */
    uint8_t  fDetonate;   /* +0x0007 (1) */
    uint16_t grbitPlrNow; /* +0x0008 (2) */
} THMINE;                 /* size=0xa */

typedef struct _thpack {
    uint16_t idPlanet : 10, /* +0x0000 (2) @bit0 */
        iWarp : 4,          /* @bit10 */
        fMoved : 1,         /* @bit14 */
        fInclude : 1;       /* @bit15 */
    int16_t  rgwtMin[3];    /* +0x0002 (6) */
    uint16_t wtMax : 14,    /* +0x0008 (2) @bit0 */
        iDecayRate : 2;     /* @bit14 */
} THPACK;                   /* size=0xa */

typedef struct _thtrader {
    POINT    ptDest;      /* +0x0000 (4) */
    uint16_t iWarp : 4,   /* +0x0004 (2) @bit0 */
        fInclude : 1,     /* @bit4 */
        unused : 11;      /* @bit5 */
    uint16_t grbitPlr;    /* +0x0006 (2) */
    uint16_t grbitTrader; /* +0x0008 (2) */
} THTRADER;               /* size=0xa */

typedef struct _thworm {
    uint16_t iStable : 2,  /* +0x0000 (2) @bit0 */
        cLastMove : 10,    /* @bit2 */
        fDestKnown : 1,    /* @bit12 */
        fInclude : 1;      /* @bit13 */
    uint16_t grbitPlr;     /* +0x0002 (2) */
    uint16_t grbitPlrTrav; /* +0x0004 (2) */
    uint16_t idPartner;    /* +0x0006 (2) */
} THWORM;                  /* size=0x8 */

typedef struct _thing {
    union {
        uint16_t idFull; /* +0x0000 (2) */
        uint16_t id : 9, /* +0x0000 (2) @bit0 */
            iplr : 4,    /* @bit9 */
            ith : 3;     /* @bit13 */
    };
    POINT pt; /* +0x0002 (4) */
    union {
        uint8_t  rgb[10]; /* +0x0006 (10) */
        THMINE   thm;     /* +0x0006 (10) */
        THPACK   thp;     /* +0x0006 (10) */
        THWORM   thw;     /* +0x0006 (8) */
        THTRADER tht;     /* +0x0006 (10) */
    };
    uint16_t turn; /* +0x0010 (2) */
} THING;           /* size=0x12 */

typedef struct _sel {
    POINT      pt;        /* +0x0000 (4) */
    GrobjClass grobj;     /* +0x0004 (2) */
    GrobjClass grobjFull; /* +0x0006 (2) */
    int16_t    id;        /* +0x0008 (2) */
    int16_t    iwpAct;    /* +0x000A (2) */
    SCAN       scan;      /* +0x000C (16) */
    FLEET      fl;        /* +0x001C (124) */
    PLANET     pl;        /* +0x0098 (56) */
    THING      th;        /* +0x00D0 (18) */
} SEL;                    /* size=0xe2 */

typedef struct _tile {
    int16_t yTop;                        /* +0x0000 (2) */
    int16_t dyFull;                      /* +0x0002 (2) */
    int16_t grbit;                       /* +0x0004 (2) */
    void (**pfn)(uint16_t, TILE *, OBJ); /* +0x0006 (4) */
    uint16_t iCol : 3,                   /* +0x000A (2) @bit0 */
        id : 4,                          /* @bit3 */
        fPopped : 1,                     /* @bit7 */
        fNullPtr : 1,                    /* @bit8 */
        fMinTitle : 1,                   /* @bit9 */
        fErase : 1,                      /* @bit10 */
        fFixCtls : 1,                    /* @bit11 */
        fMinDraw : 1;                    /* @bit12 */
    uint16_t fUnused : 4;                /* +0x000C (2) @bit0 */
    uint16_t idh;                        /* +0x000E (2) */
} TILE;                                  /* size=0x10 */

typedef struct _timer {
    int16_t mdForce;        /* +0x0000 (2) */
    int16_t fAutoGenWhenIn; /* +0x0002 (2) */
    union {
        int16_t  hrsForce;      /* +0x0004 (2) */
        uint16_t minForce : 12, /* +0x0004 (2) @bit0 */
            cPlr : 4;           /* @bit12 */
    };
    int32_t tickcount; /* +0x0006 (4) */
} TIMER;               /* size=0xa */

typedef struct _tok {
    uint16_t   id;            /* +0x0000 (2) */
    uint8_t    iplr;          /* +0x0002 (1) */
    GrobjClass grobj;         /* +0x0003 (1) */
    uint8_t    ishdef;        /* +0x0004 (1) */
    uint8_t    brc;           /* +0x0005 (1) */
    uint8_t    initBase;      /* +0x0006 (1) */
    uint8_t    initMin;       /* +0x0007 (1) */
    uint8_t    initMac;       /* +0x0008 (1) */
    uint8_t    itokTarget;    /* +0x0009 (1) */
    uint8_t    pctCloak;      /* +0x000A (1) */
    uint8_t    pctJam;        /* +0x000B (1) */
    uint8_t    pctBC;         /* +0x000C (1) */
    uint8_t    pctCap;        /* +0x000D (1) */
    uint8_t    pctBeamDef;    /* +0x000E (1) */
    uint16_t   wt;            /* +0x000F (2) */
    uint16_t   dpShield;      /* +0x0011 (2) */
    uint16_t   csh;           /* +0x0013 (2) */
    DV         dv;            /* +0x0015 (2) */
    uint16_t   mdTarget1 : 4, /* +0x0017 (2) @bit0 */
        mdTarget2 : 4,        /* @bit4 */
        mdTactic : 4,         /* @bit8 */
        mdTarget0 : 4;        /* @bit12 */
    uint16_t dxyLim : 4,      /* +0x0019 (2) @bit0 */
        dxyMax : 4,           /* @bit4 */
        spd : 4,              /* @bit8 */
        cTarget : 4;          /* @bit12 */
    union {
        uint16_t fActive : 1, /* +0x001B (2) @bit0 */
            fDetector : 1,    /* @bit1 */
            fTorp : 1,        /* @bit2 */
            fRegen : 1,       /* @bit3 */
            fMoved : 1,       /* @bit4 */
            dzDis : 5,        /* @bit5 */
            dwt : 4,          /* @bit10 */
            dMovesLeft : 2;   /* @bit14 */
        uint16_t wFlags;      /* +0x001B (2) */
    };
} TOK; /* size=0x1d */

typedef struct _btldata {
    uint16_t id;       /* +0x0000 (2) */
    uint8_t  cplr;     /* +0x0002 (1) */
    uint8_t  ctok;     /* +0x0003 (1) */
    uint16_t grfPlr;   /* +0x0004 (2) */
    uint16_t cbData;   /* +0x0006 (2) */
    uint16_t idPlanet; /* +0x0008 (2) */
    POINT    pt;       /* +0x000A (4) */
    TOK      rgtok[0]; /* +0x000E (0) */
} BTLDATA;             /* size=0xe */

typedef struct _torp {
    int16_t  id;             /* +0x0000 (2) */
    char     rgTech[6];      /* +0x0002 (6) */
    char     szName[32];     /* +0x0008 (32) */
    int16_t  cMass;          /* +0x0028 (2) */
    uint16_t resCost;        /* +0x002A (2) */
    int16_t  rgwtOreCost[3]; /* +0x002C (6) */
    int16_t  ibmp;           /* +0x0032 (2) */
    int16_t  dRangeMax;      /* +0x0034 (2) */
    int16_t  dp;             /* +0x0036 (2) */
    int16_t  init;           /* +0x0038 (2) */
    int16_t  dHitChance;     /* +0x003A (2) */
} TORP;                      /* size=0x3c */

typedef struct _turnserial {
    int32_t lSerialNumber; /* +0x0000 (4) */
    uint8_t rgbConfig[11]; /* +0x0004 (11) */
    uint8_t bPad;          /* +0x000F (1) */
} TURNSERIAL;              /* size=0x10 */

typedef struct _vers {
    uint16_t verInc : 5, /* +0x0000 (2) @bit0 */
        verMinor : 7,    /* @bit5 */
        verMajor : 4;    /* @bit12 */
} VERS;                  /* size=0x2 */

typedef struct _wn {
    RECT     rc;             /* +0x0000 (8) */
    uint16_t fMaximized : 1, /* +0x0008 (2) @bit0 */
        fMinimized : 1,      /* @bit1 */
        fInitalized : 1,     /* @bit2 */
        fUnused : 13;        /* @bit3 */
} WN;                        /* size=0xa */

typedef struct _ini {
    WN wnFrame; /* +0x0000 (10) */
    union {
        uint16_t fStartupFile : 1, /* +0x000A (2) @bit0 */
            fCmdLine : 1,          /* @bit1 */
            fWait : 1,             /* @bit2 */
            fGen : 1,              /* @bit3 */
            fTry : 1,              /* @bit4 */
            grobjSel : 4,          /* @bit5 */
            fBatch : 1,            /* @bit9 */
            fNewGame : 1,          /* @bit10 */
            fDumpFleets : 1,       /* @bit11 */
            fDumpPlanets : 1,      /* @bit12 */
            fDumpMap : 1,          /* @bit13 */
            fValidate : 1,         /* @bit14 */
            fLogging : 1;          /* @bit15 */
        uint16_t wFlags;           /* +0x000A (2) */
    };
    uint16_t turn;     /* +0x000C (2) */
    int16_t  iObjSel;  /* +0x000E (2) */
    int16_t  idPlayer; /* +0x0010 (2) */
    int32_t  lid;      /* +0x0012 (4) */
    int16_t  cTurnGen; /* +0x0016 (2) */
    int16_t  iMsg;     /* +0x0018 (2) */
} INI;                 /* size=0x1a */

typedef struct _xfer {
    int16_t    id;    /* +0x0000 (2) */
    GrobjClass grobj; /* +0x0002 (2) */
    union {
        FLEET  fl; /* +0x0004 (124) */
        PLANET pl; /* +0x0004 (56) */
        THING  th; /* +0x0004 (18) */
    };
} XFER; /* size=0x80 */

typedef struct _xferfull {
    uint16_t id1;        /* +0x0000 (2) */
    uint16_t id2;        /* +0x0002 (2) */
    uint8_t  grobj1 : 4, /* +0x0004 (1) @bit0 */
        grobj2 : 4;      /* @bit4 */
    int32_t rgcQuan[5];  /* +0x0005 (20) */
} XFERFULL;              /* size=0x19 */

typedef struct _ziporder {
    TASKXPORT txp;        /* +0x0000 (10) */
    char      szName[13]; /* +0x000A (13) */
    uint8_t   fValid;     /* +0x0017 (1) */
} ZIPORDER;               /* size=0x18 */

typedef struct _zipprodq1 {
    uint8_t fNoResearch; /* +0x0000 (1) */
    uint8_t cpq;         /* +0x0001 (1) */
    PRODQ1  rgpq[12];    /* +0x0002 (24) */
} ZIPPRODQ1;             /* size=0x1a */

typedef struct _player {
    char     iPlayer;     /* +0x0000 (1) */
    char     cShDef;      /* +0x0001 (1) */
    int16_t  cPlanet;     /* +0x0002 (2) */
    uint16_t cFleet : 12, /* +0x0004 (2) @bit0 */
        cshdefSB : 4;     /* @bit12 */
    union {
        uint16_t det : 3, /* +0x0006 (2) @bit0 */
            reserved : 9, /* @bit0 */
            iPlrBmp : 5,  /* @bit3 */
            fInclude : 1, /* @bit8 */
            mdPlayer : 7, /* @bit9 */
            fAi : 1,      /* @bit9 */
            lvlAi : 3,    /* @bit10 */
            idAi : 3;     /* @bit13 */
        uint16_t wMdPlr;  /* +0x0006 (2) */
    };
    int16_t  idPlanetHome;   /* +0x0008 (2) */
    uint16_t wScore;         /* +0x000A (2) */
    int32_t  lSalt;          /* +0x000C (4) */
    char     rgEnvVar[3];    /* +0x0010 (3) */
    char     rgEnvVarMin[3]; /* +0x0013 (3) */
    char     rgEnvVarMax[3]; /* +0x0016 (3) */
    char     pctIdealGrowth; /* +0x0019 (1) */
    int8_t   rgTech[6];      /* +0x001A (6) */
    uint32_t rgResSpent[6];  /* +0x0020 (24) */
    char     pctResearch;    /* +0x0038 (1) */
    char     iTechCur;       /* +0x0039 (1) */
    int32_t  lResLastYear;   /* +0x003A (4) */
    char     rgAttr[16];     /* +0x003E (16) */
    uint32_t grbitAttr;      /* +0x004E (4) */
    uint16_t grbitTrader;    /* +0x0052 (2) */
    union {
        uint16_t fDead : 1, /* +0x0054 (2) @bit0 */
            fCrippled : 1,  /* @bit1 */
            fCheater : 1,   /* @bit2 */
            fLearned : 1,   /* @bit3 */
            fHacker : 1,    /* @bit4 */
            unused : 11;    /* @bit5 */
        uint16_t wFlags;    /* +0x0054 (2) */
    };
    ZIPPRODQ1 zpq1;             /* +0x0056 (26) */
    int8_t    rgmdRelation[16]; /* +0x0070 (16) */
    char      szName[32];       /* +0x0080 (32) */
    char      szNames[32];      /* +0x00A0 (32) */
} PLAYER;                       /* size=0xc0 */

typedef struct _tutor {
    union {
        int16_t  wFlags;       /* +0x0000 (2) */
        uint16_t fVisible : 1, /* +0x0000 (2) @bit0 */
            fGameSaved : 1,    /* @bit1 */
            fChange : 1,       /* @bit2 */
            fTurnDone : 1,     /* @bit3 */
            fTutorDone : 1,    /* @bit4 */
            fNoErrors : 1,     /* @bit5 */
            cError : 3,        /* @bit6 */
            fAutoComplete : 1, /* @bit9 */
            fProgress : 1,     /* @bit10 */
            fTBVis : 1,        /* @bit11 */
            fValidQ : 1,       /* @bit12 */
            fFreeing : 1,      /* @bit13 */
            fShowHidMsg : 1,   /* @bit14 */
            unused : 1;        /* @bit15 */
    };
    int16_t   idt;       /* +0x0002 (2) */
    int16_t   idtBold;   /* +0x0004 (2) */
    int16_t   idh;       /* +0x0006 (2) */
    int16_t   idsError;  /* +0x0008 (2) */
    int16_t   iScanZoom; /* +0x000A (2) */
    int16_t   icolFSort; /* +0x000C (2) */
    uint16_t  grbitScan; /* +0x000E (2) */
    HWND      hwnd;      /* +0x0010 (2) */
    ZIPPRODQ1 zpq;       /* +0x0012 (26) */
} TUTOR;                 /* size=0x2c */

typedef struct _zipprodq {
    char    szName[13]; /* +0x0000 (13) */
    uint8_t fValid;     /* +0x000D (1) */
    union {
        ZIPPRODQ1 zpq1; /* +0x000E (26) */
        struct {
            uint8_t fNoResearch; /* +0x000E (1) */
            uint8_t cpq;         /* +0x000F (1) */
            PRODQ1  rgpq[12];    /* +0x0010 (24) */
        };
    };
} ZIPPRODQ; /* size=0x28 */

#endif
