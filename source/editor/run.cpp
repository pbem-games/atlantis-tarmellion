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

#include "wx/wxprec.h"
#include "wx/utils.h"
#include "config.h"
#include "run.h"
#include "game.h"
#include "gui.h"
#ifdef __USE_EMAIL__
 #include "see.h"
#endif

BEGIN_EVENT_TABLE( RunDialog, wxDialog )
	EVT_BUTTON( RUN_RESET, RunDialog::OnReset )
	EVT_BUTTON( RUN_CLOSE, RunDialog::OnClose )
	EVT_BUTTON( RUN_ALL, RunDialog::OnRunAll )
	EVT_BUTTON( RUN_STEP_DOWNLOADORDERS, RunDialog::OnRunStep )
	EVT_BUTTON( RUN_STEP_CHECKORDERS, RunDialog::OnRunStep )
	EVT_BUTTON( RUN_STEP_MAILCHECKED, RunDialog::OnRunStep )
	EVT_BUTTON( RUN_STEP_SETUPTURN, RunDialog::OnRunStep )
	EVT_BUTTON( RUN_STEP_READPLAYERS, RunDialog::OnRunStep )
	EVT_BUTTON( RUN_STEP_READORDERS, RunDialog::OnRunStep )
	EVT_BUTTON( RUN_STEP_QUITINACTIVEFACTIONS, RunDialog::OnRunStep )
	EVT_BUTTON( RUN_STEP_RUNORDERS, RunDialog::OnRunStep )
	EVT_BUTTON( RUN_STEP_WRITEREPORT, RunDialog::OnRunStep )
	EVT_BUTTON( RUN_STEP_WRITEPLAYERS, RunDialog::OnRunStep )
	EVT_BUTTON( RUN_STEP_REMOVEDEADFACTIONS, RunDialog::OnRunStep )
	EVT_BUTTON( RUN_STEP_SAVEGAME, RunDialog::OnRunStep )
	EVT_BUTTON( RUN_STEP_MAILREPORTS, RunDialog::OnRunStep )

	EVT_BUTTON( 100 + RUN_STEP_CHECKORDERS, RunDialog::OnRunStepTo )
	EVT_BUTTON( 100 + RUN_STEP_MAILCHECKED, RunDialog::OnRunStepTo )
	EVT_BUTTON( 100 + RUN_STEP_SETUPTURN, RunDialog::OnRunStepTo )
	EVT_BUTTON( 100 + RUN_STEP_READPLAYERS, RunDialog::OnRunStepTo )
	EVT_BUTTON( 100 + RUN_STEP_READORDERS, RunDialog::OnRunStepTo )
	EVT_BUTTON( 100 + RUN_STEP_QUITINACTIVEFACTIONS, RunDialog::OnRunStepTo )
	EVT_BUTTON( 100 + RUN_STEP_RUNORDERS, RunDialog::OnRunStepTo )
	EVT_BUTTON( 100 + RUN_STEP_WRITEREPORT, RunDialog::OnRunStepTo )
	EVT_BUTTON( 100 + RUN_STEP_WRITEPLAYERS, RunDialog::OnRunStepTo )
	EVT_BUTTON( 100 + RUN_STEP_REMOVEDEADFACTIONS, RunDialog::OnRunStepTo )
	EVT_BUTTON( 100 + RUN_STEP_SAVEGAME, RunDialog::OnRunStepTo )
	EVT_BUTTON( 100 + RUN_STEP_MAILREPORTS, RunDialog::OnRunStepTo )

END_EVENT_TABLE()


AString GetRealAddress( AString & addy )
{
	char * str = addy.Str();
	bool ok = false;
	int len = strlen( str );

	for( int i = 0; i < len ; i++ ) {
		if( str[i] == '<' ) {
			ok = true;
			break;
		}
	}
	if( !ok ) {
		AString temp;
		temp = AString( "<" ) + addy + ">";
		return temp;
	}
	return addy;

}

