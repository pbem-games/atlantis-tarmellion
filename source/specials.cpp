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
#include "battle.h"
#include <gamedata.h>

void Soldier::SetupHealing() {
	if (unit->type == U_MAGE) {
		healtype = unit->GetSkill(S_MAGICAL_HEALING);
		if (healtype > 5) healtype = 5;
		if (healtype > 0) {
			healing = HealDefs[healtype].num;
			healitem = -1;
			return;
		}
	}

	if (unit->items.GetNum(I_HEALINGPOTION)) {
		healtype = 1;
		unit->items.SetNum(I_HEALINGPOTION, unit->items.GetNum(I_HEALINGPOTION)-1);
		healing = 10;
		healitem = I_HEALINGPOTION;
	} else {
		healing = unit->GetSkill(S_HEALING) * Globals->HEALS_PER_MAN;
		if (healing) {
			healtype = 1;
			int max_heal = -1;
			forlist(&(unit->items)) {
				Item *i = (Item *) elem;
				if (ItemDefs[i->type].attributes & ItemType::CAN_HEAL && ItemDefs[i->type].pValue > max_heal) {
					max_heal = ItemDefs[i->type].pValue;
					healitem = i->type;
	 			}
			}
			int amnt = unit->items.GetNum(healitem);
			if (amnt < healing) healing = amnt;
			unit->items.SetNum(healitem,amnt - healing);
		}
	}
}

int Army::CheckSpecialTarget(int special,int tar) {
	SpecialType *spd = &SpecialDefs[special];
	int i;
	int match = 0;

	if (spd->targflags & SpecialType::HIT_BUILDINGIF) {
		match = 0;
		if (!soldiers[tar]->building) return 0;
		for(i = 0; i < 3; i++) {
			if (soldiers[tar]->building &&
					(spd->buildings[i] == soldiers[tar]->building)) match = 1;
		}
		if (!match) return 0;
	}

	if (spd->targflags & SpecialType::HIT_BUILDINGEXCEPT) {
		match = 0;
		if (!soldiers[tar]->building) return 0;
		for(i = 0; i < 3; i++) {
			if (soldiers[tar]->building &&
					(spd->buildings[i] == soldiers[tar]->building)) match = 1;
		}
		if (match) return 0;
	}

	if (spd->targflags & SpecialType::HIT_SOLDIERIF) {
		match = 0;
		if (soldiers[tar]->race == -1) return 0;
		for(i = 0; i < 7; i++) {
			if (soldiers[tar]->race == spd->targets[i]) match = 1;
		}
		if (!match) return 0;
	}

	if (spd->targflags & SpecialType::HIT_SOLDIEREXCEPT) {
		match = 0;
		if (soldiers[tar]->race == -1) return 0;
		for(i = 0; i < 7; i++) {
			if (soldiers[tar]->race == spd->targets[i]) match = 1;
		}
		if (match) return 0;
	}

	if (spd->targflags & SpecialType::HIT_EFFECTIF) {
		match = 0;
		for(i = 0; i < 3; i++) {
			if (soldiers[tar]->HasEffect(spd->effects[i])) match = 1;
		}
		if (!match) return 0;
	}

	if (spd->targflags & SpecialType::HIT_EFFECTEXCEPT) {
		match = 0;
		for(i = 0; i < 3; i++) {
			if (soldiers[tar]->HasEffect(spd->effects[i])) match = 1;
		}
		if (match) return 0;
	}

	if (spd->targflags & SpecialType::HIT_MOUNTIF) {
		match = 0;
		if (soldiers[tar]->riding == -1) return 0;
		for(i = 0; i < 7; i++) {
			if (soldiers[tar]->riding == spd->targets[i]) match = 1;
		}
		if (!match) return 0;
	}

	if (spd->targflags & SpecialType::HIT_MOUNTEXCEPT) {
		match = 0;
		if (soldiers[tar]->riding == -1) return 0;
		for(i = 0; i < 7; i++) {
			if (soldiers[tar]->riding == spd->targets[i]) match = 1;
		}
		if (match) return 0;
	}

	if (spd->targflags & SpecialType::HIT_ILLUSION) {
		// All illusions are of type monster, so lets make sure we get it
		// right.  If we ever have other types of illusions, we can change
		// this.
		if (!(ItemDefs[soldiers[tar]->race].type & IT_MONSTER))
			return 0;
		if (ItemDefs[soldiers[tar]->race].index != MONSTER_ILLUSION)
			return 0;
	}

	if (spd->targflags & SpecialType::HIT_NOMONSTER) {
		if (ItemDefs[soldiers[tar]->race].type & IT_MONSTER)
			return 0;
	}
	return 1;
}

