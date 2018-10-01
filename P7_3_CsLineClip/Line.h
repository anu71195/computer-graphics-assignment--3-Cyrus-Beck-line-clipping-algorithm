class vec : public POINT
{
public:
	vec(long x, long y) : POINT{x,y}
	{}

	vec(POINT A, POINT B) : POINT{B.x - A.x, B.y - A.y}
	{}

	vec() : POINT {0,0}
	{}

	friend LONG cross(vec A, vec B)
	{
		return A.x*B.y - B.x*A.y;
	}
};

void calculateSlope(POINT start, POINT end, bool& bVerticalLine, float& m);
// For Bresenham's line drawing 
void drawLineSegment(HDC hdc, POINT start, POINT end, COLORREF clr);
void drawNextLineSegment(POINT end, COLORREF clrLine);
void setupLineSegmentDrawing(HDC hdc, POINT& start, POINT& end);
bool findNextPtInLineSegment(POINT end, POINT& nextPt);
// For scan line drawing 
void drawScanLineSegment(HDC hdc, POINT start, POINT end, COLORREF clr);
void drawNextScanLineSegment(POINT end, COLORREF clr);
void setupScanLineSegmentDrawing(HDC hdc, POINT& start, POINT& end);
bool findNextPtInScanLineSegment(POINT end, POINT& nextPt);
