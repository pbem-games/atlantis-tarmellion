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

#include "wx/wx.h"
#include "../object.h"
#include "gui.h"

// Control ids
enum
{
	Move_X,
	Move_Y,
	Move_Z,
	Move_Object,
	Move_RegionName,
};

class MoveDialog : public wxDialog
{
	public:
		MoveDialog( wxWindow *parent, Object * initObject, Object ** moveToObject,
			        bool chooseObject = true );
		~MoveDialog();

	private:
		void OnChangeRegion( wxCommandEvent & event );
		void OnCancel( wxCommandEvent & event );
		void OnOK( wxCommandEvent & event );

		void PopulateObjectComboBox();

		wxTextCtrl * m_txtX;
		wxTextCtrl * m_txtY;
		wxComboBox * m_cboZ;
		wxComboBox * m_cboObject;
		wxStaticText * m_lblRegionName;
		wxButton * m_btnOk;

		wxSizer * m_sizerMove;
		Object ** m_moveToObject;
		bool m_chooseObject;

		DECLARE_EVENT_TABLE()
		
};
