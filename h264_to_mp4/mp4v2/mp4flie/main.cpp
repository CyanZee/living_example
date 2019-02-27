#include <cstdio>
using namespace std;
#include "MP4Encoder.h"
#include <windows.h>

void Release(MP4Encoder &encoder, FILE *&video, FILE *&audio);

#define MAX_BUFFER_SIZE 500000
#define EXIT_WITH_FAIL_VALUE(fun, content, encoder, video, audio) \
do \
{ \
	if (!fun) \
	{ \
		Release(encoder, video, audio); \
		printf(content); \
		return 1; \
	} \
} \
while (false)

uint8_t *buf = NULL;
bool success = false;
bool Init(int &argc, const char **&argv, FILE *&video, FILE *&audio)
{
	if (argc < 3)
	{
		printf("Usage: %s arg1 arg2\n	arg1 is video file.\n	arg2 is audio file.\n", argv[0]);
		return false;
	}
	if (fopen_s(&video, argv[1], "rb") != 0)
	{
		printf("Could not open video file!\n");
		return false;
	}
	if (fopen_s(&audio, argv[2], "rb") != 0)
	{
		printf("Could not open audio file!\n");
		return false;
	}
	buf = new uint8_t[MAX_BUFFER_SIZE];
	if (buf == NULL)
	{
		printf("Could not alloc memory!\n");
		return false;
	}
	return true;
}

void Release(MP4Encoder &encoder, FILE *&video, FILE *&audio)
{
	if (video != NULL)
	{
		printf("Close video file.\n");
		fclose(video);
		video = NULL;
	}
	if (audio != NULL)
	{
		printf("Close audio file.\n");
		fclose(audio);
		audio = NULL;
	}
	encoder.MP4ReleaseFile();
	printf("Release buffer.\n");
	delete buf;
	if (success)
		return;
	printf("Remove mp4 file.\n");
	remove("test.mp4");
}

uint64_t pts = 0;
int ret = -1, len = 0;
MP4EncoderResult AddH264Track(MP4Encoder &encoder, FILE *&fp)
{
	ret = fread(&len, sizeof(int), 1, fp);
	if (ret == 1)
	{
		ret = fread(&pts, sizeof(uint64_t), 1, fp);
		if (ret == 1)
		{
			ret = fread(buf, sizeof(uint8_t), len, fp);
			if (ret == len)
			{
			    byte dta[25];
				for (int i = 0; i < 25; i++)
				{
					dta[i] = buf[i];
				}
				return encoder.MP4AddH264Track(buf, len, 640, 480);
			}
			else
				return MP4ENCODER_ERROR(MP4ENCODER_E_ADD_VIDEO_TRACK);
		}
		else
			return MP4ENCODER_ERROR(MP4ENCODER_E_ADD_VIDEO_TRACK);
	}
	else
		return MP4ENCODER_ERROR(MP4ENCODER_E_ADD_VIDEO_TRACK);
}

MP4EncoderResult AddAACTrack(MP4Encoder &encoder, FILE *&fp)
{
	ret = fread(&len, sizeof(int), 1, fp);
	if (ret == 1)
	{
		ret = fread(&pts, sizeof(uint64_t), 1, fp);
		if (ret == 1)
		{
			ret = fread(buf, sizeof(uint8_t), len, fp);
			if (ret == len)
				return encoder.MP4AddAACTrack(buf, len);
			else
				return MP4ENCODER_ERROR(MP4ENCODER_E_ADD_AUDIO_TRACK);
		}
		else
			return MP4ENCODER_ERROR(MP4ENCODER_E_ADD_AUDIO_TRACK);
	}
	else
		return MP4ENCODER_ERROR(MP4ENCODER_E_ADD_AUDIO_TRACK);
}

bool WriteH264Data(MP4Encoder &encoder, FILE *fp)
{
	bool endOfFile = false;
	MP4EncoderResult result;
	while (!endOfFile)
	{
		ret = fread(&len, sizeof(int), 1, fp);
		if (ret == 1)
		{
			ret = fread(&pts, sizeof(uint64_t), 1, fp);
			if (ret == 1)
			{
				ret = fread(buf, sizeof(uint8_t), len, fp);
				if (ret == len)

					result = encoder.MP4WriteH264Data(buf, len, pts);
				else
					result = MP4ENCODER_ERROR(MP4ENCODER_E_WRITE_VIDEO_DATA);
			}
			else
				result = MP4ENCODER_ERROR(MP4ENCODER_E_WRITE_VIDEO_DATA);
		}
		else
			endOfFile = true;
		if (result != MP4ENCODER_ENONE)
			return false;
	}
	return true;
}

bool WriteAACData(MP4Encoder &encoder, FILE *fp)
{
	bool endOfFile = false;
	MP4EncoderResult result;
	while (!endOfFile)
	{
		ret = fread(&len, sizeof(int), 1, fp);
		if (ret == 1)
		{
			ret = fread(&pts, sizeof(uint64_t), 1, fp);
			if (ret == 1)
			{
				ret = fread(buf, sizeof(uint8_t), len, fp);
				if (ret == len)
					result = encoder.MP4WriteAACData(buf, len, pts);
				else
					result = MP4ENCODER_ERROR(MP4ENCODER_E_WRITE_AUDIO_DATA);
			}
			else
				result = MP4ENCODER_ERROR(MP4ENCODER_E_WRITE_AUDIO_DATA);
		}
		else
			endOfFile = true;
		if (result != MP4ENCODER_ENONE)
			return false;
	}
	return true;
}

int main(int argc, const char *argv[])
{
	MP4Encoder encoder;
	FILE *video(NULL), *audio(NULL);
	EXIT_WITH_FAIL_VALUE(Init(argc, argv, video, audio), \
		"Initialization failed! Exit now...\n", encoder, video, audio);
	EXIT_WITH_FAIL_VALUE(!((int)encoder.MP4CreateFile("test.mp4", 5)), \
		"Create file failed! Exit now...\n", encoder, video, audio);
	EXIT_WITH_FAIL_VALUE(!((int)AddH264Track(encoder, video)), \
		"Add H264 Track failed! Exit now...\n", encoder, video, audio);
	EXIT_WITH_FAIL_VALUE(!((int)AddAACTrack(encoder, audio)), \
		"Add AAC Track failed! Exit now...\n", encoder, video, audio);
	WriteH264Data(encoder, video);
	WriteAACData(encoder, audio);
//	EXIT_WITH_FAIL_VALUE(WriteH264Data(encoder, video), \
		"Error: Write H264 Data! Exit now...\n", encoder, video, audio);
//	EXIT_WITH_FAIL_VALUE(WriteAACData(encoder, audio), \
		"Error: Write AAC Data! Exit now...\n", encoder, video, audio);
	success = true;
	Release(encoder, video, audio);
	return 0;
}