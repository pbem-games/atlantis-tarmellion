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

#include "config.h"
#include "glist.h"
#include "gui.h"
#include "../aregion.h"
#include "../market.h"
#include "../object.h"
#include "../unit.h"
#include "../production.h"
#include "../faction.h"
#include <string.h>
#include "wx/imaglist.h"

#include "bitmaps/list.xpm"

#include "bitmaps/arrowup.xpm"
#include "bitmaps/arrowdown.xpm"
#include "bitmaps/zoom_in.xpm"
#include "bitmaps/zoom_out.xpm"

BEGIN_EVENT_TABLE( ListPage, wxNotebookPage )
	EVT_GRID_CELL_LEFT_CLICK( ListPage::OnLClick )
	EVT_SIZE( ListPage::OnSize )
	EVT_GRID_LABEL_LEFT_CLICK( ListPage::OnSort )
	EVT_GRID_EDITOR_SHOWN( ListPage::OnEdit )
	EVT_KEY_DOWN( ListPage::OnKeyDown )
//	EVT_GRID_RANGE_SELECT( ListPage::SkipEvent )
//	EVT_GRID_SELECT_CELL( ListPage::SkipEvent )
END_EVENT_TABLE()

// ---------------------------------------------------------------------------
// ListCanvas
// ---------------------------------------------------------------------------

ListCanvas::ListCanvas( wxWindow *parent )
		   :wxNotebook( parent, -1, wxDefaultPosition, wxDefaultSize)
{
	wxColour listColour( 220, 230, 230 );
	listWait = false;
	unitPage = 0;
	regionPage = 0;
	factionPage = 0;
	marketPage = 0;
	objectPage = 0;
	productionPage = 0;
	selectedElems = new AElemArray();
	curSelection = -1;
	SetBackgroundColour( app->guiColourLt );
	
//	wxImageList * ilist = new wxImageList( 15, 16 );

//	ilist->Add( wxBitmap( arrowup_xpm ) );
//	ilist->Add( wxBitmap( arrowdown_xpm ) );
//	ilist->Add( wxBitmap( zoom_in_xpm ) );
//	ilist->Add( wxBitmap( zoom_out_xpm ) );
//	ilist->Add( wxBitmap( "test.bmp", wxBITMAP_TYPE_BMP ) );

//	AssignImageList( ilist );
}

ListCanvas::~ListCanvas()
{
	delete selectedElems;
}

void ListCanvas::Init()
{
	DeleteAllPages();
	unitPage = 0;
	factionPage = 0;
	regionPage = 0;
	marketPage = 0;
	objectPage = 0;
	productionPage = 0;

//	if( unitPage ) delete unitPage;
	unitPage = new ListUnitPage( this );
	AddPage( unitPage, "Units" );

//	if( factionPage ) delete factionPage;
	factionPage = new ListFactionPage( this );
	AddPage( factionPage, "Factions" );

//	if( regionPage ) delete regionPage;
	regionPage = new ListRegionPage( this );
	AddPage( regionPage, "Regions" );

//	if( objectPage ) delete objectPage;
	objectPage = new ListObjectPage( this );
	AddPage( objectPage, "Objects" );

//	SetPageImage( 0, 0 );
//	SetPageImage( 1, 1 );
//	SetPageImage( 2, 2 );
//	SetPageImage( 3, 3 );

//	if( marketPage ) delete marketPage;
	if( GuiConfig.listMarketsProducts ) {
		marketPage = new ListMarketPage( this );
		AddPage( marketPage, "Markets" );
	}

//	if( productionPage ) delete productionPage;
	if( GuiConfig.listMarketsProducts ) {
		productionPage = new ListProductionPage( this );
		AddPage( productionPage, "Products" );
	}
}

void ListCanvas::UpdateItem( ARegion * item ) 
{
	if( regionPage )
		regionPage->UpdateItem( item );
}

void ListCanvas::UpdateItem( Object * item ) 
{
	if( objectPage )
		objectPage->UpdateItem( item );
}

void ListCanvas::UpdateItem( Market * item ) 
{
	if( marketPage )
		marketPage->UpdateItem( item );
}

void ListCanvas::UpdateItem( Production * item ) 
{
	if( productionPage )
		productionPage->UpdateItem( item );
}

void ListCanvas::UpdateItem( Faction * item ) 
{
	if( factionPage )
		factionPage->UpdateItem( item );
}

void ListCanvas::UpdateItem( Unit * item )
{
	if( unitPage )
		unitPage->UpdateItem( item );
}

void ListCanvas::SelectItem( AListElem * element, int type )
{
	ListPage * page = GetPageByType( type );
	if( page == NULL ) return;
	SelectPage( page );
	page->SelectItem( element );
}

