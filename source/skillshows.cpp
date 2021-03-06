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
#include "skills.h"
#include "items.h"
#include "object.h"
#include <gamedata.h>
#include "astring.h"

#define SKILL_ENABLED(X) (!(SkillDefs[(X)].flags & SkillType::DISABLED))
#define SKILL_DISABLED(X) (SkillDefs[(X)].flags & SkillType::DISABLED)
#define OBJECT_ENABLED(X) (!(ObjectDefs[(X)].flags & ObjectType::DISABLED))
#define OBJECT_DISABLED(X) (ObjectDefs[(X)].flags & ObjectType::DISABLED)


AString *ShowSkill::Report(Faction *f) {
	if (SkillDefs[skill].flags & SkillType::DISABLED) return NULL;

	AString *str = new AString;
	int val;
	RangeType *range = NULL;

	// Here we pick apart the skill
	switch (skill) {
		case S_FARMING:
			if (level > 1) break;
			*str += "This skill deals with all aspects of agricultural production of plants and their derivates.";
			break;
		case S_RANCHING:
			if (level > 1) break;
			*str += "This skill deals with all aspects raising and exploiting all kinds of livestock.";
			break;
		case S_MINING:
			if (level > 1) break;
			*str += "This skill deals with all aspects of extracting raw "
				"metals and gems from the earth.";
			break;
		case S_LUMBERJACK:
			if (level > 1) break;
			*str += "This skill deals with all aspects of various wood production.";
			break;
		case S_QUARRYING:
			if (level > 1) break;
			*str += "This skill deals with all aspects of various stone production.";
			break;
		case S_HUNTING:
			if (level > 1) break;
			*str += "This skill deals with all aspects of hunting down useful ressources in the wilderness.";
			break;
		case S_FISHING:
			if (level > 1) break;
			*str += "This skill deals with all aspects of extracting ressources from the water.";
			break;
		case S_HERBLORE:
			if (level > 1) break;
			*str += "This skill deals with all aspects of finding wild plantlife.";
			break;
		case S_SMITHING:
			if (level > 1) break;
			*str += "This skill deals with all aspects of production of weapons, armor and metallic goods. It is needed to perfect the arts of weaponcraft or armorcraft";
			break;
		case S_TOOLMAKING:
			if (level > 1) break;
			*str += "This skill deals with all aspects of production of wooden goods that are not weapons.";
			break;
		case S_BUILDING:
			if (level > 1) break;
			*str += "This skill deals with the construction of "
				"fortifications, roads and other buildings, except for trade structures.";
			break;
		case S_SHIPBUILDING:
			if (level > 1) break;
			*str += "This skill deals with the constructions of all types "
				"of ships.";
			break;
		case S_ENTERTAINMENT:
			if (level > 1) break;
			*str += "A unit with this skill may use the ENTERTAIN order "
				"to generate funds. The amount of silver gained will "
				"be 20 per man, times the level of the entertainers. "
				"This amount is limited by the region that the unit is in.";
			break;
		case S_TACTICS:
			if (level > 1) break;
			*str += "Tactics allows the unit, and all allies, to gain a "
				"free round of attacks during battle. The army with the "
				"highest level tactician in a battle will receive this free "
				"round; if the highest levels are equal, no free round is "
				"awarded. Only one free round total will be awarded for any "
				"reason.";
			break;
		case S_COMBAT:
			if (level > 1) break;
			*str += "This skill gives the unit a bonus in hand to hand "
				"combat. Also, a unit with this skill may TAX or PILLAGE.";
			break;
		case S_ANIMALTRAINING:
			if (level > 1) break;
			*str += "This skill deals with all aspects of animal husbandry. It is used to train mounts."; 
			break;
		case S_ANIMALTAMING:
			if (level > 1) break;
			*str += "This skill deals with all aspects of taming wild animals. It is used to tame and train dangerous animals that can aid the owner in combat.";
			break;
		case S_MONSTERTAMING:
			if (level > 1) break;
			*str += "This skill deals with all aspects of taming wild beasts. It is used to tame and train monsters that can aid the owner in combat.";
			break;
		case S_RIDING:
			if (level > 1) break;
			*str += "A unit with this skill, if possessing a mount, may "
				"gain a bonus in combat, if the battle is in a location "
				"where that mount may be utilized and if the skill of the "
				"rider is sufficient to control that mount. The bonus "
				"gained can vary with the mount, the riders skill, and the "
				"terrain.";
			break;
		case S_CROSSBOW:
			if (level > 1) break;
			*str += "A unit with this skill may use any kind of crossbow, either in battle, or to TAX or PILLAGE a "
				"region.";
			break;
		case S_ARCHERY:
			if (level > 1) break;
			*str += "A unit with this skill may use any kind of bow, either in battle, or to TAX or PILLAGE a "
				"region.";
			break;
		case S_STEALTH:
			if (level > 1) break;
			*str += "A unit with this skill is concealed from being seen";
			if (SKILL_ENABLED(S_OBSERVATION)) {
				*str += ", except by units with an Observation skill greater "
					"than or equal to the stealthy unit's Stealth level";
			}
			*str += ".";
			*str += " Knowledge of this skill excludes knowledge of scouting.";
			break;
		case S_OBSERVATION:
			if (level > 1) break;
			*str += "A unit with this skill can see stealthy units or "
				"monsters whose stealth rating is less than or equal to "
				"the observing unit's Observation level. The unit can "
				"also determine the faction owning a unit, provided its "
				"Observation level is higher than the other unit's Stealth "
				"level.";
			break;
		case S_HEALING:
			if (level > 1) break;
			*str += "A unit with this skill is able to heal ";
			*str += Globals->HEALS_PER_MAN;
			*str += " times skill level units hurt in battle.";
			break;
		case S_SAILING:
			if (level > 1) break;
			*str += "A unit with this skill may use the SAIL order to sail "
				"ships.";
			break;
		case S_FORCE:
			if (level > 1) break;
			*str += "The Force skill is one of the three fundamental skills of magic. It deals with the astral force a mage may wield. Mages of the Force Path are often called Sorcerers. They excel in destructive battle magic, weather spells and the summoning of demons.";
			break;
		case S_PATTERN:
			if (level > 1) break;
			*str += "The Pattern skill is one of the three fundamental skills of magic. It deals with the magical ability to influence matter. Mages of the Pattern Path are often called Druids. They excel in nature magic, the production of artifacts and the use of magical gates. Good druids of high skill can even bond with good dragons, while evil druids of same might may control evil dragons.";
			break;
		case S_SPIRIT:
			if (level > 1) break;
			*str += "The Spirit skill is one of the three fundamental skills of magic. It deals with the power of the mind. Mages of the Spirit Path are often called Enchanters or Priests. They excel in discerning hidden things, harming or tricking the minds of other people and they may delve into the dark art of necromancy.";
			break;
		case S_FIRE:
			if (level > 1) break;
			*str += "The Fire skill is an offensive skill for the use in combat. It allows the mage to channel his force into the shape of fire.";
			break;
		case S_ICE:
			if (level > 1) break;
			*str += "The Ice skill is an offensive skill for the use in combat. It allows the mage to create a cone of extreme coldness.";
			break;
		case S_MIND:
			if (level > 1) break;
			*str += "The Mindstrike skill is an offensive skill for the use in combat. It allows the mage to lash out with his psychic power to harm his enemies.";
			break;
		case S_EARTHQUAKE:
			if (level > 1) break;
			*str += "The Earthquake skill is an offensive skill for use in combat. It allows the mage to influence the very rocks his enemies hide behind.";
			break;
		case S_FORCE_SHIELD:
			if (level > 1) break;
			*str += "The Force Shield skill is a defensive skill for use in combat. It allows the mage to shield his allies from nonmagical harm.";
			break;
		case S_ENERGY_SHIELD:
			if (level > 1) break;
			*str += "The Energy Shield skill is a defensive skill for use in combat. It allows the mage to shield his allies from harmful magical energy.";
			break;
		case S_SPIRIT_SHIELD:
			if (level > 1) break;
			*str += "The Spirit Shield skill is a defensive skill for use in combat. It allows the mage to shield his allies from attacks on their minds.";
			break;
		case S_WEATHER_SHIELD:
			if (level > 1) break;
			*str += "The Weather Shield skill is a defensive skill for use in combat. It allows the mage to shield his allies from weather attacks.";
			break;
		case S_MAGICAL_HEALING:
			/* XXX -- This should be cleaner somehow. */
			if (level == 1) {
				*str += "This skill enables the mage to magically heal units "
					"after battle. A mage at this level can heal up to 10 "
					"casualties, with a 50 percent rate of success. No order "
					"is necessary to use this spell, it will be used "
					"automatically when the mage is involved in battle.";
			} else if (level == 3) {
				*str += "A mage at this level of skill can work greater "
					"wonders of healing with his new found powers; he may "
					"heal up to 25 casualties, with a success rate of 75 "
					"percent.";
			} else if (level == 5) {
				*str += "A mage at this skill level can bring soldiers back "
					"from near death; he may heal up to 100 casualties, with "
					"a 90 percent rate of success.";
			}
			break;
		case S_GATE_LORE:
			/* XXX -- This should be cleaner somehow. */
			if (level == 1) {
				*str += "Gate Lore is the art of detecting and using magical "
					"Gates, which are spread through the world. The Gates are "
					"numbered in order, but spread out randomly, so there is "
					"no correlation between the Gate number and the Gate's "
					"location. A mage with skill 1 in Gate Lore can see a "
					"Gate if one exists in the same region as the mage. This "
					"detection is automatic; the Gate will appear in the "
					"region report. A mage with skill 1 in Gate Lore may "
					"also jump through a Gate into another region on the same "
					"level containing a gate, selected at random. To use "
					"Gate Lore in this manner, use the syntax CAST Gate_Lore "
					"RANDOM UNITS <unit> ... UNITS is followed by a list "
					"of units to follow the mage through the Gate (the mage "
					"always jumps through the Gate). At level 1, the mage "
					"may carry 15 weight units through the Gate (including "
					"the weight of the mage).";
			} else if (level == 2) {
				*str += "A mage with Gate Lore skill 2 can detect Gates in "
					"adjacent regions. The mage should use the syntax CAST "
					"Gate_Lore DETECT in order to detect these nearby Gates. "
					"Also, at level 2 Gate Lore, the mage may carry 100 "
					"weight units through a Gate when doing a random jump.";
			} else if (level == 3) {
				*str += "A mage with Gate Lore skill 3 and higher can step "
					"through a Gate into another region containing a specific "
					"Gate. To use this spell, use the syntax CAST Gate_Lore "
					"GATE <number> UNITS <unit> ... <number> specifies the "
					"Gate that the mage will jump to. UNITS is followed by a "
					"list of units to follow the mage through the gate (the "
					"mage always jumps through the gate). At level 3, the "
					"mage may carry 15 weight units through the Gate "
					"(including the mage). Also, a level 3 or higher mage "
					"doing a random gate jump may carry 1000 weight units "
					"through the Gate.";
			} else if (level == 4) {
				*str += "A mage with Gate Lore skill 4 may carry 100 weight "
					"units through a Gate.";
			} else if (level == 5) {
				*str += "A mage with Gate Lore skill 5 may carry 1000 weight "
					"units through a Gate.";
			}
			break;
		case S_PORTAL_LORE:
			if (level > 1) break;
			/* XXX -- This should be cleaner somehow. */
			if (ITEM_DISABLED(I_PORTAL)) break;
			*str += "A mage with the Portal Lore skill may, with the aid of "
				"another mage, make a temporary Gate between two regions, and "
				"send units from one region to another. In order to do this, "
				"both mages (the caster, and the target mage) must have "
				"Portals, and the caster must be trained in Portal Lore. The "
				"caster may teleport units weighing up to 50 weight units "
				"times his skill level, to the target mage's region. ";
			val = SkillDefs[skill].rangeIndex;
			if (val != -1) range = &RangeDefs[val];
			if (range) {
				*str += " The target region must be within ";
				*str += range->rangeMult;
				switch(range->rangeClass) {
					case RangeType::RNG_LEVEL:
						*str += " times the caster's skill level ";
						break;
					case RangeType::RNG_LEVEL2:
						*str += " times the caster's skill level squared ";
						break;
					case RangeType::RNG_LEVEL3:
						*str += " times the caster's skill level cubed ";
						break;
					default:
					case RangeType::RNG_ABSOLUTE:
						break;
				}
				*str += "regions of the caster. ";
			}
			*str += "To use this skill, CAST Portal_Lore <target> UNITS "
				"<unit> ..., where <target> is the unit number of the "
				"target mage, and <unit> is a list of units to be "
				"teleported (the casting mage may teleport himself, if "
				"he so desires).";
			break;
		case S_FARSIGHT:
			if (level > 1) break;
			*str += "A mage with this skill may obtain region reports on "
				"distant regions. The report will be as if the mage was in "
				"the distant region himself.";
			val = SkillDefs[skill].rangeIndex;
			if (val != -1) range = &RangeDefs[val];
			if (range) {
				if (range->flags & RangeType::RNG_SURFACE_ONLY) {
					*str += " This skill only works on the surface of the "
						"world.";
				}
				*str += " The target region must be within ";
				*str += range->rangeMult;
				switch(range->rangeClass) {
					case RangeType::RNG_LEVEL:
						*str += " times the caster's skill level ";
						break;
					case RangeType::RNG_LEVEL2:
						*str += " times the caster's skill level squared ";
						break;
					case RangeType::RNG_LEVEL3:
						*str += " times the caster's skill level cubed ";
						break;
					default:
					case RangeType::RNG_ABSOLUTE:
						break;
				}
				*str += "regions of the caster. ";
				if (range->flags & RangeType::RNG_CROSS_LEVELS) {
					*str += "Coordinates of locations not on the surface are "
						"scaled to the surface coordinates for this "
						"calculation. Attempting to view across different "
						"levels increases the distance by ";
					*str += range->crossLevelPenalty;
					*str += " per level difference. ";
					*str += "To use this skill, CAST Farsight REGION <x> <y> "
						"<z> where <x>, <y>, and <z> are the coordinates of "
						"the region that you wish to view. If you omit the "
						"<z> coordinate, the <z> coordinate of the caster's "
						"current region will be used.";
					if (Globals->UNDERWORLD_LEVELS +
							Globals->UNDERDEEP_LEVELS == 1) {
						*str += " The <z> coordinate for the surface is '1' "
							"and the <z>-coordinate for the underworld is "
							"'2'.";
					}
					*str += " Note that Farsight cannot be used either into "
						"or out of the Nexus.";
				} else {
					*str += "To use this skill, CAST Farsight REGION <x> "
						"<y>, where <x> and <y> are the coordinates of the "
						"region that you wish to view.";
				}
			}
			if (Globals->IMPROVED_FARSIGHT) {
				*str += " Any other skills which the mage has which give "
					"region information will be used when farsight is used.";
			} else {
				*str += " Note that Farsight does not work in conjunction "
					"with other skills or spells; the mage can only rely on "
					"his normal facilities while casting Farsight.";
			}
			break;
		case S_MIND_READING:
			/* XXX -- This should be cleaner somehow. */
			if (level == 1) {
				*str += "A mage with Mind Reading skill 1 may cast the spell "
					"and determine the faction affiliation of any unit he can "
					"see. To use the spell in this manner, CAST Mind_Reading "
					"<unit>, where <unit> is the target unit.";
			} else if (level == 3) {
				*str += "A mage with Mind Reading skill 3 will automatically "
					"determine the faction affiliation of any unit he can "
					"see. Usage of this skill is automatic, and no order is "
					"needed to use it.";
			} else if (level == 5) {
				*str += "A mage with Mind Reading skill 5 can get a full "
					"unit report on any unit he can see. To use this skill, "
					"CAST Mind_Reading <unit> where <unit> is the target "
					"unit.";
			}
			break;
		case S_TELEPORTATION:
			if (level > 1) break;
			/* XXX -- This should be cleaner somehow. */
			*str += "A mage with this skill may teleport himself across"
				"great distances, even without the use of a gate. The mage "
				"may teleport up to 15 weight units per skill level.";
			val = SkillDefs[skill].rangeIndex;
			if (val != -1) range = &RangeDefs[val];
			if (range) {
				if (range->flags & RangeType::RNG_SURFACE_ONLY) {
					*str += " This skill only works on the surface of the "
						"world.";
				}
				*str += " The target region must be within ";
				*str += range->rangeMult;
				switch(range->rangeClass) {
					case RangeType::RNG_LEVEL:
						*str += " times the caster's skill level ";
						break;
					case RangeType::RNG_LEVEL2:
						*str += " times the caster's skill level squared ";
						break;
					case RangeType::RNG_LEVEL3:
						*str += " times the caster's skill level cubed ";
						break;
					default:
					case RangeType::RNG_ABSOLUTE:
						break;
				}
				*str += "regions of the caster. ";
				if (range->flags & RangeType::RNG_CROSS_LEVELS) {
					*str += "Coordinates of locations not on the surface are "
						"scaled to the surface coordinates for this "
						"calculation. Attempting to view across different "
						"levels increases the distance by ";
					*str += range->crossLevelPenalty;
					*str += " per level difference. ";
					*str += "To use this skill, CAST Teleportation REGION "
						"<x> <y> <z> where <x>, <y>, and <z> are the "
						"coordinates of the region that you wish to "
						"teleport to. If you omit the <z> coordinate, the "
						"<z> coordinate of the caster's current region will "
						"be used.";
					if (Globals->UNDERWORLD_LEVELS +
							Globals->UNDERDEEP_LEVELS == 1) {
						*str += " The <z> coordinate for the surface is '1' "
							"and the <z>-coordinate for the underworld is "
							"'2'.";
					}
					*str += " Note that Teleportation cannot be used either "
						"into or out of the Nexus.";
				} else {
					*str += "To use this skill, CAST Teleportation REGION "
						"<x> <y>, where <x> and <y> are the coordinates of "
						"the region that you wish to teleport to.";
				}
			}
			break;
		case S_WEATHER_LORE:
			if (level > 1) break;
			/* XXX -- This should be templated */
			*str += "Weather Lore is the magic of the weather; a mage with "
				"this skill can predict the weather in nearby regions. "
				"Weather Lore also allows further study into more powerful "
				"areas of magic. The weather may be predicted for 3 months "
				"at level 1, 6 months at level 3 and a full year at level "
				"5.";
			val = SkillDefs[skill].rangeIndex;
			if (val != -1) range = &RangeDefs[val];
			if (range) {
				if (range->flags & RangeType::RNG_SURFACE_ONLY) {
					*str += " This skill only works on the surface of the "
						"world.";
				}
				*str += " The target region must be within ";
				*str += range->rangeMult;
				switch(range->rangeClass) {
					case RangeType::RNG_LEVEL:
						*str += " times the caster's skill level ";
						break;
					case RangeType::RNG_LEVEL2:
						*str += " times the caster's skill level squared ";
						break;
					case RangeType::RNG_LEVEL3:
						*str += " times the caster's skill level cubed ";
						break;
					default:
					case RangeType::RNG_ABSOLUTE:
						break;
				}
				*str += "regions of the caster. ";
				if (range->flags & RangeType::RNG_CROSS_LEVELS) {
					*str += "Coordinates of locations not on the surface are "
						"scaled to the surface coordinates for this "
						"calculation. Attempting to view across different "
						"levels increases the distance by ";
					*str += range->crossLevelPenalty;
					*str += " per level difference. ";
					*str += "To use this skill, CAST Weather_Lore REGION "
						"<x> <y> <z> where <x>, <y>, and <z> are the "
						"coordinates of the region where you wish to "
						"predict the weather. If you omit the <z> "
						"coordinate, the <z> coordinate of the caster's "
						"current region will be used.";
					if (Globals->UNDERWORLD_LEVELS +
							Globals->UNDERDEEP_LEVELS == 1) {
						*str += " The <z> coordinate for the surface is '1' "
							"and the <z>-coordinate for the underworld is "
							"'2'.";
					}
					*str += " Note that Weather Lore cannot be used either "
						"into or out of the Nexus.";
				} else {
					*str += "To use this skill, CAST Weather_Lore REGION "
						"<x> <y>, where <x> and <y> are the coordinates of "
						"the region where you wish to predict the weather.";
				}
			}
			*str += " A mage with Weather Lore skill will perceive the use "
				"of Weather Lore by any other mage in the same region.";
			break;
		case S_SUMMON_WIND:
			/* XXX -- This should be cleaner somehow. */
			/* This is cleaner now.*/
			/*if (level == 1) {
				*str += "A mage with knowledge of Summon Wind can summon "
					   "up the powers of the wind to aid him in sea or "
					   "air travel. Usage of this spell is automatic.";
				if (OBJECT_ENABLED(O_LONGBOAT)) {
					*str += " At level 1, if the mage is in a Longboat";
					if (OBJECT_ENABLED(O_ROWINGBOAT)) {
					  *str += " or Rowing Boat";
					}
					*str += ", that ship will get 2 extra movement points.";
				}
				*str += " If the mage is flying, he will receive 2 extra "
					   "movement points.";
			} else if (level == 2) {
				if (OBJECT_DISABLED(O_CLIPPER)) break;
				*str += "With level 2 Summon Wind, any ship of Clipper size "
					   "or smaller that the mage is inside will receive a "
					   "2 movement point bonus.";
			} else if (level == 3) {
				*str += "At level 3 of Summon Wind, any ship the mage is in "
					   "will receive a 2 movement point bonus. Note that "
					   "study of Summon Wind beyond level 3 does not "
					   "yield any further powers.";
			}*/
			*str += "A mage with knowledge of Summon Wind can summon "
				"up the powers of the wind to aid him in sea or "
				"air travel. Usage of this spell is automatic.";
			*str += " The vessel the mage is in will recieve a movement bonus equal to his skill level.";
            *str += " If the mage is flying, he will receive extra movement points equal to his skill level.";
			break;
		case S_SUMMON_STORM:
			if (level > 1) break;
			*str += "The Summon Storm skill is an offensive skill for use in combat. It allows the mage to hinder his enemies in personal combat.";
			break;
		case S_SUMMON_TORNADO:
			if (level > 1) break;
			*str += "The Summon Tornado skill is a mighty offensive skill for use in combat. It allows the mage to decimate his enemies with his powers over the weather.";
			break;
		case S_CALL_LIGHTNING:
			if (level > 1) break;
			*str += "The Call Lightning skill is a mighty offensive skill for use in combat. It allows the mage to decimate his enemies by drowning them in rain and blasting them with lightning.";
			break;
		case S_CLEAR_SKIES:
			/* XXX -- this range stuff needs cleaning up */
			if (level > 1) break;
			if (SkillDefs[skill].flags & SkillType::CAST) {
				*str += "When cast using the CAST order, it causes the "
					"region to have good weather for the entire month; "
					"movement is at the normal rate (even if it is winter) "
					"and the economic production of the region is improved "
					"for a month (this improvement of the economy will "
					"actually take effect during the turn after the spell "
					"is cast).";
				val = SkillDefs[skill].rangeIndex;
				if (val != -1) range = &RangeDefs[val];
				if (range) {
					if (range->flags & RangeType::RNG_SURFACE_ONLY) {
						*str += " This skill only works on the surface of "
							"the world.";
					}
					*str += " The target region must be within ";
					*str += range->rangeMult;
					switch(range->rangeClass) {
						case RangeType::RNG_LEVEL:
							*str += " times the caster's skill level ";
							break;
						case RangeType::RNG_LEVEL2:
							*str += " times the caster's skill level squared ";
							break;
						case RangeType::RNG_LEVEL3:
							*str += " times the caster's skill level cubed ";
							break;
						default:
						case RangeType::RNG_ABSOLUTE:
							break;
					}
					*str += "regions of the caster. ";
					if (range->flags & RangeType::RNG_CROSS_LEVELS) {
						*str += "Coordinates of locations not on the surface "
							"are scaled to the surface coordinates for this "
							"calculation. Attempting to view across "
							"different levels increases the distance by ";
						*str += range->crossLevelPenalty;
						*str += " per level difference. ";
						*str += "To use this skill, CAST Clear_Skies REGION "
							"<x> <y> <z> where <x>, <y>, and <z> are the "
							"coordinates of the region where you wish to "
							"improve the weather. If you omit the <z> "
							"coordinate, the <z> coordinate of the caster's "
							"current region will be used.";
						if (Globals->UNDERWORLD_LEVELS +
								Globals->UNDERDEEP_LEVELS == 1) {
							*str += " The <z> coordinate for the surface is "
								"'1' and the <z>-coordinate for the "
								"underworld is '2'.";
						}
						*str += " Note that Clear Skies cannot be used "
							"either into or out of the Nexus.";
					} else {
						*str += "To use this skill, CAST Clear_Skies REGION "
							"<x> <y>, where <x> and <y> are the coordinates "
							"of the region where you wish to improve the "
							"weather.";
					}
				} else {
					*str += " To use the spell in this fashion, CAST "
						"Clear_Skies; no arguments are necessary.";
				}
			}
			break;
		case S_EARTH_LORE:
			if (level > 1) break;
			*str += "Earth Lore is the study of nature, plants, and animals. "
				"A mage with knowledge of Earth Lore can use his knowledge "
				"of nature to aid local farmers, raising money for himself, "
				"and aiding the agricultural production in the "
				"region. To use the spell, CAST Earth_Lore; the mage will "
				"receive an amount of money based on his level, and the "
				"economy of the region. Also, a mage with knowledge of Earth "
				"Lore will detect the use of Earth Lore by any other mage in "
				"the same region.";
			break;
		case S_DRAGON_LORE:
			if (level > 1) break;
			*str += "Dragon Lore is the study of good dragons. It has no direct magical applications, but is needed to understand good dragons good enough to bond with them after more study.";
			break;
		case S_WYRM_LORE:
			if (level > 1) break;
			*str += "Wyrm Lore is the study of evil dragons. It has no direct magical applications, but is needed to know enough about evil dragons to be able to control them after more study.";
			break;
		case S_WOLF_LORE:
			/* XXX -- This should be cleaner somehow. */
		  if (level == 1) {
			if (ITEM_DISABLED(I_WOLF)) break;
			*str += "A mage with Wolf Lore skill may summon wolves, who will "
				"fight for him in combat. A mage may summon a number of "
				"wolves equal to ";
			if( Globals->TARMELLION_SUMMONING ) {
				*str += "his skill level plus one, divided by two, ";
			} else {
				*str += "his skill level ";
			}
			*str += "per month. ";
			*str += "The mage may control a total of ";
			if( Globals->TARMELLION_SUMMONING ) {
				*str += "his skill level squared ";
			} else {
				*str += "his skill level squared times 4 ";
			}
			*str += "wolves; the wolves will "
				"be placed in the mages inventory. To "
				"summon wolves, the mage should issue the order CAST "
				"Wolf_Lore";
			if( Globals->TARMELLION_SUMMONING ) {
			  *str += " WOLF.";
			} else {
			  *str += ".";
			}
		  } else if (level == 3) {
			if (ITEM_DISABLED(I_ICEWOLF)) break;
			*str += "A mage with Wolf Lore skill of level 3 may summon ice wolves, who will "
				"fight for him in combat. A mage may summon one ice "
				"wolf ";
			*str += "per month. ";
			*str += "The mage may control a total of ";
			*str += "his skill level ";
			*str += "ice wolves; the ice wolves will "
				"be placed in the mages inventory. To "
				"summon ice wolves, the mage should issue the order CAST "
				"Wolf_Lore ICE_WOLF.";
		  }
		  break;
		case S_BIRD_LORE:
			/* XXX -- This should be cleaner somehow. */
			if (level == 1) {
				*str += "A mage with Bird Lore may control the birds of the "
					"sky. At skill level 1, the mage can control small "
					"birds, sending them to an adjacent region to obtain a "
					"report on that region. (This skill only works on the "
					"surface of the world, as there are no birds elsewhere)."
					" To use this skill, CAST Bird_Lore DIRECTION <dir>, "
					"where <dir> is the direction the mage wishes the birds "
					"to report on.";
			} else if (level == 3) {
				if (ITEM_DISABLED(I_EAGLE)) break;
				*str += "A mage with Bird Lore 3 can summon eagles to join "
					"him, who will aid him in combat, and provide for flying "
					"transportation. A mage may summon a number of "
					"eagles equal to ";
				if( Globals->TARMELLION_SUMMONING ) {
					*str += "his skill level plus one, divided by two, ";
				} else {
					*str += "his skill level plus one, divided by two, ";
				}
				*str += "per month. ";
				*str += "The mage may control a total of ";
				if( Globals->TARMELLION_SUMMONING ) {
					*str += "his skill level squared ";
				} else {
					*str += "his skill level squared ";
				}
				*str += "eagles; the eagles "
					"will appear in his inventory. To summon an eagle, issue "
					"the order CAST Bird_Lore EAGLE.";
			}
			
			break;
		case S_SUMMON_DRAGON:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
//			if (ITEM_DISABLED(I_REDDRAGON)) break;
			*str += "A mage with Summon Dragon skill can summon good dragons to "
				"join him, to aid in battle, and provide flying "
				"transportation. ";
			if( !Globals->TARMELLION_SUMMONING ) {
				*str += "A mage at level 1 has a low chance of "
					"successfully summoning a dragon, gradually increasing until "
					"at level 5 he may summon one dragon per turn; the total "
					"number of dragons that a mage may control at one time is "
					"equal to his skill level.  To attempt to summon a good dragon, ";
			} else {
				*str += "To summon a good dragon, ";
			}
			*str += "CAST Summon_Dragon <dragon> to cast this spell, where <dragon> "
				"is the desired type of dragon.";
			break;
		case S_SUMMON_WYRM:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
//			if (ITEM_DISABLED(I_REDDRAGON)) break;
			*str += "A mage with Summon Wyrm skill can summon evil dragons to "
				"join him, to aid in battle, and provide flying "
				"transportation. ";
			if( !Globals->TARMELLION_SUMMONING ) {
				*str += "A mage at level 1 has a low chance of "
					"successfully summoning a dragon, gradually increasing until "
					"at level 5 he may summon one dragon per turn; the total "
					"number of dragons that a mage may control at one time is "
					"equal to his skill level.  To attempt to summon an evil dragon, ";
			} else {
				*str += "To summon an evil dragon, ";
			}
			*str += "CAST Summon_Wyrm <dragon> to cast this spell, where <dragon> "
				"is the desired type of dragon.";
			break;
		case S_NECROMANCY:
			if (level > 1) break;
			*str += "Necromancy is the magic of death; a mage versed in "
				"Necromancy may raise and control the dead, and turn the "
				"powers of death to his own nefarious purposes. The "
				"Necromancy skill does not have any direct application, but "
				"is required to study the more powerful Necromantic skills. "
				"A mage with knowledge of Necromancy will detect the use of "
				"Necromancy by any other mage in the same region.";
			break;
		case S_SUMMON_SKELETONS:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
			if (ITEM_DISABLED(I_SKELETON)) break;
			*str += "A mage with the Summon Skeletons skill may summon "
				"skeletons into his inventory, to aid him in battle";
			if( !Globals->TARMELLION_SUMMONING ) {
				*str += ". Skeletons may be given to other units, as they follow "
				"instructions mindlessly; however, they have a 10 percent "
					"chance of decaying each turn";
			}
			if( Globals->TARMELLION_SUMMONING ) {
				*str += ". A mage can summon two skeletons per month. ";
			} else {
				*str += ". A mage can summon skeletons "
					"at an average rate of 40 percent times his level squared. ";
			}
			if( Globals->TARMELLION_SUMMONING ) {
				*str += "The mage may control a total of his skill level "
					"squared skeletons. ";
			}
			*str +=	"To use the spell, use the order CAST Summon_Skeletons, "
				"and the mage will summon as many skeletons as he is able.";
			break;
		case S_RAISE_UNDEAD:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
			if (ITEM_DISABLED(I_UNDEAD)) break;
			*str += "A mage with the Raise Undead skill may summon undead "
				"into his inventory, to aid him in battle";
			if( !Globals->TARMELLION_SUMMONING ) {
				*str += ". Undead may be given to other units, as they follow instructions "
				"mindlessly; however, they have a 10 percent "
					"chance of decaying each turn";
			}
			if( Globals->TARMELLION_SUMMONING ) {
				*str += ". A mage can summon one undead per month. ";
			} else {
				*str += ". A mage can summon undead "
					"at an average rate of 10 percent times his level squared. ";
			}
			if( Globals->TARMELLION_SUMMONING ) {
				*str += "The mage may control a total of his skill level "
					"undead. ";
			}
			*str += "To use the "
				"spell, use the order CAST Raise_Undead and the mage will "
				"summon as many undead as he is able.";
			break;
		case S_SUMMON_LICH:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
			if (ITEM_DISABLED(I_LICH)) break;
			*str += "A mage with the Summon Lich skill may summon a lich "
				"into his inventory, to aid him in battle";
			if( !Globals->TARMELLION_SUMMONING ) {
				*str += ". Liches may be given to other units, as they follow instructions "
					"mindlessly; however, they have a 10 percent "
					"chance of decaying each turn";
			}
			if( Globals->TARMELLION_SUMMONING ) {
				*str += ". A mage can summon one lich per month. ";
			} else {
				*str += ". A mage has a 2 percent times his level "
					"squared chance of summoning a lich";
			}
			if( Globals->TARMELLION_SUMMONING ) {
				*str += "The mage may control a total of his skill level "
					"liches. ";
			}
			*str += "To summon a lich, use the order CAST Summon_Lich.";
			break;
		case S_CREATE_AURA_OF_FEAR:
			if (level > 1) break;
			*str += "The Create Aura of Fear skill is an offensive skill for use in combat. It allows the mage to impress his enemies with fear of himself, reducing their effectiveness in combat.";
			break;
		case S_SUMMON_BLACK_WIND:
			if (level > 1) break;
			*str += "The Summon Black Wind skill is a mighty offensive skill for use in combat. It allows the mage to send a cloud of obnoxious gases to his enemies, that will kill them if touched.";
			break;
		case S_BANISH_UNDEAD:
			if (level > 1) break;
			*str += "The Banish Undead skill is a defensive skill for use in combat. It allows the mage to send undead creatures back to their graves.";
			break;
		case S_DEMON_LORE:
			if (level > 1) break;
			*str += "Demon Lore is the art of summoning and controlling "
				"demons. The Demon Lore skill does not give the mage any "
				"direct skills, but is required to study further into the "
				"Demonic arts. A mage with knowledge of Demon Lore will "
				"detect the use of Demon Lore by any other mage in the same "
				"region.";
			break;
		case S_SUMMON_IMPS:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
			if (ITEM_DISABLED(I_IMP)) break;
			*str += "A mage with the Summon Imps skill may summon imps into "
				"his inventory, to aid him in combat. ";
			*str += "A mage may summon a number of imps equal to ";
			if( Globals->TARMELLION_SUMMONING ) {
				*str += "his skill level plus 1, divided by 2 ";
			} else {
				*str += "his level ";
			}
			*str += "per month. ";
			if( Globals->TARMELLION_SUMMONING ) {
				*str += "The mage may control a total of "
						"his skill level squared imps. ";
			} else {
				*str += "The imps have a chance of "
					"breaking free of the mage's control at the end of each "
					"turn. This chance is based on the number of imps in the "
					"mage's control; if the mage has his skill level squared "
					"times 4 imps, the chance is 5 percent; this chance goes "
					"up or down quickly if the mage controls more or fewer imps. ";
			}
			*str +=	"To use this spell, the mage should issue the order CAST "
				"Summon_Imps, and the mage will summon as many imps as he "
				"is able.";
			break;
		case S_SUMMON_DEMON:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
			if (ITEM_DISABLED(I_DEMON)) break;
			*str += "A mage with the Summon Demon skill may summon demons "
				"into his inventory, to aid him in combat. ";
			if( Globals->TARMELLION_SUMMONING ) {
				*str += "A mage may summon one demon ";
			} else {
				*str += "A mage may summon one demon ";
			}
			*str += "per month. ";
			if( Globals->TARMELLION_SUMMONING ) {
				*str += "The mage may control a total of "
						"his skill level demons. ";
			} else {
				*str += "The demons have a chance of "
				"breaking free of the mage's control at the end of each "
				"turn. This chance is based on the number of demons in the "
				"mage's control; if the mage has a number of demons equal "
				"to his skill level squared, the chance is 5 percent; this "
				"chance goes up or down quickly if the mage controls more or "
				"fewer demons. ";
			}
			*str += "To use this spell, the mage should issue the "
				"order CAST Summon_Demon.";
			break;
		case S_SUMMON_BALROG:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
			if (ITEM_DISABLED(I_BALROG)) break;
			*str += "A mage with the Summon Balrog skill may summon a balrog "
				"into his inventory, to aid him in combat. ";
			if( Globals->TARMELLION_SUMMONING ) {
				*str += "A mage may summon one balrog per month. ";
			} else {
				*str += "A mage has a 20 "
				"percent times his skill level chance of summoning a balrog, "
				"but may only summon a balrog if one is not already under "
				"his control. ";
			}
			if( Globals->TARMELLION_SUMMONING ) {
				*str += "The mage may only control one balrog at once. ";
			} else {
				*str += "As with other demons, the balrog has a chance "
				"of breaking free of the mage's control at the end of each "
				"turn. This chance is equal to 1 over 4 times the mage's "
				"skill level to the fourth power (or, from 1 over 4 at "
				"level 1, to 1 over 2500 at level 5). ";
			}
			*str += "To use this spell, "
				"the mage should issue the order CAST Summon_Balrog.";
			break;
		case S_BANISH_DEMONS:
			if (level > 1) break;
			*str += "The Banish Demons skill is a defensive skill for use in combat. It allows the mage to send demonic creatures back to hell.";
			break;
		case S_ILLUSION:
			if (level > 1) break;
			*str += "Illusion is the magic of creating images of things that "
				"do not actually exist. The Illusion skill does not have any "
				"direct applications, but is required for further study of "
				"illusory magic. A mage with knowledge of the Illusion "
				"skill will detect the use of Illusion by any other mage in "
				"the same region.";
			break;
		case S_PHANTASMAL_ENTERTAINMENT:
			/* XXX -- This should be cleaner somehow */
			if (level > 1) break;
			*str += "A mage with the Phantasmal Entertainment skill may use "
				"his powers of Illusion to earn money by creating "
				"illusory fireworks, puppet shows, etc. In effect, "
				"Phantasmal Entertainment grants the mage Entertainment "
				"skill equal to ten times his Phantasmal Entertainment "
				"level. To use this skill, use the ENTERTAIN order.";
			break;
		case S_CREATE_PHANTASMAL_BEASTS:
			/* XXX -- This should be cleaner somehow. */
			if (level == 1) {
				*str += "A mage with Create Phantasmal Beasts may summon "
					"illusory beasts that appear in the mage's inventory. "
					"These beasts will fight in combat, but do not attack, "
					"and are killed whenever they are attacked.";
				if (ITEM_ENABLED(I_IWOLF)) {
					if( Globals->TARMELLION_SUMMONING ) {
						*str += " Create Phantasmal Beasts at level 1 allows the "
							"mage to summon illusory wolves. A mage may summon "
							"up to two illusory wolves per month, and may control "
							"up to his skill level squared illusory wolves in total.";
					} else {
						*str += " Create Phantasmal Beasts at level 1 allows the "
							"mage to summon illusory wolves; the number the "
							"mage can summon, or have in his inventory at one "
							"time is equal to the mage's skill squared times 4.";
					}
				}
				*str +=	" To use this spell, the mage should CAST ";
				if( Globals->TARMELLION_SUMMONING ) {
					*str += "Create_Phantasmal_Beasts <beast>, where "
						"<beast> is the type of illusion the mage wishes to "
						"summon.";
				} else {
					*str += "Create_Phantasmal_Beasts WOLF <number>, where "
						"<number> is the number of wolves that the mage "
						"wishes to have appear in his inventory.";
				}
				*str += " Note: illusory beasts will appear on reports as "
					"if they were normal items, except on the owner's "
					"report, where they are marked as illusory. To "
					"reference these items in orders, you must prepend an "
					"'i' to the normal string. (For example: to reference "
					"an illusory wolf, you would use 'iWOLF').";
			} else if (level == 3) {
				if (ITEM_DISABLED(I_IEAGLE)) break;
				if( Globals->TARMELLION_SUMMONING ) {
					*str += " Create Phantasmal Beasts at level 3 allows the "
						"mage to summon illusory eagles. A mage may summon "
						"up to one illusory eagle per month, and may control "
						"up to his skill level illusory eagles in total.";
				} else {
					*str += " Create Phantasmal Beasts at level 3 allows the "
						"mage to summon illusory eagles; the number the "
						"mage can summon, or have in his inventory at one "
						"time is equal to the mage's skill level minus 2, "
						"squared. To summon illusory eagles, the mage should CAST "
						"Create_Phantasmal_Beasts EAGLE <number>, where <number> "
						"is the number of eagles that the mage wishes to have "
						"appear in his inventory.";
				}
			} else if (level == 5) {
//				if (ITEM_DISABLED(I_IRED_DRAGON)) break;
				if( Globals->TARMELLION_SUMMONING ) {
					*str += " Create Phantasmal Beasts at level 5 allows the "
						"mage to summon illusory dragons. A mage may summon "
						"up to one illusory dragon per month, and may control "
						"up to his skill level illusory dragons in total.";
				} else {
					*str += " Create Phantasmal Beasts at level 5 allows the "
						"mage to summon an illusory dragon; the mage "
						"can only have one illusory dragon in his inventory "
						"at one time. To summon an illusory dragon, the mage "
						"should CAST Create_Phantasmal_Beasts DRAGON.";
				}
			}
			break;
		case S_CREATE_PHANTASMAL_UNDEAD:
			/* XXX -- This should be cleaner somehow. */
			if (level == 1) {
				*str += "A mage with Create Phantasmal Undead may summon "
					"illusory undead that appear in the mage's inventory. "
					"These undead will fight in combat, but do not attack, "
					"and are killed whenever they are attacked.";
				if (ITEM_ENABLED(I_ISKELETON)) {
					if( Globals->TARMELLION_SUMMONING ) {
						*str += " Create Phantasmal Undead at level 1 allows the "
							"mage to summon illusory skeletons. A mage may summon "
							"up to two illusory skeletons per month, and may control "
							"up to his skill level squared illusory skeletons in total.";
					} else {
						*str += " Create Phantasmal Undead at level 1 allows the "
							"mage to summon illusory skeletons; the number the "
							"mage can summon, or have in his inventory at one "
							"time is equal to the mage's skill squared times 4.";
					}
				}
				*str +=	" To use this spell, the mage should CAST ";
				if( Globals->TARMELLION_SUMMONING ) {
					*str += "Create_Phantasmal_Undead <undead>, where "
						"<undead> is the type of illusion the mage wishes to "
						"summon.";
				} else {
					*str += "Create_Phantasmal_Beasts SKELETON <number>, where "
						"<number> is the number of skeletons that the mage "
						"wishes to have appear in his inventory.";
				}
				*str += " Note: illusory undead will appear on reports as "
					"if they were normal items, except on the owner's "
					"report, where they are marked as illusory. To "
					"reference these items in orders, you must prepend an "
					"'i' to the normal string. (For example: to reference "
					"an illusory skeleton, you would use 'iSKELETON').";
			} else if (level == 3) {
				if (ITEM_DISABLED(I_IUNDEAD)) break;
				if( Globals->TARMELLION_SUMMONING ) {
					*str += " Create Phantasmal Undead at level 3 allows the "
						"mage to summon illusory undead. A mage may summon "
						"up to one illusory undead per month, and may control "
						"up to his skill level illusory undead in total.";
				} else {
					*str += " Create Phantasmal Undead at level 3 allows the "
						"mage to summon illusory undead; the number the "
						"mage can summon, or have in his inventory at one "
						"time is equal to the mage's skill level minus 2, "
						"squared. To summon illusory undead, the mage should CAST "
						"Create_Phantasmal_Undead UNDEAD <number>, where <number> "
						"is the number of undead that the mage wishes to have "
						"appear in his inventory.";
				}
			} else if (level == 5) {
				if (ITEM_DISABLED(I_ILICH)) break;
				if( Globals->TARMELLION_SUMMONING ) {
					*str += " Create Phantasmal Undead at level 5 allows the "
						"mage to summon illusory liches. A mage may summon "
						"up to one illusory lich per month, and may control "
						"up to his skill level times two illusory liches in total.";
				} else {
					*str += " Create Phantasmal Undead at level 5 allows the "
						"mage to summon an illusory lich; the mage "
						"can only have one illusory lich in his inventory "
						"at one time. To summon an illusory lich, the mage "
						"should CAST Create_Phantasmal_Undead LICH.";
				}
			}
			break;
		case S_CREATE_PHANTASMAL_DEMONS:
			/* XXX -- This should be cleaner somehow. */
			if (level == 1) {
				*str += "A mage with Create Phantasmal Demons may summon "
					"illusory demons that appear in the mage's inventory. "
					"These demons will fight in combat, but do not attack, "
					"and are killed whenever they are attacked.";
				if (ITEM_ENABLED(I_IIMP)) {
					if( Globals->TARMELLION_SUMMONING ) {
						*str += " Create Phantasmal Demons at level 1 allows the "
							"mage to summon illusory imps. A mage may summon "
							"up to two illusory imps per month, and may control "
							"up to his skill level squared illusory imps in total.";
					} else {
						*str += " Create Phantasmal Demons at level 1 allows the "
							"mage to summon illusory imps; the number the "
							"mage can summon, or have in his inventory at one "
							"time is equal to the mage's skill squared times 4.";
					}
				}
				*str +=	" To use this spell, the mage should CAST ";
				if( Globals->TARMELLION_SUMMONING ) {
					*str += "Create_Phantasmal_Beasts <demon>, where "
						"<demon> is the type of illusion the mage wishes to "
						"summon.";
				} else {
					*str += "Create_Phantasmal_Beasts IMP <number>, where "
						"<number> is the number of imps that the mage "
						"wishes to have appear in his inventory.";
				}
				*str += " Note: illusory demons will appear on reports as "
					"if they were normal items, except on the owner's "
					"report, where they are marked as illusory. To "
					"reference these items in orders, you must prepend an "
					"'i' to the normal string. (For example: to reference "
					"an illusory imp, you would use 'iIMP').";
			} else if (level == 3) {
				if (ITEM_DISABLED(I_IDEMON)) break;
				if( Globals->TARMELLION_SUMMONING ) {
					*str += " Create Phantasmal Demons at level 3 allows the "
						"mage to summon illusory demons. A mage may summon "
						"up to one illusory demon per month, and may control "
						"up to his skill level squared illusory demons in total.";
				} else {
					*str += " Create Phantasmal Demons at level 3 allows the "
						"mage to summon illusory demons; the number the "
						"mage can summon, or have in his inventory at one "
						"time is equal to the mage's skill level minus 2, "
						"squared. To summon illusory demons, the mage should CAST "
						"Create_Phantasmal_Beasts DEMON <number>, where <number> "
						"is the number of demons that the mage wishes to have "
						"appear in his inventory.";
				}
			} else if (level == 5) {
				if (ITEM_DISABLED(I_IBALROG)) break;
				if( Globals->TARMELLION_SUMMONING ) {
					*str += " Create Phantasmal Demons at level 5 allows the "
						"mage to summon illusory balrogs. A mage may summon "
						"up to one illusory balrog per month, and may control "
						"only one balrog at one time.";
				} else {
					*str += " Create Phantasmal Demons at level 5 allows the "
						"mage to summon an illusory balrog; the mage "
						"can only have one illusory balrog in his inventory "
						"at one time. To summon an illusory balrog, the mage "
						"should CAST Create_Phantasmal_Beasts BALROG.";
				}
			}
			break;

		case S_INVISIBILITY:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
			*str += "The Invisibility skill allows a mage to render other "
				"units nearly invisible to other factions, giving them a +3 "
				"bonus to Stealth. This invisibility will last until the "
				"next Magic round. To cast this spell, use the order CAST "
				"Invisibility UNITS <unit> ..., where <unit> is a list of "
				"the units that the mage wishes to render invisible. A mage "
				"may render invisible a number of men or creatures equal to "
				"his skill level squared.";
			if (SKILL_ENABLED(S_OBSERVATION)) {
				*str += "In addition, a mage with the Invisibility skill "
					"receives a bonus to his Stealth skill equal to his "
					"Invisibility skill.";
			}
			break;
		case S_TRUE_SEEING:
			if (level > 1) break;
			*str += "A mage with the True Seeing spell can see illusions "
				"for what they really are. Whether or not the mage can see "
				"through the illusion depends on his True Seeing skill "
				"being higher that the Illusion skill of the mage casting "
				"the illusion. This spell does not require any order to "
				"use; it is used automatically.";
			if (SKILL_ENABLED(S_OBSERVATION)) {
				*str += "In addition, a mage with the True Seeing skill "
					"receives a bonus to his Observation skill equal to his "
					"True Seeing skill.";
			}
			break;
		case S_DISPEL_ILLUSIONS:
			if (level > 1) break;
			*str += "The Dispel Illusion skill is used to counter the use of illusions by another mage.";
			break;
		case S_ARTIFACT_LORE:
			if (level > 1) break;
			*str += "Artifact Lore is one of the most advanced forms of "
				"magic; in general, creation of an artifact requires both "
				"Artifact Lore, and an appropriate skill for the item being "
				"created. A mage with knowledge of the Artifact Lore skill "
				"will detect the use of Artifact Lore by any other mage in "
				"the region.";
			break;
		case S_CREATE_RING_OF_INVISIBILITY:
			/* XXX -- This should be cleaner somehow. */
			if (ITEM_DISABLED(I_RINGOFI)) break;
			if (level > 1) break;
			*str += "A mage with the Create Ring of Invisibility skill may "
				"create a Ring of Invisibility, which grants a 3 bonus to a "
				"unit's effective Stealth (note that a unit must possess "
				"one ring for each man to gain this bonus).";
			*str += " A mage has a 20 percent times his level chance to "
				"create a Ring of Invisibility. To use this spell, the mage "
				"should CAST Create_Ring_of_Invisibility.";
			break;
		case S_CREATE_CLOAK_OF_INVULNERABILITY:
			/* XXX -- This should be cleaner somehow. */
			if (ITEM_DISABLED(I_CLOAKOFINVULNERABILITY)) break;
			if (level > 1) break;
			*str += "A mage with the Create Cloak of Invulnerability skill "
				"may create a Cloak of Invulnerability. A mage has a 20 "
				"percent times his level chance to create a Cloak of "
				"Invulnerability. To use this spell, the mage should CAST "
				"Create_Cloak_of_Invulnerability.";
			break;
		case S_CREATE_STAFF_OF_FIRE:
			/* XXX -- This should be cleaner somehow. */
			if (ITEM_DISABLED(I_STAFFOFF)) break;
			if (level > 1) break;
			*str += "A mage with the Create Staff of Fire skill may create a "
				"Staff of Fire. A Staff of Fire allows any mage to throw "
				"fireballs in combat as if he had a Fire skill of 3. A mage "
				"has a 20 percent times his level chance to create a Staff "
				"of Fire. To use this spell, CAST Create_Staff_of_Fire.";
			break;
		case S_CREATE_STAFF_OF_LIGHTNING:
			/* XXX -- This should be cleaner somehow. */
			if (ITEM_DISABLED(I_STAFFOFL)) break;
			if (level > 1) break;
			*str += "A mage with the Create Staff of Lightning skill may "
				"create a Staff of Lightning. A Staff of Lightning allows "
				"any mage to call down lightning bolts as if he had Call "
				"Lightning skill of 3. A mage has a 20 percent times his "
				"level chance to create a Staff of Lightning. To use this "
				"spell, CAST Create_Staff_of_Lightning.";
			break;
		case S_CREATE_AMULET_OF_TRUE_SEEING:
			/* XXX -- This should be cleaner somehow. */
			if (ITEM_DISABLED(I_AMULETOFTS)) break;
			if (level > 1) break;
			*str += "A mage with the Create Amulet of Tree Seeing skill may "
				"create an Amulet of True Seeing. This amulet gives the "
				"possessor a bonus of 3 to his effective Observation skill.";
			*str += "A mage has a 20 percent times his skill level chance to "
			   "create an Amulet of True Seeing. To use this spell, CAST "
			   "Create_Amulet_of_True_Seeing.";
			break;
		case S_CREATE_AMULET_OF_PROTECTION:
			/* XXX -- This should be cleaner somehow. */
			if (ITEM_DISABLED(I_AMULETOFP)) break;
			if (level > 1) break;
			*str += "A mage with the Create Amulet of Protection skill may "
				"create Amulets of Protection, which grants the possesor a "
				"personal Spirit Shield of 3. A mage may create his skill "
				"level of these amulets per turn. To use this spell, CAST "
				"Create_Amulet_of_Protection.";
			break;
		case S_CREATE_RUNESWORD:
			/* XXX -- This should be cleaner somehow. */
			if (ITEM_DISABLED(I_RUNESWORD)) break;
			if (level > 1) break;
			*str += "A mage with the Create Runesword skill may create a "
				"Runesword, which when wielded in combat gives the wielder "
				"a plus 4 bonus to Combat skill, and also allows the wielder "
				"to project an Aura of Fear in battle, as if he had Create "
				"Aura of Fear skill of level 3 (provided the wielder is "
				"not casting any other combat spells). A mage has a 20 "
				"percent times his skill level chance of creating a "
				"Runesword. To cast this spell, CAST Create_Runesword.";
			break;
		case S_CREATE_SHIELDSTONE:
			/* XXX -- This should be cleaner somehow. */
			if (ITEM_DISABLED(I_SHIELDSTONE)) break;
			if (level > 1) break;
			*str += "A mage with the Create Shieldstone skill may create "
				"Shieldstones, which confers upon the possessor a personal "
				"Energy Shield of 3. A mage may create his skill level in "
				"Shieldstones per turn. To use this spell, CAST "
				"Create_Shieldstone";
			break;
		case S_CREATE_MAGIC_CARPET:
			/* XXX -- This should be cleaner somehow. */
			if (ITEM_DISABLED(I_MCARPET)) break;
			if (level > 1) break;
			*str += "A mage with the Create Magic Carpet skill may create "
				"Magic Carpets, which provide for flying transportation. A "
				"Magic Carpet can carry up to 50 weight units in the air. "
				"Casting this spell allows the mage to create his skill "
				"level in Magic Carpets. To cast the spell, CAST "
				"Create_Magic_Carpet.";
			break;
		case S_ENGRAVE_RUNES_OF_WARDING:
			/* XXX -- This should be cleaner somehow. */
			if (level == 1) {
				*str += "A mage with the Engrave Runes of Warding may "
					"engrave runes of warding on a building; these runes "
					"will give any occupants of the building a personal "
					"Energy Shield and Spirit Shield, both at level 3. A "
					"mage has a 20 percent chance per level of succeeding "
					"with each attempt to cast this spell. To use this "
					"spell, the mage should CAST Engrave_Runes_of_Warding, "
					"and be within the building he wishes to engrave runes "
					"upon. This spell costs 600 silver to cast.";
				if (OBJECT_ENABLED(O_TOWER)) {
					*str += " At level 1, the mage may engrave runes of "
						"warding upon a Tower.";
				}
			} else if (level == 2) {
				int comma = 0;
				if (OBJECT_DISABLED(O_FORT) && OBJECT_DISABLED(O_MAGICALTOWER))
					break;
				*str += "At this level, the mage may engrave runes of "
					"warding upon ";
				if (OBJECT_ENABLED(O_FORT)) {
					*str += "a Fort";
					comma = 1;
				}
				if (OBJECT_ENABLED(O_MAGICALTOWER)) {
					if (comma) *str += ", and ";
					*str += "a Magic Tower";
				}
				*str += ".";
			} else if (level == 3) {
				if (OBJECT_DISABLED(O_CASTLE)) {
					*str += "At this level, the mage may engrave runes of "
						"warding upon a Castle.";
				}
			} else if (level == 4) {
				if (OBJECT_ENABLED(O_CITADEL)) {
					*str += "At this level, the mage may engrave runes of "
						"warding upon a Citadel.";
				}
			} else if (level == 5) {
				if (OBJECT_ENABLED(O_MAGICALFORTRESS)) {
					*str += "At this level, the mage may engrave runes of "
						"warding upon a Magical Fortress, which grants "
						"the inhabitants an Energy Shield and Spirit "
						"Shield at level 5.";
				}
			}
			break;
		case S_CONSTRUCT_GATE:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
			*str += "A mage with the Construct Gate skill may construct a "
				"Gate in a region. The mage has a 20 percent times his "
				"skill level chance of success, and the attempt costs 1000 "
				"silver. To use this spell, the mage should issue the order "
				"CAST Construct_Gate.";
			break;
		case S_ENCHANT_WEAPONS:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
			if (ITEM_DISABLED(I_ENCHANTEDSWORD)) break;
			*str += "The Enchant Weapons skill enables the mage to create enchanted weapons. Because only wood and iron can be modified by the mage, only wooden or iron weapons can be enchanted. The mage should "
				"issue the order CAST Enchant_Weapons <weapon> to cast this spell, where <weapon> is the desired item.";
			break;
		case S_ENCHANT_ARMOR:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
			if (ITEM_DISABLED(I_ENCHANTEDPLATEARMOR)) break;
			*str += "The Enchant Armor skill enables the mage to create enchanted armor. Because mithril and admantium are not influenced by magic, he can only enchant armor out of normal ressources like iron or hide. The mage should issue the "
				"order CAST Enchant_Armor <armor>to cast this spell, where <armour> is the desired item.";
			break;
		case S_CONSTRUCT_PORTAL:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
			if (ITEM_DISABLED(I_PORTAL)) break;
			*str += "A mage with the Construct Portal skill may construct a "
				"Portal";
			if (SKILL_ENABLED(S_PORTAL_LORE)) {
				*str += " for use with the Portal Lore skill";
			}
			*str += ". The mage has a 20 percent times his skill level "
				"chance of creating a Portal, and the attempt costs 600 "
				"silver. To use this spell, CAST Construct_Portal.";
			break;
		case S_MANIPULATE:
			if (!Globals->APPRENTICES_EXIST) break;
			if (level > 1) break;
			*str += "A unit with this skill becomes an apprentice mage. "
				"While apprentices cannot cast spells directly, they can "
				"use magic items normally only usable by mages. Continued "
				"study of this skill gives no further advantages.";
			break;
		case S_WEAPONCRAFT:
			if (level > 1) break;
			*str += "The weaponcraft skill is an advanced version of the "
				"smithing skill. It allows the production of the most advanced weapons.";
			break;
		case S_ARMORCRAFT:
			if (level > 1) break;
			*str += "The armorcraft skill is an advanced version of the "
				"smithing skill. It allows the production of the most advanced armors.";
			break;
		case S_GEMCUTTING:
			if (level > 1) break;
			*str += "This skill enables a unit to fashion higher quality "
				"gems from lower quality ones.";
			break;
		case S_SCOUTING:
			if (level > 1) break;
			*str += "This skill enables a unit to pass behind enemy lines, providing the "
				"unit's Scouting skill is equal to or greater than the enemy's Observation "
				"skill. A unit with this skill will be able to see certain buildings "
				"that may normally be hidden. "
				"This skill will also decrease the amount of maintenance the unit "
				"must pay by 2 silver per skill level per man.";
		        *str += " Knowledge of this skill excludes knowledge of stealth.";
			break;
		case S_COOKING:
			if (level > 1) break;
			*str += "This skill deals with creating provisions from basic "
					"foodstuffs.  A skilled cook can feed many more people "
					"than a farmer alone.";
			break;
		case S_CREATE_FOOD:
			/* XXX -- This should be cleaner somehow. */
			if (level > 1) break;
			if (ITEM_DISABLED(I_FOOD)) break;
			*str += "A mage with the Create Food skill may magically "
				"create food. A mage may create ";
			*str += ItemDefs[I_FOOD].mOut;
			*str += " times his skill level provisions per turn. The "
				"mage should issue the order CAST Create_Food to cast "
				"this spell.";
			break;
		case S_ENCHANT_RINGS:
			if (level > 1) break;
			*str += "A mage with the Enchant Rings skill may create "
					"magic rings that increase a certain skill (note that a unit must possess "
					"one ring for each man to gain this bonus). To use this spell, the mage "
					"should CAST Enchant_Rings <ring>, where <ring> is the name of the magic "
					"ring the caster wants to create.";
			break;
		case S_ENCHANT_AMULETS:
			if (level > 1) break;
			*str += "A mage with the Enchant Amulet skill may create "
					"magic amulets that increase a certain skill (note that a unit must possess "
					"one ring for each man to gain this bonus). To use this spell, the mage "
					"should CAST Enchant_Amulets <amulet>, where <amulet> is the name of the "
					"magic amulet the caster wants to create.";
			break;
		case S_CREATE_CHARMS:
			if (level > 1) break;
			*str += "A mage with the Create Charms skill may create "
					"magic charms that increase a certain skill (note that a unit must possess "
					"one charm for each man to gain this bonus). To use this spell, the mage "
					"should CAST Create_Charms <charm>, where <charm> is the name of the magic "
					"charm the caster wants to create.";
			break;
	}

	AString temp;
	AString temp1;
	AString temp2;
	AString temp3;
	int comma = 0;
	int comma1 = 0;
	int comma2 = 0;
	int last = -1;
	int last1 = -1;
	int last2 = -1;
	unsigned int c;
	int i;

	// If this is a combat spell, note it.
	if (level == 1 && (SkillDefs[skill].flags & SkillType::COMBAT)) {
		*str += AString(" A mage with this skill can cast ") +
			ShowSpecial(SkillDefs[skill].special, level, 0, 0);
		*str += " In order to use this spell in combat, the mage should use "
			"the COMBAT order to set it as his combat spell.";
	}

	// production and ability to see items
	temp += "A unit with this skill is able to determine if a region "
		"contains ";
	temp1 += "A unit with this skill may PRODUCE ";
	temp2 += "A unit with this skill may create ";
	for(i = NITEMS - 1; i >= 0; i--) {
		if (ITEM_DISABLED(i)) continue;
		if (ItemDefs[i].mSkill==skill &&
				ItemDefs[i].mLevel==level &&
				last2==-1) {
			int canmagic = 1;
			for(c = 0; c < sizeof(ItemDefs[i].mInput)/sizeof(Materials); c++) {
				if (ItemDefs[i].mInput[c].item == -1) continue;
				if (ITEM_DISABLED(ItemDefs[i].mInput[c].item)) {
					canmagic = 0;
				}
			}
			if (canmagic) {
				last2 = i;
			}
		}
		if (ItemDefs[i].pSkill == skill && ItemDefs[i].pLevel == level) {
			int canmake = 1;
			int resource = 1;
			for(c = 0; c < sizeof(ItemDefs[i].pInput)/sizeof(Materials); c++) {
				if (ItemDefs[i].pInput[c].item == -1) continue;
				resource = 0;
				if (ITEM_DISABLED(ItemDefs[i].pInput[c].item)) {
					canmake = 0;
				}
			}
			if (canmake && last1 == -1) {
				last1 = i;
			}
			if (resource && (ItemDefs[i].type & IT_ADVANCED) && last == -1) {
				last = i;
			}
		}

	}

	for(i = 0; i < NITEMS; i++) {
		if (ITEM_DISABLED(i)) continue;
		int illusion = ((ItemDefs[i].type & IT_MONSTER) &&
				(ItemDefs[i].index == MONSTER_ILLUSION));
		if (ItemDefs[i].mSkill == skill && ItemDefs[i].mLevel == level) {
			int canmagic = 1;
			for(c = 0; c < sizeof(ItemDefs[i].mInput)/sizeof(Materials); c++) {
				if (ItemDefs[i].mInput[c].item == -1) continue;
				if (ITEM_DISABLED(ItemDefs[i].mInput[c].item)) {
					canmagic = 0;
				}
			}
			if (canmagic) {
				if (comma2) {
					if (last2 == i) {
						if (comma2 > 1) temp2 += ",";
						temp2 += " and ";
					} else {
						temp2 += ", ";
					}
				}
				comma2++;
				temp2 += AString(illusion?"illusory ":"") + ItemDefs[i].names;
				if (f) {
					f->DiscoverItem(i, 1, 1);
				}
			}
		}
		if (ItemDefs[i].pSkill == skill && ItemDefs[i].pLevel == level) {
			int canmake = 1;
			int resource = 1;
			for(c = 0; c < sizeof(ItemDefs[i].pInput)/sizeof(Materials); c++) {
				if (ItemDefs[i].pInput[c].item == -1) continue;
				resource = 0;
				if (ITEM_DISABLED(ItemDefs[i].pInput[c].item)) {
					canmake = 0;
				}
			}
			if (canmake) {
				if (comma1) {
					if (last1 == i) {
						if (comma1 > 1) temp1 += ",";
						temp1 += " and ";
					} else {
						temp1 += ", ";
					}
				}
				comma1++;
				temp1 += AString(illusion?"illusory ":"") + ItemDefs[i].names;
				if (f) {
					f->DiscoverItem(i, 1, 1);
				}
			}
			if (resource && (ItemDefs[i].type & IT_ADVANCED)) {
				if (comma) {
					if (last == i) {
						if (comma > 1) temp += ",";
						temp += " and ";
					} else {
						temp += ", ";
					}
				}
				comma++;
				temp += AString(illusion?"illusory ":"") + ItemDefs[i].names;
			}
		}
	}
	if (comma1) {
		if (!(*str == "")) *str += " ";
		*str += temp1 + ".";
	}
	if (comma) {
		if (!(*str == "")) *str += " ";
		*str += temp + ".";
	}
	if (comma2) {
		if (!(*str == "")) *str += " ";
		*str += temp2 + " via magic.";
	}

	// Spell failure
	if (SkillDefs[skill].failChance > 0) {
		temp3 = " The chance that this spell will fail is equal to ";
		temp3 += AString(SkillDefs[skill].failChance) + " ";

		if (SkillDefs[skill].failType == SkillType::FAIL_MINUS_SKILL ) {
			temp3 += "minus the caster's skill level ";
		} else if (SkillDefs[skill].failType == SkillType::FAIL_DIVIDE_SKILL ) {
			temp3 += "divided by the caster's skill level ";
		} else if( SkillDefs[skill].failType == SkillType::FAIL_MINUS_SKILL_SQUARED ) {
			temp3 += "minus the caster's skill level squared ";
		} else if( SkillDefs[skill].failType == SkillType::FAIL_MINUS_SKILL_TIMES_TEN ) {
			temp3 += "minus ten times the caster's skill level ";
		} else if( SkillDefs[skill].failType == SkillType::FAIL_MINUS_SKILL_TIMES_TWENTY ) {
			temp3 += "minus twenty times the caster's skill level ";
		}		
		temp3 += "percent, per casting.";
	}

	// Buildings
	comma = 0;
	temp = "A unit with this skill may BUILD the following structures: ";
	for(i = 0; i < NOBJECTS; i++) {
		if (OBJECT_DISABLED(i)) continue;
		if (ObjectDefs[i].skill == skill && ObjectDefs[i].level == level) {
			if (comma) temp += ", ";
			comma = 1;
			temp += ObjectDefs[i].name;
			if (f) {
				f->objectshows.Add(ObjectDescription(i));
			}
		}
	}
	if (comma) {
		if (!(*str == "")) *str += " ";
		*str += temp + ".";
	}

	// Required building
	if( SkillDefs[skill].building != -1 ) {
		*str += AString(" This skill must be studied inside a ") +
				ObjectDefs[SkillDefs[skill].building].name + ". ";
	}

	// Required skills
	last = -1;
	if (level == 1) {
		comma = 0;
		int found = 0;
		temp = "This skill requires ";
		for(c=0; c<sizeof(SkillDefs[skill].depends)/sizeof(SkillDepend); c++) {
			if (SkillDefs[skill].depends[c].skill == -1) continue;
			if (SKILL_DISABLED(SkillDefs[skill].depends[c].skill)) continue;
			found = 1;
			if (last == -1) {
				last = c;
				continue;
			}
			temp += SkillStrs(SkillDefs[skill].depends[last].skill) + " " +
				SkillDefs[skill].depends[last].level + ", ";
			last = c;
			comma++;
		}
		if (found) {
			if (comma) {
				temp += "and ";
			}

			temp += SkillStrs(SkillDefs[skill].depends[last].skill) + " " +
					SkillDefs[skill].depends[last].level;

			if (!(*str == "")) *str += " ";
			*str += temp + " to begin to study.";
		}
	}

	if (level == 1) {
		if (SkillDefs[skill].cost) {
			if (!(*str == "")) *str += " ";
			*str += AString("This skill costs ") + SkillDefs[skill].cost +
				" silver per month of study.";
		}
		if (SkillDefs[skill].flags & SkillType::SLOWSTUDY) {
			if (!(*str == "")) *str += " ";
			*str += "This skill is studied at one half the normal speed.";
		}
	}

	temp1 = SkillStrs(skill) + " " + level + ": ";
	if (*str == "") {
		*str = temp1 + "No skill report.";
	} else {
		*str = temp1 + *str + temp3;
	}

	return str;
}
