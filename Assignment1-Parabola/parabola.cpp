#include <math.h>
#include <windows.h>
#include "parabola.h"

DRAWING_DATA gDrawData; // global data
void drawparabola();


void reDraw(HWND hwnd)
{
  InvalidateRect(hwnd, NULL, 1);
}

void drawPoint(int x, int y)
{
  Ellipse(gDrawData.hdcMem,x-10,y-10,x+10,y+10); //marking the center 
}

void setupMenus(HWND hwnd)
{
  HMENU hMenu;
  
  hMenu=GetMenu(hwnd);

  switch (gDrawData.drawMode)
  {
    case READY_MODE :
      // enable parabola menu
      EnableMenuItem(hMenu, ID_parabola,
                     MF_BYCOMMAND|MF_ENABLED);
      break;
    case DRAW_MODE :
    case DRAWN_MODE :
      // disable parabola menu
      EnableMenuItem(hMenu, ID_parabola,
                     MF_BYCOMMAND|MF_GRAYED);
      break;
  }
}

void setDrawMode(MODE mode, HWND hwnd)
{
  gDrawData.drawMode = mode;
  setupMenus(hwnd);
}

void createMemoryBitmap(HDC hdc)
{
  gDrawData.hdcMem = CreateCompatibleDC(hdc);
  gDrawData.hbmp = CreateCompatibleBitmap(hdc, 
    gDrawData.maxBoundary.cx, gDrawData.maxBoundary.cy);
  SelectObject(gDrawData.hdcMem, gDrawData.hbmp);
  PatBlt(gDrawData.hdcMem, 0, 0, gDrawData.maxBoundary.cx, 
         gDrawData.maxBoundary.cy, PATCOPY);
}

void initialize(HWND hwnd, HDC hdc)
{
  gDrawData.hDrawPen=CreatePen(PS_SOLID, 1, RGB(255,0, 0));

  gDrawData.maxBoundary.cx = GetSystemMetrics(SM_CXSCREEN);
  gDrawData.maxBoundary.cy = GetSystemMetrics(SM_CYSCREEN);

  createMemoryBitmap(hdc);
  setDrawMode(READY_MODE, hwnd);
}

void cleanup()
{
  DeleteDC(gDrawData.hdcMem);
}

void reset(HWND hwnd)
{
  gDrawData.centre.x = gDrawData.centre.y = 0;

  gDrawData.drawMode = READY_MODE;

  PatBlt(gDrawData.hdcMem, 0, 0, gDrawData.maxBoundary.cx, 
         gDrawData.maxBoundary.cy, PATCOPY);

  reDraw(hwnd);
  setupMenus(hwnd);
}

void plot_sympoint(int ex, int ey, COLORREF clr)
{
  int cx = gDrawData.centre.x;
  int cy = gDrawData.centre.y;

  SetPixel(gDrawData.hdcMem, ex+cx,cy-ey, clr);
  SetPixel(gDrawData.hdcMem, ex+cx,cy+ey, clr);
}

void addPoint(HWND hwnd, int x, int y)
{
  switch (gDrawData.drawMode)
  {
    case DRAW_MODE:
      /* the coordinates of the centre is stored 
         and the parabola is drawn */
      SelectObject(gDrawData.hdcMem, gDrawData.hDrawPen);
      drawPoint(x,y);
      gDrawData.centre.x = x;
      gDrawData.centre.y = y;
      drawparabola();
      setDrawMode(DRAWN_MODE, hwnd);
      reDraw(hwnd);
      break;
    case DRAWN_MODE:
      MessageBox(hwnd,
       "Parabola already drawn, now you can clear the area", 
          "Warning",MB_OK);
      break;
    default:
      break;
  }
}

void drawImage(HDC hdc)
{
  BitBlt(hdc, 0, 0, gDrawData.maxBoundary.cx, 
    gDrawData.maxBoundary.cy, gDrawData.hdcMem, 
    0, 0, SRCCOPY);
}

void processLeftButtonDown(HWND hwnd, int x, int y)
{
  addPoint(hwnd,x,y);
}

void processCommand(int cmd, HWND hwnd)
{
  int response;
  switch(cmd)
  {
    case ID_CLEAR:
      reset(hwnd);
      setDrawMode(READY_MODE, hwnd);
      break;
    case ID_parabola:
      setDrawMode(DRAW_MODE, hwnd);

      break;
    case ID_EXIT:
        response=MessageBox(hwnd,
          "Quit the program?", "EXIT", 
          MB_YESNO);
        if(response==IDYES) 
            PostQuitMessage(0);
        break;
    default:
      break;
  }
}

void drawparabola()
{
//parabola x = y^2 for -10=<y<=10 .. i.e. for x in range [0,100] 
// We use symmetry along the x-axis and simply copy the points from 1st quadrant to 4th quadrant.
// for drawing the parabola we have made four cases depending upon the value and sign of slope.
// We keep a decision parameter named p to decide the next pixel to be selected.
// To avoid float calculation instead of calculating m as 1/2y we have compared 1 to 2y 
// We initialize our curve from point [0.5,1]
  int x,y;		// co-ordinates
  x=0;	// initial x
  
  y=0;	// initial y  as slope at , [0,0] is > 1
  plot_sympoint(x,y, RGB(0,0,0));
//g(x,y) = y^2-x;
  int i = 1;
  int m = 1;			 
  float p = 1.0/2.0;     // initial value of p is 0.5
  while(y<=25){
             if(m > 2*y){
                     y = y + 1;
                     if(p<0){
                     		 p = p + 2*y + 3;
                             x = x;
                             
                     }
                     else{
                            p = p + 2*y + 2;
                            x = x + 1;
                     }

             }
             if(0<m<=2*y){
                      x = x + 1;
                      if(p<0){
                              p = p + 2*y +1;
                              y = y+1;
                      }
                      else{
                              p = p - 1;
                              y = y;
                      }
             }
             if(m<-2*y){
                     y = y + 1;
                     if(p<0){
                     		p = p + 2*y + 3;
                            x = x;
                             
                     }
                     else{
                             p = p + 2*y + 4;
                             x = x - 1;
                     }

             }
             if(-2*y<=m<=0){
                      x = x - 1;
                      if(p<0){
                              p = p + 2*y + 1;
                              y = y+1;
                      }
                      else{
                              p = p + 1;
                              y = y;
                      }
             }
             plot_sympoint(x,y, RGB(0,0,0));
    }
  
}



LRESULT CALLBACK WindowF(HWND hwnd,UINT message,WPARAM wParam,
                         LPARAM lParam) 
{
  HDC hdc;
  PAINTSTRUCT ps;
  int x,y;

  switch(message)
  {
    case WM_CREATE:
      hdc = GetDC(hwnd);
      initialize(hwnd, hdc);
      ReleaseDC(hwnd, hdc);
      break;
    
    case WM_COMMAND:
      processCommand(LOWORD(wParam), hwnd);
      break;

    case WM_LBUTTONDOWN:
      x = LOWORD(lParam);
      y = HIWORD(lParam);
      processLeftButtonDown(hwnd, x,y);
      break;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      drawImage(hdc);
      EndPaint(hwnd, &ps);
      break;

    case WM_DESTROY:
      cleanup();
      PostQuitMessage(0);
      break;
  }
  // Call the default window handler
  return DefWindowProc(hwnd, message, wParam, lParam);
}

