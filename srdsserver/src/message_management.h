//
// Created by mlacaud on 04/03/18.
//

#ifndef SRDSSERVER_MESSAGE_MANAGEMENT_H
#define SRDSSERVER_MESSAGE_MANAGEMENT_H

void ecall_handlemessage(int csock, char * msg, int size);
void handleProxy(int csock, char * msg, int msgsize);
void handleTracker(int csock, char * msg);
char* substr(char * arr, int begin, int len);
int testEndTransferEncoding(char * msg, int size);
int testContentLength(int contentLength, int totalSize);
int extractSize(char * msg);
char * extractBuffer(char * msg, int size);

#endif //SRDSSERVER_MESSAGE_MANAGEMENT_H
