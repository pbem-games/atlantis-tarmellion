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

#include "edit.h"
#include "gui.h"

#include "../gamedata.h"
#include "../aregion.h"
#include "../items.h"
#include "../astring.h"
#include "../object.h"
#include "../unit.h"
#include "../faction.h"
#include "editaux.h"

#include "wx/valtext.h"
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
    EVT_BUTTON( Edit_Tool_Add_Object, EditPage::OnButton )
    EVT_BUTTON( Edit_Tool_Add_Unit, EditPage::OnButton )
    EVT_TEXT( Edit_Region_Number, EditPage::OnTextUpdate )
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

    EVT_SIZE( EditPage::OnSize )

END_EVENT_TABLE()
    
wxTextValidator *textStringValidator = ( wxTextValidator * ) NULL;
wxTextValidator *numStringValidator = ( wxTextValidator * ) NULL;

// ---------------------------------------------------------------------------
// EditFrame
// ---------------------------------------------------------------------------

EditFrame::EditFrame( wxWindow *parent, const wxPoint& pos,
                     const wxSize& size )
          : wxWindow( parent, -1, pos, size,
                    wxDEFAULT_FRAME_STYLE, "Edit" )
{
}

EditFrame::~EditFrame()
{
	delete textStringValidator;
	delete numStringValidator;
}

void EditFrame::Init()
{
	int width,height;
	GetClientSize( &width, &height );
	wxSize size( width,height );
	wxStringList array;
	textStringValidator = new wxTextValidator( wxFILTER_INCLUDE_CHAR_LIST );
	for( char c = 127; c >= 0; c-- ) {
		if( islegal( c ) ) array.Add( wxString( c ) );
	}
	textStringValidator->SetIncludeList( array );

	numStringValidator = new wxTextValidator( wxFILTER_NUMERIC );

	factionPage = new EditFactionPage( this, wxDefaultPosition, size );
	regionPage = new EditRegionPage( this,wxDefaultPosition,size );
	marketPage = new EditMarketPage( this,wxDefaultPosition,size );
	productionPage = new EditProductionPage( this,wxDefaultPosition,size );
	objectPage = new EditObjectPage( this,wxDefaultPosition,size );
	unitPage = new EditUnitPage( this,wxDefaultPosition,size );

	HideAllPages();
}

void EditFrame::OnSize( wxSizeEvent& event )
{
	int width,height;

	GetClientSize( &width, &height );

	factionPage->SetSize( width,height );
	factionPage->CentreOnParent();
	regionPage->SetSize( width,height );
	regionPage->CentreOnParent();
	marketPage->SetSize( width,height );
	marketPage->CentreOnParent();
	productionPage->SetSize( width,height );
	productionPage->CentreOnParent();
	objectPage->SetSize( width,height );
	objectPage->CentreOnParent();
	unitPage->SetSize( width,height );
	unitPage->CentreOnParent();
}

void EditFrame::ShowEditFaction()
{
	HideAllPages();
	if( app->selectedFactions->GetCount() == 0 ) return;
	factionPage->LoadData();
	factionPage->Show();
}

void EditFrame::ShowEditRegion()
{
	HideAllPages();
	if( app->selectedRegions->GetCount() == 0 ) return;
	regionPage->LoadData();
	regionPage->Show();
}

void EditFrame::ShowEditMarket()
{
	HideAllPages();
	if( app->selectedMarkets->GetCount() == 0 ) return;
	marketPage->LoadData();
	marketPage->Show();
}

void EditFrame::ShowEditProduction()
{
	HideAllPages();
	if( app->selectedProductions->GetCount() == 0 ) return;
	productionPage->LoadData();
	productionPage->Show();
}

void EditFrame::ShowEditObject()
{
	HideAllPages();
	if( app->selectedObjects->GetCount() == 0 ) return;
	objectPage->LoadData();
	objectPage->Show();
}

void EditFrame::ShowEditUnit()
{
	HideAllPages();
	if( app->selectedUnits->GetCount() == 0 ) return;
	unitPage->LoadData();
	unitPage->Show();
}

void EditFrame::HideAllPages()
{
	factionPage->Show( false );
	regionPage->Show( false );
	marketPage->Show( false );
	productionPage->Show( false );
	objectPage->Show( false );
	unitPage->Show( false );
}

// ---------------------------------------------------------------------------
// EditPage
// ---------------------------------------------------------------------------

EditPage::EditPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
	     : wxScrolledWindow( parent, -1, pos, size, wxVSCROLL | wxHSCROLL )
{
	editWait = false;
	border = new wxStaticBox( this, -1, "" );
	sizerEdit = new wxStaticBoxSizer( border, wxVERTICAL );

	toolBar = new wxToolBar( this, -1, wxDefaultPosition, wxDefaultSize,
	                        wxTB_HORIZONTAL | wxNO_BORDER | wxTB_FLAT );

    wxBitmap* bitmaps[3];

    bitmaps[0] = new wxBitmap( add_xpm );
    bitmaps[1] = new wxBitmap( delete_xpm );
    bitmaps[2] = new wxBitmap( move_xpm );

	toolBar->AddTool( Edit_Tool_Add, "", *( bitmaps[0] ), "Add" );
	toolBar->AddTool( Edit_Tool_Delete, "", *( bitmaps[1] ), "Delete" );
	toolBar->AddTool( Edit_Tool_Move, "", *( bitmaps[2] ), "Move" );

    toolBar->Realize();

	sizerTool = new wxBoxSizer( wxHORIZONTAL );
	sizerTool->Add( toolBar, 1 );
    int i;
    for( i = 0; i < 3; i++ )
        delete bitmaps[i];
}

EditPage::~EditPage()
{
	delete border;
	delete sizerEdit;
}

