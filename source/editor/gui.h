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
// END A3HEADER

#ifndef GUI_CLASS
#define GUI_CLASS

class GuiFrame;
class GuiApp;

#include "edit.h"
#include "map.h"
#include "tree.h"
#include "aextend.h"
#include "list.h"

#include "../game.h"
#include "../aregion.h"
#include "../object.h"
#include "../production.h"
#include "../unit.h"
#include "../faction.h"

#include "wx/wx.h"
#include "wx/splitter.h"

// control ids
enum
{
	Gui_Quit,
	Gui_About,
	Gui_Game_Open,
	Gui_Game_Save,
	Gui_Game_Run,
	Gui_Game_CheckOrders,
	Gui_Options_Recreate,
	Gui_Show_Tree,
	Gui_Show_List,
	Gui_Show_Map,
	Gui_Window_Arrange,
	Gui_Game_Options,
};

enum
{
	SELECT_REGION,
	SELECT_OBJECT,
	SELECT_UNIT,
	SELECT_FACTION,
	SELECT_MARKET,
	SELECT_PRODUCTION,
	_SELECT_ELEMENTS,
	SELECT_GAME,
	SELECT_LEVEL,
	NUM_SELECT
};

class GuiApp : public wxApp
{
	friend class GuiFrame;
	public:
		~GuiApp();

		void Select( ARegion *, bool add = false );
		void Select( Object *, bool add = false );
		void Select( Unit *, bool add = false );
		void Select( Game *, bool add = false );
		void Select( Faction *, bool add = false );
		void Select( Market *, bool add = false );
		void Select( Production *, bool add = false );
		void Select( ARegionArray *, bool add = false );

		void UpdateFactions();
		void UpdateRegions();
		void UpdateUnits();
		void UpdateObjects();
		void UpdateMarkets();
		void UpdateProductions();
		void UpdateLevels();

		void Trash( Unit * );
		void Trash( Faction * );
		void Trash( Market * );
		void Trash( Production * );
		void Trash( Object * );
	//	void TrashLevel( ARegionArray * );
	//	void TrashRegion( ARegion * );

		Unit * AddUnit( Faction * pFaction = NULL );
		Faction * AddFaction();
		Market * AddMarket();
		Production * AddProduction();
		Object * AddObject();

	//	ARegion * AddRegion();

		void UpdateSelection();
		void UpdateSelection( AElemArray * , int, wxWindow * from = NULL );

		bool IsSelected( AListElem * );
		void UpdateStatusBar( ARegion * );
		void UpdateStatusBarDebug( int xpos, int ypos, int hexX, int hexY, int scrollX, int scrollY , int hexSize, int hexHeight );
		bool TerrainHasEnabledRace( int );
		int SaveGame();
		int ParseOrder( const char * );

		Game * m_game;

		ARegionArray * selectedLevel;
		AElemArray * selectedElems;
		int curSelection;
		wxColour guiColourLt;
		wxColour guiColourDk;

	private:
		bool OnInit();

		int LoadGame( const char * );

		void Select( AListElem *, int selectType, bool add ); 

		bool IsSelected( Game * );
		bool IsSelected( ARegionArray * );

		void AddToSelected( AListElem * );
		void AddToSelected( Game * );
		void AddToSelected( ARegionArray * );

		void RemoveFromSelected( AListElem * );
		void RemoveFromSelected( Game * );
		void RemoveFromSelected( ARegionArray * );

		void SetSelectionType( int, bool alwaysClear = false );

		void PostLoadGame();

		int runStep;
		wxMenu * fileMenu;
};

class GuiSplitter : public wxSplitterWindow 
{
	public:
		GuiSplitter( wxWindow * );
	
	private:
		void OnDoubleClickSash( int x, int y);
		
};

class GuiFrame : public wxFrame
{
	public:
		GuiFrame( const wxSize& size );
		~GuiFrame();

		void CreatePanes();

		MapCanvas * map;
		TreeCanvas * tree;
		EditFrame * editor;
		ListCanvas * list;

	private:
		bool waitDraw;

		void RefreshWindows();
		void InitToolBar( wxToolBar * );

		void OnSize( wxSizeEvent & );
		void OnAbout( wxCommandEvent & );
		void OnNewWindow( wxCommandEvent & );
		void OnQuit( wxCommandEvent & );
		void OnClose( wxCloseEvent & );
		void OnGameOpen( wxCommandEvent & );
		void OnGameSave( wxCommandEvent & );
		void OnGameRun( wxCommandEvent & );
		void OnGameOptions( wxCommandEvent & );
		void OnOptionsRecreate( wxCommandEvent & );
		void OnOptionsTerrain( wxCommandEvent & );
		void OnOptionsShowTreeHeaders( wxCommandEvent & );
		void OnOptionsShowFunk( wxCommandEvent & );
		void OnOptionsShowListMarketsProducts( wxCommandEvent & );

		GuiSplitter * vSplitter;
		GuiSplitter * hSplitter;

		DECLARE_EVENT_TABLE()

};

extern GuiApp * app;
extern GuiFrame * frame;

#endif


