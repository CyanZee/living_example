#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

//#include <rga/RgaApi.h>
#include <opencv2/opencv.hpp>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
	#include <libavutil/hwcontext_drm.h>
    //#include "rkdrm_display.h"
}

static void decode(AVCodecContext *ctx, AVPacket *packet) {
	AVFrame *pFrame = av_frame_alloc();

	int ret = 0;
	if((ret = avcodec_send_packet(ctx, packet)) < 0) {
		printf("avcodec_send_packet\n");
		return;
	}
	static int sum = 0;

	while(1) {
		ret = avcodec_receive_frame(ctx, pFrame);
		if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			printf("need more packet\n");
			av_frame_free(&pFrame);
			return;
		}else if(ret < 0) {
			printf("--- ret: %d\n", ret);
			return;
		}

		AVDRMFrameDescriptor *desc = (AVDRMFrameDescriptor *) pFrame->data[0];

		if(sum == 10) {
            //FILE *fp_yuv = fopen("output.yuv", "wb+");
            //fwrite(desc->objects[0].ptr, 1, 1280*720*1.5, fp_yuv); //V
            //fclose(fp_yuv);

            printf("+++ save to jpg.\n");
			cv::Mat yuv_nv12;
			int y_size = ctx->width * ctx->height;
            yuv_nv12.create(720*3/2, 1280, CV_8UC1);
            memcpy((void*)yuv_nv12.data, desc->objects[0].ptr , y_size*3/2);

			cv::Mat rgbImg;
			cv::cvtColor(yuv_nv12, rgbImg, CV_YUV2BGR_NV12);
			imwrite("output.jpg", rgbImg);
		}

        sum++;
		printf("+++ decode success \n");
	}
}


int main(int argc, char *argv[]) {
    printf("+++ Enter testing... \n");

    //char rtspUrl[] = "SampleVideo_1280x720_5mb.mp4";
    char rtspUrl[] = "rtsp://admin:linux666@192.168.1.140:554/h264/ch1/main/av_stream";

    AVFormatContext *pFormatCtx;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec;
    AVFrame         *pFrame;
    AVPacket        *packet;
    AVDictionary *dict = NULL;

    av_register_all();
    avformat_network_init();

    pFormatCtx = avformat_alloc_context();

    av_dict_set(&dict, "rtsp_transport", "tcp", 0);
    av_dict_set(&dict, "stimeout", "30000000", 0);

    if(avformat_open_input(&pFormatCtx, rtspUrl, NULL, &dict) != 0) {
        printf("Couldn't open input stream.\n");
        return -1;
    }

    if(avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        printf("Couldn't find stream information. \n");
        return -1;
    }

    av_dump_format(pFormatCtx, 0, rtspUrl, 0);

    int videoStream = -1;
    for(int i = 0; i < pFormatCtx->nb_streams; i++) {
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }

    if(videoStream == -1) {
        printf("Didn't find a video stream. \n");
        return -1;
    }

    AVCodecParameters *codecpar = pFormatCtx->streams[videoStream]->codecpar;
    if(AV_CODEC_ID_H264 == codecpar->codec_id) {
        printf("use h264 decoder... \n");
        pCodec = avcodec_find_decoder_by_name("h264_rkmpp");
        if(pCodec == NULL) {
            printf("Unsupported h264 codec! \n");
            return -1;
        }
    } else {
        pCodec = avcodec_find_decoder(codecpar->codec_id);
        if(pCodec == NULL) {
            printf("Unsupported codec by codec_id.\n");
            return -1;
        }
    }
    av_format_set_video_codec(pFormatCtx, pCodec);

    pCodecCtx = avcodec_alloc_context3(pCodec);
    if(!pCodecCtx) {
        printf("Can't allocate decoder context.\n");
        return -1;
    }

    //pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->pix_fmt = AV_PIX_FMT_DRM_PRIME;

    if(avcodec_parameters_to_context(pCodecCtx, codecpar) != 0) {
		printf("Couldn't copy codec context.\n");
		return -1;
	}
	if(avcodec_parameters_from_context(codecpar, pCodecCtx) != 0) {
		printf("Error initializing the decoder context.\n");
		return -1;
	}

	if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		printf("Error avcodec_open2() exit. \n");
		return -1;
	}
    
    //Output Info -------------------
    printf("-----------  File Information ---------------\n");
    av_dump_format(pFormatCtx, 0, rtspUrl, 0);
    printf("-------------------------------\n");

    packet = av_packet_alloc();

    while(av_read_frame(pFormatCtx, packet) >= 0) {
        if(packet->stream_index == videoStream) {
            printf("+++ try to decode one frame. \n");
            decode(pCodecCtx, packet);
        }
    }

    return 0;
}
