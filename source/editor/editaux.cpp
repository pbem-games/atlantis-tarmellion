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
#include "editaux.h"
#include "gui.h"

#include "gamedata.h"
#include "../aregion.h"
#include "../items.h"
#include "../astring.h"
#include "../object.h"
#include "../unit.h"
#include "../faction.h"
#include "../skills.h"
#include "alist.h"

BEGIN_EVENT_TABLE( EditAux, wxDialog )
	EVT_BUTTON( wxID_OK, EditAux::OnOK )
	EVT_CLOSE( EditAux::OnClose )
	EVT_GRID_LABEL_LEFT_CLICK( EditAux::OnSort )
	EVT_GRID_EDITOR_SHOWN( EditAux::OnEdit )
	EVT_GRID_CELL_LEFT_CLICK( EditAux::OnLClick )
	EVT_KEY_DOWN( EditAux::OnKeyDown )
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( EditItemAux, wxDialog )
	EVT_GRID_LABEL_LEFT_CLICK( EditAux::OnSort )
	EVT_BUTTON( wxID_OK, EditItemAux::OnOK )
	EVT_GRID_EDITOR_SHOWN( EditAux::OnEdit )
	EVT_GRID_CELL_LEFT_CLICK( EditAux::OnLClick )
	EVT_KEY_DOWN( EditAux::OnKeyDown )
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( EditSkillAux, wxDialog )
	EVT_GRID_LABEL_LEFT_CLICK( EditAux::OnSort )
	EVT_BUTTON( wxID_OK, EditSkillAux::OnOK )
	EVT_GRID_EDITOR_SHOWN( EditAux::OnEdit )
	EVT_GRID_CELL_LEFT_CLICK( EditAux::OnLClick )
	EVT_KEY_DOWN( EditAux::OnKeyDown )
END_EVENT_TABLE()

/**
 * Default constructor
 */
EditAux::EditAux( wxWindow *parent, const wxString & title, const wxPoint& pos, const wxSize& size )
		:wxDialog( parent, -1, title, pos, size, wxFRAME_TOOL_WINDOW )
{
	editCol = -1;
	sortCol = 0;
	grid = NULL;
	sizerAux = NULL;
	SetBackgroundColour( app->guiColourLt );
	frame->EnableWindows( false );
	lastRow = -1;
	allowCellEdit = false;
}

/**
 * Default destructor
 */
EditAux::~EditAux()
{
	if( grid ) delete grid;
	if( sizerAux ) delete sizerAux;
	array.Clear();
	selectedArray.Clear();
	frame->EnableWindows( true );
}

/**
 * Cancel any cell edit event
 */
void EditAux::OnEdit( wxGridEvent & event)
{
	if( !allowCellEdit )
		event.Veto();
}

/**
 * Initialise the grid
 */
void EditAux::InitGrid( int cols )
{
	grid = new wxGrid( this, -1 );
	grid->CreateGrid( 0, 0 );
	grid->SetRowLabelSize( 0 );
	grid->SetColLabelSize( 20 );
	grid->SetSelectionMode( wxGrid::wxGridSelectRows );
	grid->AppendCols( cols );
	grid->DisableDragRowSize();
	grid->SetCellHighlightPenWidth( 0 );
}

/**
 * Highlight pre-selected rows
 */
void EditAux::PreSelectRows( AElemArray * arr )
{
	UnhighlightAllRows();
	if( arr ) {
		int i;
		for( i = 0; i < (int) arr->GetCount(); i++ ) {
			AListElem * e = arr->Item( i );
			int index = array.Index( e );
			if( index != wxNOT_FOUND ) {
				HighlightRow( index );
				lastRow = index;
			}
		}
//	} else {
//		grid->SelectRow( 0, false );
	}
	ResetView();
}

/**
 * Put elements associated with any highlighted rows into selectedArray
 */
