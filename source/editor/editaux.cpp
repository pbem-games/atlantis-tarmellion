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

#include "editaux.h"
#include "gui.h"

#include "../gamedata.h"
#include "../aregion.h"
#include "../items.h"
#include "../astring.h"
#include "../object.h"
#include "../unit.h"
#include "../faction.h"
#include "../skills.h"
#include "alist.h"

BEGIN_EVENT_TABLE( EditAux, wxDialog )
	EVT_CLOSE( EditAux::OnClose )
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( EditSkillAux, EditAux )
	EVT_GRID_SELECT_CELL( EditSkillAux::OnSelect )
	EVT_CLOSE( EditSkillAux::OnClose )
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( EditItemAux, EditAux )
	EVT_GRID_SELECT_CELL( EditSkillAux::OnSelect )
	EVT_CLOSE( EditSkillAux::OnClose )
END_EVENT_TABLE()

EditAux::EditAux( wxWindow *parent, const wxString & title, const wxPoint& pos, const wxSize& size )
		:wxDialog( parent, -1, title, pos, size, wxDEFAULT_DIALOG_STYLE )
{
}

EditAux::~EditAux()
{
	delete grid;
}

//void EditAux::Init( AElemArray * arr, int type )
//{
//}

void EditAux::InitGrid( int cols, bool selectRows )
{
	grid = new wxGrid( this, -1 );//, wxDefaultPosition, GetClientSize() );
	grid->CreateGrid( 0, 0 );
	grid->SetRowLabelSize( 0 );
	grid->SetColLabelSize( 20 );
	if( selectRows )
		grid->SetSelectionMode( wxGrid::wxGridSelectRows );
	grid->AppendCols( cols );
}

int EditAux::AddRowToGrid( AListElem * aelem, wxGridCellAttr * rowAttr )
{
	grid->AppendRows();
	int row = grid->GetNumberRows() - 1;
	if( rowAttr )
		grid->SetRowAttr( row, rowAttr );
	return row;
}

void EditAux::PreSelectRows( AElemArray * arr )
{
	if( arr ) {
		int i;
		for( i = 0; i < abs( arr->GetCount() ); i++ ) {
			AListElem * e = arr->Item( i );
			int index = array.Index( e );
			if( index != wxNOT_FOUND ) {
				grid->SelectRow( index, true );
				grid->MakeCellVisible( index, 0 );
			}
		}
	} else {
		grid->SelectRow( 0, false );
	}
}

void EditAux::OnClose( wxCloseEvent & event )
{
	for( int i = grid->GetNumberRows()-1; i >= 0; i-- ) {
		if( !( grid->IsInSelection( i, 0 ) ) ) {
			if( canDelete ) delete array[i];
			array.RemoveAt( i );
		}
	}
	event.Skip();
}

// ---------------------------------------------------------------------------
// EditRegionAux
// ---------------------------------------------------------------------------


EditRegionAux::EditRegionAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			  :EditAux( parent, "Regions", pos, size )
{
	canDelete = false;
}

EditRegionAux::~EditRegionAux()
{
	//delete dummy region
	for( int i = 0; i < abs( array.GetCount() ); i++ ) {
		ARegion * r = ( ARegion * ) array[i];
		if( r->num == -1 ) delete r;
		array[i] = NULL;
	}
}

void EditRegionAux::Init( AElemArray * arr )
{
	//set up grid
	InitGrid( 2 );
	grid->SetColLabelValue( 0, "Location" );
	grid->SetColLabelValue( 1, "Name" );

	wxGridCellAttr * rowAttr = new wxGridCellAttr();
	rowAttr->SetReadOnly();

	//load items into grid
	int row, i;

	ARegion * reg = new ARegion();

	reg->sortString = "( none )";
	reg->name = new AString( "" );
	reg->num = -1;
	array.Add( reg );

	forlist( app->GetRegions() )	{
		ARegion * r = ( ARegion * ) elem;

		AString temp = AString( "( " ) + r->xloc + ", " +
						   r->yloc + ", " +
						   r->zloc + " )";
		AString * temp2 = new AString( temp );
		r->sortString = temp2->Str();
		array.Add( r );
	}

	array.Sort( CompareElem );
	for( i = 0; i < abs( array.GetCount() ); i++ ) {
		ARegion * r = ( ARegion * ) array[i];
		row = AddRowToGrid( r, rowAttr );
		grid->SetCellValue( row, 0, r->sortString );
		grid->SetCellValue( row, 1, r->name->Str() );
	}

	//size columns
	InitSizer();
	int w,h;
	grid->GetClientSize( &w,&h );
	grid->SetColSize( 0,50 );
	grid->SetColSize( 1,w-51 );

	//pre-select items
	PreSelectRows( arr );
}

