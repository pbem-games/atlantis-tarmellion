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
// Date			Person			Comments
// ----			------			--------
// 2001/Feb/22	Joseph Traub	Moved the Item and such definitions out
//								into gamedata.cpp
//
#include "gamedata.h"
#include "gamedefs.h"

//
// Define the various globals for this game.
//
// If you change any of these, it is incumbent on you, the GM to change
// the html file containing the rules to correctly reflect the changes!
//

static int am[] = { 0, 15, 30, 45, 60, 75, 90 };
int *allowedMages = am;
int allowedMagesSize = sizeof(am) / sizeof(am[0]);

static int aa[] = { 0, 2, 4, 6, 8, 10, 12 };
int *allowedApprentices = aa;
int allowedApprenticesSize = sizeof(aa) / sizeof(aa[0]);

static int aw[] = { 0, 15, 30, 45, 60, 75 };
int *allowedTaxes = aw;
int allowedTaxesSize = sizeof(aw) / sizeof(aw[0]);

static int at[] = { 0, 18, 36, 54, 72, 90 };
int *allowedTrades = at;
int allowedTradesSize = sizeof(at) / sizeof(at[0]);

static int uw[] = { 6, 6, 6, 0 };
//static int uw[] = { 6 };
int *UnderworldCities = uw;
int UnderworldCityLevels = sizeof(uw) / sizeof(uw[0]);

static int ud[] = { 6, 6, 6, 6 };
//static int ud[] = { 6 };
int *UnderdeepCities = ud;
int UnderdeepCityLevels = sizeof(ud) / sizeof(ud[0]);

static int su[] = { 23 };
int *SurfaceCities = su;
int SurfaceCityLevels = sizeof(su) / sizeof(su[0]);

