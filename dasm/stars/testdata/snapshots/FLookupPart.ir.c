int16_t FLookupPart(PART *ppart) {
    int16_t raMajor;
    HS      hs;

L_524e:
    raMajor = GetRaceStat(rgplr[idPlayer], rsMajorAdv);
    hs.grhst = ppart->hs.grhst;
    HIWORD(hs) = HIWORD(ppart->hs);
    goto L_6019;

L_5287:
    return 0x0;

L_528d:
    if ((hs.iItem < iengineCount))
        goto L_52a1;
    else
        goto L_529b;

L_529b:
    return 0x0;

L_52a1:
    ppart->pengine = &(rgengine[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_52c6;

L_52c6:
    if ((hs.iItem != iengineSettlersDelight))
        goto L_52e6;
    else
        goto L_52d4;

L_52d4:
    if ((raMajor == 0x0))
        goto L_52e6;
    else
        goto L_52dd;

L_52dd:
    return 0xffff;

L_52e6:
    if ((hs.iItem < iengineSubGalacticFuelScoop))
        goto L_5302;
    else
        goto L_52f4;

L_52f4:
    if ((hs.iItem <= iengineGalaxyScoop))
        goto L_5310;
    else
        goto L_5302;

L_5302:
    if ((hs.iItem != iengineRadiatingHydroRamScoop))
        goto L_533a;
    else
        goto L_5310;

L_5310:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceNoRamscoops) == 0x0))
        goto L_533a;
    else
        goto L_5331;

L_5331:
    return 0xffff;

L_533a:
    if ((hs.iItem == iengineGalaxyScoop))
        goto L_5356;
    else
        goto L_5348;

L_5348:
    if ((hs.iItem != iengineFuelMizer))
        goto L_5380;
    else
        goto L_5356;

L_5356:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceIFE) != 0x0))
        goto L_5380;
    else
        goto L_5377;

L_5377:
    return 0xffff;

L_5380:
    if ((hs.iItem != iengineInterspace10))
        goto L_53b8;
    else
        goto L_538e;

L_538e:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceNoRamscoops) != 0x0))
        goto L_53b8;
    else
        goto L_53af;

L_53af:
    return 0xffff;

L_53b8:
    if ((FShouldPartBeHidden(ppart) == 0x0))
        goto L_609c;
    else
        goto L_53cb;

L_53cb:
    return 0xffff;

L_53d4:
    if ((hs.iItem < ishieldCount))
        goto L_53e8;
    else
        goto L_53e2;

L_53e2:
    return 0x0;

L_53e8:
    ppart->pshield = &(rgshield[hs.iItem]);
    if ((hs.iItem != ishieldShadowShield))
        goto L_5423;
    else
        goto L_5411;

L_5411:
    if ((raMajor == 0x1))
        goto L_5423;
    else
        goto L_541a;

L_541a:
    return 0xffff;

L_5423:
    if ((hs.iItem != ishieldCrobySharmor))
        goto L_5443;
    else
        goto L_5431;

L_5431:
    if ((raMajor == 0x4))
        goto L_5443;
    else
        goto L_543a;

L_543a:
    return 0xffff;

L_5443:
    if ((FShouldPartBeHidden(ppart) == 0x0))
        goto L_609c;
    else
        goto L_5456;

L_5456:
    return 0xffff;

L_545f:
    if ((hs.iItem < ihuldefOrbitalFort))
        goto L_5473;
    else
        goto L_546d;

L_546d:
    return 0x0;

L_5473:
    ppart->phul = &(rghuldef[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_5498;

L_5498:
    if ((hs.iItem == ihuldefMiniColonyShip))
        goto L_54b4;
    else
        goto L_54a6;

L_54a6:
    if ((hs.iItem != ihuldefMetaMorph))
        goto L_54c6;
    else
        goto L_54b4;

L_54b4:
    if ((raMajor == 0x0))
        goto L_54c6;
    else
        goto L_54bd;

L_54bd:
    return 0xffff;

L_54c6:
    if ((hs.iItem == ihuldefFuelTransport))
        goto L_54e2;
    else
        goto L_54d4;

L_54d4:
    if ((hs.iItem != ihuldefSuperFreighter))
        goto L_54f4;
    else
        goto L_54e2;

L_54e2:
    if ((raMajor == 0x4))
        goto L_54f4;
    else
        goto L_54eb;

L_54eb:
    return 0xffff;

L_54f4:
    if ((hs.iItem == ihuldefMiner))
        goto L_552c;
    else
        goto L_5502;

L_5502:
    if ((hs.iItem == ihuldefMaxiMiner))
        goto L_552c;
    else
        goto L_5510;

L_5510:
    if ((hs.iItem == ihuldefMidgetMiner))
        goto L_552c;
    else
        goto L_551e;

L_551e:
    if ((hs.iItem != ihuldefUltraMiner))
        goto L_5556;
    else
        goto L_552c;

L_552c:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceOBRM) == 0x0))
        goto L_5556;
    else
        goto L_554d;

