//
// Created by mlacaud on 04/03/18.
//
#include <cstdio>
#include <cstring>
#include <csignal>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include "common_socket.h"
#include "server.h"
#include "message_management.h"
#include <iostream>
#include <list>
#include <sys/socket.h>

void int_handler(int x)
{
    exit(0);
}

void startServer(int port){
    int sock = do_socket();
    int csock;
    std::list<int> client_sockets;
    int enable = 1;

    signal(SIGINT || SIGKILL, int_handler);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
    }

    if (sock < 0) {
        perror("socket");
        exit(errno);
    }

    do_bind(sock, port);

    do_listen(sock);

    pthread_t thread_id;


    while( true )
    {
        csock = do_accept(sock);
        client_sockets.push_back(csock);
        std::thread t( connection_handler, csock);
        t.detach();
    }
}


void * connection_handler(int csock)
{
    int sock = csock;
    int read_size;
    char client_message[1024] = "";

    //Receive a message from client
    while( (read_size = do_recv(sock , client_message)) > 0 )
    {

        ecall_handlemessage(sock, client_message);

        //clear the message buffer
        memset(client_message, 0, 1024);
    }

    if(read_size == 0)
    {
        printf("Client disconnected\n");
        fflush(stdout);
        close(sock);
    }
    else if(read_size == -1)
    {
        perror("recv failed\n");
        close(sock);
    }

    return 0;
}

void ocall_sendanswer(int csock, char * msg) {
    do_send(csock , msg);
}