void ListCanvas::DeselectAll( )
{
//	unitPage->DeselectAll();
//	factionPage->DeselectAll();
//	regionPage->DeselectAll();
//	if( marketPage )
//		marketPage->DeselectAll();
//	if( productionPage )
//		productionPage->DeselectAll();
//	if( objectPage )
//		objectPage->DeselectAll();
	unitPage->UnhighlightAllRows();
	factionPage->UnhighlightAllRows();
	regionPage->UnhighlightAllRows();
	if( marketPage )
		marketPage->UnhighlightAllRows();
	if( productionPage )
		productionPage->UnhighlightAllRows();
	if( objectPage )
		objectPage->UnhighlightAllRows();
}

void ListCanvas::SelectPage( ListPage * page )
{
	while( page != GetPage( GetSelection() ) ) {
		AdvanceSelection();
	}
}

// ---------------------------------------------------------------------------
// ListPage
// ---------------------------------------------------------------------------

ListPage::ListPage( wxWindow *parent ) 
		 :wxNotebookPage( parent, -1 )
{
	editCol = -1;
	sortCol = 0;
	waitSelect = false;
	notebook = (ListCanvas *) parent;
	lastRow = -1;
}

ListPage::~ListPage()
{
	delete grid;
	delete gridSizer;
}

void ListPage::OnSize( wxSizeEvent & event )
{
	int w,h;
	GetClientSize( &w, &h );
	if( w>0 && h>0 ) {
		gridSizer->SetDimension( 1, 1, w-1, h-1 );
		grid->GetClientSize( &w, &h );
		int cols = grid->GetNumberCols();
		grid->SetColSize( 0, w/cols -1);
		for( int i = 1; i < cols; i++ )
			grid->SetColSize( i, w/cols );
	}
}

void ListPage::OnSort( wxGridEvent & event )
{
	wxBeginBusyCursor();

	waitSelect = true;

	int col = event.GetCol();
	int rows = grid->GetNumberRows();

	int i;
	for( i = 0; i < rows; i++ ) {
		AListElem * elem = array[i];
		wxString temp = grid->GetCellValue( i, col );
		elem->SetSortString( temp.GetData() );
	}

	if( col == sortCol ) {
		array.Sort( CompareElemRev );
		sortCol = -1;
	} else {
		array.Sort( CompareElem );
		sortCol = col;
	}
	grid->ClearGrid();
	WriteGrid();	
	grid->ClearSelection();

	for( i = 0; i < (int) notebook->selectedElems->GetCount(); i++ ) {
		AListElem * e = notebook->selectedElems->Item( i );
		int index = array.Index( e );
		if( index != wxNOT_FOUND ) {
			grid->SelectRow( index, true );
		}
	}

	waitSelect = false;
	wxEndBusyCursor();

}

void ListPage::OnEdit( wxGridEvent & event)
{
	if( event.GetCol() != editCol )
		event.Veto();
}

void ListPage::WriteGrid()
{
}

void ListPage::OnSelectLines( wxGridRangeSelectEvent & event )
{
}

void ListPage::SelectItem( AListElem * item )
{
	// todo: speed this up - index it
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		if( array[i] == item ) {
			HighlightRow( i );
			break;
		}
	}
	if( i >= (int) array.GetCount() ) i = 0;

	if( lastRow == -1 ) lastRow = i;
}

void ListPage::DeselectAll()
{
	grid->ClearSelection();
}

void ListPage::OnLClick( wxGridEvent & event )
{
	int row = event.GetRow();
	bool add = false;
	if( event.ControlDown() || event.ShiftDown() )
		add = true;

	SelectItem( row, add );
	lastRow = row;
	app->UpdateSelection();
}

void ListPage::SelectLines( wxGridRangeSelectEvent & event, int type )
{
	event.Veto();
	event.Skip();
	return;
	if( notebook->listWait ) return;
	if( waitSelect ) return;
	waitSelect = true;
	bool add = false;

	AElemArray * tempSelection = new AElemArray();
	for( int i = 0; i < grid->GetNumberRows(); i++ ) {
		if( grid->IsInSelection( i, 0 ) ) {
			tempSelection->Add( array[i] );
		}
	}

	app->UpdateSelection( tempSelection, type );

	delete tempSelection;
	waitSelect = false;
}

void ListPage::OnKeyDown( wxKeyEvent & event )
{
	int key = event.GetKeyCode();
	if( key == WXK_DOWN || key == WXK_UP ) {
		int shift = 0;
		if( key == WXK_DOWN ) shift = 1;
		else if( key == WXK_UP ) shift = -1;

		if( lastRow + shift > grid->GetRows() || lastRow + shift < 0 )
			return;

		bool add = event.ShiftDown();

		if( add && grid->IsInSelection( lastRow + shift, 0 ) ) {
			// Not selecting a new row. We should remove the last row from selection
			SelectItem( lastRow, add );
			lastRow += shift;
		} else {
			SelectItem( lastRow + shift, add );
			lastRow += shift;
		}
		app->UpdateSelection();
	} else if( key == WXK_LEFT || key == WXK_RIGHT || key == WXK_HOME || key == WXK_END ||
		key == WXK_PAGEUP || key == WXK_PAGEDOWN || key == WXK_NUMPAD_LEFT || 
		key == WXK_NUMPAD_RIGHT || key == WXK_NUMPAD_HOME || key == WXK_NUMPAD_END ||
		key == WXK_NUMPAD_PAGEUP || key == WXK_NUMPAD_PAGEDOWN ) {
		//don't process these
	} else {
		event.Skip();
	}
}

