#ifndef STARS_DECOMPILED_TURN2_H
#define STARS_DECOMPILED_TURN2_H

#include <stdint.h>
#include <windows.h>

void    Produce();
int16_t CBuildProdItem(PLANET *lppl, PROD *lpprod, PROD *pprodPartial, int32_t *rgRes, int16_t fAlchemy, int16_t *pmdStatus, int16_t fCalcOnly);
int16_t FBuildObject(PLANET *lppl, GrobjClass grobj, int16_t iItem, int16_t cBuilt, int32_t *rgMinerals);
void    CreateShip(int16_t iPlr, FLEET *lpfl, int16_t ishdef, int16_t cShip);
void    RandomEvents();
void    TransferToOthers();
void    DropColonists();
void    HealShips();
void    AutoTerraform();
void    RemoteTerraforming();
int16_t FQueueColonistDrop(FLEET *lpfl, PLANET *lppl, int32_t cColonists);
void    UpdatePopulations();
void    UpdateGuesses();
void    MineMinerals();
void    MeteorStrike();
void    TossNonAutoBuildItems(PLANET *lppl);
void    PlanetaryClimateChange();
void    DiscoverNewMinerals();
void    MysteryTrader();
void    UpdatePlayerScores();
void    CreateBackupDir();
int16_t FPacketDecay(THING *lpth, int16_t pctRate);
void    ThingDecay();
void    UnmarkMineFields();
void    SweepForMines();
void    BreedColonistsInTransit();
void    UpdateResearchStatus(int16_t fUsePool);
int16_t IBestRemoteTerra(PLANET *lppl, int16_t iplr, int16_t fHelp);

#endif
