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
#include "game.h"
#include "battle.h"
#include "army.h"
#include "gamedefs.h"
#include "gamedata.h"
#include "items.h"

#define ADD_ATTACK 1
#define ADD_DEFENSE 2

Battle::Battle() {
	asstext = 0;
}

Battle::~Battle() {
	if (asstext) {
		delete asstext;
	}
}

void Battle::FreeRound(Army * att,Army * def, int ass) {
	/* Write header */
	AddLine(*(att->leader->name) + " gets a free round of attacks.");

	/* Update both army's shields */
	att->shields.DeleteAll();
	UpdateShields(att);

	def->shields.DeleteAll();
	UpdateShields(def);

	//
	// Update the attacking armies round counter
	//
	att->round++;

	int overwhelmed = 0;
	if (att->NumFront() > 4 * def->NumFront() && def->NumFront() && def->NumBehind()) {
		AddLine(*(def->leader->name) + " is overwhelmed.");
		overwhelmed = 1;
	}
	/* Run attacks until done */
	int alv = def->NumAlive();
	while (att->CanAttack() && def->NumAlive()) {
		int num = getrandom(att->CanAttack());
		int behind;
		Soldier * a = att->GetAttacker(num, behind);
		DoAttack(att->round, a, att, def, behind, ass, overwhelmed);
	}

	/* Write losses */
	def->Regenerate(this);
	alv -= def->NumAlive();
	if( !Globals->AGGREGATE_BATTLE_REPORTS ) {
		AddLine(*(def->leader->name) + " loses " + alv + ".");
	} else {
		int ok = 0;
		for( int i = 0; i < NUM_WEAPON_CLASSES; i++ ) {
			if( att->roundAttacks[i] > 0 ) {
				ok = 1;
				break;
			}
		}
		if( !ok ) {
			for( int i = 0; i < NUMSPECIALS; i++ ) {
				if( att->specialAttacks[i] > 0 ) {
					ok = 1;
					break;
				}
			}
		}
		if( ok ) {
			AddLine( "Attacker does:" );
			for( int i = 0; i < NUM_WEAPON_CLASSES; i++ ) {
				if( att->roundAttacks[i] == 0 ) continue;
				AddLine( AString( " " ) + att->roundAttacks[i] + " " + WeapClass( i ) + 
					" attack" + (att->roundAttacks[i] == 1 ? "" : "s") + 
					" killing " + att->roundHits[i] + "." );
			}
			for( int i = 0; i < NUMSPECIALS; i++ ) {
				if( att->specialAttacks[i] == 0 ) continue;
				AddLine( AString( " " ) + att->specialAttacks[i] + " " + 
					SpecialDefs[i].attackdesc +
					" attack" + (att->specialAttacks[i] == 1 ? "" : "s") + 
					" killing " + att->specialHits[i] + "." );
			}
		}
		forlist( &att->roundLeaderReports ) {
			AddLine( *( AString * ) elem );
		}
		AddLine( AString( "Defender loses " ) + alv + ".");
		
	}
	AddLine("");
	att->Reset();
}

