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
#include "edit.h"
#include "gui.h"

#include "gamedata.h"
#include "../aregion.h"
#include "../items.h"
#include "../astring.h"
#include "../object.h"
#include "../unit.h"
#include "../faction.h"
#include "editaux.h"
#include "movedlg.h"

#include "wx/toolbar.h"
#include <math.h>

#include "bitmaps/add.xpm"
#include "bitmaps/delete.xpm"
#include "bitmaps/move.xpm"
#include "bitmaps/add_market.xpm"
#include "bitmaps/add_production.xpm"
#include "bitmaps/add_object.xpm"
#include "bitmaps/add_unit.xpm"

BEGIN_EVENT_TABLE( EditFrame, wxWindow )

    EVT_SIZE( EditFrame::OnSize )

END_EVENT_TABLE()

BEGIN_EVENT_TABLE( EditPage, wxScrolledWindow )

	EVT_MENU( Edit_Tool_Add, EditPage::OnToolAdd )
	EVT_MENU( Edit_Tool_Delete, EditPage::OnToolDelete )
	EVT_MENU( Edit_Tool_Move, EditPage::OnToolMove )

    EVT_TEXT( Edit_Faction_Number, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Faction_Name, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Faction_Address, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Faction_Password, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Faction_Unclaimed, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Faction_LastChange, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Faction_LastOrders, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Faction_Times, EditPage::OnTextUpdate )
    EVT_COMBOBOX( Edit_Faction_Temformat, EditPage::OnTextUpdate )
    EVT_COMBOBOX( Edit_Faction_Quit, EditPage::OnTextUpdate )

    EVT_MENU( Edit_Tool_Add_Market, EditPage::OnButton )
    EVT_MENU( Edit_Tool_Add_Production, EditPage::OnButton )
    EVT_MENU( Edit_Tool_Add_Object, EditPage::OnButton )
    EVT_MENU( Edit_Tool_Add_Unit, EditPage::OnButton )

    EVT_TEXT( Edit_Level_Name, EditPage::OnTextUpdate )
    EVT_COMBOBOX( Edit_Level_LevelType, EditPage::OnTextUpdate )

//    EVT_TEXT( Edit_Region_Number, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Region_Name, EditPage::OnTextUpdate )
	EVT_BUTTON( Edit_Region_Type, EditPage::OnButton )
    EVT_TEXT( Edit_Region_Buildingseq, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Region_Gate, EditPage::OnTextUpdate )
    EVT_BUTTON( Edit_Region_Race, EditPage::OnButton )
    EVT_TEXT( Edit_Region_Pop, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Region_Basepop, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Region_Wages, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Region_Maxwages, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Region_Money, EditPage::OnTextUpdate )
    EVT_BUTTON( Edit_Region_Town, EditPage::OnButton )
    EVT_TEXT( Edit_Region_TownName, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Region_TownPop, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Region_TownBasepop, EditPage::OnTextUpdate )
    EVT_BUTTON( Edit_Region_Object, EditPage::OnButton )
    EVT_BUTTON( Edit_Region_N, EditPage::OnButton )
    EVT_BUTTON( Edit_Region_NE, EditPage::OnButton )
    EVT_BUTTON( Edit_Region_SE, EditPage::OnButton )
    EVT_BUTTON( Edit_Region_S, EditPage::OnButton )
    EVT_BUTTON( Edit_Region_SW, EditPage::OnButton )
    EVT_BUTTON( Edit_Region_NW, EditPage::OnButton )

	EVT_COMBOBOX( Edit_Market_Type, EditPage::OnTextUpdate )
    EVT_COMBOBOX( Edit_Market_Item, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Market_Price, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Market_Amount, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Market_Minpop, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Market_Maxpop, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Market_Minamt, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Market_Maxamt, EditPage::OnTextUpdate )

	EVT_COMBOBOX( Edit_Production_Itemtype, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Production_Baseamount, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Production_Amount, EditPage::OnTextUpdate )

	EVT_TEXT( Edit_Object_Name, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Object_Describe, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Object_Region, EditPage::OnTextUpdate )
    EVT_BUTTON( Edit_Object_Inner, EditPage::OnButton )
    EVT_TEXT( Edit_Object_Num, EditPage::OnTextUpdate )
    EVT_COMBOBOX( Edit_Object_Type, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Object_Incomplete, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Object_Runes, EditPage::OnTextUpdate )

    EVT_TEXT( Edit_Unit_Num, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Unit_Name, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Unit_Describe, EditPage::OnTextUpdate )
    EVT_BUTTON( Edit_Unit_Faction, EditPage::OnButton )
    EVT_COMBOBOX( Edit_Unit_Type, EditPage::OnTextUpdate )
    EVT_COMBOBOX( Edit_Unit_Guard, EditPage::OnTextUpdate )
    EVT_COMBOBOX( Edit_Unit_Reveal, EditPage::OnTextUpdate )
    EVT_COMBOBOX( Edit_Unit_Flags, EditPage::OnTextUpdate )
    EVT_BUTTON( Edit_Unit_Combat, EditPage::OnButton )
    EVT_BUTTON( Edit_Unit_ReadyItem, EditPage::OnButton )
    EVT_BUTTON( Edit_Unit_Items, EditPage::OnButton )
    EVT_BUTTON( Edit_Unit_Skills, EditPage::OnButton )
    EVT_BUTTON( Edit_Unit_ReadyWeapon, EditPage::OnButton )
    EVT_BUTTON( Edit_Unit_ReadyArmor, EditPage::OnButton )

    EVT_TEXT( Edit_Game_Month, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Game_Year, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Game_FactionSeq, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Game_UnitSeq, EditPage::OnTextUpdate )
    EVT_TEXT( Edit_Game_ShipSeq, EditPage::OnTextUpdate )
    EVT_BUTTON( Edit_Game_GuardFaction, EditPage::OnButton )
    EVT_BUTTON( Edit_Game_MonFaction, EditPage::OnButton )

    EVT_SIZE( EditPage::OnSize )

END_EVENT_TABLE()

// ---------------------------------------------------------------------------
// EditFrame
// ---------------------------------------------------------------------------

/**
 * Default constructor
 */
EditFrame::EditFrame( wxWindow *parent, const wxPoint& pos,
                     const wxSize& size )
          : wxWindow( parent, -1, pos, size,
                    wxDEFAULT_FRAME_STYLE, "Edit" )
{
	selectedElems = new AElemArray();
	curSelection = -1;
	selectedLevel = 0;
}

/**
 * Default destructor
 */
EditFrame::~EditFrame()
{
	delete selectedElems;
}

/**
 * Load edit pages
 */
void EditFrame::Init()
{
	wxSize size = GetClientSize();

	factionPage = new EditFactionPage( this, wxDefaultPosition, size );
	regionPage = new EditRegionPage( this,wxDefaultPosition,size );
	marketPage = new EditMarketPage( this,wxDefaultPosition,size );
	productionPage = new EditProductionPage( this,wxDefaultPosition,size );
	objectPage = new EditObjectPage( this,wxDefaultPosition,size );
	unitPage = new EditUnitPage( this,wxDefaultPosition,size );
	levelPage = new EditLevelPage( this,wxDefaultPosition,size );
	gamePage = new EditGamePage( this, wxDefaultPosition, size );

	SetBackgroundColour( app->guiColourLt );

	HideAllPages();
}

/**
 * Resize child windows
 */
void EditFrame::OnSize( wxSizeEvent& event )
{
	int width,height;

	GetClientSize( &width, &height );

	factionPage->SetSize( width,height );
	regionPage->SetSize( width,height );
	marketPage->SetSize( width,height );
	productionPage->SetSize( width,height );
	objectPage->SetSize( width,height );
	unitPage->SetSize( width,height );
	levelPage->SetSize( width,height );
	gamePage->SetSize( width,height );
}

/**
 * Hide all child pages
 */
void EditFrame::HideAllPages()
{
	factionPage->Move(-1000, -1000);
	regionPage->Move(-1000, -1000);
	marketPage->Move(-1000, -1000);
	productionPage->Move(-1000, -1000);
	objectPage->Move(-1000, -1000);
	unitPage->Move(-1000, -1000);
	levelPage->Move(-1000, -1000);
	gamePage->Move(-1000, -1000);
}

/**
 * Hide all pages except the shown page
 */
void EditFrame::ShowPage( EditPage * page )
{
	if( curPage == page ) return;

	if( page != factionPage )
		factionPage->Move(-1000, 0);

	if( page != regionPage )
		regionPage->Move(-1000, 0);

	if( page != marketPage )
		marketPage->Move(-1000, 0);

	if( page != productionPage )
		productionPage->Move(-1000, 0);

	if( page != objectPage )
		objectPage->Move(-1000, 0);

	if( page != unitPage )
		unitPage->Move(-1000, 0);

	if( page != levelPage )
		levelPage->Move(-1000, 0);

	if( page != gamePage )
		gamePage->Move(-1000, 0);

	page->Move( 0, 0 );
	curPage = page;
}

// ---------------------------------------------------------------------------
// EditPage
// ---------------------------------------------------------------------------

/**
 * Default constructor
 */
EditPage::EditPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
	     : wxScrolledWindow( parent, -1, pos, size, wxVSCROLL | wxHSCROLL )
{
	editWait = false;
	border = new wxStaticBox( this, -1, "" );
	
	sizerEdit = new wxStaticBoxSizer( border, wxVERTICAL );

	editTop = (EditFrame * ) parent;

	toolBar = new wxToolBar( this, -1, wxDefaultPosition, wxDefaultSize,
	                         wxTB_HORIZONTAL | wxNO_BORDER | wxTB_FLAT );

    wxBitmap* bitmaps[3];

    bitmaps[0] = new wxBitmap( add_xpm );
    bitmaps[1] = new wxBitmap( delete_xpm );
    bitmaps[2] = new wxBitmap( move_xpm );

	toolBar->AddTool( Edit_Tool_Add, "", *( bitmaps[0] ), "Add" );
	toolBar->AddTool( Edit_Tool_Delete, "", *( bitmaps[1] ), "Delete" );
	toolBar->AddTool( Edit_Tool_Move, "", *( bitmaps[2] ), "Move" );

    for( int i = 0; i < 3; i++ )
        delete bitmaps[i];

	border->SetBackgroundColour( app->guiColourLt );
	SetBackgroundColour( app->guiColourLt );
//	toolBar->SetBackgroundColour( app->guiColourLt );

    toolBar->Realize();

	sizerTool = new wxBoxSizer( wxHORIZONTAL );
	sizerTool->Add( toolBar, 1 );

	// Set validators
	wxStringList array;
	textStringValidator = new wxTextValidator( wxFILTER_INCLUDE_CHAR_LIST );
	for( char c = 127; c >= 0; c-- ) {
		if( islegal( c ) ) array.Add( wxString( c ) );
	}
	textStringValidator->SetIncludeList( array );
	numStringValidator = new wxTextValidator( wxFILTER_NUMERIC );

}

/**
 * Default destructor
 */
EditPage::~EditPage()
{
	delete border;
	delete sizerEdit;
	delete sizerTool;
	delete textStringValidator;
	delete numStringValidator;
}

void EditPage::EnableAllControls( bool b )
{
}

void EditPage::ClearAllControls()
{
}

void EditPage::OnTextUpdate( wxCommandEvent & event )
{
}

void EditPage::OnButton( wxCommandEvent & event )
{
}

/**
 * Size all sizers and scrollbars correctly
 */
void EditPage::OnSize( wxSizeEvent & event )
{
	int w,h;
	GetClientSize( &w, &h );
	if( w>0 && h>0 ) {
		int offset = toolBar->GetSize().y;
		SetScrollbars( 5, 10, w/5, h /10 );
		sizerEdit->SetDimension( 1, offset + 1, w-1, h-5 - offset );
		sizerTool->SetDimension( 1, 1, w-1, offset - 1 );
	}
}

/**
 * Add data to a control. Change to (...) if the data is different from the current value
 */
int EditPage::AddToControl( wxTextCtrl * pText, long number )
{
	if( pText->GetValue() == "(...)" )
		return 0;
	if( pText->GetValue() == "" ) {
		*pText << number;
		return 1;
	} else {
		long num;
		pText->GetValue().ToLong( &num );
		if( num != number ) {
			pText->SetValue( "(...)" );
			return 0;
		}
	}
	return 1;
}

