#ifndef _MYFFMPEG_H_
#define _MYFFMPEG_H_

extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h" 
#include "libswscale/swscale.h" 
#include "libavutil/dict.h" 
}

#include <string>

class MyFFmpeg
{
public:
	static MyFFmpeg *Get()
	{
		static MyFFmpeg ff;
		return &ff;
	}
	
	bool Open(const char *path);
	void Close();
	AVPacket Read();
	std::string GetError();
	virtual ~MyFFmpeg();
	int totalMs = 0;

protected:
	char errorbuf[1024];
	AVFormatContext *ac = NULL;
	AVCodec *pCodec;
	AVCodecContext *pCodecCtx;
	MyFFmpeg();

};
#endif
