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

#ifndef EDIT_CLASS
#define EDIT_CLASS

class EditPage;
class EditFrame;

#include "../astring.h"
#include "../aregion.h"

#include "wx/wx.h"

#include "editaux.h"

// control ids
enum
{
	Edit_Tool_Add,
	Edit_Tool_Delete,
	Edit_Tool_Move,
	Edit_Tool_Add_Market,
	Edit_Tool_Add_Production,
	Edit_Tool_Add_Object,
	Edit_Tool_Add_Unit,
	Edit_Faction_Number,
	Edit_Faction_Name,
	Edit_Faction_Address,
	Edit_Faction_Password,
	Edit_Faction_Unclaimed,
	Edit_Faction_LastChange,
	Edit_Faction_LastOrders,
	Edit_Faction_Times,
	Edit_Faction_Temformat,
	Edit_Faction_Quit,
	Edit_Level_Name,
	Edit_Level_LevelType,
//	Edit_Region_Number,
	Edit_Region_Name,
	Edit_Region_Type,
	Edit_Region_Buildingseq,
	Edit_Region_Gate,
	Edit_Region_Race,
	Edit_Region_Pop,
	Edit_Region_Basepop,
	Edit_Region_Wages,
	Edit_Region_Maxwages,
	Edit_Region_Money,
	Edit_Region_Town,
	Edit_Region_TownName,
	Edit_Region_TownPop,
	Edit_Region_TownBasepop,
	Edit_Region_Object,
	Edit_Region_N,
	Edit_Region_NE,
	Edit_Region_SE,
	Edit_Region_S,
	Edit_Region_SW,
	Edit_Region_NW,
	Edit_Market_Type,
	Edit_Market_Item,
	Edit_Market_Price,
	Edit_Market_Amount,
	Edit_Market_Minpop,
	Edit_Market_Maxpop,
	Edit_Market_Minamt,
	Edit_Market_Maxamt,
	Edit_Production_Itemtype,
	Edit_Production_Baseamount,
	Edit_Production_Amount,
	Edit_Production_Skill,
	Edit_Production_Level,
	Edit_Production_Productivity,
	Edit_Production_Activity,
	Edit_Object_Name,
	Edit_Object_Describe,
	Edit_Object_Region,
	Edit_Object_Inner,
	Edit_Object_Num,
	Edit_Object_Type,
	Edit_Object_Incomplete,
	Edit_Object_Runes,
	Edit_Unit_Num,
	Edit_Unit_Name,
	Edit_Unit_Describe,
	Edit_Unit_Faction,
	Edit_Unit_Type,
	Edit_Unit_Guard,
	Edit_Unit_Reveal,
	Edit_Unit_Flags,
	Edit_Unit_Combat,
	Edit_Unit_ReadyItem,
	Edit_Unit_ReadyWeapon,
	Edit_Unit_ReadyArmor,
	Edit_Unit_Items,
	Edit_Unit_Skills,
	Edit_Game_Month,
	Edit_Game_Year,
	Edit_Game_Seed,
	Edit_Game_FactionSeq,
	Edit_Game_UnitSeq,
	Edit_Game_ShipSeq,
	Edit_Game_GuardFaction,
	Edit_Game_MonFaction
};

class EditPage : public wxScrolledWindow
{
	public:
		EditPage( wxWindow* parent, const wxPoint& pos, const wxSize& size );
		~EditPage();

		virtual void EnableAllControls( bool );
		virtual void ClearAllControls();

		virtual void OnTextUpdate( wxCommandEvent & );
		virtual void OnButton( wxCommandEvent & );
		virtual void SelectItem( AListElem * );
		virtual void PostLoad();

		void ReloadPage();
		void OnSize( wxSizeEvent & );

		int AddToControl( wxTextCtrl *, long );
		int AddToControl( wxTextCtrl *, const char * );
		int AddToControl( wxButton *, const char * );
		int AddToControl( wxComboBox *, int );

		void UpdateControl( wxTextCtrl *, int & );
		void UpdateControl( wxTextCtrl *, unsigned int & );
		void UpdateControl( wxTextCtrl *, AString * );
		void UpdateControl( wxComboBox *, int & );

		void DeleteCombo( wxComboBox * );

