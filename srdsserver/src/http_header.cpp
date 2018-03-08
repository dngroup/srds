//
// Created by mlacaud on 04/03/18.
//
#include <iostream>
#include <map>
#include <cstring>
#include "http_header.h"


int isHttp(char* msg) {
    std::string beginning(msg, 0, 4);
    if (beginning == "GET " || beginning == "POST" || beginning == "PUT " || beginning == "DELE" || beginning == "HTTP") {
        return 0;
    } else {
        return 1;
    }
}

int getPosEndOfHeader(char * msg) {
    std::string allmsg(msg);
    return allmsg.find("\r\n\r\n");
}


std::map<std::string, std::string> parse_headers(char * msg) {
    int endPos = 0;
    int pos = 0;
    int oldPos = 0;
    std::string allmsg(msg);
    int posmiddle = 0;
    int firstSpace = 0;
    int secondSpace = 0;
    std::map<std::string, std::string> headers;

    endPos = allmsg.find("\r\n\r\n");

    pos = allmsg.find("\r\n");
    firstSpace = allmsg.find(" ");
    std::string sMethod1("Method");
    std::string sMethod2(msg, oldPos, firstSpace);
    headers[copystring(sMethod1)] = copystring(sMethod2);
    secondSpace = allmsg.find(" ", firstSpace + 1);
    std::string sPath1("Path");
    std::string sPath2(msg, firstSpace + 1, secondSpace - firstSpace - 1);
    headers[copystring(sPath1)] = copystring(sPath2);

    std::string sProto1("Protocol");
    std::string sProto2(msg, secondSpace + 1, pos - secondSpace - 1);
    headers[copystring(sProto1)] = copystring(sProto2);

    while (pos < endPos && pos > 0) {
        oldPos = pos;
        pos = allmsg.find("\r\n", pos + 1);
        posmiddle = allmsg.find(":", oldPos + 1);
        std::string s1(msg, oldPos + 2, posmiddle - oldPos - 2);
        std::string s2(msg, posmiddle + 2, pos - posmiddle - 2);
        headers[copystring(s1)] = copystring(s2);
    }

    return headers;
}

std::string copystring(std::string string) {
    char *y = new char[string.length() + 1];
    std::strcpy(y, string.c_str());
    std::string string2(y);
    return string2;
}

char* createNewHeader(std::string header, std::string address) {
    int posHost = header.find("Host: ") + 6;
    int posEnd = header.find("\r\n", posHost);

    header.replace(posHost, posEnd-posHost, address);

    char *y = new char[header.length() + 1]; // or
    std::strcpy(y, header.c_str());
    return y;
}