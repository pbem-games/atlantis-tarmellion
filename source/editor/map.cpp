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
//
// Mapping functionality based on Atlantis Little Helper, by Max
// Shariy


#include "map.h"
#include "gui.h"

#include "../alist.h"
#include "../aregion.h"
#include "config.h"

#include <math.h>
#include "wx/toolbar.h"
#include "bitmaps/arrowup.xpm"
#include "bitmaps/arrowdown.xpm"
#include "bitmaps/map.xpm"

#include "../gamedata.h"

const double cos30 = sqrt( 3 )/2;
const double tan30 = 1/sqrt( 3 );

// ---------------------------------------------------------------------------
// event tables
// ---------------------------------------------------------------------------

BEGIN_EVENT_TABLE( MapChild, wxMDIChildFrame )
	EVT_MENU( MAP_PLANE_DOWN, MapChild::OnPlaneDown )
	EVT_MENU( MAP_PLANE_UP, MapChild::OnPlaneUp )
	EVT_CLOSE( MapChild::OnClose )
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( MapCanvas, wxWindow )
	EVT_MOUSE_EVENTS( MapCanvas::OnEvent )
	EVT_PAINT( MapCanvas::OnPaint )
	EVT_SCROLLWIN( MapCanvas::OnScroll )
	EVT_SIZE( MapCanvas::OnResize )
END_EVENT_TABLE()

// ---------------------------------------------------------------------------
// MapChild
// ---------------------------------------------------------------------------

MapChild::MapChild( wxMDIParentFrame *parent, const wxString& title,
                    const wxPoint& pos, const wxSize& size,
                    const long style )
         : wxMDIChildFrame( parent, -1, title, pos, size,
                            style )
{
	SetIcon( wxIcon( map_xpm ) );

	canvas = ( MapCanvas * ) NULL;
	hexSize = 12;
	SetVirtualSizeHints( 100, 100 );

	CreateToolBar( wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL );
	InitToolBar( GetToolBar() );

	int width, height;

	GetClientSize( &width, &height );
	canvas = new MapCanvas( this, pos, size );

	InitPlane( 1 );

}

void MapChild::InitPlane( int p )
{
	plane = p;

	ARegionArray * pRegArray = app->GetRegions()->pRegionArrays[plane];

	hexesX = pRegArray->x;
	hexesY = pRegArray->y;

	canvas->SetScrollbar( wxVERTICAL, 0, 5, hexesY / 2 );
	canvas->SetScrollbar( wxHORIZONTAL, 0, 5, hexesX * 3/2 );

	canvas->SetPlane( plane, pRegArray );
	canvas->xscroll = 0;
	canvas->yscroll = 0;
	canvas->SetHexSize( hexSize );

	canvas->AdjustScrollBars();
	canvas->hexesX = hexesX;
	canvas->hexesY = hexesY;
	canvas->DrawMap();

	AString title = AString( "Level " ) + p;
	if( pRegArray->strName )
		title += AString(" : ") + *pRegArray->strName;
	SetTitle( title.Str() );
}

void MapChild::InitToolBar( wxToolBar * toolBar )
{
	wxBitmap* bitmaps[2];

	bitmaps[0] = new wxBitmap( arrowup_xpm );
	bitmaps[1] = new wxBitmap( arrowdown_xpm );

	toolBar->AddTool( MAP_PLANE_UP, "", *( bitmaps[0] ), "Up one plane" );
	toolBar->AddTool( MAP_PLANE_DOWN, "", *( bitmaps[1] ), "Down one plane" );
	if( plane >= app->GetRegions()->numLevels -1 )
		toolBar->EnableTool( MAP_PLANE_DOWN, false );
	if( plane <=0 )
		toolBar->EnableTool( MAP_PLANE_UP, true );
	toolBar->Realize();
	int i;
	for( i = 0; i < 2; i++ )
		delete bitmaps[i];

}

