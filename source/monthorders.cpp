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
#include "gamedata.h"

void Game::RunSailOrders() {
	// ALT 28-Jul-2000
	// Fixes to prevent sailing of incomplete ships
	int tmpError = 0;

	forlist(&regions) {
		ARegion * r = (ARegion *) elem;
		AList regs;
		forlist(&r->objects) {
			Object * o = (Object *) elem;
			Unit * u = o->GetOwner();
			if (u && u->monthorders &&
					u->monthorders->type == O_SAIL &&
					o->IsBoat()) {
				if (o->incomplete < 1) {
					ARegionPtr * p = new ARegionPtr;
					p->ptr = Do1SailOrder(r,o,u);
					regs.Add(p);
				} else {
					tmpError = 1;
				}
			} else {
				tmpError = 2;
			}

			if (tmpError) {
				forlist(&o->units) {
					Unit * u2 = (Unit *) elem;
					if (u2->monthorders && u2->monthorders->type == O_SAIL) {
						delete u2->monthorders;
						u2->monthorders = 0;
						switch(tmpError) {
							case 1:
								u2->Error("SAIL: Ship is not finished.");
								break;
							case 2:
								u2->Error("SAIL: Owner must sail ship.");
								break;
						}
					}
				}
			}
		}
		{
			forlist(&regs) {
				ARegion * r2 = ((ARegionPtr *) elem)->ptr;
				DoAutoAttacksRegion(r2);
			}
		}
	}
}

ARegion * Game::Do1SailOrder(ARegion * reg,Object * ship,Unit * cap) {
	SailOrder * o = (SailOrder *) cap->monthorders;
	int movepoints = ObjectDefs[ship->type].sailSpeed;
	int moveok = 0;

	AList facs;
	int wgt = 0;
	int slr = 0;
	forlist(&ship->units) {
		Unit * unit = (Unit *) elem;
		if (unit->guard == GUARD_GUARD) unit->guard = GUARD_NONE;
		if (!GetFaction2(&facs,unit->faction->num)) {
			FactionPtr * p = new FactionPtr;
			p->ptr = unit->faction;
			facs.Add(p);
		}
		wgt += unit->Weight();
		if (unit->monthorders && unit->monthorders->type == O_SAIL) {
			slr += unit->GetSkill(S_SAILING) * unit->GetMen();
			unit->Practise(S_SAILING);
		}

		// XXX - sheesh... gotta do something about this.
		// I simply got rid of it, why having different swin skill levels for different ships
		// Now a different skill level in swin gives different move bonus
		int windlevel = unit->GetSkill(S_SUMMON_WIND);
		if (windlevel) {
			movepoints += windlevel;
			unit->Event("Casts Summon Wind to aid the ship's "
				"progress.");
			unit->Practise(S_SUMMON_WIND);
		}
		//	switch (ship->type) {
		//		case O_LONGBOAT:
		//			movepoints += 2;
		//			unit->Event("Casts Summon Wind to aid the ship's "
		//						"progress.");
		//			unit->Practise(S_SUMMON_WIND);
		//			break;
		//		case O_CLIPPER:
		//		case O_AIRSHIP:
		//			if (windlevel > 1) {
		//				movepoints += 2;
		//				unit->Event("Casts Summon Wind to aid the ship's "
		//							"progress.");
		//				unit->Practise(S_SUMMON_WIND);
		//			}
		//			break;
		//		default:
		//			if (windlevel > 2) {
		//				movepoints += 2;
		//				unit->Event("Casts Summon Wind to aid the ship's "
		//							"progress.");
		//				unit->Practise(S_SUMMON_WIND);
		//			}
		//			break;
		//	}
	}

	if (wgt > ObjectDefs[ship->type].capacity) {
		cap->Error("SAIL: Ship is overloaded.");
		moveok = 1;
	} else {
		if (slr < ObjectDefs[ship->type].sailors) {
			cap->Error("SAIL: Not enough sailors.");
			moveok = 1;
		} else {
			while (o->dirs.Num()) {
				MoveDir * x = (MoveDir *) o->dirs.First();
				o->dirs.Remove(x);
				int i = x->dir;
				delete x;
				ARegion * newreg = reg->neighbors[i];
				if (!newreg) {
					cap->Error("SAIL: Can't sail that way.");
					break;
				}
				int cost = 1;
				if (Globals->WEATHER_EXISTS) {
					if (newreg->weather != W_NORMAL) cost = 2;
				}

				if (ship->type != O_AIRSHIP && !newreg->IsCoastal()) {
					cap->Error("SAIL: Can't sail inland.");
					break;
				}

//				if ((ship->type != O_AIRSHIP) &&
//					(TerrainDefs[reg->type].similar_type != R_OCEAN) &&
//					(TerrainDefs[newreg->type].similar_type != R_OCEAN)) {
//					cap->Error("SAIL: Can't sail inland.");
//					break;
//				}
				if ((ship->type != O_AIRSHIP) &&
					!(TerrainDefs[reg->type].flags & TerrainType::CANSAIL) &&
					!(TerrainDefs[newreg->type].flags & TerrainType::CANSAIL))
				{
					cap->Error("SAIL: Can't sail inland.");
					break;
				}

				// Check to see if sailing THROUGH land!
				// always allow retracing steps
				if (Globals->PREVENT_SAIL_THROUGH &&
						(ship->type != O_AIRSHIP) &&
						(ship->prevdir != -1) &&
						(ship->prevdir != reg->GetRealDirComp(i))) {
					int blocked1 = 0;
					int blocked2 = 0;
					int d1 = reg->GetRealDirComp(ship->prevdir);
					int d2 = i;
					if (d1 > d2) {
						int tmp = d1;
						d1 = d2;
						d2 = tmp;
					}
					for (int k = d1+1; k < d2; k++) {
						ARegion *land1 = reg->neighbors[k];
						int l = k + 3;
						if (l >= NDIRS) l = l - NDIRS;
						ARegion *land2 = reg->neighbors[l];
						if (!land1 ||
							!(TerrainDefs[land1->type].flags & TerrainType::CANSAIL))
							blocked1 = 1;
						if (!land2 ||
							!(TerrainDefs[land2->type].flags & TerrainType::CANSAIL))
							blocked2 = 1;
					}
					if ((blocked1) && (blocked2))
					{
						cap->Error(AString("SAIL: Could not sail ") +
								DirectionStrs[i] + AString(" from ") +
								reg->ShortPrint(&regions) +
								". Cannot sail through land.");
						break;
					}
				}

				if (movepoints < cost) {
					cap->Error("SAIL: Can't sail that far;"
								" remaining moves queued.");
					TurnOrder *tOrder = new TurnOrder;
					tOrder->repeating = 0;
					AString order = "SAIL ";
					order += DirectionAbrs[i];
					forlist(&o->dirs) {
						MoveDir *move = (MoveDir *) elem;
						order += " ";
						order += DirectionAbrs[move->dir];
					}
					tOrder->turnOrders.Add(new AString(order));
					cap->turnorders.Insert(tOrder);
					break;
				}

				movepoints -= cost;
				ship->MoveObject(newreg);
				ship->SetPrevDir(i);
				forlist(&facs) {
					Faction * f = ((FactionPtr *) elem)->ptr;
					f->Event(*ship->name + AString(" sails from ") +
							reg->ShortPrint(&regions) + AString(" to ") +
							newreg->ShortPrint(&regions) + AString("."));
				}
				if (Globals->TRANSIT_REPORT != GameDefs::REPORT_NOTHING) {
					forlist(&ship->units) {
						// Everyone onboard gets to see the sights
						Unit *unit = (Unit *)elem;
						Farsight *f;
						// Note the hex being left
						forlist(&reg->passers) {
							f = (Farsight *)elem;
							if (f->unit == unit) {
								// We moved into here this turn
								f->exits_used[i] = 1;
							}
						}
						// And mark the hex being entered
						f = new Farsight;
						f->faction = unit->faction;
						f->level = 0;
						f->unit = unit;
						f->exits_used[newreg->GetRealDirComp(i)] = 1;
						newreg->passers.Add(f);
					}
				}
				reg = newreg;
				if (newreg->ForbiddenShip(ship)) {
					cap->faction->Event(*ship->name +
							AString(" is stopped by guards in ") +
							newreg->ShortPrint(&regions) + AString("."));
					break;
				}
			}
		}
	}

	/* Clear out everyone's orders */
	{
		forlist(&ship->units) {
			Unit * unit = (Unit *) elem;
			if (!moveok) {
				unit->alias = 0;
			}

			if (unit->monthorders) {
				if ((!moveok && unit->monthorders->type == O_MOVE) ||
					unit->monthorders->type == O_SAIL) {
					delete unit->monthorders;
					unit->monthorders = 0;
				}
			}
		}
	}

	return reg;
}

