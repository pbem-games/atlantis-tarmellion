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
#include <gamedata.h>
#include <stdio.h>
#ifdef WIN32
#include <memory.h>
#endif
#include <string.h>

#define LINE_WIDTH         70
#define MAP_WIDTH          23
#define TMPL_MAP_WIDTH     20
#define TMPL_MAP_OFS        1
#define FILL_SIZE           6
#define TEMPLATE_MAX_LINES 13

static void TrimWrite(Areport *f, char *buffer);

static char *TemplateMap[] = {
//12345678901234567890
	"        ____        ",   // 1
	"nw     /    \\     ne",  // 2
	"  ____/      \\____  ",  // 3
	" /    \\      /    \\ ", // 4
	"/      \\____/      \\", // 5
	"\\      /    \\      /", // 6
	" \\____/      \\____/ ", // 7
	" /    \\      /    \\ ", // 8
	"/      \\____/      \\", // 9
	"\\      /    \\      /", // 10
	" \\____/      \\____/ ", // 11
	"      \\      /      ",  // 12
	"sw     \\____/     se"   // 13
};

static int dircrd[] = {
	// X    Y
	 8-1,  7-1,   // center
	 8-1,  3-1,   // N
	14-1,  5-1,   // NE
	14-1,  9-1,   // SE
	 8-1, 11-1,   // S
	 2-1,  9-1,   // SW
	 2-1,  5-1    // NW
};


