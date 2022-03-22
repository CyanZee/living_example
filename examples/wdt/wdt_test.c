#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#define WDT "/dev/watchdog"
int main(void)
{
    int wdt_fd = -1;
    int timeout;
    wdt_fd = open(WDT, O_WRONLY);
    if (wdt_fd == -1)  {
        printf("fail to open "WDT "!\n");
    }
    printf(WDT " is opened!\n");
    timeout = 10;
    ioctl(wdt_fd, WDIOC_SETTIMEOUT, &timeout);
    ioctl(wdt_fd, WDIOC_GETTIMEOUT, &timeout);
    printf("The timeout was is %d seconds\n", timeout);
#if 1                                         // 循环喂狗
    while(1)  {
        write(wdt_fd, "\0", 1);
        sleep(12);    // 喂狗时间为9秒，小于设定的10秒；如果喂狗时间大于超时时间，将会发生看门狗复位
    }
#else                                          // 关闭看门狗
   write(wdt_fd, "V", 1);
   close(wdt_fd);
   printf( WDT " is closeed!\n");
#endif
    return 0;
}