void Game::RunTeachOrders() {
	forlist((&regions)) {
		ARegion * r = (ARegion *) elem;
		forlist((&r->objects)) {
			Object * obj = (Object *) elem;
			forlist((&obj->units)) {
				Unit * u = (Unit *) elem;
				if (u->monthorders) {
					if (u->monthorders->type == O_TEACH) {
						Do1TeachOrder(r,u);
						delete u->monthorders;
						u->monthorders = 0;
					}
				}
			}
		}
	}
}

void Game::Do1TeachOrder(ARegion * reg,Unit * unit) {
	/* First pass, find how many to teach */
	if (Globals->LEADERS_EXIST && !unit->IsLeader()) {
		/* small change to handle Ceran's mercs */
	  //		if (!unit->GetMen(I_MERCENARY)) {
			// Mercs can teach even though they are not leaders.
			// They cannot however improve their own skills
			unit->Error("TEACH: Only leaders can teach.");
			return;
			//		}
	}

	int students = 0;
	TeachOrder * order = (TeachOrder *) unit->monthorders;
	forlist(&order->targets) {
		UnitId * id = (UnitId *) elem;
		Unit * target = reg->GetUnitId(id,unit->faction->num);
		if (!target) {
			order->targets.Remove(id);
			unit->Error("TEACH: No such unit.");
			delete id;
		} else {
			if (target->faction->GetAttitude(unit->faction->num) < A_FRIENDLY) {
				unit->Error(AString("TEACH: ") + *(target->name) +
							" is not a member of a friendly faction.");
				order->targets.Remove(id);
				delete id;
			} else {
				if (!target->monthorders ||
					target->monthorders->type != O_STUDY) {
					unit->Error(AString("TEACH: ") + *(target->name) +
								" is not studying.");
					order->targets.Remove(id);
					delete id;
				} else {
					int sk = ((StudyOrder *) target->monthorders)->skill;
					if (unit->GetRealSkill(sk) <= target->GetRealSkill(sk)) {
						unit->Error(AString("TEACH: ") +
									*(target->name) + " is not studying "
									"a skill you can teach.");
						order->targets.Remove(id);
						delete id;
					} else {
						students += target->GetMen();
					}
				}
			}
		}
	}
	if (!students) return;

	int days = (30 * unit->GetMen() * Globals->STUDENTS_PER_TEACHER);

	/* We now have a list of valid targets */
	{
		forlist(&order->targets) {
			UnitId * id = (UnitId *) elem;
			Unit * u = reg->GetUnitId(id,unit->faction->num);

			int umen = u->GetMen();
			int tempdays = (umen * days) / students;
			if (tempdays > 30 * umen) tempdays = 30 * umen;
			days -= tempdays;
			students -= umen;

			StudyOrder * o = (StudyOrder *) u->monthorders;
			o->days += tempdays;
			if (o->days > 30 * umen)
			{
				days += o->days - 30 * umen;
				o->days = 30 * umen;
			}
			unit->Event(AString("Teaches ") + SkillDefs[o->skill].name +
						" to " + *u->name + ".");
			// The TEACHER may learn something in this process!
			unit->Practise(o->skill);
		}
	}
}

