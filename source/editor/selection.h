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

#ifndef SELECTION_CLASS
#define SELECTION_CLASS

class SelectionCanvas;

#include "../alist.h"
#include "../aregion.h"
#include "aextend.h"

#include "wx/wx.h"
#include "wx/treectrl.h"

// control ids
enum
{
	Selection_Tree,
	Selection_Tool_Back,
	Selection_Tool_Forward,
	Selection_Tool_Zoom_In,
	Selection_Tool_Zoom_Out,
};

class SelectionCanvas : public wxScrolledWindow
{
public:
	SelectionCanvas( wxWindow *parent );
	~SelectionCanvas();
	void Init();

	void AddItem( Faction *, wxTreeItemId, bool populate = true );
	void AddItem( ARegion *, wxTreeItemId, bool populate = true );
	void AddItem( Market *, wxTreeItemId );
	void AddItem( Production *, wxTreeItemId );
	void AddItem( Object *, wxTreeItemId, bool populate = true );
	void AddItem( Unit *, wxTreeItemId );
	void AddItem( Game *, wxTreeItemId );
	void AddItem( ARegionArray *, wxTreeItemId, bool populate = true );

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

	void AddElem( AListElem * );
	void RemoveElem( AListElem * );

	void UpdateSelectionLeafs( int type );
	void UpdateSelectionRegions();
	void UpdateSelectionFactions();
	void UpdateSelectionObjects();
	void UpdateSelectionMarkets();
	void UpdateSelectionProductions();
	void UpdateSelectionUnits( bool unitByFaction = true );

	void OnSize( wxSizeEvent & );
	void OnLeafSelection( wxTreeEvent & );
	void OnLeafSelecting( wxTreeEvent & );
	bool IsLeafHighlighted( wxTreeItemId );
	void OnBack( wxCommandEvent & );
	void OnForward( wxCommandEvent & );
	void OnZoomOut( wxCommandEvent & );
	void OnZoomIn( wxCommandEvent & );
	void OnZoomIn( wxTreeEvent & );
	void ZoomIn();

	void Resize();

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

	wxSizer * sizerSelect;
	wxSizer * sizerTool;
	wxToolBar * toolBar;

};


class TreeLeaf : public wxTreeItemData
{
	public:
		~TreeLeaf();

		int class_id;
};

class RegionLeaf : public TreeLeaf
{
	public:
		RegionLeaf( ARegion * );

		ARegion * region;
};

class LevelLeaf : public TreeLeaf
{
	public:
		LevelLeaf( ARegionArray *, int );

		ARegionArray * regionArray;
		int level;
};

class FactionLeaf : public TreeLeaf
{
	public:
		FactionLeaf( Faction * );

		Faction * faction;
};

class MarketLeaf : public TreeLeaf
{
	public:
		MarketLeaf( Market * );

		Market * market;
};

class ObjectLeaf : public TreeLeaf
{
	public:
		ObjectLeaf( Object * );

		Object * object;
};

class UnitLeaf : public TreeLeaf
{
	public:
		UnitLeaf( Unit *, bool byFac );

		bool byFaction;
		Unit * unit;
};

class ProductionLeaf : public TreeLeaf
{
	public:
		ProductionLeaf( Production * );

		Production * production;
};

class GameLeaf : public TreeLeaf
{
	public:
		GameLeaf( Game * );

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
