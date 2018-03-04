//
// Created by mlacaud on 04/03/18.
//
#ifndef SRDSSERVER_HTTP_HEADER_H
#define SRDSSERVER_HTTP_HEADER_H

#include <iostream>
#include <map>

std::map<std::string,std::string> parse_headers(char * msg);

char * getAnswerHeader();

char * postAnswerHeader();

std::string copystring(std::string string);

#endif //SRDSSERVER_HTTP_HEADER_H