RunDialog::RunDialog( wxWindow *parent, int step )
          :wxDialog( parent, -1, wxString( "Run Game" ), wxDefaultPosition, wxSize(380, 470 ) )
{
	lastStep = step;

	wxStaticBox * border = new wxStaticBox( this, -1, "" );

	sizerMain = new wxStaticBoxSizer( border, wxVERTICAL ); 

	wxSizer * top = new wxBoxSizer( wxHORIZONTAL );
	wxSizer * bottom = new wxBoxSizer( wxHORIZONTAL );

	wxSizer * left = new wxBoxSizer( wxVERTICAL );
	wxSizer * middle = new wxBoxSizer( wxVERTICAL );
	wxSizer * right = new wxBoxSizer( wxVERTICAL );

	// Steps
	left->Add( CreateStep( RUN_STEP_DOWNLOADORDERS, "Download orders" ),1,wxEXPAND);
	left->Add( CreateStep( RUN_STEP_CHECKORDERS, "Syntax check orders" ),1,wxEXPAND);
	left->Add( CreateStep( RUN_STEP_MAILCHECKED, "Mail checked orders" ),1,wxEXPAND);
	left->Add( CreateStep( RUN_STEP_SETUPTURN, "Setup Turn" ),1,wxEXPAND);
	left->Add( CreateStep( RUN_STEP_READPLAYERS, "Read Gamemaster File" ),1, wxEXPAND);
	left->Add( CreateStep( RUN_STEP_READORDERS, "Read Orders Files" ),1, wxEXPAND);
	left->Add( CreateStep( RUN_STEP_QUITINACTIVEFACTIONS, "Quit Inactive Factions" ),1, wxEXPAND);
	left->Add( CreateStep( RUN_STEP_RUNORDERS, "Run Orders" ),1, wxEXPAND);
	left->Add( CreateStep( RUN_STEP_WRITEREPORT, "Write Report" ),1, wxEXPAND);
	left->Add( CreateStep( RUN_STEP_WRITEPLAYERS, "Write Players File" ),1, wxEXPAND);
	left->Add( CreateStep( RUN_STEP_REMOVEDEADFACTIONS, "Remove Dead Factions"),1, wxEXPAND);
	left->Add( CreateStep( RUN_STEP_SAVEGAME, "Save Game" ),1, wxEXPAND);
	left->Add( CreateStep( RUN_STEP_MAILREPORTS, "Mail turn reports"),1, wxEXPAND);

	// Buttons
	wxButton * butOK = new wxButton( this, RUN_ALL, "&Run all" );
	wxButton * butReset = new wxButton( this, RUN_RESET, "&Reset" );
	wxButton * butCancel = new wxButton( this, RUN_CLOSE, "&Close" );
	right->Add( butOK, 1, wxALIGN_RIGHT | wxALL, 2);
	right->Add( butReset, 0, wxALIGN_RIGHT | wxALL, 2);
	right->Add( butCancel, 0, wxALIGN_RIGHT | wxALL, 2);
	
	top->Add( left,3, wxALIGN_LEFT );
	top->Add( right,2, wxALIGN_RIGHT);

	output = new wxTextCtrl( this, -1, "", wxDefaultPosition, wxDefaultSize, 
		                     wxTE_MULTILINE | wxTE_READONLY );
	bottom->Add( output, 1);

	SetBackgroundColour( app->guiColourLt );
	butOK->SetBackgroundColour( app->guiColourDk );
	butReset->SetBackgroundColour( app->guiColourDk );
	butCancel->SetBackgroundColour( app->guiColourDk );

	sizerMain->Add( top, 0, wxEXPAND  );
	sizerMain->Add( bottom, 1, wxEXPAND | wxALIGN_BOTTOM );

	SetSizer( sizerMain );

	for( int i = 1; i <= lastStep; i++ ) {
		wxButton * but = ( wxButton * ) FindWindow( i );
		but->Enable( false );
		but = ( wxButton * ) FindWindow( 100 + i );
		but->Enable( false );
	}
	if( !GuiConfig.useEmail ) {
		wxButton * but = ( wxButton * ) FindWindow( RUN_STEP_DOWNLOADORDERS );
		but->Enable( false );
		but = ( wxButton * ) FindWindow( 100 + RUN_STEP_DOWNLOADORDERS );
		but->Enable( false );
		but = ( wxButton * ) FindWindow( RUN_STEP_MAILCHECKED );
		but->Enable( false );
		but = ( wxButton * ) FindWindow( 100 + RUN_STEP_MAILCHECKED );
		but->Enable( false );
		but = ( wxButton * ) FindWindow( RUN_STEP_MAILREPORTS );
		but->Enable( false );
		but = ( wxButton * ) FindWindow( 100 + RUN_STEP_MAILREPORTS );
		but->Enable( false );
	} else {
#ifdef __USE_EMAIL__
		// attach SEE
		int code = seeAttach( 1, 0 );
		if( code < 0 && code != -72 ) {
			SeeFail( code );
			GuiConfig.useEmail = false;
		}
#endif
	}

}

