#pragma once
#include "common.h"
#include "touch_structure.h"
#include "touch_control.h"

#define INJECT_TOUCH_INPUT_IS_BROKEN
#ifdef WIN32
    #include "windows_impl/touch_injector.h"
    #include "windows_impl/mouse_injector.h"
#endif

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
        std::uint32_t contacts_max;
        std::uint32_t index;
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
    void handle_touch(const std::vector<touch_data>& peer_data, std::string peer);

    std::mutex& get_std_io_mutex()
    {
        static std::mutex std_io_mtx;
        return std_io_mtx;
    }
private:
    peer_config_map peer_config_storage;
    std::uint32_t host_x_res;
    std::uint32_t host_y_res;

#ifdef WIN32
    touch_injector win_injector_touch;
    mouse_injector win_injector_mouse;
#endif

    void inject_input_event(const std::vector<touch_data>& refined_data)
    {
#ifdef WIN32
#ifdef INJECT_TOUCH_INPUT_IS_BROKEN
        win_injector_mouse.update_mouse_data(refined_data);
#else
        win_injector_touch.update_touch_data(refined_data);
#endif
#endif
    }

    // Helper methods for touch_data refining
    std::uint32_t calculate_x(std::uint32_t peer_x, std::uint32_t peer_x_max, std::uint32_t peer_x_res, std::uint32_t peer_region_x1, std::uint32_t peer_region_x2);
    std::uint32_t calculate_y(std::uint32_t peer_y, std::uint32_t peer_y_max, std::uint32_t peer_y_res, std::uint32_t peer_region_y1, std::uint32_t peer_region_y2);
    std::uint32_t calculate_pointer_id(std::uint32_t peer_index, std::uint32_t peer_slot);
};