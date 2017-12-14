#ifndef DRAWPANE_H
#define DRAWPANE_H

#include "wx/wx.h"
#include <vector>

#include "myleakage.h"
#include "mycolors.h"

#include "air/leakage.h"
#include "air/multizone.h"

using namespace std;

enum SelectedTool
{
  NO_TOOL=0,
  ZONE_TOOL=1,
  LEAKAGE_TOOL=2,
  EDIT_TOOL=3
};

enum DrawingTool
{
  NOT_DRAWING=0,
  DRAWING_ZONE=1
};

class DrawPane : public wxPanel
{
public:

  vector<MyZone> zones;
  MyZone temporaryZone;
  vector<MyLeakage> leakages;
  MyLeakage temporaryLeakage;

  DrawPane(wxFrame* parent);

  wxMouseState mouseState;

  SelectedTool currentTool;
  DrawingTool currentDrawingState;
 
  void paintEvent(wxPaintEvent & evt);
  void paintNow();
 
  void render(wxDC& dc);

  void mouseDown(wxMouseEvent& event);
  void mouseUp(wxMouseEvent& event);
  void mouseMoved(wxMouseEvent& event);

  void scrollUp(wxMouseEvent& event);

  
  SelectedTool getToolState();
  void setStateNoTool(wxCommandEvent& event);
  void setStateZoneTool(wxCommandEvent& event);
  void setStateLeakageTool(wxCommandEvent& event);
  void setStateEditTool(wxCommandEvent& event);

  bool displayArrow = false;

private:

  wxPoint mouse;
  wxPoint mouse_down;

  void updateMousePosition();
  bool pointIsInsideZones(wxPoint p, vector<MyZone> z);
  wxPoint snap(wxPoint p, vector<MyZone> z);
  bool zoneIntersectsZones(MyZone t, vector<MyZone> z);
  bool pointOnEdge(wxPoint p, vector<MyZone> z);
  bool pointCloseToPoint(const wxPoint &a, const wxPoint &b, int d);
  bool pointCloseToCorner(const wxPoint &p, const vector<MyZone> z);
  bool pointInsideCircle(const wxPoint &p, const wxPoint &cp, const int &r);
  bool pointInsideTemperatureCircles(wxPoint p, vector<MyZone> &z, int &m);

  // Air
public:
  MultiZone multizone;

/*
	wxMouseState mouseState;

  BasicDrawPane(); 
	void mouseDown(wxMouseEvent& event);
	void mouseMoved(wxMouseEvent& event);
	
  vector<Rectangle> rectangles;
	
  float x=100;
	float y=100;
	int x_last;
	int y_last;*/
 
    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
 
    DECLARE_EVENT_TABLE()
};

#endif
