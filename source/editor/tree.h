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

#ifndef TREE_CLASS
#define TREE_CLASS

#include "../alist.h"
#include "../aregion.h"

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
	TreeCanvas( wxWindow *parent, const wxPoint& pos, const wxSize& size );
	void Init();

	void SelectItem( Faction *, bool add = false );
	void SelectItem( ARegion *, bool add = false );
	void SelectItem( Market *, bool add = false );
	void SelectItem( Production *, bool add = false );
	void SelectItem( Object *, bool add = false );
	void SelectItem( Unit *, bool add = false );

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
	void UpdateItem( ARegion * );
	void UpdateItem( Market * );
	void UpdateItem( Production * );
	void UpdateItem( Object * );
	void UpdateItem( Unit * );

	wxTreeCtrl * tree;

	DECLARE_EVENT_TABLE()
private:
	bool treeWait;
	wxTreeItemId treeFactions;
	wxTreeItemId treeLevels;

	wxTreeItemId FindItem( ARegionArray * );
	wxTreeItemId FindItem( ARegion * );
	wxTreeItemId FindItem( Faction * );
	wxTreeItemId FindItem( Object * );
	wxTreeItemId FindItem( Unit *, bool unitByFaction = true );
	wxTreeItemId FindItem( Market * );
	wxTreeItemId FindItem( Production * );

	void OnResize( wxSizeEvent & );
	void OnLeafSelection( wxTreeEvent & );

};

class TreeChild : public wxMDIChildFrame
{
public:
	TreeChild( wxMDIParentFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style );
	
	TreeCanvas * canvas;
	void InitTree();
private:

	void OnClose( wxCloseEvent & );
	void OnMultiToggle( wxCommandEvent & );
	DECLARE_EVENT_TABLE()
};

class TreeLeaf : public wxTreeItemData
{
public:
	~TreeLeaf();
	virtual void UpdateEditor();
};

class RegionLeaf : public TreeLeaf
{
public:
	RegionLeaf( ARegion * );
	void UpdateEditor();

	ARegion * region;
};

class LevelLeaf : public TreeLeaf
{
public:
	LevelLeaf( ARegionArray *, int );
	void UpdateEditor();

	ARegionArray * regionArray;
	int level;
};

class FactionLeaf : public TreeLeaf
{
public:
	FactionLeaf( Faction * );
	void UpdateEditor();

	Faction * faction;
};

class MarketLeaf : public TreeLeaf
{
public:
	MarketLeaf( Market * );
	void UpdateEditor();

	Market * market;
};

class ObjectLeaf : public TreeLeaf
{
public:
	ObjectLeaf( Object * );
	void UpdateEditor();

	Object * object;
};

class UnitLeaf : public TreeLeaf
{
public:
	UnitLeaf( Unit * );
	void UpdateEditor();

	Unit * unit;
};

class ProductionLeaf : public TreeLeaf
{
public:
	ProductionLeaf( Production * );
	void UpdateEditor();

	Production * production;
};

#endif