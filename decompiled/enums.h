#ifndef STARS_DECOMPILED_ENUMS_H
#define STARS_DECOMPILED_ENUMS_H

enum HeapType { htOrd = 0, htString, htMsg, htPlanets, htLog, htFleets, htMisc, htShips, htPlrMsg, htPerm, htThings, htBattle, htCount };
typedef uint16_t HeapType;

enum AiLevel {
    lvlAiEasy = 0,
    lvlAiStandard = 1,
    lvlAiTough = 2,
    lvlAiExpert = 3,
    lvlAiRandom = 4,
};
typedef uint16_t AiLevel;

enum DetType {
    detNone = 0,
    detMinimal = 1,
    detObscure = 2,
    detSome = 3,
    detMore = 4,
    detAll = 7,
};
typedef uint16_t DetType;

enum MineralType {
    SupplyButtonsOnly = -2, // transfer dialog: redraw only the buttons
    SupplyAll = -1,         // transfer dialog: redraw every cargo row
    Ironium = 0,
    Boranium = 1,
    Germanium = 2,
    Colonists = 3,
    Resources = 3,
    Fuel = 4,
};
typedef int16_t MineralType;

enum EnvType {
    Gravity = 0,
    Temperature = 1,
    Radiation = 2,
};
typedef uint16_t EnvType;

enum TechFieldType {
    Energy = 0,
    Weapons = 1,
    Propulsion = 2,
    Construction = 3,
    Electronics = 4,
    Biotechnology = 5,
    TechFieldCount = 6,
};
typedef uint16_t TechFieldType;

enum BattleUnitFlags {
    grBuOurUnits = 0x0001,
    grBuTheirUnits = 0x0002,
    grBuIncludeSb = 0x0004,
    // hull classes, by HullCategory as the battle report columns count them
    grBuClassUnarmed = 0x0008,
    grBuClassScout = 0x0010,
    grBuClassWarship = 0x0020,
    grBuClassBomber = 0x0040,
    grBuClassUtility = 0x0080,
    grBuClassAll = 0x00F8,
};
typedef uint16_t BattleUnitFlags;

enum AttackWho {
    iplrAttackNobody = 0,
    iplrAttackEnemies = 1,
    iplrAttackNeutralsEnemies = 2,
    iplrAttackEveryone = 3,
    iplrAttackPlayer = 4,
};
typedef uint16_t AttackWho;

enum mdProdStat {
    mdProdStatComplete = 0,
    mdProdStatCompleteAuto = 1,
    mdProdStatSkippedAuto = 2,
    mdProdStatSomeAuto = 3,
    mdProdStatNoneAuto = 4,
    mdProdStatSome = 5,
    mdProdStatBlockedDiff = 6,
    mdProdStatBlockedSame = 7,
};
typedef uint16_t mdProdStat;

enum GrPopupType {
    grPopupMineral = 1,
    grPopupPlayer = 2,
    grPopupFleet = 3,
    grPopupUnknownObj = 4,
    grPopupPlanetEnv = 5,
    grPopupShipOrders = 6,
    grPopupPlanet = 7,
    grPopupPlanetIndustry = 8,
    grPopupComponent = 9,
    grPopupString = 10,
    grPopupShdef = 11,
    grPopupResources = 12,
    grPopupUnknown = 13,
    grPopupShdefSB = 14,
    grPopupShdefBuild = 15,
};
typedef uint16_t GrPopupType;

enum HtMineType {
    htMineNone = 0,
    htMineMineralConc1 = 1,
    htMineMineralConc2 = 2,
    htMineMineralConc3 = 3,
    htMineUnused4 = 4,
    htMineScale = 5,
    htMineEnvVar0 = 6,
    htMineEnvVar1 = 7,
    htMineEnvVar2 = 8,
    htMineScanSel = 9,
    htMineOwner = 10,
    htMineShipOrFleet = 11,
    htMinePlanet = 12,
    htMineStarbase = 13,
    htMineMinefieldType = 14,
};
typedef uint16_t HtMineType;

enum HtMsgType {
    htMsgNone = 0,
    htMsgCurrent = 1,
    htMsgZoom = 2,
    htMsgMode = 3,
};
typedef uint16_t HtMsgType;

enum DtFileType {
    dtXY = 0,
    dtLog = 1,
    dtHost = 2,
    dtTurn = 3,
    dtHist = 4,
    dtRace = 5,
};
typedef uint16_t DtFileType;

enum RaceGrbit {
    ibitRaceIFE = 0x00,
    ibitRaceTT = 0x01,
    ibitRaceARM = 0x02,
    ibitRaceISB = 0x03,
    ibitRaceGeneralizedResearch = 0x04,
    ibitRaceUltimateRecycling = 0x05,
    ibitRaceMineralAlchemy = 0x06,
    ibitRaceNoRamscoops = 0x07,
    ibitRaceCheapEngines = 0x08,
    ibitRaceOBRM = 0x09,
    ibitRaceNoAdvScanner = 0x0a,
    ibitRaceLowStartingPop = 0x0b,
    ibitRaceBleedingEdgeTech = 0x0c,
    ibitRaceRegeneratingShields = 0x0d,
    ibitRaceTech3 = 0x1d,
    ibitRaceAIPlayer = 0x1e,
    ibitRaceCheapFact = 0x1f,
    ibitRaceLast = 32,
};
typedef uint16_t RaceGrbit;

// RaceTraitBits is a race's grbitAttr: one bit per RaceGrbit index, the
// lesser racial traits and the other per-race switches.
enum RaceTraitBits {
    grbitRaceIFE = 0x00000001,
    grbitRaceTT = 0x00000002,
    grbitRaceARM = 0x00000004,
    grbitRaceISB = 0x00000008,
    grbitRaceGeneralizedResearch = 0x00000010,
    grbitRaceUltimateRecycling = 0x00000020,
    grbitRaceMineralAlchemy = 0x00000040,
    grbitRaceNoRamscoops = 0x00000080,
    grbitRaceCheapEngines = 0x00000100,
    grbitRaceOBRM = 0x00000200,
    grbitRaceNoAdvScanner = 0x00000400,
    grbitRaceLowStartingPop = 0x00000800,
    grbitRaceBleedingEdgeTech = 0x00001000,
    grbitRaceRegeneratingShields = 0x00002000,
    grbitRaceTech3 = 0x20000000,
    grbitRaceAIPlayer = 0x40000000,
    grbitRaceCheapFact = 0x80000000,
};
typedef uint32_t RaceTraitBits;

enum RaceStat {
    rsResGen = 0,
    rsFactProd = 1,
    rsFactBuild = 2,
    rsFactOperate = 3,
    rsMineProd = 4,
    rsMineBuild = 5,
    rsMineOperate = 6,
    rsUseLeftover = 7,
    rsTechBonus1 = 8,
    rsTechBonus2 = 9,
    rsTechBonus3 = 10,
    rsTechBonus4 = 11,
    rsTechBonus5 = 12,
    rsTechBonus6 = 13,
    rsMajorAdv = 14,
};
typedef uint16_t RaceStat;

enum RaceAttribute {
    raCheapCol = 0,
    raStealth = 1,
    raAttack = 2,
    raTerra = 3,
    raDefend = 4,
    raMines = 5,
    raMassAccel = 6,
    raStargate = 7,
    raMacintosh = 8,
    raNone = 9,
    raMax = 10,
};
typedef uint16_t RaceAttribute;

enum GrobjClass {
    grobjNone = 0x0,
    grobjPlanet = 0x1,
    grobjFleet = 0x2,
    grobjOther = 0x4,
    grobjThing = 0x8,
    mdNoRecurse = 0x0020,
    mdScanRadius = 0x0040,
    mdExact = 0x0080,
    mdRecurseMask = mdNoRecurse | mdExact,
};
typedef uint16_t GrobjClass;

enum HullSlotType {
    hstNone = 0x0000,
    hstEngine = 0x0001,
    hstScanner = 0x0002,
    hstShield = 0x0004,
    hstArmor = 0x0008,
    hstBeam = 0x0010,
    hstTorp = 0x0020,
    hstBomb = 0x0040,
    hstMining = 0x0080,
    hstMines = 0x0100,
    hstSpecialSB = 0x0200,
    hstSBHull = 0x0400,
    hstSpecialE = 0x0800,
    hstSpecialM = 0x1000,
    hstTerra = 0x2000,
    hstHull = 0x4000,
    hstPlanetary = 0x8000,
    hstWeapon = hstBeam | hstTorp,
    hstShArm = hstShield | hstArmor,
    hstSpecialEM = hstSpecialE | hstSpecialM,
    hstScanSpec = hstScanner | hstSpecialE | hstSpecialM,
    hstShWeap = hstShield | hstBeam | hstTorp,
    hstSomeSB = hstSpecialSB | hstSpecialE,
    hstSpecMine = hstSpecialEM | hstMines,
    hstShSpec = hstShield | hstSpecialE | hstSpecialM,
    hstScanSpecArm = hstScanner | hstArmor | hstSpecialE | hstSpecialM,
    hstEnabled = 0x19FF,
    hstEnabledSB = hstShield | hstArmor | hstWeapon | hstSpecialSB | hstSpecialE,
    hstSome = 0x193E,

};
typedef uint16_t HullSlotType;

enum HulDef {
    ihuldefSmallFreighter = 0,
    ihuldefMediumFreighter = 1,
    ihuldefLargeFreighter = 2,
    ihuldefSuperFreighter = 3,
    ihuldefScout = 4,
    ihuldefFrigate = 5,
    ihuldefDestroyer = 6,
    ihuldefCruiser = 7,
    ihuldefBattleCruiser = 8,
    ihuldefBattleship = 9,
    ihuldefDreadnought = 10,
    ihuldefPrivateer = 11,
    ihuldefRogue = 12,
    ihuldefGalleon = 13,
    ihuldefMiniColonyShip = 14,
    ihuldefColonyShip = 15,
    ihuldefMiniBomber = 16,
    ihuldefB17Bomber = 17,
    ihuldefStealthBomber = 18,
    ihuldefB52Bomber = 19,
    ihuldefMidgetMiner = 20,
    ihuldefMiniMiner = 21,
    ihuldefMiner = 22,
    ihuldefMaxiMiner = 23,
    ihuldefUltraMiner = 24,
    ihuldefFuelTransport = 25,
    ihuldefSuperFuelXport = 26,
    ihuldefMiniMineLayer = 27,
    ihuldefSuperMineLayer = 28,
    ihuldefNubian = 29,
    ihuldefMiniMorph = 30,
    ihuldefMetaMorph = 31,
    ihuldefOrbitalFort = 32,
    ihuldefSpaceDock = 33,
    ihuldefSpaceStation = 34,
    ihuldefUltraStation = 35,
    ihuldefDeathStart = 36,
};
typedef uint16_t HulDef;

enum StartingStarbase {
    Starbase = 0,
    AcceleratorPlatform = 1,
    PortholetoBeyond = 2,
    StarterColony = 3,
};
typedef uint16_t StartingStarbase;

enum StartingShip {
    LilliputianFreighter = 0,
    ShadowTransport = 1,
    SmaugarianPeepingTom = 2,
    ArmedProbe = 3,
    LongRangeScout = 4,
    ShadowSleuth = 5,
    Teamster = 6,
    StalwartDefender = 7,
    Swashbuckler = 8,
    SantaMaria = 9,
    Pinta = 10,
    Mayflower = 11,
    SporeCloud = 12,
    Gadfly = 13,
    CottonPicker = 14,
    PotatoBug = 15,
    LittleHen = 16,
    ChangeofHeart = 17,
    SpeedTurtle = 18,
    MTLifeboat = 19,
    MTScout = 20,
    MTProbe = 21,
};
typedef uint16_t StartingShip;

enum ThingType {
    ithMinefield = 0,
    ithMineralPacket = 1,
    ithWormhole = 2,
    ithMysteryTrader = 3,
};
typedef uint16_t ThingType;

enum MdBuild {
    mdBuildShdef = 0,
    mdBuildHuldef = 1,
    mdBuildEnemyShdef = 2,
    mdBuildComp = 3,
    mdBuildEdit = 4,
};
typedef uint16_t MdBuild;

enum MdXfer {
    mdXferNone = -1,
    mdXferCargo = 0,
    mdXferShips = 1,
};
typedef int16_t MdXfer;

enum ProdItemType {
    iobjMine = 0,
    iobjFactory = 1,
    iobjDefense = 2,
    iobjAlchemy = 3,
    iobjMinTerraform = 4,
    iobjMaxTerraform = 5,
    iobjPacket = 6,
    mdIdleFactory = 7,
    mdIdleMine = 8,
    mdIdleDefense = 9,
    /* 10 unused ? */
    mdIdleAlchemy = 11,
    mdIdleTerraform = 12,
    iobjGenesis = 13,
    iobjPacketIron = 14,
    iobjPacketBor = 15,
    iobjPacketGerm = 16,
    iobjPacketMixed = 17,

    iobjPlanetaryScannerFirst = 18,
    iobjPlanetaryScannerViewer50 = 18,
    iobjPlanetaryScannerViewer90 = 19,
    iobjPlanetaryScannerScoper150 = 20,
    iobjPlanetaryScannerScoper220 = 21,
    iobjPlanetaryScannerScoper280 = 22,
    iobjPlanetaryScannerSnooper320X = 23,
    iobjPlanetaryScannerSnooper400X = 24,
    iobjPlanetaryScannerSnooper500X = 25,
    iobjPlanetaryScannerSnooper620X = 26,
    iobjPlanetaryScannerLast = 26,
    iobjPlanetaryScanner = 27,
    iobjUnknown = 31,
};
typedef uint16_t ProdItemType;

