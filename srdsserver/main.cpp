#include <iostream>
#include <cstring>
#include <unistd.h>
#include "src/server.h"
#include "src/common_socket.h"

int main(int argc, char ** argv) {
    char * arg;
    int port;
    if (argc != 2) {
        port = 8080;
    } else {
        port = atoi(argv[1]);
    }

    for (int i = 1; i <= argc; i++) {
        arg = argv[i];
        std::cout << arg << std::endl;
    }

    startServer(port);

    return 0;
}