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
// MODIFICATIONS
// Date			Person			Change
// ----			------			------
// 2000/MAR/16	Larry Stanbery	Corrected bug in Soldier constructor.
//								Fixed "assassination bug" reported on
//								DejaNews.
// 2001/Feb/18	Joseph Traub	Added Apprentices concept from Lacondon
//								Conquest
#include "army.h"
#include "gameio.h"
#include <gamedata.h>

enum {
	WIN_NO_DEAD,
	WIN_DEAD,
	LOSS
};

Soldier::Soldier(Unit * u,Object * o,int regtype,int r,int ass) {
	race = r;
	unit = u;
	building = 0;

	healing = 0;
	healtype = 0;
	healitem = -1;
	canbehealed = 1;
	regen = 0;

	armor = -1;
	shield = -1;
	riding = -1;
	weapon = -1;
	mount = 0;
	rider = 0;

	amulet = -1;
	ring1 = -1;
	ring2 = -1;

	attacks = 1;
	attacktype = ATTACK_COMBAT;

	special = -1;
	slevel = 0;
	specialfail = 0;

	askill = 0;

	dskill[ATTACK_COMBAT] = 0;
	dskill[ATTACK_ENERGY] = -2;
	dskill[ATTACK_SPIRIT] = -2;
	dskill[ATTACK_WEATHER] = -2;
	dskill[ATTACK_RIDING] = 0;
	dskill[ATTACK_RANGED] = 0;
	damage = 0;
	hits = 1;
	maxhits = 1;
	amuletofi = 0;
	battleItems = 0;

	effects = 0;

	int debug=0;
//	if( u->num == 62 ) debug = 1;

	/* Building bonus */
	if (o->capacity) {
		building = o->type;
		for (int i=0; i<NUM_ATTACK_TYPES; i++) {
			dskill[i] += ObjectDefs[building].protection[i];
		}
		if (o->runes) {
		  if (dskill[ATTACK_ENERGY] < o->runes) {
		    dskill[ATTACK_ENERGY] = o->runes;
		  }
		  if (dskill[ATTACK_SPIRIT] < o->runes) {
		    dskill[ATTACK_SPIRIT] = o->runes;
		  }
		}
		o->capacity--;
	}


	/* Is this a monster? */
	if (ItemDefs[r].type & IT_MONSTER || ItemDefs[r].type & IT_MOUNT) {
		int mon = ItemDefs[r].index;
		if( ItemDefs[r].type & IT_MOUNT ) 
			mon = MountDefs[mon].monster;
		if (u->type == U_WMON)
			name = AString(MonDefs[mon].name) + " in " + *(unit->name);
		else
			name = AString(MonDefs[mon].name) +
				AString(" controlled by ") + *(unit->name);
		askill = MonDefs[mon].attackLevel;
		dskill[ATTACK_COMBAT] += MonDefs[mon].defense[ATTACK_COMBAT];
		if (MonDefs[mon].defense[ATTACK_ENERGY] > dskill[ATTACK_ENERGY]) {
			dskill[ATTACK_ENERGY] = MonDefs[mon].defense[ATTACK_ENERGY];
		}
		if (MonDefs[mon].defense[ATTACK_SPIRIT] > dskill[ATTACK_SPIRIT]) {
			dskill[ATTACK_SPIRIT] = MonDefs[mon].defense[ATTACK_SPIRIT];
		}
		if (MonDefs[mon].defense[ATTACK_WEATHER] > dskill[ATTACK_WEATHER]) {
			dskill[ATTACK_WEATHER] = MonDefs[mon].defense[ATTACK_WEATHER];
		}
		dskill[ATTACK_RIDING] += MonDefs[mon].defense[ATTACK_RIDING];
		dskill[ATTACK_RANGED] += MonDefs[mon].defense[ATTACK_RANGED];
		damage = 0;
		hits = MonDefs[mon].hits;
		if (hits < 1) hits = 1;
		maxhits = hits;
		attacks = MonDefs[mon].numAttacks;
		if (!attacks) attacks = 1;
		special = MonDefs[mon].special;
		slevel = MonDefs[mon].specialLevel;
		if (Globals->MONSTER_BATTLE_REGEN) {
			regen = MonDefs[mon].regen;
			if (regen < 0) regen = 0;
		}
		return;
	}

	name = *(unit->name);

	if( debug ) {
		Awrite( AString( "New Soldier from" ) + name );
	}

	SetupHealing();

	SetupSpell();
	SetupCombatItems();

	// Set up armor
	int i, item, armorType;
	for (i = 0; i < MAX_READY; i++) {
		// Check preferred armor first.
		item = unit->readyArmor[i];
		if (item == -1) break;
		armorType = ItemDefs[item].index;
		item = unit->GetArmor(armorType, ass);
		if (item != -1) {
			armor = item;
			break;
		}
	}
	if (armor == -1) {
		for (armorType = 1; armorType < NUMARMORS; armorType++) {
			item = unit->GetArmor(armorType, ass);
			if (item != -1) {
				armor = item;
				break;
			}
		}
	}

	//
	// Check if this unit is mounted
	//

	int terrainflags = TerrainDefs[regtype].flags;
	int canFly = (terrainflags & TerrainType::FLYINGMOUNTS);
	int canRide = (terrainflags & TerrainType::RIDINGMOUNTS);
	int ridingBonus = 0;
//	if (canFly || canRide) {
//		//
//		// Mounts of some type _are_ allowed in this region
//		//
	// New rule: soldiers can _always_ ride mounts, regardless of skill and terrain.
	// Terrain/skill only decides the bonus a unit can get. Also no difference between
	// riding and flying mounts

		int mountType;
		for (mountType = 1; mountType < NUMMOUNTS; mountType++) {
			item = unit->GetMount(mountType, canFly, canRide, ridingBonus, false);
			if (item == -1) continue;
			// Defer adding the combat bonus until we know if the weapon
			// allows it.  The defense bonus for riding can be added now
			// however.
			dskill[ATTACK_RIDING] += ridingBonus;
			riding = item;
			if( MountDefs[ItemDefs[riding].index].monster != -1 ) {
				mount = new Soldier( u, o, regtype, riding, ass );
				mount->rider = this;
			}

			break;
		}
		if( riding == -1 ) {
			// no mount found that will give a riding bonus, get any mount
			for (mountType = 1; mountType < NUMMOUNTS; mountType++) {
				item = unit->GetMount(mountType, canFly, canRide, ridingBonus, true);
				if (item == -1) continue;
				// Defer adding the combat bonus until we know if the weapon
				// allows it.  The defense bonus for riding can be added now
				// however.
				dskill[ATTACK_RIDING] += ridingBonus;
				riding = item;
				if( MountDefs[ItemDefs[riding].index].monster != -1 ) {
					mount = new Soldier( u, o, regtype, riding, ass );
					mount->rider = this;
				}

				break;
			}
		}

//	}

	//
	// Find the correct weapon for this soldier.
	//
	int weaponType;
	int attackBonus = 0;
	int defenseBonus = 0;
	int numAttacks = 1;
	for (i = 0; i < MAX_READY; i++) {
		// Check the preferred weapon first.
		item = unit->readyWeapon[i];
		if (item == -1) break;
		weaponType = ItemDefs[item].index;
		item = unit->GetWeapon(weaponType, riding, ridingBonus, attackBonus,
				defenseBonus, numAttacks, ass);
		if (item != -1) {
			weapon = item;
			break;
		}
	}
	if (weapon == -1) {
		for (weaponType = 1; weaponType < NUMWEAPONS; weaponType++) {
			item = unit->GetWeapon(weaponType, riding, ridingBonus,
					attackBonus, defenseBonus, numAttacks,
					ass);
			if (item != -1) {
				weapon = item;
				break;
			}
		}
	}
	// Find a shield if unit can have one.
	if (weapon == -1 || !(WeaponDefs[weapon].flags & WeaponType::TWOHANDED)) {
		for (i = 0; i < MAX_READY; i++) {
			item = unit->readyArmor[i];
 			if (item == -1) break;
			armorType = ItemDefs[item].index;
			if (ArmorDefs[armorType].flags & ArmorType::SHIELD) {
				item = unit->GetArmor(armorType, ass);
				if (item != -1) {
					shield = item;
					break;
				}
			}
		}
		if (shield == -1) {
			for (armorType = 1; armorType < NUMARMORS; armorType++) {
				if (ArmorDefs[armorType].flags & ArmorType::SHIELD) {
					item = unit->GetArmor(armorType, ass);
					if (item != -1) {
						shield = item;
						break;
					}
				}
			}
		}
	}

	// If we did not get a weapon, set attack and defense bonuses to
	// combat skill (and riding bonus if applicable).
	if (weapon == -1) {
		numAttacks = 1;
	} else {
		// Okay.  We got a weapon.
		// If this weapon also has a special
		// and we don't have a special set, use that special.
		// Weapons (like Runeswords) which are both weapons and battle
		// items will be skipped in the battle items setup and handled
		// here.
		if ((ItemDefs[weapon].type & IT_BATTLE) && special == -1) {
			special =
			    BattleItemDefs[ItemDefs[weapon].battleindex].index;
			slevel =
			    BattleItemDefs[ItemDefs[weapon].battleindex].skillLevel;
		}
	}

	unit->Practise(S_COMBAT);
	if (ridingBonus) 
		unit->Practise(S_RIDING);
		
	// Set the attack and defense skills
	// These will include the riding bonus if they should be included.
	askill = unit->GetSkill(S_COMBAT) * 3 + ridingBonus + attackBonus;
	dskill[ATTACK_COMBAT] = unit->GetSkill(S_COMBAT) * 3 + defenseBonus;
	attacks = numAttacks;

	// Does this race have monster stats?
	if( ItemDefs[race].type & IT_MAN && ManDefs[ItemDefs[race].index].monster != -1 ) {
		MonType * mt = &MonDefs[ManDefs[ItemDefs[race].index].monster];

		// Check minimum number of attacks and hits
		if( attacks >= 0 && mt->numAttacks > attacks ) attacks = mt->numAttacks;
        if( mt->hits > hits ) {
			hits = mt->hits;
			maxhits = hits;
		}

		// Use race special if one isn't already set
		if( mt->special != -1 && mt->specialLevel && special == -1 ) {
			special = mt->special;
			slevel = mt->specialLevel;
		}

		// If race has higher defence ratings vs. non-combat attacks, use them
		if (mt->defense[ATTACK_ENERGY] > dskill[ATTACK_ENERGY]) {
			dskill[ATTACK_ENERGY] = mt->defense[ATTACK_ENERGY];
		}
		if (mt->defense[ATTACK_SPIRIT] > dskill[ATTACK_SPIRIT]) {
			dskill[ATTACK_SPIRIT] = mt->defense[ATTACK_SPIRIT];
		}
		if (mt->defense[ATTACK_WEATHER] > dskill[ATTACK_WEATHER]) {
			dskill[ATTACK_WEATHER] = mt->defense[ATTACK_WEATHER];
		}
		if (mt->defense[ATTACK_RIDING] > dskill[ATTACK_RIDING]) {
			dskill[ATTACK_RIDING] = mt->defense[ATTACK_RIDING];
		}
		if (mt->defense[ATTACK_RANGED] > dskill[ATTACK_RANGED]) {
			dskill[ATTACK_RANGED] = mt->defense[ATTACK_RANGED];
		}
	}

	if( debug ) {
//		Awrite( AString("- Armor: ") + armor );
		Awrite( AString("- Weapon: ") + weapon );
		Awrite( AString("- NumAttacks: ") + attacks );
		Awrite( AString("- Attack: ") +  askill );
		Awrite( AString("- Defense: ") +  dskill[ATTACK_COMBAT] );
//		Awrite( AString("- Mount: ") + riding );
//		Awrite( AString("- Shield: ") + shield );
	}


}