void Battle::UpdateShields(Army *a) {
	for (int i=0; i<a->notbehind; i++) {
		int shtype = -1;
		SpecialType *spd;

		if (a->soldiers[i]->special == -1) continue;
		spd = &SpecialDefs[a->soldiers[i]->special];

		if (!(spd->effectflags & SpecialType::FX_SHIELD) &&
				!(spd->effectflags & SpecialType::FX_DEFBONUS)) continue;

		if (spd->effectflags & SpecialType::FX_SHIELD) {
			for(shtype = 0; shtype < 4; shtype++) {
				if (spd->shield[shtype] == -1) continue;
				Shield *sh = new Shield;
				sh->shieldtype = spd->shield[shtype];
				sh->shieldskill = a->soldiers[i]->slevel;
				a->shields.Add(sh);
			}
		}

		if (spd->effectflags & SpecialType::FX_DEFBONUS && a->round == 0) {
			for(shtype = 0; shtype < 4; shtype++) {
				if (spd->defs[shtype].type == -1) continue;
				int bonus = spd->defs[shtype].val;
				if (spd->effectflags & SpecialType::FX_USE_LEV)
					bonus *= a->soldiers[i]->slevel;
				a->soldiers[i]->dskill[spd->defs[shtype].type] += bonus;
			}
		}

		AddLine(*(a->soldiers[i]->unit->name) + " casts " +
				spd->shielddesc + ".");
	}
}

void Battle::DoSpecialAttack(int round, Soldier *a, Army *attackers,
		Army *def, int behind, int canattackback) {
	SpecialType *spd;
	int i, num, tot = -1;
	AString results[4];
	int dam = 0;

	if (a->special == -1) return;
	spd = &SpecialDefs[a->special];

	if (!(spd->effectflags & SpecialType::FX_DAMAGE)) return;

	int fullreport = 0;
	if( !Globals->AGGREGATE_BATTLE_REPORTS ) {
		fullreport = 1;
	} else if( a->unit->GetMen() == 1 &&
		       ItemDefs[a->race].type & IT_MAN &&
			   ManDefs[ItemDefs[a->race].index].flags & ManType::LEADER )
	{
		fullreport = 1;
	}
	
	// Check for spell failure
	if( getrandom( 100 ) < a->specialfail ) {
		if( fullreport ) {
			AString temp = a->name + " " + spd->spelldesc + ", but the spell fizzles.";
			if( Globals->AGGREGATE_BATTLE_REPORTS ) {
				attackers->roundLeaderReports.Add( new AString( temp ) );
			} else {
				AddLine( temp );
			}
		}
		return;
	}

	for(i = 0; i < 4; i++) {
		if (spd->damage[i].type == -1) continue;
		int times = spd->damage[i].value;
		if (spd->effectflags & SpecialType::FX_USE_LEV)
			times *= a->slevel;
		int realtimes = spd->damage[i].minnum + getrandom(times) +
			getrandom(times);
		num = def->DoAnAttack(a->special, realtimes,
				spd->damage[i].type, a->slevel,
				spd->damage[i].flags, spd->damage[i].dclass,
				spd->damage[i].effect, 0, canattackback);
		if (spd->effectflags & SpecialType::FX_DONT_COMBINE && num != -1) {
			if (spd->damage[i].effect == -1) {
				results[dam] = AString("killing ") + num;
				dam++;
			} else {
				results[dam] = AString(spd->spelldesc2) + num;
			}
		}
		if (num != -1) {
			if (tot == -1) tot = num;
			else tot += num;
			if( !fullreport )
				attackers->specialHits[a->special] += num;
		}
		if( !fullreport )
			attackers->specialAttacks[a->special] += realtimes;
	}
	if (fullreport) {
		if (tot == -1) {
			AString temp = a->name + " " + spd->spelldesc + ", but it is deflected.";
			if( Globals->AGGREGATE_BATTLE_REPORTS ) {
				attackers->roundLeaderReports.Add( new AString( temp ) );
			} else {
				AddLine( temp );
			}	
		} else {
			AString temp;
			if (spd->effectflags & SpecialType::FX_DONT_COMBINE) {
				temp = a->name + " " + spd->spelldesc;
				for(i = 0; i < dam; i++) {
					if (i) temp += ", ";
					if (i == dam-1) temp += " and ";
					temp += results[dam];
				}
				temp += AString(spd->spelltarget) + ".";
			} else {
				temp =a->name + " " + spd->spelldesc + ", " + spd->spelldesc2 +
						tot + spd->spelltarget + ".";
			}
			if( Globals->AGGREGATE_BATTLE_REPORTS ) {
				attackers->roundLeaderReports.Add( new AString( temp ) );
			} else {
				AddLine( temp );
			}
		}
	}
}