// ---------------------------------------------------------------------------
// EditSkillTypeAux
// ---------------------------------------------------------------------------


EditSkillTypeAux::EditSkillTypeAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Skills", pos, size )
{
	canDelete = true;
}

EditSkillTypeAux::~EditSkillTypeAux()
{
}

void EditSkillTypeAux::Init( AElemArray * arr, int type )
{
	//set up grid
	InitGrid( 1 );
	grid->SetColLabelValue( 0, "Skill" );

	wxGridCellAttr * rowAttr = new wxGridCellAttr();
	rowAttr->SetReadOnly();

	//load items into grid
	int row, i;

	Skill * s = new Skill();
	s->sortString = "( none )";
	s->type = -1;
	array.Add( s );

	for( i = 0; i<NSKILLS; i++ ) {
		if( SkillDefs[i].flags & SkillType::DISABLED ) continue;
		if( !type || SkillDefs[i].flags & type ) {
			s = new Skill();
			s->sortString = SkillDefs[i].name;
			s->type = i;
			array.Add( s );
		}
	}
	array.Sort( CompareElem );
	for( i = 0; i < abs( array.GetCount() ); i++ ) {
		s = ( Skill * )array[i];
		row = AddRowToGrid( s, rowAttr );
		grid->SetCellValue( row, 0, s->sortString );
	}

	//size columns
	InitSizer();
	int w,h;
	grid->GetClientSize( &w,&h );
	grid->SetColSize( 0,w-1 );

	//pre-select items
	PreSelectRows( arr );
}

void EditSkillTypeAux::PreSelectRows( AElemArray * arr )
{
	if( arr ) {
		int i,j = -1;
		for( i = 0; i < abs( arr->GetCount() ); i++ ) {
			int type = ( ( Skill * ) arr->Item( i ) )->type;
			for( j = 0; j < abs( array.GetCount() ); j++ ) {
				if( type == ( ( Skill * ) array[j] )->type ) {
					grid->SelectRow( j, true );
					grid->MakeCellVisible( j,0 );
				}
			}
		}
	} else {
		grid->SelectRow( 0, false );
	}
}

// ---------------------------------------------------------------------------
// EditItemTypeAux
// ---------------------------------------------------------------------------


EditItemTypeAux::EditItemTypeAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Items", pos, size )
{
	canDelete = true;
}

EditItemTypeAux::~EditItemTypeAux()
{
}

void EditItemTypeAux::Init( AElemArray * arr, int type )
{
	//set up grid
	InitGrid( 1 );
	grid->SetColLabelValue( 0, "Item" );

	wxGridCellAttr * rowAttr = new wxGridCellAttr();
	rowAttr->SetReadOnly();

	//load items into grid
	int row, i;

	Item * it = new Item();
	it->type = -1;
	it->sortString = "( none )";
	array.Add( it );

	for( i=0; i<NITEMS; i++ ) {
		if( ItemDefs[i].flags & ItemType::DISABLED ) continue;
		if( !type || ItemDefs[i].type & type ) {
			it = new Item();
			it->type = i;
			it->sortString = ItemDefs[i].name;
			array.Add( it );
		}
	}
	array.Sort( CompareElem );
	for( i = 0; i < abs( array.GetCount() ); i++ ) {
		it = ( Item * ) array[i];
		row = AddRowToGrid( it, rowAttr );
		grid->SetCellValue( row, 0, it->sortString );
	}

	//size columns
	InitSizer();
	int w,h;
	grid->GetClientSize( &w,&h );
	grid->SetColSize( 0,w-1 );

	//pre-select items
	PreSelectRows( arr );

}

