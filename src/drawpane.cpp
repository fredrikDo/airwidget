#include "drawpane.h"

DrawPane::DrawPane(wxFrame* parent) : wxPanel(parent)
{
  // Make drawing pane backround color white
  //SetBackgroundColour(*wxGREEN);
  SetBackgroundColour(MyColors::COLOR_DRAWPANE_BACKGROUND);


  // State when no tool is selected
  this -> currentTool = NO_TOOL;

  // Add boundary
  multizone.addFixedPressure(0);
}

void DrawPane::updateMousePosition()
{
  mouseState = wxGetMouseState();

  // Update mouse position relative to window
  mouse.x = mouseState.GetX() - GetScreenPosition().x;
  mouse.y = mouseState.GetY() - GetScreenPosition().y;
}

bool DrawPane::pointIsInsideZones(wxPoint p, vector<MyZone> z)
{
  // Check if point is inside vector of zones and return
  // true or false

  bool r = false;
  
  for (int i=0; i<z.size(); i++)
  {
    if (z[i].Contains(p))
    {
      r = true;
      break;
    }
  }
  return r;
}

bool DrawPane::zoneIntersectsZones(MyZone t, vector<MyZone> z)
{
  bool r = false;

  t.allPositive();
  t.Deflate(1, 1);

  for (int i=0; i<z.size(); i++)
  {
    if (t.Intersects(z[i]))
    {
      r = true;
      break;
    }
  }
  return r;
}

wxPoint DrawPane::snap(wxPoint p, vector<MyZone> z)
{
  int delta = 5;

  for (int i=0; i<z.size(); i++)
  {
    // x-direction
    if (abs(p.x - z[i].GetLeft()) < delta)
    {
      p.x = z[i].GetLeft();
    }
    else if (abs(p.x - z[i].GetRight()) < delta)
    {
      p.x = z[i].GetRight();
    }
    // y-direction
    if (abs(p.y - z[i].GetTop()) < delta)
    {
      p.y = z[i].GetTop();
    }
    else if (abs(p.y - z[i].GetBottom()) < delta)
    {
      p.y = z[i].GetBottom();
    }
  }
  return p;
}

bool DrawPane::pointOnEdge(wxPoint p, vector<MyZone> z)
{
  bool r = false;

  for (int i=0; i<z.size(); i++)
  {
    if (z[i].Contains(p))
    {
      if (p.x == z[i].GetLeft()
            || p.x == z[i].GetRight()
            || p.y == z[i].GetTop()
            || p.y == z[i].GetBottom())
      {
        r = true;
      }
    }
  }
  return r;
}

bool DrawPane::pointCloseToPoint(const wxPoint &a, const wxPoint &b, int d)
{
  bool r = false;

  if (pow(a.x - b.x, 2) + pow(a.y - b.y, 2) < pow(d, 2))
  {
    r = true;
  }
  return r;
}

bool DrawPane::pointCloseToCorner(const wxPoint &p, const vector<MyZone> z)
{
  bool r = false;
  int d = 5;

  for (int i=0; i<z.size(); i++)
  {
    if (pointCloseToPoint(p, z[i].GetTopLeft(), d)
          || pointCloseToPoint(p, z[i].GetTopRight(), d)
          || pointCloseToPoint(p, z[i].GetBottomLeft(), d)
          || pointCloseToPoint(p, z[i].GetBottomRight(), d))
    {
      r = true;
    }
  }
  return r;
}

bool DrawPane::pointInsideCircle(const wxPoint &p, const wxPoint &cp, const int &r)
{
  bool ret = false;

  if (pow(p.x - cp.x, 2) + pow(p.y - cp.y, 2) < r * r)
  {
    ret = true;
  }
  return ret;
}

bool DrawPane::pointInsideTemperatureCircles(wxPoint p, vector<MyZone> &zones, int &m)
{
  // See if point is inside temperature circle of any of the zones
  // and returns zone number

  bool r = false;

  for (int i=0; i<zones.size(); i++)
  {
    if (pointInsideCircle(p, zones[i].getCenter(), 15))
    {
      r = true; //zones[i].setMouseOnTemperature(true);
      m = i;
    }
    /*
    else
    {
      zones[i].setMouseOnTemperature(false);
    }*/
  }
  return r;
}

void DrawPane::paintEvent(wxPaintEvent & evt)
{
  wxPaintDC dc(this);
  render(dc);
}

void DrawPane::paintNow()
{
  wxClientDC dc(this);
  render(dc);
}

