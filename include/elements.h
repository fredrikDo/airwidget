#ifndef ELEMENTS_H 
#define ELEMENTS_H

#include "wx/wx.h"

#include <vector>
#include "myleakage.h"
#include "drawpaneFunctions.h"
#include "../../airflow/lib/multizone.h"

using namespace std;

struct Element
{
  wxPoint p1;
  wxPoint p2;
  int z_left; // Zone number to the left/above
  int z_right; // Zone number to the right/below
  double delta_P1, delta_P2; // Pressure difference at each point
  wxPoint P1, P2; // Pressure difference to be drawn
  double h;
};

class Elements
{
private:
  int num_elements;
  
public:
  void update(vector<MyZone> &zones);
  void updatePressures(MultiZone &m);
  void draw(wxDC &dc);
  int min_iterator(const vector<wxPoint> &vec, char axis);
  vector<wxPoint> sort(vector <wxPoint> unsorted, char axis);
  vector<Element> elements;
};

#endif