void EditAux::FinalizeArray()
{
	if( grid->IsCellEditControlShown() ) {
		SaveEditValue();
	}
	selectedArray.Clear();
	for( int i = grid->GetNumberRows()-1; i >= 0; i-- ) {
		if( grid->IsInSelection( i, 0 ) ) {
			selectedArray.Add( array[i] );
		}
	}
}

/*
 * Process a left-click event
 */
void EditAux::OnLClick( wxGridEvent & event )
{
	int row = event.GetRow();

	if( grid->IsCellEditControlShown() ) {
		SaveEditValue();
	}

	bool add = false;
	if( event.ControlDown() || event.ShiftDown() )
		add = true;

	if( !add && editCol != -1 && grid->IsInSelection( row, 0 ) ) {
		bool b = grid->IsReadOnly( row, 0 );
		bool c = grid->IsReadOnly( row, editCol );
		// change value for this selection
		allowCellEdit = true;
		grid->SetGridCursor( row, editCol );
//		grid->GetDefaultEditor()->BeginEdit( row, editCol, grid );
		lastEditValue = grid->GetCellValue( row, editCol );
		grid->EnableCellEditControl();
//		wxMessageBox( "Editing...", "Test", wxICON_EXCLAMATION );
		allowCellEdit = false;
	} else {
		if( !add )
			UnhighlightAllRows();
		HighlightRow( row );
		ResetView();
	}
	lastRow = row;
}

void EditAux::SaveEditValue()
{
	grid->HideCellEditControl();
	grid->SaveEditControlValue();
	wxString value = grid->GetCellValue( lastRow, editCol );
	if( value != lastEditValue ) {
		for( int i = grid->GetNumberRows()-1; i >= 0; i-- ) {
			if( grid->IsInSelection( i, 0 ) ) {
				grid->SetCellValue( i, editCol, value );
			}
		}
	}
}

/**
 * Finalize array and close dialog
 */
void EditAux::OnOK( wxCommandEvent & event )
{
	FinalizeArray();
	EndModal( wxID_OK );
}

/**
 * Finalize array and close dialog
 */
void EditItemAux::OnOK( wxCommandEvent & event )
{
	FinalizeArray();
	EndModal( wxID_OK );
}

/**
 * Finalize array and close dialog
 */
void EditSkillAux::OnOK( wxCommandEvent & event )
{
	FinalizeArray();
	EndModal( wxID_OK );
}

/**
 * Close dialog
 */
void EditAux::OnClose( wxCloseEvent & event )
{
	SetReturnCode( wxID_CANCEL );
	event.Skip();
}

/**
 * Sort grid (and corresponding array) alphabetically according to
 * the grid column selected
 */
