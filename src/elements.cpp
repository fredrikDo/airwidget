#include "elements.h"

int Elements::min_iterator(const vector<wxPoint> &vec, char axis)
{
  // Returns the iterator to the smallest value of either x or y
  int it = 0;
  if (axis == 'x')
  {
    for (int i=0; i<vec.size(); i++)
    {
      if (vec[i].x < vec[it].x)
      {
        it = i;
      }
    }
  }
  else if (axis == 'y')
  {
    for (int i=0; i<vec.size(); i++)
    {
      if (vec[i].y < vec[it].y)
      {
        it = i;
      }
    }
  }
  else
  {
    cout << "Warning in Elements::main_iterator(), wxPoint only contain x or y" << endl;
  }
  return it;
}

vector <wxPoint> Elements::sort(vector <wxPoint> unsorted, char axis)
{
  // Sort using Selection Sort Algorithm //

  vector <wxPoint> sorted;

  while (!unsorted.empty())
  {
    int min_it = min_iterator(unsorted, axis);
    sorted.push_back(unsorted[min_it]);
    unsorted.erase(unsorted.begin()+min_it);
  }
 
  return sorted;
}

void Elements::updatePressures(MultiZone &m)
{
  // D....
  // ADJUST THIS WHEN ADDING "PROPER" HEIGHT INTO THE MODEL
  for (int i=0; i<elements.size(); i++)
  {
    elements[i].delta_P1 = m.getPressure(elements[i].z_left, 1000-elements[i].p1.y) -
      m.getPressure(elements[i].z_right, 1000-elements[i].p1.y);
    
    elements[i].delta_P2 = m.getPressure(elements[i].z_left, 1000-elements[i].p2.y) -
      m.getPressure(elements[i].z_right, 1000-elements[i].p2.y);
  }
  
  // Point to be drawn as pressuredifference
  float scale = 0.3;
  for (int i=0; i<elements.size(); i++)
  {
    elements[i].P1 = elements[i].p1 + wxPoint(scale*elements[i].delta_P1, 0);
    elements[i].P2 = elements[i].p2 + wxPoint(scale*elements[i].delta_P2, 0);
  }
}

void Elements::update(vector<MyZone> &zones)
{
  elements.clear(); 

  Element temp;
 
  /** Vertical Elements **/
 
  // Find x-positions with vertical lines
  vector<wxPoint> all_pos;
  for (int i=0; i<zones.size(); i++)
  {
    all_pos.push_back(zones[i].GetTopLeft());
    all_pos.push_back(zones[i].GetTopRight());
    all_pos.push_back(zones[i].GetBottomLeft());
    all_pos.push_back(zones[i].GetBottomRight());
  }
 
  // Sort in with respect to x
  all_pos = sort(all_pos, 'y');
  all_pos = sort(all_pos, 'x');

  // Print vector of wxPoint
  /* 
  for (int i=0; i<all_pos.size(); i++)
  {
    cout << "x: " << all_pos[i].x << " y: " << all_pos[i].y << endl;
  }
  cout << endl;
  */

  // Add points to vector of elements
  for (int i=0; i<all_pos.size()-1; i++)
  {
    if (all_pos[i].x == all_pos[i+1].x && all_pos[i].y != all_pos[i+1].y)
    {
      temp.p1 = all_pos[i];
      temp.p2 = all_pos[i+1];
      elements.push_back(temp);
    }
  }

  // Get the zone number on each side of the element, -1 means outside zone
  for (int i=0; i<elements.size(); i++)
  {
    elements[i].z_left = getZoneNumber(elements[i].p1-wxPoint(5,-5), zones);
    elements[i].z_right = getZoneNumber(elements[i].p1-wxPoint(-5,-5), zones);

    // Add local height to each element
    elements[i].h = elements[i].p2.y - elements[i].p1.y;
  }
  
  /*
  for (int i=0; i<elements.size(); i++)
  {
    cout << "left: " << elements[i].z_left << endl;
    cout << "right: " << elements[i].z_right << endl;
  }
  cout << endl;
  */
  
  /** Horizontal Elements **/
  
  // Add code here...
}

void Elements::draw(wxDC &dc)
{
  /*
  for (int i=0; i<elements.size(); i++)
  {
    dc.SetPen( wxPen(wxColor(255,0,0)));
    dc.DrawLine(elements[i].p1, elements[i].p2);
    dc.DrawCircle(wxPoint(elements[i].p1.x, elements[i].p1.y+0.5*(elements[i].p2.y-elements[i].p1.y)), wxCoord(4));
  }
  */
  
  // TEST
  //dc.DrawLine();
  dc.SetBrush(wxColour(200,200,200));
  
  for (int i=0; i<elements.size(); i++)
  {
    wxPoint p[4] = {elements[i].p1, elements[i].p2, elements[i].P2, elements[i].P1};
    dc.DrawPolygon(4, p);
  }
}