static char *terfill[] = {
	// block
	" #### ",
	" #### ",

	// ocean
	"  ~ ~ ",
	" ~ ~  ",

	// plain
	"      ",
	"      ",

	// forest
	"  ^ ^ ",
	" ^ ^  ",

	// mountain
	" /\\/\\ ",
	"/  \\ \\",

	// swamp
	"  v v ",
	" v v  ",

	// jungle
	"  @ @ ",
	" @ @  ",

	// desert
	"  . . ",
	" . .  ",

	// tundra
	"  ' ' ",
	" ' '  ",

	// cavern
	"  . . ",
	" . .  ",

	// underforest
	"  ^ ^ ",
	" ^ ^  ",

	// tunnels
	"      ",
	"      ",

	// nexus
	" !!!! ",
	" !!!! ",

	// volcano
	" /\\/\\ ",
	"/ *\\*\\",

	// lake
	" ~    ",
	"    ~ ",

	// Island Plain
	"      ",
	"      ",

	// Island swamp
	"  v v ",
	" v v  ",

	// Island mountain
	" /\\/\\ ",
	"/  \\ \\",

	// Tarmellion ocean
	"  ~ ~ ",
	" ~ ~  ",

	// Tarmellion plain
	"      ",
	"      ",

	// Tarmellion plain
	"      ",
	"      ",

	// Tarmellion plain
	"      ",
	"      ",

	// Tarmellion lake
	" ~    ",
	"    ~ ",

	// Tarmellion lake
	" ~    ",
	"    ~ ",

	// Tarmellion lake
	" ~    ",
	"    ~ ",

	// Tarmellion forest
	"  ^ ^ ",
	" ^ ^  ",

	// Tarmellion forest
	"  ^ ^ ",
	" ^ ^  ",

	// Tarmellion forest
	"  ^ ^ ",
	" ^ ^  ",

	// Tarmellion mystforest
	" * * *",
	" | | |",

	// Tarmellion mountain
	" /\\/\\ ",
	"/  \\ \\",

	// Tarmellion mountain
	" /\\/\\ ",
	"/  \\ \\",

	// Tarmellion mountain
	" /\\/\\ ",
	"/  \\ \\",

	// Tarmellion hill
	"  * * ",
	" * *  ",

	// Tarmellion hill
	"  * * ",
	" * *  ",

	// Tarmellion hill
	"  * * ",
	" * *  ",

	// Tarmellion swamp
	"  v v ",
	" v v  ",

	// Tarmellion swamp
	"  v v ",
	" v v  ",

	// Tarmellion swamp
	"  v v ",
	" v v  ",

	// Tarmellion jungle
	"  @ @ ",
	" @ @  ",

	// Tarmellion jungle
	"  @ @ ",
	" @ @  ",

	// Tarmellion jungle
	"  @ @ ",
	" @ @  ",

	// Tarmellion desert
	"  . . ",
	" . .  ",

	// Tarmellion desert
	"  . . ",
	" . .  ",

	// Tarmellion desert
	"  . . ",
	" . .  ",

	// Tarmellion volcano
	" /\\/\\ ",
	"/ *\\*\\",

	// Tarmellion tundra
	"  ' ' ",
	" ' '  ",

	// Tarmellion tundra
	"  ' ' ",
	" ' '  ",

	// Tarmellion tundra
	"  ' ' ",
	" ' '  ",

	// Tarmellion cavern
	"  . . ",
	" . .  ",

	// Tarmellion cavern
	"  . . ",
	" . .  ",

	// Tarmellion cavern
	"  . . ",
	" . .  ",

	// Tarmellion underforest
	"  ^ ^ ",
	" ^ ^  ",

	// Tarmellion underforest
	"  ^ ^ ",
	" ^ ^  ",

	// Tarmellion underforest
	"  ^ ^ ",
	" ^ ^  ",

	// Tarmellion tunnels
	"      ",
	"      ",

	// Tarmellion tunnels
	"      ",
	"      ",

	// Tarmellion tunnels
	"      ",
	"      ",

	// Tarmellion grotto
	" ~ . ~",
	" . ~ .",

	// Tarmellion plain
	"      ",
	"      ",

	// Tarmellion plain
	"      ",
	"      ",

	// Tarmellion plain
	"      ",
	"      ",

	// Tarmellion lake
	" ~    ",
	"    ~ ",

	// Tarmellion lake
	" ~    ",
	"    ~ ",

	// Tarmellion lake
	" ~    ",
	"    ~ ",

	// Tarmellion forest
	"  ^ ^ ",
	" ^ ^  ",

	// Tarmellion forest
	"  ^ ^ ",
	" ^ ^  ",

	// Tarmellion forest
	"  ^ ^ ",
	" ^ ^  ",

	// Tarmellion mystforest
	" * * *",
	" | | |",

	// Tarmellion mountain
	" /\\/\\ ",
	"/  \\ \\",

	// Tarmellion mountain
	" /\\/\\ ",
	"/  \\ \\",

	// Tarmellion mountain
	" /\\/\\ ",
	"/  \\ \\",

	// Tarmellion hill
	"  * * ",
	" * *  ",

	// Tarmellion hill
	"  * * ",
	" * *  ",

	// Tarmellion hill
	"  * * ",
	" * *  ",

	// Tarmellion swamp
	"  v v ",
	" v v  ",

	// Tarmellion swamp
	"  v v ",
	" v v  ",

	// Tarmellion swamp
	"  v v ",
	" v v  ",

	// Tarmellion jungle
	"  @ @ ",
	" @ @  ",

	// Tarmellion jungle
	"  @ @ ",
	" @ @  ",

	// Tarmellion jungle
	"  @ @ ",
	" @ @  ",

	// Tarmellion desert
	"  . . ",
	" . .  ",

	// Tarmellion desert
	"  . . ",
	" . .  ",

	// Tarmellion desert
	"  . . ",
	" . .  ",

	// Tarmellion volcano
	" /\\/\\ ",
	"/ *\\*\\",

	// Tarmellion tundra
	"  ' ' ",
	" ' '  ",

	// Tarmellion tundra
	"  ' ' ",
	" ' '  ",

	// Tarmellion tundra
	"  ' ' ",
	" ' '  ",

	// Tarmellion cavern
	"  . . ",
	" . .  ",

	// Tarmellion cavern
	"  . . ",
	" . .  ",

	// Tarmellion cavern
	"  . . ",
	" . .  ",

	// Tarmellion deepforest
	"  ^ ^ ",
	" ^ ^  ",

	// Tarmellion deepforest
	"  ^ ^ ",
	" ^ ^  ",

	// Tarmellion deepforest
	"  ^ ^ ",
	" ^ ^  ",

	// Tarmellion tunnels
	"      ",
	"      ",

	// Tarmellion tunnels
	"      ",
	"      ",

	// Tarmellion tunnels
	"      ",
	"      ",

	// Tarmellion grotto
	" ~ . ~",
	" . ~ .",

	// Ceran ocean
	"  ~ ~ ",
	" ~ ~  ",

	// Ceran ocean
	"  ~ ~ ",
	" ~ ~  ",

	// Ceran plain
	"      ",
	"      ",

	// Ceran plain
	"      ",
	"      ",

	// Ceran plain
	"      ",
	"      ",

	// Ceran plain
	"      ",
	"      ",

	// Ceran grassland
	" '` '`",
	"'` '` ",

	// Ceran grassland
	" '` '`",
	"'` '` ",

	// Ceran grassland
	" '` '`",
	"'` '` ",

	// Ceran grassland
	" '` '`",
	"'` '` ",

	// Ceran lake
	" ~    ",
	"    ~ ",

	// Ceran lake
	" ~    ",
	"    ~ ",

	// Ceran lake
	" ~    ",
	"    ~ ",

	// Ceran forest
	"  ^ ^ ",
	" ^ ^  ",

	// Ceran forest
	"  ^ ^ ",
	" ^ ^  ",

	// Ceran forest
	"  ^ ^ ",
	" ^ ^  ",

	// Ceran forest
	"  ^ ^ ",
	" ^ ^  ",

	// Ceran mystforest
	" * * *",
	" | | |",

	// Ceran mystforest
	" * * *",
	" | | |",

	// Ceran mountain
	" /\\/\\ ",
	"/  \\ \\",

	// Ceran mountain
	" /\\/\\ ",
	"/  \\ \\",

	// Ceran mountain
	" /\\/\\ ",
	"/  \\ \\",

	// Ceran mountain
	" /\\/\\ ",
	"/  \\ \\",

	// Ceran hill
	"  * * ",
	" * *  ",

	// Ceran hill
	"  * * ",
	" * *  ",

	// Ceran hill
	"  * * ",
	" * *  ",

	// Ceran hill
	"  * * ",
	" * *  ",

	// Ceran swamp
	"  v v ",
	" v v  ",

	// Ceran swamp
	"  v v ",
	" v v  ",

	// Ceran swamp
	"  v v ",
	" v v  ",

	// Ceran swamp
	"  v v ",
	" v v  ",

	// Ceran jungle
	"  @ @ ",
	" @ @  ",

	// Ceran jungle
	"  @ @ ",
	" @ @  ",

	// Ceran jungle
	"  @ @ ",
	" @ @  ",

	// Ceran jungle
	"  @ @ ",
	" @ @  ",

	// Ceran desert
	"  . . ",
	" . .  ",

	// Ceran desert
	"  . . ",
	" . .  ",

	// Ceran desert
	"  . . ",
	" . .  ",

	// Ceran desert
	"  . . ",
	" . .  ",

	// Ceran wasteland
	"  ; ; ",
	" ; ;  ",

	// Ceran wasteland
	"  ; ; ",
	" ; ;  ",

	// Ceran tundra
	"  ' ' ",
	" ' '  ",

	// Ceran tundra
	"  ' ' ",
	" ' '  ",

	// Ceran tundra
	"  ' ' ",
	" ' '  ",

	// Ceran tundra
	"  ' ' ",
	" ' '  ",

	// Ceran cavern
	"  . . ",
	" . .  ",

	// Ceran cavern
	"  . . ",
	" . .  ",

	// Ceran cavern
	"  . . ",
	" . .  ",

	// Ceran cavern
	"  . . ",
	" . .  ",

	// Ceran underforest
	"  ^ ^ ",
	" ^ ^  ",

	// Ceran underforest
	"  ^ ^ ",
	" ^ ^  ",

	// Ceran underforest
	"  ^ ^ ",
	" ^ ^  ",

	// Ceran underforest
	"  ^ ^ ",
	" ^ ^  ",

	// Ceran tunnels
	"      ",
	"      ",

	// Ceran tunnels
	"      ",
	"      ",

	// Ceran underlake
	" ~    ",
	"    ~ ",

	// Ceran underlake
	" ~    ",
	"    ~ ",

	// Ceran underlake
	" ~    ",
	"    ~ ",

	// Ceran dcavern
	"  . . ",
	" . .  ",

	// Ceran dcavern
	"  . . ",
	" . .  ",

	// Ceran dcavern
	"  . . ",
	" . .  ",

	// Ceran dcavern
	"  . . ",
	" . .  ",

	// Ceran deepforest
	"  ^ ^ ",
	" ^ ^  ",

	// Ceran deepforest
	"  ^ ^ ",
	" ^ ^  ",

	// Ceran deepforest
	"  ^ ^ ",
	" ^ ^  ",

	// Ceran deepforest
	"  ^ ^ ",
	" ^ ^  ",

	// Ceran dtunnels
	"      ",
	"      ",

	// Ceran dtunnels
	"      ",
	"      ",

	// Tarmellion mines
	"  x x ",
	" x x  ",

	// Tarmellion mines
	"  x x ",
	" x x  ",

	// Tarmellion mines
	"  x x ",
	" x x  ",

	// Tarmellion mines
	"  x x ",
	" x x  ",

	// Tarmellion mines
	"  x x ",
	" x x  ",

	// Tarmellion mines
	"  x x ",
	" x x  ",

	// Tarmellion chamber
	"   .  ",
	" .   .",

	// Tarmellion chamber
	"   .  ",
	" .   .",

	// Tarmellion chamber
	"   .  ",
	" .   .",

	// Tarmellion chamber
	"   .  ",
	" .   .",

	// Tarmellion chamber
	"   .  ",
	" .   .",

	// Tarmellion chamber
	"   .  ",
	" .   .",

	// Ceran ethernal ice
	" . .  ",
	"  . . ",

	// Ceran glacier
	" .^.^ ",
	" ^.^. ",

	// Ceran crystal cavern
	"| | | ",
	" v v v",

	// Ceran blue cavern
	"| | | ",
	" v v v",

	// Ceran red cavern
	"| | | ",
	" v v v",

	// Ceran yellow cavern
	"| | | ",
	" v v v",

	// Ceran orange cavern
	"| | | ",
	" v v v",

	// Ceran green cavern
	"| | | ",
	" v v v",

	// Ceran violet cavern
	"| | | ",
	" v v v",

	// Ceran black cavern
	"| | | ",
	" v v v",

	// Ceran white cavern
	"| | | ",
	" v v v",

};