void ListPage::SelectItem( int row, bool add )
{
}

void ListPage::SkipEvent( wxGridRangeSelectEvent & event )
{
	event.Veto();
}

void ListPage::SkipEvent( wxGridEvent & event )
{
	event.Veto();
}

/*
 * Reset grid view: make last cell visible
 */
void ListPage::ResetView()
{
	if( !grid->IsInSelection( lastRow, 0 ) ) lastRow = -1;
	if( lastRow != -1 )
		grid->MakeCellVisible( lastRow, 0 );
}

// Highlight a row in the grid
void ListPage::HighlightRow( int row )
{
	// Do it cheap way first - just select the row
	grid->SelectRow( row, true );
}

// Un-highlight a row in the grid
void ListPage::UnhighlightRow( int row )
{
	// To come
}

// Un-highlight all rows in grid
void ListPage::UnhighlightAllRows()
{
	// Cheap way for now:
	grid->ClearSelection();
}

// ---------------------------------------------------------------------------
// ListUnitPage
// ---------------------------------------------------------------------------

ListUnitPage::ListUnitPage( wxWindow * parent )
			:ListPage( parent )
{
	//id, name, type, faction, region
	array.Clear();
	gridSizer = new wxBoxSizer( wxVERTICAL );
	grid = new wxGrid( this, -1 );
	grid->CreateGrid( 0, 5 );
	grid->SetRowLabelSize( 0 );
	grid->SetColLabelSize( 20 );
	grid->SetColLabelValue( 0, "Id" );
	grid->SetColLabelValue( 1, "Name" );
	grid->SetColLabelValue( 2, "Type" );
	grid->SetColLabelValue( 3, "Faction" );
	grid->SetColLabelValue( 4, "Region" );

	grid->SetSelectionMode( wxGrid::wxGridSelectRows );
	grid->DisableDragRowSize();
	grid->SetCellHighlightPenWidth( 0 );

	forlist( &app->m_game->regions ) {
		ARegion * r = ( ARegion * ) elem;
		forlist( &r->objects ) {
			Object * o = ( Object * ) elem;
			forlist( &o->units ) {
				Unit * u = ( Unit * ) elem;
				AString temp = AString( u->num );
				u->SetSortString( temp.Str() );
				array.Add( u );
			}
		}
	}
	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount() );
	WriteGrid();
	gridSizer->Add( grid, 1, wxEXPAND );
	ResetView();
}

void ListUnitPage::WriteGrid()
{
	int count = array.GetCount();
	for( int i = 0; i < count; i++ ) {
		Unit * u = ( Unit * ) array[i];
		ARegion * r = u->object->region;
		AString temp = AString( "(" ) + r->xloc + ", " +
							   r->yloc + ", " +
							   r->zloc + ") ";
		if( r->name ) temp += *r->name;

		grid->SetCellValue( i, 0, AString(u->num).Str() );
		grid->SetCellValue( i, 1, u->name->Str() );
		AString temp2;
		switch( u->type ) {
			case U_NORMAL:
				temp2 = "Normal";
				break;
			case U_MAGE:
				temp2 = "Mage";
				break;
			case U_GUARD:
				temp2 = "Guard";
				break;
			case U_WMON:
				temp2 = "Monster";
				break;
			case U_GUARDMAGE:
				temp2 = "Guard mage";
				break;
			case U_APPRENTICE:
				temp2 = "Apprentice";
				break;
			default:
				temp2 = "Unknown";
				break;
		}
		grid->SetCellValue( i, 2, temp2.Str() );
		grid->SetCellValue( i, 3, u->faction->name->Str() );
		grid->SetCellValue( i, 4, temp.Str() );
	}
}

void ListUnitPage::OnSelectLines( wxGridRangeSelectEvent & event )
{
	SelectLines( event, SELECT_UNIT ); 
}

void ListUnitPage::SelectItem( int row, bool add )
{
	Unit * elementClicked = ( Unit * ) array[row];
	app->Select( elementClicked, add );
}

void ListUnitPage::UpdateItem( Unit * item )
{
	for( unsigned int i = 0; i < array.GetCount(); i++ ) {
		if( array[i] == item ) {
			Unit * u = ( Unit * ) array[i];
			ARegion * r = u->object->region;
			AString temp = AString( "(" ) + r->xloc + ", " +
								   r->yloc + ", " +
								   r->zloc + ") ";
			if( r->name ) temp += *r->name;

			grid->SetCellValue( i, 0, AString(u->num).Str() );
			grid->SetCellValue( i, 1, u->name->Str() );
			AString temp2;
			switch( u->type ) {
				case U_NORMAL:
					temp2 = "Normal";
					break;
				case U_MAGE:
					temp2 = "Mage";
					break;
				case U_GUARD:
					temp2 = "Guard";
					break;
				case U_WMON:
					temp2 = "Monster";
					break;
				case U_GUARDMAGE:
					temp2 = "Guard mage";
					break;
				case U_APPRENTICE:
					temp2 = "Apprentice";
					break;
				default:
					temp2 = "Unknown";
					break;
			}
			grid->SetCellValue( i, 2, temp2.Str() );
			grid->SetCellValue( i, 3, u->faction->name->Str() );
			grid->SetCellValue( i, 4, temp.Str() );
		}
	}
}

