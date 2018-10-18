#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 3)
        {
                printf("Usage: ./main infile outfile.\n");
                return 1;
        }
	
	VideoCapture cap(argv[1]);
	float fps = cap.get(CV_CAP_PROP_FPS);
	long framewidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	long framehight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	VideoWriter writer(argv[2],CV_FOURCC('M','J','P','G'),fps,Size(framewidth,framehight));
	
	int i = 0;
	while(1)
	{	
		Mat frame;
		cap >> frame;
		i++;
		if(frame.empty())
		{
			break;
		}
		if(i>0&i<250)
		{
			//imshow(argv[2],frame);
			writer<<frame;
			//waitKey(5);	
		}
	}
	return 0;
}
