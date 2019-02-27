#include <iostream>
#include "short_video.h"

//#define COMMAND_CALL

#ifdef COMMAND_CALL
void tester()
{
	std::cout << "There is command call" << std::endl;
}
#else
void tester()
{
	std::cout << "There is non " << std::endl;
}
#endif

int main(void) {
	std::string infile = "test01.mp4";
	std::string outfile = "test01_shortvideo.mp4";
	tester();
	ShortVideo shortvideo;
//	while(1)
//	{
	if(shortvideo.CutByFramePoint(infile, outfile, 250, 500))
	{
		std::cout << "Success to cut a short video." << std::endl;
	}else {
		std::cout << "Failed to cut a short video." << std::endl;
	}
//	}
	return 0;
}