void EditAux::OnSort( wxGridEvent & event )
{
	wxBeginBusyCursor();

	int col = event.GetCol();
	int rows = grid->GetNumberRows();

	AElemArray selected;

	for( int i = 0; i < rows; i++ ) {
		AListElem * elem = array[i];
		wxString temp = grid->GetCellValue( i, col );
		if( grid->IsInSelection( i, 0 ) )
			selected.Add( elem );
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
	PreSelectRows( &selected );
	selected.Clear();

	wxEndBusyCursor();

}

/**
 * Initialise the dialog's sizer and main buttons
 */
void EditAux::InitSizer()
{
	int w, h;
	
	sizerAux = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer * top = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer * bottom = new wxBoxSizer( wxHORIZONTAL );

	top->Add( grid, 1, wxEXPAND );

	wxButton * but = new wxButton( this, wxID_OK, "&Ok" );
	but->SetBackgroundColour( app->guiColourDk );
	bottom->Add( but, 2);
	bottom->Add( 1,1,1 );
	but = new wxButton( this, wxID_CANCEL, "&Cancel" );
	but->SetBackgroundColour( app->guiColourDk );
	bottom->Add( but, 2);

//	wxBitmapButton * but2 = new wxBitmapButton( this, wxID_CANCEL, wxBitmap( "test.bmp", wxBITMAP_TYPE_BMP ) );
//	bottom->Add( but2, 2);

	sizerAux->Add( top, 1, wxEXPAND | wxALL, 2 );
	sizerAux->Add( bottom, 0, wxALIGN_BOTTOM | wxALL, 2 );
	GetClientSize( &w, &h );
	sizerAux->SetDimension( 0, 0, w, h );
}

void EditAux::WriteGrid()
{
}

/*
 * Reset grid view: Make last cell visible
 */
void EditAux::ResetView()
{
	if( lastRow != -1 )
		grid->MakeCellVisible( lastRow, 0 );
}

/**
 * Change selection when a key is pressed
 */
void EditAux::OnKeyDown( wxKeyEvent & event )
{
	int key = event.GetKeyCode();
	if( key == WXK_DOWN || key == WXK_UP ) {
		int shift = 0;
		if( key == WXK_DOWN ) shift = 1;
		else if( key == WXK_UP ) shift = -1;

		if( lastRow + shift > grid->GetRows() || lastRow + shift < 0 )
			return;

//		bool add = event.ShiftDown();

//		if( add && grid->IsInSelection( lastRow + shift, 0 ) ) {
//			// Not selecting a new row. We should remove the last row from selection
//			SelectItem( lastRow, add );
//			lastRow += shift;
//		} else {
//			SelectItem( lastRow + shift, add );
			if( lastRow != -1 )
				UnhighlightAllRows();
//				UnHighlightRow( lastRow );
			lastRow += shift;
			HighlightRow( lastRow );
//		}
//		app->UpdateSelection();
	} else if( key == WXK_LEFT || key == WXK_RIGHT || key == WXK_HOME || key == WXK_END ||
		key == WXK_PAGEUP || key == WXK_PAGEDOWN || key == WXK_NUMPAD_LEFT || 
		key == WXK_NUMPAD_RIGHT || key == WXK_NUMPAD_HOME || key == WXK_NUMPAD_END ||
		key == WXK_NUMPAD_PAGEUP || key == WXK_NUMPAD_PAGEDOWN ) {
		//don't process these
	} else if( key >= 'A' && key <= 'Z' && !event.AltDown() ) {
		// Find the next row that begins with the keyed letter.
		wxString val;
		char letter;

		int firstRow = lastRow;
		if( firstRow < 0 ) firstRow = 0;

		for( int i = firstRow + 1; i < grid->GetNumberRows(); i++ ) {
			val = grid->GetCellValue( i, 0 );
			letter = toupper( val.GetChar( 0 ) );
			if( letter == key ) {
//				UnhighlightRow( lastRow );
				UnhighlightAllRows();
				lastRow = i;
				HighlightRow( lastRow );
				ResetView();
				return;
			}
		}
		for( int i = 0; i < firstRow; i++ ) {
			val = grid->GetCellValue( i, 0 );
			letter = toupper( val.GetChar( 0 ) );
			if( letter == key ) {
//				UnhighlightRow( lastRow );
				UnhighlightAllRows();
				lastRow = i;
				HighlightRow( lastRow );
				ResetView();
				return;
			}
		}
/*	} else if( grid->IsCellEditControlShown() ) {
		if( key == WXK_RETURN ) {
			grid->HideCellEditControl();
			grid->SaveEditControlValue();
			if( grid->GetCellValue( lastRow, editCol ) != lastEditValue )
				SaveEditValue( grid->GetCellValue( lastRow, editCol ) );
		} else if( key == WXK_ESCAPE ) {
			grid->HideCellEditControl();
		} else {
			event.Skip();
		}
*/	} else {
		event.Skip();
	}
}

// Highlight a row in the grid
void EditAux::HighlightRow( int row )
{
	// Do it cheap way first - just select the row
	grid->SelectRow( row, true );
}

// Un-highlight a row in the grid
void EditAux::UnhighlightRow( int row )
{
	// To come?
}

// Un-highlight all rows in grid
void EditAux::UnhighlightAllRows()
{
	// Cheap way for now:
	grid->ClearSelection();
}

// ---------------------------------------------------------------------------
// EditRegionAux
// ---------------------------------------------------------------------------

/**
 * Default constructor
 */
EditRegionAux::EditRegionAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			  :EditAux( parent, "Regions", pos, size )
{
	canDelete = false;
}

/**
 * Default destructor
 */
EditRegionAux::~EditRegionAux()
{
	//delete dummy region
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		ARegion * r = ( ARegion * ) array[i];
		if( r->num == -1 ) {
			delete ( ARegion * ) r;
			array.RemoveAt( i );
			break;
		}
	}
}

