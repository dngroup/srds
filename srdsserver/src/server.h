//
// Created by mlacaud on 04/03/18.
//

#ifndef SRDSSERVER_SERVER_H
#define SRDSSERVER_SERVER_H

void startServer(int port);

void* connection_handler(int csock);

void ocall_sendanswer(int csock, char * msg);
#endif //SRDSSERVER_SERVER_H
