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

#ifndef EDITAUX_CLASS
#define EDITAUX_CLASS

#include "../astring.h"
#include "../unit.h"

#include "aextend.h"

#include "wx/wx.h"
#include "wx/grid.h"

class EditAux : public wxDialog
{
	public:
		EditAux( wxWindow *parent, const wxString & title, const wxPoint& pos, const wxSize& size );
		~EditAux();

		AElemArray selectedArray;

	protected:
		void OnClose( wxCommandEvent & event );
		void OnOK( wxCommandEvent & event );

		virtual void WriteGrid();

		void InitGrid( int, bool selectRows = true );
		void InitSizer();

		void PreSelectRows( AElemArray * );

		void OnClose( wxCloseEvent & );
		void OnSort( wxGridEvent & );
		void OnEdit( wxGridEvent & );
//		void OnOK( wxCommandEvent & );

		void FinalizeArray();

		wxGrid * grid;
		wxSizer * sizerAux;

		bool editWait;
		bool canDelete;
		int editCol;
		int sortCol;

		AElemArray array;

		DECLARE_EVENT_TABLE()
};

class EditRegionAux : public EditAux
{
	public:
		EditRegionAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
		~EditRegionAux();
		void Init( AElemArray * );

	private:
		void WriteGrid();
};

class EditSkillTypeAux : public EditAux
{
	public:
		EditSkillTypeAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
		~EditSkillTypeAux();
		void Init( AElemArray *, int type = 0 );

	private:
		void WriteGrid();
		void PreSelectRows( AElemArray * );
};

class EditItemTypeAux : public EditAux
{
	public:
		EditItemTypeAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
		~EditItemTypeAux();
		void Init( AElemArray *, int type = 0 );

	private:
		void WriteGrid();
		void PreSelectRows( AElemArray * );

};

class EditFactionAux : public EditAux
{
	public:
		EditFactionAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
		~EditFactionAux();
		void Init( AElemArray * );

	private:
		void WriteGrid();
};

class EditSkillAux : public EditAux
{
	public:
		EditSkillAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
		~EditSkillAux();
		void Init( AElemArray * );

	private:
		void OnOK( wxCommandEvent & event );
		void WriteGrid();
		void FinalizeArray();

		DECLARE_EVENT_TABLE()
};

class EditItemAux : public EditAux
{
	public:
		EditItemAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
		~EditItemAux();
		void Init( AElemArray * );

	private:
		void OnOK( wxCommandEvent & event );
		void WriteGrid();
		void FinalizeArray();

		DECLARE_EVENT_TABLE()
};

class EditTerrainTypeAux : public EditAux
{
	public:
		EditTerrainTypeAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
		~EditTerrainTypeAux();
		void Init( AElemArray *, int type = 0 );

	private:
		void WriteGrid();
		void PreSelectRows( AElemArray * );
};

class EditObjectAux : public EditAux
{
	public:
		EditObjectAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
		~EditObjectAux();
		void Init( ARegion *, AElemArray * );

	private:
		void WriteGrid();
};

#endif
