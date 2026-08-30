#include "common.h"

BTLDATA   *vlpbdVCR = 0;
BTLDATA   *vlpbdVCRNext = 0;
BTLPLAN   *rglpbtlplan[16] = {0};
BTLPLAN    btlplan = {0};
BTLREC    *vlpbrVCR = 0;
BTN       *rgbtnXfer = 0;
COLDROP   *lpcd = 0;
COLORREF   crButtonFace = 0;
COLORREF   crButtonHilite = 0;
COLORREF   crButtonShadow = 0;
COLORREF   crButtonText = 0;
COLORREF   crWindow = 0;
COLORREF   crWindowText = 0;
COLORREF   crcRCW = 0;
COLORREF   crgbtnXfer = 0;
FLEET    **rglpfl = 0;
FRAMESTUFF vfs = {0};
GAME       game = {
          .mdSize = 2,
          .mdDensity = 1,
          .cPlayer = 2,
          .mdStartDist = 1,
};
GDATA        gd = {0};
HB          *rglphb[12] = {0};
HBITMAP      hbmpBackBld = 0;
HBITMAP      hbmpMono = 0;
HBITMAP      hbmpMsg = 0;
HBITMAP      hbmpNumbers = 0;
HBITMAP      hbmpScanShip = 0;
HBITMAP      hbmpScanner = 0;
HBITMAP      hbmpUnknownPlanet = 0;
HBRUSH       hbr50Screen = 0;
HBRUSH       hbrBBlue = 0;
HBRUSH       hbrBlue = 0;
HBRUSH       hbrButtonFace = 0;
HBRUSH       hbrButtonHilite = 0;
HBRUSH       hbrButtonShadow = 0;
HBRUSH       hbrButtonText = 0;
HBRUSH       hbrCargo = 0;
HBRUSH       hbrDesktop = 0;
HBRUSH       hbrDkYellow = 0;
HBRUSH       hbrDock = 0;
HBRUSH       hbrEnemy = 0;
HBRUSH       hbrGray = 0;
HBRUSH       hbrGreen = 0;
HBRUSH       hbrLightGray = 0;
HBRUSH       hbrPurple = 0;
HBRUSH       hbrRadar = 0;
HBRUSH       hbrRadarNear = 0;
HBRUSH       hbrRed = 0;
HBRUSH       hbrSelect = 0;
HBRUSH       hbrShip = 0;
HBRUSH       hbrStarbase = 0;
HBRUSH       hbrTooltip = 0;
HBRUSH       hbrWindow = 0;
HBRUSH       hbrWindowFrame = 0;
HBRUSH       hbrWindowText = 0;
HBRUSH       hbrYellow = 0;
HCURSOR      hcurArrowHelp = 0;
HCURSOR      hcurCloseGrab = 0;
HCURSOR      hcurHand = 0;
HCURSOR      hcurNoWay = 0;
HCURSOR      hcurOpenGrab = 0;
HCURSOR      hcurResize4Way = 0;
HCURSOR      hcurResizeNS = 0;
HCURSOR      hcurResizeWE = 0;
HCURSOR      hcurScanAdd = 0;
HCURSOR      hcurScanner = 0;
HCURSOR      hcurTrashCan = 0;
HDR          hdrCur = {0};
HDR          hdrPrev = {0};
HGLOBAL      hdibPlanets = 0;
HGLOBAL      hdibPlaque = 0;
HGLOBAL      hdibRaces = 0;
HGLOBAL      hdibRacesT = 0;
HGLOBAL      hdibRacesX = 0;
HGLOBAL      hdibThings = 0;
HGLOBAL      hdibToolbar = 0;
HICON        hiconHost = 0;
HICON        hiconStars = 0;
HICON        hiconWait = 0;
HPEN         hpenDkBlue = 0;
HPEN         hpenDkGreen = 0;
HPEN         hpenDkPurple = 0;
HPEN         hpenDkYellow = 0;
HPEN         hpenEnemy = 0;
HPEN         hpenMassPath = 0;
HPEN         hpenRadar = 0;
HPEN         hpenRadarNear = 0;
HPEN         hpenShip = 0;
HPEN         hpenStarbase = 0;
HPEN         hpenYellow = 0;
HRGN         hrgnHuge = 0;
HRGN         hrgnScratch = 0;
HS           rghsFutureTech[8] = {0};
HWND         hwndActive = 0;
HWND         hwndBattleDD = 0;
HWND         hwndBrowser = 0;
HWND         hwndBrowserChild = 0;
HWND         hwndFleetCompLB = 0;
HWND         hwndFrame = 0;
HWND         hwndMDIClient = 0;
HWND         hwndMain = 0;
HWND         hwndMessage = 0;
HWND         hwndMine = 0;
HWND         hwndMineCB = 0;
HWND         hwndMsgDrop = 0;
HWND         hwndMsgEdit = 0;
HWND         hwndMsgScroll = 0;
HWND         hwndOrderED = 0;
HWND         hwndPlanet = 0;
HWND         hwndPlanetProdLB = 0;
HWND         hwndPopup = 0;
HWND         hwndProdDlg = 0;
HWND         hwndProgressGauge = 0;
HWND         hwndRaceParent = 0;
HWND         hwndRepCB = 0;
HWND         hwndReportDlg = 0;
HWND         hwndScanner = 0;
HWND         hwndScoreXDlg = 0;
HWND         hwndShipDD = 0;
HWND         hwndShipLB = 0;
HWND         hwndSlotDlg = 0;
HWND         hwndTBRadar = 0;
HWND         hwndTb = 0;
HWND         hwndTitle = 0;
HWND         hwndTooltip = 0;
HWND         hwndVCRDlg = 0;
HWND         hwndZipOrderDlg = 0;
HullSlotType rgmapBuildBmps[21] = {hstEnabled,  hstEngine,    hstScanner, hstShield, hstWeapon,   hstSome,        hstSpecialEM,
                                   hstScanSpec, hstBomb,      hstShArm,   hstArmor,  hstMining,   hstScanSpecArm, hstShWeap,
                                   hstMines,    hstSpecialSB, hstSomeSB,  6400,      hstSpecialE, hstSpecialM,    hstShSpec};
