//
// Created by mlacaud on 04/03/18.
//

#ifndef SRDSSERVER_CLIENT_H
#define SRDSSERVER_CLIENT_H

void ocall_startClient(int * csock, char * address);

void ocall_sendToClient(int sock, char * request, int size, char * finalbuffer);

void ocall_receiveFromClient(int sock, char * finalbuffer);

void ocall_getSocketIP(int sock, char * clientip);

void ocall_closesocket(int sock);

#endif //SRDSSERVER_CLIENT_H