void MapChild::OnPlaneUp( wxCommandEvent & event )
{
	int plane = canvas->planeNum;
	if( plane <= 0 ) return;

	plane--;
	GetToolBar()->EnableTool( MAP_PLANE_DOWN, true );
	if( plane >= app->GetRegions()->numLevels -1 )
		GetToolBar()->EnableTool( MAP_PLANE_UP, false );
	app->DeselectAllRegions();
	InitPlane( plane );

}

void MapChild::OnPlaneDown( wxCommandEvent & event )
{
	int plane = canvas->planeNum;
	if( plane >= app->GetRegions()->numLevels -1 ) return;

	plane++;

	GetToolBar()->EnableTool( MAP_PLANE_UP, true );
	if( plane <= 0 ) GetToolBar()->EnableTool( MAP_PLANE_DOWN, false );
	app->DeselectAllRegions();
	InitPlane( plane );

}

void MapChild::OnClose( wxCloseEvent & event )
{
	// stop user from closing this window
}

// ---------------------------------------------------------------------------
// MapCanvas
// ---------------------------------------------------------------------------

// Define a constructor for map canvas
MapCanvas::MapCanvas( wxWindow *parent, const wxPoint& pos, const wxSize& size)
		: wxWindow( parent, -1, pos, size,
						   wxSUNKEN_BORDER |
						   wxVSCROLL | wxHSCROLL )
{
	m_AtlaX0 = 0;
	m_AtlaY0 = 0;
	yscroll = 0;
	xscroll = 0;

	SetBackgroundColour( wxColour( _T( "BLACK" ) ) );
}

void MapCanvas::SetHexSize( int a )
{
	hexSize = a;
	hexHalfSize   = hexSize / 2;
	hexHalfHeight = ( int )( hexSize * cos30 + 0.5 );	 
	hexHeight	  = hexHalfHeight * 2;

	int width, height;
	long CX,CY,CXOld,CYOld;

	GetClientSize( &width, &height );
	WinToAtla( width/2, height/2, CXOld, CYOld );
	CX = ( long )( ( double )CXOld * ( double )hexSize		 / ( double )hexSize	   + 0.5 );
	CY = ( long )( ( double )CYOld * ( double )hexHalfHeight / ( double )hexHalfHeight + 0.5 );

	m_AtlaX0 = CX - width/2;
	m_AtlaY0 = CY - height/2;

}

void MapCanvas::SetPlane( int plane, ARegionArray * pAArray )
{
	planeNum = plane;
	m_plane = pAArray;
}

void MapCanvas::DrawMap()
{
	wxClientDC dc( this );

	DrawMap( &dc );
}

void MapCanvas::DrawMap( wxDC * pDC )
{
	if( !app->GetRegions() )
		return;

	int width,height;
	GetClientSize( &width, &height );
	wxRect rect( 0, 0, width, height );

	pDC->SetClippingRegion( rect );
	forlist( app->GetRegions() ) {
		ARegion * r = ( ARegion * ) elem;
		if( r->zloc == planeNum )
			DrawHex( r, pDC, 0 );
	}

	DrawBorders( pDC );
}

void MapCanvas::DrawBorders( wxDC * pDC )
{
	int width,height;
	GetClientSize( &width, &height );

	int x0, y0;

	//black borders first
	{
		forlist( app->GetRegions() ){
			ARegion * r = ( ARegion * ) elem;
			if( r->zloc == planeNum ) {
				GetHexCenter( r->xloc, r->yloc, x0, y0 );
				if( app->selectedRegions->Index( r ) != wxNOT_FOUND )
					DrawAHex( x0, y0, wxBLACK_PEN, wxTRANSPARENT_BRUSH, pDC,
					          r->neighbors );
			}
		}
	}

	//now hilighted borders
	for( int i = 0; i < abs( app->selectedRegions->GetCount() ); i++ ) {
		ARegion * r = ( ARegion * ) app->selectedRegions->Item( i );
		if( r->zloc == planeNum ) {
			GetHexCenter( r->xloc, r->yloc, x0, y0 );
			DrawAHex( x0, y0, wxWHITE_PEN, wxTRANSPARENT_BRUSH, pDC,
				      r->neighbors );
		}
	}

}

