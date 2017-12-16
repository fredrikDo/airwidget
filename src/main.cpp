#include "wx/wx.h"
#include "wx/toolbar.h"

#include "drawpane.h"
#include "icons.h"

const int ID_ZONE_TOOL = 100;
const int ID_LEAKAGE_TOOL = 101;
const int ID_EDIT_TOOL = 102;

const int ID_ARROW_DISP = 203;
const int ID_PRESSURE_VERTICAL_DISP = 204;

 // Declare our main frame class
class MyFrame : public wxFrame
{
  public:
  // Constructor
  MyFrame(const wxString& title);
  
  // Event handlers
  void OnQuit(wxCommandEvent& event);
  void setStateZoneTool(wxCommandEvent& event);
  void setStateLeakageTool(wxCommandEvent& event);
  void setStateEditTool(wxCommandEvent& event);
 
  void setStateDisplayArrow(wxCommandEvent& event);
  void setStateDisplayPressureVertical(wxCommandEvent& event);
  
  // Main drawpane 
  DrawPane* mainDrawPane;

  // Toolbar
  wxToolBar* toolBar;

  private:
  // This class handles events
  DECLARE_EVENT_TABLE()
};

MyFrame::MyFrame(const wxString& title)
 : wxFrame(NULL, wxID_ANY, title)
{
  // Create a status bar just for fun
  CreateStatusBar(2);
  SetStatusText(wxT("STAY CALM!"));

  // Create drawpane
  mainDrawPane = new DrawPane(this);
  
  // Create a toolbar (consider swithing to wxAuiToolBar)
  toolBar = new wxToolBar(this, wxID_ANY,
    wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER);

  wxImage::AddHandler(new wxPNGHandler);
  
  wxBitmap bmpLeak = wxBITMAP_PNG_FROM_DATA(leak);
  wxBitmap bmpAdjust = wxBITMAP_PNG_FROM_DATA(adjust);
  wxBitmap bmpDraw = wxBITMAP_PNG_FROM_DATA(draw);
 
  wxBitmap bmpArrow = wxBITMAP_PNG_FROM_DATA(arrow);
  wxBitmap bmpPressure_vertical = wxBITMAP_PNG_FROM_DATA(pressure_vertical);  
 
  this -> SetToolBar(toolBar);
  toolBar -> SetToolBitmapSize(wxSize(128,128));
 
  // Add tools... 
  toolBar -> AddCheckTool(ID_ZONE_TOOL, wxT("Zone"), bmpDraw);
  toolBar -> AddCheckTool(ID_LEAKAGE_TOOL, wxT("Leakage"), bmpLeak);
  toolBar -> AddCheckTool(ID_EDIT_TOOL, wxT("Adjust"), bmpAdjust);
  toolBar -> AddSeparator();

  toolBar -> AddCheckTool(ID_ARROW_DISP, wxT("Arrow"), bmpArrow);
  toolBar -> AddCheckTool(ID_PRESSURE_VERTICAL_DISP, wxT("Pressure Vertical"), bmpPressure_vertical);
  
  toolBar -> Realize();
}

void MyFrame::OnQuit(wxCommandEvent& event)
{
  Close();
}

void MyFrame::setStateDisplayArrow(wxCommandEvent& event)
{
  if (mainDrawPane -> displayArrow)
  {
    mainDrawPane -> displayArrow = false;
  }
  else
  {
    mainDrawPane -> displayArrow = true;
  }
  mainDrawPane -> Refresh();
}

void MyFrame::setStateDisplayPressureVertical(wxCommandEvent& event)
{
  if (mainDrawPane -> displayPressureVertical)
  {
    mainDrawPane -> displayPressureVertical = false;
  }
  else
  {
    mainDrawPane -> displayPressureVertical = true;
  }
}

void MyFrame::setStateZoneTool(wxCommandEvent& event)
{
  if (mainDrawPane -> getToolState() == ZONE_TOOL)
  {
    mainDrawPane -> setStateNoTool(event);
  }
  else
  {
    mainDrawPane -> setStateZoneTool(event); 
  }

  // Untoggle other tools
  if (toolBar -> GetToolState(ID_ZONE_TOOL))
  {
    toolBar -> ToggleTool(ID_LEAKAGE_TOOL, false);
    toolBar -> ToggleTool(ID_EDIT_TOOL, false);
  }
}

void MyFrame::setStateLeakageTool(wxCommandEvent& event)
{
  if (mainDrawPane -> getToolState() == LEAKAGE_TOOL)
  {
    mainDrawPane -> setStateNoTool(event);
  }
  else
  {
    mainDrawPane -> setStateLeakageTool(event);
  }

  // Untoggle other tools
  if (toolBar -> GetToolState(ID_LEAKAGE_TOOL))
  {
    toolBar -> ToggleTool(ID_ZONE_TOOL, false);
    toolBar -> ToggleTool(ID_EDIT_TOOL, false);
  }
}

void MyFrame::setStateEditTool(wxCommandEvent& event)
{
  if (mainDrawPane -> getToolState() == EDIT_TOOL)
  {
    mainDrawPane -> setStateNoTool(event);
  }
  else
  {
    mainDrawPane -> setStateEditTool(event);
  }

  // Untoggle other tools
  if (toolBar -> GetToolState(ID_EDIT_TOOL))
  {
    toolBar -> ToggleTool(ID_ZONE_TOOL, false);
    toolBar -> ToggleTool(ID_LEAKAGE_TOOL, false);
  }
}

// Event table for DrawPane
BEGIN_EVENT_TABLE(DrawPane, wxPanel)
  EVT_LEFT_DOWN(DrawPane::mouseDown)
  EVT_LEFT_UP(DrawPane::mouseUp)
  EVT_MOTION(DrawPane::mouseMoved)
  // catch paint events
  EVT_PAINT(DrawPane::paintEvent)
  // mouse
  EVT_MOUSEWHEEL(DrawPane::scrollUp)
END_EVENT_TABLE()

// Declare the application class
class MyApp : public wxApp
{
  public:
  // Called on application startup
  virtual bool OnInit();
};

// Implements MyApp& wxGetApp()
DECLARE_APP(MyApp)

// Give wxWidgets the means to create a MyApp object
IMPLEMENT_APP(MyApp)

// Event table for MyFrame
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
  EVT_TOOL(ID_ZONE_TOOL, MyFrame::setStateZoneTool)
  EVT_TOOL(ID_LEAKAGE_TOOL, MyFrame::setStateLeakageTool)
  EVT_TOOL(ID_EDIT_TOOL, MyFrame::setStateEditTool)
  EVT_TOOL(ID_ARROW_DISP, MyFrame::setStateDisplayArrow)
  EVT_TOOL(ID_PRESSURE_VERTICAL_DISP, MyFrame::setStateDisplayPressureVertical)
END_EVENT_TABLE()

bool MyApp::OnInit()
{
  // Create the main application window
  MyFrame *frame = new MyFrame(wxT("Minimal wxWidgets App"));
  // Show it
  frame->Show(true);
  // Start the event loop
  return true;
}
