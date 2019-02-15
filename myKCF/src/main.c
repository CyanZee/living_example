#include "KCFTrackerEX.h"
#include <iostream>

#define IMAGES 150

using namespace std;
using namespace cv;

int main()
{
	KCFTrackerEx oKCFTrackerEx;

	char picture[125];
	Mat frame;
	
	int m_nObjNum = 10; //dectionOutputInfo1.m_nObjNum
	for(int n = 0; n < IMAGES; n++)
	{
		//step1 deal
		snprintf(picture,125,"images/%d.jpg",n);
		cout << "+++ " << picture << endl;
		frame = imread(picture);
		//frame = imread("test.jpg");

		oKCFTrackerEx.KCFTrackerUpdate(frame);

		Rect rect1;
		Mat frame2;
		for(int i = 0; i < m_nObjNum; i++)
		{
			/*
			rect1.x = 
			rect1.y = 
			rect1.width = 
			rect1.height = 
			*/
			if(rect1.x < 0)
			{
				rect1.width = frame.cols + rect1.x;
				rect1.x = 0;
			}
			if(rect1.y < 0)
			{
				rect1.height = frame.rows + rect1.y;
				rect1.y = 0;
			}
			if(rect1.x + rect1.width > frame.cols)
			{
				rect1.width = frame.cols - rect1.x;
			}
			if(rect1.y + rect1.height > frame.rows)
			{
				rect1.height = frame.rows - rect1.y;
			}

			if(rect1.width <= 0 || rect1.height <= 0)
			{
				cout << "--- step1 continue." << endl;
				continue;
			}
			frame2 = frame(rect1).clone();			
			snprintf(picture,125,"befores/%d-%d.jpg",n,i);
			imwrite(picture, frame2); 

			if(oKCFTrackerEx.iKCFTrackerCheck(rect1.x, rect1.y, rect1.width, rect1.height))
			{
				snprintf(picture,125,"removes/%d-%d.jpg",n,i);
				imwrite(picture, frame2);
				cout << "AntionChan:This is old target, drop it! " << picture << endl;
				//continue;
			}
			else
			{
				snprintf(picture,125,"remainders/%d-%d.jpg",n,i);
				imwrite(picture, frame2);
			}
		}
	}
        return 0;
}

