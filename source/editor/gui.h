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
// END A3HEADER

#ifndef GUI_CLASS
#define GUI_CLASS

#include "edit.h"
#include "map.h"
#include "tree.h"
#include "alistex.h"

#include "../game.h"
#include "../aregion.h"
#include "../object.h"
#include "../production.h"
#include "../unit.h"
#include "../faction.h"

#include "wx/wx.h"

// control ids
enum
{
	Gui_Quit,
	Gui_About,
	Gui_Game_Open,
	Gui_Game_Save,
	Gui_Options_Recreate,
	Gui_Show_Tree,
	Gui_Show_Map,
};

class MyApp : public wxApp
{
public:
	~MyApp();
	bool OnInit();
	Game * m_game;
	int LoadGame( const char * );
	int SaveGame( const char * );

	ARegionList * GetRegions();
	AList * GetFactions();
	int * GetFacSeq();

	void UpdateStatusBar( ARegion * );

	AElemArray * selectedRegions;
	AElemArray * selectedUnits;
	AElemArray * selectedFactions;
	AElemArray * selectedMarkets;
	AElemArray * selectedObjects;
	AElemArray * selectedProductions;

	void SelectRegion( ARegion *, bool add = false );
	void SelectObject( Object *, bool add = false );
	void SelectUnit( Unit *, bool add = false );
	void SelectFaction( Faction *, bool add = false );
	void SelectMarket( Market *, bool add = false );
	void SelectProduction( Production *, bool add = false );
	int DeselectUnit( Unit * );
	int DeselectFaction( Faction * );
	int DeselectMarket( Market * );
	void DeselectAllRegions();

	void TrashUnit( Unit * );
	void TrashFaction( Faction * );
	void TrashMarket( Market * );
	void TrashProduction( Production * );
	void TrashObject( Object * );

	void UpdateFactions();
	void UpdateRegions();
	void UpdateUnits();
	void UpdateObjects();
	void UpdateMarkets();
	void UpdateProductions();

//	void TrashRegion( ARegion * );

	Unit * AddUnit( Faction * pFaction = NULL );
	Faction * AddFaction();
	Market * AddMarket();
	Production * AddProduction();
	Object * AddObject();
//	ARegion * AddRegion();

	bool TerrainHasEnabledRace( int );
	bool recreateData;
};

class MyFrame : public wxMDIParentFrame
{
public:

	MapChild * map;
	TreeChild * tree;
	EditFrame * editor;

	wxSizer * sizerSelector;
	wxSizer * sizerEdit;
	wxSizer * sizerMap;

	MyFrame( wxWindow *parent, const wxWindowID id, const wxString& title,
			const wxPoint& pos, const wxSize& size, const long style );

	void AdjustSizers();
	void InitToolBar( wxToolBar * );

	void OnSize( wxSizeEvent & );
	void OnAbout( wxCommandEvent & );
	void OnNewWindow( wxCommandEvent & );
	void OnQuit( wxCommandEvent & );
	void OnClose( wxCloseEvent & );

	void OnGameOpen( wxCommandEvent & );
	void OnGameSave( wxCommandEvent & );
	void OnShowMap( wxCommandEvent & );
	void OnShowTree( wxCommandEvent & );
	void OnOptionsRecreate( wxCommandEvent & );

	DECLARE_EVENT_TABLE()

};

extern MyApp * app;
extern MyFrame * frame;

#endif