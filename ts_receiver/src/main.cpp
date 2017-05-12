#include <iostream>
#include "touch_control.h"
#include "tcp_ts_server.h"
#include "Constants.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Port not specified" << std::endl;
        return 1;
    }

    try
    {
        int port = std::stoi(argv[1]);
        boost::asio::io_service io_service;
        tcp_ts_server server(io_service, (unsigned short)port);
        
        std::cout << "TCP server initialized, beginning to listen..." << std::endl;
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}