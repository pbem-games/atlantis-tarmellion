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

#include "options.h"
#include "gui.h"
#include "config.h"
#include "wx/grid.h"
#include "gamedata.h"
#include "wx/colordlg.h"

BEGIN_EVENT_TABLE( OptionsDialog, wxDialog )
	EVT_CHECKBOX( OPT_USEEMAIL, OptionsDialog::OnUseEmail )
	EVT_CHECKBOX( OPT_RECREATE, OptionsDialog::OnRecreate )
//	EVT_CHECKBOX( OPT_SHOWHEADERS, OptionsDialog::OnShowHeaders )
	EVT_CHECKBOX( OPT_SHOWTERRAINPICS, OptionsDialog::OnShowTerrainPics )
	EVT_CHECKBOX( OPT_LISTMARKETSPRODUCTS, OptionsDialog::OnListMarketsProducts )
	EVT_TEXT( OPT_POP3SERVER, OptionsDialog::OnPop3Server )
	EVT_TEXT( OPT_SMTPSERVER, OptionsDialog::OnSmtpServer )
	EVT_TEXT( OPT_USERNAME, OptionsDialog::OnUsername )
	EVT_TEXT( OPT_PASSWORD, OptionsDialog::OnPassword )
	EVT_TEXT( OPT_EMAIL, OptionsDialog::OnEmail )
    EVT_BUTTON( OPT_EDITTERRAIN, OptionsDialog::OnEditTerrain )
//	EVT_BUTTON( OPT_CLOSE, OptionsDialog::OnClose )

END_EVENT_TABLE()

BEGIN_EVENT_TABLE( ConfigColors, wxDialog )
	EVT_GRID_EDITOR_SHOWN( ConfigColors::OnEdit )
	EVT_GRID_CELL_LEFT_DCLICK( ConfigColors::OnDClick )
END_EVENT_TABLE()

/**
 * Default constructor
 * Create notebook pages and controls
 */
