#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

bool InPolygon(Point p, Point pt[], int nCount);

int main()
{
 
	int npt = 4;
	Point points[4];
	points[0] = Point(1007,3607);
	points[1] = Point(2197,817);
	points[2] = Point(4642,908);
	points[3] = Point(4881,3566);
	Point t_point(3304,2526);
	if(InPolygon(t_point, points, npt))
	{
		cout << "The t_point is in this polygon." << endl;
	}
	else
	{
		cout << "The t_point is out." << endl;
	}	

	return 0;
}

bool InPolygon(Point p, Point pt[], int nCount)
{
	int nCross = 0;
	for(int i = 0; i < nCount; i++)
	{
		Point p1 = pt[i];
		Point p2 = pt[(i+1) % nCount];
		if(p1.y == p2.y)
		{
			if(p.y == p1.y && p.x >= min(p1.x,p2.x) && p.x <= max(p1.x,p2.x))
			{
				return true;
			}
			continue;
		}
		if(p.y < min(p1.y,p2.y) || p.y > max(p1.y,p2.y))
		{
			continue;
		}
		double x = (double)(p.y - p1.y) * (double)(p2.x - p1.x)/ (double)(p2.y - p1.y) + p1.x;
		
		if(x > p.x)
		{
			nCross++;
		}
		else if(x == p.x)
		{
			return true;
		}
	}
	if(nCross % 2 == 1)
	{
		return true;
	}
	return false;
}