RunDialog::~RunDialog()
{
#ifdef __USE_EMAIL__
	if( GuiConfig.useEmail ) {
		seeRelease();
	}
#endif
	
}

wxBoxSizer * RunDialog::CreateStep( int id, const wxString & label )
{
	wxBoxSizer * sizerRow = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer * sizerCol = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer * sizerCol2 = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer * sizerCol3 = new wxBoxSizer( wxVERTICAL );

	wxStaticText * text = new wxStaticText( this, -1, label );
	wxButton * but = new wxButton( this, id, "Run", wxDefaultPosition );
	wxButton * but2 = new wxButton( this, id + 100, "to", wxDefaultPosition );

	wxSize size = but->GetDefaultSize();
	but->SetSize( size );
	size.SetWidth( size.GetWidth() / 2 );
	but2->SetSize( size );

	sizerCol->Add( text, 1, wxALIGN_LEFT );
	sizerCol2->Add( but, 0, wxALIGN_RIGHT );
	sizerCol3->Add( but2, 0, wxALIGN_LEFT );

	text->SetBackgroundColour( app->guiColourLt );
	but->SetBackgroundColour( app->guiColourDk );
	but2->SetBackgroundColour( app->guiColourDk );

	sizerRow->Add( sizerCol, 2 );
	sizerRow->Add( sizerCol2, 0 );
	sizerRow->Add( sizerCol3, 0 );
	return sizerRow;
}

void RunDialog::RunStep( int step, bool runPrev )
{
	int curStep;
	if( runPrev ) {
		curStep = lastStep + 1;
	} else { 
		curStep = step;
	}
	for( ; curStep <= step; curStep++ ) {
		wxBeginBusyCursor();
		int ok = 0;
		switch( curStep ) {
			case RUN_STEP_DOWNLOADORDERS:
				ok = RunDownloadOrders();
				break;
			case RUN_STEP_CHECKORDERS:
				ok = RunCheckOrders();
				break;
			case RUN_STEP_MAILCHECKED:
				ok = RunMailChecked();
				break;
			case RUN_STEP_SETUPTURN:
				ok = RunSetupTurn();
				break;
			case RUN_STEP_READPLAYERS:
				ok = RunReadPlayers();
				break;
			case RUN_STEP_READORDERS:
				ok = RunReadOrders();
				break;
			case RUN_STEP_QUITINACTIVEFACTIONS:
				ok = RunQuitInactive();
				break;
			case RUN_STEP_RUNORDERS:
				ok = RunOrders();
				break;
			case RUN_STEP_WRITEREPORT:
				ok = RunWriteReports();
				break;
			case RUN_STEP_WRITEPLAYERS:
				ok = RunWritePlayers();
				break;
			case RUN_STEP_REMOVEDEADFACTIONS:
				ok = RunRemoveDead();
				break;
			case RUN_STEP_SAVEGAME:
				ok = RunSave();
				break;
			case RUN_STEP_MAILREPORTS:
				ok = RunMailReports();
				break;
		}

		wxEndBusyCursor();
		if( !ok ) break;

		wxButton * but = ( wxButton * ) FindWindow( curStep );
		but->Enable( false );
		but = ( wxButton * ) FindWindow( 100 + curStep );
		but->Enable( false );
		lastStep = curStep;
	}
}

