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


// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#define SUPPRESS_MSG 1

#include "wx/toolbar.h"
#include "wx/colordlg.h"

#include "bitmaps/open.xpm"
#include "bitmaps/save.xpm"
//#include "bitmaps/list.xpm"
//#include "bitmaps/help.xpm"
// #include "bitmaps/gui_big.xpm"
#include "bitmaps/gui.xpm"
//#include "bitmaps/map.xpm"
//#include "bitmaps/tree.xpm"

#include "config.h"
#include "gui.h"
#include "map.h"
#include "run.h"
//#include "tree.h"
#include "gamedata.h"
#include "options.h"
#include "wx/valtext.h"

#include <math.h>

IMPLEMENT_APP( GuiApp )

// ---------------------------------------------------------------------------
// global variables
// ---------------------------------------------------------------------------

GuiFrame *frame = ( GuiFrame * ) NULL;
GuiApp *app = ( GuiApp * ) NULL;

// ---------------------------------------------------------------------------
// event tables
// ---------------------------------------------------------------------------

BEGIN_EVENT_TABLE( GuiFrame, wxFrame )
	EVT_MENU( Gui_Game_Options, GuiFrame::OnGameOptions )
	EVT_MENU( Gui_Game_Open, GuiFrame::OnGameOpen )
	EVT_MENU( Gui_Game_Save, GuiFrame::OnGameSave )
	EVT_MENU( Gui_Game_Run, GuiFrame::OnGameRun )

	EVT_MENU( Gui_About, GuiFrame::OnAbout )
	EVT_MENU( Gui_Quit, GuiFrame::OnQuit )

//	EVT_CLOSE( GuiFrame::OnClose )

	EVT_SIZE( GuiFrame::OnSize )

END_EVENT_TABLE()


// ---------------------------------------------------------------------------
// GuiApp
// ---------------------------------------------------------------------------

/**
 * Destructor
 */
GuiApp::~GuiApp()
{
	/// Auto-save terrain colour definitions
	if( wxFileExists( "terrain.txt" ) )
		wxRemoveFile( "terrain.txt" );

	SaveTerrainColors( "terrain.txt" );

	/// Auto-save config
	if( wxFileExists( "config.txt" ) )
		wxRemoveFile( "config.txt" );

	SaveConfig( "config.txt" );

	/// Clean up
	WX_CLEAR_ARRAY( TerrainColors );
	if( m_game ) {
		if( wxFileExists( "game.bak" ) )
			wxRemoveFile( "game.bak" );
		m_game->SaveGame( "game.bak" );
		delete m_game;
	}
	delete selectedElems;

}

/**
 * Initialise application
 */
bool GuiApp::OnInit()
{
	app = this;

	/// Init variables
	m_game = NULL;
	selectedElems = new AElemArray();
	selectedLevel = NULL;
	runStep = 0;
	curSelection = -1;
	curHistory = 0;

	/// Load terrain colors
	LoadTerrainColors( "terrain.txt" );

	/// Load config
	if( wxFileExists( "config.txt" ) )
		LoadConfig( "config.txt" );

	/// Set window colours
	guiColourLt.Set( 210, 235, 235 );
	guiColourDk.Set( 200, 225, 245 );

	/// Create the main frame window
	wxPoint p( GuiConfig.windowX, GuiConfig.windowY );
	wxSize s( GuiConfig.windowH, GuiConfig.windowV );
	frame = new GuiFrame( p, s );

#ifdef __WXMSW__
	/// Maximize it?
	if( GuiConfig.windowMax )
		frame->Maximize();

	/// Give it an icon
	frame->SetIcon( wxIcon( _T( "gui_icn" ) ) );
#else
	frame->SetIcon( wxIcon( gui_xpm ) );
#endif
	
	/// Set menubar
	fileMenu = new wxMenu;
	fileMenu->Append( Gui_Game_Open, "&Open\tCtrl-O", "Open new game" );
	fileMenu->Append( Gui_Game_Save, "&Save\tCtrl-S", "Save game" );
	fileMenu->Append( Gui_Game_Run, "&Run\tCtrl-R", "Run game" );
	fileMenu->Append( Gui_Game_Options, "O&ptions\tCtrl-P", "Options" );
	fileMenu->Append( Gui_Quit, "&Exit\tAlt-X", "Quit the program" );

	fileMenu->Enable( Gui_Game_Run, false );
	
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append( Gui_About, "&About\tF1" );
	
	wxMenuBar *menuBar = new wxMenuBar;
	
	menuBar->Append( fileMenu, "&File" );
	menuBar->Append( helpMenu, "&Help" );
	
	frame->SetMenuBar( menuBar );
	
	/// Set statusbar
	frame->CreateStatusBar();
	int widths[5] = {50, -3, -1, -1, -1};
	frame->GetStatusBar()->SetFieldsCount( 5, widths );
	frame->Show( TRUE );
	
	SetTopWindow( frame );

	/// Open game file automatically if asked
	if( argc>1 ) {
		LoadGame( argv[1] );
		Select( m_game );
		UpdateSelection();
	} else {
		AString s = GuiConfig.lastGameFile;
		if( !( s == "" ) && wxFileExists( s.Str() ) ) {
			LoadGame( s.Str() );
		}
	}

	return TRUE;
}