/**
 * Add data to a control. Change to (...) if the data is different from the current value
 */
int EditPage::AddToControl( wxTextCtrl * pText, const char * string )
{
	if( pText->GetValue() == "(...)" )
		return 0;

	if( pText->GetValue() == "" ) {
		pText->SetValue( string );
		return 1;
	} else if( pText->GetValue() != string ) {
		pText->SetValue( "(...)" );
		return 0;
	}
	return 1;
}

/**
 * Add data to a control. Change to (...) if the data is different from the current value
 */
int EditPage::AddToControl( wxButton * pButton, const char * string )
{
	if( pButton->GetLabel() == "(...)" )
		return 0;

	if( pButton->GetLabel() == "" ) {
		pButton->SetLabel( string );
		return 1;
	} else if( !string || pButton->GetLabel() != string ) {
		pButton->SetLabel( "(...)" );
		return 0;
	}
	return 1;

}

/**
 * Add data to a control. Change to (...) if the data is different from the current value
 */
int EditPage::AddToControl( wxComboBox * pCombo, int index )
{
	int i = pCombo->GetSelection();
	int j;
	int count = pCombo->GetCount();

	if( i == -1 ) {
		for( j = 0; j < count -1; j++ ) {
			if( pCombo->GetClientData( j ) == NULL ) continue;
			if( *( int * )pCombo->GetClientData( j ) == index ) {
				pCombo->SetSelection( j );
				return 1;
			}
		}
	}

	if( pCombo->GetClientData( i ) == NULL ) return 0;

	if( *( int * )pCombo->GetClientData( i ) == index ) return 1;

	for( j = 0; j < count -1; j++ ) {
		if( pCombo->GetClientData( j ) == NULL ) {
			pCombo->SetSelection( j );
			return 0;
		}
	}
	return 1;
}

/**
 * Update an AString with the value of the control
 */
void EditPage::UpdateControl( wxTextCtrl * pText, AString * str )
{
	if( pText->GetValue() != "" )  {
		*str = pText->GetValue().GetData();
	}
}

/**
 * Update an int with the value of the control
 */
void EditPage::UpdateControl( wxTextCtrl * pText, int & num )
{
 	long number;
	if( pText->GetValue() != "" ) {
		pText->GetValue().ToLong( &number );
		num = number;
	}
}

/**
 * Update an int with the value of the control
 */
void EditPage::UpdateControl( wxTextCtrl * pText, unsigned int & num )
{
 	long number;
	if( pText->GetValue() != "" ) {
		pText->GetValue().ToLong( &number );
		num = number;
	}
}

/**
 * Update an int with the value of the control
 */
void EditPage::UpdateControl( wxComboBox * pCombo, int & num )
{
	int i = pCombo->GetSelection();
	if( pCombo->GetClientData( i ) == NULL ) return;
	num = *( int * ) pCombo->GetClientData( i );
}

/**
 * Delete all the data held by a combo-box
 */
void EditPage::DeleteCombo( wxComboBox * pCombo )
{
	for( int i = 0; i < pCombo->GetCount(); i++ )
		if( pCombo->GetClientData( i ) )
			delete ( int * ) pCombo->GetClientData( i );
}

/**
 * Get a single region choice from user
 */
ARegion * EditPage::GetRegionChoice( const wxString & message, const wxString & caption )
{
	int num = app->m_game->regions.Num();
	wxArrayString choices;
	choices.Alloc( num );
	AElemArray regions;
	int count = 0;
	forlist( &app->m_game->regions ) {
		ARegion * r = ( ARegion * ) elem;
		AString temp = AString( "( " ) + r->xloc + ", " + r->yloc + ", " +
						   r->zloc + " ) " + r->name->Str();
		choices.Add( temp.Str() );
		regions.Add( r );
		count++;
	}

	int index =  wxGetSingleChoiceIndex( "Select region to move to.",
							  "Move", choices, this );

	if( index == -1 )
		return NULL;
	else
		return ( ARegion * ) regions[index];
}

void EditPage::OnToolAdd( wxCommandEvent & event )
{
}

void EditPage::OnToolDelete( wxCommandEvent & event )
{
}

void EditPage::OnToolMove( wxCommandEvent & event )
{
}


// ---------------------------------------------------------------------------
// EditRegionPage
// ---------------------------------------------------------------------------

/**
 * Default constructor. Creates all necessary controls
 */
EditRegionPage::EditRegionPage( wxWindow *parent, const wxPoint& pos,
                                const wxSize& size )
               : EditPage( parent, pos, size )
{
//	CreateControl( this, &editNumber, Edit_Region_Number, "Number: ", true );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editName, Edit_Region_Name, "Name: ", sizerEdit, *textStringValidator );

	CreateControl( this, &editType, Edit_Region_Type, "Type: *", sizerEdit );
	CreateControl( this, &editRace, Edit_Region_Race, "Race: ", sizerEdit );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editMoney, Edit_Region_Money, "Money: ", sizerEdit, *numStringValidator );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editGate, Edit_Region_Gate, "Gate: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editBuildingseq, Edit_Region_Buildingseq, "Building seq: ", sizerEdit, *numStringValidator );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

//	CreateControl( this, &editPop, Edit_Region_Pop, "Pop: ", true );
//	CreateControl( this, &editBasepop, Edit_Region_Basepop, "Base Pop: ", true );
	CreateControl( this, &editPop, Edit_Region_Pop, "Pop: ", sizerEdit, *numStringValidator,
				   &editBasepop, Edit_Region_Basepop, "Base: " );

//	CreateControl( this, &editWages, Edit_Region_Wages, "Wages: ", true );
//	CreateControl( this, &editMaxwages, Edit_Region_Maxwages, "Max Wages: ", true );
	CreateControl( this, &editWages, Edit_Region_Wages, "Wages: ", sizerEdit, *numStringValidator,
				   &editMaxwages, Edit_Region_Maxwages, "Max: " );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );


	// Town
	CreateButton( this, &editTown, Edit_Region_Town, "Town...", sizerEdit, wxALIGN_LEFT);
	
	CreateControl( this, &editTownName, Edit_Region_TownName, " Name: ", sizerEdit, *textStringValidator );
//	CreateControl( this, &editTownPop, Edit_Region_TownPop, " Pop: ", true );
//	CreateControl( this, &editTownBasepop, Edit_Region_TownBasepop, " Base Pop: ", true );
	CreateControl( this, &editTownPop, Edit_Region_TownPop, " Pop: ", sizerEdit, *numStringValidator, 
				   &editTownBasepop, Edit_Region_TownBasepop, " Base: " );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	//Exits
	wxSizer * sizerRow = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText * text = new wxStaticText( this, -1, "Exits:" );
	sizerRow->Add( 1,1,1 );
	sizerRow->Add( text, 0, wxALIGN_CENTRE_VERTICAL );
	sizerRow->Add( 1,1,1 );
	sizerEdit->Add( sizerRow, 0, wxALL | wxGROW );

	editExits[D_NORTH] = new wxButton( this, Edit_Region_N, "", wxDefaultPosition );
	editExits[D_NORTHEAST] = new wxButton( this, Edit_Region_NE, "", wxDefaultPosition );
	editExits[D_SOUTHEAST] = new wxButton( this, Edit_Region_SE, "", wxDefaultPosition );
	editExits[D_SOUTH] = new wxButton( this, Edit_Region_S, "", wxDefaultPosition );
	editExits[D_SOUTHWEST] = new wxButton( this, Edit_Region_SW, "", wxDefaultPosition );
	editExits[D_NORTHWEST] = new wxButton( this, Edit_Region_NW, "", wxDefaultPosition );

	for( int i = 0; i < NDIRS; i++ )
		editExits[i]->SetBackgroundColour( app->guiColourDk );
		
	sizerRow = new wxBoxSizer( wxHORIZONTAL );
	sizerRow->Add( 1,1,1 );
	sizerRow->Add( editExits[D_NORTH], 2 );
	sizerRow->Add( 1,1,1 );
	sizerEdit->Add( sizerRow, 0, wxALL | wxGROW, 0 );

	sizerRow = new wxBoxSizer( wxHORIZONTAL );
	sizerRow->Add( editExits[D_NORTHWEST], 2 );
	sizerRow->Add( 1,1,1 );
	sizerRow->Add( editExits[D_NORTHEAST], 2 );
	sizerEdit->Add( sizerRow, 0, wxALL | wxGROW, 0 );

	sizerRow = new wxBoxSizer( wxHORIZONTAL );
	sizerRow->Add( editExits[D_SOUTHWEST], 2 );
	sizerRow->Add( 1,1,1 );
	sizerRow->Add( editExits[D_SOUTHEAST], 2 );
	sizerEdit->Add( sizerRow, 0, wxALL | wxGROW, 0 );

	sizerRow = new wxBoxSizer( wxHORIZONTAL );
	sizerRow->Add( 1,1,1 );
	sizerRow->Add( editExits[D_SOUTH], 2 );
	sizerRow->Add( 1,1,1 );
	sizerEdit->Add( sizerRow, 0, wxALL | wxGROW, 0 );

	toolBar->AddSeparator();
	toolBar->AddTool( Edit_Tool_Add_Market, "",
					  wxBitmap( add_market_xpm ), "Add market" );
	toolBar->AddTool( Edit_Tool_Add_Production, "",
					  wxBitmap( add_production_xpm ), "Add production" );
	toolBar->AddTool( Edit_Tool_Add_Object, "",
					  wxBitmap( add_object_xpm ), "Add object" );
	toolBar->AddTool( Edit_Tool_Add_Unit, "",
					  wxBitmap( add_unit_xpm ), "Add unit" );

	toolBar->Realize();

	toolBar->EnableTool( Edit_Tool_Add, false );
	toolBar->EnableTool( Edit_Tool_Delete, false );
	toolBar->EnableTool( Edit_Tool_Move, false );

}

/**
 * Default destructor
 */
EditRegionPage::~EditRegionPage()
{
}

/** 
 * Enable/Disable all controls on page
 */
void EditRegionPage::EnableAllControls( bool b )
{
//	editNumber->Enable( b );
	editName->Enable( b );
	editType->Enable( b );
	editBuildingseq->Enable( b );
	editGate->Enable( b );
	editRace->Enable( b );
	editPop->Enable( b );
	editBasepop->Enable( b );
	editWages->Enable( b );
	editMaxwages->Enable( b );
	editMoney->Enable( b );
	editTown->Enable( b );
	editTownName->Enable( b );
	editTownPop->Enable( b );
	editTownBasepop->Enable( b );
	for( int i = 0; i < NDIRS; i++ )
		editExits[i]->Enable( b );
}

/**
 * Clear all controls on page
 */
void EditRegionPage::ClearAllControls()
{
	border->SetTitle( "Region" );
//	editNumber->Clear();
	editName->Clear();
	editType->SetLabel( "" );
	editBuildingseq->Clear();
	editGate->Clear();
	editRace->SetLabel( "" );
	editPop->Clear();
	editBasepop->Clear();
	editWages->Clear();
	editMaxwages->Clear();
	editMoney->Clear();
	editTown->SetLabel( "Town..." );
	editTownName->Clear();
	editTownPop->Clear();
	editTownBasepop->Clear();
	for( int i = 0; i < NDIRS; i++ )
		editExits[i]->SetLabel( "" );

}

/**
 * Process event when a textbox is updated.
 * Update the actual game data, and the tree/list view display
 *  for the updated data for the current selection
 */
void EditRegionPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;
	bool updateMap = false;
	for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
		ARegion * r = ( ARegion * ) editTop->selectedElems->Item( i );
	
		switch ( event.GetId() )
		{
//		case Edit_Region_Number:
//			UpdateControl( editNumber, r->num );
//			break;
		case Edit_Region_Name:
			UpdateControl( editName, r->name );
			break;
		case Edit_Region_Buildingseq:
			UpdateControl( editBuildingseq, r->buildingseq );
			break;
		case Edit_Region_Gate:
			UpdateControl( editGate, r->gate );
			break;
		case Edit_Region_Pop:
			UpdateControl( editPop, r->population );
			break;
		case Edit_Region_Basepop:
			UpdateControl( editBasepop, r->basepopulation );
			break;
		case Edit_Region_Wages:
			UpdateControl( editWages, r->wages );
			break;
		case Edit_Region_Maxwages:
			UpdateControl( editMaxwages, r->maxwages );
			break;
		case Edit_Region_Money:
			UpdateControl( editMoney, r->money );
			break;
		case Edit_Region_TownName:
			if( r->town ) {
				UpdateControl( editTownName, r->town->name );
				updateMap = true;
			}
			break;
		case Edit_Region_TownPop:
			if( r->town ) {
				UpdateControl( editTownPop, r->town->pop );
				updateMap = true;
			}
			break;
		case Edit_Region_TownBasepop:
			if( r->town ) {
				UpdateControl( editTownBasepop, r->town->basepop );
			}
			break;
		}
	}
	app->UpdateRegions( updateMap );
}

/**
 * Process event when a button is updated.
 */
void EditRegionPage::OnButton( wxCommandEvent & event )
{
	if( editWait )
		return;

	int dir = -1;
	int x, y, width, height;
	GetParent()->GetSize( &width, &height );
	GetParent()->GetPosition( &x, &y );
	GetParent()->ClientToScreen( &x, &y );

	AElemArray * tempSelection = new AElemArray();
	int selectType = -1;
	// Todo: selec all the items after addition
	if( event.GetId() == Edit_Tool_Add_Market ) {
		// Add a market to selected regions
		for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
			ARegion * r = ( ARegion * ) editTop->selectedElems->Item( i );
			Market * m = new Market( M_BUY, I_SILVER, 0, 0, 0, 0, 0, 0 );
			r->markets.Add( m );
			m->region = r->num;
//			frame->tree->AddItem( m );
			tempSelection->Add( m );
			selectType = SELECT_MARKET;
//			frame->tree->UpdateItem( r, true );
		}
	} else if( event.GetId() == Edit_Tool_Add_Production ) {
		// Add a product to selected regions
		for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
			ARegion * r = ( ARegion * ) editTop->selectedElems->Item( i );
			Production * p = new Production( I_SILVER ,0 );
			r->products.Add( p );
			p->region = r->num;
//			frame->tree->AddItem( p );
			tempSelection->Add( p );
			selectType = SELECT_PRODUCTION;
		}
	} else if( event.GetId() == Edit_Tool_Add_Object ) {
		// Add an object to selected regions
		for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
			ARegion * r = ( ARegion * ) editTop->selectedElems->Item( i );
			Object * o = new Object( r );
			o->num = r->buildingseq++;
			o->name = new AString( AString( ObjectDefs[O_DUMMY].name ) + " [" + o->num + "]" );
			r->objects.Add( o );
//			frame->tree->AddItem( o, false );
			tempSelection->Add( o );
			selectType = SELECT_OBJECT;
		}
	} else if( event.GetId() == Edit_Tool_Add_Unit ) {
		// Add a unit to selected regions
		for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
			ARegion * r = ( ARegion * ) editTop->selectedElems->Item( i );
			Unit * u = app->AddUnit();
			u->MoveUnit( r->GetDummy() );
//			frame->tree->AddItem( u );
			tempSelection->Add( u );
			selectType = SELECT_UNIT;
		}
	} else if( event.GetId() == Edit_Region_Type ) {
		// Open region selection window

		//need bigger window for this one
		EditTerrainTypeAux aux( this, wxPoint( x + width + 3, y + 3 ), wxSize( 300, frame->map->GetSize().y - 2 ) );

		aux.Init( editTop->selectedElems );

		// Update edit window and objects
		if( aux.ShowModal() == wxID_OK ) {
			editType->SetLabel( "" );
			if( aux.selectedArray.GetCount()>0 ) {
				for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
					ARegion * r = ( ARegion * ) editTop->selectedElems->Item( i );

					int type = ( ( ARegion * ) aux.selectedArray[0] )->type;

					if( type == -1 ) {
						break;
					}
					AddToControl( editType, TerrainDefs[type].name );
					r->type = type;
					// recreate region data?
					if( GuiConfig.recreateData ) {
						// delete markets
//						forlist( &r->markets ) {
//							Market * m = ( Market * ) elem;
//							app->Trash( m );
//						}
						r->markets.DeleteAll();

						// delete products
//						{
//							forlist( &r->products ) {
//								Production * p = ( Production * ) elem;
//								app->Trash( p );
//							}
//						}
						r->products.DeleteAll();

						r->SetupProds();

						int towns = Globals->TOWNS_EXIST;
						Globals->TOWNS_EXIST = 0;

						if( app->TerrainHasEnabledRace( type ) ) {
							r->SetupPop();
						} else {
							r->race = -1;
							r->population = 0;
							r->basepopulation = 0;
							r->wages = 0;
							r->maxwages = 0;
							r->money = 0;
						}

						Globals->TOWNS_EXIST = towns;
						if( r->town )
							r->SetupCityMarket();
		
//						frame->tree->UpdateItem( r, true );
					}
				}
				if( GuiConfig.recreateData ) {
					ReloadPage();
				}
			}
			app->UpdateRegions();
		}
	} else if( event.GetId() == Edit_Region_Race ) {

		//open item selection window

		EditItemTypeAux aux( this, wxPoint( x + width + 3, y + 3 ), wxSize( 250, frame->map->GetSize().y - 2 ) );

		Item * it = NULL;
		AElemArray selected;
		for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
			ARegion * r = ( ARegion * ) editTop->selectedElems->Item( i );
			it = new Item();
			it->type = r->race;
			selected.Add( it );
		}
		aux.Init( &selected, IT_MAN );

		if( aux.ShowModal() == wxID_OK ) {
			//update edit window and objects
			editRace->SetLabel( "" );
			if( aux.selectedArray.GetCount()>0 ) {
				for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
					ARegion * r = ( ARegion * ) editTop->selectedElems->Item( i );

					int type = ( ( Item * ) aux.selectedArray[0] )->type;
					r->race = type;
					AddToControl( editRace, ItemDefs[type].names );
				}
			}
			app->UpdateRegions( false );
			WX_CLEAR_ARRAY( selected );
		}
	} else if( event.GetId() == Edit_Region_Town ) {
		// Add/Delete a town
		ARegion * r = (ARegion *) editTop->selectedElems->Item( 0 );
		if( r->town ) {
			// Delete the town?
			int answer = wxMessageBox( "Delete town?", "Confirm",
			                           wxOK | wxCANCEL | wxCENTRE |
									   wxICON_QUESTION, frame);
			if( answer == wxOK ) {
				delete r->town;
				r->town = 0;
				if( GuiConfig.recreateData ) {
		//			forlist( &r->markets ) {
		//				Market * m = (Market *) elem;
		//				if( m->item < 0 ) continue;
		//				if( ItemDefs[m->item].type & IT_MAN ) continue;
		//				app->Trash( m );
		//				delete m;
		//			}
					r->markets.DeleteAll();
				}
//				frame->tree->UpdateItem( r, true );
				app->UpdateRegions();
			}
		} else {
			// Add a town?
			if( GuiConfig.recreateData ) {
				// Recreate population/market data
				int minpop, maxpop;
				wxArrayString choices;
				choices.Add("Random");
				choices.Add("Village");
				choices.Add("Town");
				choices.Add("City");

				wxString answer = wxGetSingleChoice( "What size town do you wish to create?",
													 "Choose town size",
													 choices );

				if( answer != "" ) {

					if( answer == "City" ) {
						minpop = Globals->CITY_POP / 2;
						maxpop = Globals->CITY_POP;
					} else if( answer == "Town" ) {
						minpop = Globals->CITY_POP / 4;
						maxpop = Globals->CITY_POP / 2;
					} else if( answer == "Village" ) {
						minpop = Globals->CITY_POP / 8;
						maxpop = Globals->CITY_POP / 4;
					} else {
						minpop = -1;
						maxpop = -1;
					}

					r->AddTown(0, minpop, maxpop);
				}
			} else {
				r->town = new TownInfo;
			}

			if( r->town ) {
				r->town->name = new AString("NoName");
				app->UpdateRegions();
			}
		}
		ReloadPage();
//		frame->tree->UpdateItem( r, true );
	} else if( event.GetId() == Edit_Region_N ) {
		dir = D_NORTH;
	} else if( event.GetId() == Edit_Region_NE ) {
		dir = D_NORTHEAST;
	} else if( event.GetId() == Edit_Region_SE ) {
		dir = D_SOUTHEAST;
	} else if( event.GetId() == Edit_Region_S ) {
		dir = D_SOUTH;
	} else if( event.GetId() == Edit_Region_SW ) {
		dir = D_SOUTHWEST;
	} else if( event.GetId() == Edit_Region_NW ) {
		dir = D_NORTHWEST;
	}

	if( dir != -1 ) {
		// Edit neighbour pointer
		EditRegionAux aux( this, wxPoint( x + width + 3, y + 3 ), wxSize( 250, frame->map->GetSize().y - 2 ) );

		AElemArray selected;
		for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
			ARegion * r = ( ARegion * ) editTop->selectedElems->Item( i );
			selected.Add( r->neighbors[dir] );
		}
		aux.Init( &selected );

		if( aux.ShowModal() == wxID_OK ) {
			//update edit window and objects
			if( aux.selectedArray.GetCount()>0 ) {
				editExits[dir]->SetLabel( "" );
				for( int i = 0; i < (int) editTop->selectedElems->GetCount() ; i++ ) {
					ARegion * r = ( ARegion * ) editTop->selectedElems->Item( i );
					ARegion * r2 = ( ARegion * ) aux.selectedArray[0];
					if( r2->num == -1 ) {
						r->neighbors[dir] = 0;
						continue;
					}
					r->neighbors[dir] = r2;
					AString temp = AString( "( " ) + r2->xloc + ", " + r2->yloc + ", "
								   + r2->zloc + " )";
					editExits[dir]->SetLabel( temp.Str() );
				}
			}
		}
		selected.Clear();
		aux.selectedArray.Clear();
	}

	if( selectType != -1 ) {
		app->UpdateSelection( tempSelection, selectType );
	}
	delete tempSelection;
	
}

void EditRegionPage::OnToolAdd( wxCommandEvent & event )
{
}

void EditRegionPage::OnToolDelete( wxCommandEvent & event )
{
}

void EditRegionPage::OnToolMove( wxCommandEvent & event )
{
}

// ---------------------------------------------------------------------------
// EditLevelPage
// ---------------------------------------------------------------------------

/**
 * Default constructor. Creates all necessary controls
 */
EditLevelPage::EditLevelPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );
	CreateControl( this, &editName, Edit_Level_Name, "Name: ", sizerEdit, *textStringValidator );
	CreateControl( this, &editLevelType, Edit_Level_LevelType, "Type: ", sizerEdit );

	editLevelType->Append( "Nexus", new int( ARegionArray::LEVEL_NEXUS ) );
	editLevelType->Append( "Surface", new int( ARegionArray::LEVEL_SURFACE ) );
	editLevelType->Append( "Underworld", new int( ARegionArray::LEVEL_UNDERWORLD ) );
	editLevelType->Append( "Underdeep", new int( ARegionArray::LEVEL_UNDERDEEP ) );
	editLevelType->Append( "(...)" );

	toolBar->EnableTool( Edit_Tool_Add, false );
	toolBar->EnableTool( Edit_Tool_Delete, false );
	toolBar->EnableTool( Edit_Tool_Move, false );
}

/**
 * Default destructor
 */
EditLevelPage::~EditLevelPage()
{
	DeleteCombo( editLevelType );
}

/** 
 * Enable/Disable all controls on page
 */
void EditLevelPage::EnableAllControls( bool b )
{
	editName->Enable( b );
	editLevelType->Enable( b );
}

/**
 * Clear all controls on page
 */
void EditLevelPage::ClearAllControls()
{
	border->SetTitle( "Level" );
	editName->Clear();
	editLevelType->SetSelection( -1 );
}

/**
 * Process event when a textbox is updated.
 * Update the actual game data, and the tree/list view display
 *  for the updated data for the current selection
 */
void EditLevelPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;

	ARegionArray * level = editTop->selectedLevel;
	
	switch ( event.GetId() )
	{
		case Edit_Level_Name:
			if( !level->strName )
				level->strName = new AString();
			UpdateControl( editName, level->strName );
			break;
		case Edit_Level_LevelType:
			UpdateControl( editLevelType, level->levelType );
			break;
	}
	app->UpdateLevels();
}

void EditLevelPage::OnButton( wxCommandEvent & event )
{
}

void EditLevelPage::OnToolAdd( wxCommandEvent & event )
{
}

void EditLevelPage::OnToolDelete( wxCommandEvent & event )
{
}

void EditLevelPage::OnToolMove( wxCommandEvent & event )
{
}

// ---------------------------------------------------------------------------
// EditFactionPage
// ---------------------------------------------------------------------------

/**
 * Default constructor. Creates all necessary controls
 */
EditFactionPage::EditFactionPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );
	CreateControl( this, &editNumber, Edit_Faction_Number, "Number: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editName, Edit_Faction_Name, "Name: ", sizerEdit, *textStringValidator );
	CreateControl( this, &editAddress, Edit_Faction_Address, "Address: ", sizerEdit, *textStringValidator );
	CreateControl( this, &editPassword, Edit_Faction_Password, "Password: ", sizerEdit, *textStringValidator );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editUnclaimed, Edit_Faction_Unclaimed, "Unclaimed: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editTimes, Edit_Faction_Times, "Times: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editTemformat, Edit_Faction_Temformat, "Template: ", sizerEdit );
	CreateControl( this, &editQuit, Edit_Faction_Quit, "Quit: ", sizerEdit );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editLastChange, Edit_Faction_LastChange, "Last change: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editLastOrders, Edit_Faction_LastOrders, "Last orders: ", sizerEdit, *numStringValidator );

	// Fill Combo boxes
	editTemformat->Append( "Off", new int( TEMPLATE_OFF ) );
	editTemformat->Append( "Short", new int( TEMPLATE_SHORT ) );
	editTemformat->Append( "Long", new int( TEMPLATE_LONG ) );
	editTemformat->Append( "Map", new int( TEMPLATE_MAP ) );
	editTemformat->Append( "(...)" );

	editQuit->Append( "<none>", new int( QUIT_NONE ) );
	editQuit->Append( "By order", new int( QUIT_BY_ORDER ) );
	editQuit->Append( "By GM", new int( QUIT_BY_GM ) );
	editQuit->Append( "Restart", new int( QUIT_AND_RESTART ) );
	editQuit->Append( "Won game", new int( QUIT_WON_GAME ) );
	editQuit->Append( "Game over", new int( QUIT_GAME_OVER ) );
	editQuit->Append( "(...)" );

	toolBar->AddSeparator();
	toolBar->AddTool( Edit_Tool_Add_Unit, "",
					  wxBitmap( add_unit_xpm ), "Add unit" );
	toolBar->Realize();

	toolBar->EnableTool( Edit_Tool_Add, true );
	toolBar->EnableTool( Edit_Tool_Delete, true );
	toolBar->EnableTool( Edit_Tool_Move, false );
}

/**
 * Default destructor
 */
EditFactionPage::~EditFactionPage()
{
	DeleteCombo( editTemformat );
	DeleteCombo( editQuit );
}

/** 
 * Enable/Disable all controls on page
 */
void EditFactionPage::EnableAllControls( bool b )
{
	editNumber->Enable( b );
	editLastChange->Enable( b );
	editLastOrders->Enable( b );
	editUnclaimed->Enable( b );
	editName->Enable( b );
	editAddress->Enable( b );
	editPassword->Enable( b );
	editTimes->Enable( b );
	editTemformat->Enable( b );
	editQuit->Enable( b );
}

/**
 * Clear all controls on page
 */
void EditFactionPage::ClearAllControls()
{
	border->SetTitle( "Faction" );
	editNumber->Clear();
	editLastChange->Clear();
	editLastOrders->Clear();
	editUnclaimed->Clear();
	editName->Clear();
	editAddress->Clear();
	editPassword->Clear();
	editTimes->Clear();
	editTemformat->SetSelection( -1 );
	editQuit->SetSelection( -1 );
}

/**
 * Process event when a textbox is updated.
 * Update the actual game data, and the tree/list view display
 *  for the updated data for the current selection
 */
void EditFactionPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;
	for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
		Faction * f = ( Faction * ) editTop->selectedElems->Item( i );
	
		switch ( event.GetId() )
		{
		case Edit_Faction_Number:
			UpdateControl( editNumber, f->num );
			break;
		case Edit_Faction_Name:
			UpdateControl( editName, f->name );
			break;
		case Edit_Faction_Address:
			UpdateControl( editAddress, f->address );
			break;
		case Edit_Faction_Password:
			UpdateControl( editPassword, f->password );
			break;
		case Edit_Faction_Unclaimed:
			UpdateControl( editUnclaimed, f->unclaimed );
			break;
		case Edit_Faction_LastChange:
			UpdateControl( editLastChange, f->lastchange );
			break;
		case Edit_Faction_LastOrders:
			UpdateControl( editLastOrders, f->lastorders );
			break;
		case Edit_Faction_Times:
			UpdateControl( editTimes, f->times );
			break;
		case Edit_Faction_Temformat:
			UpdateControl( editTemformat, f->temformat );
			break;
		case Edit_Faction_Quit:
			UpdateControl( editQuit, f->quit );
			break;
		}
	}
	app->UpdateFactions();
}

/**
 * Process event when a button is pressed.
 */
void EditFactionPage::OnButton( wxCommandEvent & event )
{
	if( editWait )
		return;

	if( event.GetId() == Edit_Tool_Add_Unit ) {
		// Add unit to selected factions
		// Todo: test with multiple factions selected
		ARegion * r = GetRegionChoice( "Select region to add unit(s) to.", "New unit" );
		if( !r ) return;

		AElemArray * tempSelection = new AElemArray();
		for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
			Faction * f = ( Faction * ) editTop->selectedElems->Item( i );
			Unit * u = app->AddUnit(f);
			u->MoveUnit( r->GetDummy() );
//			frame->tree->AddItem( u );
			tempSelection->Add( u );
		}
		app->UpdateSelection( tempSelection, SELECT_UNIT );
		delete tempSelection;
	}
}

/**
 * Clone faction
 */
void EditFactionPage::OnToolAdd( wxCommandEvent & event )
{
	Faction * f = app->AddFaction();
//	frame->tree->AddItem( f );
	frame->list->AddItem( f );
	app->Select( f );
	app->UpdateSelection();
}

/**
 * Delete faction
 */
void EditFactionPage::OnToolDelete( wxCommandEvent & event )
{
	if( editTop->selectedElems->GetCount() == 0 ) return;

	int answer = wxMessageBox( "Really delete faction(s) and units?", "Confirm",
	                          wxYES_NO | wxCENTRE | wxICON_QUESTION );

	if( answer == wxYES ) {
		for( int i = (int) editTop->selectedElems->GetCount()-1 ; i >= 0; i-- ) {
			Faction * f = ( Faction * ) editTop->selectedElems->Item( i );
			{
				forlist( &app->m_game->regions ) {
					ARegion * r = ( ARegion * ) elem;
					forlist( &r->objects ) {
						Object * o = ( Object * ) elem;
						forlist( &o->units ) {
							Unit * u = ( Unit * ) elem;
							if( u->faction == f ) {
								app->Trash( u );
							}
						}
					}
				}
			}
			app->Trash( f );
		}
	}
	( ( EditFrame * )GetParent() )->HideAllPages();
	app->UpdateSelection();

}

void EditFactionPage::OnToolMove( wxCommandEvent & event )
{
}

// ---------------------------------------------------------------------------
// EditMarketPage
// ---------------------------------------------------------------------------

/**
 * Default constructor. Creates all necessary controls
 */
EditMarketPage::EditMarketPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editType, Edit_Market_Type, "Type: ", sizerEdit );
	CreateControl( this, &editItem, Edit_Market_Item, "Item: ", sizerEdit, true );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editPrice, Edit_Market_Price, "Price: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editAmount, Edit_Market_Amount, "Amount: ", sizerEdit, *numStringValidator );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editMinpop, Edit_Market_Minpop, "Min pop: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editMaxpop, Edit_Market_Maxpop, "Max pop: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editMinamt, Edit_Market_Minamt, "Min amount: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editMaxamt, Edit_Market_Maxamt, "Max amount: ", sizerEdit, *numStringValidator );

	// Fill Combo boxes
	editType->Append( "Sell", new int( M_BUY ) );
	editType->Append( "Buy", new int( M_SELL ) );
	editType->Append( "(...)" );

	for( int i = 0; i < NITEMS; i++ ) {
		if( i == I_SILVER ) {
			editItem->Append( ItemDefs[i].names, new int( i ) );
			continue;
		}
		if( ItemDefs[i].flags & ItemType::DISABLED ||
			ItemDefs[i].flags & ItemType::NOMARKET ) continue;
		editItem->Append( ItemDefs[i].names, new int( i ) );
	}
	editItem->Append( "(...)" );

	toolBar->EnableTool( Edit_Tool_Add, true );
	toolBar->EnableTool( Edit_Tool_Delete, true );
	toolBar->EnableTool( Edit_Tool_Move, true );
}

/**
 * Default destructor
 */
EditMarketPage::~EditMarketPage()
{
	DeleteCombo( editType );
	DeleteCombo( editItem );
}

/** 
 * Enable/Disable all controls on page
 */
void EditMarketPage::EnableAllControls( bool b )
{
	editType->Enable( b );
	editItem->Enable( b );
	editPrice->Enable( b );
	editAmount->Enable( b );
	editMinpop->Enable( b );
	editMaxpop->Enable( b );
	editMinamt->Enable( b );
	editMaxamt->Enable( b );

}

/**
 * Clear all controls on page
 */
void EditMarketPage::ClearAllControls()
{
	border->SetTitle( "Market" );
	editType->SetSelection( -1 );
	editItem->SetSelection( -1 );
	editPrice->Clear();
	editAmount->Clear();
	editMinpop->Clear();
	editMaxpop->Clear();
	editMinamt->Clear();
	editMaxamt->Clear();
}

/**
 * Process event when a textbox is updated.
 * Update the actual game data, and the tree/list view display
 *  for the updated data for the current selection
 */
void EditMarketPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;
	for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
		Market * m = ( Market * ) editTop->selectedElems->Item( i );
	
		switch ( event.GetId() )
		{
		case Edit_Market_Type:
			UpdateControl( editType,m->type );
			break;
		case Edit_Market_Item:
			UpdateControl( editItem,m->item );
			break;
		case Edit_Market_Price:
			UpdateControl( editPrice, m->price );
			break;
		case Edit_Market_Amount:
			UpdateControl( editAmount, m->amount );
			break;
		case Edit_Market_Minpop:
			UpdateControl( editMinpop, m->minpop );
			break;
		case Edit_Market_Maxpop:
			UpdateControl( editMaxpop, m->maxpop );
			break;
		case Edit_Market_Minamt:
			UpdateControl( editMinamt, m->minamt );
			break;
		case Edit_Market_Maxamt:
			UpdateControl( editMaxamt, m->maxamt );
			break;
		}
	}
	app->UpdateMarkets();
}

/**
 * Process event when a button is pressed.
 */
void EditMarketPage::OnButton( wxCommandEvent & event ){
}

/**
 * Clone market(s)
 */
void EditMarketPage::OnToolAdd( wxCommandEvent & event )
{
	Market * mOld = ( Market * ) editTop->selectedElems->Item(0);
	ARegion * r = app->m_game->regions.GetRegion(mOld->region);
	Market * m = new Market( mOld->type, mOld->item, mOld->price, mOld->amount,
							 mOld->minpop, mOld->maxpop, mOld->minamt, mOld->maxamt );
	r->markets.Add( m );
	m->region = r->num;
//	frame->tree->AddItem( m );
	frame->list->AddItem( m );
	app->Select( m );
	app->UpdateSelection();
}

/**
 * Delete market(s)
 */
