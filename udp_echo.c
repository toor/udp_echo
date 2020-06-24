#include <stdio.h>
#include <netinet/in.h>
#include <stddef.h>
#include <sys/socket.h>
#include <unistd.h>

char buf[1024];

int main(void) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in in_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(1234),
        .sin_addr = {htonl(0x123456)},
    };

    if (bind(fd, (struct sockaddr *)&in_addr, sizeof(in_addr)) < 0) {
        perror("bind");
        return 1;
    }

    for (;;) {
        socklen_t addrlen = sizeof(in_addr);

        ssize_t len = recvfrom(fd, buf, sizeof(buf), 0,
                (struct sockaddr *)&in_addr, &addrlen);

        if (len < 0) {
            perror("read");
            return 1;
        }

        addrlen = sizeof(in_addr);

        if ((len = sendto(fd, buf, len, 0, (struct sockaddr *)&in_addr, addrlen)) < 0) {
            perror("write");
            return 1;
        }
    }
}
