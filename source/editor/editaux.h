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

#ifndef EDITAUX_CLASS
#define EDITAUX_CLASS

#include "../astring.h"
#include "../unit.h"

#include "alistex.h"

#include "wx/wx.h"
#include "wx/grid.h"

class EditAux : public wxDialog
{
public:
	EditAux( wxWindow *parent, const wxString & title, const wxPoint& pos, const wxSize& size );
	~EditAux();

	void InitGrid( int, bool selectRows = true );
	int AddRowToGrid( AListElem *, wxGridCellAttr * rowAttr = NULL );
	void InitSizer();

	void PreSelectRows( AElemArray * );

	void OnClose( wxCloseEvent & );

	bool editWait;
	bool canDelete;

	AElemArray array;
	wxGrid * grid;
	wxSizer * sizerAux;

	DECLARE_EVENT_TABLE()
};

class EditRegionAux : public EditAux
{
public:
	EditRegionAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditRegionAux();
	void Init( AElemArray * );
};

class EditSkillTypeAux : public EditAux
{
public:
	EditSkillTypeAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditSkillTypeAux();
	void Init( AElemArray *, int type = 0 );
	void PreSelectRows( AElemArray * );
};

class EditItemTypeAux : public EditAux
{
public:
	EditItemTypeAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditItemTypeAux();
	void Init( AElemArray *, int type = 0 );
	void PreSelectRows( AElemArray * );

};

class EditFactionAux : public EditAux
{
public:
	EditFactionAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditFactionAux();
	void Init( AElemArray * );
};

class EditSkillAux : public EditAux
{
public:
	EditSkillAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditSkillAux();
	void Init( AElemArray * );
	void OnSelect( wxGridEvent & );
	void OnClose( wxCloseEvent & );

	DECLARE_EVENT_TABLE()
};

class EditItemAux : public EditAux
{
public:
	EditItemAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditItemAux();
	void Init( AElemArray * );
	void OnSelect( wxGridEvent & );
	void OnClose( wxCloseEvent & );

	DECLARE_EVENT_TABLE()
};

class EditTerrainTypeAux : public EditAux
{
public:
	EditTerrainTypeAux( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditTerrainTypeAux();
	void Init( AElemArray *, int type = 0 );
	void PreSelectRows( AElemArray * );
};

#endif