/**
 * Initialise dialog and array
 */
void EditRegionAux::Init( AElemArray * arr )
{
	//set up grid
	InitGrid( 2 );
	grid->SetColLabelValue( 0, "Location" );
	grid->SetColLabelValue( 1, "Name" );

	//load items into array
	ARegion * reg = new ARegion();

	reg->SetName( "" );
	reg->num = -1;
	reg->SetSortString( "" );
	array.Add( reg );

	forlist( &app->m_game->regions )	{
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

	//size columns
	InitSizer();
	int w, h;
	grid->GetClientSize( &w, &h );
	grid->SetColSize( 0, 55 );
	grid->SetColSize( 1, w - 56 );
	grid->GetScrollPixelsPerUnit( &w, &h );
	grid->SetScrollRate( 0, h );

	//pre-select items
	PreSelectRows( arr );
}

/**
 * Write items from array to grid
 */
void EditRegionAux::WriteGrid()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		ARegion * r = ( ARegion * ) array[i];
		AString temp("");
		if( r->num != -1 ) {
			temp = AString( "(" ) + r->xloc + ", " +
							   r->yloc + ", " +
							   r->zloc + ")";
		} else {
			temp = AString( "(none)" );
		}
		grid->SetCellValue( i, 0, temp.Str() );
		if( r->num != -1 )
			grid->SetCellValue( i, 1, r->name->Str() );
		else
			grid->SetCellValue( i, 1, "" );
	}
}

// ---------------------------------------------------------------------------
// EditSkillTypeAux
// ---------------------------------------------------------------------------


/**
 * Default constructor
 */
EditSkillTypeAux::EditSkillTypeAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Skills", pos, size )
{
	canDelete = true;
}

/**
 * Default destructor
 */
EditSkillTypeAux::~EditSkillTypeAux()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		delete ( Skill * ) array[i];
	}
}

/**
 * Initialise dialog and array
 */
void EditSkillTypeAux::Init( AElemArray * arr, int type )
{
	//set up grid
	InitGrid( 1 );
	grid->SetColLabelValue( 0, "Skill" );

	//load items into array

	Skill * s = new Skill();
	s->SetSortString( "(none)" );
	s->type = -1;
	array.Add( s );

	for( int i = 0; i<NSKILLS; i++ ) {
		if( SkillDefs[i].flags & SkillType::DISABLED ) continue;
		if( !type || SkillDefs[i].flags & type ) {
			s = new Skill();
			s->SetSortString( SkillDefs[i].name );
			s->type = i;
			array.Add( s );
		}
	}
	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount());
	WriteGrid();

	//size columns
	InitSizer();
	int w, h;
	grid->GetClientSize( &w, &h );
	grid->SetColSize( 0, w - 1 );
	grid->GetScrollPixelsPerUnit( &w, &h );
	grid->SetScrollRate( 0, h );

	//pre-select items
	PreSelectRows( arr );
}

/**
 * Write items from array to grid
 */
void EditSkillTypeAux::WriteGrid()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		Skill * s = ( Skill * )array[i];
		if( s->type == -1 )
			grid->SetCellValue( i, 0, "(none)" );
		else
			grid->SetCellValue( i, 0, SkillDefs[s->type].name );
	}
}

/**
 * Highlight pre-selected rows
 */
