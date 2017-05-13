#include "common.h"
#include "constants.h"
#include "tcp_ts_client.h"

int main(int argc, char* argv[])
{
    std::srand(std::time(0));

    if (argc < 3)
    {
        std::cerr << "Usage: ts_sender <hostname/ip> <port>" << std::endl;
        return 1;
    }

    try
    {
        std::cout << "Inititalizing TCP client instance. " << std::endl;

        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(argv[1], argv[2]);
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp_ts_client client(io_service, iterator);

        io_service.run();        
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}