void EditMarketPage::OnToolDelete( wxCommandEvent & event )
{
	if( editTop->selectedElems->GetCount() == 0 ) return;

	int answer = wxMessageBox( "Really delete market(s)?", "Confirm",
	                          wxYES_NO | wxCENTRE | wxICON_QUESTION );

	if( answer == wxYES ) {
		for( int i = (int) editTop->selectedElems->GetCount()-1; i >= 0; i-- ) {
			Market * m = ( Market * ) editTop->selectedElems->Item( i );
			app->Trash( m );
		}
	}
	( ( EditFrame * )GetParent() )->HideAllPages();

}

/**
 * Move market(s)
 */
void EditMarketPage::OnToolMove( wxCommandEvent & event )
{
	Object * destObj;
	Market * m = ( Market * ) editTop->selectedElems->Item( 0 );

	destObj = app->m_game->regions.GetRegion( m->region )->GetDummy();

	MoveDialog dlg( this, destObj, &destObj, false );

	if( dlg.ShowModal() == wxID_OK ) {
		if( destObj != NULL ) {
			for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
 				Market * m = ( Market * ) editTop->selectedElems->Item( i );
				ARegion * oldReg = app->m_game->regions.GetRegion( m->region );
				oldReg->markets.Remove( m );
				destObj->region->markets.Add( m );
				m->region = destObj->region->num;
			}
		}
	}
}

// ---------------------------------------------------------------------------
// EditProductionPage
// ---------------------------------------------------------------------------

/**
 * Default constructor. Creates all necessary controls
 */
EditProductionPage::EditProductionPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editItemtype, Edit_Production_Itemtype, "Item: ", sizerEdit, true );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editAmount, Edit_Production_Amount, "Amount: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editBaseamount, Edit_Production_Baseamount, "Base amount: ", sizerEdit, *numStringValidator );

	// Fill Combo boxes
	for( int i = 0; i < NITEMS; i++ ) {
		if( i == I_SILVER ) {
			editItemtype->Append( ItemDefs[i].names, new int( i ) );
			continue;
		}
		if( ItemDefs[i].flags & ItemType::DISABLED ||
		   ItemDefs[i].pSkill == -1 ) continue;
		if( ItemDefs[i].pInput[0].item != -1 ) continue;
		if( ItemDefs[i].pInput[1].item != -1 ) continue;
		if( ItemDefs[i].pInput[2].item != -1 ) continue;
		if( ItemDefs[i].pInput[3].item != -1 ) continue;
		editItemtype->Append( ItemDefs[i].names, new int( i ) );
	}

	toolBar->EnableTool( Edit_Tool_Add, true );
	toolBar->EnableTool( Edit_Tool_Delete, true );
	toolBar->EnableTool( Edit_Tool_Move, true );
}

/**
 * Default destructor
 */
EditProductionPage::~EditProductionPage()
{
	DeleteCombo( editItemtype );
}

/** 
 * Enable/Disable all controls on page
 */
void EditProductionPage::EnableAllControls( bool b )
{
	editItemtype->Enable( b );
	editAmount->Enable( b );
	editBaseamount->Enable( b );
}

/**
 * Clear all controls on page
 */
void EditProductionPage::ClearAllControls()
{
	border->SetTitle( "Production" );
	editItemtype->SetSelection( -1 );
	editAmount->Clear();
	editBaseamount->Clear();
}

/**
 * Process event when a textbox is updated.
 * Update the actual game data, and the tree/list view display
 *  for the updated data for the current selection
 */
void EditProductionPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;
	for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
		Production * p = ( Production * ) editTop->selectedElems->Item( i );
	
		switch ( event.GetId() )
		{
			case Edit_Production_Itemtype:
				UpdateControl( editItemtype, p->itemtype );
				break;
			case Edit_Production_Amount:
				UpdateControl( editAmount, p->amount );
				break;
			case Edit_Production_Baseamount:
				UpdateControl( editBaseamount, p->baseamount );
				break;
		}
	}
	app->UpdateProductions();
}

void EditProductionPage::OnButton( wxCommandEvent & event )
{
}

/**
 * Clone production
 */
void EditProductionPage::OnToolAdd( wxCommandEvent & event )
{
	Production * pOld = ( Production * ) editTop->selectedElems->Item(0);
	ARegion * r = app->m_game->regions.GetRegion(pOld->region);
	Production * p = new Production( pOld->itemtype, 0 );
	p->amount = pOld->amount;
	p->baseamount = pOld->baseamount;
	p->skill = pOld->skill;
	p->productivity = pOld->productivity;

	r->products.Add( p );
	p->region = r->num;
//	frame->tree->AddItem( p );
	frame->list->AddItem( p );
	app->Select( p );
	app->UpdateSelection();
}

/**
 * Delete production
 */
void EditProductionPage::OnToolDelete( wxCommandEvent & event )
{
	if( editTop->selectedElems->GetCount() == 0 ) return;

	int answer = wxMessageBox( "Really delete production(s)?", "Confirm",
	                          wxYES_NO | wxCENTRE | wxICON_QUESTION );

	if( answer == wxYES ) {
		for( int i = (int) editTop->selectedElems->GetCount()-1; i >= 0; i-- ) {
			Production * p = ( Production * ) editTop->selectedElems->Item( i );
			app->Trash( p );
		}
	}
	( ( EditFrame * )GetParent() )->HideAllPages();

}

/**
 * Move production
 */
void EditProductionPage::OnToolMove( wxCommandEvent & event )
{
	Object * destObj;
	Production * p = ( Production * ) editTop->selectedElems->Item( 0 );

	destObj = app->m_game->regions.GetRegion( p->region )->GetDummy();

	MoveDialog dlg( this, destObj, &destObj, false );

	if( dlg.ShowModal() == wxID_OK ) {
		if( destObj != NULL ) {
			for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
 				Production * p = ( Production * ) editTop->selectedElems->Item( i );
				ARegion * oldReg = app->m_game->regions.GetRegion( p->region );
				oldReg->products.Remove( p );
				destObj->region->products.Add( p );
				p->region = destObj->region->num;
			}
		}
	}

}

// ---------------------------------------------------------------------------
// EditObjectPage
// ---------------------------------------------------------------------------

/**
 * Default constructor. Creates all necessary controls
 */
EditObjectPage::EditObjectPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editNum, Edit_Object_Num, "Number: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editType, Edit_Object_Type, "Type: ", sizerEdit, true );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editName, Edit_Object_Name, "Name: ", sizerEdit, *textStringValidator );
	CreateControl( this, &editDescribe, Edit_Object_Describe, "Description: ", sizerEdit, *textStringValidator );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editInner, Edit_Object_Inner, "Inner: ", sizerEdit );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editRunes, Edit_Object_Runes, "Runes: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editIncomplete, Edit_Object_Incomplete, "Incomplete: ", sizerEdit, *numStringValidator );

	// Fill Combo boxes
	for( int i=0; i < NOBJECTS; i++ ) {
		if( ObjectDefs[i].flags & ObjectType::DISABLED ) continue;
		editType->Append( ObjectDefs[i].name, new int( i ) );
	}
	editType->Append( "(...)" );

	toolBar->AddSeparator();
	toolBar->AddTool( Edit_Tool_Add_Unit, "",
					  wxBitmap( add_unit_xpm ), "Add unit" );
	toolBar->Realize();

	toolBar->EnableTool( Edit_Tool_Add, true );
	toolBar->EnableTool( Edit_Tool_Delete, true );
	toolBar->EnableTool( Edit_Tool_Move, true );
}

/**
 * Default destructor
 */
EditObjectPage::~EditObjectPage()
{
	DeleteCombo( editType );
}

/** 
 * Enable/Disable all controls on page
 */
void EditObjectPage::EnableAllControls( bool b )
{
	editName->Enable( b );
	editDescribe->Enable( b );
	editInner->Enable( b );
	editNum->Enable( b );
	editIncomplete->Enable( b );
	editRunes->Enable( b );
	editType->Enable( b );
}

/**
 * Clear all controls on page
 */
void EditObjectPage::ClearAllControls()
{
	border->SetTitle( "Object" );
	editName->Clear();
	editDescribe->Clear();
	editInner->SetLabel( "" );
	editNum->Clear();
	editIncomplete->Clear();
	editRunes->Clear();
	editType->SetSelection( -1 );
}

/**
 * Process event when a textbox is updated.
 * Update the actual game data, and the tree/list view display
 *  for the updated data for the current selection
 */
void EditObjectPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;
	for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
		Object * o = ( Object * ) editTop->selectedElems->Item( i );

		switch ( event.GetId() )
		{
		case Edit_Object_Name:
			UpdateControl( editName, o->name );
			break;
		case Edit_Object_Describe:
			if( !o->describe )
				o->describe = new AString();
			UpdateControl( editDescribe, o->describe );
			break;
		case Edit_Object_Num:
			UpdateControl( editNum, o->num );
			break;
		case Edit_Object_Type:
				UpdateControl( editType, o->type );
			break;
		case Edit_Object_Incomplete:
			UpdateControl( editIncomplete, o->incomplete );
			break;
		case Edit_Object_Runes:
			UpdateControl( editRunes, o->runes );
			break;
		}
	}
	app->UpdateObjects();
}

/**
 * Process event when a button is pressed.
 */
void EditObjectPage::OnButton( wxCommandEvent & event )
{
	if( editWait ) return;

	int x, y, width, height;
	GetParent()->GetSize( &width, &height );
	GetParent()->GetPosition( &x, &y );
	GetParent()->ClientToScreen( &x, &y );

	if( event.GetId() == Edit_Tool_Add_Unit ) {
		// Add unit to object(s)
		AElemArray * tempSelection = new AElemArray();
//		editTop->selectedElems->Clear();
		for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
			Object * o = ( Object * ) editTop->selectedElems->Item( i );
			Unit * u = app->AddUnit();
			u->MoveUnit( o );
//			frame->tree->AddItem( u );
			frame->list->AddItem( u );
			tempSelection->Add( u );
		}
		app->UpdateSelection( tempSelection, SELECT_UNIT );
		delete tempSelection;
	} else if( event.GetId() == Edit_Object_Inner ){
		// Change inner location
		EditRegionAux aux( this, wxPoint( x + width + 3, y + 3 ), wxSize( 250, frame->map->GetSize().y - 2 ) );

		AElemArray selected;
		for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
 		Object * o = ( Object * ) editTop->selectedElems->Item( i );
			ARegion * r = app->m_game->regions.GetRegion( o->inner );
			selected.Add( r );
		}
		aux.Init( &selected );

		if( aux.ShowModal() == wxID_OK ) {
			if( aux.selectedArray.GetCount() > 0 ) {
				//update edit window and objects
				editInner->SetLabel( "" );
				for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
					Object * o = ( Object * ) editTop->selectedElems->Item( i );
					ARegion * r = ( ARegion * ) aux.selectedArray[0];
					o->inner = r->num;
					AString temp = AString( "( " ) + r->xloc + ", " + r->yloc + ", "
								   + r->zloc + " )";
					editInner->SetLabel( temp.Str() );
				}		
			}
		}
		selected.Clear();
		aux.selectedArray.Clear();
	}
	app->UpdateObjects();
}

/**
 * Clone object
 */
void EditObjectPage::OnToolAdd( wxCommandEvent & event )
{
	Object * oOld = ( Object * ) editTop->selectedElems->Item(0);
	ARegion * r = oOld->region;
	Object * o = new Object( r );
	o->num = r->buildingseq++;
	o->name = new AString( AString( ObjectDefs[O_DUMMY].name ) + " [" + o->num + "]" );
	r->objects.Add( o );
//	frame->tree->AddItem( o, false );
	frame->list->AddItem( o );
	app->Select(o);
	app->UpdateSelection();
}

/**
 * Delete object
 */
void EditObjectPage::OnToolDelete( wxCommandEvent & event )
{
	if( editTop->selectedElems->GetCount() == 0 ) return;

	int answer = wxMessageBox( "Really delete object(s)?", "Confirm",
	                          wxYES_NO | wxCENTRE | wxICON_QUESTION );

	if( answer == wxYES ) {
		for( int i = (int) editTop->selectedElems->GetCount()-1; i >= 0; i-- ) {
			Object * o = ( Object * ) editTop->selectedElems->Item( i );
			app->Trash( o );
		}
	}
	( ( EditFrame * )GetParent() )->HideAllPages();

}

