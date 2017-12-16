#ifndef MYLEAKAGE_H
#define MYLEAKAGE_H

#include "wx/wx.h"
#include <vector>

#include "myzone.h"
#include "mycolors.h"
#include "../../airflow/lib/leakage.h"

using namespace std;

class MyLeakage : public wxPoint
{
  public:
    MyLeakage();
    void setPosition(wxPoint p);
    wxPoint getPosition();

    int con1, con2; // node connections for leakage
    int h;
    NodeType t1, t2;
    void relateToZones(vector<MyZone> z);
    bool horizontal;
    void drawArrow(wxDC &dc, double flow, double maxFlow);
    void draw(wxDC &dc);
};

#endif
