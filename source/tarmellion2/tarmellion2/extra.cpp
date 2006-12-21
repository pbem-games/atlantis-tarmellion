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
#include <gamedata.h>

int Game::SetupFaction( Faction *pFac )
{
    pFac->unclaimed = Globals->START_MONEY + TurnNumber() * 50;

    if(pFac->noStartLeader)
      return 1;

    ARegion *reg = NULL;
	if(pFac->pStartLoc) {
		reg = pFac->pStartLoc;
	} else {
		ARegionArray *pArr = regions.GetRegionArray(ARegionArray::LEVEL_SURFACE);
		while(!reg || TerrainDefs[reg->type].similar_type == R_OCEAN) {
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

	//unused items disabled
	DisableItem(I_MAN);
	DisableItem(I_SLAVE);
	DisableItem(I_MERCENARY);
	DisableItem(I_LHIGHMAN);
	DisableItem(I_HIGHMAN);
	DisableItem(I_LDARKMAN);
	DisableItem(I_DARKMAN);
	//DisableItem(I_LVIKING);
	//DisableItem(I_VIKING);
	//DisableItem(I_LBUCCANEER);
	//DisableItem(I_BUCCANEER);
	DisableItem(I_LHIGHLANDER);
	DisableItem(I_HIGHLANDER);
	DisableItem(I_LBARBARIAN);
	DisableItem(I_BARBARIAN);
	DisableItem(I_LPLAINSMAN);
	DisableItem(I_PLAINSMAN);
	DisableItem(I_LHELOT);
	DisableItem(I_HELOT);
	DisableItem(I_LBEARTRIBE);
	DisableItem(I_BEARTRIBE);
	DisableItem(I_LNORTHMAN);
	DisableItem(I_NORTHMAN);
	DisableItem(I_LGYPSY);
	DisableItem(I_GYPSY);
	DisableItem(I_LWAGONFOLK);
	DisableItem(I_WAGONFOLK);
	//DisableItem(I_LICELANDER);
	//DisableItem(I_ICELANDER);
	//DisableItem(I_LHORSEPEOPLE);
	//DisableItem(I_HORSEPEOPLE);
	//DisableItem(I_LNOMAD);
	//DisableItem(I_NOMAD);
	DisableItem(I_LTRIBESMAN);
	DisableItem(I_TRIBESMAN);
	DisableItem(I_LHEADHUNTER);
	DisableItem(I_HEADHUNTER);
	DisableItem(I_LWOODSMAN);
	DisableItem(I_WOODSMAN);
	//DisableItem(I_LFFOLK);
	//DisableItem(I_FFOLK);
	DisableItem(I_LAMAZON);
	DisableItem(I_AMAZON);
	DisableItem(I_LHILLMAN);
	DisableItem(I_HILLMAN);
	//DisableItem(I_LLOWLANDER);
	//DisableItem(I_LOWLANDER);
	DisableItem(I_LHALFORC);
	DisableItem(I_HALFORC);
	DisableItem(I_LCAVEDWELLER);
	DisableItem(I_CAVEDWELLER);
	DisableItem(I_LTROGLODYTE);
	DisableItem(I_TROGLODYTE);
	//DisableItem(I_LDARKMAN);
	//DisableItem(I_DARKMAN);
	DisableItem(I_LDEEPFFOLK);
	DisableItem(I_DEEPFFOLK);
	DisableItem(I_LNIGHTSTALKER);
	DisableItem(I_NIGHTSTALKER);
	DisableItem(I_LCOMMONMAN);
	DisableItem(I_COMMONMAN);
	//DisableItem(I_LHUMAN);
	//DisableItem(I_HUMAN);
	DisableItem(I_LGNOLL);
	DisableItem(I_GNOLL);
	DisableItem(I_LHILLDWARF);
	DisableItem(I_HILLDWARF);
	DisableItem(I_LMOUNTAINDWARF);
	DisableItem(I_MOUNTAINDWARF);
	DisableItem(I_LICEDWARF);
	DisableItem(I_ICEDWARF);
	DisableItem(I_LDESERTDWARF);
	DisableItem(I_DESERTDWARF);
	DisableItem(I_LUNDERDWARF);
	DisableItem(I_UNDERDWARF);
	DisableItem(I_LDARKDWARF);
	DisableItem(I_DARKDWARF);
	DisableItem(I_LDEEPDWARF);
	DisableItem(I_DEEPDWARF);
	DisableItem(I_LGULLYDWARF);
	DisableItem(I_GULLYDWARF);
	DisableItem(I_LWOODELF);
	DisableItem(I_WOODELF);
	DisableItem(I_LSEAELF);
	DisableItem(I_SEAELF);
	DisableItem(I_LHIGHELF);
	DisableItem(I_HIGHELF);
	DisableItem(I_LTRIBALELF);
	DisableItem(I_TRIBALELF);
	DisableItem(I_LNIGHTELF);
	DisableItem(I_NIGHTELF);
	DisableItem(I_LDRAGONLORD);
	DisableItem(I_DRAGONLORD);
	DisableItem(I_LDARKELF);
	DisableItem(I_DARKELF);
	//DisableItem(I_LDROWELF);
	//DisableItem(I_DROWELF);
	DisableItem(I_LDEATHWALKER);
	DisableItem(I_DEATHWALKER);
	DisableItem(I_LORC);
	DisableItem(I_ORC);
	DisableItem(I_LURUK);
	DisableItem(I_URUK);
	DisableItem(I_LGOBLINMAN);
	DisableItem(I_GOBLINMAN);
	DisableItem(I_LOGREMAN);
	DisableItem(I_OGREMAN);
	DisableItem(I_LHOBBIT);
	DisableItem(I_HOBBIT);
	DisableItem(I_LGNOME);
	DisableItem(I_GNOME);
	DisableItem(I_LMINOTAUR);
	DisableItem(I_MINOTAUR);
	DisableItem(I_LLIZARDMAN);
	DisableItem(I_LIZARDMAN);
	DisableItem(I_LFAIRY);
	DisableItem(I_FAIRY);
	DisableItem(I_LTITAN);
	DisableItem(I_TITAN);
	DisableItem(I_STEELDRAGON_S);
	DisableItem(I_COPPERDRAGON_S);
	DisableItem(I_BRONZEDRAGON_S);
	DisableItem(I_SILVERDRAGON_S);
	DisableItem(I_GOLDENDRAGON_S);
	DisableItem(I_STEELDRAGON_O);
	DisableItem(I_COPPERDRAGON_O);
	DisableItem(I_BRONZEDRAGON_O);
	DisableItem(I_SILVERDRAGON_O);
	DisableItem(I_GOLDENDRAGON_O);
	DisableItem(I_STEELDRAGON_A);
	DisableItem(I_COPPERDRAGON_A);
	DisableItem(I_BRONZEDRAGON_A);
	DisableItem(I_SILVERDRAGON_A);
	DisableItem(I_GOLDENDRAGON_A);
	DisableItem(I_STEELDRAGON_B);
	DisableItem(I_COPPERDRAGON_B);
	DisableItem(I_BRONZEDRAGON_B);
	DisableItem(I_SILVERDRAGON_B);
	DisableItem(I_GOLDENDRAGON_B);
	DisableItem(I_GREENDRAGON_S);
	DisableItem(I_BROWNDRAGON_S);
	DisableItem(I_BLUEDRAGON_S);
	DisableItem(I_REDDRAGON_S);
	DisableItem(I_WHITEDRAGON_S);
	DisableItem(I_BLACKDRAGON_S);
	DisableItem(I_GREENDRAGON_O);
	DisableItem(I_BROWNDRAGON_O);
	DisableItem(I_BLUEDRAGON_O);
	DisableItem(I_REDDRAGON_O);
	DisableItem(I_WHITEDRAGON_O);
	DisableItem(I_BLACKDRAGON_O);
	DisableItem(I_GREENDRAGON_A);
	DisableItem(I_BROWNDRAGON_A);
	DisableItem(I_BLUEDRAGON_A);
	DisableItem(I_REDDRAGON_A);
	DisableItem(I_WHITEDRAGON_A);
	DisableItem(I_BLACKDRAGON_A);
	DisableItem(I_GREENDRAGON_B);
	DisableItem(I_BROWNDRAGON_B);
	DisableItem(I_BLUEDRAGON_B);
	DisableItem(I_REDDRAGON_B);
	DisableItem(I_WHITEDRAGON_B);
	DisableItem(I_BLACKDRAGON_B);
	DisableItem(I_HHIGHMAN);
	DisableItem(I_HDARKMAN);
	DisableItem(I_HHIGHLANDER);
	DisableItem(I_HBARBARIAN);
	DisableItem(I_HPLAINSMAN);
	DisableItem(I_HHELOT);
	DisableItem(I_HBEARTRIBE);
	DisableItem(I_HNORTHMAN);
	DisableItem(I_HGYPSY);
	DisableItem(I_HWAGONFOLK);
	DisableItem(I_HTRIBESMAN);
	DisableItem(I_HHEADHUNTER);
	DisableItem(I_HWOODSMAN);
	DisableItem(I_HAMAZON);
	DisableItem(I_HHILLMAN);
	DisableItem(I_HHALFORC);
	DisableItem(I_HCAVEDWELLER);
	DisableItem(I_HTROGLODYTE);
	DisableItem(I_HDEEPFFOLK);
	DisableItem(I_HNIGHTSTALKER);
	DisableItem(I_HCOMMONMAN);
	DisableItem(I_HGNOLL);
	DisableItem(I_HHILLDWARF);
	DisableItem(I_HMOUNTAINDWARF);
	DisableItem(I_HUNDERDWARF);
	DisableItem(I_HDARKDWARF);
	DisableItem(I_HDEEPDWARF);
	DisableItem(I_HGULLYDWARF);
	DisableItem(I_HWOODELF);
	DisableItem(I_HSEAELF);
	DisableItem(I_HHIGHELF);
	DisableItem(I_HTRIBALELF);
	DisableItem(I_HNIGHTELF);
	DisableItem(I_HDRAGONLORD);
	DisableItem(I_HDARKELF);
	DisableItem(I_HDEATHWALKER);
	DisableItem(I_HORC);
	DisableItem(I_HURUK);
	DisableItem(I_HGOBLINMAN);
	DisableItem(I_HOGREMAN);
	DisableItem(I_HHOBBIT);
	DisableItem(I_HGNOME);
	DisableItem(I_HMINOTAUR);
	DisableItem(I_HLIZARDMAN);
	DisableItem(I_BLUECRYSTAL);
	DisableItem(I_REDCRYSTAL);
	DisableItem(I_YELLOWCRYSTAL);
	DisableItem(I_ORANGECRYSTAL);
	DisableItem(I_GREENCRYSTAL);
	DisableItem(I_VIOLETCRYSTAL);
	DisableItem(I_WHITECRYSTAL);
	DisableItem(I_BLACKCRYSTAL);
	DisableItem(I_BLUEPOWERCRYSTAL);
	DisableItem(I_REDPOWERCRYSTAL);
	DisableItem(I_YELLOWPOWERCRYSTAL);
	DisableItem(I_ORANGEPOWERCRYSTAL);
	DisableItem(I_GREENPOWERCRYSTAL);
	DisableItem(I_VIOLETPOWERCRYSTAL);
	DisableItem(I_WHITEPOWERCRYSTAL);
	DisableItem(I_BLACKPOWERCRYSTAL);
	DisableItem(I_DESERTDRAKE);
	DisableItem(I_SWAMPDRAKE);
	DisableItem(I_CAVEDRAKE);
	DisableItem(I_RAINBOWDRAKE);
	DisableItem(I_LHORSEMAN);
	DisableItem(I_HORSEMAN);
	DisableItem(I_HHORSEMAN);
	DisableItem(I_LEASTERLING);
	DisableItem(I_EASTERLING);
	DisableItem(I_HEASTERLING);
	DisableItem(I_RAINBOWDAGGER);
	DisableItem(I_DARKSWORD);
	DisableItem(I_STARAXE);
	DisableItem(I_WSHIELD);
	DisableItem(I_ISHIELD);
	DisableItem(I_MSHIELD);
	DisableItem(I_ASHIELD);
	DisableItem(I_HEARTWOOD);
	DisableItem(I_DWARVINSTEEL);
	DisableItem(I_ELVENBOW);
	DisableItem(I_DWARVINBATTLEAXE);
	DisableItem(I_STEEL);
	DisableItem(I_SSHIELD);
	DisableItem(I_STEELCHAINARMOR);
	DisableItem(I_STEELSCALEARMOR);
	DisableItem(I_STEELPLATEARMOR);

	//unused skills disabled
	DisableSkill(S_GEMCUTTING);
	DisableSkill(S_CREATE_STAFF_OF_LIGHTNING);

	//unused objects disabled
	//DisableObject(O_BLACKKEEP);
	DisableObject(O_JOINER);
	DisableObject(O_WEAPONFORGE);
	DisableObject(O_ARMORFORGE);
	DisableObject(O_TEAMTRAINER);
	DisableObject(O_APOTHECARY);
	DisableObject(O_PELTMONGER);
	DisableObject(O_SCULPTOR);
	DisableObject(O_JEWELER);
	DisableObject(O_FISHERY);
	DisableObject(O_FARMHOUSE);
	DisableObject(O_RANCHHOUSE);
	DisableObject(O_CHANDLER);
	DisableObject(O_RUNEMASTER);
	DisableObject(O_EDCLIFFS);
	DisableObject(O_GDCLIFFS);

	// Make skills take twice as long to study.
	//ModifySkillFlags(S_GATE_LORE, SkillType::MAGIC | SkillType::CAST | SkillType::SLOWSTUDY);
	// Modify the various spells which are allowed to cross levels

	if(Globals->EASIER_UNDERWORLD) {
		ModifyRangeFlags(RANGE_TELEPORT, RangeType::RNG_CROSS_LEVELS);
		ModifyRangeFlags(RANGE_FARSIGHT, RangeType::RNG_CROSS_LEVELS);
		ModifyRangeFlags(RANGE_CLEAR_SKIES, RangeType::RNG_CROSS_LEVELS);
		ModifyRangeFlags(RANGE_WEATHER_LORE, RangeType::RNG_CROSS_LEVELS);
	}

	return;
}
