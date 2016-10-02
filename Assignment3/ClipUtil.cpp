#include <windows.h>
#include <math.h>
#include "ClipUtil.h"
#include "Line.h"

LRESULT CALLBACK NumberOfLines(HWND hdlg,UINT mess,WPARAM more,LPARAM pos);

DRAWING_DATA gDrawData; // global data

void reDraw(HWND hwnd)
{
  InvalidateRect(hwnd, NULL, 1);
}

void setupMenus(HWND hwnd)
{
  HMENU hmenu, hsubmenu;
  hmenu=GetMenu(hwnd);
  hsubmenu=GetSubMenu(hmenu, 0);

  switch (gDrawData.drawMode)
  {
    case READY_MODE :
      //enable 'Draw Rectangle',disable 'Draw Line','Clip' menus
      EnableMenuItem(hsubmenu, ID_DRAW_RECTANGLE,
                     MF_BYCOMMAND|MF_ENABLED);
      EnableMenuItem(hsubmenu, ID_DRAW_LINE,
                     MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hsubmenu, ID_CLIP,
                     MF_BYCOMMAND|MF_GRAYED);
      break;
    case DRAW_RECTANGLE_MODE :
      //enable 'Draw Line',disable 'Draw Rectangle','Clip' menus
      EnableMenuItem(hsubmenu, ID_DRAW_RECTANGLE,
                     MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hsubmenu, ID_DRAW_LINE,
                     MF_BYCOMMAND|MF_ENABLED);
      EnableMenuItem(hsubmenu, ID_CLIP,
                     MF_BYCOMMAND|MF_GRAYED);
      break;
    case DRAW_LINE_MODE :
      //enable 'Clip',disable 'Draw Line, Rectangle' menus
      EnableMenuItem(hsubmenu, ID_DRAW_RECTANGLE,
                     MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hsubmenu, ID_DRAW_LINE,
                     MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hsubmenu, ID_CLIP,
                     MF_BYCOMMAND|MF_ENABLED);
      break;
    case CLIP_MODE :
    case CLIPPED_MODE :
      //disable 'Clip' and 'Draw Line, Rectangle' menus
      EnableMenuItem(hsubmenu,
                     ID_DRAW_RECTANGLE,MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hsubmenu, ID_DRAW_LINE,
                     MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hsubmenu, ID_CLIP,
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
 for(int i=0;i<=10;i++){

  gDrawData.beginPt[i].x = gDrawData.beginPt[i].y = 0;
  gDrawData.endPt[i].x = gDrawData.endPt[i].y = 0;
  gDrawData.lineEndPts[i][0].x = 
    gDrawData.lineEndPts[i][0].y = 0;
  gDrawData.lineEndPts[i][1].x = 
    gDrawData.lineEndPts[i][1].y = 0;
}
  gDrawData.rectCornerPts[0].x = 
    gDrawData.rectCornerPts[0].y = 0;
  gDrawData.rectCornerPts[1].x = 
    gDrawData.rectCornerPts[1].y = 0;

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
 for(int i=0;i<=10;i++){

  gDrawData.beginPt[i].x = gDrawData.beginPt[i].y = 0;
  gDrawData.endPt[i].x = gDrawData.endPt[i].y = 0;
  gDrawData.lineEndPts[i][0].x = 
    gDrawData.lineEndPts[i][0].y = 0;
  gDrawData.lineEndPts[i][1].x = 
    gDrawData.lineEndPts[i][1].y = 0;
}   
  gDrawData.rectCornerPts[0].x = 
    gDrawData.rectCornerPts[0].y = 0;
  gDrawData.rectCornerPts[1].x = 
    gDrawData.rectCornerPts[1].y = 0;
  
  gDrawData.drawMode = READY_MODE;

  PatBlt(gDrawData.hdcMem, 0, 0, gDrawData.maxBoundary.cx, 
         gDrawData.maxBoundary.cy, PATCOPY);

  reDraw(hwnd);
  setupMenus(hwnd);
}

void drawImage(HDC hdc)
{
  BitBlt(hdc, 0, 0, gDrawData.maxBoundary.cx, 
    gDrawData.maxBoundary.cy, gDrawData.hdcMem, 0, 0, SRCCOPY);
}

void performRubberBanding(HWND hwnd, int x, int y)
{
  int r = 0;
  HDC hdc = GetDC(hwnd);

  switch(gDrawData.drawMode)
  {
    case DRAW_LINE_MODE :
      drawLineSegment(hdc, gDrawData.beginPt[gDrawData.count], 
                      gDrawData.endPt[gDrawData.count], CLR_BG);
      drawRectangle(hdc,
                    gDrawData.rectCornerPts[0], 
                    gDrawData.rectCornerPts[1],
                    CLR_RECT); 
      gDrawData.endPt[gDrawData.count].x = x;
      gDrawData.endPt[gDrawData.count].y = y;
      drawLineSegment(hdc, gDrawData.beginPt[gDrawData.count], 
                      gDrawData.endPt[gDrawData.count], CLR_LINE); 
      break;

    case DRAW_RECTANGLE_MODE :
      drawRectangle(hdc, gDrawData.beginPt[0], 
                    gDrawData.endPt[0], CLR_BG);
      gDrawData.endPt[0].x = x;
      gDrawData.endPt[0].y = y;
      drawRectangle(hdc, gDrawData.beginPt[0], 
                    gDrawData.endPt[0], CLR_RECT);
      break;
    default :
      break;
  }
  ReleaseDC(hwnd, hdc) ;
}

void processLeftButtonDown(HWND hwnd, int x, int y)
{
  switch (gDrawData.drawMode)
  {
  	case DRAW_RECTANGLE_MODE:
  		gDrawData.beginPt[0].x = gDrawData.endPt[0].x = x;
      	gDrawData.beginPt[0].y = gDrawData.endPt[0].y = y;
      	reDraw(hwnd);
      	break;
    case DRAW_LINE_MODE:
        gDrawData.count++;
        if(gDrawData.count<=gDrawData.number){
	  
	  		gDrawData.beginPt[gDrawData.count].x = gDrawData.endPt[gDrawData.count].x = x;
      		gDrawData.beginPt[gDrawData.count].y = gDrawData.endPt[gDrawData.count].y = y;
    	}
      	else{
	      	MessageBox(hwnd,"Max Number of lines reached", "EXIT", MB_YESNO);
	  }
	  
	  //reDraw(hwnd);
      break;

    default:
      return;
  }
}

void processMouseMove(HWND hwnd, int x, int y)
{
  switch (gDrawData.drawMode)
  {
    case DRAW_LINE_MODE:
    case DRAW_RECTANGLE_MODE:
      performRubberBanding(hwnd, x, y);
      break;
    default:
      break;
  }
}

void draw()
{
  switch (gDrawData.drawMode)
  {
   case DRAW_LINE_MODE:
    	
    	for(int i=1;i<=gDrawData.number;i++){
	  
      		gDrawData.lineEndPts[i][0].x = gDrawData.beginPt[i].x;
      		gDrawData.lineEndPts[i][0].y = gDrawData.beginPt[i].y;

		    gDrawData.lineEndPts[i][1].x = gDrawData.endPt[i].x;
	    	gDrawData.lineEndPts[i][1].y = gDrawData.endPt[i].y;

    	  	drawLineSegment(gDrawData.hdcMem, gDrawData.beginPt[i], 
                      gDrawData.endPt[i], CLR_LINE);
		}	
      	break;
 
    case DRAW_RECTANGLE_MODE:
      gDrawData.rectCornerPts[0].x = gDrawData.beginPt[0].x;
      gDrawData.rectCornerPts[0].y = gDrawData.beginPt[0].y;

      gDrawData.rectCornerPts[1].x = gDrawData.endPt[0].x;
      gDrawData.rectCornerPts[1].y = gDrawData.endPt[0].y;
      
      drawRectangle(gDrawData.hdcMem, 
        gDrawData.rectCornerPts[0], 
        gDrawData.rectCornerPts[1], CLR_RECT);
      break;

    default:
      return;
  }
}

void processCommonCommand(int cmd, HWND hwnd)
{
  int response;
  switch(cmd)
  {
    case ID_CLEAR:
      gDrawData.count = 0;
      gDrawData.number = 1;
      reset(hwnd);
      break;
    case ID_EXIT:
      response = 
        MessageBox(hwnd,"Quit the program?", "EXIT", MB_YESNO);
        if(response==IDYES) 
          PostQuitMessage(0);
        break;
    default:
      break;
  }
}

void swap(LONG& x, LONG& y)
{
  LONG tmp;
  tmp = x;
  x = y;
  y = tmp;
}

void computeClipDimensions()
{
  if (gDrawData.rectCornerPts[0].x>gDrawData.rectCornerPts[1].x)
    swap(gDrawData.rectCornerPts[0].x,
         gDrawData.rectCornerPts[1].x);
  if (gDrawData.rectCornerPts[0].y>gDrawData.rectCornerPts[1].y)
    swap(gDrawData.rectCornerPts[0].y,
         gDrawData.rectCornerPts[1].y);

  gDrawData.clipMin.x = gDrawData.rectCornerPts[0].x-1;
  gDrawData.clipMax.x = gDrawData.rectCornerPts[1].x+1;
  gDrawData.clipMin.y = gDrawData.rectCornerPts[0].y-1;
  gDrawData.clipMax.y = gDrawData.rectCornerPts[1].y+1;
}

bool isPointWithinClipBoundary(POINT pt)  
{
  if ((pt.x <= gDrawData.clipMin.x) ||
      (pt.x >= gDrawData.clipMax.x) ||
      (pt.y <= gDrawData.clipMin.y) ||
      (pt.y >= gDrawData.clipMax.y))
    return false;
  return true;
}

bool isLineWithinClipBoundary(POINT start, POINT end)
{
  return (isPointWithinClipBoundary(start) && 
          isPointWithinClipBoundary(end));
}

bool isLineOutsideClipBoundary(POINT start, POINT end)
{
  if (((gDrawData.beginPt[0].x >= start.x) && 
       (gDrawData.beginPt[0].x >= end.x)) ||
      ((gDrawData.endPt[0].x <= start.x) && 
       (gDrawData.endPt[0].x <= end.x)))
    return true;
  if (((gDrawData.beginPt[0].y >= start.y) && 
       (gDrawData.beginPt[0].y >= end.y)) ||
      ((gDrawData.endPt[0].y <= start.y) && 
       (gDrawData.endPt[0].y <= end.y)))
    return true;
  return false;
}

bool isValidRectangle(POINT a, POINT b)
{
  if ((abs(a.x - b.x) >= 10) &&
      (abs(a.y - b.y) >= 10))
    return true;
  else
    return false;
}

bool isValidLine(POINT a, POINT b)
{
  double length;
  double xDiff, yDiff;
  xDiff = a.x - b.x;
  yDiff = a.y - b.y;
  length = sqrt(xDiff * xDiff + yDiff * yDiff);
  if (length >= 10) 
    return true;
  else
    return false;
}

void processCommand(int cmd, HWND hwnd)
{
  switch(cmd)
  {
    case ID_DRAW_LINE:
      DialogBox(NULL,"MyDB",hwnd,(DLGPROC)NumberOfLines);    
      if (isValidRectangle(gDrawData.beginPt[0], gDrawData.endPt[0]))
      {
        //valid rectangle, then switch to next mode
        //draw the rectangle first as drawn through rubber banding
        draw();
        setDrawMode(DRAW_LINE_MODE, hwnd);
      }
      else
      {
        MessageBox(hwnd,
         "Please draw a valid rectangle of at least 10x10 pixels", 
         "Warning!", MB_OK);
      }
      break;
    case ID_DRAW_RECTANGLE:
     setDrawMode(DRAW_RECTANGLE_MODE, hwnd);
     break;
    case ID_CLIP:
      
      if (isValidLine(gDrawData.beginPt[0], gDrawData.endPt[0]))
      {
        // valid line, then switch to next mode
        // draw the line first as drawn through rubber banding
        draw();
        setDrawMode(CLIP_MODE, hwnd);
        computeClipDimensions();
        clip(hwnd);
      }
      else
      {
        MessageBox(hwnd,
         "Please draw a valid line of length at least 10 pixels", 
         "Warning!", MB_OK);
      }
      break;
    default:
      processCommonCommand(cmd, hwnd);
      return;
  }
}


LRESULT CALLBACK NumberOfLines(HWND hdlg,UINT mess,WPARAM more,LPARAM pos)
{
  	char str[20];
  	switch(mess)
  	{
	    case WM_COMMAND:
	     	switch(more)
	      	{
		        case ID_OK:
	    		    GetDlgItemText(hdlg,ID_NUMBER,str,20);
	          		gDrawData.number=(long int)(atof(str));
	          		if(gDrawData.number < 1 || 
	             		gDrawData.number > 10 )
	          		{
	            		MessageBox(hdlg,
	              		"Number should be between 1 and 10.", 
	              		"Warning!",MB_ICONERROR);
	          		} 
	          		else{
	          			EndDialog(hdlg,TRUE);
			  		}
	          
	          		return 1;
	          		break;

        		case ID_CANCEL:
          			EndDialog(hdlg,TRUE);
          			break;
      		}
      		break;
    	default:
      		break;
  	}	
  	return 0;
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

    case WM_MOUSEMOVE:
      if(wParam & MK_LBUTTON)
      {
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        processMouseMove(hwnd, x,y);
      }
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
    
    default:
      break;
  }
  // Call the default window handler
  return DefWindowProc(hwnd, message, wParam, lParam);
}

bool replaceColor(HDC hdc, long x, long y,
                  COLORREF clrFrom, COLORREF clrTo)
{
  //returns true if replacement is applicable,otherwise false
  if(GetPixel(hdc, (int) x, (int) y) == clrFrom)
  {
    SetPixel(hdc, (int) x, (int) y, clrTo);
    return true;
  }
  return false;
}

void performCorrectionAtClipPts(HDC hdc, 
      POINT clipPt, COLORREF clrLine, COLORREF clrErase)
{
  POINT pt;

  if ((clipPt.x == gDrawData.clipMin.x) || 
      (clipPt.x == gDrawData.clipMax.x))
  {
    // on vertical side of clipping
    pt.x = clipPt.x;
    pt.y = clipPt.y;    
    while (replaceColor(hdc, pt.x, pt.y, clrLine, clrErase))
    {
      pt.y--;
    }
    pt.x = clipPt.x;
    pt.y = clipPt.y+1;
    while (replaceColor(hdc, pt.x, pt.y, clrLine, clrErase))
    {
      pt.y++;
    }    
  }
  if ((clipPt.y == gDrawData.clipMin.y) || 
      (clipPt.y == gDrawData.clipMax.y))
  {
    // on horizontal side of clipping
    pt.x = clipPt.x;
    pt.y = clipPt.y;    
    while (replaceColor(hdc, pt.x, pt.y, clrLine, clrErase))
    {
      pt.x--;
    }
    pt.x = clipPt.x+1;
    pt.y = clipPt.y;
    while (replaceColor(hdc, pt.x, pt.y, clrLine, clrErase))
    {
      pt.x++;
    }
  }
}

void drawRectangle(HDC hdc, POINT corner1, POINT corner2, 
                   COLORREF clr)
{
  POINT pt1, pt2;
  pt1.x = corner1.x;
  pt1.y = corner2.y;
  pt2.x = corner2.x;
  pt2.y = corner1.y;
  
  drawLineSegment(hdc, corner1, pt1, clr);
  drawLineSegment(hdc, pt1, corner2, clr);
  drawLineSegment(hdc, corner2, pt2, clr);
  drawLineSegment(hdc, pt2, corner1, clr);
}