// NEW FUNCTION DK 2000.03.07,
// converted WriteReport
//
void ARegion::WriteTemplateHeader( Areport *f, Faction *fac, ARegionList *pRegs, int month) {

	f->PutStr("");

	f->PutStr("-------------------------------------------------"
			  "----------", 1);

	// plain (X,Y) in Blah, contains Blah
	f->PutStr(Print( pRegs ), 1);

	char buffer[LINE_WIDTH+1];
	char *data = buffer + MAP_WIDTH;
	int line = 0;

	Production *prod;
	int any;

	// ----------------------------------------------------------------
	GetMapLine(buffer, line, pRegs);
	TrimWrite(f, buffer);
	line++;

	// ----------------------------------------------------------------

	if (Globals->WEATHER_EXISTS) {
		GetMapLine(buffer, line, pRegs);

		char* nextWeather = "";
		int nxtweather = pRegs->GetWeather( this, (month + 1) % 12 );
		if (nxtweather == W_WINTER)
			nextWeather = "winter";
		if (nxtweather == W_MONSOON)
			nextWeather = "monsoon";
		if (nxtweather == W_NORMAL)
			nextWeather = "clear";
		sprintf(data, "Next %s", nextWeather);

		TrimWrite(f, buffer);
		line++;
	}

	// ----------------------------------------------------------------
	GetMapLine(buffer, line, pRegs);
	sprintf(data, "Tax  %5i", money);
	TrimWrite(f, buffer);
	line++;

	// ----------------------------------------------------------------
	prod = products.GetProd(I_SILVER, S_ENTERTAINMENT);
	if (prod) {
		GetMapLine(buffer, line, pRegs);
		sprintf(data, "Ente %5i", prod->amount);
		TrimWrite(f, buffer);
		line++;
	}

	// ----------------------------------------------------------------
	prod = products.GetProd(I_SILVER, -1);
	if (prod) {
		GetMapLine(buffer, line, pRegs);
		sprintf(data, "Wage %5i (max %i)", prod->productivity, prod->amount);
		TrimWrite(f, buffer);
		line++;
	}

	// ----------------------------------------------------------------
	any = 0;
	{
		forlist(&markets) {
			Market * m = (Market *) elem;
			if (!m->amount) continue;
			if (m->type == M_SELL) {

				// deactivated by henrik
				//if (ItemDefs[m->item].type & IT_ADVANCED)
				//	if (!HasItem(fac,m->item)) continue;

				if (!any) {
					GetMapLine(buffer, line, pRegs);
					TrimWrite(f, buffer);
					line++;
				}

				GetMapLine(buffer, line, pRegs);

				if (m->amount == -1) {
					sprintf(data, "%s unlim %4s @ %3i",
						(any ? "    " : "Want"),
						ItemDefs[m->item].abr,
						m->price);
				} else {
					sprintf(data, "%s %5i %4s @ %3i",
						(any ? "    " : "Want"),
						m->amount,
						ItemDefs[m->item].abr,
						m->price);
				}
				TrimWrite(f, buffer);
				line++;

				any = 1;
			}
		}
	}

	// ----------------------------------------------------------------
	any = 0;
	{
		forlist(&markets) {
			Market * m = (Market *) elem;
			if (!m->amount) continue;
			if (m->type == M_BUY) {

				if (!any) {
					GetMapLine(buffer, line, pRegs);
					TrimWrite(f, buffer);
					line++;
				}

				GetMapLine(buffer, line, pRegs);

				if (m->amount == -1) {
					sprintf(data, "%s unlim %4s @ %3i",
						(any ? "    " : "Sell"),
						ItemDefs[m->item].abr,
						m->price);
				} else {
					sprintf(data, "%s %5i %4s @ %3i",
						(any ? "    " : "Sell"),
						m->amount,
						ItemDefs[m->item].abr,
						m->price);
				}

				TrimWrite(f, buffer);
				line++;
				any = 1;
			}
		}
	}

	// ----------------------------------------------------------------
	any = 0;
	{
		forlist((&products)) {
			Production * p = ((Production *) elem);
			if (ItemDefs[p->itemtype].type & IT_ADVANCED) {
				if (!CanMakeAdv(fac,p->itemtype)) continue;
			} else {
				if (p->itemtype == I_SILVER) continue;
			}

			if (!any) {
				GetMapLine(buffer, line, pRegs);
				TrimWrite(f, buffer);
				line++;
			}

			GetMapLine(buffer, line, pRegs);

			if (p->amount == -1) {
				sprintf(data, "%s unlim %4s",
					(any ? "    " : "Prod"),
					ItemDefs[p->itemtype].abr);
			} else {
				sprintf(data, "%s %5i %4s",
					(any ? "    " : "Prod"),
					p->amount,
					ItemDefs[p->itemtype].abr);
			}

			TrimWrite(f, buffer);
			line++;
			any = 1;

		}
	}

	// ----------------------------------------------------------------

	if (Globals->GATES_EXIST && gate) {
		int sawgate = 0;
		forlist(&objects) {
			Object *o = (Object *) elem;
			forlist(&o->units) {
				Unit *u = (Unit *) elem;
				if (!sawgate && u->faction == fac && u->GetSkill(S_GATE_LORE)) {
					GetMapLine(buffer, line, pRegs);
					TrimWrite(f, buffer);
					line++;

					GetMapLine(buffer, line, pRegs);
					sprintf(data, "Gate %4i", gate);
					TrimWrite(f, buffer);
					line++;

					sawgate = 1;
				}
			}
		}
	}

	// ----------------------------------------------------------------
	while (line < TEMPLATE_MAX_LINES) {
		GetMapLine(buffer, line, pRegs);
		TrimWrite(f, buffer);
		line++;
	}
}