void MapCanvas::SelectRegion( ARegion * pRegion, bool redrawBorders )
{
	wxClientDC dc( this );

	DrawHex( pRegion, &dc, 0 );

	if( redrawBorders ) {
		DrawBorders( &dc );
	}
}

void MapCanvas::DeselectRegion( ARegion * pRegion, bool redrawBorders )
{
	wxClientDC dc( this );
	DrawHex( pRegion, &dc, 1 );
	if( redrawBorders ) {
		DrawBorders( &dc );
	}
}

void MapCanvas::DrawHex( ARegion * pRegion, wxDC * pDC, int deselect )
{
	// deselect == 0  : no change
	// deselect == 1  : hex is being deselected

	wxBrush terrBrush;
	wxPen borderPen;

	// Get Hex Center
	if( !pRegion ) return;

	int x0, y0;
	GetHexCenter( pRegion->xloc, pRegion->yloc, x0, y0 );

	int height,width;
	GetClientSize( &width,&height );

	if( x0 < 0 - hexSize || x0 > width + hexSize ||
		y0 < 0 - hexHeight || y0 > height + hexHeight )
	{
		return;
	}

	borderPen.SetColour( wxColour( _T( "BLACK" ) ) );

	if( !deselect )
	{
		// Edge hexes - terrain only
		int x1,y1;
		ARegion * edgeHex;

		ARegion **exits;
		if( pRegion->xloc == 0 ) {
			//draw SW and NW neighbours
			edgeHex = pRegion->neighbors[4];// SW
			x1 = x0 - hexSize *3/2;
			y1 = y0 + hexHalfHeight;
			// kludge: don't draw nexus neighbours
			if( edgeHex && edgeHex->zloc == pRegion->zloc ) {
				SetTerrainBrush( edgeHex->type, terrBrush, 0 );
				exits = edgeHex->neighbors;
			} else {
				terrBrush.SetColour( wxColour( _T( "BLACK" ) ) );
				exits = NULL;
			}
			DrawAHex( x1, y1, &borderPen, &terrBrush, pDC, exits );
	
			edgeHex = pRegion->neighbors[5];// NW
			y1 = y0 - hexHalfHeight;
			if( edgeHex && edgeHex->zloc == pRegion->zloc ) {
				SetTerrainBrush( edgeHex->type, terrBrush, 0 );
				exits = edgeHex->neighbors;
			} else {
				terrBrush.SetColour( wxColour( _T( "BLACK" ) ) );
				exits = NULL;
			}
			DrawAHex( x1, y1, &borderPen, &terrBrush, pDC, exits );
		}
		if( pRegion->xloc == m_plane->x-1 && 
			m_plane->x-1 * hexSize *3/2 < width ) {
			//draw NE and SE neighbours
			edgeHex = pRegion->neighbors[1];// NE

			x1 = x0 + hexSize*3/2;
			y1 = y0 - hexHalfHeight;
			if( edgeHex && edgeHex->zloc == pRegion->zloc ) {
				SetTerrainBrush( edgeHex->type, terrBrush, 0 );
				exits = edgeHex->neighbors;
			} else {
				terrBrush.SetColour( wxColour( _T( "BLACK" ) ) );
				exits = NULL;
			}
			DrawAHex( x1, y1, &borderPen, &terrBrush, pDC, exits );
			edgeHex = pRegion->neighbors[2];// SE
			y1 = y0 + hexHalfHeight;
			if( edgeHex && edgeHex->zloc == pRegion->zloc ) {
				SetTerrainBrush( edgeHex->type, terrBrush, 0 );
				exits = edgeHex->neighbors;
			} else {
				terrBrush.SetColour( wxColour( _T( "BLACK" ) ) );
				exits = NULL;
			}
			DrawAHex( x1, y1, &borderPen, &terrBrush, pDC, exits );
		}
		if( pRegion->yloc == 1 ) {
			//draw N neighbour
			edgeHex = pRegion->neighbors[0];// N
			x1 = x0;
			y1 = y0 - hexHeight;
			if( edgeHex && edgeHex->zloc == pRegion->zloc ) {
				SetTerrainBrush( edgeHex->type, terrBrush, 0 );
				exits = edgeHex->neighbors;
			} else {
				terrBrush.SetColour( wxColour( _T( "BLACK" ) ) );
				exits = NULL;
			}
			DrawAHex( x1, y1, &borderPen, &terrBrush, pDC, exits );
		}
		if( pRegion->yloc == m_plane->y-2 &&
			m_plane->y-1 * hexHeight < height ) {
			//draw S neighbour
			edgeHex = pRegion->neighbors[3];// S
			x1 = x0;
			y1 = y0 + hexHeight;
			if( edgeHex && edgeHex->zloc == pRegion->zloc ) {
				SetTerrainBrush( edgeHex->type, terrBrush, 0 );
				exits = edgeHex->neighbors;
			} else {
				terrBrush.SetColour( wxColour( _T( "BLACK" ) ) );
				exits = NULL;
			}
			DrawAHex( x1, y1, &borderPen, &terrBrush, pDC, exits );
		}
	}

	// Hex
	int hilite = 0;
	if( !deselect && ( app->selectedRegions->Index( pRegion ) != wxNOT_FOUND ))
		hilite = 1;

	SetTerrainBrush( pRegion->type, terrBrush, hilite );

	DrawAHex( x0, y0, &borderPen, &terrBrush, pDC, pRegion->neighbors );

	// Symbols
	int SymLeft   = x0 - hexHalfSize;
	int SymBottom = y0 + hexHalfHeight - 2;

	//	City	
	if( pRegion->town )
		DrawTown( x0, y0, pDC );
	   
	//	Gate
	if( pRegion->gate )
	{
		DrawGate( SymLeft, SymBottom, pDC );
		GetNextIconPos( x0, y0, SymLeft, SymBottom );
	}

	// Structures
	forlist( &pRegion->objects ) {
		Object * o = ( Object * ) elem;
		if( o->IsRoad() ) {
			DrawRoad( x0, y0, pDC, o );
			if( o->type == O_SHAFT ) {
				DrawShaft( SymLeft, SymBottom, pDC );			 
				GetNextIconPos( x0, y0, SymLeft, SymBottom );
			}
			else if( o->IsBoat() ) {
				DrawBoat( SymLeft, SymBottom, pDC );
				GetNextIconPos( x0, y0, SymLeft, SymBottom );
			}
			else if( o->type != O_DUMMY ) {
				DrawOther( SymLeft, SymBottom, pDC );
				GetNextIconPos( x0, y0, SymLeft, SymBottom );
			}
		}
	}

}

