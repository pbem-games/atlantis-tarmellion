// START A3HEADER
//
// This source file is part of the Atlantis PBM game program.
// Copyright (C) 1995-1999 Geoff Dunbar
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program, in the file license.txt. If not, write
// to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// See the Atlantis Project web page for details:
// http://www.prankster.com/project
//
// END A3HEADER
//
// This file contains extra game-specific functions
//
#include "game.h"
#include "gamedata.h"

int Game::SetupFaction(Faction *pFac) {
	pFac->unclaimed = Globals->START_MONEY + TurnNumber() * 50;

	if (pFac->noStartLeader)
		return 1;

	ARegion *reg = NULL;
	if (pFac->pStartLoc) {
		reg = pFac->pStartLoc;
	} else if (!Globals->MULTI_HEX_NEXUS) {
		reg = (ARegion *)(regions.First());
	} else {
		ARegionArray *pArr = regions.GetRegionArray(ARegionArray::LEVEL_NEXUS);
		while(!reg) {
			reg = pArr->GetRegion(getrandom(pArr->x), getrandom(pArr->y));
		}
	}

	//
	// Set up first unit.
	//
	Unit *temp2 = GetNewUnit(pFac);
	temp2->SetMen(I_LEADERS, 1);
	temp2->reveal = REVEAL_FACTION;

	temp2->type = U_NORMAL;
	temp2->Study(S_COMBAT, 30);
	temp2->Study(S_ARCHERY, 30);
	temp2->Study(S_CROSSBOW, 30);
	temp2->Study(S_RIDING, 30);
	temp2->Study(S_TACTICS, 30);
	temp2->Study(S_STEALTH, 30);
	temp2->Study(S_OBSERVATION, 30);
	//temp2->items.SetNum(I_FOOD, 6);

	if (TurnNumber() >= 12) {
			temp2->Study(S_COMBAT, 60);
			temp2->Study(S_ARCHERY, 60);
			temp2->Study(S_CROSSBOW, 60);
			temp2->Study(S_RIDING, 60);
			temp2->Study(S_TACTICS, 60);
			temp2->Study(S_STEALTH, 60);
			temp2->Study(S_OBSERVATION, 60);
	}
	if (TurnNumber() >= 24) {
			temp2->Study(S_COMBAT, 90);
			temp2->Study(S_ARCHERY, 90);
			temp2->Study(S_CROSSBOW, 90);
			temp2->Study(S_RIDING, 90);
			temp2->Study(S_TACTICS, 90);
			temp2->Study(S_STEALTH, 90);
			temp2->Study(S_OBSERVATION, 90);
	}
	if (TurnNumber() >= 36) {
			temp2->Study(S_COMBAT, 120);
			temp2->Study(S_ARCHERY, 120);
			temp2->Study(S_CROSSBOW, 120);
			temp2->Study(S_RIDING, 120);
			temp2->Study(S_TACTICS, 120);
			temp2->Study(S_STEALTH, 120);
			temp2->Study(S_OBSERVATION, 120);
	}
	if (TurnNumber() >= 48) {
			temp2->Study(S_COMBAT, 150);
			temp2->Study(S_ARCHERY, 150);
			temp2->Study(S_CROSSBOW, 150);
			temp2->Study(S_RIDING, 150);
			temp2->Study(S_TACTICS, 150);
			temp2->Study(S_STEALTH, 150);
			temp2->Study(S_OBSERVATION, 150);
	}

	if (Globals->UPKEEP_MINIMUM_FOOD > 0)
	{
		if (!(ItemDefs[I_FOOD].flags & ItemType::DISABLED))
			temp2->items.SetNum(I_FOOD, 6);
		else if (!(ItemDefs[I_FISH].flags & ItemType::DISABLED))
			temp2->items.SetNum(I_FISH, 6);
		else if (!(ItemDefs[I_LIVESTOCK].flags & ItemType::DISABLED))
			temp2->items.SetNum(I_LIVESTOCK, 6);
		else if (!(ItemDefs[I_GRAIN].flags & ItemType::DISABLED))
			temp2->items.SetNum(I_GRAIN, 2);
		temp2->items.SetNum(I_SILVER, 10);
	}

	temp2->MoveUnit(reg->GetDummy());

	return 1;
}

