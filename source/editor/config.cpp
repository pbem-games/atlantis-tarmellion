// START A3HEADER
//
// This source file is part of Atlantis GUI
// Copyright (C) 2003-2004 Ben Lloyd
//
// To be used with the Atlantis PBM game program.
// Copyright (C) 1995-2004 Geoff Dunbar
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
#include "../fileio.h"
#include "../gamedata.h"
#include "../gameio.h"
#include "wx/filename.h"

TerrainArray TerrainColors;
MapConfigType MapConfig;
GuiConfigType GuiConfig;

/**
 * Comparison function for sorting terrain colours
 */
int CompareTerrain( TerrainConfigType ** t1, TerrainConfigType ** t2)
{
	if( ( *t1 )->terrain > ( *t2 )->terrain ) return 1;
	if( ( *t1 )->terrain == ( *t2 )->terrain ) return 0;
	return -1;
}


/**
 * Load default colours and bitmaps for standard terrains
 * These don't necessarily have to be in the same order is in gamedata.h
 */
void LoadDefaultColors()
{
	TerrainColors.Add( new TerrainConfigType( R_OCEAN, 0, 0, 255, "ocean.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_PLAIN, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_FOREST, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_MOUNTAIN, 190, 95, 0, "mountain.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_SWAMP, 160, 167, 79, "swamp.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_JUNGLE, 0, 127, 0, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_DESERT, 223, 160, 50, "desert.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_TUNDRA, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_NEXUS, 0,100,100, "nexus.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_VOLCANO, 132, 65, 0, "volcano.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_LAKE, 210, 210, 245, "lake.bmp" ) );

	TerrainColors.Add( new TerrainConfigType( R_ISLAND_PLAIN, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_ISLAND_SWAMP, 160, 167, 79, "swamp.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_ISLAND_MOUNTAIN, 190, 95, 0, "mountain.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_OCEAN1, 0,0,255, "ocean.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_PLAIN1, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_PLAIN2, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_PLAIN3, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_LAKE1, 32, 65, 0, "lake.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_LAKE2, 132, 65, 0, "lake.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_LAKE3, 132, 65, 0, "lake.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_FOREST1, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_FOREST2, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_FOREST3, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MYSTFOREST1, 0, 54, 74, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MOUNTAIN1, 190, 95, 0, "mountain.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MOUNTAIN2, 190, 95, 0, "mountain.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MOUNTAIN3, 190, 95, 0, "mountain.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_HILL1, 255, 207, 74, "hill.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_HILL2, 255, 207, 74, "hill.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_HILL3, 255, 207, 74, "hill.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_SWAMP1, 160, 167, 79, "swamp.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_SWAMP2, 160, 167, 79, "swamp.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_SWAMP3, 160, 167, 79, "swamp.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_JUNGLE1, 0, 127, 0, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_JUNGLE2, 0, 127, 0, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_JUNGLE3, 0, 127, 0, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_DESERT1, 223, 160, 50, "desert.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_DESERT2, 223, 160, 50, "desert.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_DESERT3, 223, 160, 50, "desert.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_VOLCANO1, 132, 65, 0, "volcano.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_TUNDRA1, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_TUNDRA2, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_TUNDRA3, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_CAVERN1, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_CAVERN2, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_CAVERN3, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_UNDERFOREST1, 0, 130, 66, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_UNDERFOREST2, 0, 130, 66, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_UNDERFOREST3, 0, 130, 66, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_TUNNELS1, 132, 65, 0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_TUNNELS2, 132, 65, 0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_TUNNELS3, 132, 65, 0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_GROTTO1, 0,0,0, "" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_PLAIN4, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_PLAIN5, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_PLAIN6, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_LAKE4, 132, 65, 0, "lake.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_LAKE5, 132, 65, 0, "lake.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_LAKE6, 132, 65, 0, "lake.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_FOREST4, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_FOREST5, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_FOREST6, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MYSTFOREST2, 0, 54, 74, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MOUNTAIN4, 190, 95, 0, "mountain.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MOUNTAIN5, 190, 95, 0, "mountain.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MOUNTAIN6, 190, 95, 0, "mountain.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_HILL4, 255, 207, 74, "hill.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_HILL5, 255, 207, 74, "hill.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_HILL6, 255, 207, 74, "hill.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_SWAMP4, 160, 167, 79, "swamp.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_SWAMP5, 160, 167, 79, "swamp.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_SWAMP6, 160, 167, 79, "swamp.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_JUNGLE4, 0, 127, 0, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_JUNGLE5, 0, 127, 0, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_JUNGLE6, 0, 127, 0, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_DESERT4, 223, 160, 50, "desert.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_DESERT5, 223, 160, 50, "desert.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_DESERT6, 223, 160, 50, "desert.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_VOLCANO2, 132, 65, 0, "volcano.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_TUNDRA4, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_TUNDRA5, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_TUNDRA6, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_CAVERN4, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_CAVERN5, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_CAVERN6, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_DEEPFOREST1, 0, 130, 66, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_DEEPFOREST2, 0, 130, 66, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_DEEPFOREST3, 0, 130, 66, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_TUNNELS4, 132, 65, 0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_TUNNELS5, 132, 65, 0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_TUNNELS6, 132, 65, 0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_GROTTO2, 0,0,0, "" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_OCEAN, 0,0,255, "ocean.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_OCEAN1, 0,0,255, "ocean.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDPLAIN, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDPLAIN1, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVPLAIN, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVPLAIN1, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDGRASSLAND, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDGRASSLAND1, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVGRASSLAND, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVGRASSLAND1, 255, 230, 160, "plains.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDLAKE, 132, 65, 0, "lake.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_NELAKE, 132, 65, 0, "lake.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVLAKE, 132, 65, 0, "lake.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDFOREST, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDFOREST1, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVFOREST, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVFOREST1, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_MYSTFOREST, 0, 54, 74, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_MYSTFOREST1, 0, 54, 74, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDMOUNTAIN, 190, 95, 0, "mountain.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDMOUNTAIN1, 190, 95, 0, "mountain.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVMOUNTAIN, 190, 95, 0, "mountain.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVMOUNTAIN1, 190, 95, 0, "mountain.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDHILL, 255, 207, 74, "hill.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDHILL1, 255, 207, 74, "hill.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVHILL, 255, 207, 74, "hill.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVHILL1, 255, 207, 74, "hill.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDSWAMP, 160, 167, 79, "swamp.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDSWAMP1, 160, 167, 79, "swamp.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVSWAMP, 160, 167, 79, "swamp.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVSWAMP1, 160, 167, 79, "swamp.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDJUNGLE, 0, 127, 0, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDJUNGLE1, 0, 127, 0, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVJUNGLE, 0, 127, 0, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVJUNGLE1, 0, 127, 0, "jungle.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDDESERT, 223, 160, 50, "desert.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDDESERT1, 223, 160, 50, "desert.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVDESERT, 223, 160, 50, "desert.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVDESERT1, 223, 160, 50, "desert.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_WASTELAND, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_WASTELAND1, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDTUNDRA, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDTUNDRA1, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVTUNDRA, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVTUNDRA1, 189, 203, 231, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDCAVERN, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDCAVERN1, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVCAVERN, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVCAVERN1, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDUFOREST, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDUFOREST1, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVUFOREST, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVUFOREST1, 64, 200, 64, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_TUNNELS, 132, 65, 0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_TUNNELS1, 132, 65, 0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDULAKE, 132, 65, 0, "lake.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_NEULAKE, 132, 65, 0, "lake.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVULAKE, 132, 65, 0, "lake.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDDCAVERN, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDDCAVERN1, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVDCAVERN, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVDCAVERN1, 66, 0, 0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDDFOREST, 0, 130, 66, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GDDFOREST1, 0, 130, 66, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVDFOREST, 0, 130, 66, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_EVDFOREST1, 0, 130, 66, "forest.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_DTUNNELS, 132, 65, 0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_DTUNNELS1, 132, 65, 0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MINES1, 0,0,0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MINES2, 0,0,0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MINES3, 0,0,0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MINES4, 0,0,0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MINES5, 0,0,0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_MINES6, 0,0,0, "tunnel.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_CHAMBER1, 0,0,0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_CHAMBER2, 0,0,0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_CHAMBER3, 0,0,0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_CHAMBER4, 0,0,0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_CHAMBER5, 0,0,0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_T_CHAMBER6, 0,0,0, "cavern.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_ICE, 0,0,0, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GLACIER, 0,0,0, "tundra.bmp" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_CRYSTALCAVERN, 0,0,0, "" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_BLUECAVERN, 0,0,0, "" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_REDCAVERN, 0,0,0, "" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_YELLOWCAVERN, 0,0,0, "" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_ORANGECAVERN, 0,0,0, "" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_GREENCAVERN, 0,0,0, "" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_VIOLETCAVERN, 0,0,0, "" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_BLACKCAVERN, 0,0,0, "" ) );
	TerrainColors.Add( new TerrainConfigType( R_CE_WHITECAVERN, 0,0,0, "" ) );

	TerrainColors.Sort( CompareTerrain );
}