void EditSkillTypeAux::PreSelectRows( AElemArray * arr )
{
	UnhighlightAllRows();
	if( arr ) {
		int i, j = -1;
		for( i = 0; i < (int) arr->GetCount(); i++ ) {
			int type = ( ( Skill * ) arr->Item( i ) )->type;
			for( j = 0; j < (int) array.GetCount(); j++ ) {
				if( type == ( ( Skill * ) array[j] )->type ) {
//					grid->SelectRow( j, true );
//					grid->MakeCellVisible( j,0 );
					HighlightRow( j );
					lastRow = j;
				}
			}
		}
//	} else {
//		grid->SelectRow( 0, false );
	}
	ResetView();
}

// ---------------------------------------------------------------------------
// EditItemTypeAux
// ---------------------------------------------------------------------------

/**
 * Default constructor
 */
EditItemTypeAux::EditItemTypeAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Items", pos, size )
{
	canDelete = true;
}

/**
 * Default destructor
 */
EditItemTypeAux::~EditItemTypeAux()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		delete ( Item * ) array[i];
	}
}

/**
 * Initialise dialog and array
 */
void EditItemTypeAux::Init( AElemArray * arr, int type )
{
	//set up grid
	InitGrid( 1 );
	grid->SetColLabelValue( 0, "Item" );

	//load items into array
	Item * it = new Item();
	it->type = -1;
	it->SetSortString( "(none)" );
	array.Add( it );

	for( int i = 0; i < NITEMS; i++ ) {
		if( ItemDefs[i].flags & ItemType::DISABLED ) continue;
		if( !type || ItemDefs[i].type & type ) {
			it = new Item();
			it->type = i;
			it->SetSortString( ItemDefs[i].name );
			array.Add( it );
		}
	}
	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount() );
	WriteGrid();

	//size columns
	InitSizer();
	int w, h;
	grid->GetClientSize( &w, &h );
	grid->SetColSize( 0, w - 1 );
	grid->GetScrollPixelsPerUnit( &w, &h );
	grid->SetScrollRate( 0, h );

	//pre-select items
	PreSelectRows( arr );
}

/**
 * Write items from array to grid
 */
void EditItemTypeAux::WriteGrid()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		Item * it = ( Item * ) array[i];
		if( it->type == -1 )
			grid->SetCellValue( i, 0, "(none)" );
		else
			grid->SetCellValue( i, 0, ItemDefs[it->type].name );
	}
}

/**
 * Highlight pre-selected rows
 */
void EditItemTypeAux::PreSelectRows( AElemArray * arr )
{
	UnhighlightAllRows();
	if( arr ) {
		int i, j = -1;
		for( i = 0; i < (int) arr->GetCount(); i++ ) {
			int type = ( ( Item * ) arr->Item( i ) )->type;
			for( j = 0; j < (int) array.GetCount() ; j++ ) {
				if( type == ( ( Item * ) array[j] )->type ) {
//					grid->SelectRow( j, true );
//					grid->MakeCellVisible( j,0 );
					HighlightRow( j );
					lastRow = j;
				}
			}
		}
//	} else {
//		grid->SelectRow( 0, false );
	}
	ResetView();
}

// ---------------------------------------------------------------------------
// EditFactionAux
// ---------------------------------------------------------------------------

/**
 * Default constructor
 */
EditFactionAux::EditFactionAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Factions", pos, size )
{
	canDelete = false;
}

/**
 * Default destructor
 */
EditFactionAux::~EditFactionAux()
{
}

/**
 * Initialise dialog and array
 */
void EditFactionAux::Init( AElemArray * arr )
{
	//set up grid
	InitGrid( 2 );
	grid->SetColLabelValue( 0, "Number" );
	grid->SetColLabelValue( 1, "Name" );

	//load items into array
//	Faction * f = new Faction();
//	f->num = -1;
//	f->sortString = "( none )";
//	array.Add( it );

	forlist( &app->m_game->factions ) {
		Faction * f = ( Faction * ) elem;
		f->SetSortString( AString( f->num ).Str() );
		array.Add( f );
	}
	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount() );
	WriteGrid();

	//size columns
	InitSizer();
	int w, h;
	grid->GetClientSize( &w, &h );
	grid->SetColSize( 0, 40 );
	grid->SetColSize( 1, w - 41 );
	grid->GetScrollPixelsPerUnit( &w, &h );
	grid->SetScrollRate( 0, h );

	//pre-select items
	PreSelectRows( arr );

}

