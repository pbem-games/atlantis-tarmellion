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
#ifndef ITEMS_CLASS
#define ITEMS_CLASS

class Item;
class ItemType;

#include "fileio.h"
#include "gamedefs.h"
#include "alist.h"
#include "astring.h"

enum {
	ATTACK_COMBAT,
	ATTACK_ENERGY,
	ATTACK_SPIRIT,
	ATTACK_WEATHER,
	ATTACK_RIDING,
	ATTACK_RANGED,
	NUM_ATTACK_TYPES
};

enum {
	IT_NORMAL = 0x0001,
	IT_ADVANCED = 0x0002,
	IT_TRADE = 0x0004,
	IT_MAN = 0x0008,
	IT_MONSTER = 0x0010,
	IT_MAGIC = 0x0020,
	IT_WEAPON = 0x0040,
	IT_ARMOR = 0x0080,
	IT_MOUNT = 0x0100,
	IT_BATTLE = 0x0200,
	IT_SPECIAL = 0x0400,
	IT_TOOL = 0x0800,
	IT_FOOD = 0x1000,
	IT_ABSTRACT = 0x2000,
	IT_QUEST = 0x4000,
	IT_MONKEEP = 0x8000,
};

struct Materials {
	int item;
	int amt;
};

class ItemType {
	public:
		char *name;
		char *names;
		char *abr;

		enum {
			CANTGIVE = 0x1,
			DISABLED = 0x2,
			NOMARKET = 0x4,
			// This item requires ANY of its inputs, not ALL of them
			ORINPUTS = 0x8,
			// A number of items are produced equal to the producer's
			// skill, based on a fixed number of inputs
			SKILLOUT = 0x10,
			// this item is only associated with evil.
			EVIL = 0x20,
			// this item is only associated with good.
			GOOD = 0x40,
			// this item cannot be seen by oponents (used to be weight zero.)
			HIDDEN = 0x80,
		};
		int flags;
		enum {
		  CAN_CONSUME = 0x1,
		  // Can be eaten for maintance, pValue gives priority.
		  CAN_HEAL = 0x2,
		  // Can be used to heal, pValue gives percentage chance.
		  STOP_THEFT = 0x4,
		  // Item stops theft.
		  STOP_ASSASINATE = 0x8,
		  // Item stops assasination attempts.
		  OBSE_BONUS = 0x10,
		  // pValue is added to observation.
		  STEA_BONUS = 0x20,
		  // pValue is added to stealth
		};
		int attributes;
		int pValue;

		int pSkill; // production skill
		int pLevel; // production skill level
		int pMonths; // Man months required for production
		int pOut; // How many of the item we get
		Materials pInput[4];

		int mSkill; // magical production skill
		int mLevel; // magical production skill level
		int mOut; // How many of the item are conjured
		Materials mInput[4];

		int weight;
		int type;
		int baseprice;
		int combat;
		int index;
		int battleindex;

		int walk;
		int ride;
		int fly;
		int swim;

		int hitchItem;
		int hitchwalk;
		// LLS
		int mult_item;
		int mult_val;
		int requiredstructure;
		int byproducts[3];
};

extern ItemType * ItemDefs;

class ManType {
	public:

	enum {
		NONE    = 0x00,
		LEADER  = 0x01,
		POPULUS = 0x02,
		SCARCE  = 0x04,
	};

	int flags;
	int minority; // This is used to point to another race which my ocuppy the hex.
	// {guard weapon,guard armor,(guard shield),guard equiptment,starting equiptment, better weapon, better armor}
	// guarditems[0..1] are for normal city guards,
	// guarditems[4..6] are for starting city guard,
	// guarditems[2..3] are for both.
	int guarditems[7];
	int speciallevel[20];
	int defaultlevel;
	int defaultmagiclevel;
	int specialskills[20];
	int compatibleraces[5];
	int incompatibleraces[5];
	enum {
		M_NONE		= 0x0000,
		M_HUMAN 	= 0x0001,
		M_ELF   	= 0x0002,
		M_DWARF		= 0x0004,
		M_GOBLINOID	= 0x0008,
		M_BEASTMAN	= 0x0010,
		M_GNOME		= 0x0020,
		M_FAIRY		= 0x0040,
		M_TITAN		= 0x0080,
		M_LIZARDMAN	= 0x0100,
		M_HOBBIT	= 0x0200,
		M_ALL		= 0xFFFF,
	};
	int species;
	int alternaterace;
};

extern ManType * ManDefs;

class MonType {
	public:
		int attackLevel;
		int defense[NUM_ATTACK_TYPES];

		int numAttacks;
		int hits;
		int regen;

		int tactics;
		int stealth;
		int obs;

		int special;
		int specialLevel;

		int silver;
		int spoiltype;
		int spoilitems[5]; /* overrides spoiltype */
		int hostile; /* Percent */
		int number;
		char *name;
};

