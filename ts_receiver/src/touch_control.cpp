#include "touch_control.h"
#include "common.h"
#include "config_mgr.h"

void touch_control::initialize()
{
    // Get sender count and then fetch all sender configuration
    std::uint32_t sender_count = config_mgr::instance().get_uint32("senders.count");

    // Sender config indexing starts from 1
    for (std::uint32_t i = 1; i < (sender_count + 1); i++)
    {
        std::string peer;
        peer_config_data data;
        peer = config_mgr::instance().get_string("senders." + std::to_string(i) + ".peer");
        data.x_max = config_mgr::instance().get_uint32("senders." + std::to_string(i) + ".x_max");
        data.x_res = config_mgr::instance().get_uint32("senders." + std::to_string(i) + ".x_res");
        data.y_max = config_mgr::instance().get_uint32("senders." + std::to_string(i) + ".y_max");
        data.y_res = config_mgr::instance().get_uint32("senders." + std::to_string(i) + ".y_res");
        data.region_x1 = config_mgr::instance().get_uint32("senders." + std::to_string(i) + ".region_x1");
        data.region_x2 = config_mgr::instance().get_uint32("senders." + std::to_string(i) + ".region_x2");
        data.region_y1 = config_mgr::instance().get_uint32("senders." + std::to_string(i) + ".region_y1");
        data.region_y2 = config_mgr::instance().get_uint32("senders." + std::to_string(i) + ".region_y2");
        peer_config_storage[peer] = data;
    }
}

void touch_control::print_peer_config_data(std::string peer)
{
    if (peer_config_storage.find(peer) == peer_config_storage.end()) {
        std::cout << "touch_control: config storage key '" + peer + "' does not exist" << std::endl;
    }
    else {
        peer_config_data data = peer_config_storage[peer];
        std::cout << "touch_control: printing config of peer '" + peer + "' ..." << std::endl
            << "  config.x_max = " << data.x_max
            << ", config.x_res = " << data.x_res << std::endl
            << "  config.y_max = " << data.y_max
            << ", config.y_res = " << data.y_res << std::endl
            << "  config.region_x1 = " << data.region_x1
            << ", config.region_x2 = " << data.region_x2 << std::endl
            << "  config.region_y1 = " << data.region_y1
            << ", config.region_y2 = " << data.region_y2 << std::endl;
    }
}