void Battle::DoAttack(int round, Soldier *a, Army *attackers, Army *def,
			int behind, int ass, int canattackback)
{
	DoSpecialAttack(round, a, attackers, def, behind, canattackback);
	if (!def->NumAlive()) return;

	if (!behind && (a->riding != -1)) {
		MountType *pMt = &MountDefs[ItemDefs[a->riding].index];
		if (pMt->mountSpecial != -1) {
			int i, num, tot = -1;
			SpecialType *spd = &SpecialDefs[pMt->mountSpecial];
			for(i = 0; i < 4; i++) {
				int times = spd->damage[i].value;
				if (spd->effectflags & SpecialType::FX_USE_LEV)
					times *= pMt->specialLev;
				int realtimes = spd->damage[i].minnum + getrandom(times) +
					getrandom(times);
				num  = def->DoAnAttack(pMt->mountSpecial, realtimes,
						spd->damage[i].type, pMt->specialLev,
						spd->damage[i].flags, spd->damage[i].dclass,
						spd->damage[i].effect, 0, canattackback);
				if (num != -1) {
					if (tot == -1) tot = num;
					else tot += num;
					//attackers->roundHits[spd->damage[i].dclass] += num;
					attackers->specialHits[pMt->mountSpecial] += num;		
				}
				attackers->specialAttacks[pMt->mountSpecial] += realtimes;
			}
			if (tot != -1) {
				if( !Globals->AGGREGATE_BATTLE_REPORTS ) {
					AddLine(a->name + " " + spd->spelldesc + ", " +
							spd->spelldesc2 + tot + spd->spelltarget + ".");
				} 
			}
		}
	}
	if (!def->NumAlive()) return;

	int numAttacks = a->attacks;
	int totHit = 0;
	int fullreport = 0;

	if( a->unit->GetMen() == 1 && ItemDefs[a->race].type & IT_MAN &&
		ManDefs[ItemDefs[a->race].index].flags & ManType::LEADER )
	{
		fullreport = 1;
	}

	if (a->attacks < 0) {
		if (round % (-1 * a->attacks) == 1)
			numAttacks = 1;
		else
			numAttacks = 0;
	} else if (ass && (Globals->MAX_ASSASSIN_FREE_ATTACKS > 0) &&
			(numAttacks > Globals->MAX_ASSASSIN_FREE_ATTACKS)) {
		numAttacks = Globals->MAX_ASSASSIN_FREE_ATTACKS;
	}

	WeaponType *pWep = 0;
	if (a->weapon != -1) pWep = &WeaponDefs[ItemDefs[a->weapon ].index];
	int flags = WeaponType::SHORT;
	int attackType = ATTACK_COMBAT;
	int mountBonus = 0;
	int attackClass = SLASHING;
	if (pWep) {
		flags = pWep->flags;
		attackType = pWep->attackType;
		mountBonus = pWep->mountBonus;
		attackClass = pWep->weapClass;
	}
	if (behind) {
		if (!pWep || (!(pWep->flags & WeaponType::RANGED)) )
			numAttacks = 0;
	}

	for (int i = 0; i < numAttacks; i++) {

		int numHit = def->DoAnAttack(0, 1, attackType, a->askill, flags, attackClass,
				0, mountBonus, canattackback);
		if( numHit != -1 ) totHit += numHit;

		if (!def->NumAlive()) break;
	}

	if( Globals->AGGREGATE_BATTLE_REPORTS && numAttacks > 0 ) {
		if( fullreport ) {
			if( totHit < 0 ) totHit = 0;
			AString temp = a->name + " does " + numAttacks + " " + WeapClass( attackClass ) +
				" attack" + ( numAttacks == 1 ? "" : "s" ) + " killing " + totHit + ".";
			attackers->roundLeaderReports.Add( new AString( temp ) );
		} else {
			attackers->roundHits[attackClass] += totHit;
			attackers->roundAttacks[attackClass] += numAttacks;
		}
	}

	a->ClearOneTimeEffects();

	// Mount gets to attack too
	if( a->mount )
		DoAttack(round, a->mount, attackers, def, behind, ass, canattackback);
	
}