enum StringId {
    idsUniverseDefinitionFileSeemsMissingCorrupt = 0x0000,
    idsPlayerLogFileAppearsCorruptUnableLoad = 0x0001,
    idsHistoryFileAppearsCorruptHistoricalDataWill = 0x0002,
    idsGameFileAppearsCorruptUnableLoadFile = 0x0003,
    idsCantOpenFile = 0x0004,
    idsUniverseCreationFileAppearsInvalid = 0x0005,
    idsIllegalGameTitle = 0x0006,
    idsLine2HasBadUniverseDefinitionParameter = 0x0007,
    idsLine3HasBadUniverseDefinitionParameter = 0x0008,
    idsLine4HasImproperNumberPlayerFiles = 0x0009,
    idsLineDUnableLoadRaceFileS = 0x000a,
    idsLineDHasImproperVictoryConditionDefinition = 0x000b,
    idsUniverseDefinitionFileAppearsTooShort = 0x000c,
    idsFileDoesBelongVersionStars = 0x000d,
    idsGameCurrentlyLoaded = 0x000e,
    idsCantChangeZoomFactorUntilGameOpen = 0x000f,
    idsLogFileHasReachedMaximumAllowableSize = 0x0010,
    idsUnableCreateLogFile = 0x0011,
    idsUnableCreateHistoryFile = 0x0012,
    idsUnableCreateHostFile = 0x0013,
    idsUnableCreateUniverseDefinitionFile = 0x0014,
    idsHostFileMarkedUseAnotherInstanceStars = 0x0015,
    idsErrorWritingFile = 0x0016,
    idsUnableLoadBitmaps = 0x0017,
    idsUnableInitializeStars = 0x0018,
    idsUnableOpenHostFile = 0x0019,
    idsMemory = 0x001a,
    idsUnableOpenNewTurnFile = 0x001b,
    idsFileDate = 0x001c,
    idsFileGame = 0x001d,
    idsLogFileRecentGameTryingLoadIgnoring = 0x001e,
    idsAmountCargoMaySpecifyHereMustBetween = 0x001f,
    idsThereIsntEnoughFreeMemoryModifyProduction = 0x0020,
    idsMessageTypeHasFilteredWillShownDefault = 0x0021,
    idsMessagesHaveSentYearFilteredIfWant = 0x0022,
    idsWarningColonizeMissionCannotCarriedBecauseNone = 0x0023,
    idsNoteShipsFleetWillDismantledProvideSupplies = 0x0024,
    idsNoteShipsFleetWillDismantledMineralsCan = 0x0025,
    idsNoteShipsFleetWillDismantledMineralsWill = 0x0026,
    idsFuelUsageVsWarpSpeed = 0x0027,
    idsShieldCoverageVsDefenseQuan = 0x0028,
    idsEngineCanMountedMiniColonizerHullRequires = 0x0029,
    idsEngineCreatesPowerfulWavesRadiationWillKill = 0x002a,
    idsEngineRequiresLesserRacialTraitImprovedFuel = 0x002b,
    idsEngineRequiresLesserRacialTraitImprovedFuel2 = 0x002c,
    idsEngineRequiresLesserRacialTraitRamScoop = 0x002d,
    idsStargateRequiresPrimaryRacialTraitInterstellarTr = 0x002e,
    idsMassDriverRequiresPrimaryRacialTraitPacket = 0x002f,
    idsHullWillHaveBuiltScannerIfJack = 0x0030,
    idsEnemyFleetsOrbitingPlanetCanDetectedD = 0x0031,
    idsEnemyFleetsCannotDetectedScannerUnlessSame = 0x0032,
    idsScannerCapableDeterminingPlanetsEnvironmentCompo = 0x0033,
    idsScannerCanDeterminePlanetsBasicStatsDistance = 0x0034,
    idsScannerCanDeterminePlanetsBasicStatsDistance2 = 0x0035,
    idsScannerCapablePenetratingDefensesEnemyFleetsAllo = 0x0036,
    idsScannerCanDeterminePlanetsStatsDistance120 = 0x0037,
    idsScannerRequiresPrimaryRacialTraitSuperStealth = 0x0038,
    idsCloakRequiresPrimaryRacialTraitSuperStealth = 0x0039,
    idsArmorShieldRequiresPrimaryRacialTraitSuper = 0x003a,
    idsShieldRequiresPrimaryRacialTraitInnerStrength = 0x003b,
    idsShieldDecreasesRangeWhichEnemyShipsCan = 0x003c,
    idsArmorDecreasesRangeWhichEnemyShipsCan = 0x003d,
    idsArmorAlsoActsPartShieldWhichWill = 0x003e,
    idsShieldAlsoContainsArmorComponentWhichWill = 0x003f,
    idsShieldAlsoProvides65dpArmor5Jamming = 0x0040,
    idsBombWillAvailableIfPrimaryRaceTrait = 0x0041,
    idsStargatesAvailableIfPrimaryRaceTraitHyper = 0x0042,
    idsCargo = 0x0043,
    idsTechnologyStatus = 0x0044,
    idsExpectedResearchBenefits = 0x0045,
    idsCurrentlyResearching = 0x0046,
    idsResourceAllocation = 0x0047,
    idsFuelCapacity = 0x0048,
    idsCargoCapacity = 0x0049,
    idsArmorStrength = 0x004a,
    idsInitiative = 0x004b,
    idsResourcesNeededComplete = 0x004c,
    idsEstimatedTimeCompletion = 0x004d,
    idsAnnualResourcesPlanets = 0x004e,
    idsTotalResourcesSpentResearchLastYear = 0x004f,
    idsResourcesBudgetedResearch = 0x0050,
    idsYearsProjectedResearchBudget = 0x0051,
    idsFieldResearch = 0x0052,
    idsSameField = 0x0053,
    idsEnergy = 0x0054,
    idsWeapons = 0x0055,
    idsPropulsion = 0x0056,
    idsConstruction = 0x0057,
    idsElectronics = 0x0058,
    idsBiotechnology = 0x0059,
    idsLowestField = 0x005a,
    idsEner = 0x005b,
    idsWeap = 0x005c,
    idsProp = 0x005d,
    idsConst = 0x005e,
    idsElect = 0x005f,
    idsBio = 0x0060,
    idsSureWantDeleteCurrentWaypoint = 0x0061,
    idsGameAlreadyHostedAnotherInstanceStarsWould = 0x0062,
    idsTaskHere = 0x0063,
    idsTransport = 0x0064,
    idsColonize = 0x0065,
    idsRemoteMining = 0x0066,
    idsMergeFleet = 0x0067,
    idsScrapFleet = 0x0068,
    idsLayMineField = 0x0069,
    idsPatrol = 0x006a,
    idsRoute = 0x006b,
    idsTransferFleet = 0x006c,
    idsAction = 0x006d,
    idsLoadAvailable = 0x006e,
    idsUnload = 0x006f,
    idsLoadExactly = 0x0070,
    idsUnloadExactly = 0x0071,
    idsFill = 0x0072,
    idsWait = 0x0073,
    idsLoadDunnage = 0x0074,
    idsSetAmount = 0x0075,
    idsSetWaypoint = 0x0076,
    idsLoadOptimal = 0x0077,
    idsNobody = 0x0078,
    idsEnemies = 0x0079,
    idsNeutralsEnemies = 0x007a,
    idsEveryone = 0x007b,
    idsImprovePlanet = 0x007c,
    idsUndoTerraforming = 0x007d,
    idsMines = 0x007e,
    idsFactories = 0x007f,
    idsDefenses = 0x0080,
    idsAlchemy = 0x0081,
    idsMinTerraform = 0x0082,
    idsMaxTerraform = 0x0083,
    idsMineralPackets = 0x0084,
    idsFactory = 0x0085,
    idsMine = 0x0086,
    idsDefenses2 = 0x0087,
    ids0136Blank = 0x0088,
    idsMineralAlchemy = 0x0089,
    idsTerraformEnvironment = 0x008a,
    idsGenesisDevice = 0x008b,
    idsIroniumMineralPacket = 0x008c,
    idsBoraniumMineralPacket = 0x008d,
    idsGermaniumMineralPacket = 0x008e,
    idsMixedMineralPacket = 0x008f,
    idsWindows = 0x0090,
    idsStarsIni = 0x0091,
    idsMisc = 0x0092,
    idsZiporders = 0x0093,
    idsMain = 0x0094,
    idsShiptiles = 0x0095,
    idsPlanettiles = 0x0096,
    idsScanzoom = 0x0097,
    idsSelection = 0x0098,
    idsFiles = 0x0099,
    idsWait2 = 0x009a,
    idsFile1 = 0x009b,
    idsTurn = 0x009c,
    idsScanmodev25 = 0x009d,
    idsScanfilterv25 = 0x009e,
    idsScanefilterv25 = 0x009f,
    idsScanmines = 0x00a0,
    idsScanradar = 0x00a1,
    idsMineralscale = 0x00a2,
    idsLayout = 0x00a3,
    idsGlobalsettings = 0x00a4,
    idsStyle1width = 0x00a5,
    idsStyle1height = 0x00a6,
    idsStyle1height2 = 0x00a7,
    idsStyle2width = 0x00a8,
    idsStyle2height = 0x00a9,
    idsStyle2height2 = 0x00aa,
    idsToolbar = 0x00ab,
    idsGameid = 0x00ac,
    idsResolution = 0x00ad,
    idsDefaultpassword = 0x00ae,
    idsProgress = 0x00af,
    idsBackups = 0x00b0,
    idsReportplanwin = 0x00b1,
    idsReportfleetwin = 0x00b2,
    idsReportefleetwin = 0x00b3,
    idsReportbtlwin = 0x00b4,
    idsReportplanfld = 0x00b5,
    idsReportplansort = 0x00b6,
    idsReportfleetfld = 0x00b7,
    idsReportfleetsort = 0x00b8,
    idsReportefleetfld = 0x00b9,
    idsReportefltsort = 0x00ba,
    idsReportbtlfld = 0x00bb,
    idsReportbtlsort = 0x00bc,
    idsReportdefgraph = 0x00bd,
    idsSoundfx = 0x00be,
    idsHistoryinfo = 0x00bf,
    idsVcrspeed = 0x00c0,
    idsMusic = 0x00c1,
    idsTracks = 0x00c2,
    idsTrack = 0x00c3,
    idsFonts = 0x00c4,
    idsArial = 0x00c5,
    idsArialbold = 0x00c6,
    idsArialitalic = 0x00c7,
    idsArialbolditalic = 0x00c8,
    idsNewreports = 0x00c9,
    idsNohostnames = 0x00ca,
    idsMessage = 0x00cb,
    idsLogging = 0x00cc,
    idsHave = 0x00cd,
    idsOn = 0x00ce,
    idsMayBuild = 0x00cf,
    idsHoweverColonistsCurrentlyCapableOperating = 0x00d0,
    idsThem = 0x00d1,
    idsApplyDefineProductionTemplate = 0x00d2,
    idsRightClickBlueDiamondApplyProductionTemplate = 0x00d3,
    idsPodIncreasesFuelCapacityShipDmg = 0x00d4,
    idsPodIncreasesCargoCapacityShipDkt = 0x00d5,
    idsPodIncreasesCargoCapacityShip250ktProvides = 0x00d6,
    idsPodAllowsShipColonizePlanetWillDismantle = 0x00d7,
    idsModuleContainsEmptyOrbitalHullWhichCan = 0x00d8,
    idsDeviceAllowsShipJumpPlanetaryStargatesRange = 0x00d9,
    idsDeflectorDecreasesDamageDoneBeamWeaponsShip = 0x00da,
    idsCloaksUnarmedHullsReducingRangeWhichScanners = 0x00db,
    idsCloaksAnyShipReducingRangeWhichScanners = 0x00dc,
    idsCloaksAnyShip30Acts10Jammer = 0x00dd,
    idsRememberLoadColonistsBeforeEmbarkingMission = 0x00de,
    idsModuleContainsRobotsCapableMining = 0x00df,
    idsKtEachMineralDependingConcentrationUninhabitedPl = 0x00e0,
    idsModuleAlsoActs30Cloak30Jammer = 0x00e1,
    idsWarningFleetContainsShipsRemoteMiningModules = 0x00e2,
    idsWarningFleetContainsShipsAdjusterModules = 0x00e3,
    idsWarningMustPlanetPerformRemoteTerraforming = 0x00e4,
    idsNoteCanMineUninhabitedPlanets = 0x00e5,
    idsWarningFleetHasMineLayingPods = 0x00e6,
    idsWarningFleetHasBeamsSweepMines = 0x00e7,
    idsFleetCanDestroyLdMinesPerYear = 0x00e8,
    idsFleetCanLayLdMinesPerYear = 0x00e9,
    idsPasswordHaveEnteredIncorrectPleaseTry = 0x00ea,
    idsPasswordsTypedTwoFieldsSamePleaseReenter = 0x00eb,
    idsHaveUsingDemoVersionStars20Days = 0x00ec,
    idsGenerates = 0x00ed,
    idsResourcesEachYear = 0x00ee,
    idsResourcesHaveAllocatedResearch = 0x00ef,
    idsLeaves = 0x00f0,
    idsResourcesUsePlanet = 0x00f1,
    idsResourcesPlanetEqualSquareRootPopulation = 0x00f2,
    idsPlanetaryDataAvailableEstimateMineralMiningRates = 0x00f3,
    idsShipDesignDoesHaveAnyEnginesMust = 0x00f4,
    idsMaximumColonistGrowthRatePerYear = 0x00f5,
    idsOneResourceGeneratedEachYearEvery = 0x00f6,
    idsColonists = 0x00f7,
    idsEvery10FactoriesProduce = 0x00f8,
    idsResourcesEachYear2 = 0x00f9,
    idsFactoriesRequire = 0x00fa,
    idsResourcesBuild = 0x00fb,
    idsEvery10000ColonistsMayOperate = 0x00fc,
    idsFactories2 = 0x00fd,
    idsEvery10MinesProduce = 0x00fe,
    idsEachMineralEveryYear = 0x00ff,
    idsMinesRequire = 0x0100,
    idsResourcesBuild2 = 0x0101,
    idsEvery10000ColonistsMayOperate2 = 0x0102,
    idsMines2 = 0x0103,
    idsAnnualResourcesPlanetValueSqrtPopulationEnergy = 0x0104,
    idsMsg0261 = 0x0105,
    idsSurfaceMinerals = 0x0106,
    idsMineralConcentrations = 0x0107,
    idsMines3 = 0x0108,
    idsFactories3 = 0x0109,
    idsDefenses3 = 0x010a,
    idsStarsUnableSaveRaceDataFilePlease = 0x010b,
    idsSettlersDelightEngineMayMountedDesignsBased = 0x010c,
    idsOrbitalConstructionModuleMayMountedDesignsBased = 0x010d,
    idsCustomRaceWizardStepD6 = 0x010e,
    idsViewRacePageD6 = 0x010f,
    idsAdvancedNewGameWizardStepD3 = 0x0110,
    idsViewGameParametersPageD3 = 0x0111,
    idsPrimaryRacialTrait = 0x0112,
    idsDescriptionTrait = 0x0113,
    idsMustExpandSurviveGivenSmallCheapColony = 0x0114,
    idsRaceWillGrowTwiceGrowthRateSelect = 0x0115,
    idsCompletelyFlexibleMetaMorphHullWillAvailable = 0x0116,
    idsCanSneakThroughEnemyTerritoryExecuteStunning = 0x0117,
    idsCargoDoesDecreaseCloakingAbilitiesStealthBomber = 0x0118,
    idsTwoScannersWhichAllowStealMineralsEnemy = 0x0119,
    idsRuleBattleFieldColonistsAttackBetterShips = 0x011a,
    idsStartGameKnowledgeTech6WeaponsTech = 0x011b,
    idsUnfortunatelyRaceDoesntUnderstandNecessityBuildi = 0x011c,
    idsExpertFiddlingPlanetaryEnvironmentsStartGameTech = 0x011d,
    idsBombsUnterraformEnemyWorldsTerraformingCostsNoth = 0x011e,
    idsVariable1PerYear = 0x011f,
    idsStrongHardDefeatColonistsRepelAttacksBetter = 0x0120,
    idsCanLaySpeedTrapMineFieldsHave = 0x0121,
    idsSmartBombsPlanetaryDefensesCost40Though = 0x0122,
    idsExpertLayingMineFieldsHaveVastArray = 0x0123,
    idsMineFieldsActScannersHaveAbilityRemote = 0x0124,
    idsMineFieldsStartGame2MineLaying = 0x0125,
    idsRaceExcelsAcceleratingMineralPacketsDistantPlane = 0x0126,
    idsWillEventuallyAbleFlingPacketsMindNumbing = 0x0127,
    idsWillStartGameOwningSecondPlanetDistance = 0x0128,
    idsRaceExcelsBuildingStargatesStartTech5 = 0x0129,
    idsHaveStargatesEventuallyMayBuildStargatesWhich = 0x012a,
    idsPlanetStargateWhichRangeOneStargatesExceeding = 0x012b,
    idsRaceDevelopedAlternatePlanePeopleCannotSurvive = 0x012c,
    idsHaveIntrinsicAbilityMineScanEnemyFleets = 0x012d,
    idsDeterminedTypeStarbaseHaveWillEventuallyAble = 0x012e,
    idsRaceDoesSpecializeSingleAreaStartGame = 0x012f,
    idsScoutDestroyerFrigateHullsHaveBuiltPenetrating = 0x0130,
    ids0305Blank = 0x0131,
    idsImprovedFuelEfficiency = 0x0132,
    idsTotalTerraforming = 0x0133,
    idsAdvancedRemoteMining = 0x0134,
    idsImprovedStarbases = 0x0135,
    idsGeneralizedResearch = 0x0136,
    idsUltimateRecycling = 0x0137,
    idsMineralAlchemy2 = 0x0138,
    idsRamScoopEngines = 0x0139,
    idsCheapEngines = 0x013a,
    idsBasicRemoteMining = 0x013b,
    idsAdvancedScanners = 0x013c,
    idsLowStartingPopulation = 0x013d,
    idsBleedingEdgeTechnology = 0x013e,
    idsRegeneratingShields = 0x013f,
    idsGivesFuelMizerGalaxyScoopEnginesIncreases = 0x0140,
    idsAllowsTerraformInvestingSolelyBiotechnologyMayTe = 0x0141,
    idsGivesThreeAdditionalMiningHullsTwoNew = 0x0142,
    idsGivesTwoNewStarbaseDesignsStardockAllows = 0x0143,
    idsRaceTakesHolisticApproachResearchHalfResources = 0x0144,
    idsWhenScrapFleetStarbaseRecover90Minerals = 0x0145,
    idsAllowsTurnResourcesMineralsFourTimesEfficiently = 0x0146,
    idsEnginesWhichTravelWarp5GreaterBurning = 0x0147,
    idsCanThrowEnginesTogetherHalfCostHowever = 0x0148,
    idsMiningShipAvailableWillMiniMinerTrait = 0x0149,
    idsPlanetPenetratingScannersWillAvailableHoweverCon = 0x014a,
    idsWillStart30FewerColonists = 0x014b,
    idsNewTechsInitiallyCostTwiceMuchBuild = 0x014c,
    idsShields40StrongerListedRatingShieldsRegenerate = 0x014d,
    idsRobotMinerRequiresLesserRacialTraitAdvanced = 0x014e,
    idsMiningHullRequiresLesserRacialTraitAdvanced = 0x014f,
    idsRobotMinerWillAvailableIfLesserRacial = 0x0150,
    idsMineRequiresPrimaryRacialTraitSpaceDemolition = 0x0151,
    idsMineRequiresPrimaryRacialTraitSpaceDemolition2 = 0x0152,
    idsHullRequiresPrimaryRacialTraitHyperExpansion = 0x0153,
    idsHullUnavailableIfHaveRaceDisadvantageBasic = 0x0154,
    idsPartRequiresPrimaryRacialTraitInnerStrength = 0x0155,
    idsPartRequiresPrimaryRacialTraitWarMonger = 0x0156,
    idsHullRequiresPrimaryRacialTraitWarMonger = 0x0157,
    idsHullRequiresPrimaryRacialTraitSuperStealth = 0x0158,
    idsHullRequiresPrimaryRacialTraitInnerStrength = 0x0159,
    idsHullRequiresPrimaryRacialTraitAlternateReality = 0x015a,
    idsPlanetaryDefenseUnavailablePrimaryRacialTraitWar = 0x015b,
    idsTotalTerraformingRequiresLesserRacialTraitTotal = 0x015c,
    idsPartAvailableAlternateRealityRaces = 0x015d,
    idsPartRequiresPrimaryRacialTraitAlternateReality = 0x015e,
    idsPartRequiresPrimaryRacialTraitClaimAdjuster = 0x015f,
    idsModifiedMiningRobotTerraformsInhabitedPlanets1 = 0x0160,
    idsBombDoesKillColonistsDestroyInstallationsBomb = 0x0161,
    idsAllowsModifyAnyPlanetsThreeEnvironmentVariables = 0x0162,
    idsAllowsModifyPlanetsSDOriginalValue = 0x0163,
    idsScannerWillUnavailableIfHaveLesserRacial = 0x0164,
    idsHullRequiresPrimaryRaceTraitSpaceDemolition = 0x0165,
    idsIfTerraform = 0x0166,
    idsPlanetsValueWouldImprove = 0x0167,
    idsValueDOutsideHabitableRangeRace = 0x0168,
    idsValueDAwayIdealValueRace = 0x0169,
    idsNormalView = 0x016a,
    idsSurfaceMineralView = 0x016b,
    idsMineralConcentrationView = 0x016c,
    idsPlanetValueView = 0x016d,
    idsPopulationView = 0x016e,
    idsPlayerInfoView = 0x016f,
    idsAddWayPointsMode = 0x0170,
    idsScannerCoverageOverlay = 0x0171,
    idsMineFieldsOverlay = 0x0172,
    idsFleetPathsOverlay = 0x0173,
    idsIdleFleetsFilter = 0x0174,
    idsPlanetNamesOverlay = 0x0175,
    idsShipDesignFilter = 0x0176,
    idsDesignFilterMenu = 0x0177,
    idsEnemyShipClassFilter = 0x0178,
    idsEnemyClassFilterMenu = 0x0179,
    idsZoomMenu = 0x017a,
    idsShipCountsOverlay = 0x017b,
    idsScannerEffective = 0x017c,
    idsColony = 0x017d,
    idsFreighter = 0x017e,
    idsScout = 0x017f,
    idsWarship = 0x0180,
    idsUtility = 0x0181,
    idsBomber = 0x0182,
    idsMiner = 0x0183,
    idsFuelTransport = 0x0184,
    idsMustHaveLeastOnePlayerGame = 0x0185,
    idsTransportCloakingModuleMayPlacedHullCould = 0x0186,
    idsCanExpect1DPlanetsWillHabitable = 0x0187,
    idsPlanetsWillHabitableRace = 0x0188,
    idsVirtuallyPlanetsWillHabitableRace = 0x0189,
    idsIncreasesSpeedBattle1DSquareMovement = 0x018a,
    idsYetImplemented = 0x018b,
    idsEngineWillUnavailableIfHaveLesserRacial = 0x018c,
    idsRightClickBlueDiamondBringPopupMenu = 0x018d,
    idsTurnHasSubmittedChangesMadeAfterTurn = 0x018e,
    idsNewTurnCurrentlyGeneratedHostNewTurn = 0x018f,
    idsNoneDisengage = 0x0190,
    idsAny = 0x0191,
    idsStarbase = 0x0192,
    idsArmedShips = 0x0193,
    idsBombersFreighters = 0x0194,
    idsUnarmedShips = 0x0195,
    idsFuelTransports = 0x0196,
    idsFreighters = 0x0197,
    idsDisengage = 0x0198,
    idsDisengageIfChallenged = 0x0199,
    idsMinimizeDamageSelf = 0x019a,
    idsMaximizeNetDamage = 0x019b,
    idsMaximizeDamageRatio = 0x019c,
    idsMaximizeDamage = 0x019d,
    idsDisengage2 = 0x019e,
    idsCargo2 = 0x019f,
    idsGoto = 0x01a0,
    idsMerge = 0x01a1,
    idsGoto2 = 0x01a2,
    idsPrev = 0x01a3,
    idsNext = 0x01a4,
    idsRename = 0x01a5,
    idsJettison = 0x01a6,
    idsSplit = 0x01a7,
    idsSplit2 = 0x01a8,
    idsMerge2 = 0x01a9,
    idsChange = 0x01aa,
    idsClear = 0x01ab,
    idsFuel = 0x01ac,
    idsCargoHold = 0x01ad,
    idsIronium = 0x01ae,
    idsBoranium = 0x01af,
    idsGermanium = 0x01b0,
    idsColonists2 = 0x01b1,
    idsPacketShell = 0x01b2,
    idsPlanets = 0x01b3,
    idsStarbases = 0x01b4,
    idsUnarmedShips2 = 0x01b5,
    idsEscortShips = 0x01b6,
    idsCapitalShips = 0x01b7,
    idsTechLevels = 0x01b8,
    idsResources = 0x01b9,
    idsScore = 0x01ba,
    idsRank = 0x01bb,
    idsCurrent = 0x01bc,
    idsFieldStudy = 0x01bd,
    idsN999999 = 0x01be,
    idsBombWillKillAnyPlanetsPopulation = 0x01bf,
    idsBombWillKillApproximatelyDDPlanets = 0x01c0,
    idsIfPlanetHasDefensesBombGuaranteedKill = 0x01c1,
    idsBombWillDamagePlanetsMinesFactories = 0x01c2,
    idsBombWillDestroyApproximatelyDPlanetsMines = 0x01c3,
    idsDifficultyLevel = 0x01c4,
    idsUniverseSize = 0x01c5,
    idsPlayerRace = 0x01c6,
    idsDensity = 0x01c7,
    idsPlayerPositions = 0x01c8,
    idsAdvancedGame = 0x01c9,
    idsButtonAllowsConfigureMultiPlayerGamesCustom = 0x01ca,
    idsShootingFishBarrel = 0x01cb,
    idsWalkPark = 0x01cc,
    idsSleepwalkingParadise = 0x01cd,
    idsBigEasy = 0x01ce,
    idsEternalBliss = 0x01cf,
    idsDuckHunt = 0x01d0,
    idsBarefootJaywalk = 0x01d1,
    idsRumbleJungle = 0x01d2,
    idsInfectedRootCanal = 0x01d3,
    idsJungleSafari = 0x01d4,
    idsMicroHardball = 0x01d5,
    idsRollerBall = 0x01d6,
    idsWallStreet = 0x01d7,
    idsBigLeague = 0x01d8,
    idsLongRoadMorning = 0x01d9,
    idsToughNuts = 0x01da,
    idsBladeRunner = 0x01db,
    idsDDay = 0x01dc,
    idsWorldWarIii = 0x01dd,
    idsEternityHell = 0x01de,
    idsNewGame = 0x01df,
    idsOpenGame = 0x01e0,
    idsContinueGame = 0x01e1,
    idsEXitStars = 0x01e2,
    idsAppearsHavePlanetaryDefenses = 0x01e3,
    idsHasPlanetaryDefensesApproximatelyDCoverage = 0x01e4,
    idsSureWantDeleteEverythingPlanetsProductionQueue = 0x01e5,
    idsTutorial = 0x01e6,
    idsTutorialGame = 0x01e7,
    idsTutorialHasRunBeforeWouldLikeDestroy = 0x01e8,
    idsCurrentlyRunningStarsTutorialDoWantExit = 0x01e9,
    idsTutorialTurnWillGeneratedHaveYetCompleted = 0x01ea,
    idsTutorialProductionQueueDoesContainRequestedItem = 0x01eb,
    idsTutorialHaveGivenFleetWrongDestinationPress = 0x01ec,
    idsTutorialHaveGivenFleetTaskDestinationWaypoint = 0x01ed,
    idsTutorialHaveGivenFleetWrongTaskDestination = 0x01ee,
    idsTutorialHaveLoadedWrongCargoFleetPlease = 0x01ef,
    idsTutorialProductionQueueDoesContainRightCount = 0x01f0,
    idsTutorialShouldDeleteShipDesignPointTutorial = 0x01f1,
    idsTutorialShouldCustomizeShipDesignPointTutorial = 0x01f2,
    idsTutorialHaveAlreadyCopiedAppropriateShipDesign = 0x01f3,
    idsTutorialHaveTriedCopyWrongShipDesign = 0x01f4,
    idsTutorialHaventPlacedCorrectNumberComponentsSlot = 0x01f5,
    idsTutorialHavePlacedWrongComponentSlotDrag = 0x01f6,
    idsTutorialMustFinishTutorialTasksBeforeExiting = 0x01f7,
    idsTutorialHaventYetFinishedCreatingNewDesign = 0x01f8,
    idsTutorialHaventPickedCorrectImageDesignPress = 0x01f9,
    idsTutorialVerifyHaveRadiatingHydroRamScoop = 0x01fa,
    idsTutorialDontHaveRightPartsDesignVerify = 0x01fb,
    idsTutorialDontHaveRightPartsDesignVerify2 = 0x01fc,
    idsTutorialDontHaveRightPartsDesignVerify3 = 0x01fd,
    idsTutorialDontHaveCorrectHullSelectedHull = 0x01fe,
    idsTutorialDontHaveCorrectShipSelectedShip = 0x01ff,
    idsTutorialNameDesignEditboxMustGaterChange = 0x0200,
    idsTutorialNameDesignEditboxMustMineLayer = 0x0201,
    idsTutorialHaventSelectedRightFleetsMergeReread = 0x0202,
    idsTutorialHaventAddedRightPartDesignVerify = 0x0203,
    idsTutorialHaventAskedMergeAnyFleetsYear = 0x0204,
    idsMineLayer = 0x0205,
    idsStinger = 0x0206,
    idsGater = 0x0207,
    idsTutorialHaventGivenZipOrderRightName = 0x0208,
    idsDropcol = 0x0209,
    idsTutorialFinishedCanContinuePlayGameStart = 0x020a,
    idsRandom = 0x020b,
    idsUnknownPlayer = 0x020c,
    idsExpansionPlayer = 0x020d,
    idsSorryCantFindPlanetFleetName = 0x020e,
    idsHumanControlled = 0x020f,
    idsAiControlled = 0x0210,
    idsHumanCurrentlyInactive = 0x0211,
    idsPopulation = 0x0212,
    idsWillGrowLd00Ld00Year = 0x0213,
    idsWillGrowYear = 0x0214,
    idsUnableCreateNewTurnFile = 0x0215,
    idsUnableUpdateTurnFile = 0x0216,
    idsNoteDYearsDataRead = 0x0217,
    idsCompletion = 0x0218,
    idsWaypointTaskS = 0x0219,
    idsWaypointS = 0x021a,
    idsWarpSpeedD = 0x021b,
    idsWarpSpeedStopped = 0x021c,
    idsFleetMassLdkt = 0x021d,
    idsNone = 0x021e,
    idsFuel2 = 0x021f,
    idsShipCountLd = 0x0220,
    idsCLd00 = 0x0221,
    idsPop = 0x0222,
    idsPopulation2 = 0x0223,
    idsVal = 0x0224,
    idsValue = 0x0225,
    idsUninhabited = 0x0226,
    idsOld = 0x0227,
    idsReportDYear = 0x0228,
    idsN999mr = 0x0229,
    idsPopulation1000000 = 0x022a,
    idsReportCurrent = 0x022b,
    idsWarningIgnoringUnexpectedDataAfterEof = 0x022c,
    idsVersionD02dC = 0x022d,
    idsYearDCMessagesDD = 0x022e,
    idsYearDCMessagesNone = 0x022f,
    idsDefault = 0x0230,
    idsCustomizeZipOrders = 0x0231,
    idsCustomizeProductionTemplates = 0x0232,
    idsResourceInfo = 0x0233,
    idsClear2 = 0x0234,
    idsRemoveTransportOrders = 0x0235,
    idsWaitload = 0x0236,
    idsWaitFullLoadMinerals = 0x0237,
    idsQuikload = 0x0238,
    idsLoadMineralsAvailable = 0x0239,
    idsQuikdrop = 0x023a,
    idsUnloadEverythingFleetCarrying = 0x023b,
    idsButtonClickOrderChoice = 0x023c,
    idsZipordClickDiamondRightMouse = 0x023d,
    idsTransportOrdersOne3CommonSetsSelect = 0x023e,
    idsZipordProvidesAbilityQuicklySetFleets = 0x023f,
    idsColonists3 = 0x0240,
    idsSupport = 0x0241,
    idsWould = 0x0242,
    idsIfColonize = 0x0243,
    idsPopulation3 = 0x0244,
    idsEnemyPopulation = 0x0245,
    idsApproximately = 0x0246,
    idsUnknown = 0x0247,
    idsUninhabited2 = 0x0248,
    idsWillKillOffApproximately = 0x0249,
    idsColonistsEachTurn = 0x024a,
    idsColonistsSettleEveryTurn = 0x024b,
    idsWillSupportPopulation = 0x024c,
    idsWithinRange = 0x024d,
    idsOf = 0x024e,
    idsIs = 0x024f,
    idsTo = 0x0250,
    idsOn2 = 0x0251,
    idsModify = 0x0252,
    idsCurrently = 0x0253,
    idsUnknown2 = 0x0254,
    idsColonistsImmune = 0x0255,
    idsEffects = 0x0256,
    idsColonistsPreferPlanetsWhere = 0x0257,
    idsBetween = 0x0258,
    idsAnd = 0x0259,
    idsCurrentlyPossessTechnology = 0x025a,
    idsShipName = 0x025b,
    idsPlanet = 0x025c,
    idsN9999 = 0x025d,
    idsMineralConcentration0000000kt = 0x025e,
    idsY = 0x025f,
    idsX = 0x0260,
    idsId = 0x0261,
    idsPlayerD = 0x0262,
    idsNone2 = 0x0263,
    idsMineralConcentration = 0x0264,
    idsSurface = 0x0265,
    idsMiningRate = 0x0266,
    idsZipord = 0x0267,
    idsTutorialHaveGivenIncorrectTransferOrderPlease = 0x0268,
    idsTutorialHaveGivenIncorrectQuantityTransferOrder = 0x0269,
    idsSureWishGenerateOptionDoesGuaranteePlayers = 0x026a,
    idsCurrentlyHaveDSSDProduction = 0x026b,
    idsCurrentlyHaveDSSProductionQueues = 0x026c,
    idsCurrentlyHaveDSSIfDelete = 0x026d,
    idsCurrentlyHaveDSSDProduction2 = 0x026e,
    idsCurrentlyHaveDSSProductionQueues2 = 0x026f,
    idsCurrentlyHaveDSSIfDelete2 = 0x0270,
    idsCurrentlyHaveDSSProductionQueues3 = 0x0271,
    idsTaskS = 0x0272,
    idsWpS = 0x0273,
    idsWarpD = 0x0274,
    idsWarpStopped = 0x0275,
    idsMassLdkt = 0x0276,
    idsDesignProgramming = 0x0277,
    ids0632Blank = 0x0278,
    idsJeffJohnson = 0x0279,
    idsJeffMcbride = 0x027a,
    ids0635Blank = 0x027b,
    ids0636Blank = 0x027c,
    idsAdditionalAi = 0x027d,
    ids0638Blank = 0x027e,
    idsJeffreyKrauss = 0x027f,
    ids0640Blank = 0x0280,
    ids0641Blank = 0x0281,
    idsArtwork = 0x0282,
    ids0643Blank = 0x0283,
    idsMichaelCMiller = 0x0284,
    idsEmblazonMultimediaInc = 0x0285,
    idsEricChang = 0x0286,
    idsMichaelReichmann = 0x0287,
    ids0648Blank = 0x0288,
    ids0649Blank = 0x0289,
    idsHelpFile = 0x028a,
    ids0651Blank = 0x028b,
    idsKurtKremer = 0x028c,
    idsBrettKremer = 0x028d,
    ids0654Blank = 0x028e,
    ids0655Blank = 0x028f,
    idsTechnicalAdvice = 0x0290,
    ids0657Blank = 0x0291,
    idsDavidPugh = 0x0292,
    ids0659Blank = 0x0293,
    ids0660Blank = 0x0294,
    idsMusic2 = 0x0295,
    ids0662Blank = 0x0296,
    idsEmilHerceg = 0x0297,
    ids0664Blank = 0x0298,
    ids0665Blank = 0x0299,
    idsSoundEffects = 0x029a,
    ids0667Blank = 0x029b,
    idsMahendraSampath = 0x029c,
    ids0669Blank = 0x029d,
    ids0670Blank = 0x029e,
    idsPlayTesters = 0x029f,
    ids0672Blank = 0x02a0,
    idsSamBelcher = 0x02a1,
    idsBillBolosky = 0x02a2,
    idsDaveBuchthal = 0x02a3,
    idsKentCedola = 0x02a4,
    idsPeterCelella = 0x02a5,
    idsDanielChenault = 0x02a6,
    idsPaulEnfield = 0x02a7,
    idsMichaelGrier = 0x02a8,
    idsPeterHenriksen = 0x02a9,
    idsWilliamHerlan = 0x02aa,
    idsPeteHorodan = 0x02ab,
    idsBrentJensen = 0x02ac,
    idsMarkKenworthy = 0x02ad,
    idsStuKlingman = 0x02ae,
    idsSteveKruy = 0x02af,
    idsRobertLamb = 0x02b0,
    idsJimLane = 0x02b1,
    idsHiltonLange = 0x02b2,
    idsJonLevee = 0x02b3,
    idsChrisMcbride = 0x02b4,
    idsJeffMccashland = 0x02b5,
    idsBethMoursund = 0x02b6,
    idsChrisNoon = 0x02b7,
    idsTonyPacheco = 0x02b8,
    idsChrisPeltz = 0x02b9,
    idsTonyReynolds = 0x02ba,
    idsJenniferSchlickbernd = 0x02bb,
    idsErikSnapper = 0x02bc,
    idsAndrewSterian = 0x02bd,
    idsJeffStone = 0x02be,
    idsRichardSun = 0x02bf,
    idsDavidThiel = 0x02c0,
    idsBradThompson = 0x02c1,
    idsThomasVoigt = 0x02c2,
    idsRossYoungs = 0x02c3,
    ids0708Blank = 0x02c4,
    idsNewTurnAvailableWouldLikeLoad = 0x02c5,
    idsNewTurnAvailable = 0x02c6,
    idsSorryTurnHasAlreadyGeneratedAnyChanges = 0x02c7,
    idsAutoGenerateDisabledBecauseHumanPlayersDead = 0x02c8,
    idsNoteStarsPrefersScreenResolutionLeast800x600 = 0x02c9,
    idsFileCreatedNewerVersionStarsMustUpgrade = 0x02ca,
    idsDead = 0x02cb,
    idsTurned = 0x02cc,
    idsStill = 0x02cd,
    idsPartiallyDone = 0x02ce,
    idsCorrupted = 0x02cf,
    idsRightYear = 0x02d0,
    idsRightGame = 0x02d1,
    idsLocationDD = 0x02d2,
    idsFieldTypeS = 0x02d3,
    idsFieldRadiusDLYLdMines = 0x02d4,
    idsDecayRateLdYear = 0x02d5,
    idsMinesLaidPerYear = 0x02d6,
    idsMaximumSafeSpeed = 0x02d7,
    idsChanceLYHit = 0x02d8,
    idsDmgDoneEachShip = 0x02d9,
    idsMinDamageDoneFleet = 0x02da,
    idsNumbersParenthesisFleetsContainingShipRamScoop = 0x02db,
    idsRenameFleet = 0x02dc,
    idsStarbaseHullRequiresLesserRacialTraitImproved = 0x02dd,
    idsStarbaseHullHasSpaceDockCanBuild = 0x02de,
    idsStarbaseHullDoesHaveSpaceDockCan = 0x02df,
    idsAllowsFleetsWithoutCargoJumpAnyOther = 0x02e0,
    idsAllowsPlanetsFlingMineralPacketsOtherPlanets = 0x02e1,
    idsDone = 0x02e2,
    idsCancel = 0x02e3,
    idsDesign = 0x02e4,
    idsView = 0x02e5,
    idsDeleteDesign = 0x02e6,
    idsEditDesign = 0x02e7,
    idsWorkDone = 0x02e8,
    idsCargo3 = 0x02e9,
    idsFuel3 = 0x02ea,
    idsDock = 0x02eb,
    idsMax = 0x02ec,
    idsUnlimited = 0x02ed,
    idsDD = 0x02ee,
    idsLdLd = 0x02ef,
    idsD = 0x02f0,
    idsD2 = 0x02f1,
    idsN16 = 0x02f2,
    idsNeedsD = 0x02f3,
    idsD3 = 0x02f4,
    idsDSeconds = 0x02f5,
    idsD02d = 0x02f6,
    idsD02d02d = 0x02f7,
    idsDDaysD02d02d = 0x02f8,
    idsRequiresExactly = 0x02f9,
    idsCanHold = 0x02fa,
    idsOne = 0x02fb,
    idsOr = 0x02fc,
    idsSS = 0x02fd,
    idsKt = 0x02fe,
    idsN00 = 0x02ff,
    idsCostS = 0x0300,
    idsHull = 0x0301,
    idsCostOneSS = 0x0302,
    idsMaxFuel = 0x0303,
    idsArmor = 0x0304,
    idsShields = 0x0305,
    idsRating = 0x0306,
    idsDamage = 0x0307,
    idsPredefinedRace = 0x0308,
    idsCustomRace = 0x0309,
    idsComputerPlayer = 0x030a,
    idsPlayer = 0x030b,
    idsPlayer2 = 0x030c,
    idsEditRace = 0x030d,
    idsNew = 0x030e,
    idsOpen = 0x030f,
    idsS = 0x0310,
    idsSS2 = 0x0311,
    idsSSComputerPlayer = 0x0312,
    idsSXD = 0x0313,
    idsSHD = 0x0314,
    idsGameXy = 0x0315,
    idsStarsGameFilesXy = 0x0316,
    idsStarsGameFilesRFiles = 0x0317,
    idsStarsGameFilesMHstRStars = 0x0318,
    idsPlayer16 = 0x0319,
    idsNewTurnAvailable2 = 0x031a,
    idsHostModeDPlayer = 0x031b,
    idsOut = 0x031c,
    idsC04d04d04d04d = 0x031d,
    idsCCD = 0x031e,
    idsRepeatOrders = 0x031f,
    idsDeceased = 0x0320,
    idsMineralsHand = 0x0321,
    idsMines4 = 0x0322,
    idsFactories4 = 0x0323,
    idsResourcesYear = 0x0324,
    idsPopulation4 = 0x0325,
    idsScannerType = 0x0326,
    idsScannerRange = 0x0327,
    idsDDLY = 0x0328,
    idsDLightYears = 0x0329,
    idsDLY = 0x032a,
    idsDefenses4 = 0x032b,
    idsDefenseType = 0x032c,
    idsDefCoverage = 0x032d,
    idsProduction = 0x032e,
    idsDYears = 0x032f,
    idsDDYears = 0x0330,
    idsDYear = 0x0331,
    idsNever = 0x0332,
    idsSkipped = 0x0333,
    idsNeeded = 0x0334,
    idsDanger = 0x0335,
    idsUnload2 = 0x0336,
    idsUncertain = 0x0337,
    idsFleetsOrbit = 0x0338,
    idsOtherFleetsHere = 0x0339,
    idsPlanetView = 0x033a,
    idsPlanet2 = 0x033b,
    idsQueueEmpty = 0x033c,
    idsTopQueue = 0x033d,
    idsProductionQueueS = 0x033e,
    idsRequiredMinerals = 0x033f,
    idsDDoneCompletion = 0x0340,
    idsSpace = 0x0341,
    idsCost = 0x0342,
    idsStarbase2 = 0x0343,
    idsDockCapacity = 0x0344,
    idsArmor2 = 0x0345,
    idsShields2 = 0x0346,
    idsDamage2 = 0x0347,
    idsLddp = 0x0348,
    idsMassDriver = 0x0349,
    idsMaxed = 0x034a,
    idsNever2 = 0x034b,
    idsLdYearC = 0x034c,
    idsNoneAvailable = 0x034d,
    idsTechReq = 0x034e,
    idsNone3 = 0x034f,
    idsCostLdk = 0x0350,
    idsCostLd = 0x0351,
    idsUnavail = 0x0352,
    idsAvailable = 0x0353,
    idsMassDkt = 0x0354,
    idsWarp = 0x0355,
    idsShieldStrength = 0x0356,
    idsArmorStrength2 = 0x0357,
    idsPower = 0x0358,
    idsRange = 0x0359,
    idsAccuracy = 0x035a,
    idsCurrentlyHaveFleetsUsingBattlePlanIf = 0x035b,
    idsNoteNewPasswordWillTakeEffectUntil = 0x035c,
    idsNotePasswordEffectiveImmediately = 0x035d,
    idsChangeHostPassword = 0x035e,
    idsEnterPassword = 0x035f,
    idsLdLdLightYears = 0x0360,
    idsLdLdLY = 0x0361,
    idsDeepSpace = 0x0362,
    idsSpaceDD = 0x0363,
    idsSSMineField = 0x0364,
    idsOrbitingS = 0x0365,
    idsSD = 0x0366,
    idsShipTransfer = 0x0367,
    idsStopped = 0x0368,
    idsWarpLd = 0x0369,
    idsWarpD2 = 0x036a,
    idsLdLdkt = 0x036b,
    idsLdLdmg = 0x036c,
    idsPercentCloaked = 0x036d,
    idsInfinite = 0x036e,
    idsEstRange = 0x036f,
    idsLdLY = 0x0370,
    idsBestWarp = 0x0371,
    idsFleetComposition = 0x0372,
    idsN9999LY = 0x0373,
    idsFuelCargo = 0x0374,
    idsJettison2 = 0x0375,
    idsXFer = 0x0376,
    idsDeepSpace2 = 0x0377,
    idsMiningRatePerYear = 0x0378,
    idsN99999Kt = 0x0379,
    idsWaypointTask = 0x037a,
    idsEstFuelUsage = 0x037b,
    idsLdkt = 0x037c,
    idsLdmg = 0x037d,
    idsWarpFactor = 0x037e,
    idsTravelTime = 0x037f,
    idsDistance = 0x0380,
    idsFleetWaypoints = 0x0381,
    idsComing = 0x0382,
    idsWayPt = 0x0383,
    idsBattlePlans = 0x0384,
    idsDYearC = 0x0385,
    idsIindefinitely = 0x0386,
    idsRelations = 0x0387,
    idsRelation = 0x0388,
    idsStatus = 0x0389,
    idsSetDest = 0x038a,
    idsRoute2 = 0x038b,
    idsTravelingWarpD = 0x038c,
    idsS2 = 0x038d,
    idsDestination = 0x038e,
    idsN9999992 = 0x038f,
    idsDD2 = 0x0390,
    idsDD3 = 0x0391,
    idsDDEngine = 0x0392,
    idsDD4 = 0x0393,
    idsUseStargate = 0x0394,
    idsSmineralPacket = 0x0395,
    idsSalvage = 0x0396,
    idsDeviceRequiresPrimaryRacialTraitInnerStrength = 0x0397,
    idsDeviceRequiresPrimaryRacialTraitSpaceDemolition = 0x0398,
    idsDeviceRequiresPrimaryRacialTraitInterstellarTrav = 0x0399,
    idsDeviceRequiresPrimaryRacialTraitHyperExpansion = 0x039a,
    idsSlowsShipsCombat1SquareMovement = 0x039b,
    idsReducesEffectivenessOtherPlayersCloaks5 = 0x039c,
    idsActs200mgAntiMatterFuelTankGenerates = 0x039d,
    idsIncreasesDamageDoneBeamWeaponsShipD = 0x039e,
    idsJammingDeviceRequiresPrimaryRacialTraitInner = 0x039f,
    idsHasDChanceDeflectingIncomingTorpedoesDeflected = 0x03a0,
    idsModuleIncreasesAccuracyTorpedoesDIncreasesInitia = 0x03a1,
    idsOriginPartUnknown = 0x03a2,
    idsOriginHullUnknown = 0x03a3,
    idsOriginProcessUnknown = 0x03a4,
    idsOriginEngineUnknownAdds14Square = 0x03a5,
    idsPartAlsoActs100dpShield20Cloak = 0x03a6,
    idsPartAlsoActs10CloakIncreasesTorpedo = 0x03a7,
    idsWeaponCanAlsoBombPlanets2Colonists = 0x03a8,
    idsProcessGivesPlanetNewBirthTracesCivilization = 0x03a9,
    idsOwns = 0x03aa,
    idsPlanets2 = 0x03ab,
    ids0940Blank = 0x03ac,
    idsAttainsTech = 0x03ad,
    idsIn = 0x03ae,
    idsFields = 0x03af,
    idsExceedsScore = 0x03b0,
    idsMsg0945 = 0x03b1,
    ids0946Blank = 0x03b2,
    idsExceedsSecondPlaceScore = 0x03b3,
    idsMsg0948 = 0x03b4,
    ids0949Blank = 0x03b5,
    idsHasProductionCapacity = 0x03b6,
    idsThousand = 0x03b7,
    ids0952Blank = 0x03b8,
    idsOwns2 = 0x03b9,
    idsCapitalShips2 = 0x03ba,
    ids0955Blank = 0x03bb,
    idsHasHighestScoreAfter = 0x03bc,
    idsYears = 0x03bd,
    ids0958Blank = 0x03be,
    idsWinnerMustMeet = 0x03bf,
    idsAboveSelectedCriteria = 0x03c0,
    ids0961Blank = 0x03c1,
    idsLeast = 0x03c2,
    idsYearsMustPassBeforeWinnerDeclared = 0x03c3,
    ids0964Blank = 0x03c4,
    idsPlanets3 = 0x03c5,
    idsHistory = 0x03c6,
    idsRockSolid = 0x03c7,
    idsStable = 0x03c8,
    idsMostlyStable = 0x03c9,
    idsAverage = 0x03ca,
    idsSlightlyVolatile = 0x03cb,
    idsVolatile = 0x03cc,
    idsExtremelyVolatile = 0x03cd,
    idsLocation = 0x03ce,
    idsDestination2 = 0x03cf,
    idsStability = 0x03d0,
    idsDD5 = 0x03d1,
    idsTraderRequestsInterestedPartiesSendFleetLeast = 0x03d2,
    idsTraderTravelingWarpD = 0x03d3,
    idsEasterBunny = 0x03d4,
    idsKilljoy = 0x03d5,
    idsMommasHelper = 0x03d6,
    idsTurtle = 0x03d7,
    idsPoodle = 0x03d8,
    idsMite = 0x03d9,
    idsGnat = 0x03da,
    idsRobin = 0x03db,
    idsOstrich = 0x03dc,
    idsGoose = 0x03dd,
    idsLyingBastard = 0x03de,
    idsPitBull = 0x03df,
    idsToothlessTiger = 0x03e0,
    idsRhodeIslandRed = 0x03e1,
    idsRamRod = 0x03e2,
    idsSpittingCobra = 0x03e3,
    idsVenomousDreadnought = 0x03e4,
    idsCrownJewel = 0x03e5,
    idsSilverSerpent = 0x03e6,
    idsXenocide = 0x03e7,
    idsTyphoon = 0x03e8,
    idsQuark = 0x03e9,
    idsWhip = 0x03ea,
    idsLash = 0x03eb,
    idsTerror = 0x03ec,
    idsDogWar = 0x03ed,
    idsPidgeon = 0x03ee,
    idsRagingRukh = 0x03ef,
    idsManifestDestiny = 0x03f0,
    idsFlyingCow = 0x03f1,
    idsBitterHarvest = 0x03f2,
    idsPeacock = 0x03f3,
    idsSaguaro = 0x03f4,
    idsBadlandsExpress = 0x03f5,
    idsBrightSpot = 0x03f6,
    idsStrangeLove = 0x03f7,
    idsDrDeath = 0x03f8,
    idsScorch = 0x03f9,
    idsGroundHog = 0x03fa,
    idsNakedMoleRat = 0x03fb,
    idsTerrier = 0x03fc,
    idsPick = 0x03fd,
    idsGouge = 0x03fe,
    idsGorge = 0x03ff,
    idsGut = 0x0400,
    idsAirdale = 0x0401,
    idsEgg = 0x0402,
    idsBusyBee = 0x0403,
    idsSeeder = 0x0404,
    idsSpore = 0x0405,
    idsPhoenix = 0x0406,
    idsPlymouth = 0x0407,
    idsDuty = 0x0408,
    idsVassal = 0x0409,
    idsGlovebox = 0x040a,
    idsPerfectLogic = 0x040b,
    idsBoxcar = 0x040c,
    idsBoot = 0x040d,
    idsPeet = 0x040e,
    idsC74 = 0x040f,
    idsLor = 0x0410,
    idsBlackHold = 0x0411,
    idsPricklyPear = 0x0412,
    idsBristlyLlama = 0x0413,
    idsSilentMule = 0x0414,
    idsSaguaro2 = 0x0415,
    idsCrunchyCritter = 0x0416,
    idsLongJohnSilver = 0x0417,
    idsBlackbeard = 0x0418,
    idsThistle = 0x0419,
    idsZombie = 0x041a,
    idsTyphoid = 0x041b,
    idsZeppo = 0x041c,
    idsLuckyEddie = 0x041d,
    idsWidget = 0x041e,
    idsPoly = 0x041f,
    idsMog = 0x0420,
    idsRanger = 0x0421,
    idsScrapper = 0x0422,
    idsBogey = 0x0423,
    idsHorseFly = 0x0424,
    idsHornet = 0x0425,
    idsDragonFly = 0x0426,
    idsWasp = 0x0427,
    idsIntruder = 0x0428,
    idsInterceptor = 0x0429,
    idsQuest = 0x042a,
    idsInfiniteVision = 0x042b,
    idsBrassKnuckle = 0x042c,
    idsTalon = 0x042d,
    idsNaagra = 0x042e,
    idsCattleProd = 0x042f,
    idsAsunder = 0x0430,
    idsBlade = 0x0431,
    idsGuardianAngel = 0x0432,
    idsSkyFort = 0x0433,
    idsSilverTower = 0x0434,
    idsPentagon = 0x0435,
    idsMonolith = 0x0436,
    idsRockGibraltar = 0x0437,
    idsPotato = 0x0438,
    idsCube = 0x0439,
    idsDeathDemand = 0x043a,
    idsHipSquare = 0x043b,
    idsSphereDoom = 0x043c,
    idsGatewayHell = 0x043d,
    idsEvilSpawn = 0x043e,
    idsAll = 0x043f,
    idsArmor3 = 0x0440,
    idsBeamWeapons = 0x0441,
    idsBombs = 0x0442,
    idsElectrical = 0x0443,
    idsEngines = 0x0444,
    idsMechanical = 0x0445,
    idsMineLayers = 0x0446,
    idsMiningRobots = 0x0447,
    idsOrbital = 0x0448,
    idsPlanetary = 0x0449,
    idsScanners = 0x044a,
    idsShields3 = 0x044b,
    idsShipHulls = 0x044c,
    idsStarbaseHulls = 0x044d,
    idsTerraforming = 0x044e,
    idsTorpedoes = 0x044f,
    idsWeapons2 = 0x0450,
    idsDevices = 0x0451,
    idsSafeHullMass = 0x0452,
    idsSafeRange = 0x0453,
    idsWarningShipsDktMightSuccessfullyGatedD = 0x0454,
    idsWarningShipsCanSuccessfullyGatedDL = 0x0455,
    idsWarningShipsDktCanSuccessfullyGatedExceeding = 0x0456,
    idsWarningReceivingPlanetMustHaveMassDriver = 0x0457,
    idsWarningReceivingPlanetMustHaveMassDriver2 = 0x0458,
    idsPlanetName = 0x0459,
    idsStarbase3 = 0x045a,
    idsPopulation5 = 0x045b,
    idsCap = 0x045c,
    idsValue2 = 0x045d,
    idsProduction2 = 0x045e,
    idsMine2 = 0x045f,
    idsFact = 0x0460,
    idsDefense = 0x0461,
    idsMinerals = 0x0462,
    idsMiningRate2 = 0x0463,
    idsMinConc = 0x0464,
    idsResources2 = 0x0465,
    idsDriverDest = 0x0466,
    idsRoutingDest = 0x0467,
    idsN100100 = 0x0468,
    idsN100 = 0x0469,
    idsN10001000 = 0x046a,
    idsN1000 = 0x046b,
    idsD4 = 0x046c,
    idsSort = 0x046d,
    idsReverseSort = 0x046e,
    idsHide = 0x046f,
    idsColumn = 0x0470,
    idsShow = 0x0471,
    idsFleetName = 0x0472,
    idsId2 = 0x0473,
    idsLocation2 = 0x0474,
    idsDestination3 = 0x0475,
    idsEta = 0x0476,
    idsTask = 0x0477,
    idsFuel4 = 0x0478,
    idsCargo4 = 0x0479,
    idsComposition = 0x047a,
    idsCloak = 0x047b,
    idsBattlePlan = 0x047c,
    idsMass = 0x047d,
    idsFleetName2 = 0x047e,
    idsId3 = 0x047f,
    idsLocation3 = 0x0480,
    idsWarp2 = 0x0481,
    idsMass2 = 0x0482,
    idsComposition2 = 0x0483,
    idsShips = 0x0484,
    idsUnarmed = 0x0485,
    idsScout2 = 0x0486,
    idsWarship2 = 0x0487,
    idsBomber2 = 0x0488,
    idsUtility2 = 0x0489,
    idsLocation4 = 0x048a,
    idsSb = 0x048b,
    idsSides = 0x048c,
    idsUnits = 0x048d,
    idsOurs = 0x048e,
    idsTheirs = 0x048f,
    idsUnarmed2 = 0x0490,
    idsScout3 = 0x0491,
    idsWarship3 = 0x0492,
    idsBomber3 = 0x0493,
    idsUtility3 = 0x0494,
    idsOurDead = 0x0495,
    idsDead2 = 0x0496,
    idsOursLeft = 0x0497,
    idsTheirsLeft = 0x0498,
    idsPlanetSummaryReportDPlanetC = 0x0499,
    idsFleetSummaryReportDFleetC = 0x049a,
    idsOthersFleetsSummaryReportDFleetC = 0x049b,
    idsBattleSummaryReportDBattleC = 0x049c,
    idsDelayed = 0x049d,
    idsDy = 0x049e,
    idsGeneratingDataYearD = 0x049f,
    idsLdD = 0x04a0,
    idsDestroyingDShip = 0x04a1,
    idsLdDamageShieldsS = 0x04a2,
    idsLdDamageArmorC = 0x04a3,
    idsSelectionDD = 0x04a4,
    idsShieldsLd = 0x04a5,
    idsShieldsNone = 0x04a6,
    idsDamageLdD = 0x04a7,
    idsDamageD = 0x04a8,
    idsDamageNone = 0x04a9,
    idsArmorLd = 0x04aa,
    idsMovementS = 0x04ab,
    idsInitiativeMoves = 0x04ac,
    idsInitMove = 0x04ad,
    idsCloakJam = 0x04ae,
    idsScannerRange2 = 0x04af,
    idsScanner = 0x04b0,
    idsDD6 = 0x04b1,
    idsDDD = 0x04b2,
    idsDS = 0x04b3,
    idsDDDoing = 0x04b4,
    idsWithinDLY = 0x04b5,
    idsAnyEnemy = 0x04b6,
    idsHullWillManufacture200UnitsFuelEach = 0x04b7,
    idsHullWillDoubleEfficiencyMineLayingPods = 0x04b8,
    idsDead3 = 0x04b9,
    idsPlayerScores = 0x04ba,
    idsVictoryConditions = 0x04bb,
    idsProgressTimeline = 0x04bc,
    idsDetonateMineFieldYear = 0x04bd,
    idsUnusedD = 0x04be,
    idsCustomD = 0x04bf,
    idsCustomize = 0x04c0,
    idsCustomOrders = 0x04c1,
    idsRenameZipOrder = 0x04c2,
    idsRenameProductionTemplate = 0x04c3,
    idsAutoBuildOrders = 0x04c4,
    idsSD2 = 0x04c5,
    idsContributeResearch = 0x04c6,
    idsDontContributeResearch = 0x04c7,
    idsEmptyCustomSlot = 0x04c8,
    idsC = 0x04c9,
    idsPleaseEnterUniqueEightCharacterSerialNumber = 0x04ca,
    idsMachineConfigurationAppearsHaveChangedPleaseRe = 0x04cb,
    idsPleaseEnterOwnUniqueSerialNumberPrevent = 0x04cc,
    idsSerialNumberHaveEnteredValid = 0x04cd,
    idsStarsTutorPageD80 = 0x04ce,
    idsTutorialHaveGivenFleetWrongNamePlease = 0x04cf,
    idsTorpedoesDeflected = 0x04d0,
    idsJammingD = 0x04d1,
    idsWarningDestinationWaypointFleetMergeWillSucessfu = 0x04d2,
    idsSorryFileCreatedOlderVersionStarsIncompatible = 0x04d3,
    idsArmorRequiresPrimaryRacialTraitInnerStrength = 0x04d4,
    idsCosts75ExtraResearchFieldsStartTech = 0x04d5,
    idsUniverseDefinitionHasSuccessfullyWrittenSMap = 0x04d6,
    idsUnableWriteUniverseDefinitionSMapOperation = 0x04d7,
    idsKnownPlanetInformationHasSuccessfullyWrittenS = 0x04d8,
    idsUnableWritePlanetInformationSOperationTerminated = 0x04d9,
    idsKnownFleetInformationHasSuccessfullyWrittenS = 0x04da,
    idsUnableWriteFleetInformationSOperationTerminated = 0x04db,
    idsPlanetNameOwnerStarbaseTypeReportAge = 0x04dc,
    idsIronMrBoraMrGermMrIron = 0x04dd,
    idsGravTempRadGravorigTemporigRadorigTerra = 0x04de,
    idsFleetNameXYPlanetDestinationBattle = 0x04df,
    idsShipCntIronBoraGermColFuel = 0x04e0,
    idsOwnerEtaWarpMassCloakScanPen = 0x04e1,
    idsMineField = 0x04e2,
    idsMineralPacket = 0x04e3,
    idsWormhole = 0x04e4,
    idsMysteryTrader = 0x04e5,
    idsSalvageField = 0x04e6,
    idsMysteryObject = 0x04e7,
    idsFleet = 0x04e8,
    idsRoute3 = 0x04e9,
    idsN = 0x04ea,
    idsRaceIncapableBuildingFactories = 0x04eb,
    idsRaceIncapableBuildingMinesHoweverColonistsHave = 0x04ec,
    idsOrganic = 0x04ed,
    idsRaceCannotBuildPlanetaryScannersStarbasesHave = 0x04ee,
    idsPlanetaryScannersDefensesAvailableAlternateReali = 0x04ef,
    idsMsg1264 = 0x04f0,
    idsDockCapacity2 = 0x04f1,
    idsPhaseDDRoundDD = 0x04f2,
    idsPlaybackSpeedD = 0x04f3,
    idsWeaponWillDamageShieldsHasEffectArmor = 0x04f4,
    idsWeaponHitsTargetsRangeEachTimeFired = 0x04f5,
    idsWeaponAlsoMakesExcellentMineSweeperCapable = 0x04f6,
    idsMaxPopulation = 0x04f7,
    idsMaxPop = 0x04f8,
    idsBattlePlan2 = 0x04f9,
    idsIntercept = 0x04fa,
    idsDesigns = 0x04fb,
    idsInvertFilter = 0x04fc,
    idsDesigns2 = 0x04fd,
    idsMineFields = 0x04fe,
    idsMineFields2 = 0x04ff,
    idsMineFields3 = 0x0500,
    idsMineFieldsFriends = 0x0501,
    idsMineFieldsNeutrals = 0x0502,
    idsMineFieldsEnemies = 0x0503,
    idsTacticS = 0x0504,
    idsTacticSDMoves = 0x0505,
    idsPrimayTargetS = 0x0506,
    idsSecondaryTargetS = 0x0507,
    idsAutomatic = 0x0508,
    idsInitiativeD = 0x0509,
    idsRaceHas = 0x050a,
    idsN9999999 = 0x050b,
    idsEvery = 0x050c,
    idsHours = 0x050d,
    ids1294Blank = 0x050e,
    ids1295Blank = 0x050f,
    idsMinutesAfter = 0x0510,
    idsPlayerSLeft = 0x0511,
    idsCantCopyShipDesignBecauseCantBuild = 0x0512,
    idsSmartBombsStrictlyAdditiveHaveMinimumKill = 0x0513,
    idsPartUnavailbleWarMonger = 0x0514,
    idsAdvantagePointsCurrentlyHoleDPointsCannot = 0x0515,
    idsCantHaveDWaypoints = 0x0516,
    idsSendMessagesDD = 0x0517,
    idsUpTo = 0x0518,
    idsTutorialContributeLeftoverCheckboxProductionQueu = 0x0519,
    idsMakeTutorialReappearCompleteTaskChooseTutorial = 0x051a,
    idsPlanetaryScanner = 0x051b,
    idsTorpedoesDeflected2 = 0x051c,
    idsDamage3 = 0x051d,
    idsHw = 0x051e,
    idsN30 = 0x051f,
    idsStarsUniverseMap = 0x0520,
    idsYearD = 0x0521,
    idsPlanet3 = 0x0522,
    idsOrbitalFort = 0x0523,
    idsStarbase4 = 0x0524,
    idsUnoccupiedPlanet = 0x0525,
    idsPlayer2sPlanet = 0x0526,
    idsMustSpecifyNumberBetween19 = 0x0527,
    idsUnablePrintGameMapPrinterMayOff = 0x0528,
    idsCapitalShipMissilesDoTwiceStatedDamage = 0x0529,
    idsDemoVersionStarsLimitedGames80Years = 0x052a,
    idsStarsSHostMode = 0x052b,
    idsWaitingNewTurn = 0x052c,
    idsTemperature = 0x052d,
    idsResearch = 0x052e,
    idsAdvantage = 0x052f,
    idsPointsLeft = 0x0530,
    idsStarsRaceFilesR = 0x0531,
    idsRandom2 = 0x0532,
    idsPredefinedRaces = 0x0533,
    idsN2 = 0x0534,
    idsTo2 = 0x0535,
    idsTo3 = 0x0536,
    idsArial2 = 0x0537,
    idsArialBold = 0x0538,
    idsArialItalic = 0x0539,
    idsArialBoldItalic = 0x053a,
    idsAttacksS = 0x053b,
    idsAnd2 = 0x053c,
    idsDmg = 0x053d,
    idsLevel = 0x053e,
    idsSTechLevelD = 0x053f,
    idsBleedingEdge = 0x0540,
    idsNum = 0x0541,
    idsStandard = 0x0542,
    idsSmart = 0x0543,
    idsDecreased = 0x0544,
    idsIncreased = 0x0545,
    idsOf2 = 0x0546,
    idsOrigin = 0x0547,
    idsFiltered = 0x0548,
    idsEverybody = 0x0549,
    idsSCC = 0x054a,
    idsSCC2 = 0x054b,
    idsPrev2 = 0x054c,
    idsGoto3 = 0x054d,
    idsNext2 = 0x054e,
    idsDelete = 0x054f,
    idsReply = 0x0550,
    idsLdktYr = 0x0551,
    idsSInfo = 0x0552,
    idsOld2 = 0x0553,
    idsSummary = 0x0554,
    idsDeepSpaceWaypoint = 0x0555,
    idsLy = 0x0556,
    idsLightYears = 0x0557,
    idsFrom = 0x0558,
    idsName = 0x0559,
    idsHave2 = 0x055a,
    idsAre = 0x055b,
    idsHas = 0x055c,
    idsIs2 = 0x055d,
    idsPlayerD2 = 0x055e,
    idsWeightedAverage = 0x055f,
    idsNone4 = 0x0560,
    idsFieldDD = 0x0561,
    idsSureWantForceGenerateDTurnsRow = 0x0562,
    idsGeneratingYearD = 0x0563,
    idsFailed = 0x0564,
    idsSucceeded = 0x0565,
    idsCantFindHostFile = 0x0566,
    idsHumanoid = 0x0567,
    idsRabbitoid = 0x0568,
    idsInsectoid = 0x0569,
    idsNucleotid = 0x056a,
    idsSilicanoid = 0x056b,
    idsAntetheral = 0x056c,
    idsRandom3 = 0x056d,
    idsBerserker = 0x056e,
    idsBulushi = 0x056f,
    idsGolem = 0x0570,
    idsNulon = 0x0571,
    idsTritizoid = 0x0572,
    idsValadiac = 0x0573,
    idsUbert = 0x0574,
    idsFelite = 0x0575,
    idsFerret = 0x0576,
    idsHouseCat = 0x0577,
    idsCrusher = 0x0578,
    idsPicardi = 0x0579,
    idsRushn = 0x057a,
    idsAmerican = 0x057b,
    idsHawk = 0x057c,
    idsEagle = 0x057d,
    idsMensoid = 0x057e,
    idsLoraxoid = 0x057f,
    idsHicardi = 0x0580,
    idsNairnian = 0x0581,
    idsCleaver = 0x0582,
    idsHooveron = 0x0583,
    idsNee = 0x0584,
    idsKurkonian = 0x0585,
};
typedef uint16_t StringId;
enum MessageId {
    idmColonistsDroppedMassacredGroundTroops = 0x0000,
    idmColonistsDroppedDestroyedPlanetaryDefensesRestMa = 0x0001,
    idmColonistsForcedTransportDiedBecauseDidColonize = 0x0002,
    idmGroundTroopsValiantlyDestroyedAttackingBarbarian = 0x0003,
    idmPlanetaryDefensesGroundTroopsDestroyedInvadingTr = 0x0004,
    idmMultitudeEnemiesHaveMountedProngAttackResulting = 0x0005,
    idmInvolvedWayAssaultNobodysTroopsSurvivedBrutal = 0x0006,
    idmHaveAttackedFirstRateStormTroopersThough = 0x0007,
    idmInvolvedWayRaceUninhabitedPlanetForcesCrush = 0x0008,
    idmColonistsDestroyedWayRaceUninhabitedPlanetContro = 0x0009,
    idmColonistsControl = 0x000a,
    idmColonistsHaveDeployedOrbitalConstructionModuleHa = 0x000b,
    idmTroopsCrushSColonistsControlPlanet = 0x000c,
    idmColonistsDroppedDestroyedSpiritedFighting = 0x000d,
    idmThereMassiveBloodBathInvolvingFleetsRaces = 0x000e,
    idmSlaughteredOppositionWithoutLosingSingleShip = 0x000f,
    idmDestroyedOppositionMinimalLosses = 0x0010,
    idmDefeatedOppositionSufferedHeavyLosses = 0x0011,
    idmFleetsDestroyedOpposition = 0x0012,
    idmFleetsDefeatedOppositionSufferedHeavyLosses = 0x0013,
    idmFleetObliteratedWayStruggleWhichSurvived = 0x0014,
    idmFleetsDestroyedWayBattleLeavingSoleSurvivor = 0x0015,
    idmPeopleWitnessedSpectacleFleetsOtherRacesOblitera = 0x0016,
    idmColonyObservedForcesDefeatingForcesOtherRaces = 0x0017,
    idmForcesDestroyedEachOther = 0x0018,
    idmGloriousStompedForces = 0x0019,
    idmMightyDefeatedForcesTookHeavyCasualties = 0x001a,
    idmFleetsTrouncedBarbarousForces = 0x001b,
    idmVigilantFleetsManagedDefeatSavageVerminWithout = 0x001c,
    idmBraveForcesObliteratedVastlyGreaterForcesCowardl = 0x001d,
    idmForcesDiedValiantlyTakingManyVerminThem = 0x001e,
    idmLostTerribleMassacrePerpetratedVillainous = 0x001f,
    idmCloseFightDidGreatDamageForcesBefore = 0x0020,
    idmPeopleWatchedAmazementForcesAnnihilatedEachOther = 0x0021,
    idmColonyObservedForcesDefeatingForces = 0x0022,
    idmColonistsHaveDiedOffLongerControlPlanet = 0x0023,
    idmColonistsOrbitingHaveDiedOffStarbaseHas = 0x0024,
    idmPopulationHasDecreased = 0x0025,
    idmPopulationHasDecreasedColonistsDueOvercrowding = 0x0026,
    idmHasRunFuel = 0x0027,
    idmSWaypointAppearsHaveDestroyedHasDisappeared = 0x0028,
    idmFleetTrackingAppearsHaveDuckedBehindOrders = 0x0029,
    idmFleetTrackingAppearsHaveOutrunRangeScanners = 0x002a,
    idmHasLoaded = 0x002b,
    idmHasBeamed = 0x002c,
    idmHasUnloaded = 0x002d,
    idmHasBeamed2 = 0x002e,
    idmStarbaseHasBuiltNew = 0x002f,
    idmStarbaseHasBuiltNewShips = 0x0030,
    idmStarbaseHasBuiltNewWhichRouted = 0x0031,
    idmStarbaseHasBuiltNewShipsWhichRouted = 0x0032,
    idmStarbaseHasBuiltNewWhichWillRouted = 0x0033,
    idmStarbaseHasBuiltNewShipsWhichWill = 0x0034,
    idmHaveBuiltFactory = 0x0035,
    idmHaveBuiltFactories = 0x0036,
    idmHaveBuiltMine = 0x0037,
    idmHaveBuiltMines = 0x0038,
    idmHaveBuiltDefenseOutpost = 0x0039,
    idmHaveBuiltDefenseOutposts = 0x003a,
    idmHaveUpgradedDefensesUseTechnology = 0x003b,
    idmThereIsntEnoughFuelAvailableAllowGet = 0x003c,
    idmWillNeverMakeWaypointFuelCapacityMg = 0x003d,
    idmHasCompletedOrdersProductionQueueEmpty = 0x003e,
    idmProductionQueueEmpty = 0x003f,
    idmColonistsHaveJumpedShipLongerControlPlanet = 0x0040,
    idmColonistsOrbitingHaveAbandonedStarbaseLongerCont = 0x0041,
    idmSuccessfullyTransferred = 0x0042,
    idmSuccessfullyTransferred2 = 0x0043,
    idmSuccessfullyReceived = 0x0044,
    idmSuccessfullyReceived2 = 0x0045,
    idmAttemptedTransferSuccessfullyReceived = 0x0046,
    idmAttemptedTransferColonistsSuccessfullyReceivedRe = 0x0047,
    idmReceivedHoweverSentRemainderLostSpace = 0x0048,
    idmReceivedHoweverColonistsSentRemainsOtherColonist = 0x0049,
    idmAttemptedTransferNoneSuccessfullyReceived = 0x004a,
    idmAttemptedTransferNoneColonistsSuccessfullyReceiv = 0x004b,
    idmAttemptedReceiveHoweverLostDeepSpace = 0x004c,
    idmAttemptedReceiveHoweverNoneColonistsSuccessfully = 0x004d,
    idmHasCompletedAssignedOrders = 0x004e,
    idmStarbaseFailedBuildNewShipTypeBecause = 0x004f,
    idmScientistsHaveCompletedResearchTechLevelWill = 0x0050,
    idmHasOrderColonizeCurrentlyOrbitPlanetOrder = 0x0051,
    idmHasOrdersColonizeAlreadyPopulatedColonizeOrder = 0x0052,
    idmHasOrdersColonizeHaveFailedBringAlong = 0x0053,
    idmHasOrdersColonizeNoneShipsHaveColonization = 0x0054,
    idmHasTriedBeamColonistsPlanetUninhabitedMust = 0x0055,
    idmCaptainHasAttemptedBeamColonistsOverruledBridge = 0x0056,
    idmColonistsAttemptingSetShopReducedProtoplasmicBlo = 0x0057,
    idmColonistsAssaultingHaveKilledForcesOrbitingStarb = 0x0058,
    idmHasDismantledKtMineralsWhichHaveDeposited = 0x0059,
    idmHasDismantledKtMineralsStarbaseOrbiting = 0x005a,
    idmHasDismantledScrapLeftDeepSpace = 0x005b,
    idmHasDismantledKtMineralsWhichHaveDeposited2 = 0x005c,
    idmHasDismantledKtMineralsStarbaseOrbitingUltimate = 0x005d,
    idmColonistsSettlingHaveFoundStrangeArtifactBoostin = 0x005e,
    idmRecentBreakthroughHasAlsoGivenBenefit = 0x005f,
    idmHasBombedKillingColonists = 0x0060,
    idmHasBombedDestroyingOneInstallation = 0x0061,
    idmHasBombedDestroyingDefensesFactoriesMines = 0x0062,
    idmHasBombedKillingColonistsDestroyingOneInstallati = 0x0063,
    idmHasBombedKillingColonistsDestroyingDefensesFacto = 0x0064,
    idmHasBombedKillingColonistsPlanetaryDefensesStoppe = 0x0065,
    idmHasBombedDestroyingOneInstallationPlanetaryDefen = 0x0066,
    idmHasBombedDestroyingFactoriesMinesPlanetaryDefens = 0x0067,
    idmHasBombedKillingColonistsDestroyingOneInstallati2 = 0x0068,
    idmHasBombedKillingColonistsDestroyingDefensesFacto2 = 0x0069,
    idmHasBombedKillingColonists2 = 0x006a,
    idmHasBombedDestroyingOneInstallations = 0x006b,
    idmHasBombedDestroyingDefensesFactoriesMines2 = 0x006c,
    idmHasBombedKillingColonistsDestroyingOneInstallati3 = 0x006d,
    idmHasBombedKillingColonistsDestroyingDefensesFacto3 = 0x006e,
    idmHasBombedKillingColonistsPlanetaryDefensesDestro = 0x006f,
    idmHasBombedDestroyingOneInstallationsPlanetaryDefe = 0x0070,
    idmHasBombedDestroyingDefensesFactoriesMinesPlaneta = 0x0071,
    idmHasBombedKillingColonistsDestroyingOneInstallati4 = 0x0072,
    idmHasBombedKillingColonistsDestroyingDefensesFacto4 = 0x0073,
    idmEngineRadiationHasKilledColonistsTraveling = 0x0074,
    idmHadOrdersMineFleetDoesntHaveAny = 0x0075,
    idmRemoteMiningRobotsHadOrdersMinePlanet = 0x0076,
    idmRemoteMiningRobotsHadOrdersMineDeep = 0x0077,
    idmRecentBreakthroughHasAlsoGivenHullType = 0x0078,
    idmHasLoaded2 = 0x0079,
    idmHasBeamed3 = 0x007a,
    idmTerraformingEffortsHave = 0x007b,
    idmHasBuiltNewPlanetaryScanner = 0x007c,
    idmHasLoadedMiningRobotsWorking = 0x007d,
    idmBattleTookPlacePressGotoButtonView = 0x007e,
    idmTipCanHideUnimportantMessagesClickingCheckmark = 0x007f,
    idmTipAddWaypointsSelectShipClickDesired = 0x0080,
    idmTipDesignOwnShipsPressF4Select = 0x0081,
    idmTipPopupHelpAvailableManyDisplayedStatistics = 0x0082,
    idmSmallCometHasCrashedBringingNewMinerals = 0x0083,
    idmMediumSizedCometHasCrashedBringingSignificant = 0x0084,
    idmLargeCometHasCrashedBringingWideVariety = 0x0085,
    idmHugeCometHasCrashedEmbeddingVastQuantities = 0x0086,
    idmSmallCometHasCrashedPlanetKilling25 = 0x0087,
    idmMediumSizedCometHasCrashedPlanetKilling = 0x0088,
    idmLargeCometHasCrashedPlanetKilling65 = 0x0089,
    idmHugeCometHasCrashedPlanetKilling85 = 0x008a,
    idmHasRunFuelFleetsSpeedHasDecreased = 0x008b,
    idmScientistsHaveTransmutedCommonMaterialsKtEach = 0x008c,
    idmBattleTookPlaceDestroyedScreamsColonistsEcho = 0x008d,
    idmBattleTookPlaceDestroyedColonistsHaveJoined = 0x008e,
    idmHasBombedKillingOffEnemyColonists = 0x008f,
    idmHasBombedKillingColonists3 = 0x0090,
    idmBattleTookPlaceDestroyedTakingDamage = 0x0091,
    idmBattleTookPlaceDestroyedWhichTookDamage = 0x0092,
    idmBattleTookPlaceDestroyedHoweverTookDamage = 0x0093,
    idmBattleTookPlaceDestroyedWhichDamagedFray = 0x0094,
    idmBattleTookPlaceNeitherNorDestroyedIncident = 0x0095,
    idmBattleTookPlaceDestroyedTakingDamage2 = 0x0096,
    idmBattleTookPlaceDestroyedWhichTookDamage2 = 0x0097,
    idmBattleTookPlaceDestroyedHoweverTookDamage2 = 0x0098,
    idmBattleTookPlaceDestroyedWhichDamagedFray2 = 0x0099,
    idmBattleTookPlaceNeitherNorCompletelyDestroyed = 0x009a,
    idmBattleTookPlaceAgainstForcesDestroyedEnemy = 0x009b,
    idmBattleTookPlaceAgainstForcesDestroyedEnemys = 0x009c,
    idmBattleTookPlaceAgainstForcesDestroyedEnemy2 = 0x009d,
    idmBattleTookPlaceAgainstForcesDestroyedEnemys2 = 0x009e,
    idmBattleTookPlaceAgainstNeitherForcesNor = 0x009f,
    idmBattleTookPlaceAgainstForcesDestroyedTaking = 0x00a0,
    idmBattleTookPlaceAgainstDestroyedEnemysForces = 0x00a1,
    idmBattleTookPlaceAgainstForcesDestroyedHowever = 0x00a2,
    idmBattleTookPlaceAgainstDestroyedEnemysForces2 = 0x00a3,
    idmBattleTookPlaceInvolvingRacesForcesDestroyed = 0x00a4,
    idmBattleTookPlaceInvolvingRacesLostForces = 0x00a5,
    idmBattleTookPlaceInvolvingRacesEntireArmada = 0x00a6,
    idmBattleTookPlaceInvolvingRacesEntireArmada2 = 0x00a7,
    idmBattleTookPlaceInvolvingRacesLostForces2 = 0x00a8,
    idmHomePlanetPeopleReadyLeaveNestExplore = 0x00a9,
    idmHaveFoundPlanetOccupiedSomeoneElseCurrently = 0x00aa,
    idmHaveFoundNewPlanetWhichUnfortunatelyHabitable = 0x00ab,
    idmHaveFoundNewHabitablePlanetColonistsWill = 0x00ac,
    idmHaveFoundNewPlanetDontKnowIf = 0x00ad,
    idmHaveFoundNewPlanetWhichHaveAbility = 0x00ae,
    idmHasBuiltManyMinesCurrentPopulationCan = 0x00af,
    idmHasBuiltManyMinesPlanetCanSupport = 0x00b0,
    idmHasBuiltManyFactoriesCurrentPopulationCan = 0x00b1,
    idmHasBuiltManyFactoriesPlanetCanSupport = 0x00b2,
    idmHasBuiltManyDefensesCurrentPopulationCan = 0x00b3,
    idmHasBuiltManyDefensesPlanetCanSupport = 0x00b4,
    idmForcesHaveDeclaredWinnerGameAdvisedAccept = 0x00b5,
    idmHaveDeclaredWinnerGameMayContinuePlay = 0x00b6,
    idmAlongHaveDeclaredWinnersGameMayContinue = 0x00b7,
    idmDeadPlanetsHaveOverrunSpaceshipsDefeated = 0x00b8,
    idmOrderBuildScannerCanceledAlreadyHaveScanner = 0x00b9,
    idmStarbaseBuiltNewShipSTypeLost = 0x00ba,
    idmTracesHaveEliminatedGalaxyMayRestPeace = 0x00bb,
    idmTracesEveryOtherRivalHaveEliminatedGalaxy = 0x00bc,
    idmHasAccomplishedRemoteTerraformingCurrentlyCapabl = 0x00bd,
    idmSomeoneHasSweptMinesMineField = 0x00be,
    idmHasAttemptedLayMinesOrderHasCanceled = 0x00bf,
    idmMysteryTraderHasDecidedMakeAnotherPass = 0x00c0,
    idmDueRigorsWarpAccelerationColonistsHaveDied = 0x00c1,
    idmHasSweptMinesMineField = 0x00c2,
    idmHasDispersedMines = 0x00c3,
    idmHasIncreasedMinefieldMines = 0x00c4,
    idmHasStoppedMineField = 0x00c5,
    idmHasStoppedMineFieldFleetHasTaken = 0x00c6,
    idmHasStoppedMineFieldFleetHasTaken2 = 0x00c7,
    idmHasAnnihilatedMineField = 0x00c8,
    idmHasStoppedMineField2 = 0x00c9,
    idmHasStoppedMineFieldMinesHaveInflicted = 0x00ca,
    idmHasStoppedMineFieldMinesHaveInflicted2 = 0x00cb,
    idmHasAnnihilatedMineField2 = 0x00cc,
    idmHasBuiltNew = 0x00cd,
    idmHasBuiltNewShipsKtTotalHull = 0x00ce,
    idmHasBuiltNewShipsAnySizeCan = 0x00cf,
    idmRecentBreakthroughHasAlsoGivenHullDesign = 0x00d0,
    idmMineralPacketFormedHasDisintegratedBecausePlanet = 0x00d1,
    idmMineralPacketFormedHasDisintegratedBecauseDidnt = 0x00d2,
    idmHasProducedMineralPacketWhichHasDestination = 0x00d3,
    idmHasProducedMineralPacketWhichHasCombined = 0x00d4,
    idmMassAcceleratorHasSuccessfullyCapturedPacketCont = 0x00d5,
    idmMassAcceleratorPartiallySuccessfullyCapturingKtM = 0x00d6,
    idmMassAcceleratorPartiallySuccessfullyCapturingKtM2 = 0x00d7,
    idmBombardedKtMineralPacketColonistsKilledCollision = 0x00d8,
    idmBombardedKtMineralPacketColonistsDefensesDestroy = 0x00d9,
    idmAnnihilatedMineralPacketColonistsKilled = 0x00da,
    idmDidntGetAttemptedTransferMineralPacketAnother = 0x00db,
    idmDidntGetAnyAttemptedTransferMineralPacket = 0x00dc,
    idmUnableTransferKtKtRequest = 0x00dd,
    idmAttemptedUseStargateStargateExistsThere = 0x00de,
    idmOneShipsDestroyedWhenEnginesReactedTrying = 0x00df,
    idmShipsDestroyedDueEngineStrain = 0x00e0,
    idmDestroyedMassiveReactorAccidentDueUnsafeOperatin = 0x00e1,
    idmAttemptedUseStargateReachCouldBecauseStargate = 0x00e2,
    idmAttemptedUseStargateReachCouldBecauseDestination = 0x00e3,
    idmAttemptedUseStargateReachCouldBecauseShips = 0x00e4,
    idmAttemptedUseStargateReachCouldBecauseStarbase = 0x00e5,
    idmAttemptedUseStargateCouldBecauseStarbaseOwned = 0x00e6,
    idmHeedlessDangerAttemptedUseStargateReachFleet = 0x00e7,
    idmUsedStargateReachLosingShipsTreacherousVoid = 0x00e8,
    idmUsedStargateReachLosingShipsUnforgivingVoid = 0x00e9,
    idmUsedStargateReachUnfortunatelyLosingShipsGreat = 0x00ea,
    idmUsedStargateReachLosingUnbelievableShipsJump = 0x00eb,
    idmHasUnloadedKtMineralsPreparationJumpingThrough = 0x00ec,
    idmHasUnloadedColonistsPreparationJumpingThroughSta = 0x00ed,
    idmHasUnloadedColonistsKtMineralsPreparationJumping = 0x00ee,
    idmWreckageDiscoveredBattleHasBoostedResearchResour = 0x00ef,
    idmWreckageBattleOccurredOrbitHasBoostedResearch = 0x00f0,
    idmFleetFoundWreckageBattleWhichHasBoosted = 0x00f1,
    idmUnableEngageEnginesDueBalkyEquipmentEngineers = 0x00f2,
    idmSRamScoopsHaveProducedMgFuel = 0x00f3,
    idmStarbaseHasSweptMinesMineField = 0x00f4,
    idmUnableCompleteMergeOrdersWaypointDestinationWasn = 0x00f5,
    idmUnableCompleteMergeOrdersDestinationFleetWasnt = 0x00f6,
    idmHasMerged = 0x00f7,
    idmWormholeHeadingHasVanishedOrdersHaveChanged = 0x00f8,
    idmColonyReportsBattleTookPlaceOrbitForces = 0x00f9,
    idmReportsBattleTookPlaceForcesInvolved = 0x00fa,
    idmColonistsHaveMadeGoodUseTimeIncreasing = 0x00fb,
    idmDoesHaveEnoughMineralsAvailableFlingAny = 0x00fc,
    idmFundamentalChangesEnvironmentHavePermanentlyAlte = 0x00fd,
    idmSurveyorsHaveDiscoveredPreviouslyUnknownDepositS = 0x00fe,
    idmPatrollingHasTargetedIntercept = 0x00ff,
    idmPopulationSuspectsUsurperProductivityOff20Growth = 0x0100,
    idmColonistsSuspectFactEmperorProductivityOff20 = 0x0101,
    idmHasRefusedMoveDoubtingAuthorityRulePress = 0x0102,
    idmFleetCaptainsHaveStagedStrikeDemandFree = 0x0103,
    idmHasDefectedRanksDueInabilityProjectLegitimate = 0x0104,
    idmCrewHasSoldOffCargoBlackMarket = 0x0105,
    idmFreedomFightersHaveAttackedDestroyedMinesPress = 0x0106,
    idmFreedomFightersHaveStolenKtStockpilesPress = 0x0107,
    idmMysteryTraderHasRefusedGiveCaptainAudience = 0x0108,
    idmHasAbsorbedMysteryTraderTraderHasGiven = 0x0109,
    idmHasAbsorbedMysteryTraderReturnTraderHas = 0x010a,
    idmHasAbsorbedMysteryTraderHaveGivenPlans = 0x010b,
    idmHasAbsorbedMysteryTraderReturnHaveGiven = 0x010c,
    idmHasAbsorbedMysteryTraderHoweverTraderUnable = 0x010d,
    idmHasAbsorbedMysteryTraderHoweverTraderUnable2 = 0x010e,
    idmHasAbsorbedMysteryTraderReturnHaveGiven2 = 0x010f,
    idmMysteryTraderHeadingHasVanishedOrdersHave = 0x0110,
    idmMineFieldHeadingHasVanishedOrdersHave = 0x0111,
    idmDoesHaveEnoughMineralsAvailableContinueAuto = 0x0112,
    idmBattleTookPlaceAgainstDestroyedEnemyForces = 0x0113,
    idmBattleTookPlaceAgainstForcesDestroyed = 0x0114,
    idmBattleTookPlaceAgainstNeitherNorEnemys = 0x0115,
    idmBattleTookPlaceAgainstNeitherForcesNor2 = 0x0116,
    idmRaceDefinitionHasTamperedStatisticsHaveAltered = 0x0117,
    idmMysteryTraderEyesCaptainSuspiciouslySuggestsHe = 0x0118,
    idmHasStolen = 0x0119,
    idmObsolete = 0x011a,
    idmStrongFundamentalForcesHaveRebirthed = 0x011b,
    idmAttemptedExecuteTransferOrdersInvolvingEitherFue = 0x011c,
    idmAttemptedExecuteTransferOrdersInvolvingFuelPlane = 0x011d,
    idmHadOrdersTransferCargoFutilePursuit = 0x011e,
    idmAttemptedLoadPlanetDontControlOrderHas = 0x011f,
    idmAttemptedLoadFleetDontControlOrderHas = 0x0120,
    idmAttemptedSetAmountBoardUnfortunatelyCouldntProvi = 0x0121,
    idmAttemptedSetNumberBoardUnfortunatelyCouldntProvi = 0x0122,
    idmAttemptedLoadDeepSpaceAttemptUnsuccessful = 0x0123,
    idmAttemptedShanghaiColonistsAttemptUnsuccessful = 0x0124,
    idmAttemptedStealMgFuelAttemptUnsuccessful = 0x0125,
    idmFailedLoadFuel = 0x0126,
    idmHasRerouted = 0x0127,
    idmHasReroutedUnfortuentlyDoesHaveEnoughFuel = 0x0128,
    idmHasOrdersBuildMineralPacketEitherDoesnt = 0x0129,
    idmHasOrdersBuildPlanetaryInstallationsBeyondMaximu = 0x012a,
    idmMysteriousTradingVesselBroadcastingProposalHasDe = 0x012b,
    idmHasImprovedValue = 0x012c,
    idmCurrentlyUnableImproveValueBeyond = 0x012d,
    idmHasRetroBombedUndoingTerraforming = 0x012e,
    idmHasOrdersTerraformBeyondMaximumAllowedOrders = 0x012f,
    idmMysteryTraderHasUnexplicablyChangedHisCourse = 0x0130,
    idmMineralPacketHasPermanentlyDefault = 0x0131,
    idmMineralPacketHasPermanentlyDefault2 = 0x0132,
    idmMineralPacketHas = 0x0133,
    idmMineralPacketHas2 = 0x0134,
    idmHasTriedBeamColonistsPlanetsStarbaseWould = 0x0135,
    idmScientistsHaveCompletedResearchTechLevelPrimary = 0x0136,
    idmHasExecutedOrdersFollowFleetAwaitsFurther = 0x0137,
    idmHadOrdersFollowFleetWhichDidntMove = 0x0138,
    idmStarbaseBuiltNewSDueLack27b = 0x0139,
    idmExaminationWreckageBattleUncoveredPlansNewPart = 0x013a,
    idmExaminationWreckageBattleUncoveredPlansNewShip = 0x013b,
    idmHasDismantledKtMineralsStarbaseOrbitingProcess = 0x013c,
    idmHasDismantledKtMineralsStarbaseOrbitingProcess2 = 0x013d,
    idmHasDismantledKtMineralsWhichHaveDeposited3 = 0x013e,
    idmHasDismantledKtMineralsStarbaseOrbitingHas = 0x013f,
    idmHasDismantledKtMineralsWhichHaveDeposited4 = 0x0140,
    idmHasDismantledKtMineralsStarbaseOrbiting2 = 0x0141,
    idmHasDismantledKtMineralsWhichHaveDeposited5 = 0x0142,
    idmHasDismantledKtMineralsStarbaseOrbitingUltimate2 = 0x0143,
    idmBattleTookPlaceDestroyedKillingColonistsBargain = 0x0144,
    idmRecentBreakthroughHasAlsoTaughtHowBuild = 0x0145,
    idmBombardedPacketContainingKtMineralsHoweverPacket = 0x0146,
    idmAttemptedReachViaStargateCouldBecauseStargate = 0x0147,
    idmCouldntGiveAwayBecausePlayerDead = 0x0148,
    idmCouldntGiveAwayBecauseThereColonistsBoard = 0x0149,
    idmCouldntGiveAwayBecauseDidntHaveAdministrative = 0x014a,
    idmAttemptedGiveFleetDontHaveEnoughExcess = 0x014b,
    idmSnubAttemptedGiftRefuseFleet = 0x014c,
    idmHasSuccessfullyGiven = 0x014d,
    idmHaveGiven = 0x014e,
    idmHasAbsorbedMysteryTraderReturnHaveGiven3 = 0x014f,
    idmHasAbsorbedMysteryTraderReturnTraderTried = 0x0150,
    idmMassPacketAppearsCollisionCourseWhichCurrently = 0x0151,
    idmStarbaseScheduledCompleteRemainingProductionItem = 0x0152,
    idmHaveReceivedOneBattleRecordingYear = 0x0153,
    idmHaveReceivedBattleRecordingsYear = 0x0154,
    idmAllowedTransferColonistsAnotherPlayer = 0x0155,
    idmHasAutoTerraformedValue = 0x0156,
    idmRecentBreakthroughHasAlsoTaughtHowBuild2 = 0x0157,
    idmBreedingActivitiesHaveOverflowedLivingSpaceColon = 0x0158,
    idmIntelligenceGatheringActivitiesCombinedSynergist = 0x0159,
    idmHasDegradedValue = 0x015a,
    idmCurrentlyUnableDegradeValueBeyond = 0x015b,
    idmEngineersHaveManagedImproveUnderlying1 = 0x015c,
    idmHaveInfoNewPlanetIfColonizeCan = 0x015d,
    idmUnableUseStargateBecauseHadColonistsBoard = 0x015e,
    idmHasAnnihilatedMineField3 = 0x015f,
    idmHasDamagedDetonatingMineFieldFleetHas = 0x0160,
    idmHasTakenDamageDetonatingMineFieldFleet = 0x0161,
    idmHasAnnihilatedMineField4 = 0x0162,
    idmHasDamagedDetonatingMineFieldMinesHave = 0x0163,
    idmHasDamagedDetonatingMineFieldMinesHave2 = 0x0164,
    idmHasTriedBeamColonistsDeepSpaceOrder = 0x0165,
    idmFleetsHaveBombedKillingColonists = 0x0166,
    idmFleetsHaveBombedDestroyingOneInstallation = 0x0167,
    idmFleetsHaveBombedDestroyingDefensesFactoriesMines = 0x0168,
    idmFleetsHaveBombedKillingColonistsDestroyingOne = 0x0169,
    idmFleetsHaveBombedKillingColonistsDestroyingDefens = 0x016a,
    idmFleetsHaveBombedKillingColonistsPlanetaryDefense = 0x016b,
    idmFleetsHaveBombedDestroyingOneInstallationPlaneta = 0x016c,
    idmFleetsHaveBombedDestroyingFactoriesMinesPlanetar = 0x016d,
    idmFleetsHaveBombedKillingColonistsDestroyingOne2 = 0x016e,
    idmFleetsHaveBombedKillingColonistsDestroyingDefens2 = 0x016f,
    idmFleetsHaveBombedKillingColonists2 = 0x0170,
    idmFleetsHaveBombedDestroyingOneInstallations = 0x0171,
    idmFleetsHaveBombedDestroyingDefensesFactoriesMines2 = 0x0172,
    idmFleetsHaveBombedKillingColonistsDestroyingOne3 = 0x0173,
    idmFleetsHaveBombedKillingColonistsDestroyingDefens3 = 0x0174,
    idmFleetsHaveBombedKillingColonistsPlanetaryDefense2 = 0x0175,
    idmFleetsHaveBombedDestroyingOneInstallationsPlanet = 0x0176,
    idmFleetsHaveBombedDestroyingDefensesFactoriesMines3 = 0x0177,
    idmFleetsHaveBombedKillingColonistsDestroyingOne4 = 0x0178,
    idmFleetsHaveBombedKillingColonistsDestroyingDefens4 = 0x0179,
    idmFleetsHaveRetroBombedUndoingTerraforming = 0x017a,
    idmFleetsHaveRetroBombedUndoingTerraforming2 = 0x017b,
    idmFleetsHaveBombedKillingOffEnemyColonists = 0x017c,
    idmFleetsHaveBombedKillingColonists3 = 0x017d,
    idmFailedLayMinesYearDueTechnicalDifficulties = 0x017e,
    idmFailedFlingMineralPacketDueTechnicalDifficulties = 0x017f,
    idmDueExcessiveFleetManeuveringBattleAreaFleets = 0x0180,
    idmBombardedKtMineralPacketFortunatelyOneHome = 0x0181,
    idmHackedRaceDiscoveredRaceStatisticsHaveAltered = 0x0182,
};
typedef uint16_t MessageId;
// TutorId is a tutorial text fragment, eight to a page; tutor.idt holds a
// page's first fragment and tutor.idtBold the highlighted instruction.
enum TutorId {
    idtWelcomeStarsTutorialWillGuideThrough36 = 0,
    idtHomePlanetCoupleScoutsDestroyerFreighterColony = 1,
    idtThereFiveMessagesMessagesPaneEachYear = 2,
    idtAboutPlanetsFleetsAboutEventsKnownPlayers = 3,
    idtYearMessagesPlayingTipsNoneThemRequire = 4,
    idtReadMessages = 5,
    idtCanClickButtonUseArrowKey = 6,
    idt0007Blank = 7,
    idtExamineTilesCommandPaneUpperLeftPortion = 8,
    idtControlsTilesGiveFullInformationCommandPlanet = 9,
    idtFleetsOrbitTileShowsFuelCargoBoard = 10,
    idtPressTilesGotoButtonCommandArmedProbe = 11,
    idtPaneGivesInformationCommandFleet = 12,
    idtLetsSendScoutOffExploringHasAutomatically = 13,
    idtScannerPaneShowsMapUniverse = 14,
    idtHoldShiftKeyClickLeftMouseButton = 15,
    idtAccordingFleetWaypointsTileWillTake2 = 16,
    idtLongRangeScout2HasSixTimes = 17,
    idtHitNKeyLookFleet = 18,
    idtLongRangeScout2UnarmedWeLikely = 19,
    idtTowardsPlanetsAboveRight = 20,
    idtHoldShiftKeyLeftClickPlanet90210 = 21,
    idt0022Blank = 22,
    idt0023Blank = 23,
    idtLetsMoveOurFleet = 24,
    idtTimePressButtonTileShowingLongRange = 25,
    idtSantaMaria3ColonyFleetWeDont = 26,
    idtPress = 27,
    idtTeamster4FreighterWeDontHaveAnything = 28,
    idtPress2 = 29,
    idtStalwartDefender5DestroyerWillUsefulScout = 30,
    idtHoldShiftKeySelectAlexander = 31,
    idtHitNKey = 32,
    idtCottonPicker6RemoteMinerWellSend = 33,
    idtHitNKey2 = 34,
    idtBackArmedProbe1ThatsFleetsRight = 35,
    idtOtherThingWeShouldDoYearPick = 36,
    idtChooseResearchCommandsMenu = 37,
    idtChangeFieldStudyWeaponsPressDone = 38,
    idtThatsTurnHitF9GenerateYear = 39,
    idtReadMessageMessagesPaneWeveGotPlenty = 40,
    idtBuildingFactories = 41,
    idtPressChangeButtonProductionTile = 42,
    idtSelectFactoryLeftHandListboxHoldShift = 43,
    idtShiftKeyCausesAddButtonAdd10 = 44,
    idtMessageYearScannerPaneShowsFleetsHave = 45,
    idtYetArrivedSoThereNothingDoYear = 46,
    idtHitF9KeyGenerateYear = 47,
    idtReadFirstMessagePressGotoMessagesPane = 48,
    idtLetsGiveArmedProbe1BunchPlaces = 49,
    idtHoldShiftKeyLeftClickHiho = 50,
    idtVacancy = 51,
    idtSlime = 52,
    idtWallaby = 53,
    idtOxygen = 54,
    idtReadMessagePressGotoCommandLongRange = 55,
    idtWeWantSendFleetExploreAreaAbove = 56,
    idtHoldShiftKeySelectDwarte = 57,
    idtMobius = 58,
    idtCastle = 59,
    idtMoholdi = 60,
    idtReadMessage = 61,
    idtAnd = 62,
    idtGotoStalwartDefender5 = 63,
    idtHoldShiftKeySelectShaggyDog = 64,
    idtSeaSquared = 65,
    idtRedStorm = 66,
    idtBloop = 67,
    idtKalamazoo = 68,
    idtReadTwoMessages = 69,
    idtAnd2 = 70,
    idtPressGotoDisplayStatsPruneSummaryPane = 71,
    idtTopGraphSummaryPaneShowsPruneHas = 72,
    idtCurrentTechnology = 73,
    idtDiamondsBottomGraphShowMineralConcentrationsPrun = 74,
    idtThereMinePlanet = 75,
    idtClickRightMouseButtonStoveTopSelect = 76,
    idtShiftClickPrune = 77,
    idtLookWaypointTaskTile = 78,
    idtClickDropdownChangeTaskRemoteMining = 79,
    idtMoveMessageGotoAlexander = 80,
    idtDiamondsBottomGraphShowAlexandersMineralConcentr = 81,
    idtSendingCottonPicker6PruneRightThing = 82,
    idtClickVariousPlacesSummaryPaneGetPopup = 83,
    idtReadMessage2 = 84,
    idtAnd3 = 85,
    idtGotoPlanet90210 = 86,
    idt0087Blank = 87,
    idtSince90210FinePlanetHighMineralConcentrations = 88,
    idtRightClickStoveTopSelectSantaMaria = 89,
    idtClickXferButtonTileLabeledOrbitingStove = 90,
    idtClickDragColonistsGaugeFillingHold25kt = 91,
    idtShiftClick90210 = 92,
    idtSelectColonizeDropdownWaypointTaskTile = 93,
    idtThatsYear = 94,
    idtHitF9GenerateYear = 95,
    idtFirstMessageQuiteCommonWeDontNeed = 96,
    idtFilterClickingBlueCheckMarkUpperLeft = 97,
    idtMoveMessageGotoStoveTop = 98,
    idtWeDoWantHaveKeepAddingFactories = 99,
    idtBuild30FactoriesEveryYear = 100,
    idtPressChangeButtonProductionTile2 = 101,
    idtSelectFactoriesAutoBuildLeftHandListbox = 102,
    idt0103Blank = 103,
    idtReadTwoMessagesGoto90210 = 104,
    idtProductionQueueHereEmptyWeOughtDo = 105,
    idtHitQKey = 106,
    idtDoubleClickFactory3TimesMine3 = 107,
    idtWillTake10YearsBuild3Factories = 108,
    idtRightClickStoveTopSelectTeamster4 = 109,
    idtClickXferButtonCommandPane = 110,
    idtFillHoldColonistsHitOk = 111,
    idtShiftClick902102 = 112,
    idtChangeWaypointTaskTransport = 113,
    idtRightClickBlueDiamondWaypointTaskTile = 114,
    idtReadMessageGotoHiho = 115,
    idtNoticeArmedProbe1WhichBlueTriangle = 116,
    idtDarkYellowCircleSurroundingArmedProbe1 = 117,
    idtDoubleClickArmedProbe1 = 118,
    idt0119Blank = 119,
    idtArmedProbe1DoesntNeedGoWay = 120,
    idtClickHiho = 121,
    idtAnd4 = 122,
    idtHitDeleteKey = 123,
    idtArmedProbe1WillGoVacancyWithout = 124,
    idtThatsYear2 = 125,
    idtChangePaceInsteadHittingF9 = 126,
    idtSelectGenerateTurnMenu = 127,
    idtReadFirstMessageGotoShaggyDog = 128,
    idtWellAddColonizerQueueBitFirstLets = 129,
    idtDoubleClickStalwartDefender5JustAbove = 130,
    idtSelectWaypointShaggyDog = 131,
    idtPressDeleteKey = 132,
    idtReadMessageGotoDwarte = 133,
    idtWhatUnpleasantPlace = 134,
    idtDoubleClickStoveTop = 135,
    idtHitChangeButtonProductionTileOpenStove = 136,
    idtDoubleClickSantaMariaLeftHandListbox = 137,
    idtNoticeFactoryTopQueueSantaMariaDisplayed = 138,
    idtProductionTileMeansWillFinishedYear = 139,
    idtBlueItemsProductionQueueWillMakePartial = 140,
    idtRedItemsMayNeverFinish = 141,
    idtGoAhead = 142,
    idtGenerateWhenReady = 143,
    idtReadFirstMessageGotoNewSantaMaria = 144,
    idtHasReplacedOldSantaMariaFleet3 = 145,
    idtClickCargoGaugeFuelCargoTile = 146,
    idtDoesSameThingHittingXferButton = 147,
    idtFillHoldFullColonistsHitOk = 148,
    idtWhereWeWantedSendColonizer = 149,
    idtClickButtonToolbarShowPlanetsHowHabitable = 150,
    idtShiftClickBigGreenShaggyDogBelow = 151,
    idtSetWaypointTaskColonize = 152,
    idtSwitchScannerBackNormalViewClickingLeftmost = 153,
    idtRead2MessagesGotoTeamster4 = 154,
    idtShiftClickStoveTopSendHome = 155,
    idtSelect90210PressingGotoButtonTileLabeled = 156,
    idtNoticeFactoriesWillDone2YearsInstead = 157,
    idtReadMessageDeleteArmedProbe1sWaypoint = 158,
    idtThatsYearGenerateWhenReady = 159,
    idtReadFirstMessageGotoTeamster4 = 160,
    idtCottonPicker6HasRemoteMiningPrune = 161,
    idtMineralsBackStoveTop = 162,
    idtShiftClickPrune2 = 163,
    idtSetWaypointTaskTransport = 164,
    idtRightClickBlueDiamondSelectQuikloadZip = 165,
    idtShiftClickBackStoveTop = 166,
    idt0167Blank = 167,
    idtNoticeWaypointTaskHasCopiedPreviousWaypoint = 168,
    idtPruneStovetop = 169,
    idtRightClickBlueDiamondSelectQuikdropZip = 170,
    idtClickRepeatOrdersCheckboxFleetWaypointsTile = 171,
    idtTeamster4WillContinueHaulMineralsPrune = 172,
    idtSelectStoveTop = 173,
    idtAddSantaMariaProductionQueue = 174,
    idtGoAheadGenerateIDare = 175,
    idtReadFirstMessageGotoNewSantaMaria2 = 176,
    idtClickToolbarButtonPutScannerPlanetValue = 177,
    idtRedStormClearlyBestPlanetAvailable = 178,
    idtGiveSantaMaria7ColonizeTaskRed = 179,
    idtThereLeastTwoColonizablePlanetsSeaSquared = 180,
    idtAddThreeSantaMariasStoveTopsProduction = 181,
    idtPressLeftmostToolbarButtonPutScannerBack = 182,
    idt0183Blank = 183,
    idtReadMessage3 = 184,
    idtWellKeepGettingMineBuildingMessagesForever = 185,
    idtFilterThemClickingBlueCheckMarkMessages = 186,
    idtGoMessage = 187,
    idtGoto90210OpenProductionQueue = 188,
    idtShiftDoubleClickFactoriesAutoBuildMines = 189,
    idtReadRestMessages = 190,
    idt0191Blank = 191,
    idtClickRedTriangleBetweenSlimeVacancy = 192,
    idtEnemyScoutShipRightClickingFleetImage = 193,
    idtArmedAccordingProjectedPathScannerHeadedVacancy = 194,
    idtArmedProbe1WontAbleCatchWe = 195,
    idtAddTwoArmedProbesStoveTopsQueue = 196,
    idtThatsYear3 = 197,
    idtGenerateWhenReady2 = 198,
    idt0199Blank = 199,
    idtReadFirstMessageGotoNewColonyShips = 200,
    idtWeHavePlacesSendTwoThemSo = 201,
    idtHitSplitButtonFleetCompositionTile = 202,
    idtMoveOneSantaMariasFleet10Hit = 203,
    idtLeavesTwoSantaMariasFleetCurrentlyCommanding = 204,
    idtLoadFleetColonistsGiveColonizeTaskSlime = 205,
    idtWeDontWantBothColonizersGoSlime = 206,
    idt0207Blank = 207,
    idtNoticeFleetHasOneSantaMariaOther = 208,
    idtClickWaypointSlimeDragSeaSquared = 209,
    idtNoticeThereWaypointLinesGoingStoveTop = 210,
    idtSantaMaria8SantaMaria11Have = 211,
    idtReadMessageGotoNewArmedScouts = 212,
    idtIfHeadDirectlyVacancyEnemyScoutWill = 213,
    idtLetsTryHeadThemOffPass = 214,
    idtShiftClickHiho = 215,
    idtReadMessageFilter = 216,
    idtReadLastMessageGotoWallaby = 217,
    idtWallabyOwnedBerserkersLightlyPopulatedNearlyTerr = 218,
    idtClickGreenRadiationBarSummaryPaneRead = 219,
    idtReason7SoundsFamiliar = 220,
    idtHitF5OpenResearchDialog = 221,
    idt0222Blank = 222,
    idt0223Blank = 223,
    idtRightRadiationTerraform7OneExpectedBenefits = 224,
    idtBenefitsListedBlueWillTakeOneAdditional = 225,
    idtClickWordRadiationDialogSeeRequirements = 226,
    idtCurrentRateResearchWeaponsTech5Will = 227,
    idtIncreaseResourcesBudgetedResearch30HitDone = 228,
    idtWellSendTroopShipWallabySoonThats = 229,
    idtGenerateWhenReady3 = 230,
    idt0231Blank = 231,
    idtReadFirstMessageGotoResearchDialog = 232,
    idtNoticeRadiation7ListedGreenIndicatingWill = 233,
    idtCurrentLevelStudyWeaponsEstimatedTimeCompletion = 234,
    idtNoticeFieldResearchCurrentlySetSameField = 235,
    idtChangeFieldResearchConstructionHitDone = 236,
    idtSoonReachTech5WeaponsResearchFocus = 237,
    idtReadMessageFilter2 = 238,
    idt0239Blank = 239,
    idtReadMessageGotoOxygen = 240,
    idtRightClickStoveTopSelectSantaMaria2 = 241,
    idtLoadColonists = 242,
    idtSendColonizeOxygen = 243,
    idtSinceWeveSeenOxygenAlreadyWeCan = 244,
    idtSelectArmedProbe1DragWaypointOxygen = 245,
    idtThatsYear4 = 246,
    idtGenerateWhenReady4 = 247,
    idtFilterMessageAboutDismantlingColonizer = 248,
    idtReadMessageGotoShaggyDog = 249,
    idtOpenShaggyDogsProductionQueue = 250,
    idtAdd3FactoriesAutoBuild3Mines = 251,
    idtWeShouldAlsoSetDefaultQueueSo = 252,
    idtColonizersHaveEnRoute = 253,
    idtOpenProductionQueue = 254,
    idtRightClickBlueDiamondSelectCustomize = 255,
    idtHitImportButtonCopyShaggyDogsQueue = 256,
    idtOkProductionDialog = 257,
    idtEveryNewPlanetColonizeWillAutomaticallyGet = 258,
    idtReadMessageGotoBloopLooksLikeNice = 259,
    idtAddSantaMariaStoveTopsQueue = 260,
    idtWeWantTakeWallabyTeamster4Busy = 261,
    idtAddNewTeamsterStoveTopsQueue = 262,
    idtGenerateWhenReady5 = 263,
    idtReadFirstMessageGotoArmedProbe1 = 264,
    idtShiftClickHacker = 265,
    idtReadMessageGotoLongRangeScout2 = 266,
    idtGuyIsntWorthMuchAnymoreHesToo = 267,
    idtShiftClickStoveTopChangeWaypointTask = 268,
    idtReadMessageSendStalwartDefender5Stove = 269,
    idtWillAutomaticallyRefueledStarbaseWhenArrives = 270,
    idtReadMessageGotoArmedProbe9Shift = 271,
    idtReadMessageGotoNewSantaMaria = 272,
    idtUseToolbarScannerSummaryPaneFigureWhich = 273,
    idtGiveSantaMaria3OrdersColonizeDont = 274,
    idtReadMessageGotoTeamster12 = 275,
    idtLoadColonistsAssignWaypointWallaby = 276,
    idtChangeWaypointTaskTransport2 = 277,
    idtSetSecondDropdownWaypointTaskTileColonists = 278,
    idtThirdUnload = 279,
    idtReadMessageAdd70MinesTopStove = 280,
    idtReadMessageGotoResearchDialog = 281,
    idtLeaveFieldStudyConstructionChangeFieldResearch = 282,
    idtReadMessageHitGotoOpenTechnologyBrowser = 283,
    idtWhenDoneReadingAboutBetaTorpedoClose = 284,
    idtReadTwoMessagesLookingTechBrowserIf = 285,
    idtReadMessageGotoArmedProbe = 286,
    idt0287Blank = 287,
    idtGollyNailedOneThemNoticeButtonNormally = 288,
    idtPressViewOpenBattleVcr = 289,
    idtUseVcrControlsWatchPlaybackBattleHit = 290,
    idtReadRestMessages2 = 291,
    idtStoveTopBusyBuildingMinesYearSo = 292,
    idtGeneralWalkingThroughMessagesHandlingOnesSeem = 293,
    idtWorkAnyParticularYear = 294,
    idtGenerateWhenReady6 = 295,
    idtReadFirstMessageGotoButtonDisabledI = 296,
    idtReadMessageGotoArmedProbe9 = 297,
    idtLooksLikeNailedAnotherOneThereYellow = 298,
    idtSalvageLeftBattle = 299,
    idtRightClickArmedProbe9SelectSalvage = 300,
    idtSummaryPaneShowsSalvageConsistsFewKt = 301,
    idtFreighterAfterAnyway = 302,
    idtSelectViewFindTypeTeamster4Hit = 303,
    idtIfCantFindWhereYellowSelectionArrow = 304,
    idtTeamster4StoveTopWillBackPrune = 305,
    idtReadMessage4 = 306,
    idtWellExplainsWhatHappenedArmedProbe1 = 307,
    idtWatchSadBattleIfWantMoveMessage = 308,
    idtThatsLikeOneMayWantWatch = 309,
    idtReadMessageGotoRedStorm = 310,
    idt0311Blank = 311,
    idtOtherBitShortColonistsRedStormDoing = 312,
    idtReadMessageGotoSlime = 313,
    idtLookSummaryPaneSlimeOutsideHabitableRange = 314,
    idtWhatWeNeedDoAddTerraformingProduction = 315,
    idtOpenSlimesProductionQueueAddTwoTerraform = 316,
    idtLookProductionTileISuspectNeedFew = 317,
    idtAddTwoTeamstersStoveTopsProductionQueue = 318,
    idtGenerateWhenReady7 = 319,
    idtReadFirstMessageLoadTeamster1Colonists = 320,
    idtSendSlimeOrdersUnloadThem = 321,
    idtReadMessage5 = 322,
    idtLooksLikeLoadReinforcementsTeamster1Carrying = 323,
    idtReadMessageOpenResearchDialogChangeField = 324,
    idtReadMessageCheckRoboMinerTechBrowser = 325,
    idtWeCouldUseMinersHelpStripPrune = 326,
    idtHitF4OpenShipDesigner = 327,
    idtSelectAvailableHullTypes = 328,
    idtChooseMiniMinerDropdown = 329,
    idtHitCopySelectedDesign = 330,
    idtLeftSideDisplaysListEveryPartCapable = 331,
    idtDragLongHump6EnginePartsList = 332,
    idtDragRhinoScannerScannerElectMechSlot = 333,
    idtSelectMiningRobotsPartsCategoryDropdown = 334,
    idtDragRoboMinerEachMiningSlots = 335,
    idtShipDesignNameImageJustFine = 336,
    idtHitOkFinishEditingDesign = 337,
    idtDoneCloseDesigner = 338,
    idtAddOneNewMiniMinersStoveTops = 339,
    idtWillTake6YearsFinishShipJust = 340,
    idtOpenStoveTopsQueue = 341,
    idtSelectMineLeftHandListboxTopQueue = 342,
    idt0343Blank = 343,
    idtClickEachItemsProductionTileMiniMiner = 344,
    idtReadFinalMessage = 345,
    idtLowEnoughMineralsPointDesigningAdditionalShips = 346,
    idtWeLeftArmedProbe9HangingNear = 347,
    idtSendArmedProbe9BackStoveTop = 348,
    idtThatsEnoughYear = 349,
    idtGenerateNewYear = 350,
    idt0351Blank = 351,
    idtReadFirstMessageGotoSeaSquared = 352,
    idtOtherNeedingPeopleDoingJustFine = 353,
    idtReadFinalMessageGotoOxygen = 354,
    idtPlanetSlightlyHabitableRangeWeShouldAdd = 355,
    idtAddMinTerraform2OxygensQueueRight = 356,
    idtThatsYearAutomationMakesYearsFlyFaster = 357,
    idtGenerateWhenReady8 = 358,
    idt0359Blank = 359,
    idtReadFirstMessageSendArmedProbe9 = 360,
    idtReadMessageGotoNewTeamsterFillColonists = 361,
    idtWeWouldLikeSendColonistsWhereNeeded = 362,
    idtChoosePlanetsReportMenu = 363,
    idtClickTitleValueColumnSortValue = 364,
    idtPlanetLargestNegativeValueWallabyColonistsWill = 365,
    idtHitEscKeyClosePlanetSummaryReport = 366,
    idtSendTeamster7WallabyUnloadColonists = 367,
    idtReadTwoMessagesOpenResearchDialog = 368,
    idtExpectedResearchBenefitsEitherBlueBlackWhich = 369,
    idtWeLearnAnythingFieldResearchAlreadySet = 370,
    idtCloseDialog = 371,
    idtReadRemainingMessagesSendTeamster12Back = 372,
    idtMineDispenser50SoundedInterestingIsntGood = 373,
    idtGenerateNewYear2 = 374,
    idt0375Blank = 375,
    idtReadFirstMessageSendStalwartDefender5 = 376,
    idtReadMessageGotoNewMiniMiner = 377,
    idtShiftClickPruneSetWaypointTaskMerge = 378,
    idtNoticeWaypointPruneHasChangedCottonPicker = 379,
    idtReadMessageOpenStoveTopsProductionQueue = 380,
    idtIncreaseNumberAutoBuildFactories60Add = 381,
    idtReadTwoMessagesChangeFieldResearchConstruction = 382,
    idtReadFinalMessageGenerate = 383,
    idtReadMessages2 = 384,
    idtSendTeamster1BackStoveTop = 385,
    idtSureEasyTurn = 386,
    idtGenerateNewYear3 = 387,
    idt0388Blank = 388,
    idt0389Blank = 389,
    idt0390Blank = 390,
    idt0391Blank = 391,
    idtReadFirstMessageAddTeamsterStoveTops = 392,
    idtReadMessageGoto90210 = 393,
    idtWellWeCouldPlayProductionQueueLooks = 394,
    idtReadTwoMessagesOpenResearchDialog2 = 395,
    idtClickDifferentItemsListedExpectedBenefitsBox = 396,
    idtStargateSoundsLikeFunFrigateAlsoLooks = 397,
    idtCloseDialogWithoutMakingAnyChanges = 398,
    idtReadRemainingMessagesGenerateYear = 399,
    idtReadFirstMessageLoadTeamster12Colonists = 400,
    idtAddWaypointWallabyUnloadColonists = 401,
    idtShiftClickBackStoveTopChangeTask = 402,
    idtClickRepeatOrdersCheckboxFleetWaypointsTile2 = 403,
    idtEstFuelUsageClaimsWeWillNeed = 404,
    idtFleetWillLighterWillNeedMuchFuel = 405,
    idtReadMessageGotoNewTeamster = 406,
    idtLoadColonists2 = 407,
    idtAddWaypointOxygenUnloadColonists = 408,
    idtShiftClickBackStoveTopChangeTask2 = 409,
    idtClickRepeatOrdersCheckboxFleetWaypointsTile3 = 410,
    idtBothFreightersWillContinueMovingColonistsAway = 411,
    idtReadMessageAddMaxTerraformAutoBuild = 412,
    idtRead3MessagesSendTeamster7Back = 413,
    idtReadLastMessageGenerateYear = 414,
    idt0415Blank = 415,
    idtReadFirstTwoMessagesSendArmedProbe = 416,
    idtRead3MessagesChangeFieldResearchWeapons = 417,
    idtReadFinalMessageHitF4OpenShip = 418,
    idtSelectStarbasesCopySelectedDesign = 419,
    idtSelectOrbitalPartsCategoryDragStargate100 = 420,
    idtChangeDesignNameGaterClickRightArrow = 421,
    idtAddGaterStoveTopsQueue = 422,
    idtGenerate = 423,
    idtReadFirstMessageLoadTeamster1Colonists2 = 424,
    idtSendUnloadColonistsWallaby = 425,
    idtWeDoFrequentlyEnoughWeShouldSimplify = 426,
    idtRightClickBlueDiamondWaypointTaskTile2 = 427,
    idtHitImportNameOrderDropcolOkBoth = 428,
    idtFutureWeCanSetFleetsTaskUsing = 429,
    idtShiftClickStoveTopChangeTransportOption = 430,
    idtClickRepeatOrders = 431,
    idtReadRestMessages3 = 432,
    idtYouveUpgradedStarbaseStoveTopWeDont = 433,
    idtHitF3OpenPlanetSummaryReport = 434,
    idtFindMinConcColumnRightClickReverse = 435,
    idtOxygenSeaSquaredRedStormWallabyHave = 436,
    idtWallabyHasHighestPopulationClosestBerserkersPlan = 437,
    idtUnfortunatelyWallabyStillHasNegativeGrowthRate = 438,
    idtGenerateWhenReady9 = 439,
    idtReadFirstMessageGotoTeamster42 = 440,
    idtSeemsMiniMinerWeAddedPruneHas = 441,
    idtWeCouldDecreaseSpeedEachLegTeamster = 442,
    idtRemoteMinersProducing = 443,
    idtMineralsWeCanCarryEachTripWe = 444,
    idtClickDragFuelGaugeOtherFleetsHere = 445,
    idt0446Blank = 446,
    idt0447Blank = 447,
    idtAddTeamsterStoveTopsQueue = 448,
    idtRead4MessagesChangeFieldResearchPropulsion = 449,
    idtReadRemainingMessagesOpenShipDesigner = 450,
    idtViewAvailableHullTypesSelectFrigateDropdown = 451,
    idtDragDaddyLongLegs7EngineSlot = 452,
    idtSelectMineLayersDropdownDrag3Mine = 453,
    idtChangeDesignNameMineLayerOkDesign = 454,
    idtAddMineLayerStoveTopsQueue = 455,
    idtClickRedTriangleWallaby = 456,
    idtBerserkerColonizerHeadedVacancyWarp6 = 457,
    idtRightClickWallabySelectStalwartDefender5 = 458,
    idtShiftClickEnemyFleet = 459,
    idtShouldSufficientYear = 460,
    idtGenerateWhenReady10 = 461,
    idt0462Blank = 462,
    idt0463Blank = 463,
    idtReadFirstMessageGotoStalwartDefender5 = 464,
    idtFleetDisplayedPurpleScannerMeansThereEnemy = 465,
    idtAlsoTellsUsAlthoughCaughtColonizerDidnt = 466,
    idtYoullSeeMessageAboutBattleLater = 467,
    idtReadMessageGotoTeamster7 = 468,
    idtOpenPlanetSummaryReportSortPopulation = 469,
    idtOxygenHasLowestPopulationYouveAlreadyGot = 470,
    idtHitEscCloseReport = 471,
    idtLoadTeamster7ColonistsSendSeaSquared = 472,
    idtSetWaypointTaskTransport2 = 473,
    idtRightClickBlueDiamondChooseDropcol = 474,
    idtReadTwoMessagesGotoNewTeamster = 475,
    idtFreighterWeBuiltMergeOneGoingBack = 476,
    idtSelectTeamster4ListboxOtherFleetsHere = 477,
    idtPressMergeButtonFleetCompositionTile = 478,
    idtClickTeamster3MergeFleetsDialogHit = 479,
    idtReadMessageSetMineLayer8sTask = 480,
    idtReadMessageAddMiniMinerStoveTops = 481,
    idtRead2MessagesWatchBattle = 482,
    idtBerserkersSantaMaria80DamagedCanKill = 483,
    idtRightClickBlueDiamondFleetWaypointsTile = 484,
    idtReadLastMessageDoubleClickArmedProbe = 485,
    idtDragWaypointLaTeDaSpeedBump = 486,
    idtShiftClickLeverGenerate = 487,
    idtReadFirstMessageSendStalwartDefender52 = 488,
    idtReadMessageGotoMiniMiner3Send = 489,
    idtReadRemainingMessages = 490,
    idtThats = 491,
    idtGenerateWill = 492,
    idt0493Blank = 493,
    idt0494Blank = 494,
    idt0495Blank = 495,
    idtReadFirstTwoMessages = 496,
    idtClickButtonToolbar = 497,
    idtNoticeThereLotGreenWorldsWeNeed = 498,
    idtHitF4OpenShipDesigner2 = 499,
    idtSelectSantaMariaDropdown = 500,
    idtHitEditSelectedDesign = 501,
    idtDragLongHump6EngineDesignParts = 502,
    idtOkDesignHitDoneCloseDialog = 503,
    idtAdd3ImprovedSantaMariasStoveTops = 504,
    idtReadMessageGotoSeaSquared = 505,
    idtSeaSquaredHasBuiltManyFactoriesMines = 506,
    idtAddMaxTerraformAutoBuild2End = 507,
    idtRead2MessagesChangeFieldResearchConstruction = 508,
    idtReadRestMessages4 = 509,
    idtAnd5 = 510,
    idtGenerateWhenReady11 = 511,
    idtReadFirstThreeMessages = 512,
    idtGoto3NewSantaMarias = 513,
    idtLoadThemColonistsSendThemColonizeLever = 514,
    idtHitSplitButtonFleetCompositionTile2 = 515,
    idtDragSantaMaria10sWaypointSpeedBump = 516,
    idtSantaMaria11sBloop = 517,
    idtReadRestMessages5 = 518,
    idt0519Blank = 519,
    idtTeamster12WillArriveStoveTopYear = 520,
    idtAdd3TeamstersStoveTopsQueue = 521,
    idtAugmentOurTroopLiftEffort = 522,
    idtClickEnemyShipNearWallaby = 523,
    idtBerserkersTryingColonizeVacancyFolksNeverLearn = 524,
    idtSelectStalwartDefender5DragDestinationEnemy = 525,
    idtYearWellHaveDesignNewDestroyerGo = 526,
    idtGenerateWhenYoureReady = 527,
    idtReadFirstMessageGotoStalwartDefender52 = 528,
    idtOnceWeveWoundedFinishedOffColonizer = 529,
    idtRightClickBlueDiamondFleetWaypointsTile2 = 530,
    idtTellsDestroyerFollowDestroyEnemyColonizer = 531,
    idtReadMessageGotoArmedProbe92 = 532,
    idtWellLeaveFleetHereGuardLeverSince = 533,
    idtReadMessageGotoNewFleet = 534,
    idtFillColonists = 535,
    idtWeWantMergeNewTeamstersOtherFleet = 536,
    idtSelectTeamster12PressMergeButtonFleet = 537,
    idtReadMessage6 = 538,
    idtWellDealStoveTopAfterWeFinish = 539,
    idtReadRestMessagesViewingBattleColonizerIf = 540,
    idtPromisedLastYearLetsDesignDestroyerTake = 541,
    idtHitF4OpenShipDesigner3 = 542,
    idt0543Blank = 543,
    idtWeWantPowerfulWeAlsoWantWeigh = 544,
    idtSelectAvailableHullTypesChooseDestroyerDropdown = 545,
    idtAddRadiatingHydroRamScoop2Carbonic = 546,
    idtAddFuelTankMechanicalSlotBattleComputer = 547,
    idtTotalMassDesign97kt = 548,
    idtClickRightArrowButtonBelowShipImage = 549,
    idtPut10DestroyersStoveTopsQueue = 550,
    idtGenerate2 = 551,
    idtReadFirstMessageGotoStalwartDefender53 = 552,
    idtOnce = 553,
    idtSendWallaby = 554,
    idtRead4MessagesGotoNewDestroyerArmada = 555,
    idtSendWreakHavocBerserkerStarbaseHacker = 556,
    idtReadRestMessages6 = 557,
    idtAnd6 = 558,
    idtGenerateTurn = 559,
    idtReadFirstMessageGotoTeamster43 = 560,
    idtWeNeedSlowFleetLegStoveTop = 561,
    idtClickStoveTopFleetWaypointsTileDecrease = 562,
    idtOurReturnTripWillTakeExtraYear = 563,
    idtRead4Messages = 564,
    idtSendNewDestroyerHackerWell = 565,
    idtAvoidRepeatWorkSendingEveryNewFleet = 566,
    idtSelectStoveTopControlClickHacker = 567,
    idtNoticeProductionTileNewShipsWillRouted = 568,
    idtRead3Messages = 569,
    idtOpenResearchDialogSetFieldResearchEnergy = 570,
    idtReadRestMessages7 = 571,
    idtGotoTeamster7 = 572,
    idtDoesntHaveEnoughFuelGetBackStove = 573,
    idtGiveTeamster7OrdersScrapFleet = 574,
    idt0575Blank = 575,
    idtLetsFinishOffBerserkersOnceBuildingBombing = 576,
    idtHitF4OpenShipDesigner4 = 577,
    idtSelectAvailableHullTypesChooseB17 = 578,
    idtAddRadiatingHydroRamScoopEngines = 579,
    idtHoldShiftKeyDrag4BlackCat = 580,
    idtOkDesignCloseShipDesigner = 581,
    idtAdd10B17BombersStoveTops = 582,
    idtGenerateWhenReady12 = 583,
    idtCongratulationsYouveDeclaredWinner = 584,
    idtTutorialWillContinueFewYearsGiveAdditional = 585,
    idtReadFirst3MessagesGotoNewB = 586,
    idtNoticeTheyveAlreadyRoutedHacker = 587,
    idtReadRestMessages8 = 588,
    idtEverythingElseAutomated = 589,
    idtGenerateWhenYoureReady2 = 590,
    idt0591Blank = 591,
    idtReadFirst4MessagesGotoWallaby = 592,
    idtTerraformingEffortHasFinallyPaidOffWed = 593,
    idtAdd100MinesWallabysQueue = 594,
    idtControlClickAddButtonAdd100Item = 595,
    idtReadRestMessages9 = 596,
    idtThereIsntAnythingPressingDoYearOur = 597,
    idtGenerateWill2 = 598,
    idt0599Blank = 599,
    idtReadFirst3MessagesGotoDestroyer13 = 600,
    idtNotice9DestroyersShownRedBarAbout = 601,
    idtClickDestroyerFleetCompositionTile = 602,
    idtIfRunningLeast800x600ModeWillSee = 603,
    idtRead8MessagesViewAssaultEnemyStarbase = 604,
    idtWellBerserkersShouldntBuildingAnyColonizersNotic = 605,
    idtReadRestMessagesGenerate = 606,
    idt0607Blank = 607,
    idtReadFirst6MessagesGotoStoveTop = 608,
    idtAddAnother10B17BombersProduction = 609,
    idtHoldingPatternWaitingOurBombersArriveHacker = 610,
    idtReadRestMessagesWatchBattles = 611,
    idtGenerateWhenReady13 = 612,
    idt0613Blank = 613,
    idt0614Blank = 614,
    idt0615Blank = 615,
    idtNothingMuchHappeningYearViewBattleHacker = 616,
    idtWillHaveDesignFasterShipUsingFaster = 617,
    idtFirstBombersArriveYear = 618,
    idtReadMessagesGenerateWhenReady = 619,
    idt0620Blank = 620,
    idt0621Blank = 621,
    idt0622Blank = 622,
    idt0623Blank = 623,
    idtReadThroughMessages = 624,
    idtN2B17BombersKilledFewEnemy = 625,
    idtNewArrivalsSeveralYearsShouldMakeDifference = 626,
    idtThereNumberThingsWeCouldDoOur = 627,
    idtGenerateWhenReady14 = 628,
    idt0629Blank = 629,
    idt0630Blank = 630,
    idt0631Blank = 631,
    idtCongratulationsHaveReachedEndTutorial = 632,
    idtHitF10ViewScoreNoticeBerserkersHave = 633,
    idtNeedFinishBombingBerserkerPlanetsBuildShip = 634,
    idtWillTrulyRuleGalaxy = 635,
    idtReadMessages3 = 636,
    idtWhenGenerateYoureOwn = 637,
    idt0638Blank = 638,
    idt0639Blank = 639,
};
typedef uint16_t TutorId;