void Soldier::SetupSpell() {
	if (unit->type != U_MAGE && unit->type != U_GUARDMAGE) return;

	if (unit->combat != -1) {
		slevel = unit->GetSkill(unit->combat);
		if (!slevel) {
			//
			// The unit can't cast this spell!
			//
			unit->combat = -1;
			return;
		}

		SkillType *pST = &SkillDefs[ unit->combat ];
		if (!(pST->flags & SkillType::COMBAT)) {
			//
			// This isn't a combat spell!
			//
			unit->combat = -1;
			return;
		}

		special = pST->special;
		// Set spell failure chance
		specialfail = GetSpellFailureChance( unit, unit->combat );

		unit->Practise(unit->combat);
	}
}

void Soldier::SetupCombatItems() {
	int battleType;
	for (battleType = 1; battleType < NUMBATTLEITEMS; battleType++) {
		BattleItemType *pBat = &BattleItemDefs[ battleType ];

		int item = unit->GetBattleItem(battleType);
		if (item == -1) continue;

		// If we are using the ready command, skip this item unless
		// it's the right one, or unless it is a shield, ring or amulet
		// which doesn't need preparing.
		
		if (!Globals->USE_PREPARE_COMMAND ||
				((unit->readyItem == -1) &&
				 (Globals->USE_PREPARE_COMMAND == GameDefs::PREPARE_NORMAL)) ||
				(pBat->itemNum==unit->readyItem) ||
//				(pBat->flags & BattleItemType::SHIELD) ||
				(pBat->flags & BattleItemType::RING) ||
				(pBat->flags & BattleItemType::AMULET) ) {
			if ((pBat->flags & BattleItemType::SPECIAL) && special != -1) {
				// This unit already has a special attack so give the item
				// back to the unit as they aren't going to use it.
				unit->items.SetNum(item, unit->items.GetNum(item)+1);
				continue;
			}
			if( (pBat->flags & BattleItemType::AMULET && amulet != -1) ||
				(pBat->flags & BattleItemType::RING && ring1 != -1 && ring2 != -1) )
			{
				// This unit already has an item of this type so give the item
				// back to the unit as they aren't going to use it.
				unit->items.SetNum(item, unit->items.GetNum(item)+1);
				continue;
			}

			if (pBat->flags & BattleItemType::MAGEONLY &&
			   unit->type != U_MAGE && unit->type != U_GUARDMAGE &&
			   unit->type != U_APPRENTICE) {
				// Only mages/apprentices can use this item so give the
				// item back to the unit as they aren't going to use it.
				unit->items.SetNum(item, unit->items.GetNum(item)+1);
				continue;
			}

			/* Make sure amulets of invulnerability are marked */
			if (item == I_AMULETOFI) {
				amuletofi = 1;
			}

			if( pBat->index == -1 && pBat->bonusType == -1 ) {
				// This item gives no bonus in combat - don't equip it
				unit->items.SetNum(item, unit->items.GetNum(item)+1);
				continue;
			}

			if (pBat->flags & BattleItemType::SPECIAL) {
				special = pBat->index;
				slevel = pBat->skillLevel;
			}

			if( pBat->flags & BattleItemType::AMULET ||
				pBat->flags & BattleItemType::RING )
			{
				// If item has a shield special effect, add it now
				if( pBat->index != -1 ) {
					SpecialType *sp = &SpecialDefs[pBat->index];
					if (sp->effectflags & SpecialType::FX_SHIELD) {
						for (int i = 0; i < 4; i++) {
							if (sp->shield[i] == NUM_ATTACK_TYPES) {
								for (int j = 0; j < NUM_ATTACK_TYPES; j++) {
									if (dskill[j] < pBat->skillLevel)
										dskill[j] = pBat->skillLevel;
								}
							} else if (sp->shield[i] >= 0) {
								if (dskill[sp->shield[i]] < pBat->skillLevel)
									dskill[sp->shield[i]] = pBat->skillLevel;
							}
						}
					}
				}

				// Add bonuses
				if( pBat->bonusType & BattleItemType::ADD_ATTACK )
					askill += pBat->bonusAmount;

				if( pBat->bonusType & BattleItemType::ADD_DEFENSE )
					dskill[ATTACK_COMBAT] += pBat->bonusAmount;

				if( pBat->bonusType & BattleItemType::ADD_HITS )
					hits += pBat->bonusAmount;

				if( pBat->bonusType & BattleItemType::ADD_ATTACKS )
					attacks += pBat->bonusAmount;

				// Add to soldier's item slot
				if( pBat->flags & BattleItemType::AMULET ) {
					amulet = item;
				} else if( pBat->flags & BattleItemType::RING ) {
					if( ring1 == -1 ) {
						ring1 = item;
					} else {
						ring2 = item;
					}
				}
			} else {
				SET_BIT(battleItems, battleType);
			}

		} else {
			// We are using prepared items and this item is NOT the one
			// we have prepared, so give it back to the unit as they won't
			// use it.
			unit->items.SetNum(item, unit->items.GetNum(item)+1);
			continue;
		}
	}
}

