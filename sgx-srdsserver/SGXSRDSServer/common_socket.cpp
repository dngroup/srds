//
// Created by mlacaud on 04/03/18.
//
#include "common_socket.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

int do_socket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }
    return sock;
}

void do_bind(int sock, int port) {
    struct sockaddr_in sin = { 0 };

    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    sin.sin_family = AF_INET;

    sin.sin_port = htons(port);

    if(bind (sock, (struct sockaddr *) &sin, sizeof sin) != 0)
    {
        perror("bind()");
        exit(errno);
    }
}

void do_listen(int sock) {
    if(listen(sock, 100) < 0)
    {
        perror("listen()");
        exit(errno);
    }
}

int do_accept(int sock) {
    struct sockaddr_in csin = { 0 };
    int csock;

    socklen_t sinsize = sizeof csin;

    csock = accept(sock, (struct sockaddr *)&csin, &sinsize);

    if(csock < 0)
    {
        perror("Error with accept");
    }

    return csock;
}

int do_connect(int sock, struct sockaddr_in * sin) {
    return connect(sock, (struct sockaddr*)sin, sizeof(struct sockaddr));
}

int do_send(int sock, char * buffer, int size) {
    size_t n = 0;
    if((n = send(sock, buffer, size, MSG_NOSIGNAL)) <= 0)
    {
    	perror("Can not send to server");
    }

    return n;
}

int do_recv(int sock, char * buffer) {
    ssize_t n = 0;
    if((n = recv(sock, buffer, 1024, 0)) < 0)
    {
    	
        perror("Error while receiving");
    }

    return n;
}

