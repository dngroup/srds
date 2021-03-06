//
// Created by mlacaud on 04/03/18.
//
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include "common_socket.h"
#include <arpa/inet.h>

#include "SGXSRDSEnclave_u.h"

void ocall_startClient(int * csock, char * address) {
    int sock = 0;
    struct sockaddr_in sin = { 0 };
    struct hostent *hostinfo;
    std::string addr(address);
    if (addr.find(":") < 0) {
    	return;
    }
    std::string stringaddress = addr.substr(0, addr.find(":"));
    int port = std::stoi(addr.substr(addr.find(":") + 1));

    sock = do_socket();

    if (sock < 0) {
    	perror("Socket creation failed");
        return;
    }

    hostinfo = gethostbyname(stringaddress.c_str());
    if (hostinfo == NULL) {
        perror("Unknown host");
        return;
    }

    sin.sin_addr = *(struct in_addr *) hostinfo->h_addr;
    sin.sin_port = htons(port);
    sin.sin_family = AF_INET;

    if(do_connect(sock, &sin) < 0) {
        perror("Failed to connect");
        printf("Address: %s\n", address);
        fflush(stdout);
        return;
    }

    *csock = sock;
    //printf("Start client: *csock=%i\n", *csock);
}

void ocall_sendToClient(int sock2, char * request, int size2, char * finalbuffer2) {
    int sizeAnswer = 0;
    int sizeIntinChar = 4;
    int sock = sock2;
    int size = size2;
    char * finalbuffer = (char *) malloc(1024 + sizeIntinChar);
    memset(finalbuffer, '\0', (1024 + sizeIntinChar));
    char buffer[1024];
    memset(buffer, '\0', 1024);
    do_send(sock, request, size);
    sizeAnswer = do_recv(sock, buffer);
    finalbuffer[0] = (sizeAnswer >> 24) & 0xFF;
    finalbuffer[1] = (sizeAnswer >> 16) & 0xFF;
    finalbuffer[2] = (sizeAnswer >> 8) & 0xFF;
    finalbuffer[3] = sizeAnswer & 0xFF;
    for (int i = 0; i < sizeAnswer; i++) {
        finalbuffer[i + sizeIntinChar] = buffer[i];
    }

    /*printf("Request: size=%i \n%s\n", size2, request);
    fflush(stdout);
    printf("Receive: size=%i socket=%i\n%s\n", sizeAnswer, sock, buffer);
    fflush(stdout);
    printf("Finalbuffer: \n%s\n", buffer);
    fflush(stdout);*/

    memcpy(finalbuffer2,finalbuffer,sizeAnswer+sizeIntinChar);
}

void ocall_receiveFromClient(int sock, char * finalbuffer2) {
    int size = 0;
    int sizeIntinChar = 4;
    char * finalbuffer = (char*)malloc(sizeof(char) * (1024 + sizeIntinChar));
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
    memcpy(finalbuffer2,finalbuffer,size+sizeIntinChar);
}

void ocall_getSocketIP(int sock, char * clientip) {
	struct sockaddr_in addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	getpeername(sock, (struct sockaddr *)&addr, &addr_size);
	strcpy(clientip, inet_ntoa(addr.sin_addr));
	/*
	strcat(clientip, ":");
	sprintf(portc, "%d", port);
	strcat(clientip, portc);
	*/
}

void ocall_closesocket(int sock) {
    close(sock);
}