L_554d:
    return 0xffff;

L_5556:
    if ((hs.iItem == ihuldefMidgetMiner))
        goto L_5580;
    else
        goto L_5564;

L_5564:
    if ((hs.iItem == ihuldefMiner))
        goto L_5580;
    else
        goto L_5572;

L_5572:
    if ((hs.iItem != ihuldefUltraMiner))
        goto L_55aa;
    else
        goto L_5580;

L_5580:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceARM) != 0x0))
        goto L_55aa;
    else
        goto L_55a1;

L_55a1:
    return 0xffff;

L_55aa:
    if ((hs.iItem == ihuldefDreadnought))
        goto L_55c6;
    else
        goto L_55b8;

L_55b8:
    if ((hs.iItem != ihuldefBattleCruiser))
        goto L_55d8;
    else
        goto L_55c6;

L_55c6:
    if ((raMajor == 0x2))
        goto L_55d8;
    else
        goto L_55cf;

L_55cf:
    return 0xffff;

L_55d8:
    if ((hs.iItem != ihuldefRogue))
        goto L_55f8;
    else
        goto L_55e6;

L_55e6:
    if ((raMajor == 0x1))
        goto L_55f8;
    else
        goto L_55ef;

L_55ef:
    return 0xffff;

L_55f8:
    if ((hs.iItem != ihuldefStealthBomber))
        goto L_5618;
    else
        goto L_5606;

L_5606:
    if ((raMajor == 0x1))
        goto L_5618;
    else
        goto L_560f;

L_560f:
    return 0xffff;

L_5618:
    if ((hs.iItem == ihuldefMiniMineLayer))
        goto L_5634;
    else
        goto L_5626;

L_5626:
    if ((hs.iItem != ihuldefSuperMineLayer))
        goto L_5646;
    else
        goto L_5634;

L_5634:
    if ((raMajor == 0x5))
        goto L_5646;
    else
        goto L_563d;

L_563d:
    return 0xffff;

L_5646:
    if ((FShouldPartBeHidden(ppart) == 0x0))
        goto L_609c;
    else
        goto L_5659;

L_5659:
    return 0xffff;

L_5662:
    if ((hs.iItem < 0x5))
        goto L_5676;
    else
        goto L_5670;

L_5670:
    return 0x0;

L_5676:
    ppart->pcom = &(rghuldefSB[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_569b;

L_569b:
    if ((hs.iItem == 0x1))
        goto L_56b7;
    else
        goto L_56a9;

L_56a9:
    if ((hs.iItem != 0x3))
        goto L_56de;
    else
        goto L_56b7;

L_56b7:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceISB) != 0x0))
        goto L_56de;
    else
        goto L_56d8;

L_56d8:
    return 0xffff;

L_56de:
    if ((hs.iItem != 0x4))
        goto L_609c;
    else
        goto L_56ec;

L_56ec:
    if ((raMajor == 0x8))
        goto L_609c;
    else
        goto L_56f5;

L_56f5:
    return 0xffff;

L_56fe:
    if ((hs.iItem < iarmorCount))
        goto L_5712;
    else
        goto L_570c;

L_570c:
    return 0x0;

L_5712:
    ppart->parmor = &(rgarmor[hs.iItem]);
    if ((hs.iItem != iarmorDepletedNeutronium))
        goto L_574d;
    else
        goto L_573b;

L_573b:
    if ((raMajor == 0x1))
        goto L_574d;
    else
        goto L_5744;

L_5744:
    return 0xffff;

L_574d:
    if ((hs.iItem != iarmorFieldedKelarium))
        goto L_576d;
    else
        goto L_575b;

L_575b:
    if ((raMajor == 0x4))
        goto L_576d;
    else
        goto L_5764;

L_5764:
    return 0xffff;