void Game::Run1BuildOrder(ARegion * r,Object * obj,Unit * u) {
	if (!TradeCheck(r, u->faction)) {
		u->Error("BUILD: Faction can't produce in that many regions.");
		delete u->monthorders;
		u->monthorders = 0;
		return;
	}

	int sk = ObjectDefs[obj->type].skill;
	if (sk == -1) {
		u->Error("BUILD: Can't build that.");
		delete u->monthorders;
		u->monthorders = 0;
		return;
	}

	int usk = u->GetSkill(sk);
	if (usk < ObjectDefs[obj->type].level) {
		u->Error("BUILD: Can't build that.");
		delete u->monthorders;
		u->monthorders = 0;
		return;
	}

	int restrictedRegion = 0;
	int allowedRegion = 0;
	for (unsigned int index=0;index<sizeof(ObjectDefs[obj->type].allowedRegions)/sizeof(int);index++) {
	  if (ObjectDefs[obj->type].allowedRegions[index] != -1) {
	    restrictedRegion = 1;
	  }
	  if (ObjectDefs[obj->type].allowedRegions[index] == TerrainDefs[r->type].similar_type) {
	    allowedRegion = 1;
	    break;
	  }
	}
	if (restrictedRegion && (allowedRegion == 0)) {
	  u->Error("BUILD: Can't build that in this region.");
	  delete u->monthorders;
	  u->monthorders = 0;
	  return;
	}

	int needed = obj->incomplete;
	int type = obj->type;
	// AS
	if (((ObjectDefs[type].flags&ObjectType::NEVERDECAY) || !Globals->DECAY) &&
			needed < 1) {
		u->Error("BUILD: Object is finished.");
		delete u->monthorders;
		u->monthorders = 0;
		return;
	}

	// AS
	if (needed <= -(ObjectDefs[type].maxMaintenance)) {
		u->Error("BUILD: Object does not yet require maintenance.");
		delete u->monthorders;
		u->monthorders = 0;
		return;
	}

	int it = ObjectDefs[type].item;
	int itn;
	if (it == I_WOOD_OR_STONE) {
		itn = u->items.GetNum(I_WOOD) + u->items.GetNum(I_STONE);
	} else {
		itn = u->items.GetNum(it);
	}

	if (itn == 0) {
		u->Error("BUILD: Don't have the required item.");
		delete u->monthorders;
		u->monthorders = 0;
		return;
	}

	int num = u->GetMen() * usk + u->GetBuildBonus(type); // Sharky (added the "+ u->...")

	// JLT
	if (obj->incomplete == ObjectDefs[type].cost) {
		if (ObjectIsShip(type)) {
			obj->num = shipseq++;
			obj->SetName(new AString("Ship"));
		} else {
			obj->num = u->object->region->buildingseq++;
			obj->SetName(new AString("Building"));
		}
	}
	// Hack to fix bogus ship numbers
	if (ObjectIsShip(type) && obj->num < 1000) {
		obj->num = shipseq++;
		obj->SetName(new AString("Ship"));
	}

	// AS
	AString job;
	if (needed < 1) {
		// This looks wrong, but isn't.
		// If a building has a maxMaintainence of 75 and the road is at
		// -70 (ie, 5 from max) then we want the value of maintMax to be
		// 5 here.  Then we divide by maintFactor (some things are easier
		// to refix than others) to get how many items we need to fix it.
		// Then we fix it by that many items * maintFactor
		int maintMax = ObjectDefs[type].maxMaintenance + needed;
		maintMax /= ObjectDefs[type].maintFactor;
		if (num > maintMax) num = maintMax;
		if (itn < num) num = itn;
		job = " maintenance ";
		obj->incomplete -= (num * ObjectDefs[type].maintFactor);
		if (obj->incomplete < -(ObjectDefs[type].maxMaintenance))
			obj->incomplete = -(ObjectDefs[type].maxMaintenance);
	} else if (needed > 0) {
		if (num > needed) num = needed;
		if (itn < num) num = itn;
		job = " construction ";
		obj->incomplete -= num;
		if (obj->incomplete == 0) {
			obj->incomplete = -(ObjectDefs[type].maxMaintenance);
		}
	}

	/* Perform the build */

	if (it == I_WOOD_OR_STONE) {
		if (num > u->items.GetNum(I_STONE)) {
			num -= u->items.GetNum(I_STONE);
			u->items.SetNum(I_STONE,0);
			u->items.SetNum(I_WOOD,u->items.GetNum(I_WOOD) - num);
		} else {
			u->items.SetNum(I_STONE,u->items.GetNum(I_STONE) - num);
		}
	} else {
		u->items.SetNum(it,itn - num);
	}

	// AS
	u->Event(AString("Performs") + job + "on " + *(obj->name) + ".");
	u->Practise(sk);

	delete u->monthorders;
	u->monthorders = 0;
}

void Game::RunBuildHelpers(ARegion *r) {
	forlist((&r->objects)) {
		Object *obj = (Object *) elem;
		forlist ((&obj->units)) {
			Unit *u = (Unit *) elem;
			if (u->monthorders) {
				if (u->monthorders->type == O_BUILD) {
					BuildOrder *o = (BuildOrder *)u->monthorders;
					Object *tarobj = NULL;
					if (o->target) {
						Unit *target = r->GetUnitId(o->target,u->faction->num);
						if (!target) {
							u->Error("BUILD: No such unit to help.");
							delete u->monthorders;
							u->monthorders = 0;
							continue;
						}
						// Make sure that unit is building
						if (target->monthorders &&
								target->monthorders->type != O_BUILD) {
							u->Error("BUILD: Unit isn't building.");
							delete u->monthorders;
							u->monthorders = 0;
							continue;
						}
						// Make sure that unit considers you friendly!
						if (target->faction->GetAttitude(u->faction->num) <
								A_FRIENDLY) {
							u->Error("BUILD: Unit you are helping rejects "
									"your help.");
							delete u->monthorders;
							u->monthorders = 0;
							continue;
						}
						tarobj = target->build;
						if (tarobj == NULL) tarobj = target->object;
						if (u->object != tarobj)
							u->MoveUnit(tarobj);
					} else if (u->build != NULL && u->build != u->object) {
						u->MoveUnit(u->build);
					}
				}
			}
		}
	}
}


void Game::RunMonthOrders() {
	forlist(&regions) {
		ARegion * r = (ARegion *) elem;
		RunIdleOrders(r);
		RunSettleOrders(r);
		RunStudyOrders(r);
		RunBuildHelpers(r);
		RunProduceOrders(r);
	}
}

void Game::RunIdleOrders(ARegion *r)
{
	forlist((&r->objects)) {
		Object *obj = (Object *)elem;
		forlist((&obj->units)) {
			Unit *u = (Unit *)elem;
			if (u->monthorders && u->monthorders->type == O_IDLE) {
				u->Event("Sits idle.");
				delete u->monthorders;
				u->monthorders = 0;
			}
		}
	}
}