/**
 * Create a new faction
 */
Faction * GuiApp::AddFaction()
{
	Faction *temp = new Faction( m_game->factionseq );
	AString x( "NoAddress" );
	temp->SetAddress( x );
	temp->lastorders = m_game->TurnNumber();

	m_game->factions.Add( temp );
	m_game->factionseq++;
	return temp;
}

/**
 * Create a new unit
 */
Unit * GuiApp::AddUnit(Faction * pFaction)
{
	Unit * nu = 0;

	/// Add new unit to specified faction, or to guard faction if none specified
	if( pFaction ) {
		nu = m_game->GetNewUnit(pFaction);
	} else {
		forlist( &m_game->factions ) {
			Faction * f = ( Faction * ) elem;
			if( f->num == m_game->guardfaction ) {
				nu =  m_game->GetNewUnit( f );
				break;
			}
		}
	}

	/// If we've already got a unit selected, copy everything over to the new units
	if( nu && curSelection == SELECT_UNIT && selectedElems->GetCount() > 0 ) {
		Unit * u = (Unit * ) selectedElems->Item(0);
		nu->combat = u->combat;
		nu->SetDescribe( u->describe );
		nu->flags = u->flags;
		nu->free = u->free;
		nu->readyItem = u->readyItem;
		nu->SetName( u->name->getlegal2() );
		nu->type = u->type;
		nu->guard = u->guard;
		forlist( &u->items ) {
			Item * i = ( Item * ) elem;
			nu->items.SetNum( i->type, i->num );
		}
		{	forlist( &u->skills ) {
				Skill * s = ( Skill * ) elem;
				nu->SetSkill( s->type, s->days );
			}
		}
	}

	return nu;
}

/**
 * Create and initialise the main window panes
 */
void GuiFrame::CreatePanes()
{
	app->selectedElems->Clear();
	app->selectedLevel = NULL;
	app->curSelection = -1;

	// Delete windows if they're already created
	if( map ) delete map;
	if( list ) delete list;
//	if( tree ) delete tree;
//	if( selection ) delete selection;
//	if( vSplitter ) delete vSplitter;
	if( hSplitter ) delete hSplitter;
	editor->HideAllPages();
	hSplitter = new GuiSplitter(this);
//	vSplitter = new GuiSplitter(hSplitter);

	// Create tree view
//	tree = new TreeCanvas( vSplitter );
// 	tree->Init();
//	tree->Show( true );

	// Create list view
	list = new ListCanvas( hSplitter );
	list->Init();
	list->Show( true );

	// Create map view
	map = new MapFrame( hSplitter );

	// Show everything
	int splitH = GuiConfig.splitterH;
	if( splitH <= 0 ) splitH = GetClientSize().y / 2;
//	int splitV = GuiConfig.splitterV;
//	if( splitV <= 0 ) splitV = GetClientSize().x / 2;

	hSplitter->SplitHorizontally( map, list, splitH );
//	hSplitter->SplitHorizontally( map, vSplitter, splitH );
//	vSplitter->SplitVertically( list, tree, splitV );
	map->Show( true );
	RefreshWindows();

	// Select the main game
	app->Select( app->m_game );
	app->UpdateSelection();
}

/**
 * Load a game file
 */
