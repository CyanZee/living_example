#include "MyFFmpeg.h"
#include "MP4Encoder.h"
#include <iostream>
int main(void)
{
// cut video & save to mp4
	#if 1 
	char infile[1024] = "test.avi";
	//char infile[1024] = "testvideo.mp4";
	//char infile[1024] = "testvideo.h264";
	char *outfile = new char[255];
	bool save_flag = false;
	int frame_idx = 0;
	int sec = 10;
	FILE *fp;

	//avcodec_register_all();
	//avformat_network_init();

	snprintf(outfile, 255, "%sshortvideo_%ds.mp4", "./", sec);
	std::cout << "+++ Save the video file: " << outfile << std::endl;
	
	MP4Encoder mp4Encoder;
	MP4FileHandle hMp4File = mp4Encoder.CreateMP4File(outfile,704,576);

	std::cout << "+++ Going to MyFFmpeg::Get()->Open " << std::endl;	
	if (MyFFmpeg::Get()->Open(infile))
	{
		printf("+++ Success to open the file: %s\n",infile);
	}
	else
	{
		printf("--- Failed to open the file:[%s], error: %s\n",infile,MyFFmpeg::Get()->GetError().c_str());
		return -1;
	}

	unsigned char *buffer = new unsigned char[1024*1024*1024];
	int total_size = 0;
	for (;;) 
	{
		AVPacket pkt = MyFFmpeg::Get()->Read();
		if (pkt.size == 0)
		{
			printf("+++ The buffer size is: %d\n",total_size);
			break;
		}	
		frame_idx++;
		if ((*(pkt.data+4)&0x1f) == 7)
		{
			save_flag = true;
		}
		if (frame_idx <= sec * 25 && save_flag)
		{
			memcpy(buffer+total_size,pkt.data,pkt.size);	
			total_size += pkt.size;
		}
		//release
		av_packet_unref(&pkt);
		av_init_packet(&pkt);
	}
	
	int pos = 0;	
	int writelen = 0;
	for(int i=total_size-1;i>=0;i--)
	{
		if(buffer[i--]==0x01&&buffer[i--]==0x00&&buffer[i--]==0x00&&buffer[i--]==0x00)
		{	
			writelen = i+5;
			break;
		}
	}
	printf("+++ writelen: %d \n", writelen);
	writelen = mp4Encoder.WriteH264Data(hMp4File,buffer,writelen);			
	printf("+++ writelen end: %d \n", writelen);
	
	delete[] buffer;
	mp4Encoder.CloseMP4File(hMp4File);

	delete[] outfile;
	printf("+++ The infile's total frame is: frame_idx = %d\n", frame_idx);	
	
#endif

#if 0 
//tester save to .h264
	bool save_flag = false;
	char infile[1024] = "test.avi";
	//char infile[1024] = "tester.h264";
	//char infile[1024] = "testvideo.mp4";
	int frame_idx = 0;
	int sec = 10;
	FILE *fp;
	char *outfile = new char[255];
	snprintf(outfile, 255, "%sshortvideo_%ds.h264", "./", sec);

	//avcodec_register_all();
	//avformat_network_init();

	if (MyFFmpeg::Get()->Open(infile))
	{
		printf("+++ Success to open the file: %s\n",infile);
	}
	else
	{
		printf("--- Failed to open the file:[%s], error: %s\n",infile,MyFFmpeg::Get()->GetError().c_str());
		return -1;
	}

	if ((fp = fopen(outfile, "wb+")) == NULL) 
	{	
		printf("--- Open file failed!\n");
		return -1;
	}

	for (;;) 
	{
		AVPacket pkt = MyFFmpeg::Get()->Read();
		if (pkt.size == 0)
		{
			break;
		}	
		frame_idx++;
		if ((*(pkt.data+4)&0x1f) == 7)
		{
			save_flag = true;
		}
		if (frame_idx <= sec * 25 && save_flag)
		{
			fwrite(pkt.data, 1, pkt.size, fp);
		}

		//release
		av_packet_unref(&pkt);
		av_init_packet(&pkt);
	}
	
	fclose(fp);
	delete[] outfile;
	printf("+++ total frame is: frame_idx = %d\n", frame_idx);	
#endif
	return 0;
}

