#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc,char const* const* argv)
{
	char const* infile;
	char picname[125];
	int i = 0;

	infile = argv[1];	

	cout << "+++ The infile is: " << infile << endl;
	VideoCapture capture(infile);
	
	while(1)
	{
		Mat frame;
		capture >> frame;
		if(frame.empty())
		{
			break;
		}
		snprintf(picname,125,"image/%d.jpg",i);	
//		cout << "+++ The picname is: " << endl;
		i++;
		imwrite(picname,frame);
	}
	return 0;
}