void RunDialog::SeeFail( int code )
{
#ifdef __USE_EMAIL__
	wxString text( "* Email error: " );
	if( code == -43 ) {
		text << "Missing '<' in email address";
	} else {
		text << code;
	}
	text << ".\n";
	output->AppendText( text );
	seeClose(0);
#endif
}


void RunDialog::OnRunAll( wxCommandEvent & event )
{
	RunStep( NUM_STEPS - 1, true );
}

void RunDialog::OnRunStep( wxCommandEvent & event )
{
	RunStep( event.GetId() );
}

void RunDialog::OnRunStepTo( wxCommandEvent & event )
{
	RunStep( event.GetId() - 100, true );
}

int RunDialog::RunDownloadOrders()
{
#ifdef __USE_EMAIL__
	if( !GuiConfig.useEmail ) return 0;

	if( GuiConfig.pop3Server == "" ) {
		wxMessageBox( "No POP3 server set.", "Error", wxICON_EXCLAMATION );
		return 0;
	}

	if( GuiConfig.userName == "" ) {
		wxMessageBox( "No username set.", "Error", wxICON_EXCLAMATION );
		return 0;
	}

	wxString password;

	if( GuiConfig.password == "" ) {
		password = wxGetPasswordFromUser( "POP3 Password:", "Password" );
	} else {
		password = GuiConfig.password.Str();
	}

	//static char buffer[1024];
	int numOrders = 0;

	wxString text( "Connecting to ");
	text << GuiConfig.pop3Server.Str() << "...\n";
	output->AppendText( text );

	//define diagnostics log file 
	seeStringParam( 0, SEE_LOG_FILE, (LPSTR) "gui_email.log" );
	//prefix file names with "1-", "2-", etc for all attachments 
	seeIntegerParam( 0, SEE_FILE_PREFIX, 1 );
	// decode unnamed attachments (if any) 
	seeIntegerParam( 0, SEE_DECODE_UNNAMED, 1 );

	// connect to POP3 server
	int code = seePop3Connect( 0,
		(LPSTR) GuiConfig.pop3Server.Str(),  // POP3 server
		(LPSTR) GuiConfig.userName.Str(),    // user
		(LPSTR) password.GetData() );         // Password
	
	if( code < 0 ) {
		SeeFail( code );
		output->AppendText( "- Error: Couldn't connect to POP3 server.\n" );
		return 0;
	}

	output->AppendText( "- Connected. Checking messages...\n");
	// get # messages waiting 
	int count = seeGetEmailCount(0);

	if( count < 0 ) {
		seeClose( 0 );
		output->AppendText( "-- No messages to download.\n" );
		return 0;
	}

	// read email messages
	for( int msgNbr = 1; msgNbr <= count; msgNbr++ ) {

		// read message
		long msgSize = seeGetEmailSize( 0, msgNbr );
		if( msgSize < 0 ) {
			output->AppendText( "-- Error: message size < 0!\n");
			seeClose( 0 );
			return 0;
		}

		if( wxFileExists( "temporders.txt" ) )
			wxRemoveFile( "temporders.txt" );

		// turn off AUTO CALL driver
		seeIntegerParam( 0, SEE_AUTO_CALL_DRIVER, 0 );

		code = seeGetEmailFile(
			0,        // channel
			msgNbr,   // message number       (1st message is 1)
			"temporders.txt",  // message file name
			".",      // message directory    (eg: "C:\\MYMAIL")
			".");     // attachment directory (eg: "C:\\MYMAIL\\ATTACH")

		if( code < 0 ) {
			text = "-- Error: Failed to download message. Code ";
			text << code << ".\n";
			output->AppendText( text );
		}

		// run the driver
		for( int i = 0; ; i++ ) {
			code = seeDriver(0);
			if( code < 0 ) {
				SeeFail( code );
				return 0;
			}
			if( code == 0 ) break;
		}

		// turn AUTO CALL back on
		seeIntegerParam( 0, SEE_AUTO_CALL_DRIVER, 1 );

		if( wxFileExists( "temporders.txt" ) ) {
			int fac = app->ParseOrder( "temporders.txt" );
			if( fac ) {
				text = "-- Orders found for faction ";
				text << fac << ", deleting message.\n";
				output->AppendText( text );
				numOrders++;
				code = seeDeleteEmail( 0, msgNbr );
				if( code < 0 ) {
					//problem;
					output->AppendText( "--- Error: Couldn't delete message!\n" );
				}
			}
			wxRemoveFile( "temporders.txt" );
		}
	}

	text = "- Downloaded ";
	text << numOrders << " orders successfully.\n";
	output->AppendText( text );
	seeClose( 0 );
	return 1;
#else
	return 0;
#endif
}

