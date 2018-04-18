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
#include <iostream>
#include <list>
#include <sys/socket.h>
#include <mutex>
#include <semaphore.h>

#include "SGXSRDSEnclave_u.h"

extern sgx_enclave_id_t global_eid;
sem_t mutex;
pthread_mutex_t lock;

void int_handler(int x)
{
    exit(0);
}

int type = 1;

void startServer(int port, int inputtype){
    
    int sock = do_socket();
    int csock;
    std::list<int> client_sockets;
    int enable = 1;
    pthread_mutex_init(&lock, NULL);
    sem_init(&mutex, 0, 4);
    type = inputtype;

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

    ecall_init(global_eid, type);

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
        pthread_mutex_lock(&lock);
        sem_wait(&mutex);
        pthread_mutex_unlock(&lock);
        ecall_handlemessage(global_eid, sock, type, client_message, read_size);
        pthread_mutex_lock(&lock);
        sem_post(&mutex);
        pthread_mutex_unlock(&lock);

        printf("out of ecall\n");
        fflush(stdout);

        //clear the message buffer
        memset(client_message, 0, 1024);
        printf("before close\n");
        fflush(stdout);
        close(sock);
        break;
    }
    printf("out of while\n");
    fflush(stdout);
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

int ocall_sendanswer(int csock, char * msg, int size) {
    return do_send(csock , msg, size);
}
