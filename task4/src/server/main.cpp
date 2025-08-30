#include <iostream>

#include "Server.h"

int main() {
    Server server{3};
    server.start();

    int input;
    std::cout << "Enter something to stop the server:" << std::endl << "> ";
    std::cin >> input;
    server.stop();
}
