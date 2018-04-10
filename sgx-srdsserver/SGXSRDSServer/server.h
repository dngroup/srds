//
// Created by mlacaud on 04/03/18.
//

#ifndef SRDSSERVER_SERVER_H
#define SRDSSERVER_SERVER_H

void startServer(int port, int inputtype);

void* connection_handler(int csock);

//void ocall_sendanswer(int csock, char * msg, int size);
#endif //SRDSSERVER_SERVER_H
