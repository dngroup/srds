//
// Created by mlacaud on 04/03/18.
//

#ifndef SRDSSERVER_CLIENT_H
#define SRDSSERVER_CLIENT_H

void ocall_startClient(int * csock, char * address, int port);

void ocall_sendToClient(int sock, char * request, int size, char * finalbuffer);

void ocall_receiveFromClient(int sock, char * finalbuffer);

#endif //SRDSSERVER_CLIENT_H
