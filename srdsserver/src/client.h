//
// Created by mlacaud on 04/03/18.
//

#ifndef SRDSSERVER_CLIENT_H
#define SRDSSERVER_CLIENT_H

int ocall_startClient(char * address); //SRDS

char * ocall_sendToClient(int sock, char * request, int size);

char * ocall_receiveFromClient(int sock);

#endif //SRDSSERVER_CLIENT_H