/**
 * Default constructor
 */
TerrainConfigType::TerrainConfigType()
{
	terrain = -1;
	colorRGB[0] = getrandom(256);
	colorRGB[1] = getrandom(256);
	colorRGB[2] = getrandom(256);
	image = NULL;
	image_h = NULL;
	image_extra = NULL;
}

/**
 * Create images for this terrain type
 *  image = default image
 *  image_h = hihglighted image
 *  image_extra = secondary highlight, for when an item in the region is selected
 */
void TerrainConfigType::InitImages( const wxString & bitmap )
{
	if( image ) delete image;
	if( image_h ) delete image_h;
	if( image_extra ) delete image_extra;

	if( bitmap != "" ) {
		wxFileName fname;

		fname.AssignDir( wxGetCwd() + fname.GetPathSeparator() + "bitmaps" );

		fname.SetFullName( bitmap );

		if( fname.FileExists() ) {
			tile = fname.GetFullName();

			// Create images
			image = new wxImage( fname.GetFullPath() );
			image_h = new wxImage( fname.GetFullPath() );
			image_extra = new wxImage( fname.GetFullPath() );

			// Get colour of mask = colour of top left pixel
			int r1 = image->GetRed( 0,0 );
			int g1 = image->GetGreen( 0,0 );
			int b1 = image->GetBlue( 0,0 );

			// Set mask colour
			image->SetMaskColour( r1, g1, b1 );
			image_h->SetMaskColour( r1, g1, b1 );
			image_extra->SetMaskColour( r1, g1, b1 );

			// Alter colour of image_h and image_extra images
			//  image_h is made lighter, image_extra is made redder 
			for( int x = 0; x < image_h->GetWidth(); x++ ) {
				for( int y = 0; y < image_h->GetHeight(); y++ ) {
					int red = image_h->GetRed( x, y );
					int green = image_h->GetGreen( x, y );
					int blue = image_h->GetBlue( x, y );

					if( red == r1 && green == g1 && blue == b1 ) continue;

					int newRed = red + ( 256-red ) / 2;
					int newGreen = green + ( 256-green ) / 2;
					int newBlue = blue + ( 256-blue ) / 2;

					image_h->SetRGB( x, y, newRed, newGreen, newBlue );

					newRed = red + (( 256-red ) * 2) / 3;
					newGreen = green ;
					newBlue = blue ;

					image_extra->SetRGB( x, y, newRed, newGreen, newBlue );

				}
			}		
		}
	}
}