// NEW FUNCTION DK 2000.03.07,
// converted WriteExits
//
void ARegion::GetMapLine(char *buffer, int line, ARegionList *pRegs ) {

	for (int m=0; m<MAP_WIDTH; m++) {
		buffer[m] = ' ';
	}
	buffer[MAP_WIDTH] = 0;

	if (line >= TEMPLATE_MAX_LINES) return;

	char *dest = buffer+TMPL_MAP_OFS;
	memcpy(dest, TemplateMap[line], TMPL_MAP_WIDTH);

	ARegion *r = this;
	int x, y, t, i = 0;
	char *name;

	t = (r->type + 1) * 2;
	name = (r->town ? r->town->name->Str() : NULL);

	for (;;) {

		x = dircrd[i*2];
		y = dircrd[i*2+1];

		if (y == line || y+1 == line) {
			if (y == line) {
				if (name) {
					int len = strlen(name);
					if (len > FILL_SIZE) len = FILL_SIZE;
					memcpy(dest + x, name, len);
				} else {
					memcpy(dest + x, terfill[t], FILL_SIZE);
				}
			} else {
				t++;
				memcpy(dest + x, terfill[t], FILL_SIZE);
			}
		}

		if (i >= NDIRS) break;

		ARegion * r = neighbors[i];
		if (r) {
			t = (r->type + 1) * 2;
			name = (r->town ? r->town->name->Str() : NULL);
		} else {
			t = 0;
			name = NULL;
		}

		i++;
	}
}

static void TrimWrite(Areport *f, char *buffer) {

	char *p = buffer + strlen(buffer) - 1;
	while (p > buffer) {
		if (*p == ' ') {
			p--;
		} else {
			break;
		}
	}
	p[1] = 0;

	f->PutStr(buffer, 1);
}