// ---------------------------------------------------------------------------
// ListFactionPage
// ---------------------------------------------------------------------------

ListFactionPage::ListFactionPage( wxWindow * parent )
			:ListPage( parent )
{
	//id, name, type, faction, region
	array.Clear();
	gridSizer = new wxBoxSizer( wxVERTICAL );
	grid = new wxGrid( this, -1 );
	grid->CreateGrid( 0, 5 );
	grid->SetRowLabelSize( 0 );
	grid->SetColLabelSize( 20 );
	grid->SetColLabelValue( 0, "Id" );
	grid->SetColLabelValue( 1, "Name" );
	grid->SetColLabelValue( 2, "War" );
	grid->SetColLabelValue( 3, "Trade" );
	grid->SetColLabelValue( 4, "Magic" );

	grid->SetSelectionMode( wxGrid::wxGridSelectRows );
	grid->DisableDragRowSize();
	grid->SetCellHighlightPenWidth( 0 );

	forlist( &app->m_game->factions ) {
		Faction * f = ( Faction * ) elem;
		AString temp = AString( f->num );
		f->SetSortString( temp.Str() );
		array.Add( f );
	}
	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount() );
	WriteGrid();
	gridSizer->Add( grid, 1, wxEXPAND );
	ResetView();
}

void ListFactionPage::WriteGrid()
{
	int count = array.GetCount();
	for( int i = 0; i < count; i++ ) {
		Faction * f = ( Faction * ) array[i];

		grid->SetCellValue( i, 0, AString(f->num).Str() );
		grid->SetCellValue( i, 1, f->name->Str() );
		grid->SetCellValue( i, 2, AString(f->type[0]).Str() );
		grid->SetCellValue( i, 3, AString(f->type[1]).Str() );
		grid->SetCellValue( i, 4, AString(f->type[2]).Str() );
	}
}

void ListFactionPage::SelectItem( int row, bool add )
{
	Faction * elementClicked = ( Faction * ) array[row];
	app->Select( elementClicked, add );
}

void ListFactionPage::OnSelectLines( wxGridRangeSelectEvent & event )
{
	SelectLines( event, SELECT_FACTION ); 
}

void ListFactionPage::UpdateItem( Faction * item )
{
	for( unsigned int i = 0; i < array.GetCount(); i++ ) {
		if( array[i] == item ) {
			Faction * f = ( Faction * ) array[i];

			grid->SetCellValue( i, 0, AString(f->num).Str() );
			grid->SetCellValue( i, 1, f->name->Str() );
			grid->SetCellValue( i, 2, AString(f->type[0]).Str() );
			grid->SetCellValue( i, 3, AString(f->type[1]).Str() );
			grid->SetCellValue( i, 4, AString(f->type[2]).Str() );
		}
	}
}

// ---------------------------------------------------------------------------
// ListRegionPage
// ---------------------------------------------------------------------------

ListRegionPage::ListRegionPage( wxWindow * parent )
			:ListPage( parent )
{
	//loc, name, terrain, gate, race, population, money
	array.Clear();
	gridSizer = new wxBoxSizer( wxVERTICAL );
	grid = new wxGrid( this, -1 );
	grid->CreateGrid( 0, 7 );
	grid->SetRowLabelSize( 0 );
	grid->SetColLabelSize( 20 );
	grid->SetColLabelValue( 0, "Location" );
	grid->SetColLabelValue( 1, "Name" );
	grid->SetColLabelValue( 2, "Terrain" );
	grid->SetColLabelValue( 3, "Gate" );
	grid->SetColLabelValue( 4, "Race" );
	grid->SetColLabelValue( 5, "Pop" );
	grid->SetColLabelValue( 6, "Money" );

	grid->SetSelectionMode( wxGrid::wxGridSelectRows );
	grid->DisableDragRowSize();
	grid->SetCellHighlightPenWidth( 0 );

	forlist( &app->m_game->regions ) {
		ARegion * r = ( ARegion * ) elem;
		AString temp = AString( "(" ) + r->xloc + ", " +
							   r->yloc + ", " +
							   r->zloc + ")";
		r->SetSortString( temp.Str() );
		array.Add( r );
	}
	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount() );
	WriteGrid();
	gridSizer->Add( grid, 1, wxEXPAND );
	ResetView();
}

