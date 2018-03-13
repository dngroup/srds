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
    char * answerFromClient;
    int httpanswer;
    int testIsEnd = 0;
    int sizeAnswerFromClient = 0;
    std::map<std::string,std::string> headersAnswer;


    client_sock = ocall_startClient(target, targetPort);
    answer = createNewHeader(msg,target);
    answerFromClient = ocall_sendToClient(client_sock, answer);
    sizeAnswerFromClient = extractSize(answerFromClient);
    finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);
    free(answerFromClient);

    httpanswer = isHttp(finalanswer);
    if(httpanswer == 0) {
        headersAnswer = parse_headers(finalanswer);
        if (headersAnswer.count("Content-Length")>0) {
            //TODO Content-Length, then read data until the end and close socket
            ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
        } else if (headersAnswer.count("Transfer-Encoding")>0) {
            //TODO Transfer-Encoding: chunked then look for the 0\r\n\r\n at the end of every packet. When found, close the socket
            //TODO Other idea: add a "Connection: close" header, so the connexion will be closed by the server

            while (testEndTransferEncoding(finalanswer) != 0) {
                ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
                free(finalanswer);
                answerFromClient = ocall_receiveFromClient(client_sock);
                sizeAnswerFromClient = extractSize(answerFromClient);
                finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);
                free(answerFromClient);
            }
            ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);

        } else {
            ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
        }
    } else {
        ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
    }
}

void handleTracker(int csock, char * msg) {
    char answer[1024] = "HTTP/1.1 200 OK\r\nContent-Length: 17\r\nContent-Type: application/json\r\nConnection: Closed\r\n\r\n[\"peer1\",\"peer2\"]\0";
    //char test[1024] = "GET / HTTP/1.1\r\nHost: lacaud.fr\r\nUser-Agent: curl/7.55.1\r\nConnection: close\r\nAccept: */*\r\n\r\n";

    ocall_sendanswer(csock, answer, strlen(answer));
}

char* substr(char* arr, int begin, int len)
{
    char* res = new char[len];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}

int testEndTransferEncoding(char* msg, int size) {
    int sizeOfEndBufferMarker = 5;
    if (size < sizeOfEndBufferMarker) {
        return 1;
    }

    char* test = substr(msg,size-sizeOfEndBufferMarker, sizeOfEndBufferMarker);

    if (strcmp(test, "0\r\n\r\n")==0) {
        return 0;
    } else {
        return 1;
    }
}

int extractSize(char * msg) {
    int size = (msg[0] << 24) + (msg [1] << 16) + (msg[2] << 8) + msg[3];
    return size;
}

char * extractBuffer(char * msg, int size) {
    int sizeIntinChar = 4;
    char * buffer = (char *)malloc(sizeof(char) * size);
    buffer = (char*)memset(buffer, '\0', size);

    for(int i = 0; i< size; i++) {
        buffer[i] = msg [i + sizeIntinChar];
    }

    return buffer;

}