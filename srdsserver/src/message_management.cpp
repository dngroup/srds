//
// Created by mlacaud on 04/03/18.
//
#include "message_management.h"
#include "http_header.h"
#include <iostream>
#include <cstring>
#include "server.h"

void ecall_handlemessage(int csock, char * msg){

    std::string beginning(msg, 0, 4);
    std::string answer = "HTTP/1.1 200 OK\r\nContent-Length: 18\r\nContent-Type: application/json\r\nConnection: Closed\r\n\r\n[\"poulet\",\"poule\"]";

    if (beginning == "GET " || beginning == "POST" || beginning == "PUT " || beginning == "DELETE") {
        std::cout << "HTTP detected" << std::endl << std::flush;
        std::map<std::string, std::string> headers = parse_headers(msg);
        char *finalanswer = new char[answer.size()+1];
        strcpy(finalanswer,answer.c_str( ));
        ocall_sendanswer(csock, finalanswer);
    }

}