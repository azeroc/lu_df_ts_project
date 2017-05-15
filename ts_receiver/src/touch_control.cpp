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
        data.contacts_max = config_mgr::instance().get_uint32("senders." + std::to_string(i) + ".contacts_max");
        data.index = i;
        peer_config_storage[peer] = data;
    }

    host_x_res = config_mgr::instance().get_uint32("host_x_res");
    host_y_res = config_mgr::instance().get_uint32("host_y_res");
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

// Before we hand over data to OS-specific touch-simulation functions, we need to refine it
// (e.g. fix the x and y coordinates and assign proper slot)
void touch_control::handle_touch(const std::vector<touch_data>& peer_data, std::string peer)
{
    if (peer_config_storage.find(peer) == peer_config_storage.end())
    {
        get_std_io_mutex().lock();
        std::cerr << "Could not fetch configuration for peer " 
            << peer 
            << ", skipping touch data handling for it..."
            << std::endl;        
        get_std_io_mutex().unlock();
    }
    else
    {
        peer_config_data config = peer_config_storage[peer];
        std::vector<touch_data> refined_data;
        std::uint32_t index = config.index;

        for (touch_data item : peer_data)
        {
            touch_data refined_item;
            refined_item.slot = calculate_pointer_id(config.index, item.slot);
            refined_item.tracking_id = item.tracking_id;

            if (item.upd_flags | touch_data::update_flags::update_x)
            {
                refined_item.x = calculate_x(item.x, config.x_max, config.x_res, config.region_x1, config.region_x2);
            }

            if (item.upd_flags | touch_data::update_flags::update_y)
            {
                refined_item.y = calculate_y(item.y, config.y_max, config.y_res, config.region_y1, config.region_y2);
            }
            
            refined_item.upd_flags = item.upd_flags;
            refined_data.push_back(refined_item);
        }

        this->inject_touch_event(refined_data);
    }
}

std::uint32_t touch_control::calculate_x(std::uint32_t peer_x, std::uint32_t peer_x_max, std::uint32_t peer_x_res, std::uint32_t peer_region_x1, std::uint32_t peer_region_x2)
{
    // Convert parameters to double for the calculation
    double d_peer_x = peer_x;
    double d_peer_x_max = peer_x_max;
    double d_peer_x_res = peer_x_res;
    double d_peer_region_x1 = peer_region_x1;
    double d_peer_region_x2 = peer_region_x2;
    double d_region_delta = peer_region_x2 - peer_region_x1;
    double d_result = 0;

    // Calculate correct x coordinate of the remote peer
    double d_real_peer_x = (d_peer_x / d_peer_x_max) * d_peer_x_res;

    // Resize and offset d_real_peer_x to fit into defined region
    if (d_peer_x_res > d_region_delta)
    {
        d_result = (d_region_delta / d_peer_x_res) * d_real_peer_x + d_peer_region_x1;
    }
    else
    {
        d_result = (d_peer_x_res / d_region_delta) * d_real_peer_x + d_peer_region_x1;
    }

    // Return refined coordinate
    return static_cast<std::uint32_t>(d_result);
}

std::uint32_t touch_control::calculate_y(std::uint32_t peer_y, std::uint32_t peer_y_max, std::uint32_t peer_y_res, std::uint32_t peer_region_y1, std::uint32_t peer_region_y2)
{
    // Convert parameters to double for the calculation
    double d_peer_y = peer_y;
    double d_peer_y_max = peer_y_max;
    double d_peer_y_res = peer_y_res;
    double d_host_region_y1 = peer_region_y1;
    double d_host_region_y2 = peer_region_y2;
    double d_region_delta = peer_region_y2 - peer_region_y1;
    double d_result = 0;

    // Calculate correct x coordinate of the remote peer
    double d_real_peer_x = (d_peer_y / d_peer_y_max) * d_peer_y_res;

    // Resize and offset d_real_peer_x to fit into defined region
    if (d_peer_y_res > d_region_delta)
    {
        d_result = (d_region_delta / d_peer_y_res) * d_real_peer_x + d_host_region_y1;
    }
    else
    {
        d_result = (d_peer_y_res / d_region_delta) * d_real_peer_x + d_host_region_y1;
    }

    // Return refined coordinate
    return static_cast<std::uint32_t>(d_result);
}

std::uint32_t touch_control::calculate_pointer_id(std::uint32_t peer_index, std::uint32_t peer_slot)
{
    // Store receiver defined peer index in 4th byte of uint32, while using rest of the 3 for peer-side slot value
    return (peer_index << 24) + peer_slot;
}