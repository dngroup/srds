//
// Created by mlacaud on 04/03/18.
//

#ifndef SRDSSERVER_MESSAGE_MANAGEMENT_H
#define SRDSSERVER_MESSAGE_MANAGEMENT_H

void ecall_handlemessage(int csock, char * msg);
void handleTracker(int csock, char * msg);
void handleProxy(int csock, char * msg);
char* substr(char* arr, int begin, int len);
int testEndTransferEncoding(char* msg);
#endif //SRDSSERVER_MESSAGE_MANAGEMENT_H
