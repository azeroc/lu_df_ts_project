#include "common.h"
#include "constants.h"
#include "config_mgr.h"
#include "tcp_ts_client.h"
#include "touch_event_parser.h"

void load_config(std::string config_file)
{
    std::vector<std::string> server_keys
    {
        "port",
        "senders.count"
    };

    // Get initial server parameters
    config_mgr::instance().load(config_file, "ts_receiver", server_keys, true);
}

int main(int argc, char* argv[])
{
    std::string config_path = "";

    if (argc < 2)
    {
        config_path = "ts_sender.ini";
    }
    else
    {
        std::string arg1 = argv[1];
        if (arg1 == "help" || arg1 == "--help" || arg1 == "-h")
        {
            std::cout << "Usage: ts_sender [config file = ts_sender.ini]" << std::endl;
            std::cout << "Check ts_sender.ini for more configuration info" << std::endl;
            return 0;
        }
        else
        {
            config_path = arg1;
        }
    }

    if (!ts_util::file_exists(config_path))
    {
        std::cerr << "Config file "
            << config_path
            << " does not exist or can't be opened"
            << std::endl;
        return 1;
    }

    try
    {
        std::cout << "Reading configuration" << std::endl;
        load_config(config_path);
        std::cout << "Inititalizing TCP client instance" << std::endl;

        // Get initial config parameters
        std::string input_type_str = config_mgr::instance().get_string("mode");
        std::string input_path = config_mgr::instance().get_string("input");
        std::string server_host = config_mgr::instance().get_string("host");
        std::string port_str = config_mgr::instance().get_string("port");
        touch_event_parser::input_type input_type;

        if (input_type_str != "dev" && input_type_str != "test")
        {
            std::cerr << "Invalid input type! It must be 'dev' or 'test' (without quotes)." << std::endl;
            return 1;
        }
        else if (input_type_str == "dev")
        {
            input_type = touch_event_parser::input_type::device;
        }
        else if (input_type_str == "test")
        {
            input_type = touch_event_parser::input_type::test_file;
        }

        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(server_host, port_str);
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

        boost::shared_ptr<tcp_ts_client> client = boost::make_shared<tcp_ts_client>(io_service, iterator);
        touch_event_parser parser(io_service, input_path, input_type, client);

        io_service.run();        
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}