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

int Game::SetupFaction( Faction *pFac )
{
	pFac->unclaimed = Globals->START_MONEY + TurnNumber() * 50;

	if(pFac->noStartLeader)
		return 1;

	ARegion *reg = NULL;
	if(pFac->pStartLoc) {
		reg = pFac->pStartLoc;
	} else if(!Globals->MULTI_HEX_NEXUS) {
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
	Unit *temp2 = GetNewUnit( pFac );
	temp2->SetMen( I_LEADERS, 1 );
	temp2->reveal = REVEAL_FACTION;

	temp2->type = U_NORMAL;
	temp2->Study(S_COMBAT, 30);
	temp2->Study(S_LONGBOW, 30);
	temp2->Study(S_CROSSBOW, 30);
	temp2->Study(S_RIDING, 30);
	temp2->Study(S_TACTICS, 30);
	temp2->Study(S_STEALTH, 30);
	temp2->Study(S_OBSERVATION, 30);
	//temp2->items.SetNum(I_FOOD, 6);

	if (TurnNumber() >= 12) {
			temp2->Study(S_COMBAT, 60);
			temp2->Study(S_LONGBOW, 60);
			temp2->Study(S_CROSSBOW, 60);
			temp2->Study(S_RIDING, 60);
			temp2->Study(S_TACTICS, 60);
			temp2->Study(S_STEALTH, 60);
			temp2->Study(S_OBSERVATION, 60);
	}
	if(TurnNumber() >= 24) {
			temp2->Study(S_COMBAT, 90);
			temp2->Study(S_LONGBOW, 90);
			temp2->Study(S_CROSSBOW, 90);
			temp2->Study(S_RIDING, 90);
			temp2->Study(S_TACTICS, 90);
			temp2->Study(S_STEALTH, 90);
			temp2->Study(S_OBSERVATION, 90);
	}
	if(TurnNumber() >= 36) {
			temp2->Study(S_COMBAT, 120);
			temp2->Study(S_LONGBOW, 120);
			temp2->Study(S_CROSSBOW, 120);
			temp2->Study(S_RIDING, 120);
			temp2->Study(S_TACTICS, 120);
			temp2->Study(S_STEALTH, 120);
			temp2->Study(S_OBSERVATION, 120);
	}
	if(TurnNumber() >= 48) {
			temp2->Study(S_COMBAT, 150);
			temp2->Study(S_LONGBOW, 150);
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

	temp2->MoveUnit( reg->GetDummy() );

	return( 1 );
}

Faction *Game::CheckVictory()
{
	ARegion *reg;
	forlist(&regions) {
		ARegion *r = (ARegion *)elem;
		forlist(&r->objects) {
			Object *obj = (Object *)elem;
			if(obj->type != O_BKEEP) continue;
			if(obj->units.Num()) return NULL;
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
				if(u->guard == GUARD_GUARD) return u->faction;
			}
		}
	}
	return NULL;
}

void Game::ModifyTablesPerRuleset(void)
{
	if(Globals->APPRENTICES_EXIST)
	   	EnableSkill(S_MANIPULATE);

	if(!Globals->GATES_EXIST)
		DisableSkill(S_GATE_LORE);
        if(Globals->NEXUS_IS_CITY) {
                ClearTerrainRaces(R_NEXUS);
                ModifyTerrainRace(R_NEXUS, 0, I_TITAN);
                ClearTerrainItems(R_NEXUS);
                ModifyTerrainItems(R_NEXUS, 0, I_GAME, 100, 10);
                ModifyTerrainItems(R_NEXUS, 1, I_STONE, 100, 10);
                ModifyTerrainItems(R_NEXUS, 2, I_WOOD, 100, 10);
                ModifyTerrainItems(R_NEXUS, 3, I_IRON, 100, 10);
                ModifyTerrainEconomy(R_NEXUS, 1000, 15, 50, 2);
        }

	DisableItem(I_MAN);
	DisableItem(I_MERCENARY);
	//DisableItem(I_PEASANT);
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

//	ModifyObjectFlags(O_BKEEP, ObjectType::NEVERDECAY);

	// Make GateLore, ConstructGate and PortalLore take twice as long to study.
	//ModifySkillFlags(S_GATE_LORE,
	//		SkillType::MAGIC | SkillType::CAST | SkillType::SLOWSTUDY);
	//ModifySkillFlags(S_CONSTRUCT_GATE,
	//		SkillType::MAGIC | SkillType::CAST | SkillType::SLOWSTUDY);
	//ModifySkillFlags(S_PORTAL_LORE,
	//		SkillType::MAGIC | SkillType::CAST | SkillType::SLOWSTUDY);

	// Modify the various spells which are allowed to cross levels
	if(Globals->EASIER_UNDERWORLD) {
		ModifyRangeFlags(RANGE_TELEPORT, RangeType::RNG_CROSS_LEVELS);
		ModifyRangeFlags(RANGE_FARSIGHT, RangeType::RNG_CROSS_LEVELS);
		ModifyRangeFlags(RANGE_CLEAR_SKIES, RangeType::RNG_CROSS_LEVELS);
		ModifyRangeFlags(RANGE_WEATHER_LORE, RangeType::RNG_CROSS_LEVELS);
	}

	return;
}