int Soldier::HasEffect(int eff) {
	if (eff < 0) return 0;
	int n = 1 << eff;
	return (effects & n);
}

void Soldier::SetEffect(int eff) {
	if (eff < 0) return;
	int n = 1 << eff;
	int i;

	EffectType *e = &EffectDefs[eff];

	askill += e->attackVal;

	for (i = 0; i < 4; i++) {
		if (e->defMods[i].type != -1) {
			dskill[e->defMods[i].type] += e->defMods[i].val;
		}
	}

	if (e->cancelEffect != -1) {
		ClearEffect(e->cancelEffect);
	}

	if (!(e->flags & EffectType::EFF_NOSET)) {
		effects = effects | n;
	}
}

void Soldier::ClearEffect(int eff) {
	if (eff < 0) return;
	int n = 1 << eff;
	int i;

	EffectType *e = &EffectDefs[eff];

	askill -= e->attackVal;

	for (i = 0; i < 4; i++) {
		if (e->defMods[i].type != -1) {
			dskill[e->defMods[i].type] -= e->defMods[i].val;
		}
	}
	effects &= ~n;
}

void Soldier::ClearOneTimeEffects(void) {
	for (int i = 0; i < NUMEFFECTS; i++) {
		if (HasEffect(i) && (EffectDefs[i].flags & EffectType::EFF_ONESHOT))
			ClearEffect(i);
	}
}

