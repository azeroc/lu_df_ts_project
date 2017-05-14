#pragma once
#include "common.h"
#include "touch_structure.h"

class touch_control
{
public:
    struct peer_config_data
    {
        std::uint32_t x_max;
        std::uint32_t x_res;
        std::uint32_t y_max;
        std::uint32_t y_res;
        std::uint32_t region_x1;
        std::uint32_t region_x2;
        std::uint32_t region_y1;
        std::uint32_t region_y2;
    };

    typedef std::map<std::string, peer_config_data> peer_config_map;

    // Singleton touch_control instance
    static touch_control& instance()
    {
        static touch_control touch_ctrl;
        return touch_ctrl;
    }

    void initialize();
    void print_peer_config_data(std::string peer); // For debug purposes
private:
    peer_config_map peer_config_storage;
};