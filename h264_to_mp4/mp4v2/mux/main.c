#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "mp4v2/mp4v2.h"
#pragma comment(lib, "../lib/libmp4v2.lib")

#define MIN_FRAME_SIZE 32
#define MAX_BUFFER_SIZE 500000
#define VIDEO_TIME_SCALE 90000
#define AUDIO_TIME_SCALE 8000

unsigned char *buffer = NULL;
int ret = -1, len = 0, cnt = 0;
uint64_t audioPTS = 0, videoPTS = 0;
MP4FileHandle file = MP4_INVALID_FILE_HANDLE;
bool videoEnd = FALSE, audioEnd = FALSE, videoFirst = TRUE, audioFirst = TRUE;

bool WriteMP4Content(FILE *fp, MP4TrackId content, bool* finish, bool type)
{
	bool exit = FALSE;
	uint64_t pts;
	ret = fread(&len, sizeof(unsigned), 1, fp);
	if (ret < 1)
	{
		*finish = TRUE;
		return FALSE;
	}
	if (len <= MIN_FRAME_SIZE)
		exit = TRUE;
	if (ret == 1)
	{
		ret = fread(&pts, sizeof(uint64_t), 1, fp);
		if (!exit)
		{
			if (type && videoFirst)
			{
				videoFirst = FALSE;
				videoPTS = pts;
			}
			else if (!type && audioFirst)
			{
				audioFirst = FALSE;
				audioPTS = pts;
			}
		}
		ret = fread(buffer, sizeof(unsigned char), len, fp);
		if (exit)
			return FALSE;
		if (type == TRUE)
		{
			buffer[0] = (len - 4) >> 24;
			buffer[1] = (len - 4) >> 16;
			buffer[2] = (len - 4) >> 8;
			buffer[3] = (len - 4) & 0xFF;
			cnt++;
			if (ret == len)
			{
				if ((buffer[4] & 0x0F) == 5)
					MP4WriteSample(file, content, buffer, len, /*MP4_INVALID_DURATION*/(pts - videoPTS) * 1.0 / (double)(1e+6) * VIDEO_TIME_SCALE, 0, 1);
				else
					MP4WriteSample(file, content, buffer, len, /*MP4_INVALID_DURATION*/(pts - videoPTS) * 1.0 / (double)(1e+6) * VIDEO_TIME_SCALE, 0, 0);
				videoPTS = pts;
				return TRUE;
			}
			else
				*finish = TRUE;
		}
		else
		{
			if (ret == len)
			{
				MP4WriteSample(file, content, buffer, len, /*MP4_INVALID_DURATION*/(pts - audioPTS) * 1.0 / (double)(1e+6) * AUDIO_TIME_SCALE, 0, 1);
				audioPTS = pts;
			}
			else
				*finish = TRUE;
		}
	}
	else
		*finish = TRUE;
	
	return FALSE;
}

int main(int argc, char *argv[])
{
	bool retValue = FALSE;
	FILE *fpAudio = NULL, *fpVideo = NULL;
	MP4TrackId video = MP4_INVALID_TRACK_ID, audio = MP4_INVALID_TRACK_ID;
	uint8_t buf1[13] = {0x67, 0x42, 0x80, 0x20, 0xE4, 0x40, 0x50, 0x1E, 0xD0, 0x0D, 0xA1, 0x42, 0x6A}, buf2[4] = {0x68, 0xCE, 0x38, 0x80}, buf3[2] = {0x15, 0x88};
	//创建 mp4 文件
	if (argc < 3)
	{
		printf("Usage: %s arg1 arg2\n	arg1 is video file.\n	arg2 is audio file.\n", argv[0]);
		return 1;
	}
	file = MP4Create("test.mp4", 0);
	if (file == MP4_INVALID_FILE_HANDLE)
	{
		printf("Open file failed!\n");
		return 1;
	}
	// 设置时间片

	if (MP4SetTimeScale(file, VIDEO_TIME_SCALE) == FALSE)
		printf("MP4SetTimeScale failed!\n");
	// 添加 h264 track
	video = MP4AddH264VideoTrack(file, VIDEO_TIME_SCALE, VIDEO_TIME_SCALE / 25,
		1280, 720,
		buf1[1], // sps[1]
		buf1[2], // sps[2]
		buf1[3], // sps[3]
		3);
	if (video == MP4_INVALID_TRACK_ID)
	{
		printf("Add video track failed!\n");
		return 1;
	}
	// 设置视频属性
	MP4SetVideoProfileLevel(file, 1);
	MP4AddH264SequenceParameterSet(file, video, buf1, 13);
	MP4AddH264PictureParameterSet(file, video, buf2, 4);

	// 添加 AAC track
	audio = MP4AddAudioTrack(file, AUDIO_TIME_SCALE, AUDIO_TIME_SCALE / 8, \
		MP4_MPEG4_AUDIO_TYPE);
	if (audio == MP4_INVALID_TRACK_ID)
	{
		printf("Add audio track failed!\n");
		return 1;
	}
	// 设置音频属性
	MP4SetAudioProfileLevel(file, 0x02);
	MP4SetTrackESConfiguration(file, audio, buf3, 2);
	fpVideo = fopen(argv[1], "rb");
	fpAudio = fopen(argv[2], "rb");
	if (fpVideo == NULL || fpAudio == NULL)
	{
		printf("File could not open!\n");
		return 1;
	}
	buffer = (unsigned char *)malloc(MAX_BUFFER_SIZE);
	while (!videoEnd || !audioEnd)
	{
		retValue = WriteMP4Content(fpVideo, video, &videoEnd, TRUE);
		retValue = WriteMP4Content(fpAudio, audio, &audioEnd, FALSE);
	}
	fclose(fpVideo);
	fclose(fpAudio);
	free(buffer);
	MP4Close(file, 0);
}
