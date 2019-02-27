#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

void ploygon(Mat img)
{
	cout << "Start to draw..." << endl;
	int lineType = 8;
	Point rook_points[4];
	rook_points[0] = Point(1007,3607);
	rook_points[1] = Point(2197,817);
	rook_points[2] = Point(4642,908);
	rook_points[3] = Point(4881,3566);
	const Point* ppt[1] = {rook_points};
	int npt[] = {4};
	polylines(img, ppt, npt, 1, 1, Scalar(0,255,255),2,8,0);
//	fillPoly(img, ppt, npt, 1, Scalar(0,255,255),lineType);
	cout << "It is going to write img." << endl;
	imwrite("result.jpg",img);	
	
}
int main(void)
{
	Mat img = imread("test.jpg");
	ploygon(img);
	return 0;	
}