/**
 * Move object
 */
void EditObjectPage::OnToolMove( wxCommandEvent & event )
{
	Object * destObj;
	destObj = ( Object * ) editTop->selectedElems->Item( 0 );

	MoveDialog dlg( this, destObj, &destObj, false );

	if( dlg.ShowModal() == wxID_OK ) {
		if( destObj != NULL ) {
			for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
 				Object * o = ( Object * ) editTop->selectedElems->Item( i );
				ARegion * oldReg = o->region ;
				oldReg->objects.Remove( o );
				destObj->region->objects.Add( o );
				o->region = destObj->region;
			}
		}
	}
}

// ---------------------------------------------------------------------------
// EditUnitPage
// ---------------------------------------------------------------------------

/**
 * Default constructor. Creates all necessary controls
 */
EditUnitPage::EditUnitPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editNum, Edit_Unit_Num, "Number: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editName, Edit_Unit_Name, "Name: ", sizerEdit, *textStringValidator );
	CreateControl( this, &editDescribe, Edit_Unit_Describe, "Description: ", sizerEdit, *textStringValidator );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );
	CreateControl( this, &editFaction, Edit_Unit_Faction, "Faction: ", sizerEdit );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editType, Edit_Unit_Type, "Type: ", sizerEdit );
	CreateControl( this, &editGuard, Edit_Unit_Guard, "Guard: ", sizerEdit );
	CreateControl( this, &editReveal, Edit_Unit_Reveal, "Reveal: ", sizerEdit );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editCombat, Edit_Unit_Combat, "Combat: ", sizerEdit );
	CreateControl( this, &editReadyItem, Edit_Unit_ReadyItem, "Ready Item: ", sizerEdit );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateButton( this, &editItems, Edit_Unit_Items, "Items--->", sizerEdit, wxALIGN_RIGHT );
	CreateButton( this, &editSkills, Edit_Unit_Skills, "Skills--->", sizerEdit, wxALIGN_RIGHT );
//	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );
//	CreateButton( this, &editItems, Edit_Unit_ReadyWeapon, "Ready weapons--->" );
//	CreateButton( this, &editSkills, Edit_Unit_ReadyArmor, "Ready armors--->" );

	// Fill Combo boxes
	editType->Append( "Normal", new int( U_NORMAL ) );
	editType->Append( "Mage", new int( U_MAGE ) );
//	editType->Append( "Warrior", new int( U_WARRIOR ) );
//	editType->Append( "Noble", new int( U_NOBLE ) );
//	editType->Append( "Trader", new int( U_TRADER ) );

	editType->Append( "Guard", new int( U_GUARD ) );
	editType->Append( "Monster", new int( U_WMON ) );
	editType->Append( "Guard mage", new int( U_GUARDMAGE ) );
	editType->Append( "Apprentice", new int( U_APPRENTICE ) );
	editType->Append( "(...)" );

	editGuard->Append( "None", new int( GUARD_NONE ) );
	editGuard->Append( "Guard", new int( GUARD_GUARD ) );
	editGuard->Append( "Avoid", new int( GUARD_AVOID ) );
	editGuard->Append( "(...)" );
	//Set and advance not available

	editReveal->Append( "None", new int( REVEAL_NONE ) );
	editReveal->Append( "Unit", new int( REVEAL_UNIT ) );
	editReveal->Append( "Faction", new int( REVEAL_FACTION ) );
	editReveal->Append( "(...)" );

	toolBar->EnableTool( Edit_Tool_Add, true );
	toolBar->EnableTool( Edit_Tool_Delete, true );
	toolBar->EnableTool( Edit_Tool_Move, true );
}

/**
 * Default destructor
 */
EditUnitPage::~EditUnitPage()
{
	DeleteCombo( editType );
	DeleteCombo( editGuard );
	DeleteCombo( editReveal );
}

/** 
 * Enable/Disable all controls on page
 */
void EditUnitPage::EnableAllControls( bool b )
{
	editNum->Enable( b );
	editName->Enable( b );
	editDescribe->Enable( b );
	editFaction->Enable( b );
	editType->Enable( b );
	editGuard->Enable( b );
	editReveal->Enable( b );
	editCombat->Enable( b );
	editReadyItem->Enable( b );
	editItems->Enable( b );
	editSkills->Enable( b );
//	editReadyWeapon->Enable( b );
//	editReadyArmor->Enable( b );
}

/**
 * Clear all controls on page
 */
void EditUnitPage::ClearAllControls()
{
	border->SetTitle( "Unit" );
	editNum->Clear();
	editName->Clear();
	editDescribe->Clear();
	editFaction->SetLabel( "" );
	editType->SetSelection( -1 );
	editGuard->SetSelection( -1 );
	editReveal->SetSelection( -1 );
	editCombat->SetLabel( "" );
	editReadyItem->SetLabel( "" );
}

/**
 * Process event when a textbox is updated.
 * Update the actual game data, and the tree/list view display
 *  for the updated data for the current selection
 */
void EditUnitPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;
	for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
		Unit * u = ( Unit * ) editTop->selectedElems->Item( i );
		switch ( event.GetId() )
		{
		case Edit_Unit_Num:
			UpdateControl( editNum, u->num );
			break;
		case Edit_Unit_Name:
			UpdateControl( editName, u->name );
			break;
		case Edit_Unit_Describe:
			if( !u->describe )
				u->describe = new AString();
			UpdateControl( editDescribe, u->describe );
			break;
		case Edit_Unit_Type:
				UpdateControl( editType, u->type );
			break;
		case Edit_Unit_Guard:
				UpdateControl( editGuard, u->guard );
			break;
		case Edit_Unit_Reveal:
				UpdateControl( editReveal, u->reveal );
			break;
		}
	}
	app->UpdateUnits();
}		

/**
 * Process event when a button is pressed.
 */
void EditUnitPage::OnButton( wxCommandEvent & event )
{
	if( editWait ) return;

	int x, y, width, height;
	GetParent()->GetSize( &width, &height );
	GetParent()->GetPosition( &x, &y );
	GetParent()->ClientToScreen( &x, &y );

	if( event.GetId() == Edit_Unit_Combat ){
		EditSkillTypeAux aux( this, wxPoint( x + width + 3, y + 3 ), wxSize( 250, frame->map->GetSize().y - 2 ) );

		Skill * s = NULL;
		AElemArray selected;
		for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
			Unit * u = ( Unit * ) editTop->selectedElems->Item( i );
			s = new Skill();
			s->type = u->combat;
			selected.Add( s );
		}
		aux.Init( &selected, SkillType::COMBAT );

		if( aux.ShowModal() == wxID_OK ) {
			if( aux.selectedArray.GetCount() > 0 ) {
				//update edit window and objects
				editCombat->SetLabel( "" );
				for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
					Unit * u = ( Unit * ) editTop->selectedElems->Item( i );

					int type = ( ( Skill * ) aux.selectedArray[0] )->type;
					u->combat = type;
					if( type != -1 )
						AddToControl( editCombat, SkillDefs[type].name );
				}
			}
		}
		WX_CLEAR_ARRAY( selected );

	} else if( event.GetId() == Edit_Unit_ReadyItem ){
		EditItemTypeAux aux( this, wxPoint( x + width + 3, y + 3 ), wxSize( 250, frame->map->GetSize().y - 2 ) );

		Item * it = NULL;
		AElemArray selected;
		for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
			Unit * u = ( Unit * ) editTop->selectedElems->Item( i );
			it = new Item();
			it->type = u->readyItem;
			selected.Add( it );
		}
		aux.Init( &selected );

		if( aux.ShowModal() == wxID_OK ) {
			//update edit window and objects
			editReadyItem->SetLabel( "" );
			if( aux.selectedArray.GetCount() > 0 ) {
				for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
					Unit * u = ( Unit * ) editTop->selectedElems->Item( i );

					int type = ( ( Item * ) aux.selectedArray[0] )->type;
					u->readyItem = type;
					if( type != -1 )
						AddToControl( editReadyItem, ItemDefs[type].name );
				}
			}
		}
		WX_CLEAR_ARRAY( selected );

	} else if( event.GetId() == Edit_Unit_Faction ){
		EditFactionAux aux( this, wxPoint( x + width + 3, y + 3 ), wxSize( 250, frame->map->GetSize().y - 2 ) );

		AElemArray selected;
		for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
			Unit * u = ( Unit * ) editTop->selectedElems->Item( i );
			selected.Add( u->faction );
		}
		aux.Init( &selected );

		if( aux.ShowModal() == wxID_OK ) {
			if( aux.selectedArray.GetCount()>0 ) {
				//update edit window and objects
				editFaction->SetLabel( "" );
				for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
					Unit * u = ( Unit * ) editTop->selectedElems->Item( i );
					u->faction = ( Faction * ) aux.selectedArray[0];
					AddToControl( editFaction, u->faction->name->Str() );
				}
			
			}
		}
		selected.Clear();
		aux.selectedArray.Clear();
	} else if( event.GetId() == Edit_Unit_Items ) {
		EditItemAux aux( this, wxPoint( x + width + 3, y + 3 ), wxSize( 250, frame->map->GetSize().y - 2 ) );

		aux.Init( editTop->selectedElems );

		if( aux.ShowModal() == wxID_OK ) {
			//update edit window and objects
			if( aux.selectedArray.GetCount()>0 ) {
				for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
					Unit * u = ( Unit * ) editTop->selectedElems->Item( i );
					for( int j = 0; j < (int) aux.selectedArray.GetCount(); j++ ) {
						Item * it = ( Item * ) aux.selectedArray[j];
						u->items.SetNum( it->type, it->num );
					}
				}
			}
		}

	} else if( event.GetId() == Edit_Unit_Skills ) {
		EditSkillAux aux( this, wxPoint( x + width + 3, y + 3 ), wxSize( 250, frame->map->GetSize().y - 2 ) );

		aux.Init( editTop->selectedElems );

		if( aux.ShowModal() == wxID_OK ) {
			//update edit window and objects
			if( aux.selectedArray.GetCount()>0 ) {
				for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
					Unit * u = ( Unit * ) editTop->selectedElems->Item( i );
					for( int j = 0; j < (int) aux.selectedArray.GetCount(); j++ ) {
						Skill * s = ( Skill * ) aux.selectedArray[j];
						if( s->days != -1 )
							u->skills.SetDays( s->type, s->days * u->GetMen() );
					}
				}
			}		
		}
	}
	app->UpdateUnits();
}


/**
 * Clone unit
 */
void EditUnitPage::OnToolAdd( wxCommandEvent & event )
{
	Unit * uOld = ( Unit * ) editTop->selectedElems->Item( 0 );
	Object * o = uOld->object;
	Faction * f = uOld->faction;
	Unit * u = app->AddUnit( f );
	u->MoveUnit( o );
//	frame->tree->AddItem( u );
	frame->list->AddItem( u );
	app->Select(u);
	app->UpdateSelection();
}

/**
 * Delete unit
 */
void EditUnitPage::OnToolDelete( wxCommandEvent & event )
{
	if( editTop->selectedElems->GetCount() == 0 ) return;

	int answer = wxMessageBox( "Really delete unit(s)?", "Confirm",
	                          wxYES_NO | wxCENTRE | wxICON_QUESTION );

	if( answer == wxYES ) {
		for( int i = (int) editTop->selectedElems->GetCount() -1; i >= 0; i-- ) {
			Unit * u = ( Unit * ) editTop->selectedElems->Item( i );
			app->Trash( u );
		}
	}
	( ( EditFrame * )GetParent() )->HideAllPages();
}

/**
 * Move unit
 */
void EditUnitPage::OnToolMove( wxCommandEvent & event )
{
	Object * destObj;
	destObj = ( ( Unit * ) editTop->selectedElems->Item( 0 ) )->object;

	MoveDialog dlg( this, destObj, &destObj );

	if( dlg.ShowModal() == wxID_OK ) {
		if( destObj != NULL ) {
			for( int i = 0; i < (int) editTop->selectedElems->GetCount(); i++ ) {
				Unit * u = ( Unit * ) editTop->selectedElems->Item( i );
				u->MoveUnit( destObj );
				app->UpdateUnits();
			}
		}
	}
}

