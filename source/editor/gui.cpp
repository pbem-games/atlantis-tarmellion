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

#include "bitmaps/open.xpm"
#include "bitmaps/save.xpm"
#include "bitmaps/help.xpm"
// #include "bitmaps/gui_big.xpm"
#include "bitmaps/gui_small.xpm"
#include "bitmaps/map.xpm"
#include "bitmaps/tree.xpm"

#include "gui.h"
#include "map.h"
#include "tree.h"

#include <math.h>
IMPLEMENT_APP( MyApp )

// ---------------------------------------------------------------------------
// global variables
// ---------------------------------------------------------------------------

MyFrame *frame = ( MyFrame * ) NULL;
MyApp *app = ( MyApp * ) NULL;

// ---------------------------------------------------------------------------
// event tables
// ---------------------------------------------------------------------------

BEGIN_EVENT_TABLE( MyFrame, wxMDIParentFrame )
	EVT_MENU( Gui_Options_Recreate, MyFrame::OnOptionsRecreate )
	EVT_MENU( Gui_Game_Open, MyFrame::OnGameOpen )
	EVT_MENU( Gui_Game_Save, MyFrame::OnGameSave )
	EVT_MENU( Gui_Show_Tree, MyFrame::OnShowTree )
	EVT_MENU( Gui_Show_Map, MyFrame::OnShowMap )
	EVT_MENU( Gui_About, MyFrame::OnAbout )
	EVT_MENU( Gui_Quit, MyFrame::OnQuit )

	EVT_CLOSE( MyFrame::OnClose )

	EVT_SIZE( MyFrame::OnSize )

END_EVENT_TABLE()

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// MyApp
// ---------------------------------------------------------------------------

MyApp::~MyApp()
{
	if( m_game )
		delete m_game;
}

bool MyApp::OnInit()
{
	// Create the main frame window
	
	frame = new MyFrame( ( wxFrame * )NULL, -1, _T( "Atlantis Gui Demo" ),
	                       wxPoint( -1, -1 ), wxSize( 500, 400 ),
		wxDEFAULT_FRAME_STYLE | 
		wxHSCROLL | wxVSCROLL );
	
	// Give it an icon
#ifdef __WXMSW__
	frame->SetIcon( wxIcon( _T( "gui_icn" ) ) );
#else
	frame->SetIcon( wxIcon( gui_small_xpm ) );
#endif
	
	// Set menubar
	wxMenu *file_menu = new wxMenu;
	
	file_menu->Append( Gui_Game_Open, _T( "&Open\tCtrl-O" ),
	                   _T( "Open new game" ) );
	file_menu->Append( Gui_Game_Save, _T( "&Save\tCtrl-S" ),
	                   _T( "Save game" ) );
	file_menu->Append( Gui_Quit, _T( "&Exit\tAlt-X" ),
	                   _T( "Quit the program" ) );

	wxMenu *options_menu = new wxMenu;
	options_menu->AppendCheckItem( Gui_Options_Recreate, _T( "&Recreate data *" ),
	                   _T( "Recreate data when marked fields are altered" ) );
	
	wxMenu *help_menu = new wxMenu;
	help_menu->Append( Gui_About, _T( "&About\tF1" ) );
	
	wxMenuBar *menu_bar = new wxMenuBar;
	
	menu_bar->Append( file_menu, _T( "&File" ) );
	menu_bar->Append( options_menu, _T( "&Options" ) );
	menu_bar->Append( help_menu, _T( "&Help" ) );
	
	frame->SetMenuBar( menu_bar );
	
	// Set statusbar
	frame->CreateStatusBar();
	int widths[5] = {50, -3, -1, -1, -1};
	frame->GetStatusBar()->SetFieldsCount( 5,widths );
	
	frame->Show( TRUE );
	frame->Maximize( true );
	
	SetTopWindow( frame );
	
	m_game = NULL;
	selectedFactions = new AElemArray();
	selectedRegions = new AElemArray();
	selectedMarkets = new AElemArray();
	selectedProductions = new AElemArray();
	selectedObjects = new AElemArray();
	selectedUnits = new AElemArray();
	
	app = this;
	
	// Open game file automatically if asked
	if( argc>1 ) {
		LoadGame( argv[1] );
	} else {
		if( wxFileExists( "game.in" ) )
			LoadGame( "game.in" );
	}

	recreateData = false;
	
	return TRUE;
}