void MapCanvas::DrawAHex( int x, int y, wxPen *pen, wxBrush *brush, wxDC *pDC,
						  ARegion * exits[] )
{
	wxPoint 	  point[7]	=  
	{ 
		wxPoint( x - hexHalfSize, y - hexHalfHeight ),	 //1
		wxPoint( x + hexHalfSize, y - hexHalfHeight ),	 //2
		wxPoint( x + hexSize	, y 				  ),   //3
		wxPoint( x + hexHalfSize, y + hexHalfHeight ),	 //4
		wxPoint( x - hexHalfSize, y + hexHalfHeight ),	 //5
		wxPoint( x - hexSize	, y 				  ),	//6
		wxPoint( x - hexSize	, y 				  )   //0
	};

	pDC->SetPen( *pen );
	pDC->SetBrush( *brush );
	pDC->DrawPolygon( 7, point, 0, 0, wxWINDING_RULE );

	wxPen pen2( pen->GetColour(), 3, wxSOLID );
	pDC->SetPen( pen2 );
	for( int i = 0; i < 6; i++ )	{
		if( exits && exits[i] == 0 ) {
			pDC->DrawLine( point[i].x, point[i].y, point[i+1].x, point[i+1].y);
		}
	}
	pDC->SetPen( *pen );
}