enum GrStat {
    grStatFuel = 1,
    grStatCargo = 2,

};
typedef uint16_t GrStat;

// isbhull is a starbase hull's index in rghuldefSB, its HulDef id less
// ihuldefOrbitalFort.
enum isbhull {
    isbhullAuto = -1, // FCreateAiStarbase: choose by design slot
    isbhullOrbitalFort = 0,
    isbhullSpaceDock = 1,
    isbhullSpaceStation = 2,
    isbhullUltraStation = 3,
    isbhullDeathStar = 4,
    isbhullCount = 5,
};
typedef int16_t isbhull;

enum iengine {
    iengineSettlersDelight = 0,
    iengineQuickJump5 = 1,
    iengineFuelMizer = 2,
    iengineLongHump6 = 3,
    iengineDaddyLongLegs7 = 4,
    iengineAlphaDrive8 = 5,
    iengineTransGalacticDrive = 6,
    iengineInterspace10 = 7,
    iengineEnigmaPulsar = 8,
    iengineTransStar10 = 9,
    iengineRadiatingHydroRamScoop = 10,
    iengineSubGalacticFuelScoop = 11,
    iengineTransGalacticFuelScoop = 12,
    iengineTransGalacticSuperScoop = 13,
    iengineTransGalacticMizerScoop = 14,
    iengineGalaxyScoop = 15,
    iengineCount = 16,
};
typedef uint16_t iengine;

