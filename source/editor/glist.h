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

#ifndef LIST_CLASS
#define LIST_CLASS

class ListPage;
class ListCanvas;

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"
#include "aextend.h"

#include "../aregion.h"
#include "../faction.h"
#include "../object.h"
#include "../unit.h"
#include "../production.h"
#include "../market.h"

class ListPage : public wxNotebookPage
{
	public:
		ListPage( wxWindow *parent );
		~ListPage();

		void SelectItem( AListElem * );
		void DeselectAll();
		void RemoveItem( AListElem * );

		void HighlightRow( int row );
		void UnhighlightRow( int row );
		void UnhighlightAllRows();

		void SkipEvent( wxGridRangeSelectEvent & );
		void SkipEvent( wxGridEvent & );

		void ResetView();

	protected:
		virtual void OnSelectLines( wxGridRangeSelectEvent & );
		void OnLClick( wxGridEvent & );
		virtual void WriteGrid();
		virtual void SelectItem( int row, bool add );

		void OnKeyDown( wxKeyEvent & );

		void OnSize( wxSizeEvent & );
		void OnSort( wxGridEvent & );
		void OnEdit( wxGridEvent & );

		void SelectLines( wxGridRangeSelectEvent &, int type );

		ListCanvas * notebook;
		wxSizer * gridSizer;
		wxGrid * grid;
		AElemArray array;

		int lastCol;
		int editCol;
		int sortCol;
		int selectedColumn;
		bool waitSelect;

		int lastRow;

		DECLARE_EVENT_TABLE()
};

class ListUnitPage : public ListPage
{
	public:
		ListUnitPage( wxWindow *parent );
		void UpdateItem( Unit * );
		void AddItem( AListElem * );

	private:
		void SelectItem( int row, bool add );
		void OnSelectLines( wxGridRangeSelectEvent & );
		void OnLClick( wxGridEvent & );
		void WriteGrid();
};

class ListFactionPage : public ListPage
{
	public:
		ListFactionPage( wxWindow *parent );
		void UpdateItem( Faction * );
		void AddItem( AListElem * );

	private:
		void SelectItem( int row, bool add );
		void OnSelectLines( wxGridRangeSelectEvent & );
		void OnLClick( wxGridEvent & );
		void WriteGrid();
};

class ListRegionPage : public ListPage
{
	public:
		ListRegionPage( wxWindow *parent );
		void UpdateItem( ARegion * );

	private:
		void SelectItem( int row, bool add );
		void OnSelectLines( wxGridRangeSelectEvent & );
		void OnLClick( wxGridEvent & );
		void WriteGrid();
};

class ListMarketPage : public ListPage
{
	public:
		ListMarketPage( wxWindow *parent );
		void UpdateItem( Market * );
		void AddItem( AListElem * );

	private:
		void SelectItem( int row, bool add );
		void OnSelectLines( wxGridRangeSelectEvent & );
		void OnLClick( wxGridEvent & );
		void WriteGrid();
};

class ListProductionPage : public ListPage
{
	public:
		ListProductionPage( wxWindow *parent );
		void UpdateItem( Production * );
		void AddItem( AListElem * );

	private:
		void SelectItem( int row, bool add );
		void OnSelectLines( wxGridRangeSelectEvent & );
		void OnLClick( wxGridEvent & );
		void WriteGrid();
};

class ListObjectPage : public ListPage
{
	public:
		ListObjectPage( wxWindow *parent );
		void UpdateItem( Object * );
		void AddItem( AListElem * );

	private:
		void SelectItem( int row, bool add );
		void OnSelectLines( wxGridRangeSelectEvent & );
		void OnLClick( wxGridEvent & );
		void WriteGrid();
};

class ListCanvas : public wxNotebook
{
	friend class ListPage;
	public:
		ListCanvas( wxWindow *parent );
		~ListCanvas();

		void Init();

		void SelectItem( AListElem *, int type );

		void AddItem( Faction * );
//		void AddItem( ARegion *, bool populate = true );
		void AddItem( Market * );
		void AddItem( Production * );
		void AddItem( Object * );
		void AddItem( Unit * );

		void RemoveItem( Faction * );
//		void RemoveItem( ARegion * );
		void RemoveItem( Market * );
		void RemoveItem( Production * );
		void RemoveItem( Object * );
		void RemoveItem( Unit * );

		void UpdateItem( Faction * );
		void UpdateItem( ARegion * );
		void UpdateItem( Market * );
		void UpdateItem( Production * );
		void UpdateItem( Object * );
		void UpdateItem( Unit * );
		void UpdateItem( ARegionArray * );

		void UpdateSelection();

	private:
		void SelectPage( ListPage * page );
		void DeselectAll();
		void ResetView();

		ListUnitPage * unitPage;
		ListFactionPage * factionPage;
		ListRegionPage * regionPage;
		ListMarketPage * marketPage;
		ListProductionPage * productionPage;
		ListObjectPage * objectPage;

		ListPage * GetPageByType( int type );

		bool listWait;

		ARegionArray * selectedLevel;
		AElemArray * selectedElems;
		int curSelection;
};


#endif
