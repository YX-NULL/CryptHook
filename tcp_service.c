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
    int server_fd, new_fd;
    ssize_t n;
    char buf[BUF_LEN];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    // 1. 创建 socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 2. 绑定端口
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    // 3. 开始监听
    listen(server_fd, 1);
    printf("Server listening on port %d...\n", PORT);

    // 4. 等待客户端连接
    new_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    if (new_fd < 0) {
        perror("accept");
        close(server_fd);
        return 1;
    }
    printf("Client connected\n");

    // 循环一直接收消息
    while (1) {
        memset(buf, 0, BUF_LEN);
        n = recv(new_fd, buf, BUF_LEN - 1, 0);
        if (n <= 0) {
            printf("Client disconnected\n");
            break;
        }
        printf("Recv: %s\n", buf);
    }

    close(new_fd);
    close(server_fd);
    return 0;
}