enum iarmor {
    iarmorTritanium = 0,
    iarmorCrobmnium = 1,
    iarmorCarbonicArmor = 2,
    iarmorStrobnium = 3,
    iarmorOrganicArmor = 4,
    iarmorKelarium = 5,
    iarmorFieldedKelarium = 6,
    iarmorDepletedNeutronium = 7,
    iarmorNeutronium = 8,
    iarmorMegaPolyShell = 9,
    iarmorValanium = 10,
    iarmorSuperlatanium = 11,
    iarmorCount = 12,
};
typedef uint16_t iarmor;

enum iscanner {
    iscannerBatScanner = 0,
    iscannerRhinoScanner = 1,
    iscannerMoleScanner = 2,
    iscannerDNAScanner = 3,
    iscannerPossumScanner = 4,
    iscannerPickPocketScanner = 5,
    iscannerChameleonScanner = 6,
    iscannerFerretScanner = 7,
    iscannerDolphinScanner = 8,
    iscannerGazelleScanner = 9,
    iscannerRNAScanner = 10,
    iscannerCheetahScanner = 11,
    iscannerElephantScanner = 12,
    iscannerEagleEyeScanner = 13,
    iscannerRobberBaronScanner = 14,
    iscannerPeerlessScanner = 15,
    iscannerCount = 16,
};
typedef uint16_t iscanner;

