#ifndef MYCOLORS_H
#define MYCOLORS_H

#include "wx/wx.h"

namespace MyColors
{
  // DRAWPANE
  const wxColour COLOR_DRAWPANE_BACKGROUND(255, 255, 255);

  // LEAKAGE
  const wxColour COLOR_LEAKAGE_LINE(0, 0, 0);
  const wxColour COLOR_LEAKAGE_BACKGROUND(255, 255, 255);

  // ZONE
  const wxColour COLOR_ZONE_BACKGROUND(255, 255, 255);
  const wxColour COLOR_ZONE_LINE(91, 91, 91);

  // AIRFLOW ARROWS
  const wxColour AIRFLOWARROW_BACKGROUND(51, 153, 255);
  const wxColour AIRFLOWARROW_LINE(51, 153, 255);

  // ZONE TEMPERATURE
  const wxColour ZONE_TEMPERATURE_BRUSH_ON(204, 255, 204);
  const wxColour ZONE_TEMPERATURE_BRUSH_OFF(250, 250, 250);
  const wxColour ZONE_TEMPERATURE_LINE(212, 212, 212);
}

#endif
