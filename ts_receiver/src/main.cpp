#include <iostream>
#include "common.h"
#include "config_mgr.h"
#include "touch_control.h"
#include "tcp_ts_server.h"
#include "constants.h"

void load_config(std::string config_file)
{
    std::vector<std::string> server_keys
    {
        "port",
        "senders.count",
        "host_x_res",
        "host_y_res"
    };

    // Get initial server parameters
    config_mgr::instance().load(config_file, "ts_receiver", server_keys, true);

    // Get sender count and then fetch all sender configuration
    std::uint32_t sender_count = config_mgr::instance().get_uint32("senders.count");
    std::vector<std::string> sender_keys;

    // Sender config indexing starts from 1
    for (std::uint32_t i = 1; i < (sender_count + 1); i++)
    {
        sender_keys.push_back("senders." + std::to_string(i) + ".peer");
        sender_keys.push_back("senders." + std::to_string(i) + ".x_max");
        sender_keys.push_back("senders." + std::to_string(i) + ".x_res");
        sender_keys.push_back("senders." + std::to_string(i) + ".y_max");
        sender_keys.push_back("senders." + std::to_string(i) + ".y_res");
        sender_keys.push_back("senders." + std::to_string(i) + ".region_x1");
        sender_keys.push_back("senders." + std::to_string(i) + ".region_x2");
        sender_keys.push_back("senders." + std::to_string(i) + ".region_y1");
        sender_keys.push_back("senders." + std::to_string(i) + ".region_y2");
        sender_keys.push_back("senders." + std::to_string(i) + ".contacts_max");
    }

    // Update config storage with sender configuration
    config_mgr::instance().load(config_file, "ts_receiver", sender_keys, false);
}

int main(int argc, char* argv[])
{
    std::string config_path = "";

    if (argc < 2)
    {
        config_path = "ts_receiver.ini";
    }
    else
    {
        std::string arg1 = argv[1];
        if (arg1 == "help" || arg1 == "--help" || arg1 == "-h")
        {
            std::cout << "Usage: ts_receiver [config file = ts_receiver.ini]" << std::endl;
            std::cout << "Check ts_receiver.ini for more configuration info" << std::endl;
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
        load_config(config_path);
        touch_control::instance().initialize();
        boost::asio::io_service io_service;
        tcp_ts_server server(io_service, (unsigned short) config_mgr::instance().get_uint32("port"));
        
        std::cout << "TCP server initialized, beginning to listen..." << std::endl;
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}