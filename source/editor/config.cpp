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

#include "config.h"
#include "../gamedata.h"

TerrainConfigType tcd[] = {
	// terrain, {Red,Green,Blue}
	{ R_OCEAN, {0,0,255} },
	{ R_PLAIN, {255, 230, 160} },
	{ R_FOREST, {64, 200, 64} },
	{ R_MOUNTAIN, {190, 95, 0} },
	{ R_SWAMP, {160, 167, 79} },
	{ R_JUNGLE, {0, 127, 0} },
	{ R_DESERT, {223, 160, 50} },
	{ R_TUNDRA, {189, 203, 231} },
	{ R_CAVERN, {66, 0, 0} },
	{ R_UFOREST, {0, 130, 66} },
	{ R_TUNNELS, {132, 65, 0} },
	{ R_NEXUS, {0,100,100} },
	{ R_VOLCANO, {132, 65, 0} },
	{ R_LAKE, {132, 65, 0} },
	{ R_ISLAND_PLAIN, {255, 230, 160} },
	{ R_ISLAND_SWAMP, {160, 167, 79} },
	{ R_ISLAND_MOUNTAIN, {190, 95, 0} },
	{ R_T_OCEAN1, {0,0,255} },
	{ R_T_PLAIN1, {255, 230, 160} },
	{ R_T_PLAIN2, {255, 230, 160} },
	{ R_T_PLAIN3, {255, 230, 160} },
	{ R_T_LAKE1,{132, 65, 0} },
	{ R_T_LAKE2, {132, 65, 0} },
	{ R_T_LAKE3, {132, 65, 0} },
	{ R_T_FOREST1, {64, 200, 64} },
	{ R_T_FOREST2, {64, 200, 64} },
	{ R_T_FOREST3, {64, 200, 64} },
	{ R_T_MYSTFOREST1, {0, 54, 74} },
	{ R_T_MOUNTAIN1, {190, 95, 0} },
	{ R_T_MOUNTAIN2, {190, 95, 0} },
	{ R_T_MOUNTAIN3, {190, 95, 0} },
	{ R_T_HILL1, {255, 207, 74} },
	{ R_T_HILL2, {255, 207, 74} },
	{ R_T_HILL3, {255, 207, 74} },
	{ R_T_SWAMP1, {160, 167, 79} },
	{ R_T_SWAMP2, {160, 167, 79} },
	{ R_T_SWAMP3, {160, 167, 79} },
	{ R_T_JUNGLE1, {0, 127, 0} },
	{ R_T_JUNGLE2, {0, 127, 0} },
	{ R_T_JUNGLE3, {0, 127, 0} },
	{ R_T_DESERT1, {223, 160, 50} },
	{ R_T_DESERT2, {223, 160, 50} },
	{ R_T_DESERT3, {223, 160, 50} },
	{ R_T_VOLCANO1, {132, 65, 0} },
	{ R_T_TUNDRA1, {189, 203, 231} },
	{ R_T_TUNDRA2, {189, 203, 231} },
	{ R_T_TUNDRA3, {189, 203, 231} },
	{ R_T_CAVERN1, {66, 0, 0} },
	{ R_T_CAVERN2, {66, 0, 0} },
	{ R_T_CAVERN3, {66, 0, 0} },
	{ R_T_UNDERFOREST1, {0, 130, 66} },
	{ R_T_UNDERFOREST2, {0, 130, 66} },
	{ R_T_UNDERFOREST3, {0, 130, 66} },
	{ R_T_TUNNELS1, {132, 65, 0} },
	{ R_T_TUNNELS2, {132, 65, 0} },
	{ R_T_TUNNELS3, {132, 65, 0} },
	{ R_T_GROTTO1, {0,0,0} },
	{ R_T_PLAIN4, {255, 230, 160} },
	{ R_T_PLAIN5, {255, 230, 160} },
	{ R_T_PLAIN6, {255, 230, 160} },
	{ R_T_LAKE4, {132, 65, 0} },
	{ R_T_LAKE5, {132, 65, 0} },
	{ R_T_LAKE6, {132, 65, 0} },
	{ R_T_FOREST4, {64, 200, 64} },
	{ R_T_FOREST5, {64, 200, 64} },
	{ R_T_FOREST6, {64, 200, 64} },
	{ R_T_MYSTFOREST2, {0, 54, 74} },
	{ R_T_MOUNTAIN4, {190, 95, 0} },
	{ R_T_MOUNTAIN5, {190, 95, 0} },
	{ R_T_MOUNTAIN6, {190, 95, 0} },
	{ R_T_HILL4, {255, 207, 74} },
	{ R_T_HILL5, {255, 207, 74} },
	{ R_T_HILL6, {255, 207, 74} },
	{ R_T_SWAMP4, {160, 167, 79} },
	{ R_T_SWAMP5, {160, 167, 79} },
	{ R_T_SWAMP6, {160, 167, 79} },
	{ R_T_JUNGLE4, {0, 127, 0} },
	{ R_T_JUNGLE5, {0, 127, 0} },
	{ R_T_JUNGLE6, {0, 127, 0} },
	{ R_T_DESERT4, {223, 160, 50} },
	{ R_T_DESERT5, {223, 160, 50} },
	{ R_T_DESERT6, {223, 160, 50} },
	{ R_T_VOLCANO2, {132, 65, 0} },
	{ R_T_TUNDRA4, {189, 203, 231} },
	{ R_T_TUNDRA5, {189, 203, 231} },
	{ R_T_TUNDRA6, {189, 203, 231} },
	{ R_T_CAVERN4, {66, 0, 0} },
	{ R_T_CAVERN5, {66, 0, 0} },
	{ R_T_CAVERN6, {66, 0, 0} },
	{ R_T_DEEPFOREST1, {0, 130, 66} },
	{ R_T_DEEPFOREST2, {0, 130, 66} },
	{ R_T_DEEPFOREST3, {0, 130, 66} },
	{ R_T_TUNNELS4, {132, 65, 0} },
	{ R_T_TUNNELS5, {132, 65, 0} },
	{ R_T_TUNNELS6, {132, 65, 0} },
	{ R_T_GROTTO2, {0,0,0} },
	{ R_CE_OCEAN, {0,0,0} },
	{ R_CE_OCEAN1, {0,0,0} },
	{ R_CE_GDPLAIN, {0,0,0} },
	{ R_CE_GDPLAIN1, {0,0,0} },
	{ R_CE_EVPLAIN, {0,0,0} },
	{ R_CE_EVPLAIN1, {0,0,0} },
	{ R_CE_GDGRASSLAND, {0,0,0} },
	{ R_CE_GDGRASSLAND1, {0,0,0} },
	{ R_CE_EVGRASSLAND, {0,0,0} },
	{ R_CE_EVGRASSLAND1, {0,0,0} },
	{ R_CE_GDLAKE, {0,0,0} },
	{ R_CE_NELAKE, {0,0,0} },
	{ R_CE_EVLAKE, {0,0,0} },
	{ R_CE_GDFOREST, {0,0,0} },
	{ R_CE_GDFOREST1, {0,0,0} },
	{ R_CE_EVFOREST, {0,0,0} },
	{ R_CE_EVFOREST1, {0,0,0} },
	{ R_CE_MYSTFOREST, {0,0,0} },
	{ R_CE_MYSTFOREST1, {0,0,0} },
	{ R_CE_GDMOUNTAIN, {0,0,0} },
	{ R_CE_GDMOUNTAIN1, {0,0,0} },
	{ R_CE_EVMOUNTAIN, {0,0,0} },
	{ R_CE_EVMOUNTAIN1, {0,0,0} },
	{ R_CE_GDHILL, {0,0,0} },
	{ R_CE_GDHILL1, {0,0,0} },
	{ R_CE_EVHILL, {0,0,0} },
	{ R_CE_EVHILL1, {0,0,0} },
	{ R_CE_GDSWAMP, {0,0,0} },
	{ R_CE_GDSWAMP1, {0,0,0} },
	{ R_CE_EVSWAMP, {0,0,0} },
	{ R_CE_EVSWAMP1, {0,0,0} },
	{ R_CE_GDJUNGLE, {0,0,0} },
	{ R_CE_GDJUNGLE1, {0,0,0} },
	{ R_CE_EVJUNGLE, {0,0,0} },
	{ R_CE_EVJUNGLE1, {0,0,0} },
	{ R_CE_GDDESERT, {0,0,0} },
	{ R_CE_GDDESERT1, {0,0,0} },
	{ R_CE_EVDESERT, {0,0,0} },
	{ R_CE_EVDESERT1, {0,0,0} },
	{ R_CE_WASTELAND, {0,0,0} },
	{ R_CE_WASTELAND1, {0,0,0} },
	{ R_CE_GDTUNDRA, {0,0,0} },
	{ R_CE_GDTUNDRA1, {0,0,0} },
	{ R_CE_EVTUNDRA, {0,0,0} },
	{ R_CE_EVTUNDRA1, {0,0,0} },
	{ R_CE_GDCAVERN, {0,0,0} },
	{ R_CE_GDCAVERN1, {0,0,0} },
	{ R_CE_EVCAVERN, {0,0,0} },
	{ R_CE_EVCAVERN1, {0,0,0} },
	{ R_CE_GDUFOREST, {0,0,0} },
	{ R_CE_GDUFOREST1, {0,0,0} },
	{ R_CE_EVUFOREST, {0,0,0} },
	{ R_CE_EVUFOREST1, {0,0,0} },
	{ R_CE_TUNNELS, {0,0,0} },
	{ R_CE_TUNNELS1, {0,0,0} },
	{ R_CE_GDULAKE, {0,0,0} },
	{ R_CE_NEULAKE, {0,0,0} },
	{ R_CE_EVULAKE, {0,0,0} },
	{ R_CE_GDDCAVERN, {0,0,0} },
	{ R_CE_GDDCAVERN1, {0,0,0} },
	{ R_CE_EVDCAVERN, {0,0,0} },
	{ R_CE_EVDCAVERN1, {0,0,0} },
	{ R_CE_GDDFOREST, {0,0,0} },
	{ R_CE_GDDFOREST1, {0,0,0} },
	{ R_CE_EVDFOREST, {0,0,0} },
	{ R_CE_EVDFOREST1, {0,0,0} },
	{ R_CE_DTUNNELS, {0,0,0} },
	{ R_CE_DTUNNELS1, {0,0,0} },
};

TerrainConfigType * TerrainConfigDefs = tcd;


void SetTerrainBrush( int terrain, wxBrush &brush, int hilight )
{
	TerrainConfigType * tct = &( TerrainConfigDefs[terrain] );

	int red = tct->colorRGB[0];
	int green = tct->colorRGB[1];
	int blue = tct->colorRGB[2];

	if( hilight ) {
		red = red + ( 256-red ) / 3;
		green = green + ( 256-green ) / 3;
		blue = blue + ( 256-blue ) / 3;
	}

	brush.SetColour( red,green,blue );
}