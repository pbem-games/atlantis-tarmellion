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

#ifndef MAP_CLASS
#define MAP_CLASS

#include "../aregion.h"

#include "aextend.h"

#include "wx/wx.h"

#define ICON_SIZE		6
#define ICON_SIZE_SMALL 4

class MapCanvas : public wxWindow
{
public:
	MapCanvas( wxWindow *parent );
	~MapCanvas();

	void UpdateSelection();

protected:

	void InitPlane( int, ARegion * center = NULL );
	void InitToolBar( wxToolBar * );
	void SetHexSize( int );
	void AdjustScrollBars();

	void GetHexCenter( int NoX, int NoY, int & WinX, int & WinY );
	void GetHexCoord( int & NoX, int & NoY, int WinX, int WinY );

	void OnEvent( wxMouseEvent& );
	void OnScroll( wxScrollWinEvent& );
	void OnResize( wxSizeEvent& );
	void OnPaint( wxPaintEvent& );

	void OnPlaneDown( wxCommandEvent & );
	void OnPlaneUp( wxCommandEvent & );
	void OnZoomIn( wxCommandEvent & );
	void OnZoomOut( wxCommandEvent & );
	void OnShowCities( wxCommandEvent & );
	void OnShowObjects( wxCommandEvent & );
	void OnShowGates( wxCommandEvent & );
	void OnShowShafts( wxCommandEvent & );
	void OnShowCoords( wxCommandEvent & );
	void OnShowNames( wxCommandEvent & );
	void OnShowOutlines( wxCommandEvent & );
	void OnLevelSelect( wxCommandEvent & );

	int ValidHexNo( int NoX, int NoY );
	void WinToAtla( int   WinX, int   WinY, long & AtlaX, long & AtlaY );
	void AtlaToWin( int & WinX, int & WinY, long   AtlaX, long	 AtlaY );
	void GetNextIconPos( int x0, int y0, int & x, int & y );
	ARegion * GetCenterHex();
	void SetCenterHex( ARegion * pRegion );

	void DrawMap( int clearFirst = 0 );
	void DrawMap( wxDC * );
	void DrawHex( ARegion *, wxDC *, int highlight = -1, int drawName = 1 );//int NoX, int NoY, wxDC * pDC, CLand * pLand, CPlane * pPlane, wxRect * prect*;
	void DrawAHex( int, int, wxPen *, wxBrush *, wxDC *, ARegion * exits[] = NULL, wxImage * image = NULL );
	void DrawBorders( wxDC * );
	void DrawCoords( wxDC * );
	void DrawNames( wxDC * );
	void DrawOverlay( wxDC * );
	void DrawTown( int, int, int, wxDC * );
	void DrawRoad( int, int, wxDC *, Object * );
	void DrawGate( int, int, wxDC * );
	void DrawShaft( int, int, wxDC * );
	void DrawBoat( int, int, wxDC * );
	void DrawOther( int, int, wxDC * );
	void DrawName( TownInfo *, int, int, wxDC * );
	void DrawString( wxString &, int, int, wxDC * );
	void SetDC( wxDC & dc );

	wxToolBar * toolbar;
	wxComboBox * comboLevel;
	wxComboBox * comboSelect;
	wxSizer * sizerTool;
	wxSizer * sizerMap;
	wxFrame * logFrame;

	int planeNum;
	ARegionArray * m_plane;

	int hexesX, hexesY;
	int hexSize;
	int hexHalfSize;
	int hexHeight;
	int hexHalfHeight;

	int xscroll;
	int yscroll;
	double lastX;
	double lastY;
	void GetMaxScroll( int & maxX, int & maxY );

	AElemArray * selectedElems;
	AElemArray * tempSelection;
	int curSelection;

	void SpreadSelection( ARegion * pRegion, int spreadBy );

	DECLARE_EVENT_TABLE()
};

// menu items ids
enum
{
	MAP_PLANE_DOWN,
	MAP_PLANE_UP,
	MAP_ZOOM_IN,
	MAP_ZOOM_OUT,
	MAP_SHOW_SHAFTS,
	MAP_SHOW_GATES,
	MAP_SHOW_CITIES,
	MAP_SHOW_OBJECTS,
	MAP_SHOW_COORDS,
	MAP_SHOW_NAMES,
	MAP_SHOW_OUTLINES,
	MAP_LEVEL,
	MAP_SELECT,
};

// spread by...
enum
{
	SPREAD_TERRAIN,
	SPREAD_PROVINCE,
	SPREAD_RACE,
};

#endif
