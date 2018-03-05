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

void int_handler(int x)
{
    exit(0);
}

void startServer(int port){
    int sock = do_socket();
    int csock;
    signal(SIGINT || SIGKILL, int_handler);

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