void ListRegionPage::WriteGrid()
{
	int count = array.GetCount();
	for( int i = 0; i < count; i++ ) {
		ARegion * r = ( ARegion * ) array[i];
		AString temp = AString( "(" ) + r->xloc + ", " +
							   r->yloc + ", " +
							   r->zloc + ")";
		grid->SetCellValue( i, 0, temp.Str() );
		grid->SetCellValue( i, 1, r->name->Str() );
		grid->SetCellValue( i, 2, TerrainDefs[r->type].name );
		grid->SetCellValue( i, 3, AString(r->gate).Str() );
		grid->SetCellValue( i, 4, ( r->race >= 0 ? ItemDefs[r->race].name : "" ) );
		grid->SetCellValue( i, 5, AString(r->population + 
									(r->town ? r->town->pop : 0) ).Str() );
		grid->SetCellValue( i, 6, AString(r->money).Str() );
	}
}

void ListRegionPage::SelectItem( int row, bool add )
{
	ARegion * elementClicked = ( ARegion * ) array[row];
	app->Select( elementClicked, add );	
}

void ListRegionPage::OnSelectLines( wxGridRangeSelectEvent & event )
{
	SelectLines( event, SELECT_REGION ); 
}

void ListRegionPage::UpdateItem( ARegion * item )
{
	for( unsigned int i = 0; i < array.GetCount(); i++ ) {
		if( array[i] == item ) {
			ARegion * r = ( ARegion * ) array[i];
			AString temp = AString( "(" ) + r->xloc + ", " +
								   r->yloc + ", " +
								   r->zloc + ")";
			grid->SetCellValue( i, 0, temp.Str() );
			grid->SetCellValue( i, 1, r->name->Str() );
			grid->SetCellValue( i, 2, TerrainDefs[r->type].name );
			grid->SetCellValue( i, 3, AString(r->gate).Str() );
			grid->SetCellValue( i, 4, ItemDefs[r->race].name );
			grid->SetCellValue( i, 5, AString(r->population + 
										(r->town ? r->town->pop : 0) ).Str() );
			grid->SetCellValue( i, 6, AString(r->money).Str() );
		}
	}
}

// ---------------------------------------------------------------------------
// ListMarketPage
// ---------------------------------------------------------------------------

ListMarketPage::ListMarketPage( wxWindow * parent )
			:ListPage( parent )
{
	//loc, type, item, num, price
	array.Clear();
	gridSizer = new wxBoxSizer( wxVERTICAL );
	grid = new wxGrid( this, -1 );
	grid->CreateGrid( 0, 5 );
	grid->SetRowLabelSize( 0 );
	grid->SetColLabelSize( 20 );
	grid->SetColLabelValue( 0, "Location" );
	grid->SetColLabelValue( 1, "Type" );
	grid->SetColLabelValue( 2, "Item" );
	grid->SetColLabelValue( 3, "Number" );
	grid->SetColLabelValue( 4, "Price" );

	grid->SetSelectionMode( wxGrid::wxGridSelectRows );
	grid->DisableDragRowSize();
	grid->SetCellHighlightPenWidth( 0 );

	forlist( &app->m_game->regions ) {
		ARegion * r = ( ARegion * ) elem;
		forlist( &r->markets ) {
			Market * m = ( Market * ) elem;
			AString temp = AString( "(" ) + r->xloc + ", " +
							   r->yloc + ", " +
							   r->zloc + ")";
			m->SetSortString( temp.Str() );
			array.Add( m );
		}
	}
	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount() );
	WriteGrid();
	gridSizer->Add( grid, 1, wxEXPAND );
	ResetView();
}

void ListMarketPage::WriteGrid()
{
	int count = array.GetCount();
	for( int i = 0; i < count; i++ ) {
		Market * m = ( Market * ) array[i];
		if( m->item == -1 ) continue;
		ARegion * r = app->m_game->regions.GetRegion( m->region );
		AString temp = AString( "(" ) + r->xloc + ", " +
							   r->yloc + ", " +
							   r->zloc + ")";
		grid->SetCellValue( i, 0, temp.Str() );
		grid->SetCellValue( i, 1, m->type == M_BUY ? "Sells" : "Buys" );
		grid->SetCellValue( i, 2, ItemDefs[m->item].name );
		grid->SetCellValue( i, 3, AString(m->amount).Str() );
		grid->SetCellValue( i, 4, AString(m->price).Str() );
	}
}

void ListMarketPage::SelectItem( int row, bool add )
{
	Market * elementClicked = ( Market * ) array[row];
	app->Select( elementClicked, add );
}

void ListMarketPage::OnSelectLines( wxGridRangeSelectEvent & event )
{
	SelectLines( event, SELECT_MARKET ); 
}

void ListMarketPage::UpdateItem( Market * item )
{
	for( unsigned int i = 0; i < array.GetCount(); i++ ) {
		if( array[i] == item ) {
			Market * m = ( Market * ) array[i];
			ARegion * r = app->m_game->regions.GetRegion( m->region );
			AString temp = AString( "(" ) + r->xloc + ", " +
								   r->yloc + ", " +
								   r->zloc + ")";
			grid->SetCellValue( i, 0, temp.Str() );
			grid->SetCellValue( i, 1, m->type == M_BUY ? "Sells" : "Buys" );
			grid->SetCellValue( i, 2, ItemDefs[m->item].name );
			grid->SetCellValue( i, 3, AString(m->amount).Str() );
			grid->SetCellValue( i, 4, AString(m->price).Str() );
		}
	}
}

