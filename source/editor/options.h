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

#ifndef OPTIONS_CLASS
#define OPTIONS_CLASS

#include "aextend.h"

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"

enum {
	OPT_USEEMAIL,
	OPT_POP3SERVER,
	OPT_SMTPSERVER,
	OPT_USERNAME,
	OPT_PASSWORD,
	OPT_EMAIL,
	OPT_RECREATE,
//	OPT_SHOWHEADERS,
	OPT_SHOWTERRAINPICS,
	OPT_LISTMARKETSPRODUCTS,
	OPT_EDITTERRAIN,
};

class OptionsDialog : public wxDialog
{
	public:
		OptionsDialog( wxWindow *parent );
		~OptionsDialog();


	private:
		void OnUseEmail( wxCommandEvent & event );
		void OnRecreate( wxCommandEvent & event );
//		void OnShowHeaders( wxCommandEvent & event );
		void OnShowTerrainPics( wxCommandEvent & event );
		void OnListMarketsProducts( wxCommandEvent & event );
		void OnPop3Server( wxCommandEvent & event );
		void OnSmtpServer( wxCommandEvent & event );
		void OnUsername( wxCommandEvent & event );
		void OnPassword( wxCommandEvent & event );
		void OnEmail( wxCommandEvent & event );
		void OnEditTerrain( wxCommandEvent & event );

		wxNotebook * notebook;
		wxPanel * generalPage;
		wxPanel * emailPage;

		wxCheckBox * recreateData;
//		wxCheckBox * showTreeHeaders;
		wxCheckBox * showTerrainPictures;
		wxCheckBox * listMarketsProducts;
		wxCheckBox * useEmail;
		wxButton * editTerrainColours;
		wxTextCtrl * pop3Server;
		wxTextCtrl * smtpServer;
		wxTextCtrl * username;
		wxTextCtrl * password;
		wxTextCtrl * email;

		DECLARE_EVENT_TABLE()

};

class ConfigColors : public wxDialog
{
	public:
		ConfigColors( wxWindow *parent, const wxString & title, const wxPoint& pos, const wxSize& size );
		~ConfigColors();

	private:
		void OnEdit( wxGridEvent & );
		void OnDClick( wxGridEvent & );

		wxGrid * grid;
		wxSizer * sizerAux;	

		DECLARE_EVENT_TABLE()
};

#endif