// ---------------------------------------------------------------------------
// EditGamePage
// ---------------------------------------------------------------------------

/**
 * Default constructor. Creates all necessary controls
 */
EditGamePage::EditGamePage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( this, &editMonth, Edit_Game_Month, "Month: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editYear, Edit_Game_Year, "Year: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editFactionSeq, Edit_Game_FactionSeq, "Faction seq: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editUnitSeq, Edit_Game_UnitSeq, "Unit seq: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editShipSeq, Edit_Game_ShipSeq, "Ship seq: ", sizerEdit, *numStringValidator );
	CreateControl( this, &editGuardFaction, Edit_Game_GuardFaction, "Guard faction: ", sizerEdit );
	CreateControl( this, &editMonFaction, Edit_Game_MonFaction, "Monster faction: ", sizerEdit );

	toolBar->EnableTool( Edit_Tool_Add, false );
	toolBar->EnableTool( Edit_Tool_Delete, false );
	toolBar->EnableTool( Edit_Tool_Move, false );
}

/**
 * Default destructor
 */
EditGamePage::~EditGamePage()
{
}

/** 
 * Enable/Disable all controls on page
 */
void EditGamePage::EnableAllControls( bool b )
{
	editMonth->Enable( b );
	editYear->Enable( b );
	editFactionSeq->Enable( b );
	editUnitSeq->Enable( b );
	editShipSeq->Enable( b );
	editGuardFaction->Enable( b );
	editMonFaction->Enable( b );
}

/**
 * Clear all controls on page
 */
void EditGamePage::ClearAllControls()
{
	border->SetTitle( "Game" );
	editMonth->Clear();
	editYear->Clear();
	editFactionSeq->Clear();
	editUnitSeq->Clear();
	editShipSeq->Clear();
	editGuardFaction->SetLabel( "" );
	editMonFaction->SetLabel( "" );
}

/**
 * Process event when a textbox is updated.
 * Update the actual game data, and the tree/list view display
 *  for the updated data for the current selection
 */
void EditGamePage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;

	Game * game = app->m_game;
	
	switch ( event.GetId() )
	{
		case Edit_Game_Month:
			UpdateControl( editMonth, game->month );
			break;
		case Edit_Game_Year:
			UpdateControl( editYear, game->year );
			break;
		case Edit_Game_FactionSeq:
			UpdateControl( editFactionSeq, game->factionseq );
			break;
		case Edit_Game_UnitSeq:
			UpdateControl( editUnitSeq, game->unitseq );
			break;
		case Edit_Game_ShipSeq:
			UpdateControl( editShipSeq, game->shipseq );
			break;
	}
	app->UpdateLevels();
}

/**
 * Process event when a button is pressed.
 */
void EditGamePage::OnButton( wxCommandEvent & event )
{
	if( editWait ) return;

	int x, y, width, height;
	GetParent()->GetSize( &width, &height );
	GetParent()->GetPosition( &x, &y );
	GetParent()->ClientToScreen( &x, &y );

	Game * game = app->m_game;
	if( event.GetId() == Edit_Game_GuardFaction ){
		EditFactionAux aux( this, wxPoint( x + width + 3, y + 3 ), wxSize( 250, frame->map->GetSize().y - 2 ) );

		Faction * fac = NULL;
		forlist(&app->m_game->factions) {
			if( (( Faction * ) elem )->num == game->guardfaction )
				fac = ( Faction * ) elem;
		}
		AElemArray selected;
		if( !fac ) {
			aux.Init( NULL );
		} else {
			selected.Add( fac );
			aux.Init( &selected );
		}

		if( aux.ShowModal() == wxID_OK ) {
			//update edit window and objects
			editGuardFaction->SetLabel( "" );
			if( aux.selectedArray.GetCount()>0 ) {
				Faction * f = ( Faction * ) aux.selectedArray[0];
				game->guardfaction = f->num;
				AddToControl( editGuardFaction, f->name->Str() );
			}
		}
		selected.Clear();
		aux.selectedArray.Clear();

	} else if( event.GetId() == Edit_Game_MonFaction ){
		EditFactionAux aux( this, wxPoint( x + width + 3, y + 3 ), wxSize( 250, frame->map->GetSize().y - 2 ) );
		AElemArray selected;

		Faction * fac = NULL;
		forlist(&app->m_game->factions) {
			if( (( Faction * ) elem )->num == game->monfaction )
				fac = ( Faction * ) elem;
		}
		if( !fac ) {
			aux.Init( NULL );
		} else {
			selected.Add( fac );
			aux.Init( &selected );
		}

		if( aux.ShowModal() == wxID_OK ) {
			//update edit window and objects
			editGuardFaction->SetLabel( "" );
			if( aux.selectedArray.GetCount()>0 ) {
				Faction * f = ( Faction * ) aux.selectedArray[0];
				game->monfaction = f->num;
				AddToControl( editMonFaction, f->name->Str() );
			}
		}
		selected.Clear();
		aux.selectedArray.Clear();
	}
	
}

void EditGamePage::OnToolAdd( wxCommandEvent & event )
{
}

void EditGamePage::OnToolDelete( wxCommandEvent & event )
{
}

void EditGamePage::OnToolMove( wxCommandEvent & event )
{
}

/**
 * Synch displayed selection with current selection
 */
void EditFrame::UpdateSelection()
{
	int i;
	// Change selections
	if( app->curSelection == curSelection ) {
		// New selection is the same type as the current selection
		// Deselect everything if there are items that aren't in the new selection
		// Select items in the new selection as necessary

		if( curSelection == SELECT_LEVEL ) {
			if( selectedLevel != app->selectedLevel ) {
				DeselectAll();
				SelectItem( selectedLevel );
			}
		} else if( curSelection == SELECT_GAME ) {
			// Nothing to do here
		} else {
			// Unselect any items not in the new selection
			for( i = 0; i < (int) selectedElems->GetCount(); i++ ) {
				AListElem * elem = selectedElems->Item( i );
				if( app->selectedElems->Index( elem ) == wxNOT_FOUND )  {
					DeselectAll();
					selectedElems->Clear();
					break;
				}
			}
		
			// Now select any items that are not in the current selection
			if( app->curSelection == SELECT_REGION ) {
				regionPage->SelectItems( app->selectedElems );
			} else {
				for( i = 0; i < (int) app->selectedElems->GetCount(); i++ ) {
					AListElem * elem = app->selectedElems->Item( i );
					if( selectedElems->Index( elem ) == wxNOT_FOUND )
						SelectItem( elem, app->curSelection );
				}
			}
		}
	} else {
		// New selection is a different type than the current selection
		// Deselect all items in the current selection and select all those in the new selection

		DeselectAll();

		// Now select the new selection
		if( app->curSelection == SELECT_LEVEL ) {
			SelectItem( app->selectedLevel );
		} else if( app->curSelection == SELECT_GAME ) {
			SelectItem( app->m_game );
		} else if( app->curSelection == SELECT_REGION ) {
			regionPage->SelectItems( app->selectedElems );
		} else {
			for( i = 0; i < (int) app->selectedElems->GetCount(); i++ ) {
				AListElem * elem = app->selectedElems->Item( i );
				SelectItem( elem, app->curSelection );
			}
		}
	}

	PostLoad( app->curSelection );

	// Synch selection values
	curSelection = app->curSelection;
	selectedLevel = app->selectedLevel;
	selectedElems->Clear();
	for( i = 0; i < (int) app->selectedElems->GetCount(); i++ ) {
		AListElem * elem = app->selectedElems->Item( i );
		selectedElems->Add( elem );
	}

	// And finally, display the last selection

}

/**
 * Select game element
 */
void EditFrame::SelectItem( Game * pGame )
{
	gamePage->SelectItem( pGame );
}

/**
 * Select level element
 */
void EditFrame::SelectItem( ARegionArray * level )
{
	levelPage->SelectItem( level );
}

/**
 * Select element
 */
void EditFrame::SelectItem( AListElem * element, int type )
{
	EditPage * page = NULL;
	switch( type ) {
		case SELECT_REGION:
			page = regionPage;
			break;
		case SELECT_OBJECT:
			page = objectPage;
			break;
		case SELECT_FACTION:
			page = factionPage;
			break;
		case SELECT_UNIT:
			page = unitPage;
			break;
		case SELECT_MARKET:
			page = marketPage;
			break;
		case SELECT_PRODUCTION:
			page = productionPage;
			break;
		case SELECT_LEVEL:
			page = levelPage;
			break;
		case SELECT_GAME:
			page = gamePage;
			break;
	}
	page->SelectItem( element );
}

/**
 * Update controls with new selection
 */
void EditRegionPage::SelectItem( AListElem * element )
{
	editWait = true;

	bool multiple = false;
	if( app->selectedElems->Count() > 1 ) multiple = true;

	ARegion * r = ( ARegion * ) element;
	AddToControl( editType, TerrainDefs[r->type].name );
	AddToControl( editRace, ( r->race != -1 ? ItemDefs[r->race].names : "" ) );
//	AddToControl( editNumber, r->num );
	AddToControl( editName, r->name->Str() );
	AddToControl( editBuildingseq, r->buildingseq );
	AddToControl( editGate, r->gate );
	AddToControl( editPop, r->population );
	AddToControl( editBasepop, r->basepopulation );
	AddToControl( editWages, r->wages );
	AddToControl( editMaxwages, r->maxwages );
	AddToControl( editMoney, r->money );
	for( int j=0; j < NDIRS; j++ ) {
		ARegion * reg = r->neighbors[j];
		if( !reg ) continue;
		AString temp = AString( "( " ) + reg->xloc + ", " + reg->yloc + ", " +
					   reg->zloc + " )";
		AddToControl( editExits[j], temp.Str() );
	}

	if( r->town && !multiple ) {
		editTown->SetLabel( "Remove town... *" );
		AddToControl( editTownName, r->town->name->Str() );
		AddToControl( editTownPop, r->town->pop );
		AddToControl( editTownBasepop, r->town->basepop );
	} else {
		editTown->SetLabel( "Add Town... *" );
		editTownName->Enable( false );
		editTownPop->Enable( false );
		editTownBasepop->Enable( false );
	}
	editWait = false;

}

/**
 * Update controls with new selection
 */
void EditRegionPage::SelectItems( AElemArray * array )
{
	editWait = true;

	int count = (int) array->Count();
	int i;

	for( i = 0; i < count; i++ ) {
		ARegion * r = ( ARegion * ) array->Item( i );
		if( !(AddToControl( editType, TerrainDefs[r->type].name ) ) ) break;
	}

	for( i = 0; i < count; i++ ) {
		ARegion * r = ( ARegion * ) array->Item( i );
		if( !(AddToControl( editRace, ( r->race != -1 ? ItemDefs[r->race].names : "" ) ) ) ) break;
	}
//	for( i = 0; i < count; i++ ) {
//		ARegion * r = ( ARegion * ) array->Item( i );
//		if( !(AddToControl( editNumber, r->num ) ) ) break;
//	}
	for( i = 0; i < count; i++ ) {
		ARegion * r = ( ARegion * ) array->Item( i );
		if( !(AddToControl( editName, r->name->Str() ) ) ) break;
	}
	for( i = 0; i < count; i++ ) {
		ARegion * r = ( ARegion * ) array->Item( i );
		if( !(AddToControl( editBuildingseq, r->buildingseq ) ) ) break;
	}
	for( i = 0; i < count; i++ ) {
		ARegion * r = ( ARegion * ) array->Item( i );
		if( !(AddToControl( editGate, r->gate ) ) ) break;
	}
	for( i = 0; i < count; i++ ) {
		ARegion * r = ( ARegion * ) array->Item( i );
		if( !(AddToControl( editPop, r->population ) ) ) break;
	}
	for( i = 0; i < count; i++ ) {
		ARegion * r = ( ARegion * ) array->Item( i );
		if( !(AddToControl( editBasepop, r->basepopulation ) ) ) break;
	}
	for( i = 0; i < count; i++ ) {
		ARegion * r = ( ARegion * ) array->Item( i );
		if( !(AddToControl( editWages, r->wages ) ) ) break;
	}
	for( i = 0; i < count; i++ ) {
		ARegion * r = ( ARegion * ) array->Item( i );
		if( !(AddToControl( editMaxwages, r->maxwages ) ) ) break;
	}
	for( i = 0; i < count; i++ ) {
		ARegion * r = ( ARegion * ) array->Item( i );
		if( !(AddToControl( editMoney, r->money ) ) ) break;
	}

	int town = 0;
	if( count == 1 ) {
		ARegion * r = ( ARegion * ) array->Item( 0 );
		for( int j = 0; j < NDIRS; j++ ) {
			editExits[j]->Enable( true );
			ARegion * reg = r->neighbors[j];
			if( !reg ) continue;
			AString temp = AString( "( " ) + reg->xloc + ", " + reg->yloc + ", " +
						   reg->zloc + " )";
			AddToControl( editExits[j], temp.Str() );
		}
		if( r->town ) {
			town = 1;
			editTown->SetLabel( "Remove town... *" );
			AddToControl( editTownName, r->town->name->Str() );
			AddToControl( editTownPop, r->town->pop );
			AddToControl( editTownBasepop, r->town->basepop );
		}
	} else {
		for( int j = 0; j < NDIRS; j++ ) {
			editExits[j]->Enable( false );
		}
	}

	if( !town ) {
		editTown->SetLabel( "Add Town... *" );
		editTownName->Enable( false );
		editTownPop->Enable( false );
		editTownBasepop->Enable( false );
	}
	editWait = false;

}

