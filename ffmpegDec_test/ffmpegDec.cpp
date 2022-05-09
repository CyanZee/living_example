#include <iostream>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
}

int main(int argc, char *argv[]) {
    printf("+++ Enter testing... \n");
    char filepath[] = "SampleVideo_1280x720_5mb.mp4";
    AVFormatContext *pFormatCtx;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec;
    AVFrame         *pFrame;
    AVPacket        *packet;

    av_register_all();
    avformat_network_init();

    pFormatCtx = avformat_alloc_context();
    if(avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0) {
        printf("Couldn't open input stream.\n");
        return -1;
    }

    if(avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        printf("Couldn't find stream information. \n");
        return -1;
    }

    av_dump_format(pFormatCtx, 0, filepath, 0);

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
    printf("codecpar->codec_id = %d \n", codecpar->codec_id);
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

    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

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

    pFrame = av_frame_alloc();
    if(!pFrame) {
        printf("Couldn't allocate video frame. \n");
        return -1;
    }
    packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    
    //Output Info -------------------
    printf("-----------  File Information ---------------\n");
    av_dump_format(pFormatCtx, 0, filepath, 0);
    printf("-------------------------------\n");

    int ret, got_picture;
    int y_size;
    FILE *fp_yuv = fopen("output.yuv", "wb+");
    while(av_read_frame(pFormatCtx, packet) >= 0) {
        if(packet->stream_index == videoStream) {
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if(ret < 0) {
                printf("Decode Error. \n");
                return -1;
            }
            if(got_picture) {
                printf("Succeed to decode 1 frame. \n");
                y_size = pCodecCtx->width * pCodecCtx->height;
                fwrite(pFrame->data[0], 1, y_size, fp_yuv);
                fwrite(pFrame->data[1], 1, y_size/4, fp_yuv);
                fwrite(pFrame->data[2], 1, y_size/4, fp_yuv);
                printf("Succeed write one frame. \n");
            }
        }
	av_free_packet(packet);
    }
    return 0;
}