void Game::RunUnitProduce(ARegion *r, Object *obj, Unit *u) {
	ProduceOrder * o = (ProduceOrder *) u->monthorders;

	if (o->item == I_SILVER) {
		u->Error("Can't do that in this region.");
		delete u->monthorders;
		u->monthorders = 0;
		return;
	}

	if ((ItemDefs[u->faction->race].flags & ItemType::GOOD) && (ItemDefs[o->item].flags & ItemType::EVIL)) {
	  u->Error("BUY: Can't produce evil aligned items.");
	  delete u->monthorders;
	  u->monthorders = 0;
	  return;
	}
	if ((ItemDefs[u->faction->race].flags & ItemType::EVIL) && (ItemDefs[o->item].flags & ItemType::GOOD)) {
	  u->Error("BUY: Can't produce good aligned items.");
	  delete u->monthorders;
	  u->monthorders = 0;
	  return;
	}

	int input = ItemDefs[o->item].pInput[0].item;
	if (input == -1) {
		u->Error("PRODUCE: Can't produce that.");
		delete u->monthorders;
		u->monthorders = 0;
		return;
	}

	int level = u->GetSkill(o->skill);
	if (level < ItemDefs[o->item].pLevel) {
		u->Error("PRODUCE: Can't produce that.");
		delete u->monthorders;
		u->monthorders = 0;
		return;
	}

	if (ItemDefs[o->item].requiredstructure != -1) {
		if (ItemDefs[o->item].requiredstructure != obj->type) {
			u->Error("PRODUCE: Not inside correct building.");
			delete u->monthorders;
			u->monthorders = 0;
			return;
		}
	}

	// LLS
	int number = u->GetMen() * level + u->GetProductionBonus(o->item);

	if (!TradeCheck(r, u->faction)) {
		u->Error("PRODUCE: Faction can't produce in that many regions.");
		delete u->monthorders;
		u->monthorders = 0;
		return;
	}

	// find the max we can possibly produce based on man-months of labor
	int maxproduced;
	if (ItemDefs[o->item].flags & ItemType::SKILLOUT)
		maxproduced = u->GetMen();
	else
		maxproduced = number/ItemDefs[o->item].pMonths;

	if (ItemDefs[o->item].flags & ItemType::ORINPUTS) {
		// Figure out the max we can produce based on the inputs
		int count = 0;
		unsigned int c;
		for(c = 0; c < sizeof(ItemDefs->pInput)/sizeof(Materials); c++) {
			int i = ItemDefs[o->item].pInput[c].item;
			if (i != -1)
				count += u->items.GetNum(i) / ItemDefs[o->item].pInput[c].amt;
		}
		if (ItemDefs[o->item].requiredstructure == obj->type) {
		  maxproduced *= (int)obj->productionratio;
		}
		if (maxproduced > count)
			maxproduced = count;
		count = maxproduced;
		// Deduct the items spent
		for(c = 0; c < sizeof(ItemDefs->pInput)/sizeof(Materials); c++) {
			int i = ItemDefs[o->item].pInput[c].item;
			int a = ItemDefs[o->item].pInput[c].amt;
			if (i != -1) {
				int amt = u->items.GetNum(i);
				if (count > amt / a) {
					count -= amt / a;
					u->items.SetNum(i, amt-(amt/a)*a);
				} else {
					u->items.SetNum(i, amt - count * a);
					count = 0;
				}
			}
		}
	}
	else {
		// Figure out the max we can produce based on the inputs
		unsigned int c;
		for(c = 0; c < sizeof(ItemDefs->pInput)/sizeof(Materials); c++) {
			int i = ItemDefs[o->item].pInput[c].item;
			if (i != -1) {
				int amt = u->items.GetNum(i);
				if (amt/ItemDefs[o->item].pInput[c].amt < maxproduced) {
					maxproduced = amt/ItemDefs[o->item].pInput[c].amt;
				}
			}
		}

		// Deduct the items spent
		for(c = 0; c < sizeof(ItemDefs->pInput)/sizeof(Materials); c++) {
			int i = ItemDefs[o->item].pInput[c].item;
			int a = ItemDefs[o->item].pInput[c].amt;
			if (i != -1) {
				int amt = u->items.GetNum(i);
				u->items.SetNum(i, amt-(maxproduced*a));
			}
		}
	}

	// Now give the items produced
	int output = maxproduced * ItemDefs[o->item].pOut;
	if (ItemDefs[o->item].flags & ItemType::SKILLOUT)
		output *= level;
	if (ItemDefs[o->item].type != IT_ABSTRACT) {
		u->items.SetNum(o->item,u->items.GetNum(o->item) + output);
	}
	for (unsigned int by=0;by<sizeof(ItemDefs[o->item].byproducts)
			/sizeof(ItemDefs[o->item].byproducts[0]);by++) {
		int byproduct = ItemDefs[o->item].byproducts[by];
		if (byproduct != -1)
			u->items.SetNum(byproduct,u->items.GetNum(byproduct) + output);
	}
	u->Event(AString("Produces ") + ItemString(o->item,output) + " in " +
			r->ShortPrint(&regions) + ".");
	u->Practise(o->skill);
	delete u->monthorders;
	u->monthorders = 0;
}

void Game::RunProduceOrders(ARegion * r) {
	{
		forlist ((&r->products))
			RunAProduction(r,(Production *) elem);
	}
	{
		forlist((&r->objects)) {
			Object * obj = (Object *) elem;
			int workers = 0;
			forlist ((&obj->units)) {
				Unit * u = (Unit *) elem;
				if (u->monthorders) {
					if (u->monthorders->type == O_PRODUCE) {
						ProduceOrder *po = (ProduceOrder *)u->monthorders;
						if (ItemDefs[po->item].requiredstructure == obj->type) {
							workers += u->GetMen();
						}
					}
				}
			}
			obj->productionratio = 1.0;
			if (workers>0) {
				if (workers > ObjectDefs[obj->type].workersallowed) {
					obj->productionratio = (float)ObjectDefs[obj->type].workersallowed/(float)workers;
					obj->Notify(*(obj->name) + AString(" ") + ObjectDefs[obj->type].name + AString(" is over worked, workers are inefficent."));
				}// else
//					obj->productionratio = 1.0;
			}
			{
				forlist ((&obj->units)) {
					Unit * u = (Unit *) elem;
					if (u->monthorders) {
						if (u->monthorders->type == O_PRODUCE) {
							RunUnitProduce(r,obj,u);
						} else {
							if (u->monthorders->type == O_BUILD)
								Run1BuildOrder(r,obj,u);
						}
					}
				}
			}
		}
	}
}

int Game::ValidProd(Unit * u, ARegion * r, Object *obj, Production * p) {
	if (u->monthorders->type != O_PRODUCE) return 0;

	ProduceOrder * po = (ProduceOrder *) u->monthorders;
	if (p->itemtype == po->item && p->skill == po->skill) {
		if (p->skill == -1) {
			po->productivity = u->GetMen() * p->productivity;
			return po->productivity;
		}
		if ((ItemDefs[u->faction->race].flags & ItemType::GOOD) && (ItemDefs[p->itemtype].flags & ItemType::EVIL)) {
			u->Error("BUY: Can't produce evil aligned items.");
			delete u->monthorders;
			u->monthorders = 0;
			return 0;
		}
		if ((ItemDefs[u->faction->race].flags & ItemType::EVIL) && (ItemDefs[p->itemtype].flags & ItemType::GOOD)) {
			u->Error("BUY: Can't produce good aligned items.");
			delete u->monthorders;
			u->monthorders = 0;
			return 0;
		}
		int level = u->GetSkill(p->skill);
		if (level < ItemDefs[p->itemtype].pLevel) {
			u->Error("PRODUCE: Unit isn't skilled enough.");
			delete u->monthorders;
			u->monthorders = 0;
			return 0;
		}

		//
		// Check faction limits on production. If the item is silver, then the
		// unit is entertaining or working, and the limit does not apply
		//
		if (p->itemtype != I_SILVER && !TradeCheck(r, u->faction)) {
			u->Error("PRODUCE: Faction can't produce in that many regions.");
			delete u->monthorders;
			u->monthorders = 0;
			return 0;
		}

		if (ItemDefs[p->itemtype].requiredstructure != -1) {
			if (ItemDefs[p->itemtype].requiredstructure != obj->type) {
				u->Error("PRODUCE: Not inside correct building.");
				delete u->monthorders;
				u->monthorders = 0;
				return 0;
			}
		}

		/* check for bonus production */
		// LLS
		int bonus = u->GetProductionBonus(p->itemtype);
		po->productivity = u->GetMen() * level * p->productivity + bonus;
		return po->productivity;
	}
	return 0;
}