int Soldier::ArmorProtect(int weaponClass) {
	//
	// Return 1 if the armor is successful
	//
	int armorType = ARMOR_NONE;
	if (armor > 0) armorType = ItemDefs[armor].index;
	if (armorType < ARMOR_NONE || armorType >= NUMARMORS)
		armorType = ARMOR_NONE;
	ArmorType *pArm = &ArmorDefs[armorType];
	int chance = pArm->saves[weaponClass];

	if (chance > getrandom(pArm->from)) return 1;
	if (shield > 0) {
		armorType = ItemDefs[shield].index;
		pArm = &ArmorDefs[armorType];
		chance = pArm->saves[weaponClass];
		if (chance > getrandom(pArm->from)) return 1;
	}

	return 0;
}

void Soldier::RestoreItems() {
	if (healing && healitem != -1) {
		if (healitem == I_HEALINGPOTION) {
			unit->items.SetNum(healitem,
					unit->items.GetNum(healitem)+healing/10);
		} else {
			unit->items.SetNum(healitem,
					unit->items.GetNum(healitem) + healing);
		}
	}
	if (weapon != -1)
		unit->items.SetNum(weapon,unit->items.GetNum(weapon) + 1);
	if (armor != -1)
		unit->items.SetNum(armor,unit->items.GetNum(armor) + 1);
	if (shield != -1)
		unit->items.SetNum(shield,unit->items.GetNum(shield) + 1);
	if (riding != -1)
		unit->items.SetNum(riding,unit->items.GetNum(riding) + 1);
	if (amulet != -1)
		unit->items.SetNum(amulet,unit->items.GetNum(amulet) + 1);
	if (ring1 != -1)
		unit->items.SetNum(ring1,unit->items.GetNum(ring1) + 1);
	if (ring2 != -1)
		unit->items.SetNum(ring2,unit->items.GetNum(ring2) + 1);

	int battleType;
	for (battleType = 1; battleType < NUMBATTLEITEMS; battleType++) {
		BattleItemType *pBat = &BattleItemDefs[ battleType ];

		if (GET_BIT(battleItems, battleType)) {
			int item = pBat->itemNum;
			unit->items.SetNum(item, unit->items.GetNum(item) + 1);
		}
	}
}

void Soldier::Alive(int state) {
	RestoreItems();

	if (state == LOSS) {
		unit->canattack = 0;
		/* Guards with amuletofi will not go off guard */
		if (!amuletofi &&
			(unit->guard == GUARD_GUARD || unit->guard == GUARD_SET)) {
			unit->guard = GUARD_NONE;
		}
	} else {
		unit->advancefrom = 0;
	}

	if (state == WIN_DEAD) {
		unit->canattack = 0;
		unit->nomove = 1;
	}
}

void Soldier::Dead() {
	RestoreItems();

	unit->SetMen(race,unit->GetMen(race) - 1);
}

Army::Army(Unit * ldr,AList * locs,int regtype,int ass) {
	int tacspell = 0;
	Unit * tactitian = ldr;

	leader = ldr;
	round = 0;
	tac = ldr->GetSkill(S_TACTICS);
	count = 0;
	hitstotal = 0;

	if (ass) {
		count = 1;
		ldr->losses = 0;
	} else {
		forlist(locs) {
			Unit * u = ((Location *) elem)->unit;
			count += u->GetSoldiers();
			u->losses = 0;
			int temp = u->GetSkill(S_TACTICS);
			if (temp > tac) {
				tac = temp;
				tactitian = u;
			}
		}
	}
	tactitian->Practise(S_TACTICS);

	soldiers = new SoldierPtr[count];
	int x = 0;
	int y = count;

	forlist(locs) {
		Unit * u = ((Location *) elem)->unit;
		Object * obj = ((Location *) elem)->obj;
		if (ass) {
			forlist(&u->items) {
				Item * it = (Item *) elem;
				if (it) {
					if (ItemDefs[ it->type ].type & IT_MAN) {
							soldiers[x] = new Soldier(u, obj, regtype,
									it->type, ass);
							hitstotal = soldiers[x]->hits;
							++x;
							goto finished_army;
					}
				}
			}
		} else {
			Item *it = (Item *) u->items.First();
			do {
				if (IsSoldier(it->type)) {
					for (int i = 0; i < it->num; i++) {
						if ((ItemDefs[ it->type ].type & IT_MAN) &&
								u->GetFlag(FLAG_BEHIND)) {
							--y;
							soldiers[y] = new Soldier(u, obj, regtype,
									it->type);
							hitstotal += soldiers[y]->hits;
						} else {
							soldiers[x] = new Soldier(u, obj, regtype,
									it->type);
							hitstotal += soldiers[x]->hits;
							++x;
						}
					}
				}
				it = (Item *) u->items.Next(it);
			} while(it);
		}
	}

finished_army:
	tac = tac + tacspell;

	canfront = x;
	canbehind = count;
	notfront = count;
	notbehind = count;

	hitsalive = hitstotal;

	if (!NumFront()) {
		canfront = canbehind;
		notfront = notbehind;
	}

	for( int i = 0; i < NUM_WEAPON_CLASSES; i++ ) {
		roundAttacks[i] = 0;
		roundHits[i] = 0;
	}
	for( int i = 0; i < NUMSPECIALS; i++ ) {
		specialAttacks[i] = 0;
		specialHits[i] = 0;
	}
}