int GuiApp::LoadGame ( const char * FName )
{
	int ok = 0;
	wxBeginBusyCursor();
	if( FName && *FName )
	{
		GuiConfig.lastGameFile = FName;
		if( m_game )
			delete m_game;
		fileMenu->Enable( Gui_Game_Run, false );
		
		m_game = new Game;
		m_game->ModifyTablesPerRuleset();
		if( m_game->OpenGame( FName ) ) {
			PostLoadGame();
			frame->CreatePanes();
			Select( m_game );
			fileMenu->Enable( Gui_Game_Run, true );
			wxEndBusyCursor();
			return 1;
		}
	}
	wxMessageBox( "Couldn't open game", "Error" );
	wxEndBusyCursor();
	return 0;
}

/**
 * Extract orders from a text file
 */
int GuiApp::ParseOrder ( const char * FName )
{
	int fac = 0;

	AString ordersName;

	if( FName && *FName )
	{
		Aorders f;
		if( f.OpenByName( FName ) == -1 ) {
			wxMessageBox( "Couldn't open orders file", "Error" );
			wxEndBusyCursor();
			return 0;
		}

		Aoutfile file;

		AString *order = f.GetLine();
		while (order) {
			AString line( *order );
			AString * token = order->gettoken();

			if (token) {
				int i = Parse1Order(token);
				delete token;
				if( i == O_ATLANTIS ) {
					if( !fac ) {
						// start of a faction's orders
						token = order->gettoken();
						if (!token) {
							// no faction number given...
							f.Close();
							return 0;
						} else {

							fac = token->value();
							delete token;
							ordersName = AString( "orders." ) + fac;

							if( wxFileExists( ordersName.Str() ) )
								wxRemoveFile( ordersName.Str() );

							if( file.OpenByName( ordersName ) == -1 ) {
								fac = 0;
							}
						}
					}
				} else if( i == O_END ) {
					// end of factions orders
					delete order;
					break;
				}
			}
			if( fac )
				file.PutStr( line );

			order = f.GetLine();
		}
		if( fac ) {
			file.Close();
		}

		f.Close();
	} else {
		wxMessageBox( "Couldn't open orders file", "Error" );
		wxEndBusyCursor();
		return 0;
	}
	return fac;

}

/**
 * Add some stuff to make market/production selections quicker
 */
void GuiApp::PostLoadGame()
{
	forlist( &m_game->regions ) {
		ARegion * r = ( ARegion * ) elem;
		forlist( &r->markets ) {
			((Market *) elem)->region = r->num;
		}
		{
			forlist( &r->products ) {
				((Production *) elem)->region = r->num;
			}
		}
	}
}

/**
 * Save a game
 */
int GuiApp::SaveGame ()
{
	wxFileDialog dialog( frame, "Save Game File", "", "game.out",
						 "Game files (game.*)|game.*|"
					     "All files (*.*)|*.*",
						 wxSAVE | wxOVERWRITE_PROMPT );
	
	dialog.SetDirectory( wxGetCwd() );
	
	if( dialog.ShowModal() == wxID_OK ) {
		const char * fname = dialog.GetPath().c_str();
		if( wxFileExists( fname ) )
			wxRemoveFile( fname );

		wxBeginBusyCursor();
		
		if( fname && *fname ) {
			if( !m_game->SaveGame( fname ) ) {
				wxMessageBox( "Couldn't save game", "Error" );
				return 0;
				GuiConfig.lastGameFile = fname;
			}
		}
		
		wxEndBusyCursor();
		return 1;
	}

	return 0;
	
}

/**
 * Update status bar with region info
 */
void GuiApp::UpdateStatusBar( ARegion * pRegion )
{
	wxStatusBar * status = frame->GetStatusBar();
	
	if( pRegion ) {
		status->SetStatusText( wxString( " (" ) << pRegion->xloc << ", " 
		                       << pRegion->yloc << ")", 0 );
		status->SetStatusText( wxString( TerrainDefs[pRegion->type].name ) <<
		                       " in " << pRegion->name->Str(), 1 );
		if( pRegion->basepopulation )
			status->SetStatusText( wxString( " " ) << pRegion->population <<
			                       " (" << pRegion->basepopulation << ") " <<
								   ItemDefs[pRegion->race].names, 2 );
		else
			status->SetStatusText( _T( "" ),2 );

		status->SetStatusText( wxString( "$" ) << pRegion->money, 3 );
		if( pRegion->gate )
			status->SetStatusText( wxString( "Gate " ) << pRegion->gate, 4 );
		else
			status->SetStatusText( _T( "" ),4 );
	} else {
		for ( int i = 0; i < status->GetFieldsCount(); i++ ) {
			status->SetStatusText( _T( "" ),i );
		}
	}
}