/**
 * Default constructor
 * Initialise colour and bitmap
 */
TerrainConfigType::TerrainConfigType(int t, int r, int g, int b, const wxString & bitmap )
{
	terrain = t;
	colorRGB[0] = r;
	colorRGB[1] = g;
	colorRGB[2] = b;
	image = NULL;
	image_h = NULL;
	image_extra = NULL;

	InitImages( bitmap );
}

/**
 * Default destructor
 */
TerrainConfigType::~TerrainConfigType()
{
	if( image ) delete image;
	if( image_h ) delete image_h;
	if( image_extra ) delete image_extra;

}

/**
 * Load terrain information from config file
 */
void LoadTerrainColors( char * filename )
{
	WX_CLEAR_ARRAY( TerrainColors );

	Ainfile file;

	// If no file exists, just load default colours
	if( !wxFileExists( filename ) ) {
		LoadDefaultColors();
		return;
	}
	if (file.OpenByName( filename ) == -1 ) return;

	AString * line = file.GetStr();
	while( line ) {
		AString * pToken = line->gettoken();
		if( pToken ) {
			// Set some initial values 
			int RGB[3];
			RGB[0] = RGB[1] = RGB[2] = 0;
			wxString tile( "NoSuchFile" );

			// Read in the data
			int terrain = pToken->value();
			delete pToken;
			for( int i = 0; i < 3; i++ ) {
				pToken = line->gettoken();
				if( !pToken ) break;
				RGB[i] = pToken->value();
				delete pToken;
			}			
			pToken = line->gettoken();
			if( pToken ) {
				tile = pToken->Str();
				delete pToken;
			}

			// Add the new terrain colour
			TerrainColors.Add( new TerrainConfigType( terrain, RGB[0], RGB[1], RGB[2], tile ) );
		}
		delete line;
		line = file.GetStr();
	}
	file.Close();
}

