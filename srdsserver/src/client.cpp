//
// Created by mlacaud on 04/03/18.
//
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include "common_socket.h"

int ocall_startClient(char * address, int port) {
    int sock = 0;
    struct sockaddr_in sin = { 0 };
    struct hostent *hostinfo;

    sock = do_socket();

    if (sock < 0) {
        return -1;
    }

    hostinfo = gethostbyname(address);
    if (hostinfo == NULL)
    {
        fprintf (stderr, "Unknown host %s.\n", address);
        return -1;
    }

    sin.sin_addr = *(struct in_addr *) hostinfo->h_addr;
    sin.sin_port = htons(port);
    sin.sin_family = AF_INET;

    if(do_connect(sock, &sin) < 0)
    {
        fprintf (stderr, "fail to connect");
        return -1;
    }

    return sock;
}

char * ocall_sendToClient(int sock, char * request) {
    char * buffer = (char*)malloc(sizeof(char) * 1024);
    buffer = (char*)memset(buffer, 0, 1024);
    do_send(sock, request);
    do_recv(sock, buffer);
    close(sock);
    return buffer;
}