Faction *Game::CheckVictory() {
	ARegion *reg;
	forlist(&regions) {
		ARegion *r = (ARegion *)elem;
		forlist(&r->objects) {
			Object *obj = (Object *)elem;
			if (obj->type != O_BKEEP) continue;
			if (obj->units.Num()) return NULL;
			reg = r;
			break;
		}
	}
	{
		// Now see find the first faction guarding the region
		forlist(&reg->objects) {
			Object *o = reg->GetDummy();
			forlist(&o->units) {
				Unit *u = (Unit *)elem;
				if (u->guard == GUARD_GUARD) return u->faction;
			}
		}
	}
	return NULL;
}

void Game::ModifyTablesPerRuleset(void) {
	if (Globals->APPRENTICES_EXIST)
	   	EnableSkill(S_MANIPULATE);

	if (!Globals->GATES_EXIST)
		DisableSkill(S_GATE_LORE);
        if (Globals->NEXUS_IS_CITY) {
                ClearTerrainRaces(R_NEXUS);
                ModifyTerrainRace(R_NEXUS, 0, I_TITAN);
                ClearTerrainItems(R_NEXUS);
                //ModifyTerrainItems(R_NEXUS, 0, I_GAME, 100, 10);
                //ModifyTerrainItems(R_NEXUS, 1, I_STONE, 100, 10);
                //ModifyTerrainItems(R_NEXUS, 2, I_WOOD, 100, 10);
                //ModifyTerrainItems(R_NEXUS, 3, I_IRON, 100, 10);
                ModifyTerrainEconomy(R_NEXUS, 1000, 15, 0, 2);
        }

	DisableItem(I_MAN);
	DisableItem(I_MERCENARY);
	DisableItem(I_LICEDWARF);
	DisableItem(I_ICEDWARF);
	DisableItem(I_LDESERTDWARF);
	DisableItem(I_DESERTDWARF);
	DisableItem(I_LFAIRY);
	DisableItem(I_FAIRY);
	DisableItem(I_TLHUMAN);
	DisableItem(I_TRHUMAN);
	DisableItem(I_TLVIKING);
	DisableItem(I_TRVIKING);
	DisableItem(I_TLDERVISH);
	DisableItem(I_TRNOMAD);
	DisableItem(I_TLBARBARIAN);
	DisableItem(I_TRBARBARIAN);
	DisableItem(I_TLHIGHLANDER);
	DisableItem(I_TRCLANSMAN);
	DisableItem(I_TLIMPERIAL);
	DisableItem(I_TRIMPERIAL);
	DisableItem(I_TLTRIBESMAN);
	DisableItem(I_TRTRIBESMAN);
	DisableItem(I_TLFFOLK);
	DisableItem(I_TRFFOLK);
	DisableItem(I_TLCAJUN);
	DisableItem(I_TRLIZARDMAN);
	DisableItem(I_TLLOWLANDER);
	DisableItem(I_TRHALFING);
	DisableItem(I_TLELF);
	DisableItem(I_TRHALFELF);
	DisableItem(I_TLDARKMAN);
	DisableItem(I_TRDARKMAN);
	DisableItem(I_TLGNOME);
	DisableItem(I_TRGNOME);
	DisableItem(I_TLDWARF);
	DisableItem(I_TRDWARF);
	DisableItem(I_TLHOBGOBLIN);
	DisableItem(I_TRGOBLIN);
	DisableItem(I_TLBUCCANEER);
	DisableItem(I_TRBUCCANEER);
	DisableItem(I_TLSANDLING);
	DisableItem(I_TRSANDLING);
	DisableItem(I_TLTROLL);
	DisableItem(I_TRGNOLL);
	DisableItem(I_TLURUK);
	DisableItem(I_TRORC);
	DisableItem(I_TLBLACKKEEP);
	DisableItem(I_TRBLACKKEEP);
	DisableItem(I_TLHEADHUNTER);
	DisableItem(I_TRHEADHUNTER);
	DisableItem(I_TLOGRE);
	DisableItem(I_TROGRE);
	DisableItem(I_TLSERPENTPEOPLE);
	DisableItem(I_TRSERPENTPEOPLE);
	DisableItem(I_TLREPUBLICAN);
	DisableItem(I_TRREPUBLICAN);
	DisableItem(I_TLAMAZON);
	DisableItem(I_TRAMAZON);
	DisableItem(I_TLMINOTAUR);
	DisableItem(I_TRMINOTAUR);
	DisableItem(I_TLDROW);
	DisableItem(I_TRDROW);
	DisableItem(I_TLDUERGAR);
	DisableItem(I_TRDUERGAR);

	// Make GateLore, ConstructGate and PortalLore take twice as long to study.
	//ModifySkillFlags(S_GATE_LORE,
	//		SkillType::MAGIC | SkillType::CAST | SkillType::SLOWSTUDY);
	//ModifySkillFlags(S_CONSTRUCT_GATE,
	//		SkillType::MAGIC | SkillType::CAST | SkillType::SLOWSTUDY);
	//ModifySkillFlags(S_PORTAL_LORE,
	//		SkillType::MAGIC | SkillType::CAST | SkillType::SLOWSTUDY);

	// Modify the various spells which are allowed to cross levels
	if (Globals->EASIER_UNDERWORLD) {
		ModifyRangeFlags(RANGE_TELEPORT, RangeType::RNG_CROSS_LEVELS);
		ModifyRangeFlags(RANGE_FARSIGHT, RangeType::RNG_CROSS_LEVELS);
		ModifyRangeFlags(RANGE_CLEAR_SKIES, RangeType::RNG_CROSS_LEVELS);
		ModifyRangeFlags(RANGE_WEATHER_LORE, RangeType::RNG_CROSS_LEVELS);
	}

	ModifyItemProductionInput(I_FLAMINGSWORD,1,I_REDPOWERCRYSTAL,1);
	ModifyItemProductionInput(I_MAGESTAFF,1,I_VIOLETPOWERCRYSTAL,1);
	ModifyItemProductionSkill(I_RINGOFI,S_ARTIFACT_LORE,5);
	ModifyItemProductionInput(I_RINGOFI,0,I_RING,1);
	ModifyItemProductionInput(I_RINGOFI,1,I_GREENPOWERCRYSTAL,1);
	ModifyItemProductionInput(I_RINGOFI,2,I_SILVER,3800);
	ModifyItemProductionOutput(I_RINGOFI,5,1);
	ModifyItemProductionSkill(I_STAFFOFF,S_ARTIFACT_LORE,5);
	ModifyItemProductionInput(I_STAFFOFF,0,I_QUARTERSTAFF,1);
	ModifyItemProductionInput(I_STAFFOFF,1,I_REDPOWERCRYSTAL,1);
	ModifyItemProductionInput(I_STAFFOFF,2,I_SILVER,3800);
	ModifyItemProductionOutput(I_STAFFOFF,5,1);
	ModifyItemProductionSkill(I_STAFFOFL,S_ARTIFACT_LORE,5);
	ModifyItemProductionInput(I_STAFFOFL,0,I_QUARTERSTAFF,1);
	ModifyItemProductionInput(I_STAFFOFL,1,I_YELLOWPOWERCRYSTAL,1);
	ModifyItemProductionInput(I_STAFFOFL,2,I_SILVER,165000);
	ModifyItemProductionOutput(I_STAFFOFL,5,1);
	ModifyItemProductionSkill(I_AMULETOFTS,S_ARTIFACT_LORE,5);
	ModifyItemProductionInput(I_AMULETOFTS,0,I_JEWELRY,1);
	ModifyItemProductionInput(I_AMULETOFTS,1,I_NECKLACE,1);
	ModifyItemProductionInput(I_AMULETOFTS,2,I_GREENPOWERCRYSTAL,1);
	ModifyItemProductionInput(I_AMULETOFTS,3,I_SILVER,3800);
	ModifyItemProductionOutput(I_AMULETOFTS,5,1);
	ModifyItemProductionSkill(I_AMULETOFP,S_ARTIFACT_LORE,5);
	ModifyItemProductionInput(I_AMULETOFP,0,I_JEWELRY,1);
	ModifyItemProductionInput(I_AMULETOFP,1,I_NECKLACE,1);
	ModifyItemProductionInput(I_AMULETOFP,2,I_ORANGEPOWERCRYSTAL,1);
	ModifyItemProductionInput(I_AMULETOFP,3,I_SILVER,1200);
	ModifyItemProductionOutput(I_AMULETOFP,5,1);
	ModifyItemProductionSkill(I_SHIELDSTONE,S_ARTIFACT_LORE,5);
	ModifyItemProductionInput(I_SHIELDSTONE,0,I_CORESTONE,1);
	ModifyItemProductionInput(I_SHIELDSTONE,1,I_GEM,1);
	ModifyItemProductionInput(I_SHIELDSTONE,2,I_ORANGEPOWERCRYSTAL,1);
	ModifyItemProductionInput(I_SHIELDSTONE,3,I_SILVER,1700);
	ModifyItemProductionOutput(I_SHIELDSTONE,5,1);

	//ModifySkillName(S_ENCHANT_WEAPONS,"improve weapon");
	//ModifySkillAbbr(S_ENCHANT_WEAPONS,"IWEA");
	//ModifySkillName(S_ENCHANT_ARMOR,"improve armor");
	//ModifySkillAbbr(S_ENCHANT_ARMOR,"IARM");

	//ModifyItemName(I_ENCHANTEDJAVELIN,"improved javelin","improved javelins");
	//ModifyItemAbr(I_ENCHANTEDJAVELIN,"IMJAVE");
	ModifyItemProductionSkill(I_ENCHANTEDJAVELIN,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDJAVELIN,0,I_JAVELIN,1);
	ModifyItemProductionOutput(I_ENCHANTEDJAVELIN,1,1);
	//ModifyItemName(I_ENHANTEDLIGHTCROSSBOW,"improved light crossbow","improved light crossbows");
	//ModifyItemAbr(I_ENHANTEDLIGHTCROSSBOW,"IMLXBO");
	ModifyItemProductionSkill(I_ENHANTEDLIGHTCROSSBOW,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENHANTEDLIGHTCROSSBOW,0,I_LIGHTCROSSBOW,1);
	ModifyItemProductionOutput(I_ENHANTEDLIGHTCROSSBOW,1,1);
	//ModifyItemName(I_ENCHANTEDSHORTBOW,"improved shortbow","improved shortbows");
	//ModifyItemAbr(I_ENCHANTEDSHORTBOW,"IMSBOW");
	ModifyItemProductionSkill(I_ENCHANTEDSHORTBOW,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDSHORTBOW,0,I_SHORTBOW,1);
	ModifyItemProductionOutput(I_ENCHANTEDSHORTBOW,1,1);
	//ModifyItemName(I_ENCHANTEDPICK,"improved pick","improved picks");
	//ModifyItemAbr(I_ENCHANTEDPICK,"IMPICK");
	ModifyItemProductionSkill(I_ENCHANTEDPICK,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDPICK,0,I_PICK,1);
	ModifyItemProductionOutput(I_ENCHANTEDPICK,1,1);
	//ModifyItemName(I_ENCHANTEDSPEAR,"improved spear","improved spears");
	//ModifyItemAbr(I_ENCHANTEDSPEAR,"IMSPEA");
	ModifyItemProductionSkill(I_ENCHANTEDSPEAR,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDSPEAR,0,I_SPEAR,1);
	ModifyItemProductionOutput(I_ENCHANTEDSPEAR,1,1);
	//ModifyItemName(I_ENCHANTEDPIKE,"improved pike","improved pikes");
	//ModifyItemAbr(I_ENCHANTEDPIKE,"IMPIKE");
	ModifyItemProductionSkill(I_ENCHANTEDPIKE,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDPIKE,0,I_PIKE,1);
	ModifyItemProductionOutput(I_ENCHANTEDPIKE,1,1);
	//ModifyItemName(I_ENCHANTEDLANCE,"improved lance","improved lances");
	//ModifyItemAbr(I_ENCHANTEDLANCE,"IMLANC");
	ModifyItemProductionSkill(I_ENCHANTEDLANCE,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDLANCE,0,I_LANCE,1);
	ModifyItemProductionOutput(I_ENCHANTEDLANCE,1,1);
	//ModifyItemName(I_ENCHANTEDQUARTERSTAFF,"improved quarterstaff","improved quarterstaffs");
	//ModifyItemAbr(I_ENCHANTEDQUARTERSTAFF,"IMQUAR");
	ModifyItemProductionSkill(I_ENCHANTEDQUARTERSTAFF,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDQUARTERSTAFF,0,I_QUARTERSTAFF,1);
	ModifyItemProductionOutput(I_ENCHANTEDQUARTERSTAFF,1,1);
	//ModifyItemName(I_ENCHANTEDCLUB,"improved club","improved clubs");
	//ModifyItemAbr(I_ENCHANTEDCLUB,"IMCLUB");
	ModifyItemProductionSkill(I_ENCHANTEDCLUB,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDCLUB,0,I_CLUB,1);
	ModifyItemProductionOutput(I_ENCHANTEDCLUB,1,1);
	//ModifyItemName(I_ENCHANTEDMACE,"improved mace","improved maces");
	//ModifyItemAbr(I_ENCHANTEDMACE,"IMMACE");
	ModifyItemProductionSkill(I_ENCHANTEDMACE,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDMACE,0,I_MACE,1);
	ModifyItemProductionOutput(I_ENCHANTEDMACE,1,1);
	//ModifyItemName(I_ENCHANTEDHAMMER,"improved hammer","improved hammers");
	//ModifyItemAbr(I_ENCHANTEDHAMMER,"IMHAMM");
	ModifyItemProductionSkill(I_ENCHANTEDHAMMER,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDHAMMER,0,I_HAMMER,1);
	ModifyItemProductionOutput(I_ENCHANTEDHAMMER,1,1);
	//ModifyItemName(I_ENCHANTEDMORNINGSTAR,"improved morning star","improved morning stars");
	//ModifyItemAbr(I_ENCHANTEDMORNINGSTAR,"IMMSTA");
	ModifyItemProductionSkill(I_ENCHANTEDMORNINGSTAR,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDMORNINGSTAR,0,I_MORNINGSTAR,1);
	ModifyItemProductionOutput(I_ENCHANTEDMORNINGSTAR,1,1);
	//ModifyItemName(I_ENCHANTEDBATTLEHAMMER,"improved battle hammer","improved battle hammers");
	//ModifyItemAbr(I_ENCHANTEDBATTLEHAMMER,"IMBHAM");
	ModifyItemProductionSkill(I_ENCHANTEDBATTLEHAMMER,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDBATTLEHAMMER,0,I_BATTLEHAMMER,1);
	ModifyItemProductionOutput(I_ENCHANTEDBATTLEHAMMER,1,1);
	//ModifyItemName(I_ENCHANTEDAXE,"improved axe","improved axes");
	//ModifyItemAbr(I_ENCHANTEDAXE,"IMAXE");
	ModifyItemProductionSkill(I_ENCHANTEDAXE,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDAXE,0,I_AXE,1);
	ModifyItemProductionOutput(I_ENCHANTEDAXE,1,1);
	//ModifyItemName(I_ENCHANTEDBATTLEAXE,"improved battle axe","improved battle axes");
	//ModifyItemAbr(I_ENCHANTEDBATTLEAXE,"IMBAXE");
	ModifyItemProductionSkill(I_ENCHANTEDBATTLEAXE,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDBATTLEAXE,0,I_BATTLEAXE,1);
	ModifyItemProductionOutput(I_ENCHANTEDBATTLEAXE,1,1);
	//ModifyItemName(I_ENCHANTEDKNIFE,"improved knife","improved knives");
	//ModifyItemAbr(I_ENCHANTEDKNIFE,"IMKNIF");
	ModifyItemProductionSkill(I_ENCHANTEDKNIFE,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDKNIFE,0,I_KNIFE,1);
	ModifyItemProductionOutput(I_ENCHANTEDKNIFE,1,1);
	//ModifyItemName(I_ENCHANTEDDAGGER,"improved dagger","improved daggers");
	//ModifyItemAbr(I_ENCHANTEDDAGGER,"IMDAG");
	ModifyItemProductionSkill(I_ENCHANTEDDAGGER,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDDAGGER,0,I_DAGGER,1);
	ModifyItemProductionOutput(I_ENCHANTEDDAGGER,1,1);
	//ModifyItemName(I_ENCHANTEDPARRYINGDAGGER,"improved parrying dagger","improved parrying daggers");
	//ModifyItemAbr(I_ENCHANTEDPARRYINGDAGGER,"IMPDAG");
	ModifyItemProductionSkill(I_ENCHANTEDPARRYINGDAGGER,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDPARRYINGDAGGER,0,I_PARRYINGDAGGER,1);
	ModifyItemProductionOutput(I_ENCHANTEDPARRYINGDAGGER,1,1);
	//ModifyItemName(I_ENCHANTEDSABRE,"improved sabre","improved sabres");
	//ModifyItemAbr(I_ENCHANTEDSABRE,"IMSABR");
	ModifyItemProductionSkill(I_ENCHANTEDSABRE,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDSABRE,0,I_SABRE,1);
	ModifyItemProductionOutput(I_ENCHANTEDSABRE,1,1);
	//ModifyItemName(I_ENCHANTEDSWORD,"improved sword","improved swords");
	//ModifyItemAbr(I_ENCHANTEDSWORD,"IMSWO");
	ModifyItemProductionSkill(I_ENCHANTEDSWORD,S_ENCHANT_WEAPONS,1);
	ModifyItemProductionInput(I_ENCHANTEDSWORD,0,I_SWORD,1);
	ModifyItemProductionOutput(I_ENCHANTEDSWORD,1,1);
	//ModifyItemName(I_ENCHANTEDBROADSWORD,"improved broadsword","improved broadswords");
	//ModifyItemAbr(I_ENCHANTEDBROADSWORD,"IMBSWO");
	ModifyItemProductionSkill(I_ENCHANTEDBROADSWORD,S_ENCHANT_WEAPONS,2);
	ModifyItemProductionInput(I_ENCHANTEDBROADSWORD,0,I_BROADSWORD,1);
	ModifyItemProductionOutput(I_ENCHANTEDBROADSWORD,1,1);
	//ModifyItemName(I_ENCHANTEDLONGSWORD,"improved longsword","improved longswords");
	//ModifyItemAbr(I_ENCHANTEDLONGSWORD,"IMLSWO");
	ModifyItemProductionSkill(I_ENCHANTEDLONGSWORD,S_ENCHANT_WEAPONS,2);
	ModifyItemProductionInput(I_ENCHANTEDLONGSWORD,0,I_LONGSWORD,1);
	ModifyItemProductionOutput(I_ENCHANTEDLONGSWORD,1,1);
	//ModifyItemName(I_ENCHANTEDCLOTHARMOR,"improved cloth armor","improved cloth armors");
	//ModifyItemAbr(I_ENCHANTEDCLOTHARMOR,"IMCLAR");
	ModifyItemProductionSkill(I_ENCHANTEDCLOTHARMOR,S_ENCHANT_ARMOR,1);
	ModifyItemProductionInput(I_ENCHANTEDCLOTHARMOR,0,I_CLOTHARMOR,1);
	ModifyItemProductionOutput(I_ENCHANTEDCLOTHARMOR,1,1);
	//ModifyItemName(I_ENCHANTEDLEATHERARMOR,"improved leather armor","improved leather armors");
	//ModifyItemAbr(I_ENCHANTEDLEATHERARMOR,"IMLARM");
	ModifyItemProductionSkill(I_ENCHANTEDLEATHERARMOR,S_ENCHANT_ARMOR,1);
	ModifyItemProductionInput(I_ENCHANTEDLEATHERARMOR,0,I_LEATHERARMOR,1);
	ModifyItemProductionOutput(I_ENCHANTEDLEATHERARMOR,1,1);
	//ModifyItemName(I_ENCHANTEDCHAINARMOR,"improved chain armor","improved chain armors");
	//ModifyItemAbr(I_ENCHANTEDCHAINARMOR,"IMCAR");
	ModifyItemProductionSkill(I_ENCHANTEDCHAINARMOR,S_ENCHANT_ARMOR,1);
	ModifyItemProductionInput(I_ENCHANTEDCHAINARMOR,0,I_IRONCHAINARMOR,1);
	ModifyItemProductionOutput(I_ENCHANTEDCHAINARMOR,1,1);
	//ModifyItemName(I_ENCHANTEDSCALEARMOR,"improved scale armor","improved scale armors");
	//ModifyItemAbr(I_ENCHANTEDSCALEARMOR,"IMSCA");
	ModifyItemProductionSkill(I_ENCHANTEDSCALEARMOR,S_ENCHANT_ARMOR,1);
	ModifyItemProductionInput(I_ENCHANTEDSCALEARMOR,0,I_IRONSCALEARMOR,1);
	ModifyItemProductionOutput(I_ENCHANTEDSCALEARMOR,1,1);
	//ModifyItemName(I_ENCHANTEDPLATEARMOR,"improved plate armor","improved plate armors");
	//ModifyItemAbr(I_ENCHANTEDPLATEARMOR,"IMPLA");
	ModifyItemProductionSkill(I_ENCHANTEDPLATEARMOR,S_ENCHANT_ARMOR,1);
	ModifyItemProductionInput(I_ENCHANTEDPLATEARMOR,0,I_IRONPLATEARMOR,1);
	ModifyItemProductionOutput(I_ENCHANTEDPLATEARMOR,1,1);
	ModifyTerrainName(R_NEXUS,"fortress");

	ModifyWeaponSkills(WEAPON_RAINBOWDAGGER,S_COMBAT,S_STEALTH);
	ModifyWeaponFlags(WEAPON_RAINBOWDAGGER,WeaponType::SHORT|WeaponType::USEINASSASSINATE|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_DARKSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_DARKSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_STARAXE,S_COMBAT,S_LUMBERJACK);
	ModifyWeaponFlags(WEAPON_STARAXE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_RUNESWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_RUNESWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_BLOODSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_BLOODSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_FLAMINGSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_FLAMINGSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_MAGESTAFF,S_COMBAT,S_FORCE);
	ModifyWeaponFlags(WEAPON_MAGESTAFF,WeaponType::NOMOUNT|WeaponType::LONG|WeaponType::TWOHANDED|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_SWIFTAXE,S_COMBAT,S_WEAPONCRAFT);
	ModifyWeaponFlags(WEAPON_SWIFTAXE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ICESPEAR,S_COMBAT,S_WEAPONCRAFT);
	ModifyWeaponFlags(WEAPON_ICESPEAR,WeaponType::LONG|WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_FIRESPEAR,S_COMBAT,S_WEAPONCRAFT);
	ModifyWeaponFlags(WEAPON_FIRESPEAR,WeaponType::LONG|WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_DWARVINBATTLEAXE,S_COMBAT,S_LUMBERJACK);
	ModifyWeaponFlags(WEAPON_DWARVINBATTLEAXE,WeaponType::NOMOUNT|WeaponType::TWOHANDED|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ADLONGSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ADLONGSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_FOIL,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_FOIL,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_WARHAMMER,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_WARHAMMER,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_WARMACE,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_WARMACE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ADBROADSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ADBROADSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_BARDICHE,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_BARDICHE,WeaponType::NOMOUNT|WeaponType::LONG|WeaponType::TWOHANDED|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_WARAXE,S_COMBAT,S_LUMBERJACK);
	ModifyWeaponFlags(WEAPON_WARAXE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ADSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ADSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_WARSPEAR,S_COMBAT,S_RIDING);
	ModifyWeaponFlags(WEAPON_WARSPEAR,WeaponType::RIDINGBONUS|WeaponType::LONG|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_WARSTAFF,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_WARSTAFF,WeaponType::NOMOUNT|WeaponType::LONG|WeaponType::TWOHANDED|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_EVENINGSTAR,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_EVENINGSTAR,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_MILONGSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_MILONGSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_EPEE,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_EPEE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_SKULLCRUSHER,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_SKULLCRUSHER,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_MIBROADSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_MIBROADSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_BATTLEMACE,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_BATTLEMACE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDLONGSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ENCHANTEDLONGSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_CLEAVER,S_COMBAT,S_LUMBERJACK);
	ModifyWeaponFlags(WEAPON_CLEAVER,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_HALBERD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_HALBERD,WeaponType::NOMOUNT|WeaponType::LONG|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ADDAGGER,S_COMBAT,S_STEALTH);
	ModifyWeaponFlags(WEAPON_ADDAGGER,WeaponType::SHORT|WeaponType::USEINASSASSINATE|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDSABRE,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ENCHANTEDSABRE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_MISWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_MISWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDBATTLEHAMMER,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ENCHANTEDBATTLEHAMMER,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDBROADSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ENCHANTEDBROADSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDMACE,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ENCHANTEDMACE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_BATTLESPEAR,S_COMBAT,S_RIDING);
	ModifyWeaponFlags(WEAPON_BATTLESPEAR,WeaponType::RIDINGBONUS|WeaponType::LONG|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_TRISTAR,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_TRISTAR,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDBATTLEAXE,S_COMBAT,S_LUMBERJACK);
	ModifyWeaponFlags(WEAPON_ENCHANTEDBATTLEAXE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDPIKE,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ENCHANTEDPIKE,WeaponType::NOMOUNT|WeaponType::LONG|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_BATTLESTAFF,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_BATTLESTAFF,WeaponType::NOMOUNT|WeaponType::LONG|WeaponType::TWOHANDED|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ENCHANTEDSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDSPEAR,S_COMBAT,S_RIDING);
	ModifyWeaponFlags(WEAPON_ENCHANTEDSPEAR,WeaponType::RIDINGBONUS|WeaponType::LONG|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDMORNINGSTAR,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ENCHANTEDMORNINGSTAR,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDQUARTERSTAFF,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ENCHANTEDQUARTERSTAFF,WeaponType::NOMOUNT|WeaponType::LONG|WeaponType::TWOHANDED|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_MIDAGGER,S_COMBAT,S_STEALTH);
	ModifyWeaponFlags(WEAPON_MIDAGGER,WeaponType::SHORT|WeaponType::USEINASSASSINATE|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDDAGGER,S_COMBAT,S_STEALTH);
	ModifyWeaponFlags(WEAPON_ENCHANTEDDAGGER,WeaponType::SHORT|WeaponType::USEINASSASSINATE|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDKNIFE,S_STEALTH,S_TOOLMAKING);
	ModifyWeaponFlags(WEAPON_ENCHANTEDKNIFE,WeaponType::SHORT|WeaponType::USEINASSASSINATE|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_LONGSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_LONGSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDCLUB,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ENCHANTEDCLUB,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDHAMMER,S_SMITHING,-1);
	ModifyWeaponFlags(WEAPON_ENCHANTEDHAMMER,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDAXE,S_SMITHING,S_LUMBERJACK);
	ModifyWeaponFlags(WEAPON_ENCHANTEDAXE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDPICK,S_QUARRYING,S_MINING);
	ModifyWeaponFlags(WEAPON_ENCHANTEDPICK,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENCHANTEDPARRYINGDAGGER,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_ENCHANTEDPARRYINGDAGGER,WeaponType::SHORT|WeaponType::USEINASSASSINATE|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_SABRE,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_SABRE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_BATTLEHAMMER,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_BATTLEHAMMER,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_BROADSWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_BROADSWORD,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_MACE,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_MACE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENRUNEDAXE,S_WEAPONCRAFT,-1);
	ModifyWeaponFlags(WEAPON_ENRUNEDAXE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_BATTLEAXE,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_BATTLEAXE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_PIKE,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_PIKE,WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_SWORD,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_SWORD,WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_ENRUNEDSPEAR,S_WEAPONCRAFT,-1);
	ModifyWeaponFlags(WEAPON_ENRUNEDSPEAR,WeaponType::RIDINGBONUS|WeaponType::LONG|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_SPEAR,S_COMBAT,S_RIDING);
	ModifyWeaponFlags(WEAPON_SPEAR,WeaponType::RIDINGBONUS|WeaponType::LONG|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_MORNINGSTAR,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_MORNINGSTAR,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_QUARTERSTAFF,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_QUARTERSTAFF,WeaponType::NOMOUNT|WeaponType::LONG|WeaponType::TWOHANDED|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_HAMMER,S_SMITHING,-1);
	ModifyWeaponFlags(WEAPON_HAMMER,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_AXE,S_SMITHING,S_LUMBERJACK);
	ModifyWeaponFlags(WEAPON_AXE,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_PICK,S_QUARRYING,S_MINING);
	ModifyWeaponFlags(WEAPON_PICK,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_DAGGER,S_COMBAT,S_STEALTH);
	ModifyWeaponFlags(WEAPON_DAGGER,WeaponType::SHORT|WeaponType::USEINASSASSINATE|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_CLUB,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_CLUB,WeaponType::RIDINGBONUS|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_PDAGGER,S_COMBAT,-1);
	ModifyWeaponFlags(WEAPON_PDAGGER,WeaponType::SHORT|WeaponType::USEINASSASSINATE|WeaponType::NEEDSKILL);
	ModifyWeaponSkills(WEAPON_KNIFE,S_COMBAT,S_TOOLMAKING);
	ModifyWeaponFlags(WEAPON_KNIFE,WeaponType::SHORT|WeaponType::USEINASSASSINATE|WeaponType::NEEDSKILL);
	return;
}