void MapCanvas::OnScroll( wxScrollWinEvent& event )
{
	int curX, curY, oldX, oldY;

	oldX = curX = GetScrollPos( wxHORIZONTAL );
	oldY = curY = GetScrollPos( wxVERTICAL );

	
	if( event.m_eventType == wxEVT_SCROLLWIN_LINEUP )
	{
		if( event.GetOrientation() == wxHORIZONTAL ) 
			curX -= 1;
		else
			curY -= 1;
	}
	else if( event.m_eventType == wxEVT_SCROLLWIN_LINEDOWN )
	{
		if( event.GetOrientation() == wxHORIZONTAL ) 
			curX += 1;
		else
			curY += 1;
	}
	else if( event.m_eventType == wxEVT_SCROLLWIN_PAGEUP )
	{
		if( event.GetOrientation() == wxHORIZONTAL )
			curX -= GetScrollThumb( wxHORIZONTAL );
		else
			curY -= GetScrollThumb( wxVERTICAL );
	}
	else if( event.m_eventType == wxEVT_SCROLLWIN_PAGEDOWN )
	{
		if( event.GetOrientation() == wxHORIZONTAL )
			curX += GetScrollThumb( wxHORIZONTAL );
		else
			curY += GetScrollThumb( wxVERTICAL );
	}
	else if( event.m_eventType == wxEVT_SCROLLWIN_THUMBTRACK )
	{
		if( event.GetOrientation() == wxHORIZONTAL )
			curX = event.GetPosition();
		else
			curY = event.GetPosition();
	}

	int maxX = GetScrollRange( wxHORIZONTAL ) - GetScrollThumb( wxHORIZONTAL );
	int maxY = GetScrollRange( wxVERTICAL ) - GetScrollThumb( wxVERTICAL );

	if( curX < 0 ) curX = 0;
	if( curX > maxX ) curX = maxX;
	if( curY < 0 ) curY = 0;
	if( curY > maxY ) curY = maxY;

	SetScrollPos( wxHORIZONTAL, curX );
	SetScrollPos( wxVERTICAL, curY );

	xscroll = curX * hexSize;
	yscroll = curY * hexHeight;

	DrawMap();
}

void MapCanvas::OnEvent( wxMouseEvent& event )
{
	long xpos,ypos;
	int hexX, hexY;

	xpos = event.GetX();
	ypos = event.GetY();
	GetHexCoord( hexX, hexY, xpos, ypos );

	ARegion * pRegion;

	if( hexX < 0 || hexX > m_plane->x-1 ||
		hexY < 0 || hexY > m_plane->y-1 ) pRegion = NULL;
	else
		pRegion = app->GetRegions()->GetRegion( hexX, hexY, planeNum );

	app->UpdateStatusBar( pRegion );

	if( event.LeftDown() && pRegion )
	{
		wxClientDC dc( this );
		if( event.ShiftDown() || event.ControlDown() ) {
			app->SelectRegion( pRegion, true );
		} else {		
			app->SelectRegion( pRegion, false );
		}
	}

}

void MapCanvas::GetHexCenter( int NoX, int NoY, int & WinX, int & WinY )
{
	// center of ( 0,0 ) hex has 0,0 Atla coordinates

	long AtlaX, AtlaY;

	AtlaX = NoX * hexSize * 3 / 2 + hexSize;
	AtlaY = NoY * hexHalfHeight + hexHalfHeight;

	AtlaToWin( WinX, WinY, AtlaX, AtlaY );

}

