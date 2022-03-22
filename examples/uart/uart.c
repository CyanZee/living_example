#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <asm/termios.h>
#define DEV_NAME  "/dev/ttyS0"
int main (int argc, char *argv[])
{
	printf("enter main\n");
	time_t lt;
	struct tm *ptr;
	//printf(asctime(ptr));

    int fd;
    int len, i,ret;
    char buf[] = "hello ZLG!";
    fd = open(DEV_NAME, O_RDWR | O_NOCTTY);
    if(fd < 0) {
				printf("open error\n");
				perror(DEV_NAME);
				return -1;
		}
		printf("open success.\n");

		while(fd) {
			len = write(fd, buf, sizeof(buf));                /* 向串口写入字符串       */
			if (len < 0) {
                  printf("write data error, ");
			}else {
				printf("write hello ZLG, ");
			}
				lt = time(NULL);
				ptr = gmtime(&lt);
				printf(ctime(&lt));
				usleep(1000*100);
		}
#if 0
    len = read(fd, buf, sizeof(buf));                 /* 在串口读入字符串       */
    if (len < 0) {
                  printf("read error \n");
                  return -1;
        }
    printf("%s", buf);                                /* 打印从串口读出的字符串  */
#endif
    return(0);
}
