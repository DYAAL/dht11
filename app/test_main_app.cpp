#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//  sr501_device0

int main(int argc, char* argv[])
{
    int fd;
    int us;
    float dist;
    char buffer[128]; // 缓冲区，用于存储从设备读取的数据

    // 检查命令行参数
    if (argc != 2) {
        fprintf(stderr, "Usage: %s /dev/sr501_device\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 打开设备节点
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }

    while (1) {
        sleep(1);
        if (read(fd, &us, 4)) {
            dist = us * 340 / 2 / 1000000;
            printf("Distance: %.2f mm\n", dist);
        } else {
            printf("Failed to read from device\n");
        }
    }

    // 关闭设备节点
    close(fd);
    printf("Device closed\n");

    return EXIT_SUCCESS;
}