ARegionList * MyApp::GetRegions()
{
	if( m_game )
		return &( m_game->regions );
	else
		return NULL;
}

AList * MyApp::GetFactions()
{
	if( m_game )
		return &( m_game->factions );
	else
		return NULL;
}

Faction * MyApp::AddFaction()
{
	Faction *temp = new Faction( m_game->factionseq );
	AString x( "NoAddress" );
	temp->SetAddress( x );
	temp->lastorders = m_game->TurnNumber();
	
	m_game->factions.Add( temp );
	m_game->factionseq++;
	return temp;
}

Unit * MyApp::AddUnit(Faction * pFaction)
{
	if( pFaction )
		return m_game->GetNewUnit(pFaction);

	forlist( &m_game->factions ) {
		Faction * f = ( Faction * ) elem;
		if( f->num == m_game->guardfaction )
			return m_game->GetNewUnit( f );
	}

	return NULL;
}

int MyApp::LoadGame ( const char * FName )
{
	int ok = 0;
	wxBeginBusyCursor();
	
	if( FName && *FName )
	{
		if( m_game )
			delete m_game;
		
		m_game = new Game;
		
		m_game->ModifyTablesPerRuleset();
		
		if( m_game->OpenGame( FName ) ) {
			if( !frame->tree ) {
				frame->tree = new TreeChild( frame, _T( "Tree View" ),
					wxPoint( -1, -1 ), wxSize( -1, -1 ),
					wxDEFAULT_FRAME_STYLE );
			}
 			frame->tree->InitTree();
			frame->tree->Show( true );
			if( !frame->map ) {
				frame->map = new MapChild( frame, _T( "Map Frame" ),
					wxPoint( -1, -1 ), wxSize( -1, -1 ),
					wxDEFAULT_FRAME_STYLE );
			} else {
				frame->map->InitPlane( 1 );
			}
			frame->map->Show( true );
			wxToolBar * toolBar = frame->GetToolBar();
			toolBar->EnableTool( Gui_Game_Save, true );
			toolBar->EnableTool( Gui_Show_Tree, true );
			toolBar->EnableTool( Gui_Show_Map, true );
			ok = 1;
		} else {
			wxMessageBox( "Couldn't open game", "Error" );
		}
	}
	
	wxEndBusyCursor();
	
	if( ok )
		return TRUE;
	else
		return FALSE;
}

int MyApp::SaveGame ( const char * FName )
{
	int ok = 0;
	wxBeginBusyCursor();
	
	if( FName && *FName )
	{
		if( m_game->SaveGame( FName ) ) {
			ok = 1;
		} else {
			wxMessageBox( "Couldn't save game", "Error" );
		}
	}
	
	wxEndBusyCursor();
	
	if( ok ) return TRUE;
	else return FALSE;
}

void MyFrame::AdjustSizers()
{
	
}

void MyApp::UpdateStatusBar( ARegion * pRegion )
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

