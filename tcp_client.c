#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8888
#define BUF_LEN 256

int main(void)
{
    int sock_fd;
    struct sockaddr_in addr;
    char buf[BUF_LEN];
    int cnt = 0;

    // 1. 创建 socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    // 本地回环地址
    addr.sin_addr.s_addr = htonl(0x7F000001);

    // 2. 连接服务端
    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock_fd);
        return 1;
    }
    printf("Connected to server\n");

    // 循环一直发送消息
    while (1) {
        snprintf(buf, BUF_LEN, "Hello TCP, count: %d", cnt++);
        send(sock_fd, buf, strlen(buf), 0);
        sleep(5);
    }

    close(sock_fd);
    return 0;
}