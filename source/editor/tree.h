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

#ifndef TREE_CLASS
#define TREE_CLASS

#include "../alist.h"
#include "../aregion.h"
#include "aextend.h"

#include "wx/wx.h"
#include "wx/treectrl.h"

// control ids
enum
{
	Tree_Multi_Select,
	Tree_Tree,
};

class TreeCanvas : public wxScrolledWindow
{
public:
	TreeCanvas( wxWindow *parent );
	~TreeCanvas();
	void Init();

	void AddItem( Faction *, bool populate = true );
	void AddItem( ARegion *, bool populate = true );
	void AddItem( Market * );
	void AddItem( Production * );
	void AddItem( Object *, bool populate = true );
	void AddItem( Unit * );

	void RemoveItem( Faction * );
	void RemoveItem( ARegion * );
	void RemoveItem( Market * );
	void RemoveItem( Production * );
	void RemoveItem( Object * );
	void RemoveItem( Unit * );

	void UpdateItem( Faction * );
	void UpdateItem( ARegion *, bool populate = false );
	void UpdateItem( Market * );
	void UpdateItem( Production * );
	void UpdateItem( Object * );
	void UpdateItem( Unit * );
	void UpdateItem( ARegionArray * );

	void UpdateSelection();
	void ToggleHeaders( bool toggle );

	bool lastUnitByFaction;

	DECLARE_EVENT_TABLE()

private:
	wxTreeItemId FindItem( ARegionArray * );
	wxTreeItemId FindItem( ARegion * );
	wxTreeItemId FindItem( Faction * );
	wxTreeItemId FindItem( Object * );
	wxTreeItemId FindItem( Unit *, bool unitByFaction = true );
	wxTreeItemId FindItem( Market * );
	wxTreeItemId FindItem( Production * );
	wxTreeItemId FindCategory( const char *, wxTreeItemId );
	wxTreeItemId FindSelected( AListElem *, int type, bool unitByFaction = false );

	void UpdateSelectionLeafs( int type );
	void UpdateSelectionRegions();
	void UpdateSelectionFactions();
	void UpdateSelectionObjects();
	void UpdateSelectionMarkets();
	void UpdateSelectionProductions();
	void UpdateSelectionUnits( bool unitByFaction = true );

	void OnResize( wxSizeEvent & );
	void OnLeafSelection( wxTreeEvent & );
	void OnLeafSelecting( wxTreeEvent & );
	bool IsLeafHighlighted( wxTreeItemId );

	void DeselectAll();
	void UnHighlightLeaf( wxTreeItemId );
	void HighlightLeaf( wxTreeItemId );

	wxTreeCtrl * tree;

	wxTreeItemId treeFactions;
	wxTreeItemId treeLevels;
	wxTreeItemId lastSelected;

	ARegionArray * selectedLevel;
	AElemArray * selectedElems;
	int curSelection;

	int selectOK;
	bool treeWait;
	bool showHeaders;

};

class TreeLeaf : public wxTreeItemData
{
	public:
		~TreeLeaf();
		virtual void MakeSelection( bool add = false );

		int class_id;
};

class RegionLeaf : public TreeLeaf
{
	public:
		RegionLeaf( ARegion * );
		void MakeSelection( bool add = false );

		ARegion * region;
};

class LevelLeaf : public TreeLeaf
{
	public:
		LevelLeaf( ARegionArray *, int );
		void MakeSelection( bool add = false );

		ARegionArray * regionArray;
		int level;
};

class FactionLeaf : public TreeLeaf
{
	public:
		FactionLeaf( Faction * );
		void MakeSelection( bool add = false );

		Faction * faction;
};

class MarketLeaf : public TreeLeaf
{
	public:
		MarketLeaf( Market * );
		void MakeSelection( bool add = false );

		Market * market;
};

class ObjectLeaf : public TreeLeaf
{
	public:
		ObjectLeaf( Object * );
		void MakeSelection( bool add = false );

		Object * object;
};

class UnitLeaf : public TreeLeaf
{
	public:
		UnitLeaf( Unit *, bool byFac );
		void MakeSelection( bool add = false );

		bool byFaction;
		Unit * unit;
};

class ProductionLeaf : public TreeLeaf
{
	public:
		ProductionLeaf( Production * );
		void MakeSelection( bool add = false );

		Production * production;
};

class GameLeaf : public TreeLeaf
{
	public:
		GameLeaf( Game * );
		void MakeSelection( bool add = false );

		Game * game;
};

enum {
	LEAF_REG,
	LEAF_LEV,
	LEAF_FAC,
	LEAF_MAR,
	LEAF_OBJ,
	LEAF_UNI,
	LEAF_PRO,
	LEAF_GAM,
};

#endif