int RunDialog::RunCheckOrders()
{
	Game * tempGame = new Game();

	AString orders, check;
	output->AppendText( "Checking orders...\n" );
	for( int i = 1; i < 1000; i++ ) {
		orders = AString( "orders." ) + i;
		if( !wxFileExists( orders.Str() ) ) continue;

		check = AString( "checked." ) + i;
		wxString text( "- Checking ");
		text << orders.Str() << "...\n";
		output->AppendText( text );

		if( wxFileExists( check.Str() ) )
			wxRemoveFile( check.Str() );

		tempGame->DoOrdersCheck( orders, check );

		if( !wxFileExists( check.Str() ) ) {
			output->AppendText( "-- Couldn't check orders!\n" );
		} else {
			output->AppendText( "-- Check successful, saving orders to Orders folder.\n" );
			if( !wxDirExists( "Orders" ) )
				wxMkdir( "Orders" );
			wxString from( orders.Str() );
			wxString to( "Orders\\" );
			to << orders.Str();
			if( wxCopyFile( from, to ) )
				wxRemoveFile( from );
		}
				
	}
	output->AppendText( "- Finished order checks.\n" );

	delete tempGame;
	return 1;
}

int RunDialog::RunMailChecked()
{
#ifdef __USE_EMAIL__

	if( !GuiConfig.useEmail ) return 0;
	if( !app->m_game ) return 0;

	if( GuiConfig.smtpServer == "" ) {
		wxMessageBox( "No SMTP server set.", "Error", wxICON_EXCLAMATION );
		return 0;
	}

	if( GuiConfig.email == "" ) {
		wxMessageBox( "No return email address set.", "Error", wxICON_EXCLAMATION );
		return 0;
	}

	wxString text( "Connecting to ");
	text << GuiConfig.smtpServer.Str() << "...\n";
	output->AppendText( text );

	int code = seeSmtpConnect( 
		0,                                   // channel 0
		(LPSTR) GuiConfig.smtpServer.Str(), // your SMTP server
		(LPSTR) GetRealAddress( GuiConfig.email ).Str(),      // your email address
		(LPSTR) GetRealAddress( GuiConfig.email ).Str() );    // Reply-To header

	if( code < 0 ) {
		SeeFail( code );
		output->AppendText( "* Error: Couldn't connect to SMTP server.\n" );
		return 0;
	}

	output->AppendText( "- Connected. Sending syntax checks...\n" );

	AString check;
	for( int i = 1; i < 1000; i++ ) {
		check = AString( "checked." ) + i;
		if( !wxFileExists( check.Str() ) ) continue;

		AString * addy = 0;
		forlist( &app->m_game->factions ) {
			Faction * f = ( Faction * ) elem;
			if( f->num == i ) {
				addy = f->address;
			}
		}

		if( !addy ) continue;

		wxString text( "-- Sending syntax check for faction ");
		text << i << ".\n";

		AString subject = AString( "Checked orders for Faction " ) + i;

		code = seeSendEmail(
			0,                     // channel 0
			(LPSTR) GetRealAddress( *addy ).Str(),   // To list
			(LPSTR) NULL,          // CC list
			(LPSTR) NULL,          // BCC list
			(LPSTR) subject.Str(), // Subject
			(LPSTR) "",            // Message text
			(LPSTR) check.Str());  // MIME attachment

		if( code < 0 ) {
			text = "-- Error: couldn't send email. Code ";
			text << code << ".\n";
			output->AppendText( text );
			continue;
		}
		text <<  "-- Syntax check sent. Deleting checked file.\n";
		output->AppendText( text );
		wxRemoveFile( check.Str() );
	}
	seeClose( 0 );
	return 1;
#else
	return 0;
#endif
}