/**
 * Save terrain info to config file
 */
void SaveTerrainColors( char * filename )
{
	Aoutfile file;

	if (file.OpenByName( filename ) == -1 ) return;

	for( unsigned int i = 0; i < TerrainColors.Count(); i ++ ) {

		TerrainConfigType * tct = TerrainColors[i];
		AString * temp = new AString( tct->terrain );
		
		for( int j = 0; j < 3; j++ ) {
			*temp += AString(" ") + AString( tct->colorRGB[j] );
		}
 
		if( tct->tile ) {
			*temp += " \"";
			*temp += tct->tile.GetData();
			*temp += "\"";
		}

		file.PutStr( temp->Str() );
		delete temp;
	}

	file.Close();
}

/**
 * Set brush to correct terrain colour
 */
void SetTerrainBrush( int terrain, wxBrush &brush, int hilight )
{
	int count = TerrainColors.Count();
	int i;

	TerrainConfigType * tct;
	for( i = 0; i < count; i++ ) {
		tct = TerrainColors[i];
		if( tct->terrain == terrain ) break;
	}

	if( i == count ) {
		brush.SetColour( 0, 0, 0 );
		return;
	}

	int red = tct->colorRGB[0];
	int green = tct->colorRGB[1];
	int blue = tct->colorRGB[2];

	if( hilight ) {
		red = red + ( 256-red ) / 3;
		green = green + ( 256-green ) / 3;
		blue = blue + ( 256-blue ) / 3;
	}

	brush.SetColour( red, green, blue );
}

/**
 * Returns correct image for terrain and highlight
 * highlight 1 = normal highlight
 * highlight 2 = extra highlight
 */
wxImage * GetTerrainTile( int terrain, int highlight )
{
	int count = TerrainColors.Count();
	int i;
	TerrainConfigType * tct;

	for( i = 0; i < count; i++ ) {
		tct = TerrainColors[i];
		if( tct->terrain == terrain ) break;
	}

	if( i == count ) {
		return NULL;
	}

	if( highlight == 1 ) 
		return tct->image_h;
	else if ( highlight == 2 )
		return tct->image_extra;
	else
		return tct->image;
}

/**
 * Save game config to file
 */
void SaveConfig( const char * filename )
{
	Aoutfile file;

	if (file.OpenByName( filename ) == -1 ) return;

	if( GuiConfig.lastGameFile == "" ) GuiConfig.lastGameFile = "none";
	if( GuiConfig.pop3Server == "" ) GuiConfig.pop3Server = "none";
	if( GuiConfig.smtpServer == "" ) GuiConfig.smtpServer = "none";
	if( GuiConfig.userName == "" ) GuiConfig.userName = "none";
	if( GuiConfig.email == "" ) GuiConfig.email = "none";
	if( GuiConfig.password == "" ) GuiConfig.password = "none";

	file.PutStr( "Gui config 0.7" );
	file.PutInt( MapConfig.showCities );
	file.PutInt( MapConfig.showObjects );
	file.PutInt( MapConfig.showGates );
	file.PutInt( MapConfig.showShafts );
	file.PutInt( MapConfig.showNames );
	file.PutInt( MapConfig.showCoords );
	file.PutInt( MapConfig.showOutlines );
	file.PutInt( MapConfig.spreadBy );

	file.PutInt( MapConfig.lastPlane );
	file.PutInt( MapConfig.lastZoom );
	file.PutInt( MapConfig.lastX );
	file.PutInt( MapConfig.lastY );

	file.PutInt( GuiConfig.recreateData );
	file.PutInt( GuiConfig.showTreeHeaders );
	file.PutInt( GuiConfig.showFunk );
	file.PutInt( GuiConfig.listMarketsProducts );
	file.PutInt( GuiConfig.useEmail );

	file.PutInt( GuiConfig.windowX );
	file.PutInt( GuiConfig.windowY );
	file.PutInt( GuiConfig.windowH );
	file.PutInt( GuiConfig.windowV );
	file.PutInt( GuiConfig.windowMax );
	file.PutInt( GuiConfig.splitterH );
	file.PutInt( GuiConfig.splitterV );

	file.PutStr( GuiConfig.lastGameFile.Str() );
	file.PutStr( GuiConfig.pop3Server.Str() );
	file.PutStr( GuiConfig.smtpServer.Str() );
	file.PutStr( GuiConfig.userName.Str() );
	file.PutStr( GuiConfig.email.Str() );
	file.PutStr( GuiConfig.password.Str() );
	
	file.Close();
}

