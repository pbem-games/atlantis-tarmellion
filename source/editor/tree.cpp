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

#include "tree.h"
#include "gui.h"

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

BEGIN_EVENT_TABLE( TreeChild, wxMDIChildFrame )
	EVT_CLOSE( TreeChild::OnClose )
	EVT_MENU( Tree_Multi_Select, TreeChild::OnMultiToggle )
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( TreeCanvas, wxScrolledWindow )
	EVT_SIZE( TreeCanvas::OnResize )
	EVT_TREE_SEL_CHANGED( Tree_Tree, TreeCanvas::OnLeafSelection )
END_EVENT_TABLE()


// ---------------------------------------------------------------------------
// TreeChild
// ---------------------------------------------------------------------------

TreeChild::TreeChild( wxMDIParentFrame *parent, const wxString& title,
				 const wxPoint& pos, const wxSize& size,
				 const long style )
	   : wxMDIChildFrame( parent, -1, title, pos, size,
						 style )
{
	SetIcon( wxIcon( tree_xpm ) );
/*	CreateToolBar( wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL );

	GetToolBar()->AddCheckTool( Tree_Multi_Select, "",
		                        wxBitmap( tree_multi_xpm ),
								wxBitmap( tree_single_xpm ),
	                            "Multiple item selection (slow!)");
	GetToolBar()->EnableTool( Tree_Multi_Select, false );
	GetToolBar()->Realize();
*/
	canvas = NULL;
}

void TreeChild::InitTree()
{
	if( canvas )
		delete canvas;

	wxSize size = GetClientSize();
	canvas = new TreeCanvas( this, wxPoint(0,0), size );
	canvas->Init();
//	GetToolBar()->EnableTool( Tree_Multi_Select, true );
}

void TreeChild::OnClose( wxCloseEvent & event )
{
}

void TreeChild::OnMultiToggle( wxCommandEvent & event )
{
    long style = canvas->tree->GetWindowStyle() ^ wxTR_MULTIPLE;

	canvas->tree->SetWindowStyleFlag( style );
	GetToolBar()->ToggleTool( Tree_Multi_Select,(style & wxTR_MULTIPLE) != 0 );
	canvas->tree->UnselectAll();
	
}

// ---------------------------------------------------------------------------
// TreeCanvas
// ---------------------------------------------------------------------------

TreeCanvas::TreeCanvas( wxWindow *parent, const wxPoint& pos,
				   const wxSize& size )
		 : wxScrolledWindow( parent, -1, pos, size, wxVSCROLL | wxHSCROLL )
{
	tree = new wxTreeCtrl( this, Tree_Tree, pos,
						  size, /*TR_MULTIPLE |*/wxTR_DEFAULT_STYLE );
	treeWait = false;
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
	if( treeWait ) return;
	treeWait = true;
	TreeLeaf * t = ( TreeLeaf * ) tree->GetItemData( event.GetItem() );
	if( t )
		t->UpdateEditor();
	treeWait = false;
}

void TreeCanvas::SelectItem( ARegion * pRegion, bool add )
{
	if( treeWait ) return;
	treeWait = true;

    if( !add && tree->GetWindowStyle() & wxTR_MULTIPLE)
		tree->UnselectAll();

	tree->SelectItem( FindItem( pRegion ) );

	treeWait = false;
}

void TreeCanvas::SelectItem( Market * pMarket, bool add )
{
	if( treeWait ) return;
	treeWait = true;

    if( !add && tree->GetWindowStyle() & wxTR_MULTIPLE)
	tree->UnselectAll();

	tree->SelectItem( FindItem( pMarket ) );
	treeWait = false;
}

void TreeCanvas::SelectItem( Production * pProduction, bool add )
{
	if( treeWait ) return;
	treeWait = true;

    if( !add && tree->GetWindowStyle() & wxTR_MULTIPLE)
	tree->UnselectAll();

	tree->SelectItem( FindItem( pProduction ) );

	treeWait = false;
}

void TreeCanvas::SelectItem( Unit * pUnit, bool add )
{
	if( treeWait ) return;
	treeWait = true;

    if( !add && tree->GetWindowStyle() & wxTR_MULTIPLE)
	tree->UnselectAll();

	tree->SelectItem( FindItem( pUnit ) );
	treeWait = false;
}