INI          ini = {0};
LOGXFER      lx = {0};
LOGXFERF     lxf = {0};
MSGPLR      *vlpmsgplrIn = 0;
MSGPLR      *vlpmsgplrOut = 0;
MdBuild      mdBuild = mdBuildShdef;
PART         vpartBrowser = {0};
PLANET     **vrglpplAi = 0;
PLANET      *lpPlanets = 0;
PLAYER      *vrgplrNew = 0;
PLAYER       rgplr[16] = {0};
PLAYER       vplr = {0};
PLAYER       vrgplrDef[7] = {{
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 15,
                                 .iPlrBmp = 1,
                                 .wMdPlr = 15,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {15, 15, 15},
                                 .rgEnvVarMax = {85, 85, 85},
                                 .pctIdealGrowth = 15,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 10, 10, 10, 10, 5, 10, 0, 1, 1, 1, 1, 1, 1, 9},
                       },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 103,
                                 .iPlrBmp = 12,
                                 .wMdPlr = 103,
                                 .rgEnvVar = {33, 58, 33},
                                 .rgEnvVarMin = {10, 35, 13},
                                 .rgEnvVarMax = {56, 81, 53},
                                 .pctIdealGrowth = 20,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 10, 9, 17, 10, 9, 10, 4, 0, 0, 2, 1, 1, 2, 7},
                                 .grbitAttr = 2147484931,
                       },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 39,
                                 .iPlrBmp = 4,
                                 .wMdPlr = 39,
                                 .rgEnvVar = {-1, 50, 85},
                                 .rgEnvVarMin = {-1, 0, 70},
                                 .rgEnvVarMax = {-1, 100, 100},
                                 .pctIdealGrowth = 10,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 10, 10, 10, 9, 10, 6, 1, 2, 2, 2, 2, 1, 0, 2},
                                 .grbitAttr = 8456,
                       },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 207,
                                 .iPlrBmp = 25,
                                 .wMdPlr = 207,
                                 .rgEnvVar = {-1, 50, 50},
                                 .rgEnvVarMin = {-1, 12},
                                 .rgEnvVarMax = {-1, 88, 100},
                                 .pctIdealGrowth = 10,
                                 .pctResearch = 15,
                                 .rgAttr = {9, 10, 10, 10, 10, 15, 5, 3, 0, 0, 0, 0, 0, 0, 1},
                                 .grbitAttr = 536870924,
                       },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 47,
                                 .iPlrBmp = 5,
                                 .wMdPlr = 47,
                                 .rgEnvVar = {-1, -1, -1},
                                 .rgEnvVarMin = {-1, -1, -1},
                                 .rgEnvVarMax = {-1, -1, -1},
                                 .pctIdealGrowth = 6,
                                 .pctResearch = 15,
                                 .rgAttr = {8, 12, 12, 15, 10, 9, 10, 3, 1, 1, 2, 2, 1},
                                 .grbitAttr = 4641,
                       },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 151,
                                 .iPlrBmp = 18,
                                 .wMdPlr = 151,
                                 .rgEnvVar = {15, 50, 85},
                                 .rgEnvVarMax = {30, 100, 100},
                                 .pctIdealGrowth = 7,
                                 .pctResearch = 15,
                                 .rgAttr = {7, 11, 10, 18, 10, 10, 10, 0, 2, 0, 2, 2, 2, 2, 5},
                                 .grbitAttr = 1476,
                       },
                             {
                                 .iPlayer = -1,
                                 .det = 7,
                                 .reserved = 255,
                                 .iPlrBmp = 31,
                                 .wMdPlr = 255,
                                 .rgEnvVar = {50, 50, 50},
                                 .rgEnvVarMin = {17, 17, 17},
                                 .rgEnvVarMax = {83, 83, 83},
                                 .pctIdealGrowth = 15,
                                 .pctResearch = 15,
                                 .rgAttr = {10, 10, 10, 10, 10, 3, 10, 0, 1, 1, 1, 1, 1, 1},
                                 .grbitAttr = 1073741824,
                       }};