enum ishield {
    ishieldMoleSkinShield = 0,
    ishieldCowHideShield = 1,
    ishieldWolverineDiffuseShield = 2,
    ishieldCrobySharmor = 3,
    ishieldShadowShield = 4,
    ishieldBearNeutrinoBarrier = 5,
    ishieldLangstonShell = 6,
    ishieldGorillaDelagator = 7,
    ishieldElephantHideFortress = 8,
    ishieldCompletePhaseShield = 9,
    ishieldCount = 10,
};
typedef uint16_t ishield;

enum ispecialE {
    ispecialETransportCloaking = 0,
    ispecialEStealthCloak = 1,
    ispecialESuperStealthCloak = 2,
    ispecialEUltraStealthCloak = 3,
    ispecialEMultiFunctionPod = 4,
    ispecialEBattleComputer = 5,
    ispecialEBattleSuperComputer = 6,
    ispecialEBattleNexus = 7,
    ispecialEJammer10 = 8,
    ispecialEJammer20 = 9,
    ispecialEJammer30 = 10,
    ispecialEJammer50 = 11,
    ispecialEEnergyCapacitor = 12,
    ispecialEFluxCapacitor = 13,
    ispecialEEnergyDampener = 14,
    ispecialETachyonDetector = 15,
    ispecialEAntiMatterGenerator = 16,
    ispecialECount = 17,
};
typedef uint16_t ispecialE;

enum ispecialM {
    ispecialMColonizationModule = 0,
    ispecialMOrbitalConstructionModule = 1,
    ispecialMCargoPod = 2,
    ispecialMSuperCargoPod = 3,
    ispecialMMultiCargoPod = 4,
    ispecialMFuelTank = 5,
    ispecialMSuperFuelTank = 6,
    ispecialMManeuveringJet = 7,
    ispecialMOverthruster = 8,
    ispecialMJumpGate = 9,
    ispecialMBeamDeflector = 10,
    ispecialMCount = 11,
};
typedef uint16_t ispecialM;

enum imines {
    iminesMineDispenser40 = 0,
    iminesMineDispenser50 = 1,
    iminesMineDispenser80 = 2,
    iminesMineDispenser130 = 3,
    iminesHeavyDispenser50 = 4,
    iminesHeavyDispenser110 = 5,
    iminesHeavyDispenser200 = 6,
    iminesSpeedTrap20 = 7,
    iminesSpeedTrap30 = 8,
    iminesSpeedTrap50 = 9,
    iminesCount = 10,
};
typedef uint16_t imines;

enum imining {
    iminingRoboMidgetMiner = 0,
    iminingRoboMiniMiner = 1,
    iminingRoboMiner = 2,
    iminingRoboMaxiMiner = 3,
    iminingRoboSuperMiner = 4,
    iminingRoboUltraMiner = 5,
    iminingAlienMiner = 6,
    iminingOrbitalAdjuster = 7,
    iminingCount = 8,
};
typedef uint16_t imining;

enum iplanetary {
    iplanetaryViewer50 = 0,
    iplanetaryViewer90 = 1,
    iplanetaryScoper150 = 2,
    iplanetaryScoper220 = 3,
    iplanetaryScoper280 = 4,
    iplanetarySnooper320X = 5,
    iplanetarySnooper400X = 6,
    iplanetarySnooper500X = 7,
    iplanetarySnooper620X = 8,
    iplanetarySDI = 9,
    iplanetaryMissileBattery = 10,
    iplanetaryLaserBattery = 11,
    iplanetaryPlanetaryShield = 12,
    iplanetaryNeutronShield = 13,
    iplanetaryGenesisDevice = 14,
    iplanetaryCount = 15,
};
typedef uint16_t iplanetary;

enum iterra {
    iterraTotalTerraform3 = 0,
    iterraTotalTerraform5 = 1,
    iterraTotalTerraform7 = 2,
    iterraTotalTerraform10 = 3,
    iterraTotalTerraform15 = 4,
    iterraTotalTerraform20 = 5,
    iterraTotalTerraform25 = 6,
    iterraTotalTerraform30 = 7,
    iterraGravityTerraform3 = 8,
    iterraGravityTerraform7 = 9,
    iterraGravityTerraform11 = 10,
    iterraGravityTerraform15 = 11,
    iterraTempTerraform3 = 12,
    iterraTempTerraform7 = 13,
    iterraTempTerraform11 = 14,
    iterraTempTerraform15 = 15,
    iterraRadiationTerraform3 = 16,
    iterraRadiationTerraform7 = 17,
    iterraRadiationTerraform11 = 18,
    iterraRadiationTerraform15 = 19,
    iterraCount = 20,
};
typedef uint16_t iterra;

enum ibomb {
    ibombLadyFingerBomb = 0,
    ibombBlackCatBomb = 1,
    ibombM70Bomb = 2,
    ibombM80Bomb = 3,
    ibombCherryBomb = 4,
    ibombLBU17Bomb = 5,
    ibombLBU32Bomb = 6,
    ibombLBU74Bomb = 7,
    ibombHushABoom = 8,
    ibombRetroBomb = 9,
    ibombSmartBomb = 10,
    ibombNeutronBomb = 11,
    ibombEnrichedNeutronBomb = 12,
    ibombPeerlessBomb = 13,
    ibombAnnihilatorBomb = 14,
    ibombCount = 15,
};
typedef uint16_t ibomb;

enum itorp {
    itorpAlphaTorpedo = 0,
    itorpBetaTorpedo = 1,
    itorpDeltaTorpedo = 2,
    itorpEpsilonTorpedo = 3,
    itorpRhoTorpedo = 4,
    itorpUpsilonTorpedo = 5,
    itorpOmegaTorpedo = 6,
    itorpAntiMatterTorpedo = 7,
    itorpJihadMissile = 8,
    itorpJuggernautMissile = 9,
    itorpDoomsdayMissile = 10,
    itorpArmageddonMissile = 11,
    itorpCount = 12,
};
typedef uint16_t itorp;

enum ibeam {
    ibeamLaser = 0,
    ibeamXRayLaser = 1,
    ibeamMiniGun = 2,
    ibeamYakimoraLightPhaser = 3,
    ibeamBlackjack = 4,
    ibeamPhaserBazooka = 5,
    ibeamPulsedSapper = 6,
    ibeamColloidalPhaser = 7,
    ibeamGatlingGun = 8,
    ibeamMiniBlaster = 9,
    ibeamBludgeon = 10,
    ibeamMarkIVBlaster = 11,
    ibeamPhasedSapper = 12,
    ibeamHeavyBlaster = 13,
    ibeamGatlingNeutrinoCannon = 14,
    ibeamMyopicDisruptor = 15,
    ibeamBlunderbuss = 16,
    ibeamDisruptor = 17,
    ibeamMultiContainedMunition = 18,
    ibeamSyncroSapper = 19,
    ibeamMegaDisruptor = 20,
    ibeamBigMuthaCannon = 21,
    ibeamStreamingPulverizer = 22,
    ibeamAntiMatterPulverizer = 23,
    ibeamCount = 24,
};
typedef uint16_t ibeam;