void TreeCanvas::SelectItem( Faction * pFaction, bool add )
{
	if( treeWait ) return;
	treeWait = true;

    if( !add && tree->GetWindowStyle() & wxTR_MULTIPLE)
	tree->UnselectAll();

	tree->SelectItem( FindItem( pFaction ) );
	treeWait = false;
}

void TreeCanvas::SelectItem( Object * pObject, bool add )
{
	if( treeWait ) return;
	treeWait = true;

    if( !add && tree->GetWindowStyle() & wxTR_MULTIPLE)
	tree->UnselectAll();

	tree->SelectItem( FindItem( pObject ) );
	treeWait = false;
}


void TreeCanvas::AddItem( Faction * pFaction, bool populate )
{
	wxTreeItemId leaf = tree->AppendItem( treeFactions, pFaction->name->Str(), -1, -1,
					 new FactionLeaf( pFaction ) );

	if( populate ) {
		forlist( app->GetRegions() ) {
			ARegion * r = ( ARegion * ) elem;
			forlist ( &r->objects ) {
				Object * o = ( Object * ) elem;
				forlist ( &o->units ) {
					Unit * u = ( Unit * ) elem;
					if( u->faction == pFaction ) {
						AString temp = *u->name;
						tree->AppendItem( leaf, temp.Str(), -1, -1, 
									 new UnitLeaf( u ) );
					}
				}
			}
		}
	}

}

void TreeCanvas::AddItem( ARegion * pRegion, bool populate )
{
	wxTreeItemId level = FindItem( app->GetRegions()->pRegionArrays[pRegion->zloc] );

	AString temp = AString( "(" ) + pRegion->xloc + ", " +
		pRegion->yloc + ", " + pRegion->zloc + ") " + *pRegion->name;

	if( pRegion->town )
		temp += AString( " [" ) + *pRegion->town->name + "]";

	wxTreeItemId leaf = tree->AppendItem( level, temp.Str(), -1, -1, new RegionLeaf( pRegion ) );

	if( populate ) {
		wxTreeItemId treeMarkets = tree->AppendItem( leaf, "Markets" );
		{
			forlist ( &pRegion->markets ) {
				Market * m = ( Market * ) elem;
				temp = AString( ( m->type == M_BUY?"Sells ":"Buys " ) );
				temp += ItemString( m->item, m->amount ) + " at $" + m->price;
				tree->AppendItem( treeMarkets, temp.Str(), -1, -1,
								 new MarketLeaf( m ) );
			}
		}
		//products
		wxTreeItemId treeProducts = tree->AppendItem( leaf, "Products" );
		{
			forlist ( &pRegion->products ) {
				Production * p = ( Production * ) elem;
				temp = ItemString( p->itemtype, p->amount );
				tree->AppendItem( treeProducts, temp.Str(), -1, -1,
								new ProductionLeaf( p ) );
			}
		}
		//objects
		wxTreeItemId treeObjects = tree->AppendItem( leaf, "Objects" );
		forlist ( &pRegion->objects ) {
			Object * o = ( Object * ) elem;
			temp = *o->name + " : " + ObjectDefs[o->type].name;
			wxTreeItemId curObject = tree->AppendItem( treeObjects, temp.Str(), -1, -1,
							 new ObjectLeaf( o ) );
			//units
			forlist ( &o->units ) {
				Unit * u = ( Unit * ) elem;
				temp = *u->name;
				tree->AppendItem( curObject, temp.Str(), -1, -1,
								 new UnitLeaf( u ) );
			}
		}
	}
}

void TreeCanvas::AddItem( Market * pMarket )
{
	ARegion * r = app->GetRegions()->GetRegion( pMarket->region );
	wxTreeItemId region = FindItem( r );

	bool found = true;
	long cookie;
	wxTreeItemId category = tree->GetFirstChild( region, cookie );
	wxTreeItemId lastcategory = tree->GetLastChild( region );
	while ( tree->GetItemText( category ) != "Markets" )
	{
		if( category == lastcategory ) {
			found = false;
			break;
		}
		category = tree->GetNextChild( region, cookie );
	}
	if( found ) {
		AString temp = AString( ( pMarket->type == M_BUY?"Sells ":"Buys " ) );
		temp += ItemString( pMarket->item, pMarket->amount ) + " at $" +
				pMarket->price;
		tree->AppendItem( category, temp.Str(), -1, -1,
					 new MarketLeaf( pMarket ) );
	}

}

