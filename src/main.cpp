//main function
#include "server.h"
#include <iostream>

int main() {
    try {
        Server server("192.168.1.100", 12345);
        server.Start();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