enum ispecialSB {
    ispecialSBStargate100250 = 0,
    ispecialSBStargateAny300 = 1,
    ispecialSBStargate150600 = 2,
    ispecialSBStargate300500 = 3,
    ispecialSBStargate100Any = 4,
    ispecialSBStargateAny800 = 5,
    ispecialSBStargateAnyAny = 6,
    ispecialSBMassDriver5 = 7,
    ispecialSBMassDriver6 = 8,
    ispecialSBMassDriver7 = 9,
    ispecialSBSuperDriver8 = 10,
    ispecialSBSuperDriver9 = 11,
    ispecialSBUltraDriver10 = 12,
    ispecialSBUltraDriver11 = 13,
    ispecialSBUltraDriver12 = 14,
    ispecialSBUltraDriver13 = 15,
    ispecialSBCount = 16,
};
typedef uint16_t ispecialSB;

enum GrbitTrader {
    grbitTraderNone = 0x0000,
    grbitTraderCargo = 0x0001,
    grbitTraderSpecial = 0x0002,
    grbitTraderShield = 0x0004,
    grbitTraderArmor = 0x0008,
    grbitTraderMiner = 0x0010,
    grbitTraderBomb = 0x0020,
    grbitTraderTorp = 0x0040,
    grbitTraderBeam = 0x0080,
    grbitTraderHull = 0x0100,
    grbitTraderEngine = 0x0200,
    grbitTraderGenesis = 0x0400,
    grbitTraderJumpgate = 0x0800,
    grbitTraderLifeboat = 0x1000,
    grbitTraderAll = 0x1fff,
};
typedef uint16_t GrbitTrader;

enum LookupResult {
    LookupInvalid = 0,     // “out of range” / not a valid part id in group
    LookupDisallowed = -1, // disallowed for race/trait/other rule
    LookupOk = 1,          // meets tech reqs (original CheckTechRequirements == 1)
    LookupNear = 2,        // “one level away in current research field”
    LookupNeedMany = 99    // multiple tech deficits
};
typedef int16_t LookupResult;

enum RecordType {
    /*
     * NOTE: Stars! file records encode a 6-bit "record type" (rt) plus a 10-bit
     * byte count (cb) in a 16-bit header word.
     *
     * In .HST files (and others), record type 0x00 is used for the footer record
     * (cb=2, data=0000). The original code treats "rt==0" as a terminator while
     * reading, so we keep rtEOF=0 for that behavior.
     */
    rtEOF = 0,
    rtLogCargoXfer8 = 1,         /* quantities are int8  (lpb[6+iLook]) */
    rtLogCargoXfer16 = 2,        /* quantities are int16 (lpb[6+2*iLook]) */
    rtLogFleetOrderDelete = 3,   /* delete 1 or 2 orders; index in *(u16*)(lpb+2), high bit => delete extra */
    rtLogFleetOrderInsert = 4,   /* insert new order at index *(i16*)(lpb+2); payload from lpb+4 */
    rtLogFleetOrderUpdate = 5,   /* overwrite existing order at index *(i16*)(lpb+2); payload from lpb+4 */
    rtPlr = 6,                   /* Player */
    rtGame = 7,                  /* Game */
    rtBOF = 8,                   /* FileHeader / BOF */
    rtLogFleetFlagBit9 = 10,     /* lpfl->wFlags_0x4 bit 9 set/cleared by (*(u16*)(lpb+2) & 1) */
    rtLogFleetOrderAttrNib = 11, /* order[index].word10 low nibble set to (*(i16*)(lpb+4) & 0xF), value constrained <=9 */
    rtMsg = 12,                  /* Message */
    rtPlanet = 13,
    rtPlanetB = 14,
    rtFleetA = 16,
    rtFleetB = 17,
    rtOrderA = 19, /* other order-like record type seen in decompile */
    rtOrderB = 20, // waypoint only
    rtString = 21, /* decompile: alloc/copy string from rgbCur when rt == 0x15 */
    rtSel = 22,    /* decompile: after things, if (rt == 0x16) ReadRt(); matches file.c rtSel */
    rtLogFleetCargoXfer = 23,
    rtLogFleetSplit = 24,  /* LpflNewSplit(&fleet) */
    rtLogCargoXfer32 = 25, /* quantities are int32 (lpb[6+4*iLook]) */
    rtShDef = 26,
    rtLogShDef = 27, /* Ship design definition (SHDEF) create/update/delete for the current player. */
    rtProdQ = 28,
    rtLogPlanetProdQ = 29,   /* Planet production queue set/clear (planet->lpplprod). */
    rtBtlPlan = 30,          /* decompile: while (rt == 0x1e) { ...battle plan... } */
    rtBtlData = 31,          /* decompile: while (rt == 0x1f || rt == 0x27) { ... } */
    rtContinue = 39,         /* decompile: inside loop: if (rt != 0x27) { ... } matches `rt != rtContinue` */
    rtHistHdr = 32,          /* decompile: after opening dtHist, expects rt == 0x20 */
    rtMsgFilt = 33,          /* decompile: checks cbbitfMsg vs cb and memcpy(bitfMsgFiltered, ...) */
    rtLogResearch = 34,      /* Research settings: pctResearch + iTechCur (packed nibble fields). */
    rtLogPlanetRouting = 35, /* Planet routing / starbase / infrastructure bitfields mutation. */
    rtLogRelations = 38,     /* memcpy rgplr[idPlayer].rgmdRelation[0..cPlayer) */
    rtChgPassword = 36,      /* file.c: if (hdrCur.rt == rtChgPassword) { lSaltCur = *(long*)rgbCur; } */
    rtLogFleetMerge = 37,    /* merge all-at-location (cb==2) or merge listed fleet ids (cb>2) */
    rtPlrMsg = 40,
    rtAiData = 41,            /* decompile: loop skips/reads while (rt == 0x29) around vlpbAiData */
    rtThing = 43,             /* decompile: if (rt == 0x2b) { cThing = rgbCur; alloc things } */
    rtLogFleetPlan = 42,      /* lpfl->iplan = *(u16*)(lpb+2) truncated */
    rtLogThingByteParam = 43, /* sets 1 byte inside THING union for a restricted subtype */
    rtLogFleetName = 44,      /* User string (fleet rename); may be compressed via FDecompressUserString. */
    rtScore = 45,             /* decompile: loop `if (rt != 0x2d) break;` in score load path */
    rtLogPlayerZpq1 = 46,     /* Host-only opaque blob (size capped at 0x1A bytes) copied into rgplr[idPlayer].zpq1. */
    rtMax = 47                /* one past highest observed (0x2d) */
};
typedef uint16_t RecordType;

enum cbStructSize {
    cbABC = 6,
    cbAIHIST = 1284,
    cbAIPART = 2,
    cbAISTARBASE = 20,
    cbARMOR = 54,
    cbBEAM = 60,
    cbBITMAP = 14,
    cbBITMAPCOREHEADER = 12,
    cbBITMAPCOREINFO = 15,
    cbBITMAPFILEHEADER = 14,
    cbBITMAPINFO = 44,
    cbBITMAPINFOHEADER = 40,
    cbBOMB = 58,
    cbBTLDATA = 14,
    cbBTLPLAN = 36,
    cbBTLREC = 6,
    cbBTLREC26 = 6,
    cbBTN = 14,
    cbBTNT = 24,
    cbCBTACTIVATESTRUCT = 4,
    cbCHOOSECOLOR = 32,
    cbCHOOSEFONT = 46,
    cbCLIENTCREATESTRUCT = 4,
    cbCOLDROP = 12,
    cbCOMPAREITEMSTRUCT = 18,
    cbCOMPART = 52,
    cbCOMPLEX = 16,
    cbCOMPLEXL = 20,
    cbCOMSTAT = 5,
    cbCREATESTRUCT = 34,
    cbCYBERINFO = 2,
    cbCYBERINFOTEMP = 2,
    cbDCB = 25,
    cbDEBUGHOOKINFO = 14,
    cbDELETEITEMSTRUCT = 12,
    cbDEVNAMES = 8,
    cbDOCINFO = 10,
    cbDRAWCIR = 24,
    cbDRAWITEMSTRUCT = 26,
    cbDRIVERINFOSTRUCT = 134,
    cbDRVCONFIGINFO = 12,
    cbDV = 2,
    cbENGINE = 78,
    cbENUMLOGFONT = 146,
    cbEVENTMSG = 10,
    cbEXCEPTION = 28,
    cbEXCEPTIONL = 34,
    cbFINDREPLACE = 36,
    cbFIXED = 4,
    cbFLEET = 124,
    cbFLEETID = 2,
    cbFLEETSOME = 12,
    cbFRAMESTUFF = 22,
    cbGAME = 64,
    cbGDATA = 10,
    cbGLYPHMETRICS = 12,
    cbHANDLETABLE = 2,
    cbHARDWAREHOOKSTRUCT = 10,
    cbHB = 16,
    cbHDR = 2,
    cbHELPWININFO = 14,
    cbHS = 4,
    cbHUL = 123,
    cbHULDEF = 143,
    cbINI = 26,
    cbITEMACTION = 2,
    cbKERNINGPAIR = 6,
    cbKILL = 8,
    cbLOGBRUSH = 8,
    cbLOGFONT = 50,
    cbLOGPALETTE = 8,
    cbLOGPEN = 10,
    cbLOGXFER = 24,
    cbLOGXFERF = 36,
    cbLSB = 4,
    cbMAT2 = 16,
    cbMDICREATESTRUCT = 26,
    cbMDPLR = 2,
    cbMEASUREITEMSTRUCT = 14,
    cbMENUITEMTEMPLATE = 5,
    cbMENUITEMTEMPLATEHEADER = 4,
    cbMETAFILEPICT = 8,
    cbMETAHEADER = 18,
    cbMETARECORD = 8,
    cbMINES = 54,
    cbMINING = 54,
    cbMINMAXINFO = 20,
    cbMOUSEHOOKSTRUCT = 12,
    cbMSG = 18,
    cbMSGBIG = 18,
    cbMSGHDR = 4,
    cbMSGPLR = 12,
    cbMSGTURN = 5,
    cbMULTIKEYHELP = 4,
    cbNEWTEXTMETRIC = 41,
    cbOBJ = 2,
    cbOFN = 72,
    cbOFSTRUCT = 136,
    cbORDER = 18,
    cbOUTLINETEXTMETRIC = 114,
    cbPAINTSTRUCT = 32,
    cbPALETTEENTRY = 4,
    cbPANOSE = 10,
    cbPART = 8,
    cbPD = 52,
    cbPL = 4,
    cbPLANET = 56,
    cbPLANETARY = 54,
    cbPLANETMINIMAL = 6,
    cbPLANETSOME = 23,
    cbPLAYER = 192,
    cbPLORD = 4,
    cbPLPROD = 4,
    cbPOINT = 4,
    cbPOINTFX = 8,
    cbPOPUPDATA = 22,
    cbPROD = 4,
    cbPRODQ1 = 2,
    cbRECT = 8,
    cbRGBQUAD = 4,
    cbRGBTRIPLE = 3,
    cbRPT = 54,
    cbRTBOF = 16,
    cbRTCHGNAME = 37,
    cbRTCHGPLANETLONG = 6,
    cbRTCHGPRODQ = 2,
    cbRTCHGSHDEF = 19,
    cbRTHISTHDR = 4,
    cbRTLOGHDR = 17,
    cbRTLOGTHING = 4,
    cbRTPLANET = 4,
    cbRTSHDEF = 17,
    cbRTSHIPINT = 4,
    cbRTSHIPINT2 = 6,
    cbRTWAYPT = 22,
    cbRTXFER = 7,
    cbRTXFERF = 9,
    cbRTXFERL = 10,
    cbRTXFERX = 8,
    cbSBAR = 12,
    cbSCAN = 16,
    cbSCANNER = 56,
    cbSCORE = 20,
    cbSCOREX = 24,
    cbSEGINFO = 16,
    cbSEL = 226,
    cbSELSOME = 28,
    cbSHDEF = 147,
    cbSHIELD = 54,
    cbSIZE = 4,
    cbSPECIAL = 54,
    cbSPECIALSB = 56,
    cbSTARPACK = 4,
    cbTASKLAYMINES = 4,
    cbTASKPATROL = 4,
    cbTASKSELL = 2,
    cbTASKXPORT = 10,
    cbTERRA = 54,
    cbTEXTMETRIC = 31,
    cbTHING = 18,
    cbTHMINE = 10,
    cbTHPACK = 10,
    cbTHTRADER = 10,
    cbTHWORM = 8,
    cbTILE = 16,
    cbTIMER = 10,
    cbTIMERINFO = 12,
    cbTOK = 29,
    cbTORP = 60,
    cbTTPOLYCURVE = 12,
    cbTTPOLYGONHEADER = 16,
    cbTURNSERIAL = 16,
    cbTUTOR = 44,
    cbVERS = 2,
    cbWINDEBUGINFO = 26,
    cbWINDOWPLACEMENT = 22,
    cbWINDOWPOS = 14,
    cbWN = 10,
    cbWNDCLASS = 26,
    cbXFER = 128,
    cbXFERFULL = 25,
    cbZIPORDER = 24,
    cbZIPPRODQ = 40,
    cbZIPPRODQ1 = 26,
    cbcomplex = 16,
};
typedef uint16_t cbStructSize;

enum DialogId {
    /* ship / fleet */
    IDD_MERGE_FLEETS = 82, /* MergeFleetsDlg */
    IDD_TRANSFER = 91,     /* TransferDlg */
    IDD_SLOT = 92,         /* SlotDlg */
    IDD_PRODUCTION = 93,   /* ProductionDlg */
    IDD_ORDER_INFO = 97,   /* OrderInfoDlg */

    /* common / utility */
    IDD_SERIAL_NUMBER = 86, /* MsgDlg; template captioned "Stars! Serial Number" */
    IDD_ZIP_PROD = 89,      /* ZipProdDlg / ZipOrderDlg */
    IDD_ABOUT = 90,         /* About */
    IDD_HOST_MODE = 115,    /* HostOptionsDialog */
    IDD_PASSWORD = 140,     /* PASSWORD dialog */
    IDD_NEW_PASSWORD = 141, /* NewPasswordDlg */

    /* research / browser */
    IDD_RESEARCH = 127, /* ResearchDlg */
    IDD_BROWSER = 128,  /* Browser child dialog */

    /* race wizard */
    IDD_RACE_WIZARD_1 = 146, /* RaceWizardDlg1 */
    IDD_RACE_WIZARD_2 = 147, /* unnamed proc (slot between 1 and 3) */
    IDD_RACE_WIZARD_3 = 148, /* RaceWizardDlg3 */
    IDD_RACE_WIZARD_4 = 149, /* RaceWizardDlg4 */
    IDD_RACE_WIZARD_5 = 150, /* RaceWizardDlg5 */
    IDD_RACE_WIZARD_6 = 151, /* RaceWizardDlg6 */

    /* VCR */
    IDD_VCR = 160, /* VCRDlg */

    /* new game */
    IDD_SIMPLE_NEW_GAME = 209, /* SimpleNewGameDlg */
    IDD_NEW_GAME_1 = 390,      /* NewGameDlg */
    IDD_NEW_GAME_2 = 391,      /* NewGameDlg2 */
    IDD_NEW_GAME_3 = 392,      /* NewGameDlg3 */

    IDD_Gauge = 393, /* never seen invoked */

    /* host / options */
    IDD_HOST_OPTIONS = 1026, /* HostOptionsDialog */
    IDD_SCORE = 102,         /* ScoreXDlg */

    /* battle / plans */
    IDD_BATTLE_PLANS = 2013, /* BattlePlansDlg */
    IDD_RENAME = 2019,       /* RenameDlg / NewPlanNameDlg (shared template) */

    /* relations / diplomacy */
    IDD_RELATIONS = 2008, /* RelationsDlg */

    IDD_SAVE_TURN1 = 1068, /* Save  */
    IDD_SAVE_TURN2 = 2025, /* Save  */

    /* tutorial / panic */
    IDD_PANIC = 2504, /* PanicDlg */
    IDD_TUTOR = 2502, /* never referenced */

    /* find */
    IDD_FIND = 4202, /* FindDlg */
    IDD_PRINT_MAP = 214, /* PrintMapDlg */
};
typedef uint16_t DialogId;

#undef IDOK
#undef IDCANCEL
#undef IDHELP
#undef IDC_HELP

// AboutControl names the controls of the About dialog.
enum AboutControl {
    IDC_ABOUT_ORDER_INFO = 118,
    IDC_ABOUT_DEMO_TEXT = 1025,
    IDC_ABOUT_CREDITS_TEXT = 1055,
};
typedef uint16_t AboutControl;

// OrderInfoControl names the controls of the order information dialog.
enum OrderInfoControl {
    IDC_ORDER_INFO_TEXT = 1025,
};
typedef uint16_t OrderInfoControl;

// PrintMapControl names the controls of the Print Map dialog.
enum PrintMapControl {
    IDC_PRINT_MAP_PAGES_X = 268,
    IDC_PRINT_MAP_PAGES_Y = 269,
};
typedef uint16_t PrintMapControl;

// SimpleNewGameControl names the controls of the simple New Game dialog.
enum SimpleNewGameControl {
    IDC_SIMPLE_NEW_GAME_EASY = 200,
    IDC_SIMPLE_NEW_GAME_STANDARD = 201,
    IDC_SIMPLE_NEW_GAME_HARDER = 202,
    IDC_SIMPLE_NEW_GAME_EXPERT = 203,
    IDC_SIMPLE_NEW_GAME_CUSTOMIZE_RACE = 210,
    IDC_SIMPLE_NEW_GAME_ADVANCED = 211,
    IDC_SIMPLE_NEW_GAME_TUTORIAL = 212,
    IDC_SIMPLE_NEW_GAME_TINY = 1000,
    IDC_SIMPLE_NEW_GAME_SMALL = 1001,
    IDC_SIMPLE_NEW_GAME_MEDIUM = 1002,
    IDC_SIMPLE_NEW_GAME_LARGE = 1003,
    IDC_SIMPLE_NEW_GAME_HUGE = 1004,
};
typedef uint16_t SimpleNewGameControl;

// NewGame1Control names the controls of the first advanced New Game wizard page.
enum NewGame1Control {
    IDC_NEW_GAME_TINY = 1000,
    IDC_NEW_GAME_SMALL = 1001,
    IDC_NEW_GAME_MEDIUM = 1002,
    IDC_NEW_GAME_LARGE = 1003,
    IDC_NEW_GAME_HUGE = 1004,
    IDC_NEW_GAME_SPARSE = 1005,
    IDC_NEW_GAME_NORMAL = 1006,
    IDC_NEW_GAME_DENSE = 1007,
    IDC_NEW_GAME_PACKED = 1008,
    IDC_NEW_GAME_CLOSE = 1009,
    IDC_NEW_GAME_MODERATE = 1010,
    IDC_NEW_GAME_FARTHER = 1011,
    IDC_NEW_GAME_DISTANT = 1012,
    IDC_NEW_GAME_MAX_MINERALS = 1016,
    IDC_NEW_GAME_SLOWER_TECH = 1017,
    IDC_NEW_GAME_ACCELERATED_BBS = 1018,
    IDC_NEW_GAME_NO_RANDOM_EVENTS = 1019,
    IDC_NEW_GAME_AI_ALLIANCES = 1020,
    IDC_NEW_GAME_PUBLIC_SCORES = 1021,
    IDC_NEW_GAME_NAME = 1030,
    IDC_NEW_GAME_GALAXY_CLUMPING = 1050,
};
typedef uint16_t NewGame1Control;

// NewGame3Control names the controls of the victory conditions New Game wizard page; the tech level checkbox covers both tech conditions.
enum NewGame3Control {
    IDC_VC_OWNS_PLANETS = 291,
    IDC_VC_TECH_LEVEL = 292,
    IDC_VC_SCORE = 293,
    IDC_VC_SECOND_PLACE = 294,
    IDC_VC_PRODUCTION = 295,
    IDC_VC_CAPITAL_SHIPS = 296,
    IDC_VC_HIGHEST_SCORE = 297,
};
typedef uint16_t NewGame3Control;

// VcrControl names the controls of the battle VCR dialog.
enum VcrControl {
    IDC_VCR_REW_ALL = 161,
    IDC_VCR_REW = 162,
    IDC_VCR_PLAY_PAUSE = 163,
    IDC_VCR_FWD = 164,
    IDC_VCR_FWD_ALL = 165,
};
typedef uint16_t VcrControl;

// HostModeControl names the controls of the host mode dialog.
enum HostModeControl {
    IDC_HOST_GENERATE_NOW = 1031,
    IDC_HOST_AUTO_GENERATE = 1032,
    IDC_HOST_GAME_NAME_TEXT = 1033,
    IDC_HOST_FILE_TEXT = 1034,
    IDC_HOST_PASSWORD = 2015,
    IDC_HOST_NEXT_YEAR_TEXT = 2016,
    IDC_HOST_TIME_SINCE_TEXT = 2017,
};
typedef uint16_t HostModeControl;

// HostOptionsControl names the controls of the auto generate options dialog; the second and third force generate options have no caption in the template.
enum HostOptionsControl {
    IDC_AUTOGEN_WHEN_ALL_IN = 1027,
    IDC_FORCE_GEN_NEVER = 2066,
    IDC_FORCE_GEN_OPTION_2 = 2067,
    IDC_FORCE_GEN_OPTION_3 = 2068,
};
typedef uint16_t HostOptionsControl;

// SlotControl names the controls of the ship designer dialog.
enum SlotControl {
    IDC_DESIGNER_COMPONENT_LIST = 2060,
    IDC_DESIGNER_SHIPS = 2064,
    IDC_DESIGNER_STARBASES = 2065,
    IDC_DESIGNER_EXISTING = 2066,
    IDC_DESIGNER_HULLS = 2067,
    IDC_DESIGNER_ENEMY_HULLS = 2068,
    IDC_DESIGNER_COMPONENTS = 2069,
};
typedef uint16_t SlotControl;

// RaceWizard1Control names the controls of the race wizard name and race page.
enum RaceWizard1Control {
    IDC_RACE_NAME = 268,
    IDC_RACE_PASSWORD = 269,
    IDC_RACE_HUMANOID = 271,
    IDC_RACE_RABBITOID = 272,
    IDC_RACE_INSECTOID = 273,
    IDC_RACE_NUCLEOTID = 274,
    IDC_RACE_SILICANOID = 275,
    IDC_RACE_ANTETHERAL = 276,
    IDC_RACE_RANDOM = 277,
    IDC_RACE_CUSTOM = 278,
    IDC_RACE_PLURAL_NAME = 2075,
};
typedef uint16_t RaceWizard1Control;

// RaceWizard2Control names the controls of the race wizard habitability page.
enum RaceWizard2Control {
    IDC_IMMUNE_TO_GRAVITY = 291,
    IDC_IMMUNE_TO_TEMPERATURE = 292,
    IDC_IMMUNE_TO_RADIATION = 293,
};
typedef uint16_t RaceWizard2Control;

// RaceWizard3Control names the controls of the race wizard economy page.
enum RaceWizard3Control {
    IDC_RACE_FACTORY_GERMANIUM_DISCOUNT = 291,
};
typedef uint16_t RaceWizard3Control;

// RaceWizard4Control names the controls of the race wizard primary racial trait page.
enum RaceWizard4Control {
    IDC_RACE_HYPER_EXPANSION = 271,
    IDC_RACE_SUPER_STEALTH = 272,
    IDC_RACE_WAR_MONGER = 273,
    IDC_RACE_CLAIM_ADJUSTER = 274,
    IDC_RACE_INNER_STRENGTH = 275,
    IDC_RACE_SPACE_DEMOLITION = 276,
    IDC_RACE_PACKET_PHYSICS = 277,
    IDC_RACE_INTERSTELLAR_TRAVELER = 278,
    IDC_RACE_ALTERNATE_REALITY = 279,
    IDC_RACE_JACK_OF_ALL_TRADES = 280,
};
typedef uint16_t RaceWizard4Control;

// RaceWizard5Control names the controls of the race wizard lesser racial trait page, in RaceGrbit order.
enum RaceWizard5Control {
    IDC_RACE_IMPROVED_FUEL_EFFICIENCY = 291,
    IDC_RACE_TOTAL_TERRAFORMING = 292,
    IDC_RACE_ADVANCED_REMOTE_MINING = 293,
    IDC_RACE_IMPROVED_STARBASES = 294,
    IDC_RACE_GENERALIZED_RESEARCH = 295,
    IDC_RACE_ULTIMATE_RECYCLING = 296,
    IDC_RACE_MINERAL_ALCHEMY = 297,
    IDC_RACE_NO_RAM_SCOOP_ENGINES = 298,
    IDC_RACE_CHEAP_ENGINES = 299,
    IDC_RACE_ONLY_BASIC_REMOTE_MINING = 300,
    IDC_RACE_NO_ADVANCED_SCANNERS = 301,
    IDC_RACE_LOW_STARTING_POPULATION = 302,
    IDC_RACE_BLEEDING_EDGE_TECHNOLOGY = 303,
    IDC_RACE_REGENERATING_SHIELDS = 304,
};
typedef uint16_t RaceWizard5Control;

// RaceWizard6Control names the controls of the race wizard research cost page.
enum RaceWizard6Control {
    IDC_RACE_ENERGY_COST_EXTRA = 271,
    IDC_RACE_ENERGY_COST_STANDARD = 272,
    IDC_RACE_ENERGY_COST_LESS = 273,
    IDC_RACE_WEAPONS_COST_EXTRA = 274,
    IDC_RACE_WEAPONS_COST_STANDARD = 275,
    IDC_RACE_WEAPONS_COST_LESS = 276,
    IDC_RACE_PROPULSION_COST_EXTRA = 277,
    IDC_RACE_PROPULSION_COST_STANDARD = 278,
    IDC_RACE_PROPULSION_COST_LESS = 279,
    IDC_RACE_CONSTRUCTION_COST_EXTRA = 280,
    IDC_RACE_CONSTRUCTION_COST_STANDARD = 281,
    IDC_RACE_CONSTRUCTION_COST_LESS = 282,
    IDC_RACE_ELECTRONICS_COST_EXTRA = 283,
    IDC_RACE_ELECTRONICS_COST_STANDARD = 284,
    IDC_RACE_ELECTRONICS_COST_LESS = 285,
    IDC_RACE_BIOTECH_COST_EXTRA = 286,
    IDC_RACE_BIOTECH_COST_STANDARD = 287,
    IDC_RACE_BIOTECH_COST_LESS = 288,
    IDC_RACE_START_HIGHER_TECH = 291,
};
typedef uint16_t RaceWizard6Control;

// ProductionControl names the controls of the production queue dialog.
enum ProductionControl {
    IDC_PRODUCTION_RESEARCH_LEFTOVERS_ONLY = 139,
    IDC_PRODUCTION_AVAILABLE_ITEMS = 1046,
    IDC_PRODUCTION_QUEUE = 1047,
    IDC_PRODUCTION_ADD = 1048,
    IDC_PRODUCTION_REMOVE = 1049,
    IDC_PRODUCTION_CLEAR = 1069,
    IDC_PRODUCTION_ITEM_UP = 1081,
    IDC_PRODUCTION_ITEM_DOWN = 1082,
};
typedef uint16_t ProductionControl;

// MergeFleetsControl names the controls of the merge fleets dialog.
enum MergeFleetsControl {
    IDC_MERGE_FLEETS_LIST = 81,
    IDC_MERGE_FLEETS_SELECT_ALL = 2040,
    IDC_MERGE_FLEETS_UNSELECT_ALL = 2041,
};
typedef uint16_t MergeFleetsControl;

// BattlePlansControl names the controls of the battle plans dialog.
enum BattlePlansControl {
    IDC_BATTLE_PLAN_COPY = 1052,
    IDC_BATTLE_PLAN_DUMP_CARGO = 1053,
    IDC_BATTLE_PLAN_SELECT = 1054,
    IDC_BATTLE_PLAN_PRIMARY_TARGET = 1055,
    IDC_BATTLE_PLAN_SECONDARY_TARGET = 1056,
    IDC_BATTLE_PLAN_TACTIC = 1057,
    IDC_BATTLE_PLAN_ATTACK_WHO = 1058,
};
typedef uint16_t BattlePlansControl;

// RelationsControl names the controls of the player relations dialog.
enum RelationsControl {
    IDC_RELATIONS_PLAYER_LIST = 2003,
    IDC_RELATIONS_NEUTRAL = 2004,
    IDC_RELATIONS_FRIEND = 2005,
    IDC_RELATIONS_ENEMY = 2006,
};
typedef uint16_t RelationsControl;

// ResearchControl names the controls of the research dialog.
enum ResearchControl {
    IDC_RESEARCH_ENERGY = 1073,
    IDC_RESEARCH_WEAPONS = 1074,
    IDC_RESEARCH_PROPULSION = 1075,
    IDC_RESEARCH_CONSTRUCTION = 1076,
    IDC_RESEARCH_ELECTRONICS = 1077,
    IDC_RESEARCH_BIOTECH = 1078,
    IDC_RESEARCH_NEXT_FIELD = 1083,
};
typedef uint16_t ResearchControl;

// ZipProdControl names the controls of the production template dialog.
enum ZipProdControl {
    IDC_ZIP_PROD_QUEUE = 1047,
    IDC_ZIP_PROD_PRESET_1 = 1073,
    IDC_ZIP_PROD_PRESET_2 = 1074,
    IDC_ZIP_PROD_PRESET_3 = 1075,
    IDC_ZIP_PROD_PRESET_4 = 1076,
};
typedef uint16_t ZipProdControl;

// GaugeControl names the controls of the progress gauge dialog.
enum GaugeControl {
    IDC_GAUGE_TEXT = 1071,
};
typedef uint16_t GaugeControl;

// BrowserControl names the controls of the technology browser dialog.
enum BrowserControl {
    IDC_BROWSER_AVAILABLE_ONLY = 266,
    IDC_BROWSER_COMPONENT_CATEGORY = 267,
};
typedef uint16_t BrowserControl;

// TutorControl names the controls of the tutorial dialog.
enum TutorControl {
    IDC_TUTOR_HINT = 118,
    IDC_TUTOR_PANIC = 2503,
};
typedef uint16_t TutorControl;

// PanicControl names the controls of the tutorial panic dialog.
enum PanicControl {
    IDC_PANIC_REDO_TURN = 2505,
    IDC_PANIC_COMPLETE_TURN = 2506,
};
typedef uint16_t PanicControl;

// NewPasswordControl names the controls of the new password dialog.
enum NewPasswordControl {
    IDC_PASSWORD_CONFIRM = 269,
};
typedef uint16_t NewPasswordControl;

// ScoreControl names the controls of the score dialog.
enum ScoreControl {
    IDC_SCORE_SWITCH = 198,
};
typedef uint16_t ScoreControl;

enum ControlId {
    // Controls shared by several dialogs with the same meaning; a dialog's
    // own controls are in its dialog_controls enum.
    IDOK = 1,
    IDCANCEL = 2,
    IDHELP = 9,
    IDC_HELP = 118,

    // wizard and list navigation
    IDC_BACK = 1070,
    IDC_NEXT = 1071,
    IDC_FINISH = 1072,

