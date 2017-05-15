#pragma once
#include "common.h"
#include "touch_structure.h"
#include <windows.h>

class mouse_injector
{
public:
    mouse_injector();
    
    void update_mouse_data(const std::vector<touch_data>& container);

    std::mutex& get_std_io_mutex()
    {
        static std::mutex std_io_mtx;
        return std_io_mtx;
    }
private:
    void release(std::uint32_t slot);
    void move(const touch_data& data);
    void pushdown(const touch_data& data);

    std::uint32_t free_contact_size;
    std::map<std::uint32_t, INPUT> active_inputs;
};