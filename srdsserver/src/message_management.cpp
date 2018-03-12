//
// Created by mlacaud on 04/03/18.
//
#include "message_management.h"
#include "http_header.h"
#include <iostream>
#include <cstring>
#include "server.h"
#include "client.h"

void ecall_handlemessage(int csock, char * msg){

    int http = isHttp(msg);

    if (http == 0) {
        handleProxy(csock, msg);
    }

}

void handleProxy(int csock, char * msg) {
    char target[1024] = "msstream.net";
    int targetPort = 8023;

    char * answer;
    int client_sock = 0;
    char * finalanswer;
    int httpanswer;
    int testIsEnd = 0;
    std::map<std::string,std::string> headersAnswer;


    client_sock = ocall_startClient(target, targetPort);
    answer = createNewHeader(msg,target);
    finalanswer = ocall_sendToClient(client_sock, answer);
    httpanswer = isHttp(finalanswer);
    if(httpanswer == 0) {
        headersAnswer = parse_headers(finalanswer);
        if (headersAnswer.count("Content-Length")>0) {
            //TODO Content-Length, then read data until the end and close socket
            ocall_sendanswer(csock, finalanswer);
        } else if (headersAnswer.count("Transfer-Encoding")>0) {
            //TODO Transfer-Encoding: chunked then look for the 0\r\n\r\n at the end of every packet. When found, close the socket
            //TODO Other idea: add a "Connection: close" header, so the connexion will be closed by the server

            while (testEndTransferEncoding(finalanswer) != 0) {
                ocall_sendanswer(csock, finalanswer);
                free(finalanswer);
                finalanswer = ocall_receiveFromClient(client_sock);
            }
            ocall_sendanswer(csock, finalanswer);

        } else {
            ocall_sendanswer(csock, finalanswer);
        }
    } else {
        ocall_sendanswer(csock, finalanswer);
    }
}

void handleTracker(int csock, char * msg) {
    char answer[1024] = "HTTP/1.1 200 OK\r\nContent-Length: 17\r\nContent-Type: application/json\r\nConnection: Closed\r\n\r\n[\"peer1\",\"peer2\"]";
    //char test[1024] = "GET / HTTP/1.1\r\nHost: lacaud.fr\r\nUser-Agent: curl/7.55.1\r\nConnection: close\r\nAccept: */*\r\n\r\n";

    ocall_sendanswer(csock, answer);
}

char* substr(char* arr, int begin, int len)
{
    char* res = new char[len];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}

int testEndTransferEncoding(char* msg) {
    if (strlen(msg)<5) {
        return 1;
    }

    char* test = substr(msg,strlen(msg)-5, 5);

    if (strcmp(test, "0\r\n\r\n")==0) {
        return 0;
    } else {
        return 1;
    }


}