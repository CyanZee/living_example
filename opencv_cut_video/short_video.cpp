#include <iostream>
#include <opencv2/opencv.hpp>

//#include <stdlib.h>
//#include <stdio.h>
//#include <sys/types.h>
//#include <unistd.h>
//#include <sys/wait.h>
//#include <zlog.h>

#include "short_video.h"

using namespace cv;
using namespace std;

ShortVideo::ShortVideo() {
}

ShortVideo::~ShortVideo() {}

bool ShortVideo::CutByTimePoint(const string &infile, const string &outfile, int start, int end) {
	return true;	
}

bool ShortVideo::CutByFramePoint(const string &infile, const string &outfile, int start, int end) {

	VideoCapture capture(infile.c_str());
	//VideoCapture capture("./test.avi");
	float fps = capture.get(CV_CAP_PROP_FPS);
	long framewidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	long framehight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

	//VideoWriter writer("./shortvideo.avi",CV_FOURCC('M','J','P','G'),fps,Size(framewidth,framehight));
	VideoWriter writer(outfile.c_str(),CV_FOURCC('D','I','V','X'),fps,Size(framewidth,framehight));
	//VideoWriter writer(outfile.c_str(),CV_FOURCC('X','2','6','4'),fps,Size(framewidth,framehight));
	
	int i = 0;
	while(1)
	{	
		Mat frame;
		capture >> frame;
		i++;
		if(frame.empty())
		{
			break;
		}
		if(i>start&i<end)
		{
			//imshow("./shortvideo.avi",frame);
			writer<<frame;
			//waitKey(5);	
		}
		if(i==end)
		{
			break;
		}
	}
	return true;
}

bool ShortVideo::CutByTimePoint(const string &infile, const string &outfile, int origin, int left, int right) {
		return true;	
}

bool ShortVideo::CutByFramePoint(const string &infile, const string &outfile, int origin, int left, int right) {
	return true;
}

