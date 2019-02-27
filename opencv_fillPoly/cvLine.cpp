#include <cv.h>
#include <highgui.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#define WIDTH 256
#define HEIGHT 256

using namespace cv;

void draw_external_contour_gray(CvSeq *seq, IplImage* grayImage)
{
	if(seq->total < 2)
	{ 
		return;
	}
	CvPoint* prePoint = (CvPoint*)cvGetSeqElem(seq, 0);
	CvPoint* lastPoint = (CvPoint*)cvGetSeqElem(seq, seq->total - 1);
	cvLine(grayImage, *prePoint, *lastPoint,cvScalar(255), 1, 8, 0);
	for(int i = 1; i < seq->total; i++) 
	{
		CvPoint *p;
		p = (CvPoint*)cvGetSeqElem(seq, i);
		cvLine(grayImage, *prePoint, *p, cvScalar(255), 1, 8, 0);
		*prePoint = *p;
	}
}

int main()
{
	Mat image = imread("test.jpg");
	Mat mask = Mat::zeros(image.size(), CV_8UC1);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeqWriter writer;
	cvStartWriteSeq(CV_32SC2,sizeof(CvSeq), sizeof(CvPoint),storage,&write);
	CvPoint p1 = {357,916}; CvPoint p2 = {720,268}; CvPoint p3 = {1303,300}; CvPoint p4 = {1658,998};
	CV_WRITE_SEQ_ELEM(p1,writer);
	CV_WRITE_SEQ_ELEM(p2,writer);
	CV_WRITE_SEQ_ELEM(p3,writer);
	CV_WRITE_SEQ_ELEM(p4,writer);
	
	cvFlushSeqWriter(&writer);
	CvSeq* contour = cvEndWriteSeq(&writer);
	printf("contour.size = %d", contour->total);
	IplImage* imask = &IplImage(mask);
	IplImage* iimage = &IplImage(image);

	draw_external_contour_gray(contour, imask);
	
	Mat maskImage;
	image.copyTo(maskImage, mask);
	imwrite("mask.jpg",maskImage);
	return 0;
}	
	
