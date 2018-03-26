//
// Created by mlacaud on 04/03/18.
//
#ifndef SRDSSERVER_HTTP_HEADER_H
#define SRDSSERVER_HTTP_HEADER_H

#include <iostream>
#include <map>

int isHttp(char* msg);

int getPosEndOfHeader(char * msg);

std::map<std::string,std::string> parse_headers(char * msg);

std::string getAnswerHeader();

std::string postAnswerHeader();

std::string copystring(std::string string);

char* createNewHeader(char * msg, std::string address, int msgsize);

#endif //SRDSSERVER_HTTP_HEADER_H
