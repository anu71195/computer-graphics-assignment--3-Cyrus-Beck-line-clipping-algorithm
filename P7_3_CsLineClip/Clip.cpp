#include <windows.h>
#include <utility>
#include "Line.h"
#include "ClipUtil.h"
#include <stdio.h>
#include <math.h>
//int code(double x, double y)
//{
//	return (((x < gDrawData.clipMin.x) << 3) +
//		((x > gDrawData.clipMax.x) << 2) +
//		((y < gDrawData.clipMin.y) << 1) +
//		(y > gDrawData.clipMax.y));
//}
//
//void scLineClip(POINT start, POINT end)
//{
//	bool bVerticalLine;
//	float m;
//	double x1Clip, y1Clip, x2Clip, y2Clip;
//	int c1, c2;
//	POINT clipStart, clipEnd;
//
//	// groundwork before drawing contiguous line segments for clipping
//	setupLineSegmentDrawing(gDrawData.hdcMem, start, end);
//	calculateSlope(start, end, bVerticalLine, m);
//
//	x1Clip = start.x;
//	y1Clip = start.y;
//	x2Clip = end.x;
//	y2Clip = end.y;
//
//	c1 = code(x1Clip, y1Clip); /* region code of start point*/
//	c2 = code(x2Clip, y2Clip); /* region code of end point*/
//
//	while (c1 | c2)
//	{
//		/* two points are not inside the rectangle */
//		if (c1 & c2)
//		{
//			// line is completely outside, erase entire line
//			drawNextLineSegment(end, CLR_BG);
//			return;
//		}
//		if (c1)
//		{
//			if (c1 & 8)
//			{
//				y1Clip += m * (gDrawData.clipMin.x - x1Clip);
//				x1Clip = gDrawData.clipMin.x;
//			}
//			else if (c1 & 4)
//			{
//				y1Clip += m * (gDrawData.clipMax.x - x1Clip);
//				x1Clip = gDrawData.clipMax.x;
//			}
//			else if (c1 & 2)
//			{
//				if (!bVerticalLine)
//					x1Clip += (gDrawData.clipMin.y - y1Clip) / m;
//				y1Clip = gDrawData.clipMin.y;
//			}
//			else if (c1 & 1)
//			{
//				if (!bVerticalLine)
//					x1Clip += (gDrawData.clipMax.y - y1Clip) / m;
//				y1Clip = gDrawData.clipMax.y;
//			}
//			c1 = code(x1Clip, y1Clip);
//		}//End OF if(c1)
//		else // c1 is false, so c2 must be true
//		{
//			if (c2 & 8)
//			{
//				y2Clip += m * (gDrawData.clipMin.x - x2Clip);
//				x2Clip = gDrawData.clipMin.x;
//			}
//			else if (c2 & 4)
//			{
//				y2Clip += m * (gDrawData.clipMax.x - x2Clip);
//				x2Clip = gDrawData.clipMax.x;
//			}
//			else if (c2 & 2)
//			{
//				if (!bVerticalLine)
//					x2Clip += (gDrawData.clipMin.y - y2Clip) / m;
//				y2Clip = gDrawData.clipMin.y;
//			}
//			else if (c2 & 1)
//			{
//				if (!bVerticalLine)
//					x2Clip += (gDrawData.clipMax.y - y2Clip) / m;
//				y2Clip = gDrawData.clipMax.y;
//			}
//			c2 = code(x2Clip, y2Clip);
//		}//End OF else
//	}//End of while
//	clipStart.x = (long)(x1Clip);
//	clipStart.y = (long)(y1Clip);
//	clipEnd.x = (long)(x2Clip);
//	clipEnd.y = (long)(y2Clip);
//
//	drawNextLineSegment(clipStart, CLR_BG);
//	drawNextLineSegment(clipEnd, CLR_LINE);
//	drawNextLineSegment(end, CLR_BG);
//	performCorrectionAtClipPts(gDrawData.hdcMem, clipStart, CLR_LINE, CLR_BG);
//	performCorrectionAtClipPts(gDrawData.hdcMem, clipEnd, CLR_LINE, CLR_BG);
//}
//
//void clip(HWND hwnd)
//{
//	scLineClip(gDrawData.lineEndPts[0], gDrawData.lineEndPts[1]);
//	reDraw(hwnd);
//	setDrawMode(CLIPPED_MODE, hwnd);
//}

