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

#ifndef RUN_CLASS
#define RUN_CLASS

#include "aextend.h"

#include "wx/wx.h"

enum {
	DUMMY,
	RUN_STEP_DOWNLOADORDERS,
	RUN_STEP_CHECKORDERS,
	RUN_STEP_MAILCHECKED,
	RUN_STEP_SETUPTURN,
	RUN_STEP_READPLAYERS,
	RUN_STEP_READORDERS,
	RUN_STEP_QUITINACTIVEFACTIONS,
	RUN_STEP_RUNORDERS,
	RUN_STEP_WRITEREPORT,
	RUN_STEP_WRITEPLAYERS,
	RUN_STEP_REMOVEDEADFACTIONS,
	RUN_STEP_SAVEGAME,
	RUN_STEP_MAILREPORTS,
	NUM_STEPS,
	RUN_ALL,
	RUN_CLOSE,
	RUN_RESET,
};

class RunDialog : public wxDialog
{
	public:
		RunDialog( wxWindow *parent, int step );
		~RunDialog();

	private:
		void OnSize( wxSizeEvent & );
		void OnRunAll( wxCommandEvent & event );
		void OnRunStep( wxCommandEvent & event );
		void OnRunStepTo( wxCommandEvent & event );
		void OnClose( wxCommandEvent & event );
		void OnReset( wxCommandEvent & event );

		wxBoxSizer * CreateStep( int id, const wxString & label );

		void RunStep( int step, bool runPrev = false );

		int RunDownloadOrders();
		int RunCheckOrders();
		int RunMailChecked();
		int RunSetupTurn();
		int RunReadPlayers();
		int RunReadOrders();
		int RunQuitInactive();
		int RunOrders();
		int RunWriteReports();
		int RunWritePlayers();
		int RunRemoveDead();
		int RunSave();
		int RunMailReports();

		void SeeFail( int code );

		wxSizer * sizerMain;

		int lastStep;
		wxTextCtrl * output;


	DECLARE_EVENT_TABLE()
};

#endif