void MapCanvas::GetHexCoord( int & NoX, int & NoY, int WinX, int WinY )
{
	// center of ( 0,0 ) hex has 0,0 Atla coordinates

	long AtlaX, AtlaY;
	int  ApprX, ApprY; //approximite hex no
	int  x, y, x0, y0;
	int  xx, yy;
	xx = -1;  yy = -1;

	WinToAtla( WinX, WinY, AtlaX, AtlaY );

	ApprX = ( AtlaX - hexSize ) / ( hexSize * 3 / 2 ); // hexSize must be even!
	ApprY = ( AtlaY - hexHalfHeight ) / hexHalfHeight;

	int rise, run;
	double maxdist = hexSize *200;
	double dist;
	for( x = ApprX-1; x <= ApprX+1; x++ )
	{
		for( y = ApprY-2; y <= ApprY+2; y++ )
		{
			if( ValidHexNo( x,y ) )
			{
				GetHexCenter( x,y,x0,y0 );

				// find distance from this hex center to the map point
				rise = ( x0 - WinX ) ;
				run = ( y0 - WinY );
				dist = sqrt( rise*rise + run*run );


				if( dist<maxdist ) {
					xx = x0; yy = y0; NoX = x; NoY = y; maxdist = dist;
				}

			}
		}
	}
}

int MapCanvas::ValidHexNo( int NoX, int NoY )
{	
	// sum of coordinates must be even
	int x = NoX + NoY;

	return x == ( ( x>>1 )<<1 );
}

inline void MapCanvas::WinToAtla( int	 WinX,  int    WinY,
								  long & AtlaX, long & AtlaY )
{
	AtlaX = WinX + m_AtlaX0 + xscroll;
	AtlaY = WinY + m_AtlaY0 + yscroll;
}

inline void MapCanvas::AtlaToWin( int &  WinX,  int &  WinY,
								  long   AtlaX, long   AtlaY )
{
	WinX = AtlaX - m_AtlaX0 - xscroll;
	WinY = AtlaY - m_AtlaY0 - yscroll;
}

void MapCanvas::OnPaint( wxPaintEvent &WXUNUSED( event ) )
{
	wxPaintDC dc( this );
	DrawMap( &dc );
}


void MapCanvas::DrawTown( int x0, int y0, wxDC * pDC )
{
	pDC->SetPen  ( *wxBLACK_PEN );
	pDC->SetBrush( *wxWHITE_BRUSH );
	pDC->DrawArc( x0 - 4, y0, x0 - 4, y0, x0, y0 );
	pDC->DrawArc( x0 - 3, y0, x0 - 3, y0, x0, y0 );
}

void MapCanvas::DrawRoad( int x0, int y0, wxDC * pDC, Object * obj )
{

	wxPen roadPen;
	int x2,y2;

	roadPen.SetColour( 112, 128, 144 );
	if( obj->incomplete ) {
		wxDash dash[2];
		dash[0] = 1; dash[1] = 2;
		roadPen.SetDashes( 2, dash );
	}
		
	pDC->SetPen( roadPen );

	switch ( obj->type ) {
	case O_ROADN:
		pDC->DrawLine( x0, y0, x0, y0 - hexHalfHeight );
		break;
	case O_ROADS:
		pDC->DrawLine( x0, y0, x0, y0 + hexHalfHeight );
		break;
	case O_ROADSE:
		x2 = x0 + hexHalfSize * 3 / 2; 
		y2 = y0 + hexHalfHeight / 2;
		pDC->DrawLine( x0, y0, x2, y2 );
		break;
	case O_ROADSW:
		x2 = x0 - hexHalfSize * 3 / 2; 
		y2 = y0 + hexHalfHeight / 2;
		pDC->DrawLine( x0, y0, x2, y2 );
		break;
	case O_ROADNE:
		x2 = x0 + hexHalfSize * 3 / 2; 
		y2 = y0 - hexHalfHeight / 2;
		pDC->DrawLine( x0, y0, x2, y2 );
		break;
	case O_ROADNW:
		x2 = x0 - hexHalfSize * 3 / 2; 
		y2 = y0 - hexHalfHeight / 2;
		pDC->DrawLine( x0, y0, x2, y2 );
		break;
	}
}