Army::~Army() {
	delete [] soldiers;
}

void Army::Reset() {
	canfront = notfront;
	canbehind = notbehind;
	notfront = notbehind;
	for( int i = 0; i < NUM_WEAPON_CLASSES; i++ ) {
		roundAttacks[i] = 0;
		roundHits[i] = 0;
	}
	for( int i = 0; i < NUMSPECIALS; i++ ) {
		specialAttacks[i] = 0;
		specialHits[i] = 0;
	}
	roundLeaderReports.DeleteAll();
}

void Army::WriteLosses(Battle * b) {
	b->AddLine(*(leader->name) + " loses " + (count - NumAlive()) + ".");

	if (notbehind != count) {
		AList units;
		for (int i=notbehind; i<count; i++) {
			if (!GetUnitList(&units,soldiers[i]->unit)) {
				UnitPtr *u = new UnitPtr;
				u->ptr = soldiers[i]->unit;
				units.Add(u);
			}
		}

		int comma = 0;
		AString damaged;
		forlist (&units) {
			UnitPtr *u = (UnitPtr *) elem;
			if (comma) {
				damaged += AString(", ") + AString(u->ptr->num);
			} else {
				damaged = AString("Damaged units: ") + AString(u->ptr->num);
				comma = 1;
			}
		}

		units.DeleteAll();
		b->AddLine(damaged + ".");
	}
}

void Army::GetMonSpoils(ItemList *spoils,int monitem, int free) {
	if ((Globals->MONSTER_NO_SPOILS > 0) &&
			(free >= Globals->MONSTER_SPOILS_RECOVERY)) {
		// This monster is in it's period of absolutely no spoils.
		return;
	}

	/* First, silver */
	int silv = MonDefs[ItemDefs[monitem].index].silver;
	if ((Globals->MONSTER_NO_SPOILS > 0) && (free > 0)) {
		// Adjust the spoils for length of freedom.
		silv *= (Globals->MONSTER_SPOILS_RECOVERY-free);
		silv /= Globals->MONSTER_SPOILS_RECOVERY;
	}
	spoils->SetNum(I_SILVER,spoils->GetNum(I_SILVER) + getrandom(silv));

	int spoiltype = MonDefs[ItemDefs[monitem].index].spoiltype;
	// check for an item from the spoils list, if empty continue.
	int thespoil = MonDefs[ItemDefs[monitem].index].spoilitems[getrandom(sizeof(MonDefs[ItemDefs[monitem].index].spoilitems)/sizeof(MonDefs[ItemDefs[monitem].index].spoilitems[0]))];

	if (ItemDefs[thespoil].flags & ItemType::DISABLED) {
	  thespoil = -1;
	}

	if (thespoil == -1) {
	  if (spoiltype == -1) return;
	  if (spoiltype == IT_NORMAL && getrandom(2)) spoiltype = IT_TRADE;

	  int count = 0;
	  int i;
	  for (i=0; i<NITEMS; i++) {
	    if ((ItemDefs[i].type & spoiltype) &&
		!(ItemDefs[i].type & IT_SPECIAL) &&
		!(ItemDefs[i].flags & ItemType::DISABLED)) {
	      count ++;
	    }
	  }

	  count = getrandom(count) + 1;
	  
	  for (i=0; i<NITEMS; i++) {
	    if ((ItemDefs[i].type & spoiltype) &&
		!(ItemDefs[i].type & IT_SPECIAL) &&
		!(ItemDefs[i].flags & ItemType::DISABLED)) {
	      count--;
	      if (count == 0) {
		thespoil = i;
		break;
	      }
	    }
	  }
	}

	int val = getrandom(MonDefs[ItemDefs[monitem].index].silver * 2);
	if ((Globals->MONSTER_NO_SPOILS > 0) && (free > 0)) {
		// Adjust for length of monster freedom.
		val *= (Globals->MONSTER_SPOILS_RECOVERY-free);
		val /= Globals->MONSTER_SPOILS_RECOVERY;
	}

	spoils->SetNum(thespoil,spoils->GetNum(thespoil) +
			(val + getrandom(ItemDefs[thespoil].baseprice)) /
			ItemDefs[thespoil].baseprice);
}

void Army::Regenerate(Battle *b) {
	for (int i = 0; i < count; i++) {
		Soldier *s = soldiers[i];
		if (i<notbehind) {
			int diff = s->maxhits - s->hits;
			if (diff > 0) {
				AString aName = s->name;

				if (s->damage != 0) {
					b->AddLine(aName + " takes " + s->damage +
							" hits bringing it to " + s->hits + "/" +
							s->maxhits + ".");
					s->damage = 0;
				} else {
					b->AddLine(aName + " takes no hits leaving it at " +
							s->hits + "/" + s->maxhits + ".");
				}
				if (s->regen) {
					int regen = s->regen;
					if (regen > diff) regen = diff;
					s->hits += regen;
					b->AddLine(aName + " regenerates " + regen +
							" hits bringing it to " + s->hits + "/" +
							s->maxhits + ".");
				}
			}
		}
	}
}

void Army::Lose(Battle *b,ItemList *spoils) {
	WriteLosses(b);
	for (int i=0; i<count; i++) {
		Soldier * s = soldiers[i];
		if (i<notbehind) {
			s->Alive(LOSS);
		} else {
			if ((s->unit->type==U_WMON) && (ItemDefs[s->race].type&IT_MONSTER))
				GetMonSpoils(spoils,s->race,s->unit->free);
			s->Dead();
		}
		delete s;
	}
}