/**
 * Update controls with new selection
 */
void EditFactionPage::SelectItem( AListElem * element )
{
	editWait = true;

	Faction * f = ( Faction * ) element;
	AddToControl( editNumber, f->num );
	AddToControl( editLastChange, f->lastchange );
	AddToControl( editLastOrders, f->lastorders );
	AddToControl( editUnclaimed, f->unclaimed );
	AddToControl( editName, f->name->Str() );
	AddToControl( editAddress, f->address->Str() );
	AddToControl( editPassword, f->password->Str() );
	AddToControl( editTimes, f->times );
	AddToControl( editTemformat, f->temformat );
	AddToControl( editQuit, f->quit );

	editWait = false;

}

/**
 * Update controls with new selection
 */
void EditLevelPage::SelectItem( AListElem * element )
{
	// do nothing
}

/**
 * Update controls with new selection
 */
void EditLevelPage::SelectItem( ARegionArray * level )
{
	editWait = true;
	EnableAllControls( true );
	ClearAllControls();

	AddToControl( editName, level->strName ? level->strName->Str() : "");
	AddToControl( editLevelType, level->levelType );

	editWait = false;
}

/**
 * Update controls with new selection
 */
void EditGamePage::SelectItem( AListElem * element )
{
	// do nothing
}

/**
 * Update controls with new selection
 */
void EditGamePage::SelectItem( Game * game )
{
	editWait = true;

	AddToControl( editMonth, game->month );
	AddToControl( editYear, game->year );
	AddToControl( editFactionSeq, game->factionseq );
	AddToControl( editUnitSeq, game->unitseq );
	AddToControl( editShipSeq, game->shipseq );

	Faction * fac = NULL;
	{
		forlist(&app->m_game->factions) {
			if( (( Faction * ) elem )->num == game->guardfaction )
				fac = ( Faction * ) elem;
		}
	}
	if( fac )
		AddToControl( editGuardFaction, fac->name->Str() );

	fac = NULL;
	forlist(&app->m_game->factions) {
		if( (( Faction * ) elem )->num == game->monfaction )
			fac = ( Faction * ) elem;
	}
	if( fac )
		AddToControl( editMonFaction, fac->name->Str() );

	editWait = false;

}

/**
 * Update controls with new selection
 */
void EditUnitPage::SelectItem( AListElem * element )
{
	editWait = true;

	Unit * u = ( Unit * ) element;
	AString comb = ( u->combat==-1 ? "" : SkillDefs[u->combat].name );
	AString item = ( u->readyItem == -1? "" : ItemDefs[u->readyItem].name );

	AddToControl( editNum, u->num );
	AddToControl( editName, u->name->Str() );
	if( u->describe )
		AddToControl( editDescribe, u->describe->Str() );
	AddToControl( editFaction, u->faction->name->Str() );
	AddToControl( editType, u->type );
	AddToControl( editGuard, u->guard );
	AddToControl( editReveal, u->reveal );
	AddToControl( editCombat, comb.Str() );
	AddToControl( editReadyItem, item.Str() );

	editWait = false;

}

/**
 * Update controls with new selection
 */
void EditObjectPage::SelectItem( AListElem * element )
{
	editWait = true;

	Object * o = ( Object * ) element;

	AString temp;

	if( o->inner != -1 ) {
		ARegion * r = app->m_game->regions.GetRegion( o->inner );
		temp = AString( "( " ) + r->xloc + ", " + r->yloc + ", " +
					   r->zloc + " )";
	} else {
		temp = "";
	}

	AddToControl( editName, o->name->Str() );
	if( o->describe )
		AddToControl( editDescribe, o->describe->Str() );
	AddToControl( editInner, temp.Str() );
	AddToControl( editNum, o->num );
	AddToControl( editIncomplete, o->incomplete );
	AddToControl( editRunes, o->runes );
	AddToControl( editType, o->type );
	
	editWait = false;

}

/**
 * Update controls with new selection
 */
void EditMarketPage::SelectItem( AListElem * element )
{
	editWait = true;

	Market * m = ( Market * ) element;

	AddToControl( editPrice, m->price );
	AddToControl( editAmount, m->amount );
	AddToControl( editMinpop, m->minpop );
	AddToControl( editMaxpop, m->maxpop );
	AddToControl( editMinamt, m->minamt );
	AddToControl( editMaxamt, m->maxamt );
	AddToControl( editType, m->type );
	AddToControl( editItem, m->item );	

	editWait = false;
}

/**
 * Update controls with new selection
 */
void EditProductionPage::SelectItem( AListElem * element )
{
	editWait = true;

	Production * p = ( Production * ) element;

	AddToControl( editAmount, p->amount );
	AddToControl( editBaseamount, p->baseamount );
	AddToControl( editItemtype, p->itemtype );

	editWait = false;

}

void EditPage::SelectItem( AListElem * element )
{
}

void EditPage::PostLoad()
{
}

/**
 * Set border label, and enable/disable controls as necessary
 */
void EditFactionPage::PostLoad()
{
	int count = app->selectedElems->GetCount();
	if( count == 1 ) {
		Faction * f = ( Faction * ) app->selectedElems->Item( 0 );
		border->SetTitle( f->name->Str() );
	} else {
		AString temp = AString( count ) + " Factions";
		border->SetTitle( temp.Str() );
	}
}

/**
 * Set border label, and enable/disable controls as necessary
 */
void EditLevelPage::PostLoad()
{
	if( app->selectedLevel && app->selectedLevel->regions && app->selectedLevel->regions[0] ) {
		AString temp = AString( "Level " ) + AString( app->selectedLevel->regions[0]->zloc )
			+ " : ";
		if( app->selectedLevel->strName ) 
			temp += *app->selectedLevel->strName;
		else
			temp += "(Surface)";
		border->SetTitle( temp.Str() );
	}
}

/**
 * Set border label, and enable/disable controls as necessary
 */
void EditGamePage::PostLoad()
{
	border->SetTitle( Globals->WORLD_NAME );
}

/**
 * Set border label, and enable/disable controls as necessary
 */
void EditRegionPage::PostLoad()
{
	AString temp;

	int count = (int) app->selectedElems->Count();
	if( count == 1 ) {
		ARegion * r = ( ARegion * ) app->selectedElems->Item( 0 );
		temp = AString( "( " ) + r->xloc + ", " + r->yloc + ", " +
						   r->zloc + " ) ";
		temp += AString( " : " ) + r->num;
		border->SetTitle( temp.Str() );
	} else {
		editTown->Enable( false );
		editTownName->Enable( false );
		editTownPop->Enable( false );
		editTownBasepop->Enable( false );
		temp = AString( count ) + " Regions";
		border->SetTitle( temp.Str() );
	}
}

/**
 * Set border label, and enable/disable controls as necessary
 */
void EditObjectPage::PostLoad()
{
	AString temp;

	int count = app->selectedElems->GetCount();
	if( count == 1 ) {
		Object * o = ( Object * ) app->selectedElems->Item( 0 );
		border->SetTitle( o->name->Str() );
	} else {
		AString temp = AString( count ) + " Objects";
		border->SetTitle( temp.Str() );
	}

	bool dummy = false;

	for( int i = 0; i < count; i++ ) {
		Object * o = ( Object * ) app->selectedElems->Item( i );
		if( o->region->GetDummy() == o ) {
			dummy = true;
			break;
		}
	}

	if( dummy ) {
		toolBar->EnableTool( Edit_Tool_Delete, false );
		toolBar->EnableTool( Edit_Tool_Move, false );
	} else {
		toolBar->EnableTool( Edit_Tool_Delete, true );
		toolBar->EnableTool( Edit_Tool_Move, true );
	}

}

/**
 * Set border label, and enable/disable controls as necessary
 */
void EditUnitPage::PostLoad()
{
	int count = app->selectedElems->GetCount();
	if( count == 1 ) {
		Unit * u = ( Unit * ) app->selectedElems->Item( 0 );
		border->SetTitle( u->name->Str() );
	} else {
		AString temp = AString( count ) + " Units";
		border->SetTitle( temp.Str() );
	}
}

/**
 * Set border label, and enable/disable controls as necessary
 */
void EditProductionPage::PostLoad()
{
	int count = app->selectedElems->GetCount();
	if( count == 1 ) {
		border->SetTitle( "Production" );
	} else {
		AString temp = AString( count ) + " Productions";
		border->SetTitle( temp.Str() );
	}
}

/**
 * Set border label, and enable/disable controls as necessary
 */
void EditMarketPage::PostLoad()
{
	int count = app->selectedElems->GetCount();
	if( count == 1 ) {
		border->SetTitle( "Market" );
	} else {
		AString temp = AString( count ) + " Markets";
		border->SetTitle( temp.Str() );
	}
}

/**
 * Run final processing for current page
 */
void EditFrame::PostLoad( int type )
{
	EditPage * page = NULL;
	switch( type ) {
		case SELECT_REGION:
			page = regionPage;
			break;
		case SELECT_OBJECT:
			page = objectPage;
			break;
		case SELECT_FACTION:
			page = factionPage;
			break;
		case SELECT_UNIT:
			page = unitPage;
			break;
		case SELECT_MARKET:
			page = marketPage;
			break;
		case SELECT_PRODUCTION:
			page = productionPage;
			break;
		case SELECT_LEVEL:
			page = levelPage;
			break;
		case SELECT_GAME:
			page = gamePage;
			break;
	}
	if( !page ) return;
	page->PostLoad();
	ShowPage( page );
}

/**
 * Repopulate controls with current selection
 */
void EditPage::ReloadPage()
{
	editTop->selectedElems->Clear();
	editTop->DeselectAll();
	editTop->UpdateSelection();
}

/**
 * Reset all pages and controls
 */
void EditFrame::DeselectAll()
{
	SetWait( true );

	factionPage->ClearAllControls();
	regionPage->ClearAllControls();
	marketPage->ClearAllControls();
	productionPage->ClearAllControls();
	objectPage->ClearAllControls();
	unitPage->ClearAllControls();
	levelPage->ClearAllControls();
	gamePage->ClearAllControls();
	factionPage->EnableAllControls( true );
	regionPage->EnableAllControls( true );
	marketPage->EnableAllControls( true );
	productionPage->EnableAllControls( true );
	objectPage->EnableAllControls( true );
	unitPage->EnableAllControls( true );
	levelPage->EnableAllControls( true );
	gamePage->EnableAllControls( true );

	SetWait( false );
}

/**
 * Sets wait flag on all pages
 */
void EditFrame::SetWait( bool wait )
{
	factionPage->editWait = wait;
	regionPage->editWait = wait;
	marketPage->editWait = wait;
	productionPage->editWait = wait;
	objectPage->editWait = wait;
	unitPage->editWait = wait;
	levelPage->editWait = wait;
	gamePage->editWait = wait;
}