PLPROD      *lpplProdGlob = 0;
POINT        ptPlaque = {0};
POINT        ptSpeedVCR = {0};
POINT        ptStickyBattlePlansDlg = {.x = -1, .y = -1};
POINT        ptStickyBrowserDlg = {.x = -1, .y = -1};
POINT        ptStickyFindDlg = {.x = -1, .y = -1};
POINT        ptStickyHostModeDlg = {.x = -1, .y = -1};
POINT        ptStickyMergeFleetsDlg = {.x = -1, .y = -1};
POINT        ptStickyNewDlg = {.x = -1, .y = -1};
POINT        ptStickyPrintMapDlg = {.x = -1, .y = -1};
POINT        ptStickyProduceDlg = {.x = -1, .y = -1};
POINT        ptStickyRaceDlg = {.x = -1, .y = -1};
POINT        ptStickyRelationsDlg = {.x = -1, .y = -1};
POINT        ptStickyRenameDlg = {.x = -1, .y = -1};
POINT        ptStickyResDlg = {.x = -1, .y = -1};
POINT        ptStickyScoreXDlg = {.x = -1, .y = -1};
POINT        ptStickySlotDlg = {.x = -1, .y = -1};
POINT        ptStickyTransferDlg = {.x = -1, .y = -1};
POINT        ptStickyTutorDlg = {.x = -1, .y = -1};
POINT        ptStickyVCRDlg = {.x = -1, .y = -1};
POINT        ptStickyZipOrderDlg = {.x = -1, .y = -1};
POINT        ptStickyZipProdDlg = {.x = -1, .y = -1};
POINT        ptslotGlob = {0};
POINT        rgptArrow[5] = {{.x = 3}, {.y = 3}, {.x = -1, .y = 3}, {.x = 2, .y = 3}, {.x = -3, .y = 6}};
POINT        rgptPlan[999] = {0};
POINT        rgptTriangle[3] = {{.x = 4}, {.y = 4}, {.x = -1, .y = 4}};
POINT        vptMsg = {0};
POINT        vptTbLast = {.x = -1, .y = -1};
POPUPDATA    GlobalPD = {0};
PROD        *pProdGlob = 0;
RECT        *vrgrcRCW = 0;
RECT         rcCargo = {0};
RECT         rcMsgText = {0};
RECT         rcMsgTitle = {0};
RECT         rcProdDiamond = {0};
RECT         rcSpinBot = {0};
RECT         rcSpinTop = {0};
RECT         rgrcBuildSpin[2] = {0};
RECT         rgrcRef[19] = {0};
RECT         vrcTooltip = {0};
RECT         vrgrcSlot[16] = {0};
RPT         *vprptCur = 0;
RPT          vrptBattle = {
             .grbitVisible = 65535,
             .irpt = 3,
             .cFields = 15,
             .cFieldFirst = 1,
             .fAscending = 1,
             .ptDlg = {.x = -1, .y = -1},
             .ptSize = {.x = 600, .y = 400},
};
RPT vrptEFleet = {
    .grbitVisible = 65535,
    .irpt = 2,
    .cFields = 12,
    .cFieldFirst = 1,
    .fAscending = 1,
    .ptDlg = {.x = -1, .y = -1},
    .ptSize = {.x = 600, .y = 400},
};
RPT vrptFleet = {
    .grbitVisible = 65535,
    .irpt = 1,
    .cFields = 12,
    .cFieldFirst = 1,
    .fAscending = 1,
    .ptDlg = {.x = -1, .y = -1},
    .ptSize = {.x = 600, .y = 400},
};
RPT vrptPlanet = {
    .grbitVisible = 65535,
    .cFields = 15,
    .cFieldFirst = 1,
    .fAscending = 1,
    .ptDlg = {.x = -1, .y = -1},
    .ptSize = {.x = 600, .y = 400},
};
SCOREX     *rgsxPlr[16] = {0};
SCOREX     *vlprgScoreX = 0;
SEL         sel = {0};
SHDEF      *lpshdefBuild = 0;
SHDEF      *rglpshdefSB[16] = {0};
SHDEF      *rglpshdef[16] = {0};
SHDEF       rgshdef[16] = {0};
SHDEF       shdefBuild = {0};
THING      *lpThings = 0;
THING      *lpthBattle = 0;
TILE        rgtilePlanet[6] = {{
                                   .yTop = 1,
                                   .dyFull = 85,
                                   .grbit = 128,
                                   .pfn = DrawPlanShipBitmap,
                                   .fPopped = 1,
                                   .fMinTitle = 1,
                                   .idh = 1506,
                        },
                               {
                                   .yTop = 6,
                                   .dyFull = 5,
                                   .grbit = 1,
                                   .pfn = DrawPlanetMinSum,
                                   .id = 1,
                                   .fPopped = 1,
                                   .idh = 1509,
                        },
                               {
                                   .yTop = 8,
                                   .dyFull = 6,
                                   .grbit = 8,
                                   .pfn = DrawPlanetStats,
                                   .id = 4,
                                   .fPopped = 1,
                                   .idh = 1508,
                        },
                               {
                                   .yTop = 6,
                                   .dyFull = 22,
                                   .grbit = 4,
                                   .pfn = DrawPlanetShipList,
                                   .iCol = 1,
                                   .id = 5,
                                   .fPopped = 1,
                                   .fNullPtr = 1,
                                   .idh = 1510,
                        },
                               {
                                   .yTop = 10,
                                   .dyFull = 20,
                                   .grbit = 64,
                                   .pfn = DrawPlanetProduction,
                                   .iCol = 1,
                                   .id = 6,
                                   .fPopped = 1,
                                   .idh = 1507,
                        },
                               {
                                   .yTop = 8,
                                   .dyFull = 15,
                                   .grbit = 256,
                                   .pfn = DrawPlanetStarbase,
                                   .iCol = 1,
                                   .id = 7,
                                   .fPopped = 1,
                                   .fNullPtr = 1,
                                   .fMinTitle = 1,
                                   .idh = 1511,
                        }};