void Army::Tie(Battle * b) {
	WriteLosses(b);
	for (int x=0; x<count; x++) {
		Soldier * s = soldiers[x];
		if (x<NumAlive()) {
			s->Alive(WIN_DEAD);
		} else {
			s->Dead();
		}
		delete s;
	}
}

int Army::CanBeHealed() {
	for (int i=notbehind; i<count; i++) {
		Soldier * temp = soldiers[i];
		if (temp->canbehealed) return 1;
	}
	return 0;
}

void Army::DoHeal(Battle * b) {
	// Do magical healing
	for (int i = 5; i > 0; --i)
		DoHealLevel(b, i, 0);
	// Do Normal healing
	DoHealLevel(b, 1, 1);
}

void Army::DoHealLevel(Battle *b, int type, int useItems) {
	for (int i=0; i<NumAlive(); i++) {
		Soldier * s = soldiers[i];
		int n = 0;
		if (!CanBeHealed()) break;
		if (s->healtype <= 0) continue;
		// This should be here.. Use the best healing first
		if (s->healtype != type) continue;
		if (!s->healing) continue;
		int rate;
		if (useItems == 0) {
		  rate = HealDefs[type].rate;
		} else {
		  rate = ItemDefs[s->healitem].pValue;
		}
		if (useItems) {
			if (s->healitem == -1) continue;
			if (s->healitem != I_HEALINGPOTION) s->unit->Practise(S_HEALING);
		} else {
			if (s->healitem != -1) continue;
			s->unit->Practise(S_MAGICAL_HEALING);
		}

		while (s->healing) {
			if (!CanBeHealed()) break;
			int j = getrandom(count - NumAlive()) + notbehind;
			Soldier * temp = soldiers[j];
			if (temp->canbehealed) {
				s->healing--;
				if (getrandom(100) < rate) {
					n++;
					soldiers[j] = soldiers[notbehind];
					soldiers[notbehind] = temp;
					notbehind++;
				} else
					temp->canbehealed = 0;
			}
		}
		b->AddLine(*(s->unit->name) + " heals " + n + ".");
	}
}

void Army::Win(Battle * b,ItemList * spoils) {
	int wintype;

	DoHeal(b);
	WriteLosses(b);

	int na = NumAlive();

	if (count - na) wintype = WIN_DEAD;
	else wintype = WIN_NO_DEAD;

	AList units;

	forlist(spoils) {
		Item *i = (Item *) elem;
		if (i && na) {
			Unit *u;
			UnitPtr *up;

			// Make a list of units who can get this type of spoil
			for (int x = 0; x < na; x++) {
				u = soldiers[x]->unit;
				if (u->CanGetSpoil(i)) {
					up = new UnitPtr;
					up->ptr = u;
					units.Add(up);
				}
			}

			int numReceivers = units.Num();
			int numItems = i->num;
			int forceDiscover = 1;

			// Divide spoils equally
			while( numReceivers > 0 && numItems >= numReceivers ) {
				// Give each man in unit one item
				forlist_safe(&units) {
					up = (UnitPtr *)elem;
					up->ptr->items.SetNum(i->type, up->ptr->items.GetNum(i->type)+1 );
					if( forceDiscover )
						up->ptr->faction->DiscoverItem(i->type, 0, 1);

					// Remove unit from list if it can't take any more of this item
					if( !( up->ptr->CanGetSpoil(i) ) ) units.Remove( up );

				}

				// Dont need to give item report to any more receivers
				forceDiscover = 0;

				// update counters
				numItems -= numReceivers;
				numReceivers = units.Num();
			}

                        // Allocate remainder
                        if( numItems && units.Num() ) {
                                for( int x = 0; x < numItems; x++ ) {

                                        // Choose a random receiver
                                        int t = getrandom( units.Num() );
                                        up = 0;
                                        forlist( &units ) {
                                                up = (UnitPtr *) elem;
                                                if( t == 0 ) break;
                                                t--;
                                        }

                                        // Give it an item
                                        if( up ) {
					  up->ptr->items.SetNum(i->type,up->ptr->items.GetNum(i->type)+1);
					  if( forceDiscover )
					    up->ptr->faction->DiscoverItem(i->type, 0, 1);
                                        }
                                        // Only one item each! Remove receiver from list.
                                        units.Remove( up );
                                }
                        }
			
			units.DeleteAll();

/*			int ns = units.Num();
			if (ns > 0) {
				int n = i->num/ns; // Divide spoils equally
				if (n >= 1) {
					forlist(&units) {
						up = (UnitPtr *)elem;
						up->ptr->items.SetNum(i->type,
								up->ptr->items.GetNum(i->type)+n);
						up->ptr->faction->DiscoverItem(i->type, 0, 1);
					}
				}
				n = i->num % ns; // allocate the remainder
				if (n) {
					for (int x = 0; x < n; x++) {
						int t = getrandom(ns);
						up = (UnitPtr *)units.First();
						if (up) {
							UnitPtr *p;
							while(t > 0) {
								p = (UnitPtr *)units.Next(up);
								if (p) up = p;
								else break;
								--t;
							}
							up->ptr->items.SetNum(i->type,
									up->ptr->items.GetNum(i->type)+1);
							up->ptr->faction->DiscoverItem(i->type, 0, 1);
						}
					}
				}
			}
			units.DeleteAll();
*/
		
		}
	}

	for (int x = 0; x < count; x++) {
		Soldier * s = soldiers[x];
		if (x<NumAlive()) s->Alive(wintype);
		else s->Dead();
		delete s;
	}
}

int Army::Broken() {
	if (Globals->ARMY_ROUT == GameDefs::ARMY_ROUT_FIGURES) {
		if ((NumAlive() << 1) < count) return 1;
	} else {
		if ((hitsalive << 1) < hitstotal) return 1;
	}
	return 0;
}