// ---------------------------------------------------------------------------
// ListObjectPage
// ---------------------------------------------------------------------------

ListObjectPage::ListObjectPage( wxWindow * parent )
			:ListPage( parent )
{
	//loc, name, type, inner, incomplete
	array.Clear();
	gridSizer = new wxBoxSizer( wxVERTICAL );
	grid = new wxGrid( this, -1 );
	grid->CreateGrid( 0, 5 );
	grid->SetRowLabelSize( 0 );
	grid->SetColLabelSize( 20 );
	grid->SetColLabelValue( 0, "Location" );
	grid->SetColLabelValue( 1, "Name" );
	grid->SetColLabelValue( 2, "Type" );
	grid->SetColLabelValue( 3, "Inner" );
	grid->SetColLabelValue( 4, "Incomplete" );

	grid->SetSelectionMode( wxGrid::wxGridSelectRows );
	grid->DisableDragRowSize();
	grid->SetCellHighlightPenWidth( 0 );

	forlist( &app->m_game->regions ) {
		ARegion * r = ( ARegion * ) elem;
		forlist( &r->objects ) {
			Object * o = ( Object * ) elem;
			AString temp = AString( "(" ) + r->xloc + ", " +
							   r->yloc + ", " +
							   r->zloc + ")";
			o->SetSortString( temp.Str() );
			array.Add( o );
		}
	}
	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount() );
	WriteGrid();
	gridSizer->Add( grid, 1, wxEXPAND );
	ResetView();
}

void ListObjectPage::WriteGrid()
{
	int count = array.GetCount();
	for( int i = 0; i < count; i++ ) {
		Object * o = ( Object * ) array[i];
		ARegion * r = o->region;
		AString temp = AString( "(" ) + r->xloc + ", " +
							   r->yloc + ", " +
							   r->zloc + ")";
		grid->SetCellValue( i, 0, temp.Str() );
		grid->SetCellValue( i, 1, o->name->Str() );
		grid->SetCellValue( i, 2, ObjectDefs[o->type].name );

		r = app->m_game->regions.GetRegion( o->inner );
		temp = "";
		if( r )
			temp = AString( "(" ) + r->xloc + ", " +
								   r->yloc + ", " +
								   r->zloc + ")";
		grid->SetCellValue( i, 3, temp.Str() );
		grid->SetCellValue( i, 4, AString(o->incomplete).Str() );
	}
}

void ListObjectPage::SelectItem( int row, bool add )
{
	Object * elementClicked = ( Object * ) array[row];
	app->Select( elementClicked, add );	
}

void ListObjectPage::OnSelectLines( wxGridRangeSelectEvent & event )
{
	SelectLines( event, SELECT_OBJECT ); 
}

void ListObjectPage::UpdateItem( Object * item )
{
	for( unsigned int i = 0; i < array.GetCount(); i++ ) {
		if( array[i] == item ) {
			Object * o = ( Object * ) array[i];
			ARegion * r = o->region;
			AString temp = AString( "(" ) + r->xloc + ", " +
								   r->yloc + ", " +
								   r->zloc + ")";
			grid->SetCellValue( i, 0, temp.Str() );
			grid->SetCellValue( i, 1, o->name->Str() );
			grid->SetCellValue( i, 2, ObjectDefs[o->type].name );

			r = app->m_game->regions.GetRegion( o->inner );
			temp = "";
			if( r )
				temp = AString( "(" ) + r->xloc + ", " +
									   r->yloc + ", " +
									   r->zloc + ")";
			grid->SetCellValue( i, 3, temp.Str() );
			grid->SetCellValue( i, 4, AString(o->incomplete).Str() );
		}
	}
}

// ---------------------------------------------------------------------------
// ListProductionPage
// ---------------------------------------------------------------------------

ListProductionPage::ListProductionPage( wxWindow * parent )
			:ListPage( parent )
{
	//loc, type, item, num
	array.Clear();
	gridSizer = new wxBoxSizer( wxVERTICAL );
	grid = new wxGrid( this, -1 );
	grid->CreateGrid( 0, 3 );
	grid->SetRowLabelSize( 0 );
	grid->SetColLabelSize( 20 );
	grid->SetColLabelValue( 0, "Location" );
	grid->SetColLabelValue( 1, "Item" );
	grid->SetColLabelValue( 2, "Number" );

	grid->SetSelectionMode( wxGrid::wxGridSelectRows );
	grid->DisableDragRowSize();
	grid->SetCellHighlightPenWidth( 0 );

	forlist( &app->m_game->regions ) {
		ARegion * r = ( ARegion * ) elem;
		forlist( &r->products ) {
			Production * p = ( Production * ) elem;
			AString temp = AString( "(" ) + r->xloc + ", " +
							   r->yloc + ", " +
							   r->zloc + ")";
			p->SetSortString( temp.Str() );
			array.Add( p );
		}
	}
	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount() );
	WriteGrid();
	gridSizer->Add( grid, 1, wxEXPAND );
	ResetView();
}