int Game::FindAttemptedProd(ARegion * r,Production * p) {
	int attempted = 0;
	forlist((&r->objects)) {
		int amt = 0;
		int men = 0;
		Object * obj = (Object *) elem;
		forlist((&obj->units)) {
			Unit * u = (Unit *) elem;
			if (u->monthorders) {
				amt += ValidProd(u,r,obj,p);
				men += u->GetMen();
      			}
   		}
		int workers = ObjectDefs[obj->type].workersallowed;
		if (workers != -1 || men <= workers || ItemDefs[p->itemtype].requiredstructure != obj->type) {
			attempted += amt;
			obj->productionratio = 1.0;
		} else if (men == 0) {
			obj->productionratio = 1.0;
		} else {
			attempted += amt * (workers/men);
			obj->productionratio = (float)workers/(float)men;
			obj->Notify(*(obj->name) + AString(" ") + ObjectDefs[obj->type].name + AString(" is over worked, workers are inefficent."));
		}
	}
	return attempted;
}

void Game::RunAProduction(ARegion * r,Production * p) {
	p->activity = 0;
	if (p->amount == 0) return;

	/* First, see how many units are trying to work */
	int attempted = FindAttemptedProd(r,p);
	int amt = p->amount;
	if (attempted < amt) attempted = amt;
	forlist((&r->objects)) {
		Object * obj = (Object *) elem;
		forlist((&obj->units)) {
			Unit * u = (Unit *) elem;
			if (!u->monthorders || u->monthorders->type != O_PRODUCE) continue;

			ProduceOrder * po = (ProduceOrder *) u->monthorders;
			if (po->skill != p->skill || po->item != p->itemtype) continue;

			/* We need to implement a hack to avoid overflowing */
			int uatt, ubucks;

			uatt = po->productivity;
			if (uatt && amt && attempted) {
				double dUbucks = ((double)obj->productionratio)*((double) amt) * ((double) uatt) / ((double) attempted);
				ubucks = (int) dUbucks;
			} else {
				ubucks = 0;
			}

			amt -= ubucks;
			attempted -= uatt;
			if (ItemDefs[po->item].type != IT_ABSTRACT) {
				u->items.SetNum(po->item,u->items.GetNum(po->item)
							+ ubucks);
			}
			for (unsigned int by=0;by<sizeof(ItemDefs[po->item].byproducts)/
		 			sizeof(ItemDefs[po->item].byproducts[0]);by++) {
				int byproduct = ItemDefs[po->item].byproducts[by];
				if (byproduct != -1) {
					u->items.SetNum(byproduct,u->items.GetNum(byproduct) + ubucks);
				}
			}

			p->activity += ubucks;

			/* Show in unit's events section */
			if (po->item == I_SILVER) {
				//
				// WORK
				//
				if (po->skill == -1) {
					u->Event(AString("Earns ") + ubucks + " silver working in "
							 + r->ShortPrint(&regions) + ".");
				} else {
					//
					// ENTERTAIN
					//
					u->Event(AString("Earns ") + ubucks
							 + " silver entertaining in " +
							 r->ShortPrint(&regions)
							 + ".");
					// If they don't have PHEN, then this will fail safely
					u->Practise(S_PHANTASMAL_ENTERTAINMENT);
					u->Practise(S_ENTERTAINMENT);
				}
			} else {
				/* Everything else */
				u->Event(AString("Produces ") + ItemString(po->item,ubucks) +
						 " in " + r->ShortPrint(&regions) + ".");
				u->Practise(po->skill);
			}
			delete u->monthorders;
			u->monthorders = 0;
		}
	}
}

void Game::RunStudyOrders(ARegion * r) {
	forlist((&r->objects)) {
		Object * obj = (Object *) elem;
		forlist((&obj->units)) {
			Unit * u = (Unit *) elem;
			if (u->monthorders) {
				if (u->monthorders->type == O_STUDY) {
					Do1StudyOrder(u,obj);
					delete u->monthorders;
					u->monthorders = 0;
				}
			}
		}
	}
}

