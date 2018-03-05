//
// Created by mlacaud on 04/03/18.
//
#ifndef SRDSSERVER_HTTP_HEADER_H
#define SRDSSERVER_HTTP_HEADER_H

#include <iostream>
#include <map>
int getPosEndOfHeader(char * msg);

std::map<std::string,std::string> parse_headers(char * msg);

std::string getAnswerHeader();

std::string postAnswerHeader();

std::string copystring(std::string string);

char* createNewHeader(std::string header, std::string address);

#endif //SRDSSERVER_HTTP_HEADER_H