/**
 * Used for debugging...
 */
void GuiApp::UpdateStatusBarDebug( int xpos, int ypos, int hexX, int hexY, int scrollX, int scrollY, int hexSize, int hexHeight )
{
	wxStatusBar * status = frame->GetStatusBar();

	status->SetStatusText( wxString( "(" ) << xpos << ", " 
		                   << ypos << ")", 0 );
	status->SetStatusText( wxString( " Hex (") << hexX << ", "
		                   << hexY << ")", 1 );
	status->SetStatusText( wxString( " Scroll (") << scrollX << ", "
		                   << scrollY << ")", 2 );
	status->SetStatusText( wxString( " HexSize (") << hexSize << ", "
		                   << hexHeight << ")", 3 );

}

/**
 * Remove an item from the game, and from the list and tree views
 */
void GuiApp::Trash( Unit * pUnit )
{
	{
		forlist( &pUnit->items ) {
			Item * i = ( Item * ) elem;
			pUnit->items.SetNum( i->type,0 );
		}
	}
	forlist( &pUnit->skills ) {
		Skill * s = ( Skill * ) elem;
		pUnit->skills.SetDays( s->type,0 );
	}
	
//	frame->tree->RemoveItem( pUnit );
	frame->list->RemoveItem( pUnit );
	int index = selectedElems->Index( pUnit );
	if( index != wxNOT_FOUND ) selectedElems->RemoveAt( index );
	TrashElemHistory( pUnit );
	pUnit->MoveUnit( 0 );
	delete pUnit;
}

/**
 * Remove an item from the game, and from the list and tree views
 */
void GuiApp::Trash( Faction * pFaction )
{
//	frame->tree->RemoveItem( pFaction );
	frame->list->RemoveItem( pFaction );
	
	int index = selectedElems->Index( pFaction );
	if( index != wxNOT_FOUND ) selectedElems->RemoveAt( index );
	
	m_game->factions.Remove( pFaction );
	forlist( &m_game->factions )
		( ( Faction * ) elem )->RemoveAttitude( pFaction->num );
	TrashElemHistory( pFaction );

	delete pFaction;
	
}

/**
 * Remove an item from the game, and from the list and tree views
 */
void GuiApp::Trash( Market * pMarket )
{
//	frame->tree->RemoveItem( pMarket );
	frame->list->RemoveItem( pMarket );
	
	ARegion * r = m_game->regions.GetRegion( pMarket->region );
	r->markets.Remove( pMarket );

	TrashElemHistory( pMarket );
	
	int index = selectedElems->Index( pMarket );
	if( index != wxNOT_FOUND ) selectedElems->RemoveAt( index );
}

/**
 * Remove an item from the game, and from the list and tree views
 */
void GuiApp::Trash( Production * pProduction )
{
//	frame->tree->RemoveItem( pProduction );
	frame->list->RemoveItem( pProduction );
	
	ARegion * r = m_game->regions.GetRegion( pProduction->region );
	r->products.Remove( pProduction );
	TrashElemHistory( pProduction );
	
	int index = selectedElems->Index( pProduction );
	if( index != wxNOT_FOUND ) selectedElems->RemoveAt( index );
}

/**
 * Remove an item from the game, and from the list and tree views
 */
void GuiApp::Trash( Object * pObject )
{
	ARegion * r = pObject->region;
	Object * dummy = r->GetDummy();
	forlist( &pObject->units ) {
		Unit * u = ( Unit * ) elem;
		u->MoveUnit( dummy );
	}
	
//	frame->tree->RemoveItem( pObject );
	frame->list->RemoveItem( pObject );
	
	int index = selectedElems->Index( pObject );
	if( index != wxNOT_FOUND ) selectedElems->RemoveAt( index );
	TrashElemHistory( pObject );
	
	r->objects.Remove( pObject );
	delete pObject;
}

/**
 * Remove an item from the game, and from the list and tree views
 */
//void GuiApp::Trash( ARegion * )
//{
//}

/**
 * Update faction entries in tree and list views
 */
