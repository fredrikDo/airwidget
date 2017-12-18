#include "myleakage.h"

MyLeakage::MyLeakage()
{
  //AirflowArrow airflowArrow(39,38);  
}

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

void MyLeakage::relateToZones(vector<MyZone> z)
{
  // Connection is horizontal
  // |----|----|
  // |zone|zone|
  // |    *    |
  // |    |    |
  // |----|----|

  // Connection is not horizontal
  // -----------
  // |  zone   |
  // |         |
  // -----*-----
  // |         |
  // |  zone   |
  // -----------

  wxPoint p = getPosition();

  // Deflate all zones
  for (int i=0; i<z.size(); i++)
  {
    z[i].Deflate(1, 1);
  }

  // Update connections
  con1 = 0;
  con2 = 0;
  t1 = BOUNDARY;
  t2 = BOUNDARY;
  for (int i=0; i<z.size(); i++)
  {
    if (z[i].Contains(wxPoint(p.x-5, p.y)))
    {
      // Zone to the left
      con1 = i;
      t1 = ZONE;
      horizontal = true;
    }

    if (z[i].Contains(wxPoint(p.x+5, p.y)))
    {
      // Zone to the right
      con2 = i;
      t2 = ZONE;
      horizontal = true;
    }
      //break;
    
    if (z[i].Contains(wxPoint(p.x, p.y-5)))
    {
      // Zone above
      con1 = i;
      t1 = ZONE;
      horizontal = false;
    }
    
    if (z[i].Contains(wxPoint(p.x, p.y+5)))
    {
      // Zone below
      con2 = i;
      t2 = ZONE;
      horizontal = false;
    }
  }

  // Set height
  h = 1000 - p.y; // CHANGE THIS!
}

void MyLeakage::drawArrow(wxDC &dc, double flow, double maxFlow)
{
  dc.SetPen( wxPen( MyColors::AIRFLOWARROW_LINE, 1));
  dc.SetBrush(MyColors::AIRFLOWARROW_BACKGROUND);

  double step = maxFlow/10;
  int t=10;

  for (int i=0; i<10; i++)
  {
    if (abs(flow) < i * step)
    {
      t = i + 1;
      break;
    }
  }

  //int t=6;

  if (horizontal)
  {
    //dc.DrawLine( x, y, x+15, y ); // draw line across the rectangle
    //dc.DrawLine( x, y, x-15, y ); // draw line across the rectangle
    
    dc.DrawRectangle(x-10, y-0.5*t+0, 20, t);

    if (flow < 0) // -->
    {
      //wxPoint trianglePoints[3] = {wxPoint(x+10,y-t), wxPoint(x+10,y+t-1), wxPoint(x+10+t,y)};
      //dc.DrawPolygon(3, trianglePoints);
     
      wxPoint arrow[7] = {wxPoint(x-10, y+0.5*t),
                          wxPoint(x+10, y+0.5*t),
                          wxPoint(x+10, y+t),
                          wxPoint(x+10+t, y),
                          wxPoint(x+10, y-t),
                          wxPoint(x+10, y-0.5*t),
                          wxPoint(x-10, y-0.5*t)};
      dc.DrawPolygon(7, arrow); 
      //dc.DrawLine( x+15, y, x+10, y+5 ); // draw line across the rectangle
      //dc.DrawLine( x+15, y, x+10, y-5 ); // draw line across the rectangle  
    }
    else if (flow > 0) // <--
    {
      wxPoint trianglePoints[3] = {wxPoint(x-10,y-t), wxPoint(x-10,y+t), wxPoint(x-10-t,y)};
      dc.DrawPolygon(3, trianglePoints);
      
      wxPoint arrow[7] = {wxPoint(x+10, y+0.5*t),
                          wxPoint(x-10, y+0.5*t),
                          wxPoint(x-10, y+t),
                          wxPoint(x-10-t, y),
                          wxPoint(x-10, y-t),
                          wxPoint(x-10, y+0.5*t),
                          wxPoint(x+10, y+0.5*t)};
      dc.DrawPolygon(7, arrow); 
      //dc.DrawLine( x-15, y, x-10, y+5 ); // draw line across the rectangle
      //dc.DrawLine( x-15, y, x-10, y-5 ); // draw line across the rectangle
    }
  }
  else
  {
    //dc.DrawLine( x, y, x, y+15 ); // draw line across the rectangle
    //dc.DrawLine( x, y, x, y-15 ); // draw line across the rectangle
    
    dc.DrawRectangle(x-0.5*t+0, y-10, t, 20);

    // ->
    
    if (flow < 0) // Arrow Down
    {
      wxPoint arrow[7] = {wxPoint(x-0.5*t, y-10),
                          wxPoint(x-0.5*t, y+10),
                          wxPoint(x-t, y+10),
                          wxPoint(x, y+10+t),
                          wxPoint(x+t, y+10),
                          wxPoint(x+0.5*t, y+10),
                          wxPoint(x-0.5*t, y-10)};
      dc.DrawPolygon(7, arrow);
      //dc.DrawLine( x, y+15, x+5, y+10 ); // draw line across the rectangle
      //dc.DrawLine( x, y+15, x-5, y+10 ); // draw line across the rectangle  
    }
    else if (flow > 0) // Arrow Up
    {
      wxPoint arrow[7] = {wxPoint(x-0.5*t, y+10),
                          wxPoint(x-0.5*t, y-10),
                          wxPoint(x-t, y-10),
                          wxPoint(x, y-10-t),
                          wxPoint(x+t, y-10),
                          wxPoint(x+0.5*t, y-10),
                          wxPoint(x-0.5*t, y+10)};
      dc.DrawPolygon(7, arrow);
      //dc.DrawLine( x, y-15, x+5, y-10 ); // draw line across the rectangle
      //dc.DrawLine( x, y-15, x-5, y-10 ); // draw line across the rectangle  
    }
  }
}

void MyLeakage::draw(wxDC &dc)
{
  dc.SetPen( wxPen( MyColors::COLOR_LEAKAGE_LINE, 1 ) );
  dc.SetBrush(MyColors::COLOR_LEAKAGE_BACKGROUND);
  dc.DrawCircle(x, y, 3);
}