void EditAux::InitSizer()
{
	int w,h;
	
	sizerAux = new wxBoxSizer( wxVERTICAL );
	sizerAux->Add( grid, 1, wxEXPAND );
	GetClientSize( &w,&h );
	sizerAux->SetDimension( 0,0,w,h );
}

void EditItemTypeAux::PreSelectRows( AElemArray * arr )
{
	if( arr ) {
		int i,j = -1;
		for( i = 0; i < abs( arr->GetCount() ); i++ ) {
			int type = ( ( Item * ) arr->Item( i ) )->type;
			for( j = 0; j < abs( array.GetCount() ) ; j++ ) {
				if( type == ( ( Item * ) array[j] )->type ) {
					grid->SelectRow( j, true );
					grid->MakeCellVisible( j,0 );
				}
			}
		}
	} else {
		grid->SelectRow( 0, false );
	}
}

// ---------------------------------------------------------------------------
// EditFactionAux
// ---------------------------------------------------------------------------


EditFactionAux::EditFactionAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Factions", pos, size )
{
	canDelete = false;
}

EditFactionAux::~EditFactionAux()
{
}

void EditFactionAux::Init( AElemArray * arr )
{
	//set up grid
	InitGrid( 2 );
	grid->SetColLabelValue( 0, "Number" );
	grid->SetColLabelValue( 1, "Name" );

	wxGridCellAttr * rowAttr = new wxGridCellAttr();
	rowAttr->SetReadOnly();

	//load items into grid
	int row, i;

//	Faction * f = new Faction();
//	f->num = -1;
//	f->sortString = "( none )";
//	array.Add( it );

	forlist( app->GetFactions() ) {
		Faction * f = ( Faction * ) elem;
		AString * num = new AString( f->num );
		f->sortString = num->Str();
		array.Add( f );
	}
	array.Sort( CompareElem );
	for( i = 0; i < abs( array.GetCount() ); i++ ) {
		Faction * f = ( Faction * ) array[i];
		row = AddRowToGrid( f, rowAttr );
		grid->SetCellValue( row, 0, f->sortString );
		grid->SetCellValue( row, 1, f->name->Str() );
	}

	//size columns
	InitSizer();
	int w,h;
	grid->GetClientSize( &w,&h );
	grid->SetColSize( 0,40 );
	grid->SetColSize( 1,w-41 );

	//pre-select items
	PreSelectRows( arr );

}

// ---------------------------------------------------------------------------
// EditSkillAux
// ---------------------------------------------------------------------------


EditSkillAux::EditSkillAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Skills", pos, size )
{
	canDelete = false;
}

EditSkillAux::~EditSkillAux()
{
}

void EditSkillAux::Init( AElemArray * arr )
{
	//set up grid
	editWait = true;
	InitGrid( 2 );
	grid->SetColLabelValue( 0, "Skill" );
	grid->SetColLabelValue( 1, "Days" );

	wxGridCellAttr * hiliteRowAttr = new wxGridCellAttr();
	hiliteRowAttr->SetTextColour( *wxBLUE );

	wxGridCellAttr * colSkillAttr = new wxGridCellAttr();
	colSkillAttr->SetReadOnly();

	//load items into grid
	int row, i, j;

	for( i = 0; i<NSKILLS; i++ ) {
		if( SkillDefs[i].flags & SkillType::DISABLED ) continue;
		Skill * s = new Skill();
		AString * temp = new AString();
		Unit * u = ( Unit * ) arr->Item( 0 );
		int days = u->skills.GetDays( i ) / u->GetMen();
		if( days ) {
			for( j = 1; j < abs( arr->GetCount() ); j++ ) {
				u = ( Unit * )arr->Item( j );
				if( u->skills.GetDays( i ) / u->GetMen() != days ) {
					days = -1;
					break;
				}
			}
			if( days )
				*temp += "1";
		}
		*temp += SkillDefs[i].name;
		s->sortString = temp->Str();
		s->type = i;
		s->days = days;
		array.Add( s );
	}
	array.Sort( CompareElem );
	for( i = 0; i < abs( array.GetCount() ); i++ ) {
		Skill * s = ( Skill * ) array[i];
		if( s->days ) {
			row = AddRowToGrid( s, hiliteRowAttr );
		} else {
			row = AddRowToGrid( s );
		}
		grid->SetCellValue( row, 0, SkillDefs[s->type].name );
		AString temp2( s->days == -1 ? 0 : s->days );
		grid->SetCellValue( row, 1, temp2.Str() );
	}
	grid->SetColAttr( 0, colSkillAttr );
	grid->SetColFormatNumber( 1 );

	//size columns
	InitSizer();
	int w,h;
	grid->GetClientSize( &w,&h );
	grid->SetColSize( 0,w-51 );
	grid->SetColSize( 1,50 );

	editWait = false;
}

