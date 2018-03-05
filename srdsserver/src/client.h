//
// Created by mlacaud on 04/03/18.
//

#ifndef SRDSSERVER_CLIENT_H
#define SRDSSERVER_CLIENT_H

int ocall_startClient(char * address, int port);

char * ocall_sendToClient(int sock, char * request);

#endif //SRDSSERVER_CLIENT_H
