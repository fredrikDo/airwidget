#include "myzone.h"

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

void MyZone::draw(wxDC &dc)
{
  // Draw ordinary rectangle
  dc.SetPen( wxPen( MyColors::COLOR_ZONE_LINE )); 
  dc.SetBrush(MyColors::COLOR_ZONE_BACKGROUND);
  dc.DrawRectangle(*this);
}

void MyZone::drawIllegal(wxDC &dc)
{
  // Draw illegal rectangle
  dc.SetPen(wxPen(*wxRED, 1));
  dc.DrawRectangle(*this);
}

void MyZone::setMouseOnTemperature(bool mouseOnTemperature_)
{
  mouseOnTemperature = mouseOnTemperature_;
}

void MyZone::drawTemperature(double T, wxDC &dc)
{
  if (mouseOnTemperature)
  {
    dc.SetBrush(MyColors::ZONE_TEMPERATURE_BRUSH_ON);
  }
  else
  {
    dc.SetBrush(MyColors::ZONE_TEMPERATURE_BRUSH_OFF);
  }

  wxString temperature;
  temperature << T;

  int w, h;
  dc.SetPen( wxPen( MyColors::ZONE_TEMPERATURE_LINE,6));
  dc.DrawCircle(x + 0.5 * GetWidth(), y + 0.5 * GetHeight(), 20);
  dc.GetTextExtent(temperature, &w, &h);
  dc.DrawText(temperature, x + 0.5 * (GetWidth() - w), y + 0.5 * (GetHeight() - h));
}

wxPoint MyZone::getCenter()
{
  return wxPoint(x + 0.5 * GetWidth(), y + 0.5 * GetHeight());
}