TILE        rgtileShip[7] = {{
                                 .yTop = 1,
                                 .dyFull = 85,
                                 .grbit = 128,
                                 .pfn = DrawPlanShipBitmap,
                                 .fPopped = 1,
                                 .fMinTitle = 1,
                                 .idh = 1513,
                      },
                             {
                                 .yTop = 3,
                                 .dyFull = 5,
                                 .grbit = 64,
                                 .pfn = DrawShipPlanet,
                                 .id = 5,
                                 .fPopped = 1,
                                 .fMinTitle = 1,
                                 .idh = 1514,
                      },
                             {
                                 .yTop = 11,
                                 .dyFull = 19,
                                 .grbit = 32,
                                 .id = 3,
                                 .fPopped = 1,
                                 .idh = 1518,
                      },
                             {
                                 .yTop = 6,
                                 .dyFull = 12,
                                 .grbit = 256,
                                 .pfn = DrawShipWayPtOrders,
                                 .id = 4,
                                 .fPopped = 1,
                                 .idh = 1519,
                      },
                             {
                                 .yTop = 7,
                                 .dyFull = 14,
                                 .grbit = 1,
                                 .pfn = DrawShipCargo,
                                 .iCol = 1,
                                 .id = 1,
                                 .fPopped = 1,
                                 .idh = 1515,
                      },
                             {
                                 .yTop = 12,
                                 .dyFull = 16,
                                 .grbit = 512,
                                 .pfn = DrawFleetComp,
                                 .iCol = 1,
                                 .id = 9,
                                 .fPopped = 1,
                                 .idh = 1516,
                      },
                             {
                                 .yTop = 6,
                                 .dyFull = 22,
                                 .grbit = 4,
                                 .pfn = DrawPlanetShipList,
                                 .iCol = 1,
                                 .id = 8,
                                 .fPopped = 1,
                                 .idh = 1517,
                      }};
