#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <rga/RgaApi.h>
#include "rkdrm_display.h"

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
    g_disp.fmt = DRM_FORMAT_NV12;
    g_disp.rga_fmt = RK_FORMAT_YCbCr_420_SP;
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

int main(int argc, char **argv) {

	char *buf;
	size_t file_size;
	size_t read_size;

	FILE *fp = fopen("NV12-1280x800.yuv", "rb");
	if (NULL == fp) {
		printf("Failed to open input file NV12-1280x800.yuv. \n");
		return -1;
	}
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);
	printf("input file size: %ld\n", file_size);
	if (file_size) {
		buf = malloc(file_size);
		read_size = fread(buf, 1, file_size, fp);
	
		if ((!file_size) || read_size != file_size || feof(fp)) {
			printf("fread file error.\n");
			free(buf);
			return -1;
		}
	}

    if (c_RkRgaInit()) {
        printf("%s: rga init fail! \n", __func__);
    }
    printf("to display init.\n");
	display_init(1280, 800);

    printf("to display commit.\n");
	display_commit((void*)buf, 0, RK_FORMAT_YCrCb_420_SP, 1280, 800, 0);

    getchar();
    printf("--- end ---\n");
	return 0;
}
