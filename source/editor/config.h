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

#ifndef CONFIG_CLASS
#define CONFIG_CLASS

#include "wx/wx.h"
#include "wx/image.h"

#include "../gamedefs.h"
#include "../alist.h"
#include "../astring.h"

class TerrainConfigType
{
	public:
		int terrain;
		int colorRGB[3];
		wxImage * image;
		wxImage * image_h;
		wxImage * image_extra;

		wxString tile;
		TerrainConfigType();
		TerrainConfigType(int, int, int, int, const wxString & );
		~TerrainConfigType();

		void InitImages( const wxString & bitmap );
};

class MapConfigType
{
	public:
		MapConfigType();

		bool showCities;
		bool showObjects;
		bool showGates;
		bool showShafts;
		bool showNames;
		bool showCoords;
		bool showOutlines;
		int spreadBy;
		int lastPlane;
		int lastZoom;
		int lastX;
		int lastY;
};

class GuiConfigType
{
	public:
		GuiConfigType();
		~GuiConfigType();

		bool recreateData;
		bool showTreeHeaders;
		bool showFunk;
		bool listMarketsProducts;
		bool useEmail;

		int windowX;
		int windowY;
		int windowH;
		int windowV;
		bool windowMax;
		int splitterH;
		int splitterV;

		AString lastGameFile;
		AString pop3Server;
		AString smtpServer;
		AString userName;
		AString email;
		AString password;
};

WX_DEFINE_ARRAY( TerrainConfigType *, TerrainArray );
extern TerrainArray TerrainColors;
extern MapConfigType MapConfig;
extern GuiConfigType GuiConfig;

void LoadDefaultColors();
void LoadTerrainColors( char * filename );
void SaveTerrainColors( char * filename );

void LoadConfig( const char * filename );
void SaveConfig( const char * filename );

void SetTerrainBrush( int, wxBrush &, int );
wxImage * GetTerrainTile( int terrain, int highlight );

#endif
