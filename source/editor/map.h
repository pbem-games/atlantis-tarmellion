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

#ifndef MAP_CLASS
#define MAP_CLASS

#include "../aregion.h"

#include "wx/wx.h"

#define ICON_SIZE		6
#define ICON_SIZE_SMALL 4

class MapCanvas : public wxWindow
{
public:
	MapCanvas( wxWindow *parent, const wxPoint& pos, const wxSize& size );

	void SetPlane( int, ARegionArray * );
	void SetHexSize( int );
	void AdjustScrollBars();

	void GetHexCenter( int NoX, int NoY, int & WinX, int & WinY );
	void GetHexCoord( int & NoX, int & NoY, int WinX, int WinY );

	void DrawMap();
	void SelectRegion( ARegion *, bool );
	void DeselectRegion( ARegion *, bool );
	void RedrawBorders();


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

	long m_AtlaX0; // corresponds to WinX=0
	long m_AtlaY0; // corresponds to WinY=0

protected:
	void OnEvent( wxMouseEvent& );
	void OnScroll( wxScrollWinEvent& );
	void OnResize( wxSizeEvent& );
	void OnPaint( wxPaintEvent& );

	int ValidHexNo( int NoX, int NoY );
	void WinToAtla( int   WinX, int   WinY, long & AtlaX, long & AtlaY );
	void AtlaToWin( int & WinX, int & WinY, long   AtlaX, long	 AtlaY );
	void GetNextIconPos( int x0, int y0, int & x, int & y );

	void DrawMap( wxDC * );
	void DrawHex( ARegion *, wxDC *, int );//int NoX, int NoY, wxDC * pDC, CLand * pLand, CPlane * pPlane, wxRect * prect*;
	void DrawAHex( int, int, wxPen *, wxBrush *, wxDC *, ARegion * exits[] = NULL );
	void DrawBorders( wxDC * );
	void DrawTown( int, int, wxDC * );
	void DrawRoad( int, int, wxDC *, Object * );
	void DrawGate( int, int, wxDC * );
	void DrawShaft( int, int, wxDC * );
	void DrawBoat( int, int, wxDC * );
	void DrawOther( int, int, wxDC * );

		DECLARE_EVENT_TABLE()
};

class MapChild: public wxMDIChildFrame
{
public:
	MapChild( wxMDIParentFrame *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style );

	void OnActivate( wxActivateEvent & );
	void OnRefresh( wxCommandEvent & );
	void InitPlane( int );
	void InitToolBar( wxToolBar * );

	void OnPlaneDown( wxCommandEvent & );
	void OnPlaneUp( wxCommandEvent & );
	void OnClose( wxCloseEvent & );

	MapCanvas * canvas;

protected:
	int plane;
	int hexesX;
	int hexesY;
	int hexSize;

	DECLARE_EVENT_TABLE()
};

// menu items ids
enum
{
	MAP_PLANE_DOWN,
	MAP_PLANE_UP,
};

#endif