/**
 * Write items from array to grid
 */
void EditFactionAux::WriteGrid()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		Faction * f = ( Faction * ) array[i];
		grid->SetCellValue( i, 0, AString(f->num).Str() );
		grid->SetCellValue( i, 1, f->name->Str() );
	}
}

// ---------------------------------------------------------------------------
// EditSkillAux
// ---------------------------------------------------------------------------


/**
 * Default constructor
 */
EditSkillAux::EditSkillAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Skills", pos, size )
{
	canDelete = false;
}

/**
 * Default destructor
 */
EditSkillAux::~EditSkillAux()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		delete ( Skill * ) array[i];
	}
}

/**
 * Initialise dialog and array
 */
void EditSkillAux::Init( AElemArray * arr )
{
	//set up grid
	editWait = true;
	InitGrid( 2 );
	grid->SetColLabelValue( 0, "Skill" );
	grid->SetColLabelValue( 1, "Days" );

	Unit * u = ( Unit * ) arr->Item( 0 );

	//load items into array
	for( int i = 0; i < NSKILLS; i++ ) {
		if( SkillDefs[i].flags & SkillType::DISABLED ) continue;
		Skill * s = new Skill();
		int days = 0;
		//TODO: handle units w/out skills (ie monsters)
		if( u->GetMen() )
			days = u->skills.GetDays( i ) / u->GetMen();

		for( int j = 0; j < (int) arr->GetCount(); j++ ) {
			u = ( Unit * )arr->Item( j );
			int d = 0; 
			if( u->GetMen() )
				d = u->skills.GetDays( i ) / u->GetMen();
			if( days != d )	{
				days = -1;
				break;
			}
		}

		// make sure this is sorted to the top of list
		AString temp;
		if( days ) temp += "1";
		temp += SkillDefs[i].name;
		s->SetSortString( temp.Str() );

		s->type = i;
		s->days = days;
		array.Add( s );
	}
	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount() );
	editCol = 1;
//	grid->SetColFormatNumber( 1 );

	WriteGrid();

	//size columns
	InitSizer();
	int w, h;
	grid->GetClientSize( &w, &h );
	grid->SetColSize( 0, w - 51 );
	grid->SetColSize( 1, 50 );
	grid->GetClientSize( &w, &h );
	grid->GetScrollPixelsPerUnit( &w, &h );
	grid->SetScrollRate( 0, h );

	editWait = false;
}

/**
 * Write items from array to grid
 */
void EditSkillAux::WriteGrid()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		Skill * s = ( Skill * ) array[i];
		for( int j = 0; j < grid->GetNumberCols(); j++ ) {
			if( s->days )
				grid->SetCellTextColour( i, j, *wxBLACK );
			else
				grid->SetCellTextColour( i, j, *wxRED );
		}
		grid->SetCellValue( i, 0, SkillDefs[s->type].name );
		AString temp( s->days );
		if( s->days == -1 ) temp = "...";
		grid->SetCellValue( i, 1, temp.Str() );
		grid->SetReadOnly( i, 1, false );
	}
}

void EditSkillAux::FinalizeArray()
{
	if( grid->IsCellEditControlShown() ) {
		SaveEditValue();
	}
	grid->SaveEditControlValue();
	selectedArray.Clear();
	for( int i = grid->GetNumberRows()-1; i >= 0; i-- ) {
		wxString temp = grid->GetCellValue( i, 1 );
		long num = 0;
		if( temp == "..." ) {
			num = -1;
		} else {
			temp.ToLong( &num );
		}
		( ( Skill * ) array[i] )->days = num;
		selectedArray.Add( array[i] );
	}
}

// ---------------------------------------------------------------------------
// EditItemAux
// ---------------------------------------------------------------------------


/**
 * Default constructor
 */