		ARegion * GetRegionChoice( const wxString &, const wxString & );

		wxStaticBox * border;
		wxSizer * sizerEdit;
		wxSizer * sizerTool;
		wxToolBar * toolBar;
		AElemArray * selectedElems;

		wxTextValidator * textStringValidator;
		wxTextValidator * numStringValidator;

		bool editWait;

		virtual void OnToolAdd( wxCommandEvent & );
		virtual void OnToolDelete( wxCommandEvent & );
		virtual void OnToolMove( wxCommandEvent & );

		DECLARE_EVENT_TABLE()

	protected:
		EditFrame * editTop;

};

class EditRegionPage : public EditPage
{
public:
	EditRegionPage( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditRegionPage();

	void EnableAllControls( bool );
	void ClearAllControls();

	void OnTextUpdate( wxCommandEvent & );
	void OnButton( wxCommandEvent & );
	void OnToolAdd( wxCommandEvent & );
	void OnToolDelete( wxCommandEvent & );
	void OnToolMove( wxCommandEvent & );
	void SelectItem( AListElem * );
	void SelectItems( AElemArray * );
	void PostLoad();

//	wxTextCtrl * editNumber;
	wxTextCtrl * editName;
	wxButton * editType;
	wxTextCtrl * editBuildingseq;
	wxTextCtrl * editGate;
	wxButton * editRace;
	wxTextCtrl * editPop;
	wxTextCtrl * editBasepop;
	wxTextCtrl * editWages;
	wxTextCtrl * editMaxwages;
	wxTextCtrl * editMoney;
	wxButton * editTown;
	wxTextCtrl * editTownName;
	wxTextCtrl * editTownPop;
	wxTextCtrl * editTownBasepop;
	wxButton * editExits[6];

};

class EditFactionPage : public EditPage
{
public:
	EditFactionPage( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditFactionPage();

	void EnableAllControls( bool );
	void ClearAllControls();

	void OnTextUpdate( wxCommandEvent & );
	void OnButton( wxCommandEvent & );
	void OnToolAdd( wxCommandEvent & );
	void OnToolDelete( wxCommandEvent & );
	void OnToolMove( wxCommandEvent & );
	void SelectItem( AListElem * );
	void PostLoad();

	wxTextCtrl * editNumber;
	wxTextCtrl * editLastChange;
	wxTextCtrl * editLastOrders;
	wxTextCtrl * editUnclaimed;
	wxTextCtrl * editName;
	wxTextCtrl * editAddress;
	wxTextCtrl * editPassword;
	wxTextCtrl * editTimes;
	wxComboBox * editTemformat;
	wxComboBox * editQuit;

};

class EditMarketPage : public EditPage
{
public:
	EditMarketPage( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditMarketPage();

	void EnableAllControls( bool );
	void ClearAllControls();
	void OnTextUpdate( wxCommandEvent & );
	void OnButton( wxCommandEvent & );
	void OnToolAdd( wxCommandEvent & );
	void OnToolDelete( wxCommandEvent & );
	void OnToolMove( wxCommandEvent & );
	void SelectItem( AListElem * );
	void PostLoad();

	wxComboBox * editType;
	wxComboBox * editItem;
	wxTextCtrl * editPrice;
	wxTextCtrl * editAmount;
	wxTextCtrl * editMinpop;
	wxTextCtrl * editMaxpop;
	wxTextCtrl * editMinamt;
	wxTextCtrl * editMaxamt;

};

class EditProductionPage : public EditPage
{
public:
	EditProductionPage( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditProductionPage();

	void EnableAllControls( bool );
	void ClearAllControls();
	void OnTextUpdate( wxCommandEvent & );
	void OnButton( wxCommandEvent & );
	void OnToolAdd( wxCommandEvent & );
	void OnToolDelete( wxCommandEvent & );
	void OnToolMove( wxCommandEvent & );
	void SelectItem( AListElem * );
	void PostLoad();

	wxComboBox * editItemtype;
	wxTextCtrl * editBaseamount;
	wxTextCtrl * editAmount;
	wxTextCtrl * editSkill;
	wxTextCtrl * editLevel;
	wxTextCtrl * editProductivity;
	wxTextCtrl * editActivity;

};

class EditUnitPage : public EditPage
{
public:
	EditUnitPage( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditUnitPage();

	void EnableAllControls( bool );
	void ClearAllControls();
	void OnTextUpdate( wxCommandEvent & );
	void OnButton( wxCommandEvent & );
	void OnToolAdd( wxCommandEvent & );
	void OnToolDelete( wxCommandEvent & );
	void OnToolMove( wxCommandEvent & );
	void SelectItem( AListElem * );
	void PostLoad();

	wxTextCtrl * editNum;
	wxTextCtrl * editName;
	wxTextCtrl * editDescribe;
	wxButton * editFaction;
	wxComboBox * editType;
	wxComboBox * editGuard;
	wxComboBox * editReveal;
	wxButton * editCombat;
	wxButton * editReadyItem;
	wxButton * editReadyWeapon;
	wxButton * editReadyArmor;
	wxButton * editItems;
	wxButton * editSkills;
};

class EditObjectPage : public EditPage
{
public:
	EditObjectPage( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditObjectPage();

	void EnableAllControls( bool );
	void ClearAllControls();
	void OnTextUpdate( wxCommandEvent & );
	void OnButton( wxCommandEvent & );
	void OnToolAdd( wxCommandEvent & );
	void OnToolDelete( wxCommandEvent & );
	void OnToolMove( wxCommandEvent & );
	void SelectItem( AListElem * );
	void PostLoad();

	wxTextCtrl * editName;
	wxTextCtrl * editDescribe;
	wxTextCtrl * editRegion;
	wxButton * editInner;
	wxTextCtrl * editNum;
	wxComboBox * editType;
	wxTextCtrl * editIncomplete;
	wxTextCtrl * editRunes;

};

class EditLevelPage : public EditPage
{
public:
	EditLevelPage( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditLevelPage();

//	void LoadData();
	void EnableAllControls( bool );
	void ClearAllControls();
	void OnTextUpdate( wxCommandEvent & );
	void OnButton( wxCommandEvent & );
	void OnToolAdd( wxCommandEvent & );
	void OnToolDelete( wxCommandEvent & );
	void OnToolMove( wxCommandEvent & );
	void SelectItem( AListElem * );
	void SelectItem( ARegionArray * );
	void PostLoad();

	wxTextCtrl * editName;
	wxComboBox * editLevelType;

};

class EditGamePage : public EditPage
{
public:
	EditGamePage( wxWindow* parent, const wxPoint& pos, const wxSize& size );
	~EditGamePage();