void GuiApp::UpdateFactions()
{
	if( curSelection != SELECT_FACTION ) return;
	for( int i = 0; i < (int) selectedElems->GetCount(); i++ ) {
		Faction * f = (Faction *) selectedElems->Item(i);
//		frame->tree->UpdateItem(f);
		frame->list->UpdateItem(f);
	}
}

/**
 * Update region entries in tree and list views
 */
void GuiApp::UpdateRegions( bool updateMap )
{
	if( curSelection != SELECT_REGION ) return;
	for( int i = 0; i < (int) selectedElems->GetCount(); i++ ) {
		ARegion * r = (ARegion *) selectedElems->Item(i);
//		frame->tree->UpdateItem(r);
		frame->list->UpdateItem(r);
		if( updateMap )
			frame->map->Refresh();
	}
}

/**
 * Update unit entries in tree and list views
 */
void GuiApp::UpdateUnits()
{
	if( curSelection != SELECT_UNIT ) return;
	for( int i = 0; i < (int)selectedElems->GetCount(); i++ ) {
		Unit * u = (Unit *) selectedElems->Item(i);
//		frame->tree->UpdateItem(u);
		frame->list->UpdateItem(u);
	}
}

/**
 * Update object entries in tree and list views
 */
void GuiApp::UpdateObjects()
{
	if( curSelection != SELECT_OBJECT ) return;
	for( int i = 0; i < (int)selectedElems->GetCount(); i++ ) {
		Object * o = (Object *) selectedElems->Item(i);
//		frame->tree->UpdateItem(o);
		frame->list->UpdateItem(o);
	}
}

/**
 * Update market entries in tree and list views
 */
void GuiApp::UpdateMarkets()
{
	if( curSelection != SELECT_MARKET ) return;
	for( int i = 0; i < (int)selectedElems->GetCount(); i++ ) {
		Market * m = (Market *) selectedElems->Item(i);
//		frame->tree->UpdateItem(m);
		frame->list->UpdateItem(m);
	}
}

/**
 * Update product entries in tree and list views
 */
void GuiApp::UpdateProductions()
{
	if( curSelection != SELECT_PRODUCTION ) return;
	for( int i = 0; i < (int) selectedElems->GetCount(); i++ ) {
		Production * p = (Production *) selectedElems->Item(i);
//		frame->tree->UpdateItem(p);
		frame->list->UpdateItem(p);
	}
}

/**
 * Update level entries in tree and list views
 */
void GuiApp::UpdateLevels()
{
	if( curSelection != SELECT_LEVEL ) return;
//	frame->tree->UpdateItem( selectedLevel );
}

/**
 * Checks if the terrain has any races that are enabled
 */
bool GuiApp::TerrainHasEnabledRace( int ttype )
{
	//Check if terrain has any enabled races

	TerrainType * tt = &TerrainDefs[ttype];
	int i;
	int numRaces = sizeof( tt->races ) / sizeof( int );
	for( i = 0; i < numRaces; i++ ) {
		if( tt->races[i] == -1 ) continue;
		if( !( ItemDefs[tt->races[i]].flags & ItemType::DISABLED ) )
			return true;
	}
	
	numRaces = sizeof( tt->coastal_races ) / sizeof( int );
	for( i = 0; i < numRaces; i++ ) {
		if( tt->coastal_races[i] == -1 ) continue;
		if( !( ItemDefs[tt->coastal_races[i]].flags & ItemType::DISABLED ) )
			return true;
	}
	return false;
}

// ---------------------------------------------------------------------------
// GuiFrame
// ---------------------------------------------------------------------------

/**
 * Default constructor
 */