void TreeCanvas::AddItem( Production * pProduction )
{
	ARegion * r = app->GetRegions()->GetRegion( pProduction->region );
	wxTreeItemId region = FindItem( r );

	bool found = true;
	long cookie;
	wxTreeItemId category = tree->GetFirstChild( region, cookie );
	wxTreeItemId lastcategory = tree->GetLastChild( region );
	while ( tree->GetItemText( category ) != "Products" )
	{
		if( category == lastcategory ) {
			found = false;
			break;
		}
		category = tree->GetNextChild( region, cookie );
	}
	if( found ) {
		AString temp = ItemString( pProduction->itemtype, pProduction->amount );
		tree->AppendItem( category, temp.Str(), -1, -1,
						new ProductionLeaf( pProduction ) );
	}

}

void TreeCanvas::AddItem( Object * pObject, bool populate )
{
	wxTreeItemId region = FindItem( pObject->region );
	if( region ) {
		bool found = true;
		long cookie;
		wxTreeItemId category = tree->GetFirstChild( region, cookie );
		wxTreeItemId lastcategory = tree->GetLastChild( region );
		while ( tree->GetItemText( category ) != "Objects" )
		{
			if( category == lastcategory ) {
				found = false;
				break;
			}
			category = tree->GetNextChild( region, cookie );
		}
		if( found ) {
			AString temp = *pObject->name + " : " + ObjectDefs[pObject->type].name;
			wxTreeItemId leaf = tree->AppendItem( category, _T( temp.Str() ), -1, -1,
							 new ObjectLeaf( pObject ) );
			if( populate ) {
				forlist ( &pObject->units ) {
					Unit * u = ( Unit * ) elem;
					temp = *u->name;
					tree->AppendItem( leaf, temp.Str(), -1, -1,
									 new UnitLeaf( u ) );
				}
			}
		}
	}

}


void TreeCanvas::RemoveItem( Faction * pFaction )
{
	wxTreeItemId id = FindItem( pFaction );
	tree->Delete( id );
}

void TreeCanvas::RemoveItem( ARegion * pRegion )
{
	wxTreeItemId id = FindItem( pRegion );
	tree->Delete( id );
}

void TreeCanvas::RemoveItem( Market * pMarket )
{
	wxTreeItemId id = FindItem( pMarket );
	tree->Delete( id );
}

void TreeCanvas::RemoveItem( Production * pProduction )
{
	wxTreeItemId id = FindItem( pProduction );
	tree->Delete( id );
}

void TreeCanvas::RemoveItem( Object * pObject )
{
	wxTreeItemId id = FindItem( pObject );
	tree->Delete( id );
}

void TreeCanvas::RemoveItem( Unit * pUnit )
{
	wxTreeItemId id = FindItem( pUnit );
	tree->Delete( id );
}

void TreeCanvas::UpdateItem( Faction * pFaction)
{
	wxTreeItemId id = FindItem( pFaction );
	tree->SetItemText( id, pFaction->name->Str() );
}

void TreeCanvas::UpdateItem( ARegion * pRegion)
{
	wxTreeItemId id = FindItem( pRegion );
	AString temp = AString( "(" ) + pRegion->xloc + ", " +
	                        pRegion->yloc + ", "+ pRegion->zloc +
							") " + *pRegion->name;
	if( pRegion->town ) temp += AString( " [" ) + *pRegion->town->name + "]";
	tree->SetItemText( id, temp.Str() );
}

void TreeCanvas::UpdateItem( Market * pMarket)
{
	wxTreeItemId id = FindItem( pMarket );
	AString temp = AString( ( pMarket->type == M_BUY?"Sells ":"Buys " ) );
	temp += ItemString( pMarket->item, pMarket->amount ) +
	                    " at $" + pMarket->price;
	tree->SetItemText( id, temp.Str() );
}

void TreeCanvas::UpdateItem( Production * pProduction)
{
	wxTreeItemId id = FindItem( pProduction );
	AString temp = ItemString( pProduction->itemtype, pProduction->amount );
	tree->SetItemText( id, temp.Str() );
}

void TreeCanvas::UpdateItem( Object * pObject)
{
	wxTreeItemId id = FindItem( pObject );
	AString	temp = *pObject->name + " : " + ObjectDefs[pObject->type].name;
	tree->SetItemText( id, temp.Str() );
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
		wxTreeItemId faction = FindItem( pUnit->faction );
		id1 = tree->AppendItem( faction, "", -1, -1, 
						  new UnitLeaf( pUnit ) );
	}


	wxTreeItemId id2 = FindItem( pUnit, false );
	AString temp = *pUnit->name;
	wxString t = tree->GetItemText(id1);
	if( id1.IsOk() )
		tree->SetItemText( id1, temp.Str() );
	tree->SetItemText( id2, temp.Str() );
}