void MapCanvas::GetNextIconPos( int x0, int y0, int & x, int & y )
{
	x += ( ICON_SIZE+1 );

	// assume we never get out of the lower part of a hex
		
	if( x + ICON_SIZE > x0 + hexHalfSize +
		( long )( tan30*( y0 + hexHalfHeight - y ) ) ) 
	{
		y -= ( ICON_SIZE+1 );
		x = x0 - hexHalfSize - ( long )( tan30*( y0 + hexHalfHeight - y ) );
	}
}

void MapCanvas::DrawGate( int x0, int y0, wxDC * pDC )
{
	pDC->SetPen ( *wxBLACK_PEN );
	pDC->SetBrush( *wxBLACK_BRUSH );
	pDC->DrawRectangle( x0, y0 - ICON_SIZE, ICON_SIZE, ICON_SIZE );

	pDC->SetBrush( *wxWHITE_BRUSH );
	pDC->DrawArc( x0 + ICON_SIZE / 2, y0, 
				 x0 + ICON_SIZE / 2, y0, 
				 x0 + ICON_SIZE / 2, y0 - ICON_SIZE / 2 );
}

void MapCanvas::DrawShaft( int x0, int y0, wxDC * pDC )
{
	pDC->SetPen  ( *wxBLACK_PEN );
	pDC->DrawLine( x0, y0 - ICON_SIZE, x0, y0 );
	pDC->DrawLine( x0 + ICON_SIZE, y0 - ICON_SIZE, x0 + ICON_SIZE, y0 );

	pDC->DrawLine( x0, y0 - 3, x0 + ICON_SIZE, y0 - 3 );
	pDC->DrawLine( x0, y0 - ICON_SIZE +2, x0 + ICON_SIZE, y0 - ICON_SIZE +2 );
}

void MapCanvas::DrawBoat( int x0, int y0, wxDC * pDC )
{
	wxPoint tri[3] = 
	{
		wxPoint( x0, y0 - ICON_SIZE ),
		wxPoint( x0 + ICON_SIZE / 2, y0 - 1 ),
		wxPoint( x0 + ICON_SIZE, y0 - ICON_SIZE )
	};
	pDC->SetPen  ( *wxBLACK_PEN );
	pDC->SetBrush( *wxBLACK_BRUSH );
	pDC->DrawPolygon( 3, tri, 0, 0, wxWINDING_RULE );
}

void MapCanvas::DrawOther( int x0, int y0, wxDC * pDC )
{
	pDC->SetPen  ( *wxWHITE_PEN );
	pDC->DrawRectangle( x0, y0 - ICON_SIZE, ICON_SIZE, ICON_SIZE );
}

void MapCanvas::AdjustScrollBars()
{
	int width, height;
	GetClientSize( &width,&height );

	SetScrollbar( wxVERTICAL, GetScrollPos( wxHORIZONTAL ), height / hexHeight,
		          GetScrollRange( wxVERTICAL ) );
	SetScrollbar( wxHORIZONTAL, GetScrollPos( wxHORIZONTAL ),
		          width / ( hexSize ), GetScrollRange( wxHORIZONTAL ) );
}

void MapCanvas::OnResize( wxSizeEvent& event )
{
	AdjustScrollBars();
}

void MapCanvas::RedrawBorders()
{
	wxClientDC dc( this );
	DrawBorders( &dc );
}