TIMER       vtimer = {0};
TOK        *vrgtok = 0;
TURNSERIAL *vrgts = 0;
TUTOR       tutor = {0};
XFER       *pxfer = 0;
XFERFULL   *lpxf = 0;
ZIPORDER    vrgZip[4] = {0};
ZIPPRODQ    vrgZipProd[5] = {0};
char       *MPCTD = "m%d";
char       *PCTD = "%d";
char       *PCTDKT = "%dkT";
char       *PCTDPCTPCT = "%d%%";
char       *PCTDXPCTDPCTPCT = "%d.%d%%";
char       *PCTLD = "%ld";
char       *PCTLD00 = "%ld00";
char       *lpbDefMac = 0;
char       *lpbDefUni = 0;
char       *lpchBatch = 0;
char       *lpchBatchMac = 0;
char       *mpdtsz[8] = {"xy", "x", "hst", "m", "h", "r", "log", "chk"};
char       *rgchcompstrlower = " aehilnorstbcdfgjkmpquvwxyz";
char       *rgszMineField[3] = {"Standard", "Heavy", "Speed Bump"};
char       *rgszMinerals[6] = {"Ironium", "Boranium", "Germanium", "Colonists", "Fuel", "Resources"};
char       *rgszPlanetAttrAbbr[3] = {"Grav", "Temp", "Rad"};
char       *rgszPlanetAttr[3] = {"Gravity", "Temperature", "Radiation"};
char       *rgszZipOrder[4] = {"QuikLoad", "QuikDrop", "WaitLoad", "Clear"};
char       *szButton = "BUTTON";
char       *szCombobox = "COMBOBOX";
char       *szDblDash = "-- ";
char       *szEdit = "EDIT";
char       *szHelpFile = "stars!.hlp";
char       *szListbox = "LISTBOX";
char       *vrgszComputerLevel[5] = {"Easy", "Standard", "Tough", "Expert", "Random"};
char       *vrgszComputerPlayers[7] = {"Robotoids", "Turindrones", "Automitrons", "Rototills", "Cybertrons", "Macinti", "Random"};
char       *vrgszFileNew = 0;
char       *vrgszMRU = 0;
char       *vrgszRCWWidth[2] = {"<<     >>", ">>     <<"};
char       *vrgszUnits[6] = {"kT", "kT", "kT", "00", "mg", "% "};
char        iLastGet = -1;
char        iLastMsgGet = -1;
char        iLastStrGet = -1;
char        rgbCur[1024] = "";
char        rgchcomp[13] = "+-,!.?:;'*%$";
char        rgszArial[4][32] = {0};
char        szBackup[256] = "";
char        szBase[256] = "";
char        szBrowser[13] = "starsbrowser";
char        szCRLF[3] = "\r\n";
char        szDirName[256] = "";
char        szFormatNumber[12] = "";
char        szFrame[11] = "starsframe";
char        szLastGet[19] = "";
char        szLastMsgGet[256] = "";
char        szLastStrGet[256] = "";
char        szMessage[13] = "starsmessage";
char        szMine[10] = "starsmine";
char        szMineralTitle[90] = "";
char        szMsgBuf[256] = "";
char        szMsgTitle[90] = "";
char        szPassLast[16] = "";
char        szPlanet[12] = "starsplanet";
char        szPopupBuffer[256] = "";
char        szPopup[11] = "starspopup";
char        szRaceFile[16] = "";
char        szRacePass[16] = "";
char        szReport[12] = "starsreport";
char        szScan[10] = "starsscan";
char        szTb[8] = "starstb";
char        szTitle[11] = "starstitle";
char        szTooltip[8] = "starstt";
char        szWork[360] = "";
char        vszDefPass[17] = "";
double      __fac = 0;
int16_t    *lpMsg = 0;
int16_t    *rgXferValidHulls = 0;
int16_t    *vrgiflMerge = 0;
int16_t     bitTbl[8] = {1, 2, 4, 8, 16, 32, 64, 128};
int16_t     cColDrop = 0;
int16_t     cFleet = 0;
int16_t     cFutureTech = 0;
int16_t     cMinGrafMax = 5000;
int16_t     cMsg = 0;
int16_t     cPlanet = 0;
int16_t     cProdGlob = 0;
int16_t     cRandStack = 0;
int16_t     cThing = 0;
int16_t     cThingAlloc = 0;
int16_t     cXferFull = 0;
int16_t     cXferValidHulls = 0;
int16_t     cbbitfMsg = 49;
int16_t     chbrCache = 0;
int16_t     csh = 0;
int16_t     dGal = 2000;
int16_t     dGalInv = 4000;
int16_t     dGalMinDist = 12;
int16_t     dScanInc = 0;
int16_t     dScanPage = 0;
int16_t     dxBattleDD = 0;
int16_t     dxFleetCompLB = 0;
int16_t     dxMaxMineralQuan = 0;
int16_t     dxOrderED = 0;
int16_t     dxPlanetProdLB = 0;
int16_t     dxResLeft = 0;
int16_t     dxResRadio = 0;
int16_t     dxResRight = 0;
int16_t     dxResStrRight = 0;
int16_t     dxShipDD = 0;
int16_t     dxShipLB = 0;
int16_t     dxTip = 0;
int16_t     dxWinFrame = 0;
int16_t     dxyVCRBoard = 0;
int16_t     dxyVCRSquare = 0;
int16_t     dyArial10 = 0;
int16_t     dyArial6 = 0;
int16_t     dyArial7 = 0;
int16_t     dyArial8 = 0;
int16_t     dyFleetCompLB = 0;
int16_t     dyPlanetProdLB = 0;
int16_t     dySBar = 0;
int16_t     dyShipDD = 0;
int16_t     dyShipLB = 0;
int16_t     dySysFont = 0;
int16_t     dyTitleBar = 0;
int16_t     dyWinFrame = 0;
int16_t     fAi = 0;
int16_t     fAnimate = 0;
int16_t     fBrowserValid = 0;
int16_t     fDirtyPlan = 0;
int16_t     fDlgUp = 0;
int16_t     fFileErrSilent = 0;
int16_t     fFreeingTitle = 0;
int16_t     fHullCopy = 0;
int16_t     fInEditUpdate = 0;
int16_t     fInScoreDialog = 0;
int16_t     fInScrollSet = 0;
int16_t     fLogOff = 0;
int16_t     fLogOut = 1;
int16_t     fMarkedPlanets = 0;
int16_t     fOrdersVis = 0;
int16_t     fProcessingTimer = 0;
int16_t     fRCWReadOnly = 0;
int16_t     fStarbaseDamaged = 0;
int16_t     fStarbaseDied = 0;
int16_t     fStarbaseMode = 0;
int16_t     fValidLx = 0;
int16_t     fValidLxf = 0;
int16_t     fViewFilteredMsg = 0;
int16_t     hf = -1;
int16_t     iAbout1st = 0;
int16_t     iAboutPartial = 0;
int16_t     iLastTutGet = -1;
int16_t     iMsgCur = 0;
int16_t     iMsgSendCur = 0;
int16_t     iPanelActive = 0;
int16_t     iPassCnt = 0;
int16_t     iPlanSelDlg = -1;
int16_t     iPopMenuSel = 0;
int16_t     iResTechNow = 0;
int16_t     iScanZoom = 0;
int16_t     iWindowLayout = 0;
int16_t     idBattle = 0;
int16_t     idMsgObj = 0;
int16_t     idPlayer = 1;
int16_t     idsFileError = -1;
int16_t     imemLogCur = 0;
int16_t     imemLogPrev = -1;
int16_t     imemMsgCur = 0;
int16_t     irowEFleetCur = -1;
int16_t     iselProd = 0;
int16_t     iselSlot = -2;
int16_t     ishdefBuild = 0;
int16_t     mdMsgObj = 0;
int16_t     mdXferDlg = -1;
int16_t     pctResGlob = -1;
int16_t     rgOut[16] = {0};
int16_t     rgcompstrlower[26] = {1, 77, 93, 109, 2, 125, 141, 3, 4, 157, 173, 5, 189, 6, 7, 205, 221, 8, 9, 10, 237, 253, 14, 30, 46, 62};
int16_t     rgcsxPlr[16] = {0};
int16_t     rgdxOrderDD[3] = {0};
int16_t     rgidPlan[999] = {0};
int16_t     vcBackupDirs = 1;
int16_t     vcRound = 0;
int16_t     vcScreenColors = 0;
int16_t     vcStepVCR = 0;
int16_t     vcflMerge = 0;
int16_t     vclpplAi = 0;
int16_t     vcmsgplrIn = 0;
int16_t     vcmsgplrOut = 0;
int16_t     vcplrNew = 0;
int16_t     vctok = 0;
int16_t     vdxScoreX = 0;
int16_t     vfAscendingPrev = 0;
int16_t     viInRe = 0;
int16_t     viRound = 0;
int16_t     viSpeedVCR = 1;
int16_t     viStepVCRCur = 0;
int16_t     viStore = 0;
int16_t     viSubsortPrev = -1;
int16_t     viVCRFocus = 0;
int16_t     vicolSortPrev = -1;
int16_t     vidTimerTooltip = -1;
int16_t     vidsTooltip = 0;
int16_t     vpctProgressGauge = 0;
int16_t     vpctRadarView = 100;
int16_t     vretExitValue = 0;
int16_t     vrgScanPO[2][5] = {{7, 12, 19, 4, 6}, {3, 10, 11, 2, 3}};
int16_t     vrgcPrintMapPage[2] = {1, 1};
int16_t     vyZPDStatic = -1;
int16_t     xNewGameDiamond = 0;
int16_t     xScanTop = 0;
int16_t     yBuildInfoSum = 0;
int16_t     yScanTop = 0;
int16_t     yTopFutureTech = 0;
int16_t     yTopTechNote = -1;
int32_t (**lpfnBrowserDlgProc)() = 0;
int32_t (**lpfnFakeCEProc)() = 0;
int32_t (**lpfnFakeComboProc)() = 0;
int32_t (**lpfnFakeEditProc)() = 0;
int32_t (**lpfnFakeListProc)() = 0;
int32_t (**lpfnGaugeDlgProc)() = 0;
int32_t (**lpfnHostTimerProc)() = 0;
int32_t (**lpfnRealCEProc)() = 0;
int32_t (**lpfnRealComboProc)() = 0;
int32_t (**lpfnRealEditProc)() = 0;
int32_t (**lpfnRealListProc)() = 0;
int32_t (**lpfnReportDlgProc)() = 0;
int32_t (**lpfnTutorDlgProc)() = 0;
int32_t  *vrgdpVCR = 0;
int32_t   lRandSeed1 = 17;
int32_t   lRandSeed2 = 37;
int32_t   lResBudget = 0;
int32_t   lResTotal = 0;
int32_t   lSaltCur = 0;
int32_t   lSaltLast = 0;
int32_t   rglPopMac[5] = {2500, 5000, 10000, 20000, 30000};
int32_t   rglRandStack[4][2] = {0};
int32_t   vSerialNumber = 0;
jmp_buf  *penvMem[9] = {0};
uint16_t *vlprgidFleet = 0;
uint16_t *vlprgidMisc = 0;
uint16_t *vlprgidPlanet = 0;
uint16_t *vlprgidRep = 0;
uint16_t *vlpwtCargo = 0;
uint16_t *vrgPlanResExtra = 0;
uint16_t *vrgPlrLosses = 0;
uint16_t  grbitScan = 0;
uint16_t  grbitScanEShip = 0;
uint16_t  grbitScanMines = 0;
uint16_t  grbitScanShip = 0;
uint16_t  grfMissed = 0;
uint16_t  hAccel = 0;
uint16_t  hAccelTitle = 0;
uint16_t  hInst = 0;
uint16_t  mphtcbAlloc[12] = {63488, 4096, 4096, 4096, 8192, 63488, 65280, 17472, 4096, 6144, 2048, 65280};
uint16_t  rghbrCache[32] = {0};
uint16_t  rghbrMinSum[4][2] = {0};
uint16_t  rghbrMineral[5] = {0};
uint16_t  rghbrPat[3] = {0};
uint16_t  rghbrPlanetAttr[3][2] = {0};
uint16_t  rghdibInventory[7] = {0};
uint16_t  rghdibShipsT[5] = {0};
uint16_t  rghdibShips[5] = {0};
uint16_t  rghfontArial10[2] = {0};
uint16_t  rghfontArial6[1] = {0};
uint16_t  rghfontArial7[1] = {0};
uint16_t  rghfontArial8[5] = {0};
uint16_t  rghiconVCR[7] = {0};
uint16_t  rghwndBtnSplash[4] = {0};
uint16_t  rghwndBtn[13] = {0};
uint16_t  rghwndMsgBtn[4] = {0};
uint16_t  rghwndOrderDD[3] = {0};
uint16_t  rgidRaceBtn[5] = {2, 1070, 1071, 1072, 118};
uint16_t  uDateInstalled = 0;
uint16_t  uTimerId = 0;
uint16_t  uTimerType = 0;
uint16_t  vcPasswordFailures = 0;
uint16_t  vhdibTitle = 0;
uint16_t  vhpal = 0;
uint16_t  vhpalSplash = 0;
uint16_t  wVersFile = 0;
uint32_t  ctickLast = 0;
uint32_t  rgcrCache[32] = {0};
uint32_t  rgcrMinerals[6] = {16711680, 32512, 65535, 16777215, 255};
uint32_t  rgcrPlrHistory[16] = {4190448, 255,     65280,   16711680, 65535,   16711935, 16776960, 127,
                                32512,   8323072, 8355711, 6801465,  2326527, 16744227, 32639,    6316128};
