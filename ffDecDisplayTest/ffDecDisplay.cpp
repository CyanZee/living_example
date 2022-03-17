#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <rga/RgaApi.h>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
    #include "rkdrm_display.h"
}

#define BUF_COUNT 3
//#define USE_NV12
#define USE_RGB888

struct display {
	int fmt;
	int width;
	int height;
	int plane_type;
	struct drm_dev dev;
	struct drm_buf buf[BUF_COUNT];
	int buf_cnt;
	int rga_fmt;
	
#if 0
	int x;
	int y;
	int w;
	int h;
#endif

	//YUV_Color color;
};

struct display g_disp;
static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

static int drm_display_init(struct display *disp)
{
    int ret;
    if (drmInit(&disp->dev)) {
        fprintf(stderr, "drmInit: Failed\n");
        return -1;
    }

    for (int i = 0; i < disp->buf_cnt; i++) {
        ret = drmGetBuffer(disp->dev.drm_fd, disp->width, disp->height, disp->fmt, &disp->buf[i]);
        if (ret) {
            fprintf(stderr, "Alloc drm buffer failed, %d\n", i);
            return -1;
        }
    }

    return 0;
}

int display_init(int width, int height)
{
    int ret;
#ifdef USE_NV12
    //g_disp.fmt = DRM_FORMAT_NV12;
    g_disp.fmt = DRM_FORMAT_YUV444;
    //g_disp.rga_fmt = RK_FORMAT_YCbCr_420_SP;
    g_disp.rga_fmt = RK_FORMAT_YCbCr_444_SP;
#endif
#ifdef USE_RGB888
    g_disp.fmt = DRM_FORMAT_BGR888;
    g_disp.rga_fmt = RK_FORMAT_BGR_888;
#endif
    g_disp.width = width;
    g_disp.height = height;
    g_disp.plane_type = DRM_PLANE_TYPE_OVERLAY;
    g_disp.buf_cnt = BUF_COUNT;
    //g_disp.color = set_yuv_color(COLOR_R);
    ret = drm_display_init(&g_disp);
    if (ret)
        return ret;

    return 0;
}

void drm_commit(struct display *disp, int num, void *ptr, int fd, int fmt, int w, int h, int rotation)
{
    int ret;
    rga_info_t src, dst;
    char *map = disp->buf[num].map;
    int dst_w = disp->width;
    int dst_h = disp->height;
    int dst_fmt = disp->rga_fmt;

    memset(&src, 0, sizeof(rga_info_t));
    src.fd = -1;
    src.virAddr = ptr;
    src.mmuFlag = 1;
    src.rotation = rotation;
    rga_set_rect(&src.rect, 0, 0, w, h, w, h, fmt);
    memset(&dst, 0, sizeof(rga_info_t));
    dst.fd = -1;
    dst.virAddr = map;
    dst.mmuFlag = 1;
    rga_set_rect(&dst.rect, 0, 0, dst_w, dst_h, dst_w, dst_h, dst_fmt);
    if (c_RkRgaBlit(&src, &dst, NULL)) {
        printf("%s: rga fail\n", __func__);
        return;
    }

    //pthread_mutex_lock(&g_lock);
    //YUV_Rect rect = {g_disp.x, g_disp.y, g_disp.w, g_disp.h};
    //YUV_Color color = g_disp.color;
    //pthread_mutex_unlock(&g_lock);
    //if (rect.x || rect.y || rect.width || rect.height)
       // yuv420_draw_rectangle(map, dst_w, dst_h, rect, color);
    ret = drmCommit(&disp->buf[num], disp->width, disp->height, 0, 0, &disp->dev, disp->plane_type);
    if (ret) {
        fprintf(stderr, "display commit error, ret = %d\n", ret);
    }
}

void display_commit(void *ptr, int fd, int fmt, int w, int h, int rotation)
{
    static int num = 0;

    drm_commit(&g_disp, num, ptr, fd, fmt, w, h, rotation);
    num = (num + 1) % BUF_COUNT;
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

    //rga init
    if (c_RkRgaInit()) {
        printf("rga init fail! \n");
        return -1;
    }
    printf("to display init.\n");
	display_init(1920, 1080);


    int ret, got_picture;
    int y_size;
    void *buf;
    buf = malloc(pCodecCtx->width * pCodecCtx->height * 1.5);
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
                memset(buf, 0x00, y_size*1.5);
                memcpy((void*)buf, pFrame->data[0], y_size);
                memcpy((void*)buf+y_size, pFrame->data[1], y_size/4);
                memcpy((void*)buf+y_size*5/4, pFrame->data[2], y_size/4);
                //fwrite(pFrame->data[0], 1, y_size, fp_yuv); //Y
                //fwrite(pFrame->data[1], 1, y_size/4, fp_yuv); //U
                //fwrite(pFrame->data[2], 1, y_size/4, fp_yuv); //V

                display_commit((void*)buf, 0, RK_FORMAT_YCrCb_420_P, 1280, 720, 0);
                printf("Succeed write one frame. \n");
            }
        }
    }

    return 0;
}