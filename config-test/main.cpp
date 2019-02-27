#include <zlog.h>
#include <iostream>

#include "config.h"

int main() {
    int rc = dzlog_init("zlog.conf", "my_cat");
    if (rc) {
        printf("zlog init fail\n");
        return -1;
    }

    dzlog_debug("config testing");

    Config fig;
    fig.load();
    return 0;
}