std::pair<double, double> findPoint(POINT P, POINT Q, double t)
{
	return std::make_pair(t*(Q.x - P.x) + P.x, t*(Q.y - P.y) + P.y);
}

void drawClippedLine()
{
	POINT A, B;
	if (gDrawData.overlap < 0)
	{
		A = gDrawData.lineEndPts[0];
		B = gDrawData.lineEndPts[1];
	}
	else
	{
		A = gDrawData.cornerPts[gDrawData.overlap];
		B = gDrawData.cornerPts[(gDrawData.overlap + 1) % gDrawData.nCornerPts];
	}
	if (gDrawData.nIntrscn < 1)
		return;
	SelectObject(gDrawData.hdcMem, CreatePen(PS_SOLID, 1, CLR_CLIP));
	MoveToEx(gDrawData.hdcMem, A.x, A.y, NULL);
	LineTo(gDrawData.hdcMem, B.x, B.y);
}

void DRAWING_DATA::intersection()
{
	//POINT P = { 0,0 }, POINT Q = { 0,0 };
	POINT &P = lineEndPts[0];
	POINT &Q = lineEndPts[1];
	vec PQ(P, Q), S;
	long numerator, denominator;
	//double t, l;
	overlap = -1;
	nIntrscn = 0;
	for (int i = 0; i < nCornerPts; i++)
	{
		S = vec(cornerPts[i], cornerPts[(i + 1) % nCornerPts]);
		numerator = cross(vec(cornerPts[i], P), PQ);
		denominator = cross(S, PQ);
		if (denominator == 0) //parallel
		{
			if (numerator == 0) //overlapping
			{
				overlap = i;
				return;
			}
			//non intersecting
			continue;
		}
		else if (abs(numerator) > abs(denominator) || (denominator*numerator <= 0))
			continue;	//outside the side
		//t = (double)numerator / denominator;
		numerator = cross(S, vec(P, cornerPts[i]));
		lambda[nIntrscn] = (double)numerator / denominator;
		nIntrscn++;
		if (nIntrscn > 1)
			break;
	}

	switch (nIntrscn)
	{
	case 2:
		if (lambda[0] > lambda[1])
			std::swap(lambda[0], lambda[1]);
		if (lambda[0] > 1 || lambda[1] < 0)
		{
			nIntrscn = 0;
			break;
		}
		else
		{
			if (lambda[0] > 0)
			{
				//std::pair<double, double> dP = findPoint(beginPt, endPt, lambda[0]);
				//P = POINT{ (long)dP.first, (long)dP.second };
				double pxterm = lambda[0] * (endPt.x - beginPt.x),
				       pyterm = lambda[0] * (endPt.y - beginPt.y);
				if (pxterm > 0)
					pxterm = ceil(pxterm);
				else
					pxterm = floor(pxterm);
				if (pyterm > 0)
					pyterm = ceil(pyterm);
				else
					pyterm = floor(pyterm);
				P = { (long)pxterm + beginPt.x, (long)pyterm + beginPt.y };
			}
			if (lambda[1] < 1)
			{
				//std::pair<double, double> dQ = findPoint(beginPt, endPt, lambda[1]);
				//Q = POINT{ (long)dQ.first, (long)dQ.second };
				double pxterm = lambda[1] * (endPt.x - beginPt.x),
				       pyterm = lambda[1] * (endPt.y - beginPt.y);
				if (pxterm < 0)
					pxterm = ceil(pxterm);
				else
					pxterm = floor(pxterm);
				if (pyterm < 0)
					pyterm = ceil(pyterm);
				else
					pyterm = floor(pyterm);
				Q = { (long)pxterm + beginPt.x, (long)pyterm + beginPt.y };
			}
		}
		break;
	case 1:
	{
		std::pair<double, double> dP = findPoint(P, Q, lambda[0]);
		P = Q = POINT{ (long)round(dP.first), (long)round(dP.second) };
		break;
	}
	default:
		break;
	}
}