EditItemAux::EditItemAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Items", pos, size )
{
	canDelete = false;
}

/**
 * Default destructor
 */
EditItemAux::~EditItemAux()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		delete ( Item * ) array[i];
	}
}

/**
 * Initialise dialog and array
 */
void EditItemAux::Init( AElemArray * arr )
{
	//set up grid
	editWait = true;
	InitGrid( 2 );
	grid->SetColLabelValue( 0, "Item" );
	grid->SetColLabelValue( 1, "Number" );

	//load items into array
	for( int i = 0; i < NITEMS; i++ ) {
		if( ItemDefs[i].flags & ItemType::DISABLED ) continue;
		Item * it = new Item();
		AString temp = AString();
		Unit * u = ( Unit * ) arr->Item( 0 );
		int amt = u->items.GetNum( i );
		if( amt ) {
			for( int j = 1; j < (int) arr->GetCount(); j++ ) {
				u = ( Unit * )arr->Item( j );
				if( u->items.GetNum( i ) != amt ) {
					amt = -1;
					break;
				}
			}
			if( amt )
				temp += "1";
		}
		temp += ItemDefs[i].names;
		it->SetSortString( temp.Str() );
		it->type = i;
		it->num = amt;
		array.Add( it );
	}
	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount() );
	editCol = 1;
	grid->SetColFormatNumber( 1 );
	WriteGrid();

	//size columns
	InitSizer();
	int w, h;
	grid->GetClientSize( &w, &h );
	grid->SetColSize( 0, w - 51 );
	grid->SetColSize( 1, 50 );
	grid->GetScrollPixelsPerUnit( &w, &h );
	grid->SetScrollRate( 0, h );

	editWait = false;
}

/**
 * Write items from array to grid
 */
void EditItemAux::WriteGrid()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		Item * it = ( Item * ) array[i];
		for( int j = 0; j < grid->GetNumberCols(); j++ ) {
			if( it->num )
				grid->SetCellTextColour( i, j, *wxBLUE );
			else
				grid->SetCellTextColour( i, j, *wxBLACK );
		}
		grid->SetCellValue( i, 0, ItemDefs[it->type].name );
		AString temp( it->num == -1 ? 0 : it->num );
		grid->SetCellValue( i, 1, temp.Str() );
	}
}

void EditItemAux::FinalizeArray()
{
	if( grid->IsCellEditControlShown() ) {
		SaveEditValue();
	}
	grid->SaveEditControlValue();
	selectedArray.Clear();
	for( int i = grid->GetNumberRows()-1; i >= 0; i-- ) {
		wxString temp = grid->GetCellValue( i, 1 );
		long num = 0;
		temp.ToLong( &num );
		if( num ) {
			( (Item * ) array[i] )->num = num;
			selectedArray.Add( array[i] );
		}
	}
}

// ---------------------------------------------------------------------------
// EditTerrainTypeAux
// ---------------------------------------------------------------------------


/**
 * Default constructor
 */
EditTerrainTypeAux::EditTerrainTypeAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Terrain", pos, size )
{
	canDelete = false;
}

/**
 * Default destructor
 */
EditTerrainTypeAux::~EditTerrainTypeAux()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		delete ( ARegion * ) array[i];
	}
}

/**
 * Initialise dialog and array
 */