int Army::NumSpoilers() {
	int na = NumAlive();
	int count = 0;
	for (int x=0; x<na; x++) {
		Unit * u = soldiers[x]->unit;
		if (!(u->flags & FLAG_NOSPOILS)) count++;
	}
	return count;
}

int Army::NumAlive() {
	return notbehind;
}

int Army::CanAttack() {
	return canbehind;
}

int Army::NumFront() {
	return (canfront + notfront - canbehind);
}

int Army::NumBehind() {
  return NumAlive() - NumFront();
}

int Army::NumFrontHits() {
	int totHits = 0;

	for (int i = 0; i < canfront; i++) {
		totHits += soldiers[i]->maxhits;
	}
	for (int i = canbehind; i < notfront; i++) {
		totHits += soldiers[i]->maxhits;
	}
	return totHits;
}

Soldier * Army::GetAttacker(int i,int &behind) {
	Soldier * retval = soldiers[i];
	if (i<canfront) {
		soldiers[i] = soldiers[canfront-1];
		soldiers[canfront-1] = soldiers[canbehind-1];
		soldiers[canbehind-1] = retval;
		canfront--;
		canbehind--;
		behind = 0;
		return retval;
	}
	soldiers[i] = soldiers[canbehind-1];
	soldiers[canbehind-1] = soldiers[notfront-1];
	soldiers[notfront-1] = retval;
	canbehind--;
	notfront--;
	behind = 1;
	return retval;
}

int Army::GetTargetNum(int special, int canAttackBehind) {
	int tars = NumFront();
	if (canAttackBehind) {
		//tars += NumAlive();
		tars = NumAlive();
	}
	if (tars == 0) {
		canfront = canbehind;
		notfront = notbehind;
		tars = NumFront();
		if (tars == 0) {
			return -1;
		}
	}

	if (SpecialDefs[special].targflags) {
		int validtargs = 0;
		int i, start = -1;

		for (i = 0; i < canfront; i++) {
			if (CheckSpecialTarget(special, i)) {
				validtargs++;
				if (start == -1) start = i;
			}
		}
		for (i = canbehind; i < notfront; i++) {
			if (CheckSpecialTarget(special, i)) {
				validtargs++;
				if (start == -1) start = i;
			}
		}
		if (validtargs) {
			int targ = getrandom(validtargs);
			for (i = start; i < notfront; i++) {
				if (i == canfront) i = canbehind;
				if (CheckSpecialTarget(special, i)) {
					if (!targ--) return i;
				}
			}
		}
	} else {
		int i = getrandom(tars);
		if (canAttackBehind) {
			return i;
		} else {
			if (i<canfront) return i;
			return i + canbehind - canfront;
		}
	}
	return -1;
}

int Army::GetEffectNum(int effect) {
	int validtargs = 0;
	int i, start = -1;

	for (i = 0; i < canfront; i++) {
		if (soldiers[i]->HasEffect(effect)) {
			validtargs++;
			// slight scan optimisation - skip empty initial sequences
			if (start == -1) start = i;
		}
	}
	for (i = canbehind; i < notfront; i++) {
		if (soldiers[i]->HasEffect(effect)) {
			validtargs++;
			// slight scan optimisation - skip empty initial sequences
			if (start == -1) start = i;
		}
	}
	if (validtargs) {
		int targ = getrandom(validtargs);
		for (i = start; i < notfront; i++) {
			if (i == canfront) i = canbehind;
			if (soldiers[i]->HasEffect(effect)) {
				if (!targ--) return i;
			}
		}
	}
	return -1;
}

Soldier * Army::GetTarget(int i) {
	return soldiers[i];
}

int pow(int b,int p) {
	int b2 = b;
	for (int i=1; i<p; i++) {
		b2 *= b;
	}
	return b2;
}

int Hits(int a,int d) {
	int tohit = 1,tomiss = 1;
	if (a>d) {
		tohit = pow(2,a-d);
	} else if (d>a) {
		tomiss = pow(2,d-a);
	}
	if (getrandom(tohit+tomiss) < tohit) return 1;
	return 0;
}

int Army::RemoveEffects(int num, int effect) {
	int ret = 0;
	for (int i = 0; i < num; i++) {
		//
		// Try to find a target unit.
		//
		int tarnum = GetEffectNum(effect);
		if (tarnum == -1) continue;
		Soldier *tar = GetTarget(tarnum);

		//
		// Remove the effect
		//
		tar->ClearEffect(effect);
		ret++;
	}
	return(ret);
}