int RunDialog::RunMailReports()
{

#ifdef __USE_EMAIL__
	if( !GuiConfig.useEmail ) return 0;

	if( GuiConfig.smtpServer == "" ) {
		wxMessageBox( "No SMTP server set.", "Error", wxICON_EXCLAMATION );
		return 0;
	}

	if( GuiConfig.email == "" ) {
		wxMessageBox( "No return email address set.", "Error", wxICON_EXCLAMATION );
		return 0;
	}

	wxString text( "Connecting to " );
	text << GuiConfig.smtpServer.Str() << "...\n";
	output->AppendText( text );

	int code = seeSmtpConnect(
		0,                                   // channel 0
		(LPSTR) GuiConfig.smtpServer.Str(), // SMTP server 
		(LPSTR) GetRealAddress( GuiConfig.email ).Str() ,      // email address
		(LPSTR) GetRealAddress( GuiConfig.email ).Str() );    // Reply-To header

	if( code < 0 ) {
		SeeFail( code );
		output->AppendText( "* Error: Couldn't connect to SMTP server.\n" );
		return 0;
	}

	output->AppendText( "- Connected. Sending reports...\n" );

	if( !wxDirExists( "Reports" ) )
		wxMkdir( "Reports" );

	wxString from;
	wxString to;

	AString report;
	for( int i = 1; i < 1000; i++ ) {
		report = AString( "report." ) + i;
		if( !wxFileExists( report.Str() ) ) continue;

		AString * addy = 0;
		forlist( &app->m_game->factions ) {
			Faction * f = ( Faction * ) elem;
			if( f->num == i ) {
				addy = f->address;
			}
		}

		if( !addy ) continue;

		wxString text( "-- Sending report for faction ");
		text << i << ".\n";

		AString subject = AString( "Report for Faction " ) + i;

		code = seeSendEmail(
			0,                       // channel 0
			(LPSTR) GetRealAddress( *addy ).Str(),     // To list
			(LPSTR) NULL,            // CC list
			(LPSTR) NULL,            // BCC list
			(LPSTR) subject.Str(),   // Subject
			(LPSTR) "",		         // Message text 
			(LPSTR) report.Str() );  // MIME attachment

		if( code < 0 ) {
			text = "-- Error: couldn't send email. Code ";
			text << code << ".\n";
			output->AppendText( text );
			continue;
		}
		text << "--- Report " << i << " sent. Backing up file.\n";
		output->AppendText( text );

		from = report.Str();
		to = "Reports\\";
		to << report.Str() << "." << app->m_game->TurnNumber();
		if( wxCopyFile( from, to ) )
			wxRemoveFile( from );

	}
    // turn AUTO CALL back on for seeClose
    seeIntegerParam( 0, SEE_AUTO_CALL_DRIVER, 1 );

	seeClose( 0 );
	return 1;
#else
	return 0;
#endif
}

int RunDialog::RunSetupTurn()
{
	output->AppendText( "Setting up turn...\n" );
	app->m_game->PreProcessTurn();
	output->AppendText( "- Turn setup complete.\n" );
	return 1;
}

int RunDialog::RunReadPlayers()
{
	output->AppendText( "Reading in players file...\n" );
	if( !wxFileExists( "players.in" ) ) {
		output->AppendText( "- players.in not found!\n" );
		return 0;
	}
	if( !app->m_game->ReadPlayers() ) {
		output->AppendText( "- Can't read players file\n" );
		return 0;
	}
	if (app->m_game->gameStatus == Game::GAME_STATUS_FINISHED) {
		output->AppendText( "- This game is finished!\n" );
		return 0;
	}
	app->m_game->gameStatus = Game::GAME_STATUS_RUNNING;
	output->AppendText( "- Players file read in.\n" );
	return 1;
}

int RunDialog::RunReadOrders()
{
	output->AppendText( "Reading orders...\n" );
	app->m_game->ReadOrders();
	output->AppendText( "- Orders read in.\n" );
	return 1;
}