void EditTerrainTypeAux::Init( AElemArray * arr, int type )
{
	//set up grid
	InitGrid( 5 );
	grid->SetColLabelValue( 0, "Terrain" );
	grid->SetColLabelValue( 1, "Race 1" );
	grid->SetColLabelValue( 2, "Race 2" );
	grid->SetColLabelValue( 3, "Race 3" );
	grid->SetColLabelValue( 4, "Race 4" );

	//load items into array
//	ARegion * r = new ARegion();
//	r->SetSortString( "(none)" );
//	r->type = -1;
//	array.Add( r );

	for( int i = 0; i < R_NUM; i++ ) {
//		if( TerrainDefs[i].flags & TerrainType::DISABLED ) continue;
		if( !type || TerrainDefs[i].flags & type ) {
			ARegion * r = new ARegion();
			AString temp = AString( TerrainDefs[i].name );
			temp += AString(i);
			r->SetSortString( temp.Str() );
			r->type = i;
			array.Add( r );
		}
	}
	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount() );
	WriteGrid();

	//size columns
	InitSizer();
	int w, h;
	grid->GetClientSize( &w, &h );
	grid->SetColSize( 0, w/5 );
	grid->SetColSize( 1, w/5 );
	grid->SetColSize( 2, w/5 );
	grid->SetColSize( 3, w/5 );
	grid->SetColSize( 4, w/5 );
	grid->GetScrollPixelsPerUnit( &w, &h );
	grid->SetScrollRate( 0, h );

	//pre-select items
	PreSelectRows( arr );
}

/**
 * Write items from array to grid
 */
void EditTerrainTypeAux::WriteGrid()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		int j;
		ARegion * r = ( ARegion * )array[i];
		TerrainType tt = TerrainDefs[r->type];
		for( j = 0; j < grid->GetNumberCols(); j++ ) {
			if( app->TerrainHasEnabledRace( r->type ) )
				grid->SetCellTextColour( i, j, *wxBLACK );
			else
				grid->SetCellTextColour( i, j, *wxRED );
		}

		grid->SetCellValue( i, 0, TerrainDefs[r->type].name );
		for( j = 0; j < 4; j++ ) {
			int race = tt.races[j];
			if( race != -1 ) {
				grid->SetCellValue( i, j+1, ItemDefs[race].name );
			}
		}
	}
}

/**
 * Highlight pre-selected rows
 */
void EditTerrainTypeAux::PreSelectRows( AElemArray * arr )
{
	UnhighlightAllRows();
	if( arr ) {
		int i, j = -1;
		for( i = 0; i < (int) arr->GetCount(); i++ ) {
			int type = ( ( ARegion * ) arr->Item( i ) )->type;
			for( j = 0; j < (int) array.GetCount(); j++ ) {
				if( type == ( ( ARegion * ) array[j] )->type ) {
//					grid->SelectRow( j, true );
//					grid->MakeCellVisible( j,0 );
					HighlightRow( j );
					lastRow = j;
				}
			}
		}
//	} else {
//		grid->SelectRow( 0, false );
	}
	ResetView();
}

// ---------------------------------------------------------------------------
// EditObjectAux
// ---------------------------------------------------------------------------


/**
 * Default constructor
 */
EditObjectAux::EditObjectAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Items", pos, size )
{
	canDelete = false;
}

/**
 * Default destructor
 */
EditObjectAux::~EditObjectAux()
{
}

/**
 * Initialise dialog and array
 */
void EditObjectAux::Init( ARegion * pRegion, AElemArray * arr )
{
	//set up grid
	editWait = true;
	InitGrid( 2 );
	grid->SetColLabelValue( 0, "Number" );
	grid->SetColLabelValue( 1, "Name" );

	//load items into array
	forlist( &pRegion->objects ) {
		Object * o = ( Object * ) elem;
		o->SetSortString( AString( o->num ).Str() );
		array.Add( o );
	}

	array.Sort( CompareElem );
	grid->AppendRows( array.GetCount() );
	WriteGrid();

	//size columns
	InitSizer();
	int w, h;
	grid->GetClientSize( &w, &h );
	grid->SetColSize( 0, w - 51 );
	grid->SetColSize( 1, 50 );
	grid->GetScrollPixelsPerUnit( &w, &h );
	grid->SetScrollRate( 0, h );

	editWait = false;
}

/**
 * Write items from array to grid
 */
void EditObjectAux::WriteGrid()
{
	for( int i = 0; i < (int) array.GetCount(); i++ ) {
		Object * o = ( Object * ) array[i];
		grid->SetCellValue( i, 0, AString( o->num ).Str() );
		grid->SetCellValue( i, 1, o->name->Str() );
	}
}