GuiFrame::GuiFrame( wxPoint & pos, wxSize & size )
		:wxFrame( ( wxFrame * ) NULL, -1, _T( "Atlantis Gui v 0.7.1" ), pos,
		           size, wxDEFAULT_FRAME_STYLE | /*wxHSCROLL | wxVSCROLL |*/
				   wxNO_FULL_REPAINT_ON_RESIZE )
{
	editor = new EditFrame( this, wxDefaultPosition, wxDefaultSize );
	editor->Init();
	selection = new SelectionCanvas( this );
	hSplitter = NULL;
//	vSplitter = NULL;
//	tree = NULL;
	map = NULL;
	list = NULL;
	hSplitter = new GuiSplitter( this );
//	vSplitter = new GuiSplitter( hSplitter );

	// Create toolbar
	CreateToolBar( wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL );
	GetToolBar()->SetBackgroundColour( app->guiColourDk );

	InitToolBar( GetToolBar() );

	// Accelerators
	wxAcceleratorEntry entries[6];
	entries[0].Set( wxACCEL_CTRL, ( int ) 'O', Gui_Game_Open );
	entries[1].Set( wxACCEL_CTRL, ( int ) 'S', Gui_Game_Save );
	entries[2].Set( wxACCEL_CTRL, ( int ) 'X', Gui_Quit );
	entries[3].Set( wxACCEL_CTRL, ( int ) 'A', Gui_About );
	entries[4].Set( wxACCEL_CTRL, ( int ) 'P', Gui_Game_Options );
	entries[5].Set( wxACCEL_CTRL, ( int ) 'R', Gui_Game_Run );
	wxAcceleratorTable accel( 5, entries );
	SetAcceleratorTable( accel );
}

/**
 * Default destructor
 */
GuiFrame::~GuiFrame()
{
	GuiConfig.windowMax = IsMaximized();
	if( !IsMaximized() ) {
		if( GetPosition().x > 0 )
			GuiConfig.windowX = GetPosition().x;
		if( GetPosition().y > 0 )
			GuiConfig.windowY = GetPosition().y;
		if( GetSize().x > 0 )
		GuiConfig.windowH = GetSize().x;
		if( GetSize().y > 0 )
		GuiConfig.windowV = GetSize().y;
	}
	if( hSplitter )
		GuiConfig.splitterH = hSplitter->GetSashPosition();
//	if( vSplitter )
//		GuiConfig.splitterV = vSplitter->GetSashPosition();
}

/**
 * Resize sub-windows if necessary
 */
void GuiFrame::RefreshWindows()
{
	int w, h;
	GetClientSize( &w, &h );

	int editWidth = 220;
	int editHeight = 450;

	if( editor )
		editor->SetSize( 0, 0, editWidth, editHeight );
	if( selection )
		selection->SetSize( 0, editHeight, editWidth, h - editHeight );

	hSplitter->SetSize( editWidth, 0, w - editWidth, h );
}

/**
 * Exit the GUI
 */
//void GuiFrame::OnClose( wxCloseEvent& event )
//{
//	event.Skip();
//}

/**
 * Exit the GUI
 */
void GuiFrame::OnQuit( wxCommandEvent& WXUNUSED( event ) )
{
	Close();
}

/**
 * Show program info
 */
void GuiFrame::OnAbout( wxCommandEvent& WXUNUSED( event ) )
{
	wxMessageBox( _T( "Atlantis GUI\n" )
	              _T( "Author: Ben Lloyd (c) 2004\n" )
	              _T( "About Atlantis GUI" ) );
}

/**
 * Refresh windows win frame resized
 */
void GuiFrame::OnSize( wxSizeEvent& event )
{
	RefreshWindows();
#ifdef __WXUNIVERSAL__	 
	event.Skip();
#endif

}

/**
 * Show options dialog
 */
void GuiFrame::OnGameOptions( wxCommandEvent & event )
{
	OptionsDialog options( this );
	options.ShowModal();
}

/**
 * Initialise toolbar
 */
void GuiFrame::InitToolBar( wxToolBar* toolBar )
{
	wxBitmap* bitmaps[6];
	
	bitmaps[0] = new wxBitmap( open_xpm );
	bitmaps[1] = new wxBitmap( save_xpm );
//	bitmaps[2] = new wxBitmap( help_xpm );
	
	toolBar->AddTool( Gui_Game_Open, "", *( bitmaps[0] ), _T( "Open file" ) );
	toolBar->AddTool( Gui_Game_Save, "", *( bitmaps[1] ), _T( "Save file" ) );
//	toolBar->AddSeparator();
//	toolBar->AddTool( Gui_Help", *( bitmaps[2] ), _T( "Help" ) );
	
	toolBar->EnableTool( Gui_Game_Save, false );
	toolBar->Realize();
	
	int i;
	for ( i = 0; i < 2; i++ )
		delete bitmaps[i];
}

/**
 * Get name of file to open, and load the game from it
 */
