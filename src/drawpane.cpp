#include "wx/wx.h"

#include "drawpane.h"

void MyLeakage::setPosition(wxPoint p)
{
  x = p.x;
  y = p.y;
}

wxPoint MyLeakage::getPosition()
{
  wxPoint r;
  r.x = x;
  r.y = y;

  return r;
}

void MyZone::allPositive()
{
  if (this -> IsEmpty())
  {
    wxPoint fst = GetTopLeft();
    wxPoint snd = GetBottomRight();

    int w = GetWidth();
    int h = GetHeight();

    if (w < 0 && h > 0)
    {
      SetTopLeft(wxPoint(snd.x, fst.y));
      SetBottomRight(wxPoint(fst.x, snd.y));
    }
    else if (w > 0 && h < 0)
    {
      SetTopLeft(wxPoint(fst.x, snd.y));
      SetBottomRight(wxPoint(snd.x, fst.y));
    }
    else if (w < 0 && h < 0)
    {
      SetTopLeft(wxPoint(snd.x, snd.y));
      SetBottomRight(wxPoint(fst.x, fst.y));
    }
  }
}

DrawPane::DrawPane(wxFrame* parent) : wxPanel(parent)
{
  // Make drawing pane backround color white
  SetBackgroundColour(*wxGREEN);

  // State when no tool is selected
  this -> currentTool = NO_TOOL;
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

void DrawPane::render(wxDC&  dc)
{
    // draw some text
    //dc.DrawText(wxT("Testing"), 40, 60); 
 
    // draw a circle
    //dc.SetBrush(*wxGREEN_BRUSH); // green filling
    //dc.SetPen( wxPen( wxColor(255,0,0), 5 ) ); // 5-pixels-thick red outline
    //dc.DrawCircle( wxPoint(200,100), 25 /* radius */ );
 
    // draw a rectangle
    //dc.SetBrush(*wxBLUE_BRUSH); // blue filling
  dc.SetPen( wxPen( wxColor(0, 0, 0), 1 ) ); // 10-pixels-thick pink outline

  for (int i=0; i<zones.size(); i++)
  {
    //dc.DrawRectangle(zones[i].x, zones[i].y, zones[i].w, zones[i].h);
    //dc.DrawRectangle(wxRect(zones[i].start, zones[i].end));
    dc.DrawRectangle(zones[i]);
  }

  // Draw leakages
  for (int i=0; i<leakages.size(); i++)
  {
    dc.DrawCircle(leakages[i], 5);
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
            dc.SetPen(wxPen(*wxRED, 1));
          }

          // Draw temporaryZone
          dc.DrawRectangle(temporaryZone);
          break;
      }
      break;
    case LEAKAGE_TOOL:
      dc.DrawCircle(temporaryLeakage, 5);
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
        leakages.push_back(temporaryLeakage);
      }
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
      }
      // Stop drawing the zone
      currentDrawingState = NOT_DRAWING;
      break;

    case LEAKAGE_TOOL:
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
  }
  Refresh();
}

SelectedTool DrawPane::getToolState()
{
  return currentTool;
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