L_576d:
    if ((FShouldPartBeHidden(ppart) == 0x0))
        goto L_609c;
    else
        goto L_5780;

L_5780:
    return 0xffff;

L_5789:
    if ((hs.iItem < ispecialECount))
        goto L_579d;
    else
        goto L_5797;

L_5797:
    return 0x0;

L_579d:
    ppart->pspecial = &(rgspecialE[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_57c2;

L_57c2:
    if ((FShouldPartBeHidden(ppart) == 0x0))
        goto L_57db;
    else
        goto L_57d5;

L_57d5:
    return 0xffff;

L_57db:
    goto L_5841;
    if ((raMajor == 0x1))
        goto L_57f3;
    else
        goto L_57ed;

L_57ed:
    return 0xffff;

L_57f3:
    goto L_5874;
    if ((raMajor == 0x5))
        goto L_5805;
    else
        goto L_57ff;

L_57ff:
    return 0xffff;

L_5805:
    goto L_5874;
    if ((raMajor == 0x7))
        goto L_5817;
    else
        goto L_5811;

L_5811:
    return 0xffff;

L_5817:
    goto L_5874;
    if ((raMajor == 0x0))
        goto L_5829;
    else
        goto L_5823;

L_5823:
    return 0xffff;

L_5829:
    goto L_5874;
    if ((raMajor == 0x4))
        goto L_609c;
    else
        goto L_5835;

L_5835:
    return 0xffff;

L_5841:

L_5874:

L_5877:
    if ((hs.iItem < ispecialMCount))
        goto L_588b;
    else
        goto L_5885;

L_5885:
    return 0x0;

L_588b:
    ppart->pspecial = &(rgspecialM[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_58b0;

L_58b0:
    if ((FShouldPartBeHidden(ppart) == 0x0))
        goto L_58c9;
    else
        goto L_58c3;

L_58c3:
    return 0xffff;

L_58c9:
    if ((hs.iItem != ispecialMColonizationModule))
        goto L_58e9;
    else
        goto L_58d7;

L_58d7:
    if ((raMajor != 0x8))
        goto L_58e9;
    else
        goto L_58e0;

L_58e0:
    return 0xffff;

L_58e9:
    if ((hs.iItem != ispecialMOrbitalConstructionModule))
        goto L_609c;
    else
        goto L_58f7;

L_58f7:
    if ((raMajor == 0x8))
        goto L_609c;
    else
        goto L_5900;

L_5900:
    return 0xffff;

L_5909:
    if ((hs.iItem < ispecialSBCount))
        goto L_591d;
    else
        goto L_5917;

L_5917:
    return 0x0;

L_591d:
    ppart->pspecialsb = &(rgspecialSB[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_5942;

L_5942:
    if ((hs.iItem < ispecialSBMassDriver5))
        goto L_598c;
    else
        goto L_5950;

L_5950:
    if ((hs.iItem > ispecialSBUltraDriver13))
        goto L_598c;
    else
        goto L_595e;

L_595e:
    if ((hs.iItem == ispecialSBMassDriver7))
        goto L_609c;
    else
        goto L_596c;

L_596c:
    if ((hs.iItem == ispecialSBUltraDriver10))
        goto L_609c;
    else
        goto L_597a;

L_597a:
    if ((raMajor == 0x6))
        goto L_609c;
    else
        goto L_5983;

L_5983:
    return 0xffff;

L_598c:
    if ((hs.iItem < ispecialSBStargate100250))
        goto L_609c;
    else
        goto L_599a;

L_599a:
    if ((hs.iItem > ispecialSBStargateAnyAny))
        goto L_609c;
    else
        goto L_59a8;

L_59a8:
    if ((raMajor == 0x7))
        goto L_59d3;
    else
        goto L_59b1;

L_59b1:
    if ((hs.iItem == ispecialSBStargateAny300))
        goto L_59cd;
    else
        goto L_59bf;

L_59bf:
    if ((hs.iItem < ispecialSBStargate100Any))
        goto L_59d3;
    else
        goto L_59cd;

L_59cd:
    return 0xffff;

L_59d3:
    if ((raMajor != 0x0))
        goto L_609c;
    else
        goto L_59dc;

L_59dc:
    return 0xffff;

L_59e5:
    if ((hs.iItem < iminesCount))
        goto L_59f9;
    else
        goto L_59f3;

L_59f3:
    return 0x0;

L_59f9:
    ppart->pmines = &(rgmines[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_5a1e;

L_5a1e:
    if ((hs.iItem == iminesMineDispenser40))
        goto L_5a8e;
    else
        goto L_5a2c;

L_5a2c:
    if ((hs.iItem == iminesMineDispenser80))
        goto L_5a8e;
    else
        goto L_5a3a;

L_5a3a:
    if ((hs.iItem == iminesMineDispenser130))
        goto L_5a8e;
    else
        goto L_5a48;

L_5a48:
    if ((hs.iItem == iminesHeavyDispenser50))
        goto L_5a8e;
    else
        goto L_5a56;

L_5a56:
    if ((hs.iItem == iminesHeavyDispenser110))
        goto L_5a8e;
    else
        goto L_5a64;

L_5a64:
    if ((hs.iItem == iminesHeavyDispenser200))
        goto L_5a8e;
    else
        goto L_5a72;

L_5a72:
    if ((hs.iItem == iminesSpeedTrap30))
        goto L_5a8e;
    else
        goto L_5a80;

L_5a80:
    if ((hs.iItem != iminesSpeedTrap50))
        goto L_5aa0;
    else
        goto L_5a8e;

L_5a8e:
    if ((raMajor == 0x5))
        goto L_5aa0;
    else
        goto L_5a97;

L_5a97:
    return 0xffff;

L_5aa0:
    if ((hs.iItem != iminesSpeedTrap20))
        goto L_5ac9;
    else
        goto L_5aae;

L_5aae:
    if ((raMajor == 0x5))
        goto L_5ac9;
    else
        goto L_5ab7;

L_5ab7:
    if ((raMajor == 0x4))
        goto L_5ac9;
    else
        goto L_5ac0;

L_5ac0:
    return 0xffff;

L_5ac9:
    if ((hs.iItem != iminesMineDispenser50))
        goto L_609c;
    else
        goto L_5ad7;

L_5ad7:
    if ((raMajor != 0x2))
        goto L_609c;
    else
        goto L_5ae0;

L_5ae0:
    return 0xffff;

L_5ae9:
    if ((hs.iItem < iminingCount))
        goto L_5afd;
    else
        goto L_5af7;

L_5af7:
    return 0x0;

L_5afd:
    ppart->pmining = &(rgmining[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_5b22;

L_5b22:
    if ((hs.iItem == iminingRoboMiner))
        goto L_5b68;
    else
        goto L_5b30;

L_5b30:
    if ((hs.iItem == iminingRoboMaxiMiner))
        goto L_5b68;
    else
        goto L_5b3e;

L_5b3e:
    if ((hs.iItem == iminingRoboSuperMiner))
        goto L_5b68;
    else
        goto L_5b4c;

L_5b4c:
    if ((hs.iItem == iminingRoboMidgetMiner))
        goto L_5b68;
    else
        goto L_5b5a;

L_5b5a:
    if ((hs.iItem != iminingRoboUltraMiner))
        goto L_5b92;
    else
        goto L_5b68;

L_5b68:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceOBRM) == 0x0))
        goto L_5b92;
    else
        goto L_5b89;

L_5b89:
    return 0xffff;

L_5b92:
    if ((hs.iItem == iminingRoboMidgetMiner))
        goto L_5bae;
    else
        goto L_5ba0;

L_5ba0:
    if ((hs.iItem != iminingRoboUltraMiner))
        goto L_5bd8;
    else
        goto L_5bae;

L_5bae:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceARM) != 0x0))
        goto L_5bd8;
    else
        goto L_5bcf;

L_5bcf:
    return 0xffff;

L_5bd8:
    if ((hs.iItem != iminingOrbitalAdjuster))
        goto L_5bf8;
    else
        goto L_5be6;

L_5be6:
    if ((raMajor == 0x3))
        goto L_5bf8;
    else
        goto L_5bef;

L_5bef:
    return 0xffff;

L_5bf8:
    if ((FShouldPartBeHidden(ppart) == 0x0))
        goto L_609c;
    else
        goto L_5c0b;

L_5c0b:
    return 0xffff;

L_5c14:
    if ((hs.iItem < iscannerCount))
        goto L_5c28;
    else
        goto L_5c22;

L_5c22:
    return 0x0;

L_5c28:
    ppart->pscanner = &(rgscanner[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_5c4d;

L_5c4d:
    if ((hs.iItem == iscannerFerretScanner))
        goto L_5c77;
    else
        goto L_5c5b;

L_5c5b:
    if ((hs.iItem == iscannerDolphinScanner))
        goto L_5c77;
    else
        goto L_5c69;

L_5c69:
    if ((hs.iItem != iscannerElephantScanner))
        goto L_5ca1;
    else
        goto L_5c77;

L_5c77:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceNoAdvScanner) == 0x0))
        goto L_5ca1;
    else
        goto L_5c98;

L_5c98:
    return 0xffff;

L_5ca1:
    if ((hs.iItem == iscannerChameleonScanner))
        goto L_5ccb;
    else
        goto L_5caf;

L_5caf:
    if ((hs.iItem == iscannerPickPocketScanner))
        goto L_5ccb;
    else
        goto L_5cbd;

L_5cbd:
    if ((hs.iItem != iscannerRobberBaronScanner))
        goto L_609c;
    else
        goto L_5ccb;

L_5ccb:
    if ((raMajor == 0x1))
        goto L_609c;
    else
        goto L_5cd4;

L_5cd4:
    return 0xffff;

L_5cdd:
    if ((hs.iItem < ibeamCount))
        goto L_5cf1;
    else
        goto L_5ceb;

L_5ceb:
    return 0x0;

L_5cf1:
    ppart->pbeam = &(rgbeam[hs.iItem]);
    if ((hs.iItem != ibeamMiniGun))
        goto L_5d29;
    else
        goto L_5d1a;

L_5d1a:
    if ((raMajor == 0x4))
        goto L_5d29;
    else
        goto L_5d23;

L_5d23:
    return 0xffff;

L_5d29:
    if ((hs.iItem == ibeamBlunderbuss))
        goto L_5d45;
    else
        goto L_5d37;

L_5d37:
    if ((hs.iItem != ibeamGatlingNeutrinoCannon))
        goto L_5d57;
    else
        goto L_5d45;

L_5d45:
    if ((raMajor == 0x2))
        goto L_5d57;
    else
        goto L_5d4e;

L_5d4e:
    return 0xffff;

L_5d57:
    if ((FShouldPartBeHidden(ppart) == 0x0))
        goto L_609c;
    else
        goto L_5d6a;

L_5d6a:
    return 0xffff;

L_5d73:
    if ((hs.iItem < itorpCount))
        goto L_5d87;
    else
        goto L_5d81;

L_5d81:
    return 0x0;

L_5d87:
    ppart->ptorp = &(rgtorp[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_5dac;

L_5dac:
    if ((FShouldPartBeHidden(ppart) == 0x0))
        goto L_609c;
    else
        goto L_5dbf;

L_5dbf:
    return 0xffff;

L_5dc8:
    if ((hs.iItem < ibombCount))
        goto L_5ddc;
    else
        goto L_5dd6;

L_5dd6:
    return 0x0;

L_5ddc:
    ppart->pbomb = &(rgbomb[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_5e01;

L_5e01:
    if ((hs.iItem < ibombSmartBomb))
        goto L_5e2f;
    else
        goto L_5e0f;

L_5e0f:
    if ((hs.iItem > ibombAnnihilatorBomb))
        goto L_5e2f;
    else
        goto L_5e1d;

L_5e1d:
    if ((raMajor != 0x4))
        goto L_5e2f;
    else
        goto L_5e26;

L_5e26:
    return 0xffff;

L_5e2f:
    if ((hs.iItem != ibombRetroBomb))
        goto L_5e4f;
    else
        goto L_5e3d;

L_5e3d:
    if ((raMajor == 0x3))
        goto L_5e4f;
    else
        goto L_5e46;

L_5e46:
    return 0xffff;

L_5e4f:
    if ((FShouldPartBeHidden(ppart) == 0x0))
        goto L_609c;
    else
        goto L_5e62;

L_5e62:
    return 0xffff;

L_5e6b:
    if ((hs.iItem < iplanetaryCount))
        goto L_5e7f;
    else
        goto L_5e79;

L_5e79:
    return 0x0;

L_5e7f:
    ppart->pplanetary = &(rgplanetary[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_5ea4;

L_5ea4:
    if ((hs.iItem < iplanetaryViewer50))
        goto L_5ef7;
    else
        goto L_5eb2;

L_5eb2:
    if ((hs.iItem > iplanetarySnooper620X))
        goto L_5ef7;
    else
        goto L_5ec0;

L_5ec0:
    if ((ppart->pplanetary->grAbility >= 0x0))
        goto L_5ef7;
    else
        goto L_5ed0;

L_5ed0:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceNoAdvScanner) == 0x0))
        goto L_5ef7;
    else
        goto L_5ef1;

L_5ef1:
    return 0xffff;

L_5ef7:
    if ((hs.iItem < iplanetaryViewer50))
        goto L_5f22;
    else
        goto L_5f05;

L_5f05:
    if ((hs.iItem > iplanetarySnooper620X))
        goto L_5f22;
    else
        goto L_5f13;

L_5f13:
    if ((raMajor != 0x8))
        goto L_5f22;
    else
        goto L_5f1c;

L_5f1c:
    return 0xffff;

L_5f22:
    if ((hs.iItem < iplanetarySDI))
        goto L_5f4d;
    else
        goto L_5f30;

L_5f30:
    if ((hs.iItem > iplanetaryNeutronShield))
        goto L_5f4d;
    else
        goto L_5f3e;

L_5f3e:
    if ((raMajor != 0x8))
        goto L_5f4d;
    else
        goto L_5f47;

L_5f47:
    return 0xffff;

L_5f4d:
    if ((hs.iItem < iplanetaryLaserBattery))
        goto L_5f7b;
    else
        goto L_5f5b;

L_5f5b:
    if ((hs.iItem > iplanetaryNeutronShield))
        goto L_5f7b;
    else
        goto L_5f69;

L_5f69:
    if ((raMajor != 0x2))
        goto L_5f7b;
    else
        goto L_5f72;

L_5f72:
    return 0xffff;

L_5f7b:
    if ((FShouldPartBeHidden(ppart) == 0x0))
        goto L_609c;
    else
        goto L_5f8e;

L_5f8e:
    return 0xffff;

L_5f97:
    if ((hs.iItem < iterraCount))
        goto L_5fab;
    else
        goto L_5fa5;

L_5fa5:
    return 0x0;

L_5fab:
    ppart->pterra = &(rgterra[hs.iItem]);
    if ((idPlayer == 0xffff))
        goto L_609c;
    else
        goto L_5fd0;

L_5fd0:
    if ((hs.iItem < iterraTotalTerraform3))
        goto L_609c;
    else
        goto L_5fde;

L_5fde:
    if ((hs.iItem > iterraTotalTerraform30))
        goto L_609c;
    else
        goto L_5fec;

L_5fec:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceTT) != 0x0))
        goto L_609c;
    else
        goto L_600d;

L_600d:
    return 0xffff;

L_6019:
    if ((hs.grhst == hstEngine))
        goto L_528d;
    else
        goto L_6021;

L_6021:
    if ((hs.grhst == hstScanner))
        goto L_5c14;
    else
        goto L_6029;

L_6029:
    if ((hs.grhst == hstShield))
        goto L_53d4;
    else
        goto L_6031;

L_6031:
    if ((hs.grhst == hstArmor))
        goto L_56fe;
    else
        goto L_6039;

L_6039:
    if ((hs.grhst == hstBeam))
        goto L_5cdd;
    else
        goto L_6041;

L_6041:
    if ((hs.grhst == hstTorp))
        goto L_5d73;
    else
        goto L_6049;

L_6049:
    if ((hs.grhst == hstBomb))
        goto L_5dc8;
    else
        goto L_6051;

L_6051:
    if ((hs.grhst == hstMining))
        goto L_5ae9;
    else
        goto L_6059;

L_6059:
    if ((hs.grhst == hstMines))
        goto L_59e5;
    else
        goto L_6061;

L_6061:
    if ((hs.grhst == hstSpecialSB))
        goto L_5909;
    else
        goto L_6069;

L_6069:
    if ((hs.grhst == hstSBHull))
        goto L_5662;
    else
        goto L_6071;

L_6071:
    if ((hs.grhst == hstSpecialE))
        goto L_5789;
    else
        goto L_6079;

L_6079:
    if ((hs.grhst == hstSpecialM))
        goto L_5877;
    else
        goto L_6081;

L_6081:
    if ((hs.grhst == hstTerra))
        goto L_5f97;
    else
        goto L_6089;

L_6089:
    if ((hs.grhst == hstHull))
        goto L_545f;
    else
        goto L_6091;

L_6091:
    if ((hs.grhst != hstPlanetary))
        goto L_5287;
    else
        goto L_6096;

L_6096:

L_609c:
    return TechStatus(&(ppart->pcom->rgTech));
}