void MyApp::SelectRegion( ARegion * pRegion, bool add )
{
	bool selected = false;
	int index = selectedRegions->Index( pRegion );
	if( index != wxNOT_FOUND ) selected = true;
	if( add ) {
		if( selected ) {
			selectedRegions->Remove( pRegion );
			frame->map->canvas->DeselectRegion( pRegion, true );
		} else {
			selectedRegions->Add( pRegion );
			frame->map->canvas->SelectRegion( pRegion, true );
		}
	} else {
		int num = abs( selectedRegions->GetCount() );
		for( int i = 0; i < num; i++ ) {
			ARegion * r = ( ARegion * ) selectedRegions->Item( i );
			frame->map->canvas->DeselectRegion( r, false );
		}
		selectedRegions->Clear();
		if( !selected || num > 1 ) {
			selectedRegions->Add( pRegion );
			frame->map->canvas->SelectRegion( pRegion, true );
		}
	}
	frame->tree->canvas->SelectItem( pRegion, add );
	frame->editor->ShowEditRegion();
}

void MyApp::SelectProduction( Production * pProduction, bool add )
{
	SelectElem( selectedProductions, pProduction, add );
	
	frame->tree->canvas->SelectItem( pProduction, add );
	frame->editor->ShowEditProduction();
}

void MyApp::SelectMarket( Market * pMarket, bool add )
{
	SelectElem( selectedMarkets, pMarket, add );
	
	frame->tree->canvas->SelectItem( pMarket, add );
	frame->editor->ShowEditMarket();
}

void MyApp::SelectFaction( Faction * pFaction, bool add )
{
	SelectElem( selectedFactions, pFaction, add );
	
	frame->tree->canvas->SelectItem( pFaction, add );
	frame->editor->ShowEditFaction();
}

void MyApp::SelectUnit( Unit * pUnit, bool add )
{
	SelectElem( selectedUnits, pUnit, add );
	
	frame->tree->canvas->SelectItem( pUnit, add );
	frame->editor->ShowEditUnit();
}

void MyApp::SelectObject( Object * pObject, bool add )
{
	SelectElem( selectedObjects, pObject, add );
	
	frame->tree->canvas->SelectItem( pObject, add );
	frame->editor->ShowEditObject();
}

void MyApp::DeselectAllRegions()
{
	for( int i=0; i < abs( selectedRegions->GetCount() ) -1; i++ ) {
		ARegion * r = ( ARegion * ) selectedRegions->Item( i );
		frame->map->canvas->DeselectRegion( r, false );
	}
	selectedRegions->Clear();
	frame->map->canvas->RedrawBorders();
	frame->editor->ShowEditRegion();
}

void MyApp::TrashUnit( Unit * pUnit )
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
	
	frame->tree->canvas->RemoveItem( pUnit );
	
	int index = selectedUnits->Index( pUnit );
	if( index != wxNOT_FOUND ) selectedUnits->RemoveAt( index );
	
	pUnit->MoveUnit( 0 );
	delete pUnit;
}

void MyApp::TrashFaction( Faction * pFaction )
{
	frame->tree->canvas->RemoveItem( pFaction );
	
	int index = selectedFactions->Index( pFaction );
	if( index != wxNOT_FOUND ) selectedFactions->RemoveAt( index );
	
	m_game->factions.Remove( pFaction );
	forlist( &m_game->factions )
		( ( Faction * ) elem )->RemoveAttitude( pFaction->num );
	delete pFaction;
	
}

void MyApp::TrashMarket( Market * pMarket )
{
	frame->tree->canvas->RemoveItem( pMarket );
	
	ARegion * r = m_game->regions.GetRegion( pMarket->region );
	r->markets.Remove( pMarket );
	
	int index = selectedMarkets->Index( pMarket );
	if( index != wxNOT_FOUND ) selectedMarkets->RemoveAt( index );
}

void MyApp::TrashProduction( Production * pProduction )
{
	frame->tree->canvas->RemoveItem( pProduction );
	
	ARegion * r = m_game->regions.GetRegion( pProduction->region );
	r->products.Remove( pProduction );
	
	int index = selectedProductions->Index( pProduction );
	if( index != wxNOT_FOUND ) selectedProductions->RemoveAt( index );
}

