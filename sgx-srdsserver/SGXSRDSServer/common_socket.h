//
// Created by mlacaud on 04/03/18.
//

#ifndef SRDSSERVER_COMMON_SOCKET_H
#define SRDSSERVER_COMMON_SOCKET_H

int do_socket();

void do_bind(int sock, int port);

void do_listen(int sock);

int do_accept(int sock);

int do_connect(int sock, struct sockaddr_in * sin);

int do_recv(int sock, char * buffer);

int do_send(int sock, char * buffer, int size);



#endif //SRDSSERVER_COMMON_SOCKET_H