uint32_t  vtickTooltip1stVis = 0;
uint32_t  vtickTooltipLast = 0;
uint8_t  *lpLog = 0;
uint8_t  *lpb2k = 0;
uint8_t  *lpbBattleCur = 0;
uint8_t  *lpbBattleLog = 0;
uint8_t  *lpbBattleT = 0;
uint8_t  *vAiMacRecycleSB = 0;
uint8_t  *vlpMemStream = 0;
uint8_t  *vlpbAiData = 0;
uint8_t  *vlpbAiPlanet = 0;
uint8_t   _ctype[0];
uint8_t   bitfMsgFiltered[49] = {0};
uint8_t   bitfMsgSent[49] = {0};
uint8_t   mpiTypeiItem[3] = {0, 4, 7};
uint8_t   rgTechBattle[6] = {0};
uint8_t   rgTechTrader[13] = {0};
uint8_t   rgcbtlplan[16] = {0};
uint8_t   rghbrCacheUse[32] = {0};
uint8_t   rgszSpeed[30] = {45, 45, 0, 189, 0, 0, 190, 0, 0, 49, 0, 0, 49, 188, 0, 49, 189, 0, 49, 190, 0, 50, 0, 0, 50, 188, 0, 50, 189};
uint8_t   vbrcVCRFocus = 0;
uint8_t   vrgAiArmadaPotency[4] = {0};
uint8_t   vrgAiCyberArmadaPotency[4] = {0};
uint8_t   vrgbEnvCur[11] = {0};
uint8_t   vrgbMachineConfig[11] = {0};
uint8_t   vrgcAiParts[45] = {1, 1, 4, 4, 6, 3, 4, 3, 4, 6, 7, 1, 4, 5, 2, 4, 1, 2, 3, 2, 4, 2, 3,
                             4, 2, 1, 7, 3, 1, 1, 4, 1, 1, 1, 4, 4, 8, 2, 5, 2, 1, 5, 1, 2, 3};
uint8_t   vrgplrTypeNew[16] = {0};