void Game::Do1StudyOrder(Unit *u,Object *obj) {
	StudyOrder * o = (StudyOrder *) u->monthorders;
	int sk = o->skill;
	int cost = SkillCost(sk) * u->GetMen();
	int reset_man = -1;
	if (cost > u->GetMoney()) {
		u->Error("STUDY: Not enough funds.");
		return;
	}

	if( SkillDefs[sk].building != -1 && 
		SkillDefs[sk].building != u->object->type )
	{
		u->Error(AString("STUDY: Unit must be inside a ") +
				 ObjectDefs[SkillDefs[sk].building].name + " to study this skill.");
		return;
	}

	//Small patch for Ceran Mercs
	//	if (u->GetMen(I_MERCENARY)) {
	//		u->Error("STUDY: Mercenaries are not allowed to study.");
	//		return;
	//	}

	if ((SkillDefs[sk].flags & SkillType::MAGIC) && u->type != U_MAGE) {
		if (u->type == U_APPRENTICE) {
			u->Error("STUDY: An apprentice cannot be made into an mage.");
			return;
		}
		if (Globals->FACTION_LIMIT_TYPE != GameDefs::FACLIM_UNLIMITED) {
			if (CountMages(u->faction) + u->GetMen() > AllowedMages(u->faction)) {
				u->Error("STUDY: Mage limit exceeded.");
				return;
			}
		}
		if (u->GetMen() != 1 && !Globals->MULTIPLE_MAGES_PER_UNIT) {
			u->Error("STUDY: Only 1-man units can be magicians.");
			return;
		}
		if (!(Globals->MAGE_NONLEADERS)) {
			if (!u->IsLeader()) {
				u->Error("STUDY: Only leaders may study magic.");
				return;
			}
		}
		reset_man = u->type;
		u->type = U_MAGE;
	}

	if ((SkillDefs[sk].flags&SkillType::APPRENTICE) &&
			u->type != U_APPRENTICE) {
		if (u->type == U_MAGE) {
			u->Error("STUDY: A mage cannot be made into an apprentice.");
			return;
		}

		if (Globals->FACTION_LIMIT_TYPE != GameDefs::FACLIM_UNLIMITED) {
			if (CountApprentices(u->faction)>=AllowedApprentices(u->faction)) {
				u->Error("STUDY: Can't have another apprentice.");
				return;
			}
		}
		if (u->GetMen() != 1) {
			u->Error("STUDY: Only 1-man units can be apprentices.");
			return;
		}
		if (!(Globals->MAGE_NONLEADERS)) {
			if (!u->IsLeader()) {
				u->Error("STUDY: Only leaders may be apprentices.");
				return;
			}
		}
		reset_man = u->type;
		u->type = U_APPRENTICE;
	}

	int days = 30 * u->GetMen() + o->days;

	if ((SkillDefs[sk].flags & SkillType::MAGIC) && u->GetSkill(sk) >= 2) {
		if (Globals->LIMITED_MAGES_PER_BUILDING) {
			if (obj->incomplete > 0 || obj->type == O_DUMMY) {
				u->Error("Warning: Magic study rate outside of a building "
						"cut in half above level 2.");
				days /= 2;
			} else if (obj->mages == 0) {
				u->Error("Warning: Magic rate cut in half above level 2 due "
						"to number of mages studying in structure.");
				days /= 2;
			} else {
				obj->mages--;
			}
		} else if (!(ObjectDefs[obj->type].protect) || (obj->incomplete > 0)) {
			u->Error("Warning: Magic study rate outside of a building cut in "
					"half above level 2.");
			days /= 2;
		}
	}

	if (SkillDefs[sk].flags & SkillType::SLOWSTUDY) {
		days /= 2;
	}

	if (u->Study(sk,days)) {
		u->SetMoney(u->GetMoney() - cost);
		u->Event(AString("Studies ") + SkillDefs[sk].name + ".");
	} else {
		// if we just tried to become a mage or apprentice, but
		// were unable to study, reset unit to whatever it was before.
		if (reset_man != -1)
			u->type = reset_man;
	}
}

void Game::RunMoveOrders() {
	for (int phase = 0; phase < Globals->MAX_SPEED; phase++) {
		{
			forlist((&regions)) {
				ARegion * region = (ARegion *) elem;
				forlist((&region->objects)) {
					Object * obj = (Object *) elem;
					forlist(&obj->units) {
						Unit * unit = (Unit *) elem;
						Object *tempobj = obj;
						DoMoveEnter(unit,region,&tempobj);
					}
				}
			}
		}

		AList * locs = new AList;
		forlist((&regions)) {
			ARegion * region = (ARegion *) elem;
			forlist((&region->objects)) {
				Object * obj = (Object *) elem;
				forlist(&obj->units) {
					Unit * unit = (Unit *) elem;
					if (phase == unit->movepoints && unit->monthorders &&
						(unit->monthorders->type == O_MOVE ||
						 unit->monthorders->type == O_ADVANCE) &&
						!unit->nomove) {
						locs->Add(DoAMoveOrder(unit,region,obj));
					}
				}
			}
/*
	  DoAdvanceAttacks(locs);
	  locs->DeleteAll();
*/
		}
		DoAdvanceAttacks(locs);
		locs->DeleteAll();
	}
}

void Game::DoMoveEnter(Unit * unit,ARegion * region,Object **obj) {
	MoveOrder * o;
	if (!unit->monthorders ||
			((unit->monthorders->type != O_MOVE) &&
			 (unit->monthorders->type != O_ADVANCE)))
		return;
	o = (MoveOrder *) unit->monthorders;

	while (o->dirs.Num()) {
		MoveDir * x = (MoveDir *) o->dirs.First();
		int i = x->dir;
		if (i != MOVE_OUT && i < MOVE_ENTER) return;
		o->dirs.Remove(x);
		delete x;

		if (i >= MOVE_ENTER) {
			Object * to = region->GetObject(i - MOVE_ENTER);
			if (!to) {
				unit->Error("MOVE: Can't find object.");
				continue;
			}

			if (!to->CanEnter(region,unit)) {
				unit->Error("ENTER: Can't enter that.");
				continue;
			}

			Unit *forbid = to->ForbiddenBy(region, unit);
			if (forbid && !o->advancing) {
				unit->Error("ENTER: Is refused entry.");
				continue;
			}

			if (forbid && region->IsSafeRegion())
			{
				unit->Error("ENTER: No battles allowed in safe regions.");
				continue;
			}

			if (forbid && !(unit->IsAlive() && unit->canattack)) {
				unit->Error(AString("ENTER: Unable to attack ") +
						*(forbid->name));
				continue;
			}

			int done = 0;

			AList targets;
			forlist( &to->units ) {
				Unit * u2 = (Unit * ) elem;
				if( u2->Forbids( to->region, unit ) ) {
					UnitPtr * p = new UnitPtr;
					p->ptr = u2;
					targets.Add( p );
				}
			}
			if( targets.Num() ) {
				int result = RunBattle(region, unit, &targets, 0, 0);
				if (result == BATTLE_IMPOSSIBLE) {
					unit->Error(AString("ENTER: Unable to attack guards."));
					done = 1;
					break;
				}
				if (!unit->IsAlive() || !unit->canattack) {
				  done = 1;
				  break;
				}
			}
			if (done) continue;

			unit->MoveUnit(to);
			unit->Event(AString("Enters ") + *(to->name) + ".");
			*obj = to;
		} else {
			if (i == MOVE_OUT) {
				if (TerrainDefs[region->type].similar_type == R_OCEAN &&
						(!unit->CanSwim() ||
						 unit->GetFlag(FLAG_NOCROSS_WATER)))
				{
					unit->Error("MOVE: Can't leave ship.");
					continue;
				}

				Object * to = region->GetDummy();
				unit->MoveUnit(to);
				*obj = to;
			}
		}
	}
}

