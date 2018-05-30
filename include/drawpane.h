#ifndef DRAWPANE_H
#define DRAWPANE_H

#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include <vector>

#include "myleakage.h"
#include "mycolors.h"
#include "elements.h"
#include "drawpaneFunctions.h"

#include "../../airflow/lib/leakage.h"
#include "../../airflow/lib/multizone.h"

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

  // Create Elements that contains all drawn elements
  Elements elements;

  wxMouseState mouseState;

  SelectedTool currentTool;
  DrawingTool currentDrawingState;
 
  void paintEvent(wxPaintEvent & evt);
	void eraseEvent(wxEraseEvent& evt);
  void paintNow();
 
  void render(wxDC &dc);

  void drawHelpLines(wxDC &dc);

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
  bool displayPressureVertical = false;

private:

  wxPoint mouse;
  wxPoint mouse_down;

  void updateMousePosition();

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
