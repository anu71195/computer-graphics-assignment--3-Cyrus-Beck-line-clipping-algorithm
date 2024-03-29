#define ID_DRAW_POLYGON 40001
#define ID_CLEAR          40002
#define ID_EXIT           40003
#define ID_DRAW_LINE      40004
#define ID_CLIP           40005

const COLORREF CLR_BG = RGB(255, 255, 255);
const COLORREF CLR_RECT = RGB(128, 128, 128);
const COLORREF CLR_LINE = RGB(0, 255, 0);
const COLORREF CLR_CLIP = RGB(255, 0, 0);

enum MODE
{
	READY_MODE,
	DRAW_RECTANGLE_MODE,
	DRAW_POLYGON_MODE,
	DRAWN_POLYGON_MODE,
	DRAW_LINE_MODE,
	CLIP_MODE,
	CLIPPED_MODE
};

const int nMaxNoOfCornerPts = 100;

class DRAWING_DATA
{
public:
	HDC hdcMem;
	HBITMAP hbmp;
	MODE drawMode;
	POINT beginPt, endPt;

	POINT cornerPts[nMaxNoOfCornerPts];/* corner points of polygon */
	SIZE maxBoundary;
	int orientation;
	int nCornerPts; /* number of corner points of the polygon */

	int overlap;
	int nIntrscn;
	double lambda[2];
	double xIntrscn[2],yIntrscn[2];
	POINT intrscn[2];

	POINT rectCornerPts[2];
	POINT clipMin, clipMax;
	POINT lineEndPts[2];

	void reset();
	bool checkForConvexity(int, int);
	void intersection();
};

extern DRAWING_DATA gDrawData; // global data

void reDraw(HWND hwnd);
void setDrawMode(MODE mode, HWND hwnd);
//void performCorrectionAtClipPts(HDC hdc, POINT clipPt, COLORREF clrLine, COLORREF clrErase);
//void drawRectangle(HDC hdc, POINT corner1, POINT corner2, COLORREF clr);
//void clip(HWND hwnd);
//bool isPointWithinClipBoundary(POINT pt);
//bool isLineWithinClipBoundary(POINT start, POINT end);
//bool isLineOutsideClipBoundary(POINT start, POINT end);
//void computeClipDimensions();
void processRightButtonDown(HWND hwnd);
void drawClippedLine();