	void EnableAllControls( bool );
	void ClearAllControls();
	void OnTextUpdate( wxCommandEvent & );
	void OnButton( wxCommandEvent & );
	void OnToolAdd( wxCommandEvent & );
	void OnToolDelete( wxCommandEvent & );
	void OnToolMove( wxCommandEvent & );
	void SelectItem( AListElem * );
	void SelectItem( Game * );
	void PostLoad();

	wxTextCtrl * editMonth;
	wxTextCtrl * editYear;
	wxTextCtrl * editFactionSeq;
	wxTextCtrl * editUnitSeq;
	wxTextCtrl * editShipSeq;
	wxButton * editGuardFaction;
	wxButton * editMonFaction;

};

class EditFrame: public wxWindow
{
	public:
		EditFrame( wxWindow *parent, const wxPoint& pos, const wxSize& size );
		~EditFrame();
		void Init();
		void UpdateSelection();
		void HideAllPages();
		void DeselectAll();

		ARegionArray * selectedLevel;
		AElemArray * selectedElems;
		int curSelection;

	private:
		void ShowPage( EditPage * page );
		void OnSize( wxSizeEvent & );

		EditFactionPage * factionPage;
		EditLevelPage * levelPage;
		EditRegionPage * regionPage;
		EditMarketPage * marketPage;
		EditProductionPage * productionPage;
		EditObjectPage * objectPage;
		EditUnitPage * unitPage;
		EditGamePage * gamePage;
		EditPage * currentPage;

		void SelectItem( AListElem *, int type );
		void SelectItem( Game * );
		void SelectItem( ARegionArray * );
		void PostLoad( int type );

		void SetWait( bool wait );

		EditPage * curPage;

		DECLARE_EVENT_TABLE()
};

#endif
