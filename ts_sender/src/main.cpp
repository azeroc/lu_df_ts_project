#include "common.h"
#include "constants.h"
#include "tcp_ts_client.h"
#include "touch_event_parser.h"

int main(int argc, char* argv[])
{
    std::srand(std::time(0));

    if (argc < 5)
    {
        std::cerr << "Usage: ts_sender <input type (dev/test)> <input file> <hostname/ip> <port>" << std::endl;
        return 1;
    }

    // Input type argument parsing
    std::string arg1 = argv[1];
    touch_event_parser::input_type input_type;

    if (arg1 != "dev" && arg1 != "test")
    {
        std::cerr << "Invalid input type! It must be 'dev' or 'test' (without quotes)." << std::endl;
        return 1;
    }
    else if (arg1 == "dev")
    {
        input_type = touch_event_parser::input_type::device;
    }
    else if (arg1 == "test")
    {
        input_type = touch_event_parser::input_type::test_file;
    }

    try
    {
        std::cout << "Inititalizing TCP client instance. " << std::endl;

        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(argv[3], argv[4]);
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

        boost::shared_ptr<tcp_ts_client> client = boost::make_shared<tcp_ts_client>(io_service, iterator);
        touch_event_parser parser(io_service, argv[2], input_type, client);

        io_service.run();        
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}