OptionsDialog::OptionsDialog( wxWindow *parent )
          :wxDialog( parent, -1, wxString( "Options" ), wxDefaultPosition, wxSize( 400, 300 ) )
{


	// Notebook
	notebook = new wxNotebook( this, -1 );

	wxSizer * optionsSizer = new wxBoxSizer( wxVERTICAL );
	optionsSizer->Add( notebook, 1, wxEXPAND, 0 );
	SetSizer( optionsSizer );

	// General options
	// Create controls - recreate data, show tree headers, funky colours,
	//  edit terr colors, list markets and prods
	generalPage = new wxPanel( notebook );
	wxSizer * generalSizer = new wxBoxSizer( wxVERTICAL );
	generalSizer->Add( 0, 0, 0, wxTOP | wxBOTTOM, 5 ); // Spacer
	CreateControl( generalPage, &recreateData, OPT_RECREATE, "Recreate data", generalSizer );
	CreateControl( generalPage, &showTerrainPictures, OPT_SHOWTERRAINPICS,
		           "Show terrain pictures", generalSizer );
	CreateControl( generalPage, &listMarketsProducts, OPT_LISTMARKETSPRODUCTS,
		           "Show markets and products in list view", generalSizer );
	generalSizer->Add( 1, 10, 1 );
	CreateButton( generalPage, &editTerrainColours, OPT_EDITTERRAIN, "Edit terrain colours",
		          generalSizer, wxALIGN_LEFT  );
//	generalSizer->Add( CreateControl( generalPage, &recreateData, OPT_RECREATE, "Recreate data" ), 0, wxEXPAND | wxTOP | wxBOTTOM, 5 );
////	generalSizer->Add( CreateControl( generalPage, &showTreeHeaders, OPT_SHOWHEADERS, "Show tree headers" ), 0, wxEXPAND | wxTOP | wxBOTTOM, 5 );
//	generalSizer->Add( CreateControl( generalPage, &showTerrainPictures, OPT_SHOWTERRAINPICS, "Show terrain pictures" ), 0, wxEXPAND | wxTOP | wxBOTTOM, 5 );
//	generalSizer->Add( CreateControl( generalPage, &listMarketsProducts, OPT_LISTMARKETSPRODUCTS, "Show markets and products in list view" ), 0, wxEXPAND | wxTOP | wxBOTTOM, 5 );
//	generalSizer->Add( CreateButton( generalPage, &editTerrainColours, OPT_EDITTERRAIN, "Edit terrain colours", wxALIGN_LEFT ), 0, wxEXPAND | wxTOP | wxBOTTOM, 5 );

	// Set initial values
	recreateData->SetValue( GuiConfig.recreateData );
//	showTreeHeaders->SetValue( GuiConfig.showTreeHeaders );
	showTerrainPictures->SetValue( GuiConfig.showFunk );
	listMarketsProducts->SetValue( GuiConfig.listMarketsProducts );

	// Finalize
	generalPage->SetSizer( generalSizer );
	notebook->AddPage( generalPage, "General" );

	// Email options
	// Create controls - useEmail, pop3Server, smtpServer, username, password
	//  email
	emailPage = new wxPanel( notebook );
	wxSizer * emailSizer = new wxBoxSizer( wxVERTICAL );

	emailSizer->Add( 0, 0, 0, wxTOP | wxBOTTOM, 5 ); // Spacer
	CreateControl( emailPage, &useEmail, OPT_USEEMAIL, "Enable email extensions", emailSizer );
	emailSizer->Add( 0, 0, 0, wxTOP | wxBOTTOM, 5 ); // Spacer
	CreateControl( emailPage, &pop3Server, OPT_POP3SERVER, "POP3 server", emailSizer, wxDefaultValidator );
	CreateControl( emailPage, &smtpServer, OPT_SMTPSERVER, "SMTP server", emailSizer, wxDefaultValidator );
	CreateControl( emailPage, &username, OPT_USERNAME, "Username", emailSizer, wxDefaultValidator );
	CreateControl( emailPage, &password, OPT_PASSWORD, "Password", emailSizer, wxDefaultValidator );
	CreateControl( emailPage, &email, OPT_EMAIL, "Email address", emailSizer, wxDefaultValidator );

	// Set initial values
	useEmail->SetValue( GuiConfig.useEmail );

	pop3Server->SetValue( GuiConfig.pop3Server.Str() );
	smtpServer->SetValue( GuiConfig.smtpServer.Str() );
	username->SetValue( GuiConfig.userName.Str() );
	password->SetValue( GuiConfig.password.Str() );
	email->SetValue( GuiConfig.email.Str() );

	// Disable/enable email controls depending on useEmail
	pop3Server->Enable( GuiConfig.useEmail );
	smtpServer->Enable( GuiConfig.useEmail );
	username->Enable( GuiConfig.useEmail );
	password->Enable( GuiConfig.useEmail );
	email->Enable( GuiConfig.useEmail );

	// Finalize
	emailPage->SetSizer( emailSizer );
	notebook->AddPage( emailPage, "Email" );

	// Set colours
//	SetBackgroundColour( app->guiColourLt );
//	generalPage->SetBackgroundColour( app->guiColourLt );
//	emailPage->SetBackgroundColour( app->guiColourLt );
//	notebook->SetBackgroundColour( app->guiColourLt );

//	int w,h;
//	GetClientSize( &w, &h );
//	emailSizer->SetDimension( 1, 1, w-2, h-2 );

}

/**
 * Default destructor
 */
OptionsDialog::~OptionsDialog()
{
}

/**
 * Update useEmail option
 */
void OptionsDialog::OnUseEmail( wxCommandEvent & event )
{
	GuiConfig.useEmail = event.IsChecked();

	pop3Server->Enable( GuiConfig.useEmail );
	smtpServer->Enable( GuiConfig.useEmail );
	username->Enable( GuiConfig.useEmail );
	password->Enable( GuiConfig.useEmail );
	email->Enable( GuiConfig.useEmail );
}

/**
 * Show terrain config dialog
 */
void OptionsDialog::OnEditTerrain( wxCommandEvent & event )
{
	ConfigColors terrain( this, _T( "Config colors" ), wxPoint( -1, -1),
		                  wxSize( 300, 300 ) );
	terrain.ShowModal();
}

/**
 * Update email option
 */
void OptionsDialog::OnEmail( wxCommandEvent & event )
{
	GuiConfig.email = email->GetValue().GetData();
}

/**
 * Update pop3Server option
 */
void OptionsDialog::OnPop3Server( wxCommandEvent & event )
{
	GuiConfig.pop3Server = pop3Server->GetValue().GetData();
}

/**
 * Update smtpServer option
 */
void OptionsDialog::OnSmtpServer( wxCommandEvent & event )
{
	GuiConfig.smtpServer = smtpServer->GetValue().GetData();
}

/**
 * Update userName option
 */
void OptionsDialog::OnUsername( wxCommandEvent & event )
{
	GuiConfig.userName = username->GetValue().GetData();
}