Location * Game::DoAMoveOrder(Unit * unit, ARegion * region, Object * obj) {
	Location * loc = new Location;
	MoveOrder * o = (MoveOrder *) unit->monthorders;
	int movetype = unit->MoveType();

	if (unit->guard == GUARD_GUARD) unit->guard = GUARD_NONE;
	if (o->advancing) unit->guard = GUARD_ADVANCE;

	/* Ok, now we can move a region */
	if (o->dirs.Num()) {
		MoveDir * x = (MoveDir *) o->dirs.First();
		o->dirs.Remove(x);
		int i = x->dir;
		int startmove = 0;
		delete x;

		/* Setup region to move to */
		ARegion * newreg;
		if (i == MOVE_IN) {
			if (obj->inner == -1) {
				unit->Error("MOVE: Can't move IN there.");
				goto done_moving;
			}
			newreg = regions.GetRegion(obj->inner);
		} else {
			newreg = region->neighbors[i];
		}

		if (!newreg) {
			unit->Error(AString("MOVE: Can't move that direction."));
			goto done_moving;
		}

		if (region->type == R_NEXUS && newreg->IsStartingCity())
			startmove = 1;

		if ((TerrainDefs[region->type].similar_type == R_OCEAN) &&
		   (!unit->CanSwim() || unit->GetFlag(FLAG_NOCROSS_WATER))) {
			unit->Error(AString("MOVE: Can't move while in the ocean."));
			goto done_moving;
		}

		int cost = newreg->MoveCost(movetype, region, i);

		if (region->type != R_NEXUS &&
				unit->CalcMovePoints() - unit->movepoints < cost) {
			if (unit->MoveType() == M_NONE) {
				unit->Error("MOVE: Unit is overloaded and cannot move.");
			} else {
				unit->Error("MOVE: Unit has insufficient movement points;"
						" remaining moves queued.");
				TurnOrder *tOrder = new TurnOrder;
				AString order;
				tOrder->repeating = 0;
				if (o->advancing) order = "ADVANCE ";
				else order = "MOVE ";
				if (i < NDIRS) order += DirectionAbrs[i];
				else if (i == MOVE_IN) order += "IN";
				else if (i == MOVE_OUT) order += "OUT";
				else order += i - MOVE_ENTER;
				forlist(&o->dirs) {
					MoveDir *move = (MoveDir *) elem;
					order += " ";
					if (move->dir < NDIRS) order += DirectionAbrs[move->dir];
					else if (move->dir == MOVE_IN) order += "IN";
					else if (move->dir == MOVE_OUT) order += "OUT";
					else order += move->dir - MOVE_ENTER;
				}
				tOrder->turnOrders.Add(new AString(order));
				unit->turnorders.Insert(tOrder);
			}
			goto done_moving;
		}

		if ((TerrainDefs[newreg->type].similar_type == R_OCEAN) &&
		   (!unit->CanSwim() || unit->GetFlag(FLAG_NOCROSS_WATER))) {
			unit->Event(AString("Discovers that ") +
						newreg->ShortPrint(&regions) + " is " +
						TerrainDefs[newreg->type].name + ".");
			goto done_moving;
		}

		if (unit->type == U_WMON && newreg->town && newreg->IsGuarded()) {
			unit->Event("Monsters don't move into guarded towns.");
			goto done_moving;
		}

		if (unit->guard == GUARD_ADVANCE) {
			Unit *ally = newreg->ForbiddenByAlly(unit);
			if (ally && !startmove) {
				unit->Event(AString("Can't ADVANCE: ") + *(newreg->name) +
							" is guarded by " + *(ally->name) + ", an ally.");
				goto done_moving;
			}
		}

		Unit * forbid = newreg->Forbidden(unit);
		if (forbid && !startmove && unit->guard != GUARD_ADVANCE) {
			int obs = unit->GetObservation();
			unit->Event(AString("Is forbidden entry to ") +
						newreg->ShortPrint(&regions) + " by " +
						forbid->GetName(obs) + ".");
			obs = forbid->GetObservation();
			forbid->Event(AString("Forbids entry to ") +
						  unit->GetName(obs) + ".");
			goto done_moving;
		}

		/* Clear the unit's alias out, so as not to interfere with TEACH */
		unit->alias = 0;

		unit->movepoints += cost;
		unit->MoveUnit(newreg->GetDummy());

		AString temp;
		switch (movetype) {
		case M_WALK:
			temp = "Walks ";
			if (TerrainDefs[newreg->type].similar_type == R_OCEAN)
				temp = "Swims ";
			break;
		case M_RIDE:
			temp = "Rides ";
			break;
		case M_FLY:
			temp = "Flies ";
			break;
		}
		unit->Event(temp + AString("from ") + region->ShortPrint(&regions)
					+ AString(" to ") + newreg->ShortPrint(&regions) +
					AString("."));
		if (forbid) {
			unit->advancefrom = region;
		}
		if (Globals->TRANSIT_REPORT != GameDefs::REPORT_NOTHING) {
			// Update our visit record in the region we are leaving.
			Farsight *f;
			forlist(&region->passers) {
				f = (Farsight *)elem;
				if (f->unit == unit) {
					// We moved into here this turn
					if (i < MOVE_IN) {
						f->exits_used[i] = 1;
					}
				}
			}
			// And mark the hex being entered
			f = new Farsight;
			f->faction = unit->faction;
			f->level = 0;
			f->unit = unit;
			if (i < MOVE_IN) {
				f->exits_used[newreg->GetRealDirComp(i)] = 1;
			}
			newreg->passers.Add(f);
		}
		region = newreg;
	}

	loc->unit = unit;
	loc->region = region;
	loc->obj = obj;
	return loc;

done_moving:
	delete o;
	unit->monthorders = 0;
	loc->unit = unit;
	loc->region = region;
	loc->obj = obj;
	return loc;
}

void Game::RunTunnelOrders()
{
	forlist(&regions) {
		ARegion * pRegion = (ARegion *) elem;
		forlist(&pRegion->objects) {
			Object * obj = (Object *) elem;
			forlist(&obj->units) {
				Unit * u = (Unit *) elem;
				if(u->monthorders) {
					if (u->monthorders->type == O_TUNNEL) {
						Do1TunnelOrder(pRegion,u);
						delete u->monthorders;
						u->monthorders = 0;
					}
				}
			}
		}
	}
}

