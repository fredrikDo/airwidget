#ifndef MYZONE_H
#define MYZONE_H

#include "wx/wx.h"
#include "mycolors.h"

class MyZone : public wxRect
{
  // 42
  public:
    void allPositive();
    void draw(wxDC &dc);
    void drawIllegal(wxDC &dc);
    void drawTemperature(double T, wxDC &dc);
    void setMouseOnTemperature(bool);
    wxPoint getCenter();

  private:
    bool mouseOnTemperature;
};

#endif
