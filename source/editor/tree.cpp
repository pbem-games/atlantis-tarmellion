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

#include "tree.h"
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

#include "bitmaps/tree_single.xpm"
#include "bitmaps/tree_multi.xpm"

BEGIN_EVENT_TABLE( TreeCanvas, wxScrolledWindow )
	EVT_SIZE( TreeCanvas::OnResize )
	EVT_TREE_SEL_CHANGED( Tree_Tree, TreeCanvas::OnLeafSelection )
	EVT_TREE_SEL_CHANGING( Tree_Tree, TreeCanvas::OnLeafSelecting )
END_EVENT_TABLE()

// ---------------------------------------------------------------------------
// TreeCanvas
// ---------------------------------------------------------------------------

TreeCanvas::TreeCanvas( wxWindow *parent )
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

}

TreeCanvas::~TreeCanvas()
{
	delete selectedElems;
	delete tree;
}

void TreeCanvas::OnResize( wxSizeEvent & event )
{
	if( tree ) {
		int w, h;
		GetClientSize( &w,&h );
		tree->SetSize( w,h );
	}
}

void TreeCanvas::OnLeafSelection( wxTreeEvent & event )
{
	wxTreeItemId id = event.GetItem();
	if( !id.IsOk() ) return; // this may happen a few times with every leaf selection for some reason..

	TreeLeaf * t = ( TreeLeaf * ) tree->GetItemData( id );
	if( t )
		t->MakeSelection();
}

void TreeCanvas::OnLeafSelecting( wxTreeEvent & event )
{
	if( treeWait ) {
		event.Veto();
		event.Skip();
		return;
	}
	treeWait = true;

	TreeLeaf * t = ( TreeLeaf * ) tree->GetItemData( event.GetItem() );
	if( t )
		t->MakeSelection();

	app->UpdateSelection();

	event.Veto();
	event.Skip();
	treeWait = false;
}

void TreeCanvas::DeselectAll()
{
	return;
	UnHighlightLeaf( lastSelected );
	lastSelected = wxTreeItemId();
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

void TreeCanvas::UnHighlightLeaf( wxTreeItemId id )
{
	if( id.IsOk() ) {
		tree->SetItemTextColour( id, wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ) );
		tree->SetItemBackgroundColour( id, GetBackgroundColour()/*wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE )*/ );
	}
}

