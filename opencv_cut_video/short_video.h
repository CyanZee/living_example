#ifndef _SHORT_VIDEO_H_
#define _SHORT_VIDEO_H_

#include <string.h>


class ShortVideo {
public:
	ShortVideo();
	~ShortVideo();
	
	bool CutByTimePoint(const std::string &infile, const std::string &outfile, int origin, int left, int right);
	bool CutByTimePoint(const std::string &infile, const std::string &outfile, int start, int end);     //cut a video around time-point
	bool CutByFramePoint(const std::string &infile, const std::string &outfile, int origin,int left, int right);
	bool CutByFramePoint(const std::string &infile, const std::string &outfile, int start, int end);   //cut a video around frame-point
};


#endif










