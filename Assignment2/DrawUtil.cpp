#include <windows.h>
#include "Fill.h"

LRESULT CALLBACK DlgAxis(HWND hdlg,UINT mess,WPARAM more,LPARAM pos);
DRAWING_DATA gDrawData; // global data

void reDraw(HWND hwnd)
{
	InvalidateRect(hwnd, NULL, 1);
}

void drawPoint(int x, int y)
{
  	SetPixel(gDrawData.hdcMem, x, y, RGB(0,255,255));
}

void setpoint(int x, int y, int cx, int cy)
{
	drawPoint(cx + x, cy +y);
	drawPoint(cx+x, cy -y);
	drawPoint(cx -x, cy +y);
	drawPoint(cx - x, cy -y);
	drawPoint(cx + y, cy +x);
	drawPoint(cx+y, cy -x);
	drawPoint(cx -y, cy +x);
	drawPoint(cx - y, cy -x);
}

void circle(int cx, int cy, int r)
{
	int x = 0;
	int y = r;
	
	double p = 5/4 - r;
	while(y >=x)
	{
		if(p<0)
		{
			x = x+1;
			y = y;
			p  = p + 2*x + 1;
		}
		else
		{
			x = x+ 1;
			y = y - 1;
			p = p + 2*x + 1 - 2*y;
		}
		setpoint(x,y,cx,cy);
	}
}


void drawCircle(int x, int y)
{
    circle(x,y,gDrawData.radius);
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
  	gDrawData.nCornerPts = 0;
  	gDrawData.hDrawPen=CreatePen(PS_SOLID, 1, CLR_BOUNDARY);
  	gDrawData.hFillPen=CreatePen(PS_SOLID, 1, CLR_FILL);

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
  	gDrawData.nCornerPts = 0;
  	gDrawData.drawMode = READY_MODE;

  	PatBlt(gDrawData.hdcMem, 0, 0, gDrawData.maxBoundary.cx, 
    gDrawData.maxBoundary.cy, PATCOPY);

  	reDraw(hwnd);
  	setupMenus(hwnd);
}

void CaptureCentres(HWND hwnd, int x, int y)
{
  /* the coordinates of the points are stored in an array */

  	if (gDrawData.nCornerPts < nMaxNoOfCornerPts)
  	{
    	SelectObject(gDrawData.hdcMem, gDrawData.hDrawPen);
    	drawPoint(x,y);
    	gDrawData.cornerPts[gDrawData.nCornerPts].x = x;
    	gDrawData.cornerPts[gDrawData.nCornerPts].y = y;
    	gDrawData.nCornerPts++;
    	drawCircle(x, y);
  	}  
}


void drawImage(HDC hdc)
{
  	BitBlt(hdc, 0, 0, gDrawData.maxBoundary.cx, 
    gDrawData.maxBoundary.cy, gDrawData.hdcMem, 0, 0, SRCCOPY);
}

void processLeftButtonDown(HWND hwnd, int x, int y)
{
	  switch (gDrawData.drawMode)
  	{
	    case DRAW_MODE:
		    CaptureCentres(hwnd,x,y);
      		reDraw(hwnd);
      		break;

    	default:
      		return;
  	}
}

void processRightButtonDown(HWND hwnd)
{
	MessageBox(hwnd,"Drawing Complete","Remember",MB_OK);
  	if (gDrawData.drawMode == DRAW_MODE)
  	{
	    setDrawMode(DRAWN_MODE, hwnd);
	    reDraw(hwnd);
  	}
}

void processCommonCommand(int cmd, HWND hwnd)
{
  	int response;
  	switch(cmd)
  	{
	    case ID_CLEAR:
	      reset(hwnd);
	      setDrawMode(READY_MODE, hwnd);
	      break;
	    case ID_CIRCLE:
		    DialogBox(NULL,"MyDB",hwnd,(DLGPROC)DlgAxis);
    		MessageBox(hwnd,"Click the right button after drawing all circles","Remember",MB_OK);
      		setDrawMode(DRAW_MODE, hwnd);
      		break;
    	case ID_EXIT:
      		response=MessageBox(hwnd,"Quit the program?", "EXIT",
                          MB_YESNO);
      		if(response==IDYES)
        		PostQuitMessage(0);
      		break;
    	default:
      		break;
  	}
}

LRESULT CALLBACK DlgAxis(HWND hdlg,UINT mess,WPARAM more,LPARAM pos)
{
  	char str[20];
  	switch(mess)
  	{
	    case WM_COMMAND:
	     	switch(more)
	      	{
		        case ID_OK:
	    		    GetDlgItemText(hdlg,ID_RADIUS,str,20);
	          		gDrawData.radius=(long int)(atof(str));
	          		if(gDrawData.radius < 5 || 
	             		gDrawData.radius > 200 )
	          		{
	            		MessageBox(hdlg,
	              		"Radius should be between 5 and 200.", 
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

    	case WM_RBUTTONDOWN:
      		processRightButtonDown(hwnd);
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
