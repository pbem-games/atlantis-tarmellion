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

#include "movedlg.h"
#include "gui.h"

BEGIN_EVENT_TABLE( MoveDialog, wxDialog )

    EVT_TEXT( Move_X, MoveDialog::OnChangeRegion )
    EVT_TEXT( Move_Y, MoveDialog::OnChangeRegion )
    EVT_COMBOBOX( Move_Z, MoveDialog::OnChangeRegion )
    EVT_BUTTON( wxID_OK, MoveDialog::OnOK )
    EVT_BUTTON( wxID_CANCEL, MoveDialog::OnCancel )
	
END_EVENT_TABLE()

/**
 * Default constructor
 * Create controls
 */
MoveDialog::MoveDialog( wxWindow *parent, Object * initObject, Object ** moveToObject,
					    bool chooseObject )
           :wxDialog( parent, -1, wxString( "Move" ), wxDefaultPosition, wxSize( 330, 160 ) )
{
	m_moveToObject = moveToObject;
	m_chooseObject = chooseObject;
//	wxTextValidator * numStringValidator = new wxTextValidator( wxFILTER_NUMERIC );

	// Init sizers
	wxStaticBox * border = new wxStaticBox( this, -1, "" );	
	m_sizerMove = new wxStaticBoxSizer( border, wxVERTICAL );
	wxSizer * leftSizer = new wxBoxSizer( wxVERTICAL );
	wxSizer * rightSizer = new wxBoxSizer( wxVERTICAL );
	wxSizer * bottomSizer = new wxBoxSizer( wxHORIZONTAL );
	wxSizer * topSizer = new wxBoxSizer( wxHORIZONTAL );

	// Init controls
	CreateControl( this, &m_txtX, Move_X, "X: ", leftSizer, wxDefaultValidator );
	CreateControl( this, &m_txtY, Move_Y, "Y: ", leftSizer, wxDefaultValidator );
	CreateControl( this, &m_cboZ, Move_Z, "Z: ", leftSizer, false );
	leftSizer->Add( 1, 1, 1 );
	CreateControl( this, &m_cboObject, Move_Object, "Object: ", leftSizer, false );

	// Init buttons
	wxSizer * sizerRow = new wxBoxSizer( wxHORIZONTAL );
	m_btnOk = new wxButton( this, wxID_OK, "&Ok" );
	m_btnOk->SetBackgroundColour( app->guiColourDk );
	sizerRow->Add( m_btnOk );
	rightSizer->Add( sizerRow, 1, wxGROW );
	rightSizer->Add( 1, 1, 1 );
	sizerRow = new wxBoxSizer( wxHORIZONTAL );
	wxButton * but = new wxButton( this, wxID_CANCEL, "&Cancel" );
	but->SetBackgroundColour( app->guiColourDk );
	sizerRow->Add( but );
	rightSizer->Add( sizerRow, 1, wxGROW );

	// Init region label
	sizerRow = new wxBoxSizer( wxHORIZONTAL );
	m_lblRegionName = new wxStaticText( this, Move_RegionName, "" );
	m_lblRegionName->SetBackgroundColour( app->guiColourLt );
	bottomSizer->Add( 1, 1, 1 );
	bottomSizer->Add( m_lblRegionName, 2 );
	bottomSizer->Add( 1, 1, 1 );

	// Populate Z combo
	for( int i = 0; i < app->m_game->regions.numLevels; i++ ) {
		if( app->m_game->regions.pRegionArrays[i]->strName ) {
			m_cboZ->Append( app->m_game->regions.pRegionArrays[i]->strName->Str(), new int(i) );
		} else {
			m_cboZ->Append( "Surface", new int(i) );
		}
	}

	// Set fields
	m_txtX->SetLabel( AString(initObject->region->xloc).Str() );
	m_txtY->SetLabel( AString(initObject->region->yloc).Str() );
	m_cboZ->SetSelection( initObject->region->zloc );
	PopulateObjectComboBox();
	if( m_chooseObject ) {
		for( int i = 0; i < m_cboObject->GetCount(); i++ ) {
			if( m_cboObject->GetClientData( i ) == initObject ) {
				m_cboObject->SetSelection( i );
				break;
			}
		}
	}
	m_lblRegionName->SetLabel( initObject->region->ShortPrint( &app->m_game->regions ).Str() );

	// Set sizers
	topSizer->Add( leftSizer, 1 );
	topSizer->Add( 20, 1, 0 );
	topSizer->Add( rightSizer, 2 );
	m_sizerMove->Add( topSizer, 1 );
	m_sizerMove->Add( bottomSizer );
	int w,h;
	GetClientSize( &w, &h );
	m_sizerMove->SetDimension( 1, 1, w-2, h-2 );
	m_cboObject->Enable( m_chooseObject );
	m_btnOk->Enable( true );
}


/**
 * Default destructor
 */
MoveDialog::~MoveDialog()
{
	// Clear level combo box
	for( int i = 0; i < m_cboZ->GetCount(); i++ )
		if( m_cboZ->GetClientData( i ) )
			delete ( int * ) m_cboZ->GetClientData( i );
}
	

/**
 * Populate object combo box with all objects in the current region
 */
void MoveDialog::PopulateObjectComboBox()
{
	// Clear combo box
	for( int i = m_cboObject->GetCount() - 1; i >= 0; i-- )
		m_cboObject->Delete( i );

	// Check if we have a valid region
	ARegion * r = 0;
	long x, y;
	m_txtX->GetValue().ToLong( &x );
	m_txtY->GetValue().ToLong( &y );
	int * z = ( int * ) m_cboZ->GetClientData( m_cboZ->GetSelection() );
	if( z )
		r = app->m_game->regions.GetRegion( (int) x, (int) y, *z );

	if( r ) {
		if( m_chooseObject ) {
			forlist( &r->objects ) {
				Object * o = ( Object * ) elem;
				m_cboObject->Append( o->name->Str(), o );
			}
			m_cboObject->Enable( true );
		} else {
			m_cboObject->Append( r->GetDummy()->name->Str(), r->GetDummy() );
		}
		m_cboObject->SetSelection( 0 );
		m_lblRegionName->SetLabel( r->ShortPrint( &app->m_game->regions ).Str() );
		m_btnOk->Enable( true );
	} else {
		m_lblRegionName->SetLabel( "" );
		m_cboObject->Enable( false );
		m_btnOk->Enable( false );
	}
}

void MoveDialog::OnChangeRegion( wxCommandEvent & event ) 
{
	PopulateObjectComboBox();
}

void MoveDialog::OnOK( wxCommandEvent & event ) 
{
	int i = m_cboObject->GetSelection();
	*m_moveToObject = ( Object * ) m_cboObject->GetClientData( i );
	EndModal( wxID_OK );
}

void MoveDialog::OnCancel( wxCommandEvent & event ) 
{
	*m_moveToObject = NULL;
	EndModal( wxID_CANCEL );
}
