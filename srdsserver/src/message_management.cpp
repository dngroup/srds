//
// Created by mlacaud on 04/03/18.
//
#include "message_management.h"
#include "http_header.h"
#include <iostream>
#include <cstring>
#include "server.h"
#include "client.h"

void ecall_handlemessage(int csock, char * msg, int size){ //SRDS

    int http = isHttp(msg);

    if (http == 0) {
        handleProxy(csock, msg, size);
    } else {

        int option = isOption(msg);
        if (option == 0) {
            handleOption(csock);
        }
    }

}

void handleProxy(int csock, char * msg, int msgsize) {
    int targetPort = 8023;

    char * answer;
    int client_sock = 0;
    char * finalanswer;
    char * answerFromClient;
    int httpanswer;
    int testIsEnd = 0;
    int sizeAnswerFromClient = 0;
    int totalSizeAnswer = 0;
    std::map<std::string,std::string> headersAnswer;
    //SRDS
    std::map<std::string,std::string> headersRequest;
    headersRequest = parse_headers(msg);
    char * target = (char *)headersRequest.at("X-Forwarded-Host").c_str();
    client_sock = ocall_startClient(target);
    //SRDS END
    answer = createNewHeader(msg, target, msgsize);
    answerFromClient = ocall_sendToClient(client_sock, answer, (int)strlen(answer));
    sizeAnswerFromClient = extractSize(answerFromClient);
    finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);
    free(answerFromClient);

    httpanswer = isHttp(finalanswer);
    if(httpanswer == 0) {
        headersAnswer = parse_headers(finalanswer);
        if (headersAnswer.count("Content-Length")>0 || headersAnswer.count("content-length")>0) { //SRDS
            //SRDS
            std::string contentLength;
            if (headersAnswer.count("Content-Length")>0) {
                contentLength = "Content-Length";
            } else {
                contentLength = "content-length";
            }
            //SRDS END
            //TODO Content-Length, then read data until the end and close socket
            totalSizeAnswer += sizeAnswerFromClient - std::stoi(headersAnswer.at("HeaderSize"));

            while (testContentLength(std::stoi(headersAnswer.at(contentLength)), totalSizeAnswer) != 0) { //SRDS
                ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
                free(finalanswer);
                answerFromClient = ocall_receiveFromClient(client_sock);
                sizeAnswerFromClient = extractSize(answerFromClient);
                finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);
                free(answerFromClient);
                totalSizeAnswer += sizeAnswerFromClient;
            }
            ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
            free(finalanswer);
        } else if (headersAnswer.count("Transfer-Encoding")>0) {
            //TODO Transfer-Encoding: chunked then look for the 0\r\n\r\n at the end of every packet. When found, close the socket
            //TODO Other idea: add a "Connection: close" header, so the connexion will be closed by the server
            while (testEndTransferEncoding(finalanswer, sizeAnswerFromClient) != 0 && sizeAnswerFromClient != 0) {
                ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
                free(finalanswer);
                answerFromClient = ocall_receiveFromClient(client_sock);
                sizeAnswerFromClient = extractSize(answerFromClient);
                finalanswer = extractBuffer(answerFromClient, sizeAnswerFromClient);
                free(answerFromClient);
            }
            ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
            free(finalanswer);


        } else {
            ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
            free(finalanswer);
        }
    } else {
        ocall_sendanswer(csock, finalanswer, sizeAnswerFromClient);
        free(finalanswer);
    }
}

void handleTracker(int csock, char * msg) {
    char answer[1024] = "HTTP/1.1 200 OK\r\nContent-Length: 17\r\nContent-Type: application/json\r\nConnection: Closed\r\n\r\n[\"peer1\",\"peer2\"]\0";
    //char test[1024] = "GET / HTTP/1.1\r\nHost: lacaud.fr\r\nUser-Agent: curl/7.55.1\r\nConnection: close\r\nAccept: */*\r\n\r\n";

    ocall_sendanswer(csock, answer, strlen(answer));
}

//SRDS
void handleOption(int csock) {
    char answer[1024] = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, OPTIONS\r\nAccess-Control-Allow-Headers: Origin, Content-Type, Accept, x-forwarded-host\r\nConnection: Closed\r\n\r\n\0";
    //char test[1024] = "GET / HTTP/1.1\r\nHost: lacaud.fr\r\nUser-Agent: curl/7.55.1\r\nConnection: close\r\nAccept: */*\r\n\r\n";

    ocall_sendanswer(csock, answer, strlen(answer));
}
//SRDS END

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

int testContentLength(int contentLength, int totalSize) {
    if (totalSize >= contentLength) {
        return 0;
    } else {
        return 1;
    }
}

int extractSize(char * msg) {
    int size = ((unsigned char)msg[0] << 24) + ((unsigned char)msg [1] << 16) + ((unsigned char)msg[2] << 8) + (unsigned char)msg[3];
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