void EditPage::LoadData()
{
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

void EditPage::OnSize( wxSizeEvent & event )
{
	int w,h;
	GetClientSize( &w, &h );
	if( w>0 && h>0 ) {
		int offset = toolBar->GetSize().y;
		SetScrollbars( 5, 10, w/5, ( h+offset )/10 );
		sizerEdit->SetDimension( 1, offset + 1, w-1, h-1 - offset );
		sizerTool->SetDimension( 1, 1, w-1, offset - 1 );
	}
}

void EditPage::CreateControl( wxTextCtrl ** ppText, wxWindowID id,
                        const wxString & label, bool numeric )
{
	wxSizer *sizerRow = new wxBoxSizer( wxHORIZONTAL );

	wxTextValidator * validator;
	if( numeric )
		validator = numStringValidator;
	else
		validator = textStringValidator;

	wxStaticText *lab = new wxStaticText( this, -1, label );
	wxTextCtrl *text = new wxTextCtrl( this, id, _T( "" ), wxDefaultPosition, wxDefaultSize, 0, *validator );
	sizerRow->Add( lab, 2 );
	sizerRow->Add( text, 3 );

	if( ppText )
		*ppText = text;

	sizerEdit->Add( sizerRow, 0, wxALL | wxGROW, 0 );
}

void EditPage::CreateControl( wxComboBox ** ppComboBox, wxWindowID id,
                        const wxString & label, bool sort )
{
	wxSizer *sizerRow = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *text = new wxStaticText( this, -1, label );
	long style = wxCB_DROPDOWN | wxCB_READONLY;
	if( sort )
		style = style | wxCB_SORT;

	wxComboBox *combo =	new wxComboBox( this, id, "", wxDefaultPosition,
									   wxDefaultSize, 0 ,NULL, style );

	sizerRow->Add( text, 2 );
	sizerRow->Add( combo, 3 );

	if( ppComboBox )
		*ppComboBox = combo;

	sizerEdit->Add( sizerRow, 0, wxALL | wxGROW, 0 );
}

void EditPage::CreateControl( wxButton ** ppButton, wxWindowID id,
                        const wxString & label )
{
	wxSizer *sizerRow = new wxBoxSizer( wxHORIZONTAL );
	wxStaticText *text = new wxStaticText( this, -1, label );

	wxButton *button = new wxButton( this, id, "", wxDefaultPosition );
	
	sizerRow->Add( text, 2 );
	sizerRow->Add( button, 3 );

	if( ppButton )
		*ppButton = button;

	sizerEdit->Add( sizerRow, 0, wxALL | wxGROW, 0 );
}

void EditPage::CreateButton( wxButton ** ppButton, wxWindowID id,
                        const wxString & label, int align )
{
	wxSizer *sizerRow = new wxBoxSizer( wxHORIZONTAL );
	wxButton *button = new wxButton( this, id, label, wxDefaultPosition );

	if( align == wxALIGN_RIGHT || align == wxALIGN_CENTRE )
		sizerRow->Add( 1, 1, 2 );
	sizerRow->Add( button, 3, wxGROW, 5 );
	if( align == wxALIGN_LEFT || align == wxALIGN_CENTRE )
		sizerRow->Add( 1, 1, 2 );

	if( ppButton )
		*ppButton = button;

	sizerEdit->Add( sizerRow, 0, wxALL | wxGROW, 0 );
}

void EditPage::AddToControl( wxTextCtrl * pText, long number )
{
	if( pText->GetValue() == "(...)" )
		return;
	if( pText->GetValue() == "" ) {
		*pText << number;
	} else {
		long num;
		pText->GetValue().ToLong( &num );
		if( num != number )
			pText->SetValue( "(...)" );
	}
}

void EditPage::AddToControl( wxTextCtrl * pText, const char * string )
{
	if( pText->GetValue() == "(...)" )
		return;

	if( pText->GetValue() == "" )
		pText->SetValue( string );
	else if( pText->GetValue() != string )
		pText->SetValue( "(...)" );
}

void EditPage::AddToControl( wxButton * pButton, const char * string )
{
	if( pButton->GetLabel() == "(...)" )
		return;

	if( pButton->GetLabel() == "" )
		pButton->SetLabel( string );
	else if( !string || pButton->GetLabel() != string )
		pButton->SetLabel( "(...)" );

}

void EditPage::AddToControl( wxComboBox * pCombo, int index )
{
	int i = pCombo->GetSelection();
	int j;
	int count = pCombo->GetCount();

	if( i == -1 ) {
		for( j = 0; j < count -1; j++ ) {
			if( pCombo->GetClientData( j ) == NULL ) continue;
			if( *( int * )pCombo->GetClientData( j ) == index ) {
				pCombo->SetSelection( j );
				return;
			}
		}
	}

	if( pCombo->GetClientData( i ) == NULL ) return;

	if( *( int * )pCombo->GetClientData( i ) == index ) return;

	for( j = 0; j < count -1; j++ ) {
		if( pCombo->GetClientData( j ) == NULL ) {
			pCombo->SetSelection( j );
			return;
		}
	}
}

void EditPage::UpdateControl( wxTextCtrl * pText, AString * str )
{
	if( pText->GetValue() != "" )  {
		*str = pText->GetValue().GetData();
	}
}

void EditPage::UpdateControl( wxTextCtrl * pText, int & num )
{
 	long number;
	if( pText->GetValue() != "" ) {
		pText->GetValue().ToLong( &number );
		num = number;
	}
}

void EditPage::UpdateControl( wxComboBox * pCombo, int & num )
{
	int i = pCombo->GetSelection();
	if( pCombo->GetClientData( i ) == NULL ) return;
	num = *( int * ) pCombo->GetClientData( i );
}

void EditPage::DeleteCombo( wxComboBox * pCombo )
{
	for( int i = 0; i < pCombo->GetCount()-1; i++ )
		delete ( int * ) pCombo->GetClientData( i );
}


ARegion * EditPage::GetRegionChoice( const wxString & message, const wxString & caption )
{
	int num = app->GetRegions()->Num();
	wxArrayString choices;
	choices.Alloc( num );
	AElemArray regions;
//	ARegion * regions = new ARegion[num];
	int count = 0;
	forlist( app->GetRegions() ) {
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

EditRegionPage::EditRegionPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	CreateControl( &editNumber, Edit_Region_Number, "Number: ", true );
	CreateControl( &editName, Edit_Region_Name, "Name: " );
	CreateControl( &editType, Edit_Region_Type, "Type: *" );
	CreateControl( &editRace, Edit_Region_Race, "Race: " );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( &editPop, Edit_Region_Pop, "Pop: ", true );
	CreateControl( &editBasepop, Edit_Region_Basepop, "Base Pop: ", true );
	CreateControl( &editWages, Edit_Region_Wages, "Wages: ", true );
	CreateControl( &editMaxwages, Edit_Region_Maxwages, "Max Wages: ", true );
	CreateControl( &editMoney, Edit_Region_Money, "Money: ", true );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( &editGate, Edit_Region_Gate, "Gate: ", true );
	CreateControl( &editBuildingseq, Edit_Region_Buildingseq, "Building seq: ", true );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	// Town
	CreateButton( &editTown, Edit_Region_Town, "Town...", wxALIGN_LEFT);
//	wxSizer * sizerRow = new wxBoxSizer( wxHORIZONTAL );
//	wxStaticText * text = new wxStaticText( this, -1, "Town:" );
//	sizerRow->Add( 1,1,1 );
//	sizerRow->Add( text, 0, wxALIGN_CENTRE_VERTICAL );
//	sizerRow->Add( 1,1,1 );
//	sizerEdit->Add( sizerRow, 0, wxALL | wxGROW );
	
	CreateControl( &editTownName, Edit_Region_TownName, " Name: " );
	CreateControl( &editTownPop, Edit_Region_TownPop, " Pop: ", true );
	CreateControl( &editTownBasepop, Edit_Region_TownBasepop, " Base Pop: ", true );
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

EditRegionPage::~EditRegionPage()
{
}

void EditRegionPage::LoadData()
{
	editWait = true;
	EnableAllControls( true );
	ClearAllControls();

	int count = abs( app->selectedRegions->GetCount() );
	AString temp;
	if( count == 1 ) {
		ARegion * r = ( ARegion * ) app->selectedRegions->Item( 0 );
		temp = AString( "( " ) + r->xloc + ", " + r->yloc + ", " +
						   r->zloc + " ) ";
		border->SetTitle( temp.Str() );
	} else {
		editTown->Enable( false );
		editTownName->Enable( false );
		editTownPop->Enable( false );
		editTownBasepop->Enable( false );
		temp = AString( count ) + " Regions";
		border->SetTitle( temp.Str() );
	}

	for( int i = 0; i < count; i++ ) {
		ARegion * r = ( ARegion * ) app->selectedRegions->Item( i );
		AddToControl( editType, TerrainDefs[r->type].name );
		AddToControl( editRace, ( r->race != -1 ? ItemDefs[r->race].names : "" ) );
		AddToControl( editNumber, r->num );
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

		if( r->town && count == 1) {
			editTown->SetLabel( "Remove town..." );
			AddToControl( editTownName, r->town->name->Str() );
			AddToControl( editTownPop, r->town->pop );
			AddToControl( editTownBasepop, r->town->basepop );
		} else {
			editTown->SetLabel( "Add Town..." );
			editTownName->Enable( false );
			editTownPop->Enable( false );
			editTownBasepop->Enable( false );
		}
	}
	editWait = false;

}

void EditRegionPage::EnableAllControls( bool b )
{
	editNumber->Enable( b );
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

void EditRegionPage::ClearAllControls()
{
	border->SetTitle( "Region" );
	editNumber->Clear();
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

void EditRegionPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;
	for( int i = 0; i < abs( app->selectedRegions->GetCount() ); i++ ) {
		ARegion * r = ( ARegion * ) app->selectedRegions->Item( i );
	
		switch ( event.GetId() )
		{
		case Edit_Region_Number:
			UpdateControl( editNumber, r->num );
			break;
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
			}
			break;
		case Edit_Region_TownPop:
			if( r->town ) {
				UpdateControl( editTownPop, r->town->pop );
			}
			break;
		case Edit_Region_TownBasepop:
			if( r->town ) {
				UpdateControl( editTownBasepop, r->town->basepop );
			}
			break;
		}
	}
	app->UpdateRegions();
}

void EditRegionPage::OnButton( wxCommandEvent & event )
{
	if( editWait )
		return;

	int dir = -1;
	int pw, ph;
	GetParent()->GetSize( &pw,&ph );

	if( event.GetId() == Edit_Tool_Add_Market ) {
		app->selectedMarkets->Clear();
		for( int i = 0; i < abs( app->selectedRegions->GetCount() ); i++ ) {
			ARegion * r = ( ARegion * ) app->selectedRegions->Item( i );
			Market * m = new Market( M_BUY, 0, 0, 0, 0, 0, 0, 0 );
			r->markets.Add( m );
			m->region = r->num;
			frame->tree->canvas->AddItem( m );
			app->SelectMarket( m, true );
		}
	} else if( event.GetId() == Edit_Tool_Add_Production ) {
		app->selectedProductions->Clear();
		for( int i = 0; i < abs( app->selectedRegions->GetCount() ); i++ ) {
			ARegion * r = ( ARegion * ) app->selectedRegions->Item( i );
			Production * p = new Production( 0 ,0 );
			r->products.Add( p );
			p->region = r->num;
			frame->tree->canvas->AddItem( p );
			app->SelectProduction( p, true );
		}
	} else if( event.GetId() == Edit_Tool_Add_Object ) {
		app->selectedObjects->Clear();
		for( int i = 0; i < abs( app->selectedRegions->GetCount() ); i++ ) {
			ARegion * r = ( ARegion * ) app->selectedRegions->Item( i );
			Object * o = new Object( r );
			o->num = r->buildingseq++;
			o->name = new AString( AString( ObjectDefs[O_DUMMY].name ) + " [" + o->num + "]" );
			r->objects.Add( o );
			frame->tree->canvas->AddItem( o, false );
			app->SelectObject(o, true);
		}
	} else if( event.GetId() == Edit_Tool_Add_Unit ) {
		app->selectedUnits->Clear();
		for( int i = 0; i < abs( app->selectedRegions->GetCount() ); i++ ) {
			ARegion * r = ( ARegion * ) app->selectedRegions->Item( i );
			Unit * u = app->AddUnit();
			u->MoveUnit( r->GetDummy() );
			frame->tree->canvas->AddItem( u );
			app->SelectUnit(u, true);
		}
	} else if( event.GetId() == Edit_Region_Type ) {
		//open region selection window

		//need bigger window for this one
		EditTerrainTypeAux aux( this, wxPoint( pw, 0 ), wxSize( 360, ph/2 ) );

		aux.Init( app->selectedRegions );
		aux.ShowModal();

		//update edit window and objects
		editType->SetLabel( "" );
		if( aux.array.GetCount()>0 ) {
			for( int i = 0; i < abs( app->selectedRegions->GetCount() ); i++ ) {
				ARegion * r = ( ARegion * ) app->selectedRegions->Item( i );

				int type = ( ( ARegion * ) aux.array[0] )->type;
				r->type = type;
				// recreate region data?
				if( app->recreateData ) {
					// delete markets
					int i;
					int count = abs( app->selectedMarkets->GetCount() ) -1;
					for( i = count; i >= 0; i-- ) {
						Market * m = ( Market * ) app->selectedMarkets->Item( i );
						app->TrashMarket( m );
					}
					// delete products
					count = abs( app->selectedProductions->GetCount() ) -1;
					for( i = count; i >= 0; i-- ) {
						Production * p = ( Production * ) app->selectedProductions->Item( i );
						app->TrashProduction( p );
					}

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

				}
				AddToControl( editType, TerrainDefs[type].name );
			}
			if( app->recreateData )
				LoadData();
		}
		WX_CLEAR_ARRAY( aux.array );
	} else if( event.GetId() == Edit_Region_Race ) {

		//open item selection window

		EditItemTypeAux aux( this, wxPoint( pw, 0 ), wxSize( 180, ph/2 ) );

		Item * it = NULL;
		AElemArray selected;
		for( int i = 0; i < abs( app->selectedRegions->GetCount() ); i++ ) {
			ARegion * r = ( ARegion * ) app->selectedRegions->Item( i );
			it = new Item();
			it->type = r->race;
			selected.Add( it );
		}
		aux.Init( &selected, IT_MAN );

		aux.ShowModal();

		//update edit window and objects
		editRace->SetLabel( "" );
		if( aux.array.GetCount()>0 ) {
			for( int i = 0; i < abs( app->selectedRegions->GetCount() ); i++ ) {
				ARegion * r = ( ARegion * ) app->selectedRegions->Item( i );

				int type = ( ( Item * ) aux.array[0] )->type;
				r->race = type;
				AddToControl( editRace, ItemDefs[type].names );
			}
		}
		WX_CLEAR_ARRAY( selected );
		WX_CLEAR_ARRAY( aux.array );
	} else if( event.GetId() == Edit_Region_Town ) {
		ARegion * r = (ARegion *) app->selectedRegions->Item( 0 );
		if( r->town ) {
			int answer = wxMessageBox( "Delete town?", "Confirm",
			                           wxOK | wxCANCEL | wxCENTRE |
									   wxICON_QUESTION, frame);
			if( answer == wxOK ) {
				delete r->town;
				r->town = 0;
			}
		} else {
				r->town = new TownInfo;
				r->town->name = new AString("");
		}
		LoadData();
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
		EditRegionAux aux( this, wxPoint( pw,0 ), wxSize( 180,ph/2 ) );

		AElemArray selected;
		for( int i = 0; i < abs( app->selectedRegions->GetCount() ); i++ ) {
			ARegion * r = ( ARegion * ) app->selectedRegions->Item( i );
			selected.Add( r->neighbors[dir] );
		}
		aux.Init( &selected );

		aux.ShowModal();

		//update edit window and objects
		if( aux.array.GetCount()>0 ) {
			editExits[dir]->SetLabel( "" );
			for( int i = 0; i < abs( app->selectedRegions->GetCount() ); i++ ) {
				ARegion * r = ( ARegion * ) app->selectedRegions->Item( i );
				ARegion * r2 = ( ARegion * ) aux.array[0];
				r->neighbors[dir] = r2;
				AString temp = AString( "( " ) + r2->xloc + ", " + r2->yloc + ", "
				               + r2->zloc + " )";
				editExits[dir]->SetLabel( temp.Str() );
			}
		
		}
		selected.Clear();
		aux.array.Clear();
	}
	app->UpdateRegions();
	
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
// EditFactionPage
// ---------------------------------------------------------------------------

EditFactionPage::EditFactionPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	CreateControl( &editNumber, Edit_Faction_Number, "Number: ", true );
	CreateControl( &editName, Edit_Faction_Name, "Name: " );
	CreateControl( &editAddress, Edit_Faction_Address, "Address: " );
	CreateControl( &editPassword, Edit_Faction_Password, "Password: " );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( &editUnclaimed, Edit_Faction_Unclaimed, "Unclaimed: ", true );
	CreateControl( &editTimes, Edit_Faction_Times, "Times: ", true );
	CreateControl( &editTemformat, Edit_Faction_Temformat, "Template: " );
	CreateControl( &editQuit, Edit_Faction_Quit, "Quit: " );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( &editLastChange, Edit_Faction_LastChange, "Last change: " );
	CreateControl( &editLastOrders, Edit_Faction_LastOrders, "Last orders: ", true );

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

EditFactionPage::~EditFactionPage()
{
	DeleteCombo( editTemformat );
	DeleteCombo( editQuit );
}

void EditFactionPage::LoadData()
{
	editWait = true;
	EnableAllControls( true );
	ClearAllControls();

	int count = app->selectedFactions->GetCount();
	if( count == 1 ) {
		Faction * f = ( Faction * ) app->selectedFactions->Item( 0 );
		border->SetTitle( f->name->Str() );
	} else {
		AString temp = AString( count ) + " Factions";
		border->SetTitle( temp.Str() );
	}

	for( int i = 0; i < count; i++ ) {
		Faction * f = ( Faction * ) app->selectedFactions->Item( i );
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
	}
	editWait = false;

}

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

void EditFactionPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;
	for( int i = 0; i < abs( app->selectedFactions->GetCount() ); i++ ) {
		Faction * f = ( Faction * ) app->selectedFactions->Item( i );
	
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

void EditFactionPage::OnButton( wxCommandEvent & event )
{
	if( editWait )
		return;

	if( event.GetId() == Edit_Tool_Add_Unit ) {
		ARegion * r = GetRegionChoice( "Select region to add unit(s) to.", "New unit" );
		if( !r ) return;

		app->selectedUnits->Clear();
		for( int i = 0; i < abs( app->selectedFactions->GetCount() ); i++ ) {
			Faction * f = ( Faction * ) app->selectedFactions->Item( i );
			Unit * u = app->AddUnit(f);
			u->MoveUnit( r->GetDummy() );
			frame->tree->canvas->AddItem( u );
			app->SelectUnit(u, true);
		}
	}
}

void EditFactionPage::OnToolAdd( wxCommandEvent & event )
{
	Faction * f = app->AddFaction();
	frame->tree->canvas->AddItem( f );
	app->SelectFaction( f );
}

void EditFactionPage::OnToolDelete( wxCommandEvent & event )
{
	if( app->selectedFactions->GetCount() == 0 ) return;

	int answer = wxMessageBox( "Really delete faction(s) and units?", "Confirm",
	                          wxYES_NO | wxCENTRE | wxICON_QUESTION );

	if( answer == wxYES ) {
		for( int i = abs( app->selectedFactions->GetCount()-1 ); i >= 0; i-- ) {
			Faction * f = ( Faction * ) app->selectedFactions->Item( i );
			{
				forlist( app->GetRegions() ) {
					ARegion * r = ( ARegion * ) elem;
					forlist( &r->objects ) {
						Object * o = ( Object * ) elem;
						forlist( &o->units ) {
							Unit * u = ( Unit * ) elem;
							if( u->faction == f ) {
								app->TrashUnit( u );
							}
						}
					}
				}
			}
			app->TrashFaction( f );
		}
	}
	( ( EditFrame * )GetParent() )->HideAllPages();

}

void EditFactionPage::OnToolMove( wxCommandEvent & event )
{
}

// ---------------------------------------------------------------------------
// EditMarketPage
// ---------------------------------------------------------------------------

EditMarketPage::EditMarketPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	CreateControl( &editType, Edit_Market_Type, "Type: " );
	CreateControl( &editItem, Edit_Market_Item, "Item: ", true );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( &editPrice, Edit_Market_Price, "Price: ", true );
	CreateControl( &editAmount, Edit_Market_Amount, "Amount: ", true );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( &editMinpop, Edit_Market_Minpop, "Min pop: ", true );
	CreateControl( &editMaxpop, Edit_Market_Maxpop, "Max pop: ", true );
	CreateControl( &editMinamt, Edit_Market_Minamt, "Min amount: ", true );
	CreateControl( &editMaxamt, Edit_Market_Maxamt, "Max amount: ", true );

	// Fill Combo boxes
	editType->Append( "Sell", new int( M_BUY ) );
	editType->Append( "Buy", new int( M_SELL ) );
	editType->Append( "(...)" );

	for( int i=0; i < NITEMS; i++ ) {
		if( ItemDefs[i].flags & ItemType::DISABLED ||
			ItemDefs[i].flags & ItemType::NOMARKET ) continue;
		editItem->Append( ItemDefs[i].names, new int( i ) );
	}
	editItem->Append( "(...)" );

	toolBar->EnableTool( Edit_Tool_Add, true );
	toolBar->EnableTool( Edit_Tool_Delete, true );
	toolBar->EnableTool( Edit_Tool_Move, true );
}

EditMarketPage::~EditMarketPage()
{
	DeleteCombo( editType );
	DeleteCombo( editItem );
}

void EditMarketPage::LoadData()
{
	editWait = true;
	EnableAllControls( true );
	ClearAllControls();

	int count = app->selectedMarkets->GetCount();
	if( count == 1 ) {
		border->SetTitle( "Market" );
	} else {
		AString temp = AString( count ) + " Markets";
		border->SetTitle( temp.Str() );
	}

	for( int i = 0; i < count; i++ ) {
		Market * m = ( Market * ) app->selectedMarkets->Item( i );

		AddToControl( editPrice, m->price );
		AddToControl( editAmount, m->amount );
		AddToControl( editMinpop, m->minpop );
		AddToControl( editMaxpop, m->maxpop );
		AddToControl( editMinamt, m->minamt );
		AddToControl( editMaxamt, m->maxamt );
		AddToControl( editType, m->type );
		AddToControl( editItem, m->item );	
	}
	editWait = false;

}

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

void EditMarketPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;
	for( int i = 0; i < abs( app->selectedMarkets->GetCount() ); i++ ) {
		Market * m = ( Market * ) app->selectedMarkets->Item( i );
	
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

void EditMarketPage::OnButton( wxCommandEvent & event ){
}

void EditMarketPage::OnToolAdd( wxCommandEvent & event )
{
	Market * mOld = ( Market * ) app->selectedMarkets->Item(0);
	ARegion * r = app->GetRegions()->GetRegion(mOld->region);
	Market * m = new Market( M_BUY, 0, 0, 0, 0, 0, 0, 0 );
	r->markets.Add( m );
	m->region = r->num;
	frame->tree->canvas->AddItem( m );
	app->SelectMarket( m );
}

void EditMarketPage::OnToolDelete( wxCommandEvent & event )
{
	if( app->selectedMarkets->GetCount() == 0 ) return;

	int answer = wxMessageBox( "Really delete market(s)?", "Confirm",
	                          wxYES_NO | wxCENTRE | wxICON_QUESTION );

	if( answer == wxYES ) {
		for( int i = abs( app->selectedMarkets->GetCount()-1 ); i >= 0; i-- ) {
			Market * m = ( Market * ) app->selectedMarkets->Item( i );
			app->TrashMarket( m );
		}
	}
	( ( EditFrame * )GetParent() )->HideAllPages();

}

void EditMarketPage::OnToolMove( wxCommandEvent & event )
{
	ARegion * newReg = GetRegionChoice( "Select region to move to.", "Move market" );
	if( !newReg ) return;

	for( int i = 0; i < abs( app->selectedMarkets->GetCount() ); i++ )
	{
		Market * m = ( Market * ) app->selectedMarkets->Item( i );
		ARegion * oldReg = app->GetRegions()->GetRegion( m->region );
		frame->tree->canvas->RemoveItem( m );
		oldReg->markets.Remove( m );
		newReg->markets.Add( m );
		m->region = newReg->num;
		frame->tree->canvas->AddItem( m );
	}
}

// ---------------------------------------------------------------------------
// EditProductionPage
// ---------------------------------------------------------------------------

EditProductionPage::EditProductionPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	CreateControl( &editItemtype, Edit_Production_Itemtype, "Item: ", true );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( &editAmount, Edit_Production_Amount, "Amount: ", true );
	CreateControl( &editBaseamount, Edit_Production_Baseamount, "Base amount: ", true );

	// Fill Combo boxes
	for( int i = 0; i < NITEMS; i++ ) {
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

EditProductionPage::~EditProductionPage()
{
	DeleteCombo( editItemtype );
}

void EditProductionPage::LoadData()
{
	editWait = true;
	EnableAllControls( true );
	ClearAllControls();

	int count = app->selectedProductions->GetCount();
	if( count == 1 ) {
		border->SetTitle( "Production" );
	} else {
		AString temp = AString( count ) + " Productions";
		border->SetTitle( temp.Str() );
	}

	for( int i = 0; i < count; i++ ) {
		Production * p = ( Production * ) app->selectedProductions->Item( i );

		AddToControl( editAmount, p->amount );
		AddToControl( editBaseamount, p->baseamount );
		AddToControl( editItemtype, p->itemtype );
	}
	editWait = false;

}

void EditProductionPage::EnableAllControls( bool b )
{
	editItemtype->Enable( b );
	editAmount->Enable( b );
	editBaseamount->Enable( b );
}

void EditProductionPage::ClearAllControls()
{
	border->SetTitle( "Production" );
	editItemtype->SetSelection( -1 );
	editAmount->Clear();
	editBaseamount->Clear();
}

void EditProductionPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;
	for( int i = 0; i < abs( app->selectedProductions->GetCount() ); i++ ) {
		Production * p = ( Production * ) app->selectedProductions->Item( i );
	
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

void EditProductionPage::OnToolAdd( wxCommandEvent & event )
{
	Production * pOld = ( Production * ) app->selectedProductions->Item(0);
	ARegion * r = app->GetRegions()->GetRegion(pOld->region);
	Production * p = new Production( 0 ,0 );
	r->products.Add( p );
	p->region = r->num;
	frame->tree->canvas->AddItem( p );
	app->SelectProduction( p );

}

void EditProductionPage::OnToolDelete( wxCommandEvent & event )
{
	if( app->selectedProductions->GetCount() == 0 ) return;

	int answer = wxMessageBox( "Really delete production(s)?", "Confirm",
	                          wxYES_NO | wxCENTRE | wxICON_QUESTION );

	if( answer == wxYES ) {
		for( int i = abs( app->selectedProductions->GetCount()-1 ); i >= 0; i-- ) {
			Production * p = ( Production * ) app->selectedProductions->Item( i );
			app->TrashProduction( p );
		}
	}
	( ( EditFrame * )GetParent() )->HideAllPages();

}

void EditProductionPage::OnToolMove( wxCommandEvent & event )
{
	ARegion * newReg = GetRegionChoice( "Select region to move to.", "Move product" );
	if( !newReg ) return;

	for( int i = 0; i < abs( app->selectedProductions->GetCount() ); i++ )
	{
		Production * p = ( Production * ) app->selectedProductions->Item( i );
		ARegion * oldReg = app->GetRegions()->GetRegion( p->region );
		frame->tree->canvas->RemoveItem( p );
		oldReg->products.Remove( p );
		newReg->products.Add( p );
		p->region = newReg->num;
		frame->tree->canvas->AddItem( p );
	}
}

// ---------------------------------------------------------------------------
// EditObjectPage
// ---------------------------------------------------------------------------

EditObjectPage::EditObjectPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	CreateControl( &editNum, Edit_Object_Num, "Number: ", true );
	CreateControl( &editType, Edit_Object_Type, "Type: ", true );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( &editName, Edit_Object_Name, "Name: " );
	CreateControl( &editDescribe, Edit_Object_Describe, "Describe: " );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( &editInner, Edit_Object_Inner, "Inner: " );
	CreateControl( &editRunes, Edit_Object_Runes, "Runes: ", true );
	CreateControl( &editIncomplete, Edit_Object_Incomplete, "Incomplete: ", true );

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

EditObjectPage::~EditObjectPage()
{
	DeleteCombo( editType );
}

void EditObjectPage::LoadData()
{
	editWait = true;
	EnableAllControls( true );
	ClearAllControls();

	int count = app->selectedObjects->GetCount();
	if( count == 1 ) {
		Object * o = ( Object * ) app->selectedObjects->Item( 0 );
		border->SetTitle( o->name->Str() );
	} else {
		AString temp = AString( count ) + " Objects";
		border->SetTitle( temp.Str() );
	}

	bool dummy = false;
	for( int i = 0; i < count; i++ ) {
		Object * o = ( Object * ) app->selectedObjects->Item( i );
		AString temp;

		if( o->region->GetDummy() == o ) dummy = true;

		if( o->inner != -1 ) {
			ARegion * r = app->GetRegions()->GetRegion( o->inner );
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
	}
	if( dummy ) {
		toolBar->EnableTool( Edit_Tool_Delete, false );
		toolBar->EnableTool( Edit_Tool_Move, false );
	} else {
		toolBar->EnableTool( Edit_Tool_Delete, true );
		toolBar->EnableTool( Edit_Tool_Move, true );
	}
	editWait = false;
}

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

void EditObjectPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;
	for( int i = 0; i < abs( app->selectedObjects->GetCount() ); i++ ) {
		Object * o = ( Object * ) app->selectedObjects->Item( i );

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

void EditObjectPage::OnButton( wxCommandEvent & event ){
	if( editWait ) return;
	int pw,ph;
	GetParent()->GetSize( &pw,&ph );

	if( event.GetId() == Edit_Tool_Add_Unit ) {
		app->selectedUnits->Clear();
		for( int i = 0; i < abs( app->selectedObjects->GetCount() ); i++ ) {
			Object * o = ( Object * ) app->selectedObjects->Item( i );
			Unit * u = app->AddUnit();
			u->MoveUnit( o );
			frame->tree->canvas->AddItem( u );
			app->SelectUnit(u, true);
		}
	} else if( event.GetId() == Edit_Object_Inner ){
		EditRegionAux aux( this, wxPoint( pw,0 ), wxSize( 180,ph/2 ) );

		AElemArray selected;
		for( int i = 0; i < abs( app->selectedObjects->GetCount() ); i++ ) {
 		Object * o = ( Object * ) app->selectedObjects->Item( i );
			ARegion * r = app->GetRegions()->GetRegion( o->inner );
			selected.Add( r );
		}
		aux.Init( &selected );

		aux.ShowModal();

		//update edit window and objects
		editInner->SetLabel( "" );
		if( aux.array.GetCount()>0 ) {
			for( int i = 0; i < abs( app->selectedObjects->GetCount() ); i++ ) {
				Object * o = ( Object * ) app->selectedObjects->Item( i );
				ARegion * r = ( ARegion * ) aux.array[0];
				o->inner = r->num;
				AString temp = AString( "( " ) + r->xloc + ", " + r->yloc + ", "
				               + r->zloc + " )";
				editInner->SetLabel( temp.Str() );
			}
		
		}
		selected.Clear();
		aux.array.Clear();
	}
	app->UpdateObjects();
}

void EditObjectPage::OnToolAdd( wxCommandEvent & event )
{
	Object * oOld = ( Object * ) app->selectedObjects->Item(0);
	ARegion * r = oOld->region;
	Object * o = new Object( r );
	o->num = r->buildingseq++;
	o->name = new AString( AString( ObjectDefs[O_DUMMY].name ) + " [" + o->num + "]" );
	r->objects.Add( o );
	frame->tree->canvas->AddItem( o, false );
	app->SelectObject(o);
}

void EditObjectPage::OnToolDelete( wxCommandEvent & event )
{
	if( app->selectedObjects->GetCount() == 0 ) return;

	int answer = wxMessageBox( "Really delete object(s)?", "Confirm",
	                          wxYES_NO | wxCENTRE | wxICON_QUESTION );

	if( answer == wxYES ) {
		for( int i = abs( app->selectedObjects->GetCount()-1 ); i >= 0; i-- ) {
			Object * o = ( Object * ) app->selectedObjects->Item( i );
			app->TrashObject( o );
		}
	}
	( ( EditFrame * )GetParent() )->HideAllPages();

}

void EditObjectPage::OnToolMove( wxCommandEvent & event )
{
	ARegion * newReg = GetRegionChoice( "Select region to move to.", "Move object" );
	if( !newReg ) return;

	for( int i = 0; i < abs( app->selectedObjects->GetCount() ); i++ )
	{
		Object * o = ( Object * ) app->selectedObjects->Item( i );
		ARegion * oldReg = o->region;
		frame->tree->canvas->RemoveItem( o );
		oldReg->objects.Remove( o );
		newReg->objects.Add( o );
		o->region = newReg;
		frame->tree->canvas->AddItem( o );

	}
}

// ---------------------------------------------------------------------------
// EditUnitPage
// ---------------------------------------------------------------------------

EditUnitPage::EditUnitPage( wxWindow *parent, const wxPoint& pos,
                   const wxSize& size )
         : EditPage( parent, pos, size )
{
	CreateControl( &editNum, Edit_Unit_Num, "Number: ", true );
	CreateControl( &editName, Edit_Unit_Name, "Name: " );
	CreateControl( &editDescribe, Edit_Unit_Describe, "Describe: " );
	CreateControl( &editFaction, Edit_Unit_Faction, "Faction: " );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( &editType, Edit_Unit_Type, "Type: " );
	CreateControl( &editGuard, Edit_Unit_Guard, "Guard: " );
	CreateControl( &editReveal, Edit_Unit_Reveal, "Reveal: " );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateControl( &editCombat, Edit_Unit_Combat, "Combat: " );
	CreateControl( &editReadyItem, Edit_Unit_ReadyItem, "Ready Item: " );
	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );

	CreateButton( &editItems, Edit_Unit_Items, "Items--->", wxALIGN_RIGHT );
	CreateButton( &editSkills, Edit_Unit_Skills, "Skills--->", wxALIGN_RIGHT );
//	sizerEdit->Add( new wxBoxSizer( wxHORIZONTAL ), 0, wxALL | wxGROW, 0 );
//	CreateButton( &editItems, Edit_Unit_ReadyWeapon, "Ready weapons--->" );
//	CreateButton( &editSkills, Edit_Unit_ReadyArmor, "Ready armors--->" );

	// Fill Combo boxes
	editType->Append( "Normal", new int( U_NORMAL ) );
	editType->Append( "Mage", new int( U_MAGE ) );
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

EditUnitPage::~EditUnitPage()
{
	DeleteCombo( editType );
	DeleteCombo( editGuard );
	DeleteCombo( editReveal );
}

void EditUnitPage::LoadData()
{
	editWait = true;
	EnableAllControls( true );
	ClearAllControls();

	int count = app->selectedUnits->GetCount();
	if( count == 1 ) {
		Unit * u = ( Unit * ) app->selectedUnits->Item( 0 );
		border->SetTitle( u->name->Str() );
	} else {
		AString temp = AString( count ) + " Units";
		border->SetTitle( temp.Str() );
	}

	for( int i = 0; i < count; i++ ) {
		Unit * u = ( Unit * ) app->selectedUnits->Item( i );
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
	}
	editWait = false;
}

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

void EditUnitPage::OnTextUpdate( wxCommandEvent & event )
{
	if( editWait ) return;
	for( int i = 0; i < abs( app->selectedUnits->GetCount() ); i++ ) {
		Unit * u = ( Unit * ) app->selectedUnits->Item( i );
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

void EditUnitPage::OnButton( wxCommandEvent & event )
{
	if( editWait ) return;

	int pw,ph;
	GetParent()->GetSize( &pw,&ph );

	if( event.GetId() == Edit_Unit_Combat ){
		EditSkillTypeAux aux( this, wxPoint( pw,0 ), wxSize( 180,ph/2 ) );

		Skill * s = NULL;
		AElemArray selected;
		for( int i = 0; i < abs( app->selectedUnits->GetCount() ); i++ ) {
			Unit * u = ( Unit * ) app->selectedUnits->Item( i );
			s = new Skill();
			s->type = u->combat;
			selected.Add( s );
		}
		aux.Init( &selected, SkillType::COMBAT );

		aux.ShowModal();

		//update edit window and objects
		editCombat->SetLabel( "" );
		if( aux.array.GetCount()>0 ) {
			for( int i = 0; i < abs( app->selectedUnits->GetCount() ); i++ ) {
				Unit * u = ( Unit * ) app->selectedUnits->Item( i );

				int type = ( ( Skill * ) aux.array[0] )->type;
				u->combat = type;
				AddToControl( editCombat, SkillDefs[type].name );
			}
		}
		WX_CLEAR_ARRAY( selected );
		WX_CLEAR_ARRAY( aux.array );

	} else if( event.GetId() == Edit_Unit_ReadyItem ){
		EditItemTypeAux aux( this, wxPoint( pw,0 ), wxSize( 180,ph/2 ) );

		Item * it = NULL;
		AElemArray selected;
		for( int i = 0; i < abs( app->selectedUnits->GetCount() ); i++ ) {
			Unit * u = ( Unit * ) app->selectedUnits->Item( i );
			it = new Item();
			it->type = u->readyItem;
			selected.Add( it );
		}
		aux.Init( &selected );

		aux.ShowModal();

		//update edit window and objects
		editReadyItem->SetLabel( "" );
		if( aux.array.GetCount()>0 ) {
			for( int i = 0; i < abs( app->selectedUnits->GetCount() ); i++ ) {
				Unit * u = ( Unit * ) app->selectedUnits->Item( i );

				int type = ( ( Item * ) aux.array[0] )->type;
				u->readyItem = type;
				AddToControl( editReadyItem, ItemDefs[type].name );
			}
		}
		WX_CLEAR_ARRAY( selected );
		WX_CLEAR_ARRAY( aux.array );

	} else if( event.GetId() == Edit_Unit_Faction ){
		EditFactionAux aux( this, wxPoint( pw,0 ), wxSize( 180,ph/2 ) );

		AElemArray selected;
		for( int i = 0; i < abs( app->selectedUnits->GetCount() ); i++ ) {
			Unit * u = ( Unit * ) app->selectedUnits->Item( i );
			selected.Add( u->faction );
		}
		aux.Init( &selected );
		aux.ShowModal();

		//update edit window and objects
		editFaction->SetLabel( "" );
		if( aux.array.GetCount()>0 ) {
			for( int i = 0; i < abs( app->selectedUnits->GetCount() ); i++ ) {
				Unit * u = ( Unit * ) app->selectedUnits->Item( i );
				u->faction = ( Faction * ) aux.array[0];
				AddToControl( editFaction, u->faction->name->Str() );
			}
		
		}
		selected.Clear();
		aux.array.Clear();
	} else if( event.GetId() == Edit_Unit_Items ) {
		EditItemAux aux( this, wxPoint( pw,0 ), wxSize( 180,ph/2 ) );

		aux.Init( app->selectedUnits );
		aux.ShowModal();

		//update edit window and objects
		if( aux.array.GetCount()>0 ) {
			for( int i = 0; i < abs( app->selectedUnits->GetCount() ); i++ ) {
				Unit * u = ( Unit * ) app->selectedUnits->Item( i );
				for( int j = 0; j < abs( aux.array.GetCount() ); j++ ) {
					Item * it = ( Item * ) aux.array[j];
					u->items.SetNum( it->type, it->num );
				}
			}
		}
		WX_CLEAR_ARRAY( aux.array );

	} else if( event.GetId() == Edit_Unit_Skills ) {
		EditSkillAux aux( this, wxPoint( pw,0 ), wxSize( 180,ph/2 ) );

		aux.Init( app->selectedUnits );
		aux.ShowModal();

		//update edit window and objects
		if( aux.array.GetCount()>0 ) {
			for( int i = 0; i < abs( app->selectedUnits->GetCount() ); i++ ) {
				Unit * u = ( Unit * ) app->selectedUnits->Item( i );
				for( int j = 0; j < abs( aux.array.GetCount() ); j++ ) {
					Skill * s = ( Skill * ) aux.array[j];
					u->skills.SetDays( s->type, s->days * u->GetMen() );
				}
			}
		
		}
		WX_CLEAR_ARRAY( aux.array );
	}
	app->UpdateUnits();
}


void EditUnitPage::OnToolAdd( wxCommandEvent & event )
{
	Unit * uOld = ( Unit * ) app->selectedUnits->Item( 0 );
	Object * o = uOld->object;
	Faction * f = uOld->faction;
	Unit * u = app->AddUnit( f );
	u->MoveUnit( o );
	frame->tree->canvas->AddItem( u );
	app->SelectUnit(u);
}

void EditUnitPage::OnToolDelete( wxCommandEvent & event )
{
	if( app->selectedUnits->GetCount() == 0 ) return;

	int answer = wxMessageBox( "Really delete unit(s)?", "Confirm",
	                          wxYES_NO | wxCENTRE | wxICON_QUESTION );

	if( answer == wxYES ) {
		for( int i = abs( app->selectedUnits->GetCount()	-1 ); i >= 0; i-- ) {
			Unit * u = ( Unit * ) app->selectedUnits->Item( i );
			app->TrashUnit( u );
		}
	}
	( ( EditFrame * )GetParent() )->HideAllPages();
}

void EditUnitPage::OnToolMove( wxCommandEvent & event )
{
	ARegion * newReg = GetRegionChoice( "Select region to move to.", "Move unit" );

	if( !newReg ) return;

	for( int i = 0; i < abs( app->selectedUnits->GetCount() ); i++ )
	{
		Unit * u = ( Unit * ) app->selectedUnits->Item( i );
		frame->tree->canvas->RemoveItem( u );
		u->MoveUnit( newReg->GetDummy() );
		frame->tree->canvas->AddItem( u );
	}

}