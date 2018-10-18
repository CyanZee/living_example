#include "MyFFmpeg.h"
#include <iostream>

bool MyFFmpeg::Open(const char *path)
{
	avformat_network_init();

	if(!(ac = avformat_alloc_context()))
	{	
		printf("--- Can't alloc context.\n");
		return false;
	}

	int result = avformat_open_input(&ac, path, NULL, NULL);
	if (result != 0)
	{
		av_strerror(result, errorbuf, sizeof(errorbuf));
		printf("--- Failed to avformat_open_input().\n");
		return false;
	}
	if (avformat_find_stream_info(ac, NULL) < 0) 
	{
		printf("--- Can't get stream info\n");
		return false;
	}

	av_dump_format(ac, 0, path, 0);

	int videoStream = -1;
	for(int i = 0; i < ac->nb_streams; i++) 
	{
		if(ac->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) 
		{
			videoStream = i;
			break;
		}
	}
	if(videoStream == -1) 
	{
		return -1;
	}
	AVCodecParameters* codecpar = ac->streams[videoStream]->codecpar;

	std::cout << "+++ codecpar->codec_id = " << codecpar->codec_id << std::endl;
	if(AV_CODEC_ID_H264 == codecpar->codec_id)
	{
		std::cout << "+++ entering... 1" << std::endl;
		pCodec = avcodec_find_decoder_by_name("h264_cuvid");
		if(pCodec == NULL) 
		{
			std::cout << "--- Unsupported codec!" << std::endl;
			return -1;
		}
	}
	else if(AV_CODEC_ID_MPEG4 == codecpar->codec_id)
	{
		std::cout << "+++ entering... 2" << std::endl;
		pCodec = avcodec_find_decoder_by_name("mpeg4_cuvid");
		if(pCodec == NULL) 
		{
			std::cout << "--- Unsupported codec!" << std::endl;
			return -1;
		}
	}
	else
	{
		std::cout << "+++ entering... 3" << std::endl;
		pCodec = avcodec_find_decoder_by_name("h264_cuvid");
		if(pCodec == NULL) 
		{
			std::cout << "--- Unsupported codec!" << std::endl;
			return -1;
		}
	}

	av_format_set_video_codec(ac, pCodec);
	pCodecCtx = avcodec_alloc_context3(pCodec);
	if(!pCodecCtx)
	{
		std::cout << "--- Can't allocate decoder context." << std::endl;
		return -1;
	}

	if(avcodec_parameters_to_context(pCodecCtx, codecpar) != 0) 
	{
		std::cout << "--- Couldn't copy codec context" << std::endl;
		return -1;
	}
	if(avcodec_parameters_from_context(codecpar, pCodecCtx) != 0) 
	{
		std::cout << "--- Error initializing the decoder context." << std::endl;
		return -1;
	}

	if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0) 
	{
		std::cout << "--- Error avcodec_open2() exit." << std::endl;
		return -1;
	}

	//totalMs = ((ac->duration / AV_TIME_BASE)*1000);
	return true;
}

void MyFFmpeg::Close()
{
	if (ac) avformat_close_input(&ac);
}

std::string MyFFmpeg::GetError()
{
	std::string result = this->errorbuf;
	return result;
}

AVPacket MyFFmpeg::Read()
{
	AVPacket pkt;
	memset(&pkt, 0, sizeof(AVPacket));
	if (!ac)
	{
		return pkt;
	}
	int err = av_read_frame(ac, &pkt);
	if (err != 0)
	{
		av_strerror(err, errorbuf, sizeof(errorbuf));
	}
	return pkt;
}

MyFFmpeg::MyFFmpeg()
{
	errorbuf[0] = '\0';
	av_register_all();
}

MyFFmpeg::~MyFFmpeg()
{
}