int Army::DoAnAttack(int special, int numAttacks, int attackType,
		int attackLevel, int flags, int weaponClass, int effect,
		int mountBonus, int attackbehind) {
	int debug = 0;
	/* 1. Check against Global effects (not sure how yet) */
	/* 2. Attack shield */
	Shield *hi;
	int combat = 0;
	int canShield = 0;
	switch(attackType) {
		case ATTACK_RANGED:
			canShield = 1;
			// fall through
		case ATTACK_COMBAT:
		case ATTACK_RIDING:
			combat = 1;
			break;
		case ATTACK_ENERGY:
		case ATTACK_WEATHER:
		case ATTACK_SPIRIT:
			canShield = 1;
			break;
	}

	if (canShield) {
		int shieldType = attackType;

		hi = shields.GetHighShield(shieldType);
		if (hi) {
			/* Check if we get through shield */
			if (!Hits(attackLevel, hi->shieldskill)) {
				return -1;
			}

			if (!effect && !combat) {
				/* We got through shield... if killing spell, destroy shield */
				shields.Remove(hi);
				delete hi;
			}
		}
	}

	//
	// Now, loop through and do attacks
	//
	int ret = 0;
	for (int i = 0; i < numAttacks; i++) {
		/* 3. Get the target */
		int tarnum = GetTargetNum(special,attackbehind);
		if (tarnum == -1) continue;
		Soldier * tar = GetTarget(tarnum);

//		if( tar->unit->num == 62 && special ) debug = 1;
		if( debug ) {
			Awrite( AString( "Attack against " ) + tar->name + "; askill " + attackLevel +
				"; attackBehind " + attackbehind + "; attack type " + attackType + "." );
		}
		// 3.1  50% chance of attacking mount
		if( tar->mount ) {
			if( getrandom(2) ) {
				// attack mount
				if( debug ) Awrite( "- Attacking mount instead of rider." );
				tar = tar->mount;
			}
		}
		int tarFlags = 0;
		if (tar->weapon != -1) {
			tarFlags = WeaponDefs[ItemDefs[tar->weapon].index].flags;
		}

		/* 4. Add in any effects, if applicable */
		int tlev = 0;
		if (attackType != NUM_ATTACK_TYPES)
			tlev = tar->dskill[ attackType ];
		if (special > 0) {
			if ((SpecialDefs[special].effectflags&SpecialType::FX_NOBUILDING) &&
					tar->building) {
				tlev -= 2;
			}
		}

		if( debug ) {
			AString temp = "Defense array: [ ";
			for( int i = 0; i < NUM_ATTACK_TYPES; i++ ) {
				temp += tar->dskill[ i ];
				temp += " ";
			}
			temp += " ]";
			Awrite( temp );
			Awrite( AString( "- Attacking: " ) + attackLevel + " vs " + tlev + " (first check)." );
		}

		/* 4.1 Check whether defense is allowed against this weapon */
		if ((flags & WeaponType::NODEFENSE) && (tlev > 0)) tlev = 0;

		if (!(flags & WeaponType::RANGED)) {
			/* 4.2 Check relative weapon length */
			int attLen = 1;
			int defLen = 1;
			if (flags & WeaponType::LONG) attLen = 2;
			else if (flags & WeaponType::SHORT) attLen = 0;
			if (tarFlags & WeaponType::LONG) defLen = 2;
			else if (tarFlags & WeaponType::SHORT) defLen = 0;
			if (attLen > defLen) attackLevel++;
			else if (defLen > attLen) tlev++;
		}

		/* 4.3 Add bonuses versus mounted */
		if (tar->riding != -1) attackLevel += mountBonus;

		if( debug ) {
			Awrite( AString( "- Attacking: " ) + attackLevel + " vs " + tlev + "." );
		}
		/* 5. Attack soldier */
		if (attackType != NUM_ATTACK_TYPES &&
			attackType != ATTACK_DISPEL ) {
			if (!(flags & WeaponType::ALWAYSREADY)) {
				if (getrandom(2)) {
					if( debug ) {
						Awrite( "--Misses!" );
					}
					continue;
				}
			}

			if (!Hits(attackLevel,tlev)) {
				if( debug ) {
					Awrite( "--Fails to hit!" );
				}
				continue;
			}
		}
		if( debug ) {
			Awrite( "--Hits!" );
		}

		/* 6. If attack got through, apply effect, or kill */
		if (!effect) {
			/* 7. Last chance... Check armor */
			if (tar->ArmorProtect(weaponClass)) {
				if( debug ) {
					Awrite( "---Protected by armour!" );
				}
				continue;
			}

			/* 8. Seeya! */
			// Did we hit a mount?
			if( tar->rider ) {
				if (!tar->amuletofi) {
					int hitsTaken = 1;
					if( attackType == ATTACK_DISPEL ) {
						hitsTaken = tar->hits;
					}
					tar->damage += hitsTaken;
					tar->hits -= hitsTaken;
					if( tar->hits <= 0 ) {
						if( debug ) {
							Awrite( "---Mount dead!" );
						}
						tar = tar->rider;
						delete tar->mount;
						tar->mount = 0;
						tar->riding = -1;
					}
				}
			} else {
				if( debug ) {
					Awrite( AString("---Soldier takes 1 hit, taking it down to ") + tar->hits + ".");
				}
				Kill(tarnum, attackType == ATTACK_DISPEL);
			}
			ret++;
		} else {
			if (tar->HasEffect(effect)) {
				continue;
			}
			tar->SetEffect(effect);
			ret++;
		}
	}
	return ret;
}

void Army::Kill(int killed, int destroy) {
	Soldier *temp = soldiers[killed];

	if (temp->amuletofi) return;

	int hitsTaken = 1;
	if( destroy )
		hitsTaken = temp->hits;

	if (Globals->ARMY_ROUT == GameDefs::ARMY_ROUT_HITS_INDIVIDUAL)
		hitsalive -= hitsTaken;
	temp->damage += hitsTaken;
	temp->hits -= hitsTaken;
	if (temp->hits > 0) return;
	temp->unit->losses++;
	if (Globals->ARMY_ROUT == GameDefs::ARMY_ROUT_HITS_FIGURE) {
		if (ItemDefs[temp->race].type & IT_MONSTER) {
			hitsalive -= MonDefs[ItemDefs[temp->race].index].hits;
		} else {
			// Assume everything that is a soldier and isn't a monster is a
			// man.
			hitsalive--;
		}
	}

	if (killed < canfront) {
		soldiers[killed] = soldiers[canfront-1];
		soldiers[canfront-1] = temp;
		killed = canfront - 1;
		canfront--;
	}

	if (killed < canbehind) {
		soldiers[killed] = soldiers[canbehind-1];
		soldiers[canbehind-1] = temp;
		killed = canbehind-1;
		canbehind--;
	}

	if (killed < notfront) {
		soldiers[killed] = soldiers[notfront-1];
		soldiers[notfront-1] = temp;
		killed = notfront-1;
		notfront--;
	}

	soldiers[killed] = soldiers[notbehind-1];
	soldiers[notbehind-1] = temp;
	notbehind--;
}