extern MonType * MonDefs;

enum {
	SLASHING,		// e.g. sword attack (This is default)
	PIERCING,		// e.g. spear or arrow attack
	CRUSHING,		// e.g. mace attack
	CLEAVING,		// e.g. axe attack
	ARMORPIERCING,	// e.g. crossbow double bow
	MAGIC_ENERGY,	// e.g. fire, dragon breath
	MAGIC_SPIRIT,	// e.g. black wind
	MAGIC_WEATHER,	// e.g. tornado
	NUM_WEAPON_CLASSES
};


class WeaponType {
	public:
		enum {
			NEEDSKILL = 0x1, // No bonus or use unless skilled
			ALWAYSREADY = 0x2, // Ignore the 50% chance to attack
			NODEFENSE = 0x4, // No combat defense against this weapon
			NOFOOT = 0x8, // Weapon cannot be used on foot (e.g. lance)
			NOMOUNT = 0x10, // Weapon cannot be used mounted (e.g. pike)
			SHORT = 0x20, // Short melee weapon (e.g. shortsword, hatchet)
			LONG = 0x40, // Long melee weapon (e.g. lance, pike)
			RANGED = 0x80, // Missile weapon
			NOATTACKERSKILL = 0x100, // Attacker gets no combat/skill defense.
			RIDINGBONUS = 0x200, // Unit gets riding bonus on att and def.
			RIDINGBONUSDEFENSE = 0x400, // Unit gets riding bonus on def only.
			USEINASSASSINATE = 0x800, // weapon can be used in assination.
			TWOHANDED = 0x1000, // Unit cannot use a shield with weapon.
		};
		int flags;

		int baseSkill;
		int orSkill;

		int weapClass;
		int attackType;
		//
		// For numAttacks:
		// - A positive number is the number of attacks per round.
		// - A negative number is the number of rounds per attack.
		// - NUM_ATTACKS_HALF_SKILL indicates that the weapon gives as many
		//   attacks as the skill of the user divided by 2, rounded up.
		// - NUM_ATTACKS_HALF_SKILL+1 indicates that the weapon gives an extra
		//   attack above that, etc.
		// - NUM_ATTACKS_SKILL indicates the the weapon gives as many attacks
		//   as the skill of the user.
		// - NUM_ATTACKS_SKILL+1 indicates the the weapon gives as many
		//   attacks as the skill of the user + 1, etc.
		//
		enum {
			NUM_ATTACKS_HALF_SKILL = 50,
			NUM_ATTACKS_SKILL = 100,
		};
		int numAttacks;

		int attackBonus;
		int defenseBonus;
		int mountBonus;
};

extern WeaponType *WeaponDefs;

class ArmorType {
	public:
		enum {
			USEINASSASSINATE = 0x1,
			SHIELD = 0x2,
		};

		int flags;
		//
		// Against attacks, the chance of the armor protecting the wearer
		// is: <type>Chance / from
		//
		int from;
		int saves[NUM_WEAPON_CLASSES];
};

extern ArmorType *ArmorDefs;

class MountType {
	public:
		//
		// This is the skill needed to use this mount.
		//
		int skill;

		//
		// This is the minimum bonus (and minimal skill level) for this mount.
		//
		int minBonus;

		//
		// This is the maximum bonus this mount will grant.
		//
		int maxBonus;

		//
		// This is the max bonus a mount will grant if it can normally fly
		// but the region doesn't allow flying mounts
		int maxHamperedBonus;

		// If the mount has a special effect it generates when ridden in
		// combat
		int mountSpecial;
		int specialLev;
};

extern MountType *MountDefs;

class BattleItemType {
	public:
		enum {
			MAGEONLY = 0x1,
			SPECIAL = 0x2,
			SHIELD = 0x4,
		};

		int flags;
		int itemNum;
		int index;
		int skillLevel;
};

extern BattleItemType *BattleItemDefs;
AString AttType(int atype);
int ParseGiveableItem(AString *);
int ParseAllItems(AString *);
int ParseEnabledItem(AString *);
int ParseBattleItem(int);

AString ItemString(int type,int num);
AString *ItemDescription(int item, int full);

int IsSoldier(int);

class Item : public AListElem {
	public:
		Item();
		~Item();

		void Readin(Ainfile *);
		void Writeout(Aoutfile *);

		AString Report(int);

		int type;
		int num;
		int selling;
};

class ItemList : public AList {
	public:
		void Readin(Ainfile *);
		void Writeout(Aoutfile *);

		AString Report(int,int,int);
		AString BattleReport();

		int Weight();
		int GetNum(int);
		void SetNum(int,int); /* type, number */
		int CanSell(int);
		void Selling(int, int); /* type, number */
};

extern AString ShowSpecial(int special, int level, int expandLevel,
		int fromItem);

#endif
