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

#include "selection.h"
#include "gui.h"
#include "config.h"

#include "bitmaps/tree.xpm"

#include "../alist.h"
#include "../aregion.h"
#include "../faction.h"
#include "../object.h"
#include "../unit.h"
#include "../market.h"
#include "../production.h"

#include "bitmaps/arrowleft.xpm"
#include "bitmaps/arrowright.xpm"
#include "bitmaps/zoom_in.xpm"
#include "bitmaps/zoom_out.xpm"

#include "wx/toolbar.h"

BEGIN_EVENT_TABLE( SelectionCanvas, wxScrolledWindow )
	EVT_SIZE( SelectionCanvas::OnSize )
	EVT_TREE_SEL_CHANGED( Selection_Tree, SelectionCanvas::OnLeafSelection )
//	EVT_TREE_SEL_CHANGING( Selection_Tree, SelectionCanvas::OnLeafSelecting )
	EVT_MENU( Selection_Tool_Back, SelectionCanvas::OnBack )
	EVT_MENU( Selection_Tool_Forward, SelectionCanvas::OnForward )
	EVT_MENU( Selection_Tool_Zoom_In, SelectionCanvas::OnZoomIn )
	EVT_MENU( Selection_Tool_Zoom_Out, SelectionCanvas::OnZoomOut )
	EVT_TREE_ITEM_ACTIVATED( Selection_Tree, SelectionCanvas::OnZoomIn )
END_EVENT_TABLE()

// ---------------------------------------------------------------------------
// SelectionCanvas
// ---------------------------------------------------------------------------

SelectionCanvas::SelectionCanvas( wxWindow *parent )
			    :wxScrolledWindow( parent, -1, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL )
{
//	SetIcon( wxIcon( tree_xpm ) );
	tree = 0;
	treeWait = false;
	selectedElems = new AElemArray();
	selectedLevel = NULL;
	curSelection = -1;
	selectOK = 1;
	SetBackgroundColour( app->guiColourLt );
	lastUnitByFaction = false;

	toolBar = new wxToolBar( this, -1, wxDefaultPosition, wxDefaultSize,
	                         wxTB_HORIZONTAL | wxNO_BORDER | wxTB_FLAT );
    wxBitmap* bitmaps[4];

    bitmaps[0] = new wxBitmap( arrowleft_xpm );
    bitmaps[1] = new wxBitmap( arrowright_xpm );
    bitmaps[2] = new wxBitmap( zoom_in_xpm );
    bitmaps[3] = new wxBitmap( zoom_out_xpm );

	toolBar->AddTool( Selection_Tool_Back, "", *( bitmaps[0] ), "Back" );
	toolBar->AddTool( Selection_Tool_Forward, "", *( bitmaps[1] ), "Forward" );
	toolBar->AddTool( Selection_Tool_Zoom_In, "", *( bitmaps[2] ), "Zoom in" );
	toolBar->AddTool( Selection_Tool_Zoom_Out, "", *( bitmaps[3] ), "Zoom out" );

    for( int i = 0; i < 4; i++ )
        delete bitmaps[i];

    toolBar->Realize();

//	sizerSelect = new wxBoxSizer( wxVERTICAL );
//	sizerTool = new wxBoxSizer( wxHORIZONTAL );
//	sizerTool->Add( toolBar, 1 );
}

SelectionCanvas::~SelectionCanvas()
{
	delete selectedElems;
	delete tree;
}


void SelectionCanvas::Resize()
{
	int w, h;
	GetClientSize( &w, &h );
//	if( w > 0 && h > 0 ) {
		int offset = toolBar->GetSize().y;
//		SetScrollbars( 5, 10, w / 5, h / 10 );
//		sizerSelect->SetDimension( 1, offset + 1, w-1, h-5 - offset );
//		sizerTool->SetDimension( 1, 1, w-1, offset - 1 );
//	}
	toolBar->SetSize( 1, 1, w-1, offset );
	if( tree ) {
		tree->SetSize( 1, offset+1, w-1, h -5 -offset );
	}
}

void SelectionCanvas::OnSize( wxSizeEvent & event )
{
	Resize();
//	if( tree ) {
//		int w, h;
//		GetClientSize( &w,&h );
//
//		tree->SetSize( w,h );
//	}
}

/**
 * Intercept leaf selection events
 * When leaf selected, enable/disable toolbar buttons as required
 * Also, automatically expand any children under an 'Object' header leaf
 */
