#include <iostream>
#include <cstring>
#include <unistd.h>
#include "src/server.h"
#include "src/common_socket.h"

int main(int argc, char ** argv) {
    char * arg;

    for (int i = 1; i <= argc; i++) {
        arg = argv[i];
        std::cout << arg << std::endl;
    }

    startServer(atoi(argv[1]));

    return 0;
}