void Game::Do1TunnelOrder(ARegion * pRegion, Unit * pUnit)
{
	// Get target region
	ARegion * target = pRegion->neighbors[( (TunnelOrder *) pUnit->monthorders )->dir];
	if( !target ) {
		pUnit->Error("TUNNEL: No region in that direction.");
		return;
	}
	if( !( TerrainDefs[target->type].flags & TerrainType::CANTUNNEL ) ) {
		pUnit->Error("TUNNEL: Cannot tunnel into this type of terrain.");
		return;
	}

	// Unit must be able to move into new region
	int newTerrain = TerrainDefs[target->type].similar_type;
	int cost = TerrainDefs[newTerrain].movepoints;
	
	if(	pUnit->CalcMovePoints() - pUnit->movepoints < cost) {
		if (pUnit->MoveType() == M_NONE) {
			pUnit->Error("TUNNEL: Unit is overloaded and cannot move.");
		} else {
			pUnit->Error("TUNNEL: Unit has insufficient movement points.");
		}
		return;
	}

	// Unit must be skilled in quarrying and mining
	// Only units with a skill level of at least (difficulty) will count
	// Unit requires total of (difficulty * 1000) levels of mining and quarrying
	// Difficulty of > 1 requires 1 pick or enchanted pick for every man
	// Difficulty of > 2 requires 1 enchanted pick per man
	int difficulty = 1;
	if( target->type == R_CE_GRANITE ) difficulty = 2;
	else if( target->type == R_CE_BASALT ) difficulty = 3;

	if( pUnit->GetSkill( S_MINING ) < difficulty ) {
		pUnit->Error(AString("TUNNEL: Unit must know at least ") + SkillDefs[S_MINING].name + " " +
			difficulty + " to tunnel through " + TerrainDefs[target->type].name + ".");
		return;
	}
	if( pUnit->GetSkill( S_QUARRYING ) < difficulty ) {
		pUnit->Error(AString("TUNNEL: Unit must know at least ") + SkillDefs[S_QUARRYING].name + " " +
			difficulty + " to tunnel through " + TerrainDefs[target->type].name + ".");
		return;
	}

	int men = pUnit->GetMen();
	if( difficulty > 0 ) {
		int picks = pUnit->items.GetNum(I_ENCHANTEDPICK);
		if( difficulty < 3 ) picks += pUnit->items.GetNum(I_PICK);
		if( picks < men ) men = picks;
	}
	
	if( men * pUnit->GetSkill(S_MINING) < (difficulty * 1000) ) {
		pUnit->Error( AString("TUNNEL: Unit does not have enough tools and/or skilled men") );
//		              + " (" + 
//					  pUnit->GetMen() + ", " + pUnit->items.GetNum(I_PICK) + 
//					  ", " + pUnit->items.GetNum(I_ENCHANTEDPICK) + ", " +
//					  pUnit->GetSkill(S_MINING) + ", " + 
//					  (men * pUnit->GetSkill(S_MINING)) +
//					  "/" +
//					  (difficulty*1000) + ")." );
		return;
	}
	if( men * pUnit->GetSkill(S_QUARRYING) < (difficulty * 1000) ) {
		pUnit->Error( AString("TUNNEL: Unit does not have enough tools and/or skilled men") );
		return;
	}


	// OK, now let's tunnel!

	// First get the byproducts of the venture
	// This will be random number, determined by products in terraindef
	for( int i = 0; i < 21; i++ ) {
		int item = TerrainDefs[target->type].prods[i].product;
		if( item != -1 && ITEM_ENABLED(item) ) {
			if( getrandom(100) < TerrainDefs[target->type].prods[i].chance ) {
				int amt = getrandom( TerrainDefs[target->type].prods[i].amount ) + 1;
				pUnit->items.SetNum( item, pUnit->items.GetNum( item ) + amt );
				pUnit->Event(AString("Produces ") + ItemString(item, amt) + 
					" from tunnelling.");
			}
		}
	}

	pUnit->Event(AString( "Tunnels into ") + target->ShortPrint(&regions) + "." );

	// Change terrain type
	target->type = newTerrain;

	// Change products and economy
	target->products.DeleteAll();
	target->markets.DeleteAll();
	target->SetupProds();
	target->SetupPop();

	// Move tunnelers into region
	pUnit->MoveUnit( target->GetDummy() );

}


void Game::RunSettleOrders(ARegion * pRegion)
{
	forlist(&pRegion->objects) {
		Object * obj = (Object *) elem;
		forlist(&obj->units) {
			Unit * u = (Unit *) elem;
			if(u->monthorders) {
				if (u->monthorders->type == O_SETTLE) {
					Do1SettleOrder(pRegion,u);
					delete u->monthorders;
					u->monthorders = 0;
				}
			}
		}
	}
}

void Game::Do1SettleOrder(ARegion * pRegion, Unit * pUnit)
{
	// Can't settle region if it is guarded by another faction
	if( !pRegion->CanPillage( pUnit ) ) {
		pUnit->Error("SETTLE: A unit is on guard.");
		return;
	}

	// Men must be >= 10% of population
	int menNeeded = pRegion->Population() / 10;
	if( pUnit->GetMen() < menNeeded ) {
		pUnit->Error("SETTLE: Not enough men to settle this region");
		return;
	}

	// Can only change population to one that the same alignment
	// as that of the settling unit's faction
	int rand = getrandom(100);
	int newRace;
	if (rand<50) 
		newRace = ManDefs[ItemDefs[pRegion->race].index].alternaterace[0];
	else if (rand<80)
		newRace = ManDefs[ItemDefs[pRegion->race].index].alternaterace[1];
	else
		newRace = ManDefs[ItemDefs[pRegion->race].index].alternaterace[2];

	if( ( ItemDefs[newRace].flags & ItemType::EVIL &&
		  !(ItemDefs[pUnit->faction->race].flags & ItemType::EVIL) ) ||
		( ItemDefs[newRace].flags & ItemType::GOOD &&
 		  !(ItemDefs[pUnit->faction->race].flags & ItemType::GOOD) ) ) {
		pUnit->Error("SETTLE: Can't change race to one that is of a different alignment to your race.");
		return;
	}

	// Remove men markets
	forlist( &pRegion->markets ) {
		Market * m = ( Market * ) elem;
		if( m->type == M_BUY && ItemDefs[m->item].type & IT_MAN ) {
			pRegion->markets.Remove( m );
			delete m;		
		}
	}

	// Set up new men markets
	Market * m;
	float ratio = ItemDefs[newRace].baseprice / (float)Globals->BASE_MAN_COST;
	m = new Market(M_BUY, newRace, (int)(pRegion->Wages()*4*ratio),
					pRegion->Population()/5, 0, 10000, 0, 2000);
	pRegion->markets.Add( m );

	// Add minority races
	int minority = ManDefs[ItemDefs[newRace].index].minority;
	int divisor = 25;
	while (minority != -1) {
		ratio = ItemDefs[minority].baseprice / (float)Globals->BASE_MAN_COST;
		int current = pRegion->Population() / divisor;
		int max = 10000/divisor;
		if (ManDefs[ItemDefs[minority].index].flags & ManType::POPULUS) {
			current *= 2;
			max *= 2;
		} else if (ManDefs[ItemDefs[minority].index].flags & ManType::SCARCE) {
			current /= 2;
			max /= 2;
		}
		m = new Market(M_BUY, minority, (int)(pRegion->Wages()*4*ratio),
			   current, 0, 10000, 0, max);
		pRegion->markets.Add(m);
		minority = ManDefs[ItemDefs[minority].index].minority;
		divisor *= 5;
		if (divisor > 10000) break; // sanity check
	}
	
	// Change race
	pRegion->race = newRace;

	pUnit->Event(AString("Changes population in ") + pRegion->ShortPrint(&regions) +
				 " to " + ItemDefs[newRace].names);

}
