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
#include "../alist.h"
#include "../aregion.h"
#include "../game.h"
#include "gui.h"

/**
 * Is this List element in the current selection ?
 */
bool GuiApp::IsSelected( AListElem * element )
{
	if( curSelection < _SELECT_ELEMENTS ) {
		if( selectedElems ) {
			if( selectedElems->Index( element ) != wxNOT_FOUND ) {
				return true;
			}
		}
	}
	return false;
}

/**
 * Is the main game the current selection ?
 */
bool GuiApp::IsSelected( Game * game )
{
	if( curSelection == SELECT_GAME && m_game ) return true;

	return false;
}

/**
 * Is this level the current selection ?
 */
bool GuiApp::IsSelected( ARegionArray * level )
{
	if( curSelection == SELECT_LEVEL && selectedLevel ) return true;

	return false;
}

/**
 * Add this element to the current selection.
 */
void GuiApp::AddToSelected( AListElem * element )
{
	selectedElems->Add( element );
}

/**
 * Add this level to the current selection.
 */
void GuiApp::AddToSelected( ARegionArray * level )
{
	selectedLevel = level;
}

/**
 * Add this game to the current selection.
 */
void GuiApp::AddToSelected( Game * game )
{
	// nothing to do here!
}

/**
 * Remove this element from the current selection.
 */
void GuiApp::RemoveFromSelected( AListElem * element )
{
	selectedElems->Remove( element );
}

/**
 * Remove this level from the current selection.
 */
void GuiApp::RemoveFromSelected( ARegionArray * level )
{
	selectedLevel = NULL;
}

/**
 * Remove this game from the current selection.
 */
void GuiApp::RemoveFromSelected( Game * game )
{
	// nothing to do here!
}

/**
 * Set what type of element/game/level the current selection refers to
 */
void GuiApp::SetSelectionType( int type, bool alwaysClear )
{
	if( curSelection == type ) {
		if( alwaysClear ) selectedElems->Clear();
		return;
	}
	selectedElems->Clear();
	selectedLevel = NULL;
	curSelection = type;
}

/**
 * Select an element. If add is true, the element will be added to the array of selected
 * elements, or removed if it is already present.
 * If add is false(default) the selection array will be cleared first, and the new
 * element will be the only selection
 */
void GuiApp::Select( AListElem * element, int selectType, bool add )
{
	if( add ) {
		SetSelectionType( selectType );
		if( IsSelected( element ) ) {
			RemoveFromSelected( element );
		} else {
			AddToSelected( element );
		}
	} else {
		SetSelectionType( selectType, true );
		AddToSelected( element );
	}
}

/**
 * Select a region.
 */
void GuiApp::Select( ARegion * element, bool add )
{
	Select( element, SELECT_REGION, add );
}

/**
 * Select a production.
 */
void GuiApp::Select( Production * element, bool add )
{
	Select( element, SELECT_PRODUCTION, add );
}

/**
 * Select a production.
 */
void GuiApp::Select( Market * element, bool add )
{
	Select( element, SELECT_MARKET, add );
}

/**
 * Select a faction.
 */
void GuiApp::Select( Faction * element, bool add )
{
	Select( element, SELECT_FACTION, add );
}

/**
 * Select a unit
 */
void GuiApp::Select( Unit * element, bool add )
{
	Select( element, SELECT_UNIT, add );
}

/**
 * Select an object.
 */
void GuiApp::Select( Object * element, bool add )
{
	Select( element, SELECT_OBJECT, add );
}

/**
 * Select a level
 */
void GuiApp::Select( ARegionArray * pArray, bool add )
{
	SetSelectionType( SELECT_LEVEL );
	selectedLevel = pArray;
}

/**
 * Select the game
 */
void GuiApp::Select( Game * pGame, bool add )
{
	SetSelectionType( SELECT_GAME );
}

void GuiApp::UpdateSelection( bool addToHistory )
{
	if( addToHistory )
		SaveHistory();
	wxBeginBusyCursor();
//	frame->tree->UpdateSelection();
	frame->map->UpdateSelection();
	frame->list->UpdateSelection();
	frame->editor->UpdateSelection();
	frame->selection->UpdateSelection();
	wxEndBusyCursor();
}

void GuiApp::UpdateSelection( AElemArray * array, int type, bool addToHistory )
{
	wxBeginBusyCursor();
	selectedElems->Clear();
	curSelection = type;
	for( int i = 0; i < (int) array->GetCount(); i++ ) {
		AListElem * elem = array->Item( i );
		AddToSelected( elem );
	}

	if( addToHistory )
		SaveHistory();

//	frame->tree->UpdateSelection();
	frame->map->UpdateSelection();
	frame->list->UpdateSelection();
	frame->editor->UpdateSelection();
	frame->selection->UpdateSelection();
	wxEndBusyCursor();
}

/**
 * Save current selection in selection history
 */
void GuiApp::SaveHistory()
{
	if( selectedElems->GetCount() == 0 ) return;

	/// Check the new selection does not exactly match the old selection
	if( curHistory && curHistory->elems.GetCount() == selectedElems->GetCount() )  {
		bool notfound = false;
		for( int i = 0; i < (int) selectedElems->GetCount(); i++ ) {
			AListElem * elem = selectedElems->Item( i );
			if( curHistory->elems.Index( elem ) == wxNOT_FOUND ) {
				notfound = true;
				break;
			}
		}
		if( !notfound ) {
			// exact same selection
			return;
		}
	}

	/// Delete any future history items
	AListElem * e = curHistory;
	if( e ) e = e->next;

	while( e ) {
		history.Remove( e );
		AListElem * e2 = e->next;
		delete e;
		e = e2;
	}

	/// Create new history item
	HistoryItem * h = new HistoryItem();
	h->elems = *selectedElems;
	h->selectionType = curSelection;
	curHistory = h;

	/// Add to history stack
	history.Add( h );
}

/**
 * Go forward one selection history
 */
void GuiApp::ForwardHistory()
{
	if( curHistory && curHistory->next ) {
		curHistory = ( HistoryItem * ) curHistory->next;
		UpdateSelection( &curHistory->elems, curHistory->selectionType, false );
	}
}

/**
 * Go back one selection history
 */
void GuiApp::BackHistory()
{
	if( curHistory && curHistory->prev ) {
		curHistory = ( HistoryItem * ) curHistory->prev;
		UpdateSelection( &curHistory->elems, curHistory->selectionType, false );
	}
}

/**
 * Default constructor
 */
HistoryItem::HistoryItem()
{
	selectionType = 0;
}
