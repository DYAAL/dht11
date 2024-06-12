#include "test_poll_app.h"
#include <fcntl.h> // 需要使用 open 函数
#include <memory>
#include <stdexcept>
#include <stdio.h> // 需要使用 perror 函数
#include <string>
#include <sys/poll.h>
#include <unistd.h> // 需要使用 close 函数
int main(int argc, char* argv[])
{
    std::string word_buf;
    PollStruct poll_struct(2, -1);
    poll_struct.set_poll_info(0, 0, POLLIN);
    int fd = open("/dev/key0", O_RDWR);
    if (fd == -1) {
        perror("Failed to open /dev/null");
        return 1;
    }
    poll_struct.set_poll_info(1, fd, POLLIN);
    while(1){
        poll_struct.poll_func();
        if (poll_struct.get_revents(0) & poll_struct.get_events(0) ) {
            printf("poll 0\n");
        }
        if(poll_struct.get_revents(1) & poll_struct.get_events(1)){
            printf("poll 1\n");
            char buf[10];
            read(fd, buf, 10);
            printf("read: %s\n", buf);
        }
        // TODO
    }
    close(fd);
    return 0;
}