void GuiFrame::OnGameOpen( wxCommandEvent& WXUNUSED( event ) )
{
	AString s = GuiConfig.lastGameFile;
	if( s == "" ) {
		s = AString( "game." ) + Globals->WORLD_NAME + "." + 0;
	}
	wxFileDialog dialog( this, "Open Game File", "", s.Str(),
						 "Game files (game.*)|game.*|"
					     "All files (*.*)|*.*", wxOPEN );
	
	dialog.SetDirectory( wxGetCwd() );
	
	if( dialog.ShowModal() == wxID_OK )
		app->LoadGame( dialog.GetPath().c_str() );
}

/**
 * Save the game
 */
void GuiFrame::OnGameSave( wxCommandEvent& WXUNUSED( event ) )
{
	app->SaveGame();
}

/**
 * Show the run game dialog
 */
void GuiFrame::OnGameRun( wxCommandEvent& WXUNUSED( event ) )
{
	if( !app->m_game ) return;

	int oldFacSeq = app->m_game->factionseq;

	RunDialog run( this, app->runStep );
	int step = run.ShowModal();
	// What do we need to update?
	bool updateFacs = false;
	bool updateAll = false;
	if( app->runStep < RUN_STEP_READPLAYERS && step >= RUN_STEP_READPLAYERS )
		updateFacs = true;
	if( app->runStep < RUN_STEP_RUNORDERS && step >= RUN_STEP_RUNORDERS )
		updateAll = true;

	if( updateFacs && !updateAll && app->m_game->factionseq > oldFacSeq ) {
		forlist( &app->m_game->factions ) {
			Faction * f = ( Faction *) elem;
			if( f->num > oldFacSeq ) {
//				tree->AddItem( f );
				list->AddItem( f );
			}
		}
	}

	if( updateAll ) {
		CreatePanes();
	}

	if( step >= NUM_STEPS - 1 ) step = 0;

	app->runStep = step;
}

/**
 * Default constuctor
 */
GuiSplitter::GuiSplitter( wxWindow *parent )
            : wxSplitterWindow( parent, -1,
                                wxDefaultPosition, wxDefaultSize,
                                wxSP_3D | wxCLIP_CHILDREN)
{
}

void GuiSplitter::OnDoubleClickSash( int x, int y )
{
	// do nothing
}

void GuiFrame::EnableWindows( bool enable )
{
	if( map ) map->Enable( enable );
//	if( tree ) tree->Enable( enable );
	if( editor ) editor->Enable( enable );
	if( selection ) selection->Enable( enable );
	if(	list ) list->Enable( enable );
}

void GuiApp::TrashElemHistory( AListElem * element )
{
	forlist( &history ) {
		HistoryItem * h = ( HistoryItem * ) elem;
		for( int i = (int) selectedElems->GetCount() - 1; i >= 0; i-- ) {
			AListElem * elem = selectedElems->Item( i );
			if( elem == element ) {
				selectedElems->Remove( elem );
			}
		}
	}
}

/**
 * Create a text control with preceding label
 * If ppText2, id2 and label2 are specified, another control and label will
 *  be added alongside the first in the same sizer
 * Text box(es) and label(s) are added to sizerParent
 * If numeric is true, text box will take only numeric input. Otherwise will only
 *  take valid Atlantis characters
 */
void CreateControl( wxWindow * parent, wxTextCtrl ** ppText, wxWindowID id, const wxString & label,
				    wxSizer * sizerParent, const wxValidator & validator, wxTextCtrl ** ppText2,
					wxWindowID id2, const wxString & label2 )
{
	wxSizer *sizerRow;

	// Create label
	wxStaticText *lab = new wxStaticText( parent, -1, label );

	// Create text box
//	wxTextValidator * validator;
//	if( numeric )
//		validator = numStringValidator;
//	else
//		validator = textStringValidator;	
	wxTextCtrl *text = new wxTextCtrl( parent, id, "", wxDefaultPosition, wxDefaultSize, 0, validator );

	// Add to sizer
	wxSizer *sizerLeft = new wxBoxSizer( wxHORIZONTAL );
	sizerLeft->Add( lab, 2 );
	sizerLeft->Add( text, 3 );

	// Store textBox reference
	if( ppText )
		*ppText = text;

	// Add second set if necessary
	if( id2 != -1 ) {
		// Create controls
		lab = new wxStaticText( parent, -1, label2 );
		text = new wxTextCtrl( parent, id2, "", wxDefaultPosition, wxDefaultSize, 0, validator );

		// Put it all together
		wxSizer *sizerRight = new wxBoxSizer( wxHORIZONTAL );
		sizerRight->Add( lab, 2 );
		sizerRight->Add( text, 3 );
		sizerRow = new wxBoxSizer( wxHORIZONTAL );
		sizerRow->Add( sizerLeft, 8 );
		sizerRow->Add( 1, 1, 1 );
		sizerRow->Add( sizerRight, 8 );

		// Store second text box reference
		if( ppText2 )
			*ppText2 = text;
	} else {
		sizerRow = sizerLeft;
	}

	sizerParent->Add( sizerRow, 0, wxALL | wxGROW, 0 );
}