/**
 * Update password option
 */
void OptionsDialog::OnPassword( wxCommandEvent & event )
{
	GuiConfig.password = password->GetValue().GetData();
}

/**
 * Update recreateData option
 */
void OptionsDialog::OnRecreate( wxCommandEvent & event )
{
	GuiConfig.recreateData = event.IsChecked();
}

/**
 * Update showTreeHeaders option
 */
/*
void OptionsDialog::OnShowHeaders( wxCommandEvent & event )
{
	GuiConfig.showTreeHeaders = event.IsChecked();
	frame->tree->Init();
}
*/
/**
 * Update showFunk option
 */
void OptionsDialog::OnShowTerrainPics( wxCommandEvent & event )
{
	GuiConfig.showFunk = event.IsChecked();
	frame->map->Refresh();
}

/**
 * Update listMarketsProducts option
 */
void OptionsDialog::OnListMarketsProducts( wxCommandEvent & event )
{
	GuiConfig.listMarketsProducts = event.IsChecked();
	frame->list->Init();
}





/**
 * Default constructor
 * Creates grid displaying terrains and their currently assigned
 *  colours and bitmaps
 */
ConfigColors::ConfigColors( wxWindow *parent, const wxString & title, const wxPoint& pos, const wxSize& size )
		:wxDialog( parent, -1, title, pos, size, wxDEFAULT_DIALOG_STYLE )
{
	// Init grid
	grid = new wxGrid( this, -1 );
	grid->CreateGrid( 0, 0 );
	grid->SetRowLabelSize( 0 );
	grid->SetColLabelSize( 20 );
	grid->SetSelectionMode( wxGrid::wxGridSelectRows );
	grid->AppendCols( 3 );
 	grid->SetColLabelValue( 0, "Terrain" );
	grid->SetColLabelValue( 1, "Color" );
	grid->SetColLabelValue( 2, "Tile" );

	// Add rows
	int rows = TerrainColors.Count();
	if( rows > R_NUM ) rows = R_NUM;
	grid->AppendRows( rows );

	// Set rows
	for( int i = 0; i < rows; i++ ) {
		grid->SetCellValue( i, 0, TerrainDefs[i].name );
		wxColour col( TerrainColors[i]->colorRGB[0],
			          TerrainColors[i]->colorRGB[1],
					  TerrainColors[i]->colorRGB[2] );

		grid->SetCellBackgroundColour( i, 1, col );
		grid->SetCellValue( i, 2, TerrainColors[i]->tile );
	}

	int w, h;

	// Set sizer
	sizerAux = new wxBoxSizer( wxVERTICAL );
	sizerAux->Add( grid, 1, wxEXPAND );
	GetClientSize( &w, &h );
	sizerAux->SetDimension( 0, 0, w, h );

	// Set grid columns
	grid->GetClientSize( &w, &h );
	grid->SetColSize( 0, 100 );
	grid->SetColSize( 1, 55 );
	grid->SetColSize( 2, w - 156 );

}

/** 
 * Default destructor
 */
ConfigColors::~ConfigColors()
{
	for( int i = 0; i < grid->GetNumberRows(); i++ ) {
		if( TerrainColors[i]->tile != grid->GetCellValue( i, 2 ) )
			TerrainColors[i]->InitImages( grid->GetCellValue( i, 2 ) );
	}
	if( grid ) delete grid;
	if( sizerAux ) delete sizerAux;
	frame->map->Refresh();
}

/**
 * Cancel cell editing unless user is editing bitmap file name
 */
void ConfigColors::OnEdit( wxGridEvent & event)
{
	if( event.GetCol() != 2 )
		event.Veto();
}

/**
 * Open color selection dialog
 * Assign color to current terrain
 */
void ConfigColors::OnDClick( wxGridEvent & event )
{
	int row = event.GetRow();
	if( row > (int) TerrainColors.Count() ) return;
	wxDialog colorSelect;

	TerrainConfigType * terr = TerrainColors[row];

	wxColor old( terr->colorRGB[0], terr->colorRGB[1], terr->colorRGB[2] );

	wxColor clr = wxGetColourFromUser( frame, old );

	if( clr.Ok() ) {
		terr->colorRGB[0] = clr.Red();
		terr->colorRGB[1] = clr.Green();
		terr->colorRGB[2] = clr.Blue();
		grid->SetCellBackgroundColour( row, 1, clr );
		grid->Refresh();
	}
}