void EditSkillAux::OnClose( wxCloseEvent & event )
{
	grid->SaveEditControlValue();
	for( int a = grid->GetNumberRows()-1; a >= 0; a-- ) {
		Skill * s = ( Skill * ) array[a];
		wxString temp = grid->GetCellValue( a, 1 );
		long num = 0;
		temp.ToLong( &num );
		s->days = num;
	}
	grid->SelectAll();
	event.Skip();
}

void EditSkillAux::OnSelect( wxGridEvent & event )
{
	if( !editWait ) {

//		int col = event.GetCol();
//		if( col == 0 ) {
//			grid->MoveCursorRight( FALSE );
	//		if( grid->CanEnableCellControl() )
//			grid->EnableCellEditControl();
	//			grid->ShowCellEditControl();
//		}
	//		int row = event.GetRow();
	//		grid->SelectBlock( row,1,row,1 );
	//	}
	}
	event.Skip();

}

// ---------------------------------------------------------------------------
// EditItemAux
// ---------------------------------------------------------------------------


EditItemAux::EditItemAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Items", pos, size )
{
	canDelete = false;
}

EditItemAux::~EditItemAux()
{
}

void EditItemAux::Init( AElemArray * arr )
{
	//set up grid
	editWait = true;
	InitGrid( 2 );
	grid->SetColLabelValue( 0, "Item" );
	grid->SetColLabelValue( 1, "Number" );

	wxGridCellAttr * hiliteRowAttr = new wxGridCellAttr();
	hiliteRowAttr->SetTextColour( *wxBLUE );

	wxGridCellAttr * colItemAttr = new wxGridCellAttr();
	colItemAttr->SetReadOnly();

	//load items into grid
	int row, i, j;

	for( i = 0; i<NITEMS; i++ ) {
		if( ItemDefs[i].flags & ItemType::DISABLED ) continue;
		Item * it = new Item();
		AString * temp = new AString();
		Unit * u = ( Unit * ) arr->Item( 0 );
		int amt = u->items.GetNum( i );
		if( amt ) {
			for( j = 1; j < abs( arr->GetCount() ); j++ ) {
				u = ( Unit * )arr->Item( j );
				if( u->items.GetNum( i ) != amt ) {
					amt = -1;
					break;
				}
			}
			if( amt )
				*temp += "1";
		}
		*temp += ItemDefs[i].names;
		it->sortString = temp->Str();
		it->type = i;
		it->num = amt;
		array.Add( it );
	}
	array.Sort( CompareElem );
	for( i = 0; i < abs( array.GetCount() ); i++ ) {
		Item * it = ( Item * ) array[i];
		if( it->num ) {
			row = AddRowToGrid( it, hiliteRowAttr );
		} else {
			row = AddRowToGrid( it );
		}
		grid->SetCellValue( row, 0, ItemDefs[it->type].name );
		AString temp2( it->num == -1 ? 0 : it->num );
		grid->SetCellValue( row, 1, temp2.Str() );
	}
	grid->SetColAttr( 0, colItemAttr );
	grid->SetColFormatNumber( 1 );

	//size columns
	InitSizer();
	int w,h;
	grid->GetClientSize( &w,&h );
	grid->SetColSize( 0,w-51 );
	grid->SetColSize( 1,50 );

	editWait = false;
}