void MyApp::TrashObject( Object * pObject )
{
	ARegion * r = pObject->region;
	Object * dummy = r->GetDummy();
	forlist( &pObject->units ) {
		Unit * u = ( Unit * ) elem;
		u->MoveUnit( dummy );
	}
	
	frame->tree->canvas->RemoveItem(pObject);
	
	int index = selectedObjects->Index( pObject );
	if( index != wxNOT_FOUND ) selectedObjects->RemoveAt( index );
	
	r->objects.Remove( pObject );
	delete pObject;
}

//void MyApp::TrashRegion( ARegion * )
//{
//}

void MyApp::UpdateFactions()
{
	for( int i = 0; i < abs(selectedFactions->GetCount()); i++ ) {
		Faction * f = (Faction *) selectedFactions->Item(i);
		frame->tree->canvas->UpdateItem(f);
	}
}

void MyApp::UpdateRegions()
{
	for( int i = 0; i < abs(selectedRegions->GetCount()); i++ ) {
		ARegion * r = (ARegion *) selectedRegions->Item(i);
		frame->tree->canvas->UpdateItem(r);
	}
}

void MyApp::UpdateUnits()
{
	for( int i = 0; i < abs(selectedUnits->GetCount()); i++ ) {
		Unit * u = (Unit *) selectedUnits->Item(i);
		frame->tree->canvas->UpdateItem(u);
	}
}

void MyApp::UpdateObjects()
{
	for( int i = 0; i < abs(selectedObjects->GetCount()); i++ ) {
		Object * o = (Object *) selectedObjects->Item(i);
		frame->tree->canvas->UpdateItem(o);
	}
}

void MyApp::UpdateMarkets()
{
	for( int i = 0; i < abs(selectedMarkets->GetCount()); i++ ) {
		Market * m = (Market *) selectedMarkets->Item(i);
		frame->tree->canvas->UpdateItem(m);
	}
}

void MyApp::UpdateProductions()
{
	for( int i = 0; i < abs(selectedProductions->GetCount()); i++ ) {
		Production * p = (Production *) selectedProductions->Item(i);
		frame->tree->canvas->UpdateItem(p);
	}
}

bool MyApp::TerrainHasEnabledRace( int ttype )
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
// MyFrame
// ---------------------------------------------------------------------------

// Define my frame constructor
MyFrame::MyFrame( wxWindow *parent,
				 const wxWindowID id,
				 const wxString& title,
				 const wxPoint& pos,
				 const wxSize& size,
				 const long style )
				 : wxMDIParentFrame( parent, id, title, pos, size,
				 style | wxNO_FULL_REPAINT_ON_RESIZE )
{
	editor = new EditFrame( this, wxDefaultPosition, wxDefaultSize );
	editor->Init();
	tree = NULL;
	map = NULL;
	
	CreateToolBar( wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL );
	InitToolBar( GetToolBar() );
	
	// Accelerators
	wxAcceleratorEntry entries[4];
	entries[0].Set( wxACCEL_CTRL, ( int ) 'O', Gui_Game_Open );
	entries[1].Set( wxACCEL_CTRL, ( int ) 'S', Gui_Game_Save );
	entries[2].Set( wxACCEL_CTRL, ( int ) 'X', Gui_Quit );
	entries[3].Set( wxACCEL_CTRL, ( int ) 'A', Gui_About );
	wxAcceleratorTable accel( 3, entries );
	SetAcceleratorTable( accel );
}

void MyFrame::OnClose( wxCloseEvent& event )
{
	event.Skip();
}

void MyFrame::OnQuit( wxCommandEvent& WXUNUSED( event ) )
{
	Close();
}

void MyFrame::OnAbout( wxCommandEvent& WXUNUSED( event ) )
{
	wxMessageBox( _T( "Atlantis GUI Demo\n" )
	              _T( "Author: Ben Lloyd (c) 2003\n" )
	              _T( "Usage: gui.exe" ),
	              _T( "About GUI Demo" ) );
}