void ListProductionPage::WriteGrid()
{
	int count = array.GetCount();
	for( int i = 0; i < count; i++ ) {
		Production * p = ( Production * ) array[i];
		ARegion * r = app->m_game->regions.GetRegion( p->region );
		AString temp = AString( "(" ) + r->xloc + ", " +
							   r->yloc + ", " +
							   r->zloc + ")";
		grid->SetCellValue( i, 0, temp.Str() );
		grid->SetCellValue( i, 1, ItemDefs[p->itemtype].name );
		grid->SetCellValue( i, 2, AString(p->amount).Str() );
	}
}

void ListProductionPage::SelectItem( int row, bool add )
{
	Production * elementClicked = ( Production * ) array[row];
	app->Select( elementClicked, add );	
}

void ListProductionPage::OnSelectLines( wxGridRangeSelectEvent & event )
{
	SelectLines( event, SELECT_PRODUCTION ); 
}

void ListProductionPage::UpdateItem( Production * item )
{
	for( unsigned int i = 0; i < array.GetCount(); i++ ) {
		if( array[i] == item ) {
			Production * p = ( Production * ) array[i];
			ARegion * r = app->m_game->regions.GetRegion( p->region );
			AString temp = AString( "(" ) + r->xloc + ", " +
								   r->yloc + ", " +
								   r->zloc + ")";
			grid->SetCellValue( i, 0, temp.Str() );
			grid->SetCellValue( i, 1, ItemDefs[p->itemtype].name );
			grid->SetCellValue( i, 2, AString(p->amount).Str() );
		}
	}
}

void ListCanvas::AddItem( Faction * pFaction )
{
	factionPage->AddItem( pFaction );
}

void ListCanvas::AddItem( Market * pMarket )
{
	marketPage->AddItem( pMarket );
}

void ListCanvas::AddItem( Production * pProduction )
{
	productionPage->AddItem( pProduction );
}

void ListCanvas::AddItem( Object * pObject )
{
	factionPage->AddItem( pObject );
}

void ListCanvas::AddItem( Unit * pUnit )
{
	unitPage->AddItem( pUnit );
}

void ListCanvas::RemoveItem( Faction * pFaction )
{
	factionPage->RemoveItem( pFaction );
}

void ListCanvas::RemoveItem( Market * pMarket )
{
	marketPage->RemoveItem( pMarket );
}

void ListCanvas::RemoveItem( Production * pProduction )
{
	productionPage->RemoveItem( pProduction );
}

void ListCanvas::RemoveItem( Object * pObject )
{
	objectPage->RemoveItem( pObject );
}

void ListCanvas::RemoveItem( Unit * pUnit )
{
	unitPage->RemoveItem( pUnit );
}

void ListPage::RemoveItem( AListElem * element )
{
	int row = array.Index( element );
	array.RemoveAt( row );
	grid->DeleteRows( row );
}

void ListFactionPage::AddItem( AListElem * element )
{
	Faction * f = ( Faction * ) element;

	array.Add( f );
	grid->AppendRows();
	int row = grid->GetNumberRows() - 1;
	grid->SetCellValue( row, 0, AString(f->num).Str() );
	grid->SetCellValue( row, 1, f->name->Str() );
	grid->SetCellValue( row, 2, AString(f->type[0]).Str() );
	grid->SetCellValue( row, 3, AString(f->type[1]).Str() );
	grid->SetCellValue( row, 4, AString(f->type[2]).Str() );

}

void ListProductionPage::AddItem( AListElem * element )
{
	Production * p = ( Production * ) element;

	array.Add( p );
	grid->AppendRows();
	int row = grid->GetNumberRows() - 1;
	ARegion * r = app->m_game->regions.GetRegion( p->region );
	AString temp = AString( "(" ) + r->xloc + ", " +
							r->yloc + ", " +
							r->zloc + ")";
	grid->SetCellValue( row, 0, temp.Str() );
	grid->SetCellValue( row, 1, ItemDefs[p->itemtype].name );
	grid->SetCellValue( row, 2, AString(p->amount).Str() );
}

void ListObjectPage::AddItem( AListElem * element )
{
	Object * o = ( Object * ) element;

	array.Add( o );
	grid->AppendRows();
	int row = grid->GetNumberRows() - 1;
	ARegion * r = o->region;
	AString temp = AString( "(" ) + r->xloc + ", " +
						r->yloc + ", " +
						r->zloc + ")";
	grid->SetCellValue( row, 0, temp.Str() );
	grid->SetCellValue( row, 1, o->name->Str() );
	grid->SetCellValue( row, 2, ObjectDefs[o->type].name );

	r = app->m_game->regions.GetRegion( o->inner );
	temp = "";
	if( r )
		temp = AString( "(" ) + r->xloc + ", " +
							r->yloc + ", " +
							r->zloc + ")";
	grid->SetCellValue( row, 3, temp.Str() );
	grid->SetCellValue( row, 4, AString(o->incomplete).Str() );
}