void DrawPane::render(wxDC& dc)
{
    // Solve
    if (!zones.empty() && leakages.size() > 1)
    {
      multizone.solve();
    }

    // draw some text
    //dc.DrawText(wxT("Testing"), 40, 60); 
 
    // draw a circle
    //dc.SetBrush(*wxGREEN_BRUSH); // green filling
    //dc.SetPen( wxPen( wxColor(255,0,0), 5 ) ); // 5-pixels-thick red outline
    //dc.DrawCircle( wxPoint(200,100), 25 /* radius */ );
 
    // draw a rectangle
    //dc.SetBrush(*wxBLUE_BRUSH); // blue filling
  dc.SetPen( wxPen( MyColors::COLOR_ZONE_LINE )); // 10-pixels-thick pink outline
  dc.SetBrush(MyColors::COLOR_ZONE_BACKGROUND);

  for (int i=0; i<zones.size(); i++)
  {
    //dc.DrawRectangle(zones[i].x, zones[i].y, zones[i].w, zones[i].h);
    //dc.DrawRectangle(wxRect(zones[i].start, zones[i].end));
    //dc.DrawRectangle(zones[i]);
    zones[i].draw(dc);
    zones[i].drawTemperature(multizone.getZoneTemperature(i), dc);
  }

  // Draw leakages
  double maxFlow = multizone.getMaxLeakageMassFlow();
  for (int i=0; i<leakages.size(); i++)
  {
    leakages[i].draw(dc);
    leakages[i].drawArrow(dc, multizone.getLeakageMassflow(i), maxFlow);
  }

  switch(currentTool)
  {
    case NO_TOOL:
      // Do nothing!
      break;
    case ZONE_TOOL:
      switch(currentDrawingState)
      {
        case NOT_DRAWING:
          // Do nothing!
          break;
        case DRAWING_ZONE:
          
          // Color red if intersecting with other zones
          if (zoneIntersectsZones(temporaryZone, zones))
          {
            temporaryZone.drawIllegal(dc);
          }
          else
          {
            // Draw temporaryZone
            temporaryZone.draw(dc);
          }
          break;
      }
      break;
    case LEAKAGE_TOOL:
      temporaryLeakage.draw(dc);
      break;
    case EDIT_TOOL:
      break;
  }

  //dc.DrawRectangle(10, 10, 50, 50);
    // draw a line
    //dc.SetPen( wxPen( wxColor(0,0,0), 3 ) ); // black line, 3 pixels thick
    //dc.DrawLine( 300, 100, 700, 300 ); // draw line across the rectangle

    // Look at the wxDC docs to learn how to draw other stuff
}

void DrawPane::mouseDown(wxMouseEvent& event)
{
  updateMousePosition();  

  currentDrawingState = DRAWING_ZONE;

  switch(currentTool)
  {
    case NO_TOOL:
      // Do nothing!
      break;
    case ZONE_TOOL:
      // If mouse is not inside any zone then
      // start drawing a new zone
      if (!pointIsInsideZones(mouse, zones))
      {
        //temporaryZone.start = mouse;
        temporaryZone.SetTopLeft(mouse);

        // Save mouse coordinates
        mouse_down = mouse;
      }
      // If mouse is inside a zone then set
      // drawing state to NOT_DRAWING
      else
      {
        currentDrawingState = NOT_DRAWING;
      }
      break;
    case LEAKAGE_TOOL:
      if (pointOnEdge(temporaryLeakage.getPosition(), zones)
            && !pointCloseToCorner(temporaryLeakage.getPosition(), zones))
      {
        // Update connection
        temporaryLeakage.relateToZones(zones);
        // Add leakage to airflow-model
        multizone.addLeakage(temporaryLeakage.h, 1.0, 0.65,
          {temporaryLeakage.con1, temporaryLeakage.con2},
          {temporaryLeakage.t1, temporaryLeakage.t2});
        leakages.push_back(temporaryLeakage);
      }
      break;
    case EDIT_TOOL:
      break;
  }
}

void DrawPane::mouseUp(wxMouseEvent& event)
{
  switch(currentTool)
  {
    case NO_TOOL:
      break;

    case ZONE_TOOL:

      // If mouse moved since mouse_down then add new zone
      // to list of zones
      if (mouse.x != mouse_down.x && mouse.y != mouse_down.y
          && !zoneIntersectsZones(temporaryZone, zones))
      {
        temporaryZone.allPositive();
        zones.push_back(temporaryZone);
        
        // Add airflowzone
        // 20 degC temperature
        multizone.addZone(temporaryZone.GetHeight(), 293);
        //cout << temporaryZone.GetHeight() << endl << endl;
      }
      // Stop drawing the zone
      currentDrawingState = NOT_DRAWING;
      break;

    case LEAKAGE_TOOL:
      break;
    case EDIT_TOOL:
      break;
  }
}

void DrawPane::mouseMoved(wxMouseEvent& event)
{
  updateMousePosition();

  switch(currentTool)
  {
    case NO_TOOL:
      // Do nothing
      break;

    case ZONE_TOOL:
      switch(currentDrawingState)
      {
        case NOT_DRAWING:
          // Do nothing!
          break;
        case DRAWING_ZONE:
          mouse = snap(mouse, zones); 
          temporaryZone.SetBottomRight(mouse);
          break;
      }
    case LEAKAGE_TOOL:
      mouse = snap(mouse, zones);
      temporaryLeakage.setPosition(mouse);
      break;
    case EDIT_TOOL:
      int i = 0;
      if (pointInsideTemperatureCircles(mouse, zones, i))
      {
        zones[i].setMouseOnTemperature(true);
      }
      else
      {
        zones[i].setMouseOnTemperature(false);
      }
      break;
  }
  Refresh();
}

SelectedTool DrawPane::getToolState()
{
  return currentTool;
}

void DrawPane::scrollUp(wxMouseEvent& event)
{
  switch(currentTool)
  {
    case NO_TOOL:
      break;
    case ZONE_TOOL:
      break;
    case LEAKAGE_TOOL:
      break;
    case EDIT_TOOL:
      int i = 0;
      if (pointInsideTemperatureCircles(mouse, zones, i))
      {
        multizone.adjustZoneTemperature(event.GetWheelRotation(), i);
      }
      break;
  }
  Refresh();
}

void DrawPane::setStateNoTool(wxCommandEvent& event)
{
  this -> currentTool = NO_TOOL;
}

void DrawPane::setStateZoneTool(wxCommandEvent& event)
{
  // Switch to drawing tool
  this -> currentTool = ZONE_TOOL;
  
  this -> currentDrawingState = NOT_DRAWING;
}

void DrawPane::setStateLeakageTool(wxCommandEvent& event)
{
  this -> currentTool = LEAKAGE_TOOL;
}

void DrawPane::setStateEditTool(wxCommandEvent& event)
{
  this -> currentTool = EDIT_TOOL;
}