void EditItemAux::OnClose( wxCloseEvent & event )
{
	grid->SaveEditControlValue();
	for( int a = grid->GetNumberRows()-1; a >= 0; a-- ) {
		Item * it = ( Item * ) array[a];
		wxString temp = grid->GetCellValue( a, 1 );
		long num = 0;
		temp.ToLong( &num );
		it->num = num;
	}
	grid->SelectAll();
	event.Skip();
}

void EditItemAux::OnSelect( wxGridEvent & event )
{
	if( !editWait ) {

		int col = event.GetCol();
		if( col == 0 ) {
			int row = event.GetRow();
			grid->SelectBlock( row,1,row,1 );
		}
	}
	event.Skip();

}

// ---------------------------------------------------------------------------
// EditTerrainTypeAux
// ---------------------------------------------------------------------------


EditTerrainTypeAux::EditTerrainTypeAux( wxWindow *parent, const wxPoint& pos, const wxSize& size )
			 :EditAux( parent, "Skills", pos, size )
{
	canDelete = true;
}

EditTerrainTypeAux::~EditTerrainTypeAux()
{
}

void EditTerrainTypeAux::Init( AElemArray * arr, int type )
{
	//set up grid
	InitGrid( 5 );
	grid->SetColLabelValue( 0, "Terrain" );
	grid->SetColLabelValue( 1, "Race 1" );
	grid->SetColLabelValue( 2, "Race 2" );
	grid->SetColLabelValue( 3, "Race 3" );
	grid->SetColLabelValue( 4, "Race 4" );

	wxGridCellAttr * rowAttr = new wxGridCellAttr();
	rowAttr->SetReadOnly();
	wxGridCellAttr * hiliteRowAttr = new wxGridCellAttr();
	hiliteRowAttr->SetReadOnly();
	hiliteRowAttr->SetTextColour(*wxRED);

	//load items into grid
	int row, i;

	ARegion * r = new ARegion();
	r->sortString = "( none )";
	r->type = -1;
	array.Add( r );

	for( i = 0; i<R_NUM; i++ ) {
//		if( TerrainDefs[i].flags & TerrainType::DISABLED ) continue;
		if( !type || TerrainDefs[i].flags & type ) {
			r = new ARegion();
			AString * temp = new AString( TerrainDefs[i].name );
			*temp += AString(i);
			r->sortString = temp->Str();
			r->type = i;
			array.Add( r );
		}
	}
	array.Sort( CompareElem );
	for( i = 0; i < abs( array.GetCount() ); i++ ) {
		r = ( ARegion * )array[i];
		TerrainType tt = TerrainDefs[r->type];
		if( app->TerrainHasEnabledRace( r->type ) )
			row = AddRowToGrid( r, rowAttr );
		else
			row = AddRowToGrid( r, hiliteRowAttr );

		grid->SetCellValue( row, 0, TerrainDefs[r->type].name );
		for( int j = 0; j < 4; j++ ) {
			int race = tt.races[j];
			if( race != -1 )
				grid->SetCellValue( row, j+1, ItemDefs[race].name );
		}
	}

	//size columns
	InitSizer();
	int w,h;
	grid->GetClientSize( &w,&h );
	grid->SetColSize( 0,w/5 );
	grid->SetColSize( 1,w/5 );
	grid->SetColSize( 2,w/5 );
	grid->SetColSize( 3,w/5 );
	grid->SetColSize( 4,w/5 );

	//pre-select items
	PreSelectRows( arr );
}

void EditTerrainTypeAux::PreSelectRows( AElemArray * arr )
{
	if( arr ) {
		int i,j = -1;
		for( i = 0; i < abs( arr->GetCount() ); i++ ) {
			int type = ( ( ARegion * ) arr->Item( i ) )->type;
			for( j = 0; j < abs( array.GetCount() ); j++ ) {
				if( type == ( ( ARegion * ) array[j] )->type ) {
					grid->SelectRow( j, true );
					grid->MakeCellVisible( j,0 );
				}
			}
		}
	} else {
		grid->SelectRow( 0, false );
	}
}