void Battle::NormalRound(int round,Army * a,Army * b) {
	/* Write round header */
	AddLine(AString("Round ") + round + ":");

	/* Update both army's shields */
	UpdateShields(a);
	UpdateShields(b);

	/* Initialize variables */
	a->round++;
	b->round++;
	int aalive = a->NumAlive();
	int aialive = aalive;
	int balive = b->NumAlive();
	int bialive = balive;
	int aatt = a->CanAttack();
	int batt = b->CanAttack();

	int boverwhelmed = 0;
	int aoverwhelmed = 0;
	if (a->NumFront() > 4 * b->NumFront() && b->NumFront() && b->NumBehind()) {
	  AddLine(*(b->leader->name) + " is overwhelmed.");
	  boverwhelmed = 1;
	} else if (b->NumFront() > 4 * a->NumFront() && a->NumFront() && a->NumBehind()) {
	  AddLine(*(a->leader->name) + " is overwhelmed.");
	  aoverwhelmed = 1;
	}

	/* Run attacks until done */
	while (aalive && balive && (aatt || batt))
	{
		int num = getrandom(aatt + batt);
		int behind;
		if (num >= aatt)
		{
			num -= aatt;
			Soldier * s = b->GetAttacker(num, behind);
			DoAttack(b->round, s, b, a, behind, 0, aoverwhelmed);
		}
		else
		{
			Soldier * s = a->GetAttacker(num, behind);
			DoAttack(a->round, s, a, b, behind, 0, boverwhelmed);
		}
		aalive = a->NumAlive();
		balive = b->NumAlive();
		aatt = a->CanAttack();
		batt = b->CanAttack();
	}

	/* Finish round */
	a->Regenerate(this);
	b->Regenerate(this);
 	aialive -= aalive;
	bialive -= balive;
	if( !Globals->AGGREGATE_BATTLE_REPORTS ) {
		AddLine(*(a->leader->name) + " loses " + aialive + ".");
		AddLine(*(b->leader->name) + " loses " + bialive + ".");
	} else {
		int ok = 0;
		for( int i = 0; i < NUM_WEAPON_CLASSES; i++ ) {
			if( a->roundAttacks[i] > 0 ) {
				ok = 1;
				break;
			}
		}
		if( !ok ) {
			for( int i = 0; i < NUMSPECIALS; i++ ) {
				if( a->specialAttacks[i] > 0 ) {
					ok = 1;
					break;
				}
			}
		}
		if( ok ) {
			AddLine( "Attacker does:" );
			for( int i = 0; i < NUM_WEAPON_CLASSES; i++ ) {
				if( a->roundAttacks[i] == 0 ) continue;
				AddLine( AString( " " ) + a->roundAttacks[i] + " " + WeapClass( i ) + 
					" attack" + (a->roundAttacks[i] == 1 ? "" : "s") + 
					" killing " + a->roundHits[i] + "." );
			}
			for( int i = 0; i < NUMSPECIALS; i++ ) {
				if( a->specialAttacks[i] == 0 ) continue;
				AddLine( AString( " " ) + a->specialAttacks[i] + " " +
					SpecialDefs[i].attackdesc +
					" attack" + (a->specialAttacks[i] == 1 ? "" : "s") + 
					" killing " + a->specialHits[i] + "." );
			}
		}
		forlist( &a->roundLeaderReports ) {
			AddLine( *( AString * ) elem );
		}
		AddLine( AString( "Defender loses " ) + bialive + ".");
		ok = 0;
		for( int i = 0; i < NUM_WEAPON_CLASSES; i++ ) {
			if( b->roundAttacks[i] > 0 ) {
				ok = 1;
				break;
			}
		}
		if( !ok ) {
			for( int i = 0; i < NUMSPECIALS; i++ ) {
				if( b->specialAttacks[i] > 0 ) {
					ok = 1;
					break;
				}
			}
		}
		if( ok ) {
			AddLine( "Defender does:" );
			for( int i = 0; i < NUM_WEAPON_CLASSES; i++ ) {
				if( b->roundAttacks[i] == 0 ) continue;
				AddLine( AString( " " ) + b->roundAttacks[i] + " " + WeapClass( i ) + 
					" attack" + (b->roundAttacks[i] == 1 ? "" : "s") + 
					" killing " + b->roundHits[i] + "." );
			}
			for( int i = 0; i < NUMSPECIALS; i++ ) {
				if( b->specialAttacks[i] == 0 ) continue;
				AddLine( AString( " " ) + b->specialAttacks[i] + " " +
					SpecialDefs[i].attackdesc +
					" attack" + (b->specialAttacks[i] == 1 ? "" : "s") + 
					" killing " + b->specialHits[i] + "." );
			}
		}
		{
			forlist( &b->roundLeaderReports ) {
				AddLine( *( AString * ) elem );
			}
		}
		AddLine( AString( "Attacker loses " ) + aialive + ".");
	}
	AddLine("");
	a->Reset();
	b->Reset();
}

void Battle::GetSpoils(AList * losers, ItemList *spoils, int ass) {
	forlist(losers) {
		Unit * u = ((Location *) elem)->unit;
		int numalive = u->GetSoldiers();
		int numdead = u->losses;
		forlist(&u->items) {
			Item * i = (Item *) elem;
			if (IsSoldier(i->type)) continue;
			if (ItemDefs[i->type].attributes & ItemType::STOP_ASSASINATE) continue;
			float percent = (float)numdead/(float)(numalive+numdead);
			int num = (int)(i->num * percent);
			int num2 = (num + getrandom(2))/2;
			spoils->SetNum(i->type, spoils->GetNum(i->type) + num2);
			u->items.SetNum(i->type, i->num - num);
		}
	}
}