void MyFrame::OnSize( wxSizeEvent& event )
{
	int editWidth = 200;
	
	int w, h;
	GetClientSize( &w, &h );
	
	editor->SetSize( 0, 0, editWidth, h );
	GetClientWindow()->SetSize( editWidth, 0, w - editWidth, h );
	
#ifdef __WXUNIVERSAL__	 
	event.Skip();
#endif
}

void MyFrame::OnShowMap( wxCommandEvent & event )
{
	if( map ) {
		map->Activate();
	} else if( app->m_game ) {
		map = new MapChild( frame, _T( "Map Frame" ),
		                    wxPoint( -1, -1 ), wxSize( -1, -1 ),
		                    wxDEFAULT_FRAME_STYLE );
		map->Show();
		map->Activate();
	}
}

void MyFrame::OnShowTree( wxCommandEvent & event ) {
	if( tree ) {
		tree->Activate();
	} else if( app->m_game ) {
		tree = new TreeChild( frame, _T( "Tree View" ),
		                      wxPoint( -1, -1 ), wxSize( -1, -1 ),
		                      wxDEFAULT_FRAME_STYLE );
		tree->Show( true );
		tree->Activate();
	}
}

void MyFrame::OnOptionsRecreate( wxCommandEvent & event )
{
	wxMenuItem * item = GetMenuBar()->FindItem(Gui_Options_Recreate);

	app->recreateData = item->IsChecked();

}

void MyFrame::InitToolBar( wxToolBar* toolBar )
{
	wxBitmap* bitmaps[5];
	
	bitmaps[0] = new wxBitmap( open_xpm );
	bitmaps[1] = new wxBitmap( save_xpm );
	bitmaps[2] = new wxBitmap( help_xpm );
	bitmaps[3] = new wxBitmap( tree_xpm );
	bitmaps[4] = new wxBitmap( map_xpm );
	
	toolBar->AddTool( Gui_Game_Open, "", *( bitmaps[0] ), _T( "Open file" ) );
	toolBar->AddTool( Gui_Game_Save, "", *( bitmaps[1] ), _T( "Save file" ) );
	toolBar->AddSeparator();
	toolBar->AddTool( Gui_Show_Tree, "", *( bitmaps[3] ), _T( "Tree view" ) );
	toolBar->AddTool( Gui_Show_Map, "", *( bitmaps[4] ), _T( "Map view" ) );
	//	toolBar->AddSeparator();
	//	toolBar->AddTool( Gui_Help", *( bitmaps[2] ), _T( "Help" ) );
	
	toolBar->EnableTool( Gui_Game_Save, false );
	toolBar->EnableTool( Gui_Show_Tree, false );
	toolBar->EnableTool( Gui_Show_Map, false );
	toolBar->Realize();
	
	int i;
	for ( i = 0; i < 5; i++ )
		delete bitmaps[i];
}


void MyFrame::OnGameOpen( wxCommandEvent& WXUNUSED( event ) )
{
	wxFileDialog dialog( this, "Open Game File", "", "game.in",
						 "Game files (game.*)|game.*|"
					     "All files (*.*)|*.*",
						 wxOPEN | wxHIDE_READONLY );
	
	dialog.SetDirectory( wxGetCwd() );
	
	if( dialog.ShowModal() == wxID_OK )
		app->LoadGame( dialog.GetPath().c_str() );
}

void MyFrame::OnGameSave( wxCommandEvent& WXUNUSED( event ) )
{
	wxFileDialog dialog( this, "Save Game File", "", "game.out",
						 "Game files (game.*)|game.*|"
					     "All files (*.*)|*.*",
						 wxSAVE | wxOVERWRITE_PROMPT );
	
	dialog.SetDirectory( wxGetCwd() );
	
	if( dialog.ShowModal() == wxID_OK ) {
		const char * fname = dialog.GetPath().c_str();
		if( wxFileExists( fname ) )
			wxRemoveFile( fname );

		app->SaveGame( fname );
	}
}