    // name and text edits
    IDC_EDIT1 = 268,
    IDC_EDITTEXT = 268,
    IDC_EDITNAME = 2075,
    IDC_PASSWORD_STATUS_TEXT = 2018,
    IDC_COMBOBOX = 2074,

    // list item buttons
    IDC_RENAME = 1051,
    IDC_IMPORT = 2070,
    IDC_DELETE = 2071,
    IDC_EDIT = 2072,
    IDC_SHIPLIST = 1035,

    // save turn
    IDC_SAVE = 1065,
    IDC_SAVESUBMIT = 1066,
    IDC_NO_DON_T_SAVE = 1067,

    // ---- Menu and accelerator commands (WM_COMMAND ids) -------------------
    IDM_DEBUG_DUMP_FLEETS = 0x0053,   // DumpFleets()
    IDM_DEBUG_DUMP_PLANETS = 0x0054,  // DumpPlanets()
    IDM_DEBUG_DUMP_UNIVERSE = 0x0055, // DumpUniverse()

    // ---- About / score dialogs -------------------------------------------
    IDM_GAME_SCORE = 0x005F,  // Score dialog (one entry point)
    IDM_GAME_SCORE2 = 0x0060, // Score dialog (alternate entry point)
    IDM_HELP_ABOUT = 0x0063,  // About dialog

    // ---- Fleet waypoint editing ------------------------------------------
    IDM_FLEET_DELETE_WAYPOINT = 0x0067, // Delete current waypoint (confirm)
    IDM_FLEET_INSERT_WAYPOINT = 0x0068, // Waypoint insert/delete sibling command

    // ---- File / game lifecycle -------------------------------------------
    IDM_FILE_HOST_GAME = 0x0069,       // Host game ?
    IDM_TURN_WAIT_NEW = 0x006A,        // Turn > Wait for New
    IDM_FILE_OPEN_GAME = 0x006D,       // Open game
    IDM_FILE_NEW_GAME = 0x006E,        // New game wizard
    IDM_FILE_RETURN_TO_TITLE = 0x0071, // Close game, return to title screen

    // Toolbar/accelerator aliases that jump to the same paths
    IDM_TOOL_NEW_GAME = 0x0ED8,  // Alias: New game
    IDM_TOOL_OPEN_GAME = 0x0ED9, // Alias: Open game

    // ---- Commands (ship design / research / diplomacy) --------------------
    IDM_GAME_SHIP_BUILDER = 0x007D, // ShipBuilder
    IDM_GAME_RESEARCH = 0x007E,     // Research dialog

    // Diplomacy / battle plans / turn control cluster
    IDM_GAME_RELATIONS = 0x07D9,     // Relations dialog
    IDM_GAME_WAIT_FOR_TURN = 0x07DA, // Wait-for-turn dialog/command
    IDM_GAME_BATTLE_PLANS1 = 0x07DB, // Battle plans dialog
    IDM_GAME_BATTLE_PLANS2 = 0x07DC, // Battle plans dialog (alias)
    IDM_GAME_RELATIONS2 = 0x07DE,    // Relations dialog (alias)

    // ---- View / window layout --------------------------------------------
    IDM_VIEW_LAYOUT_0 = 0x0082, // Window layout 0
    IDM_VIEW_LAYOUT_1 = 0x0083, // Window layout 1
    IDM_VIEW_LAYOUT_2 = 0x0084, // Window layout 2 ("small" layout)

    // Browser toggle (menu vs alias ID)
    IDM_VIEW_BROWSER_TOGGLE = 0x0088,  // Toggle tech browser window
    IDM_VIEW_BROWSER_TOGGLE2 = 0x0100, // Alias: browser toggle

    // Help index (menu vs alias ID)
    IDM_HELP_CONTENTS = 0x008A,  // Help index/contents
    IDM_HELP_CONTENTS2 = 0x0101, // Alias: help index/contents

    // ---- Race wizards -----------------------------------------------------
    IDM_RACE_CREATE = 0x0081, // Race creation wizard (default players)
    IDM_RACE_EDIT1 = 0x009C,  // Race edit wizard (existing player)
    IDM_RACE_EDIT2 = 0x009D,  // Race edit wizard (alias)

    // ---- Reports ----------------------------------------------------------
    IDM_REPORT_PLANET = 0x08FD,      // Planet report
    IDM_REPORT_CYCLE = 0x08FE,       // Cycle report type
    IDM_REPORT_FLEET = 0x08FF,       // Fleet report
    IDM_REPORT_ENEMY_FLEET = 0x0900, // Enemy fleets report
    IDM_REPORT_BATTLE = 0x0901,      // Battles report

    // ---- MRU (Most Recently Used) slots ----------------------------------
    IDM_FILE_MRU1 = 0x10CC, // MRU slot 1
    IDM_FILE_MRU2 = 0x10CD, // MRU slot 2
    IDM_FILE_MRU3 = 0x10CE, // MRU slot 3
    IDM_FILE_MRU4 = 0x10CF, // MRU slot 4
    IDM_FILE_MRU5 = 0x10D0, // MRU slot 5
    IDM_FILE_MRU6 = 0x10D1, // MRU slot 6
    IDM_FILE_MRU7 = 0x10D2, // MRU slot 7
    IDM_FILE_MRU8 = 0x10D3, // MRU slot 8
    IDM_FILE_MRU9 = 0x10D4, // MRU slot 9

    // ---- Scanner zoom factors (radio group) -------------------------------
    IDM_SCAN_ZOOM_0 = 0x0F3D, // scanner zoom (entry 0)
    IDM_SCAN_ZOOM_1 = 0x0F3E, // scanner zoom (entry 1)
    IDM_SCAN_ZOOM_2 = 0x0F3F, // scanner zoom (entry 2)
    IDM_SCAN_ZOOM_3 = 0x0F40, // scanner zoom (entry 3)
    IDM_SCAN_ZOOM_4 = 0x0F41, // scanner zoom (entry 4) (baseline in code)
    IDM_SCAN_ZOOM_5 = 0x0F42, // scanner zoom (entry 5)
    IDM_SCAN_ZOOM_6 = 0x0F43, // scanner zoom (entry 6)
    IDM_SCAN_ZOOM_7 = 0x0F44, // scanner zoom (entry 7)
    IDM_SCAN_ZOOM_8 = 0x0F45, // scanner zoom (entry 8)

    // ---- Turn ending / host/generate variants ------------------------------
    IDM_TURN_END_A = 0x0EDA, // end turn variant A
    IDM_TURN_END_B = 0x0EDB, // end turn variant B (toggles an internal bit)

    // ---- Dynamic popup range ----------------------------------------------
    IDM_POPUP_BASE = 15000, // Dynamic popup items start here (inferred)

    // ---- Debug: force-generate turns (decompiler had type confusion) -------
    IDM_DEBUG_GEN_10_TURNS = 21000,  // generate 10 turns (inferred)
    IDM_DEBUG_GEN_100_TURNS = 21001, // generate 100 turns (0x5209)
    IDM_DEBUG_GEN_1000_TURNS = 21002,
    IDM_VIEW_PLAYER_COLORS = 0x098D,
    IDM_UNKNOWN_09C1 = 0x09C1,
    IDM_HELP_INTRO = 0x09C2,
    IDM_UNKNOWN_09C4 = 0x09C4,
    IDM_HELP_TUTORIAL = 0x09C5,
    IDM_FILE_EXIT = 0x0EE2,
    IDM_FRAME_POST_OPEN = 0x0FA1,
    IDM_VIEW_FIND = 0x1068,
    IDM_UNKNOWN_1069 = 0x1069,

    IDM_GAME_RESEARCH2 = 0x0087,
    IDM_GAME_SHIP_BUILDER2 = 0x0089,
    IDM_VIEW_GAME_PARAMS = 0x009E,
    IDM_VIEW_GAME_PARAMS2 = 0x009F,
    IDM_VIEW_TOOLBAR = 0x00B3,
    IDM_FILE_PRINT_MAP = 0x00D5,
    IDM_TITLE_NEW_GAME = 0x00FA,
    IDM_TITLE_OPEN_GAME = 0x00FB,
    IDM_TITLE_CONTINUE = 0x00FC,
    IDM_TITLE_EXIT = 0x00FD,
    IDM_CMD_CHANGE_PASSWORD = 0x010E,
    IDM_TURN_SAVE_SUBMIT = 0x0428,

    WMX_UNKNOWN_0069 = 0x0069,
    WMX_UNKNOWN_006A = 0x006A,
    WMX_UNKNOWN_006C = 0x006C,
    WMX_UNKNOWN_006F = 0x006F,

};
typedef uint16_t ControlId;

/* Numeric cursor resources, named after the hcur globals they load into;
 * the others are named (SCANNERCUR, ...). */
#undef IDC_HAND
enum CursorId {
    IDC_NO_WAY = 121,
    IDC_TRASH_CAN = 122,
    IDC_RESIZE_WE = 258,
    IDC_RESIZE_NS = 260,
    IDC_RESIZE_4WAY = 263,
    IDC_ARROW_HELP = 264,
    IDC_HAND = 265,
};
typedef uint16_t CursorId;

/* Numeric bitmap resources; the others are named (CARGOBMP, ...). IDB_ ones
 * are loaded with LoadBitmap, IDDIB_ ones as DIBs through FindResource. */
enum BitmapId {
    IDDIB_PLAYER_ICONS_TINY = 79,
    IDDIB_PLAYER_ICONS_SMALL = 80,
    IDDIB_THING_ICONS = 87,
    IDDIB_SCANNER_TOOLBAR = 88,
    IDDIB_PLANET_ICONS = 112,
    IDB_EMPTY_HULL_SLOT = 119,
    IDDIB_PLAYER_ICONS = 133,
    IDB_MSGFILTER_CHECKBOX = 134,
    IDB_TOOLBAR = 178,
    IDB_FILTER_CHECKBOX_MONO = 199,
    IDB_FONT_DIGITS = 249,
    IDDIB_SPLASH = 449,
    IDB_MINESPAT_1 = 460,
    IDB_MINESPAT_2 = 461,
    IDB_MINESPAT_3 = 462,
    IDDIB_TECH_ICONS_1 = 500,
    IDDIB_TECH_ICONS_2 = 501,
    IDDIB_TECH_ICONS_3 = 502,
    IDDIB_TECH_ICONS_4 = 503,
    IDDIB_TECH_ICONS_5 = 504,
    IDDIB_TECH_ICONS_6 = 505,
    IDDIB_TECH_ICONS_7 = 506,
    IDDIB_HULL_ICONS_1 = 552,
    IDDIB_HULL_ICONS_2 = 553,
    IDDIB_HULL_ICONS_3 = 554,
    IDDIB_HULL_ICONS_4 = 555,
    IDDIB_HULL_ICONS_5 = 556,
    IDDIB_HULL_ICONS_SMALL_1 = 557,
    IDDIB_HULL_ICONS_SMALL_2 = 558,
    IDDIB_HULL_ICONS_SMALL_3 = 559,
    IDDIB_HULL_ICONS_SMALL_4 = 560,
    IDDIB_HULL_ICONS_SMALL_5 = 561,
    IDDIB_NUM_DESIGNS_PLATE = 1079,
};
typedef uint16_t BitmapId;

enum AcceleratorId {
    IDA_MAIN = 116,
    IDA_TITLE = 1080,
};
typedef uint16_t AcceleratorId;

/* Tutorial game files, each stored as its own custom resource type. */
enum TutorialResourceId {
    RT_TUTORIAL_HST = 10000,
    IDR_TUTORIAL_HST = 10001,
    RT_TUTORIAL_M1 = 10002,
    IDR_TUTORIAL_M1 = 10003,
    RT_TUTORIAL_M2 = 10004,
    IDR_TUTORIAL_M2 = 10005,
};
typedef uint16_t TutorialResourceId;

enum VictoryCondition {
    vcOwnsPercentPlanets = 0,     /* "Owns % of all planets." */
    vcAttainsTechLevel = 1,       /* "Attains Tech X in Y fields." (level) */
    vcAttainsTechFields = 2,      /* number of tech fields */
    vcExceedsScore = 3,           /* "Exceeds a score of X." */
    vcExceedsSecondPlaceBy = 4,   /* "Exceeds second place score by X." */
    vcProductionCapacity = 5,     /* "Has a production capacity of X thousand." */
    vcOwnsCapitalShips = 6,       /* "Owns X capital ships." */
    vcHighestScoreAfterYears = 7, /* "Has the highest score after X years." */
    vcMeetsNumCriteria = 8,       /* "Winner must meet X of the above selected criteria." */
    vcMinYearsBeforeWin = 9       /* "At least X years must pass before a winner is declared." */
};
typedef uint16_t VictoryCondition;

enum MdOpenFlags {
    /* access + share combinations */
    mdRead = 0x0020,      /* OF_READ | OF_SHARE_DENY_WRITE */
    mdReadWrite = 0x0012, /* OF_READWRITE | OF_SHARE_EXCLUSIVE */

    /* create/truncate */
    mdCreate = 0x1012, /* OF_CREATE | OF_READWRITE | OF_SHARE_EXCLUSIVE */

    /* Stars!-specific modifier */
    mdNoOpenErr = 0x4000,
};
typedef uint16_t MdOpenFlags;

enum TaskType {
    grTaskNone = 0,
    grTaskXfer = 1, /* transport / transfer cargo */
    grTaskColonize = 2,
    grTaskMine = 3, /* remote mining */
    grTaskMerge = 4,
    grTaskScrap = 5,
    grTaskLayMines = 6,
    grTaskPatrol = 7,
    grTaskAutoRoute = 8, /* auto-route / auto-order */
    grTaskGive = 9,
};
typedef uint16_t TaskType;

enum XferActionType {
    iActionNone = 0, /* implicit / cleared */

    iActionLoadAll = 1,     /* "Load All Available"        */
    iActionUnloadAll = 2,   /* "Unload All"                */
    iActionLoadExact = 3,   /* "Load Exactly..."           */
    iActionUnloadExact = 4, /* "Unload Exactly..."         */
    iActionFillPercent = 5, /* "Fill Up to %..."           */
    iActionWaitPercent = 6, /* "Wait for %..."             */
    iActionLoadDunnage = 7, /* "Load Dunnage"              */
    iActionSetAmount = 8,   /* "Set Amount to..."          */
    iActionSetWaypoint = 9, /* "Set Waypoint to..."        */
    /* iActionLoadOptimal is encoded via iActionLoadDunnage + fuel path */
};
typedef uint16_t XferActionType;

enum MdTarget {
    mdTargetNone = 0,              /* "None/Disengage" */
    mdTargetAny = 1,               /* "Any" */
    mdTargetStarbase = 2,          /* "Starbase" */
    mdTargetArmedShips = 3,        /* "Armed Ships" */
    mdTargetBombersFreighters = 4, /* "Bombers/Freighters" */
    mdTargetUnarmedShips = 5,      /* "Unarmed Ships" */
    mdTargetFuelTransports = 6,    /* "Fuel Transports" */
    mdTargetFreighters = 7,        /* "Freighters" */
};
typedef uint16_t MdTarget;

enum BattleTactic {
    mdTacticDisengage = 0,             /* "Disengage" */
    mdTacticDisengageIfChallenged = 1, /* "Disengage if challenged" */
    mdTacticMinDamageToSelf = 2,       /* "Minimize damage to self" */
    mdTacticMaxNetDamage = 3,          /* "Maximize net damage" */
    mdTacticMaxDamageRatio = 4,        /* "Maximize damage ratio" */
    mdTacticMaxDamage = 5,             /* "Maximize damage" */
};
typedef uint16_t BattleTactic;

enum GrfWeapon {
    bitFBeamLow = 0x0001,
    bitFBeamHigh = 0x0002,
    bitFTorp = 0x0004,
    bitFMissile = 0x0008,
    bitFDeflected = 0x0080,
};
typedef uint16_t GrfWeapon;


// AI research targets pack the tech field in the upper three bits and level in the lower five.
enum AiResearchTarget {
    aiResearchEnergy2 = (0 << 5) | 2,
    aiResearchEnergy3 = (0 << 5) | 3,
    aiResearchEnergy4 = (0 << 5) | 4,
    aiResearchEnergy6 = (0 << 5) | 6,
    aiResearchEnergy7 = (0 << 5) | 7,
    aiResearchEnergy9 = (0 << 5) | 9,
    aiResearchEnergy10 = (0 << 5) | 10,
    aiResearchEnergy14 = (0 << 5) | 14,
    aiResearchEnergy15 = (0 << 5) | 15,
    aiResearchEnergy18 = (0 << 5) | 18,
    aiResearchEnergy20 = (0 << 5) | 20,
    aiResearchEnergy22 = (0 << 5) | 22,
    aiResearchEnergy23 = (0 << 5) | 23,
    aiResearchEnergy26 = (0 << 5) | 26,
    aiResearchWeapons3 = (1 << 5) | 3,
    aiResearchWeapons5 = (1 << 5) | 5,
    aiResearchWeapons6 = (1 << 5) | 6,
    aiResearchWeapons7 = (1 << 5) | 7,
    aiResearchWeapons8 = (1 << 5) | 8,
    aiResearchWeapons10 = (1 << 5) | 10,
    aiResearchWeapons11 = (1 << 5) | 11,
    aiResearchWeapons14 = (1 << 5) | 14,
    aiResearchWeapons15 = (1 << 5) | 15,
    aiResearchWeapons17 = (1 << 5) | 17,
    aiResearchWeapons20 = (1 << 5) | 20,
    aiResearchWeapons23 = (1 << 5) | 23,
    aiResearchWeapons24 = (1 << 5) | 24,
    aiResearchWeapons26 = (1 << 5) | 26,
    aiResearchPropulsion2 = (2 << 5) | 2,
    aiResearchPropulsion5 = (2 << 5) | 5,
    aiResearchPropulsion6 = (2 << 5) | 6,
    aiResearchPropulsion7 = (2 << 5) | 7,
    aiResearchPropulsion8 = (2 << 5) | 8,
    aiResearchPropulsion9 = (2 << 5) | 9,
    aiResearchPropulsion12 = (2 << 5) | 12,
    aiResearchPropulsion13 = (2 << 5) | 13,
    aiResearchPropulsion16 = (2 << 5) | 16,
    aiResearchPropulsion17 = (2 << 5) | 17,
    aiResearchPropulsion20 = (2 << 5) | 20,
    aiResearchPropulsion22 = (2 << 5) | 22,
    aiResearchPropulsion26 = (2 << 5) | 26,
    aiResearchConstruction3 = (3 << 5) | 3,
    aiResearchConstruction4 = (3 << 5) | 4,
    aiResearchConstruction6 = (3 << 5) | 6,
    aiResearchConstruction10 = (3 << 5) | 10,
    aiResearchConstruction13 = (3 << 5) | 13,
    aiResearchConstruction16 = (3 << 5) | 16,
    aiResearchConstruction17 = (3 << 5) | 17,
    aiResearchConstruction18 = (3 << 5) | 18,
    aiResearchConstruction20 = (3 << 5) | 20,
    aiResearchConstruction21 = (3 << 5) | 21,
    aiResearchConstruction23 = (3 << 5) | 23,
    aiResearchConstruction24 = (3 << 5) | 24,
    aiResearchConstruction26 = (3 << 5) | 26,
    aiResearchElectronics3 = (4 << 5) | 3,
    aiResearchElectronics5 = (4 << 5) | 5,
    aiResearchElectronics6 = (4 << 5) | 6,
    aiResearchElectronics7 = (4 << 5) | 7,
    aiResearchElectronics9 = (4 << 5) | 9,
    aiResearchElectronics10 = (4 << 5) | 10,
    aiResearchElectronics12 = (4 << 5) | 12,
    aiResearchElectronics16 = (4 << 5) | 16,
    aiResearchElectronics17 = (4 << 5) | 17,
    aiResearchElectronics19 = (4 << 5) | 19,
    aiResearchElectronics21 = (4 << 5) | 21,
    aiResearchElectronics26 = (4 << 5) | 26,
    aiResearchBiotechnology3 = (5 << 5) | 3,
    aiResearchBiotechnology4 = (5 << 5) | 4,
    aiResearchBiotechnology6 = (5 << 5) | 6,
    aiResearchBiotechnology7 = (5 << 5) | 7,
    aiResearchBiotechnology9 = (5 << 5) | 9,
    aiResearchBiotechnology10 = (5 << 5) | 10,
    aiResearchBiotechnology11 = (5 << 5) | 11,
    aiResearchBiotechnology12 = (5 << 5) | 12,
    aiResearchBiotechnology18 = (5 << 5) | 18,
    aiResearchBiotechnology26 = (5 << 5) | 26,
};
typedef uint16_t AiResearchTarget;

// AiRace identifies a computer player's race and the AI routine that plays it.
enum AiRace {
    idAiRobotoid = 0,
    idAiTurinDrone = 1,
    idAiAutomitron = 2,
    idAiRototill = 3,
    idAiCybertron = 4,
    idAiMacinti = 5,
    idAiRandom = 6, // replaced with a random race when the game is created
    idAiMaid = 7,   // housekeeping AI that runs a human player's empire
};
typedef uint16_t AiRace;

// MineFieldType is a minefield's kind.
enum MineFieldType {
    mineStandard = 0,
    mineHeavy = 1,
    mineSpeedBump = 2,
};
typedef uint16_t MineFieldType;

// ScanView is the scanner's view mode, the low nibble of grbitScan.
enum ScanView {
    scanViewNormal = 0,
    scanViewSurfaceMinerals = 1,
    scanViewMineralConc = 2,
    scanViewPlanetValue = 3,
    scanViewPopulation = 4,
    scanViewNoPlayerInfo = 5,
};
typedef uint16_t ScanView;

// GrbitScan holds the scanner's overlay and filter toggles above the view mode
// in grbitScan.
enum GrbitScan {
    grbitScanViewMask = 0x000f, // ScanView
    grbitScanAddWaypoints = 0x0010,
    grbitScanCoverage = 0x0020,
    grbitScanMineFields = 0x0040,
    grbitScanFleetPaths = 0x0080,
    grbitScanIdleFleets = 0x0100,
    grbitScanDesignFilter = 0x0200,
    grbitScanPlanetNames = 0x0400,
    grbitScanEnemyFilter = 0x0800,
    grbitScanShipCounts = 0x1000,
    grbitScanPlayerColors = 0x2000,
    grbitScanToggleMask = 0x3ff0,
};
typedef uint16_t GrbitScan;

// ToolbarButton is a scanner toolbar button; the negative values are layout
// entries of vrgTBBtn.
enum ToolbarButton {
    tbScannerRange = -3, // scanner range readout
    tbSpacer = -2,
    tbSeparator = -1,
    tbNormalView = 0,
    tbSurfaceMineralView = 1,
    tbMineralConcView = 2,
    tbPlanetValueView = 3,
    tbPopulationView = 4,
    tbNoPlayerInfoView = 5,
    tbAddWaypoints = 6,
    tbScannerCoverage = 7,
    tbMineFields = 8,
    tbFleetPaths = 9,
    tbIdleFleets = 10,
    tbPlanetNames = 11,
    tbShipDesignFilter = 12,
    tbShipDesignFilterMenu = 13,
    tbEnemyClassFilter = 14,
    tbEnemyClassFilterMenu = 15,
    tbZoomMenu = 16,
    tbShipCounts = 17,
};
typedef int16_t ToolbarButton;

// UniverseSize is the galaxy size; its width is 400 * (size + 1) light years.
enum UniverseSize {
    sizeTiny = 0,
    sizeSmall = 1,
    sizeMedium = 2,
    sizeLarge = 3,
    sizeHuge = 4,
};
typedef uint16_t UniverseSize;

// UniverseDensity is the galaxy's planet density.
enum UniverseDensity {
    densitySparse = 0,
    densityNormal = 1,
    densityDense = 2,
    densityPacked = 3,
};
typedef uint16_t UniverseDensity;

// StartDistance is the distance between players' homeworlds.
enum StartDistance {
    startDistClose = 0,
    startDistModerate = 1,
    startDistFarther = 2,
    startDistDistant = 3,
};
typedef uint16_t StartDistance;

// MsgGoto is a message's goto target: a planet id (>= 0), a fleet id, a
// battle id | 0x4000, a part as 0xc000 | (hst bit << 8) | iItem, or one of
// these.
enum MsgGoto {
    gotoBattleReport = -7,
    gotoThing = -6, // the thing id is the message's first parameter
    gotoSerialNumber = -5,
    gotoScore = -4,
    gotoShipDesign = -3,
    gotoResearch = -2,
    gotoNone = -1,
    gotoBattle = 0x4000,
    gotoRelations = 0x4800,
};
typedef int16_t MsgGoto;

// MdMsgObj is what the message window's goto button opens.
enum MdMsgObj {
    mdMsgObjNone = 0,
    mdMsgObjPlanet = 1,
    mdMsgObjFleet = 2,
    mdMsgObjResearch = 3,
    mdMsgObjPart = 4,
    mdMsgObjShipDesign = 5,
    mdMsgObjBattle = 6,
    mdMsgObjRelations = 7,
    mdMsgObjScore = 8,
    mdMsgObjSerialNumber = 9,
    mdMsgObjThing = 10,
    mdMsgObjBattleReport = 11,
};
typedef uint16_t MdMsgObj;

// TileBits selects the planet or fleet detail tiles DrawPlanShip draws. A bit
// shared by both tile sets is named for both.
enum TileBits {
    tileMineralsOrCargo = 0x0001,
    tileShipList = 0x0004,
    tilePlanetStats = 0x0008,
    tileFleetOrders = 0x0020,
    tileProductionOrOrbit = 0x0040,
    tileBitmap = 0x0080,
    tileStarbaseOrWaypoint = 0x0100,
    tileFleetComp = 0x0200,
    tileAll = 0x0fff,
    tileMinimized = 0x4000,
    tileErase = 0x8000,
};
typedef uint16_t TileBits;

// ReportType is a report dialog's report.
enum ReportType {
    rptPlanets = 0,
    rptFleets = 1,
    rptEnemyFleets = 2,
    rptBattles = 3,
};
typedef uint16_t ReportType;

// PlanetReportColumn is a column of the planet report.
enum PlanetReportColumn {
    colPlanetName = 0,
    colPlanetStarbase = 1,
    colPlanetPopulation = 2,
    colPlanetCapacity = 3,
    colPlanetValue = 4,
    colPlanetProduction = 5,
    colPlanetMines = 6,
    colPlanetFactories = 7,
    colPlanetDefense = 8,
    colPlanetMinerals = 9,
    colPlanetMiningRate = 10,
    colPlanetMinConc = 11,
    colPlanetResources = 12,
    colPlanetDriverDest = 13,
    colPlanetRoutingDest = 14,
    colPlanetCount = 15,
};
typedef uint16_t PlanetReportColumn;

// FleetReportColumn is a column of the fleet report.
enum FleetReportColumn {
    colFleetName = 0,
    colFleetId = 1,
    colFleetLocation = 2,
    colFleetDestination = 3,
    colFleetEta = 4,
    colFleetTask = 5,
    colFleetFuel = 6,
    colFleetCargo = 7,
    colFleetComposition = 8,
    colFleetCloak = 9,
    colFleetBattlePlan = 10,
    colFleetMass = 11,
    colFleetCount = 12,
};
typedef uint16_t FleetReportColumn;

// EnemyFleetReportColumn is a column of the other players' fleet report.
enum EnemyFleetReportColumn {
    colEnemyFleetName = 0,
    colEnemyFleetId = 1,
    colEnemyFleetLocation = 2,
    colEnemyFleetWarp = 3,
    colEnemyFleetMass = 4,
    colEnemyFleetComposition = 5,
    colEnemyFleetShips = 6,
    colEnemyFleetUnarmed = 7,
    colEnemyFleetScout = 8,
    colEnemyFleetWarship = 9,
    colEnemyFleetBomber = 10,
    colEnemyFleetUtility = 11,
    colEnemyFleetCount = 12,
};
typedef uint16_t EnemyFleetReportColumn;

// BattleReportColumn is a column of the battle report.
enum BattleReportColumn {
    colBattleLocation = 0,
    colBattleStarbase = 1,
    colBattleSides = 2,
    colBattleUnits = 3,
    colBattleOurs = 4,
    colBattleTheirs = 5,
    colBattleUnarmed = 6,
    colBattleScout = 7,
    colBattleWarship = 8,
    colBattleBomber = 9,
    colBattleUtility = 10,
    colBattleOurDead = 11,
    colBattleTheirDead = 12,
    colBattleOursLeft = 13,
    colBattleTheirsLeft = 14,
    colBattleCount = 15,
};
typedef uint16_t BattleReportColumn;

// WindowLayout is the main window layout, chosen by screen size.
enum WindowLayout {
    layoutLarge = 0,
    layoutMedium = 1,
    layoutSmall = 2,
};
typedef uint16_t WindowLayout;

// ScanZoom is the scanner zoom level.
enum ScanZoom {
    zoom25 = -4,
    zoom38 = -3,
    zoom50 = -2,
    zoom75 = -1,
    zoom100 = 0,
    zoom125 = 1,
    zoom150 = 2,
    zoom200 = 3,
    zoom400 = 4,
};
typedef int16_t ScanZoom;

// MdMark is the turn-file flag FMarkFile sets and FCheckFile tests.
enum MdMark {
    mdMarkInUse = 1,
    mdMarkDone = 2,
    mdMarkMulti = 4,
    mdMarkAi = 8, // the player is run by the housekeeping AI
};
typedef uint16_t MdMark;

// HostTimer is the job of the host-mode timer.
enum HostTimer {
    hostTimerAutoGen = 13,   // poll for turns to generate
    hostTimerWaitTurn = 14,  // wait for the next turn after submitting
    hostTimerTurnReady = 15, // flash the window when a new turn is ready
};
typedef uint16_t HostTimer;

// ProgressStep is an UpdateProgressGauge argument that advances the gauge
// instead of setting it.
enum ProgressStep {
    progressStep1 = -927,
    progressStep4 = -926,
};
typedef int16_t ProgressStep;

// AddItemMode is where AddItemToQueue puts an item in the production queue.
enum AddItemMode {
    addItemFront = 0,
    addItemEnd = 1,
    addItemReplace = 2, // clear the queue first
};
typedef uint16_t AddItemMode;

// MainMenu is a top-level menu's position in the frame menu bar.
enum MainMenu {
    menuFile = 0,
    menuView = 1,
    menuTurn = 2,
    menuCommands = 3,
    menuReport = 4,
    menuHelp = 5,
};
typedef uint16_t MainMenu;

// TutorShipBuilderAction is the ship designer button FTutorialEnabledShipBuilder
// checks against the tutorial's current step.
enum TutorShipBuilderAction {
    tutsbDelete = 0,
    tutsbCopy = 1,
    tutsbEdit = 2,
    tutsbAccept = 3,     // OK on an edited design
    tutsbCancelEdit = 4, // Cancel on an edited design
};
typedef uint16_t TutorShipBuilderAction;