/**
 * Create a combo-box control with preceding label
 * Combo box and label are added to sizerParent
 * If sort == true, combo box will sort any items added to it
 */
void CreateControl( wxWindow * parent, wxComboBox ** ppComboBox, wxWindowID id, const wxString & label,
				    wxSizer * sizerParent, bool sort )
{
	// Create label
	wxStaticText *text = new wxStaticText( parent, -1, label );

	// Create comboBox
	long style = wxCB_DROPDOWN | wxCB_READONLY;
	if( sort )
		style = style | wxCB_SORT;
	wxComboBox *combo =	new wxComboBox( parent, id, "", wxDefaultPosition,
									   wxDefaultSize, 0 ,NULL, style );

	// Add to sizer
	wxSizer *sizerRow = new wxBoxSizer( wxHORIZONTAL );
	sizerRow->Add( text, 2 );
	sizerRow->Add( combo, 3 );
	sizerParent->Add( sizerRow, 0, wxALL | wxGROW, 0 );

	// Store comboBox reference
	if( ppComboBox )
		*ppComboBox = combo;

}

/**
 * Create a button control with preceding label
 * Button and label are added to sizerParent
 */
void CreateControl( wxWindow * parent, wxButton ** ppButton, wxWindowID id,
				    const wxString & label, wxSizer * sizerParent )
{
	// Create label
	wxStaticText *text = new wxStaticText( parent, -1, label );

	// Create button
	wxButton *button = new wxButton( parent, id, "", wxDefaultPosition );
	button->SetBackgroundColour( app->guiColourDk );

	// Add them to the sizer
	wxSizer *sizerRow = new wxBoxSizer( wxHORIZONTAL );
	sizerRow->Add( text, 2 );
	sizerRow->Add( button, 3 );
	sizerParent->Add( sizerRow, 0, wxALL | wxGROW, 0 );

	// Store the button reference
	if( ppButton )
		*ppButton = button;

}

/**
 * Create a button control, with label on button
 * Button is added to sizerParent, and can be placed on the
 *  left, right, or centre of the sizer (using align = wxALIGN_???)
 */
void CreateButton( wxWindow * parent, wxButton ** ppButton, wxWindowID id, const wxString & label,
				   wxSizer * sizerParent, int align )
{

	// Create button
	wxButton *button = new wxButton( parent, id, label, wxDefaultPosition );
	button->SetBackgroundColour( app->guiColourDk );

	// Add to sizer
	wxSizer *sizerRow = new wxBoxSizer( wxHORIZONTAL );
	if( align == wxALIGN_RIGHT || align == wxALIGN_CENTRE )
		sizerRow->Add( 1, 1, 2 );
	sizerRow->Add( button, 3, wxGROW, 5 );
	if( align == wxALIGN_LEFT || align == wxALIGN_CENTRE )
		sizerRow->Add( 1, 1, 2 );
	sizerParent->Add( sizerRow, 0, wxALL | wxGROW, 0 );

	// Store button reference
	if( ppButton )
		*ppButton = button;

}

/**
 * Create checkbox control
 * Checkbox is added to sizerParent
 */
void CreateControl( wxWindow * parent, wxCheckBox ** ppCheck, wxWindowID id, const wxString & label,
				    wxSizer * sizerParent )
{
	// Create checkbox
	wxCheckBox *check = new wxCheckBox( parent, id, label );
	check->SetBackgroundColour( app->guiColourLt );

	// Add to sizer
	wxSizer *sizerRow = new wxBoxSizer( wxHORIZONTAL );
	sizerRow->Add( check, 1 );
	sizerParent->Add( sizerRow, 0, wxALL | wxGROW, 0 );

	// Store checkbox reference
	if( ppCheck )
		*ppCheck = check;
}
