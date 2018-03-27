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

#include "SGXSRDSEnclave_u.h"

void ocall_startClient(int * csock, char * address, int port) {
    int sock = 0;
    struct sockaddr_in sin = { 0 };
    struct hostent *hostinfo;

    sock = do_socket();

    if (sock < 0) {
        return;
    }

    hostinfo = gethostbyname(address);
    if (hostinfo == NULL)
    {
        fprintf (stderr, "Unknown host %s.\n", address);
        return;
    }

    sin.sin_addr = *(struct in_addr *) hostinfo->h_addr;
    sin.sin_port = htons(port);
    sin.sin_family = AF_INET;

    if(do_connect(sock, &sin) < 0)
    {
        fprintf (stderr, "fail to connect");
        return;
    }

    *csock = sock;
}

void ocall_sendToClient(int sock, char * request, int size, char * finalbuffer) {
    int sizeAnswer = 0;
    int sizeIntinChar = 4;
    finalbuffer = (char*)malloc(sizeof(char) * (1024 + sizeIntinChar));
    finalbuffer = (char*)memset(finalbuffer, '\0', (1024 + sizeIntinChar));
    char * buffer = (char*)malloc(sizeof(char) * 1024);
    buffer = (char*)memset(buffer, '\0', 1024);
    do_send(sock, request, size);
    sizeAnswer = do_recv(sock, buffer);
    finalbuffer[0] = (sizeAnswer >> 24) & 0xFF;
    finalbuffer[1] = (sizeAnswer >> 16) & 0xFF;
    finalbuffer[2] = (sizeAnswer >> 8) & 0xFF;
    finalbuffer[3] = sizeAnswer & 0xFF;
    for (int i = 0; i < sizeAnswer; i++) {
        finalbuffer[i + sizeIntinChar] = buffer[i];
    }
}

void ocall_receiveFromClient(int sock, char * finalbuffer) {
    int size = 0;
    int sizeIntinChar = 4;
    finalbuffer = (char*)malloc(sizeof(char) * (1024 + sizeIntinChar));
    finalbuffer = (char*)memset(finalbuffer, '\0', (1024 + sizeIntinChar));
    char * buffer = (char*)malloc(sizeof(char) * 1024);
    buffer = (char*)memset(buffer, 0, 1024);
    size = do_recv(sock, buffer);
    finalbuffer[0] = (size >> 24) & 0xFF;
    finalbuffer[1] = (size >> 16) & 0xFF;
    finalbuffer[2] = (size >> 8) & 0xFF;
    finalbuffer[3] = size & 0xFF;
    for (int i = 0; i < size; i++) {
        finalbuffer[i + sizeIntinChar] = buffer[i];
    }
}