// HelpContextId is a topic of stars!.hlp, as WinHelp's HELP_CONTEXT data
// selects it: the [MAP] numbers of the help file's |CTXOMAP, named by topic
// title. Many share their number with the dialog control they explain.
// Generated by scripts/hlp-context-enum.py.
enum HelpContextId {
    idhStarsPlayersGuideContents = 1, // Stars! Player's Guide - Contents
    idhIntroductionAndPlayerSupport = 2, // Introduction and Player Support
    idhTheStarsScreen = 3, // The Stars! Screen
    idhStarsDialogs = 4, // Stars! dialogs
    idhPlayingStars = 6, // Playing Stars!
    idhTheGuts = 7, // The Guts
    idhHowTo = 8, // How To...
    idhSetupAndHosting = 9, // Setup and Hosting
    idhNewGameSetupBasic = 1002, // New Game Setup (Basic)
    idhBeginTutorial = 1003, // Begin Tutorial
    idhDifficultyLevel = 1004, // Difficulty Level
    idhTinyUniverse = 1005, // Tiny Universe
    idhSmallUniverse = 1006, // Small Universe
    idhMediumUniverse = 1007, // Medium Universe
    idhLargeUniverse = 1008, // Large Universe
    idhHugeUniverse = 1009, // Huge Universe
    idhPlayerRace = 1010, // Player Race
    idhNewGameSetupAdvanced = 1011, // New Game Setup (Advanced)
    idhStep1SpecifyingTheUniverse = 1012, // Step 1: Specifying the Universe
    idhGameName = 1013, // Game Name
    idhDensity = 1014, // Density
    idhPlayerPositions = 1015, // Player Positions
    idhBeginnerUnlimitedMinerals = 1016, // Beginner: Unlimited Minerals
    idhSlowerTechAdvances = 1017, // Slower Tech Advances
    idhComputerPlayersFormAlliances = 1018, // Computer Players Form Alliances
    idhAcceleratedBBSPlay = 1019, // Accelerated BBS Play
    idhStep2SpecifyingThePlayers = 1020, // Step 2: Specifying the Players
    idhStep3VictoryConditions = 1021, // Step 3: Victory Conditions
    idhCustomRaceWizard = 1022, // Custom Race Wizard
    idhStep1BasicDefinition = 1023, // Step 1: Basic Definition
    idhRaceNameAndPassword = 1024, // Race Name and Password
    idhPredefinedRaces = 1025, // Predefined Races
    idhLeftoverAdvantagePointsSurfaceMinerals = 1026, // Leftover Advantage Points -- Surface Minerals
    idhLeftoverAdvantagePointsMines = 1028, // Leftover Advantage Points -- Mines
    idhLeftoverAdvantagePointsFactories = 1029, // Leftover Advantage Points -- Factories
    idhLeftoverAdvantagePointsDefenses = 1030, // Leftover Advantage Points -- Defenses
    idhRaceIcon = 1031, // Race Icon
    idhStep2PrimaryRacialTraits = 1032, // Step 2: Primary Racial Traits
    idhHyperExpansion = 1033, // Hyper-Expansion
    idhSuperStealth = 1034, // Super-Stealth
    idhWarMonger = 1035, // War Monger
    idhInnerStrength = 1036, // Inner-Strength
    idhSpaceDemolition = 1037, // Space Demolition
    idhPacketPhysics = 1038, // Packet Physics
    idhInterstellarTraveller = 1039, // Interstellar Traveller
    idhJackOfAllTrades = 1040, // Jack of All Trades
    idhStep3LesserTraitsPlayerRace = 1041, // Step 3: Lesser Traits (Player Race)
    idhImprovedFuelEfficiency = 1042, // Improved Fuel Efficiency
    idhTotalTerraforming1043 = 1043, // Total Terraforming
    idhImprovedStarbases = 1044, // Improved Starbases
    idhGeneralizedResearch = 1045, // Generalized Research
    idhMineralAlchemy1046 = 1046, // Mineral Alchemy
    idhNoRamscoopEngines = 1047, // No Ramscoop Engines
    idhCheapEngines = 1048, // Cheap Engines
    idhOnlyBasicRemoteMining = 1049, // Only Basic Remote Mining
    idhNoAdvancedScanners = 1050, // No Advanced Scanners
    idhLowStartingPopulation = 1051, // Low Starting Population
    idhRegeneratingShields = 1052, // Regenerating Shields
    idhStep4PopulationGrowthFactors = 1053, // Step 4: Population Growth Factors
    idhGrowthConditions = 1054, // Growth Conditions
    idhMaximumPopulationGrowth = 1055, // Maximum Population Growth
    idhStep5PopulationEfficiencyPlayerRace = 1056, // Step 5: Population Efficiency (Player Race)
    idhStep6ResearchCostsPlayerRace = 1057, // Step 6: Research Costs (Player Race)
    idhFinishAndSave = 1058, // Finish and Save
    idhProductionDialog = 1059, // Production Dialog
    idhProductionInventory = 1060, // Production Inventory
    idhProductionQueue = 1061, // Production Queue
    idhShipDesigner = 1066, // Ship Designer
    idhShipSchematic = 1068, // Ship Schematic
    idhShipComponentList = 1069, // Ship Component List
    idhResearchDialog = 1070, // Research Dialog
    idhTechnologyStatus = 1071, // Technology Status
    idhExpectedResearchBenefits = 1072, // Expected Research Benefits
    idhCurrentlyResearching = 1073, // Currently Researching
    idhResourceAllocation = 1074, // Resource Allocation
    idhCargoTransferDialogs = 1075, // Cargo Transfer Dialogs
    idhBetweenYourPlanetAndYourFleet = 1076, // Between Your Planet And Your Fleet
    idhBetweenYourFleetAndAPlanet = 1077, // Between Your Fleet and a Planet
    idhBetweenYourFleets = 1078, // Between Your Fleets
    idhBetweenYourFleetAndAnOpponentsFleet = 1079, // Between Your Fleet and an Opponents Fleet
    idhShipTransferDialog = 1080, // Ship Transfer Dialog
    idhBattlePlansDialog = 1081, // Battle Plans Dialog
    idhBattleVCR = 1082, // Battle VCR
    idhPlayerRelationsDialog = 1083, // Player Relations Dialog
    idhChangePassword = 1084, // Change Password
    idhFindPlanetOrFleet = 1085, // Find Planet or Fleet
    idhHostModeDialog = 1088, // Host Mode Dialog
    idhRenameFleetDialog = 1095, // Rename Fleet dialog
    idhAdvancedRemoteMining = 1096, // Advanced Remote Mining
    idhPublicPlayerScores = 1097, // Public Player Scores
    idhCustomZipOrdersDialog = 1098, // Custom Zip Orders dialog
    idhLeftoverAdvantagePointsMineralConcentration = 1099, // Leftover Advantage Points  Mineral Concentration
    idhClaimAdjuster = 1100, // Claim Adjuster
    idhAlternateReality = 1101, // Alternate Reality
    idhUltimateRecycling = 1102, // Ultimate Recycling
    idhBleedingEdgeTechnology = 1103, // Bleeding Edge Technology
    idhNoRandomEvents = 1104, // No Random Events
    idhCustomizeProductionTemplatesDialog = 1106, // Customize Production Templates dialog
    idhMergeFleetsDialog = 1107, // Merge Fleets dialog
    idhGalaxyClumping = 1108, // Galaxy Clumping
    idhScoreSheet = 1109, // Score sheet
    idhChangingTheBasicLayout = 1502, // Changing the Basic Layout
    idhShrinkingAndGrowingPanes = 1503, // Shrinking and Growing Panes
    idhMovingAndCollapsingTiles = 1504, // Moving and Collapsing Tiles
    idhCommandingAPlanet = 1505, // Commanding a Planet
    idhPlanetTile = 1506, // Planet Tile
    idhProductionTile = 1507, // Production Tile
    idhStatusTile = 1508, // Status Tile
    idhMineralsOnHandTile = 1509, // Minerals on Hand Tile
    idhFleetsInOrbitTile = 1510, // Fleets in Orbit Tile
    idhStarbaseTile = 1511, // Starbase Tile
    idhCommandingAFleet = 1512, // Commanding a Fleet
    idhFleetTile = 1513, // Fleet Tile
    idhLocationTile = 1514, // Location Tile
    idhFuelAndCargoTile = 1515, // Fuel and Cargo Tile
    idhFleetCompositionTile = 1516, // Fleet Composition Tile
    idhOtherFleetsHereTile = 1517, // Other Fleets Here Tile
    idhFleetWaypointsTile = 1518, // Fleet Waypoints Tile
    idhWaypointTaskTile = 1519, // Waypoint Task Tile
    idhTransport = 1520, // Transport
    idhColonize = 1522, // Colonize
    idhPatrol = 1523, // Patrol
    idhRemoteMining1524 = 1524, // Remote Mining
    idhScrapFleet = 1525, // Scrap Fleet
    idhSelectingAnObjectToCommand = 1526, // Selecting an Object to Command
    idhObtainingAPlanetOrFleetSummary = 1527, // Obtaining a Planet or Fleet Summary
    idhLayMineFields = 1528, // Lay Mine Fields
    idhMergeWithFleet = 1530, // Merge with Fleet
    idhRoute = 1531, // Route
    idhRemoteTerraforming1532 = 1532, // Remote Terraforming
    idhTransferFleet = 1533, // Transfer Fleet
    idhTheGutsOfCombat = 2008, // The Guts of Combat
    idhAboutTheBattleBoard = 2009, // About the Battle Board
    idhDamageRepair = 2013, // Damage Repair
    idhMovementInitiativeAndFiringInBattle = 2014, // Movement, Initiative and Firing in Battle
    idhWeaponProperties = 2015, // Weapon Properties
    idhTheGutsOfMassDrivers = 2016, // The Guts of Mass Drivers
    idhFilesUsedInStars = 2017, // Files Used in Stars!
    idhArmorShieldsAndDamage = 2018, // Armor, Shields and Damage
    idhTheGutsOfMinefields = 2022, // The Guts of Minefields
    idhAlternateRealityRaces = 2024, // Alternate Reality Races
    idhTheGutsOfCloaking = 2025, // The Guts of Cloaking
    idhAboutDataTables = 2501, // About Data Tables
    idhArmor = 2502, // Armor
    idhBeamWeapons = 2503, // Beam Weapons
    idhBombsTable = 2504, // Bombs table
    idhEnginesTable = 2506, // Engines table
    idhScannersTable = 2508, // Scanners table
    idhShieldsTable = 2509, // Shields table
    idhMiningTable = 2511, // Mining table
    idhTerraformingTable = 2513, // Terraforming table
    idhOrbitalDevicesTable = 2516, // Orbital Devices table
    idhElectricalDevicesTable = 2518, // Electrical Devices table
    idhMechanicalDevicesTable = 2519, // Mechanical Devices table
    idhMineLayingTable = 2520, // Mine Laying table
    idhPlanetaryInstallationsTable = 2521, // Planetary Installations table
    idhShipHullsTable = 2522, // Ship Hulls table
    idhStarbaseHullsTable = 2523, // Starbase Hulls table
    idhTorpedoesTable = 2524, // Torpedoes table
    idhPlanets3002 = 3002, // Planets
    idhYourHomeWorldAndOtherInhabitedPlanets = 3003, // Your Home World and Other Inhabited Planets
    idhPopulation = 3004, // Population
    idhMinerals = 3005, // Minerals
    idhMines = 3006, // Mines
    idhFactories = 3007, // Factories
    idhTerraforming = 3009, // Terraforming
    idhTypesOfTerraformingTechnology = 3011, // Types of Terraforming Technology
    idhTotalTerraforming3012 = 3012, // Total Terraforming
    idhBuildingPlanetaryDefenses = 3013, // Building Planetary Defenses
    idhPlanetBasedScanners = 3014, // Planet-based Scanners
    idhOrbitalDevices = 3015, // Orbital Devices
    idhStarbases = 3016, // Starbases
    idhStargates = 3017, // Stargates
    idhMassDriverBasics = 3018, // Mass Driver Basics
    idhProduction = 3019, // Production
    idhHowProductionWorks = 3020, // How Production Works
    idhAddingAnItemToTheProductionQueue = 3021, // Adding an Item to the Production Queue
    idhAddAnItemToTheTopOfTheQueue = 3022, // Add an item to the top of the queue
    idhAddAnItemToTheMiddleOfTheQueue = 3023, // Add an item to the middle of the queue
    idhAddAnItemToTheBottomOfTheQueue = 3024, // Add an item to the bottom of the queue
    idhMoveAnItemInTheQueue = 3025, // Move an item in the queue
    idhRemovingAnItemFromTheProductionQueue = 3026, // Removing an Item from the Production Queue
    idhClearingTheProductionQueue = 3027, // Clearing the Production Queue
    idhUnblockingAProductionQueue = 3028, // Unblocking a Production Queue
    idhAddingAutoBuildItemsToTheQueue = 3029, // Adding Auto Build Items to the Queue
    idhConditionsThatAffectProduction = 3031, // Conditions that Affect Production
    idhResearch = 3032, // Research
    idhFieldsOfStudy = 3033, // Fields of Study
    idhAllocatingResourcesForResearch = 3034, // Allocating Resources for Research
    idhTheCostOfResearch = 3035, // The Cost of Research
    idhDesigningShips = 3037, // Designing Ships
    idhHowToApproachShipDesign = 3038, // How to Approach Ship Design
    idhDesigningANewShipFromScratch = 3039, // Designing a New Ship from Scratch
    idhEditingAnExistingShipDesign = 3040, // Editing an Existing Ship Design
    idhDeletingAnExistingShipDesign = 3041, // Deleting an Existing Ship Design
    idhReachingTheMaximumNumberOfDesigns = 3042, // Reaching the Maximum Number of Designs
    idhCountingTheNumberOfShipDesigns = 3043, // Counting the Number of Ship Designs
    idhAddingShipBasedScanners = 3044, // Adding Ship-based Scanners
    idhAddingCloakingDevices = 3045, // Adding Cloaking Devices
    idhEngines = 3046, // Engines
    idhLearningAboutOtherPlayersHulls = 3047, // Learning About Other Player's Hulls
    idhManagingFleets = 3048, // Managing Fleets
    idhAssemblingFleets = 3049, // Assembling Fleets
    idhWarpSpeed = 3050, // Warp Speed
    idhFindingASingleFleet = 3051, // Finding a Single Fleet
    idhFindingASpecificFleetComposition = 3052, // Finding a Specific Fleet Composition
    idhSwitchingBetweenFleets = 3053, // Switching Between Fleets
    idhNamingFleets = 3054, // Naming Fleets
    idhUsingFuel = 3055, // Using Fuel
    idhRendezvousingFleets = 3056, // Rendezvousing Fleets
    idhTransferringCargo = 3057, // Transferring Cargo
    idhJettisoningCargo = 3058, // Jettisoning Cargo
    idhSplittingAndMergingFleets = 3059, // Splitting and Merging Fleets
    idhScrappingFleets = 3060, // Scrapping Fleets
    idhNavigation = 3061, // Navigation
    idhAddingFleetWaypoints = 3062, // Adding Fleet Waypoints
    idhMovingFleetWaypoints = 3063, // Moving Fleet Waypoints
    idhDeletingFleetWaypoints = 3064, // Deleting Fleet Waypoints
    idhStargateNavigation = 3065, // Stargate Navigation
    idhWormholeNavigation = 3066, // Wormhole Navigation
    idhColonization = 3068, // Colonization
    idhChoosingPlanetsToColonize = 3069, // Choosing Planets to Colonize
    idhColonizingAnUninhabitedPlanet = 3070, // Colonizing an Uninhabited Planet
    idhShuttlingColonistsWithFreighters = 3071, // Shuttling Colonists with Freighters
    idhHeyThatPlanetsAlreadyInhabited = 3072, // Hey, that Planet's Already Inhabited!
    idhMining = 3073, // Mining
    idhMiningColonizedWorlds = 3074, // Mining Colonized Worlds
    idhRemoteMining3075 = 3075, // Remote Mining
    idhCreatingARobotMiningFleet = 3076, // Creating a Robot Mining Fleet
    idhTransportingFreight = 3078, // Transporting Freight
    idhShippingFreight = 3079, // Shipping Freight
    idhFlingingMassPackets = 3080, // Flinging Mass Packets
    idhTheBasicsOfCombat = 3081, // The Basics of Combat
    idhFleetToFleetCombat = 3082, // Fleet-to-fleet Combat
    idhBombingPlanets = 3083, // Bombing Planets
    idhGroundCombat = 3084, // Ground Combat
    idhLayingMinefields = 3085, // Laying Minefields
    idhStarbaseCombat = 3086, // Starbase Combat
    idhDeclaringEnemiesAndFriends = 3087, // Declaring Enemies and Friends
    idhBattlePlans = 3088, // Battle Plans
    idhMakingANewBattlePlan = 3089, // Making a New Battle Plan
    idhReviewABattleInSpace = 3092, // Review a Battle in Space
    idhViewingEnemyFleetsInTheSummaryPane = 3093, // Viewing Enemy Fleets in the Summary Pane
    idhViewingEnemyShipDesigns = 3094, // Viewing Enemy Ship Designs
    idhPatroling = 3095, // Patroling
    idhScanningAndCloaking = 3096, // Scanning and Cloaking
    idhSelectingFleetsInTheScannerPane = 3097, // Selecting Fleets in the Scanner Pane
    idhScanningPlanets = 3098, // Scanning Planets
    idhCloakingOrHidingFromAnOpponentsScanners = 3099, // Cloaking, or Hiding from an Opponents Scanners
    idhDetectingAnOpponentsFleets = 3100, // Detecting an Opponents Fleets
    idhScannerTechnology = 3102, // Scanner Technology
    idhCreatingACustomTransportZipOrder = 3103, // Creating a Custom Transport Zip Order
    idhChangingTheContentsOfABattlePlan = 3105, // Changing the Contents of a Battle Plan
    idhHowToTerraform = 3106, // How to Terraform
    idhMinefields = 3107, // Minefields
    idhSweepingMinefields = 3108, // Sweeping Minefields
    idhPiratingUsingStealthBasedScanners = 3114, // Pirating using Stealth-based Scanners
    idhDiplomacyAndTrade = 3115, // Diplomacy and Trade
    idhRoutingFleets = 3116, // Routing Fleets
    idhProductionTemplates = 3117, // Production Templates
    idhTargeting = 3118, // Targeting
    idhTactics = 3119, // Tactics
    idhRemoteTerraforming3120 = 3120, // Remote Terraforming
    idhRemotelyDetonatingMinefields = 3121, // Remotely Detonating Minefields
    idhSalvageFromSpaceBattles = 3122, // Salvage from Space Battles
    idhPlanetReports = 3123, // Planet Reports
    idhChangingTheOrderOfPlanetsInTheProductionDialog = 3124, // Changing the Order of Planets in the Production dialog
    idhViewingStarsTechnology = 3125, // Viewing Stars! Technology
    idhReportsOnYourFleets = 3126, // Reports on Your Fleets
    idhJointVenturesInRemoteMining = 3127, // Joint Ventures in Remote Mining
    idhMineralPacketBombardment = 3128, // Mineral Packet Bombardment
    idhBattleReports = 3129, // Battle Reports
    idhFleetReportsOnEnemiesAndOtherPlayers = 3130, // Fleet Reports on Enemies and other Players
    idhReports = 3131, // Reports
    idhPrintingAMapOfTheUniverse = 3132, // Printing a Map of the Universe
    idhWhatYouNeedToPlay = 3501, // What You Need to Play
    idhTuningStarsForYourScreenResolution = 3502, // Tuning Stars for Your Screen Resolution
    idhStartingASinglePlayerGame = 3504, // Starting a Single Player Game
    idhHostingAMultiPlayerGame = 3505, // Hosting a Multi-Player Game
    idhWhatEachPlayerNeedsToDo = 3507, // What Each Player Needs to Do
    idhHostingANetworkGame = 3508, // Hosting a Network Game
    idhBeingAbsentFromPlay = 3511, // Being Absent from Play
    idhWinning = 3512, // Winning
    idhOptionsForLaunchingStars = 3513, // Options for Launching Stars!
    idhExitingStars = 3514, // Exiting Stars!
    idhCopyProtection = 3516, // Copy Protection
    idhPlayingWithACustomRace = 3517, // Playing with a Custom Race
    idhCreatingAndSavingACustomRace = 3518, // Creating and Saving a Custom Race
    idhAddingAnExistingRaceToANewGame = 3520, // Adding an Existing Race to a New Game
    idhEditingAnExistingCustomRace = 3521, // Editing an Existing Custom Race
    idhSubmittingBugReports = 3523, // Submitting Bug Reports
    idhHostingModemAndEmailGames = 3526, // Hosting Modem and Email Games
    idhSavingYourGameWhatItMeans = 3528, // Saving Your Game--What it Means
    idhCreatingAPassword = 3529, // Creating a Password
    idhCreatingAUniverseFromTheCommandLine = 3531, // Creating a Universe from the Command Line
    idhStarsWebSite = 3532, // Stars! Web Site
    idhPlayingTheTutorial = 3533, // Playing the Tutorial
    idhReplayingAPreviousTurn = 3534, // Replaying a Previous Turn
    idhHostingHotSeatGames = 3535, // Hosting Hot-Seat Games
    idhAddingExpansionPlayers = 3538, // Adding Expansion Players
    idhOrderingTheRetailVersionOfStars = 4001, // Ordering the Retail Version of Stars!
    idhWelcomeToStars = 4501, // Welcome to Stars!
    idhMultiPlayerGames = 4506, // Multi-player games
    idhAddedCostOfResearch = 5501, // Added Cost of Research
    idhAIs = 5502, // AIs
    idhAIsAndAdvantagePoints = 5503, // AIs and Advantage Points
    idhAnnualGrowthRate = 5504, // Annual Growth Rate
    idhBestWarpSpeed = 5505, // Best Warp Speed
    idhDefensesAndInvadingTroops = 5507, // Defenses and Invading Troops
    idhDisengaging = 5508, // Disengaging
    idhEnergySourcesForStarships = 5509, // Energy Sources for Starships
    idhFactory = 5510, // Factory
    idhFibonacciSeries = 5511, // Fibonacci Series
    idhFleetColors = 5512, // Fleet Colors
    idhFuelPoorPlanets = 5513, // Fuel Poor Planets
    idhLoadFromFleet = 5514, // Load from Fleet
    idhLoadOptimal = 5515, // Load Optimal
    idhLosingColonists = 5516, // Losing Colonists
    idhMaximumShipDesignsAndShips = 5517, // Maximum Ship Designs and Ships
    idhMine = 5519, // Mine
    idhMineralAlchemy5520 = 5520, // Mineral Alchemy
    idhOrbitRingColors = 5521, // Orbit Ring Colors
    idhPlanetPenetratingScanners = 5522, // Planet penetrating scanners
    idhResources = 5523, // Resources
    idhRoundOfBattle = 5524, // Round of Battle
    idhShipClasses = 5527, // Ship Classes
    idhToken = 5528, // Token
    idhWaitForFleet = 5529, // Wait for Fleet
    idhCollateralDamage = 5530, // Collateral Damage
    idhDefineInitiative = 5531, // define Initiative
    idhViewsInTheScannerPane = 5532, // Views in the Scanner pane
    idhBattleSpeed = 5533, // Battle Speed
    idhCapitalShip = 5534, // Capital Ship
    idhDialogsAndDisplays = 5535, // Dialogs and Displays
    idhKeyboardShortcuts = 6001, // Keyboard Shortcuts
    idhRating = 6501, // Rating
    idhRaceDescriptionFileNameRNFiles = 6502, // Race Description file -- name.rN files
    idhHostFile = 6503, // Host file
    idhPlayerLogFile = 6504, // Player Log file
    idhRaceFile = 6505, // Race file
    idhRaceFileHowToPopup = 12003, // Race File How to Popup
    idhHowToManageProduction = 12004, // How to Manage Production
    idhHowToAssignWaypoints = 12005, // How to Assign Waypoints
    idhHowToManageBattles = 12006, // How to Manage Battles
    idhHowToDesignShips = 12007, // How to Design Ships
    idhHowToColonize = 12008, // How to Colonize
    idhPopupPlanetTiles = 12009, // Popup Planet Tiles
    idhPopupFleetTiles = 12010, // Popup Fleet Tiles
    idhPopupWaypointTasks = 12011, // Popup Waypoint Tasks
    idhPopupScannerTopics = 12012, // popup Scanner Topics
    idhPopupBattleDetails = 12015, // popup Battle Details
    idhPopupTables = 12017, // popup Tables
    idhPlanets12018 = 12018, // Planets
    idhPopupTerraforming = 12020, // popup Terraforming
    idhPopupOrbitalDevices = 12021, // popup Orbital Devices
    idhPopupProduction = 12022, // popup Production
    idhPopupResearch = 12023, // popup Research
    idhPopupShipDesign = 12024, // popup Ship Design
    idhPopupFleetManagement = 12025, // popup Fleet Management
    idhPopupNavigation = 12026, // popup Navigation
    idhPopupColonization = 12027, // popup Colonization
    idhPopupMining = 12028, // popup Mining
    idhPopupFreight = 12029, // popup Freight
    idhPopupCombat = 12030, // popup Combat
    idhPopupScanning = 12033, // popup Scanning
    idhMenuCustomRaceWizardSteps = 12040, // menu Custom Race Wizard Steps
    idhMenuAdvancedSetup = 12041, // menu Advanced Setup
    idhHowToManageFleets = 12042, // How to Manage Fleets
    idhScannerViews = 12043, // Scanner Views
    idhPopupResDialog = 12046, // popup res dialog
    idhPopupMinefields = 12048, // popup Minefields
    idhPopupBattlePlans = 12049, // popup Battle Plans
    idhPopupMessagesPane = 12050, // popup Messages Pane
    idhHostingMultiPlayerGames = 12051, // Hosting Multi-Player Games
    idhMessagesPane = 14001, // Messages Pane
    idhTheGotoPreviousAndNextButtons = 14002, // The Goto, Previous and Next Buttons
    idhSendingMessagesToOtherPlayers = 14003, // Sending Messages to other Players
    idhFilteredMessageCheckbox = 14004, // Filtered Message Checkbox
    idhFilteringMessageTypes = 14005, // Filtering Message Types
    idhScannerPane = 14006, // Scanner Pane
    idhChoosingYourViewOfTheUniverse = 14008, // Choosing Your View of the Universe
    idhNormalView = 14009, // Normal View
    idhPlanetValueView = 14010, // Planet Value View
    idhMineralsAtPlanetView = 14011, // Minerals at Planet View
    idhPopulationView = 14012, // Population View
    idhNoPlayerInformationView = 14013, // No Player Information View
    idhAddWaypointsOverlay = 14014, // Add Waypoints Overlay
    idhRadarOverlay = 14015, // Radar Overlay
    idhFleetOverlay = 14016, // Fleet Overlay
    idhShipFilterOverlay = 14017, // Ship Filter Overlay
    idhPlanetNamesOverlay = 14018, // Planet Names Overlay
    idhStatusBar = 14019, // Status Bar
    idhZooming = 14022, // Zooming
    idhSelectionSummaryPane = 14023, // Selection Summary pane
    idhPlanetSummary = 14024, // Planet Summary
    idhMultipleObjectsIndicator = 14025, // Multiple Objects Indicator
    idhReportVintage = 14026, // Report Vintage
    idhPopulationStatus = 14027, // Population Status
    idhSelectionValue = 14028, // Selection Value
    idhStarbaseIndicator = 14029, // Starbase Indicator
    idhEnvironmentGraph = 14030, // Environment Graph
    idhMineralContentGraph = 14031, // Mineral Content Graph
    idhFleetSummary = 14032, // Fleet Summary
    idhMineFieldsOverlay = 14033, // Mine Fields overlay
    idhKeyToTheScanner = 14034, // Key to the Scanner
    idhIdleFleetsOverlay = 14035, // Idle Fleets Overlay
    idhQuickReferenceToScannerUsage = 14036, // Quick Reference to Scanner Usage
    idhFilterEnemyShipsOverlay = 14037, // Filter Enemy Ships overlay
    idhMineralConcentrationView = 14038, // Mineral Concentration view
    idhShipCountOverlay = 14039, // Ship Count overlay
    idhRainbowEffect = 14040, // Rainbow Effect
    idhDisplayingPlayerColors = 14041, // Displaying Player Colors
    idhTroubleshootingWhatToDoWhenTheShipHitsTheFan = 52224, // Troubleshooting: What to Do when the Ship Hits the Fan
};
typedef uint16_t HelpContextId;

// HullCategory is a hull's role, as the fleet and battle reports group
// ships: unarmed hulls are colony ships, freighters, miners and fuel
// transports.
enum HullCategory {
    hullCatColony = 0,
    hullCatFreighter = 1,
    hullCatScout = 2,         // scout, frigate, destroyer
    hullCatWarship = 3,       // cruiser through dreadnought
    hullCatUtility = 4,       // privateer, rogue, galleon, mine layers, Nubian, morphs
    hullCatBomber = 5,
    hullCatMiner = 6,
    hullCatFuelTransport = 7,
};
typedef uint16_t HullCategory;

// HullAttack is how a hull fights; battle code treats any nonzero value as armed.
enum HullAttack {
    hullAttackNone = 0,
    hullAttackLight = 1,  // scout, frigate, destroyer, privateer
    hullAttackHeavy = 2,  // cruisers and up, rogue, galleon, Nubian, morphs
    hullAttackBomber = 3,
};
typedef uint16_t HullAttack;

// BeamAbility is a beam weapon's grfAbilities.
enum BeamAbility {
    beamSapper = 0x0001,  // damages shields only
    beamGatling = 0x0002, // hits every target in range
};
typedef uint16_t BeamAbility;

// EngineAbility is a special engine's grfAbilities, which selects its
// description and restrictions.
enum EngineAbility {
    engineAbilityNone = 0,
    engineSettlersDelight = 1,      // mini-colonizer hulls only
    engineRadiatingRamScoop = 2,    // radiation kills colonists
    engineFuelMizer = 3,            // requires Improved Fuel Efficiency
    engineGalaxyScoop = 4,          // requires Improved Fuel Efficiency
    engineInterspace10 = 5,         // unavailable with No Ram Scoop Engines
    engineEnigmaPulsar = 6,         // origin unknown
};
typedef uint16_t EngineAbility;

// ScannerAbility is a scanner's grfAbilities.
enum ScannerAbility {
    scannerAbilityNone = 0,
    scannerPenetrating50 = 1,
    scannerPenetrating100 = 2,
    scannerPenetrating200 = 3,
    scannerSteals = 4, // Super Stealth mineral thieves
};
typedef uint16_t ScannerAbility;

// PaneSplitter is the set of frame splitter bars a point is on.
enum PaneSplitter {
    splitVertical = 0x0001, // between the left panes and the scanner
    splitMessages = 0x0002, // below the messages pane
    splitLower = 0x0004,    // the second horizontal bar
};
typedef uint16_t PaneSplitter;

// RaceWizardPage is the race wizard page shown, in wizard order.
enum RaceWizardPage {
    rwPageNone = -1,
    rwPageRace = 1,          // IDD_RACE_WIZARD_1
    rwPagePrimaryTrait = 2,  // IDD_RACE_WIZARD_4
    rwPageLesserTraits = 3,  // IDD_RACE_WIZARD_5
    rwPageHabitability = 4,  // IDD_RACE_WIZARD_2
    rwPageEconomy = 5,       // IDD_RACE_WIZARD_3
    rwPageResearch = 6,      // IDD_RACE_WIZARD_6
};
typedef int16_t RaceWizardPage;

// WizardButton is a wizard page's result: the index of the button pressed
// in rgidRaceBtn.
enum WizardButton {
    wizCancel = 0,
    wizBack = 1,
    wizNext = 2,
    wizFinish = 3,
    wizHelp = 4,
};
typedef uint16_t WizardButton;

// PacketDecay is how many warps a mineral packet was flung over its
// driver's rating, and so how fast it decays.
enum PacketDecay {
    decayNone = 0,
    decay10Pct = 1,
    decay25Pct = 2,
    decay50Pct = 3,
};
typedef uint16_t PacketDecay;

// CompassDir is a direction on the map, counterclockwise from east; y
// grows southward.
enum CompassDir {
    dirEast = 0,
    dirNorthEast = 1,
    dirNorth = 2,
    dirNorthWest = 3,
    dirWest = 4,
    dirSouthWest = 5,
    dirSouth = 6,
    dirSouthEast = 7,
};
typedef uint16_t CompassDir;

#endif