void SelectionCanvas::OnLeafSelection( wxTreeEvent & event )
{
	wxTreeItemId id = event.GetItem();
	if( !id.IsOk() ) return; // this may happen a few times with every leaf selection for some reason..

	TreeLeaf * t = ( TreeLeaf * ) tree->GetItemData( id );

	if( t && curSelection != SELECT_GAME ) {
		// If we select a leaf the is part of the game, allow user to move selection
		// to the leaf's parent, or crop the selection
		toolBar->EnableTool( Selection_Tool_Zoom_Out, true );
		wxTreeItemIdValue cookie;

		// If we've selected the only leaf in the first tree level, no need
		// to zoom in, as we'll have exactly the same result
		if( selectedElems->GetCount() == 1 &&
			tree->GetFirstChild( tree->GetRootItem(), cookie ) == id )
		{
			toolBar->EnableTool( Selection_Tool_Zoom_In, false );
		} else {
            toolBar->EnableTool( Selection_Tool_Zoom_In, true );
		}
	} else {
		// No game entity associated with this leaf, so disable buttons
		toolBar->EnableTool( Selection_Tool_Zoom_Out, false );
		toolBar->EnableTool( Selection_Tool_Zoom_In, false );

		// Expand children if we've selected an Object leaf header
		if( tree->GetItemText( id ) == "Object" ) {
			wxTreeItemIdValue cookie;
			wxTreeItemId obj = tree->GetFirstChild( id, cookie );
			while( obj.IsOk() ) {
				tree->Expand( obj );
				obj = tree->GetNextChild( id, cookie );
			}
		}
	}
}

void SelectionCanvas::OnLeafSelecting( wxTreeEvent & event )
{
//	if( treeWait ) {
//		event.Veto();
//		event.Skip();
//		return;
//	}
//	treeWait = true;

//	TreeLeaf * t = ( TreeLeaf * ) tree->GetItemData( event.GetItem() );
//	if( t )
//		t->MakeSelection();

//	app->UpdateSelection();

//	event.Veto();
//	event.Skip();
//	treeWait = false;
}

void SelectionCanvas::DeselectAll()
{
	return;
//	UnHighlightLeaf( lastSelected );
//	lastSelected = wxTreeItemId();
	if( tree->GetWindowStyle() & wxTR_MULTIPLE ) {
		wxArrayTreeItemIds array;
		int num = tree->GetSelections( array );
		if( num )
			tree->UnselectAll();
	} else {
		wxTreeItemId id = tree->GetSelection();
		if( id.IsOk() )
			tree->UnselectAll();
	}
}