int Battle::Run( ARegion * region, Unit * att, AList * atts, AList * targets,
		         AList * defs, int ass, ARegionList *pRegs)
{
	Army * armies[2];
	AString temp;
	assassination = ASS_NONE;
	attacker = att->faction;
	Unit * tar = (Unit *)((UnitPtr *)targets->First())->ptr;

	armies[0] = new Army(att,atts,region->type,ass);
	armies[1] = new Army(tar,defs,region->type,ass);

	if (ass) {
		FreeRound(armies[0],armies[1], ass);
	} else {
		if (armies[0]->tac > armies[1]->tac) FreeRound(armies[0],armies[1]);
		if (armies[1]->tac > armies[0]->tac) FreeRound(armies[1],armies[0]);
	}

	int round = 1;
	while (!armies[0]->Broken() && !armies[1]->Broken() && round < 101) {
		NormalRound(round++,armies[0],armies[1]);
	}

	if ((armies[0]->Broken() && !armies[1]->Broken()) ||
		(!armies[0]->NumAlive() && armies[1]->NumAlive())) {
		if (ass) assassination = ASS_FAIL;

		if (armies[0]->NumAlive()) {
		  AddLine(*(armies[0]->leader->name) + " is routed!");
		  FreeRound(armies[1],armies[0]);
		} else {
		  AddLine(*(armies[0]->leader->name) + " is destroyed!");
		}
		AddLine("Total Casualties:");
		ItemList *spoils = new ItemList;
		armies[0]->Lose(this, spoils);
		GetSpoils(atts, spoils, ass);
		if (spoils->Num()) {
			temp = AString("Spoils: ") + spoils->Report(2,0,1) + ".";
		} else {
			temp = "Spoils: none.";
		}
		armies[1]->Win(this, spoils);
		AddLine("");
		AddLine(temp);
		AddLine("");
		delete spoils;
		delete armies[0];
		delete armies[1];
		return BATTLE_LOST;
	}

	if ((armies[1]->Broken() && !armies[0]->Broken()) ||
		(!armies[1]->NumAlive() && armies[0]->NumAlive())) {
		if (ass) {
			assassination = ASS_SUCC;
			asstext = new AString(*(armies[1]->leader->name) +
			  " is assassinated in " +
			  region->ShortPrint(pRegs) +
			  "!");
		}
		if (armies[1]->NumAlive()) {
		  AddLine(*(armies[1]->leader->name) + " is routed!");
			FreeRound(armies[0],armies[1]);
		} else {
		  AddLine(*(armies[1]->leader->name) + " is destroyed!");
		}
		AddLine("Total Casualties:");
		ItemList *spoils = new ItemList;
		armies[1]->Lose(this, spoils);
		GetSpoils(defs, spoils, ass);
		if (spoils->Num()) {
			temp = AString("Spoils: ") + spoils->Report(2,0,1) + ".";
		} else {
			temp = "Spoils: none.";
		}
		armies[0]->Win(this, spoils);
		AddLine("");
		AddLine(temp);
		AddLine("");
		delete spoils;
		delete armies[0];
		delete armies[1];
		return BATTLE_WON;
	}

	AddLine("The battle ends indecisively.");
	AddLine("");
	AddLine("Total Casualties:");
	armies[0]->Tie(this);
	armies[1]->Tie(this);
	temp = "Spoils: none.";
	AddLine("");
	AddLine(temp);
	AddLine("");
	delete armies[0];
	delete armies[1];
	return BATTLE_DRAW;
}

void Battle::WriteSides( ARegion * r, Unit * att, AList * targets,
			             AList * atts, AList * defs, int ass,
			             ARegionList *pRegs )
{
	Unit * tar = (Unit *)((UnitPtr *) targets->First())->ptr;
	if (ass) {
		AddLine(*att->name + " attempts to assassinate " + *tar->name
			+ " in " + r->ShortPrint(pRegs) + "!");
	} else {
		AddLine(*att->name + " attacks " + *tar->name + " in " +
			r->ShortPrint(pRegs) + "!");
	}
	AddLine("");

	int dobs = 0;
	int aobs = 0;
	{
		forlist(defs) {
			int a = ((Location *)elem)->unit->GetObservation();
			if (a > dobs) dobs = a;
		}
	}

	AddLine("Attackers:");
	{
		forlist(atts) {
			int a = ((Location *)elem)->unit->GetObservation();
			if (a > aobs) aobs = a;
			AString * temp = ((Location *) elem)->unit->BattleReport(dobs);
			AddLine(*temp);
			delete temp;
		}
	}
	AddLine("");
	AddLine("Defenders:");
	{
		forlist(defs) {
			AString * temp = ((Location *) elem)->unit->BattleReport(aobs);
			AddLine(*temp);
			delete temp;
		}
	}
	AddLine("");
}

void Battle::Report(Areport * f,Faction * fac) {
	if (assassination == ASS_SUCC && fac != attacker) {
		f->PutStr(*asstext);
		f->PutStr("");
		return;
	}
	forlist(&text) {
		AString * s = (AString *) elem;
		f->PutStr(*s);
	}
}