void TreeCanvas::HighlightLeaf( wxTreeItemId id )
{
	if( id.IsOk() ) {
		tree->SetItemTextColour( id, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
		tree->SetItemBackgroundColour( id, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
		tree->SelectItem( wxTreeItemId() );
	}
}

bool TreeCanvas::IsLeafHighlighted( wxTreeItemId id )
{
	if( tree->GetItemBackgroundColour( id ) == wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) )
		return true;

	return false;
}

void TreeCanvas::AddItem( Faction * pFaction, bool populate )
{
	wxTreeItemId leaf = tree->AppendItem( treeFactions, pFaction->name->Str(), -1, -1,
					 new FactionLeaf( pFaction ) );
	pFaction->treeId = leaf;

	if( populate ) {
		forlist( &app->m_game->regions ) {
			ARegion * r = ( ARegion * ) elem;
			forlist ( &r->objects ) {
				Object * o = ( Object * ) elem;
				forlist ( &o->units ) {
					Unit * u = ( Unit * ) elem;
					if( u->faction == pFaction ) {
						AString temp = *u->name;
						tree->AppendItem( leaf, temp.Str(), -1, -1, 
									 new UnitLeaf( u, true ) );
					}
				}
			}
		}
	}
}

void TreeCanvas::AddItem( ARegion * pRegion, bool populate )
{
	wxTreeItemId level = FindItem( app->m_game->regions.pRegionArrays[pRegion->zloc] );

	AString temp = AString( "(" ) + pRegion->xloc + ", " +
		pRegion->yloc + ", " + pRegion->zloc + ") " + *pRegion->name;

	if( pRegion->town )
		temp += AString( " [" ) + *pRegion->town->name + "]";

	wxTreeItemId leaf = tree->AppendItem( level, temp.Str(), -1, -1, new RegionLeaf( pRegion ) );

	wxTreeItemId current = leaf;
	pRegion->treeId = leaf;

	if( populate ) {
		if( GuiConfig.showTreeHeaders ) current = tree->AppendItem( leaf, "Markets" );
		{
			forlist ( &pRegion->markets ) {
				Market * m = ( Market * ) elem;
				temp = AString( ( m->type == M_BUY?"Sells ":"Buys " ) );
				temp += ItemString( m->item, m->amount ) + " at $" + m->price;
				m->treeId = tree->AppendItem( current, temp.Str(), -1, -1,
								 new MarketLeaf( m ) );
			}
		}
		//products
		if( GuiConfig.showTreeHeaders ) current = tree->AppendItem( leaf, "Products" );
		{
			forlist ( &pRegion->products ) {
				Production * p = ( Production * ) elem;
				temp = AString( "Produces ") + ItemString( p->itemtype, p->amount );
				p->treeId = tree->AppendItem( current, _T( temp.Str() ), -1, -1,
								new ProductionLeaf( p ) );
			}
		}
		//objects
		if( GuiConfig.showTreeHeaders ) current = tree->AppendItem( leaf, "Objects" );
		forlist ( &pRegion->objects ) {
			Object * o = ( Object * ) elem;
			temp = *o->name + " : " + ObjectDefs[o->type].name;
			wxTreeItemId curObject = tree->AppendItem( current, temp.Str(), -1, -1,
							 new ObjectLeaf( o ) );
			o->treeId = curObject;
			//units
			forlist ( &o->units ) {
				Unit * u = ( Unit * ) elem;
				temp = *u->name;
				u->treeId = tree->AppendItem( curObject, temp.Str(), -1, -1,
								 new UnitLeaf( u, false ) );
			}
		}
	}
}

void TreeCanvas::AddItem( Market * pMarket )
{
	ARegion * r = app->m_game->regions.GetRegion( pMarket->region );
//	wxTreeItemId region = FindItem( r );
	wxTreeItemId region = r->treeId;

	wxTreeItemId category = region;

	if( GuiConfig.showTreeHeaders )
		category = FindCategory( "Markets", region );

	if( category.IsOk() ) {
		AString temp = AString( ( pMarket->type == M_BUY?"Sells ":"Buys " ) );
		temp += ItemString( pMarket->item, pMarket->amount ) + " at $" +
				pMarket->price;
		pMarket->treeId = tree->AppendItem( category, temp.Str(), -1, -1,
					 new MarketLeaf( pMarket ) );
	}

}

void TreeCanvas::AddItem( Production * pProduction )
{
	ARegion * r = app->m_game->regions.GetRegion( pProduction->region );
//	wxTreeItemId region = FindItem( r );
	wxTreeItemId region = r->treeId;

	wxTreeItemId category = region;

	if( GuiConfig.showTreeHeaders )
		category = FindCategory( "Products", region );

	if( category.IsOk() ) {
		AString temp = AString( "Produces ") + ItemString( pProduction->itemtype, pProduction->amount );
		pProduction->treeId = tree->AppendItem( category, _T( temp.Str() ), -1, -1,
						new ProductionLeaf( pProduction ) );
	}

}

void TreeCanvas::AddItem( Object * pObject, bool populate )
{
//	wxTreeItemId region = FindItem( pObject->region );
	wxTreeItemId region = pObject->region->treeId;
	if( region ) {
		wxTreeItemId category = region;

		if( GuiConfig.showTreeHeaders ) {
			category = FindCategory( "Objects", region );
		}


		if( category.IsOk() ) {
			AString temp = *pObject->name + " : " + ObjectDefs[pObject->type].name;
			wxTreeItemId leaf = tree->AppendItem( category, _T( temp.Str() ), -1, -1,
							 new ObjectLeaf( pObject ) );
			pObject->treeId = leaf;
			if( populate ) {
				forlist ( &pObject->units ) {
					Unit * u = ( Unit * ) elem;
					temp = *u->name;
					u->treeId = tree->AppendItem( leaf, temp.Str(), -1, -1,
									 new UnitLeaf( u, false ) );
				}
			}
		}
	}

}


void TreeCanvas::RemoveItem( Faction * pFaction )
{
//	wxTreeItemId id = FindItem( pFaction );
	tree->Delete( pFaction->treeId );
}

void TreeCanvas::RemoveItem( ARegion * pRegion )
{
//	wxTreeItemId id = FindItem( pRegion );
	tree->Delete( pRegion->treeId );
}

void TreeCanvas::RemoveItem( Market * pMarket )
{
//	wxTreeItemId id = FindItem( pMarket );
	tree->Delete( pMarket->treeId );
}

void TreeCanvas::RemoveItem( Production * pProduction )
{
//	wxTreeItemId id = FindItem( pProduction );
	tree->Delete( pProduction->treeId );
}

void TreeCanvas::RemoveItem( Object * pObject )
{
//	wxTreeItemId id = FindItem( pObject );
	tree->Delete( pObject->treeId );
}

void TreeCanvas::RemoveItem( Unit * pUnit )
{
	wxTreeItemId id = FindItem( pUnit, true );
	tree->Delete( id );
//	id = FindItem( pUnit, false );
	tree->Delete( pUnit->treeId );
}

void TreeCanvas::UpdateItem( Faction * pFaction)
{
//	wxTreeItemId id = FindItem( pFaction );
	tree->SetItemText( pFaction->treeId, pFaction->name->Str() );
}

void TreeCanvas::UpdateItem( ARegionArray * pAArray)
{
	wxTreeItemId id = FindItem( pAArray );
	int num = ( (LevelLeaf *) tree->GetItemData( id ) )->level;
	AString temp = AString( "(" ) + num + ") ";
	if( pAArray->strName ) temp += *pAArray->strName; 
	tree->SetItemText( id, temp.Str() );
}

void TreeCanvas::UpdateItem( ARegion * pRegion, bool populate )
{
//	wxTreeItemId id = FindItem( pRegion );
	wxTreeItemId id = pRegion->treeId;
	AString temp = AString( "(" ) + pRegion->xloc + ", " +
	                        pRegion->yloc + ", "+ pRegion->zloc +
							") " + *pRegion->name;
	if( pRegion->town ) temp += AString( " [" ) + *pRegion->town->name + "]";
	tree->SetItemText( id, temp.Str() );

	if( populate ) {
		long cookie;
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
}

void TreeCanvas::UpdateItem( Market * pMarket)
{
//	wxTreeItemId id = FindItem( pMarket );
	AString temp = AString( ( pMarket->type == M_BUY?"Sells ":"Buys " ) );
	temp += ItemString( pMarket->item, pMarket->amount ) +
	                    " at $" + pMarket->price;
	tree->SetItemText( pMarket->treeId, temp.Str() );
}

void TreeCanvas::UpdateItem( Production * pProduction)
{
//	wxTreeItemId id = FindItem( pProduction );
	AString temp = ItemString( pProduction->itemtype, pProduction->amount );
	tree->SetItemText( pProduction->treeId, temp.Str() );
}

void TreeCanvas::UpdateItem( Object * pObject)
{
//	wxTreeItemId id = FindItem( pObject );
	AString	temp = *pObject->name + " : " + ObjectDefs[pObject->type].name;
	tree->SetItemText( pObject->treeId, temp.Str() );
}

void TreeCanvas::UpdateItem( Unit * pUnit)
{
	wxTreeItemId id1 = FindItem( pUnit, true );

	bool found = false;

	if( !id1.IsOk() ) {
		//unit may have changed faction, so look thru all factions
		long cookie1;
		wxTreeItemId faction = tree->GetFirstChild( treeFactions, cookie1 );
		wxTreeItemId lastfaction = tree->GetLastChild( treeFactions );
		while( faction.IsOk() && ( ( FactionLeaf * )tree->GetItemData( faction ) )->faction !=
		                 pUnit->faction ) {
			found = true;
			long cookie2;

			wxTreeItemId unit = tree->GetFirstChild( faction, cookie2 );
			wxTreeItemId lastunit = tree->GetLastChild( faction );
			while ( unit.IsOk() && ( ( UnitLeaf * )tree->GetItemData( unit ) )->unit !=
				    pUnit )
			{
				if( unit == lastunit ) {
					found = false;
					break;
				}
				unit = tree->GetNextChild( faction, cookie2 );
			}
			if( found ) {
				id1 = unit;
				break;
			}

			if( faction == lastfaction ) break;

			faction = tree->GetNextChild( treeFactions, cookie1 );
		}
	}

	if( found ) {
		// unit is under the wrong faction
		tree->Delete(id1);
//		wxTreeItemId faction = FindItem( pUnit->faction );
		wxTreeItemId faction = pUnit->faction->treeId;
		id1 = tree->AppendItem( faction, "", -1, -1, 
						  new UnitLeaf( pUnit, true ) );
	}


	wxTreeItemId id2 = pUnit->treeId;
//	wxTreeItemId id2 = FindItem( pUnit, false );
	AString temp = *pUnit->name;
	if( id1.IsOk() ) {
		wxString t = tree->GetItemText(id1);
		tree->SetItemText( id1, temp.Str() );
		tree->SetItemText( id2, temp.Str() );
	}
}


void TreeCanvas::AddItem( Unit * pUnit )
{
	AString temp = *pUnit->name;

//	wxTreeItemId object = FindItem( pUnit->object );
	pUnit->treeId = tree->AppendItem( pUnit->object->treeId, temp.Str(), -1, -1,
					 new UnitLeaf( pUnit, false ) );

//	wxTreeItemId faction = FindItem( pUnit->faction );
	tree->AppendItem( pUnit->faction->treeId, temp.Str(), -1, -1, 
					 new UnitLeaf( pUnit, true ) );

}

wxTreeItemId TreeCanvas::FindItem( ARegionArray * pArr )
{
	long cookie;
	wxTreeItemId level = tree->GetFirstChild( treeLevels, cookie );
	if( ! level.IsOk() ) return wxTreeItemId();
	wxTreeItemId lastlevel = tree->GetLastChild( treeLevels );
	if( level ) {
		bool found = true;
		while ( ( ( LevelLeaf * )tree->GetItemData( level ) )->regionArray !=
				 pArr )
		{
			if( level == lastlevel ) {
				found = false;
				break;
			}
			level = tree->GetNextChild( treeLevels, cookie );
		}
		if( found )
			return level;
	}
	return wxTreeItemId();
}


wxTreeItemId TreeCanvas::FindItem( ARegion * pRegion )
{
	wxTreeItemId level = FindItem( app->m_game->regions.pRegionArrays[pRegion->zloc] );
	long cookie;
	bool found = true;
	if( level ) {
		wxTreeItemId region = tree->GetFirstChild( level, cookie );
		if( ! region.IsOk() ) return wxTreeItemId();
		wxTreeItemId lastregion = tree->GetLastChild( level );
		while ( ( ( RegionLeaf * )tree->GetItemData( region ) )->region !=
			pRegion )
		{
			if( region == lastregion ) {
				found = false;
				break;
			}
			region = tree->GetNextChild( level, cookie );
		}
		if( found )
			return region;
	}
	return wxTreeItemId();
}

wxTreeItemId TreeCanvas::FindItem( Faction * pFaction )
{
	long cookie;
	wxTreeItemId faction = tree->GetFirstChild( treeFactions, cookie );
	if( ! faction.IsOk() ) return wxTreeItemId();
	wxTreeItemId lastfaction = tree->GetLastChild( treeFactions );
	if( faction ) {
		bool found = true;
		while ( ( ( FactionLeaf * )tree->GetItemData( faction ) )->faction !=
			   pFaction )
		{
			if( faction == lastfaction ) {
				found = false;
				break;
			}
			faction = tree->GetNextChild( treeFactions, cookie );
		}
		if( found )
			return faction;
	}
	return wxTreeItemId();
}

wxTreeItemId TreeCanvas::FindItem( Object * pObject )
{
	wxTreeItemId region = FindItem( pObject->region );
	if( region ) {
		long cookie;
		wxTreeItemId category = region;

		if( GuiConfig.showTreeHeaders ) {
			category = FindCategory( "Objects", region );
		}

		if( category.IsOk() ) {
			wxTreeItemId current = tree->GetFirstChild( category, cookie );
			if( !current.IsOk() ) return wxTreeItemId();

			wxTreeItemId last = tree->GetLastChild( category );

			bool found = true;
			while ( ( ( ObjectLeaf * )tree->GetItemData( current ) )->object !=
				pObject )
			{
				if( current == last ) {
					found = false;
					break;
				}
				current = tree->GetNextChild( category, cookie );
			}
			if( found )
				return current;
		}
	}
	return wxTreeItemId();
}

wxTreeItemId TreeCanvas::FindItem( Market * pMarket )
{
	ARegion * r = app->m_game->regions.GetRegion( pMarket->region );
	wxTreeItemId region = FindItem( r );

	long cookie;
	wxTreeItemId category = region;

	if( GuiConfig.showTreeHeaders ) {
		category = FindCategory( "Markets", region );
	}

	if( category.IsOk() ) {
		wxTreeItemId current = tree->GetFirstChild( category, cookie );
		if( ! current.IsOk() ) return wxTreeItemId();
		wxTreeItemId last = tree->GetLastChild( category );

		bool found = true;
		while ( ( ( MarketLeaf * )tree->GetItemData( current ) )->market !=
			pMarket )
		{
			if( current == last ) {
				found = false;
				break;
			}
			current = tree->GetNextChild( category, cookie );
		}
		if( found )
			return current;
	}
	return wxTreeItemId();
}

wxTreeItemId TreeCanvas::FindItem( Production * pProduction )
{
	ARegion * r = app->m_game->regions.GetRegion( pProduction->region );
	wxTreeItemId region = FindItem( r );

	long cookie;
	wxTreeItemId category = region;

	if( GuiConfig.showTreeHeaders ) {
		category = FindCategory( "Products", region );
	}

	if( category.IsOk() ) {
		wxTreeItemId current = tree->GetFirstChild( category, cookie );
		if( ! current.IsOk() ) return wxTreeItemId();
		wxTreeItemId last = tree->GetLastChild( category );

		bool found = true;
		while ( ( ( ProductionLeaf * )tree->GetItemData( current ) )->production !=
			pProduction )
		{
			if( current == last ) {
				found = false;
				break;
			}
			current = tree->GetNextChild( category, cookie );
		}
		if( found )
			return current;
	}
	return wxTreeItemId();
}

wxTreeItemId TreeCanvas::FindItem( Unit * pUnit, bool unitByFaction )
{
	wxTreeItemId branch;

	if( unitByFaction )
		branch = FindItem( pUnit->faction );
	else
		branch = FindItem( pUnit->object );
		
	if( branch ) {
		bool found = true;
		long cookie;

		wxTreeItemId unit = tree->GetFirstChild( branch, cookie );
		if( ! unit.IsOk() ) return wxTreeItemId();
		wxTreeItemId lastunit = tree->GetLastChild( branch );
		while ( ( ( UnitLeaf * )tree->GetItemData( unit ) )->unit !=
			pUnit )
		{
			if( unit == lastunit ) {
				found = false;
				break;
			}
			unit = tree->GetNextChild( branch, cookie );
		}
		if( found )
			return unit;
	}

	return wxTreeItemId();
}

wxTreeItemId TreeCanvas::FindCategory( const char * str, wxTreeItemId top )
{
	long cookie;
	wxTreeItemId current;
	wxTreeItemId last;

	current = tree->GetFirstChild( top, cookie );
	last = tree->GetLastChild( top );
	while ( tree->GetItemText( current ) != str )
	{
		if( current == last )
			return wxTreeItemId();

		current = tree->GetNextChild( top, cookie );
	}
	return current;

}

void TreeCanvas::Init()
{
	if( tree ) delete tree;
	lastSelected = wxTreeItemId();

	tree = new wxTreeCtrl( this, Tree_Tree, wxDefaultPosition,
						  GetClientSize(), /*TR_MULTIPLE |*/wxTR_DEFAULT_STYLE );

	AString temp;

	tree->SetBackgroundColour( GetBackgroundColour() );
	wxTreeItemId root = tree->AddRoot( "Atlantis", -1, -1, new GameLeaf( app->m_game ) );
	wxTreeItemId current;

	// factions
	treeFactions = tree->AppendItem( root, _T( "Factions" ) );
	{
		forlist ( &app->m_game->factions ) {
			Faction * f = ( Faction * ) elem;
			f->treeId = tree->AppendItem( treeFactions, f->name->Str(), -1, -1,
							new FactionLeaf( f ) );
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
}

// ---------------------------------------------------------------------------
// TreeLeaf
// ---------------------------------------------------------------------------

TreeLeaf::~TreeLeaf()
{
}

void TreeLeaf::MakeSelection( bool add )
{
}

FactionLeaf::FactionLeaf( Faction * pFaction ) : TreeLeaf()
{
	faction = pFaction;
	class_id = LEAF_FAC;
	
}

void FactionLeaf::MakeSelection( bool add )
{
	app->Select( faction, add );
}

LevelLeaf::LevelLeaf( ARegionArray * pAArray, int l ) : TreeLeaf()
{
	regionArray = pAArray;
	level = l;
	class_id = LEAF_LEV;
}

void LevelLeaf::MakeSelection( bool add )
{
	app->Select( regionArray, add );
}

RegionLeaf::RegionLeaf( ARegion * pRegion ) : TreeLeaf()
{
	region = pRegion;
	class_id = LEAF_REG;
}

void RegionLeaf::MakeSelection( bool add )
{
	app->Select( region, add );
}

MarketLeaf::MarketLeaf( Market * pMarket ) : TreeLeaf()
{
	market = pMarket;
	class_id = LEAF_MAR;
}

void MarketLeaf::MakeSelection( bool add )
{
	app->Select( market,add );
}

ProductionLeaf::ProductionLeaf( Production * pProduction ) : TreeLeaf()
{
	production = pProduction;
	class_id = LEAF_PRO;
}

void ProductionLeaf::MakeSelection( bool add )
{
	app->Select( production, add );
}

ObjectLeaf::ObjectLeaf( Object * pObject ) : TreeLeaf()
{
	object = pObject;
	class_id = LEAF_OBJ;
}

void ObjectLeaf::MakeSelection( bool add )
{
	app->Select( object, add );
}

UnitLeaf::UnitLeaf( Unit * pUnit, bool byFac ) : TreeLeaf()
{
	unit = pUnit;
	byFaction = byFac;
	class_id = LEAF_UNI;
}

void UnitLeaf::MakeSelection( bool add )
{
	frame->tree->lastUnitByFaction = byFaction;
	app->Select( unit, add );
}

GameLeaf::GameLeaf( Game * pGame ) : TreeLeaf()
{
	game = pGame;
	class_id = LEAF_GAM;
}

void GameLeaf::MakeSelection( bool add )
{
	app->Select( game, add );
}

void TreeCanvas::UpdateSelection()
{
	int i;

	bool treeOld = treeWait;
	treeWait = true;

//	tree->UnselectAll();

	if( curSelection == SELECT_LEVEL ) {
		// Change highlighted level
		if( app->curSelection != SELECT_LEVEL || app->selectedLevel != selectedLevel ) { 
			UnHighlightLeaf( FindItem( selectedLevel ) );
		}
	}

	if( curSelection == SELECT_GAME && app->curSelection != SELECT_GAME ) {
		UnHighlightLeaf( tree->GetRootItem() );
	}

	if( app->curSelection == SELECT_LEVEL ) {
		HighlightLeaf( FindItem( app->selectedLevel ) );
	} else if( app->curSelection == SELECT_GAME ) {
		HighlightLeaf( tree->GetRootItem() );
	} else {
		for( i = 0; i < (int) selectedElems->GetCount(); i++ ) {
			AListElem * elem = selectedElems->Item( i );
			if( curSelection == SELECT_UNIT ) {
				lastSelected = FindSelected( elem, SELECT_UNIT, lastUnitByFaction );
				UnHighlightLeaf( lastSelected );
				lastSelected = FindSelected( elem, SELECT_UNIT, !lastUnitByFaction );
				UnHighlightLeaf( lastSelected );
			} else {		
				lastSelected = elem->treeId;
				UnHighlightLeaf( lastSelected );
				tree->Collapse( lastSelected );
			}
		}
		for( i = 0; i < (int) app->selectedElems->GetCount(); i++ ) {
			AListElem * elem = app->selectedElems->Item( i );
			if( curSelection == SELECT_UNIT ) {
				lastSelected = FindSelected( elem, SELECT_UNIT, !lastUnitByFaction );
				HighlightLeaf( lastSelected );
				lastSelected = FindSelected( elem, SELECT_UNIT, lastUnitByFaction );
				HighlightLeaf( lastSelected );
			} else {		
				lastSelected = elem->treeId;
				HighlightLeaf( lastSelected );
			}
		}

	}

	tree->Refresh();

	// Synch selection values
	curSelection = app->curSelection;
	selectedLevel = app->selectedLevel;
	selectedElems->Clear();
	for( i = 0; i < (int) app->selectedElems->GetCount(); i++ ) {
		AListElem * elem = app->selectedElems->Item( i );
		selectedElems->Add( elem );
	}

	// Make sure children are visible on last selection first
	if( selectedElems->Count() == 1 && tree->GetChildrenCount( lastSelected, false ) ) {
		tree->Expand( lastSelected );
	 	tree->ScrollTo( tree->GetLastChild( tree->GetLastChild( treeLevels ) ) );
	}

	// And finally, display the last selection
	tree->EnsureVisible( lastSelected );

	treeWait = treeOld;
}


wxTreeItemId TreeCanvas::FindSelected( AListElem * element, int type, bool unitByFaction )
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