void TreeCanvas::AddItem( Unit * pUnit )
{
	AString temp = *pUnit->name;

	wxTreeItemId object = FindItem( pUnit->object );
	tree->AppendItem( object, temp.Str(), -1, -1,
					 new UnitLeaf( pUnit ) );

	wxTreeItemId faction = FindItem( pUnit->faction );
	tree->AppendItem( faction, temp.Str(), -1, -1, 
					 new UnitLeaf( pUnit ) );

}

wxTreeItemId TreeCanvas::FindItem( ARegionArray * pArr )
{
	long cookie;
	wxTreeItemId level = tree->GetFirstChild( treeLevels, cookie );
	if( ! level.IsOk() ) return new wxTreeItemId;
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
	wxTreeItemId level = FindItem( app->GetRegions()->pRegionArrays[pRegion->zloc] );
	long cookie;
	bool found = true;
	if( level ) {
		wxTreeItemId region = tree->GetFirstChild( level, cookie );
		if( ! region.IsOk() ) return new wxTreeItemId;
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
	if( ! faction.IsOk() ) return new wxTreeItemId;
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
		bool found = true;
		long cookie;
		wxTreeItemId category = tree->GetFirstChild( region, cookie );
		wxTreeItemId lastcategory = tree->GetLastChild( region );
		while ( tree->GetItemText( category ) != "Objects" )
		{
			if( category == lastcategory ) {
				found = false;
				break;
			}
			category = tree->GetNextChild( region, cookie );
		}
		if( found ) {
			wxTreeItemId object = tree->GetFirstChild( category, cookie );
			if( ! object.IsOk() ) return new wxTreeItemId;
			wxTreeItemId lastobject = tree->GetLastChild( category );
			while ( ( ( ObjectLeaf * )tree->GetItemData( object ) )->object !=
				pObject )
			{
				if( object == lastobject ) {
					found = false;
					break;
				}
				object = tree->GetNextChild( category, cookie );
			}
			if( found )
				return object;
		}
	}
	return wxTreeItemId();
}

wxTreeItemId TreeCanvas::FindItem( Market * pMarket )
{
	ARegion * r = app->GetRegions()->GetRegion( pMarket->region );
	wxTreeItemId region = FindItem( r );

	bool found = true;
	long cookie;
	wxTreeItemId category = tree->GetFirstChild( region, cookie );
	wxTreeItemId lastcategory = tree->GetLastChild( region );
	while ( tree->GetItemText( category ) != "Markets" )
	{
		if( category == lastcategory ) {
			found = false;
			break;
		}
		category = tree->GetNextChild( region, cookie );
	}
	if( found ) {
		wxTreeItemId market = tree->GetFirstChild( category, cookie );
		if( ! market.IsOk() ) return new wxTreeItemId;
		wxTreeItemId lastmarket = tree->GetLastChild( category );
		while ( ( ( MarketLeaf * )tree->GetItemData( market ) )->market !=
			pMarket )
		{
			if( market == lastmarket ) {
				found = false;
				break;
			}
			market = tree->GetNextChild( category, cookie );
		}
		if( found )
			return market;
	}
	return wxTreeItemId();
}