void Battle::AddLine(const AString & s) {
	AString * temp = new AString(s);
	text.Add(temp);
}

void Game::GetDFacs( ARegion * r, Unit * attacker, AList * targets,
					 AList & defs, AList & dfacs )
{
	int debug = 0;

	// Cycle through units in the region to see if they will defend against the attack
	forlist( &r->objects ) {
		Object * obj = (Object *) elem;
		forlist( &obj->units ) {
			Unit * u = (Unit *) elem;
			int add = 0;

			if( !u->IsAlive() ) continue;
//			if( attacker->GetAttitude( r, u ) == A_HOSTILE ) add = 1;
//			if( u->type == U_GUARD || u->type == U_GUARDMAGE ) add = 1;

			// check if unit has been attacked, or if it can help a
			// unit that has been attacked
			forlist( targets ) {
				UnitPtr * up = (UnitPtr *) elem;
				Unit * t = (Unit *) up->ptr;
				if( u == t ) {
					// unit has been attacked, so add unit as a defender
					if( debug ) {
						Awrite( AString("-- Unit targetted: ") + *u->name );
					}
					add = 1;
					break;
				}
				if( t->GetFlag( FLAG_NOAID ) ) {
					// this target wants no aid from anyone else, so no
					// need to check if current unit can help this target
					continue;
				}

				if( u->GetAttitude( r, t ) == A_ALLY ) {
					// unit may be able to help the target, check flags first
					if( u->guard != GUARD_AVOID ) {
						// flags OK, add unit as a defender
						add = 1;
						if( debug ) {
							Awrite( AString("-- Unit ") + *u->name + " allied to target " + *t->name );
						}
						break;
					}
				}
			}

			if( add ) {
				// Add this unit as a defender
				Location * l = new Location;
				l->unit = u;
				l->obj = u->object;
				l->region = r;
				defs.Add( l );

				// Add faction to defenders
				if( !GetFaction2( &dfacs, u->faction->num ) ) {
					FactionPtr * fp = new FactionPtr;
					fp->ptr = u->faction;
					dfacs.Add(fp);
				}
			}
		} 
	}
}

void Game::GetAFacs( ARegion * r, Unit * att, AList * targets,
					 AList & atts, AList & dfacs, AList & afacs )
{
	int debug = 0;

	// If attacker is noaid, it will not draw in any helpers
	if( att->GetFlag( FLAG_NOAID ) ) {
		// Add this unit as an attacker
		if( debug ) Awrite( "- Attacker is NOAID!" );
		Location * l = new Location;
		l->unit = att;
		l->obj = r->GetDummy();
		l->region = r;
		atts.Add( l );

		// Add faction to attackers
		FactionPtr * fp = new FactionPtr;
		fp->ptr = att->faction;
		afacs.Add(fp);
	}

	// Cycle through units in the region to see if they will join in the attack
	forlist( &r->objects ) {
		Object * obj = (Object *) elem;
		forlist( &obj->units ) {
			Unit * u = (Unit *) elem;

			if( debug )  Awrite( AString( "- Checking if unit " ) + u->num + " can attack..." );

			if( !u->IsAlive() ) {
				if( debug ) Awrite( "-- Unit is not alive!" );
				continue;

			}

			if( !u->canattack ) {
				if( debug ) Awrite( "-- Unit cannot attack!" );
				continue;
			}

			int add = 0;
				
			if( u->faction == att->faction && ( u->guard != GUARD_AVOID || u == att ) ) {
				// unit is from same faction and not avoiding, add unit to attackers
				add = 1;
				if( debug ) Awrite( "-- Unit is from same faction and not avoiding, add unit to attackers" );
			} else if( GetFaction2( &dfacs, u->faction->num ) ) {
				// unit's faction is already on the defending side!
				if( debug ) Awrite( "-- Unit's faction is already on the defending side! Skip" );
				add = 0;
				continue;
			} else {
				if( debug ) Awrite( "--- Checking if unit's target matches this one." );

				forlist( targets ) {
					UnitPtr * up = (UnitPtr *) elem;
					Unit * tar = (Unit *) up->ptr;

					if( add ) continue;

					if( u->GetAttitude( r, tar ) == A_HOSTILE && u->guard != GUARD_AVOID ) {
						// unit is hostile to target, add unit to attackers
						add = 1;
						continue;
					}

					if( u->guard == GUARD_ADVANCE && tar->Forbids( r, u ) ) {
						// unit has advanced against this target, add unit to attackers
						add = 1;
						continue;
					}

					// cycle through unit's attack orders
					forlist( &u->attackorders ) {
						AttackOrder * order = (AttackOrder *) elem;
						forlist( &order->targets ) {
							UnitId * id = (UnitId *) elem;
							Unit * t = r->GetUnitId( id, u->faction->num );
							if( !t ) continue;
							if( t == tar ) {
								// Found target in attack orders, add unit to attackers
								// and remove target from orders
								order->targets.Remove( id );
								delete id;
								add = 1;
								break;
							}
						}
						if( add ) break;
					}
				}
			}

			if( add ) {
				// Add this unit as an attacker
				Location * l = new Location;
				l->unit = u;
				l->obj = r->GetDummy();
				l->region = r;
				atts.Add( l );

				// Add faction to attackers
				if( !GetFaction2( &dfacs, u->faction->num ) ) {
					FactionPtr * fp = new FactionPtr;
					fp->ptr = u->faction;
					afacs.Add(fp);
				}
			}
		}
	}
}