void SelectionCanvas::UnHighlightLeaf( wxTreeItemId id )
{
	if( id.IsOk() ) {
		tree->SetItemTextColour( id, wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
		tree->SetItemBackgroundColour( id, GetBackgroundColour()/*wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE )*/ );
	}
}

void SelectionCanvas::HighlightLeaf( wxTreeItemId id )
{
	if( id.IsOk() ) {
		tree->SetItemTextColour( id, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
		tree->SetItemBackgroundColour( id, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
		tree->SelectItem( wxTreeItemId() );
	}
}

bool SelectionCanvas::IsLeafHighlighted( wxTreeItemId id )
{
	if( tree->GetItemBackgroundColour( id ) == wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) )
		return true;

	return false;
}

void SelectionCanvas::AddItem( Game * game, wxTreeItemId parent )
{
	if( !parent.IsOk() )
		tree->AddRoot( "Atlantis", -1, -1, new GameLeaf( game ) );
	else
	  	wxTreeItemId leaf = tree->AppendItem( parent, "Atlantis", -1, -1,
							new GameLeaf( game ) );
}

void SelectionCanvas::AddItem( ARegionArray * level, wxTreeItemId parent, bool populate )
{
	int levnum = 0;
	ARegion * r = level->GetRegion( 0, 0 );
	if( r ) levnum = r->zloc;

	AString temp = AString( "(" ) + levnum + ") ";

	if( level->strName )
		temp += *level->strName;
	else
		temp += "surface";

  	wxTreeItemId leaf = tree->AppendItem( parent, temp.Str(), -1, -1,
						new LevelLeaf( level, levnum ) );

	if( populate ) {
		for( int y = 0; y < level->y; y++ ) {
			for( int x = 0; x < level->x; x++ ) {
				ARegion * r = level->GetRegion( x, y );
				if( !r ) continue;
				AddItem( r, leaf );
			}
		}
	}
}

void SelectionCanvas::AddItem( Faction * pFaction, wxTreeItemId parent, bool populate )
{
  	wxTreeItemId leaf = tree->AppendItem( parent, pFaction->name->Str(), -1, -1,
					 new FactionLeaf( pFaction ) );

	if( populate ) {
		forlist( &app->m_game->regions ) {
			ARegion * r = ( ARegion * ) elem;
			forlist( &r->objects ) {
				Object * o = ( Object * ) elem;
				forlist ( &o->units ) {
					Unit * u = ( Unit * ) elem;
					if( u->faction == pFaction ) {
						AddItem( u, leaf );
					}
				}
			}
		}
	}
}

void SelectionCanvas::AddItem( ARegion * pRegion, wxTreeItemId parent, bool populate )
{
	AString temp = AString( "(" ) + pRegion->xloc + ", " +
		pRegion->yloc + ", " + pRegion->zloc + ") " + *pRegion->name;

	if( pRegion->town )
		temp += AString( " [" ) + *pRegion->town->name + "]";

	wxTreeItemId leaf = tree->AppendItem( parent, temp.Str(), -1, -1, new RegionLeaf( pRegion ) );

	wxTreeItemId current = leaf;

	if( populate ) {
		if( GuiConfig.showTreeHeaders ) current = tree->AppendItem( leaf, "Markets" );
		{
			forlist( &pRegion->markets ) {
				Market * m = ( Market * ) elem;
				AddItem( m, current );
			}
		}
		//products
		if( GuiConfig.showTreeHeaders ) current = tree->AppendItem( leaf, "Products" );
		{
			forlist ( &pRegion->products ) {
				Production * p = ( Production * ) elem;
				AddItem( p, current );
			}
		}
		//objects
		if( GuiConfig.showTreeHeaders ) current = tree->AppendItem( leaf, "Objects" );
		forlist ( &pRegion->objects ) {
			Object * o = ( Object * ) elem;
			AddItem( o, current );
		}
	}
}

void SelectionCanvas::AddItem( Market * pMarket, wxTreeItemId parent )
{
	// temp
	if( pMarket->item < 0 ) {
		return;
	}

	AString temp = AString( ( pMarket->type == M_BUY?"Sells ":"Buys " ) );
	temp += ItemString( pMarket->item, pMarket->amount ) + " at $" +
			pMarket->price;
	pMarket->treeId = tree->AppendItem( parent, temp.Str(), -1, -1,
					new MarketLeaf( pMarket ) );
}

void SelectionCanvas::AddItem( Production * pProduction, wxTreeItemId parent )
{
	AString temp = AString( "Produces ") + ItemString( pProduction->itemtype, pProduction->amount );
	pProduction->treeId = tree->AppendItem( parent, temp.Str(), -1, -1,
					new ProductionLeaf( pProduction ) );
}

void SelectionCanvas::AddItem( Object * pObject, wxTreeItemId parent, bool populate )
{
	AString temp = *pObject->name + " : " + ObjectDefs[pObject->type].name;
	wxTreeItemId leaf = tree->AppendItem( parent, temp.Str(), -1, -1,
						new ObjectLeaf( pObject ) );
	if( populate ) {
		forlist ( &pObject->units ) {
			Unit * u = ( Unit * ) elem;
			AddItem( u, leaf );
		}
	}
}

void SelectionCanvas::RemoveItem( Faction * pFaction )
{
	wxTreeItemId id = FindItem( pFaction );
	tree->Delete( id );
}

void SelectionCanvas::RemoveItem( ARegion * pRegion )
{
	wxTreeItemId id = FindItem( pRegion );
	tree->Delete( id );
}

void SelectionCanvas::RemoveItem( Market * pMarket )
{
	wxTreeItemId id = FindItem( pMarket );
	tree->Delete( id );
}

void SelectionCanvas::RemoveItem( Production * pProduction )
{
	wxTreeItemId id = FindItem( pProduction );
	tree->Delete( id );
}

void SelectionCanvas::RemoveItem( Object * pObject )
{
	wxTreeItemId id = FindItem( pObject );
	tree->Delete( id );
}

void SelectionCanvas::RemoveItem( Unit * pUnit )
{
	wxTreeItemId id = FindItem( pUnit );
	tree->Delete( id );
}

void SelectionCanvas::UpdateItem( Faction * pFaction )
{
	wxTreeItemId id = FindItem( pFaction );
	tree->SetItemText( id, pFaction->name->Str() );
}

void SelectionCanvas::UpdateItem( ARegionArray * pAArray )
{
	wxTreeItemId id = FindItem( pAArray );
	int num = ( (LevelLeaf *) tree->GetItemData( id ) )->level;
	AString temp = AString( "(" ) + num + ") ";
	if( pAArray->strName ) temp += *pAArray->strName; 
	tree->SetItemText( id, temp.Str() );
}

void SelectionCanvas::UpdateItem( ARegion * pRegion, bool populate )
{
/*	wxTreeItemId id = FindItem( pRegion );
	AString temp = AString( "(" ) + pRegion->xloc + ", " +
	                        pRegion->yloc + ", "+ pRegion->zloc +
							") " + *pRegion->name;
	if( pRegion->town ) temp += AString( " [" ) + *pRegion->town->name + "]";
	tree->SetItemText( id, temp.Str() );

	if( populate ) {
		wxTreeItemIdValue cookie;
		bool found = true;
	
		wxTreeItemId current, next, last;

		if( !GuiConfig.showTreeHeaders ) {
			//remove markets and products
			current = tree->GetFirstChild( id, cookie );
			last = tree->GetLastChild( id );
			while( current.IsOk() ) {
				TreeLeaf * leaf = (TreeLeaf *) tree->GetItemData( current );
				next = tree->GetNextChild( id, cookie );
				if( leaf->class_id == LEAF_MAR || leaf->class_id == LEAF_PRO )
					tree->Delete( current );
				if( !next.IsOk() )
					break;
				current = next;
			}
			current = id;
		} else {
			current = FindCategory( "Markets", id );
		}

		if( current.IsOk() ) {
			if( GuiConfig.showTreeHeaders ) {
				tree->DeleteChildren( current );
			}
			forlist( &pRegion->markets ) {
				Market * m = ( Market * ) elem;
				if( m->item == -1 )
					continue;
				//Hack:
				m->region = pRegion->num;
				temp = AString( ( m->type == M_BUY?"Sells ":"Buys " ) );
				temp += ItemString( m->item, m->amount ) + " at $" + m->price;
				tree->AppendItem( current, _T( temp.Str() ), -1, -1,
								 new MarketLeaf( m ) );
			}
		}

		found = true;
		if( GuiConfig.showTreeHeaders ) {
			current = FindCategory( "Products", id );
		}

		if( current.IsOk() ) {
			if( GuiConfig.showTreeHeaders ) {
				tree->DeleteChildren( current );
			}
			forlist( &pRegion->products ) {
				Production * p = ( Production * ) elem;
				//Hack:
				p->region = pRegion->num;
				temp = AString( "Produces ") + ItemString( p->itemtype, p->amount );
				tree->AppendItem( current, _T( temp.Str() ), -1, -1,
								new ProductionLeaf( p ) );
			}
		}
	}
*/}

void SelectionCanvas::UpdateItem( Market * pMarket)
{
	wxTreeItemId id = FindItem( pMarket );
	AString temp = AString( ( pMarket->type == M_BUY?"Sells ":"Buys " ) );
	temp += ItemString( pMarket->item, pMarket->amount ) +
	                    " at $" + pMarket->price;
	tree->SetItemText( id, temp.Str() );
}

void SelectionCanvas::UpdateItem( Production * pProduction)
{
	wxTreeItemId id = FindItem( pProduction );
	AString temp = ItemString( pProduction->itemtype, pProduction->amount );
	tree->SetItemText( id, temp.Str() );
}

void SelectionCanvas::UpdateItem( Object * pObject)
{
	wxTreeItemId id = FindItem( pObject );
	AString	temp = *pObject->name + " : " + ObjectDefs[pObject->type].name;
	tree->SetItemText( id, temp.Str() );
}

void SelectionCanvas::UpdateItem( Unit * pUnit)
{
	wxTreeItemId id = FindItem( pUnit );

	AString temp = *pUnit->name;
	tree->SetItemText( id, temp.Str() );
}


void SelectionCanvas::AddItem( Unit * pUnit, wxTreeItemId parent )
{
	AString temp = *pUnit->name;

	tree->AppendItem( parent, temp.Str(), -1, -1, 
					  new UnitLeaf( pUnit, true ) );
}

wxTreeItemId SelectionCanvas::FindItem( ARegionArray * pArr )
{
	if( curSelection != SELECT_LEVEL ) return wxTreeItemId();

	wxTreeItemIdValue cookie;
	wxTreeItemId level = tree->GetFirstChild( tree->GetRootItem(), cookie );
	if( !level.IsOk() ) return wxTreeItemId();

	wxTreeItemId lastlevel = tree->GetLastChild( tree->GetRootItem() );
	bool found = true;

	while ( ( ( LevelLeaf * )tree->GetItemData( level ) )->regionArray !=
			 pArr )
		{
		if( level == lastlevel ) {
			found = false;
			break;
		}
		level = tree->GetNextChild( tree->GetRootItem(), cookie );
	}
	if( found )
		return level;

	return wxTreeItemId();
}


wxTreeItemId SelectionCanvas::FindItem( ARegion * pRegion )
{
	if( curSelection != SELECT_REGION ) return wxTreeItemId();

	wxTreeItemIdValue cookie;
	wxTreeItemId region = tree->GetFirstChild( tree->GetRootItem(), cookie );
	if( !region.IsOk() ) return wxTreeItemId();

	bool found = true;
	wxTreeItemId lastregion = tree->GetLastChild( tree->GetRootItem() );
	
	wxString dbg = tree->GetItemText( region );

	while ( ( ( RegionLeaf * )tree->GetItemData( region ) )->region !=
			pRegion )
	{
		if( region == lastregion ) {
			found = false;
			break;
		}
		region = tree->GetNextChild( tree->GetRootItem(), cookie );
	}
	if( found )
		return region;

	return wxTreeItemId();
}

wxTreeItemId SelectionCanvas::FindItem( Faction * pFaction )
{
	if( curSelection != SELECT_FACTION ) return wxTreeItemId();

	wxTreeItemIdValue cookie;
	wxTreeItemId faction = tree->GetFirstChild( tree->GetRootItem(), cookie );
	if( !faction.IsOk() ) return wxTreeItemId();

	wxTreeItemId lastfaction = tree->GetLastChild( tree->GetRootItem() );
	bool found = true;

	while ( ( ( FactionLeaf * )tree->GetItemData( faction ) )->faction !=
		   pFaction )
	{
		if( faction == lastfaction ) {
			found = false;
			break;
		}
		faction = tree->GetNextChild( tree->GetRootItem(), cookie );
	}
	if( found )
		return faction;

	return wxTreeItemId();
}

wxTreeItemId SelectionCanvas::FindItem( Object * pObject )
{
	if( curSelection != SELECT_OBJECT ) return wxTreeItemId();

	wxTreeItemIdValue cookie;
	wxTreeItemId current = tree->GetFirstChild( tree->GetRootItem(), cookie );
	if( !current.IsOk() ) return wxTreeItemId();

	wxTreeItemId last = tree->GetLastChild( tree->GetRootItem() );
	bool found = true;

	while ( ( ( ObjectLeaf * )tree->GetItemData( current ) )->object !=
			pObject )
	{
		if( current == last ) {
			found = false;
			break;
		}
		current = tree->GetNextChild( tree->GetRootItem(), cookie );
	}
	if( found )
		return current;
	
	return wxTreeItemId();
}

wxTreeItemId SelectionCanvas::FindItem( Market * pMarket )
{
	if( curSelection != SELECT_MARKET ) return wxTreeItemId();

	wxTreeItemIdValue cookie;
	wxTreeItemId current = tree->GetFirstChild( tree->GetRootItem(), cookie );
	if( ! current.IsOk() ) return wxTreeItemId();

	wxTreeItemId last = tree->GetLastChild( tree->GetRootItem() );
	bool found = true;

	while ( ( ( MarketLeaf * )tree->GetItemData( current ) )->market !=
		pMarket )
	{
		if( current == last ) {
			found = false;
			break;
		}
		current = tree->GetNextChild( tree->GetRootItem(), cookie );
	}
	if( found )
		return current;

	return wxTreeItemId();
}

wxTreeItemId SelectionCanvas::FindItem( Production * pProduction )
{
	if( curSelection != SELECT_PRODUCTION ) return wxTreeItemId();

	wxTreeItemIdValue cookie;
	wxTreeItemId current = tree->GetFirstChild( tree->GetRootItem(), cookie );
	if( ! current.IsOk() ) return wxTreeItemId();

	wxTreeItemId last = tree->GetLastChild( tree->GetRootItem() );
	bool found = true;

	while ( ( ( ProductionLeaf * )tree->GetItemData( current ) )->production !=
			pProduction )
	{
		if( current == last ) {
			found = false;
			break;
		}
		current = tree->GetNextChild( tree->GetRootItem(), cookie );
	}
	if( found )
		return current;

	return wxTreeItemId();
}

wxTreeItemId SelectionCanvas::FindItem( Unit * pUnit, bool unitByFaction )
{
	if( curSelection != SELECT_UNIT ) return wxTreeItemId();

	bool found = true;
	wxTreeItemIdValue cookie;
	wxTreeItemId unit = tree->GetFirstChild( tree->GetRootItem(), cookie );
	if( ! unit.IsOk() ) return wxTreeItemId();

	wxTreeItemId lastunit = tree->GetLastChild( tree->GetRootItem() );

	while ( ( ( UnitLeaf * )tree->GetItemData( unit ) )->unit !=
			pUnit )
	{
		if( unit == lastunit ) {
			found = false;
			break;
		}
		unit = tree->GetNextChild( tree->GetRootItem(), cookie );
	}
	if( found )
		return unit;

	return wxTreeItemId();
}

void SelectionCanvas::Init()
{
	if( tree ) delete tree;
	lastSelected = wxTreeItemId();

	tree = new wxTreeCtrl( this, Selection_Tree, wxDefaultPosition,
						  GetClientSize(), /*TR_MULTIPLE |*/wxTR_DEFAULT_STYLE 
						   | wxTR_HIDE_ROOT );


	AString temp;

	tree->SetBackgroundColour( GetBackgroundColour() );
	selectedElems->Clear();
	AddItem( app->m_game, wxTreeItemId() );
	Resize();
//	sizerSelect->Add( tree, 1 );
	/*
	wxTreeItemId root = tree->AddRoot( "Atlantis", -1, -1, new GameLeaf( app->m_game ) );
	wxTreeItemId current;

	// factions
	treeFactions = tree->AppendItem( root, _T( "Factions" ) );
	{
		forlist ( &app->m_game->factions ) {
			Faction * f = ( Faction * ) elem;
			f->treeId = tree->AppendItem( treeFactions, f->name->Str(), -1, -1,
							new F/actionLeaf( f ) );
		}
	}

	// levels
	treeLevels = tree->AppendItem( root, _T( "Levels" ) );

	int level;
	for( level = 0; level < app->m_game->regions.numLevels; level++ ) {
		ARegionArray * pArr = app->m_game->regions.pRegionArrays[ level ];
		temp = AString( "(" ) + level + ") ";
		if( pArr->strName )
			temp += *pArr->strName;
//		else
//			temp += "surface";
		wxTreeItemId curLevel = tree->AppendItem( treeLevels, temp.Str(),
												 -1,-1,
												 new LevelLeaf( pArr, level ) );
		// regions
		for( int y = 0; y < pArr->y; y++ ) {
			for( int x = 0; x < pArr->x; x++ ) {
				ARegion * r = pArr->GetRegion( x,y );
				if( !r ) continue;
				temp = AString( "(" ) + r->xloc + ", " + r->yloc + ", "
					+ r->zloc + ") " + *r->name;
				if( r->town ) temp += AString( " [" ) + *r->town->name + "]";
				wxTreeItemId curRegion = tree->AppendItem( curLevel, temp.Str(), -1, -1,
								 new RegionLeaf( r ) );
				//markets
				current = curRegion;
				r->treeId = curRegion;
				if( GuiConfig.showTreeHeaders )
					current = tree->AppendItem( curRegion, _T( "Markets" ) );
				{
					forlist ( &r->markets ) {
						Market * m = ( Market * ) elem;
						if( m->item == -1 )
							continue;
//						//Hack:
//						m->region = r->num;
						temp = AString( ( m->type == M_BUY?"Sells ":"Buys " ) );
						temp += ItemString( m->item, m->amount ) + " at $" + m->price;
						m->treeId = tree->AppendItem( current, _T( temp.Str() ), -1, -1,
										 new MarketLeaf( m ) );
					}
				}

				//products
				if( GuiConfig.showTreeHeaders )
					current = tree->AppendItem( curRegion, _T( "Products" ) );
				{
					forlist ( &r->products ) {
						Production * p = ( Production * ) elem;
//						//Hack:
//						p->region = r->num;
						temp = AString( "Produces ") + ItemString( p->itemtype, p->amount );
						p->treeId = tree->AppendItem( current, _T( temp.Str() ), -1, -1,
										new ProductionLeaf( p ) );
					}
				}
				//objects
				if( GuiConfig.showTreeHeaders )
					current = tree->AppendItem( curRegion, _T( "Objects" ) );
				forlist ( &r->objects ) {
					Object * o = ( Object * ) elem;
					temp = *o->name + " : " + ObjectDefs[o->type].name;
					wxTreeItemId curObject = tree->AppendItem( current, _T( temp.Str() ), -1, -1,
									 new ObjectLeaf( o ) );
					o->treeId = curObject;
					//units
					forlist ( &o->units ) {
						Unit * u = ( Unit * ) elem;
						temp = *u->name;
						u->treeId = tree->AppendItem( curObject, _T( temp.Str() ), -1, -1,
										 new UnitLeaf( u, false ) );
						//add to factions
						wxTreeItemId faction = FindItem( u->faction );
						tree->AppendItem( faction, _T( temp.Str() ), -1, -1, 
										 new UnitLeaf( u, true ) );
					}
				}
			}
		}
	}
	*/
}

void SelectionCanvas::UpdateSelection()
{
	bool treeOld = treeWait;
	treeWait = true;

	if( app->curSelection == SELECT_GAME ) {
		Init();
		AddItem( app->m_game, tree->GetRootItem() );
		curSelection = app->curSelection;
	} else if( app->curSelection == SELECT_LEVEL ) {
		Init();
		AddItem( app->selectedLevel, tree->GetRootItem() );
		selectedLevel = app->selectedLevel;
		curSelection = app->curSelection;
	} else {
		// Clear tree of unneeded elements
		if( app->curSelection != curSelection ) {
			Init();
			curSelection = app->curSelection;
		} else {
			for( int i = (int) selectedElems->GetCount() - 1; i >= 0; i-- ) {
				AListElem * elem = selectedElems->Item( i );

				if( app->selectedElems->Index( elem ) == wxNOT_FOUND ) {
					RemoveElem( elem );
					selectedElems->Remove( elem );
				}
			}
		}
		// Add new elements
		for( int i = 0; i < (int) app->selectedElems->GetCount(); i++ ) {
			AListElem * elem = app->selectedElems->Item( i );

			if( selectedElems->Index( elem ) == wxNOT_FOUND ) {
				AddElem( elem );
				selectedElems->Add( elem );
			}
		}
	}

	tree->Refresh();
	
	wxTreeItemIdValue cookie;
	wxTreeItemId id = tree->GetFirstChild( tree->GetRootItem(), cookie );
	if( selectedElems->Count() == 1 ) {
		tree->Expand( id );
	}
//	} else {
//		while( id.IsOk() ) {
//			tree->Collapse( id );
//			id = tree->GetNextSibling();
//		}
//	}

	// Make sure children are visible on last selection first
//	if( selectedElems->Count() == 1 && tree->GetChildrenCount( lastSelected, false ) ) {
//		tree->Expand( lastSelected );
//	 	tree->ScrollTo( tree->GetLastChild( tree->GetLastChild( treeLevels ) ) );
//	}

	// And finally, display the last selection
//	tree->EnsureVisible( lastSelected );

	/// Update toolbar
	toolBar->EnableTool( Selection_Tool_Back, app->curHistory && app->curHistory->prev );
	toolBar->EnableTool( Selection_Tool_Forward, app->curHistory && app->curHistory->next );
	toolBar->EnableTool( Selection_Tool_Zoom_In, false );
	toolBar->EnableTool( Selection_Tool_Zoom_Out, false );
	treeWait = treeOld;
}


wxTreeItemId SelectionCanvas::FindSelected( AListElem * element, int type, bool unitByFaction )
{
	switch( type ) {
		case SELECT_REGION:
			return ( (ARegion *) element )->treeId;
			break;
		case SELECT_UNIT:
			if( unitByFaction ) {
				return FindItem( (Unit *) element, unitByFaction );
			} else {
				return ( (Unit *) element )->treeId;
			}
			break;
		case SELECT_FACTION:
			return ( (Faction *) element )->treeId;
			break;
		case SELECT_MARKET:
			return ( (Market *) element )->treeId;
			break;
		case SELECT_PRODUCTION:
			return ( (Production *) element )->treeId;
			break;
		case SELECT_OBJECT:
			return ( (Object *) element )->treeId;
			break;
	}

	return wxTreeItemId();
}

void SelectionCanvas::AddElem( AListElem * element ) 
{
	switch( curSelection ) {
		case SELECT_REGION:
			AddItem( (ARegion *) element, tree->GetRootItem() );
			break;
		case SELECT_UNIT:
			AddItem( (Unit *) element, tree->GetRootItem() );
			break;
		case SELECT_FACTION:
			AddItem( (Faction *) element, tree->GetRootItem() );
			break;
		case SELECT_MARKET:
			AddItem( (Market *) element, tree->GetRootItem() );
			break;
		case SELECT_PRODUCTION:
			AddItem( (Production *) element, tree->GetRootItem() );
			break;
		case SELECT_OBJECT:
			AddItem( (Object *) element, tree->GetRootItem() );
			break;
	}
}

void SelectionCanvas::RemoveElem( AListElem * element ) 
{
	switch( curSelection ) {
		case SELECT_REGION:
			RemoveItem( (ARegion *) element );
			break;
		case SELECT_UNIT:
			RemoveItem( (Unit *) element );
			break;
		case SELECT_FACTION:
			RemoveItem( (Faction *) element );
			break;
		case SELECT_MARKET:
			RemoveItem( (Market *) element );
			break;
		case SELECT_PRODUCTION:
			RemoveItem( (Production *) element );
			break;
		case SELECT_OBJECT:
			RemoveItem( (Object *) element );
			break;
	}
}

void SelectionCanvas::OnBack( wxCommandEvent & event )
{
	app->BackHistory();
}

void SelectionCanvas::OnForward( wxCommandEvent & event )
{
	app->ForwardHistory();
}

void SelectionCanvas::OnZoomOut( wxCommandEvent & event )
{
	TreeLeaf * t = ( TreeLeaf * ) tree->GetItemData( tree->GetSelection() );
	if( !t ) return;

	switch( t->class_id ) {
		case LEAF_GAM:
			return;
		case LEAF_FAC:
		case LEAF_LEV:
			app->Select( app->m_game );
			break;
		case LEAF_REG:
			app->Select( app->m_game->regions.GetRegionArray( ( ( RegionLeaf * ) t )->region->zloc ) );
			break;
		case LEAF_OBJ:
			app->Select( ( ( ObjectLeaf * ) t )->object->region );
			break;
		case LEAF_UNI:
			app->Select( ( ( UnitLeaf * ) t )->unit->object );
			break;
		case LEAF_MAR:
			app->Select( app->m_game->regions.GetRegion( ( ( MarketLeaf * ) t )->market->region ) );
			break;
		case LEAF_PRO:
			app->Select( app->m_game->regions.GetRegion( ( ( ProductionLeaf * ) t )->production->region ) );
			break;
	}
	app->UpdateSelection();
}

void SelectionCanvas::OnZoomIn( wxTreeEvent & event )
{
	ZoomIn();
}

void SelectionCanvas::OnZoomIn( wxCommandEvent & event )
{
	ZoomIn();
}

void SelectionCanvas::ZoomIn()
{
	TreeLeaf * t = ( TreeLeaf * ) tree->GetItemData( tree->GetSelection() );
	if( !t ) return;

	switch( t->class_id ) {
		case LEAF_GAM:
			app->Select( app->m_game );
			break;
		case LEAF_FAC:
			app->Select( ( ( FactionLeaf * ) t )->faction );
			break;
		case LEAF_LEV:
			app->Select( ( ( LevelLeaf * ) t )->regionArray );
			break;
		case LEAF_REG:
			app->Select( ( ( RegionLeaf * ) t )->region );
			break;
		case LEAF_OBJ:
			app->Select( ( ( ObjectLeaf * ) t )->object );
			break;
		case LEAF_UNI:
			app->Select( ( ( UnitLeaf * ) t )->unit );
			break;
		case LEAF_MAR:
			app->Select( ( ( MarketLeaf * ) t )->market );
			break;
		case LEAF_PRO:
			app->Select( ( ( ProductionLeaf * ) t )->production );
			break;
	}
	app->UpdateSelection();
}


// ---------------------------------------------------------------------------
// TreeLeaf
// ---------------------------------------------------------------------------

TreeLeaf::~TreeLeaf()
{
}

FactionLeaf::FactionLeaf( Faction * pFaction ) : TreeLeaf()
{
	faction = pFaction;
	class_id = LEAF_FAC;
	
}

LevelLeaf::LevelLeaf( ARegionArray * pAArray, int l ) : TreeLeaf()
{
	regionArray = pAArray;
	level = l;
	class_id = LEAF_LEV;
}

RegionLeaf::RegionLeaf( ARegion * pRegion ) : TreeLeaf()
{
	region = pRegion;
	class_id = LEAF_REG;
}

MarketLeaf::MarketLeaf( Market * pMarket ) : TreeLeaf()
{
	market = pMarket;
	class_id = LEAF_MAR;
}

ProductionLeaf::ProductionLeaf( Production * pProduction ) : TreeLeaf()
{
	production = pProduction;
	class_id = LEAF_PRO;
}

ObjectLeaf::ObjectLeaf( Object * pObject ) : TreeLeaf()
{
	object = pObject;
	class_id = LEAF_OBJ;
}

UnitLeaf::UnitLeaf( Unit * pUnit, bool byFac ) : TreeLeaf()
{
	unit = pUnit;
	byFaction = byFac;
	class_id = LEAF_UNI;
}

GameLeaf::GameLeaf( Game * pGame ) : TreeLeaf()
{
	game = pGame;
	class_id = LEAF_GAM;
}
