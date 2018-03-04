//
// Created by mlacaud on 04/03/18.
//
#include "message_management.h"
#include "http_header.h"
#include <iostream>
#include "server.h"

void ecall_handlemessage(int csock, char * msg){

    std::string beginning(msg, 0, 4);
    int pos = 0;
    int endPos = 0;

    if (beginning == "GET " || beginning == "POST" || beginning == "PUT " || beginning == "DELETE") {
        std::cout << "HTTP detected" << std::endl << std::flush;
        std::map<std::string, std::string> headers = parse_headers(msg);

    }

    ocall_sendanswer(csock, "HTTP/1.1 200 OK\r\nContent-Length: 17\r\nContent-Type: application/json\r\nConnection: Closed\r\n\r\n{\"name\":\"poulet\"}");


}