void ListMarketPage::AddItem( AListElem * element )
{
	if( !GuiConfig.listMarketsProducts ) return;

	Market * m = ( Market * ) element;

	array.Add( m );
	grid->AppendRows();
	int row = grid->GetNumberRows() - 1;
	ARegion * r = app->m_game->regions.GetRegion( m->region );
	AString temp = AString( "(" ) + r->xloc + ", " +
							r->yloc + ", " +
							r->zloc + ")";
	grid->SetCellValue( row, 0, temp.Str() );
	grid->SetCellValue( row, 1, m->type == M_BUY ? "Sells" : "Buys" );
	grid->SetCellValue( row, 2, ItemDefs[m->item].name );
	grid->SetCellValue( row, 3, AString(m->amount).Str() );
	grid->SetCellValue( row, 4, AString(m->price).Str() );

}

void ListUnitPage::AddItem( AListElem * element )
{
	Unit * u = ( Unit * ) element;

	array.Add( u );
	grid->AppendRows();
	int row = grid->GetNumberRows() - 1;
	ARegion * r = u->object->region;
	AString temp = AString( "(" ) + r->xloc + ", " +
							r->yloc + ", " +
							r->zloc + ") ";
	if( r->name ) temp += *r->name;

	grid->SetCellValue( row, 0, AString(u->num).Str() );
	grid->SetCellValue( row, 1, u->name->Str() );
	AString temp2;
	switch( u->type ) {
		case U_NORMAL:
			temp2 = "Normal";
			break;
		case U_MAGE:
			temp2 = "Mage";
			break;
		case U_GUARD:
			temp2 = "Guard";
			break;
		case U_WMON:
			temp2 = "Monster";
			break;
		case U_GUARDMAGE:
			temp2 = "Guard mage";
			break;
		case U_APPRENTICE:
			temp2 = "Apprentice";
			break;
		default:
			temp2 = "Unknown";
			break;
	}
	grid->SetCellValue( row, 2, temp2.Str() );
	grid->SetCellValue( row, 3, u->faction->name->Str() );
	grid->SetCellValue( row, 4, temp.Str() );

}

void ListCanvas::UpdateSelection()
{
	int i;

	bool listOld = listWait;
	listWait = true;

	// Change selections
	if( app->curSelection == curSelection ) {
		// New selection is the same type as the current selection
		// Deselect the items that aren't in the new selection and select those that are

		if( curSelection == SELECT_LEVEL ) {
			// Nothing to do here
		} else if( curSelection == SELECT_GAME ) {
			// Nothing to do here
		} else {
			// Grid lines can't be individually unselected.
			// Unselect everything if necessary
			for( i = 0; i < (int) selectedElems->GetCount(); i++ ) {
				AListElem * elem = selectedElems->Item( i );
				if( app->selectedElems->Index( elem ) == wxNOT_FOUND ) {
					DeselectAll();
//					UnhighlightAllRows();
					selectedElems->Clear();
					break;
				}
			}
		
			// Now Highlight any items that are not in the current selection
			for( i = 0; i < (int) app->selectedElems->GetCount(); i++ ) {
				AListElem * elem = app->selectedElems->Item( i );
				if( selectedElems->Index( elem ) == wxNOT_FOUND )
					SelectItem( elem, app->curSelection );
			}
		}
	} else {
		// New selection is a different type than the current selection
		// Deselect all items in the current selection and select all those in the new selection

		DeselectAll();

		// Now Highlight the new selection
		if( app->curSelection == SELECT_LEVEL ) {
			// nothing to do
		} else if( app->curSelection == SELECT_GAME ) {
			// nothing to do
		} else {
			for( i = 0; i < (int) app->selectedElems->GetCount(); i++ ) {
				AListElem * elem = app->selectedElems->Item( i );
				SelectItem( elem, app->curSelection );
			}
		}
	}

	// Synch selection values
	curSelection = app->curSelection;
	selectedLevel = app->selectedLevel;
	selectedElems->Clear();
	for( i = 0; i < (int) app->selectedElems->GetCount(); i++ ) {
		AListElem * elem = app->selectedElems->Item( i );
		selectedElems->Add( elem );
	}

	// And finally, display the last selection
	ResetView();

	listWait = listOld;
}

ListPage * ListCanvas::GetPageByType( int type )
{
	ListPage * page = NULL;
	switch( type ) {
		case SELECT_REGION:
			page = regionPage;
			break;
		case SELECT_OBJECT:
			page = objectPage;
			break;
		case SELECT_FACTION:
			page = factionPage;
			break;
		case SELECT_UNIT:
			page = unitPage;
			break;
		case SELECT_MARKET:
			page = marketPage;
			break;
		case SELECT_PRODUCTION:
			page = productionPage;
			break;
	}
	return page;
}

void ListCanvas::ResetView()
{
	ListPage * page = GetPageByType( curSelection );
	if( page == NULL ) return;

	page->ResetView();
}