/**
 * Load game config from file
 */
void LoadConfig( const char * filename )
{
	Ainfile file;

	if (file.OpenByName( filename ) == -1 ) return;

	AString * title = file.GetStr();

	if( !title || !( *title == "Gui config 0.7" )) return;

	delete title;

	MapConfig.showCities = (file.GetInt() ? true:false );
	MapConfig.showObjects = (file.GetInt() ? true:false );
	MapConfig.showGates = (file.GetInt() ? true:false );
	MapConfig.showShafts = (file.GetInt() ? true:false );
	MapConfig.showNames = (file.GetInt() ? true:false );
	MapConfig.showCoords = (file.GetInt() ? true:false );
	MapConfig.showOutlines = (file.GetInt() ? true:false );

	MapConfig.spreadBy = file.GetInt();
	MapConfig.lastPlane = file.GetInt();
	MapConfig.lastZoom = file.GetInt();
	MapConfig.lastX = file.GetInt();
	MapConfig.lastY = file.GetInt();

	GuiConfig.recreateData = (file.GetInt() ? true:false );
	GuiConfig.showTreeHeaders = (file.GetInt() ? true:false );
	GuiConfig.showFunk = (file.GetInt() ? true:false );
	GuiConfig.listMarketsProducts = (file.GetInt() ? true:false );
	GuiConfig.useEmail = (file.GetInt() ? true:false );

	GuiConfig.windowX = file.GetInt();
	GuiConfig.windowY = file.GetInt();
	GuiConfig.windowH = file.GetInt();
	GuiConfig.windowV = file.GetInt();
	GuiConfig.windowMax = (file.GetInt() ? true:false );
	GuiConfig.splitterH = file.GetInt();
	GuiConfig.splitterV = file.GetInt();

	AString * s = file.GetStr();
	if( !s || *s == "none" )
		s = new AString( "" );
	GuiConfig.lastGameFile = *s;
	delete s;

	s = file.GetStr();
	if( !s || *s == "none" )
		s = new AString( "" );
	GuiConfig.pop3Server = *s;
	delete s;

	s = file.GetStr();
	if( !s || *s == "none" )
		s = new AString( "" );
	GuiConfig.smtpServer = *s;
	delete s;

	s = file.GetStr();
	if( !s || *s == "none" )
		s = new AString( "" );
	GuiConfig.userName = *s;
	delete s;

	s = file.GetStr();
	if( !s || *s == "none" )
		s = new AString( "" );
	GuiConfig.email = *s;
	delete s;

	s = file.GetStr();
	if( !s || *s == "none" )
		s = new AString( "" );
	GuiConfig.password = *s;
	delete s;


	file.Close();
#ifndef __USE_EMAIL__
	GuiConfig.useEmail = false;
#endif

}


/**
 * Default constructor
 */
MapConfigType::MapConfigType()
{
	showCities = true;
	showObjects = true;
	showGates = true;
	showShafts = true;
	showNames = true;
	showCoords = true;
	showOutlines = false;
	spreadBy = 0;
	lastPlane = 1;
	lastZoom = 12;
	lastX = 0;
	lastY = 0;
}

/** 
 * Default constructor
 */
GuiConfigType::GuiConfigType()
{
	recreateData = true;
	showTreeHeaders = true;
	showFunk = true;
	listMarketsProducts = false;
	useEmail = true;
	windowX = -1;
	windowY = -1;
	windowH = -1;
	windowV = -1;
	windowMax = false;
	splitterH = -1;
	splitterV = -1;
}

/**
 * Default destructor
 */
GuiConfigType::~GuiConfigType()
{
}