int Game::CanAttack(ARegion * r,AList * afacs,Unit * u) {
	int see = 0;
	int ride = 0;
	forlist(afacs) {
		FactionPtr * f = (FactionPtr *) elem;
		if (f->ptr->CanSee(r,u) == 2) {
			if (ride == 1) return 1;
			see = 1;
		}
		if (f->ptr->CanCatch(r,u)) {
			if (see == 1) return 1;
			ride = 1;
		}
	}
	return 0;
}

void Game::GetSides( ARegion *r, AList & afacs, AList & dfacs, AList & atts,
		             AList & defs, Unit * attacker, AList * targets, int ass,
					 int adv )
{
	if (ass) {
		Unit * tar = (Unit *)((UnitPtr *)targets->First())->ptr;

		/* Assassination attempt */
		Location * l = new Location;
		l->unit = attacker;
		l->obj = r->GetDummy();
		l->region = r;
		atts.Add(l);

		l = new Location;
		l->unit = tar;
		l->obj = r->GetDummy();
		l->region = r;
		defs.Add(l);

		return;
	}

	int j=NDIRS;
	int noaida = 0, noaidd = 0;
	for (int i=-1;i<j;i++) {
		ARegion * r2 = r;
		if (i>=0) {
			r2 = r->neighbors[i];
			if (!r2) continue;
			forlist(&r2->objects) {
				/* Can't get building bonus in another region */
				((Object *) elem)->capacity = 0;
			}
		} else {
			forlist(&r2->objects) {
				Object * o = (Object *) elem;
				/* Set building capacity */
				if (o->incomplete < 1 && o->IsBuilding()) {
					o->capacity = ObjectDefs[o->type].protect;
				}
			}
		}
		forlist (&r2->objects) {
			Object * o = (Object *) elem;
			forlist (&o->units) {
				Unit * u = (Unit *) elem;
				int add = 0;

				// First, can the unit be involved in the battle at all?
				if( !u->IsAlive() ) continue;
				if( i != -1 && u->GetFlag( FLAG_HOLDING ) ) continue;
				// check avoid flag?

				if( GetFaction2(&afacs,u->faction->num) ) {
					// The unit is on the attacking side, check if the
					// unit should be in the battle
					if( !u->canattack ) continue;
					if( u->guard == GUARD_AVOID ) {
						// unit is avoiding, and will only attack if it has issued
						// a specific attack order, in which case it has already been added
						// to the attacker list
						continue;
					}
					if( noaida ) {
						// Attacker has set no aid, so *no-one* should aid the attacker
						continue;
					}

					if( i != -1 && !u->CanMoveTo(r2,r) ) {
						// Unit is in a neighbouring region, but cannot reach the battle
						continue;
					}
					
					if( ::GetUnit(&atts,u->num) ) {
						// Unit is already in the attacker list!
						continue;
					}
					add = ADD_ATTACK;

				} else {
					// The unit is not on the attacking side

					// First, check for the noaid flag; if it is set,
					// only units from this region will join on the
					// defensive side
					
					if( i != -1 && noaidd ) {
						// Unit is in neighbouring region, but defenders no not
						// want aid
						continue;
					}

					if( GetFaction2(&dfacs,u->faction->num) ) {
						// Unit is on the defending side
						if( u->guard == GUARD_AVOID ) {
							// The unit is avoiding, and doesn't
							// want to be in the battle if he can
							// avoid it
							continue;
						}


//									if (u == tar ||
//											(u->faction == tar->faction &&
//											 i==-1 &&
//											 CanAttack(r,&afacs,u))) {
//										add = ADD_DEFENSE;
//									}
							// The unit is not avoiding, and wants
							// to defend, if it can
						if( !u->CanMoveTo(r2,r) ) {
							// Unit can't reach battle
							continue;
						}
						if( ::GetUnit(&defs,u->num) ) {
							// Unit is already in the defender list!
							continue;
						}
						add = ADD_DEFENSE;
					} else if( u->type == U_GUARD || u->type == U_GUARDMAGE) {
						// The unit is a city guardsman
						if (i == -1 && adv == 0)
							add = ADD_DEFENSE;
					}

				}

				if (add == ADD_ATTACK) {
					Location * l = new Location;
					l->unit = u;
					l->obj = o;
					l->region = r2;
					atts.Add(l);
				} else if (add == ADD_DEFENSE) {
					Location * l = new Location;
					l->unit = u;
					l->obj = o;
					l->region = r2;
					defs.Add(l);
				}
			}
		}

		// If we are in the original region, check for the noaid status of
		// the units involved
		//
		if( i == -1 ) {
			noaida = 1;
			forlist( &atts ) {
				Location *l = (Location *) elem;
				if( !l->unit->GetFlag(FLAG_NOAID) ) {
					noaida = 0;
					break;
				}
			}

			noaidd = 1;
			{
				forlist (&defs) {
					Location *l = (Location *) elem;
					if (!l->unit->GetFlag(FLAG_NOAID)) {
						noaidd = 0;
						break;
					}
				}
			}
		}
	}
}

