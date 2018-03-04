//
// Created by mlacaud on 04/03/18.
//
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "common_socket.h"
#include "server.h"
#include "message_management.h"
#include <iostream>

void startServer(int port){
    int sock = do_socket();
    int csock;

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

        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &csock) < 0)
        {
            perror("could not create thread");
            close(csock);
        }

    }
}


void * connection_handler(void *csock)
{
    int sock = *(int*)csock;
    int read_size;
    char client_message[1024] = "";


    //Receive a message from client
    while( (read_size = do_recv(sock , client_message)) > 0 )
    {

        //Send the message back to client
        //do_send(sock , client_message);
        ecall_handlemessage(sock, client_message);

        //clear the message buffer
        memset(client_message, 0, 1024);
    }

    if(read_size == 0)
    {
        printf("Client disconnected");
        fflush(stdout);
        close(sock);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
        close(sock);
    }

    return 0;
}

void ocall_sendanswer(int csock, char * msg) {
    do_send(csock , msg);
}