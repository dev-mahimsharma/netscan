#include "netlens.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
// Windows includes are in netlens.h
#else
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

bool netlens_init(void) {
#ifdef _WIN32
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
    return true;
#endif
}

void netlens_cleanup(void) {
#ifdef _WIN32
    WSACleanup();
#endif
}

bool parse_ip(const char *ip_str, uint32_t *ip_out) {
    struct in_addr addr;
    if (inet_pton(AF_INET, ip_str, &addr) == 1) {
        *ip_out = ntohl(addr.s_addr);
        return true;
    }
    return false;
}

void format_ip(uint32_t ip, char *buffer, size_t size) {
    struct in_addr addr;
    addr.s_addr = htonl(ip);
    inet_ntop(AF_INET, &addr, buffer, size);
}

// Checks if a TCP port is open on the given IP address
bool check_tcp_port(uint32_t ip, uint16_t port, int timeout_ms) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return false;
    }

    // Set socket to non-blocking
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
#else
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif

    struct sockaddr_in target;
    memset(&target, 0, sizeof(target));
    target.sin_family      = AF_INET;
    target.sin_port        = htons(port);
    target.sin_addr.s_addr = htonl(ip);

    int res = connect(sock, (struct sockaddr *)&target, sizeof(target));
    if (res < 0) {
#ifdef _WIN32
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
#else
        if (errno == EINPROGRESS) {
#endif
            fd_set set;
            FD_ZERO(&set);
            FD_SET(sock, &set);

            struct timeval timeout;
            timeout.tv_sec  = timeout_ms / 1000;
            timeout.tv_usec = (timeout_ms % 1000) * 1000;

            res = select(sock + 1, NULL, &set, NULL, &timeout);
            if (res > 0) {
                int       error = 0;
                socklen_t len   = sizeof(error);
                getsockopt(sock, SOL_SOCKET, SO_ERROR, (char *)&error, &len);
                if (error != 0) {
                    res = -1;
                }
            } else {
                // Timeout or error in select
                res = -1;
            }
        }
    }

#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    return (res >= 0);
}