wxTreeItemId TreeCanvas::FindItem( Production * pProduction )
{
	ARegion * r = app->GetRegions()->GetRegion( pProduction->region );
	wxTreeItemId region = FindItem( r );

	bool found = true;
	long cookie;
	wxTreeItemId category = tree->GetFirstChild( region, cookie );
	wxTreeItemId lastcategory = tree->GetLastChild( region );
	while ( tree->GetItemText( category ) != "Products" )
	{
		if( category == lastcategory ) {
			found = false;
			break;
		}
		category = tree->GetNextChild( region, cookie );
	}
	if( found ) {
		wxTreeItemId production = tree->GetFirstChild( category, cookie );
		if( ! production.IsOk() ) return new wxTreeItemId;
		wxTreeItemId lastProduction = tree->GetLastChild( category );
		while ( ( ( ProductionLeaf * )tree->GetItemData( production ) )->production !=
			pProduction )
		{
			if( production == lastProduction ) {
				found = false;
				break;
			}
			production = tree->GetNextChild( category, cookie );
		}
		if( found )
			return production;
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
		if( ! unit.IsOk() ) return new wxTreeItemId;
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

void TreeCanvas::Init()
{
	AString temp;

	tree->SetBackgroundColour( GetBackgroundColour() );
	wxTreeItemId root = tree->AddRoot( _T( "Atlantis" ) );

	// factions
	treeFactions = tree->AppendItem( root, _T( "Factions" ) );
	{
		forlist ( app->GetFactions() ) {
			Faction * f = ( Faction * ) elem;
			tree->AppendItem( treeFactions, f->name->Str(), -1, -1,
							new FactionLeaf( f ) );
		}
	}
	// levels
	treeLevels = tree->AppendItem( root, _T( "Levels" ) );

	int level;
	for( level = 0; level < app->GetRegions()->numLevels; level++ ) {
		ARegionArray * pArr = app->GetRegions()->pRegionArrays[ level ];
		temp = AString( "(" ) + level + ") ";
		if( pArr->strName )
			temp += *pArr->strName;
		else
			temp += "surface";
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
				wxTreeItemId treeMarkets = tree->AppendItem( curRegion, _T( "Markets" ) );
				{
					forlist ( &r->markets ) {
						Market * m = ( Market * ) elem;
						//Hack:
						m->region = r->num;
						temp = AString( ( m->type == M_BUY?"Sells ":"Buys " ) );
						temp += ItemString( m->item, m->amount ) + " at $" + m->price;
						tree->AppendItem( treeMarkets, _T( temp.Str() ), -1, -1,
										 new MarketLeaf( m ) );
					}
				}
				//products
				wxTreeItemId treeProducts = tree->AppendItem( curRegion, _T( "Products" ) );
				{
					forlist ( &r->products ) {
						Production * p = ( Production * ) elem;
						//Hack:
						p->region = r->num;
						temp = ItemString( p->itemtype, p->amount );
						tree->AppendItem( treeProducts, _T( temp.Str() ), -1, -1,
										new ProductionLeaf( p ) );
					}
				}
				//objects
				wxTreeItemId treeObjects = tree->AppendItem( curRegion, _T( "Objects" ) );
				forlist ( &r->objects ) {
					Object * o = ( Object * ) elem;
					temp = *o->name + " : " + ObjectDefs[o->type].name;
					wxTreeItemId curObject = tree->AppendItem( treeObjects, _T( temp.Str() ), -1, -1,
									 new ObjectLeaf( o ) );
					//units
					forlist ( &o->units ) {
						Unit * u = ( Unit * ) elem;
						temp = *u->name;
						tree->AppendItem( curObject, _T( temp.Str() ), -1, -1,
										 new UnitLeaf( u ) );
						//add to factions
						wxTreeItemId faction = FindItem( u->faction );
						tree->AppendItem( faction, _T( temp.Str() ), -1, -1, 
										 new UnitLeaf( u ) );
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

void TreeLeaf::UpdateEditor()
{
}

FactionLeaf::FactionLeaf( Faction * pFaction ) : TreeLeaf()
{
	faction = pFaction;
}

void FactionLeaf::UpdateEditor()
{
	app->SelectFaction( faction );
}

LevelLeaf::LevelLeaf( ARegionArray * pAArray, int l ) : TreeLeaf()
{
	regionArray = pAArray;
	level = l;
}

void LevelLeaf::UpdateEditor()
{
}

RegionLeaf::RegionLeaf( ARegion * pRegion ) : TreeLeaf()
{
	region = pRegion;
}

void RegionLeaf::UpdateEditor()
{
	app->SelectRegion( region );
}

MarketLeaf::MarketLeaf( Market * pMarket ) : TreeLeaf()
{
	market = pMarket;
}

void MarketLeaf::UpdateEditor()
{
	app->SelectMarket( market );
}

ProductionLeaf::ProductionLeaf( Production * pProduction ) : TreeLeaf()
{
	production = pProduction;
}

void ProductionLeaf::UpdateEditor()
{
	app->SelectProduction( production );
}

ObjectLeaf::ObjectLeaf( Object * pObject ) : TreeLeaf()
{
	object = pObject;
}

void ObjectLeaf::UpdateEditor()
{
	app->SelectObject( object );
}

UnitLeaf::UnitLeaf( Unit * pUnit ) : TreeLeaf()
{
	unit = pUnit;
}

void UnitLeaf::UpdateEditor()
{
	app->SelectUnit( unit );
}