int RunDialog::RunQuitInactive()
{
	output->AppendText( "Quitting inactive factions...\n" );
	app->m_game->RemoveInactiveFactions();
	output->AppendText( "- Inactive factions QUITted.\n" );
	return 1;
}

int RunDialog::RunOrders()
{
	output->AppendText( "Running turn...\n" );
	app->m_game->RunOrders();
	output->AppendText( "- Turn run successfully.\n" );
	return 1;
}

int RunDialog::RunWriteReports()
{
	output->AppendText( "Writing report...\n" );
	app->m_game->WriteReport();
	app->m_game->battles.DeleteAll();
	output->AppendText( "- All reports written.\n" );
	return 1;
}

int RunDialog::RunWritePlayers()
{
	output->AppendText( "Writing players file...\n" );
	app->m_game->WritePlayers();
	if( wxFileExists( "players.in" ) )
		wxRemoveFile( "players.in" );
	if( wxFileExists( "players.out" ) ) {
		wxCopyFile( "players.out", "players.in" );
		wxRemoveFile( "players.out" );
	}

	output->AppendText( "- Players file written.\n" );
	return 1;
}

int RunDialog::RunRemoveDead()
{
	output->AppendText( "Removing dead factions...\n" );
	app->m_game->DeleteDeadFactions();
	output->AppendText( "- Dead factions removed.\n" );
	return 1;
}

int RunDialog::RunSave()
{
	AString fname = AString( "game." ) + Globals->WORLD_NAME + "." + app->m_game->TurnNumber();
	wxString text( "Saving game to " );
	text << fname.Str() << ".\n";
	
	output->AppendText( text );

	if( wxFileExists( fname.Str() ) ) {
		output->AppendText( "- File already exists!\n" );
		if( !app->SaveGame() ) {
			output->AppendText( "-- Game not saved!\n" );
			return 0;
		} else {
			output->AppendText( "-- Game saved.\n" );
			return 1;
		}
	}

	if( !app->m_game->SaveGame( fname.Str() ) ) {
		output->AppendText( "- Save game failed!\n" );
		return 0;
	}

	if( !wxDirExists( "Games" ) )
		wxMkdir( "Games" );
	wxString from( fname.Str() );
	wxString to( "Games\\" );
	to << fname.Str();
	bool ok = wxCopyFile( from, to );

	// remove last turn
	fname = AString( "game." ) + Globals->WORLD_NAME + "." + ( app->m_game->TurnNumber() -1 );
	from = fname.Str();
	if( wxFileExists( from ) ) {
		to = "Games\\";
		to << fname.Str();
		if( !wxFileExists( to ) ) {
			wxCopyFile( from, to );
		}
		wxRemoveFile( from );
	}

	if( ok ) {
		output->AppendText( "- Game saved.\n" );
		GuiConfig.lastGameFile = fname;
	}

	return 1;
}

void RunDialog::OnClose( wxCommandEvent & event )
{
	EndModal( lastStep );
}

void RunDialog::OnReset( wxCommandEvent & event )
{
	for( int i = 1; i < NUM_STEPS; i++ ) {
		wxButton * but = ( wxButton * ) FindWindow( i );
		but->Enable( true );
		but = ( wxButton * ) FindWindow( 100 + i );
		but->Enable( true );
	}
	if( !GuiConfig.useEmail ) {
		wxButton * but = ( wxButton * ) FindWindow( RUN_STEP_DOWNLOADORDERS );
		but->Enable( false );
		but = ( wxButton * ) FindWindow( 100 + RUN_STEP_DOWNLOADORDERS );
		but->Enable( false );
		but = ( wxButton * ) FindWindow( RUN_STEP_MAILCHECKED );
		but->Enable( false );
		but = ( wxButton * ) FindWindow( 100 + RUN_STEP_MAILCHECKED );
		but->Enable( false );
		but = ( wxButton * ) FindWindow( RUN_STEP_MAILREPORTS );
		but->Enable( false );
		but = ( wxButton * ) FindWindow( 100 + RUN_STEP_MAILREPORTS );
		but->Enable( false );
	}
	lastStep = 0;
}

void RunDialog::OnSize( wxSizeEvent & event )
{
}
