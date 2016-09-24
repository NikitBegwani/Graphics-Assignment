#define ID_CIRCLE          40001
#define ID_CLEAR           40002
#define ID_EXIT            40003
#define ID_FILL            40004
#define ID_OK       	   40005
#define ID_CANCEL          40006
#define ID_RADIUS          40007
#define ID_LABEL1          40008



const COLORREF CLR_FILL = RGB(0, 0, 200);
const COLORREF CLR_BOUNDARY = RGB(0, 0, 0);
const COLORREF CLR_BG = RGB(255, 255, 255);

typedef enum
{
	READY_MODE,
	DRAW_MODE,
	DRAWN_MODE,
	FILL_MODE,
	FILLED_MODE
} MODE;

const int nMaxNoOfCornerPts = 3;

typedef struct
{
  	HDC hdcMem; 
  	HBITMAP hbmp;
  	HPEN hDrawPen, hFillPen;
  	MODE drawMode;
  	POINT cornerPts[nMaxNoOfCornerPts];/* corner points of polygon */
  	SIZE maxBoundary;
  	int nCornerPts; /* number of corner points of the polygon */ 
  	int radius;
} DRAWING_DATA;

typedef struct
{
	int yMin, yMax;
  	double x;
  	double mInv;
} EDGE_ENTRY;

extern DRAWING_DATA gDrawData; // global data being shared across files

void setupMenus(HWND hwnd);
void processRightButtonDown(HWND hwnd);
void processCommand(int cmd, HWND hwnd);
void setDrawMode(MODE mode, HWND hwnd);
void reDraw(HWND hwnd);
void processCommonCommand(int cmd, HWND hwnd);
