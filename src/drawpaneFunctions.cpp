#include "drawpaneFunctions.h"

bool pointIsInsideZones(wxPoint p, vector<MyZone> z)
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

bool zoneIntersectsZones(MyZone t, vector<MyZone> z)
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

wxPoint snap(wxPoint p, vector<MyZone> z)
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

bool pointOnEdge(wxPoint p, vector<MyZone> z)
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

bool pointCloseToPoint(const wxPoint &a, const wxPoint &b, int d)
{
  bool r = false;

  if (pow(a.x - b.x, 2) + pow(a.y - b.y, 2) < pow(d, 2))
  {
    r = true;
  }
  return r;
}

bool pointCloseToCorner(const wxPoint &p, const vector<MyZone> z)
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

bool pointInsideCircle(const wxPoint &p, const wxPoint &cp, const int &r)
{
  bool ret = false;

  if (pow(p.x - cp.x, 2) + pow(p.y - cp.y, 2) < r * r)
  {
    ret = true;
  }
  return ret;
}

bool pointInsideTemperatureCircles(wxPoint p, vector<MyZone> &zones, int &m)
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

int getZoneNumber(const wxPoint &p, vector<MyZone> &z)
{
  // See if wxPoint is inside any zone and return
  // the zone number. If wxPoint is not inside any
  // zone (-1) is returned

  int ret = -1;

  for (int i=0; i<z.size(); i++)
  {
    if (z[i].Contains(wxPoint(p.x, p.y)))
    {
      ret = i;
    }
  }
  return ret;
}