void Game::KillDead(Location * l) {
	if (!l->unit->IsAlive()) {
		l->region->Kill(l->unit);
	} else {
		if (l->unit->advancefrom) {
			l->unit->MoveUnit(l->unit->advancefrom->GetDummy());
		}
	}
}

int Game::RunBattle( ARegion * r, Unit * attacker, AList * targets, int ass,
					 int adv )
{
	AList afacs,dfacs;
	AList atts,defs;
	FactionPtr * fp;
	int result;

	AString atype = (ass ? "ASSASSINATE":"ATTACK" );

	int debug = 0;

	if( debug ) {
		Awrite(AString("Attacking unit = ") + *attacker->name);
		AString temp;
		if( targets ) {
			forlist( targets ) {
				UnitPtr * up = ( UnitPtr * ) elem;
				Unit * u = up->ptr;
				temp += *u->name + " ";
			}
			Awrite(AString(" Targets = ") + temp);
		} else {
			Awrite(" No Targets!!!");
		}
	}

	// No battles in a safe region
	if( r->IsSafeRegion() ) {
		AString temp = atype + ": No battles allowed in safe regions.";
		attacker->Error( temp );
		if( debug ) Awrite( temp );
		return BATTLE_IMPOSSIBLE;
	}

	// Make sure target is valid
	forlist( targets ) {
		UnitPtr * p = (UnitPtr *) elem;
		Unit * t = p->ptr;

		if( attacker->GetAttitude( r, t ) == A_ALLY) {
			// Cannot attack/assinate ally
			AString temp = atype + ": Can't attack an ally.";
			attacker->Error( temp );
			if( debug ) Awrite( temp );
			return BATTLE_IMPOSSIBLE;
		}

		if( ass ) {
			// Assassination attempt
			Unit * target = (Unit *)((UnitPtr *)targets->First())->ptr;

			fp = new FactionPtr;
			fp->ptr = attacker->faction;
			afacs.Add(fp);
			fp = new FactionPtr;
			fp->ptr = target->faction;
			dfacs.Add(fp);
			break;
		}
	}

	if( !ass ) {
		if( debug ) Awrite( "Getting defending factions" );
		GetDFacs( r, attacker, targets, defs, dfacs );
		if( GetFaction2(&dfacs,attacker->faction->num) ) {
			// This unit's faction is on the defender's side!
			// It must be allied to one of the targets
			attacker->Error("ATTACK: Can't attack an ally.");
			return BATTLE_IMPOSSIBLE;
		}
		if( debug ) {
			AString temp;
			forlist( &dfacs ) {
				FactionPtr * fp = ( FactionPtr * ) elem;
				Faction * f = fp->ptr;
				temp += *f->name + " ";
			}
			Awrite( AString("- Defending Facs = ") + temp );
			temp = "";
			{forlist( &defs ) {
				UnitPtr * up = ( UnitPtr * ) elem;
				Unit * u = up->ptr;
				temp += *u->name + " ";
			}}
			Awrite( AString("- Defending Units = ") + temp );
			temp = "";
			{forlist( &afacs ) {
				FactionPtr * fp = ( FactionPtr * ) elem;
				Faction * f = fp->ptr;
				temp += *f->name + " ";
			}}
			Awrite( AString("- Attacking Facs = ") + temp );
			temp = "";
			{forlist( &atts ) {
				UnitPtr * up = ( UnitPtr * ) elem;
				Unit * u = up->ptr;
				temp += *u->name + " ";
			}}
			Awrite( AString("- Attacking Units = ") + temp );

		}
		if( debug ) Awrite( "Getting attacking factions..." );
		GetAFacs( r, attacker, targets, atts, dfacs, afacs );
		if( debug ) {
			AString temp;
			forlist( &dfacs ) {
				FactionPtr * fp = ( FactionPtr * ) elem;
				Faction * f = fp->ptr;
				temp += *f->name + " ";
			}
			Awrite( AString("- Defending Facs = ") + temp );
			temp = "";
			{forlist( &defs ) {
				UnitPtr * up = ( UnitPtr * ) elem;
				Unit * u = up->ptr;
				temp += *u->name + " ";
			}}
			Awrite( AString("- Defending Units = ") + temp );
			temp = "";
			{forlist( &afacs ) {
				FactionPtr * fp = ( FactionPtr * ) elem;
				Faction * f = fp->ptr;
				temp += *f->name + " ";
			}}
			Awrite( AString("- Attacking Facs = ") + temp );
			temp = "";
			{forlist( &atts ) {
				UnitPtr * up = ( UnitPtr * ) elem;
				Unit * u = up->ptr;
				temp += *u->name + " ";
			}}
			Awrite( AString("- Attacking Units = ") + temp );

		}
	}
	if( debug ) Awrite( "Getting sides");
	GetSides( r, afacs, dfacs, atts, defs, attacker, targets, ass, adv );
		if( debug ) {
			AString temp;
			forlist( &dfacs ) {
				FactionPtr * fp = ( FactionPtr * ) elem;
				Faction * f = fp->ptr;
				temp += *f->name + " ";
			}
			Awrite( AString("- Defending Facs = ") + temp );
			temp = "";
			{forlist( &defs ) {
				UnitPtr * up = ( UnitPtr * ) elem;
				Unit * u = up->ptr;
				temp += *u->name + " ";
			}}
			temp = "";
			Awrite( AString("- Defending Units = ") + temp );
			{forlist( &afacs ) {
				FactionPtr * fp = ( FactionPtr * ) elem;
				Faction * f = fp->ptr;
				temp += *f->name + " ";
			}}
			Awrite( AString("- Attacking Facs = ") + temp );
			temp = "";
			{forlist( &atts ) {
				UnitPtr * up = ( UnitPtr * ) elem;
				Unit * u = up->ptr;
				temp += *u->name + " ";
			}}
			Awrite( AString("- Attacking Units = ") + temp );

		}

	if (atts.Num() <= 0) {
		// This shouldn't happen, but just in case
		Awrite(AString("Cannot find any attackers!"));
	}
	if (defs.Num() <= 0) {
		// This shouldn't happen, but just in case
		Awrite(AString("Cannot find any defenders!"));
		return BATTLE_IMPOSSIBLE;
	}

	Battle * b = new Battle;
	b->WriteSides(r,attacker,targets,&atts,&defs,ass, &regions);

	battles.Add(b);
	{
		forlist(&factions) {
			Faction * f = (Faction *) elem;
			if (GetFaction2(&afacs,f->num) || GetFaction2(&dfacs,f->num) ||
					r->Present(f)) {
				BattlePtr * p = new BattlePtr;
				p->ptr = b;
				f->battles.Add(p);
			}
		}
	}
	result = b->Run(r,attacker,&atts,targets,&defs,ass, &regions);

	/* Remove all dead units */
	{
		forlist(&atts) {
			KillDead((Location *) elem);
		}
	}
	{
		forlist(&defs) {
			KillDead((Location *) elem);
		}
	}
	return result;
}

int AttackTypeToWeaponClass( int attackType )
{
	if( attackType == ATTACK_ENERGY ) return MAGIC_ENERGY;
	return SLASHING;
}