static GameDefs g = {
	"Ceran",				// RULESET_NAME
	MAKE_ATL_VER(2, 0, 4), // RULESET_VERSION

	2, /* FOOT_SPEED */
	4, /* HORSE_SPEED */
	4, /* SHIP_SPEED */
	6, /* FLY_SPEED */
	8, /* MAX_SPEED */

	10, /* STUDENTS_PER_TEACHER */

	10, /* MAINTENANCE_COST */
	20, /* LEADER_COST */

	0,  /* MAINTAINENCE_MULTIPLIER */
	GameDefs::MULT_NONE, /* MULTIPLIER_USE */

	33, /* STARVE_PERCENT */
	GameDefs::STARVE_NONE, /* SKILL_STARVATION */

	5020, /* START_MONEY */
	5, /* WORK_FRACTION */
	20, /* ENTERTAIN_FRACTION */
	20, /* ENTERTAIN_INCOME */

	50, /* TAX_INCOME */

	5, /* HEALS_PER_MAN */

	20, /* GUARD_REGEN */ /* percent */
	40, /* CITY_GUARD */
	50, /* GUARD_MONEY */
	4000, /* CITY_POP */

	25, /* WMON_FREQUENCY */
	25, /* LAIR_FREQUENCY */

	75, /* FACTION_POINTS */

	60, /* TIMES_REWARD */

	1, // TOWNS_EXIST
	GameDefs::RACIAL_LEADERS, // LEADERS_EXIST

	3, // SKILL_LIMIT_NONLEADERS
	1, // MAGE_NONLEADERS
	1, // RACES_EXIST
	1, // GATES_EXIST
	1, // FOOD_ITEMS_EXIST
	1, // CITY_MONSTERS_EXIST
	1, // WANDERING_MONSTERS_EXIST
	1, // LAIR_MONSTERS_EXIST
	1, // WEATHER_EXISTS
	1, // OPEN_ENDED
	1, // NEXUS_EXISTS
	0, // CONQUEST_GAME

	1, // RANDOM_ECONOMY

	1, // VARIABLE_ECONOMY

	50, // CITY_MARKET_NORMAL_AMT
	20, // CITY_MARKET_ADVANCED_AMT
	50, // CITY_MARKET_TRADE_AMT
	20, // CITY_MARKET_MAGIC_AMT
	0,  // MORE_PROFITABLE_TRADE_GOODS

	50,	// BASE_MAN_COST

	0, // LASTORDERS_MAINTAINED_BY_SCRIPTS

	10, // MAX_INACTIVE_TURNS

	0, // EASIER_UNDERWORLD

	1, // DEFAULT_WORK_ORDER

	GameDefs::FACLIM_FACTION_TYPES, // FACTION_LIMIT_TYPE

	GameDefs::WFLIGHT_MUST_LAND,	// FLIGHT_OVER_WATER

	0,   // SAFE_START_CITIES
	10000, // AMT_START_CITY_GUARDS
	1,   // START_CITY_GUARDS_PLATE
	0,   // START_CITY_MAGES
	3,   // START_CITY_TACTICS

	0,   // APPRENTICES_EXIST

	"Ceran", // WORLD_NAME

	1,  // NEXUS_GATE_OUT
	1,  // NEXUS_IS_CITY

	1,	// BATTLE_FACTION_INFO
	1,	// ALLOW_WITHDRAW
	1000,	// CITY_RENAME_COST
	0,	// TAX_PILLAGE_MONTH_LONG
	1,	// MULTI_HEX_NEXUS
//	1,	// UNDERWORLD_LEVELS
//	1,	// UNDERDEEP_LEVELS
	4,	// UNDERWORLD_LEVELS
	4,	// UNDERDEEP_LEVELS
	1,	// ABYSS_LEVEL
	120,	// TOWN_PROBABILITY
	20,	// TOWN_SPREAD
	80,	// TOWNS_NOT_ADJACENT
	2,	// LESS_ARCTIC_TOWNS
	10,	// ARCHIPELAGO
	0,	// LAKES_EXIST
	GameDefs::DESERT_ONLY, // LAKE_WAGE_EFFECT
	1,	// LAKESIDE_IS_COASTAL
	100,	// ODD_TERRAIN
	0,	// IMPROVED_FARSIGHT
	1,	// GM_REPORT
	1,	// DECAY
	0,	// LIMITED_MAGES_PER_BUILDING
	GameDefs::REPORT_SHOW_LITTLE, // TRANSIT_REPORT
	1,  // MARKETS_SHOW_ADVANCED_ITEMS
	GameDefs::PREPARE_NORMAL,	// USE_PREPARE_COMMAND
	0,	// MONSTER_ADVANCE_MIN_PERCENT
	40,	// MONSTER_ADVANCE_HOSTILE_PERCENT
	0,	// HAVE_EMAIL_SPECIAL_COMMANDS
	0,	// START_CITIES_START_UNLIMITED
	1,	// PROPORTIONAL_AMTS_USAGE
	1,  // USE_WEAPON_ARMOR_COMMAND
	2,  // MONSTER_NO_SPOILS
	2,  // MONSTER_SPOILS_RECOVERY
	0,  // MAX_ASSASSIN_FREE_ATTACKS
	1,  // RELEASE_MONSTERS
	1,  // CHECK_MONSTER_CONTROL_MID_TURN
	1,  // DETECT_GATE_NUMBERS
	GameDefs::ARMY_ROUT_HITS_FIGURE,  // ARMY_ROUT
	1,	// FULL_TRUESEEING_BONUS
	1,	// FULL_INVIS_ON_SELF
	1,	// MONSTER_BATTLE_REGEN
	GameDefs::TAX_ME, // WHO_CAN_TAX
	10,	// SKILL_PRACTISE_AMOUNT
	0,	// UPKEEP_MINIMUM_FOOD
	-1,	// UPKEEP_MAXIMUM_FOOD
	10,	// UPKEEP_FOOD_VALUE
	1,	// PREVENT_SAIL_THROUGH
	0,	// ALLOW_TRIVIAL_PORTAGE
	0,      // AUTOMATIC_FOOD
        4,	// CITY_TAX_COST;
        3,	// TOWN_TAX_COST;
        2,	// VILLAGE_TAX_COST;
        2,	// CITY_TRADE_COST;
        2,	// TOWN_TRADE_COST;
        1,	// VILLAGE_TRADE_COST;
	GameDefs::FP_EQU,	// FP_DISTRIBUTION
	1,	// FP_FACTOR
	1,	// ACN_IS_STARTING_CITY

};

GameDefs * Globals = &g;
