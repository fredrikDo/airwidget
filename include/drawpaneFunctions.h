#ifndef DRAWPANEFUNCTIONS_H
#define DRAWPANEFUNCTIONS_H

#include "wx/wx.h"
#include <vector>

#include "myzone.h"

using namespace std;

bool pointIsInsideZones(wxPoint p, vector<MyZone> z);
wxPoint snap(wxPoint p, vector<MyZone> z);
bool zoneIntersectsZones(MyZone t, vector<MyZone> z);
bool pointOnEdge(wxPoint p, vector<MyZone> z);
bool pointCloseToPoint(const wxPoint &a, const wxPoint &b, int d);
bool pointCloseToCorner(const wxPoint &p, const vector<MyZone> z);
bool pointInsideCircle(const wxPoint &p, const wxPoint &cp, const int &r);
bool pointInsideTemperatureCircles(wxPoint p, vector<MyZone> &z, int &m);
int getZoneNumber(const wxPoint &a, vector<MyZone> &z);

#endif
