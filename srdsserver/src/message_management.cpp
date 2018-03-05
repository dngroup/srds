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

    std::string beginning(msg, 0, 4);
    char * answer;
    //char answer[1024] = "HTTP/1.1 200 OK\r\nContent-Length: 18\r\nContent-Type: application/json\r\nConnection: Closed\r\n\r\n[\"poulet\",\"poule\"]";
    //char test[1024] = "GET / HTTP/1.1\r\nHost: lacaud.fr\r\nUser-Agent: curl/7.55.1\r\nConnection: close\r\nAccept: */*\r\n\r\n";
    char target[1024] = "simondasilva.fr";
    int client_sock = 0;
    char * finalanswer;

    if (beginning == "GET " || beginning == "POST" || beginning == "PUT " || beginning == "DELETE") {
        std::cout << "HTTP detected" << std::endl << std::flush;
        std::map<std::string, std::string> headers = parse_headers(msg);

        client_sock = ocall_startClient(target, 80);
        answer = createNewHeader(msg,target);
        finalanswer = ocall_sendToClient(client_sock, answer);

        ocall_sendanswer(csock, finalanswer);
    }

}